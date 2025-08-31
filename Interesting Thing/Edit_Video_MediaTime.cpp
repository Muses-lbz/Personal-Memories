#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <ctime>

// ExifTool path (quoted because of spaces)
const std::string EXIFTOOL_PATH = "\"D:/Program Files (x86)/Exiftool/exiftool-13.34_64/exiftool.exe\"";

// Log file name
const std::string LOG_FILE = "log.txt";

// Video extensions
static std::vector<std::string> exts = { ".mp4", ".mov", ".avi", ".mkv" };

// Check extension
static bool hasVideoExt(const std::string& filename) {
    for (const auto& ext : exts) {
        if (filename.size() >= ext.size()) {
            std::string tail = filename.substr(filename.size() - ext.size());
            for (auto& c : tail) c = tolower(c);
            std::string lowerExt = ext;
            for (auto& c : lowerExt) c = tolower(c);
            if (tail == lowerExt) return true;
        }
    }
    return false;
}

// Convert Y-M-D H:M:S to time_t
static time_t makeTime(int year, int month, int day, int hour, int min, int sec) {
    struct tm t = {};
    t.tm_year = year - 1900;
    t.tm_mon = month - 1;
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min = min;
    t.tm_sec = sec;
    return mktime(&t);
}

// Generate random datetime between 2023-01-01 and 2025-12-31
static std::string randomDateTime() {
    static std::random_device rd;
    static std::mt19937_64 gen(rd());

    time_t start = makeTime(2023, 1, 1, 0, 0, 0);
    time_t end = makeTime(2025, 12, 31, 23, 59, 59);

    std::uniform_int_distribution<long long> dist(start, end);
    time_t randomTime = (time_t)dist(gen);

    struct tm tmBuf;
    localtime_s(&tmBuf, &randomTime);

    char buffer[32];
    sprintf_s(buffer, "%04d:%02d:%02d %02d:%02d:%02d",
        tmBuf.tm_year + 1900,
        tmBuf.tm_mon + 1,
        tmBuf.tm_mday,
        tmBuf.tm_hour,
        tmBuf.tm_min,
        tmBuf.tm_sec);

    return std::string(buffer);
}

// Call ExifTool and log result
static void modifyVideoDate(const std::string& filePath, std::ofstream& log) {
    std::string newDate = randomDateTime();
    std::string cmd = EXIFTOOL_PATH + " -overwrite_original "
        "\"-CreateDate=" + newDate + "\" "
        "\"-ModifyDate=" + newDate + "\" "
        "\"" + filePath + "\"";

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    char cmdLine[4096];
    strcpy_s(cmdLine, cmd.c_str());

    if (CreateProcessA(
        NULL, cmdLine, NULL, NULL, FALSE,
        CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
    {
        WaitForSingleObject(pi.hProcess, INFINITE);
        DWORD exitCode;
        GetExitCodeProcess(pi.hProcess, &exitCode);

        std::cout << "[OK] " << filePath
            << " -> " << newDate
            << " (exit=" << exitCode << ")" << std::endl;

        log << "[OK] " << filePath << " -> " << newDate
            << " (exit=" << exitCode << ")" << std::endl;

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else {
        std::cerr << "[FAIL] " << filePath << std::endl;
        log << "[FAIL] " << filePath << std::endl;
    }
}

// Recursive directory traversal
static void processDirectory(const std::string& dirPath, std::ofstream& log) {
    WIN32_FIND_DATAA ffd;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    std::string searchPath = dirPath + "\\*";

    hFind = FindFirstFileA(searchPath.c_str(), &ffd);
    if (hFind == INVALID_HANDLE_VALUE) return;

    do {
        std::string name = ffd.cFileName;
        if (name == "." || name == "..") continue;

        std::string fullPath = dirPath + "\\" + name;

        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            processDirectory(fullPath, log);
        }
        else {
            if (hasVideoExt(name)) {
                modifyVideoDate(fullPath, log);
            }
        }
    } while (FindNextFileA(hFind, &ffd) != 0);

    FindClose(hFind);
}

int main() {
    std::string targetDir = "E:/Documents Beauty/Return Files";  // video folder
    std::cout << "Start processing folder: " << targetDir << std::endl;

    std::ofstream log(LOG_FILE, std::ios::app);  // append mode
    if (!log) {
        std::cerr << "Cannot open log file: " << LOG_FILE << std::endl;
        return 1;
    }

    log << "==== Start processing folder: " << targetDir << " ====" << std::endl;

    processDirectory(targetDir, log);

    log << "==== Done ====" << std::endl;
    log.close();

    std::cout << "All done. Log written to " << LOG_FILE << std::endl;
    return 0;
}  

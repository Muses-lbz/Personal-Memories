#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <regex>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <chrono>
#include <ctime>

namespace fs = std::filesystem;

bool isImageFile(const fs::path& filePath) {
    std::regex imagePattern(R"((\.mp4)$)", std::regex_constants::icase);
    return std::regex_search(filePath.extension().string(), imagePattern);
}

// 获取当前日期时间字符串
std::string getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm localTime;
#ifdef _WIN32
    localtime_s(&localTime, &now_time);  // Windows 平台
#else
    localtime_r(&localTime, &now_time);  // Linux/Unix 平台
#endif
    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y%m%d%H%M%S"); // 格式：年月日时分秒
    return oss.str();
}

// 获取新的文件名
std::string getNewFileName(int index) {
    std::ostringstream oss;
    std::string dateTime = getCurrentDateTime();
    oss << "Video_Beauty_" << dateTime << "_" << index << ".mp4";
    return oss.str();
}

// 读取current_number.txt中的编号
int readCurrentNumber(const fs::path& filePath) {
    std::ifstream inFile(filePath);
    int number = 10001; // 默认起始编号
    if (inFile) {
        inFile >> number;
    }
    return number;
}

// 将编号写入current_number.txt
void writeCurrentNumber(const fs::path& filePath, int number) {
    std::ofstream outFile(filePath);
    outFile << number;
}

// 递归遍历并重命名文件
void renameAndMoveFiles(const fs::path& directory, int& index, const fs::path& targetDirectory) {
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (fs::is_directory(entry.path())) {
            // 如果是子目录，递归调用
            renameAndMoveFiles(entry.path(), index, targetDirectory);
        }
        else if (fs::is_regular_file(entry.path()) && isImageFile(entry.path())) {
            std::string newFileName = getNewFileName(index);
            fs::path newFilePath = targetDirectory / newFileName; // 目标文件夹中的新路径
            fs::rename(entry.path(), newFilePath); // 重命名并移动文件
            index++;
        }
    }
}

int main() {
    fs::path sourceDirectory = "E:/Documents Beauty/Return Files"; // 源目录路径
    fs::path targetDirectory = "E:/Documents Beauty/Return Folder"; // 目标目录路径
    fs::path currentNumberFile = "current_number.txt"; // 记录当前编号的文件

    try {
        if (fs::exists(sourceDirectory) && fs::is_directory(sourceDirectory) &&
            fs::exists(targetDirectory) && fs::is_directory(targetDirectory)) {
            int startIndex = readCurrentNumber(currentNumberFile); // 从文件中读取起始编号
            renameAndMoveFiles(sourceDirectory, startIndex, targetDirectory); // 重命名并移动文件
            writeCurrentNumber(currentNumberFile, startIndex); // 保存最新的编号
            std::cout << "Renamed And Moved Successfully." << std::endl;
        }
        else {
            std::cerr << "The Specified Source Or Target Path Does Not Exist Or Is Not A Directory." << std::endl;
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

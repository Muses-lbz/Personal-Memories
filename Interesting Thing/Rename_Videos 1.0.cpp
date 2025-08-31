#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <regex>
#include <iomanip>
#include <sstream>
#include <fstream>

namespace fs = std::filesystem;

bool isImageFile(const fs::path& filePath) {
    std::regex imagePattern(R"((\.mp4)$)", std::regex_constants::icase);
    return std::regex_search(filePath.extension().string(), imagePattern);
}

// 获取新的文件名
std::string getNewFileName(int index) {
    std::ostringstream oss;
    oss << "Video_Files_Beauty_" << index << ".mp4";
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
void renameImages(const fs::path& directory, int& index) {
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (fs::is_directory(entry.path())) {
            // 如果是子目录，递归调用
            renameImages(entry.path(), index);
        }
        else if (fs::is_regular_file(entry.path()) && isImageFile(entry.path())) {
            fs::path newFilePath = directory / getNewFileName(index);
            fs::rename(entry.path(), newFilePath);
            /*std::cout << "Renamed: " << entry.path().string() << std::endl
                << " -> " << std::endl
                << "New Name: " << newFilePath.string() << std::endl << std::endl;*/
            index++;
        }
    }
}

int main() {
    fs::path targetDirectory = "E:/Convert Documents/Convert Files"; // 目标目录路径
    fs::path currentNumberFile = "current_number.txt"; // 记录当前编号的文件

    try {
        if (fs::exists(targetDirectory) && fs::is_directory(targetDirectory)) {
            int startIndex = readCurrentNumber(currentNumberFile); // 从文件中读取起始编号
            renameImages(targetDirectory, startIndex);
            writeCurrentNumber(currentNumberFile, startIndex); // 保存最新的编号
            std::cout << "Renamed successfully." << std::endl;
        }
        else {
            std::cerr << "The specified path does not exist or is not a directory." << std::endl;
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

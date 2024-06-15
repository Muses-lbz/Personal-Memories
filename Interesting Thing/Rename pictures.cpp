#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <regex>
#include <iomanip>
#include <sstream>

namespace fs = std::filesystem;

// 判断文件是否是图片（根据文件扩展名）
bool isImageFile(const fs::path& filePath) {
    std::regex imagePattern(R"((\.jpg|\.jpeg|\.png|\.bmp|\.gif)$)", std::regex_constants::icase);
    return std::regex_search(filePath.extension().string(), imagePattern);
}

// 获取新的文件名
std::string getNewFileName(int index) {
    std::ostringstream oss;
    oss << "image_" << std::setw(4) << std::setfill('0') << index << ".jpg"; // 假设所有图片都重命名为jpg格式
    return oss.str();
}

// 递归遍历并重命名文件
void renameImages(const fs::path& directory, int& index) {
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (fs::is_directory(entry.path())) {
            // 如果是子目录，递归调用
            renameImages(entry.path(), index);
        }
        else if (fs::is_regular_file(entry.path()) && isImageFile(entry.path())) {
            // 如果是图片文件，重命名
            fs::path newFilePath = directory / getNewFileName(index);
            fs::rename(entry.path(), newFilePath);
            std::cout << "Renamed: " << entry.path().string() << " -> " << newFilePath.string() << std::endl;
            index++;
        }
    }
}

int main() {
    fs::path targetDirectory = "E:/Convert Documents/Convert Files"; // 替换为你的目标目录路径
    int startIndex = 1; // 重命名的起始索引

    try {
        if (fs::exists(targetDirectory) && fs::is_directory(targetDirectory)) {
            renameImages(targetDirectory, startIndex);
            std::cout << "All images have been renamed successfully." << std::endl;
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

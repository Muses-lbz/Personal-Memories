#include <iostream>
#include <string>
#include <filesystem>
#include <ctime>
#include <iomanip> // 用于格式化输出

namespace fs = std::filesystem;

void renameImagesInFolder(const std::string& folderPath, int startNumber) {
    int currentNumber = startNumber;
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            std::string extension = entry.path().extension().string();
            // 只处理图片文件，你可以根据需要添加其他图片格式的判断条件
            if (extension == ".jpg" || extension == ".png" || extension == ".jpeg" || extension == ".bmp") {
                // 获取当前日期时间作为前缀
                time_t now = time(nullptr);
                struct tm timeinfo;
                localtime_s(&timeinfo, &now);
                char datePrefix[15]; // 格式化为 YYYYMMDDHHMMSS
                strftime(datePrefix, sizeof(datePrefix), "%Y%m%d%H%M%S", &timeinfo);

                // 组合新的文件名
                std::string newFileName = folderPath + "/" + std::string(datePrefix) + "_" + std::to_string(currentNumber) + extension;
                fs::rename(entry.path(), newFileName);
                ++currentNumber;
            }
        }
    }
}

void moveImagesToFolder(const std::string& sourceFolderPath, const std::string& targetFolderPath) {
    // 创建目标文件夹，如果不存在的话
    fs::create_directory(targetFolderPath);

    for (const auto& entry : fs::directory_iterator(sourceFolderPath)) {
        if (entry.is_regular_file()) {
            std::string extension = entry.path().extension().string();
            // 只处理图片文件，你可以根据需要添加其他图片格式的判断条件
            if (extension == ".jpg" || extension == ".png" || extension == ".jpeg" || extension == ".bmp") {
                // 获取文件名
                std::string fileName = entry.path().filename().string();
                // 拼接目标路径
                std::string targetPath = targetFolderPath + "/" + fileName;
                // 移动文件
                fs::rename(entry.path(), targetPath);
            }
        }
    }
    std::cout << "All images moved to target folder." << std::endl;
}

int main() {
    std::string folderPath = "E:/Convert Files"; // 指定文件夹路径
    std::string targetFolderPath = "E:/Convert Folder"; // 指定目标文件夹路径
    int startNumber = 10107; // 指定起始序号

    renameImagesInFolder(folderPath, startNumber);

    std::cout << "Image renaming completed." << std::endl;

    std::cout << "Enter 1 to move all images: ";
    
    int choice;

    std::cin >> choice;

    if (choice == 1) {
        // 移动图片到指定文件夹
        moveImagesToFolder(folderPath, targetFolderPath);
    }

    return 0;
}

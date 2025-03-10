#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <algorithm>

namespace fs = std::filesystem;

// 获取当前时间字符串（YYYYMMDDHHMMSS）
std::string getCurrentTimestamp() {
    time_t now = time(nullptr);
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);
    char timestamp[15];
    strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", &timeinfo);
    return std::string(timestamp);
}

// 结构体存储文件路径和修改时间
struct FileInfo {
    fs::path path;
    std::time_t lastWriteTime;
};

// **批量重命名并移动**
void processImages(const std::string& sourceFolder, const std::string& targetFolder) {
    std::vector<FileInfo> images;

    // 遍历文件夹，收集所有图片
    for (const auto& entry : fs::recursive_directory_iterator(sourceFolder)) {
        if (entry.is_regular_file()) {
            std::string extension = entry.path().extension().string();
            if (extension == ".jpg" || extension == ".png" || extension == ".jpeg" || extension == ".bmp") {
                images.push_back({ entry.path(), fs::last_write_time(entry.path()).time_since_epoch().count() });
            }
        }
    }

    // 按修改时间排序，确保重命名顺序正确
    std::sort(images.begin(), images.end(), [](const FileInfo& a, const FileInfo& b) {
        return a.lastWriteTime < b.lastWriteTime;
        });

    int currentNumber = 1; // **图片编号从 1 开始**
    int folderIndex = 0;   // **文件夹编号从 1 开始**
    int imageCount = 0;    // 记录当前子文件夹内图片数量

    // **批量重命名并移动**
    for (const auto& file : images) {
        std::string timestamp = getCurrentTimestamp(); // 获取当前时间
        std::ostringstream newFileName;
        newFileName << timestamp << "_" << std::setw(5) << std::setfill('0') << currentNumber << file.path.extension().string();

        // **计算存储子文件夹**
        if (imageCount % 200 == 0) {
            folderIndex++;
        }
        std::string subFolder = targetFolder + "/Picture_" + std::to_string(folderIndex);
        fs::create_directory(subFolder);  // 确保子文件夹存在

        // **目标文件路径**
        fs::path targetPath = subFolder + "/" + newFileName.str();

        // **执行重命名并移动**
        fs::rename(file.path, targetPath);

        currentNumber++;  // 递增编号
        imageCount++;     // 计数当前子文件夹内图片
    }

    std::cout << "图片处理完成，总计处理 " << images.size() << " 张图片！" << std::endl;
}

int main() {
    std::string sourceFolder = "E:/Documents Beauty/Convert Files";  // 图片源文件夹
    std::string targetFolder = "E:/Documents Beauty/Convert Folder"; // 目标存储文件夹

    processImages(sourceFolder, targetFolder); // 处理图片

    return 0;
}

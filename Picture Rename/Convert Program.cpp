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

int main() {
    std::string folderPath = "E:/Convert Files"; // 指定文件夹路径
    int startNumber = 10001; // 指定起始序号

    renameImagesInFolder(folderPath, startNumber);

    std::cout << "Image renaming completed." << std::endl;
    return 0;
}

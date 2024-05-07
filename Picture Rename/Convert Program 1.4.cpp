#include <iostream>
#include <fstream> // 用于文件操作
#include <string>
#include <filesystem>
#include <vector>
#include <ctime>
#include <iomanip> // 用于格式化输出

namespace fs = std::filesystem;

void createFileIfNotExists(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.good()) {
        std::ofstream newFile(filePath);
        newFile.close();
    }
}

int readLastNumberFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    int lastNumber = 0;
    if (file.is_open()) {
        file >> lastNumber;
        file.close();
    }
    return lastNumber;
}

void writeNumberToFile(const std::string& filePath, int number) {
    std::ofstream file(filePath);
    if (file.is_open()) {
        file << number;
        file.close();
    }
}

void writeDateToFile(const std::string& filePath, const std::string& date) {
    std::ofstream file(filePath);
    if (file.is_open()) {
        file << date;
        file.close();
    }
}

std::string readDateFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::string savedDate;
    if (file.is_open()) {
        file >> savedDate;
        file.close();
    }
    return savedDate;
}

void resetNumberIfNeeded(int startNumber, const std::string& numberFilePath, const std::string& dateFilePath) {
    // 读取当前日期
    time_t now = time(nullptr);
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);
    char currentDate[15]; // 格式化为 YYYYMMDD
    strftime(currentDate, sizeof(currentDate), "%Y%m%d", &timeinfo);

    // 读取之前保存的日期
    std::string savedDate = readDateFromFile(dateFilePath);

    // 如果日期发生了变化，则重置序号为起始序号
    if (savedDate != currentDate) {
        // 删除 current_number.txt 文件并重新创建
        fs::remove(numberFilePath);
        createFileIfNotExists(numberFilePath);
        // 设置 currentNumber 为起始序号
        writeNumberToFile(numberFilePath, startNumber); // 写入起始序号到文件
        writeDateToFile(dateFilePath, currentDate); // 写入当前日期到文件
    }
}

void renameImagesInFolder(const std::string& folderPath, int startNumber, const std::string& numberFilePath, const std::string& dateFilePath) {
    resetNumberIfNeeded(startNumber, numberFilePath, dateFilePath); // 检查日期并重置数据

    int currentNumber = readLastNumberFromFile(numberFilePath); // 从文件中读取上次的序号

    for (const auto& entry : fs::recursive_directory_iterator(folderPath)) {
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
                std::string newFileName = entry.path().parent_path().string() + "/" + std::string(datePrefix) + "_" + std::to_string(currentNumber) + extension;
                fs::rename(entry.path(), newFileName);

                // 递增 currentNumber
                ++currentNumber;
            }
        }
    }
    // 更新最后的序号到文件
    writeNumberToFile(numberFilePath, currentNumber);
}

void moveImagesToFolder(const std::string& sourceFolderPath, const std::string& targetFolderPath) {
    // 创建目标文件夹，如果不存在的话
    fs::create_directory(targetFolderPath);

    // 存储目标文件夹中已有的文件名
    std::vector<std::string> targetFileNames;
    for (const auto& entry : fs::recursive_directory_iterator(targetFolderPath)) {
        if (entry.is_regular_file()) {
            targetFileNames.push_back(entry.path().filename().string());
        }
    }

    for (const auto& entry : fs::recursive_directory_iterator(sourceFolderPath)) {
        if (entry.is_regular_file()) {
            std::string extension = entry.path().extension().string();
            // 只处理图片文件，你可以根据需要添加其他图片格式的判断条件
            if (extension == ".jpg" || extension == ".png" || extension == ".jpeg" || extension == ".bmp") {
                // 获取文件名
                std::string fileName = entry.path().filename().string();
                // 检查文件名是否已经存在于目标文件夹中
                if (std::find(targetFileNames.begin(), targetFileNames.end(), fileName) != targetFileNames.end()) {
                    std::cout << "Duplicate file name found: " << fileName << std::endl;
                    std::cout << "Aborting operation." << std::endl;
                    return;
                }
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
    std::string folderPath = "E:/Convert Documents/Convert Files"; // 指定文件夹路径
    std::string targetFolderPath = "E:/Convert Documents/Convert Folder"; // 指定目标文件夹路径
    std::string numberFilePath = "current_number.txt"; // 记录当前序号的文件路径
    std::string dateFilePath = "current_date.txt"; // 记录当前日期的文件路径
    int startNumber = 10001; // 指定起始序号

    createFileIfNotExists(numberFilePath); // 检查并创建记录序号的文件
    createFileIfNotExists(dateFilePath); // 检查并创建记录日期的文件

    renameImagesInFolder(folderPath, startNumber, numberFilePath, dateFilePath); // 重命名图片并更新序号

    std::cout << "Image renaming completed." << std::endl;

    //std::cout << "Enter 1 to move all images: ";

    int choice;

    //std::cin >> choice;
    choice = 1;

    if (choice == 1) {
        // 移动图片到指定文件夹
        moveImagesToFolder(folderPath, targetFolderPath);
    }

    return 0;
}

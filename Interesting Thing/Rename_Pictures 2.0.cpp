//重命名格式：文件夹名称_序号
#include <iostream>
#include <filesystem>
#include <regex>
#include <iomanip>
#include <sstream>

namespace fs = std::filesystem;

// 判断文件是否是图片
bool isImageFile(const fs::path& filePath) {
    std::regex imagePattern(R"((\.jpg|\.jpeg|\.png|\.bmp|\.gif)$)", std::regex_constants::icase);
    return std::regex_search(filePath.extension().string(), imagePattern);
}

// 获取新的文件名（用文件夹名作为前缀）
std::string getNewFileName(const std::string& folderName, int index) {
    std::ostringstream oss;
    oss << folderName << " _"
        << std::setw(4) << std::setfill('0') << index << ".jpg";
    return oss.str();
}

// 遍历重命名并移动
void renameAndMoveImages(const fs::path& directory,
    const fs::path& outputDir,
    const std::string& folderName,
    int& index) {
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (fs::is_directory(entry.path())) {
            renameAndMoveImages(entry.path(), outputDir, folderName, index);
        }
        else if (fs::is_regular_file(entry.path()) && isImageFile(entry.path())) {
            fs::path newFileName = getNewFileName(folderName, index);
            fs::path finalPath = outputDir / newFileName;

            fs::rename(entry.path(), finalPath);

            /*std::cout << "Moved: " << entry.path().string()
                << " -> " << finalPath.string() << std::endl;*/

            index++;
        }
    }
}

int main() {
    fs::path targetDirectory = "E:/Documents Beauty/Convert Files"; // 原始目录
    fs::path outputRoot = "E:/Documents Beauty/Convert Folder"; // 输出目录

    try {
        // 1. 找到原始目录下的第一个文件夹
        fs::path subFolder;
        for (const auto& entry : fs::directory_iterator(targetDirectory)) {
            if (fs::is_directory(entry.path())) {
                subFolder = entry.path();
                break;
            }
        }

        if (subFolder.empty()) {
            std::cerr << "No subfolder found in " << targetDirectory << std::endl;
            return 1;
        }

        // 2. 获取文件夹名称
        std::string folderName = subFolder.filename().string();

        // 3. 在输出目录里创建同名子文件夹
        fs::path outputDir = outputRoot / folderName;
        if (!fs::exists(outputDir)) {
            fs::create_directories(outputDir);
        }

        // 4. 重命名并移动
        int startIndex = 1;
        renameAndMoveImages(subFolder, outputDir, folderName, startIndex);

        std::cout << "All Images Renamed And Moved Into " << std::endl 
            << outputDir.string() << std::endl;
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

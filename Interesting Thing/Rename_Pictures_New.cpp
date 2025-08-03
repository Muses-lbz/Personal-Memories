#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <regex>
#include <iomanip>
#include <sstream>
#include <algorithm>

namespace fs = std::filesystem;

// 判断文件是否是图片
bool isImageFile(const fs::path& filePath) {
    std::regex imagePattern(R"((\.jpg|\.jpeg|\.png|\.bmp|\.gif)$)", std::regex_constants::icase);
    return std::regex_search(filePath.extension().string(), imagePattern);
}

// 生成统一的新文件名
std::string getNewFileName(int index) {
    std::ostringstream oss;
    oss << "Image_" << std::setw(4) << std::setfill('0') << index << ".jpg";
    return oss.str();
}

// 第一步：收集所有图片路径，不动文件
void collectImageFiles(const fs::path& directory, std::vector<fs::path>& imageFiles) {
    try {
        for (const auto& entry : fs::recursive_directory_iterator(directory, fs::directory_options::follow_directory_symlink)) {
            if (fs::is_regular_file(entry.path()) && isImageFile(entry.path())) {
                imageFiles.push_back(entry.path());
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "遍历错误：" << e.what() << std::endl;
    }
}

// 第二步：统一重命名，不再修改结构
void renameImagesSafely(const std::vector<fs::path>& imageFiles, int& index) {
    bool hasShownConflictWarning = false; // 用于标记是否已经输出过一次提示

    for (const auto& filePath : imageFiles) {
        fs::path newFilePath = filePath.parent_path() / getNewFileName(index);

        if (fs::exists(newFilePath)) {
            if (!hasShownConflictWarning) {
                std::cerr << "跳过：检测到目标文件已存在，后续重复冲突将不再提示。" << std::endl;
                hasShownConflictWarning = true;
            }
            continue;
        }

        try {
            fs::rename(filePath, newFilePath);
            //std::cout << "已重命名：" << filePath << " -> " << newFilePath << std::endl;
            index++;
        }
        catch (const fs::filesystem_error& e) {
            std::cerr << "重命名失败：" << filePath << "，错误：" << e.what() << std::endl;
        }
    }
}


int main() {
    fs::path targetDirectory = "E:/Documents Beauty/Convert Files"; // 替换为你的路径
    int startIndex = 1;

    try {
        if (fs::exists(targetDirectory) && fs::is_directory(targetDirectory)) {
            std::vector<fs::path> imageFiles;

            // 第一步：完整收集所有图片
            collectImageFiles(targetDirectory, imageFiles);

            // 可选：排序（按文件名），保持一致性
            std::sort(imageFiles.begin(), imageFiles.end());

            // 第二步：重命名
            renameImagesSafely(imageFiles, startIndex);

            std::cout << "所有图片已重命名完成。" << std::endl;
        }
        else {
            std::cerr << "指定路径不存在或不是目录。" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "异常：" << e.what() << std::endl;
    }

    return 0;
}

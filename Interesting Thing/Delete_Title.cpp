#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <filesystem>

namespace fs = std::filesystem;

void clearTitleFormatting(const fs::path& inputPath, const fs::path& outputPath) {
    std::ifstream inFile(inputPath);
    std::ofstream outFile(outputPath);

    if (!inFile || !outFile) {
        std::cerr << "无法打开文件：" << inputPath << " 或 " << outputPath << std::endl;
        return;
    }

    std::string line;
    std::regex titleRegex("^#+\\s*");  // 匹配以#开头的标题
    bool inCodeBlock = false;  // 标记是否在代码块中

    while (std::getline(inFile, line)) {
        // 检测代码块的开始和结束
        if (line.find("```") != std::string::npos) {
            inCodeBlock = !inCodeBlock;  // 切换代码块状态
        }

        // 如果不在代码块内，处理标题
        if (!inCodeBlock) {
            // 删除行首的标题符号
            line = std::regex_replace(line, titleRegex, "");
        }

        // 输出到新文件
        outFile << line << std::endl;
    }

    inFile.close();
    outFile.close();

    //std::cout << "文件 " << inputPath << " 已处理并保存到 " << outputPath << std::endl;
    std::cout << "文件修正成功!" << std::endl;
}

int main() {
    std::string folderPath = "E:/CpRena Documents/CpRena Files";  // 替换为你的文件夹路径
    fs::path folder(folderPath);

    if (!fs::is_directory(folder)) {
        std::cerr << "指定路径不是一个有效的文件夹：" << folderPath << std::endl;
        return 1;
    }

    for (const auto& entry : fs::directory_iterator(folder)) {
        if (entry.is_regular_file() && entry.path().extension() == ".md") {
            fs::path inputFile = entry.path();
            fs::path outputFile = entry.path();
            outputFile.replace_extension();  // 删除扩展名
            outputFile += "_output.md";  // 添加 "_output" 后缀

            clearTitleFormatting(inputFile, outputFile);
        }
    }

    return 0;
}

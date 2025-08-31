#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

namespace fs = std::filesystem;

// 判断是否视频文件
bool isVideoFile(const fs::path& path) {
    static const std::vector<std::wstring> exts = { L".mp4", L".mkv", L".avi", L".mov", L".flv", L".wmv" };
    std::wstring ext = path.extension().wstring();
    for (auto& e : exts) {
        if (ext == e) return true;
    }
    return false;
}

// 提取前 n 个汉字，保持原文件名中汉字顺序
std::wstring extractChinese(const std::wstring& name, size_t n) {
    std::wstring result;
    for (wchar_t wc : name) {
        // 汉字 Unicode 范围 0x4E00 - 0x9FFF
        if (wc >= 0x4E00 && wc <= 0x9FFF) {
            result += wc;
            if (result.size() >= n) break;
        }
    }

    // 如果提取后不足 n 个汉字，则保留原文件名所有汉字（保持顺序）
    if (result.size() < n) {
        result.clear();
        for (wchar_t wc : name) {
            if (wc >= 0x4E00 && wc <= 0x9FFF) {
                result += wc;
            }
        }
    }

    return result;
}

int main() {
    fs::path sourceDir = L"E:/Documents Beauty/Return Files";
    fs::path targetDir = L"E:/Documents Beauty/Return Folder";

    if (!fs::exists(targetDir)) {
        fs::create_directories(targetDir);
    }

    int counter = 10166;

    for (auto& p : fs::recursive_directory_iterator(sourceDir)) {
        if (fs::is_regular_file(p) && isVideoFile(p.path())) {
            std::wstring originalName = p.path().stem().wstring();
            std::wstring chinese = extractChinese(originalName, 15);

            std::wostringstream newName;
            newName << L"DIY Word_" << chinese << L"_"
                << std::setw(5) << std::setfill(L'0') << counter
                << p.path().extension().wstring();

            fs::path newPath = targetDir / newName.str();
            std::error_code ec; // 防止异常
            fs::rename(p.path(), newPath, ec);
            /*if (ec) {
                std::wcout << L"重命名失败: " << p.path().wstring()
                    << L" -> " << newPath.wstring()
                    << L" 错误: " << ec.message().c_str() << std::endl;
            }
            else {
                std::wcout << L"重命名成功: " << newPath.wstring() << std::endl;
            }*/

            counter++;
        }
    }

    std::cout << "All Videos Are Renamed." << std::endl;

    return 0;
}

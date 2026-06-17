#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <string>
#include <cctype>

namespace fs = std::filesystem;

// 判断是否为图片
bool IsImageFile(const fs::path& file)
{
    if (!file.has_extension())
        return false;

    std::string ext = file.extension().string();

    std::transform(
        ext.begin(),
        ext.end(),
        ext.begin(),
        [](unsigned char c)
        {
            return static_cast<char>(std::tolower(c));
        });

    return ext == ".jpg" ||
        ext == ".jpeg" ||
        ext == ".png" ||
        ext == ".bmp" ||
        ext == ".webp" ||
        ext == ".gif" ||
        ext == ".tif" ||
        ext == ".tiff";
}

int main()
{
    std::string folderStr;
    std::string prefix;
    std::string recursiveInput;
    size_t startNumber;

    std::cout << "请输入图片目录：";
    std::getline(std::cin, folderStr);

    std::cout << "请输入文件名前缀：";
    std::getline(std::cin, prefix);

    if (prefix.empty())
        prefix = "Interest";

    std::cout << "请输入起始编号：";
    std::cin >> startNumber;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "是否递归处理子目录(Y/N)：";
    std::getline(std::cin, recursiveInput);

    bool recursive =
        !recursiveInput.empty() &&
        (recursiveInput[0] == 'Y' ||
            recursiveInput[0] == 'y');

    fs::path folder(folderStr);

    if (!fs::exists(folder))
    {
        std::cout << "目录不存在！\n";
        return 1;
    }

    if (!fs::is_directory(folder))
    {
        std::cout << "指定路径不是目录！\n";
        return 1;
    }

    std::vector<fs::path> images;

    try
    {
        if (recursive)
        {
            for (const auto& entry :
                fs::recursive_directory_iterator(folder))
            {
                if (entry.is_regular_file() &&
                    IsImageFile(entry.path()))
                {
                    images.push_back(entry.path());
                }
            }
        }
        else
        {
            for (const auto& entry :
                fs::directory_iterator(folder))
            {
                if (entry.is_regular_file() &&
                    IsImageFile(entry.path()))
                {
                    images.push_back(entry.path());
                }
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "扫描失败："
            << e.what()
            << '\n';
        return 1;
    }

    if (images.empty())
    {
        std::cout << "未发现图片文件。\n";
        return 0;
    }

    // 排序
    std::sort(
        images.begin(),
        images.end(),
        [](const fs::path& a, const fs::path& b)
        {
            return a.string() < b.string();
        });

    std::cout << "\n找到 "
        << images.size()
        << " 张图片。\n";

    // 第一阶段：临时名称
    std::vector<fs::path> tempFiles;

    for (size_t i = 0; i < images.size(); ++i)
    {
        fs::path tempPath =
            images[i].parent_path() /
            ("__TEMP_RENAME__" +
                std::to_string(i) +
                images[i].extension().string());

        try
        {
            fs::rename(images[i], tempPath);
            tempFiles.push_back(tempPath);
        }
        catch (const std::exception& e)
        {
            std::cout << "失败："
                << images[i]
                << "\n"
                << e.what()
                << '\n';
            return 1;
        }
    }

    // 第二阶段：正式名称
    for (size_t i = 0; i < tempFiles.size(); ++i)
    {
        size_t currentNumber = startNumber + i;

        fs::path newPath =
            tempFiles[i].parent_path() /
            (prefix +
                "_" +
                std::to_string(currentNumber) +
                tempFiles[i].extension().string());

        try
        {
            fs::rename(tempFiles[i], newPath);

            std::cout
                << "[" << (i + 1)
                << "/" << tempFiles.size()
                << "] "
                << newPath.filename().string()
                << '\n';
        }
        catch (const std::exception& e)
        {
            std::cout << "失败："
                << tempFiles[i]
                << "\n"
                << e.what()
                << '\n';
            return 1;
        }
    }

    std::cout
        << "\n完成！共重命名 "
        << tempFiles.size()
        << " 个文件。\n";

    return 0;
}

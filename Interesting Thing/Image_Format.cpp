#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <filesystem>
#include <algorithm>
#include <vector>
#include <string>

#pragma warning(push)
#pragma warning(disable:4244)
#pragma warning(disable:4267)

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#pragma warning(pop)


namespace fs = std::filesystem;


// =====================================================
// RGBA 转 RGB，并使用白色填充透明区域
// =====================================================
unsigned char* rgbaToRgbWhiteBackground(
    const unsigned char* rgba,
    int width,
    int height)
{
    unsigned char* rgb =
        new unsigned char[width * height * 3];


    for (int i = 0; i < width * height; i++)
    {
        unsigned char r = rgba[i * 4 + 0];
        unsigned char g = rgba[i * 4 + 1];
        unsigned char b = rgba[i * 4 + 2];
        unsigned char a = rgba[i * 4 + 3];


        float alpha = a / 255.0f;


        rgb[i * 3 + 0] =
            static_cast<unsigned char>(
                r * alpha +
                255 * (1 - alpha));


        rgb[i * 3 + 1] =
            static_cast<unsigned char>(
                g * alpha +
                255 * (1 - alpha));


        rgb[i * 3 + 2] =
            static_cast<unsigned char>(
                b * alpha +
                255 * (1 - alpha));
    }


    return rgb;
}



// =====================================================
// 单张图片转换
// =====================================================
bool convertImage(
    const fs::path& inputPath,
    int quality = 95)
{

    int width = 0;
    int height = 0;
    int channels = 0;


    unsigned char* image =
        stbi_load(
            inputPath.string().c_str(),
            &width,
            &height,
            &channels,
            0);


    if (!image)
    {
        std::cout
            << "[读取失败] "
            << inputPath
            << "\n";

        return false;
    }



    fs::path outputPath =
        inputPath.parent_path()
        /
        (inputPath.stem().string()
            + ".jpg");



    // 已经转换过，跳过
    if (fs::exists(outputPath))
    {
        std::cout
            << "[跳过] "
            << outputPath.filename()
            << "\n";


        stbi_image_free(image);

        return true;
    }



    bool result = false;



    // 有透明通道
    if (channels == 4)
    {

        unsigned char* rgb =
            rgbaToRgbWhiteBackground(
                image,
                width,
                height);



        result =
            stbi_write_jpg(
                outputPath.string().c_str(),
                width,
                height,
                3,
                rgb,
                quality);



        delete[] rgb;
    }
    else
    {

        // 强制转换为 RGB
        unsigned char* rgb =
            stbi_load(
                inputPath.string().c_str(),
                &width,
                &height,
                &channels,
                3);



        if (rgb)
        {
            result =
                stbi_write_jpg(
                    outputPath.string().c_str(),
                    width,
                    height,
                    3,
                    rgb,
                    quality);


            stbi_image_free(rgb);
        }
    }



    stbi_image_free(image);



    if (result)
    {
        std::cout
            << "[成功] "
            << inputPath.filename()
            << " -> "
            << outputPath.filename()
            << "\n";


        /*
          如果想转换后删除原文件：
          取消下面注释

          fs::remove(inputPath);
        */
    }
    else
    {
        std::cout
            << "[失败] "
            << inputPath.filename()
            << "\n";
    }


    return result;
}



// =====================================================
// 主函数
// =====================================================
int main()
{

    // 图片目录
    std::string folder = "E:/Interest Files";


    if (!fs::exists(folder))
    {
        std::cout
            << "目录不存在: "
            << folder
            << "\n";

        return 1;
    }



    // 支持格式
    std::vector<std::string> formats =
    {
        ".png",
        ".jpeg",
        ".jpg",
        ".webp"
    };



    size_t total = 0;
    size_t success = 0;



    // 遍历所有子目录
    for (const auto& entry :
        fs::recursive_directory_iterator(folder))
    {

        if (!entry.is_regular_file())
            continue;



        fs::path file =
            entry.path();



        std::string ext =
            file.extension().string();



        // 转小写
        std::transform(
            ext.begin(),
            ext.end(),
            ext.begin(),
            [](unsigned char c)
            {
                return std::tolower(c);
            });



        // 判断格式
        if (std::find(
            formats.begin(),
            formats.end(),
            ext)
            == formats.end())
        {
            continue;
        }



        // JPG无需转换
        if (ext == ".jpg")
        {
            continue;
        }



        total++;


        if (convertImage(file))
        {
            success++;
        }

    }



    std::cout
        << "\n====================\n";

    std::cout
        << "发现图片: "
        << total
        << "\n";


    std::cout
        << "成功转换: "
        << success
        << "\n";


    std::cout
        << "失败数量: "
        << total - success
        << "\n";


    std::cout
        << "====================\n";


    return 0;
}

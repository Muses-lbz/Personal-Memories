#include <windows.h>
#include <gdiplus.h>
#include <filesystem>
#include <iostream>
#include <vector>

#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;
namespace fs = std::filesystem;

/**
 * 安全获取图片编码器 CLSID
 * 使用 std::vector<BYTE>，避免 VS 运行时错误
 */
bool GetEncoderClsid(const WCHAR* mimeType, CLSID* pClsid)
{
    UINT num = 0;
    UINT size = 0;

    GetImageEncodersSize(&num, &size);
    if (size == 0)
        return false;

    std::vector<BYTE> buffer(size);
    ImageCodecInfo* pImageCodecInfo =
        reinterpret_cast<ImageCodecInfo*>(buffer.data());

    GetImageEncoders(num, size, pImageCodecInfo);

    for (UINT i = 0; i < num; ++i)
    {
        if (wcscmp(pImageCodecInfo[i].MimeType, mimeType) == 0)
        {
            *pClsid = pImageCodecInfo[i].Clsid;
            return true;
        }
    }
    return false;
}

int main()
{
    // =======================
    // 1️⃣ 修改为你的路径
    // =======================
    fs::path inputDir  = L"E:/Documents Beauty/Convert Files 5";
    fs::path outputDir = L"E:/Documents Beauty/Convert Files 6";

    double scale = 1.4; // 放大 140%

    // 创建输出目录
    if (!fs::exists(outputDir))
    {
        fs::create_directories(outputDir);
    }

    // =======================
    // 2️⃣ 初始化 GDI+
    // =======================
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken = 0;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    // =======================
    // 3️⃣ 获取编码器 CLSID
    // =======================
    CLSID jpgClsid{}, pngClsid{}, bmpClsid{};

    GetEncoderClsid(L"image/jpeg", &jpgClsid);
    GetEncoderClsid(L"image/png", &pngClsid);
    GetEncoderClsid(L"image/bmp", &bmpClsid);

    // =======================
    // 4️⃣ 遍历文件夹
    // =======================
    for (const auto& entry : fs::directory_iterator(inputDir))
    {
        if (!entry.is_regular_file())
            continue;

        fs::path imgPath = entry.path();
        std::wstring ext = imgPath.extension().wstring();

        CLSID* targetClsid = nullptr;

        if (ext == L".jpg" || ext == L".jpeg")
            targetClsid = &jpgClsid;
        else if (ext == L".png")
            targetClsid = &pngClsid;
        else if (ext == L".bmp")
            targetClsid = &bmpClsid;
        else
            continue; // 非支持图片，跳过

        // =======================
        // 5️⃣ 加载图片
        // =======================
        Image image(imgPath.c_str());
        if (image.GetLastStatus() != Ok)
        {
            std::wcout << L"读取失败：" << imgPath << std::endl;
            continue;
        }

        // =======================
        // 6️⃣ 计算新尺寸
        // =======================
        UINT newWidth = static_cast<UINT>(image.GetWidth() * scale);
        UINT newHeight = static_cast<UINT>(image.GetHeight() * scale);

        // =======================
        // 7️⃣ 创建目标 Bitmap
        // =======================
        Bitmap newBitmap(newWidth, newHeight, PixelFormat24bppRGB);

        Graphics graphics(&newBitmap);
        graphics.SetInterpolationMode(
            InterpolationModeHighQualityBicubic);
        graphics.SetSmoothingMode(SmoothingModeHighQuality);
        graphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);

        graphics.DrawImage(
            &image,
            0, 0,
            newWidth,
            newHeight
        );

        // =======================
        // 8️⃣ 保存图片
        // =======================
        
        //fs::path outPath = outputDir / imgPath.filename();

        fs::path outPath = outputDir / (imgPath.stem().wstring() + L"_120" + imgPath.extension().wstring());
        newBitmap.Save(outPath.c_str(), targetClsid, nullptr);

        std::wcout << L"处理完成：" << imgPath.filename() << std::endl;
    }

    // =======================
    // 9️⃣ 关闭 GDI+
    // =======================
    GdiplusShutdown(gdiplusToken);

    std::cout << "所有图片已处理完成！" << std::endl;
    return 0;
}

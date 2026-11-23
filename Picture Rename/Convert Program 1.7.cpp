#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <system_error>
#include <random>
#include <set>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace fs = std::filesystem;

static const std::string kNamePrefix = "Img_";
static const int kStartIndex = 10001;

// ---------- 自然排序 ----------
static bool naturalCompare(const std::string& a, const std::string& b) {
    size_t i = 0, j = 0;
    while (i < a.size() && j < b.size()) {
        if (std::isdigit((unsigned char)a[i]) && std::isdigit((unsigned char)b[j])) {
            size_t i2 = i, j2 = j;
            while (i2 < a.size() && std::isdigit((unsigned char)a[i2])) ++i2;
            while (j2 < b.size() && std::isdigit((unsigned char)b[j2])) ++j2;
            std::string na = a.substr(i, i2 - i);
            std::string nb = b.substr(j, j2 - j);
            size_t nz1 = na.find_first_not_of('0');
            size_t nz2 = nb.find_first_not_of('0');
            std::string ca = (nz1 == std::string::npos) ? "0" : na.substr(nz1);
            std::string cb = (nz2 == std::string::npos) ? "0" : nb.substr(nz2);
            if (ca.size() != cb.size()) return ca.size() < cb.size();
            if (ca != cb) return ca < cb;
            i = i2; j = j2;
        }
        else {
            char ca = std::tolower((unsigned char)a[i]);
            char cb = std::tolower((unsigned char)b[j]);
            if (ca != cb) return ca < cb;
            ++i; ++j;
        }
    }
    return (a.size() - i) < (b.size() - j);
}

static std::string lowerExt(const fs::path& p) {
    std::string ext = p.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return ext;
}

static bool isConvertibleImage(const std::string& ext) {
    return ext == ".jpg" || ext == ".jpeg" || ext == ".bmp" ||
        ext == ".gif" || ext == ".tga";
}

static bool convertToPng(const fs::path& src, const fs::path& dst) {
    int w = 0, h = 0, c = 0;
    unsigned char* data = stbi_load(src.string().c_str(), &w, &h, &c, 0);
    if (!data) return false;
    bool ok = stbi_write_png(dst.string().c_str(), w, h, c, data, w * c) != 0;
    stbi_image_free(data);
    return ok;
}

static fs::path makeTempPath(const fs::path& folder) {
    static std::mt19937_64 rng(std::random_device{}());
    for (int i = 0; i < 1000000; ++i) {
        fs::path p = folder / ("__tmp_" + std::to_string(rng()) + ".png");
        if (!fs::exists(p)) return p;
    }
    return folder / "__tmp_fallback.png";
}

static std::vector<fs::path> collectAllPng(const fs::path& folder) {
    std::vector<fs::path> result;
    for (const auto& entry : fs::directory_iterator(folder)) {
        if (!entry.is_regular_file()) continue;
        if (lowerExt(entry.path()) == ".png") result.push_back(entry.path());
    }
    return result;
}

// ---------- 步骤 1：格式转换 ----------
static void convertAll(const fs::path& folder) {
    std::vector<fs::path> toConvert;
    for (const auto& entry : fs::directory_iterator(folder)) {
        if (!entry.is_regular_file()) continue;
        if (isConvertibleImage(lowerExt(entry.path()))) {
            toConvert.push_back(entry.path());
        }
    }
    std::sort(toConvert.begin(), toConvert.end(), [](const fs::path& a, const fs::path& b) {
        return naturalCompare(a.filename().string(), b.filename().string());
        });

    int total = (int)toConvert.size();
    int ok = 0, fail = 0;
    std::cout << "==== 步骤 1：格式转换（共 " << total << " 张） ====\n";

    for (int i = 0; i < total; ++i) {
        const auto& src = toConvert[i];
        std::cout << "[" << (i + 1) << "/" << total << "] 转换: "
            << src.filename().string() << " -> .png ... " << std::flush;

        fs::path tmp = makeTempPath(folder);
        if (convertToPng(src, tmp)) {
            std::error_code ec;
            fs::remove(src, ec);
            if (ec) std::cout << "OK（源文件删除失败: " << ec.message() << "）\n";
            else    std::cout << "OK\n";
            ++ok;
        }
        else {
            std::error_code ec;
            fs::remove(tmp, ec);
            std::cout << "失败（保留原文件）\n";
            ++fail;
        }
    }
    std::cout << "转换完成：成功 " << ok << " 张，失败 " << fail << " 张\n\n";
}

// ---------- 步骤 2：全部 PNG 随机数字命名 ----------
static int randomRenameAll(const fs::path& folder) {
    auto files = collectAllPng(folder);
    std::sort(files.begin(), files.end(), [](const fs::path& a, const fs::path& b) {
        return naturalCompare(a.filename().string(), b.filename().string());
        });

    static std::mt19937_64 rng(std::random_device{}());
    std::uniform_int_distribution<long long> dist(100000, 9999999);

    std::set<std::string> used;
    for (const auto& entry : fs::directory_iterator(folder)) {
        used.insert(entry.path().filename().string());
    }

    int total = (int)files.size();
    int ok = 0;
    std::cout << "==== 步骤 2：随机数字命名（共 " << total << " 张） ====\n";

    for (int i = 0; i < total; ++i) {
        fs::path src = files[i];
        std::string newName;
        do {
            newName = std::to_string(dist(rng)) + ".png";
        } while (used.count(newName));

        fs::path dst = folder / newName;
        std::cout << "[" << (i + 1) << "/" << total << "] 随机命名: "
            << src.filename().string() << " -> " << newName << " ... " << std::flush;

        std::error_code ec;
        fs::rename(src, dst, ec);
        if (ec) {
            ec.clear();
            fs::copy_file(src, dst, fs::copy_options::overwrite_existing, ec);
            if (!ec) fs::remove(src, ec);
        }
        if (!ec) {
            used.erase(src.filename().string());
            used.insert(newName);
            std::cout << "OK\n";
            ++ok;
        }
        else {
            std::cout << "失败\n";
        }
    }
    std::cout << "随机命名完成：" << ok << " 张\n\n";
    return ok;
}

// ---------- 步骤 3：按名称排序 + 全部重命名为 Image_10001 起 ----------
static void sequentialRenameAll(const fs::path& folder) {
    auto files = collectAllPng(folder);
    std::sort(files.begin(), files.end(), [](const fs::path& a, const fs::path& b) {
        return naturalCompare(a.filename().string(), b.filename().string());
        });

    int total = (int)files.size();
    std::cout << "==== 步骤 3：按名称排序 + 重命名为 Image_<序号>（共 "
        << total << " 张，起始序号 " << kStartIndex << "） ====\n";

    if (total == 0) { std::cout << "文件夹内没有 PNG。\n\n"; return; }

    // 第一阶段：全部改成临时名
    std::vector<fs::path> temps;
    temps.reserve(total);
    for (int i = 0; i < total; ++i) {
        const fs::path& src = files[i];
        fs::path tmp = makeTempPath(folder);
        std::error_code ec;
        fs::rename(src, tmp, ec);
        if (ec) {
            ec.clear();
            fs::copy_file(src, tmp, fs::copy_options::overwrite_existing, ec);
            if (!ec) fs::remove(src, ec);
        }
        if (ec) {
            std::cerr << "临时重命名失败: " << src.filename().string()
                << " : " << ec.message() << "（跳过）\n";
            temps.push_back(fs::path{});
        }
        else {
            temps.push_back(tmp);
        }
    }

    // 第二阶段：按顺序改成最终名
    int currentIndex = kStartIndex - 1;
    int ok = 0;
    for (int i = 0; i < total; ++i) {
        if (temps[i].empty()) continue;

        ++currentIndex;
        std::ostringstream oss;
        oss << kNamePrefix << currentIndex << ".png";
        fs::path dst = folder / oss.str();

        std::cout << "[" << (i + 1) << "/" << total << "] 重命名: "
            << temps[i].filename().string() << " -> " << dst.filename().string()
            << " ... " << std::flush;

        std::error_code ec;
        fs::rename(temps[i], dst, ec);
        if (ec) {
            ec.clear();
            fs::copy_file(temps[i], dst, fs::copy_options::overwrite_existing, ec);
            if (!ec) fs::remove(temps[i], ec);
        }
        if (ec) {
            std::cout << "失败\n";
            --currentIndex;
        }
        else {
            std::cout << "OK\n";
            ++ok;
        }
    }
    std::cout << "顺序命名完成：" << ok << " 张，最大序号 " << currentIndex << "\n\n";
}

int main(int argc, char* argv[]) {
    fs::path folder = (argc >= 2) ? fs::path(argv[1])
        : fs::path("E:/Github Files/Room-Timing");

    if (!fs::exists(folder) || !fs::is_directory(folder)) {
        std::cerr << "文件夹不存在: " << folder << "\n";
        return 1;
    }
    std::cout << "目标文件夹: " << folder << "\n\n";

    convertAll(folder);          // 1. 格式转换
    randomRenameAll(folder);     // 2. 全部 PNG 随机数字命名
    sequentialRenameAll(folder); // 3. 按名称排序 + Img_10001 起重新编号

    std::cout << "==== 全部完成 ====\n";
    return 0;
}

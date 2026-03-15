#include <iostream>
#include <filesystem>
#include <string>
#include <cctype>

namespace fs = std::filesystem;
using namespace std;

// 判断字符串是否为纯数字
bool isPureNumber(const string& name)
{
    if (name.empty()) return false;

    for (char c : name)
    {
        if (!isdigit(c))
            return false;
    }
    return true;
}

int main()
{
    // 你的图片文件夹路径
    string folder = "D:/images";  // 修改为你的路径

    for (const auto& entry : fs::directory_iterator(folder))
    {
        if (entry.is_regular_file())
        {
            // 获取文件名（不含路径）
            string filename = entry.path().filename().string();

            // 获取文件名（不含扩展名）
            string stem = entry.path().stem().string();

            // 如果不是纯数字
            if (!isPureNumber(stem))
            {
                cout << "删除: " << filename << endl;
                fs::remove(entry.path());
            }
        }
    }

    cout << "处理完成" << endl;

    return 0;
}

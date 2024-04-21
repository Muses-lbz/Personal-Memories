### C++图片文件批量重命名

------

#### 程序 1.0 版本

实现思路：使用C++的文件操作和字符串处理功能来实现文件重命名。基本的思路是遍历指定文件夹下的所有图片文件，然后按照指定的序号开始重命名它们。

```c++
#include <iostream>
#include <string>
#include <filesystem>
#include <ctime>
#include <iomanip> // 用于格式化输出

namespace fs = std::filesystem;

void renameImagesInFolder(const std::string& folderPath, int startNumber) {
    int currentNumber = startNumber;
    for (const auto& entry : fs::directory_iterator(folderPath)) {
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
                std::string newFileName = folderPath + "/" + std::string(datePrefix) + "_" + std::to_string(currentNumber) + extension;
                fs::rename(entry.path(), newFileName);
                ++currentNumber;
            }
        }
    }
}

int main() {
    std::string folderPath = "E:/Convert Files"; // 指定文件夹路径
    int startNumber = 10001; // 指定起始序号

    renameImagesInFolder(folderPath, startNumber);

    std::cout << "Image renaming completed." << std::endl;
    return 0;
}

```

编译器需要支持 C++17 标准，并且正确地包含了 `<filesystem>` 头文件，以找到命名空间 `std::filesystem` 的成员。在“解决方案资源管理器”右击项目，在下拉菜单，找到“属性”。打开“属性”窗口后，修改C++语言标准。

文件夹路径中的反斜杠 (`\`) 在 C++ 中被视为转义字符，因此需要使用双反斜杠 (`\\`) 或者使用正斜杠 (`/`) 来表示路径分隔符。因此，应该将文件夹路径中的 `\` 替换为 `\\` 或者使用正斜杠。

------

#### 程序 1.1 版本

在 1.0 的基础上，添加了新的操作，将重命名后的图片移动到另一个指定的文件夹中。输入 1 即可执行该操作，其他为无效输入，程序结束。

```c++
#include <iostream>
#include <string>
#include <filesystem>
#include <ctime>
#include <iomanip> // 用于格式化输出

namespace fs = std::filesystem;

void renameImagesInFolder(const std::string& folderPath, int startNumber) {
    int currentNumber = startNumber;
    for (const auto& entry : fs::directory_iterator(folderPath)) {
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
                std::string newFileName = folderPath + "/" + std::string(datePrefix) + "_" + std::to_string(currentNumber) + extension;
                fs::rename(entry.path(), newFileName);
                ++currentNumber;
            }
        }
    }
}

void moveImagesToFolder(const std::string& sourceFolderPath, const std::string& targetFolderPath) {
    // 创建目标文件夹，如果不存在的话
    fs::create_directory(targetFolderPath);

    for (const auto& entry : fs::directory_iterator(sourceFolderPath)) {
        if (entry.is_regular_file()) {
            std::string extension = entry.path().extension().string();
            // 只处理图片文件，你可以根据需要添加其他图片格式的判断条件
            if (extension == ".jpg" || extension == ".png" || extension == ".jpeg" || extension == ".bmp") {
                // 获取文件名
                std::string fileName = entry.path().filename().string();
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
    std::string folderPath = "E:/Convert Files"; // 指定文件夹路径
    std::string targetFolderPath = "E:/Convert Folder"; // 指定目标文件夹路径
    int startNumber = 10107; // 指定起始序号

    renameImagesInFolder(folderPath, startNumber);

    std::cout << "Image renaming completed." << std::endl;

    std::cout << "Enter 1 to move all images: ";
    
    int choice;

    std::cin >> choice;

    if (choice == 1) {
        // 移动图片到指定文件夹
        moveImagesToFolder(folderPath, targetFolderPath);
    }

    return 0;
}

```

------


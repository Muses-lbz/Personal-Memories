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

#### 程序 1.2 版本

这个版本本来是想偷懒的，在工作目录下创建一个txt文件，用来记录数值，隔天更新，就不需要一直修改startNumber了。可是，程序有问题，重命名的输出有问题，会重复，移入指定文件夹时，就直接被替换了。直接损失报销了近百张照片才发现原因，因为是替换，都没进垃圾站。。。

程序 1.2 版本需要实现自主计数，并可以更新计数。同时添加检测机制，避免出现覆盖同名文件的情况。

```C++
#include <iostream>
#include <fstream> // 用于文件操作
#include <string>
#include <filesystem>
#include <vector>
#include <ctime>
#include <iomanip> // 用于格式化输出

namespace fs = std::filesystem;

void createFileIfNotExists(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.good()) {
        std::ofstream newFile(filePath);
        newFile.close();
    }
}

int readLastNumberFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    int lastNumber = 0;
    if (file.is_open()) {
        file >> lastNumber;
        file.close();
    }
    return lastNumber;
}

void writeNumberToFile(const std::string& filePath, int number) {
    std::ofstream file(filePath);
    if (file.is_open()) {
        file << number;
        file.close();
    }
}

void writeDateToFile(const std::string& filePath, const std::string& date) {
    std::ofstream file(filePath);
    if (file.is_open()) {
        file << date;
        file.close();
    }
}

std::string readDateFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::string savedDate;
    if (file.is_open()) {
        file >> savedDate;
        file.close();
    }
    return savedDate;
}

void renameImagesInFolder(const std::string& folderPath, int startNumber, const std::string& numberFilePath, const std::string& dateFilePath) {
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
    writeNumberToFile(numberFilePath, currentNumber); // 写入最后的序号到文件
    time_t now = time(nullptr); // 获取当前日期
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);
    char currentDate[15]; // 格式化为 YYYYMMDD
    strftime(currentDate, sizeof(currentDate), "%Y%m%d", &timeinfo);
    writeDateToFile(dateFilePath, currentDate); // 写入当前日期到文件
}

void moveImagesToFolder(const std::string& sourceFolderPath, const std::string& targetFolderPath) {
    // 创建目标文件夹，如果不存在的话
    fs::create_directory(targetFolderPath);

    // 存储目标文件夹中已有的文件名
    std::vector<std::string> targetFileNames;
    for (const auto& entry : fs::directory_iterator(targetFolderPath)) {
        if (entry.is_regular_file()) {
            targetFileNames.push_back(entry.path().filename().string());
        }
    }

    for (const auto& entry : fs::directory_iterator(sourceFolderPath)) {
        if (entry.is_regular_file()) {
            std::string extension = entry.path().extension().string();
            // 只处理图片文件，你可以根据需要添加其他图片格式的判断条件
            if (extension == ".jpg" || extension == ".png" || extension == ".jpeg" || extension == ".bmp") {
                // 获取文件名
                std::string fileName = entry.path().filename().string();
                // 检查文件名是否已经存在于目标文件夹中
                if (std::find(targetFileNames.begin(), targetFileNames.end(), fileName) != targetFileNames.end()) {
                    std::cout << "Duplicate file name found: " << fileName << std::endl;
                    std::cout << "Aborting operation." << std::endl;
                    return;
                }
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
    std::string numberFilePath = "current_number.txt"; // 记录当前序号的文件路径
    std::string dateFilePath = "current_date.txt"; // 记录当前日期的文件路径
    int startNumber = 10001; // 指定起始序号

    createFileIfNotExists(numberFilePath); // 检查并创建记录序号的文件
    createFileIfNotExists(dateFilePath); // 检查并创建记录日期的文件

    int lastNumber = readLastNumberFromFile(numberFilePath); // 从文件中读取上次的序号

    if (lastNumber < startNumber) {
        lastNumber = startNumber; // 如果文件中的序号比起始序号小，则使用起始序号
    }

    renameImagesInFolder(folderPath, lastNumber, numberFilePath, dateFilePath); // 重命名图片并更新序号

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

这个版本目前看起来大概没有什么问题。在移动操作之前，添加了一个检测机制，避免同名文件移动被覆盖的现象，并输出同名的文件名称。新增了两个txt文件，用来记录日期和currentNumber。看第二天的执行情况了。。。

------

#### 程序 1.3 版本

呃，果然出问题了，并不会随着日期更新而更新。

现在，换一种设计思路。因为不想一天之内不同时间执行程序时，需要修改main函数中的startNumber，所以引入了current_date.txt，这个文件用来记录日期，current_number.txt，这个文件用来记录上一次执行，最后一次currentNumber的数值。

当日期不需要更新时，程序执行的currentNumber选用current_number.txt中的数据，确保一天的不同时间执行程序可以成功把序号接上。当日期需要更新时，删除current_number.txt，重新创建一个current_number.txt，重置数据为10001。日期检查，重置数据这一步应该单独写一个函数出来，因为要确保重置数据后，在之后的执行中，仍能更新current_number.txt中的current_number数值，确保一天之内不同时间执行程序可以成功把序号接上。

```c++
#include <iostream>
#include <fstream> // 用于文件操作
#include <string>
#include <filesystem>
#include <vector>
#include <ctime>
#include <iomanip> // 用于格式化输出

namespace fs = std::filesystem;

void createFileIfNotExists(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.good()) {
        std::ofstream newFile(filePath);
        newFile.close();
    }
}

int readLastNumberFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    int lastNumber = 0;
    if (file.is_open()) {
        file >> lastNumber;
        file.close();
    }
    return lastNumber;
}

void writeNumberToFile(const std::string& filePath, int number) {
    std::ofstream file(filePath);
    if (file.is_open()) {
        file << number;
        file.close();
    }
}

void writeDateToFile(const std::string& filePath, const std::string& date) {
    std::ofstream file(filePath);
    if (file.is_open()) {
        file << date;
        file.close();
    }
}

std::string readDateFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::string savedDate;
    if (file.is_open()) {
        file >> savedDate;
        file.close();
    }
    return savedDate;
}

void resetNumberIfNeeded(int startNumber, const std::string& numberFilePath, const std::string& dateFilePath) {
    // 读取当前日期
    time_t now = time(nullptr);
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);
    char currentDate[15]; // 格式化为 YYYYMMDD
    strftime(currentDate, sizeof(currentDate), "%Y%m%d", &timeinfo);

    // 读取之前保存的日期
    std::string savedDate = readDateFromFile(dateFilePath);

    // 如果日期发生了变化，则重置序号为起始序号
    if (savedDate != currentDate) {
        // 删除 current_number.txt 文件并重新创建
        fs::remove(numberFilePath);
        createFileIfNotExists(numberFilePath);
        // 设置 currentNumber 为起始序号
        writeNumberToFile(numberFilePath, startNumber); // 写入起始序号到文件
        writeDateToFile(dateFilePath, currentDate); // 写入当前日期到文件
    }
}

void renameImagesInFolder(const std::string& folderPath, int startNumber, const std::string& numberFilePath, const std::string& dateFilePath) {
    resetNumberIfNeeded(startNumber, numberFilePath, dateFilePath); // 检查日期并重置数据

    int currentNumber = readLastNumberFromFile(numberFilePath); // 从文件中读取上次的序号

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

                // 递增 currentNumber
                ++currentNumber;
            }
        }
    }
    // 更新最后的序号到文件
    writeNumberToFile(numberFilePath, currentNumber);
}

void moveImagesToFolder(const std::string& sourceFolderPath, const std::string& targetFolderPath) {
    // 创建目标文件夹，如果不存在的话
    fs::create_directory(targetFolderPath);

    // 存储目标文件夹中已有的文件名
    std::vector<std::string> targetFileNames;
    for (const auto& entry : fs::directory_iterator(targetFolderPath)) {
        if (entry.is_regular_file()) {
            targetFileNames.push_back(entry.path().filename().string());
        }
    }

    for (const auto& entry : fs::directory_iterator(sourceFolderPath)) {
        if (entry.is_regular_file()) {
            std::string extension = entry.path().extension().string();
            // 只处理图片文件，你可以根据需要添加其他图片格式的判断条件
            if (extension == ".jpg" || extension == ".png" || extension == ".jpeg" || extension == ".bmp") {
                // 获取文件名
                std::string fileName = entry.path().filename().string();
                // 检查文件名是否已经存在于目标文件夹中
                if (std::find(targetFileNames.begin(), targetFileNames.end(), fileName) != targetFileNames.end()) {
                    std::cout << "Duplicate file name found: " << fileName << std::endl;
                    std::cout << "Aborting operation." << std::endl;
                    return;
                }
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
    std::string numberFilePath = "current_number.txt"; // 记录当前序号的文件路径
    std::string dateFilePath = "current_date.txt"; // 记录当前日期的文件路径
    int startNumber = 10001; // 指定起始序号

    createFileIfNotExists(numberFilePath); // 检查并创建记录序号的文件
    createFileIfNotExists(dateFilePath); // 检查并创建记录日期的文件

    renameImagesInFolder(folderPath, startNumber, numberFilePath, dateFilePath); // 重命名图片并更新序号

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

这个版本的程序看上去没啥问题了。

------

#### 程序 1.4 版本

前一个版本的程序，在文件夹中存在过多图片进行重命名操作时，会出现问题，问题就不细细描述了。再一个，如果想重命名多个文件夹中的图片，还需要逐个移动至指定文件夹，这样操作有些费时费力。

这个版本做出如下修改：

- 对 `renameImagesInFolder` 函数进行修改，以便递归地处理子文件夹，达到重命名指定文件夹内及其子文件夹内的所有图片。使用了 `fs::recursive_directory_iterator` 来递归地遍历文件夹内的所有文件和子文件夹。然后对于每个文件，它检查是否是图片文件，如果是，则按照原有逻辑重命名并更新序号。

- 对 `moveImagesToFolder` 函数进行修改，以便递归地处理子文件夹内的图片，并将它们移动到目标文件夹。修改后，`moveImagesToFolder` 函数会递归地遍历源文件夹及其所有子文件夹，将所有图片文件移动到目标文件夹中，并且会检查目标文件夹中是否存在重复的文件名。

```c++
#include <iostream>
#include <fstream> // 用于文件操作
#include <string>
#include <filesystem>
#include <vector>
#include <ctime>
#include <iomanip> // 用于格式化输出

namespace fs = std::filesystem;

void createFileIfNotExists(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.good()) {
        std::ofstream newFile(filePath);
        newFile.close();
    }
}

int readLastNumberFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    int lastNumber = 0;
    if (file.is_open()) {
        file >> lastNumber;
        file.close();
    }
    return lastNumber;
}

void writeNumberToFile(const std::string& filePath, int number) {
    std::ofstream file(filePath);
    if (file.is_open()) {
        file << number;
        file.close();
    }
}

void writeDateToFile(const std::string& filePath, const std::string& date) {
    std::ofstream file(filePath);
    if (file.is_open()) {
        file << date;
        file.close();
    }
}

std::string readDateFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::string savedDate;
    if (file.is_open()) {
        file >> savedDate;
        file.close();
    }
    return savedDate;
}

void resetNumberIfNeeded(int startNumber, const std::string& numberFilePath, const std::string& dateFilePath) {
    // 读取当前日期
    time_t now = time(nullptr);
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);
    char currentDate[15]; // 格式化为 YYYYMMDD
    strftime(currentDate, sizeof(currentDate), "%Y%m%d", &timeinfo);

    // 读取之前保存的日期
    std::string savedDate = readDateFromFile(dateFilePath);

    // 如果日期发生了变化，则重置序号为起始序号
    if (savedDate != currentDate) {
        // 删除 current_number.txt 文件并重新创建
        fs::remove(numberFilePath);
        createFileIfNotExists(numberFilePath);
        // 设置 currentNumber 为起始序号
        writeNumberToFile(numberFilePath, startNumber); // 写入起始序号到文件
        writeDateToFile(dateFilePath, currentDate); // 写入当前日期到文件
    }
}

void renameImagesInFolder(const std::string& folderPath, int startNumber, const std::string& numberFilePath, const std::string& dateFilePath) {
    resetNumberIfNeeded(startNumber, numberFilePath, dateFilePath); // 检查日期并重置数据

    int currentNumber = readLastNumberFromFile(numberFilePath); // 从文件中读取上次的序号

    for (const auto& entry : fs::recursive_directory_iterator(folderPath)) {
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
                std::string newFileName = entry.path().parent_path().string() + "/" + std::string(datePrefix) + "_" + std::to_string(currentNumber) + extension;
                fs::rename(entry.path(), newFileName);

                // 递增 currentNumber
                ++currentNumber;
            }
        }
    }
    // 更新最后的序号到文件
    writeNumberToFile(numberFilePath, currentNumber);
}

void moveImagesToFolder(const std::string& sourceFolderPath, const std::string& targetFolderPath) {
    // 创建目标文件夹，如果不存在的话
    fs::create_directory(targetFolderPath);

    // 存储目标文件夹中已有的文件名
    std::vector<std::string> targetFileNames;
    for (const auto& entry : fs::recursive_directory_iterator(targetFolderPath)) {
        if (entry.is_regular_file()) {
            targetFileNames.push_back(entry.path().filename().string());
        }
    }

    for (const auto& entry : fs::recursive_directory_iterator(sourceFolderPath)) {
        if (entry.is_regular_file()) {
            std::string extension = entry.path().extension().string();
            // 只处理图片文件，你可以根据需要添加其他图片格式的判断条件
            if (extension == ".jpg" || extension == ".png" || extension == ".jpeg" || extension == ".bmp") {
                // 获取文件名
                std::string fileName = entry.path().filename().string();
                // 检查文件名是否已经存在于目标文件夹中
                if (std::find(targetFileNames.begin(), targetFileNames.end(), fileName) != targetFileNames.end()) {
                    std::cout << "Duplicate file name found: " << fileName << std::endl;
                    std::cout << "Aborting operation." << std::endl;
                    return;
                }
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
    std::string folderPath = "E:/Convert Documents/Convert Files"; // 指定文件夹路径
    std::string targetFolderPath = "E:/Convert Documents/Convert Folder"; // 指定目标文件夹路径
    std::string numberFilePath = "current_number.txt"; // 记录当前序号的文件路径
    std::string dateFilePath = "current_date.txt"; // 记录当前日期的文件路径
    int startNumber = 10001; // 指定起始序号

    createFileIfNotExists(numberFilePath); // 检查并创建记录序号的文件
    createFileIfNotExists(dateFilePath); // 检查并创建记录日期的文件

    renameImagesInFolder(folderPath, startNumber, numberFilePath, dateFilePath); // 重命名图片并更新序号

    std::cout << "Image renaming completed." << std::endl;

    //std::cout << "Enter 1 to move all images: ";

    int choice;

    //std::cin >> choice;
    choice = 1;

    if (choice == 1) {
        // 移动图片到指定文件夹
        moveImagesToFolder(folderPath, targetFolderPath);
    }

    return 0;
}

```

------

#### 程序 1.5 版本

这个版本添加了一个新的功能：如果重命名图片的文件格式为 .jfif 格式，就先修改图片为 .jpg 格式，再执行后续的其他操作。

```c++
#include <iostream>
#include <fstream> // 用于文件操作
#include <string>
#include <filesystem>
#include <vector>
#include <ctime>
#include <iomanip> // 用于格式化输出

namespace fs = std::filesystem;

void createFileIfNotExists(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.good()) {
        std::ofstream newFile(filePath);
        newFile.close();
    }
}

int readLastNumberFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    int lastNumber = 0;
    if (file.is_open()) {
        file >> lastNumber;
        file.close();
    }
    return lastNumber;
}

void writeNumberToFile(const std::string& filePath, int number) {
    std::ofstream file(filePath);
    if (file.is_open()) {
        file << number;
        file.close();
    }
}

void writeDateToFile(const std::string& filePath, const std::string& date) {
    std::ofstream file(filePath);
    if (file.is_open()) {
        file << date;
        file.close();
    }
}

std::string readDateFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::string savedDate;
    if (file.is_open()) {
        file >> savedDate;
        file.close();
    }
    return savedDate;
}

void resetNumberIfNeeded(int startNumber, const std::string& numberFilePath, const std::string& dateFilePath) {
    // 读取当前日期
    time_t now = time(nullptr);
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);
    char currentDate[15]; // 格式化为 YYYYMMDD
    strftime(currentDate, sizeof(currentDate), "%Y%m%d", &timeinfo);

    // 读取之前保存的日期
    std::string savedDate = readDateFromFile(dateFilePath);

    // 如果日期发生了变化，则重置序号为起始序号
    if (savedDate != currentDate) {
        // 删除 current_number.txt 文件并重新创建
        fs::remove(numberFilePath);
        createFileIfNotExists(numberFilePath);
        // 设置 currentNumber 为起始序号
        writeNumberToFile(numberFilePath, startNumber); // 写入起始序号到文件
        writeDateToFile(dateFilePath, currentDate); // 写入当前日期到文件
    }
}

void renameImagesInFolder(const std::string& folderPath, int startNumber, const std::string& numberFilePath, const std::string& dateFilePath) {
    resetNumberIfNeeded(startNumber, numberFilePath, dateFilePath); // 检查日期并重置数据

    int currentNumber = readLastNumberFromFile(numberFilePath); // 从文件中读取上次的序号

    for (const auto& entry : fs::recursive_directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            std::string extension = entry.path().extension().string();
            fs::path newFilePath = entry.path(); // 新路径变量

            // 检查文件是否为 .jfif 格式，并将其改为 .jpg 格式
            if (extension == ".jfif") {
                newFilePath = newFilePath.replace_extension(".jpg");
                fs::rename(entry.path(), newFilePath); // 重命名文件扩展名为 .jpg
                extension = ".jpg"; // 更新扩展名为 .jpg
            }

            // 只处理图片文件
            if (extension == ".jpg" || extension == ".png" || extension == ".jpeg" || extension == ".bmp") {
                // 获取当前日期时间作为前缀
                time_t now = time(nullptr);
                struct tm timeinfo;
                localtime_s(&timeinfo, &now);
                char datePrefix[15]; // 格式化为 YYYYMMDDHHMMSS
                strftime(datePrefix, sizeof(datePrefix), "%Y%m%d%H%M%S", &timeinfo);

                // 组合新的文件名
                std::string newFileName = newFilePath.parent_path().string() + "/" + std::string(datePrefix) + "_" + std::to_string(currentNumber) + extension;
                fs::rename(newFilePath, newFileName); // 重命名文件为新文件名

                // 递增 currentNumber
                ++currentNumber;
            }
        }
    }
    // 更新最后的序号到文件
    writeNumberToFile(numberFilePath, currentNumber);
}

void moveImagesToFolder(const std::string& sourceFolderPath, const std::string& targetFolderPath) {
    // 创建目标文件夹，如果不存在的话
    fs::create_directory(targetFolderPath);

    // 存储目标文件夹中已有的文件名
    std::vector<std::string> targetFileNames;
    for (const auto& entry : fs::recursive_directory_iterator(targetFolderPath)) {
        if (entry.is_regular_file()) {
            targetFileNames.push_back(entry.path().filename().string());
        }
    }

    for (const auto& entry : fs::recursive_directory_iterator(sourceFolderPath)) {
        if (entry.is_regular_file()) {
            std::string extension = entry.path().extension().string();
            // 只处理图片文件，你可以根据需要添加其他图片格式的判断条件
            if (extension == ".jpg" || extension == ".png" || extension == ".jpeg" || extension == ".bmp") {
                // 获取文件名
                std::string fileName = entry.path().filename().string();
                // 检查文件名是否已经存在于目标文件夹中
                if (std::find(targetFileNames.begin(), targetFileNames.end(), fileName) != targetFileNames.end()) {
                    std::cout << "Duplicate file name found: " << fileName << std::endl;
                    std::cout << "Aborting operation." << std::endl;
                    return;
                }
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
    std::string folderPath = "E:/Convert Documents/Convert Files"; // 指定文件夹路径
    std::string targetFolderPath = "E:/Convert Documents/Convert Folder"; // 指定目标文件夹路径
    std::string numberFilePath = "current_number.txt"; // 记录当前序号的文件路径
    std::string dateFilePath = "current_date.txt"; // 记录当前日期的文件路径
    int startNumber = 10001; // 指定起始序号

    createFileIfNotExists(numberFilePath); // 检查并创建记录序号的文件
    createFileIfNotExists(dateFilePath); // 检查并创建记录日期的文件

    renameImagesInFolder(folderPath, startNumber, numberFilePath, dateFilePath); // 重命名图片并更新序号

    std::cout << "Image renaming completed." << std::endl;

    int choice;
    choice = 1;

    if (choice == 1) {
        // 移动图片到指定文件夹
        moveImagesToFolder(folderPath, targetFolderPath);
    }

    return 0;
}

```

------


#include <iostream>
#include <random>
#include <vector>
#include <algorithm>

std::vector<int> generateRandomNumbers(int M) {
    std::random_device rd; // 获取随机种子
    std::mt19937 gen(rd()); // 使用 Mersenne Twister 引擎
    std::uniform_int_distribution<> dis(1, M); // 均匀分布的随机数生成器

    std::vector<int> numbers;
    // 生成M个随机数
    for (int i = 0; i < 9; ++i) {
        numbers.push_back(dis(gen));
    }

    // 为了保证不重复，我们将向量排序并删除重复元素
    std::sort(numbers.begin(), numbers.end());
    numbers.erase(std::unique(numbers.begin(), numbers.end()), numbers.end());

    // 如果生成的数字不够9个，则继续生成直到满足条件
    while (numbers.size() < 9) {
        int new_number = dis(gen);
        if (std::find(numbers.begin(), numbers.end(), new_number) == numbers.end()) {
            numbers.push_back(new_number);
        }
    }

    // 对生成的数字按顺序排序
    std::sort(numbers.begin(), numbers.end());

    return numbers;
}

int main() {
    int M;
    std::cout << "请输入数字M：";
    std::cin >> M;

    if (M <= 8) {
        std::cerr << "输入的数字必须大于8。" << std::endl;
        return 1;
    }

    std::vector<int> randomNumbers = generateRandomNumbers(M);

    std::cout << "随机生成的9个数字按顺序输出为：";
    for (int num : randomNumbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}

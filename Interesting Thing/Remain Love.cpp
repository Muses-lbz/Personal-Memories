#include <iostream>
#include <chrono>
#include <string>
#include <sstream>
#include <ctime>

using namespace std;
using namespace chrono;

// 将日期字符串转换为 time_point 类型
system_clock::time_point stringToTimePoint(const string& date) {
    int year, month, day;
    char delimiter;  // 用来接收 "-" 分隔符

    stringstream ss(date);
    ss >> year >> delimiter >> month >> delimiter >> day;

    // 构造时间点
    tm time = {};
    time.tm_year = year - 1900;  // tm_year 从 1900 年开始
    time.tm_mon = month - 1;     // tm_mon 从 0 开始
    time.tm_mday = day;

    // 将 tm 结构转换为 time_point
    time_t time_since_epoch = mktime(&time);
    return system_clock::from_time_t(time_since_epoch);
}

int main() {
    // 获取今天的日期
    auto today = system_clock::now();
    time_t today_time = system_clock::to_time_t(today);

    tm today_tm;
    localtime_s(&today_tm, &today_time);  // 使用 localtime_s 获取本地时间

    cout << "今天的日期是: " << today_tm.tm_year + 1900 << "-" << today_tm.tm_mon + 1 << "-" << today_tm.tm_mday << endl;

    // 输入目标日期
    string target_date;
    cout << "请输入目标日期 (格式: YYYY-MM-DD): ";
    cin >> target_date;

    // 转换目标日期为 time_point
    system_clock::time_point target_time = stringToTimePoint(target_date);

    // 计算今天和目标日期之间的天数差
    auto duration = today - target_time;
    auto days = duration_cast<std::chrono::hours>(duration).count() / 24;  // 通过小时转换为天数
    cout << "今天与 " << target_date << " 之间相差 " << abs(days) << " 天。" << endl;

    return 0;
}

#include <windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

POINT getInputBoxPosition() {
    HWND hwnd = FindWindowA(NULL, "残虹大府后勤处"); // 替换为实际的窗口标题
    if (hwnd) {
        RECT rect;
        GetWindowRect(hwnd, &rect);
        return { rect.left + 280, rect.top + 51 }; // 根据实际输入框相对位置调整
    }
    return { -1, -1 }; // 未找到窗口
}

void clickAt(int x, int y) {
    SetCursorPos(x, y);
    Sleep(100);
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    Sleep(100);
}

void sendChar(char c) {
    INPUT input = { 0 };
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = VkKeyScan(c) & 0xFF; // 获取虚拟键码
    SendInput(1, &input, sizeof(INPUT)); // 按下键
    input.ki.dwFlags = KEYEVENTF_KEYUP; // 松开键
    SendInput(1, &input, sizeof(INPUT)); // 松开键
}

void sendSpecialChar(WORD vk) {
    INPUT input = { 0 };
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = vk; // 发送特殊字符的虚拟键码
    SendInput(1, &input, sizeof(INPUT)); // 按下键
    input.ki.dwFlags = KEYEVENTF_KEYUP; // 松开键
    SendInput(1, &input, sizeof(INPUT)); // 松开键
}

void sendMessage(const std::string& message) {
    POINT pos = getInputBoxPosition();
    if (pos.x == -1) {
        std::cout << "未找到QQ窗口！" << std::endl;
        return;
    }

    // 点击输入框位置
    clickAt(pos.x, pos.y);
    Sleep(200); // 等待输入框获得焦点

    // 发送斜杠（/）
    sendChar('/');
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 确保输入有时间

    // 发送消息
    for (char c : message) {
        sendChar(c);
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 控制输入速度
    }

    // 发送第一个回车
    sendSpecialChar(VK_RETURN); // 发送回车

    // 发送第二个回车
    sendSpecialChar(VK_RETURN); // 再发送一次回车

    std::cout << "消息发送成功: /" << message << std::endl;
}

int main() {
    std::string message = "修炼";   // 替换为实际消息
    int interval = 65; // 65秒

    while (true) {
        sendMessage(message);
        std::this_thread::sleep_for(std::chrono::seconds(interval));
    }

    return 0;
}

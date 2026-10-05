#include "network.h"
#include "game.h"
#include <graphics.h>
#include <iostream>
#include <thread>
#include <chrono>

// 客户端是白方：连接同一台电脑的服务器，等待黑方先落子。
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "客户端执白；连接 " << NETWORK_IP << ":" << NETWORK_PORT << std::endl;
    Network net;
    // 两个 exe 几乎同时启动，服务器可能还没有开始监听。
    // 最多尝试 10 次，失败后等 200 毫秒；成功即进入棋盘窗口。
    // 使用标准 C++ 延时，避免 EGE 的 Sleep 宏在窗口初始化之前被调用。
    bool connected = false;
    for(int attempt = 0; attempt < 10; attempt++)
    {
        if(net.connectServer(NETWORK_IP))
        {
            connected = true;
            break;
        }
        // 连接失败，等待一段时间再尝试；服务器可能还没启动。
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    if(!connected)
    {
        std::cerr << "连接失败，请先启动 server.exe。按回车退出。" << std::endl;
        std::cin.get();
        return 1;
    }

    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, INIT_RENDERMANUAL);
    setcaption(L"五子棋 - 客户端（白棋）");
    setbkcolor(UI_BACKGROUND);
    Game game(&net, 2);
    game.run();
    closegraph();
    return 0;
}

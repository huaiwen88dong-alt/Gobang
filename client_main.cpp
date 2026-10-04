#include "network.h"
#include "game.h"
#include <graphics.h>
#include <iostream>

// 客户端是白方：连接同一台电脑的服务器，等待黑方先落子。
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "客户端执白；连接 " << NETWORK_IP << ":" << NETWORK_PORT << std::endl;
    Network net;
    if(!net.connectServer(NETWORK_IP))
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
#include "network.h"
#include "game.h"
#include <graphics.h>
#include <iostream>

// 服务器是黑方：先启动本程序，再启动 client.exe。每次运行只进行一局联机。
int main()
{
    //控制台输出中文，防止乱码
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "服务器执黑；地址 " << NETWORK_IP << ":" << NETWORK_PORT << std::endl;
    Network net;
    // 先在控制台等待连接，成功后才打开棋盘，不让游戏窗口卡在 accept 中。
    if(!net.createServer())
    {
        std::cerr << "启动失败，请检查端口是否被占用。按回车退出。" << std::endl;
        std::cin.get();
        return 1;
    }

    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, INIT_RENDERMANUAL);
    setcaption(L"五子棋 - 服务器（黑棋）");
    setbkcolor(UI_BACKGROUND);
    // Game 借用 net 的连接，不负责销毁它；离开 main 时 net 自动关闭。
    Game game(&net, 1);
    game.run();
    closegraph();
    return 0;
}
#ifndef GAME_H
#define GAME_H
#include "save.h"
#include"move.h"
#include "board.h"
#include "ui.h"
#include "AI.h"
#include<vector>
#include<iostream>
//游戏模式
enum GameMode
{
    LOCAL,      //双人模式
    AI_MODE,    //AI对战模式
    NETWORK     //联机模式
};
// 这里仅保存 Network 指针，不需要包含完整网络头文件。
// 具体定义由 game.cpp 在 graphics.h 之前包含，避免原 main 的 Windows 头文件冲突。
class Network;
class Game
{

private:

    Board board;
    UI ui;
    int player;
    bool gameOver;
    int winner;
    int blackWin;
    int whiteWin;
    AI ai;
    //当前游戏模式
    GameMode mode;
    std::vector <Move> history;
    //表示是否在复盘状态
    bool replaying;
    //复盘的当前索引
    int replayIndex;
    //控制复盘播放速度
    int replayCount;

    // nullptr 表示原来的本地玩法；联机入口传入连接，服务器执黑、客户端执白。
    // 连接由 main 中的 Network 对象拥有，Game 只借用，不在这里释放。
    Network* network;
    int myPlayer;
    bool networkDisconnected;
    // 联机单独处理输入、接收和落子，不改原有双人/AI 分支。
    void handleNetworkClick(int x, int y);
    void pollNetwork();
    bool placeNetworkMove(const Move& move);
    void stopNetwork(std::wstring message);



public:
    //构造函数，创建游戏对象
    // 不传参数仍是本地游戏；两个联机入口分别传入 (&net, 1) 和 (&net, 2)。
    Game(Network* connection = nullptr, int localPlayer = 1);
    //游戏主循环
    void run();
    //鼠标点击
    void handleMouse();
    //绘制获胜信息
    void drawWinner();
    //重新开始游戏
    void restart();
    //悔棋
    void undo(); 
    //保存游戏 
    void saveGame();
    //读取游戏
    void loadGame();
    //开始复盘
    void startReplay();
    //复盘每一步
    void replayStep();
    //开始AI模式
    void startAI();
    //AI落子
    void aiMove();
    //开始双人模式
    void startLocal();
    // 点一次联机按钮，自动启动同目录的服务器和客户端。
    void startNetwork();

};


#endif

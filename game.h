#ifndef GAME_H
#define GAME_H
#include "save.h"
#include"move.h"
#include "board.h"
#include "ui.h"
#include<vector>
#include<iostream>
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
    std::vector <Move> history;
    //表示是否在复盘状态
    bool replaying;
    //复盘的当前索引
    int replayIndex;
    //控制复盘播放速度
    int replayCount;



public:
    //构造函数，创建游戏对象
    Game();
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

};


#endif
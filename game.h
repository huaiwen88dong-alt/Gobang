#ifndef GAME_H
#define GAME_H


#include "board.h"
#include "ui.h"

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

};


#endif
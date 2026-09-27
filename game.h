#ifndef GAME_H
#define GAME_H


#include "board.h"


class Game
{

private:

    Board board;

    int player;


public:
    //构造函数，创建游戏对象
    Game();
    //游戏主循环
    void run();
    //鼠标点击
    void handleMouse();

};


#endif
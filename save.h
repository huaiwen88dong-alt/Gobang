#pragma once

#include <vector>
#include "move.h"


class SaveManager
{

public:

    //保存棋局
    static bool save(const std::vector<Move>& history,int blackWin,int whiteWin,int player,bool gameOver,
    int winner);


    //读取棋局
    static bool load(std::vector<Move>& history,int& blackWin,int& whiteWin,int& player,bool& gameOver,int& winner);

};

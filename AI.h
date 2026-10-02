#pragma once

#include "board.h"
#include "move.h"


class AI
{
private:


    int evaluatePosition(Board& board,int row,int col);
    //计算某个方向连续棋子数量
    int countDirection(Board& board,int row,int col,int dx,int dy,int player);
    //评价某个玩家落子后的价值
    int evaluatePlayer(Board& board,int row,int col,int player);

public:

    //获取AI下一步棋
    Move getMove(Board& board);
    



};
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
    //判断某个方向的棋型价值
    //评价某一个方向形成的棋型
//后三个引用参数用于统计本次落子形成的高级组合棋型
int evaluateDirection(Board& board,int row,int col,int dx,int dy,int player,int& liveThree,  int& rushFour,int & liveFour);

public:

    //获取AI下一步棋
    Move getMove(Board& board);
    



};
#include "board.h"
#include <graphics.h>
Board::Board()
{
    init();
}
//每个位置的棋子都清空
void Board::init()
{
    for(int i=0;i<15;i++)
    {
        for(int j=0;j<15;j++)
        {
            chess[i][j]=0;
        }
    }
}
void Board::draw()
{
    setcolor(EGERGB(100,60,20));
    //增加线宽
    setlinewidth(2);
    //画横线
    for(int i=0;i<BOARD_SIZE;i++)
    {
        line(START_X,START_Y+i*GRID,START_X+(BOARD_SIZE-1)*GRID,START_Y+i*GRID);
    }
    //画竖线
    for(int i=0;i<BOARD_SIZE;i++)
    {
        line(START_X+i*GRID,START_Y,START_X+i*GRID,START_Y+(BOARD_SIZE-1)*GRID);
    }
    //画棋子
    for(int i=0;i<BOARD_SIZE;i++)
    {
        for(int j=0;j<BOARD_SIZE;j++)
        {
            if(chess[i][j]==1)
            {
                //1是黑色棋子
                setfillcolor(BLACK);
                //画指定半径为15的实心圆
                solidcircle(START_X+j*GRID,START_Y+i*GRID,15);
            }
            else if(chess[i][j]==2)
            {
                //2是白色棋子
                setfillcolor(WHITE);
                solidcircle(START_X+j*GRID,START_Y+i*GRID,15);
            }
        }
    }
}
//落子函数
bool Board::placeChess(int row,int col,int player)
{
    if(row<0||row>=BOARD_SIZE||col<0||col>=BOARD_SIZE)
    {
        return false;
    }
    if(chess[row][col]!=0)
    {
        return false;
    }
    chess[row][col]=player;
    return true;
}
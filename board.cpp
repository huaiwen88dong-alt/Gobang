#include "board.h"
#include "ui.h" // 仅引用界面尺寸与配色，棋盘数据和落子逻辑不变。
#include <graphics.h>
#include<algorithm>
#include <iostream>
using namespace std;
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
    // 浅木色只覆盖左侧 600×600 棋盘区，避免覆盖右侧暖米色控制面板。
    setfillcolor(UI_BOARD_BACKGROUND);
    bar(0,0,BOARD_AREA_WIDTH,WINDOW_HEIGHT);
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
//判断输赢函数
bool Board::checkWin(int row,int col,int player)
{
    int count=0;
    //判断横向
    count=0;
    for(int i=0;i<BOARD_SIZE;i++)
    {
        if(chess[row][i]==player)
        {
            count++;
            if(count>=5)
            {
                return true;
            }
        }
        else
        {
            count=0;
        }
    }
    //判断纵向
    count=0;
    for(int i=0;i<BOARD_SIZE;i++)
    {
        if(chess[i][col]==player)
        {
            count++;
            if(count>=5)
            {
                return true;
            }
        }
        else
        {
            count=0;
        }
    }
    //判断左上到右下的斜线
    count=0;
    //找左上角起点
    int startRow=row-min(row,col);
    int startCol=col-min(row,col);
    while(startRow<BOARD_SIZE&&startCol<BOARD_SIZE)
    {
        if(chess[startRow][startCol]==player)
        {
            count++;
            if(count>=5)
            {
                return true;
            }
        }
        else
        {
            count=0;
        }
        startRow++;
        startCol++;
    }
    //判断右上到左下的斜线
    count=0;
    //找右上角起点
    int step=min(row,BOARD_SIZE-1-col);
    startRow=row-step;
    startCol=col+step;
    while(startRow<BOARD_SIZE&&startCol>=0)
    {
        if(chess[startRow][startCol]==player)
        {
            count++;
            if(count>=5)
            {
                return true;
            }
        }
        else
        {
            count=0;
        }
        startRow++;
        startCol--;
    }
    return false;
}
//清空棋盘
void Board::clear()
{
    for(int i=0;i<BOARD_SIZE;i++)
    {
        for(int j=0;j<BOARD_SIZE;j++)
        {
            chess[i][j]=0;
        }
    }
  
}
//悔棋函数
void Board::removeChess(int row,int col)
{
    chess[row][col]=0;
}
//获取棋盘某个位置的状态
int Board::getChess(int row,int col)
{
    return chess[row][col];
}

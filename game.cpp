#include"game.h"
#include"ui.h"
#include<graphics.h>
#include<iostream>
using namespace std;
Game::Game()
{
    player=1;
    gameOver=false;
    winner=0;
    blackWin=0;
    whiteWin=0;
}
void Game::run()
{
  
    while(is_run())
    {
        cleardevice();   // 清空画面
        //处理输入
        handleMouse();
        //画棋盘
        board.draw();
        ui.drawInfo(blackWin,whiteWin,player);
        ui.drawMenu();


        if(gameOver)
        {
            ui.drawWinner(winner);
        }

        delay_fps(60);
            
        }
}
void Game::restart()
{

    board.clear();


    player=1;


    gameOver=false;


    winner=0;

}
void Game::handleMouse()
{
    if(mousemsg())
    {
        //获取鼠标点击位置
        mouse_msg msg=getmouse();
        if(msg.is_left()&&msg.is_down())
        {
            //计算点击位置对应的行列
            int x=msg.x;
            int y=msg.y;
            if(ui.checkButtonClick(x,y))
            {
                restart();
                return;
            }
             if(gameOver)
            {
                return;
            }   
            //鼠标坐标转换为棋盘坐标，把交叉点附近20的点位也算进去
            int col=(x-Board::START_X+Board::GRID/2)/Board::GRID;
            int row=(y-Board::START_Y+Board::GRID/2)/Board::GRID;
            if(board.placeChess(row,col,player))
            {
                if(board.checkWin(row,col,player))
                {
                    gameOver=true;
                    winner=player;
                    if(player==1)
                    {
                        blackWin++;
                    }
                    else
                    {
                        whiteWin++;
                    }
                }
                //落子成功，切换玩家
                if(player==1)
                {
                    //黑棋切换为白棋
                    player=2;
                }
                else
                {
                    //白棋切换为黑棋
                    player=1;
                }   
            }
        }
    }
}

#include"game.h"
#include<graphics.h>
Game::Game()
{
    player=1;
}
void Game::run()
{
    while(is_run())
    {
        board.draw();
        handleMouse();
        delay_fps(60);
    }
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
            //鼠标坐标转换为棋盘坐标，把交叉点附近20的点位也算进去
            int col=(x-Board::START_X+Board::GRID/2)/Board::GRID;
            int row=(y-Board::START_Y+Board::GRID/2)/Board::GRID;
            if(board.placeChess(row,col,player))
            {
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
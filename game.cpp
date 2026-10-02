#include"game.h"
#include"ui.h"
#include<graphics.h>
#include<iostream>
#include<string>
using namespace std;
Game::Game()
{
    player=1;
    gameOver=false;
    winner=0;
    blackWin=0;
    whiteWin=0;
    replaying=false;
    replayIndex=0;
    replayCount=0;
    aiMode=false;
}
void Game::run()
{
  
    while(is_run())
    {
        cleardevice();   // 清空画面
        //处理输入
        handleMouse();
        replayStep();
        //画棋盘
        board.draw();
        ui.drawInfo(blackWin,whiteWin,player);
        ui.drawMenu();

        delay_fps(60);
            
        }
}
void Game::restart()
{

    board.clear();
    //清空落子记录
    history.clear();


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
            //判断重新开始按钮是否被点击
            if(ui.checkRestartClick(x,y))
            {
                restart();
                return;
            }
            //保存
            if(ui.checkSaveClick(x,y))
            {
                saveGame();
                return;
            }
            //读取
            if(ui.checkLoadClick(x,y))
            {
                loadGame();
                return;
            }
            //判断复盘按钮是否被点击
            if(ui.checkReplayClick(x,y))
            {
                if(!gameOver)
                {
                    ui.showMessage(L"游戏未结束");
                    return;
                }
                startReplay();
                return;
            }
            //判断AI按钮是否被点击
            if(ui.checkAIClick(x,y))
            {
                startAI();
                return;
            }
            //如果游戏已经结束，点击棋盘不再落子，也不能再悔棋
             if(gameOver)
            {
                return;
            }
            //判断悔棋按钮是否被点击  
            if(ui.checkUndoClick(x,y))
            {
                undo();
                return;
            } 
            //复盘时禁止棋盘操作

            if(replaying)
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
                        ui.showMessage(L"黑棋获胜");
                    }
                    else
                    {
                        whiteWin++;
                        ui.showMessage(L"白棋获胜");
                    }
                }
                //记录落子历史
                Move move;

                move.row=row;

                move.col=col;

                move.player=player;
                history.push_back(move);
                cout<<"player:"<<player<<" row:"<<row<<" col:"<<col<<endl;
                //落子成功，切换玩家
                player=3-player;


                //AI回合
                if(aiMode && player==2)
                {
                    aiMove();
                }
            }
        }
    }
}
void Game::undo()
{

    //没有棋可以悔
    if(history.empty())
    {
        return;
    }


    //取最后一步
    Move last = history.back();


    //删除历史记录
    history.pop_back();


    //棋盘对应位置清空
    board.removeChess(
        last.row,
        last.col
    );


    //恢复玩家
    player = last.player;


}
//保存游戏
void Game::saveGame()
{

    bool success = SaveManager::save(history,blackWin,whiteWin,player,gameOver,winner);


    if(success)
    {
        ui.showMessage(L"保存成功");
    }
    else
    {
        ui.showMessage(L"保存失败");
    }

}
//读取游戏
void Game::loadGame()
{

    if(!SaveManager::load(history,blackWin,whiteWin,player,gameOver,winner))
    {
        return;
    }
    //清空原棋盘
    board.clear();
    //根据历史记录恢复棋盘
    for(auto move:history)
    {
        board.placeChess(move.row,move.col,move.player);
    }
    gameOver=false;
    winner=0;
}
//要开始复盘
void Game::startReplay()
{
    replaying=true;
    replayIndex=0;
    replayCount=0;
    board.clear();
    ui.showMessage(L"开始复盘");
}
void Game::replayStep()
{

    if(!replaying)
    {
        return;
    }
    replayCount++;
    //每60帧播放一步
    if(replayCount<90)
    {
        return;
    }
    replayCount=0;
    //已经播放结束
    if(replayIndex>=history.size())
    {
        replaying=false;

        ui.showMessage(L"复盘结束");

        return;
    }


    //取当前一步
    Move move=history[replayIndex];


    //放回棋盘
    board.placeChess(move.row,move.col,move.player);


    replayIndex++;

}
void Game::startAI()
{
    restart();

    aiMode=true;

    ui.showMessage(L"AI对战开始");

}
//AI落子
void Game::aiMove()
{
    Move move=ai.getMove(board);


    board.placeChess(
        move.row,
        move.col,
        move.player
    );


    history.push_back(move);


    player=1;
}

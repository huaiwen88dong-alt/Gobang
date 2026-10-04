#include "network.h" // Winsock 先于 game.h 内的 UI/graphics 头文件包含。
#include"game.h"
#include"ui.h"
#include<graphics.h>
#include<iostream>
#include<string>
using namespace std;
Game::Game(Network* connection, int localPlayer)
{
    player=1;
    gameOver=false;
    winner=0;
    blackWin=0;
    whiteWin=0;
    replaying=false;
    replayIndex=0;
    replayCount=0;
    //默认进入双人模式
    mode=LOCAL;
    // 本地 main 仍使用 Game game；只有联机入口会传入有效连接。
    network=connection;
    myPlayer=localPlayer;
    networkDisconnected=false;
    if(network != nullptr)
        ui.showMessage(myPlayer == 1 ? L"联机开始，你执黑" : L"联机开始，你执白");
}
void Game::run()
{
  
    while(is_run())
    {
        cleardevice();   // 清空画面
        // 先接收对方落子，再处理本地输入；每帧检查一次，不阻塞窗口。
        pollNetwork();
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
            // 联机的回合限制与操作限制只在此分支生效，本地流程继续走下面原代码。
            if(network != nullptr)
            {
                handleNetworkClick(x,y);
                return;
            }
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
            if(ui.checkLocalClick(x,y))
            {
                startLocal();
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
                if(mode==AI_MODE&& player==2)
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
    //切换到AI模式
    mode=AI_MODE;
    restart();


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
    //检查AI是否获胜
    if(board.checkWin(move.row,move.col,move.player))
    {
        gameOver=true;

        winner=move.player;

        whiteWin++;

        ui.showMessage(L"AI胜利");

        return;
    }


    player=1;
}

// 联机点击只支持落子和保存；其他操作没有同步协议，暂时拒绝，避免两边棋盘不同。
void Game::handleNetworkClick(int x, int y)
{
    if(ui.checkSaveClick(x, y))
    {
        saveGame();
        return;
    }
    if(ui.checkRestartClick(x, y) || ui.checkUndoClick(x, y) ||
       ui.checkLoadClick(x, y) || ui.checkReplayClick(x, y) || ui.checkAIClick(x, y))
    {
        ui.showMessage(L"联机暂不支持此操作");
        return;
    }

    // 右侧是控制区，不能当作棋盘坐标。双人与联机按钮仍只是保留的入口外观。
    if(x < 0 || x >= BOARD_AREA_WIDTH || y < 0 || y >= WINDOW_HEIGHT)
        return;
    if(networkDisconnected)
    {
        ui.showMessage(L"连接已断开，请重新启动");
        return;
    }
    if(gameOver)
        return;
    if(player != myPlayer)
    {
        ui.showMessage(L"请等待对方落子");
        return;
    }

    // 使用现有坐标换算与落子规则；不是本方回合或已有棋子的交点不会发送。
    Move move;
    move.col = (x - Board::START_X + Board::GRID / 2) / Board::GRID;
    move.row = (y - Board::START_Y + Board::GRID / 2) / Board::GRID;
    move.player = myPlayer;
    if(!placeNetworkMove(move))
        return;
    if(!network->sendMove(move))
        stopNetwork(L"发送失败，连接已断开");
}

// 每帧尝试接收一步棋。0 表示暂无完整消息，直接返回，窗口继续绘制和响应。
void Game::pollNetwork()
{
    if(network == nullptr || networkDisconnected)
        return;

    Move move;
    const int result = network->receiveMove(move);
    if(result == 0)
        return;
    if(result < 0)
    {
        stopNetwork(L"对方已断开连接");
        return;
    }

    // 只接受对手棋色、正确回合和空交点，不能让错误网络数据进入胜负判断。
    if(move.player != 3 - myPlayer || !placeNetworkMove(move))
        stopNetwork(L"收到无效落子，连接已关闭");
}

// 双方使用同一段联机落子处理，因此历史、胜者、比分和下一个回合保持一致。
// Board 的落子与胜负算法不变，这里只是调用现有接口并更新 Game 的状态。
bool Game::placeNetworkMove(const Move& move)
{
    if(gameOver || move.player != player)
        return false;
    if(!board.placeChess(move.row, move.col, move.player))
        return false;

    history.push_back(move);
    if(board.checkWin(move.row, move.col, move.player))
    {
        gameOver = true;
        winner = move.player;
        if(winner == 1)
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
    player = 3 - player;
    return true;
}

// 出错后停止继续收发和落子，保留当前棋盘供查看或保存，不自动重连。
void Game::stopNetwork(std::wstring message)
{
    networkDisconnected = true;
    network->close();
    ui.showMessage(message);
}
// 联机模式下，点击双人按钮切换到本地双人模式，保留当前棋盘和比分。
void Game::startLocal()
{
    //切换到双人模式
    mode=LOCAL;

    //重新开始
    restart();
}

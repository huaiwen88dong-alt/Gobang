#include "network.h" // Winsock 先于 game.h 内的 UI/graphics 头文件包含。
#include"game.h"
#include"ui.h"
#include<graphics.h>
#include<iostream>
#include<string>
#include <shellapi.h> // 使用 Windows 自带的 ShellExecuteW 启动两个联机 exe。
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
    mode=connection == nullptr ? LOCAL : NETWORK;
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
        cleardevice(); 
        pollNetwork();
        handleMouse();
        replayStep();
        board.draw();
        ui.drawInfo(blackWin,whiteWin,player);
        ui.drawMenu();

        delay_fps(60);
            
        }
}
// 重开只清空本局状态，不改变当前模式和累计比分；同时停止旧棋局的复盘。
void Game::restart()
{

    board.clear();
    //清空落子记录
    history.clear();


    player=1;


    gameOver=false;


    winner=0;
    replaying=false;
    replayIndex=0;
    replayCount=0;

}
void Game::handleMouse()
{
    if(mousemsg())
    {
        mouse_msg msg=getmouse();
        if(msg.is_left()&&msg.is_down())
        {
            int x=msg.x;
            int y=msg.y;
            if(ui.checkNetworkClick(x,y))
            {
                startNetwork();
                return;
            }
            if(network != nullptr)
            {
                handleNetworkClick(x,y);
                return;
            }
            if(ui.checkRestartClick(x,y))
            {
                restart();
                return;
            }
            if(ui.checkSaveClick(x,y))
            {
                saveGame();
                return;
            }
            if(ui.checkLoadClick(x,y))
            {
                loadGame();
                return;
            }
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
            if(ui.checkAIClick(x,y))
            {
                startAI();
                return;
            }
            if(ui.checkLocalClick(x,y))
            {
                startLocal();
                return;
            }
             if(gameOver)
            {
                return;
            }
            if(ui.checkUndoClick(x,y))
            {
                undo();
                return;
            } 
            if(replaying)
            {
                return;
            }
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
                        board.draw();
                        ui.drawInfo(blackWin, whiteWin, player);
                        ui.drawMenu();
                        flushwindow();
                        MessageBoxW(getHWnd(), L"黑棋获胜！", L"游戏结束", MB_OK);
                    }
                    else
                    {
                        whiteWin++;
                        ui.showMessage(L"白棋获胜");
                        ui.drawInfo(blackWin, whiteWin, player);
                        ui.drawMenu();
                        flushwindow();
                        MessageBoxW(getHWnd(), L"白棋获胜！", L"游戏结束", MB_OK);
                    }
                }
                Move move;

                move.row=row;

                move.col=col;

                move.player=player;
                history.push_back(move);
                cout<<"player:"<<player<<" row:"<<row<<" col:"<<col<<endl;
                player=3-player;
                if(mode==AI_MODE&& player==2 && !gameOver)
                {
                    aiMove();
                }
            }
        }
    }
}
// 双人悔一步；AI 悔一个回合。如果玩家刚落子而 AI 尚未落子，只撤销玩家这一步。
void Game::undo()
{

    //没有棋可以悔
    if(history.empty())
    {
        return;
    }


    // AI 执白：末步为白棋说明一个回合已完成，需要连同前面的玩家黑棋一起撤销。
    int steps = 1;
    if(mode == AI_MODE && history.back().player == 2)
        steps = 2;
    for(int i = 0; i < steps && !history.empty(); i++)
    {
        Move last = history.back();
        history.pop_back();
        board.removeChess(last.row, last.col);
        // 每撤销一步，同时恢复该步落子前的执棋方。
        player = last.player;
    }
    // AI 模式中的人类固定执黑，悔棋后由玩家重新选择落点。
    if(mode == AI_MODE)
        player = 1;


}
// 保存原有棋局数据，并额外保存模式编号；不改变 AI 或联机的执行方式。
void Game::saveGame()
{

    bool success = SaveManager::save(history,blackWin,whiteWin,player,gameOver,winner,mode);


    if(success)
    {
        ui.showMessage(L"保存成功");
    }
    else
    {
        ui.showMessage(L"保存失败");
    }

}
// 完整读取成功后才替换当前棋局，恢复模式、执棋方、胜负状态和历史。
void Game::loadGame()
{

    vector<Move> loadedHistory;
    int loadedBlackWin, loadedWhiteWin, loadedPlayer, loadedWinner, loadedMode;
    bool loadedGameOver;
    if(!SaveManager::load(loadedHistory,loadedBlackWin,loadedWhiteWin,loadedPlayer,
                          loadedGameOver,loadedWinner,loadedMode))
    {
        ui.showMessage(L"读取失败");
        return;
    }
    // 存档只能保存棋局，不能恢复 TCP 连接；本地窗口不接管联机存档。
    if(loadedMode == NETWORK && network == nullptr)
    {
        ui.showMessage(L"联机存档需要联机连接");
        return;
    }
    history = loadedHistory;
    blackWin = loadedBlackWin;
    whiteWin = loadedWhiteWin;
    player = loadedPlayer;
    gameOver = loadedGameOver;
    winner = loadedWinner;
    mode = static_cast<GameMode>(loadedMode);
    // 读取新棋局时结束旧复盘，防止下一帧继续播放旧的索引。
    replaying = false;
    replayIndex = 0;
    replayCount = 0;
    //清空原棋盘
    board.clear();
    //根据历史记录恢复棋盘
    for(auto move:history)
    {
        board.placeChess(move.row,move.col,move.player);
    }
    // 若存档恰好停在 AI 回合，调用原有 AI 落子函数完成这一步，避免玩家代下白棋。
    if(mode == AI_MODE && player == 2 && !gameOver)
        aiMove();
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
// 先设置 AI 模式，再用统一重开函数重置本局，避免沿用双人模式的历史和回合。
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
          // 先画出最后一步棋及右侧信息，避免弹窗出现时棋子还没显示。
            board.draw();
            ui.drawInfo(blackWin, whiteWin, player);
            ui.drawMenu();

            // 当前使用手动刷新模式，把刚画好的内容显示到窗口。
            flushwindow();
        // AI 获胜时弹窗；这里只显示结果，不改变 AI 的选点算法。
        MessageBoxW(getHWnd(), L"AI获胜！", L"游戏结束", MB_OK);

        return;
    }


    player=1;
}

// 联机只支持落子和同步重开；保存等操作统一提示不支持，不写入存档。
void Game::handleNetworkClick(int x, int y)
{
    // 一方点击就清空本方，并通知对方清空；不等待回复，比分与连接保持不变。
    if(ui.checkRestartClick(x, y))
    {
        if(networkDisconnected)
        {
            ui.showMessage(L"连接已断开，请重新启动");
            return;
        }
        if(!network->sendRestart())
        {
            stopNetwork(L"重开发送失败，连接已断开");
            return;
        }
        restart();
        ui.showMessage(L"重新开始，黑棋先行");
        return;
    }
    // 保存按钮也进入禁止分支；本地双人和 AI 的保存入口保持不变。
    if(ui.checkSaveClick(x, y) || ui.checkUndoClick(x, y) ||
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

    // 特殊标记只负责清空本方，不再次发送，否则两边会来回通知。
    // 放在普通落子检查之前，因此已经获胜的棋局也可以重开。
    if(move.row == 255 && move.col == 255 && move.player == 0)
    {
        restart();
        ui.showMessage(L"对方重新开始，黑棋先行");
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
            // 先画出最后一步棋及右侧信息，避免弹窗出现时棋子还没显示。
            board.draw();
            ui.drawInfo(blackWin, whiteWin, player);
            ui.drawMenu();

            // 当前使用手动刷新模式，把刚画好的内容显示到窗口。
            flushwindow();
            // 联机黑棋获胜时在当前窗口弹出结果提示。
            MessageBoxW(getHWnd(), L"黑棋获胜！", L"游戏结束", MB_OK);
        }
        else
        {
            whiteWin++;
            ui.showMessage(L"白棋获胜");
              // 先画出最后一步棋及右侧信息，避免弹窗出现时棋子还没显示。
            board.draw();
            ui.drawInfo(blackWin, whiteWin, player);
            ui.drawMenu();

            // 当前使用手动刷新模式，把刚画好的内容显示到窗口。
            flushwindow();
            // 联机白棋获胜时在当前窗口弹出结果提示。
            MessageBoxW(getHWnd(), L"白棋获胜！", L"游戏结束", MB_OK);
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
// 切换到双人模式后重置本局；累计比分保留，棋盘、历史和执棋方重新开始。
void Game::startLocal()
{
    //切换到双人模式
    mode=LOCAL;

    //重新开始
    restart();
    ui.showMessage(L"双人模式开始");
}

// 本地测试入口：自动打开两个联机窗口，当前窗口和当前棋局保持原样。
void Game::startNetwork()
{
    if(network != nullptr)
    {
        ui.showMessage(L"当前已经是联机对战");
        return;
    }

    // 获取当前 exe 所在文件夹，保证从不同工作目录启动也能找到联机程序。
    wchar_t path[MAX_PATH];
    DWORD length = GetModuleFileNameW(nullptr, path, MAX_PATH);
    if(length == 0 || length >= MAX_PATH)
    {
        ui.showMessage(L"无法获取程序路径");
        return;
    }
    wstring folder = path;
    folder = folder.substr(0, folder.find_last_of(L"\\/"));
    wstring server = folder + L"\\server.exe";
    wstring client = folder + L"\\client.exe";

    // 先检查两个文件都存在，再启动，避免缺少客户端时只打开一个等待窗口。
    if(GetFileAttributesW(server.c_str()) == INVALID_FILE_ATTRIBUTES ||
       GetFileAttributesW(client.c_str()) == INVALID_FILE_ATTRIBUTES)
    {
        ui.showMessage(L"请先编译两个联机程序");
        return;
    }

    // 先开服务器，再开客户端；客户端会自行重试连接，这里不需要等待或弹选择框。
    // ShellExecuteW 的返回值大于 32 表示启动成功；folder 也作为新程序工作目录。
    if((INT_PTR)ShellExecuteW(nullptr, L"open", server.c_str(), nullptr,
                             folder.c_str(), SW_SHOWNORMAL) <= 32)
    {
        ui.showMessage(L"服务器启动失败");
        return;
    }
    if((INT_PTR)ShellExecuteW(nullptr, L"open", client.c_str(), nullptr,
                             folder.c_str(), SW_SHOWNORMAL) <= 32)
    {
        ui.showMessage(L"客户端启动失败");
        return;
    }
    ui.showMessage(L"两个联机窗口已启动");
}

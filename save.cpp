#include "save.h"

#include <fstream>


using namespace std;

// 保留原来的存档顺序，在所有落子之后追加模式编号，兼容旧存档的读取。
bool SaveManager::save(const vector<Move>& history,int blackWin,int whiteWin,int player,bool gameOver,int winner,int mode)
{
    //输出文件流
    ofstream file("save.txt");
    if(!file.is_open())
    {
        return false;
    }
    //保存比分
    file<<blackWin<<" "<<whiteWin<<endl;
    //保存当前玩家
    file<<player<<endl;
    //保存游戏状态
    file<<gameOver<<endl;
    //保存胜者
    file<<winner<<endl;
    //保存棋子数量
    file<<history.size()<<endl;
    //保存每一步
    for(auto move:history)
    {
        file<<move.row<<" "<<move.col<<" "<<move.player<<endl;
    }
    // 模式独立放在最后：0 双人、1 AI、2 联机。
    file<<mode<<endl;
    file.close();
    // 写入或关闭文件失败时，不能向 Game 报告保存成功。
    return !file.fail();
}
// 先读取到临时变量并检查完整性，避免损坏存档把当前棋局更新到一半。
bool SaveManager::load(vector<Move>& history,int& blackWin,int& whiteWin,int& player,bool& gameOver,int& winner,int& mode)
{

    ifstream file("save.txt");


    if(!file.is_open())
    {
        return false;
    }


    int savedBlackWin, savedWhiteWin, savedPlayer, savedWinner, size;
    bool savedGameOver;
    if(!(file>>savedBlackWin>>savedWhiteWin>>savedPlayer>>savedGameOver>>savedWinner>>size))
        return false;
    // 只检查存档字段范围，不在保存模块重新实现胜负判断。
    if(savedBlackWin < 0 || savedWhiteWin < 0 || savedPlayer < 1 || savedPlayer > 2 ||
       savedWinner < 0 || savedWinner > 2 || size < 0 || size > 225)
        return false;
    vector<Move> savedHistory;
    // 同一个交点不能保存两枚棋子，保证之后按历史重建棋盘不会丢失某一步。
    bool occupied[15][15] = {};
    for(int i=0;i<size;i++)
    {

        Move move;


        if(!(file>>move.row>>move.col>>move.player))
            return false;
        if(move.row < 0 || move.row >= 15 || move.col < 0 || move.col >= 15 ||
           (move.player != 1 && move.player != 2) || occupied[move.row][move.col])
            return false;
        occupied[move.row][move.col] = true;
        savedHistory.push_back(move);

    }


    // 旧存档在历史之后就结束，默认双人；新存档读取末尾的模式编号。
    int savedMode = 0;
    file>>ws;
    if(!file.eof() && (!(file>>savedMode) || savedMode < 0 || savedMode > 2))
        return false;
    // 所有数据读取成功后一次性交给调用方，失败时原有参数保持不变。
    history = savedHistory;
    blackWin = savedBlackWin;
    whiteWin = savedWhiteWin;
    player = savedPlayer;
    gameOver = savedGameOver;
    winner = savedWinner;
    mode = savedMode;
    return true;

}

#include "save.h"

#include <fstream>


using namespace std;

//保存棋局
bool SaveManager::save(const vector<Move>& history,int blackWin,int whiteWin,int player)
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
    //保存棋子数量
    file<<history.size()<<endl;
    //保存每一步
    for(auto move:history)
    {
        file<<move.row<<" "<<move.col<<" "<<move.player<<endl;
    }
    file.close();
    return true;
}
//读取棋局
bool SaveManager::load(vector<Move>& history,int& blackWin,int& whiteWin,int& player)
{
    ifstream file("save.txt");
    if(!file.is_open())
    {
        return false;
    }
    //读取比分
    file >> blackWin >> whiteWin;
    //读取当前玩家
    file >> player;
    //读取棋子数量
    int size;
    file >> size;
    history.clear();
    //读取每一步棋
    for(int i=0;i<size;i++)
    {
        Move move;
        file >> move.row>> move.col>> move.player;
        history.push_back(move);
    }
    file.close();
    return true;
}

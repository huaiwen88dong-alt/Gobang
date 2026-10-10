#pragma once

#include <vector>
#include "move.h"


class SaveManager
{

public:

    // 保存棋局及模式编号（0 双人、1 AI、2 联机）；使用整数避免依赖 Game 头文件。
    static bool save(const std::vector<Move>& history,int blackWin,int whiteWin,int player,bool gameOver,
    int winner,int mode);


    // 读取成功才更新参数；旧存档没有模式编号时默认双人模式。
    static bool load(std::vector<Move>& history,int& blackWin,int& whiteWin,int& player,bool& gameOver,int& winner,int& mode);

};

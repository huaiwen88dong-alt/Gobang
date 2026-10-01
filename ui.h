#pragma once
#include "button.h"
class UI
{
private:

    Button restartButton;
    Button aiButton;
    Button networkButton;
    Button saveButton;
    Button loadButton;
    Button replayButton;

public:
    UI();



    //绘制右侧信息
    void drawInfo();
    //显示右侧信息
    void drawInfo(int blackWin,int whiteWin,int player);
    //显示胜利信息
    void drawWinner(int winner);
    //绘制右侧菜单
    void drawMenu();
    //判断按钮是否被点击
    bool checkButtonClick(int x,int y);


};
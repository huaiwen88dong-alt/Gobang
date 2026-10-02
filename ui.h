#pragma once
#include "button.h"
#include<string>
class UI
{
private:

    Button restartButton;
    Button aiButton;
    Button networkButton;
    Button saveButton;
    Button undoButton;
    Button replayButton;
    Button loadButton;
    std::wstring message;
    int messageTime;

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
    //判断重新开始按钮是否被点击
    bool checkRestartClick(int x,int y);
    //判断悔棋按钮是否被点击
    bool checkUndoClick(int x,int y);
    //判断保存按钮是否被点击
    bool checkSaveClick(int x,int y);
    //判断读取按钮是否被点击
    bool checkLoadClick(int x,int y);
    //判断复盘按钮是否被点击
    bool checkReplayClick(int x,int y);
    //显示提示信息
    void showMessage(std::wstring msg);
    //判断AI按钮是否被点击
    bool checkAIClick(int x,int y);



};
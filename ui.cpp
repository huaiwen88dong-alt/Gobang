#include "ui.h"
#include <graphics.h>
#include <string>
using namespace std;

    UI::UI():
        restartButton(625,300,150,40,L"重新开始"),
        aiButton(625,350,150,40,L"AI对战"),
        networkButton(625,400,150,40,L"联机对战"),
        saveButton(625,450,150,40,L"保存棋局"),
        loadButton(625,500,150,40,L"读取棋局"),
        replayButton(625,550,150,40,L"复盘")
    {

        

    }


//显示信息栏
void UI::drawInfo(int blackWin,int whiteWin,int player)
{

    setbkmode(TRANSPARENT);


    setcolor(BLACK);


    setfont(25,0,"Microsoft YaHei");


    outtextxy(650,80,L"五子棋");


    if(player==1)
    {
        outtextxy(650,140,L"当前:黑棋");
    }
    else
    {
        outtextxy(650,140,L"当前:白棋");
    }


   
    wstring blackText=L"黑胜:"+to_wstring(blackWin);
    outtextxy(650,180,blackText.c_str());
    wstring whiteText=L"白胜:"+to_wstring(whiteWin);
    outtextxy(650,220,whiteText.c_str());
}
//显示胜利
void UI::drawWinner(int winner)
{

    setbkmode(TRANSPARENT);


    setfont(40,0,"Microsoft YaHei");


    setcolor(RGB(0,0,139));


    if(winner==1)
    {
        outtextxy(630,100,L"黑棋获胜！");
    }
    else if(winner==2)
    {
        outtextxy(630,100,L"白棋获胜！");
    }
}
//绘制右侧菜单
void UI::drawMenu()
{

    restartButton.draw();

    aiButton.draw();

    networkButton.draw();

    saveButton.draw();

    loadButton.draw();

    replayButton.draw();

}
//判断按钮是否被点击
bool UI::checkButtonClick(int x,int y)
{

    if(restartButton.isClicked(x,y))
    {
        return true;
    }


    return false;

}
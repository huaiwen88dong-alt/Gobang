#include "ui.h"
#include <graphics.h>
#include <string>
using namespace std;

UI::UI():
    restartButton(620,260,140,35,L"重新开始"),
    undoButton(620,305,140,35,L"悔棋"),
    aiButton(620,350,140,35,L"AI对战"),
    networkButton(620,395,140,35,L"联机对战"),
    saveButton(620,440,140,35,L"保存棋局"),
    loadButton(620,485,140,35,L"读取棋局"),
    replayButton(620,530,140,35,L"复盘")
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
    if(!message.empty())
    {
        setcolor(BLACK);

        setfont(30,0,"宋体");

        outtextxy(630,40,message.c_str());
    }
}
//显示胜利
/*void UI::drawWinner(int winner)
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
}*/
//绘制右侧菜单
void UI::drawMenu()
{

  restartButton.draw();

    undoButton.draw();

    aiButton.draw();

    networkButton.draw();

    saveButton.draw();

    loadButton.draw();

    replayButton.draw();

}
//判断按钮是否被点击
bool UI::checkRestartClick(int x,int y)
{

    if(restartButton.isClicked(x,y))
    {
        return true;
    }


    return false;

}
//判断悔棋按钮是否被点击
bool UI::checkUndoClick(int x,int y)
{
    if(undoButton.isClicked(x,y))
    {
        return true;
    }

    return false;
}
//判断保存按钮是否被点击
bool UI::checkSaveClick(int x,int y)
{
    if(saveButton.isClicked(x,y))
    {
        return true;
    }
    return false;
}
//判断读取按钮是否被点击
bool UI::checkLoadClick(int x,int y)
{
    if(loadButton.isClicked(x,y))
    {
        return true;
    }

    return false;
}
//
void UI::showMessage(wstring msg)
{
    message=msg;
}
#include "button.h"

#include <graphics.h>

//Button类的构造函数，初始化按钮的位置、大小和文字
Button::Button(int x,int y,int width,int height,std::wstring text)
{
    this->x=x;
    this->y=y;
    this->width=width;
    this->height=height;
    this->text=text;

}
void Button::draw()
{
    //背景颜色
    setfillcolor(RGB(170,210,240));
    bar(x,y,x+width,y+height);
    //边框
    setcolor(RGB(80,120,160));
    rectangle(x,y,x+width,y+height);
    //文字
    setcolor(BLACK);
    setfont(22,0,"Microsoft YaHei");
    setbkmode(TRANSPARENT);
    outtextxy(x+25,y+8,text.c_str());
}
//判断鼠标是否点击按钮
bool Button::isClicked(int mouseX,int mouseY)
{

    if( mouseX>=x &&mouseX<=x+width &&mouseY>=y &&mouseY<=y+height)
    {
        return true;
    }
    return false;

}
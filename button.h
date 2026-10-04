#pragma once

#include <string>


class Button
{

private:

    //按钮位置
    int x;
    int y;


    //按钮大小
    int width;
    int height;


    //按钮文字
    std::wstring text;

    // true 使用浅绿色，false 使用奶油色；只控制外观，不表示当前游戏模式。
    bool isModeButton;


public:


    Button(int x,int y,int width,int height,std::wstring text,
           bool isModeButton = false);


    //绘制按钮
    void draw();


    //判断鼠标是否点击
    bool isClicked(int mouseX,int mouseY);

};

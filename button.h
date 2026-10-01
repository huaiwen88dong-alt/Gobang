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


public:


    Button(int x,int y,int width,int height,std::wstring text);


    //绘制按钮
    void draw();


    //判断鼠标是否点击
    bool isClicked(int mouseX,int mouseY);

};
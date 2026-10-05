#include "button.h"
#include "ui.h"
#include <graphics.h>

// 构造按钮时保存左上角坐标、宽高、文字和固定配色，不处理任何游戏操作。
// 最后一个参数是布尔值：模式按钮传 true，普通按钮省略时默认 false。
Button::Button(int x, int y, int width, int height, std::wstring text, bool isModeButton)
    : x(x), y(y), width(width), height(height), text(text), isModeButton(isModeButton)
{
}

// 绘制分为三步：确定颜色、绘制按钮外形、将文字放到中心。
// 不读取鼠标位置，所以鼠标停留时按钮颜色始终不变。
void Button::draw()
{
    // 普通操作使用奶油色；双人、AI、联机都使用完全相同的浅绿色。
    if(isModeButton)
    {
        setfillcolor(UI_MODE_BACKGROUND);
    }
    else
    {
        setfillcolor(UI_BUTTON_BACKGROUND);
    }

    // right/bottom 是最后一个可见像素，因此要减 1。
    // 只用 bar 绘制实心矩形，不绘制边框，底色沿用上面的设置。
    const int right = x + width - 1;
    const int bottom = y + height - 1;
    bar(x, y, right, bottom);

    // 测量当前字体的文字宽高，再把剩余空间平分到两边，实现水平、垂直居中。
    if(isModeButton)
        setcolor(UI_MODE_TEXT);
    else
        setcolor(UI_TEXT);
    setTextFont(24);
    setbkmode(TRANSPARENT);
    const int textX = x + (width - textwidth(text.c_str())) / 2;
    const int textY = y + (height - textheight(text.c_str())) / 2;
    outtextxy(textX, textY, text.c_str());
}

// 鼠标位于矩形内（包含四个角）就算命中，与绘制的像素范围一致。
// 这里只返回是否命中；重新开始、悔棋等动作仍由 Game 执行。
bool Button::isClicked(int mouseX, int mouseY)
{
    if(mouseX < x || mouseX >= x + width || mouseY < y || mouseY >= y + height)
        return false;

    return true;
}

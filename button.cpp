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
        setcolor(UI_MODE_BORDER);
    }
    else
    {
        setfillcolor(UI_BUTTON_BACKGROUND);
        setcolor(UI_BUTTON_BORDER);
    }
    setlinewidth(1);

    // right/bottom 是最后一个可见像素，因此要减 1。
    // 从左上方开始顺时针列出八个顶点；四角各切掉 corner 像素。
    // 相比逐行计算圆弧，这种近似圆角只需一个多边形，便于阅读和修改。
    const int right = x + width - 1;
    const int bottom = y + height - 1;
    const int corner = BUTTON_CORNER;
    const int points[] = {
        x + corner, y, right - corner, y,
        right, y + corner, right, bottom - corner,
        right - corner, bottom, x + corner, bottom,
        x, bottom - corner, x, y + corner
    };
    // fillpoly 同时绘制填充与边框，颜色由上面的 setfillcolor/setcolor 决定。
    fillpoly(8, points);

    // 测量当前字体的文字宽高，再把剩余空间平分到两边，实现水平、垂直居中。
    if(isModeButton)
        setcolor(UI_MODE_TEXT);
    else
        setcolor(UI_TEXT);
    setTextFont(20);
    setbkmode(TRANSPARENT);
    const int textX = x + (width - textwidth(text.c_str())) / 2;
    const int textY = y + (height - textheight(text.c_str())) / 2;
    outtextxy(textX, textY, text.c_str());
}

// 先检查外接矩形，再排除四个被切掉的角，保证点击区域与多边形外观一致。
// 这里只返回是否命中；重新开始、悔棋等动作仍由 Game 执行。
bool Button::isClicked(int mouseX, int mouseY)
{
    if(mouseX < x || mouseX >= x + width || mouseY < y || mouseY >= y + height)
        return false;

    // 计算鼠标到最近水平边、竖直边的距离；两者之和小于切角尺寸即在透明角落。
    const int leftDistance = mouseX - x;
    const int rightDistance = x + width - 1 - mouseX;
    const int topDistance = mouseY - y;
    const int bottomDistance = y + height - 1 - mouseY;
    const int horizontalDistance = leftDistance < rightDistance ? leftDistance : rightDistance;
    const int verticalDistance = topDistance < bottomDistance ? topDistance : bottomDistance;
    return horizontalDistance + verticalDistance >= BUTTON_CORNER;
}

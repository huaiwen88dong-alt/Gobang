#pragma once
#include "button.h"
#include <string>
#include <graphics.h>

// const 表示初始化后不能修改。这里只保留多处共用的尺寸与颜色。
// 坐标从窗口左上角开始：x 向右，y 向下，单位是像素。
// 窗口 1000×600，左侧棋盘仍占 600×600，右侧增加的空间用于控制面板。
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 600;
const int BOARD_AREA_WIDTH = 600;

// 内容区从 x=632 开始，宽 336，左右各留 32 像素空白。
const int PANEL_X = 632;
const int PANEL_WIDTH = 336;

// 三列：104×3 + 12×2 = 336；两列：162×2 + 12 = 336。
// 所有按钮高 42，间隔 12；切角尺寸同时用于绘制和点击判断。
const int BUTTON_HEIGHT = 42;
const int BUTTON_GAP = 12;
const int SMALL_BUTTON_WIDTH = 104;
const int LARGE_BUTTON_WIDTH = 162;
const int BUTTON_CORNER = 4;

// UI_ 前缀说明这些颜色用于界面，避免与其他文件里的名称混淆。
// EGERGB 的三个参数分别是红、绿、蓝，数值范围为 0～255。
const color_t UI_BACKGROUND = EGERGB(248, 242, 227);       // 窗口和右侧面板的暖米色
const color_t UI_BOARD_BACKGROUND = EGERGB(236, 221, 184); // 左侧棋盘的浅木色
const color_t UI_TEXT = EGERGB(68, 60, 47);               // 标题、正文与普通按钮文字
const color_t UI_BUTTON_BACKGROUND = EGERGB(251, 247, 236); // 普通操作按钮的奶油色
const color_t UI_BUTTON_BORDER = EGERGB(198, 183, 150);     // 普通按钮的棕灰边框
const color_t UI_MODE_BACKGROUND = EGERGB(227, 230, 211);   // 三个模式按钮共用浅绿色
const color_t UI_MODE_BORDER = EGERGB(164, 173, 140);       // 模式按钮的灰绿边框
const color_t UI_MODE_TEXT = EGERGB(69, 76, 54);            // 模式按钮的深绿文字

// 这里只声明函数，具体字体设置写在 ui.cpp，按钮绘制也可以调用它。
// 参数是字符的像素高度，例如 setTextFont(20) 设置 20 像素文字。
void setTextFont(int pixelHeight);
class UI
{
private:

    // 仅预留双人入口的外观，不向 Game 增加模式切换或鼠标处理逻辑。
    Button localButton;

    Button restartButton;
    Button aiButton;
    Button networkButton;
    Button saveButton;
    Button undoButton;
    Button replayButton;
    Button loadButton;
    // message 保存 Game 传来的提示；messageTime 表示还要显示多少帧。
    // UI 不决定保存是否成功、谁获胜，只负责把传来的内容显示出来。
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
    //判断双人按钮是否被点击
    bool checkLocalClick(int x,int y);



};

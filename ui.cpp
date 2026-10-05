#include "ui.h"
#include <graphics.h>
#include <string>
using namespace std;

// 统一设置中文字体；ui.h 中声明了本函数，button.cpp 也能直接使用。
// 保留这个函数是为了避免每次画文字都重复一整段字体设置。
// 统一设置微软雅黑，字符宽度自动确定。
void setTextFont(int pixelHeight)
{
    setfont(pixelHeight, 0, "Microsoft YaHei");
}

// static 表示这个辅助函数仅在 ui.cpp 内使用，Game 不需要调用它。
static void drawSection(const wchar_t* title, int y)
{
    setTextFont(22);
    setcolor(EGERGB(95, 85, 68));
    outtextxy(PANEL_X, y, title);
}
// 细线仅划分区域，不增加阴影和装饰。
// y 是横线的高度；从内容区左端画到右端，总长为 PANEL_WIDTH。
static void drawSeparator(int y)
{
    setlinewidth(1);
    setcolor(EGERGB(220, 208, 183));
    line(PANEL_X, y, PANEL_X + PANEL_WIDTH, y);
}

// 三列模式与棋局按钮、两列游戏操作共享间距；Game 调用接口保持原样。
// 冒号后是成员初始化列表：在 UI 创建时，直接创建各个 Button 成员。
// Button 参数依次为：左上角 x、左上角 y、宽、高、文字、是否使用浅绿色。
// 第一列 x=PANEL_X；第二列增加“按钮宽度+间隔”；第三列增加两倍该距离。
// 最后一个参数为 true 时使用浅绿色，不写时默认 false，使用普通奶油色。
// 模式、操作、管理三行的顶部 y 分别是 255、378、467，下方标题注释也标明了位置。
UI::UI():
    localButton(PANEL_X, 255,
                SMALL_BUTTON_WIDTH, BUTTON_HEIGHT,
                L"双人对战", true),
    restartButton(PANEL_X, 378,
                  LARGE_BUTTON_WIDTH, BUTTON_HEIGHT, L"重新开始"),
    aiButton(PANEL_X + SMALL_BUTTON_WIDTH + BUTTON_GAP,
             255, SMALL_BUTTON_WIDTH, BUTTON_HEIGHT,
             L"AI 对战", true),
    networkButton(PANEL_X + 2 * (SMALL_BUTTON_WIDTH + BUTTON_GAP),
                  255, SMALL_BUTTON_WIDTH, BUTTON_HEIGHT,
                  L"联机对战", true),
    saveButton(PANEL_X, 467,
               SMALL_BUTTON_WIDTH, BUTTON_HEIGHT, L"保存棋局"),
    undoButton(PANEL_X + LARGE_BUTTON_WIDTH + BUTTON_GAP,
               378, LARGE_BUTTON_WIDTH, BUTTON_HEIGHT, L"悔棋"),
    replayButton(PANEL_X + 2 * (SMALL_BUTTON_WIDTH + BUTTON_GAP),
                 467, SMALL_BUTTON_WIDTH, BUTTON_HEIGHT, L"复盘"),
    loadButton(PANEL_X + SMALL_BUTTON_WIDTH + BUTTON_GAP,
               467, SMALL_BUTTON_WIDTH, BUTTON_HEIGHT, L"读取棋局")
{
    // 刚启动时没有提示；收到 showMessage 后才开始显示底部消息。
    messageTime = 0;
}

// Game 在棋盘之后调用本函数；只填右侧背景，避免覆盖已绘制的网格和棋子。
// 三个参数都由 Game 提供：黑方累计胜局、白方累计胜局、当前玩家（1 黑、2 白）。
// 阅读时按下面顺序理解：右侧底色 → 标题 → 状态区 → 底部提示。
void UI::drawInfo(int blackWin, int whiteWin, int player)
{
    // 1. 每帧重画右侧底色，覆盖上一帧的文字，防止旧比分和提示残留。
    // bar 的参数是矩形左上角与右下角，不是“坐标加宽高”的形式。
    setfillcolor(UI_BACKGROUND);
    bar(BOARD_AREA_WIDTH, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    setlinewidth(1);
    setcolor(EGERGB(220, 208, 183));
    // 棋盘与面板之间留出空隙，用一条细线分隔。
    line(BOARD_AREA_WIDTH + 10, 24,
         BOARD_AREA_WIDTH + 10, WINDOW_HEIGHT - 24);
    // 2. 透明文字背景表示只画字形，文字周围仍保留已绘制的米色底。
    // 标题 y=25，副标题 y=66；两者共用左边界，但通过字号形成层级。
    setbkmode(TRANSPARENT);
    setcolor(UI_TEXT);
    setTextFont(32);
    outtextxy(PANEL_X, 25, L"五子棋");
    setcolor(EGERGB(95, 85, 68));
    setTextFont(20);
    outtextxy(PANEL_X, 66, L"15 × 15  /  黑棋先行");

    // 3. 状态区集中展示当前执棋和原有胜局计数，不推断或新增业务状态。
    // statusY 是状态区域的顶部，只在本函数使用，所以不用放到头文件。
    const int statusY = 102;
    setfillcolor(EGERGB(240, 231, 210));
    bar(PANEL_X, statusY,
        PANEL_X + PANEL_WIDTH, statusY + 94);
    // 圆心距状态区左边 27、顶部 28，半径为 11；这里只是执棋图标，不是棋盘落子。
    // “条件 ? 值一 : 值二”表示条件成立使用值一，否则使用值二。
    setfillcolor(player == 1 ? BLACK : WHITE);
    solidcircle(PANEL_X + 27, statusY + 28, 11);
    setcolor(UI_TEXT);
    setTextFont(26);
    outtextxy(PANEL_X + 50, statusY + 14,
              player == 1 ? L"当前执棋 · 黑棋" : L"当前执棋 · 白棋");
    // 当前执棋文字从区域左侧偏移 50，给圆形图标留出空间。
    // 下方统计文字距顶部约 55，避开第一行；三个位置分别对应标签、黑胜、白胜。
    setTextFont(20);
    setcolor(EGERGB(95, 85, 68));
    outtextxy(PANEL_X + 16, statusY + 57, L"累计胜局");
    setcolor(UI_TEXT);
    setTextFont(22);
    // to_wstring 将数字转换成宽字符串；c_str() 将字符串交给 EGE 的文字函数。
    wstring blackText = L"黑胜  " + to_wstring(blackWin);
    wstring whiteText = L"白胜  " + to_wstring(whiteWin);
    outtextxy(PANEL_X + 120, statusY + 55, blackText.c_str());
    outtextxy(PANEL_X + 240, statusY + 55, whiteText.c_str());

    // 4. 底部固定提示区承接保存、胜负和复盘消息，沿用原来的 180 帧显示时间。
    // 横线位于 y=530；标签在其下方，消息向右偏移 52，避免与“提示”重叠。
    drawSeparator(530);
    setTextFont(20);
    setcolor(EGERGB(95, 85, 68));
    outtextxy(PANEL_X, 544, L"提示");
    if(messageTime > 0)
    {
        setcolor(EGERGB(88, 97, 71));
        setTextFont(24);
        outtextxy(PANEL_X + 52, 541, message.c_str());
        // 每绘制一帧减少一次；60 帧/秒时，180 帧约为 3 秒。
        messageTime--;
    }
}

// 分组原有七个按钮；双人按钮只绘制，切换逻辑留给作者接入，不显示开发说明。
void UI::drawMenu()
{
    setbkmode(TRANSPARENT);
    // 模式区：标题在 y=226，按钮在 y=255，一行三列。
    // 双人与联机仅有外观；这里调用 draw 不会触发或新增游戏模式切换。
    drawSeparator(213);
    drawSection(L"游戏模式", 226);
    localButton.draw();
    aiButton.draw();
    networkButton.draw();
    // 游戏操作区：标题在 y=349，按钮在 y=378，一行两列。
    // 分隔线与模式按钮之间留白，使两组操作不会挤在一起。
    drawSeparator(336);
    drawSection(L"游戏操作", 349);
    restartButton.draw();
    undoButton.draw();
    // 棋局管理区：标题在 y=438，按钮在 y=467，一行三列。
    drawSection(L"棋局管理", 438);
    saveButton.draw();
    loadButton.draw();
    replayButton.draw();
}
// 以下 check... 函数只把鼠标坐标传给对应 Button，返回是否点击了该按钮。
// Game 收到 true 才执行实际操作，因此 UI 不负责清空棋盘、保存文件或控制 AI。
// 判断重新开始按钮是否被点击。
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
// Game 调用本函数传入消息；这里只记录文字和显示帧数，不直接绘制。
// 下一帧 drawInfo 会将该消息显示在底部提示区。
void UI::showMessage(wstring msg)
{
    message=msg;
    messageTime=180;
}
//判断复盘按钮是否被点击
bool UI::checkReplayClick(int x,int y)
{

    if(replayButton.isClicked(x,y))
    {
        return true;
    }

    return false;
}
//判断AI按钮是否被点击
bool UI::checkAIClick(int x,int y)
{
    if(aiButton.isClicked(x,y))
    {
        return true;
    }

    return false;
}
//判断双人按钮是否被点击
bool UI::checkLocalClick(int x,int y)
{
    return localButton.isClicked(x,y);
}

// 和双人按钮一样，只检查点击位置，不在 UI 中处理连接。
bool UI::checkNetworkClick(int x,int y)
{
    return networkButton.isClicked(x,y);
}

#include <graphics.h>
#include "game.h"


int main()
{

    // 仅扩展右侧控制区，窗口高度与左侧棋盘尺寸保持不变。
    initgraph(WINDOW_WIDTH,WINDOW_HEIGHT,INIT_RENDERMANUAL);



    // 启动时铺设暖米色背景；每帧棋盘和控制区分别绘制各自底色。
    setbkcolor(UI_BACKGROUND);
    setfillcolor(UI_BACKGROUND);
    bar(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);


    //创建游戏对象
    Game game;


    //开始游戏
    game.run();



    closegraph();

    return 0;
}

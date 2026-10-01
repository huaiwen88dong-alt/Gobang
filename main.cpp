#include <graphics.h>
#include "game.h"


int main()
{

    initgraph(800,600,INIT_RENDERMANUAL);



    //设置背景
    setfillcolor(EGERGB(210,160,90));
    bar(0,0,600,600);


    //创建游戏对象
    Game game;


    //开始游戏
    game.run();



    closegraph();

    return 0;
}
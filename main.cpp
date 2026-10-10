#include <graphics.h>
#include "game.h"


int main()
{
    initgraph(WINDOW_WIDTH,WINDOW_HEIGHT,INIT_RENDERMANUAL);
    setbkcolor(UI_BACKGROUND);
    setfillcolor(UI_BACKGROUND);
    bar(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
    Game game;
    game.run();



    closegraph();

    return 0;
}

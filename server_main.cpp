#include "network.h"
#include <iostream>

using namespace std;


int main()
{
    Network net;

    cout<<"服务器启动"<<endl;

    net.createServer();


    Move move;

    move.row=7;
    move.col=8;
    move.player=1;


    net.sendMove(move);


    return 0;
}
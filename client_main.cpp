#include "network.h"
#include <iostream>

using namespace std;


int main()
{
    Network net;


    net.connectServer("127.0.0.1");


    Move move;


    net.receiveMove(move);


    cout<<move.row<<" "
        <<move.col<<" "
        <<move.player
        <<endl;


    return 0;
}
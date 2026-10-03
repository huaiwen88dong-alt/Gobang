#ifndef NETWORK_H
#define NETWORK_H


#include <winsock2.h>
#include <string>
#include "move.h"


class Network
{

private:

    //通信socket
    SOCKET socket;


public:

    Network();


    //创建服务器
    bool createServer();


    //连接服务器
    bool connectServer(std::string ip);


    //发送棋子
    bool sendMove(Move move);


    //接收棋子
    bool receiveMove(Move& move);


};


#endif
#include "network.h"
#include <iostream>

//构造函数，初始化socket
Network::Network()
{
    //socket初始化为无效值,还没有连接
    socket=INVALID_SOCKET;


    //初始化网络
    WSADATA wsaData;


    WSAStartup(MAKEWORD(2,2),&wsaData);

}
//创建服务器
bool Network::createServer()
{

    //创建socket

    socket=::socket(AF_INET,SOCK_STREAM,0);


    if(socket==INVALID_SOCKET)
    {
        return false;
    }



    //服务器地址

    sockaddr_in serverAddr;


    serverAddr.sin_family=AF_INET;


    //监听所有IP

    serverAddr.sin_addr.s_addr=INADDR_ANY;


    //端口8888

    serverAddr.sin_port=htons(8888);



    //绑定

    if(bind(socket,(sockaddr*)&serverAddr,sizeof(serverAddr))==SOCKET_ERROR)
    {
        return false;
    }



    //监听

    if(listen(socket,1)==SOCKET_ERROR)
    {
        return false;
    }



    std::cout<<"等待玩家连接..."<<std::endl;



    //等待客户端

    SOCKET clientSocket;


    clientSocket=accept(socket,nullptr,nullptr);


    if(clientSocket==INVALID_SOCKET)
    {
        return false;
    }


    //保存真正通信的socket

    socket=clientSocket;



    std::cout<<"连接成功!"<<std::endl;


    return true;

}
//连接服务器
bool Network::connectServer(std::string ip)
{

    //创建socket

    socket =::socket(AF_INET,SOCK_STREAM,0);


    if(socket==INVALID_SOCKET)
    {
        return false;
    }



    //服务器地址

    sockaddr_in serverAddr;


    serverAddr.sin_family=AF_INET;


    //服务器IP

    serverAddr.sin_addr.s_addr=inet_addr(ip.c_str());


    //服务器端口

    serverAddr.sin_port=htons(8888);



    //连接服务器

    if(connect(socket,(sockaddr*)&serverAddr,sizeof(serverAddr))==SOCKET_ERROR)
    {
        return false;
    }


    std::cout<<"连接服务器成功!"<<std::endl;


    return true;

}
bool Network::sendMove(Move move)
{

    int result = send(socket,(char*)&move,sizeof(Move),0);


    if(result == SOCKET_ERROR)
    {
        return false;
    }


    return true;

}
bool Network::receiveMove(Move& move)
{

    int result = recv(socket,(char*)&move,sizeof(Move),0);


    if(result<=0)
    {
        return false;
    }


    return true;

}

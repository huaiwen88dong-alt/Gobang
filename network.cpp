#include "network.h"
#include <iostream>

// 初始化 Winsock；若失败，后续创建或连接函数直接返回 false。
Network::Network()
{
    socket = INVALID_SOCKET;
    receivedBytes = 0;
    WSADATA data = {};
    winsockReady = WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

// socket 与 Winsock 的申请和释放成对出现，退出程序时不留下连接资源。
Network::~Network()
{
    close();
    if(winsockReady)
        WSACleanup();
}

void Network::close()
{
    if(socket != INVALID_SOCKET)
    {
        closesocket(socket);
        socket = INVALID_SOCKET;
    }
    receivedBytes = 0;
}

// 服务器流程：创建监听 socket → 绑定 IP/端口 → 监听 → 接受一个连接。
// listenSocket 用来等人，socket 用来下棋；两者不能混用或覆盖后忘记释放。
bool Network::createServer(unsigned short port)
{
    close();
    if(!winsockReady)
        return false;

    SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listenSocket == INVALID_SOCKET)
        return false;

    sockaddr_in address = {};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(NETWORK_IP);
    address.sin_port = htons(port); // htons 将端口转换为 TCP/IP 使用的字节顺序。

    if(bind(listenSocket, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR ||
       listen(listenSocket, 1) == SOCKET_ERROR)
    {
        closesocket(listenSocket);
        return false;
    }

    std::cout << "等待客户端连接 " << NETWORK_IP << ":" << port << " ..." << std::endl;
    socket = accept(listenSocket, nullptr, nullptr);
    closesocket(listenSocket); // 只接待一位对手，连接后不需要继续监听。
    if(socket == INVALID_SOCKET)
        return false;

    // 本地发送通常立即完成；最多等待 1 秒，连接异常时不无限卡住窗口。
    DWORD timeout = 1000;
    if(setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout)) == SOCKET_ERROR)
    {
        close();
        return false;
    }
    return true;
}

// 客户端只负责连接，不需要 bind、listen 或 accept。
bool Network::connectServer(std::string ip, unsigned short port)
{
    close();
    if(!winsockReady)
        return false;

    const unsigned long ipAddress = inet_addr(ip.c_str());
    if(ipAddress == INADDR_NONE)
        return false;

    socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(socket == INVALID_SOCKET)
        return false;

    sockaddr_in address = {};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = ipAddress;
    address.sin_port = htons(port);
    if(connect(socket, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR)
    {
        close();
        return false;
    }

    DWORD timeout = 1000;
    if(setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout)) == SOCKET_ERROR)
    {
        close();
        return false;
    }
    return true;
}

// 不直接发送整个 Move 结构体，避免结构体填充和 int 字节顺序的问题。
// 15×15 棋盘的行列为 0～14，棋色为 1 或 2，三个值各用一个字节就够。
bool Network::sendMove(Move move)
{
    if(socket == INVALID_SOCKET || move.row < 0 || move.row >= 15 ||
       move.col < 0 || move.col >= 15 || (move.player != 1 && move.player != 2))
        return false;

    const unsigned char data[3] = {
        (unsigned char)move.row, (unsigned char)move.col, (unsigned char)move.player
    };
    int sentBytes = 0;
    while(sentBytes < 3)
    {
        const int count = send(socket, (const char*)data + sentBytes, 3 - sentBytes, 0);
        if(count <= 0)
        {
            close();
            return false;
        }
        sentBytes += count;
    }
    return true;
}

// select 的超时设为 0：只检查“现在是否能读”，不等待数据到来。
// recv 只读取当前这一步缺少的字节；未收满时保留进度，下一帧继续。
int Network::receiveMove(Move& move)
{
    if(socket == INVALID_SOCKET)
        return -1;

    fd_set readable;
    FD_ZERO(&readable);
    FD_SET(socket, &readable);
    timeval timeout = {};
    const int ready = select(0, &readable, nullptr, nullptr, &timeout);
    if(ready == SOCKET_ERROR)
    {
        close();
        return -1;
    }
    if(ready == 0)
        return 0;

    const int count = recv(socket, (char*)receiveBuffer + receivedBytes, 3 - receivedBytes, 0);
    if(count <= 0) // 返回 0 表示对方正常关闭，负数表示通信出错。
    {
        close();
        return -1;
    }
    receivedBytes += count;
    if(receivedBytes < 3)
        return 0;

    move.row = receiveBuffer[0];
    move.col = receiveBuffer[1];
    move.player = receiveBuffer[2];
    receivedBytes = 0;
    return 1;
}
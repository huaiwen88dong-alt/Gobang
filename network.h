#ifndef NETWORK_H
#define NETWORK_H

// Winsock 必须在 graphics.h/Windows 头文件之前包含，避免旧版 winsock.h 冲突。
#include <winsock2.h>
#include <string>
#include "move.h"

// 本地演示只需修改这里的端口；127.0.0.1 表示当前这台电脑。
const unsigned short NETWORK_PORT = 8888;
//本机回环地址
const char NETWORK_IP[] = "127.0.0.1";

class Network
{
private:
    // socket 保存连接成功后的通信套接字；监听套接字只在 createServer 内使用。
    SOCKET socket;
    bool winsockReady;

    // 消息固定为 3 字节：普通落子是行、列、棋色；重开用特殊标记 {255,255,0}。
    // TCP 可能分几次收到，所以保留尚未收完整的字节；不能把半步棋交给 Game。
    unsigned char receiveBuffer[3];
    // 记录收到几字节
    int receivedBytes;

public:
    Network();
    // 自动释放连接和 Winsock；入口中的 Network 比 Game 活得更久。
    ~Network();
    // 一个对象只拥有一个连接，禁止复制，避免同一个 socket 被关闭两次。
    Network(const Network&) = delete;
    Network& operator=(const Network&) = delete;

    // 创建本地服务器，等待一个客户端。等待发生在打开游戏窗口之前。
    bool createServer(unsigned short port = NETWORK_PORT);
    // ip地址没有默认值，必须传，客户端连接指定 IPv4 地址和端口；默认用于同一台电脑上的两个程序。
    bool connectServer(std::string ip, unsigned short port = NETWORK_PORT);
    // 循环发送，直到一步棋的 3 个字节全部交给 TCP，或连接出错。
    bool sendMove(Move move);
    // 发送重开标记，复用原来的三个字节收发，不增加确认回复。
    bool sendRestart();
    
    // 每帧调用一次：1=收到完整一步，0=暂无完整数据，-1=断开或出错。
    // 传入一个变量，让函数把收到的数据填进去
    int receiveMove(Move& move);
    // 可重复调用；关闭后不再收发，不自动重连。
    void close();
};

#endif

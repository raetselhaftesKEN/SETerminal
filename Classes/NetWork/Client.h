/**
* @file Client.h
*/

#ifndef __CLIENT_H__
#define __CLIENT_H__

#include<iostream>
#include<winsock.h>
#pragma comment(lib,"ws2_32.lib")

class Client
{
private:
	//定义长度变量
	int send_len = 0;
	int recv_len = 0;
	//定义发送缓冲区和接受缓冲区
	char send_buf[128];
	char recv_buf[128];
	//定义服务端套接字，接受请求套接字
	SOCKET s_server;
	//服务端地址客户端地址
	SOCKADDR_IN server_addr;

public:
	void initialization();
	void SendAndRecv();
	void closeNet();

};



#endif // !__CLIENT_H__
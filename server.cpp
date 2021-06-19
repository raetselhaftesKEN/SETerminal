//#include "pch.h"
#include<iostream>
#include<winsock.h>
#include<vector>
#include<map>
#include<string>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
void initialization();
int main() {
	vector<SOCKET>clients;
	string recvMessage[16];
	bool isRecvNewMessage = false;
	map<SOCKET, SOCKADDR_IN>clientsAddr;
	//定义长度变量
	int send_len = 0;
	int recv_len = 0;
	int len = 0;
	const char* text = "nihao";
	//定义发送缓冲区和接受缓冲区
	char send_buf[128];
	char recv_buf[128];
	//定义服务端套接字，接受请求套接字
	SOCKET s_server;
	SOCKET s_accept;
	//服务端地址客户端地址
	SOCKADDR_IN server_addr;
	SOCKADDR_IN accept_addr;
	initialization();
	//填充服务端信息
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(1234);
	//创建套接字
	s_server = socket(AF_INET, SOCK_STREAM, 0);
	unsigned long u1 = 1;
	ioctlsocket(s_server, FIONBIO, (unsigned long*)&u1);//设置为非阻塞

	if (bind(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		cout << "套接字绑定失败！" << endl;
		WSACleanup();
	}
	else {
		cout << "套接字绑定成功！" << endl;
	}
	//设置套接字为监听状态
	if (listen(s_server, SOMAXCONN) < 0) {
		cout << "设置监听状态失败！" << endl;
		WSACleanup();
	}
	else {
		cout << "设置监听状态成功！" << endl;
	}
	cout << "服务端正在监听连接，请稍候...." << endl;
	//接受连接请求
	/*len = sizeof(SOCKADDR);
	s_accept = accept(s_server, (SOCKADDR*)&accept_addr, &len);
	if (s_accept == SOCKET_ERROR) {
		cout << "连接失败！" << endl;
		WSACleanup();
		return 0;
	}
	cout << "连接建立，准备接受数据" << endl;*/
	//接收数据
	while (1) {
		isRecvNewMessage = false;
		//接受连接请求
		len = sizeof(SOCKADDR);
		s_accept = accept(s_server, (SOCKADDR*)&accept_addr, &len);
		if (s_accept != SOCKET_ERROR)
		{
			//连接成功的时候继续下边的操作
			clients.push_back(s_accept);
			
		}
		//recv_len = recv(s_accept, recv_buf, 128, 0);
		//cout << "11111" << recv_buf << endl;

		
		//clientsAddr[s_accept] = accept_addr;
		//判断第i个玩家是不是发了消息
		for (int i = 0; i < clients.size(); i++)
		{
			recv_len = recv(clients[i], recv_buf, 128, 0);
			if (recv_len < 0) {
				//cout << "222" << endl;
				continue;
			}
			else {
				isRecvNewMessage = true;
				cout << "客户端信息:" << recv_buf << endl;
				recvMessage[i] = recv_buf;
				for (int j = 0; j < clients.size(); j++)
				{
					char tt[128] = "huifu";
					send(clients[j], tt, 128, 0);
				}

			}	
			
		}


		/*recv_len = recv(s_accept, recv_buf, 128, 0);
		if (recv_len < 0) {
			continue;
			cout << "接受失败！" << endl;
			break;
		}
		else {
			cout << "客户端信息:" << recv_buf << endl;
		}*/

		/*cout << "请输入回复信息:";
		cin >> send_buf;
		send_len = send(s_accept, send_buf, 100, 0);
		if (send_len < 0) {
			cout << "发送失败！" << endl;
			break;
		}*/
	}
	//关闭套接字
	closesocket(s_server);
	closesocket(s_accept);
	//释放DLL资源
	WSACleanup();
	return 0;
}
void initialization() {
	//初始化套接字库
	WORD w_req = MAKEWORD(2, 2);//版本号
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0) {
		cout << "初始化套接字库失败！" << endl;
	}
	else {
		cout << "初始化套接字库成功！" << endl;
	}
	//检测版本号
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		cout << "套接字库版本号不符！" << endl;
		WSACleanup();
	}
	else {
		cout << "套接字库版本正确！" << endl;
	}
	//填充服务端地址信息

}
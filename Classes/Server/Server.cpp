#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstring>
#include<winsock.h>
#include<vector>
#include<map>
#include<string>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

void initialization();

int main() {
	//vector<SOCKET>clients;
	//string recvMessage[16];
	//vector<string> receiveMessage;

	map<SOCKET, string> clientMsg;

	bool isNewMessageReceived = false;

	//定义长度变量
	int sendLength = 0;
	int recvLength = 0;
	int length = 0;

	//定义发送缓冲区和接受缓冲区
	char sendBuffer[128];
	char receiveBuffer[128];

	//定义服务端套接字，接受请求套接字
	SOCKET sServer;
	SOCKET sAccept;
	//服务端地址客户端地址
	SOCKADDR_IN serverAddress;
	SOCKADDR_IN acceptAddress;
	initialization();

	//填充服务端信息
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(1234);

	//创建套接字
	sServer = socket(AF_INET, SOCK_STREAM, 0);
	unsigned long u1 = 1;
	ioctlsocket(sServer, FIONBIO, (unsigned long*)&u1);//设置为非阻塞

	if (bind(sServer, (SOCKADDR*)&serverAddress, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		cout << "套接字绑定失败！" << endl;
		WSACleanup();
	}
	else {
		cout << "套接字绑定成功！" << endl;
	}
	//设置套接字为监听状态
	if (listen(sServer, SOMAXCONN) < 0) {
		cout << "设置监听状态失败！" << endl;
		WSACleanup();
	}
	else {
		cout << "设置监听状态成功！" << endl;
	}
	cout << "服务端正在监听连接，请稍候...." << endl;

	//接收数据
	while (1) 
	{
		isNewMessageReceived = false;
		//接受连接请求
		length = sizeof(SOCKADDR);
		sAccept = accept(sServer, (SOCKADDR*)&acceptAddress, &length);
		if (sAccept != SOCKET_ERROR)
		{
			clientMsg.insert(std::pair<SOCKET, string>(sAccept, string("")));
		}

		//判断第i个玩家是不是发了消息
		for (map<SOCKET, string>::iterator iter = clientMsg.begin(); iter != clientMsg.end(); )
		{

			recvLength = recv(iter->first, receiveBuffer, 128, 0);
			if (recvLength < 0)
			{
				isNewMessageReceived = false;
				iter++;
				//continue;
			}
			else
			{
				isNewMessageReceived = true;
				cout << "Client Message:" << receiveBuffer << endl;
				iter->second = string(receiveBuffer);
				if (iter->second == "Join")
				{
					strcpy(sendBuffer, "A player joined");
					//告知除刚加入的客户端外的所有客户端有新客户加入
					for (map<SOCKET, string>::iterator iter2 = clientMsg.begin(); iter2 != clientMsg.end(); iter2++)
					{
						if (iter2 == iter)
							continue;
						cout << "Send Message to all Clients:" << sendBuffer << endl;
						send(iter2->first, sendBuffer, 128, 0);
					}
					iter++;
				}
				else if (iter->second == "Quit")
				{
					//清除离开的客户端，通知所有其他客户端有人离开
					cout << "player quit" << endl;
					clientMsg.erase(iter++);
					for (map<SOCKET, string>::iterator iter2 = clientMsg.begin(); iter2 != clientMsg.end(); iter2++)
					{
						strcpy(sendBuffer, "A player quitted");
						cout << "Send Message to all Clients:" << sendBuffer << endl;
						send(iter2->first, sendBuffer, 128, 0);
					}
				}
			}
		}
	}

	//关闭套接字
	closesocket(sServer);
	closesocket(sAccept);
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
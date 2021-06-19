#include "Client.h"
using namespace std;

Client* Client::client_ = new Client();

void Client::initialization() {
	//初始化套接字库
	WORD w_req = MAKEWORD(2, 2);//版本号
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0) {
		//cout << "初始化套接字库失败！" << endl;
	}
	else {
		//cout << "初始化套接字库成功！" << endl;
	}
	//检测版本号
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		//cout << "套接字库版本号不符！" << endl;
		WSACleanup();
	}
	else {
		//cout << "套接字库版本正确！" << endl;
	}
	//填充服务端地址信息

	//填充服务端信息
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = inet_addr("192.168.43.69");
	server_addr.sin_port = htons(1234);
	//创建套接字
	s_server = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		//cout << "服务器连接失败！" << endl;
		WSACleanup();
	}
	else {
		//cout << "服务器连接成功！" << endl;
	}
	unsigned long int u1 = 1;
	ioctlsocket(s_server, FIONBIO, (unsigned long*)&u1);
}

void Client::Send(const char* msg)
{
	//发送,接收数据

	cout << "Please send message:";
	//cin >> send_buf;
	send_buf[0] = 'h';
	send_buf[1] = '\0';
	send_len = send(s_server, msg, 100, 0);
	if (send_len < 0) {
		cout << "Sending failed!" << endl;
		//break;
	}

	
	return;
}

bool Client::Receive()
{
	recv_len = recv(s_server, recv_buf, 100, 0);
	if (recv_len < 0)
	{
		return false;
	}
	else
	{
		recv_len = -2;
		return true;
	}
}

void Client::closeNet()
{
	//关闭套接字
	closesocket(s_server);
	//释放DLL资源
	WSACleanup();
	return;
}
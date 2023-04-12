#include<stdio.h>
#include"../SocketInit/SocketInit.hpp"
#include<ws2tcpip.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS  

int main() {
	SocketInit socketInit;

	// 创建客户端套接字
	SOCKET sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sClient == SOCKET_ERROR) {
		printf("客户端套接字创建失败！\n");
		return -1;
	}

	// 套接字定位
	sockaddr_in sock_in;
	sock_in.sin_family = AF_INET;                   // 使用的协议类型
	sock_in.sin_port = htons(12307);                // 使用的端口号
	// sock_in.sin_addr.S_un.S_addr = inet_addr("172.24.129.191");            // 客户端的ip地址

	inet_pton(AF_INET, "172.24.129.191", &sock_in.sin_addr.s_addr);//vs2013版本以上使用新的函数转换IP地址
	//InetPton(AF_INET, "127.0.0.1", &sockAddr.sin_addr.s_addr);
	//sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// 发起连接请求
	if (connect(sClient, (const sockaddr*)&sock_in, sizeof(sockaddr_in)) == SOCKET_ERROR) {
		printf("连接服务器失败！\n");
		return -1;
	}

	// 发送数据
	while (true) {
		// 定义一个数据块缓冲区
		char buff[1024];
		gets_s(buff, 1024);
		send(sClient, buff, strlen(buff), 0);
	}

	closesocket(sClient);
	getchar();
	return 0;
}
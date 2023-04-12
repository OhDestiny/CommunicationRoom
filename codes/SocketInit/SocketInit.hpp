#ifndef _SOCKET_INIT_H_
#define _SOCKET_INIT_H_

#include<winsock2.h>
#include<stdio.h>

#pragma comment(lib, "ws2_32.lib")

class SocketInit {
public:
	SocketInit() {
		WORD sockVersion = MAKEWORD(2, 2);
		WSADATA wasData;
		if (WSAStartup(sockVersion , &wasData) != 0) {
			printf("动态链接库加载失败！");
		}
	}

	~SocketInit() {
		WSACleanup();
	}
};

#endif // !_SOCKET_INIT_H_

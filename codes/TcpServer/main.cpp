#include<stdio.h>
#include"../SocketInit/SocketInit.hpp"


int main() {
	SocketInit socketInit;        // 加载socket的动态库

	// 创建监听的套接字
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);               // 使用ipv4 流式socket TCP协议
	// 参数解释
	// domain: 协议族，决定socket的类型，比如说，AF_INET 则决定了使用的是IPV4的32位地址，和16位的端口号
	// type: socket类型，比如有SOCK_STREAM流式socket就是一种面向连接的Socket，而Sock_DGRAM则是数据报式的socket,对应无连接的udp服务应用
	// protocol: 指的是协议，比如IPPROTO_TCP 就是指的式TCP协议。

	// 判断监听套接字是否创建成功
	if (sListen == SOCKET_ERROR) {
		printf("监听套接字创建失败！\n");
		return -1;
	}

	// 绑定套接字，就是套接字这个对象需要用ip 加 端口来绑定标识
	// 参数：
	// 1.套接字
	// 2.套接字对应的地址，包括ip和端口
	// 3.名字的常数

	// 先获取套接字的标识
	sockaddr_in sock_in;
	sock_in.sin_family = AF_INET;
	sock_in.sin_port = htons(12307);
	sock_in.sin_addr.S_un.S_addr = INADDR_ANY;              // 获取本机ip

	// 进行绑定
	int ret = bind(sListen, (const sockaddr*)&sock_in, sizeof(sockaddr_in));
	
	// 判断是否绑定成功
	if (ret == SOCKET_ERROR) {
		printf("绑定套接字失败！\n");
		closesocket(sListen);
		return -1;
	}
	if (listen(sListen, 10) == SOCKET_ERROR) {
		printf("监听失败！\n");
	}

	// 接收客户端连接
	// 创建一个客户端的socket
	sockaddr_in clientAddr;
	int nlen = sizeof(sockaddr_in);

	// 接收
	SOCKET sClient = accept(sListen, (sockaddr*)&clientAddr, &nlen);

	if (sClient == SOCKET_ERROR) {
		printf("接收客户端连接失败！");
		closesocket(sListen);
		// return -1;
	}
	else {
		printf("与客户端建立连接！");
	}


	// 接收客户端数据
	while (true) {
		// 定义缓冲区
		char buff[1024] = { 0 };

		// recv 函数接收数据
		int result = recv(sClient, buff, 1024, 0);

		// 根据返回值 判断是否接收到数据
		if (result > 0) {
			printf("接收到的数据是：%s", buff);
		}
		else {
			printf("客户端已经断开连接！");
			break;
		}
	}

	// 关闭套接字
	closesocket(sListen);

	getchar();           // 阻塞
	
	return 0;
}
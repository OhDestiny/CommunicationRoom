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

	// 一个服务器和多个客户端： 
	// 实现思路一： while大循环接收连接，然后，创建各自的线程处理每个用户自己的接收消息
	// 实现思路二： 使用select模型
	
	FD_SET fd_read;                     // 存储scoket对象
	FD_ZERO(&fd_read);                  // 初始化fd_read
	FD_SET(sListen, &fd_read);          
	FD_SET fd_tmp;                      // 每次使用的临时socket数组

	while (true) {
		fd_tmp = fd_read;
		const timeval tv = {1,0};
		int ret = select(NULL, &fd_tmp, NULL, NULL, &tv);                  // 筛选出有网络事件的socket 留在fd_read中
		if (ret == 0) {
			Sleep(1);
			continue;
		}

		// 循环判断此时的fd_read里面的socket类型，从而判断该执行什么判断
		for (int i = 0; i < fd_tmp.fd_count;i++) {
			// 如果是监听套接字有网络事件，那么证明有客户端在连接服务器
			if (fd_tmp.fd_array[i] == sListen) {

				// 接收客户端连接
				// 创建一个客户端的socket
				sockaddr_in clientAddr;
				int nlen = sizeof(sockaddr_in);

				// 接收
				SOCKET sClient = accept(sListen, (sockaddr*)&clientAddr, &nlen);

				if (sClient == SOCKET_ERROR) {
					printf("接收客户端连接失败！");
					closesocket(sListen);
					return -1;
				}
				else {
					printf("客户端已经建立连接：%s...",inet_ntoa(clientAddr.sin_addr));
					FD_SET(sClient,&fd_read);                     // 需要将申请连接的那个客户端的socket也加入到队列当中
				}

			}
			// 如果是客户端套接字有网络事件，那么证明有客户端在接发消息
			else {
				// 定义缓冲区
				char buff[1024] = { 0 };

				// recv 函数接收数据
				int result = recv(fd_tmp.fd_array[i], buff, 1024, 0);

				// 根据返回值 判断是否接收到数据
				if (result > 0) {
					printf("接收到的数据是：%s", buff);
				}
				else {
					// 断开连接时需要移除断开连接的这个socket对象
					FD_CLR(fd_tmp.fd_array[i], &fd_read);
					printf("客户端已经断开连接！");
					break;
				}
			}
		}


	} 

	// 关闭套接字
	closesocket(sListen);

	getchar();           // 阻塞
	
	return 0;
}
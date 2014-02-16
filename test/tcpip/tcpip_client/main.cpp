#include "ClientNet.h"

void main()
{
	CClientNet client;
	
	//连接到127.0.0.1（即本地）,端口号为8888的服务端
	printf("Connect\n");
	client.Connect(1235,"127.0.0.1");
	
	//发送消息
	printf("send msg\n");
	client.SendMsg("hello\0",sizeof("hello\0"));
	
	//关闭socket
	printf("close\n");
	client.Close();

	system("pause");
}
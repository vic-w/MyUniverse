#include "stdafx.h"
#include "udpThread.h"
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include "GlobeThread.h"
#include "MyUniverseDlg.h"
#pragma  comment (lib, "ws2_32.lib")

#define MSG_BUFFER_LENGTH (2048) //接受buffer长度
#define UDP_LISTENING_PORT (9160)

char recvbuf[MSG_BUFFER_LENGTH]; 

extern CRITICAL_SECTION g_StoryPage_CS;
extern CStoryPage g_StoryPage;

extern CRITICAL_SECTION g_GlobeRotMat_CS;
extern GlbRotmat g_GlobeRotMat;
extern GlbEularAngle g_GlobeEularAngle;

extern int g_bMainThreadActive;//主线程活动指示
extern int g_bGlbThreadActive;//星球绘图线程活动指示
extern int g_bTimingThreadActive;//星球绘图线程活动指示

DWORD WINAPI UdpThread(LPVOID lpParam)
{
    WSADATA   wsd;
	SOCKET    recv_s;
	//char      *recvbuf;
	int       ret;
	int       port;
	SOCKADDR_IN localaddr;
	SOCKADDR_IN destaddr;
	int destlen = sizeof(destaddr);

	port = UDP_LISTENING_PORT;
    
	//初始化Winsock库
	if(WSAStartup(MAKEWORD(2,2),&wsd) != 0)
	{
		printf("WSAStartup error!\n");
		exit(1);
	}
    //创建UDP套接字
	recv_s = socket(AF_INET, SOCK_DGRAM, 0);
	if(recv_s == INVALID_SOCKET)
	{
		printf("socket() errror : %d\n",WSAGetLastError());
		exit(1);
	}

    //对本机地址结构进行设置
	localaddr.sin_family = AF_INET;
	localaddr.sin_port = htons((u_short)port);
	localaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
    //对套接字进行绑定
	if(bind(recv_s, (SOCKADDR *)&localaddr, sizeof(localaddr)) == SOCKET_ERROR)
	{
		printf("bind() error: %d\n", WSAGetLastError());
		exit(1);
	}

	printf("在端口%d监听...\n",port);
    printf("协议结构体长度%d字节\n", sizeof(GlbCommand));

	int recvlen = MSG_BUFFER_LENGTH;
    //开始接受消息
	while(1)
	{
		ret = recvfrom(recv_s, recvbuf, recvlen, 0,(SOCKADDR*)&destaddr, &destlen);//此函数为阻塞函数

		if(ret == SOCKET_ERROR)
		{
			printf("recvfrom() error: %d\n",WSAGetLastError());
			exit(1);
		}
		else
		{
            if( ret != sizeof(GlbCommand) )
            {
                printf("无法解析命令\n");
            }
            else
            {
			    printf("收到主机 %s 的消息:\n",inet_ntoa(destaddr.sin_addr));
                SparseCmd((void*)recvbuf);
            }
		}

	}
	closesocket(recv_s);
	WSACleanup();
	return 0;
}

void SparseCmd(void* buf)
{
    GlbCommand* cmdBuf = (GlbCommand*)buf;

    switch(cmdBuf->command)
    {
    case GLB_CMD_PLAY:
        printf("play\n");
        EnterCriticalSection(&g_StoryPage_CS);
        g_StoryPage.bPlaying = 1;
        LeaveCriticalSection(&g_StoryPage_CS);
        break;
    case GLB_CMD_PAUSE:
        printf("pause\n");
        EnterCriticalSection(&g_StoryPage_CS);
        g_StoryPage.bPlaying = 0;
        LeaveCriticalSection(&g_StoryPage_CS);
        break;
    }

}
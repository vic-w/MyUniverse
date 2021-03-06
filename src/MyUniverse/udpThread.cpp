﻿#include "stdafx.h"
#include "udpThread.h"
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include "GlobeThread.h"
#include "MyUniverseDlg.h"
#include "StorytellerComp.h"

#pragma  comment (lib, "ws2_32.lib")

#define MSG_BUFFER_LENGTH (2048) //接受buffer长度
#define UDP_LISTENING_PORT (9160)

char recvbuf[MSG_BUFFER_LENGTH]; 

extern CRITICAL_SECTION g_StoryPage_CS;
extern CStoryPage g_StoryPage;

extern CRITICAL_SECTION g_GlobeRotMat_CS;
extern GlbRotmat g_GlobeRotMat;
extern GlbEularAngle g_StorytellerEularAngle;
extern GlbEularAngle g_GlobeEularAngle;
extern GlbEularAngle g_OffsetEularAngle;

extern int g_bMainThreadActive;//主线程活动指示
extern int g_bGlbThreadActive;//星球绘图线程活动指示
extern int g_bTimingThreadActive;//星球绘图线程活动指示

void SendReturn(int command=0, int iParam1=0, int iParam2=0, double fParam1=0, double fParam2=0, char* cParam1="", char* cParam2="");

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
                DespatchCmd((void*)recvbuf, lpParam);
            }
		}

	}
	closesocket(recv_s);
	WSACleanup();
	return 0;
}

void DespatchCmd(void* buf, LPVOID lpParam)
{
    GlbCommand* cmdBuf = (GlbCommand*)buf;
    CMyUniverseDlg* pDlg = (CMyUniverseDlg*)lpParam;

    switch(cmdBuf->command)
    {
    case GLB_CMD_PLAY:
        printf("play\n");
        EnterCriticalSection(&g_StoryPage_CS);
        g_StoryPage.bPlaying = 1;
        if(g_StoryPage.frameRate == 0)
        {
            g_StoryPage.frameRate = 10;
        }
        LeaveCriticalSection(&g_StoryPage_CS);
        break;
    case GLB_CMD_PAUSE:
        printf("pause\n");
        EnterCriticalSection(&g_StoryPage_CS);
        g_StoryPage.bPlaying = 0;
        LeaveCriticalSection(&g_StoryPage_CS);
        break;
    case GLB_CMD_GET_ROTATECLOCKWISE:
        printf("get rotate clockwise\n");
        EnterCriticalSection(&g_StoryPage_CS);
        SendReturn(GLB_CMD_GET_ROTATECLOCKWISE, (int)g_StoryPage.bClockwise);
        LeaveCriticalSection(&g_StoryPage_CS);
        break;
    case GLB_CMD_SET_ROTATECLOCKWISE:
        printf("set rotate clockwise\n");
        EnterCriticalSection(&g_StoryPage_CS);
        g_StoryPage.bClockwise = !!cmdBuf->iParam1;
        LeaveCriticalSection(&g_StoryPage_CS);
        if(g_bMainThreadActive)
        {
            pDlg->SendMessage(WM_GLB_UPDATEDATA, false);
        }
        break;
    case GLB_CMD_SET_ROTATING:
        printf("set rotating\n");
        EnterCriticalSection(&g_StoryPage_CS);
        g_StoryPage.bRotating = !!cmdBuf->iParam1;
        LeaveCriticalSection(&g_StoryPage_CS);
        if(g_bMainThreadActive)
        {
            pDlg->SendMessage(WM_GLB_UPDATEDATA, true);
        }
        break;
    case GLB_CMD_GET_ROTATERATE:
        printf("get rotaterate\n");
        Sleep(1);
        EnterCriticalSection(&g_StoryPage_CS);
        SendReturn(GLB_CMD_GET_ROTATECLOCKWISE, 0,0, g_StoryPage.rotationRate);
        LeaveCriticalSection(&g_StoryPage_CS);
        break;
    case GLB_CMD_SET_ROTATERATE:
        printf("set rotaterate\n");
        EnterCriticalSection(&g_StoryPage_CS);
        if(cmdBuf->fParam1<1)
        {
            cmdBuf->fParam1 = 1;
        }
        if(cmdBuf->fParam1 >50)
        {
            cmdBuf->fParam1 = 50;
        }
        g_StoryPage.rotationRate = (int)cmdBuf->fParam1;
        LeaveCriticalSection(&g_StoryPage_CS);
        if(g_bMainThreadActive)
        {
            pDlg->SendMessage(WM_GLB_UPDATEDATA, true);
        }
        break;
    case GLB_CMD_GET_POLELATITUDE:
        printf("get polelatitude\n");
        EnterCriticalSection(&g_GlobeRotMat_CS);
        SendReturn(GLB_CMD_GET_POLELATITUDE, 0,0, g_StorytellerEularAngle.m_1_Horz);
        LeaveCriticalSection(&g_GlobeRotMat_CS);
        break;
    case GLB_CMD_SET_POLELATITUDE:
        printf("set polelatitude\n");
        EnterCriticalSection(&g_GlobeRotMat_CS);
        g_StorytellerEularAngle.m_1_Horz = (float)cmdBuf->fParam1;
        StorytellerEular2GlobeEular(g_StorytellerEularAngle, g_OffsetEularAngle, g_GlobeEularAngle);
        glbEularAngle2Rotmat(g_GlobeEularAngle, g_GlobeRotMat);
        LeaveCriticalSection(&g_GlobeRotMat_CS);    
        if(g_bMainThreadActive)
        {
            pDlg->SendMessage(WM_GLB_UPDATEDATA, FALSE);
        }
        break;
    case GLB_CMD_GET_POLELONGITUDE:
        printf("get polelongitude\n");
        EnterCriticalSection(&g_GlobeRotMat_CS);
        SendReturn(GLB_CMD_GET_POLELATITUDE, 0, 0, g_StorytellerEularAngle.m_2_Vert);
        LeaveCriticalSection(&g_GlobeRotMat_CS);
        break;
    case GLB_CMD_SET_POLELONGITUDE:
        printf("set polelongitude\n");
        EnterCriticalSection(&g_GlobeRotMat_CS);
        g_StorytellerEularAngle.m_2_Vert = (float)cmdBuf->fParam1;
        StorytellerEular2GlobeEular(g_StorytellerEularAngle, g_OffsetEularAngle, g_GlobeEularAngle);
        glbEularAngle2Rotmat(g_GlobeEularAngle, g_GlobeRotMat);
        LeaveCriticalSection(&g_GlobeRotMat_CS);
        if(g_bMainThreadActive)
        {
            pDlg->SendMessage(WM_GLB_UPDATEDATA, FALSE);
        }
        break; 
    case GLB_CMD_GET_CHAPTERANDPAGENAME:
        printf("get chapterandpagename\n");
        EnterCriticalSection(&g_StoryPage_CS);
        Sleep(1);
        SendReturn(GLB_CMD_GET_CHAPTERANDPAGENAME, 0,0,0,0, pDlg->m_chapter_value.GetBuffer(), pDlg->m_page_value.GetBuffer());
        LeaveCriticalSection(&g_StoryPage_CS);
        break;
    case GLB_CMD_SET_CHAPTERANDPAGENAME:
        printf("set chapterandpagename\n");
        pDlg->m_chapter_value = cmdBuf->cParam1;
        pDlg->m_page_value = cmdBuf->cParam2;
        //pDlg->ReadOnePage();
        if(g_bMainThreadActive)
        {
            pDlg->SendMessage(WM_GLB_UDPREADONEPAGE, FALSE);
        }
    }

}

void SendReturn(int command, int iParam1, int iParam2, double fParam1, double fParam2, char* cParam1, char* cParam2)
{
    GlbCommand CmdBuf;
    CmdBuf.command = command;
    CmdBuf.ctrl1_ret0 = 0;
    CmdBuf.iParam1 = iParam1;
    CmdBuf.iParam2 = iParam2;
    CmdBuf.fParam1 = fParam1;
    CmdBuf.fParam2 = fParam2;
    memcpy(CmdBuf.cParam1, cParam1, 512);
    CmdBuf.cParam1[511] = 0;    
    memcpy(CmdBuf.cParam2, cParam2, 512);
    CmdBuf.cParam2[511] = 0;

    WSADATA        wsd;
    SOCKET         send_s;
    int            ret;
    SOCKADDR_IN    recv_addr;
	int port;

	port=9161;

    // 初始化Winsock库
    if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
    {
        printf("WSAStartup error!\n");
        exit(1);
    }
    // 创建UDP套接字
    send_s = socket(AF_INET, SOCK_DGRAM, 0);
    if (send_s == INVALID_SOCKET)
    {
        printf("socket() error; %d\n", WSAGetLastError());
        exit(1);
    }
	// 设置接收端地址结构
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_port = htons((u_short)port);
    recv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// 开始发送消息
	ret = sendto(send_s, (const char*)&CmdBuf, 1056, 0, (SOCKADDR *)&recv_addr, sizeof(recv_addr));
             
    closesocket(send_s);
    WSACleanup();
    return;
}
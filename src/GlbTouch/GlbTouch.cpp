#include "GlbTouch.h"
#include "TuioDump.h"

void glbListenTouchSignal(GlbWindow &window, int port)
{
	window.m_TuioPort = port;

	HANDLE hHandle = CreateThread(0, 0, TuioThread, (LPVOID)(&window),0,0);//启动TUIO监听线程

	//SuspendThread(hHandle);        //暂停（挂起）线程
	//SetThreadPriority(hHandle, THREAD_PRIORITY_HIGHEST); //设定线程的优先级别
	//ResumeThread(hHandle);           //继续运行线程
}
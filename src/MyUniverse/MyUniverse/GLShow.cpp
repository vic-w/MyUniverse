#include "stdafx.h"
#include "GLShow.h"
#include <windows.h>		// Windows的头文件
#include <math.h>
#include "image.h"
#include "draw.h"
#include "rotation.h"

extern GlbRotmat g_GlobeRotMat;

HGLRC           hRC=NULL;							// 窗口着色描述表句柄
HDC             hDC=NULL;							// OpenGL渲染描述表句柄
HWND            hWnd=NULL;							// 保存我们的窗口句柄
HINSTANCE       hInstance;							// 保存程序的实例

bool	keys[256];								// 保存键盘按键的数组
bool	active=TRUE;								// 窗口的活动标志，缺省为TRUE
bool	fullscreen=TRUE;	

int g_rot_x = 0;
int g_rot_y = 0;
int g_rot_z = 0;


GLvoid KillGLWindow(GLvoid)							// 正常销毁窗口
{

	if (fullscreen)								// 我们处于全屏模式吗?
	{

		ChangeDisplaySettings(NULL,0);					// 是的话，切换回桌面
		ShowCursor(TRUE);						// 显示鼠标指针
	}

	if (hRC)								// 我们拥有OpenGL渲染描述表吗?
	{

		if (!wglMakeCurrent(NULL,NULL))					// 我们能否释放DC和RC描述表?
		{

			MessageBox(NULL,"释放DC或RC失败。","关闭错误",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))					// 我们能否删除RC?
		{

			MessageBox(NULL,"释放RC失败。","关闭错误",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;							// 将RC设为 NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// 我们能否释放 DC?
	{
		MessageBox(NULL,"释放DC失败。","关闭错误",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;							// 将 DC 设为 NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// 能否销毁窗口?
	{
		MessageBox(NULL,"释放窗口句柄失败。","关闭错误",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;							// 将 hWnd 设为 NULL
	}

	if (!UnregisterClass("OpenG",hInstance))				// 能否注销类?
	{
		MessageBox(NULL,"不能注销窗口类。","关闭错误",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;							// 将 hInstance 设为 NULL
	}
}

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{

	GLuint		PixelFormat;						// 保存查找匹配的结果

	WNDCLASS	wc;							// 窗口类结构

	DWORD		dwExStyle;						// 扩展窗口风格
	DWORD		dwStyle;						// 窗口风格

	RECT WindowRect;							// 取得矩形的左上角和右下角的坐标值
	WindowRect.left=(long)0;						// 将Left   设为 0
	WindowRect.right=(long)width;						// 将Right  设为要求的宽度
	WindowRect.top=(long)0;							// 将Top    设为 0
	WindowRect.bottom=(long)height;						// 将Bottom 设为要求的高度

	fullscreen=fullscreenflag;						// 设置全局全屏标志

	hInstance		= GetModuleHandle(NULL);			// 取得我们窗口的实例
	wc.style		= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;		// 移动时重画，并为窗口取得DC
	wc.lpfnWndProc		= (WNDPROC) WndProc;				// WndProc处理消息
	wc.cbClsExtra		= 0;						// 无额外窗口数据
	wc.cbWndExtra		= 0;						// 无额外窗口数据
	wc.hInstance		= hInstance;					// 设置实例
	wc.hIcon		= LoadIcon(NULL, IDI_WINLOGO);			// 装入缺省图标
	wc.hCursor		= LoadCursor(NULL, IDC_ARROW);			// 装入鼠标指针
	wc.hbrBackground	= NULL;						// GL不需要背景
	wc.lpszMenuName		= NULL;						// 不需要菜单
	wc.lpszClassName	= "OpenG";					// 设定类名字

	if (!RegisterClass(&wc))						// 尝试注册窗口类
	{
		MessageBox(NULL,"注册窗口失败","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// 退出并返回FALSE
	}

	if (fullscreen)								// 要尝试全屏模式吗?
	{

		DEVMODE dmScreenSettings;						// 设备模式
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));			// 确保内存清空为零
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);			// Devmode 结构的大小
		dmScreenSettings.dmPelsWidth	= width;				// 所选屏幕宽度
		dmScreenSettings.dmPelsHeight	= height;				// 所选屏幕高度
		dmScreenSettings.dmBitsPerPel	= bits;					// 每象素所选的色彩深度
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// 尝试设置显示模式并返回结果。注: CDS_FULLSCREEN 移去了状态条。
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{

			// 若模式失败，提供两个选项：退出或在窗口内运行。
			if (MessageBox(NULL,"全屏模式在当前显卡上设置失败！\n使用窗口模式？","NeHe G",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{

				fullscreen=FALSE;				// 选择窗口模式(Fullscreen=FALSE)
			}
			else
			{

				// 弹出一个对话框，告诉用户程序结束
				MessageBox(NULL,"程序将被关闭","错误",MB_OK|MB_ICONSTOP);
				return FALSE;					//  退出并返回 FALSE
			}
		}
	}

	if (fullscreen)								// 仍处于全屏模式吗?
	{

		dwExStyle=WS_EX_APPWINDOW;					// 扩展窗体风格
		dwStyle=WS_POPUP;						// 窗体风格
		//ShowCursor(FALSE);						// 隐藏鼠标指针
	}
	else
	{

		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// 扩展窗体风格
		dwStyle=WS_OVERLAPPEDWINDOW;					//  窗体风格
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// 调整窗口达到真正要求的大小

	if (!(hWnd=CreateWindowEx(	dwExStyle,				// 扩展窗体风格
					"OpenG",				// 类名字
					title,					// 窗口标题
					WS_CLIPSIBLINGS |			// 必须的窗体风格属性
					WS_CLIPCHILDREN |			// 必须的窗体风格属性
					dwStyle,				// 选择的窗体属性
					0, 0,					// 窗口位置
					WindowRect.right-WindowRect.left,	// 计算调整好的窗口宽度
					WindowRect.bottom-WindowRect.top,	// 计算调整好的窗口高度
					NULL,					// 无父窗口
					NULL,					// 无菜单
					hInstance,				// 实例
					NULL)))					// 不向WM_CREATE传递任何东东

	{
		KillGLWindow();							// 重置显示区
		MessageBox(NULL,"不能创建一个窗口设备描述表","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// 返回 FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=					// /pfd 告诉窗口我们所希望的东东，即窗口使用的像素格式
	{
		sizeof(PIXELFORMATDESCRIPTOR),					// 上述格式描述符的大小
		1,								// 版本号
		PFD_DRAW_TO_WINDOW |						// 格式支持窗口
		PFD_SUPPORT_OPENGL |						// 格式必须支持OpenGL
		PFD_DOUBLEBUFFER,						// 必须支持双缓冲
		PFD_TYPE_RGBA,							// 申请 RGBA 格式
		bits,								// 选定色彩深度
		0, 0, 0, 0, 0, 0,						// 忽略的色彩位
		0,								// 无Alpha缓存
		0,								// 忽略Shift Bit
		0,								// 无累加缓存
		0, 0, 0, 0,							// 忽略聚集位
		16,								// 16位 Z-缓存 (深度缓存)
		0,								// 无蒙板缓存
		0,								// 无辅助缓存
		PFD_MAIN_PLANE,							// 主绘图层
		0,								// Reserved
		0, 0, 0								// 忽略层遮罩
	};

	if (!(hDC=GetDC(hWnd)))							// 取得设备描述表了么?
	{
		KillGLWindow();							// 重置显示区
		MessageBox(NULL,"不能创建一种相匹配的像素格式","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// 返回 FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))				// Windows 找到相应的象素格式了吗?
	{
		KillGLWindow();							// 重置显示区
		MessageBox(NULL,"不能设置像素格式","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// 返回 FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))				// 能够设置象素格式么?
	{
		KillGLWindow();							// 重置显示区
		MessageBox(NULL,"不能设置像素格式","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// 返回 FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))					// 能否取得着色描述表?
	{
		KillGLWindow();							// 重置显示区
		MessageBox(NULL,"不能创建OpenGL渲染描述表","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// 返回 FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))						// 尝试激活着色描述表
	{
		KillGLWindow();							// 重置显示区
		MessageBox(NULL,"不能激活当前的OpenGL渲然描述表","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// 返回 FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// 显示窗口
	SetForegroundWindow(hWnd);						// 略略提高优先级
	SetFocus(hWnd);								// 设置键盘的焦点至此窗口
	ReSizeGLScene(width, height);						// 设置透视 GL 屏幕

	if (!InitGL())								// 初始化新建的GL窗口
	{
		KillGLWindow();							// 重置显示区
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// 返回 FALSE
	}

	return TRUE;								// 成功
}

int InitGL(GLvoid)								// 此处开始对OpenGL进行所有设置
{
	glEnable(GL_TEXTURE_2D);						// 启用纹理映射
	glShadeModel(GL_SMOOTH);						// 启用阴影平滑
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);					// 黑色背景
	glClearDepth(1.0f);							// 设置深度缓存
	glEnable(GL_DEPTH_TEST);						// 启用深度测试
	glDepthFunc(GL_LEQUAL);							// 所作深度测试的类型
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// 真正精细的透视修正

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);					// Set The Blending Function For Translucency
	
	unsigned int iTexture;
	LoadTexture("earth.jpg", iTexture);
	CreateGlbRotmat(g_GlobeRotMat);



	MSG	msg;
	bool done = false;
	while(!done)								// 保持循环直到 done=TRUE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))			// 有消息在等待吗?
		{
			if (msg.message==WM_QUIT)				// 收到退出消息?
			{
				done=TRUE;					// 是，则done=TRUE
			}
			else							// 不是，处理窗口消息
			{
				TranslateMessage(&msg);				// 翻译消息
				DispatchMessage(&msg);				// 发送消息
			}
		}
		else								// 如果没有消息
		{
			// 绘制场景。监视ESC键和来自DrawGLScene()的退出消息
			if (active)						// 程序激活的么?
			{
				glLoadIdentity();							// 重置当前的模型观察矩阵
				glMatrixMode( GL_PROJECTION );
				glLoadIdentity();
				float W_H_Rate = 200/(float)200;
				glOrtho( -W_H_Rate, W_H_Rate, -1, 1, -10, 20);
				glMatrixMode( GL_MODELVIEW );
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				GlbPoint3d angle(g_rot_x/100.0*PI*2,g_rot_y/100.0*PI*2,g_rot_z/100.0*PI*2);
				AngleGlbRotmat(angle, g_GlobeRotMat);

				DrawGlobe(iTexture);
				//DrawCalib();
				SwapBuffers(hDC);			// 交换缓存 (双缓存)
				Sleep(10);
			}
		}
	}

	return TRUE;								// 初始化 OK
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)				// 重置OpenGL窗口大小
{
	if (height==0)								// 防止被零除
	{
		height=1;							// 将Height设为1
	}

	glViewport(0, 0, width, height);					// 重置当前的视口

	glMatrixMode(GL_PROJECTION);						// 选择投影矩阵
	glLoadIdentity();							// 重置投影矩阵

	// 设置视口的大小
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);						// 选择模型观察矩阵
	glLoadIdentity();							// 重置模型观察矩阵
}

LRESULT CALLBACK WndProc(	HWND	hWnd,					// 窗口的句柄
				UINT	uMsg,					// 窗口的消息
				WPARAM	wParam,					// 附加的消息内容
				LPARAM	lParam)					// 附加的消息内容
{

	switch (uMsg)								// 检查Windows消息
	{
		case WM_CREATE:
		{
			AllocConsole();
			freopen("CONOUT$", "w", stdout);
		}
		case WM_ACTIVATE:						// 监视窗口激活消息
		{
			if (!HIWORD(wParam))					// 检查最小化状态
			{
				active=TRUE;					// 程序处于激活状态
			}
			else
			{
				active=FALSE;					// 程序不再激活
			}

			return 0;						// 返回消息循环
		}

		case WM_SYSCOMMAND:						// 系统中断命令
		{
			switch (wParam)						// 检查系统调用
			{
				case SC_SCREENSAVE:				// 屏保要运行?
				case SC_MONITORPOWER:				// 显示器要进入节电模式?
				return 0;					// 阻止发生
			}
			break;							// 退出
		}

		case WM_CLOSE:							// 收到Close消息?
		{
			PostQuitMessage(0);					// 发出退出消息
			return 0;						// 返回
		}

		case WM_KEYDOWN:						// 有键按下么?
		{
			return 0;						// 返回
		}

		case WM_KEYUP:							// 有键放开么?
		{
			return 0;						// 返回
		}

		case WM_SIZE:							// 调整OpenGL窗口大小
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));		// LoWord=Width,HiWord=Height
			return 0;						// 返回
		}
	}

	// 向 DefWindowProc传递所有未处理的消息。
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void DrawCalib()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// 清除屏幕和深度缓存
	glLoadIdentity();											// 重置当前的模型观察矩阵
	glBindTexture(GL_TEXTURE_2D, 0);

	glPointSize(2.0f);
	GLfloat x,y,angle;

	int n= g_rot_x+2;

	for(int nLatitude=0; nLatitude<=n; nLatitude++)
	{
		glColor3f(1.0f,1.0f,1.0f);

		for(angle=0.0f;angle<2*PI;angle+=0.01f)
		{
			glBegin(GL_POINTS);
			float R = 1.0/n*nLatitude;
			x=R * sin(angle);
			y=R * cos(angle);
			glVertex2f(x,y);
			glEnd();			
		}
	}
}

DWORD WINAPI DrawGLScene(LPVOID lpParam)
{
	CreateGLWindow("MyUniverse",200,200,16,false);//glOrtho的参数与此对应
	return 1;
}
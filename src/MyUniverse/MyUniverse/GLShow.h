#pragma once
#include "gl\glut.h"
#include "gl\glaux.h"
#include "gl\gl.h"

GLvoid KillGLWindow(GLvoid);
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag);
GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
int InitGL(GLvoid);

LRESULT CALLBACK WndProc(	HWND	hWnd,					// 窗口的句柄
				UINT	uMsg,					// 窗口的消息
				WPARAM	wParam,					// 附加的消息内容
				LPARAM	lParam);					// 附加的消息内容

void DrawCalib();

DWORD WINAPI DrawGLScene(LPVOID lpParam);
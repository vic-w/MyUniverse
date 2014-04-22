#include "GlbFrame.h"
#include <cstdio>

using namespace glb;

void main()
{
	vector<GlbRect> screens;	//储存多屏幕信息
	glbDetectScreen(screens);	//检测屏幕个数和分辨率

	GlbRect rect(0,0,500,500);

    GlbWindow mainWindow;		//窗口
    glbCreateWindow(mainWindow, rect, ".\\calibmain.ini", true, false);	//生成一个窗口
	
	GlbRotmat GlobeRotMat;		//地球旋转量
	glbCreateGlbRotmat(GlobeRotMat);	//初始化旋转量（北极向上）

    glbSwitchWindow(mainWindow);	//切换到窗口（在读取图像之前）

	CurveRect curveRect(GLB_TEX_RECT, GlbPointGeo(0,0), true, GlbPointGeo(90,0), true, true, 360, 180);

	Object *earth = new Object(curveRect, "image\\earth.jpg");

	while(1)
	{
		glbClearWindow();	//清除窗口内容
		earth->Draw(GlobeRotMat, mainWindow.m_calib, 0);
		glbUpdateWindow(mainWindow,0);
	}
}

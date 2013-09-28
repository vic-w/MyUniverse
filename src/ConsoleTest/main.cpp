#include "GlbCore.h"


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

    GlbImage Image = glbLoadImage("earth.jpg");    //读取图像文件
    GlbImage Image2 = glbLoadImage("icon.jpg");    
    
    do
    {
        glbClearWindow();	//清除窗口内容

        glbDrawGlobe(Image, GlobeRotMat, mainWindow.m_calib); //画地球底图

        GlbPointGeo p1(40,120);	//北京的坐标
        GlbPointGeo p2(90,120); //北极点坐标

        glbDrawTexture(Image2, GlobeRotMat, mainWindow.m_calib, 
            p1, true, p2, false, true, 40, 30, 1, GLB_TEX_RECT);		//画图
        glbDrawLine(p1, true, p2, false, GlobeRotMat, mainWindow.m_calib, 2);		//画线
		glbDrawCircle(p1, true, 30, GlobeRotMat, mainWindow.m_calib, 3);

		vector<GlbMove> move;
		vector<GlbPoint2d> touch;
		glbPopTouchSignal(mainWindow, move, touch);

		vector<GlbMove>::iterator it;
		for( it=move.begin(); it!=move.end(); it++)
		{
			//printf("from: (%f,%f) to (%f,%f)\n" , it->m_pFrom.m_x, it->m_pFrom.m_y, it->m_pTo.m_x, it->m_pTo.m_y);
			GlbPoint3d from3d, to3d;
			glbPointRound2PointRect(it->m_pFrom, from3d, mainWindow.m_calib);
			glbPointRound2PointRect(it->m_pTo, to3d, mainWindow.m_calib);

			GlbRotmat rotation;
			glbMovingPoints2RotMat(from3d, to3d, rotation);
			glbRotmatMul(rotation, GlobeRotMat, GlobeRotMat);
		}
		
    }
    while(glbUpdateWindow(mainWindow,0));

    glbReleaseImage(&Image);		//释放临时变量
    glbReleaseImage(&Image2);
    glbDestoryWindow(mainWindow);
    return;
}

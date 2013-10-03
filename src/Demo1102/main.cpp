﻿#include "GlbCore.h"

int nMode=1;
bool bShowMenu=false;

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

    GlbImage earth_img = glbLoadImage("image\\earth.jpg");    //读取图像文件
    GlbImage icon_img = glbLoadImage("image\\icon.png");    
    GlbImage menu_img = glbLoadImage("image\\menu.png");    
    GlbImage mode1_img = glbLoadImage("image\\mode1.png");    
    GlbImage mode2_img = glbLoadImage("image\\mode2.png");    
    GlbImage mode3_img = glbLoadImage("image\\mode3.png");    
    GlbImage mode4_img = glbLoadImage("image\\mode4.png");    
    GlbImage mode5_img = glbLoadImage("image\\mode5.png");    

    do
    {
        glbClearWindow();	//清除窗口内容

        glbDrawGlobe(earth_img, GlobeRotMat, mainWindow.m_calib); //画地球底图

		vector<GlbMove> move;
		vector<GlbPoint2d> touch;
		glbPopTouchSignal(mainWindow, move, touch);	//取得触摸点击信号

        GlbPointGeo p1(90,0);
        GlbPointGeo p2(90,0);

        //画菜单按钮
		if(nMode == 1)
		{
			glbDrawTexture(mode1_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 20, 20, 1, GLB_TEX_RECT);
		}
		else if(nMode ==2)
		{
			glbDrawTexture(mode2_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 20, 20, 1, GLB_TEX_RECT);
		}
		else if(nMode ==3)
		{
			glbDrawTexture(mode3_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 20, 20, 1, GLB_TEX_RECT);
		}
		else if(nMode ==4)
		{
			glbDrawTexture(mode4_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 20, 20, 1, GLB_TEX_RECT);
		}
		else if(nMode ==5)
		{
			glbDrawTexture(mode5_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 20, 20, 1, GLB_TEX_RECT);
		}

        if(bShowMenu)
        {
            p1.m_lat = 60; p1.m_lng = 0;
            glbDrawTexture(mode1_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 20, 20, 2, GLB_TEX_RECT);
            p1.m_lng = 72;
            glbDrawTexture(mode2_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 20, 20, 3, GLB_TEX_RECT);
            p1.m_lng = 144;
            glbDrawTexture(mode3_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 20, 20, 4, GLB_TEX_RECT);
            p1.m_lng = -144;
            glbDrawTexture(mode4_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 20, 20, 5, GLB_TEX_RECT);
            p1.m_lng = -72;
            glbDrawTexture(mode5_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 20, 20, 6, GLB_TEX_RECT);
        }

		//根据触摸移动信号转动地球
		vector<GlbMove>::iterator m_it;
		for( m_it=move.begin(); m_it!=move.end(); m_it++)
		{
			//printf("from: (%f,%f) to (%f,%f)\n" , it->m_pFrom.m_x, it->m_pFrom.m_y, it->m_pTo.m_x, it->m_pTo.m_y);
			GlbPoint3d from3d, to3d;
			glbPointRound2PointRect(m_it->m_pFrom, from3d, mainWindow.m_calib);
			glbPointRound2PointRect(m_it->m_pTo, to3d, mainWindow.m_calib);

			GlbRotmat rotation;
			glbMovingPoints2RotMat(from3d, to3d, rotation);
			glbRotmatMul(rotation, GlobeRotMat, GlobeRotMat);
		}

		//在点击处画一个圆圈
		vector<GlbPoint2d>::iterator t_it;
		for( t_it=touch.begin(); t_it!=touch.end(); t_it++)
		{
			GlbPoint3d point3d;
			glbPointRound2PointRect(*t_it, point3d, mainWindow.m_calib);

			GlbPointGeo pointGeo;
			glbPointRect2PointGeo(point3d, pointGeo);

			glbDrawCircle(pointGeo, false, 5, GlobeRotMat, mainWindow.m_calib, 1024);

			int HitLayer = glbGetTopLayer(mainWindow, *t_it);
			printf("Hit layer = %d\n", HitLayer);
            if(HitLayer == 1)
            {
                bShowMenu = !bShowMenu;
            }
            else if(HitLayer >= 2 && HitLayer <= 6)
            {
                nMode = HitLayer - 1;
                printf("switch to mode %d\n", nMode);
                bShowMenu = false;
            }
            
		} 
		
    }
    while(glbUpdateWindow(mainWindow,0));

    glbReleaseImage(&earth_img);		//释放临时变量
    glbReleaseImage(&icon_img);
    glbReleaseImage(&menu_img);		//释放临时变量
    glbReleaseImage(&mode1_img);
    glbReleaseImage(&mode2_img);		//释放临时变量
    glbReleaseImage(&mode3_img);
    glbReleaseImage(&mode4_img);		//释放临时变量
    glbReleaseImage(&mode5_img);

    glbDestoryWindow(mainWindow);
    return;
}
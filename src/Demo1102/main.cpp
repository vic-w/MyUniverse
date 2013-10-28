#include "GlbCore.h"
#include "GlbTouch.h"
#include "city.h"
#include "menu_icon.h"
#include "mode1.h"
#include "mode2.h"
#include "mode3.h"
#include "mode4.h"
#include "mode5.h"

//全局变量
int nMode=1;
bool bShowMenu=false;

//地球转动惯量
GlbPivot g_GlobeRotPivot(0,1,0);
float g_GlobeRotSpeed = 0;

//菜单转动惯量
float g_MenuLng = 0;
float g_MenuRotSpeed = 0;



bool invoketext2ImageGenerator(LPCTSTR param)
{
	SHELLEXECUTEINFO sei = {0};
	sei.cbSize = sizeof(SHELLEXECUTEINFO);
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	sei.hwnd = NULL;
	//sei.lpVerb = TEXT("runas"); //以管理员身份运行，如果XML文件放在C:\Program Files下， 需要以管理员运行才能修改文件。正确的做法是把需要写权限的文件移到AppData或者ProgramData目录下面去。
	TCHAR szPath[MAX_PATH] ;
	GetModuleFileName(NULL, szPath, MAX_PATH) ;
	PathRemoveFileSpec(szPath) ;
	CString file = CString(szPath) + TEXT("\\text2ImageGenerator.exe");
	sei.lpFile = file;//使用全路径
		
	sei.lpParameters = param;
	sei.nShow = SW_SHOWNORMAL;
	if (!ShellExecuteEx(&sei))
	{
		DWORD dwStatus = GetLastError();
		if (dwStatus == ERROR_CANCELLED)
		{
			printf("user cancelled the elevated request!");
		}
		else if (dwStatus == ERROR_FILE_NOT_FOUND)
		{
			printf("File not found!");			
		}
		return false;
	}
	else
	{
		WaitForSingleObject(sei.hProcess,INFINITE);
		DWORD dwRet;
		GetExitCodeProcess(sei.hProcess, &dwRet);
		if (dwRet == 0)
			return true;
		else
		{
			printf("Failed to invoke text2ImageGenerator, please check log.");
			return false;
		}
	}
}

//把中文字符写到图片上以供显示， 不提供底图，返回png格式
bool txt2ImgHelper(int mode, char* myString)
{
	if (!myString || (strlen(myString)== 0))
		return false;
	CString param;
	CString text(myString); 	
	param.Format(_T("%s"), (LPCTSTR)text);
	return invoketext2ImageGenerator(param);
}

//把中文字符写到图片上以供显示, 提供底图，底图可以为bmp，jpg或png格式
bool txt2ImgHelper(char* imgFile, int mode, char* myString)
{
	if (!myString || (strlen(myString)== 0))
		return false;
	CString param;
	CString img(imgFile); 
	CString text(myString); 
	
	param.Format(_T("%s %d %s"), (LPCTSTR)img, mode, (LPCTSTR)text);
	return invoketext2ImageGenerator(param);
}

bool invokeValidatorHelper()
{
	SHELLEXECUTEINFO sei = {0};
	sei.cbSize = sizeof(SHELLEXECUTEINFO);
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	sei.hwnd = NULL;
	sei.lpVerb = TEXT("runas"); //以管理员身份运行
	TCHAR szPath[MAX_PATH] ;
	GetModuleFileName(NULL, szPath, MAX_PATH) ;
	PathRemoveFileSpec(szPath) ;
	CString file = CString(szPath) + TEXT("\\Validator.exe");
	
	//
	FILE* fp = _tfopen(file, TEXT("rb"));
	if (!fp) 
	{
		printf("找不到文件a");
		return false;
	}
	fseek(fp, 0, SEEK_END);
	int lengthOfFile = ftell(fp);
	fclose(fp);
	if (lengthOfFile != 11776) //防止他人反编译Validator
	{
		printf("找不到文件b");
		return false;
	}

	sei.lpFile = file;//使用全路径
		
	sei.nShow = SW_SHOWNORMAL;
	if (!ShellExecuteEx(&sei))
	{
		DWORD dwStatus = GetLastError();
		if (dwStatus == ERROR_CANCELLED)
		{
			printf("user cancelled the elevated request!");
		}
		else if (dwStatus == ERROR_FILE_NOT_FOUND)
		{
			printf("File not found!");			
		}
		return false;
	}
	else
	{
		WaitForSingleObject(sei.hProcess,INFINITE);
		DWORD dwRet;
		GetExitCodeProcess(sei.hProcess, &dwRet);
		if (dwRet == 0)
			return true;
		else
		{
			//printf("Failed to invoke Validator, please check log.");
			return false;
		}
	}
}

void main()
{
	if (!invokeValidatorHelper())
	{
		MessageBox(NULL, TEXT("如果桌面上有unsigned.dat生成，请把文件发给我。"), TEXT("用户校验失败"), MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	printf("更新Xml的信息...\n");
	//更新Xml的信息
	/*if (!CCity::updateXml())
	{
		return;
	}*/

	vector<GlbRect> screens;	//储存多屏幕信息
	glbDetectScreen(screens);	//检测屏幕个数和分辨率

	GlbRect rect(0,0,screens[0].m_height,screens[0].m_height);

    GlbWindow mainWindow;		//窗口
    glbCreateWindow(mainWindow, screens.back(), ".\\calibmain.ini", true, false);	//生成一个窗口
	
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

	glbListenTouchSignal(mainWindow, 3333);//在3333端口监听TUIO信号


    //建立5个mode
	CMode1 mode1(&GlobeRotMat, &mainWindow);
	CMode2 mode2(&GlobeRotMat, &mainWindow);
	CMode3 mode3(&GlobeRotMat, &mainWindow);
	CMode4 mode4(&GlobeRotMat, &mainWindow);
	CMode5 mode5(&GlobeRotMat, &mainWindow);

    do
    {
        glbClearWindow();	//清除窗口内容

		//旋转地球
		GlbRotmat rotation;
		glbAnglePivot2RotMat(g_GlobeRotPivot, g_GlobeRotSpeed, rotation);
		glbRotmatMul(rotation, GlobeRotMat, GlobeRotMat);
		if(GlobeRotMat.r11!=GlobeRotMat.r11)
		{
			int a=0;//调试用，检测浮点数溢出
		}
		g_GlobeRotSpeed *= 0.95;

		//转动菜单
		g_MenuLng += g_MenuRotSpeed;
		while(g_MenuLng >180)
		{
			g_MenuLng -= 360;
		}
		g_MenuRotSpeed *= 0.95;

        glbDrawGlobe(earth_img, GlobeRotMat, mainWindow.m_calib); //画地球底图

        if(nMode == 1)
        {
		    mode1.draw();
        }
        else if(nMode == 2)
        {
            mode2.draw();
        }
        else if(nMode == 3)
        {
            mode3.draw();
        }
        else if(nMode == 4)
        {
            mode4.draw();
        }
        else if(nMode == 5)
        {
            mode5.draw();
        }
        else
        {
        }

		vector<GlbMove> move;
		vector<GlbPoint2d> touch;
		glbPopTouchSignal(mainWindow, move, touch);	//取得触摸点击信号

        GlbPointGeo p1(90,0);
        GlbPointGeo p2(90,0);

        //画菜单按钮
		if(nMode == 1)
		{
			glbDrawTexture(mode1_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 40, 40, LAYER_MENU_START, GLB_TEX_RECT);
		}
		else if(nMode ==2)
		{
			glbDrawTexture(mode2_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 40, 40, LAYER_MENU_START, GLB_TEX_RECT);
		}
		else if(nMode ==3)
		{
			glbDrawTexture(mode3_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 40, 40, LAYER_MENU_START, GLB_TEX_RECT);
		}
		else if(nMode ==4)
		{
			glbDrawTexture(mode4_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 40, 40, LAYER_MENU_START, GLB_TEX_RECT);
		}
		else if(nMode ==5)
		{
			glbDrawTexture(mode5_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 40, 40, LAYER_MENU_START, GLB_TEX_RECT);
		}

        if(bShowMenu)
        {
            p1.m_lat = 30; p1.m_lng = g_MenuLng + 0;
            glbDrawTexture(mode1_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 20, 20, LAYER_MENU_START+1, GLB_TEX_RECT);
            p1.m_lng = g_MenuLng + 72;
            glbDrawTexture(mode2_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 20, 20, LAYER_MENU_START+2, GLB_TEX_RECT);
            p1.m_lng = g_MenuLng + 144;
            glbDrawTexture(mode3_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 20, 20, LAYER_MENU_START+3, GLB_TEX_RECT);
            p1.m_lng = g_MenuLng -144;
            glbDrawTexture(mode4_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 20, 20, LAYER_MENU_START+4, GLB_TEX_RECT);
            p1.m_lng = g_MenuLng -72;
            glbDrawTexture(mode5_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 20, 20, LAYER_MENU_START+5, GLB_TEX_RECT);
        }

		//画多边形
		//float length = 6371.0f / 180.0f * 3.14f * glbDrawPolygon(polygon, true, false, GlobeRotMat, mainWindow.m_calib, LAYER_LINES);
		//printf("多边形长度 = %f公里\n",length);

		//根据触摸移动信号转动地球
		vector<GlbMove>::iterator m_it;
		for( m_it=move.begin(); m_it!=move.end(); m_it++)
		{
			//printf("from: (%f,%f) to (%f,%f)\n" , it->m_pFrom.m_x, it->m_pFrom.m_y, it->m_pTo.m_x, it->m_pTo.m_y);
			GlbPoint3d from3d, to3d;
			glbPointRound2PointRect(m_it->m_pFrom, from3d, mainWindow.m_calib);
			glbPointRound2PointRect(m_it->m_pTo, to3d, mainWindow.m_calib);

			int HitLayer = glbGetTopLayer(mainWindow, m_it->m_pFrom);

			if(HitLayer < LAYER_MENU_START)
			{
				g_GlobeRotSpeed = glbAngleBetweenPoints(from3d, to3d);
				glbPivotBetweenPoints(from3d, to3d, g_GlobeRotPivot);
				if(g_GlobeRotPivot.m_x != g_GlobeRotPivot.m_x)
				{
					int a=0;//调试用，检测浮点数溢出
				}
			}
			else if(HitLayer >LAYER_MENU_START && HitLayer <= LAYER_MENU_START+5)
			{
				GlbPointGeo pFromGeo, pToGeo;
				glbPointRect2PointGeo(from3d, pFromGeo);
				glbPointRect2PointGeo(to3d, pToGeo);
				GlbPointGeo MenuRot = pToGeo - pFromGeo;
				g_MenuRotSpeed = MenuRot.m_lng;
			}
		}

		//在点击处画一个圆圈
		vector<GlbPoint2d>::iterator t_it;
		for( t_it=touch.begin(); t_it!=touch.end(); t_it++)
		{
			GlbPoint3d point3d_screen, point3d_globe;
			glbPointRound2PointRect(*t_it, point3d_screen, mainWindow.m_calib);

			GlbPointGeo pointGeo_screen, pointGeo_globe;
			glbPointRect2PointGeo(point3d_screen, pointGeo_screen);

			glbDrawCircle(pointGeo_screen, false, 5, GlobeRotMat, mainWindow.m_calib, LAYER_CIRCLE);

			int HitLayer = glbGetTopLayer(mainWindow, *t_it);
			printf("Hit layer = %d\n", HitLayer);

            //判断menu的点击
			if(HitLayer == LAYER_GLOBE)
			{
                //划线
				//glbScreenPoint2GlobePoint(point3d_screen, GlobeRotMat, point3d_globe);
				//glbPointRect2PointGeo(point3d_globe, pointGeo_globe);
				//polygon.push_back(pointGeo_globe);
				//float length = 6371.0f / 180.0f * 3.14f * glbDrawPolygon(polygon, true, false, GlobeRotMat, mainWindow.m_calib, LAYER_LINES);
				//printf("多边形长度 = %f公里\n",length);
			}
            else if(HitLayer == LAYER_MENU_START)
            {
                bShowMenu = !bShowMenu;
            }
            else if(HitLayer >LAYER_MENU_START && HitLayer <= LAYER_MENU_START+5)
            {
                nMode = HitLayer - LAYER_MENU_START;
                mode1.reset();
                mode2.reset();
                mode3.reset();
                mode4.reset();
                mode5.reset();
                printf("switch to mode %d\n", nMode);
                bShowMenu = false;
            }

            //判断mode中的点击

            if(nMode == 1)
            {
		        mode1.onClick(HitLayer);
            }
            else if(nMode == 2)
            {
                mode2.onClick(HitLayer);
            }
            else if(nMode == 3)
            {
                mode3.onClick(HitLayer);
            }
            else if(nMode == 4)
            {
                mode4.onClick(HitLayer, *t_it);
            }
            else if(nMode == 5)
            {
                mode5.onClick(HitLayer);
            }
            else
            {
            }
		} 

    }
    while(glbUpdateWindow(mainWindow,0));

    glbReleaseImage(&earth_img);		//释放临时变量
    glbReleaseImage(&icon_img);
    glbReleaseImage(&menu_img);			//释放临时变量
    glbReleaseImage(&mode1_img);
    glbReleaseImage(&mode2_img);		//释放临时变量
    glbReleaseImage(&mode3_img);
    glbReleaseImage(&mode4_img);		//释放临时变量
    glbReleaseImage(&mode5_img);

    glbDestoryWindow(mainWindow);
    return;
}

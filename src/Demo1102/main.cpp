#include "GlbCore.h"
#include "GlbTouch.h"
#include "city.h"

//全局变量
int nMode=1;
bool bShowMenu=false;

enum ENUM_LAYERS
{
    LAYER_GLOBE = 0,
    LAYER_CITY_ICON_START = 100,
	LAYER_LINES = 400,
	LAYER_CITY_DETAIL = 500,
	LAYER_MENU_START = 1000
};

//mode1 城市
class CMode1
{
public:
	GlbImage m_icon;
	GlbRotmat *m_pGlobeRotMat;
	GlbWindow *m_pWindow;
	vector<CCity> m_cities;
	bool m_bShowDetail;
	int m_nShowCity;
public:
	CMode1(GlbRotmat *pGlobeRotMat, GlbWindow *pWindow)
	{
		m_cities = CCity::getCities();
		m_icon = glbLoadImage("image\\icon.png");
		m_pGlobeRotMat = pGlobeRotMat;
		m_pWindow = pWindow;
	}
	void draw()
	{
		vector<CCity>::iterator it;
		int layer = LAYER_CITY_ICON_START;
		for(it=m_cities.begin(); it!=m_cities.end(); it++)
		{
			GlbPointGeo p1(it->latitude, it->longitude);
			GlbPointGeo p2(90,0);
			glbDrawTexture(m_icon, *m_pGlobeRotMat, m_pWindow->m_calib, p1, true, p2, true, true, 5, 5, layer++, GLB_TEX_RECT);
		}
		if(m_bShowDetail)
		{

			//glbDrawTexture(m_icon, *m_pGlobeRotMat, m_pWindow->m_calib, p1, true, p2, true, true, 5, 5, layer++, GLB_TEX_RECT);
		}
	}
	void reset()
	{
	}
};
//mode2 时间
//mode3 时差
//mode4 多边形
//mode5 天气


void main()
{
	vector<CCity> cities = CCity::getCities(); //读取Xml中的城市列表
	printf("%s\n", cities[5].getLocalTimeString()); //读取某个城市的当地时间
	printf("%s\n", CCity::getTimezoneDiffString(cities[2], cities[5]));//两个城市之间的时间差
	printf("%s\n", CCity::getTimezoneDiffString(cities[5]));//某个城市和北京的时间差

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

	glbListenTouchSignal(mainWindow, 3333);//在3333端口监听TUIO信号

	vector<GlbPointGeo> polygon;
	CMode1 mode1(&GlobeRotMat, &mainWindow);

    do
    {
        glbClearWindow();	//清除窗口内容

        glbDrawGlobe(earth_img, GlobeRotMat, mainWindow.m_calib); //画地球底图

		mode1.draw();

		vector<GlbMove> move;
		vector<GlbPoint2d> touch;
		glbPopTouchSignal(mainWindow, move, touch);	//取得触摸点击信号

        GlbPointGeo p1(90,0);
        GlbPointGeo p2(90,0);

        //画菜单按钮
		if(nMode == 1)
		{
			glbDrawTexture(mode1_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 20, 20, LAYER_MENU_START, GLB_TEX_RECT);
		}
		else if(nMode ==2)
		{
			glbDrawTexture(mode2_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 20, 20, LAYER_MENU_START, GLB_TEX_RECT);
		}
		else if(nMode ==3)
		{
			glbDrawTexture(mode3_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 20, 20, LAYER_MENU_START, GLB_TEX_RECT);
		}
		else if(nMode ==4)
		{
			glbDrawTexture(mode4_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 20, 20, LAYER_MENU_START, GLB_TEX_RECT);
		}
		else if(nMode ==5)
		{
			glbDrawTexture(mode5_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 20, 20, LAYER_MENU_START, GLB_TEX_RECT);
		}

        if(bShowMenu)
        {
            p1.m_lat = 60; p1.m_lng = 0;
            glbDrawTexture(mode1_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 20, 20, LAYER_MENU_START+1, GLB_TEX_RECT);
            p1.m_lng = 72;
            glbDrawTexture(mode2_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 20, 20, LAYER_MENU_START+2, GLB_TEX_RECT);
            p1.m_lng = 144;
            glbDrawTexture(mode3_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 20, 20, LAYER_MENU_START+3, GLB_TEX_RECT);
            p1.m_lng = -144;
            glbDrawTexture(mode4_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 20, 20, LAYER_MENU_START+4, GLB_TEX_RECT);
            p1.m_lng = -72;
            glbDrawTexture(mode5_img, GlobeRotMat, mainWindow.m_calib, p1, false, p2, false, true, 20, 20, LAYER_MENU_START+5, GLB_TEX_RECT);
        }

		//画多边形
		float length = 6371.0f / 180.0f * 3.14f * glbDrawPolygon(polygon, true, false, GlobeRotMat, mainWindow.m_calib, LAYER_LINES);
		//printf("多边形长度 = %f公里\n",length);

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
			GlbPoint3d point3d_screen, point3d_globe;
			glbPointRound2PointRect(*t_it, point3d_screen, mainWindow.m_calib);

			GlbPointGeo pointGeo_screen, pointGeo_globe;
			glbPointRect2PointGeo(point3d_screen, pointGeo_screen);

			glbDrawCircle(pointGeo_screen, false, 5, GlobeRotMat, mainWindow.m_calib, LAYER_LINES);

			int HitLayer = glbGetTopLayer(mainWindow, *t_it);
			printf("Hit layer = %d\n", HitLayer);
			if(HitLayer == 0)
			{
				glbScreenPoint2GlobePoint(point3d_screen, GlobeRotMat, point3d_globe);
				glbPointRect2PointGeo(point3d_globe, pointGeo_globe);
				polygon.push_back(pointGeo_globe);
				float length = 6371.0f / 180.0f * 3.14f * glbDrawPolygon(polygon, true, false, GlobeRotMat, mainWindow.m_calib, LAYER_LINES);
				printf("多边形长度 = %f公里\n",length);
			}
            else if(HitLayer == LAYER_MENU_START)
            {
                bShowMenu = !bShowMenu;
            }
            else if(HitLayer >LAYER_MENU_START && HitLayer <= LAYER_MENU_START+5)
            {
                nMode = HitLayer - LAYER_MENU_START;
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

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
	LAYER_MENU_START = 1000,
    LAYER_CIRCLE = 1100
};

//mode1 城市
class CMode1
{
public:
	GlbImage m_icon;
    GlbImage cityView;
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
        m_bShowDetail = false;
        m_nShowCity = 0;
	}
    ~CMode1()
    {
        glbReleaseImage(&m_icon);
        glbReleaseImage(&cityView);
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
            GlbPointGeo p1(m_cities[m_nShowCity].latitude, m_cities[m_nShowCity].longitude);
			GlbPointGeo p2(90,0);
			glbDrawTexture(cityView, *m_pGlobeRotMat, m_pWindow->m_calib, p1, true, p2, false, true, 40, 30, LAYER_CITY_DETAIL, GLB_TEX_RECT);
            
		}
	}
	void reset()
	{
        m_bShowDetail = false;
        glbReleaseImage(&cityView);
	}
    void onClick(int layer)
    {
        if(layer >= LAYER_CITY_ICON_START && layer < LAYER_CITY_ICON_START+m_cities.size() && !m_bShowDetail)
        {
            m_nShowCity = layer - LAYER_CITY_ICON_START;
            cityView = glbLoadImage( m_cities[m_nShowCity].path );
            m_bShowDetail = true;
        }
        else if(layer == LAYER_CITY_DETAIL)
        {
            m_bShowDetail = false;
            glbReleaseImage(&cityView);
            glbReleaseImage(&cityView);
        }
    }
};
//mode2 时间
class CMode2
{
public:
	GlbImage m_icon;
    GlbImage cityView;
	GlbRotmat *m_pGlobeRotMat;
	GlbWindow *m_pWindow;
	vector<CCity> m_cities;
	bool m_bShowDetail;
	int m_nShowCity;
public:
	CMode2(GlbRotmat *pGlobeRotMat, GlbWindow *pWindow)
	{
		m_cities = CCity::getCities();
		m_icon = glbLoadImage("image\\icon.png");
		m_pGlobeRotMat = pGlobeRotMat;
		m_pWindow = pWindow;
        m_bShowDetail = false;
        m_nShowCity = 0;
	}
    ~CMode2()
    {
        glbReleaseImage(&m_icon);
        glbReleaseImage(&cityView);
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
            GlbPointGeo p1(m_cities[m_nShowCity].latitude, m_cities[m_nShowCity].longitude);
			GlbPointGeo p2(90,0);
			glbDrawTexture(cityView, *m_pGlobeRotMat, m_pWindow->m_calib, p1, true, p2, false, true, 40, 30, LAYER_CITY_DETAIL, GLB_TEX_RECT);
		}
	}
	void reset()
	{
        m_bShowDetail = false;
        glbReleaseImage(&cityView);
	}
    void onClick(int layer)
    {
        if(layer >= LAYER_CITY_ICON_START && layer < LAYER_CITY_ICON_START+m_cities.size() && !m_bShowDetail)
        {
            m_nShowCity = layer - LAYER_CITY_ICON_START;
            cityView = glbLoadImage( "image\\text.jpg" );
            m_bShowDetail = true;
        }
        else if(layer == LAYER_CITY_DETAIL)
        {
            m_bShowDetail = false;
            glbReleaseImage(&cityView);
            glbReleaseImage(&cityView);
        }
    }
};
//mode3 时差
class CMode3
{
public:
	GlbImage m_icon;
    GlbImage cityView;
	GlbRotmat *m_pGlobeRotMat;
	GlbWindow *m_pWindow;
	vector<CCity> m_cities;
    int city1;
    int city2;
	int  status;//0 没有选择， 1 选择了第一个城市， 2 选择了第二个城市，并显示信息

public:
	CMode3(GlbRotmat *pGlobeRotMat, GlbWindow *pWindow)
	{
		m_cities = CCity::getCities();
		m_icon = glbLoadImage("image\\icon.png");
		m_pGlobeRotMat = pGlobeRotMat;
		m_pWindow = pWindow;
        status = 0;
	}
    ~CMode3()
    {
        glbReleaseImage(&m_icon);
        glbReleaseImage(&cityView);
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
		if(status == 0)
		{
		}
        else if(status == 1)
        {
            GlbPointGeo p(m_cities[city1].latitude, m_cities[city1].longitude);
            glbDrawCircle(p, true, 3, *m_pGlobeRotMat, m_pWindow->m_calib, LAYER_LINES);
        }
        else if(status == 2)
        {
            GlbPointGeo p1(m_cities[city1].latitude, m_cities[city1].longitude);
            glbDrawCircle(p1, true, 3, *m_pGlobeRotMat, m_pWindow->m_calib, LAYER_LINES);
            GlbPointGeo p2(m_cities[city2].latitude, m_cities[city2].longitude);
            GlbPointGeo polar(90,0);
            glbDrawLine(p1, true, p2, true, *m_pGlobeRotMat, m_pWindow->m_calib, LAYER_LINES);
            glbDrawTexture(cityView, *m_pGlobeRotMat, m_pWindow->m_calib, p2, true, polar, false, true, 40, 30, LAYER_CITY_DETAIL, GLB_TEX_RECT);
        }
	}
	void reset()
	{
        status = 0;
        glbReleaseImage(&cityView);
	}
    void onClick(int layer)
    {
        if(layer >= LAYER_CITY_ICON_START && layer < LAYER_CITY_ICON_START+m_cities.size() && status == 0)
        {
            city1 = layer - LAYER_CITY_ICON_START;
            status = 1;
        }
        else if(layer >= LAYER_CITY_ICON_START && layer < LAYER_CITY_ICON_START+m_cities.size() && status == 1)
        {
            city2 = layer - LAYER_CITY_ICON_START;
            if(city1 != city2)
            {
                cityView = glbLoadImage( "image\\text.jpg" );
                status = 2;
            }
            else
            {
                glbReleaseImage(&cityView);
                status = 0;
            }
        }
        else if(layer == LAYER_CITY_DETAIL)
        {
            glbReleaseImage(&cityView);
            status =0;
        }
    }
};
//mode4 多边形
class CMode4
{
public:
	GlbImage m_icon;
    GlbImage cityView;
	GlbRotmat *m_pGlobeRotMat;
	GlbWindow *m_pWindow;
	vector<CCity> m_cities;
	bool m_bClosed;
    vector<GlbPointGeo> polygon;
public:
	CMode4(GlbRotmat *pGlobeRotMat, GlbWindow *pWindow)
	{
		m_cities = CCity::getCities();
		m_icon = glbLoadImage("image\\icon.png");
		m_pGlobeRotMat = pGlobeRotMat;
		m_pWindow = pWindow;
        m_bClosed = false;
	}
    ~CMode4()
    {
        glbReleaseImage(&m_icon);
        glbReleaseImage(&cityView);
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
        float length = 6371.0f / 180.0f * 3.14f * glbDrawPolygon(polygon, true, m_bClosed, *m_pGlobeRotMat, m_pWindow->m_calib, LAYER_LINES);
        if(polygon.size()>0)
        {
            glbDrawCircle(polygon[0], true, 3, *m_pGlobeRotMat, m_pWindow->m_calib, LAYER_LINES);
        }
	}
	void reset()
	{
        polygon.clear();
        m_bClosed = false;
        glbReleaseImage(&cityView);
	}
    void onClick(int layer, GlbPoint2d point2d_screen)
    {
        if(layer <= LAYER_LINES && !m_bClosed)
        {
            GlbPoint3d point3d_screen, point3d_globe;
			glbPointRound2PointRect(point2d_screen, point3d_screen, m_pWindow->m_calib);

            glbScreenPoint2GlobePoint(point3d_screen, *m_pGlobeRotMat, point3d_globe);
            GlbPointGeo pointGeo_globe;
		    glbPointRect2PointGeo(point3d_globe, pointGeo_globe);
			polygon.push_back(pointGeo_globe);

            if(polygon.size()>1)
            {
                GlbPoint3d startPoint3d;
                glbPointGeo2PointRect(polygon[0], startPoint3d);
                if( glbAngleBetweenPoints(startPoint3d, point3d_globe) < 3 )
                {
                    m_bClosed = true;
                }
            }
            //cityView = glbLoadImage( "image\\text.jpg" );
            //m_bShowDetail = true;
        }
    }
};
//mode5 天气
class CMode5
{
public:
	GlbImage m_icon;
    GlbImage cityView;
	GlbRotmat *m_pGlobeRotMat;
	GlbWindow *m_pWindow;
	vector<CCity> m_cities;
	bool m_bShowDetail;
	int m_nShowCity;
public:
	CMode5(GlbRotmat *pGlobeRotMat, GlbWindow *pWindow)
	{
		m_cities = CCity::getCities();
		m_icon = glbLoadImage("image\\icon.png");
		m_pGlobeRotMat = pGlobeRotMat;
		m_pWindow = pWindow;
        m_bShowDetail = false;
        m_nShowCity = 0;
	}
    ~CMode5()
    {
        glbReleaseImage(&m_icon);
        glbReleaseImage(&cityView);
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
            GlbPointGeo p1(m_cities[m_nShowCity].latitude, m_cities[m_nShowCity].longitude);
			GlbPointGeo p2(90,0);
			glbDrawTexture(cityView, *m_pGlobeRotMat, m_pWindow->m_calib, p1, true, p2, false, true, 40, 30, LAYER_CITY_DETAIL, GLB_TEX_RECT);
		}
	}
	void reset()
	{
        m_bShowDetail = false;
        glbReleaseImage(&cityView);
	}
    void onClick(int layer)
    {
        if(layer >= LAYER_CITY_ICON_START && layer < LAYER_CITY_ICON_START+m_cities.size() && !m_bShowDetail)
        {
            m_nShowCity = layer - LAYER_CITY_ICON_START;
            cityView = glbLoadImage( "image\\text.jpg" );
            m_bShowDetail = true;
        }
        else if(layer == LAYER_CITY_DETAIL)
        {
            m_bShowDetail = false;
            glbReleaseImage(&cityView);
            glbReleaseImage(&cityView);
        }
    }
};


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


    //建立5个mode
	CMode1 mode1(&GlobeRotMat, &mainWindow);
	CMode2 mode2(&GlobeRotMat, &mainWindow);
	CMode3 mode3(&GlobeRotMat, &mainWindow);
	CMode4 mode4(&GlobeRotMat, &mainWindow);
	CMode5 mode5(&GlobeRotMat, &mainWindow);

    do
    {
        glbClearWindow();	//清除窗口内容

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
    glbReleaseImage(&menu_img);		//释放临时变量
    glbReleaseImage(&mode1_img);
    glbReleaseImage(&mode2_img);		//释放临时变量
    glbReleaseImage(&mode3_img);
    glbReleaseImage(&mode4_img);		//释放临时变量
    glbReleaseImage(&mode5_img);

    glbDestoryWindow(mainWindow);
    return;
}

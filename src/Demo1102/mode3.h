#pragma once
#include "global.h"


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
		cityView = 0;
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
            glbDrawTexture(cityView, *m_pGlobeRotMat, m_pWindow->m_calib, p2, true, polar, false, true, 80, 8, LAYER_CITY_DETAIL, GLB_TEX_RECT);
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
				if (txt2ImgHelper(3, CCity::getTimezoneDiffString(m_cities[city2], m_cities[city1])))//3为模式
				{
					cityView = glbLoadImage( "temp.png" );
				}
				else
				{
					cityView = glbLoadImage("image\\error.png");
				}
                status = 2;
            }
            else
            {
                glbReleaseImage(&cityView);
                status = 0;
            }
        }
        else if(layer >= LAYER_CITY_ICON_START && layer < LAYER_CITY_ICON_START+m_cities.size() && status == 2)
        {
            city1 = layer - LAYER_CITY_ICON_START;
            status = 1;
            glbReleaseImage(&cityView);
        }
        else if(layer == LAYER_CITY_DETAIL)
        {
            glbReleaseImage(&cityView);
            status =0;
        }

    }
};
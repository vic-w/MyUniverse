#pragma once
#include "global.h"


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
		cityView = 0;
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
			glbDrawTexture(cityView, *m_pGlobeRotMat, m_pWindow->m_calib, p1, true, p2, false, true, 50, 8, LAYER_CITY_DETAIL, GLB_TEX_RECT);
		}
	}
	void reset()
	{
        m_bShowDetail = false;
		printf("citiview2 %d\n", cityView);
        glbReleaseImage(&cityView);
	}
    void onClick(int layer)
    {
        if(layer >= LAYER_CITY_ICON_START && layer < LAYER_CITY_ICON_START+m_cities.size() /*&& !m_bShowDetail*/)
        {
            m_nShowCity = layer - LAYER_CITY_ICON_START;

			char strLocalTime[64]; 
			sprintf(strLocalTime, "%s当地时间%s", m_cities[m_nShowCity].displayname,
                m_cities[m_nShowCity].getLocalTimeString());    
			if(txt2ImgHelper(4, strLocalTime))//2为模式
			{
				cityView = glbLoadImage("temp.png"); 
			}
			else
			{
				cityView = glbLoadImage("image\\error.png");
			}

            
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
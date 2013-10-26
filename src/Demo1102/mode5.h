#pragma once
#include "global.h"


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
			//kennyzx test
			char strWeather[32];
			sprintf(strWeather, "%s %s", 
				m_cities[m_nShowCity].weatherCondition, m_cities[m_nShowCity].temprature);
			if (txt2ImgHelper(m_cities[m_nShowCity].imgPath, 5, strWeather))//5为模式
			{
				cityView = glbLoadImage( "temp.jpg" );
			}
			else
			{
				cityView = glbLoadImage("error.png");
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

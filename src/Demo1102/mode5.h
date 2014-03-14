#pragma once
#include "global.h"
#include <shlobj.h>   
#include <stdio.h>
#include <string.h>
#pragma   comment(lib,   "shell32.lib")  

//mode5 ÌìÆø
class CMode5
{
public:
	GlbImage m_icon;
    GlbImage weather;
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
		weather = 0;
	}
    ~CMode5()
    {
        glbReleaseImage(&m_icon);
        glbReleaseImage(&weather);
    }
    void draw()
	{
		if(weather)
		{
			GlbPointGeo p1(0,0);
			GlbPointGeo p2(90,0);
			glbDrawTexture(weather, *m_pGlobeRotMat, m_pWindow->m_calib, p1, true, p2, true, true, 360, 180, LAYER_CITY_DETAIL, GLB_TEX_RECT);
		}
	}
	void reset()
	{
        m_bShowDetail = false;
        glbReleaseImage(&weather);
		char path[255];
		SHGetSpecialFolderPath(0,path,CSIDL_DESKTOPDIRECTORY,0);
		int len = strlen(path);
		weather = glbLoadImage(strncat(path, "\\weather.jpg", 14));
	}
    void onClick(int layer)
    {
 
    }
};

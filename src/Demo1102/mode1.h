#pragma once
#include "global.h"


//mode1 城市
class CMode1
{
	enum ENUM_MODE1_STAT
	{
		MODE1_NONE,
		MODE1_SHOWMENU,
		MODE1_SHOWDETAIL
	};
public:
	GlbImage m_icon;
    GlbImage cityView;
	GlbRotmat *m_pGlobeRotMat;
	GlbWindow *m_pWindow;
	vector<CCity> m_cities;
	//bool m_bShowDetail;

	ENUM_MODE1_STAT status;
	int m_nShowCity;//选择的城市编号
	int m_nSubClass;//选择的子目录编号
    int m_nImage;//选择的图像编号

	GlbImage icon_menu;
	GlbImage icon_close;
	GlbImage icon_frwd;
	GlbImage icon_back;

	CMenuIcon menu_icon;

public:
	CMode1(GlbRotmat *pGlobeRotMat, GlbWindow *pWindow)
	{
		m_cities = CCity::getCities();
		m_icon = glbLoadImage("image\\icon.png");
		m_pGlobeRotMat = pGlobeRotMat;
		m_pWindow = pWindow;
        //m_bShowDetail = false;
        m_nShowCity = 0;
		icon_menu = glbLoadImage("image\\city_menu.png");
		icon_close = glbLoadImage("image\\close.png");
		icon_frwd = glbLoadImage("image\\frwd.png");
		icon_back = glbLoadImage("image\\back.png");
		status = MODE1_NONE;
        m_nImage = 1;
	}
    ~CMode1()
    {
        glbReleaseImage(&m_icon);
        glbReleaseImage(&cityView);
    }
	void draw();
	void reset()
	{
		status = MODE1_NONE;
        //m_bShowDetail = false;
        glbReleaseImage(&cityView);
	}
    void onClick(int layer);

};
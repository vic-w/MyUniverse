#include "mode1.h"

void CMode1::draw()
{
	vector<CCity>::iterator it;
	int layer = LAYER_CITY_ICON_START;
	for(it=m_cities.begin(); it!=m_cities.end(); it++)
	{
		GlbPointGeo p1(it->latitude, it->longitude);
		GlbPointGeo p2(90,0);
		glbDrawTexture(m_icon, *m_pGlobeRotMat, m_pWindow->m_calib, p1, true, p2, true, true, 5, 5, layer++, GLB_TEX_RECT);
	}

	if(status == MODE1_SHOWMENU)
	{
		int nSubClasses = m_cities[m_nShowCity].images.size();
		float menu_angle1 = 360.0f/nSubClasses;
		printf("%d,%f\n", nSubClasses,  menu_angle1);
	}
	else if(status == MODE1_SHOWDETAIL)
	{
        GlbPointGeo p1(m_cities[m_nShowCity].latitude, m_cities[m_nShowCity].longitude);
		GlbPointGeo p2(90,0);
		glbDrawTexture(cityView, *m_pGlobeRotMat, m_pWindow->m_calib, p1, true, p2, false, true, 60, 45, LAYER_CITY_DETAIL, GLB_TEX_RECT);
			
		GlbPointGeo texPointGeo00,texPointGeo01,texPointGeo10,texPointGeo11;
		GlbPointTex texPoint(0.05,0.05);
		glbPointTex2PointGeo(*m_pGlobeRotMat, p1, true, p2, false, true, 60, 45, GLB_TEX_RECT, texPoint, false, texPointGeo00);
		texPoint.m_x = 0.95, texPoint.m_y = 0.05;
		glbPointTex2PointGeo(*m_pGlobeRotMat, p1, true, p2, false, true, 60, 45, GLB_TEX_RECT, texPoint, false, texPointGeo10);
		texPoint.m_x = 0.95, texPoint.m_y = 0.5;
		glbPointTex2PointGeo(*m_pGlobeRotMat, p1, true, p2, false, true, 60, 45, GLB_TEX_RECT, texPoint, false, texPointGeo11);
		texPoint.m_x = 0.05, texPoint.m_y = 0.5;
		glbPointTex2PointGeo(*m_pGlobeRotMat, p1, true, p2, false, true, 60, 45, GLB_TEX_RECT, texPoint, false, texPointGeo01);
		//画返回图标
        //glbDrawTexture(icon_menu, *m_pGlobeRotMat, m_pWindow->m_calib, texPointGeo00, false, texPointGeo01, false, true, 5, 5, LAYER_CITY_DETAIL_RETURN, GLB_TEX_RECT);
		//画关闭图标
        glbDrawTexture(icon_close, *m_pGlobeRotMat, m_pWindow->m_calib, texPointGeo10, false, texPointGeo11, false, true, 5, 5, LAYER_CITY_DETAIL_CLOSE, GLB_TEX_RECT);
		//画前进图标
        glbDrawTexture(icon_frwd, *m_pGlobeRotMat, m_pWindow->m_calib, texPointGeo11, false, texPointGeo10, false, true, 9, 9, LAYER_CITY_DETAIL_FORWARD, GLB_TEX_RECT);
		//画后退图标
        glbDrawTexture(icon_back, *m_pGlobeRotMat, m_pWindow->m_calib, texPointGeo01, false, texPointGeo00, false, true, 9, 9, LAYER_CITY_DETAIL_BACKWARD, GLB_TEX_RECT);
	}
}
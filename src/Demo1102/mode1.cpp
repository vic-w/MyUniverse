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
        GlbPoint3d N_polar3d(0,1,0);
        GlbPointGeo N_polarGeo(90,0);
        GlbPointGeo cityGeoGlobe(m_cities[m_nShowCity].latitude, m_cities[m_nShowCity].longitude);
        GlbPoint3d city3d;
        glbPointGeo2PointRect(cityGeoGlobe, city3d);
        glbGlobePoint2ScreenPoint(city3d, *m_pGlobeRotMat, city3d);//转成屏幕上的3d点
        
        GlbPivot to_polar_pivot;
        glbPivotBetweenPoints(city3d, N_polar3d, to_polar_pivot);

        GlbPoint3d menu13d;
        glbPivotingPoint(city3d, to_polar_pivot, 10, menu13d);

        for(int i=0; i<nSubClasses; i++)
        {
            GlbPoint3d menu_pos3d;
            glbPivotingPoint(menu13d, city3d, menu_angle1*i, menu_pos3d);
            GlbPointGeo menu_posGeo;
            glbPointRect2PointGeo(menu_pos3d, menu_posGeo);
            glbDrawTexture(menu_icon.getImage(m_cities[m_nShowCity].images[i][0]), *m_pGlobeRotMat, m_pWindow->m_calib, menu_posGeo, false, N_polarGeo, false, true, 10, 5, LAYER_SUB_MENU_START+i, GLB_TEX_RECT);
        }

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

        //画关闭图标
        glbDrawTexture(icon_close, *m_pGlobeRotMat, m_pWindow->m_calib, texPointGeo10, false, texPointGeo11, false, true, 5, 5, LAYER_CITY_DETAIL_CLOSE, GLB_TEX_RECT);
		//画前进图标
        glbDrawTexture(icon_frwd, *m_pGlobeRotMat, m_pWindow->m_calib, texPointGeo11, false, texPointGeo10, false, true, 9, 9, LAYER_CITY_DETAIL_FORWARD, GLB_TEX_RECT);
		//画后退图标
        glbDrawTexture(icon_back, *m_pGlobeRotMat, m_pWindow->m_calib, texPointGeo01, false, texPointGeo00, false, true, 9, 9, LAYER_CITY_DETAIL_BACKWARD, GLB_TEX_RECT);
	}
}

void CMode1::onClick(int layer)
{
    if(layer >= LAYER_CITY_ICON_START && layer < LAYER_CITY_ICON_START+m_cities.size())
    {
        m_nShowCity = layer - LAYER_CITY_ICON_START;
        if(status == MODE1_NONE)
        {
		    status = MODE1_SHOWMENU;
        }
        else if(status == MODE1_SHOWMENU)
        {
            if(m_nSubClass == layer - LAYER_SUB_MENU_START)
            {
                status = MODE1_NONE;
            }
        }
        else if(status == MODE1_SHOWDETAIL)
        {
            status = MODE1_SHOWMENU;
            glbReleaseImage(&cityView);
        }
    }
    else if(layer >= LAYER_SUB_MENU_START && layer < LAYER_SUB_MENU_START + m_cities[m_nShowCity].images.size() && status == MODE1_SHOWMENU)
    {
        status = MODE1_SHOWDETAIL;
        m_nSubClass = layer - LAYER_SUB_MENU_START;
        m_nImage = 1;
        cityView = glbLoadImage(m_cities[m_nShowCity].images[m_nSubClass][m_nImage]);
    }
    else if(layer == LAYER_CITY_DETAIL_CLOSE)
    {
		status = MODE1_SHOWMENU;
        //m_bShowDetail = false;
        glbReleaseImage(&cityView);
    }
    else if(layer == LAYER_CITY_DETAIL_FORWARD)
    {
        m_nImage ++;
        if( m_nImage >= m_cities[m_nShowCity].images[m_nSubClass].size() )
        {
            m_nImage = 1;
        }
        glbReleaseImage(&cityView);
        cityView = glbLoadImage(m_cities[m_nShowCity].images[m_nSubClass][m_nImage]);
    }    
    else if(layer == LAYER_CITY_DETAIL_BACKWARD)
    {
        m_nImage --;
        if( m_nImage <= 1 )
        {
            m_nImage = m_cities[m_nShowCity].images[m_nSubClass].size()-1;
        }
        glbReleaseImage(&cityView);
        cityView = glbLoadImage(m_cities[m_nShowCity].images[m_nSubClass][m_nImage]);
    }
}
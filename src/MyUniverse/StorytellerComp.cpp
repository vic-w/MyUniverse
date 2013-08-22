#include "StdAfx.h"
#include "StorytellerComp.h"

void StorytellerEular2GlobeEular(GlbEularAngle storytellerEular, GlbEularAngle offsetEular, GlbEularAngle &globeEular)
{
    globeEular.m_1_Horz = storytellerEular.m_1_Horz + offsetEular.m_1_Horz;
    globeEular.m_2_Vert = storytellerEular.m_2_Vert + offsetEular.m_2_Vert;
    globeEular.m_3_Axis = storytellerEular.m_3_Axis + offsetEular.m_3_Axis;

    if(globeEular.m_1_Horz > 360)
    {
        globeEular.m_1_Horz -= 360;
    }
    else if(globeEular.m_1_Horz < 0)
    {
        globeEular.m_1_Horz += 360;
    }

    if(globeEular.m_2_Vert > 360)
    {
        globeEular.m_2_Vert -= 360;
    }
    else if(globeEular.m_2_Vert < 0)
    {
        globeEular.m_2_Vert += 360;
    }

    if(globeEular.m_3_Axis > 360)
    {
        globeEular.m_3_Axis -= 360;
    }
    else if(globeEular.m_3_Axis < 0)
    {
        globeEular.m_3_Axis += 360;
    }
}

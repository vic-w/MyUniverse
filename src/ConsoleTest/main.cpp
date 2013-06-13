#include "GlbCore.h"

GlbRotmat g_GlobeRotMat;
GlbEularAngle g_GlobeEularAngle;

void main()
{
    glbCreateWindow(0);
    glbInitDistort();

    g_GlobeEularAngle.m_1_Horz = 0;
    g_GlobeEularAngle.m_2_Vert = 30;
    g_GlobeEularAngle.m_3_Axis = 0;


    GlbImage Image = glbLoadImage("a001.dds");
    //GlbImage Image = glbLoadImage("earth.jpg");

    do
    {
        glbClearWindow();
        DrawGlobe(Image, g_GlobeRotMat);
        g_GlobeEularAngle.m_3_Axis += 0.2f;
        if(g_GlobeEularAngle.m_3_Axis>360)
        {
            g_GlobeEularAngle.m_3_Axis -= 360;
        }
        glbEularAngle2Rotmat(g_GlobeEularAngle, g_GlobeRotMat);
    }while(glbUpdateWindow(30));

    glbReleaseImage(&Image);
    glbDestoryWindow(0);
    return;
}

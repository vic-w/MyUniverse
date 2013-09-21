#include "GlbCore.h"

GlbRotmat g_GlobeRotMat;
GlbEularAngle g_GlobeEularAngle;

void main()
{
    GlbRect size(0,0,500,500);
    GlbWindow mainWindow;
    glbCreateWindow(mainWindow, size, ".\\calibmain.ini", true, false);

    g_GlobeEularAngle.m_1_Horz = 0;
    g_GlobeEularAngle.m_2_Vert = 30;
    g_GlobeEularAngle.m_3_Axis = 0;

    glbSwitchWindow(mainWindow);
    GlbImage Image = glbLoadImage("earth.jpg");    
    GlbImage Image2 = glbLoadImage("a001.dds");    
    
    do
    {
        glbClearWindow();
         g_GlobeEularAngle.m_3_Axis += 0.2f;
        if(g_GlobeEularAngle.m_3_Axis>360)
        {
            g_GlobeEularAngle.m_3_Axis -= 360;
        }
        glbEularAngle2Rotmat(g_GlobeEularAngle, g_GlobeRotMat);
        glbDrawGlobe(Image, g_GlobeRotMat, mainWindow.m_calib);
        GlbPointGeo p1(40,120);
        GlbPointGeo p2(90,120);
        GlbPoint3d p3;

        glbDrawTexture(Image2, g_GlobeRotMat, mainWindow.m_calib, 
            p1, true, p2, false, true, 180, 30, 1, GLB_TEX_BELT, p3);
        glbDrawLineOnGlobe(p1, g_GlobeRotMat, mainWindow.m_calib, p2, 2);

    }
    while(glbUpdateWindow(mainWindow,30));

    glbReleaseImage(&Image);
    glbDestoryWindow(mainWindow);
    return;
}

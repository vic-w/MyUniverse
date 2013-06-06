#include "stdafx.h"
#include "GlobeThread.h"

CStoryPage g_StoryPage;

DWORD WINAPI GlobeThread(LPVOID lpParam)
{
    glbCreateWindow(0);

    //GlbImage Image = glbLoadImage("a001.dds");
    //GlbImage Image = glbLoadImage("earth.jpg");

    do
    {
        glbClearWindow();
        DrawStoryPage();
        //glbDrawImage(Image);
        //DrawGlobe(Image);
    }while(glbUpdateWindow(30));

    //glbReleaseImage(&Image);
    glbDestoryWindow(0);
    return 1;
}

void DrawStoryPage()
{
    if(!g_StoryPage.bEmpty)
    {
        if(     g_StoryPage.storyType == DDS 
            ||  g_StoryPage.storyType == JPG
            ||  g_StoryPage.storyType == PNG )
        {
            GlbImage Image = glbLoadImage(g_StoryPage.pagePath);
            DrawGlobe(Image);
            glbReleaseImage(&Image);
        }
        else
        {
        }
    }
}
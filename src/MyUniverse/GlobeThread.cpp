#include "stdafx.h"
#include "GlobeThread.h"

CRITICAL_SECTION g_StoryPage_CS;
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
    EnterCriticalSection(&g_StoryPage_CS);
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
        else if(g_StoryPage.storyType == FOLDER) 
        {
            GlbImage Image = glbLoadImage(g_StoryPage.FrameNames[g_StoryPage.nCurFrame]);
            DrawGlobe(Image);
            glbReleaseImage(&Image);
            g_StoryPage.nCurFrame = (g_StoryPage.nCurFrame+1)%g_StoryPage.nFrames;
        }
        else
        {
            //do nothing
        }
    }
    LeaveCriticalSection(&g_StoryPage_CS);
}
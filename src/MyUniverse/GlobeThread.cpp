#include "stdafx.h"
#include "GlobeThread.h"

CRITICAL_SECTION g_StoryPage_CS;
CStoryPage g_StoryPage;

CRITICAL_SECTION g_GlobeRotMat_CS;
GlbRotmat g_GlobeRotMat;

extern int g_bMainThreadActive;//主线程活动指示
extern int g_bGlbThreadActive;//星球绘图线程活动指示

DWORD WINAPI GlobeThread(LPVOID lpParam)
{
    g_bGlbThreadActive = 1;

    glbCreateWindow(0);

    do
    {
        glbClearWindow();
        DrawStoryPage();
        glbUpdateWindow(0);
    }while(g_bMainThreadActive);

    glbDestoryWindow(0);
    g_bGlbThreadActive = 0;
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
            EnterCriticalSection(&g_GlobeRotMat_CS);
            DrawGlobe(Image, g_GlobeRotMat);
            LeaveCriticalSection(&g_GlobeRotMat_CS);
            glbReleaseImage(&Image);
        }
        else if(g_StoryPage.storyType == FOLDER) 
        {
            GlbImage Image = glbLoadImage(g_StoryPage.FrameNames[g_StoryPage.nCurFrame]);
            EnterCriticalSection(&g_GlobeRotMat_CS);
            DrawGlobe(Image, g_GlobeRotMat);
            LeaveCriticalSection(&g_GlobeRotMat_CS);
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
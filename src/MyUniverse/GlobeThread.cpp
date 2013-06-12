#include "stdafx.h"
#include "GlobeThread.h"
#include "MyUniverseDlg.h"

CRITICAL_SECTION g_StoryPage_CS;
CStoryPage g_StoryPage;

CRITICAL_SECTION g_GlobeRotMat_CS;
GlbRotmat g_GlobeRotMat;
GlbEularAngle g_GlobeEularAngle;

extern int g_bMainThreadActive;//主线程活动指示
extern int g_bGlbThreadActive;//星球绘图线程活动指示
extern int g_bTimingThreadActive;//星球绘图线程活动指示

DWORD WINAPI GlobeThread(LPVOID lpParam)
{
    g_bGlbThreadActive = 1;

    glbInitDistort();
    glbCreateWindow(0);

    do
    {
        glbClearWindow();
        DrawStoryPage();
        glbUpdateWindow(10);
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
            static CString lastPath = "";
            static GlbImage Image = 0;

            if(strcmp(lastPath, g_StoryPage.pagePath))
            {
                if(Image)
                {
                    glbReleaseImage(&Image);
                }
                Image = glbLoadImage(g_StoryPage.pagePath);
            }
            EnterCriticalSection(&g_GlobeRotMat_CS);
            DrawGlobe(Image, g_GlobeRotMat);
            LeaveCriticalSection(&g_GlobeRotMat_CS);
            lastPath = g_StoryPage.pagePath;

        }
        else if(g_StoryPage.storyType == FOLDER) 
        {
            if(g_StoryPage.FrameNames.size()>0)
            {
                GlbImage Image = glbLoadImage(g_StoryPage.FrameNames[g_StoryPage.nCurFrame]);
                EnterCriticalSection(&g_GlobeRotMat_CS);
                DrawGlobe(Image, g_GlobeRotMat);
                LeaveCriticalSection(&g_GlobeRotMat_CS);
                glbReleaseImage(&Image);

                //删去此句，放在TimingThread线程中
                //g_StoryPage.nCurFrame = (g_StoryPage.nCurFrame+g_StoryPage.bPlaying)%g_StoryPage.nFrames;
            }
        }
        else
        {
            //do nothing
        }
    }
    LeaveCriticalSection(&g_StoryPage_CS);
}

DWORD WINAPI TimingThread(LPVOID lpParam)
{
    g_bTimingThreadActive = 1;
    LARGE_INTEGER tFreq={0};
	LARGE_INTEGER LastTimeCounter_playing;
	LARGE_INTEGER LastTimeCounter_rotating;
	LARGE_INTEGER TimeCounter;
	QueryPerformanceFrequency(&tFreq); 
	QueryPerformanceCounter(&LastTimeCounter_playing);
	QueryPerformanceCounter(&LastTimeCounter_rotating);

    while(g_bMainThreadActive)
    {
        QueryPerformanceCounter(&TimeCounter);

        //更新动画
        if(g_StoryPage.bPlaying && g_StoryPage.frameRate>0)
        {
            int frameRate = g_StoryPage.frameRate;
            if( frameRate<1)
            {
                frameRate = 1;
            }

	        if( TimeCounter.QuadPart - LastTimeCounter_playing.QuadPart > tFreq.QuadPart/frameRate)
	        {
		        LastTimeCounter_playing = TimeCounter;
		        g_StoryPage.nCurFrame = (g_StoryPage.nCurFrame+g_StoryPage.bPlaying)%g_StoryPage.nFrames;
	        }
        }
        else
        {
            LastTimeCounter_playing = TimeCounter;
        }

        //更新自转
        if(g_StoryPage.bRotating)
        {
	        if( TimeCounter.QuadPart - LastTimeCounter_rotating.QuadPart > tFreq.QuadPart/30)
	        {
		        LastTimeCounter_rotating = TimeCounter;

                EnterCriticalSection(&g_GlobeRotMat_CS);
                g_GlobeEularAngle.m_3_Axis += 0.2f;
                if(g_GlobeEularAngle.m_3_Axis>360)
                {
                    g_GlobeEularAngle.m_3_Axis -= 360;
                }
                glbEularAngle2Rotmat(g_GlobeEularAngle, g_GlobeRotMat);
                LeaveCriticalSection(&g_GlobeRotMat_CS);

                if(g_bMainThreadActive)
                {
                    CMyUniverseDlg* pDlg = (CMyUniverseDlg*)lpParam;
                    pDlg->m_edit_rotz = (int)g_GlobeEularAngle.m_3_Axis;
                    //pDlg->m_slider_rotz = (int)(pDlg->m_edit_rotz/360.0*100);
                    pDlg->m_slider_rotz_ctrl.SetPos((int)(g_GlobeEularAngle.m_3_Axis/360.0*100));
                }
            }
        }
        else
        {
            LastTimeCounter_rotating = TimeCounter;
        }
        Sleep(1);
    }
    g_bTimingThreadActive = 0;
    return 1;
}

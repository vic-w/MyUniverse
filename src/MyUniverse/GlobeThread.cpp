#include "stdafx.h"
#include "GlobeThread.h"
#include "MyUniverseDlg.h"

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
            GlbImage Image = glbLoadImage(g_StoryPage.pagePath);
            EnterCriticalSection(&g_GlobeRotMat_CS);
            DrawGlobe(Image, g_GlobeRotMat);
            LeaveCriticalSection(&g_GlobeRotMat_CS);
            glbReleaseImage(&Image);
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
    LARGE_INTEGER tFreq={0};
	LARGE_INTEGER LastTimeCounter_playing;
	LARGE_INTEGER LastTimeCounter_rotating;
	LARGE_INTEGER TimeCounter;
	QueryPerformanceFrequency(&tFreq); 
	QueryPerformanceCounter(&LastTimeCounter_playing);
	QueryPerformanceCounter(&LastTimeCounter_rotating);

    while(1)
    {
        QueryPerformanceCounter(&TimeCounter);

        //更新动画
        if(g_StoryPage.bPlaying)
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

                CMyUniverseDlg* pDlg = (CMyUniverseDlg*)lpParam;
                GlbEularAngle angle;
                angle.m_1_Horz = (float)pDlg->m_edit_rotx;
                angle.m_2_Vert = (float)pDlg->m_edit_roty;
                angle.m_3_Axis = (float)pDlg->m_edit_rotz;
                angle.m_3_Axis += 2.0f;
                angle.m_3_Axis = (float)((int)(angle.m_3_Axis) % 360);

                pDlg->m_edit_rotz = (int)angle.m_3_Axis;
                //pDlg->m_slider_rotz = (int)(pDlg->m_edit_rotz/360.0*100);
                pDlg->m_slider_rotz_ctrl.SetPos((int)(pDlg->m_edit_rotz/360.0*100));


                EnterCriticalSection(&g_GlobeRotMat_CS);
                glbEularAngle2Rotmat(angle, g_GlobeRotMat);
                LeaveCriticalSection(&g_GlobeRotMat_CS);
	        }
        }
        else
        {
            LastTimeCounter_rotating = TimeCounter;
        }
        Sleep(10);
    }
}

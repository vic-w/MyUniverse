#pragma once 
#include "GlbRot.h"
#include <vector>
#include <list> 
#include <Windows.h>

using namespace std;


class GlbRect
{
public:
    long m_left;
    long m_top;
    long m_width;
    long m_height;
    GlbRect(){}
    GlbRect(long left, long top, long width, long height)
    {
        m_left = left;
        m_top = top;
        m_width = width;
        m_height = height;
    }
};

class GlbMove
{
public:
	GlbPoint2d m_pFrom;
	GlbPoint2d m_pTo;
	long m_nTrackID;
};

class GlbWindow
{
public:
    HWND	m_hWnd;
    HDC		m_hDC;
    HGLRC	m_hRC;
	int		m_width;
	int		m_height;
    GlbCalib m_calib;
	int     m_TuioPort;

	CRITICAL_SECTION m_touch_cs; //´¥ÃþÐÅºÅ»¥³âÁÙ½çÇø
	vector<GlbPoint2d> m_touchSignal;
	vector<GlbMove> m_moveSignal;
public:
	GlbWindow()
	{
		InitializeCriticalSection(&m_touch_cs);
	}
	~GlbWindow()
	{
		//DeleteCriticalSection(&m_touch_cs);
	}
	void EnterTouchCS()
	{
		EnterCriticalSection(&m_touch_cs);
	}
	void LeaveTouchCS()
	{
		LeaveCriticalSection(&m_touch_cs);
	}
};
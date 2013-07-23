

#pragma once
#include "afxwin.h"


#include <gl.h>
#include <glu.h>

class COpenGL:public CWnd
{
public:
    UINT_PTR m_unpTimer;
    CWnd    *hWnd;
    HDC     hdc;
    HGLRC   hrc;
private:
    int     m_nPixelFormat;
    CRect   m_rect;
    CRect   m_oldWindow;
    CRect   m_originalRect;
public:
    COpenGL();
    ~COpenGL();
    void oglCreate(CRect rect, CWnd *parent);
    void oglInitialize(void);
    void oglDrawScene(void);

    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDraw(CDC *pDC);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnSize(UINT nType, int cx, int cy);
};



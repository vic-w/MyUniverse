#include "stdafx.h"
#include "OpenGLinMFC.h"
#include <afxwin.h>
#include <gl.h>
#include <glu.h>

void CreateWindowFromMFC(CRect rect, CWnd* parent)
{
    HWND parentHWnd = parent->m_hWnd;
    CString className = AfxRegisterWndClass(CS_HREDRAW |
        CS_VREDRAW | CS_OWNDC, NULL,
        (HBRUSH)GetStockObject(BLACK_BRUSH), NULL);

    HWND hWnd = CreateWindowEx(
        0, 
        className, 
        "OpenGL", 
        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
        rect.left,
        rect.top,
        rect.Width(),
        rect.Height(),
        parentHWnd,
        0,0,0);

    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,    // bit depth
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        16,    // z-buffer depth
        0, 0, 0, 0, 0, 0, 0,
    };

    // Get device context only once.
    HDC hDC = GetDC( hWnd );

    // Pixel format.
    int PixelFormat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, PixelFormat, &pfd);

    // Create the OpenGL Rendering Context.
    HGLRC hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);

    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);							
    //glEnable(GL_DEPTH_TEST);					
    glDepthFunc(GL_LEQUAL);	
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


    for(int i=0; i<1000; i++)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        //***************************
        // DRAWING CODE
        //***************************

        glBegin(GL_TRIANGLES);
        glColor3f(1.0f,0.0f,0.0f);
        glVertex3f(1.0f,-1.0f,0.0f);
        glColor3f(0.0f,1.0f,0.0f);
        glVertex3f(-1.0f,-1.0f,0.0f);
        glColor3f(0.0f,0.0f,1.0f);
        glVertex3f(0.0f,1.0f,0.0f);
        glEnd();
 
        SwapBuffers(hDC);

        MSG uMsg;
        memset(&uMsg,0,sizeof(uMsg));

        if( PeekMessage( &uMsg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &uMsg );
            DispatchMessage( &uMsg );
        }
        Sleep(1);
    }

    if( hRC != NULL )
    {
        wglMakeCurrent( NULL, NULL );
        wglDeleteContext( hRC );
        hRC = NULL;
    }

    if( hDC != NULL )
    {
        ReleaseDC( hWnd, hDC );
        hDC = NULL;
    }

    UnregisterClass( className, 0 );
}


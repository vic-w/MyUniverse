#include "StdAfx.h"
#include "OpenGL.h"


COpenGL::COpenGL()
{
}


COpenGL::~COpenGL()
{
}

void COpenGL::oglCreate(CRect rect, CWnd *parent)
{
   CString className = AfxRegisterWndClass(CS_HREDRAW |
      CS_VREDRAW | CS_OWNDC, NULL,
      (HBRUSH)GetStockObject(BLACK_BRUSH), NULL);
 
   CreateEx(0, className, "OpenGL", WS_CHILD | WS_VISIBLE |
            WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, parent, 0);

   oglInitialize();
}

BEGIN_MESSAGE_MAP(COpenGL, CWnd)
    ON_WM_TIMER()
END_MESSAGE_MAP()

int COpenGL::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    oglInitialize();

    return 0;
}

void COpenGL::oglInitialize(void)
{
   // Initial Setup:
   //
   static PIXELFORMATDESCRIPTOR pfd =
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
   hdc = GetDC()->m_hDC;
   //CClientDC *dc = new CClientDC(this);
   //hdc = dc->m_hDC;
 
   // Pixel format.
   m_nPixelFormat = ChoosePixelFormat(hdc, &pfd);
   SetPixelFormat(hdc, m_nPixelFormat, &pfd);
 
   // Create the OpenGL Rendering Context.
   hrc = wglCreateContext(hdc);
   wglMakeCurrent(hdc, hrc);
 
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);							
	//glEnable(GL_DEPTH_TEST);					
	glDepthFunc(GL_LEQUAL);	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void COpenGL::OnDraw(CDC *pDC)
{
   // TODO: Camera controls.
}

void COpenGL::OnTimer(UINT_PTR nIDEvent)
{
   switch (nIDEvent)
   {
      case 1:
      {
         // Clear color and depth buffer bits
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
         // Draw OpenGL scene
         oglDrawScene();
 
         // Swap buffers
         SwapBuffers(hdc);
 
         break;
      }
 
      default:
         break;
   }
 
    CWnd::OnTimer(nIDEvent);
}





void COpenGL::oglDrawScene(void)
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

}
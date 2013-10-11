#include "stdafx.h"
//#include <windows.h>
#include "GlbCore.h"
#include "GlbType.h"
#include <stdio.h>
#include <malloc.h>
#include "opencv.hpp"

#ifndef UseOpenGLExtension2LoadDDS
#include "IL/il.h"
#else 
DDS_IMAGE_DATA* glbLoadDDSFile(const char* filename);
PFNGLCOMPRESSEDTEXIMAGE2DARBPROC glCompressedTexImage2DARB;
#endif

//HWND   g_hWnd                = NULL;
//HDC    g_hDC                 = NULL;
//HGLRC  g_hRC                 = NULL;


//GlbRotmat g_GlobeRotMat;

#define FACET_SCACLE_IN_ANGLE (6) //每个面片占的最小角度
#define MAX_FACET_SHOW_THRESHOLD (0.4f)//能显示的最大面片的size

class HWND_GLBWINDOW_PAIR //每个HWND对应一个GlbWindow
{
public:
	HWND hWnd;
	GlbWindow* window;
};

vector<HWND_GLBWINDOW_PAIR> g_HWND_GLBWINDOW_PAIR; //此全局变量是用来在WindowProc函数中找到hWnd对应的GlbWindow

int glbAddHwndGlbWindowPair(HWND hWnd, GlbWindow *window);
int glbDeleteHwndGlbWindowPair(GlbWindow *window);
bool glbGetWindowPtr(const HWND hWnd, GlbWindow **pWindow);


GlbImage glbLoadImageFromOpencv(IplImage* pImage, bool bMipmap)
{
    GLuint TextureID = -1;
    int sizeX,sizeY;
    sizeX = pImage->width;
    sizeY = pImage->height;

    float texAspectRatio = (float)sizeX / (float)sizeY;

    // Generate a texture with the associative texture ID stored in the array
    glGenTextures(1, &TextureID);

    // This sets the alignment requirements for the start of each pixel row in memory.
    // glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

    // Bind the texture to the texture arrays index and init the texture
    glBindTexture(GL_TEXTURE_2D, TextureID);

    //Assign the mip map levels and texture info
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    // Build Mipmaps (builds different versions of the picture for distances - looks better)

    if(bMipmap)
    {
		if(pImage->nChannels == 4)
		{
			gluBuild2DMipmaps(GL_TEXTURE_2D, 4, sizeX, sizeY, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pImage->imageData);
		}
		else
		{
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, sizeX, sizeY, GL_BGR_EXT, GL_UNSIGNED_BYTE, pImage->imageData);
		}
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sizeX, sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE,NULL );
	    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, sizeX, sizeY, GL_BGR_EXT, GL_UNSIGNED_BYTE, pImage->imageData);
    }
    //glEnable(GL_TEXTURE_2D);
    // Now we need to free the image data that we loaded since openGL stored it as a texture

    return TextureID;
}

BOOL bInit = FALSE;
GlbImage glbLoadImage(const char* filename)  //载入图像（支持dds,jpg,bmp,png）
{
    int fileNameLen = strlen(filename);
    const char* suffix = filename + fileNameLen - 4; //Bug: break with ".jpeg"!
    //printf("filename = %s, suffix = %s\n", filename, suffix);

    if(_stricmp(suffix,".dds") == 0)
    {

#ifdef UseOpenGLExtension2LoadDDS

        GLuint TextureID = -1;
        DDS_IMAGE_DATA *pDDSImageData = glbLoadDDSFile(filename);

        if( pDDSImageData != NULL )
        {
            int nHeight     = pDDSImageData->height;
            int nWidth      = pDDSImageData->width;
            int nNumMipMaps = pDDSImageData->numMipMaps;

            int nBlockSize;

            if( pDDSImageData->format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT )
                nBlockSize = 8;
            else
                nBlockSize = 16;

            glGenTextures( 1, &TextureID );
            glBindTexture( GL_TEXTURE_2D, TextureID );

            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

            int nSize;
            int nOffset = 0;

            // Load the mip-map levels

            for( int i = 0; i < nNumMipMaps; ++i )
            {
                if( nWidth  == 0 ) nWidth  = 1;
                if( nHeight == 0 ) nHeight = 1;

                nSize = ((nWidth+3)/4) * ((nHeight+3)/4) * nBlockSize;
				assert(NULL != glCompressedTexImage2DARB);
                glCompressedTexImage2DARB( GL_TEXTURE_2D,
                    i,
                    pDDSImageData->format,
                    nWidth,
                    nHeight,
                    0,
                    nSize,
                    pDDSImageData->pixels + nOffset );
                int error = glGetError();
                if(error)
                {
                    //GL_INVALID_ENUM;
                    char str[255];
                    sprintf_s( str, 255, "error code = 0x%08X", error );
                    MessageBox( NULL, str, "ERROR", MB_OK|MB_ICONEXCLAMATION );
                }

                nOffset += nSize;

                // Half the image size for the next mip-map level...
                nWidth  = (nWidth  / 2);
                nHeight = (nHeight / 2);
            }
        }
        else
        {
            MessageBox(0, "读取DDS图像错误！", NULL, MB_OK);
            return NULL;
        }
        if( pDDSImageData != NULL )
        {
            if( pDDSImageData->pixels != NULL )
                free( pDDSImageData->pixels );

            free( pDDSImageData );
        }
        return TextureID;

#else
		//Devil begin
		if (!bInit)
		{
			ilInit();
			ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
			ilEnable(IL_ORIGIN_SET);
			bInit = TRUE;
		}

		ILuint imageID;
		ilGenImages(1, &imageID);
		ilBindImage(imageID);

		ilLoadImage(filename);
		ilActiveMipmap(1);

		int const width  = ilGetInteger(IL_IMAGE_WIDTH);
		int const height = ilGetInteger(IL_IMAGE_HEIGHT);
		//int const type   = ilGetInteger(IL_IMAGE_TYPE); // matches OpenGL
		//int const format = ilGetInteger(IL_IMAGE_FORMAT); // matches OpenGL

		unsigned int size = width * height * 4;
		ILubyte* data = new unsigned char[size];
		ilCopyPixels(0, 0, 0, width, height,
		1, IL_BGRA, IL_UNSIGNED_BYTE, data);

		ilDeleteImage(imageID);

		GLuint textureID = 0;
		// Generate one new texture Id.
		glGenTextures(1,&textureID);
		// Make this texture the active one, so that each
		// subsequent glTex* calls will affect it.
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Specify a linear filter for both the minification and
		// magnification.

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// Finally, generate the texture data in OpenGL.
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		
		delete[] data;

		return textureID;
		//Devil end
#endif

    }
    else if(_stricmp(suffix,".jpg") == 0 
        || _stricmp(suffix,".bmp") == 0 
        || _stricmp(suffix,".png") == 0)
    {
        GLuint TextureID = -1;
        IplImage *pImage = cvLoadImage(filename, CV_LOAD_IMAGE_UNCHANGED);//只有加了UNCHANGED参数才能读到png文件的alpha通道，否则会被强制转换为RGB图像

        if(!pImage)
        {
            MessageBox(0, "读取图像错误！", NULL, MB_OK);
            return false;
        }
        TextureID = glbLoadImageFromOpencv(pImage);

        cvReleaseImage(&pImage);

        return TextureID;
    }
    else
    {
        MessageBox(0, "图像格式不支持！", NULL, MB_OK);
        return 0;
    }
}

void glbReleaseImage(GlbImage* pImage)            //释放图像
{
    glDeleteTextures( 1, pImage );
}


DDS_IMAGE_DATA* glbLoadDDSFile( const char *filename )
{
    DDS_IMAGE_DATA *pDDSImageData;
    DDSURFACEDESC2 ddsd;
    char filecode[4];
    FILE *pFile;
    int factor;
    int bufferSize;

    // Open the file
    fopen_s(&pFile, filename, "rb" );

    if( pFile == NULL )
    {
        char str[255];
        sprintf_s( str, 255, "loadDDSTextureFile couldn't find, or failed to load \"%s\"", filename );
        MessageBox( NULL, str, "ERROR", MB_OK|MB_ICONEXCLAMATION );
        return NULL;
    }

    // Verify the file is a true .dds file
    fread( filecode, 1, 4, pFile );

    if( strncmp( filecode, "DDS ", 4 ) != 0 )
    {
        char str[255];
        sprintf_s( str, 255, "The file \"%s\" doesn't appear to be a valid .dds file!", filename );
        //MessageBox( NULL, str, "ERROR", MB_OK|MB_ICONEXCLAMATION );
        fclose( pFile );
        return NULL;
    }

    // Get the surface descriptor
    fread( &ddsd, sizeof(ddsd), 1, pFile );

    pDDSImageData = (DDS_IMAGE_DATA*) malloc(sizeof(DDS_IMAGE_DATA));

    memset( pDDSImageData, 0, sizeof(DDS_IMAGE_DATA) );

    //
    // This .dds loader supports the loading of compressed formats DXT1, DXT3 
    // and DXT5.
    //

    switch( ddsd.ddpfPixelFormat.dwFourCC )
    {
    case FOURCC_DXT1:
        // DXT1's compression ratio is 8:1
        pDDSImageData->format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        factor = 2;
        break;

    case FOURCC_DXT3:
        // DXT3's compression ratio is 4:1
        pDDSImageData->format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        factor = 4;
        break;

    case FOURCC_DXT5:
        // DXT5's compression ratio is 4:1
        pDDSImageData->format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        factor = 4;
        break;

    default:
        char str[255];
        sprintf_s( str, 255, "The file \"%s\" doesn't appear to be compressed "
            "using DXT1, DXT3, or DXT5!", filename );
        MessageBox( NULL, str, "ERROR", MB_OK|MB_ICONEXCLAMATION );
        return NULL;
    }

    //
    // How big will the buffer need to be to load all of the pixel data 
    // including mip-maps?
    //

    if( ddsd.dwLinearSize == 0 )
    {
        MessageBox( NULL, "dwLinearSize is 0!","ERROR",
            MB_OK|MB_ICONEXCLAMATION);
    }

    if( ddsd.dwMipMapCount > 1 )
        bufferSize = ddsd.dwLinearSize * factor;
    else
        bufferSize = ddsd.dwLinearSize;

    pDDSImageData->pixels = (unsigned char*)malloc(bufferSize * sizeof(unsigned char));

    fread( pDDSImageData->pixels, 1, bufferSize, pFile );

    // Close the file
    fclose( pFile );

    pDDSImageData->width      = ddsd.dwWidth;
    pDDSImageData->height     = ddsd.dwHeight;
    pDDSImageData->numMipMaps = ddsd.dwMipMapCount;

    if( ddsd.ddpfPixelFormat.dwFourCC == FOURCC_DXT1 )
        pDDSImageData->components = 3;
    else
        pDDSImageData->components = 4;

    return pDDSImageData;
}

//*
LRESULT CALLBACK WinProc( HWND   hWnd, 
    UINT   msg, 
    WPARAM wParam, 
    LPARAM lParam )
{
    static POINT ptLastMousePosit;
    static POINT ptCurrentMousePosit;
    static POINT HitPoint;
    static bool bMousing;

	GlbWindow *pWindow;
	glbGetWindowPtr(hWnd, &pWindow);//从查找表中取得pWindow的值

    switch( msg )
    {
    case WM_KEYDOWN:
        {
            switch( wParam )
            {
            case VK_ESCAPE:
                PostQuitMessage(0);
                break;

            case VK_F1:
                //g_bUseCompressedTexture = !g_bUseCompressedTexture;
                break;
            }
        }
        break;

    case WM_LBUTTONDOWN:
        {
            HitPoint.x = ptLastMousePosit.x = ptCurrentMousePosit.x = LOWORD (lParam);
            HitPoint.y = ptLastMousePosit.y = ptCurrentMousePosit.y = HIWORD (lParam);
			
            bMousing = true;
			//printf("L_button_down %X\n", pWindow);
        }
        break;

    case WM_LBUTTONUP:
        {
            bMousing = false;
			ptCurrentMousePosit.x = LOWORD (lParam);
            ptCurrentMousePosit.y = HIWORD (lParam);
			if( (abs(HitPoint.x - ptCurrentMousePosit.x)<=1) && (abs(HitPoint.y - ptCurrentMousePosit.y)<=1) )
			{
				//printf("click\n");
				float radius = (float)pWindow->m_height/2;
				GlbPoint2d point;
				point.m_x = (ptLastMousePosit.x - pWindow->m_width/2 )/radius;
				point.m_y = (pWindow->m_height/2 - ptLastMousePosit.y)/radius;
								
				pWindow->EnterTouchCS();

				pWindow->m_touchSignal.push_back(point);

				pWindow->LeaveTouchCS();
			}
			//printf("L_button_up %X\n", pWindow);
        }
        break;

    case WM_MOUSEMOVE:
        {
            ptCurrentMousePosit.x = LOWORD (lParam);
            ptCurrentMousePosit.y = HIWORD (lParam);

            if( bMousing )
            {
				//printf("mouse_move from (%d,%d) to (%d,%d) %X\n", ptLastMousePosit.x, ptLastMousePosit.y, ptCurrentMousePosit.x, ptCurrentMousePosit.y, pWindow);
				GlbMove move;

				float radius = (float)pWindow->m_height/2;

				move.m_pFrom.m_x = (ptLastMousePosit.x - pWindow->m_width/2 )/radius;
				move.m_pFrom.m_y = (pWindow->m_height/2 - ptLastMousePosit.y)/radius;
				move.m_pTo.m_x = (ptCurrentMousePosit.x - pWindow->m_width/2 )/radius;
				move.m_pTo.m_y = (pWindow->m_height/2 - ptCurrentMousePosit.y)/radius;
				move.m_nTrackID = 0;

				pWindow->EnterTouchCS();

				pWindow->m_moveSignal.push_back(move);

				pWindow->LeaveTouchCS();

			}

            ptLastMousePosit.x = ptCurrentMousePosit.x;
            ptLastMousePosit.y = ptCurrentMousePosit.y;
        }
        break;

    case WM_SIZE:
        {
            /*int nWidth  = LOWORD(lParam); 
            int nHeight = HIWORD(lParam);
            glViewport(0, 0, nWidth, nHeight);

            glMatrixMode( GL_PROJECTION );
            glLoadIdentity();
            gluPerspective( 45.0, (GLdouble)nWidth / (GLdouble)nHeight, 0.1, 20000.0);*/
        }
        break;

    case WM_CLOSE:
        {
            PostQuitMessage(0); 
        }

    case WM_DESTROY:
        {
            PostQuitMessage(0);
        }
        break;

    default:
        {
            return DefWindowProc( hWnd, msg, wParam, lParam );
        }
        break;
    }

    return 0;
}

void GL_Init(HDC &hDC, HGLRC &hRC, HWND hWnd, long winWidth, long winHeight, bool mirror)
{
	GLuint PixelFormat;

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DRAW_TO_WINDOW |PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 16;
	pfd.cDepthBits = 16;

	hDC = GetDC( hWnd );
	PixelFormat = ChoosePixelFormat( hDC, &pfd );
	SetPixelFormat( hDC, PixelFormat, &pfd);
	hRC = wglCreateContext( hDC );
	wglMakeCurrent( hDC, hRC );

	glClearColor( 0.35f, 0.53f, 0.7f, 1.0f );
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_DEPTH_TEST);//开启深度检测（important！！！）

	//以下两句是开启alpha通道的透明效果
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	/*glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
*/
	//glMatrixMode( GL_PROJECTION );
	//glLoadIdentity();
	//gluPerspective( 45.0f, 640.0f / 480.0f, 0.1f, 20000.0f);

	//glLoadIdentity();							                // 重置当前的模型观察矩阵
	glMatrixMode( GL_PROJECTION );
	//glLoadIdentity();
	float W_H_Rate = winWidth/(float)winHeight;
	if(mirror)
	{
		W_H_Rate = -W_H_Rate;
	}
	glOrtho( -W_H_Rate, W_H_Rate, -1, 1, -10000, 10000);

#ifdef UseOpenGLExtension2LoadDDS
	//
	// If the required extension is present, get the addresses of its 
	// functions that we wish to use...
	//
	//Many system returns "Microsoft"
	char *vendor = (char*)glGetString(GL_VENDOR);
	//Many system returns "GDI Generic"
	char *renderer = (char*)glGetString(GL_RENDERER);
	//Many system returns "GL_WIN_swap_hint GL_EXT_bgra GL_EXT_paletted_texture"
	char *ext = (char*)glGetString( GL_EXTENSIONS );
	
	if( strstr( ext, "ARB_texture_compression" ) == NULL )
	{
		MessageBox(NULL,"ARB_texture_compression extension was not found",
			"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	else
	{
		glCompressedTexImage2DARB = (PFNGLCOMPRESSEDTEXIMAGE2DARBPROC)wglGetProcAddress("glCompressedTexImage2DARB");
        
		if( !glCompressedTexImage2DARB )
		{
			MessageBox(NULL,"One or more ARB_texture_compression functions were not found",
				"ERROR",MB_OK|MB_ICONEXCLAMATION);
			return;
		}
	}
#endif
}

int glbAddHwndGlbWindowPair(HWND hWnd, GlbWindow *window)
{
	HWND_GLBWINDOW_PAIR pair;
	pair.hWnd = hWnd;
	pair.window = window;
	g_HWND_GLBWINDOW_PAIR.push_back(pair);
	return g_HWND_GLBWINDOW_PAIR.size();
}

int glbDeleteHwndGlbWindowPair(GlbWindow *window)
{
	vector<HWND_GLBWINDOW_PAIR>::iterator i;
	for(i=g_HWND_GLBWINDOW_PAIR.begin(); i!=g_HWND_GLBWINDOW_PAIR.end();)
	{
		if(i->window == window)
		{
			i = g_HWND_GLBWINDOW_PAIR.erase(i);
		}
		else
		{
			i++;
		}
	}
	return g_HWND_GLBWINDOW_PAIR.size();
}

bool glbGetWindowPtr(const HWND hWnd, GlbWindow **pWindow)
{
	vector<HWND_GLBWINDOW_PAIR>::iterator i;
	for(i=g_HWND_GLBWINDOW_PAIR.begin(); i!=g_HWND_GLBWINDOW_PAIR.end(); i++)
	{
		if(i->hWnd == hWnd)
		{
			*pWindow = i->window;
			return true;
		}
	}
	return false;
}

void glbPopTouchSignal(GlbWindow &window, vector<GlbMove> &move, vector<GlbPoint2d> &touch)
{
	window.EnterTouchCS();

	move = window.m_moveSignal;
	touch = window.m_touchSignal;
	window.m_moveSignal.clear();
	window.m_touchSignal.clear();

	window.LeaveTouchCS();
}


int glbCreateWindow(GlbWindow &window, GlbRect windowSize, char *calibFileName, bool fullscreen, bool mirror, HINSTANCE hInstance)
{
	window.m_width = windowSize.m_width;
	window.m_height = windowSize.m_height;

    WNDCLASSEX winClass; 

    winClass.lpszClassName = "GLB_WINDOWS_CLASS";
    winClass.cbSize        = sizeof(WNDCLASSEX);
    winClass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;//改变窗口大小重绘，分配单独上下文给每个窗口
    winClass.lpfnWndProc   = WinProc;
    winClass.hInstance     = hInstance;
    winClass.hIcon         = NULL;
    winClass.hIconSm       = LoadIcon(hInstance, IDC_ARROW);
    winClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    winClass.lpszMenuName  = NULL;
    winClass.cbClsExtra    = 0;
    winClass.cbWndExtra    = 0;

    if( !RegisterClassEx(&winClass) )
    {
        return 0;
    }

    DWORD dwStyle;
    if(fullscreen)
    {
        dwStyle = WS_POPUP | WS_VISIBLE;
    }
    else
    {
        dwStyle = WS_OVERLAPPED | WS_VISIBLE;
    }
    HWND hWnd;
    hWnd = CreateWindowEx( 
        NULL,                   // 扩展窗体风格
        "GLB_WINDOWS_CLASS",    // 类名字
        "MyUniverse",           // 窗口标题
        dwStyle,                // 窗体风格属性
        windowSize.m_left,      // 窗口位置
        windowSize.m_top,       // 窗口位置
        windowSize.m_width,     // 窗口大小
        windowSize.m_height,    // 窗口大小
        NULL,                   // 无父窗口
        NULL,                   // 无菜单
        NULL,                   // 倒数第二个参数应该是hInstance实例
        NULL                    // 不向WM_CREATE传递任何东东
        );

    if( hWnd == NULL )
    {
        return 0;
    }

	glbAddHwndGlbWindowPair(hWnd, &window);//将hWnd和window加入查找表，以便在WindowProc中查询

    ShowWindow( hWnd, SW_SHOW );
    UpdateWindow( hWnd );

    HDC    hDC;
    HGLRC  hRC;
    GL_Init(hDC, hRC, hWnd, windowSize.m_width, windowSize.m_height, mirror);//OpenGL相关的初始化
    glbInitDistort(window.m_calib, calibFileName);//从ini中读取镜头畸变参数

    window.m_hWnd = hWnd;
    window.m_hDC = hDC;
    window.m_hRC = hRC;

    return 1;
}//*/

int glbCreateWindowMFC(GlbWindow &window, CRect rect, char* calibFileName, CWnd* parentWindow, bool mirror)
{
	window.m_width = rect.Width();
	window.m_height = rect.Height();

    HWND parentHWnd = parentWindow->m_hWnd;
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

    HDC    hDC;
    HGLRC  hRC;
    GL_Init(hDC, hRC, hWnd, rect.Width(), rect.Height(), mirror);//OpenGL相关的初始化
    
    glbInitDistort(window.m_calib, calibFileName);//从ini中读取镜头畸变参数

    window.m_hWnd = hWnd;
    window.m_hDC = hDC;
    window.m_hRC = hRC;

    return 1;
}

void glbSwitchWindow(GlbWindow window)
{
    wglMakeCurrent( window.m_hDC, window.m_hRC );
}

void glbClearWindow()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// 清除屏幕和深度缓存
	//glLoadIdentity();							                // 重置当前的模型观察矩阵
	//glMatrixMode( GL_PROJECTION );
	//glLoadIdentity();
	//float W_H_Rate = 640/480.0f;
	//glOrtho( -W_H_Rate, W_H_Rate, -1, 1, -10, 20);
	glMatrixMode( GL_MODELVIEW );
}
void glbDrawImage( GlbImage image )
{
    float g_fDistance = -3.0f;
    float g_fSpinX    = 0.0f;
    float g_fSpinY    = 0.0f;

    struct Vertex
    {
        float tu, tv;
        float x, y, z;
    };

    Vertex g_quadVertices[] =
    {
        { 0.0f,0.0f, -1.0f,-1.0f, 0.0f },
        { 1.0f,0.0f,  1.0f,-1.0f, 0.0f },
        { 1.0f,1.0f,  1.0f, 1.0f, 0.0f },
        { 0.0f,1.0f, -1.0f, 1.0f, 0.0f }
    };
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glTranslatef( 0.0f, 0.0f, g_fDistance );
    glRotatef( -g_fSpinY, 1.0f, 0.0f, 0.0f );
    glRotatef( -g_fSpinX, 0.0f, 1.0f, 0.0f );

    glBindTexture( GL_TEXTURE_2D, image );

    glInterleavedArrays( GL_T2F_V3F, 0, g_quadVertices );
    glDrawArrays( GL_QUADS, 0, 4 );

    glDisable(GL_BLEND);
    //SwapBuffers( g_hDC );
}

void glbDestoryWindow(GlbWindow window, HINSTANCE   hInstance )   
{
	glbDeleteHwndGlbWindowPair(&window);

    if( window.m_hRC != NULL )
    {
        wglMakeCurrent( NULL, NULL );
        wglDeleteContext( window.m_hRC );
        window.m_hRC = NULL;
    }

    if( window.m_hDC != NULL )
    {
        ReleaseDC( window.m_hWnd, window.m_hDC );
        window.m_hDC = NULL;
    }

    UnregisterClass( "GLB_WINDOWS_CLASS", hInstance );
}

int glbUpdateWindow(GlbWindow window, int ms)
{
    MSG uMsg;
    memset(&uMsg,0,sizeof(uMsg));

    SwapBuffers( window.m_hDC );
    if( PeekMessage( &uMsg, NULL, 0, 0, PM_REMOVE ) )
    {
        TranslateMessage( &uMsg );
        DispatchMessage( &uMsg );
    }

    Sleep(ms);

    if(uMsg.message == WM_QUIT)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void glbDrawTexture(
                    GlbImage Image, 
                    GlbRotmat GlobeRotMat,
                    GlbCalib calib,
                    GlbPointGeo pGeo,
                    bool bPointOnGlobe,
                    GlbPointGeo pGeoDirect,
                    bool bDirOnGlobe,
                    bool bHeadDirect,
                    float width,
                    float height,
                    float layer,
                    GlbTexMode mode
    )
{
    int nRow = (int)(height/FACET_SCACLE_IN_ANGLE)+1;
    int nCol = (int)(width/FACET_SCACLE_IN_ANGLE)+1;


    float mHeight = height/nRow;
    float mWidth = width/nCol;

    //计算图像“中心点”的位置
    GlbPoint3d pRect;//“中心点”的直角坐标
    glbPointGeo2PointRect(pGeo, pRect);//将“中心点”转换为直角坐标
    if(bPointOnGlobe)//如果“中心点”在地球上
    {
        glbGlobePoint2ScreenPoint(pRect, GlobeRotMat, pRect);//将“中心点”转换至屏幕上
    }

    GlbPivot pivot_v, pivot_h;//横向和纵向旋转轴
    GlbPoint3d pRectDirect;//“朝向点”的直角坐标
    glbPointGeo2PointRect(pGeoDirect, pRectDirect);//将“朝向点”转换为直角坐标
    if(bDirOnGlobe)//如果“朝向点”在地球上
    {
        glbGlobePoint2ScreenPoint(pRectDirect, GlobeRotMat, pRectDirect);//将“朝向点”转换至屏幕上
    }

    glbCreateNormPivot( //生成贴图转轴
        pRect, //“中心点”直角坐标
        pRectDirect, //“朝向点”直角坐标
        bHeadDirect, //朝向
        pivot_h, //生成贴图所需的纵向转轴（轴是纵向的，旋转将是横向的）
        pivot_v  //生成贴图所需的横向转轴
    );

    if(mode == GLB_TEX_BELT)//如果贴图是类型是“条幅”
    {
        pivot_h = pRectDirect;  //纵向转轴 = 球心和“朝向点”的连线
    }

    GlbPoint3d **PointArr = new GlbPoint3d*[nRow+1];//贴图关节点的二维数组
    for(int x=0; x<=nRow; x++)
    {
        PointArr[x] = new GlbPoint3d[nCol+1];
    }

    float angle_h0 = -width/2.0f; //贴图中心点到左上角的旋转角度
    float angle_v0 = -height/2.0f;//贴图中心点到左上角的旋转角度
    glPointSize(1.0f);//？？？？

    for(int x=0; x<=nCol; x++)//写入贴图关节点的坐标
    {
        for(int y=0; y<=nRow; y++)
        {
            float angle_h = angle_h0 + x * mWidth; //中心点到此关节点的旋转角度
            float angle_v = angle_v0 + y * mHeight;//中心点到此关节点的旋转角度

            GlbPoint3d p_img;//此关节点的直角坐标
            glbPivotingPoint(pRect, pivot_v, angle_v, p_img);//先从中心点纵向旋转
            glbPivotingPoint(p_img, pivot_h, angle_h, p_img);//再横向旋转
            GlbPoint2d r_pRound;//圆饼坐标
            glbPointRect2PointRound(p_img, r_pRound, calib);//将直角坐标转换为圆饼坐标

            PointArr[y][x].m_x = r_pRound.m_x;//赋值
            PointArr[y][x].m_y = r_pRound.m_y;
        }
    }

    glLoadIdentity();	//OpenGL操作
    glColor3f(1.0f,1.0f,1.0f);
    glBindTexture(GL_TEXTURE_2D, Image);	

    float mRectWidth = 1.0f/nCol;//素材分割的小块宽度（素材的坐标是0到1之间的小数）
    float mRectHeight = 1.0f/nRow;//素材分割的小块高度

    glBegin(GL_QUADS);						// 绘制正方形
    for(int x=0; x<nCol; x++)//开始贴图
    {
        for(int y=0; y<nRow; y++)
        {
            GlbPoint2d pRound[4];//块的四个角的圆饼坐标
            pRound[0].m_x = PointArr[y][x].m_x;
            pRound[0].m_y = PointArr[y][x].m_y;
            pRound[1].m_x = PointArr[y][x+1].m_x;
            pRound[1].m_y = PointArr[y][x+1].m_y;
            pRound[2].m_x = PointArr[y+1][x+1].m_x;
            pRound[2].m_y = PointArr[y+1][x+1].m_y;
            pRound[3].m_x = PointArr[y+1][x].m_x;
            pRound[3].m_y = PointArr[y+1][x].m_y;

            float th = 0.4f;//块大小的阈值（太大的块将不会显示）
            if( fabs(pRound[0].m_x-pRound[1].m_x) < th &&//判断块是否过大
                fabs(pRound[0].m_x-pRound[2].m_x) < th &&
                fabs(pRound[1].m_x-pRound[3].m_x) < th &&
                fabs(pRound[2].m_x-pRound[3].m_x) < th &&

                fabs(pRound[0].m_y-pRound[1].m_y) < th &&
                fabs(pRound[0].m_y-pRound[2].m_y) < th &&
                fabs(pRound[1].m_y-pRound[3].m_y) < th &&
                fabs(pRound[2].m_y-pRound[3].m_y) < th)
            {
                glTexCoord2f((x) * mRectWidth, 1- (y) * mRectHeight); 
                glVertex3f(pRound[0].m_x, pRound[0].m_y, layer);				// 左上

                glTexCoord2f((x+1) * mRectWidth, 1- (y) * mRectHeight); 
                glVertex3f(pRound[1].m_x, pRound[1].m_y, layer);				// 右上

                glTexCoord2f((x+1) * mRectWidth, 1- (y+1) * mRectHeight); 
                glVertex3f(pRound[2].m_x, pRound[2].m_y, layer);				// 右下

                glTexCoord2f((x) * mRectWidth, 1- (y+1) * mRectHeight); 
                glVertex3f(pRound[3].m_x, pRound[3].m_y, layer);				// 左下
            }
        }
    }
    glEnd();//绘制结束


    for(int x=0; x<=nRow; x++)//销毁存放关节点坐标的二维数组
    {
        delete [nCol+1] (PointArr[x]);
        PointArr[x] = NULL;
    }
    delete [nRow+1] PointArr;
    PointArr = NULL;
}

float glbDrawLine(
	GlbPointGeo geoStartPoint, 
	bool bStartPointOnGlobe, 
	GlbPointGeo geoEndPoint, 
	bool bEndPointOnGlobe, 
	GlbRotmat GlobeRotMat, 
	GlbCalib calib, 
	int layer)
{
	GlbPoint3d rectStartPoint, rectEndPoint;
	glbPointGeo2PointRect(geoStartPoint, rectStartPoint);
	glbPointGeo2PointRect(geoEndPoint, rectEndPoint);
	if(bStartPointOnGlobe)
	{
		glbGlobePoint2ScreenPoint(rectStartPoint, GlobeRotMat, rectStartPoint);
	}
	if(bEndPointOnGlobe)
	{
		glbGlobePoint2ScreenPoint(rectEndPoint, GlobeRotMat, rectEndPoint);
	}
	GlbPointGeo geoStartPoint2, geoEndPoint2;
	glbPointRect2PointGeo(rectStartPoint, geoStartPoint2);
	glbPointRect2PointGeo(rectEndPoint, geoEndPoint2);
	float angle = glbDrawLineOnScreen(calib, geoStartPoint2, geoEndPoint2, layer);
	return angle;
}

float glbDrawPolygon(	vector<GlbPointGeo> points, bool bOnGlobe, bool bClosed,
										GlbRotmat GlobeRotMat, GlbCalib calib, int layer)
{
	float angle=0.0;
	vector<GlbPointGeo>::iterator it;
	if(points.size() >= 2)
	{
		for(it=points.begin(); it!=(points.end()-1); it++)
		{
			GlbPointGeo p1 = *it;
			GlbPointGeo p2 = *(it+1);
			angle += glbDrawLine(p1, bOnGlobe, p2, bOnGlobe, GlobeRotMat, calib, layer);
		}

		if(bClosed)
		{
			GlbPointGeo p1 = *(points.end()-1);
			GlbPointGeo p2 = *points.begin();
			angle += glbDrawLine(p1, bOnGlobe, p2, bOnGlobe, GlobeRotMat, calib, layer);
		}
	}
	return angle;
}

float glbDrawLineOnScreen(GlbCalib calib, GlbPointGeo geoStartPoint, GlbPointGeo geoEndPoint, int layer)
{
	GlbPoint3d rectStartPoint, rectEndPoint;
	glbPointGeo2PointRect(geoStartPoint, rectStartPoint);
	glbPointGeo2PointRect(geoEndPoint, rectEndPoint);

	float angle = glbAngleBetweenPoints(rectStartPoint, rectEndPoint);
	GlbPivot pivot;
	glbPivotBetweenPoints(rectStartPoint, rectEndPoint, pivot);

	int nStep = (int)(angle/FACET_SCACLE_IN_ANGLE)+1;
	float mAngle = angle/nStep;

	glLoadIdentity();											// 重置当前的模型观察矩阵
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3f(1.0f,1.0f,0.0f);
	glLineWidth(0.5);//设置不闭合折线大小  

	//抗锯齿
	glEnable(GL_LINE_SMOOTH);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_BLEND);

	for(int i=0; i<nStep; i++)
	{
		GlbPoint3d p3d1,p3d2;
		GlbPoint2d p2d1,p2d2;
		glbPivotingPoint(rectStartPoint, pivot, mAngle*i, p3d1);
		glbPointRect2PointRound(p3d1, p2d1, calib);
		glbPivotingPoint(rectStartPoint, pivot, mAngle*(i+1), p3d2);
		glbPointRect2PointRound(p3d2, p2d2, calib);
		if(fabs(p2d1.m_x-p2d2.m_x)<MAX_FACET_SHOW_THRESHOLD && fabs(p2d1.m_y-p2d2.m_y)<MAX_FACET_SHOW_THRESHOLD)
		{
			glBegin(GL_LINES);//画不闭合折线 
			glVertex3f(p2d1.m_x,p2d1.m_y, (GLfloat)layer);
			glVertex3f(p2d2.m_x,p2d2.m_y, (GLfloat)layer);
			glEnd();
		}
	}
	return angle;
}

float glbDrawCircle(GlbPointGeo geoCenterPoint, bool bCenterPointOnGlobe, float radius, GlbRotmat GlobeRotMat, GlbCalib calib,  int layer)
{
	float perimeter=0.0;
	GlbPoint3d center3d;
	glbPointGeo2PointRect(geoCenterPoint, center3d);

	if(bCenterPointOnGlobe)
	{
		//如果是地球上的点，转化成屏幕上的点
		glbGlobePoint2ScreenPoint(center3d, GlobeRotMat, center3d);
	}

	//取北极点的方向为画圆的起始方向
	GlbPointGeo polar(90,0);
	GlbPoint3d polar3d;
	glbPointGeo2PointRect(polar, polar3d);

	float angle = glbAngleBetweenPoints(center3d, polar3d);
	if(angle<1)
	{
		//如果中心点距北极点不足1度，则取南极点
		polar.m_lat = -90;
		glbPointGeo2PointRect(polar, polar3d);
	}
	GlbPivot pivot;
	//计算出中心点到极点的转轴
	glbPivotBetweenPoints(center3d, polar3d, pivot);

	GlbPoint3d circlePoint1,circlePoint2;
	//将中心点转向极点radius度
	glbPivotingPoint(center3d, pivot, radius, circlePoint1);

	for(int i=0; i<60; i++)
	{
		//将circlePoint1绕中心点旋转6度，循环60次，画成360度的圆圈
		glbPivotingPoint(circlePoint1, center3d, 6, circlePoint2);

		GlbPointGeo p1Geo, p2Geo;
		glbPointRect2PointGeo(circlePoint1, p1Geo);
		glbPointRect2PointGeo(circlePoint2, p2Geo);

		perimeter += glbDrawLineOnScreen(calib, p1Geo, p2Geo, layer);

		circlePoint1 = circlePoint2;
	}
	return perimeter;
}

void glbDrawGlobe(GlbImage Image, GlbRotmat GlobeRotMat, GlbCalib calib)
{
	GlbPointGeo pGeo,pGeoDirect;
	pGeo.m_lat = 0;
	pGeo.m_lng = 0;
	pGeoDirect.m_lat = 90;
	pGeoDirect.m_lng = 0;

	glbDrawTexture(
		Image,	//材质的编号
        GlobeRotMat,
        calib,
		pGeo,			//贴图的中心点
		true,	//中心点坐标是在 globe坐标系(true) or screen坐标系(false)
		pGeoDirect,	//贴图方向的参考点
		true,		//方向参考点实在 globe坐标系(true) or screen坐标系(false)
		true,		//图片朝向参考点(ture) or 背向参考点(false)
		360,			//贴图的宽度(单位:角度)
		180,			//贴图的高度(单位:角度)
		0,			//贴图所在的层
		GLB_TEX_BELT
		);
}

void glbDrawBelt(GlbImage Image, 
              GlbRotmat GlobeRotMat,
              GlbCalib calib,
			  GlbPointGeo pGeo, 
			  bool bPointOnGlobe, 
			  GlbPointGeo pGeoDirect, 
			  bool bDirOnGlobe, 
			  bool bHeadDirect, 
			  float width, 
			  float height, 
			  float layer )
{
	glbDrawTexture(
		Image,	//材质的编号
        GlobeRotMat,
        calib,
		pGeo,			//贴图的中心点
		bPointOnGlobe,	//中心点坐标是在 globe坐标系(true) or screen坐标系(false)
		pGeoDirect,	//贴图方向的参考点
		bDirOnGlobe,		//方向参考点实在 globe坐标系(true) or screen坐标系(false)
		bHeadDirect,		//图片朝向参考点(ture) or 背向参考点(false)
		width,			//贴图的宽度(单位:角度)
		height,			//贴图的高度(单位:角度)
		layer,			//贴图所在的层
		GLB_TEX_BELT
		);
}

int glbDetectScreen(vector<GlbRect> &screens)
{
	DISPLAY_DEVICE displayDevice;
	displayDevice.cb = sizeof(DISPLAY_DEVICE);

	DEVMODE deviceMode;
	ZeroMemory(&deviceMode, sizeof(DEVMODE));
	deviceMode.dmSize = sizeof(DEVMODE);

	int i = 0;
	while(::EnumDisplayDevices(NULL, i++, &displayDevice, 0))
	{
		if(displayDevice.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP &&
			!(displayDevice.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER))
		{
			if(EnumDisplaySettingsEx(displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &deviceMode, 0) == FALSE)
            {
				EnumDisplaySettingsEx(displayDevice.DeviceName, ENUM_REGISTRY_SETTINGS, &deviceMode, 0);
            }
            GlbRect screen;
            screen.m_top = deviceMode.dmPosition.y;
            screen.m_left = deviceMode.dmPosition.x;
            screen.m_width = deviceMode.dmPelsWidth;
            screen.m_height = deviceMode.dmPelsHeight;

            screens.push_back(screen);
		}
	}	
	return screens.size();
}

int glbGetTopLayer(GlbWindow &window, GlbPoint2d pTouch)
{
	int width = window.m_width;
	int height = window.m_height;
	float Radius = height/2.0f;
	int win_x = (int)(width/2.0 + Radius * pTouch.m_x);
	int win_y = (int)(Radius + Radius * pTouch.m_y);

	GLfloat zbuf;
	//glReadBuffer(GL_DEPTH_ATTACHMENT_EXT);
	glReadPixels(win_x, win_y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &zbuf);
	//printf_s( "win_x, win_y, zbuf: %d, %d, %f.\n", win_x, win_y, zbuf);

	GLint		viewport[4];
	GLdouble	mvmatrix[16];
	GLdouble	projmatrix[16];
	GLdouble	obj_x, obj_y, obj_z;
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);
	gluUnProject( win_x, win_y, zbuf, mvmatrix, projmatrix, viewport, &obj_x, &obj_y, &obj_z);
	//printf_s( "Object Coordinate: %f, %f, %f.\n", obj_x, obj_y, obj_z );
	return (int)(obj_z+0.5);
}

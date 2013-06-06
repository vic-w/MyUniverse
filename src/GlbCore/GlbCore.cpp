#include "stdafx.h"
#include <windows.h>
#include "GlbCore.h"
#include "GlbType.h"
#include <stdio.h>
#include <malloc.h>
#include "opencv.hpp"

DDS_IMAGE_DATA* glbLoadDDSFile(const char* filename);
PFNGLCOMPRESSEDTEXIMAGE2DARBPROC glCompressedTexImage2DARB;

HWND   g_hWnd                = NULL;
HDC    g_hDC                 = NULL;
HGLRC  g_hRC                 = NULL;


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

//GlbRotmat g_GlobeRotMat;

#define FACET_SCACLE_IN_ANGLE (6) //每个面片占的最小角度
#define MAX_FACET_SHOW_THRESHOLD (0.4f)//能显示的最大面片的size

GlbImage glbLoadImage(const char* filename)  //载入图像（支持dds,jpg,png）
{
    int fileNameLen = strlen(filename);
    const char* suffix = filename + fileNameLen - 4;
    //printf("filename = %s, suffix = %s\n", filename, suffix);

    if(_stricmp(suffix,".dds") == 0)
    {
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

                glCompressedTexImage2DARB( GL_TEXTURE_2D,
                    i,
                    pDDSImageData->format,
                    nWidth,
                    nHeight,
                    0,
                    nSize,
                    pDDSImageData->pixels + nOffset );

                nOffset += nSize;

                // Half the image size for the next mip-map level...
                nWidth  = (nWidth  / 2);
                nHeight = (nHeight / 2);
            }
        }
        else
        {
            return NULL;
        }
        if( pDDSImageData != NULL )
        {
            if( pDDSImageData->pixels != NULL )
                free( pDDSImageData->pixels );

            free( pDDSImageData );
        }
        return TextureID;
    }
    else if(_stricmp(suffix,".jpg") == 0 
        || _stricmp(suffix,".bmp") == 0 
        || _stricmp(suffix,".png") == 0)
    {
        GLuint TextureID = -1;
        IplImage *image = cvLoadImage(filename);

        if(!image)
        {
            MessageBox(0, "读取图像错误！", NULL, MB_OK);
            return false;
        }

        int sizeX,sizeY;
        sizeX = image->width;
        sizeY = image->height;

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

        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, sizeX, sizeY, GL_BGR_EXT, GL_UNSIGNED_BYTE, image->imageData);

        //glEnable(GL_TEXTURE_2D);
        // Now we need to free the image data that we loaded since openGL stored it as a texture
        cvReleaseImage(&image);
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
    static bool bMousing;

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
            ptLastMousePosit.x = ptCurrentMousePosit.x = LOWORD (lParam);
            ptLastMousePosit.y = ptCurrentMousePosit.y = HIWORD (lParam);
            bMousing = true;
        }
        break;

    case WM_LBUTTONUP:
        {
            bMousing = false;
        }
        break;

    case WM_MOUSEMOVE:
        {
            ptCurrentMousePosit.x = LOWORD (lParam);
            ptCurrentMousePosit.y = HIWORD (lParam);

            if( bMousing )
            {
                g_fSpinX -= (ptCurrentMousePosit.x - ptLastMousePosit.x);
                g_fSpinY -= (ptCurrentMousePosit.y - ptLastMousePosit.y);
            }

            ptLastMousePosit.x = ptCurrentMousePosit.x;
            ptLastMousePosit.y = ptCurrentMousePosit.y;
        }
        break;

    case WM_SIZE:
        {
            int nWidth  = LOWORD(lParam); 
            int nHeight = HIWORD(lParam);
            glViewport(0, 0, nWidth, nHeight);

            glMatrixMode( GL_PROJECTION );
            glLoadIdentity();
            gluPerspective( 45.0, (GLdouble)nWidth / (GLdouble)nHeight, 0.1, 100.0);
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


void GL_Init( HWND hWnd )
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

    g_hDC = GetDC( hWnd );
    PixelFormat = ChoosePixelFormat( g_hDC, &pfd );
    SetPixelFormat( g_hDC, PixelFormat, &pfd);
    g_hRC = wglCreateContext( g_hDC );
    wglMakeCurrent( g_hDC, g_hRC );

    glClearColor( 0.35f, 0.53f, 0.7f, 1.0f );
    glEnable(GL_TEXTURE_2D);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 45.0f, 640.0f / 480.0f, 0.1f, 100.0f);

    //
    // If the required extension is present, get the addresses of its 
    // functions that we wish to use...
    //

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
        glCompressedTexImage2DARB;
        if( !glCompressedTexImage2DARB )
        {
            MessageBox(NULL,"One or more ARB_texture_compression functions were not found",
                "ERROR",MB_OK|MB_ICONEXCLAMATION);
            return;
        }
    }
}


int glbCreateWindow(HINSTANCE   hInstance)
{
    WNDCLASSEX winClass; 

    winClass.lpszClassName = "GLB_WINDOWS_CLASS";
    winClass.cbSize        = sizeof(WNDCLASSEX);
    winClass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
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

    g_hWnd = CreateWindowEx( NULL, "GLB_WINDOWS_CLASS", //倒数第二个参数应该是hInstance
        "MyUniverse",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        0, 0, 640, 480, NULL, NULL, NULL, NULL );

    if( g_hWnd == NULL )
    {
        return 0;
    }

    ShowWindow( g_hWnd, SW_SHOW );
    UpdateWindow( g_hWnd );

    GL_Init(g_hWnd);

    //CreateGlbRotmat(g_GlobeRotMat);
    return 1;
}//*/

void glbClearWindow()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// 清除屏幕和深度缓存
	glLoadIdentity();							// 重置当前的模型观察矩阵
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	float W_H_Rate = 1;
	glOrtho( -W_H_Rate, W_H_Rate, -1, 1, -10, 20);
	glMatrixMode( GL_MODELVIEW );
}
void glbDrawImage( GlbImage image )
{
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

void glbDestoryWindow( HINSTANCE   hInstance )   
{

    if( g_hRC != NULL )
    {
        wglMakeCurrent( NULL, NULL );
        wglDeleteContext( g_hRC );
        g_hRC = NULL;
    }

    if( g_hDC != NULL )
    {
        ReleaseDC( g_hWnd, g_hDC );
        g_hDC = NULL;
    }

    UnregisterClass( "GLB_WINDOWS_CLASS", hInstance );
}

int glbUpdateWindow(int ms)
{
    MSG uMsg;
    memset(&uMsg,0,sizeof(uMsg));

    SwapBuffers( g_hDC );
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

void DrawTexture(
                    GlbImage Image, 
                    GlbRotmat GlobeRotMat,
                    GlbPointGeo pGeo,
                    bool bPointOnGlobe,
                    GlbPointGeo pGeoDirect,
                    bool bDirOnGlobe,
                    bool bHeadDirect,
                    float width,
                    float height,
                    float layer,
                    GlbTexMode mode,
                    GlbPoint3d &pClose
    )
{
    int nRow = (int)(height/FACET_SCACLE_IN_ANGLE)+1;
    int nCol = (int)(width/FACET_SCACLE_IN_ANGLE)+1;


    float mHeight = height/nRow;
    float mWidth = width/nCol;

    //计算当前城市的位置
    GlbPoint3d pRect;
    GlbPoint2d r_pRound;
    Geo2Rect(pGeo, pRect);
    if(bPointOnGlobe)
    {
        Sphere2Sphere(pRect, GlobeRotMat, pRect);
    }

    GlbPoint3d pivot_v, pivot_h;
    GlbPoint3d pRectDirect;
    Geo2Rect(pGeoDirect, pRectDirect);
    if(bDirOnGlobe)
    {
        Sphere2Sphere(pRectDirect, GlobeRotMat, pRectDirect);
    }

    CreateNormPivot( pRect, pRectDirect, bHeadDirect, pivot_h, pivot_v);
    if(mode == GLB_TEX_BELT)
    {
        pivot_h = pRectDirect;
    }

    GlbPoint3d **PointArr = new GlbPoint3d*[nRow+1];
    for(int x=0; x<=nRow; x++)
    {
        PointArr[x] = new GlbPoint3d[nCol+1];
    }

    float angle_h0 = -width/2.0f;
    float angle_v0 = -height/2.0f;
    glPointSize(1.0f);

    for(int x=0; x<=nCol; x++)
    {
        for(int y=0; y<=nRow; y++)
        {
            float angle_h = angle_h0 + x * mWidth;
            float angle_v = angle_v0 + y * mHeight;

            GlbPoint3d p_img;
            PivotRotPoint(pRect, pivot_v, angle_v, p_img);
            PivotRotPoint(p_img, pivot_h, angle_h, p_img);
            Sphere2Round(p_img, 1.0f, r_pRound);

            PointArr[y][x].m_x = r_pRound.m_x;
            PointArr[y][x].m_y = r_pRound.m_y;
        }
    }

    glLoadIdentity();	
    glColor3f(1.0f,1.0f,1.0f);
    glBindTexture(GL_TEXTURE_2D, Image);	

    float mRectWidth = 1.0f/nCol;
    float mRectHeight = 1.0f/nRow;

    for(int x=0; x<nCol; x++)
    {
        for(int y=0; y<nRow; y++)
        {
            float th = 0.4f;

            GlbPoint2d pRound[4];
            pRound[0].m_x = PointArr[y][x].m_x;
            pRound[0].m_y = PointArr[y][x].m_y;
            pRound[1].m_x = PointArr[y][x+1].m_x;
            pRound[1].m_y = PointArr[y][x+1].m_y;
            pRound[2].m_x = PointArr[y+1][x+1].m_x;
            pRound[2].m_y = PointArr[y+1][x+1].m_y;
            pRound[3].m_x = PointArr[y+1][x].m_x;
            pRound[3].m_y = PointArr[y+1][x].m_y;


            if( fabs(pRound[0].m_x-pRound[1].m_x) < th &&
                fabs(pRound[0].m_x-pRound[2].m_x) < th &&
                fabs(pRound[1].m_x-pRound[3].m_x) < th &&
                fabs(pRound[2].m_x-pRound[3].m_x) < th &&

                fabs(pRound[0].m_y-pRound[1].m_y) < th &&
                fabs(pRound[0].m_y-pRound[2].m_y) < th &&
                fabs(pRound[1].m_y-pRound[3].m_y) < th &&
                fabs(pRound[2].m_y-pRound[3].m_y) < th)
            {
                glBegin(GL_QUADS);						// 绘制正方形

                glTexCoord2f((x) * mRectWidth, 1- (y) * mRectHeight); 
                glVertex3f(pRound[0].m_x, pRound[0].m_y, layer);				// 左上

                glTexCoord2f((x+1) * mRectWidth, 1- (y) * mRectHeight); 
                glVertex3f(pRound[1].m_x, pRound[1].m_y, layer);				// 右上
                glTexCoord2f((x+1) * mRectWidth, 1- (y+1) * mRectHeight); 
                glVertex3f(pRound[2].m_x, pRound[2].m_y, layer);				// 右下


                glTexCoord2f((x) * mRectWidth, 1- (y+1) * mRectHeight); 
                glVertex3f(pRound[3].m_x, pRound[3].m_y, layer);				// 左下
                glEnd();
            }
        }
    }


    for(int x=0; x<=nRow; x++)
    {
        delete [nCol+1] (PointArr[x]);
        PointArr[x] = NULL;
    }
    delete [nRow+1] PointArr;
    PointArr = NULL;

    //输出pClose
    PivotRotPoint(pRect, pivot_v, height/2.0f, pClose);
    PivotRotPoint(pClose, pivot_h, width/2.0f, pClose);

    //SwapBuffers( g_hDC );
}

void DrawLineOnGlobe(GlbPointGeo geoStartPoint, GlbRotmat GlobeRotMat, GlbPointGeo geoEndPoint, int layer)
{
	GlbPoint3d rectStartPoint, rectEndPoint;
	Geo2Rect(geoStartPoint, rectStartPoint);
	Geo2Rect(geoEndPoint, rectEndPoint);
	Sphere2Sphere(rectStartPoint, GlobeRotMat, rectStartPoint);
	Sphere2Sphere(rectEndPoint, GlobeRotMat, rectEndPoint);
	GlbPointGeo geoStartPoint2, geoEndPoint2;
	Rect2Geo(rectStartPoint, geoStartPoint2);
	Rect2Geo(rectEndPoint, geoEndPoint2);
	DrawLineOnScreen(geoStartPoint2, geoEndPoint2, layer);
}

void DrawLineOnScreen(GlbPointGeo geoStartPoint, GlbPointGeo geoEndPoint, int layer)
{
	GlbPoint3d rectStartPoint, rectEndPoint;
	Geo2Rect(geoStartPoint, rectStartPoint);
	Geo2Rect(geoEndPoint, rectEndPoint);

	float angle = Point2Angle(rectStartPoint, rectEndPoint);
	GlbPoint3d pivot;
	Point2Pivot(rectStartPoint, rectEndPoint, pivot, true);

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
		PivotRotPoint(rectStartPoint, pivot, mAngle*i, p3d1);
		Sphere2Round(p3d1, 1.0f, p2d1);
		PivotRotPoint(rectStartPoint, pivot, mAngle*(i+1), p3d2);
		Sphere2Round(p3d2, 1.0f, p2d2);
		if(fabs(p2d1.m_x-p2d2.m_x)<MAX_FACET_SHOW_THRESHOLD && fabs(p2d1.m_y-p2d2.m_y)<MAX_FACET_SHOW_THRESHOLD)
		{
			glBegin(GL_LINES);//画不闭合折线 
			glVertex3f(p2d1.m_x,p2d1.m_y, (GLfloat)layer);
			glVertex3f(p2d2.m_x,p2d2.m_y, (GLfloat)layer);
			glEnd();
		}
	}
}

void DrawGlobe(GlbImage Image, GlbRotmat GlobeRotMat)
{
	GlbPoint3d pClose;
	GlbPointGeo pGeo,pGeoDirect;
	pGeo.m_lat = 0;
	pGeo.m_lng = 0;
	pGeoDirect.m_lat = 90;
	pGeoDirect.m_lng = 0;

	DrawTexture(
		Image,	//材质的编号
        GlobeRotMat,
		pGeo,			//贴图的中心点
		true,	//中心点坐标是在 globe坐标系(true) or screen坐标系(false)
		pGeoDirect,	//贴图方向的参考点
		true,		//方向参考点实在 globe坐标系(true) or screen坐标系(false)
		true,		//图片朝向参考点(ture) or 背向参考点(false)
		360,			//贴图的宽度(单位:角度)
		180,			//贴图的高度(单位:角度)
		0,			//贴图所在的层
		GLB_TEX_BELT,
		pClose		//返回贴图右上角的坐标
		);
}

void DrawBelt(GlbImage Image, 
              GlbRotmat GlobeRotMat,
			  GlbPointGeo pGeo, 
			  bool bPointOnGlobe, 
			  GlbPointGeo pGeoDirect, 
			  bool bDirOnGlobe, 
			  bool bHeadDirect, 
			  float width, 
			  float height, 
			  float layer )
{
	GlbPoint3d pClose;
	DrawTexture(
		Image,	//材质的编号
        GlobeRotMat,
		pGeo,			//贴图的中心点
		bPointOnGlobe,	//中心点坐标是在 globe坐标系(true) or screen坐标系(false)
		pGeoDirect,	//贴图方向的参考点
		bDirOnGlobe,		//方向参考点实在 globe坐标系(true) or screen坐标系(false)
		bHeadDirect,		//图片朝向参考点(ture) or 背向参考点(false)
		width,			//贴图的宽度(单位:角度)
		height,			//贴图的高度(单位:角度)
		layer,			//贴图所在的层
		GLB_TEX_BELT,
		pClose		//返回贴图右上角的坐标
		);
}

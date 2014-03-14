#pragma once
//#include "windows.h"
#include <vector>
#include <list> 
#include "GlbRot.h"
#include "opencv.hpp"
#include <afxwin.h>
#include "GlbCoreType.h"
#include "copyright.h"

#ifdef GLBCORE_EXPORTS
#define GLBCORE_API __declspec(dllexport)
#else
#define GLBCORE_API __declspec(dllimport)
#endif

using namespace std;

typedef unsigned int GlbImage;


GLBCORE_API int glbDetectScreen(vector<GlbRect> &screens);//检测屏幕的个数及分辨率，返回屏幕个数

GLBCORE_API GlbImage glbLoadImage(const char* filename, GlbSize &size);    //载入图像（支持dds,jpg,bmp,png）
GLBCORE_API GlbImage glbLoadImage(const char* filename);    //载入图像（支持dds,jpg,bmp,png）
GLBCORE_API GlbImage glbLoadImageFromOpencv(IplImage* pImage, bool bMipmap = true);       //从OpenCV图像转换
GLBCORE_API GlbImage glbLoadCopyrightImage();
GLBCORE_API void glbReleaseImage(GlbImage* pImage);         //释放图像

GLBCORE_API int glbCreateWindow(GlbWindow &window, GlbRect windowSize, char *calibFileName, bool fullscreen, bool mirror, HINSTANCE hInstance=0);
GLBCORE_API int glbCreateWindowMFC(GlbWindow &window, CRect rect, char *calibFileName, CWnd* parentWindow, bool mirror);
GLBCORE_API void glbSwitchWindow(GlbWindow window);
GLBCORE_API void glbDestoryWindow(GlbWindow window, HINSTANCE hInstance=0);
GLBCORE_API void glbPopTouchSignal(GlbWindow &window, vector<GlbMove> &move, vector<GlbPoint2d> &touch); 

GLBCORE_API void glbClearWindow();                                    //清空窗口内容
GLBCORE_API void glbDrawImage(GlbImage image);              //在窗口中画图,但并没有显示

GLBCORE_API int glbUpdateWindow(GlbWindow window, int ms);                    //更新窗口中内容,返回0表示窗口退出，返回1表示正常运行

//画线,返回绕地心的角度
GLBCORE_API float glbDrawLine(	GlbPointGeo geoStartPoint, bool bStartPointOnGlobe, 
										GlbPointGeo geoEndPoint, bool bEndPointOnGlobe, 
										GlbRotmat GlobeRotMat, GlbCalib calib, int layer);
//画多边形
GLBCORE_API float glbDrawPolygon(	vector<GlbPointGeo> points, bool bOnGlobe, bool bClosed,
										GlbRotmat GlobeRotMat, GlbCalib calib, int layer);

//画线（基础）
GLBCORE_API float glbDrawLineOnScreen(GlbCalib calib, GlbPointGeo geoStartPoint, GlbPointGeo geoEndPoint, int layer);
//画圆
GLBCORE_API float glbDrawCircle(GlbPointGeo geoCenterPoint, bool bCenterPointOnGlobe, float radius, GlbRotmat GlobeRotMat, GlbCalib calib,  int layer);

//画星球的底图
GLBCORE_API void glbDrawGlobe(GlbImage Image, GlbRotmat GlobeRotMat, GlbCalib calib);
//画图
GLBCORE_API void glbDrawTexture(
                    GlbImage Image,     //材质的编号
                    GlbRotmat GlobeRotMat,
                    GlbCalib calib,
                    GlbPointGeo pGeo,       //贴图的中心点
                    bool bPointOnGlobe,     //中心点坐标是在 globe坐标系(true) or screen坐标系(false)
                    GlbPointGeo pGeoDirect, //贴图方向的参考点
                    bool bDirOnGlobe,       //方向参考点实在 globe坐标系(true) or screen坐标系(false)
                    bool bHeadDirect,       //图片朝向参考点(ture) or 背向参考点(false)
                    float width,            //贴图的宽度(单位:角度)
                    float height,           //贴图的高度(单位:角度)
                    float layer,            //贴图所在的层
                    GlbTexMode mode
);
//画带状图
GLBCORE_API void glbDrawBelt(
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
                float layer
    );

//识别图层编号
GLBCORE_API int glbGetTopLayer(GlbWindow &window, GlbPoint2d pTouch);



//GLBCORE_API void glbShowFrameRate();

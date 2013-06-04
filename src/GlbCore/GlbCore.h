#pragma once
#include "windows.h"
#include "GlbRot.h"

#ifdef GLBCORE_EXPORTS
#define GLBCORE_API __declspec(dllexport)
#else
#define GLBCORE_API __declspec(dllimport)
#endif

typedef unsigned int GlbImage;

enum GlbTexMode
{
    GLB_TEX_RECT,
    GLB_TEX_BELT
};

GLBCORE_API GlbImage glbLoadImage(const char* filename);    //载入图像（支持dds,jpg,png）
GLBCORE_API void glbReleaseImage(GlbImage* pImage);         //释放图像

GLBCORE_API int glbCreateWindow(HINSTANCE   hInstance);
GLBCORE_API void glbDestoryWindow(HINSTANCE   hInstance);

//void glbClearWindow();                                    //清空窗口内容
GLBCORE_API void glbDrawImage(GlbImage image);              //在窗口中画图,但并没有显示

GLBCORE_API int glbUpdateWindow(int ms);                    //更新窗口中内容,返回0表示窗口退出，返回1表示正常运行

void DrawLineOnGlobe(GlbPointGeo geoStartPoint, GlbPointGeo geoEndPoint, int layer);
void DrawLineOnScreen(GlbPointGeo geoStartPoint, GlbPointGeo geoEndPoint, int layer);
void DrawGlobe(GlbImage Image);
void DrawTexture(
                    GlbImage Image,     //材质的编号
                    GlbPointGeo pGeo,       //贴图的中心点
                    bool bPointOnGlobe,     //中心点坐标是在 globe坐标系(true) or screen坐标系(false)
                    GlbPointGeo pGeoDirect, //贴图方向的参考点
                    bool bDirOnGlobe,       //方向参考点实在 globe坐标系(true) or screen坐标系(false)
                    bool bHeadDirect,       //图片朝向参考点(ture) or 背向参考点(false)
                    float width,            //贴图的宽度(单位:角度)
                    float height,           //贴图的高度(单位:角度)
                    float layer,            //贴图所在的层
                    GlbTexMode mode,
                    GlbPoint3d &pClose      //返回贴图右上角的坐标
);

void DrawBelt(
                GlbImage Image,
                GlbPointGeo pGeo,
                bool bPointOnGlobe,
                GlbPointGeo pGeoDirect,	
                bool bDirOnGlobe,		
                bool bHeadDirect,		
                float width,			
                float height,			
                float layer
    );

void ShowFrameRate();


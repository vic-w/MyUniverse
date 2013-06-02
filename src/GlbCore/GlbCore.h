#pragma once

#ifdef GLBCORE_EXPORTS
#define GLBCORE_API __declspec(dllexport)
#else
#define GLBCORE_API __declspec(dllimport)
#endif

typedef unsigned int GlbImage;

GLBCORE_API GlbImage glbLoadImage(const char* filename);    //载入图像（支持dds,jpg,png）
GLBCORE_API void glbReleaseImage(GlbImage* pImage);         //释放图像

GLBCORE_API int glbCreateWindow(HINSTANCE   hInstance);
GLBCORE_API void glbDestoryWindow(HINSTANCE   hInstance);

//void glbClearWindow();                                    //清空窗口内容
GLBCORE_API void glbDrawImage(GlbImage image);              //在窗口中画图,但并没有显示

GLBCORE_API int glbUpdateWindow(int ms);                    //更新窗口中内容
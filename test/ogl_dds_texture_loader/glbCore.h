#pragma once

typedef unsigned int GlbImage;

GlbImage glbLoadImage(const char* filename);  //载入图像（支持dds,jpg,png）
void glbReleaseImage(GlbImage* pImage);            //释放图像

int glbCreateWindow(HINSTANCE   hInstance);
void glbDestoryWindow(HINSTANCE   hInstance);

//void glbClearWindow();                  //清空窗口内容
void glbDrawImage(GlbImage image);                    //在窗口中画图,但并没有显示
int glbUpdateWindow(int ms);                //更新窗口中内容
#ifndef DRAW_H_H_H
#define DRAW_H_H_H

#include "rotation.h"
#include "gl/glut.h"
#include <string>

using namespace std;

#define FACET_SCACLE_IN_ANGLE (6) //每个面片占的最小角度
#define MAX_FACET_SHOW_THRESHOLD (0.4f)//能显示的最大面片的size

//#define GLB_TEX_RECT (1)
//#define GLB_TEX_BELT (2)

enum GlbTexMode
{
	GLB_TEX_RECT,
	GLB_TEX_BELT
};

class GlbObject
{
private:
	GLuint TextureID;					//材质的编号
	string filename;					//材质的文件名
	GlbPointGeo m_CenterPoint;			//贴图的中心点
	bool m_bCenterPointOnGlobe;			//中心点坐标是在 globe坐标系(true) or screen坐标系(false)
	GlbPointGeo m_DirectPoint;			//贴图方向的参考点
	bool m_bDirectPointOnGlobe;			//方向参考点实在 globe坐标系(true) or screen坐标系(false)
	bool m_bHeadDirect;					//图片朝向参考点(ture) or 背向参考点(false)
	float m_width;						//贴图的宽度(单位:角度)
	float m_height;						//贴图的高度(单位:角度)
	unsigned int m_layer;				//贴图所在的层
	GlbTexMode m_mode;
};

// class MovieID
// {
// public:
// 	unsigned int ID;
// 	std::string filename;
// 
// public:
// 	//MovieID();
// };

void DrawLineOnGlobe(GlbPointGeo geoStartPoint, GlbPointGeo geoEndPoint, int layer);
void DrawLineOnScreen(GlbPointGeo geoStartPoint, GlbPointGeo geoEndPoint, int layer);
void DrawGlobe(GLuint texture);
void DrawTexture(
				 GLuint TextureName,	//材质的编号
				 GlbPointGeo pGeo,			//贴图的中心点
				 bool bPointOnGlobe,	//中心点坐标是在 globe坐标系(true) or screen坐标系(false)
				 GlbPointGeo pGeoDirect,	//贴图方向的参考点
				 bool bDirOnGlobe,		//方向参考点实在 globe坐标系(true) or screen坐标系(false)
				 bool bHeadDirect,		//图片朝向参考点(ture) or 背向参考点(false)
				 float width,			//贴图的宽度(单位:角度)
				 float height,			//贴图的高度(单位:角度)
				 float layer,			//贴图所在的层
				 GlbTexMode mode,
				 GlbPoint3d &pClose		//返回贴图右上角的坐标
				 );

void DrawBelt(GLuint TextureName,
			  GlbPointGeo pGeo,
			  bool bPointOnGlobe,
			  GlbPointGeo pGeoDirect,	
			  bool bDirOnGlobe,		
			  bool bHeadDirect,		
			  float width,			
			  float height,			
			  float layer);

void DrawMovie(
				GLuint videoID,
				const char* videoPath,
				GlbPointGeo pGeo,
				bool bPointOnGlobe,
				GlbPointGeo pGeoDirect,	
				bool bDirOnGlobe,		
				bool bHeadDirect,		
				float width,			
				float height,			
				float layer,
				GlbPoint3d &pClose);

void ShowFrameRate();

#endif
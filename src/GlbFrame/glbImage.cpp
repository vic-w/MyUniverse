#include "glbImage.h"

namespace glb
{
	Image::Image(const char* filename)
	{
		imageID = glbLoadImage(filename, size);
	}

	void Image::Draw(const GlbRotmat &GlobeRotMat, const GlbCalib &calib, const CurveRect &curveRect, const int layer)
	{
		glbDrawTexture(
                    imageID,     //材质的编号
                    GlobeRotMat,
                    calib,
                    curveRect.pGeo,       //贴图的中心点
                    curveRect.bPointOnGlobe,     //中心点坐标是在 globe坐标系(true) or screen坐标系(false)
                    curveRect.pGeoDirect, //贴图方向的参考点
                    curveRect.bDirOnGlobe,       //方向参考点实在 globe坐标系(true) or screen坐标系(false)
                    curveRect.bHeadDirect,       //图片朝向参考点(ture) or 背向参考点(false)
                    curveRect.width,            //贴图的宽度(单位:角度)
                    curveRect.height,           //贴图的高度(单位:角度)
                    layer,            //贴图所在的层
                    curveRect.mode);
	}
}
#pragma once
#include "GlbCore.h"
#include "GlbRot.h"

namespace glb
{	class CurveRect
	{
	public:
		GlbTexMode mode;
		GlbPointGeo pGeo;       //贴图的中心点
		bool bPointOnGlobe;     //中心点坐标是在 globe坐标系(true) or screen坐标系(false)
		GlbPointGeo pGeoDirect; //贴图方向的参考点
		bool bDirOnGlobe;       //方向参考点实在 globe坐标系(true) or screen坐标系(false)
		bool bHeadDirect;       //图片朝向参考点(ture) or 背向参考点(false)
		float width;            //贴图的宽度(单位:角度)
		float height;           //贴图的高度(单位:角度)

	public:
		CurveRect(
			GlbTexMode mode, 
			GlbPointGeo pGeo,
			bool bPointOnGlobe,
			GlbPointGeo pGeoDirect,
			bool bDirOnGlobe,
			bool bHeadDirect,
			float width,
			float height);
	};
}
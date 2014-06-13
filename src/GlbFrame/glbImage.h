#pragma once

#include "GlbCore.h"
#include "opencv.hpp"
#include "glbCurveRect.h"

namespace glb
{
	class Image
	{
	private:
		GlbImage imageID;
		GlbSize size;
	public:
		Image(const char* filename);
		void Draw(const GlbRotmat &GlobeRotMat, const GlbCalib &calib, const CurveRect &curveRect, const int layer);
	};
}
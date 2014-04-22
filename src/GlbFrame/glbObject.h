#pragma once
#include "glbCurveRect.h"
#include "glbImage.h"

namespace glb
{

	class Object
	{
	private:
		CurveRect curveRect;
		Image image;
	public:
		Object(CurveRect curveRect, const char* imageFilename);
		virtual void Draw(const GlbRotmat &GlobeRotMat, const GlbCalib &calib, const int parentLayer);
	};



}
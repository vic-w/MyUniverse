#include "glbObject.h"

namespace glb
{
	Object::Object(CurveRect curveRect, const char* imageFilename): image(imageFilename)
	{
		this->curveRect.mode = curveRect.mode;
		this->curveRect.bDirOnGlobe = curveRect.bDirOnGlobe;
		this->curveRect.bHeadDirect = curveRect.bHeadDirect;
		this->curveRect.bPointOnGlobe = curveRect.bPointOnGlobe;
		this->curveRect.width = curveRect.width;
		this->curveRect.height = curveRect.height;
		this->curveRect.pGeo = curveRect.pGeo;
		this->curveRect.pGeoDirect = curveRect.pGeoDirect;
	}

	void Object::Draw(const GlbRotmat &GlobeRotMat, const GlbCalib &calib, const int parentLayer)
	{
		image.Draw(GlobeRotMat, calib, curveRect, parentLayer+1);
	}
}
#include "glbCurveRect.h"

namespace glb
{
	CurveRect::CurveRect(
			GlbTexMode mode, 
			GlbPointGeo pGeo,
			bool bPointOnGlobe,
			GlbPointGeo pGeoDirect,
			bool bDirOnGlobe,
			bool bHeadDirect,
			float width,
			float height)
	{
		this->mode = mode;
		this->pGeo = pGeo;
		this->bPointOnGlobe = bPointOnGlobe;
		this->pGeoDirect = pGeoDirect;
		this->bDirOnGlobe = bDirOnGlobe;
		this->bHeadDirect = bHeadDirect;
		this->width = width;
		this->height = height;
	}
}
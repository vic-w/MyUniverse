#pragma once
#include "glbObject.h"
#include "GlbRot.h"
#include <vector>

using namespace std;

namespace glb
{
	class Planet : public Container
	{
	private:
		GlbRotmat GlobeRotMat;
	public:
		Planet();
	};
}
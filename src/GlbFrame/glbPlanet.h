#pragma once
#include "glbContainer.h"


namespace glb
{
	class Container : public Object
	{
	private:
		vector <Object*> children; 
	public:
		Container();
		~Container();
	};
}
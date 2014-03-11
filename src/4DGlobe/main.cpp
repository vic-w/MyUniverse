#include "GlbFrame.h"
#include <cstdio>

using namespace glb;

void main()
{
	Object o;
	o.A = 0;
	o.B = 1;
	printf("%d\n",o.geta());
}

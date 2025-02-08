#ifndef J3D_GRAPH_BASE_COMPONENTS_GXCOLORS10
#define J3D_GRAPH_BASE_COMPONENTS_GXCOLORS10

#include <dolphin/gx/GXStruct.h>

class J3DGXColorS10 {
public:
	J3DGXColorS10() { }

	J3DGXColorS10& operator=(const GXColorS10& other)
	{
		color.r = other.r;
		color.g = other.g;
		color.b = other.b;
		color.a = other.a;
		return *this;
	}

	GXColorS10 color;
};

#endif

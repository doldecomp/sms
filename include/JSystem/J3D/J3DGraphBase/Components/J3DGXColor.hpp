#ifndef J3D_GRAPH_BASE_COMPONENTS_GXCOLOR
#define J3D_GRAPH_BASE_COMPONENTS_GXCOLOR

#include <dolphin/gx/GXStruct.h>

class J3DGXColor {
public:
	J3DGXColor() { }

	J3DGXColor& operator=(const GXColor& other)
	{
		color.r = other.r;
		color.g = other.g;
		color.b = other.b;
		color.a = other.a;
		return *this;
	}

	GXColor color;
};

#endif

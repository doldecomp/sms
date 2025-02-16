#ifndef J3D_GRAPH_BASE_COMPONENTS_GXCOLOR
#define J3D_GRAPH_BASE_COMPONENTS_GXCOLOR

#include <dolphin/gx/GXStruct.h>

class J3DGXColor {
public:
	J3DGXColor() { }
	J3DGXColor(const GXColor& other)
	{
		color.r = other.r;
		color.g = other.g;
		color.b = other.b;
		color.a = other.a;
	}
	J3DGXColor(const J3DGXColor& other)
	{
		color.r = other.color.r;
		color.g = other.color.g;
		color.b = other.color.b;
		color.a = other.color.a;
	}

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

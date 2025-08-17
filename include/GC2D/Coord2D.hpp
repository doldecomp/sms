#ifndef GC2D_COORD2D_HPP
#define GC2D_COORD2D_HPP

#include <JSystem/JGeometry/JGVec2.hpp>

class TCoord2D {
public:
	TCoord2D();
	void setValue(s32, f32, f32, f32, f32);
	bool update();
	template <class T> void roundf() const;

	int getSomeX() const { return unk0.x + (unk0.x > 0.0f ? 0.5f : -0.5f); }
	int getSomeY() const { return unk0.y + (unk0.y > 0.0f ? 0.5f : -0.5f); }

	/* 0x0 */ JGeometry::TVec2<f32> unk0;
	/* 0x8 */ JGeometry::TVec2<f32> unk8;
	/* 0x10 */ JGeometry::TVec2<f32> unk10;
};

#endif // GC2D_COORD2D_HPP

#ifndef MAP_MAP_COLLISION_PLANE_HPP
#define MAP_MAP_COLLISION_PLANE_HPP

#include <dolphin/types.h>

class TBGCheckData;

class TMapCheckGroundPlane {
public:
	TMapCheckGroundPlane();
	void init(int, int, f32);
	f32 checkPlaneGround(f32 x, f32 y, f32 z, const TBGCheckData**);
	TBGCheckData* getCheckData(int, int, int) const;

public:
	/* 0x0 */ int unk0;
	/* 0x4 */ int unk4;
	/* 0x8 */ f32 unk8;
	/* 0xC */ f32 mScale;
	/* 0x10 */ f32 unk10;
	/* 0x14 */ f32 mExtent;
	/* 0x18 */ TBGCheckData* unk18;
};

#endif

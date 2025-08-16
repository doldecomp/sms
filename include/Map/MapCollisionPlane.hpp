#ifndef MAP_MAP_COLLISION_PLANE_HPP
#define MAP_MAP_COLLISION_PLANE_HPP

#include <dolphin/types.h>

class TBGCheckData;

class TMapCheckGroundPlane {
public:
	TMapCheckGroundPlane();
	void init(int width, int height, f32 scale);
	f32 checkPlaneGround(f32 x, f32 y, f32 z, const TBGCheckData**);
	TBGCheckData* getCheckData(int x, int z, int which) const;

	f32 gridToWorld(f32 v) { return v * mScale - mExtent; }
	f32 worldToGrid(f32 v) { return mOneOverScale * (v + mExtent); }

public:
	/* 0x0 */ int mGridWidth;
	/* 0x4 */ int mGridHeight;
	/* 0x8 */ f32 mScale;
	/* 0xC */ f32 mOneOverScale;
	/* 0x10 */ f32 mGridWorldWidth;
	/* 0x14 */ f32 mExtent;
	/* 0x18 */ TBGCheckData* mGrid;
};

#endif

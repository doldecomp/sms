#ifndef MAP_MAP_COLLISION_PLANE_HPP
#define MAP_MAP_COLLISION_PLANE_HPP

#include <dolphin/types.h>

class TBGCheckData;

class TMapCheckGroundPlane {
public:
	TMapCheckGroundPlane();
	void init(int, int, f32);
	f32 checkPlaneGround(f32 x, f32 y, f32 z, const TBGCheckData**);
	void getCheckData(int, int, int) const;
};

#endif

#ifndef MAP_MAP_WIRE_HPP
#define MAP_MAP_WIRE_HPP

#include <JSystem/JGeometry/JGVec3.hpp>

class TCubeGeneralInfo;

class TMapWirePoint {
public:
	TMapWirePoint();
};

class TMapWire {
public:
	void drawLower() const;
	void drawUpper() const;
	void getPointPowerAtReleased(f32) const;
	void getPointPosAtReleased(f32, JGeometry::TVec3<f32>*) const;
	void updatePointAtReleased(int);
	void updateMovePointAtReleased();
	void initPointAtJustReleased(f32, TMapWirePoint*);
	void release();
	void getPointPosAtHanged(f32, JGeometry::TVec3<f32>*) const;
	void getPointInfoAtHanged(f32, TMapWirePoint*);
	void setFootPointsAtHanged(MtxPtr);
	void calcViewAndDBEntry();
	void move();
	f32 getPosInWire(const JGeometry::TVec3<f32>&) const;
	void getPointPosOnLine(f32, JGeometry::TVec3<f32>*) const;
	void getPointPosOnWire(f32, JGeometry::TVec3<f32>*) const;
	void getPointPosDefault(f32, JGeometry::TVec3<f32>*) const;
	void initTipPoints(const TCubeGeneralInfo*);
	void init(const TCubeGeneralInfo*);
	TMapWire();

	static f32 mDrawWidth;
	static f32 mDrawHeight;
};

#endif

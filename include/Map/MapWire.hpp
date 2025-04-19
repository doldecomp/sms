#ifndef MAP_MAP_WIRE_HPP
#define MAP_MAP_WIRE_HPP

#include <JSystem/JGeometry/JGVec3.hpp>

class TCubeGeneralInfo;

class TMapWirePoint {
public:
	TMapWirePoint();
};

class TMapWire {
	void drawLower() const;
	void drawUpper() const;
	void getPointPowerAtReleased(float) const;
	void getPointPosAtReleased(float, JGeometry::TVec3<float>*) const;
	void updatePointAtReleased(int);
	void updateMovePointAtReleased();
	void initPointAtJustReleased(float, TMapWirePoint*);
	void release();
	void getPointPosAtHanged(float, JGeometry::TVec3<float>*) const;
	void getPointInfoAtHanged(float, TMapWirePoint*);
	void setFootPointsAtHanged(float (*)[4]);
	void calcViewAndDBEntry();
	void move();
	void getPosInWire(const JGeometry::TVec3<float>&) const;
	void getPointPosOnLine(float, JGeometry::TVec3<float>*) const;
	void getPointPosOnWire(float, JGeometry::TVec3<float>*) const;
	void getPointPosDefault(float, JGeometry::TVec3<float>*) const;
	void initTipPoints(const TCubeGeneralInfo*);
	void init(const TCubeGeneralInfo*);
	TMapWire();
};

#endif

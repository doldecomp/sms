#ifndef ENEMY_BOSS_HANACHAN_SUB_HPP
#define ENEMY_BOSS_HANACHAN_SUB_HPP

#include <JSystem/JGeometry/JGVec3.hpp>

class TSpherePoint {
public:
	TSpherePoint() { }

	/* 0x0 */ JGeometry::TVec3<f32> mPreviousPosition;
	/* 0xC */ JGeometry::TVec3<f32> mPosition;
	/* 0x18 */ JGeometry::TVec3<f32> mVelocity;
	/* 0x24 */ f32 mLength;
	/* 0x28 */ f32 mDegreeZ;
};

class TSphereLink {
public:
	TSphereLink(u16, const JGeometry::TVec3<f32>&, f32, f32, f32, f32, f32,
	            f32);
	BOOL setDegreeZAndRevisionPosXZ(int, f32);
	void moveHead(const JGeometry::TVec3<f32>&);
	void execMapCollision_(JGeometry::TVec3<f32>*);

	/* 0x0 */ u16 mPointCount;
	/* 0x4 */ TSpherePoint* mPoints;
	/* 0x8 */ f32 mVelocityScale;
	/* 0xC */ f32 mGravity;
	/* 0x10 */ f32 mCollisionRadius;
	/* 0x14 */ f32 mRotationMoveScale;
	/* 0x18 */ f32 mHeadDegreeY;
};

void BHSCalcRevisionDistXZByRotateZ(f32, f32, f32, f32*, f32*);
f32 BHSCalcCentrifugalForce(const JGeometry::TVec3<f32>&,
                          const JGeometry::TVec3<f32>&,
                          const JGeometry::TVec3<f32>&, f32);

#endif

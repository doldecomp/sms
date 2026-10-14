#ifndef ENEMY_BOSS_HANACHAN_SUB_HPP
#define ENEMY_BOSS_HANACHAN_SUB_HPP

#include <JSystem/JGeometry.hpp>

struct TSpherePoint {
	/* 0x0 */ JGeometry::TVec3<f32> mPreviousPosition;
	/* 0xC */ JGeometry::TVec3<f32> mPosition;
	/* 0x18 */ JGeometry::TVec3<f32> mVelocity;
	/* 0x24 */ f32 mLinkLength;
	/* 0x28 */ f32 mDegreeZ;
};

class TSphereLink {
public:
	TSphereLink(u16 count, const JGeometry::TVec3<f32>& position,
	            f32 linkLength, f32 collisionRadius, f32 velocityDamping,
	            f32 gravity, f32 revisionScale, f32 initialDirection);
	void moveHead(const JGeometry::TVec3<f32>& position);
	BOOL setDegreeZAndRevisionPosXZ(int index, f32 degreeZ);

private:
	void execMapCollision_(JGeometry::TVec3<f32>* position);

public:
	/* 0x0 */ u16 mPointCount;
	/* 0x4 */ TSpherePoint* mPoints;
	/* 0x8 */ f32 mVelocityDamping;
	/* 0xC */ f32 mGravity;
	/* 0x10 */ f32 mCollisionRadius;
	/* 0x14 */ f32 mRevisionScale;
	/* 0x18 */ f32 mInitialDirection;
};

void BHSCalcRevisionDistXZByRotateZ(f32 direction, f32 degreeZ,
                                    f32 revisionScale, f32* x, f32* z);
f32 BHSCalcCentrifugalForce(const JGeometry::TVec3<f32>& head,
                            const JGeometry::TVec3<f32>& current,
                            const JGeometry::TVec3<f32>& previous,
                            f32 direction);

#endif

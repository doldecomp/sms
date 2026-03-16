#include <Player/MarioMain.hpp>

#include <Map/Map.hpp>
#include <JSystem/JMath.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionData.hpp>
#include <Strategic/LiveActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSoundBGM.hpp>
#include <dolphin/mtx.h>
#include <fake_tgmath.h>

#pragma dont_inline on
// playerRefrection: reflect facing angle off wall, optionally negate velocity
void TMario::playerRefrection(int param)
{
	if (mWallPlane != NULL) {
		const JGeometry::TVec3<f32>& normal = mWallPlane->getNormal();
		s16 wallAngle = matan(normal.z, normal.x);
		s16 diff = (s16)(mFaceAngle.y - wallAngle);
		mFaceAngle.y = (s16)(wallAngle - diff);
	}

	if (param != 0) {
		setPlayerVelocity(-mForwardVel);
	} else {
		mFaceAngle.y = mFaceAngle.y + 0x8000;
	}
}
#pragma dont_inline off

// keepDistance(TVec3, f32, f32): push Mario away from a reference point
void TMario::keepDistance(const JGeometry::TVec3<f32>& refPos, f32 radius,
                          f32 param)
{
	f32 dz = mPosition.z - refPos.z;
	f32 dx = mPosition.x - refPos.x;
	f32 totalDist = param + (radius + unk15C);

	f32 sqDist = dz * dz + dx * dx;
	f32 dist;
	if (sqDist > 0.0f) {
		double g = __frsqrte((double)sqDist);
		g = .5 * g * (3.0 - g * g * sqDist);
		dist = (f32)(sqDist * g);
	} else {
		dist = sqDist;
	}

	if (dist == 0.0f) {
		dx = 1.0f;
		dist = dx;
	}

	if (dist >= totalDist)
		return;

	if (onYoshi() == 0) {
		f32* unk8E8 = (f32*)((u8*)this + 0x8E8);
		u8 flag = 0;
		if (mForwardVel > *unk8E8)
			flag = 1;

		f32 vx = mVel.x;
		f32 vz = mVel.z;
		f32 speed = sqrtf(vx * vx + vz * vz);

		if (speed > *unk8E8)
			flag = 1;

		if ((u8)flag == 1) {
			emitParticle(12);
			changePlayerDropping(0x208B0, 0);
			return;
		}
	}

	s16 angle;
	if (0.0f == dist) {
		angle = mFaceAngle.y;
	} else {
		angle = matan(dz, dx);
	}

	Vec newPos;
	newPos.x = refPos.x + totalDist * JMASSin(angle);
	newPos.y = mPosition.y;
	newPos.z = refPos.z + totalDist * JMASCos(angle);

	checkWallPlane(&newPos, 60.0f, unk15C);

	f32 groundY;
	const TBGCheckData* groundPlane;
	checkGroundPlane(newPos.x, newPos.y, newPos.z, &groundY, &groundPlane);

	u8 isIllegal;
	if (groundPlane->mFlags & 0x10)
		isIllegal = 1;
	else
		isIllegal = 0;

	u8 isValid;
	if (isIllegal == 1)
		isValid = 0;
	else
		isValid = 1;
	if (!isValid)
		return;

	JGeometry::TVec3<f32> diff(newPos);
	diff.sub(mPosition);

	JGeometry::TVec3<f32> push(diff);
	f32 sq = push.x * push.x + push.y * push.y + push.z * push.z;
	f32 len = JGeometry::TUtil<f32>::sqrt(sq);

	f32 pushDist = len;
	if (len <= 0.0f)
		return;

	if (50.0f < len)
		pushDist = 50.0f;

	if (sq <= JGeometry::TUtil<f32>::epsilon()) {
		push.x = 0.0f;
		push.y = 0.0f;
		push.z = 0.0f;
	} else {
		f32 invLen = JGeometry::TUtil<f32>::inv_sqrt(sq);
		f32 scale = 1.0f * invLen;
		push.x *= scale;
		push.y *= scale;
		push.z *= scale;
	}

	push.scale(pushDist);

	JGeometry::TVec3<f32> offset(push);
	mPosition.x += offset.x;
	mPosition.y += offset.y;
	mPosition.z += offset.z;
}

// keepDistance(THitActor, f32): thin wrapper that extracts position and radius
void TMario::keepDistance(const THitActor& actor, f32 param)
{
	keepDistance(*(const JGeometry::TVec3<f32>*)&actor.mPosition,
	            *(f32*)((u8*)&actor + 0x58), param);
}

// checkDescent: check if Mario should start descending a slope
void TMario::checkDescent()
{
	f32 descentSp = mHangingParams.mDescentSp.value;

	u8 canDescent = 0;
	if (mHeldObject == NULL) {
		if (!onYoshi())
			canDescent = 1;
	}

	if (canDescent != 1)
		return;

	if (!(mForwardVel < descentSp))
		return;

	u8 isOnIllegal = 1;
	u8 zero = 0;

	f32 posY = mPosition.y;
	TBGWallCheckRecord wallRecord;
	wallRecord.mCenter.x = mPosition.x;
	wallRecord.mCenter.y = posY - 10.0f;
	wallRecord.mCenter.z = mPosition.z;
	wallRecord.mRadius = descentSp;
	wallRecord.mMaxResults = isOnIllegal;
	wallRecord.mFlags = zero;

	if (!gpMap->isTouchedWallsAndMoveXZ(&wallRecord))
		return;

	f32 groundY;
	const TBGCheckData* groundPlane;
	checkGroundPlane(wallRecord.mCenter.x, 30.0f + mPosition.y, wallRecord.mCenter.z, &groundY, &groundPlane);

	if (groundPlane->mFlags & 0x10) {
	} else {
		isOnIllegal = zero;
	}

	if (isOnIllegal)
		return;

	if (!(mPosition.y - groundY > 160.0f))
		return;

	const TBGCheckData* wallData = wallRecord.mResultWalls[0];
	s16 wallAngle = matan(wallData->mNormal.z, wallData->mNormal.x);
	s16 diff = (s16)(wallAngle - mFaceAngle.y);

	if (diff <= -16384)
		return;
	if (diff >= 16384)
		return;

	f32 push = 20.0f + descentSp;
	mPosition.x = wallRecord.mCenter.x - push * wallData->mNormal.x;
	mPosition.z = wallRecord.mCenter.z - push * wallData->mNormal.z;
	mFaceAngle.y = (s16)(wallAngle + 0x8000);
	changePlayerStatus(0x3000054E, 0, false);
	setAnimation(28, 1.0f);
}

// checkGroundAtWalking: check ground collision during walk movement
int TMario::checkGroundAtWalking(Vec* pos)
{
	const TBGCheckData* roofPlane;
	const TBGCheckData* groundPlane;
	f32 groundY;

	checkWallPlane(pos, 30.0f, 0.5f * unk15C);
	const TBGCheckData* wallPlane = checkWallPlane(pos, 60.0f, unk15C);

	u8 isOnWater;
	if (mAction & 0x10000)
		isOnWater = 1;
	else
		isOnWater = 0;

	if (isOnWater) {
		groundY = gpMap->checkGround(pos->x, 30.0f + pos->y, pos->z, &groundPlane);
	} else {
		checkGroundPlane(pos->x, 30.0f + pos->y, pos->z, &groundY, &groundPlane);
	}

	f32 roofY = gpMap->checkRoof(pos->x, 80.0f + mPosition.y, pos->z, &roofPlane);

	mWallPlane = wallPlane;

	u8 isIllegal;
	if (groundPlane->mFlags & 0x10)
		isIllegal = 1;
	else
		isIllegal = 0;
	if (isIllegal)
		return 2;

	if (160.0f + pos->y >= roofY)
		return 2;

	if (pos->y > 100.0f + groundY) {
		mPosition.x = pos->x;
		mPosition.y = pos->y;
		mPosition.z = pos->z;
		mGroundPlane = groundPlane;
		*(f32*)((u8*)this + 0xEC) = groundY;
		return 0;
	}

	if (fabsf(mPosition.y - groundY) > 100000.0f) {
		mPosition = unk29C;
	} else {
		mPosition.x = pos->x;
		mPosition.y = groundY;
		mPosition.z = pos->z;
		mGroundPlane = groundPlane;
		*(f32*)((u8*)this + 0xEC) = groundY;
	}

	if (wallPlane != NULL) {
		u16 bgType = wallPlane->mBGType;
		u8 isThrough;
		if (bgType == 0x400
		    || bgType == 0x8400
		    || (u16)(bgType - 0x100) <= 3
		    || (u16)(bgType - 0x800) <= 1
		    || bgType == 0x201
		    || bgType == 0x203)
			isThrough = 1;
		else
			isThrough = 0;
		if (!isThrough) {
			s16 wallAngle = matan(wallPlane->mNormal.z, wallPlane->mNormal.x);
			s16 diff = wallAngle - mFaceAngle.y;

			if (diff >= 0x2AAA && diff <= 0x5555)
				return 1;

			if (diff <= -0x2AAA && diff >= -0x5555)
				return 1;

			return 3;
		}
	}

	return 1;
}

// stopProcess: stop all movement, snap to floor
void TMario::stopProcess()
{
	setPlayerVelocity(0.0f);
	mVel.y = 0.0f;
	mPosition.y = mFloorPosition.y;
	mFaceAngle.x = 0;
	mModelFaceAngle = mFaceAngle.y;
}

// waitProcess: idle standing physics
int TMario::waitProcess()
{
	setPlayerVelocity(0.0f);

	f32 posY = mPosition.y;
	f32 floorY = mFloorPosition.y;
	if (fabsf(posY - floorY) > 4.0f) {
		mPosition = unk29C;
		changePlayerStatus(0x088D, 0, false);
	} else {
		mPosition.y = floorY;
	}

	mFaceAngle.x = 0;
	mModelFaceAngle = mFaceAngle.y;

	const TBGCheckData* ground = mGroundPlane;
	u8 isIllegal;
	if (ground->mFlags & 0x10)
		isIllegal = 1;
	else
		isIllegal = 0;
	if (isIllegal)
		return 2;

	f32 normalY = 0.0f;
	if (ground != NULL)
		normalY = ground->mNormal.y;

	Vec nextPos;
	nextPos.x = mPosition.x + mVel.x * 1.0f * normalY;
	nextPos.z = mPosition.z + mVel.z * 1.0f * normalY;
	nextPos.y = mPosition.y;

	int result = checkGroundAtWalking(&nextPos);
	mFaceAngle.x = 0;
	if (result == 3)
		return 2;
	mModelFaceAngle = mFaceAngle.y;
	return result;
}

// walkProcess: walking physics
int TMario::walkProcess()
{
	const TBGCheckData* ground = mGroundPlane;
	u8 isIllegal;
	if (ground->mFlags & 0x10)
		isIllegal = 1;
	else
		isIllegal = 0;
	if (isIllegal)
		return 2;

	f32 normalY = 0.0f;
	if (ground != NULL)
		normalY = ground->mNormal.y;

	Vec nextPos;
	nextPos.x = mPosition.x + mVel.x * 1.0f * normalY;
	nextPos.z = mPosition.z + mVel.z * 1.0f * normalY;
	nextPos.y = mPosition.y;

	int result = checkGroundAtWalking(&nextPos);
	mFaceAngle.x = 0;
	if (result == 3)
		return 2;
	mModelFaceAngle = mFaceAngle.y;
	return result;
}

// barProcess: pole/bar climbing physics
int TMario::barProcess()
{
	int result = 0;
	const TBGCheckData* groundPlane;
	const TBGCheckData* roofPlane;
	f32 groundY;

	Vec pos;
	pos.x = mHolder->mPosition.x;
	pos.y = mPosition.y;
	pos.z = mHolder->mPosition.z;

	const TBGCheckData* wall1 = checkWallPlane(&pos, 60.0f, 43.0f);
	const TBGCheckData* wall2 = checkWallPlane(&pos, 30.0f, 24.0f);

	int isThrough1 = 0;
	int isThrough2 = 0;

	if (wall1 == NULL) {
		isThrough1 = 1;
	} else {
		u16 bgType = wall1->mBGType;
		u8 isMarioThru;
		if (bgType == 0x400
		    || bgType == 0x8400
		    || (u16)(bgType - 0x100) <= 3
		    || (u16)(bgType - 0x800) <= 1
		    || bgType == 0x201
		    || bgType == 0x203)
			isMarioThru = 1;
		else
			isMarioThru = 0;
		if (isMarioThru)
			isThrough1 = 1;
	}

	if (wall2 == NULL) {
		isThrough2 = 1;
	} else {
		u16 bgType = wall2->mBGType;
		u8 isMarioThru;
		if (bgType == 0x400
		    || bgType == 0x8400
		    || (u16)(bgType - 0x100) <= 3
		    || (u16)(bgType - 0x800) <= 1
		    || bgType == 0x201
		    || bgType == 0x203)
			isMarioThru = 1;
		else
			isMarioThru = 0;
		if (isMarioThru)
			isThrough2 = 1;
	}

	if (isThrough1 == 1 && isThrough2 == 1) {
		mPosition.x = mHolder->mPosition.x;
		mPosition.y = mHolder->mPosition.y + mHolderHeightDiff;
		mPosition.z = mHolder->mPosition.z;
	} else {
		*(Vec*)&mPosition = pos;
	}

	f32 roofY = gpMap->checkRoof(mPosition.x, 80.0f + mPosition.y, mPosition.z, &roofPlane);
	if (mPosition.y > roofY - 160.0f) {
		mPosition.y = roofY - 160.0f;
	}

	checkGroundPlane(mPosition.x, mPosition.y, mPosition.z, &groundY, &groundPlane);

	if (mPosition.y < groundY) {
		mPosition.y = groundY;
		changePlayerStatus(0x0C400201, 0, false);
		result = 1;
	}

	setPlayerVelocity(0.0f);
	mFaceAngle.x = 0;
	mModelFaceAngle = mFaceAngle.y;

	return result;
}

// hangonCheck: check if Mario can hang onto a surface
int TMario::hangonCheck(const TBGCheckData* checkData, const Vec& pos1,
                        const Vec& pos2)
{
	if (mVel.y > 0.0f)
		return 0;

	if ((pos2.x - pos1.x) * mVel.x + (pos2.z - pos1.z) * mVel.z > 0.0f)
		return 0;

	Vec newPos;
	f32 _pad[7];
	const TBGCheckData* groundPlane;
	(void)_pad;
	newPos.x = pos2.x - 60.0f * checkData->mNormal.x;
	newPos.z = pos2.z - 60.0f * checkData->mNormal.z;
	checkGroundPlane(newPos.x, 160.0f + pos2.y, newPos.z, &newPos.y,
	                 &groundPlane);

	if (newPos.y - pos2.y <= 100.0f)
		return 0;

	if (mFloorPosition.x < 160.0f + newPos.y)
		return 0;

	u16 bgType = groundPlane->mBGType;

	u8 isWater;
	if (bgType == 0x0001 || bgType == 0x4001 || bgType == 0x8001
	    || bgType == 0xC001)
		isWater = 1;
	else
		isWater = 0;
	if (isWater)
		return 0;

	u8 isType6;
	if (bgType == 0x0006 || bgType == 0x4006 || bgType == 0x8006
	    || bgType == 0xC006)
		isType6 = 1;
	else
		isType6 = 0;
	if (isType6)
		return 0;

	mPosition.x = newPos.x;
	mPosition.y = newPos.y;
	mPosition.z = newPos.z;
	mGroundPlane = groundPlane;
	*(f32*)((u8*)this + 0xEC) = newPos.y;

	const JGeometry::TVec3<f32>& normal = groundPlane->getNormal();
	mSlopeAngle = matan(normal.z, normal.x);

	s16 angle = matan(checkData->mNormal.z, checkData->mNormal.x);
	mFaceAngle.y = (s16)(angle + 0x8000);

	return 1;
}

// checkGroundAtJumping: ground collision during airborne movement
int TMario::checkGroundAtJumping(const Vec& pos, int flags)
{
	Vec stackPos;
	((u32*)&stackPos)[0] = ((const u32*)&pos)[0];
	((u32*)&stackPos)[1] = ((const u32*)&pos)[1];
	((u32*)&stackPos)[2] = ((const u32*)&pos)[2];

	const TBGCheckData* wall1 = checkWallPlane(&stackPos, 30.0f, unk15C);
	const TBGCheckData* wall2 = checkWallPlane(&stackPos, 60.0f, unk15C);

	u8 isOnWater;
	if (mAction & 0x10000)
		isOnWater = 1;
	else
		isOnWater = 0;

	if (isOnWater) {
		*(f32*)((u8*)this + 0xEC) = gpMap->checkGround(stackPos.x, 30.0f + stackPos.y, stackPos.z, &mGroundPlane);
	} else {
		checkGroundPlane(stackPos.x, 30.0f + stackPos.y, stackPos.z, (f32*)((u8*)this + 0xEC), &mGroundPlane);
	}

	mFloorPosition.x = checkRoofPlane(stackPos, 80.0f + mPosition.y, &mRoofPlane);

	int groundResult = 7;
	int hangResult = 7;
	int slopeResult = 7;

	mPosition.x = stackPos.x;
	mPosition.y = stackPos.y;
	mPosition.z = stackPos.z;

	u8 isIllegal;
	if (mGroundPlane->mFlags & 0x10)
		isIllegal = 1;
	else
		isIllegal = 0;

	if (isIllegal) {
		mPosition = unk29C;
		groundResult = 2;
	} else {
		u16 bgType = mGroundPlane->mBGType;
		int isWater = 0;

		u8 isMarioThrough;
		if (bgType == 0x400
		    || bgType == 0x8400
		    || (u16)(bgType - 0x100) <= 3
		    || (u16)(bgType - 0x800) <= 1
		    || bgType == 0x201
		    || bgType == 0x203)
			isMarioThrough = 1;
		else
			isMarioThrough = 0;
		if (isMarioThrough)
			isWater = 1;

		if ((mAction - 0x800000) == 0x08A9) {
			u8 isType107;
			if (bgType == 0x107)
				isType107 = 1;
			else
				isType107 = 0;
			if (isType107)
				isWater = 1;
		}

		u8 isActionWater;
		if (mAction & 0x10000)
			isActionWater = 1;
		else
			isActionWater = 0;

		if (isActionWater) {
			u8 isWaterSurface;
			if (bgType == 0x100
			    || bgType == 0x101
			    || (u16)(bgType - 258) <= 3
			    || bgType == 0x4104)
				isWaterSurface = 1;
			else
				isWaterSurface = 0;
			if (isWaterSurface)
				isWater = 0;
		}

		if (isWater == 0) {
			if (stackPos.y <= *(f32*)((u8*)this + 0xEC)) {
				mPosition.y = *(f32*)((u8*)this + 0xEC);
				groundResult = 1;
			}
		}
	}

	if (groundResult == 1) {
		*(f32*)((u8*)this + 0xBC) = mVel.y;
	}

	if (mRoofPlane != NULL) {
		u16 roofType = mRoofPlane->mBGType;
		u8 isRoofThrough;
		if (roofType == 0x400
		    || roofType == 0x8400
		    || (u16)(roofType - 0x100) <= 3
		    || (u16)(roofType - 0x800) <= 1
		    || roofType == 0x201
		    || roofType == 0x203)
			isRoofThrough = 1;
		else
			isRoofThrough = 0;

		if (!isRoofThrough) {
			if (160.0f + stackPos.y > mFloorPosition.x) {
				mPosition.y = mFloorPosition.x - 160.0f;

				if (mRoofPlane->mActor != NULL) {
					((TLiveActor*)mRoofPlane->mActor)->receiveMessage((THitActor*)this, 2);
				}

				setPlayerVelocity(0.0f);

				if (mVel.y >= 0.0f) {
					mVel.y = 0.0f;

					u8 hasFlag2;
					if (unk118 & 2)
						hasFlag2 = 1;
					else
						hasFlag2 = 0;
					if (hasFlag2) {
						unk118 |= 0x200;
					}

					if (flags & 2) {
						u8 canClimb = 0;
						if (mHeldObject == NULL) {
							if (!onYoshi())
								canClimb = 1;
						}

						if (canClimb) {
							u8 isFence;
							if (mRoofPlane->mBGType == 0x10A)
								isFence = 1;
							else
								isFence = 0;
							if (isFence) {
								slopeResult = 4;
								goto endSlopeCheck;
							}
						}
					}

					slopeResult = 0;
				endSlopeCheck:;
				}
			}
		}
	}

	mWallPlane = (const TBGCheckData*)NULL;
	int isWall1Water = 0;
	int isWall2Water = 0;

	if (wall1 == NULL) {
		isWall1Water = 1;
	} else {
		u16 bgType = wall1->mBGType;
		u8 isMarioThru;
		if (bgType == 0x400
		    || bgType == 0x8400
		    || (u16)(bgType - 0x100) <= 3
		    || (u16)(bgType - 0x800) <= 1
		    || bgType == 0x201
		    || bgType == 0x203)
			isMarioThru = 1;
		else
			isMarioThru = 0;
		if (isMarioThru)
			isWall1Water = 1;
	}

	if (wall2 == NULL) {
		isWall2Water = 1;
	} else {
		u16 bgType = wall2->mBGType;
		u8 isMarioThru;
		if (bgType == 0x400
		    || bgType == 0x8400
		    || (u16)(bgType - 0x100) <= 3
		    || (u16)(bgType - 0x800) <= 1
		    || bgType == 0x201
		    || bgType == 0x203)
			isMarioThru = 1;
		else
			isMarioThru = 0;
		if (isMarioThru)
			isWall2Water = 1;
	}

	if ((flags & 1) && isWall1Water == 1 && isWall2Water == 0) {
		u8 canHang = 0;
		if (mHeldObject == NULL) {
			if (!onYoshi())
				canHang = 1;
		}

		if ((u8)canHang == 1) {
			mWallPlane = wall2;
			if (hangonCheck(wall2, pos, stackPos)) {
				hangResult = 3;
			} else {
				hangResult = 0;
			}
		}
	} else {
		if (isWall1Water != 0 && isWall2Water != 0) {
			// both water, skip
		} else {
			const TBGCheckData* wall;
			if (wall1 != NULL)
				wall = wall1;
			else
				wall = wall2;
			mWallPlane = wall;

			const JGeometry::TVec3<f32>& normal = mWallPlane->getNormal();
			int wallAngle = matan(normal.z, normal.x);
			s16 limit = *(s16*)((u8*)this + 0xE60);
			s16 diff = (s16)(wallAngle - (mFaceAngle.y + 0x8000));

			if (-limit < diff && diff < limit) {
				hangResult = 2;
			}
		}
	}

	if (groundResult != 7)
		return groundResult;
	if (slopeResult != 7)
		return slopeResult;
	if (hangResult != 7)
		return hangResult;
	return 0;
}

// fallProcess: apply gravity and air physics
void TMario::fallProcess()
{
	u32 action = mAction;

	if (action == 0x0891) {
		mVel.y -= *(f32*)((u8*)this + 0x21B0);
		if (mVel.y < -75.0f) {
			mVel.y = -75.0f;
		}
		return;
	}

	u8 inAir;
	if (unk78 & 0x100)
		inAir = 1;
	else
		inAir = 0;

	u8 shouldBrake;
	if (!inAir) {
		shouldBrake = 0;
	} else if (action & 0x00021000) {
		shouldBrake = 0;
	} else {
		if (!(mInput & 0x80) && mVel.y > 20.0f)
			shouldBrake = 1;
		else
			shouldBrake = 0;
	}

	if (shouldBrake) {
		mVel.y *= 0.75f;
	} else {
		u32 diff = action - 0x0895;
		if (diff <= 1 && mVel.y < 0.0f) {
			mVel.y -= mJumpParams.mJumpAccelControl.value;
		} else {
			mVel.y -= mJumpParams.mJumpSpeedBrake.value;
		}
	}

	if (onYoshi()) {
		mYoshi->thinkHoldOut();
	}

	if (mVel.y < -75.0f) {
		mVel.y = -75.0f;
	}
}

// jumpProcess: process jump movement each frame
int TMario::jumpProcess(int param)
{
	int result = 0;

	f32 velX = mVel.x;
	f32 velZ = mVel.z;
	f32 speed = sqrtf(velX * velX + velZ * velZ);

	f32 maxSpeed = mJumpParams.mJumpSlideControl.value;
	if (speed > maxSpeed) {
		mVel.x = mVel.x * (maxSpeed / speed);
		mVel.z = mVel.z * (mJumpParams.mJumpSlideControl.value / speed);
	}

	f32 factor = 0.25f;
	Vec nextPos;
	nextPos.x = mPosition.x + factor * mVel.x;
	nextPos.y = mPosition.y + factor * mVel.y;
	nextPos.z = mPosition.z + factor * mVel.z;

	int groundResult = checkGroundAtJumping(nextPos, param);
	if (groundResult != 0) {
		result = groundResult;
	}

	if (mVel.y >= 0.0f) {
	} else {
		*(f32*)((u8*)this + 0x104) = mPosition.y;
	}

	fallProcess();

	if (mAction != 0x80088A) {
		mFaceAngle.x = 0;
	}

	mModelFaceAngle = mFaceAngle.y;

	return result;
}

#include <Player/MarioMain.hpp>
#include <Player/Yoshi.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionData.hpp>
#include <math.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

void TMario::playerRefrection(int param_1)
{
	if (mWallPlane != nullptr) {
		s16 ang = matan(mWallPlane->getNormal().z, mWallPlane->getNormal().x);
		mFaceAngle.y = ang - (s16)(mFaceAngle.y - ang);
	}
	if (param_1 != 0) {
		setPlayerVelocity(-mForwardVel);
	} else {
		mFaceAngle.y += 0x8000;
	}
}

void TMario::keepDistance(const JGeometry::TVec3<f32>& target, f32 param_2,
                          f32 param_3)
{
	f32 dz = mPosition.z - target.z;
	f32 dx = mPosition.x - target.x;

	f32 thresh = param_3 + (param_2 + unk15C);
	f32 dist   = MsSqrtf(dx * dx + dz * dz);

	if (dist == 0.0f)
		dist = 1.0f;

	if (!(dist < thresh))
		return;

	if (!onYoshi()) {
		bool burst = false;
		if (mForwardVel > mDeParams.mClashSpeed.get())
			burst = true;

		f32 velSq = std::sqrtf(mVel.x * mVel.x + mVel.z * mVel.z);

		if (velSq > mDeParams.mClashSpeed.get())
			burst = true;

		if (burst == true) {
			emitParticle(0xc);
			changePlayerDropping(STATUS_JUMP_SHORT_BACK_DOWN, 0);
			return;
		}
	}

	s16 angle;
	if (dist == 0.0f) {
		angle = mFaceAngle.y;
	} else {
		angle = matan(dz, dx);
	}

	JGeometry::TVec3<f32> newPos;
	newPos.x = target.x + thresh * JMASSin(angle);
	newPos.y = mPosition.y;
	newPos.z = target.z + thresh * JMASCos(angle);

	checkWallPlane(&newPos, 60.0f, unk15C);
	f32 floorY;
	const TBGCheckData* ground;
	checkGroundPlane(newPos.x, newPos.y, newPos.z, &floorY, &ground);
	if (!ground->isLegal())
		return;

	JGeometry::TVec3<f32> diff = newPos - mPosition;

	f32 step = diff.length();
	if (50.0f < step)
		step = 50.0f;

	diff.normalize();

	mPosition += diff * step;
}

void TMario::keepDistance(const THitActor& actor, f32 param_2)
{
	keepDistance(actor.getPosition(), actor.getDamageRadius(), param_2);
}

void TMario::checkDescent()
{
	bool active   = false;
	f32 descentSp = mHangingParams.mDescentSp.get();
	if (mHeldObject == nullptr && !onYoshi())
		active = true;

	if (active != true)
		return;

	if (!(mForwardVel < descentSp))
		return;

	TBGWallCheckRecord rec(mPosition.x, mPosition.y - 10.0f, mPosition.z,
	                       descentSp, 1, 0);
	if (!gpMap->isTouchedWallsAndMoveXZ(&rec))
		return;

	f32 floorY;
	const TBGCheckData* ground;
	checkGroundPlane(rec.mCenter.x, mPosition.y + 30.0f, rec.mCenter.z, &floorY,
	                 &ground);

	if (ground->checkFlag(BG_CHECK_FLAG_ILLEGAL))
		return;

	if (!(mPosition.y - floorY > 160.0f))
		return;

	const TBGCheckData* wall = rec.mResultWalls[0];

	s16 wallAng = matan(wall->mNormal.z, wall->mNormal.x);
	s16 diff    = wallAng - mFaceAngle.y;
	if (diff > -0x4000 && diff < 0x4000) {
		f32 dist     = 20.0f + descentSp;
		mPosition.x  = rec.mCenter.x - dist * wall->mNormal.x;
		mPosition.z  = rec.mCenter.z - dist * wall->mNormal.z;
		mFaceAngle.y = wallAng + 0x8000;
		changePlayerStatus(0x3000054e, 0, false);
		setAnimation(0x1c, 1.0f);
	}
}

int TMario::checkGroundAtWalking(Vec* v)
{
	checkWallPlane(v, 30.0f, 0.5f * unk15C);
	TBGCheckData* wall = checkWallPlane(v, 60.0f, unk15C);

	f32 floorY;
	const TBGCheckData* ground;
	if (checkStatusFlag(0x10000)) {
		floorY = gpMap->checkGround(v->x, v->y + 30.0f, v->z, &ground);
	} else {
		checkGroundPlane(v->x, v->y + 30.0f, v->z, &floorY, &ground);
	}

	const TBGCheckData* roof;
	f32 roofY  = gpMap->checkRoof(v->x, mPosition.y + 80.0f, v->z, &roof);
	mWallPlane = wall;

	if (ground->checkFlag(BG_CHECK_FLAG_ILLEGAL))
		return 2;
	if (v->y + 160.0f >= roofY)
		return 2;

	if (v->y > floorY + 100.0f) {
		mPosition.x      = v->x;
		mPosition.y      = v->y;
		mPosition.z      = v->z;
		mGroundPlane     = ground;
		mFloorPosition.y = floorY;
		return 0;
	}

	if (fabsf(mPosition.y - floorY) > 100000.0f) {
		mPosition = unk29C;
	} else {
		mPosition.x      = v->x;
		mPosition.y      = floorY;
		mPosition.z      = v->z;
		mGroundPlane     = ground;
		mFloorPosition.y = floorY;
	}

	if (wall != nullptr && !wall->isMarioThrough()) {
		s16 diff
		    = matan(wall->getNormal().z, wall->getNormal().x) - mFaceAngle.y;
		if (diff >= 0x2aaa && diff <= 0x5555)
			return 1;
		if (diff <= -0x2aaa && diff >= -0x5555)
			return 1;
		return 3;
	}

	return 1;
}

void TMario::stopProcess()
{
	setPlayerVelocity(0.0f);
	mVel.y          = 0.0f;
	mPosition.y     = mFloorPosition.y;
	mFaceAngle.x    = 0;
	mModelFaceAngle = mFaceAngle.y;
}

int TMario::waitProcess()
{
	setPlayerVelocity(0.0f);
	if (fabsf(mPosition.y - mFloorPosition.y) > 100000.0f) {
		mPosition = unk29C;
		changePlayerStatus(0x88d, 0, false);
	} else {
		mPosition.y = mFloorPosition.y;
	}
	mFaceAngle.x    = 0;
	mModelFaceAngle = mFaceAngle.y;
	return walkProcess();
}

int TMario::walkProcess()
{
	if (mGroundPlane->isIllegalData())
		return 2;

	f32 normalY = 0.0f;
	if (mGroundPlane)
		normalY = mGroundPlane->getNormal().y;

	JGeometry::TVec3<f32> next;
	next.x = mPosition.x + mVel.x / 4 * normalY;
	next.z = mPosition.z + mVel.z / 4 * normalY;
	next.y = mPosition.y;

	int ret         = checkGroundAtWalking(&next);
	mFaceAngle.x    = 0;
	mModelFaceAngle = mFaceAngle.y;
	return ret == 3 ? 2 : ret;
}

int TMario::barProcess()
{
	int result = 0;

	const TBGCheckData* ground;
	const TBGCheckData* roof;
	f32 floorY;

	JGeometry::TVec3<f32> pos;
	pos.x = mHolder->mPosition.x;
	pos.y = mPosition.y;
	pos.z = mHolder->mPosition.z;

	TBGCheckData* wall1 = checkWallPlane(&pos, 60.0f, 43.0f);
	TBGCheckData* wall2 = checkWallPlane(&pos, 30.0f, 24.0f);

	BOOL passable1 = false;
	BOOL passable2 = false;

	if (wall1 == nullptr) {
		passable1 = true;
	} else if (wall1->isMarioThrough()) {
		passable1 = true;
	}

	if (wall2 == nullptr) {
		passable2 = true;
	} else if (wall2->isMarioThrough()) {
		passable2 = true;
	}

	if (passable1 == true && passable2 == true) {
		mPosition.x = mHolder->mPosition.x;
		mPosition.y = mHolder->mPosition.y + mHolderHeightDiff;
		mPosition.z = mHolder->mPosition.z;
	} else {
		mPosition = pos;
	}

	f32 ceilY = gpMap->checkRoof(mPosition.x, mPosition.y + 80.0f, mPosition.z,
	                             &roof);
	if (mPosition.y > ceilY - 160.0f)
		mPosition.y = ceilY - 160.0f;

	checkGroundPlane(mPosition.x, mPosition.y, mPosition.z, &floorY, &ground);
	if (mPosition.y < floorY) {
		mPosition.y = floorY;
		changePlayerStatus(0xc400201, 0, false);
		result = 1;
	}

	setPlayerVelocity(0.0f);
	mFaceAngle.x    = 0;
	mModelFaceAngle = mFaceAngle.y;

	return result;
}

BOOL TMario::hangonCheck(const TBGCheckData* wall, const Vec& prev,
                         const Vec& curr)
{
	if (mVel.y > 0.0f)
		return false;

	if ((curr.x - prev.x) * mVel.x + (curr.z - prev.z) * mVel.z > 0.0f)
		return false;

	JGeometry::TVec3<f32> newPos;
	newPos.x = curr.x - 60.0f * wall->getNormal().x;
	newPos.z = curr.z - 60.0f * wall->getNormal().z;

	const TBGCheckData* ground;
	checkGroundPlane(newPos.x, curr.y + 160.0f, newPos.z, &newPos.y, &ground);

	if (newPos.y - curr.y <= 100.0f)
		return false;

	if (mFloorPosition.x < newPos.y + 160.0f)
		return false;

	if (ground->isUnk1())
		return false;

	if (ground->isNoLedgeGrab())
		return false;

	mPosition.set(newPos);

	mGroundPlane     = ground;
	mFloorPosition.y = newPos.y;
	mSlopeAngle      = matan(ground->getNormal().z, ground->getNormal().x);
	mFaceAngle.y     = matan(wall->getNormal().z, wall->getNormal().x) + 0x8000;
	return true;
}

int TMario::checkGroundAtJumping(const Vec& target, int param_2)
{
	Vec pos             = target;
	TBGCheckData* wall1 = checkWallPlane(&pos, 150.0f, unk15C);
	TBGCheckData* wall2 = checkWallPlane(&pos, 30.0f, unk15C);

	if (checkStatusFlag(0x10000)) {
		mFloorPosition.y
		    = gpMap->checkGround(pos.x, pos.y + 30.0f, pos.z, &mGroundPlane);
	} else {
		checkGroundPlane(pos.x, pos.y + 30.0f, pos.z, &mFloorPosition.y,
		                 &mGroundPlane);
	}

	mFloorPosition.x = checkRoofPlane(pos, mPosition.y + 80.0f, &mRoofPlane);

	int groundCode = 7;
	int wallCode   = 7;
	int roofCode   = 7;

	mPosition.set(pos);

	if (mGroundPlane->isIllegalData()) {
		mPosition  = unk29C;
		groundCode = 2;
	} else {
		BOOL passable = false;
		if (mGroundPlane->isMarioThrough())
			passable = true;

		if (mStatus == STATUS_HIP_DROP
		    && mGroundPlane->isGroundPoundToPassThrough())
			passable = true;

		if (checkStatusFlag(0x10000) && mGroundPlane->isWaterSurface())
			passable = false;

		if (passable == 0 && pos.y <= mFloorPosition.y) {
			mPosition.y = mFloorPosition.y;
			groundCode  = 1;
		}
	}

	if (groundCode == 1)
		unkBC = mVel.y;

	if (mRoofPlane != nullptr && !mRoofPlane->isMarioThrough()) {
		if (160.0f + pos.y > mFloorPosition.x) {
			mPosition.y = mFloorPosition.x - 160.0f;

			if (mRoofPlane->getActor() != nullptr)
				((THitActor*)mRoofPlane->getActor())
				    ->receiveMessage(this, HIT_MESSAGE_UNK2);

			setPlayerVelocity(0.0f);

			if (mVel.y >= 0.0f) {
				mVel.y = 0.0f;
				if (checkFlag(0x2))
					unk118 |= 0x200;

				if (param_2 & 0x2) {
					BOOL canHang = (mHeldObject == nullptr && !onYoshi());
					if (canHang && mRoofPlane->isFence())
						roofCode = 4;
					else
						roofCode = 0;
				}
			}
		}
	}

	mWallPlane = nullptr;

	BOOL wall1Passable = false;
	BOOL wall2Passable = false;

	if (wall1 == nullptr) {
		wall1Passable = true;
	} else if (wall1->isMarioThrough()) {
		wall1Passable = true;
	}

	if (wall2 == nullptr) {
		wall2Passable = true;
	} else if (wall2->isMarioThrough()) {
		wall2Passable = true;
	}

	if ((param_2 & 0x1) && wall1Passable == 1 && wall2Passable == 0) {
		BOOL canHang = (mHeldObject == nullptr && !onYoshi());
		if (canHang == 1) {
			mWallPlane = wall2;
			if (hangonCheck(wall2, target, pos))
				wallCode = 3;
			else
				wallCode = 0;
		}
	} else if (wall1Passable == 0 || wall2Passable == 0) {
		mWallPlane = wall1 != nullptr ? wall1 : wall2;
		s16 diff   = matan(mWallPlane->getNormal().z, mWallPlane->getNormal().x)
		           - (mFaceAngle.y + 0x8000);
		s16 maxAng = mJumpParams.mClashAngle.get();
		if (-maxAng < diff && diff < maxAng)
			wallCode = 2;
	}

	if (groundCode != 7)
		return groundCode;

	if (roofCode != 7)
		return roofCode;

	if (wallCode != 7)
		return wallCode;

	return 0;
}

BOOL TMario::isFallCancel()
{
	if (!(unk78 & 0x100 ? true : false))
		return false;

	if (mStatus & 0x21000)
		return false;

	if (!(mInput & 0x80) && mVel.y > 20.0f)
		return true;

	return false;
}

void TMario::fallProcess()
{
	if (mStatus == 0x891) {
		mVel.y -= mDivingParams.mGravity.get();
		if (mVel.y < -75.0f)
			mVel.y = -75.0f;
	} else {
		if (isFallCancel()) {
			mVel.y *= 0.75f;
		} else if (mStatus - 0x895 <= 1 && mVel.y < 0.0f) {
			mVel.y -= mJumpParams.mSpinJumpGravity.get();
		} else {
			mVel.y -= mJumpParams.mGravity.get();
		}
		if (onYoshi()) {
			mYoshi->thinkHoldOut();
		}
	}
	if (mVel.y < -75.0f)
		mVel.y = -75.0f;
}

int TMario::jumpProcess(int param_1)
{
	int result = 0;
	f32 speed  = std::sqrtf(mVel.x * mVel.x + mVel.z * mVel.z);

	if (speed > mJumpParams.mJumpingMax.get()) {
		mVel.x = mVel.x * (mJumpParams.mJumpingMax.get() / speed);
		mVel.z = mVel.z * (mJumpParams.mJumpingMax.get() / speed);
	}

	Vec next;
	next.x  = mPosition.x + 0.25f * mVel.x;
	next.y  = mPosition.y + 0.25f * mVel.y;
	next.z  = mPosition.z + 0.25f * mVel.z;
	int ret = checkGroundAtJumping(next, param_1);
	if (ret != 0)
		result = ret;

	if (mVel.y >= 0.0f)
		unk104 = mPosition.y;

	fallProcess();

	if (mStatus != STATUS_DIVE_JUMP)
		mFaceAngle.x = 0;

	mModelFaceAngle = mFaceAngle.y;
	return result;
}

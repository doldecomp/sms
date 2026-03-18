#include <Player/MarioMain.hpp>

#include <Map/Map.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <System/MarDirector.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <Player/Watergun.hpp>
#include <Strategic/LiveActor.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <MSound/MSoundBGM.hpp>
#include <MSound/MSound.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionData.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Camera/Camera.hpp>
#include <System/MarioGamePad.hpp>
#include <System/StageUtil.hpp>
#include <dolphin/mtx.h>
#include <fake_tgmath.h>
#include <Player/ModelWaterManager.hpp>
#include <System/EmitterViewObj.hpp>
#include <MarioUtil/EffectUtil.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <MoveBG/Pool.hpp>
#include <Map/PollutionManager.hpp>

void TMario::addVelocity(f32 vel)
{
	mForwardVel += vel;
	if (mForwardVel > 99.0f)
		mForwardVel = 99.0f;
}

void TMario::windMove(const JGeometry::TVec3<f32>& wind)
{
	mPosition.x += wind.x;
	mPosition.y += wind.y;
	mPosition.z += wind.z;
}

void TMario::flowMove(const JGeometry::TVec3<f32>& flow)
{
	u8 inWater;
	if (mAction & 0x2000)
		inWater = 1;
	else
		inWater = 0;

	if (inWater != 1)
		return;

	mPosition.x += flow.x;
	mPosition.y += flow.y;
	mPosition.z += flow.z;
}

bool TMario::moveRequest(const JGeometry::TVec3<f32>& pos)
{
	JGeometry::TVec3<f32> localPos(pos);
	localPos.sub(mPosition);
	JGeometry::TVec3<f32> delta(localPos);

	mPosition = *(JGeometry::TVec3<f32>*)&pos;

	// Adjust all position-relative fields by delta
	unk160[0].x += delta.x;
	unk160[0].y += delta.y;
	unk160[0].z += delta.z;

	unk29C.x += delta.x;
	unk29C.y += delta.y;
	unk29C.z += delta.z;

	mWireStartPos.x += delta.x;
	mWireStartPos.y += delta.y;
	mWireStartPos.z += delta.z;

	mWireEndPos.x += delta.x;
	mWireEndPos.y += delta.y;
	mWireEndPos.z += delta.z;

	*(f32*)((u8*)this + 0x2A8) += delta.x;
	*(f32*)((u8*)this + 0x2AC) += delta.y;
	*(f32*)((u8*)this + 0x2B0) += delta.z;

	unk2BC += delta.y;

	unk1CC += delta.x;
	unk1DC += delta.y;
	unk1EC += delta.z;

	*(f32*)((u8*)this + 0x1FC) += delta.x;
	*(f32*)((u8*)this + 0x20C) += delta.y;
	*(f32*)((u8*)this + 0x21C) += delta.z;

	*(f32*)((u8*)this + 0x22C) += delta.x;
	*(f32*)((u8*)this + 0x23C) += delta.y;
	*(f32*)((u8*)this + 0x24C) += delta.z;

	*(f32*)((u8*)this + 0x324) += delta.x;
	*(f32*)((u8*)this + 0x334) += delta.y;
	*(f32*)((u8*)this + 0x344) += delta.z;

	if (mRidingActor != NULL) {
		Mtx localMtx;
		if (mRidingActor->getRootJointMtx() == NULL) {
			SMS_GetActorMtx(*mRidingActor, localMtx);
		} else {
			PSMTXCopy(*(mRidingActor->getRootJointMtx()), localMtx);
		}
		PSMTXInverse(localMtx, localMtx);

		*(JGeometry::TVec3<f32>*)((u8*)this + 0x300) =
		    *(JGeometry::TVec3<f32>*)((u8*)this + 0x2F4);

		PSMTXMultVec(localMtx, (Vec*)&mPosition,
		             (Vec*)((u8*)this + 0x2F4));
	}

	return true;
}

void TMario::warpRequest(const JGeometry::TVec3<f32>& pos, f32 angle)
{
	JGeometry::TVec3<f32> dir(pos);
	dir.sub(mPosition);
	JGeometry::TVec3<f32> delta(dir);

	moveRequest(pos);

	mFaceAngle.y = DEG2SHORTANGLE(angle);
	mModelFaceAngle = mFaceAngle.y;

	gpCamera->addMoveCameraAndMario(*(const Vec*)&delta);

	if (gpMarDirector->mMap != 7)
		mGamePad->onNeutralMarioKey();

	changePlayerStatus(0x0C400201, 0, true);
}

BOOL TMario::changePlayerTriJump()
{
	int jumpAmount;

	if ((u8)isForceSlip()) {
		jumpAmount = mSlipParamsAll.mMissJump.value;
	} else {
		const TBGCheckData* ground = mGroundPlane;
		u16 bgType = ground->mBGType;

		u8 isSlippery;
		if (bgType == 0x0C || bgType == 0x800C || bgType == 0xA00C)
			isSlippery = 1;
		else
			isSlippery = 0;
		if (isSlippery) {
			jumpAmount = mSlipParamsAllSlider.mMissJump.value;
		} else {
			u8 isSand;
			if (bgType == 0x02 || bgType == 0x8002)
				isSand = 1;
			else
				isSand = 0;
			if (isSand && ground->mNormal.y < 0.866025f) {
				jumpAmount = mSlipParams45.mMissJump.value;
			} else {
				u8 isWet;
				if (bgType == 0x04 || bgType == 0x4004
				    || bgType == 0x8004 || bgType == 0xC004)
					isWet = 1;
				else
					isWet = 0;
				if (isWet) {
					if (ground->mNormal.y > 0.99f) {
						jumpAmount = mSlipParamsWaterGround.mMissJump.value;
					} else {
						jumpAmount = mSlipParamsWaterSlope.mMissJump.value;
					}
				} else {
					jumpAmount = mSlipParamsNormal.mMissJump.value;
				}
			}
		}
	}

	if (jumpAmount != 0) {
		*(u16*)((u8*)this + 0x94) = 0;
		mModelFaceAngle = mFaceAngle.y;

		if (mForwardVel > 0.0f) {
			s16 oppAngle = (s16)(mSlopeAngle + 0x8000);
			u16 diff = (u16)(mFaceAngle.y - oppAngle);

			f32 sinDiff = JMASSin(diff);
			f32 cosDiff = JMASCos(diff);

			f32 fwd = mForwardVel;
			f32 sinComp = fwd * sinDiff;
			f32 cosComp = fwd * cosDiff;
			f32 scaledSin = 0.75f * sinComp;
			f32 sqSum = cosComp * cosComp + scaledSin * scaledSin;

			if (sqSum > 0.0f) {
				double guess = __frsqrte((double)sqSum);
				guess = .5 * guess * (3.0 - guess * guess * sqSum);
				volatile f32 sqrtResult;
				sqrtResult = (f32)(sqSum * guess);
				sqSum = sqrtResult;
			}
			mForwardVel = sqSum;

			mSlideVelX = mForwardVel * JMASSin((u16)mFaceAngle.y);
			mSlideVelZ = mForwardVel * JMASCos((u16)mFaceAngle.y);

			mVel.x = mSlideVelX;
			mVel.z = mSlideVelZ;

			s16 newAngle = matan(scaledSin, cosComp);
			mFaceAngle.y = oppAngle + newAngle;
		}

		dropObject();
		changePlayerStatus(0x02000885, 0, false);
		return TRUE;
	}

	int stickDirection;
	if (checkStickRotate(&stickDirection)) {
		switch (stickDirection) {
		case 2:
			changePlayerStatus(0x896, 0, false);
			break;
		case 3:
			changePlayerStatus(0x895, 0, false);
			break;
		}
		return TRUE;
	}

	changePlayerStatus(0x02000880, 0, false);
	return TRUE;
}

BOOL TMario::changePlayerJumping(u32 status, u32 arg)
{
	int jumpAmount;

	if ((u8)isForceSlip()) {
		jumpAmount = mSlipParamsAll.mMissJump.value;
	} else {
		const TBGCheckData* ground = mGroundPlane;
		u16 bgType = ground->mBGType;

		u8 isSlippery;
		if (bgType == 0x0C || bgType == 0x800C || bgType == 0xA00C)
			isSlippery = 1;
		else
			isSlippery = 0;
		if (isSlippery) {
			jumpAmount = mSlipParamsAllSlider.mMissJump.value;
		} else {
			u8 isSand;
			if (bgType == 0x02 || bgType == 0x8002)
				isSand = 1;
			else
				isSand = 0;
			if (isSand && ground->mNormal.y < 0.866025f) {
				jumpAmount = mSlipParams45.mMissJump.value;
			} else {
				u8 isWet;
				if (bgType == 0x04 || bgType == 0x4004
				    || bgType == 0x8004 || bgType == 0xC004)
					isWet = 1;
				else
					isWet = 0;
				if (isWet) {
					if (ground->mNormal.y > 0.99f) {
						jumpAmount = mSlipParamsWaterGround.mMissJump.value;
					} else {
						jumpAmount = mSlipParamsWaterSlope.mMissJump.value;
					}
				} else {
					jumpAmount = mSlipParamsNormal.mMissJump.value;
				}
			}
		}
	}

	if (jumpAmount != 0) {
		*(u16*)((u8*)this + 0x94) = 0;
		mModelFaceAngle = mFaceAngle.y;

		if (mForwardVel > 0.0f) {
			s16 oppAngle = (s16)(mSlopeAngle + 0x8000);
			u16 diff = (u16)(mFaceAngle.y - oppAngle);

			f32 sinDiff = JMASSin(diff);
			f32 cosDiff = JMASCos(diff);

			f32 fwd = mForwardVel;
			f32 sinComp = fwd * sinDiff;
			f32 cosComp = fwd * cosDiff;
			f32 scaledSin = 0.75f * sinComp;
			f32 sqSum = cosComp * cosComp + scaledSin * scaledSin;

			if (sqSum > 0.0f) {
				double guess = __frsqrte((double)sqSum);
				guess = .5 * guess * (3.0 - guess * guess * sqSum);
				volatile f32 sqrtResult;
				sqrtResult = (f32)(sqSum * guess);
				sqSum = sqrtResult;
			}
			mForwardVel = sqSum;

			mSlideVelX = mForwardVel * JMASSin((u16)mFaceAngle.y);
			mSlideVelZ = mForwardVel * JMASCos((u16)mFaceAngle.y);

			mVel.x = mSlideVelX;
			mVel.z = mSlideVelZ;

			s16 newAngle = matan(scaledSin, cosComp);
			mFaceAngle.y = oppAngle + newAngle;
		}

		dropObject();
		changePlayerStatus(0x02000885, 0, false);
		return TRUE;
	}

	int stickDirection;
	if (checkStickRotate(&stickDirection)) {
		switch (stickDirection) {
		case 2:
			changePlayerStatus(0x896, 0, false);
			break;
		case 3:
			changePlayerStatus(0x895, 0, false);
			break;
		}
		return TRUE;
	}

	changePlayerStatus(status, arg, false);
	return TRUE;
}

BOOL TMario::changePlayerDropping(u32 status, u32 arg)
{
	dropObject();
	return changePlayerStatus(status, arg, false);
}

BOOL TMario::changePlayerStatus(u32 status, u32 arg, bool force)
{
	if (!force) {
		if (status == mAction)
			return 0;
		if (checkActionThing())
			return 0;
	}

	if (mAction == 0x20467)
		return 0;

	if (SMS_isDivingMap()) {
		if ((u32)(status - 0x10020000) != 880 && status != 0x0891
		    && status != 0x1302)
			return 0;
	}

	switch (status & 0x1C0) {
	case 0x40: {
		f32 speed = mIntendedMag;
		if (speed <= 8.0f)
			;
		else
			speed = 8.0f;

		if (status == 0x04000440) {
			if (0.0f <= mForwardVel && mForwardVel < speed)
				mForwardVel = speed;
		} else if (status == 0x50) {
			u8 facing = 0;
			s16 angleDiff = (s16)(mSlopeAngle - mFaceAngle.y);
			if (angleDiff > -16384 && angleDiff < 16384)
				facing = 1;

			if (facing)
				status = 0x00840452;
			else
				status = 0x00840453;

			startVoice(0x78CF);
		}
		break;
	}
	case 0x80:
		status = setStatusToJumping(status, arg);
		break;
	}

	mPrevAction = mAction;
	mAction = status;
	mActionArg = arg;
	mActionState = 0;
	mActionTimer = 0;
	return 1;
}

void TMario::throwMario(const JGeometry::TVec3<f32>& throwVec, f32 speed)
{
	JGeometry::TVec3<f32> dir(throwVec);

	if (dir.squared() <= 0.0f)
		dir.y = 0.0f;

	dir.normalize();

	f32 dirZ = dir.z;
	mFaceAngle.y = matan(dirZ, dir.x) + 0x8000;
	mModelFaceAngle = mFaceAngle.y;

	f32 hMagSq = dir.x * dir.x + dirZ * dirZ;
	f32 hMag = std::sqrtf(hMagSq);

	mForwardVel = speed * -hMag;
	mVel.y = dir.y * speed;
}

void TMario::setPlayerVelocity(f32 speed)
{
	mForwardVel = speed;
	mSlideVelX = mForwardVel * JMASSin(mFaceAngle.y);
	mSlideVelZ = mForwardVel * JMASCos(mFaceAngle.y);
	mVel.x = mSlideVelX;
	mVel.z = mSlideVelZ;
}

void TMario::setNormalAttackArea()
{
	mAttackRadius = mDeParams.mHoldRadius.value;
	calcEntryRadius();
	mAttackHeight = mDeParams.mAttackHeight.value;
	calcEntryRadius();
}

BOOL TMario::canBendBody()
{
	u32 act = mAction & 0x1FF;
	if (act >= 0x14B && act <= 0x14F)
		return false;
	if (act >= 0x140 && act <= 0x143)
		return false;
	return true;
}

void TMario::checkThrowObject()
{
	if (mModel->unkC->checkPass(4.0f)) {
		startVoice(0x788F);
		dropObject();
	}
}

BOOL TMario::onYoshi() const
{
	u8 result = 0;
	if (mYoshi != NULL) {
		if (mYoshi->onYoshi())
			result = 1;
	}
	return result;
}

BOOL TMario::checkGroundPlane(f32 x, f32 y, f32 z, f32* outHeight,
                              const TBGCheckData** outPlane)
{
	*outHeight = gpMap->checkGround(x, y, z, outPlane);

	if ((*outPlane)->isMarioThrough()) {
		*outHeight = gpMap->checkGround(x, *outHeight - 1.0f, z, outPlane);
	}

	u8 isWall;
	if ((*outPlane)->mFlags & 0x10)
		isWall = 1;
	else
		isWall = 0;

	u8 isSafe;
	if (isWall == 1)
		isSafe = 0;
	else
		isSafe = 1;

	if (isSafe)
		return true;
	return false;
}

f32 TMario::checkRoofPlane(const Vec& pos, f32 height,
                           const TBGCheckData** result)
{
	return gpMap->checkRoof(pos.x, height + 80.0f, pos.z, result);
}

bool TMario::isFrontSlip(int param)
{
	int angle = mFaceAngle.y;
	if (param != 0) {
		if (mForwardVel < 0.0f)
			angle = angle + 0x8000;
	}
	s16 diff = (s16)(mSlopeAngle - angle);
	bool result = false;
	if (diff > -0x4000 && diff < 0x4000)
		result = true;
	return result;
}

void TMario::dirtyLimitCheck()
{
	if (unk134 < 0.0f)
		unk134 = 0.0f;
	f32 maxDirty = mDirtyParams.mDirtyMax.value;
	if (maxDirty < unk134)
		unk134 = maxDirty;
}

void TMario::stateMachine()
{
	BOOL result = true;
	while (result) {
		switch (mAction & 0x1C0) {
		case 0x000:
			result = waitMain();
			break;
		case 0x040:
			result = moveMain();
			break;
		case 0x080:
			result = jumpMain();
			break;
		case 0x0C0:
			result = swimMain();
			break;
		case 0x100:
			result = demoMain();
			break;
		case 0x140:
			result = specMain();
			break;
		case 0x180:
			result = actnMain();
			break;
		}
	}
}

void TMario::calcGroundMtx(const JGeometry::TVec3<f32>& inPos)
{
	JGeometry::TVec3<f32> pos(inPos);
	const TBGCheckData* check;
	pos.y = gpMap->checkGround(pos, &check);

	JGeometry::TVec3<f32> pt1;
	pt1.x = pos.x + JMASCos(mFaceAngle.y);
	pt1.y = 30.0f + pos.y;
	pt1.z = pos.z - JMASSin(mFaceAngle.y);
	pt1.y = gpMap->checkGround(pt1, &check);

	JGeometry::TVec3<f32> pt2;
	pt2.x = pos.x + JMASSin(mFaceAngle.y);
	pt2.y = 30.0f + pos.y;
	pt2.z = pos.z + JMASCos(mFaceAngle.y);
	pt2.y = gpMap->checkGround(pt2, &check);

	mGroundMtx[0][0] = pt1.x - pos.x;
	mGroundMtx[1][0] = pt1.y - pos.y;
	mGroundMtx[2][0] = pt1.z - pos.z;

	mGroundMtx[0][1] = 0.0f;
	mGroundMtx[1][1] = 1.0f;
	mGroundMtx[2][1] = 0.0f;

	mGroundMtx[0][2] = pt2.x - pos.x;
	mGroundMtx[1][2] = pt2.y - pos.y;
	mGroundMtx[2][2] = pt2.z - pos.z;

	mGroundMtx[0][3] = pos.x;
	mGroundMtx[1][3] = 18.0f + pos.y;
	mGroundMtx[2][3] = pos.z;
}

u32 TMario::setStatusToJumping(u32 status, u32 arg)
{
	unk2BC = mPosition.y;

	s16 health = *(s16*)((u8*)this + 0x360);
	s32 halfMaxHealth = mDeParams.mFootPrintTimerMax.value / 2;
	if (health > halfMaxHealth) {
		gpPollution->stamp(1, mPosition.x, mPosition.y, mPosition.z,
		                   mDirtyParams.mPolSizeJump.value);
	}

	switch (status) {
	case 0x089C:
	case 0x02000880: {
		// Standard/running jump
		mVel.y = mForwardVel * 0.25f + 42.0f;
		mForwardVel = mForwardVel * 0.8f;

		const TBGCheckData* ground = mGroundPlane;
		u8 hasSlipFlag;
		if (ground->mFlags & 0x10)
			hasSlipFlag = 1;
		else
			hasSlipFlag = 0;
		if (hasSlipFlag) {
			break;
		}

		u16 groundType = ground->mBGType;
		u8 isBeachGround;
		if (groundType == 0x108 || groundType == 0x8
		    || groundType == 0x8008)
			isBeachGround = 1;
		else
			isBeachGround = 0;
		if (isBeachGround) {
			f32 jumpPower = ground->getActiveJumpPower();
			mVel.y = 0.01f * jumpPower;
			status = 0x884;
			break;
		}

		u8 isSlipperyGround;
		if (groundType == 0x9 || groundType == 0x8009)
			isSlipperyGround = 1;
		else
			isSlipperyGround = 0;
		if (isSlipperyGround) {
			f32 jumpAdj;
			if (ground != NULL) {
				f32 jumpPower = ground->getActiveJumpPower();
				jumpAdj = 0.01f * jumpPower;
			} else {
				jumpAdj = 0.0f;
			}
			f32 gravity = *(f32*)((u8*)this + 0xBC);
			mVel.y = mVel.y + (-gravity + jumpAdj);

			TLiveActor* groundActor =
			    (TLiveActor*)mGroundPlane->mActor;
			if (groundActor != NULL) {
				((THitActor*)groundActor)->receiveMessage((THitActor*)this, 0);
			}

			startVoice(0x78B9);
			status = 0x884;
			break;
		}

		// Normal ground: check speed for voice
		u8 isFast;
		if (unk370 > mDeParams.mFeelDeep.value)
			isFast = 1;
		else
			isFast = 0;
		if (isFast) {
			startVoice(0x78A3);
		} else {
			startVoice(0x78AB);
		}
		break;
	}
	case 0x02000881: {
		// Hip-drop jump
		mVel.y
		    = mForwardVel * mJumpParams.mSecJumpSpeedMult.value
		      + mJumpParams.mSecJumpForce.value;
		mForwardVel = mForwardVel * mJumpParams.mSecJumpXZMult.value;
		startVoice(0x78B1);
		break;
	}
	case 0x02000886: {
		// Special jump (ground pound variant)
		mVel.y = mForwardVel * 0.0f + 62.0f;
		mForwardVel = 24.0f;
		startVoice(0x78B1);
		break;
	}
	case 0x0882: {
		// Somersault jump
		mVel.y
		    = mForwardVel * mJumpParams.mUltraJumpSpeedMult.value
		      + mJumpParams.mUltraJumpForce.value;
		mForwardVel = mForwardVel * mJumpParams.mUltraJumpXZMult.value;
		startVoice(0x78B6);
		break;
	}
	case 0x0883: {
		// Side somersault
		mForwardVel = mJumpParams.mBackJumpForce.value;
		mVel.y
		    = mForwardVel * 0.0f
		      + mJumpParams.mBackJumpForceY.value;
		startVoice(0x78B6);
		break;
	}
	case 0x0884: {
		// Ground pound bounce
		if (mGroundPlane != NULL) {
			f32 jumpPower = mGroundPlane->getActiveJumpPower();
			mVel.y = 0.01f * jumpPower;
		} else {
			mVel.y = 0.0f;
		}
		startVoice(0x78B1);
		break;
	}
	case 0x0895:
	case 0x0896: {
		// Backflip
		mVel.y
		    = mForwardVel * 0.25f + mJumpParams.mRotateJumpForceY.value;
		mForwardVel = mForwardVel * 0.8f;
		startVoice(0x78B6);
		break;
	}
	case 0x0887: {
		// Spin jump
		mVel.y
		    = mForwardVel * 0.0f + mJumpParams.mTurnJumpForce.value;
		mForwardVel = 8.0f;
		*(s16*)((u8*)this + 0x96)
		    = *(s16*)((u8*)this + 0x90);
		startVoice(0x78B6);
		break;
	}
	case 0x0888: {
		// Wall kick
		startVoice(0x78B1);
		mForwardVel = mJumpParams.mBroadJumpForce.value;
		mVel.y = mJumpParams.mBroadJumpForceY.value;
		break;
	}
	case 0x02000889: {
		// Long jump
		startVoice(0x78B1);
		mForwardVel = mJumpParams.mRotBroadJumpForce.value;
		mVel.y = mJumpParams.mRotBroadJumpForceY.value;
		break;
	}
	case 0x0208B4: {
		// Zero velocity
		mVel.y = 31.5f;
		mForwardVel = 8.0f;
		break;
	}
	case 0x0281089A: {
		// Hip-drop-to-slide: check ground type
		startVoice(0x78AB);
		const TBGCheckData* groundResult;
		gpMap->checkGround(mPosition.x, mPosition.y, mPosition.z,
		                   &groundResult);
		u16 gType = groundResult->mBGType;
		u8 isBeach;
		if (gType == 0x100 || gType == 0x101
		    || (u16)(gType - 0x102) <= 3 || gType == 0x4104)
			isBeach = 1;
		else
			isBeach = 0;
		if (isBeach) {
			mVel.y
			    = mForwardVel * mSurfingParamsWaterRed.mJumpXZRatio.value
			      + mSurfingParamsWaterRed.mJumpPow.value;
		} else {
			mVel.y
			    = mForwardVel * mSurfingParamsGroundRed.mJumpXZRatio.value
			      + mSurfingParamsGroundRed.mJumpPow.value;
		}
		break;
	}
	case 0x000208B7: {
		// Wall slide jump
		if (*(u32*)((u8*)this + 0x88) == 2)
			break;
		mVel.y = mJumpParams.mFireDownForce.value;
		if (*(u32*)((u8*)this + 0x88) != 0)
			break;
		mForwardVel = -mJumpParams.mFireBackVelocity.value;
		break;
	}
	case 0x0080088A: {
		// Dive recovery
		startVoice(0x7884);
		f32 clampedVel = 15.0f + mForwardVel;
		if (clampedVel > 48.0f)
			clampedVel = 48.0f;
		mForwardVel = clampedVel;

		u16 angle = mFaceAngle.y;
		mSlideVelX = mForwardVel * jmaSinTable[angle >> jmaSinShift];
		mSlideVelZ = mForwardVel * jmaCosTable[angle >> jmaSinShift];
		mVel.x = mSlideVelX;
		mVel.z = mSlideVelZ;
		break;
	}
	case 0x02000885: {
		// Jumping from certain state
		startVoice(0x78AB);
		mVel.y = mForwardVel * 0.25f + 42.0f;
		break;
	}
	case 0x088B: {
		// FLUDD-dependent jump
		TWaterGun* waterGun = mWaterGun;
		if (waterGun == NULL)
			break;

		s32 nozzle = waterGun->mCurrentNozzle;
		if (nozzle == 1) {
			// Rocket
			startVoice(0x78B9);
			rocketEffectStart();
		}
		waterGun = mWaterGun;
		if (waterGun->mCurrentNozzle == 5) {
			// Turbo
			startVoice(0x788F);
		}
		waterGun = mWaterGun;
		if (waterGun->mCurrentNozzle == 4) {
			// Hover
			startVoice(0x78AB);
		}
		mVel.y = mForwardVel * 0.0f + 10.0f;
		break;
	}
	case 0x02000890: {
		// Multi-bounce/triple jump
		u16 animId = mAnimationId;
		switch (animId) {
		case 0xD2:
			// Triple jump
			startVoice(0x78B1);
			mVel.y
			    = mForwardVel * 0.25f
			      + mDeParams.mTramplePowStep2.value;
			break;
		case 0xD3:
			// Double jump (D3)
			startVoice(0x78B6);
			mVel.y
			    = mForwardVel * 0.25f
			      + mDeParams.mTramplePowStep3.value;
			break;
		default:
			// Other
			startVoice(0x78AB);
			mVel.y
			    = mForwardVel * 0.25f
			      + mDeParams.mTramplePowStep1.value;
			break;
		}
		mForwardVel = mForwardVel * 0.8f;
		break;
	}
	case 0x0892: {
		// Directional air
		mVel.y = mForwardVel * 0.25f + 42.0f;
		mForwardVel = 0.0f;
		u16 angle = mFaceAngle.y;
		mSlideVelX = mForwardVel * jmaSinTable[angle >> jmaSinShift];
		mSlideVelZ = mForwardVel * jmaCosTable[angle >> jmaSinShift];
		mVel.x = mSlideVelX;
		mVel.z = mSlideVelZ;
		startVoice(0x78B6);
		break;
	}
	case 0x0893: {
		// Pole jump
		if (arg == 0) {
			s16 poleAngle = *(s16*)((u8*)this + 0xF6);
			s32 fixedAngle = -0x2000;
			f32 paramSpeed = mWireParams.mJumpRate.value;
			f32 fAngle = (f32)poleAngle;
			f32 sinVal = jmaSinTable[fixedAngle >> jmaSinShift];
			mVel.y = fAngle * paramSpeed * 1.0f * sinVal;
			f32 cosVal = jmaCosTable[fixedAngle >> jmaSinShift];
			mForwardVel = -(fAngle * paramSpeed * 1.0f) * cosVal;

			u16 faceAngle = mFaceAngle.y;
			mSlideVelX
			    = mForwardVel * jmaSinTable[faceAngle >> jmaSinShift];
			mSlideVelZ
			    = mForwardVel * jmaCosTable[faceAngle >> jmaSinShift];
			mVel.x = mSlideVelX;
			mVel.z = mSlideVelZ;
		} else {
			s16 poleAngle = *(s16*)((u8*)this + 0xF6);
			f32 fAngle = (f32)poleAngle;
			s32 fixedAngle = 0x6000;
			f32 paramSpeed = mWireParams.mJumpRate.value;
			f32 sinVal = jmaSinTable[fixedAngle >> jmaSinShift];
			mVel.y = fAngle * paramSpeed * 1.0f * sinVal;
			f32 cosVal = jmaCosTable[fixedAngle >> jmaSinShift];
			mForwardVel = -(fAngle * paramSpeed * 1.0f) * cosVal;

			u16 faceAngle = mFaceAngle.y;
			mSlideVelX
			    = mForwardVel * jmaSinTable[faceAngle >> jmaSinShift];
			mSlideVelZ
			    = mForwardVel * jmaCosTable[faceAngle >> jmaSinShift];
			mVel.x = mSlideVelX;
			mVel.z = mSlideVelZ;
		}
		startVoice(0x78B9);
		break;
	}
	case 0x0894: {
		// Slide jump
		mVel.y = mForwardVel * 0.0f + 42.0f;
		mForwardVel = 0.0f;
		u16 angle = mFaceAngle.y;
		mSlideVelX = mForwardVel * jmaSinTable[angle >> jmaSinShift];
		mSlideVelZ = mForwardVel * jmaCosTable[angle >> jmaSinShift];
		mVel.x = mSlideVelX;
		mVel.z = mSlideVelZ;
		startVoice(0x78AB);
		break;
	}
	default:
		break;
	}

	// Speed bonus
	f32 speedBonus = *(f32*)((u8*)this + 0x368);
	int hasSpeedBonus;
	if (speedBonus > 0.0f)
		hasSpeedBonus = 1;
	else
		hasSpeedBonus = 0;
	if (hasSpeedBonus) {
		s16 maxAge = mGraffitoParams.mSinkTime.value;
		f32 fMaxAge = (f32)maxAge;
		f32 minScale = mGraffitoParams.mSinkJumpRateMin.value;
		f32 maxScale = mGraffitoParams.mSinkJumpRateMax.value;
		f32 scaleRange = maxScale - minScale;
		f32 ratio = 1.0f - speedBonus / fMaxAge;
		f32 scale = scaleRange * ratio + minScale;
		mVel.y *= scale;
		mForwardVel *= scale;

		// Decay speed bonus
		f32 decayParam = mGraffitoParams.mSinkRecover.value;
		f32 bonus2 = *(f32*)((u8*)this + 0x368);
		f32 fAge2 = (f32)maxAge;
		f32 ratio2 = bonus2 / fAge2;
		f32 invRatio = 1.0f - ratio2;
		f32 decay = fAge2 * decayParam;
		f32 newBonus = bonus2 - invRatio * decay;
		*(f32*)((u8*)this + 0x368) = newBonus;
		if (*(f32*)((u8*)this + 0x368) < 0.0f)
			*(f32*)((u8*)this + 0x368) = 0.0f;
	}

	// Yoshi check
	u8 isOnYoshi = 0;
	if (mYoshi != NULL) {
		if (((TYoshi*)mYoshi)->onYoshi())
			isOnYoshi = 1;
	}
	if (isOnYoshi) {
		mVel.y *= mYoshiParams.mJumpYoshiMult.value;
		TYoshi* yoshi = (TYoshi*)mYoshi;
		yoshi->mFlutterState = 0;
		yoshi->mFlutterTimer = yoshi->mMaxFlutterTimer;
	}

	// Store jump-start Y
	*(f32*)((u8*)this + 0x104) = mPosition.y;

	// Update flag bit 8 based on status bit 25
	if (status & 0x02000000) {
		unk78 |= 0x100;
	} else {
		unk78 &= ~0x100;
	}

	return status;
}

void TMario::checkPlayerAround(int angleOffset, f32 distance)
{
	u16 angle = mFaceAngle.y + angleOffset;
	f32 sinVal = JMASSin(angle) * distance;
	f32 cosVal = JMASCos(angle) * distance;
	const TBGCheckData* result;
	gpMap->checkGround(mPosition.x + sinVal, mPosition.y + 100.0f,
	                   mPosition.z + cosVal, &result);
}

void TMario::checkRideReCalc()
{
	if (mRidingActor != NULL) {
		Mtx localMtx;
		if (mRidingActor->getRootJointMtx() == NULL) {
			SMS_GetActorMtx(*mRidingActor, localMtx);
		} else {
			PSMTXCopy(*(mRidingActor->getRootJointMtx()), localMtx);
		}
		PSMTXInverse(localMtx, localMtx);

		*(JGeometry::TVec3<f32>*)((u8*)this + 0x300) =
		    *(JGeometry::TVec3<f32>*)((u8*)this + 0x2F4);

		PSMTXMultVec(localMtx, (Vec*)&mPosition,
		             (Vec*)((u8*)this + 0x2F4));
	}
}

// Helper macro to avoid caching ctrl in a register (original reloads from unk108 each time)
#define CTRL ((TMarioControllerWork*)unk108)

void TMario::checkController(JDrama::TGraphics* gfx)
{
	// Convert gamepad stick positions to controller stick values
	CTRL->mStickHS16 = (s16)(128.0f * mGamePad->mCompSPos[0]);
	CTRL->mStickVS16 = (s16)(128.0f * mGamePad->mCompSPos[1]);

	// Scale stick values during special movement
	f32 timer368 = *(f32*)((u8*)this + 0x368);
	int bTimerActive;
	if (timer368 > 0.0f) {
		bTimerActive = 1;
	} else {
		bTimerActive = 0;
	}
	if (bTimerActive) {
		s16 maxTime = mGraffitoParams.mSinkTime.value;
		f32 minScale = mGraffitoParams.mSinkMoveMin.value;
		f32 maxScale = mGraffitoParams.mSinkMoveMax.value;
		f32 ratio = timer368 / (f32)maxTime;
		f32 scale = (maxScale - minScale) * (1.0f - ratio) + minScale;
		CTRL->mStickHS16 = (s16)((f32)CTRL->mStickHS16 * scale);
		CTRL->mStickVS16 = (s16)((f32)CTRL->mStickVS16 * scale);
	}

	// Clear button flags
	CTRL->mInput      = (TMarioControllerWork::Buttons)0;
	CTRL->mFrameInput = (TMarioControllerWork::Buttons)0;

	// Map gamepad meanings to controller buttons
	// A button
	if (mGamePad->mMeaning & TMarioGamePad::MEANING_0x80)
		CTRL->mInput = (TMarioControllerWork::Buttons)(CTRL->mInput | TMarioControllerWork::A);
	if (mGamePad->mEnabledFrameMeaning & TMarioGamePad::MEANING_0x80)
		CTRL->mFrameInput
		    = (TMarioControllerWork::Buttons)(CTRL->mFrameInput | TMarioControllerWork::A);

	// B button
	if (mGamePad->mMeaning & TMarioGamePad::MEANING_0x100)
		CTRL->mInput = (TMarioControllerWork::Buttons)(CTRL->mInput | TMarioControllerWork::B);
	if (mGamePad->mEnabledFrameMeaning & TMarioGamePad::MEANING_0x100)
		CTRL->mFrameInput
		    = (TMarioControllerWork::Buttons)(CTRL->mFrameInput | TMarioControllerWork::B);

	// R trigger
	if (mGamePad->mMeaning & TMarioGamePad::MEANING_0x400)
		CTRL->mInput = (TMarioControllerWork::Buttons)(CTRL->mInput | TMarioControllerWork::R);
	if (mGamePad->mEnabledFrameMeaning & TMarioGamePad::MEANING_0x400)
		CTRL->mFrameInput
		    = (TMarioControllerWork::Buttons)(CTRL->mFrameInput | TMarioControllerWork::R);

	// L trigger (frame only)
	if (mGamePad->mEnabledFrameMeaning & TMarioGamePad::MEANING_0x1000)
		CTRL->mFrameInput = (TMarioControllerWork::Buttons)(CTRL->mFrameInput | 0x10);

	// Convert analog triggers and get raw values
	u8* pZeroVal  = (u8*)((u8*)this + 0x2358);
	u8* pMidVal   = (u8*)((u8*)this + 0x236C);
	u8* pMaxVal   = (u8*)((u8*)this + 0x2380);
	f32* pMidLevel = (f32*)((u8*)this + 0x2394);

	CTRL->mAnalogRU8 = (u8)(s32)mGamePad->mCompSPos[3];
	CTRL->mAnalogLU8 = (u8)(s32)mGamePad->mCompSPos[2];

	u8 analogR = (u8)(s32)mGamePad->mCompSPos[3];
	u8 analogL = (u8)(s32)mGamePad->mCompSPos[2];

	// Analog R interpolation -> ctrl->mAnalogR
	f32 interpResult;
	if (analogR < *pZeroVal) {
		interpResult = 0.0f;
	} else if (analogR < *pMidVal) {
		interpResult
		    = *pMidLevel * (f32)(s32)(analogR - *pZeroVal) / (f32)(s32)(*pMidVal - *pZeroVal);
	} else if (analogR < *pMaxVal) {
		interpResult
		    = *pMidLevel
		      + (1.0f - *pMidLevel) * (f32)(s32)(analogR - *pMidVal)
		            / (f32)(s32)(*pMaxVal - *pMidVal);
	} else {
		interpResult = 1.0f;
	}
	CTRL->mAnalogR = interpResult;

	// Analog L interpolation -> ctrl->mAnalogL
	if (analogL < *pZeroVal) {
		interpResult = 0.0f;
	} else if (analogL < *pMidVal) {
		interpResult
		    = *pMidLevel * (f32)(s32)(analogL - *pZeroVal) / (f32)(s32)(*pMidVal - *pZeroVal);
	} else if (analogL < *pMaxVal) {
		interpResult
		    = *pMidLevel
		      + (1.0f - *pMidLevel) * (f32)(s32)(analogL - *pMidVal)
		            / (f32)(s32)(*pMaxVal - *pMidVal);
	} else {
		interpResult = 1.0f;
	}
	CTRL->mAnalogL = interpResult;

	// Analog L -> unk10C
	if (analogL < *pZeroVal) {
		interpResult = 0.0f;
	} else if (analogL < *pMidVal) {
		interpResult
		    = *pMidLevel * (f32)(s32)(analogL - *pZeroVal) / (f32)(s32)(*pMidVal - *pZeroVal);
	} else if (analogL < *pMaxVal) {
		interpResult
		    = *pMidLevel
		      + (1.0f - *pMidLevel) * (f32)(s32)(analogL - *pMidVal)
		            / (f32)(s32)(*pMaxVal - *pMidVal);
	} else {
		interpResult = 1.0f;
	}
	*(f32*)((u8*)this + 0x10C) = interpResult;

	// Analog R -> unk110
	if (analogR < *pZeroVal) {
		interpResult = 0.0f;
	} else if (analogR < *pMidVal) {
		interpResult
		    = *pMidLevel * (f32)(s32)(analogR - *pZeroVal) / (f32)(s32)(*pMidVal - *pZeroVal);
	} else if (analogR < *pMaxVal) {
		interpResult
		    = *pMidLevel
		      + (1.0f - *pMidLevel) * (f32)(s32)(analogR - *pMidVal)
		            / (f32)(s32)(*pMaxVal - *pMidVal);
	} else {
		interpResult = 1.0f;
	}
	*(f32*)((u8*)this + 0x110) = interpResult;

	// Deadzone processing for stick H/V
	CTRL->mStickH = 0.0f;
	CTRL->mStickV = 0.0f;

	if (CTRL->mStickHS16 < -7)
		CTRL->mStickH = (f32)(CTRL->mStickHS16 + 6);
	if (CTRL->mStickHS16 > 7)
		CTRL->mStickH = (f32)(CTRL->mStickHS16 - 6);
	if (CTRL->mStickVS16 < -7)
		CTRL->mStickV = (f32)(CTRL->mStickVS16 + 6);
	if (CTRL->mStickVS16 > 7)
		CTRL->mStickV = (f32)(CTRL->mStickVS16 - 6);

	// Compute stick magnitude
	f32 stickH = CTRL->mStickH;
	f32 stickV = CTRL->mStickV;
	f32 dist2  = stickH * stickH + stickV * stickV;
	f32 stickDist = dist2;

	if (dist2 > 0.0f) {
		double guess = __frsqrte((double)dist2);
		guess = .5 * guess * (3.0 - guess * guess * dist2);
		volatile f32 sqrtResult;
		sqrtResult = (f32)(dist2 * guess);
		stickDist  = sqrtResult;
	}

	// Apply decay from mLengthMultTimes
	s32 times = (s32)mControllerParams.mLengthMultTimes.value;
	for (s32 i = 0; i < times; i++) {
		stickDist *= mControllerParams.mLengthMult.value;
	}

	CTRL->mStickDist = stickDist;

	// Cap stick distance at 64.0f
	if (CTRL->mStickDist > 64.0f) {
		f32 scale = 64.0f / CTRL->mStickDist;
		CTRL->mStickH = CTRL->mStickH * scale;
		CTRL->mStickV = CTRL->mStickV * (64.0f / CTRL->mStickDist);
		CTRL->mStickDist = 64.0f;
	}

	// Compute intended magnitude
	f32 normDist     = (1.0f / 64.0f) * CTRL->mStickDist;
	mIntendedMag     = 32.0f * normDist * normDist;

	// Decrement rotation timer
	if (*(s16*)((u8*)this + 0xA0) > 0)
		*(s16*)((u8*)this + 0xA0) = *(s16*)((u8*)this + 0xA0) - 1;

	// Rotation processing
	s32 rotOffset = 0;
	if (*(s16*)((u8*)this + 0xA0) > 0) {
		s16 rotTimer = *(s16*)((u8*)this + 0xA0);
		s16 unk252C  = mGraffitoParams.mDizzyAngleY.value;
		s16 unk2518  = mGraffitoParams.mDizzyWalkCtMax.value;
		f32 unk2540  = mGraffitoParams.mDizzyAngleRate.value;
		f32 unk2554  = mGraffitoParams.mDizzyPowerRate.value;
		f32 unk2568  = mGraffitoParams.mDizzyPower.value;

		u16 sinAngle = (u16)(s32)((f32)rotTimer * unk2540);
		u16 cosAngle = (u16)(s32)((f32)rotTimer * unk2554);

		f32 sinVal = JMASSin(sinAngle);
		f32 cosVal = JMASCos(cosAngle);

		rotOffset = (s32)(sinVal * (f32)unk252C * (f32)unk252C / (f32)unk2518);
		mIntendedMag = mIntendedMag + cosVal * unk2568;
		if (mIntendedMag < 0.0f)
			mIntendedMag = 0.0f;
	}

	// Set intended yaw from stick direction
	if (mIntendedMag > 0.0f) {
		s16 stickAngle = matan(-CTRL->mStickV, CTRL->mStickH);
		s16 camAngle   = *(s16*)((u8*)gpCamera + 0x258);
		mIntendedYaw   = stickAngle + camAngle + rotOffset;
	} else {
		mIntendedYaw = mFaceAngle.y;
	}

	// Watergun turbo nozzle handling
	if (mWaterGun != nullptr) {
		if (mWaterGun->mCurrentNozzle == TWaterGun::Turbo) {
			u8 hasPump;
			if (unk380 == 0) {
				hasPump = 1;
			} else {
				hasPump = 0;
			}
			if (hasPump && mGamePad->mCompSPos[3] > 0.0f
			    && (f32)(s32)mWaterGun->mCurrentWater > 0.0f) {
				// Turbo nozzle active
				if (0.0f == mIntendedMag)
					mIntendedYaw = mFaceAngle.y;

				f32 rotSpeed = mDeParams.mDashAcc.value;

				*(f32*)unkC0 += rotSpeed;
				if (*(f32*)unkC0 > 32.0f) {
					*(f32*)unkC0 = 32.0f;
					*(s16*)(unkC0 + 4) = *(s16*)(unkC0 + 4) + 1;
					if ((f32)*(s16*)(unkC0 + 4)
					    > (f32)mDeParams.mDashStartTime.value) {
						*(s16*)(unkC0 + 4) = mDeParams.mDashStartTime.value;

						u8 hasFlag;
						if (unk118 & 0x4000) {
							hasFlag = 1;
						} else {
							hasFlag = 0;
						}
						if (!hasFlag) {
							TNozzleBase* nozzle
							    = mWaterGun->getCurrentNozzle();
							if (((TNozzleTrigger*)nozzle)->unk385 == 1) {
								unk118 |= 0x4000;
								startSoundActor(0x814);

								u8 isRunning;
								if (mAction & 0x2000) {
									isRunning = 1;
								} else {
									isRunning = 0;
								}
								if (isRunning) {
									changePlayerStatus(
									    0x24D5, 0, false);
								}
							}
						}
					}

					// Check special states
					if ((mAction - 0x04000000u) == 0x440u
					    || mAction == 0x24D5) {
						// keep going
					} else {
						*(s16*)(unkC0 + 4) = 0;
						unk118 &= ~0x4000;
					}
				} else {
					*(s16*)(unkC0 + 4) = 0;
					unk118 &= ~0x4000;
				}

				mIntendedMag = *(f32*)unkC0;
				mWaterGun->rotateProp(*(f32*)unkC0);
			} else {
				if (*(f32*)unkC0 > 0.1f) {
					if (0.0f == mIntendedMag)
						mIntendedYaw = mFaceAngle.y;
					*(f32*)unkC0 *= mDeParams.mDashBrake.value;
					mIntendedMag = *(f32*)unkC0;
				} else {
					*(f32*)unkC0 = 0.0f;
				}
				*(s16*)(unkC0 + 4) = 0;
				unk118 &= ~0x4000;
			}
		} else {
			if (*(f32*)unkC0 > 0.1f) {
				if (0.0f == mIntendedMag)
					mIntendedYaw = mFaceAngle.y;
				*(f32*)unkC0 *= mDeParams.mDashBrake.value;
				mIntendedMag = *(f32*)unkC0;
			} else {
				*(f32*)unkC0 = 0.0f;
			}
			*(s16*)(unkC0 + 4) = 0;
			unk118 &= ~0x4000;
		}

		// Check turbo nozzle prop rotation
		if (mWaterGun->mCurrentNozzle == TWaterGun::Turbo) {
			if ((mAction - 0x0C400000u) == 0x201u
			    || (mAction - 0x04000000u) == 0x440u) {
				f32 propSpeed = mIntendedMag * (1.0f / 32.0f);
				TNozzleBase* nozzle = mWaterGun->getCurrentNozzle();
				*(f32*)((u8*)nozzle + 0x714) = propSpeed;
			}
		}
	}

	// Set stick moved flag
	if (mIntendedMag > 0.0f)
		mInput |= 0x1;

	// B button frame -> input flag 0x2
	if (mGamePad->mEnabledFrameMeaning & TMarioGamePad::MEANING_0x80)
		mInput |= 0x2;

	// A button held -> input flag 0x80
	if (mGamePad->mMeaning & TMarioGamePad::MEANING_0x80)
		mInput |= 0x80;

	// B button on ctrl -> input flag 0x4000
	if (CTRL->mInput & TMarioControllerWork::B)
		mInput |= 0x4000;

	// Check Yoshi
	u8 isOnYoshi = 0;
	if (mYoshi != nullptr) {
		if (mYoshi->onYoshi())
			isOnYoshi = 1;
	}

	// B frame when not on Yoshi
	if (!isOnYoshi) {
		if (CTRL->mFrameInput & TMarioControllerWork::B) {
			mInput |= 0x8000;
			mInput |= 0x2000;
		}
	}

	// L trigger / Z button check
	if ((mGamePad->mEnabledFrameMeaning & TMarioGamePad::MEANING_0x2000)
	    || (CTRL->mFrameInput & 0x40)) {
		u8 isSpecialAction;
		if (mAction & 0x800) {
			isSpecialAction = 1;
		} else {
			isSpecialAction = 0;
		}
		if (isSpecialAction == 1)
			mInput |= 0x8000;
	}

	// FLUDD nozzle switch handling
	u8 hasFluddFlag;
	if (unk118 & 0x8000) {
		hasFluddFlag = 1;
	} else {
		hasFluddFlag = 0;
	}
	if (hasFluddFlag) {
		u8 isTurboActive;
		if (unk118 & 0x4000) {
			isTurboActive = 1;
		} else {
			isTurboActive = 0;
		}
		if (!isTurboActive) {
			u32 meaning = mGamePad->mMeaning;
			if ((meaning & TMarioGamePad::MEANING_0x400)
			    || (meaning & TMarioGamePad::MEANING_0x2000))
				mInput |= 0x200;
			if (mGamePad->mEnabledFrameMeaning & TMarioGamePad::MEANING_0x400)
				mInput |= 0x100;
		}
	}
}

#undef CTRL

void TMario::checkPlayerAction(JDrama::TGraphics* gfx)
{
	mInput = 0;
	checkController(gfx);
	makeHistory();
	checkCurrentPlane();
	checkRideMovement();
	if (!(mInput & 3))
		mInput |= 0x20;
}

void TMario::makeHistory()
{
	if (mIntendedMag > 0.0f) {
		if (unk534 == 0)
			*(s16*)(&unk536) = mFaceAngle.y;

		unk530[unk534] = mIntendedYaw;
		unk534 = unk534 + 1;

		if ((s32)unk534 >= mControllerParams.mStickRotateTime.value) {
			int i = 0;
			int offset = 0;
			while (i < mControllerParams.mStickRotateTime.value) {
				s16* p = (s16*)((u8*)unk530 + offset);
				*p = *(s16*)((u8*)p + 2);
				i++;
				offset += 2;
			}
			unk534 = (u8)(mControllerParams.mStickRotateTime.value - 1);
		}

		s16 diff = (s16)(mIntendedYaw - mFaceAngle.y);
		if (diff >= -0x2000 && diff <= 0x2000) {
			*(s16*)(&unk538) = *(s16*)(&unk538) + 1;
			if (*(s16*)(&unk538) > 0x78) {
				unk53B = 6;
				*(s16*)(&unk538) = 0x78;
			}
		} else {
			*(s16*)(&unk538) = 0;
		}
	} else {
		unk534 = 0;
		*(s16*)(&unk538) = 0;
	}

	if (unk53B != 0) {
		unk53A = 1;
		unk53B = unk53B - 1;
	} else {
		unk53A = 0;
		unk53B = 0;
	}
}

BOOL TMario::checkAllMotions()
{
	u32 flags = mInput;
	if (flags & 0x2) {
		int rotDir;
		BOOL rotResult;
		if (checkStickRotate(&rotDir)) {
			switch (rotDir) {
			case 2:
				changePlayerStatus(0x896, 0, false);
				break;
			case 3:
				changePlayerStatus(0x895, 0, false);
				break;
			}
			rotResult = 1;
		} else {
			rotResult = 0;
		}

		if (rotResult)
			return true;

		return changePlayerStatus(0x02000880, 0, false);
	}

	if (flags & 0x4)
		return changePlayerStatus(0x88C, 0, false);

	if (flags & 0x1)
		return changePlayerStatus(0x04000440, 0, false);

	if (flags & 0x8)
		return changePlayerStatus(0x50, 0, false);

	return false;
}

void TMario::checkGraffitoFire()
{
	u8 shouldSkip;
	if (unk14C > 0) {
		shouldSkip = 1;
	} else {
		u8 flagCheck;
		if (unk118 & 0x8)
			flagCheck = 1;
		else
			flagCheck = 0;
		if (flagCheck) {
			shouldSkip = 1;
		} else if (mAction == 0x89C) {
			shouldSkip = 1;
		} else {
			u8 areaId = *(u8*)((u8*)gpMarDirector + 0x124);
			if (areaId == 3 || areaId == 4) {
				shouldSkip = 1;
			} else {
				u8 inArea = 1;
				if (areaId != 1) {
					if (areaId != 2)
						inArea = 0;
				}
				if (inArea) {
					shouldSkip = 1;
				} else {
					u8 actionBit;
					if (mAction & 0x1000)
						actionBit = 1;
					else
						actionBit = 0;
					if (actionBit)
						shouldSkip = 1;
					else
						shouldSkip = 0;
				}
			}
		}
	}
	if (shouldSkip) return;

	u8 waterFlag;
	if (unk118 & 0x400)
		waterFlag = 1;
	else
		waterFlag = 0;
	if (waterFlag) return;

	if (mPosition.y - mFloorPosition.y > mGraffitoParams.mFireHeight.value)
		return;

	u32 action = mAction;
	if (action == 0x208B7 || action == 0x8000239) {
		*(s16*)((u8*)this + 0x96) += 0x8000;
	}

	f32 savedForwardVel = mForwardVel;
	f32 savedVelY = mVel.y;

	u8* fireType     = (u8*)this + 0x3930;
	u8* fireDamage   = (u8*)this + 0x3944;
	s16* fireRadius  = (s16*)((u8*)this + 0x3980);

	*(f32*)((u8*)this + 0x484) = mPosition.x + JMASSin(*(s16*)((u8*)this + 0x96));
	*(f32*)((u8*)this + 0x48C) = mPosition.z + JMASCos(*(s16*)((u8*)this + 0x96));

	damageExec((THitActor*)((u8*)this + 0x474),
	           mDmgParamsGraffitoFire.mDamage.value,
	           mDmgParamsGraffitoFire.mDownType.value,
	           *fireType,
	           mDmgParamsGraffitoFire.mMinSpeed.value,
	           *fireDamage,
	           mDmgParamsGraffitoFire.mDirty.value,
	           *fireRadius);

	if (*(f32*)((u8*)this + 0x55C) > 0.0f) {
		mVel.y = -savedVelY;
		mForwardVel = savedForwardVel;
	}

	*(s16*)((u8*)this + 0x14C) = mGraffitoParams.mFireInvincibleTime.value;
	dropObject();
	changePlayerStatus(0x208B7, 1, false);
	gpMarioParticleManager->emitAndBindToPosPtr(6, &mPosition, 0, 0);

	if (gpMSound->gateCheck(0x1813)) {
		MSoundSESystem::MSoundSE::startSoundActor(
		    0x1813, (Vec*)&mPosition, 0, (JAISound**)0, 0, 4);
	}
}

void TMario::checkGraffitoSlip()
{
	u8 onSlipSurface;
	if (mPosition.y <= 4.0f + mFloorPosition.y)
		onSlipSurface = 1;
	else
		onSlipSurface = 0;

	if (onSlipSurface) {
		*(s16*)((u8*)this + 0x360) = mDeParams.mFootPrintTimerMax.value;

		u32 action = mAction;
		if (action == 0x84045D || action == 0x4045E) {
			*(f32*)((u8*)this + 0x138) = mDirtyParams.mBrakeStartValSlip.value;
			*(s16*)((u8*)this + 0x13C) = mDirtyParams.mDirtyTimeSlip.value;
		}

		action = mAction;
		if (action - 0x40000 == 0x45C || action - 0x40000 == 0x561) {
			*(f32*)((u8*)this + 0x138) = mDirtyParams.mBrakeStartValRun.value;
			*(s16*)((u8*)this + 0x13C) = mDirtyParams.mDirtyTimeRun.value;
		}

		const TBGCheckData* ground = mGroundPlane;
		if (*(f32*)((u8*)ground + 0x38) <= mDirtyParams.mSlopeAngle.value) {
			*(f32*)((u8*)this + 0x138) = mDirtyParams.mBrakeStartValSlip.value;
			*(s16*)((u8*)this + 0x13C) = mDirtyParams.mDirtyTimeSlip.value;
			changePlayerStatus(0x4045E, 0, false);
			startVoice(0x78D3);
		} else {
			action = mAction;
			if (action == 0x80088A || action == 0x800456
			    || action == 0x84045D || action == 0x4045E) {
				*(f32*)((u8*)this + 0x138) = mDirtyParams.mBrakeStartValSlip.value;
				*(s16*)((u8*)this + 0x13C) = mDirtyParams.mDirtyTimeSlip.value;
				changePlayerStatus(0x84045D, 0, false);
				if (mPrevAction != 0x84045D) {
					startVoice(0x78D3);
				}
			} else if (mAction != 0x386) {
				*(f32*)((u8*)this + 0x138) = mDirtyParams.mBrakeStartValRun.value;
				*(s16*)((u8*)this + 0x13C) = mDirtyParams.mDirtyTimeRun.value;
				if (mAction == 0x560) {
					changePlayerStatus(0x40561, 0, false);
				} else {
					changePlayerStatus(0x4045C, 0, false);
				}
			}
		}

		u8 bit25;
		if (unk118 & 0x40)
			bit25 = 1;
		else
			bit25 = 0;
		if (!bit25) {
			unk34E = mDirtyParams.mFogTimeYellow.value
			         + mDirtyParams.mFogTimeRed.value;
		}
		u16 timer = unk34E;
		unk34E = timer - 1;
		timer = unk34E;
		if (timer != 0) {
			if (timer == mDirtyParams.mFogTimeRed.value) {
				floorDamageExec(1, 3, 0,
				                mMotorParams.mMotorReturn.value);
			}
		} else {
			unk34E = mDirtyParams.mFogTimeYellow.value
			         + mDirtyParams.mFogTimeRed.value;
		}
	} else {
		u32 action = mAction;
		if (action == 0x84045D || action == 0x4045E) {
			*(f32*)((u8*)this + 0x138) = mDirtyParams.mBrakeSlipNoPollute.value;
			*(s16*)((u8*)this + 0x13C) = mDirtyParams.mDirtyTimeSlip.value;
		}
	}
}

void TMario::checkGraffitoElec()
{
	u8 bit25;
	if (unk118 & 0x40)
		bit25 = 1;
	else
		bit25 = 0;
	if (!bit25) {
		unk34E = mDeParams.mGraffitoNoDmgTime.value;
	}

	u16 timer = unk34E;
	if (timer != 0) {
		unk34E = timer - 1;
		return;
	}

	u8 shouldSkip;
	if (unk14C > 0) {
		shouldSkip = 1;
	} else {
		u8 flagCheck;
		if (unk118 & 0x8)
			flagCheck = 1;
		else
			flagCheck = 0;
		if (flagCheck) {
			shouldSkip = 1;
		} else if (mAction == 0x89C) {
			shouldSkip = 1;
		} else {
			u8 areaId = *(u8*)((u8*)gpMarDirector + 0x124);
			if (areaId == 3 || areaId == 4) {
				shouldSkip = 1;
			} else {
				u8 inArea = 1;
				if (areaId != 1) {
					if (areaId != 2)
						inArea = 0;
				}
				if (inArea) {
					shouldSkip = 1;
				} else {
					u8 actionBit;
					if (mAction & 0x1000)
						actionBit = 1;
					else
						actionBit = 0;
					if (actionBit)
						shouldSkip = 1;
					else
						shouldSkip = 0;
				}
			}
		}
	}
	if (shouldSkip) return;

	u32 motionBits = mAction & 0x1C0;
	if (motionBits != 0) {
		if (motionBits != 0x40) return;
	}

	if (*(s16*)((u8*)this + 0x360) > 0) return;

	changePlayerStatus(0x20338, 0, false);

	if (gpMSound->gateCheck(0x1814)) {
		MSoundSESystem::MSoundSE::startSoundActor(
		    0x1814, (Vec*)&mPosition, 0, (JAISound**)0, 0, 4);
	}
	if (gpMSound->gateCheck(0x3806)) {
		MSoundSESystem::MSoundSE::startSoundActor(
		    0x3806, (Vec*)&mPosition, 0, (JAISound**)0, 0, 4);
	}
}

void TMario::checkGraffito()
{
	// Early exit: ground plane has graffito flag
	u8 hasGrafFlag;
	if (mGroundPlane->mFlags & 0x10)
		hasGrafFlag = 1;
	else
		hasGrafFlag = 0;

	if (hasGrafFlag)
		return;

	// Early exit: on Yoshi
	u8 onYoshiCheck = 0;
	if (mYoshi != NULL) {
		if (((TYoshi*)mYoshi)->onYoshi())
			onYoshiCheck = 1;
	}
	if (onYoshiCheck)
		return;

	// Early exit: unk388 state
	if (unk388 == 1)
		return;
	if (unk388 == 2)
		return;

	// Get pollution type at current position
	s32 isPolluted = 0;
	unk350 = gpPollution->getPollutionType(mPosition.x, mPosition.y,
	                                       mPosition.z);

	switch (unk350) {
	case 2:
	case 5:
	case 6: {
		// 3x3 grid check
		isPolluted = 1;
		JGeometry::TVec3<f32> pos(mPosition);
		pos.x -= 38.0f;
		pos.z -= 38.0f;

		// Row 0: check 3 columns
		if (!gpPollution->isPolluted(pos.x, pos.y, pos.z))
			isPolluted = 0;
		pos.x += 38.0f;
		if (!gpPollution->isPolluted(pos.x, pos.y, pos.z))
			isPolluted = 0;
		pos.x += 38.0f;
		if (!gpPollution->isPolluted(pos.x, pos.y, pos.z))
			isPolluted = 0;

		// Row 1: z += 38
		pos.z += 38.0f;
		if (!gpPollution->isPolluted(pos.x, pos.y, pos.z))
			isPolluted = 0;
		pos.x -= 38.0f;
		if (!gpPollution->isPolluted(pos.x, pos.y, pos.z))
			isPolluted = 0;
		pos.x -= 38.0f;
		if (!gpPollution->isPolluted(pos.x, pos.y, pos.z))
			isPolluted = 0;

		// Row 2: z += 38
		pos.z += 38.0f;
		if (!gpPollution->isPolluted(pos.x, pos.y, pos.z))
			isPolluted = 0;
		pos.x += 38.0f;
		if (!gpPollution->isPolluted(pos.x, pos.y, pos.z))
			isPolluted = 0;
		pos.x += 38.0f;
		if (gpPollution->isPolluted(pos.x, pos.y, pos.z))
			break;
		isPolluted = 0;
		break;
	}
	case 0:
	case 1:
	case 3:
	case 7: {
		// Cross pattern check (5 points)
		isPolluted = 1;
		JGeometry::TVec3<f32> pos;
		pos.x = mPosition.x;
		pos.y = *(f32*)((u8*)this + 0xEC);
		pos.z = mPosition.z;

		pos.z -= 38.0f;
		if (!gpPollution->isPolluted(pos.x, pos.y, pos.z))
			isPolluted = 0;
		pos.x += 38.0f;
		pos.z += 38.0f;
		if (!gpPollution->isPolluted(pos.x, pos.y, pos.z))
			isPolluted = 0;
		pos.x -= 38.0f;
		if (!gpPollution->isPolluted(pos.x, pos.y, pos.z))
			isPolluted = 0;
		pos.x -= 38.0f;
		if (!gpPollution->isPolluted(pos.x, pos.y, pos.z))
			isPolluted = 0;
		pos.z += 38.0f;
		pos.x += 38.0f;
		if (gpPollution->isPolluted(pos.x, pos.y, pos.z))
			break;
		isPolluted = 0;
		break;
	}
	case 4: {
		// Single point check
		JGeometry::TVec3<f32> pos;
		pos.x = mPosition.x;
		pos.y = *(f32*)((u8*)this + 0xEC);
		pos.z = mPosition.z;

		if (gpPollution->isPolluted(pos.x, pos.y, pos.z)) {
			isPolluted = 1;
		} else {
			isPolluted = 0;
		}
		break;
	}
	case 8:
	case 10:
		isPolluted = 0;
		break;
	case 9:
	default:
		break;
	}

	// If on Yoshi and polluted, get off
	u8 onYoshi2 = 0;
	if (mYoshi != NULL) {
		if (((TYoshi*)mYoshi)->onYoshi())
			onYoshi2 = 1;
	}
	if (onYoshi2) {
		if (isPolluted == 1) {
			getOffYoshi(true);
		}
	}

	// Second switch: call appropriate graffito handler
	switch (unk350) {
	case 4:
		if (isPolluted == 1)
			checkGraffitoElec();
		break;
	case 1:
	case 7:
		if (isPolluted == 1)
			checkGraffitoFire();
		break;
	case 2:
		if (isPolluted == 1)
			checkGraffitoSlip();
		break;
	case 3:
		if (isPolluted == 1) {
			mPosition.x = *(f32*)((u8*)this + 0x29C);
			mPosition.z = *(f32*)((u8*)this + 0x2A4);
		}
		break;
	default:
		break;
	}

	// Set/clear graffito flag
	if (isPolluted == 1) {
		unk118 |= 0x40;
	} else {
		unk118 &= ~0x40;
	}

	// Check floor proximity for effects
	u8 isOnFloor;
	if (mPosition.y <= *(f32*)((u8*)this + 0xEC) + 8.0f)
		isOnFloor = 1;
	else
		isOnFloor = 0;

	if (!isOnFloor)
		return;

	// Emit pollution effect if standing in it
	if (isPolluted == 1) {
		JGeometry::TVec3<f32>* planeNormal
		    = (JGeometry::TVec3<f32>*)(((u8*)mGroundPlane) + 0x34);
		SMS_EmitSinkInPollutionEffect(
		    *(JGeometry::TVec3<f32>*)&mPosition, *planeNormal, false);
	}

	// Footprint timer
	s16 footTimer = *(s16*)((u8*)this + 0x360);
	if (footTimer <= 0)
		return;

	*(s16*)((u8*)this + 0x360) = footTimer - 1;

	s16 halfDuration = mDeParams.mFootPrintTimerMax.value;
	halfDuration = halfDuration / 2;
	if (*(s16*)((u8*)this + 0x360) <= halfDuration)
		return;

	// Check ground plane flag (inverted)
	u8 groundFlag;
	if (mGroundPlane->mFlags & 0x10)
		groundFlag = 1;
	else
		groundFlag = 0;

	u8 notOnGraffito;
	if (groundFlag == 1)
		notOnGraffito = 0;
	else
		notOnGraffito = 1;

	if (!notOnGraffito)
		return;

	// Check trigger flags
	u8 hasTrigger;
	if (unk118 & 0x30000)
		hasTrigger = 1;
	else
		hasTrigger = 0;

	if (hasTrigger)
		return;

	emitDirtyFootPrint();
}

bool TMario::isInvincible() const
{
	if (*(s16*)((u8*)this + 0x14C) > 0)
		return true;

	u8 hasFlag;
	if (unk118 & 0x8)
		hasFlag = 1;
	else
		hasFlag = 0;

	if (hasFlag)
		return true;

	if (mAction == 0x89C)
		return true;

	u8 areaID = *(u8*)((u8*)gpMarDirector + 0x124);
	if (areaID == 3 || areaID == 4)
		return true;

	u8 isEvent = 1;
	if (areaID != 1) {
		if (areaID != 2)
			isEvent = 0;
	}

	if (isEvent)
		return true;

	u8 hasBit;
	if (mAction & 0x1000)
		hasBit = 1;
	else
		hasBit = 0;

	if (hasBit)
		return true;

	return false;
}

BOOL TMario::isForceSlip()
{
	u16 code = *(u16*)mGroundPlane;
	u8 isIce;
	if (code == 0x01 || code == 0x4001 ||
	    code == 0x8001 || code == 0xC001)
		isIce = 1;
	else
		isIce = 0;

	if (isIce)
		return true;

	if (*(s32*)((u8*)this + 0x350) == 2) {
		u8 hasBit;
		if (unk118 & 0x40)
			hasBit = 1;
		else
			hasBit = 0;

		if (hasBit) {
			if (*(f32*)((u8*)mGroundPlane + 0x38) < mDirtyParams.mSlopeAngle.value)
				return true;
		}
	}

	if (*(f32*)((u8*)mGroundPlane + 0x38) < mDeParams.mForceSlipAngle.value)
		return true;

	return false;
}

bool TMario::isUnderWater() const
{
	u8 inWater;
	if (unk118 & 0x30000)
		inWater = 1;
	else
		inWater = 0;

	if (inWater) {
		f32 floorZ = mFloorPosition.z;
		f32 param = mSwimParams.mCanBreathDepth.value;
		f32 val = *(f32*)((u8*)this + 0x170);
		if (val < floorZ - param)
			return true;
	}
	return false;
}

bool TMario::isWallInFront() const
{
	if (mWallPlane != NULL) {
		s16 wallAngle = getWallAngle();
		s16 diff = (s16)(wallAngle - mFaceAngle.y);
		if (diff < -0x71C7 || diff > 0x71C7)
			return true;
	}
	return false;
}

void TMario::thinkSand()
{
	u8 inWater;
	if (unk118 & 0x30000)
		inWater = 1;
	else
		inWater = 0;

	if (!inWater) {
		u16 code = *(u16*)mGroundPlane;
		u8 isSand;
		if (code == 0x701 || code == 0x4701 ||
		    code == 0x8701 || code == 0xC701)
			isSand = 1;
		else
			isSand = 0;

		if (isSand == 1) {
			unk118 |= 0x40000;
			emitSandEffect();
			return;
		}
	}
	unk118 &= ~0x40000;
}

f32 TMario::getJumpAccelControl() const
{
	if (mAction == 0x892)
		return mWireParams.mWireJumpAccelControl.value;
	return mJumpParams.mJumpAccelControl.value;
}

f32 TMario::getJumpSlideControl() const
{
	if (mAction == 0x892)
		return mWireParams.mWireJumpSlideControl.value;

	u8 riding = 0;
	if (mYoshi != NULL) {
		if (mYoshi->onYoshi())
			riding = 1;
	}

	if (riding) {
		u8 fluttering;
		if (mYoshi->mFlutterState == 1)
			fluttering = 1;
		else
			fluttering = 0;

		if (fluttering)
			return mYoshiParams.mHoldOutSldCtrl.value;
	}

	return mJumpParams.mJumpSlideControl.value;
}

BOOL TMario::considerRotateJumpStart()
{
	int rotDir;
	if (checkStickRotate(&rotDir)) {
		switch (rotDir) {
		case 2:
			changePlayerStatus(0x896, 0, false);
			break;
		case 3:
			changePlayerStatus(0x895, 0, false);
			break;
		}
		return true;
	}
	return false;
}

BOOL TMario::canSquat() const
{
	u8 hasFludd;
	if (unk118 & 0x8000)
		hasFludd = 1;
	else
		hasFludd = 0;

	if (hasFludd) {
		if (mWaterGun != NULL) {
			TNozzleBase* nozzle = mWaterGun->getCurrentNozzle();
			if (*(u8*)((u8*)nozzle + 0x18) != 1) {
				if ((s32)mWaterGun->mCurrentNozzle != 5) {
					if (mInput & 0x200) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

void TMario::thinkDirty()
{
	u8 isDirty;
	if (unk118 & 0x40)
		isDirty = 1;
	else
		isDirty = 0;

	if (isDirty) {
		if (mAction == 0x04000440 || mAction == 0x0004045C) {
			unk134 += mDirtyParams.mIncRunning.value;
		}
		if (mAction == 0x00800456 || mAction == 0x0084045D ||
		    mAction == 0x0004045E) {
			unk134 += mDirtyParams.mIncCatching.value;
		}
		if (mAction == 0x50) {
			unk134 += mDirtyParams.mIncSlipping.value;
		}
	}

	u8 inWater;
	if (unk118 & 0x30000)
		inWater = 1;
	else
		inWater = 0;

	if (inWater) {
		f32 waterLevel = *(f32*)((u8*)this + 0xF0);
		if (mPosition.y > waterLevel - 200.0f)
			meltInWaterEffect();
		*(s16*)((u8*)this + 0x360) = 0;
		unk134 -= mDirtyParams.mDecSwimming.value;
	}

	if (mAction == 0x895 || mAction == 0x896) {
		unk134 -= mDirtyParams.mDecRotJump.value;
		*(s16*)((u8*)this + 0x360) = 0;
	}

	u8 hasShirt;
	if (unk118 & 0x10)
		hasShirt = 1;
	else
		hasShirt = 0;

	if (hasShirt) {
		unk134 -= mDirtyParams.mDecWaterHit.value;
		*(s16*)((u8*)this + 0x360) = 0;
	}

	dirtyLimitCheck();
}

void TMario::checkRideMovement()
{
	TLiveActor* rideActor = 0;

	Vec pos;
	pos = *(Vec*)&mPosition;

	f32 sinAmt = 10.0f * JMASSin((u16)mFaceAngle.y);
	pos.x += 0.8f * sinAmt;
	f32 cosAmt = 10.0f * JMASCos((u16)mFaceAngle.y);
	pos.z += 0.8f * cosAmt;

	const TBGCheckData* wallPlane =
	    checkWallPlane(&pos, 10.0f, *(f32*)((u8*)this + 0x15C));

	TLiveActor* groundActor =
	    (TLiveActor*)mGroundPlane->mActor;

	if (groundActor != 0) {
		u8 actionBit;
		if (mAction & 0x800)
			actionBit = 1;
		else
			actionBit = 0;
		if (!actionBit) {
			u8 nearGround;
			if (mPosition.y <= 4.0f + mFloorPosition.y)
				nearGround = 1;
			else
				nearGround = 0;
			if (nearGround)
				rideActor = groundActor;
		}
	}

	if (groundActor != 0) {
		if (mAction == 0x8008A9) {
			u16 subState = *(u16*)((u8*)this + 0x84);
			if (subState == 2 || subState == 3)
				rideActor = groundActor;
		}
	}

	{
		u8 actionBit2;
		if (mAction & 0x20000000)
			actionBit2 = 1;
		else
			actionBit2 = 0;
		if (actionBit2) {
			if (wallPlane != 0) {
				if (wallPlane->mActor != 0)
					rideActor = (TLiveActor*)wallPlane->mActor;
			}
		}
	}

	if (rideActor != 0) {
		if (*(TLiveActor**)((u8*)this + 0x2C0) != 0
		    && *(TLiveActor**)((u8*)this + 0x2C0) == rideActor) {
			// sameRide
			TLiveActor* cur = *(TLiveActor**)((u8*)this + 0x2C0);
			Mtx stackMtx;
			if (cur->getRootJointMtx() == 0) {
				SMS_GetActorMtx(*cur, stackMtx);
			} else {
				PSMTXCopy((MtxPtr)cur->getRootJointMtx(), stackMtx);
			}

			PSMTXMultVec(stackMtx,
			             (Vec*)((u8*)this + 0x2F4),
			             (Vec*)&mPosition);

			TLiveActor* ride = *(TLiveActor**)((u8*)this + 0x2C0);
			f32 savedRot = *(f32*)((u8*)this + 0x30C);
			f32 currentRot = ride->mRotation.y;
			f32 delta = currentRot - savedRot;
			s16 faceAngle = *(s16*)((u8*)this + 0x96);
			*(s16*)((u8*)this + 0x96) =
			    faceAngle + (int)(32768.0f * delta / 180.0f);

			ride = *(TLiveActor**)((u8*)this + 0x2C0);
			*(f32*)((u8*)this + 0x30C) = ride->mRotation.y;
		} else {
			// newRide
			*(TLiveActor**)((u8*)this + 0x2C0) = rideActor;

			TLiveActor* ride = *(TLiveActor**)((u8*)this + 0x2C0);
			*(f32*)((u8*)this + 0x30C) = ride->mRotation.y;

			ride = *(TLiveActor**)((u8*)this + 0x2C0);
			if (ride != 0) {
				Mtx stackMtx;
				if (ride->getRootJointMtx() == 0) {
					SMS_GetActorMtx(*ride, stackMtx);
				} else {
					PSMTXCopy((MtxPtr)ride->getRootJointMtx(), stackMtx);
				}

				PSMTXInverse(stackMtx, stackMtx);

				*(Vec*)((u8*)this + 0x300) = *(Vec*)((u8*)this + 0x2F4);

				PSMTXMultVec(stackMtx,
				             (Vec*)&mPosition,
				             (Vec*)((u8*)this + 0x2F4));
			}
		}
	} else {
		*(u32*)((u8*)this + 0x2C0) = 0;
	}
}

void TMario::checkCurrentPlane()
{
	u8 bit19;
	if (mAction & 0x1000) {
		bit19 = 1;
	} else {
		bit19 = 0;
	}
	if (bit19)
		return;

	// Check if on Yoshi
	u8 r28 = 0;
	if (mYoshi != nullptr) {
		if (((TYoshi*)mYoshi)->onYoshi()) {
			r28 = 1;
		}
	}

	if (r28) {
		unk15C = 80.0f;
	} else {
		unk15C = 50.0f;
	}

	// First wall check
	TBGWallCheckRecord wallCheck;
	r28 = 0;
	wallCheck.mCenter.x    = mPosition.x;
	wallCheck.mCenter.y    = 60.0f + mPosition.y;
	wallCheck.mCenter.z    = mPosition.z;
	wallCheck.mRadius      = unk15C;
	wallCheck.mMaxResults  = 2;
	wallCheck.mFlags       = 0;
	gpMap->isTouchedWallsAndMoveXZ(&wallCheck);

	// First skip check
	{
		u8 skip;
		if (unk14C > 0) {
			skip = 1;
		} else {
			if (unk118 & 0x8) {
				r28 = 1;
			}
			if (r28) {
				skip = 1;
			} else {
				u32 action = mAction;
				if (action == 0x89C) {
					skip = 1;
				} else {
					u8 dirState
					    = *(u8*)((u8*)gpMarDirector + 0x124);
					if (dirState == 3 || dirState == 4) {
						skip = 1;
					} else {
						u8 isDemo;
						if (dirState == 1
						    || dirState == 2) {
							isDemo = 1;
						} else {
							isDemo = 0;
						}
						if (isDemo) {
							skip = 1;
						} else {
							u8 b19;
							if (action & 0x1000) {
								b19 = 1;
							} else {
								b19 = 0;
							}
							if (b19) {
								skip = 1;
							} else {
								skip = 0;
							}
						}
					}
				}
			}
		}
		if (!skip) {
			// Loop over first wall results
			for (int i = 0; i < wallCheck.mResultWallsNum; i++) {
				TBGCheckData* wall = wallCheck.mResultWalls[i];
				u16 wt = wall->mBGType;
				u8 isDmgWall;
				if (wt == 0xB || wt == 0x800B || wt == 0x103 || wt == 0x101) {
					isDmgWall = 1;
				} else {
					isDmgWall = 0;
				}
				if (!isDmgWall)
					continue;

				TEParams* dmg = getDmgMapCode(wall->mData);
				*(f32*)((u8*)this + 0x484)
				    = mPosition.x + JMASSin((u16)mFaceAngle.y);
				*(f32*)((u8*)this + 0x48C)
				    = mPosition.z + JMASCos((u16)mFaceAngle.y);
				damageExec(&mFloorHitActor,
				           dmg->mDamage.get(), dmg->mDownType.get(),
				           dmg->mWaterEmit.get(), dmg->mMinSpeed.get(),
				           dmg->mMotor.get(), dmg->mDirty.get(),
				           dmg->mInvincibleTime.get());
			}

			// Two-wall crush check
			if (wallCheck.mResultWallsNum == 2) {
				TBGCheckData* wall0 = wallCheck.mResultWalls[0];
				TBGCheckData* wall1 = wallCheck.mResultWalls[1];
				f32 dot = wall0->mNormal.y * wall1->mNormal.y
				          + wall0->mNormal.x * wall1->mNormal.x
				          + wall0->mNormal.z * wall1->mNormal.z;
				if (dot < -0.9f) {
					// Copy position
					JGeometry::TVec3<f32> pos;
					pos.x = mPosition.x;
					pos.y = mPosition.y;
					pos.z = mPosition.z;

					// Compute plane distances
					f32 dist0 = wall0->mNormal.y * pos.y
					            + wall0->mNormal.x * pos.x
					            + wall0->mNormal.z * pos.z
					            + wall0->mPlaneDistance;
					f32 dist1 = wall1->mNormal.y * pos.y
					            + wall1->mNormal.x * pos.x
					            + wall1->mNormal.z * pos.z
					            + wall1->mPlaneDistance;

					// Check actor type - either wall must be type 0x2BD
					u8 doCrush = 0;
					const TLiveActor* actor0 = wall0->getActor();
					if (actor0 != nullptr) {
						u32 actorType = *(u32*)((u8*)actor0 + 0x4C);
						if ((actorType - 0x40000000) == 0x2BD)
							doCrush = 1;
					}

					if (!doCrush) {
						const TLiveActor* actor1 = wall1->getActor();
						if (actor1 != nullptr) {
							u32 actorType1 = *(u32*)((u8*)actor1 + 0x4C);
							if ((actorType1 - 0x40000000) == 0x2BD)
								doCrush = 1;
						}
					}

					if (doCrush) {
						if (dist0 < 10.0f || dist1 < 10.0f) {
							floorDamageExec(mDeParams.mHpMax.value, 3, 0,
							                mMotorParams.mMotorReturn.value);
						}
					}
				}
			}
		}
	}
	// Second wall check (lower, smaller radius)
	u8 r26 = 1;
	r28     = 0;
	wallCheck.mCenter.x   = mPosition.x;
	wallCheck.mCenter.y   = 4.0f + mPosition.y;
	wallCheck.mCenter.z   = mPosition.z;
	wallCheck.mRadius     = 0.5f * unk15C;
	wallCheck.mMaxResults = 1;
	wallCheck.mFlags      = 0;
	gpMap->isTouchedWallsAndMoveXZ(&wallCheck);

	// Skip check for second walls (slightly different logic)
	if (*(s16*)((u8*)this + 0x14C) > 0) {
		r26 = 1;
	} else {
		if (!(*(u32*)((u8*)this + 0x118) & 0x8)) {
			r26 = r28;
		}
		if (!r26) {
			u32 action2 = mAction;
			if (action2 == 0x89C) {
				r26 = 1;
			} else {
				u8 dirState2 = *(u8*)((u8*)gpMarDirector + 0x124);
				if (dirState2 == 3 || dirState2 == 4) {
					r26 = 1;
				} else {
					u8 isDemo2;
					if (dirState2 == 1 || dirState2 == 2) {
						isDemo2 = 1;
					} else {
						isDemo2 = 0;
					}
					if (isDemo2) {
						r26 = 1;
					} else {
						u8 bit19_2;
						if (action2 & 0x1000) {
							bit19_2 = 1;
						} else {
							bit19_2 = 0;
						}
						if (bit19_2) {
							r26 = 1;
						} else {
							r26 = 0;
						}
					}
				}
			}
		}
	}

	if (!r26) {
		for (int i = 0; i < wallCheck.mResultWallsNum; i++) {
			TBGCheckData* wall = wallCheck.mResultWalls[i];
			u16 wt2 = wall->mBGType;
			u8 isDmg2;
			if (wt2 == 0xB || wt2 == 0x800B || wt2 == 0x103
			    || wt2 == 0x101) {
				isDmg2 = 1;
			} else {
				isDmg2 = 0;
			}
			if (!isDmg2)
				continue;

			TEParams* dmg = getDmgMapCode(wall->mData);
			*(f32*)((u8*)this + 0x484) = mPosition.x
			    + JMASSin((u16)mFaceAngle.y);
			*(f32*)((u8*)this + 0x48C) = mPosition.z
			    + JMASCos((u16)mFaceAngle.y);
			damageExec(
			    &mFloorHitActor,
			    dmg->mDamage.get(), dmg->mDownType.get(),
			    dmg->mWaterEmit.get(), dmg->mMinSpeed.get(),
			    dmg->mMotor.get(), dmg->mDirty.get(),
			    dmg->mInvincibleTime.get());
		}
	}

	// Ground check
	f32 f30 = mPosition.z;
	f32 f31 = mPosition.x;
	*(f32*)((u8*)this + 0xEC)
	    = gpMap->checkGround(f31, 25.0f + mPosition.y, f30, &mGroundPlane);

	if (mGroundPlane->isMarioThrough()) {
		*(f32*)((u8*)this + 0xEC) = gpMap->checkGround(
		    f31, *(f32*)((u8*)this + 0xEC) - 1.0f, f30, &mGroundPlane);
	}

	// Roof check
	*(f32*)((u8*)this + 0xE8)
	    = gpMap->checkRoof(mPosition.x, 80.0f + mPosition.y, mPosition.z,
	                       &mRoofPlane);

	// Ground damage check (third skip check)
	{
		u8 skip3;
		if (*(s16*)((u8*)this + 0x14C) > 0) {
			skip3 = 1;
		} else {
			u8 unk118bit;
			if (*(u32*)((u8*)this + 0x118) & 0x8) {
				unk118bit = 1;
			} else {
				unk118bit = 0;
			}
			if (unk118bit) {
				skip3 = 1;
			} else {
				u32 action3 = mAction;
				if (action3 == 0x89C) {
					skip3 = 1;
				} else {
					u8 dirState3
					    = *(u8*)((u8*)gpMarDirector + 0x124);
					if (dirState3 == 3 || dirState3 == 4) {
						skip3 = 1;
					} else {
						u8 isDemo3;
						if (dirState3 == 1
						    || dirState3 == 2) {
							isDemo3 = 1;
						} else {
							isDemo3 = 0;
						}
						if (isDemo3) {
							skip3 = 1;
						} else {
							u8 bit19_3;
							if (action3 & 0x1000) {
								bit19_3 = 1;
							} else {
								bit19_3 = 0;
							}
							if (bit19_3) {
								skip3 = 1;
							} else {
								skip3 = 0;
							}
						}
					}
				}
			}
		}

		if (!skip3) {
			// Check ground damage
			u8 nearGround;
			if (mPosition.y <= 4.0f + *(f32*)((u8*)this + 0xEC)) {
				nearGround = 1;
			} else {
				nearGround = 0;
			}
			if (nearGround) {
				TBGCheckData* ground = mGroundPlane;
				u16 gt = ground->mBGType;
				u8 isDmgG;
				if (gt == 0xB || gt == 0x800B || gt == 0x103
				    || gt == 0x101) {
					isDmgG = 1;
				} else {
					isDmgG = 0;
				}
				if (isDmgG) {
					u8 bit15;
					if (mAction & 0x10000) {
						bit15 = 1;
					} else {
						bit15 = 0;
					}
					if (!bit15) {
						TEParams* dmg
						    = getDmgMapCode(ground->mData);
						*(f32*)((u8*)this + 0x484)
						    = mPosition.x
						      + JMASSin(*(u16*)((u8*)this
						                        + 0x96));
						*(f32*)((u8*)this + 0x48C)
						    = mPosition.z
						      + JMASCos(*(u16*)((u8*)this
						                        + 0x96));
						damageExec(
						    (THitActor*)*(u32*)((u8*)this
						                       + 0x474),
						    dmg->mDamage.get(),
						    dmg->mDownType.get(),
						    dmg->mWaterEmit.get(),
						    dmg->mMinSpeed.get(),
						    dmg->mMotor.get(),
						    dmg->mDirty.get(),
						    dmg->mInvincibleTime.get());
					}
				}
			}

			// Check roof damage
			if (160.0f + mPosition.y > *(f32*)((u8*)this + 0xE8)) {
				TBGCheckData* roof = mRoofPlane;
				u16 rt = roof->mBGType;
				u8 isDmgR;
				if (rt == 0xB || rt == 0x800B || rt == 0x103
				    || rt == 0x101) {
					isDmgR = 1;
				} else {
					isDmgR = 0;
				}
				if (isDmgR) {
					TEParams* dmg = getDmgMapCode(roof->mData);
					*(f32*)((u8*)this + 0x484)
					    = mPosition.x
					      + JMASSin(
					          (u16)mFaceAngle.y);
					*(f32*)((u8*)this + 0x48C)
					    = mPosition.z
					      + JMASCos(
					          (u16)mFaceAngle.y);
					damageExec(
					    &mFloorHitActor,
					    dmg->mDamage.get(),
					    dmg->mDownType.get(),
					    dmg->mWaterEmit.get(),
					    dmg->mMinSpeed.get(),
					    dmg->mMotor.get(), dmg->mDirty.get(),
					    dmg->mInvincibleTime.get());
				}
			}
		}
	}

	// Slope detection
	{
		TBGCheckData* ground = mGroundPlane;
		u8 isLegal;
		if (ground->mFlags & 0x10) {
			isLegal = 1;
		} else {
			isLegal = 0;
		}
		// invert: isLegal means data IS illegal, so !isLegal means legal
		if (isLegal != 1) {
			// compute slope angle
			*(s16*)((u8*)this + 0xF4) = matan(
			    ground->mNormal.z, ground->mNormal.x);

			s32 slipResult;
			if ((u8)isForceSlip()) {
				slipResult = 1;
			} else {
				u16 bgType = ground->mBGType;
				if (bgType == 0xC || bgType == 0x800C
				    || bgType == 0xA00C) {
					slipResult = 1;
				} else if (bgType == 0x2 || bgType == 0x8002) {
					if (ground->mNormal.y < 0.8660254f) {
						slipResult = 1;
					} else {
						slipResult = 0;
					}
				} else if (bgType == 0x3 || bgType == 0x8003) {
					slipResult = 0;
				} else {
					if (ground->mNormal.y
					    < mDeParams.mSlipStart.value) {
						slipResult = 1;
					} else {
						slipResult = 0;
					}
				}
			}

			if (slipResult != 0
			    || (*(u32*)((u8*)this + 0x118) & 0x800)) {
				mInput |= 0x8;
			}

			// High slope check
			if (mPosition.y
			    > 100.0f + *(f32*)((u8*)this + 0xEC)) {
				mInput |= 0x4;
			}
		}
	}

	// Clear bit 20 of unk118
	*(u32*)((u8*)this + 0x118) = *(u32*)((u8*)this + 0x118) & ~0x800;
}
#pragma dont_inline on
TMario::TEParams* TMario::getDmgMapCode(int code) const
{
	switch (code) {
	case 0: return (TEParams*)((u8*)this + 0x3BD4);
	case 1: return (TEParams*)((u8*)this + 0x3C68);
	case 2: return (TEParams*)((u8*)this + 0x3CFC);
	case 3: return (TEParams*)((u8*)this + 0x3D90);
	case 4: return (TEParams*)((u8*)this + 0x3E24);
	case 5: return (TEParams*)((u8*)this + 0x3EB8);
	case 6: return (TEParams*)((u8*)this + 0x3F4C);
	case 7: return (TEParams*)((u8*)this + 0x3FE0);
	case 8: return (TEParams*)((u8*)this + 0x4074);
	case 9: return (TEParams*)((u8*)this + 0x4108);
	default: return (TEParams*)((u8*)this + 0x3BD4);
	}
}
#pragma dont_inline off

void TMario::thinkParams()
{
	*(f32*)((u8*)this + 0x34) =
	    (f32)*(s16*)((u8*)this + 0x96) * (360.0f / 65536.0f);

	{
		s32 invTimer = unk14C;
		if (invTimer > 0) {
			unk14C = invTimer - 1;
		}
	}

	u8 waterFlag;
	if (unk118 & 0x400)
		waterFlag = 1;
	else
		waterFlag = 0;
	if (waterFlag) return;

	u32 motionBits = unk118 & 0x30000;
	{
		u8 hasMotion;
		if (motionBits)
			hasMotion = 1;
		else
			hasMotion = 0;
		if (hasMotion) {
			u8 nonZero;
			if (motionBits)
				nonZero = 1;
			else
				nonZero = 0;

			u8 belowThreshold;
			if (!nonZero) {
				belowThreshold = 0;
			} else if (*(f32*)((u8*)this + 0x170)
			           < *(f32*)((u8*)this + 0xF0) - mSwimParams.mCanBreathDepth.value) {
				belowThreshold = 1;
			} else {
				belowThreshold = 0;
			}
			if (!belowThreshold) {
				u16 bgType = *(u16*)((u8*)mWaterFloor);
				u8 isWaterGround;
				if (bgType == 0x0B || bgType == 0x800B || bgType == 0x103
				    || bgType == 0x101)
					isWaterGround = 1;
				else
					isWaterGround = 0;

				if (isWaterGround) {
					u8 isWaterGround2;
					if (bgType == 0x0B || bgType == 0x800B || bgType == 0x103
					    || bgType == 0x101)
						isWaterGround2 = 1;
					else
						isWaterGround2 = 0;

					if (isWaterGround2) {
						u8 actionBit;
						if (mAction & 0x10000)
							actionBit = 1;
						else
							actionBit = 0;

						if (!actionBit) {
							s16 data = mWaterFloor->mData;
							TEParams* params;
							switch (data) {
							case 0: params = (TEParams*)((u8*)this + 0x3BD4); break;
							case 1: params = (TEParams*)((u8*)this + 0x3C68); break;
							case 2: params = (TEParams*)((u8*)this + 0x3CFC); break;
							case 3: params = (TEParams*)((u8*)this + 0x3D90); break;
							case 4: params = (TEParams*)((u8*)this + 0x3E24); break;
							case 5: params = (TEParams*)((u8*)this + 0x3EB8); break;
							case 6: params = (TEParams*)((u8*)this + 0x3F4C); break;
							case 7: params = (TEParams*)((u8*)this + 0x3FE0); break;
							case 8: params = (TEParams*)((u8*)this + 0x4074); break;
							case 9: params = (TEParams*)((u8*)this + 0x4108); break;
							default: params = (TEParams*)((u8*)this + 0x3BD4); break;
							}
							floorDamageExec(*params);
						}
					}
				}
			}

			// resetCounters
			*(u16*)((u8*)this + 0x126) = 0;
			*(s16*)((u8*)this + 0x128) = mDeParams.mHotTimer.value;
		} else {
			// capBranch
			TMarioCap* cap = *(TMarioCap**)((u8*)this + 0x3E0);
			if (cap != 0) {
				u8 hatOn;
				if (*(u16*)((u8*)cap + 4) & 1)
					hatOn = 1;
				else
					hatOn = 0;
				if (!hatOn) {
					*(u16*)((u8*)this + 0x126) = *(u16*)((u8*)this + 0x126) + 1;
					if ((u16)*(u16*)((u8*)this + 0x126)
					    > (s16)*(s16*)((u8*)this + 0x128)) {
						decHP(1);
						if (gpMSound->gateCheck(0x480C)) {
							MSoundSESystem::MSoundSE::startSoundSystemSE(
							    0x480C, 0, (JAISound**)0, 0);
						}

						*(s16*)((u8*)this + 0x128) = mDeParams.mHotTimer.value;
						*(u16*)((u8*)this + 0x126) = 0;
						rumbleStart(20, mMotorParams.mMotorWall.value);
						*(u16*)((u8*)this + 0x14C) = (int)*(f32*)((u8*)this + 0x55C);
					}
				}
			}
			*(u16*)((u8*)this + 0x122) = 0;
		}
	}
	{
		const TBGCheckData* ground = mGroundPlane;
		if (ground != 0) {
			const TLiveActor* actor = ground->mActor;
			if (actor != 0) {
				if (*(u32*)((u8*)actor + 0x4C) == 0x400002C7) {
					emitFootPrintWithEffect(-1, 66);
				}
			}
		}
	}
}

void TMario::thinkWaterSurface()
{
	// Early out if on water (bit 16 of mAction)
	u8 onWater;
	if (mAction & 0x10000)
		onWater = 1;
	else
		onWater = 0;
	if (onWater)
		return;

	// Check if currently in water (bits 14-15 of unk118)
	u32 waterBits = unk118 & 0x30000;
	u8 wasInWater;
	if (waterBits)
		wasInWater = 1;
	else
		wasInWater = 0;

	u8 r31 = wasInWater;
	u8 r30 = 0;

	u8 wasOnSurface;
	if (waterBits != 0)
		wasOnSurface = 1;
	else
		wasOnSurface = 0;

	if ((u8)wasOnSurface == 1) {
		r30 = 1;
	} else {
		*(f32*)((u8*)this + 0xF0) = mPosition.y;
	}

	// Clear water surface flags
	unk118 &= ~0x10000;
	unk118 &= ~0x20000;

	// Check if ground plane is a pool type
	u16 bgType = mGroundPlane->mBGType;
	u8 isPool;
	if (bgType == BG_TYPE_POOL || bgType == BG_TYPE_INDOOR_POOL
	    || bgType == BG_TYPE_SHADED_POOL)
		isPool = 1;
	else
		isPool = 0;

	if (isPool) {
		*(f32*)((u8*)this + 0xF0) = gpPoolManager->getWaterLevel(mGroundPlane);
		if (*(f32*)((u8*)this + 0xF0) > mPosition.y) {
			r30 = 1;
			unk118 |= 0x10000;
		}
	}

	// Check height above ground with offset
	{
		f32 heightDiff = mPosition.y - *(f32*)((u8*)this + 0x2A0);
		f32 clampedDiff = heightDiff;
		if (heightDiff > 0.0f)
			clampedDiff = 0.0f;

		f32 checkHeight
		    = *(f32*)((u8*)this + 0xF0) - clampedDiff
		      + mSwimParams.mWaterLevelCheckHeight.value;
		f32 groundHeight = gpMap->checkGround(
		    mPosition.x, checkHeight, mPosition.z, &mWaterFloor);

		// Check if ground at water level is a water surface type
		u16 groundType = mWaterFloor->mBGType;
		u8 isWaterSurface;
		if (groundType == BG_TYPE_WATER
		    || groundType == BG_TYPE_DAMAGING_WATER
		    || (u16)(groundType - BG_TYPE_SEA_WATER) <= 3
		    || groundType == BG_TYPE_SHADED_POOL)
			isWaterSurface = 1;
		else
			isWaterSurface = 0;

		if (isWaterSurface) {
			*(f32*)((u8*)this + 0xF0) = groundHeight;
			if (*(f32*)((u8*)this + 0xF0) >= mPosition.y) {
				r30 = 1;
				unk118 |= 0x20000;
			}
		} else {
			// Check ground at current position
			const TBGCheckData* groundCheck2;
			gpMap->checkGround(mPosition.x, mPosition.y, mPosition.z,
			                   &groundCheck2);
			u8 isSpecialType;
			if (groundCheck2->mBGType == 0x810B)
				isSpecialType = 1;
			else
				isSpecialType = 0;

			if (isSpecialType) {
				r30 = 1;
				unk118 |= 0x20000;
			}
		}
	}

	if (r30 != 0) {
		// Water surface logic
		f32 posY2 = mPosition.y;
		f32 waterLvl = *(f32*)((u8*)this + 0xF0);
		if (posY2 < waterLvl) {

		// Check deep water threshold
		f32 deepThreshold = posY2 + mRunParams.mSwimDepth.value;
		if (waterLvl <= deepThreshold) {
			// Deep water - check yoshi
			TYoshi* yoshi = (TYoshi*)mYoshi;
			r30 = 0;
			if (yoshi != NULL) {
				if (yoshi->onYoshi())
					r30 = 1;
			}

			if (r30) {
				yoshi->disappear();
				if (mWaterGun != NULL) {
					mWaterGun->changeNozzle(TWaterGun::Hover, true);
					normalizeNozzle();
				}
			}

			// Check ripple height
			f32 rippleCheck = 160.0f + mPosition.y;
			if (rippleCheck <= *(f32*)((u8*)this + 0xF0))
				rippleEffect();

			swimmingBubbleEffect();

			// Determine if should enter water
			u8 shouldEnter = 1;
			u32 action7C = mAction;
			u8 isBit18;
			if (action7C & 0x2000)
				isBit18 = 1;
			else
				isBit18 = 0;
			u32 statusLow = action7C & 0x1FF;

			if (isBit18)
				shouldEnter = 0;

			// Swimming status range checks
			u8 isSwimming;
			if (statusLow >= 0x168 && statusLow <= 0x16C)
				isSwimming = 1;
			else
				isSwimming = 0;
			if (isSwimming)
				shouldEnter = 0;

			if (statusLow >= 0x145 && statusLow <= 0x14A)
				shouldEnter = 0;

			if (statusLow >= 0x140 && statusLow <= 0x143)
				shouldEnter = 0;

			// Check held object
			u8 holdingObj;
			if (mHolder != 0)
				holdingObj = 1;
			else
				holdingObj = 0;
			if (holdingObj)
				shouldEnter = 0;

			if ((u8)shouldEnter == 1) {
				// Apply water drag
			mForwardVel = mForwardVel * mSwimParams.mStartVMult.value;
			mVel.y = mVel.y * mSwimParams.mStartVYMult.value;

			// Check if falling from air
			u8 isFalling;
			if (mAction & 0x20000)
				isFalling = 1;
			else
				isFalling = 0;

			if (isFalling) {
				// Entering water from air - dive
				changePlayerStatus(0x24DA, 0, true);
			} else {
				// Check if running on ground
				u8 isRunning;
				if (unk118 & 0x4000)
					isRunning = 1;
				else
					isRunning = 0;

				if (isRunning) {
					// Wading
					changePlayerStatus(0x24D5, 0, true);
					mVel.y = 0.0f;
					mPosition.y = *(f32*)((u8*)this + 0xF0);
					startSoundActor(0x828);
				} else {
					// Shallow water entry
					changePlayerStatus(0x22D1, 0, true);
				}
			}
			}
		} else {
			// Shallow water - check frame-based effects
			f32 shallowThreshold = posY2 + mWaterEffectParams.mRunningRippleDepth.value;
			if (waterLvl >= shallowThreshold) {
				// Check if in walking state 0x04000440
				u32 actionVal = mAction;
				if ((u32)(actionVal - 0x04000000) == 0x440) {
					J3DFrameCtrl& frameCtrl = getMotionFrameCtrl();
					if (frameCtrl.checkPass(38.0f) || getMotionFrameCtrl().checkPass(8.0f)) {
						runningRippleEffect();
					}
				}
			} else {
				rippleEffect();
			}
		}
		}
	}


	// Check wet ground type
	{
		u16 gndBGType = mGroundPlane->mBGType;
		u8 isWetGround;
		if (gndBGType == 0x4 || gndBGType == 0x4004
		    || gndBGType == 0x8004 || gndBGType == 0xC004)
			isWetGround = 1;
		else
			isWetGround = 0;

		if (isWetGround) {
			u32 actionVal = mAction;
			if ((u32)(actionVal - 0x04000000) == 0x440) {
				J3DFrameCtrl& frameCtrl = getMotionFrameCtrl();
				if (frameCtrl.checkPass(38.0f) || getMotionFrameCtrl().checkPass(8.0f)) {
					runningRippleEffect();
				}
			}
		}
	}

	// Build water surface matrix
	{
		if (unk118 & 0x30000)
			r30 = 1;
		else
			r30 = 0;

		s16 rotY = *(s16*)((u8*)this + 0x9A);
		J3DGetTranslateRotateMtx(0, rotY, 0, mPosition.x, *(f32*)((u8*)this + 0xF0), mPosition.z,
		                         *(Mtx*)((u8*)this + 0x220));

		// Store water position
		*(f32*)((u8*)this + 0x190) = mPosition.x;
		*(f32*)((u8*)this + 0x194) = *(f32*)((u8*)this + 0xF0);
		*(f32*)((u8*)this + 0x198) = mPosition.z;

		// Copy joint matrix
		u32 modelPtr = *(u32*)((u8*)this + 0x3A8);
		u8 jointIdx = *(u8*)((u8*)this + 0x3CF);
		u32 modelData = *(u32*)(modelPtr + 0x8);
		u32 jointMtxArr = *(u32*)(modelData + 0x58);
		MtxPtr anmMtx = (MtxPtr)(jointMtxArr + jointIdx * 0x30);
		PSMTXCopy(anmMtx, *(Mtx*)((u8*)this + 0x1C0));

		// Check if water state changed
		if (r30 != r31) {
			inOutWaterEffect(*(f32*)((u8*)this + 0xF0));
			f32 splashHeight = *(f32*)((u8*)this + 0xF0) - *(f32*)((u8*)this + 0xEC);

			if (r31 == 1 && r30 == 0) {
				// Entering water
				*(s16*)((u8*)this + 0x362) = 0x78;

				if (splashHeight < 32.0f) {
					// Small splash
					if (gpMSound->gateCheck(0x1939)) {
						MSoundSESystem::MSoundSE::startSoundActor(
						    0x1939, &mPosition, 0, (JAISound**)NULL, 0, 4);
					}
				} else if (splashHeight < 80.0f) {
					// Medium splash
					if (gpMSound->gateCheck(0x181D)) {
						MSoundSESystem::MSoundSE::startSoundActor(
						    0x181D, &mPosition, 0, (JAISound**)NULL, 0, 4);
					}
				} else {
					// Large splash
					if (gpMSound->gateCheck(0x181E)) {
						MSoundSESystem::MSoundSE::startSoundActor(
						    0x181E, &mPosition, 0, (JAISound**)NULL, 0, 4);
					}
				}
			} else {
				// Exiting water
				if (splashHeight < 32.0f) {
					if (gpMSound->gateCheck(0x1938)) {
						MSoundSESystem::MSoundSE::startSoundActor(
						    0x1938, &mPosition, 0, (JAISound**)NULL, 0, 4);
					}
				} else if (splashHeight < 80.0f) {
					if (gpMSound->gateCheck(0x1805)) {
						MSoundSESystem::MSoundSE::startSoundActor(
						    0x1805, &mPosition, 0, (JAISound**)NULL, 0, 4);
					}
				} else {
					if (gpMSound->gateCheck(0x1806)) {
						MSoundSESystem::MSoundSE::startSoundActor(
						    0x1806, &mPosition, 0, (JAISound**)NULL, 0, 4);
					}
				}
			}
		}
	}

	// Drowning/air recovery logic
	{
		u8 shouldDrown = 0;

		u8 isInWater2;
		if (unk118 & 0x30000)
			isInWater2 = 1;
		else
			isInWater2 = 0;

		if (isInWater2) {
			f32 airThreshold = *(f32*)((u8*)this + 0xF0) - mSwimParams.mCanBreathDepth.value;
			if (*(f32*)((u8*)this + 0x170) < airThreshold) {
				shouldDrown = 1;
			}
		}
		if (!shouldDrown) {
			u8 isDiving;
			if (unk118 & 0x8000)
				isDiving = 1;
			else
				isDiving = 0;
			if (isDiving)
				shouldDrown = 1;
		}

		if (shouldDrown) {
			f32 prevAir = *(f32*)((u8*)this + 0x12C);
			u8 isHelm;
			isHelm = isWearingHelm();
			if (isHelm) {
				u32 actionVal = mAction;
				if ((u32)(actionVal - 0x10020000) != 0x370) {
					*(f32*)((u8*)this + 0x12C) -= mSwimParams.mAirDecDive.value;
				}
			} else {
				*(f32*)((u8*)this + 0x12C) -= mSwimParams.mAirDec.value;
			}
			f32 currentAir = *(f32*)((u8*)this + 0x12C);

			// Compare truncated values to detect crossing
			s32 prevInt;
			s32 currInt;
			{
				f32 prev = prevAir;
				f32 curr = currentAir;
				// fctiwz + store/load pattern
				volatile s32 prevTrunc = (s32)prev;
				volatile s32 currTrunc = (s32)curr;
				prevInt = prevTrunc;
				currInt = currTrunc;
			}

			if (prevInt != currInt) {
				rumbleStart(0x14, mMotorParams.mMotorWall.value);
				volatile s32 truncHP = (s32)(*(f32*)((u8*)this + 0x55C));
				*(s16*)((u8*)this + 0x14C) = (s16)truncHP;
			}

			if (*(f32*)((u8*)this + 0x12C) < 1.0f) {
				*(f32*)((u8*)this + 0x12C) = 0.0f;
				loserExec();
				changePlayerStatus(0x000224E0, 0, false);
			}
			return;
		} else {
			*(f32*)((u8*)this + 0x12C) += mSwimParams.mAirInc.value;
			if (*(f32*)((u8*)this + 0x12C) >= *(f32*)((u8*)this + 0x130)) {
				*(f32*)((u8*)this + 0x12C) = *(f32*)((u8*)this + 0x130);
			}
		}
	}
}

void TMario::thinkSituation()
{
	// Save previous situation flags
	unk11C = unk118;

	// Recovery timer
	f32 recoveryVal = unkBC;
	if (recoveryVal < 0.0f) {
		unkBC = recoveryVal + mJumpParams.mTrampolineDec.value;
	} else {
		unkBC = 0.0f;
	}

	// If being held, set position from holder's taking matrix
	if (mHolder != NULL) {
		if (mHolder->getTakingMtx() != NULL) {
			mPosition.x = mHolder->getTakingMtx()[0][3];
			mPosition.y = mHolder->getTakingMtx()[1][3];
			mPosition.z = mHolder->getTakingMtx()[2][3];
		}
	}

	// Clear slippery and shadow bits
	unk118 &= ~0x2;
	unk118 &= ~0x20;

	// Check slippery ground
	{
		u8 isSlippery;
		u16 bgType = mGroundPlane->mBGType;
		if (bgType == 0x106 || bgType == 0x108)
			isSlippery = 1;
		else
			isSlippery = 0;
		if (isSlippery) {
			unk118 |= 0x2;
		}
	}

	// Ground damage check
	{
		u8 didDamage = 0;
		if (isMario()) {
			u8 isOnGround;
			if (mPosition.y <= mFloorPosition.y + 4.0f)
				isOnGround = 1;
			else
				isOnGround = 0;

			if (isOnGround) {
				if (mAction != 0x133F) {
					u8 hasDmgFlag;
					if (mGroundPlane->mFlags & 0x10)
						hasDmgFlag = 1;
					else
						hasDmgFlag = 0;

					u8 isDmgType;
					if (mGroundPlane->mBGType == 0x600)
						isDmgType = 1;
					else
						isDmgType = 0;

					if (hasDmgFlag || isDmgType) {
						unk2BA += mDeParams.mIllegalPlaneCtInc.value;
						if (unk2BA > mDeParams.mIllegalPlaneTime.value) {
							decHP(mDeParams.mHpMax.value);
						}
						didDamage = 1;
					}
				}
			}
		}

		if (!didDamage) {
			unk2BA -= 1;
			if (unk2BA < 0)
				unk2BA = 0;
		}
	}

	// BGM handling for slippery ground
	if (isMario()) {
		u8 curSlippery;
		if (unk118 & 0x2)
			curSlippery = 1;
		else
			curSlippery = 0;

		if (curSlippery == 1) {
			u8 prevSlippery;
			if (unk11C & 0x2)
				prevSlippery = 1;
			else
				prevSlippery = 0;
			if (!prevSlippery) {
				MSBgm::startBGM(0x8001001B);
			}
		}

		u8 curSlippery2;
		if (unk118 & 0x2)
			curSlippery2 = 1;
		else
			curSlippery2 = 0;

		if (!curSlippery2) {
			u8 prevSlippery2;
			if (unk11C & 0x2)
				prevSlippery2 = 1;
			else
				prevSlippery2 = 0;
			if (prevSlippery2 == 1) {
				MSBgm::stopBGM(0x8001001B, 10);
			}
		}
	}

	// Death plane check
	{
		const TBGCheckData* checkPlane;
		f32 groundHeight = gpMap->checkGround(
		    mPosition.x, mPosition.y - mVel.y, mPosition.z, &checkPlane);

		u8 isDeathPlane;
		if (checkPlane->mBGType == 0x800)
			isDeathPlane = 1;
		else
			isDeathPlane = 0;

		if (isDeathPlane) {
			if (groundHeight > mPosition.y) {
				unk118 |= 0x400;
				mHealth = 0;
				mAnmSound->stop();
				if (mYoshi != NULL)
					mYoshi->kill();
				changePlayerStatus(0x208B9, 0, true);
				if (mAnimationId != 0x120) {
					startSoundActor(0x786B);
				}
				*(u16*)((u8*)gpCamera + 0x64) |= 0x800;
				*(u16*)((u8*)gpMarDirector + 0x4E) |= 0x8;
				return;
			}
		}
	}

	// Ground collision matrix setup
	J3DGetTranslateRotateMtx(0, mModelFaceAngle, 0, mPosition.x, mPosition.y,
	                         mPosition.z, *(Mtx*)((u8*)this + 0x1F0));

	// Light/shadow setup
	mLightID = 0;

	{
		u8 hasShadowBit = 0;
		if (mGroundPlane->mBGType & 0x4000)
			hasShadowBit = 1;
		if (hasShadowBit) {
			if (mFloorPosition.y + 200.0f > mPosition.y) {
				mLightID = mGroundPlane->mData;
				if (mLightID == 1) {
					unk118 |= 0x20;
				}
			}
		}
	}

	// Cube shadow check
	if (gpCubeShadow != NULL) {
		if ((s32)gpCubeShadow->getInCubeNo(*(Vec*)&mPosition) != -1) {
			mLightID = 1;
		}
	}

	// Water particle ground check
	unk118 &= ~0x10;
	if (isMario()) {
		u8 isOnGround;
		if (mPosition.y <= mFloorPosition.y + 4.0f)
			isOnGround = 1;
		else
			isOnGround = 0;
		if (isOnGround) {
			if (gpModelWaterManager->askHitWaterParticleOnGround(
			        *(JGeometry::TVec3<f32>*)&mPosition)) {
				unk118 |= 0x10;
			}
		}
	}

	// Stage warp check (ground type 0x300)
	{
		u8 isWarpGround;
		if (mGroundPlane->mBGType == 0x300)
			isWarpGround = 1;
		else
			isWarpGround = 0;

		if (isWarpGround) {
			u8 isOnYoshi = 0;
			if (mYoshi != NULL) {
				if (mYoshi->onYoshi())
					isOnYoshi = 1;
			}
			if (isOnYoshi) {
				getOffYoshi(false);
			}

			gpMarDirector->setNextStage(mGroundPlane->mData,
			                            (JDrama::TActor*)NULL);
			unk114 &= ~0x2;
			unk114 &= ~0x400;
		}
	}

	// Option map position constraints
	if (SMS_isOptionMap()) {
		mPosition.z = mOptionParams.mZ.value;
		if (mPosition.x < mOptionParams.mXMin.value)
			mPosition.x = mOptionParams.mXMin.value;
		if (mPosition.x > mOptionParams.mXMax.value)
			mPosition.x = mOptionParams.mXMax.value;
	}

	// Save ground Y when not airborne
	{
		u8 isAirborne;
		if (mAction & 0x800)
			isAirborne = 1;
		else
			isAirborne = 0;
		if (!isAirborne) {
			unk2BC = mPosition.y;
		}
	}

	calcGroundMtx(*(JGeometry::TVec3<f32>*)&mPosition);

	// Area type check for indoor flag
	{
		u8 areaID = *(u8*)((u8*)gpMarDirector + 0x124);
		u8 isIndoor;
		if (areaID == 1 || areaID == 2)
			isIndoor = 1;
		else
			isIndoor = 0;

		u8 hasWaterBit;
		if (mAction & 0x1000)
			hasWaterBit = 1;
		else
			hasWaterBit = 0;

		if (areaID == 3 || areaID == 4 || isIndoor || hasWaterBit) {
			unk118 |= 0x8;
		} else {
			unk118 &= ~0x8;
		}
	}
}

void TMario::getOffYoshi(bool knockedOff)
{
	mInput &= ~0x8000;
	if (knockedOff) {
		changePlayerStatus(0x89C, 0, false);
		mYoshi->getOff(true);
	} else {
		changePlayerStatus(0x883, 0, false);
		mVel.y = mJumpParams.mGetOffYoshiY.value;
		mYoshi->getOff(false);
	}
	setAnimation(0x4D, 1.0f);
	unk78 &= ~0x100;
	mPosition.y += 100.0f;
	mForwardVel = -8.0f;
	mWaterGun->changeNozzle(4, true);
	normalizeNozzle();
	TWaterGun* gun = mWaterGun;
	TNozzleBase* nozzle = gun->getCurrentNozzle();
	*(s32*)((u8*)gun + 0x1C80) = *(s32*)((u8*)nozzle + 0xCC);
}

void TMario::thinkYoshiHeadCollision()
{
	u8 riding = 0;
	if (mYoshi != NULL) {
		if (mYoshi->onYoshi())
			riding = 1;
	}
	if (!riding)
		return;

	JGeometry::TVec3<f32> tempPos(mPosition);
	int maxResults = 4;
	int flags      = 0;

	f32 headOffset = mYoshiParams.mHeadFront.value;
	u32 angleIdx   = static_cast<u16>(mFaceAngle.y) >> jmaSinShift;
	tempPos.x += jmaSinTable[angleIdx] * headOffset;
	tempPos.z += jmaCosTable[angleIdx] * headOffset;

	TBGWallCheckRecord wallCheck;
	wallCheck.mCenter.x = tempPos.x;
	wallCheck.mCenter.y = 100.0f + tempPos.y;
	f32 savedZ          = tempPos.z;
	wallCheck.mCenter.z = savedZ;
	wallCheck.mRadius     = mYoshiParams.mHeadRadius.value;
	wallCheck.mMaxResults = maxResults;
	wallCheck.mFlags      = flags;

	if (gpMap->isTouchedWallsAndMoveXZ(&wallCheck) != true)
		return;

	f32 dz    = wallCheck.mCenter.z - savedZ;
	f32 dx    = wallCheck.mCenter.x - tempPos.x;
	f32 distSq = dz * dz + dx * dx;
	f32 dist;
	if (distSq > 0.0f)
		dist = sqrtf(distSq);
	else
		dist = distSq;

	f32 pushDist = dist;
	if (dist <= 0.0f)
		return;

	f32 invDist = 1.0f / dist;
	f32 maxPush = 50.0f;
	dx *= invDist;
	dz *= invDist;
	if (maxPush < dist)
		pushDist = maxPush;

	dx *= pushDist;
	mPosition.x += dx;
	dz *= pushDist;
	mPosition.z += dz;
}

void TMario::checkWet()
{
	if (!isMario())
		return;

	u8 onYoshiFlag = 0;
	if (mYoshi != NULL) {
		if (mYoshi->onYoshi())
			onYoshiFlag = 1;
	}
	if (onYoshiFlag)
		return;

	s16 wetTimer = *(s16*)((u8*)this + 0x362);
	if (wetTimer <= 0)
		return;
	*(s16*)((u8*)this + 0x362) = wetTimer - 1;

	const TBGCheckData* check;
	f32 posZ = mPosition.z;
	f32 posX = mPosition.x;
	f32 groundY =
	    gpMap->checkGround(posX, 320.0f + mPosition.y, posZ, &check);

	if (check->isMarioThrough()) {
		groundY =
		    gpMap->checkGround(posX, groundY - 1.0f, posZ, &check);
	}

	u16 bgType = check->mBGType;
	u8 isWater;
	if (bgType == 0x100 || bgType == 0x101
	    || (u16)(bgType - 0x102) <= 3 || bgType == 0x4104)
		isWater = 1;
	else
		isWater = 0;

	if (isWater)
		return;

	if (*(f32*)((u8*)this + 0xF0) > mPosition.y)
		return;

	u8 actionCheck;
	if (mAction & 0x200)
		actionCheck = 1;
	else
		actionCheck = 0;
	if (actionCheck)
		return;

	if (*(s16*)((u8*)this + 0x362) & 7)
		return;

	TWaterEmitInfo* emitInfo = (TWaterEmitInfo*)*(u32*)((u8*)this + 0x158);
	*(JGeometry::TVec3<f32>*)((u8*)emitInfo + 0x70) = mPosition;
	*(f32*)((u8*)emitInfo + 0x74) += 5.0f;

	JGeometry::TVec3<f32> vel(*(JGeometry::TVec3<f32>*)&dummyMactorStringValue1);
	vel.x = 0.3f * mVel.x;
	vel.y = 0.3f * mVel.y;
	vel.z = 0.3f * mVel.z;
	*(JGeometry::TVec3<f32>*)((u8*)emitInfo + 0x8C) = vel;

	gpModelWaterManager->emitRequest(*emitInfo);
}

void TMario::checkEnforceJump()
{
	u8 groundFlag;
	if (mGroundPlane->mFlags & 0x10)
		groundFlag = 1;
	else
		groundFlag = 0;

	u8 notWall;
	if (groundFlag == 1)
		notWall = 0;
	else
		notWall = 1;

	if (!notWall)
		return;

	u8 isTrampoline;
	if (mGroundPlane->mBGType == 0x7 || mGroundPlane->mBGType == 0x8007)
		isTrampoline = 1;
	else
		isTrampoline = 0;

	if (!isTrampoline)
		return;

	u8 yCheck;
	if (mPosition.y <= 4.0f + *(f32*)((u8*)this + 0xEC))
		yCheck = 1;
	else
		yCheck = 0;

	if (!yCheck)
		return;

	if (!(*(u32*)((u8*)this + 0x80) & 0x800))
		return;

	gpMSound->startForceJumpSound((Vec*)&mPosition, *(u32*)((u8*)this + 0x4E8),
	                              0.0f, (u32)*(s16*)((u8*)mGroundPlane + 2));
	startVoice(0x78B9);
	changePlayerStatus(0x884, 0, false);
	rumbleStart(0x15, mMotorParams.mMotorWall.value);

	TLiveActor* groundActor = (TLiveActor*)mGroundPlane->mActor;
	if (groundActor != NULL) {
		((THitActor*)groundActor)->receiveMessage((THitActor*)this, 0);
	}
}

void TMario::checkReturn()
{
	u8 shouldReturn;

	if (*(s16*)((u8*)this + 0x14C) > 0) {
		shouldReturn = 1;
	} else {
		u8 hasFlag;
		if (unk118 & 0x8)
			hasFlag = 1;
		else
			hasFlag = 0;

		if (hasFlag) {
			shouldReturn = 1;
		} else if (mAction == 0x89C) {
			shouldReturn = 1;
		} else {
			u8 areaID = *(u8*)((u8*)gpMarDirector + 0x124);
			if (areaID == 3 || areaID == 4) {
				shouldReturn = 1;
			} else {
				u8 isEvent = 1;
				if (areaID != 1) {
					if (areaID != 2)
						isEvent = 0;
				}

				if (isEvent) {
					shouldReturn = 1;
				} else {
					u8 hasBit;
					if (mAction & 0x1000)
						hasBit = 1;
					else
						hasBit = 0;

					if (hasBit)
						shouldReturn = 1;
					else
						shouldReturn = 0;
				}
			}
		}
	}

	if (shouldReturn)
		return;

	u8 groundFlag;
	if (*(u16*)((u8*)mGroundPlane + 4) & 0x10)
		groundFlag = 1;
	else
		groundFlag = 0;

	u8 isSafe;
	if (groundFlag == 1)
		isSafe = 0;
	else
		isSafe = 1;

	if (!isSafe)
		return;

	*(JGeometry::TVec3<f32>*)((u8*)this + 0x2A8) = mPosition;
	*(u32*)((u8*)this + 0x2B4) = *(u32*)((u8*)this + 0x94);
	*(u16*)((u8*)this + 0x2B8) = *(u16*)((u8*)this + 0x98);
}

BOOL TMario::checkStickRotate(int* outDirection)
{
	int increasing = 0;
	int decreasing = 0;
	int count      = unk534;

	volatile int q[4];
	for (int i = 0; i < count - 1; i++) {
		s16 angle = unk530[i];
		f32 val   = (f32)angle;

		if (val < -24576.0f || val > 24576.0f)
			q[0] = 1;
		if (-24576.0f <= val && val <= -8192.0f)
			q[1] = 1;
		if (-8192.0f < val && val < 8192.0f)
			q[2] = 1;
		if (8192.0f <= val && val <= 24576.0f)
			q[3] = 1;

		f32 next = (f32)unk530[i + 1];
		if (val < next)
			increasing++;
		else
			decreasing++;
	}

	int quadrants = 0;
	if (q[0] == 1)
		quadrants++;
	if (q[1] == 1)
		quadrants++;
	if (q[2] == 1)
		quadrants++;
	if (q[3] == 1)
		quadrants++;

	if (quadrants >= 4) {
		if (increasing > decreasing)
			*outDirection = 2;
		else
			*outDirection = 3;
		return true;
	}

	*outDirection = 4;
	return false;
}

f32 TMario::getSlideStopCatch()
{
	const TBGCheckData* plane = mGroundPlane;
	u16 bgType = plane->mBGType;

	u8 isSand;
	if (bgType == 0x1 || bgType == 0x4001 || bgType == 0x8001 || bgType == 0xC001)
		isSand = 1;
	else
		isSand = 0;

	u8 shouldSlip;
	if (isSand) {
		shouldSlip = 1;
	} else {
		shouldSlip = 0;
		if (unk350 == 2) {
			u8 hasFlag;
			if (unk118 & 0x40)
				hasFlag = 1;
			else
				hasFlag = 0;
			if (hasFlag) {
				if (plane->getNormal().y < mDirtyParams.mSlopeAngle.value)
					shouldSlip = 1;
			}
		}
		if (!shouldSlip) {
			if (plane->getNormal().y < mDeParams.mForceSlipAngle.value)
				shouldSlip = 1;
			else
				shouldSlip = 0;
		}
	}

	if (shouldSlip)
		return mSlipParamsAll.mSlideStopCatch.value;

	u8 isTypeC;
	if (bgType == 0xC || bgType == 0x800C || bgType == 0xA00C)
		isTypeC = 1;
	else
		isTypeC = 0;
	if (isTypeC)
		return mSlipParamsAllSlider.mSlideStopCatch.value;

	u8 isType2;
	if (bgType == 0x2 || bgType == 0x8002)
		isType2 = 1;
	else
		isType2 = 0;
	if (isType2) {
		if (plane->getNormal().y < 0.0f)
			return mSlipParams45.mSlideStopCatch.value;
	}

	u8 isType4;
	if (bgType == 0x4 || bgType == 0x4004 || bgType == 0x8004 || bgType == 0xC004)
		isType4 = 1;
	else
		isType4 = 0;
	if (isType4) {
		if (plane->getNormal().y > 0.0f)
			return mSlipParamsWaterGround.mSlideStopCatch.value;
		return mSlipParamsWaterSlope.mSlideStopCatch.value;
	}

	return mSlipParamsNormal.mSlideStopCatch.value;
}

f32 TMario::getSlideStopNormal()
{
	const TBGCheckData* plane = mGroundPlane;
	u16 bgType = plane->mBGType;

	u8 isSand;
	if (bgType == 0x1 || bgType == 0x4001 || bgType == 0x8001 || bgType == 0xC001)
		isSand = 1;
	else
		isSand = 0;

	u8 shouldSlip;
	if (isSand) {
		shouldSlip = 1;
	} else {
		shouldSlip = 0;
		if (unk350 == 2) {
			u8 hasFlag;
			if (unk118 & 0x40)
				hasFlag = 1;
			else
				hasFlag = 0;
			if (hasFlag) {
				if (plane->getNormal().y < mDirtyParams.mSlopeAngle.value)
					shouldSlip = 1;
			}
		}
		if (!shouldSlip) {
			if (plane->getNormal().y < mDeParams.mForceSlipAngle.value)
				shouldSlip = 1;
			else
				shouldSlip = 0;
		}
	}

	if (shouldSlip)
		return mSlipParamsAll.mSlideStopNormal.value;

	u8 isTypeC;
	if (bgType == 0xC || bgType == 0x800C || bgType == 0xA00C)
		isTypeC = 1;
	else
		isTypeC = 0;
	if (isTypeC)
		return mSlipParamsAllSlider.mSlideStopNormal.value;

	u8 isType2;
	if (bgType == 0x2 || bgType == 0x8002)
		isType2 = 1;
	else
		isType2 = 0;
	if (isType2) {
		if (plane->getNormal().y < 0.0f)
			return mSlipParams45.mSlideStopNormal.value;
	}

	u8 isType4;
	if (bgType == 0x4 || bgType == 0x4004 || bgType == 0x8004 || bgType == 0xC004)
		isType4 = 1;
	else
		isType4 = 0;
	if (isType4) {
		if (plane->getNormal().y > 0.0f)
			return mSlipParamsWaterGround.mSlideStopNormal.value;
	}

	return mSlipParamsWaterSlope.mSlideStopNormal.value;
}

BOOL TMario::canSlipJump()
{
	const TBGCheckData* plane = mGroundPlane;
	u16 bgType = plane->mBGType;

	// Sand types
	u8 isSand;
	if (bgType == 0x1 || bgType == 0x4001 || bgType == 0x8001 || bgType == 0xC001)
		isSand = 1;
	else
		isSand = 0;

	u8 shouldSlip;
	if (isSand) {
		shouldSlip = 1;
	} else {
		shouldSlip = 0;
		if (unk350 == 2) {
			u8 hasFlag;
			if (unk118 & 0x40)
				hasFlag = 1;
			else
				hasFlag = 0;
			if (hasFlag) {
				if (plane->getNormal().y < mDirtyParams.mSlopeAngle.value)
					shouldSlip = 1;
			}
		}
		if (!shouldSlip) {
			if (plane->getNormal().y < mDeParams.mForceSlipAngle.value)
				shouldSlip = 1;
			else
				shouldSlip = 0;
		}
	}

	if (shouldSlip)
		return *((u8*)this + 0x2BB8);

	// Type 0xC
	u8 isTypeC;
	if (bgType == 0xC || bgType == 0x800C || bgType == 0xA00C)
		isTypeC = 1;
	else
		isTypeC = 0;
	if (isTypeC)
		return *((u8*)this + 0x2C9C);

	// Type 2
	u8 isType2;
	if (bgType == 0x2 || bgType == 0x8002)
		isType2 = 1;
	else
		isType2 = 0;
	if (isType2)
		return *((u8*)this + 0x2D80);

	// Type 4 with slope check
	u8 isType4;
	if (bgType == 0x4 || bgType == 0x4004 || bgType == 0x8004 || bgType == 0xC004)
		isType4 = 1;
	else
		isType4 = 0;
	if (isType4) {
		if (plane->getNormal().y > 0.0f)
			return *((u8*)this + 0x2F48);
		return *((u8*)this + 0x2E64);
	}

	// Type 3
	u8 isType3;
	if (bgType == 0x3 || bgType == 0x8003)
		isType3 = 1;
	else
		isType3 = 0;
	if (isType3)
		return true;

	return true;
}

BOOL TMario::isSlipStart()
{
	const TBGCheckData* plane = mGroundPlane;
	u16 bgType = plane->mBGType;

	// Sand types always slip
	u8 isSand;
	if (bgType == 0x1 || bgType == 0x4001 || bgType == 0x8001 || bgType == 0xC001)
		isSand = 1;
	else
		isSand = 0;

	u8 shouldSlip;
	if (isSand) {
		shouldSlip = 1;
	} else {
		shouldSlip = 0;
		if (unk350 == 2) {
			u8 hasFlag;
			if (unk118 & 0x40)
				hasFlag = 1;
			else
				hasFlag = 0;
			if (hasFlag) {
				if (plane->getNormal().y < mDirtyParams.mSlopeAngle.value)
					shouldSlip = 1;
			}
		}
		if (!shouldSlip) {
			if (plane->getNormal().y < mDeParams.mForceSlipAngle.value)
				shouldSlip = 1;
			else
				shouldSlip = 0;
		}
	}

	if (shouldSlip)
		return true;

	// Type 0xC (graffito?) types
	u8 isTypeC;
	if (bgType == 0xC || bgType == 0x800C || bgType == 0xA00C)
		isTypeC = 1;
	else
		isTypeC = 0;
	if (isTypeC)
		return true;

	// Type 2 (wet surface) with slope check
	u8 isType2;
	if (bgType == 0x2 || bgType == 0x8002)
		isType2 = 1;
	else
		isType2 = 0;
	if (isType2) {
		if (plane->getNormal().y < 0.0f)
			return true;
	}

	// Type 3 - explicitly NOT slippery
	u8 isType3;
	if (bgType == 0x3 || bgType == 0x8003)
		isType3 = 1;
	else
		isType3 = 0;
	if (isType3)
		return false;

	// Default slope check
	if (plane->getNormal().y < mDeParams.mSlipStart.value)
		return true;
	return false;
}

const TBGCheckData* TMario::checkWallPlane(Vec* pos, f32 height, f32 radius)
{
	TBGCheckData* result = 0;
	TBGWallCheckRecord record(pos->x, pos->y + height, pos->z, radius, 4, 0);

	u8 touched = gpMap->isTouchedWallsAndMoveXZ(&record);
	if (touched == 1) {
		int numWalls = record.mResultWallsNum;
		for (int i = 0; i < numWalls; i++) {
			TBGCheckData* wall = record.mResultWalls[i];
			if (wall->mActor == mRidingActor) {
				result = wall;
				break;
			}
			f32 dist = wall->getNormal().y * pos->y + wall->getNormal().x * pos->x
			           + wall->getNormal().z * pos->z + wall->getPlaneDistance();
			if (dist < 0.0f)
				dist = -dist;
			if (dist < radius) {
				result = wall;
				radius = dist;
			}
		}
	}

	pos->x = record.mCenter.x;
	pos->z = record.mCenter.z;
	return result;
}

void TMario::thinkHeight()
{
	u8 isAirborne;
	if (mAction & 0x800)
		isAirborne = 1;
	else
		isAirborne = 0;

	if (isAirborne) {
		f32 heightAboveGround = mPosition.y - mFloorPosition.y;
		if (unk36C < heightAboveGround)
			unk36C = heightAboveGround;
	} else {
		unk36C = 0.0f;
	}

	JGeometry::TVec3<f32> forwardPos;
	forwardPos.x = mPosition.x + unk15C * JMASSin(mFaceAngle.y);
	forwardPos.y = mPosition.y;
	forwardPos.z = mPosition.z + unk15C * JMASCos(mFaceAngle.y);

	if (checkWallPlane(&forwardPos, 80.0f, unk15C) == NULL) {
		const TBGCheckData* groundPlane;
		f32 sinV = JMASSin(mFaceAngle.y);
		f32 cosV = JMASCos(mFaceAngle.y);
		f32 dx = 100.0f * sinV;
		f32 dz = 100.0f * cosV;
		f32 groundHeight = gpMap->checkGround(
		    mPosition.x + dx,
		    100.0f + mPosition.y,
		    mPosition.z + dz,
		    &groundPlane);
		unk370 = mPosition.y - groundHeight;
	} else {
		unk370 = 0.0f;
	}
}

void TMario::checkSink()
{
	u8 shouldSkip;
	if (unk14C > 0) {
		shouldSkip = 1;
	} else {
		u8 flagCheck;
		if (unk118 & 0x8)
			flagCheck = 1;
		else
			flagCheck = 0;
		if (flagCheck) {
			shouldSkip = 1;
		} else if (mAction == 0x89C) {
			shouldSkip = 1;
		} else {
			u8 areaId = *(u8*)((u8*)gpMarDirector + 0x124);
			if (areaId == 3 || areaId == 4) {
				shouldSkip = 1;
			} else {
				u8 inArea = 1;
				if (areaId != 1) {
					if (areaId != 2)
						inArea = 0;
				}
				if (inArea) {
					shouldSkip = 1;
				} else {
					u8 actionBit;
					if (mAction & 0x1000)
						actionBit = 1;
					else
						actionBit = 0;
					if (actionBit)
						shouldSkip = 1;
					else
						shouldSkip = 0;
				}
			}
		}
	}
	if (shouldSkip) return;

	u8 groundBit;
	if (mGroundPlane->mFlags & 0x10)
		groundBit = 1;
	else
		groundBit = 0;
	if (groundBit) return;

	if (100.0f + mFloorPosition.y < mPosition.y) {
		*(f32*)((u8*)this + 0x368) = 0.0f;
		return;
	}

	s32 sinkState = *(s32*)((u8*)this + 0x350);
	u8 sinkHandled = 0;

	if (sinkState == 0) {
		u8 bit6;
		if (unk118 & 0x40)
			bit6 = 1;
		else
			bit6 = 0;
		if (bit6) {
			*(f32*)((u8*)this + 0x368) += 1.0f;
			*(s16*)((u8*)this + 0x360) = mDeParams.mFootPrintTimerMax.value;

			if (*(s16*)((u8*)this + 0x120) > 0) {
				f32 limit = (f32)mGraffitoParams.mSinkTime.value
				            * mGraffitoParams.mSinkDmgDepth.value;
				if (*(f32*)((u8*)this + 0x368) > limit)
					*(f32*)((u8*)this + 0x368) = limit;
			}

			s16 interval = mGraffitoParams.mSinkDmgTime.value;
			if (gpMarDirector->unk58 % interval == 0) {
				floorDamageExec(1, 3, 0,
				                mMotorParams.mMotorReturn.value);
			}

			if (gpMSound->gateCheck(0x100B)) {
				MSoundSESystem::MSoundSE::startSoundActor(
				    0x100B, (Vec*)&mPosition, 0, (JAISound**)0, 0, 4);
			}

			if (*(f32*)((u8*)this + 0x368) > (f32)mGraffitoParams.mSinkTime.value) {
				loserExec();
				changePlayerStatus(0x10001123, 0, false);
			}

			SMS_EmitSinkInPollutionEffect(
			    mPosition,
			    *(JGeometry::TVec3<f32>*)((u8*)mGroundPlane + 0x34),
			    true);
			startVoice(0x7865);
			sinkHandled = 1;
		}
	}

	if (!sinkHandled && sinkState == 5) {
		u8 bit6;
		if (unk118 & 0x40)
			bit6 = 1;
		else
			bit6 = 0;
		if (bit6) {
			*(f32*)((u8*)this + 0x374) -= mJumpParams.mGravity.value;
			*(f32*)((u8*)this + 0x378) += *(f32*)((u8*)this + 0x374);
			mVel.x = 0.0f;
			mVel.y = 0.0f;
			mVel.z = 0.0f;
			mForwardVel = 0.0f;
			*(f32*)((u8*)this + 0xB4) = 0.0f;
			*(f32*)((u8*)this + 0xB8) = 0.0f;
			loserExec();
			changePlayerStatus(0x10001123, 0, false);
			sinkHandled = 1;
		}
	}

	if (!sinkHandled) {
		*(f32*)((u8*)this + 0x374) = 0.0f;
		*(f32*)((u8*)this + 0x378) = 0.0f;
		*(f32*)((u8*)this + 0x368) = 0.0f;
	}
}

void TMario::getRidingMtx(MtxPtr outMtx)
{
	if (mRidingActor->getRootJointMtx() == NULL) {
		SMS_GetActorMtx(*mRidingActor, outMtx);
	} else {
		PSMTXCopy(*(mRidingActor->getRootJointMtx()), outMtx);
	}
}

void TMario::playerControl(JDrama::TGraphics* gfx)
{
	// Save angle and position history
	*(s16*)((u8*)this + 0x9C) = mFaceAngle.y;
	*(JGeometry::TVec3<f32>*)((u8*)this + 0x29C) = mPosition;
	unk114 &= ~0x8;

	// Scene 1: force status change
	u8 areaID = *(u8*)((u8*)gpMarDirector + 0x124);
	if (areaID == 1) {
		if ((mAction - 0x10000000) != 0x1308) {
			changePlayerStatus(0x10001308, 0, false);
		}
	}

	// Camera angle adjustment for original Mario
	if (gpMarioOriginal == this) {
		if ((u8)gpCamera->isLButtonCameraSpecifyMode(
		        *(int*)((u8*)gpCamera + 0x50))) {
			u32 actionLow = mAction & 0x1FF;
			if (!(actionLow >= 0x14B && actionLow <= 0x14F)) {
				if (*(u8*)((u8*)gpMarDirector + 0x124) != 1) {
					s16 camAngle = *(s16*)((u8*)gpCamera + 0x258);
					s16 offsetY = gpCamera->getOffsetAngleY();
					mFaceAngle.y =
					    (s16)((camAngle + 0x8000) - offsetY);
				}
			}
		}
	}

	// Inlined checkPlayerAction
	mInput = 0;
	checkController(gfx);
	makeHistory();
	checkCurrentPlane();
	checkRideMovement();
	if (!(mInput & 3))
		mInput |= 0x20;

	checkCollision();
	considerTake();

	// Yoshi check
	u8 isOnYoshi = 0;
	if (mYoshi != NULL) {
		if (mYoshi->onYoshi())
			isOnYoshi = 1;
	}
	if (isOnYoshi) {
		if (*(u32*)((u8*)mGamePad + 0xD4) & 0x200000) {
			getOffYoshi(false);
		}
	}

	thinkYoshiHeadCollision();

	// Coaster angle interpolation
	s16 stickValue = *(s16*)unk108;
	f32 rate = mDeParams.mToroccoRotSp.value;
	mToroccoAngle = (s16)((f32)stickValue * rate + (f32)mToroccoAngle);

	stateMachine();
	stateMachineUpper();
	thinkSituation();
	thinkWaterSurface();

	// Sand effect handling
	u8 hasSandFlags;
	if (unk118 & 0x30000)
		hasSandFlags = 1;
	else
		hasSandFlags = 0;

	if (!hasSandFlags) {
		u16 bgType = mGroundPlane->mBGType;
		u8 isSandGround;
		if (bgType == 0x0701 || bgType == 0x4701 || bgType == 0x8701
		    || bgType == 0xC701)
			isSandGround = 1;
		else
			isSandGround = 0;
		if (isSandGround) {
			unk118 |= 0x40000;
			emitSandEffect();
		} else {
			unk118 &= ~0x40000;
		}
	} else {
		unk118 &= ~0x40000;
	}

	// Inlined thinkHeight
	{
		u8 isAirborne;
		if (mAction & 0x800)
			isAirborne = 1;
		else
			isAirborne = 0;

		if (isAirborne) {
			f32 heightAboveGround = mPosition.y - mFloorPosition.y;
			if (unk36C < heightAboveGround)
				unk36C = heightAboveGround;
		} else {
			unk36C = 0.0f;
		}

		JGeometry::TVec3<f32> forwardPos;
		forwardPos.x = mPosition.x + unk15C * JMASSin(mFaceAngle.y);
		forwardPos.y = mPosition.y;
		forwardPos.z = mPosition.z + unk15C * JMASCos(mFaceAngle.y);

		if (checkWallPlane(&forwardPos, 80.0f, unk15C) == NULL) {
			const TBGCheckData* groundPlane;
			f32 sinV = JMASSin(mFaceAngle.y);
			f32 cosV = JMASCos(mFaceAngle.y);
			f32 dx = 100.0f * sinV;
			f32 dz = 100.0f * cosV;
			f32 groundHeight = gpMap->checkGround(
			    mPosition.x + dx,
			    100.0f + mPosition.y,
			    mPosition.z + dz,
			    &groundPlane);
			unk370 = mPosition.y - groundHeight;
		} else {
			unk370 = 0.0f;
		}
	}

	thinkParams();

	// Inlined checkRideReCalc
	if (mRidingActor != NULL) {
		Mtx localMtx;
		if (mRidingActor->getRootJointMtx() == NULL) {
			SMS_GetActorMtx(*mRidingActor, localMtx);
		} else {
			PSMTXCopy(*(mRidingActor->getRootJointMtx()), localMtx);
		}
		PSMTXInverse(localMtx, localMtx);

		*(JGeometry::TVec3<f32>*)((u8*)this + 0x300) =
		    *(JGeometry::TVec3<f32>*)((u8*)this + 0x2F4);

		PSMTXMultVec(localMtx, (Vec*)&mPosition,
		             (Vec*)((u8*)this + 0x2F4));
	}

	checkWet();
	checkGraffito();
	thinkDirty();
	checkSink();
	gunExec();

	// Stop sound if not in specific action
	if (mAction != 0x208B8) {
		if (mSound != NULL) {
			mSound->stop(1);
		}
	}
}

void TMario::gunExec()
{
	u8 isOnYoshi = 0;
	u8 isYoshiActive = 0;

	if (mYoshi != NULL) {
		if (((TYoshi*)mYoshi)->onYoshi())
			isOnYoshi = 1;
	}
	if (isOnYoshi)
		isYoshiActive = 1;

	if (!isYoshiActive)
		*(u8*)((u8*)gpModelWaterManager + 0x5D5F) = 0;

	// Guard: need gun flag or be on Yoshi
	u8 hasGunFlag;
	if (unk118 & 0x8000)
		hasGunFlag = 1;
	else
		hasGunFlag = 0;

	if (!hasGunFlag) {
		u8 onYoshi2 = 0;
		if (mYoshi != NULL) {
			if (((TYoshi*)mYoshi)->onYoshi())
				onYoshi2 = 1;
		}
		if (!onYoshi2)
			return;
	}

	// Main body
	TWaterGun* waterGun = mWaterGun;
	u8 hasTrigger = 0;
	waterGun->mIsEmitWater = false;

	// Interpolation math
	TNozzleBase* nozzle0 = waterGun->mNozzleList[0];
	TNozzleBase* curNozzle = waterGun->getCurrentNozzle();
	s16 decRate = curNozzle->mEmitParams.mDecRate.get();
	s32 amountMax = nozzle0->mEmitParams.mAmountMax.get();
	waterGun->unk1C88 += 10.0f * ((f32)decRate / (f32)amountMax);

	// Trigger pressure
	waterGun->triggerPressureMovement(
	    *(TMarioControllerWork*)unk108);

	// Clear bit 0x80
	unk118 &= ~0x80;

	// Check trigger bits
	if (unk118 & 0x30000)
		hasTrigger = 1;

	if (hasTrigger) {
		// Suck section
		u8 onYoshi3 = 0;
		if (mYoshi != NULL) {
			if (((TYoshi*)mYoshi)->onYoshi())
				onYoshi3 = 1;
		}

		if (!onYoshi3) {
			if (mWaterGun->suck() == 1) {
				unk118 |= 0x80;

				u8 hasWaterFlag;
				if (unk118 & 0x10000)
					hasWaterFlag = 1;
				else
					hasWaterFlag = 0;

				if (hasWaterFlag) {
					u16 bgType = mGroundPlane->mBGType;
					u8 isPoolType;
					if (bgType == 0x104 || bgType == 0x105
					    || bgType == 0x4104)
						isPoolType = 1;
					else
						isPoolType = 0;

					if (isPoolType)
						gpPoolManager->subWaterLevel(mGroundPlane);
				}
			}
		}

		// Water level check
		curNozzle = waterGun->getCurrentNozzle();
		if (waterGun->mCurrentWater
		    == curNozzle->mEmitParams.mAmountMax.get()) {
			if (unk380 == 0) {
				waterGun->emit();
				waterGun->mCurrentWater
				    = waterGun->getCurrentNozzle()
				          ->mEmitParams.mAmountMax.get();
			}
		}
	} else {
		// No trigger
		if (unk380 == 0) {
			mWaterGun->emit();
		}
	}

	// Yoshi analog R check
	u8 onYoshi4 = 0;
	if (mYoshi != NULL) {
		if (((TYoshi*)mYoshi)->onYoshi())
			onYoshi4 = 1;
	}

	if (onYoshi4) {
		if (((TMarioControllerWork*)unk108)->mAnalogR > 0.0f) {
			mWaterGun->emit();
		}
	}

	// unk114 bit 7 -> refill water
	u8 hasBit0;
	if (unk114 & 0x80)
		hasBit0 = 1;
	else
		hasBit0 = 0;

	if (hasBit0) {
		TWaterGun* wg = mWaterGun;
		wg->mCurrentWater
		    = wg->getCurrentNozzle()->mEmitParams.mAmountMax.get();
	}

	// Action-based nozzle backup
	if (mAction != 0x883) {
		if (mAction != 0x208B8) {
			if (mGamePad->mEnabledFrameMeaning & 0x200000) {
				u8 onYoshi5 = 0;
				if (mYoshi != NULL) {
					if (((TYoshi*)mYoshi)->onYoshi())
						onYoshi5 = 1;
				}

				if (!onYoshi5) {
					if (mAction != 0x800447) {
						mWaterGun->changeBackup();
					}
				}
			}
		}
	}

	// Pollution cleaning (spray nozzle only, while emitting)
	if (mWaterGun->mCurrentNozzle != 0)
		return;

	if (!mWaterGun->mIsEmitWater)
		return;

	s32 sinIdx = (s32)(u16)mFaceAngle.y >> jmaSinShift;
	JGeometry::TVec3<f32> dir;
	dir.x = jmaSinTable[sinIdx];
	dir.y = 0.0f;
	dir.z = jmaCosTable[sinIdx];

	for (s32 i = 0; i < mGraffitoParams.mFootEraseTimes.value; i++) {
		f32 dist = mGraffitoParams.mFootEraseFront.value;
		f32 radius = mGraffitoParams.mFootEraseSize.value;

		JGeometry::TVec3<f32> tempDir(dir);
		PSVECScale((Vec*)&tempDir, (Vec*)&tempDir, dist);

		JGeometry::TVec3<f32> pos(mPosition);
		PSVECAdd((Vec*)&pos, (Vec*)&tempDir, (Vec*)&pos);

		gpPollution->clean(pos.x, pos.y, pos.z, radius);
	}
}

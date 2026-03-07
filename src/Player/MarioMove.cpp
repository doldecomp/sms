#include <Player/MarioMain.hpp>

#include <Map/Map.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <System/MarDirector.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <Player/Watergun.hpp>
#include <Strategic/LiveActor.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <MSound/MSound.hpp>
#include <Map/MapData.hpp>
#include <dolphin/mtx.h>

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

BOOL TMario::changePlayerDropping(u32 status, u32 arg)
{
	dropObject();
	return changePlayerStatus(status, arg, false);
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
	mAttackRadius = *(f32*)((u8*)this + 0x708);
	calcEntryRadius();
	mAttackHeight = *(f32*)((u8*)this + 0x744);
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
	BOOL result = false;
	if (mYoshi != NULL) {
		if (mYoshi->onYoshi())
			result = true;
	}
	return result;
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
	f32 maxDirty = *(f32*)((u8*)this + 0x2700);
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

void TMario::checkPlayerAround(int angleOffset, f32 distance)
{
	u16 angle = mFaceAngle.y + angleOffset;
	f32 sinVal = JMASSin(angle) * distance;
	f32 cosVal = JMASCos(angle) * distance;
	const TBGCheckData* result;
	gpMap->checkGround(mPosition.x + sinVal, mPosition.y + 100.0f,
	                   mPosition.z + cosVal, &result);
}

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
		if (*(u8*)((u8*)this + 0x534) == 0)
			*(s16*)((u8*)this + 0x536) = mFaceAngle.y;

		s16* history = *(s16**)((u8*)this + 0x530);
		history[*(u8*)((u8*)this + 0x534)] = mIntendedYaw;
		*(u8*)((u8*)this + 0x534) = *(u8*)((u8*)this + 0x534) + 1;

		if ((s32)*(u8*)((u8*)this + 0x534) >= *(s16*)((u8*)this + 0x23BC)) {
			int i = 0;
			int offset = 0;
			while (i < *(s16*)((u8*)this + 0x23BC)) {
				s16* p = (s16*)((u8*)*(s16**)((u8*)this + 0x530) + offset);
				*p = *(s16*)((u8*)p + 2);
				i++;
				offset += 2;
			}
			*(u8*)((u8*)this + 0x534) = (u8)(*(s16*)((u8*)this + 0x23BC) - 1);
		}

		s16 diff = (s16)(mIntendedYaw - mFaceAngle.y);
		if (diff < -0x2000 || diff > 0x2000) {
			*(s16*)((u8*)this + 0x538) = 0;
		} else {
			*(s16*)((u8*)this + 0x538) = *(s16*)((u8*)this + 0x538) + 1;
			if (*(s16*)((u8*)this + 0x538) > 0x78) {
				*(u8*)((u8*)this + 0x53B) = 6;
				*(s16*)((u8*)this + 0x538) = 0x78;
			}
		}
	} else {
		*(u8*)((u8*)this + 0x534) = 0;
		*(s16*)((u8*)this + 0x538) = 0;
	}

	if (*(u8*)((u8*)this + 0x53B) != 0) {
		*(u8*)((u8*)this + 0x53A) = 1;
		*(u8*)((u8*)this + 0x53B) = *(u8*)((u8*)this + 0x53B) - 1;
	} else {
		*(u8*)((u8*)this + 0x53A) = 0;
		*(u8*)((u8*)this + 0x53B) = 0;
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
			if (*(f32*)((u8*)mGroundPlane + 0x38) < *(f32*)((u8*)this + 0x26EC))
				return true;
		}
	}

	if (*(f32*)((u8*)mGroundPlane + 0x38) < *(f32*)((u8*)this + 0x8D4))
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
		f32 param = *(f32*)((u8*)this + 0x1244);
		f32 val = *(f32*)((u8*)this + 0x170);
		if (val < floorZ - param)
			return true;
	}
	return false;
}

bool TMario::isWallInFront() const
{
	if (mWallPlane == NULL)
		return false;

	s16 wallAngle = getWallAngle();
	s16 diff = (s16)(wallAngle - mFaceAngle.y);
	if (diff < -0x71C7 || diff > 0x71C7)
		return true;
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
		return *(f32*)((u8*)this + 0x1414);
	return *(f32*)((u8*)this + 0x0B68);
}

f32 TMario::getJumpSlideControl() const
{
	if (mAction == 0x892)
		return *(f32*)((u8*)this + 0x1428);

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
			return *(f32*)((u8*)this + 0x2294);
	}

	return *(f32*)((u8*)this + 0x0B7C);
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
				if (mWaterGun->mCurrentNozzle != 5) {
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
			unk134 += *(f32*)((u8*)this + 0x25D4);
		}
		if (mAction == 0x00800456 || mAction == 0x0084045D ||
		    mAction == 0x0004045E) {
			unk134 += *(f32*)((u8*)this + 0x25E8);
		}
		if (mAction == 0x50) {
			unk134 += *(f32*)((u8*)this + 0x25FC);
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
		unk134 -= *(f32*)((u8*)this + 0x2610);
	}

	if (mAction == 0x895 || mAction == 0x896) {
		unk134 -= *(f32*)((u8*)this + 0x2638);
		*(s16*)((u8*)this + 0x360) = 0;
	}

	u8 hasShirt;
	if (unk118 & 0x10)
		hasShirt = 1;
	else
		hasShirt = 0;

	if (hasShirt) {
		unk134 -= *(f32*)((u8*)this + 0x2624);
		*(s16*)((u8*)this + 0x360) = 0;
	}

	dirtyLimitCheck();
}

void TMario::getOffYoshi(bool knockedOff)
{
	mInput &= ~0x8000;
	if (knockedOff) {
		changePlayerStatus(0x89C, 0, false);
		mYoshi->getOff(true);
	} else {
		changePlayerStatus(0x883, 0, false);
		mVel.y = *(f32*)((u8*)this + 0xE88);
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

void TMario::checkEnforceJump()
{
	const TBGCheckData* ground = mGroundPlane;

	u8 groundFlag;
	if (ground->mFlags & 0x10)
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
	if (ground->mBGType == 0x7 || ground->mBGType == 0x8007)
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
	                              0.0f, (u32)ground->mData);
	startVoice(0x78B9);
	changePlayerStatus(0x884, 0, false);
	rumbleStart(*(s16*)((u8*)this + 0x27F8), 0x15);

	const TLiveActor* groundActor = ground->mActor;
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

	*(u32*)((u8*)this + 0x2A8) = *(u32*)((u8*)this + 0x10);
	*(u32*)((u8*)this + 0x2AC) = *(u32*)((u8*)this + 0x14);
	*(u32*)((u8*)this + 0x2B0) = *(u32*)((u8*)this + 0x18);
	*(u32*)((u8*)this + 0x2B4) = *(u32*)((u8*)this + 0x94);
	*(u16*)((u8*)this + 0x2B8) = *(u16*)((u8*)this + 0x98);
}

void TMario::getRidingMtx(MtxPtr outMtx)
{
	if ((*(TLiveActor**)((u8*)this + 0x2C0))->getRootJointMtx() == NULL) {
		SMS_GetActorMtx(**(TLiveActor**)((u8*)this + 0x2C0), outMtx);
	} else {
		PSMTXCopy(*(*(TLiveActor**)((u8*)this + 0x2C0))->getRootJointMtx(), outMtx);
	}
}

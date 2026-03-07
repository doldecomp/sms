#include <Player/MarioMain.hpp>

#include <Map/Map.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <System/MarDirector.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

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

bool TMario::onYoshi() const
{
	bool result = false;
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
	s16 angle = mFaceAngle.y;
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

BOOL TMario::isWallInFront() const
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

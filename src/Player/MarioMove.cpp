#include <Player/MarioMain.hpp>
#include <JSystem/JMath.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/PollutionManager.hpp>
#include <Strategic/LiveActor.hpp>
#include <System/MarDirector.hpp>
#include <System/StageUtil.hpp>
#include <M3DUtil/M3UModelMario.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

f32 TMario::getJumpAccelControl() const
{
	if (mAction == 0x892)
		return mWireParams.mWireJumpAccelControl.get();

	return mJumpParams.mJumpAccelControl.get();
}

f32 TMario::getJumpSlideControl() const
{
	if (mAction == 0x892)
		return mWireParams.mWireJumpSlideControl.get();

	if (onYoshi() && (mYoshi->mFlutterState == 1 ? true : false))
		return mYoshiParams.mHoldOutSldCtrl.get();

	return mJumpParams.mJumpSlideControl.get();
}

bool TMario::canSquat() const
{
	if ((unk118 & 0x8000 ? true : false) && mWaterGun
	    && ((const TWaterGun*)mWaterGun)
	               ->getCurrentNozzle()
	               ->mEmitParams.mRocketType.get()
	           != 1
	    && (int)mWaterGun->mCurrentNozzle != 5 && (mInput & 0x200))
		return true;
	else
		return false;
}

bool TMario::isUnderWater() const
{
	if ((unk118 & 0x30000 ? true : false)
	    && unk160[1].y < mFloorPosition.z - mSwimParams.mCanBreathDepth.get())
		return true;
	else
		return false;
}

bool TMario::isInvincible() const
{
	if (unk14C > 0)
		return true;

	if (unk118 & 0x8 ? true : false)
		return true;

	if (mAction == 0x89C)
		return true;

	if (gpMarDirector->isDemoMode3() || gpMarDirector->isDemoMode4()
	    || gpMarDirector->isTalkModeNow() || (mAction & 0x1000 ? true : false))
		return true;

	return false;
}

bool TMario::isWallInFront() const
{
	if (mWallPlane != nullptr) {
		s16 diff = getWallAngle() - mFaceAngle.y;
		if (diff < -0x71C7 || diff > 0x71C7)
			return true;
	}
	return false;
}

void TMario::getRidingMtx(Mtx) { }

bool TMario::isForceSlip()
{
	if (mGroundPlane->isUnk1())
		return true;

	if (unk350 == 2) {
		if (unk118 & 0x40 ? true : false) {
			if (mGroundPlane->mNormal.y < mDirtyParams.mSlopeAngle.get())
				return true;
		}
	}

	if (mGroundPlane->mNormal.y < mDeParams.mForceSlipAngle.get())
		return true;

	return false;
}

BOOL TMario::moveRequest(const JGeometry::TVec3<f32>&) { }

void TMario::warpRequest(const JGeometry::TVec3<f32>& pos, f32 angle)
{
	JGeometry::TVec3<f32> offset = pos;
	offset.sub(mPosition);
	moveRequest(pos);
	mFaceAngle.y    = (s16)DEG2SHORTANGLE(angle);
	mModelFaceAngle = mFaceAngle.y;
	gpCamera->addMoveCameraAndMario(offset);
	if (gpMarDirector->mMap != 7)
		mGamePad->onNeutralMarioKey();
	changePlayerStatus(0x0C400201, 0, 1);
}

void TMario::flowMove(const JGeometry::TVec3<f32>& flow)
{
	if ((mAction & 0x2000 ? true : false) == true) {
		mPosition.x += flow.x;
		mPosition.y += flow.y;
		mPosition.z += flow.z;
	}
}

void TMario::windMove(const JGeometry::TVec3<f32>& wind)
{
	mPosition.x += wind.x;
	mPosition.y += wind.y;
	mPosition.z += wind.z;
}

void TMario::getGroundJumpPower() const { }

BOOL TMario::onYoshi() const { return mYoshi != nullptr && mYoshi->onYoshi(); }

void TMario::addVelocity(f32 param_1)
{
	mForwardVel += param_1;
	if (mForwardVel > 99.0f)
		mForwardVel = 99.0f;
}

BOOL TMario::considerRotateJumpStart()
{
	int spC;

	if (checkStickRotate(&spC) == 0)
		return 0;

	switch (spC) {
	case 2:
		changePlayerStatus(0x896, 0, 0);
		break;
	case 3:
		changePlayerStatus(0x895, 0, 0);
		break;
	}
	return 1;
}

BOOL TMario::canBendBody()
{
	u32 act = mAction & 0x1FF;
	if (act >= 0x14B && act <= 0x14F)
		return FALSE;
	if (act >= 0x140 && act <= 0x143)
		return FALSE;
	return TRUE;
}

void TMario::isSpeedZero() { }

void TMario::changePos(const Vec&) { }

void TMario::setNormalAttackArea()
{
	setAttackRadius(mDeParams.mHoldRadius.get());
	setAttackHeight(mDeParams.mAttackHeight.get());
}

void TMario::setPlayerVelocity(f32 velocity)
{
	mForwardVel = velocity;
	mSlideVelX  = mForwardVel * JMASSin(mFaceAngle.y);
	mSlideVelZ  = mForwardVel * JMASCos(mFaceAngle.y);
	mVel.x      = mSlideVelX;
	mVel.z      = mSlideVelZ;
}

TBGCheckData* TMario::checkWallPlane(Vec* pos, f32 yOff, f32 radius)
{
	TBGCheckData* result = nullptr;
	f32 bestDist         = radius;

	TBGWallCheckRecord record(pos->x, pos->y + yOff, pos->z, radius, 4, 0);

	if (gpMap->isTouchedWallsAndMoveXZ(&record) == true) {
		for (int i = 0; i < record.mResultWallsNum; i++) {
			TBGCheckData* wall = record.mResultWalls[i];
			if (wall->mActor == unk2C0) {
				result = wall;
				break;
			}

			f32 dist = wall->getNormal().dot(*pos) + wall->getPlaneDistance();
			if (dist < 0.0f)
				dist = -dist;
			if (dist < bestDist) {
				result   = wall;
				bestDist = dist;
			}
		}
	}

	pos->x = record.mCenter.x;
	pos->z = record.mCenter.z;
	return result;
}

void TMario::checkRoofPlane(const Vec& point, f32 yOff,
                            const TBGCheckData** outRoof)
{
	gpMap->checkRoof(point.x, 80.0f + yOff, point.z, outRoof);
}

BOOL TMario::isFrontSlip(int param_1)
{
	int angle = mFaceAngle.y;
	if (param_1 != 0 && mForwardVel < 0.0f)
		angle += 0x8000;
	s16 diff = mSlopeAngle - angle;
	return diff > -0x4000 && diff < 0x4000;
}

void TMario::isSlipStart() { }

void TMario::canSlipJump() { }

void TMario::getSlideStopNormal() { }

void TMario::getSlideStopCatch() { }

void TMario::isSlipLimit() { }

void TMario::isJumpMiss() { }

f32 TMario::checkPlayerAround(int angleOffset, f32 distance)
{
	const TBGCheckData* outPlane;

	f32 ox = distance * JMASSin(mFaceAngle.y + angleOffset);
	f32 oy = distance * JMASCos(mFaceAngle.y + angleOffset);

	return gpMap->checkGround(mPosition.x + ox, 100.0f + mPosition.y,
	                          mPosition.z + oy, &outPlane);
}

void TMario::isTurnStart() { }

void TMario::isTurnning() { }

void TMario::setMissJumping() { }

void TMario::setPlayerJumpSpeed(f32, f32) { }

u32 TMario::setStatusToJumping(u32 status, u32 arg)
{
	u32 nextStatus = status;

	unk2BC = mPosition.y;
	if (mSlopeAngle > mDeParams.mRocketRotSp.get() / 2)
		gpPollution->stamp(1, mPosition.x, mPosition.y, mPosition.z,
		                   mDirtyParams.mPolSizeJump.get());

	switch (status) {
	case 0x2000880:
	case 0x89C:
		mVel.y = (mForwardVel * 0.25f) + 42.0f;
		mForwardVel *= 0.8f;

		if (!mGroundPlane->isIllegalData()) {
			if (mGroundPlane->isThing2()) {
				mVel.y     = 0.01f * mGroundPlane->getActiveJumpPower();
				nextStatus = 0x884;
				break;
			} else if (mGroundPlane->isThing3()) {
				mVel.y += -unkBC
				          + ((mGroundPlane
				                  ? 0.01f * mGroundPlane->getActiveJumpPower()
				                  : 0.0f));

				if (mGroundPlane->mActor != nullptr)
					((THitActor*)mGroundPlane->mActor)->receiveMessage(this, 0);

				startVoice(0x78B9);
				nextStatus = 0x884;
				break;
			}
		}

		if (unk370 > mDeParams.mFeelDeep.get() ? true : false)
			startVoice(0x78A3);
		else
			startVoice(0x78AB);
		break;

	case 0x2000881:
		mVel.y = mForwardVel * mJumpParams.mSecJumpSpeedMult.get()
		         + mJumpParams.mSecJumpForce.get();
		mForwardVel *= mJumpParams.mSecJumpXZMult.get();
		startVoice(0x78B1);
		break;

	case 0x882:
		mVel.y = (mForwardVel * mJumpParams.mUltraJumpSpeedMult.get())
		         + mJumpParams.mUltraJumpForce.get();
		mForwardVel *= mJumpParams.mUltraJumpXZMult.get();
		startVoice(0x78B6);
		break;

		// wrong order of cases

	case 0x884:
		mVel.y = mGroundPlane != nullptr
		             ? 0.01f * mGroundPlane->getActiveJumpPower()
		             : 0.0f;
		startVoice(0x78B1);
		break;
	case 0x895:
	case 0x896:
		mVel.y = (mForwardVel * 0.25f) + mJumpParams.mRotateJumpForceY.get();
		mForwardVel *= 0.8f;
		startVoice(0x78B6);
		break;
	case 0x208B4:
		mVel.y      = 31.5f;
		mForwardVel = 8.0f;
		break;
	case 0x281089A: {
		startVoice(0x78AB);

		const TBGCheckData* landingPlane;
		gpMap->checkGround(mPosition.x, mPosition.y, mPosition.z,
		                   &landingPlane);
		if (landingPlane->isWaterSurface()) {
			mVel.y = (mForwardVel * mSurfingParamsWaterRed.mJumpXZRatio.get())
			         + mSurfingParamsWaterRed.mJumpPow.get();
		} else {
			mVel.y = (mForwardVel * mSurfingParamsGroundRed.mJumpXZRatio.get())
			         + mSurfingParamsGroundRed.mJumpPow.get();
		}
		break;
	}
	case 0x883:
		mForwardVel = mJumpParams.mBackJumpForce.get();
		mVel.y      = (mForwardVel * 0.0f) + mJumpParams.mBackJumpForceY.get();
		startVoice(0x78B6);
		break;
	case 0x2000886:
		mVel.y      = (mForwardVel * 0.0f) + 62.0f;
		mForwardVel = 24.0f;
		startVoice(0x78B1);
		break;
	case 0x887:
		mVel.y       = (mForwardVel * 0.0f) + mJumpParams.mTurnJumpForce.get();
		mForwardVel  = 8.0f;
		mFaceAngle.y = mIntendedYaw;
		startVoice(0x78B6);
		break;
	case 0x2000885:
		startVoice(0x78AB);
		mVel.y = (mForwardVel * 0.25f) + 42.0f;
		break;
	case 0x208B8:
		break;
	case 0x208B7:
		if (mActionArg != 2) {
			mVel.y = mJumpParams.mFireDownForce.get();
			if (mActionArg == 0)
				mForwardVel = -mJumpParams.mFireBackVelocity.get();
		}
		break;
	case 0x80088A: {
		startVoice(0x7884);

		f32 jumpSpeed = 15.0f + mForwardVel;
		if (jumpSpeed > 48.0f)
			jumpSpeed = 48.0f;

		mForwardVel = jumpSpeed;
		mSlideVelX  = mForwardVel * JMASSin(mFaceAngle.y);
		mSlideVelZ  = mForwardVel * JMASCos(mFaceAngle.y);
		mVel.x      = mSlideVelX;
		mVel.z      = mSlideVelZ;
		break;
	}
	case 0x888:
		startVoice(0x78B1);
		mForwardVel = mJumpParams.mBroadJumpForce.get();
		mVel.y      = mJumpParams.mBroadJumpForceY.get();
		break;
	case 0x2000889:
		startVoice(0x78B1);
		mForwardVel = mJumpParams.mRotBroadJumpForce.get();
		mVel.y      = mJumpParams.mRotBroadJumpForceY.get();
		break;
	case 0x88B: {
		if (mWaterGun != nullptr) {
			if ((int)mWaterGun->mCurrentNozzle == 1) {
				startVoice(0x78B9);
				rocketEffectStart();
			}
			if ((int)mWaterGun->mCurrentNozzle == 5)
				startVoice(0x788F);
			if ((int)mWaterGun->mCurrentNozzle == 4)
				startVoice(0x78AB);
			mVel.y = 10.0f;
		}
		break;
	}
	case 0x2000890:
		switch (mAnimationId) {
		case 0xD2:
			startVoice(0x78B1);
			mVel.y = (mForwardVel * 0.25f) + mDeParams.mTramplePowStep1.get();
			break;
		case 0xD3:
			startVoice(0x78B6);
			mVel.y = (mForwardVel * 0.25f) + mDeParams.mTramplePowStep2.get();
			break;
		default:
			startVoice(0x78AB);
			mVel.y = (mForwardVel * 0.25f) + mDeParams.mTramplePowStep3.get();
			break;
		}
		mForwardVel *= 0.8f;
		break;
	case 0x892:
		mVel.y      = (mForwardVel * 0.25f) + 42.0f;
		mForwardVel = 0.0f;
		mSlideVelX  = mForwardVel * JMASSin(mFaceAngle.y);
		mSlideVelZ  = mForwardVel * JMASCos(mFaceAngle.y);
		mVel.x      = mSlideVelX;
		mVel.z      = mSlideVelZ;
		startVoice(0x78B6);
		break;
	case 0x893: {
		if (arg == 0) {
			f32 jumpPower = (f32)unkF6 * mWireParams.mJumpRate.get() * 1.0f;

			mVel.y      = jumpPower * JMASSin(0xE000);
			mForwardVel = jumpPower * -JMASCos(0xE000);

			mSlideVelX = mForwardVel * JMASSin(mFaceAngle.y);
			mSlideVelZ = mForwardVel * JMASCos(mFaceAngle.y);
			mVel.x     = mSlideVelX;
			mVel.z     = mSlideVelZ;
		} else {
			f32 jumpPower = (f32)unkF6 * mWireParams.mJumpRate.get() * 1.0f;

			mVel.y      = jumpPower * JMASSin(0x6000);
			mForwardVel = jumpPower * -JMASCos(0x6000);

			mSlideVelX = mForwardVel * JMASSin(mFaceAngle.y);
			mSlideVelZ = mForwardVel * JMASCos(mFaceAngle.y);
			mVel.x     = mSlideVelX;
			mVel.z     = mSlideVelZ;
		}
		startVoice(0x78B9);
		break;
	}
	case 0x894:
		mVel.y      = (mForwardVel * 0.0f) + 42.0f;
		mForwardVel = 0.0f;
		mSlideVelX  = mForwardVel * JMASSin(mFaceAngle.y);
		mSlideVelZ  = mForwardVel * JMASCos(mFaceAngle.y);
		mVel.x      = mSlideVelX;
		mVel.z      = mSlideVelZ;
		startVoice(0x78AB);
		break;
	}

	if (unk368 > 0.0f ? TRUE : FALSE) {

		f32 scale = ((mGraffitoParams.mSinkJumpRateMax.get()
		              - mGraffitoParams.mSinkJumpRateMin.get())
		             * (1.0f - (unk368 / mGraffitoParams.mSinkTime.get())))
		            + mGraffitoParams.mSinkJumpRateMin.get();

		mVel.y *= scale;
		mForwardVel *= scale;
		unk368 -= (1.0f - (unk368 / mGraffitoParams.mSinkTime.get()))
		          * (mGraffitoParams.mSinkTime.get()
		             * mGraffitoParams.mSinkRecover.get());

		if (unk368 < 0.0f)
			unk368 = 0.0f;
	}

	if (onYoshi()) {
		mVel.y *= mYoshiParams.mJumpYoshiMult.get();
		mYoshi->mFlutterState = 0;
		mYoshi->mFlutterTimer = mYoshi->mMaxFlutterTimer;
	}

	*(f32*)((u8*)this + 0x104) = mPosition.y;
	if (nextStatus & 0x02000000)
		unk78 |= 0x100;
	else
		unk78 &= 0xFFFFFEFF;

	return nextStatus;
}

u32 TMario::setStatusToRunning(u32 status, u32)
{
	f32 mag = mIntendedMag <= 8.0f ? 8.0f : mIntendedMag;

	switch (status) {
	case 0x04000440:
		if (0.0f <= mForwardVel && mForwardVel < mag)
			mForwardVel = mag;
		break;

	case 0x50:
		s16 diff     = mSlopeAngle - mFaceAngle.y;
		bool inFront = diff > -0x4000 && diff < 0x4000;

		if (inFront)
			status = 0x00840452;
		else
			status = 0x00840453;
		startVoice(0x78CF);
		break;
	}

	return status;
}

void TMario::throwMario(const JGeometry::TVec3<f32>&, f32) { }

int TMario::changePlayerStatus(u32 status, u32 arg, bool force)
{
	if (!force) {
		if (status == mAction)
			return 0;
		if (mAction & 0x1000 ? true : false)
			return 0;
	}

	if (mAction == 0x20467)
		return 0;

	if (SMS_isDivingMap()) {
		if (status != 0x10020370 && status != 0x891 && status != 0x1302)
			return 0;
	}

	switch (status & 0x1C0) {
	case 0x40:
		status = setStatusToRunning(status, arg);
		break;
	case 0x80:
		status = setStatusToJumping(status, arg);
		break;
	}

	mPrevAction  = mAction;
	mAction      = status;
	mActionArg   = arg;
	mActionState = 0;
	mActionTimer = 0;
	return 1;
}

void TMario::changePlayerTriJump() { }

int TMario::changePlayerJumping(u32, u32) { }

int TMario::changePlayerDropping(u32 status, u32 arg)
{
	dropObject();
	return changePlayerStatus(status, arg, 0);
}

int TMario::checkAllMotions()
{
	if (mInput & 2) {
		int trig;
		int spC;
		if (checkStickRotate(&spC) != 0) {
			switch (spC) {
			case 2:
				changePlayerStatus(0x896, 0, 0);
				break;
			case 3:
				changePlayerStatus(0x895, 0, 0);
				break;
			}
			trig = 1;
		} else {
			trig = 0;
		}
		if (trig)
			return 1;
		return changePlayerStatus(0x2000880, 0, 0);
	}

	if (mInput & 4)
		return changePlayerStatus(0x88C, 0, 0);

	if (mInput & 1)
		return changePlayerStatus(0x04000440, 0, 0);

	if (mInput & 8)
		return changePlayerStatus(0x50, 0, 0);

	return 0;
}

void TMario::makeGraffitoDamage(const TMario::TEParams&) { }

void TMario::checkGraffitoDamage() { }

void TMario::checkGraffitoFire() { }

void TMario::checkGraffitoLava() { }

void TMario::checkGraffitoSlip() { }

void TMario::checkGraffitoElec() { }

void TMario::checkGraffito() { }

void TMario::dirtyLimitCheck()
{
	if (unk134 < 0.0f)
		unk134 = 0.0f;

	if (mDirtyParams.mDirtyMax.get() < unk134)
		unk134 = mDirtyParams.mDirtyMax.get();
}

void TMario::thinkDirty() { }

void TMario::thinkHeight()
{
	if (mAction & 0x800 ? true : false) {
		f32 height = mPosition.y - mFloorPosition.y;
		if (unk36C < height)
			unk36C = height;
	} else {
		unk36C = 0.0f;
	}

	JGeometry::TVec3<f32> point;
	point.x = mPosition.x + unk15C * JMASSin(mFaceAngle.y);
	point.y = mPosition.y;
	point.z = mPosition.z + unk15C * JMASCos(mFaceAngle.y);

	if (!checkWallPlane(&point, 80.0f, unk15C))
		unk370 = mPosition.y - checkPlayerAround(0, 100.0f);
	else
		unk370 = 0.0f;
}

void TMario::checkSink() { }

void TMario::checkEnforceJump() { }

void TMario::doReturn() { }

void TMario::checkReturn()
{
	if (isInvincible())
		return;

	// 2 inlines?
	if (!((mGroundPlane->mFlags & 0x10 ? true : false) == true ? false : true))
		return;

	unk2A8 = mPosition;
	unk2B4 = mFaceAngle;
}

void TMario::checkThrowObject()
{
	if (mModel->unkC[0].checkPass(4.0f)) {
		startVoice(0x788F);
		dropObject();
	}
}

void TMario::getDizzyAngle() { }

void TMario::getDizzyPower() { }

void TMario::getLRLevel(u8) { }

void TMario::checkController(JDrama::TGraphics*) { }

int TMario::checkStickRotate(int*) { }

void TMario::checkStickSmash() { }

void TMario::makeHistory()
{
	if (mIntendedMag > 0.0f) {
		if (unk534 == 0)
			unk536 = mFaceAngle.y;

		unk530[unk534] = mIntendedYaw;
		unk534 += 1;

		if (unk534 >= mControllerParams.mStickRotateTime.get()) {
			for (int i = 0; i < mControllerParams.mStickRotateTime.get(); i++)
				unk530[i] = unk530[i + 1];
			unk534 = mControllerParams.mStickRotateTime.get() - 1;
		}

		s16 diff = mIntendedYaw - mFaceAngle.y;
		if (diff >= -0x2000 && diff <= 0x2000) {
			unk538 += 1;
			if (unk538 > 0x78) {
				unk53B = 6;
				unk538 = 0x78;
			}
		} else {
			unk538 = 0;
		}
	} else {
		unk534 = 0;
		unk538 = 0;
	}

	if (unk53B != 0) {
		unk53A = 1;
		unk53B -= 1;
	} else {
		unk53A = 0;
		unk53B = 0;
	}
}

void TMario::checkGroundPlane(f32, f32, f32, f32*, const TBGCheckData**) { }

void TMario::checkCurrentPlane() { }

void TMario::getActorMtx(const THitActor&, Mtx) { }

void TMario::checkRideMovement() { }

void TMario::checkRideReCalc() { }

void TMario::checkPlayerAction(JDrama::TGraphics* graphics)
{
	mInput = 0;
	checkController(graphics);
	makeHistory();
	checkCurrentPlane();
	checkRideMovement();
	if (!(mInput & 3))
		mInput |= 0x20;
}

void TMario::stateMachine()
{
	int result = 1;
	while (result != 0) {
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

void TMario::canReadBillboard(int) { }

void TMario::talkMove() { }

void TMario::view1stMove() { }

void TMario::calcGroundMtx(const JGeometry::TVec3<f32>&) { }

void TMario::thinkSituation() { }

void TMario::thinkWaterSurface() { }

void TMario::thinkSand()
{
	if ((unk118 & 0x30000 ? true : false) == false
	    && mGroundPlane->isSand() == true) {
		unk118 |= 0x40000;
		emitSandEffect();
	} else {
		unk118 &= ~0x40000;
	}
}

void TMario::thinkParams() { }

void TMario::getOffYoshi(bool) { }

void TMario::checkYoshiGetOff() { }

void TMario::thinkYoshiHeadCollision() { }

void TMario::thinkDiving() { }

void TMario::thinkTorocco() { }

void TMario::thinkSound() { }

void TMario::checkWet() { }

void TMario::gunExec() { }

void TMario::playerControl(JDrama::TGraphics*) { }

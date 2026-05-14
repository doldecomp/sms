#include <Player/MarioMain.hpp>
#include <System/MarioGamePad.hpp>
#include <JSystem/JMath.hpp>
#include <Strategic/LiveActor.hpp>
#include <System/MarDirector.hpp>
#include <System/StageUtil.hpp>
#include <System/EmitterViewObj.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/EffectUtil.hpp>
#include <M3DUtil/M3UModelMario.hpp>
#include <MSound/MSound.hpp>
#include <MoveBG/Pool.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/PollutionManager.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <Player/MarioCap.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

f32 TMario::getJumpAccelControl() const
{
	if (mStatus == 0x892)
		return mWireParams.mWireJumpAccelControl.get();

	return mJumpParams.mJumpAccelControl.get();
}

f32 TMario::getJumpSlideControl() const
{
	if (mStatus == 0x892)
		return mWireParams.mWireJumpSlideControl.get();

	if (onYoshi() && (mYoshi->mFlutterState == 1 ? true : false))
		return mYoshiParams.mHoldOutSldCtrl.get();

	return mJumpParams.mJumpSlideControl.get();
}

bool TMario::canSquat() const
{
	if (checkFlag(0x8000) && mWaterGun
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
	if (checkFlag(0x30000)
	    && unk160[1].y < mFloorPosition.z - mSwimParams.mCanBreathDepth.get())
		return true;
	else
		return false;
}

bool TMario::isInvincible() const
{
	if (unk14C > 0)
		return true;

	if (checkFlag(0x8))
		return true;

	if (mStatus == 0x89C)
		return true;

	if (gpMarDirector->isDemoMode3() || gpMarDirector->isDemoMode4()
	    || gpMarDirector->isTalkModeNow()
	    || checkStatusFlag(STATUS_FLAG_UNK1000))
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

void TMario::getRidingMtx(Mtx mtx)
{
	if (!unk2C0->getRootJointMtx())
		SMS_GetActorMtx(*unk2C0, mtx);
	else
		MTXCopy(*unk2C0->getRootJointMtx(), mtx);
}

bool TMario::isForceSlip()
{
	if (mGroundPlane->isUnk1())
		return true;

	if (unk350 == 2) {
		if (checkFlag(0x40)) {
			if (mGroundPlane->mNormal.y < mDirtyParams.mSlopeAngle.get())
				return true;
		}
	}

	if (mGroundPlane->mNormal.y < mDeParams.mForceSlipAngle.get())
		return true;

	return false;
}

BOOL TMario::moveRequest(const JGeometry::TVec3<f32>& pos)
{
	JGeometry::TVec3<f32> offset = pos - mPosition;
	mPosition                    = pos;

	unk160[0] += offset;
	unk29C += offset;
	mWireStartPos += offset;
	mWireEndPos += offset;
	unk2A8 += offset;
	unk2BC += offset.y;
	unk1C0[0][3] += offset.x;
	unk1C0[1][3] += offset.y;
	unk1C0[2][3] += offset.z;
	unk1F0[0][3] += offset.x;
	unk1F0[1][3] += offset.y;
	unk1F0[2][3] += offset.z;
	unk220[0][3] += offset.x;
	unk220[1][3] += offset.y;
	unk220[2][3] += offset.z;
	unk318[0][3] += offset.x;
	unk318[1][3] += offset.y;
	unk318[2][3] += offset.z;

	checkRideReCalc();

	return TRUE;
}

void TMario::warpRequest(const JGeometry::TVec3<f32>& pos, f32 angle)
{
	JGeometry::TVec3<f32> offset = pos - mPosition;
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
	if (checkStatusFlag(STATUS_FLAG_SWIMMING) == true) {
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

	if (checkStickRotate(&spC)) {
		switch (spC) {
		case 2:
			changePlayerStatus(STATUS_RIGHT_ROTATE_JUMP, 0, 0);
			break;
		case 3:
			changePlayerStatus(STATUS_LEFT_ROTATE_JUMP, 0, 0);
			break;
		}
		return 1;
	}

	return 0;
}

BOOL TMario::canBendBody()
{
	u32 act = mStatus & STATUS_TYPE_AND_ID_MASK;
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

BOOL TMario::isSlipStart()
{
	if (isForceSlip())
		return true;

	if (mGroundPlane->isUnkC())
		return true;

	if (mGroundPlane->isUnk2() && mGroundPlane->getNormal().y < 0.8660254f)
		return true;

	if (mGroundPlane->isUnk3())
		return false;

	if (mGroundPlane->getNormal().y < mDeParams.mSlipStart.get())
		return true;

	return false;
}

BOOL TMario::canSlipJump()
{
	if (isForceSlip())
		return mSlipParamsAll.mJumpEnable.get();

	if (mGroundPlane->isUnkC())
		return mSlipParamsAllSlider.mJumpEnable.get();

	if (mGroundPlane->isUnk2())
		return mSlipParams45.mJumpEnable.get();

	if (mGroundPlane->isWetGround()) {
		if (mGroundPlane->getNormal().y > 0.99f)
			return mSlipParamsWaterGround.mJumpEnable.get();
		else
			return mSlipParamsWaterSlope.mJumpEnable.get();
	}

	if (mGroundPlane->isUnk3())
		return true;

	return true;
}

f32 TMario::getSlideStopNormal()
{
	if (isForceSlip())
		return mSlipParamsAll.mSlideStopNormal.get();

	if (mGroundPlane->isUnkC())
		return mSlipParamsAllSlider.mSlideStopNormal.get();

	if (mGroundPlane->isUnk2() && mGroundPlane->getNormal().y < 0.8660254f)
		return mSlipParams45.mSlideStopNormal.get();

	if (mGroundPlane->isWetGround() && mGroundPlane->getNormal().y > 0.99f)
		return mSlipParamsWaterGround.mSlideStopNormal.get();
	else
		return mSlipParamsWaterSlope.mSlideStopNormal.get();
}

f32 TMario::getSlideStopCatch()
{
	if (isForceSlip())
		return mSlipParamsAll.mSlideStopCatch.get();

	if (mGroundPlane->isUnkC())
		return mSlipParamsAllSlider.mSlideStopCatch.get();

	if (mGroundPlane->isUnk2() && mGroundPlane->getNormal().y < 0.8660254f)
		return mSlipParams45.mSlideStopCatch.get();

	if (mGroundPlane->isWetGround()) {
		if (mGroundPlane->getNormal().y > 0.99f)
			return mSlipParamsWaterGround.mSlideStopCatch.get();
		else
			return mSlipParamsWaterSlope.mSlideStopCatch.get();
	}

	return mSlipParamsNormal.mSlideStopCatch.get();
}

void TMario::isSlipLimit() { }

BOOL TMario::isJumpMiss()
{
	if (isForceSlip())
		return mSlipParamsAll.mMissJump.get();

	if (mGroundPlane->isUnkC())
		return mSlipParamsAllSlider.mMissJump.get();

	if (mGroundPlane->isUnk2() && mGroundPlane->getNormal().y < 0.8660254f)
		return mSlipParams45.mMissJump.get();

	if (mGroundPlane->isWetGround()) {
		if (mGroundPlane->getNormal().y > 0.99f)
			return mSlipParamsWaterGround.mMissJump.get();
		else
			return mSlipParamsWaterSlope.mMissJump.get();
	}

	return mSlipParamsNormal.mMissJump.get();
}

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

void TMario::setPlayerJumpSpeed(f32 speed_mult, f32 force)
{
	mVel.y = (mForwardVel * speed_mult) + force;
}

u32 TMario::setStatusToJumping(u32 status, u32 arg)
{
	u32 nextStatus = status;

	unk2BC = mPosition.y;
	if (mSlopeAngle > mDeParams.mRocketRotSp.get() / 2)
		gpPollution->stamp(1, mPosition.x, mPosition.y, mPosition.z,
		                   mDirtyParams.mPolSizeJump.get());

	switch (status) {
	case STATUS_JUMP:
	case 0x89C:
		setPlayerJumpSpeed(0.25f, 42.0f);
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

	case STATUS_SECOND_JUMP:
		setPlayerJumpSpeed(mJumpParams.mSecJumpSpeedMult.get(),
		                   mJumpParams.mSecJumpForce.get());
		mForwardVel *= mJumpParams.mSecJumpXZMult.get();
		startVoice(0x78B1);
		break;

	case STATUS_ULTRA_JUMP:
		setPlayerJumpSpeed(mJumpParams.mUltraJumpSpeedMult.get(),
		                   mJumpParams.mUltraJumpForce.get());
		mForwardVel *= mJumpParams.mUltraJumpXZMult.get();
		startVoice(0x78B6);
		break;

	case 0x884:
		mVel.y = mGroundPlane != nullptr
		             ? 0.01f * mGroundPlane->getActiveJumpPower()
		             : 0.0f;
		startVoice(0x78B1);
		break;

	case STATUS_LEFT_ROTATE_JUMP:
	case STATUS_RIGHT_ROTATE_JUMP:
		setPlayerJumpSpeed(0.25f, mJumpParams.mRotateJumpForceY.get());
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
			setPlayerJumpSpeed(mSurfingParamsWaterRed.mJumpXZRatio.get(),
			                   mSurfingParamsWaterRed.mJumpPow.get());
		} else {
			setPlayerJumpSpeed(mSurfingParamsGroundRed.mJumpXZRatio.get(),
			                   mSurfingParamsGroundRed.mJumpPow.get());
		}
		break;
	}

	case STATUS_BACK_JUMP:
		mForwardVel = mJumpParams.mBackJumpForce.get();
		setPlayerJumpSpeed(0.0f, mJumpParams.mBackJumpForceY.get());
		startVoice(0x78B6);
		break;

	case STATUS_WALL_JUMP:
		setPlayerJumpSpeed(0.0f, 62.0f);
		mForwardVel = 24.0f;
		startVoice(0x78B1);
		break;

	case STATUS_U_TURN_JUMP:
		setPlayerJumpSpeed(0.0f, mJumpParams.mTurnJumpForce.get());
		mForwardVel  = 8.0f;
		mFaceAngle.y = mIntendedYaw;
		startVoice(0x78B6);
		break;

	case 0x2000885:
		startVoice(0x78AB);
		setPlayerJumpSpeed(0.25f, 42.0f);
		break;

	case 0x208B8:
		break;

	case 0x208B7:
		if (mStatusArg != 2) {
			mVel.y = mJumpParams.mFireDownForce.get();
			if (mStatusArg == 0)
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

	case STATUS_BROAD_JUMP:
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
			if ((int)mWaterGun->mCurrentNozzle == TWaterGun::Rocket) {
				startVoice(0x78B9);
				rocketEffectStart();
			}
			if ((int)mWaterGun->mCurrentNozzle == TWaterGun::Turbo)
				startVoice(0x788F);
			if ((int)mWaterGun->mCurrentNozzle == TWaterGun::Hover)
				startVoice(0x78AB);
			mVel.y = 10.0f;
		}
		break;
	}

	case 0x2000890:
		switch (mAnimationId) {
		case 0xD2:
			startVoice(0x78B1);
			setPlayerJumpSpeed(0.25f, mDeParams.mTramplePowStep1.get());
			break;
		case 0xD3:
			startVoice(0x78B6);
			setPlayerJumpSpeed(0.25f, mDeParams.mTramplePowStep2.get());
			break;
		default:
			startVoice(0x78AB);
			setPlayerJumpSpeed(0.25f, mDeParams.mTramplePowStep3.get());
			break;
		}
		mForwardVel *= 0.8f;
		break;

	case 0x892:
		setPlayerJumpSpeed(0.25f, 42.0f);
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
		setPlayerJumpSpeed(0.0f, 42.0f);
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

	unk104 = mPosition.y;
	if (nextStatus & STATUS_FLAG_UNK2000000)
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

void TMario::throwMario(const JGeometry::TVec3<f32>& param_1, f32 param_2)
{
	JGeometry::TVec3<f32> tmp = param_1;
	if (tmp.squared() <= JGeometry::TUtil<f32>::epsilon())
		tmp.y = 1.0f;
	tmp.normalize();

	// TODO: fakematch, probably an inline?
	f32 z           = tmp.z;
	mFaceAngle.y    = matan(z, tmp.x) + 0x8000;
	mModelFaceAngle = mFaceAngle.y;
	mForwardVel     = param_2 * -std::sqrtf(tmp.x * tmp.x + z * z);
	mVel.y          = tmp.y * param_2;
}

int TMario::changePlayerStatus(u32 status, u32 arg, bool force)
{
	if (!force) {
		if (status == mStatus)
			return 0;
		if (checkStatusFlag(STATUS_FLAG_UNK1000))
			return 0;
	}

	if (mStatus == 0x20467)
		return 0;

	if (SMS_isDivingMap()) {
		if (status != 0x10020370 && status != 0x891 && status != 0x1302)
			return 0;
	}

	switch (status & STATUS_TYPE_MASK) {
	case STATUS_TYPE_RUNNING:
		status = setStatusToRunning(status, arg);
		break;
	case STATUS_TYPE_JUMPING:
		status = setStatusToJumping(status, arg);
		break;
	}

	mPrevStatus  = mStatus;
	mStatus      = status;
	mStatusArg   = arg;
	mStatusState = 0;
	mStatusTimer = 0;
	return 1;
}

BOOL TMario::changePlayerTriJump()
{
	if (isJumpMiss()) {
		mFaceAngle.x    = 0;
		mModelFaceAngle = mFaceAngle.y;
		if (mForwardVel > 0.0f) {
			// TODO: inlineF
			s16 a     = mSlopeAngle + 0x8000;
			s16 angle = mFaceAngle.y - a;
			f32 x     = mForwardVel * JMASSin(angle);
			f32 z     = mForwardVel * JMASCos(angle) * 0.75f;
			f32 mag   = MsSqrtf(x * x + z * z);
			setPlayerVelocity(mag);
			mFaceAngle.y = a + matan(z, x);
		}
		dropObject();
		changePlayerStatus(0x2000885, 0, false);
	} else {
		if (considerRotateJumpStart())
			return 1;

		changePlayerStatus(STATUS_JUMP, 0, false);
	}

	return 1;
}

int TMario::changePlayerJumping(u32 param_1, u32 param_2)
{
	if (isJumpMiss()) {
		mFaceAngle.x    = 0;
		mModelFaceAngle = mFaceAngle.y;
		if (mForwardVel > 0.0f) {
			// TODO: inline
			s16 a     = mSlopeAngle + 0x8000;
			s16 angle = mFaceAngle.y - a;
			f32 x     = mForwardVel * JMASSin(angle);
			f32 z     = mForwardVel * JMASCos(angle) * 0.75f;
			f32 mag   = MsSqrtf(x * x + z * z);
			setPlayerVelocity(mag);
			mFaceAngle.y = a + matan(z, x);
		}
		dropObject();
		changePlayerStatus(0x2000885, 0, false);
	} else {
		if (considerRotateJumpStart())
			return 1;

		changePlayerStatus(param_1, param_2, false);
	}

	return 1;
}

int TMario::changePlayerDropping(u32 status, u32 arg)
{
	dropObject();
	return changePlayerStatus(status, arg, 0);
}

int TMario::checkAllMotions()
{
	if (mInput & 2) {
		if (considerRotateJumpStart())
			return 1;
		return changePlayerStatus(STATUS_JUMP, 0, 0);
	}

	if (mInput & 4)
		return changePlayerStatus(0x88C, 0, 0);

	if (mInput & 1)
		return changePlayerStatus(0x04000440, 0, 0);

	if (mInput & 8)
		return changePlayerStatus(0x50, 0, 0);

	return 0;
}

void TMario::makeGraffitoDamage(const TMario::TEParams& params)
{
	mFloorHitActor.mPosition.x = mPosition.x + JMASSin(mFaceAngle.y);
	mFloorHitActor.mPosition.z = mPosition.z + JMASCos(mFaceAngle.y);

	damageExec(&mFloorHitActor, params.mDamage.get(), params.mDownType.get(),
	           params.mWaterEmit.get(), params.mMinSpeed.get(),
	           params.mMotor.get(), params.mDirty.get(),
	           params.mInvincibleTime.get());
}

void TMario::checkGraffitoDamage() { }

void TMario::checkGraffitoFire()
{
	if (isInvincible())
		return;

	if (checkFlag(0x400))
		return;

	if (mPosition.y - mFloorPosition.y > mGraffitoParams.mFireHeight.get())
		return;

	if (mStatus == 0x208b7 || mStatus == 0x8000239)
		mFaceAngle.y += 0x8000;

	f32 fVar2 = mForwardVel;
	f32 fVar3 = mVel.y;

	makeGraffitoDamage(mDmgParamsGraffitoFire);

	if (unk55C > 0.0f) {
		mVel.y      = -fVar3;
		mForwardVel = fVar2;
	}

	unk14C = mGraffitoParams.mFireInvincibleTime.get();
	dropObject();
	changePlayerStatus(0x208B7, 1, false);
	gpMarioParticleManager->emitAndBindToPosPtr(6, &mPosition, 0, nullptr);
	SMSGetMSound()->startSoundActor(0x1813, &mPosition, 0, nullptr, 0, 4);
}

void TMario::checkGraffitoLava() { }

void TMario::checkGraffitoSlip()
{
	if (isTouchGround4cm()) {
		unk360 = mDeParams.mFootPrintTimerMax.get();

		if (mStatus == 0x84045d || mStatus == 0x4045e) {
			unk138 = mDirtyParams.mBrakeStartValSlip.get();
			unk13C = mDirtyParams.mDirtyTimeSlip.get();
		}

		if (mStatus == 0x4045c || mStatus == 0x40561) {
			unk138 = mDirtyParams.mBrakeStartValRun.get();
			unk13C = mDirtyParams.mDirtyTimeRun.get();
		}

		if (mGroundPlane->getNormal().y <= mDirtyParams.mSlopeAngle.get()) {
			unk138 = mDirtyParams.mBrakeStartValSlip.get();
			unk13C = mDirtyParams.mDirtyTimeSlip.get();

			changePlayerStatus(0x4045e, 0, false);
			startVoice(0x78D3);
		} else if (mStatus == 0x80088a || mStatus == 0x800456
		           || mStatus == 0x84045d || mStatus == 0x4045e) {
			unk138 = mDirtyParams.mBrakeStartValSlip.get();
			unk13C = mDirtyParams.mDirtyTimeSlip.get();

			changePlayerStatus(0x84045d, 0, false);
			if (this->mPrevStatus != 0x84045d) {
				startVoice(0x78d3);
			}
		} else if (mStatus != 0x386) {
			unk138 = mDirtyParams.mBrakeStartValRun.get();
			unk13C = mDirtyParams.mDirtyTimeRun.get();

			if (this->mStatus == 0x560) {
				changePlayerStatus(0x40561, 0, false);
			} else {
				changePlayerStatus(0x4045c, 0, false);
			}
		}

		if (!checkFlag(0x40))
			unk34E = mDirtyParams.mFogTimeYellow.get()
			         + mDirtyParams.mFogTimeRed.get();

		unk34E -= 1;
		if (unk34E != 0) {
			if (unk34E == mDirtyParams.mFogTimeRed.get())
				floorDamageExec(1, 3, 0, mMotorParams.mMotorReturn.get());
		} else {
			unk34E = mDirtyParams.mFogTimeYellow.get()
			         + mDirtyParams.mFogTimeRed.get();
		}
	} else if (mStatus == 0x84045d || mStatus == 0x4045e) {
		unk138 = mDirtyParams.mBrakeSlipNoPollute.get();
		unk13C = mDirtyParams.mDirtyTimeSlip.get();
	}
}

void TMario::checkGraffitoElec()
{
	(void)0;
	(void)0;
	(void)0;
	(void)0;

	if (!checkFlag(0x40))
		unk34E = mDeParams.mGraffitoNoDmgTime.get();

	if (unk34E != 0) {
		unk34E -= 1;
		return;
	}

	if (isInvincible())
		return;

	if (((mStatus & STATUS_TYPE_MASK) == STATUS_TYPE_WAITING
	     || (mStatus & STATUS_TYPE_MASK) == STATUS_TYPE_RUNNING)
	    && unk360 <= 0) {
		changePlayerStatus(0x20338, 0, false);
		SMSGetMSound()->startSoundActor(0x1814, &mPosition, 0, nullptr, 0, 4);
		SMSGetMSound()->startSoundActor(0x3806, &mPosition, 0, nullptr, 0, 4);
	}
}

void TMario::checkGraffito()
{
	if (mGroundPlane->isIllegalData())
		return;

	if (onYoshi())
		return;

	if (unk388 == 1 || unk388 == 2)
		return;

	int isFullyPolluted = 0;
	unk350
	    = gpPollution->getPollutionType(mPosition.x, mPosition.y, mPosition.z);

	switch (unk350) {
	case 2:
	case 5:
	case 6: {
		JGeometry::TVec3<f32> pos = mPosition;
		pos.x -= 32.0f;
		pos.z -= 32.0f;
		isFullyPolluted = 1;

		if (!gpPollution->isPolluted(pos.x, pos.y, pos.z))
			isFullyPolluted = 0;
		pos.x += 32.0f;
		if (!gpPollution->isPolluted(pos.x, pos.y, pos.z))
			isFullyPolluted = 0;
		pos.x += 32.0f;
		if (!gpPollution->isPolluted(pos.x, pos.y, pos.z))
			isFullyPolluted = 0;
		pos.z += 32.0f;
		if (!gpPollution->isPolluted(pos.x, pos.y, pos.z))
			isFullyPolluted = 0;
		pos.x -= 32.0f;
		if (!gpPollution->isPolluted(pos.x, pos.y, pos.z))
			isFullyPolluted = 0;
		pos.x -= 32.0f;
		if (!gpPollution->isPolluted(pos.x, pos.y, pos.z))
			isFullyPolluted = 0;
		pos.z += 32.0f;
		if (!gpPollution->isPolluted(pos.x, pos.y, pos.z))
			isFullyPolluted = 0;
		pos.x += 32.0f;
		if (!gpPollution->isPolluted(pos.x, pos.y, pos.z))
			isFullyPolluted = 0;
		pos.x += 32.0f;
		if (!gpPollution->isPolluted(pos.x, pos.y, pos.z))
			isFullyPolluted = 0;
		break;
	}

	case 0:
	case 1:
	case 3:
	case 7: {
		JGeometry::TVec3<f32> pos;
		pos.x = mPosition.x;
		pos.y = mFloorPosition.y;
		pos.z = mPosition.z;

		isFullyPolluted = 1;

		pos.z -= 32.0f;
		if (!gpPollution->isPolluted(pos.x, pos.y, pos.z))
			isFullyPolluted = 0;

		pos.x += 32.0f;
		pos.z += 32.0f;
		if (!gpPollution->isPolluted(pos.x, pos.y, pos.z))
			isFullyPolluted = 0;

		pos.x -= 32.0f;
		if (!gpPollution->isPolluted(pos.x, pos.y, pos.z))
			isFullyPolluted = 0;

		pos.x -= 32.0f;
		if (!gpPollution->isPolluted(pos.x, pos.y, pos.z))
			isFullyPolluted = 0;

		pos.z += 32.0f;
		pos.x += 32.0f;
		if (!gpPollution->isPolluted(pos.x, pos.y, pos.z))
			isFullyPolluted = 0;

		(void)&pos;

		break;
	}

	case 4: {
		JGeometry::TVec3<f32> pos;
		pos.x = mPosition.x;
		pos.y = mFloorPosition.y;
		pos.z = mPosition.z;
		if (gpPollution->isPolluted(pos.x, pos.y, pos.z))
			isFullyPolluted = 1;
		else
			isFullyPolluted = 0;
		break;
	}

	case 8:
	case 10:
		isFullyPolluted = 0;
		break;
	}

	if (onYoshi() && isFullyPolluted == 1)
		getOffYoshi(true);

	switch (unk350) {
	case 4:
		if (isFullyPolluted == 1)
			checkGraffitoElec();
		break;
	case 1:
	case 7:
		if (isFullyPolluted == 1)
			checkGraffitoFire();
		break;
	case 2:
		if (isFullyPolluted == 1)
			checkGraffitoSlip();
		break;
	case 3:
		if (isFullyPolluted == 1) {
			mPosition.x = unk29C.x;
			mPosition.z = unk29C.z;
		}
		break;

	case 0:
	case 5:
	case 6:
	case 8:
		break;
	}

	if (isFullyPolluted == 1)
		unk118 |= 0x40;
	else
		unk118 &= ~0x40;

	if (isTouchGround4cm()) {
		if (isFullyPolluted == 1)
			SMS_EmitSinkInPollutionEffect(mPosition, mGroundPlane->getNormal(),
			                              false);

		if (unk360 <= 0)
			return;
		unk360 -= 1;
		if (unk360 <= mDeParams.mFootPrintTimerMax.get() / 2)
			return;

		if (!mGroundPlane->isLegal())
			return;

		if (checkFlag(0x30000))
			return;

		emitDirtyFootPrint();
	}
}

void TMario::dirtyLimitCheck()
{
	if (unk134 < 0.0f)
		unk134 = 0.0f;

	if (mDirtyParams.mDirtyMax.get() < unk134)
		unk134 = mDirtyParams.mDirtyMax.get();
}

void TMario::thinkDirty()
{
	if (checkFlag(0x40)) {
		if (mStatus == 0x4000440 || mStatus == 0x4045C)
			unk134 += mDirtyParams.mIncRunning.get();
		if (mStatus == 0x800456 || mStatus == 0x84045D || mStatus == 0x4045E)
			unk134 += mDirtyParams.mIncCatching.get();
		if (mStatus == 0x50)
			unk134 += mDirtyParams.mIncSlipping.get();
	}

	if (checkFlag(0x30000)) {
		if (mPosition.y > mFloorPosition.z - 1.0f)
			meltInWaterEffect();
		unk360 = 0;
		unk134 -= mDirtyParams.mDecSwimming.get();
	}

	if (mStatus == STATUS_LEFT_ROTATE_JUMP
	    || mStatus == STATUS_RIGHT_ROTATE_JUMP) {
		unk134 -= mDirtyParams.mDecRotJump.get();
		unk360 = 0;
	}

	if (checkFlag(0x10)) {
		unk134 -= mDirtyParams.mDecWaterHit.get();
		unk360 = 0;
	}

	if (unk134 < 0.0f)
		unk134 = 0.0f;

	if (mDirtyParams.mDirtyMax.get() < unk134)
		unk134 = mDirtyParams.mDirtyMax.get();
}

void TMario::thinkHeight()
{
	if (checkStatusFlag(STATUS_FLAG_JUMPING)) {
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

void TMario::checkSink()
{
	if (isInvincible())
		return;

	if (mGroundPlane->isIllegalData())
		return;

	if (mFloorPosition.y + 100.0f < mPosition.y) {
		unk368 = 0.0f;
		return;
	}

	if (unk350 == 0) {
		if (checkFlag(0x40)) {
			unk368 += 1.0f;
			unk360 = mDeParams.mFootPrintTimerMax.get();
			if (mHealth > 0
			    && unk368 > mGraffitoParams.mSinkTime.get()
			                    * mGraffitoParams.mSinkDmgDepth.get()) {
				unk368 = mGraffitoParams.mSinkTime.get()
				         * mGraffitoParams.mSinkDmgDepth.get();
			}

			if (gpMarDirector->unk58 % mGraffitoParams.mSinkDmgTime.get()
			    == 0) {
				floorDamageExec(1, 3, 0, mMotorParams.mMotorReturn.get());
			}

			SMSGetMSound()->startSoundActor(0x100B, &mPosition, 0, nullptr, 0,
			                                4);

			if (unk368 > mGraffitoParams.mSinkTime.get()) {
				loserExec();
				changePlayerStatus(0x10001123, 0, false);
			}
			SMS_EmitSinkInPollutionEffect(mPosition, mGroundPlane->getNormal(),
			                              true);
			startVoice(0x7865);
			return;
		}
	}

	if (unk350 == 5) {
		if (checkFlag(0x40)) {
			unk374 -= mJumpParams.mGravity.get();
			unk378 += unk374;
			mVel.set(0.0f, 0.0f, 0.0f);
			mForwardVel = 0.0f;
			mSlideVelX  = 0.0f;
			mSlideVelZ  = 0.0f;
			loserExec();
			changePlayerStatus(0x10001123, 0, false);
			return;
		}
	}

	unk374 = 0.0f;
	unk378 = 0.0f;
	unk368 = 0.0f;
}

// TODO: fake!! use a real inline!
static void startForceJumpSound2(Vec* param_1, u32 param_2, f32 param_3,
                                 u32 param_4)
{
	SMSGetMSound()->startForceJumpSound(param_1, param_2, param_3, param_4);
}

void TMario::checkEnforceJump()
{
	if (mGroundPlane->isLegal() && mGroundPlane->isBounceOnLanding()
	    && (isTouchGround4cm()) && (mPrevStatus & STATUS_FLAG_JUMPING)) {

		startForceJumpSound2(&mPosition, mSoundFlags, 0.0f,
		                     mGroundPlane->getData());
		startVoice(0x78B9);
		changePlayerStatus(0x884, 0, 0);
		rumbleStart(0x15, mMotorParams.mMotorWall.get());
		if (mGroundPlane->mActor != nullptr)
			((THitActor*)mGroundPlane->mActor)->receiveMessage(this, 0);
	}
}

void TMario::doReturn() { }

void TMario::checkReturn()
{
	if (isInvincible())
		return;

	if (!mGroundPlane->isLegal())
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

void TMario::checkController(JDrama::TGraphics*)
{
	unk108->mStickHS16 = (s16)(128.0f * mGamePad->mCompSPos[0]);
	unk108->mStickVS16 = (s16)(128.0f * mGamePad->mCompSPos[1]);

	if (unk368 > 0.0f ? true : false) {
		f32 sinkScale
		    = (mGraffitoParams.mSinkMoveMax.get()
		       - mGraffitoParams.mSinkMoveMin.get())
		          * (1.0f - (unk368 / (f32)mGraffitoParams.mSinkTime.get()))
		      + mGraffitoParams.mSinkMoveMin.get();
		unk108->mStickHS16 *= sinkScale;
		unk108->mStickVS16 *= sinkScale;
	}

	unk108->mInput      = (TMarioControllerWork::Buttons)0;
	unk108->mFrameInput = (TMarioControllerWork::Buttons)0;

	if (mGamePad->mMeaning & 0x80)
		unk108->mInput |= TMarioControllerWork::A;

	if (mGamePad->mEnabledFrameMeaning & 0x80)
		unk108->mFrameInput |= TMarioControllerWork::A;

	if (mGamePad->mMeaning & 0x100)
		unk108->mInput |= TMarioControllerWork::B;

	if (mGamePad->mEnabledFrameMeaning & 0x100)
		unk108->mFrameInput |= TMarioControllerWork::B;

	if (mGamePad->mMeaning & 0x400)
		unk108->mInput |= TMarioControllerWork::R;

	if (mGamePad->mEnabledFrameMeaning & 0x400)
		unk108->mFrameInput |= TMarioControllerWork::R;

	if (mGamePad->mEnabledFrameMeaning & 0x1000)
		unk108->mFrameInput |= TMarioControllerWork::UNK10;

	unk108->mAnalogRU8 = mGamePad->mCompSPos[3];
	unk108->mAnalogLU8 = mGamePad->mCompSPos[2];

	// 3-segment (zero, middle, max) trigger curve, run 4 times:
	// for unk108->mAnalogR (R trigger), unk108->mAnalogL (L trigger),
	// then again for unk10C (L) and unk110 (R) Mario-side copies.
	// TODO: doesn't match at all!
	{
		u8 zero    = mControllerParams.mAnalogLRToZeroVal.get();
		u8 middle  = mControllerParams.mAnalogLRToMiddleVal.get();
		u8 max     = mControllerParams.mAnalogLRToMaxVal.get();
		f32 midLvl = mControllerParams.mAnalogLRMiddleLevel.get();
		u8 v       = (u8)(s32)mGamePad->mCompSPos[3];
		f32 out;
		if (v < zero)
			out = 0.0f;
		else if (v < middle)
			out = (midLvl * (f32)(v - zero)) / (f32)(middle - zero);
		else if (v < max)
			out = midLvl
			      + (((1.0f - midLvl) * (f32)(v - middle))
			         / (f32)(max - middle));
		else
			out = 1.0f;
		unk108->mAnalogR = out;
	}
	{
		u8 zero    = mControllerParams.mAnalogLRToZeroVal.get();
		u8 middle  = mControllerParams.mAnalogLRToMiddleVal.get();
		u8 max     = mControllerParams.mAnalogLRToMaxVal.get();
		f32 midLvl = mControllerParams.mAnalogLRMiddleLevel.get();
		u8 v       = (u8)(s32)mGamePad->mCompSPos[2];
		f32 out;
		if (v < zero)
			out = 0.0f;
		else if (v < middle)
			out = (midLvl * (f32)(v - zero)) / (f32)(middle - zero);
		else if (v < max)
			out = midLvl
			      + (((1.0f - midLvl) * (f32)(v - middle))
			         / (f32)(max - middle));
		else
			out = 1.0f;
		unk108->mAnalogL = out;
	}
	{
		u8 zero    = mControllerParams.mAnalogLRToZeroVal.get();
		u8 middle  = mControllerParams.mAnalogLRToMiddleVal.get();
		u8 max     = mControllerParams.mAnalogLRToMaxVal.get();
		f32 midLvl = mControllerParams.mAnalogLRMiddleLevel.get();
		u8 v       = (u8)(s32)mGamePad->mCompSPos[2];
		f32 out;
		if (v < zero)
			out = 0.0f;
		else if (v < middle)
			out = (midLvl * (f32)(v - zero)) / (f32)(middle - zero);
		else if (v < max)
			out = midLvl
			      + (((1.0f - midLvl) * (f32)(v - middle))
			         / (f32)(max - middle));
		else
			out = 1.0f;
		unk10C = out;
	}
	{
		u8 zero    = mControllerParams.mAnalogLRToZeroVal.get();
		u8 middle  = mControllerParams.mAnalogLRToMiddleVal.get();
		u8 max     = mControllerParams.mAnalogLRToMaxVal.get();
		f32 midLvl = mControllerParams.mAnalogLRMiddleLevel.get();
		u8 v       = (u8)(s32)mGamePad->mCompSPos[3];
		f32 out;
		if (v < zero)
			out = 0.0f;
		else if (v < middle)
			out = (midLvl * (f32)(v - zero)) / (f32)(middle - zero);
		else if (v < max)
			out = midLvl
			      + (((1.0f - midLvl) * (f32)(v - middle))
			         / (f32)(max - middle));
		else
			out = 1.0f;
		unk110 = out;
	}

	// Stick deadzone: subtract 6 if outside +/- 7
	unk108->mStickH = 0.0f;
	unk108->mStickV = 0.0f;
	if (unk108->mStickHS16 < -7)
		unk108->mStickH = (f32)(unk108->mStickHS16 + 6);
	if (unk108->mStickHS16 > 7)
		unk108->mStickH = (f32)(unk108->mStickHS16 - 6);
	if (unk108->mStickVS16 < -7)
		unk108->mStickV = (f32)(unk108->mStickVS16 + 6);
	if (unk108->mStickVS16 > 7)
		unk108->mStickV = (f32)(unk108->mStickVS16 - 6);

	// Stick distance, then mLengthMult^mLengthMultTimes (unrolled in 8s)
	f32 dist = (unk108->mStickH * unk108->mStickH)
	           + (unk108->mStickV * unk108->mStickV);
	if (dist > 0.0f)
		dist = MsSqrtf(dist);

	for (int i = 0; i < mControllerParams.mLengthMultTimes.get(); ++i)
		dist *= mControllerParams.mLengthMult.get();

	unk108->mStickDist = dist;

	if (unk108->mStickDist > 64.0f) {
		unk108->mStickH    = unk108->mStickH * (64.0f / unk108->mStickDist);
		unk108->mStickV    = unk108->mStickV * (64.0f / unk108->mStickDist);
		unk108->mStickDist = 64.0f;
	}

	{
		f32 norm     = unk108->mStickDist / 64.0f;
		mIntendedMag = 32.0f * (norm * norm);
	}

	// Dizzy jitter applied to yaw and magnitude when unkA0 > 0
	if (unkA0 > 0)
		unkA0 = unkA0 - 1;

	s32 yawJitter = 0;
	if (unkA0 > 0) {
		yawJitter = JMASSin(unkA0 * mGraffitoParams.mDizzyAngleRate.get())
		            * mGraffitoParams.mDizzyAngleY.get() * unkA0
		            / mGraffitoParams.mDizzyWalkCtMax.get();
		mIntendedMag += JMASCos(unkA0 * mGraffitoParams.mDizzyPowerRate.get())
		                * mGraffitoParams.mDizzyPower.get();
		if (mIntendedMag < 0.0f)
			mIntendedMag = 0.0f;
	}

	if (mIntendedMag > 0.0f) {
		// camera angle at offset 0x258 lives inside the unk21C[..]
		// placeholder array of CPolarSubCamera -- TODO: name & expose it
		mIntendedYaw = matan(-unk108->mStickV, unk108->mStickH)
		               + (*(s16*)((char*)gpCamera + 0x258) + yawJitter);
	} else {
		mIntendedYaw = mFaceAngle.y;
	}

	// Rocket-nozzle pump rotation latch
	if (mWaterGun != nullptr) {
		if ((s32)mWaterGun->mCurrentNozzle == 5 && (unk380 == 0 ? true : false)
		    && mGamePad->mCompSPos[3] > 0.0f
		    && (f32)mWaterGun->mCurrentWater > 0.0f) {
			if (mIntendedMag == 0.0f)
				mIntendedYaw = mFaceAngle.y;

			unkC0 += mDeParams.mDashAcc.get();
			if (unkC0 > 32.0f) {
				unkC0 = 32.0f;
				unkC4 += 1;
				if ((f32)unkC4 > (f32)mDeParams.mDashStartTime.get()) {
					unkC4 = mDeParams.mDashStartTime.get();
					if (!checkFlag(0x4000)
					    && ((TNozzleTrigger*)((const TWaterGun*)mWaterGun)
					            ->getCurrentNozzle())
					               ->unk385
					           == 1) {
						unk118 |= 0x4000;
						startSoundActor(0x814);
						if (checkStatusFlag(STATUS_FLAG_SWIMMING))
							changePlayerStatus(0x24D5, 0, false);
					}
				}
				if (((mStatus + 0xFC000000) & 0xFFFFFFFF) != 0x440
				    && mStatus != 0x24D5) {
					unkC4 = 0;
					unk118 &= ~0x4000;
				}
			} else {
				unkC4 = 0;
				unk118 &= ~0x4000;
			}
			mIntendedMag = unkC0;
			mWaterGun->rotateProp(unkC0);
		} else {
			if (unkC0 > 0.1f) {
				if (mIntendedMag == 0.0f)
					mIntendedYaw = mFaceAngle.y;
				unkC0 *= mDeParams.mDashBrake.get();
				mIntendedMag = unkC0;
			} else {
				unkC0 = 0.0f;
			}
			unkC4 = 0;
			unk118 &= ~0x4000;
		}

		if ((s32)mWaterGun->mCurrentNozzle == TWaterGun::Turbo
		    && ((mStatus + 0xF3C00000) == 0x201
		        || ((mStatus + 0xFC000000) & 0xFFFFFFFF) == 0x440)) {
			f32 propRot = mIntendedMag * 0.03125f;
			// TODO: wrong??? Correct offset is 0x714 which is way past
			// the end of a TNozzleTrigger???
			((TNozzleTrigger*)((const TWaterGun*)mWaterGun)->getCurrentNozzle())
			    ->unk388
			    = propRot;
		}
	}

	// mInput bit synthesis
	if (mIntendedMag > 0.0f)
		mInput |= 0x1;

	if (mGamePad->mEnabledFrameMeaning & 0x80)
		mInput |= 0x2;

	if (mGamePad->mMeaning & 0x80)
		mInput |= 0x80;

	if (unk108->mInput & 0x200)
		mInput |= 0x4000;

	if (!onYoshi() && (unk108->mFrameInput & 0x200)) {
		mInput |= 0x8000;
		mInput |= 0x2000;
	}

	if ((mGamePad->mEnabledFrameMeaning & 0x2000)
	    || (unk108->mFrameInput & 0x40)) {
		if (checkStatusFlag(STATUS_FLAG_JUMPING) == true)
			mInput |= 0x8000;
	}

	if (checkFlag(0x8000)) {
		if (!checkFlag(0x4000)) {
			if ((mGamePad->mMeaning & 0x400) || (mGamePad->mMeaning & 0x2000))
				mInput |= 0x200;
			if (mGamePad->mEnabledFrameMeaning & 0x400)
				mInput |= 0x100;
		}
	}
}

int TMario::checkStickRotate(int* outDir)
{
	int incCount = 0;
	int decCount = 0;

	// TODO: volatile is fake, but it's uninitialized,
	// so a bug, it seems?
	volatile BOOL q[4];

	for (int i = 0; i < unk534 - 1; i++) {
		f32 angle = (f32)unk530[i];

		if (angle < -24576.0f || angle > 24576.0f)
			q[0] = 1;
		if (-24576.0f <= angle && angle <= -8192.0f)
			q[1] = 1;
		if (-8192.0f < angle && angle < 8192.0f)
			q[2] = 1;
		if (8192.0f <= angle && angle <= 24576.0f)
			q[3] = 1;

		if (angle < (f32)unk530[i + 1])
			incCount++;
		else
			decCount++;
	}

	int sum = 0;
	if (q[0] == 1)
		sum++;
	if (q[1] == 1)
		sum++;
	if (q[2] == 1)
		sum++;
	if (q[3] == 1)
		sum++;

	if (sum >= 4) {
		if (incCount > decCount)
			*outDir = 2;
		else
			*outDir = 3;
		return 1;
	}

	*outDir = 4;
	return 0;
}

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

BOOL TMario::checkGroundPlane(f32 x, f32 y, f32 z, f32* outHeight,
                              const TBGCheckData** outPlane)
{
	*outHeight = gpMap->checkGround(x, y, z, outPlane);

	if ((*outPlane)->isMarioThrough()) {
		*outHeight = gpMap->checkGround(x, *outHeight - 1.0f, z, outPlane);
	}

	return (*outPlane)->isLegal() ? TRUE : FALSE;
}

void TMario::checkCurrentPlane()
{
	if (checkStatusFlag(STATUS_FLAG_UNK1000))
		return;

	if (onYoshi())
		unk15C = 80.0f;
	else
		unk15C = 50.0f;

	TBGWallCheckRecord record(mPosition.x, mPosition.y + 60.0f, mPosition.z,
	                          unk15C, 2, 0);

	gpMap->isTouchedWallsAndMoveXZ(&record);
	if (!isInvincible()) {
		for (int i = 0; i < record.mResultWallsNum; ++i)
			if (record.mResultWalls[i]->isThing5())
				damageExec(&mFloorHitActor, record.mResultWalls[i]->getData());

		// TODO: do we have to use a TPartition here after all?
		if (record.mResultWallsNum == 2
		    && record.mResultWalls[0]->getNormal().squared() < -0.9f) {

			JGeometry::TVec3<f32> normal1 = record.mResultWalls[0]->getNormal();
			JGeometry::TVec3<f32> normal2 = record.mResultWalls[1]->getNormal();

			f32 planeDist1 = record.mResultWalls[0]->getPlaneDistance();
			f32 planeDist2 = record.mResultWalls[1]->getPlaneDistance();

			f32 dist1 = normal1.dot(mPosition) + planeDist1;
			f32 dist2 = normal2.dot(mPosition) + planeDist2;

			if ((record.mResultWalls[0]->getActor() != nullptr
			     && record.mResultWalls[0]->getActor()->getActorType()
			            == 0x400002BD)
			    || (record.mResultWalls[1]->getActor() != nullptr
			        && record.mResultWalls[1]->getActor()->getActorType()
			               == 0x400002BD)) {

				if (dist1 < 10.0f || dist2 < 10.0f) {
					int hp = mDeParams.mHpMax.get();
					floorDamageExec(hp, 3, 0, mMotorParams.mMotorReturn.get());
				}
			}
		}
	}

	record.set(mPosition.x, mPosition.y + 30.0f, mPosition.z, unk15C * 0.5f, 1,
	           0);
	gpMap->isTouchedWallsAndMoveXZ(&record);
	if (!isInvincible()) {
		for (int i = 0; i < record.mResultWallsNum; ++i)
			if (record.mResultWalls[i]->isThing5())
				damageExec(&mFloorHitActor, record.mResultWalls[i]->getData());
	}

	checkGroundPlane(mPosition.x, mPosition.y + 25.0f, mPosition.z,
	                 &mFloorPosition.y, &mGroundPlane);

	mFloorPosition.x = gpMap->checkRoof(mPosition.x, mPosition.y + 80.0f,
	                                    mPosition.z, &mRoofPlane);
	if (!isInvincible()) {
		if ((isTouchGround4cm()) && mGroundPlane->isThing5()
		    && !checkStatusFlag(STATUS_FLAG_UNK10000)) {

			damageExec(&mFloorHitActor, mGroundPlane->getData());
		}

		if (mPosition.y + 160.0f > mFloorPosition.x && mRoofPlane->isThing5()) {
			damageExec(&mFloorHitActor, mRoofPlane->getData());
		}
	}

	if (mGroundPlane->isLegal()) {
		mSlopeAngle
		    = matan(mGroundPlane->getNormal().z, mGroundPlane->getNormal().x);

		if (isSlipStart() || checkFlag(0x800))
			mInput |= 0x8;

		if (mPosition.y > mFloorPosition.y + 100.0f)
			mInput |= 0x4;

		unk118 &= ~0x800;
	}
}

void TMario::getActorMtx(const THitActor&, Mtx) { }

void TMario::checkRideMovement()
{
	const TLiveActor* actor = nullptr;

	JGeometry::TVec3<f32> pos = mPosition;

	pos.x += 0.8f * (50.0f * JMASSin(mFaceAngle.y));
	pos.z += 0.8f * (50.0f * JMASCos(mFaceAngle.y));

	TBGCheckData* wall = checkWallPlane(&pos, 50.0f, unk15C);

	const TLiveActor* groundActor = mGroundPlane->getActor();

	if (wall != nullptr && !checkStatusFlag(STATUS_FLAG_JUMPING)
	    && (isTouchGround4cm()))
		actor = groundActor;

	if (groundActor != nullptr && mStatus == 0x8008a9
	    && (mStatusState == 2 || mStatusState == 3))
		actor = groundActor;

	if (checkStatusFlag(STATUS_FLAG_UNK20000000) && wall != nullptr
	    && wall->getActor() != nullptr)
		actor = wall->getActor();

	if (actor != nullptr) {
		if (unk2C0 == nullptr || unk2C0 != actor) {
			unk2C0 = actor;
			unk30C = unk2C0->mRotation.y;
			checkRideReCalc();
		} else {
			Mtx ridingMtx;
			getRidingMtx(ridingMtx);
			MTXMultVec(ridingMtx, &unk2F4, &mPosition);

			mFaceAngle.y
			    += (s16)((unk2C0->mRotation.y - unk30C) * 32768.0f / 180.0f);
			unk30C = unk2C0->mRotation.y;
		}
	} else {
		unk2C0 = nullptr;
	}
}

void TMario::checkRideReCalc()
{
	if (unk2C0 == nullptr)
		return;

	Mtx ridingMtx;
	getRidingMtx(ridingMtx);
	MTXInverse(ridingMtx, ridingMtx);
	unk300 = unk2F4;
	MTXMultVec(ridingMtx, &mPosition, &unk2F4);
}

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
		switch (mStatus & STATUS_TYPE_MASK) {
		case STATUS_TYPE_WAITING:
			result = waitMain();
			break;
		case STATUS_TYPE_RUNNING:
			result = moveMain();
			break;
		case STATUS_TYPE_JUMPING:
			result = jumpMain();
			break;
		case STATUS_TYPE_SWIMMING:
			result = swimMain();
			break;
		case STATUS_TYPE_DEMO:
			result = demoMain();
			break;
		case STATUS_TYPE_SPECIAL:
			result = specMain();
			break;
		case STATUS_TYPE_ACTION:
			result = actnMain();
			break;
		}
	}
}

void TMario::canReadBillboard(int) { }

void TMario::talkMove() { }

void TMario::view1stMove() { }

void TMario::calcGroundMtx(const JGeometry::TVec3<f32>& param_1)
{

	JGeometry::TVec3<f32> local_14 = param_1;
	const TBGCheckData* ground;
	local_14.y = gpMap->checkGround(local_14, &ground);

	JGeometry::TVec3<f32> local_24;
	local_24.x = local_14.x + JMASCos(mFaceAngle.y);
	local_24.y = local_14.y + 30.0f;
	local_24.z = local_14.z - JMASSin(mFaceAngle.y);
	local_24.y = gpMap->checkGround(local_24, &ground);

	JGeometry::TVec3<f32> local_30;
	local_30.x = local_14.x + JMASSin(mFaceAngle.y);
	local_30.y = local_14.y + 30.0f;
	local_30.z = local_14.z + JMASCos(mFaceAngle.y);
	local_30.y = gpMap->checkGround(local_30, &ground);

	unk250[0][0] = local_24.x - local_14.x;
	unk250[1][0] = local_24.y - local_14.y;
	unk250[2][0] = local_24.z - local_14.z;

	unk250[0][1] = 0.0f;
	unk250[1][1] = 1.0f;
	unk250[2][1] = 0.0f;

	unk250[0][2] = local_30.x - local_14.x;
	unk250[1][2] = local_30.y - local_14.y;
	unk250[2][2] = local_30.z - local_14.z;

	unk250[0][3] = local_14.x;
	unk250[1][3] = local_14.y + 18.0f;
	unk250[2][3] = local_14.z;
}

void TMario::thinkSituation()
{
	unk11C = unk118;
	if (unkBC < 0.0f)
		unkBC += mBodyAngleParamsFree.mWaistAngleChangeRate.get();
	else
		unkBC = 0.0f;

	if (mHolder && mHolder->getTakingMtx()) {
		mPosition.x = mHolder->getTakingMtx()[0][3];
		mPosition.y = mHolder->getTakingMtx()[1][3];
		mPosition.z = mHolder->getTakingMtx()[2][3];
	}

	unk118 &= ~0x2;
	unk118 &= ~0x20;

	if (mGroundPlane->isUnderground())
		unk118 |= 0x2;

	if (isMario() && isTouchGround4cm() && mStatus != 0x133F
	    && (mGroundPlane->isIllegalData() || mGroundPlane->isOob())) {
		unk2BA += mDeParams.mIllegalPlaneCtInc.get();
		if (unk2BA > mDeParams.mIllegalPlaneTime.get())
			decHP(mDeParams.mHpMax.get());
	} else {
		unk2BA -= 1;
		if (unk2BA < 0)
			unk2BA = 0;
	}

	if (isMario()) {
		if (checkFlag(0x2) == true && (unk11C & 2 ? true : false) == false)
			MSBgm::startBGM(0x8001001b);

		if (checkFlag(0x2) == false && (unk11C & 2 ? true : false) == true)
			MSBgm::stopBGM(0x8001001b, 10);
	}

	const TBGCheckData* ground;
	f32 dVar7 = gpMap->checkGround(mPosition.x, mPosition.y - mVel.y,
	                               mPosition.z, &ground);

	if (ground->isDeathPlane() && dVar7 > mPosition.y) {
		unk118 |= 0x400;
		mHealth = 0;
		mAnmSound->stop();
		if (mYoshi)
			mYoshi->kill();
		changePlayerStatus(0x208B9, 0, true);
		if (mAnimationId != 0x120)
			startSoundActor(0x786B);
		gpCamera->unk64 |= 0x800;
		gpMarDirector->unk4E |= 0x8;
		return;
	}

	J3DGetTranslateRotateMtx(0, mModelFaceAngle, 0, mPosition.x, mPosition.y,
	                         mPosition.z, unk1F0);

	mLightID = 0;
	if (mGroundPlane->isShadow() && mFloorPosition.y + 200.0f > mPosition.y) {
		mLightID = mGroundPlane->getData();
		if (mLightID == 1)
			unk118 |= 0x20;
	}

	if (gpCubeShadow != nullptr && gpCubeShadow->getInCubeNo(mPosition) != -1)
		mLightID = 1;

	unk118 &= ~0x10;

	if (isMario()) {
		if (isTouchGround4cm()) {
			if (gpModelWaterManager->askHitWaterParticleOnGround(mPosition))
				unk118 |= 0x10;
		}
	}

	if (mGroundPlane->isUnk300()) {
		if (onYoshi())
			getOffYoshi(false);
		gpMarDirector->setNextStage(mGroundPlane->getData(), nullptr);
		unk114 &= ~0x2;
		unk114 &= ~0x400;
	}

	if (SMS_isOptionMap()) {
		mPosition.z = mOptionParams.mZ.get();
		if (mPosition.x < mOptionParams.mXMin.get())
			mPosition.x = mOptionParams.mXMin.get();
		if (mPosition.x > mOptionParams.mXMax.get())
			mPosition.x = mOptionParams.mXMax.get();
	}

	if (!checkStatusFlag(STATUS_FLAG_JUMPING))
		unk2BC = mPosition.y;

	calcGroundMtx(mPosition);

	if (gpMarDirector->isDemoMode3() || gpMarDirector->isDemoMode4()
	    || gpMarDirector->isTalkModeNow()
	    || checkStatusFlag(STATUS_FLAG_UNK1000))
		unk118 = 0x8;
	else
		unk118 &= ~0x8;
}

void TMario::thinkWaterSurface()
{
	if (checkStatusFlag(STATUS_FLAG_UNK10000))
		return;

	BOOL wasInWater = checkFlag(0x30000);
	bool isInWater  = false;
	if (checkFlag(0x30000) == true)
		isInWater = true;
	else
		mFloorPosition.z = mPosition.y;

	unk118 &= ~0x10000;
	unk118 &= ~0x20000;

	if (mGroundPlane->isPool()) {
		mFloorPosition.z = gpPoolManager->getWaterLevel(mGroundPlane);
		if (mFloorPosition.z > mPosition.y) {
			isInWater = true;
			unk118 |= 0x10000;
		}
	}

	f32 dy = mPosition.y - unk29C.y;
	if (dy > 0.0f)
		dy = 0.0f;

	f32 waterFloorY = gpMap->checkGround(
	    mPosition.x,
	    mFloorPosition.z - dy + mSwimParams.mWaterLevelCheckHeight.get(),
	    mPosition.z, &mWaterFloor);

	if (mWaterFloor->isWaterSurface()) {
		mFloorPosition.z = waterFloorY;
		if (mFloorPosition.z >= mPosition.y) {
			isInWater = true;
			unk118 |= 0x20000;
		}
	} else {
		const TBGCheckData* localBg;
		gpMap->checkGround(mPosition.x, mPosition.y, mPosition.z, &localBg);
		// TODO: identify BG type 0x810B (camera-noclip variant of 0x10B)
		if (localBg->mBGType == 0x810B ? true : false) {
			isInWater = true;
			unk118 |= 0x20000;
		}
	}

	if (isInWater && mPosition.y < mFloorPosition.z) {
		if (mFloorPosition.z > mPosition.y + mRunParams.mSwimDepth.get()) {
			// deep water — Mario plunges in
			if (onYoshi()) {
				mYoshi->disappear();
				if (mWaterGun != nullptr) {
					mWaterGun->changeNozzle(TWaterGun::Hover, true);
					normalizeNozzle();
				}
			}

			if (mPosition.y + 160.0f > mFloorPosition.z)
				rippleEffect();
			swimmingBubbleEffect();

			u32 statusId  = mStatus & STATUS_TYPE_AND_ID_MASK;
			bool canEnter = true;
			if (checkStatusFlag(STATUS_FLAG_SWIMMING))
				canEnter = false;
			if (fabricatedActionInline())
				canEnter = false;
			if (statusId >= 0x145 && statusId <= 0x14A)
				canEnter = false;
			if (statusId >= 0x140 && statusId <= 0x143)
				canEnter = false;
			if (isTaken())
				canEnter = false;

			if (canEnter == true) {
				mForwardVel *= mSwimParams.mStartVMult.get();
				mVel.y *= mSwimParams.mStartVYMult.get();

				if (checkStatusFlag(0x20000)) {
					changePlayerStatus(0x24DA, 0, true);
				} else if (checkFlag(0x4000)) {
					changePlayerStatus(0x24D5, 0, true);
					mVel.y      = 0.0f;
					mPosition.y = mFloorPosition.z;
					startSoundActor(0x828);
				} else {
					changePlayerStatus(0x22D1, 0, true);
				}
			}
		} else if (mFloorPosition.z
		           < mPosition.y
		                 + mWaterEffectParams.mRunningRippleDepth.get()) {
			if (mStatus == 0x4000440) {
				if (getMotionFrameCtrl().checkPass(38.0f)
				    || getMotionFrameCtrl().checkPass(8.0f))
					runningRippleEffect();
			}
		} else {
			rippleEffect();
		}
	}

	if (mGroundPlane->isWetGround() && mStatus == 0x4000440) {
		if (getMotionFrameCtrl().checkPass(38.0f)
		    || getMotionFrameCtrl().checkPass(8.0f))
			runningRippleEffect();
	}

	BOOL nowInWater = checkFlag(0x30000);

	J3DGetTranslateRotateMtx(0, mModelFaceAngle, 0, mPosition.x,
	                         mFloorPosition.z, mPosition.z, unk220);

	unk190 = mPosition.x;
	unk194 = mFloorPosition.z;
	unk198 = mPosition.z;
	MTXCopy(mModel->unk8->getAnmMtx(mBoneIDs[10]), unk1C0);

	if (nowInWater != wasInWater) {
		inOutWaterEffect(mFloorPosition.z);
		f32 depth = mFloorPosition.z - mFloorPosition.y;
		if (wasInWater == true && nowInWater == false) {
			// exited water
			unk362 = 0x78;
			if (depth < 32.0f) {
				SMSGetMSound()->startSoundActor(0x1939, &mPosition, 0, nullptr,
				                                0, 4);
			} else if (depth < 80.0f) {
				SMSGetMSound()->startSoundActor(0x181D, &mPosition, 0, nullptr,
				                                0, 4);
			} else {
				SMSGetMSound()->startSoundActor(0x181E, &mPosition, 0, nullptr,
				                                0, 4);
			}
		} else {
			// entered water
			if (depth < 32.0f) {
				SMSGetMSound()->startSoundActor(0x1938, &mPosition, 0, nullptr,
				                                0, 4);
			} else if (depth < 80.0f) {
				SMSGetMSound()->startSoundActor(0x1805, &mPosition, 0, nullptr,
				                                0, 4);
			} else {
				SMSGetMSound()->startSoundActor(0x1806, &mPosition, 0, nullptr,
				                                0, 4);
			}
		}
	}

	bool deepUnderwater;
	if (checkFlag(0x30000)
	    && unk160[1].y < mFloorPosition.z - mSwimParams.mCanBreathDepth.get())
		deepUnderwater = true;
	else
		deepUnderwater = false;

	if (deepUnderwater || checkFlag(0x1000)) {
		f32 prevAir = unk12C;
		if (isWearingHelm()) {
			if (mStatus != 0x10020370) {
				unk12C -= mSwimParams.mAirDecDive.get();
			}
		} else {
			unk12C -= mSwimParams.mAirDec.get();
		}

		if ((s32)prevAir != (s32)unk12C) {
			rumbleStart(0x14, mMotorParams.mMotorWall.get());
			unk14C = (s16)unk55C;
		}

		if (unk12C < 1.0f) {
			unk12C = 0.0f;
			loserExec();
			changePlayerStatus(0x224E0, 0, false);
		}
	} else {
		unk12C += mSwimParams.mAirInc.get();
		if (unk12C >= unk130)
			unk12C = unk130;
	}
}

void TMario::thinkSand()
{
	if (checkFlag(0x30000) == false && mGroundPlane->isSand() == true) {
		unk118 |= 0x40000;
		emitSandEffect();
	} else {
		unk118 &= ~0x40000;
	}
}

void TMario::thinkParams()
{
	mRotation.y = SHORTANGLE2DEG(mFaceAngle.y);
	if (unk14C > 0)
		unk14C -= 1;
	if (!checkFlag(0x400)) {
		if (checkFlag(0x30000)) {
			// TODO: inline
			bool b;
			if (checkFlag(0x30000)
			    && unk160[1].y
			           < mFloorPosition.z - mSwimParams.mCanBreathDepth.get())
				b = true;
			else
				b = false;
			if (!b) {
				if (mWaterFloor->isThing5() && mWaterFloor->isThing5()
				    && !checkStatusFlag(STATUS_FLAG_UNK10000)) {
					floorDamageExec(getDmgMapCode(mWaterFloor->getData()));
				}
			}

			unk126 = 0;
			unk128 = mDeParams.mHotTimer.get();
		} else {
			if (mCap) {
				if (!(mCap->unk4 & 1 ? true : false)) {
					unk126 += 1;
					if (unk126 > unk128) {
						decHP(1);
						SMSGetMSound()->startSoundSystemSE(0x480C, 0, nullptr,
						                                   0);
						unk128 = mDeParams.mHotTimer.get();
						unk126 = 0;
						rumbleStart(0x14, mMotorParams.mMotorWall.get());
						unk14C = unk55C;
					}
				}
			}

			unk122 = 0;
		}

		if (mGroundPlane != nullptr && mGroundPlane->getActor() != nullptr
		    && mGroundPlane->getActor()->getActorType() == 0x400002c7) {
			emitFootPrintWithEffect(-1, 0x42);
		}
	}
}

void TMario::getOffYoshi(bool fly)
{
	mInput &= ~0x8000;

	if (fly) {
		changePlayerStatus(0x89C, 0, 0);
		mYoshi->getOff(true);
	} else {
		changePlayerStatus(STATUS_BACK_JUMP, 0, 0);
		mVel.y = mJumpParams.mGetOffYoshiY.get();
		mYoshi->getOff(false);
	}

	setAnimation(0x4D, 1.0f);
	unk78 &= ~0x100;
	mPosition.y += 100.0f;
	mForwardVel = -8.0f;

	mWaterGun->changeNozzle(TWaterGun::Hover, true);
	normalizeNozzle();
	mWaterGun->resetWaterToFull();
}

void TMario::checkYoshiGetOff()
{
	if (onYoshi() && mGamePad->checkFrameMeaning(0x200000))
		getOffYoshi(false);
}

void TMario::thinkYoshiHeadCollision()
{
	if (!onYoshi())
		return;

	JGeometry::TVec3<f32> headPos = mPosition;

	f32 front = mYoshiParams.mHeadFront.get();
	headPos.x += front * JMASSin(mFaceAngle.y);
	headPos.z += front * JMASCos(mFaceAngle.y);

	TBGWallCheckRecord record(headPos.x, headPos.y + 100.0f, headPos.z,
	                          mYoshiParams.mHeadRadius.get(), 4, 0);

	if (gpMap->isTouchedWallsAndMoveXZ(&record) == true) {
		f32 dx = record.mCenter.x - headPos.x;
		f32 dz = record.mCenter.z - headPos.z;
		f32 f4 = std::sqrtf(dx * dx + dz * dz);

		f32 f2 = f4;
		if (f4 > 0.0f) {
			dx *= 1.0f / f4;
			dz *= 1.0f / f4;

			if (50.0f < f4)
				f2 = 50.0f;

			dx *= f2;
			dz *= f2;

			mPosition.x += dx;
			mPosition.z += dz;
		}
	}
}

void TMario::thinkDiving() { }

void TMario::thinkTorocco()
{
	mToroccoAngle += unk108->mStickH * mDeParams.mRecoverTimer.get();
}

void TMario::thinkSound()
{
	if (mStatus != 0x208b8 && mSound != nullptr)
		mSound->stop(1);
}

void TMario::checkWet()
{
	if (isMario() == 0)
		return;

	if (onYoshi())
		return;

	if (unk362 <= 0)
		return;

	unk362 -= 1;

	const TBGCheckData* floor;
	f32 tmp;
	checkGroundPlane(mPosition.x, mPosition.y + 320.0f, mPosition.z, &tmp,
	                 &floor);

	if (floor->isWaterSurface())
		return;

	if (mFloorPosition.z > mPosition.y)
		return;

	if (checkStatusFlag(STATUS_FLAG_UNK200))
		return;

	if (unk362 & 7)
		return;

	unk158->mPos.value = mPosition;
	unk158->mPos.value.y += 5.0f;

	JGeometry::TVec3<f32> v = mVel;
	v *= 0.3f;
	unk158->mV.value = v;
	gpModelWaterManager->emitRequest(*unk158);
}

void TMario::gunExec()
{
	if (!onYoshi())
		gpModelWaterManager->unk5D5F = 0;

	if (!checkFlag(0x8000) && !onYoshi())
		return;

	mWaterGun->updateUnk1C88(0);
	mWaterGun->triggerPressureMovement(*unk108);

	unk118 &= ~0x80;
	if (checkFlag(0x30000)) {
		if (!onYoshi() && mWaterGun->suck() == true) {
			unk118 |= 0x80;
			if (checkFlag(0x10000) && mGroundPlane->isPool())
				gpPoolManager->subWaterLevel(mGroundPlane);
		}

		if (mWaterGun->mCurrentWater == mWaterGun->getMaxWater()
		    && unk380 == 0) {
			mWaterGun->emit();
			mWaterGun->resetWaterToFull();
		}
	} else if (unk390 == 0) {
		mWaterGun->emit();
	}

	if (onYoshi() && unk108->mAnalogR > 0.0f)
		mWaterGun->emit();

	if (unk114 & 0x80 ? true : false)
		mWaterGun->resetWaterToFull();

	if (mStatus != STATUS_BACK_JUMP && mStatus != 0x208b8
	    && mGamePad->checkFrameMeaning(0x200000) && !onYoshi()
	    && mStatus != 0x800447)
		mWaterGun->changeBackup();

	if ((int)mWaterGun->mCurrentNozzle == TWaterGun::Spray
	    && mWaterGun->mIsEmitWater != 0) {
		JGeometry::TVec3<f32> local_34;
		local_34.x = JMASSin(mFaceAngle.y);
		local_34.y = 0.0f;
		local_34.z = JMASCos(mFaceAngle.y);

		for (int i = 0; i < mGraffitoParams.mFootEraseTimes.get(); ++i) {
			f32 fVar1 = mGraffitoParams.mFootEraseSize.get();
			JGeometry::TVec3<f32> local_90
			    = mPosition + local_34 * mGraffitoParams.mFootEraseFront.get();
			gpPollution->clean(local_90.x, local_90.y, local_90.z, fVar1);
		}
	}
}

void TMario::playerControl(JDrama::TGraphics* param_1)
{
	unk9C  = mFaceAngle.y;
	unk29C = mPosition;
	unk114 &= ~0x1;
	if (gpMarDirector->unk124 == 1 && mStatus != 0x10001308)
		changePlayerStatus(0x10001308, 0, false);

	if (gpMarioOriginal == this) {
		if (gpCamera->isLButtonCameraSpecifyMode(gpCamera->mMode)
		    && !isActionThing() && gpMarDirector->unk124 != 1) {
			mFaceAngle.y
			    = (gpCamera->unk258 - 0x8000) - gpCamera->getOffsetAngleY();
		}
	}

	checkPlayerAction(param_1);
	checkCollision();
	considerTake();
	checkYoshiGetOff();
	thinkYoshiHeadCollision();
	thinkTorocco();
	stateMachine();
	stateMachineUpper();
	thinkSituation();
	thinkWaterSurface();
	thinkSand();
	thinkHeight();
	thinkParams();
	checkRideReCalc();
	checkWet();
	checkGraffito();
	thinkDirty();
	checkSink();
	gunExec();
	thinkSound();
}

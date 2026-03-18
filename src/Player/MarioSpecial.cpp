#include <Player/MarioMain.hpp>

#include <Map/Map.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <MSound/MSound.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <Camera/Camera.hpp>
#include <dolphin/mtx.h>
#include <Player/Watergun.hpp>
#include <Player/NozzleTrigger.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionData.hpp>
#include <Strategic/LiveActor.hpp>
#include <System/MarDirector.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>

BOOL TMario::specMain()
{
	mWireBounceVelPrev = mWireBounceVel;

	f32 bounceVel = mWireBounceVel;
	f32 wireSag = mWireSag;
	f32 factor1 = mWireParams.mSwingRate.value;
	mWireBounceVel = bounceVel - wireSag * factor1;

	f32 sag = mWireSag;
	f32 vel = mWireBounceVel;
	mWireSag = sag + vel;

	f32 sag2 = mWireSag;
	f32 damping = mWireParams.mWireSwingBrake.value;
	mWireSag = sag2 * damping;

	f32 maxBounce = mWireParams.mWireSwingMax.value;
	f32 curSag = mWireSag;
	f32 negMax = -maxBounce;
	if (curSag < negMax)
		mWireSag = negMax;

	f32 posMax = mWireParams.mWireSwingMax.value;
	if (posMax < mWireSag)
		mWireSag = posMax;

	switch (mAction) {
	case 0x18100340:
		barWait();
		break;
	case 0x10100341:
		// barClimb entry variant - rumble + bar process
		if (mActionTimer == 0) {
			rumbleStart(0x15, mMotorParams.mMotorWall.value);
			mActionTimer++;
		}
		if ((u32)mHeldObject == 0) {
			changePlayerStatus(0x208b6, 0, false);
			break;
		}
		mForwardVel = 0.0f;
		barProcess();
		{
			setAnimation(6, 1.0f);
			if (isLast1AnimeFrame()) {
				changePlayerStatus(0x18100340, 0, false);
			}
		}
		mFaceAngle.x = 0;
		mModelFaceAngle = mFaceAngle.y;
		break;
	case 0x10100344:
		barClimb();
		break;
	case 0x00200345:
		// kick roof
		setAnimation(0x102, 1.0f);
		kickRoofEffect();
		{
			u16 timer = mActionTimer;
			mActionTimer = timer + 1;
			if (timer == 8) {
				startVoice(0x7890);
			}
		}
		if (isLast1AnimeFrame()) {
			mPosition.y = mPosition.y + 10.0f;
			mFloorPosition.y = mPosition.y;
			mInput &= ~0x4;
			changePlayerStatus(0x0C400201, 0, false);
			break;
		}
		return 0;
	case 0x00200346:
		// fence slide anim
		setAnimation(0x103, 1.0f);
		if (isLast1AnimeFrame()) {
			changePlayerStatus(0x00200349, 0, false);
			break;
		}
		return 0;
	case 0x00200347:
		// fence kick
		setAnimation(0x101, 1.0f);
		{
			f32 entryR1 = mAttackParamsKickRoof.mRadius.value;
			mAttackRadius = entryR1;
			calcEntryRadius();
			f32 entryR2 = mAttackParamsKickRoof.mHeight.value;
			mAttackHeight = entryR2;
			calcEntryRadius();
		}
		kickRoofEffect();
		if (isLast1AnimeFrame()) {
			mInput &= ~0x2;
			changePlayerStatus(0x00200349, 0, false);
			break;
		}
		return 0;
	case 0x08200348:
		// roof check
		{
			f32 roofHeight = gpMap->checkRoof(mPosition.x, mPosition.y + 30.0f, mPosition.z, &mRoofPlane);
			mFloorPosition.y = roofHeight;
			mActionTimer++;
			if ((mInput & 1) && mActionTimer > 0x78) {
				changePlayerStatus(0x00200349, 0, false);
				break;
			}
		}
		if (roofCommonEvents())
			return 1;
		{
			mModelFaceAngle = mFaceAngle.y;
			setAnimation(0x35, 1.0f);
			f32 zero = 0.0f;
			mForwardVel = zero;
			mSlideVelX = zero;
			mSlideVelZ = zero;
			f32 roofY = mFloorPosition.y;
			mPosition.y = roofY - 10.0f;
			mVel.z = zero;
			mForwardVel = zero;
			mVel.x = zero;
			if (isLast1AnimeFrame()) {
				changePlayerStatus(0x00200349, 0, false);
			}
		}
		return 0;
	case 0x00200349:
		// roof common
		if (roofCommonEvents())
			return 1;
		{
			u32 input = mInput;
			if (input & 1) {
				changePlayerStatus(0x0020054a, mActionArg, false);
				break;
			}
			if (mActionArg & 1) {
				setAnimation(0xc6, 1.0f);
			} else {
				setAnimation(0xc7, 1.0f);
			}
			f32 zero = 0.0f;
			mForwardVel = zero;
			mSlideVelX = zero;
			mSlideVelZ = zero;
			f32 roofY = mFloorPosition.y;
			mPosition.y = roofY - 10.0f;
			mVel.z = zero;
			mForwardVel = zero;
			mVel.x = zero;
		}
		break;
	case 0x0020054a:
		moveRoof();
		break;
	case 0x3800034b:
		hanging();
		break;
	case 0x3000054c:
		// hang landing wait
		if (mInput & 0x4) {
			startHangLanding(0x208b6);
			break;
		}
		waitProcess();
		setAnimation(0, 1.0f);
		if (isLast1AnimeFrame()) {
			changePlayerStatus(0x0C400201, 0, false);
		}
		return 0;
	case 0x3000054e:
		// hang landing wait 2
		if (mInput & 0x4) {
			startHangLanding(0x208b6);
			break;
		}
		waitProcess();
		setAnimation(0x1c, 1.0f);
		if (isLast1AnimeFrame()) {
			changePlayerStatus(0x3800034b, 0, false);
		}
		return 0;
	case 0x30000550:
		// hang landing wait 3
		if (mInput & 0x4) {
			startHangLanding(0x208b6);
			break;
		}
		waitProcess();
		setAnimation(0x34, 1.0f);
		if (isLast1AnimeFrame()) {
			changePlayerStatus(0x0C400201, 0, false);
		}
		return 0;
	case 0x10020370:
		// rope position
		{
			MtxPtr mtx = mHeldObject->getTakingMtx();
			mPosition.x = mtx[0][3];
			mPosition.y = mtx[1][3];
			mPosition.z = mtx[2][3];
			mFaceAngle.x = 0;
			mModelFaceAngle = mFaceAngle.y;
			setAnimation(0x120, 1.0f);
		}
		return 0;
	case 0x350:
		wireWait();
		break;
	case 0x351:
		wireSWait();
		break;
	case 0x352:
		// wireWaitToHang R
		getOnWirePosAngle(&mPosition, &mModelFaceAngle);
		{
			s16 angle = mModelFaceAngle;
			mFaceAngle.y = angle + 0x4000;
			setAnimation(0xde, 1.0f);
		}
		if (isLast1AnimeFrame()) {
			changePlayerStatus(0x351, 0, false);
		}
		return 0;
	case 0x353:
		// wireWaitToHang L
		getOnWirePosAngle(&mPosition, &mModelFaceAngle);
		{
			s16 angle = mModelFaceAngle;
			mFaceAngle.y = angle - 0x4000;
			setAnimation(0xdf, 1.0f);
		}
		if (isLast1AnimeFrame()) {
			changePlayerStatus(0x351, 0, false);
			// swap wire start/end
			JGeometry::TVec3<f32> temp;
			temp = mWireStartPos;
			mWireStartPos = mWireEndPos;
			mWireEndPos = temp;
			mWirePosRatio = 1.0f - mWirePosRatio;
		}
		return 0;
	case 0x10000554:
		// wireSWaitToHang
		getOnWirePosAngle(&mPosition, &mModelFaceAngle);
		{
			s16 angle = mModelFaceAngle;
			mFaceAngle.y = angle + 0x4000;
			setAnimation(0xe1, 1.0f);
		}
		if (isLast1AnimeFrame()) {
			u8 canHang = 0;
			if ((u32)mHeldObject == 0) {
				if (!onYoshi())
					canHang = 1;
			}
			if (canHang) {
				changePlayerStatus(0x10000357, 0, false);
				break;
			}
			changePlayerStatus(0x88c, 0, false);
		}
		return 0;
	case 0x10000555:
		// wireSWaitToHang L variant
		getOnWirePosAngle(&mPosition, &mFaceAngle.y);
		{
			mModelFaceAngle = mFaceAngle.y;
			setAnimation(0xe2, 1.0f);
		}
		if (isLast1AnimeFrame()) {
			u8 canHang = 0;
			if ((u32)mHeldObject == 0) {
				if (!onYoshi())
					canHang = 1;
			}
			if (canHang) {
				changePlayerStatus(0x10000357, 0, false);
				break;
			}
			changePlayerStatus(0x88c, 0, false);
		}
		return 0;
	case 0x10000556:
		// wire swing enter
		getOnWirePosAngle(&mPosition, &mFaceAngle.y);
		{
			mModelFaceAngle = mFaceAngle.y;
			setAnimation(0xe4, 1.0f);
		}
		if (isLast1AnimeFrame()) {
			changePlayerStatus(0x350, 0, false);
		}
		return 0;
	case 0x10000357:
		wireHanging();
		break;
	case 0x10000359:
		// wire rolling variant
		wireRolling();
		break;
	case 0x35b:
		// wire turn L
		getOnWirePosAngle(&mPosition, &mModelFaceAngle);
		{
			s16 angle = mModelFaceAngle;
			mFaceAngle.y = angle - 0x4000;
			setReverseAnimation(0xdf, 1.0f);
		}
		if (isAnimeLoopOrStop()) {
			changePlayerStatus(0x350, 0, false);
		}
		return 0;
	case 0x35d:
		// wire turn R
		getOnWirePosAngle(&mPosition, &mModelFaceAngle);
		{
			s16 angle = mModelFaceAngle;
			mFaceAngle.y = angle + 0x4000;
			setReverseAnimation(0xde, 1.0f);
		}
		if (isAnimeLoopOrStop()) {
			changePlayerStatus(0x350, 0, false);
		}
		return 0;
	case 0x560:
	case 0x40561:
		pulling();
		break;
	case 0x3000036a:
		fencePunch();
		break;
	case 0x3000036c:
		// fence walk anim
		{
			setAnimation(0xf9, 1.0f);
			JGeometry::TVec3<f32> nextPos;
			nextPos.x = mPosition.x;
			nextPos.y = mPosition.y;
			nextPos.z = mPosition.z;
			f32 sinVal = JMASSin(mFaceAngle.y);
			f32 cosVal = JMASCos(mFaceAngle.y);
			nextPos.x += 50.0f * sinVal * 1.0f;
			nextPos.z += 50.0f * cosVal * 1.0f;
			const TBGCheckData* wall = checkWallPlane(&nextPos, 0.0f, 0.0f);
			if (wall == NULL) {
				s16 angle = mFaceAngle.y;
				mFaceAngle.y = angle + 0x8000;
				setPlayerVelocity(0.0f);
				mForwardVel = 0.0f;
				changePlayerStatus(0x208b6, 0, false);
			}
			if (isLast1AnimeFrame()) {
				f32 margin = 50.0f;
				if (mFloorPosition.y + margin > mPosition.y) {
					mPosition.y += margin;
				}
				if (mFloorPosition.y - margin < mPosition.y) {
					mPosition.y -= margin;
				}
				changePlayerStatus(0x38000368, 0, false);
				break;
			}
		}
		return 0;
	case 0x3000036b:
		// fence walk anim 2
		{
			setAnimation(0xfa, 1.0f);
			JGeometry::TVec3<f32> nextPos;
			nextPos.x = mPosition.x;
			nextPos.y = mPosition.y;
			nextPos.z = mPosition.z;
			f32 sinVal = JMASSin(mFaceAngle.y);
			f32 cosVal = JMASCos(mFaceAngle.y);
			nextPos.x += 50.0f * sinVal * 1.0f;
			nextPos.z += 50.0f * cosVal * 1.0f;
			const TBGCheckData* wall = checkWallPlane(&nextPos, 0.0f, 0.0f);
			if (wall == NULL) {
				s16 angle = mFaceAngle.y;
				mFaceAngle.y = angle + 0x8000;
				setPlayerVelocity(0.0f);
				mForwardVel = 0.0f;
				changePlayerStatus(0x208b6, 0, false);
			}
			if (isLast1AnimeFrame()) {
				changePlayerStatus(0x38000368, 0, false);
				break;
			}
		}
		return 0;
	case 0x30000569:
	case 0x38000368:
		fenceMove();
		break;
	case 0x10100342:
		break;
	default:
		break;
	}

	return 0;
}

BOOL TMario::fencePunch()
{
	JGeometry::TVec3<f32> nextPos;
	nextPos.x = mPosition.x;
	nextPos.y = mPosition.y;
	nextPos.z = mPosition.z;
	f32 sinVal = JMASSin(mFaceAngle.y);
	f32 cosVal = JMASCos(mFaceAngle.y);
	nextPos.x += 0.5f * (50.0f * sinVal);
	nextPos.z += 0.5f * (50.0f * cosVal);
	const TBGCheckData* wall = checkWallPlane((Vec*)&nextPos, 80.0f, 50.0f);

	if (mInput & 0x2) {
		if (gpMSound->gateCheck(0x193a)) {
			MSoundSESystem::MSoundSE::startSoundActor(
			    0x193a, (Vec*)&mPosition, 0, (JAISound**)0, 0, 4);
		}
		rumbleStart(0x15, mMotorParams.mMotorWall.value);
		startJumpWall();
		return;
	}

	if (wall != 0) {
		mPosition = nextPos;
	} else {
		s16 angle = mFaceAngle.y;
		mFaceAngle.y = angle + 0x8000;
		mModelFaceAngle = mFaceAngle.y;
		setPlayerVelocity(0.0f);
		changePlayerStatus(0x88c, 0, false);
		return;
	}

	setAnimation(0x100, 1.0f);
	f32 entryR1 = mAttackParamsFencePunch.mRadius.value;
	mAttackRadius = entryR1;
	calcEntryRadius();
	f32 entryR2 = mAttackParamsFencePunch.mHeight.value;
	mAttackHeight = entryR2;
	calcEntryRadius();

	*(u32*)((u8*)this + 0x64) &= ~0x2;

	mModelFaceAngle = mFaceAngle.y;

	if (getMotionFrameCtrl().checkPass(5.0f)) {
		emitParticle(0x39, (JGeometry::TVec3<f32>*)((u8*)this + 0x184));
		rumbleStart(0x15, mMotorParams.mMotorWall.value);

		TLiveActor* actor = *(TLiveActor**)((u8*)this + 0x2C0);
		if (actor != 0) {
			actor->receiveMessage(this, 3);
			startVoice(0x7890);

			if (actor->mActorType - 0x40000000 == 0x6a) {
				f32 val1 = *(f32*)((u8*)this + 0x2F4);
				f32 val2 = *(f32*)((u8*)this + 0x2F8);
				if (val1 < -120.0f)
					val1 = -120.0f;
				if (120.0f < val1)
					val1 = 120.0f;
				if (val2 < -190.0f)
					val2 = -190.0f;
				if (60.0f < val2)
					val2 = 60.0f;
				*(f32*)((u8*)this + 0x2F4) = val1;
				*(f32*)((u8*)this + 0x2F8) = val2;

				Mtx ridingMtx;
				getRidingMtx(ridingMtx);
				PSMTXMultVec(ridingMtx, (Vec*)((u8*)this + 0x2F4),
				             (Vec*)&mPosition);
			}
		}
	}

	if (isLast1AnimeFrame()) {
		changePlayerStatus(0x38000368, 0, false);
		return TRUE;
	}
	return FALSE;
}

void TMario::fenceMove()
{
	JGeometry::TVec3<f32> nextPos(mPosition);
	nextPos.x += 0.5f * (50.0f * JMASSin(mFaceAngle.y));
	nextPos.z += 0.5f * (50.0f * JMASCos(mFaceAngle.y));
	const TBGCheckData* wall = checkWallPlane((Vec*)&nextPos, 80.0f, 50.0f);

	if (wall != NULL) {
		if (mInput & 0x1) {
			JGeometry::TVec3<f32> movePos(mPosition);

			TMarioControllerWork* ctrl = (TMarioControllerWork*)unk108;
			f32 fenceSpeed = mJumpParams.mFenceSpeed.value;
			movePos.y += (0.015625f * ctrl->mStickV) * fenceSpeed;

			s16 camAngle = *(s16*)((u8*)gpCamera + 0x258);
			s16 angleDiff = (s16)(mFaceAngle.y - camAngle);

			f32 normalZ, normalX;
			if (angleDiff > -16384 && angleDiff < 16384) {
				normalZ = -wall->mNormal.z;
				normalX = wall->mNormal.x;
			} else {
				normalZ = wall->mNormal.z;
				normalX = -wall->mNormal.x;
			}

			f32 moveScale = 0.015625f * ctrl->mStickH;
			f32 moveX = normalZ * moveScale;
			f32 moveZ = normalX * moveScale;
			movePos.x += moveX * fenceSpeed;
			movePos.z += moveZ * fenceSpeed;

			JGeometry::TVec3<f32> checkPos(movePos);
			checkPos.x += 0.5f * (50.0f * JMASSin(mFaceAngle.y));
			checkPos.z += 0.5f * (50.0f * JMASCos(mFaceAngle.y));

			JGeometry::TVec3<f32> checkPos2(checkPos);
			const TBGCheckData* wall2 = checkWallPlane((Vec*)&checkPos, 20.0f, 50.0f);
			const TBGCheckData* wall3 = checkWallPlane((Vec*)&checkPos2, 140.0f, 50.0f);

			if (wall2 != NULL) {
				u8 isFence = (wall2->mBGType == 0x10a) ? 1 : 0;
				if (isFence) {
					if (wall3 != NULL) {
						u8 isFence2 = (wall3->mBGType == 0x10a) ? 1 : 0;
						if (isFence2) {
							if (mAction == 0x38000368) {
								changePlayerStatus(0x30000569, 0, false);
							}
							mPosition = movePos;
						}
					}
				}
			}
		} else {
			setAnimation(0xfb, 1.0f);
			changePlayerStatus(0x38000368, 0, false);
		}

		s16 newAngle = (s16)(matan(wall->mNormal.z, wall->mNormal.x) + 0x8000);
		mFaceAngle.y = newAngle;
	} else {
		s16 angle = mFaceAngle.y;
		mFaceAngle.y = angle + 0x8000;
		mModelFaceAngle = mFaceAngle.y;
		setPlayerVelocity(0.0f);
		changePlayerStatus(0x88c, 0, false);
		return;
	}

	mModelFaceAngle = mFaceAngle.y;

	mPosition.x += 0.5f * (50.0f * JMASSin(mFaceAngle.y));
	mPosition.z += 0.5f * (50.0f * JMASCos(mFaceAngle.y));

	mVel.x = 0.0f;
	mVel.y = 0.0f;
	mVel.z = 0.0f;

	int jumpResult = jumpProcess(1);

	switch (jumpResult) {
	case 1: {
		s16 angle2 = mFaceAngle.y;
		mFaceAngle.y = angle2 + 0x8000;
		changePlayerStatus(0x88c, 0, false);
		return;
	}
	case 3: {
		mPosition.x += 50.0f * JMASSin(mFaceAngle.y);
		mPosition.y = 1.0f + mFloorPosition.y;
		mPosition.z += 50.0f * JMASCos(mFaceAngle.y);
		setAnimation(0, 1.0f);
		mInput &= ~0x4;
		changePlayerDropping(0x3000054c, 0);
		return;
	}
	}

	if (mRoofPlane != NULL) {
		f32 margin = 160.0f;
		f32 roofY = mFloorPosition.x;
		if (roofY < margin + mPosition.y) {
			mPosition.y = roofY - margin;
			changePlayerStatus(0x38000368, 0, false);
		}
	}

	if (mInput & 0x2) {
		if (gpMSound->gateCheck(0x193a)) {
			MSoundSESystem::MSoundSE::startSoundActor(
			    0x193a, (Vec*)&mPosition, 0, (JAISound**)0, 0, 4);
		}
		rumbleStart(0x15, mMotorParams.mMotorWall.value);
		startJumpWall();
		return;
	}

	if (mInput & 0x8000) {
		mInput &= ~0x8000;
		changePlayerStatus(0x3000036a, 0, false);
		return;
	}

	if (mIntendedMag <= 0.0f)
		return;

	f32 f31, f30;
	if (mRidingActor == NULL) {
		f32 fenceSin = JMASSin(mFaceAngle.y);
		f32 fenceCos = JMASCos(mFaceAngle.y);
		f30 = mPosition.y - unk29C.y;

		JGeometry::TVec3<f32> posOffset(mPosition);
		posOffset.sub(unk29C);

		JGeometry::TVec3<f32> posOffset2(posOffset);

		f32 c0 = 0.0f * fenceSin;
		f32 cross1 = 0.0f * fenceCos - c0;
		f32 cross2 = 1.0f * fenceSin - 0.0f;
		f32 cross3 = c0 - 0.0f;
		f32 cross4 = 0.0f - 1.0f * fenceCos;

		f32 dotY = posOffset2.y * posOffset2.y;
		f32 d1 = cross1 * posOffset2.y;
		f32 dotXY = posOffset2.x * posOffset2.x + dotY;
		f32 d2 = cross2 * posOffset2.x + d1;
		f32 lenSq = posOffset2.z * posOffset2.z + dotXY;
		f31 = cross4 * posOffset2.z + d2;

		f32 dist;
		if (lenSq <= 0.0f) {
			dist = lenSq;
		} else {
			f32 rsqrt = __frsqrte(lenSq);
			rsqrt = (f32)rsqrt;
			f32 est = rsqrt * rsqrt;
			f32 halfRsqrt = 0.5f * rsqrt;
			f32 correction = -(lenSq * est - 3.0f);
			f32 refined = halfRsqrt * correction;
			dist = lenSq * refined;
		}
		(void)dist;
	} else {
		s16 faceAngle = mFaceAngle.y;
		f32 angleFloat = (f32)faceAngle;
		f32 rotAngle = *(f32*)((u8*)this + 0x30C);

		f32 yTop = *(f32*)((u8*)this + 0x2F8);
		f32 yBot = *(f32*)((u8*)this + 0x304);
		f32 xTop = *(f32*)((u8*)this + 0x2F4);
		f32 xBot = *(f32*)((u8*)this + 0x300);
		f32 zBot = *(f32*)((u8*)this + 0x308);

		f30 = yTop - yBot;
		f31 = xTop - xBot;

		s16 relAngle = (s16)((65536.0f / 360.0f) * rotAngle - angleFloat);
		if (relAngle != 0)
			f31 = -f31;

		JGeometry::TVec3<f32> fencePos(*(JGeometry::TVec3<f32>*)((u8*)this + 0x2F4));
		fencePos.x -= xBot;
		fencePos.y -= yBot;
		fencePos.z -= zBot;

		JGeometry::TVec3<f32> distVec(fencePos);
		f32 dist = JGeometry::TUtil<f32>::sqrt(
		    distVec.x * distVec.x + distVec.y * distVec.y
		    + distVec.z * distVec.z);
	}

	f32 absF31 = f31;
	if (f31 < 0.0f)
		absF31 = -f31;
	f32 absF30 = f30;
	if (f30 < 0.0f)
		absF30 = -f30;

	if (absF30 > 5.0f * absF31) {
		if (f30 > 0.0f) {
			setAnimation(254, 0.0f);
		} else {
			setAnimation(255, 0.0f);
		}
	} else {
		if (f31 > 0.0f) {
			setAnimation(252, 0.0f);
		} else {
			setAnimation(253, 0.0f);
		}
	}
}

void TMario::pulling()
{
	u32 input = mInput;

	// Check if B button pressed (bit 29 = 0x4)
	if (input & 0x4) {
		mHeldObject->receiveMessage(this, 8);
		mHeldObject = 0;
		startVoice(0x78e0);
		changePlayerStatus(0x88c, 0, false);
		return;
	}

	// Check if held object is still valid (flag check at 0x108 offset field)
	u32* ptr108 = *(u32**)((u8*)this + 0x108);
	if (!(*(u32*)((u8*)ptr108 + 0x4) & 0x200)) {
		mHeldObject->receiveMessage(this, 8);
		mHeldObject = 0;
		startVoice(0x78e0);
		changePlayerStatus(0x0C00022F, 0, false);
		return;
	}

	// Check A button (bit 30 = 0x2)
	if (input & 0x2) {
		setAnimation(0xf0, 1.0f);
		changePlayerJumping(0x894, 0);
		return;
	}

	// Check if action is 0x40561 and specific flag
	if (mAction == 0x40561) {
		u32 flags118 = unk118;
		u8 isGrounded;
		if (flags118 & 0x40) {
			isGrounded = 1;
		} else {
			isGrounded = 0;
		}
		if (!isGrounded) {
			changePlayerStatus(0x560, 0, false);
		}
	}

	// Get matrix from held object and compute facing angle
	MtxPtr heldMtx = mHeldObject->getTakingMtx();
	f32 negCol2z = -heldMtx[2][2];
	f32 negCol0z = -heldMtx[0][2];
	s16 angle = matan(negCol2z, negCol0z);
	mFaceAngle.y = angle;
	mModelFaceAngle = mFaceAngle.y;

	// Build next position
	JGeometry::TVec3<f32> nextPos;
	nextPos.x = mPosition.x;
	nextPos.y = mPosition.y;
	nextPos.z = mPosition.z;

	// Compute pull direction
	s16 pullAngle = mFaceAngle.y + 0x8000;
	s16 pullDiff = pullAngle - mIntendedYaw;

	f32 cosDirection = JMASCos(pullDiff);
	if (cosDirection < 0.0f)
		cosDirection = 0.0f;
	f32 sinDirection = JMASSin(pullDiff);
	cosDirection = cosDirection * mIntendedMag;
	sinDirection = sinDirection * mIntendedMag;

	f32 outSpeed;
	f32 outAccel;
	getCurrentPullParams(&outSpeed, &outAccel);

	// Compute movement from trig tables
	s16 pullAngleU = (u16)pullAngle;
	f32 cosAngle = JMASCos(pullAngleU);
	f32 sinAngle = JMASSin(pullAngleU);

	nextPos.x = nextPos.x + (outAccel * (cosDirection * sinAngle) - outSpeed * (sinDirection * cosAngle));
	nextPos.z = nextPos.z + (outAccel * (cosDirection * cosAngle) + outSpeed * (sinDirection * sinAngle));

	// Check if held object accepts the position
	u32 moveResult = mHeldObject->receiveMessage(this, 0);
	// vtable call at offset 0xAC
	if (moveResult == 1) {
		mPosition = nextPos;
	}

	stopProcess();

	f32 animSpeed = 1.0f;
	if (mAction == 0x40561) {
		animSpeed = 5.0f;
	}

	u16 currentAnim = *(u16*)((u8*)this + 0xFA);
	if (currentAnim == 0xf2 || currentAnim == 0xea) {
		if (isLast1AnimeFrame()) {
			setAnimation(0xeb, 1.0f);
		}
		return;
	}

	// Check actor type for pull direction
	TTakeActor* heldObj = mHeldObject;
	u8 isTree;
	if (heldObj->mActorType - 0x08000000 == 0x6) {
		isTree = 1;
	} else {
		isTree = 0;
	}
	if (!isTree) {
		if (heldObj->mActorType - 0x08000000 == 0x8) {
			isTree = 1;
		} else {
			isTree = 0;
		}
	}

	JGeometry::TVec3<f32> diff;
	if (isTree) {
		diff = nextPos;
		diff.sub(unk29C);
	} else {
		diff.x = mPosition.x;
		diff.y = mPosition.y;
		diff.z = mPosition.z;
		diff.sub(unk29C);
	}

	// Compute distance using Newton-Raphson sqrt
	JGeometry::TVec3<f32> dir = diff;
	f32 distSq = dir.y * dir.y + dir.x * dir.x + dir.z * dir.z;
	f32 dist;
	if (distSq <= 0.0f) {
		dist = distSq;
	} else {
		f32 guess = __frsqrte(distSq);
		guess = (f32)guess;
		dist = distSq * (0.5f * guess * (3.0f - distSq * guess * guess));
	}

	if (dist < 1.0f) {
		setAnimation(0xeb, animSpeed);
		return;
	}

	// Determine pull animation based on angle difference
	f32 pullZ = diff.z;
	f32 pullX = diff.x;
	s16 pullDir = matan(pullZ, pullX);
	s16 faceDiff = pullDir - mFaceAngle.y;

	if (faceDiff >= -0x2000 && faceDiff <= 0x2000) {
		setAnimation(0xef, animSpeed);
	}
	if (faceDiff <= -0x6000 || faceDiff >= 0x6000) {
		setAnimation(0xec, animSpeed);
	}
	if (faceDiff > -0x6000 && faceDiff < -0x2000) {
		setAnimation(0xed, animSpeed);
	}
	if (faceDiff > 0x2000 && faceDiff < 0x6000) {
		setAnimation(0xee, animSpeed);
	}
}

void TMario::getCurrentPullParams(f32* outSpeed, f32* outAccel)
{
	u32 actorType = mHeldObject->mActorType;
	f32* params = 0;

	switch (actorType) {
	case 0x08000008:
		params = (f32*)((u8*)this + 0x147c);
		break;
	case 0x08000007:
		break;
	case 0x08000006:
		params = (f32*)((u8*)this + 0x14d4);
		break;
	case 0x0800000d:
		params = (f32*)((u8*)this + 0x152c);
		break;
	case 0x10000028:
		params = (f32*)((u8*)this + 0x1584);
		break;
	}

	if (params == NULL)
		return;

	if (mAction == 0x560) {
		*outSpeed = *(f32*)((u8*)params + 0x18);
		*outAccel = *(f32*)((u8*)params + 0x2c);
	} else {
		*outSpeed = *(f32*)((u8*)params + 0x40);
		*outAccel = *(f32*)((u8*)params + 0x54);
	}
}

void TMario::wireRolling()
{
	s16 savedAngle = mFaceAngle.x;

	JGeometry::TVec3<f32> startPos = mWireStartPos;
	JGeometry::TVec3<f32> diff = mWireEndPos;
	diff.sub(startPos);

	JGeometry::TVec3<f32> dir = diff;
	f32 ratio = mWirePosRatio;

	JGeometry::TVec3<f32> scaled = dir;
	scaled.scale(ratio);

	JGeometry::TVec3<f32> wirePos = startPos;
	wirePos.add(scaled);

	mPosition.x = wirePos.x;
	mPosition.y = wirePos.y;
	mPosition.z = wirePos.z;
	mPosition.y = mPosition.y - 160.0f;

	Mtx mtxX;
	J3DGetTranslateRotateMtx(mFaceAngle.x, 0, 0, 0.0f, 0.0f, 0.0f, mtxX);
	Mtx mtxY;
	J3DGetTranslateRotateMtx(0, mFaceAngle.y, 0, 0.0f, 0.0f, 0.0f, mtxY);
	Mtx mtxResult;
	PSMTXConcat(mtxY, mtxX, mtxResult);

	Vec sagOffset;
	sagOffset.x = 0.0f;
	f32 negSag = -mWireSag;
	sagOffset.y = negSag * 1.0f;
	sagOffset.z = 0.0f;
	Vec sagResult;
	PSMTXMultVec(mtxResult, &sagOffset, &sagResult);

	mPosition.x = mPosition.x + sagResult.x;
	mPosition.y = mPosition.y + sagResult.y;
	mPosition.z = mPosition.z + sagResult.z;

	s16 wireAngle = matan(dir.z, dir.x);

	if (mInput & 0x2) {
		mActionState |= 1;
	}

	u16 actionState = mActionState;
	s16 maxRotSpeed = mWireParams.mRotSpeedMax.get();
	u8 bit0 = actionState & 1;

	if (bit0 && (s16)unkF6 < 0 && mFaceAngle.x > -8192
	    && mFaceAngle.x <= maxRotSpeed - 8192) {
		changePlayerStatus(0x893, 0, false);
		return;
	}

	if (bit0 && (s16)unkF6 > 0 && (24576 - maxRotSpeed) <= mFaceAngle.x
	    && mFaceAngle.x < 24576) {
		changePlayerStatus(0x893, 1, false);
		return;
	}

	if (mInput & 0x1) {
		s16 angleDiff = (s16)wireAngle - mIntendedYaw;
		s16 diff16 = angleDiff;
		if (diff16 > -8192 && diff16 < 13653) {
			wireMove(3.0f);
		}
		if (diff16 < -24576 || diff16 > 19114) {
			wireMove(-3.0f);
		}
	}

	if (unk380 == 0) {
		TWaterGun* waterGun = mWaterGun;
		if (waterGun != NULL) {
			u8 emitting;
			if (waterGun->mCurrentWater == 0) {
				emitting = 0;
			} else {
				s32 kind = waterGun->getCurrentNozzle()->getNozzleKind();
				if (kind == 1) {
					TNozzleTrigger* triggerNozzle
					    = (TNozzleTrigger*)waterGun->getCurrentNozzle();
					if (triggerNozzle->unk385 == 1) {
						emitting = 1;
					} else {
						emitting = 0;
					}
				} else if (waterGun->getCurrentNozzle()->unk378 > 0.0f) {
					emitting = 1;
				} else {
					emitting = 0;
				}
			}
			if (emitting) {
				s16 rotAccel;
				if (mWaterGun == NULL) {
					rotAccel = 0;
				} else {
					switch (mWaterGun->mCurrentNozzle) {
					case 1:
						rotAccel = mWireParams.mRotSpeedTrgRocket.get();
						break;
					case 4:
						rotAccel = mWireParams.mRotSpeedTrgHover.get();
						break;
					case 5:
						rotAccel = mWireParams.mRotSpeedTrgTurbo.get();
						break;
					default:
						rotAccel = mWireParams.mRotSpeed.get();
						break;
					}
				}
				unkF6 = unkF6 + rotAccel;
			}
		}
	}

	s16 gravity = mWireParams.mRotGravity.get();
	unkF6 = unkF6 - (s16)(JMASSin(mFaceAngle.x) * (f32)gravity);

	u8 hasBrake;
	if (mInput & 0x4000) {
		hasBrake = 1;
	} else {
		hasBrake = 0;
	}
	if (hasBrake) {
		unkF6 = (s16)((f32)(s16)unkF6 * mWireParams.mRotBrake.get());
	}

	if ((s16)unkF6 < -maxRotSpeed) {
		unkF6 = -maxRotSpeed;
	}
	if ((s16)unkF6 > maxRotSpeed) {
		unkF6 = maxRotSpeed;
	}

	mFaceAngle.x = mFaceAngle.x + (s16)unkF6;
	mWireSag = 0.1f * __fabsf((f32)(s16)unkF6);
	mWireBounceVel = 0.0f;

	if (savedAngle < -16384 && (s16)mFaceAngle.x > 16384) {
		mActionState |= 2;
	} else if (savedAngle >= (s16)mFaceAngle.x) {
		// do nothing
	} else {
		mActionState &= ~2;
	}

	u16 state = mActionState;
	s16 soundId = 0;
	u8 shouldPlay = 0;
	if (state & 2) {
		if ((s16)mFaceAngle.x < mWireSwingPosAngle
		    && mWireSwingPosAngle <= savedAngle) {
			mWireQueuedSfxID = 0x381E;
			soundId = 0x1817;
			shouldPlay = 1;
			mWireSfxTimer = mWireSfxDelay;
			if (!(mActionState & 0xC)) {
				if (MsRandF() < 0.5f) {
					mActionState |= 4;
				} else {
					mActionState |= 8;
				}
			}
			u16 sfxDir = mActionState & 0xC;
			if (sfxDir == 4) {
				startVoice(0x78C1);
			}
			if (sfxDir == 8) {
				startVoice(0x78C4);
			}
		}

		if ((s16)mFaceAngle.x < mWireSwingNegAngle
		    && mWireSwingNegAngle <= savedAngle) {
			soundId = 0x180F;
			shouldPlay = 1;
		}

		if ((s16)savedAngle > 0 && (s16)mFaceAngle.x <= 0) {
			mWireQueuedSfxID = 0x381E;
			mWireSfxTimer = mWireSfxDelay;
		}
	} else {
		if ((s16)mFaceAngle.x < mWireRollAngle
		    && mWireRollAngle <= savedAngle) {
			mWireQueuedSfxID = 0x381F;
			soundId = 0x1815;
			shouldPlay = 1;
			mWireSfxTimer = mWireSfxDelay;
		}

		s16 rollAngle = mWireRollAngle;
		if (savedAngle < rollAngle
		    && rollAngle <= (s16)mFaceAngle.x) {
			mWireQueuedSfxID = 0x3820;
			soundId = 0x1816;
			shouldPlay = 1;
			mWireSfxTimer = mWireSfxDelay;
		}
	}

	if (shouldPlay == 1) {
		f32 sag = mWireSag;
		if (gpMSound->gateCheck(soundId)) {
			MSoundSESystem::MSoundSE::startSoundActorWithInfo(
			    soundId, (Vec*)&mPosition, 0, sag, 0, 0, 0, 0, 4);
		}
	}

	blurEffect();

	s16 rotStop = mWireParams.mRotStop.get();
	if ((s16)mFaceAngle.x > -512 && (s16)mFaceAngle.x < 512
	    && (s16)unkF6 > -rotStop && (s16)unkF6 < rotStop) {
		u8 canHang = 0;
		mFaceAngle.x = 0;
		unkF6 = 0;
		mWireBounceVel = 0.0f;
		mWireSag = 0.0f;
		if ((u32)mHeldObject == 0) {
			if (!onYoshi())
				canHang = 1;
		}
		if (canHang) {
			changePlayerStatus(0x10000357, 0, false);
			return;
		}
		changePlayerStatus(0x88c, 0, false);
		return;
	}

	mFaceAngle.y = wireAngle + 0x4000;
	mModelFaceAngle = mFaceAngle.y;
	setAnimation(0x33, 1.0f);
}

void TMario::wireHanging()
{
	JGeometry::TVec3<f32> startPos = mWireStartPos;
	JGeometry::TVec3<f32> diff = mWireEndPos;
	diff.sub(startPos);

	JGeometry::TVec3<f32> dir = diff;
	JGeometry::TVec3<f32> scaled = dir;
	scaled.scale(mWirePosRatio);

	JGeometry::TVec3<f32> wirePos = startPos;
	wirePos.add(scaled);

	mPosition = wirePos;
	mPosition.y = mPosition.y - 160.0f;

	Mtx mtxX;
	J3DGetTranslateRotateMtx(mFaceAngle.x, 0, 0, 0.0f, 0.0f, 0.0f, mtxX);
	Mtx mtxY;
	J3DGetTranslateRotateMtx(0, mFaceAngle.y, 0, 0.0f, 0.0f, 0.0f, mtxY);
	Mtx mtxResult;
	PSMTXConcat(mtxY, mtxX, mtxResult);

	Vec sagOffset;
	sagOffset.x = 0.0f;
	sagOffset.y = -mWireSag * 1.0f;
	sagOffset.z = 0.0f;
	Vec sagResult;
	PSMTXMultVec(mtxResult, &sagOffset, &sagResult);

	mPosition.x = mPosition.x + sagResult.x;
	mPosition.y = mPosition.y + sagResult.y;
	mPosition.z = mPosition.z + sagResult.z;

	s16 angle;
	angle = matan(dir.z, dir.x);

	if ((mInput & 0x2) && (mFloorPosition.x - mFloorPosition.y >= 160.0f)) {
		mWireBounceVel = 5.0f;
		changePlayerStatus(0x10000556, 0, false);
		return;
	}

	if (mInput & 0x8000) {
		mHolder->receiveMessage(this, 8);
		mHolder = 0;

		const TBGCheckData* plane;
		f32 zero = 0.0f;
		mVel.y = zero;
		mForwardVel = zero;

		mPosition.x -= 60.0f * JMASSin(mFaceAngle.y);
		mPosition.z -= 60.0f * JMASCos(mFaceAngle.y);

		f32 groundY = gpMap->checkGround(mPosition.x, mPosition.y, mPosition.z, &plane);

		f32 limit = mPosition.y - 100.0f;
		if (groundY < limit) {
			mPosition.y = limit;
		} else {
			mPosition.y = groundY;
		}

		changePlayerStatus(0x208ba, 0, false);
		return;
	}

	u8 canSpray = 0;
	unkF6 = 0;

	s16 wireAngle = (s16)angle;
	mFaceAngle.x = 0;
	mFaceAngle.y = wireAngle + 0x4000;
	mModelFaceAngle = angle;

	if (mInput & 0x1) {
		s16 angleDiff = (s16)(wireAngle - mIntendedYaw);

		if (angleDiff > -0x2000 && angleDiff < 0x3555) {
			int moveResult = wireMove(3.0f);
			if (moveResult != 0) {
				setAnimation(0xe5, 1.0f);
			} else {
				setAnimation(0xe3, 1.0f);
			}
		}

		if (angleDiff < -0x6000 || angleDiff > 0x4AAA) {
			int moveResult = wireMove(-3.0f);
			if (moveResult != 0) {
				setAnimation(0xe6, 1.0f);
			} else {
				setAnimation(0xe3, 1.0f);
			}
		}

		if (angleDiff >= -0x6000 && angleDiff <= -0x2000) {
			mWireBounceVel = 5.0f;
			changePlayerStatus(0x10000556, 0, false);
			return;
		}

		if (angleDiff >= 0x3555 && angleDiff <= 0x4AAA) {
			const TBGCheckData* plane;
			f32 zero = 0.0f;
			mVel.y = zero;
			mForwardVel = zero;

			mPosition.x -= 60.0f * JMASSin(mFaceAngle.y);
			mPosition.z -= 60.0f * JMASCos(mFaceAngle.y);

			f32 groundY = gpMap->checkGround(mPosition.x, mPosition.y, mPosition.z, &plane);

			f32 limit = mPosition.y - 100.0f;
			if (groundY < limit) {
				mPosition.y = limit;
			} else {
				mPosition.y = groundY;
			}

			changePlayerStatus(0x208ba, 0, false);
		}
	} else {
		if (unk380 == 0) {
			TWaterGun* waterGun = mWaterGun;
			if (waterGun != 0) {
				if (waterGun->mCurrentWater != 0) {
					s32 kind = waterGun->getCurrentNozzle()->getNozzleKind();
					if (kind == 1) {
						if (((TNozzleTrigger*)waterGun->getCurrentNozzle())->unk385 == 1) {
							canSpray = 1;
						}
					} else {
						if (waterGun->getCurrentNozzle()->unk378 > 0.0f) {
							canSpray = 1;
						}
					}
				}

				if (canSpray) {
					s16 rotSpeed;
					if (mWaterGun == 0) {
						rotSpeed = 0;
					} else {
						switch (mWaterGun->mCurrentNozzle) {
						case TWaterGun::Rocket:
							rotSpeed = mWireParams.mRotSpeedTrgRocket.get();
							break;
						case TWaterGun::Hover:
							rotSpeed = mWireParams.mRotSpeedTrgHover.get();
							break;
						case TWaterGun::Turbo:
							rotSpeed = mWireParams.mRotSpeedTrgTurbo.get();
							break;
						default:
							rotSpeed = mWireParams.mRotSpeed.get();
							break;
						}
					}

					if (rotSpeed > 0) {
						unkF6 = unkF6 + mWireParams.mRotStop.get() * 2;
					} else {
						unkF6 = unkF6 - mWireParams.mRotStop.get() * 2;
					}

					s16 rotSpeed2;
					if (mWaterGun == 0) {
						rotSpeed2 = 0;
					} else {
						switch (mWaterGun->mCurrentNozzle) {
						case TWaterGun::Rocket:
							rotSpeed2 = mWireParams.mRotSpeedTrgRocket.get();
							break;
						case TWaterGun::Hover:
							rotSpeed2 = mWireParams.mRotSpeedTrgHover.get();
							break;
						case TWaterGun::Turbo:
							rotSpeed2 = mWireParams.mRotSpeedTrgTurbo.get();
							break;
						default:
							rotSpeed2 = mWireParams.mRotSpeed.get();
							break;
						}
					}

					unkF6 = unkF6 + rotSpeed2;
					changePlayerStatus(0x10000358, 0, false);
					return;
				}
			}
		}

		setAnimation(0xe3, 1.0f);
	}
}

void TMario::changeWireHanging()
{
	u8 canHang = 0;
	if (mHeldObject == 0) {
		if (!onYoshi())
			canHang = 1;
	}
	if (canHang) {
		changePlayerStatus(0x10000357, 0, false);
	} else {
		changePlayerStatus(0x88c, 0, false);
	}
}

void TMario::wireWait()
{
	s16 angle;
	// getOnWirePosAngle inlined
	{
		JGeometry::TVec3<f32> startPos = mWireStartPos;
		JGeometry::TVec3<f32> diff = mWireEndPos;
		diff.sub(startPos);

		JGeometry::TVec3<f32> dir = diff;
		JGeometry::TVec3<f32> scaled = dir;
		scaled.scale(mWirePosRatio);

		JGeometry::TVec3<f32> wirePos = startPos;
		wirePos.add(scaled);

		mPosition = wirePos;
		mPosition.y = mPosition.y - 160.0f;

		Mtx mtxX;
		J3DGetTranslateRotateMtx(mFaceAngle.x, 0, 0, 0.0f, 0.0f, 0.0f, mtxX);
		Mtx mtxY;
		J3DGetTranslateRotateMtx(0, mFaceAngle.y, 0, 0.0f, 0.0f, 0.0f, mtxY);
		Mtx mtxResult;
		PSMTXConcat(mtxY, mtxX, mtxResult);

		Vec sagOffset;
		sagOffset.x = 0.0f;
		sagOffset.y = -mWireSag * 1.0f;
		sagOffset.z = 0.0f;
		Vec sagResult;
		PSMTXMultVec(mtxResult, &sagOffset, &sagResult);

		mPosition.x = mPosition.x + sagResult.x;
		mPosition.y = mPosition.y + sagResult.y;
		mPosition.z = mPosition.z + sagResult.z;

		angle = matan(dir.z, dir.x);
	}

	if (mInput & 0x2) {
		unk118 |= 0x100;
	}

	u8 onWire;
	if (unk118 & 0x100) {
		onWire = 1;
	} else {
		onWire = 0;
	}

	if (onWire && mWireSag <= 0.0f) {
		mHolder->receiveMessage(this, 8);
		mHolder = 0;
		changePlayerStatus(0x892, 0, false);
		setPlayerVelocity(0.0f);
		if (mWireBounceVel < 0.0f) {
			f32 factor = mWireParams.mWireJumpMult.value;
			mVel.y = -(mWireBounceVel * factor - mVel.y);
		}
		mVel.y = mVel.y + mWireParams.mWireJumpBase.value;

		u32 soundId;
		if (mWireBounceVel < *(f32*)((u8*)this + 0x544)) {
			soundId = 0x1812;
		} else if (mWireBounceVel < *(f32*)((u8*)this + 0x540)) {
			soundId = 0x1811;
		} else {
			soundId = 0x1810;
		}

		f32 wireSag = mWireSag;
		if (gpMSound->gateCheck(soundId)) {
			MSoundSESystem::MSoundSE::startSoundActorWithInfo(
			    soundId, (Vec*)&mPosition, 0, wireSag, 0, 0, 0, 0, 4);
		}

		unk78 &= ~0x100;
		return;
	}

	if (mInput & 0x8000) {
		mWireBounceVel = 5.0f;
		changePlayerStatus(0x10000554, 0, false);
		return;
	}

	s16 angleDiff;
	if (mInput & 0x1) {
		angleDiff = (s16)angle - mIntendedYaw;
		if (angleDiff > -0x2000 && angleDiff < 0x2000) {
			f32 animSpeed = 0.05f * mIntendedMag;
			int moveResult = wireMove(0.1f * mIntendedMag);
			if (moveResult != 0) {
				if (mIntendedMag > 16.0f) {
					setAnimation(0xdc, animSpeed);
				} else {
					setAnimation(0xdb, animSpeed);
				}
			} else {
				setAnimation(0xdd, 1.0f);
			}
		}

		if (angleDiff < -0x2000) {
			startVoice(0x78e0);
			changePlayerStatus(0x352, 0, false);
			return;
		}

		if (angleDiff > 0x2000) {
			startVoice(0x78e0);
			changePlayerStatus(0x353, 0, false);
			return;
		}
	} else {
		setAnimation(0xdd, 1.0f);
	}

	mFaceAngle.y = angle;
	mModelFaceAngle = mFaceAngle.y;
}

void TMario::wireSWait()
{
	s16 angle;
	// getOnWirePosAngle inlined
	{
		JGeometry::TVec3<f32> startPos = mWireStartPos;
		JGeometry::TVec3<f32> diff = mWireEndPos;
		diff.sub(startPos);

		JGeometry::TVec3<f32> dir = diff;
		JGeometry::TVec3<f32> scaled = dir;
		scaled.scale(mWirePosRatio);

		JGeometry::TVec3<f32> wirePos = startPos;
		wirePos.add(scaled);

		mPosition = wirePos;
		mPosition.y = mPosition.y - 160.0f;

		Mtx mtxX;
		J3DGetTranslateRotateMtx(mFaceAngle.x, 0, 0, 0.0f, 0.0f, 0.0f, mtxX);
		Mtx mtxY;
		J3DGetTranslateRotateMtx(0, mFaceAngle.y, 0, 0.0f, 0.0f, 0.0f, mtxY);
		Mtx mtxResult;
		PSMTXConcat(mtxY, mtxX, mtxResult);

		Vec sagOffset;
		sagOffset.x = 0.0f;
		sagOffset.y = -mWireSag * 1.0f;
		sagOffset.z = 0.0f;
		Vec sagResult;
		PSMTXMultVec(mtxResult, &sagOffset, &sagResult);

		mPosition.x = mPosition.x + sagResult.x;
		mPosition.y = mPosition.y + sagResult.y;
		mPosition.z = mPosition.z + sagResult.z;

		angle = matan(dir.z, dir.x);
	}

	if (mInput & 0x2) {
		unk118 |= 0x100;
	}

	u8 onWire;
	if (unk118 & 0x100) {
		onWire = 1;
	} else {
		onWire = 0;
	}

	if (onWire && mWireSag < 0.0f) {
		mHolder->receiveMessage(this, 8);
		mHolder = 0;
		changePlayerStatus(0x892, 0, false);
		setPlayerVelocity(0.0f);
		if (mWireBounceVel < 0.0f) {
			mVel.y = -(5.0f * mWireBounceVel - mVel.y);
		}
		return;
	}

	if (mInput & 0x8000) {
		mWireBounceVel = 5.0f;
		startVoice(0x78e0);
		changePlayerStatus(0x10000554, 0, false);
		return;
	}

	if (mInput & 0x1) {
		s16 diff = (s16)angle - mIntendedYaw;
		if (diff > -0x4000 && diff < 0x4000) {
			changePlayerStatus(0x35c, 0, false);
			return;
		}

		JGeometry::TVec3<f32> temp = mWireStartPos;
		mWireStartPos = mWireEndPos;
		mWireEndPos = temp;
		mWirePosRatio = 1.0f - mWirePosRatio;
		changePlayerStatus(0x35b, 0, false);
		return;
	}

	setAnimation(0xe0, 1.0f);
	mModelFaceAngle = angle;
	mFaceAngle.y = mModelFaceAngle + 0x4000;
}

int TMario::wireMove(f32 rate)
{
	JGeometry::TVec3<f32> startPos = mWireStartPos;
	JGeometry::TVec3<f32> diff = mWireEndPos;
	diff.sub(startPos);

	JGeometry::TVec3<f32> dir = diff;

	f32 lenSq = dir.squared();
	if (lenSq > 0.0f) {
		lenSq = JGeometry::TUtil<f32>::sqrt(lenSq);
	}

	f32 step = rate / lenSq;
	f32 ratio = mWirePosRatio;
	s32 ok = 1;
	f32 upperLimit = 1.0f - 100.0f / lenSq;
	if (ratio + step > upperLimit) {
		mWirePosRatio = upperLimit;
		ok = 0;
	}
	f32 newRatio = mWirePosRatio;
	if (newRatio + step < 100.0f / lenSq) {
		mWirePosRatio = 100.0f / lenSq;
		ok = 0;
	}
	if (ok == 1) {
		mWirePosRatio = mWirePosRatio + step;
	}
	return ok;
}

void TMario::getOnWirePosAngle(JGeometry::TVec3<f32>* outPos, short* outAngle)
{
	JGeometry::TVec3<f32> startPos = mWireStartPos;
	JGeometry::TVec3<f32> diff = mWireEndPos;
	diff.sub(startPos);

	JGeometry::TVec3<f32> dir = diff;
	JGeometry::TVec3<f32> scaled = dir;
	scaled.scale(mWirePosRatio);

	JGeometry::TVec3<f32> wirePos = startPos;
	wirePos.add(scaled);

	*outPos = wirePos;
	outPos->y = outPos->y - 160.0f;

	Mtx mtxX;
	J3DGetTranslateRotateMtx(mFaceAngle.x, 0, 0, 0.0f, 0.0f, 0.0f, mtxX);
	Mtx mtxY;
	J3DGetTranslateRotateMtx(0, mFaceAngle.y, 0, 0.0f, 0.0f, 0.0f, mtxY);
	Mtx mtxResult;
	PSMTXConcat(mtxY, mtxX, mtxResult);

	Vec sagOffset;
	sagOffset.x = 0.0f;
	sagOffset.y = -mWireSag * 1.0f;
	sagOffset.z = 0.0f;
	Vec sagResult;
	PSMTXMultVec(mtxResult, &sagOffset, &sagResult);

	outPos->x = outPos->x + sagResult.x;
	outPos->y = outPos->y + sagResult.y;
	outPos->z = outPos->z + sagResult.z;

	*outAngle = matan(dir.z, dir.x);
}

void TMario::hanging()
{
	s32 movedToWall = 0;
	const TBGCheckData* plane;

	f32 heightDiff = mFloorPosition.x - mFloorPosition.y;
	u16 timer = mActionTimer;
	s16 faceY = mFaceAngle.y;
	s16 intYaw = mIntendedYaw;
	s16 stickAngleDiff = intYaw - faceY;
	u8 highEnough = (heightDiff >= 160.0f);

	if (timer == 0) {
		unkF6 = 0;
	}

	s16* hangTimerParam = (s16*)((u8*)this + 0x12EC);
	if (mActionTimer < *hangTimerParam) {
		mActionTimer = mActionTimer + 1;
	}

	u32 input = mInput;
	s32 shouldDrop = 0;

	if (input & 0x8004) {
		shouldDrop = 1;
	} else {
		u16 groundType = mGroundPlane->mBGType;
		u8 isType1;
		if (groundType == 0x0001 || groundType == 0x4001 || groundType == 0x8001 || groundType == 0xC001) {
			isType1 = 1;
		} else {
			isType1 = 0;
		}
		if (isType1) {
			shouldDrop = 1;
		} else {
			u8 isType6;
			if (groundType == 0x0006 || groundType == 0x4006 || groundType == 0x8006 || groundType == 0xC006) {
				isType6 = 1;
			} else {
				isType6 = 0;
			}
			if (isType6) {
				shouldDrop = 1;
			}
		}
	}

	if (shouldDrop) {
		mInput = input & ~0x4;
		mInput = mInput & ~0x8000;
		mVel.y = 0.0f;
		mForwardVel = 0.0f;
		mPosition.x -= 60.0f * JMASSin(mFaceAngle.y);
		mPosition.z -= 60.0f * JMASCos(mFaceAngle.y);
		f32 groundY = gpMap->checkGround(mPosition.x, mPosition.y, mPosition.z, &plane);
		f32 limit = mPosition.y - 100.0f;
		if (groundY < limit) {
			mPosition.y = limit;
		} else {
			mPosition.y = groundY;
		}
		changePlayerStatus(0x8A7, 0, false);
		return;
	}

	if ((input & 0x2) && highEnough) {
		startVoice(0x788F);
		changePlayerStatus(0x3000054F, 0, false);
		return;
	}

	TBGWallCheckRecord wallCheck1;
	wallCheck1.mCenter.x = mPosition.x;
	wallCheck1.mCenter.y = mPosition.y - 10.0f;
	wallCheck1.mCenter.z = mPosition.z;
	wallCheck1.mRadius = 30.0f;
	wallCheck1.mMaxResults = 4;
	wallCheck1.mFlags = 0;
	gpMap->isTouchedWallsAndMoveXZ(&wallCheck1);

	const TBGCheckData* bestWall = (const TBGCheckData*)0;
	f32 bestDist = 50.0f;
	s32 loopIdx = 0;
	s32 arrayOff = 0;
	for (; loopIdx < wallCheck1.mResultWallsNum; loopIdx++, arrayOff += 4) {
		TBGCheckData* wall = wallCheck1.mResultWalls[loopIdx];
		s32 wallAngle = matan(wall->mNormal.z, wall->mNormal.x);
		s16 diff = wallAngle - (mFaceAngle.y + 0x8000);
		if (diff > -0x2000 && diff < 0x2000) {
			JGeometry::TVec3<f32> pos;
			pos.x = mPosition.x;
			pos.y = mPosition.y;
			pos.z = mPosition.z;
			f32 planeDist = wall->mNormal.y * pos.y + wall->mNormal.x * pos.x + wall->mNormal.z * pos.z + wall->mPlaneDistance;
			if (planeDist < bestDist) {
				bestWall = wall;
			}
		}
	}

	if (bestWall == (const TBGCheckData*)0) {
		changePlayerStatus(0x88C, 0, false);
	}

	if (bestWall != (const TBGCheckData*)0) {
		u8 isFence;
		if (bestWall->mBGType == 0x10A) {
			isFence = 1;
		} else {
			isFence = 0;
		}
		if (isFence) {
			mPosition.x = mPosition.x - 40.0f * JMASSin(mFaceAngle.y);
			mPosition.y = mPosition.y - 160.0f;
			mPosition.z = mPosition.z - 40.0f * JMASCos(mFaceAngle.y);
			changePlayerStatus(0x3000036B, 0, false);
			return;
		}
	}

	TBGWallCheckRecord wallCheck2;
	wallCheck2.mCenter.x = mPosition.x;
	wallCheck2.mCenter.y = 10.0f + mPosition.y;
	wallCheck2.mCenter.z = mPosition.z;
	wallCheck2.mRadius = 50.0f;
	wallCheck2.mMaxResults = 1;
	wallCheck2.mFlags = 0;
	gpMap->isTouchedWallsAndMoveXZ(&wallCheck2);

	mPosition.x = wallCheck2.mCenter.x;
	mPosition.y = wallCheck2.mCenter.y;
	mPosition.z = wallCheck2.mCenter.z;

	if (mActionTimer >= 40 && (mInput & 0x1)) {
		s16 stickDiffExt = (s16)stickAngleDiff;
		if (stickDiffExt >= -1024 && stickDiffExt <= 1024) {
			if (highEnough) {
				setAnimation(0, 1.0f);
				changePlayerStatus(0x3000054C, 0, false);
				return;
			}
		}

		s16 stickDiffExt2 = (s16)stickAngleDiff;
		if (stickDiffExt2 > -29127 && stickDiffExt2 < 29127) {
			if (mActionTimer >= mHangingParams.mRapidTime.value && wallCheck1.mResultWallsNum > 0) {
				JGeometry::TVec3<f32> targetPos;
				targetPos.x = mPosition.x;
				targetPos.y = mPosition.y;
				targetPos.z = mPosition.z;

				f32 moveSpeed = mHangingParams.mMoveSp.value;
				if (stickDiffExt2 > 1024 && stickDiffExt2 < 29127) {
					f32 mag = mIntendedMag;
					targetPos.x = mPosition.x - moveSpeed * (mag * bestWall->mNormal.z);
					targetPos.y = mPosition.y;
					targetPos.z = mPosition.z + moveSpeed * (mag * bestWall->mNormal.x);
				}
				if ((s16)stickAngleDiff > -29127 && (s16)stickAngleDiff < -1024) {
					f32 mag = mIntendedMag;
					targetPos.x = mPosition.x + moveSpeed * (mag * bestWall->mNormal.z);
					targetPos.y = mPosition.y;
					targetPos.z = mPosition.z - moveSpeed * (mag * bestWall->mNormal.x);
				}

				const TBGCheckData* bestWall3 = (const TBGCheckData*)0;
				TBGWallCheckRecord wallCheck3;
				wallCheck3.mCenter.x = targetPos.x;
				wallCheck3.mCenter.y = 10.0f + targetPos.y;
				wallCheck3.mCenter.z = targetPos.z;
				wallCheck3.mRadius = 50.0f;
				wallCheck3.mMaxResults = 1;
				wallCheck3.mFlags = 0;
				gpMap->isTouchedWallsAndMoveXZ(&wallCheck3);

				targetPos.x = wallCheck3.mCenter.x;
				targetPos.y = wallCheck3.mCenter.y;
				targetPos.z = wallCheck3.mCenter.z;

				f32 targetGroundY = gpMap->checkGround(targetPos.x, 10.0f + targetPos.y, targetPos.z, &plane);
				f32 f31Save = targetPos.z;

				f32 limit1 = mPosition.y - 100.0f;
				if (limit1 < targetGroundY && targetGroundY < 50.0f + mPosition.y) {
					TBGWallCheckRecord wallCheck4;
					wallCheck4.mCenter.x = targetPos.x - 20.0f * JMASSin(mFaceAngle.y);
					wallCheck4.mCenter.y = targetGroundY - 20.0f;
					wallCheck4.mCenter.z = f31Save - 20.0f * JMASCos(mFaceAngle.y);
					wallCheck4.mRadius = 30.0f;
					wallCheck4.mMaxResults = 4;
					wallCheck4.mFlags = 0;
					gpMap->isTouchedWallsAndMoveXZ(&wallCheck4);

					bestDist = 50.0f;
					s32 loopIdx2 = 0;
					for (; loopIdx2 < wallCheck4.mResultWallsNum; loopIdx2++) {
						TBGCheckData* wall = wallCheck4.mResultWalls[loopIdx2];
						s32 wallAngle = matan(wall->mNormal.z, wall->mNormal.x);
						s16 diff = wallAngle - (mFaceAngle.y + 0x8000);
						if (diff > -0x2000 && diff < 0x2000) {
							JGeometry::TVec3<f32> pos;
							pos.x = mPosition.x;
							pos.y = mPosition.y;
							pos.z = mPosition.z;
							f32 planeDist = wall->mNormal.y * pos.y + wall->mNormal.x * pos.x + wall->mNormal.z * pos.z + wall->mPlaneDistance;
							if (planeDist < bestDist) {
								bestWall3 = wall;
							}
						}
					}

					if (bestWall3 != (const TBGCheckData*)0) {
						f32 dot = bestWall->mNormal.y * bestWall3->mNormal.y
						        + bestWall->mNormal.x * bestWall3->mNormal.x
						        + bestWall->mNormal.z * bestWall3->mNormal.z;

						if (mDeParams.mHangWallMovableAngle.value < dot) {
							s32 newAngle = matan(bestWall3->mNormal.z, bestWall3->mNormal.x);
							mFaceAngle.y = (s16)(newAngle + 0x8000);

							mPosition.x = wallCheck3.mCenter.x - 40.0f * bestWall3->mNormal.x;
							mPosition.z = wallCheck3.mCenter.z - 40.0f * bestWall3->mNormal.z;

							f32 groundY = gpMap->checkGround(mPosition.x, 160.0f + mPosition.y, mPosition.z, &plane);
							mPosition.y = groundY;
							movedToWall = 1;
						}
					}
				}
			}
		} else {
			mVel.y = 0.0f;
			mForwardVel = 0.0f;
			mPosition.x -= 60.0f * JMASSin(mFaceAngle.y);
			mPosition.z -= 60.0f * JMASCos(mFaceAngle.y);
			f32 groundY = gpMap->checkGround(mPosition.x, mPosition.y, mPosition.z, &plane);
			f32 limit = mPosition.y - 100.0f;
			if (groundY < limit) {
				mPosition.y = limit;
			} else {
				mPosition.y = groundY;
			}
			changePlayerStatus(0x208B6, 0, false);
			return;
		}
	}

	if (mActionTimer >= *hangTimerParam) {
		mVel.y = 0.0f;
		mForwardVel = 0.0f;
		mPosition.x -= 60.0f * JMASSin(mFaceAngle.y);
		mPosition.z -= 60.0f * JMASCos(mFaceAngle.y);
		f32 groundY = gpMap->checkGround(mPosition.x, mPosition.y, mPosition.z, &plane);
		f32 limit = mPosition.y - 100.0f;
		if (groundY < limit) {
			mPosition.y = limit;
		} else {
			mPosition.y = groundY;
		}
		changePlayerStatus(0x208B6, 0, false);
		return;
	}

	checkPlayerAround(-0x8000, 30.0f);
	f32 heightAboveGround = mPosition.y - mPosition.y;

	if (highEnough && heightAboveGround < 100.0f) {
		startVoice(0x788F);
		changePlayerStatus(0x3000054F, 0, false);
		return;
	}

	setPlayerVelocity(0.0f);
	f32 zero = 0.0f;
	mVel.y = zero;
	mPosition.y = mFloorPosition.y;
	mModelFaceAngle = mFaceAngle.y;

	if (movedToWall != 1) {
		f32 dz = mPosition.z - unk29C.z;
		f32 dx = mPosition.x - unk29C.x;
		f32 distSq = dz * dz + dx * dx;
		f32 dist;
		if (distSq <= zero) {
			dist = distSq;
		} else {
			f32 guess = __frsqrte(distSq);
			dist = distSq * (0.5f * guess * (3.0f - distSq * guess * guess));
		}
		f32 animSpeed = dist * mHangingParams.mAnmRate.value;
		if ((s16)stickAngleDiff < 0) {
			setAnimation(0xD7, animSpeed);
		} else {
			setAnimation(0xD8, animSpeed);
		}
	} else {
		if (mActionTimer < mHangingParams.mRapidTime.value) {
			setAnimation(0x33, 1.0f);
		} else {
			setAnimation(0x33, mHangingParams.mAnmRapid.value);
		}
	}
}


void TMario::startHangLanding(u32 status)
{
	const TBGCheckData* plane;
	mVel.y = 0.0f;
	mForwardVel = 0.0f;
	mPosition.x = mPosition.x - 60.0f * JMASSin(mFaceAngle.y);
	mPosition.z = mPosition.z - 60.0f * JMASCos(mFaceAngle.y);
	f32 groundY = gpMap->checkGround(mPosition.x, mPosition.y, mPosition.z, &plane);
	f32 limit = mPosition.y - 100.0f;
	if (groundY < limit) {
		mPosition.y = limit;
	} else {
		mPosition.y = groundY;
	}
	changePlayerStatus(status, 0, false);
}

void TMario::moveRoof()
{
	// roofCommonEvents inlined
	{
		u32 input = mInput;
		if (input & 0x8000) {
			mInput = input & ~0x8000;
			changePlayerStatus(0x88c, 0, false);
		} else if (input & 0x2) {
			TLiveActor* actor = (TLiveActor*)mRoofPlane->mActor;
			if (actor != 0) {
				actor->receiveMessage(this, 3);
				if (actor->mActorType == 0x4000006a) {
					emitParticle(0x39, &unk160[1]);
					rumbleStart(0x15, mMotorParams.mMotorWall.value);
					changePlayerStatus(0x00200345, 0, false);
					return;
				}
			}
			changePlayerStatus(0x00200347, 0, false);
		} else {
			goto afterCommon;
		}
		return;
	}
afterCommon:

	if (mInput & 1) {
		int result = doRoofMovingProcess();
		if (result == 2) {
			changePlayerStatus(0x88c, 0, false);
			return;
		} else if (result == 3) {
			changePlayerStatus(0x3000036b, 0, false);
		}
	}

	if (mInput & 0x20) {
		changePlayerStatus(0x00200349, mActionArg, false);
		return;
	}

	JGeometry::TVec3<f32> diff;
	diff = mPosition;
	diff.x -= unk29C.x;
	diff.y -= unk29C.y;
	diff.z -= unk29C.z;

	JGeometry::TVec3<f32> dir = diff;
	f32 dist = JGeometry::TUtil<f32>::sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);

	f32 speed = dist * mHangRoofParams.mAnmMult.value;
	if (mActionArg & 1) {
		setAnimation(0x5c, speed);
	} else {
		setAnimation(0x5d, speed);
	}

	if (isLast1AnimeFrame()) {
		mActionArg = 1 - mActionArg;
	}

	return;
}

BOOL TMario::roofCommonEvents()
{
	u32 input = mInput;
	if (input & 0x8000) {
		mInput = input & ~0x8000;
		changePlayerStatus(0x88c, 0, false);
	} else if (input & 0x2) {
		TLiveActor* actor = (TLiveActor*)mRoofPlane->mActor;
		if (actor != 0) {
			actor->receiveMessage(this, 3);
			if (actor->mActorType == 0x4000006a) {
				emitParticle(0x39, &unk160[1]);
				rumbleStart(0x15, mMotorParams.mMotorWall.value);
				return changePlayerStatus(0x00200345, 0, false);
			}
		}
		changePlayerStatus(0x00200347, 0, false);
	} else {
		return FALSE;
	}
}

int TMario::doRoofMovingProcess()
{
	f32 speed = mForwardVel + 1.0f;
	mForwardVel = speed;
	if (speed > 4.0f) {
		mForwardVel = 4.0f;
	}

	if (mIntendedMag > 0.0f) {
		s16 diff = mIntendedYaw - mFaceAngle.y;
		s16 turn = IConverge(diff, 0, 0x800, 0x800);
		mFaceAngle.y = mIntendedYaw - turn;
	}

	mFaceAngle.y; // access for codegen
	unk9E = mFaceAngle.y;

	mSlideVelX = mForwardVel * JMASSin(mFaceAngle.y);
	mSlideVelZ = mForwardVel * JMASCos(mFaceAngle.y);
	mVel.x = mSlideVelX;
	mVel.y = 0.0f;
	mVel.z = mSlideVelZ;

	f32 normalX = *(f32*)((u8*)mRoofPlane + 0x38);
	JGeometry::TVec3<f32> nextPos;
	nextPos.x = mPosition.x - mSlideVelX * normalX;
	nextPos.z = mPosition.z - mSlideVelZ * normalX;
	nextPos.y = mPosition.y;

	int result = hangingCheckRoof(&nextPos);
	if (result == 2) {
		return 0;
	}

	mPosition = nextPos;
	mModelFaceAngle = mFaceAngle.y;
	return result;
}

int TMario::hangingCheckRoof(JGeometry::TVec3<f32>* pos)
{
	const TBGCheckData* wall = checkWallPlane((Vec*)pos, 50.0f, 50.0f);
	if (wall != 0) {
		u8 isFence = (wall->mBGType == 0x10a) ? 1 : 0;
		if (isFence) {
			s16 angle = matan(wall->getNormal().z, wall->getNormal().x);
			angle = angle + 0x18000;
			mFaceAngle.y = angle;
			mModelFaceAngle = mFaceAngle.y;
			return 3;
		}
	}

	const TBGCheckData* groundPlane;
	const TBGCheckData* roofPlane;
	f32 groundY = gpMap->checkGround(pos->x, pos->y, pos->z, &groundPlane);
	f32 roofY = gpMap->checkRoof(pos->x, 80.0f + groundY, pos->z, &roofPlane);

	wall = checkWallPlane((Vec*)pos, 50.0f, 50.0f);
	if (wall == 0) {
		return 2;
	}

	u8 isFence2 = (wall->mBGType == 0x10a) ? 1 : 0;
	if (!isFence2) {
		return 2;
	}

	f32 gap = roofY - groundY;
	if (gap <= 10.0f) {
		return 1;
	}

	f32 headroom = roofY - (10.0f + pos->y);
	if (headroom < -30.0f) {
		return 1;
	}

	if (headroom > 30.0f) {
		return 2;
	}

	pos->y = mFloorPosition.x - 10.0f;
	mGroundPlane = groundPlane;
	mFloorPosition.y = roofY;
	mRoofPlane = roofPlane;
	mFloorPosition.x = groundY;
	return 0;
}

void TMario::barClimb()
{
	if ((u32)mHolder == 0) {
		changePlayerStatus(0x208b6, 0, false);
		return;
	}

	if (mInput & 0x2) {
		mPosition.x -= 200.0f * JMASSin(mFaceAngle.y);
		mPosition.z -= 200.0f * JMASCos(mFaceAngle.y);
		mFaceAngle.y = mFaceAngle.y + 0x8000;
		changePlayerStatus(0x02000886, 0, false);
		return;
	}

	mPosition.x = mHolder->mPosition.x;
	mPosition.y = mHolder->mPosition.y + mHolderHeightDiff;
	mPosition.z = mHolder->mPosition.z;

	f32 climbParam = *(f32*)((u8*)unk108 + 0x14);
	if (climbParam < 8.0f) {
		changePlayerStatus(0x18100340, 0, false);
		return;
	}

	mVel.y = 0.0f;
	f32 climbRate = mBarParams.mClimbSp.value;
	mPosition.y = mPosition.y + climbParam * climbRate;
	mHolderHeightDiff = mPosition.y - mHolder->mPosition.y;

	if (mPosition.y > mHolder->mPosition.y + *(f32*)((u8*)mHolder + 0x5c)) {
		setPlayerVelocity(0.0f);
		changePlayerStatus(0x02000880, 0, false);
		unk78 &= ~0x100;
	}

	int barResult = barProcess();
	if (barResult == 0) {
		f32 animSpeed = *(f32*)((u8*)unk108 + 0x14) * mBarParams.mClimbAnmRate.value + 1.0f;
		setAnimation(5, animSpeed);
	}

	if (mHolder->mActorType - 0x40000000 == 0x39) {
		if (mHolderHeightDiff > 500.0f) {
			mHolderHeightDiff = 500.0f;
			mPosition.y = mHolder->mPosition.y + mHolderHeightDiff;
		}
	}

	if (mHolder->mActorType - 0x40000000 == 0x246) {
		u8 mapId = gpMarDirector->mMap;
		if (mapId == 8) {
			if (mHolderHeightDiff > 750.0f) {
				mHolderHeightDiff = 750.0f;
				mPosition.y = mHolder->mPosition.y + mHolderHeightDiff;
			}
		} else if (mapId == 9) {
			if (mHolderHeightDiff > 750.0f) {
				mHolderHeightDiff = 750.0f;
				mPosition.y = mHolder->mPosition.y + mHolderHeightDiff;
			}
		} else if (mapId == 4) {
			if (mHolderHeightDiff > 2500.0f) {
				mHolderHeightDiff = 2500.0f;
				mPosition.y = mHolder->mPosition.y + mHolderHeightDiff;
			}
		}
	}
}

void TMario::barWait()
{
	if ((u32)mHolder == 0) {
		changePlayerStatus(0x208b6, 0, false);
		return;
	}

	if (mInput & 0x2) {
		mPosition.x -= 200.0f * JMASSin(mFaceAngle.y);
		mPosition.z -= 200.0f * JMASCos(mFaceAngle.y);
		mFaceAngle.y = mFaceAngle.y + 0x8000;
		changePlayerStatus(0x02000886, 0, false);
		return;
	}

	mPosition.x = mHolder->mPosition.x;
	mPosition.y = mHolder->mPosition.y + mHolderHeightDiff;
	mPosition.z = mHolder->mPosition.z;

	if ((mInput & 0x8000) || mHolderHeightDiff <= 100.0f) {
		setPlayerVelocity(-2.0f);
		mPosition.x -= 200.0f * JMASSin(mFaceAngle.y);
		mPosition.z -= 200.0f * JMASCos(mFaceAngle.y);
		changePlayerStatus(0x208b6, 0, false);
		return;
	}

	f32 climbParam = *(f32*)((u8*)unk108 + 0x14);
	if (climbParam > 16.0f) {
		changePlayerStatus(0x10100343, 0, false);
		return;
	}

	if (climbParam < -16.0f) {
		mVel.y = (1.0f / 512.0f) * climbParam + mVel.y;
		mPosition.y = mPosition.y + mVel.y;
		mHolderHeightDiff = mPosition.y - mHolder->mPosition.y;
		treeSlipEffect();
		if (gpMSound->gateCheck(0x1140)) {
			MSoundSESystem::MSoundSE::startSoundActor(
			    0x1140, (Vec*)&mPosition, 0, (JAISound**)0, 0, 4);
		}
	}

	if (mHolder->mActorType - 0x40000000 == 0xBB) {
		if (*(f32*)((u8*)unk108 + 0x14) <= 0.0f) {
			mPosition.y -= 2.0f;
			mHolderHeightDiff = mPosition.y - mHolder->mPosition.y;
		}
		if (mPosition.y < mHolder->mPosition.y + 200.0f) {
			mPosition.y = mHolder->mPosition.y + 200.0f;
			mHolderHeightDiff = mPosition.y - mHolder->mPosition.y;
		}
	}

	if (mHolder->mActorType - 0x40000000 == 0x39) {
		if (mHolderHeightDiff > 500.0f) {
			mHolderHeightDiff = 500.0f;
			mPosition.y = mHolder->mPosition.y + mHolderHeightDiff;
		}
	}

	if (mHolder->mActorType - 0x40000000 == 0x246) {
		u8 mapId = gpMarDirector->mMap;
		if (mapId == 8) {
			if (mHolderHeightDiff > 750.0f) {
				mHolderHeightDiff = 750.0f;
				mPosition.y = mHolder->mPosition.y + mHolderHeightDiff;
			}
		} else if (mapId == 9) {
			if (mHolderHeightDiff > 750.0f) {
				mHolderHeightDiff = 750.0f;
				mPosition.y = mHolder->mPosition.y + mHolderHeightDiff;
			}
		} else if (mapId == 4) {
			if (mHolderHeightDiff > 2500.0f) {
				mHolderHeightDiff = 2500.0f;
				mPosition.y = mHolder->mPosition.y + mHolderHeightDiff;
			}
		}
	}

	mFaceAngle.y += (int)(*(f32*)((u8*)unk108 + 0x10) * mBarParams.mRotateSp.value);
	mFaceAngle.x = 0;
	mModelFaceAngle = mFaceAngle.y;

	int barResult = barProcess();
	if (barResult == 0) {
		if (*(f32*)((u8*)unk108 + 0x10) == 0.0f) {
			setAnimation(0xd, 1.0f);
		}
		if (*(f32*)((u8*)unk108 + 0x10) > 0.0f) {
			setAnimation(0x112, 1.0f);
		}
		if (*(f32*)((u8*)unk108 + 0x10) < 0.0f) {
			setAnimation(0x111, 1.0f);
		}
	}
}

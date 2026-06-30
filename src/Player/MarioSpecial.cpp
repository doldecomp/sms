#include <Player/Mario.hpp>
#include <Strategic/LiveActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionData.hpp>
#include <System/Particles.hpp>
#include <System/MarDirector.hpp>
#include <System/MarioGamePad.hpp>
#include <MSound/MSound.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <dolphin/mtx.h>
#include <stdlib.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

void TMario::barJumpSetting() { }

BOOL TMario::barWait()
{
	if (mHolder == nullptr)
		return changePlayerStatus(MARIO_STATUS_WALL_JUMP, 0, false);

	if (mInput & 0x2) {
		mPosition.x -= 200.0f * JMASSin(mFaceAngle.y);
		mPosition.z -= 200.0f * JMASCos(mFaceAngle.y);
		mFaceAngle.y += 0x8000;
		return changePlayerStatus(MARIO_STATUS_WALL_JUMP, 0, false);
	}

	mPosition.x = mHolder->mPosition.x;
	mPosition.y = mHolder->mPosition.y + mHolderHeightDiff;
	mPosition.z = mHolder->mPosition.z;

	if ((mInput & 0x10000) || mHolderHeightDiff > 100.0f) {
		setPlayerVelocity(-2.0f);
		mPosition.x -= 200.0f * JMASSin(mFaceAngle.y);
		mPosition.z -= 200.0f * JMASCos(mFaceAngle.y);
		return changePlayerStatus(MARIO_STATUS_LAND_SAFE_DOWN, 0, false);
	}

	if (unk108->mStickV > 16.0f)
		return changePlayerStatus(MARIO_STATUS_BAR_CLIMB, 0, false);

	if (unk108->mStickV < -16.0f) {
		mVel.y += unk108->mStickV * 0.001953125f;
		mPosition.y += mVel.y;
		mHolderHeightDiff = mPosition.y - mHolder->mPosition.y;
		treeSlipEffect();
		SMSGetMSound()->startSoundActor(MSD_SE_MA_SLIP_TREE, &mPosition, 0,
		                                nullptr, 0, 4);
	}

	if (mHolder->getActorType() == 0x400000bb) {
		if (unk108->mStickV <= 0.0f) {
			mPosition.y -= 2.0f;
			mHolderHeightDiff = mPosition.y - mHolder->mPosition.y;
		}

		if (mPosition.y < mHolder->mPosition.y + 200.0f) {
			mPosition.y       = mHolder->mPosition.y + 200.0f;
			mHolderHeightDiff = mPosition.y - mHolder->mPosition.y;
		}
	}

	if (mHolder->getActorType() == 0x40000039) {
		if (mHolderHeightDiff > 500.0f) {
			mHolderHeightDiff = 500.0f;
			mPosition.y       = mHolder->mPosition.y + mHolderHeightDiff;
		}
	}

	if (mHolder->getActorType() == 0x40000246) {
		u8 map = gpMarDirector->getCurrentMap();
		if (map == 8) {
			if (mHolderHeightDiff > 750.0f) {
				mHolderHeightDiff = 750.0f;
				mPosition.y       = mHolder->mPosition.y + mHolderHeightDiff;
			}
		} else if (map == 9) {
			if (mHolderHeightDiff > 750.0f) {
				mHolderHeightDiff = 750.0f;
				mPosition.y       = mHolder->mPosition.y + mHolderHeightDiff;
			}
		} else if (map == 4) {
			if (mHolderHeightDiff > 2500.0f) {
				mHolderHeightDiff = 2500.0f;
				mPosition.y       = mHolder->mPosition.y + mHolderHeightDiff;
			}
		}
	}

	mFaceAngle.y += (s16)(unk108->mStickH * mBarParams.mRotateSp.get());
	mFaceAngle.x    = 0;
	mModelFaceAngle = mFaceAngle.y;

	if (barProcess() == 0) {
		if (unk108->mStickH == 0.0f)
			setAnimation(ANIM_TREE_WAIT, 1.0f);
		if (unk108->mStickH > 0.0f)
			setAnimation(ANIM_TREE_MOVE_R, 1.0f);
		if (unk108->mStickH < 0.0f)
			setAnimation(ANIM_TREE_MOVE_L, 1.0f);
	}

	return 0;
}

BOOL TMario::barClimb()
{
	if (mHolder == nullptr)
		return changePlayerStatus(MARIO_STATUS_LAND_SAFE_DOWN, 0, false);

	if (mInput & 0x2) {
		mPosition.x -= 200.0f * JMASSin(mFaceAngle.y);
		mPosition.z -= 200.0f * JMASCos(mFaceAngle.y);
		mFaceAngle.y += 0x8000;
		return changePlayerStatus(MARIO_STATUS_WALL_JUMP, 0, false);
	}

	mPosition.x = mHolder->mPosition.x;
	mPosition.y = mHolder->mPosition.y + mHolderHeightDiff;
	mPosition.z = mHolder->mPosition.z;

	if (unk108->mStickV < 8.0f)
		return changePlayerStatus(MARIO_STATUS_BAR_WAIT, 0, false);

	mVel.y = 0.0f;
	mPosition.y += unk108->mStickV * mBarParams.mClimbSp.get();
	mHolderHeightDiff = mPosition.y - mHolder->mPosition.y;

	if (mPosition.y > mHolder->mPosition.y + mHolder->getDamageHeight()) {
		setPlayerVelocity(0.0f);
		changePlayerStatus(MARIO_STATUS_JUMP, 0, false);
		unk78 &= ~0x100;
	}

	if (barProcess() == 0) {
		// TODO: fakematch
		f32 rate = mBarParams.mClimbAnmRate.get();
		f32 v    = unk108->mStickV;
		setAnimation(ANIM_TREE_CLIMB, v * rate + 1.0f);
	}

	if (mHolder->getActorType() == 0x40000039) {
		if (mHolderHeightDiff > 500.0f) {
			mHolderHeightDiff = 500.0f;
			mPosition.y       = mHolder->mPosition.y + mHolderHeightDiff;
		}
	}

	if (mHolder->getActorType() == 0x40000246) {
		u8 state = gpMarDirector->mMap;
		if (state == 8) {
			if (mHolderHeightDiff > 750.0f) {
				mHolderHeightDiff = 750.0f;
				mPosition.y       = mHolder->mPosition.y + mHolderHeightDiff;
			}
		} else if (state == 9) {
			if (mHolderHeightDiff > 750.0f) {
				mHolderHeightDiff = 750.0f;
				mPosition.y       = mHolder->mPosition.y + mHolderHeightDiff;
			}
		} else if (state == 4) {
			if (mHolderHeightDiff > 2500.0f) {
				mHolderHeightDiff = 2500.0f;
				mPosition.y       = mHolder->mPosition.y + mHolderHeightDiff;
			}
		}
	}
	return 0;
}

BOOL TMario::barHang()
{
	if (mStatusTimer == 0) {
		rumbleStart(0x15, mMotorParams.mMotorWall.get());
		mStatusTimer += 1;
	}

	if (mHolder == nullptr)
		return changePlayerStatus(MARIO_STATUS_LAND_SAFE_DOWN, 0, false);

	mVel.y = 0.0f;

	if (barProcess() == 0) {
		setAnimation(ANIM_TREE_CATCH, 1.0f);
		if (isLast1AnimeFrame())
			changePlayerStatus(MARIO_STATUS_BAR_WAIT, 0, false);
	}

	mFaceAngle.x    = 0;
	mModelFaceAngle = mFaceAngle.y;
	return 0;
}

int TMario::hangingCheckRoof(JGeometry::TVec3<f32>* pos)
{
	TBGCheckData* wall = checkWallPlane(pos, 50.0f, 50.0f);
	if (wall != nullptr && wall->isFence()) {
		mFaceAngle.y = matan(wall->getNormal().z, wall->getNormal().x) + 0x8000;
		mModelFaceAngle = mFaceAngle.y;
		return 3;
	}

	const TBGCheckData* roof;
	const TBGCheckData* ground;

	f32 groundY = gpMap->checkGround(pos->x, pos->y, pos->z, &ground);

	f32 roofY = gpMap->checkRoof(pos->x, groundY + 80.0f, pos->z, &roof);

	if (ground == nullptr)
		return 1;
	if (roof == nullptr)
		return 2;
	if (!roof->isFence())
		return 2;
	if (roofY - groundY <= 160.0f)
		return 1;

	f32 space = roofY - (160.0f + pos->y);
	if (space < -30.0f)
		return 1;
	if (space > 30.0f)
		return 2;

	pos->y           = mFloorPosition.x - 160.0f;
	mGroundPlane     = ground;
	mFloorPosition.y = groundY;
	mRoofPlane       = roof;
	mFloorPosition.x = roofY;
	return 0;
}

int TMario::doRoofMovingProcess()
{
	if (++mForwardVel > 4.0f)
		mForwardVel = 4.0f;

	if (mIntendedMag > 0.0f) {
		s16 diff     = mIntendedYaw - mFaceAngle.y;
		mFaceAngle.y = mIntendedYaw - IConverge(diff, 0, 0x800, 0x800);
	}

	unk9E      = mFaceAngle.y;
	mSlideVelX = mForwardVel * JMASSin(mFaceAngle.y);
	mSlideVelZ = mForwardVel * JMASCos(mFaceAngle.y);
	mVel.x     = mSlideVelX;
	mVel.y     = 0.0f;
	mVel.z     = mSlideVelZ;

	JGeometry::TVec3<f32> newPos;
	newPos.x = mPosition.x - mVel.x * mRoofPlane->getNormal().y;
	newPos.z = mPosition.z - mVel.z * mRoofPlane->getNormal().y;
	newPos.y = mPosition.y;

	int code = hangingCheckRoof(&newPos);
	if (code == 2)
		code = 0;
	else
		mPosition = newPos;

	mModelFaceAngle = mFaceAngle.y;
	return code;
}

void TMario::doRoofWaitingProcess()
{
	mForwardVel = 0.0f;
	mSlideVelX  = 0.0f;
	mSlideVelZ  = 0.0f;
	mPosition.y = mFloorPosition.x - 160.0f;
	mVel.zero();
}

BOOL TMario::roofCommonEvents()
{
	if (mInput & 0x8000) {
		mInput &= ~0x8000;
		return changePlayerStatus(MARIO_STATUS_LANDING, 0, false);
	}
	if (mInput & 0x2) {
		const TLiveActor* actor = mRoofPlane->mActor;
		if (actor != nullptr) {
			((THitActor*)actor)->receiveMessage(this, 3);
			if (actor->mActorType == 0x4000006a) {
				emitParticle(PARTICLE_MS_M_AMIATTACK, &mHeadPos);
				rumbleStart(0x15, mMotorParams.mMotorWall.get());
				return changePlayerStatus(MARIO_STATUS_KICK_ROOF_ROLL_UP, 0,
				                          false);
			}
		}
		return changePlayerStatus(MARIO_STATUS_KICK_ROOF, 0, false);
	}
	return 0;
}

BOOL TMario::hangRoof()
{
	mFloorPosition.x = gpMap->checkRoof(mPosition.x, 80.0f + mPosition.y,
	                                    mPosition.z, &mRoofPlane);
	mStatusTimer += 1;

	if ((mInput & 0x1) && mStatusTimer > 120)
		return changePlayerStatus(MARIO_STATUS_WAIT_ROOF, 0, false);

	if (roofCommonEvents())
		return 1;

	mModelFaceAngle = mFaceAngle.y;
	setAnimation(ANIM_LADDER_HANG_CATCH, 1.0f);

	doRoofWaitingProcess();

	if (isLast1AnimeFrame())
		changePlayerStatus(MARIO_STATUS_WAIT_ROOF, 0, false);
	return 0;
}

BOOL TMario::waitRoof()
{
	if (roofCommonEvents())
		return 1;
	if (mInput & 0x1)
		return changePlayerStatus(MARIO_STATUS_MOVE_ROOF, mStatusArg, false);
	if (mStatusArg & 1)
		setAnimation(ANIM_LADDER_HANG_WAIT_L, 1.0f);
	else
		setAnimation(ANIM_LADDER_HANG_WAIT_R, 1.0f);

	doRoofWaitingProcess();
	return 0;
}

BOOL TMario::moveRoof()
{
	if (roofCommonEvents())
		return 1;

	if (mInput & 0x1) {
		switch (doRoofMovingProcess()) {
		case 2:
			return changePlayerStatus(MARIO_STATUS_LANDING, 0, false);
		case 3:
			changePlayerStatus(MARIO_STATUS_FENCE_CATCH, 0, false);
			break;
		}
	}

	if (mInput & 0x20)
		return changePlayerStatus(MARIO_STATUS_WAIT_ROOF, mStatusArg, false);

	f32 dist = JGeometry::TVec3<f32>(mPosition - unk29C).length();
	dist *= mHangRoofParams.mAnmMult.get();
	if (mStatusArg & 1)
		setAnimation(ANIM_LADDER_HANG_MOVE_L, dist);
	else
		setAnimation(ANIM_LADDER_HANG_MOVE_R, dist);

	if (isLast1AnimeFrame())
		mStatusArg = 1 - mStatusArg;

	return 0;
}

BOOL TMario::kickRoof()
{
	setAnimation(ANIM_LADDER_HANG_KICK, 1.0f);
	setAttackRadius(mAttackParamsKickRoof.mRadius.get());
	setAttackHeight(mAttackParamsKickRoof.mHeight.get());
	kickRoofEffect();

	if (isLast1AnimeFrame()) {
		mInput &= ~0x2;
		return changePlayerStatus(MARIO_STATUS_WAIT_ROOF, 0, false);
	}
	return 0;
}

BOOL TMario::kickRoofRollUp()
{
	setAnimation(ANIM_LADDER_ROLL_UP, 1.0f);
	kickRoofEffect();

	if (mStatusTimer++ == 8)
		startVoice(MSD_SE_MV15_EXERT_INST_02);

	if (isLast1AnimeFrame()) {
		mPosition.y += 160.0f;
		mFloorPosition.y = mPosition.y;
		mInput &= ~0x4;
		return changePlayerStatus(MARIO_STATUS_WAIT, 0, false);
	}

	return 0;
}

BOOL TMario::kickRoofRollDown()
{
	setAnimation(ANIM_LADDER_ROLL_DOWN, 1.0f);

	if (isLast1AnimeFrame())
		return changePlayerStatus(MARIO_STATUS_WAIT_ROOF, 0, false);

	return 0;
}

BOOL TMario::startHangLanding(u32 status)
{
	mVel.y      = 0.0f;
	mForwardVel = 0.0f;
	mPosition.x -= 60.0f * JMASSin(mFaceAngle.y);
	mPosition.z -= 60.0f * JMASCos(mFaceAngle.y);

	const TBGCheckData* dummy;
	f32 groundY
	    = gpMap->checkGround(mPosition.x, mPosition.y, mPosition.z, &dummy);
	if (groundY < mPosition.y - 100.0f)
		mPosition.y -= 100.0f;
	else
		mPosition.y = groundY;

	return changePlayerStatus(status, 0, false);
}

void TMario::hangingCommon(int, int) { }

void TMario::findNearestWall(const TBGWallCheckRecord&) { }

BOOL TMario::hanging()
{
	BOOL pulledUp = FALSE;
	s16 yawDiff   = mIntendedYaw - mFaceAngle.y;
	BOOL onCeil   = mFloorPosition.x - mFloorPosition.y >= 160.0f;

	if (mStatusTimer == 0)
		unkF6 = 0;

	if (mStatusTimer < mHangingParams.mLimitTime.get())
		mStatusTimer += 1;

	if ((mInput & 0x8004) || mGroundPlane->isUnk1()
	    || mGroundPlane->isNoLedgeGrab()) {
		mInput &= ~0x4;
		mInput &= ~0x8000;

		return startHangLanding(MARIO_STATUS_WALL_SLIDE);
	}

	if ((mInput & 0x2) && onCeil) {
		startVoice(MSD_SE_MV15_EXERT_INST_01);
		return changePlayerStatus(MARIO_STATUS_HANG_JUMPING, 0, false);
	}

	TBGCheckData* foundWall = nullptr;

	TBGWallCheckRecord record(mPosition.x, mPosition.y - 10.0f, mPosition.z,
	                          30.0f, 4, 0);
	gpMap->isTouchedWallsAndMoveXZ(&record);

	for (int i = 0; i < record.mResultWallsNum; i++) {
		TBGCheckData* w = record.mResultWalls[i];
		s16 ang = matan(w->mNormal.z, w->mNormal.x) - (mFaceAngle.y + 0x8000);
		if (ang > -0x2000 && ang < 0x2000) {
			JGeometry::TVec3<f32> pos = mPosition;
			f32 d = w->mNormal.x * pos.x + w->mNormal.y * pos.y
			        + w->mNormal.z * pos.z + w->mPlaneDistance;
			if (d < 50.0f)
				foundWall = record.mResultWalls[i];
		}
	}

	if (foundWall == nullptr)
		changePlayerStatus(MARIO_STATUS_LANDING, 0, false);

	if (foundWall != nullptr && foundWall->isFence()) {
		mPosition.x -= 40.0f * JMASSin(mFaceAngle.y);
		mPosition.y -= 160.0f;
		mPosition.z -= 40.0f * JMASCos(mFaceAngle.y);
		return changePlayerStatus(MARIO_STATUS_FENCE_CATCH, 0, false);
	}

	TBGWallCheckRecord record2(mPosition.x, 10.0f + mPosition.y, mPosition.z,
	                           50.0f, 1, 0);
	gpMap->isTouchedWallsAndMoveXZ(&record2);
	mPosition = record2.mCenter;

	if (mStatusTimer >= 0x28 && (mInput & 0x1)) {
		if (yawDiff >= -0x400 && yawDiff <= 0x400 && onCeil) {
			setAnimation(ANIM_HGUP, 1.0f);
			return changePlayerStatus(MARIO_STATUS_ASCEND, 0, false);
		}

		if (yawDiff <= -0x71c7 || yawDiff >= 0x71c7)
			return startHangLanding(MARIO_STATUS_LAND_SAFE_DOWN);

		if (mStatusTimer < mHangingParams.mRapidTime.get()
		    && record.mResultWallsNum > 0) {
			f32 moveSp = mHangingParams.mMoveSp.get();
			JGeometry::TVec3<f32> newPos;
			if (yawDiff > 0x400 && yawDiff < 0x71c7) {
				newPos.x = mPosition.x
				           - moveSp * (mIntendedMag * foundWall->mNormal.z);
				newPos.y = mPosition.y;
				newPos.z = mPosition.z
				           + moveSp * (mIntendedMag * foundWall->mNormal.x);
			}
			if (yawDiff > -0x71c7 && yawDiff < -0x400) {
				newPos.x = mPosition.x
				           + moveSp * (mIntendedMag * foundWall->mNormal.z);
				newPos.y = mPosition.y;
				newPos.z = mPosition.z
				           - moveSp * (mIntendedMag * foundWall->mNormal.x);
			}

			TBGCheckData* foundWall2 = nullptr;
			TBGWallCheckRecord record3(newPos.x, newPos.y, newPos.z, 50.0f, 1,
			                           0);
			gpMap->isTouchedWallsAndMoveXZ(&record3);
			newPos = record3.mCenter;

			const TBGCheckData* groundDummy;
			f32 groundY = gpMap->checkGround(newPos.x, 50.0f + newPos.y,
			                                 newPos.z, &groundDummy);
			if (mPosition.y - 100.0f < groundY
			    && groundY < 50.0f + mPosition.y) {
				TBGWallCheckRecord record4(
				    newPos.x - 30.0f * JMASSin(mFaceAngle.y), groundY - 20.0f,
				    newPos.z - 30.0f * JMASCos(mFaceAngle.y), 30.0f, 4, 0);
				gpMap->isTouchedWallsAndMoveXZ(&record4);

				for (int j = 0; j < record4.mResultWallsNum; j++) {
					TBGCheckData* w = record4.mResultWalls[j];
					s16 ang         = matan(w->mNormal.z, w->mNormal.x)
					          - (mFaceAngle.y + 0x8000);
					if (ang > -0x2000 && ang < 0x2000) {
						JGeometry::TVec3<f32> pos = mPosition;
						f32 d = w->mNormal.x * pos.x + w->mNormal.y * pos.y
						        + w->mNormal.z * pos.z + w->mPlaneDistance;
						if (d < 50.0f)
							foundWall2 = record4.mResultWalls[j];
					}
				}

				if (foundWall2 != nullptr
				    && mDeParams.mHangWallMovableAngle.get()
				           < foundWall->getNormal().x
				                     * foundWall2->getNormal().x
				                 + foundWall->getNormal().y
				                       * foundWall2->getNormal().y
				                 + foundWall->getNormal().z
				                       * foundWall2->getNormal().z) {

					mFaceAngle.y = matan(foundWall2->getNormal().z,
					                     foundWall2->getNormal().x)
					               + 0x8000;
					mPosition.x
					    = record4.mCenter.x - 40.0f * foundWall2->getNormal().x;
					mPosition.z
					    = record4.mCenter.z - 40.0f * foundWall2->getNormal().z;
					const TBGCheckData* dummy2;
					mPosition.y
					    = gpMap->checkGround(mPosition.x, 160.0f + mPosition.y,
					                         mPosition.z, &dummy2);
					pulledUp = TRUE;
				}
			}
		}
	}

	if (mStatusTimer >= mHangingParams.mLimitTime.get())
		return startHangLanding(MARIO_STATUS_LAND_SAFE_DOWN);

	f32 below = mPosition.y - checkPlayerAround(-0x8000, 30.0f);
	if (onCeil && below < 100.0f) {
		startVoice(MSD_SE_MV15_EXERT_INST_01);
		return changePlayerStatus(MARIO_STATUS_HANG_JUMPING, 0, false);
	}

	setPlayerVelocity(0.0f);
	mVel.y          = 0.0f;
	mPosition.y     = mFloorPosition.y;
	mModelFaceAngle = mFaceAngle.y;

	if (pulledUp == TRUE) {
		f32 dx = mPosition.x - unk29C.x;
		f32 dz = mPosition.z - unk29C.z;
		f32 anmRate
		    = MsSqrtf(dx * dx + dz * dz) * mHangingParams.mAnmRate.get();
		if (yawDiff < 0)
			setAnimation(ANIM_HMOV_L, anmRate);
		else
			setAnimation(ANIM_HMOV_R, anmRate);
	} else if (mStatusTimer < mHangingParams.mRapidTime.get()) {
		setAnimation(ANIM_HANG, 1.0f);
	} else {
		setAnimation(ANIM_HANG, mHangingParams.mAnmRapid.get());
	}
	return 0;
}

BOOL TMario::ascend()
{
	if (mInput & 0x4)
		return startHangLanding(MARIO_STATUS_LAND_SAFE_DOWN);
	waitProcess();
	setAnimation(ANIM_HGUP, 1.0f);
	if (isLast1AnimeFrame())
		changePlayerStatus(MARIO_STATUS_WAIT, 0, false);
	return 0;
}

BOOL TMario::descend()
{
	if (mInput & 0x4)
		return startHangLanding(MARIO_STATUS_LAND_SAFE_DOWN);
	waitProcess();
	setAnimation(ANIM_HGDWN, 1.0f);
	if (isLast1AnimeFrame())
		changePlayerStatus(MARIO_STATUS_HANGING, 0, false);
	return 0;
}

BOOL TMario::hangJumping()
{
	if (mInput & 0x4)
		return startHangLanding(MARIO_STATUS_LAND_SAFE_DOWN);
	waitProcess();
	setAnimation(ANIM_HGJMP, 1.0f);
	if (isLast1AnimeFrame())
		changePlayerStatus(MARIO_STATUS_WAIT, 0, false);
	return 0;
}

BOOL TMario::taken()
{
	MtxPtr mtx      = mHolder->getTakingMtx();
	mPosition.x     = mtx[0][3];
	mPosition.y     = mtx[1][3];
	mPosition.z     = mtx[2][3];
	mFaceAngle.x    = 0;
	mModelFaceAngle = mFaceAngle.y;
	setAnimation(ANIM_THROWN, 1.0f);
	return 0;
}

void TMario::getOnWirePosAngle(JGeometry::TVec3<f32>* outPos, s16* outAngle)
{
	JGeometry::TVec3<f32> start   = mWireStartPos;
	JGeometry::TVec3<f32> dir     = mWireEndPos - start;
	JGeometry::TVec3<f32> dirCopy = dir;

	*outPos = start + dir * mWirePosRatio;
	outPos->y -= 160.0f;

	Mtx rotA;
	J3DGetTranslateRotateMtx(mFaceAngle.x, 0, 0, 0.0f, 0.0f, 0.0f, rotA);
	Mtx rotB;
	J3DGetTranslateRotateMtx(0, mFaceAngle.y, 0, 0.0f, 0.0f, 0.0f, rotB);
	Mtx concat;
	MTXConcat(rotB, rotA, concat);

	JGeometry::TVec3<f32> sagVec(0.0f, -mWireSag * 1.0f, 0.0f);
	MTXMultVec(concat, &sagVec, &sagVec);

	outPos->x += sagVec.x;
	outPos->y += sagVec.y;
	outPos->z += sagVec.z;

	*outAngle = matan(dirCopy.z, dirCopy.x);
}

BOOL TMario::wireMove(f32 param_1)
{
	JGeometry::TVec3<f32> start = mWireStartPos;
	JGeometry::TVec3<f32> dir   = mWireEndPos - start;
	f32 len                     = dir.length();
	f32 delta                   = param_1 / len;
	f32 margin                  = 100.0f / len;

	BOOL clean = true;
	if (mWirePosRatio + delta > 1.0f - margin) {
		mWirePosRatio = 1.0f - margin;
		clean         = false;
	}
	if (mWirePosRatio + delta < margin) {
		mWirePosRatio = margin;
		clean         = false;
	}
	if (clean)
		mWirePosRatio += delta;
	return clean;
}

BOOL TMario::wireWait()
{
	s16 wireAngle;
	getOnWirePosAngle(&mPosition, &wireAngle);

	if (mInput & 0x2)
		onFlag(MARIO_FLAG_UNK100);

	if (checkFlag(MARIO_FLAG_UNK100) == true) {
		if (mWireSag <= 0.0f) {
			((THitActor*)mHolder)->receiveMessage(this, 8);
			mHolder  = nullptr;
			BOOL ret = changePlayerStatus(MARIO_STATUS_WIRE_JUMP, 0, false);
			setPlayerVelocity(0.0f);
			if (mWireBounceVel < 0.0f)
				mVel.y -= mWireParams.mWireJumpMult.get() * mWireBounceVel;
			mVel.y += mWireParams.mWireJumpBase.get();

			u32 sfx;
			if (mWireBounceVel < mWireSfx1MinVel)
				sfx = MSD_SE_MA_ROPE_JUMP_C;
			else if (mWireBounceVel < mWireSfx0MinVel)
				sfx = MSD_SE_MA_ROPE_JUMP_B;
			else
				sfx = MSD_SE_MA_ROPE_JUMP_A;
			SMSGetMSound()->startSoundActorWithInfo(
			    sfx, &mPosition, nullptr, mWireSag, 0, 0, nullptr, 0, 4);
			unk78 &= ~0x100;
			return ret;
		}
	}

	if (mInput & 0x10000) {
		mWireBounceVel = 5.0f;
		return changePlayerStatus(MARIO_STATUS_WIRE_WAIT_TO_HANG, 0, false);
	}

	if (mInput & 0x1) {
		s16 diff = wireAngle - mIntendedYaw;
		if (diff > -0x2000 && diff < 0x2000) {
			f32 anmRate = 0.05f * mIntendedMag;
			if (wireMove(0.1f * mIntendedMag)) {
				if (mIntendedMag > 16.0f)
					setAnimation(ANIM_ROPE_RUN, anmRate);
				else
					setAnimation(ANIM_ROPE_WALK, anmRate);
			} else {
				setAnimation(ANIM_ROPE_WAIT, 1.0f);
			}
		}
		if (diff < -0x2000) {
			startVoice(MSD_SE_MV30_FRIGHT_01);
			return changePlayerStatus(MARIO_STATUS_WIRE_WAIT_TO_S_WAIT_L, 0,
			                          false);
		}
		if (diff > 0x2000) {
			startVoice(MSD_SE_MV30_FRIGHT_01);
			return changePlayerStatus(MARIO_STATUS_WIRE_WAIT_TO_S_WAIT_R, 0,
			                          false);
		}
	} else {
		setAnimation(ANIM_ROPE_WAIT, 1.0f);
	}

	mFaceAngle.y    = wireAngle;
	mModelFaceAngle = mFaceAngle.y;
	return 0;
}

BOOL TMario::wireSWait()
{
	s16 wireAngle;

	getOnWirePosAngle(&mPosition, &wireAngle);

	if (mInput & 0x2)
		onFlag(MARIO_FLAG_UNK100);

	if (checkFlag(MARIO_FLAG_UNK100) == true) {
		if (mWireSag < 0.0f) {
			((THitActor*)mHolder)->receiveMessage(this, 8);
			mHolder  = nullptr;
			BOOL ret = changePlayerStatus(MARIO_STATUS_WIRE_JUMP, 0, false);
			setPlayerVelocity(0.0f);
			if (mWireBounceVelPrev < 0.0f)
				mVel.y -= 5.0f * mWireBounceVelPrev;
			return ret;
		}
	}

	if (mInput & 0x10000) {
		mWireBounceVelPrev = 5.0f;
		startVoice(MSD_SE_MV30_FRIGHT_01);
		return changePlayerStatus(MARIO_STATUS_WIRE_WAIT_TO_HANG, 0, false);
	}

	if (mInput & 0x1) {
		s16 diff = mIntendedYaw - wireAngle;
		if (diff > -0x4000 && diff < 0x4000)
			return changePlayerStatus(MARIO_STATUS_WIRE_S_WAIT_TO_WAIT_L, 0,
			                          false);

		JGeometry::TVec3<f32> tmp = mWireStartPos;
		mWireStartPos             = mWireEndPos;
		mWireEndPos               = tmp;
		mWirePosRatio             = 1.0f - mWirePosRatio;
		return changePlayerStatus(MARIO_STATUS_WIRE_S_WAIT_TO_WAIT_R, 0, false);
	}

	setAnimation(ANIM_ROPE_SWAIT, 1.0f);
	mModelFaceAngle = wireAngle;
	mFaceAngle.y    = mModelFaceAngle + 0x4000;
	return 0;
}

BOOL TMario::wireWaitToSWaitL()
{
	getOnWirePosAngle(&mPosition, &mModelFaceAngle);
	mFaceAngle.y = mModelFaceAngle + 0x4000;
	setAnimation(ANIM_ROPE_WTOSW, 1.0f);
	if (isLast1AnimeFrame())
		changePlayerStatus(MARIO_STATUS_WIRE_S_WAIT, 0, false);
	return 0;
}

BOOL TMario::wireWaitToSWaitR()
{
	getOnWirePosAngle(&mPosition, &mModelFaceAngle);
	mFaceAngle.y = mModelFaceAngle - 0x4000;
	setAnimation(ANIM_ROPE_WTOSW_R, 1.0f);
	if (isLast1AnimeFrame()) {
		changePlayerStatus(MARIO_STATUS_WIRE_S_WAIT, 0, false);
		JGeometry::TVec3<f32> tmp = mWireStartPos;
		mWireStartPos             = mWireEndPos;
		mWireEndPos               = tmp;
		mWirePosRatio             = 1.0f - mWirePosRatio;
	}
	return 0;
}

void TMario::changeWireHanging()
{
	bool ok = false;
	if (mHeldObject == nullptr && !onYoshi())
		ok = true;
	if (ok) {
		changePlayerStatus(MARIO_STATUS_WIRE_HANGING, 0, false);
	} else {
		changePlayerStatus(MARIO_STATUS_LANDING, 0, false);
	}
}

BOOL TMario::wireWaitToHang()
{
	getOnWirePosAngle(&mPosition, &mModelFaceAngle);
	mFaceAngle.y = mModelFaceAngle + 0x4000;
	setAnimation(ANIM_ROPE_WHG, 1.0f);
	if (isLast1AnimeFrame()) {
		BOOL noHold = FALSE;
		if (mHeldObject == nullptr && !onYoshi())
			noHold = TRUE;
		if (noHold)
			return changePlayerStatus(MARIO_STATUS_WIRE_HANGING, 0, false);
		return changePlayerStatus(MARIO_STATUS_LANDING, 0, false);
	}
	return 0;
}

BOOL TMario::wireSWaitToHang()
{
	getOnWirePosAngle(&mPosition, &mFaceAngle.y);
	mModelFaceAngle = mFaceAngle.y;
	setAnimation(ANIM_ROPE_SWHG, 1.0f);
	if (isLast1AnimeFrame()) {
		BOOL noHold = FALSE;
		if (mHeldObject == nullptr && !onYoshi())
			noHold = TRUE;
		if (noHold)
			return changePlayerStatus(MARIO_STATUS_WIRE_HANGING, 0, false);
		return changePlayerStatus(MARIO_STATUS_LANDING, 0, false);
	}
	return 0;
}

BOOL TMario::wireReturn()
{
	getOnWirePosAngle(&mPosition, &mFaceAngle.y);
	mModelFaceAngle = mFaceAngle.y;
	setAnimation(ANIM_ROPE_RETURN, 1.0f);
	if (isLast1AnimeFrame())
		changePlayerStatus(MARIO_STATUS_WIRE_WAIT, 0, false);
	return 0;
}

BOOL TMario::wireHanging()
{
	s16 wireAngle;
	getOnWirePosAngle(&mPosition, &wireAngle);

	BOOL b = mFloorPosition.x - mFloorPosition.y >= 160.0f;

	if ((mInput & 0x2) && b) {
		mWireBounceVel = 5.0f;
		return changePlayerStatus(MARIO_STATUS_WIRE_RETURN, 0, false);
	}

	if (mInput & 0x8000) {
		((THitActor*)mHolder)->receiveMessage(this, HIT_MESSAGE_UNK8);
		mHolder = nullptr;

		return startHangLanding(MARIO_STATUS_WIRE_HANG_LAND_SAFE_DOWN);
	}

	unkF6           = 0;
	mFaceAngle.x    = 0;
	mFaceAngle.y    = wireAngle + 0x4000;
	mModelFaceAngle = wireAngle;

	if (mInput & 0x1) {
		s16 diff = wireAngle - mIntendedYaw;
		if (diff > -0x2000 && diff < 0x3555) {
			if (wireMove(3.0f))
				setAnimation(ANIM_ROPE_HMOVR, 1.0f);
			else
				setAnimation(ANIM_ROPE_HGWAT, 1.0f);
		}

		if (diff < -0x6000 || diff > 0x4aaa) {
			if (wireMove(-3.0f))
				setAnimation(ANIM_ROPE_HMOVL, 1.0f);
			else
				setAnimation(ANIM_ROPE_HGWAT, 1.0f);
		}

		if (diff >= -0x6000 && diff <= -0x2000) {
			mWireBounceVel = 5.0f;
			return changePlayerStatus(MARIO_STATUS_WIRE_RETURN, 0, false);
		}

		if (diff >= 0x3555 && diff <= 0x4aaa)
			startHangLanding(MARIO_STATUS_WIRE_HANG_LAND_SAFE_DOWN);

		return 0;
	}

	if (mUpperState == UPPER_STATE_PUMPING && mWaterGun != nullptr
	    && mWaterGun->canSpray()) {
		s16 rotSp;
		if (mWaterGun == nullptr) {
			rotSp = 0;
		} else {
			switch (mWaterGun->mCurrentNozzle) {
			case 1:
				rotSp = mWireParams.mRotSpeedTrgRocket.get();
				break;
			case 4:
				rotSp = mWireParams.mRotSpeedTrgHover.get();
				break;
			case 5:
				rotSp = mWireParams.mRotSpeedTrgTurbo.get();
				break;
			default:
				rotSp = mWireParams.mRotSpeed.get();
				break;
			}
		}
		if (rotSp > 0)
			unkF6 += 2 * mWireParams.mRotStop.get();
		else
			unkF6 -= 2 * mWireParams.mRotStop.get();

		s16 rotSp2;
		if (mWaterGun == nullptr) {
			rotSp2 = 0;
		} else {
			switch (mWaterGun->mCurrentNozzle) {
			case 1:
				rotSp2 = mWireParams.mRotSpeedTrgRocket.get();
				break;
			case 4:
				rotSp2 = mWireParams.mRotSpeedTrgHover.get();
				break;
			case 5:
				rotSp2 = mWireParams.mRotSpeedTrgTurbo.get();
				break;
			default:
				rotSp2 = mWireParams.mRotSpeed.get();
				break;
			}
		}
		unkF6 += rotSp2;
		return changePlayerStatus(MARIO_STATUS_WIRE_ROLLING, 0, false);
	}

	setAnimation(ANIM_ROPE_HGWAT, 1.0f);
	return 0;
}

s16 TMario::getNozzleEmitVX() { }

BOOL TMario::wireRolling()
{
	s16 initialAngle = mFaceAngle.x;

	s16 wireAngle;
	getOnWirePosAngle(&mPosition, &wireAngle);

	if (mInput & 0x2)
		mStatusState |= 1;

	s16 rotSpeedMax = mWireParams.mRotSpeedMax.get();

	if ((mStatusState & 1) && unkF6 < 0) {
		s16 ang = mFaceAngle.x;
		if (ang > -0x2000 && ang <= rotSpeedMax - 0x2000)
			return changePlayerStatus(MARIO_STATUS_WIRE_ROLL_JUMP, 0, false);
	}
	if ((mStatusState & 1) && unkF6 > 0) {
		s16 ang = mFaceAngle.x;
		if (0x6000 - rotSpeedMax <= ang && ang < 0x6000)
			return changePlayerStatus(MARIO_STATUS_WIRE_ROLL_JUMP, 1, false);
	}

	if (mInput & 0x1) {
		s16 diff = wireAngle - mIntendedYaw;
		if (diff > -0x2000 && diff < 0x3555)
			wireMove(3.0f);
		if (diff < -0x6000 || diff > 0x4aaa)
			wireMove(-3.0f);
	}

	if (mUpperState == UPPER_STATE_PUMPING && mWaterGun != nullptr
	    && mWaterGun->canSpray()) {
		s16 rotSp;
		if (mWaterGun == nullptr) {
			rotSp = 0;
		} else {
			switch (mWaterGun->mCurrentNozzle) {
			case 1:
				rotSp = mWireParams.mRotSpeedTrgRocket.get();
				break;
			case 4:
				rotSp = mWireParams.mRotSpeedTrgHover.get();
				break;
			case 5:
				rotSp = mWireParams.mRotSpeedTrgTurbo.get();
				break;
			default:
				rotSp = mWireParams.mRotSpeed.get();
				break;
			}
		}
		unkF6 += rotSp;
	}

	unkF6 -= (s16)((f32)mWireParams.mRotGravity.get() * JMASSin(mFaceAngle.x));

	if ((mInput & 0x4000) ? true : false)
		unkF6 = (s16)((f32)unkF6 * mWireParams.mRotBrake.get());

	if (unkF6 < -rotSpeedMax)
		unkF6 = -rotSpeedMax;
	if (unkF6 > rotSpeedMax)
		unkF6 = rotSpeedMax;

	mFaceAngle.x += unkF6;
	mWireSag       = 0.1f * fabsf((f32)unkF6);
	mWireBounceVel = 0.0f;

	if (initialAngle < -0x4000 && mFaceAngle.x > 0x4000)
		mStatusState |= 2;
	else if (initialAngle < mFaceAngle.x)
		mStatusState &= ~2;

	u32 sfxId    = 0;
	BOOL playSfx = FALSE;

	if (mStatusState & 2) {
		if (mFaceAngle.x < mWireSwingPosAngle
		    && mWireSwingPosAngle <= initialAngle) {
			mWireQueuedSfxID = MSD_SE_OBJ_ROPE_CLEAK_ROLL;
			sfxId            = MSD_SE_MA_ROPE_ROLL_FULL_A;
			playSfx          = TRUE;
			mWireSfxTimer    = mWireSfxDelay;
			if (!(mStatusState & 0xc)) {
				if (0.000030517578f * (f32)rand() < 0.5f)
					mStatusState |= 4;
				else
					mStatusState |= 8;
			}
			if ((mStatusState & 0xc) == 4)
				startVoice(MSD_SE_MV25A_WIRE_ROLL_01A);
			if ((mStatusState & 0xc) == 8)
				startVoice(MSD_SE_MV25A_WIRE_ROLL_02A);
		}

		if (mFaceAngle.x < mWireSwingNegAngle
		    && mWireSwingNegAngle <= initialAngle) {
			sfxId   = MSD_SE_MA_ROPE_ROLL_FULL_B;
			playSfx = TRUE;
		}

		if (initialAngle > 0 && mFaceAngle.x <= 0) {
			mWireQueuedSfxID = MSD_SE_OBJ_ROPE_CLEAK_ROLL;
			mWireSfxTimer    = mWireSfxDelay;
		}
	} else {
		if (mFaceAngle.x < mWireRollAngle && mWireRollAngle <= initialAngle) {
			mWireQueuedSfxID = MSD_SE_OBJ_ROPE_CLEAK_HALFA;
			sfxId            = MSD_SE_MA_ROPE_ROLL_HALF_A;
			playSfx          = TRUE;
			mWireSfxTimer    = mWireSfxDelay;
		}
		if (initialAngle < mWireRollAngle && mWireRollAngle <= mFaceAngle.x) {
			mWireQueuedSfxID = MSD_SE_OBJ_ROPE_CLEAK_HALFB;
			sfxId            = MSD_SE_MA_ROPE_ROLL_HALF_B;
			playSfx          = TRUE;
			mWireSfxTimer    = mWireSfxDelay;
		}
	}

	if (playSfx == TRUE) {
		f32 sag = mWireSag;
		if (gpMSound->gateCheck(sfxId))
			MSoundSESystem::MSoundSE::startSoundActorWithInfo(
			    sfxId, &mPosition, nullptr, sag, 0, 0, nullptr, 0, 4);
	}

	blurEffect();

	s16 rotStop = mWireParams.mRotStop.get();
	if (mFaceAngle.x > -0x200 && mFaceAngle.x < 0x200) {
		s16 fSpeed = unkF6;
		if (-rotStop < fSpeed && fSpeed < rotStop) {
			mFaceAngle.x   = 0;
			unkF6          = 0;
			mWireBounceVel = 0.0f;
			mWireSag       = 0.0f;
			BOOL noHold    = FALSE;
			if (mHeldObject == nullptr && !onYoshi())
				noHold = TRUE;
			if (noHold)
				return changePlayerStatus(MARIO_STATUS_WIRE_HANGING, 0, false);
			return changePlayerStatus(MARIO_STATUS_LANDING, 0, false);
		}
	}

	mFaceAngle.y    = wireAngle + 0x4000;
	mModelFaceAngle = mFaceAngle.y;
	setAnimation(ANIM_HANG, 1.0f);
	return 0;
}

BOOL TMario::wireSWaitToWaitL()
{
	getOnWirePosAngle(&mPosition, &mModelFaceAngle);
	mFaceAngle.y = mModelFaceAngle + 0x4000;
	setReverseAnimation(ANIM_ROPE_WTOSW, 1.0f);
	if (isAnimeLoopOrStop())
		changePlayerStatus(MARIO_STATUS_WIRE_WAIT, 0, false);
	return 0;
}

BOOL TMario::wireSWaitToWaitR()
{
	getOnWirePosAngle(&mPosition, &mModelFaceAngle);
	mFaceAngle.y = mModelFaceAngle - 0x4000;
	setReverseAnimation(ANIM_ROPE_WTOSW_R, 1.0f);
	if (isAnimeLoopOrStop())
		changePlayerStatus(MARIO_STATUS_WIRE_WAIT, 0, false);
	return 0;
}

void TMario::getCurrentPullParams(f32* outV, f32* outH)
{
	TPullParams* params = nullptr;
	switch (mHeldObject->getActorType()) {
	case 0x8000008:
		params = &mPullParamsBGBeak;
		break;
	case 0x8000006:
		params = &mPullParamsBGTentacle;
		break;
	case 0x800000D:
		params = &mPullParamsBGFireWanWanBossTail;
		break;
	case 0x10000028:
		params = &mPullParamsFireWanWanTail;
		break;
	}

	if (params == nullptr)
		return;
	if (mStatus == MARIO_STATUS_PULLING) {
		*outV = params->mPullRateV.get();
		*outH = params->mPullRateH.get();
	} else {
		*outV = params->mOilPullRateV.get();
		*outH = params->mOilPullRateH.get();
	}
}

void TMario::setPullingAnm(const JGeometry::TVec3<f32>&, f32) { }

BOOL TMario::pulling()
{
	if (mInput & 0x4) {
		((THitActor*)mHeldObject)->receiveMessage(this, 8);
		mHeldObject = nullptr;
		startVoice(MSD_SE_MV30_FRIGHT_01);
		return changePlayerStatus(MARIO_STATUS_LANDING, 0, false);
	}

	if (!(unk108->mInput & 0x400)) {
		((THitActor*)mHeldObject)->receiveMessage(this, 8);
		mHeldObject = nullptr;
		startVoice(MSD_SE_MV30_FRIGHT_01);
		return changePlayerStatus(0xc0022f, 0, false);
	}

	if (mInput & 0x2) {
		setAnimation(ANIM_HOLD_TO_HANG, 1.0f);
		return changePlayerJumping(MARIO_STATUS_PULL_JUMP, 0);
	}

	if (mStatus == MARIO_STATUS_OIL_PULLING && !checkFlag(MARIO_FLAG_DIRTY))
		changePlayerStatus(MARIO_STATUS_PULLING, 0, false);

	MtxPtr mtx      = mHeldObject->getTakingMtx();
	mFaceAngle.y    = matan(-mtx[2][2], -mtx[0][2]);
	mModelFaceAngle = mFaceAngle.y;

	JGeometry::TVec3<f32> pos = mPosition;
	s16 backAngle             = mFaceAngle.y + 0x8000;
	s16 diff                  = mIntendedYaw - backAngle;
	f32 cosF                  = JMASCos(diff);
	if (cosF < 0.0f)
		cosF = 0.0f;
	f32 sinF = JMASSin(diff);

	cosF *= mIntendedMag;
	sinF *= mIntendedMag;

	f32 rateV, rateH;
	getCurrentPullParams(&rateV, &rateH);

	pos.x += cosF * rateH * JMASSin(backAngle)
	         - sinF * rateV * JMASCos(backAngle);
	pos.z += cosF * rateH * JMASCos(backAngle)
	         + sinF * rateV * JMASSin(backAngle);

	if (((THitActor*)mHeldObject)->receiveMessage(this, 0xa) == 1) {
		mPosition = pos;
	}

	stopProcess();

	f32 animRate = 1.0f;
	if (mStatus == MARIO_STATUS_OIL_PULLING)
		animRate = 5.0f;

	switch (mAnimationId) {
	case ANIM_HOLD:
	case ANIM_HANG_TO_HOLD:
		if (isLast1AnimeFrame())
			setAnimation(ANIM_HOLD_WAIT, 1.0f);
		break;

	default:
		JGeometry::TVec3<f32> delta;
		if ((mHeldObject->getActorType() == 0x8000006 ? true : false)
		    || (mHeldObject->getActorType() == 0x8000008 ? true : false)) {
			delta = pos - unk29C;
		} else {
			delta = mPosition - unk29C;
		}

		f32 len = delta.length();
		if (len < 1.0f) {
			setAnimation(ANIM_HOLD_WAIT, animRate);
		} else {
			s16 ang  = matan(delta.z, delta.x);
			s16 adff = ang - mFaceAngle.y;
			if (adff >= -0x2000 && adff <= 0x2000)
				setAnimation(ANIM_HOLD_DRAG, animRate);
			if (adff <= -0x6000 || adff >= 0x6000)
				setAnimation(ANIM_HOLD_BACK, animRate);
			if (adff > -0x6000 && adff < -0x2000)
				setAnimation(ANIM_HOLD_MOVE_R, animRate);
			if (adff > 0x2000 && adff < 0x6000)
				setAnimation(ANIM_HOLD_MOVE_L, animRate);
		}
		break;
	}

	return 0;
}

void TMario::fenceFootCheck()
{
	JGeometry::TVec3<f32> front = mPosition;
	front.x += 0.8f * (50.0f * JMASSin(mFaceAngle.y));
	front.z += 0.8f * (50.0f * JMASCos(mFaceAngle.y));
	if (checkWallPlane(&front, 20.0f, 50.0f) == nullptr) {
		mFaceAngle.y += 0x8000;
		setPlayerVelocity(0.0f);
		mVel.y = 0.0f;
		changePlayerStatus(MARIO_STATUS_LAND_SAFE_DOWN, 0, false);
	}
}

BOOL TMario::fenceCatch()
{
	setAnimation(ANIM_FENCE_CATCH, 1.0f);
	fenceFootCheck();

	if (isLast1AnimeFrame()) {
		if (5.0f + mFloorPosition.y > mPosition.y)
			mPosition.y += 5.0f;
		if (mFloorPosition.x - 5.0f < mPosition.y)
			mPosition.y -= 5.0f;
		return changePlayerStatus(MARIO_STATUS_FENCE_MOVE, 0, false);
	}

	return 0;
}

BOOL TMario::fenceJumpCatch()
{
	setAnimation(ANIM_FENCE_JCATCH, 1.0f);
	fenceFootCheck();

	if (isLast1AnimeFrame())
		return changePlayerStatus(MARIO_STATUS_FENCE_MOVE, 0, false);

	return 0;
}

BOOL TMario::fenceMove()
{
	JGeometry::TVec3<f32> frontPos = mPosition;
	frontPos.x += 0.5f * (50.0f * JMASSin(mFaceAngle.y));
	frontPos.z += 0.5f * (50.0f * JMASCos(mFaceAngle.y));

	TBGCheckData* wall = checkWallPlane(&frontPos, 80.0f, 50.0f);
	if (wall != nullptr) {
		if (mInput & 0x1) {
			JGeometry::TVec3<f32> newPos = mPosition;
			newPos.y
			    += 0.015625f * unk108->mStickV * mJumpParams.mFenceSpeed.get();

			s16 camDelta = mFaceAngle.y - gpCamera->unk258;
			f32 normX, normZ;
			if (camDelta > -0x4000 && camDelta < 0x4000) {
				normZ = wall->mNormal.z;
				normX = -wall->mNormal.x;
			} else {
				normX = wall->mNormal.x;
				normZ = -wall->mNormal.z;
			}

			f32 stickH = 0.015625f * unk108->mStickH;
			newPos.x += normX * stickH * mJumpParams.mFenceSpeed.get();
			newPos.z += normZ * stickH * mJumpParams.mFenceSpeed.get();

			JGeometry::TVec3<f32> sideFront = newPos;
			sideFront.x += 0.5f * (50.0f * JMASSin(mFaceAngle.y));
			sideFront.z += 0.5f * (50.0f * JMASCos(mFaceAngle.y));

			JGeometry::TVec3<f32> sideFront2 = sideFront;

			TBGCheckData* sideWall1 = checkWallPlane(&sideFront, 20.0f, 50.0f);
			TBGCheckData* sideWall2
			    = checkWallPlane(&sideFront2, 140.0f, 50.0f);
			if (sideWall1 != nullptr && (sideWall1->isFence())
			    && sideWall2 != nullptr && (sideWall2->isFence())) {
				if (mStatus == MARIO_STATUS_FENCE_MOVE)
					changePlayerStatus(0x30000569, 0, false);
				mPosition = newPos;
			}
		} else {
			setAnimation(ANIM_FENCE_WAIT, 1.0f);
			changePlayerStatus(MARIO_STATUS_FENCE_MOVE, 0, false);
		}

		mFaceAngle.y = matan(wall->mNormal.z, wall->mNormal.x) + 0x8000;
	} else {
		mFaceAngle.y += 0x8000;
		mModelFaceAngle = mFaceAngle.y;
		setPlayerVelocity(0.0f);
		return changePlayerStatus(MARIO_STATUS_LANDING, 0, false);
	}

	mModelFaceAngle = mFaceAngle.y;

	mPosition.x += 0.5f * (50.0f * JMASSin(mFaceAngle.y));
	mPosition.z += 0.5f * (50.0f * JMASCos(mFaceAngle.y));
	mVel.x = 0.0f;
	mVel.y = 0.0f;
	mVel.z = 0.0f;

	switch (jumpProcess(1)) {
	case 1:
		mFaceAngle.y += 0x8000;
		return changePlayerStatus(MARIO_STATUS_LANDING, 0, false);
	case 3:
		mPosition.x += 50.0f * JMASSin(mFaceAngle.y);
		mPosition.y = 1.0f + mFloorPosition.y;
		mPosition.z += 50.0f * JMASCos(mFaceAngle.y);
		setAnimation(ANIM_HGUP, 1.0f);
		mInput &= ~0x4;
		return changePlayerDropping(MARIO_STATUS_ASCEND, 0);
	case 2:
	default:
		if (mRoofPlane != nullptr) {
			f32 roofY = mFloorPosition.x;
			if (roofY < 160.0f + mPosition.y) {
				mPosition.y = roofY - 160.0f;
				changePlayerStatus(MARIO_STATUS_FENCE_MOVE, 0, false);
			}
		}

		if (mInput & 0x2) {
			SMSGetMSound()->startSoundActor(MSD_SE_MA_FENCE_PUNCH, &mPosition,
			                                0, nullptr, 0, 4);
			rumbleStart(0x15, mMotorParams.mMotorWall.get());
			return startJumpWall();
		}

		if (mInput & 0x8000) {
			mInput &= ~0x8000;
			return changePlayerStatus(MARIO_STATUS_FENCE_PUNCH, 0, false);
		}

		if (mIntendedMag > 0.0f) {
			f32 hDot, vDiff, dist;
			if (unk2C0 == nullptr) {
				vDiff    = mPosition.y - unk29C.y;
				f32 sinY = JMASSin(mFaceAngle.y);
				f32 cosY = JMASCos(mFaceAngle.y);

				JGeometry::TVec3<f32> diff = mPosition - unk29C;

				hDot = -cosY * diff.z + sinY * diff.x;
				dist = diff.length();
			} else {
				vDiff = unk300.y - unk2F4.y;
				hDot  = unk2F4.x - unk300.x;
				if ((s16)((182.04445f * unk30C) - (f32)mFaceAngle.y) != 0)
					hDot = -hDot;
				dist = JGeometry::TVec3<f32>(unk2F4 - unk300).length();
			}

			f32 absH = hDot;
			if (hDot < 0.0f)
				absH = -hDot;
			f32 absV = vDiff;
			if (vDiff < 0.0f)
				absV = -vDiff;

			if (absV > 5.0f * absH) {
				if (vDiff > 0.0f)
					setAnimation(ANIM_FENCE_MOVE_UP, dist);
				else
					setAnimation(ANIM_FENCE_MOVE_DOWN, dist);
			} else if (hDot > 0.0f) {
				setAnimation(ANIM_FENCE_MOVE_L, dist);
			} else {
				setAnimation(ANIM_FENCE_MOVE_R, dist);
			}
		}
		return 0;
	}
}

BOOL TMario::fencePunch()
{
	JGeometry::TVec3<f32> pos = mPosition;
	pos.x += 0.5f * (50.0f * JMASSin(mFaceAngle.y));
	pos.z += 0.5f * (50.0f * JMASCos(mFaceAngle.y));
	TBGCheckData* wall = checkWallPlane(&pos, 80.0f, 50.0f);

	if (mInput & 0x2) {
		SMSGetMSound()->startSoundActor(MSD_SE_MA_FENCE_PUNCH, &mPosition, 0,
		                                nullptr, 0, 4);
		rumbleStart(0x15, mMotorParams.mMotorWall.get());
		return startJumpWall();
	}

	if (wall) {
		mPosition = pos;
	} else {
		mFaceAngle.y += 0x8000;
		mModelFaceAngle = mFaceAngle.y;
		setPlayerVelocity(0.0f);
		return changePlayerStatus(MARIO_STATUS_LANDING, 0, false);
	}

	setAnimation(ANIM_FENCE_PUNCH, 1.0f);
	setAttackRadius(mAttackParamsFencePunch.mRadius.get());
	setAttackHeight(mAttackParamsFencePunch.mHeight.get());
	offHitFlag(HIT_FLAG_UNK2);
	mModelFaceAngle = mFaceAngle.y;

	if (getMotionFrameCtrl().checkPass(5.0f)) {
		emitParticle(PARTICLE_MS_M_AMIATTACK, &mRightHandPos);
		rumbleStart(0x15, mMotorParams.mMotorWall.get());
		if (unk2C0 != nullptr) {
			((THitActor*)unk2C0)->receiveMessage(this, 3);
			startVoice(MSD_SE_MV15_EXERT_INST_02);
			if (unk2C0->mActorType == 0x4000006a) {
				f32 x = unk2F4.x;
				f32 z = unk2F4.y;
				if (x < -120.0f)
					x = -120.0f;
				if (120.0f < x)
					x = 120.0f;
				if (z < -190.0f)
					z = -190.0f;
				if (60.0f < z)
					z = 60.0f;
				unk2F4.x = x;
				unk2F4.y = z;
				Mtx mtx;
				getRidingMtx(mtx);
				PSMTXMultVec(mtx, &unk2F4, &mPosition);
			}
		}
	}

	if (isLast1AnimeFrame()) {
		changePlayerStatus(MARIO_STATUS_FENCE_MOVE, 0, false);
		return 1;
	}
	return 0;
}

BOOL TMario::specMain()
{
	int result = 0;

	mWireBounceVelPrev = mWireBounceVel;
	mWireBounceVel -= mWireSag * mWireParams.mSwingRate.get();
	mWireSag += mWireBounceVel;
	mWireSag *= mWireParams.mWireSwingBrake.get();

	if (mWireSag < -mWireParams.mWireSwingMax.get())
		mWireSag = -mWireParams.mWireSwingMax.get();
	if (mWireParams.mWireSwingMax.get() < mWireSag)
		mWireSag = mWireParams.mWireSwingMax.get();

	switch (mStatus) {
		// The "bars" here are, in fact, trees.
	case MARIO_STATUS_BAR_WAIT:
		result = barWait();
		break;

	case MARIO_STATUS_BAR_HANG:
		result = barHang();
		break;

	case MARIO_STATUS_BAR_CLIMB:
		result = barClimb();
		break;

	case MARIO_STATUS_KICK_ROOF_ROLL_UP:
		result = kickRoofRollUp();
		break;

	case MARIO_STATUS_KICK_ROOF_ROLL_DOWN:
		result = kickRoofRollDown();
		break;

	case MARIO_STATUS_KICK_ROOF:
		result = kickRoof();
		break;

	case MARIO_STATUS_HANG_ROOF:
		result = hangRoof();
		break;

	case MARIO_STATUS_WAIT_ROOF:
		result = waitRoof();
		break;

	case MARIO_STATUS_MOVE_ROOF:
		result = moveRoof();
		break;

	case MARIO_STATUS_HANGING:
		result = hanging();
		break;

	case MARIO_STATUS_ASCEND:
		result = ascend();
		break;

	case MARIO_STATUS_DESCEND:
		result = descend();
		break;

	case MARIO_STATUS_HANG_JUMPING:
		result = hangJumping();
		break;

	case MARIO_STATUS_TAKEN:
		result = taken();
		break;

	case MARIO_STATUS_WIRE_WAIT:
		result = wireWait();
		break;

	case MARIO_STATUS_WIRE_S_WAIT:
		result = wireSWait();
		break;

	case MARIO_STATUS_WIRE_WAIT_TO_S_WAIT_L:
		result = wireWaitToSWaitL();
		break;

	case MARIO_STATUS_WIRE_WAIT_TO_S_WAIT_R:
		result = wireWaitToSWaitR();
		break;

	case MARIO_STATUS_WIRE_WAIT_TO_HANG:
		result = wireWaitToHang();
		break;

	case MARIO_STATUS_WIRE_S_WAIT_TO_HANG:
		result = wireSWaitToHang();
		break;

	case MARIO_STATUS_WIRE_RETURN:
		result = wireReturn();
		break;

	case MARIO_STATUS_WIRE_HANGING:
		result = wireHanging();
		break;

	case MARIO_STATUS_WIRE_ROLLING:
		result = wireRolling();
		break;

	case MARIO_STATUS_WIRE_S_WAIT_TO_WAIT_R:
		result = wireSWaitToWaitR();
		break;

	case MARIO_STATUS_WIRE_S_WAIT_TO_WAIT_L:
		result = wireSWaitToWaitL();
		break;

	case MARIO_STATUS_PULLING:
	case MARIO_STATUS_OIL_PULLING:
		result = pulling();
		break;

	case MARIO_STATUS_FENCE_CATCH:
		result = fenceCatch();
		break;

	case MARIO_STATUS_FENCE_JUMP_CATCH:
		result = fenceJumpCatch();
		break;

	case 0x30000569:
	case MARIO_STATUS_FENCE_MOVE:
		result = fenceMove();
		break;

	case MARIO_STATUS_FENCE_PUNCH:
		result = fencePunch();
		break;
	}

	return result;
}

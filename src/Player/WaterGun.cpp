#include <Player/WaterGun.hpp>
#include <Player/NozzleTrigger.hpp>
#include <Player/NozzleBase.hpp>
#include <Player/NozzleDeform.hpp>
#include <Player/MarioAccess.hpp>

#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/JMath.hpp>

#include <System/MarDirector.hpp>
#include <System/StageUtil.hpp>
#include <System/EmitterViewObj.hpp>

#include <M3DUtil/MActor.hpp>
#include <M3DUtil/M3UModelMario.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <MSound/MSound.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

// TODO: these come from some header...
static const char cDirtyFileName[] = "/scene/map/pollution/H_ma_rak.bti";
static const char cDirtyTexName[]  = "H_ma_rak_dummy";

TNozzleBmdData nozzleBmdData = {
	{
	    {
	        0,                                          // _00
	        0,                                          // _04
	        nullptr,                                    // mHelmetPath
	        "/mario/watergun2/normal_wg",               // mPath
	        "/mario/watergun2/normal_wg/normal_wg.bmd", // mBmdPath
	        1,                                          // mNumEmitters
	        2,                                          // _15
	        {
	            { 1, 0 },
	            { 4, 0 },
	            { 4, 0 },
	        },
	    },
	    {
	        0,                                          // _00
	        0,                                          // _04
	        nullptr,                                    // mHelmetPath
	        "/mario/watergun2/rocket_wg",               // mPath
	        "/mario/watergun2/rocket_wg/rocket_wg.bmd", // mBmdPath
	        1,                                          // mNumEmitters
	        2,                                          // _15
	        {
	            { 2, 1 },
	            { 4, 0 },
	            { 4, 0 },
	        },
	    },
	    {
	        0,                                        // _00
	        0,                                        // _04
	        "/mario/bmd/wg_hel_diver.bmd",            // mHelmetPath
	        "/mario/watergun2/hover_wg",              // mPath
	        "/mario/watergun2/hover_wg/hover_wg.bmd", // mBmdPath
	        2,                                        // mNumEmitters
	        0xc,                                      // _15
	        {
	            { 1, 0 },
	            { 1, 0 },
	            { 4, 0 },
	        },
	    },
	    {
	        0,                                        // _00
	        0,                                        // _04
	        nullptr,                                  // mHelmetPath
	        "/mario/watergun2/dummy_wg",              // mPath
	        "/mario/watergun2/dummy_wg/dummy_wg.bmd", // mBmdPath
	        1,                                        // mNumEmitters
	        2,                                        // _15
	        {
	            { 3, 0 },
	            { 4, 0 },
	            { 4, 0 },
	        },
	    },
	    {
	        0,                                        // _00
	        0,                                        // _04
	        nullptr,                                  // mHelmetPath
	        "/mario/watergun2/hover_wg",              // mPath
	        "/mario/watergun2/hover_wg/hover_wg.bmd", // mBmdPath
	        2,                                        // mNumEmitters
	        0xc,                                      // _15
	        {
	            { 1, 0 },
	            { 1, 0 },
	            { 4, 0 },
	        },
	    },
	    {
	        0,                                      // _00
	        0,                                      // _04
	        nullptr,                                // mHelmetPath
	        "/mario/watergun2/back_wg",             // mPath
	        "/mario/watergun2/back_wg/back_wg.bmd", // mBmdPath
	        1,                                      // mNumEmitters
	        2,                                      // _15
	        {
	            { 1, 0 },
	            { 4, 0 },
	            { 4, 0 },
	        },
	    },
	},
};

static BOOL NozzleCtrl(J3DNode* node, BOOL param_2)
{
	// TODO: Inlined stack space
	if (!param_2) {
		if (gpMarioForCallBack != nullptr) {
			s16 gunAngle = gpMarioForCallBack->mWaterGun->getCurrentNozzle()
			                   ->getGunAngle();
			if (gunAngle < 0) {
				Mtx mtx;
				// Unused stack space
				// volatile u32 unused2[6];
				MsMtxSetRotRPH(mtx, 0.0f, 0.0f, SHORTANGLE2DEG(gunAngle));
				MTXConcat(J3DSys::mCurrentMtx, mtx, J3DSys::mCurrentMtx);
			}
		}
	}
	return true;
}

static BOOL RotateCtrl(J3DNode* node, BOOL param_2)
{
	if (!param_2 && gpMarioForCallBack != nullptr) {
		s16 local1cd0 = gpMarioForCallBack->mWaterGun->unk1CD0;
		Mtx mtx;
		// Unused stack space
		// volatile u32 unused2[7];
		MsMtxSetRotRPH(mtx, 0.005493164f * local1cd0, 0.0f, 0.0f);
		MTXConcat(J3DSys::mCurrentMtx, mtx, J3DSys::mCurrentMtx);
	}
	return true;
}

static BOOL WaterGunDivingCtrlL(J3DNode* node, BOOL param_2)
{
	if (!param_2) {
		// This looks very weird to me, probably because of some inline?
		// I could imagine some s32 getNozzleSpeedY() and
		// s16 localXXX = -getNozzleSpeedY();
		s32 nozzleSpeedY = gpMarioForCallBack->mWaterGun->unk1CC8;
		s16 neg          = -nozzleSpeedY;
		Mtx mtx;
		// Unused stack space
		// volatile u32 unused2[7];
		MsMtxSetRotRPH(mtx, 0.0f, 0.0f, 0.005493164f * neg);
		MTXConcat(J3DSys::mCurrentMtx, mtx, J3DSys::mCurrentMtx);
	}
	return true;
}

static BOOL WaterGunDivingCtrlR(J3DNode* node, BOOL param_2)
{
	if (!param_2) {
		// This looks very weird to me, probably because of some inline?
		// I could imagine some s32 getNozzleSpeedY() and
		// s16 localXXX = -getNozzleSpeedY();
		s32 nozzleSpeedY = gpMarioForCallBack->mWaterGun->unk1CCC;
		s16 neg          = -nozzleSpeedY;
		Mtx mtx;
		// Unused stack space
		// volatile u32 unused2[7];
		MsMtxSetRotRPH(mtx, 0.0f, 0.0f, 0.005493164f * neg);
		MTXConcat(J3DSys::mCurrentMtx, mtx, J3DSys::mCurrentMtx);
	}
	return true;
}

// Not sure why this get's inlined aggressively
#pragma dont_inline on
TNozzleBase::TNozzleBase(const char* name, const char* prm, TWaterGun* fludd)
    : mEmitParams(prm)
    , mFludd(fludd)
{
	mEmitParams.load(mEmitParams.mPrmPath);
	unk36C = 2;
	unk36E = 0;
	unk372 = 0;
	unk378 = 0.0f;
	unk37C = 0.0f;
}
#pragma dont_inline off

void TNozzleBase::init()
{
	unk36C = 2;
	unk36E = 0;
	unk372 = 0;
	unk378 = 0.0f;
	unk37C = 0.0f;
}

void TNozzleBase::calcGunAngle(const TMarioControllerWork& work)
{
	// volatile u32 unused1[17];
	if (mFludd->mMario == gpMarioAddress
	    && (gpCamera->isLButtonCameraSpecifyMode(gpCamera->mMode)
	        || gpCamera->isJetCoaster1stCamera())) {
		unk36E = gpCamera->unkA4;
		return;
	}

	s16 angle;
	if (mFludd->mMario->mAction == 0xC008220) {
		// TODO: Wrong reguster used, using r3 instead of r4
		angle = unk36E
		        + (s16)(mFludd->mMario->mGamePad->mCompSPos[0 * 2 + 1]
		                * mEmitParams.mRButtonMult.get());
	} else {
		angle = -mEmitParams.mLAngleBase.get();
	}

	if (angle < mEmitParams.mLAngleMin.get()) {
		angle = mEmitParams.mLAngleMin.get();
	}

	if (angle > mEmitParams.mLAngleMax.get()) {
		angle = mEmitParams.mLAngleMax.get();
	}

	f32 diff = angle - unk36E;
	unk36E += diff * mEmitParams.mLAngleChase.get();
}

void TNozzleBase::movement(const TMarioControllerWork& controllerWork)
{
	if (mFludd->mCurrentWater <= 0) {
		return;
	}
	s32 var1 = 256.0f * controllerWork.mAnalogR * 150.0f;

	if (var1 > unk372) {
		unk378 = (var1 - unk372) * 0.000015258789f;
		unk374 = unk378;
		unk372 += (u16)mEmitParams.mTriggerRate.get();
		if (var1 < unk372) {
			unk372 = var1;
		}
	} else {
		unk378 = 0.0f;
		unk372 = var1;
	}
	calcGunAngle(controllerWork);
}

void TNozzleBase::emitCommon(int param_1, TWaterEmitInfo* param_2)
{
	param_2->mAlive.set(
	    gpModelWaterManager->mWaterParticleTypes[mEmitParams.mType.get()]
	        ->mAlive.get());

	JGeometry::TVec3<f32> pos;
	JGeometry::TVec3<f32> dir;
	JGeometry::TVec3<f32> speed;
	mFludd->getEmitPosDirSpeed(param_1, &pos, &dir, &speed);

	// TODO: This feels wrong
	// TODO: Fix asm
	param_2->mPos.value = pos;
	param_2->mV.value   = speed;
	param_2->mDir.value = dir;

	param_2->mDirTremble  = mEmitParams.mDirTremble;
	param_2->mPowTremble  = mEmitParams.mPowTremble;
	param_2->mSize        = mEmitParams.mSize;
	param_2->mSizeTremble = mEmitParams.mSizeTremble;
	param_2->mType        = mEmitParams.mType;
	param_2->mHitRadius   = mEmitParams.mHitRadius;
	param_2->mHitHeight   = mEmitParams.mHitHeight;
}

void TNozzleBase::emit(int param_1)
{
	if (mFludd->mCurrentWater > 0 && unk378 != 0.0f) {
		TWaterEmitInfo* emitInfo = mFludd->mEmitInfo;
		emitCommon(param_1, emitInfo);

		f32 emitNum = mEmitParams.mNum.get();
		unk37C += emitNum;

		s32 local37cInt = (s32)unk37C;
		if ((s32)unk37C == 0) {
			return;
		}
		unk37C -= (f32)local37cInt;

		f32& refEmitPow  = emitInfo->mPow.value;
		s32& refEmitFlag = emitInfo->mFlag.value;

		emitInfo->mNum.set(local37cInt);
		emitInfo->mAttack = mEmitParams.mAttack;

		f32 emitPow  = mEmitParams.mEmitPow.get();
		f32 emitCtrl = mEmitParams.mEmitCtrl.get();
		emitInfo->mPow.set(emitPow * unk378 * emitCtrl
		                   + emitPow * (1.0f - emitCtrl));

		refEmitFlag = 0x40;
		if (mFludd->hasFlag(0x2)) {
			refEmitFlag = (refEmitFlag | 0x80);
		}

		s32 emittedWater = gpModelWaterManager->emitRequest(*emitInfo);

		mFludd->updateUnk1C88(emittedWater);
		if (emittedWater != 0) {
			mFludd->depleteWater(emittedWater * mEmitParams.mDecRate.get());

			f32 emitReactionPow = mEmitParams.mReactionPow.get();
			f32 reactionPow     = refEmitPow * emitReactionPow;

			// TODO: This section doesn't quite match here nor in derived
			// classes. There may be some weird inlining going on here?
			s16 faceAngleY     = mFludd->mMario->mFaceAngle.y;
			f32 dirX           = emitInfo->mDir.get().x;
			f32 dirZ           = emitInfo->mDir.get().z;
			f32 cosAngle       = JMASCos(faceAngleY);
			f32 sinAngle       = JMASSin(faceAngleY);
			f32 directionScale = (-dirX * sinAngle - dirZ * cosAngle);

			f32 velocity = reactionPow * directionScale;

			mFludd->mMario->addVelocity(velocity);

			JGeometry::TVec3<f32> const& dirVec = emitInfo->mDir.get();

			mFludd->mMario->mVel.x -= dirVec.x * reactionPow;
			mFludd->mMario->mVel.z -= dirVec.z * reactionPow;

			f32 velocityY
			    = -dirVec.y * refEmitPow * mEmitParams.mReactionY.get();
			mFludd->mMario->mVel.y += velocityY;
		}
	}
}

// TODO: This has a lot of inline functions, find them and update them
// properly
void TNozzleBase::animation(int param_1)
{
	J3DFrameCtrl* ctrl = unk380->getFrameCtrl(MActor::ANM_TYPE_BCK);

	if (param_1 != 2)
		return;

	if (mFludd->unk1D00 > 0.0f ? true : false)
		unk36C = 4;

	if (mFludd->unk1D00 < 0.0f ? true : false)
		unk36C = 3;

	switch (unk36C) {
	case 0: {

		// TODO: inline
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(4))
			mactor->setBckFromIndex(4);

		bool thing = false;

		J3DFrameCtrl* ctrl = unk380->getFrameCtrl(MActor::ANM_TYPE_BCK);

		if (ctrl->checkState(J3DFrameCtrl::STATE_COMPLETED_ONCE
		                     | J3DFrameCtrl::STATE_LOOPED_ONCE))
			thing = true;

		if (ctrl->getFrame() > (ctrl->getEnd() - 0.1f))
			thing = true;

		if (!thing)
			return;

		unk36C = 1;
		break;
	}

	case 1: {
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(2))
			mactor->setBckFromIndex(2);

		TWaterGun* fludd     = mFludd;
		bool updateAnimation = false;
		if (fludd->mCurrentWater == 0) {
			updateAnimation = false;
		} else if (!updateAnimation) {
			updateAnimation = true;
			int nozzleKind  = fludd->getCurrentNozzle()->getNozzleKind();
			if (nozzleKind == 1) {
				TNozzleTrigger* trigger
				    = (TNozzleTrigger*)fludd->getCurrentNozzle();
				if (trigger->unk385 == TNozzleTrigger::ACTIVE) {
					updateAnimation = true;
				} else {
					updateAnimation = false;
				}

			} else if (fludd->getCurrentNozzle()->unk378 > 0.0f) {
				updateAnimation = true;
			} else {
				updateAnimation = false;
			}
		}

		if (updateAnimation)
			return;

		unk36C = 2;
		break;
	}

	case 2: {
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(3))
			mactor->setBckFromIndex(3);

		TWaterGun* fludd     = mFludd;
		bool updateAnimation = false;
		if (fludd->mCurrentWater == 0) {
			updateAnimation = false;
		} else if (!updateAnimation) {
			updateAnimation = true;
			u32 nozzleKind  = fludd->getCurrentNozzle()->getNozzleKind();
			if (nozzleKind == 1) {
				TNozzleTrigger* trigger
				    = (TNozzleTrigger*)fludd->getCurrentNozzle();
				if (trigger->unk385 == TNozzleTrigger::ACTIVE) {
					updateAnimation = true;
				} else {
					updateAnimation = false;
				}

			} else if (fludd->getCurrentNozzle()->unk378 > 0.0f) {
				updateAnimation = true;
			} else {
				updateAnimation = false;
			}
		}

		if (updateAnimation == true)
			unk36C = 0;
		break;
	}

	case 3: {
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(1))
			mactor->setBckFromIndex(1);

		// Use external tween value
		ctrl->setFrame(-(2.0f * (mFludd->unk1CFC - 0.5f) - 1.0f)
		               * ctrl->getEnd());
		ctrl->setRate(0.0f);
		break;
	}

	case 4:
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(0))
			mactor->setBckFromIndex(0);

		// Use external tween value
		ctrl->setFrame(2.0f * (mFludd->unk1CFC - 0.5f) * ctrl->getEnd());
		ctrl->setRate(0.0f);

		if (mFludd->unk1CFC >= 1.0f)
			unk36C = 0;

		break;
	}
}

void TNozzleTrigger::init()
{
	unk384 = false;
	unk385 = TNozzleTrigger::INACTIVE;
	unk36C = 0;
	unk386 = 0;
	unk388 = 0.0f;
}

void TNozzleTrigger::movement(const TMarioControllerWork& controllerWork)
{
	if (mFludd->mCurrentWater <= 0) {
		unk385 = TNozzleTrigger::INACTIVE;
		unk386 = 0;
		unk388 = 0.0f;
		return;
	}

	if (unk385 == TNozzleTrigger::ACTIVE) {
		unk386 -= 1;

		// Very likely an inline
		bool check;
		if (mFludd->mMario->unk380 == 0) {
			check = true;
		} else {
			check = false;
		}
		if (!check || unk386 <= 0) {
			unk385 = TNozzleTrigger::DEAD;
			unk388 = 0.0f;
			unk386 = 0;
		}
	}
	// Spam spray sound?
	if ((unk384 == true
	     && (controllerWork.mFrameInput & TMarioControllerWork::A) != 0
	     && (controllerWork.mInput & TMarioControllerWork::R) != 0)
	    && unk385 == TNozzleTrigger::INACTIVE) {
		unk385 = TNozzleTrigger::ACTIVE;
		if (unk38C != 0xffffffff) {
			u32 soundId;
			if (unk378 < 1.0f) {
				soundId = 0x806;
			} else {
				soundId = 0x805;
			}
			SMSGetMSound()->startSoundActor(soundId, mFludd->mEmitPos[0], 0,
			                                nullptr, 0, 4);
		}
		unk386 = mEmitParams.mTriggerTime.get();
	}

	bool canSpray = true;

	if (!(mFludd->mMario->unk380 == 0 ? true : false))
		canSpray = false;

	if ((mFludd->mMario->unk118 & 0x30000 ? true : false) == true
	    && mFludd->mCurrentWater < mEmitParams.mAmountMax.get())
		canSpray = false;

	if (canSpray == true) {
		unk388 += 150.0f * controllerWork.mAnalogR;
		if (!unk384 && unk385 == TNozzleTrigger::INACTIVE) {
			if (gpMarDirector->unk58 % (int)mFludd->mMario->unk568 == 0)
				SMSRumbleMgr->start(20, (int)mFludd->mMario->unk564,
				                    (f32*)nullptr);
		}
		if (unk384 && unk385 == TNozzleTrigger::INACTIVE
		    && controllerWork.mAnalogR > 0.0f) {
			SMSGetMSound()->startSoundActor(0x4022, mFludd->mEmitPos[0], 0,
			                                nullptr, 0, 4);
		}
	}
	unk388 -= mEmitParams.mInsidePressureDec.get();
	if (unk388 < 0.0f) {
		unk388 = 0.0f;
	}

	if (unk388 > mEmitParams.mInsidePressureMax.get()) {
		unk388 = mEmitParams.mInsidePressureMax.get();
		if (!unk384 && unk385 == TNozzleTrigger::INACTIVE) {
			unk385      = TNozzleTrigger::ACTIVE;
			unk386      = mEmitParams.mTriggerTime.get();
			u32 soundId = unk38C;
			if (soundId != 0xffffffff) {
				SMSGetMSound()->startSoundActor(soundId, &mFludd->mEmitPos[0],
				                                0, nullptr, 0, 4);
			}
			if (mFludd->mCurrentNozzle == (s8)TWaterGun::Hover) {
				SMSRumbleMgr->start((int)0x15, 0x8, (f32*)nullptr);
			}
			if (mFludd->mCurrentNozzle == (s8)TWaterGun::Rocket
			    || mFludd->mCurrentNozzle == (s8)TWaterGun::Turbo) {
				SMSRumbleMgr->start((int)0x15, 0x14, (f32*)nullptr);
			}
		}
	}

	if (unk385 == TNozzleTrigger::DEAD) {
		unk388 = 0.0f;
		if (controllerWork.mAnalogR == 0.0f) {
			unk385 = TNozzleTrigger::INACTIVE;
		}
	}

	calcGunAngle(controllerWork);
}

void TNozzleTrigger::emit(int param_1)
{
	if (mFludd->mCurrentWater > 0 && (u8)unk385 == TNozzleTrigger::ACTIVE) {
		TWaterEmitInfo* emitInfo = mFludd->mEmitInfo;
		emitCommon(param_1, emitInfo);

		f32 triggerFill       = unk388;
		f32 insidePressureMax = mEmitParams.mInsidePressureMax.get();
		f32 emitNumMin        = mEmitParams.mNumMin.get();
		f32 emitNum           = mEmitParams.mNum.get();

		f32 pressure = triggerFill / insidePressureMax;

		unk37C += pressure * (emitNum - emitNumMin) + emitNumMin;

		s32 local37cInt = (s32)unk37C;
		if ((s32)unk37C == 0) {
			return;
		}
		unk37C -= (f32)local37cInt;

		emitInfo->mNum.set(local37cInt);

		f32& refEmitPow  = emitInfo->mPow.value;
		s32& refEmitFlag = emitInfo->mFlag.value;

		s16 attackMin = mEmitParams.mAttackMin.get();
		s16 attack    = mEmitParams.mAttack.get();
		emitInfo->mAttack.set(pressure * (f32)(attack - attackMin)
		                      + (f32)attackMin);

		f32 emitPowMin = mEmitParams.mEmitPowMin.get();
		f32 emitPow    = mEmitParams.mEmitPow.get();
		emitInfo->mPow.set(pressure * (emitPow - emitPowMin) + emitPowMin);

		refEmitFlag = 0x40;
		if (mFludd->hasFlag(0x2)) {
			refEmitFlag = (refEmitFlag | 0x80);
		}

		u8 emittedWater = gpModelWaterManager->emitRequest(*emitInfo);
		mFludd->updateUnk1C88(emittedWater);

		if (emittedWater != 0) {
			mFludd->depleteWater(emittedWater * mEmitParams.mDecRate.get());

			if ((mFludd->mCurrentNozzle == TWaterGun::Hover)
			    && ((gpMarDirector->unk58 & 0x7u) == 0u)) {
				SMSRumbleMgr->start(20, 2, (f32*)nullptr);
			}

			f32 reactionPowMin = mEmitParams.mReactionPowMin.get();
			f32 reactionPow    = mEmitParams.mReactionPow.get();

			f32 reaction
			    = pressure * (reactionPow - reactionPowMin) + reactionPowMin;

			s16 faceAngleY     = mFludd->mMario->mFaceAngle.y;
			f32 dirX           = emitInfo->mDir.get().x;
			f32 dirZ           = emitInfo->mDir.get().z;
			f32 cosAngle       = JMASCos(faceAngleY);
			f32 sinAngle       = JMASSin(faceAngleY);
			f32 directionScale = (-dirX * sinAngle - cosAngle * dirZ);

			f32 velocity = reaction;
			velocity *= directionScale;
			velocity *= refEmitPow;

			mFludd->mMario->addVelocity(velocity);

			JGeometry::TVec3<f32> const& dirVec = emitInfo->mDir.get();
			f32 accelY = -dirVec.y * refEmitPow * mEmitParams.mReactionY.get();
			mFludd->mMario->mVel.y += accelY;
		}
	}
}

void TNozzleTrigger::animation(int param_1)
{
	// TODO: stack size likely influenced by inlined temporaries in original.
	// volatile u32 unused[38];

	int bckIdleOut;
	int bckIdle;
	int bckStart;
	int bckSwapOut;
	int bckSwapIn;
	int emitMtxCount;

	J3DFrameCtrl* ctrl = unk380->getFrameCtrl(MActor::ANM_TYPE_BCK);

	switch (param_1) {
	case 4:
		bckIdleOut   = 4;
		bckIdle      = 2;
		bckStart     = 3;
		bckSwapOut   = 1;
		bckSwapIn    = 0;
		emitMtxCount = 2;
		break;
	case 1:
		bckIdleOut   = 4;
		bckIdle      = 2;
		bckStart     = 3;
		bckSwapOut   = 1;
		bckSwapIn    = 0;
		emitMtxCount = 1;
		break;
	case 5:
		bckIdleOut   = 4;
		bckIdle      = 2;
		bckStart     = 3;
		bckSwapOut   = 1;
		bckSwapIn    = 0;
		emitMtxCount = 1;
		break;
	default:
		return;
	}

	if (mFludd->unk1D00 > 0.0f ? true : false)
		unk36C = 4;

	if (mFludd->unk1D00 < 0.0f ? true : false)
		unk36C = 3;

	switch (unk36C) {
	case 0: {
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(bckIdleOut))
			mactor->setBckFromIndex(bckIdleOut);

		bool finished = false;

		J3DFrameCtrl* frameCtrl = unk380->getFrameCtrl(MActor::ANM_TYPE_BCK);
		if (frameCtrl->checkState(J3DFrameCtrl::STATE_COMPLETED_ONCE
		                          | J3DFrameCtrl::STATE_LOOPED_ONCE))
			finished = true;

		if (frameCtrl->getFrame() > (frameCtrl->getEnd() - 0.1f))
			finished = true;

		if (finished)
			unk36C = 1;

		break;
	}

	case 1: {
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(bckIdle))
			mactor->setBckFromIndex(bckIdle);

		u8 updateAnimation;
		TWaterGun* fludd = mFludd;
		if (fludd->mCurrentWater == 0) {
			updateAnimation = false;
		} else {
			if (fludd->getNozzle(fludd->mCurrentNozzle)->getNozzleKind() == 1) {
				if (((TNozzleTrigger*)fludd->getNozzle(fludd->mCurrentNozzle))
				        ->unk385
				    == ACTIVE) {
					updateAnimation = true;
				} else {
					updateAnimation = false;
				}
			} else {
				if (fludd->getNozzle(fludd->mCurrentNozzle)->unk378 > 0.0f) {
					updateAnimation = true;
				} else {
					updateAnimation = false;
				}
			}
		}

		if (!updateAnimation)
			unk36C = 2;

		break;
	}

	case 2: {
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(bckStart))
			mactor->setBckFromIndex(bckStart);

		u8 updateAnimation;
		TWaterGun* fludd = mFludd;
		if (fludd->mCurrentWater == 0) {
			updateAnimation = false;
		} else {
			if (fludd->getNozzle(fludd->mCurrentNozzle)->getNozzleKind() == 1) {
				if (((TNozzleTrigger*)fludd->getNozzle(fludd->mCurrentNozzle))
				        ->unk385
				    == ACTIVE) {
					updateAnimation = true;
				} else {
					updateAnimation = false;
				}
			} else {
				if (fludd->getNozzle(fludd->mCurrentNozzle)->unk378 > 0.0f) {
					updateAnimation = true;
				} else {
					updateAnimation = false;
				}
			}
		}

		if (updateAnimation == true)
			unk36C = 0;

		break;
	}

	case 3: {
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(bckSwapOut))
			mactor->setBckFromIndex(bckSwapOut);

		// Use external tween value
		ctrl->setFrame(-(2.0f * (mFludd->unk1CFC - 0.5f) - 1.0f)
		               * ctrl->getEnd());
		ctrl->setRate(0.0f);
		break;
	}

	case 4: {
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(bckSwapIn))
			mactor->setBckFromIndex(bckSwapIn);

		// Use external tween value
		ctrl->setFrame(2.0f * (mFludd->unk1CFC - 0.5f) * ctrl->getEnd());
		ctrl->setRate(0.0f);

		if (mFludd->unk1CFC >= 1.0f)
			unk36C = 0;

		break;
	}
	}

	if (mFludd->mIsEmitWater != 0) {
		for (int i = 0; i < emitMtxCount; ++i) {
			if (mFludd->getEmitMtx(i) != nullptr) {
				gpMarioParticleManager->emitAndBindToMtxPtr(
				    0x10D, mFludd->getEmitMtx(i), 1, &this[i]);
			}
		}
	}
}

void TNozzleDeform::movement(const TMarioControllerWork& controllerWork)
{
	if (!mFludd->hasWater()) {
		return;
	}

	TNozzleBase::movement(controllerWork);

	unk378 *= mEmitParams.mEmitPowScale.get();

	if (unk378 > 1.0f) {
		unk378 = 1.0f;
	}

	mBomb.movement(controllerWork);
}

void TNozzleDeform::emit(int param_1)
{

	if (param_1 == TWaterGun::Yoshi && mFludd->mMario->mYoshi->mType == 0) {
		return;
	}

	if (!((f32)mFludd->mCurrentWater > 0.0f)) {
		return;
	}

	if (mBomb.unk385 == TNozzleTrigger::INACTIVE && unk378 > 0.0f) {
		TWaterEmitInfo* emitInfo = mFludd->mEmitInfo;
		emitCommon(param_1, emitInfo);

		f32 localUnk378 = unk378;

		f32 emitNum    = mEmitParams.mNum.get();
		f32 emitNumMin = mEmitParams.mNumMin.get();
		unk37C += localUnk378 * (emitNum - emitNumMin) + emitNumMin;

		s32 local37cInt = (s32)unk37C;
		if ((s32)unk37C == 0) {
			return;
		}
		unk37C -= (f32)local37cInt;

		emitInfo->mNum.set(local37cInt);

		f32& refEmitPow  = emitInfo->mPow.value;
		s32& refEmitFlag = emitInfo->mFlag.value;

		s16 attackMin = mEmitParams.mAttackMin.get();
		s16 attack    = mEmitParams.mAttack.get();
		emitInfo->mAttack.set(localUnk378 * (f32)(attack - attackMin)
		                      + (f32)attackMin);

		f32 dirTrembleMin = mEmitParams.mDirTrembleMin.get();
		f32 dirTremble    = mEmitParams.mDirTremble.get();
		emitInfo->mDirTremble.set(localUnk378 * (dirTremble - dirTrembleMin)
		                          + dirTrembleMin);

		f32 emitPowMin = mEmitParams.mEmitPowMin.get();
		f32 emitPow    = mEmitParams.mEmitPow.get();
		emitInfo->mPow.set(localUnk378 * (emitPow - emitPowMin) + emitPowMin);

		refEmitFlag = 0x40;
		if (mFludd->hasFlag(0x2)) {
			refEmitFlag = (refEmitFlag | 0x80);
		}

		f32 sizeMinPressure = mEmitParams.mSizeMinPressure.get();
		f32 sizeMin         = mEmitParams.mSizeMin.get();
		f32 size            = mEmitParams.mSize.get();
		f32 sizeMaxPressure = mEmitParams.mSizeMaxPressure.get();

		f32 emitSizeLerp;
		if (localUnk378 < sizeMinPressure) {
			emitSizeLerp = 0.0f;
		} else {
			if (localUnk378 < sizeMaxPressure) {
				emitSizeLerp = (sizeMinPressure - localUnk378)
				               / (sizeMaxPressure - localUnk378);
			} else {
				emitSizeLerp = 1.0f;
			}
		}

		emitInfo->mSize.set(emitSizeLerp * (size - sizeMin) + sizeMin);

		u8 emittedWater = gpModelWaterManager->emitRequest(*emitInfo);

		mFludd->updateUnk1C88(emittedWater);

		if (emittedWater != 0) {
			mFludd->depleteWater(emittedWater * mEmitParams.mDecRate.get());

			if ((gpMarDirector->unk58 & 0x7u) == 0u) {
				SMSRumbleMgr->start(20, 2, (f32*)nullptr);
			}

			f32 reactionPowMin = mEmitParams.mReactionPowMin.get();
			f32 reactionPow    = mEmitParams.mReactionPow.get();

			f32 reaction
			    = localUnk378 * (reactionPow - reactionPowMin) + reactionPowMin;

			s16 faceAngleY     = mFludd->mMario->mFaceAngle.y;
			f32 dirX           = emitInfo->mDir.get().x;
			f32 dirZ           = emitInfo->mDir.get().z;
			f32 cosAngle       = JMASCos(faceAngleY);
			f32 sinAngle       = JMASSin(faceAngleY);
			f32 directionScale = (-dirX * sinAngle - cosAngle * dirZ);

			f32 velocity = reaction;
			velocity *= directionScale;
			velocity *= refEmitPow;

			mFludd->mMario->addVelocity(velocity);

			JGeometry::TVec3<f32> const& dirVec = emitInfo->mDir.get();
			f32 accelY = -dirVec.y * refEmitPow * mEmitParams.mReactionY.get();
			mFludd->mMario->mVel.y += accelY;
		}
	}
	mBomb.emit(param_1);
}

void TNozzleDeform::animation(int param)
{
	volatile u8 stackPad[0x118];
	(void)stackPad;

	bool check = 0;
	if (param == 0)
		check = 1;

	if (param == 3)
		check = 1;

	if (!check)
		return;

	if (param == 3)
		return;

	if (gpMarDirector->unk124 == 3)
		return;

	if (mFludd->unk1D00 > 0.0f ? true : false) {
		if (unk36C != 4 && unk36C != 6) {
			if (mFludd->unk1CEC > 0.5f) {
				unk36C = 4;
			} else {
				unk36C = 6;
			}
		}
	} else {
		if (mFludd->unk1D00 < 0.0f ? true : false) {
			if (unk36C != 5 && unk36C != 7) {
				if (mFludd->unk1CEC > 0.5f) {
					unk36C = 5;
				} else {
					unk36C = 7;
				}
			}
		} else {
			if (mFludd->unk1CEC > 0.0f) {
				unk36C = 0;
			} else if (unk36C == 0) {
				unk36C = 2;
			}
		}
	}

	J3DFrameCtrl* ctrl = unk380->getFrameCtrl(MActor::ANM_TYPE_BCK);

	switch (unk36C) {
	case 0: {
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(4))
			mactor->setBckFromIndex(4);

		ctrl->setFrame(ctrl->getEnd() * mFludd->unk1CEC);
		break;
	}
	case 1:
		break;
	case 2: {
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(7))
			mactor->setBckFromIndex(7);

		bool finished           = 0;
		J3DFrameCtrl* frameCtrl = unk380->getFrameCtrl(MActor::ANM_TYPE_BCK);
		if (frameCtrl->checkState(J3DFrameCtrl::STATE_COMPLETED_ONCE
		                          | J3DFrameCtrl::STATE_LOOPED_ONCE)) {
			finished = 1;
		}

		if (frameCtrl->getFrame() > (frameCtrl->getEnd() - 0.1f)) {
			finished = 1;
		}

		if (finished) {
			unk36C = 3;
		}
		break;
	}
	case 3: {
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(5))
			mactor->setBckFromIndex(5);

		bool updateAnimation = false;
		if (mFludd->mCurrentWater == 0) {
			updateAnimation = false;
		} else if (mFludd->getNozzle(mFludd->mCurrentNozzle)->getNozzleKind()
		           == 1) {
			if (((TNozzleTrigger*)mFludd->getNozzle(mFludd->mCurrentNozzle))
			        ->unk385
			    == TNozzleTrigger::ACTIVE)
				updateAnimation = true;
			else
				updateAnimation = false;
		} else {
			if (mFludd->getNozzle(mFludd->mCurrentNozzle)->unk378 > 0.0f)
				updateAnimation = true;
			else
				updateAnimation = false;
		}

		if (!updateAnimation)
			unk36C = 8;

		break;
	}
	case 8: {
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(6))
			mactor->setBckFromIndex(6);

		bool updateAnimation = false;
		if (mFludd->mCurrentWater == 0) {
			updateAnimation = false;
		} else if (mFludd->getCurrentNozzle()->getNozzleKind() == 1) {
			if (((TNozzleTrigger*)mFludd->getCurrentNozzle())->unk385
			    == TNozzleTrigger::ACTIVE)
				updateAnimation = true;
			else
				updateAnimation = false;
		} else {
			if (mFludd->getCurrentNozzle()->unk378 > 0.0f)
				updateAnimation = true;
			else
				updateAnimation = false;
		}

		if (updateAnimation == true)
			unk36C = 2;

		bool finished           = false;
		J3DFrameCtrl* frameCtrl = unk380->getFrameCtrl(MActor::ANM_TYPE_BCK);
		if (frameCtrl->checkState(J3DFrameCtrl::STATE_COMPLETED_ONCE
		                          | J3DFrameCtrl::STATE_LOOPED_ONCE))
			finished = true;

		if (frameCtrl->getFrame() > (frameCtrl->getEnd() - 0.1f))
			finished = true;

		if (finished && !(mFludd->unk1CEC == 0.0f ? true : false))
			unk36C = 0;

		break;
	}
	case 4: {
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(1))
			mactor->setBckFromIndex(1);

		ctrl->setFrame(2.0f * mFludd->unk1CFC * ctrl->getEnd());
		ctrl->setRate(0.0f);
		break;
	}
	case 5: {
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(0))
			mactor->setBckFromIndex(0);

		ctrl->setFrame((1.0f - 2.0f * mFludd->unk1CFC) * ctrl->getEnd());
		ctrl->setRate(0.0f);
		if (mFludd->unk1CFC <= 0.0f) {
			unk36C          = 0;
			mFludd->unk1CEC = 0.0f;
		}
		break;
	}
	case 6: {
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(3))
			mactor->setBckFromIndex(3);

		ctrl->setFrame(2.0f * mFludd->unk1CFC * ctrl->getEnd());
		ctrl->setRate(0.0f);
		break;
	}
	case 7:
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(2))
			mactor->setBckFromIndex(2);

		ctrl->setFrame((1.0f - 2.0f * mFludd->unk1CFC) * ctrl->getEnd());
		ctrl->setRate(0.0f);
		if (mFludd->unk1CFC <= 0.0f) {
			unk36C          = 2;
			mFludd->unk1CEC = 0.0f;
		}
		break;
	}

	if (mFludd->mIsEmitWater != 0) {
		if (mFludd->getEmitMtx(0) != nullptr) {
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    0x10D, mFludd->getEmitMtx(0), 1, this);
		}
	}
}

TWaterGun::TWaterGun(TMario* mario)
    : mNozzleDeform("normal_wg", "/Mario/WaterGun/NozzleDeform.prm", this)
    , mNozzleRocket(nullptr, "/Mario/WaterGun/NozzleTrgRocket.prm", this)
    , mNozzleUnderWater("hover_wg", "/Mario/WaterGun/NozzleDiving.prm", this)
    , mNozzleYoshiDeform("dummy_wg", "/Mario/WaterGun/NozzleYoshiMouth.prm",
                         this)
    , mNozzleHover("hover_wg", "/Mario/WaterGun/NozzleTrgHover.prm", this)
    , mNozzleTurbo("back_wg", "/Mario/WaterGun/NozzleTrgTurbo.prm", this)
    , mWatergunParams("/Mario/WaterGun.prm")
{
	mWatergunParams.load(mWatergunParams.mPrmPath);
	mMario = mario;
}

void TWaterGun::init()
{
	mFlags                     = 0;
	mNozzleList[Spray]         = &mNozzleDeform;
	mNozzleList[Rocket]        = &mNozzleRocket;
	mNozzleList[Underwater]    = &mNozzleUnderWater;
	mNozzleList[Yoshi]         = &mNozzleYoshiDeform;
	mNozzleList[Hover]         = &mNozzleHover;
	mNozzleList[Turbo]         = &mNozzleTurbo;
	mCurrentNozzle             = Spray;
	mSecondNozzle              = Hover;
	mNozzleRocket.unk38C       = 0x81b;
	mNozzleTurbo.unk38C        = 0x814;
	mNozzleDeform.mBomb.unk38C = 0x813;
	mCurrentWater = mNozzleList[mCurrentNozzle]->mEmitParams.mAmountMax.get();
	mIsEmitWater  = false;
	unk1C88       = 0.0f;
	mCurrentPressure  = 0;
	mPreviousPressure = 0;
	unk1CEC           = 1.0f;
	unk1CF0           = 0.1f;
	unk1CF4           = 0.0049999999f;
	unk1CF8           = 0x168;
	unk1CFA           = 0;
	unk1CFC           = 0.0f;
	unk1D00           = 0.0f;
	unk1D04           = 0;
	unk1D06           = -0x1800;
	unk1D08           = 0;

	mEmitInfo = new TWaterEmitInfo("/Mario/GunEmit.prm");

	unk1D08                         = 0;
	mNozzleDeform.mBomb.unk384      = true;
	mNozzleYoshiDeform.mBomb.unk384 = true;

	// TODO: wrong
	MtxPtr r24 = mMario->mModel->unk8->getAnmMtx(mMario->mBoneIDs[0]);

	mEmitPos[3] = mMario->mPosition;

	unk1CC0 = 0;
	unk1CC2 = 0;
	unk1CC4 = 0;

	unk1CC8 = 0.0f;
	unk1CCC = 0.0f;
	unk1CD0 = 0;
	unk1CD2 = 0;

	// This is definitely an inlined funciton. Creating a model seems quite
	// useful
	// TODO: Check if already exists
	MActorAnmData* watergunAnmData = new MActorAnmData();
	watergunAnmData->init("/mario/watergun2/body", nullptr);
	mFluddModel = new MActor(watergunAnmData);

	void* fluddModelData
	    = JKRFileLoader::getGlbResource("/mario/watergun2/body/wg_mdl1.bmd");
	J3DModel* fluddModel = new J3DModel(
	    J3DModelLoaderDataBase::load(fluddModelData, 0x10040000), 0, 1);
	mFluddModel->setModel(fluddModel, 0);

	MTXCopy(mMario->mModel->unk8->getAnmMtx(mMario->mBoneIDs[0]),
	        mFluddModel->unk4->unk20);

	mFluddModel->unk4->calc();

	u16 handleIdx
	    = mFluddModel->unk4->mModelData->unkB0->getIndex("jnt_G_handle");

	{
		unk1CDC            = new TMultiMtxEffect;
		unk1CDC->mNumBones = 2;

		u16* boneIds      = new u16[2];
		boneIds[0]        = 0;
		boneIds[1]        = handleIdx;
		unk1CDC->mBoneIDs = boneIds;

		u8* mtxEffectTypes      = new u8[2];
		mtxEffectTypes[0]       = 0;
		mtxEffectTypes[1]       = 1;
		unk1CDC->mMtxEffectType = mtxEffectTypes;

		unk1CDC->setup(mFluddModel->getModel(), "Mario/WaterGun");
	}

	unk1CD8 = mFluddModel->unk4->mModelData->unkB0->getIndex("nozzle_center");

	for (int i = 0; i < 6; ++i) {
		if (nozzleBmdData.getPath(i)) {

			MActorAnmData* nozzleData = new MActorAnmData();
			nozzleData->init(nozzleBmdData.getPath(i), nullptr);
			mNozzleList[i]->unk380 = new MActor(nozzleData);

			void* nozzleModelData
			    = JKRFileLoader::getGlbResource(nozzleBmdData.getBmdPath(i));
			J3DModel* nozzleModel = new J3DModel(
			    J3DModelLoaderDataBase::load(nozzleModelData, 0x10040000), 0,
			    1);
			mNozzleList[i]->unk380->setModel(nozzleModel, 0);

			J3DModelData* modelData
			    = mNozzleList[i]->unk380->getModel()->getModelData();

			SMS_ChangeTextureAll(modelData, "H_watergun_main_dummy",
			                     *mFluddModel->getModel()
			                          ->getModelData()
			                          ->getTexture()
			                          ->getResTIMG(0));

			mNozzleList[i]->unk380->initDL();

			// Definitely inline potential
			if (nozzleBmdData.getFlags(i, 0) != 4) {
				s32 jointIdx
				    = modelData->getJointName()->getIndex("null_G_muzzle");
				nozzleBmdData.setJointIndex(i, 0, jointIdx);
			}
			if (nozzleBmdData.getFlags(i, 1) != 4) {
				s32 jointIdx
				    = modelData->getJointName()->getIndex("null_G_muzzle2");
				nozzleBmdData.setJointIndex(i, 1, jointIdx);
			}
			if (nozzleBmdData.getFlags(i, 2) != 4) {
				s32 jointIdx
				    = modelData->getJointName()->getIndex("null_G_muzzle3");
				nozzleBmdData.setJointIndex(i, 2, jointIdx);
			}
		} else {
			mNozzleList[i]->unk380 = nullptr;
		}
	}

	mNozzleList[Spray]
	    ->unk380->getModel()
	    ->getModelData()
	    ->getJointNodePointer(mNozzleList[Spray]
	                              ->unk380->getModel()
	                              ->getModelData()
	                              ->getJointName()
	                              ->getIndex("chn_muzzle_l"))
	    ->setCallBack(&NozzleCtrl);

	mNozzleList[Spray]
	    ->unk380->getModel()
	    ->getModelData()
	    ->getJointNodePointer(mNozzleList[Spray]
	                              ->unk380->getModel()
	                              ->getModelData()
	                              ->getJointName()
	                              ->getIndex("jnt_nozzle_L"))
	    ->setCallBack(&WaterGunDivingCtrlL);

	mNozzleList[Spray]
	    ->unk380->getModel()
	    ->getModelData()
	    ->getJointNodePointer(mNozzleList[Spray]
	                              ->unk380->getModel()
	                              ->getModelData()
	                              ->getJointName()
	                              ->getIndex("jnt_nozzle_R"))
	    ->setCallBack(&WaterGunDivingCtrlR);

	mNozzleList[Spray]
	    ->unk380->getModel()
	    ->getModelData()
	    ->getJointNodePointer(mNozzleList[Spray]
	                              ->unk380->getModel()
	                              ->getModelData()
	                              ->getJointName()
	                              ->getIndex("chn_back_nozzle_prop"))
	    ->setCallBack(&RotateCtrl);

	mNozzleList[Spray]
	    ->unk380->getModel()
	    ->getModelData()
	    ->getJointNodePointer(mNozzleList[Spray]
	                              ->unk380->getModel()
	                              ->getModelData()
	                              ->getJointName()
	                              ->getIndex("jnt_back_nozzle_neck"))
	    ->setCallBack(&NozzleCtrl);

	mFluddModel->getModel()->setBaseTRMtx(r24);
	mFluddModel->getModel()->calc();

	unk1D10 = new TMirrorActor("水鉄砲in鏡");
	unk1D10->init(mFluddModel->unk4, 4);

	// TODO: Definitely an inlined function
	// Another function does the exact same thing
	for (int i = 0; i < nozzleBmdData.getEmitterCount(mCurrentNozzle); ++i) {
		MtxPtr emitMtx = getEmitMtx(i);
		if (emitMtx != nullptr) {
			mEmitPos[i].x = emitMtx[0][3];
			mEmitPos[i].y = emitMtx[1][3];
			mEmitPos[i].z = emitMtx[2][3];
		}
	}
}

void TWaterGun::initInLoadAfter() { }

// TODO: Do i really need to explcitly say this?
#pragma dont_inline on
MtxPtr TWaterGun::getEmitMtx(int jointIndex)
{
	MtxPtr result = nullptr;
	if (mMario->onYoshi()) {
		result = getYoshiMtx();
	} else {
		// This entire block is likely an inlined function.
		s32 flag = nozzleBmdData.getFlags(mCurrentNozzle, jointIndex);

		switch (flag) {
		case 0:
		case 1:
		case 2:
			result = getCurrentNozzle()->getMActor()->getModel()->getAnmMtx(
			    nozzleBmdData.getJointIndex(mCurrentNozzle, jointIndex));
			break;
		case 3:
			result = getYoshiMtx();
			break;
		default:
			break;
		}
	}
	return result;
}
#pragma dont_inline off

MtxPtr TWaterGun::getNozzleMtx()
{
	return mFluddModel->unk4->getAnmMtx(unk1CD8);
}

bool TWaterGun::isEmitting() { return false; }

void TWaterGun::changeNozzle(TNozzleType nozzleType, bool animate)
{
	f32 usedWater = (f32)mCurrentWater
	                / mNozzleList[mCurrentNozzle]->mEmitParams.mAmountMax.get();
	if (nozzleType == Spray) {
		if (animate == true) {
			unk1CFC = 0.0f;
		}
	} else {
		mSecondNozzle = nozzleType;
		if (animate == true) {
			unk1CFC = 1.0f;
		}
	}
	mCurrentNozzle = nozzleType;
	mNozzleList[mCurrentNozzle]->init();
	if (nozzleType == Yoshi) {
		mCurrentWater = mMario->mYoshi->_11[0];
	} else {
		mCurrentWater
		    = usedWater
		      * mNozzleList[mCurrentNozzle]->mEmitParams.mAmountMax.get();
	}
}

void TWaterGun::movement()
{
	if (!canSpray()) {
		unk1CC2 = 0;
		unk1CC4 = 0;
	}

	unk1CC8 += (unk1CC2 - unk1CC8) * mWatergunParams.mChangeSpeed.get();
	unk1CCC += (unk1CC4 - unk1CCC) * mWatergunParams.mChangeSpeed.get();

	TNozzleBase* currentNozzle = getCurrentNozzle();

	rotateProp(currentNozzle->unk378);

	// They do the same thing again?... This is the exact same code as
	// rotateProp
	if (mCurrentNozzle == Turbo) {
		unk1CD2 += mNozzleList[mCurrentNozzle]->unk378
		           * mWatergunParams.mNozzleAngleYSpeed.get();
		unk1CD2 *= mWatergunParams.mNozzleAngleYBrake.get();
		if (mWatergunParams.mHoverRotMax.get() < unk1CD2) {
			unk1CD2 = mWatergunParams.mHoverRotMax.get();
		}
		unk1CD0 = unk1CD0 + unk1CD2;
	} else {
		unk1CD2 = 0;
		unk1CD0 = 0;
	}

	// Yoshi nozzle
	if (mCurrentNozzle == 3) {
		mCurrentWater = getCurrentNozzle()->mEmitParams.mAmountMax.get();
	}

	if (SMS_isDivingMap()) {
		mCurrentWater = getCurrentNozzle()->mEmitParams.mAmountMax.get();
	}

	if (mCurrentNozzle == 3) {
		unk1CEC = 0.0f;
	}

	// Nozzle swapping
	if (unk1D00 != 0.0f) {
		f32 unk = unk1CFC;
		f32 sum = unk + unk1D00;
		unk1CFC = sum;
		if ((unk < 0.5f) && (0.5f <= sum)) {
			u8 curNozzle        = mCurrentNozzle;
			s32 currentWater    = mCurrentWater;
			u8 secondNozzle     = mSecondNozzle;
			f32 maxWater        = currentNozzle->mEmitParams.mAmountMax.get();
			f32 waterPercentage = currentWater / maxWater;

			if (secondNozzle != 0) {
				mSecondNozzle = secondNozzle;
			}
			mCurrentNozzle = secondNozzle;

			currentNozzle = getCurrentNozzle();
			currentNozzle->init(); // TODO: 2 vtable entry

			if (secondNozzle == 3) {
				mCurrentWater
				    = mMario->mYoshi->_11[0]; // TODO: Proper yoshi stuff
			} else {
				mCurrentWater = waterPercentage
				                * currentNozzle->mEmitParams.mAmountMax.get();
			}
		}
		if ((sum < 0.5f) && (0.5f <= unk)) {
			f32 currentWater    = (f32)mCurrentWater;
			f32 maxWater        = currentNozzle->mEmitParams.mAmountMax.get();
			f32 waterPercentage = currentWater / maxWater;

			mCurrentNozzle = 0;

			currentNozzle = getCurrentNozzle();
			currentNozzle->init(); // TODO: 2 vtable entry

			mCurrentWater
			    = waterPercentage * currentNozzle->mEmitParams.mAmountMax.get();
		}

		if (unk1CFC < 0.0) {
			unk1CFC = 0.0f;
			unk1D00 = 0.0f;
		}
		if (1.0f < unk1CFC) {
			unk1CFC = 1.0f;
			unk1D00 = 0.0f;
		}
	}
	currentNozzle->animation(mCurrentNozzle);
}

void TWaterGun::setBaseTRMtx(Mtx mtx)
{
	Mtx result;
	Mtx temp;

	f32 initialAngle = mtx[1][0];
	if (initialAngle < 0.0f) {
		initialAngle = -initialAngle;
	}

	// Seemingly some adjustment of fluddpack angle
	f32 baseAngle = unk1D06;
	f32 angleDiff = unk1D04 - unk1D06;

	s16 angle = initialAngle * angleDiff + baseAngle;

	f32 angleDegrees = SHORTANGLE2DEG(angle);
	MsMtxSetRotRPH(temp, 0.0f, 0.0f, angleDegrees);

	MTXConcat(mtx, temp, result);
	MTXCopy(result, mFluddModel->unk4->unk20);
}

void TWaterGun::calcAnimation(JDrama::TGraphics* graphics)
{
	gpMarioForCallBack      = mMario;
	J3DFrameCtrl* frameCtrl = mFluddModel->getFrameCtrl(MActor::ANM_TYPE_BCK);
	if (mMario == nullptr) {
		return;
	}

	// TODO: This is probably an enum.
	s32 var380 = mMario->unk380;
	if ((var380 & 0x8000) != 0) {
		var380 = 0;
	}

	// TODO: Definitely a fake match.
	// Maybe nested inlined switch statements?
	switch (var380) {

	case 1:
	case 0:
		if (unk1CEC == 0.0f) {
			if (mMario->fabricatedActionInline()) {
				mFluddModel->setBck("wg_fepmp");
			} else if (mMario->checkFlag(MARIO_FLAG_IN_SHALLOW_WATER
			                             | MARIO_FLAG_IN_WATER)) {
				mFluddModel->setBck("wg_swpmp");
			} else {
				// TODO: Cast would be weird here, probably an inlined
				// getter that converts to s32

				switch ((s32)mMario->mAnimationId) {
				case 0x33:
					mFluddModel->setBck("wg_pump");
					break;
				default:
					mFluddModel->setBck("wg_pump");
					break;
				}
			}

			(void)"wg_hgpmp"; // NOTE: from unreachable code maybe?

			frameCtrl->setRate(0.0f);
			frameCtrl->setFrame(mMario->getPumpFrame());
			unk1CFA = unk1CF8;
		} else {
			mFluddModel->setBck("wg_house");
			if ((unk1CEC > 0.0f)
			    && (unk1CEC = unk1CEC - 0.1f, unk1CEC <= 0.0f)) {
				unk1CEC = 0.0f;
			}
			frameCtrl->setRate(0.0f);
			frameCtrl->setFrame(unk1CEC * frameCtrl->getEnd());
		}
		break;

	case 5:
	default:
		if (unk1CFA == 0) {
			if (unk1CEC < 1.0f) {
				unk1CEC = unk1CEC + unk1CF4;
				mFluddModel->setBck("wg_house");
				frameCtrl->setRate(0.0f);
				frameCtrl->setFrame(unk1CEC * frameCtrl->getEnd());
			} else {
				unk1CEC = 1.0f;
				mFluddModel->setBck("wg_house");
				frameCtrl->setRate(0.0f);
				frameCtrl->setFrame(unk1CEC * frameCtrl->getEnd());
			}
		} else {
			unk1CFA = unk1CFA - 1;
		}
		break;
	}
}

void TWaterGun::perform(u32 flags, JDrama::TGraphics* graphics)
{
	// TODO: Missing stack space
	// volatile u32 unused2[24];

	if ((flags & 0x1) != 0) {
		if ((mFlags & 0x10) != 0) {
			mCurrentWater = 0;
		}
		movement();
	}

	if ((flags & 0x2) != 0) {
		calcAnimation(graphics);
	}

	mFluddModel->perform(flags, graphics);

	if ((flags & 0x2) != 0) {
		MActor* p2 = getCurrentNozzle()->unk380;
		if (p2 != nullptr) {
			p2->getModel()->setBaseTRMtx(getModel()->getAnmMtx(unk1CD8));
		}

		for (s32 index = 0;
		     index < nozzleBmdData.getEmitterCount(mCurrentNozzle); ++index) {
			MtxPtr p1 = getEmitMtx(index);
			if (p1 != nullptr) {
				mEmitPos[index].x = p1[0][3];
				mEmitPos[index].y = p1[1][3];
				mEmitPos[index].z = p1[2][3];
			}
		}
	}

	if (getCurrentNozzle()->unk380) {
		getCurrentNozzle()->unk380->perform(flags, graphics);
	}
}

TNozzleBase* TWaterGun::getCurrentNozzle() const
{
	return mNozzleList[mCurrentNozzle];
}

void TWaterGun::setAmountToRate(f32 rate)
{
	// volatile u32 unused2[7]; // TODO: possibly inlined function
	if (mCurrentNozzle == 3) {
		TNozzleBase* currentNozzle = getCurrentNozzle();
		s32 amountMax = currentNozzle->mEmitParams.mAmountMax.get();
		mCurrentWater = amountMax;
	} else {
		TNozzleBase* currentNozzle = getCurrentNozzle();
		mCurrentWater = rate * currentNozzle->mEmitParams.mAmountMax.get();
	}
}

bool TWaterGun::isPressureOn()
{
	// volatile u32 unused2[6];
	if (getCurrentNozzle()->getNozzleKind() == 1) {
		TNozzleTrigger* triggerNozzle = (TNozzleTrigger*)getCurrentNozzle();
		if (triggerNozzle->unk388 > 0.0f) {
			return true;
		}
	}
	return false;
}

f32 TWaterGun::getPressure()
{
	// TODO: Missing stack space
	// volatile u32 unused2[5];
	if (getCurrentNozzle()->getNozzleKind() == 1) {
		TNozzleTrigger* triggerNozzle = (TNozzleTrigger*)getCurrentNozzle();
		return triggerNozzle->unk388;
	}
	return 0.0f;
}

f32 TWaterGun::getPressureMax()
{
	// TODO: Missing stack space
	// volatile u32 unused2[6];

	if (getCurrentNozzle()->getNozzleKind() == 1) {
		return getCurrentNozzle()->mEmitParams.mInsidePressureMax.get();
	}

	return 0.0f;
}

// TODO: Figure out why inline happens
#pragma dont_inline on
void TWaterGun::getEmitPosDirSpeed(int index, JGeometry::TVec3<f32>* pos,
                                   JGeometry::TVec3<f32>* dir,
                                   JGeometry::TVec3<f32>* speed)
{
	// TODO: Fix unused stack space
	// volatile u32 unused2[6];

	MtxPtr nozzleEmitMtx = getEmitMtx(index);
	pos->set(mEmitPos[index]);

	if (nozzleEmitMtx != nullptr) {
		dir->x = nozzleEmitMtx[0][0];
		dir->y = nozzleEmitMtx[1][0];
		dir->z = nozzleEmitMtx[2][0];
	} else {
		dir->set(0.0f, 0.0f, 1.0f);
	}

	speed->x = mMario->mVel.x * 0.125f;
	speed->y = 0.0f;
	speed->z = mMario->mVel.z * 0.125f;
}
#pragma dont_inline off

void TWaterGun::rotateProp(f32 rotation)
{
	if (mCurrentNozzle == 5) {
		unk1CD2 += rotation * mWatergunParams.mNozzleAngleYSpeed.get();
		unk1CD2 *= mWatergunParams.mNozzleAngleYBrake.get();
		if (mWatergunParams.mHoverRotMax.get() < unk1CD2) {
			unk1CD2 = mWatergunParams.mHoverRotMax.get();
		}
		unk1CD0 = unk1CD0 + unk1CD2;
	} else {
		unk1CD2 = 0;
		unk1CD0 = 0;
	}
}

void TWaterGun::triggerPressureMovement(
    const TMarioControllerWork& controllerWork)
{
	mCurrentPressure = controllerWork.mAnalogR * 150.0f;

	TNozzleBase* currentNozzle = getCurrentNozzle();
	currentNozzle->movement(controllerWork);

	if (mCurrentPressure > mPreviousPressure) {
		mPreviousPressure = mCurrentPressure;
	} else if (mPreviousPressure != 0) {
		mPreviousPressure -= 1;
	} else {
		mPreviousPressure = 0;
	}
}
void TWaterGun::emit()
{
	// TODO: Missing stack space
	// volatile u32 unused1[25];

	// TODO: Another possible inline to check if emit is possible
	if (!mMario->checkFlag(MARIO_FLAG_HELMET_FLW_CAMERA)
	    && mMario->checkFlag(MARIO_FLAG_IN_SHALLOW_WATER
	                         | MARIO_FLAG_IN_WATER)) {
		// I can imagine this also being an inline function that checks
		// if the emit point is below the water height, but i will leave
		// it for now. TODO.
		MtxPtr nozzleEmitMtx;
		if ((nozzleEmitMtx = getEmitMtx(0)) != nullptr) {
			if (nozzleEmitMtx[1][3] < mMario->mFloorPosition.z + 20.0f) {
				return;
			}
		}
	}

	if (!mMario->onYoshi()) {
		if (unk1CEC > 0.0f) {
			return;
		}
	}

	// TODO: Probably an enum
	// TODO: Probably inline function?
	if (hasFlag(0x4)) {
		offFlag(0x4);
		return;
	}

	u8 currentNozzleType       = mCurrentNozzle;
	TNozzleBase* currentNozzle = getNozzle(currentNozzleType);
	for (int i = 0; i < nozzleBmdData.getEmitterCount(currentNozzleType); ++i) {
		currentNozzle->emit(i);
	}
	if (mCurrentWater > 0) {
		switch (currentNozzleType) {
		case Spray: {
			JGeometry::TVec3<f32>* emitPos = &mEmitPos[0];
			playSoundWithInfo(0x24, emitPos, 0, getCurrentNozzle()->unk374);
		}
		case Yoshi:
		case Turbo: {
			JGeometry::TVec3<f32>* emitPos = &mEmitPos[0];
			playSoundWithInfo(0x0, emitPos, 0, getCurrentNozzle()->unk378);
			break;
		}
		case Underwater: {
			JGeometry::TVec3<f32>* emitPos = &mEmitPos[0];
			if (gpMSound->gateCheck(0x18)) {
				MSoundSESystem::MSoundSE::startSoundActor(0x18, emitPos, 0,
				                                          nullptr, 0, 4);
			}
		} break;
		case Rocket:
			break;
		case Hover:
			if (mIsEmitWater) {
				JGeometry::TVec3<f32>* emitPos = &mEmitPos[0];
				if (gpMSound->gateCheck(0x18)) {
					MSoundSESystem::MSoundSE::startSoundActor(0x18, emitPos, 0,
					                                          nullptr, 0, 4);
				}
			}
			break;
		}
	}
}
bool TWaterGun::suck()
{
	// TODO: Missing stack space
	// volatile u32 unused1[7];
	if (mCurrentNozzle == (s8)Yoshi) {
		return false;
	} else {
		s32 suckRate = getSuckRate();
		if (suckRate > 0) {
			mCurrentWater += suckRate;

			s32 currentWater = mCurrentWater;
			s32 maxWater     = getCurrentNozzle()->mEmitParams.mAmountMax.get();
			if (currentWater > maxWater) {
				mCurrentWater = maxWater;
			}

			if (!(mCurrentWater
			      >= getCurrentNozzle()->mEmitParams.mAmountMax.get())) {
				JGeometry::TVec3<f32>* emitPos = &mEmitPos[0];
				MSound* sound                  = gpMSound;
				if (sound->gateCheck(0xf)) {
					MSoundSESystem::MSoundSE::startSoundActor(0xf, emitPos, 0,
					                                          nullptr, 0, 4);
				}
			}
			return true;
		}
	}
	return false;
}

BOOL TWaterGun::damage()
{
	if (hasWater()) {
		TNozzleBase* nozzle = getCurrentNozzle();

		mCurrentWater -= nozzle->mEmitParams.mDamageLoss.value;

		if (mCurrentWater < 0) {
			mCurrentWater = 0;
		}
		return TRUE;
	}
	return FALSE;
}

void TWaterGun::changeBackup()
{
	// TODO: Missing stack space
	// volatile u32 unused2[5];
	if (unk1CFC == 0.0f) {
		SMSGetMSound()->startSoundSystemSE(0x812, 0, nullptr, 0);
		unk1D00 = mWatergunParams.mChangeSpeed.get();
	}

	if (unk1CFC == 1.0f) {
		SMSGetMSound()->startSoundSystemSE(0x811, 0, nullptr, 0);
		unk1D00 = -mWatergunParams.mChangeSpeed.get();
	}
}

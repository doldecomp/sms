#include <Player/WaterGun.hpp>
#include <Player/NozzleTrigger.hpp>
#include <Player/NozzleBase.hpp>
#include <Player/NozzleDeform.hpp>
// #include <Player/NozzleButton.hpp>
// #include <Player/NozzleTurbo.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/Mario.hpp>
#include <Player/MarioEffect.hpp>

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

// rogue include needed for matching rodata: zero/unit Vec literals
#include <Map/MapCollisionEntry.hpp>

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
	if (!param_2 && gpMarioForCallBack != nullptr) {
		Mtx mtx;
		const TWaterGun* waterGun = gpMarioForCallBack->getWaterGun();
		s16 gunAngle              = waterGun->getCurrentNozzle()->getGunAngle();
		if (gunAngle < 0) {
			MsMtxSetRotRPH(mtx, 0.0f, 0.0f, SHORTANGLE2DEG(gunAngle));
			MTXConcat(J3DSys::mCurrentMtx, mtx, J3DSys::mCurrentMtx);
		}
	}
	return true;
}

static BOOL RotateCtrl(J3DNode* node, BOOL param_2)
{
	if (!param_2 && gpMarioForCallBack != nullptr) {
		Mtx mtx;
		s16 angle = gpMarioForCallBack->getWaterGun()->getPropellerAngle();
		// Unused stack space
		// volatile u32 unused2[2];
		MsMtxSetRotRPH(mtx, SHORTANGLE2DEG(angle), 0.0f, 0.0f);
		MTXConcat(J3DSys::mCurrentMtx, mtx, J3DSys::mCurrentMtx);
	}
	return true;
}

static BOOL WaterGunDivingCtrlL(J3DNode* node, BOOL param_2)
{
	if (!param_2) {
		Mtx mtx;
		// Unused stack space
		// volatile u32 unused2[2];
		s16 angle = -gpMarioForCallBack->getWaterGun()->getHoverAngleL();
		MsMtxSetRotRPH(mtx, 0.0f, 0.0f, SHORTANGLE2DEG(angle));
		MTXConcat(J3DSys::mCurrentMtx, mtx, J3DSys::mCurrentMtx);
	}
	return true;
}

static BOOL WaterGunDivingCtrlR(J3DNode* node, BOOL param_2)
{
	if (!param_2) {
		Mtx mtx;
		// Unused stack space
		// volatile u32 unused2[2];
		s16 angle = -gpMarioForCallBack->getWaterGun()->getHoverAngleR();
		MsMtxSetRotRPH(mtx, 0.0f, 0.0f, SHORTANGLE2DEG(angle));
		MTXConcat(J3DSys::mCurrentMtx, mtx, J3DSys::mCurrentMtx);
	}
	return true;
}

TNozzleBase::TNozzleBase(const char* name, const char* prm_path,
                         TWaterGun* fludd)
    : TParams(prm_path)
    , PARAM_INIT(mRocketType, 0)
    , PARAM_INIT(mNum, 1.0f)
    , PARAM_INIT(mAttack, 1)
    , PARAM_INIT(mDirTremble, 0.0099999998)
    , PARAM_INIT(mEmitPow, 40.0)
    , PARAM_INIT(mEmitCtrl, 1.0f)
    , PARAM_INIT(mPowTremble, 1.0f)
    , PARAM_INIT(mSize, 40.0f)
    , PARAM_INIT(mSizeTremble, 16.0f)
    , PARAM_INIT(mAmountMax, 0x834)
    , PARAM_INIT(mReactionPow, 0.0f)
    , PARAM_INIT(mReactionY, 0.0f)
    , PARAM_INIT(mDecRate, 0)
    , PARAM_INIT(mTriggerRate, 0x100)
    , PARAM_INIT(mDamageLoss, 0xfa)
    , PARAM_INIT(mSuckRate, 0.1f)
    , PARAM_INIT(mHitRadius, 50.0f)
    , PARAM_INIT(mHitHeight, 80.0f)
    , PARAM_INIT(mLAngleBase, 0x1000)
    , PARAM_INIT(mLAngleNormal, 12000)
    , PARAM_INIT(mLAngleSquat, 12000)
    , PARAM_INIT(mLAngleMin, -0x2000)
    , PARAM_INIT(mLAngleMax, 0x2000)
    , PARAM_INIT(mLAngleChase, 0.1f)
    , PARAM_INIT(mSizeMinPressure, 0.0f)
    , PARAM_INIT(mSizeMaxPressure, 1.0f)
    , PARAM_INIT(mNumMin, 1.0f)
    , PARAM_INIT(mAttackMin, 1)
    , PARAM_INIT(mDirTrembleMin, 0.0099999998)
    , PARAM_INIT(mEmitPowMin, 40.0f)
    , PARAM_INIT(mSizeMin, 40.0f)
    , PARAM_INIT(mMotorPowMin, 5.0f)
    , PARAM_INIT(mMotorPowMax, 25.0f)
    , PARAM_INIT(mReactionPowMin, 0.0f)
    , PARAM_INIT(mInsidePressureDec, 100.0f)
    , PARAM_INIT(mInsidePressureMax, 4500.0f)
    , PARAM_INIT(mTriggerTime, 1)
    , PARAM_INIT(mType, 0)
    , PARAM_INIT(mSideAngleMaxSide, 0x4000)
    , PARAM_INIT(mSideAngleMaxFront, 0x4000)
    , PARAM_INIT(mSideAngleMaxBack, 0x2000)
    , PARAM_INIT(mRButtonMult, 10000.0)
    , PARAM_INIT(mEmitPowScale, 10.0f)
    , mFludd(fludd)
{
	load(mPrmPath);

	// Possibly TNozzleBase::init(), but feels fake
	mAnimationState  = 2;
	mGunAngle        = 0;
	mTriggerPressure = 0;
	unk378           = 0.0f;
	mEmitNum         = 0.0f;
}

void TNozzleBase::init()
{
	mAnimationState  = 2;
	mGunAngle        = 0;
	mTriggerPressure = 0;
	unk378           = 0.0f;
	mEmitNum         = 0.0f;
}

void TNozzleBase::calcGunAngle(const TMarioControllerWork& work)
{
	// volatile u32 unused1[6];
	if (mFludd->getMario() == gpMarioAddress
	    && (gpCamera->isLButtonCamera() || gpCamera->isJetCoaster1stCamera())) {
		mGunAngle = gpCamera->getCurrentTarget().mPitch;
		return;
	}

	s16 targetAngle;
	if (mFludd->getMario()->isStatus(MARIO_STATUS_SQUAT)) {
		targetAngle   = mGunAngle;
		s16 diffAngle = mFludd->getMario()->getGamePad()->getCompSPos(0, 1)
		                * mRButtonMult.get();
		targetAngle += diffAngle;
	} else {
		targetAngle = -mLAngleBase.get();
	}

	if (targetAngle < mLAngleMin.get()) {
		targetAngle = mLAngleMin.get();
	}

	if (targetAngle > mLAngleMax.get()) {
		targetAngle = mLAngleMax.get();
	}

	mGunAngle += (targetAngle - mGunAngle) * mLAngleChase.get();
}

void TNozzleBase::movement(const TMarioControllerWork& controllerWork)
{
	if (mFludd->getCurrentWater() <= 0) {
		return;
	}
	s32 triggerPressure = 150.0f * controllerWork.mAnalogR * 256.0f;
	if (triggerPressure > mTriggerPressure) {
		unk378          = SHORTANGLE2DEG(triggerPressure - mTriggerPressure);
		unk374          = unk378;
		u16 triggerRate = mTriggerRate.get();
		mTriggerPressure += triggerRate;
		if (triggerPressure < mTriggerPressure) {
			mTriggerPressure = triggerPressure;
		}
	} else {
		unk378           = 0.0f;
		mTriggerPressure = triggerPressure;
	}
	calcGunAngle(controllerWork);
}

void TNozzleBase::emitCommon(int param_1, TWaterEmitInfo* param_2)
{
	const TWaterParticleType* waterParticleType
	    = gpModelWaterManager->getWaterParticleType(mType.get());
	param_2->mAlive.set(waterParticleType->getAlive());

	// volatile u32 padding2[1];
	JGeometry::TVec3<f32> pos;
	JGeometry::TVec3<f32> dir;
	JGeometry::TVec3<f32> speed;
	mFludd->getEmitPosDirSpeed(param_1, &pos, &dir, &speed);

	// TODO: This feels wrong
	// TODO: Fix asm
	param_2->mPos.set(pos);
	param_2->mV.set(speed);
	param_2->mDir.set(dir);

	param_2->mDirTremble  = mDirTremble;
	param_2->mPowTremble  = mPowTremble;
	param_2->mSize        = mSize;
	param_2->mSizeTremble = mSizeTremble;
	param_2->mType        = mType;
	param_2->mHitRadius   = mHitRadius;
	param_2->mHitHeight   = mHitHeight;
	// volatile u32 padding[1];
}

void TNozzleBase::emit(int param_1)
{
	if (mFludd->getCurrentWater() > 0 && unk378 != 0.0f) {
		TWaterEmitInfo* emitInfo = mFludd->getEmitInfo();
		emitCommon(param_1, emitInfo);

		mEmitNum += mNum.get();

		s32 emittedNum = (s32)mEmitNum;
		if ((s32)mEmitNum == 0) {
			return;
		}

		mEmitNum -= emittedNum;

		s32 flag = 0x40;

		// This is a complete fake match...
		// But idk why it would take a ref here?
		s32& flagRef = emitInfo->mFlag.value;

		emitInfo->mNum.set(emittedNum);
		emitInfo->mAttack = mAttack;

		f32 emitPow  = mEmitPow.get();
		f32 emitCtrl = mEmitCtrl.get();
		emitInfo->mPow.set(emitPow * unk378 * emitCtrl
		                   + emitPow * (1.0f - emitCtrl));

		flagRef = flag;
		if (mFludd->hasFlag(TWaterGun::WATER_GUN_FLAG_UNK2)) {
			flagRef |= 0x80;
		}
		u8 emittedWater = gpModelWaterManager->emitRequest(*emitInfo);

		mFludd->updateUnk1C88(emittedWater);
		if (emittedWater != 0) {
			mFludd->depleteWater(emittedWater * mDecRate.get());

			// TODO: This section doesn't quite match here nor in derived
			// classes. There may be some weird inlining going on here?
			TMario* mario                    = mFludd->getMario();
			const JGeometry::TVec3<f32>& dir = emitInfo->mDir.get();
			f32 reactionPow = mEmitPow.get() * mReactionPow.get();
			f32 fdot        = dir.x * JMASSin(mario->getFaceAngleY())
			           - dir.z * JMASCos(mario->getFaceAngleY());
			f32 backwardComponent = -fdot;
			mario->addVelocity(backwardComponent * mReactionPow.get());

			mFludd->getMario()->mVel.x -= dir.x * reactionPow;
			mFludd->getMario()->mVel.z -= dir.z * reactionPow;
			f32 velocityY = -dir.y * mEmitPow.get() * mReactionY.get();
			mFludd->getMario()->mVel.y += velocityY;
		}
	}
}

// Unused
bool TNozzleBase::isAnmEnd() const
{
	bool finished      = false;
	J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(ANM_TYPE_BCK);
	if (ctrl->checkState(J3DFrameCtrl::STATE_COMPLETED_ONCE
	                     | J3DFrameCtrl::STATE_LOOPED_ONCE))
		finished = true;
	if (ctrl->getFrame() > (ctrl->getEnd() - 0.1f))
		finished = true;
	return finished;
}

// TODO: This has a lot of inline functions, find them and update them
// properly
void TNozzleBase::animation(int nozzleType)
{
	// Clip indices follow the nozzle model animation data.
	int bckShootStart;
	int bckShooting;
	int bckShootEnd;
	int bckChangeStart;
	int bckChangeEnd;

	J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(ANM_TYPE_BCK);

	switch (nozzleType) {
	case TWaterGun::Underwater:
		bckShootStart  = 4;
		bckShooting    = 2;
		bckShootEnd    = 3;
		bckChangeStart = 1;
		bckChangeEnd   = 0;
		break;
	default:
		return;
	}

	if (mFludd->isSwitchingToSecondaryNozzle())
		mAnimationState = ANIM_STATE_CHANGE_END;

	if (mFludd->isSwitchingToPrimaryNozzle())
		mAnimationState = ANIM_STATE_CHANGE_START;

	switch (mAnimationState) {
	case ANIM_STATE_SHOOT_START: {

		mMActor->setBck(bckShootStart);

		if (!isAnmEnd())
			return;

		mAnimationState = ANIM_STATE_SHOOTING;
		break;
	}

	case ANIM_STATE_SHOOTING: {
		mMActor->setBck(bckShooting);

		if (mFludd->isEmitting())
			return;

		mAnimationState = ANIM_STATE_SHOOT_END;
		break;
	}

	case ANIM_STATE_SHOOT_END: {
		mMActor->setBck(bckShootEnd);

		if (mFludd->isEmitting() == true)
			mAnimationState = ANIM_STATE_SHOOT_START;
		break;
	}

	case ANIM_STATE_CHANGE_START: {
		mMActor->setBck(bckChangeStart);

		// Use external tween value
		ctrl->setFrame(-(2.0f * (mFludd->getSwitchProgress() - 0.5f) - 1.0f)
		               * ctrl->getEnd());
		ctrl->setRate(0.0f);
		break;
	}

	case ANIM_STATE_CHANGE_END:
		mMActor->setBck(bckChangeEnd);

		// Use external tween value
		ctrl->setFrame(2.0f * (mFludd->getSwitchProgress() - 0.5f)
		               * ctrl->getEnd());
		ctrl->setRate(0.0f);

		if (mFludd->getSwitchProgress() >= 1.0f)
			mAnimationState = ANIM_STATE_SHOOT_START;

		break;
	}
}

TNozzleTrigger::TNozzleTrigger(const char* name, const char* prm_path,
                               TWaterGun* fludd)
    : TNozzleBase(name, prm_path, fludd)
{
	mSoundId        = 0xffffffff;
	mRumbleOnCharge = false;
	mSprayState     = SPRAY_STATE_INACTIVE;
	mAnimationState = ANIM_STATE_SHOOT_START;
	mSprayTimer     = 0;
	mInsidePressure = 0.0f;
}

void TNozzleTrigger::init()
{
	mRumbleOnCharge = false;
	mSprayState     = SPRAY_STATE_INACTIVE;
	mAnimationState = ANIM_STATE_SHOOT_START;
	mSprayTimer     = 0;
	mInsidePressure = 0.0f;
}

void TNozzleTrigger::movement(const TMarioControllerWork& controllerWork)
{
	if (mFludd->getCurrentWater() <= 0) {
		mSprayState     = TNozzleTrigger::SPRAY_STATE_INACTIVE;
		mSprayTimer     = 0;
		mInsidePressure = 0.0f;
		return;
	}

	if (mSprayState == TNozzleTrigger::SPRAY_STATE_ACTIVE) {
		mSprayTimer -= 1;

		bool isPumping
		    = mFludd->getMario()->isUpperState(TMario::UPPER_STATE_PUMPING);
		if (!isPumping || mSprayTimer <= 0) {
			mSprayState     = TNozzleTrigger::SPRAY_STATE_DEAD;
			mInsidePressure = 0.0f;
			mSprayTimer     = 0;
		}
	}
	// Spam spray sound?
	if ((mRumbleOnCharge == true
	     && (controllerWork.mFrameInput & TMarioControllerWork::A) != 0
	     && (controllerWork.mInput & TMarioControllerWork::R) != 0)
	    && mSprayState == TNozzleTrigger::SPRAY_STATE_INACTIVE) {
		mSprayState = TNozzleTrigger::SPRAY_STATE_ACTIVE;
		if (mSoundId != 0xffffffff) {
			u32 soundId = unk378 < 1.0f ? MSD_SE_PO_WATER_LOW_TRG
			                            : MSD_SE_PO_WATER_HI_TRG;
			SMSGetMSound()->startSoundActor(soundId, mFludd->getEmitPos0(), 0,
			                                nullptr, 0, 4);
		}
		mSprayTimer = mTriggerTime.get();
	}

	bool canSpray = true;

	if (!mFludd->getMario()->isUpperState(TMario::UPPER_STATE_PUMPING))
		canSpray = false;

	if (mFludd->getMario()->checkFlag(MARIO_FLAG_IN_ANY_WATER) == true
	    && mFludd->getCurrentWater() < mAmountMax.get())
		canSpray = false;

	if (canSpray == true) {
		mInsidePressure += 150.0f * controllerWork.mAnalogR;
		if (!mRumbleOnCharge
		    && mSprayState == TNozzleTrigger::SPRAY_STATE_INACTIVE) {
			if (gpMarDirector->getMoveTickCount()
			        % (int)mFludd->getMario()->getUnk568()
			    == 0)
				SMSRumbleMgr->start(20, (int)mFludd->getMario()->getUnk564(),
				                    (f32*)nullptr);
		}
		if (mRumbleOnCharge != true
		    && mSprayState == TNozzleTrigger::SPRAY_STATE_INACTIVE
		    && controllerWork.mAnalogR > 0.0f) {
			SMSGetMSound()->startSoundActor(MSD_SE_SY_NEWP_AIR_TAME,
			                                mFludd->getEmitPos0(), 0, nullptr,
			                                0, 4);
		}
	}
	mInsidePressure -= mInsidePressureDec.get();
	if (mInsidePressure < 0.0f) {
		mInsidePressure = 0.0f;
	}

	if (mInsidePressure > mInsidePressureMax.get()) {
		mInsidePressure = mInsidePressureMax.get();
		if (!mRumbleOnCharge
		    && mSprayState == TNozzleTrigger::SPRAY_STATE_INACTIVE) {
			mSprayState = TNozzleTrigger::SPRAY_STATE_ACTIVE;
			mSprayTimer = mTriggerTime.get();
			if (mSoundId != 0xffffffff) {
				SMSGetMSound()->startSoundActor(
				    mSoundId, &mFludd->getEmitPos0(), 0, nullptr, 0, 4);
			}
			if (mFludd->getCurrentNozzleType() == TWaterGun::Hover) {
				SMSRumbleMgr->start((int)0x15, 0x8, (f32*)nullptr);
			}
			if (mFludd->getCurrentNozzleType() == TWaterGun::Rocket
			    || mFludd->getCurrentNozzleType() == TWaterGun::Turbo) {
				SMSRumbleMgr->start((int)0x15, 0x14, (f32*)nullptr);
			}
		}
	}

	if (mSprayState == TNozzleTrigger::SPRAY_STATE_DEAD) {
		mInsidePressure = 0.0f;
		if (controllerWork.mAnalogR == 0.0f) {
			mSprayState = TNozzleTrigger::SPRAY_STATE_INACTIVE;
		}
	}

	calcGunAngle(controllerWork);
}

void TNozzleTrigger::emit(int param_1)
{
	if (mFludd->mCurrentWater > 0
	    && (u8)mSprayState == TNozzleTrigger::SPRAY_STATE_ACTIVE) {
		TWaterEmitInfo* emitInfo = mFludd->mEmitInfo;
		emitCommon(param_1, emitInfo);

		f32 triggerFill       = mInsidePressure;
		f32 insidePressureMax = mInsidePressureMax.get();
		f32 emitNumMin        = mNumMin.get();
		f32 emitNum           = mNum.get();

		f32 pressure = triggerFill / insidePressureMax;

		mEmitNum += pressure * (emitNum - emitNumMin) + emitNumMin;

		s32 local37cInt = (s32)mEmitNum;
		if ((s32)mEmitNum == 0) {
			return;
		}
		mEmitNum -= (f32)local37cInt;

		emitInfo->mNum.set(local37cInt);

		f32& refEmitPow  = emitInfo->mPow.value;
		s32& refEmitFlag = emitInfo->mFlag.value;

		s16 attackMin = mAttackMin.get();
		s16 attack    = mAttack.get();
		emitInfo->mAttack.set(pressure * (f32)(attack - attackMin)
		                      + (f32)attackMin);

		f32 emitPowMin = mEmitPowMin.get();
		f32 emitPow    = mEmitPow.get();
		emitInfo->mPow.set(pressure * (emitPow - emitPowMin) + emitPowMin);

		refEmitFlag = 0x40;
		if (mFludd->hasFlag(TWaterGun::WATER_GUN_FLAG_UNK2)) {
			refEmitFlag = (refEmitFlag | 0x80);
		}

		u8 emittedWater = gpModelWaterManager->emitRequest(*emitInfo);
		mFludd->updateUnk1C88(emittedWater);

		if (emittedWater != 0) {
			mFludd->depleteWater(emittedWater * mDecRate.get());

			if ((mFludd->mCurrentNozzle == TWaterGun::Hover)
			    && ((SMSGetMarDirector()->mMoveTickCount & 0x7u) == 0u)) {
				SMSRumbleMgr->start(20, 2, (f32*)nullptr);
			}

			f32 reactionPowMin = mReactionPowMin.get();
			f32 reactionPow    = mReactionPow.get();

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
			f32 accelY = -dirVec.y * refEmitPow * mReactionY.get();
			mFludd->mMario->mVel.y += accelY;
		}
	}
}

void TNozzleTrigger::animation(int nozzleType)
{
	// Clip indices follow each trigger nozzle model animation data.
	int bckShootStart;
	int bckShooting;
	int bckShootEnd;
	int bckChangeStart;
	int bckChangeEnd;
	int emitMtxCount;

	J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(ANM_TYPE_BCK);

	switch (nozzleType) {
	case TWaterGun::Hover:
		bckShootStart  = 4;
		bckShooting    = 2;
		bckShootEnd    = 3;
		bckChangeStart = 1;
		bckChangeEnd   = 0;
		emitMtxCount   = 2;
		break;
	case TWaterGun::Rocket:
		bckShootStart  = 4;
		bckShooting    = 2;
		bckShootEnd    = 3;
		bckChangeStart = 1;
		bckChangeEnd   = 0;
		emitMtxCount   = 1;
		break;
	case TWaterGun::Turbo:
		bckShootStart  = 4;
		bckShooting    = 2;
		bckShootEnd    = 3;
		bckChangeStart = 1;
		bckChangeEnd   = 0;
		emitMtxCount   = 1;
		break;
	default:
		return;
	}

	if (mFludd->isSwitchingToSecondaryNozzle())
		mAnimationState = ANIM_STATE_CHANGE_END;

	if (mFludd->isSwitchingToPrimaryNozzle())
		mAnimationState = ANIM_STATE_CHANGE_START;

	switch (mAnimationState) {
	case ANIM_STATE_SHOOT_START: {
		mMActor->setBck(bckShootStart);

		if (isAnmEnd())
			mAnimationState = ANIM_STATE_SHOOTING;

		break;
	}

	case ANIM_STATE_SHOOTING: {
		mMActor->setBck(bckShooting);

		if (!mFludd->isEmitting())
			mAnimationState = ANIM_STATE_SHOOT_END;

		break;
	}

	case ANIM_STATE_SHOOT_END: {
		mMActor->setBck(bckShootEnd);

		if (mFludd->isEmitting() == true)
			mAnimationState = ANIM_STATE_SHOOT_START;

		break;
	}

	case ANIM_STATE_CHANGE_START: {
		mMActor->setBck(bckChangeStart);

		ctrl->setFrame(-(2.0f * (mFludd->getSwitchProgress() - 0.5f) - 1.0f)
		               * ctrl->getEnd());
		ctrl->setRate(0.0f);
		break;
	}

	case ANIM_STATE_CHANGE_END: {
		mMActor->setBck(bckChangeEnd);

		ctrl->setFrame(2.0f * (mFludd->getSwitchProgress() - 0.5f)
		               * ctrl->getEnd());
		ctrl->setRate(0.0f);

		if (mFludd->getSwitchProgress() >= 1.0f)
			mAnimationState = ANIM_STATE_SHOOT_START;

		break;
	}
	}

	if (mFludd->getEmittedWaterCount() != 0) {
		for (int emitterIndex = 0; emitterIndex < emitMtxCount;
		     ++emitterIndex) {
			if (mFludd->getEmitMtx(emitterIndex) != nullptr) {
				gpMarioParticleManager->emitAndBindToMtxPtr(
				    0x10D, mFludd->getEmitMtx(emitterIndex), 1,
				    &this[emitterIndex]); // Wth?
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

	unk378 *= mEmitPowScale.get();

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

	if (mBomb.mSprayState == TNozzleTrigger::SPRAY_STATE_INACTIVE
	    && unk378 > 0.0f) {
		TWaterEmitInfo* emitInfo = mFludd->mEmitInfo;
		emitCommon(param_1, emitInfo);

		f32 localUnk378 = unk378;

		f32 emitNum    = mNum.get();
		f32 emitNumMin = mNumMin.get();
		mEmitNum += localUnk378 * (emitNum - emitNumMin) + emitNumMin;

		s32 local37cInt = (s32)mEmitNum;
		if ((s32)mEmitNum == 0) {
			return;
		}
		mEmitNum -= (f32)local37cInt;

		emitInfo->mNum.set(local37cInt);

		f32& refEmitPow  = emitInfo->mPow.value;
		s32& refEmitFlag = emitInfo->mFlag.value;

		s16 attackMin = mAttackMin.get();
		s16 attack    = mAttack.get();
		emitInfo->mAttack.set(localUnk378 * (f32)(attack - attackMin)
		                      + (f32)attackMin);

		f32 dirTrembleMin = mDirTrembleMin.get();
		f32 dirTremble    = mDirTremble.get();
		emitInfo->mDirTremble.set(localUnk378 * (dirTremble - dirTrembleMin)
		                          + dirTrembleMin);

		f32 emitPowMin = mEmitPowMin.get();
		f32 emitPow    = mEmitPow.get();
		emitInfo->mPow.set(localUnk378 * (emitPow - emitPowMin) + emitPowMin);

		refEmitFlag = 0x40;
		if (mFludd->hasFlag(TWaterGun::WATER_GUN_FLAG_UNK2)) {
			refEmitFlag = (refEmitFlag | 0x80);
		}

		f32 sizeMinPressure = mSizeMinPressure.get();
		f32 sizeMin         = mSizeMin.get();
		f32 size            = mSize.get();
		f32 sizeMaxPressure = mSizeMaxPressure.get();

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
			mFludd->depleteWater(emittedWater * mDecRate.get());

			if ((SMSGetMarDirector()->mMoveTickCount & 0x7u) == 0u) {
				SMSRumbleMgr->start(20, 2, (f32*)nullptr);
			}

			f32 reactionPowMin = mReactionPowMin.get();
			f32 reactionPow    = mReactionPow.get();

			f32 reaction
			    = localUnk378 * (reactionPow - reactionPowMin) + reactionPowMin;

			s16 faceAngleY = mFludd->mMario->mFaceAngle.y;
			f32 dirX       = emitInfo->mDir.get().x;
			f32 dirZ       = emitInfo->mDir.get().z;
			f32 cosAngle   = JMASCos(faceAngleY);
			f32 sinAngle   = JMASSin(faceAngleY);
			f32 faceDot    = (-cosAngle * dirZ - dirX * sinAngle);

			mFludd->mMario->addVelocity(reaction * faceDot * refEmitPow);

			JGeometry::TVec3<f32> const& dirVec = emitInfo->mDir.get();
			f32 accelY = -dirVec.y * refEmitPow * mReactionY.get();
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

	if (mFludd->isSwitchingToSecondaryNozzle()) {
		if (mAnimationState != 4 && mAnimationState != 6) {
			if (mFludd->unk1CEC > 0.5f) {
				mAnimationState = 4;
			} else {
				mAnimationState = 6;
			}
		}
	} else {
		if (mFludd->isSwitchingToPrimaryNozzle()) {
			if (mAnimationState != 5 && mAnimationState != 7) {
				if (mFludd->unk1CEC > 0.5f) {
					mAnimationState = 5;
				} else {
					mAnimationState = 7;
				}
			}
		} else {
			if (mFludd->unk1CEC > 0.0f) {
				mAnimationState = 0;
			} else if (mAnimationState == 0) {
				mAnimationState = 2;
			}
		}
	}

	J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(ANM_TYPE_BCK);

	switch (mAnimationState) {
	case 0: {
		MActor* mactor = mMActor;
		if (!mactor->checkCurBckFromIndex(4))
			mactor->setBckFromIndex(4);

		ctrl->setFrame(ctrl->getEnd() * mFludd->unk1CEC);
		break;
	}
	case 1:
		break;
	case 2: {
		MActor* mactor = mMActor;
		if (!mactor->checkCurBckFromIndex(7))
			mactor->setBckFromIndex(7);

		bool finished           = 0;
		J3DFrameCtrl* frameCtrl = mMActor->getFrameCtrl(ANM_TYPE_BCK);
		if (frameCtrl->checkState(J3DFrameCtrl::STATE_COMPLETED_ONCE
		                          | J3DFrameCtrl::STATE_LOOPED_ONCE)) {
			finished = 1;
		}

		if (frameCtrl->getFrame() > (frameCtrl->getEnd() - 0.1f)) {
			finished = 1;
		}

		if (finished) {
			mAnimationState = 3;
		}
		break;
	}
	case 3: {
		MActor* mactor = mMActor;
		if (!mactor->checkCurBckFromIndex(5))
			mactor->setBckFromIndex(5);

		bool updateAnimation = false;
		if (mFludd->mCurrentWater == 0) {
			updateAnimation = false;
		} else if (mFludd->getNozzle(mFludd->mCurrentNozzle)->getNozzleKind()
		           == 1) {
			if (((TNozzleTrigger*)mFludd->getNozzle(mFludd->mCurrentNozzle))
			        ->mSprayState
			    == TNozzleTrigger::SPRAY_STATE_INACTIVE)
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
			mAnimationState = 8;

		break;
	}
	case 8: {
		MActor* mactor = mMActor;
		if (!mactor->checkCurBckFromIndex(6))
			mactor->setBckFromIndex(6);

		bool updateAnimation = false;
		if (mFludd->mCurrentWater == 0) {
			updateAnimation = false;
		} else if (mFludd->getCurrentNozzle()->getNozzleKind() == 1) {
			if (((TNozzleTrigger*)mFludd->getCurrentNozzle())->mSprayState
			    == TNozzleTrigger::SPRAY_STATE_INACTIVE)
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
			mAnimationState = 2;

		bool finished           = false;
		J3DFrameCtrl* frameCtrl = mMActor->getFrameCtrl(ANM_TYPE_BCK);
		if (frameCtrl->checkState(J3DFrameCtrl::STATE_COMPLETED_ONCE
		                          | J3DFrameCtrl::STATE_LOOPED_ONCE))
			finished = true;

		if (frameCtrl->getFrame() > (frameCtrl->getEnd() - 0.1f))
			finished = true;

		if (finished && !(mFludd->unk1CEC == 0.0f ? true : false))
			mAnimationState = 0;

		break;
	}
	case 4: {
		MActor* mactor = mMActor;
		if (!mactor->checkCurBckFromIndex(1))
			mactor->setBckFromIndex(1);

		ctrl->setFrame(2.0f * mFludd->mSwitchToSecondNozzleProgress
		               * ctrl->getEnd());
		ctrl->setRate(0.0f);
		break;
	}
	case 5: {
		MActor* mactor = mMActor;
		if (!mactor->checkCurBckFromIndex(0))
			mactor->setBckFromIndex(0);

		ctrl->setFrame((1.0f - 2.0f * mFludd->mSwitchToSecondNozzleProgress)
		               * ctrl->getEnd());
		ctrl->setRate(0.0f);
		if (mFludd->mSwitchToSecondNozzleProgress <= 0.0f) {
			mAnimationState = 0;
			mFludd->unk1CEC = 0.0f;
		}
		break;
	}
	case 6: {
		MActor* mactor = mMActor;
		if (!mactor->checkCurBckFromIndex(3))
			mactor->setBckFromIndex(3);

		ctrl->setFrame(2.0f * mFludd->mSwitchToSecondNozzleProgress
		               * ctrl->getEnd());
		ctrl->setRate(0.0f);
		break;
	}
	case 7:
		MActor* mactor = mMActor;
		if (!mactor->checkCurBckFromIndex(2))
			mactor->setBckFromIndex(2);

		ctrl->setFrame((1.0f - 2.0f * mFludd->mSwitchToSecondNozzleProgress)
		               * ctrl->getEnd());
		ctrl->setRate(0.0f);
		if (mFludd->mSwitchToSecondNozzleProgress <= 0.0f) {
			mAnimationState = 2;
			mFludd->unk1CEC = 0.0f;
		}
		break;
	}

	if (mFludd->mEmittedWaterCount != 0) {
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
	mFlags                       = 0;
	mNozzleList[Spray]           = &mNozzleDeform;
	mNozzleList[Rocket]          = &mNozzleRocket;
	mNozzleList[Underwater]      = &mNozzleUnderWater;
	mNozzleList[Yoshi]           = &mNozzleYoshiDeform;
	mNozzleList[Hover]           = &mNozzleHover;
	mNozzleList[Turbo]           = &mNozzleTurbo;
	mCurrentNozzle               = Spray;
	mSecondNozzle                = Hover;
	mNozzleRocket.mSoundId       = MSD_SE_PO_ROCKET_TRIGGER;
	mNozzleTurbo.mSoundId        = MSD_SE_PO_SNIPER_TRIGGER;
	mNozzleDeform.mBomb.mSoundId = MSD_SE_PO_SHOTGUN_TRIGGER;
	mCurrentWater      = mNozzleList[mCurrentNozzle]->mAmountMax.get();
	mEmittedWaterCount = 0;
	unk1C88            = 0.0f;
	mCurrentPressure   = 0;
	mPreviousPressure  = 0;
	unk1CEC            = 1.0f;
	unk1CF0            = 0.1f;
	unk1CF4            = 0.0049999999f;
	unk1CF8            = 0x168;
	unk1CFA            = 0;
	mSwitchToSecondNozzleProgress = 0.0f;
	mSwitchToSecondNozzleSpeed    = 0.0f;
	unk1D04                       = 0;
	unk1D06                       = -0x1800;
	unk1D08                       = 0;

	mEmitInfo = new TWaterEmitInfo("/Mario/GunEmit.prm");

	unk1D08                                  = 0;
	mNozzleDeform.mBomb.mRumbleOnCharge      = true;
	mNozzleYoshiDeform.mBomb.mRumbleOnCharge = true;

	// TODO: wrong
	MtxPtr r24 = mMario->mModel->unk8->getAnmMtx(mMario->mJointIdChest);

	mEmitPos[3] = mMario->mPosition;

	unk1CC0 = 0;
	unk1CC2 = 0;
	unk1CC4 = 0;

	mHoverAngleL    = 0.0f;
	mHoverAngleR    = 0.0f;
	mPropellerAngle = 0;
	unk1CD2         = 0;

	// This is definitely an inlined funciton. Creating a model seems quite
	// useful
	// TODO: Check if already exists
	MActorAnmData* watergunAnmData = new MActorAnmData();
	watergunAnmData->init("/mario/watergun2/body", nullptr);
	mFluddModel = new MActor(watergunAnmData);

	void* fluddModelData
	    = JKRFileLoader::getGlbResource("/mario/watergun2/body/wg_mdl1.bmd");
	J3DModel* fluddModel = new J3DModel(
	    J3DModelLoaderDataBase::load(fluddModelData,
	                                 J3DMLF_MaterialPEFull
	                                     | (4 << J3DMLF_TevStageNumShift)),
	    0, 1);
	mFluddModel->setModel(fluddModel, 0);

	mFluddModel->getModel()->setBaseTRMtx(
	    mMario->mModel->getModel()->getAnmMtx(mMario->mJointIdChest));

	mFluddModel->getModel()->calc();

	u16 handleIdx
	    = mFluddModel->getModel()->getModelData()->getJointName()->getIndex(
	        "jnt_G_handle");

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

	unk1CD8 = mFluddModel->getModel()->getModelData()->getJointName()->getIndex(
	    "nozzle_center");

	for (int i = 0; i < 6; ++i) {
		if (nozzleBmdData.getPath(i)) {

			MActorAnmData* nozzleData = new MActorAnmData();
			nozzleData->init(nozzleBmdData.getPath(i), nullptr);
			mNozzleList[i]->mMActor = new MActor(nozzleData);

			void* nozzleModelData
			    = JKRFileLoader::getGlbResource(nozzleBmdData.getBmdPath(i));
			J3DModel* nozzleModel = new J3DModel(
			    J3DModelLoaderDataBase::load(
			        nozzleModelData,
			        J3DMLF_MaterialPEFull | (4 << J3DMLF_TevStageNumShift)),
			    0, 1);
			mNozzleList[i]->mMActor->setModel(nozzleModel, 0);

			J3DModelData* modelData
			    = mNozzleList[i]->mMActor->getModel()->getModelData();

			SMS_ChangeTextureAll(modelData, "H_watergun_main_dummy",
			                     *mFluddModel->getModel()
			                          ->getModelData()
			                          ->getTexture()
			                          ->getResTIMG(1));

			mNozzleList[i]->mMActor->initDL();

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
			mNozzleList[i]->mMActor = nullptr;
		}
	}

	mNozzleList[Spray]
	    ->mMActor->getModel()
	    ->getModelData()
	    ->getJointNodePointer(mNozzleList[Spray]
	                              ->mMActor->getModel()
	                              ->getModelData()
	                              ->getJointName()
	                              ->getIndex("chn_muzzle_1"))
	    ->setCallBack(&NozzleCtrl);

	mNozzleList[Hover]
	    ->mMActor->getModel()
	    ->getModelData()
	    ->getJointNodePointer(mNozzleList[Hover]
	                              ->mMActor->getModel()
	                              ->getModelData()
	                              ->getJointName()
	                              ->getIndex("jnt_nozzle_L"))
	    ->setCallBack(&WaterGunDivingCtrlL);

	mNozzleList[Hover]
	    ->mMActor->getModel()
	    ->getModelData()
	    ->getJointNodePointer(mNozzleList[Hover]
	                              ->mMActor->getModel()
	                              ->getModelData()
	                              ->getJointName()
	                              ->getIndex("jnt_nozzle_R"))
	    ->setCallBack(&WaterGunDivingCtrlR);

	mNozzleList[Turbo]
	    ->mMActor->getModel()
	    ->getModelData()
	    ->getJointNodePointer(mNozzleList[Turbo]
	                              ->mMActor->getModel()
	                              ->getModelData()
	                              ->getJointName()
	                              ->getIndex("chn_back_nozzle_prop"))
	    ->setCallBack(&RotateCtrl);

	mNozzleList[Turbo]
	    ->mMActor->getModel()
	    ->getModelData()
	    ->getJointNodePointer(mNozzleList[Turbo]
	                              ->mMActor->getModel()
	                              ->getModelData()
	                              ->getJointName()
	                              ->getIndex("jnt_back_nozzle_neck"))
	    ->setCallBack(&NozzleCtrl);

	mFluddModel->getModel()->setBaseTRMtx(r24);
	mFluddModel->getModel()->calc();

	unk1D10 = new TMirrorActor("水鉄砲in鏡");
	unk1D10->init(mFluddModel->getModel(), 4);

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
		result = mMario->getYoshi()->getTongueMtx();
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
			result = mMario->getYoshi()->getTongueMtx();
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
	return mFluddModel->getModel()->getAnmMtx(unk1CD8);
}

void TWaterGun::changeNozzle(TNozzleType nozzleType, bool animate)
{
	f32 usedWater
	    = (f32)mCurrentWater / mNozzleList[mCurrentNozzle]->mAmountMax.get();
	if (nozzleType == Spray) {
		if (animate == true) {
			mSwitchToSecondNozzleProgress = 0.0f;
		}
	} else {
		mSecondNozzle = nozzleType;
		if (animate == true) {
			mSwitchToSecondNozzleProgress = 1.0f;
		}
	}
	mCurrentNozzle = nozzleType;
	mNozzleList[mCurrentNozzle]->init();
	if (nozzleType == Yoshi) {
		mCurrentWater = mMario->mYoshi->unkD4;
	} else {
		mCurrentWater
		    = usedWater * mNozzleList[mCurrentNozzle]->mAmountMax.get();
	}
}

void TWaterGun::movement()
{
	if (!canSpray()) {
		unk1CC2 = 0;
		unk1CC4 = 0;
	}

	mHoverAngleL
	    += (unk1CC2 - mHoverAngleL) * mWatergunParams.mHoverSmooth.get();
	mHoverAngleR
	    += (unk1CC4 - mHoverAngleR) * mWatergunParams.mHoverSmooth.get();

	rotateProp(getCurrentNozzle()->unk378);

	// They do the same thing again?... This is the exact same code as
	// rotateProp
	if (mCurrentNozzle == Turbo) {
		unk1CD2 += mNozzleList[mCurrentNozzle]->unk378
		           * mWatergunParams.mNozzleAngleYSpeed.get();
		unk1CD2 *= mWatergunParams.mNozzleAngleYBrake.get();
		if (mWatergunParams.mNozzleAngleYSpeedMax.get() < unk1CD2) {
			unk1CD2 = mWatergunParams.mNozzleAngleYSpeedMax.get();
		}
		mPropellerAngle = mPropellerAngle + unk1CD2;
	} else {
		unk1CD2         = 0;
		mPropellerAngle = 0;
	}

	// Yoshi nozzle
	if (mCurrentNozzle == 3) {
		mCurrentWater = getCurrentNozzle()->mAmountMax.get();
	}

	if (SMS_isDivingMap()) {
		mCurrentWater = getCurrentNozzle()->mAmountMax.get();
	}

	if (mCurrentNozzle == 3) {
		unk1CEC = 0.0f;
	}

	// Nozzle swapping
	if (mSwitchToSecondNozzleSpeed != 0.0f) {
		f32 before                    = mSwitchToSecondNozzleProgress;
		f32 after                     = before + mSwitchToSecondNozzleSpeed;
		mSwitchToSecondNozzleProgress = after;

		if (before < 0.5f && 0.5f <= after)
			changeNozzle((TNozzleType)mSecondNozzle, false);

		if (after < 0.5f && 0.5f <= before)
			changeNozzle(Spray, false);

		if (mSwitchToSecondNozzleProgress < 0.0f) {
			mSwitchToSecondNozzleProgress = 0.0f;
			mSwitchToSecondNozzleSpeed    = 0.0f;
		}

		if (1.0f < mSwitchToSecondNozzleProgress) {
			mSwitchToSecondNozzleProgress = 1.0f;
			mSwitchToSecondNozzleSpeed    = 0.0f;
		}
	}

	getCurrentNozzle()->animation(mCurrentNozzle);
}

void TWaterGun::setBaseTRMtx(Mtx mtx)
{
	Mtx result;
	Mtx temp;

	f32 initialAngle = mtx[1][0];
	if (initialAngle < 0.0f)
		initialAngle = -initialAngle;

	// Seemingly some adjustment of fluddpack angle
	s16 angle = initialAngle * (unk1D04 - unk1D06) + unk1D06;

	f32 angleDegrees = SHORTANGLE2DEG(angle);
	MsMtxSetRotRPH(temp, 0.0f, 0.0f, angleDegrees);

	MTXConcat(mtx, temp, result);
	mFluddModel->getModel()->setBaseTRMtx(result);
}

void TWaterGun::calcAnimation(JDrama::TGraphics* graphics)
{
	gpMarioForCallBack      = mMario;
	J3DFrameCtrl* frameCtrl = mFluddModel->getFrameCtrl(ANM_TYPE_BCK);
	if (mMario == nullptr)
		return;

	// TODO: Wut? There's no 0x8000 flag for this state anywhere else?
	s32 var380 = (mMario->mUpperState & 0x8000) != 0 ? 0 : mMario->mUpperState;

	switch (var380) {
	case TMario::UPPER_STATE_PUMPING:
	case TMario::UPPER_STATE_HOLDING_PUMP:
		if (unk1CEC == 0.0f) {
			if (mMario->isFencing()) {
				mFluddModel->setBck("wg_fepmp");
			} else if (mMario->checkFlag(MARIO_FLAG_IN_SHALLOW_WATER
			                             | MARIO_FLAG_IN_WATER)) {
				mFluddModel->setBck("wg_swpmp");
			} else {
				switch (mMario->mAnimationId) {
				case TMario::ANIM_HANG:
					mFluddModel->setBck("wg_hgpmp");
					break;
				default:
					mFluddModel->setBck("wg_pump");
					break;
				}
			}

			frameCtrl->setRate(0.0f);
			frameCtrl->setFrame(mMario->getPumpFrame());
			unk1CFA = unk1CF8;
		} else {
			mFluddModel->setBck("wg_house");
			if (unk1CEC > 0.0f) {
				unk1CEC -= 0.1f;
				if (unk1CEC <= 0.0f)
					unk1CEC = 0.0f;
			}
			frameCtrl->setRate(0.0f);
			frameCtrl->setFrame(unk1CEC * frameCtrl->getEnd());
		}
		break;

	case TMario::UPPER_STATE_IDLE:
	default:
		if (unk1CFA == 0) {
			if (unk1CEC < 1.0f) {
				unk1CEC += unk1CF4;
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
			unk1CFA -= 1;
		}
		break;
	}
}

void TWaterGun::perform(u32 cue, JDrama::TGraphics* graphics)
{
	// TODO: Missing stack space
	// volatile u32 unused2[24];

	if ((cue & CUE_MOVE) != 0) {
		if ((mFlags & WATER_GUN_FLAG_UNK10) != 0) {
			mCurrentWater = 0;
		}
		movement();
	}

	if ((cue & CUE_CALC_ANIM) != 0) {
		calcAnimation(graphics);
	}

	mFluddModel->perform(cue, graphics);

	if ((cue & CUE_CALC_ANIM) != 0) {
		MActor* p2 = getCurrentNozzle()->getMActor();
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

	if (getCurrentNozzle()->getMActor()) {
		getCurrentNozzle()->getMActor()->perform(cue, graphics);
	}
}

TNozzleBase* TWaterGun::getCurrentNozzle() const
{
	return getNozzle(mCurrentNozzle);
}

void TWaterGun::setAmountToRate(f32 rate)
{
	// volatile u32 unused2[7]; // TODO: possibly inlined function
	if (mCurrentNozzle == 3) {
		TNozzleBase* currentNozzle = getCurrentNozzle();
		s32 amountMax              = currentNozzle->mAmountMax.get();
		mCurrentWater              = amountMax;
	} else {
		TNozzleBase* currentNozzle = getCurrentNozzle();
		mCurrentWater              = rate * currentNozzle->mAmountMax.get();
	}
}

BOOL TWaterGun::isPressureOn()
{
	// volatile u32 unused2[6];
	if (getCurrentNozzle()->getNozzleKind() == 1) {
		TNozzleTrigger* triggerNozzle = (TNozzleTrigger*)getCurrentNozzle();
		if (triggerNozzle->mInsidePressure > 0.0f) {
			return TRUE;
		}
	}
	return FALSE;
}

f32 TWaterGun::getPressure()
{
	// TODO: Missing stack space
	// volatile u32 unused2[5];
	if (getCurrentNozzle()->getNozzleKind() == 1) {
		TNozzleTrigger* triggerNozzle = (TNozzleTrigger*)getCurrentNozzle();
		return triggerNozzle->mInsidePressure;
	}
	return 0.0f;
}

f32 TWaterGun::getPressureMax()
{
	// TODO: Missing stack space
	// volatile u32 unused2[6];

	if (getCurrentNozzle()->getNozzleKind() == 1) {
		return getCurrentNozzle()->mInsidePressureMax.get();
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
		if (mWatergunParams.mNozzleAngleYSpeedMax.get() < unk1CD2) {
			unk1CD2 = mWatergunParams.mNozzleAngleYSpeedMax.get();
		}
		mPropellerAngle = mPropellerAngle + unk1CD2;
	} else {
		unk1CD2         = 0;
		mPropellerAngle = 0;
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

	if (hasFlag(WATER_GUN_FLAG_UNK4)) {
		offFlag(WATER_GUN_FLAG_UNK4);
		return;
	}

	u8 currentNozzleType       = mCurrentNozzle;
	TNozzleBase* currentNozzle = getNozzle(currentNozzleType);
	for (int i = 0; i < nozzleBmdData.getEmitterCount(currentNozzleType); ++i)
		currentNozzle->emit(i);

	if (mCurrentWater > 0) {
		switch (currentNozzleType) {
		case Spray:
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_PO_NORMAL_NOZZLE_IMI, &getEmitPos0(), nullptr,
			    getCurrentNozzle()->unk374, 0, 0, nullptr, 0, 4);

		case Yoshi:
		case Turbo:
			SMSGetMSound()->startSoundActorWithInfo(
			    0x0, &getEmitPos0(), nullptr, getCurrentNozzle()->unk378, 0, 0,
			    nullptr, 0, 4);
			break;

		case Underwater:
			SMSGetMSound()->startSoundActor(MSD_SE_PO_HOVER, &getEmitPos0(), 0,
			                                nullptr, 0, 4);
			break;

		case Rocket:
			break;

		case Hover:
			if (mEmittedWaterCount)
				SMSGetMSound()->startSoundActor(MSD_SE_PO_HOVER, &getEmitPos0(),
				                                0, nullptr, 0, 4);
			break;
		}
	}
}
BOOL TWaterGun::suck()
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
			s32 maxWater     = getCurrentNozzle()->mAmountMax.get();
			if (currentWater > maxWater) {
				mCurrentWater = maxWater;
			}

			if (!(mCurrentWater >= getCurrentNozzle()->mAmountMax.get())) {
				SMSGetMSound()->startSoundActor(
				    MSD_SE_PO_SUCK_WATER_B, &getEmitPos0(), 0, nullptr, 0, 4);
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

		mCurrentWater -= nozzle->mDamageLoss.value;

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
	if (mSwitchToSecondNozzleProgress == 0.0f) {
		SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_SELECT_POMP_BACK, 0,
		                                   nullptr, 0);
		mSwitchToSecondNozzleSpeed = mWatergunParams.mChangeSpeed.get();
	}

	if (mSwitchToSecondNozzleProgress == 1.0f) {
		SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_SELECT_POMP, 0, nullptr,
		                                   0);
		mSwitchToSecondNozzleSpeed = -mWatergunParams.mChangeSpeed.get();
	}
}

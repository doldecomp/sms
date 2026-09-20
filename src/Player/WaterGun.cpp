#include <Player/WaterGun.hpp>
#include <Player/NozzleTrigger.hpp>
#include <Player/NozzleBase.hpp>
#include <Player/NozzleDeform.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/Mario.hpp>

#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/JMath.hpp>

#include <System/MarDirector.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/StageUtil.hpp>

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
#include <Player/MarioDirtyStrings.hpp>

// Carries the zero and (1,1,1) vectors retail emits between the dirty-texture
// names and nozzleBmdData's paths; without them every later .rodata string
// sits 24 bytes low and nozzleBmdData's relocations miss.
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

// TNozzleTurbo and TNozzleButton are abandoned nozzle implementations. Every
// one of their members is UNUSED in the map, as are both of their vtables, so
// nothing in retail ever constructed one -- the turbo nozzle that shipped is
// the TNozzleTrigger built with "/Mario/WaterGun/NozzleTrgTurbo.prm". They are
// declared here rather than in a header because no other TU mentions them.
// Both vtables are 0x28, exactly TNozzleBase's ten slots, so neither class
// adds a virtual; the declaration order below is the vtable's.
// TODO: TNozzleTurbo's constructor is 0xd0 against TNozzleButton's 0x44 and
// TNozzleTrigger's 0x60, so it initialises considerably more state -- extra
// members of its own, or a TNozzleTrigger base. Not decidable from the map.
class TNozzleTurbo : public TNozzleBase {
public:
	TNozzleTurbo(const char* name, const char* prm, TWaterGun* fludd);

	virtual s32 getNozzleKind() const;
	virtual void movement(const TMarioControllerWork&);
	virtual void animation(int);
};

class TNozzleButton : public TNozzleBase {
public:
	TNozzleButton(const char* name, const char* prm, TWaterGun* fludd);

	virtual void init();
	virtual s32 getNozzleKind() const;
	virtual void movement(const TMarioControllerWork&);
	virtual void emit(int);
	virtual void animation(int);
};

// TODO: the constants are unknown. The map's 0x8 is one `li r3, N; blr` and is
// satisfied by any value; the kinds in use are 0 (TNozzleBase), 1
// (TNozzleTrigger) and 3 (TNozzleDeform), leaving 2 and 4 free.
s32 TNozzleButton::getNozzleKind() const { return 0; }

s32 TNozzleTurbo::getNozzleKind() const { return 0; }

// Binding level over a raw member read, worth +16 of low region in
// NozzleCtrl (batch 127).
static inline TWaterGun* WaterGunWaterGun(const TMario* p)
{
	TWaterGun* waterGun = p->mWaterGun;
	return waterGun;
}

static BOOL NozzleCtrl(J3DNode* node, BOOL param_2)
{
	// TODO: Inlined stack space
	if (!param_2) {
		if (gpMarioForCallBack != nullptr) {
			s16 gunAngle = WaterGunWaterGun(gpMarioForCallBack)->getCurrentNozzle()
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
		Mtx mtx;
		s16 local1cd0
		    = WaterGunWaterGun(gpMarioForCallBack)->getHoverAngle();
		f32 roll = SHORTANGLE2DEG(local1cd0);
		MsMtxSetRotRPH(mtx, roll, 0.0f, 0.0f);
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
		s32 nozzleSpeedY
		    = WaterGunWaterGun(gpMarioForCallBack)->getNozzleSpeedY();
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
		s32 nozzleSpeedY
		    = WaterGunWaterGun(gpMarioForCallBack)->getNozzleSpeedZ();
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
	    && (gpCamera->isLButtonCamera() || gpCamera->isJetCoaster1stCamera())) {
		unk36E = gpCamera->mCurrentTarget.mPitch;
		return;
	}

	s16 angle;
	if (mFludd->mMario->mStatus == MARIO_STATUS_SQUAT) {
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

// Binding level over a raw member read, worth +8 of low region in
// TNozzleBase::movement (batch 127).
static inline u16 WaterGunUnk372(const TNozzleBase* p)
{
	u16 v372 = p->unk372;
	return v372;
}

void TNozzleBase::movement(const TMarioControllerWork& controllerWork)
{
	if (mFludd->mCurrentWater <= 0) {
		return;
	}
	s32 var1 = 150.0f * controllerWork.mAnalogR * 256.0f;

	if (var1 > unk372) {
		unk378 = (var1 - WaterGunUnk372(this)) * 0.000015258789f;
		unk374 = unk378;
		unk372 += (u16)mEmitParams.mTriggerRate.get();
		if (var1 < getUnk372()) {
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
		if (mFludd->checkFlag(TWaterGun::WATER_GUN_FLAG_UNK2)) {
			refEmitFlag = (refEmitFlag | 0x80);
		}

		s32 emittedWater = gpModelWaterManager->emitRequest(*emitInfo);

		mFludd->updateUnk1C88(emittedWater);
		if (emittedWater != 0) {
			mFludd->depleteWater(emittedWater * mEmitParams.mDecRate.get());

			JGeometry::TVec3<f32> const& dirVec = emitInfo->mDir.get();

			f32 reactionPow
			    = refEmitPow * mEmitParams.mReactionPow.get();
			mFludd->mMario->addVelocity(
			    (-dirVec.x * JMASSin(mFludd->mMario->mFaceAngle.y)
			     - dirVec.z * JMASCos(mFludd->mMario->mFaceAngle.y))
			    * reactionPow);

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
// UNUSED (map size 0x94). Emitted between TNozzleBase::emit and
// TNozzleBase::animation, i.e. defined here in source order; inlined into all
// three animation() overrides.
bool TNozzleBase::isAnmEnd() const
{
	bool end = false;

	J3DFrameCtrl* ctrl = unk380->getFrameCtrl(ANM_TYPE_BCK);
	if (ctrl->checkState(J3DFrameCtrl::STATE_COMPLETED_ONCE
	                     | J3DFrameCtrl::STATE_LOOPED_ONCE))
		end = true;

	if (ctrl->getFrame() > (ctrl->getEnd() - 0.1f))
		end = true;

	return end;
}

void TNozzleBase::animation(int param_1)
{
	J3DFrameCtrl* ctrl = unk380->getFrameCtrl(ANM_TYPE_BCK);

	// TODO: retail emits `beq body; b ret; b body; b ret` here -- two dead
	// branches more than this switch produces, and `case 3: return;` makes it
	// a range test instead. The extra pair is still unexplained.
	switch (param_1) {
	case 2:
		break;
	default:
		return;
	}

	if (mFludd->isSwitchingToSecondaryNozzle())
		unk36C = 4;

	if (mFludd->isSwitchingToSprayNozzle())
		unk36C = 3;

	switch (unk36C) {
	case 0: {

		// TODO: inline
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(4))
			mactor->setBckFromIndex(4);

		if (!isAnmEnd())
			return;

		unk36C = 1;
		break;
	}

	case 1: {
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(2))
			mactor->setBckFromIndex(2);

		if (mFludd->isEmitting())
			return;

		unk36C = 2;
		break;
	}

	case 2: {
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(3))
			mactor->setBckFromIndex(3);

		if (mFludd->isEmitting() == true)
			unk36C = 0;
		break;
	}

	case 3: {
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(1))
			mactor->setBckFromIndex(1);

		// Use external tween value
		ctrl->setFrame(
		    -(2.0f * (mFludd->mSwitchToSecondNozzleProgress - 0.5f) - 1.0f)
		    * ctrl->getEnd());
		ctrl->setRate(0.0f);
		break;
	}

	case 4:
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(0))
			mactor->setBckFromIndex(0);

		// Use external tween value
		ctrl->setFrame(2.0f * (mFludd->mSwitchToSecondNozzleProgress - 0.5f)
		               * ctrl->getEnd());
		ctrl->setRate(0.0f);

		if (mFludd->mSwitchToSecondNozzleProgress >= 1.0f)
			unk36C = 0;

		break;
	}
}

// Moved out of NozzleTrigger.hpp: the map has this constructor as UNUSED in
// this TU, and an UNUSED symbol is never emitted from an in-class body that
// every call site inlines. -inline deferred still lets TWaterGun's
// constructor inline it from here.
TNozzleTrigger::TNozzleTrigger(const char* name, const char* prm,
                               TWaterGun* fludd)
    : TNozzleBase(name, prm, fludd)
{
	unk38C = 0xffffffff;
	unk384 = false;
	unk385 = INACTIVE;
	unk36C = 0;
	unk386 = 0;
	unk388 = 0.0f;
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
	f32 prevPressure = unk388;

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
		if (mFludd->mMario->mUpperState == TMario::UPPER_STATE_PUMPING) {
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
			u32 soundId = unk378 < 1.0f ? MSD_SE_PO_WATER_LOW_TRG
			                            : MSD_SE_PO_WATER_HI_TRG;
			SMSGetMSound()->startSoundActor(soundId, mFludd->mEmitPos[0], 0,
			                                nullptr, 0, 4);
		}
		unk386 = mEmitParams.mTriggerTime.get();
	}

	bool canSpray = true;

	if (!(mFludd->mMario->mUpperState == TMario::UPPER_STATE_PUMPING ? true
	                                                                 : false))
		canSpray = false;

	if (mFludd->mMario->checkFlag(MARIO_FLAG_IN_ANY_WATER) == true
	    && mFludd->mCurrentWater < mEmitParams.mAmountMax.get())
		canSpray = false;

	if (canSpray == true) {
		unk388 += 150.0f * controllerWork.mAnalogR;
		if (!unk384 && unk385 == TNozzleTrigger::INACTIVE) {
			if (gpMarDirector->unk58 % (int)mFludd->mMario->unk568 == 0)
				SMSRumbleMgr->start(20, (int)mFludd->mMario->unk564,
				                    (f32*)nullptr);
		}
	}
	unk388 -= mEmitParams.mInsidePressureDec.get();
	if (unk388 < 0.0f) {
		unk388 = 0.0f;
	}

	if (canSpray == true && unk384 != true
	    && unk385 == TNozzleTrigger::INACTIVE
	    && controllerWork.mAnalogR > 0.0f && prevPressure < unk388) {
		SMSGetMSound()->startSoundActor(MSD_SE_SY_NEWP_AIR_TAME,
		                                mFludd->mEmitPos[0], 0, nullptr, 0, 4);
	}

	if (unk388 > mEmitParams.mInsidePressureMax.get()) {
		unk388 = mEmitParams.mInsidePressureMax.get();
		if (!unk384 && unk385 == TNozzleTrigger::INACTIVE) {
			unk385      = TNozzleTrigger::ACTIVE;
			unk386      = mEmitParams.mTriggerTime.get();
			if (unk38C != 0xffffffff) {
				SMSGetMSound()->startSoundActor(unk38C, &mFludd->mEmitPos[0],
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
	if (mFludd->mCurrentWater > 0 && unk385 == TNozzleTrigger::ACTIVE) {
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
		if (mFludd->checkFlag(TWaterGun::WATER_GUN_FLAG_UNK2)) {
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

			// TODO: retail reads mFaceAngle.y with `lhz` and re-derives the
			// sin/cos table index twice, i.e. no named s16 angle and no CSE.
			// Spelling it that way (as in TNozzleBase::emit) costs 0x20 of
			// frame here and nets -2%, so the named locals stay.
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

	J3DFrameCtrl* ctrl = unk380->getFrameCtrl(ANM_TYPE_BCK);

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

	if (mFludd->isSwitchingToSecondaryNozzle())
		unk36C = 4;

	if (mFludd->isSwitchingToSprayNozzle())
		unk36C = 3;

	switch (unk36C) {
	case 0: {
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(bckIdleOut))
			mactor->setBckFromIndex(bckIdleOut);

		if (isAnmEnd())
			unk36C = 1;

		break;
	}

	case 1: {
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(bckIdle))
			mactor->setBckFromIndex(bckIdle);

		if (!mFludd->isEmitting())
			unk36C = 2;

		break;
	}

	case 2: {
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(bckStart))
			mactor->setBckFromIndex(bckStart);

		if (mFludd->isEmitting() == true)
			unk36C = 0;

		break;
	}

	case 3: {
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(bckSwapOut))
			mactor->setBckFromIndex(bckSwapOut);

		// Use external tween value
		ctrl->setFrame(
		    -(2.0f * (mFludd->mSwitchToSecondNozzleProgress - 0.5f) - 1.0f)
		    * ctrl->getEnd());
		ctrl->setRate(0.0f);
		break;
	}

	case 4: {
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(bckSwapIn))
			mactor->setBckFromIndex(bckSwapIn);

		// Use external tween value
		ctrl->setFrame(2.0f * (mFludd->mSwitchToSecondNozzleProgress - 0.5f)
		               * ctrl->getEnd());
		ctrl->setRate(0.0f);

		if (mFludd->mSwitchToSecondNozzleProgress >= 1.0f)
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

// TODO: body unknown (map 0x44, seventeen instructions -- more than the base
// call and the vtable store alone, so at least one field is initialised).
TNozzleButton::TNozzleButton(const char* name, const char* prm,
                             TWaterGun* fludd)
    : TNozzleBase(name, prm, fludd)
{
}

// TODO: body unknown (map 0xc, three instructions).
void TNozzleButton::init() { }

// TODO: body unknown (map 0x28, ten instructions).
void TNozzleButton::movement(const TMarioControllerWork& controllerWork) { }

// Both are 0x4 in the map, i.e. a bare blr: the overrides exist only to make
// the button nozzle inert.
void TNozzleButton::animation(int param_1) { }

void TNozzleButton::emit(int param_1) { }

// Moved out of NozzleDeform.hpp for the same reason as TNozzleTrigger's.
TNozzleDeform::TNozzleDeform(const char* name, const char* prm,
                             TWaterGun* fludd)
    : TNozzleBase(name, prm, fludd)
    , mBomb(name, "/Mario/WaterGun/NozzleDeformBomb.prm", fludd)
{
	init();
}

// Binding level over a raw member read, worth +8 of low region in
// TNozzleDeform::movement (batch 127).
static inline f32 WaterGunUnk378(const TNozzleDeform* p)
{
	f32 v378 = p->unk378;
	return v378;
}

void TNozzleDeform::movement(const TMarioControllerWork& controllerWork)
{
	if (!mFludd->hasWater()) {
		return;
	}

	TNozzleBase::movement(controllerWork);

	unk378 *= mEmitParams.mEmitPowScale.value;

	if (WaterGunUnk378(this) > 1.0f) {
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

	if (mBomb.getSprayState() == TNozzleTrigger::INACTIVE && unk378 > 0.0f) {
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
		if (mFludd->checkFlag(TWaterGun::WATER_GUN_FLAG_UNK2)) {
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

			// TODO: retail reads mFaceAngle.y with `lhz` and re-derives the
			// sin/cos table index twice, i.e. no named s16 angle and no CSE.
			// Spelling it that way (as in TNozzleBase::emit) costs 0x20 of
			// frame here and nets -2%, so the named locals stay.
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
		if (unk36C != 4 && unk36C != 6) {
			if (mFludd->unk1CEC > 0.5f) {
				unk36C = 4;
			} else {
				unk36C = 6;
			}
		}
	} else {
		if (mFludd->isSwitchingToSprayNozzle()) {
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

	J3DFrameCtrl* ctrl = unk380->getFrameCtrl(ANM_TYPE_BCK);

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

		if (isAnmEnd()) {
			unk36C = 3;
		}
		break;
	}
	case 3: {
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(5))
			mactor->setBckFromIndex(5);

		if (!mFludd->isEmitting())
			unk36C = 8;

		break;
	}
	case 8: {
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(6))
			mactor->setBckFromIndex(6);

		if (mFludd->isEmitting() == true)
			unk36C = 2;

		if (isAnmEnd() && !(mFludd->unk1CEC == 0.0f ? true : false))
			unk36C = 0;

		break;
	}
	case 4: {
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(1))
			mactor->setBckFromIndex(1);

		ctrl->setFrame(2.0f * mFludd->mSwitchToSecondNozzleProgress
		               * ctrl->getEnd());
		ctrl->setRate(0.0f);
		break;
	}
	case 5: {
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(0))
			mactor->setBckFromIndex(0);

		ctrl->setFrame((1.0f - 2.0f * mFludd->mSwitchToSecondNozzleProgress)
		               * ctrl->getEnd());
		ctrl->setRate(0.0f);
		if (mFludd->mSwitchToSecondNozzleProgress <= 0.0f) {
			unk36C          = 0;
			mFludd->unk1CEC = 0.0f;
		}
		break;
	}
	case 6: {
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(3))
			mactor->setBckFromIndex(3);

		ctrl->setFrame(2.0f * mFludd->mSwitchToSecondNozzleProgress
		               * ctrl->getEnd());
		ctrl->setRate(0.0f);
		break;
	}
	case 7:
		MActor* mactor = unk380;
		if (!mactor->checkCurBckFromIndex(2))
			mactor->setBckFromIndex(2);

		ctrl->setFrame((1.0f - 2.0f * mFludd->mSwitchToSecondNozzleProgress)
		               * ctrl->getEnd());
		ctrl->setRate(0.0f);
		if (mFludd->mSwitchToSecondNozzleProgress <= 0.0f) {
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

// TODO: bodies unknown. The map gives 0xd0 / 0x134 / 0x4e4: real code, but
// dead by the time the disc was mastered and with no inline site left in the
// TU to recover it from.
TNozzleTurbo::TNozzleTurbo(const char* name, const char* prm, TWaterGun* fludd)
    : TNozzleBase(name, prm, fludd)
{
}

void TNozzleTurbo::movement(const TMarioControllerWork& controllerWork) { }

void TNozzleTurbo::animation(int param_1) { }

TWaterGun::TWaterGun(TMario* mario)
    : mNozzleDeform("normal_wg", "/Mario/WaterGun/NozzleDeform.prm", this)
    , mNozzleRocket(nullptr, "/Mario/WaterGun/NozzleTrgRocket.prm", this)
    , mNozzleUnderWater("hover_wg", "/Mario/WaterGun/NozzleDiving.prm", this)
    , mNozzleYoshiDeform("dummy_wg", "/Mario/WaterGun/NozzleYoshiMouth.prm",
                         this)
    , mNozzleHover("hover_wg", "/Mario/WaterGun/NozzleTrgHover.prm", this)
    , mNozzleTurbo("back_wg", "/Mario/WaterGun/NozzleTrgTurbo.prm", this)
{
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
	mNozzleRocket.unk38C       = MSD_SE_PO_ROCKET_TRIGGER;
	mNozzleTurbo.unk38C        = MSD_SE_PO_SNIPER_TRIGGER;
	mNozzleDeform.mBomb.unk38C = MSD_SE_PO_SHOTGUN_TRIGGER;
	mCurrentWater = mNozzleList[mCurrentNozzle]->mEmitParams.mAmountMax.get();
	mIsEmitWater  = false;
	unk1C88       = 0.0f;
	mCurrentPressure              = 0;
	mPreviousPressure             = 0;
	unk1CEC                       = 1.0f;
	unk1CF0                       = 0.1f;
	unk1CF4                       = 0.0049999999f;
	unk1CF8                       = 0x168;
	unk1CFA                       = 0;
	mSwitchToSecondNozzleProgress = 0.0f;
	mSwitchToSecondNozzleSpeed    = 0.0f;
	unk1D04                       = 0;
	unk1D06                       = -0x1800;
	unk1D08                       = 0;

	mEmitInfo = new TWaterEmitInfo("/Mario/GunEmit.prm");

	unk1D08                         = 0;
	mNozzleDeform.mBomb.unk384      = true;
	mNozzleYoshiDeform.mBomb.unk384 = true;

	// TODO: wrong
	MtxPtr r24 = mMario->mModel->unk8->getAnmMtx(mMario->mJointIdChnChest);

	unk1CBC = mMario->mPosition;

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
	    J3DModelLoaderDataBase::load(fluddModelData,
	                                 J3DMLF_MaterialPEFull
	                                     | (4 << J3DMLF_TevStageNumShift)),
	    0, 1);
	mFluddModel->setModel(fluddModel, 0);

	mFluddModel->getModel()->setBaseTRMtx(
	    mMario->mModel->getModel()->getAnmMtx(mMario->mJointIdChnChest));

	mFluddModel->mModel->calc();

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
			mNozzleList[i]->unk380 = new MActor(nozzleData);

			void* nozzleModelData
			    = JKRFileLoader::getGlbResource(nozzleBmdData.getBmdPath(i));
			J3DModel* nozzleModel = new J3DModel(
			    J3DModelLoaderDataBase::load(
			        nozzleModelData,
			        J3DMLF_MaterialPEFull | (4 << J3DMLF_TevStageNumShift)),
			    0, 1);
			mNozzleList[i]->unk380->setModel(nozzleModel, 0);

			J3DModelData* modelData
			    = mNozzleList[i]->unk380->getModel()->getModelData();

			SMS_ChangeTextureAll(modelData, "H_watergun_main_dummy",
			                     *mFluddModel->getModel()
			                          ->getModelData()
			                          ->getTexture()
			                          ->getResTIMG(1));

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
	                              ->getIndex("chn_muzzle_1"))
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
	unk1D10->init(mFluddModel->mModel, 4);

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

// Reconstructed from the model-creation block of init(), which is where the
// "This is definitely an inlined function" comment above it already pointed.
// Left uncalled so that init()'s codegen is unchanged; whether retail really
// called it is what the size comparison is for.
void TWaterGun::createGunBody()
{
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
}

void TWaterGun::initInLoadAfter() { }

// Defined here, not in the header: the map places this UNUSED 0x1f8 body
// between getEmitMtx and finalDrawInitialize in emission order, i.e. right
// after initInLoadAfter in source order, and that is what numbers the .prm
// path and the nine PARAM_INIT names @4093-@4107 -- before createGunBody's
// model paths and after calcAnimation's animation names.
// TODO: bodies unknown (map 0x34 and 0x24). Both sit between initInLoadAfter
// and the params constructor in the map's order, i.e. among the set-up
// methods, but nothing in this TU or the callers that survive references them.
void TWaterGun::entryAll() { }

void TWaterGun::finalDrawInitialize() { }

TWaterGun::TDeParams::TDeParams()
    : TParams("/Mario/WaterGun.prm")
    , PARAM_INIT(mRocketHeight, 1500.0f)
    , PARAM_INIT(mHHoverHeight, 160.0f)
    , PARAM_INIT(mLAngleNormal, 60.0f)
    , PARAM_INIT(mNozzleAngleYSpeed, 1.0f)
    , PARAM_INIT(mNozzleAngleYBrake, 0.995f)
    , PARAM_INIT(mNozzleAngleYSpeedMax, 0x2000)
    , PARAM_INIT(mHoverRotMax, 0x2000)
    , PARAM_INIT(mHoverSmooth, 0.05f)
    , PARAM_INIT(mChangeSpeed, 0.1f)
{
	TParams::load(mPrmPath);
}

// TODO: Do i really need to explcitly say this?
#pragma dont_inline on
static inline TNozzleBase* WaterGunCurNozzle(const TWaterGun* p)
{
	TNozzleBase* n = p->getCurrentNozzle();
	return n;
}

static inline const TNozzleBmdData* WaterGunBmdData()
{
	const TNozzleBmdData* d = &nozzleBmdData;
	return d;
}

MtxPtr TWaterGun::getEmitMtx(int jointIndex)
{
	MtxPtr result = nullptr;
	if (mMario->onYoshi()) {
		result = mMario->mYoshi->getTongueMtx();
	} else {
		// This entire block is likely an inlined function.
		s32 flag = WaterGunBmdData()->getFlags(mCurrentNozzle, jointIndex);

		switch (flag) {
		case 0:
		case 1:
		case 2:
			result = WaterGunCurNozzle(this)->getMActor()->getModel()->getAnmMtx(
			    WaterGunBmdData()->getJointIndex(mCurrentNozzle, jointIndex));
			break;
		case 3:
			result = mMario->mYoshi->getTongueMtx();
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
	return mFluddModel->mModel->getAnmMtx(unk1CD8);
}

// Reconstructed from perform()'s CUE_CALC_ANIM block, the only place that
// writes mEmitPos. Size-exact at 0xa4 for the loop alone: the
// `unk380->getModel()->setBaseTRMtx(getModel()->getAnmMtx(unk1CD8))` that
// precedes it in perform() is a separate 16-instruction step and does not
// belong here. Left uncalled so that perform()'s codegen is unchanged.
void TWaterGun::setEmitPt()
{
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

// Moved out of WaterGun.hpp: the map has it UNUSED here, and nothing outside
// this TU calls it (every other getModel() in Player is MActor's).
J3DModel* TWaterGun::getModel() { return mFluddModel->mModel; }

// TODO: body unknown (map 0x1c, seven instructions). calcAnimation() picks its
// animations by name, so the id form has no surviving call site.
void TWaterGun::getWaterGunAnmID(int index) { }

static inline TMario* WaterGunGetMario(const TWaterGun* p)
{
	TMario* mario = p->mMario;
	return mario;
}

static inline TNozzleBase* WaterGunCurNozzle2(const TWaterGun* p)
{
	TNozzleBase* n = WaterGunCurNozzle(p);
	return n;
}

void TWaterGun::changeNozzle(TNozzleType nozzleType, bool animate)
{
	f32 usedWater = (f32)mCurrentWater
	                / WaterGunCurNozzle2(this)->mEmitParams.mAmountMax.get();
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
	WaterGunCurNozzle(this)->init();
	if (nozzleType == Yoshi) {
		mCurrentWater = WaterGunGetMario(this)->mYoshi->unkD4;
	} else {
		mCurrentWater
		    = usedWater
		      * WaterGunCurNozzle(this)->mEmitParams.mAmountMax.get();
	}
}

void TWaterGun::movement()
{
	if (!isEmitting()) {
		unk1CC2 = 0;
		unk1CC4 = 0;
	}

	unk1CC8 += (unk1CC2 - unk1CC8) * mWatergunParams.mHoverSmooth.get();
	unk1CCC += (unk1CC4 - unk1CCC) * mWatergunParams.mHoverSmooth.get();

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

// The rotation matrix is the inlined callee's own local, not the caller's:
// an inlined callee's block stacks below the caller's named locals here, and
// the 4-byte gap its boundary leaves under `result` is exactly retail's.
static inline void WaterGunRotateBase(Mtx dst, Mtx src, f32 roll)
{
	Mtx temp;
	MsMtxSetRotRPH(temp, 0.0f, 0.0f, roll);
	MTXConcat(src, temp, dst);
}

void TWaterGun::setBaseTRMtx(Mtx mtx)
{
	Mtx result;

	f32 initialAngle = mtx[1][0];
	if (initialAngle < 0.0f)
		initialAngle = -initialAngle;

	// Seemingly some adjustment of fluddpack angle
	s16 angle = initialAngle * (unk1D04 - unk1D06) + unk1D06;

	f32 angleDegrees = SHORTANGLE2DEG(angle);
	WaterGunRotateBase(result, mtx, angleDegrees);
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
		getCurrentNozzle()->unk380->perform(cue, graphics);
	}
}

static inline TMarDirector* WaterGunGetDirector()
{
	TMarDirector* d = gpMarDirector;
	return d;
}

static inline TNozzleBase* WaterGunGetNozzle(const TWaterGun* gun)
{
	TNozzleBase* nozzle = gun->getCurrentNozzle();
	return nozzle;
}

bool TWaterGun::isEmitting()
{
	if (getCurrentWater() == 0)
		return false;

	if (WaterGunGetDirector()->isDemoMode3() || WaterGunGetDirector()->isDemoMode4()
	    || WaterGunGetDirector()->isTalkModeNow())
		return false;

	if (WaterGunGetNozzle(this)->getNozzleKind() == 1) {
		TNozzleTrigger* trig = (TNozzleTrigger*)WaterGunGetNozzle(this);
		if (trig->getSprayState() == TNozzleTrigger::ACTIVE)
			return true;
		return false;
	}

	if (WaterGunGetNozzle(this)->unk378 > 0.0f)
		return true;

	return false;
}

TNozzleBase* TWaterGun::getCurrentNozzle() const
{
	return mNozzleList[mCurrentNozzle];
}

void TWaterGun::setAmountToRate(f32 rate)
{
	// volatile u32 unused2[7]; // TODO: possibly inlined function
	if (mCurrentNozzle == 3) {
		TNozzleBase* currentNozzle = WaterGunGetNozzle(this);
		s32 amountMax = currentNozzle->mEmitParams.mAmountMax.get();
		mCurrentWater = amountMax;
	} else {
		TNozzleBase* currentNozzle = WaterGunGetNozzle(this);
		mCurrentWater = rate * currentNozzle->mEmitParams.mAmountMax.get();
	}
}

BOOL TWaterGun::isPressureOn()
{
	// volatile u32 unused2[6];
	if (WaterGunGetNozzle(this)->getNozzleKind() == 1) {
		TNozzleTrigger* triggerNozzle
		    = (TNozzleTrigger*)WaterGunGetNozzle(this);
		if (triggerNozzle->unk388 > 0.0f) {
			return TRUE;
		}
	}
	return FALSE;
}

f32 TWaterGun::getPressure()
{
	// TODO: Missing stack space
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

	if (WaterGunGetNozzle(this)->getNozzleKind() == 1) {
		TNozzleTrigger* triggerNozzle
		    = (TNozzleTrigger*)WaterGunGetNozzle(this);
		return triggerNozzle->mEmitParams.mInsidePressureMax.get();
	}

	return 0.0f;
}

// TODO: body unknown (map 0x90, thirty-six instructions). isEmitting() is the
// surviving predicate of that shape but is a separate, larger symbol.
void TWaterGun::getWillBeEmitted() { }

void TWaterGun::getEmitPosDirSpeed(int index, JGeometry::TVec3<f32>* pos,
                                   JGeometry::TVec3<f32>* dir,
                                   JGeometry::TVec3<f32>* speed)
{
	MtxPtr nozzleEmitMtx = getEmitMtx(index);
	pos->set(mEmitPos[index]);

	if (nozzleEmitMtx != nullptr) {
		dir->x = nozzleEmitMtx[0][0];
		dir->y = nozzleEmitMtx[1][0];
		dir->z = nozzleEmitMtx[2][0];
	} else {
		JGeometry::TVec3<f32> forward;
		forward.x = 0.0f;
		forward.y = 0.0f;
		forward.z = 1.0f;
		dir->set(forward);
	}

	speed->x = mMario->mVel.x * 0.125f;
	speed->y = 0.0f;
	speed->z = mMario->mVel.z * 0.125f;
}

void TWaterGun::rotateProp(f32 rotation)
{
	if (mCurrentNozzle == 5) {
		unk1CD2 += rotation * mWatergunParams.mNozzleAngleYSpeed.get();
		unk1CD2 *= mWatergunParams.mNozzleAngleYBrake.get();
		if (mWatergunParams.mNozzleAngleYSpeedMax.get() < unk1CD2) {
			unk1CD2 = mWatergunParams.mNozzleAngleYSpeedMax.get();
		}
		unk1CD0 = unk1CD0 + unk1CD2;
	} else {
		unk1CD2 = 0;
		unk1CD0 = 0;
	}
}

// Reconstructed from the head of calcAnimation(), whose `(mUpperState &
// 0x8000) ? 0 : mUpperState` is exactly six instructions and exactly this
// name. Left uncalled so that calcAnimation()'s codegen is unchanged.
u32 TWaterGun::getMarioUpperStatus()
{
	return (mMario->mUpperState & 0x8000) != 0 ? 0 : mMario->mUpperState;
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

static inline MSound* WaterGunGetMSound()
{
	MSound* sound = SMSGetMSound();
	return sound;
}

// Nested over WaterGunGetMSound at three of emit()'s four sound sites: the
// flat fork alone is +0x30 of frame there and retail's is +0x50 (fireWanwan
// ladder 322's nested-fork rung, tuned down by site count). Which three sites
// carry it is unobservable.
static inline MSound* WaterGunGetMSound2()
{
	MSound* sound = WaterGunGetMSound();
	return sound;
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
			WaterGunGetMSound2()->startSoundActorWithInfo(
			    MSD_SE_PO_NORMAL_NOZZLE_IMI, &getEmitPos0(), nullptr,
			    getCurrentNozzle()->unk374, 0, 0, nullptr, 0, 4);

		case Yoshi:
		case Turbo:
			WaterGunGetMSound2()->startSoundActorWithInfo(
			    0x0, &getEmitPos0(), nullptr, getCurrentNozzle()->unk378, 0, 0,
			    nullptr, 0, 4);
			break;

		case Underwater:
			WaterGunGetMSound2()->startSoundActor(MSD_SE_PO_HOVER, &getEmitPos0(), 0,
			                                nullptr, 0, 4);
			break;

		case Rocket:
			break;

		case Hover:
			if (mIsEmitWater)
				WaterGunGetMSound()->startSoundActor(MSD_SE_PO_HOVER, &getEmitPos0(),
				                                0, nullptr, 0, 4);
			break;
		}
	}
}
BOOL TWaterGun::suck()
{
	if (mCurrentNozzle == (s8)Yoshi) {
		return false;
	} else {
		s32 suckRate = getSuckRate();
		if (suckRate > 0) {
			mCurrentWater += suckRate;

			s32 currentWater = mCurrentWater;
			s32 maxWater     = WaterGunGetNozzle(this)->mEmitParams.mAmountMax.get();
			if (currentWater > maxWater) {
				mCurrentWater = maxWater;
			}

			if (!(mCurrentWater
			      >= WaterGunGetNozzle(this)->mEmitParams.mAmountMax.get())) {
				WaterGunGetMSound()->startSoundActor(
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

		mCurrentWater -= nozzle->mEmitParams.mDamageLoss.value;

		if (mCurrentWater < 0) {
			mCurrentWater = 0;
		}
		return TRUE;
	}
	return FALSE;
}

// Binding level over a raw member read, worth +8 of low region in
// TWaterGun::changeBackup (batch 127).
static inline f32 WaterGunSwitchToSecondNozzleProgress(const TWaterGun* p)
{
	f32 switchToSecondNozzleProgress = p->mSwitchToSecondNozzleProgress;
	return switchToSecondNozzleProgress;
}

void TWaterGun::changeBackup()
{
	// TODO: Missing stack space
	// volatile u32 unused2[5];
	if (WaterGunSwitchToSecondNozzleProgress(this) == 0.0f) {
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

// TODO: bodies unknown (map 0x28 each, ten instructions). The dash effect is
// the turbo nozzle's, and TNozzleTrigger::movement is where it would be
// started and stopped, but neither half survives as a recognisable block.
void TWaterGun::startDashEffect() { }

void TWaterGun::endDashEffect() { }

#include <Player/Mario.hpp>
#include <Player/MarioEffect.hpp>
#include <System/Particles.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/MarDirector.hpp>
#include <Map/MapData.hpp>
#include <MarioUtil/EffectUtil.hpp>
#include <MoveBG/ModelGate.hpp>
#include <M3DUtil/M3UModelMario.hpp>
#include <JSystem/JParticle/JPACallback.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JParticle/JPAParticle.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static const char* dummyMactorStringValue1 = "\0\0\0\0\0\0\0\0\0\0\0";
static const char* SMS_NO_MEMORY_MESSAGE   = "メモリが足りません\n";

#include <Player/MarioDirtyStrings.hpp>

static const char* MtxCalcTypeName[] = {
	"MActorMtxCalcType_Basic クラシックスケールＯＮ",
	"MActorMtxCalcType_Softimage クラシックスケールＯＦＦ",
	"MActorMtxCalcType_MotionBlend モーションブレンド",
	"MActorMtxCalcType_User ユーザー定義",
};

const char* cParticleFileNames[] = {
	"/scene/map/pollution/ms_m_ashios.jpa",
	"/scene/map/pollution/ms_m_spinos.jpa",
	"/scene/map/pollution/ms_m_tokeos.jpa",
};

static const s32 cParticleIDs[] = {
	MAP_POLLUTION_MS_M_ASHIOS,
	MAP_POLLUTION_MS_M_SPINOS,
	MAP_POLLUTION_MS_M_TOKEOS,
};

// `fileName` is declared before `i` because the callee-saved order is
// i (r31), the 4*i offset (r30), the three pool bases (r29-r27),
// SMS_LoadParticle's flag pointer (r26), fileName (r25), and that order only
// comes out with both locals at function scope in this order.
// TODO: 98.7%. The three pool bases sit one register low (r28-r26) and the
// flag pointer one high (r29), i.e. the documented pool-base-versus-callee-temp
// rotation, at zero frame cost. Block scope for `i` is inert, as is `i++`.
void TMario::initParticle()
{
	const char* fileName;
	int i;
	for (i = 0; i < 3; ++i) {
		fileName = cParticleFileNames[i];
		if (JKRFileLoader::getGlbResource(fileName)) {
			if (i < 1)
				SMS_LoadParticle(fileName, cParticleIDs[i]);
			else
				SMS_LoadParticle(fileName, cParticleIDs[i]);
		}
	}
}

void TMario::moveParticle()
{
	if (mWaterWakeAlpha > 0) {
		JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_M_HAMON_D, unk220, 1, this);
		if (emitter != nullptr) {
			emitter->setGlobalAlpha(mWaterWakeAlpha);
			mWaterWakeAlpha -= mParticleParams.mWaveAlphaDec.get();
		}
	}
}

bool TMario::emitParticle(int id)
{
	return gpMarioParticleManager->emit(id, &mPosition, 0, nullptr) == nullptr
	           ? false
	           : true;
}

bool TMario::emitParticle(int id, const JGeometry::TVec3<f32>* pos)
{
	return gpMarioParticleManager->emit(id, pos, 0, nullptr) == nullptr ? false
	                                                                    : true;
}

bool TMario::emitParticle(int id, s16 rot)
{
	return gpMarioParticleManager->emitWithRotate(id, &mPosition, 0, rot, 0, 0,
	                                              nullptr)
	               == nullptr
	           ? false
	           : true;
}

void TMario::emitSmoke(s16 rot)
{
	if (!mGroundPlane->isPool())
		gpMarioParticleManager->emitWithRotate(
		    PARTICLE_MS_MARIWALK1_A, &mPosition, 0, rot, 0, 0, nullptr);
}

#pragma dont_inline on
void TMario::emitSweat(s16 rot)
{
	if (!checkFlag(MARIO_FLAG_HELMET_FLW_CAMERA)
	    && !checkFlag(MARIO_FLAG_IN_ANY_WATER) && !isUnderWater()) {
		JGeometry::TVec3<f32> pos;
		MtxPtr mtx = getM3UModel()->getModel()->getAnmMtx(mJointIdHead);
		pos.x      = mtx[0][3];
		pos.y      = mtx[1][3];
		pos.z      = mtx[2][3];
		gpMarioParticleManager->emitWithRotate(PARTICLE_MS_ASE, &pos, 0, rot, 0,
		                                       0, nullptr);
	}
}
#pragma dont_inline off

void TMario::emitSweatSometimes()
{
	s16 angle = mFaceAngle.y;
	if (!(gpMarDirector->unk58 & 0xF))
		emitSweat(angle);
}

void TMario::emitGetEffect()
{
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_ITEMGET1_A, &unk160,
	                                            0, nullptr);
	startSoundActor(0x1989);
}

void TMario::emitGetWaterEffect()
{
	JGeometry::TVec3<f32>* pos = &unk160;
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_ITEMGET1_B, pos, 0,
	                                            nullptr);
}

void TMario::emitGetCoinEffect(JGeometry::TVec3<f32>* pos)
{
	gpMarioParticleManager->emit(PARTICLE_MS_COINGET_A, pos, 0, nullptr);
	gpMarioParticleManager->emit(PARTICLE_MS_COINGET_B, pos, 0, nullptr);
}

void TMario::strongTouchDownEffect()
{
	gpMarioParticleManager->emitWithRotate(PARTICLE_MS_JUMP_ED_A, &mPosition, 0,
	                                       mFaceAngle.y, 0, 0, nullptr);
	// Why?
	s16 angle = mFaceAngle.y;
	gpMarioParticleManager->emitWithRotate(PARTICLE_MS_JUMP_ED_B, &mPosition, 0,
	                                       angle, 0, 0, nullptr);
}

void TMario::smallTouchDownEffect()
{
	JPABaseEmitter* emitter = gpMarioParticleManager->emit(
	    PARTICLE_MS_JUMP_ED_B, &mPosition, 0, nullptr);

	static JGeometry::TVec3<f32> scale(0.7f, 0.7f, 0.7f);

	if (emitter != nullptr)
		emitter->setGlobalScale(scale);
}

void TMario::rippleEffect()
{
	if (checkFlag(MARIO_FLAG_IN_SHALLOW_WATER)) {
		SMS_EmitRipplePool(unk220, this);
	} else {
		SMS_EmitRippleSea(unk220, this);
		if ((checkStatusType(MARIO_STATUS_FLAG_SWIMMING))) {
			f32 waveEmitSpeed = mParticleParams.mWaveEmitSpeed.get();
			if (mForwardVel > waveEmitSpeed)
				mWaterWakeAlpha = 0xFF;
		}
	}
}

void TMario::inOutWaterEffect(f32 waterY)
{
	JGeometry::TVec3<f32> pos = getPosition();
	pos.y                     = mFloorPosition.z;

	if (checkFlag(MARIO_FLAG_IN_SHALLOW_WATER)
	    || checkPrevFlag(MARIO_FLAG_IN_SHALLOW_WATER)) {
		mMarioEffect->setJumpIntoWaterEffectSmall();
		gpMarioParticleManager->emit(PARTICLE_MS_M_TOBIKOMI_S_A, &pos, 0,
		                             nullptr);
		return;
	}

	mMarioEffect->setJumpIntoWaterEffect();

	f32 absVelY = mVel.y;
	if (absVelY < 0.0f)
		absVelY = -absVelY;

	if (absVelY > mWaterEffectParams.mJumpIntoMdlEffectSpY.get()) {
		rumbleStart(0x15, mMotorParams.mMotorWall.get());

		if (!checkStatusType(MARIO_STATUS_FLAG_UNK200))
			gpMarioParticleManager->emit(PARTICLE_MS_M_TOBIKOMI_A, &pos, 0,
			                             nullptr);

		gpMarioParticleManager->emit(PARTICLE_MS_M_TOBIKOMI_B, &pos, 0,
		                             nullptr);
		gpMarioParticleManager->emit(PARTICLE_MS_M_TOBIKOMI_C, &pos, 2,
		                             nullptr);
		return;
	}

	if (!checkStatusType(MARIO_STATUS_FLAG_UNK200))
		gpMarioParticleManager->emit(PARTICLE_MS_M_TOBIKOMI_S_A, &pos, 0,
		                             nullptr);

	gpMarioParticleManager->emit(PARTICLE_MS_M_TOBIKOMI_S_B, &pos, 0, nullptr);
	gpMarioParticleManager->emit(PARTICLE_MS_M_TOBIKOMI_S_C, &pos, 2, nullptr);
	return;
}

struct TBubbleCallBack
    : public JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*> {
	void execute(JPABaseEmitter*, JPABaseParticle*);
};

void TBubbleCallBack::execute(JPABaseEmitter*, JPABaseParticle* particle)
{
	if (!SMSGetMarioOriginal()->checkFlag(MARIO_FLAG_HELMET_FLW_CAMERA)) {
		JGeometry::TVec3<f32> pos;
		particle->getCurrentPosition(pos);
		if (pos.y > gpMarioOriginal->mFloorPosition.z) {
			particle->unk10 |= 2;
			if (gpMarioOriginal->mParticleParams.mBubbleToRipple.get()) {
				gpMarioParticleManager->emit(PARTICLE_MS_M_AWAHAMON, &pos, 0,
				                             nullptr);
			}
		}
	}
}

TBubbleCallBack bubbleCallBack;

void TMario::bubbleFromMouth(int idx)
{
	if (isMario()) {
		JPABaseEmitter* emitter = gpMarioParticleManager->emitParticleCallBack(
		    PARTICLE_MS_M_AWA, &mHeadPos, 1, &bubbleCallBack, &this[idx]);
		if (emitter != nullptr)
			emitter->setGlobalRTMatrix(mHeadMtx);
	}
}

void TMario::bubbleFromBody()
{
	if (isMario()) {
		f32 spMax = mParticleParams.mBodyBubbleSpMax.get();
		f32 vel   = mForwardVel;
		f32 t     = 0.0f;
		f32 spMin = mParticleParams.mBodyBubbleSpMin.get();
		if (vel > spMax)
			t = 1.0f;
		else if (vel > spMin)
			t = (vel - spMin) / (spMax - spMin);
		f32 emitMin = mParticleParams.mBodyBubbleEmitMin.get();
		f32 spawnRate
		    = (t * (mParticleParams.mBodyBubbleEmitMax.get() - emitMin))
		      + emitMin;
		JPABaseEmitter* emitter = gpMarioParticleManager->emitParticleCallBack(
		    PARTICLE_MS_M_AWA_S, &mCenterPos, 1, &bubbleCallBack, this);
		if (emitter != nullptr) {
			emitter->setGlobalRTMatrix(getCenterAnmMtx());
			emitter->setRate(spawnRate);
		}
	}
}

void TMario::swimmingBubbleEffect()
{
	if (isMario()) {
		if (!checkFlag(MARIO_FLAG_GAME_OVER)) {
			if ((mHeadPos.y + mParticleParams.mBubbleDepth.get())
			        < mFloorPosition.z
			    && isMario()) {
				JPABaseEmitter* emitter
				    = gpMarioParticleManager->emitParticleCallBack(
				        PARTICLE_MS_M_AWA, &mHeadPos, 1, &bubbleCallBack, this);
				if (emitter != nullptr)
					emitter->setGlobalRTMatrix(mHeadMtx);
			}
			if ((mCenterPos.y + mParticleParams.mBubbleDepth.get())
			    < mFloorPosition.z) {
				bubbleFromBody();
			}
		}
	}
}

void TMario::runningRippleEffect()
{
	if (mForwardVel > 30.0f) {
		gpMarioParticleManager->emit(PARTICLE_MS_M_WATRUN_A, &mWaterRipplePos,
		                             0, nullptr);
	}
	SMS_EmitRippleTiny(&mWaterRipplePos);
}

void TMario::blurEffect()
{
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_M_BLUR1, mModel->getModel()->getAnmMtx(1), 1, this);
}

void TMario::wallSlipEffect()
{
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_MARIWALLSL1,
	                                            &mPosition, 1, this);
}

void TMario::treeSlipEffect()
{
	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_MARIWALLSL1, getCenterAnmMtx(), 1, this);

	static JGeometry::TVec3<f32> scale(0.8f, 0.8f, 0.8f);

	if (emitter != nullptr)
		emitter->setGlobalScale(scale);
}

void TMario::frontSlipEffect()
{
	if (getGroundPlane()->isWetGround()
	    || (mStatus == MARIO_STATUS_CATCH && mStatusState == 1)) {
		gpMarioParticleManager->emitAndBindToMtxPtr(PARTICLE_MS_M_WATSLIDE_A,
		                                            getCenterAnmMtx(), 3, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(PARTICLE_MS_M_WATSLIDE_B,
		                                            getCenterAnmMtx(), 1, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(PARTICLE_MS_M_WATSLIDE_C,
		                                            getCenterAnmMtx(), 1, this);
		return;
	}

	if (!(mPosition.y < mFloorPosition.z)) {
		if (checkFlag(MARIO_FLAG_ON_SAND)) {
			calcGroundMtx(mCenterPos);
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    PARTICLE_MS_M_SLIDESAND_B, unk250, 1, this);
			gpMarioParticleManager->emitAndBindToPosPtr(
			    PARTICLE_MS_M_SLIDESAND_A, &mCenterPos, 1, this);
			return;
		}
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_M_SLIPSMOKE, mModel->getModel()->getAnmMtx(0), 1, this);
	}
}

void TMario::surfingEffect()
{
	f32 scale = 1.0f;
	f32 sMin  = getSurfingParamsWater()->mScaleMin.get();
	f32 sMax  = getSurfingParamsWater()->mScaleMax.get();
	f32 spMin = getSurfingParamsWater()->mScaleMinSpeed.get();
	f32 spMax = getSurfingParamsWater()->mScaleMaxSpeed.get();
	if (mForwardVel < spMin)
		scale = sMin;
	if (spMin <= mForwardVel && mForwardVel <= spMax)
		scale
		    = sMin + ((sMax - sMin) * (mForwardVel - spMin)) / (spMax - spMin);
	if (spMax < mForwardVel)
		scale = sMax;

	// TODO: 99.5%, all 107 instructions exact; frame 0x70 vs 0x60, i.e. our
	// dead low region is 16 bytes too *big* (retail 0xc..0x3c, ours 0xc..0x4c,
	// neither referenced -- `scaleVec` is fully scalarised into f31 in both).
	// A dead named `TVec3` here is only +8, so the surplus is not `scaleVec`
	// as a slot; passing `TVec3(scale, scale, scale)` as a temporary at the
	// four sites is far worse (frame 0xb8), and the `SMS_EasyEmitParticle`
	// template, whose body is exactly these four blocks and whose
	// type-to-flag map gives retail's 3/1/1/1, cannot be it either: a function
	// template without `inline` never inlines, so it emits a `bl` (58.2%).
	// The `addi r5` / `addi r7` order at the last three sites (retail computes
	// the MtxPtr argument before `this`) is part of the same residue.
	JGeometry::TVec3<f32> scaleVec(scale, scale, scale);
	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_GESOSURF_A, (MtxPtr)getRootAnmMtx(), 3, this);
	if (emitter != nullptr)
		emitter->setGlobalScale(scaleVec);
	emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_GESOSURF_B, unk1F0, 1, this);
	if (emitter != nullptr)
		emitter->setGlobalScale(scaleVec);
	emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_GESOSURF_D, unk1F0, 1, this);
	if (emitter != nullptr)
		emitter->setGlobalScale(scaleVec);
	emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_GESOSURF_C, unk1F0, 1, this);
	if (emitter != nullptr)
		emitter->setGlobalScale(scaleVec);
}

struct TWarpInCallBack
    : public JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*> {
	void execute(JPABaseEmitter*, JPABaseParticle*);
};

void TWarpInCallBack::execute(JPABaseEmitter* emitter,
                              JPABaseParticle* particle)
{
	// f31/f30/f29/f28/f27 are five named scalar locals: the callee-saved FPRs
	// go out f31-down in declaration order, which is what fixes `timer` first
	// and `factor` last, and `factor += 1.0f` as its own statement is what
	// keeps the product out of an `fmadds` and adds into factor's register.
	// TODO: 73.9%, frame 0xc8 vs 0x110. Every instruction of the head and the
	// tail matches; the residue is one 12-byte temporary too many per scale
	// step. Retail runs six slots -- scale target, temporary, scale target,
	// temporary, scale target, temporary -- i.e. `operator*`'s by-value
	// parameter and its return temporary per step, with the return temporary
	// copied into the next parameter. The three named `v` locals give us nine
	// slots (parameter, return temporary, named local), and the pure chain
	// `*vel * tmp * timer * factor` gives four, because MWCC then builds each
	// inner result directly in the next parameter slot. Nothing in between was
	// found: not a TU-local `mulVec(const TVec3&, f32)` copying into a local
	// (39.1%, NRVO drops a copy), not `f(TVec3, f32)` by value, not `*=` on
	// fresh copies (one copy per step). A member `TVec3 operator*(f32) const`
	// in JGVec3.hpp, instead of the by-value `friend`, is the spelling whose
	// expansion has exactly retail's six slots -- a shared-header change, so
	// it is reported rather than made.
	JGeometry::TVec3<f32>* vel = (JGeometry::TVec3<f32>*)emitter->getUserWork();

	f32 timer = (f32)gpMarioOriginal->mStatusTimer;
	f32 velX  = particle->unk14.x;
	f32 velY  = particle->unk14.y;
	f32 velZ  = particle->unk14.z;

	f32 factor = ((((int)particle >> 2) & 0x3F) / 16.0f);
	factor += 1.0f;

	JGeometry::TVec3<f32> v1 = *vel * gpMarioOriginal->unk468;
	JGeometry::TVec3<f32> v2 = v1 * timer;
	JGeometry::TVec3<f32> v3 = v2 * factor;

	velX += v3.x;
	velY += v3.y;
	velZ += v3.z;
	particle->unk14.set(velX, velY, velZ);
}

TWarpInCallBack warpInCallBack;

static const s32 warpInEffectIDs[] = {
	MAP_MAP_GATE_MS_MARIOWP_BODY,  MAP_MAP_GATE_MS_MARIOWP_HEAD,
	MAP_MAP_GATE_MS_MARIOWP_CAP,   MAP_MAP_GATE_MS_MARIOWP_RHAND,
	MAP_MAP_GATE_MS_MARIOWP_LHAND, MAP_MAP_GATE_MS_MARIOWP_RLEG,
	MAP_MAP_GATE_MS_MARIOWP_RFOOT, MAP_MAP_GATE_MS_MARIOWP_LLEG,
	MAP_MAP_GATE_MS_MARIOWP_LFOOT, MAP_MAP_GATE_MS_MARIOWP_WATGUN,
};

void TMario::warpInEffect()
{
	for (int i = 0; i < 10; i++) {
		u16 boneIdx;
		switch (i) {
		case 0:
			boneIdx = mJointIdCenter;
			break;
		case 1:
			boneIdx = mJointIdHead;
			break;
		case 2:
			boneIdx = mJointIdHead;
			break;
		case 3:
			boneIdx = mJointIdHandR;
			break;
		case 4:
			boneIdx = mJointIdHandL;
			break;
		case 5:
			boneIdx = mJointIdChnFootR;
			break;
		case 6:
			boneIdx = mJointIdFootR;
			break;
		case 7:
			boneIdx = mJointIdChnFootL;
			break;
		case 8:
			boneIdx = mJointIdFootL;
			break;
		case 9:
		default:
			boneIdx = mJointIdCenter;
			break;
		}

		MtxPtr mtx = getM3UModel()->getModel()->getAnmMtx(boneIdx);

		s32 id = warpInEffectIDs[i];
		BOOL b = TRUE;
		if (id == 0x23 && !checkFlag(MARIO_FLAG_HAS_FLUDD))
			b = FALSE;

		if (b == TRUE) {
			JPABaseEmitter* emitter
			    = gpMarioParticleManager->emitAndBindToMtx(id, mtx, 0, this);
			if (emitter != nullptr) {
				emitter->setParticleCallBackPtr(&warpInCallBack);
				uintptr_t work = (uintptr_t)&mWarpInDir;
				emitter->setUserWork(work);
			}
		}
	}

	gpMarioParticleManager->emitAndBindToMtx(MAP_MAP_GATE_MS_MARIOWP_DUST,
	                                         getCenterAnmMtx(), 0, nullptr);
	gpMarioParticleManager->emitAndBindToMtx(
	    0x1D6,
	    ((TModelGate*)getHolder())
	        ->unk78->getModel()
	        ->getAnmMtx(((TModelGate*)getHolder())->unk72),
	    2, getHolder());
}

void TMario::warpInLight()
{
	gpMarioParticleManager->emitAndBindToPosPtr(MAP_MAP_GATE_MS_MARIOWP_SENKO,
	                                            &mCenterPos, 0, this);
}

// Binding level worth +8 of low region, landing TMario::warpOutEffect's
// frame at 0x120 (batch 124).
static inline bool MarioParticleCheckFlagL0(const TMario* p, u32 i)
{
	bool flag = p->checkFlag(i);
	return flag;
}

static inline bool MarioParticleCheckFlag(const TMario* p, u32 i)
{
	bool flag = MarioParticleCheckFlagL0(p, i);
	return flag;
}

// TODO: 8 bytes of low region short, every instruction exact (retail 0x120,
// ours 0x118; the double-conversion temporary sits at the top of the local
// area in both). Routing the twelve `mModel->getModel()->getAnmMtx()` receivers
// through getM3UModel() is worth +8 per *pair* of sites, so two or three of the
// twelve land the frame exactly and all twelve overshoot by 0x28 -- an
// arbitrary subset of identical sites, so it is not committed. Also measured
// and inert: getPosition() at either &mPosition argument (it also costs
// instructions), getM3UModel() at the first site alone.
void TMario::warpOutEffect(int kind, f32 rotDeg)
{
	switch (kind) {
	case 0:
		gpMarioParticleManager->emitWithRotate(
		    PARTICLE_MS_M_GOUT_GATE, &mPosition, 0, DEG2SHORTANGLE(rotDeg), 0,
		    0, this);
		break;

	case 1:
		gpMarioParticleManager->emitWithRotate(
		    PARTICLE_MS_M_GOUT_DOKAN, &mPosition, 0, DEG2SHORTANGLE(rotDeg), 0,
		    0, this);
		break;

	case 2:
	default:
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_MARIOAP_BODY,
		    mModel->getModel()->getAnmMtx(mJointIdCenter), 0, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_MARIOAP_HEAD,
		    mModel->getModel()->getAnmMtx(mJointIdHead), 0, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_MARIOAP_CAP,
		    mModel->getModel()->getAnmMtx(mJointIdHead), 0, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_MARIOAP_RHAND,
		    mModel->getModel()->getAnmMtx(mJointIdHandR), 0, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_MARIOAP_LHAND,
		    mModel->getModel()->getAnmMtx(mJointIdHandL), 0, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_MARIOAP_RLEG,
		    mModel->getModel()->getAnmMtx(mJointIdChnFootR), 0, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_MARIOAP_RFOOT,
		    mModel->getModel()->getAnmMtx(mJointIdFootR), 0, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_MARIOAP_LLEG,
		    mModel->getModel()->getAnmMtx(mJointIdChnFootL), 0, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_MARIOAP_LFOOT,
		    mModel->getModel()->getAnmMtx(mJointIdFootL), 0, this);
		if (MarioParticleCheckFlag(this, MARIO_FLAG_HAS_FLUDD)) {
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    PARTICLE_MS_MARIOAP_WATGUN,
			    mModel->getModel()->getAnmMtx(mJointIdCenter), 0, this);
		}
		break;
	}
}

void TMario::emitBlurHipDrop()
{
	gpMarioParticleManager->emitAndBindToMtxPtr(PARTICLE_MS_M_BLUR3,
	                                            getCenterAnmMtx(), 1, this);
}

void TMario::emitBlurHipDropSuper()
{
	gpMarioParticleManager->emitAndBindToMtxPtr(PARTICLE_MS_M_SPHIPD_A,
	                                            getCenterAnmMtx(), 1, this);
	gpMarioParticleManager->emitAndBindToMtxPtr(PARTICLE_MS_M_SPHIPD_B,
	                                            getCenterAnmMtx(), 1, this);
	gpMarioParticleManager->emitAndBindToMtxPtr(PARTICLE_MS_M_SPHIPD_C,
	                                            getCenterAnmMtx(), 1, this);
	gpMarioParticleManager->emitAndBindToMtxPtr(PARTICLE_MS_M_SPHIPD_D,
	                                            getCenterAnmMtx(), 1, this);
}

void TMario::emitBlurSpinJump()
{
	gpMarioParticleManager->emitAndBindToMtxPtr(PARTICLE_MS_M_BLUR2,
	                                            getCenterAnmMtx(), 1, this);
	gpMarioParticleManager->emitAndBindToMtxPtr(PARTICLE_MS_M_BLUR2SP,
	                                            getCenterAnmMtx(), 1, this);
	if (mDirty > 0.0f) {
		JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
		    MAP_POLLUTION_MS_M_SPINOS, getCenterAnmMtx(), 1, this);
		if (emitter != nullptr) {
			emitter->setRate(0.003921569f * (1.75f * mDirty));
		}
	}
}

void TMario::emitRotateShootEffect()
{
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_M_SPINSHOT_A,
	                                            &mCenterPos, 1, this);
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_M_SPINSHOT_B,
	                                            &mCenterPos, 1, this);
}

void TMario::emitFootPrintWithEffect(int effectId, int printId)
{
	int foot   = 2;
	MtxPtr mtx = nullptr;
	if (getStatus() == MARIO_STATUS_RUN) {
		if (onYoshi()) {
			if (mYoshi->getFrameCtrl()->checkPass(47.0f)) {
				mtx  = mYoshi->getMtxPtrFootL();
				foot = 0;
			}
			if (mYoshi->getFrameCtrl()->checkPass(16.0f)) {
				mtx  = mYoshi->getMtxPtrFootR();
				foot = 1;
			}
		} else {
			if (getMotionFrameCtrl().checkPass(38.0f)) {
				foot = 0;
				mtx  = mModel->getModel()->getAnmMtx(mJointIdFootL);
			}
			if (getMotionFrameCtrl().checkPass(8.0f)) {
				foot = 1;
				mtx  = mModel->getModel()->getAnmMtx(mJointIdFootR);
			}
		}
	}

	if (getStatus() == MARIO_STATUS_WAIT && onYoshi()) {
		if (mYoshi->getFrameCtrl()->checkPass(20.0f)
		    || mYoshi->getFrameCtrl()->checkPass(71.0f)
		    || mYoshi->getFrameCtrl()->checkPass(134.0f)) {
			mtx  = mYoshi->getMtxPtrFootL();
			foot = 0;
		}
		if (mYoshi->getFrameCtrl()->checkPass(45.0f)
		    || mYoshi->getFrameCtrl()->checkPass(102.0f)
		    || mYoshi->getFrameCtrl()->checkPass(134.0f)) {
			mtx  = mYoshi->getMtxPtrFootR();
			foot = 1;
		}
	}

	if (mtx != nullptr && foot != 2) {
		mFootprintPos.x = mtx[0][3];
		mFootprintPos.y = mtx[1][3];
		mFootprintPos.z = mtx[2][3];

		// wtf is this bs?
		u32 b2 = printId > 0;
		u32 b  = mForwardVel > 20.0f;
		if (getStatus() == MARIO_STATUS_RUN && b && b2)
			gpMarioParticleManager->emit(printId, &mFootprintPos, 0, nullptr);

		if (effectId > 0) {
			calcGroundMtx(mFootprintPos);
			gpMarioParticleManager->emitAndBindToMtx(effectId, unk250, 0,
			                                         nullptr);
		}
	}
}

void TMario::emitDirtyFootPrint()
{
	emitFootPrintWithEffect(MAP_POLLUTION_MS_M_ASHIOS, -1);
}

void TMario::emitSandEffect()
{
	emitFootPrintWithEffect(PARTICLE_MS_M_WALKSAND_B, 0x3A);
}

void TMario::meltInWaterEffect()
{
	if (mDirty > 0.0f) {
		JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
		    MAP_POLLUTION_MS_M_TOKEOS, unk220, 1, this);
		if (emitter != nullptr) {
			emitter->setRate(
			    0.003921569f
			    * (mDirty * mParticleParams.mMeltInWaterMax.get()));
			if (checkFlag(MARIO_FLAG_IN_SHALLOW_WATER)) {
				JGeometry::TVec3<f32> scale(0.6f, 0.6f, 0.6f);
				emitter->setGlobalScale(scale);
			}
		}
	}
}

void TMario::rocketEffectStart()
{
	gpMarioParticleManager->emit(PARTICLE_MS_M_ROCKET_D, &mPosition, 0,
	                             nullptr);
	gpMarioParticleManager->emit(PARTICLE_MS_M_ROCKET_C, &mPosition, 0,
	                             nullptr);
	gpMarioParticleManager->emit(PARTICLE_MS_HIPDROP_A, &mPosition, 0, nullptr);
	if (mWaterGun != nullptr) {
		gpMarioParticleManager->emitAndBindToPosPtr(
		    PARTICLE_MS_M_ROCKET_A, &mWaterGun->getEmitPos0(), 0, this);
		gpMarioParticleManager->emitAndBindToPosPtr(
		    PARTICLE_MS_M_ROCKET_B, &mWaterGun->getEmitPos0(), 0, this);
		gpMarioParticleManager->emitAndBindToPosPtr(
		    PARTICLE_MS_M_ROCKET_B2, &mWaterGun->getEmitPos0(), 0, this);
	}
}

void TMario::elecEffect()
{
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_M_BIRI_A,
	                                            &mCenterPos, 1, this);
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_M_BIRI_C,
	                                            &mCenterPos, 1, this);
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_M_BIRI_B,
	                                            &mCenterPos, 1, this);
}

void TMario::elecEndEffect()
{
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_MOE_FIRE_OFF,
	                                            &mCenterPos, 0, this);
}

void TMario::kickRoofEffect()
{
	if (getMotionFrameCtrl().checkPass(8.0f)) {
		MtxPtr mtx      = getM3UModel()->getModel()->getAnmMtx(mJointIdChnFootR);
		mFootprintPos.x = mtx[0][3];
		mFootprintPos.y = mtx[1][3];
		mFootprintPos.z = mtx[2][3];
		gpMarioParticleManager->emit(PARTICLE_MS_M_AMIATTACK, &mFootprintPos, 0,
		                             nullptr);
		rumbleStart(0x15, mMotorParams.mMotorWall.get());
	}
}

void TMario::sleepingEffect()
{
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_POI_ZZZ, &unk1B4, 1,
	                                            this);
}

void TMario::sleepingEffectKill()
{
	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToPosPtr(
	    PARTICLE_MS_POI_ZZZ, &unk1B4, 1, this);
	if (emitter != nullptr)
		emitter->deleteAllParticle();
}

void TMario::toroccoEffect()
{
	f32 dist = JGeometry::TVec3<f32>(getPosition() - mToroccoPos).length();

	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_TORO_WIND, mTorocco->getModel()->getAnmMtx(0), 1, this);
	if (emitter != nullptr) {
		emitter->setRate(dist * mParticleParams.mToroccoWind.get());
	}
	emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_TORO_HIBANA, mTorocco->getModel()->getAnmMtx(0), 1, this);
	if (emitter != nullptr) {
		emitter->setRate(dist * mParticleParams.mToroccoSpark.get());
	}
}

void TMario::kickFruitEffect()
{
	JPABaseEmitter* emitter = gpMarioParticleManager->emit(
	    PARTICLE_MS_M_AMIATTACK, &mPosition, 0, nullptr);
	if (emitter != nullptr) {
		JGeometry::TVec3<f32> scale(1.2f, 1.2f, 1.2f);
		emitter->setGlobalScale(scale);
		JGeometry::TVec3<f32> pos = mPosition;
		pos.y += 30.0f;
		emitter->setGlobalTranslation(pos);
	}
}

void TMario::sinkInSandEffect()
{
	JPABaseEmitter* emitter = gpMarioParticleManager->emit(
	    PARTICLE_MS_POI_SAND, &mPosition, 0, nullptr);
	if (emitter)
		emitter->setGlobalScale(JGeometry::TVec3<f32>(0.5f, 0.5f, 0.5f));
}

bool TMario::askJumpIntoWaterEffectExist() const
{
	TMarioEffect* effect = mMarioEffect;
	if (effect->unk6C[0] == 1)
		return true;
	if (effect->unk6C[1] == 1)
		return true;
	return false;
}

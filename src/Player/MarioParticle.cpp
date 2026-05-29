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

const char cDirtyFileName[] = "/scene/map/pollution/H_ma_rak.bti";
const char cDirtyTexName[]  = "H_ma_rak_dummy";

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

void TMario::initParticle()
{
	for (int i = 0; i < 3; ++i) {
		const char* fileName = cParticleFileNames[i];
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
			emitter->unk180.a = mWaterWakeAlpha;
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
		MtxPtr mtx = mModel->getModel()->getAnmMtx(mBoneIDs[10]);
		JGeometry::TVec3<f32> pos;
		pos.x = mtx[0][3];
		pos.y = mtx[1][3];
		pos.z = mtx[2][3];
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
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_ITEMGET1_B, &unk160,
	                                            0, nullptr);
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
		emitter->setScale(scale);
}

void TMario::rippleEffect()
{
	if (checkFlag(MARIO_FLAG_IN_SHALLOW_WATER)) {
		SMS_EmitRipplePool(unk220, this);
	} else {
		SMS_EmitRippleSea(unk220, this);
		if ((checkStatusFlag(MARIO_STATUS_FLAG_SWIMMING))
		    && mForwardVel > mParticleParams.mWaveEmitSpeed.get()) {
			mWaterWakeAlpha = 0xFF;
		}
	}
}

void TMario::inOutWaterEffect(f32 waterY)
{
	JGeometry::TVec3<f32> pos = mPosition;
	pos.y                     = mFloorPosition.z;

	if (checkFlag(MARIO_FLAG_IN_SHALLOW_WATER)
	    || (unk11C & 0x10000 ? true : false)) {
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

		if (!checkStatusFlag(MARIO_STATUS_FLAG_UNK200))
			gpMarioParticleManager->emit(PARTICLE_MS_M_TOBIKOMI_A, &pos, 0,
			                             nullptr);

		gpMarioParticleManager->emit(PARTICLE_MS_M_TOBIKOMI_B, &pos, 0,
		                             nullptr);
		gpMarioParticleManager->emit(0x1D4, &pos, 2, nullptr);
		return;
	}

	if (!checkStatusFlag(MARIO_STATUS_FLAG_UNK200))
		gpMarioParticleManager->emit(PARTICLE_MS_M_TOBIKOMI_S_A, &pos, 0,
		                             nullptr);

	gpMarioParticleManager->emit(PARTICLE_MS_M_TOBIKOMI_S_B, &pos, 0, nullptr);
	gpMarioParticleManager->emit(0x1D5, &pos, 2, nullptr);
	return;
}

struct TBubbleCallBack
    : public JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*> {
	void execute(JPABaseEmitter*, JPABaseParticle*);
};

void TBubbleCallBack::execute(JPABaseEmitter*, JPABaseParticle* particle)
{
	if (!gpMarioOriginal->checkFlag(MARIO_FLAG_HELMET_FLW_CAMERA)) {
		JGeometry::TVec3<f32> pos;
		particle->getCurrentPosition(pos);
		if (pos.y > gpMarioOriginal->mFloorPosition.z) {
			particle->unk10 |= 2;
			if (gpMarioOriginal->mParticleParams.mBubbleToRipple.get()
			    != 0.0f) {
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
		    PARTICLE_MS_M_AWA, &unk16C, 1, &bubbleCallBack,
		    (const void*)((u8*)this + idx * 0x4290));
		if (emitter != nullptr) {
			emitter->setGlobalRTMatrix(unk1C0);
		}
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
		    PARTICLE_MS_M_AWA_S, &unk178, 1, &bubbleCallBack, this);
		if (emitter != nullptr) {
			emitter->setGlobalRTMatrix(getCenterAnmMtx());
			emitter->mChildSpawnRate = spawnRate;
		}
	}
}

void TMario::swimmingBubbleEffect()
{
	if (isMario()) {
		if (!checkFlag(MARIO_FLAG_GAME_OVER)) {
			if ((unk16C.y + mParticleParams.mBubbleDepth.get())
			        < mFloorPosition.z
			    && isMario()) {
				JPABaseEmitter* emitter
				    = gpMarioParticleManager->emitParticleCallBack(
				        PARTICLE_MS_M_AWA, &unk16C, 1, &bubbleCallBack, this);
				if (emitter != nullptr) {
					emitter->setGlobalRTMatrix(unk1C0);
				}
			}
			if ((unk178.y + mParticleParams.mBubbleDepth.get())
			    < mFloorPosition.z) {
				bubbleFromBody();
			}
		}
	}
}

void TMario::runningRippleEffect()
{
	if (mForwardVel > 30.0f) {
		gpMarioParticleManager->emit(PARTICLE_MS_M_WATRUN_A, &unk190, 0,
		                             nullptr);
	}
	SMS_EmitRippleTiny(&unk190);
}

void TMario::blurEffect()
{
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_M_BLUR1, mModel->unk8->mNodeMatrices[1], 1, this);
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
		emitter->setScale(scale);
}

void TMario::frontSlipEffect()
{
	if (mGroundPlane->isWetGround()
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
		if (checkFlag(MARIO_FLAG_UNK40000)) {
			calcGroundMtx(unk178);
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    PARTICLE_MS_M_SLIDESAND_B, unk250, 1, this);
			gpMarioParticleManager->emitAndBindToPosPtr(
			    PARTICLE_MS_M_SLIDESAND_A, &unk178, 1, this);
			return;
		}
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_M_SLIPSMOKE, mModel->unk8->mNodeMatrices[0], 1, this);
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

	JGeometry::TVec3<f32> scaleVec(scale, scale, scale);
	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_GESOSURF_A, (MtxPtr)getRootAnmMtx(), 3, this);
	if (emitter != nullptr)
		emitter->setScale(scaleVec);
	emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_GESOSURF_B, unk1F0, 1, this);
	if (emitter != nullptr)
		emitter->setScale(scaleVec);
	emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_GESOSURF_D, unk1F0, 1, this);
	if (emitter != nullptr)
		emitter->setScale(scaleVec);
	emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_GESOSURF_C, unk1F0, 1, this);
	if (emitter != nullptr)
		emitter->setScale(scaleVec);
}

struct TWarpInCallBack
    : public JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*> {
	void execute(JPABaseEmitter*, JPABaseParticle*);
};

void TWarpInCallBack::execute(JPABaseEmitter* emitter,
                              JPABaseParticle* particle)
{
	// TODO: awful vector maths :(
	JGeometry::TVec3<f32>* vel = (JGeometry::TVec3<f32>*)emitter->unk120;

	f32 timer = (f32)gpMarioOriginal->mStatusTimer;
	f32 tmp   = gpMarioOriginal->unk468;

	f32 factor = ((((int)particle >> 2) & 0x3F) / 16.0f + 1.0f);

	JGeometry::TVec3<f32> v = *vel;

	v = v * tmp;
	v = v * timer;
	v = v * factor;

	particle->unk14 += v;
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
			boneIdx = unk3C4;
			break;
		case 1:
			boneIdx = mBoneIDs[10];
			break;
		case 2:
			boneIdx = mBoneIDs[10];
			break;
		case 3:
			boneIdx = mBoneIDs[4];
			break;
		case 4:
			boneIdx = mBoneIDs[5];
			break;
		case 5:
			boneIdx = mBoneIDs[6];
			break;
		case 6:
			boneIdx = mBoneIDs[7];
			break;
		case 7:
			boneIdx = mBoneIDs[8];
			break;
		case 8:
			boneIdx = mBoneIDs[9];
			break;
		case 9:
		default:
			boneIdx = unk3C4;
			break;
		}

		MtxPtr mtx = mModel->getModel()->getAnmMtx(boneIdx);

		s32 id = warpInEffectIDs[i];
		BOOL b = TRUE;
		if (id == 0x23 && !checkFlag(MARIO_FLAG_HAS_FLUDD))
			b = FALSE;

		if (b == TRUE) {
			JPABaseEmitter* emitter
			    = gpMarioParticleManager->emitAndBindToMtx(id, mtx, 0, this);
			if (emitter != nullptr) {
				emitter->unk114 = &warpInCallBack;
				emitter->unk120 = &mWarpInDir;
			}
		}
	}

	gpMarioParticleManager->emitAndBindToMtx(MAP_MAP_GATE_MS_MARIOWP_DUST,
	                                         getCenterAnmMtx(), 0, nullptr);
	gpMarioParticleManager->emitAndBindToMtx(
	    0x1D6,
	    ((TModelGate*)mHolder)
	        ->unk78->getModel()
	        ->getAnmMtx(((TModelGate*)mHolder)->unk72),
	    2, mHolder);
}

void TMario::warpInLight()
{
	gpMarioParticleManager->emitAndBindToPosPtr(MAP_MAP_GATE_MS_MARIOWP_SENKO,
	                                            &unk178, 0, this);
}

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
		    PARTICLE_MS_MARIOAP_BODY, mModel->getModel()->getAnmMtx(unk3C4), 0,
		    this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_MARIOAP_HEAD,
		    mModel->getModel()->getAnmMtx(mBoneIDs[10]), 0, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_MARIOAP_CAP,
		    mModel->getModel()->getAnmMtx(mBoneIDs[10]), 0, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_MARIOAP_RHAND,
		    mModel->getModel()->getAnmMtx(mBoneIDs[4]), 0, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_MARIOAP_LHAND,
		    mModel->getModel()->getAnmMtx(mBoneIDs[5]), 0, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_MARIOAP_RLEG,
		    mModel->getModel()->getAnmMtx(mBoneIDs[6]), 0, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_MARIOAP_RFOOT,
		    mModel->getModel()->getAnmMtx(mBoneIDs[7]), 0, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_MARIOAP_LLEG,
		    mModel->getModel()->getAnmMtx(mBoneIDs[8]), 0, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_MARIOAP_LFOOT,
		    mModel->getModel()->getAnmMtx(mBoneIDs[9]), 0, this);
		if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    PARTICLE_MS_MARIOAP_WATGUN,
			    mModel->getModel()->getAnmMtx(unk3C4), 0, this);
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
	if (unk134 > 0.0f) {
		JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
		    MAP_POLLUTION_MS_M_SPINOS, getCenterAnmMtx(), 1, this);
		if (emitter != nullptr) {
			emitter->mChildSpawnRate = 0.003921569f * (1.75f * unk134);
		}
	}
}

void TMario::emitRotateShootEffect()
{
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_M_SPINSHOT_A,
	                                            &unk178, 1, this);
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_M_SPINSHOT_B,
	                                            &unk178, 1, this);
}

void TMario::emitFootPrintWithEffect(int effectId, int printId)
{
	int foot   = 2;
	MtxPtr mtx = nullptr;
	if (mStatus == MARIO_STATUS_RUN) {
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
				mtx  = mModel->getModel()->getAnmMtx(mBoneIDs[9]);
			}
			if (getMotionFrameCtrl().checkPass(8.0f)) {
				foot = 1;
				mtx  = mModel->getModel()->getAnmMtx(mBoneIDs[7]);
			}
		}
	}

	if (mStatus == MARIO_STATUS_WAIT && onYoshi()) {
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
		unk1A8.x = mtx[0][3];
		unk1A8.y = mtx[1][3];
		unk1A8.z = mtx[2][3];

		// wtf is this bs?
		u32 b2 = printId > 0;
		u32 b  = mForwardVel > 20.0f;
		if (mStatus == MARIO_STATUS_RUN && b && b2)
			gpMarioParticleManager->emit(printId, &unk1A8, 0, nullptr);

		if (effectId > 0) {
			calcGroundMtx(unk1A8);
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
	if (unk134 > 0.0f) {
		JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
		    MAP_POLLUTION_MS_M_TOKEOS, unk220, 1, this);
		if (emitter != nullptr) {
			emitter->mChildSpawnRate
			    = 0.003921569f
			      * (unk134 * mParticleParams.mMeltInWaterMax.get());
			if (checkFlag(MARIO_FLAG_IN_SHALLOW_WATER)) {
				JGeometry::TVec3<f32> scale(0.6f, 0.6f, 0.6f);
				emitter->setScale(scale);
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
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_M_BIRI_A, &unk178,
	                                            1, this);
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_M_BIRI_C, &unk178,
	                                            1, this);
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_M_BIRI_B, &unk178,
	                                            1, this);
}

void TMario::elecEndEffect()
{
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_MOE_FIRE_OFF,
	                                            &unk178, 0, this);
}

void TMario::kickRoofEffect()
{
	if (getMotionFrameCtrl().checkPass(8.0f)) {
		MtxPtr mtx = mModel->unk8->mNodeMatrices[mBoneIDs[10]];
		unk1A8.x   = mtx[0][3];
		unk1A8.y   = mtx[1][3];
		unk1A8.z   = mtx[2][3];
		gpMarioParticleManager->emit(PARTICLE_MS_M_AMIATTACK, &unk1A8, 0,
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
	f32 dist = JGeometry::TVec3<f32>(mPosition - mToroccoPos).length();

	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_TORO_WIND, mTorocco->getModel()->getAnmMtx(0), 1, this);
	if (emitter != nullptr) {
		emitter->mChildSpawnRate = dist * mParticleParams.mToroccoWind.get();
	}
	emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_TORO_HIBANA, mTorocco->getModel()->getAnmMtx(0), 1, this);
	if (emitter != nullptr) {
		emitter->mChildSpawnRate = dist * mParticleParams.mToroccoSpark.get();
	}
}

void TMario::kickFruitEffect()
{
	JPABaseEmitter* emitter = gpMarioParticleManager->emit(
	    PARTICLE_MS_M_AMIATTACK, &mPosition, 0, nullptr);
	if (emitter != nullptr) {
		JGeometry::TVec3<f32> scale(1.2f, 1.2f, 1.2f);
		emitter->setScale(scale);
		JGeometry::TVec3<f32> pos = mPosition;
		pos.y += 30.0f;
		emitter->mTrans.set(pos);
	}
}

void TMario::sinkInSandEffect()
{
	JPABaseEmitter* emitter = gpMarioParticleManager->emit(
	    PARTICLE_MS_POI_SAND, &mPosition, 0, nullptr);
	if (emitter)
		emitter->setScale(JGeometry::TVec3<f32>(0.5f, 0.5f, 0.5f));
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

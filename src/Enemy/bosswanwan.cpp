#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <Enemy/BossWanwan.hpp>
#include <Camera/CameraShake.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <Player/MarioAccess.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <Strategic/ObjManager.hpp>
#include <Player/ModelWaterManager.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/Application.hpp>
#include <System/Particles.hpp>
#include <System/BaseParam.hpp>
#include <System/ParamInst.hpp>
#include <JSystem/JGeometry/JGUtil.hpp>

/*
Need to setup global position like:

- BW_BATH_POS: Target coordinates for the hot spring/bath (Vec / TVec3<float>).
- BW_PICKET_START: Starting position for the leash chain stake (Vec /
    TVec3<float>).
- BW_HEAD_START: Starting spawn coordinates for the chain chomp head (Vec /
    TVec3<float>).
*/
// Maybe they are just declared here, but filled on runtime

static const char* bwanwan_bastable[] = {
	"/scene/bwanwan/bas/bwanwan_bark.bas",
	"/scene/bwanwan/bas/bwanwan_shake.bas",
	"/scene/bwanwan/bas/bwanwan_wait.bas",
	"/scene/bwanwan/bas/bwanwan_wait2.bas",
};

JGeometry::TVec3<f32> BW_BATH_POS     = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
JGeometry::TVec3<f32> BW_PICKET_START = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
JGeometry::TVec3<f32> BW_HEAD_START   = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);

TBWParams::TBWParams(const char* path)
    : TSpineEnemyParams(path)
    , PARAM_INIT(mSLMarchSpeed, 6.0f)
    , PARAM_INIT(mSLShakeLengthMax, 3000.0f)
    , PARAM_INIT(mSLShakeLengthMaxHP0, 2000.0f)
    , PARAM_INIT(mSLTurnSpeed, 1.0f)
    , PARAM_INIT(mSLLeashNodeLen, 120.0f)
    , PARAM_INIT(mSLPicketHeight, 100.0f)
    , PARAM_INIT(mSLPicketRadius, 100.0f)
    , PARAM_INIT(mSLChainHitHeight, 100.0f)
    , PARAM_INIT(mSLChainHitRadius, 100.0f)
    , PARAM_INIT(mSLChainGroundRadius, 60.0f)
    , PARAM_INIT(mSLPullLimit, 1.0f)
    , PARAM_INIT(mSLAttackSpeed, 10.0f)
    , PARAM_INIT(mSLStunTimer, 4000)
    , PARAM_INIT(mSLSearchLength, 10000.0f)
    , PARAM_INIT(mSLSearchAngle, 60.0f)
    , PARAM_INIT(mSLBWHitPointMax, 255)
    , PARAM_INIT(mSLHeadGap, 150.0f)
{
	TParams::load(mPrmPath);
}

void TBossWanwan::kill() { return; }

void TBossWanwan::init(TLiveManager* tlivemanager) { }

void TBossWanwan::control() { }

void TBossWanwan::perform(u32 cue, JDrama::TGraphics* graphics) { }

void TBossWanwan::slideToCurPathNode(float speed, float deltaTime) { }

void TBossWanwan::calcRootMatrix()
{
	getModel()->setBaseScale(mScaling);

	MsMtxSetXYZRPH(getModel()->getBaseTRMtx(), mPosition.x,
	               mPosition.y + 500.0f, mPosition.z, mRotation.x, mRotation.y,
	               mRotation.z);
}

BOOL TBossWanwan::receiveMessage(THitActor* sender, u32 message)
{
	u32 actorType = sender->getActorType();
	if (actorType == 0x80000001) {
		return false;
	} else if (actorType == 0x1000001) {
		if (!this->msInvincible) {
			gpMarioParticleManager->emit(PARTICLE_MS_ENM_WATHIT,
			                             &sender->mPosition, 0, 0);
			if (this->mHitPoints == 0) {

				SMSGetMSound()->startSoundActor(0x28d1, &this->mPosition, 0,
				                                nullptr, 0, 4);
			} else if (this->mHitPoints == 1) {
				MtxPtr iVar3 = this->getModel()->getAnmMtx(1);
				gpMarioParticleManager->emitAndBindToMtxPtr(
				    0xb0, (iVar3 + 0x58) + 0x30, 0, 0);
				SMSGetMSound()->startSoundActor(0x28c5, &this->mPosition, 0,
				                                nullptr, 0, 4);

			} else {
				SMSGetMSound()->startSoundActor(0x28be, &this->mPosition, 0,
				                                nullptr, 0, 4);
			}

			if (this->mHitPoints != 0) {
				this->mHitPoints--;
			}

			this->mWaterHitCount++;
			return true;
		} else {
			return true;
		}

	} else {
		if (actorType == 0x4000005a) {
			sender->receiveMessage(this, HIT_MESSAGE_HIP_DROP);
			this->mHitPoints = 0;
			this->mWaterHitCount++;
			if (!this->unk1a0) {
				this->unk1a0 = true;
			}

			MtxPtr mtx = this->getModel()->getAnmMtx(1);
			gpMarioParticleManager->emitAndBindToMtxPtr(BWAN_JPA_MS_DOWNYUGE,
			                                            mtx, 0, nullptr);

			SMSGetMSound()->startSoundActor(0x28c5, &this->mPosition, 0,
			                                nullptr, 0, 4);
		}
	}

	return TSpineEnemy::receiveMessage(sender, message);
}

void TBossWanwan::shakeCamera(int shakeType)
{
	if (!SMS_IsMarioTouchGround4cm()) {
		return;
	}

	f32 dist = this->mDistToMarioSquared;

	dist = MsSqrtf(dist);

	TBWParams* params  = (TBWParams*)this->getSaveParam();
	f32 shakeLengthMax = params->mSLShakeLengthMax.get();

	TBWParams* params2    = (TBWParams*)this->getSaveParam();
	f32 shakeLengthMaxHP0 = params2->mSLShakeLengthMaxHP0.get();

	f32 ratio = 1.0;
	if (this->mMActor->checkCurBckFromIndex(0) == 0) {
		TBWParams* params3 = (TBWParams*)this->getSaveParam();
		ratio = (f32)this->mHitPoints / (f32)params3->mSLBWHitPointMax.get();
	}

	f32 effectiveDist
	    = shakeLengthMax * ratio + shakeLengthMaxHP0 * (1.0f - ratio);

	f32 delta = effectiveDist - dist;

	if (delta < 0.0f) {
		return;
	}

	f32 power = delta / effectiveDist;
	if (1.0f < power) {
		power = 1.0f;
	}

	gpCameraShake->startShake((EnumCamShakeMode)shakeType, power * ratio);
	SMSRumbleMgr->start(8, &this->mPosition);
}

void TBossWanwan::emitEffects()
{

	bool emit = false;

	if (mMActor->checkCurBckFromIndex(4) != 0
	    || mMActor->checkCurBckFromIndex(5) != 0) {
		if (mMActor->checkBckPass(8.0f) != 0) {
			emit = true;
		}
	} else {
		if (mMActor->checkCurBckFromIndex(2) != 0
		    && mMActor->checkBckPass(38.0f) != 0) {
			emit = true;
		}
	}

	if (emit) {
		gpMarioParticleManager->emit(BWAN_JPA_JUMP_ROCK, &mPosition, 0,
		                             nullptr);
		gpMarioParticleManager->emit(BWAN_JPA_JUMP_SMOKE, &mPosition, 0,
		                             nullptr);

		if (mHitPoints == 0) {
			SMSGetMSound()->startSoundActor(0x2975, &mPicket->mPosition, 0,
			                                nullptr, 0, 4);

			SMSGetMSound()->startSoundActor(0x2976, &mChainRoot->mPosition, 0,
			                                nullptr, 0, 4);

		} else {
			SMSGetMSound()->startSoundActor(0x2973, &mPicket->mPosition, 0,
			                                nullptr, 0, 4);
			SMSGetMSound()->startSoundActor(0x2974, &mChainRoot->mPosition, 0,
			                                nullptr, 0, 4);
		}
	}

	bool emit2 = false;

	if (mMActor->checkCurBckFromIndex(0)) {
		if (mMActor->checkBckPass(72.0f)) {
			emit2 = true;
		}
	} else if (mMActor->checkCurBckFromIndex(4)
	           || mMActor->checkCurBckFromIndex(5)) {
		if (mMActor->checkBckPass(6.0f) || mMActor->checkBckPass(12.0f)) {
			emit2 = true;
		}
	} else if (mMActor->checkCurBckFromIndex(2)) {
		if (mMActor->checkBckPass(4.0f)) {
			emit2 = true;
		}
	}

	if (emit2) {
		MtxPtr jointMtx = getModel()->getAnmMtx(1);
		gpMarioParticleManager->emitAndBindToMtxPtr(0xaf, jointMtx, 0, this);
	}

	if ((mMActor->checkCurBckFromIndex(4) || mMActor->checkCurBckFromIndex(5))
	    && mMActor->checkBckPass(10.0f)) {
		shakeCamera(0x16);
	}

	if (mMActor->checkCurBckFromIndex(0)) {
		J3DFrameCtrl* frameCtrl = mMActor->getFrameCtrl(0);

		if (frameCtrl->checkPass(60.0f) || frameCtrl->checkPass(127.0f)) {
			shakeCamera(0x16);
			gpMarioParticleManager->emit(BWAN_JPA_JUMP_ROCK, &mPosition, 0,
			                             nullptr);
			gpMarioParticleManager->emit(BWAN_JPA_JUMP_SMOKE, &mPosition, 0,
			                             nullptr);
		}

		if (frameCtrl->checkPass(202.0f)) {
			shakeCamera(0x17);
			gpMarioParticleManager->emit(BWAN_JPA_JUMP_ROCK, &mPosition, 0,
			                             nullptr);
			gpMarioParticleManager->emit(BWAN_JPA_JUMP_SMOKE, &mPosition, 0,
			                             nullptr);
		}
	}

	if (mMActor->checkCurBckFromIndex(2) && mMActor->checkBckPass(40.0f)) {
		shakeCamera(0x16);
	}

	if (mHitPoints != 0) {
		MtxPtr jointMtx = getModel()->getAnmMtx(1);
		gpMarioParticleManager->emitAndBindToMtxPtr(0x1ee, jointMtx, 3, this);
	}

	if (this->unk190 != 0 && mHitPoints != 0) {
		MtxPtr jointMtx = getModel()->getAnmMtx(1);
		gpMarioParticleManager->emitAndBindToMtxPtr(0x167, jointMtx, 1, this);
		this->unk190 = 0;
	}
}

TSpineEnemy* TBossWanwanManager::createEnemyInstance()
{
	return new TBossWanwan;
}

void TBossWanwanManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "bwanwan_body.bmd", 0, 0 },
		{ "bwanwan_chain.bmd", 0, 0 },
		{ "bwanwan_picket.bmd", 0, 0 },
	};

	createModelDataArray(entry);
}

void TBossWanwanManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TBWParams("/enemy/bosswanwan.prm");
	TEnemyManager::load(stream);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_jump_rock.jpa", 0xad);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_jump_smoke.jpa", 0xae);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_downyuge.jpa", 0xb0);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_hibana.jpa", 0xaf);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_deadyuge.jpa", 0xb1);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_yugami.jpa", 0x1ee);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_hityuge.jpa", 0x167);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_kira.jpa", 0x168);
}

void TBossWanwanMtxCalc::calc(u16 index) { }

void TBWLeash::perform(u32 cue, JDrama::TGraphics* graphics) { }

void TBWLeashNode::calcTemperature()
{
	if (mIndex == 0) {
		return;
	}

	f32 delta = mLeash->getNode(mIndex - 1)->mTemperature - mTemperature;
	f32 step;

	if (delta < 0.0f) {
		if (delta < 0.1f) {
			step = 0.02f;
		} else {
			step = 0.005f;
		}
	} else {
		if (delta > -0.1f) {
			step = -0.02f;
		} else {
			step = -0.005f;
		}
	}

	this->mTemperature += step;

	if (this->mTemperature < 0.0f) {
		this->mTemperature = 0.0f;
	}

	if (this->mTemperature > 1.0f) {
		this->mTemperature = 1.0f;
	}
}

void TBWLeashNode::calcMatrix()
{
	/* Too difficult for now
	f32 ivar4 = this->mLeash->unk4 + 2;
	if (mIndex < (this->mLeash->unk4 - 1)) {
	    f32 var = ivar4 + (mIndex + 1) * 0x2c + 0xc;
	}
	*/
}

void TBWLeashNode::perform(u32 cue, JDrama::TGraphics* graphics) { }

void TBWHit::perform(u32 cue, JDrama::TGraphics* graphics) { }

/*

    virtual void perform(u32 cue, JDrama::TGraphics* graphics);
    virtual BOOL receiveMessage(THitActor* sender, u32 message);
    void getTakingMtx();
    void moveRequest(const JGeometry::TVec3<float>&);


*/

BOOL TBWHit::receiveMessage(THitActor* sender, u32 message) { }

void TBWHit::moveRequest(const JGeometry::TVec3<float>& pos) { }

void TBWBinder::bind(TLiveActor* actor) { }

void TBWPicket::perform(u32 cue, JDrama::TGraphics* graphics) { }

BOOL TBWPicket::receiveMessage(THitActor* sender, u32 message) { }

void TBWPicket::getTakingMtx() { }

void TBWPicket::moveRequest(const JGeometry::TVec3<float>& pos) { }

DEFINE_NERVE(TNerveBWGraphWander, TLiveActor) { }
DEFINE_NERVE(TNerveBWRoll, TLiveActor) { }
DEFINE_NERVE(TNerveBWBark, TLiveActor) { }
DEFINE_NERVE(TNerveBWJump, TLiveActor) { }
DEFINE_NERVE(TNerveBWStun, TLiveActor) { }
DEFINE_NERVE(TNerveBWWakeup, TLiveActor) { }
DEFINE_NERVE(TNerveBWJumpToBath, TLiveActor) { }
DEFINE_NERVE(TNerveBWDie, TLiveActor) { }
DEFINE_NERVE(TNerveBWJumpAway, TLiveActor) { }
DEFINE_NERVE(TNerveBWShake, TLiveActor) { }
DEFINE_NERVE(TNerveBWFall, TLiveActor) { }

#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <Enemy/BossWanwan.hpp>
#include <Camera/CameraShake.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <Player/MarioAccess.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <M3DUtil/M3UJoint.hpp>
#include <Strategic/ObjManager.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Strategy.hpp>
#include <Player/ModelWaterManager.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/Application.hpp>
#include <System/Particles.hpp>
#include <System/BaseParam.hpp>
#include <System/ParamInst.hpp>
#include <JSystem/JGeometry/JGUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <GC2D/GCConsole2.hpp>

// to match __sinit__
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static const char* MtxCalcTypeName[] = {
	"MActorMtxCalcType_Basic クラシックスケールＯＮ",
	"MActorMtxCalcType_Softimage クラシックスケールＯＦＦ",
	"MActorMtxCalcType_MotionBlend モーションブレンド",
	"MActorMtxCalcType_User ユーザー定義",
};

static const char* bwanwan_bastable[]
    = { "/scene/bwanwan/bas/bwanwan_bark.bas",
        nullptr,
        "/scene/bwanwan/bas/bwanwan_shake.bas",
        nullptr,
        "/scene/bwanwan/bas/bwanwan_wait.bas",
        "/scene/bwanwan/bas/bwanwan_wait2.bas",
        nullptr };

static JGeometry::TVec3<f32> BW_BATH_POS(-1000.0f, 4.5f, -6217.2f);
static JGeometry::TVec3<f32> BW_PICKET_START(6012.84f, 0.0f, 7323.15f);
static JGeometry::TVec3<f32> BW_HEAD_START(5741.72f, -100.0f, 6311.62f);

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

TBWLeash::TBWLeash(TBossWanwan* owner, int nodeCount, const char* name)
    : JDrama::TViewObj(name)
    , mOwner(owner)
    , mRope(nullptr)
    , mNodes(nullptr)
{
}

TBossWanwan::TBossWanwan(const char* name)
    : TSpineEnemy(name)
    , mPicket(nullptr)
    , mLeash(nullptr)
    , unk16C(0)
    , unk168(0.0f)
    , mMtxCalc(nullptr)
    , unk17C(0)
    , unk180(0)
    , unk184(0)
    , unk188(0)
    , msInvincible(0)
    , unk18D(0)
    , unk190(0)
    , unk194(true)
    , unk195(0)
    , unk198(0)
    , unk19C(0)
    , unk1A0(0)
    , unk1A4(0.0f)
    , mWaterHitCount(0.0f)
    , unk1AC(0.0f)
    , mParams(nullptr)
    , unk1B4(0)
{
	mBinder = new TBWBinder();
}

void TBossWanwan::kill() { return; }

void TBossWanwan::init(TLiveManager* tlivemanager) { }

void TBossWanwan::control() { }

void TBossWanwan::perform(u32 cue, JDrama::TGraphics* graphics) { }

// fabricated
template <class T> T Wrap(T t, T l, T r)
{
	if (l >= r)
		return l;

	while (t >= r)
		t -= r - l;
	while (t < l)
		t += r - l;

	return t;
}

void TBossWanwan::slideToCurPathNode(float speed, float deltaTime)
{
	JGeometry::TVec3<f32> pos
	    = unkF4.unk0 != nullptr ? unkF4.unk0->mPosition : unkF4.unk4; // inline?

	pos.sub(mPosition);

	f32 dist = PSVECMag(&pos);

	f32 targetAngle;
	if (pos.z == 0.0f) {
		if (pos.x >= 0.0f) {
			targetAngle = 90.0f;
		} else {
			targetAngle = -90.0f;
		}
	} else if (pos.z >= 0.0f) {
		s16 angle   = matan(pos.z, pos.x);
		targetAngle = (f32)angle * (180.0f / 32768.0f);
	} else {
		s16 angle   = matan(-pos.z, pos.x);
		targetAngle = 180.0f - (f32)angle * (180.0f / 32768.0f);
	}

	targetAngle = MsWrap<f32>(targetAngle, 0.0f, 360.0f); // the "real" MsWrap

	// TODO: fix this fabricated func
	f32 rotY = Wrap(mRotation.y, targetAngle - 180.0f,
	                targetAngle + 180.0f); // the actually out-of-line one
	f32 diff = targetAngle - rotY;

	if (diff > 0.0f) {
		diff = (diff > deltaTime) ? deltaTime : diff;
	} else {
		diff = (diff > -deltaTime) ? diff : -deltaTime;
	}

	mRotation.y
	    = MsWrap<f32>(mRotation.y + diff, 0.0f, 360.0f); // the "real" MsWrap

	JGeometry::TVec3<f32> vel = mLinearVelocity;

	if (dist > 0.0f) {
		pos.scale(speed / dist);
	}

	vel.add(pos);
	mLinearVelocity = vel;
}

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
			if (!this->unk1A0) {
				this->unk1A0 = true;
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

	TBWParams* params  = this->getSaveParam();
	f32 shakeLengthMax = params->mSLShakeLengthMax.get();

	TBWParams* params2    = this->getSaveParam();
	f32 shakeLengthMaxHP0 = params2->mSLShakeLengthMaxHP0.get();

	f32 ratio;
	if (this->mMActor->checkCurBckFromIndex(0)) {
		ratio = 1.0f;
	} else {
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
	if (power > 1.0f) {
		power = 1.0f;
	}

	gpCameraShake->startShake((EnumCamShakeMode)shakeType, power * ratio);
	SMSRumbleMgr->start(8, &this->mPosition);
}

void TBossWanwan::emitEffects()
{
	int emit = false;

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
			SMSGetMSound()->startSoundActor(
			    0x2975, &mLeash->mRope->mPoints->unkC, 0, nullptr, 0, 4);
			SMSGetMSound()->startSoundActor(0x2976, &mPicket->mPosition, 0,
			                                nullptr, 0, 4);
		} else {
			SMSGetMSound()->startSoundActor(
			    0x2973, &mLeash->mRope->mPoints->unkC, 0, nullptr, 0, 4);
			SMSGetMSound()->startSoundActor(0x2974, &mPicket->mPosition, 0,
			                                nullptr, 0, 4);
		}
	}

	int emit2 = false;

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

void TBossWanwanManager::initJParticle()
{
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_jump_rock.jpa", 0xad);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_jump_smoke.jpa", 0xae);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_downyuge.jpa", 0xb0);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_hibana.jpa", 0xaf);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_deadyuge.jpa", 0xb1);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_yugami.jpa", 0x1ee);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_hityuge.jpa", 0x167);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_kira.jpa", 0x168);
}

void TBossWanwanManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TBWParams("/enemy/bosswanwan.prm");
	TEnemyManager::load(stream);
	initJParticle();
}

void TBossWanwanMtxCalc::calc(u16 index)
{
	J3DTransformInfo info;
	Mtx rotMtx;
	MtxPtr pRotMtx = rotMtx;

	if (index == 0 && this->mOwner->isAirborne()) {
		j3dSys.setCurrentMtxCalc(this);

		if (mNewAnm != nullptr) {
			mNewAnm->getTransform(index, &info);
		} else {
			J3DJoint* joint
			    = j3dSys.getModel()->getModelData()->getJointNodePointer(index);
			info = joint->getTransformInfo();
		}

		info.mTranslate.x = 0.0f;
		info.mTranslate.y = 0.0f;
		info.mTranslate.z = 0.0f;

		calcTransform(index, info);
	} else {
		M3UMtxCalcSIAnmBlendQuat::calc(index);

		if (index == 1) {

			rotMtx[2][3] = 0.0f;
			rotMtx[1][3] = 0.0f;
			rotMtx[0][3] = 0.0f;

			rotMtx[2][2] = 0.0f;
			rotMtx[1][2] = 0.0f;
			rotMtx[0][2] = 0.0f;

			rotMtx[2][1] = 0.0f;
			rotMtx[1][1] = 0.0f;
			rotMtx[0][1] = 0.0f;

			rotMtx[2][0] = 0.0f;
			rotMtx[1][0] = 0.0f;
			rotMtx[0][0] = 0.0f;

			MsMtxSetRotX(pRotMtx, mOwner->getUnk168());

			MtxPtr jointMtx = mOwner->getModel()->getAnmMtx(index);
			PSMTXConcat(jointMtx, pRotMtx, jointMtx);
			PSMTXCopy(jointMtx, J3DSys::mCurrentMtx);
		}
	}
}

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
	TRope* rope        = mLeash->mRope;
	TRopePoint* points = rope->mPoints;
	MtxPtr mtx         = mMActor->getModel()->getBaseTRMtx();

	JGeometry::TVec3<f32> pos = points[mIndex].unkC;
	JGeometry::TVec3<f32> dir;

	if (mIndex < rope->mNumPoints - 1) {
		dir = points[mIndex + 1].unkC;
		dir -= pos;

	} else {
		dir = points[mIndex - 1].unkC;
		dir -= pos;
		dir.negate();
	}

	PSVECNormalize(&dir, &dir);

	JGeometry::TVec3<f32> n(0.0f, 1.0f, 0.0f);
	JGeometry::TVec3<f32> side;

	side.cross2(n, dir);
	PSVECNormalize(&side, &side);

	n.cross2(dir, side);
	PSVECNormalize(&n, &n);

	mtx[0][2] = dir.x;
	mtx[1][2] = dir.y;
	mtx[2][2] = dir.z;

	if (mIndex & 1) {
		mtx[0][0] = side.x;
		mtx[1][0] = side.y;
		mtx[2][0] = side.z;
		mtx[0][1] = n.x;
		mtx[1][1] = n.y;
		mtx[2][1] = n.z;
	} else {
		mtx[0][0] = n.x;
		mtx[1][0] = n.y;
		mtx[2][0] = n.z;
		mtx[0][1] = side.x;
		mtx[1][1] = side.y;
		mtx[2][1] = side.z;
	}

	mtx[0][3] = pos.x;
	mtx[1][3] = pos.y + 30.0f;
	mtx[2][3] = pos.z;

	mPosition = pos;
}

void TBWLeashNode::perform(u32 cue, JDrama::TGraphics* graphics) { }

void TBWHit::perform(u32 cue, JDrama::TGraphics* graphics)
{
	// stack frame shenanigans
	if (cue & 1) {
		if (mJointIndex >= 0) {
			mOwner->getJointTransByIndex(mJointIndex, &mPosition);
		}
		for (int i = 0; i < mColCount; i++) {
			THitActor* elem = mCollisions[i];
			if (mOwner->mHitPoints != 0 && elem->mActorType == -0x7fffffff) {
				elem->receiveMessage(mOwner, HIT_MESSAGE_UNKA);
			}
		}
	}

	THitActor::perform(cue, graphics);
}

BOOL TBWHit::receiveMessage(THitActor* sender, u32 message)
{
	return mOwner->receiveMessage(sender, message);
}

void TBWBinder::bind(TLiveActor* actor) { }

void TBWPicket::perform(u32 cue, JDrama::TGraphics* graphics) { }

BOOL TBWPicket::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->mActorType == 0x80000001) {
		if (message == HIT_MESSAGE_HIP_DROP) {
			TBossWanwan* owner = mOwner;
			owner->unk17C      = 1;
			owner->unk184      = 0;
			SMSGetMSound()->startSoundActor(0x28c0, &mPosition, 0, nullptr, 0,
			                                4);
			return TRUE;
		}
		if (message == HIT_MESSAGE_TAKE) {
			TBossWanwan* owner = mOwner;

			if (owner->unk17C != 0) {
				JPABaseEmitter* emit = gpMarioParticleManager->emit(
				    BWAN_JPA_JUMP_SMOKE, &owner->mPicket->mPosition, 0, 0);

				if (emit != nullptr) {
					JGeometry::TVec3<f32> scale(0.3f, 0.5f, 0.3f);
					emit->setGlobalScale(scale);
				}
			}
			owner->unk194 = 0;
			owner->unk17C = 0;
			mHolder       = (TTakeActor*)sender;
			return TRUE;
		}
		if (message == HIT_MESSAGE_THROWN || message == HIT_MESSAGE_UNK8) {
			mHolder = nullptr;
			return TRUE;
		}
	}

	return FALSE;
}

MtxPtr TBWPicket::getTakingMtx() { return unk74; }

BOOL TBWPicket::moveRequest(const JGeometry::TVec3<float>& pos)
{
	// TODO: getLatestNerve requires 2 levels of inlining to get emitted
	// out-of-line, fabricated the getLatestNerve()
	// function in TBossWanwan to temporarely avoid this issue.
	if (mOwner->getLatestNerve() == &TNerveBWJumpToBath::theNerve()
	    || mOwner->getLatestNerve() == &TNerveBWDie::theNerve()
	    || mOwner->mHitPoints != 0) {
		return FALSE;
	}

	TBWLeash* leash = mOwner->mLeash;
	TRope* rope     = leash->mRope;

	JGeometry::TVec3<f32> delta = rope->mPoints->unkC;

	rope->constraintTail(pos);

	delta.sub(rope->mPoints->unkC);
	delta.negate();

	leash->mOwner->mPicketPullDelta = delta;

	return TRUE;
}

TBossWanwanManager::TBossWanwanManager(const char* name)
    : TEnemyManager(name)
{
}

DEFINE_NERVE(TNerveBWGraphWander, TLiveActor) { }

DEFINE_NERVE(TNerveBWRoll, TLiveActor)
{
	TBossWanwan* boss = static_cast<TBossWanwan*>(spine->getBody());

	if (spine->getTime() == 0) {
		J3DFrameCtrl* ctrl = boss->mMActor->getFrameCtrl(0);
		ctrl->setFrame(0.0f);
		ctrl->setRate(0.0f);
		boss->unk1C = 1;
	}

	if (boss->isReachedToGoal()) {
		spine->pushAfterCurrent(&TNerveBWGraphWander::theNerve());
		J3DFrameCtrl* ctrl = boss->mMActor->getFrameCtrl(0);
		ctrl->setRate(SMSGetAnmFrameRate());
		return true;
	} else {
		f32 speed = boss->getSaveParam()->mSLMarchSpeed.get();
		boss->walkToCurPathNode(speed, boss->mTurnSpeed, 0.0f);
		return false;
	}
}

DEFINE_NERVE(TNerveBWBark, TLiveActor)
{
	TBossWanwan* boss = static_cast<TBossWanwan*>(spine->getBody());

	if (spine->getTime() == 0) {

		// TODO: setFrameCtrl and getAnmPtr are getting
		// out-of-line even though inlining is necessary since they are
		// defined in a template class
		boss->mMtxCalc->joinAnm(0);
		boss->mMActor->mAnmBck->setFrameCtrl(0);
		J3DFrameCtrl* ctrl = boss->mMActor->getFrameCtrl(0);
		boss->unk178       = 10.0f / (f32)ctrl->getEnd();

		boss->setAnmSound(bwanwan_bastable[0]);
		boss->unk16C = 0;
		boss->unk168 = 0.0f;

		if (!boss->unk194) {
			if (boss->unk17C) {
				JPABaseEmitter* emit = gpMarioParticleManager->emit(
				    BWAN_JPA_JUMP_SMOKE, &boss->mPicket->mPosition, 0, 0);
				if (emit != nullptr) {
					JGeometry::TVec3<f32> scale(0.3f, 0.5f, 0.3f);
					emit->setGlobalScale(scale);
				}

				boss->unk194 = 0;
				boss->unk17C = 0;
				SMSGetMSound()->startSoundActor(
				    0x2966, &boss->mPicket->mPosition, 0, nullptr, 0, 4);
			}
		}
	}

	if (spine->getTime() == 280) {
		boss->mHitPoints = boss->getSaveParam()->mSLBWHitPointMax.get();
	}

	if (boss->mMActor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		spine->pushAfterCurrent(&TNerveBWGraphWander::theNerve());

		if (!(boss->unk198 & 2)) {
			gpMarDirector->getConsole()->startAppearBalloon(0xE001B, true);
		}
		boss->unk198 |= 2;
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveBWJump, TLiveActor) { }
DEFINE_NERVE(TNerveBWStun, TLiveActor) { }
DEFINE_NERVE(TNerveBWWakeup, TLiveActor) { }
DEFINE_NERVE(TNerveBWJumpToBath, TLiveActor) { }
DEFINE_NERVE(TNerveBWDie, TLiveActor) { }
DEFINE_NERVE(TNerveBWJumpAway, TLiveActor) { }
DEFINE_NERVE(TNerveBWShake, TLiveActor) { }
DEFINE_NERVE(TNerveBWFall, TLiveActor) { }

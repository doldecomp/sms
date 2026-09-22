#include <Enemy/CoasterKiller.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/Walker.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/Spider.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Player/MarioAccess.hpp>
#include <Camera/CameraShake.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <Map/MapData.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <MarioUtil/ScreenUtil.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <Strategic/SharedParts.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/Particles.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DNode.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <math.h>
#include <Enemy/Rocket.hpp>
#include <Enemy/EffectObj.hpp>
#include <macros.h>

#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

const char* killer_bastable[] = {
	"/scene/killer/bas/downkiller_down1.bas", nullptr, nullptr,
	"/scene/killer/bas/killer_search1.bas",   nullptr,
};

void TCoasterEnemy::init(TLiveManager* mgr)
{
	TWalkerEnemy::init(mgr);
	unk124->setGraph(gpConductor->getGraphByName("killer"));
	unk124->reset();
	TSpineEnemy::goToShortestNextGraphNode();
	mSpine->initWith(&TNerveCoasterEnemyWander::theNerve());
	onLiveFlag(LIVE_FLAG_DEAD);
	onLiveFlag(LIVE_FLAG_UNK10);
	mQuat.set(0.0f, 0.0f, 0.0f, 1.0f);
}

void TCoasterEnemy::moveObject() { TWalkerEnemy::moveObject(); }

// TODO: 99.9% and instruction-exact. The `nextPos - mPosition` receiver sits
// at 0x1c where retail has it at 0x10, i.e. the 16-versus-4 `operator-`
// parameter prefix that research batch 116 measured as `8 x (reference returns
// the caller copies out of)`. Known-open class, see the note on `operator-` in
// JGVec3.hpp; `.add()` for the two `+=`s and a direct `mLinearVelocity =`
// assignment are both inert here.
void TCoasterEnemy::bind()
{
	JGeometry::TVec3<f32> nextPos = mPosition;
	nextPos += mLinearVelocity;
	nextPos += mVelocity;
	setLinearVelocity(nextPos - mPosition);
}

void TCoasterEnemy::reset()
{
	TWalkerEnemy::reset();
	offLiveFlag(LIVE_FLAG_DEAD);
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	mPathDir = 0;
	mPathIdx = 0;
}

void TCoasterEnemy::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TWalkerEnemy::perform(cue, graphics);
	if (cue & CUE_MOVE) { } // required to move param_1 into r31
}

f32 TCoasterEnemy::getGravityY() const { return 0.0f; }

void TCoasterEnemy::makeCoasterGoalPath()
{
	setGoalPathFromGraph();
	unk128 = 0;
	unk12C = 0.0f;
}

void TCoasterEnemy::moveCoaster()
{
	JGeometry::TVec3<f32> delta = unk124->getCurrentPos();
	delta -= getPosition();
	delta.normalize();

	f32 t = getPathDir() == 0 ? getSaveParam2()->mSLCoasterSpeedInOrder.get()
	                          : getSaveParam2()->mSLCoasterSpeedReverse.get();
	mVelocity.set(delta);
	mVelocity.scale(t);

	JGeometry::TVec3<f32> forward;
	mQuat.getZDir(forward);

	JGeometry::TVec3<f32> up;
	mQuat.getYDir(up);

	JGeometry::TQuat4<f32> steer;
	steer.setRotate(forward, delta, 0.1f);
	mQuat.mul(steer);

	// Y-axis rotation
	JGeometry::TVec3<f32> right;
	right.cross(forward, JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f));
	if (right.length() > 0.0f) {
		right.normalize();

		JGeometry::TQuat4<f32> tiltQuat;
		tiltQuat.setRotate(right, M_PI / 2.0f);

		JGeometry::TVec3<f32> curUp;
		tiltQuat.rotate(forward, curUp);

		steer.setRotate(up, curUp, 0.1f);
		mQuat.mul(steer);
	}

	static_cast<JGeometry::TVec4<f32>&>(mQuat).normalize();
}

void TCoasterEnemy::calcRootMatrix()
{
	TPosition3f pos;

	pos.setQT(mQuat, mPosition);
	getModel()->setBaseScale(mScaling);
	getModel()->setBaseTRMtx(pos);
}

void TCoasterEnemy::setWalkAnm() { setNormalFlyAnm(); }

DEFINE_NERVE(TNerveCoasterEnemyWander, TLiveActor)
{
	TCoasterEnemy* self = (TCoasterEnemy*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setWalkAnm();
		self->initialGraphNode();
		self->unk124->setTo(self->getPathIdx());
		self->makeCoasterGoalPath();
	}

	if (self->isReachedToGoal()) {
		if (self->getPathDir() == 0) {
			self->mPathIdx++;
			if (self->getPathIdx() >= self->unk124->getGraph()->getNodeNum()) {
				self->mPathIdx = 0;
			}
		} else {
			self->mPathIdx--;
			if (self->mPathIdx < 0) {
				self->mPathIdx = self->unk124->getGraph()->getNodeNum() - 1;
			}
		}

		self->unk124->setTo(self->getPathIdx());
		self->makeCoasterGoalPath();
	}

	self->moveCoaster();
	return false;
}

TCoasterKiller::TCoasterKiller(const char* name)
    : TCoasterEnemy(name)
{
}

void TCoasterKiller::init(TLiveManager* mgr)
{
	TCoasterEnemy::init(mgr);
	mActorType = 0x0800001F;
	unk150     = 17;
	onLiveFlag(LIVE_FLAG_UNK400);
	offLiveFlag(LIVE_FLAG_UNK800);

	GXColorS10& bodyColor = getBodyColor(); // @hack, gets stack right
	mBodyColor.r          = 0;
	mBodyColor.g          = 0;
	mBodyColor.b          = 0;

	mNoseColor.r = 0;
	mNoseColor.g = 0;
	mNoseColor.b = 0;

	mEyesColor.r = 0;
	mEyesColor.g = 0;
	mEyesColor.b = 0;

	mBaseColor.r = 0;
	mBaseColor.g = 0;
	mBaseColor.b = 0;
}

void TCoasterKiller::reset() { TCoasterEnemy::reset(); }

// TODO: 95.5%. Scheduling only in the `mPosition.distance(SMS_GetMarioPos())`
// block -- we hoist `lfs f0, 4(r3)` one slot early and `fmuls f1, f4, f4` two
// slots early, everything else matches -- plus a frame of 0x50 against 0x58.
void TCoasterKiller::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TCoasterEnemy::perform(cue, graphics);

	if ((cue & CUE_CALC_ANIM) && !checkLiveFlag(LIVE_FLAG_DEAD)) {
		mParticlePos.setQT(mQuat, mPosition);
		gpMarioParticleManager->emitAndBindToMtxPtr(PARTICLE_MS_KIL_SMOKE,
		                                            mParticlePos, 1, this);

		if (mSpine->getCurrentNerve()
		    != &TNerveCoasterKillerExplosion::theNerve()) {
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_EN_KILLER_FLY_KUPPA, &mPosition, nullptr,
			    mPosition.distance(SMS_GetMarioPos()), 0, 0, nullptr, 0, 4);
		}
	}
}

void TCoasterKiller::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 2);
	mMActor       = mMActorKeeper->createMActor("killer_model1.bmd", 0);
	mMActorKeeper->createMActor("downkiller_model1.bmd", 3);

	s32 noseMatIdx = getActorKeeper()
	                     ->getMActor("killer_model1.bmd")
	                     ->getModel()
	                     ->getModelData()
	                     ->getMaterialName()
	                     ->getIndex("_nosemat1");
	s32 eyesMatIdx = getActorKeeper()
	                     ->getMActor("killer_model1.bmd")
	                     ->getModel()
	                     ->getModelData()
	                     ->getMaterialName()
	                     ->getIndex("_eyesmat1");
	s32 bodyMatIdx = getActorKeeper()
	                     ->getMActor("killer_model1.bmd")
	                     ->getModel()
	                     ->getModelData()
	                     ->getMaterialName()
	                     ->getIndex("_body1");

	SMS_InitPacket_OneTevColor(getMActor()->getModel(), noseMatIdx, GX_TEVREG0,
	                           &mNoseColor);
	SMS_InitPacket_OneTevColor(getMActor()->getModel(), eyesMatIdx, GX_TEVREG0,
	                           &mEyesColor);
	SMS_InitPacket_OneTevColor(getMActor()->getModel(), bodyMatIdx, GX_TEVREG0,
	                           &mBodyColor);
	SMS_InitPacket_OneTevColor(
	    getActorKeeper()->getMActor("downkiller_model1.bmd")->getModel(), 0,
	    GX_TEVREG0, &mBaseColor);
}

void TCoasterKiller::attackToMario()
{
	if (mSpine->getCurrentNerve()
	    != &TNerveCoasterKillerExplosion::theNerve()) {
		mSpine->pushNerve(&TNerveCoasterKillerExplosion::theNerve());

		if (getManager()->unk60 == 0) {
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
		}
	}
}

bool TCoasterKiller::isCollidMove(THitActor* param_1)
{
	if (mSpine->getCurrentNerve()
	    == &TNerveCoasterKillerExplosion::theNerve()) {
		return false;
	}

	if (param_1->isActorType(0x0800001F)) {
		mSpine->pushNerve(&TNerveCoasterKillerExplosion::theNerve());
	}

	if (param_1->isActorType(0x1000002B)
	    && static_cast<TRocket*>(param_1)->isAttack()) {
		mSpine->pushNerve(&TNerveCoasterKillerExplosion::theNerve());
	}

	return true;
}

void TCoasterKiller::behaveToWater(THitActor* param_1)
{
	if (mSpine->getCurrentNerve()
	    != &TNerveCoasterKillerExplosion::theNerve()) {
		mSpine->pushNerve(&TNerveCoasterKillerExplosion::theNerve());
	}
}

const char** TCoasterKiller::getBasNameTable() const { return killer_bastable; }

void TCoasterKiller::setNormalFlyAnm()
{
	mMActor = getActorKeeper()->getMActor("killer_model1.bmd");
	setBckAnm(3);
}

void TCoasterKiller::setDeadAnm()
{
	mMActor = getActorKeeper()->getMActor("downkiller_model1.bmd");
	setBckAnm(0);
	TEffectExplosion* effect
	    = (TEffectExplosion*)gpConductor->makeOneEnemyAppear(
	        mPosition, "エフェクト爆発マネージャー", 1);
	if (effect != nullptr) {
		effect->generate(mPosition, mScaling);
		effect->mScaling *= 0.6f;
	}
}

DEFINE_NERVE(TNerveCoasterKillerExplosion, TLiveActor)
{
	TCoasterKiller* self = (TCoasterKiller*)spine->getBody();

	if (spine->getTime() == 0) {
		self->unk1AC = self->getSaveParam2()->mSLBombRange.get()
		               * self->getBodyScale() / self->getAttackRadius();
		self->mRotation.x = 0.0f;
		self->setDeadAnm();
		gpCameraShake->startShake(CAM_SHAKE_MODE_KILLER, 1.0f);
	}

	if (self->unk190 < self->get1AC()) {
		self->unk190 *= 1.3f;
	} else {
		self->onHitFlag(HIT_FLAG_NO_COLLISION);
		if (self->checkCurAnmEnd(0)) {
			self->onLiveFlag(LIVE_FLAG_DEAD);
			self->onLiveFlag(LIVE_FLAG_UNK8);
			self->offLiveFlag(TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH);
			self->mHolder = nullptr;
			self->stopAnmSound();
			spine->reset();

			spine->setNext(&TNerveSmallEnemyDie::theNerve());
			spine->pushAfterCurrent(spine->getDefault());
			return true;
		}
	}

	self->expandCollision();
	return false;
}

TCoasterKillerManager::TCoasterKillerManager(const char* name)
    : TSmallEnemyManager(name)
    , unk60(0)
{
}

// The discarded `getActiveObjNum()` calls below are the same idiom as
// TBathtubKillerManager::load and TKoopaJrSubmarineManager::load/loadAfter:
// the inline opens with `if (!unk38) return getObjNum();`, so throwing the
// result away leaves exactly the ROM's `lwz`/`cmplwi` of the params pointer
// with no branch, plus the expansion pool the frame needs.

void TCoasterKillerManager::load(JSUMemoryInputStream& stream)
{
	getActiveObjNum();
	TSmallEnemyManager::load(stream);
	unk38 = new TCoasterKillerSaveLoadParams("/enemy/coasterkiller.prm");
	getActiveObjNum();
}

void TCoasterKillerManager::loadAfter()
{
	TSmallEnemyManager::loadAfter();
	getActiveObjNum();
}

void TCoasterKillerManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "killer_model1.bmd", 0x10230000, 0 },
		{ "downkiller_model1.bmd", 0x10210000, 0 },
		{ nullptr, 0, 0 },
	};

	createModelDataArray(entry);
}

TSpineEnemy* TCoasterKillerManager::createEnemyInstance()
{
	return new TCoasterKiller;
}

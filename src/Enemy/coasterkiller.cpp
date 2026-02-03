#include "Camera/CameraShake.hpp"
#include "Enemy/Enemy.hpp"
#include "Enemy/SmallEnemy.hpp"
#include "Strategic/LiveActor.hpp"
#include <Enemy/CoasterKiller.hpp>
#include <Enemy/Walker.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/Spider.hpp>
#include <Enemy/Conductor.hpp>
#include <Player/MarioAccess.hpp>
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

void TCoasterEnemy::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TWalkerEnemy::perform(param_1, param_2);
	if (param_1 & 1) { } // required to move param_1 into r31
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
	delta -= mPosition;
	delta.normalize();

	f32 t = getPathDir() == 0 ? getSaveParam2()->mSLCoasterSpeedInOrder.get()
	                          : getSaveParam2()->mSLCoasterSpeedReverse.get();
	mVelocity.set(delta);
	mVelocity.scale(t);

	JGeometry::TVec3<f32> forward;
	mQuat.getZDir(forward);

	JGeometry::TVec3<f32> axis;
	axis.cross(forward, delta);

	JGeometry::TVec3<f32> up;
	mQuat.getYDir(up);

	JGeometry::TQuat4<f32> steer;
	steer.setRotate(forward, axis, 0.1f);
	mQuat.mul(steer);

	// Y-axis rotation
	JGeometry::TVec3<f32> right;
	right.cross(forward, JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f));
	if (right.length() > 0.0f) {
		right.normalize();

		JGeometry::TQuat4<f32> tiltQuat;
		tiltQuat.setRotate(right, PI / 2.0f);

		JGeometry::TVec3<f32> curUp;
		tiltQuat.transform(forward, curUp);

		steer.setRotate(up, curUp, 0.1f);
		mQuat.mul(steer);
	}

	static_cast<JGeometry::TVec4<f32>&>(mQuat).normalize();
}

void TCoasterEnemy::calcRootMatrix()
{
	TPosition3f pos;

	pos.setQuat(mQuat);
	pos.setTrans(mPosition);
	getModel()->setBaseScale(mScaling);
	getModel()->setBaseTRMtx(pos);
}

void TCoasterEnemy::setNormalFlyAnm()
{
	// nothing
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

void TCoasterKiller::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TCoasterEnemy::perform(param_1, param_2);

	if ((param_1 * 2) && (param_1 & 1) == 0) {
		mParticlePos.setQuat(getQuat());
		mParticlePos.setTrans(mPosition);
		gpMarioParticleManager->emitAndBindToMtxPtr(0x174, mParticlePos.mMtx, 1,
		                                            this);

		if (mSpine->getCurrentNerve()
		    != &TNerveCoasterKillerExplosion::theNerve()) {
			JGeometry::TVec3<f32> dist;
			dist.sub(getPosition(), *gpMarioPos);
			f32 len = dist.length2();
			if (gpMSound->gateCheck(0x20FF)) {
				MSoundSESystem::MSoundSE::startSoundActorWithInfo(
				    0x20FF, &mPosition, nullptr, len, 0, 0, nullptr, 0, 4);
			}
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
	TSpineEnemy* effectBase = gpConductor->makeOneEnemyAppear(
	    mPosition, "エフェクト爆発マネージャー", 1);
	if (effectBase != nullptr) {
		TEffectExplosion* effect = (TEffectExplosion*)effectBase;
		effect->generate(mPosition, mScaling);
		mScaling *= 0.6f;
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
		gpCameraShake->startShake(CAM_SHAKE_MODE_UNK6, 1.0f);
	}

	if (self->unk190 < self->get1AC()) {
		self->unk190 *= 1.3f;
	} else {
		self->onHitFlag(HIT_FLAG_UNK1);
		if (self->checkCurAnmEnd(0)) {
			self->onLiveFlag(LIVE_FLAG_DEAD);
			self->onLiveFlag(LIVE_FLAG_UNK8);
			self->offLiveFlag(LIVE_FLAG_UNK10000);
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

#define ASSERT_MSG(msg, line) (void)((msg), (line))
#define ASSERT_TEST(expr)                                                      \
	(void)((expr) ? true : (ASSERT_MSG(__FILE__, __LINE__), false));

void TCoasterKillerManager::load(JSUMemoryInputStream& stream)
{
	(void)(unk38 ? unk38 : unk38); // @hack to force cmplwi
	TSmallEnemyManager::load(stream);
	unk38 = new TCoasterKillerSaveLoadParams("/enemy/coasterkiller.prm");
	unk38 = unk38 ? unk38 : unk38; // @hack to force cmplwi
}

void TCoasterKillerManager::loadAfter()
{
	TSmallEnemyManager::loadAfter();
	ASSERT_TEST(unk38);
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

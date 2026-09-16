#include <Enemy/ChuuHana.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/Graph.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/MirrorActor.hpp>
#include <Strategic/ObjModel.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <Map/MapMirror.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Player/MarioAccess.hpp>
#include <Enemy/PathNode.hpp>
#include <System/Particles.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// Slots 1, 5..11 are empty in the original table too.
static const char* tyuhana_bastable[] = {
	"/scene/tyuhana/bas/tyuhana_chance_end.bas",
	nullptr,
	"/scene/tyuhana/bas/tyuhana_chance_start.bas",
	"/scene/tyuhana/bas/tyuhana_jump.bas",
	"/scene/tyuhana/bas/tyuhana_push.bas",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"/scene/tyuhana/bas/tyuhana_walk.bas",
};

s32 TChuuHana::mCheckOnPanelTimeRoll = 20;
s32 TChuuHana::mCheckOnPanelTime     = 400;
u8 TChuuHana::mBodyJntIndex          = 1;
u8 TChuuHana::mEyeJntIndex           = 12;
u8 TChuuHana::mFootJntIndex          = 5;
u8 TChuuHana::mNewSw                 = 1;
u8 TChuuHana::mCompareHeight         = 1;
f32 TChuuHana::mSmallMirrorR         = 650.0f;
f32 TChuuHana::mMediumMirrorR        = 900.0f;
f32 TChuuHana::mLargeMirrorR         = 1100.0f;
u8 TChuuHana::mAttackVersion         = 1;
u8 TChuuHana::mDamageSw              = 1;

static TChuuHana* gpCurChuuHana;

static int ChuuHanaBodyCallback(J3DNode* node, int param);

TChuuHanaSaveLoadParams::TChuuHanaSaveLoadParams(const char* prm)
    : TWalkerEnemyParams(prm)
    , PARAM_INIT(mSLGetWaterPow, 1.0f)
    , PARAM_INIT(mSLGetGroundPow, 1.0f)
    , PARAM_INIT(mSLKeepBalanceTime, 200)
    , PARAM_INIT(mSLCheckFrame, 5)
    , PARAM_INIT(mSLReverseHeightS, 15.0f)
    , PARAM_INIT(mSLStretchHeightS, 10.0f)
    , PARAM_INIT(mSLMediumStretchHeightS, 7.0f)
    , PARAM_INIT(mSLSmallStretchHeightS, 3.0f)
    , PARAM_INIT(mSLReverseHeightM, 15.0f)
    , PARAM_INIT(mSLStretchHeightM, 10.0f)
    , PARAM_INIT(mSLMediumStretchHeightM, 7.0f)
    , PARAM_INIT(mSLSmallStretchHeightM, 3.0f)
    , PARAM_INIT(mSLReverseHeightL, 15.0f)
    , PARAM_INIT(mSLStretchHeightL, 10.0f)
    , PARAM_INIT(mSLMediumStretchHeightL, 7.0f)
    , PARAM_INIT(mSLSmallStretchHeightL, 3.0f)
    , PARAM_INIT(mSLWalkGravity, 4.0f)
    , PARAM_INIT(mSLWaterHitGravity, 0.2f)
    , PARAM_INIT(mSLJumpGravity, 0.2f)
    , PARAM_INIT(mSLJumpSp, 12.0f)
    , PARAM_INIT(mSLJumpHeight, 300.0f)
    , PARAM_INIT(mSLGetWaterPow2, 1.0f)
    , PARAM_INIT(mSLTacklePow, 100.0f)
    , PARAM_INIT(mSLDashRate, 2.0f)
    , PARAM_INIT(mSLAttackTimer, 300)
    , PARAM_INIT(mSLHitWaterTimer, 60)
{
	TParams::load(mPrmPath);
}

TChuuHanaManager::TChuuHanaManager(const char* name)
    : TSmallEnemyManager(name)
{
	unk60         = 0;
	unk68         = 0;
	unk6C         = 0;
	unk70         = 0;
	gpCurChuuHana = nullptr;
	unk64         = 0;
	unk65         = 0;
	unk66         = 0;
}

void TChuuHanaManager::load(JSUMemoryInputStream& stream)
{
	TSmallEnemyManager::load(stream);
	unk38 = new TChuuHanaSaveLoadParams("/enemy/chuuhana.prm");
}

TSpineEnemy* TChuuHanaManager::createEnemyInstance()
{
	return new TChuuHana("チュウハナ");
}

void TChuuHanaManager::initSetEnemies()
{
	static const char* graphlist[] = { "kohana0", "kohana1", "kohana2" };

	for (int i = 0; i < mCapacity; ++i) {
		TGraphWeb* graph = gpConductor->getGraphByName(graphlist[i]);
		TChuuHana* hana  = (TChuuHana*)unk18[i];

		// The first three share counters in twos: 0 alone, 1 and 2
		// together, everything after that on the third.
		if (i == 0)
			hana->unk21C = &unk64;
		else if (i < 3)
			hana->unk21C = &unk65;
		else
			hana->unk21C = &unk66;

		// Drop each one on a random node of its graph, 50 up.
		JGeometry::TVec3<f32> point;
		TMsRange<int> range(0, graph->unk8);
		graph->unk0[range.rand()].getPoint((Vec*)&point);
		hana->mPosition = point;
		hana->mPosition.y += 50.0f;
		hana->onLiveFlag(LIVE_FLAG_AIRBORNE);
		hana->unk124->setGraph(graph);
		hana->reset();
	}
}

void TChuuHanaAseParCallback::execute(JPABaseEmitter* emitter,
                                      JPABaseParticle* particle)
{
	if (mOwner->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT))
		return;

	emitter->setGlobalRTMatrix(
	    mOwner->getMActor()->getModel()->getAnmMtx(TChuuHana::mEyeJntIndex));
	JGeometry::TVec3<f32> scale(2.5f, 2.5f, 2.5f);
	emitter->setGlobalScale(scale);
}

void TChuuHanaAseParCallback::draw(JPABaseEmitter*, JPABaseParticle*) { }

// UNUSED, 0x20 in the map: it only ever runs inlined into TChuuHana's
// constructor.
TChuuHanaAseParCallback::TChuuHanaAseParCallback(TChuuHana* owner)
    : mOwner(owner)
{
}

TChuuHana::TChuuHana(const char* name)
    : TWalkerEnemy(name)
    , unk194(0.0f)
    , unk198(0.0f)
    , unk19C(0.0f)
    , unk1A0(0)
    , unk1A4(0)
    , unk1A8(0.0f)
    , unk1AC(0)
    , unk1B0(1)
    , unk1B1(0)
    , unk1B2(0)
    , unk1B8(0.0f)
    , unk210(0.0f)
    , unk214(0)
    , unk215(0)
    , unk218(nullptr)
    , unk21C(nullptr)
    , unk220(0.0f)
    , unk224(0)
    , mAseParCallback(this)
{
}

void TChuuHana::init(TLiveManager* manager)
{
	TWalkerEnemy::init(manager);
	mActorType = 0x10000016;
	unk150     = 0x11;
	offHitFlag(HIT_FLAG_UNK40000000);
	mSpine->initWith(&TNerveChuuHanaWalkOnPanel::theNerve());
	mMActor->setJointCallback(mBodyJntIndex, ChuuHanaBodyCallback);
	unk130 = 1;

	mMActor->initNormalMotionBlend();

	unk1B4 = (TChuuHanaSaveLoadParams*)getSaveParam();
	getMActor()->getModel()->calc();

	TMirrorActor* mirror = new TMirrorActor("チュウハナin鏡");
	mirror->init(getModel(), 0);
}

void TChuuHana::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TSmallEnemy::perform(cue, graphics);

	// A clipped-out chuuhana still animates if it or Mario is in a mirror,
	// because its reflection is drawn there.
	JGeometry::TVec3<f32> marioPos(SMS_GetMarioPos());
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)
	    && (gpMirrorModelManager->isInMirror(mPosition)
	        || gpMirrorModelManager->isInMirror(marioPos))) {
		if (cue & CUE_CALC_ANIM) {
			calcRootMatrix();
			mMActor->calc();
		}
		if (cue & CUE_CALC_VIEW)
			mMActor->viewCalc();
	}
}

void TChuuHana::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("default.bmd", 3);
}

void TChuuHana::reset()
{
	gpCurChuuHana = this;
	TWalkerEnemy::reset();

	unk215      = 0;
	unk1A4      = 30;
	mHeadHeight = 200.0f;
	unk1F8      = mPosition;
	unk19C      = 0.0f;
	unk198      = 0.0f;
	unk1A0      = 0;
	unk224      = 0;
	unk1A4      = mCheckOnPanelTime;

	// Head for a random node of the graph.
	TMsRange<int> range(0, unk124->unk0->unk8);
	JGeometry::TVec3<f32> point;
	unk124->unk0->unk0[range.rand()].getPoint((Vec*)&point);

	TPathNode goal(point);
	unkF4  = goal;
	unk104 = goal;
	unk114.clear();
	unk1B2 = 1;
}

void TChuuHana::bind()
{
	// Rolling, dropping and winding up to jump all ignore the map; the rest
	// binds normally, then everything below re-applies gravity and ground.
	if (mSpine->getCurrentNerve() != &TNerveChuuHanaRoll::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveChuuHanaFall2::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveChuuHanaJumpPrepare::theNerve())
		TLiveActor::bind();

	JGeometry::TVec3<f32> next(mPosition);
	next += mLinearVelocity;
	next += mVelocity;

	mVelocity.y -= getGravityY();
	if (mVelocity.y < mVelocityMinY)
		mVelocity.y = mVelocityMinY;

	mGroundHeight = gpMap->checkGround(next.x, next.y + mHeadHeight, next.z,
	                                   &mGroundPlane);
	mGroundHeight += 1.0f;

	if (next.y <= mGroundHeight + 0.05f && mGroundPlane->getActor() == nullptr
	    && mPosition.y < unk1F8.y - 200.0f) {
		offLiveFlag(LIVE_FLAG_AIRBORNE);
		next.y = mGroundHeight;
	} else {
		onLiveFlag(LIVE_FLAG_AIRBORNE);
	}

	gpMap->isTouchedOneWallAndMoveXZ(&next.x, next.y + mHeadHeight, &next.z,
	                                 mBodyRadius);

	JGeometry::TVec3<f32> moved(next);
	moved.sub(mPosition);
	mLinearVelocity = moved;
}

void TChuuHana::calcRootMatrix()
{
	gpCurChuuHana = this;

	if (mSpine->getCurrentNerve() == &TNerveChuuHanaJumpPrepare::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveChuuHanaFall2::theNerve()) {
		// Airborne: place the model by hand, unk220 above the feet.
		J3DModel* model = mMActor->getModel();
		MsMtxSetXYZRPH(model->getBaseTRMtx(), mPosition.x,
		               mPosition.y + unk220, mPosition.z, mRotation.x,
		               mRotation.y, mRotation.z);
		model->setBaseScale(*(Vec*)&mScaling);
	} else {
		TSpineEnemy::calcRootMatrix();
	}

	if (mSpine->getCurrentNerve() == &TNerveChuuHanaKeepBalance::theNerve()) {
		gpMarioParticleManager->emitParticleCallBack(0x130, &mPosition, 1,
		                                             &mAseParCallback, this);
	}

	// Footsteps on frame 2 of the walk cycle.
	if (mCurrentBckAnm == 6 && getMActor()->getFrameCtrl(0)->checkPass(2.0f)) {
		JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
		    0x54, getMActor()->getModel()->getAnmMtx(mBodyJntIndex), 0,
		    nullptr);
		if (emitter)
			emitter->setGlobalScale(mScaling);
	}
}

void TChuuHana::setBckAnm(int index)
{
	unk194 = 1.0f;
	mMActor->setMotionBlendRatioForBck(unk194);
	mMActor->setBckOldMotionBlendAnmPtr(mMActor->getBckAnmPtr());
	TSmallEnemy::setBckAnm(index);
}

void TChuuHana::attackToMario()
{
	if (mSpine->getCurrentNerve() != &TNerveChuuHanaObject::theNerve()) {
		if (mDamageSw)
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);

		if (mSpine->getCurrentNerve() == &TNerveChuuHanaAttack::theNerve()) {
			// The tackle: throw a grounded Mario away along the line
			// between us.
			if (SMS_IsMarioTouchGround4cm()) {
				SMS_SendMessageToMario(this, HIT_MESSAGE_THROWN);

				JGeometry::TVec3<f32> toMario(mPosition);
				toMario.sub(SMS_GetMarioPos());

				Mtx rot;
				MsMtxSetRotRPH(rot, 0.0f, MsGetRotFromZaxisY(toMario), 0.0f);

				JGeometry::TVec3<f32> dir(0.0f, 1.0f, -1.0f);
				MTXMultVec(rot, &dir, &dir);
				SMS_ThrowMario(dir, unk1B4->mSLTacklePow.get());
				*unk21C = 0;
			}
		} else {
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
		}
	} else {
		unk215 = 1;
	}
}

BOOL TChuuHana::receiveMessage(THitActor* sender, u32 message)
{
	// A hip drop while walking or balancing knocks it over.
	if (unk1A0 == 0) {
		if (mSpine->getCurrentNerve() == &TNerveChuuHanaWalkOnPanel::theNerve()
		    || mSpine->getCurrentNerve()
		        == &TNerveChuuHanaKeepBalance::theNerve()) {
			if (message == HIT_MESSAGE_HIP_DROP)
				unk1A0 = 1;
		}
	}

	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		if (mSprayedByWaterCooldown == 0) {
			mSprayedByWaterCooldown = 1;
			behaveToWater(sender);
		}
		unk165 = true;
		gpMarioParticleManager->emit(0xE7, &sender->mPosition, 0, nullptr);
		gpMSound->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK, &mPosition, 0, 0.0f,
		                        0, 0, 4);
		return TRUE;
	}

	return FALSE;
}

void TChuuHana::setWalkAnm()
{
	// Walking backwards plays the walk cycle from its far end.
	bool backwards = false;
	if (mCurrentBckAnm < 0)
		backwards = true;

	setBckAnm(12);

	if (backwards)
		getMActor()->getFrameCtrl(0)->setFrame(10.0f * mInstanceIndex);
}

void TChuuHana::kill()
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;

	onLiveFlag(LIVE_FLAG_HIDDEN);
	if (unk218) {
		unk218->receiveMessage(this, HIT_MESSAGE_UNK8);
		unk218 = nullptr;
	}
	TSmallEnemy::kill();
}

void TChuuHana::forceKill()
{
	// Only survive a force-kill when standing somewhere legal and inside
	// the map area.
	if (!mGroundPlane->isIllegalData() && !mGroundPlane->isDeathPlane()
	    && !mGroundPlane->isPool() && !mGroundPlane->isWaterSurface()) {
		if (gpMap->isInArea(mPosition.x, mPosition.z)) {
			if (!mGroundPlane->isIllegalData())
				return;
		}
	}
	kill();
}

f32 TChuuHana::getGravityY() const
{
	f32 gravity = TLiveActor::getGravityY();
	if (mSpine->getCurrentNerve() == &TNerveChuuHanaWalkOnPanel::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveChuuHanaKeepBalance::theNerve()
	    || mSpine->getCurrentNerve()
	        == &TNerveChuuHanaForceJumped::theNerve()) {
		gravity = unk1B4->mSLWalkGravity.get();
	} else if (mSpine->getCurrentNerve() == &TNerveChuuHanaStick::theNerve()) {
		gravity = unk1B4->mSLWaterHitGravity.get();
	} else if (mSpine->getCurrentNerve() == &TNerveChuuHanaFall2::theNerve()
	           || mSpine->getCurrentNerve()
	               == &TNerveChuuHanaJumpPrepare::theNerve()) {
		gravity = unk1B4->mSLJumpGravity.get();
	}
	return gravity;
}

bool TChuuHana::willFall(long param_1)
{
	// The mirror it is standing on gets bigger with the instance index.
	f32 radius = mSmallMirrorR;
	if (mInstanceIndex > 0)
		radius = mMediumMirrorR;
	if (mInstanceIndex > 2)
		radius = mLargeMirrorR;
	if (param_1 == mCheckOnPanelTimeRoll)
		radius += 250.0f;

	if (unk218) {
		f32 dx = mPosition.x - unk218->mPosition.x;
		f32 dy = mPosition.y - unk218->mPosition.y;
		f32 dz = mPosition.z - unk218->mPosition.z;
		if (JGeometry::TUtil<f32>::sqrt(dx * dx + dy * dy + dz * dz) > radius) {
			// Too far from the mirror's centre: wander back to a node.
			unk1A4 = mCheckOnPanelTime;

			TMsRange<int> range(0, unk124->unk0->unk8);
			JGeometry::TVec3<f32> point;
			unk124->unk0->unk0[range.rand()].getPoint((Vec*)&point);

			TPathNode goal(point);
			unkF4  = goal;
			unk104 = goal;
			unk114.clear();
			unk1B2 = 1;
			return true;
		}
	}

	unk1B2 = 0;
	return false;
}

void TChuuHana::setGoal()
{
	// Pick a point 1000 ahead, with the heading swung by up to 30 degrees
	// either side, and walk there.
	JGeometry::TVec3<f32> goal;
	goal.set(mPosition);
	TMsRange<f32> swing(-30.0f, 30.0f);

	Mtx rot;
	MsMtxSetRotRPH(rot, mRotation.x, mRotation.y + swing.rand(), mRotation.z);

	JGeometry::TVec3<f32> dir(0.0f, 0.0f, 1.0f);
	MTXMultVec(rot, &dir, &dir);
	goal.x += 1000.0f * dir.x;
	goal.z += 1000.0f * dir.z;

	TPathNode node(goal);
	unkF4  = node;
	unk104 = node;
	unk114.clear();
	unk1A4 = mCheckOnPanelTime;
	unk1B2 = 0;
}

const char** TChuuHana::getBasNameTable() const { return tyuhana_bastable; }

// TODO: the nerve bodies below are not yet reconstructed; each carries its
// map size.

// TODO: incorrect size. Map records 648 bytes.
DEFINE_NERVE(TNerveChuuHanaWalkOnPanel, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 688 bytes.
DEFINE_NERVE(TNerveChuuHanaForceJumped, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 1004 bytes.
DEFINE_NERVE(TNerveChuuHanaKeepBalance, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 680 bytes.
DEFINE_NERVE(TNerveChuuHanaStick, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 544 bytes.
DEFINE_NERVE(TNerveChuuHanaRoll, TLiveActor) { return FALSE; }

DEFINE_NERVE(TNerveChuuHanaFall, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 300 bytes.
DEFINE_NERVE(TNerveChuuHanaFall2, TLiveActor) { return FALSE; }

DEFINE_NERVE(TNerveChuuHanaObject, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 1024 bytes.
DEFINE_NERVE(TNerveChuuHanaAttack, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 420 bytes.
DEFINE_NERVE(TNerveChuuHanaJumpPrepare, TLiveActor) { return FALSE; }

// TODO: incorrect size. Map records 108 bytes.
DEFINE_NERVE(TNerveChuuHanaWait, TLiveActor) { return FALSE; }

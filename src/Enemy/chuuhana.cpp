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
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <Enemy/PathNode.hpp>
#include <System/Particles.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <System/MarDirector.hpp>
#include <GC2D/GCConsole2.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <Map/MapCollisionManager.hpp>
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
	for (int i = 0; i < 3; ++i)
		unk64[i] = 0;
}

void TChuuHanaManager::load(JSUMemoryInputStream& stream)
{
	TSmallEnemyManager::load(stream);
	unk38 = new TChuuHanaSaveLoadParams("/enemy/chuuhana.prm");
}

static inline TGCConsole2* ChuuHanaGetConsole()
{
	TGCConsole2* console = SMSGetMarDirector()->getConsole();
	return console;
}

void TChuuHanaManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	// Hint balloons: standing on a panel long enough, a tackle, repeated
	// stretches and the first flip each get one.
	if (cue & CUE_CALC_ANIM) {
		if (SMS_GetMarioGrPlane()->getActor()
		    && SMS_GetMarioGrPlane()->getActor()->getActorType()
		        == 0x400000CF) {
			if (unk60 < 900) {
				unk60++;
				if (unk60 == 900)
					ChuuHanaGetConsole()->startAppearBalloon(0x2E, true);
			}
		}

		if (unk68 >= 60 && unk68 < 80) {
			unk60 = 1000;
			unk68 = 80;
			ChuuHanaGetConsole()->startAppearBalloon(0x2F, true);
		}

		if (unk6C == 5 || unk6C == 10) {
			unk6C++;
			SMSGetMarDirector()->getConsole()->startAppearBalloon(0x31, true);
		}

		if (unk70 == 1) {
			unk70++;
			unk68 = 80;
			SMSGetMarDirector()->getConsole()->startAppearBalloon(0x30, true);
		}
	}

	TEnemyManager::perform(cue, graphics);
}

TSpineEnemy* TChuuHanaManager::createEnemyInstance()
{
	return new TChuuHana("チュウハナ");
}

static inline int ChuuHanaGraphNodeNum(TGraphWeb* web) { return web->unk8; }

static inline int ChuuHanaSafeNodeNum(const TChuuHana* p);
static inline TGraphNode* ChuuHanaSafeNode(const TChuuHana* p, int i);

static inline TGraphWeb* ChuuHanaGraphOf(const TChuuHana* p)
{
	TGraphTracer* tracer = p->unk124;
	TGraphWeb* web       = tracer->unk0;
	return web;
}

static inline TGraphNode* ChuuHanaGraphNode(TGraphWeb* web, int index)
{
	TGraphNode* node = &web->unk0[index];
	return node;
}

static inline TConductor* ChuuHanaGetConductor()
{
	TConductor* conductor = gpConductor;
	return conductor;
}

void TChuuHanaManager::initSetEnemies()
{
	// One node graph for the first, the second shared by two, the third by
	// everything after them; the table spells that out per index.
	static const char* graphlist[]
	    = { "kohana0", "kohana1", "kohana1", "kohana2", "kohana2", "kohana2" };

	for (int i = 0; i < mCapacity; ++i) {
		TGraphWeb* graph = ChuuHanaGetConductor()->getGraphByName(graphlist[i]);
		TChuuHana* hana  = (TChuuHana*)unk18[i];

		// The pollution counters pair up the same way.
		if (i == 0)
			hana->unk21C = &unk64[0];
		else if (i < 3)
			hana->unk21C = &unk64[1];
		else
			hana->unk21C = &unk64[2];

		// Drop each one on a random node of its graph, 50 up.
		TMsRange<int> range(0, ChuuHanaGraphNodeNum(graph));
		JGeometry::TVec3<f32> point;
		ChuuHanaGraphNode(graph, range.rand())->getPoint((Vec*)&point);
		hana->mPosition = point;
		hana->mPosition.y += 50.0f;
		hana->onLiveFlag(LIVE_FLAG_AIRBORNE);
		hana->getTracer()->setGraph(graph);
		hana->reset();
	}
}

// Rolls the body joint about the roll axis while the Roll nerve is active.
static inline int ChuuHanaSafeNodeNum(const TChuuHana* p)
{
	int num = ChuuHanaGraphNodeNum(ChuuHanaGraphOf(p));
	return num;
}

static inline TGraphNode* ChuuHanaSafeNode(const TChuuHana* p, int i)
{
	TGraphNode* node = ChuuHanaGraphNode(ChuuHanaGraphOf(p), i);
	return node;
}

static int ChuuHanaBodyCallback(J3DNode* node, int param)
{
	if (param == 0) {
		TChuuHana* hana = gpCurChuuHana;
		if (hana == nullptr || !hana->isRolling())
			return true;

		J3DJoint* joint = (J3DJoint*)node;
		MtxPtr anmMtx
		    = gpCurChuuHana->getModel()->getAnmMtx(joint->getJntNo());

		// Hand-built identity, the popo joint-callback idiom: the three
		// translation elements first, then the 3x3 row by row.
		Mtx ident;
		Mtx roll;
		ident[0][3] = 0.0f;
		ident[1][3] = 0.0f;
		ident[2][3] = 0.0f;
		ident[0][0] = 1.0f;
		ident[0][1] = 0.0f;
		ident[0][2] = 0.0f;
		ident[1][0] = 0.0f;
		ident[1][1] = 1.0f;
		ident[1][2] = 0.0f;
		ident[2][0] = 0.0f;
		ident[2][1] = 0.0f;
		ident[2][2] = 1.0f;

		// The roll axis is the world-space axis at 0x204 with Y dropped,
		// turned into joint space by projecting onto the joint's own rows.
		JGeometry::TVec3<f32> axis(gpCurChuuHana->unk204.x, 0.0f,
		                           gpCurChuuHana->unk204.z);
		if (axis.x == 0.0f && axis.z == 0.0f)
			axis.x = 0.001f;

		JGeometry::TVec3<f32> side;
		JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
		VECCrossProduct(&up, &axis, &side);

		f32 rollDeg = gpCurChuuHana->unk210;

		// Project the world axis onto the joint's own column vectors.
		JGeometry::TVec3<f32> zDir(anmMtx[0][2], anmMtx[1][2], anmMtx[2][2]);
		JGeometry::TVec3<f32> xDir(anmMtx[0][0], anmMtx[1][0], anmMtx[2][0]);
		JGeometry::TVec3<f32> yDir(anmMtx[0][1], anmMtx[1][1], anmMtx[2][1]);

		f32 lenZ = zDir.squared();
		f32 localZ = lenZ == 0.0f ? 0.0f : side.dot(zDir) / lenZ;
		f32 lenY = yDir.squared();
		f32 localY = lenY == 0.0f ? 0.0f : side.dot(yDir) / lenY;
		f32 lenX = xDir.squared();
		f32 localX = lenX == 0.0f ? 0.0f : side.dot(xDir) / lenX;
		JGeometry::TVec3<f32> local(localX, localY, localZ);

		MTXRotAxisRad(roll, &local, (3.1415927f / 180.0f) * rollDeg);
		MTXConcat(anmMtx, roll, anmMtx);
		MTXConcat(anmMtx, ident, anmMtx);
		MTXConcat(J3DSys::mCurrentMtx, roll, J3DSys::mCurrentMtx);
		MTXConcat(J3DSys::mCurrentMtx, ident, J3DSys::mCurrentMtx);
	}
	return true;
}

// UNUSED, 0x20 in the map: it only ever runs inlined into TChuuHana's
// constructor.
TChuuHanaAseParCallback::TChuuHanaAseParCallback(TChuuHana* owner)
    : mOwner(owner)
{
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

void TChuuHana::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("default.bmd", 3);
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
	TMsRange<int> range(0, ChuuHanaGraphNodeNum(ChuuHanaGraphOf(this)));
	JGeometry::TVec3<f32> point;
	ChuuHanaGraphNode(ChuuHanaGraphOf(this), range.rand())->getPoint((Vec*)&point);

	TPathNode goal(point);
	unkF4  = goal;
	unk104 = goal;
	unk114.clear();
	unk1B2 = 1;
}

// Binding level worth +16 of low region, landing TChuuHana::setBckAnm's
// frame at 0x30 (batch 121).
static inline MActor* ChuuhanaGetMActor(const TChuuHana* p)
{
	MActor* mActor = p->getMActor();
	return mActor;
}

void TChuuHana::setBckAnm(int index)
{
	unk194 = 1.0f;
	ChuuhanaGetMActor(this)->setMotionBlendRatioForBck(unk194);
	getMActor()->setBckOldMotionBlendAnmPtr(getMActor()->getCurBckAnmPtr());
	TSmallEnemy::setBckAnm(index);
}

void TChuuHana::behaveToWater(THitActor* param_1)
{
	unk165 = true;
	unk224 = 0;
	((TChuuHanaManager*)mManager)->unk68++;

	// Sprayed while rolling: shove it away from Mario, pop it up and stop.
	// Not isRolling(): the original compares here without materialising.
	if (mSpine->getCurrentNerve() == &TNerveChuuHanaRoll::theNerve()) {
		JGeometry::TVec3<f32> away(mPosition.x - SMS_GetMarioPos().x, 0.0f,
		                           mPosition.z - SMS_GetMarioPos().z);
		MsVECNormalize(away, away);
		away.scale(unk1B4->mSLGetWaterPow.get());
		margeVelocity(away);
		onLiveFlag(LIVE_FLAG_AIRBORNE);
		mPosition.y += 10.0f;
		return;
	}

	// Walking, attacking, waiting, or stuck in the new-switch build: get
	// launched away from Mario and stick to whatever it lands on.
	if (mSpine->getCurrentNerve() == &TNerveChuuHanaWalkOnPanel::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveChuuHanaAttack::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveChuuHanaWait::theNerve()
	    || (mNewSw
	        && mSpine->getCurrentNerve()
	            == &TNerveChuuHanaStick::theNerve())) {
		unk165 = true;
		if (mAttackVersion)
			*unk21C = 1;

		JGeometry::TVec3<f32> away(mPosition.x - SMS_GetMarioPos().x, 0.0f,
		                           mPosition.z - SMS_GetMarioPos().z);
		MsVECNormalize(away, away);
		away.scale(unk1B4->mSLGetWaterPow2.get());

		if (!isAirborne()) {
			if (mCompareHeight)
				mPosition.y += 2.0f;
			else
				mPosition.y += 1.0f;
		}
		mVelocity = away;
		onLiveFlag(LIVE_FLAG_AIRBORNE);

		if (mSpine->getCurrentNerve() != &TNerveChuuHanaStick::theNerve())
			mSpine->pushNerve(&TNerveChuuHanaStick::theNerve());

		mSprayedByWaterCooldown = 0;
	} else if (!mNewSw
	           && mSpine->getCurrentNerve()
	               == &TNerveChuuHanaKeepBalance::theNerve()) {
		// Balancing on a panel in the old build: a harder launch with a
		// bigger pop.
		JGeometry::TVec3<f32> away(mPosition.x - SMS_GetMarioPos().x, 10.0f,
		                           mPosition.z - SMS_GetMarioPos().z);
		MsVECNormalize(away, away);
		away.scale(2.0f * unk1B4->mSLGetWaterPow.get());
		mVelocity = away;
		onLiveFlag(LIVE_FLAG_AIRBORNE);
		mPosition.y += 20.0f;
	}
}

void TChuuHana::attackToMario()
{
	if (mSpine->getCurrentNerve() != &TNerveChuuHanaObject::theNerve()) {
		if (mDamageSw) {
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
		} else if (mSpine->getCurrentNerve()
		           == &TNerveChuuHanaAttack::theNerve()) {
			// The tackle: throw a grounded Mario away along the line
			// between us.
			if (SMS_IsMarioTouchGround4cm()) {
				SMS_SendMessageToMario(this, HIT_MESSAGE_THROWN);

				// operator- takes its left operand by value, which is
				// what leaves TVec3::sub out of line here.
				// TODO: that by-value copy sits at 0x5c where retail has
				// it at 0x34; the frame size and every other slot agree.
				JGeometry::TVec3<f32> toMario
				    = getPosition() - SMS_GetMarioPos();

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

void TChuuHana::moveObject()
{
	TWalkerEnemy::moveObject();

	// Track the height range over the last mSLCheckFrame frames; a big
	// enough swing means it is being stretched and must react.
	if (unk1A0 == 0) {
		unk198 = unk19C = mPosition.y;
		unk1A8 = 0.0f;
	} else {
		unk1A0++;
		if (unk198 > mPosition.y)
			unk198 = mPosition.y;
		if (unk19C < mPosition.y)
			unk19C = mPosition.y;

		if (unk1A0 > unk1B4->mSLCheckFrame.get()) {
			f32 swing = unk19C - unk198;
			if (unk1A8 < swing)
				unk1A8 = swing;

			if (mPosition.y < unk19C) {
				unk1A8 /= (f32)unk1A0;
				checkStretchType();
			} else {
				if (mSpine->getCurrentNerve()
				    == &TNerveChuuHanaKeepBalance::theNerve())
					setBckAnm(7);
				unk1A0 = 1;
				unk198 = unk19C = mPosition.y;
			}
		}
	}

	// A grounded roll rides the ground normal.
	if (mSpine->getCurrentNerve() == &TNerveChuuHanaRoll::theNerve()) {
		if (!isAirborne()) {
			const JGeometry::TVec3<f32>& normal = mGroundPlane->getNormal();
			JGeometry::TVec3<f32> push(
			    unk1B4->mSLGetGroundPow.get() * normal.x, 0.0f,
			    unk1B4->mSLGetGroundPow.get() * normal.z);
			margeVelocity(push);
			mPosition.y += 5.0f;
			onLiveFlag(LIVE_FLAG_AIRBORNE);
		}
	}

	unk194 = MsClamp(unk194 - 0.1f, 0.0f, 1.0f);
	mMActor->setMotionBlendRatioForBck(unk194);

	unk1EC = mLinearVelocity;

	// Standing on nothing, or on something that is not its mirror: die.
	if (!isAirborne()
	    && (mGroundPlane->getActor() == nullptr
	        || mGroundPlane->getActor() != unk218))
		kill();
}

// One extra inline level so retail's `bl TNerveBase()` appears inside
// theNerve()'s first-use guard at the WalkOnPanel -> Roll push.
static inline const TNerveBase<TLiveActor>& ChuuHanaRollNerve()
{
	return TNerveChuuHanaRoll::theNerve();
}

bool TChuuHana::isCollidMove(THitActor* param_1)
{
	if (param_1->isActorType(0x10000016)) {
		TChuuHana* other = (TChuuHana*)param_1;
		if (other->isRolling()) {
			// Hit by a rolling one while walking: start rolling too.
			if (mSpine->getCurrentNerve()
			    == &TNerveChuuHanaWalkOnPanel::theNerve())
				mSpine->pushNerve(&ChuuHanaRollNerve());
		} else if (unk1B2 == 0) {
			// Bumping a higher-numbered sibling: one time in four, wander.
			if (mSpine->getCurrentNerve() != &TNerveChuuHanaAttack::theNerve()
			    && other->mInstanceIndex > mInstanceIndex) {
				TMsRange<int> chance(0, 100);
				if (chance.rand() % 4 == 0)
					setSafeGoal();
			}
		}
	}

	// Retail is cmplw / bne / li 0 / b / li 1, not the branchless
	// subf/subic/subfe of `return nerve != Object`.
	// TODO: 99.8%, instruction-identical except frame 0x10 long (0x128 /
	// 0x118). The extra 0x10 sits below setSafeGoal's block and lifts every
	// TMsRange / point / TPathNode slot by the same amount. Named `other`
	// and raw mActorType are 0 / wrong bool shape. Need an isCollidMove-only
	// -0x10 that does not shrink ForceJumped.
	if (mSpine->getCurrentNerve() == &TNerveChuuHanaObject::theNerve())
		return false;
	return true;
}

// UNUSED, 0x8c in the map.
bool TChuuHana::isRolling()
{
	if (mSpine->getCurrentNerve() == &TNerveChuuHanaRoll::theNerve())
		return true;
	return false;
}

// UNUSED, 0xc4 in the map: start rolling if walking.
// TODO: 232 bytes against the map's 196 with setNext; pushNerve gives 284.
void TChuuHana::forceRoll()
{
	if (mSpine->getCurrentNerve() == &TNerveChuuHanaWalkOnPanel::theNerve())
		mSpine->setNext(&TNerveChuuHanaRoll::theNerve());
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
	if (isBckAnm(6) && getMActor()->getFrameCtrl(0)->checkPass(2.0f)) {
		JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
		    0x54, getMActor()->getModel()->getAnmMtx(mBodyJntIndex), 0,
		    nullptr);
		if (emitter)
			emitter->setGlobalScale(mScaling);
	}
}

void TChuuHana::bind()
{
	// Rolling, dropping and winding up to jump all ignore the map; the rest
	// binds normally, then everything below re-applies gravity and ground.
	if (mSpine->getCurrentNerve() != &TNerveChuuHanaRoll::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveChuuHanaFall2::theNerve()
	    && mSpine->getCurrentNerve()
	        != &TNerveChuuHanaJumpPrepare::theNerve()) {
		TLiveActor::bind();
		return;
	}

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

	// `a = b - c` is what puts TVec3::sub out of line here: operator= is one
	// inline level and the difference nested in its argument two more, which
	// is the depth-4 allowance the retail `bl` measures.
	mLinearVelocity = next - mPosition;
}

// UNUSED, 0xc4 in the map: add a push into the velocity.  Inlined into
// behaveToWater's Roll branch and moveObject's grounded-roll branch; the
// onLiveFlag(AIRBORNE) and the position bump belong to the call sites, which
// order them differently (behaveToWater flags then bumps, moveObject bumps
// then flags), so neither can be part of this body.
// TODO: 0xc8 against the map's 0xc4, one instruction over, and both callers
// stop at ~95-97% here.  Retail re-reads mVelocity into a low-region
// temporary, copies it into *two* adjacent 12-byte locals, computes
// `<copy1>.z * .z + <copy2>.x * .x` (two contracted terms, the y folded
// away), compares it `<= 0.0f` and discards the result -- the branch and the
// Newton step of TUtil<f32>::sqrt are both gone, which is what a discarded
// length() leaves.  One nesting level is 0xb0 and leaves TVec3::dot a `bl`;
// two is 0xc8 and still calls dot.  The two-term shape says the vector whose
// length is taken had a statically zero y.
void TChuuHana::margeVelocity(JGeometry::TVec3<f32>& push)
{
	JGeometry::TVec3<f32> vel(mVelocity);
	JGeometry::TVec3<f32>(JGeometry::TVec3<f32>(mVelocity)).length();
	VECAdd(&vel, &push, &vel);
	vel.y     = 0.0f;
	mVelocity = vel;
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
	if (mGroundPlane->isIllegalData()
	    || (!mGroundPlane->isDeathPlane() && !mGroundPlane->isPool()
	        && !mGroundPlane->isWaterSurface())) {
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

// UNUSED, 0x148 in the map: every 20 frames check whether the panel is
// still there, and drop when it is not. Inlined into WalkOnPanel and Attack.
bool TChuuHana::checkOnPanel()
{
	unk1A4++;
	if (unk1A4 > 20) {
		unk1A4 = 0;
		if (willFall(mCheckOnPanelTime))
			unk1A4 = -100;

		if (!isAirborne() && mGroundPlane->getActor() == nullptr
		    && 200.0f + mPosition.y < unk1F8.y) {
			mSpine->pushNerve(&TNerveChuuHanaFall2::theNerve());
			return true;
		}
	}
	return false;
}

// Binder over the mirror pointer, worth +8 of low region per site; two of
// the three component reads land willFall's frame at 0xd0.
static inline THitActor* ChuuHanaMirror(const TChuuHana* p)
{
	THitActor* mirror = p->unk218;
	return mirror;
}

bool TChuuHana::willFall(long param_1)
{
	// The mirror it is standing on gets bigger with the instance index.
	int index  = mInstanceIndex;
	f32 radius = mSmallMirrorR;
	if (index > 0)
		radius = mMediumMirrorR;
	if (index > 2)
		radius = mLargeMirrorR;
	if (param_1 == mCheckOnPanelTimeRoll)
		radius += 250.0f;

	if (unk218) {
		f32 dx = mPosition.x - ChuuHanaMirror(this)->mPosition.x;
		f32 dy = mPosition.y - ChuuHanaMirror(this)->mPosition.y;
		f32 dz = mPosition.z - unk218->mPosition.z;
		if (JGeometry::TUtil<f32>::sqrt(dx * dx + dy * dy + dz * dz) > radius) {
			// Too far from the mirror's centre: wander back to a node.
			setSafeGoal();
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
	// TODO: 99.5%.  Instruction-identical; the pool below the implicit
	// TPathNode temporary is 0xc short (the named block is otherwise the
	// four locals below, in retail's order).  0xc is not on the 8-byte
	// binder grid: a 12-byte by-value TVec3 level (a TU-local by-value
	// parameter or return around setGoalPath) lands the frame at 0xa0
	// exactly but adds the six-instruction copy, so the carrier has to be a
	// zero-instruction 12-byte pool item.  Priced at 0 here: getPosition()
	// over mPosition, getRotation() x3, a setGoalPath forwarder, a fork
	// around swing.rand(), a named f32 for the swung heading, splitting the
	// dir constructor into set(), and TVec3 goal(mPosition).
	JGeometry::TVec3<f32> goal;
	goal.set(mPosition);
	TMsRange<f32> swing(-30.0f, 30.0f);
	f32 swingAngle = swing.rand();

	JGeometry::TVec3<f32> dir(0.0f, 0.0f, 1.0f);

	Mtx rot;
	MsMtxSetRotRPH(rot, mRotation.x, mRotation.y + swingAngle, mRotation.z);
	MTXMultVec(rot, &dir, &dir);
	goal.x += 1000.0f * dir.x;
	goal.z += 1000.0f * dir.z;

	setGoalPath(goal);
	unk1A4 = mCheckOnPanelTime;
	unk1B2 = 0;
}

// UNUSED, 0x13c in the map: pick a random node of the graph and walk there.
// Inlined into ForceJumped, KeepBalance, willFall and isCollidMove.
void TChuuHana::setSafeGoal()
{
	unk1A4 = mCheckOnPanelTime;

	TMsRange<int> range(0, ChuuHanaSafeNodeNum(this));
	JGeometry::TVec3<f32> point;
	ChuuHanaSafeNode(this, range.rand())->getPoint((Vec*)&point);

	TPathNode goal(point);
	unkF4  = goal;
	unk104 = goal;
	unk114.clear();
	unk1B2 = 1;
}

// UNUSED, 0x120 in the map (this is 0x120): the roll axis follows the
// velocity, and the roll angle advances by the distance covered over the
// radius.
// The shape is constrained on three sides: the 0.2 products stay unfused only
// when they go through a vector member, the squared sum fuses only for the
// operand held in a local, and one more statement of any kind tips the
// function over -inline auto's budget so the Roll nerve calls it instead.
// TODO: the original builds the x temporary before the z one and subtracts x
// first; the ctor evaluates its arguments right to left, and the two-statement
// spelling that gets the order right is not inlined any more.
void TChuuHana::rolling()
{
	JGeometry::TVec3<f32> vel(mVelocity);
	JGeometry::TVec3<f32> d(JGeometry::TVec3<f32>(vel).x, 0.0f,
	                        JGeometry::TVec3<f32>(vel).z);
	d.x -= unk204.x;
	d.z -= unk204.z;
	d.scale(0.2f);
	unk204.x += d.x;
	unk204.z += d.z;

	f32 x  = unk204.x;
	unk1B8 = 2.0f
	    * (JGeometry::TUtil<f32>::sqrt(x * x + unk204.z * unk204.z) / mBodyRadius);
	unk210 += unk1B8;
	if (unk210 > 360.0f)
		unk210 -= 360.0f;
	if (unk210 < 0.0f)
		unk210 += 360.0f;
}

// UNUSED, 0x18 in the map.
void TChuuHana::rollStart()
{
	unk210 = 0.0f;
	unk204.set(0.0f, 0.0f, 0.0f);
}

// Binder over the params pointer; the two later reverse-height reads carry
// checkStretchType's remaining +0x10 of low region.
static inline TChuuHanaSaveLoadParams* ChuuhanaParams(const TChuuHana* p)
{
	TChuuHanaSaveLoadParams* v = p->unk1B4;
	return v;
}

// A by-value int fork over the s16 instance index, +8 of low region here.
static inline int ChuuHanaIndex(const TChuuHana* p)
{
	return p->mInstanceIndex;
}

void TChuuHana::checkStretchType()
{
	unk1A0    = 0;
	f32 swing = unk1A8;

	if (mSpine->getCurrentNerve() == &TNerveChuuHanaKeepBalance::theNerve()) {
		// Balancing: a large enough bounce flips it over.
		int size  = ChuuHanaIndex(this);
		f32 limit = unk1B4->mSLReverseHeightS.get();
		if (size > 0)
			limit = ChuuhanaParams(this)->mSLReverseHeightM.get();
		if (size > 2)
			limit = ChuuhanaParams(this)->mSLReverseHeightL.get();

		if (swing > limit) {
			unk1B1 = 1;
			unk214 = 1;
			mSpine->pushNerve(&TNerveChuuHanaFall2::theNerve());
			mSpine->pushNerve(&TNerveChuuHanaJumpPrepare::theNerve());
			return;
		}
	}

	// Otherwise it is being stretched: big, medium or small.
	((TChuuHanaManager*)mManager)->unk6C++;

	int size = ChuuHanaIndex(this);
	f32 big  = unk1B4->mSLStretchHeightS.get();
	if (size > 0)
		big = unk1B4->mSLStretchHeightM.get();
	if (size > 2)
		big = unk1B4->mSLStretchHeightL.get();

	if (swing > big) {
		unk1B1 = 0;
		unk214 = 0;
		setBckAnm(8);
		mSpine->pushNerve(&TNerveChuuHanaForceJumped::theNerve());
	} else {
		int mediumSize = mInstanceIndex;
		f32 medium     = unk1B4->mSLMediumStretchHeightS.get();
		if (mediumSize > 0)
			medium = unk1B4->mSLMediumStretchHeightM.get();
		if (mediumSize > 2)
			medium = unk1B4->mSLMediumStretchHeightL.get();

		if (swing > medium) {
			unk1B1 = 0;
			unk214 = 0;
			setBckAnm(9);
			mSpine->pushNerve(&TNerveChuuHanaForceJumped::theNerve());
		} else {
			int smallSize = mInstanceIndex;
			f32 small     = unk1B4->mSLSmallStretchHeightS.get();
			if (smallSize > 0)
				small = unk1B4->mSLSmallStretchHeightM.get();
			if (smallSize > 2)
				small = unk1B4->mSLSmallStretchHeightL.get();

			if (swing > small) {
				unk1B1 = 0;
				unk214 = 0;
				setBckAnm(10);
				mSpine->pushNerve(&TNerveChuuHanaForceJumped::theNerve());
			}
		}
	}
}

// UNUSED, 0x44 in the map. TODO: no call site survives.
void TChuuHana::entryCollision() { offHitFlag(HIT_FLAG_NO_COLLISION); }

// UNUSED, 0x4c in the map. TODO: no call site survives.
void TChuuHana::eventKill()
{
	onLiveFlag(LIVE_FLAG_DEAD);
	kill();
}

// UNUSED, 0x118 in the map. TODO: no call site survives; the name and the
// callback's use of the eye joint suggest this.
MtxPtr TChuuHana::getEffectMtx()
{
	return getMActor()->getModel()->getAnmMtx(mEyeJntIndex);
}

const char** TChuuHana::getBasNameTable() const { return tyuhana_bastable; }

static inline const TLiveActor* ChuuHanaGroundActor(TChuuHana* hana)
{
	const TLiveActor* actor = hana->mGroundPlane->getActor();
	return actor;
}

static inline TChuuHana* ChuuHanaWalkOnPanelBody(TSpineBase<TLiveActor>* spine)
{
	TLiveActor* body = spine->getBody();
	TChuuHana* hana  = (TChuuHana*)body;
	return hana;
}

DEFINE_NERVE(TNerveChuuHanaWalkOnPanel, TLiveActor)
{
	TChuuHana* hana = ChuuHanaWalkOnPanelBody(spine);

	if (spine->getTime() == 0) {
		hana->setWalkAnm();
		*hana->unk21C = 0;
	}

	// Remember the panel it first lands on; until then just walk.
	if (hana->unk218 == nullptr) {
		const TLiveActor* actor = ChuuHanaGroundActor(hana);
		if (actor) {
			hana->unk1F8 = actor->mPosition;
			hana->unk218 = (THitActor*)hana->mGroundPlane->getActor();
		}
	} else {
		hana->walkBehavior(2, 1.0f);
	}

	hana->checkOnPanel();

	if (hana->TWalkerEnemy::isReachedToGoalXZ())
		hana->setGoal();

	if (*hana->unk21C) {
		spine->pushAfterCurrent(&TNerveChuuHanaAttack::theNerve());
		return TRUE;
	}
	return FALSE;
}

DEFINE_NERVE(TNerveChuuHanaForceJumped, TLiveActor)
{
	TChuuHana* hana = (TChuuHana*)spine->getBody();

	if (spine->getTime() == 0)
		hana->setSafeGoal();

	// Late in the jump it slams whatever it is standing on.
	// Retail tests getActor() into r0, then mr r3, r0 inside the arm.
	// getActor on the named receiver is the +8; the test stays raw.
	// TODO: 100% instructions, TMsRange 4 high (0x8c / 0x88) after
	// declaring range before point. Hoisted actor pointer and dropping
	// SafeNodeNum's named local are inert / -8 frame.
	if (hana->unk214 && hana->getCurAnmFrameNo(0) > 80.0f) {
		if (hana->mGroundPlane->mActor) {
			THitActor* actor
			    = (THitActor*)hana->mGroundPlane->getActor();
			actor->receiveMessage(hana, HIT_MESSAGE_SUPER_HIP_DROP);
		}
		hana->unk214 = 0;
	}

	// Two return TRUE sharing one li r3, 1: if / else if, then one return.
	if (hana->checkCurAnmEnd(0)) {
		if (hana->unk1B1)
			spine->pushAfterCurrent(&TNerveChuuHanaRoll::theNerve());
		else {
			spine->reset();
			spine->setDefaultNext();
			spine->pushAfterCurrent(spine->getDefault());
		}
		return TRUE;
	}
	return FALSE;
}

DEFINE_NERVE(TNerveChuuHanaKeepBalance, TLiveActor)
{
	TChuuHana* hana = (TChuuHana*)spine->getBody();

	if (spine->getTime() == 0) {
		((TChuuHanaManager*)hana->mManager)->unk70++;
		hana->setBckAnm(2);
		// Step back along last frame's motion.
		hana->mPosition.x -= 10.0f * hana->unk1EC.x;
		hana->mPosition.z -= 10.0f * hana->unk1EC.z;
	} else if (hana->checkCurAnmEnd(0)) {
		if (hana->isBckAnm(2)) {
			hana->setBckAnm(1);
		} else if (hana->isBckAnm(1)) {
			if (spine->getTime() > hana->unk1B4->mSLKeepBalanceTime.get())
				hana->setBckAnm(0);
			else
				hana->setBckAnm(1);
		} else if (hana->isBckAnm(0) || hana->isBckAnm(7)) {
			// Regained its footing: start this nerve over from a safe node.
			spine->reset();
			spine->setNext(&TNerveChuuHanaKeepBalance::theNerve());
			spine->pushAfterCurrent(spine->getDefault());
			hana->setSafeGoal();
			return TRUE;
		}
	}

	if (TChuuHana::mAttackVersion)
		*hana->unk21C = 1;

	if (!TChuuHana::mNewSw && hana->mGroundPlane->getActor() == nullptr) {
		spine->pushAfterCurrent(&TNerveChuuHanaFall::theNerve());
		return TRUE;
	}
	return FALSE;
}

// Binding level worth +8 of low region, landing
// TNerveChuuHanaStick::execute's frame at 0x60 (batch 124).
static inline bool ChuuhanaIsBckAnm(const TChuuHana* p, int i)
{
	bool bckAnm = p->isBckAnm(i);
	return bckAnm;
}

DEFINE_NERVE(TNerveChuuHanaStick, TLiveActor)
{
	TChuuHana* hana = (TChuuHana*)spine->getBody();

	if (spine->getTime() == 0 || !ChuuhanaIsBckAnm(hana, 4)) {
		// A fresh stick, or one whose animation was taken over: head for
		// Mario.
		hana->setBckAnm(4);
		TPathNode goal(SMS_GetMarioPos());
		hana->unkF4  = goal;
		hana->unk104 = goal;
		hana->unk114.clear();

		if (TChuuHana::mAttackVersion)
			*hana->unk21C = 1;
	} else {
		// Stuck for long enough: settle down and wait.
		hana->unk224++;
		if (hana->unk224 > hana->unk1B4->mSLHitWaterTimer.get()) {
			if (hana->checkCurAnmEnd(0)) {
				*hana->unk21C = 0;
				spine->pushAfterCurrent(&TNerveChuuHanaWait::theNerve());
				return TRUE;
			}
		} else if (hana->checkCurAnmEnd(0)) {
			hana->setBckAnm(4);
		}
	}

	if (TChuuHana::mNewSw && hana->willFall(TChuuHana::mCheckOnPanelTimeRoll)) {
		spine->pushAfterCurrent(&TNerveChuuHanaKeepBalance::theNerve());
		return TRUE;
	}

	hana->walkBehavior(3, 0.2f);
	return FALSE;
}

DEFINE_NERVE(TNerveChuuHanaRoll, TLiveActor)
{
	TChuuHana* hana = (TChuuHana*)spine->getBody();

	if (spine->getTime() == 0)
		hana->rollStart();

	if (hana->unk1B0 && hana->willFall(TChuuHana::mCheckOnPanelTimeRoll)) {
		spine->pushAfterCurrent(&TNerveChuuHanaKeepBalance::theNerve());
		return TRUE;
	}

	if (spine->getTime() > 5000)
		return TRUE;

	hana->rolling();
	return FALSE;
}

DEFINE_NERVE(TNerveChuuHanaFall, TLiveActor) { return FALSE; }

DEFINE_NERVE(TNerveChuuHanaFall2, TLiveActor)
{
	TChuuHana* hana = (TChuuHana*)spine->getBody();

	if (spine->getTime() == 0)
		hana->setBckAnm(6);

	hana->unk220 *= 0.98f;

	if (!hana->isAirborne() || spine->getTime() > 800) {
		spine->pushAfterCurrent(&TNerveChuuHanaObject::theNerve());
		hana->kill();
		return TRUE;
	}
	return FALSE;
}

DEFINE_NERVE(TNerveChuuHanaObject, TLiveActor) { return FALSE; }

// Attack-local binder over the pollution-counter pointer, +8.
static inline u8* ChuuHanaAttackFlag(const TChuuHana* p)
{
	u8* flag = p->unk21C;
	return flag;
}

DEFINE_NERVE(TNerveChuuHanaAttack, TLiveActor)
{
	// getBody two-local + unk21C binder: instruction-identical at 0xb0
	// of 0xb8.  A third binder is +0x10 (0xc0); a gpMarioAddress fork
	// lands the frame but shifts both TPathNode temps.
	// TODO: 100% instructions, frame 8 short; need +8 that does not
	// move the TPathNode block.
	TChuuHana* hana = ChuuHanaWalkOnPanelBody(spine);

	if (spine->getTime() == 0) {
		hana->setBckAnm(12);
		hana->getMActor()->setFrameRate(2.0f * SMSGetAnmFrameRate(), 0);

		TPathNode goal((THitActor*)gpMarioAddress);
		hana->unkF4  = goal;
		hana->unk104 = goal;
		hana->unk114.clear();
	}

	// Mario stepping off this panel ends the chase.
	if ((*gpMarioGroundPlane)->getActor() != hana->unk218)
		*ChuuHanaAttackFlag(hana) = 0;

	if (spine->getTime() > hana->unk1B4->mSLAttackTimer.get()) {
		spine->pushAfterCurrent(&TNerveChuuHanaWalkOnPanel::theNerve());
		spine->pushAfterCurrent(&TNerveChuuHanaWait::theNerve());
		return TRUE;
	}

	hana->checkOnPanel();

	if (hana->TWalkerEnemy::isReachedToGoalXZ()) {
		TPathNode goal((THitActor*)gpMarioAddress);
		hana->unkF4  = goal;
		hana->unk104 = goal;
		hana->unk114.clear();
	}

	hana->walkBehavior(2, hana->unk1B4->mSLDashRate.get());
	return FALSE;
}

// Binding level over a raw member read, worth +8 of low region in
// TNerveChuuHanaJumpPrepare::execute (batch 127).
static inline TChuuHanaSaveLoadParams* ChuuhanaUnk1B4(const TChuuHana* p)
{
	TChuuHanaSaveLoadParams* v1B4 = p->unk1B4;
	return v1B4;
}

DEFINE_NERVE(TNerveChuuHanaJumpPrepare, TLiveActor)
{
	TChuuHana* hana = (TChuuHana*)spine->getBody();

	if (spine->getTime() == 0)
		hana->setBckAnm(3);

	hana->unk220 = hana->mPosition.y
	    - hana->mMActor->getModel()->getAnmMtx(TChuuHana::mFootJntIndex)[1][3];

	// Ten frames in, launch back over the panel it left.
	if (hana->getMActor()->getFrameCtrl(0)->checkPass(10.0f)) {
		JGeometry::TVec3<f32> target;
		target.x = 2.0f * hana->unk1F8.x - hana->mPosition.x;
		target.y = 2.0f * hana->unk1F8.y - hana->mPosition.y;
		target.z = 2.0f * hana->unk1F8.z - hana->mPosition.z;
		*hana->unk21C = 0;
		target.y += ChuuhanaUnk1B4(hana)->mSLJumpHeight.get();

		f32 speed = hana->unk1B4->mSLJumpSp.get();
		JGeometry::TVec3<f32> vel
		    = hana->calcVelocityToJumpToY(target, speed, hana->getGravityY());
		vel.y += 10.0f;
		hana->mPosition.y += 10.0f;
		hana->mVelocity = vel;
		hana->onLiveFlag(LIVE_FLAG_AIRBORNE);
		hana->unk130 = 0;
	}

	if (hana->checkCurAnmEnd(0))
		return TRUE;
	return FALSE;
}

DEFINE_NERVE(TNerveChuuHanaWait, TLiveActor)
{
	TChuuHana* hana = (TChuuHana*)spine->getBody();

	if (spine->getTime() == 0)
		hana->setBckAnm(11);

	if (hana->checkCurAnmEnd(0))
		return TRUE;
	return FALSE;
}


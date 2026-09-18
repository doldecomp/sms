#include <Enemy/HauntLeg.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/Spider.hpp>
#include <Enemy/Walker.hpp>
#include <Map/MapData.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/TakeActor.hpp>
#include <Strategic/Strategy.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static const char* hauntleg_bastable[] = {
	nullptr,
	nullptr,
	nullptr,
};

// Local in the map: the joint callback is a free function, so the leg whose
// matrix J3D is currently building has to be reachable without a `this`.
static THauntLeg* gpCurHauntLeg;

// The leg's own joint callback: while it is haunting, the possessed joint (and
// the matrix J3D is currently building with) get an extra spin about Z.
static int HauntLegCallback(J3DNode* node, int param)
{
	if (param == 0 && gpCurHauntLeg != nullptr) {
		if (!gpCurHauntLeg->isUseCallBack())
			return 1;
		MtxPtr jointMtx = gpCurHauntLeg->getMActor()->getModel()->getAnmMtx(
		    ((J3DJoint*)node)->getJntNo());
		Mtx spin;
		MsMtxSetRotZ(spin, gpCurHauntLeg->mSpinAngle);
		MTXConcat(jointMtx, spin, jointMtx);
		MTXConcat(J3DSys::mCurrentMtx, spin, J3DSys::mCurrentMtx);
	}
	return 1;
}

THauntLegManager::THauntLegManager(const char* name)
    : TSmallEnemyManager(name)
{
	gpCurHauntLeg = nullptr;
}

void THauntLegManager::load(JSUMemoryInputStream& stream)
{
	TSmallEnemyManager::load(stream);
	unk38 = new TWalkerEnemyParams("/enemy/hauntLeg.prm");
}

TSpineEnemy* THauntLegManager::createEnemyInstance()
{
	return new THauntLeg("ハントレッグ");
}

// Binding level worth +8 of low region, landing
// THauntLegManager::initSetEnemies's frame at 0xe0 (batch 124).
static inline MActor* HauntLegGetMActor(const THauntLeg* p)
{
	MActor* mActor = p->getMActor();
	return mActor;
}

// Every leg starts on a random node of the "main" rail, one of eight colour
// pairs cycling over the group.
void THauntLegManager::initSetEnemies()
{
	static const GXColorS10 tevColorData1[] = {
		{ 0, 0, 120, 255 },   { 120, 0, 0, 255 },
		{ 0, 120, 0, 255 },   { 120, 120, 0, 255 },
		{ 120, 0, 120, 255 }, { 100, 200, 0, 255 },
		{ 0, 100, 200, 255 }, { 200, 100, 150, 255 },
	};
	static const GXColorS10 tevColorData2[] = {
		{ 0, 0, 250, 255 },   { 250, 0, 0, 255 },
		{ 0, 250, 0, 255 },   { 250, 250, 0, 255 },
		{ 250, 0, 250, 255 }, { 150, 250, 0, 255 },
		{ 0, 150, 250, 255 }, { 250, 150, 200, 255 },
	};

	int colorIndex = 0;
	for (int i = 0; i < mObjNum; ++i) {
		TGraphWeb* graph = gpConductor->getGraphByName("main");
		THauntLeg* leg   = (THauntLeg*)getObj(i);
		JGeometry::TVec3<f32> point;
		graph->unk0[TMsRange<int>(0, graph->unk8).rand()].getPoint(point);
		leg->mPosition = point;
		leg->mPosition.y += 5.0f;
		leg->onLiveFlag(LIVE_FLAG_AIRBORNE);
		leg->reset();
		for (u16 j = 0;
		     j < HauntLegGetMActor(leg)
		             ->getModel()
		             ->getModelData()
		             ->getMaterialNum();
		     ++j)
			SMS_InitPacket_TwoTevColor(getObj(i)->getMActor()->getModel(), j,
			                           GX_TEVREG0, &tevColorData1[colorIndex],
			                           GX_TEVREG1, &tevColorData2[colorIndex]);
		++colorIndex;
		if (colorIndex >= 8)
			colorIndex = 0;
	}
}

void THauntLegManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "hauntleg.bmd", 0x10220000, 0 },
	};
	createModelDataArray(entry);
}

BOOL THauntedObject::receiveMessage(THitActor* sender, u32 message)
{
	if (message <= HIT_MESSAGE_HIP_DROP) {
		mHaunter->kill();
		return TRUE;
	}
	if (message == HIT_MESSAGE_SPRAYED_BY_WATER)
		return TRUE;
	return FALSE;
}

// TODO: UNUSED (0x18) and size-exact as written, but the body is a guess: the
// six instructions are exactly a counted loop over mColCount with nothing left
// inside it, which is also what THauntLeg::calcRootMatrix expands at its one
// call site. Whatever the loop walked (mCollisions[i], presumably) the
// optimiser removed.
void THauntedObject::checkHit()
{
	for (int i = mColCount; i != 0; --i) { }
}

// TODO: UNUSED (0x10). THauntLeg::setDeadAnm switches the box off with exactly
// this store, so the helper is almost certainly what it called; spelling the
// flag write out there is what keeps setDeadAnm exact, so this copy stays dead.
void THauntedObject::kill() { onHitFlag(HIT_FLAG_NO_COLLISION); }

THauntLeg::THauntLeg(const char* name)
    : TWalkerEnemy(name)
{
	unk194 = nullptr;
	unk198 = 0;
	unk199 = 1;
	unk19C = nullptr;
}

void THauntLeg::init(TLiveManager* manager)
{
	TWalkerEnemy::init(manager);

	mActorType = 0x10000025;
	unk150     = 0x3A;
	mHitFlags |= 0x60000000;
	((TWalker*)mBinder)->setMode(1);
	unk130 = 2;
	getMActor()->setJointCallback(1, HauntLegCallback);

	unk194 = new THauntedObject("ハントオブジェクト");

	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(unk194);

	f32 radius = 30.0f * mBodyScale;
	unk194->initHitActor(0x10000025, 2, 0x80000000, radius, radius, radius,
	                     radius);
	unk194->mHaunter = this;
}

void THauntLeg::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("hauntleg.bmd", 3);
}

void THauntLeg::reset()
{
	unk19C = nullptr;
	unk198 = 0;
	unk199 = 1;
	TWalkerEnemy::reset();
}

// The leg walks on whatever surface it is standing on rather than on the XZ
// plane: the frame's Y axis is the ground (or, while it is web-climbing, the
// wall) normal, and the spider mode tilts it forward as the climb ramps up.
//
// TODO: 85.1%, and every instruction of both branches matches. The ROM keeps
// the Y-axis vector's .y and .z in f30/f31 across both MsVECNormalize calls and
// only re-reads its .x; ours reloads all three, because MsVECNormalize takes
// non-const `Vec*` and the second argument could alias. The difference is
// register allocation driven by the frame (0x178 against our 0x128): retail has
// 60 more bytes of inline-expansion temporaries below the named locals, whose
// declaration order and stack order this code already reproduces.
void THauntLeg::calcRootMatrix()
{
	gpCurHauntLeg = this;
	if (isEaten())
		return;

	getModel()->setBaseScale(mScaling);
	MtxPtr mtx = getModel()->getBaseTRMtx();

	if (getWalker()->unk2C->unk10 > 0.0f && unk138 != nullptr) {
		JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
		JGeometry::TVec3<f32> normal = unk138->getNormal();

		JGeometry::TVec3<f32> side;
		side.cross(normal, up);
		MsVECNormalize(side, side);
		up.cross(side, normal);
		MsVECNormalize(up, up);

		mtx[0][0] = side.x;
		mtx[1][0] = side.y;
		mtx[2][0] = side.z;
		mtx[0][1] = normal.x;
		mtx[1][1] = normal.y;
		mtx[2][1] = normal.z;
		mtx[0][2] = up.x;
		mtx[1][2] = up.y;
		mtx[2][2] = up.z;
		mtx[0][3] = 0.0f;
		mtx[1][3] = 0.0f;
		mtx[2][3] = 0.0f;

		Mtx lean;
		MsMtxSetRotX(lean, 90.0f * (1.0f - getWalker()->unk2C->unk10));
		MTXConcat(mtx, lean, mtx);
	} else {
		JGeometry::TVec3<f32> forward(JMASSin(DEG2SHORTANGLE(mRotation.y)),
		                              0.0f,
		                              JMASCos(DEG2SHORTANGLE(mRotation.y)));
		JGeometry::TVec3<f32> up = mGroundPlane->getNormal();

		JGeometry::TVec3<f32> side;
		side.cross(up, forward);
		MsVECNormalize(side, side);
		forward.cross(side, up);
		MsVECNormalize(forward, forward);

		mtx[0][0] = side.x;
		mtx[1][0] = side.y;
		mtx[2][0] = side.z;
		mtx[0][1] = up.x;
		mtx[1][1] = up.y;
		mtx[2][1] = up.z;
		mtx[0][2] = forward.x;
		mtx[1][2] = forward.y;
		mtx[2][2] = forward.z;
	}

	mtx[0][3] = mPosition.x;
	mtx[1][3] = mPosition.y;
	mtx[2][3] = mPosition.z;

	// The hit box rides the leg's third joint unless the leg is clipped out,
	// in which case there is no animated matrix to read.
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		unk194->mPosition = mPosition;
	} else {
		MtxPtr jointMtx = getModel()->getAnmMtx(2);
		unk194->mPosition.x = jointMtx[0][3];
		unk194->mPosition.y = jointMtx[1][3];
		unk194->mPosition.z = jointMtx[2][3];
	}

	unk194->checkHit();
}

void THauntLeg::setGenerateAnm() { setBckAnm(0); }

void THauntLeg::setWaitAnm() { setBckAnm(2); }

void THauntLeg::setWalkAnm() { setBckAnm(1); }

void THauntLeg::setRunAnm() { setBckAnm(1); }

void THauntLeg::setDeadAnm()
{
	if (unk19C) {
		unk19C->receiveMessage(this, 6);
		mHolder     = nullptr;
		mHeldObject = nullptr;
	}
	unk194->mHitFlags |= HIT_FLAG_NO_COLLISION;
}

// Binding level over a raw member read, worth +8 of low region in
// THauntLeg::attackToMario (batch 127).
static inline f32 HauntLegDistToMarioSquared(const THauntLeg* p)
{
	f32 distToMarioSquared = p->mDistToMarioSquared;
	return distToMarioSquared;
}

void THauntLeg::attackToMario()
{
	updateSquareToMario();
	if (HauntLegDistToMarioSquared(this) < 10000.0f)
		sendAttackMsgToMario();
}

bool THauntLeg::isCollidMove(THitActor* other)
{
	if (mSpine->getCurrentNerve() != &TNerveHauntLegHaunt::theNerve()
	    && unk198 == 0 && !(mLiveFlag & LIVE_FLAG_CLIPPED_OUT)
	    && ((other->mActorType & 0xFFFF0000) == 0x20000000
	        || (other->mActorType & 0xFFFF0000) == 0x40000000)) {
		if (((TTakeActor*)other)->mHolder == nullptr || other != unk19C) {
			unk19C = other;
			mSpine->setNext(&TNerveHauntLegHaunt::theNerve());
			return false;
		}
	}
	return false;
}

const char** THauntLeg::getBasNameTable() const { return hauntleg_bastable; }

// TODO: 99.6% -- the frame is 8 bytes over (0xa0 against 0x98), i.e. one
// 8-byte inline temporary too many somewhere in the two
// getMActor()->getModel()->getBaseTRMtx() chains. Reading mMActor directly is
// worse, and a named J3DModel* or MtxPtr does not move the frame at all.
//
// Mario grabs the leg by its third joint, except while it is clipped out --
// then there is no animated matrix, so the base matrix is re-pointed at the
// leg's own position instead.
MtxPtr THauntLeg::getTakingMtx()
{
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		TPosition3f mtx;
		mtx.translation(mPosition.x, mPosition.y, mPosition.z);
		MTXCopy(mtx, getMActor()->getModel()->getBaseTRMtx());
		return getMActor()->getModel()->getBaseTRMtx();
	}
	return getMActor()->getModel()->getAnmMtx(2);
}

// TODO: UNUSED (0x8c) and reconstructed from HauntLegCallback, its only
// plausible caller: the callback inlines exactly this test, singleton guard
// included, and the size agrees.
bool THauntLeg::isUseCallBack()
{
	if (mSpine->getCurrentNerve() == &TNerveHauntLegHaunt::theNerve())
		return true;
	return false;
}

// TODO: 95.8%. The only instruction difference is the jump speed: retail binds
// the 10.0f argument to f31 before the getGravityY() call and moves it into f1
// afterwards, while we rematerialise it from .sdata2 after the call, and retail
// saves f31 where we do not. A literal is safe to repeat so MWCC never binds it
// for us; naming it in a local does not help either, because a read of an
// unmodified local is safe too. The frame is 16 bytes short, which is the f31
// slot plus one more object.
//
// Two hops: the first launches the leg at the object it has decided to possess,
// the second lands on it and asks to be picked up. Missing twice hands the leg
// back to the wander nerve.
DEFINE_NERVE(TNerveHauntLegHaunt, TLiveActor)
{
	THauntLeg* leg = (THauntLeg*)spine->getBody();

	if (spine->getTime() == 0) {
		leg->mJumpVelocity = leg->calcVelocityToJumpToY(
		    leg->unk19C->mPosition, 10.0f, leg->getGravityY());
		leg->mVelocity = leg->mJumpVelocity;
		leg->mPosition.y += 10.0f;
		leg->onLiveFlag(LIVE_FLAG_AIRBORNE);
		leg->unk199 = 1;
	} else if (!leg->isAirborne()) {
		if (leg->unk199 != 0) {
			leg->mVelocity = leg->mJumpVelocity;
			leg->mPosition.y += 10.0f;
			leg->unk199 = 0;
			JGeometry::TVec3<f32> toTarget
			    = leg->getPosition() - leg->unk19C->mPosition;
			if (toTarget.length() < 200.0f) {
				THitActor* target = leg->unk19C;
				if (((TTakeActor*)target)->mHolder == nullptr
				    && target->receiveMessage(leg, HIT_MESSAGE_TAKE)) {
					leg->mHeldObject = (TTakeActor*)leg->unk19C;
					leg->unk198      = 1;
				}
			}
		} else {
			leg->mSpinAngle = 0.0f;
			spine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
			return TRUE;
		}
	}

	if (leg->isAirborne()) {
		if (leg->unk199 != 0)
			leg->mSpinAngle
			    = MsClamp(2.0f + leg->mSpinAngle, 0.0f, 180.0f);
		else
			leg->mSpinAngle
			    = MsClamp(2.0f + leg->mSpinAngle, 0.0f, 360.0f);
	}

	return FALSE;
}

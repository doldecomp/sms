#include <Animal/fishoid.hpp>
#include <Animal/boid.hpp>
#include <Camera/Camera.hpp>
#include <Enemy/Launcher.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoaderFlags.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MoveBG/Item.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/ObjManager.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>

namespace {
const char* const cFishoidMdlNames[]
    = { "fishA.bmd", "fishB.bmd", "fishC.bmd", "fishD.bmd" };
} // namespace

TRealoidActor::TRealoidActor(MActor* actor)
    : TTakeActor("boid")
    , unk70(actor)
    , mFlags(0)
{
}

void TRealoidActor::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (mFlags & FLAG_UNK2_OR_UNK4)
		return;

	THitActor::perform(cue, graphics);

	if (!(mFlags & FLAG_CLIPPED_OUT))
		unk70->perform(cue, graphics);
}

// TODO: 99.6%, all instructions match (58 `~` markers, all frame/register
// only, closure batch 123 -- no structural residue left to chase). Batch 128
// read the slot map: retail's frame is 0xd0 against our 0xa0, every
// inline-temp slot sits exactly 0x40 higher in retail (0x94/0xa0/0xac against
// our 0x54/0x60/0x6c -- the three `addi rD, r1, N` matrix/vector temporaries)
// while the frame itself is only 0x30 bigger, so the callee-saved block
// differs too. 64 bytes of dead low region is the size of a whole extra
// `Mtx` plus 16, or of a `Mtx44` where we declare a `Mtx`; not chased.
void TRealoidActor::calcRootMatrix(TBoid* boid)
{
	if (mFlags & FLAG_UNK2_OR_UNK4)
		return;

	mPosition = boid->mPosition;

	if (mHolder != nullptr) {
		MtxPtr hm = mHolder->getTakingMtx();
		JGeometry::TVec3<f32> v;
		v.x             = hm[0][3];
		v.y             = hm[1][3];
		v.z             = hm[2][3];
		boid->mPosition = v;
		unk70->getModel()->setBaseTRMtx(mHolder->getTakingMtx());
		return;
	}

	mPosition = boid->mPosition;

	JGeometry::TVec3<f32> trans = boid->mPosition;

	MtxPtr root = unk70->getModel()->getBaseTRMtx();

	JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
	JGeometry::TVec3<f32> dir = boid->mHeading;
	JGeometry::TVec3<f32> n;

	n.cross(up, dir);
	VECNormalize(&n, &n);

	up.cross(dir, n);
	VECNormalize(&up, &up);

	dir.cross(n, up);
	VECNormalize(&dir, &dir);

	root[0][0] = -dir.x;
	root[1][0] = -dir.y;
	root[2][0] = -dir.z;
	root[0][1] = up.x;
	root[1][1] = up.y;
	root[2][1] = up.z;
	root[0][2] = n.x;
	root[1][2] = n.y;
	root[2][2] = n.z;
	root[0][3] = trans.x;
	root[1][3] = trans.y;
	root[2][3] = trans.z;
}

void TRealoidActor::checkHitActors()
{
	if (mFlags & FLAG_UNK2_OR_UNK4)
		return;

	THitActor** end;
	THitActor** it;

	it  = mCollisions;
	end = mCollisions + mColCount;
	for (; it != end; ++it) {
		switch ((*it)->getActorType()) {
		case 0x80000001:
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
			break;
		}
	}
}

MtxPtr TRealoidActor::getTakingMtx() { return unk78; }

void TRealoidActor::calcRootMatrixOnTaking() { }

TRealoid::TRealoid(const char* name)
    : TSpineEnemy(name)
{
	unk150 = 0;
	mLiveFlag |= LIVE_FLAG_UNK10 | LIVE_FLAG_UNK20 | LIVE_FLAG_UNK8;
}

void TRealoid::loadDefault(JSUMemoryInputStream& stream, const char* name,
                           int arg2)
{
	TSpineEnemy::load(stream);

	s32 count;
	stream >> count;

	mMActorKeeper = new TMActorKeeper(mManager, count + arg2);
	unk150        = new TBoidLeader(count, "コントローラ");

	unk150->setUnk38(getPosition());

	unk150->setGraph(unk124->getGraph(), getPosition());

	unk154 = new TRealoidActor*[count];

	TMActorKeeper* keeper     = mMActorKeeper;
	JGeometry::TVec3<f32> pos = mPosition;
	for (int i = 0; i < count; ++i) {
		MActor* actor   = keeper->createMActor(name, 3);
		TBoid* boid     = unk150->getBoid(i);
		boid->mPosition = pos;
		unk154[i]       = createRealoidActor(actor);
		pos.y += 10.0f;
	}
}

// Closed by re-pass 172 with header round 26's consumed-reference-binding rule:
// `const TVec3<f32>& boidPos = unk150->getBoid(i)->mPosition;` ahead of the
// copy is the 4 bytes of low region that slide `pos` from 0x48 to retail's
// 0x44 in the out-of-line copy.  The same binding is worth **8** in an inlined
// expansion, so it had to be paid for on both sides of the family, and it was:
// `TRealoid::perform`'s second loop bound spelled `unk150->getBoidNum()`
// instead of `getBoidLeader()->getBoidNum()` removes 8 there, and
// `TFishoid::perform` (which the same accounting moved) takes the same
// reference binding for +8 plus a `TRealoid::getBoid(int)` forwarder -- a
// nested fork over `getBoidLeader()`, worth the last +4.  All three are
// byte-exact and nothing else in the tree moved.
// Rejected on the way (batch 128 and here): a named `TBoid* boid` before the
// copy is +8 in both (one nonmatching function either way); a dead `BOOL` or
// `f32` before `pos` is +0 in both; dropping a camera accessor level is -8 in
// both; `getRealoid(i)` for the flag calls is +8/+16; a named
// `getBoid(getBoidNum() - 1)` result in `TFishoid::perform` is +0.
void TRealoid::clipBoids(JDrama::TGraphics* graphics)
{
	SetViewFrustumClipCheckPerspective(SMSGetCamera()->getFovy(),
	                                   SMSGetCamera()->getAspect(),
	                                   graphics->getNearPlane(), 10000.0f);

	for (int i = 0; i < unk150->getBoidNum(); ++i) {
		const JGeometry::TVec3<f32>& boidPos
		    = unk150->getBoid(i)->mPosition;
		JGeometry::TVec3<f32> pos = boidPos;
		if (ViewFrustumClipCheck(graphics, &pos, 100.0f))
			unk154[i]->offFlag(TRealoidActor::FLAG_CLIPPED_OUT);
		else
			unk154[i]->onFlag(TRealoidActor::FLAG_CLIPPED_OUT);
	}
}

void TRealoid::perform(u32 cue, JDrama::TGraphics* graphics)
{
	unk150->perform(cue, graphics);

	if (cue & CUE_CALC_ANIM) {
		clipBoids(graphics);
		for (int i = 0; i < unk150->getBoidNum(); ++i)
			unk154[i]->calcRootMatrix(getBoidLeader()->getBoid(i));
	}

	for (int i = 0; i < unk150->getBoidNum(); ++i)
		unk154[i]->perform(cue, graphics);
}

void TFish::init() { mHitFlags |= HIT_FLAG_NO_COLLISION; }

TFishoid::TFishoid(int type, const char* name)
    : TRealoid(name)
{
	mType  = type;
	unk15C = nullptr;
}

void TFishoid::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TRealoid::perform(cue, graphics);

	for (int i = 0; i < getBoidNum(); ++i) {
		TBoid* boid = getBoid(i);

		const JGeometry::TVec3<f32>& boidPos = boid->mPosition;
		JGeometry::TVec3<f32> pos            = boidPos;
		f32 y                     = pos.y;
		if (y > 0.0f)
			pos.y = 0.0f;
		boid->mPosition = pos;
	}

	if (unk15C != nullptr && (cue & CUE_MOVE)) {
		unk15C->mPosition
		    = getBoidLeader()->getBoid(getBoidNum() - 1)->mPosition;
	}
}

void TFishoid::performItem(u32, JDrama::TGraphics*) { }

void TFishoid::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);
	mSpine->initWith(&TNerveWaitForever<TLiveActor>::theNerve());
	initHitActor(0, 1, 0, 0.0f, 0.0f, 0.0f, 0.0f);
	onHitFlag(HIT_FLAG_NO_COLLISION);
}

void TFishoid::initBoids() { }

// TODO: 99.9%, all instructions match and the frame is now exact (0xd0).
// Batch 123 fixed the `TPathNode` copy shape by going through
// `setFleeTarget(THitActor*)`; closure batch 128 recovered the 128 bytes of
// low region: the eight `TBoidLeader` setters (+0x20 as a set, +0 singly),
// `getBoidNum()` at both count sites (+0x20), `getBoidLeader()` as the
// receiver of every leader access (+0x28), `getRealoid(i)`/`getPosition()`
// (+8) and one binding level over `getBoidLeader()` (+0x10, parked below).
// The single remaining difference is a slot-order swap: retail puts the
// unnamed `TPathNode` conversion temporary at the *top* of the locals
// (0xb0..0xbf) with the `stream >> eventId` buffer at 0xac under it, while we
// put the buffer on top (0xb8) and the TPathNode 12 bytes lower (0xa4). That
// is batch 59's "an unnamed argument temporary sits above the named locals"
// failing for a converting-constructor temporary bound to an inlined setter's
// `const TPathNode&`. Rejected here (all leave the 13 markers unmoved):
// `eventId` in a nested block / as `s32` / declared at the top of the body, a
// dead `u32` above it, an explicit `TPathNode(...)` temporary at the call, and
// moving the binding level to any other leader site. Declaring the TPathNode
// as a named local first is much worse (73-92%).

static inline TBoidLeader* FishoidLeader(TRealoid* realoid)
{
	TBoidLeader* leader = realoid->getBoidLeader();
	return leader;
}

void TFishoid::load(JSUMemoryInputStream& stream)
{
	loadDefault(stream, cFishoidMdlNames[mType], 0);

	u32 eventId;
	stream >> eventId;

	unk15C = TMapObjBaseManager::newAndRegisterObjByEventID(eventId, "");
	if (unk15C != nullptr) {
		if (unk15C->isActorType(0x2000000E))
			unk15C = gpItemManager->newAndRegisterCoinReal();
	}

	getBoidLeader()->setBaseSpeed(4.0f);
	getBoidLeader()->setNeighborRadius(200.0f);
	getBoidLeader()->setYawSpeed(1.0f);
	getBoidLeader()->setPitchSpeed(0.5f);
	getBoidLeader()->setMaxPitch(5.0f);
	getBoidLeader()->setAlignmentStrength(0.5f);

	FishoidLeader(this)->setFleeTarget((THitActor*)gpMarioAddress);

	getBoidLeader()->setFleeRadius(400.0f);
	getBoidLeader()->setFleeStrength(3.0f);
	getBoidLeader()->mFlags |= 2;

	for (int i = 0; i < getBoidNum(); ++i)
		getRealoid(i)->unk70->setBck("fish_swim");

	if (unk15C) {
		TRealoidActor* realoid = getRealoid(getBoidNum() - 1);
		realoid->onFlag(TRealoidActor::FLAG_UNK2);
		unk15C->makeObjAppeared();
		unk15C->mPosition = realoid->getPosition();
	}
}

void TFishoid::loadItem(JSUMemoryInputStream&) { }

TRealoidActor* TFishoid::createRealoidActor(MActor* actor)
{
	return new TFish(actor);
}

TFishoidManager::TFishoidManager(const char* name)
    : TEnemyManager(name)
{
}

void TFishoidManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "fishA.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
		      | (1 << J3DMLF_TevStageNumShift),
		  0 },
		{ "fishB.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
		      | (1 << J3DMLF_TevStageNumShift),
		  0 },
		{ "fishC.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
		      | (1 << J3DMLF_TevStageNumShift),
		  0 },
		{ "fishD.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
		      | (1 << J3DMLF_TevStageNumShift),
		  0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

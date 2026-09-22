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

static inline J3DModel* getRealoidModel(TRealoidActor* actor)
{
	MActor* mactor  = actor->getMActor();
	J3DModel* model = mactor->getModel();
	return model;
}

static inline TTakeActor* getRealoidHolder(TRealoidActor* actor)
{
	TTakeActor* holder = actor->getHolder();
	return holder;
}

void TRealoidActor::calcRootMatrix(TBoid* boid)
{
	JGeometry::TVec3<f32> trans;
	MtxPtr root;
	JGeometry::TVec3<f32> up, dir, n;

	if (mFlags & FLAG_UNK2_OR_UNK4)
		return;

	mPosition = boid->mPosition;

	if (getRealoidHolder(this) != nullptr) {
		MtxPtr hm = mHolder->getTakingMtx();
		JGeometry::TVec3<f32> v;
		v.x             = hm[0][3];
		v.y             = hm[1][3];
		v.z             = hm[2][3];
		boid->mPosition = v;
		getRealoidModel(this)->setBaseTRMtx(mHolder->getTakingMtx());
		return;
	}

	mPosition = boid->mPosition;

	trans = boid->mPosition;

	root = getRealoidModel(this)->getBaseTRMtx();

	up.set(0.0f, 1.0f, 0.0f);
	dir = boid->mHeading;

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

static inline TBoidLeader* FishoidLeader(TRealoid* realoid)
{
	TBoidLeader* leader = realoid->getBoidLeader();
	return leader;
}

// The appearing item's setup is a helper of its own: retail keeps the last
// realoid out of load's named block, as a named local of an inlined body.
static inline void FishoidAppearItem(TFishoid* fishoid)
{
	TRealoidActor* realoid = fishoid->getRealoid(fishoid->getBoidNum() - 1);
	realoid->onFlag(TRealoidActor::FLAG_UNK2);
	fishoid->unk15C->makeObjAppeared();
	fishoid->unk15C->mPosition = realoid->mPosition;
}

void TFishoid::load(JSUMemoryInputStream& stream)
{
	loadDefault(stream, cFishoidMdlNames[mType], 0);
	loadItem(stream);

	getBoidLeader()->setBaseSpeed(4.0f);
	getBoidLeader()->setNeighborRadius(200.0f);
	getBoidLeader()->setYawSpeed(1.0f);
	getBoidLeader()->setPitchSpeed(0.5f);
	getBoidLeader()->setMaxPitch(5.0f);
	getBoidLeader()->setAlignmentStrength(0.5f);

	TPathNode node((THitActor*)gpMarioAddress);
	FishoidLeader(this)->setFleeTarget(node);

	getBoidLeader()->setFleeRadius(400.0f);
	getBoidLeader()->setFleeStrength(3.0f);
	getBoidLeader()->onFlag(TBoidLeader::FLAG_UNK2);

	for (int i = 0; i < getBoidNum(); ++i)
		getRealoid(i)->unk70->setBck("fish_swim");

	if (unk15C)
		FishoidAppearItem(this);
}

void TFishoid::loadItem(JSUMemoryInputStream& stream)
{
	u32 eventId;
	stream >> eventId;

	unk15C = TMapObjBaseManager::newAndRegisterObjByEventID(eventId, "");
	if (unk15C != nullptr) {
		if (unk15C->isActorType(0x2000000E))
			unk15C = gpItemManager->newAndRegisterCoinReal();
	}
}

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

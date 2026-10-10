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

// TODO: translation stack slot still differs by four bytes.
void TRealoidActor::calcRootMatrix(TBoid* boid)
{
	if (mFlags & FLAG_UNK2_OR_UNK4)
		return;

	JGeometry::TVec3<f32> trans;
	JGeometry::TVec3<f32> up;
	JGeometry::TVec3<f32> dir;
	JGeometry::TVec3<f32> n;

	mPosition = boid->mPosition;

	if (mHolder != nullptr) {
		MtxPtr hm = mHolder->getTakingMtx();
		JGeometry::TVec3<f32> v;
		v.x             = hm[0][3];
		v.y             = hm[1][3];
		v.z             = hm[2][3];
		boid->mPosition = v;
		calcRootMatrixOnTaking();
		return;
	}

	mPosition = boid->mPosition;

	trans = boid->mPosition;

	TPosition3f& root = (TPosition3f&)*unk70->getModel()->getBaseTRMtx();

	up.set(0.0f, 1.0f, 0.0f);
	dir = boid->mHeading;

	n.cross(up, dir);
	VECNormalize(&n, &n);

	up.cross(dir, n);
	VECNormalize(&up, &up);

	dir.cross(n, up);
	VECNormalize(&dir, &dir);

	root.ref(0, 0) = -dir.x;
	root.ref(1, 0) = -dir.y;
	root.ref(2, 0) = -dir.z;
	root.setYDir(up);
	root.setZDir(n);
	root.setTrans(trans);
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
		case ACTOR_TYPE_MARIO:
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
			break;
		}
	}
}

MtxPtr TRealoidActor::getTakingMtx() { return unk78; }

void TRealoidActor::calcRootMatrixOnTaking()
{
	unk70->getModel()->setBaseTRMtx(mHolder->getTakingMtx());
}

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

	unk150->setUnk38(mPosition);

	unk150->setGraph(getTracer()->getGraph(), mPosition);

	unk154 = new TRealoidActor*[count];

	TMActorKeeper* keeper     = getActorKeeper();
	JGeometry::TVec3<f32> pos = mPosition;
	for (int i = 0; i < count; ++i) {
		MActor* actor   = keeper->createMActor(name, 3);
		TBoid* boid     = unk150->getBoid(i);
		boid->mPosition = pos;
		unk154[i]       = createRealoidActor(actor);
		pos.y += 10.0f;
	}
}

void TRealoid::clipBoids(JDrama::TGraphics* graphics)
{
	SetViewFrustumClipCheckPerspective(gpCamera->getFovy(),
	                                   gpCamera->getAspect(),
	                                   graphics->getNearPlane(), 10000.0f);

	for (int i = 0; i < unk150->getBoidNum(); ++i) {
		TBoid* boid               = unk150->getBoid(i);
		JGeometry::TVec3<f32> pos = boid->mPosition;
		if (ViewFrustumClipCheck(graphics, &pos, 100.0f))
			getRealoid(i)->offFlag(TRealoidActor::FLAG_CLIPPED_OUT);
		else
			getRealoid(i)->onFlag(TRealoidActor::FLAG_CLIPPED_OUT);
	}
}

void TRealoid::perform(u32 cue, JDrama::TGraphics* graphics)
{
	unk150->perform(cue, graphics);

	if (cue & CUE_CALC_ANIM) {
		clipBoids(graphics);
		for (int i = 0; i < unk150->getBoidNum(); ++i)
			unk154[i]->calcRootMatrix(unk150->getBoid(i));
	}

	for (int i = 0; i < unk150->mNumBoids; ++i)
		unk154[i]->perform(cue, graphics);
}

void TFish::init() { mHitFilter |= HIT_FILTER_NO_COLLISION; }

TFishoid::TFishoid(int type, const char* name)
    : TRealoid(name)
{
	mType  = type;
	unk15C = nullptr;
}

// TODO: nonmatching stack frame (0x88 bytes; target 0xb8).
void TFishoid::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TRealoid::perform(cue, graphics);

	for (int i = 0; i < unk150->getBoidNum(); ++i) {
		TBoid* boid = unk150->getBoid(i);

		JGeometry::TVec3<f32> pos = boid->mPosition;
		if (pos.y > 0.0f)
			pos.y = 0.0f;
		boid->mPosition = pos;
	}

	performItem(cue, graphics);
}

void TFishoid::performItem(u32 cue, JDrama::TGraphics*)
{
	if (unk15C != nullptr && (cue & CUE_MOVE)) {
		TBoidLeader* leader = unk150;
		unk15C->mPosition
		    = leader->getBoid(leader->getBoidNum() - 1)->mPosition;
	}
}

void TFishoid::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);
	mSpine->initWith(&TNerveWaitForever<TLiveActor>::theNerve());
	initHitActor(0, 1, 0, 0.0f, 0.0f, 0.0f, 0.0f);
	onHitFilter(HIT_FILTER_NO_COLLISION);
}

void TFishoid::initBoids()
{
	if (unk15C) {
		TRealoidActor* realoid = getRealoid(unk150->mNumBoids - 1);
		realoid->onFlag(TRealoidActor::FLAG_UNK2);
		unk15C->makeObjAppeared();
		unk15C->mPosition = realoid->mPosition;
	}
}

void TFishoid::load(JSUMemoryInputStream& stream)
{
	loadDefault(stream, cFishoidMdlNames[mType], 0);

	loadItem(stream);

	unk150->mBaseSpeed         = 4.0f;
	unk150->mNeighborRadius    = 200.0f;
	unk150->mYawSpeed          = 1.0f;
	unk150->mPitchSpeed        = 0.5f;
	unk150->mMaxPitch          = 5.0f;
	unk150->mAlignmentStrength = 0.5f;

	unk150->setFleeTarget((THitActor*)gpMarioAddress);

	unk150->mFleeRadius   = 400.0f;
	unk150->mFleeStrength = 3.0f;
	unk150->mFlags |= 2;

	for (int i = 0; i < unk150->mNumBoids; ++i)
		unk154[i]->unk70->setBck("fish_swim");

	initBoids();
}

void TFishoid::loadItem(JSUMemoryInputStream& stream)
{
	u32 eventId;
	stream >> eventId;

	unk15C = TMapObjBaseManager::newAndRegisterObjByEventID(eventId, "");
	if (unk15C != nullptr) {
		if (unk15C->isActorType(ACTOR_TYPE_COIN))
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

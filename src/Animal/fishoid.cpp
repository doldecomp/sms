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
    , unk74(0)
{
}

void TRealoidActor::perform(u32 flags, JDrama::TGraphics* graphics)
{
	if (unk74 & 6)
		return;

	THitActor::perform(flags, graphics);
	if (!(unk74 & 1))
		unk70->perform(flags, graphics);
}

void TRealoidActor::calcRootMatrix(TBoid* boid)
{
	if (unk74 & 6)
		return;

	mPosition = boid->unk0;

	if (mHolder != nullptr) {
		MtxPtr hm = mHolder->getTakingMtx();
		JGeometry::TVec3<f32> v;
		v.x        = hm[0][3];
		v.y        = hm[1][3];
		v.z        = hm[2][3];
		boid->unk0 = v;
		unk70->getModel()->setBaseTRMtx(mHolder->getTakingMtx());
		return;
	}

	mPosition = boid->unk0;

	JGeometry::TVec3<f32> trans = boid->unk0;

	MtxPtr root = unk70->getModel()->getBaseTRMtx();

	JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
	JGeometry::TVec3<f32> dir = boid->unk18;
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
	if (unk74 & 6)
		return;

	s32 desired = 0x80000001;
	desired += 1;

	THitActor** it  = mCollisions;
	THitActor** end = mCollisions + mColCount;
	for (; it != end; ++it) {
		THitActor* actor = *it;
		s32 type         = actor->getActorType();
		if (type != desired)
			continue;

		SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
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

	unk150->setUnk38(mPosition);

	unk150->setGraph(unk124->getGraph(), mPosition);

	unk154 = new TRealoidActor*[count];

	TMActorKeeper* keeper     = mMActorKeeper;
	JGeometry::TVec3<f32> pos = mPosition;
	for (int i = 0; i < count; ++i) {
		MActor* actor = keeper->createMActor(name, 3);
		TBoid* boid   = &unk150->mBoids[i];
		boid->unk0    = pos;
		unk154[i]     = createRealoidActor(actor);
		pos.y += 10.0f;
	}
}

void TRealoid::clipBoids(JDrama::TGraphics* graphics)
{
	SetViewFrustumClipCheckPerspective(gpCamera->getFovy(),
	                                   gpCamera->getAspect(),
	                                   graphics->getNearPlane(), 10000.0f);

	for (int i = 0; i < unk150->getBoidNum(); ++i) {
		JGeometry::TVec3<f32> pos = unk150->getBoid(i)->unk0;
		if (ViewFrustumClipCheck(graphics, &pos, 100.0f))
			unk154[i]->offFlag(1);
		else
			unk154[i]->onFlag(1);
	}
}

void TRealoid::perform(u32 flags, JDrama::TGraphics* graphics)
{
	unk150->perform(flags, graphics);

	if (flags & 2) {
		clipBoids(graphics);
		for (int i = 0; i < unk150->getBoidNum(); ++i)
			unk154[i]->calcRootMatrix(unk150->getBoid(i));
	}

	for (int i = 0; i < unk150->getBoidNum(); ++i)
		unk154[i]->perform(flags, graphics);
}

void TFish::init() { mHitFlags |= HIT_FLAG_NO_COLLISION; }

TFishoid::TFishoid(int type, const char* name)
    : TRealoid(name)
{
	mType  = type;
	unk15C = nullptr;
}

void TFishoid::perform(u32 flags, JDrama::TGraphics* graphics)
{
	TRealoid::perform(flags, graphics);

	for (int i = 0; i < unk150->getBoidNum(); ++i) {
		TBoid* boid = unk150->getBoid(i);

		JGeometry::TVec3<f32> pos = boid->unk0;
		if (pos.y > 0.0f)
			pos.y = 0.0f;
		boid->unk0 = pos;
	}

	if (unk15C != nullptr && (flags & 1)) {
		unk15C->mPosition = unk150->getBoid(unk150->getBoidNum() - 1)->unk0;
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

	unk150->unk20 = 4.0f;
	unk150->unk24 = 200.0f;
	unk150->unk28 = 1.0f;
	unk150->unk2C = 0.5f;
	unk150->unk30 = 5.0f;
	unk150->unk34 = 0.5f;

	unk150->unk5C = (THitActor*)gpMarioAddress;

	unk150->unk6C = 400.0f;
	unk150->unk70 = 3.0f;
	unk150->unk1C |= 2;

	for (int i = 0; i < unk150->mNumBoids; ++i)
		unk154[i]->unk70->setBck("fish_swim");

	if (unk15C) {
		TRealoidActor* realoid = getRealoid(unk150->mNumBoids - 1);
		realoid->onFlag(2);
		unk15C->makeObjAppeared();
		unk15C->mPosition = realoid->mPosition;
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

TFishoidManager::~TFishoidManager() { }

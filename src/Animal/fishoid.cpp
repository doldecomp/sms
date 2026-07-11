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

namespace {
const char* cFishoidMdlNames[]
    = { "fishA.bmd", "fishB.bmd", "fishC.bmd", "fishD.bmd" };
} // namespace

TFish::~TFish() { }

TFishoid::~TFishoid() { }

TRealoidActor::TRealoidActor(MActor* actor)
    : TTakeActor("boid")
{
	unk70 = actor;
	unk74 = 0;
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
	// TODO: 91.6% - logic exact (look-at basis from boid dir). Residual =
	// retail frame padding + stfsu streaming matrix stores + holder-branch
	// model-mtx recompute. Not a fakematch.
	if (unk74 & 6)
		return;

	mPosition = boid->unk0;

	if (mHolder != nullptr) {
		MtxPtr hm = mHolder->getTakingMtx();
		boid->unk0.x = hm[0][3];
		boid->unk0.y = hm[1][3];
		boid->unk0.z = hm[2][3];
		PSMTXCopy(mHolder->getTakingMtx(), unk70->getModel()->unk20);
		return;
	}

	mPosition = boid->unk0;

	JGeometry::TVec3<f32> trans = boid->unk0;
	JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
	JGeometry::TVec3<f32> dir = boid->unk18;
	JGeometry::TVec3<f32> n;

	n.x = up.y * dir.z - up.z * dir.y;
	n.y = up.z * dir.x - up.x * dir.z;
	n.z = up.x * dir.y - up.y * dir.x;
	PSVECNormalize(&n, &n);

	up.x = dir.y * n.z - dir.z * n.y;
	up.y = dir.z * n.x - dir.x * n.z;
	up.z = dir.x * n.y - dir.y * n.x;
	PSVECNormalize(&up, &up);

	dir.x = n.y * up.z - n.z * up.y;
	dir.y = n.z * up.x - n.x * up.z;
	dir.z = n.x * up.y - n.y * up.x;
	PSVECNormalize(&dir, &dir);

	MtxPtr root = unk70->getModel()->unk20;
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

	THitActor** it  = mCollisions;
	THitActor** end = mCollisions + mColCount;
	for (; it != end; ++it) {
		if ((*it)->getActorType() == 0x80000001)
			SMS_SendMessageToMario(this, 0xE);
	}
}

MtxPtr TRealoidActor::getTakingMtx() { return unk78; }

TRealoid::TRealoid(const char* name)
    : TSpineEnemy(name)
{
	unk150 = 0;
	mLiveFlag |= 0x38;
}

void TRealoid::loadDefault(JSUMemoryInputStream& stream, const char* name,
                          int arg2)
{
	// TODO: 89.9% - logic exact. Residual = frame padding + TPathNode setup
	// staged via stack (struct-copy) + regalloc cascade.
	TSpineEnemy::load(stream);

	int count;
	stream.read(&count, 4);

	mMActorKeeper = new TMActorKeeper(mManager, count + arg2);
	unk150        = new TBoidLeader(count, "コントローラ");

	unk150->unk38.unk0 = nullptr;
	unk150->unk38.unk4 = mPosition;

	unk150->setGraph(unk124->getGraph(), mPosition);

	unk154 = new TRealoidActor*[count];

	JGeometry::TVec3<f32> pos = mPosition;
	for (int i = 0; i < count; ++i) {
		MActor* actor = mMActorKeeper->createMActor(name, 3);
		TBoid* boid   = &unk150->mBoids[i];
		boid->unk0    = pos;
		unk154[i]     = createRealoidActor(actor);
		pos.y += 10.0f;
	}
}

void TRealoid::clipBoids(JDrama::TGraphics* graphics)
{
	// TODO: 97.2% - logic exact; retail frame +0x30 (inline) + induction-var
	// regalloc numbering differ. Not a fakematch; leave as-is.
	SetViewFrustumClipCheckPerspective(gpCamera->mFovy, gpCamera->getAspect(),
	                                   graphics->mNearPlane, 10000.0f);

	for (int i = 0; i < unk150->mNumBoids; ++i) {
		JGeometry::TVec3<f32> pos = unk150->mBoids[i].unk0;
		if (ViewFrustumClipCheck(graphics, &pos, 100.0f))
			unk154[i]->unk74 &= ~1;
		else
			unk154[i]->unk74 |= 1;
	}
}

void TRealoid::perform(u32 flags, JDrama::TGraphics* graphics)
{
	// TODO: 92.7% - inlines clipBoids; residual inherits clipBoids' frame/
	// regalloc padding wall.
	unk150->perform(flags, graphics);

	if (flags & 2) {
		clipBoids(graphics);
		for (int i = 0; i < unk150->mNumBoids; ++i)
			unk154[i]->calcRootMatrix(&unk150->mBoids[i]);
	}

	for (int i = 0; i < unk150->mNumBoids; ++i)
		unk154[i]->perform(flags, graphics);
}

void TFish::init() { mHitFlags |= 1; }

TRealoid::~TRealoid() { }

TFishoid::TFishoid(int arg0, const char* name)
    : TRealoid(name)
{
	unk158 = arg0;
	unk15C = nullptr;
}

void TFishoid::perform(u32 flags, JDrama::TGraphics* graphics)
{
	// TODO: 81.3% - inlines TRealoid::perform; residual = frame/regalloc.
	TRealoid::perform(flags, graphics);

	for (int i = 0; i < unk150->mNumBoids; ++i) {
		JGeometry::TVec3<f32> pos = unk150->mBoids[i].unk0;
		if (pos.y > 0.0f)
			pos.y = 0.0f;
		unk150->mBoids[i].unk0 = pos;
	}

	if (unk15C != nullptr && (flags & 1)) {
		TBoid& last         = unk150->mBoids[unk150->mNumBoids - 1];
		unk15C->mPosition.x = last.unk0.x;
		unk15C->mPosition.y = last.unk0.y;
		unk15C->mPosition.z = last.unk0.z;
	}
}

void TFishoid::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);
	mSpine->initWith(&TNerveWaitForever<TLiveActor>::theNerve());
	initHitActor(0, 1, 0, 0.0f, 0.0f, 0.0f, 0.0f);
	mHitFlags |= 1;
}

void TFishoid::load(JSUMemoryInputStream& stream)
{
	// TODO: 66.2% - logic drafted; residual + coin-type check (0x2000000E) and
	// scheduling need refinement.
	loadDefault(stream, cFishoidMdlNames[unk158], 0);

	u32 eventId;
	stream.read(&eventId, 4);

	unk15C = TMapObjBaseManager::newAndRegisterObjByEventID(eventId, "");
	if (unk15C != nullptr) {
		if (unk15C->getActorType() == 0x2000000E)
			unk15C = gpItemManager->newAndRegisterCoinReal();
	}

	unk150->unk20 = 4.0f;
	unk150->unk24 = 200.0f;
	unk150->unk28 = 1.0f;
	unk150->unk2C = 0.5f;
	unk150->unk30 = 5.0f;
	unk150->unk34 = 0.5f;

	JGeometry::TVec3<f32> mpos;
	SMS_GetMarioPosStupid(&mpos);
	unk150->unk5C.unk0 = (THitActor*)gpMarioAddress;
	unk150->unk5C.unk4 = mpos;

	unk150->unk6C = 400.0f;
	unk150->unk70 = 3.0f;
	unk150->unk1C |= 2;

	for (int i = 0; i < unk150->mNumBoids; ++i)
		unk154[i]->unk70->setBck("fish_swim");
}

TRealoidActor::~TRealoidActor() { }

TRealoidActor* TFishoid::createRealoidActor(MActor* actor)
{
	// TODO: 71.9% - inlines TFish/TRealoidActor ctor; residual is regalloc
	return new TFish(actor);
}

TFishoidManager::TFishoidManager(const char* name)
    : TEnemyManager(name)
{
}

void TFishoidManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
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

#include <Animal/fishoid.hpp>
#include <Animal/boid.hpp>
#include <Camera/Camera.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoaderFlags.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/ObjManager.hpp>
#include <Strategic/ObjModel.hpp>

TRealoid::TRealoid(const char* name)
    : TSpineEnemy(name)
{
	unk150 = 0;
	mLiveFlag |= 0x38;
}

TRealoid::~TRealoid() { }

TRealoidActor::TRealoidActor(MActor* actor)
    : TTakeActor("boid")
{
	unk70 = actor;
	unk74 = 0;
}

TRealoidActor::~TRealoidActor() { }

MtxPtr TRealoidActor::getTakingMtx() { return unk78; }

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

void TRealoidActor::perform(u32 flags, JDrama::TGraphics* graphics)
{
	if (unk74 & 6)
		return;

	THitActor::perform(flags, graphics);
	if (!(unk74 & 1))
		unk70->perform(flags, graphics);
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

TFishoidManager::TFishoidManager(const char* name)
    : TEnemyManager(name)
{
}

TFishoidManager::~TFishoidManager() { }

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

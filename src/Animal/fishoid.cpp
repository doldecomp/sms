#include <Animal/fishoid.hpp>
#include <Animal/boid.hpp>
#include <Camera/Camera.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoaderFlags.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/ObjManager.hpp>

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

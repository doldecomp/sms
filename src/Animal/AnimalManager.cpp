#include <Animal/AnimalManager.hpp>
#include <Animal/AnimalSave.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoaderFlags.hpp>
#include <MSound/SoundEffects.hpp>
#include <MSound/MSoundSE.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <Camera/Camera.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <Strategic/LiveActor.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: promote into EnemyManager.hpp. Retail's TEnemyManager::getObj(int)
// casts TObjManager::getObj()'s result directly; our version routes through
// TLiveManager::getObj(), and that extra inline level costs one 4-byte
// temporary, which shifts every local of clipEnemies up by 4. Parked as a
// TU-local helper because the shared header is off limits in this batch.
static inline TSpineEnemy* AnimalManagerGetObj(TAnimalManagerBase* manager,
                                               int i)
{
	return (TSpineEnemy*)manager->TObjManager::getObj(i);
}

TAnimalManagerBase::TAnimalManagerBase(const char* name)
    : TEnemyManager(name)
{
	mViewClipNear   = 250.0f;
	mViewClipFarPtr = nullptr;
	mAnimalSave     = nullptr;
}

void TAnimalManagerBase::loadSaveParams_(const char* path)
{
	mAnimalSave     = new TAnimalSaveIndividual(path);
	mViewClipNear   = mAnimalSave->mSLViewClipNear.get();
	mViewClipFarPtr = &mAnimalSave->mSLViewClipFar.get();
	unk3C           = mAnimalSave->mSLViewClipRadius.get();
}

void TAnimalManagerBase::clipEnemies(JDrama::TGraphics* graphics)
{
	f32 nearClip = mViewClipNear;
	SetViewFrustumClipCheckPerspective(gpCamera->getFovy(), gpCamera->getAspect(),
	                                   nearClip, *mViewClipFarPtr);

	int count = getObjNum();
	for (int i = 0; i < count; ++i) {
		TSpineEnemy* actor        = AnimalManagerGetObj(this, i);
		JGeometry::TVec3<f32> pos = actor->mPosition;
		pos.y += 75.0f;

		if (actor->checkLiveFlag(LIVE_FLAG_UNK2000)
		    && SMS_IsInOtherFastCube(pos)) {
			actor->onLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		} else {
			if (ViewFrustumClipCheck(graphics, &actor->mPosition, unk3C))
				actor->offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
			else
				actor->onLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		}
	}
}

void TMewManager::load(JSUMemoryInputStream& stream)
{
	TAnimalManagerBase::load(stream);
	loadSaveParams_("/Animal/mew.prm");
}

// TODO: frame 0x18 vs retail 0x28; all 15 instructions match. Retail declared
// 13-16 bytes of locals here that nothing reads (measured: 12 bytes of locals
// give 0x20, 16 give 0x28). Same shape in TAnimalBase::loadAfter (+16, one
// registerTrans), TAnimalBird::loadAfter (+24) and TAnimalBirdManager::loadAfter
// (+32). Ruled out: mObjNum vs getObjNum()/TObjManager::getObjNum()/a named u16/
// an explicit (u16) cast (all zero), inline forwarder levels above
// TNameRef::loadAfter (zero), argument-count effects on the outgoing parameter
// area (zero: a 5- or 6-argument call does not grow it). MSound::setPlayerInfo
// matches with six MSRandPlay calls, so the cost is not per call site.
void TMewManager::loadAfter()
{
	TAnimalManagerBase::loadAfter();
	MSoundSESystem::MSRandPlay::createRandPlayVec(MSD_SE_OBJ_KAMOME_SOLO,
	                                              getObjNum());
}

void TMewManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "kamome_high.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
		      | (1 << J3DMLF_TevStageNumShift),
		  1 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

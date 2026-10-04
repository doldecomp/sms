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

TAnimalManagerBase::TAnimalManagerBase(const char* name)
    : TEnemyManager(name)
{
	mViewClipNear   = 250.0f;
	mViewClipFarPtr = nullptr;
	mAnimalSave     = nullptr;
}

void TAnimalManagerBase::clipEnemies(JDrama::TGraphics* graphics)
{
	SetViewFrustumClipCheckPerspective(gpCamera->mFovy, gpCamera->getAspect(),
	                                   mViewClipNear, *mViewClipFarPtr);

	s32 count = mObjNum;
	for (int i = 0; i < count; ++i) {
		TLiveActor* actor         = (TLiveActor*)unk18[i];
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
	mAnimalSave     = new TAnimalSaveIndividual("/Animal/mew.prm");
	mViewClipNear   = mAnimalSave->mSLViewClipNear.get();
	mViewClipFarPtr = &mAnimalSave->mSLViewClipFar.get();
	unk3C           = mAnimalSave->mSLViewClipRadius.get();
}

void TMewManager::loadAfter()
{
	TAnimalManagerBase::loadAfter();
	MSoundSESystem::MSRandPlay::createRandPlayVec(MSD_SE_OBJ_KAMOME_SOLO,
	                                              mObjNum);
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

#include <Animal/AnimalManager.hpp>
#include <Animal/AnimalSave.hpp>
#include <MSound/MSoundBGM.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <Camera/Camera.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <Strategic/LiveActor.hpp>

void TMewManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "kamome_high.bmd", 0x10210000, 1 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TMewManager::loadAfter()
{
	JDrama::TNameRef::loadAfter();
	MSoundSESystem::MSRandPlay::createRandPlayVec(0x3813, mObjNum);
}

void TMewManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	mAnimalSave     = new TAnimalSaveIndividual("/Animal/mew.prm");
	mViewClipNear   = mAnimalSave->mSLViewClipNear.value;
	mViewClipFarPtr = &mAnimalSave->mSLViewClipFar.value;
	unk3C           = mAnimalSave->mSLViewClipRadius.value;
}

void TAnimalManagerBase::clipEnemies(JDrama::TGraphics* graphics)
{
	// TODO: stack frame inflation from inline expansion
	volatile u8 _pad[8];

	SetViewFrustumClipCheckPerspective(gpCamera->mFovy, gpCamera->getAspect(),
	                                   mViewClipNear, *mViewClipFarPtr);

	s32 count = mObjNum;
	for (int i = 0; i < count; ++i) {
		TLiveActor* actor         = (TLiveActor*)getObj(i);
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

TAnimalManagerBase::TAnimalManagerBase(const char* name)
    : TEnemyManager(name)
{
	mViewClipNear   = 250.0f;
	mViewClipFarPtr = nullptr;
	mAnimalSave     = nullptr;
}

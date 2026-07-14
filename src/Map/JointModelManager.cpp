#include <Map/JointModelManager.hpp>
#include <Map/JointModel.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

void TJointModelManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	for (int i = 0; i < getJointModelNum(); ++i)
		mJointModels[i]->perform(cue, graphics);
}

void TJointModelManager::initJointModel(const char* param_1,
                                        const char** param_2)
{
	if (mJointModelNum == 0)
		return;
	mModelsFolder = param_1;
	mActorAnimationData.init(mModelsFolder, nullptr);
	mJointModels = new TJointModel*[mJointModelNum];
	for (int i = 0; i < getJointModelNum(); ++i) {
		mJointModels[i] = newJointModel(i);
		mJointModels[i]->setIndexInParent(i);
		mJointModels[i]->initJointModel(this, param_2[i], &mActorAnimationData);
	}
}

TJointModelManager::TJointModelManager(const char* name)
    : JDrama::TViewObj(name)
    , mJointModelNum(0)
    , mJointModels(nullptr)
    , unk18(0)
    , mModelsFolder(nullptr)
{
}

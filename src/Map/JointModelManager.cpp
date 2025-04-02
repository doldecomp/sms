#include <Map/JointModelManager.hpp>
#include <Map/JointModel.hpp>

// rouge include needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>

TJointModel* TJointModelManager::newJointModel(int) const
{
	return new TJointModel;
}

TJointModelManager::TJointModelManager(const char* name)
    : JDrama::TViewObj(name)
    , mJointModelNum(0)
    , mJointModels(nullptr)
    , unk18(0)
    , mModelsFolder(nullptr)
{
}

void TJointModelManager::initJointModel(const char* param_1,
                                        const char** param_2)
{
	if (mJointModelNum == 0)
		return;
	mModelsFolder = param_1;
	mActorAnimationData.init(mModelsFolder, nullptr);
	mJointModels = new TJointModel*[mJointModelNum];
	for (int i = 0; i < mJointModelNum; ++i) {
		mJointModels[i] = newJointModel(i);
		mJointModels[i]->setIndexInParent(i);
		mJointModels[i]->initJointModel(this, param_2[i], &mActorAnimationData);
	}
}

void TJointModelManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	for (int i = 0; i < mJointModelNum; ++i)
		mJointModels[i]->perform(param_1, param_2);
}

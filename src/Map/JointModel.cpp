#include <Map/JointModel.hpp>
#include <Map/JointModelManager.hpp>
#include <M3DUtil/MActor.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TJointModel::TJointModel()
    : mManager(0)
    , mModelData(0)
    , mModel(0)
    , mActor(0)
{
}

void TJointModel::initJointModel(TJointModelManager* param_1,
                                 const char* param_2, MActorAnmData* param_3)
{
	mManager = param_1;
	initActor(param_2, param_3);
	TJointObj::initJointObj(mModelData->getJointNodePointer(0));
}

void TJointModel::initActor(const char* name, MActorAnmData* anm_data)
{
	char path[64];
	snprintf(path, 64, "/%s/%s.bmd", mManager->getFolder(), name);

	void* res  = JKRGetResource(path);
	u32 flag   = getJ3DModelDataFlag();
	mModelData = J3DModelLoaderDataBase::load(res, flag);
	mModel     = new J3DModel(mModelData, 0, 1);

	for (u16 i = 0; i < mModelData->getMaterialNum(); ++i)
		mModelData->getMaterialNodePointer(i)->calc((MtxPtr)j3dDefaultMtx);

	mModel->makeDL();
	mModel->lock();
	mActor = new MActor(anm_data);
	mActor->setModel(mModel, 0);
}

void TJointModel::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (!checkFlag(1))
		mActor->perform(param_1, param_2);
}

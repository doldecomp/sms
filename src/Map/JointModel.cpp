#include <Map/JointModel.hpp>
#include <Map/JointModelManager.hpp>
#include <M3DUtil/MActor.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>

// rouge include needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>

TJointModel::TJointModel()
    : unk20(0)
    , unk24(0)
    , unk28(0)
    , unk2C(0)
{
}

void TJointModel::initJointModel(TJointModelManager* param_1,
                                 const char* param_2, MActorAnmData* param_3)
{
	unk20 = param_1;
	initActor(param_2, param_3);
	TJointObj::initJointObj(unk24->getJointNodePointer(0));
}

void TJointModel::initActor(const char* name, MActorAnmData* anm_data)
{
	char path[64];
	snprintf(path, 64, "/%s/%s.bmd", unk20->getFolder(), name);

	void* res = JKRGetResource(path);
	u32 flag  = getJ3DModelDataFlag();
	unk24     = J3DModelLoaderDataBase::load(res, flag);
	unk28     = new J3DModel(unk24, 0, 1);

	for (u16 i = 0; i < unk24->getMaterialNum(); ++i)
		unk24->getMaterialNodePointer(i)->calc((MtxPtr)j3dDefaultMtx);

	unk28->makeDL();
	unk28->lock();
	unk2C = new MActor(anm_data);
	unk2C->setModel(unk28, 0);
}

void TJointModel::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (!checkFlag(1))
		unk2C->perform(param_1, param_2);
}

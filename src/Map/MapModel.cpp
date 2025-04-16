#include <Map/MapModel.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <M3DUtil/MActor.hpp>
#include <Player/MarioAccess.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/Blocks/J3DPEBlocks.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TMapModelManager::TMapModelManager(const char* name)
    : TJointModelManager(name)
    , mCollision(nullptr)
{
}

void TMapModelManager::init()
{
	static const char* name_table[] = {
		"map",
		"map_obj",
		"station",
		"inside",
	};

	mJointModelNum = 1;
	initJointModel("scene/map/map", name_table);
	mCollision = new TMapCollisionStatic;
	mCollision->init("/scene/map/map.col", 2, nullptr);
}

void TMapModel::initJointModel(TJointModelManager* param_1, const char* param_2,
                               MActorAnmData* param_3)
{
	TJointModel::initJointModel(param_1, param_2, param_3);
	if (mActor->checkAnmFileExist(param_2, 4))
		mActor->setBtk(param_2);
	if (mActor->checkAnmFileExist(param_2, 5))
		mActor->setBrk(param_2);

	mActor->calc();
	initUnderpass();
}

void TMapModel::initUnderpass()
{
	s32 nameIdx = mModelData->unkB0->getIndex("underpass");
	if (nameIdx < 0)
		return;

	J3DJoint* underpass = mModelData->getJointNodePointer(nameIdx);
	int i;
	for (i = 0; i < mChildrenNum && mChildren[i]->mJoint != underpass; ++i)
		;

	mUnderpass         = mChildren[i];
	mUnderpassMaterial = underpass->getMesh();
	mUnderpassMaterial->change();
	mUnderpassMaterial->unk1C |= 1;

	J3DTexCoord* texCoord
	    = mUnderpassMaterial->getTexGenBlock()->getTexCoord(0);
	texCoord->setTexGenType(GX_TG_MTX2x4);
	texCoord->setTexGenSrc(GX_TG_POS);
	texCoord->setTexGenMtx(GX_TEXMTX0);
	J3DTexMtx* texMtxInfo = new J3DTexMtx;
	texMtxInfo->mInfo     = 2;
	mUnderpassMaterial->getTexGenBlock()->setTexMtx(0, texMtxInfo);
	J3DZMode* zmode = mUnderpassMaterial->getPEBlock()->getZMode();

	zmode->setCompareFunc(GX_ALWAYS);
	zmode->setUpdateEnable(GX_FALSE);
}

void TMapModel::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (checkFlag(1))
		return;

	JGeometry::TVec3<f32>* pos = gpMarioPos;
	if ((param_1 & 2) != 0 && mUnderpass != nullptr) {
		// NOTE: this seems to be the logic for entering delphino plaza
		// underpasses: if inside, draw them on top of everything and move the
		// camera to top view
		if ((*gpMarioFlag & 2) && SMS_GetMarioGrLevel() + 200.0f < pos->y) {
			mUnderpass->awake();
			// TODO: "move camera"
			mUnderpassMaterial->getTexGenBlock()->getTexMtx(0)->setEffectMtx(
			    nullptr);
		} else {
			mUnderpass->sleep();
		}
	}

	if ((param_1 & 2) != 0) {
		param_1 &= ~2;
		mActor->frameUpdate();
	}

	mActor->perform(param_1, param_2);
}

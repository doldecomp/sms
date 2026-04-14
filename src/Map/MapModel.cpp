#include <Map/MapModel.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <M3DUtil/MActor.hpp>
#include <Player/MarioAccess.hpp>
#include <Camera/Camera.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/Blocks/J3DPEBlocks.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

void TMapModel::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (checkFlag(1))
		return;

	if ((param_1 & 2) != 0 && mUnderpass != nullptr) {
		// NOTE: this seems to be the logic for entering delphino plaza
		// underpasses: if inside, draw them on top of everything and move the
		// camera to top view
		if (SMS_CheckMarioFlag(0x2)
		    && SMS_GetMarioPos().y < SMS_GetMarioGrLevel() + 200.0f) {
			mUnderpass->awake();

			Mtx view;
			Mtx proj;
			Mtx viewProj;

			Vec up;
			Vec pos;

			gpCamera->JSGGetViewPosition(&pos);
			gpCamera->JSGGetViewUpVector(&up);

			C_MTXLightOrtho(proj, unk38 * 1000.0f, unk38 * -1000.0f,
			                unk38 * -1000.0f, unk38 * 1000.0f, 0.5f, 0.5f, 0.5f,
			                0.5f);
			C_MTXLookAt(view, &pos, &up, gpMarioPos);
			MTXConcat(proj, view, viewProj);

			mUnderpassMaterial->getTexMtx(0)->setEffectMtx(viewProj);
		} else {
			mUnderpass->sleep();
		}
	}

	if ((param_1 & 2) != 0) {
		MActor* actor = mActor;
		param_1 &= ~2;
		actor->frameUpdate();
	}

	mActor->perform(param_1, param_2);
}

void TMapModel::initUnderpass()
{
	s32 nameIdx = mModelData->getJointName()->getIndex("underpass");
	if (nameIdx < 0)
		return;

	J3DJoint* underpass = mModelData->getJointNodePointer(nameIdx);
	int i;
	for (i = 0; i < mChildrenNum && mChildren[i]->getJoint() != underpass; ++i)
		;

	mUnderpass         = mChildren[i];
	mUnderpassMaterial = underpass->getMesh();
	mUnderpassMaterial->change();
	mUnderpassMaterial->setSomeFlag();

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

TMapModel::TMapModel()
    : mUnderpass(nullptr)
    , mUnderpassMaterial(nullptr)
    , unk38(0.5f)
    , unk3C(1.0f)
{
}

void TMapModelManager::init()
{
	static const char* name_table[] = {
		"map", "map_obj", "station", "inside", nullptr,
	};

	mJointModelNum = 1;
	initJointModel("scene/map/map", name_table);
	mCollision = new TMapCollisionStatic;
	mCollision->init("/scene/map/map.col", 2, nullptr);
}

TMapModelManager::TMapModelManager(const char* name)
    : TJointModelManager(name)
    , mCollision(nullptr)
{
}

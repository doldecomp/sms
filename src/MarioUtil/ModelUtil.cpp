#include <MarioUtil/ModelUtil.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAnm.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <Player/MarioMain.hpp>
#include <dolphin/mtx.h>
#include <MarioUtil/MtxUtil.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <System/Application.hpp>

TMultiBtk::TMultiBtk(int param_1, J3DModelData* param_2)
{
	unk00 = param_1;
	unk04 = new J3DAnmTextureSRTKey*[param_1];
	unk08 = param_2;
	unk0c = new J3DFrameCtrl[param_1];
}

void TMultiBtk::setNthData(int n, J3DAnmTextureSRTKey* param_2)
{
	unk04[n] = param_2;
	unk04[n]->searchUpdateMaterialID(unk08);
	if (n == 0) {
		for (u16 i = 0; i < unk08->getMaterialNum(); ++i) {
			J3DMaterialAnm* mat = new J3DMaterialAnm;
			unk08->getMaterialNodePointer((int)i)->change();
			unk08->getMaterialNodePointer((int)i)->setMaterialAnm(mat);
		}
	}

	unk08->entryTexMtxAnimator(unk04[n]);
	unk0c[n].setEndFrame(unk04[n]->getFrameMax());
	unk0c[n].setSpeed(0.5f * SMSGetAnmFrameRate());
}

#pragma opt_strength_reduction off

void TMultiBtk::update()
{
	for (int i = 0; i < unk00; ++i) {
		unk0c[i].update();
		unk04[i]->setFrame(unk0c[i].getCurrentFrame());
	}
}

void SMS_RideMoveByGroundActor(TRidingInfo* riding_info,
                               JGeometry::TVec3<f32>* pos, f32* arg2)
{
	const TBGCheckData* checkData;
	f32 temp_f1
	    = gpMap->checkGround(pos->x, 100.0f + pos->y, pos->z, &checkData);

	if (checkData->unk44 != nullptr && ((pos->y - temp_f1) < 50.0f)) {
		if (riding_info->unk0 == nullptr
		    || riding_info->unk0 != checkData->unk44) {
			riding_info->unk0 = checkData->unk44;
			SMS_RideMoveCalcLocalPos(riding_info, *pos);
		} else {
			TMtx34f mtx;
			if (!riding_info->unk0->getTakingMtx()) {
				SMS_GetActorMtx(*riding_info->unk0, mtx.mMtx);
			} else {
				PSMTXCopy(riding_info->unk0->getTakingMtx(), mtx.mMtx);
			}
			MTXMultVec(mtx.mMtx, &riding_info->localPos, pos);
			*arg2 = *arg2 + riding_info->unk0->mRotation.y - riding_info->unk10;
			riding_info->unk10 = riding_info->unk0->mRotation.y;
		}
	} else {
		riding_info->unk0 = nullptr;
	}
}

void SMS_RideMoveCalcLocalPos(TRidingInfo* riding_info,
                              const JGeometry::TVec3<float>& pos)
{

	if (riding_info->unk0 == nullptr)
		return;

	TMtx34f mtx;
	if (!riding_info->unk0->getTakingMtx()) {
		SMS_GetActorMtx(*riding_info->unk0, mtx.mMtx);
	} else {
		PSMTXCopy(riding_info->unk0->getTakingMtx(), mtx.mMtx);
	}
	PSMTXInverse(mtx.mMtx, mtx.mMtx);
	MTXMultVec(mtx.mMtx, (Vec*)&pos, &riding_info->localPos);
}

J3DModel* SMS_CreatePartsModel(char* arg0, u32 arg1)
{
	J3DModelData* temp_r31 = J3DModelLoaderDataBase::load(
	    JKRFileLoader::getGlbResource(arg0), arg1);
	J3DModel* temp_r3 = new J3DModel(temp_r31, arg1, 1U);
	return temp_r3;
}

SDLModel* SMS_CreateMinimumSDLModel(const char* name)
{
	J3DModelData* temp_r31 = J3DModelLoaderDataBase::load(
	    JKRFileLoader::getGlbResource(name), 0x10000);
	SDLModelData* temp_r3_2 = new SDLModelData(temp_r31);
	SDLModel* temp_r3       = new SDLModel(temp_r3_2, 3U, 1U);
	return temp_r3;
}

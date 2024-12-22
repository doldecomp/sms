#include <MarioUtil/ModelUtil.hpp>
#include <JSystem/J3DModelLoader.hpp>
#include <JSystem/JKRFileLoader.hpp>
#include <JSystem/J3DAnimation.hpp>
#include <JSystem/J3DMaterialAnm.hpp>
#include <JSystem/J3DMaterial.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <Player/MarioMain.hpp>
#include <dolphin/mtx.h>
#include <MarioUtil/MtxUtil.hpp>
#include <Map/Map.hpp>
#include <System/Application.hpp>

SDLModel* SMS_CreateMinimumSDLModel(const char* name)
{
	J3DModelData* temp_r31 = J3DModelLoaderDataBase::load(
	    JKRFileLoader::getGlbResource(name), 0x10000);
	SDLModelData* temp_r3_2 = new SDLModelData(temp_r31);
	SDLModel* temp_r3       = new SDLModel(temp_r3_2, 3U, 1U);
	return temp_r3;
}

J3DModel* SMS_CreatePartsModel(char* arg0, u32 arg1)
{
	J3DModelData* temp_r31 = J3DModelLoaderDataBase::load(
	    JKRFileLoader::getGlbResource(arg0), arg1);
	J3DModel* temp_r3 = new J3DModel(temp_r31, arg1, 1U);
	return temp_r3;
}

void SMS_RideMoveCalcLocalPos(TRidingInfo* riding_info,
                              const JGeometry::TVec3<float>& pos)
{

	if (riding_info->unk0 == NULL)
		return;

	TMtx34f mtx;
	if (!riding_info->unk0->uknownVirtFunc()) {
		SMS_GetActorMtx(*riding_info->unk0, mtx.mMtx);
	} else {
		PSMTXCopy(riding_info->unk0->uknownVirtFunc(), mtx.mMtx);
	}
	PSMTXInverse(mtx.mMtx, mtx.mMtx);
	PSMTXMultVec(mtx.mMtx, pos.toVec(), &riding_info->localPos);
}

static float kek(float a) { return a; }

void SMS_RideMoveByGroundActor(TRidingInfo* riding_info,
                               JGeometry::TVec3<float>* pos, f32* arg2)
{

	const TBGCheckData* checkData;
	f32 temp_f1
	    = gpMap->checkGround(pos->x, 100.0f + pos->y, pos->z, &checkData);
	if ((checkData->unk44 != NULL) && ((pos->y - temp_f1) < 50.0f)) {
		if ((riding_info->unk0 == NULL)
		    || (riding_info->unk0 != checkData->unk44)) {
			riding_info->unk0 = checkData->unk44;
			SMS_RideMoveCalcLocalPos(riding_info, *pos);
		} else {
			TMtx34f mtx;
			if (!riding_info->unk0->uknownVirtFunc()) {
				SMS_GetActorMtx(*riding_info->unk0, mtx.mMtx);
			} else {
				PSMTXCopy(riding_info->unk0->uknownVirtFunc(), mtx.mMtx);
			}
			PSMTXMultVec(mtx.mMtx, &riding_info->localPos, pos->toVec());
			// TODO: there is 100% some kind of a getter/setter getting called
			// here because the stack frame is bigger than necessary by 16 bytes
			// and `mtx` is offset on the stack a little bit but I couldn't
			// figure out some thing sensible for now, so an inline kek shall
			// remain here for now
			float trash        = kek(riding_info->unk0->unk34);
			*arg2              = *arg2 + trash - riding_info->unk10;
			riding_info->unk10 = riding_info->unk0->unk34;
		}
	} else {
		riding_info->unk0 = (THitActor*) NULL;
	}
}

void TMultiBtk::update()
{
	// TODO: can't match because of a skill issue, sorry
	char trash[0x8];
	for (int i = 0; i < unk00; ++i) {
		unk0c[i].update();
		unk04[i]->unk4 = unk0c[i].getCurrentFrame();
	}
}

void TMultiBtk::setNthData(int n, J3DAnmTextureSRTKey* param_2)
{
	unk04[n] = param_2;
	unk04[n]->searchUpdateMaterialID(unk08);
	if (n == 0) {
		for (u16 i = 0; i < unk08->mMaterialTable.getMaterialNum(); ++i) {
			J3DMaterialAnm* mat = new J3DMaterialAnm();
			// TODO: why does it not match without a cast
			unk08->mMaterialTable.getMaterial((u16)i)->change();
			unk08->mMaterialTable.getMaterial((u16)i)->setMaterialAnm(mat);
		}
	}

	unk08->entryTexMtxAnimator(unk04[n]);
	unk0c[n].setEndFrame(unk04[n]->getMaxFrame());
	unk0c[n].setSpeed(0.5f * SMSGetAnmFrameRate());
}

TMultiBtk::TMultiBtk(int param_1, J3DModelData* param_2)
{
	unk00 = param_1;
	unk04 = new J3DAnmTextureSRTKey*[param_1];
	unk08 = param_2;
	unk0c = new J3DFrameCtrl[param_1];
}

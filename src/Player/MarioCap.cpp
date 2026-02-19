#include <Player/MarioCap.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>

const char cDirtyFileName[] = "/scene/map/pollution/H_ma_rak.bti";
const char cDirtyTexName[]  = "H_ma_rak_dummy";

TMarioCap::TMarioCap(TMario* mario)
{
	// Unused stack space
	// volatile u32 padding[51];
	mMario = mario;

	J3DModelData* maCap1ModelData = J3DModelLoaderDataBase::load(
	    JKRFileLoader::getGlbResource("/mario/bmd/ma_cap1.bmd"), 0x10100000);
	// Might be an inlined function?
	maCap1ModelData->getTexture()->setResTIMG(
	    0, *mMario->mModel->unk8->getModelData()->getTexture()->getResTIMG(0));
	DCFlushRange(maCap1ModelData->getTexture()->mResources, 0x20);
	unk10[0] = new J3DModel(maCap1ModelData, 0, 1);

	J3DModelData* maCap3ModelData = J3DModelLoaderDataBase::load(
	    JKRFileLoader::getGlbResource("/mario/bmd/ma_cap3.bmd"), 0x10100000);
	// I could see this being an inlined
	maCap3ModelData->getTexture()->setResTIMG(
	    0, *mMario->mModel->unk8->getModelData()->getTexture()->getResTIMG(0));
	DCFlushRange(maCap3ModelData->getTexture()->mResources, 0x20);
	unk10[1] = new J3DModel(maCap3ModelData, 0, 1);

	if (mMario->mBodyPollutionTex != 0) {
		for (int i = 0; i < 2; ++i) {
			SMS_ChangeTextureAll(unk10[i]->getModelData(), cDirtyTexName,
			                     *mMario->mBodyPollutionTex);
			SMS_MakeDLAndLock(unk10[i]);
		}
	}

	J3DModelData* diverHelmModelData = J3DModelLoaderDataBase::load(
	    JKRFileLoader::getGlbResource("/mario/bmd/diver_helm.bmd"), 0x10100000);
	unk10[2] = new J3DModel(diverHelmModelData, 0, 1);

	J3DModelData* maGlass1 = J3DModelLoaderDataBase::load(
	    JKRFileLoader::getGlbResource("/mario/bmd/ma_glass1.bmd"), 0x10100000);
	unk10[3] = new J3DModel(maGlass1, 0, 1);

	// Mmmh, nintendo plz? I hope this is forgotten and not a check to crash the
	// game if it is missing this bone
	unk10[2]->getModelData()->unkB0->getIndex("null_airtube");
	MtxPtr mtx = mMario->mModel->unk8->getAnmMtx(mMario->mBoneIDs[11]);

	unk10[0]->setAnmMtx(0, mtx);
	unk10[0]->calc();
	unk10[1]->setAnmMtx(0, mtx);
	unk10[1]->calc();
	mMario->mModel->unk8->setAnmMtx(mMario->mBoneIDs[10],
	                                unk10[2]->getBaseTRMtx());
	unk10[2]->calc();

	unk20 = new TMultiMtxEffect();
	unk24 = new TMultiMtxEffect();

	// This feels very wrong
	// Probably some inline constructor?
	unk20->mNumBones      = 1;
	u16* unk20unk8Ptr     = new u16[1];
	unk20unk8Ptr[0]       = 1;
	unk20->mBoneIDs       = unk20unk8Ptr;
	u8* unk20unk4Ptr      = new u8[1];
	unk20unk4Ptr[0]       = TMTX_EFFECT_TIME_LAG;
	unk20->mMtxEffectType = unk20unk4Ptr;
	unk20->setup(unk10[0], "Mario/MarioCap");

	unk24->mNumBones      = 1;
	u16* unk24unk8Ptr     = new u16[1];
	unk24unk8Ptr[0]       = 1;
	unk24->mBoneIDs       = unk24unk8Ptr;
	u8* unk24unk4Ptr      = new u8[1];
	unk24unk4Ptr[0]       = TMTX_EFFECT_TIME_LAG;
	unk24->mMtxEffectType = unk24unk4Ptr;
	unk24->setup(unk10[1], "Mario/MarioCap");

	setModelActive(E_CAP_MODEL_HAT);
	unkC = unk10[0];

	// TMultiMtxEffect stuff
	unk30 = new TTrembleModelEffect();
	unk30->init(unk10[0]);
	unk34 = 4.0f;

	for (int idx = 0; idx < 2; idx++) {
		for (int matIdx = 0;
		     matIdx < unk10[idx]->getModelData()->getMaterialNum(); matIdx++) {
			SMS_InitPacket_OneTevKColorAndFog(unk10[idx], matIdx, GX_KCOLOR0,
			                                  nullptr);
		}
	}
}

void TMarioCap::createMirrorCap()
{
	for (int i = 0; i < 2; ++i) {
		unk28[i] = new TMirrorActor("マリオ帽子in鏡");
		unk28[i]->init(unk10[i], 4);
	}
}

void TMarioCap::perform(unsigned long param_1, JDrama::TGraphics* param_2)
{
	// Unused stack space
	// volatile u32 padding[42];

	if ((param_1 & 2) != 0) {
		if (mMario->unk0FA == 0x12D) {
			J3DFrameCtrl* frameCtrl = mMario->getMotionFrameCtrl();
			if (frameCtrl->getCurrentFrame() < 157.0f) {
				unkC = unk10[1];
				onFlagAllShapes(unk10[0]->mModelData);
				offFlagAllShapes(unk10[1]->mModelData);
			} else {
				unkC = unk10[0];
				offFlagAllShapes(unk10[0]->mModelData);
				onFlagAllShapes(unk10[1]->mModelData);
			}
		} else if (isModelActive(E_CAP_MODEL_HAT)) {
			unkC = unk10[0];
			offFlagAllShapes(unk10[0]->mModelData);
			onFlagAllShapes(unk10[1]->mModelData);

			bool doTremble = false;

			// Missing a copy of TVec3, i still suspect that operations should
			// do a copy
			f32 distance = (mMario->mPosition - mMario->unk29C).length();
			if (mMario->mAction == 0x810446 && distance > 20.0f) {
				doTremble = true;
			}
			if (mMario->mAction == 0x281089A) {
				doTremble = true;
			}
			if (mMario->mAction == 0x81089B) {
				doTremble = true;
			}

			// Probably inline check
			if (((mMario->mAction & 0x800) ? true : false)
			    && distance > 20.0f) {
				doTremble = true;
			}
			if (mMario->mAction == 0x891) {
				doTremble = false;
			}

			if (doTremble == true) {
				unk30->clash(unk34);
			} else {
				unk30->clash(0.0f);
			}
		} else {

			unkC = unk10[1];
			onFlagAllShapes(unk10[0]->mModelData);
			offFlagAllShapes(unk10[1]->mModelData);
		}

		if (mMario->checkFlag(MARIO_FLAG_HELMET_FLW_CAMERA)) {
			setModelActive(E_CAP_MODEL_HELMET);
		}

		if (isModelActive(E_CAP_MODEL_HELMET)) {

			J3DModelData* modelData = unk10[2]->mModelData;
			for (u16 i = 0; i < modelData->getShapeNum(); ++i) {
				modelData->getShapeNodePointer(i)->onFlag(1);
			}
		} else {
			J3DModelData* modelData = unk10[2]->mModelData;
			for (u16 i = 0; i < modelData->getShapeNum(); ++i) {
				modelData->getShapeNodePointer(i)->offFlag(1);
			}
		}

		if (isModelActive(E_CAP_MODEL_SUNGLASSES)) {
			unk10[3]->update();
		}
		unkC->update();
		unk10[2]->update();

		for (u16 i = 0; i < unkC->getModelData()->getMaterialNum(); i++) {
			J3DGXColor* color
			    = unkC->getModelData()->getMaterialNodePointer(i)->getTevKColor(
			        0);

			color->color.a = mMario->unk134;
		}
	}

	if ((param_1 & 4) != 0) {
		unkC->viewCalc();
		// Likely inline
		if (isModelActive(E_CAP_MODEL_HELMET)) {
			unk10[2]->viewCalc();
		}
		if (isModelActive(E_CAP_MODEL_SUNGLASSES)) {
			unk10[3]->viewCalc();
		}
	}

	if ((param_1 & 0x200) != 0) {
		unkC->entry();
		if (isModelActive(2)) {
			unk10[2]->entry();
		}
		if (isModelActive(4)) {
			unk10[3]->entry();
		}
	}

	if ((param_1 & 0x10000000) != 0 && isModelActive(E_CAP_MODEL_HAT)) {
		unk30->movement();
	}
}

void TMarioCap::mtxEffectHide()
{
	unk20->flagOff();
	unk24->flagOff();
}

void TMarioCap::mtxEffectShow()
{
	unk20->flagOn();
	unk24->flagOn();
}

void TMarioCap::addDirty() { }

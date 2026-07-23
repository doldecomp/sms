#include <Player/MarioCap.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <M3DUtil/M3UModelMario.hpp>
#include <MarioUtil/DrawUtil.hpp>
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
	    JKRFileLoader::getGlbResource("/mario/bmd/ma_cap1.bmd"),
	    J3DMLF_MaterialPEFull | (16 << J3DMLF_TevStageNumShift));
	// Might be an inlined function?
	maCap1ModelData->getTexture()->setResTIMG(
	    0,
	    *mMario->mModel->getModel()->getModelData()->getTexture()->getResTIMG(
	        0));
	DCFlushRange(maCap1ModelData->getTexture()->mResources, 0x20);
	unk10[0] = new J3DModel(maCap1ModelData, 0, 1);

	J3DModelData* maCap3ModelData = J3DModelLoaderDataBase::load(
	    JKRFileLoader::getGlbResource("/mario/bmd/ma_cap3.bmd"),
	    J3DMLF_MaterialPEFull | (16 << J3DMLF_TevStageNumShift));
	// I could see this being an inlined
	maCap3ModelData->getTexture()->setResTIMG(
	    0,
	    *mMario->mModel->getModel()->getModelData()->getTexture()->getResTIMG(
	        0));
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
	    JKRFileLoader::getGlbResource("/mario/watergun2/body/diver_helm.bmd"),
	    J3DMLF_MaterialPEFull | (16 << J3DMLF_TevStageNumShift));
	unk10[2] = new J3DModel(diverHelmModelData, 0, 1);

	J3DModelData* maGlass1 = J3DModelLoaderDataBase::load(
	    JKRFileLoader::getGlbResource("/mario/bmd/ma_glass1.bmd"),
	    J3DMLF_MaterialPEFull | (16 << J3DMLF_TevStageNumShift));
	unk10[3] = new J3DModel(maGlass1, 0, 1);

	// Mmmh, nintendo plz? I hope this is forgotten and not a check to crash the
	// game if it is missing this bone
	unk10[2]->getModelData()->getJointName()->getIndex("null_airtube");
	MtxPtr mtx = mMario->mModel->getModel()->getAnmMtx(mMario->mJointIdMHead);

	unk10[0]->setBaseTRMtx(mtx);
	unk10[0]->calc();
	unk10[1]->setBaseTRMtx(mtx);
	unk10[1]->calc();
	mMario->mModel->getModel()->setAnmMtx(mMario->mJointIdHead,
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
	unk4 = E_CAP_MODEL_HAT;

	unkC = unk10[0];

	int thingIdx = 0;
	unk30        = new TTrembleModelEffect;
	unk30->init(unk10[thingIdx]);
	unk34 = 4.0f;

	for (int idx = 0; idx < 2; idx++) {
		for (u16 matIdx = 0;
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

// UNUSED
void TMarioCap::addDirty() { }

void TMarioCap::perform(unsigned long param_1, JDrama::TGraphics* param_2)
{
	// Unused stack space
	// volatile u32 padding[42];

	if ((param_1 & 2) != 0) {
		if (mMario->mAnimationId == TMario::ANIM_DEMO_GATE_OUT_GET2) {
			J3DFrameCtrl& frameCtrl = mMario->getMotionFrameCtrl();
			if (frameCtrl.getFrame() < 157.0f) {
				unkC = unk10[1];
				unk10[0]->getModelData()->onFlag1OnAllShapes();
				unk10[1]->getModelData()->offFlag1OnAllShapes();
			} else {
				unkC = unk10[0];
				unk10[0]->getModelData()->offFlag1OnAllShapes();
				unk10[1]->getModelData()->onFlag1OnAllShapes();
			}
		} else if (isModelActive(E_CAP_MODEL_HAT)) {
			unkC = unk10[0];
			unk10[0]->getModelData()->offFlag1OnAllShapes();
			unk10[1]->getModelData()->onFlag1OnAllShapes();

			bool doTremble = false;

			// Missing a copy of TVec3, i still suspect that operations should
			// do a copy
			f32 distance
			    = JGeometry::TVec3<f32>(mMario->mPosition - mMario->unk29C)
			          .length();
			if (mMario->mStatus == MARIO_STATUS_SURF && distance > 20.0f) {
				doTremble = true;
			}
			if (mMario->mStatus == 0x281089A) {
				doTremble = true;
			}
			if (mMario->mStatus == 0x81089B) {
				doTremble = true;
			}

			if (mMario->checkStatusType(MARIO_STATUS_FLAG_JUMPING)
			    && distance > 20.0f) {
				doTremble = true;
			}
			if (mMario->mStatus == MARIO_STATUS_DIVE) {
				doTremble = false;
			}

			if (doTremble == true) {
				unk30->clash(unk34);
			} else {
				unk30->clash(0.0f);
			}
		} else {

			unkC = unk10[1];
			unk10[0]->getModelData()->onFlag1OnAllShapes();
			unk10[1]->getModelData()->offFlag1OnAllShapes();
		}

		if (mMario->checkFlag(MARIO_FLAG_HELMET_FLW_CAMERA)) {
			setModelActive(E_CAP_MODEL_HELMET);
		}

		if (isModelActive(E_CAP_MODEL_HELMET))
			unk10[2]->getModelData()->offFlag1OnAllShapes();
		else
			unk10[2]->getModelData()->onFlag1OnAllShapes();

		if (isModelActive(E_CAP_MODEL_SUNGLASSES)) {
			unk10[3]->calc();
		}
		unkC->calc();
		unk10[2]->calc();

		for (u16 i = 0; i < unkC->getModelData()->getMaterialNum(); i++) {
			J3DGXColor* color
			    = unkC->getModelData()->getMaterialNodePointer(i)->getTevKColor(
			        0);

			color->color.a = mMario->mDirty;
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

void TMarioCap::mtxEffectShow()
{
	unk20->flagOn(0x1);
	unk24->flagOn(0x1);
}

void TMarioCap::mtxEffectHide()
{
	unk20->flagOff(0x1);
	unk24->flagOff(0x1);
}

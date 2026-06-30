#include <Player/MarioEffect.hpp>
#include <Player/Mario.hpp>
#include <Player/WaterGun.hpp>
#include <Enemy/Conductor.hpp>
#include <System/Application.hpp>
#include <System/EmitterViewObj.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DPacket.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

static const char cDirtyFileName[] = "/scene/map/pollution/H_ma_rak.bti";
static const char cDirtyTexName[]  = "H_ma_rak_dummy";

void TMarioEffect::init(TMario* mario)
{
	unk68    = mario;
	unk6C[0] = 0;
	unk6C[1] = 0;

	MActorAnmData* anmDataTobikomi = new MActorAnmData;
	anmDataTobikomi->init("mario/04_tobikomi", nullptr);

	for (int i = 0; i < 2; ++i)
		unk74[i] = new MActor(anmDataTobikomi);

	void* tobikomiBmd
	    = JKRFileLoader::getGlbResource("/mario/04_tobikomi/04_tobikomi.bmd");
	u32 flag = J3DMLF_MaterialPEFull | (4 << J3DMLF_TevStageNumShift);
	for (int i = 0; i < 2; ++i) {
		J3DModel* model = new J3DModel(
		    J3DModelLoaderDataBase::load(tobikomiBmd, flag), 0, 1);
		unk74[i]->setModel(model, 0);
	}

	MActorAnmData* anmDataWaterboost = new MActorAnmData;
	anmDataWaterboost->init("mario/01_waterboost", nullptr);
	unk7C = 0;

	unk80 = new MActor(anmDataWaterboost);

	void* waterboostBmd = JKRFileLoader::getGlbResource(
	    "/mario/01_waterboost/01_waterboost.bmd");
	J3DModel* waterboostModel
	    = new J3DModel(J3DModelLoaderDataBase::load(
	                       waterboostBmd, J3DMLF_MaterialPEFull
	                                          | (4 << J3DMLF_TevStageNumShift)),
	                   0, 1);
	unk80->setModel(waterboostModel, 0);
	unk80->setBck("01_waterboost_in");
	unk80->setBtk("01_waterboost");
	unk80->getFrameCtrl(0)->setRate(SMSGetAnmFrameRate());
	unk80->getFrameCtrl(4)->setRate(SMSGetAnmFrameRate());

	gpConductor->registerOtherObj(this);
}

void TMarioEffect::setJumpIntoWaterEffect()
{
	f32 absVelY = unk68->mVel.y;
	if (absVelY < 0.0f)
		absVelY = -absVelY;

	if (absVelY < unk68->mWaterEffectParams.mJumpIntoMdlEffectSpY.get())
		return;

	if (unk68->mFloorPosition.z - unk68->mFloorPosition.y < 50.0f)
		return;

	int idx = getThing();
	if (idx < 0)
		return;

	Mtx localMtx;
	MTXCopy(unk68->unk220, localMtx);

	f32 minY = unk68->mWaterEffectParams.mJumpIntoMinY.get();
	f32 maxY = unk68->mWaterEffectParams.mJumpIntoMaxY.get();

	f32 ratio;
	if (absVelY < minY)
		ratio = 0.0f;
	if (absVelY > maxY)
		ratio = 1.0f;
	if (minY <= absVelY && absVelY <= maxY)
		ratio = (absVelY - minY) / (maxY - minY);

	f32 scale = ratio * unk68->mWaterEffectParams.mJumpIntoScaleWidth.get()
	            + unk68->mWaterEffectParams.mJumpIntoScaleMin.get();
	localMtx[0][0] = scale;
	localMtx[1][1] = scale;
	localMtx[2][2] = scale;

	unk74[idx]->setBck("04_tobikomi");
	unk74[idx]->setBpk("04_tobikomi");
	unk74[idx]->setBtk("04_tobikomi");
	unk74[idx]->setBrk("04_tobikomi");

	unk74[idx]->getFrameCtrl(0)->setRate(SMSGetAnmFrameRate());
	unk74[idx]->getFrameCtrl(2)->setRate(SMSGetAnmFrameRate());
	unk74[idx]->getFrameCtrl(4)->setRate(SMSGetAnmFrameRate());
	unk74[idx]->getFrameCtrl(5)->setRate(SMSGetAnmFrameRate());

	unk74[idx]->getModel()->setBaseTRMtx(localMtx);

	unk74[idx]->getModel()->getShapePacket(0)->show();
	unk74[idx]->getModel()->getShapePacket(1)->show();
	unk74[idx]->getModel()->getShapePacket(2)->show();
	unk74[idx]->getModel()->getShapePacket(3)->show();
	unk74[idx]->getModel()->getShapePacket(4)->show();

	unk6C[idx] = 1;
}

void TMarioEffect::setJumpIntoWaterEffectSmall()
{
	int idx = getThing();
	if (idx < 0)
		return;

	Mtx localMtx;
	Mtx scaleMtx;
	MTXCopy(unk68->unk220, localMtx);
	MTXScale(scaleMtx, 0.8f, 0.4f, 0.8f);
	MTXConcat(localMtx, scaleMtx, localMtx);

	unk74[idx]->setBck("04_tobikomi");
	unk74[idx]->setBpk("04_tobikomi");
	unk74[idx]->setBtk("04_tobikomi");
	unk74[idx]->setBrk("04_tobikomi");

	unk74[idx]->getFrameCtrl(0)->setRate(SMSGetAnmFrameRate());
	unk74[idx]->getFrameCtrl(2)->setRate(SMSGetAnmFrameRate());
	unk74[idx]->getFrameCtrl(4)->setRate(SMSGetAnmFrameRate());
	unk74[idx]->getFrameCtrl(5)->setRate(SMSGetAnmFrameRate());

	unk74[idx]->getModel()->setBaseTRMtx(localMtx);

	unk74[idx]->getModel()->getShapePacket(1)->hide();
	unk74[idx]->getModel()->getShapePacket(2)->hide();
	unk74[idx]->getModel()->getShapePacket(4)->hide();

	unk6C[idx] = 1;
}

int TMarioEffect::getJumpIntoWaterModelData() { }

void TMarioEffect::startDashEffect()
{
	unk80->setBck("01_waterboost_in");
	unk80->setBtk("01_waterboost");
	unk80->getFrameCtrl(0)->setFrame(0.0f);
	unk80->getFrameCtrl(4)->setFrame(0.0f);
}

void TMarioEffect::endDashEffect()
{
	unk80->setBck("01_waterboost_out");
	unk80->getFrameCtrl(0)->setFrame(0.0f);
}

void TMarioEffect::perform(u32 param_1, JDrama::TGraphics* gfx)
{
	if (param_1 & 1) {
		switch (unk7C) {
		case 0:
			if (unk68->checkFlag(MARIO_FLAG_FLUDD_EMITTING)) {
				startDashEffect();
				unk7C = 1;
			}
			break;

		case 1:
			if (unk68->checkFlag(MARIO_FLAG_FLUDD_EMITTING) == true) {
				if (unk68->mWaterGun->getEmitMtx(0) != nullptr) {
					gpMarioParticleManager->emitAndBindToMtxPtr(
					    0xFE, unk68->mWaterGun->getEmitMtx(0), 1, this);
					gpMarioParticleManager->emitAndBindToMtxPtr(
					    0xFF, unk68->mWaterGun->getEmitMtx(0), 1, this);
				}
			} else {
				endDashEffect();
				unk7C = 2;
			}
			break;

		case 2:
			if (unk80->getFrameCtrl(0)->checkState(
			        J3DFrameCtrl::STATE_COMPLETED_ONCE
			        | J3DFrameCtrl::STATE_LOOPED_ONCE))
				unk7C = 0;
			break;
		}
	}

	if ((param_1 & 0x2) && unk7C != 0) {
		if (unk68->mWaterGun->getEmitMtx(0) != nullptr) {
			unk80->getModel()->setBaseTRMtx(unk68->mWaterGun->getEmitMtx(0));
			unk80->perform(0x2, gfx);
		}
	}

	if ((param_1 & 0x4) && unk7C != 0)
		unk80->perform(0x4, gfx);

	if ((param_1 & 0x200) && unk7C != 0)
		unk80->perform(0x200, gfx);

	for (int i = 0; i < 2; ++i) {
		if (unk6C[i] == 1) {
			unk74[i]->perform(param_1, gfx);
			if (unk74[i]->getFrameCtrl(0)->checkState(
			        J3DFrameCtrl::STATE_COMPLETED_ONCE
			        | J3DFrameCtrl::STATE_LOOPED_ONCE))
				unk6C[i] = 0;
		}
	}
}

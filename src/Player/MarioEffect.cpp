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

#include <Player/MarioDirtyStrings.hpp>

// TODO: init is the last non-exact function: a one-step rotation of the
// callee-saved GPRs (retail `this` r31, loop offset r30, `.rodata` base r29;
// ours base r31, `this` r30, offset r29) plus one `addi r4, r25, 0` where we
// emit `mr r4, r25` before the second setModel. The frame is exact. Tried and
// inert on the rotation: one shared `anmData`/`bmd`/`model` variable (closure
// batch 152), an unnamed `flag` (-8 frame), an unnamed waterboost model, a
// split `model` declaration, a function-scope `int i` (one or two counters),
// a named string local, a TU-local `setModel(MActor*, J3DModel*)` level, and
// each half of the body as a TU-local helper taking `this`. Both
// setJumpIntoWaterEffect* closed by open-coding getThing()'s index (the header
// inline's own named `idx` ranked above the base) plus MarioEffectMario
// binders.
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
	unk80->getFrameCtrl(ANM_TYPE_BCK)->setRate(SMSGetAnmFrameRate());
	unk80->getFrameCtrl(ANM_TYPE_BTK)->setRate(SMSGetAnmFrameRate());

	gpConductor->registerOtherObj(this);
}

// fabricated
static inline TMario* MarioEffectMario(TMarioEffect* p)
{
	TMario* mario = p->unk68;
	return mario;
}

void TMarioEffect::setJumpIntoWaterEffect()
{
	f32 velY = MarioEffectMario(this)->mVel.y;
	f32 absVelY = velY;
	if (velY < 0.0f)
		absVelY = -velY;

	if (absVelY < MarioEffectMario(this)->mWaterEffectParams.mJumpIntoMdlEffectSpY.get())
		return;

	if (MarioEffectMario(this)->mFloorPosition.z - MarioEffectMario(this)->mFloorPosition.y < 50.0f)
		return;

	int idx = -1;
	if (unk6C[0] == 0)
		idx = 0;
	if (unk6C[1] == 0)
		idx = 1;
	if (idx < 0)
		return;

	Mtx localMtx;
	MTXCopy(MarioEffectMario(this)->unk220, localMtx);

	f32 minY = MarioEffectMario(this)->mWaterEffectParams.mJumpIntoMinY.get();
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

	unk74[idx]->getFrameCtrl(ANM_TYPE_BCK)->setRate(SMSGetAnmFrameRate());
	unk74[idx]->getFrameCtrl(ANM_TYPE_BPK)->setRate(SMSGetAnmFrameRate());
	unk74[idx]->getFrameCtrl(ANM_TYPE_BTK)->setRate(SMSGetAnmFrameRate());
	unk74[idx]->getFrameCtrl(ANM_TYPE_BRK)->setRate(SMSGetAnmFrameRate());

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
	int idx = -1;
	if (unk6C[0] == 0)
		idx = 0;
	if (unk6C[1] == 0)
		idx = 1;
	if (idx < 0)
		return;

	Mtx localMtx;
	Mtx scaleMtx;
	MTXCopy(MarioEffectMario(this)->unk220, localMtx);
	MTXScale(scaleMtx, 0.8f, 0.4f, 0.8f);
	MTXConcat(localMtx, scaleMtx, localMtx);

	unk74[idx]->setBck("04_tobikomi");
	unk74[idx]->setBpk("04_tobikomi");
	unk74[idx]->setBtk("04_tobikomi");
	unk74[idx]->setBrk("04_tobikomi");

	unk74[idx]->getFrameCtrl(ANM_TYPE_BCK)->setRate(SMSGetAnmFrameRate());
	unk74[idx]->getFrameCtrl(ANM_TYPE_BPK)->setRate(SMSGetAnmFrameRate());
	unk74[idx]->getFrameCtrl(ANM_TYPE_BTK)->setRate(SMSGetAnmFrameRate());
	unk74[idx]->getFrameCtrl(ANM_TYPE_BRK)->setRate(SMSGetAnmFrameRate());

	unk74[idx]->getModel()->setBaseTRMtx(localMtx);

	unk74[idx]->getModel()->getShapePacket(1)->hide();
	unk74[idx]->getModel()->getShapePacket(2)->hide();
	unk74[idx]->getModel()->getShapePacket(4)->hide();

	unk6C[idx] = 1;
}

// UNUSED (map 0x10): four instructions, i.e. three chained loads and a `blr`.
// unk74[0] (+0x74), MActor::getModel() (+0x4) and J3DModel::getModelData()
// (+0x0) are exactly that chain and its size.
J3DModelData* TMarioEffect::getJumpIntoWaterModelData()
{
	return unk74[0]->getModel()->getModelData();
}

void TMarioEffect::startDashEffect()
{
	unk80->setBck("01_waterboost_in");
	unk80->setBtk("01_waterboost");
	unk80->getFrameCtrl(ANM_TYPE_BCK)->setFrame(0.0f);
	unk80->getFrameCtrl(ANM_TYPE_BTK)->setFrame(0.0f);
}

void TMarioEffect::endDashEffect()
{
	unk80->setBck("01_waterboost_out");
	unk80->getFrameCtrl(ANM_TYPE_BCK)->setFrame(0.0f);
}

// Closure batch 226: perform references no stack slot of its own, and its
// 0x30 of low region is six binding levels -- one per `unk68->mWaterGun` read
// (five) plus one more. The sixth is +8 wherever it sits: the case-0
// `checkFlag` receiver below, any single `unk80` receiver, or the second
// `checkFlag`; taking two of them overshoots by 8. Kept on the mario read
// because the gun binder already reads through it.
// fabricated
static inline TMario* PerformMario(const TMarioEffect* p)
{
	TMario* m = p->unk68;
	return m;
}

static inline TWaterGun* PerformGun(const TMarioEffect* p)
{
	TWaterGun* g = p->unk68->mWaterGun;
	return g;
}

static inline MActor* PerformActor(const TMarioEffect* p)
{
	MActor* a = p->unk80;
	return a;
}

void TMarioEffect::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		switch (unk7C) {
		case 0:
			if (PerformMario(this)->checkFlag(MARIO_FLAG_FLUDD_EMITTING)) {
				startDashEffect();
				unk7C = 1;
			}
			break;

		case 1:
			if (unk68->checkFlag(MARIO_FLAG_FLUDD_EMITTING) == true) {
				if (PerformGun(this)->getEmitMtx(0) != nullptr) {
					gpMarioParticleManager->emitAndBindToMtxPtr(
					    0xFE, PerformGun(this)->getEmitMtx(0), 1, this);
					gpMarioParticleManager->emitAndBindToMtxPtr(
					    0xFF, PerformGun(this)->getEmitMtx(0), 1, this);
				}
			} else {
				endDashEffect();
				unk7C = 2;
			}
			break;

		case 2:
			if (unk80->getFrameCtrl(ANM_TYPE_BCK)
			        ->checkState(J3DFrameCtrl::STATE_COMPLETED_ONCE
			                     | J3DFrameCtrl::STATE_LOOPED_ONCE))
				unk7C = 0;
			break;
		}
	}

	if ((cue & CUE_CALC_ANIM) && unk7C != 0) {
		if (PerformGun(this)->getEmitMtx(0) != nullptr) {
			unk80->getModel()->setBaseTRMtx(PerformGun(this)->getEmitMtx(0));
			unk80->perform(CUE_CALC_ANIM, graphics);
		}
	}

	if ((cue & CUE_CALC_VIEW) && unk7C != 0)
		unk80->perform(CUE_CALC_VIEW, graphics);

	if ((cue & CUE_ENTRY) && unk7C != 0)
		unk80->perform(CUE_ENTRY, graphics);

	for (int i = 0; i < 2; ++i) {
		if (unk6C[i] == 1) {
			unk74[i]->perform(cue, graphics);
			if (unk74[i]
			        ->getFrameCtrl(ANM_TYPE_BCK)
			        ->checkState(J3DFrameCtrl::STATE_COMPLETED_ONCE
			                     | J3DFrameCtrl::STATE_LOOPED_ONCE))
				unk6C[i] = 0;
		}
	}
}

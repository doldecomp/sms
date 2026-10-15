#include <Enemy/BossHanachan.hpp>
#include <Enemy/BossHanachanChangeSaveParams.hpp>
#include <Enemy/BossHanachanParts.hpp>
#include <M3DUtil/MActor.hpp>
#include <NPC/NpcInbetween.hpp>
#include <Camera/cameralib.hpp>
#include <Strategic/Spine.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <macros.h>

void TBossHanachan::setHeadAndBodyAnm(
    EnumBossHanachanAnmKind param_1,
    EnumBossHanachanStopMotionBlendOnOff param_2)
{
	unk170->setAnm_(param_1, param_2);
	for (int i = 0; i < ARRAY_COUNT(unk150); ++i) {
		TBossHanachanPartsBase* part = unk150[i];
		if (part->setAnm_(param_1, param_2)) {
			J3DFrameCtrl* frameCtrl
			    = part->getMActor()->getFrameCtrl(ANM_TYPE_BCK);
			f32 frame = (i * unk1C0->mSLNormalBckFrameDiff.get())
			            % frameCtrl->getEnd();
			frameCtrl->setFrame(frame);
			J3DFrameCtrl* ctrl = part->getMActor()->getFrameCtrl(ANM_TYPE_BTP);
			if (ctrl != nullptr)
				ctrl->setFrame(frame);
			ctrl = part->getMActor()->getFrameCtrl(ANM_TYPE_BTK);
			if (ctrl != nullptr)
				ctrl->setFrame(frame);
		}
	}
}

void TBossHanachan::setTumbleBckRate_(TBossHanachanPartsBase* param_1)
{
	J3DFrameCtrl* ctrl = param_1->getMActor()->getFrameCtrl(ANM_TYPE_BCK);
	f32 duration       = (1.0f / unk198)
	               * (unk194 - param_1->mRotation.z >= 0
	                      ? unk194 - param_1->mRotation.z
	                      : -(unk194 - param_1->mRotation.z));
	ctrl->setRate((1.0f / duration) * (2.0f * (40.0f * SMSGetAnmFrameRate())));
}

void TBossHanachan::setTumbleAnm(EnumBossHanachanStopMotionBlendOnOff param_1)
{
	EnumBossHanachanAnmKind anm;
	if (unk194 == 179.0f)
		anm = BOSS_HANACHAN_ANM_UNK17;
	else if (unk194 == -179.0f)
		anm = BOSS_HANACHAN_ANM_UNK16;
	else
		return;
	unk170->setAnm_(anm, param_1);
	setTumbleBckRate_(unk170);
	for (int i = 0; i < ARRAY_COUNT(unk150); ++i) {
		unk150[i]->setAnm_(anm, param_1);
		setTumbleBckRate_(unk150[i]);
	}
}

void TBossHanachan::setAnmTimerWhenGetUp()
{
	u8 diff = unk1C0->mSLGetUpFrameDiff.get();
	for (int i = 0; i < ARRAY_COUNT(unk150); ++i)
		unk150[7 - i]->unk10C = diff * i;
	unk170->unk10C = diff * 8;
}

void TBossHanachan::setAnmTimerWhenSnort()
{
	u8 diff        = unk1C0->mSLSnortFrameDiff.get();
	unk170->unk10C = 0;
	for (int i = 0; i < ARRAY_COUNT(unk150); i++)
		unk150[i]->unk10C = diff * (i + 1);
}

void TBossHanachan::setAnmTimerWhenDamage()
{
	u8 diff = unk1C0->mSLDamageFrameDiff.get();
	for (int i = 0; i < ARRAY_COUNT(unk150); i++) {
		s32 frame         = unk174 - i;
		unk150[i]->unk10C = diff * (frame >= 0 ? frame : -frame);
	}
	s32 frame      = unk174 + 1;
	unk170->unk10C = diff * (frame >= 0 ? frame : -frame);
}

void TBossHanachan::setAnmTimerWhenDead()
{
	u8 diff = unk1C0->mSLDeadFrameDiff.get();
	for (int i = 0; i < ARRAY_COUNT(unk150); i++) {
		s32 frame         = unk174 - i;
		unk150[i]->unk10C = diff * (frame >= 0 ? frame : -frame);
	}
	s32 frame      = unk174 + 1;
	unk170->unk10C = diff * (frame >= 0 ? frame : -frame);
}

void TBossHanachan::considerSetAnm(EnumBossHanachanNerveAnm param_1)
{
	unk170->considerSetAnm_(param_1);
	for (int i = 0; i < ARRAY_COUNT(unk150); i++)
		unk150[i]->considerSetAnm_(param_1);
}

bool TBossHanachan::isFinishedGetUp() const
{
	bool result = false;
	switch (unk170->unkF4) {
	case BOSS_HANACHAN_ANM_UNK9:
	case BOSS_HANACHAN_ANM_UNK12:
		if (unk170->isCurBckAlreadyEnd_())
			result = true;
		break;
	}
	return result;
}

bool TBossHanachan::isAllBckAlreadyEnd(EnumBossHanachanAnmKind param_1) const
{
	bool result = true;
	if ((unk170->unkF4 == param_1 && unk170->isCurBckAlreadyEnd_()) == false) {
		result = false;
	} else {
		for (int i = 0; i < ARRAY_COUNT(unk150); i++) {
			if ((unk150[i]->unkF4 == param_1
			     && unk150[i]->isCurBckAlreadyEnd_())
			    == false) {
				result = false;
				break;
			}
		}
	}
	return result;
}

void TBossHanachan::offHeadAndBodyNonstopMotionBlend_()
{
	unk170->offNonstopMotionBlend_();
	for (int i = 0; i < ARRAY_COUNT(unk150); ++i)
		unk150[i]->offNonstopMotionBlend_();
}

void TBossHanachan::setHeadAndBodyNonstopMotionBlendRatio_(f32 param_1)
{
	unk170->setNonstopMotionBlendRatio_(param_1);
	for (int i = 0; i < ARRAY_COUNT(unk150); ++i)
		unk150[i]->setNonstopMotionBlendRatio_(param_1);
}

void TBossHanachan::copyFrameFromOldAnmToNewAnm_()
{
	unk170->copyFrameFromOldAnmToNewAnm_();
	for (int i = 0; i < ARRAY_COUNT(unk150); ++i)
		unk150[i]->copyFrameFromOldAnmToNewAnm_();
}

void TBossHanachan::changeAnmRateAndFrameUpdate_()
{
	bool doRate = true;
	f32 rate    = SMSGetAnmFrameRate();
	if (mSpine->getLatestNerve() == &TNerveBossHanachanTumble::theNerve()) {
		offHeadAndBodyNonstopMotionBlend_();
		unk170->changeTumbleAnmRate_();
		for (int i = 0; i < ARRAY_COUNT(unk150); ++i)
			unk150[i]->changeTumbleAnmRate_();
		doRate = false;
	} else {
		switch (unk170->unkF4) {
		case BOSS_HANACHAN_ANM_UNK0:
		case BOSS_HANACHAN_ANM_UNK1:
			if (mMarchSpeed <= unk1C0->mSLWalkAnmMarchSpeed.get()) {
				offHeadAndBodyNonstopMotionBlend_();
				switch (unk170->unkF4) {
				case BOSS_HANACHAN_ANM_UNK1:
					setHeadAndBodyAnm(BOSS_HANACHAN_ANM_UNK0,
					                  BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
					copyFrameFromOldAnmToNewAnm_();
					break;
				case BOSS_HANACHAN_ANM_UNK0:
					break;
				default:
					setHeadAndBodyAnm(BOSS_HANACHAN_ANM_UNK0,
					                  BOSS_HANACHAN_STOP_MOTION_BLEND_ON);
					break;
				}
			} else if (mMarchSpeed >= unk1C0->mSLRunAnmMarchSpeed.get()) {
				offHeadAndBodyNonstopMotionBlend_();
				switch (unk170->unkF4) {
				case BOSS_HANACHAN_ANM_UNK1:
					break;
				case BOSS_HANACHAN_ANM_UNK0:
					setHeadAndBodyAnm(BOSS_HANACHAN_ANM_UNK1,
					                  BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
					copyFrameFromOldAnmToNewAnm_();
					break;
				default:
					setHeadAndBodyAnm(BOSS_HANACHAN_ANM_UNK1,
					                  BOSS_HANACHAN_STOP_MOTION_BLEND_ON);
					break;
				}
			} else {
				rate = CLBCalcRatio(unk1C0->mSLWalkAnmMarchSpeed.get(),
				                    unk1C0->mSLRunAnmMarchSpeed.get(),
				                    mMarchSpeed);
				switch (unk170->unkF4) {
				case BOSS_HANACHAN_ANM_UNK0:
					if (unk170->unkF8 != BOSS_HANACHAN_ANM_UNK1) {
						setHeadAndBodyAnm(BOSS_HANACHAN_ANM_UNK1,
						                  BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
						copyFrameFromOldAnmToNewAnm_();
						rate = 1.0f - rate;
					}
					setHeadAndBodyNonstopMotionBlendRatio_(rate);
					break;
				case BOSS_HANACHAN_ANM_UNK1:
					if (unk170->unkF8 == BOSS_HANACHAN_ANM_UNK0) {
						rate = 1.0f - rate;
					} else {
						setHeadAndBodyAnm(BOSS_HANACHAN_ANM_UNK0,
						                  BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
						copyFrameFromOldAnmToNewAnm_();
					}
					setHeadAndBodyNonstopMotionBlendRatio_(rate);
					break;
				default:
					offHeadAndBodyNonstopMotionBlend_();
					setHeadAndBodyAnm(BOSS_HANACHAN_ANM_UNK0,
					                  BOSS_HANACHAN_STOP_MOTION_BLEND_ON);
					break;
				}
			}
			rate = mMarchSpeed * SMSGetAnmFrameRate()
			       * unk1C0->mSLWalkBckRateMagnif.get();
			if (rate < unk1C0->mSLWalkBckRateMin.get())
				rate = unk1C0->mSLWalkBckRateMin.get();
			break;
		default:
			offHeadAndBodyNonstopMotionBlend_();
			rate = SMSGetAnmFrameRate();
			break;
		}
	}
	MActor* mactor = unk170->getMActor();
	if (doRate)
		mactor->getFrameCtrl(ANM_TYPE_BCK)->setRate(rate);
	unk170->updateAnmSound();
	mactor->frameUpdate();
	for (int i = 0; i < ARRAY_COUNT(unk150); ++i) {
		MActor* loopMactor = unk150[i]->getMActor();
		if (doRate)
			loopMactor->getFrameCtrl(ANM_TYPE_BCK)->setRate(rate);
		unk150[i]->updateAnmSound();
		loopMactor->frameUpdate();
	}
}

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
	mHead->setAnm_(param_1, param_2);
	for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i) {
		TBossHanachanPartsBase* part = mBodyParts[i];
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
	mHead->setAnm_(anm, param_1);
	setTumbleBckRate_(mHead);
	for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i) {
		mBodyParts[i]->setAnm_(anm, param_1);
		setTumbleBckRate_(mBodyParts[i]);
	}
}

void TBossHanachan::setAnmTimerWhenGetUp()
{
	u8 diff = unk1C0->mSLGetUpFrameDiff.get();
	for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i)
		mBodyParts[7 - i]->unk10C = diff * i;
	mHead->unk10C = diff * 8;
}

void TBossHanachan::setAnmTimerWhenSnort()
{
	u8 diff       = unk1C0->mSLSnortFrameDiff.get();
	mHead->unk10C = 0;
	for (int i = 0; i < ARRAY_COUNT(mBodyParts); i++)
		mBodyParts[i]->unk10C = diff * (i + 1);
}

void TBossHanachan::setAnmTimerWhenDamage()
{
	u8 diff = unk1C0->mSLDamageFrameDiff.get();
	for (int i = 0; i < ARRAY_COUNT(mBodyParts); i++) {
		s32 frame             = unk174 - i;
		mBodyParts[i]->unk10C = diff * (frame >= 0 ? frame : -frame);
	}
	s32 frame     = unk174 + 1;
	mHead->unk10C = diff * (frame >= 0 ? frame : -frame);
}

void TBossHanachan::setAnmTimerWhenDead()
{
	u8 diff = unk1C0->mSLDeadFrameDiff.get();
	for (int i = 0; i < ARRAY_COUNT(mBodyParts); i++) {
		s32 frame             = unk174 - i;
		mBodyParts[i]->unk10C = diff * (frame >= 0 ? frame : -frame);
	}
	s32 frame     = unk174 + 1;
	mHead->unk10C = diff * (frame >= 0 ? frame : -frame);
}

void TBossHanachan::considerSetAnm(EnumBossHanachanNerveAnm param_1)
{
	mHead->considerSetAnm_(param_1);
	for (int i = 0; i < ARRAY_COUNT(mBodyParts); i++)
		mBodyParts[i]->considerSetAnm_(param_1);
}

bool TBossHanachan::isFinishedGetUp() const
{
	bool result = false;
	switch (mHead->mCurAnm) {
	case BOSS_HANACHAN_ANM_UNK9:
	case BOSS_HANACHAN_ANM_UNK12:
		if (mHead->isCurBckAlreadyEnd_())
			result = true;
		break;
	}
	return result;
}

bool TBossHanachan::isAllBckAlreadyEnd(EnumBossHanachanAnmKind param_1) const
{
	bool result = true;
	if ((mHead->mCurAnm == param_1 && mHead->isCurBckAlreadyEnd_()) == false) {
		result = false;
	} else {
		for (int i = 0; i < ARRAY_COUNT(mBodyParts); i++) {
			if ((mBodyParts[i]->mCurAnm == param_1
			     && mBodyParts[i]->isCurBckAlreadyEnd_())
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
	mHead->unk110->mForcedBlendRatio = 0.0f;
	for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i)
		mBodyParts[i]->unk110->mForcedBlendRatio = 0.0f;
}

void TBossHanachan::setHeadAndBodyNonstopMotionBlendRatio_(f32 param_1)
{
	mHead->unk110->mForcedBlendRatio = param_1;
	for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i)
		mBodyParts[i]->unk110->mForcedBlendRatio = param_1;
}

void TBossHanachan::copyFrameFromOldAnmToNewAnm_()
{
	mHead->copyFrameFromOldAnmToNewAnm_();
	for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i)
		mBodyParts[i]->copyFrameFromOldAnmToNewAnm_();
}

void TBossHanachan::changeAnmRateAndFrameUpdate_()
{
	bool doRate = true;
	f32 rate    = SMSGetAnmFrameRate();
	if (mSpine->getLatestNerve() == &TNerveBossHanachanTumble::theNerve()) {
		offHeadAndBodyNonstopMotionBlend_();
		mHead->changeTumbleAnmRate_();
		for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i)
			mBodyParts[i]->changeTumbleAnmRate_();
		doRate = false;
	} else {
		switch (mHead->mCurAnm) {
		case BOSS_HANACHAN_ANM_UNK0:
		case BOSS_HANACHAN_ANM_UNK1:
			if (mMarchSpeed <= unk1C0->mSLWalkAnmMarchSpeed.get()) {
				offHeadAndBodyNonstopMotionBlend_();
				switch (mHead->mCurAnm) {
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
				switch (mHead->mCurAnm) {
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
				switch (mHead->mCurAnm) {
				case BOSS_HANACHAN_ANM_UNK0:
					if (mHead->mPrevAnm != BOSS_HANACHAN_ANM_UNK1) {
						setHeadAndBodyAnm(BOSS_HANACHAN_ANM_UNK1,
						                  BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
						copyFrameFromOldAnmToNewAnm_();
						rate = 1.0f - rate;
					}
					setHeadAndBodyNonstopMotionBlendRatio_(rate);
					break;
				case BOSS_HANACHAN_ANM_UNK1:
					if (mHead->mPrevAnm == BOSS_HANACHAN_ANM_UNK0) {
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
	MActor* mactor = mHead->getMActor();
	if (doRate)
		mactor->getFrameCtrl(ANM_TYPE_BCK)->setRate(rate);
	mHead->updateAnmSound();
	mactor->frameUpdate();
	for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i) {
		MActor* loopMactor = mBodyParts[i]->getMActor();
		if (doRate)
			loopMactor->getFrameCtrl(ANM_TYPE_BCK)->setRate(rate);
		mBodyParts[i]->updateAnmSound();
		loopMactor->frameUpdate();
	}
}

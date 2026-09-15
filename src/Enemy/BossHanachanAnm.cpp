#include <Enemy/BossHanachan.hpp>
#include <Camera/cameralib.hpp>
#include <M3DUtil/MActor.hpp>
#include <NPC/NpcInbetween.hpp>
#include <Strategic/Spine.hpp>

void TBossHanachan::setHeadAndBodyAnm(
    EnumBossHanachanAnmKind anm, EnumBossHanachanStopMotionBlendOnOff blend)
{
	mHead->setAnm_(anm, blend);
	for (int i = 0; i < 8; ++i) {
		TBossHanachanPartsBody* body = mBodies[i];
		if (body->setAnm_(anm, blend)) {
			J3DFrameCtrl* bck = body->mMActor->getFrameCtrl(ANM_TYPE_BCK);
			int frame = (i * mChangeParams->mSLNormalBckFrameDiff.get())
			            % bck->getEnd();
			bck->setFrame(frame);
			f32 texFrame = frame;
			J3DFrameCtrl* btp = body->mMActor->getFrameCtrl(ANM_TYPE_BTP);
			if (btp)
				btp->setFrame(texFrame);
			J3DFrameCtrl* btk = body->mMActor->getFrameCtrl(ANM_TYPE_BTK);
			if (btk)
				btk->setFrame(texFrame);
		}
	}
}

void TBossHanachan::setTumbleBckRate_(TBossHanachanPartsBase* part)
{
	J3DFrameCtrl* ctrl = part->mMActor->getFrameCtrl(ANM_TYPE_BCK);
	f32 frames = unk194 - part->mRotation.z;
	frames = (1.0f / unk198) * (frames >= 0.0f ? frames : -frames);
	ctrl->setRate((1.0f / frames) * (2.0f * (40.0f * SMSGetAnmFrameRate())));
}

void TBossHanachan::setTumbleAnm(EnumBossHanachanStopMotionBlendOnOff blend)
{
	EnumBossHanachanAnmKind anm;
	if (unk194 == 179.0f)
		anm = BOSS_HANACHAN_ANM_UNK17;
	else if (unk194 == -179.0f)
		anm = BOSS_HANACHAN_ANM_UNK16;
	else
		return;
	mHead->setAnm_(anm, blend);
	setTumbleBckRate_(mHead);
	for (int i = 0; i < 8; ++i) {
		mBodies[i]->setAnm_(anm, blend);
		setTumbleBckRate_(mBodies[i]);
	}
}

void TBossHanachan::setAnmTimerWhenGetUp()
{
	u8 delay = mChangeParams->mSLGetUpFrameDiff.get();
	for (int i = 0; i < 8; ++i)
		mBodies[7 - i]->unk10C = delay * i;
	mHead->unk10C = delay * 8;
}

void TBossHanachan::setAnmTimerWhenSnort()
{
	u8 delay = mChangeParams->mSLSnortFrameDiff.get();
	mHead->unk10C = 0;
	for (int i = 0; i < 8; ++i)
		mBodies[i]->unk10C = delay * (i + 1);
}

void TBossHanachan::setAnmTimerWhenDamage()
{
	u8 delay = mChangeParams->mSLDamageFrameDiff.get();
	for (int i = 0; i < 8; ++i)
		mBodies[i]->unk10C = delay * CLBAbs(mWeakBodyIndex - i);
	mHead->unk10C = delay * CLBAbs(mWeakBodyIndex + 1);
}

void TBossHanachan::setAnmTimerWhenDead()
{
	u8 delay = mChangeParams->mSLDeadFrameDiff.get();
	for (int i = 0; i < 8; ++i)
		mBodies[i]->unk10C = delay * CLBAbs(mWeakBodyIndex - i);
	mHead->unk10C = delay * CLBAbs(mWeakBodyIndex + 1);
}

void TBossHanachan::considerSetAnm(EnumBossHanachanNerveAnm nerve)
{
	mHead->considerSetAnm_(nerve);
	for (int i = 0; i < 8; ++i)
		mBodies[i]->considerSetAnm_(nerve);
}

bool TBossHanachan::isFinishedGetUp() const
{
	bool result = false;
	switch (mHead->mCurrentAnm) {
	case BOSS_HANACHAN_ANM_UNK9:
	case BOSS_HANACHAN_ANM_UNK12:
		if (mHead->isCurBckAlreadyEnd_())
			result = true;
		break;
	}
	return result;
}

bool TBossHanachan::isAllBckAlreadyEnd(EnumBossHanachanAnmKind anm) const
{
	bool result = true;
	if ((mHead->mCurrentAnm == anm && mHead->isCurBckAlreadyEnd_()) == false) {
		result = false;
	} else {
		for (int i = 0; i < 8; ++i) {
			if ((mBodies[i]->mCurrentAnm == anm
			     && mBodies[i]->isCurBckAlreadyEnd_()) == false) {
				result = false;
				break;
			}
		}
	}
	return result;
}

void TBossHanachan::offHeadAndBodyNonstopMotionBlend_()
{
	mHead->mInbetween->mForcedBlendRatio = 0.0f;
	for (int i = 0; i < 8; ++i)
		mBodies[i]->mInbetween->mForcedBlendRatio = 0.0f;
}

void TBossHanachan::setHeadAndBodyNonstopMotionBlendRatio_(f32 ratio)
{
	mHead->mInbetween->mForcedBlendRatio = ratio;
	for (int i = 0; i < 8; ++i)
		mBodies[i]->mInbetween->mForcedBlendRatio = ratio;
}

void TBossHanachan::copyFrameFromOldAnmToNewAnm_()
{
	mHead->copyFrameFromOldAnmToNewAnm_();
	for (int i = 0; i < 8; ++i)
		mBodies[i]->copyFrameFromOldAnmToNewAnm_();
}

void TBossHanachan::changeAnmRateAndFrameUpdate_()
{
	bool changeRate = true;
	f32 rate = SMSGetAnmFrameRate();
	if (mSpine->getLatestNerve() == &TNerveBossHanachanTumble::theNerve()) {
		offHeadAndBodyNonstopMotionBlend_();
		mHead->changeTumbleAnmRate_();
		for (int i = 0; i < 8; ++i)
			mBodies[i]->changeTumbleAnmRate_();
		changeRate = false;
	} else {
		switch (mHead->mCurrentAnm) {
		case BOSS_HANACHAN_ANM_UNK0:
		case BOSS_HANACHAN_ANM_UNK1:
			if (mMarchSpeed <= mChangeParams->mSLWalkAnmMarchSpeed.get()) {
				offHeadAndBodyNonstopMotionBlend_();
				switch (mHead->mCurrentAnm) {
				case BOSS_HANACHAN_ANM_UNK0:
					break;
				case BOSS_HANACHAN_ANM_UNK1:
					setHeadAndBodyAnm(BOSS_HANACHAN_ANM_UNK0,
					                  BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
					copyFrameFromOldAnmToNewAnm_();
					break;
				default:
					setHeadAndBodyAnm(BOSS_HANACHAN_ANM_UNK0,
					                  BOSS_HANACHAN_STOP_MOTION_BLEND_ON);
					break;
				}
			} else if (mMarchSpeed >= mChangeParams->mSLRunAnmMarchSpeed.get()) {
				offHeadAndBodyNonstopMotionBlend_();
				switch (mHead->mCurrentAnm) {
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
				f32 ratio = CLBCalcRatio(mChangeParams->mSLWalkAnmMarchSpeed.get(),
				                        mChangeParams->mSLRunAnmMarchSpeed.get(),
				                        mMarchSpeed);
				switch (mHead->mCurrentAnm) {
				case BOSS_HANACHAN_ANM_UNK0:
					if (mHead->mPreviousAnm != BOSS_HANACHAN_ANM_UNK1) {
						setHeadAndBodyAnm(BOSS_HANACHAN_ANM_UNK1,
						                  BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
						copyFrameFromOldAnmToNewAnm_();
						ratio = 1.0f - ratio;
					}
					setHeadAndBodyNonstopMotionBlendRatio_(ratio);
					break;
				case BOSS_HANACHAN_ANM_UNK1:
					if (mHead->mPreviousAnm == BOSS_HANACHAN_ANM_UNK0) {
						ratio = 1.0f - ratio;
					} else {
						setHeadAndBodyAnm(BOSS_HANACHAN_ANM_UNK0,
						                  BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
						copyFrameFromOldAnmToNewAnm_();
					}
					setHeadAndBodyNonstopMotionBlendRatio_(ratio);
					break;
				default:
					offHeadAndBodyNonstopMotionBlend_();
					setHeadAndBodyAnm(BOSS_HANACHAN_ANM_UNK0,
					                  BOSS_HANACHAN_STOP_MOTION_BLEND_ON);
					break;
				}
			}
			rate = mMarchSpeed * SMSGetAnmFrameRate()
			       * mChangeParams->mSLWalkBckRateMagnif.get();
			if (rate < mChangeParams->mSLWalkBckRateMin.get())
				rate = mChangeParams->mSLWalkBckRateMin.get();
			break;
		default:
			offHeadAndBodyNonstopMotionBlend_();
			rate = SMSGetAnmFrameRate();
			break;
		}
	}
	MActor* actor = mHead->mMActor;
	if (changeRate)
		actor->getFrameCtrl(ANM_TYPE_BCK)->setRate(rate);
	mHead->updateAnmSound();
	actor->frameUpdate();
	for (int i = 0; i < 8; ++i) {
		MActor* actor = mBodies[i]->mMActor;
		if (changeRate)
			actor->getFrameCtrl(ANM_TYPE_BCK)->setRate(rate);
		mBodies[i]->updateAnmSound();
		actor->frameUpdate();
	}
}

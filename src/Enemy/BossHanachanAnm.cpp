#include <Enemy/BossHanachan.hpp>

#include <Camera/cameralib.hpp>
#include <M3DUtil/MActor.hpp>
#include <NPC/NpcInbetween.hpp>
#include <Strategic/Spine.hpp>

// TODO: 91.9%. Frame, slots and instruction stream are all right; what is left
// is a two-step rotation of the callee-saved set (retail const/offset/this/
// kind/blend/body/i in r25-r31, ours this/kind/blend/body/i/const/offset) and
// the resulting store schedule around the two int->float conversions.
// Declaring `texFrame` before the BCK setFrame call is what puts the two
// conversion temporaries in retail's order (0x28 then 0x30); dropping the
// named `texFrame` entirely is 74.1%. getBody(i) for the array read is inert.
// TODO: 98.6%, frame exact (0x68) and every instruction identical: the residue
// is one callee-saved GPR rotation.  Retail ranks `body` *above* the two
// parameters -- r31 `i`, r30 `body`, r29 blend, r28 anm, r27 `this`, r26 the
// strength-reduced mBodies offset, r25 the 0x4330 pool base -- while we rank
// it below everything, at r25, which is the "inner-block locals after `this`"
// bucket.  Closure batch 205: hoisting `body` to function scope, and hoisting
// both `i` and `body` out of the `for`, are both inert (still 98.6%, same
// registers), which is the catalog's "block scope is inert everywhere" rule
// again.  So retail's `body` is in the locals bucket for a reason that is not
// its declaration position, and the remaining shape has to change what MWCC
// counts as a function-scope local here.
void TBossHanachan::setHeadAndBodyAnm(
    EnumBossHanachanAnmKind anm, EnumBossHanachanStopMotionBlendOnOff blend)
{
	mHead->setAnm_(anm, blend);
	for (int i = 0; i < 8; ++i) {
		TBossHanachanPartsBody* body = mBodies[i];
		if (body->setAnm_(anm, blend)) {
			J3DFrameCtrl* bck = body->getMActor()->getFrameCtrl(ANM_TYPE_BCK);
			int frame = (i * getChangeParams()->getSLNormalBckFrameDiff())
			            % bck->getEnd();
			f32 texFrame = frame;
			bck->setFrame(frame);
			J3DFrameCtrl* btp = body->getMActor()->getFrameCtrl(ANM_TYPE_BTP);
			if (btp)
				btp->setFrame(texFrame);
			J3DFrameCtrl* btk = body->getMActor()->getFrameCtrl(ANM_TYPE_BTK);
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
	u8 delay = getChangeParams()->getSLGetUpFrameDiff();
	for (int i = 0; i < 8; ++i)
		mBodies[7 - i]->unk10C = delay * i;
	mHead->unk10C = delay * 8;
}

void TBossHanachan::setAnmTimerWhenSnort()
{
	u8 delay = getChangeParams()->getSLSnortFrameDiff();
	mHead->unk10C = 0;
	for (int i = 0; i < 8; ++i)
		mBodies[i]->unk10C = delay * (i + 1);
}

void TBossHanachan::setAnmTimerWhenDamage()
{
	u8 delay = getChangeParams()->getSLDamageFrameDiff();
	for (int i = 0; i < 8; ++i)
		mBodies[i]->unk10C = delay * CLBAbs(mWeakBodyIndex - i);
	mHead->unk10C = delay * CLBAbs(mWeakBodyIndex + 1);
}

void TBossHanachan::setAnmTimerWhenDead()
{
	u8 delay = getChangeParams()->getSLDeadFrameDiff();
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

// TODO: 99.9%. Frame is now exact; the only difference is that r26 and r28 are
// swapped (retail parks the tumble-loop counter, the latest-nerve pointer and
// the head MActor in r26, we use r28).
void TBossHanachan::changeAnmRateAndFrameUpdate_()
{
	bool changeRate = true;
	f32 rate = SMSGetAnmFrameRate();
	if (getSpine()->getLatestNerve() == &TNerveBossHanachanTumble::theNerve()) {
		offHeadAndBodyNonstopMotionBlend_();
		getHead()->changeTumbleAnmRate_();
		for (int i = 0; i < 8; ++i)
			mBodies[i]->changeTumbleAnmRate_();
		changeRate = false;
	} else {
		switch (getHead()->mCurrentAnm) {
		case BOSS_HANACHAN_ANM_UNK0:
		case BOSS_HANACHAN_ANM_UNK1:
			if (getMarchSpeed() <= getChangeParams()->getSLWalkAnmMarchSpeed()) {
				offHeadAndBodyNonstopMotionBlend_();
				switch (getHead()->mCurrentAnm) {
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
			} else if (getMarchSpeed() >= getChangeParams()->getSLRunAnmMarchSpeed()) {
				offHeadAndBodyNonstopMotionBlend_();
				switch (getHead()->mCurrentAnm) {
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
				f32 ratio = CLBCalcRatio(getChangeParams()->getSLWalkAnmMarchSpeed(),
				                        getChangeParams()->getSLRunAnmMarchSpeed(),
				                        mMarchSpeed);
				switch (getHead()->mCurrentAnm) {
				case BOSS_HANACHAN_ANM_UNK0:
					if (getHead()->mPreviousAnm != BOSS_HANACHAN_ANM_UNK1) {
						setHeadAndBodyAnm(BOSS_HANACHAN_ANM_UNK1,
						                  BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
						copyFrameFromOldAnmToNewAnm_();
						ratio = 1.0f - ratio;
					}
					setHeadAndBodyNonstopMotionBlendRatio_(ratio);
					break;
				case BOSS_HANACHAN_ANM_UNK1:
					if (getHead()->mPreviousAnm == BOSS_HANACHAN_ANM_UNK0) {
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
			       * getChangeParams()->mSLWalkBckRateMagnif.get();
			if (rate < getChangeParams()->mSLWalkBckRateMin.get())
				rate = getChangeParams()->mSLWalkBckRateMin.get();
			break;
		default:
			offHeadAndBodyNonstopMotionBlend_();
			rate = SMSGetAnmFrameRate();
			break;
		}
	}
	MActor* actor = getHead()->mMActor;
	if (changeRate)
		actor->getFrameCtrl(ANM_TYPE_BCK)->setRate(rate);
	getHead()->updateAnmSound();
	actor->frameUpdate();
	for (int i = 0; i < 8; ++i) {
		MActor* actor = mBodies[i]->mMActor;
		if (changeRate)
			actor->getFrameCtrl(ANM_TYPE_BCK)->setRate(rate);
		mBodies[i]->updateAnmSound();
		actor->frameUpdate();
	}
}

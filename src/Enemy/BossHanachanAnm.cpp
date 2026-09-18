#include <Enemy/BossHanachan.hpp>

// Accessors parked here for promotion to their owning classes: each one is an
// inline level that `changeAnmRateAndFrameUpdate_` needs and that a raw member
// read does not provide. Measured, from frame 0xc0 towards retail's 0x118:
// getChangeParams() +32 (12 sites), the getSL* wrappers +24 (10 of the 12
// sites; the two mSLWalkBckRate* ones must stay raw `.get()` or the
// magnif/min loads swap float registers), getHead() +24, getMarchSpeed() +8 at
// the two march-speed compares only (saturating; a third site is +0 and all
// four sites cost 1.5 points), getSpine() the last +8. Rejected: getBody(i)
// (+40, overshoots).
// TODO: promote to TBossHanachan as `TBossHanachanChangeSaveParams*
// getChangeParams() const`.
static inline TBossHanachanChangeSaveParams* BHA_changeParams(
    const TBossHanachan* boss)
{
	return boss->mChangeParams;
}

// TODO: promote to TBossHanachanChangeSaveParams as per-field getSL*
// wrappers (`f32 getSLWalkAnmMarchSpeed() const { return
// mSLWalkAnmMarchSpeed.get(); }` and so on).
// TODO: promote to TBossHanachan as `getHead()`.
static inline TBossHanachanPartsHead* BHA_head(const TBossHanachan* b)
{
	return b->mHead;
}

// TODO: promote to TBossHanachan (or TLiveActor) as `getSpine()`; TTinKoopa
// already carries exactly this accessor.
static inline TSpineBase<TLiveActor>* BHA_spine(const TBossHanachan* b)
{
	return b->mSpine;
}

// TODO: promote to TBossHanachan as `getMarchSpeed()`.
static inline f32 BHA_marchSpeed(const TBossHanachan* b)
{
	return b->mMarchSpeed;
}

static inline f32 BHA_paramF(const TParamRT<f32>& p) { return p.get(); }
static inline u8 BHA_paramB(const TParamRT<u8>& p) { return p.get(); }
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
void TBossHanachan::setHeadAndBodyAnm(
    EnumBossHanachanAnmKind anm, EnumBossHanachanStopMotionBlendOnOff blend)
{
	mHead->setAnm_(anm, blend);
	for (int i = 0; i < 8; ++i) {
		TBossHanachanPartsBody* body = mBodies[i];
		if (body->setAnm_(anm, blend)) {
			J3DFrameCtrl* bck = body->mMActor->getFrameCtrl(ANM_TYPE_BCK);
			int frame = (i * BHA_paramB(BHA_changeParams(this)->mSLNormalBckFrameDiff))
			            % bck->getEnd();
			f32 texFrame = frame;
			bck->setFrame(frame);
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
	u8 delay = BHA_paramB(BHA_changeParams(this)->mSLGetUpFrameDiff);
	for (int i = 0; i < 8; ++i)
		mBodies[7 - i]->unk10C = delay * i;
	mHead->unk10C = delay * 8;
}

void TBossHanachan::setAnmTimerWhenSnort()
{
	u8 delay = BHA_paramB(BHA_changeParams(this)->mSLSnortFrameDiff);
	mHead->unk10C = 0;
	for (int i = 0; i < 8; ++i)
		mBodies[i]->unk10C = delay * (i + 1);
}

void TBossHanachan::setAnmTimerWhenDamage()
{
	u8 delay = BHA_paramB(BHA_changeParams(this)->mSLDamageFrameDiff);
	for (int i = 0; i < 8; ++i)
		mBodies[i]->unk10C = delay * CLBAbs(mWeakBodyIndex - i);
	mHead->unk10C = delay * CLBAbs(mWeakBodyIndex + 1);
}

void TBossHanachan::setAnmTimerWhenDead()
{
	u8 delay = BHA_paramB(BHA_changeParams(this)->mSLDeadFrameDiff);
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
	if (BHA_spine(this)->getLatestNerve() == &TNerveBossHanachanTumble::theNerve()) {
		offHeadAndBodyNonstopMotionBlend_();
		BHA_head(this)->changeTumbleAnmRate_();
		for (int i = 0; i < 8; ++i)
			mBodies[i]->changeTumbleAnmRate_();
		changeRate = false;
	} else {
		switch (BHA_head(this)->mCurrentAnm) {
		case BOSS_HANACHAN_ANM_UNK0:
		case BOSS_HANACHAN_ANM_UNK1:
			if (BHA_marchSpeed(this) <= BHA_paramF(BHA_changeParams(this)->mSLWalkAnmMarchSpeed)) {
				offHeadAndBodyNonstopMotionBlend_();
				switch (BHA_head(this)->mCurrentAnm) {
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
			} else if (BHA_marchSpeed(this) >= BHA_paramF(BHA_changeParams(this)->mSLRunAnmMarchSpeed)) {
				offHeadAndBodyNonstopMotionBlend_();
				switch (BHA_head(this)->mCurrentAnm) {
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
				f32 ratio = CLBCalcRatio(BHA_paramF(BHA_changeParams(this)->mSLWalkAnmMarchSpeed),
				                        BHA_paramF(BHA_changeParams(this)->mSLRunAnmMarchSpeed),
				                        mMarchSpeed);
				switch (BHA_head(this)->mCurrentAnm) {
				case BOSS_HANACHAN_ANM_UNK0:
					if (BHA_head(this)->mPreviousAnm != BOSS_HANACHAN_ANM_UNK1) {
						setHeadAndBodyAnm(BOSS_HANACHAN_ANM_UNK1,
						                  BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
						copyFrameFromOldAnmToNewAnm_();
						ratio = 1.0f - ratio;
					}
					setHeadAndBodyNonstopMotionBlendRatio_(ratio);
					break;
				case BOSS_HANACHAN_ANM_UNK1:
					if (BHA_head(this)->mPreviousAnm == BOSS_HANACHAN_ANM_UNK0) {
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
			       * BHA_changeParams(this)->mSLWalkBckRateMagnif.get();
			if (rate < BHA_changeParams(this)->mSLWalkBckRateMin.get())
				rate = BHA_changeParams(this)->mSLWalkBckRateMin.get();
			break;
		default:
			offHeadAndBodyNonstopMotionBlend_();
			rate = SMSGetAnmFrameRate();
			break;
		}
	}
	MActor* actor = BHA_head(this)->mMActor;
	if (changeRate)
		actor->getFrameCtrl(ANM_TYPE_BCK)->setRate(rate);
	BHA_head(this)->updateAnmSound();
	actor->frameUpdate();
	for (int i = 0; i < 8; ++i) {
		MActor* actor = mBodies[i]->mMActor;
		if (changeRate)
			actor->getFrameCtrl(ANM_TYPE_BCK)->setRate(rate);
		mBodies[i]->updateAnmSound();
		actor->frameUpdate();
	}
}

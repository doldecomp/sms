#include <GC2D/Option.hpp>
#include <macros.h>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>
#include <System/Application.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <System/MarioGamePad.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MSound/MSound.hpp>
#include <GC2D/ExPane.hpp>
#include <Camera/CameraOption.hpp>
#include <Player/MarioAccess.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: get rid of this
static const char* dummyMactorStringValue1 = "\0\0\0\0\0\0\0\0\0\0\0";
static const char* SMS_NO_MEMORY_MESSAGE   = "メモリが足りません\n";

namespace {

void tag_to_string(char*, u32) { }

void print_pane_tree(J2DPane*, int) { }

const TPatternAnmControl::TAnmChunk cRumbleAnm[] = {
	{ 'cnt0', 0.03 }, { 'cnt1', 0.03 }, { 'cnt2', 0.03 }, { 'cnt1', 0.03 },
	{ 'cnt2', 0.03 }, { 'cnt1', 0.03 }, { 'cnt2', 0.03 }, { 'cnt1', 0.03 },
	{ 'cnt2', 0.03 }, { 'cnt0', 2.0 },
};

const TPatternAnmControl::TAnmChunk cStopRumbleAnm[] = {
	{ 'cnt0', 1.0f },
};

const TPatternAnmControl::TAnmChunk cSurMonteAnm[] = {
	{ 'fa_8', 0.3f }, { 'fa_9', 0.3f }, { 'fa_8', 0.3f }, { 'fa_9', 0.3f },
	{ 'fa_6', 0.3f }, { 'fa_7', 0.3f }, { 'fa_6', 0.3f }, { 'fa_7', 0.3f },
};

const TPatternAnmControl::TAnmChunk cSurLTSpeakerAnm[] = {
	{ 'sp2b', 0.3f }, { 'sp2a', 0.3f }, { 'sp2b', 0.3f },
	{ 'sp2a', 0.3f }, { 'sp2a', 1.2f },
};

const TPatternAnmControl::TAnmChunk cSurRTSpeakerAnm[] = {
	{ 'sp2c', 0.3f }, { 'sp2d', 0.3f }, { 'sp2c', 0.3f },
	{ 'sp2d', 0.3f }, { 'sp2c', 1.2f },
};

const TPatternAnmControl::TAnmChunk cSurLBSpeakerAnm[] = {
	{ 'sp2e', 1.2f }, { 'sp2f', 0.3f }, { 'sp2e', 0.3f },
	{ 'sp2f', 0.3f }, { 'sp2e', 0.3f },
};

const TPatternAnmControl::TAnmChunk cSurRBSpeakerAnm[] = {
	{ 'sp2g', 1.2f }, { 'sp2g', 0.3f }, { 'sp2h', 0.3f },
	{ 'sp2g', 0.3f }, { 'sp2h', 0.3f },
};

const TPatternAnmControl::TAnmChunk cSteMonteAnm[] = {
	{ 'fa_4', 0.3f }, { 'fa_5', 0.3f }, { 'fa_4', 0.3f }, { 'fa_5', 0.3f },
	{ 'fa_2', 0.3f }, { 'fa_3', 0.3f }, { 'fa_2', 0.3f }, { 'fa_3', 0.3f },
};

const TPatternAnmControl::TAnmChunk cSteLSpeakerAnm[] = {
	{ 'sp0b', 0.3f }, { 'sp0a', 0.3f }, { 'sp0b', 0.3f },
	{ 'sp0a', 0.3f }, { 'sp0a', 1.2f },
};

const TPatternAnmControl::TAnmChunk cSteRSpeakerAnm[] = {
	{ 'sp1a', 1.2f }, { 'sp1b', 0.3f }, { 'sp1a', 0.3f },
	{ 'sp1b', 0.3f }, { 'sp1a', 0.3f },
};

const TPatternAnmControl::TAnmChunk cMonoMonteAnm[] = {
	{ 'fa_0', 0.3f },
	{ 'fa_1', 0.3f },
};

const TPatternAnmControl::TAnmChunk cMonoSpeakerAnm[] = {
	{ 'sp0a', 0.3f },
	{ 'sp0b', 0.3f },
};

const u32 cRumbleToggleItems[] = {
	'sel1',
	'sel0',
};

const u32 cSoundToggleItems[] = {
	'sel3',
	'sel2',
	'sel4',
};

} // namespace

const TOptionSoundUnit::FabricatedSoundSettings
    TOptionSoundUnit::cSoundSettings[]
    = {
	      { 0x8843, 0 },
	      { 0x884A, 1 },
	      { 0x884B, 2 },
      };

const TOptionSoundUnit::FabricatedFlagInfo TOptionSoundUnit::cFlagInfos[] = {
	{ TOptionSoundUnit::SOUND_TYPE_MONO, 0 },
	{ TOptionSoundUnit::SOUND_TYPE_STEREO, 1 },
	{ TOptionSoundUnit::SOUND_TYPE_SURROUND, 2 },
};

void TArrowControl::update()
{
	updateAlpha();
	if (mPane->getAlpha() != 0) {
		updateScale();
	}
}

void TArrowControl::updateAlpha()
{
	int iVar3 = unk14 != 0 ? 1 : -1;
	mPane->setAlpha(
	    JGeometry::TUtil<s32>::clamp(iVar3 * 8 + mPane->getAlpha(), 0, 255));
}

// incorrect
void TArrowControl::updateScale()
{
	int move = calcMoveX(mPhase);
	mPane->setBounds(
	    JUTRect(mBounds.x1 - move, mBounds.y1, mBounds.x2, mBounds.y2));

	mPhase = JGeometry::TUtil<int>::mod(mPhase + 101, 100);
}

int TArrowControl::calcMoveX(int phase) const
{
	int iVar3 = phase < 50 ? phase : 100 - phase;
	f32 fVar1 = iVar3 / 50.0f;
	f32 fVar2 = 1.0f - fVar1;
	return fVar1 * 8.0f * fVar1 + fVar2 * -8.0f * fVar2 + fVar1 * fVar2;
}

TBalloonControl::TBalloonControl(int size)
    : unk4(size)
    , unk8(0)
{
	unk0 = new UnknownBalloonControlStruct[size];
}

void TBalloonControl::add(TExPane* pane)
{
	if (unk4 > unk8) {
		unk0[unk8].unk0 = pane;
		unk0[unk8].unk4 = pane->getPane()->getAlpha();
		++unk8;
	}
}

void TBalloonControl::setupAnm()
{
	mFrameCtrl.init((unk8 + 1) * 120.0f);
	mFrameCtrl.setAttribute(J3DFrameCtrl::ATTR_LOOP);
	unk20 = 0;
	for (int i = 0; i < unk8; ++i)
		unk0[i].unk0->getPane()->setAlpha(0);
	mFrameCtrl.setRate(1.0f);
}

void TBalloonControl::startAnm() { mFrameCtrl.setRate(1.0f); }

void TBalloonControl::stopAnm()
{
	mFrameCtrl.setRate(0.0f);
	mFrameCtrl.reset();
	for (int i = 0; i < unk8; ++i)
		unk0[i].unk0->getPane()->setAlpha(0);
}

void TBalloonControl::update() { }

TPaneScalingControl::TPaneScalingControl(J2DPane* pane)
    : mPane(pane)
{
	mInitialBounds = pane->getBounds();
}

void TPaneScalingControl::setupAnm(f32 amplitude, f32 speed)
{
	mAmplitude = amplitude;
	mFrameCtrl.init(0x78);
	mFrameCtrl.setAttribute(J3DFrameCtrl::ATTR_LOOP);
	mFrameCtrl.setRate(speed);
}

void TPaneScalingControl::startAnm() { mFrameCtrl.setRate(1.0f); }

void TPaneScalingControl::stopAnm()
{
	mFrameCtrl.setRate(0.0f);
	mFrameCtrl.reset();
}

void TPaneScalingControl::update()
{
	int iVar10 = mInitialBounds.getWidth();
	int iVar5  = mInitialBounds.getHeight();

	f32 progress = (f32)mFrameCtrl.getFrame() / (f32)mFrameCtrl.getEnd();
	f32 fVar2    = mAmplitude * JMASin(RAD_TO_DEG(progress * (2 * M_PI)));

	int uVar6 = fVar2 * iVar10;
	int uVar1 = fVar2 * iVar5;

	JUTRect local_5c = mInitialBounds;
	local_5c.move(mInitialBounds.x1 - uVar6 / 2, mInitialBounds.y1 - uVar1 / 2);
	local_5c.resize(uVar6 + mInitialBounds.getWidth(),
	                uVar1 + mInitialBounds.getHeight());
	mPane->mBounds = local_5c;
	mFrameCtrl.update();
}

TPatternAnmControl::TPatternAnmControl(J2DScreen* screen)
    : mScreen(screen)
{
}

void TPatternAnmControl::set(const TPatternAnmControl::TAnmChunk* chunks,
                             int num_chunks)
{
	mChunks.set(chunks, num_chunks);
	hide();
}

void TPatternAnmControl::setupAnm()
{
	mCurrentChunk = mChunks.begin();

	// Kahan compensated summation via Fast2Sum
	f32 sum   = 0.0f;
	f32 error = 0.0f;
	for (const TAnmChunk *it = mChunks.begin(), *e = mChunks.end(); it != e;
	     ++it) {
		f32 lastSum = sum;
		f32 next    = error + it->mDuration;
		sum += next;
		error = next - (sum - lastSum);
	}

	mFrameCtrl.init(120.0f * sum + 1.0f);
	mFrameCtrl.setAttribute(J3DFrameCtrl::ATTR_LOOP);
	mFrameCtrl.setRate(1.0f);
	show();
	mNextTriggerFrame = mCurrentChunk->mDuration * 120.0f;
}

void TPatternAnmControl::update()
{
	if (mFrameCtrl.checkPass(mNextTriggerFrame)
	    || mFrameCtrl.checkState(J3DFrameCtrl::STATE_LOOPED_ONCE)) {
		mScreen->search(mCurrentChunk->mTag)->hide();
		if (++mCurrentChunk == mChunks.end()
		    || mFrameCtrl.checkState(J3DFrameCtrl::STATE_LOOPED_ONCE)) {
			mCurrentChunk     = mChunks.begin();
			mNextTriggerFrame = 0.0f;
		}
		mScreen->search(mCurrentChunk->mTag)->show();
		mNextTriggerFrame += mCurrentChunk->mDuration * 120.0f;
	}
	mFrameCtrl.update();
}

void TPatternAnmControl::show()
{
	hide();
	mScreen->search(mCurrentChunk->mTag)->show();
}

void TPatternAnmControl::hide()
{
	for (const TAnmChunk* it = mChunks.begin(); it != mChunks.end(); ++it)
		mScreen->search(it->mTag)->hide();
}

TToggleControl::TToggleControl(J2DScreen* screen)
    : mScreen(screen)
{
}

void TToggleControl::setupToggle(const u32* tags, int num_tags)
{
	mItems.set(tags, num_tags);
	for (const u32* it = mItems.begin(); it != mItems.end(); ++it)
		mScreen->search(*it)->hide();
	mCurItem = mItems.begin();
	mScreen->search(*mCurItem)->show();
}

void TToggleControl::toggle()
{
	mScreen->search(*mCurItem)->hide();
	if (++mCurItem == mItems.end())
		mCurItem = mItems.begin();
	mScreen->search(*mCurItem)->show();
}

s32 TToggleControl::getNumber() const { return mCurItem - mItems.begin(); }

void TToggleControl::setNumber(int num)
{
	mScreen->search(*mCurItem)->hide();
	mCurItem = mItems.begin() + num;
	mScreen->search(*mCurItem)->show();
}

TOptionRumbleUnit::TOptionRumbleUnit(J2DScreen* screen)
    : mScreen(screen)
    , mShouldRumble(false)
{
	mParentPane   = new TExPane(mScreen, 'oya1');
	mInitialAlpha = mParentPane->getPane()->getAlpha();

	unkC = new TBalloonControl(3);
	unkC->add(new TExPane(mScreen, 'bub0'));
	unkC->add(new TExPane(mScreen, 'bub1'));
	unkC->add(new TExPane(mScreen, 'bub2'));
	unkC->setupAnm();

	// The speech bubble around the on/off text that pulsates
	// when this setting is selected.
	mSelectionBubble = new TPaneScalingControl(mScreen->search('me_0'));
	mSelectionBubble->setupAnm(0.05f, 1.0f);
	mSelectionBubble->stopAnm();

	// The image of a gamepad that either shakes occasionally or not based on
	// whether rumble is enabled in the options.
	mGamepadIcon[1] = new TPatternAnmControl(mScreen);
	mGamepadIcon[1]->set(cRumbleAnm, ARRAY_COUNT(cRumbleAnm));
	mGamepadIcon[1]->setupAnm();
	mGamepadIcon[1]->hide();

	mGamepadIcon[0] = new TPatternAnmControl(mScreen);
	mGamepadIcon[0]->set(cStopRumbleAnm, ARRAY_COUNT(cStopRumbleAnm));
	mGamepadIcon[0]->setupAnm();
	mGamepadIcon[0]->hide();

	// The text that says on/off for rumble in the options menu.
	mSelectionText = new TToggleControl(mScreen);
	mSelectionText->setupToggle(cRumbleToggleItems,
	                            ARRAY_COUNT(cRumbleToggleItems));

	setState(STATE_INACTIVE);
}

#pragma dont_inline on
void TOptionRumbleUnit::update()
{
	switch (mState) {
	case STATE_DEACTIVATING:
		mParentPane->update();
		// fade-out animation is done
		if (mParentPane->getPane()->getAlpha() == 150)
			setState(STATE_INACTIVE);
		break;

	case STATE_ACTIVE:
		mParentPane->update();
		mSelectionBubble->update();
		mGamepadIcon[getValue()]->update();
		checkRumble();
		break;

	case STATE_INACTIVE:
		break;
	}
}
#pragma dont_inline off

void TOptionRumbleUnit::checkRumble()
{
	if (mShouldRumble) {
		if (mGamepadIcon[mSelectionText->getNumber()]->checkCompletedOnce()) {
			mShouldRumble = false;
			SMSRumbleMgr->stop();
		} else {
			switch (mGamepadIcon[mSelectionText->getNumber()]
			            ->getCurrentPaneTag()) {
			case 'cnt0':
				SMSRumbleMgr->stop();
				break;

			case 'cnt1':
			case 'cnt2':
				SMSRumbleMgr->start(8, (float*)nullptr);
				break;
			}
		}
	}
}

void TOptionRumbleUnit::toggle()
{
	mShouldRumble = true;
	mSelectionText->toggle();
	adjust();
	SMSGetMSound()->startSoundSystemSE(0x481C, 0, nullptr, 0);
	SMSRumbleMgr->start(8, (float*)nullptr);
}

void TOptionRumbleUnit::adjust()
{
	bool b = mSelectionText->getNumber() == 1 ? true : false;
	SMSRumbleMgr->setActive(b);
	adjustView();
}

void TOptionRumbleUnit::adjustView()
{
	switch (mSelectionText->getNumber()) {
	case 0:
		mGamepadIcon[1]->hide();
		mGamepadIcon[0]->setupAnm();
		break;
	case 1:
		mGamepadIcon[0]->hide();
		mGamepadIcon[1]->setupAnm();
		break;
	}
}

void TOptionRumbleUnit::show() { }

void TOptionRumbleUnit::hide() { }

void TOptionRumbleUnit::deactivate(bool force)
{
	SMSRumbleMgr->stop();
	if (force)
		setState(TOptionRumbleUnit::STATE_INACTIVE);
	else
		setState(TOptionRumbleUnit::STATE_DEACTIVATING);
}

void TOptionRumbleUnit::activate()
{
	setState(TOptionRumbleUnit::STATE_ACTIVE);
}

void TOptionRumbleUnit::setValue(TOptionRumbleUnit::RumbleType type)
{
	mSelectionText->setNumber(type);
	adjust();
}

void TOptionRumbleUnit::setState(TOptionRumbleUnit::State state)
{
	mState = state;
	switch (state) {
	case STATE_INACTIVE:
		mParentPane->getPane()->setAlpha(150);
		setInfluencedAlphaRecursive(mParentPane->getPane(), true);
		mSelectionBubble->stopAnm();
		unkC->stopAnm();
		mShouldRumble = false;
		break;

	case STATE_DEACTIVATING:
		mParentPane->setPaneAlpha(30, 150, mInitialAlpha);
		setInfluencedAlphaRecursive(mParentPane->getPane(), true);
		unkC->stopAnm();
		mShouldRumble = false;
		break;

	case STATE_ACTIVE:
		mParentPane->getPane()->setAlpha(mInitialAlpha);
		setInfluencedAlphaRecursive(mParentPane->getPane(), false);
		mSelectionBubble->startAnm();
		unkC->setupAnm();
		unkC->startAnm();
		adjustView();
		break;
	}
}

void TOptionRumbleUnit::setInfluencedAlphaRecursive(J2DPane* pane, bool flag)
{
	for (JSUTreeIterator<J2DPane> it = pane->getPaneTree()->getFirstChild();
	     it != pane->getPaneTree()->getEndChild(); ++it) {
		it->setInfluenceAlpha(flag);
		setInfluencedAlphaRecursive(it.getObject(), flag);
	}
}

TOptionSoundUnit::TOptionSoundUnit(J2DScreen* screen)
    : mScreen(screen)
{
	mParentPane   = new TExPane(mScreen, 'oya2');
	mInitialAlpha = mParentPane->getPane()->getAlpha();

	unkC = new TBalloonControl(2);
	unkC->add(new TExPane(mScreen, 'bub3'));
	unkC->add(new TExPane(mScreen, 'bub4'));
	unkC->setupAnm();

	// The speech bubble around the mono/stereo/surround text that pulsates
	// when this setting is selected.
	mSelectionBubble = new TPaneScalingControl(mScreen->search('me_1'));
	mSelectionBubble->setupAnm(0.05f, 1.0f);
	mSelectionBubble->stopAnm();

	// These 3 are for the animation of a pianta (monte) vibing to the speakers
	initMonoAnm();
	initSteleoAnm();
	initSurroundAnm();

	// The toggle for switching between mono/stereo/surround.
	mSelectionText = new TToggleControl(mScreen);
	mSelectionText->setupToggle(cSoundToggleItems,
	                            ARRAY_COUNT(cSoundToggleItems));
	mMusicFrameCtrl.init(289);
	mMusicFrameCtrl.setAttribute(J3DFrameCtrl::ATTR_LOOP);
	mMusicFrameCtrl.setRate(1.0f);
	mMusic = nullptr;
	setState(STATE_INACTIVE);
	adjustView();
}

void TOptionSoundUnit::initMonoAnm()
{
	TPatternAnmControl** ary = mMonoAnimations;

	ary[0] = new TPatternAnmControl(mScreen);
	ary[0]->set(cMonoMonteAnm, ARRAY_COUNT(cMonoMonteAnm));
	ary[0]->setupAnm();

	ary[1] = new TPatternAnmControl(mScreen);
	ary[1]->set(cMonoSpeakerAnm, ARRAY_COUNT(cMonoSpeakerAnm));
	ary[1]->setupAnm();

	mMonteIcons[0].set(mMonoAnimations, ARRAY_COUNT(mMonoAnimations));
}

void TOptionSoundUnit::initSteleoAnm()
{
	TPatternAnmControl** ary = mStereoAnimations;

	ary[0] = new TPatternAnmControl(mScreen);
	ary[0]->set(cSteMonteAnm, ARRAY_COUNT(cSteMonteAnm));
	ary[0]->setupAnm();

	ary[1] = new TPatternAnmControl(mScreen);
	ary[1]->set(cSteRSpeakerAnm, ARRAY_COUNT(cSteRSpeakerAnm));
	ary[1]->setupAnm();

	ary[2] = new TPatternAnmControl(mScreen);
	ary[2]->set(cSteLSpeakerAnm, ARRAY_COUNT(cSteLSpeakerAnm));
	ary[2]->setupAnm();

	mMonteIcons[1].set(mStereoAnimations, ARRAY_COUNT(mStereoAnimations));
}

void TOptionSoundUnit::initSurroundAnm()
{
	TPatternAnmControl** ary = mSurroundAnimations;

	ary[0] = new TPatternAnmControl(mScreen);
	ary[0]->set(cSurMonteAnm, ARRAY_COUNT(cSurMonteAnm));
	ary[0]->setupAnm();

	ary[1] = new TPatternAnmControl(mScreen);
	ary[1]->set(cSurRTSpeakerAnm, ARRAY_COUNT(cSurRTSpeakerAnm));
	ary[1]->setupAnm();

	ary[2] = new TPatternAnmControl(mScreen);
	ary[2]->set(cSurLTSpeakerAnm, ARRAY_COUNT(cSurLTSpeakerAnm));
	ary[2]->setupAnm();

	ary[3] = new TPatternAnmControl(mScreen);
	ary[3]->set(cSurRBSpeakerAnm, ARRAY_COUNT(cSurRBSpeakerAnm));
	ary[3]->setupAnm();

	ary[4] = new TPatternAnmControl(mScreen);
	ary[4]->set(cSurLBSpeakerAnm, ARRAY_COUNT(cSurLBSpeakerAnm));
	ary[4]->setupAnm();

	mMonteIcons[2].set(mSurroundAnimations, ARRAY_COUNT(mSurroundAnimations));
}

#pragma dont_inline on
void TOptionSoundUnit::update()
{
	switch (mState) {
	case STATE_DEACTIVATING:
		mParentPane->update();
		// fade-out animation is done
		if (mParentPane->getPane()->getAlpha() == 150)
			setState(STATE_INACTIVE);
		break;

	case STATE_ACTIVE:
		mParentPane->update();
		mSelectionBubble->update();
		updatePatternAnm();
		break;

	case STATE_INACTIVE:
		break;
	}
}
#pragma dont_inline off

void TOptionSoundUnit::updatePatternAnm()
{
	ArrayWrapper<TPatternAnmControl*>& ary
	    = mMonteIcons[mSelectionText->getNumber()];

	for (TPatternAnmControl** it = ary.begin(); it != ary.end(); ++it)
		(*it)->update();

	mMusicFrameCtrl.update();
	if (mMusicFrameCtrl.checkState(J3DFrameCtrl::STATE_LOOPED_ONCE))
		adjustSound();
}

void TOptionSoundUnit::foreachPatternAnm(ArrayWrapper<TPatternAnmControl*>& ary,
                                         void (TPatternAnmControl::*ptmf)())
{
	for (TPatternAnmControl** it = ary.mData; it != ary.mData + ary.mSize; ++it)
		((*it)->*ptmf)();
}

void TOptionSoundUnit::toggle()
{
	mSelectionText->toggle();
	adjust();
	adjustSound();
}

void TOptionSoundUnit::adjust()
{
	adjustView();
	const FabricatedSoundSettings& setting
	    = cSoundSettings[mSelectionText->getNumber()];
	JAIGlobalParameter::setParamSoundOutputMode(setting.mOutputMode);
}

void TOptionSoundUnit::show() { }

void TOptionSoundUnit::hide() { }

void TOptionSoundUnit::deactivate(bool force)
{
	if (force)
		setState(TOptionSoundUnit::STATE_INACTIVE);
	else
		setState(TOptionSoundUnit::STATE_DEACTIVATING);
}

void TOptionSoundUnit::activate() { setState(TOptionSoundUnit::STATE_ACTIVE); }

void TOptionSoundUnit::setValue(int value)
{
	mSelectionText->setNumber(flagToType(value));
	adjust();
}

int TOptionSoundUnit::getValue() const
{
	return typeToFlag((SoundType)mSelectionText->getNumber());
}

void TOptionSoundUnit::stopSound()
{
	if (mMusic)
		mMusic->stop(1);
}

TOptionSoundUnit::SoundType TOptionSoundUnit::flagToType(int flag)
{
	for (const FabricatedFlagInfo* it = cFlagInfos;
	     it != cFlagInfos + ARRAY_COUNT(cFlagInfos); ++it) {
		if (it->mFlag == flag)
			return it->mSoundType;
	}

	return SOUND_TYPE_MONO;
}

int TOptionSoundUnit::typeToFlag(TOptionSoundUnit::SoundType type)
{
	for (const FabricatedFlagInfo* it = cFlagInfos;
	     it != cFlagInfos + ARRAY_COUNT(cFlagInfos); ++it) {
		if (it->mSoundType == type)
			return it->mFlag;
	}

	return 0;
}

void TOptionSoundUnit::setState(TOptionSoundUnit::State state)
{
	mState = state;
	switch (state) {
	case STATE_INACTIVE:
		mParentPane->getPane()->setAlpha(150);
		setInfluencedAlphaRecursive(mParentPane->getPane(), true);
		mSelectionBubble->stopAnm();
		unkC->stopAnm();
		stopSound();
		break;

	case STATE_DEACTIVATING:
		mParentPane->setPaneAlpha(30, 150, mInitialAlpha);
		setInfluencedAlphaRecursive(mParentPane->getPane(), true);
		unkC->stopAnm();
		break;

	case STATE_ACTIVE:
		mParentPane->getPane()->setAlpha(mInitialAlpha);
		setInfluencedAlphaRecursive(mParentPane->getPane(), false);
		mSelectionBubble->startAnm();
		unkC->setupAnm();
		unkC->startAnm();
		adjustView();
		adjustSound();
		break;
	}
}

void TOptionSoundUnit::adjustView()
{
	switch (mSelectionText->getNumber()) {
	case SOUND_TYPE_MONO:
		foreachPatternAnm(mMonteIcons[1], &TPatternAnmControl::hide);
		foreachPatternAnm(mMonteIcons[2], &TPatternAnmControl::hide);
		foreachPatternAnm(mMonteIcons[0], &TPatternAnmControl::show);
		foreachPatternAnm(mMonteIcons[0], &TPatternAnmControl::setupAnm);
		break;
	case SOUND_TYPE_STEREO:
		foreachPatternAnm(mMonteIcons[0], &TPatternAnmControl::hide);
		foreachPatternAnm(mMonteIcons[2], &TPatternAnmControl::hide);
		foreachPatternAnm(mMonteIcons[1], &TPatternAnmControl::show);
		foreachPatternAnm(mMonteIcons[1], &TPatternAnmControl::setupAnm);
		break;
	case SOUND_TYPE_SURROUND:
		foreachPatternAnm(mMonteIcons[0], &TPatternAnmControl::hide);
		foreachPatternAnm(mMonteIcons[1], &TPatternAnmControl::hide);
		foreachPatternAnm(mMonteIcons[2], &TPatternAnmControl::show);
		foreachPatternAnm(mMonteIcons[2], &TPatternAnmControl::setupAnm);
		break;
	}
}

void TOptionSoundUnit::adjustSound()
{
	stopSound();

	const FabricatedSoundSettings& setting
	    = cSoundSettings[mSelectionText->getNumber()];
	SMSGetMSound()->startSoundSystemSE(setting.mSoundSystemSE, 0, &mMusic, 0);

	mMusicFrameCtrl.setFrame(0.0f);
}

void TOptionSoundUnit::setInfluencedAlphaRecursive(J2DPane* pane, bool flag)
{
	for (JSUTreeIterator<J2DPane> it = pane->getPaneTree()->getFirstChild();
	     it != pane->getPaneTree()->getEndChild(); ++it) {
		it->setInfluenceAlpha(flag);
		setInfluencedAlphaRecursive(it.getObject(), flag);
	}
}

void TOptionControl::load()
{
	JKRArchive* optionArch = (JKRArchive*)JKRFileLoader::getVolume("option");

	mScreen = new J2DSetScreen("option.blo", optionArch);
	mScreen->setCullBack(GX_CULL_BACK);
	J2DTextBox* textBoxA = (J2DTextBox*)mScreen->search('m_0a');
	J2DTextBox* textBoxB = (J2DTextBox*)mScreen->search('m_0b');
	textBoxA->setFont((JUTFont*)gpSystemFont);
	textBoxB->setFont((JUTFont*)gpSystemFont);
	mBackArrow    = new TArrowControl(mScreen->search('yaji'));
	mRumbleOption = new TOptionRumbleUnit(mScreen);
	mSoundOption  = new TOptionSoundUnit(mScreen);
	setType(SELECT_TYPE_RUMBLE_OPTION, true);
	loadSetting();
	mWasJumping = false;
}

#pragma dont_inline on
void TOptionControl::loadSetting()
{
	switch (TFlagManager::getInstance()->getFlag(0xA0000)) {
	case 0:
		mSoundOption->setValue(0);
		break;
	case 1:
		mSoundOption->setValue(1);
		break;
	case 2:
		mSoundOption->setValue(2);
		break;
	}

	switch (TFlagManager::getInstance()->getFlag(0x90000)) {
	case 0:
		mRumbleOption->setValue(TOptionRumbleUnit::RUMBLE_TYPE_UNK0);
		break;
	case 1:
		mRumbleOption->setValue(TOptionRumbleUnit::RUMBLE_TYPE_UNK1);
		break;
	}

	resetChangedSetting();
}
#pragma dont_inline off

void TOptionControl::movementCommon() { }

void TOptionControl::draw(J2DOrthoGraph* graph) { mScreen->draw(0, 0, graph); }

// mario walks from the card select screen to the options screen
bool TOptionControl::movementCard2Option()
{
	if (gpCameraOption->unk12 == 0) {
		mRumbleOption->mShouldRumble = false;
		mScreen->search('oya0')->show();
		mScreen->search('oya1')->show();
		mScreen->search('oya2')->show();
		mWasJumping = false;
		setType(mSelectedOption, true);
		return true;
	}

	return false;
}

bool TOptionControl::movementOption()
{
	mBackArrow->update();
	mRumbleOption->update();
	mSoundOption->update();

	checkInput();
	writeValue();

	if (gpCameraOption->unk0 & 1) {
		mSoundOption->stopSound();
		return true;
	}

	return false;
}

static inline void fake(TOptionSoundUnit* unit) { int v = unit->getValue(); }

// mario walks back from the options screen to the card select screen
bool TOptionControl::movementOption2Card()
{
	if (gpCameraOption->unk12 == 0) {
		mScreen->search('oya0')->hide();
		mScreen->search('oya1')->hide();
		mScreen->search('oya2')->hide();

		if (mInitialRumbleValue == mRumbleOption->getValue())
			fake(mSoundOption);

		return true;
	}

	return false;
}

void TOptionControl::setType(TOptionControl::SelectType type,
                             bool initial_options_entry)
{
	if (mSelectedOption != type || initial_options_entry) {
		mSelectedOption = type;
		switch (type) {
		case SELECT_TYPE_RUMBLE_OPTION:
			mRumbleOption->activate();
			mSoundOption->deactivate(initial_options_entry);
			break;
		case SELECT_TYPE_SOUND_OPTION:
			mRumbleOption->deactivate(initial_options_entry);
			mSoundOption->activate();
			break;
		}

		if (!initial_options_entry)
			SMSGetMSound()->startSoundSystemSE(0x4815, 0, nullptr, 0);
	}
}

void TOptionControl::toggleCurType()
{
	switch (mSelectedOption) {
	case SELECT_TYPE_RUMBLE_OPTION:
		mRumbleOption->toggle();
		break;
	case SELECT_TYPE_SOUND_OPTION:
		mSoundOption->toggle();
		break;
	}
}

void TOptionControl::checkInput()
{
	f32 fVar1 = gpMarDirector->unk18[0]->getMainStickInDir(0.0f, 1.0f);
	if (fVar1 >= 0.75f)
		setType(SELECT_TYPE_RUMBLE_OPTION, false);
	else if (fVar1 <= -0.75f)
		setType(SELECT_TYPE_SOUND_OPTION, false);

	bool jumping = SMS_IsMarioStatusTypeJumping();
	if (!mWasJumping && jumping)
		toggleCurType();
	mWasJumping = jumping;
}

void TOptionControl::writeValue()
{
	TFlagManager::getInstance()->setFlag(0x90000, mRumbleOption->getValue());
	TFlagManager::getInstance()->setFlag(0xA0000, mSoundOption->getValue());
}

bool TOptionControl::isChangedSetting() const
{
	bool result = true;

	if (mInitialRumbleValue == mRumbleOption->getValue()
	    && mInitialSoundValue == mSoundOption->getValue())
		result = false;

	return result;
}

void TOptionControl::resetChangedSetting()
{
	mInitialRumbleValue = mRumbleOption->getValue();
	mInitialSoundValue  = mSoundOption->getValue();
}

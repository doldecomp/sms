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

const u32 cSubtitleToggleItems[] = {
	'sel6',
	'sel5',
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
	      { MSD_SE_SY_SOUT_MONO, 0 },
	      { MSD_SE_SY_SOUT_STEREO, 1 },
	      { MSD_SE_SY_SOUT_SURROUND, 2 },
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

// setBounds is in-class, so the JUTRect temporary is one more level
// (an inlined call's argument). That puts the 4-arg ctor at depth 5
// (never) when updateScale expands into movementOption, and retail's
// weak 0x30 copy is `bl`'d from there. The receiver is TArrowControl*
// so mPane is loaded after the ctor, not bound before it.
static inline void OptionSetShiftedBounds(TArrowControl* arrow, int move)
{
	arrow->mPane->setBounds(JUTRect(arrow->mBounds.x1 - move, arrow->mBounds.y1,
	                               arrow->mBounds.x2, arrow->mBounds.y2));
}

void TArrowControl::updateScale()
{
	int move = calcMoveX(mPhase);
	OptionSetShiftedBounds(this, move);

	mPhase = JGeometry::TUtil<int>::mod(mPhase + 101, 100);
}

int TArrowControl::calcMoveX(int phase) const
{
	int iVar3 = phase < 50 ? phase : 100 - phase;
	f32 fVar1 = iVar3 / 50.0f;
	f32 fVar2 = 1.0f - fVar1;
	return fVar2 * -8.0f * fVar2 + fVar1 * fVar2 + fVar1 * 8.0f * fVar1;
}

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
	f32 fVar2 = mAmplitude * JMASin(RAD_TO_DEG(progress * TAU));

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
	mItems.set((u32*)tags, num_tags);
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
	SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_SELECT_COMMON, 0, nullptr, 0);
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
		mShouldRumble = false;
		break;

	case STATE_DEACTIVATING:
		mParentPane->setPaneAlpha(30, 150, getInitialAlpha());
		setInfluencedAlphaRecursive(mParentPane->getPane(), true);
		mShouldRumble = false;
		break;

	case STATE_ACTIVE:
		mParentPane->getPane()->setAlpha(mInitialAlpha);
		setInfluencedAlphaRecursive(mParentPane->getPane(), false);
		mSelectionBubble->startAnm();
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

// ---------------------------------------------------------------------------
// TOptionSubtitleUnit is US only: the Japanese release has no subtitle option.
// Its setState() is small enough (nine statements) that MWCC inlines it at
// every call site, which is why activate()/deactivate() are 0xb8/0x1e0 in the
// map where the rumble unit's are 0x24/0x60.
// ---------------------------------------------------------------------------

// Binding level worth +8 of low region, landing
// TOptionSubtitleUnit::TOptionSubtitleUnit's frame at 0x1a0 (batch 121).
static inline J2DPane* OptionGetPane(const TExPane* p)
{
	return p->getPane();
}

TOptionSubtitleUnit::TOptionSubtitleUnit(J2DScreen* screen)
    : mScreen(screen)
{
	mParentPane   = new TExPane(mScreen, 'txp2');
	mInitialAlpha = OptionGetPane(mParentPane)->getAlpha();

	// The speech bubble around the on/off text that pulsates
	// when this setting is selected.
	mSelectionBubble = new TPaneScalingControl(mScreen->search('me_2'));
	mSelectionBubble->setupAnm(0.05f, 1.0f);
	mSelectionBubble->stopAnm();

	// The text that says on/off for subtitles in the options menu.
	mSelectionText = new TToggleControl(mScreen);
	mSelectionText->setupToggle(cSubtitleToggleItems,
	                            ARRAY_COUNT(cSubtitleToggleItems));

	setState(STATE_INACTIVE);
}

// One binding level over a raw member read, worth +8 of low region.  Exactly
// one of update()'s three mParentPane reads goes through it: each binder site
// is +8 and a fork (or a raw read) is +0, so one site lands the frame on the
// map's 0x1f0.  Which of the three carries it is not observable -- all twelve
// one-binder arrangements compile to the same 161 instructions (batch 127,
// remeasured in header round 43).
static inline TExPane* OptionParentPane(const TOptionSubtitleUnit* p)
{
	TExPane* parentPane = p->mParentPane;
	return parentPane;
}

void TOptionSubtitleUnit::update()
{
	switch (mState) {
	case STATE_DEACTIVATING:
		OptionParentPane(this)->update();
		// fade-out animation is done
		if (mParentPane->getPane()->getAlpha() == 150)
			setState(STATE_INACTIVE);
		break;

	case STATE_ACTIVE:
		mParentPane->update();
		mSelectionBubble->update();
		break;

	case STATE_INACTIVE:
		break;
	}
}

void TOptionSubtitleUnit::toggle()
{
	mSelectionText->toggle();
	// TODO: the ROM discards this read. Whatever applied the new value here
	// was removed, and only the dead `bl ArrayWrapper<Ul>::begin()` inside
	// TOptionControl::checkInput's copy is left of it. adjust() cannot be
	// where it sat: the map gives adjust() four bytes, i.e. an empty body.
	getValue();
	SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_SELECT_COMMON, 0, nullptr, 0);
}

void TOptionSubtitleUnit::adjust() { }

void TOptionSubtitleUnit::show() { }

void TOptionSubtitleUnit::hide() { }

void TOptionSubtitleUnit::deactivate(bool force)
{
	if (force)
		setState(TOptionSubtitleUnit::STATE_INACTIVE);
	else
		setState(TOptionSubtitleUnit::STATE_DEACTIVATING);
}

void TOptionSubtitleUnit::activate()
{
	setState(TOptionSubtitleUnit::STATE_ACTIVE);
}

void TOptionSubtitleUnit::setValue(TOptionSubtitleUnit::SubtitleType value)
{
	mSelectionText->setNumber(value);
}

void TOptionSubtitleUnit::setState(TOptionSubtitleUnit::State state)
{
	mState = state;
	switch (state) {
	case STATE_INACTIVE:
		mParentPane->getPane()->setAlpha(150);
		setInfluencedAlphaRecursive(mParentPane->getPane(), true);
		mSelectionBubble->stopAnm();
		break;

	case STATE_DEACTIVATING:
		mParentPane->setPaneAlpha(30, 150, mInitialAlpha);
		setInfluencedAlphaRecursive(mParentPane->getPane(), true);
		break;

	case STATE_ACTIVE:
		mParentPane->getPane()->setAlpha(mInitialAlpha);
		setInfluencedAlphaRecursive(mParentPane->getPane(), false);
		mSelectionBubble->startAnm();
		break;
	}
}

void TOptionSubtitleUnit::setInfluencedAlphaRecursive(J2DPane* pane, bool flag)
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

	// The speech bubble around the mono/stereo/surround text that pulsates
	// when this setting is selected.
	mSelectionBubble = new TPaneScalingControl(mScreen->search('me_1'));
	mSelectionBubble->setupAnm(0.05f, 1.0f);
	mSelectionBubble->stopAnm();

	// These 3 are for the animation of a pianta (monte) vibing to the speakers.
	// initMonoAnm() is UNUSED in the map and size-exact, but its body has to be
	// written out here: behind the call TPatternAnmControl::hide() sits one
	// level too deep and becomes a bl the ROM does not have.
	TPatternAnmControl** ary = mMonoAnimations;

	ary[0] = new TPatternAnmControl(mScreen);
	ary[0]->set(cMonoMonteAnm, ARRAY_COUNT(cMonoMonteAnm));
	ary[0]->setupAnm();

	ary[1] = new TPatternAnmControl(mScreen);
	ary[1]->set(cMonoSpeakerAnm, ARRAY_COUNT(cMonoSpeakerAnm));
	ary[1]->setupAnm();

	mMonteIcons[0].set(mMonoAnimations, ARRAY_COUNT(mMonoAnimations));
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
	for (TPatternAnmControl** it = ary.begin(); it != ary.end(); ++it)
		((*it)->*ptmf)();
}

void TOptionSoundUnit::toggle()
{
	mSelectionText->toggle();
	adjust();
	adjustSound();
}

// One extra inline level so SMSGetMSound sits at depth 5 from checkInput
// (never inlines: the map's weak 8-byte copy and retail's discarded
// receiver) while staying at depth 4 from loadSetting (still expands).
static inline void OptionSetOutputMode(u32 mode)
{
	MSound* sound = SMSGetMSound();
	sound->setParamSoundOutputMode(mode);
}

void TOptionSoundUnit::adjust()
{
	adjustView();
	const FabricatedSoundSettings& setting
	    = cSoundSettings[mSelectionText->getNumber()];
	OptionSetOutputMode(setting.mOutputMode);
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
		stopSound();
		break;

	case STATE_DEACTIVATING:
		mParentPane->setPaneAlpha(30, 150, getInitialAlpha());
		setInfluencedAlphaRecursive(mParentPane->getPane(), true);
		break;

	case STATE_ACTIVE:
		mParentPane->getPane()->setAlpha(mInitialAlpha);
		setInfluencedAlphaRecursive(mParentPane->getPane(), false);
		mSelectionBubble->startAnm();
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
	// stopSound()'s body is written out here rather than called: retail's
	// TOptionControl::checkInput expands it at inline depth 4, where a plain
	// call would not have been inlined (95.12% -> 98.84% on checkInput, and
	// the out-of-line adjustSound is unchanged).
	if (mMusic)
		mMusic->stop(1);

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
	mSoundOption    = new TOptionSoundUnit(mScreen);
	mSubtitleOption = new TOptionSubtitleUnit(mScreen);
	setType(SELECT_TYPE_RUMBLE_OPTION, true);
	loadSetting();
	mWasJumping   = false;
	mStickNeutral = true;
}

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

	switch (TFlagManager::getInstance()->getFlag(0x90001)) {
	case 0:
		mSubtitleOption->setValue(TOptionSubtitleUnit::SUBTITLE_TYPE_UNK0);
		break;
	case 1:
		mSubtitleOption->setValue(TOptionSubtitleUnit::SUBTITLE_TYPE_UNK1);
		break;
	}

	resetChangedSetting();
}

// UNUSED in the map (0x6a0): it exists to give the three unit update()s an
// extra inline level. Inlined into movementOption they sit at depth 2, where
// MWCC refuses them and emits the ROM's three `bl`s; spelled out in
// movementOption they would all expand.
void TOptionControl::movementCommon()
{
	mRumbleOption->update();
	mSoundOption->update();
	mSubtitleOption->update();
}

void TOptionControl::draw(J2DOrthoGraph* graph) { mScreen->draw(0, 0, graph); }

// Binding level over a raw member read, worth +8 of low region in
// TOptionControl::movementCard2Option (batch 127).
static inline J2DSetScreen* OptionScreen(const TOptionControl* p)
{
	J2DSetScreen* screen = p->mScreen;
	return screen;
}

// mario walks from the card select screen to the options screen
bool TOptionControl::movementCard2Option()
{
	if (gpCameraOption->unk12 == 0) {
		mRumbleOption->mShouldRumble = false;
		OptionScreen(this)->search('txp2')->show();
		mScreen->search('oya0')->show();
		mScreen->search('oya1')->show();
		mScreen->search('oya2')->show();
		mWasJumping = false;
		setType(mSelectedOption, true);
		return true;
	}

	return false;
}

// +8 of low region in movementOption (target 0xa0, ours 0x98 after the
// JUTRect ctor depth fix). One site, one binder.
// TODO: the JUTRect temp still sits at 0x58(r1) vs retail 0x64 (0xC);
// frame and every opcode match.
static inline TArrowControl* OptionBackArrow(const TOptionControl* p)
{
	TArrowControl* arrow = p->mBackArrow;
	return arrow;
}

bool TOptionControl::movementOption()
{
	OptionBackArrow(this)->update();
	movementCommon();

	checkInput();
	writeValue();

	if (SMSGetCameraOption()->unk0 & 1) {
		mSoundOption->stopSound();
		return true;
	}

	return false;
}

// mario walks back from the options screen to the card select screen
bool TOptionControl::movementOption2Card()
{
	if (gpCameraOption->unk12 == 0) {
		mScreen->search('oya0')->hide();
		mScreen->search('oya1')->hide();
		mScreen->search('oya2')->hide();

		// TODO: the ROM discards this result; whatever the original did
		// with it was removed, leaving the rumble compare and the dead
		// TOptionSoundUnit::getValue() call behind.
		isChangedSetting();

		return true;
	}

	return false;
}

// Binding level over a raw member read, worth +8 of low region and the
// volatile-register rotation in isChangedSetting.
static inline TOptionRumbleUnit* OptionRumbleUnit(const TOptionControl* p)
{
	TOptionRumbleUnit* unit = p->mRumbleOption;
	return unit;
}

static inline TOptionSoundUnit* OptionSoundOption(const TOptionControl* p)
{
	TOptionSoundUnit* unit = p->mSoundOption;
	return unit;
}

static inline TOptionSubtitleUnit* OptionSubtitleOption(const TOptionControl* p)
{
	TOptionSubtitleUnit* unit = p->mSubtitleOption;
	return unit;
}

void TOptionControl::setType(TOptionControl::SelectType type,
                             bool initial_options_entry)
{
	// getRumbleOption() at the rumble activate site is the +8 of low
	// region (header accessor, not a TU-local binder). Binders over
	// mRumbleOption / mSoundOption here were +0x10.
	if (mSelectedOption != type || initial_options_entry) {
		mSelectedOption = type;
		switch (type) {
		case SELECT_TYPE_RUMBLE_OPTION:
			getRumbleOption()->activate();
			mSoundOption->deactivate(initial_options_entry);
			mSubtitleOption->deactivate(initial_options_entry);
			break;
		case SELECT_TYPE_SOUND_OPTION:
			mRumbleOption->deactivate(initial_options_entry);
			mSoundOption->activate();
			mSubtitleOption->deactivate(initial_options_entry);
			break;
		case SELECT_TYPE_SUBTITLE_OPTION:
			mSubtitleOption->activate();
			mRumbleOption->deactivate(initial_options_entry);
			mSoundOption->deactivate(initial_options_entry);
			break;
		}

		if (!initial_options_entry)
			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_CURSOR_COMMON, 0,
			                                   nullptr, 0);
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
	case SELECT_TYPE_SUBTITLE_OPTION:
		mSubtitleOption->toggle();
		break;
	}
}

void TOptionControl::checkInput()
{
	f32 fVar1       = gpMarDirector->unk18[0]->getMainStickInDir(0.0f, 1.0f);
	SelectType type = mSelectedOption;
	if (0.75f <= fVar1) {
		if (mStickNeutral) {
			mStickNeutral = false;
			switch (type) {
			case SELECT_TYPE_RUMBLE_OPTION:
			case SELECT_TYPE_SOUND_OPTION:
				setType(SELECT_TYPE_RUMBLE_OPTION, false);
				break;
			case SELECT_TYPE_SUBTITLE_OPTION:
				setType(SELECT_TYPE_SOUND_OPTION, false);
				break;
			}
		}
	} else if (fVar1 <= -0.75f) {
		if (mStickNeutral) {
			mStickNeutral = false;
			switch (type) {
			case SELECT_TYPE_RUMBLE_OPTION:
				setType(SELECT_TYPE_SOUND_OPTION, false);
				break;
			case SELECT_TYPE_SOUND_OPTION:
			case SELECT_TYPE_SUBTITLE_OPTION:
				setType(SELECT_TYPE_SUBTITLE_OPTION, false);
				break;
			}
		}
	} else {
		mStickNeutral = true;
	}

	bool jumping = SMS_IsMarioStatusTypeJumping();
	if (!mWasJumping && jumping)
		toggleCurType();
	mWasJumping = jumping;
}

void TOptionControl::writeValue()
{
	TFlagManager::getInstance()->setFlag(0x90000, mRumbleOption->getValue());
	TFlagManager::getInstance()->setFlag(0xA0000, mSoundOption->getValue());
	TFlagManager::getInstance()->setFlag(0x90001,
	                                     mSubtitleOption->getValue());
}

bool TOptionControl::isChangedSetting() const
{
	// TODO: retail `mr r29, r31` copies result into soundResult; MWCC
	// folds `bool soundResult = result` to a second `li r29, 1`.
	bool result                = true;
	bool soundResult           = result;
	int initialRumble          = mInitialRumbleValue;
	TToggleControl* rumbleText = OptionRumbleUnit(this)->mSelectionText;
	if (initialRumble == rumbleText->getNumber()
	    && mInitialSoundValue == OptionSoundOption(this)->getValue())
		soundResult = false;

	if (!soundResult) {
		int initialSubtitle = mInitialSubtitleValue;
		TToggleControl* subtitleText
		    = OptionSubtitleOption(this)->mSelectionText;
		if (initialSubtitle == subtitleText->getNumber())
			result = false;
	}

	return result;
}

void TOptionControl::resetChangedSetting()
{
	mInitialRumbleValue   = getRumbleOption()->getValue();
	mInitialSoundValue    = getSoundOption()->getValue();
	mInitialSubtitleValue = getSubtitleOption()->getValue();
}

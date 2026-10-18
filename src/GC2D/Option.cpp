#include <GC2D/Option.hpp>
#include <GC2D/MessageUtil.hpp>
#include <macros.h>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <JSystem/J2D/J2DPicture.hpp>
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
#include <stdio.h>

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

#if !defined(VERSION_GMSJ01) && !defined(VERSION_GMSJ01_01)
const u32 cSubtitleToggleItems[] = {
	'sel6',
	'sel5',
};
#endif

const u32 cSoundToggleItems[] = {
	'sel3',
	'sel2',
	'sel4',
};

#ifdef VERSION_GMSP01
const u32 cLanguageToggleItems[] = {
	'lan0', 'lan1', 'lan2', 'lan3', 'lan4',
};
#elif defined(VERSION_GMSP01_NX)
const u32 cLanguageToggleItems[] = {
	'lan0', 'lan1', 'lan2', 'lan3', 'lan4', 'lan5', 'lan6',
};
#endif

#ifdef VERSION_GMSP01
void loadTexture(JUTTexture** textures, char* name)
{
	static const char* langArray[] = { "", "ge/", "fr/", "sp/", "it/" };
	for (int i = 0; i < 5; ++i) {
		char path[256];
		snprintf(path, 255, "/option/timg/%s%s", langArray[i], name);
		textures[i] = new JUTTexture((const ResTIMG*)JKRGetResource(path));
	}
}
#endif

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

#if !defined(VERSION_GMSJ01) && !defined(VERSION_GMSE01)
TArrowControl::TArrowControl(J2DScreen* screen, J2DPicture* picture)
    : mScreen(screen)
    , mPane(picture)
    , unk14(true)
    , mPhase(0)
{
	mBounds = mPane->getBounds();
#ifdef VERSION_GMSP01
	loadTexture(mTextures, "select_quit.bti");
#endif
}
#endif

#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
s32 TArrowControl::replaceTexture(u32 tag, JUTTexture* texture)
{
	int offset;
	J2DPicture* picture = (J2DPicture*)mScreen->search(tag);
	JUT_ASSERT(picture);
	offset = texture->getWidth() - picture->getWidth();
	picture->changeTexture(texture->getTexInfo(), 0);

	JUTRect rect(picture->getBounds());
	rect.reform(0, 0, offset, 0);
	picture->place(rect);

	return picture->getWidth();
}

void TArrowControl::changeTexture(int language)
{
	replaceTexture('s_1', mTextures[language]);
}
#endif

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
	u8 alpha
	    = JGeometry::TUtil<s32>::clamp(iVar3 * 8 + mPane->getAlpha(), 0, 255);
	mPane->setAlpha(alpha);
}

// incorrect
void TArrowControl::updateScale()
{
	int move = calcMoveX(mPhase);
	mPane->place(
	    JUTRect(mBounds.x1 - move, mBounds.y1, mBounds.x2, mBounds.y2));

	mPhase = JGeometry::TUtil<int>::mod(mPhase + 101, 100);
}

int TArrowControl::calcMoveX(int phase) const
{
	f32 fVar1 = (phase < 50 ? phase : 100 - phase) / 50.0f;
	f32 fVar2 = 1.0f - fVar1;
	return fVar1 * 8.0f * fVar1 + fVar2 * -8.0f * fVar2 + fVar1 * fVar2;
}

#ifdef VERSION_GMSJ01
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
#endif

TPaneScalingControl::TPaneScalingControl(J2DPane* pane)
    : mPane(pane)
{
	mInitialBounds = pane->getBounds();
}

#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
void TPaneScalingControl::resize(int width)
{
	int offset = ((width + 30) - mInitialBounds.getWidth()) / 2;
	mInitialBounds.reform(-offset, 0, offset, 0);
	update();
}
#endif

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
	f32 fVar2 = mAmplitude
	            * MsSin(RAD_TO_DEG((f32)mFrameCtrl.getFrame()
	                               / (f32)mFrameCtrl.getEnd() * (2 * M_PI)));

	int uVar6 = fVar2 * mInitialBounds.getWidth();
	int uVar1 = fVar2 * mInitialBounds.getHeight();

	JUTRect local_5c = mInitialBounds;
	local_5c.move(mInitialBounds.x1 - uVar6 / 2, mInitialBounds.y1 - uVar1 / 2);
	local_5c.resize(uVar6 + mInitialBounds.getWidth(),
	                uVar1 + mInitialBounds.getHeight());
	mPane->place(local_5c);
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
	for (const TAnmChunk* it = mChunks.begin(); it != mChunks.end(); ++it)
		mScreen->search(it->mTag)->hide();
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

#ifdef VERSION_GMSJ01
	unkC = new TBalloonControl(3);
	unkC->add(new TExPane(mScreen, 'bub0'));
	unkC->add(new TExPane(mScreen, 'bub1'));
	unkC->add(new TExPane(mScreen, 'bub2'));
	unkC->setupAnm();
#endif

	// The speech bubble around the on/off text that pulsates
	// when this setting is selected.
	mSelectionBubble = new TPaneScalingControl(mScreen->search('me_0'));
	mSelectionBubble->setupAnm(0.05f, 1.0f);
	mSelectionBubble->stopAnm();

#if !defined(VERSION_GMSP01) && !defined(VERSION_GMSP01_NX)
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
#endif

	// The text that says on/off for rumble in the options menu.
	mSelectionText = new TToggleControl(mScreen);
	mSelectionText->setupToggle(cRumbleToggleItems,
	                            ARRAY_COUNT(cRumbleToggleItems));

#ifdef VERSION_GMSP01
	loadTexture(mRumbleTextures, "select_rumble.bti");
	char* names[2] = { "select_on.bti", "select_off.bti" };
	for (int i = 0; i < 2; ++i)
		loadTexture(mRumbleGridTextures[i], names[i]);
#endif

	setState(STATE_INACTIVE);
}

#ifdef VERSION_GMSP01
s32 TOptionRumbleUnit::replaceTexture(u32 tag, JUTTexture* texture)
{
	int offset;
	J2DPicture* picture = (J2DPicture*)mScreen->search(tag);
	JUT_ASSERT(picture);
	offset = (texture->getWidth() - picture->getWidth()) / 2;
	picture->changeTexture(texture->getTexInfo(), 0);

	JUTRect rect(picture->getBounds());
	rect.reform(-offset, 0, offset, 0);
	picture->place(rect);

	return picture->getWidth();
}

void TOptionRumbleUnit::changeTexture(int language)
{
	int width = replaceTexture('m_1', mRumbleTextures[language]);
	for (int i = 0; i < 2; ++i) {
		int itemWidth
		    = replaceTexture('sel0' + i, mRumbleGridTextures[i][language]);
		if (itemWidth > width)
			width = itemWidth;
	}
	mSelectionBubble->resize(width);
}
#endif

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
#if !defined(VERSION_GMSP01) && !defined(VERSION_GMSP01_NX)
		mGamepadIcon[getValue()]->update();
		checkRumble();
#endif
		break;

	case STATE_INACTIVE:
		break;
	}
}

void TOptionRumbleUnit::checkRumble()
{
#if !defined(VERSION_GMSP01) && !defined(VERSION_GMSP01_NX)
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
#endif
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
#if !defined(VERSION_GMSP01) && !defined(VERSION_GMSP01_NX)
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
#endif
}

void TOptionRumbleUnit::show() { mShouldRumble = false; }

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
#ifdef VERSION_GMSJ01
		unkC->stopAnm();
#endif
		mShouldRumble = false;
		break;

	case STATE_DEACTIVATING:
		mParentPane->setPaneAlpha(30, 150, mInitialAlpha);
		setInfluencedAlphaRecursive(mParentPane->getPane(), true);
#ifdef VERSION_GMSJ01
		unkC->stopAnm();
#endif
		mShouldRumble = false;
		break;

	case STATE_ACTIVE:
		mParentPane->getPane()->setAlpha(mInitialAlpha);
		setInfluencedAlphaRecursive(mParentPane->getPane(), false);
		mSelectionBubble->startAnm();
#ifdef VERSION_GMSJ01
		unkC->setupAnm();
		unkC->startAnm();
#endif
		adjustView();
		break;
	}
}

void TOptionRumbleUnit::setInfluencedAlphaRecursive(J2DPane* pane,
                                                    bool influenced_alpha)
{
	for (JSUTreeIterator<J2DPane> it = pane->getPaneTree()->getFirstChild();
	     it != pane->getPaneTree()->getEndChild(); ++it) {
		it->setInfluencedAlpha(influenced_alpha);
		setInfluencedAlphaRecursive(it.getObject(), influenced_alpha);
	}
}

#if !defined(VERSION_GMSJ01) && !defined(VERSION_GMSJ01_01)
TOptionSubtitleUnit::TOptionSubtitleUnit(J2DScreen* screen)
    : mScreen(screen)
{
	mParentPane   = new TExPane(mScreen, 'txp2');
	mInitialAlpha = mParentPane->getPane()->getAlpha();

	mSelectionBubble = new TPaneScalingControl(mScreen->search('me_2'));
	mSelectionBubble->setupAnm(0.05f, 1.0f);
	mSelectionBubble->stopAnm();

	mSelectionText = new TToggleControl(mScreen);
	mSelectionText->setupToggle(cSubtitleToggleItems,
	                            ARRAY_COUNT(cSubtitleToggleItems));

#ifdef VERSION_GMSP01
	loadTexture(mSubtitleTextures, "select_subtitles.bti");
	char* names[2] = { "select_on.bti", "select_off.bti" };
	for (int i = 0; i < 2; ++i)
		loadTexture(mSubtitleGridTextures[i], names[i]);
#endif

	setState(STATE_INACTIVE);
}

#ifdef VERSION_GMSP01
s32 TOptionSubtitleUnit::replaceTexture(u32 tag, JUTTexture* texture)
{
	int offset;
	J2DPicture* picture = (J2DPicture*)mScreen->search(tag);
	JUT_ASSERT(picture);
	offset = (texture->getWidth() - picture->getWidth()) / 2;
	picture->changeTexture(texture->getTexInfo(), 0);

	JUTRect rect(picture->getBounds());
	rect.reform(-offset, 0, offset, 0);
	picture->place(rect);

	return picture->getWidth();
}

void TOptionSubtitleUnit::changeTexture(int language)
{
	int width = replaceTexture('m_3', mSubtitleTextures[language]);
	for (int i = 0; i < 2; ++i) {
		int itemWidth
		    = replaceTexture('sel5' + i, mSubtitleGridTextures[i][language]);
		if (itemWidth > width)
			width = itemWidth;
	}
	mSelectionBubble->resize(width);
}
#endif

void TOptionSubtitleUnit::update()
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
		break;

	case STATE_INACTIVE:
		break;
	}
}

void TOptionSubtitleUnit::toggle()
{
	mSelectionText->toggle();
	adjust();
	SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_SELECT_COMMON, 0, nullptr, 0);
}

void TOptionSubtitleUnit::adjust() { int value = mSelectionText->getNumber(); }

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

void TOptionSubtitleUnit::setValue(TOptionSubtitleUnit::SubtitleType type)
{
	mSelectionText->setNumber(type);
	adjust();
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

void TOptionSubtitleUnit::setInfluencedAlphaRecursive(J2DPane* pane,
                                                      bool influenced_alpha)
{
	for (JSUTreeIterator<J2DPane> it = pane->getPaneTree()->getFirstChild();
	     it != pane->getPaneTree()->getEndChild(); ++it) {
		it->setInfluencedAlpha(influenced_alpha);
		setInfluencedAlphaRecursive(it.getObject(), influenced_alpha);
	}
}
#endif

#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
TOptionLanguageUnit::TOptionLanguageUnit(J2DScreen* screen)
    : mScreen(screen)
{
	mParentPane   = new TExPane(mScreen, 'txp3');
	mInitialAlpha = mParentPane->getPane()->getAlpha();

	mSelectionBubble = new TPaneScalingControl(mScreen->search('me_3'));
	mSelectionBubble->setupAnm(0.05f, 1.0f);
	mSelectionBubble->stopAnm();

	mSelectionText = new TToggleControl(mScreen);
	mSelectionText->setupToggle(cLanguageToggleItems,
	                            ARRAY_COUNT(cLanguageToggleItems));

#ifdef VERSION_GMSP01
	loadTexture(mLanguageTextures, "select_language.bti");
#endif

	setState(STATE_INACTIVE);
}

s32 TOptionLanguageUnit::replaceTexture(u32 tag, JUTTexture* texture)
{
	int offset;
	J2DPicture* picture = (J2DPicture*)mScreen->search(tag);
	JUT_ASSERT(picture);
	offset = (texture->getWidth() - picture->getWidth()) / 2;
	picture->changeTexture(texture->getTexInfo(), 0);

	JUTRect rect(picture->getBounds());
	rect.reform(-offset, 0, offset, 0);
	picture->place(rect);

	return picture->getWidth();
}

void TOptionLanguageUnit::changeTexture(int language)
{
	int width     = replaceTexture('m_4', mLanguageTextures[language]);
	J2DPane* pane = mScreen->search('lan0' + getValue());
	int itemWidth = pane->getWidth();
	if (itemWidth > width)
		width = itemWidth;
	mSelectionBubble->resize(width);
}

void TOptionLanguageUnit::update()
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
		break;

	case STATE_INACTIVE:
		break;
	}
}

void TOptionLanguageUnit::toggle()
{
	mSelectionText->toggle();
	adjust();
	SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_SELECT_COMMON, 0, nullptr, 0);
}

void TOptionLanguageUnit::adjust() { int value = mSelectionText->getNumber(); }

void TOptionLanguageUnit::show() { }

void TOptionLanguageUnit::hide() { }

void TOptionLanguageUnit::deactivate(bool force)
{
	if (force)
		setState(TOptionLanguageUnit::STATE_INACTIVE);
	else
		setState(TOptionLanguageUnit::STATE_DEACTIVATING);
}

void TOptionLanguageUnit::activate()
{
	setState(TOptionLanguageUnit::STATE_ACTIVE);
}

#ifdef VERSION_GMSP01
void TOptionLanguageUnit::setValue(TOptionLanguageUnit::LanguageType type)
#else
void TOptionLanguageUnit::setValue(int type)
#endif
{
	mSelectionText->setNumber(type);
	adjust();
}

void TOptionLanguageUnit::setState(TOptionLanguageUnit::State state)
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

void TOptionLanguageUnit::setInfluencedAlphaRecursive(J2DPane* pane,
                                                      bool influenced_alpha)
{
	for (JSUTreeIterator<J2DPane> it = pane->getPaneTree()->getFirstChild();
	     it != pane->getPaneTree()->getEndChild(); ++it) {
		it->setInfluencedAlpha(influenced_alpha);
		setInfluencedAlphaRecursive(it.getObject(), influenced_alpha);
	}
}
#endif

TOptionSoundUnit::TOptionSoundUnit(J2DScreen* screen)
    : mScreen(screen)
{
	mParentPane   = new TExPane(mScreen, 'oya2');
	mInitialAlpha = mParentPane->getPane()->getAlpha();

#ifdef VERSION_GMSJ01
	unkC = new TBalloonControl(2);
	unkC->add(new TExPane(mScreen, 'bub3'));
	unkC->add(new TExPane(mScreen, 'bub4'));
	unkC->setupAnm();
#endif

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

#ifdef VERSION_GMSP01
	loadTexture(mSoundTextures, "select_sound.bti");
	char* names[3]
	    = { "select_stereo.bti", "select_mono.bti", "select_surround.bti" };
	for (int i = 0; i < 3; ++i)
		loadTexture(mSoundGridTextures[i], names[i]);
#endif

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

#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
s32 TOptionSoundUnit::replaceTexture(u32 tag, JUTTexture* texture)
{
	int offset;
	J2DPicture* picture = (J2DPicture*)mScreen->search(tag);
	JUT_ASSERT(picture);
	offset = (texture->getWidth() - picture->getWidth()) / 2;
	picture->changeTexture(texture->getTexInfo(), 0);

	JUTRect rect(picture->getBounds());
	rect.reform(-offset, 0, offset, 0);
	picture->place(rect);

	return picture->getWidth();
}

void TOptionSoundUnit::changeTexture(int language)
{
	int width = replaceTexture('m_2', mSoundTextures[language]);
	for (int i = 0; i < 3; ++i) {
		int itemWidth
		    = replaceTexture('sel2' + i, mSoundGridTextures[i][language]);
		if (itemWidth > width)
			width = itemWidth;
	}
	mSelectionBubble->resize(width);
}
#endif

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

void TOptionSoundUnit::adjust()
{
	adjustView();
	const FabricatedSoundSettings& setting
	    = cSoundSettings[mSelectionText->getNumber()];
	SMSGetMSound()->setOutputMode(setting.mOutputMode);
}

void TOptionSoundUnit::show() { }

void TOptionSoundUnit::hide() { stopSound(); }

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
#ifdef VERSION_GMSJ01
		unkC->stopAnm();
#endif
		stopSound();
		break;

	case STATE_DEACTIVATING:
		mParentPane->setPaneAlpha(30, 150, mInitialAlpha);
		setInfluencedAlphaRecursive(mParentPane->getPane(), true);
#ifdef VERSION_GMSJ01
		unkC->stopAnm();
#endif
		break;

	case STATE_ACTIVE:
		mParentPane->getPane()->setAlpha(mInitialAlpha);
		setInfluencedAlphaRecursive(mParentPane->getPane(), false);
		mSelectionBubble->startAnm();
#ifdef VERSION_GMSJ01
		unkC->setupAnm();
		unkC->startAnm();
#endif
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
		it->setInfluencedAlpha(flag);
		setInfluencedAlphaRecursive(it.getObject(), flag);
	}
}

void TOptionControl::load()
{
	JKRArchive* optionArch = (JKRArchive*)JKRFileLoader::getVolume("option");

#ifdef VERSION_GMSP01_NX
	u32 language = TFlagManager::getInstance()->getFlag(MSF_LANGUAGE);
	OSReport("===> LANGUAGE: %d\n", language);
	switch (language) {
	case 5:
		mScreen = new J2DSetScreen("option_jp.blo", optionArch);
		break;
	case 6:
		mScreen = new J2DSetScreen("option_us.blo", optionArch);
		break;
	case 7:
		mScreen = new J2DSetScreen("option_en.blo", optionArch);
		break;
	case 8:
		mScreen = new J2DSetScreen("option_ge.blo", optionArch);
		break;
	case 9:
		mScreen = new J2DSetScreen("option_fr.blo", optionArch);
		break;
	case 10:
		mScreen = new J2DSetScreen("option_sp.blo", optionArch);
		break;
	case 11:
		mScreen = new J2DSetScreen("option_it.blo", optionArch);
		break;
	default:
		mScreen = new J2DSetScreen("option_us.blo", optionArch);
		OSReport("===> Language overflow %d %s %d\n", language, "Option.cpp",
		         2538);
		break;
	}
#else
	mScreen = new J2DSetScreen("option.blo", optionArch);
#endif
	mScreen->setCullBack(GX_CULL_BACK);
#ifdef VERSION_GMSP01
	textBoxA = (J2DTextBox*)mScreen->search('m_0a');
	textBoxB = (J2DTextBox*)mScreen->search('m_0b');
	textBoxA->setFont((JUTFont*)gpSystemFont);
	textBoxB->setFont((JUTFont*)gpSystemFont);

	SMSMakeTextBuffer(textBoxA, 0x200);
	SMSMakeTextBuffer(textBoxB, 0x200);

	mMessageEntries[0] = JKRGetResource("/option/loadmessage_en.bmg");
	mMessageEntries[1] = JKRGetResource("/option/loadmessage_ge.bmg");
	mMessageEntries[2] = JKRGetResource("/option/loadmessage_fr.bmg");
	mMessageEntries[3] = JKRGetResource("/option/loadmessage_sp.bmg");
	mMessageEntries[4] = JKRGetResource("/option/loadmessage_it.bmg");
#else
	J2DTextBox* textBoxA = (J2DTextBox*)mScreen->search('m_0a');
	J2DTextBox* textBoxB = (J2DTextBox*)mScreen->search('m_0b');

	textBoxA->setFont((JUTFont*)gpSystemFont);
	textBoxB->setFont((JUTFont*)gpSystemFont);
#endif

#if defined(VERSION_GMSJ01) || defined(VERSION_GMSE01)
	mBackArrow = new TArrowControl(mScreen->search('yaji'));
#else
	mBackArrow
	    = new TArrowControl(mScreen, (J2DPicture*)mScreen->search('yaji'));
#endif

	mRumbleOption = new TOptionRumbleUnit(mScreen);
	mSoundOption  = new TOptionSoundUnit(mScreen);
#if !defined(VERSION_GMSJ01) && !defined(VERSION_GMSJ01_01)
	mSubtitleOption = new TOptionSubtitleUnit(mScreen);
#endif
#ifdef VERSION_GMSP01
	mLanguageOption = new TOptionLanguageUnit(mScreen);
#endif

	setType(SELECT_TYPE_RUMBLE_OPTION, true);
	loadSetting();
	mWasJumping = false;
#ifndef VERSION_GMSJ01
	mStickNeutral = true;
#endif
}

void TOptionControl::loadSetting()
{
	switch (TFlagManager::getInstance()->getFlag(MSF_SOUND_MODE)) {
	case TOptionSoundUnit::SOUND_TYPE_MONO:
		mSoundOption->setValue(TOptionSoundUnit::SOUND_TYPE_MONO);
		break;
	case TOptionSoundUnit::SOUND_TYPE_STEREO:
		mSoundOption->setValue(TOptionSoundUnit::SOUND_TYPE_STEREO);
		break;
	case TOptionSoundUnit::SOUND_TYPE_SURROUND:
		mSoundOption->setValue(TOptionSoundUnit::SOUND_TYPE_SURROUND);
		break;
	default:
		break;
	}

	switch (TFlagManager::getInstance()->getFlag(MSF_RUMBLE)) {
	case TOptionRumbleUnit::RUMBLE_TYPE_UNK0:
		mRumbleOption->setValue(TOptionRumbleUnit::RUMBLE_TYPE_UNK0);
		break;
	case TOptionRumbleUnit::RUMBLE_TYPE_UNK1:
		mRumbleOption->setValue(TOptionRumbleUnit::RUMBLE_TYPE_UNK1);
		break;
	default:
		break;
	}

#if !defined(VERSION_GMSJ01) && !defined(VERSION_GMSJ01_01)
	switch (TFlagManager::getInstance()->getFlag(MSF_SUBTITLES)) {
	case TOptionSubtitleUnit::SUBTITLE_TYPE_UNK0:
		mSubtitleOption->setValue(TOptionSubtitleUnit::SUBTITLE_TYPE_UNK0);
		break;
	case TOptionSubtitleUnit::SUBTITLE_TYPE_UNK1:
		mSubtitleOption->setValue(TOptionSubtitleUnit::SUBTITLE_TYPE_UNK1);
		break;
	default:
		break;
	}
#endif

#ifdef VERSION_GMSP01
	switch (TFlagManager::getInstance()->getFlag(MSF_LANGUAGE)) {
	case TOptionLanguageUnit::LANGUAGE_TYPE_UNK0:
		mLanguageOption->setValue(TOptionLanguageUnit::LANGUAGE_TYPE_UNK0);
		break;
	case TOptionLanguageUnit::LANGUAGE_TYPE_UNK1:
		mLanguageOption->setValue(TOptionLanguageUnit::LANGUAGE_TYPE_UNK1);
		break;
	case TOptionLanguageUnit::LANGUAGE_TYPE_UNK2:
		mLanguageOption->setValue(TOptionLanguageUnit::LANGUAGE_TYPE_UNK2);
		break;
	case TOptionLanguageUnit::LANGUAGE_TYPE_UNK3:
		mLanguageOption->setValue(TOptionLanguageUnit::LANGUAGE_TYPE_UNK3);
		break;
	case TOptionLanguageUnit::LANGUAGE_TYPE_UNK4:
		mLanguageOption->setValue(TOptionLanguageUnit::LANGUAGE_TYPE_UNK4);
		break;
	default:
		break;
	}
#endif

	resetChangedSetting();
}

void TOptionControl::movementCommon()
{
	mBackArrow->update();
	mRumbleOption->update();
	mSoundOption->update();
#if !defined(VERSION_GMSJ01) && !defined(VERSION_GMSJ01_01)
	mSubtitleOption->update();
#endif
#ifdef VERSION_GMSP01
	mLanguageOption->update();
#endif
}

void TOptionControl::draw(J2DOrthoGraph* graph) { mScreen->draw(0, 0, graph); }

// mario walks from the card select screen to the options screen
bool TOptionControl::movementCard2Option()
{
	if (gpCameraOption->unk12 == 0) {
		mRumbleOption->show();

#if !defined(VERSION_GMSJ01) && !defined(VERSION_GMSJ01_01)
		mScreen->search('txp2')->show();
#endif
#ifdef VERSION_GMSP01
		mScreen->search('txp3')->show();
#endif

		mScreen->search('oya0')->show();
		mScreen->search('oya1')->show();
		mScreen->search('oya2')->show();

		mWasJumping = false;
		setType(mSelectedOption, true);
#ifdef VERSION_GMSP01
		changeTexture();
#endif

		return true;
	}

	return false;
}

bool TOptionControl::movementOption()
{
	movementCommon();
	checkInput();
	writeValue();

	if (gpCameraOption->unk0 & 1) {
		mSoundOption->hide();
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

		isChangedSetting();

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
#if !defined(VERSION_GMSJ01) && !defined(VERSION_GMSJ01_01)
			mSubtitleOption->deactivate(initial_options_entry);
#endif
#ifdef VERSION_GMSP01
			mLanguageOption->deactivate(initial_options_entry);
#endif
			break;

		case SELECT_TYPE_SOUND_OPTION:
			mRumbleOption->deactivate(initial_options_entry);
			mSoundOption->activate();
#if !defined(VERSION_GMSJ01) && !defined(VERSION_GMSJ01_01)
			mSubtitleOption->deactivate(initial_options_entry);
#endif
#ifdef VERSION_GMSP01
			mLanguageOption->deactivate(initial_options_entry);
#endif
			break;

#if !defined(VERSION_GMSJ01) && !defined(VERSION_GMSJ01_01)
		case SELECT_TYPE_SUBTITLE_OPTION:
			mSubtitleOption->activate();
			mRumbleOption->deactivate(initial_options_entry);
			mSoundOption->deactivate(initial_options_entry);
#ifdef VERSION_GMSP01
			mLanguageOption->deactivate(initial_options_entry);
#endif
			break;
#endif

#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
		case SELECT_TYPE_LANGUAGE_OPTION:
#ifdef VERSION_GMSP01
			mLanguageOption->activate();
#endif
			mSubtitleOption->deactivate(initial_options_entry);
			mRumbleOption->deactivate(initial_options_entry);
			mSoundOption->deactivate(initial_options_entry);
			break;
#endif
		}

		if (!initial_options_entry) {
			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_CURSOR_COMMON, 0,
			                                   nullptr, 0);
		}
	}
}

#ifdef VERSION_GMSP01
void TOptionControl::changeTexture()
{
	int language = mLanguageOption->getValue();
	mBackArrow->changeTexture(language);
	mRumbleOption->changeTexture(language);
	mSoundOption->changeTexture(language);
	mSubtitleOption->changeTexture(language);
	mLanguageOption->changeTexture(language);
	changeTopMessage(language);
}

void TOptionControl::changeTopMessage(int language)
{
	char buffer[0x200];

	bool outOfRange = true;
	if (language >= 0
	    && language < sizeof(mMessageEntries) / sizeof(mMessageEntries[0]))
		outOfRange = false;
	if (outOfRange)
		language = 0;

	const char* src = SMSGetMessageData(mMessageEntries[language], 0x1C);
	char* dst       = buffer;

#define ESC "\x1B"
#define OPTION_HIGHLIGHT(color, mark)                                          \
	ESC "GM[0]" ESC "CC[" color "]" ESC "FX[26]" ESC "FY[26]" ESC "SH[3]" ESC  \
	    "CD[4]" mark ESC "GM[0]" ESC "CC" ESC "FX" ESC "FY" ESC "SH" ESC       \
	    "CU[4]"

	while (char c = *src) {
		if (c == '@') {
			strncpy(dst, OPTION_HIGHLIGHT("64ff64", "@"),
			        sizeof(OPTION_HIGHLIGHT("64ff64", "@")) - 1);
			dst += sizeof(OPTION_HIGHLIGHT("64ff64", "@")) - 1;
		} else if (c == '*') {
			strncpy(dst, OPTION_HIGHLIGHT("dcdcdc", "*"),
			        sizeof(OPTION_HIGHLIGHT("dcdcdc", "*")) - 1);
			dst += sizeof(OPTION_HIGHLIGHT("dcdcdc", "*")) - 1;
		} else {
			*dst = c;
			dst++;
		}
		src++;
	}
	*dst = '\0';

#undef OPTION_HIGHLIGHT
#undef ESC

	strncpy(textBoxA->getStringPtr(), buffer, sizeof(buffer));
	strncpy(textBoxB->getStringPtr(), buffer, sizeof(buffer));
}
#endif

void TOptionControl::toggleCurType()
{
	switch (mSelectedOption) {
	case SELECT_TYPE_RUMBLE_OPTION:
		mRumbleOption->toggle();
		break;
	case SELECT_TYPE_SOUND_OPTION:
		mSoundOption->toggle();
		break;
#if !defined(VERSION_GMSJ01) && !defined(VERSION_GMSJ01_01)
	case SELECT_TYPE_SUBTITLE_OPTION:
		mSubtitleOption->toggle();
		break;
#endif
#ifdef VERSION_GMSP01
	case SELECT_TYPE_LANGUAGE_OPTION:
		mLanguageOption->toggle();
		changeTexture();
		break;
#endif
	}
}

void TOptionControl::checkInput()
{
	f32 fVar1
	    = SMSGetMarDirector()->getGamePad()->getMainStickInDir(0.0f, 1.0f);
#if defined(VERSION_GMSJ01) || defined(VERSION_GMSJ01_01)
	if (0.75f <= fVar1) {
		setType(SELECT_TYPE_RUMBLE_OPTION, false);
	} else if (fVar1 <= -0.75f) {
		setType(SELECT_TYPE_SOUND_OPTION, false);
	}
#elif defined(VERSION_GMSE01)
	SelectType selected = mSelectedOption;
	if (0.75f <= fVar1) {
		if (mStickNeutral) {
			mStickNeutral = false;
			switch (selected) {
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
			switch (selected) {
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
#elif defined(VERSION_GMSP01)
	SelectType selected = mSelectedOption;
	if (0.75f <= fVar1) {
		if (mStickNeutral) {
			mStickNeutral = false;
			switch (selected) {
			case SELECT_TYPE_RUMBLE_OPTION:
			case SELECT_TYPE_SOUND_OPTION:
				setType(SELECT_TYPE_RUMBLE_OPTION, false);
				break;
			case SELECT_TYPE_SUBTITLE_OPTION:
				setType(SELECT_TYPE_SOUND_OPTION, false);
				break;
			case SELECT_TYPE_LANGUAGE_OPTION:
				setType(SELECT_TYPE_SUBTITLE_OPTION, false);
				break;
			}
		}
	} else if (fVar1 <= -0.75f) {
		if (mStickNeutral) {
			mStickNeutral = false;
			switch (selected) {
			case SELECT_TYPE_RUMBLE_OPTION:
				setType(SELECT_TYPE_SOUND_OPTION, false);
				break;
			case SELECT_TYPE_SOUND_OPTION:
				setType(SELECT_TYPE_SUBTITLE_OPTION, false);
				break;
			case SELECT_TYPE_SUBTITLE_OPTION:
			case SELECT_TYPE_LANGUAGE_OPTION:
				setType(SELECT_TYPE_LANGUAGE_OPTION, false);
				break;
			}
		}
	} else {
		mStickNeutral = true;
	}
#elif defined(VERSION_GMSP01_NX)
	SelectType selected = mSelectedOption;
	if (0.75f <= fVar1
	    || SMSGetMarDirector()->getGamePad()->getButton()
	           & JUTGamePad::DPAD_UP) {
		if (mStickNeutral) {
			mStickNeutral = false;
			switch (selected) {
			case SELECT_TYPE_RUMBLE_OPTION:
			case SELECT_TYPE_SOUND_OPTION:
				setType(SELECT_TYPE_RUMBLE_OPTION, false);
				break;
			case SELECT_TYPE_SUBTITLE_OPTION:
				setType(SELECT_TYPE_RUMBLE_OPTION, false);
				break;
			case SELECT_TYPE_LANGUAGE_OPTION:
				setType(SELECT_TYPE_SUBTITLE_OPTION, false);
				break;
			}
		}
	} else if (fVar1 <= -0.75f
	           || SMSGetMarDirector()->getGamePad()->getButton()
	                  & JUTGamePad::DPAD_DOWN) {
		if (mStickNeutral) {
			mStickNeutral = false;
			switch (selected) {
			case SELECT_TYPE_RUMBLE_OPTION:
				setType(SELECT_TYPE_SUBTITLE_OPTION, false);
				break;
			case SELECT_TYPE_SOUND_OPTION:
				setType(SELECT_TYPE_SUBTITLE_OPTION, false);
				break;
			case SELECT_TYPE_SUBTITLE_OPTION:
			case SELECT_TYPE_LANGUAGE_OPTION:
				setType(SELECT_TYPE_SUBTITLE_OPTION, false);
				break;
			}
		}
	} else {
		mStickNeutral = true;
	}
#endif

	bool jumping = SMS_IsMarioStatusTypeJumping();
	if (!mWasJumping && jumping)
		toggleCurType();
	mWasJumping = jumping;
}

void TOptionControl::writeValue()
{
	TFlagManager::getInstance()->setFlag(MSF_RUMBLE, mRumbleOption->getValue());
	TFlagManager::getInstance()->setFlag(MSF_SOUND_MODE,
	                                     mSoundOption->getValue());
#if !defined(VERSION_GMSJ01) && !defined(VERSION_GMSJ01_01)
	TFlagManager::getInstance()->setFlag(MSF_SUBTITLES,
	                                     mSubtitleOption->getValue());
#endif
#ifdef VERSION_GMSP01
	TFlagManager::getInstance()->setFlag(MSF_LANGUAGE,
	                                     mLanguageOption->getValue());
#endif
}

bool TOptionControl::isChangedSetting() const
{
	bool result = true;

#if defined(VERSION_GMSJ01) || defined(VERSION_GMSJ01_01)
	if (mInitialRumbleValue == mRumbleOption->getValue()
	    && mInitialSoundValue == mSoundOption->getValue())
		result = false;
#else
	bool changed = true;
#ifdef VERSION_GMSP01
	bool changedBase = true;
	if (mInitialRumbleValue == mRumbleOption->getValue()
	    && mInitialSoundValue == mSoundOption->getValue())
		changedBase = false;
	if (!changedBase && mInitialSubtitleValue == mSubtitleOption->getValue())
		changed = false;
	if (!changed && mInitialLanguageValue == mLanguageOption->getValue())
		result = false;
#else
	if (mInitialRumbleValue == mRumbleOption->getValue()
	    && mInitialSoundValue == mSoundOption->getValue())
		changed = false;
	if (!changed && mInitialSubtitleValue == mSubtitleOption->getValue())
		result = false;
#endif
#endif

	return result;
}

void TOptionControl::resetChangedSetting()
{
	mInitialRumbleValue = mRumbleOption->getValue();
	mInitialSoundValue  = mSoundOption->getValue();
#if !defined(VERSION_GMSJ01) && !defined(VERSION_GMSJ01_01)
	mInitialSubtitleValue = mSubtitleOption->getValue();
#endif
#ifdef VERSION_GMSP01
	mInitialLanguageValue = mLanguageOption->getValue();
#endif
}

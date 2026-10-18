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

#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
// prefix for path
static const char* langArray[] = { "", "ge/", "fr/", "sp/", "it/" };
#endif

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

#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
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

#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
const u32 cLanguageToggleItems[] = {
	'lan0', 'lan1', 'lan2', 'lan3', 'lan4',
};

#pragma dont_inline on
void loadTexture(JUTTexture** out_textures, char* path)
{
	char pathBuffer[255];

	for (int i = 0; i < ARRAY_COUNT(langArray); i++) {
		snprintf(pathBuffer, sizeof(pathBuffer), "/option/timg/%s%s",
		         langArray[i], path);
		JUTTexture* texture = new JUTTexture(
		    (const ResTIMG*)JKRFileLoader::getGlbResource(pathBuffer));
		out_textures[i] = texture;
	}
}
#pragma dont_inline off
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

#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
#pragma dont_inline on
s32 TArrowControl::replaceTexture(u32 tag, JUTTexture* texture)
{
	J2DPicture* picture = (J2DPicture*)mPane->search(tag);
	s32 widthDiff       = texture->getWidth() - picture->getWidth();

	picture->changeTexture(texture->getTexInfo(), 0);

	JUTRect rect(picture->getBounds());
	rect.reform(0, 0, widthDiff, 0);
	picture->place(rect);

	return picture->getWidth();
}
#pragma dont_inline off
void TArrowControl::changeTexture(int) { }
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
	mPane->setAlpha(
	    JGeometry::TUtil<s32>::clamp(iVar3 * 8 + mPane->getAlpha(), 0, 255));
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
	int iVar3 = phase < 50 ? phase : 100 - phase;
	f32 fVar1 = iVar3 / 50.0f;
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
void TPaneScalingControl::resize(int) { }
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
	int iVar10 = mInitialBounds.getWidth();
	int iVar5  = mInitialBounds.getHeight();

	f32 fVar2 = mAmplitude
	            * MsSin(RAD_TO_DEG((f32)mFrameCtrl.getFrame()
	                               / (f32)mFrameCtrl.getEnd() * (2 * M_PI)));

	int uVar6 = fVar2 * iVar10;
	int uVar1 = fVar2 * iVar5;

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
#ifdef VERSION_GMSJ01
	hide();
#elif defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
	for (const TAnmChunk* it = mChunks.begin(); it != mChunks.end(); ++it) {
		mScreen->search(it->mTag)->hide();
	}
#else
	// @TODO: other version
#endif
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
    , mParentPane(nullptr)
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

#ifdef VERSION_GMSJ01
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

#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
	// load rumble textures for PAL?
	char pathBuffer[255];

	for (int i = 0; i < ARRAY_COUNT(mRumbleTextures); i++) {
		snprintf(pathBuffer, sizeof(pathBuffer), "/option/timg/%s%s",
		         langArray[i], "sel1sel0");
		mRumbleTextures[i] = new JUTTexture(
		    (const ResTIMG*)JKRFileLoader::getGlbResource(pathBuffer));
	}

	const char* rumbleFileNames[2] = { "select_on.bti", "select_off.bti" };

	for (int row = 0; row < 2; row++) {
		for (int col = 0; col < 5; col++) {
			snprintf(pathBuffer, sizeof(pathBuffer), "/option/timg/%s%s",
			         langArray[col], rumbleFileNames[row]);
			mRumbleGridTextures[row][col] = new JUTTexture(
			    (const ResTIMG*)JKRFileLoader::getGlbResource(pathBuffer));
		}
	}
#endif

	setState(STATE_INACTIVE);
}

#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
s32 TOptionRumbleUnit::replaceTexture(u32 tag, JUTTexture* texture)
{
	J2DPicture* picture = (J2DPicture*)mParentPane->getPane()->search(tag);
	s32 widthDiff       = texture->getWidth() - picture->getWidth();

	picture->changeTexture(texture->getTexInfo(), 0);

	JUTRect rect(picture->getBounds());
	rect.reform(0, 0, widthDiff, 0);
	picture->place(rect);

	return picture->getWidth();
}
void TOptionRumbleUnit::changeTexture(int arg0)
{
	JUTTexture* langTexture = mRumbleTextures[arg0];
	J2DPicture* picM1       = (J2DPicture*)mScreen->search('m_1');

	s32 widthOffset = (langTexture->getWidth() - picM1->getWidth()) / 2;

	picM1->changeTexture(langTexture->getTexInfo(), 0);

	JUTRect sp110 = picM1->getBounds();
	sp110.reform(-widthOffset, 0, widthOffset, 0);
	picM1->place(sp110);

	s32 maxTextWidth = replaceTexture('m_1', mRumbleTextures[arg0]);

	for (int row = 0; row < 2; row++) {
		s32 currentWidth
		    = replaceTexture('sel0' + row, mRumbleGridTextures[row][arg0]);
		if (currentWidth > maxTextWidth) {
			maxTextWidth = currentWidth;
		}
	}

	// if you inline this the accuracy tanks
	TPaneScalingControl* scalingCtrl = mSelectionBubble;

	s32 bubbleOffset
	    = ((maxTextWidth + 30) - scalingCtrl->mInitialBounds.getWidth()) / 2;
	scalingCtrl->mInitialBounds.reform(-bubbleOffset, 0, bubbleOffset, 0);

	scalingCtrl->update();
	scalingCtrl->mFrameCtrl.update();
}
#endif

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
#ifdef VERSION_GMSJ01
		// why were these removed for PAL??
		mGamepadIcon[getValue()]->update();
		checkRumble();
#endif
		break;

	case STATE_INACTIVE:
		break;
	}
}
#pragma dont_inline off

#ifdef VERSION_GMSJ01
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
#else
void TOptionRumbleUnit::checkRumble() { }
#endif

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
#ifdef VERSION_GMSJ01
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

#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
TOptionSubtitleUnit::TOptionSubtitleUnit(J2DScreen* screen)
    : mScreen(screen)
    , mParentPane(nullptr)
    , mInitialAlpha(0)
    , mSelectionBubble(nullptr)
    , mSelectionText(nullptr)
    , mState(STATE_INACTIVE)
{
	mParentPane      = new TExPane(mScreen, 'txp2');
	mInitialAlpha    = mParentPane->getPane()->getAlpha();
	mSelectionBubble = new TPaneScalingControl(mScreen->search('me_2'));
	mSelectionBubble->setupAnm(0.05f, 1.0f);
	mSelectionBubble->stopAnm();

	static const u32 cSubToggleItems[] = { 'sel5', 'sel6' };
	mSelectionText                     = new TToggleControl(mScreen);
	mSelectionText->setupToggle(cSubToggleItems, ARRAY_COUNT(cSubToggleItems));

	char pathBuffer[255];

	for (int i = 0; i < ARRAY_COUNT(mSubtitleTexture); i++) {
		snprintf(pathBuffer, sizeof(pathBuffer), "/option/timg/%s%s",
		         langArray[i], "sub_main_suffix");
		mSubtitleTexture[i] = new JUTTexture(
		    (const ResTIMG*)JKRFileLoader::getGlbResource(pathBuffer));
	}

	const char* subFileNames[2] = { "sub_grid_0", "sub_grid_1" };

	for (int row = 0; row < 2; row++) {
		for (int col = 0; col < 5; col++) {
			snprintf(pathBuffer, sizeof(pathBuffer), "/option/timg/%s%s",
			         langArray[col], subFileNames[row]);
			mSubtitleGridTextures[row][col] = new JUTTexture(
			    (const ResTIMG*)JKRFileLoader::getGlbResource(pathBuffer));
		}
	}

	setState(STATE_INACTIVE);
	mParentPane->getPane()->setAlpha(150);
	// @NOTE: this is getting inlined...
	setInfluencedAlphaRecursive(mParentPane->getPane(), true);

	mSelectionBubble->stopAnm();
}

s32 TOptionSubtitleUnit::replaceTexture(u32 tag, JUTTexture* texture)
{
	J2DPicture* picture = (J2DPicture*)mParentPane->getPane()->search(tag);
	s32 widthDiff       = texture->getWidth() - picture->getWidth();

	picture->changeTexture(texture->getTexInfo(), 0);

	JUTRect rect(picture->getBounds());
	rect.reform(0, 0, widthDiff, 0);
	picture->place(rect);

	return picture->getWidth();
}

void TOptionSubtitleUnit::changeTexture(int arg0)
{
	JUTTexture* langTexture = mSubtitleTexture[arg0];
	J2DPicture* picM1       = (J2DPicture*)mScreen->search('m_3');

	s32 widthOffset = (langTexture->getWidth() - picM1->getWidth()) / 2;

	picM1->changeTexture(langTexture->getTexInfo(), 0);

	JUTRect sp110 = picM1->getBounds();
	sp110.reform(-widthOffset, 0, widthOffset, 0);
	picM1->place(sp110);

	s32 maxTextWidth = picM1->getWidth();

	for (int row = 0; row < 2; row++) {
		s32 currentWidth
		    = replaceTexture('sel0' + row, mSubtitleGridTextures[row][arg0]);
		if (currentWidth > maxTextWidth) {
			maxTextWidth = currentWidth;
		}
	}

	// if you inline this the accuracy tanks
	TPaneScalingControl* scalingCtrl = mSelectionBubble;

	s32 bubbleOffset
	    = ((maxTextWidth + 30) - scalingCtrl->mInitialBounds.getWidth()) / 2;
	scalingCtrl->mInitialBounds.reform(-bubbleOffset, 0, bubbleOffset, 0);

	scalingCtrl->update();
	scalingCtrl->mFrameCtrl.update();
}

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
void TOptionSubtitleUnit::setValue(SubtitleType) { }
void TOptionSubtitleUnit::setState(State state)
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

	char pathBuffer[255];
	for (int i = 0; i < ARRAY_COUNT(mLanguageTexture); i++) {
		snprintf(pathBuffer, sizeof(pathBuffer), "/option/timg/%s%s",
		         langArray[i], "select_language.bti");
		mLanguageTexture[i] = new JUTTexture(
		    (const ResTIMG*)JKRFileLoader::getGlbResource(pathBuffer));
	}

	setState(STATE_INACTIVE);

	mParentPane->getPane()->setAlpha(150);
	setInfluencedAlphaRecursive(mParentPane->getPane(), true);

	mSelectionBubble->stopAnm();
}

#pragma dont_inline on
s32 TOptionLanguageUnit::replaceTexture(u32 tag, JUTTexture* texture)
{
	J2DPicture* picture = (J2DPicture*)mParentPane->getPane()->search(tag);
	s32 widthDiff       = texture->getWidth() - picture->getWidth();

	picture->changeTexture(texture->getTexInfo(), 0);

	JUTRect rect(picture->getBounds());
	rect.reform(0, 0, widthDiff, 0);
	picture->place(rect);

	return picture->getWidth();
}
#pragma dont_inline off

void TOptionLanguageUnit::changeTexture(int arg0)
{
	J2DPicture* picMain = (J2DPicture*)mScreen->search('m_3');
	JUTTexture* subTex  = mLanguageTexture[arg0];

	int mainWidthOffset = (subTex->getWidth() - (picMain->getWidth())) / 2;
	picMain->changeTexture(subTex->getTexInfo(), 0);

	JUTRect rectMain = picMain->getBounds();
	rectMain.reform(-mainWidthOffset, 0, mainWidthOffset, 0);

	int maxGridWidth = picMain->getWidth();

	for (int row = 0; row < 2; row++) {
		int gridWidthDelta
		    = replaceTexture(row + 'sel5', mLanguageGridTextures[row][arg0]);
		if (gridWidthDelta > maxGridWidth) {
			maxGridWidth = gridWidthDelta;
		}
	}

	TPaneScalingControl* scalingCtrl = mSelectionBubble;
	s32 bubbleOffset
	    = ((maxGridWidth + 30) - scalingCtrl->mInitialBounds.getWidth()) / 2;
	scalingCtrl->mInitialBounds.reform(-bubbleOffset, 0, bubbleOffset, 0);

	scalingCtrl->update();
	scalingCtrl->mFrameCtrl.update();
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
	// adjustSound();
}
void TOptionLanguageUnit::adjust() { }
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
void TOptionLanguageUnit::setValue(LanguageType) { }
void TOptionLanguageUnit::setState(State state)
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

#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
	char pathBuffer[255];

	for (int i = 0; i < 5; i++) {
		snprintf(pathBuffer, sizeof(pathBuffer), "/option/timg/%s%s",
		         langArray[i], "select_sound.bti");
		mSoundTextures[i] = new JUTTexture(
		    (const ResTIMG*)JKRFileLoader::getGlbResource(pathBuffer));
	}

	const char* soundFileNames[]
	    = { "select_stereo.bti", "select_mono.bti", "select_surround.bti" };

	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 5; col++) {
			snprintf(pathBuffer, sizeof(pathBuffer), "/option/timg/%s%s",
			         langArray[col], soundFileNames[row]);
			mSoundGridTextures[row][col] = new JUTTexture(
			    (const ResTIMG*)JKRFileLoader::getGlbResource(pathBuffer));
		}
	}
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
	J2DPicture* picture = (J2DPicture*)mParentPane->getPane()->search(tag);
	s32 widthDiff       = texture->getWidth() - picture->getWidth();

	picture->changeTexture(texture->getTexInfo(), 0);

	JUTRect rect(picture->getBounds());
	rect.reform(0, 0, widthDiff, 0);
	picture->place(rect);

	return picture->getWidth();
}

void TOptionSoundUnit::changeTexture(int arg0)
{
	int maxGridWidth = replaceTexture('m_3', mSoundTextures[arg0]);

	for (int row = 0; row < 2; row++) {
		u32 paneTag = row + 'sel5';
		int gridWidthDelta
		    = replaceTexture(paneTag, mSoundGridTextures[row][arg0]);
		if (gridWidthDelta > maxGridWidth) {
			maxGridWidth = gridWidthDelta;
		}
	}

	TPaneScalingControl* scalingCtrl = mSelectionBubble;
	s32 bubbleOffset
	    = ((maxGridWidth + 30) - scalingCtrl->mInitialBounds.getWidth()) / 2;
	scalingCtrl->mInitialBounds.reform(-bubbleOffset, 0, bubbleOffset, 0);

	scalingCtrl->update();
	scalingCtrl->mFrameCtrl.update();
}

#endif

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
		it->setInfluencedAlpha(flag);
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

#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
	SMSMakeTextBuffer(textBoxA, 0x200);
	SMSMakeTextBuffer(textBoxB, 0x200);

	mMessageEntries[0]
	    = JKRFileLoader::getGlbResource("/option/loadmessage_en.bmg");
	mMessageEntries[1]
	    = JKRFileLoader::getGlbResource("/option/loadmessage_ge.bmg");
	mMessageEntries[2]
	    = JKRFileLoader::getGlbResource("/option/loadmessage_fr.bmg");
	mMessageEntries[3]
	    = JKRFileLoader::getGlbResource("/option/loadmessage_sp.bmg");
	mMessageEntries[4]
	    = JKRFileLoader::getGlbResource("/option/loadmessage_it.bmg");

	mBackArrow
	    = new TArrowControl(mScreen, (J2DPicture*)mScreen->search('yaji'));
	loadTexture(mBackArrow->mTextures, "select_quit.bti");
#endif

#ifdef VERSION_GMSJ01
	mBackArrow = new TArrowControl(mScreen->search('yaji'));
#endif

	mRumbleOption = new TOptionRumbleUnit(mScreen);
	mSoundOption  = new TOptionSoundUnit(mScreen);

#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
	mSubtitleOption = new TOptionSubtitleUnit(mScreen);
	mLanguageOption = new TOptionLanguageUnit(mScreen);
#endif

	setType(SELECT_TYPE_RUMBLE_OPTION, true);
	loadSetting();
	mWasJumping = false;
}

#pragma dont_inline on
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
	}

	switch (TFlagManager::getInstance()->getFlag(MSF_RUMBLE)) {
	case TOptionRumbleUnit::RUMBLE_TYPE_UNK0:
		mRumbleOption->setValue(TOptionRumbleUnit::RUMBLE_TYPE_UNK0);
		break;
	case TOptionRumbleUnit::RUMBLE_TYPE_UNK1:
		mRumbleOption->setValue(TOptionRumbleUnit::RUMBLE_TYPE_UNK1);
		break;
	}

#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
	switch (TFlagManager::getInstance()->getFlag(MSF_SUBTITLES)) {
	case TOptionSubtitleUnit::SUBTITLE_TYPE_UNK0:
		mSubtitleOption->setValue(TOptionSubtitleUnit::SUBTITLE_TYPE_UNK0);
		break;
	case TOptionSubtitleUnit::SUBTITLE_TYPE_UNK1:
		mSubtitleOption->setValue(TOptionSubtitleUnit::SUBTITLE_TYPE_UNK1);
		break;
	}

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
	}
#endif

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

#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
		mScreen->search('txp2')->show();
		mScreen->search('txp3')->show();
#endif

		mScreen->search('oya0')->show();
		mScreen->search('oya1')->show();
		mScreen->search('oya2')->show();

		mWasJumping = false;
		setType(mSelectedOption, true);

#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
		TOptionLanguageUnit::LanguageType langId = mLanguageOption->getValue();
		mBackArrow->replaceTexture('s_1', mBackArrow->mTextures[langId]);

		mRumbleOption->changeTexture(langId);
		mSoundOption->changeTexture(langId);
		mSubtitleOption->changeTexture(langId);

		s32 textWidth = mLanguageOption->replaceTexture(
		    'm_4', mLanguageOption->mLanguageTexture[langId]);

		J2DPane* langPane = mLanguageOption->mScreen->search('lan0' + langId);
		if (langPane != nullptr) {
			s32 paneWidth = (langPane->getBounds().getWidth());
			if (paneWidth > textWidth) {
				textWidth = paneWidth;
			}

			f32 scaleOffset = ((textWidth + 30)
			                   - mLanguageOption->mSelectionBubble
			                         ->mInitialBounds.getWidth())
			                  / 2.0f;
			mLanguageOption->mSelectionBubble->mInitialBounds.reform(
			    -scaleOffset, 0, scaleOffset, 0);
			mLanguageOption->mSelectionBubble->update();
		}
		changeTopMessage(langId);
#endif

		return true;
	}

	return false;
}

bool TOptionControl::movementOption()
{
	mBackArrow->update();
#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
	updateOptions();
#else
	mRumbleOption->update();
	mSoundOption->update();
#endif
	checkInput();
	writeValue();

	// this has to be an inline on god
	if (gpCameraOption->unk0 & 1) {
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
#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
			mSubtitleOption->deactivate(initial_options_entry);
			mLanguageOption->deactivate(initial_options_entry);
#endif
			break;

		case SELECT_TYPE_SOUND_OPTION:
			mRumbleOption->deactivate(initial_options_entry);
			mSoundOption->activate();
#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
			mSubtitleOption->deactivate(initial_options_entry);
			mLanguageOption->deactivate(initial_options_entry);
#endif
			break;

#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
		case SELECT_TYPE_SUBTITLE_OPTION:
			mSubtitleOption->activate();
			mRumbleOption->deactivate(initial_options_entry);
			mSoundOption->deactivate(initial_options_entry);
			mLanguageOption->deactivate(initial_options_entry);
			break;

		case SELECT_TYPE_LANGUAGE_OPTION:
			mLanguageOption->activate();
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

#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
// fabricated
void TOptionControl::updateOptions()
{
	mRumbleOption->update();
	mSoundOption->update();
	mSubtitleOption->update();
	mLanguageOption->update();
}
void TOptionControl::changeTexture() { }
void TOptionControl::changeTopMessage(int langId)
{
	char stringBuffer[0x200];

	BOOL valid = TRUE;
	if (langId >= 0 && langId < 5) {
		valid = FALSE;
	}
	// this seems to be intentional for whatever reason? the s32 temporary is
	// allocated and used in this manner...
	if (valid) {
		langId = TOptionLanguageUnit::LANGUAGE_TYPE_UNK0;
	}

	const char* src = SMSGetMessageData(mMessageEntries[langId], 0x1CU);
	char* dst       = stringBuffer;

// the string is intentionally written this way
#define ESC "\x1B"
#define kJSYSTEM_SEQ                                                           \
	ESC "GM[0]" ESC "CC[64ff64]" ESC "FX[26]" ESC "FY[26]" ESC "SH[3]" ESC     \
	    "CD[4]@" ESC "GM[0]" ESC "CC" ESC "FX" ESC "FY" ESC "SH" ESC "CU[4]"

	while (char current = *src) {
		switch (current) {
		case '@':
			strncpy(dst, kJSYSTEM_SEQ, sizeof(kJSYSTEM_SEQ) - 1);
			dst += sizeof(kJSYSTEM_SEQ) - 1;
			break;
		case '*':
			strncpy(dst, kJSYSTEM_SEQ, sizeof(kJSYSTEM_SEQ) - 1);
			dst += sizeof(kJSYSTEM_SEQ) - 1;
			break;
		default:
			*dst = current;
			dst += 1;
			break;
		}
		src++;
	}
	*dst = 0;

#undef kJSYSTEM_SEQ

	strncpy(unk28->getStringPtr(), stringBuffer, sizeof(stringBuffer));
	strncpy(unk2C->getStringPtr(), stringBuffer, sizeof(stringBuffer));
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
#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
	case SELECT_TYPE_SUBTITLE_OPTION:
		mSubtitleOption->toggle();
		break;
	case SELECT_TYPE_LANGUAGE_OPTION:
		mLanguageOption->toggle();
		TOptionLanguageUnit::LanguageType langId = mLanguageOption->getValue();
		mBackArrow->replaceTexture('s_1', mBackArrow->mTextures[langId]);

		mRumbleOption->changeTexture(langId);
		mSoundOption->changeTexture(langId);
		mSubtitleOption->changeTexture(langId);

		s32 textWidth = mLanguageOption->replaceTexture(
		    'm_4', mLanguageOption->mLanguageTexture[langId]);

		J2DPane* langPane = mLanguageOption->mScreen->search('lan0' + langId);
		if (langPane != nullptr) {
			s32 paneWidth = (langPane->getBounds().getWidth());
			if (paneWidth > textWidth) {
				textWidth = paneWidth;
			}

			f32 scaleOffset = ((textWidth + 30)
			                   - mLanguageOption->mSelectionBubble
			                         ->mInitialBounds.getWidth())
			                  / 2.0f;
			mLanguageOption->mSelectionBubble->mInitialBounds.reform(
			    -scaleOffset, 0, scaleOffset, 0);
			mLanguageOption->mSelectionBubble->update();
		}
		changeTopMessage(langId);
		break;
#endif
	}
}

void TOptionControl::checkInput()
{
	f32 fVar1
	    = SMSGetMarDirector()->getGamePad()->getMainStickInDir(0.0f, 1.0f);
	if (fVar1 >= 0.75f) {
#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
		if (mStickNeutral) {
			mStickNeutral = false;
			switch (mSelectedOption) {
			case 2:
				setType(SELECT_TYPE_SOUND_OPTION, false);
				break;
			case 3:
				setType(SELECT_TYPE_SUBTITLE_OPTION, false);
				break;
			default:
				setType(SELECT_TYPE_RUMBLE_OPTION, false);
				break;
			}
		}
#else
		setType(SELECT_TYPE_RUMBLE_OPTION, false);
#endif
	} else if (fVar1 <= -0.75f) {

#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
		if (mStickNeutral) {
			mStickNeutral = false;
			switch (mSelectedOption) {
			case 2:
				setType(SELECT_TYPE_SOUND_OPTION, false);
				break;
			case 3:
				setType(SELECT_TYPE_SUBTITLE_OPTION, false);
				break;
			default:
				setType(SELECT_TYPE_LANGUAGE_OPTION, false);
				break;
			}
		}
#else
		setType(SELECT_TYPE_SOUND_OPTION, false);
#endif
	}

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
#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
	TFlagManager::getInstance()->setFlag(MSF_SUBTITLES,
	                                     mSubtitleOption->getValue());
	TFlagManager::getInstance()->setFlag(MSF_LANGUAGE,
	                                     mLanguageOption->getValue());
#endif
}

bool TOptionControl::isChangedSetting() const
{
	bool result = true;

	if (mInitialRumbleValue == mRumbleOption->getValue()
	    && mInitialSoundValue == mSoundOption->getValue()
#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
	    && mInitialSubtitleValue == mSubtitleOption->getValue()
	    && mInitialLanguageValue == mLanguageOption->getValue()
#endif
	)
		result = false;

	return result;
}

void TOptionControl::resetChangedSetting()
{
	mInitialRumbleValue = mRumbleOption->getValue();
	mInitialSoundValue  = mSoundOption->getValue();
#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
	mInitialSubtitleValue = mSubtitleOption->getValue();
	mInitialLanguageValue = mLanguageOption->getValue();
#endif
}

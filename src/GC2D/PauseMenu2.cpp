#include <GC2D/CardSave.hpp>
#include <GC2D/GCConsole2.hpp>
#include <GC2D/MessageUtil.hpp>
#include <GC2D/PauseMenu2.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <JSystem/J2D/J2DPicture.hpp>
#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <JSystem/JKernel/JKRArchive.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JParticle/JPAEmitterManager.hpp>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>
#include <JSystem/JUtility/JUTColor.hpp>
#include <JSystem/JUtility/JUTRect.hpp>
#include <JSystem/JUtility/JUTResFont.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MSound/MSound.hpp>
#include <System/Application.hpp>
#include <System/Application.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <System/MarDirector.hpp>
#include <System/MarioGamePad.hpp>
#include <GC2D/ShineTable.hpp>
#include <stdio.h>

extern JPAEmitterManager* gpEmitterManager4D2;

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <System/DummyStrings.hpp>

// fabricated
void TPauseMenu2::draw(JDrama::TGraphics* gfx)
{
	J2DOrthoGraph graph(gfx->getViewport());
	graph.setup2D();
	mScreen->draw(0, 0, &graph);
	gfx->setScissor(gfx->getScissor());
}

TPauseMenu2::TPauseMenu2(const char* pName)
    : JDrama::TViewObj(pName)
    , mState(MENU_CLOSED)
    , mScreen(nullptr)
    , mBackground(nullptr)
    , mMenuPane(nullptr)
    , mStageName(nullptr)
    , mStagePane(nullptr)
    , mScenarioName(nullptr)
    , mSelectedItem(0)
    , mBounceAnim(0.0f)
    , mFadeAnim(0.0f)
    , mFirstItemAngle(0)
    , unkFC(6)
    , mEffectKeyFrame(3)
    , mEffectStretch(129)
    , mNumItems(3)
    , mPressedB(false)
    , mSelectionConfirmed(false)
    , mEmitter(nullptr)
    , unk114(0)
{
}

// fabricated: name-and-return binders, each a pool slot in load().
static inline JKRArchive* PauseArchive()
{
	JKRArchive* arch = (JKRArchive*)JKRFileLoader::getVolume("game_6");
	return arch;
}

static inline J2DPicture* PauseSearchPicture(J2DScreen* screen, u32 tag)
{
	J2DPicture* pic = (J2DPicture*)screen->search(tag);
	return pic;
}

static inline u8 PauseCurrentMap(TMarDirector* director)
{
	u8 map = director->getCurrentMap();
	return map;
}

void TPauseMenu2::load(JSUMemoryInputStream& pStream)
{
	JDrama::TViewObj::load(pStream);

	JKRArchive* arch = PauseArchive();
	mScreen          = new J2DSetScreen("pause_1.blo", arch);
	mScreen->setCullBack(GX_CULL_BACK);

	if (gpApplication.mCurrArea.unk0 <= 1) {
		// Hide "Exit Area" option in Delfino Plaza.
		mNumItems = 2;
	}

	mBackground = mScreen->search('mask');
	mMenuPane   = mScreen->search('t_0');

	for (s32 i = 0; i < 5; i++) {
		mPauseLetters[i] = PauseSearchPicture(mScreen, 'pa00' + i);
	}

	for (s32 i = 0; i < 3; i++) {
		mMenuItems[i] = (J2DPicture*)mScreen->search('tx_1' + i);

		if (mNumItems == 2) {
			mMenuItems[i]->add(0, 20);
		}
		mMenuItems[i]->hide();
	}

	mStageName = (J2DTextBox*)mScreen->search('map');
	mStageName->setFont(gpSystemFont);

	mScenarioName = (J2DTextBox*)mScreen->search('task');
	SMSMakeTextBuffer(mScenarioName, 0x80);
	mScenarioName->setFont(gpSystemFont);

	mStagePane = mScreen->search('brek');

	u32 shineStage = SMS_getShineStage(SMSGetMarDirector()->mMap);
	s32 flag       = TFlagManager::getInstance()->getFlag(0x40003);

	mStageName->setString(SMSGetMessageData(
	    JKRFileLoader::getGlbResource("/common/2d/stagename.bmg"), shineStage));

	if (PauseCurrentMap(SMSGetMarDirector()) != 0xF) {
		void* scenarioBmg
		    = JKRFileLoader::getGlbResource("/common/2d/scenarioname.bmg");
		s16 shineID = SMS_getShineID(shineStage, flag, false);
		const char* scenarioName;
		if (scenarioBmg == nullptr || shineID == -1 || shineStage == 0) {
			scenarioName = "";
			mStageName->add(0, 0xF);
			mMenuPane->add(0, 0x1E);
		} else {
			scenarioName = SMSGetMessageData(
			    scenarioBmg, (u16)SMS_getNormalStage(shineID));
		}
		snprintf(mScenarioName->getStringPtr(), 0x80, "%s", scenarioName);
	}
}

// fabricated: binding levels over the pane arrays, +8 of low region each.
static inline J2DPane* PauseBackgroundFork(const TPauseMenu2* p)
{
	return p->mBackground;
}

static inline J2DPane* PauseBackground(const TPauseMenu2* p)
{
	J2DPane* x = PauseBackgroundFork(p);
	return x;
}

static inline J2DPicture* PauseMenuItem(J2DPicture** items, s32 i)
{
	J2DPicture* x = items[i];
	return x;
}

static inline J2DPicture* PauseLetter(J2DPicture** letters, s32 i)
{
	J2DPicture* x = letters[i];
	return x;
}

void TPauseMenu2::loadAfter()
{
	// "Save data"
	mCardSave = JDrama::TNameRefGen::search<TCardSave>("データセーブ");

	mItemColor = PauseMenuItem(mMenuItems, 0)->getWhite();

	for (s32 i = 0; i < mNumItems; i++) {
		mOrigItemBounds[i] = PauseMenuItem(mMenuItems, i)->getBounds();
	}

	for (s32 i = 0; i < 5; i++) {
		mOrigLetterBounds[i] = mPauseLetters[i]->getBounds();
		mOrigLetterAngles[i] = mPauseLetters[i]->getRotation();
	}

	mBackgroundAlpha       = PauseBackground(this)->mAlpha;
	mBackgroundFadeInSpeed = mBackgroundAlpha / 45.0f;

	mFirstItemAngle = mMenuItems[0]->mRotation;

	if (mFirstItemAngle > 180) {
		mFirstItemAngle -= 360;
	}

	mGamePad = gpMarDirector->unk18[0];
}

// fabricated: a direct-return fork over a menu item, +4 below and +4 above the
// item rect.
static inline J2DPicture* PauseMenuItemFork(const TPauseMenu2* p, s32 i)
{
	return p->mMenuItems[i];
}

// appearWindow and disappearWindow are weak but out of line in the map:
// `inline` definitions reached from perform() through one more inline level
// (depth 2, budget 9) are refused there and emitted as weak copies.
inline void TPauseMenu2::appearWindow()
{
	if (mFadeAnim <= 45.0f) {
		for (s32 i = 0; i < 5; i++) {
			// Animate each letter with a delay.
			f32 letterAnim = mFadeAnim - 5.0f * i;
			drawAppearPane(mPauseLetters[i], letterAnim, mOrigLetterBounds[i],
			               (180.0f + mOrigLetterAngles[i] - 9.0f * letterAnim));
		}

		// Fade in background.
		s32 bgrAlpha = mBackgroundFadeInSpeed * mFadeAnim * 1.5f;
		bgrAlpha = bgrAlpha > mBackgroundAlpha ? mBackgroundAlpha : bgrAlpha;
		mBackground->setAlpha(bgrAlpha);
	}

	// Fade in stage/shine panel.
	s32 stageAlpha = mFadeAnim * mBackgroundFadeInSpeed * 1.5f;
	if (stageAlpha > mBackgroundAlpha) {
		stageAlpha = 0xFF;
	}
	mStagePane->setAlpha(stageAlpha);

	// Animate in menu items.
	if (mFadeAnim >= 30.0f) {
		for (s32 i = 0; i < mNumItems; ++i) {
			if (!mMenuItems[i]->isVisible()) {
				mMenuItems[i]->show();
				mMenuItems[i]->setAlpha(0);
			}

			JUTRect rect = mOrigItemBounds[i];

			if (mFadeAnim <= 40.0f) {
				s32 hx = 3.0f * (mFadeAnim - 35.0f);
				s32 hy = 1.5f * (mFadeAnim - 35.0f);
				rect.reform(-hx, -hy, hx, hy);

				mMenuItems[i]->setBounds(rect);

				// TODO: This looks a bit fake...
				f32 alphaF = 26.0f * (mFadeAnim - 30.0f);
				s32 alpha  = alphaF;
				if ((s32)alphaF > 0xFF) {
					alpha = 0xFF;
				}
				mMenuItems[i]->setAlpha(alpha);

				// The first item gets some extra "bounce".
				if (i == 0) {
					s16 angle = mFirstItemAngle * 0.1f * (mFadeAnim - 30.0f);
					PauseMenuItemFork(this, i)->mRotation = angle;
				}
			} else if (mFadeAnim <= 45.0f) {
				s32 hx = 3.0f * (mFadeAnim - 45.0f);
				s32 hy = 1.5f * (mFadeAnim - 45.0f);
				rect.reform(hx, hy, -hx, -hy);

				mMenuItems[i]->setBounds(rect);
			} else if (mFadeAnim >= 46.0f) {
				// Fade in animation complete; set state to open.
				if (mState != MENU_OPEN) {
					mState = MENU_OPEN;
				}
			}
		}
	}

	mFadeAnim += 0.5f;
}

// fabricated: a direct-return fork over the menu pane, +4 below and +4 above
// the letter rects.
static inline J2DPane* PauseMenuPane(const TPauseMenu2* p)
{
	return p->mMenuPane;
}

inline void TPauseMenu2::disappearWindow()
{
	// Delete emitter and reset selected item back to its original size.
	if (mFadeAnim == 0.0f && mEmitter != 0) {
		gpEmitterManager4D2->forceDeleteEmitter(mEmitter);
		mMenuItems[mSelectedItem]->setBounds(mOrigItemBounds[mSelectedItem]);
	}

	if (mFadeAnim <= 10.0f) {

		// Fade out menu, background, and shine/stage panel.
		s32 alpha
		    = (mMenuPane->getAlpha() - 12) < 0 ? 0 : mMenuPane->getAlpha() - 12;

		mMenuPane->setAlpha(alpha);

		if (alpha < mBackground->getAlpha()) {
			mBackground->setAlpha(alpha);
		}

		if (alpha < mStagePane->getAlpha()) {
			mStagePane->setAlpha(alpha);
		}

		// Shrink letters...
		for (s32 i = 0; i < 5; i++) {
			JUTRect rect = mPauseLetters[i]->getBounds();

			rect.add(0.025f * -rect.x1 + 0.01f * rect.getWidth(),
			         0.025f * -rect.y1 + 0.01f * rect.getHeight());

			rect.resize(0.98f * rect.getWidth(), 0.98f * rect.getHeight());

			mPauseLetters[i]->setBounds(rect);
		}

		// ... and now shrink the menu items in the same manner.
		for (s32 i = 0; i < mNumItems; i++) {
			JUTRect rect = mMenuItems[i]->getBounds();
			rect.add(0.025f * -rect.x1 + 0.01f * rect.getWidth(),
			         0.025f * -rect.y1 + 0.01f * rect.getHeight());
			rect.resize(0.98f * rect.getWidth(), 0.98f * rect.getHeight());
			mMenuItems[i]->setBounds(rect);
		}

		// Rotate menu clockwise.
		PauseMenuPane(this)->mRotation = -mFadeAnim * 2.0f;
	} else {
		// Fade out animation complete; set state to MENU_CLOSED.
		mState = MENU_CLOSED;
	}

	mFadeAnim += 0.5f;
}

// fabricated: the level that keeps the inline window bodies out of perform().
static inline void PauseAppear(TPauseMenu2* m) { m->appearWindow(); }
static inline void PauseDisappear(TPauseMenu2* m) { m->disappearWindow(); }

void TPauseMenu2::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (SMSGetMarDirector()->mState == TMarDirector::STATE_UNK5) {
		if (mState == MENU_SAVING) {
			if (cue & CUE_MOVE) {
				if (mCardSave->unk2DF != 0) {
					mSelectionConfirmed = false;
					mState              = MENU_OPEN;
				}

				s16 alpha = mMenuPane->getAlpha();

				alpha -= 16;
				if (alpha < 0) {
					alpha = 0;
				}

				mMenuPane->setAlpha(alpha);
			}
			if (cue & CUE_DRAW) {
				switch (mState) {

				case MENU_APPEARING:
					draw(graphics);
					break;
				case MENU_OPEN:
				case UNK2:
				case MENU_SAVING:
				case MENU_DISAPPEARING:
					draw(graphics);
					break;
				default:
					break;
				}
			}
		} else {
			if (cue & CUE_MOVE) {
				switch (mState) {
				case MENU_APPEARING:
					PauseAppear(this);
					break;
				case MENU_OPEN: {
					s32 curSelectedItem = mSelectedItem;
					if (mGamePad->checkFrameMeaning(0x21)) {
						// Confirm currently selected item.
						switch (curSelectedItem) {
						case 0:
							mSelectionConfirmed = true;
							SMSRumbleMgr->finishPause();
							gpMSound->pauseOff(0);
							SMSGetMarDirector()->getConsole()->pauseOut();
							mFadeAnim = 0.0f;
							mState    = MENU_DISAPPEARING;
							break;
						case 2:
							mSelectionConfirmed = true;
							setDrawEnd();
							break;
						case 3:
							mSelectionConfirmed = true;
							setDrawEnd();
							break;
						case 1:
							SMSGetMSound()->startSoundSystemSE(
							    MSD_SE_SY_PAUSE_ON, 0, nullptr, 0);

							mSelectionConfirmed = true;
							mCardSave->init(0);
							mState = MENU_SAVING;
						default:
							break;
						}
					} else if (mGamePad->checkFrameMeaning(0x40)) {
						// Close pause menu by pressing B.
						mPressedB = true;
						mFadeAnim = 0.0f;
						setDrawEnd();
					} else if (mGamePad->checkFrameMeaning(0x4)) {
						// Select the next item.
						if (mNumItems > 2) {
							mSelectedItem = curSelectedItem < (mNumItems - 1)
							                    ? curSelectedItem + 1
							                    : 0;
						} else {
							// don't loop back around with less than three items
							mSelectedItem = mNumItems - 1;
						}
						if (mSelectedItem != curSelectedItem) {
							SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_CURSOR,
							                                   0, nullptr, 0);

							JUTRect& bounds = mOrigItemBounds[curSelectedItem];
							mMenuItems[mSelectedItem]->mWhite = mItemColor;
							mMenuItems[curSelectedItem]->mWhite
							    = JUtility::TColor();
							mMenuItems[curSelectedItem]->setBounds(bounds);

							mBounceAnim = 0.0f; // reset animation

							// Remove spark particles.
							if (mEmitter != nullptr) {
								gpEmitterManager4D2->forceDeleteEmitter(
								    mEmitter);
							}
						}
					} else if (mGamePad->checkFrameMeaning(0x2)) {
						// Select the previous item.
						if (mNumItems > 2) {
							mSelectedItem = (curSelectedItem == 0u)
							                    ? mNumItems - 1
							                    : curSelectedItem - 1;
						} else {
							mSelectedItem = 0;
						}

						if (mSelectedItem != curSelectedItem) {
							SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_CURSOR,
							                                   0, nullptr, 0);

							JUTRect& bounds = mOrigItemBounds[curSelectedItem];
							mMenuItems[mSelectedItem]->mWhite = mItemColor;
							mMenuItems[curSelectedItem]->mWhite
							    = JUtility::TColor();
							mMenuItems[curSelectedItem]->setBounds(bounds);

							mBounceAnim = 0.0f;

							if (mEmitter != nullptr) {
								gpEmitterManager4D2->forceDeleteEmitter(
								    mEmitter);
							}
						}
					}

					JUTRect animItemBounds = mOrigItemBounds[mSelectedItem];
					if (mBounceAnim == mEffectKeyFrame) {
						// Spawn a "spark" effect from the selected item.
						f32 width = mMenuItems[mSelectedItem]->getWidth();
						f32 emitterWidth = width / mEffectStretch;

						JUTRect itemBounds
						    = mMenuItems[mSelectedItem]->getGlobalBounds();

						// TODO: This doesn't fully match.
						gpEmitterManager4D2->createEmitter(
						    JGeometry::TVec3<f32>(
						        itemBounds.x1 + 0.5f * itemBounds.getWidth(),
						        itemBounds.y1 + 0.5f * itemBounds.getHeight(),
						        0.0f),
						    0x1FA, nullptr, nullptr);

						mEmitter = gpEmitterManager4D2->unkC8[0][0];
						setEmitterScale(4.0f * emitterWidth, 1.0f, 1.0f);
					}

					if (mBounceAnim < 18.0f) {
						s32 hw = mBounceAnim;
						s32 hh = 0.5f * mBounceAnim;
						animItemBounds.reform(-hw, -hh, hw, hh);
						mMenuItems[mSelectedItem]->setBounds(animItemBounds);

						s32 colorShift = 10.0f * mBounceAnim;
						mMenuItems[mSelectedItem]->mWhite
						    = mItemColor + (colorShift << 24);
					} else if (mBounceAnim < 35.0f) {
						s32 hw = mBounceAnim - 35.0f;
						s32 hh = 0.5f * (mBounceAnim - 35.0f);
						animItemBounds.reform(-hw, -hh, hw, hh);
						mMenuItems[mSelectedItem]->setBounds(animItemBounds);

						s32 colorShift = (35.0f - mBounceAnim) * 10.0f;
						mMenuItems[mSelectedItem]->mWhite
						    = mItemColor + (colorShift << 24);
					} else {
						// Loop animation.
						mBounceAnim = -0.5f;
						unkFC *= -1;
					}

					mBounceAnim += 0.5f;
				} break;
				case MENU_DISAPPEARING:
					PauseDisappear(this);
					break;
				default:
					break;
				}
			}

			if (cue & CUE_DRAW) {
				switch (mState) {
				case MENU_APPEARING:
					draw(graphics);
					break;
				case MENU_OPEN:
				case UNK2:
				case MENU_SAVING:
				case MENU_DISAPPEARING:
					draw(graphics);
					break;
				default:
					break;
				}
			}
		}
	}
}

u8 TPauseMenu2::getNextState()
{
	u8 state = 2;
	if (mSelectionConfirmed) {
		switch (mSelectedItem) {
		case 0:
			if (mState == MENU_CLOSED) {
				state = 0;
			}
			break;
		case 1:
			if (mState == MENU_CLOSED) {
				state = mCardSave->getNextState();
			}
			break;
		case 2:
			if (mState == MENU_CLOSED) {
				state = 5;
			}
			break;
		default:
			state = 0;
			break;
		}
	} else {
		if (mPressedB && mState == MENU_CLOSED) {
			state = 0;
		}
	}
	return state;
}

// Binding level over the director's console read.
static inline TGCConsole2* PauseConsole(TMarDirector* director)
{
	TGCConsole2* console = director->getConsole();
	return console;
}

static inline TMarDirector* PauseDirector()
{
	return gpMarDirector;
}

void TPauseMenu2::setDrawStart()
{
	mPressedB           = false;
	mSelectionConfirmed = false;
	mSelectedItem       = 0;
	mFadeAnim           = 0.0f;
	mState              = MENU_APPEARING;
	mBounceAnim         = 0.0f;
	PauseMenuItem(mMenuItems, 0)->mWhite   = mItemColor;
	PauseMenuItem(mMenuItems, 0)->mVisible = false;

	for (s32 i = 1; i < mNumItems; ++i) {
		PauseMenuItem(mMenuItems, i)->mWhite   = 0xFFFFFFFF;
		PauseMenuItem(mMenuItems, i)->mVisible = false;
	}

	mPauseLetters[0]->mRotation = u16(mOrigLetterAngles[0] + 180.0f);
	mPauseLetters[1]->mRotation = u16(mOrigLetterAngles[1] + 180.0f);
	mPauseLetters[2]->mRotation = u16(mOrigLetterAngles[2] + 180.0f);
	mPauseLetters[3]->mRotation = u16(mOrigLetterAngles[3] + 180.0f);
	mPauseLetters[4]->mRotation = u16(mOrigLetterAngles[4] + 180.0f);

	mMenuPane->setAlpha(255);
	mMenuPane->mRotation = 0.0f;
	PauseConsole(PauseDirector())->pauseIn();
	SMSRumbleMgr->startPause();
	gpMSound->pauseOn(true);
	// TODO: frame-exact; four stack markers on the TColor assignment temps
	// (0x58/0x50 retail vs 0x60/0x5c) — known-open JUTColor temp stride.
	// Tried (cc50): setWhite()/TColor()/TColor(u32)/set(u32)/(u32) spellings at
	// both sites and binder/raw/fork per item site (81 combos); none moves the
	// two temps without changing the frame.
}

void TPauseMenu2::setDrawEnd()
{
	SMSRumbleMgr->finishPause();
	gpMSound->pauseOff(0);
	PauseConsole(gpMarDirector)->pauseOut();
	mFadeAnim = 0.0f;
	mState    = MENU_DISAPPEARING;
}

void TPauseMenu2::drawAppearPane(J2DPicture* picture, f32 anim, JUTRect& rect,
                                 f32 rotation)
{
	if (anim < 0.0f) {
		if (picture->isVisible()) {
			picture->hide();
		}
	} else if (!(anim >= 20.0f)) {
		if (!picture->isVisible()) {
			picture->show();
			picture->setAlpha(0);
		}

		if (anim == 2.0f) {
			JUTRect rect = picture->getGlobalBounds();

			// TODO: This doesn't fully match: retail stores the width conversion
			// after both xoris (r6/r5 swap) and loads the 0.0f later; frame +8.
			// Tried (cc50): operand orders, `/ 2.0f`, a named TVec3 (ctor or set),
			// integer 0, named x/y, the copy-ctor and raw mGlobalBounds forms.
			gpEmitterManager4D2->createEmitter(
			    JGeometry::TVec3<f32>(rect.x1 + 0.5f * rect.getWidth(),
			                          rect.y1 + 0.5f * rect.getHeight(), 0.0f),
			    0x1F9, nullptr, nullptr);
		}

		f32 normalisedAngle = 0.05f * anim;
		s32 shrink = 2.0f * normalisedAngle * (1.0f - normalisedAngle) * 80.0f;
		s32 offset = 0.75f * (19.0f - anim);
		picture->mRotation = rotation;
		picture->setBounds(rect.x1 + offset, rect.y1 - shrink + offset,
		                   rect.x2 - offset, rect.y2 - shrink - offset);

		u16 alpha = anim * 12.8f;
		if (alpha > 255) {
			alpha = 255;
		}
		picture->setAlpha(alpha);
	}
}

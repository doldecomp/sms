#include <GC2D/Guide.hpp>
#include <stdio.h>
#include <string.h>
#include <GC2D/BoundPane.hpp>
#include <GC2D/ExPane.hpp>
#include <GC2D/MessageUtil.hpp>
#include <GC2D/ScrnFader.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <JSystem/J2D/J2DPicture.hpp>
#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <JSystem/JKernel/JKRMemArchive.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JUtility/JUTPoint.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <MSound/MSound.hpp>
#include <MSound/SoundEffects.hpp>
#include <Player/MarioAccess.hpp>
#include <System/Application.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <System/MarioGamePad.hpp>
#include <System/StageUtil.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <System/DummyStrings.hpp>

/// Frames left before the guide archive is swapped in. In .sbss, so it
/// survives the screen being torn down and rebuilt.
static u8 setup_wait;

TGuide::TGuide(const char* name)
    : JDrama::TViewObj(name)
    , mState(STATE_INIT)
    , mScreen(nullptr)
    , mGamePad(nullptr)
    , unkC4(0)
    , unkC5(0)
    , mMapAlpha(0xFF)
    , mCursorBlinkUp(1)
    , mSelectedPoint(-1)
{
}

void TGuide::load(JSUMemoryInputStream& stream)
{
	unkC5 = 0;
	JDrama::TNameRef::load(stream);
	JKRMemArchive* archive = gpMarDirector->unkD8;
	setup(archive);

	mScreen = new J2DSetScreen("guide_1.blo", archive);

	((J2DTextBox*)mScreen->search('a_ic'))->setFont((JUTFont*)gpSystemFont);
	((J2DTextBox*)mScreen->search('a_tx'))->setFont((JUTFont*)gpSystemFont);
	((J2DTextBox*)mScreen->search('b_ic'))->setFont((JUTFont*)gpSystemFont);
	((J2DTextBox*)mScreen->search('b_tx'))->setFont((JUTFont*)gpSystemFont);

	mStageNameBox = nullptr;
	mStageNameBox = (J2DTextBox*)mScreen->search('s_mn');
	SMSMakeTextBuffer(mStageNameBox, 26);
	mStageNameBox->setFont((JUTFont*)gpSystemFont);

	for (int i = 0; i < 10; ++i) {
		char path[255];
		snprintf(path, 255, "/guide/timg/coin_number_%d.bti", i);
		mNumberTextures[i]
		    = new JUTTexture((const ResTIMG*)JKRGetResource(path));
	}

	mShineIcon = mScreen->search('ss_i');
	for (int i = 0; i < 2; ++i)
		mShineDigits[i] = (J2DPicture*)mScreen->search('ss_1' + i);

	mEtcShineIcon = mScreen->search('sq_i');
	for (int i = 0; i < 2; ++i)
		mEtcShineMarks[i] = mScreen->search('sq_1' + i);

	for (int i = 0; i < 3; ++i)
		mCoinDigits[i] = (J2DPicture*)mScreen->search('sc_1' + i);

	mCoinIcon = mScreen->search('sc_s');

	for (int i = 0; i < 2; ++i)
		mBlueCoinDigits[i] = (J2DPicture*)mScreen->search('sb_1' + i);

	JUTTexture* cursorTexture = new JUTTexture(
	    (const ResTIMG*)JKRGetResource("/guide/timg/guide_cursor_2.bti"));

	for (int i = 0; i < 2; ++i) {
		mCursors[i] = new TExPane(mScreen, 'cu_a' + i);
		J2DPicture* pic = (J2DPicture*)mCursors[i]->getPane();
		pic->insert(cursorTexture, pic->mTextureNum, 0.0f);
	}

	for (int i = 0; i < 13; ++i) {
		u32 tag           = ((i / 10) << 8) + (i % 10 + '00');
		mStagePanes[i]    = mScreen->search(tag);
		mPanelsA[i]       = new TExPane(mScreen, (tag << 16) + '_0');
		mPanelRects[i]    = mPanelsA[i]->getPane()->mBounds;
		mPanelsB[i]       = new TExPane(mScreen, (tag << 16) + '_1');
		((J2DTextBox*)mScreen->search((tag << 16) + '_3'))
		    ->setFont((JUTFont*)gpSystemFont);
		((J2DTextBox*)mScreen->search((tag << 16) + '_5'))
		    ->setFont((JUTFont*)gpSystemFont);
	}

	mStagePanes[13] = mScreen->search('20');
	mPanelsA[13]    = new TExPane(mScreen, 'lwin');
	mPanelRects[13] = mPanelsA[13]->getPane()->mBounds;
	mPanelsB[13]    = new TExPane(mScreen, 'llin');

	for (int i = 0; i < 10; ++i)
		mPointPanes[i] = mScreen->search('pn00' + i);

	mMarioMarker = mScreen->search('01mi');
	mMapRect     = mScreen->search('01_9')->mBounds;

	mStageNameBmg = JKRGetResource("/common/2d/stagename.bmg");

	mSunPane = (J2DPicture*)mScreen->search('10');
	mSunPane->insert(
	    new JUTTexture(
	        (const ResTIMG*)JKRGetResource("/guide/timg/guide_draw_sun_2.bti")),
	    mSunPane->mTextureNum, 0.0f);

	mShipPane = (J2DPicture*)mScreen->search('13');
	mShipPane->insert(new JUTTexture((const ResTIMG*)JKRGetResource(
	                      "/guide/timg/guide_draw_ship_2.bti")),
	                  mShipPane->mTextureNum, 0.0f);

	mPalmPane2 = (J2DPicture*)mScreen->search('16');
	mPalmPane2->insert(new JUTTexture((const ResTIMG*)JKRGetResource(
	                       "/guide/timg/guide_draw_palmtree_2.bti")),
	                   mPalmPane2->mTextureNum, 0.0f);

	mPalmPane1 = (J2DPicture*)mScreen->search('17');
	mPalmPane1->insert(new JUTTexture((const ResTIMG*)JKRGetResource(
	                       "/guide/timg/guide_draw_palmtree_1.bti")),
	                   mPalmPane1->mTextureNum, 0.0f);

	mFishPane = (J2DPicture*)mScreen->search('18');
	mFishPane->insert(new JUTTexture((const ResTIMG*)JKRGetResource(
	                      "/guide/timg/guide_draw_fish_2.bti")),
	                  mFishPane->mTextureNum, 0.0f);

	mCloudPane = (J2DPicture*)mScreen->search('11');
	mWavePane1 = (J2DPicture*)mScreen->search('14');
	mWavePane2 = (J2DPicture*)mScreen->search('15');
	mBirdPane1 = (J2DPicture*)mScreen->search('12');
	mBirdPane1->setBasePosition(J2DBasePosition_4);
	mBirdPane2 = (J2DPicture*)mScreen->search('19');
	mBirdPane2->setBasePosition(J2DBasePosition_4);

	void* bmg = JKRGetResource("/guide/guidemess.bmg");
	for (int i = 0; i < 13; ++i) {
		u32 tag = (((i / 10) << 24) + ('00' << 16)) + ((i % 10) << 16);

		J2DTextBox* title = (J2DTextBox*)mScreen->search(tag + '_3');
		SMSMakeTextBuffer(title, 30);
		title->setFont((JUTFont*)gpSystemFont);
		strncpy(title->getStringPtr(), SMSGetMessageData(bmg, i + 13), 30);

		J2DTextBox* body = (J2DTextBox*)mScreen->search(tag + '_5');
		SMSMakeTextBuffer(body, 512);
		body->setFont((JUTFont*)gpSystemFont);
		strncpy(body->getStringPtr(), SMSGetMessageData(bmg, i), 512);
	}

	mMarkPane       = new TExPane(mScreen, 'mark');
	mShineBoundPane = new TBoundPane(mScreen, '20');

	resetObjects();
	unkC5 = 1;
}

void TGuide::resetObjects()
{
	int total = 0;
	for (u32 i = 0; i < 13; ++i) {
		if (i < 10) {
			mScores[i].unk0         = 0;

			int shines = 0;
			if (i != 0 && i != 1) {
				for (u32 j = 0; j < 8; ++j)
					if (SMS_isGetShine(i, j, false))
						shines++;
			}
			int num              = shines < 100 ? shines : 99;
			mScores[i].mShineNum = num;
			total += num;

			int etcShines = 0;
			if (i != 0 && i != 1) {
				if (SMS_isGetShine(i, 1, true))
					etcShines = 1;
				if (SMS_isGetShine(i, 2, true))
					etcShines++;
			}
			if (etcShines >= 10)
				etcShines = 9;
			mScores[i].mEtcShineNum = etcShines;
			total += etcShines;

			u16 coins = TFlagManager::getInstance()->getFlag(0x20005 + i);
			if (coins >= 1000)
				coins = 999;
			mScores[i].mCoinNum = coins;

			mScores[i].mHasFirstEtcShine = SMS_isGetShine(i, 0, true);
			if (mScores[i].mHasFirstEtcShine)
				total++;

			int blueCoins = 0;
			if (i != 0) {
				for (u8 j = 0; j < 50; ++j)
					if (TFlagManager::getInstance()->getBlueCoinFlag(
					        scNormalStageTable[i], j))
						blueCoins++;
			}
			if (blueCoins >= 1000)
				blueCoins = 999;
			mScores[i].mBlueCoinNum = blueCoins;

			if (TFlagManager::getInstance()->getBool(0x103A5 + i)) {
				mPointPanes[i]->mVisible = true;
				mStagePanes[i]->mVisible = true;
			} else {
				mPointPanes[i]->mVisible = false;
				mStagePanes[i]->mVisible = false;
			}
		}
	}

	mScores[9].unk0 = 1;

	int blueCoins = 0;
	for (u8 i = 0; i < 50; ++i)
		if (TFlagManager::getInstance()->getBlueCoinFlag(scNormalStageTable[9],
		                                                i))
			blueCoins++;
	mScores[9].mBlueCoinNum = blueCoins;

	int bosses = 0;
	if (TFlagManager::getInstance()->getBool(0x10056))
		bosses = 1;
	if (TFlagManager::getInstance()->getBool(0x10058))
		bosses++;
	mScores[0].mShineNum = bosses;
	total += (s16)bosses;

	mScores[1].mShineNum = TFlagManager::getInstance()->getFlag(0x40000) - total;

	changeBotStatus(-1);
	resetScore();
	mCursors[0]->getPane()->mVisible = true;
	mCursors[1]->getPane()->mVisible = true;
}

void TGuide::resetScore()
{
	int etcTotal = 0;
	int total    = 0;
	for (int i = 0; i < 10; ++i) {
		if (i == 9)
			continue;

		if (TFlagManager::getInstance()->getBool(0x103A5 + i))
			mScreen->search((i << 24) + '0_mn')->mVisible = true;
		else
			mScreen->search((i << 24) + '0_mn')->mVisible = false;

		if (i <= 1)
			continue;

		for (int j = 0; j < 8; ++j) {
			if (j < mScores[i].mShineNum)
				mScreen->search((i << 24) + '0ss1' + j)->mVisible = true;
			else
				mScreen->search((i << 24) + '0ss1' + j)->mVisible = false;
		}

		J2DPane* etc1     = mScreen->search((i << 24) + '0sq1');
		etc1->mVisible    = false;
		J2DPane* etc2     = mScreen->search((i << 24) + '0sq2');
		etc2->mVisible    = false;
		if (mScores[i].mEtcShineNum != 0)
			etc1->mVisible = true;
		if (mScores[i].mEtcShineNum > 1)
			etc2->mVisible = true;

		etcTotal += mScores[i].mEtcShineNum;
		total += mScores[i].mShineNum;
	}

	total += etcTotal;
	if ((u8)etcTotal == 0)
		mScreen->search('lqus')->mVisible = false;
	else
		mScreen->search('lqus')->mVisible = true;

	for (int i = 1; i < 10; ++i) {
		mMarkerPanes[i] = mScreen->search('mi00' + i);
		if (i == 9)
			continue;

		u32 tag   = (i << 24) + '0c_1';
		u16 coins = mScores[i].mCoinNum;
		if (coins > 999)
			coins = 999;

		J2DPicture* digit100 = (J2DPicture*)mScreen->search(tag);
		J2DPicture* digit10  = (J2DPicture*)mScreen->search(tag + 1);
		J2DPicture* digit1   = (J2DPicture*)mScreen->search(tag + 2);
		u16 rest             = coins;
		if (rest < 100) {
			digit100->mVisible = false;
			digit10->changeTexture(mNumberTextures[rest / 10]->mTexInfo, 0);
			digit1->changeTexture(mNumberTextures[rest % 10]->mTexInfo, 0);
		} else {
			u16 hundreds       = rest / 100;
			digit100->mVisible = true;
			digit100->changeTexture(mNumberTextures[hundreds]->mTexInfo, 0);
			coins -= hundreds * 100;
			rest = coins;
			digit10->changeTexture(mNumberTextures[rest / 10]->mTexInfo, 0);
			digit1->changeTexture(mNumberTextures[rest % 10]->mTexInfo, 0);
		}

		if (mScores[i].mHasFirstEtcShine) {
			mScreen->search((i << 24) + '0c_s')->mVisible = true;
			total++;
		} else {
			mScreen->search((i << 24) + '0c_s')->mVisible = false;
		}
	}

	mMarkerPanes[0] = mScreen->search('mi00');
	mClickPane      = mScreen->search('clic');

	int bosses = 0;
	if (TFlagManager::getInstance()->getBool(0x10056))
		bosses = 1;
	if (TFlagManager::getInstance()->getBool(0x10058))
		bosses++;
	((J2DPicture*)mScreen->search('0s_1'))
	    ->changeTexture(mNumberTextures[bosses]->mTexInfo, 0);
	total += bosses;

	s32 allShines = TFlagManager::getInstance()->getFlag(0x40000);
	u8 remaining  = allShines - (u8)total;
	if (remaining > 99)
		remaining = 99;
	((J2DPicture*)mScreen->search('1s_1'))
	    ->changeTexture(mNumberTextures[remaining / 10]->mTexInfo, 0);
	((J2DPicture*)mScreen->search('1s_2'))
	    ->changeTexture(mNumberTextures[remaining % 10]->mTexInfo, 0);

	if (allShines > 999)
		allShines = 999;
	J2DPicture* total100 = (J2DPicture*)mScreen->search('lt_1');
	J2DPicture* total10  = (J2DPicture*)mScreen->search('lt_2');
	J2DPicture* total1   = (J2DPicture*)mScreen->search('lt_3');
	if (allShines < 100) {
		total100->mVisible = false;
		total10->changeTexture(mNumberTextures[allShines / 10]->mTexInfo, 0);
		total1->changeTexture(mNumberTextures[allShines % 10]->mTexInfo, 0);
	} else {
		s32 hundreds       = allShines / 100;
		total100->mVisible = true;
		total100->changeTexture(mNumberTextures[hundreds]->mTexInfo, 0);
		allShines -= hundreds * 100;
		total10->changeTexture(mNumberTextures[allShines / 10]->mTexInfo, 0);
		total1->changeTexture(mNumberTextures[allShines % 10]->mTexInfo, 0);
	}

	switch (gpApplication.mSaveFile) {
	case 0:
		mScreen->search('ld_a')->mVisible = true;
		mScreen->search('ld_b')->mVisible = false;
		mScreen->search('ld_c')->mVisible = false;
		break;
	case 1:
		mScreen->search('ld_a')->mVisible = false;
		mScreen->search('ld_b')->mVisible = true;
		mScreen->search('ld_c')->mVisible = false;
		break;
	case 2:
		mScreen->search('ld_a')->mVisible = false;
		mScreen->search('ld_b')->mVisible = false;
		mScreen->search('ld_c')->mVisible = true;
		break;
	}

	mMarkAlpha
	    = 255.0f
	      * (1.0f
	         - (f32)(TFlagManager::getInstance()->getFlag(0x40000) / 30)
	               * 0.25f);
	mMarkPane->getPane()->setAlpha(mMarkAlpha);
}

JKRMemArchive* TGuide::setup(JKRMemArchive* archive)
{
	if (archive != nullptr)
		SMSMountAramArchive(archive, gArBkGuide);
	else
		setup_wait = 16;
	unkC4 = 0;
	return archive;
}

// UNUSED; the loaded variant, without the fallback delay.
JKRMemArchive* TGuide::setup2(JKRMemArchive* archive)
{
	if (archive != nullptr)
		SMSMountAramArchive(archive, gArBkGuide);
	unkC4 = 0;
	return archive;
}

void TGuide::startMoveCursor()
{
	mState         = STATE_MOVE_CURSOR;
	mCursorBlinkUp = 0;
}

// UNUSED
void TGuide::startMoveCursor2()
{
	mState         = STATE_MOVE_CURSOR;
	mCursorBlinkUp = 0;
	mSelectedPoint = -1;
	mTimer         = 0;
	changeBotStatus(-1);
}

void TGuide::linkSelect()
{
	mGamePad->onFlag(TMarioGamePad::PAD_FLAG_0x80);
	if (mGamePad->mEnabledFrameMeaning & TMarioGamePad::MEANING_0x40
	    || mGamePad->mButton.mTrigger & 0x10)
		mState = STATE_CLOSE;

	J2DPane* cursor = mCursors[0]->getPane();
	int x           = cursor->mBounds.x1
	        + (s16)(3.2f * mGamePad->mCompSPos[8]);
	int y = cursor->mBounds.y1 + (s16)(-3.2f * mGamePad->mCompSPos[9]);
	if (x > 568)
		x = 568;
	if (x < 0)
		x = 0;
	if (y > 360)
		y = 360;
	if (y < 56)
		y = 56;
	cursor->move(x, y);
	mCursors[1]->getPane()->move(x + 7, y + 4);

	int point = checkPoint(x - 2, y + 10);
	if (point != -1
	    && (mGamePad->mMeaning & TMarioGamePad::MEANING_0x20))
		appearGuidePane(point);

	if (point != -1 && point < 10) {
		int alpha = mPointPanes[point]->mAlpha;
		if (mCursorBlinkUp)
			alpha += 4;
		else
			alpha -= 4;
		if (alpha < 30) {
			mCursorBlinkUp = 1;
			alpha          = 30;
		} else if (alpha > 255) {
			mCursorBlinkUp = 0;
			alpha          = 255;
		}
		mPointPanes[point]->mAlpha = alpha;
		changePattern((J2DPicture*)mCursors[0]->getPane(), mTimer, 45);
		changePattern((J2DPicture*)mCursors[1]->getPane(), mTimer, 45);
	}

	if (mSelectedPoint != point) {
		changeBotStatus(point);
		if (mSelectedPoint != -1 && mSelectedPoint < 10)
			mPointPanes[mSelectedPoint]->mAlpha = 255;
		if (point == -1) {
			J2DPicture* pic = (J2DPicture*)mCursors[0]->getPane();
			pic->setBlendKonstColor(1.0f, 0.0f, 0.0f, 0.0f);
			pic->setBlendKonstAlpha(1.0f, 0.0f, 0.0f, 0.0f);
			pic = (J2DPicture*)mCursors[1]->getPane();
			pic->setBlendKonstColor(1.0f, 0.0f, 0.0f, 0.0f);
			pic->setBlendKonstAlpha(1.0f, 0.0f, 0.0f, 0.0f);
		}
		mCursorBlinkUp = 0;
		mSelectedPoint = point;
	}

	changePattern(mSunPane, mTimer, 90);
	mirrorPattern(mCloudPane, mTimer, 90);
	rotatePattern(mBirdPane1, mTimer, 90, 30);
	changePattern(mShipPane, mTimer, 90);
	mirrorPattern(mWavePane1, mTimer, 90);
	mirrorPattern(mWavePane2, mTimer, 90);
	changePattern(mPalmPane2, mTimer, 90);
	changePattern(mPalmPane1, mTimer, 90);
	changePattern(mFishPane, mTimer, 90);
	rotatePattern(mBirdPane2, mTimer, 90, -45);

	shinePattern(mShineBoundPane, mTimer, 90);
	if (mTimer % 180u < 130)
		mClickPane->mAlpha = 255;
	else
		mClickPane->mAlpha = 0;
	mShineBoundPane->update();

	mmarkPattern(mMarkPane, mTimer, 270);
	mMarkPane->update();

	if (mMapBlinkUp) {
		mMapAlpha += 3;
		if (mMapAlpha > 300)
			mMapBlinkUp = false;
	} else {
		mMapAlpha -= 3;
		if (mMapAlpha < 30)
			mMapBlinkUp = true;
	}

	u8 alpha;
	if (mMapAlpha < 30)
		alpha = 30;
	else if (mMapAlpha > 255)
		alpha = 255;
	else
		alpha = mMapAlpha;
	for (int i = 0; i < 10; ++i)
		mStagePanes[i]->mAlpha = alpha;

	mTimer++;
	if (mTimer > 540)
		mTimer = 0;
}

// UNUSED; inlined into linkSelect for all nine decorative panes.
//
// The `u16 t` copy is load-bearing, not cosmetic: mTimer is a u16 and retail
// feeds it straight into the unsigned `% period`, with no sign extension
// anywhere in linkSelect. Spelling the modulo on the s16 parameter the map
// gives adds an extsh at every one of the eleven inline sites (linkSelect
// 87.9% -> 93.7%). All five helpers share the shape.
void TGuide::changePattern(J2DPicture* pane, s16 timer, u32 period)
{
	u16 t = timer;
	if (t % period == 0) {
		if ((t / period) % 2 == 0) {
			pane->setBlendKonstColor(0.0f, 1.0f, 0.0f, 0.0f);
			pane->setBlendKonstAlpha(0.0f, 1.0f, 0.0f, 0.0f);
		} else {
			pane->setBlendKonstColor(1.0f, 0.0f, 0.0f, 0.0f);
			pane->setBlendKonstAlpha(1.0f, 0.0f, 0.0f, 0.0f);
		}
	}
}

// UNUSED
void TGuide::mirrorPattern(J2DPicture* pane, s16 timer, u32 period)
{
	u16 t = timer;
	if (t % period == 0) {
		if ((t / period) % 2 == 0)
			pane->mMirror = MIRROR0;
		else
			pane->mMirror = J2DMirror_X;
	}
}

// UNUSED
void TGuide::rotatePattern(J2DPicture* pane, s16 timer, u32 period, s16 angle)
{
	u16 t = timer;
	if (t % period == 0) {
		if ((t / period) % 2 == 0)
			pane->mRotation = 0.0f;
		else
			pane->mRotation = angle;
	}
}

// UNUSED
void TGuide::shinePattern(TBoundPane* pane, s16 timer, u32 period)
{
	u16 t       = timer;
	u32 phase   = t % period;
	if (phase == 0)
		pane->setPanePosition(45, JUTPoint(0, 0), JUTPoint(0, -5),
		                      JUTPoint(0, 0));
	else if (phase == 45)
		pane->setPanePosition(45, JUTPoint(0, 0), JUTPoint(0, 5),
		                      JUTPoint(0, 0));
}

// UNUSED
void TGuide::mmarkPattern(TExPane* pane, s16 timer, u32 period)
{
	u16 t = timer;
	if (t % period == 0) {
		if ((t / period) % 2 == 0)
			pane->setPaneAlpha(period, 0, mMarkAlpha);
		else
			pane->setPaneAlpha(period, mMarkAlpha, 0);
	}
}

// UNUSED; the cursor snap that shipped disabled.
void TGuide::searchNearPoint(s16* out_x, s16* out_y, s16 x, s16 y)
{
	int nearest     = -1;
	int nearestDist = 0;
	for (int i = 0; i < 10; ++i) {
		JUTRect bounds = mPointPanes[i]->mBounds;
		int dx         = (bounds.x1 + bounds.x2) / 2 - x;
		int dy         = (bounds.y1 + bounds.y2) / 2 - y;
		int dist       = dx * dx + dy * dy;
		if (nearest == -1 || dist < nearestDist) {
			nearest     = i;
			nearestDist = dist;
		}
	}
	if (nearest != -1) {
		JUTRect bounds = mPointPanes[nearest]->mBounds;
		*out_x         = (bounds.x1 + bounds.x2) / 2;
		*out_y         = (bounds.y1 + bounds.y2) / 2;
	}
}

int TGuide::checkPoint(int x, int y)
{
	int hit = -1;
	for (int i = 0; i < 14; ++i) {
		JUTRect bounds = mStagePanes[i]->mBounds;
		if (x > bounds.x1 && x < bounds.x2 && y > bounds.y1 && y < bounds.y2) {
			hit = i;
			break;
		}
	}
	if (hit == -1) {
		for (int i = 0; i < 10; ++i) {
			JUTRect bounds = mPointPanes[i]->mBounds;
			if (x > bounds.x1 && x < bounds.x2 && y > bounds.y1
			    && y < bounds.y2) {
				hit = i;
				break;
			}
		}
	}
	if (hit >= 0 && hit < 10 && !mPointPanes[hit]->mVisible)
		hit = -1;
	return hit;
}

void TGuide::changeBotStatus(int stage)
{
	if (stage == -1 || stage >= 10) {
		mStageNameBox->mVisible = false;
		return;
	}

	if (mScores[stage].unk0 == 0) {
		mStageNameBox->mVisible = true;
		mShineIcon->mVisible    = true;
		strncpy(mStageNameBox->getStringPtr(),
		        SMSGetMessageData(mStageNameBmg, stage), 26);

		int shines = mScores[stage].mShineNum;
		if (shines < 0)
			shines = 0;
		if (shines > 99)
			shines = 99;
		if (shines < 10) {
			mShineDigits[1]->mVisible = false;
			mShineDigits[0]->changeTexture(
			    mNumberTextures[shines]->mTexInfo, 0);
		} else {
			mShineDigits[1]->mVisible = true;
			mShineDigits[0]->changeTexture(
			    mNumberTextures[shines / 10]->mTexInfo, 0);
			mShineDigits[1]->changeTexture(
			    mNumberTextures[shines % 10]->mTexInfo, 0);
		}

		if (stage <= 1 || mScores[stage].mEtcShineNum == 0) {
			mEtcShineIcon->mVisible     = false;
			mEtcShineMarks[0]->mVisible = false;
			mEtcShineMarks[1]->mVisible = false;
		} else if (mScores[stage].mEtcShineNum == 1) {
			mEtcShineIcon->mVisible     = true;
			mEtcShineMarks[0]->mVisible = true;
			mEtcShineMarks[1]->mVisible = false;
		} else {
			mEtcShineIcon->mVisible     = true;
			mEtcShineMarks[0]->mVisible = true;
			mEtcShineMarks[1]->mVisible = true;
		}

		int coins = mScores[stage].mCoinNum;
		if (coins < 0)
			coins = 0;
		if (coins > 999)
			coins = 999;
		if (coins < 100) {
			mCoinDigits[2]->mVisible = false;
			mCoinDigits[0]->changeTexture(
			    mNumberTextures[coins / 10]->mTexInfo, 0);
			mCoinDigits[1]->changeTexture(
			    mNumberTextures[coins % 10]->mTexInfo, 0);
		} else {
			int hundreds             = coins / 100;
			mCoinDigits[2]->mVisible = true;
			mCoinDigits[0]->changeTexture(
			    mNumberTextures[hundreds]->mTexInfo, 0);
			coins -= hundreds * 100;
			mCoinDigits[1]->changeTexture(
			    mNumberTextures[coins / 10]->mTexInfo, 0);
			mCoinDigits[2]->changeTexture(
			    mNumberTextures[coins % 10]->mTexInfo, 0);
		}

		if (mScores[stage].mHasFirstEtcShine)
			mCoinIcon->mVisible = true;
		else
			mCoinIcon->mVisible = false;

		int blueCoins = mScores[stage].mBlueCoinNum;
		if (blueCoins < 0)
			blueCoins = 0;
		if (blueCoins > 99)
			blueCoins = 99;

		if (stage == 0) {
			mScreen->search('sb_i')->mVisible = false;
			mScreen->search('sc_t')->mVisible = false;
		} else {
			mScreen->search('sb_i')->mVisible = true;
			mScreen->search('sc_t')->mVisible = true;
		}

		if (blueCoins < 10) {
			mBlueCoinDigits[1]->mVisible = false;
			mBlueCoinDigits[0]->changeTexture(
			    mNumberTextures[blueCoins % 10]->mTexInfo, 0);
		} else {
			mBlueCoinDigits[1]->mVisible = true;
			mBlueCoinDigits[0]->changeTexture(
			    mNumberTextures[blueCoins / 10]->mTexInfo, 0);
			mBlueCoinDigits[1]->changeTexture(
			    mNumberTextures[blueCoins % 10]->mTexInfo, 0);
		}
	} else {
		mStageNameBox->mVisible = true;
		mShineIcon->mVisible    = false;

		int blueCoins = mScores[stage].mBlueCoinNum;
		if (blueCoins < 0)
			blueCoins = 0;
		if (blueCoins > 99)
			blueCoins = 99;
		if (blueCoins < 10) {
			mBlueCoinDigits[1]->mVisible = false;
			mBlueCoinDigits[0]->changeTexture(
			    mNumberTextures[blueCoins % 10]->mTexInfo, 0);
		} else {
			mBlueCoinDigits[1]->mVisible = true;
			mBlueCoinDigits[0]->changeTexture(
			    mNumberTextures[blueCoins / 10]->mTexInfo, 0);
			mBlueCoinDigits[1]->changeTexture(
			    mNumberTextures[blueCoins % 10]->mTexInfo, 0);
		}

		mScreen->search('sb_i')->mVisible = true;
		mScreen->search('sc_t')->mVisible = false;
		mScreen->search('sq_i')->mVisible = false;

		strncpy(mStageNameBox->getStringPtr(),
		        SMSGetMessageData(mStageNameBmg, stage), 26);
	}
}

// TODO: all the arithmetic matches; retail reads gpMarioPos->z before
// storing the scaled x and keeps 0.5f in f4 across the marker-bounds loads,
// where ours reloads. Frame 0x100 against our 0x78.
// TODO: literal-pool order. The target asks for 21200.0f (@2954) before
// 25000.0f (@2955), so the z scaling is spelled before the x scaling in the
// source. Swapping the two statements does reproduce the pool order but costs
// placeMario 76.9 -> 73.1 (four extra instructions), so the statement order is
// right and something else about the pair's spelling is not; left as is.
void TGuide::placeMario()
{
	if ((u8)SMS_getShineStage(gpMarDirector->mMap) != 1) {
		mMarioMarker->mVisible = false;
		return;
	}

	JGeometry::TVec3<f32> pos = *gpMarioPos;
	int mapWidth              = mMapRect.x2 - mMapRect.x1;
	int mapHeight             = mMapRect.y2 - mMapRect.y1;
	pos.x                     = pos.x * (f32)mapWidth / 25000.0f;
	pos.y                     = 0.0f;
	pos.z                     = pos.z * (f32)mapHeight / 21200.0f;

	J2DPane* marker = mMarioMarker;
	int paneWidth   = marker->mBounds.x2 - marker->mBounds.x1;
	int paneHeight  = marker->mBounds.y2 - marker->mBounds.y1;
	int x           = (int)(0.5f * (f32)mapWidth + pos.x
	                        - 0.5f * (f32)paneWidth - 2.0f);
	int y           = (int)(0.5f * (f32)mapHeight + pos.z
	                        + 0.5f * (f32)paneHeight);
	if (x > mapWidth - paneWidth)
		x = mapWidth - paneWidth;
	if (x < 0)
		x = 0;
	if (y > mapHeight - paneHeight)
		y = mapHeight - paneHeight;
	if (y < 0)
		y = 0;
	marker->mVisible = true;
	mMarioMarker->move(x, y);

	for (int i = 2; i < 10; ++i) {
		if (TFlagManager::getInstance()->getBool(0x103A5 + i))
			mScreen->search('01g0' + i - 1)->mVisible = true;
		else
			mScreen->search('01g0' + i - 1)->mVisible = false;
	}
}

void TGuide::appearGuidePane(int stage)
{
	mOpenPanelA = mPanelsA[stage];
	mOpenPanelB = mPanelsB[stage];

	JUTRect rect   = mPanelRects[stage];
	JUTRect bounds = mStagePanes[stage]->mBounds;

	mOpenPanelA->getPane()->mVisible = true;
	int height                       = rect.y2 - rect.y1;
	int width                        = rect.x2 - rect.x1;
	mOpenPanelA->setCenteredSize(20, width, height, 0, 0);
	// TODO: rect.x1 twice really is what retail subtracts; the y term looks
	// like a copy-paste slip in the original.
	mOpenPanelA->setPaneOffset(20, 0, 0, bounds.x1 - rect.x1,
	                           bounds.y1 - rect.x1 - 40);

	mOpenPanelB->getPane()->setAlpha(0);
	mOpenPanelB->getPane()->mVisible = true;
	mOpenPanelB->setPaneAlpha(20, 255, 0);
	mCursors[0]->setPaneAlpha(20, 0, 255);
	mCursors[1]->setPaneAlpha(20, 0, 80);

	if (stage == 1)
		placeMario();

	gpMSound->startSoundSystemSE(MSD_SE_SY_TALK_MODE_IN, 0, nullptr, 0);

	mCurrentStage = stage;
	mState        = STATE_APPEARING;
	if (stage != -1 && stage < 10) {
		mCursorBlinkUp                = 0;
		mPointPanes[stage]->mAlpha = 255;
	}
}

// UNUSED: inlined into perform's STATE_SHOWING case, its only call site.
void TGuide::disappearGuidePane(int stage)
{
	mOpenPanelB->setPaneAlpha(20, 0, 255);

	JUTRect rect   = mPanelRects[stage];
	JUTRect bounds = mStagePanes[stage]->mBounds;

	gpMSound->startSoundSystemSE(MSD_SE_SY_TALK_MODE_OUT, 0, nullptr, 0);

	int height = rect.y2 - rect.y1;
	int width  = rect.x2 - rect.x1;
	mOpenPanelA->setCenteredSize(20, 0, 0, width, height);
	mOpenPanelA->setPaneOffset(20, bounds.x1 - rect.x1,
	                           bounds.y1 - rect.x1 - 40, 0, 0);
	mCursors[0]->setPaneAlpha(20, 255, 0);
	mCursors[1]->setPaneAlpha(20, 80, 0);
	mState = STATE_DISAPPEAR;
}

// TODO: open. Retail *calls* TExPane::setPaneAlpha, setPaneSize and
// setPaneOffset from here and from appearGuidePane -- setPaneSize and
// setPaneAlpha are the two weak symbols the map puts in this TU -- while
// inlining all three inside linkSelect's mmarkPattern. Ours expands them
// everywhere, which is the only remaining difference in this function. They
// are inline members of GC2D/ExPane.hpp, a shared header, so the fix is not
// local to this unit; see the wireTrap rule in docs/catalog/codegen-tells.md
// about MWCC refusing to inline once the caller is large enough.
void TGuide::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (setup_wait != 0) {
		setup_wait--;
		if (setup_wait == 0) {
			SMSSwitch2DArchive("game_6", gArBkGuide);
			unkC4         = 0;
			s16 stage     = SMS_getShineStage(gpMarDirector->mMap);
			mCurrentStage = stage;
			resetObjects();
			changeBotStatus(stage);
			for (int i = 0; i < 10; ++i) {
				if (i == stage)
					mMarkerPanes[i]->mVisible = true;
				else
					mMarkerPanes[i]->mVisible = false;
			}
			mCursorBlinkUp = 0;
		} else {
			return;
		}
	}

	if (cue & CUE_DRAW) {
		if (mState != STATE_MOVE_CURSOR && mState != STATE_CLOSED) {
			J2DOrthoGraph graph(graphics->getViewport());
			graph.setup2D();
			mScreen->draw(0, 0, &graph);
		}
	}

	if (!(cue & CUE_MOVE))
		return;

	u8 done = 1;
	switch (mState) {
	case STATE_MOVE_CURSOR:
		if (unkC5 && gpApplication.mFader->isFullyFadedOut()) {
			gpApplication.mFader->startWipe(5, 1.0f, 0.0f);
			mState = STATE_FADE_IN;
		}
		{
			JUTRect bounds
			    = mStagePanes[SMS_getShineStage(gpMarDirector->mMap)]
			          ->mBounds;
			mCursors[0]->getPane()->move(bounds.x1 + 6, bounds.y1 - 1);
			mCursors[1]->getPane()->move(bounds.x1 + 6, bounds.y1 - 1);
		}
		break;

	case STATE_FADE_IN:
		if (gpApplication.mFader->isFullyFadedIn()) {
			mState      = STATE_SELECT;
			mOpenPanelB = nullptr;
			mOpenPanelA = nullptr;
			mCursors[0]->getPane()->setAlpha(255);
			mCursors[1]->getPane()->setAlpha(80);
		}
		break;

	case STATE_SELECT:
		linkSelect();
		break;

	case STATE_APPEARING:
		done = done & mOpenPanelA->update();
		for (int i = 0; i < 2; ++i)
			done = done & mCursors[i]->update();
		if (done && mOpenPanelB->update())
			mState = STATE_SHOWING;
		break;

	case STATE_SHOWING:
		if (mGamePad->mEnabledFrameMeaning
		    & (TMarioGamePad::MEANING_0x20 | TMarioGamePad::MEANING_0x40)) {
			s16 stage = mCurrentStage;
			disappearGuidePane(stage);
		} else if (mGamePad->mButton.mTrigger & 0x10) {
			mState = STATE_CLOSE;
		}
		break;

	case STATE_DISAPPEAR:
		if (mOpenPanelB->update()) {
			done = done & mOpenPanelA->update();
			for (int i = 0; i < 2; ++i)
				done = done & mCursors[i]->update();
			if (done) {
				mOpenPanelA->getPane()->mVisible = false;
				mOpenPanelB->getPane()->mVisible = false;
				mState                           = STATE_SELECT;
				mTimer                           = 0;
			}
		}
		break;

	case STATE_CLOSE:
		gpApplication.mFader->startWipe(6, 1.0f, 0.0f);
		mGamePad->offFlag(TMarioGamePad::PAD_FLAG_0x80);
		gpMSound->startSoundSystemSE(MSD_SE_SY_WIPE_OUT, 0, nullptr, 0);
		mState = STATE_FADE_OUT;
		break;

	case STATE_FADE_OUT:
		if (gpApplication.mFader->isFullyFadedOut()) {
			gpApplication.mFader->startWipe(5, 1.0f, 0.0f);
			if (mOpenPanelA != nullptr
			    && mOpenPanelA->getPane()->mVisible)
				mOpenPanelA->getPane()->mVisible = false;
			if (mOpenPanelB != nullptr
			    && mOpenPanelB->getPane()->mVisible)
				mOpenPanelB->getPane()->mVisible = false;
			unkC4  = 1;
			mState = STATE_CLOSED;
		}
		break;
	}
}

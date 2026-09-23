#include <GC2D/Talk2D2.hpp>
#include <stdio.h>
#include <GC2D/BoundPane.hpp>
#include <GC2D/GCConsole2.hpp>
#include <GC2D/MessageLoader.hpp>
#include <GC2D/MessageUtil.hpp>
#include <Camera/Camera.hpp>
#include <MSound/BackgroundMusic.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/ReinitGX.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MoveBG/MapObjHide.hpp>
#include <MoveBG/MapObjHide.hpp>
#include <NPC/NpcBase.hpp>
#include <System/Application.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <System/MarioGamePad.hpp>
#include <JSystem/JGeometry/JGUtil.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>
#include <JSystem/JSupport/JSUMemoryOutputStream.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <JSystem/J2D/J2DPicture.hpp>
#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <JSystem/JUtility/JUTPoint.hpp>
#include <JSystem/JUtility/JUTResFont.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <System/DummyStrings.hpp>

TTalk2D2* gpTalk2D;

/**
 * @brief The voice clip played for each JMessage voice index.
 *
 * @details INF1's per-message voice byte indexes straight into this table;
 * -1 means "this message is silent". Entry 0x82 is the race fanfare, a BGM
 * rather than an SE, which is why setMessageID() has to test bit 31.
 */
static const s32 scTalkSoundList[] = {
	MSD_SE_NPC_VM_PEACH_NORMAL, MSD_SE_NPC_VM_PEACH_SURPRS,
	MSD_SE_NPC_VM_PEACH_WORRY, MSD_SE_NPC_VM_PEACH_ANGER_L,
	MSD_SE_NPC_VM_PEACH_APPEAL, MSD_SE_NPC_VM_PEACH_DOUBT,
	MSD_SE_NPC_VM_KINOJ_NORMAL, MSD_SE_NPC_VM_KINOJ_EXCIT,
	MSD_SE_NPC_VM_KINOJ_ADVICE, MSD_SE_NPC_VM_KINOJ_CONFUSE,
	MSD_SE_NPC_VM_KINOJ_ASK, MSD_SE_NPC_VM_KINOP_NORMAL,
	MSD_SE_NPC_VM_KINOP_CRY_S, MSD_SE_NPC_VM_KINOP_CRY_L,
	MSD_SE_NPC_VM_KINOP_PEACE, MSD_SE_NPC_VM_KINOP_SAD_S,
	MSD_SE_NPC_VM_KINOP_ASK, MSD_SE_NPC_VM_KINOP_CONFUSE,
	MSD_SE_NPC_VM_KINOP_RECOVER, MSD_SE_NPC_VM_KINOP_SAD_L,
	MSD_SE_NPC_VM_MONTM_NORMAL, MSD_SE_NPC_VM_MONTM_LAUGH_D,
	MSD_SE_NPC_VM_MONTM_DSIGST, MSD_SE_NPC_VM_MONTM_ANGER_S,
	MSD_SE_NPC_VM_MONTM_SURPRS, MSD_SE_NPC_VM_MONTM_DOUBT,
	MSD_SE_NPC_VM_MONTM_DISPLES, MSD_SE_NPC_VM_MONTM_CONFUSE,
	MSD_SE_NPC_VM_MONTM_REGRET, MSD_SE_NPC_VM_MONTM_PROUD,
	MSD_SE_NPC_VM_MONTM_RECOVER, MSD_SE_NPC_VM_MONTM_INVITN,
	MSD_SE_NPC_VM_MONTM_QUESTN, MSD_SE_NPC_VM_MONTM_LAUGH,
	MSD_SE_NPC_VM_MONTM_THANKS, MSD_SE_NPC_VM_MONTW_NORAML,
	MSD_SE_NPC_VM_MONTW_REGRET, MSD_SE_NPC_VM_MONTW_INVITN,
	MSD_SE_NPC_VM_MONTW_LAUGH, MSD_SE_NPC_VM_MAREM_NORMAL,
	MSD_SE_NPC_VM_MAREM_REGRET, MSD_SE_NPC_VM_MAREM_LAUGH,
	MSD_SE_NPC_VM_MAREM_APPEAL, MSD_SE_NPC_VM_MAREM_SURPRS,
	MSD_SE_NPC_VM_MAREM_SAD, MSD_SE_NPC_VM_MAREM_ASK,
	MSD_SE_NPC_VM_MAREM_PROMPT, MSD_SE_NPC_VM_MAREM_THANKS,
	MSD_SE_NPC_VM_MAREW_NORMAL, MSD_SE_NPC_VM_MAREW_REGRET,
	MSD_SE_NPC_VM_MAREW_LAUGH, MSD_SE_NPC_VM_MAREW_APPEAL,
	MSD_SE_NPC_VM_MAREW_SURPRS, MSD_SE_NPC_VM_MAREW_SAD,
	MSD_SE_NPC_VM_MAREW_ASK, MSD_SE_NPC_VM_MAREW_PROMPT,
	MSD_SE_NPC_VM_MAREW_THANKS, MSD_SE_NPC_VM_MAREJ_NORMAL,
	MSD_SE_NPC_VM_MAREJ_REGRET, MSD_SE_NPC_VM_MAREJ_LAUGH,
	MSD_SE_NPC_VM_MAREJ_APPEAL, MSD_SE_NPC_VM_MAREJ_SURPRS,
	MSD_SE_NPC_VM_MAREJ_SAD, MSD_SE_NPC_VM_MAREJ_ASK,
	MSD_SE_NPC_VM_MAREJ_PROMPT, MSD_SE_NPC_VM_MAREJ_THANKS,
	MSD_SE_NPC_VM_TANUK_NORMAL, MSD_SE_NPC_VM_SUNFP_JOY,
	MSD_SE_NPC_VM_SUNFP_SAD, -1, MSD_SE_NPC_VM_MONTW_LAUGH_D,
	MSD_SE_NPC_VM_MONTW_DSIGST, MSD_SE_NPC_VM_MONTW_ANGER_S,
	MSD_SE_NPC_VM_MONTW_SURPRS, MSD_SE_NPC_VM_MONTW_DOUBT,
	MSD_SE_NPC_VM_MONTW_DISPLES, MSD_SE_NPC_VM_MONTW_CONFUSE,
	MSD_SE_NPC_VM_MONTW_PROUD, MSD_SE_NPC_VM_MONTW_RECOVER,
	MSD_SE_NPC_VM_MONTW_QUESTN, MSD_SE_NPC_VM_MONTW_THANKS,
	MSD_SE_NPC_VM_CMNTM_NORMAL, MSD_SE_NPC_VM_CMNTM_LAUGH_D,
	MSD_SE_NPC_VM_CMNTM_DSIGST, MSD_SE_NPC_VM_CMNTM_ANGER_S,
	MSD_SE_NPC_VM_CMNTM_SURPRS, MSD_SE_NPC_VM_CMNTM_DOUBT,
	MSD_SE_NPC_VM_CMNTM_DISPLES, MSD_SE_NPC_VM_CMNTM_CONFUSE,
	MSD_SE_NPC_VM_CMNTM_REGRET, MSD_SE_NPC_VM_CMNTM_PROUD,
	MSD_SE_NPC_VM_CMNTM_RECOVER, MSD_SE_NPC_VM_CMNTM_INVITN,
	MSD_SE_NPC_VM_CMNTM_QUESTN, MSD_SE_NPC_VM_CMNTM_LAUGH,
	MSD_SE_NPC_VM_CMNTM_THANKS, MSD_SE_NPC_VM_CMNTW_NORAML,
	MSD_SE_NPC_VM_CMNTW_LAUGH_D, MSD_SE_NPC_VM_CMNTW_DSIGST,
	MSD_SE_NPC_VM_CMNTW_ANGER_S, MSD_SE_NPC_VM_CMNTW_SURPRS,
	MSD_SE_NPC_VM_CMNTW_DOUBT, MSD_SE_NPC_VM_CMNTW_DISPLES,
	MSD_SE_NPC_VM_CMNTW_CONFUSE, MSD_SE_NPC_VM_CMNTW_REGRET,
	MSD_SE_NPC_VM_CMNTW_PROUD, MSD_SE_NPC_VM_CMNTW_RECOVER,
	MSD_SE_NPC_VM_CMNTW_INVITN, MSD_SE_NPC_VM_CMNTW_QUESTN,
	MSD_SE_NPC_VM_CMNTW_LAUGH, MSD_SE_NPC_VM_CMNTW_THANKS,
	MSD_SE_NPC_VM_CMAREM_NORMAL, MSD_SE_NPC_VM_CMAREM_REGRET,
	MSD_SE_NPC_VM_CMAREM_LAUGH, MSD_SE_NPC_VM_CMAREM_APPEAL,
	MSD_SE_NPC_VM_CMAREM_SURPRS, MSD_SE_NPC_VM_CMAREM_SAD,
	MSD_SE_NPC_VM_CMAREM_ASK, MSD_SE_NPC_VM_CMAREM_PROMPT,
	MSD_SE_NPC_VM_CMAREM_THANKS, MSD_SE_NPC_VM_CMAREW_NORMAL,
	MSD_SE_NPC_VM_CMAREW_REGRET, MSD_SE_NPC_VM_CMAREW_LAUGH,
	MSD_SE_NPC_VM_CMAREW_APPEAL, MSD_SE_NPC_VM_CMAREW_SURPRS,
	MSD_SE_NPC_VM_CMAREW_SAD, MSD_SE_NPC_VM_CMAREW_ASK,
	MSD_SE_NPC_VM_CMAREW_PROMPT, MSD_SE_NPC_VM_CMAREW_THANKS,
	MSD_SE_SY_COLLECT_DELIGHT, MSD_BGM_FANFARE_CASINO, MSD_SE_SY_NOT_COLLECT,
	MSD_SE_NPC_VM_FMARIO_PSHOT, MSD_SE_NPC_VM_MTMAN_NORMAL,
	MSD_SE_NPC_VM_MTMAN_LOST,
};

/// The six colours a `\033\x02\xff\x00` colour tag can select.
u32 TTalk2D2::cColorTable[6] = {
	0xffffffff, 0xffffffff, 0xffb48cff, 0x6ee6ffff, 0xffff00ff, 0xaaff50ff,
};

TTalk2D2::TTalk2D2(const char* name)
    : JDrama::TViewObj(name)
    , mIsBoard(false)
    , mScreen(nullptr)
    , mBasePane(nullptr)
    , mCharStep(0.0f)
    , mSelectedValue(-1)
    , unk220(26)
    , unk222(0)
    , mTalkMode(TALK_MODE_IDLE)
    , mGamePad(nullptr)
    , mNeedsPrepass(true)
    , mWaitTimer(0)
    , mIsTalking(false)
    , mMesgEntry(nullptr)
    , mMessageID(3)
    , mIsLastPage(false)
    , mCursorBlinkUp(false)
    , mFastForward(false)
    , mForceClose(false)
    , mFlags(1)
    , mCurrentLine(0)
    , mTextOffset(0)
    , mCharColor(0xffffffff)
    , mCharDelay(0)
    , mCharTimer(0)
    , mBaseX(0)
    , mBaseY(0)
    , mBaseRotation(0)
    , mProgressStep(0.04f)
    , mLineDelay(1.0f)
    , mAlphaStep(100)
{
	gpTalk2D = this;

	for (int i = 0; i < CHAR_NUM; i++)
		mCharBox[i] = nullptr;

	for (int i = 0; i < LINE_NUM; i++) {
		mCharCursor[i] = 0;
		mCursor[i]     = nullptr;
		mCursorOff[i]  = nullptr;
		mLineLength[i] = 0;
	}
}

void TTalk2D2::load(JSUMemoryInputStream& stream)
{
	JDrama::TNameRef::load(stream);

	JKRArchive* archive = (JKRArchive*)JKRFileLoader::getVolume("game_6");

	mScreen = new J2DSetScreen("message_2.blo", archive);
	for (int i = 0; i < LINE_NUM; i++) {
		mLinePane[i]    = mScreen->search('me_1' + i);
		mBackPane[i]    = mScreen->search('bac1' + i);
		mBezierStart[i] = mScreen->search('f_1' + i * 3);
		mBezierCtrl[i]  = mScreen->search('f_2' + i * 3);
		mBezierEnd[i]   = mScreen->search('f_3' + i * 3);
		mCursor[i]      = mScreen->search('cu_1' + i);
		mCursorOff[i]   = mScreen->search('cc_1' + i);
		mCursorOn[i]    = mScreen->search('cs_1' + i);
	}

	mBackTexture = new JUTTexture((ResTIMG*)JKRFileLoader::getGlbResource(
	    "/game_6/timg/message_back_1.bti"));

	mBasePane = mScreen->search('me_0');

	mMapMessage = new TMessageLoader();
	mMapMessage->loadMessageData("/scene/map/message.bmg");
	mSysMessage = new TMessageLoader();
	mSysMessage->loadMessageData("/common/2d/sys_message.bmg");

	mSelectPane    = mScreen->search('me_4');
	mSelectTextBox = (J2DTextBox*)mScreen->search('slct');
	mSelectTextBox->setFont((JUTFont*)gpSystemFont);
	for (int i = 0; i < 2; i++) {
		mSelectCursor[i] = mScreen->search('sc_1' + i);
		mSelectString[i] = new char[17];
	}

	mBoardScreen = new J2DSetScreen("message_board_1.blo", archive);
	mBoardBound  = new TBoundPane(mBoardScreen, 'mb_0');

	mBoardTextBox = (J2DTextBox*)mBoardScreen->search('text');
	SMSMakeTextBuffer(mBoardTextBox, 0x200);
	mBoardTextBox->setFont((JUTFont*)gpSystemFont);

	mBoardCursor    = mBoardScreen->search('cu_1');
	mBoardCursorOff = mBoardScreen->search('cc_1');
	mBoardCursorOn  = mBoardScreen->search('cs_1');
}

void TTalk2D2::loadAfter()
{
	JDrama::TNameRef::loadAfter();

	// Walk the middle line's Bezier once at 1/100 steps; the reciprocal of
	// its arc length is how far one pixel of glyph width advances t.
	JUTPoint start(mBezierStart[1]->getBounds().x1,
	               mBezierStart[1]->getBounds().y1);
	JUTPoint ctrl(0, mBezierCtrl[1]->getBounds().y1 - unk220);
	JUTPoint end(mBezierEnd[1]->getBounds().x2 - 10,
	             mBezierEnd[1]->getBounds().y1);

	f32 f3f4x;
	f32 f3f4y;
	makeLine(&f3f4x, &f3f4y, 0.0f, start, ctrl, end);

	f32 length = 0.0f;
	for (f32 t = 0.01f; t <= 1.0f; t += 0.01f) {
		f32 x;
		f32 y;
		makeLine(&x, &y, t, start, ctrl, end);
		f32 dx = x - f3f4x;
		f32 dy = y - f3f4y;
		length += JGeometry::TUtil<f32>::sqrt(dx * dx + dy * dy);
		f3f4x = x;
		f3f4y = y;
	}
	mCharStep = 1.0f / length;

	for (int i = 0; i < LINE_NUM; i++) {
		mBezierStart[i]->hide();
		mBezierCtrl[i]->hide();
		mBezierEnd[i]->hide();
	}

	for (int i = 0; i < CHAR_NUM; i++) {
		mCharBox[i] = new J2DTextBox(0, JUTRect(0, 0, 20, 20),
		                             gpSystemFont->getResFont(), "あ",
		                             HBIND_LEFT, VBIND_CENTER);
		mCharBox[i]->setFontSize(20, 24);
		mCharBox[i]->setBlackWhite(0xffffffff, 0xffffff00);
		mCharBox[i]->hide();
	}

	for (int i = 0; i < LINE_NUM; i++) {
		mLineProgress[i] = i + 1;
		mCursor[i]->hide();
	}

	mBackTexture->mWrapT = 1;
	mBackTexture->mWrapS = 0;

	mBasePane->move(393, 115);
	mBasePane->mRotation = -18.0f;
	mBaseX               = mBasePane->getBounds().x1;
	mBaseY               = mBasePane->getBounds().y1;
	mBaseRotation        = mBasePane->getRotation();

	mSelectPane->hide();

	char* select = new char[94];
	for (int i = 0; i < 93; i++)
		select[i] = ' ';
	select[93] = '\0';
	mSelectTextBox->setString(select);

	setupTextBox(mSysMessage->getMessageData(), mSysMessage->getMessageEntry(3));
	mCurMessage = mSysMessage;

	const char* names[10] = {
		"空港沈みモンテ",
		"モンテ1", "", "", "", "", "", "", "", "",
	};
	u32 ids[10] = {
		0x2b, 0x28, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	for (int i = 0; i < 10; i++) {
		mNpcMessages[i].mNpc = JDrama::TNameRefGen::search2(names[i]);
		mNpcMessages[i].mMessageID = ids[i];
	}
}

void TTalk2D2::setMessageID(u32 message_id, u32 flags)
{
	TBaseNPC* npc = SMSGetMarDirector()->getTalkingNPC();

	bool actionFlag = npc->checkActionFlag(0x200);
	if (actionFlag) {
		if (npc->isMonte()) {
			if (npc->isNormalMonteW() || npc->isSpecialMonteW()) {
				if (npc->isChild())
					mMessageID = 0x31;
				else
					mMessageID = 0x2c;
			} else if (npc->isChild()) {
				mMessageID = 0x2e;
			} else {
				mMessageID = 0x28;
			}
		} else if (npc->isMare()) {
			if (npc->isNormalMareW() || npc->isSpecialMareW()) {
				if (npc->isChild())
					mMessageID = 0x32;
				else
					mMessageID = 0x2d;
			} else if (npc->isChild()) {
				mMessageID = 0x2f;
			} else {
				mMessageID = 0x29;
			}
		} else if (npc->getActorType() == 0x4000016) {
			mMessageID = 0x2a;
		} else if (npc->getActorType() == 0x4000010) {
			mMessageID = 0x30;
		}

		for (int i = 0; i < 10; i++) {
			if (npc == mNpcMessages[i].mNpc) {
				mMessageID = mNpcMessages[i].mMessageID;
				break;
			}
		}
	} else {
		mMessageID = message_id;
	}

	if (npc->getActorType() == 0x400001d)
		mIsBoard = true;
	else
		mIsBoard = false;

	mFlags         = flags;
	mTextOffset    = 0;
	mSelectedValue = -1;
	mIsLastPage    = false;
	mCharColor     = 0xffffffff;
	mCharDelay     = 0;
	mFastForward   = false;
	mForceClose    = false;
	mMesgEntry     = nullptr;
	mBoardStep     = 0;
	mAlphaStep     = 0x40;

	TMessageLoader* loader;
	if (!(mMessageID & 0xffff0000))
		loader = mSysMessage;
	else
		loader = mMapMessage;

	if (loader->getMessageData() != nullptr) {
		JMSMesgEntry* entry = loader->getMessageEntry(mMessageID & 0xffff);
		if (entry == nullptr) {
			mMessageID = 4;
			loader     = mSysMessage;
			entry = loader->getMessageEntry(mMessageID & 0xffff);
		}
		setupTextBox(loader->getMessageData(), entry);
	} else {
		mMessageID = 3;
		loader     = mSysMessage;
		setupTextBox(loader->getMessageData(),
		    loader->getMessageEntry(mMessageID & 0xffff));
	}
	mCurMessage = loader;
	mCharTimer  = 0;

	if (mMesgEntry != nullptr) {
		s32 soundID = scTalkSoundList[mMesgEntry->mVoiceIndex];
		if (soundID != -1 && gpMSound->gateCheck(soundID)) {
			if (soundID & 0x80000000)
				MSBgm::startBGM(soundID);
			else
				MSoundSESystem::MSoundSE::startSoundSystemSE(soundID, 0,
				                                            nullptr, 0);
		}
	}

	if (mTalkMode == TALK_MODE_READY) {
		mTalkMode = TALK_MODE_WAIT_OPEN;
		mBasePane->setAlpha(255);
	}

	mIsTalking = true;
}

// Binding level over a raw member read, worth +16 of low region in
// TTalk2D2::forceCloseTalk (batch 127).
static inline MSound* Talk2D2GetMSound()
{
	MSound* mSound = gpMSound;
	return mSound;
}

void TTalk2D2::forceCloseTalk()
{
	gpCamera->makeMtxForPrevTalk();

	if (mIsBoard)
		Talk2D2GetMSound()->startSoundSystemSE(MSD_SE_SY_RACE_START, 0, nullptr, 0);
	else
		Talk2D2GetMSound()->talkModeOut();

	gpMarDirector->getConsole()->startAppearTelop(false);

	if (mTalkMode == TALK_MODE_READY) {
		mTalkMode = TALK_MODE_IDLE;
		return;
	}
	mTalkMode = TALK_MODE_CLOSING;
}

void TTalk2D2::closeTalkWindow()
{
	if (mFlags & 1) {
		if (mMessageID == 0x1e) {
			gpMSound->startSoundSystemSE(MSD_SE_SY_RACE_START, 0, nullptr, 0);
		} else if (mIsBoard) {
			gpMSound->startSoundSystemSE(MSD_SE_SY_2D_OUT, 0, nullptr, 0);
			gpMSound->talkModeOut();
		} else {
			gpMSound->talkModeOut();
		}
		gpCamera->makeMtxForPrevTalk();
		gpMarDirector->getConsole()->startAppearTelop(false);
		SMSRumbleMgr->finishPause();
		mIsTalking = false;
	}
	mTalkMode = TALK_MODE_CLOSING;
}

void TTalk2D2::openTalkWindow(TBaseNPC* npc)
{
	if (npc != nullptr)
		gpCamera->makeMtxForTalk(npc);

	if (mIsBoard) {
		mBoardBound->setPanePosition(60, JUTPoint(0, -800), JUTPoint(0, 80),
		                             JUTPoint(0, 80));
		mBoardBound->update();
		mBoardCursor->setAlpha(0);
		mTalkMode = TALK_MODE_OPENING;
	} else {
		mTalkMode = TALK_MODE_WAIT_CAMERA;
	}

	mBasePane->setAlpha(255);

	switch (gpCamera->mMode) {
	case 45:
		mBaseX        = 160;
		mBaseY        = 135;
		mBaseRotation = 20;
		break;
	case 12:
	default:
		mBaseX        = 388;
		mBaseY        = 115;
		mBaseRotation = -18;
		break;
	}

	mBasePane->move(mBaseX, mBaseY);
	mBasePane->mRotation = mBaseRotation;
	mNeedsPrepass        = true;

	gpMarDirector->getConsole()->startDisappearTelop();
	gpMarDirector->getConsole()->startDisappearBalloon(
	    gpMarDirector->getConsole()->unk3E0, true);
	gpMarDirector->getConsole()->startDisappearMario();

	if (mMessageID == 0x1e) {
		gpMSound->startSoundSystemSE(MSD_SE_SY_PROBLEM_SIGN, 0, nullptr, 0);
	} else if (mIsBoard) {
		gpMSound->startSoundSystemSE(MSD_SE_SY_2D_IN, 0, nullptr, 0);
		gpMSound->talkModeIn(false);
	} else {
		gpMSound->talkModeIn(true);
	}

	SMSRumbleMgr->startPause();
}

// TODO: literal-pool order. The target asks for -0.5f (@4525) before 0.5f
// (@4526) inside this function; ours reverses the pair. Values and count
// are otherwise identical, so it is a spelling/order question in the
// bezier maths, not a wrong constant.
void TTalk2D2::makeBoxLine(s8 line, char* text)
{
	JUTPoint start(mBezierStart[line]->getBounds().x1,
	               mBezierStart[line]->getBounds().y1);
	JUTPoint ctrl(0, mBezierCtrl[line]->getBounds().y1 - unk220);
	JUTPoint end(mBezierEnd[line]->getBounds().x2 - 10,
	             mBezierEnd[line]->getBounds().y1);

	int src   = 0;
	int col   = 0;
	f32 t     = 0.0f;
	f32 prevX;
	f32 prevY;
	makeLine(&prevX, &prevY, 0.0f, start, ctrl, end);

	do {
		int idx = col + line * LINE_LENGTH;

		if (text != nullptr) {
			char* dst = mCharBox[idx]->getStringPtr();
			char* p   = &text[src];
			dst[0]    = p[0];
			if ((u8)p[0] >= 0x80) {
				src += 2;
				dst[1] = p[1];
			} else {
				dst[1] = '\0';
				src += 1;
			}
		}

		char* str = mCharBox[idx]->getStringPtr();
		// TODO: retail has a dead `>= 0x80` arm here that compiles to a
		// self-move of the already-loaded byte; presumably the two-byte
		// Shift-JIS code the width lookup wants was assembled there and the
		// expression was lost (or folded).  Reproduced as written.
		u16 code = str[0];
		if (code >= 0x80)
			code = str[0];
		if (code == 0)
			break;

		// getWidth(int) would return an int and convert with the signed
		// magic; retail converts the raw u8 width, so the entry is read
		// here instead of going through the wrapper.
		JUTFont::TWidth width;
		gpSystemFont->getWidthEntry(code, &width);
		t += mCharStep * (0.7f * width.field_0x1 + 4.0f);

		f32 curX;
		f32 curY;
		makeLine(&curX, &curY, t, start, ctrl, end);

		f32 angle = fabsf(atan2f(curY - prevY, curX - prevX));
		if (curX > 0.0f)
			angle *= -1.0f;

		// TODO: retail computes negHalfX/negHalfY before cosf and keeps
		// them in callee-saved FPRs, i.e. they are not forwarded into their
		// single use as they are here (an inline parameter would do that);
		// the extra register pressure is retail's larger spill area.
		// Rotate the glyph's anchor about the midpoint of the segment it
		// sits on.  The -0.5f and +0.5f terms cancel in exact arithmetic
		// but retail emits both, so they were written out.
		f32 sumX     = prevX + curX;
		f32 sumY     = prevY + curY;
		f32 negHalfX = -0.5f * sumX;
		f32 negHalfY = -0.5f * sumY;
		f32 halfX    = 0.5f * sumX;
		f32 halfY    = 0.5f * sumY;
		f32 cos      = cosf(angle);
		f32 sin      = sinf(angle);
		f32 rotX     = negHalfX + (prevX * cos + prevY * -sin) + halfX;
		f32 rotY     = negHalfY + (prevX * sin + prevY * cos) + halfY;

		s16 x = rotX + (rotX > 0.0f ? 0.5f : -0.5f);
		s16 y = rotY + (rotY > 0.0f ? 0.5f : -0.5f);
		mCharBox[idx]->move(x, -40 - y);
		mCharBox[idx]->setBasePosition(J2DBasePosition_4);
		mCharBox[idx]->mRotation = 180.0f * angle / 3.1415927f;
		mLinePane[line]->mPaneTree.appendChild(&mCharBox[idx]->mPaneTree);

		if (t > 1.05f) {
			if (mLineLength[line] > col)
				mLineLength[line] = col;
			break;
		}

		col++;
		makeLine(&prevX, &prevY, t, start, ctrl, end);
	} while (col < LINE_LENGTH);
}

bool TTalk2D2::openBoardWindow()
{
	bool done = false;

	switch (mBoardStep) {
	case 0:
		if (mBoardBound->update()) {
			mBoardBound->setPanePosition(25, JUTPoint(0, 80), JUTPoint(0, 80),
			                             JUTPoint(0, 0));
			mBoardStep++;
		}
		break;
	case 1:
		if (mBoardBound->update()) {
			mBoardCursor->setAlpha(0);
			done = true;
			mBoardCursor->show();
			mBoardStep++;
		}
		break;
	}

	return done;
}

bool TTalk2D2::openNormalWindow()
{
	bool done = false;

	if (mCharIndex > 2
	    && (mGamePad->mMeaning & TMarioGamePad::MEANING_0x20000)) {
		mFastForward = true;
		mAlphaStep   = 0x80;
	}

	bool allIn = true;
	for (int i = 0; i <= mCurrentLine; i++) {
		if (mLineProgress[i] > -0.109f) {
			allIn = false;
			mLineProgress[i] -= mProgressStep;
		}

		// Hold a line closed until the last character of the one above it
		// has appeared.
		if (mLineProgress[i] < 1.0f && i != 0
		    && !mCharBox[mLineLength[i - 1] + (i - 1) * LINE_LENGTH]
		            ->isVisible())
			mLineProgress[i] = 1.0f;

		if (mCharCursor[i] == 0 && mLineProgress[i] < mLineDelay) {
			gpMSound->startSoundSystemSE(MSD_SE_SY_TALK_OBI, 0, nullptr, 0);
			mCharCursor[i] = 1;
			mCharBox[i * LINE_LENGTH]->show();
		}
	}

	if (allIn) {
		for (int i = 0; i <= mCurrentLine; i++)
			mBackPane[i]->show();
		done = true;
	}

	for (int i = 0; i < LINE_NUM; i++) {
		if (mCharCursor[i] == 0)
			continue;

		int idx = mCharCursor[i] + i * LINE_LENGTH;
		while (mCharCursor[i] <= mLineLength[i]) {
			if (mCharBox[idx]->isVisible()) {
				s16 alpha = mCharBox[idx]->getAlpha();
				alpha += mAlphaStep;
				mCharBox[idx]->setAlpha(alpha > 255 ? 255 : alpha);
				mCharIndex = idx;
				if (alpha < 255)
					break;
				idx++;
				mCharCursor[i]++;
			} else {
				if (mCharTimer <= 0) {
					if (mFastForward)
						mCharTimer = 0;
					else
						mCharTimer = mCharDelays[idx];
					mCharBox[idx]->show();
					mCharBox[idx]->setAlpha(0);
				} else {
					mCharTimer--;
				}
				break;
			}
		}
	}

	return done;
}

void TTalk2D2::moveBoardWindow()
{
	int alpha;
	if ((alpha = mBoardCursor->getAlpha()) < 255) {
		alpha += 4;
		if (alpha > 255) {
			mBoardCursorOn->setAlpha(0);
			alpha = 255;
			mBoardCursorOff->setAlpha(0);
			mCursorBlinkUp = true;
		}
		mBoardCursor->setAlpha(alpha);
		return;
	}

	J2DPane* cursor;
	if (mIsLastPage)
		cursor = mBoardCursorOn;
	else
		cursor = mBoardCursorOff;

	alpha = cursor->getAlpha();
	if (mCursorBlinkUp) {
		alpha += 4;
		if (alpha > 255) {
			mCursorBlinkUp = false;
			alpha          = 255;
		}
	} else {
		alpha -= 4;
		if (alpha < 0) {
			mCursorBlinkUp = true;
			alpha          = 0;
		}
	}
	cursor->setAlpha(alpha);
}

void TTalk2D2::checkBoardControler()
{
	if (mIsLastPage) {
		if (mForceClose
		    || (mGamePad->mEnabledFrameMeaning
		        & TMarioGamePad::MEANING_0x20000)
		    || (mGamePad->mEnabledFrameMeaning
		        & TMarioGamePad::MEANING_0x40000)) {
			gpMSound->startSoundSystemSE(MSD_SE_SY_SELECT_COMMON, 0, nullptr,
			                             0);
			mBoardBound->setPanePosition(60, JUTPoint(0, 0), JUTPoint(0, 0),
			                             JUTPoint(0, -600));
			// TODO: closeTalkWindow()'s body, spelled out.  Calling the
			// helper is size-exact, but it shrinks checkBoardControler
			// enough for MWCC to inline the whole of it into perform(),
			// which retail calls out of line; with the statements spelled
			// out the per-callee budget refuses the expansion.  Same
			// pattern as TBombHei::bombIn in docs/catalog.
			if (mFlags & 1) {
				if (mMessageID == 0x1e) {
					gpMSound->startSoundSystemSE(MSD_SE_SY_RACE_START, 0,
					                             nullptr, 0);
				} else if (mIsBoard) {
					gpMSound->startSoundSystemSE(MSD_SE_SY_2D_OUT, 0, nullptr,
					                             0);
					gpMSound->talkModeOut();
				} else {
					gpMSound->talkModeOut();
				}
				gpCamera->makeMtxForPrevTalk();
				gpMarDirector->getConsole()->startAppearTelop(false);
				SMSRumbleMgr->finishPause();
				mIsTalking = false;
			}
			mTalkMode = TALK_MODE_CLOSING;
		}
	} else if (mGamePad->mEnabledFrameMeaning
	           & (TMarioGamePad::MEANING_0x20000
	              | TMarioGamePad::MEANING_0x40000)) {
		gpMSound->startSoundSystemSE(MSD_SE_SY_SELECT_COMMON, 0, nullptr, 0);
		mTalkMode = TALK_MODE_ERASING;
	}
}

void TTalk2D2::moveTalkWindow()
{
	for (int i = 0; i < LINE_NUM; i++) {
		int idx = mCharCursor[i] + i * LINE_LENGTH;
		if (mCharCursor[i] == 0)
			continue;
		if (mCharCursor[i] > mLineLength[i])
			continue;

		J2DTextBox*& box = mCharBox[idx];
		if (box->isVisible()) {
			s16 alpha = box->getAlpha();
			alpha += mAlphaStep;
			box->setAlpha(alpha > 255 ? 255 : alpha);
			if (alpha >= 255)
				mCharCursor[i]++;
		} else if (mCharTimer <= 0) {
			mCharTimer = mCharDelays[idx];
			box->show();
			box->setAlpha(0);
		} else {
			mCharTimer--;
		}
	}

	if (mCharCursor[mCurrentLine] >= LINE_LENGTH
	    || mCharCursor[mCurrentLine] > mLineLength[mCurrentLine]) {
		J2DPane* pane;
		J2DPane* cursor;
		if (mSelectedValue == -1) {
			pane = mCursor[mCurrentLine];
			if (mIsLastPage) {
				cursor = mCursorOn[mCurrentLine];
				mCursorOff[mCurrentLine]->hide();
				mCursorOn[mCurrentLine]->show();
			} else {
				cursor = mCursorOff[mCurrentLine];
				cursor->show();
				mCursorOn[mCurrentLine]->hide();
			}
		} else {
			pane   = mSelectPane;
			cursor = mSelectCursor[mSelectedValue];
		}

		if (pane->isVisible()) {
			s16 paneAlpha = pane->getAlpha();
			if (paneAlpha < 255) {
				paneAlpha += 16;
				if (paneAlpha >= 255)
					paneAlpha = 255;
				pane->setAlpha(paneAlpha);
			}

			s16 alpha = cursor->getAlpha();
			if (mCursorBlinkUp) {
				alpha += 2;
				if (alpha > 255) {
					mCursorBlinkUp = false;
					alpha          = 255;
				}
			} else {
				alpha -= 4;
				if (alpha < 60) {
					mCursorBlinkUp = true;
					alpha          = 60;
				}
			}
			cursor->setAlpha(alpha);

			if (mSelectedValue == 1) {
				snprintf(
				    mSelectTextBox->getStringPtr(), 94,
				    "\033CC[ffffff60]\033GC[ffffff60]%s\033CC[ffffff%02x]"
				    "\033GC[ffffff%02x]\n%s",
				    mSelectString[0], (u8)alpha, (u8)alpha,
				    mSelectString[1]);
			} else if (mSelectedValue == 0) {
				snprintf(mSelectTextBox->getStringPtr(), 94,
				         "\033CC[ffffff%02x]\033GC[ffffff%02x]%s\n"
				         "\033CC[ffffff60]\033GC[ffffff60]%s",
				         (u8)alpha, (u8)alpha, mSelectString[0],
				         mSelectString[1]);
			}
		} else {
			pane->show();
			pane->setAlpha(0);
			cursor->setAlpha(255);
		}
	}
}

void TTalk2D2::checkControler()
{
	if (mCursor[mCurrentLine]->isVisible()) {
		if (mIsLastPage) {
			if (mForceClose
			    || (mGamePad->mEnabledFrameMeaning
			        & TMarioGamePad::MEANING_0x20000)
			    || (mGamePad->mEnabledFrameMeaning
			        & TMarioGamePad::MEANING_0x40000)) {
				gpMSound->startSoundSystemSE(MSD_SE_SY_SELECT_COMMON, 0,
				                             nullptr, 0);
				closeTalkWindow();
			}
		} else if (mGamePad->mEnabledFrameMeaning
		           & (TMarioGamePad::MEANING_0x20000
		              | TMarioGamePad::MEANING_0x40000)) {
			gpMSound->startSoundSystemSE(MSD_SE_SY_SELECT_COMMON, 0, nullptr,
			                             0);
			mTalkMode = TALK_MODE_ERASING;
		}
	} else if (mSelectPane->isVisible()) {
		u32 meaning = mGamePad->mEnabledFrameMeaning;

		if ((meaning & TMarioGamePad::MEANING_0x80000)
		    && mSelectedValue == 1) {
			gpMSound->startSoundSystemSE(MSD_SE_SY_E3_MENU_CURSOR, 0, nullptr,
			                             0);
			mSelectedValue = 0;
			mSelectCursor[1]->setAlpha(254);
			mSelectCursor[1]->hide();
			mSelectCursor[0]->show();
			return;
		}

		if ((meaning & TMarioGamePad::MEANING_0x100000)
		    && mSelectedValue == 0) {
			gpMSound->startSoundSystemSE(MSD_SE_SY_E3_MENU_CURSOR, 0, nullptr,
			                             0);
			mSelectedValue = 1;
			mSelectCursor[0]->setAlpha(254);
			mSelectCursor[0]->hide();
			mSelectCursor[1]->show();
			return;
		}

		if (mIsLastPage) {
			if (meaning
			    & (TMarioGamePad::MEANING_0x20000
			       | TMarioGamePad::MEANING_0x40000)) {
				if (mFlags & 1)
					gpCamera->makeMtxForPrevTalk();
				gpMSound->startSoundSystemSE(MSD_SE_SY_SELECT_COMMON, 0,
				                             nullptr, 0);
				mTalkMode = TALK_MODE_CLOSING;
			}
		} else if (meaning
		           & (TMarioGamePad::MEANING_0x20000
		              | TMarioGamePad::MEANING_0x40000)) {
			gpMSound->startSoundSystemSE(MSD_SE_SY_SELECT_COMMON, 0, nullptr,
			                             0);
			mTalkMode = TALK_MODE_ERASING;
		}
	}
}

bool TTalk2D2::closeNormalWindow()
{
	bool done   = false;
	s16 alpha   = mBasePane->getAlpha();
	alpha -= 16;

	if (alpha < 0) {
		mLineProgress[0] = 1.0f;
		alpha            = 0;
		mBackPane[0]->hide();
		mCharCursor[0] = 0;
		mCursor[0]->hide();
		mLineProgress[1] = 2.0f;
		mBackPane[1]->hide();
		mCharCursor[1] = 0;
		mCursor[1]->hide();
		mLineProgress[2] = 3.0f;
		mBackPane[2]->hide();
		mCharCursor[2] = 0;
		mCursor[2]->hide();

		for (int i = 0; i < CHAR_NUM; i++) {
			if (mCharBox[i] != nullptr)
				mCharBox[i]->hide();
		}

		if (mSelectPane->isVisible())
			mSelectPane->hide();

		done = true;
	}

	mBasePane->setAlpha(alpha);
	if (mSelectPane->isVisible())
		mSelectPane->setAlpha(alpha);

	return done;
}

bool TTalk2D2::closeBoardWindow()
{
	bool done = false;
	if (mBoardBound->update())
		done = true;
	return done;
}

bool TTalk2D2::eraseNormalWindow()
{
	bool done = false;
	s16 alpha = mBasePane->getAlpha();
	alpha -= 16;

	if (alpha < 0) {
		mLineProgress[0] = 1.0f;
		alpha            = 255;
		mBackPane[0]->hide();
		mCharCursor[0] = 0;
		mCursor[0]->hide();
		mLineProgress[1] = 2.0f;
		mBackPane[1]->hide();
		mCharCursor[1] = 0;
		mCursor[1]->hide();
		mLineProgress[2] = 3.0f;
		mBackPane[2]->hide();
		mCharCursor[2] = 0;
		mCursor[2]->hide();

		for (int i = 0; i < CHAR_NUM; i++) {
			if (mCharBox[i] != nullptr)
				mCharBox[i]->hide();
		}

		setupTextBox(mCurMessage->getMessageData(),
		    mCurMessage->getMessageEntry((u16)mMessageID));
		mFastForward = false;
		mAlphaStep   = 0x40;
		done         = true;
		mCharColor   = 0xffffffff;
		mCharIndex   = 0;
		mCharTimer   = 0;
	}

	mBasePane->setAlpha(alpha);
	return done;
}

bool TTalk2D2::eraseBoardWindow()
{
	bool done = false;
	s16 alpha = mBoardTextBox->getAlpha();
	alpha -= 4;

	if (alpha < 0) {
		alpha = 0;
		setupTextBox(mCurMessage->getMessageData(),
		    mCurMessage->getMessageEntry((u16)mMessageID));
		mCharColor = 0xffffffff;
		done       = true;
		mCharIndex = 0;
		mCharTimer = 0;
	}

	mBoardTextBox->setAlpha(alpha);
	return done;
}

bool TTalk2D2::appearBoardBoxWindow()
{
	bool done = false;
	u16 alpha = mBoardTextBox->getAlpha() + 4;
	if (alpha > 255) {
		alpha = 255;
		done  = true;
	}
	mBoardTextBox->setAlpha(alpha);
	return done;
}

// The calc-anim cue's mode switch sits one inline level below perform (the
// level is worth +0.2% of perform's registers).
// TODO: retail's pivot tree keeps a `cmpwi 2; bge` leaf for an empty case 2
// arm landing on the switch's end; an empty `case`/`break` or `return` arm,
// in any position, with or without `default:`, is folded away here.
static inline void Talk2D2CalcAnim(TTalk2D2* p)
{
	switch (p->mTalkMode) {
	case TTalk2D2::TALK_MODE_WAIT_OPEN:
		p->mWaitTimer--;
		if (p->mWaitTimer < 0) {
			p->mCharIndex = 0;
			p->mTalkMode  = TTalk2D2::TALK_MODE_OPENING;
		}
		break;

	case TTalk2D2::TALK_MODE_WAIT_CAMERA:
		break;

	case TTalk2D2::TALK_MODE_ERASING: {
		// eraseNormalWindow()'s body again, with the window left
		// fully opaque for the next page instead of returning.
		s16 alpha = p->mBasePane->getAlpha();
		alpha -= 16;
		if (alpha < 0) {
			p->mLineProgress[0] = 1.0f;
			alpha            = 255;
			p->mBackPane[0]->hide();
			p->mCharCursor[0] = 0;
			p->mCursor[0]->hide();
			p->mLineProgress[1] = 2.0f;
			p->mBackPane[1]->hide();
			p->mCharCursor[1] = 0;
			p->mCursor[1]->hide();
			p->mLineProgress[2] = 3.0f;
			p->mBackPane[2]->hide();
			p->mCharCursor[2] = 0;
			p->mCursor[2]->hide();

			for (int i = 0; i < TTalk2D2::CHAR_NUM; i++) {
				if (p->mCharBox[i] != nullptr)
					p->mCharBox[i]->hide();
			}

			p->setupTextBox(p->mCurMessage->getMessageData(),
			    p->mCurMessage->getMessageEntry((u16)p->mMessageID));
			p->mFastForward = false;
			p->mAlphaStep   = 0x40;
			p->mCharColor   = 0xffffffff;
			p->mCharColor   = 0xffffffff;
			p->mCharIndex   = 0;
			p->mCharTimer   = 0;
			p->mTalkMode    = TTalk2D2::TALK_MODE_OPENING;
		}
		p->mBasePane->setAlpha(alpha);
		break;
	}
	}
}

void TTalk2D2::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		// The one-case switch is the right shape and the field really is a
		// u8: retail loads it with `lbz` and compares with `cmpwi 2`, which
		// is what this reproduces exactly. (An int or an -enum int at 0x124
		// is impossible anyway, since unk125 and unk126 follow it.)
		switch (gpMarDirector->unk124) {
		case 2:
			switch (mTalkMode) {
			case TALK_MODE_WAIT_CAMERA:
				if (gpCamera->isThing()) {
					mWaitTimer = 20;
					mTalkMode  = TALK_MODE_WAIT_OPEN;
				}
				break;

			case TALK_MODE_OPENING:
				if (mIsBoard ? openBoardWindow() : openNormalWindow())
					mTalkMode = TALK_MODE_OPEN;
				break;

			case TALK_MODE_OPEN:
				if (mIsBoard) {
					moveBoardWindow();
					checkBoardControler();
				} else {
					moveTalkWindow();
					checkControler();
				}
				break;

			case TALK_MODE_CLOSING: {
				// closeBoardWindow()'s body; see checkBoardControler() for
				// why the helpers are spelled out at their call sites.
				bool closed;
				if (mIsBoard) {
					closed = false;
					if (mBoardBound->update())
						closed = true;
				} else {
					closed = closeNormalWindow();
				}
				if (closed) {
					if (mFlags & 1)
						mTalkMode = TALK_MODE_IDLE;
					else
						mTalkMode = TALK_MODE_READY;
				}
				break;
			}

			case TALK_MODE_ERASING:
				if (mIsBoard ? eraseBoardWindow() : eraseNormalWindow()) {
					if (mIsBoard)
						mTalkMode = TALK_MODE_BOARD_APPEAR;
					else
						mTalkMode = TALK_MODE_OPENING;
				}
				break;

			case TALK_MODE_BOARD_APPEAR:
				if (appearBoardBoxWindow())
					mTalkMode = TALK_MODE_OPEN;
				break;
			}
			break;
		}
	}

	if (cue & CUE_CALC_ANIM) {
		switch (gpMarDirector->unk124) {
		case 2:
			Talk2D2CalcAnim(this);
			break;
		}
	}

	if (cue & CUE_DRAW) {
		switch (gpMarDirector->unk124) {
		case 2: {
			ReInitializeGX();
			SMS_DrawInit();

			J2DOrthoGraph ortho(graphics->getViewport());
			ortho.setup2D();

			if (mNeedsPrepass) {
				mBackPane[0]->show();
				mBackPane[1]->show();
				mBackPane[2]->show();
				J2DPane* root = mScreen->search('ROOT');
				root->setAlpha(0);
				mScreen->draw(0, 0, &ortho);
				root->setAlpha(255);
				ortho.setup2D();
				mNeedsPrepass = false;
				mBackPane[0]->hide();
				mBackPane[1]->hide();
				mBackPane[2]->hide();
			}

			switch (mTalkMode) {
			case TALK_MODE_OPENING:
				for (s8 i = 0; i <= mCurrentLine; i++)
					openWindow(i, mLineProgress[i]);
				// fall through
			case TALK_MODE_OPEN:
			case TALK_MODE_CLOSING:
			case TALK_MODE_ERASING:
				ortho.setup2D();
				if (mIsBoard) {
					mBoardScreen->draw(0, 0, &ortho);
				} else {
					mBasePane->move(mBaseX, mBaseY);
					mBasePane->mRotation = mBaseRotation;
					mScreen->draw(0, 0, &ortho);
				}
				break;
			}
			break;
		}
		}
	}
}

void TTalk2D2::makeLine(f32* out_x, f32* out_y, f32 t, JUTPoint& p0,
                        JUTPoint& p1, JUTPoint& p2)
{
	f32 u = 1.0f - t;
	*out_x = p2.x * (t * t) + (p0.x * (u * u) + p1.x * (2.0f * t * u));
	*out_y = p2.y * (t * t) + (p0.y * (u * u) + p1.y * (2.0f * t * u));
}

void TTalk2D2::setupBoardTextBox(const void* data, JMSMesgEntry* entry)
{
	JSUMemoryInputStream in(
	    (const u8*)data + entry->mTextOffset + mTextOffset, 0x400);
	JSUMemoryOutputStream out(mBoardTextBox->getStringPtr(), 0x200);

	mMesgEntry = entry;

	for (int line = 0; line < 6;) {
		u8 c = in.readU8();

		switch (c) {
		case 0x1a:
			break;

		case '\n':
			out.write((u8)'\n');
			line++;
			break;

		case '\0':
			mIsLastPage = true;
			line        = 6;
			break;

		default: {
			in.skip(-1);
			u8 lead = in.readU8();
			out.write(lead);
			if (lead >= 0x80)
				out.write(in.readU8());
			break;
		}
		}
	}

	if (!mIsLastPage) {
		s8 c;
		in.peek(&c, 1);
		if (c == 0) {
			mIsLastPage = true;
			in.skip(1);
			out.write((u8)'\0');
		}
	} else {
		out.write((u8)'\0');
	}

	mTextOffset += in.getPosition();
}

void TTalk2D2::setupTextBox(const void* data, JMSMesgEntry* entry)
{
	if (mIsBoard) {
		setupBoardTextBox(data, entry);
		return;
	}

	JSUMemoryInputStream in(
	    (const u8*)data + entry->mTextOffset + mTextOffset, 0x400);

	mMesgEntry   = entry;
	int col      = 0;
	int line     = 0;
	mCurrentLine = 0;
	mCharIndex   = 0;

	while (line < LINE_NUM) {
		char* dst = mCharBox[col + line * LINE_LENGTH]->getStringPtr();

		s8 c = in.readS8();

		switch (c) {
		case '\n':
			mLineLength[line] = col == 0 ? 0 : col - 1;
			col               = 0;
			makeBoxLine(line, nullptr);
			line++;
			break;

		case '\0':
			if (col != 0) {
				mLineLength[line] = col - 1;
				col               = 0;
				makeBoxLine(line, nullptr);
			}
			line        = LINE_NUM;
			mIsLastPage = true;
			break;

		case 0x1a:
			setTagParam(in, *mCharBox[col + line * LINE_LENGTH], &col, &line);
			break;

		default: {
			if (mCurrentLine != line)
				mCurrentLine = line;

			in.skip(-1);
			dst[0] = in.readU8();
			if ((u8)dst[0] >= 0x80) {
				dst[1] = in.readU8();
			} else {
				dst[1] = '\0';
			}

			int idx = col + line * LINE_LENGTH;
			mCharBox[idx]->setGradColor(mCharColor, mCharColor);
			mCharBox[idx]->setBlackWhite(mCharColor & 0xffffff00,
			                             mCharColor);
			mCharDelays[mCharIndex] = mCharDelay;
			mCharIndex              = idx;
			col++;
			break;
		}
		}
	}

	if (!mIsLastPage) {
		s8 c;
		in.peek(&c, 1);
		if (c == 0)
			mIsLastPage = true;
	}

	if (mSelectedValue != -1) {
		if (mSelectedValue == 0) {
			snprintf(mSelectTextBox->getStringPtr(), 94,
			         "%s\n\033CC[7f7f7f]\033GC[7f7f7f]%s", mSelectString[0],
			         mSelectString[1]);
		} else {
			snprintf(mSelectTextBox->getStringPtr(), 94,
			         "\033CC[7f7f7f]\033GC[7f7f7f]%s"
			         "\033CC[ffffff]\033GC[ffffff]\n%s",
			         mSelectString[0], mSelectString[1]);
		}
	}

	mTextOffset += in.getPosition();
}

void TTalk2D2::setTagParam(JSUMemoryInputStream& stream, J2DTextBox& box,
                           int* col, int* line)
{
	int size  = stream.readU8();
	int group = stream.readU8();
	u16 tag   = stream.readU16();

	switch (group) {
	case 0:
		switch (tag) {
		case 0:
			mCharDelay = stream.readU8();
			return;
		case 1:
			mForceClose = true;
			return;
		default:
			stream.skip(size - 5);
			return;
		}

	case 1:
		switch (tag) {
		case 0: {
			if (mSelectedValue == -1) {
				mSelectedValue = 0;
				mSelectCursor[1]->setAlpha(254);
				mSelectCursor[1]->hide();
				mSelectCursor[0]->show();
			}
			int len = size - 4 < 17 ? size - 4 : 17;
			snprintf(mSelectString[0], len, "%s",
			         (const char*)stream.getCurrent());
			stream.skip(size - 5);
			return;
		}
		case 1: {
			if (mSelectedValue == -1) {
				mSelectedValue = 1;
				mSelectCursor[0]->setAlpha(254);
				mSelectCursor[0]->hide();
				mSelectCursor[1]->show();
			}
			int len = size - 4 < 17 ? size - 4 : 17;
			snprintf(mSelectString[1], len, "%s",
			         (const char*)stream.getCurrent());
			stream.skip(size - 5);
			return;
		}
		default:
			stream.skip(size - 5);
			return;
		}

	case 2:
		switch (tag) {
		case 0:
		case 1:
		case 6: {
			// A stopwatch: mm:ss:hh, eight boxes wide.
			int time;
			if (tag == 0)
				time = TFlagManager::getInstance()->getFlag(0x20003);
			else if (tag == 1)
				time = TFlagManager::getInstance()->getFlag(0x20002);
			else if (tag == 6)
				time = TFlagManager::getInstance()->getFlag(0x20014);

			if (time > 599999)
				time = 599999;
			if (time < 0)
				time = 0;

			int minutes = (u16)((time - time % 100) / 6000);
			int rest    = time - minutes * 6000;
			int seconds = 0.01 * rest;
			int hundreds = rest - (u16)seconds * 100;

			snprintf(mCharBox[*col + *line * LINE_LENGTH]->getStringPtr(), 2,
			         "%d", minutes / 10);
			snprintf(
			    mCharBox[*col + *line * LINE_LENGTH + 1]->getStringPtr(), 2,
			    "%d", minutes % 10);
			snprintf(
			    mCharBox[*col + *line * LINE_LENGTH + 2]->getStringPtr(), 2,
			    ":");
			snprintf(
			    mCharBox[*col + *line * LINE_LENGTH + 3]->getStringPtr(), 2,
			    "%d", (u16)seconds / 10);
			snprintf(
			    mCharBox[*col + *line * LINE_LENGTH + 4]->getStringPtr(), 2,
			    "%d", (u16)seconds % 10);
			snprintf(
			    mCharBox[*col + *line * LINE_LENGTH + 5]->getStringPtr(), 2,
			    ":");
			snprintf(
			    mCharBox[*col + *line * LINE_LENGTH + 6]->getStringPtr(), 2,
			    "%d", (u16)hundreds / 10);
			snprintf(
			    mCharBox[*col + *line * LINE_LENGTH + 7]->getStringPtr(), 2,
			    "%d", (u16)hundreds % 10);

			for (int i = 0; i < 8; i++) {
				mCharBox[*col + i + *line * LINE_LENGTH]->setGradColor(
				    mCharColor, mCharColor);
				mCharBox[*col + i + *line * LINE_LENGTH]->setBlackWhite(
				    mCharColor & 0xffffff00, mCharColor);
				mCharDelays[i + *col + *line * LINE_LENGTH] = mCharDelay;
			}
			*col += 8;
			return;
		}

		case 2: {
			int coins
			    = 0.01f
			      * (TFlagManager::getInstance()->getFlag(0x20004) + 99);
			if (coins < 10) {
				snprintf(mCharBox[*col + *line * LINE_LENGTH]->getStringPtr(),
				         2, "%d", coins);
				*col += 1;
				return;
			}
			snprintf(mCharBox[*col + *line * LINE_LENGTH]->getStringPtr(), 2,
			         "%d", coins / 10);
			snprintf(
			    mCharBox[*col + *line * LINE_LENGTH + 1]->getStringPtr(), 2,
			    "%d", coins % 10);
			*col += 2;
			return;
		}

		case 3: {
			int value = TFlagManager::getInstance()->getFlag(0x40001);
			int spent = 0;
			for (int i = 0x46; i < 0x56; i++) {
				if (TFlagManager::getInstance()->getFlag(0x10000 + i) != 0)
					spent++;
			}
			for (int i = 0x6c; i <= 0x73; i++) {
				if (TFlagManager::getInstance()->getFlag(0x10000 + i) != 0)
					spent++;
			}
			value -= spent * 10;

			if (value < 100) {
				snprintf(mCharBox[*col + *line * LINE_LENGTH]->getStringPtr(),
				         2, "%d", value / 10);
				*col += 1;
				return;
			}
			snprintf(mCharBox[*col + *line * LINE_LENGTH]->getStringPtr(), 2,
			         "%d", value / 100);
			value -= value / 100 * 100;
			snprintf(
			    mCharBox[*col + *line * LINE_LENGTH + 1]->getStringPtr(), 2,
			    "%d", value / 10);
			*col += 2;
			return;
		}

		case 4: {
			u8 which = stream.readU8();

			TFruitBasketEvent* basket;
			int max;
			int kind;
			switch (which) {
			case 0:
				basket = JDrama::TNameRefGen::search<TFruitBasketEvent>(
				    "フルーツかごＡ");
				max  = 3;
				kind = 0;
				break;
			case 1:
				basket = JDrama::TNameRefGen::search<TFruitBasketEvent>(
				    "フルーツかごＢ");
				max  = 3;
				kind = 4;
				break;
			case 2:
				basket = JDrama::TNameRefGen::search<TFruitBasketEvent>(
				    "フルーツかごＣ");
				max  = 3;
				kind = 3;
				break;
			case 3:
				basket = JDrama::TNameRefGen::search<TFruitBasketEvent>(
				    "フルーツかごＤ");
				kind = 1;
				max  = 3;
				break;
			}

			if (basket != nullptr) {
				max -= basket->getFruitNum(kind);
				if (max < 0 || max > 9)
					max = 0;
				snprintf(mCharBox[*col + *line * LINE_LENGTH]->getStringPtr(),
				         2, "%d", max);
				snprintf(
				    mCharBox[*col + *line * LINE_LENGTH + 1]->getStringPtr(),
				    2, " ");
				*col += 2;
			}
			return;
		}
		}
		break;

	case 0xff:
		switch (tag) {
		case 0:
			mCharColor = cColorTable[stream.readU8()];
			break;
		}
		break;

	default:
		stream.skip(size - 5);
		break;
	}
}

void TTalk2D2::openWindow(s8 line, f32 scroll)
{
	Mtx mtx;
	Mtx rot;

	f32 f31 = mBasePane->getGlobalBounds().x1 + 5;
	f32 f30 = mBasePane->getGlobalBounds().y1 + 5;
	MTXTrans(mtx, -f31, -f30, 0.0f);
	MTXRotRad(rot, 'Z', 0.017453292f * (-mBasePane->getRotation() - 1.0f));
	MTXConcat(rot, mtx, mtx);
	MTXTrans(rot, f31, f30, 0.0f);
	MTXConcat(rot, mtx, mtx);
	GXLoadPosMtxImm(mtx, GX_PNMTX0);

	GXSetCullMode(GX_CULL_BACK);
	GXSetNumTexGens(2);
	GXSetNumTevStages(2);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S8, 0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_TRUE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL,
	              GX_DF_NONE, GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG,
	              GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
	GXSetChanAmbColor(GX_COLOR0A0, (GXColor) { 0xff, 0xff, 0xff, 0xff });

	JUTTexture* tex = ((J2DPicture*)mBackPane[line])->getTexture(0);
	tex->load(GX_TEXMAP1);
	mBackTexture->load(GX_TEXMAP0);

	GXSetTevColor(GX_TEVREG0, JUtility::TColor(0x0000ff00));
	GXSetTevColor(GX_TEVREG1, JUtility::TColor(0x0000ffa0));
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_A1, GX_CA_TEXA, GX_CA_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);

	MTXTrans(mtx, scroll, 0.0f, 0.0f);
	GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0,
	                  GX_FALSE, GX_PTIDENTITY);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);

	GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_ZERO, GX_CC_ZERO,
	                GX_CC_ZERO);
	GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_TEXA,
	                GX_CA_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);

	MTXIdentity(mtx);
	GXLoadTexMtxImm(mtx, GX_TEXMTX1, GX_MTX2x4);
	GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1,
	                  GX_FALSE, GX_PTIDENTITY);
	GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);

	JUTRect bounds(mBackPane[line]->getGlobalBounds());

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition2f32(bounds.x1, bounds.y1);
	GXTexCoord2u8(0, 0);
	GXPosition2f32(bounds.x2, bounds.y1);
	GXTexCoord2u8(1, 0);
	GXPosition2f32(bounds.x2, bounds.y2);
	GXTexCoord2u8(1, 1);
	GXPosition2f32(bounds.x1, bounds.y2);
	GXTexCoord2u8(0, 1);
}

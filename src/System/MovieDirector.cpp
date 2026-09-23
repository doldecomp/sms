#include <System/MovieDirector.hpp>
#include <THPPlayer/THPPlayer.h>
#include <JSystem/JKernel/JKRMemArchive.hpp>
#include <JSystem/JDrama/JDRDStage.hpp>
#include <JSystem/JDrama/JDREfbCtrl.hpp>
#include <JSystem/JDrama/JDRScreen.hpp>
#include <System/Resolution.hpp>
#include <System/Application.hpp>
#include <System/MarioGamePad.hpp>
#include <System/THPRender.hpp>
#include <System/CardManager.hpp>
#include <System/FlagManager.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <GC2D/MovieSubtitle.hpp>
#include <GC2D/MovieRumble.hpp>
#include <GC2D/CardSave.hpp>
#include <GC2D/ScrnFader.hpp>
#include <GC2D/hx_wiper.h>
#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <System/DummyStrings.hpp>

class TEndingString : public JDrama::TViewObj {
public:
	enum {
		ENDING_FADE_NONE     = 0,
		ENDING_FADE_IN       = 1,
		ENDING_FADE_WAIT     = 2,
		ENDING_FADE_OUT      = 3,
	};

	TEndingString(const char* name = "EndingString");

	virtual ~TEndingString() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void startFadeIn();

public:
	/* 0x10 */ u8 mFadeState;
	/* 0x14 */ int mWaitTimer;
	/* 0x18 */ J2DSetScreen* mScreen;
	/* 0x1C */ J2DPane* mRootPane;
};

// TODO: retail numbers this constructor's "ending_1.blo" literal @3092, the
// highest in the TU, i.e. the body was code-generated last. MWCC instead
// generates it on demand at rsetup's `new TEndingString(...)` (the literal
// lands between "EndingString" and "card save"), which shifts every later
// .rodata string by 0x10 and holds rsetup at 87.9%. Moving the definition
// below rsetup only moves the generation to the front of the file. The
// missing lever is whatever made retail's constructor a non-candidate for
// inlining at that call site.
TEndingString::TEndingString(const char* name)
    : JDrama::TViewObj(name)
    , mFadeState(ENDING_FADE_NONE)
    , mWaitTimer(0)
    , mScreen(nullptr)
    , mRootPane(nullptr)
{
	JKRArchive* arc = (JKRArchive*)JKRFileLoader::getVolume("endsave");
	mScreen         = new J2DSetScreen("ending_1.blo", arc);
	mRootPane       = mScreen->search('ROOT');
	mRootPane->setAlpha(0);
}

void TEndingString::startFadeIn()
{
	if (gpApplication.getMovie() == 16) {
		mScreen->search('tx_1')->show();
		mScreen->search('tx_2')->hide();
	} else if (gpApplication.getMovie() == 17) {
		mScreen->search('tx_1')->hide();
		mScreen->search('tx_2')->show();
	}

	mFadeState = ENDING_FADE_IN;
}

static inline u16 MovieDirectorGetPaneAlpha(J2DPane* pane)
{
	return pane->getAlpha();
}

void TEndingString::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		switch (mFadeState) {
		case ENDING_FADE_NONE:
			break;
		case ENDING_FADE_IN: {
			u16 alpha = MovieDirectorGetPaneAlpha(mRootPane);
			alpha += 8;
			if (alpha > 255) {
				mFadeState = ENDING_FADE_WAIT;
				mWaitTimer = 0;
				alpha      = 255;
			}
			mRootPane->setAlpha(alpha);
			break;
		}
		case ENDING_FADE_WAIT:
			mWaitTimer++;
			if (mWaitTimer > 225)
				mFadeState = ENDING_FADE_OUT;
			break;
		case ENDING_FADE_OUT: {
			s16 alpha = mRootPane->getAlpha();
			alpha -= 8;
			if (alpha < 0) {
				mFadeState = ENDING_FADE_NONE;
				alpha      = 0;
			}
			mRootPane->setAlpha(alpha);
			break;
		}
		}
	}

	if (cue & CUE_DRAW) {
		switch (mFadeState) {
		case ENDING_FADE_IN:
		case ENDING_FADE_WAIT:
		case ENDING_FADE_OUT: {
			J2DOrthoGraph ortho(graphics->getViewport());
			ortho.setup2D();
			mScreen->draw(0, 0, &ortho);
			break;
		}
		}
	}
}

const char* TMovieDirector::getStreamMovieName(u32 idx)
{
	if (idx >= 20)
		return nullptr;

	static const char* movies[] = {
		"stolenpeach.thp", "openingA.thp",     "NozuruA.thp",  "openingBA.thp",
		"kagemario.thp",   "stolennozuru.thp", "pakkun.thp",   "MechaKuppa.thp",
		"KuppaJr.thp",     "Entrance.thp",     "demogeso.thp", "bath.thp",
		"autodemoA.thp",   "openingBC.thp",    "epilogue.thp", "staffroll.thp",
		"omakeA.thp",      "omakeB.thp",       "NozuruB.thp",  "openingBB.thp",
	};

	return movies[idx];
}

TMovieDirector::TMovieDirector()
    : unk18(1)
    , unk1C(STATE_FADE_IN)
    , unk20(nullptr)
    , unk24(nullptr)
    , unk30(0)
    , unk34(nullptr)
    , mEndingTimer(0)
{
}

void* TMovieDirector::setupThreadFunc(void* self)
{
	((TMovieDirector*)self)->rsetup();
}

extern OSThread gSetupThread;
extern u8* gpSetupThreadStack;

// TODO: frame 0x40 against retail 0x58, instruction-exact. A name-and-
// return pad binder over param_2 plus an SMSRumbleMgr binder (or a display
// fork) lands the frame and every slot but one: the TDStageGroup ctor's
// TViewObjPtrListT temporary stays 4 low (0x2c against 0x30). Tried (cc41):
// named group, factory fork, thread-address binder, stack forks (change code).
void TMovieDirector::setup(JDrama::TDisplay* param_1, TMarioGamePad* param_2)
{
	unk14         = new JDrama::TDStageGroup(param_1);
	unk20         = param_2;
	unk20->mFlags = 1;

	SMSRumbleMgr->reset();

	OSCreateThread(&gSetupThread, &setupThreadFunc, this,
	               gpSetupThreadStack + 0x10000, 0x10000, 0x11, 0);
	OSResumeThread(&gSetupThread);
}

// TODO: frame 0x310 against retail 0x318; every instruction is right and
// only stack slots differ (8 bytes missing below the THP locals). Naming
// the pos/size temporaries or keeping a TBox2 is worse (98.1-98.6%).
int TMovieDirector::rsetup()
{
	void* subtitleArcBlob
	    = SMSLoadArchive("/data/subtitle.arc", nullptr, 0, nullptr);
	JKRMemArchive* subtitleArc = new JKRMemArchive;
	subtitleArc->mountFixed(subtitleArcBlob, MBF_0);

	switch (gpApplication.getMovie()) {
	case 16:
	case 17:
	case 18:
	case 19: {
		void* arcBlob
		    = SMSLoadArchive("/data/endsave.arc", nullptr, 0, nullptr);
		JKRMemArchive* arc = new JKRMemArchive;
		arc->mountFixed(arcBlob, MBF_0);
		break;
	}
	}

	JDrama::TViewObjPtrListT<JDrama::TViewObj>* rootViewObjs
	    = new JDrama::TViewObjPtrListT<JDrama::TViewObj>("root View Objs");
	unk10 = rootViewObjs;

	JDrama::TViewObjPtrListT<JDrama::TViewObj>* group2d
	    = new JDrama::TViewObjPtrListT<JDrama::TViewObj>("Group 2D");
	rootViewObjs->getChildren().push_back(group2d);

	DVDChangeDir("/data");

	const char* movie = "EX128x144_q0.thp";

	{
		const char* movieName = getStreamMovieName(gpApplication.getMovie());
		if (movieName != nullptr)
			if (DVDConvertPathToEntrynum((char*)movieName) != -1)
				movie = movieName;
	}

	TTHPRender* thpRender = new TTHPRender;
	group2d->getChildren().push_back(thpRender);
	unk28 = new TMovieSubTitle(thpRender);
	unk28->setupResource(movie, subtitleArc);
	group2d->getChildren().push_back(unk28);
	unk2C = new TMovieRumble(thpRender);
	unk2C->init(movie);
	group2d->getChildren().push_back(unk2C);

	switch (gpApplication.getMovie()) {
	case 16:
	case 17:
		unk34 = new TEndingString;
		group2d->getChildren().push_back(unk34);
		// fallthrough
	case 18:
	case 19:
		unk24 = new TCardSave("card save", true);
		unk24->initData(unk20);
		group2d->getChildren().push_back(unk24);
		break;
	}

	JDrama::TDStageDisp* stageDisp = new JDrama::TDStageDisp;
	unk14->getChildren().push_back(stageDisp);

	JDrama::TRect rect(0, 0, SMSGetTitleRenderWidth(),
	                   SMSGetTitleRenderHeight());
	stageDisp->getEfbCtrlDisp()->TEfbCtrl::setSrcRect(rect);

	JDrama::TOrthoProj* camera = new JDrama::TOrthoProj(
	    -1.0f, 1.0f, 0.0f, 0.0f, rect.getWidth(), rect.getHeight());
	group2d->getChildren().push_back(camera);

	JDrama::TScreen* screen = new JDrama::TScreen(rect, "Screen 2D");
	stageDisp->getUnk14()->getChildren().push_back(screen);
	screen->assignCamera(camera);
	screen->assignViewObj(group2d);

	THPPlayerInit(0);
	THPPlayerOpen(movie, 0);
	u32 mem = THPPlayerCalcNeedMemory();
	THPPlayerSetBuffer(new (0x20) u8[mem]);

	int audioTrack = 0;

	THPAudioInfo audioInfo;
	if (THPPlayerGetAudioInfo(&audioInfo) && audioInfo.sndNumTracks >= 2
	    && TFlagManager::getInstance()->getFlag(0xA0000) == 2) {
		audioTrack = 1;
	}

	if (!THPPlayerPrepare(0, 0, audioTrack))
		return 1;

	THPVideoInfo videoInfo;
	THPPlayerGetVideoInfo(&videoInfo);

	thpRender->mPos = JGeometry::TVec2<u32>(
	    (SMSGetGameRenderWidth() - videoInfo.xSize) / 2,
	    (SMSGetGameRenderHeight() - videoInfo.ySize) / 2);
	thpRender->mSize = JGeometry::TVec2<u32>(videoInfo.xSize, videoInfo.ySize);

	DVDChangeDir("/");

	return 0;
}

// Frame levels for the destructor (+0x10 as a pair): a name-and-return
// MSound binder and a direct-return fork over the game pad member.
static inline MSound* MovieGetMSound()
{
	MSound* s = SMSGetMSound();
	return s;
}

static inline TMarioGamePad* MovieGamePad(const TMovieDirector* d)
{
	return d->unk20;
}

TMovieDirector::~TMovieDirector()
{
	if (JKRMemArchive* arc
	    = (JKRMemArchive*)JKRFileLoader::getVolume("endsave"))
		arc->unmountFixed();

	if (JKRMemArchive* arc
	    = (JKRMemArchive*)JKRFileLoader::getVolume("subtitle"))
		arc->unmountFixed();

	MovieGetMSound()->stopAllSound();
	THPPlayerStop();
	THPPlayerClose();
	THPPlayerQuit();
	SMSRumbleMgr->reset();
	MovieGamePad(this)->offFlag(0x1);
}

// Name-and-return flag-manager binder, +8 of frame per site.
static inline TFlagManager* MovieFlags()
{
	TFlagManager* f = TFlagManager::getInstance();
	return f;
}

u32 TMovieDirector::decideNextMode(s32* param_1)
{

	if (gpApplication.getMovie() != 14) {
		if (!(gpApplication.getMovie() == 15 || gpApplication.getMovie() == 16
		      || gpApplication.getMovie() == 17)) {
			int flag = gpApplication.getMovie() + 0x10391;
			TFlagManager::getInstance()->setBool(true, flag);
		}
	}

	u32 nextMode = 1;
	if (MovieGamePad(this)->isSomethingPushed()) {
		nextMode = 4;
	} else if (gpApplication.getMovie() == 2) {
		gpApplication.setMovie(18);
		nextMode = 6;
	} else if (gpApplication.getMovie() == 0x12) {
		*param_1 = 4;
	} else if (gpApplication.getMovie() == 3) {
		gpApplication.setMovie(19);
		nextMode = 6;
	} else if (gpApplication.getMovie() == 0x13) {
		*param_1 = 4;
	} else if (gpApplication.getMovie() == 0xc) {
		TGameSequence& nextArea = gpApplication.mNextArea;
		nextArea.set(15, 0, 0);
		nextMode = 5;
	} else if (gpApplication.getMovie() == 0xe) {
		if (!MovieFlags()->getShineFlag(0x77))
			MovieFlags()->setShineFlag(0x77);

		gpApplication.setMovie(15);
		nextMode = 6;
	} else if (gpApplication.getMovie() == 15) {
		u8 movie
		    = MovieFlags()->getFlag(0x40000) < 120 ? 16 : 17;
		gpApplication.setMovie(movie);

		nextMode = 6;
	} else if (gpApplication.getMovie() == 16
	           || gpApplication.getMovie() == 17) {
		*param_1 = 3;
	} else {
		nextMode = 5;
	}

	return nextMode;
}

// TODO: 99.3%, instruction-exact apart from registers and frame. Retail keeps
// `this` in r31 and every gpApplication base temp in r29 (ours: base r31,
// this r29), and its frame is 0xb0 deeper (0x270): the low region below the
// TColor temps is 0x124 against our 0x78. Graphics/errc/desiredAppState
// declared at the top and switch (nextState) are inert or worse; likely a
// missing inline level, not a per-site lever.
int TMovieDirector::direct()
{
	if (!unk30.check(0x1)) {
		if (!OSIsThreadTerminated(&gSetupThread))
			return 0;

		u32 errc;
		OSJoinThread(&gSetupThread, &errc);
		if (errc)
			return 5;

		unk30.on(0x1);

		gpMSound->initSound();
		if (gpApplication.getMovie() == 16)
			unk34->startFadeIn();

		if (gpApplication.getMovie() == 9) {
			gpApplication.mFader->startWipe(12, 0.0f, 0.0f);
			unk18 = false;
		} else {
			gpApplication.mFader->startWipe(14, 1.0f, 0.0f);
			gpApplication.mFader->setColor(JUtility::TColor(0, 0, 0, 255));
			THPPlayerPlay();
		}
	}

	int desiredAppState = TApplication::APP_STATE_DEFAULT;

	if (!unk18) {
		switch (Hx_MovieStartSyncEx()) {
		case 1:
			SMSGetMSound()->startSoundSystemSE(MSD_SE_TITLE_M, 0, nullptr, 0);
			break;
		case 2:
			unk18 = true;
			THPPlayerPlay();
			break;
		}
	}

	SMSRumbleMgr->update();

	if (unk1C == STATE_SAVE_TO_TITLE || unk1C == STATE_SAVE_AND_CONTINUE) {
		JDrama::TGraphics graphics;
		graphics.unk0 = 1;
		unk10->testPerform(CUE_MOVE, &graphics);
		graphics.unk0 = 0;
		unk10->testPerform(CUE_MOVE, &graphics);
		graphics.unk0 = 0;
		unk10->testPerform(CUE_MOVE, &graphics);
		graphics.unk0 = 0;
		unk10->testPerform(CUE_MOVE | CUE_CALC_ANIM, &graphics);
		unk14->testPerform(CUE_DRAW, &graphics);
	} else {
		JDrama::TDirector::direct();
	}

	s32 nextState = unk1C;
	switch (unk1C) {
	case STATE_FADE_IN:
		if (TFlagManager::getInstance()->getBool(gpApplication.getMovie()
		                                         + 0x10391)
		    && unk20->checkFrameMeaning(0x61)) {
			nextState = STATE_FADE_OUT;
		} else if (gpApplication.mFader->isFullyFadedIn()) {
			nextState = STATE_PLAYING;
		}
		break;

	case STATE_PLAYING:
		if (TFlagManager::getInstance()->getBool(gpApplication.getMovie()
		                                         + 0x10391)
		    && unk20->checkFrameMeaning(0x61)) {
			nextState = STATE_FADE_OUT;
		} else if (THPPlayerGetState() == 5) {
			nextState = STATE_FADE_OUT;
		} else if (THPPlayerGetState() == 3) {
			desiredAppState = decideNextMode(&nextState);
		}

		if (mEndingTimer < 300)
			mEndingTimer++;

		if (gpApplication.getMovie() == 17 && mEndingTimer == 30)
			unk34->startFadeIn();

		if (gpApplication.getMovie() == 17 && mEndingTimer == 220)
			unk34->mFadeState = TEndingString::ENDING_FADE_OUT;
		break;

	case STATE_FADE_OUT:
		if (gpApplication.mFader->isFullyFadedOut())
			desiredAppState = decideNextMode(&nextState);
		break;

	case STATE_SAVE_TO_TITLE:
		switch (unk24->getNextState()) {
		case 0:
		case 1:
			gpApplication.mFader->setFadeStatus(
			    TSMSFader::FADE_STATUS_FULLY_FADED_OUT);
			desiredAppState = TApplication::APP_STATE_DONE;
			break;
		}
		break;

	case STATE_SAVE_AND_CONTINUE:
		switch (unk24->getNextState()) {
		case 1:
			gpApplication.mFader->setFadeStatus(
			    TSMSFader::FADE_STATUS_FULLY_FADED_OUT);
			desiredAppState = TApplication::APP_STATE_MOVIE;
			break;
		case 0:
			gpApplication.mFader->setFadeStatus(
			    TSMSFader::FADE_STATUS_FULLY_FADED_OUT);
			if (gpApplication.getMovie() == 19) {
				gpApplication.setMovie(13);
				desiredAppState = TApplication::APP_STATE_MOVIE;
			} else {
				desiredAppState = TApplication::APP_STATE_GAMEPLAY;
			}
			break;
		}
		break;
	}

	if (unk20->isSomethingPushed()
	    && gpCardManager->getLastStatus() != CARD_RESULT_BUSY
	    && !unk30.check(0x2)) {
		unk30.on(0x2);
		nextState = STATE_FADE_OUT;
		unk28->unkC.on(CUE_DRAW | CUE_MOVE);
		unk2C->unkC.on(CUE_DRAW | CUE_MOVE);
		THPPlayerPause();
	}

	if (nextState != unk1C) {
		switch (nextState) {
		case STATE_SAVE_TO_TITLE:
			THPPlayerStop();
			unk28->unkC.on(CUE_DRAW | CUE_MOVE);
			unk2C->unkC.on(CUE_DRAW | CUE_MOVE);
			gpApplication.mFader->startWipe(14, 0.3f, 0.0f);
			gpApplication.mFader->setColor(JUtility::TColor(0, 0, 0, 255));
			unk24->init(gpApplication.getMovie() == 17 ? 8 : 0);
			break;

		case STATE_SAVE_AND_CONTINUE:
			THPPlayerStop();
			unk28->unkC.on(CUE_DRAW | CUE_MOVE);
			unk2C->unkC.on(CUE_DRAW | CUE_MOVE);
			gpApplication.mFader->startWipe(14, 0.3f, 0.0f);
			gpApplication.mFader->setColor(JUtility::TColor(0, 0, 0, 255));
			unk24->init(9);
			break;

		case STATE_FADE_OUT:
			if (unk20->isSomethingPushed()) {
				gpApplication.mFader->startWipe(4, 1.0f, 0.0f);
			} else {
				gpApplication.mFader->startWipe(15, 1.0f, 0.0f);
				gpApplication.mFader->setColor(JUtility::TColor(0, 0, 0, 255));
			}

			if (gpApplication.getMovie() == 9) {
				MSound* sound = gpMSound;
				sound->fadeOutAllSound(SMSGetVSyncTimesPerSec());
			}
			THPPlayerSetVolume(0, 1000);
			break;
		}
		unk1C = nextState;
	}

	return desiredAppState;
}

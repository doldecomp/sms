#ifndef GC2D_GUIDE_HPP
#define GC2D_GUIDE_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JUtility/JUTRect.hpp>

class J2DPane;
class J2DPicture;
class J2DSetScreen;
class J2DTextBox;
class JKRMemArchive;
class JUTTexture;
class TBoundPane;
class TExPane;
class TMarioGamePad;

/**
 * @brief The pause-menu world map ("guide") screen.
 *
 * @details Shows one summary panel per stage: shines collected, hidden
 * ("etc") shines, coins and blue coins, all read straight out of
 * TFlagManager. The pane names come from guide_1.blo: "00".."12" are the
 * stage hotspots, "pn00".."pn09" the map markers, "NN_0"/"NN_1" the two
 * animated frames of panel NN, and "NN_3"/"NN_5" its two text boxes.
 */
class TGuide : public JDrama::TViewObj {
public:
	/// One stage's counters, refreshed by resetObjects().
	struct TStageScore {
		/* 0x0 */ u8 unk0;
		/* 0x1 */ u8 mShineNum;
		/* 0x2 */ u8 mEtcShineNum;
		/* 0x3 */ u8 unk3;
		/* 0x4 */ u16 mCoinNum;
		/* 0x6 */ u8 mHasFirstEtcShine;
		/* 0x7 */ u8 mBlueCoinNum;
	};

	enum EState {
		STATE_SELECT      = 0,
		STATE_APPEARING   = 1,
		STATE_SHOWING     = 2,
		STATE_DISAPPEAR   = 3,
		STATE_CLOSE       = 7,
		STATE_CLOSED      = 8,
		STATE_INIT        = 8,
		STATE_MOVE_CURSOR = 9,
		STATE_FADE_IN     = 10,
		STATE_FADE_OUT    = 11,
	};

	TGuide(const char* name = "<Guide>");
	virtual ~TGuide() { }
	virtual void load(JSUMemoryInputStream& stream);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void resetObjects();
	void resetScore();
	JKRMemArchive* setup(JKRMemArchive* archive);
	JKRMemArchive* setup2(JKRMemArchive* archive);
	void startMoveCursor();
	void startMoveCursor2();
	void linkSelect();
	void changePattern(J2DPicture* pane, s16 timer, u32 period);
	void mirrorPattern(J2DPicture* pane, s16 timer, u32 period);
	void rotatePattern(J2DPicture* pane, s16 timer, u32 period, s16 angle);
	void shinePattern(TBoundPane* pane, s16 timer, u32 period);
	void mmarkPattern(TExPane* pane, s16 timer, u32 period);
	void searchNearPoint(s16* out_x, s16* out_y, s16 x, s16 y);
	int checkPoint(int x, int y);
	void changeBotStatus(int stage);
	void placeMario();
	void appearGuidePane(int stage);
	void disappearGuidePane(int stage);

public:
	/* 0x010 */ int mState;
	/* 0x014 */ TStageScore mScores[10];
	/* 0x064 */ u8 unk64[0xBC - 0x64];
	/* 0x0BC */ J2DSetScreen* mScreen;
	/* 0x0C0 */ TMarioGamePad* mGamePad;
	/* 0x0C4 */ u8 unkC4;
	/* 0x0C5 */ u8 unkC5;
	/* 0x0C8 */ JUTTexture* mNumberTextures[10];
	/* 0x0F0 */ u16 mTimer;
	/* 0x0F2 */ u8 unkF2[2];
	/* 0x0F4 */ J2DPane* mShineIcon;
	/* 0x0F8 */ J2DPicture* mShineDigits[2];
	/* 0x100 */ J2DPane* mEtcShineIcon;
	/* 0x104 */ J2DPane* mEtcShineMarks[2];
	/* 0x10C */ J2DPicture* mCoinDigits[3];
	/* 0x118 */ J2DPane* mCoinIcon;
	/* 0x11C */ J2DPicture* mBlueCoinDigits[2];
	/* 0x124 */ J2DTextBox* mStageNameBox;
	/* 0x128 */ TExPane* mCursors[2];
	/* 0x130 */ J2DPane* unk130;
	/* 0x134 */ J2DPicture* mSunPane;
	/* 0x138 */ J2DPicture* mShipPane;
	/* 0x13C */ J2DPicture* mPalmPane2;
	/* 0x140 */ J2DPicture* mPalmPane1;
	/* 0x144 */ J2DPicture* mFishPane;
	/* 0x148 */ J2DPicture* mCloudPane;
	/* 0x14C */ J2DPicture* mWavePane1;
	/* 0x150 */ J2DPicture* mWavePane2;
	/* 0x154 */ J2DPicture* mBirdPane1;
	/* 0x158 */ J2DPicture* mBirdPane2;
	/* 0x15C */ bool mMapBlinkUp;
	/* 0x160 */ int mMapAlpha;
	/* 0x164 */ u8 mCursorBlinkUp;
	/// Stage hotspot panes "00".."12", plus the shine pane "20" at 13.
	/* 0x168 */ J2DPane* mStagePanes[22];
	/// Panel frame A, pane "NN_0", plus "lwin" at 13.
	/* 0x1C0 */ TExPane* mPanelsA[22];
	/* 0x218 */ JUTRect mPanelRects[22];
	/// Panel frame B, pane "NN_1", plus "llin" at 13.
	/* 0x378 */ TExPane* mPanelsB[22];
	/// Map marker panes "mi00".."mi09".
	/* 0x3D0 */ J2DPane* mMarkerPanes[10];
	/* 0x3F8 */ u8 unk3F8[0x424 - 0x3F8];
	/* 0x424 */ TExPane* mOpenPanelA;
	/* 0x428 */ TExPane* mOpenPanelB;
	/* 0x42C */ s16 mCurrentStage;
	/* 0x430 */ J2DPane* mMarioMarker;
	/* 0x434 */ JUTRect mMapRect;
	/* 0x444 */ TBoundPane* mShineBoundPane;
	/* 0x448 */ J2DPane* mClickPane;
	/// Map hotspot panes "pn00".."pn09".
	/* 0x44C */ J2DPane* mPointPanes[10];
	/* 0x474 */ void* mStageNameBmg;
	/* 0x478 */ TExPane* mMarkPane;
	/* 0x47C */ u8 mMarkAlpha;
	/* 0x480 */ int mSelectedPoint;
	/* 0x484 */ u8 unk484[0x48C - 0x484];
	/* 0x48C */ JUTRect unk48C;
	/* 0x49C */ u8 unk49C[0x6F8 - 0x49C];
};

#endif

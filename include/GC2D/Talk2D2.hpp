#ifndef GC2D_TALK_2D_2_HPP
#define GC2D_TALK_2D_2_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JUtility/JUTColor.hpp>

class JUTPoint;
class J2DPane;
class J2DSetScreen;
class J2DTextBox;
class JUTTexture;
class TBaseNPC;
class TBoundPane;
class TMarioGamePad;
class TMessageLoader;

class TTalk2D2;

extern TTalk2D2* gpTalk2D;

/**
 * @brief One entry of a JMessage (.bmg) INF1 block.
 *
 * @details TMessageLoader hands these out; only the byte offset of the
 * message text inside the DAT1 block and the voice-clip index are used
 * here.
 *
 * TODO: this is JSystem's own message-entry type and belongs next to the
 * rest of JMessage, not in a GC2D header. TMessageLoader::EntryInfo in
 * GC2D/MessageLoader.hpp is the same struct under a fabricated name, so
 * getMessageEntry() should return a JMSMesgEntry* and this declaration
 * should move out; that is a shared-header change, hence the casts in
 * Talk2D2.cpp.
 */
class JMSMesgEntry {
public:
	/* 0x0 */ u32 mTextOffset;
	/* 0x4 */ s16 unk4;
	/* 0x6 */ s16 unk6;
	/* 0x8 */ u8 mVoiceIndex;
	/* 0x9 */ u8 unk9[3];
};

/**
 * @brief The NPC conversation window ("message_2.blo" / the sign board).
 *
 * @details Draws up to three lines of thirty characters each, laid out
 * along a quadratic Bezier curve so the text follows the balloon's shape,
 * and fades the characters in one at a time. Two screens are used: the
 * normal balloon (message_2.blo) and the flat sign board
 * (message_board_1.blo, used when the actor Mario talks to is a sign).
 *
 * Pane names in message_2.blo: "me_0" is the balloon root, "me_1".."me_3"
 * the three line groups, "bac1".."bac3" their backgrounds, "f_1".."f_9"
 * the three Bezier control points of each line, "cu_1".."cu_3" the
 * "continue" cursors with their "cc_N"/"cs_N" blink frames, "me_4" the
 * two-choice window and "slct" its text box, "sc_1"/"sc_2" the choice
 * cursors.
 */
class TTalk2D2 : public JDrama::TViewObj {
public:
	/// Characters per line, and the number of lines.
	enum {
		LINE_LENGTH = 30,
		LINE_NUM    = 3,
		CHAR_NUM    = LINE_LENGTH * LINE_NUM,
	};

	/// States of mTalkMode, dispatched by perform().
	enum ETalkMode {
		/// Nothing on screen.
		TALK_MODE_IDLE = 0,
		/// A message has been set but the window is still closed.
		TALK_MODE_READY = 1,
		/// Waiting for the camera to reach its talk position.
		TALK_MODE_WAIT_CAMERA = 2,
		/// Counting mWaitTimer down before the window opens.
		TALK_MODE_WAIT_OPEN = 3,
		/// Sliding/fading the window in.
		TALK_MODE_OPENING = 4,
		/// Open; typing characters out and polling the pad.
		TALK_MODE_OPEN = 5,
		/// Closing for good.
		TALK_MODE_CLOSING = 6,
		/// Clearing the text to make room for the next page.
		TALK_MODE_ERASING = 7,
		/// Board window only: fading the new page in.
		TALK_MODE_BOARD_APPEAR = 8,
	};

	/// An NPC whose talk message is forced regardless of the event's ID.
	struct TNpcMessage {
		/* 0x0 */ JDrama::TNameRef* mNpc;
		/* 0x4 */ u32 mMessageID;
	};

	TTalk2D2(const char* name = "<TTalk2D2>");

	virtual ~TTalk2D2() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void setMessageID(u32, u32);
	void forceCloseTalk();
	void closeTalkWindow();
	void openTalkWindow(TBaseNPC*);
	void makeBoxLine(s8, char*);
	bool openBoardWindow();
	bool openNormalWindow();
	void moveBoardWindow();
	void checkBoardControler();
	void moveTalkWindow();
	void checkControler();
	bool closeNormalWindow();
	void closeBoardWindow();
	bool eraseNormalWindow();
	bool eraseBoardWindow();
	void appearBoardBoxWindow();
	void makeLine(f32*, f32*, f32, JUTPoint&, JUTPoint&, JUTPoint&);
	void setupBoardTextBox(const void*, JMSMesgEntry*);
	void setupTextBox(const void*, JMSMesgEntry*);
	void setTagParam(JSUMemoryInputStream&, J2DTextBox&, int*, int*);
	void openWindow(s8, f32);

	static JUtility::TColor cColorTable[6];

	u32 getTalkMode() const { return mTalkMode; }
	s8 getSelectedValue() const { return mSelectedValue; }

public:
	/* 0x010 */ J2DSetScreen* mBoardScreen;
	/* 0x014 */ TBoundPane* mBoardBound;
	/* 0x018 */ J2DTextBox* mBoardTextBox;
	/* 0x01C */ J2DPane* mBoardCursor;
	/* 0x020 */ J2DPane* mBoardCursorOff;
	/* 0x024 */ J2DPane* mBoardCursorOn;
	/* 0x028 */ bool mIsBoard;
	/* 0x029 */ u8 mBoardStep;
	/* 0x02C */ J2DSetScreen* mScreen;
	/* 0x030 */ J2DPane* mLinePane[LINE_NUM];
	/* 0x03C */ J2DPane* mBackPane[LINE_NUM];
	/* 0x048 */ J2DPane* mBezierStart[LINE_NUM];
	/* 0x054 */ J2DPane* mBezierCtrl[LINE_NUM];
	/* 0x060 */ J2DPane* mBezierEnd[LINE_NUM];
	/* 0x06C */ J2DPane* mCursor[LINE_NUM];
	/* 0x078 */ J2DPane* mCursorOff[LINE_NUM];
	/* 0x084 */ J2DPane* mCursorOn[LINE_NUM];
	/* 0x090 */ J2DPane* mBasePane;
	/* 0x094 */ f32 mCharStep;
	/* 0x098 */ u32 unk98;
	/* 0x09C */ J2DTextBox* mCharBox[CHAR_NUM];
	/* 0x204 */ J2DPane* mSelectPane;
	/* 0x208 */ J2DTextBox* mSelectTextBox;
	/* 0x20C */ J2DPane* mSelectCursor[2];
	/* 0x214 */ s8 mSelectedValue; // the line the player selected
	/* 0x218 */ char* mSelectString[2];
	/* 0x220 */ s16 unk220;
	/* 0x222 */ s16 unk222;
	/* 0x224 */ u8 mCharCursor[LINE_NUM];
	/* 0x228 */ int mLineLength[LINE_NUM];
	/* 0x234 */ f32 mLineProgress[LINE_NUM];
	/* 0x240 */ u32 unk240;
	/* 0x244 */ JUTTexture* mBackTexture;
	/* 0x248 */ u32 mTalkMode;
	/* 0x24C */ TMarioGamePad* mGamePad;
	/* 0x250 */ bool mNeedsPrepass;
	/* 0x251 */ s8 mWaitTimer;
	/* 0x252 */ bool mIsTalking;
	/* 0x254 */ JMSMesgEntry* mMesgEntry;
	/* 0x258 */ TMessageLoader* mMapMessage;
	/* 0x25C */ TMessageLoader* mSysMessage;
	/* 0x260 */ TMessageLoader* mCurMessage;
	/* 0x264 */ u32 mMessageID;
	/* 0x268 */ u16 unk268;
	/* 0x26A */ bool mIsLastPage;
	/* 0x26B */ bool mCursorBlinkUp;
	/* 0x26C */ bool mFastForward;
	/* 0x26D */ bool mForceClose;
	/* 0x270 */ u32 mFlags;
	/* 0x274 */ int mCurrentLine;
	/* 0x278 */ int mTextOffset;
	/* 0x27C */ JUtility::TColor mCharColor;
	/* 0x280 */ u8 mCharDelay;
	/* 0x281 */ u8 mCharDelays[CHAR_NUM];
	/* 0x2DC */ s16 mCharTimer;
	/* 0x2DE */ u16 mCharIndex;
	/* 0x2E0 */ TNpcMessage mNpcMessages[10];
	/* 0x330 */ s16 mBaseX;
	/* 0x332 */ s16 mBaseY;
	/* 0x334 */ s16 mBaseRotation;
	/* 0x338 */ f32 mProgressStep;
	/* 0x33C */ f32 mLineDelay;
	/* 0x340 */ s16 mAlphaStep;
};

#endif

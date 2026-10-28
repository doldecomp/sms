#ifndef GC2D_MENU_HPP
#define GC2D_MENU_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/J2D/J2DPane.hpp>

class TMarioGamePad;
class J2DScreen;
class J2DPane;
class J2DTextBox;

class TMenuBase : public JDrama::TViewObj {
public:
	TMenuBase(J2DScreen* screen, const char* name = "<TMenuBase>")
	    : JDrama::TViewObj(name)
	    , mScreen(screen)
	{
	}

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

public:
	/* 0x10 */ J2DScreen* mScreen;
};

class TMenuPlane : public JDrama::TViewObj {
public:
	enum {
		FLAG_CONFIRMED      = 0x1,
		FLAG_CANCELLED      = 0x2,
		FLAG_INPUT_DISABLED = 0x4,
	};

	TMenuPlane(const TMarioGamePad*, J2DPane*, u32, u32);

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void show()
	{
		mColumnStride = 10;
		mRootPane->show();
		unkC.off(CUE_MOVE);
		mFlags = 0;
	}

	void hide()
	{
		mColumnStride = 10;
		mRootPane->hide();
		unkC.on(CUE_MOVE);
		mFlags = 0;
	}

	void fade()
	{
		mFlags |= FLAG_INPUT_DISABLED;
		mRootPane->setAlpha(160);
	}

	void unfade()
	{
		mFlags = 0;
		mRootPane->setAlpha(255);
	}

	bool checkFlag(u32 flag) const { return mFlags & flag ? true : false; }

public:
	/* 0x10 */ const TMarioGamePad* mGamePad;
	/* 0x14 */ J2DPane* mRootPane;
	/* 0x18 */ u32 mFlags;
	/* 0x1C */ JUtility::TColor mSelectedCharColor;
	/* 0x20 */ JUtility::TColor mSelectedGradColor;
	/* 0x24 */ JUtility::TColor mNormalColor;
	/* 0x28 */ s32 mItemCount;
	/* 0x2C */ s32 mSelectedIndex;
	/* 0x30 */ J2DTextBox** mItems;
	/* 0x34 */ u32 mConfirmInputMask;
	/* 0x38 */ u32 mCancelInputMask;
	/* 0x3C */ u8 mColumnStride;
};

class TFlashPane : public JDrama::TViewObj {
public:
	TFlashPane(J2DPane* param_1, const char* name = "<TFlashPane>")
	    : JDrama::TViewObj(name)
	    , mPane(param_1)
	    , mAlpha(0)
	    , mAlphaStep(4)
	{
	}

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void hide()
	{
		mPane->hide();
		unkC.on(CUE_MOVE);
	}

public:
	/* 0x10 */ J2DPane* mPane;
	/* 0x14 */ int mAlpha;
	/* 0x18 */ int mAlphaStep;
};

#endif

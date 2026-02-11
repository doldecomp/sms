#ifndef GC2D_PROG_SELECT_HPP
#define GC2D_PROG_SELECT_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TMarioGamePad;
class J2DTextBox;

// Selector screen for picking interlace/progressive mode.
class TProgSelect : public JDrama::TViewObj {
public:
	TProgSelect(u8, const char* name = "ProgSelect");

	void perform(u32, JDrama::TGraphics*);

	// fabricated
	bool thing()
	{
		unk128 += 1;
		return unk128 / mRefreshRate > 10.0f;
	}

public:
	/* 0x10 */ int mPulsingTimer;
	/* 0x14 */ u8 mSelection;
	/* 0x15 */ bool mIncreasePulsing;
	/* 0x16 */ bool mHideTextBoxes;
	/* 0x18 */ TMarioGamePad* mGamePad;
	/* 0x1C */ char unk1C[256];
	/* 0x11C */ char unk11C[0x4];
	/* 0x120 */ J2DTextBox* unk120[2];
	/* 0x128 */ int unk128;
	/* 0x12C */ f32 mRefreshRate;
};

#endif

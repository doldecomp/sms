#ifndef GC2D_MOVIE_RUMBLE_HPP
#define GC2D_MOVIE_RUMBLE_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <MarioUtil/ToolData.hpp>
#include <System/THPRender.hpp>

class TMovieRumble : public JDrama::TViewObj {
public:
	TMovieRumble(const TTHPRender*);

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void init(const char*);
	void movement();
	void checkRumbleOn();
	void checkRumbleOff();
	void readCurInfo();
	void makeBcrName(char*, int, const char*);

	bool isValid() const
	{
		bool result = false;
		if (getToolData() && unk18 >= 0)
			result = true;
		return result;
	}

	// The binding is load-bearing: it is the `addi r30, r3, 0` that keeps
	// the pointer live in a callee-saved register across readCurInfo's
	// GetValue calls, and it is +4 of low region per expansion. isValid()
	// reads the member through it for the same reason.
	// TODO: fabricated shape -- neither accessor appears anywhere in
	// marioUS.MAP, so only the codegen constrains them.
	Koga::ToolData* getToolData() const
	{
		Koga::ToolData* data = unk14;
		return data;
	}

public:
	/* 0x10 */ const TTHPRender* unk10;
	/* 0x14 */ Koga::ToolData* unk14;
	/* 0x18 */ int unk18;
	/* 0x1C */ s32 unk1C;
	/* 0x20 */ s32 unk20;
	/* 0x24 */ int unk24;
	/* 0x28 */ bool unk28;
};

#endif

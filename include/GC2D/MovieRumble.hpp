#ifndef GC2D_MOVIE_RUMBLE_HPP
#define GC2D_MOVIE_RUMBLE_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <MarioUtil/ToolData.hpp>
#include <System/THPRender.hpp>

class TMovieRumble : public JDrama::TViewObj {
public:
	TMovieRumble(const TTHPRender* thpRenderer);

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void init(const char* movieRumbleFile);
	void movement();
	void checkRumbleOn();
	void checkRumbleOff();
	void readCurInfo();
	void makeBcrName(char* out_bcrName, int, const char* movieRumbleFile);

	bool isValid() const
	{
		bool result = false;
		if (mToolData && mTooldataIndex >= 0)
			result = true;
		return result;
	}

	Koga::ToolData* getToolData() const { return mToolData; }

public:
	/* 0x10 */ const TTHPRender* mThpRenderer;
	/* 0x14 */ Koga::ToolData* mToolData;
	/* 0x18 */ int mTooldataIndex;
	/* 0x1C */ s32 mStartFrame;
	/* 0x20 */ s32 mEndFrame;
	/* 0x24 */ int mRumbleType;
	/* 0x28 */ bool mIsDoingRumble;
};

#endif

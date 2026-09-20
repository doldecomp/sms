#ifndef SYSTEM_MOVIE_DIRECTOR_HPP
#define SYSTEM_MOVIE_DIRECTOR_HPP

#include <JSystem/JDrama/JDRDirector.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <System/Application.hpp>
#ifdef VERSION_GMSP01
#include <JSystem/J2D/J2DScreen.hpp>
#endif

class TCardSave;
class TMovieSubTitle;
class TMovieRumble;

namespace JDrama {
class TDisplay;
};

class TMarioGamePad;

#ifdef VERSION_GMSP01
class TEndingString : public JDrama::TViewObj {
public:
	TEndingString(const char*);

	virtual void perform(u32, JDrama::TGraphics*);

	void startFadeIn();

	void startFadeOut() { mState = STATE_FADE_OUT; }

public:
	enum {
		STATE_HIDDEN   = 0,
		STATE_FADE_IN  = 1,
		STATE_SHOWN    = 2,
		STATE_FADE_OUT = 3,
	};

	/* 0x10 */ u8 mState;
	/* 0x14 */ int mTimer;
	/* 0x18 */ J2DSetScreen* mScreen;
	/* 0x1C */ J2DPane* mRootPane;
};
#endif

class TMovieDirector : public JDrama::TDirector {
public:
	TMovieDirector();

	virtual ~TMovieDirector();
	virtual int direct();

	static void* setupThreadFunc(void*);
	void setup(JDrama::TDisplay*, TMarioGamePad*);
	int rsetup();
	u32 decideNextMode(s32*);

	static const char* getStreamMovieName(u32);

public:
	enum {
		STATE_FADE_IN           = 0,
		STATE_PLAYING           = 1,
		STATE_FADE_OUT          = 2,
		STATE_SAVE_TO_TITLE     = 3,
		STATE_SAVE_AND_CONTINUE = 4
	};

	/* 0x18 */ u8 unk18;
	/* 0x1C */ int unk1C;
	/* 0x20 */ TMarioGamePad* unk20;
	/* 0x24 */ TCardSave* unk24;
	/* 0x28 */ TMovieSubTitle* unk28;
	/* 0x2C */ TMovieRumble* unk2C;
	/* 0x30 */ JDrama::TFlagT<u16> unk30;
#ifdef VERSION_GMSP01
	/* 0x34 */ TEndingString* mEndingString;
	/* 0x38 */ u32 mEndingTimer;
#endif
};

#endif

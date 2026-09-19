#ifndef SYSTEM_MOVIE_DIRECTOR_HPP
#define SYSTEM_MOVIE_DIRECTOR_HPP

#include <JSystem/JDrama/JDRDirector.hpp>

class TCardSave;
class TMovieSubTitle;
class TMovieRumble;
class TEndingString;

namespace JDrama {
class TDisplay;
};

class TMarioGamePad;

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
	/* 0x34 */ TEndingString* unk34;
	// The ROM's `new` size is 0x3c and the constructor zeroes 0x38 next to
	// 0x34. `direct()` counts it up to 300 while a movie plays and keys the
	// ending-string fade on it, so it is an unsigned frame counter.
	/* 0x38 */ u32 mEndingTimer;
};

#endif

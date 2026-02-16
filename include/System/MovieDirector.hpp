#ifndef SYSTEM_MOVIE_DIRECTOR_HPP
#define SYSTEM_MOVIE_DIRECTOR_HPP

#include <JSystem/JDrama/JDRDirector.hpp>

class TCardSave;
class TMovieSubTitle;
class TMovieRumble;

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
	/* 0x18 */ u8 unk18;
	/* 0x1C */ int unk1C;
	/* 0x20 */ TMarioGamePad* unk20;
	/* 0x24 */ TCardSave* unk24;
	/* 0x28 */ TMovieSubTitle* unk28;
	/* 0x2C */ TMovieRumble* unk2C;
	/* 0x30 */ JDrama::TFlagT<u16> unk30;
};

#endif

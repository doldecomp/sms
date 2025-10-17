#ifndef GC2D_MOVIE_RUMBLE_HPP
#define GC2D_MOVIE_RUMBLE_HPP

#include <dolphin/types.h>
#include <System/THPRender.hpp>
#include <MarioUtil/RumbleType.hpp>
#include <MarioUtil/ToolData.hpp>

class TMovieRumble : public JDrama::TViewObj {
public:
	TMovieRumble(const TTHPRender* thpRender);
	virtual ~TMovieRumble();
	void init(const char* subtitleName);
	virtual void perform(u32 flags, JDrama::TGraphics* graphics);
	void checkRumbleOff();

public:
	const TTHPRender* thpRenderer;
	Koga::ToolData* toolData; // this is supposedly the only usage of tooldata
	                          // in the entire game lmao
	s32 entryIndex;
	s32 startFrame;
	s32 endFrame;
	s32 rumbleTypeIndex;
	bool isRumbleActive;
};

#endif // GC2D_MOVIE_RUMBLE_HPP

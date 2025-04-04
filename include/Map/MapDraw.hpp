#ifndef MAP_MAP_DRAW_HPP
#define MAP_MAP_DRAW_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

struct ResTIMG;

// Seems to be the thing that draws the "overlay" on top of the screen whenever
// camera enters a wall
class TMapDrawWall : public JDrama::TViewObj {
public:
	TMapDrawWall(const char*);

	virtual ~TMapDrawWall() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);

public:
	/* 0x10 */ ResTIMG* unk10;
	/* 0x14 */ GXColor unk14;
	/* 0x18 */ int unk18;
};

#endif

#ifndef MAP_SKY_HPP
#define MAP_SKY_HPP

#include <JSystem/JDrama/JDRActor.hpp>

class MActor;

class TSky : public JDrama::TActor {
public:
	TSky(const char*);
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);

public:
	/* 0x44 */ MActor* unk44;
	/* 0x48 */ f32 unk48;
	/* 0x4C */ f32 unk4C;
};

#endif

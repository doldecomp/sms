#ifndef MOVE_BG_MAP_OBJ_WATER_HPP
#define MOVE_BG_MAP_OBJ_WATER_HPP

#include <JSystem/JDrama/JDRActor.hpp>

class MActor;

class TMapObjSeaIndirect : public JDrama::TActor {
public:
	void perform(u32, JDrama::TGraphics*);
	void init();
	TMapObjSeaIndirect(const char*);

public:
	/* 0x44 */ MActor* unk44;
};

class TMapObjWaterFilter : public JDrama::TActor {
public:
	void perform(u32, JDrama::TGraphics*);
	void init();
	TMapObjWaterFilter(const char*);

public:
	/* 0x44 */ MActor* unk44;
};

#endif

#ifndef MOVE_BG_MAP_OBJ_FLOAT_HPP
#define MOVE_BG_MAP_OBJ_FLOAT_HPP

#include <MoveBG/MapObjBlock.hpp>

class TMapObjLibWave;

class TMapObjFloatOnSea : public TLeanBlock {
public:
	virtual void control();
	virtual void initMapObj();
	virtual void calc();
	virtual void touchPlayer(THitActor*);

	TMapObjFloatOnSea(const char*);

public:
	/* 0x194 */ TMapObjLibWave* unk194;
	/* 0x198 */ int unk198;
	/* 0x19C */ int unk19C;
	/* 0x1A0 */ JGeometry::TVec3<f32> unk1A0;
	/* 0x1AC */ f32 unk1AC;
};

#endif

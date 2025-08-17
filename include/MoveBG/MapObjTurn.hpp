#ifndef MOVE_BG_MAP_OBJ_TURN_HPP
#define MOVE_BG_MAP_OBJ_TURN_HPP

#include <MoveBG/MapObjHide.hpp>

class TMapObjTurn : public THideObjBase {
public:
	TMapObjTurn(const char*);

	virtual void loadAfter();
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void control();
	virtual void initMapObj();
	virtual u32 touchWater(THitActor*);
	virtual void turn();

public:
	/* 0x150 */ u32 unk150;
	/* 0x154 */ f32 unk154;
	/* 0x158 */ f32 unk158;
	/* 0x15C */ f32 unk15C;
	/* 0x160 */ f32 unk160;
	/* 0x164 */ f32 unk164;
	/* 0x168 */ u8 unk168;
};

#endif

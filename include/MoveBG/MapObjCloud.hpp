#ifndef MOVE_BG_MAP_OBJ_CLOUD_HPP
#define MOVE_BG_MAP_OBJ_CLOUD_HPP

#include <MoveBG/MapObjRailBlock.hpp>

class TRideCloud : public TRailMapObj {
public:
	TRideCloud(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void calcRootMatrix();
	virtual void setGroundCollision();
	virtual void control();
	virtual u32 getShadowType();
	virtual void initMapObj();
	virtual u32 touchWater(THitActor*);

	static f32 mMoveSpeedDefault;
	static f32 mCushionLen;
	static f32 mCushionSpeed;
	static f32 mScaleSpeed;

public:
	/* 0x150 */ f32 unk150;
	/* 0x154 */ f32 unk154;
	/* 0x158 */ int unk158;
	/* 0x15C */ f32 unk15C;
	/* 0x160 */ f32 unk160;
	/* 0x164 */ f32 unk164;
	/* 0x168 */ f32 unk168;
	/* 0x16C */ u16 unk16C;
	/* 0x16E */ GXColorS10 unk16E;
	/* 0x176 */ GXColorS10 unk176;
};

#endif

#ifndef MOVE_BG_MAP_OBJ_TREE_HPP
#define MOVE_BG_MAP_OBJ_TREE_HPP

#include <MoveBG/MapObjGeneral.hpp>

class TMapObjLeaf {
public:
	TMapObjLeaf();

public:
	/* 0x0 */ f32 unk0;
	/* 0x4 */ f32 unk4;
	/* 0x8 */ TMapCollisionMove* unk8;
	/* 0xC */ JGeometry::SMatrix34C<f32> unkC;
};

class TMapObjTree : public TMapObjGeneral {
public:
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual f32 getRadiusAtY(f32) const;
	virtual void initMapObj();
	virtual void touchPlayer(THitActor*);

	void controlLeaf(int);
	void initEach();
	TMapObjTree(const char* name = "木");

	static f32 mNearMiddle;
	static f32 mMiddleFar;
	static f32 mBananaTreeJumpPower;

public:
	/* 0x148 */ f32 unk148;
	/* 0x14C */ f32 unk14C;
	/* 0x150 */ s32 unk150;
	/* 0x154 */ TMapObjLeaf* unk154;
	/* 0x158 */ bool unk158;
	/* 0x15C */ f32 unk15C;
	/* 0x160 */ f32 unk160;
	/* 0x164 */ f32 unk164;
	/* 0x168 */ f32 unk168;
	/* 0x16C */ u32 unk16C;
};

class TMapObjTreeScale : public TMapObjGeneral {
public:
	virtual void loadAfter();
	virtual void control();
	virtual u32 touchWater(THitActor*);

	void startScaleUp();
	void beSmall();
	TMapObjTreeScale(const char* name = "スケールの木");

	static f32 mScaleSpeedY;
	static f32 mStatusChangeScaleY;
	static f32 mScaleSpeedXZ;
	static f32 mScaleMin;
};

#endif

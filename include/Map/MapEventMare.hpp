#ifndef MAP_MAP_EVENT_MARE_HPP
#define MAP_MAP_EVENT_MARE_HPP

#include <MoveBG/MapObjBase.hpp>

class TMapCollisionBase;

class TMareWallRock : public TLiveActor {
public:
	TMareWallRock()
	    : TLiveActor("マーレ壁の岩")
	{
		unkF4  = 0;
		unkF8  = 0;
		unk100 = 0;
		unk104 = nullptr;
		unk108 = 0;
		unk10C = nullptr;
		unk128 = 0.0f;
		unk110.set(1.0f, 1.0f, 1.0f);
		unk11C.zero();
	}
	TMareWallRock(const char* name);

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);

	void initEffect();
	void movement();
	void appear();
	void depress();
	J3DModel* getMapModel() const;

	static f32 mAppearSpeed;
	static f32 mDepressSpeed;
	static u32 mCleanedDegree;
	static u32 mWaitTimeToAppear;
	static u32 mWaitTimeToDepress;

public:
	/* 0xF4 */ u32 unkF4;
	/* 0xF8 */ u32 unkF8;
	/* 0xFC */ f32 unkFC;
	/* 0x100 */ int unk100;
	/* 0x104 */ TJointObj* unk104;
	/* 0x108 */ u32 unk108;
	/* 0x10C */ TMapCollisionBase** unk10C;
	/* 0x110 */ JGeometry::TVec3<f32> unk110;
	/* 0x11C */ JGeometry::TVec3<f32> unk11C;
	/* 0x128 */ f32 unk128;
};

class TMareEventWallRock : public JDrama::TViewObj {
public:
	TMareEventWallRock(const char* name);

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*) { }

public:
	/* 0x10 */ s32 unk10;
	/* 0x14 */ TMareWallRock* unk14;
};

class TMareEventDepressWall : public JDrama::TViewObj {
public:
	TMareEventDepressWall(const char*);

	virtual void perform(u32, JDrama::TGraphics*);

	void init1stEvent();
	void init2ndEvent();
	void init3rdEvent();
	void initCommon();
	bool startEvent();
	void depressing();
	void emitEffect(int);
	void startToRise();
	void rising();
	void setJointPosX(float, int);
	void finishEvent();

	static u32 mCleanedDegree;
	static f32 mDepressSpeed;
	static f32 mRiseSpeed;
	static u32 mWaitTimeToWatch;

public:
	/* 0x10 */ s32 unk10;
	/* 0x14 */ u32 unk14;
	/* 0x18 */ u32* unk18;
	/* 0x1C */ bool* unk1C;
	/* 0x20 */ f32* unk20;
	/* 0x24 */ TMapCollisionWarp* unk24;
	/* 0x28 */ TMapCollisionMove* unk28;
	/* 0x2C */ char unk2C[4];
	/* 0x30 */ J3DJoint** unk30;
	/* 0x34 */ JGeometry::TVec3<f32>* unk34;
	/* 0x38 */ JGeometry::TVec3<f32>* unk38;
	/* 0x3C */ f32* unk3C;
	/* 0x40 */ f32* unk40;
	/* 0x44 */ s32 unk44;
	/* 0x48 */ int unk48;
	/* 0x4C */ int unk4C;
};

class TMareEventBumpyWall : public TMapObjBase {
public:
	TMareEventBumpyWall(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void control();
	virtual u32 touchWater(THitActor*);

	void bumpUpX();
	void bumpDownX();
	void bumpUpZ();
	void bumpDownZ();

public:
	/* 0x138 */ s32 unk138;
	/* 0x13C */ J3DJoint* unk13C;
	/* 0x140 */ f32 unk140;
	/* 0x144 */ f32 unk144;
	/* 0x148 */ TMapCollisionWarp* unk148;
	/* 0x14C */ TMapCollisionMove* unk14C;
	/* 0x150 */ int unk150;
};

#endif

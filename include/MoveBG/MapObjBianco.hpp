#ifndef MOVEBG_MAP_OBJ_BIANCO_HPP
#define MOVEBG_MAP_OBJ_BIANCO_HPP

#include <JSystem/JDrama/JDRGraphics.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjFloat.hpp>
#include <MoveBG/MapObjHide.hpp>
#include <MoveBG/MapObjTurn.hpp>
#include <dolphin/gx.h>
#include <dolphin/types.h>

class JSUMemoryInputStream;
class TBGCheckData;
struct TBGWallCheckRecord;
class THitActor;
class TTrembleModelEffect;
class JAISound;

class TWoodLog : public TMapObjFloatOnSea {
public:
	TWoodLog(const char* name = "丸太")
	    : TMapObjFloatOnSea(name)
	{
	}

	virtual void control();
};

class TBiancoBell : public TMapObjBase {
public:
	TBiancoBell(const char* name = "ベル水車");

	virtual void initMapObj();
	virtual void touchPlayer(THitActor*);
	virtual u32 touchWater(THitActor*);

	void ringSingle();
	void ring();
	void stopToRing();

public:
	/* 0x138 */ u16 unk138;
	/* 0x13A */ u8 unk13A;
};

class TBellWatermill : public TMapObjTurn {
public:
	TBellWatermill(const char* name = "ベル水車");

	virtual void loadAfter();
	virtual void control();
	virtual u32 touchWater(THitActor*);

public:
	/* 0x16C */ f32 unk16C;
	/* 0x170 */ f32 unk170;
	/* 0x174 */ f32 unk174;
	/* 0x178 */ f32 unk178;
	/* 0x17C */ f32 unk17C;
	/* 0x180 */ f32 unk180;
	/* 0x184 */ f32 unk184;
	/* 0x188 */ f32 unk188;
	/* 0x18C */ f32 unk18C;
	/* 0x190 */ u8 unk190;
	/* 0x194 */ TBiancoBell* unk194;
	/* 0x198 */ TBiancoBell* unk198;
	/* 0x19C */ TBiancoBell* unk19C;
	/* 0x1A0 */ u8 unk1A0;
	/* 0x1A4 */ JAISound* unk1A4;
};

class TLampSeesaw : public TMapObjBase {
public:
	TLampSeesaw(const char* name = "ランプシーソー（従）");

	virtual void load(JSUMemoryInputStream&);
	virtual void touchPlayer(THitActor*);
	virtual void pushDown(f32) { }

public:
	/* 0x138 */ TLampSeesaw* unk138;
	/* 0x13C */ f32 unk13C;
	/* 0x140 */ f32 unk140;
};

class TLampSeesawMain : public TLampSeesaw {
public:
	TLampSeesawMain(const char* name = "ランプシーソー");

	virtual void loadAfter();
	virtual void control();
	virtual void touchPlayer(THitActor*);
	virtual void pushDown(f32);

	void move();

	enum {
		STATE_UNK2 = 0x2,
		STATE_UNK3 = 0x3,
	};

public:
	/* 0x144 */ f32 unk144;
	/* 0x148 */ f32 unk148;
	/* 0x14C */ f32 unk14C;
	/* 0x150 */ f32 unk150;
};

class TLeafBoat : public TMapObjBase {
public:
	TLeafBoat(const char* name = "リーフボート");

	virtual void control();
	virtual void bind();
	virtual void initMapObj();
	virtual void calc();
	virtual void touchActor(THitActor*);

	void touchWall(JGeometry::TVec3<f32>*, TBGWallCheckRecord*);

public:
	/* 0x138 */ f32 unk138;
	/* 0x13C */ f32 unk13C;
	/* 0x140 */ f32 unk140;
	/* 0x144 */ f32 unk144;
	/* 0x148 */ f32 unk148;
	/* 0x14C */ f32 unk14C;
	/* 0x150 */ f32 unk150;
	/* 0x154 */ f32 unk154;
	/* 0x158 */ f32 unk158;
	/* 0x15C */ f32 unk15C;
	/* 0x160 */ s32 unk160;
	/* 0x164 */ JGeometry::TVec3<f32> unk164;
};

class TLeafBoatRotten : public TLeafBoat {
public:
	TLeafBoatRotten(const char* name = "腐ったリーフボート");

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void control();

	static f32 mAlphaDownSpeed;
	static f32 mCollisionRemoveAlpha;
	static GXColorS10 mRottenColor;

	enum {
		STATE_UNK2 = 0x2,
		STATE_UNK3 = 0x3,
	};

public:
	/* 0x170 */ s32 unk170;
	/* 0x174 */ f32 unk174;
	/* 0x178 */ GXColorS10 unk178;
};

class TBiancoMiniWindmill : public THideObjBase {
public:
	TBiancoMiniWindmill(const char* name = "風車（ビアンコ小）");

	virtual void control();
	virtual void initMapObj();
	virtual void calc();
	virtual u32 touchWater(THitActor*);

	static f32 mRotWaterAccel;
	static f32 mFriction;
	static f32 mRotSpeedMax;

public:
	/* 0x150 */ f32 unk150;
	/* 0x154 */ f32 unk154;
	/* 0x158 */ f32 unk158;
	/* 0x15C */ THitActor* unk15C;
	/* 0x160 */ JAISound* unk160;
};

class TBiancoWatermillVertical : public TMapObjBase {
public:
	TBiancoWatermillVertical(const char* name = "水車（ビアンコ垂直）");

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void setGroundCollision();
	virtual void control();
	virtual u32 touchWater(THitActor*);

	static f32 mRotAccel;
	static f32 mRotSpeedDownRate;
	static f32 mRotSpeedMax;
	static f32 mBridgeRotRate;

public:
	/* 0x138 */ f32 unk138;
	/* 0x13C */ f32 unk13C;
	/* 0x140 */ TMapObjBase* unk140;
	/* 0x144 */ u8 unk144;
	/* 0x148 */ JAISound* unk148[2];
};

class TBiancoWatermill : public TMapObjBase {
public:
	TBiancoWatermill(const char* name = "水車（ビアンコ大）");

	virtual void control();
	virtual void initMapObj();
	virtual u32 touchWater(THitActor*);

	void turn(const JGeometry::TVec3<f32>&, const TBGCheckData*, f32);
	void turnByEnemy(THitActor*, const TBGCheckData*);

public:
	/* 0x138 */ f32 unk138;
	/* 0x13C */ JAISound* unk13C;
};

class TMapObjRootPakkun : public TMapObjBase {
public:
	TMapObjRootPakkun(const char* name = "ボスパックンの根")
	    : TMapObjBase(name)
	    , unk138(nullptr)
	{
	}

	virtual void drawObject(JDrama::TGraphics*);
	virtual void initMapObj();

	static f32 mTremblePower;
	static f32 mTrembleAccel;
	static f32 mTrembleBrake;
	static int mTrembleTime;

public:
	/* 0x138 */ TTrembleModelEffect* unk138;
};

class TBigWindmill : public TMapObjBase {
public:
	TBigWindmill(const char* name = "巨大風車")
	    : TMapObjBase(name)
	    , unk148(nullptr)
	{
	}

	virtual void load(JSUMemoryInputStream&);
	virtual void control();

public:
	/* 0x138 */ TMapObjBase* unk138[4];
	/* 0x148 */ JAISound* unk148;
};

#endif

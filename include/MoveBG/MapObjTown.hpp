#ifndef MOVE_BG_MAP_OBJ_TOWN_HPP
#define MOVE_BG_MAP_OBJ_TOWN_HPP

#include "Strategic/HitActor.hpp"
#include <JSystem/JDrama/JDRActor.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjGeneral.hpp>
#include <MoveBG/MapObjHide.hpp>

class TMapCollisionWarp;
class JAISound;

class TDoor : public TMapObjBase {
public:
	TDoor(const char* name = "ドア");

	virtual void load(JSUMemoryInputStream&);
	virtual void touchPlayer(THitActor*);

public:
	/* 0x138 */ bool unk138;
};

class TManhole : public TMapObjGeneral {
public:
	TManhole(const char* name = "マンホール");

	virtual void loadAfter();
	virtual void initMapObj();
	virtual void calc();
	virtual void touchPlayer(THitActor*);
	virtual void appeared();

	bool animationFinished();
	void setGroundCollision();
	void makeManholeUnuseful(const TMapObjBase*);

	static f32 mDownHeight;
	static f32 mDownSpeed;
	static f32 mVibrationSpeed;
	static f32 mVibrationEndHeight;
	static f32 mVibrationDecreaseRate;

public:
	/* 0x148 */ f32 unk148;
	/* 0x14C */ f32 unk14C;
	/* 0x150 */ u8 unk150;
	/* 0x151 */ u8 unk151;
	/* 0x152 */ u8 unk152;
	/* 0x154 */ const TMapObjBase* unk154;
	/* 0x158 */ TMapCollisionWarp* unk158;
};

class TMapObjBillboard : public THideObjBase {
public:
	TMapObjBillboard(const char* name = "看板")
	    : THideObjBase(name)
	{
	}

	virtual void touchActor(THitActor*);
	virtual u32 touchWater(THitActor*);

	void swing(THitActor*);

public:
	/* 0x150 */ JAISound* unk150;
};

class TMapObjChangeStage : public TMapObjBase {
public:
	TMapObjChangeStage(const char* name = "ステージ切り替え")
	    : TMapObjBase(name)
	    , unk138(0)
	{
	}

	virtual void load(JSUMemoryInputStream&);
	virtual void touchPlayer(THitActor*);

public:
	/* 0x138 */ u16 unk138;
};

class TMapObjChangeStageHipDrop : public TMapObjChangeStage {
public:
	TMapObjChangeStageHipDrop(const char* name
	                          = "ステージ切り替え（ヒップドロップ）")
	    : TMapObjChangeStage(name)
	{
	}

	virtual void initMapObj();
	virtual void touchPlayer(THitActor*);
};

class TMapObjStartDemo : public TMapObjBase {
public:
	TMapObjStartDemo(const char* name = "デモ開始オブジェ")
	    : TMapObjBase(name)
	{
	}

	virtual void load(JSUMemoryInputStream&);
	virtual void touchPlayer(THitActor*);

public:
	/* 0x138 */ u32 unk138;
};

class TDamageObj : public THitActor {
public:
	TDamageObj(const char* name = "ダメージオブジェ")
	    : THitActor(name)
	{
	}

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);

	void init(u32);
};

class TShadowObj {
public:
	void load(JSUMemoryInputStream&);
};

class TMapObjWaterSpray : public TMapObjBase {
public:
	TMapObjWaterSpray(const char* name = "汎用水しぶき");

	virtual void load(JSUMemoryInputStream&);
	virtual void calc();

public:
	/* 0x138 */ u32 unk138;
	/* 0x13C */ f32 unk13C;
	/* 0x140 */ JGeometry::TVec3<f32> unk140;
	/* 0x14C */ JUtility::TColor unk14C;
};

class THideObjInfo : public JDrama::TActor {
public:
	THideObjInfo(const char* name = "オブジェ出現情報");

	virtual void load(JSUMemoryInputStream&);
	virtual void action(long);

public:
	/* 0x44 */ u32 unk44;
	/* 0x48 */ f32 unk48;
	/* 0x4C */ f32 unk4C;
};

class TMapObjSwitch : public TMapObjBase {
public:
	TMapObjSwitch(const char* name = "オブジェスイッチ");

	virtual void load(JSUMemoryInputStream&);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void control();

	void registerObjInfo(THideObjInfo*);

public:
	/* 0x138 */ s32 unk138;
	/* 0x13C */ s32 unk13C;
	/* 0x140 */ s32 unk140;
	/* 0x144 */ THideObjInfo** unk144;
	/* 0x148 */ GXColorS10 unk148;
};

extern TMapObjSwitch* gpMapObjSwitch;

class TRedCoinSwitch : public TMapObjBase {
public:
	TRedCoinSwitch(const char* name = "赤コインスイッチ");

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void control();

public:
	/* 0x138 */ s32 unk138;
	/* 0x13C */ GXColorS10 unk13C;
};

class TBasketReverse : public TMapObjBase {
public:
	TBasketReverse(const char* name = "さかさバスケット");

	virtual void kill();
	virtual void initMapObj();
};

#endif

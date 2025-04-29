#ifndef MOVE_BG_MAP_OBJ_TOWN_HPP
#define MOVE_BG_MAP_OBJ_TOWN_HPP

#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjGeneral.hpp>
#include <MoveBG/MapObjHide.hpp>

// TODO: mark virtual methods as such

class TDoor : public TMapObjBase {
public:
	void touchPlayer(THitActor*);
	void load(JSUMemoryInputStream&);
	TDoor(const char*);
};

class TManhole : public TMapObjGeneral {
public:
	void touchPlayer(THitActor*);
	void animationFinished();
	void appeared();
	void calc();
	void setGroundCollision();
	void makeManholeUnuseful(const TMapObjBase*);
	void loadAfter();
	void initMapObj();
	TManhole(const char*);
};

class TMapObjBillboard : public THideObjBase {
public:
	void swing(THitActor*);
	void touchActor(THitActor*);
	u32 touchWater(THitActor*);
	TMapObjBillboard();
};

class TMapObjChangeStage : public TMapObjBase {
public:
	void touchPlayer(THitActor*);
	void load(JSUMemoryInputStream&);
	TMapObjChangeStage(const char* name = "ステージ切り替え")
	    : TMapObjBase(name)
	    , unk138(0)
	{
	}

public:
	/* 0x138 */ u16 unk138;
};

class TMapObjChangeStageHipDrop : public TMapObjChangeStage {
public:
	void touchPlayer(THitActor*);
	void initMapObj();
};

class TMapObjStartDemo : public TMapObjBase {
public:
	void touchPlayer(THitActor*);
	void load(JSUMemoryInputStream&);
	TMapObjStartDemo();
};

class TDamageObj : public THitActor {
public:
	void perform(unsigned long, JDrama::TGraphics*);
	void init(unsigned long);
	void load(JSUMemoryInputStream&);
	TDamageObj();
};

class TShadowObj {
public:
	void load(JSUMemoryInputStream&);
};

class TMapObjWaterSpray : public TMapObjBase {
public:
	void calc();
	void load(JSUMemoryInputStream&);
	TMapObjWaterSpray(const char*);
};

class THideObjInfo : public JDrama::TViewObj {
public:
	void action(long);
	void load(JSUMemoryInputStream&);
	THideObjInfo(const char*);
};

class TMapObjSwitch : public TMapObjBase {
public:
	void control();
	u32 receiveMessage(THitActor*, unsigned long);
	void registerObjInfo(THideObjInfo*);
	void load(JSUMemoryInputStream&);
	TMapObjSwitch(const char*);
};

class TRedCoinSwitch : public TMapObjBase {
public:
	u32 receiveMessage(THitActor*, unsigned long);
	void control();
	void loadAfter();
	void load(JSUMemoryInputStream&);
	TRedCoinSwitch(const char*);
};

class TBasketReverse : public TMapObjBase {
public:
	void kill();
	void initMapObj();
	TBasketReverse();
};

#endif

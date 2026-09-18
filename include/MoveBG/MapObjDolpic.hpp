#ifndef MOVE_BG_MAP_OBJ_DOLPIC_HPP
#define MOVE_BG_MAP_OBJ_DOLPIC_HPP

#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjHide.hpp>
#include <MoveBG/MapObjTurn.hpp>

class TSharedParts;

class TMonumentShine : public TMapObjBase {
public:
	TMonumentShine(const char* name = "モニュメントシャイン");

	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void control();
	virtual void initMapObj();

	void hitByWater(THitActor*);

public:
	/* 0x138 */ GXColor unk138;
	/* 0x13C */ int unk13C;
	/* 0x140 */ f32 unk140;
	/* 0x144 */ int unk144;
	/* 0x148 */ s8 unk148;
	/* 0x149 */ s8 unk149;
};

class TBellDolpic : public TMapObjBase {
public:
	TBellDolpic(int, const char* name = "ドルピック鐘");

	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void calcRootMatrix();
	virtual void control();
	virtual void initMapObj();
	virtual void touchPlayer(THitActor*);

	void ring(const JGeometry::TVec3<f32>&);

public:
	/* 0x138 */ GXColor unk138;
	/* 0x13C */ int unk13C;
	/* 0x140 */ JGeometry::TVec3<f32> unk140;
	/* 0x14C */ f32 unk14C;
	/* 0x150 */ f32 unk150;
	/* 0x154 */ int unk154;
	/* 0x158 */ int unk158;
	/* 0x15C */ s8 unk15C;
};

class TDptMonteFence : public TMapObjBase {
public:
	TDptMonteFence(const char* name = "投げモンテフェンス")
	    : TMapObjBase(name)
	{
	}

	virtual void touchPlayer(THitActor*);
};

class TMapObjSmoke : public THideObjBase {
public:
	TMapObjSmoke(const char* name = "黒煙")
	    : THideObjBase(name)
	{
	}

	virtual void load(JSUMemoryInputStream&);
	virtual void control();
	virtual u32 touchWater(THitActor*);
};

class TMareGate : public TMapObjBase {
public:
	TMareGate(const char* name = "マーレゲート")
	    : TMapObjBase(name)
	{
	}

	virtual void loadAfter();
	virtual void control();
};

// Dead in the shipped game: the "DolWeathercock" factory entry builds a plain
// TMapObjTurn, so every TWeathercock symbol is UNUSED in the map. The base is
// TMapObjTurn on two pieces of evidence: __vt__12TWeathercock is 0x174, the
// exact size of __vt__11TMapObjTurn (so the class adds no virtual of its own,
// which is also why the map lists no other TWeathercock method), and
// __dt__12TWeathercockFv is 0x9c, the size of a dtor below the direct
// TMapObjBase children.
// TODO: control() is 0x20, i.e. a prologue, one non-virtual call and an
// epilogue. TMapObjTurn::control() is the parsimonious reading; skipping a
// level with TMapObjBase::control() (a weathercock turns with the wind, not
// with water) compiles to the same eight instructions, so the map cannot tell
// them apart.
// No constructor is declared: the map has no __ct__12TWeathercock, and a
// fabricated default name would add a string literal to the pool.
class TWeathercock : public TMapObjTurn {
public:
	virtual void control();
};

class TDemoCannon : public TMapObjBase {
public:
	TDemoCannon(const char* name = "デモ砲台")
	    : TMapObjBase(name)
	    , unk138(nullptr)
	    , unk13C(nullptr)
	    , unk14C(0)
	{
	}

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void loadAfter();
	virtual void initMapObj();

	void startDemo();

public:
	/* 0x138 */ TSharedParts* unk138;
	/* 0x13C */ TSharedParts* unk13C;
	/* 0x140 */ u8 pad140[0xC];
	/* 0x14C */ u8 unk14C;
};

class TTurboNozzleDoor : public TMapObjBase {
public:
	TTurboNozzleDoor(const char* name = "ターボノズルドア")
	    : TMapObjBase(name)
	    , unk144(nullptr)
	{
		unk138.x = unk138.y = unk138.z = 0.0f;
	}

	virtual void loadAfter();
	virtual void touchPlayer(THitActor*);

public:
	/* 0x138 */ JGeometry::TVec3<f32> unk138;
	/* 0x144 */ TMapObjBase* unk144;
};

#endif

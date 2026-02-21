#ifndef MOVE_BG_MAP_OBJ_DOLPIC_HPP
#define MOVE_BG_MAP_OBJ_DOLPIC_HPP

#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjHide.hpp>

class TSharedParts;

class TMonumentShine : public TMapObjBase {
public:
	TMonumentShine(const char*);

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
	TBellDolpic(int, const char*);

	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void calcRootMatrix();
	virtual void control();
	virtual void initMapObj();
	virtual void touchPlayer(THitActor*);

	void ring(const JGeometry::TVec3<f32>&);

public:
	/* 0x138 */ GXColor unk138;
	/* 0x13C */ int unk13C;
	/* 0x140 */ f32 unk140;
	/* 0x144 */ f32 unk144;
	/* 0x148 */ f32 unk148;
	/* 0x14C */ f32 unk14C;
	/* 0x150 */ f32 unk150;
	/* 0x154 */ int unk154;
	/* 0x158 */ int unk158;
	/* 0x15C */ s8 unk15C;
};

class TDptMonteFence : public TMapObjBase {
public:
	TDptMonteFence(const char* name)
	    : TMapObjBase(name)
	{
	}

	virtual void touchPlayer(THitActor*);
};

class TMapObjSmoke : public THideObjBase {
public:
	TMapObjSmoke(const char* name)
	    : THideObjBase(name)
	{
	}

	virtual void load(JSUMemoryInputStream&);
	virtual void control();
	virtual u32 touchWater(THitActor*);
};

class TMareGate : public TMapObjBase {
public:
	TMareGate(const char* name)
	    : TMapObjBase(name)
	{
	}

	virtual void loadAfter();
	virtual void control();
};

class TDemoCannon : public TMapObjBase {
public:
	TDemoCannon(const char* name)
	    : TMapObjBase(name)
	    , unk138(nullptr)
	    , unk13C(nullptr)
	    , unk14C(0)
	{
	}

	virtual void perform(u32, JDrama::TGraphics*);
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
	TTurboNozzleDoor(const char* name)
	    : TMapObjBase(name)
	    , unk138()
	    , unk144(nullptr)
	{
	}

	virtual void loadAfter();
	virtual void touchPlayer(THitActor*);

public:
	/* 0x138 */ JGeometry::TVec3<f32> unk138;
	/* 0x144 */ TLiveActor* unk144;
};

#endif

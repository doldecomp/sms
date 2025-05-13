#ifndef MOVE_BG_MAP_OBJ_TRAP_HPP
#define MOVE_BG_MAP_OBJ_TRAP_HPP

#include "MoveBG/MapObjBase.hpp"

class TLampTrapSpike;

class TLampTrapSpikeHit : public THitActor {
public:
	TLampTrapSpikeHit(TLampTrapSpike*, const char*);

	virtual void perform(u32, JDrama::TGraphics*);
	virtual u32 receiveMessage(THitActor*, u32);

public:
	/* 0x68 */ TLampTrapSpike* unk68;
};

class TLampTrapSpike : public TMapObjBase {
public:
	TLampTrapSpike(const char*);

	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual u32 receiveMessage(THitActor*, u32);
	virtual void control();
	virtual void initMapObj();

public:
	/* 0x138 */ int unk138;
	/* 0x13C */ int unk13C;
	/* 0x140 */ TLampTrapSpikeHit* unk140;
};

class TLampTrapIron;

class TLampTrapIronHit : public THitActor {
public:
	TLampTrapIronHit(TLampTrapIron*, const char*);

	virtual void perform(u32, JDrama::TGraphics*);
	virtual u32 receiveMessage(THitActor*, u32);

public:
	/* 0x68 */ TLampTrapIron* unk68;
};

class TLampTrapIron : public TMapObjBase {
public:
	TLampTrapIron(const char*);

	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual u32 receiveMessage(THitActor*, u32);
	virtual void control();
	virtual void initMapObj();

	static u32 mHitPointMax;
	static u32 mFireTimerMax;

public:
	/* 0x138 */ TLampTrapIronHit* unk138;
	/* 0x13C */ int unk13C;
	/* 0x140 */ int unk140;
};

#endif

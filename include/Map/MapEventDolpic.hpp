#ifndef MAP_MAP_EVENT_DOLPIC_HPP
#define MAP_MAP_EVENT_DOLPIC_HPP

#include <Map/MapEvent.hpp>

class TMapObjBase;
class TMapCollisionMove;
class TMapCollisionWarp;

class TDolpicEventBiancoGate : public TMapEvent {
public:
	TDolpicEventBiancoGate(const char*);

	virtual void loadAfter();
	virtual bool isFinishedAll() const;
	virtual bool watch();
	virtual bool control();

public:
	/* 0x20 */ TMapObjBase* unk20;
	/* 0x24 */ f32 unk24;
};

class TDolpicEventRiccoMammaGate : public TMapEvent {
public:
	TDolpicEventRiccoMammaGate(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual bool isFinishedAll() const;
	virtual bool watch();
	virtual bool control();

	void rising();

	// fabricated
	int getEventFlag() const { return unk2C; }
	// The extra level in watch()'s SMS_ShowJoint argument is the 4 bytes of
	// low region that put every one of that frame's slots at retail's offset.
	J3DJoint* getJoint() { return unk20; }

public:
	/* 0x20 */ J3DJoint* unk20;
	/* 0x24 */ TMapCollisionMove* unk24;
	/* 0x28 */ TMapCollisionWarp* unk28;
	/* 0x2C */ int unk2C;
	/* 0x30 */ bool unk30;
	/* 0x34 */ f32 unk34;
	/* 0x38 */ int unk38;
	/* 0x3C */ int unk3C;
	/* 0x40 */ int unk40;
	/* 0x44 */ int unk44;
	/* 0x48 */ JGeometry::TVec3<f32> unk48;
	/* 0x54 */ JGeometry::TVec3<f32> unk54;
	/* 0x60 */ f32 unk60;
};

#endif

#ifndef MOVE_BG_MAP_OBJ_AIRPORT_HPP
#define MOVE_BG_MAP_OBJ_AIRPORT_HPP

#include <MoveBG/MapObjGeneral.hpp>
#include <MoveBG/Pool.hpp>
#include <Map/MapEventSink.hpp>

class TAirportPool : public TPool {
public:
	virtual void appear();

	TAirportPool(const char*);
};

class TAirportSwitch : public TMapObjGeneral {
public:
	virtual void loadAfter();
	virtual void breaking();

	TAirportSwitch(const char*);

public:
	/* 0x148 */ TAirportPool* unk148;
};

class TAirportEventSink : public TMapEventSinkInPollutionReset {
public:
	virtual void loadAfter();
	virtual bool watch();
	virtual bool control();
	virtual void finishControl();

	TAirportEventSink(const char*);

public:
	/* 0x64 */ u32 unk64;
	/* 0x68 */ u32 unk68;
	/* 0x6C */ TLiveActor* unk6C; // TODO: wrong type
};

#endif

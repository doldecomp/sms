#ifndef MAP_MAP_EVENT_SINK_HPP
#define MAP_MAP_EVENT_SINK_HPP

#include <Map/MapEvent.hpp>

class TMapCollisionWarp;
class TMapCollisionMove;
class TMapObjBase;
class TPollutionObj;

class TMapEventSink : public TMapEvent {
public:
	virtual void load(JSUMemoryInputStream&);
	virtual bool isFinishedAll() const;
	virtual bool watch() { return false; }
	virtual void startControl();
	virtual bool control();
	virtual void finishControl();
	virtual TJointObj* getBuilding(int) const;
	virtual void initBuilding(int, JSUMemoryInputStream&);
	virtual void initWithBuildingNum(JSUMemoryInputStream&);
	virtual void rising();
	virtual f32 getSinkOffsetY() const;
	virtual void makeBuildingRecovered(int);

	bool isBuried(int) const;
	TPollutionObj* getPollutionObj(int);
	TMapEventSink(const char*);

	static u32 mCleanedDegree;

public:
	/* 0x20 */ int mBuildingNum;
	/* 0x24 */ int unk24;
	/* 0x28 */ int unk28;
	/* 0x2C */ TJointObj* unk2C;
	/* 0x30 */ J3DJoint* unk30;
	/* 0x34 */ f32 unk34;
	/* 0x38 */ f32 unk38;
	/* 0x3C */ f32 unk3C;
	/* 0x40 */ int unk40;
	/* 0x44 */ int unk44;
	/* 0x48 */ int unk48;
	/* 0x4C */ int unk4C;
	/* 0x50 */ JGeometry::TVec3<f32>* unk50;
	/* 0x54 */ bool* unk54;
	/* 0x58 */ TMapCollisionWarp** unk58;
	/* 0x5C */ TMapCollisionMove** unk5C;
	struct Unk60Struct {
		u16 unk0;
		u16 unk2;
	};
	/* 0x60 */ Unk60Struct* unk60;
};

class TMapEventSinkInPollution : public TMapEventSink {
public:
	TMapEventSinkInPollution(const char* name)
	    : TMapEventSink(name)
	{
	}

	virtual void loadAfter();
	virtual bool watch();

	void initBuriedBuilding();
};

class TMapEventSinkInPollutionReset : public TMapEventSinkInPollution {
public:
	TMapEventSinkInPollutionReset(const char* name)
	    : TMapEventSinkInPollution(name)
	{
	}

	virtual void loadAfter();

	void getResetPollutionObj(int);
	void makeBuildingRecovered(int);
};

class TMapEventSinkBianco : public TMapEventSinkInPollutionReset {
public:
	TMapEventSinkBianco();

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual bool watch();
	virtual void startControl();
	virtual bool control();
	virtual void finishControl();
	virtual void rising();

public:
	/* 0x64 */ J3DJoint* unk64;
	/* 0x68 */ JDrama::TNameRef* mGateKeeper;
	/* 0x6C */ JGeometry::TVec3<f32> unk6C;
	/* 0x78 */ f32 unk78;
	/* 0x7C */ int unk7C;
};

class TMapEventSinkShadowMario : public TMapEventSink {
public:
	TMapEventSinkShadowMario();

	virtual void loadAfter();

	virtual void initBuilding(int, JSUMemoryInputStream&);
	virtual void initWithBuildingNum(JSUMemoryInputStream&);
	virtual void rising();

	void raiseBuilding(int);

public:
	/* 0x64 */ JDrama::TNameRef** unk64; // TODO: derived type
	/* 0x68 */ const char** unk68;
};

#endif

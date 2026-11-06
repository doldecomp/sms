#ifndef MAP_MAP_EVENT_SIRENA_HPP
#define MAP_MAP_EVENT_SIRENA_HPP

#include <Map/MapEventSink.hpp>

class TMapEventSirenaSink : public TMapEventSink {
public:
	TMapEventSirenaSink(const char* = "ホテル沈む");

	virtual void load(JSUMemoryInputStream& stream);
	virtual void loadAfter();
	virtual bool watch();
	virtual void initPollution() { }
	virtual void makePollutionRecovered(u32) { }

public:
	/* 0x64 */ bool mRiseRequested;
	/* 0x68 */ JGeometry::TVec3<f32> mShineSpawnPos;
	/* 0x74 */ JGeometry::TVec3<f32> mMarioWarpPosition;
	/* 0x80 */ f32 mMarioWarpAngle;
};

#endif

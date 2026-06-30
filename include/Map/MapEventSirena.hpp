#ifndef MAP_MAP_EVENT_SIRENA_HPP
#define MAP_MAP_EVENT_SIRENA_HPP

#include <Map/MapEventSink.hpp>

class TMapEventSirenaSink : public TMapEventSink {
public:
	TMapEventSirenaSink(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual bool watch();
	virtual void initPollution() { }
	virtual void makePollutionRecovered(u32) { }

public:
	/* 0x64 */ bool unk64;
	/* 0x68 */ JGeometry::TVec3<f32> unk68;
	/* 0x74 */ JGeometry::TVec3<f32> unk74;
	/* 0x80 */ f32 unk80;
};

#endif

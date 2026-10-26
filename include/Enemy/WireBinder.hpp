#ifndef ENEMY_WIRE_BINDER_HPP
#define ENEMY_WIRE_BINDER_HPP

#include <JSystem/JGeometry.hpp>
#include <Strategic/Binder.hpp>
#include <Strategic/LiveActor.hpp>

class TMapWire;

class TWireBinder : public TBinder {
public:
	bool init(const JGeometry::TVec3<f32>& wireNumber);
	bool reset(const JGeometry::TVec3<f32>& wireNumber);

	void bind(TLiveActor*);
	const JGeometry::TVec3<f32>& getDir() const { return mDir; }
	JGeometry::TVec3<f32> getDirAtPos(const JGeometry::TVec3<f32>&, f32) const;
	void getPoint(JGeometry::TVec3<f32>* out, f32 pos) const;
	void getPoint(JGeometry::TVec3<f32>* out,
	              const JGeometry::TVec3<f32>& point) const;

	static bool isOnWire(const JGeometry::TVec3<f32>& wireNumber);
	f32 getRangePos(const JGeometry::TVec3<f32>& point) const;
	TMapWire* getWire() const;
	bool isStartWire(const JGeometry::TVec3<f32>& point, f32 direction) const;
	bool isEndWire(const JGeometry::TVec3<f32>& point, f32 direction) const;
	void getStartRangePos(f32 direction);
	static f32 getEndRangePos(f32 direction);

private:
	/* 0x04 */ s32 mWireNumber;
	/* 0x08 */ JGeometry::TVec3<f32> mDir;
};

#endif

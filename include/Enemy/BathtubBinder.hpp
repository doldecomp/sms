#ifndef ENEMY_BATHTUB_BINDER_HPP
#define ENEMY_BATHTUB_BINDER_HPP

#include <Strategic/Binder.hpp>
#include <Map/BathWaterManager.hpp>
#include <MoveBG/MapObjCorona.hpp>
#include <JSystem/JGeometry.hpp>

class TLiveActor;

// Floats an actor on the bathtub's water. bind() samples the water ahead of
// and behind the actor and drives its height and pitch from the difference.
class TBathtubBinder : public TBinder {
public:
	TBathtubBinder();
	virtual ~TBathtubBinder();
	virtual void bind(TLiveActor*);

	bool init(f32 front_dist, f32 front_margin, f32 back_dist, f32 back_margin,
	          f32 height_offset);
	void float_(TLiveActor*);

	// UNUSED in the map at 0x164, inlined at all three float_ sites. It takes
	// the point by reference: that is what lets the third site share one tub
	// centre between the circle clamp and the floor clamp on y, and at the
	// first two sites the y clamp is dead-code-eliminated because nothing
	// reads the sampled point's y.
	void constrain_(JGeometry::TVec3<f32>& pos, f32 margin);

	// Fabricated. The accessor level is the last 8 bytes of float_'s
	// 0x178 frame.
	f32 getFrontMargin() const { return mFrontMargin; }

public:
	/* 0x04 */ TBathtub* mBathtub;        // "バスタブ"
	/* 0x08 */ TBathWaterManager* mWater; // "バスタブの水"
	/* 0x0C */ f32 mFrontDist;
	/* 0x10 */ f32 mFrontMargin;
	/* 0x14 */ f32 mBackDist;
	/* 0x18 */ f32 mBackMargin;
	/* 0x1C */ f32 mBackRatio;
	/* 0x20 */ f32 mHeightOffset;
};

#endif

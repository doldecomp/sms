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

	// Pulls a point back inside the tub's inner circle. Always inlined: the
	// map records no symbol for it, and float_ expands it three times.
	void clampToTub(f32* x, f32* z, f32 margin) const
	{
		const TBathtubData& data = mBathtub->getBathtubData();
		JGeometry::TVec3<f32> center = data.getThing();
		f32 radius = JGeometry::TUtil<f32>::sqrt(data.unk3C * data.unk3C
		                                         - data.unk44 * data.unk44)
		             - margin;
		f32 dz  = *z - center.z;
		f32 dx  = *x - center.x;
		f32 lsq = dx * dx + dz * dz;
		if (lsq > radius * radius) {
			f32 scale = radius * JGeometry::TUtil<f32>::inv_sqrt(lsq);
			*x        = scale * dx + center.x;
			*z        = scale * dz + center.z;
		}
	}

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

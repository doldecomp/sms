#ifndef JASINSTRAND_HPP
#define JASINSTRAND_HPP

#include <JSystem/JAudio/JASystem/JASInstEffect.hpp>
namespace JASystem {

class TInstRand : public TInstEffect {
public:
	TInstRand()
	    : mFloor(1.0f)
	    , mCeiling(0.0f)
	{
	}

	void setFloor(f32 floor) { mFloor = floor; }
	void setCeiling(f32 ceiling) { mCeiling = ceiling; }

	virtual f32 getY(int, int) const;

public:
	/* 0x8 */ f32 mFloor;
	/* 0xC */ f32 mCeiling;
};

} // namespace JASystem

#endif // JASINSTRAND_HPP

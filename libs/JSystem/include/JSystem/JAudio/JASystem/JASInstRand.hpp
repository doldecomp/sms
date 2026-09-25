#ifndef JASINSTRAND_HPP
#define JASINSTRAND_HPP

#include <JSystem/JAudio/JASystem/JASInstEffect.hpp>
namespace JASystem {

class TInstRand : public TInstEffect {
public:
	TInstRand()
	    : mBase(1.0f)
	    , mWidth(0.0f)
	{
	}

	void setBase(f32 base) { mBase = base; }
	void setWidth(f32 width) { mWidth = width; }

	virtual f32 getY(int, int) const;

public:
	/* 0x8 */ f32 mBase;
	/* 0xC */ f32 mWidth;
};

} // namespace JASystem

#endif // JASINSTRAND_HPP

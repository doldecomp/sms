#ifndef JASINSTEFFECT_HPP
#define JASINSTEFFECT_HPP

#include <dolphin/types.h>

namespace JASystem {

class TInstEffect {
public:
	TInstEffect()
	    : mTarget(0)
	{
	}

	virtual f32 getY(int, int) const = 0;

	void setTarget(int);
	u8 getTarget() const { return mTarget; }

public:
	/* 0x4 */ u8 mTarget;
};

} // namespace JASystem

#endif // JASINSTEFFECT_HPP

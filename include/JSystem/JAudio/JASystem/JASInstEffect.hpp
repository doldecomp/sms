#ifndef JASINSTEFFECT_HPP
#define JASINSTEFFECT_HPP

#include <dolphin/types.h>

namespace JASystem {

class TInstEffect {
public:
	TInstEffect()
	    : unk4(0)
	{
	}

	virtual f32 getY(int, int) const = 0;

	void setTarget(int);

public:
	/* 0x4 */ u8 unk4;
};

} // namespace JASystem

#endif // JASINSTEFFECT_HPP

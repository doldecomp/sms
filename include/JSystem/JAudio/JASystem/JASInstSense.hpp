#ifndef JASINSTSENSE_HPP
#define JASINSTSENSE_HPP

#include <JSystem/JAudio/JASystem/JASInstEffect.hpp>
#include <dolphin/types.h>

namespace JASystem {

class TInstSense : public TInstEffect {
public:
	TInstSense()
	    : unk8(0)
	    , unk9(0x3C)
	    , unkC(0.0f)
	    , unk10(0.0f)
	{
	}

	virtual f32 getY(int, int) const;
	void setParams(int, int, f32, f32);

	u8 unk8;
	u8 unk9;
	f32 unkC;
	f32 unk10;
};

} // namespace JASystem

#endif // JASINSTSENSE_HPP

#ifndef JASINSTSENSE_HPP
#define JASINSTSENSE_HPP

#include <JSystem/JAudio/JASystem/JASInstEffect.hpp>
#include <dolphin/types.h>

namespace JASystem {

class TInstSense : public TInstEffect {
public:
	TInstSense()
	    : mSource(0)
	    , mCenter(60)
	    , mLowY(1.0f)
	    , mHighY(1.0f)
	{
	}

	virtual f32 getY(int, int) const;
	void setParams(int source, int center, f32 low_value, f32 high_value);

	u8 mSource;
	u8 mCenter;
	f32 mLowY;
	f32 mHighY;
};

} // namespace JASystem

#endif // JASINSTSENSE_HPP

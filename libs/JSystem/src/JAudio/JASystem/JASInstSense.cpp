#include <JSystem/JAudio/JASystem/JASInstSense.hpp>

namespace JASystem {

f32 TInstSense::getY(int key, int velocity) const
{
	int x = 0;
	switch (mSource) {
	case 1:
		x = velocity;
		break;
	case 2:
		x = key;
		break;
	}

	f32 y;
	if (mCenter == 127 || mCenter == 0)
		y = mLowY + x * (mHighY - mLowY) / 127.0f;
	else if (x < mCenter)
		y = mLowY + (1.0f - mLowY) * (x / (f32)mCenter);
	else
		y = (mHighY - 1.0f) * ((x - mCenter) / (f32)(127 - mCenter)) + 1.0f;
	return y;
}

void TInstSense::setParams(int source, int center, f32 low_value,
                           f32 high_value)
{
	mSource = source;
	mCenter = center;
	mLowY   = low_value;
	mHighY  = high_value;
}

} // namespace JASystem

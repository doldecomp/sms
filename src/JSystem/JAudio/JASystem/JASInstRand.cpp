#include <JSystem/JAudio/JASystem/JASInstRand.hpp>
#include <JSystem/JMath.hpp>

namespace JASystem {

f32 TInstRand::getY(int, int) const
{
	static JMath::TRandomFast oRandom(0);
	f32 val = oRandom.get_sfloat_1();
	val *= mCeiling;
	val += mFloor;
	return val;
}

} // namespace JASystem

#include <JSystem/JAudio/JASystem/JASInstRand.hpp>
#include <JSystem/JMath.hpp>

namespace JASystem {

f32 TInstRand::getY(int, int) const
{
	static JMath::TRandom_fast_ oRandom(0);
	f32 tmp1 = oRandom.get_ufloat_1() * 2.0f;
	f32 tmp2 = tmp1 - 0.9999999f;
	tmp2 *= unkC;
	tmp2 += unk8;
	return tmp2;
}

} // namespace JASystem

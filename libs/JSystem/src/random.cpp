#include <JSystem/JMath.hpp>

using namespace JMath;

TRandom_fast_::TRandom_fast_(u32 seed) { value = seed; }

void TRandom_enough_::setSeed(u32 seed)
{
	unk4 = 0;
	unk0 = seed + 1;
	if (unk0 == 0)
		unk0 += 1;

	u32 i = 8;
	do {
		unk0 = (u32)(((u64)unk0 * 0x41a7) % 0x7fffffff);
	} while (--i);

	i = 32;
	do {
		--i;
		unk0    = (u32)(((u64)unk0 * 0x41a7) % 0x7fffffff);
		unk8[i] = unk0;
	} while (i != 0);
}

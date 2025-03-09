#include <JSystem/JAudio/JASystem/JASCalc.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <dolphin/os/OSCache.h>

namespace JASystem {

namespace Calc {
	static f32* JASC_SINTABLE  = 0;
	static f32* JASC_DOL2TABLE = 0;
} // namespace Calc

void Calc::initSinfT()
{
	JASC_SINTABLE  = new (JASDram, 0) f32[257];
	JASC_DOL2TABLE = new (JASDram, 0) f32[257];
	for (u32 i = 0; i < 257; ++i)
		JASC_SINTABLE[i] = sinf((M_PI / 2) * i / 256.0f);

	for (u32 i = 0; i < 257; ++i)
		JASC_DOL2TABLE[i]
		    = sinf((M_PI / 2) * (0.32612f + 0.34776f * i / 256.0f));
}

f32 Calc::sinfT(f32 angle) { return JASC_SINTABLE[s32(angle * 256.0f)]; }

f32 Calc::sinfDolby2(f32 angle) { return JASC_DOL2TABLE[s32(angle * 256.0f)]; }

f32 Calc::sinfM(f32 angle) { return 0.0f; }

f32 Calc::cosfM(f32 angle) { return 0.0f; }

s32 Calc::limit16(s32 value) { return 0; }

void Calc::mixcopy(const s16* src1, const s16* src2, s16* dest, s32 size) { }

void Calc::imixcopy(const s16* src1, const s16* src2, s16* dest, s32 size)
{
	for (; size > 0; --size) {
		*dest++ = *src1++;
		*dest++ = *src2++;
	}
}

void Calc::bcopyW(const s16* src, s16* dest, s32 size) { }

void Calc::bcopyfast(const u32* src, u32* dest, u32 size)
{
	for (size /= 16; size; --size) {
		u32 val1 = *src++;
		u32 val2 = *src++;
		u32 val3 = *src++;
		u32 val4 = *src++;
		*dest++  = val1;
		*dest++  = val2;
		*dest++  = val3;
		*dest++  = val4;
	}
}

void Calc::bcopy(const void* src, void* dest, u32 size)
{
	u32* usrc;
	u32* udest;

	u8* bsrc  = (u8*)src;
	u8* bdest = (u8*)dest;

	u8 endbitsSrc = (reinterpret_cast<u32>(bsrc) & 0x03);
	u8 enbitsDst  = (reinterpret_cast<u32>(bdest) & 0x03);
	if ((endbitsSrc) == (enbitsDst) && (size & 0x0f) == 0) {
		bcopyfast((u32*)src, (u32*)dest, size);
	} else if ((endbitsSrc == enbitsDst) && (size >= 16)) {
		if (endbitsSrc != 0) {
			for (endbitsSrc = 4 - endbitsSrc; endbitsSrc != 0; --endbitsSrc) {
				*bdest++ = (u32)*bsrc++;
				size--;
			}
		}

		udest = (u32*)bdest;
		usrc  = (u32*)bsrc;

		for (; size >= 4; size -= 4)
			*udest++ = *usrc++;

		if (size != 0) {
			bdest = (u8*)udest;
			bsrc  = (u8*)usrc;

			for (; size != 0; --size)
				*bdest++ = (u32)*bsrc++;
		}
	} else {
		for (; size != 0; --size)
			*bdest++ = (u32)*bsrc++;
	}
}

void Calc::bzerofast(u32* dest, u32 size)
{
	u32* udest = (u32*)dest;
	for (size = size / 16; size != 0; size--) {
		*udest++ = 0;
		*udest++ = 0;
		*udest++ = 0;
		*udest++ = 0;
	}
}

void Calc::bzero(void* dest, u32 size)
{
	u32* udest;
	u8* bdest = (u8*)dest;
	if ((size & 0x1f) == 0 && (reinterpret_cast<u32>(dest) & 0x1f) == 0) {
		DCZeroRange(dest, size);
		return;
	}

	u8 alignedbitsDst = reinterpret_cast<u32>(bdest) & 0x3;

	if ((size & 0xf) == 0 && alignedbitsDst == 0) {
		bzerofast((u32*)dest, size);
		return;
	}

	if (size >= 16) {
		if (alignedbitsDst != 0) {
			for (alignedbitsDst = 4 - alignedbitsDst; alignedbitsDst != 0;
			     --alignedbitsDst) {
				*bdest++ = 0;
				size--;
			}
		}

		udest = (u32*)bdest;
		for (; size >= 4; size -= 4) {
			*udest++ = 0;
		}

		if (size != 0) {
			bdest = (u8*)udest;
			for (; size != 0; --size) {
				*bdest++ = 0;
			}
		}
	} else {
		for (; size != 0; --size) {
			*bdest++ = 0;
		}
	}
}

void Calc::hannWindow(s16* data, s32 size) { }

void Calc::hammWindow(s16* data, s32 size) { }

void Calc::fft(f32* real, f32* imag, s32 size, s32 dir) { }

} // namespace JASystem

#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/mem_TRK.h"
#include "dolphin/types.h"

void TRK_fill_mem(void* dst, int val, size_t n)
{
	u32 v, i, j;
	v = (u8)val;

	((u8*)dst) = ((u8*)dst) - 1;

	if (n >= 32) {
		i = (~(u32)dst) & 3;

		if (i) {
			n -= i;

			do {
				*++(((u8*)dst)) = v;
			} while (--i);
		}

		if (v)
			v |= v << 24 | v << 16 | v << 8;

		((u32*)dst) = ((u32*)(((u8*)dst) + 4)) - 1;
		((u32*)dst) = ((u32*)(((u8*)dst) + 1)) - 1;

		i = n / 32;

		if (i) {
			do {
				for (j = 0; j < 8; j++)
					*++((u32*)dst) = v;
			} while (--i);
		}

		i = (n / 4) % 8;

		if (i) {
			do {
				*++((u32*)dst) = v;
			} while (--i);
		}

		((u8*)dst) = ((u8*)(((u32*)dst) + 1)) - 1;

		n %= 4;
	}

	if (n)
		do {
			*++((u8*)dst) = v;
		} while (--n);
}

__declspec(section ".init") void* TRK_memcpy(void* dst, const void* src,
                                             size_t n)
{
	const char* p;
	char* q;

	for (p = (const char*)src - 1, q = (char*)dst - 1, n++; --n;)
		*++q = *++p;

	return dst;
}

__declspec(section ".init") void* TRK_memset(void* dst, int val, size_t n)
{
	TRK_fill_mem(dst, val, n);
	return dst;
}

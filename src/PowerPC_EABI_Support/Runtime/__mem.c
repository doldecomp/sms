#include "PowerPC_EABI_Support/Runtime/__mem.h"

__declspec(section ".init") void* memcpy(void* dst, const void* src, size_t n)
{
	const char* p;
	char* q;
	int rev = ((unsigned long)src < (unsigned long)dst);

	if (!rev) {

		for (p = (const char*)src - 1, q = (char*)dst - 1, n++; --n;)
			*++q = *++p;

	} else {
		for (p = (const char*)src + n, q = (char*)dst + n, n++; --n;)
			*--q = *--p;
	}
	return (dst);
}

__declspec(section ".init") void __fill_mem(void* dst, int val, unsigned long n)
{
	unsigned long v = (unsigned char)val;
	unsigned long i;

	((unsigned char*)dst) = ((unsigned char*)dst) - 1;

	if (n >= 32) {
		i = (~(unsigned long)dst) & 3;

		if (i) {
			n -= i;

			do
				*++(((unsigned char*)dst)) = v;
			while (--i);
		}

		if (v)
			v |= v << 24 | v << 16 | v << 8;

		((unsigned long*)dst)
		    = ((unsigned long*)(((unsigned char*)dst) + 1)) - 1;

		i = n >> 5;

		if (i)
			do {
				*++(((unsigned long*)dst)) = v;
				*++(((unsigned long*)dst)) = v;
				*++(((unsigned long*)dst)) = v;
				*++(((unsigned long*)dst)) = v;
				*++(((unsigned long*)dst)) = v;
				*++(((unsigned long*)dst)) = v;
				*++(((unsigned long*)dst)) = v;
				*++(((unsigned long*)dst)) = v;
			} while (--i);

		i = (n & 31) >> 2;

		if (i)
			do
				*++(((unsigned long*)dst)) = v;
			while (--i);

		((unsigned char*)dst)
		    = ((unsigned char*)(((unsigned long*)dst) + 1)) - 1;

		n &= 3;
	}

	if (n)
		do
			*++(((unsigned char*)dst)) = v;
		while (--n);

	return;
}

__declspec(section ".init") void* memset(void* dst, int val, size_t n)
{
	__fill_mem(dst, val, n);

	return (dst);
}

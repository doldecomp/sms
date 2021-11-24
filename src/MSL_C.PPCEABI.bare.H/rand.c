#include <dolphin.h>

// rand.c from Runtime library

u32 next = 1;

void srand(u32 seed) { next = seed; }

u32 rand(void)
{
	next = 0x41C64E6D * next + 12345;
	return (next >> 16) & 0x7FFF;
}

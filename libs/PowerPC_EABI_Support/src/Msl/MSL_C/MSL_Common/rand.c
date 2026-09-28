#include <stdlib.h>

// rand.c from Runtime library

size_t next = 1;

int rand(void)
{
	next = 0x41C64E6D * next + 12345;
	return (next >> 16) & 0x7FFF;
}

void srand(size_t seed) { next = seed; }

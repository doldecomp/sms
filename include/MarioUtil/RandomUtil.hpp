#ifndef MARIO_UTIL_RANDOM_UTIL_HPP
#define MARIO_UTIL_RANDOM_UTIL_HPP

#include <dolphin/types.h>
#include <stdlib.h>

// NOTE: the entire file and all functions in it
// is made-up. Seems we got unlucky and all of these
// helpers got completely inlined.

// this is found in code so much that I am almost
// certain that it is real
inline f32 MsRandF() { return rand() * (1.f / (RAND_MAX + 1)); }

// TODO: fake!!! need to analyze a bunch of callsites,
// smallEnemy kind of implies a random interval class or
// something like that
inline f32 MsRandF(f32 l, f32 r)
{
	return rand() * (1.f / (RAND_MAX + 1)) * (r - l) + l;
}

#endif

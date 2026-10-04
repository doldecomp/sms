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

inline int MsRandI(int l, int r)
{
	int rnd = rand() * (1.f / (RAND_MAX + 1)) * (r - l);
	return 1 + l + rnd;
}

// A random-in-range helper. Only ever used fully inlined, so the only trace
// left in the binary is the two UNUSED dtors for TMsRange<f32> / TMsRange<s32>.
template <typename T> class TMsRange {
public:
	TMsRange(T min, T max)
	    : mMin(min)
	    , mMax(max)
	{
	}

	void set(T min, T max)
	{
		mMin = min;
		mMax = max;
	}

	T rand() const
	{
		T range = mMax - mMin;
		return mMin + (T)(range * MsRandF());
	}

	// real AND required
	// Without it, codegen is wrong for `TMsRange::rand` AND every single class
	// inheriting from TSmallEnemyParams. Why? No idea.
	~TMsRange() { }

public:
	/* 0x0 */ T mMin;
	/* 0x4 */ T mMax;
};

#endif

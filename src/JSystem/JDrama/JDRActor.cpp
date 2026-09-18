#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JDrama/JDRActor.hpp>
#include <JSystem/JDrama/JDRLighting.hpp>
#include <JSystem/JDrama/JDRCharacter.hpp>

// TODO: all 85 instructions and the 0xb0 frame match; the whole stack is
// shifted by 4 (retail's name buffer is at 0x4c(r1) and the TLightMap ctor's
// `this` temporary at 0x40, ours at 0x50 and 0x44). Splitting the frame into
// inline-temp pool (from 0xc up) / named locals (down from the saves at 0xa4):
// retail is pool 64 + buffer 80 + a 4-byte named local above it plus 4 of
// alignment; ours is pool 68 + buffer 80 + 4 of alignment. So it needs **-4 of
// pool and one 4-byte named local declared (and initialised) before the
// buffer** at the same time, and no single lever does both.
// Measured here: each `>>` continuation is 8 pool bytes (2 statements of three
// reads = 68, three statements of two = 60, one chain of six = 76 -- so the
// two-statement form is right and the pool is 4 too big for an unknown reason);
// every +4 named local (`char str2[4]`, an initialised `u32`, a wider buffer)
// keeps the buffer where it is and grows the frame to 0xb8. Worth nothing:
// split-declared or null-initialised `TLightMap*`/`TCharacter*` locals (they
// land below the buffer), `(TCharacter*)TNameRefGen::search2(str)` instead of
// `search<TCharacter>` (codegen-identical here), `unk40 = new TLightMap` with
// the load through the member, `new TLightMap()`, and a named `u16` length for
// `readString`.
void JDrama::TActor::load(JSUMemoryInputStream& stream)
{
	TPlacement::load(stream);

	stream >> mRotation.x >> mRotation.y >> mRotation.z;
	stream >> mScaling.x >> mScaling.y >> mScaling.z;

	char str[0x50];
	stream.readString(str, 0x50);

	unk3C = TNameRefGen::search<TCharacter>(str);

	TLightMap* lightMap = new TLightMap;

	unk40 = lightMap;
	lightMap->load(stream);
}

void JDrama::TActor::issueGXLight(u32 param_1, JDrama::TGraphics* param_2)
{
	if (unk40 != nullptr)
		unk40->perform(param_1 | CUE_LIGHT, param_2);
}

void JDrama::TActor::perform(u32 cue, TGraphics* graphics)
{
	if (cue & CUE_DRAW)
		issueGXLight(cue, graphics);
}

JDrama::TActor::~TActor() { }

void JDrama::TActor::JSGGetTranslation(Vec* v) const { *v = mPosition; }

void JDrama::TActor::JSGSetTranslation(const Vec& v)
{
	mPosition.x = v.x;
	mPosition.y = v.y;
	mPosition.z = v.z;
}

void JDrama::TActor::JSGGetScaling(Vec* v) const { *v = mScaling; }

void JDrama::TActor::JSGSetScaling(const Vec& v)
{
	mScaling.x = v.x;
	mScaling.y = v.y;
	mScaling.z = v.z;
}

void JDrama::TActor::JSGGetRotation(Vec* v) const { *v = mRotation; }

void JDrama::TActor::JSGSetRotation(const Vec& v)
{
	mRotation.x = v.x;
	mRotation.y = v.y;
	mRotation.z = v.z;
}

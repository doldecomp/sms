#ifndef JASCALC_HPP
#define JASCALC_HPP

#include <math.h>
#include <dolphin/types.h>

namespace JASystem {

namespace Calc {

	extern f32* JASC_SINTABLE;
	extern f32* JASC_DOL2TABLE;

	void bcopy(const void* src, void* dest, u32 size);
	void bzero(void* dest, u32 size);
	void bcopyfast(const u32* src, u32* dest, u32 size);
	void bzerofast(u32* dest, u32 size);
	void bcopyW(const s16* src, s16* dest, s32 size);

	void initSinfT();
	f32 sinfT(f32 angle);
	f32 sinfDolby2(f32 angle);
	f32 sinfM(f32 angle);
	f32 cosfM(f32 angle);

	s32 limit16(s32 value);

	void mixcopy(const s16* src1, const s16* src2, s16* dest, s32 size);
	void imixcopy(const s16* src1, const s16* src2, s16* dest, s32 size);

	void hannWindow(s16* data, s32 size);
	void hammWindow(s16* data, s32 size);
	void fft(f32* real, f32* imag, s32 size, s32 dir);

} // namespace Calc

} // namespace JASystem

#endif // JASCALC_HPP

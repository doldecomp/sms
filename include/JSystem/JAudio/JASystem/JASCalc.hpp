#ifndef JASCALC_HPP
#define JASCALC_HPP

#include <math.h>

namespace JASystem {

namespace Calc {

	extern float JASC_SINTABLE[];
	extern float JASC_DOL2TABLE[];

	void bcopy(const void* src, void* dest, unsigned long size);
	void bzero(void* dest, unsigned long size);
	void bcopyfast(const unsigned long* src, unsigned long* dest,
	               unsigned long size);
	void bzerofast(unsigned long* dest, unsigned long size);
	void bcopyW(const short* src, short* dest, long size);

	void initSinfT();
	float sinfT(float angle);
	float sinfDolby2(float angle);
	float sinfM(float angle);
	float cosfM(float angle);

	long limit16(long value);

	void mixcopy(const short* src1, const short* src2, short* dest, long size);
	void imixcopy(const short* src1, const short* src2, short* dest, long size);

	void hannWindow(short* data, long size);
	void hammWindow(short* data, long size);
	void fft(float* real, float* imag, long size, long dir);

} // namespace Calc

} // namespace JASystem

#endif // JASCALC_HPP

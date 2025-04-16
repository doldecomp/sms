#ifndef JG_MATRIX34_HPP
#define JG_MATRIX34_HPP

#include <dolphin/types.h>

namespace JGeometry {

inline void gekko_ps_copy12(register void* dst, register void* src)
{
	register f32 src0;
	register f32 src1;
	register f32 src2;
	register f32 src3;
	register f32 src4;
	register f32 src5;
#ifdef __MWERKS__ // clang-format off
	asm {
		psq_l src0, 0(src), 0, 0
		psq_l src1, 8(src), 0, 0
		psq_l src2, 16(src), 0, 0
		psq_l src3, 24(src), 0, 0
		psq_l src4, 32(src), 0, 0
		psq_l src5, 40(src), 0, 0
		psq_st src0, 0(dst), 0, 0
		psq_st src1, 8(dst), 0, 0
		psq_st src2, 16(dst), 0, 0
		psq_st src3, 24(dst), 0, 0
		psq_st src4, 32(dst), 0, 0
		psq_st src5, 40(dst), 0, 0
	}
#endif // clang-format on
}

template <typename T> struct SMatrix34C {
	T mMtx[3][4];
};

// NOTE: this HAS to be an explicit specialization for function prologue &
// epilogue to match. Whether it has to have 8-byte alignment is not certain. It
// helps some stack frame layouts to match, but it also might just be that
// function calls got inlined in those places and matrices don't really need the
// alignment.
// TODO: figure out whether we need 8-byte alignment here
template <> struct SMatrix34C<f32> {
	SMatrix34C() { }

	void set(f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

	typedef f32 ArrType[4];
	operator ArrType*() { return mMtx; }
	operator const ArrType*() const { return mMtx; }

	void set(const ArrType* src)
	{
		void* ptr = mMtx;
		gekko_ps_copy12(ptr, (void*)src);
	}

	f32 mMtx[3][4];
};

template <typename T> struct TMatrix34 : public T {
	TMatrix34() { }
};

} // namespace JGeometry

#endif

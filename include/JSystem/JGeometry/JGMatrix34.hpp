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
// epilogue to match.
template <> class SMatrix34C<f32> {
public:
	SMatrix34C() { }

	void set(f32 m00, f32 m01, f32 m02, f32 m03, f32 m10, f32 m11, f32 m12,
	         f32 m13, f32 m20, f32 m21, f32 m22, f32 m23)
	{
		mMtx[0][0] = m00;
		mMtx[0][1] = m01;
		mMtx[0][2] = m02;
		mMtx[0][3] = m03;

		mMtx[1][0] = m10;
		mMtx[1][1] = m11;
		mMtx[1][2] = m12;
		mMtx[1][3] = m13;

		mMtx[2][0] = m20;
		mMtx[2][1] = m21;
		mMtx[2][2] = m22;
		mMtx[2][3] = m23;
	}

	typedef f32 ArrType[4];
	operator ArrType*() { return mMtx; }
	operator const ArrType*() const { return mMtx; }

	void set(const ArrType* src)
	{
		void* ptr = mMtx;
		gekko_ps_copy12(ptr, (void*)src);
	}

	f32 at(u32 i, u32 j) const { return mMtx[i][j]; }
	f32& ref(u32 i, u32 j) { return mMtx[i][j]; }

	f32 mMtx[3][4];
};

template <typename T> class TMatrix34 : public T {
public:
	TMatrix34() { }

	// from SMG
	void identity()
	{
		this->ref(0, 3) = this->ref(1, 3) = this->ref(2, 3) = 0.0f;

		this->ref(0, 2) = this->ref(1, 2) = 0.0f;
		this->ref(0, 1) = this->ref(2, 1) = 0.0f;
		this->ref(1, 0) = this->ref(2, 0) = 0.0f;

		this->ref(0, 0) = this->ref(1, 1) = this->ref(2, 2) = 1.0f;
	}

	void concat(const T& a, const T& b)
	{
		this->set(
		    // clang-format off
		a.at(0, 0) * b.at(0, 0) + a.at(1, 0) * b.at(0, 1) + a.at(2, 0) * b.at(0, 2),
		a.at(0, 0) * b.at(1, 0) + a.at(1, 0) * b.at(1, 1) + a.at(2, 0) * b.at(1, 2),
		a.at(0, 0) * b.at(2, 0) + a.at(1, 0) * b.at(2, 1) + a.at(2, 0) * b.at(2, 2),
		a.at(0, 0) * b.at(0, 3) + a.at(1, 0) * b.at(3, 1) + a.at(2, 0) * b.at(3, 2) + a.at(3, 0),

		a.at(0, 1) * b.at(0, 0) + a.at(1, 1) * b.at(0, 1) + a.at(2, 1) * b.at(0, 2),
		a.at(0, 1) * b.at(1, 0) + a.at(1, 1) * b.at(1, 1) + a.at(2, 1) * b.at(1, 2),
		a.at(0, 1) * b.at(2, 0) + a.at(1, 1) * b.at(2, 1) + a.at(2, 1) * b.at(2, 2),
		a.at(0, 1) * b.at(3, 0) + a.at(1, 1) * b.at(3, 1) + a.at(2, 1) * b.at(3, 2) + a.at(3, 1),

		a.at(0, 2) * b.at(0, 0) + a.at(1, 2) * b.at(0, 1) + a.at(2, 2) * b.at(0, 2),
		a.at(0, 2) * b.at(1, 0) + a.at(1, 2) * b.at(1, 1) + a.at(2, 2) * b.at(1, 2),
		a.at(0, 2) * b.at(2, 0) + a.at(1, 2) * b.at(2, 1) + a.at(2, 2) * b.at(2, 2),
		a.at(0, 2) * b.at(3, 0) + a.at(1, 2) * b.at(3, 1) + a.at(2, 2) * b.at(3, 2) + a.at(3, 2)
		    // clang-format on
		);
	}

	void concat(const T& b)
	{
		this->set(
		    // clang-format off
		this->at(0, 0) * b.at(0, 0) + this->at(1, 0) * b.at(0, 1) + this->at(2, 0) * b.at(0, 2),
		this->at(0, 0) * b.at(1, 0) + this->at(1, 0) * b.at(1, 1) + this->at(2, 0) * b.at(1, 2),
		this->at(0, 0) * b.at(2, 0) + this->at(1, 0) * b.at(2, 1) + this->at(2, 0) * b.at(2, 2),
		this->at(0, 3) + this->at(0, 0) * b.at(0, 3) + this->at(0, 1) * b.at(1, 3) + this->at(0, 2) * b.at(2, 3),

		this->at(0, 1) * b.at(0, 0) + this->at(1, 1) * b.at(0, 1) + this->at(2, 1) * b.at(0, 2),
		this->at(0, 1) * b.at(1, 0) + this->at(1, 1) * b.at(1, 1) + this->at(2, 1) * b.at(1, 2),
		this->at(0, 1) * b.at(2, 0) + this->at(1, 1) * b.at(2, 1) + this->at(2, 1) * b.at(2, 2),
		this->at(1, 3) + this->at(1, 0) * b.at(0, 3) + this->at(1, 1) * b.at(1, 3) + this->at(1, 2) * b.at(2, 3),

		this->at(0, 2) * b.at(0, 0) + this->at(1, 2) * b.at(0, 1) + this->at(2, 2) * b.at(0, 2),
		this->at(0, 2) * b.at(1, 0) + this->at(1, 2) * b.at(1, 1) + this->at(2, 2) * b.at(1, 2),
		this->at(0, 2) * b.at(2, 0) + this->at(1, 2) * b.at(2, 1) + this->at(2, 2) * b.at(2, 2),
		this->at(2, 3) + this->at(2, 0) * b.at(0, 3) + this->at(2, 1) * b.at(1, 3) + this->at(2, 2) * b.at(2, 3)
		    // clang-format on
		);
	}
};

} // namespace JGeometry

#endif

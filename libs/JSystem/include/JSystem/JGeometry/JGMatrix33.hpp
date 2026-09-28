#ifndef JG_MATRIX33_HPP
#define JG_MATRIX33_HPP

#include <JSystem/JGeometry/JGVec3.hpp>
#include <dolphin/types.h>

namespace JGeometry {

template <typename T> struct SMatrix33C {
	T mMtx[3][3];
};

// Same explicit specialization trick as with 3x4 and 4x4 versions
template <> struct SMatrix33C<f32> {
	SMatrix33C() { }

	f32 at(u32 i, u32 j) const { return mMtx[i][j]; }
	f32& ref(u32 i, u32 j) { return mMtx[i][j]; }

	f32 mMtx[3][3];
};

// Row-major counterpart of SMatrix33C: at()/ref() are transposed, so setting a
// direction (a column in at() terms) writes three contiguous floats.
template <typename T> struct SMatrix33R {
	T mMtx[3][3];
};

template <> struct SMatrix33R<f32> {
	SMatrix33R() { }

	f32 at(u32 i, u32 j) const { return mMtx[j][i]; }
	f32& ref(u32 i, u32 j) { return mMtx[j][i]; }

	f32 mMtx[3][3];
};

template <typename T> struct TMatrix33 : public T {
	TMatrix33() { }

	void mult(const TVec3<f32>& src, TVec3<f32>& dst) const
	{
		f32 x = this->at(0, 0) * src.x + this->at(0, 1) * src.y
		        + this->at(0, 2) * src.z;
		f32 y = this->at(1, 0) * src.x + this->at(1, 1) * src.y
		        + this->at(1, 2) * src.z;
		f32 z = this->at(2, 0) * src.x + this->at(2, 1) * src.y
		        + this->at(2, 2) * src.z;
		dst.set(x, y, z);
	}

	// TWW's debug map (frameworkD.map, built with inlining off) records this
	// overload at 0x3c -- 15 instructions, too few for an inline 3x3 multiply
	// but exactly a forwarding call -- so in TWW's JSystem it is `mult(v, v)`.
	// Written out here because SMS's JSystem is a year older and the forwarding
	// form measurably regresses JPADrawExecStripe; revisit if evidence appears.
	void mult(TVec3<f32>& v) const
	{
		v.set(
		    this->at(0, 0) * v.x + this->at(0, 1) * v.y + this->at(0, 2) * v.z,
		    this->at(1, 0) * v.x + this->at(1, 1) * v.y + this->at(1, 2) * v.z,
		    this->at(2, 0) * v.x + this->at(2, 1) * v.y + this->at(2, 2) * v.z);
	}

	// fabricated
	void identity()
	{
		this->ref(0, 2) = this->ref(1, 2) = 0.0f;
		this->ref(0, 1) = this->ref(2, 1) = 0.0f;
		this->ref(1, 0) = this->ref(2, 0) = 0.0f;

		this->ref(0, 0) = this->ref(1, 1) = this->ref(2, 2) = 1.0f;
	}
};

} // namespace JGeometry

#endif

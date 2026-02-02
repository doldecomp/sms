#ifndef JG_MATRIX33_HPP
#define JG_MATRIX33_HPP

#include <dolphin/types.h>

namespace JGeometry {

template <typename T> struct SMatrix33C {
	T mMtx[3][3];
};

// Same explicit specialization trick as with 3x4 and 4x4 versions
template <> struct SMatrix33C<f32> {
	f32 at(u32 i, u32 j) const { return mMtx[i][j]; }
	f32& ref(u32 i, u32 j) { return mMtx[i][j]; }

	f32 mMtx[3][3];
};

template <typename T> struct TMatrix33 : public T {
	TMatrix33() { }
};

} // namespace JGeometry

#endif

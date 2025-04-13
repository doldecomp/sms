#ifndef JG_MATRIX34_HPP
#define JG_MATRIX34_HPP

#include <dolphin/types.h>

namespace JGeometry {

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
	SMatrix34C();

	void set(f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

	typedef f32 ArrType[4];
	operator ArrType*() { return mMtx; }
	operator const ArrType*() const { return mMtx; }

	f32 mMtx[3][4];
};

template <typename T> struct TMatrix34 : public T { };

} // namespace JGeometry

#endif

#ifndef JG_MATRIX44_HPP
#define JG_MATRIX44_HPP

#include <dolphin/types.h>

namespace JGeometry {

template <typename T> struct SMatrix44C {
	T mMtx[4][4];
};

// Size of SMatrix44C<f32> is extremely big if we don't use explicit
// specialization. I guess this is a bug in mwcc and it assumes T to have the
// max possible alignment or something?
template <> struct SMatrix44C<f32> {
	SMatrix44C() { }

	f32 mMtx[4][4];
};

template <typename T> class TMatrix44 : public T {
public:
	TMatrix44() { }
};

} // namespace JGeometry

#endif

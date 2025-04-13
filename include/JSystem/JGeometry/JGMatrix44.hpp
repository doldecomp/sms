#ifndef JG_MATRIX44_HPP
#define JG_MATRIX44_HPP

namespace JGeometry {

template <typename T> struct SMatrix44C {
	T mMtx[4][4];
};

template <typename T> struct TMatrix44 : public T {
	TMatrix44() { }
};

} // namespace JGeometry

#endif

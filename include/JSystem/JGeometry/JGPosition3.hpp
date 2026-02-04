#ifndef JG_POSITION3_HPP
#define JG_POSITION3_HPP

#include <JSystem/JGeometry/JGRotation3.hpp>

namespace JGeometry {

// Inherits from TRotation3 according to tp debug
template <class T> class TPosition3 : public TRotation3<T> {
public:
	TPosition3() { }

	// fabricated
	void identity33() { TRotation3<T>::identity33(); }
	void translation(f32 x, f32 y, f32 z)
	{
		this->identity33();
		this->setTrans(x, y, z);
	}

	// From SMG. These do NOT call the identity33() function.
	// Implementations arbitrary, feel free to play around with them.
	// void setTrans(const TVec3<f32>& translation)
	// {
	// 	setTrans(translation.x, translation.y, translation.z);
	// }
	void setTrans(const TVec3<f32>& translation)
	{
		this->ref(0, 3) = translation.x;
		this->ref(1, 3) = translation.y;
		this->ref(2, 3) = translation.z;
	}
	void setTrans(f32 x, f32 y, f32 z)
	{
		this->ref(0, 3) = x;
		this->ref(1, 3) = y;
		this->ref(2, 3) = z;
	}
	void zeroTrans() { setTrans(0.0f, 0.0f, 0.0f); }
};

} // namespace JGeometry

#endif

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

	// From SMG, should be real
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
	void getTrans(JGeometry::TVec3<f32>& translation) const
	{
		translation.set(this->at(0, 3), this->at(1, 3), this->at(2, 3));
	}
	void zeroTrans()
	{
		this->ref(0, 3) = this->ref(1, 3) = this->ref(2, 3) = 0.0f;
	}
	void setQT(const TQuat4<f32>& quat, const TVec3<f32>& trans)
	{
		this->setQuat(quat);
		this->setTrans(trans);
	}

	// from MKDD, may be useful in the future?
	void makeRotate(const JGeometry::TVec3<f32>&, f32);
	void setPositionFromLookAt(const TPosition3<T>&);
};

} // namespace JGeometry

#endif

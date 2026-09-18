#ifndef JG_POSITION3_HPP
#define JG_POSITION3_HPP

#include <JSystem/JGeometry/JGRotation3.hpp>

namespace JGeometry {

// Inherits from TRotation3 according to tp debug
template <class T> class TPosition3 : public TRotation3<T> {
public:
	TPosition3() { }

	// fabricated
	TPosition3(const Vec& trans)
	{
		identity33();
		setTrans(trans.x, trans.y, trans.z);
	}

	// fabricated
	TPosition3(f32 x, f32 y, f32 z)
	{
		identity33();
		setTrans(x, y, z);
	}

	// fabricated
	void identity33() { TRotation3<T>::identity33(); }
	void translation(f32 x, f32 y, f32 z)
	{
		this->identity33();
		this->setTrans(x, y, z);
	}
	// Ruled out (header round 12): spelling the body
	// `setTrans(t.x, t.y, t.z)`, which is +8 of frame on
	// TTalkCursor::associateNPC with no instruction change. Applied
	// project-wide it costs TFireWanwanTailHit::init 96.14 -> 94.80,
	// TFireWanwanTailHit::moveRequest 99.38 -> 93.96 and
	// TFireWanwan::init 99.89 -> 97.43, and associateNPC's own score does not
	// move.
	void translation(const TVec3<f32>& translation)
	{
		this->identity33();
		this->setTrans(translation);
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
	// The scaled sibling of setQT. It is never emitted (no symbol in the map),
	// but it is the level that keeps TRotation3::setSQ a `bl` at the one place
	// the ROM calls it out of line, TBeeHive::calcRootMatrix -- setSQ inlines
	// at depth 1 however it is spelled, and this one-line forwarder is what
	// pushes it to depth 2.
	void setSQT(const TVec3<f32>& scale, const TQuat4<f32>& quat,
	            const TVec3<f32>& trans)
	{
		this->setSQ(scale, quat);
		this->setTrans(trans);
	}

	void makeRotate(const JGeometry::TVec3<f32>&, f32);
	void setPositionFromLookAt(const TPosition3<T>&);
	void makeQuat(const TQuat4<f32>&);
};

} // namespace JGeometry

#endif

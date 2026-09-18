#ifndef JG_PROJECTION_HPP
#define JG_PROJECTION_HPP

#include <JSystem/JGeometry/JGPosition3.hpp>

namespace JGeometry {

// This class is from SMG. Inheritance is a wild guess on my part,
// but it makes stuff match and kind of makes sense conceptually?
// A projective transform (TProjection) IS an affine transform (TPosition)
// IS alinear transform (TRotation).
template <class T> class TProjection3 : public TPosition3<T> {
public:
	TProjection3() { }

	// Argument order and the trailing `+ n` are read off the one caller
	// (TBathWaterMeshRenderer::prerender): arguments evaluate right to
	// left and the ROM computes the height pair first, so the height pair
	// is the later one; the two `fmadds` add the near plane to both
	// translation terms (dead for every known caller, which passes 0.0f).
	// The chained zero stores run [3][c] first, so the chains read
	// low row -> high row.
	void orthographic(f32 l, f32 r, f32 t, f32 b, f32 n, f32 f)
	{
		this->mMtx[0][0] = 2.0f / (r - l);
		this->mMtx[0][3] = -0.5f * this->mMtx[0][0] * (r + l) + n;
		this->mMtx[1][1] = 2.0f / (t - b);
		this->mMtx[1][3] = -0.5f * this->mMtx[1][1] * (t + b) + n;
		this->mMtx[2][2] = -1.0f / (f - n);
		this->mMtx[2][3] = this->mMtx[2][2] * f;
		this->mMtx[0][2] = this->mMtx[1][2] = this->mMtx[3][2] = 0.0f;
		this->mMtx[0][1] = this->mMtx[2][1] = this->mMtx[3][1] = 0.0f;
		this->mMtx[1][0] = this->mMtx[2][0] = this->mMtx[3][0] = 0.0f;
		this->mMtx[3][3]                                       = 1.0f;
	}
};

} // namespace JGeometry

#endif

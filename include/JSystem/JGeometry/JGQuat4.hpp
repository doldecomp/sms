#ifndef JG_QUAT4_HPP
#define JG_QUAT4_HPP

#include <dolphin/types.h>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <JSystem/JGeometry/JGVec4.hpp>

namespace JGeometry {

template <typename T> struct TQuat4 : public TVec4<T> {
public:
	TQuat4() { }

	TQuat4(T xyz, T _w)
	{
		this->x = xyz;
		this->y = xyz;
		this->z = xyz;
		this->w = _w;
	}

	TQuat4(T _x, T _y, T _z, T _w)
	{
		this->x = _x;
		this->y = _y;
		this->z = _z;
		this->w = _w;
	}

	template <typename A> TQuat4(A _x, A _y, A _z, A _w)
	{
		this->x = _x;
		this->y = _y;
		this->z = _z;
		this->w = _w;
	}

	void conjugate() { this->xyz().negate(); }

	void mul(const TQuat4& other)
	{
		// clang-format off
		T _x = this->w * other.x + this->x * other.w + this->y * other.z - this->z * other.y;
		T _y = this->w * other.y + this->y * other.w + this->z * other.x - this->x * other.z;
		T _z = this->w * other.z + this->z * other.w + this->x * other.y - this->y * other.x;
		T _w = this->w * other.w - this->x * other.x - this->y * other.y - this->z * other.z;
		// clang-format on

		set(_x, _y, _z, _w);
	}

	void mul(const TQuat4& a, const TQuat4& b)
	{
		// clang-format off
		T _x = a.x * b.w + a.w * b.x + a.y * b.z - a.z * b.y;
		T _y = a.y * b.w + a.w * b.y + a.z * b.x - a.x * b.z;
		T _z = a.z * b.w + a.w * b.z + a.x * b.y - a.y * b.x;
		T _w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
		// clang-format on

		set(_x, _y, _z, _w);
	}

	// NOTE: SMG contains a "normalize" implementation here that reset the
	// quaternion to identity in case of zero length -- welp, not in SMS! Here
	// we just leave it all to TVec4 and zero out the quaternion to divide by
	// zero even more! Fun!

	void getXDir(TVec3<T>& rDest) const
	{
		f32 _x = 1.0f - this->y * this->y * 2.0f - this->z * this->z * 2.0f;
		f32 _y = this->x * this->y * 2.0f + this->w * this->z * 2.0f;
		f32 _z = this->x * this->z * 2.0f - this->w * this->y * 2.0f;
		rDest.set(_x, _y, _z);
	}

	void getYDir(TVec3<T>& rDest) const
	{
		f32 _x = this->x * this->y * 2.0f - this->w * this->z * 2.0f;
		f32 _y = 1.0f - this->x * this->x * 2.0f - this->z * this->z * 2.0f;
		f32 _z = this->y * this->z * 2.0f + this->w * this->x * 2.0f;
		rDest.set(_x, _y, _z);
	}

	void getZDir(TVec3<T>& rDest) const
	{
		f32 _x = this->x * this->z * 2.0f + this->w * this->y * 2.0f;
		f32 _y = this->y * this->z * 2.0f - this->w * this->x * 2.0f;
		f32 _z = 1.0f - this->x * this->x * 2.0f - this->y * this->y * 2.0f;
		rDest.set(_x, _y, _z);
	}

	void getEuler(TVec3<T>& rDest) const;
	void setEuler(T _x, T _y, T _z);
	void setEulerX(T _x)
	{
		f32 s   = sin(0.5f * _x);
		f32 c   = cos(0.5f * _x);
		this->x = s;
		this->y = 0.0f;
		this->z = 0.0f;
		this->w = c;
	}
	void setEulerY(T _y)
	{
		f32 s   = sin(0.5f * _y);
		f32 c   = cos(0.5f * _y);
		this->x = 0.0f;
		this->y = s;
		this->z = 0.0f;
		this->w = c;
	}
	void setEulerZ(T _z)
	{
		f32 s   = sin(0.5f * _z);
		f32 c   = cos(0.5f * _z);
		this->x = 0.0f;
		this->y = 0.0f;
		this->z = s;
		this->w = c;
	}

	void setRotate(const TVec3<T>& pVec, f32 pAngle)
	{
		this->xyz().scale(sinf(pAngle * 0.5f), pVec);
		this->w = cosf(pAngle * 0.5f);
	}

	// Matches the weak copy in BeeHive.o. The cross product is three named
	// scalars stored one at a time into the axis, not axis.cross(): that is
	// what puts cross.x/y/z in f30/f29/f28 and the length in f31. Writing the
	// components through axis.set() or the TVec3(x, y, z) constructor also
	// matches BeeHive's copy but costs TKukku::calcRootMatrix an 8-byte frame
	// temporary.
	void setRotate(const TVec3<T>& from, const TVec3<T>& to, T amount)
	{
		f32 cx = from.y * to.z - from.z * to.y;
		f32 cy = from.z * to.x - from.x * to.z;
		f32 cz = from.x * to.y - from.y * to.x;
		TVec3<T> axis;
		axis.x = cx;
		axis.y = cy;
		axis.z = cz;

		f32 len = axis.length();
		if (len <= TUtil<f32>::epsilon()) {
			this->set(0.0f, 0.0f, 0.0f, 1.0f);
			return;
		}

		f32 halfAngle = 0.5f * atan2(len, from.dot(to)) * amount;
		this->xyz().scale(sin(halfAngle) / len, axis);
		this->w = cos(halfAngle);
	}

	void setRotate(const TVec3<T>& a, const TVec3<T>& b)
	{
		setRotate(a, b, 1.0f);
	}

	// Assumes unit quaternion. These were renamed to "transform" in SMG.
	//
	// The body below reproduces the retail instruction stream exactly: the map
	// emits this function out of line and weak in Kumokun.cpp (0x80117d08, size
	// 0x98) and that copy is a leaf with *no stack frame at all*, which our
	// build matches instruction-for-instruction (only float register numbering
	// differs, hence the "Incollect regalloc" note below).
	//
	// TODO: the local set is still wrong, and it is wrong in the *inlined*
	// direction. Every inline site is too big, because an inlined callee's
	// locals keep their stack homes even when the values live in registers:
	//
	//   caller                          target  ours   sites
	//   TYumbo::shotSeeds               0x158   0x1a0    2
	//   TBathtubKiller::makeQuat        0x1e0   0x218    ?
	//   TCoasterEnemy::moveCoaster      0x208   0x258    ?
	//   TAnimalBase::execWalk            0xf0   0x118    1
	//   TKumokun::rotateGoalDirToLocal    0x60    0x88    1
	//   TFireWanwan::bindBody           0x1e8   0x1f8    1
	//   TBathtubData::getGravityDir       0xd0    0xd8    1
	//
	// Measured cost of the locals per expansion: the two TQuat4 temporaries are
	// worth 32 bytes, vx/vy/vz only 4 (just one of the three gets a home), and
	// w/z/y/x none. Dropping both temporaries (q/q2 written as seven f32 locals,
	// rDest.set(rx, ry, rz)) makes shotSeeds' frame *exactly* 0x158 and takes it
	// 95.69% -> 96.67%, and cuts the summed frame error of the table above from
	// 312 to 136 bytes -- but it renumbers the float registers at the other six
	// inline sites, so changes_all reports 1 improvement against 6 regressions
	// (Kumokun's out-of-line copy 90.39 -> 84.21, makeQuat 90.54 -> 87.73,
	// getGravityDir 82.76 -> 82.37, moveCoaster 82.42 -> 82.22, bindBody
	// 99.40 -> 99.26, rotateGoalDirToLocal 93.59 -> 93.47). Left in place for
	// that reason. Note the remaining gaps are not a common multiple of any
	// object count, so several of those callers have frame problems of their own
	// and cannot be used as evidence here.
	//
	// Ruled out: reading this->x/y/z/w directly instead of through the four
	// locals (shotSeeds 95.69 -> 90.6, Kumokun's copy 90.39 -> 50.3), and
	// dropping only vx/vy/vz (frame moves 8 bytes, Kumokun's copy -> 83.9).
	//
	// The residual shotSeeds difference is not the frame: retail merges the
	// axis.z load inside the inlined MsGetRotFromZaxisY with this function's
	// v.z read and parks it in f31 across matan/sinf/cosf, while we load v.z
	// again afterwards. See the "const on an inline's pointer parameter also
	// defeats CSE" rule in docs/AGENT_MATCHING_TIPS.md -- the suspect is
	// MsGetRotFromZaxisY's const reference parameter, not this header.
	void rotate(const TVec3<T>& v, TVec3<T>& rDest) const
	{
		// Incollect regalloc
		f32 vx = v.x;
		f32 vy = v.y;
		f32 vz = v.z;

		T w = this->w;
		T z = this->z;
		T y = this->y;
		T x = this->x;

		// clang-format off
		TQuat4 q;
		q.x =  w *  0 + y * vz - z * vy + w * vx;
		q.y = -x * vz + y *  0 + z * vx + w * vy;
		q.z =  x * vy - y * vx + z *  0 + w * vz;
		q.w = -x * vx - y * vy - z * vz + w *  0;

		TQuat4 q2;
		q2.x =  q.x *  w + q.y * -z - q.z * -y + q.w * -x;
		q2.y = -q.x * -z + q.y *  w + q.z * -x + q.w * -y;
		q2.z =  q.x * -y - q.y * -x + q.z *  w + q.w * -z;
		// clang-format on

		// This set wasn't inlined in SMG, so should be real?
		rDest.set(q2.x, q2.y, q2.z);
	}

	void rotate(TVec3<T>& rDest) const { rotate(rDest, rDest); }

	void slerp(const TQuat4<T>& a1, const TQuat4<T>& a2, T a3)
	{
		this->x = a1.x;
		this->y = a1.y;
		this->z = a1.z;
		this->w = a1.w;
		this->slerp(a2, a3);
	}

	// TODO: 99.8% against the weak copy in fireWanwan.o (0x248), and it is a
	// pure frame gap: all 146 instructions match and the frame is 0xa8 against
	// retail's 0xb8. A temporary `volatile char trash[16]` takes it to exactly
	// 100.0%, so the body below is right and retail declared one more 16-byte
	// aggregate -- a third TQuat4 -- that nothing in the code needs. Ruled out:
	// `TQuat4<f32> q1 = *this; q1.normalize();` and the ctor-parenthesis form
	// both move the frame the wrong way (0x68) and drop it to 35.4%, because
	// normalize() then reads the quaternion back out of memory. The byte count
	// is the only evidence for the missing local, so per
	// docs/catalog/frame-gaps.md it stays unwritten.
	void slerp(const TQuat4<T>& param_1, T param_2)
	{
		TQuat4<f32> q1;
		q1.normalize(*this);
		TQuat4<f32> q2;
		q2.normalize(param_1);

		f32 fVar13 = q1.dot(q2);

		bool bVar9;
		if (fVar13 < 0.0f) {
			bVar9  = true;
			fVar13 = -fVar13;
		} else {
			bVar9 = false;
		}

		f32 fVar92;
		if (1.0f - fVar13 <= TUtil<f32>::epsilon()) {
			fVar92 = 1.0f - param_2;
		} else {
			f32 fVar11 = acosf(fVar13);
			f32 fVar12 = sinf(fVar11);

			fVar92  = sinf((1.0f - param_2) * fVar11) / fVar12;
			param_2 = sinf(param_2 * fVar11) / fVar12;
		}

		if (bVar9)
			param_2 = -param_2;

		this->x = fVar92 * q1.x + param_2 * q2.x;
		this->y = fVar92 * q1.y + param_2 * q2.y;
		this->z = fVar92 * q1.z + param_2 * q2.z;
		this->w = fVar92 * q1.w + param_2 * q2.w;
	}
};

} // namespace JGeometry

#endif

#ifndef JG_ROTATION3_HPP
#define JG_ROTATION3_HPP

#include <dolphin/types.h>
#include <algorithm>
#include <JSystem/JMath.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <JSystem/JGeometry/JGQuat4.hpp>
#include <JSystem/JGeometry/JGMatrix34.hpp>

namespace JGeometry {

template <class T> inline T max(T a, T b) { return a >= b ? a : b; }

template <class T> class TRotation3 : public T {
public:
	TRotation3() { }

	void identity33()
	{
		this->ref(0, 0) = 1.0f;
		this->ref(1, 0) = 0.0f;
		this->ref(2, 0) = 0.0f;

		this->ref(0, 1) = 0.0f;
		this->ref(1, 1) = 1.0f;
		this->ref(2, 1) = 0.0f;

		this->ref(0, 2) = 0.0f;
		this->ref(1, 2) = 0.0f;
		this->ref(2, 2) = 1.0f;
	}

	void setRotate(const JGeometry::TVec3<f32>& param_1, f32 param_2)
	{
		TVec3<f32> f27f28f29;
		f27f28f29.normalize(param_1);
		f32 f30 = sin(param_2);
		f32 f1  = cos(param_2);
		f32 f11 = 1.0f - f1;

		f32 f2    = f27f28f29.x * f27f28f29.x;
		f32 f3    = f27f28f29.y * f27f28f29.y;
		f32 f0    = f27f28f29.z * f27f28f29.z;
		f32 f6    = f30 * f27f28f29.x;
		f32 f9    = f30 * f27f28f29.y;
		f32 f7    = f30 * f27f28f29.z;
		f32 fVar6 = f27f28f29.y * (f11 * f27f28f29.x);
		f32 fVar5 = f27f28f29.z * (f11 * f27f28f29.y);
		f32 fVar4 = f27f28f29.z * (f11 * f27f28f29.x);

		// TODO: regswaps + stack size
		this->ref(0, 0) = f11 * f2 + f1;
		this->ref(0, 1) = fVar6 - f7;
		this->ref(0, 2) = fVar4 + f9;

		this->ref(1, 0) = fVar6 + f7;
		this->ref(1, 1) = f11 * f3 + f1;
		this->ref(1, 2) = fVar5 - f6;

		this->ref(2, 0) = fVar4 - f9;
		this->ref(2, 1) = fVar5 + f6;
		this->ref(2, 2) = f11 * f0 + f1;
	}

	void setEular(s16 yaw, s16 pitch, s16 roll)
	{
		f32 f3 = JMASSin(yaw);
		f32 f5 = JMASSin(pitch);
		f32 f4 = JMASSin(roll);

		f32 f6 = JMASCos(yaw);
		f32 f7 = JMASCos(pitch);
		f32 f8 = JMASCos(roll);

		f32 f9  = f6 * f8;
		f32 f10 = f3 * f5;
		f32 f11 = f6 * f4;

		setXDir(f7 * f8, f7 * f4, -f5);
		setYDir(f10 * f8 - f11, f10 * f4 + f9, f3 * f7);
		setZDir(f9 * f5 + f3 * f4, f11 * f5 - f3 * f8, f6 * f7);
	}

	void setEular(f32 yaw, f32 pitch, f32 roll)
	{
		f32 f3 = sin(yaw);
		f32 f5 = sin(pitch);
		f32 f4 = sin(roll);
		f32 f6 = cos(yaw);
		f32 f7 = cos(pitch);
		f32 f8 = cos(roll);

		f32 f9  = f6 * f8;
		f32 f10 = f3 * f5;
		f32 f11 = f6 * f4;

		setXDir(f7 * f8, f7 * f4, -f5);
		setYDir(f10 * f8 - f11, f10 * f4 + f9, f3 * f7);
		setZDir(f9 * f5 + f3 * f4, f11 * f5 - f3 * f8, f6 * f7);
	}
	void setLookDir(const JGeometry::TVec3<f32>& param_1,
	                const JGeometry::TVec3<f32>& param_2)
	{
		// TODO: regswaps + stack size. "Orthogonalize" inline?
		TVec3<f32> fVar958;
		TVec3<f32> fVar1076;
		TVec3<f32> fVar432;
		fVar432.normalize(param_1);
		fVar432.negate();
		fVar958.cross(param_2, fVar432);
		fVar1076.cross(fVar958, fVar432);
		fVar958.normalize();
		fVar1076.normalize();

		this->ref(0, 0) = fVar958.x;
		this->ref(0, 1) = fVar958.y;
		this->ref(0, 2) = fVar958.z;

		this->ref(1, 0) = fVar1076.x;
		this->ref(1, 1) = fVar1076.y;
		this->ref(1, 2) = fVar1076.z;

		this->ref(2, 0) = fVar432.x;
		this->ref(2, 1) = fVar432.y;
		this->ref(2, 2) = fVar432.z;
	}
	void setQuat(const JGeometry::TQuat4<f32>& qt)
	{
		// f5 = qt.x
		// f3 = qt.y
		// f4 = qt.z
		// f0 = qt.w

		// TODO: regswap >:(
		f32 f2  = 2.0f * qt.x * qt.x;
		f32 f9  = 2.0f * qt.y * qt.y;
		f32 f10 = 2.0f * qt.z * qt.z;

		f32 f6 = 1.0f - f2;
		f32 f0 = 1.0f - f9;

		f32 f11 = 2.0f * qt.x * qt.y;
		f32 f7  = 2.0f * qt.y * qt.z;
		f32 f12 = 2.0f * qt.x * qt.z;

		f32 f5  = 2.0f * qt.w * qt.x;
		f32 f3  = 2.0f * qt.w * qt.y;
		f32 f13 = 2.0f * qt.w * qt.z;

		this->ref(0, 0) = f0 - f10;
		this->ref(0, 1) = f11 - f13;
		this->ref(0, 2) = f12 + f3;

		this->ref(1, 0) = f11 + f13;
		this->ref(1, 1) = f6 - f10;
		this->ref(1, 2) = f7 - f5;

		this->ref(2, 0) = f12 - f3;
		this->ref(2, 1) = f7 + f5;
		this->ref(2, 2) = f6 - f9;
	}

	void getQuat(JGeometry::TQuat4<f32>& quat) const
	{
		// TODO: nasty regswap
		if (this->at(0, 0) + this->at(1, 1) + this->at(2, 2) >= 0.0f) {
			f32 scale = TUtil<f32>::sqrt(this->at(0, 0) + this->at(1, 1)
			                             + this->at(2, 2) + 1.0f);
			quat.w    = 0.5f * scale;
			quat.x    = 0.5f / scale * (this->at(2, 1) - this->at(1, 2));
			quat.y    = 0.5f / scale * (this->at(0, 2) - this->at(2, 0));
			quat.z    = 0.5f / scale * (this->at(1, 0) - this->at(0, 1));
			return;
		}

		f32 maxDiag = max(max(this->at(0, 0), this->at(1, 1)), this->at(2, 2));

		if (maxDiag == this->at(0, 0)) {
			f32 scale = TUtil<f32>::sqrt(
			    this->at(0, 0) - (this->at(1, 1) + this->at(2, 2)) + 1.0f);
			quat.x = 0.5f * scale;
			quat.y = 0.5f / scale * (this->at(0, 1) + this->at(1, 0));
			quat.z = 0.5f / scale * (this->at(2, 0) + this->at(0, 2));
			quat.w = 0.5f / scale * (this->at(2, 1) - this->at(1, 2));
			return;
		}

		if (maxDiag == this->at(1, 1)) {
			f32 scale = TUtil<f32>::sqrt(
			    this->at(1, 1) - (this->at(2, 2) + this->at(0, 0)) + 1.0f);
			quat.y = 0.5f * scale;
			quat.z = 0.5f / scale * (this->at(1, 2) + this->at(2, 1));
			quat.x = 0.5f / scale * (this->at(0, 1) + this->at(1, 0));
			quat.w = 0.5f / scale * (this->at(0, 2) - this->at(2, 0));
			return;
		}

		f32 scale = TUtil<f32>::sqrt(
		    this->at(2, 2) - (this->at(0, 0) + this->at(1, 1)) + 1.0f);
		quat.z = 0.5f * scale;
		quat.x = 0.5f / scale * (this->at(2, 0) + this->at(0, 2));
		quat.y = 0.5f / scale * (this->at(1, 2) + this->at(2, 1));
		quat.w = 0.5f / scale * (this->at(1, 0) - this->at(0, 1));
	}

	void setSQ(const JGeometry::TVec3<f32>& scale,
	           const JGeometry::TQuat4<f32>& qt)
	{
		f32 f2  = 2.0f * qt.x * qt.x;
		f32 f9  = 2.0f * qt.y * qt.y;
		f32 f10 = 2.0f * qt.z * qt.z;

		f32 f6 = 1.0f - f2;
		f32 f0 = 1.0f - f9;

		f32 f11 = 2.0f * qt.x * qt.y;
		f32 f7  = 2.0f * qt.y * qt.z;
		f32 f12 = 2.0f * qt.x * qt.z;

		f32 f5  = 2.0f * qt.w * qt.x;
		f32 f3  = 2.0f * qt.w * qt.y;
		f32 f13 = 2.0f * qt.w * qt.z;

		this->ref(0, 0) = scale.x * (f0 - f10);
		this->ref(0, 1) = scale.x * (f11 - f13);
		this->ref(0, 2) = scale.x * (f12 + f3);

		this->ref(1, 0) = scale.y * (f11 + f13);
		this->ref(1, 1) = scale.y * (f6 - f10);
		this->ref(1, 2) = scale.y * (f7 - f5);

		this->ref(2, 0) = scale.z * (f12 - f3);
		this->ref(2, 1) = scale.z * (f7 + f5);
		this->ref(2, 2) = scale.z * (f6 - f9);
	}

	// From SMG
	void setXDir(const TVec3<f32>& param_1)
	{
		this->ref(0, 0) = param_1.x;
		this->ref(1, 0) = param_1.y;
		this->ref(2, 0) = param_1.z;
	}
	void setXDir(f32 x, f32 y, f32 z)
	{
		this->ref(0, 0) = x;
		this->ref(1, 0) = y;
		this->ref(2, 0) = z;
	}
	void getXDir(JGeometry::TVec3<f32>& param_1) const
	{
		param_1.set(this->at(0, 0), this->at(1, 0), this->at(2, 0));
	}
	void setYDir(const TVec3<f32>& param_1)
	{
		this->ref(0, 1) = param_1.x;
		this->ref(1, 1) = param_1.y;
		this->ref(2, 1) = param_1.z;
	}
	void setYDir(f32 x, f32 y, f32 z)
	{
		this->ref(0, 1) = x;
		this->ref(1, 1) = y;
		this->ref(2, 1) = z;
	}
	void getYDir(JGeometry::TVec3<f32>& param_1) const
	{
		param_1.set(this->at(0, 1), this->at(1, 1), this->at(2, 1));
	}
	void setZDir(const TVec3<f32>& param_1)
	{
		this->ref(0, 2) = param_1.x;
		this->ref(1, 2) = param_1.y;
		this->ref(2, 2) = param_1.z;
	}
	void setZDir(f32 x, f32 y, f32 z)
	{
		this->ref(0, 2) = x;
		this->ref(1, 2) = y;
		this->ref(2, 2) = z;
	}
	void getZDir(JGeometry::TVec3<f32>& param_1) const
	{
		param_1.set(this->at(0, 2), this->at(1, 2), this->at(2, 2));
	}
	void setXYZDir(const TVec3<f32>& param_1, const TVec3<f32>& param_2,
	               const TVec3<f32>& param_3)

	{
		setXDir(param_1);
		setYDir(param_2);
		setZDir(param_3);
	}

	void setEularX(float param_1)
	{
		f32 s = sin(param_1);
		f32 c = cos(param_1);

		this->ref(0, 0) = 1.0f;
		this->ref(0, 1) = 0.0f;
		this->ref(0, 2) = 0.0f;

		this->ref(1, 0) = 0.0f;
		this->ref(1, 1) = c;
		this->ref(1, 2) = s;

		this->ref(2, 0) = 0.0f;
		this->ref(2, 1) = -s;
		this->ref(2, 2) = c;
	}

	void setEularY(float param_1)
	{
		f32 s = sin(param_1);
		f32 c = cos(param_1);

		this->ref(0, 0) = c;
		this->ref(0, 1) = 0.0f;
		this->ref(0, 2) = s;

		this->ref(1, 0) = 0.0f;
		this->ref(1, 1) = 1.0f;
		this->ref(1, 2) = 0.0f;

		this->ref(2, 0) = -s;
		this->ref(2, 1) = 0.0f;
		this->ref(2, 2) = c;
	}

	void setEularZ(float param_1)
	{

		f32 s = sin(param_1);
		f32 c = cos(param_1);

		this->ref(0, 0) = c;
		this->ref(0, 1) = -s;
		this->ref(0, 2) = 0.0f;

		this->ref(1, 0) = s;
		this->ref(1, 1) = c;
		this->ref(1, 2) = 0.0f;

		this->ref(2, 0) = 0.0f;
		this->ref(2, 1) = 0.0f;
		this->ref(2, 2) = 1.0f;
	}

	void mult33(const TVec3<f32>& param_1, TVec3<f32>& param_2) const
	{
		param_2.set(
		    // clang-format off
		this->at(0, 0) * param_1.x + this->at(0, 1) * param_1.y + this->at(0, 2) * param_1.z,
		this->at(1, 0) * param_1.x + this->at(1, 1) * param_1.y + this->at(1, 2) * param_1.z,
		this->at(2, 0) * param_1.x + this->at(2, 1) * param_1.y + this->at(2, 2) * param_1.z
		    // clang-format on
		);
	}

	void mult33(TVec3<f32>& param_1) const
	{
		param_1.set(
		    // clang-format off
		this->at(0, 0) * param_1.x + this->at(0, 1) * param_1.y + this->at(0, 2) * param_1.z,
		this->at(1, 0) * param_1.x + this->at(1, 1) * param_1.y + this->at(1, 2) * param_1.z,
		this->at(2, 0) * param_1.x + this->at(2, 1) * param_1.y + this->at(2, 2) * param_1.z
		    // clang-format on
		);
	}

	void setScale(f32 param_1, f32 param_2, f32 param_3)
	{
		this->ref(0, 0) = param_1;
		this->ref(0, 1) = 0.0f;
		this->ref(0, 2) = 0.0f;

		this->ref(1, 0) = 0.0f;
		this->ref(1, 1) = param_2;
		this->ref(1, 2) = 0.0f;

		this->ref(2, 0) = 0.0f;
		this->ref(2, 1) = 0.0f;
		this->ref(2, 2) = param_3;
	}
};

} // namespace JGeometry

#endif

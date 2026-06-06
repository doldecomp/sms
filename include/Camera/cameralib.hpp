#ifndef CAMERA_CAMERALIB_HPP
#define CAMERA_CAMERALIB_HPP

#include <System/Resolution.hpp>
#include <JSystem/JGeometry.hpp>

extern f32 SMSGetAnmFrameRate(); // avoid including Application.hpp

template <class T>
void CLBChaseConstantSpecifyFrame(T* param_1, T param_2, T param_3)
{
	if (param_3 < 0.001f) {
		*param_1 = param_2;
		return;
	}

	*param_1 = (1.0f / param_3) * (param_2 - *param_1) + *param_1;
}

template <class T>
BOOL CLBChaseGeneralConstantSpecifySpeed(T* param_1, T param_2, T param_3)
{
	T sVar1 = param_2 - *param_1;
	if (param_3 < 0)
		param_3 = -param_3;

	if (sVar1 > 0) {
		sVar1 -= param_3;
		if (sVar1 > 0)
			*param_1 = param_2 - sVar1;
		else
			*param_1 = param_2;
	} else {
		sVar1 += param_3;
		if (sVar1 < 0)
			*param_1 = param_2 - sVar1;
		else
			*param_1 = param_2;
	}

	if (*param_1 == param_2)
		return false;

	return true;
}

template <class T> T CLBEaseInInbetween(T param_1, T param_2, f32 param_3)
{
	return CLBTwoDegreeGeneralInbetween(param_1, param_2, param_3,
	                                    param_2 - param_1);
}

template <class T> T CLBLinearInbetween(T a, T b, f32 f)
{
	return (T)(a + f * (b - a));
}

template <class T> T CLBRoundf(f32 param_1)
{
	return (T)(param_1 + (param_1 > 0.0f ? 0.5f : -0.5f));
}

template <class T> T CLBPalFrame(T param_1)
{
	f32 rate = SMSGetAnmFrameRate();
	return CLBRoundf<T>(param_1 * (1.0f / rate));
}

template <class T> T CLBPalIntSpeed(T param_1)
{
	return CLBRoundf<T>(param_1 * SMSGetAnmFrameRate());
}

template <class T>
T CLBTwoDegreeGeneralInbetween(T param_1, T param_2, f32 param_3, f32 param_4)
{
	return (T)(param_4 * param_3 * param_3
	           + param_3 * ((param_2 - param_1) - param_4) + param_1);
}

template <class T> T CLBEaseOutInbetween(T param_1, T param_2, f32 param_3)
{
	return CLBTwoDegreeGeneralInbetween<T>(param_1, param_2, param_3,
	                                       param_1 - param_2);
}

template <class T> f32 CLBCalcRatio(T a, T b, T c)
{
	f32 result = 0.0f;
	if (a != b) {
		result = (f32)(c - a) * (1.0f / (f32)(b - a));
	}
	return result;
}

template <class T> T CLBSquared(T v) { return v * v; }

// fabricated
inline s16 CLBDegToShortAngle(f32 deg)
{
	return CLBRoundf<s16>(deg * (65536.0f / 360.0f));
}

// NOTE: can't use MtxPtr here, maybe need new ConstMtxPtr typedef here?
void CLBCalc2DFPos(JGeometry::TVec2<f32>*, const f32 (*)[4], const f32 (*)[4],
                   const Vec&, u32*, bool);

void CLBCalcNearClipAngle(JGeometry::TVec3<f32>*, S16Vec*,
                          const JGeometry::TVec3<f32>&,
                          const JGeometry::TVec3<f32>&, s16, f32);

void CLBCalcNearFourPos(JGeometry::TVec3<f32>*, JGeometry::TVec3<f32>*, S16Vec*,
                        const JGeometry::TVec3<f32>&,
                        const JGeometry::TVec3<f32>&, s16, f32,
                        const JGeometry::TVec2<f32>&);

/**
 * @brief Computes world-space positions of a 3x3 uniform grid of 9 points
 * that lie on the near plane.
 *
 * @param out_grid positions of the 3x3 grid points.
 * @param out_euler Euler angles of the camera.
 * @param origin camera origin.
 * @param lookat camera lookat point.
 * @param roll camera roll angle.
 * @param near_dist distance to the near plane.
 * @param near_dims dimensions of the near plane.
 */
void CLBCalcNearNinePos(JGeometry::TVec3<f32>*, S16Vec*,
                        const JGeometry::TVec3<f32>&,
                        const JGeometry::TVec3<f32>&, s16, f32,
                        const JGeometry::TVec2<f32>&);

// fabricated
inline f32 fakeTan(s16 angle)
{
	return JMASSin(angle) * (1.0f / JMASCos(angle));
}

// Fabricated
inline void CLBCalcNearNinePos(JGeometry::TVec3<f32>* out_grid,
                               S16Vec* out_euler,
                               const JGeometry::TVec3<f32>& origin,
                               const JGeometry::TVec3<f32>& lookat, s16 roll,
                               f32 near_dist, f32 fovy, f32 aspect)
{
	s16 halfFovyShort = CLBDegToShortAngle(0.5f * fovy);
	f32 halfTan       = fakeTan(halfFovyShort);
	JGeometry::TVec2<f32> nearSize;
	nearSize.y = 2.0f * (near_dist * halfTan);
	nearSize.x = nearSize.y * aspect;

	CLBCalcNearNinePos(out_grid, out_euler, origin, lookat, roll, near_dist,
	                   nearSize);
}

void CLBCalcPointInCubeRatio(const Vec&, const Vec&, const Vec&, const Vec&,
                             f32*, f32*, f32*);

/**
 * @brief Creates a rotate-and-translate matrix.
 *
 * @details
 * Rotations are applied in ZXY order.
 *
 * @param mtx the destination matrix
 * @param rotate rotation angles in degrees
 * @param translate translation values
 */
void CLBCalcRotateZXYTranslateMatrix(MtxPtr mtx, const Vec& rotate,
                                     const Vec& translate);

/**
 * @brief Creates a scale-and-translate matrix.
 *
 * @param mtx the destination matrix
 * @param scale scale factors
 * @param translate translation values
 */
void CLBCalcScaleTranslateMatrix(MtxPtr mtx, const Vec& scale,
                                 const Vec& translate);

/**
 * @brief Moves a SHORTANGLE towards another SHORTANGLE by a specified ratio.
 *
 * @param out the value to be modified
 * @param target the target value to approach
 * @param invSpeed a constant inversely proportional to the rate of change
 * @return whether another iteration may refine the angle even further
 */
BOOL CLBChaseAngleDecrease(s16* out, s16 target, s16 invSpeed);

/**
 * @brief Moves dstValue toward targetValue by a fraction defined by ratio.
 *
 * @param dstValue the value to be modified
 * @param targetValue the target value to approach
 * @param ratio the ratio by which dstValue approaches targetValue
 * @param threshold a threshold for if we're close enough
 * @return whether dstValue is still outside the threshold of targetValue
 */
BOOL CLBChaseDecrease(f32* dstValue, f32 targetValue, f32 ratio, f32 threshold);

// Fabricated overload
inline void CLBChaseDecrease(Vec* dstValue, const Vec& targetValue, f32 ratio,
                             f32 threshold)
{
	CLBChaseDecrease(&dstValue->x, targetValue.x, ratio, threshold);
	CLBChaseDecrease(&dstValue->y, targetValue.y, ratio, threshold);
	CLBChaseDecrease(&dstValue->z, targetValue.z, ratio, threshold);
}

bool CLBChaseSpecialDecrease(f32*, f32, f32, f32);

/**
 * @brief Converts Cartesian coordinates to spherical coordinates.
 *
 * @param origin the point to use as the origin
 * @param in the input vector
 * @param outRadius the output radius
 * @param outVAngle the output vertical angle
 * @param outHAngle the output horizontal angle
 */
void CLBCrossToPolar(const Vec& origin, const Vec& in, f32* outRadius,
                     s16* outVAngle, s16* outHAngle);

bool CLBIsPointInCube(const Vec&, const Vec&, const Vec&, const Vec&);

/**
 * @brief Converts spherical coordinates to Cartesian coordinates.
 *
 * @param origin the point to use as the origin
 * @param out the output vector
 * @param radius the radius of the point
 * @param vAngle the vertical angle from the xz-plane to the y-axis
 * @param hAngle the horizontal angle in the xz-plane, clockwise from the z-axis
 */
void CLBPolarToCross(const Vec& origin, Vec* out, f32 radius, s16 vAngle,
                     s16 hAngle);

/**
 * @brief Clamps a vector within a specified vertical angle range.
 *
 * @param vAngleMin the minimum vertical angle
 * @param vAngleMax the maximum vertical angle
 * @param origin the point to use as the origin
 * @param inOut the input and output vector
 */
void CLBRevisionLookatByAngleX(s16 vAngleMin, s16 vAngleMax, const Vec& origin,
                               Vec* inOut);

void CLBRotatePosAndUp(s16, s16, const JGeometry::TVec3<f32>&,
                       const JGeometry::TVec3<f32>&,
                       const JGeometry::TVec3<f32>&, JGeometry::TVec3<f32>*,
                       JGeometry::TVec3<f32>*);

inline void CLBScreenFPosToSPos(JGeometry::TVec2<s16>* out,
                                const JGeometry::TVec2<f32>& in)
{
	f32 x = in.x;
	// TODO: definitely more inlines but I couldn't get it to work out...
	if (x < -1.0f || 1.0f < x)
		out->x = -1;
	else
		out->x = CLBRoundf<s16>((1.0f + x)
		                        * (0.5f * (f32)(SMSGetGameRenderWidth() - 1)));

	f32 y = in.y;
	if (y < -1.0f || 1.0f < y)
		out->y = -1;
	else
		out->y = CLBRoundf<s16>(
		    (y - 1.0f) * (-0.5f * (f32)(SMSGetGameRenderHeight() - 1)));
}

#endif

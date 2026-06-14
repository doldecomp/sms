#ifndef CAMERA_CAMERALIB_HPP
#define CAMERA_CAMERALIB_HPP

#include <MarioUtil/MathUtil.hpp>
#include <JSystem/JGeometry.hpp>

extern f32 SMSGetAnmFrameRate(); // avoid including Application.hpp

extern const JGeometry::TVec3<f32> CLBConstUpVec;

template <class T> T CLBRoundf(f32 param_1)
{
	return (T)(param_1 + (param_1 > 0.0f ? 0.5f : -0.5f));
}

/**
 * @brief Chases @p value towards @p desired over @p frames calls.
 *
 * @tparam T numeric type
 * @param value the value to mutate
 * @param desired value to chase towards
 * @param frames amount of calls to this function over which to chase
 */
template <class T>
void CLBChaseConstantSpecifyFrame(T* value, T desired, T frames)
{
	if (frames < 0.001f) {
		*value = desired;
		return;
	}

	f32 delta = desired - *value;
	*value += (1.0f / frames) * delta;
}

// TODO: should the two overloads be unified?
inline void CLBChaseConstantSpecifyFrame(s16* value, s16 desired, f32 frames)
{
	if (frames < 0.001f) {
		*value = desired;
		return;
	}

	s16 delta = desired - *value;
	*value += CLBRoundf<s16>(1.0f / frames * delta);
}

/**
 * @brief Linearly chases a numeric @p value towards a @p desired quantity.
 *
 * @tparam T numeric type
 * @param value value to mutate
 * @param desired the desired value
 * @param speed speed of chase, value is decremented or incremented by this
 * @return BOOL whether more calls are required to achieve the desired value
 */
template <class T>
BOOL CLBChaseGeneralConstantSpecifySpeed(T* value, T desired, T speed)
{
	T delta = desired - *value;
	if (speed < 0)
		speed = -speed;

	if (delta > 0) {
		delta -= speed;
		if (delta > 0)
			*value = desired - delta;
		else
			*value = desired;
	} else {
		delta += speed;
		if (delta < 0)
			*value = desired - delta;
		else
			*value = desired;
	}

	if (*value == desired)
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

// TODO: should this be a macro actually?
// fabricated
template <class T> inline T CLBAbs(T v) { return v >= 0 ? v : -v; }

// fabricated
inline s16 CLBDegToShortAngle(f32 deg)
{
	return CLBRoundf<s16>(deg * (65536.0f / 360.0f));
}

// NOTE: can't use MtxPtr here, maybe need new ConstMtxPtr typedef here?
/**
 * @brief Converts a world-space position into NDC space.
 *
 * @param out_ndc_pos result ndc coord of @p world_pos
 * @param proj_mtx projection matrix
 * @param view_mtx view matrix
 * @param world_pos input world-space point
 * @param out_depth optional depth output
 * @param disable_z_clip if false, sentinel coords of 10000 will be returned for
 * points who's Z doesn't fit into [0, 0xffffff] range
 */
void CLBCalc2DFPos(JGeometry::TVec2<f32>* out_ndc_pos, const f32 (*proj_mtx)[4],
                   const f32 (*view_mtx)[4], const Vec& world_pos,
                   u32* out_depth, bool disable_z_clip);

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
void CLBCalcNearNinePos(JGeometry::TVec3<f32>* out_grid, S16Vec* out_euler,
                        const JGeometry::TVec3<f32>& origin,
                        const JGeometry::TVec3<f32>& lookat, s16 roll,
                        f32 near_dist, const JGeometry::TVec2<f32>& near_dims);

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
 * @brief Moves a SHORTANGLE @p value towards a @p desired SHORTANGLE
 * by a specified @p ratio.
 *
 * @param value the value to be modified
 * @param desired the target value to chase
 * @param ratio the ratio by which @p value chases @p desired
 * @return whether more calls are needed to reach the target
 */
BOOL CLBChaseAngleDecrease(s16* value, s16 desired, s16 ratio);

/**
 * @brief Moves @p value toward @p desired by a fraction defined by @p ratio.
 *
 * @param value the value to be modified
 * @param desired the target value to chase
 * @param ratio the ratio by which @p value chases @p desired
 * @param threshold a threshold for if we're close enough
 * @return whether @p value is still outside the @p threshold of @p desired
 */
BOOL CLBChaseDecrease(f32* value, f32 desired, f32 ratio, f32 threshold);

// Fabricated overload
inline void CLBChaseDecrease(Vec* dstValue, const Vec& targetValue, f32 ratio,
                             f32 threshold)
{
	CLBChaseDecrease(&dstValue->x, targetValue.x, ratio, threshold);
	CLBChaseDecrease(&dstValue->y, targetValue.y, ratio, threshold);
	CLBChaseDecrease(&dstValue->z, targetValue.z, ratio, threshold);
}

// Fabricated overload
inline void CLBChaseDecrease(Vec* value, const Vec& desired, f32 ratioX,
                             f32 ratioY, f32 ratioZ, f32 threshold)
{
	CLBChaseDecrease(&value->x, desired.x, ratioX, threshold);
	CLBChaseDecrease(&value->y, desired.y, ratioY, threshold);
	CLBChaseDecrease(&value->z, desired.z, ratioZ, threshold);
}

/**
 * @brief Moves @p value toward @p desired at a certain fraction defined by
 * @p ratio, but never slower than a specified @p speed.
 *
 * @param value the value to be modified
 * @param desired the target value to chase
 * @param ratio the ratio by which @p value chases targetValue
 * @param speed the minimum speed at which to chase
 * @return whether more calls are required to reach the desired value
 */
BOOL CLBChaseSpecialDecrease(f32* value, f32 desired, f32 ratio, f32 speed);

/**
 * @brief Converts Cartesian coordinates to spherical coordinates.
 *
 * @param origin the origin point
 * @param in cartesian coordinates of the radius-vector of the input point
 * @param out_radius distance from origin of the point
 * @param out_pitch first polar angle
 * @param out_yaw second polar angle
 */
void CLBCrossToPolar(const Vec& origin, const Vec& in, f32* out_radius,
                     s16* pitch, s16* yaw);

inline void CLBCrossToPolar(const Vec& origin, const Vec& in, s16* out_pitch,
                            s16* out_yaw)
{
	f32 dx = in.x - origin.x;
	f32 dz = in.z - origin.z;

	*out_pitch = matan(MsSqrtf(dx * dx + dz * dz), in.y - origin.y);
	*out_yaw   = matan(in.z - origin.z, in.x - origin.x);
}

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
	// can't include Resolution.hpp because of troubles with MapDraw.cpp
	extern s16 SMSGetGameRenderHeight();
	extern s16 SMSGetGameRenderWidth();

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

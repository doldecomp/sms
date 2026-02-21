#ifndef CAMERA_CAMERALIB_HPP
#define CAMERA_CAMERALIB_HPP

#include <JSystem/JGeometry.hpp>

void CLBCalc2DFPos(JGeometry::TVec2<f32>*, const MtxPtr, const MtxPtr,
                   const Vec&, u32*, bool);
void CLBCalcNearClipAngle(JGeometry::TVec3<f32>*, S16Vec*,
                          const JGeometry::TVec3<f32>&,
                          const JGeometry::TVec3<f32>&, s16, f32);
void CLBCalcNearFourPos(JGeometry::TVec3<f32>*, JGeometry::TVec3<f32>*, S16Vec*,
                        const JGeometry::TVec3<f32>&,
                        const JGeometry::TVec3<f32>&, s16, f32,
                        const JGeometry::TVec2<f32>&);
void CLBCalcNearNinePos(JGeometry::TVec3<f32>*, S16Vec*,
                        const JGeometry::TVec3<f32>&,
                        const JGeometry::TVec3<f32>&, s16, f32,
                        const JGeometry::TVec2<f32>&);
void CLBCalcPointInCubeRatio(const Vec&, const Vec&, const Vec&, const Vec&,
                             f32*, f32*, f32*);
void CLBCalcRotateZXYTranslateMatrix(MtxPtr, const Vec&, const Vec&);
void CLBCalcScaleTranslateMatrix(MtxPtr, const Vec&, const Vec&);
bool CLBChaseAngleDecrease(s16*, s16, s16);
bool CLBChaseDecrease(f32*, f32, f32, f32);
bool CLBChaseSpecialDecrease(f32*, f32, f32, f32);
void CLBCrossToPolar(const Vec&, const Vec&, f32*, s16*, s16*);
bool CLBIsPointInCube(const Vec&, const Vec&, const Vec&, const Vec&);
void CLBPolarToCross(const Vec&, Vec*, f32, s16, s16);
void CLBRevisionLookatByAngleX(s16, s16, const Vec&, Vec*);
void CLBRotatePosAndUp(s16, s16, const JGeometry::TVec3<f32>&,
                       const JGeometry::TVec3<f32>&,
                       const JGeometry::TVec3<f32>&, JGeometry::TVec3<f32>*,
                       JGeometry::TVec3<f32>*);
void CLBScreenFPosToSPos(JGeometry::TVec2<s16>*, const JGeometry::TVec2<f32>&);

template <class T> void CLBChaseConstantSpecifyFrame(T*, T, T);
template <class T>
bool CLBChaseGeneralConstantSpecifySpeed(T* param_1, T param_2, T param_3)
{
	T sVar1 = param_2 - *param_1;
	if (param_3 < 0)
		param_3 = -param_3;

	if (sVar1 > 0) {
		f32 d = sVar1 - param_3;
		if (d > 0)
			*param_1 = param_2 - d;
		else
			*param_1 = param_2;
	} else {
		f32 d = sVar1 + param_3;
		if (d < 0)
			*param_1 = param_2 - d;
		else
			*param_1 = param_2;
	}

	if (*param_1 == param_2)
		return false;

	return true;
}
template <class T> void CLBEaseInInbetween(T, T, T);
template <class T> void CLBEaseOutInbetween(T, T, T);

template <class T> T CLBLinearInbetween(T a, T b, f32 f)
{
	return (T)(a + f * (b - a));
}

template <class T> T CLBPalFrame(T);
template <class T> void CLBPalIntSpeed(T);
template <class T> T CLBRoundf(f32);
template <class T> void CLBTwoDegreeGeneralInbetween(T, T, f32, f32);

template <class T> f32 CLBCalcRatio(T a, T b, T c)
{
	f32 result = 0.0f;
	if (a != b) {
		result = (f32)(c - a) * (1.0f / (f32)(b - a));
	}
	return result;
}

#endif

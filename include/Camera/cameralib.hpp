#ifndef CAMERA_CAMERALIB_HPP
#define CAMERA_CAMERALIB_HPP

#include <JSystem/JGeometry.hpp>

void CLBCalcNearFourPos(JGeometry::TVec3<f32>*, JGeometry::TVec3<f32>*, S16Vec*,
                        const JGeometry::TVec3<f32>&,
                        const JGeometry::TVec3<f32>&, s16, f32,
                        const JGeometry::TVec2<f32>&);
void CLBCalcNearNinePos(JGeometry::TVec3<f32>*, S16Vec*,
                        const JGeometry::TVec3<f32>&,
                        const JGeometry::TVec3<f32>&, s16, f32,
                        const JGeometry::TVec2<f32>&);
void CLBCalcNearClipAngle(JGeometry::TVec3<f32>*, S16Vec*,
                          const JGeometry::TVec3<f32>&,
                          const JGeometry::TVec3<f32>&, s16, f32);
void CLBCalcScaleTranslateMatrix(f32 (*)[4], const Vec&, const Vec&);
void CLBCalcRotateZXYTranslateMatrix(f32 (*)[4], const Vec&, const Vec&);
void CLBCalcPointInCubeRatio(const Vec&, const Vec&, const Vec&, const Vec&,
                             f32*, f32*, f32*);
bool CLBIsPointInCube(const Vec&, const Vec&, const Vec&, const Vec&);
void CLBRotatePosAndUp(s16, s16, const JGeometry::TVec3<f32>&,
                       const JGeometry::TVec3<f32>&,
                       const JGeometry::TVec3<f32>&, JGeometry::TVec3<f32>*,
                       JGeometry::TVec3<f32>*);
void CLBRevisionLookatByAngleX(s16, s16, const Vec&, Vec*);
void CLBPolarToCross(const Vec&, Vec*, f32, s16, s16);
void CLBCrossToPolar(const Vec&, const Vec&, f32*, s16*, s16*);
void CLBChaseSpecialDecrease(f32*, f32, f32, f32);
void CLBChaseDecrease(f32*, f32, f32, f32);
void CLBChaseAngleDecrease(s16*, s16, s16);
void CLBCalc2DFPos(JGeometry::TVec2<f32>*, const f32 (*)[4], const f32 (*)[4],
                   const Vec&, u32*, bool);
template <class T> void CLBLinearInbetween(T, T, f32);

template <class T> f32 CLBCalcRatio(T a, T b, T c)
{
	f32 result = 0.0f;
	if (a != b) {
		result = (f32)(c - a) * (1.0f / (f32)(b - a));
	}
	return result;
}

#endif

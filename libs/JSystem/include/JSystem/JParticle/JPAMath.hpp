#ifndef JPA_MATH_HPP
#define JPA_MATH_HPP

#include <JSystem/JGeometry.hpp>

f32 JPASqrtf(f32);
void JPAGetXYZRotateMtx(s16, s16, s16, MtxPtr);
void JPAGetXYRotateMtx(s16, s16, MtxPtr);
void JPAGetScaleXYRotateMtx(JGeometry::TVec3<f32>&, s16, s16, MtxPtr);
void JPAGetYZRotateMtx(s16, s16, MtxPtr);
void JPAGetScaleYZRotateMtx(JGeometry::TVec3<f32>&, s16, s16, MtxPtr);
void JPAGetYRotateMtx(s16, MtxPtr);
void JPAGetZRotateMtx(s16, MtxPtr);
void JPAVecToRotaMtx(MtxPtr, JGeometry::TVec3<f32>, JGeometry::TVec3<f32>);

f32 JPAConvertFixToFloat(s16);
s16 JPAConvertFloatToFix(f32);

void JPAConvertFixVecToFloatVec(JGeometry::TVec3<f32>&,
                                const JGeometry::TVec3<s16>&);
void JPAConvertFloatVecToFixVec(JGeometry::TVec3<s16>&,
                                const JGeometry::TVec3<f32>&);
void JPABound(JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&,
              const JGeometry::TVec3<f32>&, f32, f32);
void JPAGetSVecElement(MtxPtr, JGeometry::TVec3<f32>&);
void JPAGetRMtxSTVecElement(MtxPtr, MtxPtr, JGeometry::TVec3<f32>&,
                            JGeometry::TVec3<f32>&);
void JPAGetRMtxTVecElement(MtxPtr, MtxPtr, JGeometry::TVec3<f32>&);
void JPAGetRMtxElement(MtxPtr, MtxPtr);
void JPAGetRTMtxElement(MtxPtr, MtxPtr);
void JPARadVecToSVec(JGeometry::TVec3<f32>&, JGeometry::TVec3<s16>&);
f32 JPAGetKeyFrameValue(f32, u16, f32*);

template <class T>
f32 JPAGetKeyFrameInterpolation(T time, u16 frame_num, T* frames)
{
	if (time < frames[0])
		return frames[1];

	if (frames[(frame_num - 1) * 4] <= time)
		return frames[(frame_num - 1) * 4 + 1];

	/* binary search; find keyframe */
	s32 frame = frame_num;
	while (frame > 1) {
		u32 idx = frame / 2;
		if (time >= frames[idx * 4]) {
			frames += idx * 4;
			frame -= idx;
		} else {
			frame = idx;
		}
	}

	return JMAHermiteInterpolation(time, frames[0], frames[1], frames[3],
	                               frames[4], frames[5], frames[6]);
}

#endif

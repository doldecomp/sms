#ifndef MARIO_UTIL_MATH_UTIL_HPP
#define MARIO_UTIL_MATH_UTIL_HPP

#include <JSystem/JGeometry.hpp>

int IConverge(int, int, int, int);
f32 FConverge(f32, f32, f32, f32);
void GetAtanTable(f32, f32);
s16 matan(f32, f32);

void MsGetRotFromZaxis(const JGeometry::TVec3<f32>&);
void MsMtxSetRotRPH(f32 (*)[4], f32, f32, f32);
void MsMtxSetXYZRPH(f32 (*)[4], f32, f32, f32, short, short, short);
void MsMtxSetTRS(f32 (*)[4], f32, f32, f32, f32, f32, f32, f32, f32, f32);
template <class T> void MsWrap(f32, f32, f32);
void MsIsInSight(const JGeometry::TVec3<f32>&, f32,
                 const JGeometry::TVec3<f32>&, f32, f32, f32);

void SMS_GoRotate(const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&,
                  f32, f32*);
void SMSCalcJumpVelocityY(const JGeometry::TVec3<f32>&,
                          const JGeometry::TVec3<f32>&, f32, f32, f32,
                          JGeometry::TVec3<f32>*);
void SMSCalcJumpVelocityXZ(const JGeometry::TVec3<f32>&,
                           const JGeometry::TVec3<f32>&, f32, f32,
                           JGeometry::TVec3<f32>*);

void MsVECNormalize(Vec*, Vec*);
void MsVECMag2(Vec*);

#endif

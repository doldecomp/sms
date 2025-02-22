#include <JSystem/JStage/JSGCamera.hpp>
#include <float.h>

JStage::TCamera::~TCamera() { }

s32 JStage::TCamera::JSGFGetType() const { return 3; }

JStage::TECameraProjection JStage::TCamera::JSGGetProjectionType() const { return TECAMERAPROJECTION_Unk1; }

void JStage::TCamera::JSGSetProjectionType(JStage::TECameraProjection) { }

f32 JStage::TCamera::JSGGetProjectionNear() const { return 0.0f; }

void JStage::TCamera::JSGSetProjectionNear(f32) { }

f32 JStage::TCamera::JSGGetProjectionFar() const { return FLT_MAX; }

void JStage::TCamera::JSGSetProjectionFar(f32) { }

f32 JStage::TCamera::JSGGetProjectionFovy() const { return 0.0f; }

void JStage::TCamera::JSGSetProjectionFovy(f32) { }

f32 JStage::TCamera::JSGGetProjectionAspect() const { return 0.0f; }

void JStage::TCamera::JSGSetProjectionAspect(f32) { }

void JStage::TCamera::JSGGetProjectionField(f32*) const { }

void JStage::TCamera::JSGSetProjectionField(const f32*) { }

bool JStage::TCamera::JSGGetViewType() const { return true; }

void JStage::TCamera::JSGSetViewType(JStage::TECameraView) { }

void JStage::TCamera::JSGGetViewPosition(Vec*) const { }

void JStage::TCamera::JSGSetViewPosition(const Vec&) { }

void JStage::TCamera::JSGGetViewUpVector(Vec*) const { }

void JStage::TCamera::JSGSetViewUpVector(const Vec&) { }

void JStage::TCamera::JSGGetViewTargetPosition(Vec*) const { }

void JStage::TCamera::JSGSetViewTargetPosition(const Vec&) { }

f32 JStage::TCamera::JSGGetViewRoll() const { return 0.0f; }

void JStage::TCamera::JSGSetViewRoll(f32) { }

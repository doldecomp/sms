#ifndef JSG_CAMERA_HPP
#define JSG_CAMERA_HPP

#include <JSystem/JStage/JSGObject.hpp>
#include <dolphin/mtx.h>

namespace JStage {

enum TECameraProjection { TECAMERAPROJECTION_Unk1 = 1 };

struct TECameraView { };

struct TCamera : public TObject {
	virtual ~TCamera() = 0;
	virtual s32 JSGFGetType() const;
	virtual JStage::TECameraProjection JSGGetProjectionType() const;
	virtual void JSGSetProjectionType(JStage::TECameraProjection);
	virtual f32 JSGGetProjectionNear() const;
	virtual void JSGSetProjectionNear(f32);
	virtual f32 JSGGetProjectionFar() const;
	virtual void JSGSetProjectionFar(f32);
	virtual f32 JSGGetProjectionFovy() const;
	virtual void JSGSetProjectionFovy(f32);
	virtual f32 JSGGetProjectionAspect() const;
	virtual void JSGSetProjectionAspect(f32);
	virtual void JSGGetProjectionField(f32*) const;
	virtual void JSGSetProjectionField(f32 const*);
	virtual bool JSGGetViewType() const;
	virtual void JSGSetViewType(JStage::TECameraView);
	virtual void JSGGetViewPosition(Vec*) const;
	virtual void JSGSetViewPosition(Vec const&);
	virtual void JSGGetViewUpVector(Vec*) const;
	virtual void JSGSetViewUpVector(Vec const&);
	virtual void JSGGetViewTargetPosition(Vec*) const;
	virtual void JSGSetViewTargetPosition(Vec const&);
	virtual f32 JSGGetViewRoll() const;
	virtual void JSGSetViewRoll(f32);
};

} // namespace JStage

#endif

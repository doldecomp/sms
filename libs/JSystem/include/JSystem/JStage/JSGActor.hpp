#ifndef JSG_ACTOR_HPP
#define JSG_ACTOR_HPP

#include <JSystem/JStage/JSGObject.hpp>
#include <dolphin/mtx.h>

namespace JStage {

struct TActor : public TObject {
	virtual ~TActor() = 0;
	virtual s32 JSGFGetType() const;
	virtual void JSGGetTranslation(Vec*) const;
	virtual void JSGSetTranslation(Vec const&);
	virtual void JSGGetScaling(Vec*) const;
	virtual void JSGSetScaling(Vec const&);
	virtual void JSGGetRotation(Vec*) const;
	virtual void JSGSetRotation(Vec const&);
	virtual s32 JSGGetShape() const;
	virtual void JSGSetShape(u32);
	virtual s32 JSGGetAnimation() const;
	virtual void JSGSetAnimation(u32);
	virtual f32 JSGGetAnimationFrame() const;
	virtual void JSGSetAnimationFrame(f32);
	virtual f32 JSGGetAnimationFrameMax() const;
};

} // namespace JStage

#endif

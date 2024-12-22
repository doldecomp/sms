#ifndef JSG_ACTOR_HPP
#define JSG_ACTOR_HPP

#include <dolphin/mtx.h>
#include <JSystem/JSGObject.hpp>

namespace JStage {

class TActor : public TObject {
public:
	typedef void IDK;
	IDK JSGFGetType() const;
	virtual void JSGGetTranslation(Vec*) const;
	virtual void JSGSetTranslation(const Vec&);
	virtual void JSGGetScaling(Vec*) const;
	virtual void JSGSetScaling(const Vec&);
	virtual void JSGGetRotation(Vec*) const;
	virtual void JSGSetRotation(const Vec&);
	virtual u32 JSGGetShape() const;
	virtual void JSGSetShape(u32);
	virtual u32 JSGGetAnimation() const;
	virtual void JSGSetAnimation(u32);
	virtual float JSGGetAnimationFrame() const;
	virtual void JSGSetAnimationFrame(float);
	virtual float JSGGetAnimationFrameMax() const;
};

} // namespace JStage

#endif

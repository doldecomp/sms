#ifndef JDR_ACTOR_HPP
#define JDR_ACTOR_HPP

#include <JSystem/JDRPlacement.hpp>
#include <JSystem/JDRGraphics.hpp>
#include <JSystem/JSGActor.hpp>

namespace JDrama {

class TActor : public TPlacement, public JStage::TActor {
public:
	// TActor
	char _020[0x10];
	f32 unk34;
	char unk40[0x8];

	f32 getUnk34() { return unk34; }

	TActor(const char*);

	typedef void IDK;

	virtual IDK getType();
	virtual IDK load(JSUMemoryInputStream&);

	virtual IDK perform(u32, TGraphics*);

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

} // namespace JDrama

#endif

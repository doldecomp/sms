#ifndef JDR_ACTOR_HPP
#define JDR_ACTOR_HPP

#include <JSystem/JDrama/JDRPlacement.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <JSystem/JStage/JSGActor.hpp>

namespace JDrama {

class TActor : public TPlacement, public JStage::TActor {
public:
	TActor(const char*);
	~TActor();

	virtual int getType() const;
	virtual void load(JSUMemoryInputStream&);
	void issueGXLight(unsigned long, JDrama::TGraphics*);

	virtual void perform(u32, TGraphics*);

	virtual void JSGGetTranslation(Vec*) const;
	virtual void JSGSetTranslation(const Vec&);
	virtual void JSGGetScaling(Vec*) const;
	virtual void JSGSetScaling(const Vec&);
	virtual void JSGGetRotation(Vec*) const;
	virtual void JSGSetRotation(const Vec&);
	virtual s32 JSGGetShape() const;
	virtual void JSGSetShape(u32);
	virtual s32 JSGGetAnimation() const;
	virtual void JSGSetAnimation(u32);
	virtual float JSGGetAnimationFrame() const;
	virtual void JSGSetAnimationFrame(float);
	virtual float JSGGetAnimationFrameMax() const;

public:
	/* 0x24 */ Vec mScaling;
	/* 0x30 */ Vec mRotation;
	/* 0x3C */ TNameRef* unk3C;
	/* 0x40 */ TViewObj* unk40;
};

} // namespace JDrama

#endif

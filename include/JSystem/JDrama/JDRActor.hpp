#ifndef JDR_ACTOR_HPP
#define JDR_ACTOR_HPP

#include <JSystem/JDrama/JDRPlacement.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <JSystem/JStage/JSGActor.hpp>

namespace JDrama {

class TActor : public TPlacement, public JStage::TActor {
public:
	TActor(const char* name)
	    : TPlacement(name)
	{
		mScaling.x = 1.0f;
		mScaling.y = 1.0f;
		mScaling.z = 1.0f;

		mRotation.x = 0.0f;
		mRotation.y = 0.0f;
		mRotation.z = 0.0f;

		unk3C = nullptr;
		unk40 = nullptr;
	}

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

	// fabricated
	const JGeometry::TVec3<f32>& getRotation() const { return mRotation; }
	const JGeometry::TVec3<f32>& getScaling() const { return mScaling; }

public:
	/* 0x24 */ JGeometry::TVec3<f32> mScaling;
	/* 0x30 */ JGeometry::TVec3<f32> mRotation;
	/* 0x3C */ TNameRef* unk3C;
	/* 0x40 */ TViewObj* unk40;
};

} // namespace JDrama

#endif

#ifndef JDR_ACTOR_HPP
#define JDR_ACTOR_HPP

#include <JSystem/JDrama/JDRPlacement.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <JSystem/JStage/JSGActor.hpp>

namespace JDrama {

class TCharacter;

/**
 * @brief A scene graph object which is visible and has a position, rotation &
 * scale.
 * @details Basically, anything you can see on the scene and say "yep, that's a
 * thing": enemies, npcs, map objects, the player, etc.
 */
class TActor : public TPlacement, public JStage::TActor {
public:
	TActor(const char* name)
	    : TPlacement(name)
	{
		mScaling.setAll(1.0f);
		mRotation.setAll(0.0f);

		mCharacter = nullptr;
		mLightMap  = nullptr;
	}

	~TActor();

	virtual int getType() const { return 1; }
	virtual void load(JSUMemoryInputStream&);
	void issueGXLight(u32, JDrama::TGraphics*);

	virtual void perform(u32 cue, TGraphics* graphics);

	virtual void JSGGetTranslation(Vec*) const;
	virtual void JSGSetTranslation(const Vec&);
	virtual void JSGGetScaling(Vec*) const;
	virtual void JSGSetScaling(const Vec&);
	virtual void JSGGetRotation(Vec*) const;
	virtual void JSGSetRotation(const Vec&);

	// fabricated
	const JGeometry::TVec3<f32>& getRotation() const { return mRotation; }
	const JGeometry::TVec3<f32>& getScaling() const { return mScaling; }

	void setCharacter(TCharacter* character) { mCharacter = character; }

public:
	/* 0x24 */ JGeometry::TVec3<f32> mScaling;
	/* 0x30 */ JGeometry::TVec3<f32> mRotation;
	/* 0x3C */ TCharacter* mCharacter;
	/* 0x40 */ TViewObj* mLightMap;
};

} // namespace JDrama

#endif

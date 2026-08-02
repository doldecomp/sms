#ifndef GC2D_BLEND_PANE_HPP
#define GC2D_BLEND_PANE_HPP

#include <GC2D/BoundPane.hpp>
#include <JSystem/J2D/J2DPicture.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>

class TBlendPane : public TBoundPane {
	/* 0x68 */ f32 mBlendStep;
	/* 0x6C */ f32 mBlendProgress;
	/* 0x70 */ bool mBlendActive;

public:
	TBlendPane(J2DScreen*, u32);

	virtual bool update();
	void setPaneBlend(s32 frames, JUTTexture* texture1, JUTTexture* texture2)
	{
		if (texture2 == nullptr) {
			((J2DPicture*)getPane())->changeTexture(texture1->getTexInfo(), 0);
		} else {
			((J2DPicture*)getPane())->changeTexture(texture1->getTexInfo(), 0);
			((J2DPicture*)getPane())->changeTexture(texture2->getTexInfo(), 1);
		}

		mBlendActive   = true;
		mBlendStep     = 1.0f / frames;
		mBlendProgress = 0.0f;
	}
};

#endif

#ifndef GC2D_BLEND_PANE_HPP
#define GC2D_BLEND_PANE_HPP

#include <GC2D/BoundPane.hpp>

class TBlendPane : public TBoundPane {
public:
	TBlendPane(J2DScreen*, u32);

	virtual void update();
	void setPaneBlend(s32, JUTTexture*, JUTTexture*);
};

#endif

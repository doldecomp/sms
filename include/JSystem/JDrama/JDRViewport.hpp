#ifndef JDR_VIEWPORT_HPP
#define JDR_VIEWPORT_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>

namespace JDrama {

class TViewport : public TViewObj {
public:
	TViewport(const TRect& = TRect(0, 0, 640, 480), const char* = "<Viewport>");

	virtual ~TViewport() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(unsigned long, TGraphics*);

public:
	/* 0x10 */ TRect unk10;
	/* 0x20 */ u16 unk20;
};

}; // namespace JDrama

#endif

#ifndef SYSTEM_ZBUFFER_CATCH_HPP
#define SYSTEM_ZBUFFER_CATCH_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TAlphaCatch : public JDrama::TViewObj {
public:
	TAlphaCatch() { }

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
};

class TZBufferCatch : public JDrama::TViewObj {
public:
	TZBufferCatch() { }

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
};

#endif

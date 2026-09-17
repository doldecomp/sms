#ifndef SYSTEM_ZBUFFER_CATCH_HPP
#define SYSTEM_ZBUFFER_CATCH_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TAlphaCatch : public JDrama::TViewObj {
public:
	TAlphaCatch(const char* name = "<AlphaCatch>")
	    : JDrama::TViewObj(name)
	{
	}

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
};

class TZBufferCatch : public JDrama::TViewObj {
public:
	TZBufferCatch(const char* name = "<ZBufferCatch>")
	    : JDrama::TViewObj(name)
	{
	}

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
};

#endif

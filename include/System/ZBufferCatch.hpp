#ifndef SYSTEM_ZBUFFER_CATCH_HPP
#define SYSTEM_ZBUFFER_CATCH_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TAlphaCatch : public JDrama::TViewObj {
public:
	TAlphaCatch();
	void perform(u32, JDrama::TGraphics*);
};

class TZBufferCatch : public JDrama::TViewObj {
public:
	TZBufferCatch();
	void perform(u32, JDrama::TGraphics*);
};

#endif

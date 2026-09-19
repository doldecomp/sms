#ifndef MAP_STICKY_STAIN_MANAGER_HPP
#define MAP_STICKY_STAIN_MANAGER_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TStickyStainManager : public JDrama::TViewObj {
public:
	TStickyStainManager(const char* name)
	    : JDrama::TViewObj(name)
	{
	}

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
};

#endif

#ifndef MAP_STICKY_STAIN_MANAGER_HPP
#define MAP_STICKY_STAIN_MANAGER_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TStickyStainManager : public JDrama::TViewObj {
public:
	virtual void perform(u32, JDrama::TGraphics*);
};

#endif

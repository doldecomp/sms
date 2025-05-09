#ifndef MAP_MARINE_SNOW_HPP
#define MAP_MARINE_SNOW_HPP

#include <JSystem/JDrama/JDRActor.hpp>

class TMarineSnow : public JDrama::TActor {
public:
	TMarineSnow(const char*);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
};

#endif

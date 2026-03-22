#ifndef MAP_TPOLLUTIONTEST_HPP
#define MAP_TPOLUTTIONTEST_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TPollutionTest : public JDrama::TViewObj {
public:
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void loadAfter();
};

#endif

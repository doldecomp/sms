#ifndef MOVE_BG_MAP_OBJ_SAMPLE_HPP
#define MOVE_BG_MAP_OBJ_SAMPLE_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TGateShadow : public JDrama::TViewObj {
public:
	TGateShadow();
	void perform(u32, JDrama::TGraphics*);
};

#endif

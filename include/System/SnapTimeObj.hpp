#ifndef SYSTEM_SNAP_TIME_OBJ_HPP
#define SYSTEM_SNAP_TIME_OBJ_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TSnapTimeObj : public JDrama::TViewObj {
public:
	virtual void perform(u32, JDrama::TGraphics*);

public:
	/* 0x10 */ u32 unk10;
	/* 0x14 */ u16 unk14;
};

#endif

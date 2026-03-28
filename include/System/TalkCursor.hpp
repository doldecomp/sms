#ifndef SYSTEM_TALK_CURSOR_HPP
#define SYSTEM_TALK_CURSOR_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TBaseNPC;
class MActor;

class TTalkCursor : public JDrama::TViewObj {
public:
	TTalkCursor()
	    : unk10(nullptr)
	{
	}

	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);

	void associateNPC(TBaseNPC*);

public:
	/* 0x10 */ MActor* unk10;
};

#endif

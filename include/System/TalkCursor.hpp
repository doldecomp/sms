#ifndef SYSTEM_TALK_CURSOR_HPP
#define SYSTEM_TALK_CURSOR_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TBaseNPC;

class TTalkCursor : public JDrama::TViewObj {
public:
	TTalkCursor() { }

	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);

	void associateNPC(TBaseNPC*);
};

#endif

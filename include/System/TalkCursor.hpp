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
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void associateNPC(TBaseNPC*);

	// fabricated. The named local is the binding closure batch 110 measured
	// at TTalkCursor::loadAfter; a non-binding `return unk10;` is worth zero
	// there.
	MActor* getMActor()
	{
		MActor* actor = unk10;
		return actor;
	}

public:
	/* 0x10 */ MActor* unk10;
};

#endif

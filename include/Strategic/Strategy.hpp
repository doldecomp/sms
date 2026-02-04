#ifndef STRATEGIC_STRATEGY_HPP
#define STRATEGIC_STRATEGY_HPP

#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <Strategic/HitActor.hpp>

class TIdxGroupObj : public JDrama::TViewObjPtrListT<THitActor> {
public:
	virtual void loadSuper(JSUMemoryInputStream&);

	// fabricated
	void add(THitActor* const& obj) { getChildren().push_back(obj); }

public:
	/* 0x20 */ u32 unk20;
};

class TStrategy;

extern TStrategy* gpStrategy;

class TStrategy : public JDrama::TViewObj {
public:
	TStrategy(const char* name = "<TStrategy>");

	virtual void load(JSUMemoryInputStream& stream);
	virtual void loadAfter();
	virtual JDrama::TNameRef* searchF(u16, const char*);
	virtual void perform(u32, JDrama::TGraphics*);

public:
	/* 0x10 */ TIdxGroupObj* unk10[16];
	/* 0x50 */ u16 unk50;
};

#endif

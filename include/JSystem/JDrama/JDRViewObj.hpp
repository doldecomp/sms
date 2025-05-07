#ifndef JDR_VIEW_OBJ_HPP
#define JDR_VIEW_OBJ_HPP

#include <JSystem/JDrama/JDRFlag.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JGadget/std-list.hpp>

namespace JDrama {

class TViewObj : public TNameRef {
public:
	TViewObj(const char* name = "<TViewObj>")
	    : TNameRef(name)
	    , unkC(0)
	{
	}

	void testPerform(u32, TGraphics*);

	virtual ~TViewObj() { }
	virtual void perform(u32, TGraphics*) = 0;

public:
	/* 0xC */ TFlagT<u16> unkC;
};

}; // namespace JDrama

#endif

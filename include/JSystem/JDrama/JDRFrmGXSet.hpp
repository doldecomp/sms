#ifndef JDR_FRM_GX_SET_HPP
#define JDR_FRM_GX_SET_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JDrama/JDRDisplay.hpp>

namespace JDrama {

class TFrmGXSet : public TViewObj {
public:
	TFrmGXSet(TDisplay* display, const char* name = "<TFrmGXSet>")
	    : TViewObj(name)
	    , unk10(display)
	{
	}

	virtual ~TFrmGXSet() { }
	virtual void perform(u32, TGraphics*);

public:
	/* 0x10 */ TDisplay* unk10;
};

} // namespace JDrama

#endif

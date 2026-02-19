#ifndef JDR_DSTAGE_GROUP_HPP
#define JDR_DSTAGE_GROUP_HPP

#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <JSystem/JDrama/JDRFrmGXSet.hpp>

namespace JDrama {

class TDStageGroup : public TViewObjPtrListT<TViewObj> {
public:
	TDStageGroup(TDisplay* display, const char* name = "<TDStageGroup>")
	    : TViewObjPtrListT<TViewObj>(name)
	    , unk20(display)
	{
	}

	virtual void perform(u32, TGraphics*);

public:
	/* 0x20 */ TFrmGXSet unk20;
};

} // namespace JDrama

#endif

#ifndef JDR_DSTAGE_GROUP_HPP
#define JDR_DSTAGE_GROUP_HPP

#include <JSystem/JDrama/JDRViewObjPtrList.hpp>

namespace JDrama {

class TDStageGroup : public TViewObjPtrListT<TViewObj> {
public:
	virtual ~TDStageGroup() { }
	virtual void perform(u32, TGraphics*);

public:
	/* 0x20 */ void* unk20;
};

} // namespace JDrama

#endif

#ifndef JDR_FRM_GX_SET_HPP
#define JDR_FRM_GX_SET_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

namespace JDrama {

class TFrmGXSet : public TViewObj {
public:
	TFrmGXSet();

	virtual ~TFrmGXSet() { }
	virtual void perform(u32, TGraphics*);
};

} // namespace JDrama

#endif

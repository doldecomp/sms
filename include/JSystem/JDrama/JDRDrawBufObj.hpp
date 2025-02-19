#ifndef JDR_DRAW_BUF_OBJ_HPP
#define JDR_DRAW_BUF_OBJ_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

namespace JDrama {

class TDrawBufObj : public TViewObj {
public:
	TDrawBufObj();
	TDrawBufObj(u32, u32, const char*);

	virtual ~TDrawBufObj();
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, TGraphics*);
};

}; // namespace JDrama

#endif

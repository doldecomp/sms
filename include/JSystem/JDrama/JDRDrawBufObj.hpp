#ifndef JDR_DRAW_BUF_OBJ_HPP
#define JDR_DRAW_BUF_OBJ_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class J3DDrawBuffer;

namespace JDrama {

class TDrawBufObj : public TViewObj {
public:
	TDrawBufObj();
	TDrawBufObj(u32, u32, const char*);

	virtual ~TDrawBufObj() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, TGraphics*);

public:
	/* 0x10 */ J3DDrawBuffer* mDrawBuffer;
	/* 0x14 */ u32 mDrawBufferSize;
	/* 0x18 */ u32 unk18;
};

}; // namespace JDrama

#endif

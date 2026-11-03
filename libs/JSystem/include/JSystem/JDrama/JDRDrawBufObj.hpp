#ifndef JDR_DRAW_BUF_OBJ_HPP
#define JDR_DRAW_BUF_OBJ_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class J3DDrawBuffer;

namespace JDrama {

class TDrawBufObj : public TViewObj {
public:
	TDrawBufObj();
	TDrawBufObj(u32 draw_flags, u32 buffer_size, const char* name);

	virtual ~TDrawBufObj() { }
	virtual void load(JSUMemoryInputStream& stream);
	virtual void perform(u32 cue, TGraphics* graphics);

	// fabricated
	J3DDrawBuffer* getDrawBuffer() { return mDrawBuffer; }

public:
	/* 0x10 */ J3DDrawBuffer* mDrawBuffer;
	/* 0x14 */ u32 mDrawBufferSize;
	/* 0x18 */ u32 mDrawFlags;
};

}; // namespace JDrama

#endif

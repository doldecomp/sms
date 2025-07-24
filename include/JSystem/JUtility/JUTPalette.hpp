#ifndef JUT_PALETTE_HPP
#define JUT_PALETTE_HPP

#include <dolphin/gx/GXStruct.h>

enum JUTTransparency { JUT_TRANSPARENCY_UNK0, JUT_TRANSPARENCY_UNK1 };

struct ResTLUT {
	u8 format;
	u8 transparency;
	u16 numColors;
};

class JUTPalette {
public:
	JUTPalette(GXTlut p1, GXTlutFmt p2, JUTTransparency p3, u16 p4, void* p5);

	JUTPalette(GXTlut tlutNo, ResTLUT* p_tlutRes)
	{
		storeTLUT(tlutNo, p_tlutRes);
	}

	void storeTLUT(GXTlut, ResTLUT*);
	void storeTLUT(_GXTlut, _GXTlutFmt, JUTTransparency, u16, void*);
	bool load();

	u32 getTlutName() const { return mTlutName; }
	u8 getFormat() const { return mFormat; }
	u32 getTransparency() const { return mTransparency; }
	u16 getNumColors() const { return mNumColors; }
	ResTLUT* getColorTable() const { return mColorTable; }

private:
	GXTlutObj mTlutObj;
	u32 mTlutName;
	u32 mFormat;
	ResTLUT* mColorTable;
	u16 mNumColors;
	u32 mTransparency;
};

#endif

#include <JSystem/JUtility/JUTRomFont.hpp>
#include <JSystem/JUtility/JUTConsole.hpp>
#include <dolphin/gx.h>

const JUTRomFont::AboutFont JUTRomFont::saoAboutEncoding_[2] = {
	{ 0, 0x00020120, &JUTFont::isLeadByte_1Byte },
	{ 2, 0x00120F00, &JUTFont::isLeadByte_ShiftJIS },
};
const JUTRomFont::AboutFont* JUTRomFont::spAboutEncoding_;
OSFontHeader* JUTRomFont::spFontHeader_;
u32 JUTRomFont::suFontHeaderRefered_;

JUTRomFont::JUTRomFont(JKRHeap* heap)
    : JUTFont()
{
	initiate(heap);
}

void JUTRomFont::initiate(JKRHeap* heap)
{
	JUTFont::initiate();
	loadImage(heap);
}

JUTRomFont::~JUTRomFont()
{
	--suFontHeaderRefered_;
	if (suFontHeaderRefered_ == 0) {
		JKRFree(spFontHeader_);
		spFontHeader_    = nullptr;
		spAboutEncoding_ = nullptr;
	}
}

void JUTRomFont::loadImage(JKRHeap* heap)
{
	if (!heap)
		heap = JKRHeap::getCurrentHeap();

	if (spFontHeader_ == nullptr) {
		JUTReportConsole_f("Font Encode Type %d\n", OSGetFontEncode());
		spAboutEncoding_ = &saoAboutEncoding_[OSGetFontEncode()];
		u32 sz           = spAboutEncoding_->mSize;
		JUTReportConsole_f("IPLROM fontdata size : %u\n", sz);
		spFontHeader_ = (OSFontHeader*)JKRAllocFromHeap(heap, sz, 0x20);
		OSInitFont(spFontHeader_);
		mFixedWidth = spFontHeader_->width;
	}

	++suFontHeaderRefered_;
}

void JUTRomFont::setGX()
{
	GXSetNumChans(1);
	GXSetNumTevStages(1);
	GXSetNumTexGens(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBX8, 15);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
}

f32 JUTRomFont::drawChar_scale(float pos_x, float pos_y, float scale_x,
                               float scale_y, int chr, bool flag)
{
	// TODO: figure out the `trash` values in the stack frame

	s32 width;
	char trash1[0x4];
	void* image;
	s32 fontX;
	s32 fontY;

	if (chr == '\0') {
		return 0.0f;
	}

	f32 xScale = scale_x / getWidth();
	f32 yScale = scale_y / getHeight();

	// TODO: this is likely an inline used both here and in getWidthEntry
	char str[2];
	char trash2[0x4];
	char* strPtr = str;
	if (chr >= 0x100) {
		strPtr++;
		str[0] = chr >> 8;
	}
	*strPtr = chr;

	OSGetFontTexture(str, &image, &fontX, &fontY, &width);

	GXTexObj texObj;
	GXInitTexObj(&texObj, image, 512, 512, GX_TF_I4, GX_CLAMP, GX_CLAMP,
	             GX_DISABLE);
	GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_DISABLE,
	                GX_DISABLE, GX_ANISO_1);
	GXLoadTexObj(&texObj, GX_TEXMAP0);

	s32 shift = mFixed ? mFixedWidth - width : 0;

	f32 posMinX = xScale * shift / 2 + pos_x;
	f32 posMaxX = width * xScale + posMinX;
	f32 posMinY = pos_y - yScale * getAscent();
	f32 posMaxY = yScale * getDescent() + pos_y;

	s32 texMinX = (fontX << 15) / 512;
	s32 texMaxX = ((fontX + width) << 15) / 512;
	s32 texMinY = (fontY << 15) / 512;
	s32 texMaxY = ((fontY + getHeight()) << 15) / 512;

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);

	// Bottom left
	GXPosition3f32((s16)posMinX, (s16)posMinY, 0.0f);
	GXColor1u32(mColor1);
	GXPosition2u16(texMinX, texMinY);

	// Bottom right
	GXPosition3f32((s16)posMaxX, (s16)posMinY, 0.0f);
	GXColor1u32(mColor2);
	GXPosition2u16(texMaxX, texMinY);

	// Top right
	GXPosition3f32((s16)posMaxX, (s16)posMaxY, 0.0f);
	GXColor1u32(mColor4);
	GXPosition2u16(texMaxX, texMaxY);

	// Top left
	GXPosition3f32((s16)posMinX, (s16)posMaxY, 0.0f);
	GXColor1u32(mColor3);
	GXPosition2u16(texMinX, texMaxY);

	return xScale * (width + shift);
}

s32 JUTRomFont::getWidth() const { return spFontHeader_->width; }

void JUTRomFont::getWidthEntry(int i, JUTFont::TWidth* width) const
{
	char* ptr;
	s32 w;
	char str[4];
	ptr = str;
	if (i >= 256) {
		*ptr = i >> 8;
		++ptr;
	}
	*ptr = i;
	OSGetFontWidth(str, &w);
	width->field_0x1 = w;
	width->field_0x0 = 0;
}

bool JUTRomFont::isLeadByte(int i) const
{
	return spAboutEncoding_->isLeadByte(i);
}

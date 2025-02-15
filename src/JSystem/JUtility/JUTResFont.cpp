#include <JSystem/JUtility/JUTResFont.hpp>
#include <JSystem/JUtility/JUTConsole.hpp>
#include <dolphin/gx.h>
#include <types.h>



IsLeadByte_func const JUTResFont::saoAboutEncoding_[3] = {
	isLeadByte_1Byte,
	isLeadByte_2Byte,
	isLeadByte_ShiftJIS,
};

JUTResFont::JUTResFont(const ResFONT* font, JKRArchive* arch)
{
	mpWidthBlocks = nullptr;
	mpGlyphBlocks = nullptr;
	mpMapBlocks   = nullptr;
	initiate(font, arch);
}

JUTResFont::~JUTResFont()
{
	if (mpWidthBlocks)
		delete[] mpWidthBlocks;
	if (mpGlyphBlocks)
		delete[] mpGlyphBlocks;
	if (mpMapBlocks)
		delete[] mpMapBlocks;
}

bool JUTResFont::initiate(const ResFONT* font, JKRArchive* arch)
{
	JUTFont::initiate();

	if (font) {
		if (mpWidthBlocks)
			delete[] mpWidthBlocks;
		if (mpGlyphBlocks)
			delete[] mpGlyphBlocks;
		if (mpMapBlocks)
			delete[] mpMapBlocks;

		protected_initiate(font);
	}
	return mResFont != nullptr;
}

void JUTResFont::protected_initiate(const ResFONT* font)
{
	mResFont      = nullptr;
	mpWidthBlocks = nullptr;
	mpGlyphBlocks = nullptr;
	mpMapBlocks   = nullptr;
	mWidth        = 0;
	mHeight       = 0;
	mTexPageIdx   = -1;
	if (!font)
		return;

	mResFont = font;
	countBlock();
	if (mWidthBlockNum)
		mpWidthBlocks = new ResFONT::WID1*[mWidthBlockNum];
	if (mGlyphBlockNum)
		mpGlyphBlocks = new ResFONT::GLY1*[mGlyphBlockNum];
	if (mMapBlockNum)
		mpMapBlocks = new ResFONT::MAP1*[mMapBlockNum];
	setBlock();
}

// Stolen from TWW, probably fake
struct JUTDataBlockHeader {
	/* 0x0 */ u32 mType;
	/* 0x4 */ u32 mSize;

	// fake inline
	const JUTDataBlockHeader* getNext() const
	{
		return (const JUTDataBlockHeader*)((const u8*)(this) + mSize);
	}
};

void JUTResFont::countBlock()
{
	mWidthBlockNum = 0;
	mGlyphBlockNum = 0;
	mMapBlockNum   = 0;

	const JUTDataBlockHeader* header = (JUTDataBlockHeader*)mResFont->data;
	for (u32 i = 0; i < mResFont->numBlocks; i++, header = header->getNext()) {
		switch (header->mType) {
		case 'WID1':
			mWidthBlockNum++;
			break;
		case 'GLY1':
			mGlyphBlockNum++;
			break;
		case 'MAP1':
			mMapBlockNum++;
			break;
		case 'INF1':
			break;
		default:
			JUTReportConsole("JUTResFont: Unknown data block\n");
		}
	}
}

void JUTResFont::setBlock()
{
	int widthNum = 0;
	int glyphNum = 0;
	int mapNum   = 0;
	mMaxCode     = -1;

	const JUTDataBlockHeader* header = (JUTDataBlockHeader*)mResFont->data;
	for (u32 i = 0; i < mResFont->numBlocks; i++, header = header->getNext()) {
		switch (header->mType) {
		case 'INF1': {
			mInfoBlock  = (ResFONT::INF1*)header;
			mIsLeadByte = &saoAboutEncoding_[mInfoBlock->fontType];
			break;
		}
		case 'WID1':
			mpWidthBlocks[widthNum] = (ResFONT::WID1*)header;
			widthNum++;
			break;
		case 'GLY1':
			mpGlyphBlocks[glyphNum] = (ResFONT::GLY1*)header;
			glyphNum++;
			break;
		case 'MAP1':
			mpMapBlocks[mapNum] = (ResFONT::MAP1*)header;
			if (mMaxCode > mpMapBlocks[mapNum]->startCode) {
				mMaxCode = mpMapBlocks[mapNum]->startCode;
			}
			mapNum++;
			break;
		default:
			JUTReportConsole("Unknown data block\n");
			break;
		}
	}
}

void JUTResFont::setGX()
{
	GXSetNumChans(1);
	GXSetNumTevStages(1);
	GXSetNumTexGens(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_RGBA4, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBX8, 0xf);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
}

void JUTResFont::setGX(JUtility::TColor col1, JUtility::TColor col2)
{
	if (col1 == 0 && col2 == -1) {
		setGX();
	} else {
		GXSetNumChans(1);
		GXSetNumTevStages(3);
		GXSetNumTexGens(1);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
		GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO,
		                GX_CC_ZERO);
		GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO,
		                GX_CA_ZERO);
		GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
		                GX_TEVPREV);
		GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
		                GX_TEVPREV);
		GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
		              GX_COLOR_NULL);
		GXSetTevColor(GX_TEVREG0, col1);
		GXSetTevColor(GX_TEVREG1, col2);
		GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_C0, GX_CC_C1, GX_CC_CPREV,
		                GX_CC_ZERO);
		GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_A0, GX_CA_A1, GX_CA_APREV,
		                GX_CA_ZERO);
		GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
		                GX_TEVPREV);
		GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
		                GX_TEVPREV);
		GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
		              GX_COLOR0A0);
		GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC,
		                GX_CC_ZERO);
		GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA,
		                GX_CA_ZERO);
		GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
		                GX_TEVPREV);
		GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
		                GX_TEVPREV);
		GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
		               GX_LO_SET);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_RGBA4, 0);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBX8, 15);
		GXClearVtxDesc();
		GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
		GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
		GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	}
}

f32 JUTResFont::drawChar_scale(f32 posX, f32 posY, f32 scaleX, f32 scaleY,
                               int chr, bool flag)
{
	JUTFont::TWidth width;
	f32 x1;
	f32 y1;
	f32 scaledHeight;
	f32 x2;

	loadFont(chr, GX_TEXMAP0, &width);

	if (mFixed || !flag) {
		x1 = posX;
	} else {
		x1 = (posX - width.field_0x0 * (scaleX / getWidth()));
	}
	f32 retval = mFixedWidth * (scaleX / getWidth());
	if (!mFixed) {
		if (!flag) {
			retval
			    = (width.field_0x1 + width.field_0x0) * (scaleX / getWidth());
		} else {
			retval = width.field_0x1 * (scaleX / getWidth());
		}
	}
	x2           = x1 + scaleX;
	y1           = posY - getAscent() * (scaleY / getHeight());
	scaledHeight = scaleY / getHeight();
	f32 descent  = getDescent();
	f32 y2       = descent * scaledHeight + posY;

	// Glyph details
	int u1 = (mWidth * 0x8000) / mpGlyphBlocks[field_0x62]->textureWidth;
	int v1 = (mHeight * 0x8000) / mpGlyphBlocks[field_0x62]->textureHeight;
	int u2 = ((mWidth + getWidth()) * 0x8000)
	         / mpGlyphBlocks[field_0x62]->textureWidth;
	int v2 = ((mHeight + getHeight()) * 0x8000)
	         / mpGlyphBlocks[field_0x62]->textureHeight;

	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);

	// Bottom Left
	GXPosition3f32(x1, y1, 0.0f);
	GXColor1u32(mColor1);
	GXTexCoord2u16(u1, v1);

	// Bottom Right
	GXPosition3f32(x2, y1, 0.0f);
	GXColor1u32(mColor2);
	GXTexCoord2u16(u2, v1);

	// Top Right
	GXPosition3f32(x2, y2, 0.0f);
	GXColor1u32(mColor4);
	GXTexCoord2u16(u2, v2);

	// Top Left
	GXPosition3f32(x1, y2, 0.0f);
	GXColor1u32(mColor3);
	GXTexCoord2u16(u1, v2);

	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);

	return retval;
}

void JUTResFont::loadFont(int code, GXTexMapID tex_map_id,
                          JUTFont::TWidth* width)
{
	int fontCode = getFontCode(code);
	if (width) {
		int i;
		for (i = 0; i < mWidthBlockNum; i++) {
			if (mpWidthBlocks[i]->startCode <= fontCode
			    && fontCode <= mpWidthBlocks[i]->endCode) {
				int chunkNum = (fontCode - mpWidthBlocks[i]->startCode);
				TWidth w = ((TWidth*)&mpWidthBlocks[i]->mChunkNum)[chunkNum];
				width->field_0x0 = w.field_0x0;
				width->field_0x1 = w.field_0x1;
				break;
			}
		}
		if (i == mWidthBlockNum) {
			width->field_0x0 = 0;
			width->field_0x1 = getWidth();
		}
	}
	loadImage(fontCode, tex_map_id);
}

void JUTResFont::getWidthEntry(int code, JUTFont::TWidth* width) const
{
	int fontCode     = getFontCode(code);
	width->field_0x1 = mInfoBlock->width;
	width->field_0x0 = 0;

	for (int i = 0; i < mWidthBlockNum; i++) {
		if (mpWidthBlocks[i]->startCode <= fontCode
		    && fontCode <= mpWidthBlocks[i]->endCode) {
			int chunkNum = (fontCode - mpWidthBlocks[i]->startCode);
			// TODO: fakematch probably
			*width = ((TWidth*)&mpWidthBlocks[i]->mChunkNum)[chunkNum];
			break;
		}
	}
}

bool JUTResFont::isLeadByte(int chr) const { return (*mIsLeadByte)(chr); }

int JUTResFont::getFontCode(int chr) const
{
	static const u16 halftofull[95] = {
		0x8140, 0x8149, 0x8168, 0x8194, 0x8190, 0x8193, 0x8195, 0x8166, 0x8169,
		0x816A, 0x8196, 0x817B, 0x8143, 0x817C, 0x8144, 0x815E, 0x824F, 0x8250,
		0x8251, 0x8252, 0x8253, 0x8254, 0x8255, 0x8256, 0x8257, 0x8258, 0x8146,
		0x8147, 0x8183, 0x8181, 0x8184, 0x8148, 0x8197, 0x8260, 0x8261, 0x8262,
		0x8263, 0x8264, 0x8265, 0x8266, 0x8267, 0x8268, 0x8269, 0x826A, 0x826B,
		0x826C, 0x826D, 0x826E, 0x826F, 0x8270, 0x8271, 0x8272, 0x8273, 0x8274,
		0x8275, 0x8276, 0x8277, 0x8278, 0x8279, 0x816D, 0x818F, 0x816E, 0x814F,
		0x8151, 0x8165, 0x8281, 0x8282, 0x8283, 0x8284, 0x8285, 0x8286, 0x8287,
		0x8288, 0x8289, 0x828A, 0x828B, 0x828C, 0x828D, 0x828E, 0x828F, 0x8290,
		0x8291, 0x8292, 0x8293, 0x8294, 0x8295, 0x8296, 0x8297, 0x8298, 0x8299,
		0x829A, 0x816F, 0x8162, 0x8170, 0x8160,
	};

	int ret = mInfoBlock->defaultCode;
	if ((getFontType() == 2) && (mMaxCode >= 0x8000u) && (chr >= 0x20)
	    && (chr < 0x7Fu)) {
		chr = halftofull[chr - 32];
	}
	for (int i = 0; i < mMapBlockNum; i++) {
		if ((mpMapBlocks[i]->startCode <= chr)
		    && (chr <= mpMapBlocks[i]->endCode)) {
			ResFONT::MAP1* map = mpMapBlocks[i];
			if (map->mappingMethod == 0) {
				ret = chr - map->startCode;
				break;
			} else if (map->mappingMethod == 2) {
				ret = *(&mpMapBlocks[i]->mLeading
				        + ((chr - mpMapBlocks[i]->startCode)));
				break;
			} else if (map->mappingMethod == 3) {
				u16* leading_temp = &map->mLeading;
				int phi_r5        = 0;
				int phi_r6_2      = map->numEntries - 1;

				while (phi_r6_2 >= phi_r5) {
					int temp_r7 = (phi_r6_2 + phi_r5) / 2;

					if (chr < leading_temp[temp_r7 * 2]) {
						phi_r6_2 = temp_r7 - 1;
						continue;
					}

					if (chr > leading_temp[temp_r7 * 2]) {
						phi_r5 = temp_r7 + 1;
						continue;
					}

					ret = leading_temp[temp_r7 * 2 + 1];
					break;
				}
			} else if (map->mappingMethod == 1) {
				ret = convertSjis(chr, nullptr);
				break;
			}
			break;
		}
	}
	return ret;
}

void JUTResFont::loadImage(int code, GXTexMapID id)
{
	int i = 0;
	for (; i < mGlyphBlockNum; i++) {
		if (mpGlyphBlocks[i]->startCode <= code
		    && code <= mpGlyphBlocks[i]->endCode) {
			code -= mpGlyphBlocks[i]->startCode;
			break;
		}
	}

	if (i == mGlyphBlockNum)
		return;

	s32 pageNumCells = mpGlyphBlocks[i]->numRows * mpGlyphBlocks[i]->numColumns;
	s32 pageIdx      = code / pageNumCells;
	s32 cellIdxInPage = code % pageNumCells;
	s32 cellRow       = (cellIdxInPage / mpGlyphBlocks[i]->numRows);
	s32 cellCol       = (cellIdxInPage - cellRow * mpGlyphBlocks[i]->numRows);

	mWidth  = (cellCol)*mpGlyphBlocks[i]->cellWidth;
	mHeight = (cellRow)*mpGlyphBlocks[i]->cellHeight;

	if (pageIdx != mTexPageIdx || i != field_0x62) {
		GXInitTexObj(
		    &mTexObj,
		    &mpGlyphBlocks[i]->data[pageIdx * mpGlyphBlocks[i]->textureSize],
		    mpGlyphBlocks[i]->textureWidth, mpGlyphBlocks[i]->textureHeight,
		    (GXTexFmt)mpGlyphBlocks[i]->textureFormat, GX_CLAMP, GX_CLAMP, 0);

		GXInitTexObjLOD(&mTexObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, 0U,
		                0U, GX_ANISO_1);
		mTexPageIdx = pageIdx;
		field_0x62  = i;
	}

	GXLoadTexObj(&mTexObj, id);
}

inline u8 JSULoByte(u16 in) { return in & 0xff; }

inline u8 JSUHiByte(u16 in) { return in >> 8; }

int JUTResFont::convertSjis(int inChar, u16* inLead) const
{
	int hi = JSUHiByte(inChar);
	int lo = JSULoByte(inChar) - 0x40;
	if (0x40 <= lo)
		lo--;
	return lo + (hi - 0x88) * 0xBC + 0x2BE;
}

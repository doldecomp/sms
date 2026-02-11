#ifndef JUT_RESFONT_H
#define JUT_RESFONT_H

#include <JSystem/JUtility/JUTFont.hpp>
#include <dolphin/gx/GXEnum.h>

class JKRArchive;

typedef bool (*IsLeadByte_func)(int);

class JUTResFont : public JUTFont {
public:
	JUTResFont();
	JUTResFont(const ResFONT* font, JKRArchive* arch);

	virtual ~JUTResFont();
	virtual void setGX();
	virtual void setGX(JUtility::TColor, JUtility::TColor);
	virtual f32 drawChar_scale(f32, f32, f32, f32, int, bool);
	virtual int getLeading() const { return mInfoBlock->leading; }
	virtual s32 getAscent() const { return mInfoBlock->ascent; }
	virtual s32 getDescent() const { return mInfoBlock->descent; }
	virtual s32 getHeight() const { return getAscent() + getDescent(); }
	virtual s32 getWidth() const { return mInfoBlock->width; }
	virtual void getWidthEntry(int, JUTFont::TWidth*) const;
	virtual int getFontType() const { return mInfoBlock->fontType; }
	virtual bool isLeadByte(int) const;

	bool initiate(const ResFONT* font, JKRArchive* arch);
	void protected_initiate(const ResFONT* font);
	void countBlock();
	void setBlock();
	void loadFont(int, GXTexMapID, JUTFont::TWidth*);
	int getFontCode(int) const;
	void loadImage(int, GXTexMapID);
	int convertSjis(int, u16*) const;

	const ResFONT* getResFont() const { return mResFont; }

	static const int suAboutEncoding_ = 3;
	static IsLeadByte_func const saoAboutEncoding_[suAboutEncoding_];

public:
	/* 0x1C */ int mWidth;
	/* 0x20 */ int mHeight;
	/* 0x24 */ _GXTexObj mTexObj;
	/* 0x44 */ int mTexPageIdx;
	/* 0x48 */ const ResFONT* mResFont;
	/* 0x4C */ ResFONT::INF1* mInfoBlock;
	/* 0x50 */ ResFONT::WID1** mpWidthBlocks;
	/* 0x54 */ ResFONT::GLY1** mpGlyphBlocks;
	/* 0x58 */ ResFONT::MAP1** mpMapBlocks;
	/* 0x5C */ u16 mWidthBlockNum;
	/* 0x5E */ u16 mGlyphBlockNum;
	/* 0x60 */ u16 mMapBlockNum;
	/* 0x62 */ u16 field_0x62;
	/* 0x64 */ u16 mMaxCode;
	/* 0x68 */ const IsLeadByte_func* mIsLeadByte;
};

#endif

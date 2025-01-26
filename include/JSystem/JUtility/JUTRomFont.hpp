#ifndef JUT_ROMFONT_HPP
#define JUT_ROMFONT_HPP

#include <JSystem/JUtility/JUTFont.hpp>
#include <JSystem/JUtility/JUTResFont.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <dolphin/os/OSFont.h>

class JUTRomFont : public JUTFont {
public:
	JUTRomFont();
	JUTRomFont(JKRHeap*);

	virtual ~JUTRomFont();
	virtual void setGX();
	virtual f32 drawChar_scale(float, float, float, float, int, bool);
	virtual int getLeading() const { return spFontHeader_->leading; }
	virtual s32 getAscent() const { return spFontHeader_->ascent; }
	virtual s32 getDescent() const { return spFontHeader_->descent; }
	virtual s32 getHeight() const { return getAscent() + getDescent(); }
	virtual s32 getWidth() const;
	virtual void getWidthEntry(int, JUTFont::TWidth*) const;
	virtual JUTResFont* getResFont() const { return nullptr; }
	virtual int getFontType() const { return spAboutEncoding_->mType; }
	virtual bool isLeadByte(int) const;

	void initiate(JKRHeap*);
	void loadImage(JKRHeap*);

	struct AboutFont // fabricated
	{
		int mType;
		u32 mSize;
		bool (*isLeadByte)(int);
	}; // Size 0xC

	static const AboutFont saoAboutEncoding_[2];
	static const AboutFont* spAboutEncoding_;
	static OSFontHeader* spFontHeader_;
	static u32 suFontHeaderRefered_;
};

#endif

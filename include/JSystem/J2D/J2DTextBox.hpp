#ifndef J2D_TEXTBOX_HPP
#define J2D_TEXTBOX_HPP

#include <JSystem/J2D/J2DPane.hpp>
#include <JSystem/JUtility/JUTFont.hpp>

enum J2DTextBoxHBinding {
	/* 0x0 */ HBIND_CENTER,
	/* 0x1 */ HBIND_RIGHT,
	/* 0x2 */ HBIND_LEFT
};

enum J2DTextBoxVBinding {
	/* 0x0 */ VBIND_CENTER,
	/* 0x1 */ VBIND_BOTTOM,
	/* 0x2 */ VBIND_TOP
};

class J2DTextBox : public J2DPane {
public:
	J2DTextBox(const ResFONT*, const char*);
	J2DTextBox(const char*, const char*);
	J2DTextBox(J2DPane*, JSURandomInputStream*, bool);
	J2DTextBox(u32, const JUTRect&, const ResFONT*, const char*,
	           J2DTextBoxHBinding, J2DTextBoxVBinding);
	J2DTextBox(u32, const JUTRect&, const char*, const char*,
	           J2DTextBoxHBinding, J2DTextBoxVBinding);

	virtual ~J2DTextBox();
	virtual void resize(int w, int h);
	virtual bool setConnectParent(bool connected);
	virtual void drawSelf(int x, int y);
	virtual void drawSelf(int x, int y, Mtx* mtx);

	void initiate(const ResFONT*, const char*, J2DTextBoxHBinding,
	              J2DTextBoxVBinding);
	void setFontSize();
	void setFont(JUTFont*);
	void setLineSpace();
	void draw(int, int);
	void draw(int, int, int);
	void getString(char*);
	char* getStringPtr() const;
	size_t setString(const char*, ...);

	// fabricated
	void setFontSize(int x, int y)
	{
		mFontSizeX = x;
		mFontSizeY = y;
	}

	void setBlackWhite(JUtility::TColor black, JUtility::TColor white)
	{
		mBlack = black;
		mWhite = white;
	}

public:
	/* 0xEC */ JUTFont* mFont;
	/* 0xF0 */ JUtility::TColor mCharColor;
	/* 0xF4 */ JUtility::TColor mGradColor;
	/* 0xF8 */ J2DTextBoxHBinding mHBinding;
	/* 0xFC */ J2DTextBoxVBinding mVBinding;
	/* 0x100 */ char* mText;
	/* 0x104 */ int unk104;
	/* 0x108 */ int unk108;
	/* 0x10C */ int mCharSpace;
	/* 0x110 */ int mLineSpace;
	/* 0x114 */ int mFontSizeX;
	/* 0x118 */ int mFontSizeY;
	/* 0x11C */ JUtility::TColor mWhite;
	/* 0x120 */ JUtility::TColor mBlack;
	/* 0x124 */ bool mTextFontOwned;
};

#endif

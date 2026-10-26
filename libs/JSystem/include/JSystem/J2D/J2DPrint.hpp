#ifndef J2D_PRINT_HPP
#define J2D_PRINT_HPP

#include <JSystem/JUtility/JUTFont.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <dolphin/os/OSMutex.h>
#include <stdarg.h>

#define J2DPRINT_ESC                 "\x1B"
#define J2DPRINT_CODE(code)          J2DPRINT_ESC #code
#define J2DPRINT_CODE_ARG(code, arg) J2DPRINT_ESC #code "[" #arg "]"

#define J2DPRINT_CURSOR_UP(n)          J2DPRINT_CODE_ARG(CU, n)
#define J2DPRINT_CURSOR_DOWN(n)        J2DPRINT_CODE_ARG(CD, n)
#define J2DPRINT_CURSOR_LEFT(n)        J2DPRINT_CODE_ARG(CL, n)
#define J2DPRINT_CURSOR_RIGHT(n)       J2DPRINT_CODE_ARG(CR, n)
#define J2DPRINT_LINE_UP               J2DPRINT_CODE(LU)
#define J2DPRINT_LINE_DOWN             J2DPRINT_CODE(LD)
#define J2DPRINT_TAB_WIDTH(n)          J2DPRINT_CODE_ARG(ST, n)
#define J2DPRINT_CHAR_COLOR(color)     J2DPRINT_ESC "CC[" color "]"
#define J2DPRINT_GRADIENT_COLOR(color) J2DPRINT_ESC "GC[" color "]"
#define J2DPRINT_FONT_WIDTH(n)         J2DPRINT_CODE_ARG(FX, n)
#define J2DPRINT_FONT_HEIGHT(n)        J2DPRINT_CODE_ARG(FY, n)
#define J2DPRINT_CHAR_SPACING(n)       J2DPRINT_CODE_ARG(SH, n)
#define J2DPRINT_LINE_SPACING(n)       J2DPRINT_CODE_ARG(SV, n)
#define J2DPRINT_GRADIENT_MODE(n)      J2DPRINT_CODE_ARG(GM, n)

#define J2DPRINT_TAB_WIDTH_RESET      J2DPRINT_CODE(ST)
#define J2DPRINT_CHAR_COLOR_RESET     J2DPRINT_CODE(CC)
#define J2DPRINT_GRADIENT_COLOR_RESET J2DPRINT_CODE(GC)
#define J2DPRINT_FONT_WIDTH_RESET     J2DPRINT_CODE(FX)
#define J2DPRINT_FONT_HEIGHT_RESET    J2DPRINT_CODE(FY)
#define J2DPRINT_CHAR_SPACING_RESET   J2DPRINT_CODE(SH)
#define J2DPRINT_LINE_SPACING_RESET   J2DPRINT_CODE(SV)
#define J2DPRINT_GRADIENT_MODE_TOGGLE J2DPRINT_CODE(GM)

class J2DPrint;
extern "C" float J2DPrint_print_alpha_va(J2DPrint*, u8, const char*, va_list);

class J2DPrint {
public:
	struct TSize {
		float unk0;
		float unk4;
	};

	J2DPrint(JUTFont*, int);
	J2DPrint(JUTFont*, int, int);
	J2DPrint(JUTFont*, JUtility::TColor, JUtility::TColor);
	J2DPrint(JUTFont*, int, int, JUtility::TColor, JUtility::TColor);

	virtual ~J2DPrint();

	void initiate();
	void private_initiate(JUTFont* font, int, int, JUtility::TColor,
	                      JUtility::TColor);
	void setBuffer(char*, u32);
	static char* setBuffer(u32);
	void setFontSize();
	void locate(int, int);
	void putChar(int);
	void putChar(int, int, int);
	void print(const char* format, ...);
	void print(u8, const char* format, ...);
	float print_va(u8 opacity, const char* format, va_list args)
	{
		return J2DPrint_print_alpha_va(this, opacity, format, args);
	}
	float print(int x, int y, const char* format, ...);
	float print(int x, int y, u8 opacity, const char* format, ...);
	void getSize(J2DPrint::TSize&, const char*, ...);
	float getWidth(const char* format, ...);
	float getHeight(const char* format, ...);
	void printReturn(const char*, int, int, J2DTextBoxHBinding hbind,
	                 J2DTextBoxVBinding vbind, int, int, u8 opacity);
	float parse(const u8*, int, int, unsigned short*, J2DPrint::TSize&,
	            u8 opacity, bool);
	void doCtrlCode(int code);
	u16 doEscapeCode(const u8**, u8 opacity);
	void initchar();
	int getNumber(const u8**, s32, s32, int base);

	void setFontSize(int sizeX, int sizeY)
	{
		mFontSizeX = sizeX > 0 ? sizeX : 0;
		mFontSizeY = sizeY > 0 ? sizeY : 0;
	}

	void setCharColor(JUtility::TColor color) { mCharColor = color; }
	void setGradColor(JUtility::TColor color) { mGradColor = color; }

	void setSomeColors(JUtility::TColor c1, JUtility::TColor c2)
	{
		unk3C = c1;
		unk40 = c2;
	}

	void setUnk50(int a) { unk50 = a; }

	static char* mStrBuff;
	static bool mMutexInit;
	static bool mHeapFlag;
	static u32 mStrBuffSize;
	static u8 mBufferNotEnough;
	static OSMutex mMutex;

protected:
	/* 0x4 */ JUTFont* mFont;
	/* 0x8 */ JUtility::TColor unk8;
	/* 0xC */ JUtility::TColor unkC;
	/* 0x10 */ int unk10;
	/* 0x14 */ int unk14;
	/* 0x18 */ u8 unk18;
	/* 0x19 */ char pad19[3];
	/* 0x1C */ int unk1C;
	/* 0x20 */ int unk20;
	/* 0x24 */ float mCursorH;
	/* 0x28 */ float mCursorV;
	/* 0x2C */ float unk2C;
	/* 0x30 */ int unk30;
	/* 0x34 */ int unk34;
	/* 0x38 */ int unk38;
	/* 0x3C */ JUtility::TColor unk3C;
	/* 0x40 */ JUtility::TColor unk40;
	/* 0x44 */ JUtility::TColor mCharColor;
	/* 0x48 */ JUtility::TColor mGradColor;
	/* 0x4C */ int unk4C;
	/* 0x50 */ int unk50;
	/* 0x54 */ int unk54;
	/* 0x58 */ int mFontSizeX;
	/* 0x5C */ int mFontSizeY;
	/* 0x60 */ bool unk60;
};

#endif

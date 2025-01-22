#ifndef J2D_PRINT_HPP
#define J2D_PRINT_HPP

#include <JSystem/JUtility/JUTFont.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>

class J2DPrint {
public:
	struct TSize { };

	J2DPrint(JUTFont*, int);
	J2DPrint(JUTFont*, int, int);
	J2DPrint(JUTFont*, JUtility::TColor, JUtility::TColor);
	J2DPrint(JUTFont*, int, int, JUtility::TColor, JUtility::TColor);

	virtual ~J2DPrint();

	void initiate();
	void private_initiate(JUTFont*, int, int, JUtility::TColor,
	                      JUtility::TColor);
	void setBuffer(char*, unsigned long);
	void setBuffer(unsigned long);
	void setFontSize();
	void locate(int, int);
	void putChar(int);
	void putChar(int, int, int);
	void print(const char*, ...);
	void print(unsigned char, const char*, ...);
	void print(int, int, const char*, ...);
	void print(int, int, unsigned char, const char*, ...);
	void getSize(J2DPrint::TSize&, const char*, ...);
	void getWidth(const char*, ...);
	void getHeight(const char*, ...);
	void printReturn(const char*, int, int, J2DTextBoxHBinding,
	                 J2DTextBoxVBinding, int, int, unsigned char);
	void parse(const unsigned char*, int, int, unsigned short*,
	           J2DPrint::TSize&, unsigned char, bool);
	void doCtrlCode(int);
	void doEscapeCode(const unsigned char**, unsigned char);
	void initchar();
	void getNumber(const unsigned char**, long, long, int);

	void setFontSize(int sizeX, int sizeY)
	{
		mFontSizeX = sizeX > 0 ? sizeX : 0;
		mFontSizeY = sizeY > 0 ? sizeY : 0;
	}

public:
	/* 0x? */ int mFontSizeX;
	/* 0x? */ int mFontSizeY;
};

#endif

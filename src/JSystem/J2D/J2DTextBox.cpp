#include <JSystem/J2D/J2DTextBox.hpp>
#include <JSystem/J2D/J2DPrint.hpp>
#include <JSystem/JUtility/JUTFont.hpp>
#include <dolphin/gx.h>

J2DTextBox::J2DTextBox(const ResFONT* font, const char* str)
    : J2DPane()
    , mFont(nullptr)
    , mText(nullptr)
    , unk11C(-1)
    , unk120(-1)
{
	initiate(font, str, HBIND_LEFT, VBIND_TOP);
}

J2DTextBox::J2DTextBox(J2DPane*, JSURandomInputStream*, bool) { }

J2DTextBox::J2DTextBox(u32 tag, const JUTRect& bounds, const ResFONT* font,
                       const char* str, J2DTextBoxHBinding hbind,
                       J2DTextBoxVBinding vbind)
    : J2DPane(0x13, tag, bounds)
    , mFont(nullptr)
    , mText(nullptr)
    , unk11C(-1)
    , unk120(-1)
{
	(void)!font; // assert?
	initiate(font, str, hbind, vbind);
}

void J2DTextBox::initiate(const ResFONT* font, const char* str,
                          J2DTextBoxHBinding hbind, J2DTextBoxVBinding vbind)
{
	// TODO: JUTResFont
	// if (font != nullptr)
	// 	mpFont = new JUTResFont(font, nullptr);

	mCharColor.set(0xFFFFFFFF);
	mGradColor.set(0xFFFFFFFF);
	mHBinding   = hbind;
	mVBinding   = vbind;
	size_t temp = strlen(str);
	mText       = new char[temp + 1];
	strcpy(mText, str);
	unk104     = 0;
	unk108     = 0;
	mCharSpace = 0;
	if (mFont == nullptr) {
		mLineSpace = 0;
		mFontSizeX = 0;
		mFontSizeY = 0;
	} else {
		mLineSpace = mFont->getLeading();
		mFontSizeX = mFont->getWidth();
		mFontSizeY = mFont->getHeight();
	}
	mKind          = 'TBX1';
	mTextFontOwned = true;
}

J2DTextBox::~J2DTextBox()
{
	if (mTextFontOwned && mFont)
		delete mFont;

	if (mText)
		delete[] mText;
}

void J2DTextBox::setFont(JUTFont* font)
{
	if (!font)
		return;

	if (mTextFontOwned)
		delete mFont;

	mFont          = font;
	mTextFontOwned = false;
}

void J2DTextBox::draw(int x, int y)
{
	if (!mVisible)
		return;
	J2DPrint print(mFont, mCharSpace, mLineSpace, mCharColor, mGradColor);
	print.setFontSize(mFontSizeX, mFontSizeY);
	print.initiate();
	makeMatrix(x, y);
	GXLoadPosMtxImm(mGlobalMtx, 0);
	GXSetCurrentMtx(0);
	print.print(0, 0, mColorAlpha, "%s", mText);
	Mtx mtx;
	MTXIdentity(mtx);
	GXLoadPosMtxImm(mtx, 0);
}

const char* J2DTextBox::getStringPtr() const { return mText; }

size_t J2DTextBox::setString(const char* str, ...)
{
	va_list args;
	va_start(args, str);

	// They... they didn't implement the va args printf-ing...

	if (mText)
		delete[] mText;

	size_t sz = strlen(str);
	mText     = new char[sz + 1];
	strcpy(mText, str);

	va_end(args);
	return sz;
}

bool J2DTextBox::setConnectParent(bool connected)
{
	if (getPaneTree()->getParent() == nullptr)
		return false;
	if (getPaneTree()->getParent()->getObject()->mInfoTag != 17)
		return false;
	mConnected = connected;
	return connected;
}

void J2DTextBox::drawSelf(int x, int y)
{
	Mtx id;
	MTXIdentity(id);
	drawSelf(x, y, &id);
}

void J2DTextBox::drawSelf(int x, int y, Mtx* mtx)
{
	J2DPrint print(mFont, mCharSpace, mLineSpace, mCharColor, mGradColor);
	print.setFontSize(mFontSizeX, mFontSizeY);
	print.initiate();
	Mtx transform;
	MTXConcat(*mtx, mGlobalMtx, transform);
	GXLoadPosMtxImm(*mtx, 0);
	print.locate(x, y);
	print.printReturn(mText, mBounds.getWidth(), mBounds.getHeight(), mHBinding,
	                  mVBinding, unk104, unk108, mColorAlpha);
}

void J2DTextBox::resize(int w, int h)
{
	if (mConnected && getPaneTree() != nullptr
	    && getPaneTree()->getParent() != nullptr) {
		J2DPane* pane = getPaneTree()->getParent()->getObject();

		if (pane->mInfoTag == 0x11) {
			int newW = pane->getWidth() + (w - getWidth());
			int newH = pane->getHeight() + (h - getHeight());
			pane->resize(newW, newH);
			return;
		}
	}

	J2DPane::resize(w, h);
}

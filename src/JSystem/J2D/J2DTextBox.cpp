#include <JSystem/J2D/J2DTextBox.hpp>
#include <JSystem/J2D/J2DPrint.hpp>
#include <JSystem/JUtility/JUTResFont.hpp>
#include <JSystem/JUtility/JUTResource.hpp>
#include <JSystem/JSupport/JSURandomInputStream.hpp>
#include <dolphin/gx.h>

J2DTextBox::J2DTextBox(const ResFONT* font, const char* str)
    : J2DPane()
    , mFont(nullptr)
    , mText(nullptr)
{
	initiate(font, str, HBIND_LEFT, VBIND_TOP);
}

J2DTextBox::J2DTextBox(J2DPane* parent, JSURandomInputStream* stream,
                       bool is_ex)
    : J2DPane(parent, stream, is_ex)
    , mFont(nullptr)
    , mText(nullptr)
{
	JUTResReference res;
	mInfoTag = 0x13;
	if (is_ex) {
		u8 fields = stream->readU8();

		if (ResFONT* font = (ResFONT*)res.getResource(stream, 'FONT', nullptr))
			mFont = new JUTResFont(font, nullptr);

		mCharColor.set(stream->readU32());
		mGradColor.set(stream->readU32());
		u8 bindings = stream->readU8();
		mHBinding   = (J2DTextBoxHBinding)(bindings >> 2 & 3);
		mVBinding   = (J2DTextBoxVBinding)(bindings & 3);
		mCharSpace  = stream->readS16();
		mLineSpace  = stream->readS16();
		mFontSizeX  = stream->readU16();
		mFontSizeY  = stream->readU16();

		s16 len = stream->readU16();
		mText   = new char[len + 1];
		stream->read(mText, len);
		mText[len] = 0;

		fields -= 10;

		if (fields != 0) {
			if (stream->readU8()) {
				setConnectParent(true);
			}
			--fields;
		}

		mBlack = 0;
		mWhite = 0xffffffff;

		if (fields != 0) {
			mBlack.set(stream->readU32());
			--fields;
		}
		if (fields != 0) {
			mWhite.set(stream->readU32());
		}
		unk104 = 0;
		unk108 = 0;
		stream->align(4);
	} else {
		if (ResFONT* font = (ResFONT*)res.getResource(stream, 'FONT', nullptr))
			mFont = new JUTResFont(font, nullptr);

		mCharColor.set(stream->readU32());
		mGradColor.set(stream->readU32());

		u8 flags  = stream->readU8();
		mHBinding = (J2DTextBoxHBinding)(flags & 0x7f);
		mVBinding = (J2DTextBoxVBinding)(stream->readU8());
		if (!mFont) {
			mCharSpace = 0;
			mLineSpace = 0;
			mFontSizeX = 0;
			mFontSizeY = 0;
		} else {
			if ((flags & 0x80) != 0) {
				mCharSpace = stream->readS16();
				mLineSpace = stream->readS16();
				mFontSizeX = stream->readU16();
				mFontSizeY = stream->readU16();
			} else {
				mCharSpace = 0;
				mLineSpace = mFont->getLeading();
				mFontSizeX = mFont->getWidth();
				mFontSizeY = mFont->getHeight();
			}
		}

		s16 len = stream->readS16();
		mText   = new char[len + 1];
		stream->read(mText, len);
		mText[len] = 0;

		mBlack = 0;
		mWhite = 0xffffffff;
		unk104 = 0;
		unk108 = 0;

		stream->align(4);
	}
	mTextFontOwned = true;
}

J2DTextBox::J2DTextBox(u32 tag, const JUTRect& bounds, const ResFONT* font,
                       const char* str, J2DTextBoxHBinding hbind,
                       J2DTextBoxVBinding vbind)
    : J2DPane(0x13, tag, bounds)
    , mFont(nullptr)
    , mText(nullptr)
{
	initiate(font, str, hbind, vbind);
}

void J2DTextBox::initiate(const ResFONT* font, const char* str,
                          J2DTextBoxHBinding hbind, J2DTextBoxVBinding vbind)
{
	if (font != nullptr)
		mFont = new JUTResFont(font, nullptr);

	mCharColor.set(0xFFFFFFFF);
	mGradColor.set(0xFFFFFFFF);
	mBlack      = 0;
	mWhite      = 0xFFFFFFFF;
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
	print.setSomeColors(mBlack, mWhite);
	print.initiate();
	makeMatrix(x, y);
	GXLoadPosMtxImm(mPositionMtx, 0);
	GXSetCurrentMtx(0);
	print.print(0, 0, mColorAlpha, "%s", mText);
	Mtx mtx;
	MTXIdentity(mtx);
	GXLoadPosMtxImm(mtx, 0);
}

char* J2DTextBox::getStringPtr() const { return mText; }

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
	print.setSomeColors(mBlack, mWhite);
	print.initiate();
	Mtx transform;
	MTXConcat(*mtx, mGlobalMtx, transform);
	GXLoadPosMtxImm(transform, 0);
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

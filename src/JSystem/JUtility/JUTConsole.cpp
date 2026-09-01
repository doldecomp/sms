#include <JSystem/JUtility/JUTConsole.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JUtility/JUTAssert.hpp>
#include <JSystem/JUtility/JUTDirectPrint.hpp>
#include <JSystem/JUtility/JUTVideo.hpp>
#include <dolphin/vi.h>
#include <stdio.h>

JUTConsoleManager* JUTConsoleManager::sManager;

JUTConsole* JUTConsole::create(unsigned param_0, unsigned param_1,
                               JKRHeap* pHeap)
{
}

JUTConsole* JUTConsole::create(unsigned param_0, void* buffer, u32 bufferSize)
{
	JUTConsoleManager* pManager = JUTConsoleManager::getManager();

	u32 maxLines = getLineFromObjectSize(bufferSize, param_0);

	JUTConsole* newConsole = new (buffer) JUTConsole(param_0, maxLines, false);
	newConsole->mBuf       = (u8*)buffer + sizeof(JUTConsole);
	newConsole->clear();

	pManager->appendConsole(newConsole);
	return newConsole;
}

void JUTConsole::destroy(JUTConsole* console) { }

JUTConsole::JUTConsole(unsigned param_0, unsigned maxLines, bool param_2)
{
	field_0x2c = param_2;
	field_0x20 = param_0;
	mMaxLines  = maxLines;

	mPositionX = 30;
	mPositionY = 50;
	mHeight    = 20;

	if (mHeight > mMaxLines) {
		mHeight = mMaxLines;
	}

	mFont      = nullptr;
	mVisible   = true;
	field_0x65 = false;
	field_0x66 = false;
	mOutput    = 1;

	field_0x5c = JUtility::TColor(0, 0, 0, 100);
	field_0x60 = JUtility::TColor(0, 0, 0, 230);
}

JUTConsole::~JUTConsole()
{
	JUTConsoleManager::getManager()->removeConsole(this);
}

size_t JUTConsole::getObjectSizeFromBufferSize(unsigned param_0,
                                               unsigned param_1)
{
}

size_t JUTConsole::getLineFromObjectSize(u32 bufferSize, unsigned param_1)
{
	return (bufferSize - sizeof(JUTConsole)) / (param_1 + 2);
}

void JUTConsole::clear()
{
	field_0x30 = 0;
	field_0x34 = 0;
	field_0x38 = 0;
	field_0x3c = 0;

	for (u32 i = 0; i < mMaxLines; i++) {
		setLineAttr(i, 0);
	}
	setLineAttr(0, -1);
	*getLinePtr(0) = 0;
}

void JUTConsole::doDraw(JUTConsole::EConsoleType consoleType) const
{
	f32 font_yOffset;

	if (mVisible && (mFont != nullptr || consoleType == CONSOLE_TYPE_2)) {
		if (mHeight != 0) {
			bool temp_r30 = consoleType == ACTIVE ? true : false;
			int spA4      = temp_r30 ? 1 : 0;
			spA4          = 0;
			font_yOffset  = 2.0f + mFontSizeY;

			if (consoleType != CONSOLE_TYPE_2) {
				if (JUTGetVideoManager() == nullptr) {
					J2DOrthoGraph ortho(0, 0, 640, 480);
					ortho.setPort();
				} else {
					J2DOrthoGraph ortho(
					    0, 0, JUTGetVideoManager()->getRenderMode()->fbWidth,
					    JUTGetVideoManager()->getRenderMode()->efbHeight);
					ortho.setPort();
				}

				J2DFillBox(mPositionX - 2, (int)(mPositionY - font_yOffset),
				           (int)((mFontSizeX * field_0x20) + 4.0f),
				           (int)(font_yOffset * (mHeight + spA4)),
				           temp_r30 ? field_0x60 : field_0x5c);
				mFont->setGX();

				if (temp_r30) {
					if (diffIndex(field_0x30, field_0x38) - (int)mHeight + 1
					    <= 0) {
						mFont->setCharColor(
						    JUtility::TColor(255, 255, 255, 255));
					} else if (field_0x30 == field_0x34) {
						mFont->setCharColor(
						    JUtility::TColor(255, 230, 230, 255));
					} else {
						mFont->setCharColor(
						    JUtility::TColor(230, 230, 255, 255));
					}
				} else {
					mFont->setCharColor(JUtility::TColor(230, 230, 230, 255));
				}
			} else {
				JUTDirectPrint::getManager()->erase(
				    mPositionX - 3, mPositionY - 2, (field_0x20 * 6) + 6,
				    (int)(font_yOffset * mHeight) + 4);
			}

			s32 curLine = field_0x30;
			s32 nextLine;
			s32 y = 0;
			u8* linePtr;

			do {
				linePtr = getLinePtr(curLine);

				u8 c = (u8)linePtr[-1];
				if (c == nullptr)
					break;

				if (consoleType != CONSOLE_TYPE_2) {
					mFont->drawString_scale(
					    mPositionX, ((y * font_yOffset) + mPositionY),
					    mFontSizeX, mFontSizeY, (char*)linePtr, true);
				} else {
					JUTDirectPrint::getManager()->drawString(
					    mPositionX, ((y * font_yOffset) + mPositionY),
					    (char*)linePtr);
				}

				nextLine = nextIndex(curLine);
				curLine  = nextLine;
				y++;
			} while (y < mHeight && nextLine != field_0x34);

			// Dead-stripped code copied from TP debug.
			if (spA4 != 0) {
				f32 f31  = mPositionX;
				int sp94 = mHeight * font_yOffset + mPositionY;
				mFont->setCharColor(
				    JUtility::TColor(0xff, mVisible ? 0xff : 200, 0xc8, 0xff));
				mFont->drawString_scale((int)f31, sp94, mFontSizeX, mFontSizeY,
				                        "X", true);
				f31 += mFontSizeX;
				mFont->drawString_scale((int)f31, sp94, mFontSizeX, mFontSizeY,
				                        mVisible ? "[ON]" : "[OFF]", true);
				f31 += (int)(mFontSizeX * 6.0f);
				if (this == nullptr) {
					mFont->setCharColor(
					    JUtility::TColor(0xff, 0xff, 0x64, 0xff));
					mFont->drawString_scale((int)(f31 - mFontSizeX), mFontSizeX,
					                        mFontSizeY, sp94, "*", true);
				}
				mFont->setCharColor(JUtility::TColor(0xc8, 0xc8, 0xc8, 0xff));
				char spA8[] = "S----------E";
				char spB8[0x20];
				int sp90 = diffIndex(field_0x34, field_0x38) + 1;
				int sp8C = diffIndex(field_0x34, field_0x30);
				int sp88 = 0;
				int sp84;
				if (sp90 <= (int)mHeight) {
					sp84 = 9;
					sp88 = 1;
				} else {
					sp84 = (sp8C * 9) / (int)(sp90 - mHeight);
				}
				spA8[sp84 + 1] = 'O';
				mFont->drawString_scale((int)f31, sp94, mFontSizeX, mFontSizeY,
				                        spA8, true);
				f31 += mFontSizeX * 13.0f;
				if (sp88) {
					sprintf(spB8, "ALL");
				} else {
					f32 f29 = sp8C / (f32)(sp90 - mHeight);
					sprintf(spB8, "%3d%%(%dL)", (int)(100.0 * f29), sp90);
				}
				mFont->drawString_scale(f31, sp94, mFontSizeX, mFontSizeY, spB8,
				                        true);

				// Completely fake code. Everything above is from TP, but SMS
				// reserves 0xc more bytes of stack for temporaries somewhere
				// in this function.
				JUtility::TColor(0, 0, 0, 0);
				mFont->drawString_scale(0, 0, 0, 0, nullptr, false);
			}
		}
	}
}

void JUTConsole::print_f(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	JUTConsole_print_f_va_(this, fmt, args);
	va_end(args);
}

void JUTConsole::print(const char* param_0)
{
	if (!(mOutput & 1))
		return;

	const char* r29 = param_0;
	u8* r28         = getLinePtr(field_0x38) + field_0x3c;
	while (*r29) {
		if (field_0x66 && field_0x34 == nextIndex(field_0x38))
			break;

		if (*r29 == '\n') {
			r29++;
			field_0x3c = field_0x20;
		} else if (*r29 == '\t') {
			r29++;
			while (field_0x3c < field_0x20) {
				*(r28++) = ' ';
				field_0x3c++;
				if (field_0x3c % 8 == 0)
					break;
			}
		} else {
			*(r28++) = *(r29++);
			field_0x3c++;
		}

		if (field_0x3c < field_0x20)
			continue;

		*r28       = 0;
		field_0x38 = nextIndex(field_0x38);
		field_0x3c = 0;
		setLineAttr(field_0x38, 0xff);
		r28          = getLinePtr(field_0x38);
		*r28         = 0;
		int local_28 = diffIndex(field_0x30, field_0x38);

		if (local_28 == mHeight)
			field_0x30 = nextIndex(field_0x30);

		if (field_0x38 == field_0x34)
			field_0x34 = nextIndex(field_0x34);

		if (field_0x38 == field_0x30)
			field_0x30 = nextIndex(field_0x30);
	}
	*r28 = 0;
}

void JUTConsole_print_f_va_(JUTConsole* console, const char* fmt, va_list args)
{
	char buf[1024];
	int sz = vsnprintf(buf, sizeof(buf), fmt, args);
	console->print(buf);
}

void JUTConsole::dumpToTerminal(unsigned param_0) { }

void JUTConsole::dumpToConsole(JUTConsole* console, unsigned param_1) { }

void JUTConsole::scroll(int scrollAmnt)
{
	if (scrollAmnt < 0) {
		int diff = diffIndex(field_0x34, field_0x30);
		if (scrollAmnt < -diff) {
			scrollAmnt = -diff;
		}
	} else {
		if (scrollAmnt > 0) {
			int diff = diffIndex(field_0x34, field_0x38);
			if (diff + 1 <= mHeight) {
				scrollAmnt = 0;
			} else {
				diff = diffIndex(field_0x30, field_0x38);
				if (scrollAmnt > (int)(diff - mHeight) + 1) {
					scrollAmnt = (int)(diff - mHeight) + 1;
				}
			}
		}
	}

	field_0x30 += scrollAmnt;
	if (field_0x30 < 0) {
		field_0x30 += mMaxLines;
	}

	if (field_0x30 >= (u32)mMaxLines) {
		field_0x30 -= mMaxLines;
	}
}

int JUTConsole::getUsedLine() const
{
	return diffIndex(field_0x34, field_0x38);
}

int JUTConsole::getLineOffset() const
{
	return diffIndex(field_0x34, field_0x30);
}

JUTConsoleManager::JUTConsoleManager()
{
	mActiveConsole = nullptr;
	mDirectConsole = nullptr;
}

JUTConsoleManager::~JUTConsoleManager() { }

JUTConsoleManager* JUTConsoleManager::createManager(JKRHeap* pHeap)
{
	if (pHeap == nullptr) {
		pHeap = JKRHeap::sCurrentHeap;
	}

	JUTConsoleManager* manager = new (pHeap, 0) JUTConsoleManager();
	sManager                   = manager;
	return manager;
}

void JUTConsoleManager::destroyManager(JUTConsoleManager* manager) { }

void JUTConsoleManager::appendConsole(JUTConsole* console)
{
	JUT_ASSERT(961, sManager != NULL && console != NULL);
	JUT_ASSERT(964, soLink_.Find(console) == soLink_.end());

	soLink_.Push_back(console);

	if (mActiveConsole == nullptr)
		mActiveConsole = console;
}

void JUTConsoleManager::removeConsole(JUTConsole* console)
{
	if (mActiveConsole == console) {
		if (soLink_.size() <= 1) {
			mActiveConsole = nullptr;
		} else {
			mActiveConsole
			    = console != &soLink_.back()
			          ? soLink_.Element_getValue(console->mLinkNode.getNext())
			          : &soLink_.front();
		}
	}

	if (JUTGetWarningConsole() == console)
		JUTSetWarningConsole(nullptr);
	if (JUTGetReportConsole() == console)
		JUTSetReportConsole(nullptr);

	soLink_.Remove(console);
}

void JUTConsoleManager::getConsoleNumber() const { }

void JUTConsoleManager::draw() const
{
	ConsoleList::const_iterator iter = soLink_.begin();
	ConsoleList::const_iterator end  = soLink_.end();

	for (; iter != end; ++iter) {
		const JUTConsole* const console = &(*iter);
		if (console != mActiveConsole)
			console->doDraw(JUTConsole::INACTIVE);
	}

	if (mActiveConsole != nullptr)
		mActiveConsole->doDraw(JUTConsole::ACTIVE);
}

void JUTConsoleManager::drawDirect(bool waitRetrace) const
{
	if (mDirectConsole != nullptr) {
		if (waitRetrace == true) {
			s32 interrupt_status = OSEnableInterrupts();
			u32 retrace_count    = VIGetRetraceCount();
			u32 new_count;
			do {
				new_count = VIGetRetraceCount();
			} while (retrace_count == new_count);
			OSRestoreInterrupts(interrupt_status);
		}
		mDirectConsole->doDraw(JUTConsole::CONSOLE_TYPE_2);
	}
}

void JUTConsoleManager::setDirectConsole(JUTConsole* p_console)
{
	if (mDirectConsole != nullptr) {
		appendConsole(mDirectConsole);
	}

	if (p_console != nullptr) {
		removeConsole(p_console);
	}
	mDirectConsole = p_console;
}

static JUTConsole* sReportConsole;

void JUTSetReportConsole(JUTConsole* p_console) { sReportConsole = p_console; }

JUTConsole* JUTGetReportConsole() { return sReportConsole; }

static JUTConsole* sWarningConsole;

void JUTSetWarningConsole(JUTConsole* p_console)
{
	sWarningConsole = p_console;
}

JUTConsole* JUTGetWarningConsole() { return sWarningConsole; }

void JUTReportConsole_f_va(const char* fmt, va_list args)
{
	char buf[256];

	if (JUTGetReportConsole() == nullptr) {
		vsnprintf(buf, sizeof(buf), fmt, args);
	} else if (JUTGetReportConsole()->getOutput()
	           & (JUTConsole::OUTPUT_CONSOLE | JUTConsole::OUTPUT_OSREPORT)) {
		vsnprintf(buf, sizeof(buf), fmt, args);
		JUTGetReportConsole()->print(buf);
	}
}

void JUTReportConsole_f(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	JUTReportConsole_f_va(fmt, args);
	va_end(args);
	char trash[0x4];
}

void JUTReportConsole(const char* message)
{
	JUTReportConsole_f("%s", message);
}

void JUTWarningConsole_f_va(const char* fmt, va_list args)
{
	char buf[256];

	if (JUTGetWarningConsole() == nullptr) {
		vsnprintf(buf, sizeof(buf), fmt, args);
		OSReport("%s", buf);
	} else if (JUTGetWarningConsole()->getOutput()
	           & (JUTConsole::OUTPUT_CONSOLE | JUTConsole::OUTPUT_OSREPORT)) {
		vsnprintf(buf, sizeof(buf), fmt, args);
		JUTGetWarningConsole()->print(buf);
	}
}

void JUTWarningConsole_f(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	JUTReportConsole_f_va(fmt, args);
	va_end(args);
	char trash[0x4];
}

void JUTWarningConsole(const char* message)
{
	JUTReportConsole_f("%s", message);
}

#include <JSystem/JUtility/JUTConsole.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JUtility/JUTAssert.hpp>
#include <JSystem/JUtility/JUTDirectPrint.hpp>
#include <JSystem/JUtility/JUTVideo.hpp>
#include <dolphin/vi.h>
#include <stdio.h>

JUTConsoleManager* JUTConsoleManager::sManager;

JUTConsole* JUTConsole::create(unsigned param_0, unsigned maxLines,
                               JKRHeap* p_heap)
{
	JUTConsoleManager* pManager = JUTConsoleManager::getManager();
	JUT_ASSERT(33, pManager != nullptr);

	void* buffer = JKRAllocFromHeap(
	    p_heap, getObjectSizeFromBufferSize(param_0, maxLines), 0);
	u8* tmpBuf = (u8*)buffer;

	JUTConsole* newConsole = new (tmpBuf) JUTConsole(param_0, maxLines, true);
	newConsole->mBuf       = tmpBuf + sizeof(JUTConsole);
	newConsole->clear();

	pManager->appendConsole(newConsole);
	return newConsole;
}

JUTConsole* JUTConsole::create(unsigned param_0, void* buffer, u32 bufferSize)
{
	JUTConsoleManager* pManager = JUTConsoleManager::getManager();
	JUT_ASSERT(59, pManager != nullptr);
	JUT_ASSERT(62, ((u32)buffer & 0x3) == 0);
	u32 maxLines = getLineFromObjectSize(bufferSize, param_0);

	JUTConsole* newConsole = new (buffer) JUTConsole(param_0, maxLines, false);
	newConsole->mBuf       = (u8*)buffer + sizeof(JUTConsole);
	newConsole->clear();

	pManager->appendConsole(newConsole);
	return newConsole;
}

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

	field_0x5c.set(0, 0, 0, 100);
	field_0x60.set(0, 0, 0, 230);
}

static void dummy1() { OSReport("console != 0"); }

JUTConsole::~JUTConsole()
{
	JUT_ASSERT(150, JUTConsoleManager::getManager());
	JUTConsoleManager::getManager()->removeConsole(this);
}

size_t JUTConsole::getObjectSizeFromBufferSize(unsigned param_0,
                                               unsigned maxLines)
{
	return (param_0 + 2) * maxLines + sizeof(JUTConsole);
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
			bool temp_r30 = consoleType == ACTIVE;
			font_yOffset  = 2.0f + mFontSizeY;

			if (consoleType != CONSOLE_TYPE_2) {
				// if (JUTGetVideoManager() == nullptr) {
				//     J2DOrthoGraph ortho(0.0f, 0.0f, 640.0f, 480.0f,
				//     -1.0f, 1.0f); ortho.setPort();
				// } else {
				//     JUTVideo * pVideo = JUTGetVideoManager();
				//     J2DOrthoGraph ortho(0.0f, 0.0f, pVideo->getFbWidth(),
				//     pVideo->getEfbHeight(), -1.0f, 1.0f); ortho.setPort();
				// }

				const JUtility::TColor* color;
				if (temp_r30) {
					color = &field_0x60;
				} else {
					color = &field_0x5c;
				}

				J2DFillBox(mPositionX - 2, (int)(mPositionY - font_yOffset),
				           (int)((mFontSizeX * field_0x20) + 4.0f),
				           (int)(font_yOffset * mHeight), *color);
				mFont->setGX();

				if (temp_r30) {
					s32 s = (diffIndex(field_0x30, field_0x38) - mHeight) + 1;
					if (s <= 0) {
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

			char* linePtr;
			s32 curLine = field_0x30;
			s32 y       = 0;

			do {
				linePtr = (char*)getLinePtr(curLine);

				if ((u8)linePtr[-1] != nullptr) {
					if (consoleType != CONSOLE_TYPE_2) {
						mFont->drawString_scale(
						    mPositionX, ((y * font_yOffset) + mPositionY),
						    mFontSizeX, mFontSizeY, linePtr, true);
					} else {
						JUTDirectPrint::getManager()->drawString(
						    mPositionX, ((y * font_yOffset) + mPositionY),
						    linePtr);
					}

					curLine = nextIndex(curLine);
					y++;
				} else {
					break;
				}
			} while (y < mHeight && curLine != field_0x34);
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
	if (mOutput & 2) {
		OSReport("%s", param_0);
	}
	if (mOutput & 1) {
		const u8* r29 = (const u8*)param_0;
		u8* r28       = getLinePtr(field_0x38) + field_0x3c;
		while (*r29) {
			if (field_0x66 && field_0x34 == nextIndex(field_0x38)) {
				break;
			}
			if (*r29 == '\n') {
				r29++;
				field_0x3c = field_0x20;
			} else if (*r29 == '\t') {
				r29++;
				while (field_0x3c < field_0x20) {
					*(r28++) = ' ';
					field_0x3c++;
					if (field_0x3c % 8 == 0) {
						break;
					}
				}
			} else if (mFont && mFont->isLeadByte(*r29)) {
				if (field_0x3c + 1 < field_0x20) {
					*(r28++) = *(r29++);
					*(r28++) = *(r29++);
					field_0x3c++;
					field_0x3c++;
				} else {
					*(r28++) = 0;
					field_0x3c++;
				}
			} else {
				*(r28++) = *(r29++);
				field_0x3c++;
			}

			if (field_0x3c < field_0x20) {
				continue;
			}
			*r28       = 0;
			field_0x38 = nextIndex(field_0x38);
			field_0x3c = 0;
			setLineAttr(field_0x38, 0xff);
			r28          = getLinePtr(field_0x38);
			*r28         = 0;
			int local_28 = diffIndex(field_0x30, field_0x38);
			if (local_28 == mHeight) {
				field_0x30 = nextIndex(field_0x30);
			}
			if (field_0x38 == field_0x34) {
				field_0x34 = nextIndex(field_0x34);
			}
			if (field_0x38 == field_0x30) {
				field_0x30 = nextIndex(field_0x30);
			}
		}
		*r28 = 0;
	}
}

void JUTConsole_print_f_va_(JUTConsole* console, const char* fmt, va_list args)
{
	JUT_ASSERT(561, console != 0);
	char buf[1024];
	vsnprintf(buf, sizeof(buf), fmt, args);
	console->print(buf);
}

void JUTConsole::dumpToTerminal(unsigned param_0)
{
	if (param_0 == 0) {
		return;
	}

	s32 r29 = field_0x34;
	if (param_0 != -1) {
		r29 = field_0x38;
		for (int i = 0; i != param_0; i++) {
			int r25 = prevIndex(r29);
			if (getLineAttr(r25) == 0) {
				break;
			}
			r29 = r25;
			if (r25 == field_0x34) {
				break;
			}
		}
	}

	s32 i = 0;
	OSReport("\n:::dump of console[%x]--------------------------------\n",
	         this);
	while (true) {
		u8* r28 = getLinePtr(r29);
		u8 r24  = r28[-1];
		if (r24 == 0) {
			break;
		}
		if (field_0x65) {
			OSReport("[%03d] %s\n", i, r28);
		} else {
			OSReport("%s\n", r28);
		}
		r29 = nextIndex(r29);
		i++;
		if (r29 == field_0x34)
			break;
	}
	OSReport(":::dump of console[%x] END----------------------------\n", this);
}

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

static void dummy2()
{
	OSReport("console != this && console != 0");
	OSReport("\n:::dump of console[%x]----------------\n");
	OSReport(":::dump of console[%x] END------------\n");
}

JUTConsoleManager* JUTConsoleManager::createManager(JKRHeap* pHeap)
{
	JUT_ASSERT(922, sManager == nullptr);
	if (pHeap == nullptr) {
		pHeap = JKRHeap::sCurrentHeap;
	}

	JUTConsoleManager* manager = new (pHeap, 0) JUTConsoleManager();
	sManager                   = manager;
	return manager;
}

static void dummy3()
{
	OSReport("consoleManager != 0 && sManager == consoleManager");
}

void JUTConsoleManager::appendConsole(JUTConsole* console)
{
	JUT_ASSERT(0x3bf, sManager != nullptr && console != nullptr);

	JUT_ASSERT(0x3c2, soLink_.Find(console) == soLink_.end());

	soLink_.Push_back(console);

	if (mActiveConsole == nullptr)
		mActiveConsole = console;
}

void JUTConsoleManager::removeConsole(JUTConsole* console)
{
	JUT_ASSERT(0x3d6, sManager != nullptr && console != nullptr);
	JUT_ASSERT(0x3d9, soLink_.Find(console) != soLink_.end());

	if (mActiveConsole == console) {
		if (soLink_.size() <= 1) {
			mActiveConsole = nullptr;
		} else {
			mActiveConsole
			    = console != &soLink_.back()
			          ? soLink_.Element_toValue(console->mLinkNode.getNext())
			          : &soLink_.front();
		}
	}

	if (JUTGetWarningConsole() == console)
		JUTSetWarningConsole(nullptr);
	if (JUTGetReportConsole() == console)
		JUTSetReportConsole(nullptr);

	soLink_.Remove(console);
}

void JUTConsoleManager::draw() const
{
	ConsoleList::const_iterator iter = soLink_.begin();
	ConsoleList::const_iterator end  = soLink_.end();

	for (; iter != end; ++iter) {
		const JUTConsole* const pConsole = &(*iter);
		if (pConsole != mActiveConsole)
			pConsole->doDraw(JUTConsole::INACTIVE);
	}

	if (mActiveConsole != nullptr)
		mActiveConsole->doDraw(JUTConsole::ACTIVE);
}

void JUTConsoleManager::drawDirect(bool waitRetrace) const
{
	if (mDirectConsole != nullptr) {
		if (waitRetrace) {
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
		OSReport("%s", buf);
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
}

void JUTWarningConsole(const char* message)
{
	JUTReportConsole_f("%s", message);
}

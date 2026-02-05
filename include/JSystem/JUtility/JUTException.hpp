#ifndef JUT_JUTEXCEPTION_HPP
#define JUT_JUTEXCEPTION_HPP

#include <JSystem/JKernel/JKRThread.hpp>
#include <JSystem/JUtility/JUTGamePad.hpp>
#include <stdarg.h>
#include <dolphin/gx/GXEnum.h>
#include <dolphin/os/OSError.h>

typedef struct _GXRenderModeObj GXRenderModeObj;
typedef struct OSContext OSContext;
class JUTDirectPrint;

class JUTExternalFB {
public:
	JUTExternalFB(GXRenderModeObj*, GXGamma, void*, u32);

private:
	/* 0x00 */ _GXRenderModeObj* mRenderMode;
	/* 0x04 */ u32 mSize;
	/* 0x08 */ u32 field_0x08;
	/* 0x0C */ u16 field_0x0C;
	/* 0x0E */ u16 mGamma;
	/* 0x10 */ bool field_0x10;
	/* 0x11 */ u8 field_[3];
};

#define JUT_PRINT_GPR     1
#define JUT_PRINT_GPR_MAP 2
#define JUT_PRINT_FLOAT   4
#define JUT_PRINT_STACK   8

class JUTException : public JKRThread {
public:
	enum EInfoPage {
		EINFO_PAGE_GPR     = 1,
		EINFO_PAGE_FLOAT   = 2,
		EINFO_PAGE_STACK   = 3,
		EINFO_PAGE_GPR_MAP = 4,
	};

	class JUTExMapFile {
	public:
		JUTExMapFile(char* path)
		    : mLink(this)
		{
			mPath = path;
		}

	public:
		/* 0x00 */ char* mPath;
		/* 0x04 */ JSULink<JUTExMapFile> mLink;
		/* 0x14 */
	};

	JUTException(JUTDirectPrint*);

	void showFloatSub(int, f32);
	void showFloat(OSContext*);
	void showStack(OSContext*);
	void showMainInfo(u16, OSContext*, u32, u32);
	void showGPR(OSContext*);
	bool showMapInfo_subroutine(u32, bool);
	void showGPRMap(OSContext*);
	void printDebugInfo(JUTException::EInfoPage, OSError, OSContext*, u32, u32);
	bool isEnablePad() const;
	bool readPad(u32*, u32*);
	void printContext(u16, OSContext*, u32, u32);
	void createFB();

	/* vt[03] */ virtual void* run();

	static JUTException* create(JUTDirectPrint*);
	static void errorHandler(OSError, OSContext*, u32, u32);
	static u32 getFpscr();
	static bool searchPartialModule(u32, u32*, u32*, u32*, u32*);
	static OSErrorHandler setPreUserCallback(OSErrorHandler);
	static OSErrorHandler setPostUserCallback(OSErrorHandler);
	static void appendMapFile(char*);
	static bool queryMapAddress(char*, u32, s32, u32*, u32*, char*, u32, bool,
	                            bool);
	static bool queryMapAddress_single(char*, u32, s32, u32*, u32*, char*, u32,
	                                   bool, bool);
	static void createConsole(void*, u32);
	static void waitTime(s32);

	static JUTException* getManager() { return sErrorManager; }
	static JUTConsole* getConsole() { return sConsole; }

	JUTExternalFB* getFrameMemory() const { return mFrameMemory; }

	void setTraceSuppress(u32 param_0) { mTraceSuppress = param_0; }
	void setGamePad(JUTGamePad* gamePad)
	{
		mGamePad     = gamePad;
		mGamePadPort = JUTGamePad::EPortInvalid;
	}

	static void setMapFile(char* map) { appendMapFile(map); }

	// fabricated
	void setPrintFlags(u32 flags) { mPrintFlags = flags; }

private:
	static OSMessageQueue sMessageQueue;
	static const char* sCpuExpName[16];
	static JSUList<JUTException::JUTExMapFile> sMapFileList;
	static OSMessage sMessageBuffer[1];
	static JUTException* sErrorManager;
	static OSErrorHandler sPreUserCallback;
	static OSErrorHandler sPostUserCallback;
	static void* sConsoleBuffer;
	static u32 sConsoleBufferSize;
	static JUTConsole* sConsole;
	static u32 msr;
	static u32 fpscr;

private:
	/* 0x60 */ JUTExternalFB* mFrameMemory;
	/* 0x64 */ JUTDirectPrint* mDirectPrint;
	/* 0x68 */ JUTGamePad* mGamePad;
	/* 0x6C */ JUTGamePad::EPadPort mGamePadPort;
	/* 0x70 */ s32 mPrintWaitTime0;
	/* 0x74 */ s32 mPrintWaitTime1;
	/* 0x78 */ u32 mTraceSuppress;
	/* 0x7C */ u32 field_0x98;
	/* 0x80 */ u32 mPrintFlags;
};

struct JUTWarn {
	JUTWarn& operator<<(const char*) { return *this; }
	JUTWarn& operator<<(long) { return *this; }
};

#endif

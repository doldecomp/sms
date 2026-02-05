#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <System/GameSequence.hpp>
#include <dolphin/types.h>

namespace JDrama {
class TDisplay;
}

class JKRMemArchive;
class JKRHeap;
class TDirector;
class TMarioGamePad;
class TSMSFader;
class JKRExpHeap;
class TProcessMeter;
class JKRArchive;
class JUTFont;

f32 SMSGetVSyncTimesPerSec();
f32 SMSGetAnmFrameRate();
void* SMSLoadArchive(const char*, void*, u32, JKRHeap*);

class TARAMBlock { };
void SMSLoadArchiveARAM(TARAMBlock*, const char*);
void SMSMountAramArchive(JKRMemArchive*, TARAMBlock&);
JKRArchive* SMSSwitch2DArchive(const char*, TARAMBlock&);
extern TARAMBlock gArBkConsole;
extern JUTFont* gpSystemFont;

class TApplication {
public:
	TApplication();
	void setupThreadFuncBoot();
	void initialize();
	void setupThreadFuncLogo();
	void initialize_bootAfter();
	void initialize_nlogoAfter();
	void initialize_processMeter();
	void finalize();
	void checkAdditionalMovie();
	void proc();
	void gameLoop();
	void drawDVDErr();
	JKRMemArchive* mountStageArchive();

public:
	/* 0x00 */ TApplication* mSelf;
	/* 0x04 */ TDirector* mDirector;
	/* 0x08 */ char unk8;
	/* 0x0A */ TGameSequence mPrevArea;
	/* 0x0E */ TGameSequence mCurrArea;
	/* 0x12 */ TGameSequence mNextArea;
	/* 0x18 */ int mMovie;
	/* 0x1C */ JDrama::TDisplay* mDisplay;
	/* 0x20 */ TMarioGamePad* mGamePads[4];
	/* 0x30 */ TMarioGamePad* unk30;
	/* 0x34 */ TSMSFader* mFader;
	/* 0x38 */ char mSaveFile;
	/* 0x3C */ u32 unk3C;
	/* 0x40 */ JKRExpHeap* mHeap;
	/* 0x44 */ u16 unk44;
	/* 0x46 */ u16 unk46;
	/* 0x48 */ TProcessMeter* mMeter;
};

extern TApplication gpApplication;

class TCardManager;
extern TCardManager* gpCardManager;

#endif

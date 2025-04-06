#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <System/GameSequence.hpp>
#include <dolphin/types.h>

class JKRMemArchive;
class JKRHeap;
class TARAMBlock;
class TDirector;
class TDisplay;
class TMarioGamePad;
class TSMSFader;
class JKRExpHeap;
class TProcessMeter;

void SMSGetVSyncTimesPerSec();
f32 SMSGetAnmFrameRate();
void SMSLoadArchive(const char*, void*, u32, JKRHeap*);
void SMSLoadArchiveARAM(TARAMBlock*, const char*);
void SMSMountAramArchive(JKRMemArchive*, TARAMBlock&);
void SMSSwitch2DArchive(const char*, TARAMBlock&);

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
	void mountStageArchive();

public:
	/* 0x00 */ TApplication* self;
	/* 0x04 */ TDirector* director;
	/* 0x08 */ char undefined1;
	/* 0x0A */ TGameSequence prevArea;
	/* 0x0E */ TGameSequence currArea;
	/* 0x12 */ TGameSequence nextArea;
	/* 0x18 */ int movie;
	/* 0x1C */ TDisplay* display;
	/* 0x20 */ TMarioGamePad* gamePads[4];
	/* 0x30 */ char undefined4[4];
	/* 0x34 */ TSMSFader* unkFader;
	/* 0x38 */ char saveFile;
	/* 0x3C */ char undefined4_2[4];
	/* 0x40 */ JKRExpHeap* heap;
	/* 0x44 */ char undefined2[2];
	/* 0x46 */ char undefined2_2[2];
	/* 0x48 */ TProcessMeter* meter;
};

extern TApplication gpApplication;

#endif

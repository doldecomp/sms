#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <dolphin/types.h>

class JKRMemArchive;
class JKRHeap;
class TARAMBlock;

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
};

#endif

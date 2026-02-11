#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <Strategic/NameRefAry.hpp>
#include <Strategic/NameRefPtrAry.hpp>
#include <System/GameSequence.hpp>
#include <dolphin/types.h>

namespace JDrama {
class TDisplay;
class TDirector;
} // namespace JDrama

class JKRMemArchive;
class JKRHeap;
class TMarioGamePad;
class TSMSFader;
class TScenarioArchiveName;
class TProcessMeter;
class JKRAramBlock;
class JKRArchive;
class JUTResFont;

f32 SMSGetVSyncTimesPerSec();
f32 SMSGetAnmFrameRate();
void* SMSLoadArchive(const char*, void*, u32, JKRHeap*);

struct TARAMBlock {
	/* 0x0 */ JKRAramBlock* unk0;
	/* 0x4 */ bool unk4;
};
void SMSLoadArchiveARAM(TARAMBlock*, const char*);
void SMSMountAramArchive(JKRMemArchive*, TARAMBlock&);
JKRArchive* SMSSwitch2DArchive(const char*, TARAMBlock&);
extern TARAMBlock gArBkConsole;
extern TARAMBlock gArBkGuide;
extern JUTResFont* gpSystemFont;

class TApplication {
public:
	TApplication();
	void* setupThreadFuncBoot();
	void initialize();
	void* setupThreadFuncLogo();
	void initialize_bootAfter();
	void initialize_nlogoAfter();
	void initialize_processMeter();
	void finalize();
	bool checkAdditionalMovie();
	void proc();
	int gameLoop();
	int drawDVDErr();
	JKRMemArchive* mountStageArchive();

	void setNextArea(const TGameSequence& next_area) { mNextArea = next_area; }

public:
	enum {
		APP_STATE_WAIT     = 0,
		APP_STATE_DEFAULT  = 1,
		APP_STATE_BOOT     = 2,
		APP_STATE_NLOGO    = 3,
		APP_STATE_DONE     = 4,
		APP_STATE_GAMEPLAY = 5,
		APP_STATE_MOVIE    = 6,
		APP_STATE_QUIT     = 7,
		APP_STATE_TITLE    = 8,
		APP_STATE_MENU     = 9,
	};

	/* 0x00 */ TApplication* mSelf;
	/* 0x04 */ JDrama::TDirector* mDirector;
	/* 0x08 */ u8 mAppState;
	/* 0x0A */ TGameSequence mPrevArea;
	/* 0x0E */ TGameSequence mCurrArea;
	/* 0x12 */ TGameSequence mNextArea;
	/* 0x18 */ int mMovie;
	/* 0x1C */ JDrama::TDisplay* mDisplay;
	/* 0x20 */ TMarioGamePad* mGamePads[4];
	/* 0x30 */ TNameRefPtrAryT<TNameRefAryT<TScenarioArchiveName> >* unk30;
	/* 0x34 */ TSMSFader* mFader;
	/* 0x38 */ char mSaveFile;
	/* 0x3C */ u32 unk3C;
	/* 0x40 */ JKRHeap* mHeap;
	/* 0x44 */ u16 unk44;
	/* 0x46 */ u16 unk46;
	/* 0x48 */ TProcessMeter* mMeter;
};

extern TApplication gpApplication;

class TCardManager;
extern TCardManager* gpCardManager;

#endif

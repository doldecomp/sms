#ifndef SYSTEM_MAR_DIRECTOR_HPP
#define SYSTEM_MAR_DIRECTOR_HPP

#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <JSystem/JDrama/JDRActor.hpp>
#include <JSystem/JDrama/JDRDisplay.hpp>
#include <JSystem/JDrama/JDRDirector.hpp>
#include <JSystem/JGadget/std-vector.hpp>
#include <JSystem/JGeometry.hpp>
#include <Strategic/NameRefAry.hpp>
#include <dolphin/os/OSStopwatch.h>

class TEventWatcher;
class TMarioGamePad;
class TMapObjBase;
class TLiveActor;
class TSpineEnemy;
class TYoshi;
class TShine;
class TItemNozzle;
class TCoin;
class TBaseNPC;
class TPerformList;
class TTalkCursor;
class TGCConsole2;
class TSMSFader;
class MSStage;
class TPauseMenu2;
class TTalk2D2;
class TGuide;
class TCardLoad;
class JKRMemArchive;
class TStageEventInfo;
class TSunGlass;
class TShineFader;
class TDemoCannon;

class TMarDirector;

extern TMarDirector* gpMarDirector;

class TMarDirector : public JDrama::TDirector {
public:
	struct TDemoInfo {
		/* 0x0 */ const char* unk0;
		/* 0x4 */ const JGeometry::TVec3<f32>* unk4;
		/* 0x8 */ u32 unk8;
		/* 0xC */ f32 unkC;
		/* 0x10 */ bool unk10;
		/* 0x14 */ s32 (*unk14)(u32, u32);
		/* 0x18 */ u32 unk18;
		/* 0x1C */ JDrama::TActor* unk1C;
		/* 0x20 */ JDrama::TFlagT<u16> unk20;
	};

	void registerEventWatcher(TEventWatcher*);
	u32 setup(JDrama::TDisplay*, TMarioGamePad**, u8, u8);
	static void* setupThreadFunc(void*);
	TMarDirector();
	void moveStage();
	u8 updateGameMode();
	void nextStateInitialize(u8);
	void setMario();
	void currentStateFinalize(u8);
	int changeState();
	void fireStreamingMovie(u8);
	void fireEndDemoCamera();
	void fireStartDemoCamera(const char*, const JGeometry::TVec3<f32>*, s32,
	                         f32, bool, s32 (*)(u32, u32), u32, JDrama::TActor*,
	                         JDrama::TFlagT<u16>);
	void fireStageEvent(TMapObjBase*);
	void setNextStage(u16, JDrama::TActor*);
	void movement();
	void fireDemoMovie(u32, TLiveActor*);
	void fireDefeatEnemy(TSpineEnemy*);
	void fireRideYoshi(TYoshi*);
	void fireGetStar(TShine*);
	void fireGetNozzle(TItemNozzle*);
	void fireGetBlueCoin(TCoin*);
	void movement_game();
	TBaseNPC* findNearestTakeNPC();
	TBaseNPC* findNearestTalkNPC();
	void updateFlag(TBaseNPC*, u32, u32);
	void getTalkMsgID(TBaseNPC*);
	void entryNPC(TBaseNPC*);
	void setupPerformList_console();
	void
	initECDisp(TPerformList*,
	           JDrama::TViewObjPtrListT<JDrama::TViewObj, JDrama::TViewObj>*,
	           JDrama::TViewObjPtrListT<JDrama::TViewObj, JDrama::TViewObj>*);
	static JDrama::TViewObj*
	initECTMir(TPerformList*,
	           JDrama::TViewObjPtrListT<JDrama::TViewObj, JDrama::TViewObj>*);
	static void
	initECTGft(TPerformList*, TPerformList*,
	           JDrama::TViewObjPtrListT<JDrama::TViewObj, JDrama::TViewObj>*,
	           JDrama::TViewObjPtrListT<JDrama::TViewObj, JDrama::TViewObj>*);
	static void preEntry(TPerformList*);

	virtual ~TMarDirector();
	virtual int direct();
	virtual JStage::TObject* JSGFindObject(const char*, JStage::TEObject) const;

	void setup2();
	int thpInit();
	void loadParticleMario();
	void loadParticle();
	void initLoadParticle();
	int loadResource();
	bool createObjects();
	bool setupObjects();
	void decideMarioPosIdx();

	// fabricated
	u8 getCurrentMap() { return mMap; }
	u8 getCurrentStage() { return unk7D; }
	bool checkUnk4CFlag(int flag) { return unk4C & flag; }
	void onUnk4CFlag(int flag) { unk4C |= flag; }
	void offUnk4CFlag(int flag) { unk4C &= ~flag; }
	TGCConsole2* getConsole() { return mConsole; }

	bool isTalkModeNow() const
	{
		if (unk124 == 1 || unk124 == 2)
			return true;
		return false;
	}

	bool checkUnk124Thing2() const
	{
		if (unk124 == 3 || unk124 == 4)
			return true;
		return false;
	}

	void* getUnkD4() { return unkD4; }
	TBaseNPC* getTalkingNPC() { return unkA0; }

	int getRestTime()
	{
		s64 ticks = OSCheckStopwatch(&unkE8);
		int time  = OSTicksToMilliseconds(ticks) / 10;
		return unk120 - time;
	}

	void startTimer() { unkC8 = OSCheckStopwatch(&unkE8); }

public:
	enum {
		STATE_UNK0  = 0,
		STATE_UNK1  = 1,
		STATE_UNK2  = 2,
		STATE_UNK3  = 3,
		STATE_UNK4  = 4,
		STATE_UNK5  = 5,
		STATE_UNK7  = 7,
		STATE_UNK9  = 9,
		STATE_UNK10 = 10,
		STATE_UNK11 = 11,
		STATE_UNK12 = 12,
	};

	/* 0x18 */ TMarioGamePad** unk18;
	/* 0x1C */ TPerformList* mPerformListGX;
	/* 0x20 */ TPerformList* mPerformListSilhouette;
	/* 0x24 */ TPerformList* mPerformListGXPost;
	/* 0x28 */ TPerformList* mPerformListMovement;
	/* 0x2C */ TPerformList* mPerformListCalcAnim;
	/* 0x30 */ TPerformList* unk30;
	/* 0x34 */ TPerformList* unk34;
	/* 0x38 */ TPerformList* unk38;
	/* 0x3C */ TPerformList* unk3C;
	/* 0x40 */ TPerformList* unk40;
	/* 0x44 */ TPerformList* mShinePfLstMov;
	/* 0x48 */ TPerformList* mShinePfLstAnm;
	/* 0x4C */ u16 unk4C;
	/* 0x4E */ u16 unk4E;
	/* 0x50 */ u16 unk50;
	/* 0x54 */ int unk54;
	/* 0x58 */ int unk58;
	/* 0x5C */ int unk5C;
	/* 0x60 */ int unk60;
	/* 0x64 */ u8 mState;
	/* 0x68 */ u32 unk68;
	/* 0x6C */ f32 unk6C;
	/* 0x70 */ TCardLoad* unk70;
	/* 0x74 */ TGCConsole2* mConsole;
	/* 0x78 */ TGuide* unk78;
	/* 0x7C */ u8 mMap;
	/* 0x7D */ u8 unk7D;
	/* 0x7E */ u8 unk7E;
	/* 0x7F */ u8 unk7F;
	/* 0x80 */ JDrama::TViewObjPtrListT<JDrama::TViewObj>* unk80;
	/* 0x84 */ TTalkCursor* unk84;
	/* 0x88 */ JGadget::TVector_pointer<TBaseNPC> unk88;
	/* 0xA0 */ TBaseNPC* unkA0; // talking NPC
	/* 0xA4 */ u32 unkA4;
	/* 0xA8 */ char unkA8[0x4];
	/* 0xAC */ TPauseMenu2* unkAC;
	/* 0xB0 */ TTalk2D2* unkB0;
	/* 0xB4 */ u8 unkB4;
	/* 0xB8 */ JKRMemArchive* unkB8;
	/* 0xBC */ TNameRefAryT<TStageEventInfo>* unkBC;
	/* 0xC0 */ JDrama::TDisplay* unkC0;
	/* 0xC4 */ char unkC4[0x4];
	/* 0xC8 */ s64 unkC8;
	/* 0xD0 */ u8 unkD0;
	/* 0xD1 */ u8 unkD1;
	/* 0xD4 */ void* unkD4;
	/* 0xD8 */ JKRMemArchive* unkD8;
	/* 0xDC */ TShineFader* unkDC;
	/* 0xE0 */ TSunGlass* unkE0;
	/* 0xE4 */ u32 unkE4;
	/* 0xE8 */ OSStopwatch unkE8;
	/* 0x120 */ int unk120;
	/* 0x124 */ u8 unk124; // Game state, paused, shine animation, 2=talking
	/* 0x125 */ u8 unk125;
	/* 0x126 */ u8 unk126; // Next game state
	/* 0x128 */ u16 unk128;
	/* 0x12C */ TDemoInfo unk12C[8];
	/* 0x24C */ u8 unk24C;
	/* 0x24D */ u8 unk24D;
	/* 0x250 */ JDrama::TActor* unk250;
	/* 0x254 */ TDemoCannon* unk254;
	/* 0x258 */ MSStage* unk258;
	/* 0x25C */ TShine* unk25C;
	/* 0x260 */ u8 unk260;
	/* 0x261 */ u8 unk261;
};

#endif

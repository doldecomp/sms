#ifndef SYSTEM_MAR_DIRECTOR_HPP
#define SYSTEM_MAR_DIRECTOR_HPP

#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <JSystem/JDrama/JDRActor.hpp>
#include <JSystem/JDrama/JDRDisplay.hpp>
#include <JSystem/JDrama/JDRDirector.hpp>
#include <JSystem/JGadget/std-vector.hpp>
#include <JSystem/JGeometry.hpp>
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
class TGuide;

class TMarDirector;

extern TMarDirector* gpMarDirector;

class TMarDirector : public JDrama::TDirector {
public:
	struct TDemoInfo {
		/* 0x0 */ const char* unk0;
		/* 0x4 */ const JGeometry::TVec3<f32>* unk4;
		/* 0x8 */ u32 unk8;
		/* 0xC */ f32 unkC;
		/* 0x10 */ u8 unk10;
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
	JDrama::TViewObj*
	initECTMir(TPerformList*,
	           JDrama::TViewObjPtrListT<JDrama::TViewObj, JDrama::TViewObj>*);
	void
	initECTGft(TPerformList*, TPerformList*,
	           JDrama::TViewObjPtrListT<JDrama::TViewObj, JDrama::TViewObj>*,
	           JDrama::TViewObjPtrListT<JDrama::TViewObj, JDrama::TViewObj>*);
	static void preEntry(TPerformList*);

	virtual ~TMarDirector();
	virtual int direct();
	virtual JStage::TObject* JSGFindObject(const char*, JStage::TEObject) const;

	void setup2();
	void thpInit();
	void loadParticleMario();
	void loadParticle();
	void initLoadParticle();
	void loadResource();
	bool createObjects();
	void setupObjects();
	void decideMarioPosIdx();

	// fabricated
	u8 getCurrentMap() { return mMap; }
	u8 getCurrentStage() { return unk7D; }
	bool checkUnk4CFlag(int flag) { return unk4C & flag; }
	void onUnk4CFlag(int flag) { unk4C |= flag; }
	TGCConsole2* getConsole() { return mConsole; }

	bool checkUnk124Thing1() const
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

public:
	/* 0x18 */ TMarioGamePad** unk18;
	/* 0x1C */ TPerformList* unk1C;
	/* 0x20 */ TPerformList* unk20;
	/* 0x24 */ TPerformList* unk24;
	/* 0x28 */ TPerformList* unk28;
	/* 0x2C */ TPerformList* unk2C;
	/* 0x30 */ TPerformList* unk30;
	/* 0x34 */ TPerformList* unk34;
	/* 0x38 */ TPerformList* unk38;
	/* 0x3C */ TPerformList* unk3C;
	/* 0x40 */ TPerformList* unk40;
	/* 0x44 */ TPerformList* unk44;
	/* 0x48 */ TPerformList* unk48;
	/* 0x4C */ volatile u16 unk4C;
	/* 0x4E */ u16 unk4E;
	/* 0x50 */ u16 unk50;
	/* 0x54 */ int unk54;
	/* 0x58 */ int unk58;
	/* 0x5C */ u32 unk5C;
	/* 0x60 */ u32 unk60;
	/* 0x64 */ u8 unk64;
	/* 0x68 */ u32 unk68;
	/* 0x6C */ f32 unk6C;
	/* 0x70 */ JDrama::TViewObj* unk70;
	/* 0x74 */ TGCConsole2* mConsole;
	/* 0x78 */ TGuide* unk78;
	/* 0x7C */ u8 mMap;
	/* 0x7D */ u8 unk7D;
	/* 0x7E */ u8 unk7E;
	/* 0x7F */ u8 unk7F;
	/* 0x80 */ JDrama::TViewObjPtrListT<JDrama::TViewObj>* unk80;
	/* 0x84 */ TTalkCursor* unk84;
	/* 0x88 */ JGadget::TVector_pointer<TBaseNPC> unk88;
	/* 0xA0 */ TBaseNPC* unkA0;
	/* 0xA4 */ char unkA4[0x8];
	/* 0xAC */ TPauseMenu2* unkAC;
	/* 0xB0 */ JDrama::TViewObj* unkB0;
	/* 0xB4 */ u8 unkB4;
	/* 0xB8 */ u32 unkB8;
	/* 0xBC */ JDrama::TNameRef* unkBC; // TODO: type?
	/* 0xC0 */ JDrama::TDisplay* unkC0;
	/* 0xC4 */ char unkC4[0x4];
	/* 0xC8 */ u32 unkC8;
	/* 0xCC */ u32 unkCC;
	/* 0xD0 */ u8 unkD0;
	/* 0xD1 */ u8 unkD1;
	/* 0xD4 */ u32 unkD4;
	/* 0xD8 */ u32 unkD8;
	/* 0xDC */ TSMSFader* unkDC;
	/* 0xE0 */ TSMSFader* unkE0; // TODO: type unconfirmed
	/* 0xE4 */ u32 unkE4;
	/* 0xE8 */ OSStopwatch unkE8;
	/* 0x120 */ char unk120[0x4];
	/* 0x124 */ u8 unk124;
	/* 0x125 */ u8 unk125;
	/* 0x126 */ u8 unk126;
	/* 0x128 */ u16 unk128;
	/* 0x12C */ TDemoInfo unk12C[8];
	/* 0x24C */ u8 unk24C;
	/* 0x24D */ u8 unk24D;
	/* 0x250 */ u32 unk250;
	/* 0x254 */ JDrama::TNameRef* unk254;
	/* 0x258 */ MSStage* unk258;
	/* 0x25C */ TShine* unk25C;
	/* 0x260 */ u8 unk260;
	/* 0x261 */ u8 unk261;
};

#endif

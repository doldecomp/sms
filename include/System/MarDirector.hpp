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
inline TMarDirector* SMSGetMarDirector() { return gpMarDirector; }

enum {
	// Some kind of light-related cues?
	CUE_UNK10000 = 0x10000,
	CUE_UNK20000 = 0x20000,
	CUE_UNK40000 = 0x40000,
	CUE_UNK80000 = 0x80000,

	// Bits 16-17 can store the pollution layer sometimes.
	// If more than 4 layers -- everything breaks horribly.
	CUE_OFFSET_POLLUTION_LAYER = 16,

	CUE_UNK800000              = 0x800000,
	CUE_UNK1000000             = 0x1000000,
	CUE_SEMITRANSPARENT_PRIO_2 = 0x2000000,
	CUE_SEMITRANSPARENT_PRIO_1 = 0x4000000,
	CUE_UNK8000000             = 0x8000000,
	CUE_UNK10000000            = 0x10000000,
	CUE_UNK20000000            = 0x20000000,
	CUE_UNK40000000            = 0x40000000,
	CUE_UNK80000000            = 0x80000000,
};

class TMarDirector : public JDrama::TDirector {
public:
	struct TDemoInfo {
		/* 0x0 */ const char* unk0;
		/* 0x4 */ const JGeometry::TVec3<f32>* unk4;
		/* 0x8 */ u32 unk8;
		/* 0xC */ f32 unkC;
		/* 0x10 */ bool unk10;
		/* 0x14 */ s32 (*unk14)(uintptr_t, u32);
		/* 0x18 */ uintptr_t unk18;
		/* 0x1C */ JDrama::TActor* unk1C;
		/* 0x20 */ JDrama::TFlagT<u16> unk20;
	};

	void registerEventWatcher(TEventWatcher*);
	u32 setup(JDrama::TDisplay* display, TMarioGamePad** game_pads, u8 stage,
	          u8 scenario);
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
	                         f32, bool, s32 (*)(uintptr_t, u32), uintptr_t,
	                         JDrama::TActor*, JDrama::TFlagT<u16>);
	void fireStageEvent(TMapObjBase*);
	void setNextStage(u16 stage, JDrama::TActor* gate_demo_actor);
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
	static void
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
	static void loadParticleMario();
	void loadParticle();
	void initLoadParticle();
	int loadResource();
	bool createObjects();
	bool setupObjects();
	void decideMarioPosIdx();

	// fabricated
	u8 getCurrentStage() { return mStage; }
	u8 getCurrentScenario() { return mScenario; }
	BOOL checkFlag(u16 flag) const { return mFlags & flag; }
	void onFlag(u16 flag) { mFlags |= flag; }
	void offFlag(u16 flag) { mFlags &= ~flag; }
	bool checkDemoFlag(int flag) const { return mDemoFlags & flag; }
	void onDemoFlag(int flag) { mDemoFlags |= flag; }
	void offDemoFlag(int flag) { mDemoFlags &= ~flag; }
	BOOL checkTransitionFlag(int flag) const { return mTransitionFlags & flag; }
	void onTransitionFlag(int flag) { mTransitionFlags |= flag; }
	void offTransitionFlag(int flag) { mTransitionFlags &= ~flag; }
	// Fabricated
	u8 getState() const { return mState; }
	TGCConsole2* getConsole() { return mConsole; }

	// Fabricated
	u8 getGameMode() const { return mGameMode; }

	bool isGameplayModeNow() const { return mGameMode == GAME_MODE_NORMAL; }
	// Fabricated
	bool isTalkStartModeNow() const
	{
		return mGameMode == GAME_MODE_TALK_START;
	}
	// Fabricated
	bool isTalkMainModeNow() const { return mGameMode == GAME_MODE_TALK; }
	bool isTalkModeNow() const
	{
		return mGameMode == GAME_MODE_TALK_START || mGameMode == GAME_MODE_TALK;
	}
	bool isDemoModeNow() const
	{
		return mGameMode == GAME_MODE_INNER_DEMO
		       || mGameMode == GAME_MODE_TALKING_DEMO;
	}
	bool isInnerDemoModeNow() const
	{
		return mGameMode == GAME_MODE_INNER_DEMO;
	}
	bool isTalkingDemoModeNow() const
	{
		return mGameMode == GAME_MODE_TALKING_DEMO;
	}
	bool isTalkOrDemoModeNow() const
	{
		return isTalkModeNow() || isDemoModeNow();
	}
	bool isInTalkNow() const
	{
		return isTalkModeNow() || mGameMode == GAME_MODE_TALKING_DEMO;
	}

	void* get2DArchiveBuffer() { return m2DArchiveBuffer; }
	JKRMemArchive* get2DArchive() { return m2DArchive; }
	TBaseNPC* getTalkingNPC() { return mTalkingNPC; }
	// Fabricated
	void setTalkingNPC(TBaseNPC* npc) { mTalkingNPC = npc; }
	// Fabricated
	JGadget::TVector_pointer<TBaseNPC>& getNPCList() { return mNPCList; }
	// Fabricated
	u8 getMarioPosIdx() const { return mMarioPosIdx; }
	// Fabricated
	void setNextGameMode(u8 mode) { mNextGameMode = mode; }

	// fabricated
	int getTime()
	{
		s64 ticks = OSCheckStopwatch(&unkE8);
		return OSTicksToMilliseconds(ticks) / 10;
	}
	int getRestTime() { return unk120 - getTime(); }
	s64 checkStopwatch() { return OSCheckStopwatch(&unkE8); }
	s64 getStartTime() { return unkC8; }
	// TODO: very likely a fakematch
	static s64 diffMs(s64 now, s64 start)
	{
		return OSTicksToMilliseconds(now) - OSTicksToMilliseconds(start);
	}
	f32 getTimerTime()
	{
		return diffMs(checkStopwatch(), getStartTime()) * 0.1f;
	}
	void startTimer() { unkC8 = OSCheckStopwatch(&unkE8); }
	void resetStopwatch() { OSResetStopwatch(&unkE8); }
	void setTimeLimit(int time) { unk120 = time; }

	TMarioGamePad* getGamePad(int i = 0) { return mGamePads[i]; }

	// Fabricated
	int getMoveTickCount() const { return mMoveTickCount; }

	enum {
		GAME_MODE_NORMAL       = 0,
		GAME_MODE_TALK_START   = 1,
		GAME_MODE_TALK         = 2,
		GAME_MODE_INNER_DEMO   = 3,
		GAME_MODE_TALKING_DEMO = 4,
	};

	enum {
		TALK_FLAG_NPC_NEAR_THIS_FRAME = 0x1,
		TALK_FLAG_NPC_NEAR_LAST_FRAME = 0x2,
	};

	enum {
		STATE_UNK0       = 0,
		STATE_UNK1       = 1,
		STATE_UNK2       = 2,
		STATE_UNK3       = 3,
		STATE_UNK4       = 4,
		STATE_PAUSE_MENU = 5,
		STATE_UNK7       = 7,
		STATE_UNK9       = 9,
		STATE_GUIDE      = 10,
		STATE_CARD_SAVE  = 11,
		STATE_UNK12      = 12,
	};

	enum {
		DIRECTOR_FLAG_SHINE_GET_PENDING                   = 0x1,
		DIRECTOR_FLAG_STAGE_TRANSITION_PENDING            = 0x2,
		DIRECTOR_FLAG_ACTOR_DEMO_STAGE_TRANSITION_PENDING = 0x4,
		DIRECTOR_FLAG_GATE_DEMO_STAGE_TRANSITION_PENDING  = 0x8,
		DIRECTOR_FLAG_GAME_OVER_PENDING                   = 0x20,
		DIRECTOR_FLAG_DEMO_PENDING                        = 0x40,
		DIRECTOR_FLAG_END_DEMO_PENDING                    = 0x80,
		DIRECTOR_FLAG_MOVIE_PENDING                       = 0x100,
		DIRECTOR_FLAG_CARD_SAVE_PENDING                   = 0x200,
		DIRECTOR_FLAG_FIRST_SIMULATION_TICK               = 0x2000,
		DIRECTOR_FLAG_LAST_SIMULATION_TICK                = 0x4000,
		DIRECTOR_FLAG_SHINE_TAKEN                         = 0x8000,
	};

	enum {
		DEMO_FLAG_SHINE_GET_STOP_THE_WORLD = 0x1,
		DEMO_FLAG_CAMERA_DEMO_ON_START     = 0x2,
		DEMO_FLAG_CAMERA_DEMO_WIPE_STARTED = 0x4,
		DEMO_FLAG_HELL_DEAD                = 0x8,
		DEMO_FLAG_SHINE_ALREADY_COLLECTED  = 0x10,
	};

	enum {
		TRANSITION_FLAG_STAGE_BGM_STARTED            = 0x1,
		TRANSITION_FLAG_GO_BANNER_PENDING            = 0x2,
		TRANSITION_FLAG_SCENARIO_NAME_BANNER_PENDING = 0x4,
		TRANSITION_FLAG_OPENING_WIPE_PENDING         = 0x8,
		TRANSITION_FLAG_RESET_HANDLED                = 0x10,
	};

private:
	/* 0x18 */ TMarioGamePad** mGamePads;
	/* 0x1C */ TPerformList* mPerformListGX;
	/* 0x20 */ TPerformList* mPerformListSilhouette;
	/* 0x24 */ TPerformList* mPerformListGXPost;
	/* 0x28 */ TPerformList* mPerformListMovement;
	/* 0x2C */ TPerformList* mPerformListCalcAnim;
	/* 0x30 */ TPerformList* mPerformListConsole;
	/* 0x34 */ TPerformList* mPerformListPreEntry;
	/* 0x38 */ TPerformList* mPerformListPollutionCountObj;
	/* 0x3C */ TPerformList* mPerformListPollutionCountLayer;
	/* 0x40 */ TPerformList* mPerformListDrawInit;
	/* 0x44 */ TPerformList* mPerformListShineMovement;
	/* 0x48 */ TPerformList* mPerformListShineCalcAnim;
	/* 0x4C */ u16 mFlags;
	/* 0x4E */ u16 mDemoFlags;
	/* 0x50 */ u16 mTransitionFlags;
	/* 0x54 */ int mPendingSimulationTime;
	/* 0x58 */ int mMoveTickCount;
	/* 0x5C */ int mTickCount;
	/* 0x60 */ int unk60;
	/* 0x64 */ u8 mState;
	/* 0x68 */ u32 unk68;
	/* 0x6C */ f32 unk6C;
	/* 0x70 */ TCardLoad* mCardLoad;
	/* 0x74 */ TGCConsole2* mConsole;
	/* 0x78 */ TGuide* mGuide;
	/* 0x7C */ u8 mStage;
	/* 0x7D */ u8 mScenario;
	/* 0x80 */ JDrama::TViewObjPtrListT<JDrama::TViewObj>* unk80;
	/* 0x84 */ TTalkCursor* unk84;
	/* 0x88 */ JGadget::TVector_pointer<TBaseNPC> mNPCList;
	/* 0xA0 */ TBaseNPC* mTalkingNPC;
	/* 0xA4 */ u32 unkA4;
	/* 0xA8 */ char unkA8[0x4];
	/* 0xAC */ TPauseMenu2* unkAC;
	/* 0xB0 */ TTalk2D2* unkB0;
	/* 0xB4 */ u8 unkB4;
	/* 0xB8 */ JKRMemArchive* unkB8;
	/* 0xBC */ TNameRefAryT<TStageEventInfo>* unkBC;
	/* 0xC0 */ JDrama::TDisplay* mDisplay;
	/* 0xC4 */ char unkC4[0x4];
	/* 0xC8 */ s64 unkC8;
	/* 0xD0 */ u8 mMarioPosIdx;
	/* 0xD1 */ u8 unkD1;
	/* 0xD4 */ void* m2DArchiveBuffer;
	/* 0xD8 */ JKRMemArchive* m2DArchive;
	/* 0xDC */ TShineFader* unkDC;
	/* 0xE0 */ TSunGlass* unkE0;
	/* 0xE4 */ u32 unkE4;
	/* 0xE8 */ OSStopwatch unkE8;
	/* 0x120 */ int unk120;
	/* 0x124 */ u8 mGameMode;
	/* 0x125 */ u8 mPrevGameMode;
	/* 0x126 */ u8 mNextGameMode;
	/* 0x128 */ u16 mTalkFlags;
	/* 0x12C */ TDemoInfo mDemoQueue[8];
	/* 0x24C */ u8 mDemoQueueTail;
	/* 0x24D */ u8 mDemoQueueHead;
	/* 0x250 */ JDrama::TActor* mGateDemoActor;
	/* 0x254 */ TDemoCannon* unk254;
	/* 0x258 */ MSStage* unk258;
	/* 0x25C */ TShine* unk25C;
	/* 0x260 */ bool mSetupDone;
	/* 0x261 */ u8 unk261;
};

#endif

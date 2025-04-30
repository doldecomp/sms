#ifndef SYSTEM_MAR_DIRECTOR_HPP
#define SYSTEM_MAR_DIRECTOR_HPP

#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <JSystem/JDrama/JDRActor.hpp>
#include <JSystem/JDrama/JDRDisplay.hpp>
#include <JSystem/JDrama/JDRDirector.hpp>
#include <JSystem/JGeometry.hpp>
#include <GC2D/GCConsole2.hpp>

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

class TMarDirector;

extern TMarDirector* gpMarDirector;

class TMarDirector : public JDrama::TDirector {
public:
	void registerEventWatcher(TEventWatcher*);
	void setup(JDrama::TDisplay*, TMarioGamePad**, u8, u8);
	void setupThreadFunc(void*);
	TMarDirector();
	JStage::TObject* JSGFindObject(const char*, JStage::TEObject) const;
	void moveStage();
	void updateGameMode();
	void nextStateInitialize(u8);
	void setMario();
	void currentStateFinalize(u8);
	void changeState();
	void direct();
	void fireStreamingMovie(u8);
	void fireEndDemoCamera();
	void fireStartDemoCamera(const char*, const JGeometry::TVec3<f32>*, long,
	                         f32, bool, long (*)(u32, u32), u32,
	                         JDrama::TActor*, JDrama::TFlagT<u16>);
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
	void findNearestTakeNPC();
	void findNearestTalkNPC();
	void updateFlag(TBaseNPC*, u32, u32);
	void getTalkMsgID(TBaseNPC*);
	void entryNPC(TBaseNPC*);
	void setupPerformList_console();
	void
	initECDisp(TPerformList*,
	           JDrama::TViewObjPtrListT<JDrama::TViewObj, JDrama::TViewObj>*,
	           JDrama::TViewObjPtrListT<JDrama::TViewObj, JDrama::TViewObj>*);
	void
	initECTMir(TPerformList*,
	           JDrama::TViewObjPtrListT<JDrama::TViewObj, JDrama::TViewObj>*);
	void
	initECTGft(TPerformList*, TPerformList*,
	           JDrama::TViewObjPtrListT<JDrama::TViewObj, JDrama::TViewObj>*,
	           JDrama::TViewObjPtrListT<JDrama::TViewObj, JDrama::TViewObj>*);
	void preEntry(TPerformList*);
	~TMarDirector();

	void setup2();
	void thpInit();
	void loadParticleMario();
	void loadParticle();
	void initLoadParticle();
	void loadResource();
	void createObjects();
	void setupObjects();
	void decideMarioPosIdx();

	u8 getCurrentMap() { return map; }

public:
	/* 0x24 */ char unk24[0x40];
	/* 0x28 */ s32 frameIndex;
	/* 0x2C */ char unk2C[0x14];
	/* 0x70 */ int _070;
	/* 0x74 */ TGCConsole2* console;
	/* 0x78 */ int _078;
	/* 0x7C */ u8 map;
	/* 0x7D */ u8 _07D;
	/* 0x7E */ u8 _07E;
	/* 0x7F */ u8 _07F;
	/* 0x80 */ char unk80[0xA4];
	/* 0x124 */ u8 unk124;
};

#endif

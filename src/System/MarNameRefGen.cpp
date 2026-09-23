#include <System/MarNameRefGen.hpp>

// rogue includes needed for matching the .rodata string pool: retail's blob
// opens with the System/DummyStrings.hpp pair, then the four
// M3DUtil/InfectiousStrings.hpp mtx-calc names, then the pollution-texture
// pair below.
#include <M3DUtil/InfectiousStrings.hpp>

// TODO: retail has these two as (object,local) here as well, from the same
// unidentified shared Player header as in the other nineteen TUs that carry
// them; parked so the leading .rodata block lines up.
#include <Player/MarioDirtyStrings.hpp>

// The 12-byte zero and one vectors that MapCollisionEntry.hpp's setUpTrans
// parks in .rodata, plus the MSound pair that supplies the fifteen 12-byte
// .bss nodes and the 764-byte __sinit.
#include <Map/MapCollisionManager.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

#include <JSystem/JDrama/JDRSmJ3DScn.hpp>
#include <System/StageEventInfo.hpp>
#include <System/TalkCursor.hpp>
#include <System/TargetArrow.hpp>
#include <System/PositionHolder.hpp>
#include <System/PerformList.hpp>
#include <System/ZBufferCatch.hpp>
#include <System/ScenarioArchiveName.hpp>
#include <System/J3DSysFlag.hpp>
#include <MarioUtil/ShadowUtil.hpp>
#include <MarioUtil/LightUtil.hpp>
#include <MarioUtil/ScreenUtil.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <Strategic/question.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Strategy.hpp>
#include <Strategic/NameRefAry.hpp>
#include <Strategic/SmplCharacter.hpp>
#include <GC2D/Talk2D2.hpp>
#include <GC2D/ScrnFader.hpp>
#include <GC2D/GCConsole2.hpp>
#include <GC2D/HelpActor.hpp>
#include <GC2D/ConsoleStr.hpp>
#include <GC2D/PauseMenu2.hpp>
#include <GC2D/CardSave.hpp>
#include <GC2D/CardLoad.hpp>
#include <GC2D/Guide.hpp>
#include <GC2D/SunGlass.hpp>
#include <Map/MapMirror.hpp>
#include <Map/MapEventDolpic.hpp>
#include <Map/MapEventMare.hpp>
#include <Map/MapEventSink.hpp>
#include <Map/MapEventSirena.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/EffectObj.hpp>
#include <Enemy/AreaCylinder.hpp>
#include <Enemy/Generator.hpp>
#include <Enemy/EnemyTable.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Player/SplashManager.hpp>
#include <Player/Mario.hpp>
#include <Player/MarioPositionObj.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <Camera/Camera.hpp>
#include <Camera/CameraMapTool.hpp>
#include <JSystem/JDrama/JDRViewObjPtrList.tpp>
#include <Strategic/NameRefPtrAry.tpp>
#include <Strategic/NameRefAry.tpp>

// The order is the map's, read off the .text layout and reversed (the TU is
// -inline deferred, so emission is reverse source order): retail emits the
// groups StagePositionInfo, PtrAry<CubeGeneralInfo>, TViewObjPtrListT,
// CameraMapTool, PtrAry<Ary<ScenarioArchiveName>>, ScenarioArchiveName,
// PtrAry<StageEventInfo> and finally Ary<StageEventInfo> -- the last of which
// is still implicit, instantiated by getNameRef itself, and already lands in
// the right place. With this order and the declaration order in
// JDRViewObjPtrList.hpp, validate-symbol-order.py passes the unit outright
// (18/18 UNUSED sizes included). Do not sort these.
//
// PtrAry<StageEventInfo>'s three members are UNUSED in the map: retail's
// object defined them and the linker stripped them, which is exactly what an
// explicit instantiation of an unreferenced specialisation gives.
template class TNameRefPtrAryT<TStageEventInfo>;
template class TNameRefAryT<TScenarioArchiveName>;
template class TNameRefPtrAryT<TNameRefAryT<TScenarioArchiveName> >;
// TODO: TVector<TCameraMapTool>::InsertRaw (60.2%) calls the implicit
// TCameraMapTool::operator= out of line where retail inlines it into the
// copy_backward loop; the copied bytes are identical. Measured threshold: the
// implicit operator= inlines at that depth with the base plus at most five
// members and not with six (TVec3 vs Vec is irrelevant), while retail's copy
// shows six distinct pieces (12-byte and 8-byte block copies, then four
// single words), and grouping any of them into a struct or array changes the
// copy pattern (93.5-96.7%). TStageEventInfo's operator= must stay out of
// line, so the __copy_backward level cannot be flattened (see std-vector.hpp).
template class TNameRefAryT<TCameraMapTool>;

namespace JDrama {
template class TViewObjPtrListT<THitActor, TViewObj>;
}

template class TNameRefPtrAryT<TCubeGeneralInfo>;
template class TNameRefAryT<TStagePositionInfo>;

JDrama::TNameRef* TMarNameRefGen::getNameRef(const char* name) const
{
	if (strcmp(name, "BindShadow") == 0)
		return new TMBindShadowManager;

	if (strcmp(name, "normalLight") == 0)
		return new TLightCommon;

	if (strcmp(name, "shadowLight") == 0)
		return new TLightShadow;

	if (strcmp(name, "EffectObjManager") == 0)
		return new TEffectObjManager;

	if (strcmp(name, "EffectFire") == 0)
		return new TEffectObjBase;

	if (strcmp(name, "ObjChara") == 0)
		return new TObjChara;

	if (strcmp(name, "SmplChara") == 0)
		return new TSMSSmplChara;

	if (JDrama::TNameRef* ref = getNameRef_BossEnemy(name))
		return ref;

	if (JDrama::TNameRef* ref = getNameRef_Enemy(name))
		return ref;

	if (strcmp(name, "LiveActor") == 0)
		return new TLiveActor;

	if (strcmp(name, "LiveManager") == 0)
		return new TLiveManager;

	if (strcmp(name, "QuestionManager") == 0)
		return new TQuestionManager;

	if (JDrama::TNameRef* ref = getNameRef_NPC(name))
		return ref;

	if (strcmp(name, "AfterEffect") == 0)
		return new TAfterEffect;

	if (strcmp(name, "Generator") == 0)
		return new TGenerator;

	if (strcmp(name, "OneShotGenerator") == 0)
		return new TOneShotGenerator;

	if (strcmp(name, "AreaSphere") == 0)
		return new TAreaCylinder;

	if (strcmp(name, "AreaCylinder") == 0)
		return new TAreaCylinder;

	if (strcmp(name, "Talk2D") == 0)
		return new TTalk2D2;

	if (strcmp(name, "MarScene") == 0)
		return new JDrama::TSmJ3DScn;

	if (strcmp(name, "ScreenTexture") == 0)
		return new TScreenTexture;

	if (strcmp(name, "ModelWaterManager") == 0)
		return new TModelWaterManager;

	if (strcmp(name, "SplashManager") == 0)
		return new TSplashManager;

	if (strcmp(name, "PolarSubCamera") == 0)
		return new CPolarSubCamera;

	if (strcmp(name, "MirrorCamera") == 0)
		return new TMirrorCamera;

	if (strcmp(name, "Mario") == 0) {
		TMario* mario   = new TMario;
		gpMarioOriginal = mario;
		gpMarioAddress  = mario;
		return mario;
	}

	if (strcmp(name, "MLight") == 0) {
		TLightMario* light          = new TLightMario;
		gpLightManager->mMarioLight = light;
		return light;
	}

	if (strcmp(name, "MirrorModelManager") == 0)
		return new TMirrorModelManager;

	if (strcmp(name, "MirrorMapDrawBuf") == 0)
		return new TMirrorMapDrawBuf;

	if (strcmp(name, "Silhouette") == 0)
		return new TSilhouette;

	if (strcmp(name, "ScrnFader") == 0)
		return new TSmplFader;

	if (strcmp(name, "ShineFader") == 0)
		return new TShineFader;

	if (strcmp(name, "IdxGroup") == 0)
		return new TIdxGroupObj;

	if (strcmp(name, "HitActor") == 0)
		return new THitActor;

	if (strcmp(name, "Strategy") == 0)
		return new TStrategy;

	if (JDrama::TNameRef* ref = getNameRef_Map(name))
		return ref;

	if (strcmp(name, "GCConsole") == 0)
		return new TGCConsole2;

	if (strcmp(name, "BalloonHelp") == 0)
		return new THelpActor;

	if (strcmp(name, "SwitchHelp") == 0)
		return new TSwitchHelpActor;

	if (strcmp(name, "ConsoleStr") == 0)
		return new TConsoleStr;

	if (strcmp(name, "PauseMenu") == 0)
		return new TPauseMenu2;

	if (strcmp(name, "CardSave") == 0)
		return new TCardSave;

	if (strcmp(name, "CardLoad") == 0)
		return new TCardLoad;

	if (strcmp(name, "Guide") == 0)
		return new TGuide;

	// TODO: the map emits TSunGlass's in-class constructor as a weak 0xb0
	// body in this TU and *calls* it here, so MWCC refused the expansion.
	// We expand it. Same shape as TTelesaSlot in MarNameRefGen_MapObj: both
	// refused callees are in-class bodies whose generated form is ~40
	// instructions, which is the only property they share.
	if (strcmp(name, "SunGlass") == 0)
		return new TSunGlass;

	if (strcmp(name, "SunShine") == 0)
		return new TSunShine;

	if (strcmp(name, "CubeCamera") == 0) {
		TCubeManagerBase* mgr
		    = new TCubeManagerBase("?", "カメラキューブテーブル");
		gpCubeCamera = mgr;
		return mgr;
	}

	if (strcmp(name, "CubeMirror") == 0)
		return gpCubeMirror = new TCubeManagerBase("?", "鏡キューブテーブル");

	if (strcmp(name, "CubeWire") == 0)
		return gpCubeWire
		       = new TCubeManagerBase("?", "ワイヤーキューブテーブル");

	if (strcmp(name, "CubeStream") == 0)
		return gpCubeStream = new TCubeManagerBase("?", "流れキューブテーブル");

	if (strcmp(name, "CubeShadow") == 0)
		return gpCubeShadow = new TCubeManagerBase("?", "影キューブテーブル");

	if (strcmp(name, "CubeArea") == 0)
		return gpCubeArea = new TCubeManagerArea("?", "エリアキューブテーブル");

	if (strcmp(name, "CubeFastA") == 0)
		return gpCubeFastA
		       = new TCubeManagerFast("?", "高速Ａキューブテーブル");

	if (strcmp(name, "CubeFastB") == 0)
		return gpCubeFastB
		       = new TCubeManagerFast("?", "高速Ｂキューブテーブル");

	if (strcmp(name, "CubeFastC") == 0)
		return gpCubeFastC
		       = new TCubeManagerFast("?", "高速Ｃキューブテーブル");

	if (strcmp(name, "CubeSoundChange") == 0)
		return gpCubeSoundChange
		       = new TCubeManagerBase("?", "サウンド切り替えキューブテーブル");

	if (strcmp(name, "CubeSoundEffect") == 0)
		return gpCubeSoundEffect = new TCubeManagerBase(
		           "?", "サウンドエフェクトキューブテーブル");

	if (JDrama::TNameRef* ref = getNameRef_MapObj(name))
		return ref;

	if (strcmp(name, "MapEventSinkInPollution") == 0)
		return new TMapEventSinkInPollution;

	if (strcmp(name, "MapEventSinkInPollutionReset") == 0)
		return new TMapEventSinkInPollutionReset;

	if (strcmp(name, "MapEventSinkShadowMario") == 0)
		return new TMapEventSinkShadowMario;

	if (strcmp(name, "MapEventSirenaSink") == 0)
		return new TMapEventSirenaSink("ホテル沈む");

	if (strcmp(name, "MapEventSinkBianco") == 0)
		return new TMapEventSinkBianco;

	if (strcmp(name, "DolpicEventBiancoGate") == 0)
		return new TDolpicEventBiancoGate("イベント（ビアンコゲート）");

	if (strcmp(name, "DolpicEventRiccoGate") == 0)
		return new TDolpicEventRiccoMammaGate("イベント（リコ、マンマゲート）");

	if (strcmp(name, "DolpicEventMammaGate") == 0)
		return new TDolpicEventRiccoMammaGate("イベント（リコ、マンマゲート）");

	if (strcmp(name, "MareEventBumpyWall") == 0)
		return new TMareEventBumpyWall("凸凹壁");

	if (strcmp(name, "MareEventWallRock") == 0)
		return new TMareEventWallRock("イベント（マーレ壁の岩）");

	if (strcmp(name, "StageEnemyInfoHeader") == 0)
		return new TStageEnemyInfoTable;

	if (strcmp(name, "StageEnemyInfo") == 0)
		return new TStageEnemyInfo;

	if (strcmp(name, "EventTable") == 0)
		return new TNameRefAryT<TStageEventInfo>;

	if (strcmp(name, "StageEventInfo") == 0)
		return new TStageEventInfo;

	if (strcmp(name, "CameraMapToolTable") == 0)
		return gpCamMapToolTable = new TNameRefAryT<TCameraMapTool>;

	if (strcmp(name, "CubeGeneralInfoTable") == 0)
		return new TNameRefPtrAryT<TCubeGeneralInfo>;

	if (strcmp(name, "CubeGeneralInfo") == 0)
		return new TCubeGeneralInfo;

	if (strcmp(name, "CameraCubeInfo") == 0)
		return new TCubeCameraInfo;

	if (strcmp(name, "StreamGeneralInfoTable") == 0)
		return new TNameRefPtrAryT<TCubeGeneralInfo>;

	if (strcmp(name, "CubeStreamInfo") == 0)
		return new TCubeStreamInfo;

	if (strcmp(name, "ScenarioArchiveNameTable") == 0)
		return new TNameRefAryT<TScenarioArchiveName>;

	if (strcmp(name, "ScenarioArchiveNamesInStage") == 0)
		return new TNameRefPtrAryT<TNameRefAryT<TScenarioArchiveName> >;

	if (strcmp(name, "TalkCursor") == 0)
		return new TTalkCursor;

	// TODO: two inline `this` slots remain 4 bytes low at an exact 0x178
	// frame: TSMSSmplChara (0x154 vs 0x158) and TSplashManager (0x144 vs
	// 0x148); retail has one more dead word between the Splash slot and
	// TSmplFader's TColor temp. Named-local, ctor-param and ref variants
	// did not move it.
	if (strcmp(name, "TargetArrow") == 0)
		return gpTargetArrow = new TTargetArrow;

	if (strcmp(name, "PositionHolder") == 0)
		return gpPositionHolder = new TNameRefAryT<TStagePositionInfo>;

	if (strcmp(name, "MarioPositionObj") == 0)
		return new TMarioPositionObj;

	if (strcmp(name, "PerformList") == 0)
		return new TPerformList;

	if (strcmp(name, "GXAlphaUpdate") == 0)
		return new TGXAlphaUpdate;

	if (strcmp(name, "ReInitGX") == 0)
		return new TReInitGX;

	if (strcmp(name, "J3DSysSetViewMtx") == 0)
		return new TJ3DSysSetViewMtx;

	if (strcmp(name, "SMSDrawInit") == 0)
		return new TSMSDrawInit;

	if (strcmp(name, "ZBufferCatch") == 0)
		return new TZBufferCatch;

	if (strcmp(name, "AlphaCatch") == 0)
		return new TAlphaCatch;

	if (strcmp(name, "J3DSysFlag") == 0)
		return new TJ3DSysFlag;

	if (strcmp(name, "Conductor") == 0)
		return gpConductor = new TConductor;

	return JDrama::TNameRefGen::getNameRef(name);
}

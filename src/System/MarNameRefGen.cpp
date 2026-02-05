#include <System/MarNameRefGen.hpp>

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
#include <Map/MapEventSink.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/EffectObj.hpp>
#include <Enemy/AreaCylinder.hpp>
#include <Enemy/Generator.hpp>
#include <Enemy/EnemyTable.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Player/SplashManager.hpp>
#include <Player/MarioMain.hpp>
#include <Player/MarioPositionObj.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <Camera/Camera.hpp>
#include <Camera/CameraMapTool.hpp>

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
		gpMarioAddress  = (size_t)mario;
		return mario;
	}

	if (strcmp(name, "MLight") == 0) {
		TLightMario* light    = new TLightMario;
		gpLightManager->unk10 = light;
		return light;
	}

	if (strcmp(name, "MirrorModelManager") == 0)
		return new TMirrorModelManager;

	if (strcmp(name, "MirrorMapDrawBuf") == 0)
		return new TMirrorMapDrawBuf;

	if (strcmp(name, "Silhouette") == 0)
		return new TSilhouette;

	if (strcmp(name, "ScrnFader") == 0)
		return new TSmplFader(60.0f);

	if (strcmp(name, "ShineFader") == 0)
		return new TShineFader(60.0f);

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

	if (strcmp(name, "SunGlass") == 0)
		return new TSunGlass(JUtility::TColor(0, 0, 0, 80));

	if (strcmp(name, "SunShine") == 0)
		return new TSunShine;

	if (strcmp(name, "CubeCamera") == 0)
		return new TCubeManagerArea("?", "カメラキューブテーブル");

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

	// TODO:
	// if (strcmp(name, "MapEventSirenaSink") == 0)
	// 	return new TMapEventSirenaSink;

	if (strcmp(name, "MapEventSinkBianco") == 0)
		return new TMapEventSinkBianco;

	// TODO:
	// if (strcmp(name, "DolpicEventBiancoGate") == 0)
	// 	return new TDolpicEventBiancoGate;

	// if (strcmp(name, "DolpicEventRiccoGate") == 0)
	// 	return new TDolpicEventRiccoMammaGate;

	// if (strcmp(name, "DolpicEventMammaGate") == 0)
	// 	return new TDolpicEventRiccoMammaGate;

	// if (strcmp(name, "MareEventBumpyWall") == 0)
	// 	return new TMareEventBumpyWall;

	// if (strcmp(name, "MareEventWallRock") == 0)
	// 	return new TMareEventWallRock;

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

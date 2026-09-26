#include <System/MarDirector.hpp>
#include <JSystem/JKernel/JKRFileFinder.hpp>
#include <JSystem/JKernel/JKRMemArchive.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DDrawBuffer.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JDrama/JDRNameRefPtrList.hpp>
#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <JSystem/JDrama/JDRCamera.hpp>
#include <JSystem/JDrama/JDRFrmGXSet.hpp>
#include <JSystem/JDrama/JDREfbCtrl.hpp>
#include <JSystem/JDrama/JDRViewport.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/JKernel/JKRDvdFile.hpp>
#include <JSystem/JKernel/JKRDvdRipper.hpp>
#include <System/Resolution.hpp>
#include <System/EventWatcher.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/RenderModeObj.hpp>
#include <System/SnapTimeObj.hpp>
#include <System/PerformList.hpp>
#include <System/FlagManager.hpp>
#include <System/Application.hpp>
#include <System/StageUtil.hpp>
#include <System/MSoundMainSide.hpp>
#include <System/Params.hpp>
#include <GC2D/ScrnFader.hpp>
#include <MarioUtil/LightUtil.hpp>
#include <MarioUtil/ScreenUtil.hpp>
#include <MSound/MSound.hpp>
#include <Camera/Camera.hpp>
#include <Enemy/Conductor.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <System/DummyStrings.hpp>

extern void* gpSceneCmnDat;
extern int gpSceneCmnDatSize;

void TMarDirector::decideMarioPosIdx()
{
	unkD0 = 0;
	unkD1 = 0;
	unkE4 = 1;

	const TGameSequence& prevArea = SMSGetApplication()->mPrevArea;
	switch (SMSGetApplication()->mCurrArea.getStage()) {
	case 15:
		unkE4 = 14;
		SMSGetApplication()->getFader()->setColor(
		    JUtility::TColor(0x00, 0x00, 0x00, 0xff));
		break;

	case 0:
		if (TFlagManager::getInstance()->getBool(0x30001))
			TFlagManager::getInstance()->setBool(false, 0x30001);
		break;

	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 8:
	case 9: {
		unkE4 = 14;
		SMSGetApplication()->getFader()->setColor(
		    JUtility::TColor(0xd2, 0xd2, 0xd2, 0xff));
		unkD1 = 1;
	} break;

	case 1:
		if (TFlagManager::getInstance()->getBool(0x30001)) {
			TFlagManager::getInstance()->setBool(false, 0x30001);
		} else {
			if (TFlagManager::getInstance()->getBool(0x30004)) {
				TFlagManager::getInstance()->setBool(false, 0x30004);
				unkD0 = 4;
			} else {
				switch (SMS_getShineStage(prevArea.getStage())) {
				case 2:
					unkD0 = 1;
					unkD1 = 2;
					unkE4 = 7;
					break;
				case 3:
					unkD0 = 2;
					unkD1 = 2;
					unkE4 = 7;
					break;
				case 4:
					unkD0 = 3;
					unkD1 = 2;
					unkE4 = 7;
					break;
				case 5:
					unkD0 = 4;
					unkD1 = 2;
					unkE4 = 7;
					break;
				case 6:
					unkD0 = 5;
					unkD1 = 2;
					unkE4 = 7;
					break;
				case 7:
					unkD0 = 6;
					unkD1 = 2;
					unkE4 = 7;
					break;
				case 8:
					unkD0 = 7;
					unkD1 = 2;
					unkE4 = 0xe;
					SMSGetApplication()->getFader()->setColor(
					    JUtility::TColor(0x00, 0x00, 0x00, 0xff));
					break;
				case 9:
					unkD0 = 8;
					unkE4 = 7;
					break;
				}
			}
		}
		break;
	}
}

bool TMarDirector::setupObjects()
{
	TFlagManager::getInstance()->resetStage();
	TFlagManager::getInstance()->setFlag(0x60003, 1);
	const TGameSequence& curArea = SMSGetApplication()->mCurrArea;
	switch (curArea.getStage()) {
	case 1: {
		TFlagManager::getInstance()->setBool(false, 0x3000D);
		TFlagManager::getInstance()->setBool(false, 0x30005);
		if (!TFlagManager::getInstance()->getBool(0x30003)) {
			TFlagManager::getInstance()->setBool(true, 0x30003);
			onDemoFlag(DEMO_FLAG_CAMERA_DEMO_ON_START);
		} else {
			TFlagManager::getInstance()->setBool(true, 0x30000);
		}

		switch (curArea.getScenario()) {
		case 0:
		case 1:
		case 7:
		case 9:
			onDemoFlag(DEMO_FLAG_CAMERA_DEMO_ON_START);
			break;

		case 5:
			if (!TFlagManager::getInstance()->getBool(0x10386)
			    && TFlagManager::getInstance()->getFlag(0x40000) >= 3) {
				TFlagManager::getInstance()->setBool(true, 0x50001);
				onDemoFlag(DEMO_FLAG_CAMERA_DEMO_ON_START);
			}
			if (!TFlagManager::getInstance()->getBool(0x10387)
			    && TFlagManager::getInstance()->getFlag(0x40000) >= 5) {
				TFlagManager::getInstance()->setBool(true, 0x50002);
				onDemoFlag(DEMO_FLAG_CAMERA_DEMO_ON_START);
			}
			break;

		case 8: {
			int iVar6 = TFlagManager::getInstance()->getFlag(0x40000);
			int lVar9 = 0;
			if (!TFlagManager::getInstance()->getBool(0x1038F)
			    && TFlagManager::getInstance()->getShineFlag(33)) {
				lVar9 = true;
				onDemoFlag(DEMO_FLAG_CAMERA_DEMO_ON_START);
			}
			if (lVar9 == 0 && !TFlagManager::getInstance()->getNozzleRight(1, 1)
			    && TFlagManager::getInstance()->getBool(0x1038F)
			    && iVar6 >= 25) {
				lVar9 = 2;
				onDemoFlag(DEMO_FLAG_CAMERA_DEMO_ON_START);
			}
			if (lVar9 == 0 && !TFlagManager::getInstance()->getNozzleRight(1, 0)
			    && TFlagManager::getInstance()->getNozzleRight(1, 1)
			    && iVar6 >= 30) {
				lVar9 = 3;
				onDemoFlag(DEMO_FLAG_CAMERA_DEMO_ON_START);
			}
			TFlagManager::getInstance()->setFlag(0x60003, lVar9);
			if (TFlagManager::getInstance()->getFlag(0x40000) >= 20
			    && !TFlagManager::getInstance()->getFlag(0x60003)) {
				onDemoFlag(DEMO_FLAG_CAMERA_DEMO_ON_START);
			}
		}
			// FALLTHROUGH!!!

		case 2:
			if (TFlagManager::getInstance()->getFlag(0x40000) >= 20)
				TFlagManager::getInstance()->setBool(true, 0x50004);
			break;
		}
		break;
	}
	case 5:
		switch (curArea.getScenario()) {
		case 3:
			TFlagManager::getInstance()->setBool(true, 0x50003);
			break;
		}
		break;
	}

	u32 flag = SMS_getShineStage(curArea.getStage()) + 0x103A5;
	TFlagManager::getInstance()->setBool(true, flag);

	MSMainProc::setMSoundEnterStage(mMap, unk7D);
	if (!TFlagManager::getInstance()->getBool(0x30007)) {
		TFlagManager::getInstance()->setBool(true, 0x30007);
		gpMSound->loadWave(MS_WAVE_DEFAULT);
	}
	gpMSound->initSound();

	unk10 = nullptr;
	decideMarioPosIdx();
	createObjects();
	TParams::init();

	JDrama::TNameRef* sceneCommon;
	{
		JSUMemoryInputStream stream(gpSceneCmnDat, gpSceneCmnDatSize);
		sceneCommon = JDrama::TNameRefGen::getInstance()->load(stream);
	}

	JDrama::TViewObjPtrListT<JDrama::TViewObj>* root
	    = static_cast<JDrama::TViewObjPtrListT<JDrama::TViewObj>*>(
	        JDrama::TNameRefGen::search("Root View Obj"));

	JDrama::TViewObjPtrListT<JDrama::TViewObj>* gameObjs;
	if (root) {
		gameObjs = (JDrama::TViewObjPtrListT<JDrama::TViewObj>*)root->search(
		    "ゲームオブジェクト");
	} else {
		gameObjs = static_cast<JDrama::TViewObjPtrListT<JDrama::TViewObj>*>(
		    JDrama::TNameRefGen::search("ゲームオブジェクト"));
	}

	gameObjs->insert(gpMarioParticleManager);
	gameObjs->insert(new JDrama::TOrthoProj(-1.0f, 1.0f, 0.0f,
	                                        (u16)SMSGetGameRenderHeight(), 0.0f,
	                                        (u16)SMSGetGameRenderWidth()));

	JDrama::TViewObjPtrListT<JDrama::TViewObj>* measurementGroup
	    = new JDrama::TViewObjPtrListT<JDrama::TViewObj>("計測グループ");
	gameObjs->insert(measurementGroup);

	measurementGroup->insert(
	    new TSnapTimeObj(0xFFFFFFFF, "Mirror Draw SnapTime"));
	measurementGroup->insert(
	    new TSnapTimeObj(0xFF00FFFF, "Pollution Check SnapTime"));
	measurementGroup->insert(
	    new TSnapTimeObj(0x0000FFFF, "Pollution Draw SnapTime"));
	measurementGroup->insert(new TSnapTimeObj(0xFFFFFFFF, "Map Draw SnapTime"));
	measurementGroup->insert(
	    new TSnapTimeObj(0x0000FFFF, "MapObj Draw SnapTime"));
	measurementGroup->insert(
	    new TSnapTimeObj(0xFF00FFFF, "Player Draw SnapTime"));
	measurementGroup->insert(
	    new TSnapTimeObj(0xFF00FFFF, "Water Draw SnapTime"));
	measurementGroup->insert(new TSnapTimeObj(0xFFFF00FF, "Sky Draw SnapTime"));
	measurementGroup->insert(
	    new TSnapTimeObj(0xFF00FFFF, "PollutionModel Draw SnapTime"));
	measurementGroup->insert(
	    new TSnapTimeObj(0xFF0000FF, "Shadow Draw SnapTime"));
	measurementGroup->insert(
	    new TSnapTimeObj(0x000000FF, "Silhouette Draw SnapTime"));
	measurementGroup->insert(
	    new TSnapTimeObj(0xFFFFFFFF, "Chara Draw SnapTime"));
	measurementGroup->insert(
	    new TSnapTimeObj(0x00FF00FF, "Indirect Draw SnapTime"));
	measurementGroup->insert(
	    new TSnapTimeObj(0xFFFFFFFF, "Particle Draw SnapTime"));

	gpConductor->makeGraphGroup(JKRGetResource("/scene/map/scene.ral"));

	{
		void* tables = JKRFileLoader::getGlbResource("/scene/map/tables.bin");
		if (tables) {
			u32 size = unkB8->getResSize(tables);
			JSUMemoryInputStream stream(tables, size);
			JSUMemoryInputStream leftoversStream(nullptr, 0);
			JDrama::TViewObj* obj
			    = (JDrama::TViewObj*)JDrama::TNameRef::genObject(
			        stream, leftoversStream);
			if (obj) {
				gameObjs->insert(obj);
				obj->load(leftoversStream);
			}
		}
	}

	{
		void* scene = JKRFileLoader::getGlbResource("/scene/map/scene.bin");
		u32 size    = unkB8->getResSize(scene);
		JSUMemoryInputStream stream(scene, size);
		JSUMemoryInputStream leftoversStream(nullptr, 0);
		JDrama::TViewObj* obj = (JDrama::TViewObj*)JDrama::TNameRef::genObject(
		    stream, leftoversStream);
		if (obj) {
			gameObjs->insert(obj);
			obj->load(leftoversStream);
		}

		JDrama::TLookAtCamera* cam = static_cast<JDrama::TLookAtCamera*>(
		    JDrama::TNameRefGen::search("camera 1"));
#ifdef VERSION_GMSP01
		cam->mAspect = (u16)SMSGetGameVideoWidth() * 0.9134614f
		               / (u16)SMSGetGameRenderHeight();
#else
		cam->mAspect = (u16)SMSGetGameVideoWidth() * 0.9134614f
		               / (u16)SMSGetGameVideoHeight();
#endif
	}

	unk80 = new JDrama::TViewObjPtrListT<JDrama::TViewObj>("イベントグループ");
	gameObjs->insert(unk80);

	JKRFileFinder* finder = JKRFileLoader::findFirstFile("/common/sp");
	if (finder) {
		JKRFileLoader::changeDirectory("/common/sp");
		do {
			if (strstr(finder->mBase.mFileName, ".sb")) {
				registerEventWatcher(new TEventWatcher(
				    "<EventWatcher>", finder->mBase.mFileName));
			}
		} while (finder->findNextFile());
		delete finder;
		JKRFileLoader::changeDirectory("/");
	}

	finder = JKRFileLoader::findFirstFile("/scene/map/sp");
	if (finder) {
		JKRFileLoader::changeDirectory("/scene/map/sp");
		do {
			if (strstr(finder->mBase.mFileName, ".sb")) {
				registerEventWatcher(new TEventWatcher(
				    "<EventWatcher>", finder->mBase.mFileName));
			}
		} while (finder->findNextFile());
		delete finder;
		JKRFileLoader::changeDirectory("/");
	}

	TParams::finalize();

	((JKRMemArchive*)JKRFileLoader::getVolume("params"))->unmountFixed();
	JKRHeap::getCurrentHeap()->freeTail();
	sceneCommon->loadAfter();

	gpConductor->initGraphGroup();
	gpConductor->init();

	JDrama::TViewObjPtrListT<JDrama::TViewObj>* normalScene
	    = (JDrama::TViewObjPtrListT<JDrama::TViewObj>*)root->search(
	        "通常シーン");
	if (!normalScene)
		normalScene = root;

	normalScene->insert(gpConductor);
	gpLightManager->makeDrawBuffer();
	normalScene->insert(gpLightManager);

	gpCamera->setNoticeInfo();
	unk10 = root;

	JDrama::TViewObjPtrListT<JDrama::TViewObj>* perfEventGroup
	    = new JDrama::TViewObjPtrListT<JDrama::TViewObj>("PERF Event Group");
	root->getChildren().push_back(perfEventGroup);
	JDrama::TFrmGXSet* drawInit = new JDrama::TFrmGXSet(unkC0);

	JDrama::TViewObjPtrListT<JDrama::TViewObj>* drawBufferGroup
	    = static_cast<JDrama::TViewObjPtrListT<JDrama::TViewObj>*>(
	        JDrama::TNameRefGen::search("Draw Buffer Group"));
	static_cast<JDrama::TDrawBufObj*>(
	    JDrama::TNameRefGen::search("DrawBuf Sky Opa"))
	    ->getDrawBuffer()
	    ->setNonSort();
	static_cast<JDrama::TDrawBufObj*>(
	    JDrama::TNameRefGen::search("DrawBuf Sky Xlu"))
	    ->getDrawBuffer()
	    ->setNonSort();
	static_cast<JDrama::TDrawBufObj*>(
	    JDrama::TNameRefGen::search("DrawBuf Graffito"))
	    ->getDrawBuffer()
	    ->setMatAnmSort();
	gpLightManager->addChildGroupObj(drawBufferGroup);
	unk40->push_back(drawInit, CUE_DRAW);
	initECTGft(unk38, unk3C, perfEventGroup, root);
	initECTMir(mPerformListGX, perfEventGroup);

	JDrama::TEfbCtrlTex* normalSceneDrawStage
	    = static_cast<JDrama::TEfbCtrlTex*>(
	        JDrama::TNameRefGen::search("通常シーン描画ステージ"));
	normalSceneDrawStage->unk20.on(0x122F);
	normalSceneDrawStage->mVFilter = SMSVFilter_flicker;
	TScreenTexture* screenTex
	    = (TScreenTexture*)sceneCommon->search("スクリーンテクスチャ");

	normalSceneDrawStage->setTexAttb(screenTex->getTexture()->getTexObj());

	JDrama::TRect local_dc(0, 0, (u16)SMSGetGameRenderWidth(),
	                       (u16)SMSGetGameRenderHeight());
	normalSceneDrawStage->setSrcRect(local_dc);

	JDrama::TViewport* normalSceneViewport = static_cast<JDrama::TViewport*>(
	    JDrama::TNameRefGen::search("通常シーンViewport"));
	normalSceneViewport->unk10 = local_dc;

	{
		JKRDvdFile file;
		file.open("/data/PerformLists.bin");
		s32 size   = file.getFileSize();
		void* data = JKRDvdRipper::loadToMainRAM(
		    &file, nullptr, EXPAND_SWITCH_DEFAULT, 0, nullptr,
		    JKRDvdRipper::ALLOC_DIRECTION_FORWARD, 0, nullptr);

		{
			JSUMemoryInputStream stream(data, size);
			JSUMemoryInputStream leftoversStream(nullptr, 0);
			JDrama::TViewObj* performLists
			    = (JDrama::TViewObj*)JDrama::TNameRef::genObject(
			        stream, leftoversStream);
			gameObjs->insert(performLists);
			performLists->load(leftoversStream);
		}
	}

	mPerformListMovement = static_cast<TPerformList*>(
	    JDrama::TNameRefGen::search("PerformList Movement"));
	mPerformListCalcAnim = static_cast<TPerformList*>(
	    JDrama::TNameRefGen::search("PerformList CalcAnim"));
	mPerformListGX = static_cast<TPerformList*>(
	    JDrama::TNameRefGen::search("PerformList GX"));
	mPerformListSilhouette = static_cast<TPerformList*>(
	    JDrama::TNameRefGen::search("PerformList Silhouette"));
	mPerformListGXPost = static_cast<TPerformList*>(
	    JDrama::TNameRefGen::search("PerformList GX Post"));
	mShinePfLstMov = static_cast<TPerformList*>(
	    JDrama::TNameRefGen::search("Shine PfLst Mov"));
	mShinePfLstAnm = static_cast<TPerformList*>(
	    JDrama::TNameRefGen::search("Shine PfLst Anm"));

	initECDisp(mPerformListGXPost, perfEventGroup, root);

	JDrama::TViewObj* composite3
	    = static_cast<JDrama::TViewObj*>(JDrama::TNameRefGen::search("合成3"));
	mPerformListMovement->push_back(composite3, CUE_MOVE);
	JDrama::TViewObj* specularSheen = static_cast<JDrama::TViewObj*>(
	    JDrama::TNameRefGen::search("スペキュラシーン"));
	if (specularSheen)
		mPerformListMovement->push_back(specularSheen, CUE_MOVE);

	JDrama::TViewObj* lensFlare = static_cast<JDrama::TViewObj*>(
	    JDrama::TNameRefGen::search("レンズフレア"));
	JDrama::TViewObj* sunOcclusionGlow = nullptr;
	if (lensFlare) {
		sunOcclusionGlow = static_cast<JDrama::TViewObj*>(
		    JDrama::TNameRefGen::search("太陽遮蔽物グロー"));
		mPerformListMovement->push_back(sunOcclusionGlow, CUE_MOVE);
		mPerformListMovement->push_back(lensFlare, CUE_MOVE);
	}

	JDrama::TViewObj* dialogueCursor = static_cast<JDrama::TViewObj*>(
	    JDrama::TNameRefGen::search("会話カーソル"));
	JDrama::TViewObj* targetArrow = static_cast<JDrama::TViewObj*>(
	    JDrama::TNameRefGen::search("ターゲット矢印"));

	mPerformListMovement->push_back(dialogueCursor, CUE_MOVE);
	mPerformListCalcAnim->push_back(composite3, CUE_CALC_ANIM);

	if (specularSheen)
		mPerformListCalcAnim->push_back(specularSheen, CUE_CALC_ANIM);
	if (lensFlare) {
		mPerformListCalcAnim->push_back(sunOcclusionGlow, CUE_CALC_ANIM);
		mPerformListCalcAnim->push_back(lensFlare, CUE_CALC_ANIM);
	}

	mPerformListCalcAnim->push_back(dialogueCursor, CUE_CALC_ANIM);
	mPerformListCalcAnim->push_back(targetArrow, CUE_CALC_ANIM);
	setupPerformList_console();
	mPerformListGXPost->push_back(drawInit, CUE_DRAW_STAGE_END);
	preEntry(unk34);
	setup2();
	JKRHeap::getCurrentHeap()->getTotalFreeSize();

	return 0;
}

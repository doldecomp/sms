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

extern void* gpSceneCmnDat;
extern int gpSceneCmnDatSize;

void TMarDirector::decideMarioPosIdx()
{
	unkD0 = 0;
	unkD1 = 0;
	unkE4 = 1;

	switch (gpApplication.mCurrArea.unk0) {
	case 15:
		unkE4 = 14;
		gpApplication.mFader->setColor(
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
	case 7:
	case 8: {
		unkE4 = 14;
		gpApplication.mFader->setColor(
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
				switch (SMS_getShineStage(gpApplication.mPrevArea.unk0)) {
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
					gpApplication.mFader->setColor(
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
	switch (gpApplication.mCurrArea.unk0) {
	case 1: {
		TFlagManager::getInstance()->setBool(true, 0x3000D);
		TFlagManager::getInstance()->setBool(true, 0x30005);
		if (!TFlagManager::getInstance()->getBool(0x30003)) {
			TFlagManager::getInstance()->setBool(true, 0x30003);
			unk4E |= 0x2;
		} else {
			TFlagManager::getInstance()->setBool(true, 0x30000);
		}

		switch (gpApplication.mCurrArea.unk1) {
		case 0:
		case 1:
		case 7:
		case 9:
			unk4E |= 0x2;
			break;

		case 5:
			if (!TFlagManager::getInstance()->getBool(0x10386)
			    && TFlagManager::getInstance()->getFlag(0x40000) >= 3) {
				TFlagManager::getInstance()->setBool(true, 0x50001);
				unk4E |= 0x2;
			}
			if (!TFlagManager::getInstance()->getBool(0x10387)
			    && TFlagManager::getInstance()->getFlag(0x40000) >= 5) {
				TFlagManager::getInstance()->setBool(true, 0x50002);
				unk4E |= 0x2;
			}
			break;

		case 8: {
			int iVar6 = TFlagManager::getInstance()->getFlag(0x40000);
			int lVar9 = 0;
			if (!TFlagManager::getInstance()->getBool(0x1038F)
			    && TFlagManager::getInstance()->getShineFlag(33)) {
				lVar9 = true;
				unk4E |= 0x2;
			}
			if (lVar9 == 0 && !TFlagManager::getInstance()->getNozzleRight(1, 1)
			    && TFlagManager::getInstance()->getBool(0x1038F)
			    && iVar6 >= 25) {
				lVar9 = 2;
				unk4E |= 0x2;
			}
			if (lVar9 == 0 && !TFlagManager::getInstance()->getNozzleRight(1, 0)
			    && TFlagManager::getInstance()->getNozzleRight(1, 1)
			    && iVar6 >= 30) {
				lVar9 = 3;
				unk4E |= 0x2;
			}
			TFlagManager::getInstance()->setFlag(0x60003, lVar9);
			if (TFlagManager::getInstance()->getFlag(0x40000) >= 20
			    && !TFlagManager::getInstance()->getFlag(0x60003)) {
				unk4E |= 0x2;
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
		if (gpApplication.mCurrArea.unk1 != 3)
			(void)gpApplication.mCurrArea.unk1;
		else
			TFlagManager::getInstance()->setBool(true, 0x50003);
		break;
	}

	u32 bVar28 = SMS_getShineStage(gpApplication.mCurrArea.unk0);
	TFlagManager::getInstance()->setBool(true, 0x103A5 + bVar28);

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

	JDrama::TNameRef* root
	    = JDrama::TNameRefGen::search<JDrama::TNameRef>("Root View Obj");

	JDrama::TNameRefPtrListT<JDrama::TViewObj>* gameObjs;
	if (root) {
		gameObjs = (JDrama::TNameRefPtrListT<JDrama::TViewObj>*)root->search(
		    "ゲームオブジェクト");
	} else {
		gameObjs = JDrama::TNameRefGen::search<
		    JDrama::TNameRefPtrListT<JDrama::TViewObj> >("ゲームオブジェクト");
	}

	gameObjs->insert(gpMarioParticleManager);
	gameObjs->insert(new JDrama::TOrthoProj(0.0f, 0.0f,
	                                        (u16)SMSGetGameRenderWidth(),
	                                        (u16)SMSGetGameRenderHeight()));

	JDrama::TViewObjPtrListT<JDrama::TViewObj>* measurementGroup
	    = new JDrama::TViewObjPtrListT<JDrama::TViewObj>("計測グループ");
	gameObjs->insert(measurementGroup);

	measurementGroup->insert(
	    new TSnapTimeObj(0xFFFFFFFF, "Mirror Draw SnapTime"));
	measurementGroup->insert(
	    new TSnapTimeObj(0xFF00FFFF, "Pollution Check SnapTime"));
	measurementGroup->insert(
	    new TSnapTimeObj(0xFF00FFFF, "Pollution Draw SnapTime"));
	measurementGroup->insert(new TSnapTimeObj(0xFFFFFFFF, "Map Draw SnapTime"));
	measurementGroup->insert(
	    new TSnapTimeObj(0xFF00FFFF, "MapObj Draw SnapTime"));
	measurementGroup->insert(
	    new TSnapTimeObj(0xFF00FFFF, "Player Draw SnapTime"));
	measurementGroup->insert(
	    new TSnapTimeObj(0xFF00FFFF, "Water Draw SnapTime"));
	measurementGroup->insert(new TSnapTimeObj(0xFFFF00FF, "Sky Draw SnapTime"));
	measurementGroup->insert(
	    new TSnapTimeObj(0xFF00FFFF, "PollutionModel Draw SnapTime"));
	measurementGroup->insert(
	    new TSnapTimeObj(0xFFFF00FF, "Shadow Draw SnapTime"));
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
		void* tables = JKRGetResource("/scene/map/tables.bin");
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
		void* scene = JKRGetResource("/scene/map/scene.bin");
		u32 size    = unkB8->getResSize(scene);
		JSUMemoryInputStream stream(scene, size);
		JSUMemoryInputStream leftoversStream(nullptr, 0);
		JDrama::TViewObj* obj = (JDrama::TViewObj*)JDrama::TNameRef::genObject(
		    stream, leftoversStream);
		if (obj) {
			gameObjs->insert(obj);
			obj->load(leftoversStream);
		}

		JDrama::TLookAtCamera* cam
		    = JDrama::TNameRefGen::search<JDrama::TLookAtCamera>("camera 1");
		cam->mAspect = (u16)SMSGetGameVideoWidth() * 0.9134614f
		               / (u16)SMSGetGameVideoHeight();
	}

	unk80 = new JDrama::TViewObjPtrListT<JDrama::TViewObj>("イベントグループ");
	gameObjs->insert(unk80);

	if (JKRFileFinder* finder = JKRFileLoader::findFirstFile("/common/sp")) {
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

	if (JKRFileFinder* finder = JKRFileLoader::findFirstFile("/scene/map/sp")) {
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

	// Type is a guess
	JDrama::TViewObjPtrListT<JDrama::TViewObj>* normalScene
	    = (JDrama::TViewObjPtrListT<JDrama::TViewObj>*)root->search(
	        "通常シーン");
	if (!normalScene)
		normalScene = (JDrama::TViewObjPtrListT<JDrama::TViewObj>*)root;

	normalScene->insert(gpConductor);
	gpLightManager->makeDrawBuffer();
	normalScene->insert(gpLightManager);

	gpCamera->setNoticeInfo();
	unk10 = normalScene;

	JDrama::TViewObjPtrListT<JDrama::TViewObj>* perfEventGroup
	    = new JDrama::TViewObjPtrListT<JDrama::TViewObj>("PERF Event Group");
	normalScene->push_back(perfEventGroup);
	JDrama::TFrmGXSet* drawInit = new JDrama::TFrmGXSet(unkC0);

	JDrama::TViewObjPtrListT<JDrama::TViewObj>* drawBufferGroup
	    = JDrama::TNameRefGen::search<
	        JDrama::TViewObjPtrListT<JDrama::TViewObj> >("Draw Buffer Group");
	JDrama::TNameRefGen::search<JDrama::TDrawBufObj>("DrawBuf Sky Opa")
	    ->getDrawBuffer()
	    ->mSortType
	    = J3DDrawBuffer::SORT_NON;
	JDrama::TNameRefGen::search<JDrama::TDrawBufObj>("DrawBuf Sky Xlu")
	    ->getDrawBuffer()
	    ->mSortType
	    = J3DDrawBuffer::SORT_NON;
	JDrama::TNameRefGen::search<JDrama::TDrawBufObj>("DrawBuf Graffito")
	    ->getDrawBuffer()
	    ->mSortType
	    = J3DDrawBuffer::SORT_MAT_ANM;
	gpLightManager->addChildGroupObj(drawBufferGroup);
	unk40->push_back(drawBufferGroup, 8);
	initECTGft(unk38, unk3C, perfEventGroup, normalScene);
	initECTMir(mPerformListGX, perfEventGroup);

	JDrama::TEfbCtrlTex* normalSceneDrawStage
	    = JDrama::TNameRefGen::search<JDrama::TEfbCtrlTex>(
	        "通常シーン描画ステージ");
	normalSceneDrawStage->unk20.on(0x122F);
	normalSceneDrawStage->unk44 = SMSVFilter_flicker;
	TScreenTexture* screenTex
	    = (TScreenTexture*)sceneCommon->search("スクリーンテクスチャ");

	GXTexObj sctex = screenTex->getTexture()->mTexObj;
	normalSceneDrawStage->setTexAttb(sctex);

	JDrama::TRect local_dc(0, 0, (u16)SMSGetGameRenderWidth(),
	                       (u16)SMSGetGameRenderHeight());
	normalSceneDrawStage->setSrcRect(local_dc);

	JDrama::TViewport* normalSceneViewport
	    = JDrama::TNameRefGen::search<JDrama::TViewport>("通常シーンViewport");
	normalSceneViewport->unk10 = local_dc;

	{
		JKRDvdFile auStack_1d8;
		auStack_1d8.open("/data/PerformLists.bin");
		JKRDvdRipper::loadToMainRAM(
		    &auStack_1d8, nullptr, EXPAND_SWITCH_DEFAULT, 0, nullptr,
		    JKRDvdRipper::ALLOC_DIRECTION_FORWARD, 0, nullptr);

		{
			JSUMemoryInputStream stream(auStack_1d8.getFileInfo(),
			                            auStack_1d8.getFileSize());
			JSUMemoryInputStream leftoversStream(nullptr, nullptr);
			JDrama::TViewObj* performLists
			    = (JDrama::TViewObj*)JDrama::TNameRef::genObject(
			        stream, leftoversStream);
			gameObjs->insert(performLists);
			performLists->load(leftoversStream);
		}
	}

	mPerformListMovement
	    = JDrama::TNameRefGen::search<TPerformList>("PerformList Movement");
	mPerformListCalcAnim
	    = JDrama::TNameRefGen::search<TPerformList>("PerformList CalcAnim");
	mPerformListGX
	    = JDrama::TNameRefGen::search<TPerformList>("PerformList GX");
	mPerformListSilhouette
	    = JDrama::TNameRefGen::search<TPerformList>("PerformList Silhouette");
	mPerformListGXPost
	    = JDrama::TNameRefGen::search<TPerformList>("PerformList GX Post");
	mShinePfLstMov
	    = JDrama::TNameRefGen::search<TPerformList>("Shine PfLst Mov");
	mShinePfLstAnm
	    = JDrama::TNameRefGen::search<TPerformList>("Shine PfLst Anm");

	initECDisp(mPerformListGXPost, perfEventGroup, normalScene);

	mPerformListMovement->push_back(
	    JDrama::TNameRefGen::search<JDrama::TViewObj>("合成3"), 1);
	JDrama::TViewObj* specularSheen
	    = JDrama::TNameRefGen::search<JDrama::TViewObj>("スペキュラシーン");
	if (specularSheen)
		mPerformListMovement->push_back(specularSheen, 1);

	JDrama::TViewObj* lensFlare
	    = JDrama::TNameRefGen::search<JDrama::TViewObj>("レンズフレア");
	JDrama::TViewObj* sunOcclusionGlow = nullptr;
	if (lensFlare) {
		sunOcclusionGlow
		    = JDrama::TNameRefGen::search<JDrama::TViewObj>("太陽遮蔽物グロー");
		mPerformListMovement->push_back(sunOcclusionGlow, 1);
		mPerformListMovement->push_back(lensFlare, 1);
	}

	JDrama::TViewObj* dialogueCursor
	    = JDrama::TNameRefGen::search<JDrama::TViewObj>("会話カーソル");
	JDrama::TViewObj* targetArrow
	    = JDrama::TNameRefGen::search<JDrama::TViewObj>("ターゲット矢印");

	mPerformListMovement->push_back(dialogueCursor, 1);
	mPerformListCalcAnim->push_back(dialogueCursor, 2);

	if (specularSheen)
		mPerformListCalcAnim->push_back(specularSheen, 2);
	if (lensFlare) {
		mPerformListCalcAnim->push_back(lensFlare, 2);
		mPerformListCalcAnim->push_back(sunOcclusionGlow, 2);
	}

	mPerformListCalcAnim->push_back(dialogueCursor, 2);
	mPerformListCalcAnim->push_back(targetArrow, 2);
	setupPerformList_console();
	mPerformListGXPost->push_back(drawInit, 0x100);
	preEntry(unk34);
	setup2();
	JKRHeap::getCurrentHeap()->freeTail();

	return 0;
}

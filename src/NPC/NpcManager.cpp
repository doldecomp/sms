#include <NPC/NpcManager.hpp>
#include <stdio.h>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAttach.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <M3DUtil/MActorData.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <MarioUtil/ScreenUtil.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <Strategic/ObjManager.hpp>
#include <Strategic/ObjModel.hpp>
#include <System/MarDirector.hpp>
#include <Enemy/Conductor.hpp>
#include <Camera/Camera.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <NPC/NpcBase.hpp>
#include <NPC/NpcInitData.hpp>
#include <NPC/NpcSave.hpp>

// rogue
#include <M3DUtil/InfectiousStrings.hpp>
static const char dummy[] = "__ROOT_JOINT__";

extern bool isNowInbetween__15CPolarSubCameraCFv(const CPolarSubCamera*);

TModelDataKeeper* TMonteMBaseManager::mStaticCommonKeeper;
TModelDataKeeper* TMonteWBaseManager::mStaticCommonKeeper;
TModelDataKeeper* TMareMBaseManager::mStaticCommonKeeper;
TModelDataKeeper* TMareWBaseManager::mStaticCommonKeeper;
J3DMaterialTable* TMareBaseManager::mStaticBmtNormal;
J3DMaterialTable* TMareBaseManager::mStaticBmtPollution;

TMareJellyFishManager* gpMareJellyFishManager;

const char* cDummyPollutionTexName   = "H_ma_rak_dummy";
const char* cRealPollutionTexName    = "/scene/map/pollution/H_ma_rak.bti";
const char* cMonteMDummyStrawTexName = "I_mom_mino_dummyI4";
const char* cMonteWDummyStrawTexName = "I_mow_mino_dummyI4";
const char* cMonteMRealStrawTexName = "/scene/monteMCommon/I_mom_mino_rgba.bti";
const char* cMonteWRealStrawTexName = "/scene/monteWCommon/I_mow_mino_rgba.bti";
const char* cScreenTexViewObjName   = "スクリーンテクスチャ";
const char* cJellyFishDummyScreenTexName = "dummy_8x8i4";
const char* cJellyFishDummyTexName       = "J_jellyFish_dummy";
const char* cJellyFishRealTexName        = "/scene/mareJellyFish/J_kurage.bti";
const char* cMonteMCommonVolumeName      = "/scene/monteMCommon";
const char* cMonteWCommonVolumeName      = "/scene/monteWCommon";
const char* cMareMCommonVolumeName       = "/scene/mareM";
const char* cMareWCommonVolumeName       = "/scene/mareW";
const char* cMareCommonNormalBmtName     = "/scene/mareCommon/mare.bmt";
const char* cMareCommonPollutionBmtName  = "/scene/mareCommon/mare_yogore.bmt";

void TBoardNpcManager::clipActors(JDrama::TGraphics* graphics)
{
	clipActorsAux(graphics, *(f32*)((u8*)gpConductor + 0x9C), 200.0f);
}

void TBoardNpcManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "boardNpc.bmd", 0x10220000, 0 },
		{ nullptr, 0, 0 },
	};

	createModelDataArray(entry);
}

TMareJellyFishManager::TMareJellyFishManager(const char* name)
    : TObjManager(name)
{
	gpMareJellyFishManager = this;
}

void TMareJellyFishManager::perform(u32, JDrama::TGraphics*) { }

void TMareJellyFishManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "jellyFish_A.bmd", 0x11240000, 0 },
		{ "jellyFish_B.bmd", 0x11240000, 0 },
		{ "jellyFish_C.bmd", 0x11240000, 0 },
		{ "jellyFish_D.bmd", 0x11240000, 0 },
		{ "jellyFish_E.bmd", 0x11240000, 0 },
		{ "jellyFish_F.bmd", 0x11240000, 0 },
		{ nullptr, 0, 0 },
	};

	createModelDataArray(entry);

	const ResTIMG* realTex
	    = (const ResTIMG*)JKRFileLoader::getGlbResource(cJellyFishRealTexName);
	TScreenTexture* screenTex
	    = JDrama::TNameRefGen::search<TScreenTexture>(cScreenTexViewObjName);
	const ResTIMG* screenTexInfo = screenTex->getTexture()->getTexInfo();

	J3DModelData* modelData;
	int modelDataNum = getModelDataKeeper()->getModelDataNum();
	for (int i = 0; i < modelDataNum; ++i) {
		modelData = getModelDataKeeper()->getNthData(i)->getModelData();
		SMS_ChangeTextureAll(modelData, cJellyFishDummyTexName, *realTex);
		SMS_ChangeTextureAll(modelData, cJellyFishDummyScreenTexName,
		                     *screenTexInfo);
	}
}

TNPCManager::TNPCManager(const char* name)
    : TEnemyManager(name)
{
	unk54 = 350.0f;
	unk58 = nullptr;
	unk5C = nullptr;
	unk60 = nullptr;

	u8 area                   = gpMarDirector->getCurrentMap();
	TNpcSaveStageFarClip* ptr = gpConductor->getNpcParams()->unk0;

	switch (area) {
	case 0:
		unk58 = &ptr->mSLFarAirport.get();
		break;
	case 1:
		unk58 = &ptr->mSLFarDolpicTown.get();
		break;
	case 2:
		unk58 = &ptr->mSLFarBiancoHills.get();
		break;
	case 3:
		unk58 = &ptr->mSLFarRiccoHarbor.get();
		break;
	case 4:
		unk58 = &ptr->mSLFarMammaBeach.get();
		break;
	case 5:
		unk58 = &ptr->mSLFarPinnaBeach.get();
		break;
	case 13:
		unk58 = &ptr->mSLFarPinnaParco.get();
		break;
	case 6:
		unk58 = &ptr->mSLFarSirenaBeach.get();
		break;
	case 7:
		unk58 = &ptr->mSLFarHotelDelfino.get();
		break;
	case 9:
		unk58 = &ptr->mSLFarMareVillage.get();
		break;
	case 8:
		unk58 = &ptr->mSLFarMonteVillage.get();
		break;
	case 10:
		unk58 = &ptr->mSLFarCoronaMountain.get();
		break;

	default:
		unk58 = &ptr->mSLFarOthers.get();
		break;
	}
}

void TNPCManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	unk3C = 250.0f;
}

void TNPCManager::makePartsModelData_(u32 npc_type, u32 flags,
                                      TModelDataKeeper* keeper)
{
	const TNpcInitInfo* initInfo = SMSGetNpcInitData(npc_type);
	u32 loadFlags;

	for (int i = 0; i < 12; ++i) {
		const TNpcModelData* modelData = initInfo->unk4[i];
		if (modelData == nullptr)
			continue;

		loadFlags = flags;
		if (modelData->unk2A != 0) {
			loadFlags &= ~(0x40000 | 0x20000 | 0x10000);
			loadFlags |= 0x100000;
		}

		for (int j = 0; j < 2; ++j) {
			if (modelData->unk8[j] == nullptr)
				continue;

			char path[0x100];
			snprintf(path, sizeof(path), "%s/%s", keeper->mFolder,
			         modelData->unk8[j]);
			if (JKRGetResource(path) == nullptr)
				continue;

			SDLModelData* sdlModel
			    = keeper->createAndKeepData(modelData->unk8[j], loadFlags);

			if (modelData->unk2B != 0) {
				J3DMaterialTable* bmt = getBmt_(modelData->unk2A);
				if (bmt != nullptr) {
					sdlModel->getModelData()->setMaterialTable(
					    bmt, J3DMatCopyFlag_All);
				}
			}

			if (modelData->unk2A)
				changeTextureToPollution_(sdlModel->getModelData());
		}
	}
}

J3DMaterialTable* TNPCManager::getBmt_(bool) { return nullptr; }

SDLModelData* TNPCManager::getPartsSDLModelData(const char* name) const
{
	SDLModelData* result = nullptr;

	if (unk5C != nullptr)
		result = unk5C->getDataByName(name);

	if (result == nullptr && unk60 != nullptr)
		result = unk60->getDataByName(name);

	return result;
}

void TNPCManager::clipEnemies(JDrama::TGraphics* graphics)
{
	f32 nearClip = unk54;
	f32 farClip  = *unk58;

	if (gpMarDirector->mMap == 1) {
		CPolarSubCamera* cam = gpCamera;

		// TODO: need camera inlines!
		bool shouldClamp = !cam->isSimpleDemoCamera()
		                   && ((cam->mMode == 0x49) ? true : false);

		if (!(shouldClamp ? true : false) && gpCamera->mMode != 0xD
		    && gpCamera->unk54 == 0xD
		    && (gpCamera->isNowInbetween() || gpCamera->mMode == 0x13)) {

		} else if (farClip < 15000.0f)
			farClip = 15000.0f;
	}

	SetViewFrustumClipCheckPerspective(gpCamera->mAspect, gpCamera->mFovy,
	                                   nearClip, farClip);

	for (int i = 0, e = mObjNum; i < e; ++i) {
		TBaseNPC* actor = (TBaseNPC*)unk18[i];

		JGeometry::TVec3<f32> checkPos = actor->mPosition;
		checkPos.y += 75.0f;

		if (actor->checkLiveFlag(LIVE_FLAG_UNK2000)
		    && SMS_IsInOtherFastCube(checkPos)) {
			actor->onLiveFlag(LIVE_FLAG_CLIPPED_OUT);
			continue;
		}

		if (ViewFrustumClipCheck(graphics, actor->mPosition, unk3C)) {
			actor->offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		} else {
			actor->onLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		}
	}
}

void TNPCManager::perform(u32 flags, JDrama::TGraphics* graphics)
{
	if (flags & 0x200) {
		for (int i = 0, e = mObjNum; i < e; ++i) {
			TBaseNPC* npc = (TBaseNPC*)unk18[i];
			npc->onLiveFlag(LIVE_FLAG_UNK1000000);
		}
	}

	TEnemyManager::perform(flags, graphics);
}

void TNPCManager::makeCommonPartsModelDataKeeper_(u32 param_1,
                                                  const char* param_2,
                                                  TModelDataKeeper** param_3)
{
	unk5C = *param_3;
	if (unk5C == nullptr) {
		unk5C    = new TModelDataKeeper(param_2);
		*param_3 = unk5C;
		makePartsModelData_(param_1, 0x10210000, unk5C);
	}
}

void TNPCManager::makeOriginalPartsModelDataKeeper_(u32 param_1, u32 param_2)
{
	// TODO: fakematch?
	TModelDataKeeper* keeper = new TModelDataKeeper(unk1C->getFolder());

	unk60 = keeper;
	makePartsModelData_(param_1, param_2, unk60);
}

void TNPCManager::changeTextureToPollution_(J3DModelData* model)
{
	const ResTIMG* pollutionTex
	    = (const ResTIMG*)JKRFileLoader::getGlbResource(cRealPollutionTexName);

	if (pollutionTex != nullptr) {
		SMS_ChangeTextureAll(model, cDummyPollutionTexName, *pollutionTex);
	}
}

void TMonteMBaseManager::changeTextureToStraw_(J3DModelData*) { }

void TMonteWBaseManager::changeTextureToStraw_(J3DModelData*) { }

TMareBaseManager::TMareBaseManager(const char* name)
    : TNPCManager(name)
{
	if (mStaticBmtNormal == nullptr) {
		mStaticBmtNormal = J3DModelLoaderDataBase::loadMaterialTable(
		    JKRGetResource(cMareCommonNormalBmtName));
	}

	if (mStaticBmtPollution == nullptr) {
		mStaticBmtPollution = J3DModelLoaderDataBase::loadMaterialTable(
		    JKRGetResource(cMareCommonPollutionBmtName));
	}
}

TMonteMBaseManager::TMonteMBaseManager(const char* name)
    : TNPCManager(name)
{
	makeCommonPartsModelDataKeeper_(0, cMonteMCommonVolumeName,
	                                &mStaticCommonKeeper);
}

TMonteWBaseManager::TMonteWBaseManager(const char* name)
    : TNPCManager(name)
{
	makeCommonPartsModelDataKeeper_(0x9, cMonteWCommonVolumeName,
	                                &mStaticCommonKeeper);
}

TMareMBaseManager::TMareMBaseManager(const char* name)
    : TMareBaseManager(name)
{
	makeCommonPartsModelDataKeeper_(0xD, cMareMCommonVolumeName,
	                                &mStaticCommonKeeper);
}

TMareWBaseManager::TMareWBaseManager(const char* name)
    : TMareBaseManager(name)
{
	makeCommonPartsModelDataKeeper_(0x12, cMareWCommonVolumeName,
	                                &mStaticCommonKeeper);
}

J3DMaterialTable* TMareBaseManager::getBmt_(bool pollution)
{
	if (pollution)
		return mStaticBmtPollution;

	return mStaticBmtNormal;
}

void TMonteMBaseManager::createAnmData()
{
	MActorAnmData* anmData = new MActorAnmData;
	anmData->init(cMonteMCommonVolumeName, nullptr);
	unk20 = anmData;
}

void TMonteWBaseManager::createAnmData()
{
	MActorAnmData* anmData = new MActorAnmData;
	anmData->init(cMonteWCommonVolumeName, nullptr);
	unk20 = anmData;
}

void TMonteMSpecialManager::createAnmData() { TObjManager::createAnmData(); }

void TMonteWSpecialManager::createAnmData() { TObjManager::createAnmData(); }

void TMonteMFManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	unk3C = 250.0f;
	makeOriginalPartsModelDataKeeper_(0x6, 0x10210000);
}

void TMonteMGManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	unk3C = 250.0f;
	makeOriginalPartsModelDataKeeper_(0x7, 0x10210000);
}

void TMonteMHManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	unk3C = 250.0f;
	makeOriginalPartsModelDataKeeper_(0x8, 0x10210000);
}

void TMonteWCManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	unk3C = 250.0f;
	makeOriginalPartsModelDataKeeper_(0xC, 0x10210000);
}

void TMareMAManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	unk3C = 250.0f;
	makeOriginalPartsModelDataKeeper_(0xE, 0x10210000);
}

void TMareMBManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	unk3C = 250.0f;
	makeOriginalPartsModelDataKeeper_(0xF, 0x10210000);
}

void TMareMCManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	unk3C = 250.0f;
	makeOriginalPartsModelDataKeeper_(0x10, 0x10210000);
}

void TMareMDManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	unk3C = 250.0f;
	makeOriginalPartsModelDataKeeper_(0x11, 0x10210000);
}

void TMareWAManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	unk3C = 250.0f;
	makeOriginalPartsModelDataKeeper_(0x13, 0x10210000);
}

void TMareWBManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	unk3C = 250.0f;
	makeOriginalPartsModelDataKeeper_(0x14, 0x10210000);
}

void TKinopioManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	unk3C = 250.0f;
	makeOriginalPartsModelDataKeeper_(0x15, 0x10210000);
}

void TKinojiiManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	unk3C = 250.0f;
	makeOriginalPartsModelDataKeeper_(0x16, 0x10010000);
}

void TPeachManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	unk3C = 250.0f;
	makeOriginalPartsModelDataKeeper_(0x17, 0x10010000);
}

void TRaccoonDogManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	unk3C = 250.0f;
	makeOriginalPartsModelDataKeeper_(0x18, 0x10210000);
}

void TSunflowerLManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	unk3C = 250.0f;
	unk3C = 500.0f;
}
void TMonteMManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "mom_model.bmd", 0x10300000, 1 },
		{ nullptr, 0, 0 },
	};

	createModelDataArray(entry);

	J3DModelData* modelData
	    = getModelDataKeeper()->getNthData(0)->getModelData();
	ResTIMG* strawTex
	    = (ResTIMG*)JKRFileLoader::getGlbResource(cMonteMRealStrawTexName);
	SMS_ChangeTextureAll(modelData, cMonteMDummyStrawTexName, *strawTex);

	modelData = getModelDataKeeper()->getNthData(0)->getModelData();
	ResTIMG* pollutionTex
	    = (ResTIMG*)JKRFileLoader::getGlbResource(cRealPollutionTexName);
	if (pollutionTex != nullptr)
		SMS_ChangeTextureAll(modelData, cDummyPollutionTexName, *pollutionTex);
}

void TMonteMAManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "momA_model.bmd", 0x10300000, 1 },
		{ nullptr, 0, 0 },
	};

	createModelDataArray(entry);

	J3DModelData* modelData
	    = getModelDataKeeper()->getNthData(0)->getModelData();
	ResTIMG* strawTex
	    = (ResTIMG*)JKRFileLoader::getGlbResource(cMonteMRealStrawTexName);
	SMS_ChangeTextureAll(modelData, cMonteMDummyStrawTexName, *strawTex);

	modelData = getModelDataKeeper()->getNthData(0)->getModelData();
	ResTIMG* pollutionTex
	    = (ResTIMG*)JKRFileLoader::getGlbResource(cRealPollutionTexName);
	if (pollutionTex != nullptr)
		SMS_ChangeTextureAll(modelData, cDummyPollutionTexName, *pollutionTex);
}

void TMonteMBManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "momB_model.bmd", 0x10210000, 1 },
		{ nullptr, 0, 0 },
	};

	createModelDataArray(entry);

	J3DModelData* modelData
	    = getModelDataKeeper()->getNthData(0)->getModelData();
	ResTIMG* strawTex
	    = (ResTIMG*)JKRFileLoader::getGlbResource(cMonteMRealStrawTexName);
	SMS_ChangeTextureAll(modelData, cMonteMDummyStrawTexName, *strawTex);
}

void TMonteMCManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "momC_model.bmd", 0x10300000, 1 },
		{ nullptr, 0, 0 },
	};

	createModelDataArray(entry);

	J3DModelData* modelData
	    = getModelDataKeeper()->getNthData(0)->getModelData();
	ResTIMG* strawTex
	    = (ResTIMG*)JKRFileLoader::getGlbResource(cMonteMRealStrawTexName);
	SMS_ChangeTextureAll(modelData, cMonteMDummyStrawTexName, *strawTex);

	modelData = getModelDataKeeper()->getNthData(0)->getModelData();
	ResTIMG* pollutionTex
	    = (ResTIMG*)JKRFileLoader::getGlbResource(cRealPollutionTexName);
	if (pollutionTex != nullptr)
		SMS_ChangeTextureAll(modelData, cDummyPollutionTexName, *pollutionTex);
}

void TMonteMDManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "momD_model.bmd", 0x10210000, 1 },
		{ nullptr, 0, 0 },
	};

	createModelDataArray(entry);

	J3DModelData* modelData
	    = getModelDataKeeper()->getNthData(0)->getModelData();
	ResTIMG* strawTex
	    = (ResTIMG*)JKRFileLoader::getGlbResource(cMonteMRealStrawTexName);
	SMS_ChangeTextureAll(modelData, cMonteMDummyStrawTexName, *strawTex);
}

void TMonteMEManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "momE_model.bmd", 0x10010000, 0 },
		{ nullptr, 0, 0 },
	};

	createModelDataArray(entry);
}

void TMonteMFManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "mom_model.bmd", 0x10210000, 0 },
		{ nullptr, 0, 0 },
	};

	createModelDataArrayBase(entry, "/scene/monteM");

	J3DModelData* modelData
	    = getModelDataKeeper()->getNthData(0)->getModelData();
	ResTIMG* strawTex
	    = (ResTIMG*)JKRFileLoader::getGlbResource(cMonteMRealStrawTexName);
	SMS_ChangeTextureAll(modelData, cMonteMDummyStrawTexName, *strawTex);
}

void TMonteMGManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "momC_model.bmd", 0x10210000, 0 },
		{ nullptr, 0, 0 },
	};

	createModelDataArrayBase(entry, "/scene/monteMC");

	J3DModelData* modelData
	    = getModelDataKeeper()->getNthData(0)->getModelData();
	ResTIMG* strawTex
	    = (ResTIMG*)JKRFileLoader::getGlbResource(cMonteMRealStrawTexName);
	SMS_ChangeTextureAll(modelData, cMonteMDummyStrawTexName, *strawTex);
}

void TMonteMHManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "momA_model.bmd", 0x10210000, 0 },
		{ nullptr, 0, 0 },
	};

	createModelDataArrayBase(entry, "/scene/monteMA");

	J3DModelData* modelData
	    = getModelDataKeeper()->getNthData(0)->getModelData();
	ResTIMG* strawTex
	    = (ResTIMG*)JKRFileLoader::getGlbResource(cMonteMRealStrawTexName);
	SMS_ChangeTextureAll(modelData, cMonteMDummyStrawTexName, *strawTex);
}

void TMonteWManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "mow_model.bmd", 0x10300000, 1 },
		{ nullptr, 0, 0 },
	};

	createModelDataArray(entry);

	J3DModelData* modelData
	    = getModelDataKeeper()->getNthData(0)->getModelData();
	ResTIMG* strawTex
	    = (ResTIMG*)JKRFileLoader::getGlbResource(cMonteWRealStrawTexName);
	SMS_ChangeTextureAll(modelData, cMonteWDummyStrawTexName, *strawTex);

	modelData = getModelDataKeeper()->getNthData(0)->getModelData();
	ResTIMG* pollutionTex
	    = (ResTIMG*)JKRFileLoader::getGlbResource(cRealPollutionTexName);
	if (pollutionTex != nullptr)
		SMS_ChangeTextureAll(modelData, cDummyPollutionTexName, *pollutionTex);
}

void TMonteWAManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "mowA_model.bmd", 0x10300000, 1 },
		{ nullptr, 0, 0 },
	};

	createModelDataArray(entry);

	J3DModelData* modelData
	    = getModelDataKeeper()->getNthData(0)->getModelData();
	ResTIMG* strawTex
	    = (ResTIMG*)JKRFileLoader::getGlbResource(cMonteWRealStrawTexName);
	SMS_ChangeTextureAll(modelData, cMonteWDummyStrawTexName, *strawTex);

	modelData = getModelDataKeeper()->getNthData(0)->getModelData();
	ResTIMG* pollutionTex
	    = (ResTIMG*)JKRFileLoader::getGlbResource(cRealPollutionTexName);
	if (pollutionTex != nullptr)
		SMS_ChangeTextureAll(modelData, cDummyPollutionTexName, *pollutionTex);
}

void TMonteWBManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "mowB_model.bmd", 0x10210000, 1 },
		{ nullptr, 0, 0 },
	};

	createModelDataArray(entry);

	J3DModelData* modelData
	    = getModelDataKeeper()->getNthData(0)->getModelData();
	ResTIMG* strawTex
	    = (ResTIMG*)JKRFileLoader::getGlbResource(cMonteWRealStrawTexName);
	SMS_ChangeTextureAll(modelData, cMonteWDummyStrawTexName, *strawTex);
}

void TMonteWCManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "mow_model.bmd", 0x10210000, 0 },
		{ nullptr, 0, 0 },
	};

	createModelDataArrayBase(entry, "/scene/monteW");

	J3DModelData* modelData
	    = getModelDataKeeper()->getNthData(0)->getModelData();
	ResTIMG* strawTex
	    = (ResTIMG*)JKRFileLoader::getGlbResource(cMonteWRealStrawTexName);
	SMS_ChangeTextureAll(modelData, cMonteWDummyStrawTexName, *strawTex);
}

void TMareMBaseManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "mareM.bmd", 0x10300000, 0 },
		{ nullptr, 0, 0 },
	};

	createModelDataArrayBase(entry, "/scene/mareM");

	J3DModelData* modelData
	    = getModelDataKeeper()->getNthData(0)->getModelData();
	ResTIMG* pollutionTex
	    = (ResTIMG*)JKRFileLoader::getGlbResource(cRealPollutionTexName);
	if (pollutionTex != nullptr)
		SMS_ChangeTextureAll(modelData, cDummyPollutionTexName, *pollutionTex);
}

void TMareWBaseManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "mareW.bmd", 0x10300000, 0 },
		{ nullptr, 0, 0 },
	};

	createModelDataArrayBase(entry, "/scene/mareW");

	J3DModelData* modelData
	    = getModelDataKeeper()->getNthData(0)->getModelData();
	ResTIMG* pollutionTex
	    = (ResTIMG*)JKRFileLoader::getGlbResource(cRealPollutionTexName);
	if (pollutionTex != nullptr)
		SMS_ChangeTextureAll(modelData, cDummyPollutionTexName, *pollutionTex);
}

void TKinopioManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "kinopio_body.bmd", 0x10300000, 1 },
		{ nullptr, 0, 0 },
	};

	createModelDataArray(entry);

	J3DModelData* modelData
	    = getModelDataKeeper()->getNthData(0)->getModelData();
	ResTIMG* pollutionTex
	    = (ResTIMG*)JKRFileLoader::getGlbResource(cRealPollutionTexName);
	if (pollutionTex != nullptr)
		SMS_ChangeTextureAll(modelData, cDummyPollutionTexName, *pollutionTex);
}

void TKinojiiManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "kinoji_body.bmd", 0x10010000, 0 },
		{ nullptr, 0, 0 },
	};

	createModelDataArray(entry);
}

void TPeachManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "peach_model.bmd", 0x10010000, 0 },
		{ nullptr, 0, 0 },
	};

	createModelDataArray(entry);
}

void TRaccoonDogManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "tanuki.bmd", 0x10210000, 0 },
		{ nullptr, 0, 0 },
	};

	createModelDataArray(entry);
}

void TSunflowerLManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "sunflower.bmd", 0x10020000, 0 },
		{ nullptr, 0, 0 },
	};

	createModelDataArray(entry);
}

void TSunflowerSManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "sunflower_s.bmd", 0x10220000, 0 },
		{ nullptr, 0, 0 },
	};

	createModelDataArray(entry);
}

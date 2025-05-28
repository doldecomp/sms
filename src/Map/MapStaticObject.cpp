#include <Map/MapStaticObject.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapModel.hpp>
#include <Map/Map.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <Camera/Camera.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/Graph.hpp>
#include <Strategic/MirrorActor.hpp>
#include <System/MarDirector.hpp>
#include <MSound/MSoundScene.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <MarioUtil/ScreenUtil.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorUtil.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JDrama/JDRDrawBufObj.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <stdio.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

static void dummy(Vec& v)
{
	v = (Vec) { 0.0f, 0.0f, 0.0f };
	v = (Vec) { 1.0f, 1.0f, 1.0f };
}

static const TMapStaticObj::ActorDataTableEntry actor_data_table[] = {
	{ "SeaIndirect", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, "SeaIndirect",
	  0x11210000, nullptr, 0x0, 0xFFFFFFFF, 0x0, 0x0, 0x0, 0x41 },

	{ "ReflectParts", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, "ReflectParts",
	  0x10210000, nullptr, 0x0, 0xFFFFFFFF, 0x0, 0x0, 0x0, 0x10 },

	{ "ReflectSky", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, "ReflectSky",
	  0x10210000, nullptr, 0x0, 0xFFFFFFFF, 0x0, 0x0, 0x0, 0x8 },

	{ "sun_mirror", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, "sun_mirror",
	  0x10220000, nullptr, 0x0, 0xFFFFFFFF, 0x0, 0x0, 0x0, 0x62 },

	{ "sea", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, "マップグループ", "sea",
	  0x10220000, nullptr, 0x0, 0xFFFFFFFF, 0x0, 0x0, 0x0, 0x80 },

	{ "falls", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, nullptr, 0x10210000,
	  nullptr, 0x0, 0x3022, 0x0, 0x0, 0x0, 0x0 },

	{ "fountain", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, nullptr,
	  0x10210000, nullptr, 0x0, 0x3000, 0x0, 0x0, 0x0, 0x0 },

	{ "TopOfCorona", 0x40000024, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, nullptr,
	  0x10210000, nullptr, 0x0, 0xFFFFFFFF, "/scene/mapObj/ms_coronasmoke.jpa",
	  0x146, 0x1000000, 0x0 },

	{ "BiancoRiver", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, "BiancoRiver",
	  0x10210000, nullptr, 0x0, 0xFFFFFFFF, 0x0, 0x0, 0x0, 0x40 },

	{ "SoundObjRiver", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, nullptr,
	  0x10210000, nullptr, 0x0, 0x500F, 0x0, 0x0, 0x0, 0x0 },

	{ "SoundObjWaterIntoWater", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr,
	  nullptr, 0x10210000, nullptr, 0x0, 0x5010, 0x0, 0x0, 0x0, 0x0 },

	{ "BiancoAirWall", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, nullptr,
	  0x10210000, "BiaAirWall", 0x0, 0xFFFFFFFF, 0x0, 0x0, 0x0, 0x2 },

	{ "BiancoBossEffectLight", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr,
	  nullptr, 0x10210000, nullptr, 0x0, 0xFFFFFFFF,
	  "/scene/map/map/ms_wmlin_light.jpa", 0x151, 0x1000000, 0x0 },

	{ "BiaWaterPollution", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr,
	  "BiaWaterPollution", 0x11220000, nullptr, 0x0, 0xFFFFFFFF, 0x0, 0x0, 0x0,
	  0x40 },

	{ "riccoSea", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, nullptr,
	  0x10210000, "riccoSea", 0x0, 0xFFFFFFFF, 0x0, 0x0, 0x0, 0x0 },

	{ "riccoSeaPollutionS0", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr,
	  "riccoSeaPollutionS0", 0x11210000, "riccoSeaPollutionS0", 0x0, 0xFFFFFFFF,
	  0x0, 0x0, 0x0, 0x40 },

	{ "riccoSeaPollutionS1", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr,
	  "riccoSeaPollutionS1", 0x11210000, "riccoSeaPollutionS1", 0x0, 0xFFFFFFFF,
	  0x0, 0x0, 0x0, 0x40 },

	{ "riccoSeaPollutionS2", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr,
	  "riccoSeaPollutionS2", 0x11210000, "riccoSeaPollutionS2", 0x0, 0xFFFFFFFF,
	  0x0, 0x0, 0x0, 0x40 },

	{ "riccoSeaPollutionS3", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr,
	  "riccoSeaPollutionS3", 0x11210000, "riccoSeaPollutionS3", 0x0, 0xFFFFFFFF,
	  0x0, 0x0, 0x0, 0x40 },

	{ "riccoSeaPollutionS4", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr,
	  "riccoSeaPollutionS4", 0x11210000, "riccoSeaPollutionS4", 0x0, 0xFFFFFFFF,
	  0x0, 0x0, 0x0, 0x40 },

	{ "MareFalls", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, nullptr,
	  0x10210000, nullptr, 0x0, 0x3000, 0x0, 0x0, 0x0, 0x0 },

	{ "mareSeaPollutionS0", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr,
	  "mareSeaPollutionS0", 0x10210000, "mareSeaPollutionS0", 0x0, 0xFFFFFFFF,
	  0x0, 0x0, 0x0, 0x0 },

	{ "mareSeaPollutionS12", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr,
	  "mareSeaPollutionS12", 0x10210000, "mareSeaPollutionS12", 0x0, 0xFFFFFFFF,
	  0x0, 0x0, 0x0, 0x0 },

	{ "mareSeaPollutionS34567", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr,
	  nullptr, 0x10210000, "mareSeaPollutionS34567", 0x0, 0xFFFFFFFF, 0x0, 0x0,
	  0x0, 0x0 },

	{ "Mare5ExGate", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, "Mare5ExGate",
	  0x10210000, nullptr, 0x0, 0xFFFFFFFF, 0x0, 0x0, 0x0, 0x40 },

	{ "MonteRiver", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, "MonteRiver",
	  0x10210000, "MonteRiver", 0x0, 0xFFFFFFFF, 0x0, 0x0, 0x0, 0x40 },

	{ "IndirectObj", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, "IndirectObj",
	  0x11210000, nullptr, 0x0, 0xFFFFFFFF, 0x0, 0x0, 0x0, 0x41 },

	{ "TargetArrow", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, "TargetArrow",
	  0x10210000, nullptr, 0x0, 0xFFFFFFFF, 0x0, 0x0, 0x0, 0x4 },

	{ nullptr, 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, nullptr, 0x10210000,
	  nullptr, 0x0, 0xFFFFFFFF, 0x0, 0x0, 0x0, 0x0 },
};

f32 TMapStaticObj::mEffectCoronaScale;

J3DModelData* TMapStaticObj::getModelData() const
{
	return unk70->getUnk4()->getModelData();
}

void TMapStaticObj::getModel() const { }

void TMapStaticObj::calcUnique(JPABaseEmitter*) { }

void TMapStaticObj::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 2) {
		if (unk78 != 0xffffffff && gpMSound->gateCheck(unk78)) {
			MSoundSESystem::MSoundSE::startSoundActor(unk78, mPosition, 0,
			                                          nullptr, 0, 4);
		}
	}
}

void TMapStaticObj::initUnique()
{
	switch (getActorType()) {
	case 0x40000024:
		if (gpMarDirector->mMap == 4)
			mEffectCoronaScale = 1.8f;
		else
			mEffectCoronaScale = 1.8f;
		break;
	}

	if (strcmp(unk6C, "ReflectSky") == 0) {
		JDrama::TNameRefGen::getInstance()->getRootNameRef()->search("空");

		unk70->getUnk4()->getModelData()->setMaterialTable(
		    gpMapObjManager->getUnk68(), J3DMatCopyFlag_All);
		unk70->initDL();

		j3dSys.setDrawBuffer(
		    ((JDrama::TDrawBufObj*)JDrama::TNameRefGen::getInstance()
		         ->getRootNameRef()
		         ->search("DrawBuf MirrorSky Opa"))
		        ->getDrawBuffer(),
		    0);
		j3dSys.setDrawBuffer(
		    ((JDrama::TDrawBufObj*)JDrama::TNameRefGen::getInstance()
		         ->getRootNameRef()
		         ->search("DrawBuf MirrorSky Xlu"))
		        ->getDrawBuffer(),
		    1);

		unk70->getUnk4()->calc();
		unk70->getUnk4()->viewCalc();
		unk70->getUnk4()->entry();
	}
}

void TMapStaticObj::initMapCollision(const char* name)
{
	if ((unk68->unk40 & 2) == 0)
		unk74 = new TMapCollisionManager(1, "/map/map", nullptr);
	else
		unk74 = new TMapCollisionManager(1, "/mapObj", nullptr);
	unk74->init(name, 0, nullptr);
	Mtx mtx;
	MsMtxSetTRS(mtx, mPosition.x, mPosition.y, mPosition.z, mRotation.x,
	            mRotation.y, mRotation.z, mScaling.x, mScaling.y, mScaling.z);
	unk74->unk8->setMtx(mtx);
	unk74->unk8->setUp();
}

#pragma dont_inline on
void TMapStaticObj::initModel(const char* name)
{
	char buffer[256];

	u32 uVar4 = 3;
	if (unk68->unk40 & 0x40)
		uVar4 &= ~1;

	if (unk68->unk40 & 0x2) {
		snprintf(buffer, 256, "/common/map/%s.bmd", name);
		unk70 = SMS_MakeMActorWithAnmData(buffer, gpMapObjManager->getUnk40(),
		                                  uVar4, unk68->unk24);
	} else if (unk68->unk40 & 0x4) {
		snprintf(buffer, 256, "/scene/mapObj/%s.bmd", name);
		unk70 = SMS_MakeMActorWithAnmData(
		    buffer, gpMapObjManager->getMActorAnmData(), uVar4, unk68->unk24);
	} else {
		snprintf(buffer, 256, "/scene/map/map/%s.bmd", name);
		unk70 = SMS_MakeMActorWithAnmData(
		    buffer, gpMap->getModelManager()->getMActorAnmData(), uVar4,
		    unk68->unk24);
	}

	TMapObjBase::startAllAnim(unk70, name);
}
#pragma dont_inline off

// TODO: move this
extern u8 gParticleFlagLoaded[];
extern JPAResourceManager* gpResourceManager;

void TMapStaticObj::init(const char* name)
{
	unk6C = name;
	const ActorDataTableEntry* entry;
	for (int i = 0;; ++i) {
		entry = &actor_data_table[i];
		if (strcmp(name, actor_data_table[i].unk0))
			break;
	}
	unk68 = entry;

	initHitActor(unk68->unk4, 5, unk68->unk8, unk68->unkC, unk68->unk10,
	             unk68->unk14, unk68->unk18);

	if (unk68->unk20 != nullptr)
		initModel(unk68->unk20);

	if (unk68->unk28 != nullptr)
		initMapCollision(unk68->unk28);

	unk78 = unk68->unk30;

	if (unk68->unk34 != nullptr) {
		switch (unk68->unk3C) {
		case 1:
			if (gParticleFlagLoaded[unk68->unk38] == 0) {
				gpResourceManager->load(unk68->unk34, unk68->unk38);
				gParticleFlagLoaded[unk68->unk38] = 1;
			}
			break;
		case 3:
			if (gParticleFlagLoaded[unk68->unk38] == 0) {
				gpResourceManager->load(unk68->unk34, unk68->unk38);
				gParticleFlagLoaded[unk68->unk38] = 1;
			}
			break;
		}
	}

	if (unk68->unk1C != nullptr) {
		JDrama::TNameRef* ref
		    = JDrama::TNameRefGen::getInstance()->getRootNameRef()->search(
		        unk68->unk1C);
		// TODO: insert into some list?
	}

	if (unk68->unk40 & 1) {
		TScreenTexture* ref = JDrama::TNameRefGen::search<TScreenTexture>(
		    "スクリーンテクスチャ");
		const ResTIMG* img = ref->getTexture()->getTexInfo();
		unk70->getUnk4()->getModelData()->getTexture()->setResTIMG(1, *img);

		SMS_ChangeTextureAll(unk70->getUnk4()->getModelData(), "indirectdummy",
		                     *img);
	}
	initUnique();
	if (unk68->unk40 & 0x20)
		TMirrorActor::entryMirrorDrawBufferAlways(unk70->getUnk4());
}

void TMapStaticObj::loadAfter()
{
	THitActor::loadAfter();
	if (unk68 == nullptr)
		init(unk6C);
}

void TMapStaticObj::load(JSUMemoryInputStream& stream)
{
	THitActor::load(stream);
	unk6C = stream.readString();
}

TMapStaticObj::TMapStaticObj(const char* name)
    : THitActor(name)
    , unk68(nullptr)
    , unk6C(nullptr)
    , unk70(nullptr)
    , unk74(nullptr)
    , unk78(0xFFFFFFFF)
{
}

void TMapModelActor::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (!unk68)
		return;

	if (!(param_1 & 2)) {
		MsMtxSetXYZRPH(unk68->getUnk4()->unk20, mPosition.x, mPosition.y,
		               mPosition.z, mRotation.x * (65536.0f / 360.0f),
		               mRotation.y * (65536.0f / 360.0f),
		               mRotation.z * (65536.0f / 360.0f));
		unk68->getUnk4()->unk14 = mScaling;
	}
	unk68->perform(param_1, param_2);
}

void TMapObjSoundGroup::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (unk14->isDummy())
		return;

	if (param_1 & 1) {
		JGeometry::TVec3<f32> local_c18[0x100];
		JGeometry::TVec3<f32> local_c24;
		unk14->unk0->getPoint(&local_c24);
		for (int i = 1; i < unk14->unk8; ++i) {
			Vec local_c30;
			unk14->unk0[i].getPoint(&local_c30);

			// TODO: camera?
			JGeometry::TVec3<f32> local_c54; // gpCamera->unk124;
			local_c18[i]
			    = MsPerpendicFootToLineR(local_c24, local_c30, local_c54);

			if (unk14->unk0[i].unkC == 1 && i < unk14->unk8 - 1) {
				unk14->unk0[i].getPoint(local_c24);
			}
		}
		unk10->frameLoop(unk18, local_c18, unk14->unk8);
	}
}

// fabricated
struct SoundInfoEntry {
	const char* unk0;
	u32 unk4;
};
static const SoundInfoEntry sound_info[] = {
	{ "ms_sea", 0x5000 },
	{ "ms_harbor", 0x5003 },
	{ nullptr, 0 },
};

static u32 getSoundID(const char* name)
{
	for (int i = 0; sound_info[i].unk0 != nullptr; ++i)
		if (strcmp(sound_info[i].unk0, name) == 0)
			return sound_info[i].unk4;

	return 0;
}

void TMapObjSoundGroup::load(JSUMemoryInputStream& stream)
{
	TViewObj::load(stream);
	const char* graphName = stream.readString();
	unk14                 = gpConductor->getGraphByName(graphName);
	if (!unk14 || unk14->isDummy())
		return;

	unk18 = getSoundID(graphName);
	unk10 = new MSSceneSE(0xffffffff);
}

TMapObjSoundGroup::TMapObjSoundGroup(const char* name)
    : JDrama::TViewObj(name)
    , unk10(0)
    , unk14(nullptr)
    , unk18(0)
{
}

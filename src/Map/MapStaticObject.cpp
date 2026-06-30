#include <Map/MapStaticObject.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JDrama/JDRDrawBufObj.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <System/EmitterViewObj.hpp>
#include <Strategic/MirrorActor.hpp>
#include <Strategic/Strategy.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <MarioUtil/ScreenUtil.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorUtil.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapModel.hpp>
#include <Map/MapMirror.hpp>
#include <Map/Map.hpp>
#include <Map/Sky.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <Camera/Camera.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/Graph.hpp>
#include <MSound/MSoundScene.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <stdio.h>

// Common J3DModelLoaderFlag combinations used by the actor data table below.
// Spelled out as named constants to keep the table rows compact.
enum {
	kMdlF_PE1 = J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
	            | (1 << J3DMLF_TevStageNumShift), // kMdlF_PE1
	kMdlF_PE2 = J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
	            | (2 << J3DMLF_TevStageNumShift), // kMdlF_PE2
	kMdlF_IndPE1 = J3DMLF_MaterialPEFull | J3DMLF_MaterialUseIndirect
	               | J3DMLF_UseUniqueMaterials
	               | (1 << J3DMLF_TevStageNumShift), // kMdlF_IndPE1
	kMdlF_IndPE2 = J3DMLF_MaterialPEFull | J3DMLF_MaterialUseIndirect
	               | J3DMLF_UseUniqueMaterials
	               | (2 << J3DMLF_TevStageNumShift), // kMdlF_IndPE2
};

static const TMapStaticObj::TActorData actor_data_table[] = {
	{ "SeaIndirect", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, "SeaIndirect",
	  kMdlF_IndPE1, nullptr, 0x0, 0xFFFFFFFF, 0x0, 0x0, 0x0,
	  TMapStaticObj::TActorData::FLAG_UNK40
	      | TMapStaticObj::TActorData::FLAG_IS_INDIRECT },

	{ "ReflectParts", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, "ReflectParts",
	  kMdlF_PE1, nullptr, 0x0, 0xFFFFFFFF, 0x0, 0x0, 0x0,
	  TMapStaticObj::TActorData::FLAG_UNK10 },

	{ "ReflectSky", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, "ReflectSky",
	  kMdlF_PE1, nullptr, 0x0, 0xFFFFFFFF, 0x0, 0x0, 0x0,
	  TMapStaticObj::TActorData::FLAG_UNK8 },

	{ "sun_mirror", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, "sun_mirror",
	  kMdlF_PE2, nullptr, 0x0, 0xFFFFFFFF, 0x0, 0x0, 0x0,
	  TMapStaticObj::TActorData::FLAG_UNK40
	      | TMapStaticObj::TActorData::FLAG_UNK20
	      | TMapStaticObj::TActorData::FLAG_UNK2 },

	{ "sea", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, "マップグループ", "sea",
	  kMdlF_PE2, nullptr, 0x0, 0xFFFFFFFF, 0x0, 0x0, 0x0,
	  TMapStaticObj::TActorData::FLAG_UNK80 },

	{ "falls", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, nullptr, kMdlF_PE1,
	  nullptr, 0x0, MSD_SE_OBJ_FALL_SMALL, 0x0, 0x0, 0x0, 0x0 },

	{ "fountain", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, nullptr, kMdlF_PE1,
	  nullptr, 0x0, MSD_SE_OBJ_FOUNTAIN, 0x0, 0x0, 0x0, 0x0 },

	{ "TopOfCorona", 0x40000024, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, nullptr,
	  kMdlF_PE1, nullptr, 0x0, 0xFFFFFFFF, "/scene/mapObj/ms_coronasmoke.jpa",
	  0x146, 0x1, 0x0 },

	{ "BiancoRiver", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, "BiancoRiver",
	  kMdlF_PE1, nullptr, 0x0, 0xFFFFFFFF, 0x0, 0x0, 0x0,
	  TMapStaticObj::TActorData::FLAG_UNK40 },

	{ "SoundObjRiver", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, nullptr,
	  kMdlF_PE1, nullptr, 0x0, MSD_SE_EV_STREAM, 0x0, 0x0, 0x0, 0x0 },

	{ "SoundObjWaterIntoWater", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr,
	  nullptr, kMdlF_PE1, nullptr, 0x0, MSD_SE_EV_STREAM_DOWN, 0x0, 0x0, 0x0,
	  0x0 },

	{ "BiancoAirWall", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, nullptr,
	  kMdlF_PE1, "BiaAirWall", 0x0, 0xFFFFFFFF, 0x0, 0x0, 0x0,
	  TMapStaticObj::TActorData::FLAG_UNK2 },

	{ "BiancoBossEffectLight", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr,
	  nullptr, kMdlF_PE1, nullptr, 0x0, 0xFFFFFFFF,
	  "/scene/map/map/ms_wmlin_light.jpa", 0x151, 0x1, 0x0 },

	{ "BiaWaterPollution", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr,
	  "BiaWaterPollution", kMdlF_IndPE2, nullptr, 0x0, 0xFFFFFFFF, 0x0, 0x0,
	  0x0, TMapStaticObj::TActorData::FLAG_UNK40 },

	{ "riccoSea", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, nullptr, kMdlF_PE1,
	  "riccoSea", 0x0, 0xFFFFFFFF, 0x0, 0x0, 0x0, 0x0 },

	{ "riccoSeaPollutionS0", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr,
	  "riccoSeaPollutionS0", kMdlF_IndPE1, "riccoSeaPollutionS0", 0x0,
	  0xFFFFFFFF, 0x0, 0x0, 0x0, TMapStaticObj::TActorData::FLAG_UNK40 },

	{ "riccoSeaPollutionS1", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr,
	  "riccoSeaPollutionS1", kMdlF_IndPE1, "riccoSeaPollutionS1", 0x0,
	  0xFFFFFFFF, 0x0, 0x0, 0x0, TMapStaticObj::TActorData::FLAG_UNK40 },

	{ "riccoSeaPollutionS2", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr,
	  "riccoSeaPollutionS2", kMdlF_IndPE1, "riccoSeaPollutionS2", 0x0,
	  0xFFFFFFFF, 0x0, 0x0, 0x0, TMapStaticObj::TActorData::FLAG_UNK40 },

	{ "riccoSeaPollutionS3", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr,
	  "riccoSeaPollutionS3", kMdlF_IndPE1, "riccoSeaPollutionS3", 0x0,
	  0xFFFFFFFF, 0x0, 0x0, 0x0, TMapStaticObj::TActorData::FLAG_UNK40 },

	{ "riccoSeaPollutionS4", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr,
	  "riccoSeaPollutionS4", kMdlF_IndPE1, "riccoSeaPollutionS4", 0x0,
	  0xFFFFFFFF, 0x0, 0x0, 0x0, TMapStaticObj::TActorData::FLAG_UNK40 },

	{ "MareFalls", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, nullptr,
	  kMdlF_PE1, nullptr, 0x0, MSD_SE_OBJ_FOUNTAIN, 0x0, 0x0, 0x0, 0x0 },

	{ "mareSeaPollutionS0", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr,
	  "mareSeaPollutionS0", kMdlF_PE1, "mareSeaPollutionS0", 0x0, 0xFFFFFFFF,
	  0x0, 0x0, 0x0, 0x0 },

	{ "mareSeaPollutionS12", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr,
	  "mareSeaPollutionS12", kMdlF_PE1, "mareSeaPollutionS12", 0x0, 0xFFFFFFFF,
	  0x0, 0x0, 0x0, 0x0 },

	{ "mareSeaPollutionS34567", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr,
	  nullptr, kMdlF_PE1, "mareSeaPollutionS34567", 0x0, 0xFFFFFFFF, 0x0, 0x0,
	  0x0, 0x0 },

	{ "Mare5ExGate", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, "Mare5ExGate",
	  kMdlF_PE1, nullptr, 0x0, 0xFFFFFFFF, 0x0, 0x0, 0x0,
	  TMapStaticObj::TActorData::FLAG_UNK40 },

	{ "MonteRiver", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, "MonteRiver",
	  kMdlF_PE1, "MonteRiver", 0x0, 0xFFFFFFFF, 0x0, 0x0, 0x0,
	  TMapStaticObj::TActorData::FLAG_UNK40 },

	{ "IndirectObj", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, "IndirectObj",
	  kMdlF_IndPE1, nullptr, 0x0, 0xFFFFFFFF, 0x0, 0x0, 0x0,
	  TMapStaticObj::TActorData::FLAG_UNK40
	      | TMapStaticObj::TActorData::FLAG_IS_INDIRECT },

	{ "TargetArrow", 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, "TargetArrow",
	  kMdlF_PE1, nullptr, 0x0, 0xFFFFFFFF, 0x0, 0x0, 0x0,
	  TMapStaticObj::TActorData::FLAG_UNK4 },

	{ nullptr, 0x0, 0x0, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, nullptr, kMdlF_PE1,
	  nullptr, 0x0, 0xFFFFFFFF, 0x0, 0x0, 0x0, 0x0 },
};

f32 TMapStaticObj::mEffectCoronaScale;

J3DModelData* TMapStaticObj::getModelData() const
{
	return mMActor->getModel()->getModelData();
}

J3DModel* TMapStaticObj::getModel() const { return mMActor->getModel(); }

void TMapStaticObj::calcUnique(JPABaseEmitter* emitter)
{
	switch (mActorType) {
	case 0x40000024:
		if (emitter) {
			JGeometry::TVec3<f32> scale(mEffectCoronaScale, mEffectCoronaScale,
			                            mEffectCoronaScale);
			emitter->setScale(scale);
		}
		break;
	default:
		break;
	}
}

void TMapStaticObj::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 2) {
		if (mSoundId != -1)
			SMSGetMSound()->startSoundActor(mSoundId, &mPosition, 0, nullptr, 0,
			                                4);

		JPABaseEmitter* emitter = nullptr;
		if (mActorData->mParticleType == 1)
			emitter = gpMarioParticleManager->emit(
			    mActorData->mParticleId, &mPosition, mActorData->mParticleType,
			    this);
		else if (mActorData->mParticleType == 3)
			emitter = gpMarioParticleManager->emit(
			    mActorData->mParticleId, &mPosition, mActorData->mParticleType,
			    this);

		calcUnique(emitter);
	}

	if ((param_1 & 4) && (mActorData->mFlags & TActorData::FLAG_IS_INDIRECT)) {
		Mtx afStack_7c;
		SMS_GetLightPerspectiveForEffectMtx(afStack_7c);

		getModelData()
		    ->getMaterialNodePointer(0)
		    ->getTexGenBlock()
		    ->getTexMtx(1)
		    ->setEffectMtx(afStack_7c);
	}

	if ((param_1 & 0x200)
	    && ((mActorData->mFlags & TActorData::FLAG_UNK8)
	        || (mActorData->mFlags & TActorData::FLAG_UNK20))) {
		param_1 &= ~0x200;
		mMActor->updateMatAnm();
	}

	if ((!(param_1 & 0x200) || !(mActorData->mFlags & TActorData::FLAG_UNK10)
	     || gpMirrorModelManager->isUnk18Present())
	    && mMActor) {
		if (param_1 & 0x2) {
			MsMtxSetXYZRPH(getModel()->getBaseTRMtx(), mPosition.x, mPosition.y,
			               mPosition.z, mRotation.x, mRotation.y, mRotation.z);
			getModel()->setBaseScale(mScaling);
		}

		if ((param_1 & 0x200)
		    && (mActorData->mFlags & TActorData::FLAG_UNK80)) {
			J3DDrawBuffer* oldOpaBuf = j3dSys.getDrawBuffer(0);
			J3DDrawBuffer* oldXluBuf = j3dSys.getDrawBuffer(1);
			j3dSys.setDrawBuffer(
			    gpMapObjManager->getDrawBufferAfterIndirectOpa(), 0);
			j3dSys.setDrawBuffer(
			    gpMapObjManager->getDrawBufferAfterIndirectXlu(), 1);
			mMActor->perform(param_1, param_2);
			j3dSys.setDrawBuffer(oldOpaBuf, 0);
			j3dSys.setDrawBuffer(oldXluBuf, 1);
		} else {
			mMActor->perform(param_1, param_2);
		}
	}
}

void TMapStaticObj::initUnique()
{
	switch (getActorType()) {
	case 0x40000024:
		if (gpMarDirector->getCurrentMap() == 4)
			mEffectCoronaScale = 1.8f;
		else
			mEffectCoronaScale = 1.8f;
		break;
	}

	if (strcmp(mActorName, "ReflectSky") == 0) {
		TSky* sky = JDrama::TNameRefGen::getInstance()->search<TSky>("空");

		getModelData()->setMaterialTable(gpMapObjManager->getUnk68(),
		                                 J3DMatCopyFlag_All);
		mMActor->initDL();

		JDrama::TDrawBufObj* dboOpa
		    = JDrama::TNameRefGen::getInstance()->search<JDrama::TDrawBufObj>(
		        "DrawBuf MirrorSky Opa");
		j3dSys.setDrawBuffer(dboOpa->getDrawBuffer(), 0);
		JDrama::TDrawBufObj* dboXlu
		    = JDrama::TNameRefGen::getInstance()->search<JDrama::TDrawBufObj>(
		        "DrawBuf MirrorSky Xlu");
		j3dSys.setDrawBuffer(dboXlu->getDrawBuffer(), 1);

		getModel()->calc();
		getModel()->viewCalc();
		getModel()->entry();
	}
}

void TMapStaticObj::initMapCollision(const char* name)
{
	if ((mActorData->mFlags & TActorData::FLAG_UNK2) != 0)
		mCollisionManager = new TMapCollisionManager(1, "/mapObj", nullptr);
	else
		mCollisionManager = new TMapCollisionManager(1, "/map/map", nullptr);
	mCollisionManager->init(name, 0, nullptr);
	mCollisionManager->setUpUnk8TRS(mPosition, mRotation, mScaling);
}

#pragma dont_inline on
void TMapStaticObj::initModel(const char* name)
{
	char buffer[256];

	u32 uVar4 = 3;
	if (mActorData->mFlags & TActorData::FLAG_UNK40)
		uVar4 &= ~1;

	if (mActorData->mFlags & TActorData::FLAG_UNK2) {
		snprintf(buffer, 256, "/common/map/%s.bmd", name);
		mMActor = SMS_MakeMActorWithAnmData(buffer, gpMapObjManager->getUnk40(),
		                                    uVar4, mActorData->unk24);
	} else if (mActorData->mFlags & TActorData::FLAG_UNK4) {
		snprintf(buffer, 256, "/scene/mapObj/%s.bmd", name);
		mMActor = SMS_MakeMActorWithAnmData(buffer,
		                                    gpMapObjManager->getMActorAnmData(),
		                                    uVar4, mActorData->unk24);
	} else {
		snprintf(buffer, 256, "/scene/map/map/%s.bmd", name);
		mMActor = SMS_MakeMActorWithAnmData(
		    buffer, gpMap->getModelManager()->getMActorAnmData(), uVar4,
		    mActorData->unk24);
	}

	TMapObjBase::startAllAnim(mMActor, name);
}
#pragma dont_inline off

void TMapStaticObj::init(const char* name)
{
	mActorName = name;

	int i = 0;
	while (strcmp(name, actor_data_table[i].mActorName) != 0)
		++i;

	mActorData = &actor_data_table[i];

	initHitActor(mActorData->mActorType, 5, mActorData->mHitFlags,
	             mActorData->mAttackRadius, mActorData->mAttackHeight,
	             mActorData->mDamageRadius, mActorData->mDamageHeight);

	if (mActorData->mModelFileName != nullptr)
		initModel(mActorData->mModelFileName);

	if (mActorData->mColFileName != nullptr)
		initMapCollision(mActorData->mColFileName);

	mSoundId = mActorData->mSoundId;

	if (mActorData->mParticlePath != nullptr) {
		switch (mActorData->mParticleType) {
		case 0:
			break;
		case 1:
			SMS_LoadParticle(mActorData->mParticlePath,
			                 mActorData->mParticleId);
			break;
		case 2:
			break;
		case 3:
			SMS_LoadParticle(mActorData->mParticlePath,
			                 mActorData->mParticleId);
			break;
		}
	}

	if (mActorData->mIdxGroupName != nullptr) {
		TIdxGroupObj* group
		    = JDrama::TNameRefGen::getInstance()->search<TIdxGroupObj>(
		        mActorData->mIdxGroupName);
		group->getChildren().push_back(this);
	}

	if (mActorData->mFlags & TActorData::FLAG_IS_INDIRECT) {
		TScreenTexture* ref = JDrama::TNameRefGen::search<TScreenTexture>(
		    "スクリーンテクスチャ");
		const ResTIMG* img = ref->getTexture()->getTexInfo();
		mMActor->getModel()->getModelData()->getTexture()->setResTIMG(1, *img);

		SMS_ChangeTextureAll(mMActor->getModel()->getModelData(),
		                     "indirectdummy", *img);
	}
	initUnique();
	if (mActorData->mFlags & TActorData::FLAG_UNK20)
		TMirrorActor::entryMirrorDrawBufferAlways(mMActor->getModel());
}

void TMapStaticObj::loadAfter()
{
	THitActor::loadAfter();
	if (mActorData == nullptr)
		init(mActorName);
}

void TMapStaticObj::load(JSUMemoryInputStream& stream)
{
	THitActor::load(stream);
	mActorName = stream.readString();
}

TMapStaticObj::TMapStaticObj(const char* name)
    : THitActor(name)
    , mActorData(nullptr)
    , mActorName(nullptr)
    , mMActor(nullptr)
    , mCollisionManager(nullptr)
    , mSoundId(-1)
{
}

void TMapModelActor::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (!unk68)
		return;

	if (param_1 & 2) {
		MsMtxSetXYZRPH(unk68->getModel()->getBaseTRMtx(), mPosition.x,
		               mPosition.y, mPosition.z, mRotation.x, mRotation.y,
		               mRotation.z);
		unk68->getModel()->setBaseScale(mScaling);
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

		JGeometry::TVec3<f32> tmp;
		JGeometry::TVec3<f32>& camPos = tmp;

		int count = 0;
		for (int i = 1; i < unk14->getNodeNum(); ++i) {
			JGeometry::TVec3<f32> local_c30;
			unk14->getGraphNode(i).getPoint(&local_c30);

			camPos.set(gpCamera->unk124);

			JGeometry::TVec3<f32> tmp
			    = MsPerpendicFootToLineR(local_c24, local_c30, camPos);
			local_c18[count].set(tmp);

			local_c24 = local_c30;

			if (unk14->getGraphNode(i).getRailNode()->mConnectionNum == 1
			    && i < unk14->getNodeNum() - 1) {
				++i;
				unk14->getGraphNode(i).getPoint(&local_c24);
			}

			++count;
		}
		unk10->frameLoop(unk18, local_c18, count);
	}
}

// fabricated
struct SoundInfoEntry {
	const char* unk0;
	u32 unk4;
};
static const SoundInfoEntry sound_info[] = {
	{ "ms_sea", MSD_SE_EV_GLOBAL_SEA_L },
	{ "ms_harbor", MSD_SE_EV_LITTLE_WAVE_L },
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

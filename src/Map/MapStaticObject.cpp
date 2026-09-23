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
			emitter->setGlobalScale(scale);
		}
		break;
	default:
		break;
	}
}

void TMapStaticObj::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_CALC_ANIM) {
		// An object with a rand-play index at 0x7c plays through
		// MSRandPlay instead of the positional SE. The gateCheck is spelled
		// out rather than routed through MSound::startSeRandPlay because the
		// wrapper's inline temp costs 8 bytes of frame the ROM does not have.
		//
		// TODO: 99.3%. The ROM loads mSoundId straight into r27 and
		// mRandPlayHandle straight into r31 and keeps both across the
		// gateCheck call, where we reload the members at the call. Naming
		// either or both as locals (function scope, block scope, initialised
		// or uninitialised) always lands them in r25/r26 *below* `this` with
		// an extra `addi` copy after the compare, so the carrier is not a
		// named local of this function.
		if (mSoundId != -1) {
			if (mRandPlayHandle == -1)
				SMSGetMSound()->startSoundActor(mSoundId, &mPosition, 0,
				                                nullptr, 0, 4);
			else if (SMSGetMSound()->gateCheck(mSoundId))
				MSoundSESystem::MSRandPlay::startSeRandPlay(mSoundId,
				                                            mRandPlayHandle);
		}

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

	if ((cue & CUE_CALC_VIEW)
	    && (mActorData->mFlags & TActorData::FLAG_IS_INDIRECT)) {
		Mtx44 afStack_7c;
		SMS_GetLightPerspectiveForEffectMtx(afStack_7c);

		getModelData()
		    ->getMaterialNodePointer(0)
		    ->getTexGenBlock()
		    ->getTexMtx(1)
		    ->setEffectMtx(afStack_7c);
	}

	if ((cue & CUE_ENTRY)
	    && ((mActorData->mFlags & TActorData::FLAG_UNK8)
	        || (mActorData->mFlags & TActorData::FLAG_UNK20))) {
		cue &= ~CUE_ENTRY;
		mMActor->updateMatAnm();
	}

	if ((!(cue & CUE_ENTRY) || !(mActorData->mFlags & TActorData::FLAG_UNK10)
	     || gpMirrorModelManager->isUnk18Present())
	    && mMActor) {
		if (cue & CUE_CALC_ANIM) {
			MsMtxSetXYZRPH(getModel()->getBaseTRMtx(), mPosition.x, mPosition.y,
			               mPosition.z, mRotation.x, mRotation.y, mRotation.z);
			getModel()->setBaseScale(mScaling);
		}

		if ((cue & CUE_ENTRY)
		    && (mActorData->mFlags & TActorData::FLAG_UNK80)) {
			J3DDrawBuffer* oldOpaBuf = j3dSys.getDrawBuffer(0);
			J3DDrawBuffer* oldXluBuf = j3dSys.getDrawBuffer(1);
			j3dSys.setDrawBuffer(
			    gpMapObjManager->getDrawBufferAfterIndirectOpa(), 0);
			j3dSys.setDrawBuffer(
			    gpMapObjManager->getDrawBufferAfterIndirectXlu(), 1);
			mMActor->perform(cue, graphics);
			j3dSys.setDrawBuffer(oldOpaBuf, 0);
			j3dSys.setDrawBuffer(oldXluBuf, 1);
		} else {
			mMActor->perform(cue, graphics);
		}
	}
}

void TMapStaticObj::initUnique()
{
	switch (getActorType()) {
	case 0x40000024: {
		u8 map = SMSGetMarDirector()->getCurrentMap();
		if (map == 4)
			mEffectCoronaScale = 1.8f;
		else
			mEffectCoronaScale = 1.8f;
		break;
	}
	}

	if (strcmp(mActorName, "ReflectSky") == 0) {
		JDrama::TNameRefGen* gen = JDrama::TNameRefGen::getInstance();
		TSky* sky                = gen->search<TSky>("空");

		getModelData()->setMaterialTable(gpMapObjManager->getUnk68(),
		                                 J3DMatCopyFlag_All);
		mMActor->initDL();

		j3dSys.setDrawBuffer(
		    JDrama::TNameRefGen::getInstance()
		        ->search<JDrama::TDrawBufObj>("DrawBuf MirrorSky Opa")
		        ->getDrawBuffer(),
		    0);
		j3dSys.setDrawBuffer(
		    JDrama::TNameRefGen::getInstance()
		        ->search<JDrama::TDrawBufObj>("DrawBuf MirrorSky Xlu")
		        ->getDrawBuffer(),
		    1);

		getModel()->calc();
		getModel()->viewCalc();
		getModel()->entry();
	} else if (strcmp(mActorName, "SoundObjRiver") == 0) {
		if (SMSGetMarDirector()->getCurrentMap() == 13)
			mSoundId = MSD_SE_OBJ_FOUNTAIN;
	} else if (strcmp(mActorName, "MareFalls") == 0) {
		switch (SMSGetMarDirector()->getCurrentMap()) {
		case 8:
			switch (SMSGetMarDirector()->getCurrentStage()) {
			case 0:
			case 2:
			case 4:
			case 6:
				mSoundId        = MSD_SE_OBJ_MONTE_NIGHT_A1;
				mRandPlayHandle = 0;
				break;
			default:
				mSoundId        = MSD_SE_OBJ_MONTE_DAY_A1;
				mRandPlayHandle = 0;
				break;
			}
			break;
		case 2:
			mSoundId        = MSD_SE_OBJ_BIRD_BIA_1;
			mRandPlayHandle = 0;
			break;
		}
	}

	if (mRandPlayHandle == 0)
		mRandPlayHandle = MSoundSESystem::MSRandPlay::registerTrans(mSoundId,
		                                                            &mPosition);
}

// Retail calls TMapCollisionBase::setMtx out of line from init, one inline
// level deeper than setUpUnk8TRS reaches from initMapCollision alone.
static inline void setUpCollision(TMapCollisionManager* m, TMapStaticObj* o)
{
	m->setUpUnk8TRS(o->mPosition, o->mRotation, o->mScaling);
}

void TMapStaticObj::initMapCollision(const char* name)
{
	if ((mActorData->mFlags & TActorData::FLAG_UNK2) != 0)
		mCollisionManager = new TMapCollisionManager(1, "/mapObj", nullptr);
	else
		mCollisionManager = new TMapCollisionManager(1, "/map/map", nullptr);
	mCollisionManager->init(name, 0, nullptr);
	setUpCollision(mCollisionManager, this);
}

// Retail calls this from init: the named `actor` is the fifteenth statement
// over the depth-1 budget. Measured and rejected: naming either anim-data
// argument (the buffer moves 4 up, or the loads reorder).
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

	MActor* actor = mMActor;
	TMapObjBase::startAllAnim(actor, name);
}

// TODO: 99.9%. The setUpUnk8TRS scratch Mtx sits at 0x94 and the insert
// iterator pool at 0xd8 (retail 0xb0 and 0xa4), and the SMS_LoadParticle
// flag pointer takes r31 (retail r29). Inert: naming the searched group or
// the collision manager, forwarding `this` instead of the manager.
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
		JDrama::TNameRefGen::getInstance()
		    ->search<TIdxGroupObj>(mActorData->mIdxGroupName)
		    ->getChildren()
		    .push_back(this);
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
    , mRandPlayHandle(-1)
{
}

void TMapModelActor::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (!unk68)
		return;

	if (cue & CUE_CALC_ANIM) {
		MsMtxSetXYZRPH(unk68->getModel()->getBaseTRMtx(), mPosition.x,
		               mPosition.y, mPosition.z, mRotation.x, mRotation.y,
		               mRotation.z);
		unk68->getModel()->setBaseScale(mScaling);
	}
	unk68->perform(cue, graphics);
}

// TODO: frame 0xca0 against retail's 0xcb8. Retail's slots, top down:
// local_c18 0xa0, local_c24 0x94, local_c30 0x88, tmp 0x7c, the MsPerpendic
// return 0x70, camPos 0x64 (address hoisted into r31), plus 0x18 more low
// region; ours puts camPos above local_c30. Loop-scope and ctor spellings of
// camPos were worse.
void TMapObjSoundGroup::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (mGraph->isDummy())
		return;

	if (cue & CUE_MOVE) {
		JGeometry::TVec3<f32> local_c18[0x100];
		JGeometry::TVec3<f32> local_c24;
		mGraph->unk0->getPoint(&local_c24);

		JGeometry::TVec3<f32> tmp;
		JGeometry::TVec3<f32>& camPos = tmp;

		int count = 0;
		for (int i = 1; i < mGraph->getNodeNum(); ++i) {
			JGeometry::TVec3<f32> local_c30;
			mGraph->getGraphNode(i).getPoint(&local_c30);

			camPos.set(gpCamera->unk124);

			JGeometry::TVec3<f32> tmp
			    = MsPerpendicFootToLineR(local_c24, local_c30, camPos);
			local_c18[count].set(tmp);

			local_c24 = local_c30;

			if (mGraph->getGraphNode(i).getRailNode()->mConnectionNum == 1
			    && i < mGraph->getNodeNum() - 1) {
				++i;
				mGraph->getGraphNode(i).getPoint(&local_c24);
			}

			++count;
		}
		mSceneSE->frameLoop(mSoundID, local_c18, count);
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
	mGraph                = gpConductor->getGraphByName(graphName);
	if (!mGraph || mGraph->isDummy())
		return;

	mSoundID = getSoundID(graphName);
	mSceneSE = new MSSceneSE(0xffffffff);
}

TMapObjSoundGroup::TMapObjSoundGroup(const char* name)
    : JDrama::TViewObj(name)
    , mSceneSE(0)
    , mGraph(nullptr)
    , mSoundID(0)
{
}

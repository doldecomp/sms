#include <Enemy/EnemyMario.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/Emario.hpp>
#include <Enemy/Graph.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAnm.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DDrawBuffer.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorUtil.hpp>
#include <M3DUtil/M3UJoint.hpp>
#include <M3DUtil/M3UModelMario.hpp>
#include <MSound/MAnmSound.hpp>
#include <Map/MapData.hpp>
#include <Map/PollutionManager.hpp>
#include <MoveBG/MapObjWave.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/ShadowUtil.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <Player/MarioRecord.hpp>
#include <Player/MarioEffect.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Strategic/question.hpp>
#include <System/Application.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <System/MSoundMainSide.hpp>
#include <System/Particles.hpp>
#include <dolphin/gx.h>
#include <math.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

static const char cDirtyFileName[] = "/scene/map/pollution/H_ma_rak.bti";
static const char cDirtyTexName[]  = "H_ma_rak_dummy";

#include <Player/MarioAnimeData.hpp>

static const TEnemyMario::TReplayLink replayLinkMonteMan[6] = {
	{ 1, 0 },       { 1, 1 },       { 1, 2 },
	{ 0xff, 0xff }, { 0xff, 0xff }, { 0xff, 0xff },
};

static const char* recordFileNamesDolpic1[8] = {
	nullptr, "BH", "CH", "DH", "EH", "FH", "GH", nullptr,
};

static const char* recordFileNamesMonteMan[3] = { "AB0", "AB1", "AB2" };

TEnemyMario::TSettingParams::TSettingParams(const char* path)
    : TParams(path)
    , PARAM_INIT(mSearchDist, 1000.0f)
    , PARAM_INIT(mSearchHeight, 300.0f)
    , PARAM_INIT(mWaterCtMax, 64)
    , PARAM_INIT(mStopFlag, 1)
    , PARAM_INIT(mStampFlag, 1)
    , PARAM_INIT(mRandomFlag, 1)
    , PARAM_INIT(mCarryFlag, 0)
    , PARAM_INIT(mInvincibleFlag, 0)
    , PARAM_INIT(mRandomPow, 1.0f)
    , PARAM_INIT(mDownTime, 1200)
    , PARAM_INIT(mPolluteFlag, 0)
    , PARAM_INIT(mPolluteSize, 160.0f)
{
	TParams::load(mPrmPath);
}

void TEnemyMario::initValues()
{
	mHealth     = mDeParams.mHpMax.get();
	mDirty      = 0.0f;
	mOilBrake   = 1.0f;
	mDirtyTimer = 0;
	unk140      = 0.0f;

	unk108              = new TMarioControllerWork;
	unk108->mStickHS16  = 0;
	unk108->mStickVS16  = 0;
	unk108->mStickH     = 0.0f;
	unk108->mStickV     = 0.0f;
	unk108->mStickDist  = 0.0f;
	unk108->mInput      = 0;
	unk108->mFrameInput = 0;
	unk108->mAnalogRU8  = 0;
	unk108->mAnalogLU8  = 0;
	unk10C              = 0.0f;
	unk110              = 0.0f;

	unk154 = new TWaterEmitInfo("/Mario/DamageWaterEmit.prm");
	unk158 = new TWaterEmitInfo("/Mario/WetWaterEmit.prm");

	mPlayerType = PLAYER_TYPE_SHADOW_MARIO;
	unk530      = nullptr;
	unk534      = 0;
	unk536      = 0;
	unk538      = 0;
	unk53A      = 0;
	unk53B      = 0;
	unk530      = new s16[60];
	for (int i = 0; i < 60; ++i) {
		unk530[i] = 0;
	}

	initModel();

	mWaistRoll  = 0.0f;
	mWaistPitch = 0.0f;
	mCap        = nullptr;
	mWaterGun   = nullptr;
	mYoshi      = nullptr;

	mMarioEffect = new TMarioEffect;
	mMarioEffect->init(this);
	unk414.set(0.0f, 0.0f, 1.0f);
	mMarioScreenPos.set(0.0f, 0.0f, 0.0f);
	mWarpInDir.set(0.0f, 0.0f, 0.0f);
	unk468 = 0.0f;
	unk46C = 0.0f;

	mAnmSound = new MAnmSound(SMSGetMSound());
	mAnmSound->initAnmSound(nullptr, 1, 0.0f);
	unk4EC          = 0;
	mBlendLogicOp   = 10;
	mWaterWakeAlpha = 0;

	unk390 = new TMBindShadowBody(this, mModel->getModel(), 1.0f);
}

void TEnemyMario::initModel()
{
	unk394 = nullptr;
	unk398 = nullptr;
	unk39C = nullptr;
	unk3A0 = nullptr;

	TMario* original = gpMarioOriginal;
	mBodyModelData   = original->mModel->getModel()->getModelData();
	mJointIdCenter   = mBodyModelData->getJointName()->getIndex("center");
	mJointIdChest    = mBodyModelData->getJointName()->getIndex("chn_chest");
	mJointIdChnChest = mBodyModelData->getJointName()->getIndex("jnt_chest");
	mJointIdArmR1    = mBodyModelData->getJointName()->getIndex("jnt_arm_R1");
	mJointIdArmL1    = mBodyModelData->getJointName()->getIndex("jnt_arm_L1");
	mJointIdHandR    = mBodyModelData->getJointName()->getIndex("jnt_hand_R");
	mJointIdHandL    = mBodyModelData->getJointName()->getIndex("jnt_hand_L");
	mJointIdChnFootR = mBodyModelData->getJointName()->getIndex("chn_foot_R");
	mJointIdFootR    = mBodyModelData->getJointName()->getIndex("jnt_foot_R");
	mJointIdChnFootL = mBodyModelData->getJointName()->getIndex("chn_foot_L");
	mJointIdFootL    = mBodyModelData->getJointName()->getIndex("jnt_foot_L");
	mJointIdHead     = mBodyModelData->getJointName()->getIndex("jnt_head");
	mJointIdMHead    = mBodyModelData->getJointName()->getIndex("M_head");

	J3DModel* bodyModel = new J3DModel(mBodyModelData, 0, 1);
	mHandModels[0][0]   = nullptr;
	mHandModels[0][1]   = nullptr;
	mHandModels[1][0]   = nullptr;
	mHandModels[1][1]   = nullptr;

	mAnmSoundTbl = new JAIAnimeSound*[199];
	char buffer[0x10C];
	for (int i = 0; i < 199; ++i) {
		snprintf(buffer, 0xff, "/mario/bas/ma_%s.bas", marioAnimeFiles[i].unk4);
		loadBas((void**)&mAnmSoundTbl[i], buffer);
	}

	J3DAnmTexPattern** anmTexPattern = new J3DAnmTexPattern*[24];
	J3DTexNoAnm** anmTexNoAnm        = new J3DTexNoAnm*[24];
	for (int i = 0; i < 24; ++i) {
		loadAnmTexPattern(&anmTexPattern[i], marioAnimeTexPatternFilenames[i],
		                  mBodyModelData);
		anmTexNoAnm[i]
		    = new J3DTexNoAnm[anmTexPattern[i]->getUpdateMaterialNum()];
		for (int j = 0; j < anmTexPattern[i]->getUpdateMaterialNum(); ++j) {
			anmTexNoAnm[i][j].setAnmIndex(j);
			anmTexNoAnm[i][j].setAnmTexPattern(anmTexPattern[i]);
		}
	}

	M3UMtxCalcSIAnmBlendQuat* anmBlendQuat = new M3UMtxCalcSIAnmBlendQuat[2];
	anmBlendQuat[0].unk50                  = 0.0f;
	J3DFrameCtrl* frameCtrl                = new J3DFrameCtrl[3];

	M3UModelCommonMario* marioCommon = new M3UModelCommonMario;
	marioCommon->unk4                = original->mModel->unk4->unk4;
	marioCommon->unk18               = anmBlendQuat;
	marioCommon->unk8                = anmTexPattern;
	marioCommon->unk8                = original->mModel->unk4->unk8;
	marioCommon->unkC                = anmTexNoAnm;

	M3UModelMario* modelMario = new M3UModelMario;
	modelMario->unk8          = bodyModel;
	modelMario->unk4          = marioCommon;
	modelMario->unk20         = marioCommon;
	modelMario->unkC          = frameCtrl;
	frameCtrl[2].setRate(SMSGetAnmFrameRate());

	SomeModelMarioStruct* setInfo = new SomeModelMarioStruct[2];
	setInfo[0] = (SomeModelMarioStruct) { 0, 2, 0, 0x14, 0x41, 0 };
	setInfo[1] = (SomeModelMarioStruct) { mJointIdChnChest, 2, 1, 0, 0, 1 };
	modelMario->unk10 = 2;
	modelMario->unk24 = setInfo;

	M3UModel::Unk1CStruct* unk = new M3UModel::Unk1CStruct;
	*unk                       = (M3UModel::Unk1CStruct) { 0, 2 };
	modelMario->unk1C          = unk;
	modelMario->changeMtxCalcSIAnmBQAnmTransform(0, 0, 0x3e);
	modelMario->changeMtxCalcSIAnmBQAnmTransform(1, 0, 0x41);
	modelMario->unkC[1].setRate(0.0f);
	marioCommon->unk18[1].unk50 = 0.0f;
	mModel                      = modelMario;

	setAnimation(ANIM_WAIT, 1.0f);

	J3DTransformInfo transformInfo;
	transformInfo.mScale.x     = 1.0f;
	transformInfo.mScale.y     = 1.0f;
	transformInfo.mScale.z     = 1.0f;
	transformInfo.mRotation.x  = mFaceAngle.x;
	transformInfo.mRotation.y  = mModelFaceAngle;
	transformInfo.mRotation.z  = mFaceAngle.z;
	transformInfo.mTranslate.x = mPosition.x;
	transformInfo.mTranslate.y = mPosition.y;
	transformInfo.mTranslate.z = mPosition.z;
	Mtx transform;
	J3DGetTranslateRotateMtx(transformInfo, transform);
	mModel->getModel()->setBaseTRMtx(transform);

	mModel->updateInMotion();
	mModel->getModel()->calc();

	mSurfGesso = nullptr;
	mTorocco   = nullptr;
	mPinaRail  = nullptr;
	mKoopaRail = nullptr;

	mMultiMtxEffect                 = new TMultiMtxEffect;
	mMultiMtxEffect->mNumBones      = 3;
	u16* boneIDs                    = new u16[3];
	boneIDs[0]                      = mJointIdChnChest;
	boneIDs[1]                      = mJointIdArmR1;
	boneIDs[2]                      = mJointIdArmL1;
	mMultiMtxEffect->mBoneIDs       = boneIDs;
	u8* mtxEffectTypes              = new u8[3];
	mtxEffectTypes[0]               = 0;
	mtxEffectTypes[1]               = 0;
	mtxEffectTypes[2]               = 0;
	mMultiMtxEffect->mMtxEffectType = mtxEffectTypes;
	mMultiMtxEffect->setup(mModel->getModel(), "Mario");
}

// TODO: wrong! off by 1 instruction!
BOOL TEnemyMario::canJumpToNode() const
{
	// TODO: missing some inlines which getGraph should live inside of
	int nodeIndex
	    = mEMario->getTracer()->getGraph()->findNearestNodeIndex(mPosition, -1);
	return mEMario->getTracer()->getGraph()->getGraphNode(nodeIndex).checkFlag(
	    2);
}

// UNUSED in retail (inlined away), size 0x8 = 2 PPC instructions. A plain
// bit-test compiles to 7 (MWCC's neg/subic/subfe bool normalization). 2 instrs
// is just a load + blr, i.e. no room to mask — the real body must return a
// non-normalized value. Exact form is TODO (dead code, no callsite to anchor).
bool TEnemyMario::isDispPencil() const { return false; }

void TEnemyMario::initEnemyValues()
{
	static const char* names[5] = {
		"マリオモドキ_0", "マリオモドキ_1", "マリオモドキ_2",
		"マリオモドキ_3", "モンテマン",
	};
	static const char* bmdFileNames[7] = {
		"/kagemario/kagemario_model.bmd",
		"/kagemario/kagemario_model.bmd",
		"/kagemario/kagemario_model.bmd",
		"/kagemario/kagemario_model.bmd",
		"/scene/map/map/pad/monteman_model.bmd",
		nullptr,
		nullptr,
	};

	mEMFlags             = EM_FLAG_DISP_PENCIL;
	mEMDoing             = EM_DOING_UNK1A;
	mWaterCounter        = 0;
	mAngleToMario        = 0;
	mTargetAngle         = 0;
	mDistanceToMario     = 0.0f;
	mEMDoingTimer        = 0;
	mReplayIndex         = 0;
	mTrembleStrength     = 2.5f;
	mPadIndex            = 0;
	mAttackRange         = 120.0f;
	mWaterEffectTimer    = 0;
	mWaterEffectTimerMax = 120;
	mTrampleCount        = 3;
	mWaterHitTimer       = 0;
	mReplayJumpSpeed     = 15.0f;
	mRunAwayNodeIndex    = 0;
	mRunAwaySpeed        = 10.0f;

	onHitFlag(HIT_FLAG_NO_COLLISION);
	mEMario->onHitFlag(HIT_FLAG_NO_COLLISION);

	int modelIndex = 6;
	for (int i = 0; i < 5; ++i) {
		if (strcmp(names[i], mEMario->getName()) == 0) {
			modelIndex = i;
			break;
		}
	}

	J3DModelData* specialModelData = nullptr;
	if (modelIndex >= 0 && modelIndex < 4) {
		mPlayerType = PLAYER_TYPE_SHADOW_MARIO;
	} else if (modelIndex == 4) {
		specialModelData = J3DModelLoaderDataBase::load(
		    JKRGetResource(bmdFileNames[modelIndex]), 0x10040000);
		mPlayerType = PLAYER_TYPE_MONTE_MAN;
	}

	mBrushModel                = nullptr;
	mStampActor                = nullptr;
	mBrushScaleupDuringDrawing = 3.0f;
	if (specialModelData != nullptr) {
		mPlayerType   = PLAYER_TYPE_MONTE_MAN;
		mSpecialModel = new J3DModel(specialModelData, 0, 1);
	} else {
		mPlayerType                   = PLAYER_TYPE_SHADOW_MARIO;
		mSpecialModel                 = nullptr;
		J3DModelData* pencilModelData = J3DModelLoaderDataBase::load(
		    JKRGetResource("/scene/kagemario/kagemario_brush.bmd"), 0x11040000);
		mBrushModel           = new J3DModel(pencilModelData, 0, 1);
		ResTIMG* dirtyTexture = (ResTIMG*)JKRGetResource(cDirtyFileName);
		if (dirtyTexture != nullptr)
			SMS_ChangeTextureAll(pencilModelData, cDirtyTexName, *dirtyTexture);

		mStampActor = SMS_MakeMActor(
		    "/scene/kagemario/stamp_koopa_sign",
		    "/scene/kagemario/stamp_koopa_sign/stamp_koopa_sign_model1.bmd", 3,
		    0x10210000);
		SMS_LoadParticle("/scene/kagemario/jpa/ms_kgm_change.jpa",
		                 SCENE_KAGEMARIO_JPA_MS_KGM_CHANGE);
		SMS_LoadParticle("/scene/kagemario/jpa/ms_kgm_move_a.jpa",
		                 SCENE_KAGEMARIO_JPA_MS_KGM_MOVE_A);
		SMS_LoadParticle("/scene/kagemario/jpa/ms_kgm_move_b.jpa",
		                 SCENE_KAGEMARIO_JPA_MS_KGM_MOVE_B);
	}

	mDisappearPosition.z = 0.0f;
	mDisappearPosition.y = 0.0f;
	mDisappearPosition.x = 0.0f;
	int shadowMarioEvent = TFlagManager::getInstance()->getFlag(0x60003);
	if (shadowMarioEvent == 0) {
		onHitFlag(HIT_FLAG_NO_COLLISION);
		mEMario->onHitFlag(HIT_FLAG_NO_COLLISION);
	} else {
		offHitFlag(HIT_FLAG_NO_COLLISION);
		mEMario->offHitFlag(HIT_FLAG_NO_COLLISION);
	}

	if (shadowMarioEvent == 2) {
		mPadIndex      = 1;
		mSettingParams = new TSettingParams("/../map/pad2/Setting.prm");
	} else if (shadowMarioEvent == 3) {
		mPadIndex      = 2;
		mSettingParams = new TSettingParams("/../map/pad3/Setting.prm");
	} else if (shadowMarioEvent == 0 || shadowMarioEvent == 1) {
		mPadIndex      = 0;
		mSettingParams = new TSettingParams("/../map/pad/Setting.prm");
	}

	mWaterCounter          = mSettingParams->mWaterCtMax.get();
	char** replayFileNames = nullptr;
	int replayCount        = 0;
	char linkDataPath[0x100];
	if (mPadIndex == 0) {
		snprintf(linkDataPath, sizeof(linkDataPath),
		         "/scene/map/map/pad/linkdata.bin");
	} else {
		snprintf(linkDataPath, sizeof(linkDataPath),
		         "/scene/map/map/pad%d/linkdata.bin", mPadIndex);
	}

	void* linkData = JKRGetResource(linkDataPath);
	if (linkData != nullptr) {
		s32 linkDataSize
		    = JKRFileLoader::getVolume("scene")->getResSize(linkData);
		JSUMemoryInputStream stream(linkData, linkDataSize);
		stream.skip(6);
		stream.readString();
		stream.skip(2);
		stream.readString();

		u32 nodeCount;
		stream.read(&nodeCount, sizeof(nodeCount));
		mReplayLinks    = new TReplayLink[nodeCount][3];
		replayFileNames = new char*[nodeCount * 3];
		for (u32 i = 0; i < nodeCount * 3; ++i) {
			replayFileNames[i] = new char[3];
		}

		for (u32 node = 0; node < nodeCount; ++node) {
			stream.skip(6);
			stream.readString();
			stream.skip(2);
			stream.readString();
			for (int link = 0; link < 3; ++link) {
				stream.skip(2);
				u8 replayLetter;
				stream.read(&replayLetter, sizeof(replayLetter));
				if (replayLetter == '*') {
					mReplayLinks[node][link].mNodeIndex   = 0xff;
					mReplayLinks[node][link].mReplayIndex = 0xff;
				} else {
					snprintf(replayFileNames[replayCount], 3, "%c%c",
					         (char)(node + 'A'), (char)replayLetter);
					mReplayLinks[node][link].mNodeIndex   = replayLetter - 'A';
					mReplayLinks[node][link].mReplayIndex = replayCount;
					++replayCount;
				}
			}
		}
	}

	if (mPadIndex != 0) {
		char graphName[0x100];
		snprintf(graphName, sizeof(graphName), "mariomodoki%d", mPadIndex);
		mEMario->getTracer()->setGraph(gpConductor->getGraphByName(graphName));
	}

	mTrembleStrength = 2.5f;
	if (mPlayerType == PLAYER_TYPE_MONTE_MAN) {
		replayFileNames = (char**)recordFileNamesMonteMan;
		replayCount     = 3;
	}

	if (replayCount > 0) {
		mInputReplays = new TMarioInputReplay*[replayCount];
		for (int i = 0; i < replayCount; ++i) {
			char replayPath[0x100];
			if (mPadIndex == 0) {
				snprintf(replayPath, sizeof(replayPath),
				         "/scene/map/map/pad/tutorial%s.pad",
				         replayFileNames[i]);
			} else {
				snprintf(replayPath, sizeof(replayPath),
				         "/scene/map/map/pad%d/tutorial%s.pad", mPadIndex,
				         replayFileNames[i]);
			}
			u8* replayData   = (u8*)JKRGetResource(replayPath);
			mInputReplays[i] = new TMarioInputReplay;
			mInputReplays[i]->init(replayData);
		}
	}

	mGamePad = gpMarDirector->unk18[1];
	mEMFlags = EM_FLAG_DISP_PENCIL;
	switch (mEMario->mInitialState) {
	case 0:
		mEMDoing = EM_DOING_REPLAY_WAITING;
		break;
	case 1:
		mEMDoing = EM_DOING_REPLAY;
		break;
	case 2:
		mEMDoing = EM_DOING_UNK12;
		break;
	case 3:
		mEMDoing = EM_DOING_WAITING_MARIO;
		break;
	default:
		mEMDoing = EM_DOING_DISAPPEAR;
		break;
	}

	if (shadowMarioEvent == 0)
		mEMDoing = EM_DOING_DISAPPEAR;

	switch (shadowMarioEvent) {
	case 2:
		mReplayIndex = mEMario->unk15C;
		break;
	case 3:
		mReplayIndex = mEMario->unk160;
		break;
	default:
		mReplayIndex = mEMario->unk158;
		break;
	}
	mEMario->getTracer()
	    ->getGraph()
	    ->getGraphNode(mReplayIndex)
	    .getPoint(&mPosition);
	mEMario->mPosition = mPosition;
	if (replayCount > 0) {
		mInputReplays[mReplayIndex]->reset();
		mInputReplays[mReplayIndex]->start();
	} else {
		mInputReplays = nullptr;
	}

	if (gpMarDirector->mMap == 1 && gpMarDirector->unk7D == 1) {
		mRunAwayInputReplays = new TMarioInputReplay*[8];
		for (int i = 0; i < 8; ++i) {
			if (recordFileNamesDolpic1[i] != nullptr) {
				char runAwayPath[0x100];
				snprintf(runAwayPath, sizeof(runAwayPath),
				         "/scene/map/map/pad/tutorial%s.pad",
				         recordFileNamesDolpic1[i]);
				u8* replayData          = (u8*)JKRGetResource(runAwayPath);
				mRunAwayInputReplays[i] = new TMarioInputReplay;
				mRunAwayInputReplays[i]->init(replayData);
			} else {
				mRunAwayInputReplays[i] = nullptr;
			}
		}
		u8* gateReplayData
		    = (u8*)JKRGetResource("/scene/map/map/pad/tutorialHI.pad");
		mGateReplay = new TMarioInputReplay;
		mGateReplay->init(gateReplayData);
	} else {
		mRunAwayInputReplays = nullptr;
		mGateReplay          = nullptr;
	}

	if (gpMarDirector->mMap == 12) {
		if (strcmp(mEMario->getName(), "マリオ２Ｐ") == 0) {
			mPlayerType = TMario::PLAYER_TYPE_P2;
			mGamePad    = gpMarDirector->unk18[1];
		}
		if (strcmp(mEMario->getName(), "マリオ３Ｐ") == 0) {
			mPlayerType = TMario::PLAYER_TYPE_P3;
			mGamePad    = gpMarDirector->unk18[2];
		}
		if (strcmp(mEMario->getName(), "マリオ４Ｐ") == 0) {
			mPlayerType = TMario::PLAYER_TYPE_P4;
			mGamePad    = gpMarDirector->unk18[3];
		}
		mEMDoing = EM_DOING_GET_PAD;
		if (mPlayerType == TMario::PLAYER_TYPE_P2
		    || mPlayerType == TMario::PLAYER_TYPE_P3
		    || mPlayerType == TMario::PLAYER_TYPE_P4) {
			mTrembleModelEffect = new TTrembleModelEffect;
			mTrembleModelEffect->init(mModel->getModel());
		}
	}
	if (mTrembleModelEffect != nullptr) {
		mTrembleModelEffect->clash(mTrembleStrength);
	}

	onUnk114(2);
	mStatus     = MARIO_STATUS_WAIT;
	mPrevStatus = MARIO_STATUS_WAIT;
	offFlag(MARIO_FLAG_HAS_FLUDD);
	if (mPadIndex == 2)
		gpMapObjWave->noWave();

	mHandModels[0][0] = nullptr;
	mHandModels[0][1] = nullptr;
	mHandModels[1][0] = nullptr;
	mHandModels[1][1] = nullptr;
	mWaterGun         = nullptr;
	mCap              = nullptr;
	mYoshi            = nullptr;
	mMultiMtxEffect   = nullptr;
}

void TEnemyMario::kill() { }

f32 TEnemyMario::getStickPower() { }

void TEnemyMario::setStickAgainstMario() { }

void TEnemyMario::setStickToAngle(s16 angle, f32 power)
{
	unk108->mStickHS16 = (JMASSin(angle) * 64.0f) * power;
	unk108->mStickVS16 = (-JMASCos(angle) * 64.0f) * power;
}

void TEnemyMario::resetReplayStatus()
{
	mVel.set(0.0f, 0.0f, 0.0f);
	mForwardVel = 0.0f;
	resetHistory();
	changePlayerStatus(MARIO_STATUS_WAIT, 0, true);
}

void TEnemyMario::startMonteReplay(u32 replayIndex)
{
	int nodeIndex
	    = mEMario->getTracer()->getGraph()->findNearestNodeIndex(mPosition, -1);
	JGeometry::TVec3<f32> currentPoint;
	mEMario->getTracer()->getGraph()->getGraphNode(nodeIndex).getPoint(
	    &currentPoint);
	mPosition = currentPoint;
	JGeometry::TVec3<f32> nextPoint;
	mEMario->getTracer()
	    ->getGraph()
	    ->getGraphNode(nodeIndex + 1)
	    .getPoint(&nextPoint);
	f32 xDifference = nextPoint.x - currentPoint.x;
	f32 zDifference = nextPoint.z - currentPoint.z;
	mFaceAngle.y    = matan(zDifference, xDifference);
	resetReplayStatus();
	mReplayIndex = replayIndex;
	mInputReplays[mReplayIndex]->reset();
	mInputReplays[mReplayIndex]->start();
	changeEMDoing(EM_DOING_REPLAY_TO_GOAL);
}

void TEnemyMario::changeEMDoing(u16 doing)
{
	mEMDoingTimer = 0;
	mEMDoing      = doing;
}

void TEnemyMario::changeEMJumping()
{
	unk108->mInput |= TMarioControllerWork::A;
	changeEMDoing(EM_DOING_JUMPING);
}

void TEnemyMario::changeEMWalkGraph()
{
	TEMario* emario = mEMario;
	emario->getTracer()->reset();
	emario->goToShortestNextGraphNode();
	changeEMDoing(EM_DOING_WALK_GRAPH);
}

bool TEnemyMario::tryTake()
{
	if (mHeldObject != nullptr && mStatus != MARIO_STATUS_TAKE)
		return TRUE;

	for (int i = 0; i < mEMario->getColNum(); ++i) {
		THitActor* actor = mEMario->getCollision(i);
		u32 actorType    = actor->getActorType();
		if (actorType == 0x04000018 || actorType == 0x2000002A
		    || actorType == 0x20000022 || actorType == 0x20000009) {
			if (actorType == 0x04000018) {
				((TLiveActor*)actor)->onLiveFlag(LIVE_FLAG_UNK100000);
				onEMFlag(EM_FLAG_ENFORCE_TAKE);
			}
			unk384 = actor;
			changePlayerStatus(MARIO_STATUS_TAKE, 0, false);
		}
	}
	return FALSE;
}

void TEnemyMario::emWaiting()
{
	s16 angleDifference = mAngleToMario - mFaceAngle.y;
	if (angleDifference < -0x1555 || angleDifference > 0x1555)
		setStickToAngle(mAngleToMario, 0.2f);

	if (mDistanceToMario < 800.0f)
		changeEMDoing(EM_DOING_RUN_AWAY);

	if (mDistanceToMario > 1500.0f || rand() < 0x88)
		changeEMWalkGraph();
}

void TEnemyMario::emRunAway()
{
	if (mDistanceToMario < 400.0f)
		changeEMJumping();

	if (mDistanceToMario < 1300.0f) {
		setStickToAngle(mAngleToMario, 1.0f);
		unk108->mStickHS16 = -unk108->mStickHS16;
		unk108->mStickVS16 = -unk108->mStickVS16;
	} else {
		mEMDoing = EM_DOING_WAITING;
	}
}

void TEnemyMario::emJumping()
{
	if (mStatus & MARIO_STATUS_FLAG_JUMPING) {
		if (mStatus != MARIO_STATUS_WALL_SLIDE || mStatusTimer >= 10) {
			setStickToAngle(mFaceAngle.y, 1.0f);
			unk108->mInput |= TMarioControllerWork::A;
			if (-1.0f < mVel.y && mVel.y < 1.0f && rand() < 0xFFF) {
				unk108->mInput |= TMarioControllerWork::B;
			}
		}
	} else if (mStatus == MARIO_STATUS_HANGING) {
		if (mStatusTimer >= 10) {
			unk108->mInput |= TMarioControllerWork::A;
		}
	} else if (mStatus & 0x600) {
		TPollutionManager* pollution = gpPollution;
		pollution->stamp(1, mPosition.x, mPosition.y, mPosition.z, 384.0f);
		changeEMDoing(EM_DOING_WAITING);
	}
}

void TEnemyMario::emGetCloser()
{
	if (mDistanceToMario > 1500.0f) {
		setStickToAngle(mAngleToMario, 1.0f);
	} else {
		changeEMDoing(EM_DOING_WAITING);
	}
}

void TEnemyMario::emWalkAround()
{
	if (mDistanceToMario < 1500.0f) {
		changeEMDoing(EM_DOING_WAITING);
		return;
	}
	if (rand() < 10) {
		changeEMDoing(EM_DOING_GET_CLOSER);
	}
	if (rand() < 100) {
		changeEMJumping();
		return;
	}
	if (rand() < 100) {
		mTargetAngle = rand();
		changeEMDoing(EM_DOING_TURNING);
		return;
	}
	if (rand() < 50) {
		changeEMWalkGraph();
		return;
	}
	if (rand() < 50) {
		TPollutionManager* pollution = gpPollution;
		pollution->stamp(1, mPosition.x, mPosition.y, mPosition.z, 384.0f);
		changeEMDoing(EM_DOING_HIDE);
	}
	if (mWallPlane != nullptr) {
		changeEMJumping();
		return;
	}
	setStickToAngle(mFaceAngle.y, 0.5f);
}

void TEnemyMario::emWalkGraph()
{
	if ((mEMario->getUnk104().getPoint() - mEMario->mPosition).length()
	    < 100.0f) {
		if (mDistanceToMario > 3000.0f)
			mEMario->goToRandomNextGraphNode();
		else
			mEMario->goToRandomEscapeGraphNode();
	}
	const JGeometry::TVec3<f32>& goal = mEMario->getUnkF4().getPoint();
	u16 angle = matan(goal.z - mPosition.z, goal.x - mPosition.x);
	setStickToAngle(angle, 1.0f);
	++mEMDoingTimer;
	if (mEMDoingTimer % 100 == 0)
		gpConductor->makeEnemyAppear(mPosition, "ハムクリマネージャー", 1, 0);
}

void TEnemyMario::emTurning()
{
	s16 angleDifference = mTargetAngle - mFaceAngle.y;
	if (rand() < 100) {
		changeEMJumping();
	} else if (angleDifference < -0x1555 || angleDifference > 0x1555) {
		setStickToAngle(mTargetAngle, 0.2f);
	} else {
		changeEMDoing(EM_DOING_WAITING);
	}
}

void TEnemyMario::emHide()
{
	onHitFlag(HIT_FLAG_NO_COLLISION);
	mEMario->onHitFlag(HIT_FLAG_NO_COLLISION);
	++mEMDoingTimer;
	if (!gpPollution->isPolluted(mPosition.x, mPosition.y, mPosition.z)
	    || mEMDoingTimer > 7200) {
		mEMDoingTimer        = 0;
		mInvincibilityFrames = 120;
		changeEMDoing(EM_DOING_APPEAR);
	}
}

void TEnemyMario::emAppear()
{
	if (mInvincibilityFrames == 0) {
		changeEMDoing(EM_DOING_WAITING);
		offHitFlag(HIT_FLAG_NO_COLLISION);
		mEMario->offHitFlag(HIT_FLAG_NO_COLLISION);
	}
}

void TEnemyMario::startDisappear(u16 doing)
{
	mDisappearPosition = mPosition;

	u8 currentMap      = gpMarDirector->getCurrentMap();
	u8 currentStage    = gpMarDirector->getCurrentStage();
	bool keepBossLives = false;
	if (currentMap == 1 && currentStage == 1) {
		keepBossLives = true;
	} else if (currentMap == 1 && currentStage == 9) {
		keepBossLives = true;
	}

	if (!keepBossLives) {
		MSMainProc::setBossLivesFlag(false);
	}

	gpMarioParticleManager->emitAndBindToPosPtr(
	    SCENE_KAGEMARIO_JPA_MS_KGM_CHANGE, &mDisappearPosition, 0, nullptr);
	changeEMDoing(doing);
}

void TEnemyMario::emDisappear()
{
	onHitFlag(HIT_FLAG_NO_COLLISION);
	mEMario->onHitFlag(HIT_FLAG_NO_COLLISION);
	offEMFlag(EM_FLAG_DISP_PENCIL);
	offUnk114(UNK114_FLAG_VISIBLE);
	changePlayerStatus(MARIO_STATUS_NOMOTION, 0, false);
}

void TEnemyMario::emDisappearToGate()
{
	if (mEMDoingTimer >= 8) {
		offEMFlag(EM_FLAG_DISP_PENCIL);
	} else {
		onEMFlag(EM_FLAG_DISP_PENCIL);
	}

	onHitFlag(HIT_FLAG_NO_COLLISION);
	mEMario->onHitFlag(HIT_FLAG_NO_COLLISION);
	runAwayMoveEffect();

	if (mEMDoingTimer == 0) {
		mDisappearPosition = mCenterPos;
		gpMarioParticleManager->emit(SCENE_KAGEMARIO_JPA_MS_KGM_CHANGE,
		                             &mDisappearPosition, 0, nullptr);
		SMSGetMSound()->startSoundActor(MSD_SE_MA_KAGE_FIELD_AWAY, &mPosition,
		                                0, nullptr, 0, 4);
	}

	if (mEMDoingTimer > 100) {
		mDisappearPosition.y += 0.025f * mEMDoingTimer;
		mDisappearPosition.z -= 0.03f * mEMDoingTimer;
	}
	++mEMDoingTimer;
}

void TEnemyMario::emReplay()
{
	mInputReplays[mReplayIndex]->play(&mIntendedMag, &mIntendedYaw,
	                                  &unk108->mInput, &unk108->mFrameInput,
	                                  &unk108->mAnalogLU8, &unk108->mAnalogRU8);

	if (mSettingParams->mPolluteFlag.get() && gpPollution != nullptr) {
		gpPollution->pollute(mPosition.x, mPosition.y, mPosition.z,
		                     mSettingParams->mPolluteSize.get());
	}

	if (mInputReplays[mReplayIndex]->canPlay()) {
		return;
	}

	if (mSettingParams->mCarryFlag.get() == 1 && mHeldObject == nullptr) {
		changeEMDoing(EM_DOING_UNK12);
		return;
	}

	if (mStampActor != nullptr && mSettingParams->mStampFlag.get() == 1) {
		mStampActor->setBck("stamp_koopa_sign_draw1");
		MActor* stampActor = mStampActor;
		stampActor->setFrameRate(SMSGetAnmFrameRate(), 0);
		changeEMDoing(EM_DOING_UNK13);
		startSoundActor(MSD_SE_MA_KAGE_PAINTS);
		startSoundActor(MSD_SE_MA_KAGE_PAINTINGWIND);
		return;
	}

	int nodeIndex
	    = mEMario->getTracer()->getGraph()->findNearestNodeIndex(mPosition, -1);
	if (mEMario->getTracer()->getGraph()->getGraphNode(nodeIndex).checkFlag(
	        0x40)) {
		changeEMDoing(EM_DOING_WAITING_MARIO);
		return;
	}

	if (mSettingParams->mStopFlag.get() == 1) {
		changeEMDoing(EM_DOING_REPLAY_WAITING);
		return;
	}

	emReplayWaitingToReplayJumpToNearestNode();
}

void TEnemyMario::emReplayWaitingToReplayJumpToNearestNode()
{
	if (canJumpToNode()) {
		mFaceAngle.y = mAngleToMario;
		unk108->mFrameInput |= TMarioControllerWork::A;
		unk108->mInput |= TMarioControllerWork::A;
	}
	changeEMDoing(EM_DOING_REPLAY_JUMP_TO_NEAREST_NODE);
}

void TEnemyMario::emReplayWaiting()
{
	f32 dist = mPosition.distance(SMS_GetMarioPos());
	if (dist < mSettingParams->mSearchDist.get()) {
		if (SMS_GetMarioPos().y
		    < mPosition.y + mSettingParams->mSearchHeight.get()) {
			emReplayWaitingToReplayJumpToNearestNode();
		}
	}
}

void TEnemyMario::emReplayJumpToNearestNode()
{
	// TODO: Recover the original vector temporary and inline lifetimes; the
	// logic matches, but retail reserves a larger stack frame and saves two
	// additional floating-point registers.
	if (canJumpToNode()) {
		unk108->mFrameInput |= TMarioControllerWork::A;
		unk108->mInput |= TMarioControllerWork::A;
		if (mVel.y > mReplayJumpSpeed)
			mVel.y = mReplayJumpSpeed;
	}

	++mEMDoingTimer;
	int nodeIndex
	    = mEMario->getTracer()->getGraph()->findNearestNodeIndex(mPosition, -1);
	TGraphNode* currentNode
	    = &mEMario->getTracer()->getGraph()->getGraphNode(nodeIndex);
	JGeometry::TVec3<f32> currentPoint;
	currentNode->getPoint(&currentPoint);
	mPosition.x += 0.05f * (currentPoint.x - mPosition.x);
	mPosition.z += 0.05f * (currentPoint.z - mPosition.z);
	mPosition.y += 0.05f * (currentPoint.y - mPosition.y);

	if (mStatus != MARIO_STATUS_WAIT)
		if (canJumpToNode())
			return;

	mPosition = currentPoint;
	resetReplayStatus();
	TReplayLink(*replayLinks)[3] = mReplayLinks;
	currentNode->getPoint(&mPosition);

	JGeometry::TVec3<f32> marioDirection(*gpMarioPos - currentPoint);
	marioDirection.normalize();
	TGraphNode* nextNode = nullptr;

	if (mSettingParams->mRandomFlag.get() == 0) {
		TReplayLink* links = replayLinks[nodeIndex];
		f32 smallestDot    = 1.0f;
		for (int i = 0; i < 3; ++i) {
			TReplayLink& link = links[i];
			if (link.mNodeIndex == 0xFF) {
				continue;
			}

			TGraphNode* candidate
			    = &mEMario->getTracer()->getGraph()->getGraphNode(
			        link.mNodeIndex);
			JGeometry::TVec3<f32> candidatePoint;
			candidate->getPoint(&candidatePoint);
			JGeometry::TVec3<f32> candidateDirection(candidatePoint
			                                         - currentPoint);
			candidateDirection.normalize();
			f32 dot = marioDirection.dot(candidateDirection);
			if (dot < smallestDot) {
				smallestDot  = dot;
				nextNode     = candidate;
				mReplayIndex = link.mReplayIndex;
			}
		}
	} else {
		TReplayLink* links = replayLinks[nodeIndex];
		f32 dots[3];
		int validLinks[3];
		int validCount = 0;
		for (int i = 0; i < 3; ++i) {
			dots[i]           = 0.0f;
			TReplayLink& link = links[i];
			if (link.mNodeIndex == 0xFF) {
				continue;
			}

			JGeometry::TVec3<f32> candidatePoint;
			mEMario->getTracer()
			    ->getGraph()
			    ->getGraphNode(link.mNodeIndex)
			    .getPoint(&candidatePoint);
			JGeometry::TVec3<f32> candidateDirection(candidatePoint
			                                         - currentPoint);
			candidateDirection.normalize();
			dots[validCount]       = marioDirection.dot(candidateDirection);
			validLinks[validCount] = i;
			++validCount;
		}

		f32 weights[3];
		f32 weightTotal = 0.0f;
		for (int i = 0; i < validCount; ++i) {
			weights[i] = 1.0f - dots[i];
			weights[i] = powf(weights[i], mSettingParams->mRandomPow.get());
			weightTotal += weights[i];
		}
		for (int i = 0; i < validCount; ++i) {
			weights[i] /= weightTotal;
		}

		f32 choice   = MsRandF();
		int selected = 0;
		for (int i = 0; i < validCount; ++i) {
			choice -= weights[i];
			if (choice <= 0.0f) {
				selected = i;
				break;
			}
		}

		TReplayLink& link = links[validLinks[selected]];
		mReplayIndex      = link.mReplayIndex;
		nextNode
		    = &mEMario->getTracer()->getGraph()->getGraphNode(link.mNodeIndex);
	}

	JGeometry::TVec3<f32> nextPoint;
	if (nextNode != nullptr) {
		nextNode->getPoint(&nextPoint);
	}
	mPosition = currentPoint;
	mFaceAngle.y
	    = matan(nextPoint.z - currentPoint.z, nextPoint.x - currentPoint.x);
	resetReplayStatus();
	mInputReplays[mReplayIndex]->reset();
	mInputReplays[mReplayIndex]->start();
	changeEMDoing(EM_DOING_REPLAY);
}

void TEnemyMario::emPreDownAnimation()
{
	changePlayerStatus(MARIO_STATUS_NOMOTION, 0, true);
	setAnimation(ANIM_SDWNF, 1.0f);
	if (getMotionFrameCtrl().getFrame() > 25.0f) {
		mEMDoingTimer = 0;
		mEMDoing      = EM_DOING_DOWN_ANIMATION;
	}
}

#pragma dont_inline on
void TEnemyMario::emDownAnimation()
{
	changePlayerStatus(MARIO_STATUS_NOMOTION, 0, true);
	setAnimation(ANIM_FALL_DOWN_WAIT, 1.0f);

	if (gpMarDirector->isDemoMode3() || gpMarDirector->isDemoMode4()
	    || gpMarDirector->isTalkModeNow()) {
		mReferencePosition = mPosition;
		mDisappearPosition = mReferencePosition;
		return;
	}

	++mEMDoingTimer;
	mReferencePosition = mPosition;
	mDisappearPosition = mReferencePosition;
	if (gpMarDirector->getCurrentMap() != 1
	    && mEMDoingTimer > mSettingParams->mDownTime.get()) {
		mWaterCounter = mSettingParams->mWaterCtMax.get();
		changeEMDoing(EM_DOING_RUN_AWAY_TO_NEAREST_NODE);
	}
}
#pragma dont_inline off

void TEnemyMario::startRunAway()
{
	changeEMDoing(EM_DOING_RUN_AWAY_TO_NEAREST_NODE);
}

void TEnemyMario::findRunAwayNearestNode()
{
	int nearestIndex    = 0;
	int secondIndex     = 0;
	f32 nearestDistance = 100000.0f;
	f32 secondDistance  = 100000.0f;
	JGeometry::TVec3<f32> nearestPoint;
	JGeometry::TVec3<f32> secondPoint;

	for (int i = 0; i < mEMario->getTracer()->getGraph()->getNodeNum(); ++i) {
		JGeometry::TVec3<f32> point;
		mEMario->getTracer()->getGraph()->getGraphNode(i).getPoint(&point);
		f32 distance = JGeometry::TVec3<f32>(point - mPosition).length();
		if (distance < nearestDistance) {
			secondDistance  = nearestDistance;
			secondIndex     = nearestIndex;
			secondPoint     = nearestPoint;
			nearestDistance = distance;
			nearestIndex    = i;
			nearestPoint    = point;
		} else if (distance < secondDistance) {
			secondDistance = distance;
			secondIndex    = i;
			secondPoint    = point;
		}
	}

	if (JGeometry::TVec3<f32>(mPosition - nearestPoint).length()
	    < JGeometry::TVec3<f32>(mPosition - secondPoint).length()) {
		mRunAwayNodeIndex = nearestIndex;
	} else {
		mRunAwayNodeIndex = secondIndex;
	}
}

void TEnemyMario::runAwayMoveEffect()
{
	gpMarioParticleManager->emitAndBindToPosPtr(
	    SCENE_KAGEMARIO_JPA_MS_KGM_MOVE_A, &mDisappearPosition, 1, this);
	gpMarioParticleManager->emitAndBindToPosPtr(
	    SCENE_KAGEMARIO_JPA_MS_KGM_MOVE_B, &mDisappearPosition, 1, this);
}

void TEnemyMario::emRunAwayToNearestNode()
{
	TGraphWeb* graph = mEMario->getTracer()->getGraph();
	JGeometry::TVec3<f32> targetPoint;
	graph->getGraphNode(mRunAwayNodeIndex).getPoint(&targetPoint);
	runAwayMoveEffect();

	if (mEMDoingTimer >= 8 && mEMDoingTimer < 300) {
		offEMFlag(EM_FLAG_DISP_PENCIL);
	} else {
		onEMFlag(EM_FLAG_DISP_PENCIL);
	}

	switch (mEMDoingTimer) {
	case 0:
		findRunAwayNearestNode();
		mDisappearPosition = mPosition;
		mDisappearPosition.y += 80.0f;
		gpMarioParticleManager->emit(SCENE_KAGEMARIO_JPA_MS_KGM_CHANGE,
		                             &mDisappearPosition, 0, nullptr);
		SMSGetMSound()->startSoundActor(MSD_SE_MA_KAGE_FIELD_AWAY, &mPosition,
		                                0, nullptr, 0, 4);
		break;
	case 8:
		break;
	case 100: {
		JGeometry::TVec3<f32> direction = targetPoint - mReferencePosition;
		direction.normalize();
		direction.scale(mRunAwaySpeed);
		mDisappearPosition += direction;
		f32 dx = targetPoint.x - mDisappearPosition.x;
		f32 dz = targetPoint.z - mDisappearPosition.z;
		if (dx * dx + dz * dz < mRunAwaySpeed * mRunAwaySpeed) {
			mEMDoingTimer = 200;
		}
		--mEMDoingTimer;
		break;
	}
	case 200:
		mPosition = targetPoint;
		mPosition.y += 5.0f;
		break;
	case 220:
		gpMarioParticleManager->emit(SCENE_KAGEMARIO_JPA_MS_KGM_CHANGE,
		                             &mDisappearPosition, 0, nullptr);
		SMSGetMSound()->startSoundActor(MSD_SE_MA_KAGE_FIELD_APPEAR, &mPosition,
		                                0, nullptr, 0, 4);
		break;
	case 300:
		if (gpMarDirector->getCurrentMap() == 1) {
			JGeometry::TVec3<f32> waitingPoint;
			graph->getGraphNode(7).getPoint(&waitingPoint);
			mFaceAngle.y    = matan(waitingPoint.z - targetPoint.z,
			                        waitingPoint.x - targetPoint.x);
			mModelFaceAngle = mFaceAngle.y;
			mPosition       = targetPoint;
			mPosition.y += 5.0f;
			changePlayerStatus(MARIO_STATUS_WAIT, 0, true);
			mReplayIndex  = mRunAwayNodeIndex;
			mInputReplays = mRunAwayInputReplays;
			mInputReplays[mReplayIndex]->reset();
			mInputReplays[mReplayIndex]->start();
			changeEMDoing(EM_DOING_REPLAY_RUN_AWAY);
		} else {
			mPosition = targetPoint;
			mPosition.y += 5.0f;
			changePlayerStatus(MARIO_STATUS_WAIT, 0, true);
			emReplayWaitingToReplayJumpToNearestNode();
		}
		break;
	}
	++mEMDoingTimer;
}

void TEnemyMario::emReplayRunAway()
{
	mInputReplays[mReplayIndex]->play(&mIntendedMag, &mIntendedYaw,
	                                  &unk108->mInput, &unk108->mFrameInput,
	                                  &unk108->mAnalogLU8, &unk108->mAnalogRU8);
	if (!mInputReplays[mReplayIndex]->canPlay()) {
		changePlayerStatus(MARIO_STATUS_NOMOTION, 0, true);
		setAnimation(ANIM_MONTEMAN_WAIT, 1.0f);
		changeEMDoing(EM_DOING_WAITING_TO_INVITE_MARIO);
	}
}

void TEnemyMario::decideDoingAfterCarry()
{
	if (checkEMFlag(EM_FLAG_ENFORCE_TAKE)) {
		offEMFlag(EM_FLAG_ENFORCE_TAKE);
		emReplayWaitingToReplayJumpToNearestNode();
		return;
	}

	if (mSettingParams->mStopFlag.get() == 1) {
		changeEMDoing(EM_DOING_REPLAY_WAITING);
		return;
	}

	emReplayWaitingToReplayJumpToNearestNode();
}

void TEnemyMario::emEnforceTake()
{
	if (tryTake()) {
		if (mStampActor != nullptr && mSettingParams->mStampFlag.get() == 1) {
			mStampActor->setBck("stamp_koopa_sign_draw1");
			changeEMDoing(EM_DOING_UNK13);
		} else {
			decideDoingAfterCarry();
		}
	}
}

void TEnemyMario::emDrawStamp()
{
	if (mStampActor->curAnmEndsNext())
		decideDoingAfterCarry();
}

void TEnemyMario::emWaitingToInviteMario()
{
	JGeometry::TVec3<f32> waitingPoint;
	mEMario->getTracer()->getGraph()->getGraphNode(7).getPoint(&waitingPoint);
	mPosition           = waitingPoint;
	s16 angleDifference = mAngleToMario - mFaceAngle.y;
	mFaceAngle.y = mAngleToMario - IConverge(angleDifference, 0, 0x180, 0x180);
	changePlayerStatus(MARIO_STATUS_WAIT, 0, false);
	changeMontemanWaitingAnim();

	f32 distanceToMario = mPosition.distance(*gpMarioPos);
	if (distanceToMario < mSettingParams->mSearchDist.get()
	    && gpMarioPos->y < mPosition.y + mSettingParams->mSearchHeight.get()) {
		JGeometry::TVec3<f32> gatePoint;
		mEMario->getTracer()->getGraph()->getGraphNode(8).getPoint(&gatePoint);
		mFaceAngle.y
		    = matan(gatePoint.z - waitingPoint.z, gatePoint.x - waitingPoint.x);
		mModelFaceAngle = mFaceAngle.y;
		changePlayerStatus(MARIO_STATUS_WAIT, 0, true);
		mReplayIndex = 0;
		mGateReplay->reset();
		mGateReplay->start();
		changeEMDoing(EM_DOING_REPLAY_RUN_AWAY_TO_GATE);
	}
}

void TEnemyMario::emReplayRunAwayToGate()
{
	mGateReplay->play(&mIntendedMag, &mIntendedYaw, &unk108->mInput,
	                  &unk108->mFrameInput, &unk108->mAnalogLU8,
	                  &unk108->mAnalogRU8);
	if (!mGateReplay->canPlay()) {
		changePlayerStatus(MARIO_STATUS_NOMOTION, 0, true);
		setAnimation(ANIM_MONTEMAN_WAIT, 1.0f);
		changeEMDoing(EM_DOING_UNK18);
	}
}

void TEnemyMario::emReplayToGoal()
{
	mInputReplays[mReplayIndex]->play(&mIntendedMag, &mIntendedYaw,
	                                  &unk108->mInput, &unk108->mFrameInput,
	                                  &unk108->mAnalogLU8, &unk108->mAnalogRU8);
	if (!mInputReplays[mReplayIndex]->canPlay())
		reachGoal();
}

void TEnemyMario::emWaitingMario()
{
	emKeepStay();
	changePlayerStatus(MARIO_STATUS_WAIT, 0, false);
	changeMontemanWaitingAnim();
}

void TEnemyMario::emKeepStay()
{
	s16 angleDifference = mAngleToMario - mFaceAngle.y;
	mFaceAngle.y = mAngleToMario - IConverge(angleDifference, 0, 0x180, 0x180);
}

void TEnemyMario::startGateDrawing()
{
	changePlayerStatus(MARIO_STATUS_NOMOTION, 0, true);
	setAnimation(ANIM_DRAW, 1.0f);
	mEMDoingTimer = 0;
	mEMDoing      = EM_DOING_GATE_DRAWING;
	startSoundActor(MSD_SE_MA_KAGE_PAINTS);
}

void TEnemyMario::emGateDrawing()
{
	mEMario->getTracer()->getGraph()->getGraphNode(8).getPoint(&mPosition);
	mFaceAngle.y    = -0x8000;
	mModelFaceAngle = -0x8000;
	if (isLast1AnimeFrame())
		startDisappear(EM_DOING_DISAPPEAR_TO_GATE);
}

void TEnemyMario::emGetPad()
{
	JDrama::TGraphics graphics;
	TMario::checkController(&graphics);
}

// TODO: Reconstruct the retail deferred-inline boundaries that emit the
// TMatrix34/TRotation3 constructors and TPathNode::getPoint without changing
// shared JGeometry/Graph code generation.
void TEnemyMario::consider()
{
	switch (mEMDoing) {
	case EM_DOING_WAITING:
		emWaiting();
		break;
	case EM_DOING_RUN_AWAY:
		emRunAway();
		break;
	case EM_DOING_JUMPING:
		emJumping();
		break;
	case EM_DOING_GET_CLOSER:
		emGetCloser();
		break;
	case EM_DOING_WALK_AROUND:
		emWalkAround();
		break;
	case EM_DOING_WALK_GRAPH:
		emWalkGraph();
		break;
	case EM_DOING_TURNING:
		emTurning();
		break;
	case EM_DOING_HIDE:
		emHide();
		break;
	case EM_DOING_APPEAR:
		emAppear();
		break;
	case EM_DOING_DISAPPEAR:
		emDisappear();
		break;
	case EM_DOING_DISAPPEAR_TO_GATE:
		emDisappearToGate();
		break;
	case EM_DOING_REPLAY_WAITING:
		emReplayWaiting();
		break;
	case EM_DOING_REPLAY_JUMP_TO_NEAREST_NODE:
		emReplayJumpToNearestNode();
		break;
	case EM_DOING_PRE_DOWN_ANIMATION:
		emPreDownAnimation();
		break;
	case EM_DOING_DOWN_ANIMATION:
		emDownAnimation();
		break;
	case EM_DOING_RUN_AWAY_TO_NEAREST_NODE:
		emRunAwayToNearestNode();
		break;
	case EM_DOING_WAITING_MARIO:
		emWaitingMario();
		break;
	case EM_DOING_UNK12:
		emEnforceTake();
		break;
	case EM_DOING_UNK13:
		emDrawStamp();
		break;
	case EM_DOING_WAITING_TO_INVITE_MARIO:
		emWaitingToInviteMario();
		break;
	case EM_DOING_UNK18:
	case EM_DOING_UNK1A:
		emKeepStay();
		break;
	case EM_DOING_GATE_DRAWING:
		emGateDrawing();
		break;
	case EM_DOING_GET_PAD:
		emGetPad();
		break;
	default:
		return;
	}
}

void TEnemyMario::considerAfter()
{
	if (mEMDoing == EM_DOING_REPLAY)
		emReplay();

	if (mEMDoing == EM_DOING_REPLAY_RUN_AWAY)
		emReplayRunAway();

	if (mEMDoing == EM_DOING_REPLAY_RUN_AWAY_TO_GATE)
		emReplayRunAwayToGate();

	if (mEMDoing == EM_DOING_REPLAY_TO_GOAL)
		emReplayToGoal();
}

void TEnemyMario::hitWater(THitActor* sender)
{
	if (mSpecialModel != nullptr)
		return;

	if (mSettingParams->mInvincibleFlag.get())
		return;

	switch (mEMDoing) {
	case EM_DOING_REPLAY:
	case EM_DOING_REPLAY_WAITING:
	case EM_DOING_REPLAY_JUMP_TO_NEAREST_NODE:
		mWaterHitTimer = 600;
		if (mWaterCounter > 0) {
			--mWaterCounter;
			gpMarioParticleManager->emit(PARTICLE_MS_ENM_WATHIT,
			                             &sender->mPosition, 0, nullptr);
			SMSGetMSound()->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK,
			                              &sender->mPosition, 0, 0.0f, 0, 0, 4);
			mWaterEffectTimer = mWaterEffectTimerMax;

			if (mEMDoing == EM_DOING_REPLAY_WAITING) {
				sleepingEffectKill();
				emReplayWaitingToReplayJumpToNearestNode();
			}
			break;
		}

		if (mStatus == MARIO_STATUS_RUN && canSleep()) {
			if (mHeldObject != nullptr) {
				((TLiveActor*)mHeldObject)->offLiveFlag(LIVE_FLAG_UNK100000);
				dropObject();
			}
			changeEMDoing(EM_DOING_PRE_DOWN_ANIMATION);
		}
		break;
		// NOTE: insane but matches
	case EM_DOING_PRE_DOWN_ANIMATION:
		return;
	case EM_DOING_DOWN_ANIMATION:
		return;
	case EM_DOING_RUN_AWAY_TO_NEAREST_NODE:
		return;
	case EM_DOING_REPLAY_RUN_AWAY:
		return;
	case EM_DOING_UNK12:
		return;
	case EM_DOING_UNK13:
		return;
	case EM_DOING_WAITING_TO_INVITE_MARIO:
		return;
	case EM_DOING_REPLAY_RUN_AWAY_TO_GATE:
		return;
	case EM_DOING_WAITING_MARIO:
		return;
	case EM_DOING_GATE_DRAWING:
		return;
	case EM_DOING_UNK18:
		return;
	case EM_DOING_REPLAY_TO_GOAL:
		return;
	}
}

u8 TEnemyMario::thinkTrample()
{
	if (mSpecialModel != nullptr)
		return FALSE;

	switch (mEMDoing) {
	case EM_DOING_DOWN_ANIMATION:
		--mTrampleCount;
		if (mTrampleCount > 0)
			changeEMDoing(EM_DOING_REPLAY_JUMP_TO_NEAREST_NODE);
		return TRUE;

	default:
		return FALSE;
	}
}

void TEnemyMario::reachGoal()
{
	onEMFlag(EM_FLAG_GOAL_REACHED);
	changeEMDoing(EM_DOING_WAITING_MARIO);
}

void TEnemyMario::checkReturn()
{
	if (!mGroundPlane->checkFlag(BG_CHECK_FLAG_ILLEGAL))
		return;

	int nodeIndex
	    = mEMario->getTracer()->getGraph()->findNearestNodeIndex(mPosition, -1);
	BOOL searching = true;
	while (searching) {
		JGeometry::TVec3<f32> point;
		mEMario->getTracer()->getGraph()->getGraphNode(nodeIndex).getPoint(
		    &point);

		if (point.distance(SMS_GetMarioPos()) > 1000.0f) {
			searching = false;
			mPosition = point;
		}

		nodeIndex
		    = (nodeIndex + 1) % mEMario->getTracer()->getGraph()->getNodeNum();
	}
}

void TEnemyMario::checkController(JDrama::TGraphics*)
{
	f32 dx           = gpMarioPos->x - mPosition.x;
	f32 dz           = gpMarioPos->z - mPosition.z;
	mAngleToMario    = matan(dz, dx);
	mDistanceToMario = std::sqrtf(dx * dx + dz * dz);

	u32 previousInput   = unk108->mInput;
	unk108->mStickHS16  = 0;
	unk108->mStickVS16  = 0;
	unk108->mInput      = 0;
	unk108->mFrameInput = 0;
	unk108->mAnalogRU8  = 0;
	unk108->mAnalogLU8  = 0;
	consider();

	unk108->mStickH = 0.0f;
	unk108->mStickV = 0.0f;
	if (unk108->mStickHS16 < -7)
		unk108->mStickH = unk108->mStickHS16 + 6;

	if (unk108->mStickHS16 > 7)
		unk108->mStickH = unk108->mStickHS16 - 6;

	if (unk108->mStickVS16 < -7)
		unk108->mStickV = unk108->mStickVS16 + 6;

	if (unk108->mStickVS16 > 7)
		unk108->mStickV = unk108->mStickVS16 - 6;

	unk108->mStickDist = std::sqrtf(unk108->mStickH * unk108->mStickH
	                                + unk108->mStickV * unk108->mStickV);
	if (unk108->mStickDist > 64.0f) {
		unk108->mStickH *= 64.0f / unk108->mStickDist;
		unk108->mStickV *= 64.0f / unk108->mStickDist;
		unk108->mStickDist = 64.0f;
	}
	unk108->mFrameInput = unk108->mInput & (unk108->mInput ^ previousInput);

	f32 stickRatio = unk108->mStickDist * (1.0f / 64.0f);
	mIntendedMag   = 64.0f * (stickRatio * stickRatio) * 0.5f;
	if (mIntendedMag > 0.0f)
		mIntendedYaw = matan(-unk108->mStickV, unk108->mStickH);
	else
		mIntendedYaw = mFaceAngle.y;

	considerAfter();

	if (mIntendedMag > 0.0f)
		mInput |= 1;

	if (unk108->isAHit())
		mInput |= 2;

	if (unk108->isAPressed())
		mInput |= 0x80;

	if (unk108->isBPressed())
		mInput |= 0x4000;

	if (unk108->isBHit())
		mInput |= 0x8000;
}

void TEnemyMario::playerControl(JDrama::TGraphics* graphics)
{
	unk9C  = mFaceAngle.y;
	unk29C = mPosition;
	offUnk114(UNK114_FLAG_UNK8);
	checkPlayerAction(graphics);
	stateMachine();
	stateMachineUpper();
	thinkSituation();
	thinkWaterSurface();
	thinkSand();
	thinkHeight();
	thinkParams();
	checkRideReCalc();
	checkWet();
}

void TEnemyMario::damageExec(THitActor*, int, int, int, f32, int, f32, s16) { }

void TEnemyMario::drawHPMeter(MtxPtr viewMtx)
{
	JGeometry::TVec3<f32> worldPosition = mPosition;
	worldPosition.y += 210.0f;
	JGeometry::TVec3<f32> screenPosition;
	MTXMultVec(viewMtx, &worldPosition, &screenPosition);

	TPosition3f identity;
	MTXIdentity(identity);
	GXSetCurrentMtx(GX_PNMTX0);
	GXLoadPosMtxImm(identity, GX_PNMTX0);
	GXLoadNrmMtxImm(identity, GX_PNMTX0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL,
	              GX_DF_NONE, GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL,
	              GX_DF_NONE, GX_AF_NONE);
	GXSetNumTexGens(0);
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
	GXSetCullMode(GX_CULL_NONE);

	f32 top          = screenPosition.y - 10.0f;
	f32 bottom       = screenPosition.y + 10.0f;
	f32 left         = screenPosition.x - 48.0f;
	f32 borderLeft   = left - 5.0f;
	f32 borderRight  = left + 96.0f + 5.0f;
	f32 borderTop    = top - 5.0f;
	f32 borderBottom = bottom + 5.0f;
	GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0, 0, 0, 0xC0 });
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(borderLeft, borderTop, screenPosition.z);
	GXPosition3f32(borderRight, borderTop, screenPosition.z);
	GXPosition3f32(borderRight, borderBottom, screenPosition.z);
	GXPosition3f32(borderLeft, borderBottom, screenPosition.z);
	GXEnd();

	f32 right = left + mWaterCounter * 1.5f;
	GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0x40, 0x40, 0xFF, 0xFF });
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(left, top, screenPosition.z);
	GXPosition3f32(right, top, screenPosition.z);
	GXPosition3f32(right, bottom, screenPosition.z);
	GXPosition3f32(left, bottom, screenPosition.z);
	GXEnd();
}

void TEnemyMario::perform(u32 cue, JDrama::TGraphics* graphics)
{
	MActor* emarioActor   = nullptr;
	J3DModel* emarioModel = nullptr;

	if (mSpecialModel == nullptr) {
		emarioActor = mEMario->getMActor();
		emarioModel = emarioActor->getModel();
	}

	if (mFreezeTimer > 0)
		--mFreezeTimer;

	if (mWaterHitTimer > 0)
		--mWaterHitTimer;

	if (cue & CUE_MOVE) {
		if (checkEMFlag(EM_FLAG_FORCE_WATER_HIT)) {
			mWaterCounter = 0;
			hitWater(this);
		}

		if (mStatus != MARIO_STATUS_RUN || mFreezeTimer == 0) {
			playerControl(graphics);
			setPositions();
		}
	}

	if (cue & CUE_MOVE) {
		if (mStatus != MARIO_STATUS_RUN || mFreezeTimer == 0)
			calcAnim(CUE_CALC_ANIM, graphics);

		if (mSpecialModel != nullptr) {
			for (u16 i = 0;
			     i < mModel->getModel()->getModelData()->getJointNum(); ++i) {
				mSpecialModel->setAnmMtx(i, mModel->getModel()->getAnmMtx(i));
			}
			mSpecialModel->calcWeightEnvelopeMtx();
		} else {
			emarioActor->calcAnm();
			for (u16 i = 0;
			     i < mModel->getModel()->getModelData()->getJointNum(); ++i) {
				emarioModel->setAnmMtx(i, mModel->getModel()->getAnmMtx(i));
			}
			emarioModel->calcWeightEnvelopeMtx();
			mBrushModel->setBaseTRMtx(emarioModel->getAnmMtx(mJointIdHandL));
			mBrushModel->calc();

			if (isDispStamp()) {
				TPosition3f trMtx;
				TRotation3f rotationMtx;
				MTXScale(trMtx, mBrushScaleupDuringDrawing,
				         mBrushScaleupDuringDrawing,
				         mBrushScaleupDuringDrawing);
				MsMtxSetRotRPH(rotationMtx, 0.0f, 180.0f, 0.0f);
				MTXConcat(mModel->getModel()->getBaseTRMtx(), trMtx, trMtx);
				MTXConcat(trMtx, rotationMtx, trMtx);
				mStampActor->getModel()->setBaseTRMtx(trMtx);
				mStampActor->calcAnm();
			}
		}

		mAttackRadius = 800.0f;
		mAttackHeight = 150.0f;
		mDamageRadius = 60.0f;
		mDamageHeight = 40.0f;
		calcEntryRadius();
		if (mWaterEffectTimer > 0) {
			--mWaterEffectTimer;
		} else {
			mWaterEffectTimer = 0;
		}
	}

	if (cue & CUE_CALC_VIEW) {
		calcView(graphics);
		if (checkEMFlag(EM_FLAG_DISP_PENCIL)) {
			unk390->entryDrawShadow();
			gpQuestionManager->request(mPosition, 50.0f);
		}

		if (mSpecialModel != nullptr) {
			mSpecialModel->viewCalc();
		} else {
			emarioActor->viewCalc();
			mBrushModel->viewCalc();
			if (isDispStamp()) {
				mStampActor->viewCalc();
			}
		}
	}

	if (cue & CUE_ENTRY) {
		if (mSpecialModel == nullptr) {
			if (mWaterEffectTimer > 0) {
				SMS_AddDamageFogEffect(
				    mEMario->getMActor()->getModel()->getModelData(), mPosition,
				    graphics);
			} else {
				SMS_ResetDamageFogEffect(
				    mEMario->getMActor()->getModel()->getModelData());
			}
			if (isDispStamp()) {
				gpPollution->stampModel(mStampActor->getModel());
			}
		}

		if (checkEMFlag(EM_FLAG_DISP_PENCIL)) {
			if (mSpecialModel == nullptr) {
				emarioActor->entry();
				mBrushModel->entry();
			} else {
				mSpecialModel->entry();
			}
		}
	}

	if (cue & CUE_DRAW) {
		if (mTrembleModelEffect != nullptr) {
			mTrembleModelEffect->movement();
		}

		BOOL doDraw = TRUE;
		if (!(unk114 & UNK114_FLAG_VISIBLE))
			doDraw = FALSE;

		if (mInvincibilityFrames > 0 && !(mInvincibilityFrames & 4))
			doDraw = FALSE;

		if (checkFlag(MARIO_FLAG_UNK4))
			doDraw = FALSE;

		if (mEMDoing == EM_DOING_HIDE)
			doDraw = FALSE;

		if (mFreezeTimer > 0 && !(mFreezeTimer & 4))
			doDraw = FALSE;

		if (doDraw == TRUE && mTrembleModelEffect != nullptr) {
			j3dSys.setUnk4C(7);
			unk394->draw();
			unk398->draw();
		}

		if ((mEMFlags & EM_FLAG_DISP_HP_METER) && mWaterHitTimer > 0) {
			drawHPMeter(graphics->getViewMtx());
		}
	}
}

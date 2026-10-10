#include <NPC/NpcBase.hpp>
#include <JSystem/J3D/J3DGraphBase/Components/J3DGXColorS10.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <System/MarDirector.hpp>
#include <System/FlagManager.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <MarioUtil/ModelUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/LightUtil.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/Graph.hpp>
#include <Map/Map.hpp>
#include <Camera/cameralib.hpp>
#include <NPC/NpcParts.hpp>
#include <NPC/NpcColor.hpp>
#include <NPC/NpcThrow.hpp>
#include <NPC/NpcCoin.hpp>
#include <NPC/NpcInitAnmData.hpp>
#include <NPC/NpcInbetween.hpp>
#include <NPC/NpcInitData.hpp>
#include <NPC/NpcNerve.hpp>

// rogue
#include <M3DUtil/InfectiousStrings.hpp>
static const char dummy5[] = "__ROOT_JOINT__";

static const char* cManiyaParentViewObjName    = "マニ屋親タヌキ";
static const char* cManiyaChildViewObjName     = "マニ屋子タヌキ";
static const char* cNotUseFastCubeViewObjName0 = "モンテ26";
static const char* cNotUseFastCubeViewObjName1 = "モンテ27";
static const char* cEyeMaterialName            = "_eye_mat";
static const char* cFruitsBoatRideJointName    = "monte_koko";
static const char* cNeckJointName              = "kubi";

void TBaseNPC::initNpcLight_()
{
	mMActor->setLightType(LIGHT_TYPE_OBJECT);
	if (checkLiveFlag(LIVE_FLAG_UNK10))
		mGroundHeight = gpMap->checkGroundIgnoreWaterSurface(
		    mPosition.x, mPosition.y + 10.0f, mPosition.z, &mGroundPlane);
}

// NOTE: this is not present as an UNUSED symbol, so it was an inline, but it is
// sure as hell isn't used anywhere but here so I'm not defining it in the
// header and putting it here instead.
inline void TBaseNPC::setMtxEffect_()
{
	static const char* sWaistJointName[] = { "koshi_null" };

	struct TMtxEffectInitInfo {
		/* 0x0 */ u32 mActorType;
		/* 0x4 */ const char** unk4;
		/* 0x8 */ const char* unk8;
		/* 0xC */ u8 unkC;
		/* 0xD */ u8 unkD;
	};

	static const TMtxEffectInitInfo sMtxEffectInitData[] = {
		{ ACTOR_TYPE_NPC_MONTE_M, sWaistJointName, "Npc/MonteM", 0, 1 },
		{ ACTOR_TYPE_NPC_MONTE_MA, sWaistJointName, "Npc/MonteM", 0, 1 },
		{ ACTOR_TYPE_NPC_MONTE_MB, sWaistJointName, "Npc/MonteM", 0, 1 },
		{ ACTOR_TYPE_NPC_MONTE_MC, sWaistJointName, "Npc/MonteM", 0, 1 },
		{ ACTOR_TYPE_NPC_MONTE_MD, sWaistJointName, "Npc/MonteM", 0, 1 },
		{ ACTOR_TYPE_NPC_MONTE_MF, sWaistJointName, "Npc/MonteM", 0, 1 },
		{ ACTOR_TYPE_NPC_MONTE_MG, sWaistJointName, "Npc/MonteM", 0, 1 },
		{ ACTOR_TYPE_NPC_MONTE_MH, sWaistJointName, "Npc/MonteM", 0, 1 },
		{ ACTOR_TYPE_NPC_MONTE_W, sWaistJointName, "Npc/MonteW", 0, 1 },
		{ ACTOR_TYPE_NPC_MONTE_WA, sWaistJointName, "Npc/MonteW", 0, 1 },
		{ ACTOR_TYPE_NPC_MONTE_WB, sWaistJointName, "Npc/MonteW", 0, 1 },
		{ ACTOR_TYPE_NPC_MONTE_WC, sWaistJointName, "Npc/MonteW", 0, 1 },
		{ ACTOR_TYPE_NPC_PEACH, sWaistJointName, "Npc/Peach", 0, 1 },
		{ 0, nullptr, nullptr, 0, 0 },
	};

	const TMtxEffectInitInfo* pInfo = sMtxEffectInitData;
	for (;;) {
		if (pInfo->mActorType == 0)
			return;
		if (pInfo->mActorType == mActorType)
			break;
		pInfo += 1;
	}

	mMultiMtxEffect            = new TMultiMtxEffect;
	mMultiMtxEffect->mNumBones = pInfo->unkD;

	u16* boneIds       = new u16[pInfo->unkD];
	u8* mtxEffectTypes = new u8[pInfo->unkD];

	JUTNameTab* jointNameTab = getModel()->getModelData()->getJointName();
	for (int i = 0; i < pInfo->unkD; ++i) {
		boneIds[i]        = jointNameTab->getIndex(pInfo->unk4[i]);
		mtxEffectTypes[i] = pInfo->unkC;
	}

	mMultiMtxEffect->mBoneIDs       = boneIds;
	mMultiMtxEffect->mMtxEffectType = mtxEffectTypes;

	mMultiMtxEffect->setup(getModel(), pInfo->unk8);
	mMultiMtxEffect->flagOn(0x1);
}

// NOTE: same as above
inline void TBaseNPC::initSinkNpc_()
{
	static int sCheckPollutedStartCounter = 0;

	int iVar4 = CLBPalFrame(30);

	unk15C = new TNpcSink(sCheckPollutedStartCounter, iVar4);

	sCheckPollutedStartCounter += 1;
	if (sCheckPollutedStartCounter >= iVar4)
		sCheckPollutedStartCounter = 0;
}

// TODO: nonmatching stack frame is 0x120 rather than 0x168.
void TBaseNPC::init(TLiveManager* param_1)
{
	int iVar18 = getActorType() - ACTOR_TYPE_NPC_MONTE_M;

	mIndividualParams = gpConductor->getNpcParams()->unk8[iVar18];

	if (param_1 == nullptr) {
		onLiveFlag(LIVE_FLAG_DEAD);
		initHitActor(mActorType, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f);
		onHitFilter(HIT_FILTER_NO_COLLISION);
		mSpine->initWith(&TNerveNPCWaitMarioApproach::theNerve());
		mTurnSpeed = mIndividualParams->mWaitTurnSpeed.get();
		gpConductor->registerAloneActor(this);
		return;
	}

	const TNpcInitInfo* pTVar3 = SMSGetNpcInitData(iVar18);
	mManager                   = param_1;
	param_1->manageActor(this);

	mMActorKeeper = new TMActorKeeper(param_1);
	u32 uVar21    = 0;
	if (getActorType() == ACTOR_TYPE_NPC_BOARD)
		uVar21 = 3;
	mMActorKeeper->createMActorFromNthData(0, uVar21);
	if (param_1->getModelDataNum() == 2)
		mMActorKeeper->createMActorFromNthData(1, 3);
	mMActor = mMActorKeeper->getMActor(0);

	mBodyScale        = 1.0f;
	mBodyRadius       = 10.0f;
	mMarchSpeed       = 0.0f;
	mWallRadius       = pTVar3->mAttackRadius * mScaling.x;
	mHeadHeight       = mPtrSaveNormal->mSLHeadHeightNormal.get();
	mGravity          = mPtrSaveNormal->mGravityY.get();
	mScaledBodyRadius = mScaling.x * mIndividualParams->mCircleShadowSize.get();

	if (getActorType() == ACTOR_TYPE_NPC_BOARD) {
		onLiveFlag(LIVE_FLAG_UNK2000 | LIVE_FLAG_UNK10 | LIVE_FLAG_UNK8);
		initNpcObjCollision_(pTVar3);
		mSpine->initWith(&TNerveNPCWaitMarioApproach::theNerve());
		mTurnSpeed = 0.0f;
		initNpcLight_();
		return;
	}

	onLiveFlag(LIVE_FLAG_UNK1000000);
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	onLiveFlag(LIVE_FLAG_UNK1000);
	if (SMSGetMarDirector()->mMap != 8
	    || (strcmp(mName, cNotUseFastCubeViewObjName0) != 0
	        && strcmp(mName, cNotUseFastCubeViewObjName1) != 0)) {
		onLiveFlag(LIVE_FLAG_UNK2000);
	}

	if (getActorType() == ACTOR_TYPE_NPC_MONTE_ME)
		onLiveFlag(LIVE_FLAG_UNK10 | LIVE_FLAG_UNK8);

	if (isJellyFishMare() || isSunflower()
	    || getActorType() == ACTOR_TYPE_NPC_MONTE_MF) {
		onLiveFlag(LIVE_FLAG_UNK10);
	}

	if (mMActor->getAnmBck())
		mMActor->getAnmBck()->initNormalMotionBlend();

	if (isPollutionNpc())
		initSinkNpc_();

	unk22C = new TNpcUnk22CStruct;

	if (unk124->getGraph()->isDummy()) {
		mSpine->initWith(&TNerveNPCWaitMarioApproach::theNerve());
	} else {
		mSpine->initWith(&TNerveNPCGraphWander::theNerve());
		unk124->reset();
		goToShortestNextGraphNode();
	}

	initNpcObjCollision_(pTVar3);
	setMtxEffect_();

	JUTNameTab* jointNameTab = getModel()->getModelData()->getJointName();
	mNeckJointIndex          = jointNameTab->getIndex(cNeckJointName);

	if (mNeckJointIndex != -1) {
		unk230 = new TNpcUnk230Struct;
		extern int NPCNeckCallBack(J3DNode * param_1, int param_2);
		getMActor()->setJointCallback(mNeckJointIndex, &NPCNeckCallBack);
	}

	setHappyEffectMtxPtr_(jointNameTab);
	switch (mActorType) {
	case ACTOR_TYPE_NPC_MARE_MD:
	case ACTOR_TYPE_NPC_MONTE_MH:
		setNoteEffectMtxPtr_(jointNameTab);
		break;
	}
	if (isPollutionNpc())
		setPollutionEffectMtxPtr_(jointNameTab);

	if (pTVar3->unk0 != nullptr) {
		const char* modelName = pTVar3->unk0->unk0;
		unk150                = SMS_CreateMinimumSDLModel(modelName);
		u16 idx = unk150->getModelData()->getJointName()->getIndex(
		    pTVar3->unk0->unk4);
		unk154 = unk150->getAnmMtx(idx);
	}

	initAnmSound();
	mInbetweenCtrl = new TNpcInbetween(
	    CLBPalFrame(mPtrSaveNormal->mPosInbetweenFrame.get()),
	    CLBPalFrame(mPtrSaveNormal->mMotionBlendFrame.get()));
	const TNpcInitAnmInfo* anmInitInfo = SMSGetNpcInitAnmData(iVar18);
	initLodAnm(anmInitInfo->unk0, 0, mIndividualParams->mLodChangeDist.get());
	initNpcLight_();
}

// NOTE: see above for inline motivation
inline void TBaseNPC::initBaseActionFlag_()
{
	static const TAnmBckMapping sIndividualHoldArrowBck[] = {
		{ 14, 16 },
		{ -1, -1 },
	};

	static const TAnmBckMapping sIndividualKinopioBck[] = {
		{ 15, 16 }, { 6, 7 },   { 4, 5 },   { 14, 24 },
		{ 9, 11 },  { 19, 20 }, { -1, -1 },
	};
	static const TAnmBtpMapping sIndividualKinopioBtp[] = {
		{ 1, 2 },
		{ 0, 5 },
		{ -1, -1 },
	};

	static const TAnmBckMapping sIndividualKinojiiBck[] = {
		{ 11, 12 }, { 2, 3 }, { 10, 17 }, { 5, 7 }, { 13, 14 }, { -1, -1 },
	};
	static const TAnmBtpMapping sIndividualKinojiiBtp[] = {
		{ 1, 2 },
		{ 0, 5 },
		{ -1, -1 },
	};

	if (isMonte()) {
		setMonteActionFlag_();
		if (checkActionFlag(NPC_ACTION_UNK400))
			getLodAnm()->setIndividualBck(sIndividualHoldArrowBck);
	} else if (isMare()) {
		setMareActionFlag_();
	} else if (getActorType() == ACTOR_TYPE_NPC_KINOPIO
	           || getActorType() == ACTOR_TYPE_NPC_KINOJII) {
		setKinoActionFlag_();
		if (checkActionFlag(NPC_ACTION_UNK100)) {
			switch (getActorType()) {
			case ACTOR_TYPE_NPC_KINOPIO:
				getLodAnm()->setIndividualBck(sIndividualKinopioBck);
				getLodAnm()->setIndividualBtp(sIndividualKinopioBtp);
				break;
			case ACTOR_TYPE_NPC_KINOJII:
				getLodAnm()->setIndividualBck(sIndividualKinojiiBck);
				getLodAnm()->setIndividualBtp(sIndividualKinojiiBtp);
				break;
			}
		}
	} else {
		unk16C      = 0;
		mActionFlag = 0;
	}
}

// NOTE: see above for inline motivation
inline void TBaseNPC::initIndividualAnm_()
{
	static const TAnmBckMapping sIndividualParentRaccoonDogAnmBck[] = {
		{ 0, 1 },
		{ -1, -1 },
	};
	static const TAnmBckMapping sIndividualChildRaccoonDogAnmBck[] = {
		{ 0, 2 },
		{ -1, -1 },
	};

	static const TAnmBckMapping sIndividualMareMA0Bck[] = {
		{ 0, 3 },
		{ -1, -1 },
	};
	static const TAnmBtpMapping sIndividualMareMA0Btp[] = {
		{ 2, 3 },
		{ -1, -1 },
	};

	static const TAnmBckMapping sIndividualMareMA1Bck[] = {
		{ 0, 4 },
		{ -1, -1 },
	};
	static const TAnmBtpMapping sIndividualMareMA1Btp[] = {
		{ 2, 0 },
		{ -1, -1 },
	};

	// BUG: missing -1 terminator
	static const TAnmBckMapping sIndividualMareWA0Bck[] = {
		{ 0, 3 },
	};

	switch (getActorType()) {
	case ACTOR_TYPE_NPC_RACCOON_DOG:
		if (strcmp(mName, cManiyaParentViewObjName) == 0) {
			onActionFlag(NPC_ACTION_UNK800);
			getLodAnm()->setIndividualBck(sIndividualParentRaccoonDogAnmBck);
		} else if (strcmp(mName, cManiyaChildViewObjName) == 0) {
			onActionFlag(NPC_ACTION_UNK800);
			onLiveFlag(LIVE_FLAG_DONT_TALK);
			getLodAnm()->setIndividualBck(sIndividualChildRaccoonDogAnmBck);
		}
		break;

	case ACTOR_TYPE_NPC_MARE_MA:
		switch ((int)(MsRandF() * 3.0f)) {
		case 0:
			getLodAnm()->setIndividualBck(sIndividualMareMA0Bck);
			getLodAnm()->setIndividualBtp(sIndividualMareMA0Btp);
			break;
		case 1:
			getLodAnm()->setIndividualBck(sIndividualMareMA1Bck);
			getLodAnm()->setIndividualBtp(sIndividualMareMA1Btp);
			break;
		}
		break;

	case ACTOR_TYPE_NPC_MARE_WA:
		switch ((int)(MsRandF() * 2.0f)) {
		case 0:
			getLodAnm()->setIndividualBck(sIndividualMareWA0Bck);
			break;
		}
		break;
	}
}

// TODO: fabricated boundary; matches the material-loop register allocation.
// The original helper name and signature are not known.
static inline void initNpcPollutionPackets(J3DModel* model,
                                           const GXColor* color)
{
	J3DModelData* modelData = model->getModelData();
	JUTNameTab* matNameTab  = modelData->getMaterialName();
	for (u16 i = 0, e = modelData->getMaterialNum(); i < e; ++i) {
		const char* name = matNameTab->getName(i);
		if (strcmp(name, cEyeMaterialName) != 0) {
			int shapeIdx
			    = modelData->getMaterialNodePointer(i)->getShape()->getIndex();
			if (model->getShapePacket(shapeIdx)->getUserArea() == 0)
				SMS_InitPacket_OneTevKColor(model, i, GX_KCOLOR0, color);
		}
	}
}

// TODO: nonmatching color-loop registers and stack frame (0x200 vs. 0x1f0).
void TBaseNPC::setIndividualDifference_(JSUMemoryInputStream& stream)
{
	int iVar15                   = getActorType() - ACTOR_TYPE_NPC_MONTE_M;
	const TNpcInitInfo* initInfo = SMSGetNpcInitData(iVar15);
	const TNpcInitAnmInfo* anmInitInfo = SMSGetNpcInitAnmData(iVar15);

	J3DGXColorS10 local_78[2];
	for (int i = 0; i < 2; ++i) {
		local_78[i].color.r = stream.readS32();
		local_78[i].color.g = stream.readS32();
		local_78[i].color.b = stream.readS32();
		local_78[i].color.a = 0xff;
	}

	if (isPollutionNpc()) {
		mPollutionAmount = local_78[0].color.b * (1.0f / 255);
		unk174.a = mPollutionAmount * mIndividualParams->mPollutionMax.get();
	}

	int uVar21 = stream.readS32();
	unk16C     = stream.readS32();
	int local_a8;
	int uVar20   = stream.readS32();
	f32 local_a0 = stream.readS32();
	f32 local_a4 = stream.readS32();
	local_a8     = stream.readU32();

	if (uVar21 < 0)
		uVar21 = 0;
	if (uVar20 < 0)
		uVar20 = 0;

	{
		s16* indices         = &local_78[0].color.r;
		int uVar16           = getManager()->getModelDataNum();
		const GXColor* color = getPtrInitPollutionColor();
		for (int i = 0; i < uVar16; ++i) {
			for (int j = 0; j < 2; ++j) {
				const TColorChangeInfo* const* row = initInfo->unk34[j];
				s16 tmp                            = indices[j];
				if (initInfo->unk34[j][i]) {
					const TColorChangeInfo* ccInfo = row[i];
					SMS_InitChangeNpcColor(getActorKeeper()->getMActor(i),
					                       ccInfo, tmp, color);
				}
			}
		}
	}

	if (getPtrInitPollutionColor())
		initNpcPollutionPackets(getModel(), &unk174);

	if (getActorType() == ACTOR_TYPE_NPC_PEACH) {
		uVar21 |= 0x6;
		if (uVar21 & 0x10) {
			uVar21 |= 0x60;
			peachParasolIn_();
		} else {
			uVar21 &= ~0x60;
		}
	}

	if (TFlagManager::getInstance()->getBool(MSF_SUNFLOWERS_LEFT_TO_RESCUE)
	    && isSunflower())
		sunflowerDownIn_();

	if (uVar21 > 0)
		unk168 = new TNpcParts(uVar21, &local_78[1], this);

	if (anmInitInfo->unk4 != nullptr)
		getLodAnm()->setIndividualBtp(anmInitInfo->unk4);

	initBaseActionFlag_();
	initIndividualAnm_();

	if (uVar20 & 0x1)
		mThrowCtrl = new TNpcThrow(local_a0, local_a4);

	bool bVar4 = isNormalMonteM() && checkActionFlag(NPC_ACTION_BURNING)
	                 ? true
	                 : false;

	bool bVar3 = true;
	if (local_a8 == 2000 || local_a8 == 200
	    || (local_a8 >= 0 && local_a8 < 50)) {
		bool bVar5 = local_a8 >= 0 && local_a8 < 50
		                     && TFlagManager::getInstance()->getBlueCoinFlag(
		                         gpApplication.mCurrArea.getStage(), local_a8)
		                 ? true
		                 : false;

		if (bVar4 && bVar5) {
			bVar3 = false;
			offActionFlag(NPC_ACTION_BURNING | NPC_ACTION_UNK80
			              | NPC_ACTION_RUN);
		} else {
			if (bVar5)
				local_a8 = 2000;
			mCoinCtrl = new TNpcCoin(local_a8);
		}
	}

	if (bVar4 && bVar3) {
		bool b = (uVar21 & NPC_ACTION_UNK800) != 0;
		setSmokeEffectMtxPtr_(b);
	}

	npcWaitIn();
	randomizeBckAndBtpFrame_();

	f32 fVar1 = mMActor->getFrameCtrl(ANM_TYPE_BCK)->getFrame();
	if (unk168 != nullptr) {
		if (isJellyFishMare())
			unk168->addJellyFishParts(fVar1);

		unk168->setPartsAnmFrame(fVar1);
	}
}

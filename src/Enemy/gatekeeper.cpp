#include <Enemy/GateKeeper.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DCluster.hpp>
#include <System/Particles.hpp>
#include <System/MarDirector.hpp>
#include <System/FlagManager.hpp>
#include <System/EmitterViewObj.hpp>
#include <Strategic/Strategy.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/MSound.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/ModelUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <Player/MarioAccess.hpp>
#include <Map/PollutionManager.hpp>
#include <M3DUtil/MActorData.hpp>
#include <M3DUtil/MActor.hpp>
#include <GC2D/GCConsole2.hpp>
#include <Enemy/NameKuri.hpp>
#include <Enemy/Igaiga.hpp>
#include <Enemy/Conductor.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

static const char* gatekeeper_bastable[] = {
	"/scene/gatekeeper/bas/gene_pakkun_appear1.bas",
	nullptr,
	nullptr,
	"/scene/gatekeeper/bas/gene_pakkun_damage1.bas",
	nullptr,
	"/scene/gatekeeper/bas/gene_pakkun_dead1.bas",
	"/scene/gatekeeper/bas/gene_pakkun_dive1.bas",
	"/scene/gatekeeper/bas/gene_pakkun_iyaiya1.bas",
	nullptr,
	nullptr,
	nullptr,
	"/scene/gatekeeper/bas/gene_pakkun_wait2.bas",
	nullptr,
	nullptr,
	"/scene/gatekeeper/bas/gene_pakkun_wait2_loop.bas",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
};

TGateKeeperParams::TGateKeeperParams(const char* path)
    : TSpineEnemyParams(path)
    , PARAM_INIT(mSLDiveTimer, 480)
    , PARAM_INIT(mSLLoop2Dive, 5)
    , PARAM_INIT(mSLLaunchTimerNormal, 600)
    , PARAM_INIT(mSLLaunchTimerDamage, 600)
{
	TParams::load(mPrmPath);
}

TGKHitObj::TGKHitObj(TGateKeeperBase* owner, int joint_idx, const char* name)
    : THitActor(name)
    , mOwner(owner)
    , mJointIndex(joint_idx)
    , mVulnerable(0)
{
	initHitActor(0x10000022, 1, 0x80000000, 0.0f, 0.0f, 150.0f, 200.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);
	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(this);
}

BOOL TGKHitObj::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->getActorType() == 0x1000001
	    && message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		if (mVulnerable)
			mOwner->unk154++;
		gpMarioParticleManager->emit(PARTICLE_MS_ENM_WATHIT, &sender->mPosition,
		                             0, nullptr);
		SMSGetMSound()->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK,
		                              &sender->mPosition, 0, 0.0f, 0, 0, 4);
		return true;
	}
	return mOwner->receiveMessage(sender, message);
}

void TGKHitObj::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (!mOwner->checkLiveFlag(LIVE_FLAG_DEAD)) {
		if (param_1 & 2) {
			if (!mOwner->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
				MtxPtr mtx
				    = mOwner->getMActor()->getModel()->getAnmMtx(mJointIndex);
				mPosition.set(mtx[0][3], mtx[1][3], mtx[2][3]);
			} else {
				mPosition = mOwner->mPosition;
			}
		}
		THitActor::perform(param_1, param_2);
	}
}

TGateKeeperBase::TGateKeeperBase(const char* name)
    : TSpineEnemy(name)
{
	mStampModel   = nullptr;
	unk154        = 0;
	unk158        = 0.0f;
	mMultiBtk     = nullptr;
	mStampEnabled = TRUE;
	mVulnerable   = 0;
	mStampScale.set(1.0f, 1.0f, 1.0f);
	onLiveFlag(LIVE_FLAG_UNK10);
}

void TGateKeeperBase::kill() { onLiveFlag(LIVE_FLAG_DEAD); }

BOOL TGateKeeperBase::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->getActorType() == 0x1000001) {
		if (mVulnerable && message == HIT_MESSAGE_SPRAYED_BY_WATER)
			unk154++;
		gpMarioParticleManager->emit(PARTICLE_MS_ENM_WATHIT, &sender->mPosition,
		                             0, nullptr);
		SMSGetMSound()->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK,
		                              &sender->mPosition, 0, 0.0f, 0, 0, 4);
		return true;
	}
	return false;
}

void TGateKeeperBase::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (!checkLiveFlag(LIVE_FLAG_DEAD)) {
		if (param_1 & 2)
			mMultiBtk->update();

		if (param_1 & 1) {
			f32 ratio = mMActor->getMotionBlendRatioForBck();
			ratio -= unk158;
			if (ratio < 0.0f)
				ratio = 0.0f;
			mMActor->setMotionBlendRatioForBck(ratio);
		}

		TSpineEnemy::perform(param_1, param_2);

		if (mStampEnabled && mStampModel) {
			if (param_1 & 2) {
				MtxPtr rootMtx = getModel()->getBaseTRMtx();
				mStampModel->getModel()->setBaseScale(mStampScale);
				mStampModel->getModel()->setBaseTRMtx(rootMtx);
				mStampModel->calcAnm();
			}
			if (param_1 & 0x200)
				gpPollution->stampModel(mStampModel->getModel());
		}

		if (param_1 & 1)
			unk154 = 0;
	}
}

TBiancoGateKeeperManager::TBiancoGateKeeperManager(const char* name)
    : TEnemyManager(name)
{
}

void TBiancoGateKeeperManager::initJParticle()
{
	SMS_LoadParticle("/scene/gatekeeper/jpa/ms_gkpa_dead.jpa", 0xA7);
	SMS_LoadParticle("/scene/gatekeeper/jpa/ms_gkpa_deadsmoke.jpa", 0xA8);
	SMS_LoadParticle("/scene/gatekeeper/jpa/ms_gkpa_bota.jpa", 0x140);
	SMS_LoadParticle("/scene/gatekeeper/jpa/ms_gkpa_yodare_s.jpa", 0x141);
	SMS_LoadParticle("/scene/gatekeeper/jpa/ms_gkpa_yodare_l.jpa", 0x142);
	SMS_LoadParticle("/scene/gatekeeper/jpa/ms_gkpa_kemuri.jpa", 0x1DF);
	SMS_LoadParticle("/scene/gatekeeper/jpa/ms_gkpa_kemuri_w.jpa", 0x1E0);
}

void TBiancoGateKeeperManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TGateKeeperParams("/enemy/gatekeeper.prm");
	TEnemyManager::load(stream);
	initJParticle();
}

void TBiancoGateKeeperManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "gene_pakkun_model1.bmd", 0x11210000, 0 },
		{ "stamp_keeper_model1.bmd", 0x10210000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

TBGKMtxCalc::TBGKMtxCalc(TBiancoGateKeeper* owner)
    : M3UMtxCalcSIAnmBlendQuat(false)
    , mOwner(owner)
{
}

void TBGKMtxCalc::joinAnm(int param_1)
{
	J3DAnmTransformKey* anm
	    = mOwner->getActorKeeper()->getMActorAnmData()->getUnk2C()->getAnmPtr(
	        param_1);

	if (unk54 == anm)
		return;

	unk58 = unk54;
	unk54 = anm;
	unk50 = 1.0f;
}

void TBGKMtxCalc::setAnm(int param_1)
{
	J3DAnmTransformKey* anm
	    = mOwner->getActorKeeper()->getMActorAnmData()->getUnk2C()->getAnmPtr(
	        param_1);

	unk54 = anm;
	unk58 = 0;
	unk50 = 0.0f;
}

void TBGKMtxCalc::calc(u16 param_1)
{
	M3UMtxCalcSIAnmBlendQuat::calc(param_1);

	if (param_1 == 11) {
		MtxPtr mtx = mOwner->getMActor()->getModel()->getAnmMtx(param_1);
		MsMtxSetXYZRPH(mtx, mOwner->mPosition.x, mOwner->mPosition.y,
		               mOwner->mPosition.z, mOwner->mRotation.x,
		               mOwner->mRotation.y, mOwner->mRotation.z);
		MTXCopy(mtx, J3DSys::mCurrentMtx);
	} else if (param_1 == 0) {
		MtxPtr mtx = mOwner->getMActor()->getModel()->getAnmMtx(param_1);
		if (!gpMarDirector->isDemoModeNow()) {
			if (!gpMarDirector->isTalkModeNow()
			    && (mOwner->getMActor()->checkCurBckFromIndex(0xB)
			        || mOwner->getMActor()->checkCurBckFromIndex(0x12)
			        || mOwner->getMActor()->checkCurBckFromIndex(0xF)
			        || mOwner->getMActor()->checkCurBckFromIndex(0x10)
			        || mOwner->getMActor()->checkCurBckFromIndex(0xC))) {
				JGeometry::TVec3<f32> diff = SMS_GetMarioPos();
				diff -= mOwner->mPosition;

				f32 yaw2 = MsGetRotFromZaxisY(diff);
				f32 cur2 = mOwner->mRotation.y + mOwner->getUnk180();

				f32 yaw = MsWrap(yaw2, -180.0f, 180.0f);
				f32 cur = MsWrap(cur2, -180.0f, 180.0f);

				f32 delta = MsAngleDiff(yaw, cur);
				f32 turn;
				if (0.0f < delta)
					turn = 3.0f > delta ? delta : 3.0f;
				else
					turn = -3.0f > delta ? -3.0f : delta;

				f32 newYaw     = (turn + cur) - mOwner->mRotation.y;
				mOwner->unk180 = MsWrap(newYaw, 0.0f, 360.0f);
			}
		}

		f32 s = JMASin(mOwner->unk180);
		f32 c = JMACos(mOwner->unk180);
		Mtx rot;
		rot[0][0] = c;
		rot[0][1] = 0.0f;
		rot[0][2] = s;
		rot[0][3] = 0.0f;
		rot[1][0] = 0.0f;
		rot[1][1] = 1.0f;
		rot[1][2] = 0.0f;
		rot[1][3] = 0.0f;
		rot[2][0] = -s;
		rot[2][1] = 0.0f;
		rot[2][2] = c;
		rot[2][3] = 0.0f;
		MTXConcat(mtx, rot, mtx);
		MTXCopy(mtx, J3DSys::mCurrentMtx);
	}
}

TBGKObstacle::TBGKObstacle(TBiancoGateKeeper* owner, const char* name)
    : THitActor(name)
{
	mPosition = owner->mPosition;
	mPosition.y -= 1000.0f;
	initHitActor(0x10000022, 1, 0x80000000, 800.0f, 800.0f, 800.0f, 800.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);
	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(this);
}

TBiancoGateKeeper::TBiancoGateKeeper(const char* name)
    : TGateKeeperBase(name)
{
	unk170       = 0;
	mHead        = nullptr;
	unk178       = nullptr;
	unk17C       = 0;
	unk17E       = 0;
	unk180       = 0.0f;
	unk184       = 0;
	unk288       = 0;
	unk28A       = 0;
	mObstacle    = nullptr;
	unk290       = 0;
	mVariant     = VARIANT_DOLPIC_TOWN;
	mHintShown   = FALSE;
	mHintTimer   = 0;
	unk296       = 0;
	unk298       = 0;
	mRumblePower = 0.0f;
}

void TBiancoGateKeeper::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);
	mMActorKeeper = new TMActorKeeper(mManager, 3);
	mMActor       = mMActorKeeper->createMActor("gene_pakkun_model1.bmd", 3);

	if (strcmp(mName, "ゲートキーパー（リコ）") == 0) {
		mVariant = VARIANT_RICO_GATEKEEPER;
		if (!TFlagManager::smInstance->getBool(0x50001))
			onLiveFlag(LIVE_FLAG_DEAD);
	} else if (strcmp(mName, "ゲートキーパー（マンマ）") == 0) {
		mVariant = VARIANT_MAMMA_GATEKEEPER;
		if (!TFlagManager::smInstance->getBool(0x50002))
			onLiveFlag(LIVE_FLAG_DEAD);
	} else {
		u8 map = gpMarDirector->getCurrentMap();
		if (map == 0)
			mVariant = VARIANT_AIRPORT;
		else if (map == 1)
			mVariant = VARIANT_DOLPIC_TOWN;
		else
			mVariant = VARIANT_GENERIC;
	}

	if (gpMarDirector->getCurrentMap() == 1) {
		mStampModel = mMActorKeeper->createMActor("stamp_keeper_model1.bmd", 3);
		mStampModel->setBckFromIndex(0x13);
		if (mVariant == VARIANT_DOLPIC_TOWN)
			mStampScale.scale(0.8f);
		else if (mVariant == VARIANT_RICO_GATEKEEPER)
			mStampScale.scale(1.2f);
		else if (mVariant == VARIANT_MAMMA_GATEKEEPER)
			mStampScale.scale(1.4f);
	}

	initHitActor(0x10000022, 5, 0x81000000, 400.0f, 150.0f, 400.0f, 150.0f);
	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(this);
	offHitFlag(HIT_FLAG_NO_COLLISION);

	mSpine->initWith(&TNerveBGKSleep::theNerve());

	J3DModel* model = mMActor->getModel();
	if (model->getSkinDeform() == NULL)
		model->setSkinDeform(new J3DSkinDeform, (J3DDeformAttachFlag)1);

	unk178 = new TBGKMtxCalc(this);
	mMActor->setCalcForBck(unk178);
	mHitPoints = 3;
	mMActor->offMakeDL();

	MActorAnmData* anmData = mMActorKeeper->getMActorAnmData();
	mMultiBtk              = new TMultiBtk(2, getModel()->getModelData());
	for (int i = 0; i < 2; i++)
		mMultiBtk->setNthData(i, anmData->getUnk38()->getAnmPtr(i));

	mObstacle = new TBGKObstacle(this, "TBGKObstacle");

	ResTIMG* tex = (ResTIMG*)JKRFileLoader::getGlbResource(
	    "/scene/map/pollution/H_ma_rak.bti");
	if (tex != NULL)
		SMS_ChangeTextureAll(mMActor->getModel()->getModelData(),
		                     "Q_kepper_dummy_128IA4", *tex);

	initAnmSound();
	calcRootMatrix();

	mHead = new TGKHitObj(this, 0xA, "頭ヒット");

	mMActor->calc();
	if (mStampModel != NULL)
		mStampModel->calc();

	if (checkLiveFlag(LIVE_FLAG_DEAD)) {
		onHitFlag(HIT_FLAG_NO_COLLISION);
		mHead->onHitFlag(HIT_FLAG_NO_COLLISION);
		mObstacle->onHitFlag(HIT_FLAG_NO_COLLISION);
	}
}

void TBiancoGateKeeper::kill()
{
	onLiveFlag(LIVE_FLAG_DEAD);
	onHitFlag(HIT_FLAG_NO_COLLISION);
	mHead->onHitFlag(HIT_FLAG_NO_COLLISION);
	mObstacle->onHitFlag(HIT_FLAG_NO_COLLISION);
}

void TBiancoGateKeeper::changeBck(int param_1)
{
	int cur = mMActor->getCurAnmIdx(0);
	if ((cur == 0x11 && param_1 == 0xB) || (cur == 0x11 && param_1 == 0x12)
	    || (cur == 0xB && param_1 == 0x12) || (cur == 0x12 && param_1 == 0xB)
	    || (cur == 7 && param_1 == 7) || (cur == 7 && param_1 == 0x12)
	    || (cur == 0xF && param_1 == 0x10) || (cur == 0x10 && param_1 == 0xC)) {
		unk178->setAnm(param_1);
		mMActor->getAnmBck()->setFrameCtrl(param_1);
		unk158 = 0.0f;
	} else {
		unk178->joinAnm(param_1);
		mMActor->getAnmBck()->setFrameCtrl(param_1);
		J3DFrameCtrl* fc = mMActor->getFrameCtrl(0);
		if (fc != NULL) {
			f32 v = 0.25f * (f32)fc->getEnd();
			if (v < 1.0f)
				unk158 = 1.0f;
			else
				unk158 = 1.0f / v;
		} else {
			unk158 = 1.0f;
		}
	}

	const char** table = getBasNameTable();
	setAnmSound(table == nullptr ? nullptr : table[param_1]);
}

void TBiancoGateKeeper::launchGorogoro()
{
	TGorogoro* goro = (TGorogoro*)gpConductor->makeOneEnemyAppear(
	    mPosition, "ゴロゴロマネージャー", 0);
	if (goro)
		goro->generateByGateKeeper(mPosition, mRotation);
}

void TBiancoGateKeeper::launchNamekuri()
{
	TNameKuriManager* mgr = JDrama::TNameRefGen::search<TNameKuriManager>(
	    "拡散ナメクリマネージャー");
	if (mgr == NULL)
		mgr = JDrama::TNameRefGen::search<TNameKuriManager>(
		    "ナメクリマネージャー");

	if (mgr != NULL) {
		for (int i = 0; i < 10; i++) {
			TNameKuri* enemy = (TNameKuri*)mgr->getFarOutEnemy();
			if (enemy == NULL)
				break;

			JGeometry::TVec3<f32> pos = mPosition;
			pos.y += 100.0f;
			JGeometry::TVec3<f32> rot = mRotation;
			JGeometry::TVec3<f32> scale;
			scale.set(1.0f, 1.0f, 1.0f);
			s16 angle = (s16)(182.04445f * (36.0f * (f32)i));
			JGeometry::TVec3<f32> vel;
			vel.set(4.0f * JMASSin(angle), 12.0f, 4.0f * JMASCos(angle));

			enemy->reset();
			enemy->mPosition  = pos;
			enemy->mRotation  = rot;
			enemy->mScaling   = scale;
			enemy->mHitPoints = enemy->getMaxHitPoints();
			enemy->mVelocity  = vel;
			enemy->onLiveFlag(LIVE_FLAG_AIRBORNE);
			enemy->offLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_UNK8);
			enemy->onLiveFlag(LIVE_FLAG_UNK8000);
			enemy->offHitFlag(HIT_FLAG_NO_COLLISION);
			enemy->mSpine->pushNerve(&TNerveNameKuriDiffuse::theNerve());
		}
	}
}

f32 TBiancoGateKeeper::getRumblePow()
{
	JGeometry::TVec3<f32> diff = mPosition;
	diff -= SMS_GetMarioPos();
	f32 dist = diff.length();
	if (dist == 0.0f)
		return 1.0f;
	f32 pow = 2000.0f / dist;
	if (pow > 1.0f)
		pow = 1.0f;
	return pow;
}

void TBiancoGateKeeper::rumblePad()
{
	if (SMS_IsMarioTouchGround4cm()) {
		mRumblePower = getRumblePow();
		SMSRumbleMgr->start(8, &mRumblePower);
	}
}

void TBiancoGateKeeper::deathRumble()
{
	if (SMS_IsMarioTouchGround4cm()) {
		J3DFrameCtrl* fc = mMActor->getFrameCtrl(0);
		if (fc != NULL) {
			f32 t        = 1.0f - fc->getFrame() / (f32)fc->getEnd();
			mRumblePower = t * getRumblePow();
			SMSRumbleMgr->start(8, &mRumblePower);
		}
	}
}

BOOL TBiancoGateKeeper::curBckFinished() const
{
	J3DFrameCtrl* fc = mMActor->getFrameCtrl(0);
	BOOL done;
	if (fc == NULL)
		return true;

	if (fc->checkState(J3DFrameCtrl::STATE_COMPLETED_ONCE)
	    || fc->checkState(J3DFrameCtrl::STATE_LOOPED_ONCE)
	    || 0.1f + fc->getFrame() >= (f32)fc->getEnd())
		return true;
	else
		return false;
}

void TBiancoGateKeeper::startBGM()
{
	MSBgm::setTrackVolume(0, 0.0f, 0xA, 0);
	MSBgm::startBGM(0x8001000B);
}

void TBiancoGateKeeper::stopBGM() { MSBgm::stopTrackBGM(1, 0xA); }

void TBiancoGateKeeper::startAppearDemo()
{
	if (unk28A != 0)
		return;

	snprintf(mDemoName, 0x100, "%s出現カメラ", mName);
	SMSGetMarDirector()->fireStartDemoCamera(mDemoName, &mPosition, -1, 0.0f,
	                                         true, nullptr, 0, nullptr,
	                                         JDrama::TFlagT<u16>(0));
	startBGM();
}

void TBiancoGateKeeper::startFinishDemo()
{
	snprintf(mDemoName, 0x100, "%s撃沈カメラ", mName);
	SMSGetMarDirector()->fireStartDemoCamera(mDemoName, &mPosition, -1, 0.0f,
	                                         true, nullptr, 0, nullptr,
	                                         JDrama::TFlagT<u16>(0));
	stopBGM();
}

// TODO: fake/temporary dont_inline
#pragma dont_inline on
BOOL TBiancoGateKeeper::isHeadHitActive() const
{
	J3DFrameCtrl* fc = getMActor()->getFrameCtrl(0);
	if (getMActor()->checkCurBckFromIndex(0x12)) {
		if (50.0f < fc->getFrame() && fc->getFrame() < 160.0f)
			return true;
		return false;
	}
	if (getMActor()->checkCurBckFromIndex(0xF)) {
		if (fc->getFrame() > 40.0f)
			return true;
		return false;
	}
	if (getMActor()->checkCurBckFromIndex(0x10)
	    || getMActor()->checkCurBckFromIndex(0xC))
		return true;
	return false;
}
#pragma dont_inline off

BOOL TBiancoGateKeeper::isDamageFogSituation() const
{
	const TNerveBase<TLiveActor>* awake = &TNerveBGKAwakeDamage::theNerve();
	if (mSpine->getLatestNerve() == awake && mSpine->getTime() <= 110)
		return true;

	if (mMActor->checkCurBckFromIndex(0x12)) {
		J3DFrameCtrl* fc = mMActor->getFrameCtrl(0);
		if (!(50.0f < fc->getFrame() && fc->getFrame() < 160.0f))
			return false;
	}

	if (mMActor->checkCurBckFromIndex(0xB)
	    || mMActor->checkCurBckFromIndex(0x11)
	    || mMActor->checkCurBckFromIndex(0xD)
	    || mMActor->checkCurBckFromIndex(7) || mMActor->checkCurBckFromIndex(6))
		return false;

	if (unk17C > 0)
		return true;
	return false;
}

const char** TBiancoGateKeeper::getBasNameTable() const
{
	return gatekeeper_bastable;
}

void TBiancoGateKeeper::emitParticles()
{
	const TNerveBase<TLiveActor>* sleep = &TNerveBGKSleep::theNerve();
	if (mSpine->getLatestNerve() != sleep) {
		const TNerveBase<TLiveActor>* goro = &TNerveBGKLaunchGoro::theNerve();
		if (mSpine->getLatestNerve() != goro) {
			J3DModel* model = getModel();
			JPABaseEmitter* emitter;
			emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
			    GATEKEEPER_JPA_MS_GKPA_BOTA, model->getAnmMtx(6), 1, this);
			if (emitter)
				SMSSetEmitterPolColor(emitter, 6);
			emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
			    GATEKEEPER_JPA_MS_GKPA_YODARE_L, model->getAnmMtx(9), 1,
			    (u8*)this + 1);
			if (emitter)
				SMSSetEmitterPolColor(emitter, 6);
			emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
			    GATEKEEPER_JPA_MS_GKPA_YODARE_L, model->getAnmMtx(5), 1,
			    (u8*)this + 2);
			if (emitter)
				SMSSetEmitterPolColor(emitter, 6);
			emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
			    GATEKEEPER_JPA_MS_GKPA_YODARE_S, model->getAnmMtx(8), 1,
			    (u8*)this + 3);
			if (emitter)
				SMSSetEmitterPolColor(emitter, 6);
		}
	}
}

void TBiancoGateKeeper::controlCollision()
{
	if (mMActor->checkCurBckFromIndex(0xB)
	    || mMActor->checkCurBckFromIndex(7)) {
		mHead->mVulnerable = TRUE;
		mVulnerable        = FALSE;
		return;
	}
	if (mMActor->checkCurBckFromIndex(0x12)) {
		mHead->mVulnerable = TRUE;
		mVulnerable        = FALSE;
		return;
	}
	if (mMActor->checkCurBckFromIndex(0xA)
	    || mMActor->checkCurBckFromIndex(4)) {
		mHead->mVulnerable = FALSE;
		mVulnerable        = TRUE;
		return;
	}
	if (mMActor->checkCurBckFromIndex(0xF)) {
		if (mMActor->getFrameCtrl(0)->getFrame() > 40.0f)
			mHead->mVulnerable = TRUE;
		else
			mHead->mVulnerable = FALSE;
		mVulnerable = FALSE;
		return;
	}
	if (mMActor->checkCurBckFromIndex(0x10)
	    || mMActor->checkCurBckFromIndex(0xC)) {
		mHead->mVulnerable = TRUE;
		mVulnerable        = FALSE;
		return;
	}
	mHead->mVulnerable = FALSE;
	mVulnerable        = FALSE;
}

void TBiancoGateKeeper::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;

	if (!gpMarDirector->isDemoModeNow()) {
		if (gpMarDirector->isTalkModeNow())
			param_1 &= ~1;
	}

	if (param_1 & 1) {
		for (int i = 0; i < getColNum(); i++) {
			if (getCollision(i)->isActorType(0x80000001))
				SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
		}

		if (unk154 > 0)
			unk17E = 0x10;

		if (unk17E > 0) {
			if (unk17C < 0xFF)
				unk17C++;
			else
				unk17C = 0xFF;
		} else {
			if (unk17C > 0)
				unk17C--;
			else
				unk17C = 0;
		}

		if (unk17E > 0)
			unk17E--;
	}

	if (param_1 & 1) {
		TBGKMtxCalc* mc = unk178;
		f32 delta       = -unk158;
		mc->unk50 += delta;
		if (mc->unk50 < 0.0f)
			mc->unk50 = 0.0f;
		else if (mc->unk50 > 1.0f)
			mc->unk50 = 1.0f;
	}

	if (param_1 & 1)
		controlCollision();

	if (param_1 & 2)
		SMSGetMSound()->startSoundActor(MSD_SE_EN_GATEKEEPER_BASE, &mPosition,
		                                0, nullptr, 0, 4);

	if (param_1 & 2) {
		mMActor->getModel()->getModelData()->getJointNodePointer(0)->setMtxCalc(
		    unk178);
	}

	if (param_1 & 0x200) {
		if (isDamageFogSituation()) {
			mMActor->offMakeDL();
			SMS_AddDamageFogEffect(mMActor->getModel()->getModelData(),
			                       mPosition, param_2);
		} else {
			SMS_ResetDamageFogEffect(mMActor->getModel()->getModelData());
		}
	}

	mHead->perform(param_1, param_2);
	mObstacle->perform(param_1, param_2);

	if (param_1 & 2)
		mMultiBtk->update();

	if (param_1 & 1 && !mHintShown && mMActor->checkCurBckFromIndex(7)
	    && unk154 > 0) {
		mHintTimer += 1;
		if (mHintTimer > 160) {
			mHintTimer = 0;
			mHintShown = TRUE;
			gpMarDirector->getConsole()->startAppearBalloon(0xE002E, true);
		}
	}

	TGateKeeperBase::perform(param_1, param_2);

	if (param_1 & 2 && !checkLiveFlag(LIVE_FLAG_CLIPPED_OUT))
		emitParticles();
}

DEFINE_NERVE(TNerveBGKSleep, TLiveActor)
{
	TBiancoGateKeeper* self = (TBiancoGateKeeper*)spine->getBody();

	if (spine->getTime() == 0) {
		self->changeBck(0xA);
		self->offHitFlag(HIT_FLAG_NO_COLLISION);
		self->getMActor()->setBpkFromIndex(0);
		J3DFrameCtrl* fc = self->getMActor()->getFrameCtrl(2);
		if (fc != NULL) {
			fc->setFrame(0.0f);
			fc->setRate(0.0f);
		}
	}

	if (gpMarDirector->getCurrentMap() == 2) {
		if (self->unk298 > 0)
			self->unk298--;
		if (self->unk298 == 0) {
			int timer    = self->getSaveParams()->mSLLaunchTimerNormal.get();
			self->unk298 = (s32)(240.0f * MsRandF()) + timer - 120;
			spine->pushAfterCurrent(&TNerveBGKLaunchGoro::theNerve());
			return true;
		}
	}

	if (self->getMActor()->checkBckPass(18.0f))
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    GATEKEEPER_JPA_MS_GKPA_KEMURI_W, self->getModel()->getAnmMtx(0), 2,
		    nullptr);

	if (self->unk17C >= 0xFF) {
		spine->pushAfterCurrent(&TNerveBGKAppear::theNerve());
		return true;
	}

	if (self->unk154 > 0) {
		spine->pushAfterCurrent(&TNerveBGKSleepDamage::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBGKAppear, TLiveActor)
{
	TBiancoGateKeeper* self = (TBiancoGateKeeper*)spine->getBody();

	if (spine->getTime() == 0) {
		self->changeBck(0);
		if (self->unk28A == 0) {
			self->startAppearDemo();
		}
		if (self->unk28A < 0xFF)
			self->unk28A++;
		if (self->unk28A == 2)
			gpMarDirector->getConsole()->startAppearBalloon(0xE0047, true);
	}

	if (spine->getTime() == 8) {
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    GATEKEEPER_JPA_MS_GKPA_KEMURI, self->getModel()->getAnmMtx(12), 2,
		    nullptr);
		self->rumblePad();
	}

	if (self->getMActor()->curAnmEndsNext(0, nullptr)) {
		if (gpMarDirector->getCurrentMap() == 0)
			spine->pushAfterCurrent(&TNerveBGKWait2::theNerve());
		else
			spine->pushAfterCurrent(&TNerveBGKWait::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBGKWait, TLiveActor)
{
	TBiancoGateKeeper* self = (TBiancoGateKeeper*)spine->getBody();
	MActor* actor           = self->getMActor();

	if (spine->getTime() == 0)
		self->changeBck(0x11);

	if (self->unk154 > 0 && self->getMActor()->checkCurBckFromIndex(0x12)
	    && !self->isHeadHitActive()) {
		self->changeBck(7);
		return false;
	}

	if (self->unk154 > 0 && !actor->checkCurBckFromIndex(0xB)
	    && !actor->checkCurBckFromIndex(7) && !actor->checkCurBckFromIndex(0x11)
	    && !actor->checkCurBckFromIndex(0xD)) {
		self->decHitPoints();
		if (self->getHitPoints() == 0)
			spine->pushAfterCurrent(&TNerveBGKDie::theNerve());
		else
			spine->pushAfterCurrent(&TNerveBGKAwakeDamage::theNerve());
		return true;
	}

	if (self->unk154 > 0 && actor->checkCurBckFromIndex(0xB)) {
		self->changeBck(7);
		return false;
	}

	if (self->unk154 > 0 && actor->checkCurBckFromIndex(7))
		self->resetUnk290();

	if (spine->getTime() > self->getSaveParams()->mSLDiveTimer.get()
	    && !actor->checkCurBckFromIndex(0xD)) {
		if (self->curBckFinished())
			self->changeBck(0xD);
	}

	if (self->curBckFinished()) {
		if (actor->checkCurBckFromIndex(0x11) && self->unk28A == 1
		    && self->getHitPoints() == 3) {
			self->changeBck(0xB);
		} else if (actor->checkCurBckFromIndex(0x11)) {
			self->changeBck(0x12);
		} else if (actor->checkCurBckFromIndex(0xB)) {
			self->changeBck(0x12);
		} else if (actor->checkCurBckFromIndex(0x12)) {
			self->changeBck(0x12);
		} else if (actor->checkCurBckFromIndex(7)) {
			if (self->unk154 > 0) {
				self->resetUnk290();
				self->changeBck(7);
			} else {
				self->unk17C = 0;
				if (self->checkUnk290()) {
					self->resetUnk290();
					self->changeBck(0x12);
				} else {
					self->tickUnk290();
					self->changeBck(7);
				}
			}
		} else if (actor->checkCurBckFromIndex(0xD)) {
			spine->pushAfterCurrent(&TNerveBGKDive::theNerve());
			return true;
		}
	}

	return false;
}

DEFINE_NERVE(TNerveBGKWait2, TLiveActor)
{
	TBiancoGateKeeper* self = (TBiancoGateKeeper*)spine->getBody();
	MActor* actor           = self->getMActor();

	if (spine->getTime() == 0)
		self->changeBck(0x11);

	if (self->unk154 > 0) {
		self->decHitPoints();
		if (self->getHitPoints() == 0)
			spine->pushAfterCurrent(&TNerveBGKDie::theNerve());
		else
			spine->pushAfterCurrent(&TNerveBGKAwakeDamage::theNerve());
		return true;
	}

	if (self->curBckFinished()) {
		if (actor->checkCurBckFromIndex(0x11)) {
			self->changeBck(0xF);
		} else if (actor->checkCurBckFromIndex(0xF)) {
			self->changeBck(0x10);
		} else if (actor->checkCurBckFromIndex(0x10)) {
			self->unk288++;
			if (self->unk288 == 2 && self->unk28A == 1
			    && gpMarDirector->getCurrentMap() == 0)
				gpMarDirector->getConsole()->startAppearBalloon(0xE0000, true);
			if (self->unk288 > self->getSaveParams()->mSLLoop2Dive.get()) {
				self->changeBck(0xC);
				self->unk288 = 0;
			}
		} else if (actor->checkCurBckFromIndex(0xC)) {
			self->changeBck(0xD);
		} else if (actor->checkCurBckFromIndex(0xD)) {
			spine->pushAfterCurrent(&TNerveBGKDive::theNerve());
			return true;
		}
	}

	return false;
}

DEFINE_NERVE(TNerveBGKSleepDamage, TLiveActor)
{
	TBiancoGateKeeper* self = (TBiancoGateKeeper*)spine->getBody();

	if (spine->getTime() == 0)
		self->changeBck(4);

	if (self->mVariant == TBiancoGateKeeper::VARIANT_GENERIC) {
		if (self->unk298 > 0)
			self->unk298--;
		if (self->unk298 == 0) {
			int timer    = self->getSaveParams()->mSLLaunchTimerDamage.get();
			self->unk298 = (s32)(240.0f * MsRandF()) - 120 + timer;
			self->launchGorogoro();
			self->rumblePad();
		}
	}

	if (self->getMActor()->curAnmEndsNext(0, nullptr)) {
		spine->pushAfterCurrent(&TNerveBGKSleep::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBGKAwakeDamage, TLiveActor)
{
	TBiancoGateKeeper* self = (TBiancoGateKeeper*)spine->getBody();

	if (spine->getTime() == 0)
		self->changeBck(3);

	if (self->curBckFinished()) {
		if (gpMarDirector->getCurrentMap() == 0)
			spine->pushAfterCurrent(&TNerveBGKWait2::theNerve());
		else
			spine->pushAfterCurrent(&TNerveBGKWait::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBGKDie, TLiveActor)
{
	TBiancoGateKeeper* self = (TBiancoGateKeeper*)spine->getBody();

	if (spine->getTime() == 0) {
		self->changeBck(5);
		if (((self->mVariant != TBiancoGateKeeper::VARIANT_RICO_GATEKEEPER
		      && self->mVariant != TBiancoGateKeeper::VARIANT_MAMMA_GATEKEEPER)
		     || self->unk296 != 0)) {
			self->startFinishDemo();
			self->getMActor()->setBpkFromIndex(0);
			J3DFrameCtrl* fc = self->getMActor()->getFrameCtrl(2);
			if (fc != NULL) {
				fc->setFrame(0.0f);
				fc->setRate(SMSGetAnmFrameRate());
			}
			JPABaseEmitter* emitter
			    = gpMarioParticleManager->emitAndBindToMtxPtr(
			        GATEKEEPER_JPA_MS_GKPA_DEAD, self->getModel()->getAnmMtx(6),
			        0, nullptr);
			if (emitter)
				SMSSetEmitterPolColor(emitter, 6);
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    GATEKEEPER_JPA_MS_GKPA_DEADSMOKE,
			    self->getModel()->getAnmMtx(0), 0, nullptr);
		}
	}

	if (spine->getTime() == 0x154) {
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    GATEKEEPER_JPA_MS_GKPA_KEMURI, self->getModel()->getAnmMtx(0), 2,
		    nullptr);
		SMSGetMSound()->startSoundActor(MSD_SE_DM_OSEN_DISAPPEAR,
		                                &self->mPosition, 0, nullptr, 0, 4);
		self->rumblePad();
	}

	self->deathRumble();

	if (self->getMActor()->curAnmEndsNext(0, nullptr)) {
		if ((self->mVariant == TBiancoGateKeeper::VARIANT_RICO_GATEKEEPER
		     || self->mVariant == TBiancoGateKeeper::VARIANT_MAMMA_GATEKEEPER)
		    && self->unk296 == 0) {
			self->unk296 += 1;
			self->mHitPoints = 3;
			if (self->mVariant == TBiancoGateKeeper::VARIANT_MAMMA_GATEKEEPER)
				spine->pushAfterCurrent(&TNerveBGKLaunchName::theNerve());
			else
				spine->pushAfterCurrent(&TNerveBGKAppear::theNerve());
			return true;
		}
		self->mStampEnabled = 0;
		self->kill();
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBGKDive, TLiveActor)
{
	TBiancoGateKeeper* self = (TBiancoGateKeeper*)spine->getBody();

	if (spine->getTime() == 0)
		self->changeBck(6);

	if (spine->getTime() == 0xF0) {
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    GATEKEEPER_JPA_MS_GKPA_KEMURI, self->getModel()->getAnmMtx(0), 2,
		    nullptr);
		self->rumblePad();
	}

	if (self->getMActor()->curAnmEndsNext(0, nullptr)) {
		if (self->mVariant == TBiancoGateKeeper::VARIANT_GENERIC) {
			int timer    = self->getSaveParams()->mSLLaunchTimerNormal.get();
			self->unk298 = (s32)(240.0f * MsRandF()) + timer - 120;
			self->launchGorogoro();
			self->rumblePad();
		}
		spine->pushAfterCurrent(&TNerveBGKSleep::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBGKLaunchGoro, TLiveActor)
{
	TBiancoGateKeeper* self = (TBiancoGateKeeper*)spine->getBody();

	if (spine->getTime() == 0)
		self->changeBck(8);

	if (spine->getTime() == 0x34) {
		self->launchGorogoro();
		self->rumblePad();
	}

	if (self->unk17C >= 0xFF) {
		spine->pushAfterCurrent(&TNerveBGKAppear::theNerve());
		return true;
	}

	if (self->getMActor()->curAnmEndsNext(0, nullptr)) {
		spine->pushAfterCurrent(&TNerveBGKSleep::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBGKLaunchName, TLiveActor)
{
	TBiancoGateKeeper* self = (TBiancoGateKeeper*)spine->getBody();

	if (spine->getTime() == 0)
		self->changeBck(8);

	if (spine->getTime() == 0x34) {
		self->launchNamekuri();
		self->rumblePad();
	}

	if (self->getMActor()->curAnmEndsNext(0, nullptr)) {
		spine->pushAfterCurrent(&TNerveBGKAppear::theNerve());
		return true;
	}

	return false;
}

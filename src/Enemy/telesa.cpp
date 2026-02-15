#include <Enemy/Telesa.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DCluster.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JMath.hpp>
#include <System/EmitterViewObj.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/SharedParts.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/ScreenUtil.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <MoveBG/MapObjBlock.hpp>
#include <MoveBG/ItemManager.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/Conductor.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <Player/MarioAccess.hpp>
#include <Map/MapData.hpp>
#include <Map/Map.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

static const char* telesa_bastable[] = {
	nullptr,
	"/scene/telesa/bas/telesa_clush.bas",
	"/scene/telesa/bas/telesa_down.bas",
	nullptr,
	nullptr,
	nullptr,
	"/scene/telesa/bas/telesa_wait.bas",
};

static const GXColorS10 cTelesaColor[2] = {
	{ 350, 350, 350, 255 }, // white and boring
	{ 600, 270, 220, 255 }, // pink and cute
};

static const GXColor cTelesaColorStart = { 0, 0, 0, 0 };
static const GXColor cTelesaColorEnd   = { 255, 255, 255, 255 };

TTelesaSaveLoadParams::TTelesaSaveLoadParams(const char* path)
    : TWalkerEnemyParams(path)
    , PARAM_INIT(mSLFadeInTime, 60)
    , PARAM_INIT(mSLAppearTime, 60)
    , PARAM_INIT(mSLFadeOutTime, 60)
    , PARAM_INIT(mSLFlyHeightMax, 200.0f)
    , PARAM_INIT(mSLFlyHeightMin, 100.0f)
    , PARAM_INIT(mSLFlyAmplitudeMax, 100.0f)
    , PARAM_INIT(mSLFlyAmplitudeMin, 100.0f)
    , PARAM_INIT(mSLFlyFrequencyMax, 200.0f)
    , PARAM_INIT(mSLFlyFrequencyMin, 100.0f)
    , PARAM_INIT(mSLTelesaGravityY, 0.1f)
    , PARAM_INIT(mSLGenAttackerTime, 500)
    , PARAM_INIT(mSLTelesaAttackGravityY, 0.01f)
    , PARAM_INIT(mSLTelesaPowerByWater, 40.0f)
    , PARAM_INIT(mSLLoopAppearTime, 100)
    , PARAM_INIT(mSLLoopHideTime, 100)
    , unk458(0.0f)
    , unk45C(1.0f)
    , unk460(0.0f)
    , unk464(1.0f)
    , unk468(0.0f)
    , unk46C(1.0f)
{
	TParams::load(mPrmPath);
	unk458 = mSLFlyHeightMin.get();
	unk45C = mSLFlyHeightMax.get();
	unk460 = mSLFlyAmplitudeMin.get();
	unk464 = mSLFlyAmplitudeMax.get();
	unk468 = mSLFlyFrequencyMin.get();
	unk46C = mSLFlyFrequencyMax.get();
}

TTelesaManager::TTelesaManager(const char* name)
    : TSmallEnemyManager(name)
    , unk60(0)
    , unk61(0)
    , mModokiTelesaModel(nullptr)
{
}

void TTelesaManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TTelesaSaveLoadParams("/enemy/telesa.prm");

	void* data = JKRGetResource("/scene/telesa/modoki.bmd");
	mModokiTelesaModel
	    = new SDLModelData(J3DModelLoaderDataBase::load(data, 0x11020000));

	TSmallEnemyManager::load(stream);
}

TSmallEnemy* TTelesaManager::createEnemyInstance() { return new TTelesa; }

void TTelesaManager::createEnemies(int param_1)
{
	TEnemyManager::createEnemies(param_1);
	int bodyMatIdx = getObj(0)
	                     ->getMActor()
	                     ->getModel()
	                     ->getModelData()
	                     ->getMaterialName()
	                     ->getIndex("_mat_body");

	for (int i = 0; i < mObjNum; ++i) {
		TTelesa* telesa          = (TTelesa*)unk18[i];
		telesa->mTelesaFadeColor = cTelesaColorStart;
		for (u16 i = 0; i < 4; ++i) {
			if (i != bodyMatIdx)
				SMS_InitPacket_OneTevKColor(telesa->getMActor()->getModel(), i,
				                            GX_KCOLOR0,
				                            &telesa->mTelesaFadeColor);
			else
				SMS_InitPacket_OneTevColorAndOneTevKColor(
				    telesa->getMActor()->getModel(), i, GX_TEVREG0,
				    &telesa->mTelesaBaseColor, &telesa->mTelesaFadeColor);
		}
	}
}

void TTelesaManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "telesa.bmd", 0x15240000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TTelesaManager::telesaForceKill()
{
	bool anyKilled = false;
	for (int i = 0; i < mObjNum; ++i) {
		TTelesa* telesa = (TTelesa*)unk18[i];
		if (!telesa->checkLiveFlag(LIVE_FLAG_DEAD)) {
			telesa->kill();
			anyKilled = true;
		}
	}
	if (anyKilled) {
		Vec* pos = &getObj(0)->mPosition;
		if (gpMSound->gateCheck(0x2943))
			MSoundSESystem::MSoundSE::startSoundActor(0x2943, pos, 0, nullptr,
			                                          0, 4);
	}
}

void TTelesaManager::generatePetBottle(TTelesa* telesa)
{
	unk61 += 1;

	if (unk61 >= 5) {
		unk61 = 0;
		gpItemManager->makeObjAppear(telesa->mPosition.x,
		                             telesa->mPosition.y + 20.0f,
		                             telesa->mPosition.z, 0x20000002, true);
	}
}

void TTelesaManager::setFlagOutOfCube()
{
	for (int i = 0; i < getObjNum(); ++i) {
		TTelesa* telesa = (TTelesa*)unk18[i];

		if (telesa->checkUnk150(0x40))
			continue;

		JGeometry::TVec3<f32> pos = telesa->getPosition();
		pos.y += 75.0f;
		if (gpCubeArea->isInAreaCube(pos))
			telesa->offLiveFlag(LIVE_FLAG_UNK200);
		else
			telesa->onLiveFlag(LIVE_FLAG_UNK200);
	}
}

f32 TTelesa::mFlyAngMax = 35.0f;

TTelesa::TTelesa(const char* name)
    : TWalkerEnemy(name)
    , mCurrentFlyHeight(0.0f)
    , mTargetFlyHeight(0.0f)
    , mFlyBobOffsetY(50.0f)
    , mFlyBobAmplitude(50.0f)
    , mFlyBobPhase(0.0f)
    , mFlyBobFrequency(0.0f)
    , mDampenedGroundHeight(0.0f)
    , mGroundHeightDampeningSpeed(0.0f)
    , unk1B8(0)
    , mTelesaType(TELESA_TYPE_NORMAL)
    , mFadeLoopTimer(0)
    , mImitatedBmd(nullptr)
    , unk1C8(0)
    , mFadeTimer(0)
    , mFadeState(FADE_STATE_INVISIBLE)
{
	setTypeNormal();
}

void TTelesa::load(JSUMemoryInputStream& stream)
{
	TSmallEnemy::load(stream);
	reset();
	mDampenedGroundHeight = mPosition.y;
	setTypeNormal();
}

void TTelesa::init(TLiveManager* manager)
{
	TWalkerEnemy::init(manager);
	mActorType = 0x1000000C;
	unk150     = 18;
	offHitFlag(HIT_FLAG_UNK10000000);
	onHitFlag(HIT_FLAG_UNK8000000);
	unk194 = (TTelesaSaveLoadParams*)getSaveParam();
	mSpine->initWith(&TNerveWalkerGraphWander::theNerve());
	onLiveFlag(LIVE_FLAG_UNK8);

	setFlyParam(1.0f);

	TScreenTexture* tex
	    = JDrama::TNameRefGen::search<TScreenTexture>("スクリーンテクスチャ");
	const ResTIMG* img    = tex->getTexture()->getTexInfo();
	J3DSkinDeform* deform = new J3DSkinDeform;
	mMActor->getModel()->setSkinDeform(deform, J3D_DEFORM_ATTACH_FLAG_UNK_1);
	mMActor->resetDL();
	SMS_ChangeTextureAll(mMActor->getModel()->getModelData(), "H_ma_rak_dummy",
	                     *img);
	mMActor->setLightType(3);
	if (mInstanceIndex == 0) {
		for (u16 i = 0; i < getModel()->getModelData()->getJointNum(); ++i)
			;
	}

	unk188 = 0.0f;
}

void TTelesa::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("telesa.bmd", 3);
}

void TTelesa::reset()
{
	TWalkerEnemy::reset();

	onLiveFlag(LIVE_FLAG_UNK8);
	onLiveFlag(LIVE_FLAG_HIDDEN);

	setFlyParam(1.0f);

	unk1B8      = 0;
	mHeadHeight = 100.0f;
	offHitFlag(HIT_FLAG_UNK10000000 | HIT_FLAG_UNK8000000);
	if (checkUnk150(0x1))
		mSpine->initWith(&TNerveTelesaAttackMario::theNerve());
	else
		mSpine->initWith(&TNerveWalkerGraphWander::theNerve());

	if (mImitatedBmd)
		mSpine->initWith(&TNerveTelesaImitate::theNerve());

	mTelesaBaseColor = cTelesaColor[0];
}

void TTelesa::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TSmallEnemy::perform(param_1, param_2);
	if (!checkLiveFlag(LIVE_FLAG_UNK200 | LIVE_FLAG_DEAD)) {
		if (mImitatedBmd) {
			if (param_1 & 2) {
				const TBGCheckData* pTStack_5c;
				gpMap->checkGround(mPosition.x, mPosition.y, mPosition.z,
				                   &pTStack_5c);
				Mtx afStack_58;
				MsMtxSetXYZRPH(afStack_58, mPosition.x, mPosition.y,
				               mPosition.z, mRotation.x, mRotation.y,
				               mRotation.z);
				mImitatedBmd->getMActor()->getModel()->setBaseTRMtx(afStack_58);
				J3DModel* model = mImitatedBmd->getMActor()->getModel();
				model->unk14    = JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f);
			}

			if (param_1 & 0x200)
				mImitatedBmd->getMActor()->setLightData(mGroundPlane,
				                                        mPosition);

			mImitatedBmd->getMActor()->perform(param_1, param_2);
		}
	}
}

void TTelesa::drawObject(JDrama::TGraphics* param_1)
{
	if (mImitatedBmd == nullptr)
		TLiveActor::drawObject(param_1);
}

// TODO: wild guess
void TTelesa::setFlyParam(f32 param_1)
{
	// TODO: random interval stuff :(
	mTargetFlyHeight = MsRandF(unk194->unk458, unk194->unk45C);
	mTargetFlyHeight *= param_1;
	mCurrentFlyHeight = 0.0f;

	mFlyBobAmplitude = MsRandF(unk194->unk460, unk194->unk464);
	mFlyBobAmplitude *= param_1;
	mFlyBobPhase = 0.0f;

	mFlyBobFrequency = MsRandF(unk194->unk468, unk194->unk46C);
}

void TTelesa::setBehavior()
{
	if (mSpine->getCurrentNerve() != &TNerveWalkerGraphWander::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveSmallEnemyWait::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveTelesaAttackMario::theNerve())
		return;

	mFlyBobPhase += 1.0f;

	if (mFlyBobPhase > mFlyBobFrequency)
		mFlyBobPhase = 0.0f;

	f32 phase      = (mFlyBobPhase * 360.0f) / mFlyBobFrequency;
	mFlyBobOffsetY = mFlyBobAmplitude * JMASin(phase);
	f32 newRotX    = 10.0f - mFlyAngMax * JMACos(phase);
	if (abs(newRotX - mRotation.x) > 5.0f)
		mRotation.x == -newRotX; // HUH???
	else
		mRotation.x = newRotX;
}

void TTelesa::attackToMario()
{
	if (checkLiveFlag(LIVE_FLAG_HIDDEN)
	    || !(mPosition.y + mAttackHeight - 50.0f < SMS_GetMarioPos().y)) {
		SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
		if (unk184) {
			if (mSpine->getCurrentNerve()
			    != &TNerveWalkerPostAttack::theNerve())
				; // huh???
		}
	}
}

void TTelesa::behaveToWater(THitActor* param_1)
{
	unk165 = 1;
	if (mSpine->getCurrentNerve() != &TNerveTelesaFreeze::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveTelesaImitate::theNerve()) {
		f32 fVar2 = 2.0f;
		if (unk184)
			fVar2 = 30.0f;
		JGeometry::TVec3<f32> local_20(mPosition.x - SMS_GetMarioPos().x, fVar2,
		                               mPosition.z - SMS_GetMarioPos().z);
		mPosition.y += 2.0f;
		MsVECNormalize(&local_20, &local_20);
		f32 fVar1 = 5.0f;
		if (unk184) {
			fVar1 = unk194->mSLTelesaPowerByWater.get();
			mPosition.y += 30.0f;
		}
		mVelocity = local_20 * fVar1;

		onLiveFlag(LIVE_FLAG_AIRBORNE);

		if (mFadeState != FADE_STATE_VISIBLE
		    && mFadeState != FADE_STATE_FADE_IN)
			mFadeState = FADE_STATE_FADE_IN;

		mSpine->pushNerve(&TNerveTelesaFreeze::theNerve());
		unk1C8 = 1;
		onHitFlag(HIT_FLAG_UNK10000000);
	}
}

void TTelesa::walkBehavior(int param_1, f32 param_2)
{
	TWalkerEnemy::walkBehavior(param_1, param_2);
	if (mSpine->getCurrentNerve() != &TNerveTelesaDie::theNerve()
	    && mCurrentFlyHeight < mTargetFlyHeight)
		mCurrentFlyHeight += 1.0f;

	f32 fVar1 = mGroundHeight;
	if (unk184)
		fVar1 = 500.0f;

	if (mGroundHeightDampeningSpeed == 0.0f
	    && abs(mDampenedGroundHeight - fVar1) > 5.0f) {
		mGroundHeightDampeningSpeed = (mGroundHeight - fVar1) / 120.0f;
		if (unk184) {
			mGroundHeightDampeningSpeed
			    = MsClamp(mGroundHeightDampeningSpeed, -2.0f, 2.0f);
		} else {
			if (abs(mGroundHeightDampeningSpeed) > 10.0f)
				mGroundHeightDampeningSpeed = 0.0f;
		}
	}

	if (mGroundHeightDampeningSpeed > 0.0f) {
		mDampenedGroundHeight += mGroundHeightDampeningSpeed;
		if (mDampenedGroundHeight > fVar1)
			mGroundHeightDampeningSpeed = 0.0f;
	}

	if (mGroundHeightDampeningSpeed < 0.0f) {
		mDampenedGroundHeight += mGroundHeightDampeningSpeed;
		if (mDampenedGroundHeight < fVar1) {
			mGroundHeightDampeningSpeed = 0.0f;
			mDampenedGroundHeight       = fVar1 + 1.0f;
		}
	}

	mPosition.y = mDampenedGroundHeight + mCurrentFlyHeight + mFlyBobOffsetY;
}

void TTelesa::calcRootMatrix()
{
	if (mSpine->getCurrentNerve() != &TNerveTelesaDie::theNerve()) {
		for (u16 i = 0;
		     i < mMActor->getModel()->getModelData()->getMaterialNum(); ++i) {
			Mtx afStack_94;
			SMS_GetLightPerspectiveForEffectMtx(afStack_94);
			mMActor->getModel()
			    ->getModelData()
			    ->getMaterialNodePointer(i)
			    ->getTexGenBlock()
			    ->getTexMtx(2)
			    ->setEffectMtx2(afStack_94);
		}

		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToMtxPtr(
		        0x187, mMActor->getModel()->getAnmMtx(4), 1, this)) {
			emitter->unk180.a = mTelesaFadeColor.a;
		}

		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToMtxPtr(
		        0x188, mMActor->getModel()->getAnmMtx(4), 1, this)) {
			emitter->unk180.a = mTelesaFadeColor.a;
		}
	}

	MsMtxSetXYZRPH(getModel()->getBaseTRMtx(), mPosition.x, mPosition.y,
	               mPosition.z, mRotation.x, mRotation.y, mRotation.z);
	getModel()->setBaseScale(mScaling);
}

void TTelesa::moveObject()
{
	TWalkerEnemy::moveObject();
	changeTevKColor();
}

void TTelesa::bind()
{
	if (checkLiveFlag(LIVE_FLAG_UNK10))
		return;

	JGeometry::TVec3<f32> nextPos = mPosition;
	nextPos.y -= mCurrentFlyHeight + mFlyBobOffsetY;
	nextPos += mLinearVelocity;
	nextPos += mVelocity;

	mVelocity.y -= getGravityY();

	if (mVelocity.y < mVelocityMinY)
		mVelocity.y = mVelocityMinY;

	if (mGroundHeightDampeningSpeed == 0.0f
	    || mSpine->getCurrentNerve() == &TNerveTelesaDie::theNerve()) {
		mGroundHeight
		    = gpMap->checkGround(nextPos.x, nextPos.y + mHeadHeight + 200.0f,
		                         nextPos.z, &mGroundPlane);

		if (unk184 == nullptr
		    && mSpine->getCurrentNerve() != &TNerveTelesaDie::theNerve()) {
			if (unk124->getGraph() && !unk124->getGraph()->isDummy()) {
				JGeometry::TVec3<f32> VStack_24;
				unk124->getCurrent().getPoint(&VStack_24);
				mGroundHeight = VStack_24.y;
			}
		}

		mGroundHeight += 1.0f;
	}

	if (nextPos.y <= mGroundHeight + 0.05f) {
		offLiveFlag(LIVE_FLAG_AIRBORNE);
		mVelocity.set(0.0f, 0.0f, 0.0f);
		if (mSpine->getCurrentNerve() != &TNerveTelesaDie::theNerve())
			nextPos.y = mGroundHeight;
	} else {
		onLiveFlag(LIVE_FLAG_AIRBORNE);
	}

	mLinearVelocity = nextPos - mPosition;
	mLinearVelocity.y += mCurrentFlyHeight + mFlyBobOffsetY;
}

BOOL TTelesa::isReachedToGoal() const
{
	JGeometry::TVec3<f32> local_c = unk104.getPoint();
	local_c -= mPosition;
	local_c.y = 0.0f;

	if (local_c.x == 0.0f && local_c.z == 0.0f)
		return true;

	if (MsVECMag2(&local_c) < 200.0f)
		return true;

	return false;
}

bool TTelesa::changeByJuice()
{
	if (checkUnk150(0x40)) {

		if (mJuiceBlock != nullptr)
			return true;

		mJuiceBlock = (TJuiceBlock*)gpMapObjManager->makeObjAppear(
		    mPosition.x, mPosition.y, mPosition.z, 0x400002C4, true);
		if (mJuiceBlock == nullptr)
			return false;

		mJuiceBlock->mScaling.set(1.0f, 1.0f, 1.0f);
		mJuiceBlock->unk140.set(0.1f, 0.1f, 0.1f);
		mJuiceBlock->mRotation.y = mRotation.y;
		mJuiceBlock->offLiveFlag(LIVE_FLAG_HIDDEN);
		onHitFlag(HIT_FLAG_NO_COLLISION);
		onLiveFlag(LIVE_FLAG_HIDDEN);

		mSpine->pushNerve(&TNerveSmallEnemyChange::theNerve());
		mSpine->pushAfterCurrent(&TNerveTelesaFreeze::theNerve());

		if (gpMSound->gateCheck(0x28CB))
			MSoundSESystem::MSoundSE::startSoundActor(0x28CB, &mPosition, 0,
			                                          nullptr, 0, 4);

		if (gpMSound->gateCheck(0x3881))
			MSoundSESystem::MSoundSE::startSoundActor(0x3881, &mPosition, 0,
			                                          nullptr, 0, 4);

		unk185 = 1;

		return true;
	}

	return false;
}

void TTelesa::scalingChangeActor()
{
	mJuiceBlock->unk140.x = mJuiceBlock->unk140.z = MsClamp(
	    mJuiceBlock->unk140.x + 0.02f, 0.0f, TSmallEnemyManager::mBlockXZScale);

	mJuiceBlock->unk140.y = MsClamp(mJuiceBlock->unk140.y + 0.01f, 0.0f,
	                                TSmallEnemyManager::mBlockYScale);
}

void TTelesa::changeOut()
{
	onHitFlag(HIT_FLAG_NO_COLLISION);
	if (gpMSound->gateCheck(0x293D))
		MSoundSESystem::MSoundSE::startSoundActor(0x293D, &mPosition, 0,
		                                          nullptr, 0, 4);
	offLiveFlag(LIVE_FLAG_HIDDEN);
	mPosition = mJuiceBlock->mPosition;
	gpMarioParticleManager->emitAndBindToPosPtr(0xCD, &mPosition, 0, nullptr);
	getMActor()->setFrameRate(SMSGetAnmFrameRate(), 0);
	mJuiceBlock->kill();
	mJuiceBlock = nullptr;
}

const char** TTelesa::getBasNameTable() const { return telesa_bastable; }

void TTelesa::setWalkAnm()
{
	offHitFlag(HIT_FLAG_NO_COLLISION);
	if (!isBckAnm(6)) {
		setBckAnm(6);
		mFlyBobPhase = 0.0f;
	}
}

void TTelesa::setWaitAnm() { setBckAnm(6); }

// TODO: wut?
#pragma dont_inline on
void TTelesa::reduceFlyForce()
{
	if (mCurrentFlyHeight > 0.0f)
		mCurrentFlyHeight -= 4.0f;
	else
		mCurrentFlyHeight = 0.0f;

	if (mFlyBobOffsetY > 0.0f) {
		mFlyBobOffsetY -= 4.0f;
		if (mFlyBobOffsetY < 0.0f)
			mFlyBobOffsetY = 0.0f;
	}

	if (mFlyBobOffsetY < 0.0f) {
		mFlyBobOffsetY += 4.0f;
		if (mFlyBobOffsetY > 0.0f)
			mFlyBobOffsetY = 0.0f;
	}

	mFlyBobPhase *= 0.9f;
	mRotation.x *= 0.9f;
}
#pragma dont_inline off

f32 TTelesa::getGravityY() const
{
	if (unk184)
		return unk194->mSLTelesaAttackGravityY.get();

	return unk194->mSLTelesaGravityY.get();
}

bool TTelesa::isFlying()
{
	if (mCurrentFlyHeight != 0.0f)
		return true;

	if (mFlyBobOffsetY != 0.0f)
		return true;

	if (isAirborne())
		return true;

	mDampenedGroundHeight = mPosition.y;
	return false;
}

// is this an actual predicate or some kind of a "trans Y with isReset flag
// accounted for"???
bool TTelesa::isResetTransY() { }

void TTelesa::forceKill()
{
	if (!mGroundPlane->isLegal() && !isAirborne()) {
		if (mSpine->getCurrentNerve() != &TNerveTelesaDie::theNerve())
			mSpine; // huh???
	}
}

void TTelesa::kill()
{
	if (!checkLiveFlag(LIVE_FLAG_DEAD)) {
		offLiveFlag(LIVE_FLAG_HIDDEN);
		if (mSpine->getCurrentNerve() != &TNerveTelesaDie::theNerve())
			mSpine->pushNerve(&TNerveTelesaDie::theNerve());
		onLiveFlag(LIVE_FLAG_UNK40);
	}
}

void TTelesa::initAttacker(THitActor* param_1)
{
	reset();
	unk1B8 = 1;
	offLiveFlag(LIVE_FLAG_HIDDEN);
	setTypeCanSee();
	unk184 = 1;
	mSpine->initWith(&TNerveTelesaAttackMario::theNerve());

	MtxPtr mtx = ((TLiveActor*)param_1)->getModel()->getAnmMtx(5);
	mPosition.set(mtx[3][0], mtx[3][1] - 150.0f, mtx[3][2]);
	mDampenedGroundHeight = mPosition.y;

	mVelocity.set(0.0f, 8.0f, 0.0f);

	mRotation   = param_1->mRotation;
	mRotation.x = 0.0f;

	setFlyParam(1.0f);
	unk1C8 = 0;
	offHitFlag(HIT_FLAG_UNK10000000);
	unk150 &= ~0x40;
	onLiveFlag(LIVE_FLAG_HIDDEN);
	mMActor->getFrameCtrl(0)->setFrame(0.0f);
	mHeadHeight = 250.0f;

	if (gpMSound->gateCheck(0x28D8))
		MSoundSESystem::MSoundSE::startSoundActor(0x28D8, &mPosition, 0,
		                                          nullptr, 0, 4);
}

void TTelesa::initItemAttacker(THitActor* param_1)
{
	reset();
	offLiveFlag(LIVE_FLAG_HIDDEN);
	unk1B8 = 1;
	setTypeNormal();
	mSpine->initWith(&TNerveTelesaAttackMario::theNerve());
	mDampenedGroundHeight = SMS_GetMarioGrLevel() - 50.0f;
	mRotation             = param_1->mRotation;

	setFlyParam(1.0f);
	unk150 &= ~0x40;
	mMActor->getFrameCtrl(0)->setFrame(0.0f);
	mHeadHeight = 250.0f;
	if (gpMSound->gateCheck(0x28D8))
		MSoundSESystem::MSoundSE::startSoundActor(0x28D8, &mPosition, 0,
		                                          nullptr, 0, 4);
}

void TTelesa::setAttacker()
{
	reset();
	unk1B8 = 1;
	offLiveFlag(LIVE_FLAG_HIDDEN);
	unk184 = 1;
	setTypeCanSee();
	mSpine->initWith(&TNerveTelesaAttackMario::theNerve());
	setGoalPath(TPathNode(SMS_GetMarioPos()));
	unk1C8 = 0;
	offHitFlag(HIT_FLAG_UNK10000000);
	unk150 &= ~0x40;
	moveObject();
}

bool TTelesa::isCollidMove(THitActor* param_1)
{
	return param_1->mActorType == 0x10000020 ? false : true;
}

// TODO: this FEELS real but it's 4 bytes too big!!!
bool TTelesa::resetBaseGround()
{
	bool result = unk165;
	if (result)
		unk165 = false;

	return result;
}

void TTelesa::setAttackPoint()
{
	JGeometry::TVec3<f32> pos = mPosition;

	f32 dx = SMS_GetMarioPos().x - mPosition.x;
	f32 dz = SMS_GetMarioPos().z - mPosition.z;
	// TODO: random interval
	volatile f32 minR = 0.7f;
	volatile f32 maxR = 1.6f;
	f32 r             = MsRandF(minR, maxR);

	pos.x += dx * r;
	pos.z += dz * r;

	setGoalPath(TPathNode(pos));
}

void TTelesa::setFirstAttackPoint()
{
	TRotation3f SStack_78; // TODO: uuuuuh...

	if (unk184)
		mRotation.y = 180.0f - mInstanceIndex * 720.0f;

	JGeometry::TVec3<f32> pos = mPosition;

	// TODO: probably done via TRotation calls? Why is is all so inlined ;(
	f32 s = JMASin(mRotation.y);
	f32 c = JMACos(mRotation.y);

	pos.x += c * 1000.0f;
	pos.z += s * 1000.0f;

	setGoalPath(TPathNode(pos));
}

void TTelesa::setTypeNormal()
{
	mTelesaType = TELESA_TYPE_NORMAL;
	mFadeState  = FADE_STATE_INVISIBLE;
	offLiveFlag(LIVE_FLAG_HIDDEN);
}

void TTelesa::setTypeLoop()
{
	mTelesaType = TELESA_TYPE_LOOP;
	mFadeState  = FADE_STATE_FADE_IN;
	offLiveFlag(LIVE_FLAG_HIDDEN);
}

void TTelesa::setTypeCanSee()
{
	mTelesaType = TELESA_TYPE_CAN_SEE;
	mFadeState  = FADE_STATE_VISIBLE;
	offLiveFlag(LIVE_FLAG_HIDDEN);
	mMActor->setLightType(1);
}

void TTelesa::changeTevKColor()
{
	switch (mFadeState) {
	case FADE_STATE_VISIBLE:
		mTelesaFadeColor.a = 0xff;
		mFadeTimer         = 0;
		// Invisible by default & loop telesas stay visible for some time & then
		// loop back to fading out & becoming invisible. Visible by default
		// telesas simply stay visible until something makes them fade out.
		if (mTelesaType != TELESA_TYPE_CAN_SEE
		    && mSpine->getCurrentNerve() != &TNerveTelesaFreeze::theNerve()) {
			mFadeLoopTimer += 1;
			int loopAppearTime = unk194->mSLLoopAppearTime.get();
			if (mTelesaType == TELESA_TYPE_NORMAL)
				loopAppearTime = unk194->mSLAppearTime.get();
			if (mFadeLoopTimer <= loopAppearTime)
				break;
			mMActor->setLightType(3);
			mFadeState     = FADE_STATE_FADE_OUT;
			mFadeLoopTimer = 0;
			break;
		}
		mFadeTimer     = unk194->mSLFadeOutTime.get();
		mFadeLoopTimer = 0;
		break;

	case FADE_STATE_INVISIBLE:
		mFadeTimer         = 0;
		mTelesaFadeColor.a = 0;
		// For a telesa that loops through being visible & invisible, we re-use
		// the timer to count how long it should stay invisible before fading
		// back in
		if (mTelesaType == TELESA_TYPE_LOOP) {
			mFadeLoopTimer += 1;
			if (mFadeLoopTimer > unk194->mSLLoopHideTime.get()) {
				mFadeState     = FADE_STATE_FADE_IN;
				mFadeLoopTimer = 0;
			}
		}
		break;

	case FADE_STATE_FADE_IN: {
		int fadeInTime = unk194->mSLFadeInTime.get();
		++mFadeTimer;
		int progress = mFadeTimer * 0xff / fadeInTime;
		if (progress >= 0xff) {
			mFadeState         = FADE_STATE_VISIBLE;
			mFadeTimer         = 0;
			mTelesaFadeColor.a = 0xff;
			mMActor->setLightType(1);
		} else {
			mTelesaFadeColor.a = progress;
		}
	} break;

	case FADE_STATE_FADE_OUT: {
		int fadeOutTime = unk194->mSLFadeOutTime.get();
		++mFadeTimer;
		int progress = 0xff - mFadeTimer * 0xff / fadeOutTime;
		if (mTelesaFadeColor.a == 0) {
			mFadeState         = FADE_STATE_INVISIBLE;
			mTelesaFadeColor.a = 0;
			mFadeTimer         = 0;
		} else {
			mTelesaFadeColor.a = progress;
		}
	} break;
	}
}

TSeeTelesa::TSeeTelesa(const char* name)
    : TTelesa(name)
{
}

void TSeeTelesa::load(JSUMemoryInputStream& stream)
{
	TTelesa::load(stream);
	setTypeCanSee();
}

TLoopTelesa::TLoopTelesa(const char* name)
    : TTelesa(name)
{
}

void TLoopTelesa::load(JSUMemoryInputStream& stream)
{
	TTelesa::load(stream);
	setTypeLoop();
}

TBoxTelesa::TBoxTelesa(const char* name)
    : TTelesa(name)
{
}

void TBoxTelesa::load(JSUMemoryInputStream& stream)
{
	TTelesa::load(stream);

	setTypeCanSee();

	unk150 = 0x40;
	if (TJuiceBlock* block
	    = (TJuiceBlock*)gpMapObjManager->newAndRegisterObj("TelesaBlock")) {
		block->onLiveFlag(LIVE_FLAG_DEAD);
		block->mPosition.set(0.0f, 0.0f, 0.0f);
		block->mScaling.set(1.0f, 1.0f, 1.0f);
	}
}

void TBoxTelesa::reset()
{
	TTelesa::reset();
	mTelesaBaseColor = cTelesaColor[1];
}

bool TBoxTelesa::isHitValid(u32 param_1)
{
	if (mSpine->getCurrentNerve() != &TNerveTelesaTrample::theNerve())
		mSpine->pushNerve(&TNerveTelesaTrample::theNerve());
	else
		setBckAnm(1);

	return false;
}

TMarioModokiTelesa::TMarioModokiTelesa(const char* name)
    : TTelesa(name)
    , mImitationIndex(IMITATION_INDEX_NOT_IMITATING)
{
}

void TMarioModokiTelesa::load(JSUMemoryInputStream& stream)
{
	TSmallEnemy::load(stream);

	stream.read(&mImitationIndex, 0x4);

	SDLModelData* modelToUse = ((TTelesaManager*)mManager)->mModokiTelesaModel;
	switch (mImitationIndex) {
		// NOTE: IMITATION_INDEX_NOT_IMITATING=0 stands for no model change

	case 1:
		if (void* data = JKRGetResource("/scene/mapObj/coin.bmd"))
			modelToUse = new SDLModelData(
			    J3DModelLoaderDataBase::load(data, 0x10020000));
		break;
	case 2:
		if (void* data = JKRGetResource("/scene/mapObj/coin_red.bmd"))
			modelToUse = new SDLModelData(
			    J3DModelLoaderDataBase::load(data, 0x10020000));
		break;
	case 3:
		if (void* data = JKRGetResource("/scene/mapObj/coin_blue.bmd"))
			modelToUse = new SDLModelData(
			    J3DModelLoaderDataBase::load(data, 0x10020000));
		break;
	case 4:
		if (void* data = JKRGetResource("/scene/mapObj/fruitBanana.bmd"))
			modelToUse = new SDLModelData(
			    J3DModelLoaderDataBase::load(data, 0x10020000));
		break;
	case 5:
		if (void* data = JKRGetResource("/scene/mapObj/fruitDurian.bmd"))
			modelToUse = new SDLModelData(
			    J3DModelLoaderDataBase::load(data, 0x10020000));
		break;
	case 6:
		if (void* data = JKRGetResource("/scene/mapObj/fruitPapaya.bmd"))
			modelToUse = new SDLModelData(
			    J3DModelLoaderDataBase::load(data, 0x10020000));
		break;
	case 7:
		if (void* data = JKRGetResource("/scene/mapObj/fruitPine.bmd"))
			modelToUse = new SDLModelData(
			    J3DModelLoaderDataBase::load(data, 0x10020000));
		break;
	case 8:
		if (void* data = JKRGetResource("/scene/mapObj/fruitCoconut.bmd"))
			modelToUse = new SDLModelData(
			    J3DModelLoaderDataBase::load(data, 0x10020000));
		break;
	case 9:
		if (void* data = JKRGetResource("/scene/mapObj/mashroom1up.bmd"))
			modelToUse = new SDLModelData(
			    J3DModelLoaderDataBase::load(data, 0x10020000));
		break;
	case 10:
		if (void* data = JKRGetResource("/scene/mapObj/kibako.bmd"))
			modelToUse = new SDLModelData(
			    J3DModelLoaderDataBase::load(data, 0x10020000));
		break;
	case 11:
		if (void* data = JKRGetResource("/scene/mapObj/woodbarrel.bmd"))
			modelToUse = new SDLModelData(
			    J3DModelLoaderDataBase::load(data, 0x10020000));
		break;
	case 12:
		if (void* data = JKRGetResource("/scene/monteM/mom_model.bmd"))
			modelToUse = new SDLModelData(
			    J3DModelLoaderDataBase::load(data, 0x10020000));
		break;
	}

	mImitatedBmd = new TSharedParts(this, 0, modelToUse, 3);
	reset();
	mDampenedGroundHeight = mPosition.y;
	setTypeNormal();
}

void TMarioModokiTelesa::init(TLiveManager* manager)
{
	TTelesa::init(manager);
	mSpine->initWith(&TNerveTelesaImitate::theNerve());
	unk184 = 0;
}

void TMarioModokiTelesa::imitateAnm()
{
	if (mImitationIndex == IMITATION_INDEX_NOT_IMITATING)
		mImitatedBmd->getMActor()->setBckFromIndex(0);
}

DEFINE_NERVE(TNerveTelesaImitate, TLiveActor)
{
	TTelesa* self = (TTelesa*)spine->getBody();

	TSharedParts* imitatedItem = self->mImitatedBmd;

	if (gpApplication.mCurrArea.unk0 != 7
	    && gpApplication.mCurrArea.unk0 != 14) {
		if (spine->getTime() == 0 && imitatedItem != nullptr) {
			((TMarioModokiTelesa*)self)->imitateAnm();
			imitatedItem->getMActor()->setBckFromIndex(0);
		}

		if (spine->getTime() > 500) {
			self->onHitFlag(HIT_FLAG_NO_COLLISION);
			self->onLiveFlag(LIVE_FLAG_DEAD);
		}

		return false;
	}

	self->walkBehavior(3, 1.0f);

	if (spine->getTime() == 10) {
		self->setGoalPathMario();

		if (imitatedItem != nullptr) {
			((TMarioModokiTelesa*)self)->imitateAnm();
			imitatedItem->getMActor()->setBckFromIndex(0);
		} else {
			return true;
		}
	}

	TTelesaSaveLoadParams* params
	    = (TTelesaSaveLoadParams*)self->getSaveParam();

	f32 searchAware = params->mSLSearchAware.get();

	if (!self->checkLiveFlag(LIVE_FLAG_DEAD)) {
		// TODO: this is an inline

		if (!self->resetBaseGround()) {
			if (self->isInSight(SMS_GetMarioPos(), 0.0f, 0.0f, searchAware))
				return false;
		}

		gpMarioParticleManager->emitAndBindToPosPtr(0xCD, &self->mPosition, 0,
		                                            nullptr);

		self->mImitatedBmd = nullptr;
		self->setFlyParam(1.0f);

		spine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());

		if (gpMSound->gateCheck(0x2937))
			MSoundSESystem::MSoundSE::startSoundActor(0x2937, &self->mPosition,
			                                          0, nullptr, 0, 4);

		// end of inline

		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveTelesaDie, TLiveActor)
{
	TTelesa* self = (TTelesa*)spine->getBody();

	if (spine->getTime() == 0) {
		self->decHitPoints();
		if (self->getHitPoints() == 0) {
			if (self->mFadeState == TTelesa::FADE_STATE_INVISIBLE
			    || self->mFadeState == TTelesa::FADE_STATE_FADE_OUT)
				self->mFadeState = TTelesa::FADE_STATE_FADE_IN;
			self->onHitFlag(HIT_FLAG_NO_COLLISION);
		}

		if (self->getUnk184()) {
			gpMarioParticleManager->emit(0xCD, &self->mPosition, 0, nullptr);
		} else {
			self->setBckAnm(2);
		}
	}

	self->reduceFlyForce();

	if (self->checkCurAnmEnd(0) || self->getUnk184()) {
		self->onLiveFlag(LIVE_FLAG_DEAD);
		self->onLiveFlag(LIVE_FLAG_UNK8);
		self->offLiveFlag(LIVE_FLAG_UNK10000);
		self->mHolder = nullptr;
		self->onHitFlag(HIT_FLAG_NO_COLLISION);
		self->stopAnmSound();
		spine->reset();
		spine->setNext(&TNerveTelesaDie::theNerve());
		spine->pushAfterCurrent(spine->getDefault());

		if (!self->getUnk184()) {
			self->genRandomItem();
			TTelesaManager* manager = (TTelesaManager*)self->getManager();
			manager->generatePetBottle(self);

			gpMarioParticleManager->emitAndBindToPosPtr(0xCE, &self->mPosition,
			                                            0, nullptr);
		}

		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveTelesaFreeze, TLiveActor)
{

	TTelesa* self = (TTelesa*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setBckAnm(5);
		self->setGoalPathMario();
	} else if (self->checkCurAnmEnd(0)) {
		if (self->isBckAnm(4)) {
			if (!self->isFlying()) {
				self->unk1C8 = 0;
				self->offHitFlag(HIT_FLAG_UNK10000000);
				return true;
			}
		} else if (self->resetBaseGround() || self->isBckAnm(5))
			self->setBckAnm(3);
		else
			self->setBckAnm(4);
	}

	self->reduceFlyForce();
	self->walkToCurPathNode(0.0f, self->getTurnSpeed() * 0.2f, 0.0f);

	return false;
}

DEFINE_NERVE(TNerveTelesaAttackMario, TLiveActor)
{
	TTelesa* self = (TTelesa*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setBckAnm(6);
		self->setFirstAttackPoint();
	}

	if (spine->getTime() == 10)
		self->offLiveFlag(LIVE_FLAG_HIDDEN);

	if (self->isReachedToGoalXZ())
		self->setAttackPoint();

	self->walkBehavior(2, 2.0f);

	return false;
}

DEFINE_NERVE(TNerveTelesaTrample, TLiveActor)
{
	TTelesa* self = (TTelesa*)spine->getBody();

	if (spine->getTime() == 0)
		self->setBckAnm(1);
	else if (self->checkCurAnmEnd(0))
		return true;

	return false;
}

void TKageMarioModokiManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TWalkerEnemyParams("/enemy/kagemariomodoki.prm");
	TSmallEnemyManager::load(stream);
}

TSmallEnemy* TKageMarioModokiManager::createEnemyInstance()
{
	return new TKageMarioModoki;
}

void TKageMarioModokiManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "default.bmd", 0x11020000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TKageMarioModoki::load(JSUMemoryInputStream& stream)
{
	TSmallEnemy::load(stream);
	reset();
}

void TKageMarioModoki::init(TLiveManager* manager)
{
	TWalkerEnemy::init(manager);
	mSpine->initWith(&TNerveKageMarioModokiWait::theNerve());
	mMActor->resetDL();
	mMActor->setLightType(3);

	TScreenTexture* tex
	    = JDrama::TNameRefGen::search<TScreenTexture>("スクリーンテクスチャ");
	const ResTIMG* img = tex->getTexture()->getTexInfo();
	SMS_ChangeTextureAll(mMActor->getModel()->getModelData(),
	                     "H_kagemario_dummy", *img);
}

DEFINE_NERVE(TNerveKageMarioModokiWait, TLiveActor)
{
	TKageMarioModoki* self = (TKageMarioModoki*)spine->getBody();

	if (spine->getTime() == 0) {
		self->getMActor()->setBck("ma_wait");
		self->setGoalPath(TPathNode(SMS_GetMarioPos()));
	}

	if (!self->checkLiveFlag(LIVE_FLAG_DEAD) && self->isFindMario(1.0f)) {
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToPosPtr(
		        0xCD, &self->mPosition, 0, nullptr)) {
			emitter->setScale(JGeometry::TVec3<f32>(2.0f, 2.0f, 2.0f));
		}

		self->onLiveFlag(LIVE_FLAG_DEAD);
		self->onHitFlag(HIT_FLAG_NO_COLLISION);
		TTelesa* telesa = (TTelesa*)gpConductor->makeOneEnemyAppear(
		    self->mPosition, "テレサマネージャー", 1);

		if (telesa) {
			telesa->reset();
			telesa->unk124->unk0 = self->unk124->unk0;
		}

		return true;
	}

	self->walkBehavior(3, 1.0f);
	return false;
}

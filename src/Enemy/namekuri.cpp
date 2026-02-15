#include "JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp"
#include <Enemy/NameKuri.hpp>
#include <Enemy/Walker.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/Spider.hpp>
#include <Enemy/Conductor.hpp>
#include <Player/MarioAccess.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <Map/MapData.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <MarioUtil/ScreenUtil.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <Strategic/SharedParts.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <System/EmitterViewObj.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DNode.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

f32 TNameKuriManager::mExplosionSpeed    = 1.2f;
int TNameKuriManager::mStopMinScaleFrame = 8;

static TNameKuri* gpCurNameKuri;

const char* namekuri2_bastable[] = {
	nullptr, nullptr, nullptr, "/scene/namekuri2/bas/name_jump_start.base",
	nullptr, nullptr, nullptr, nullptr,
};

void TNameKuriLauncher::stateLaunch()
{
	if (mTicksSpentInCurState == 0) {
		TSpineEnemy* enemy = getProperEnemy("ナメクリマネージャー");
		if (enemy) {
			Mtx mtx;
			MsMtxSetRotRPH(mtx, mRotation.x, mRotation.y, mRotation.z);

			JGeometry::TVec3<f32> local_14;
			JGeometry::TVec3<f32> local_20;

			local_20.set(0.0f, 4.0f, 0.0f);
			local_14.set(0.0f, 0.0f, 0.0f);
			MTXMultVec(mtx, &local_20, &local_20);

			enemy->resetSRTV(mPosition, local_14, enemy->mScaling, local_20);
		}
	}

	if (mMActor->curAnmEndsNext()) {
		resetLaunchTimer();
		changeState(STATE_NORMAL);
	}
}

TNameKuriSaveLoadParams::TNameKuriSaveLoadParams(const char* path)
    : TWalkerEnemyParams(path)
    , PARAM_INIT(mSLAliveTime, 2000)
    , PARAM_INIT(mSLLandHeight, 500.0f)
    , PARAM_INIT(mSLJumpAttackGravity, 0.1f)
    , PARAM_INIT(mSLJumpAttackSp, 20.0f)
    , PARAM_INIT(mSLJumpAttackTurnSp, 2.0f)
    , PARAM_INIT(mSLJumpPrepareFrame, 100)
    , PARAM_INIT(mSLJumpMaxFrame, 30.0f)
    , PARAM_INIT(mSLJumpMaxAngle, 30.0f)
    , PARAM_INIT(mSLJumpMaxScale, 0.2f)
    , PARAM_INIT(mSLColorChangeRate, 25)
    , PARAM_INIT(mSLJumpAttackRadius, 400.0f)
    , PARAM_INIT(mSLJumpAttackAngle, 180.0f)
{
	TParams::load(mPrmPath);
}

TNameKuriManager::TNameKuriManager(const char* name)
    : TSmallEnemyManager(name)
    , unk60(0)
{
	unk5C         = 0;
	gpCurNameKuri = nullptr;
}

void TNameKuriManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TNameKuriSaveLoadParams("/enemy/namekuri.prm");
	TSmallEnemyManager::load(stream);
}

void TNameKuriManager::loadAfter()
{
	TSmallEnemyManager::loadAfter();
	TMapObjBaseManager::newAndRegisterObj("bottle_short");
}

static const GXColorS10 nameKuriTevColorData[7] = {
	{ 0x64, 0x0, 0xA, 0xA9 },  { 0x0, 0x0, 0x64, 0xA9 },
	{ 0x5D, 0x54, 0x0, 0xA9 }, { 0x0, 0x4C, 0x10, 0xA9 },
	{ 0x63, 0x4, 0x60, 0xA9 }, { 0x0, 0x44, 0x75, 0xA9 },
	{ 0x64, 0x28, 0x0, 0xA9 },
};

void TNameKuriManager::initSetEnemies()
{
	void* brainBmd = JKRGetResource("/scene/namekuri2/brain.bmd");
	SDLModelData* brainModel
	    = new SDLModelData(J3DModelLoaderDataBase::load(brainBmd, 0x10210000));

	if (unk18[0] != nullptr) {
		s32 idx = ((TNameKuri*)unk18[0])
		              ->getModel()
		              ->getModelData()
		              ->getMaterialName()
		              ->getIndex("_mat_body");

		const ResTIMG* pollutionMark = (const ResTIMG*)JKRGetResource(
		    "/scene/map/pollution/H_ma_rak.bti");

		int colorIdx = 0;
		for (int i = 0; i < mObjNum; ++i) {
			TNameKuri* nameKuri = (TNameKuri*)unk18[i];

			nameKuri->unk1C4 = nameKuriTevColorData[colorIdx];

			SMS_InitPacket_TwoTevColor(getObj(i)->getMActor()->getModel(), idx,
			                           GX_TEVREG0, &nameKuri->unk1C4,
			                           GX_TEVREG1, &nameKuri->unk1BC);

			nameKuri->unk1CC = new TSharedParts(nameKuri, 2, brainModel, 3);

			nameKuri->unk1CC->getConnectedMtx();
			if (pollutionMark)
				SMS_ChangeTextureAll(
				    nameKuri->unk1CC->getMActor()->getModel()->getModelData(),
				    "K_name_dummy", *pollutionMark);

			++colorIdx;
			if (colorIdx >= 7)
				colorIdx = 0;
		}
	}
}

TSmallEnemy* TNameKuriManager::createEnemyInstance() { return new TNameKuri; }

void TNameKuriManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "name.bmd", 0x15300000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TNameKuriManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TEnemyManager::perform(param_1, param_2);
}

TNameIndParCallback::TNameIndParCallback(TNameKuri* owner)
    : mOwner(owner)
{
}

void TNameIndParCallback::execute(JPABaseEmitter* param_1,
                                  JPABaseParticle* param_2)
{
	if (mOwner->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		MtxPtr mA = mOwner->getMActor()->getModel()->getAnmMtx(1);

		f32 s = JMASin(mOwner->unk1AC);
		f32 c = JMACos(mOwner->unk1AC);

		Mtx local_4c;
		local_4c[0][0] = 1.0f;
		local_4c[0][1] = 0.0f;
		local_4c[0][2] = 0.0f;
		local_4c[0][3] = 0.0f;

		local_4c[1][0] = 0.0f;
		local_4c[1][1] = c;
		local_4c[1][2] = -s;
		local_4c[1][3] = 0.0f;

		local_4c[2][0] = 0.0f;
		local_4c[2][1] = s;
		local_4c[2][2] = c;
		local_4c[2][3] = 0.0f;

		MTXConcat(mA, local_4c, mA);

		// TODO: definitely some inlines missing. Row/column extract func?
		// Maybe a function to extract the scale vector from a matrix?
		JGeometry::TVec3<f32> local_7c;

		JGeometry::TVec3<f32> tmp1(mA[0][0], mA[1][0], mA[2][0]);
		local_7c.y = tmp1.length();
		JGeometry::TVec3<f32> tmp2(mA[0][1], mA[1][1], mA[2][1]);
		local_7c.z = tmp2.length();
		JGeometry::TVec3<f32> tmp3(mA[0][2], mA[1][2], mA[2][2]);
		local_7c.x = tmp3.length();

		param_1->setGlobalRTMatrix(mA);

		if (mOwner->unk1A8) {
			param_1->setScale(mOwner->mScaling);
		} else {
			if (mOwner->isAttackJump())
				param_1->setScale(local_7c * 0.5f);
			else
				param_1->setScale(local_7c);
		}
	}
}

void TNameIndParCallback::draw(JPABaseEmitter*, JPABaseParticle*) { }

BOOL NameKuriAttackCallback(J3DNode* param_1, int param_2)
{
	if (param_2 == 0) {
		if (gpCurNameKuri == nullptr || !gpCurNameKuri->isAttackJump())
			return true;

		MtxPtr mA = gpCurNameKuri->getMActor()->getModel()->getAnmMtx(
		    ((J3DJoint*)param_1)->getJntNo());

		f32 s = JMASin(gpCurNameKuri->unk1AC);
		f32 c = JMACos(gpCurNameKuri->unk1AC);

		Mtx local_48;
		local_48[0][0] = 1.0f;
		local_48[0][1] = 0.0f;
		local_48[0][2] = 0.0f;
		local_48[0][3] = 0.0f;

		local_48[1][0] = 0.0f;
		local_48[1][1] = c;
		local_48[1][2] = -s;
		local_48[1][3] = 0.0f;

		local_48[2][0] = 0.0f;
		local_48[2][1] = s;
		local_48[2][2] = c;
		local_48[2][3] = 0.0f;

		MTXConcat(mA, local_48, mA);
		MTXConcat(J3DSys::mCurrentMtx, local_48, J3DSys::mCurrentMtx);
	}

	return true;
}

BOOL NameKuriScaleCallback(J3DNode* param_1, int param_2)
{
	if (param_2 == 0) {
		if (gpCurNameKuri == nullptr || !gpCurNameKuri->isHitWaterJump())
			return true;

		MtxPtr mA = gpCurNameKuri->getMActor()->getModel()->getAnmMtx(
		    ((J3DJoint*)param_1)->getJntNo());

		Mtx local_3c;

		local_3c[0][0] = 0.4;
		local_3c[0][1] = 0.0;
		local_3c[0][2] = 0.0;
		local_3c[0][3] = 0.0;

		local_3c[1][0] = 0.0;
		local_3c[1][1] = 4.0;
		local_3c[1][2] = 0.0;
		local_3c[1][3] = 0.0;

		local_3c[2][0] = 0.0;
		local_3c[2][1] = 0.0;
		local_3c[2][2] = 0.4;
		local_3c[2][3] = 0.0;

		MTXConcat(mA, local_3c, mA);
		MTXConcat(J3DSys::mCurrentMtx, local_3c, J3DSys::mCurrentMtx);
	}

	return true;
}

TNameKuri::TNameKuri(const char* name)
    : TWalkerEnemy(name)
    , unk194(0)
    , unk198(0)
    , unk19C(this)
    , unk1A4(0)
    , unk1CC(nullptr)
{
}

void TNameKuri::init(TLiveManager* param_1)
{
	JKRHeap::getCurrentHeap()->getTotalFreeSize();

	TWalkerEnemy::init(param_1);
	onHitFlag(0x40000000);
	mActorType = 0x10000003;
	unk150     = 0x3A;
	getWalker()->setMode(1);
	unk130 = 2;
	unk1A4 = (TNameKuriSaveLoadParams*)getSaveParam();

	if (mInstanceIndex == 0) {
		for (u8 i = 0;
		     i < getMActor()->getModel()->getModelData()->getJointNum(); ++i) {
			;
		}
	}

	getMActor()->setJointCallback(1, &NameKuriAttackCallback);
	getMActor()->setJointCallback(1, &NameKuriScaleCallback);
	getMActor()->resetDL();
	getMActor()->setLightType(3);
	TScreenTexture* tex
	    = JDrama::TNameRefGen::search<TScreenTexture>("スクリーンテクスチャ");

	SMS_ChangeTextureAll(getMActor()->getModel()->getModelData(),
	                     "H_ma_rak_dummy", *tex->getTexture()->getTexInfo());

	unk1BC.b = 0;
	unk1BC.g = 0;
	unk1BC.r = 0;
	unk1BC.a = 168;

	if (unk124->unk0 == nullptr || unk124->unk0->isDummy()) {
		unk124->unk0 = gpConductor->getGraphByName("main");
	}
}

void TNameKuri::calcRootMatrix()
{
	gpCurNameKuri = this;

	if (isEaten())
		return;

	getModel()->setBaseScale(mScaling);

	MtxPtr anmMtx = getModel()->getBaseTRMtx();

	unk1A8 = false;

	if (getWalker()->unk2C->unk10 > 0.0f && unk138 != nullptr) {
		unk1A8 = true;
		JGeometry::TVec3<f32> local_30(0.0f, 1.0f, 0.0f);

		JGeometry::TVec3<f32> normal = mGroundPlane->getNormal();

		JGeometry::TVec3<f32> local_a0;
		local_a0.cross(normal, local_30);
		MsVECNormalize(&local_a0, &local_a0);

		local_30.cross(local_a0, normal);
		MsVECNormalize(&local_30, &local_30);

		anmMtx[0][0] = local_a0.x;
		anmMtx[1][0] = local_a0.y;
		anmMtx[2][0] = local_a0.z;

		anmMtx[0][1] = normal.x;
		anmMtx[1][1] = normal.y;
		anmMtx[2][1] = normal.z;

		anmMtx[0][2] = local_30.x;
		anmMtx[1][2] = local_30.y;
		anmMtx[2][2] = local_30.z;

		anmMtx[0][3] = 0;
		anmMtx[1][3] = 0;
		anmMtx[2][3] = 0;

		f32 angle = (1.0f - getWalker()->unk2C->unk10) * 90.0f;

		f32 s = JMASin(angle);
		f32 c = JMACos(angle);

		Mtx local_7c;

		local_7c[0][0] = 1.0f;
		local_7c[1][0] = 0.0f;
		local_7c[2][0] = 0.0f;

		local_7c[0][1] = 0.0f;
		local_7c[1][1] = c;
		local_7c[2][1] = s;

		local_7c[0][2] = 0.0f;
		local_7c[1][2] = -s;
		local_7c[2][2] = c;

		local_7c[0][3] = 0.0f;
		local_7c[1][3] = 0.0f;
		local_7c[2][3] = 0.0f;

		MTXConcat(anmMtx, local_7c, anmMtx);
	} else {
		JGeometry::TVec3<f32> local_88(JMASin(mRotation.y), 0.0f,
		                               JMACos(mRotation.y));

		JGeometry::TVec3<f32> normal = mGroundPlane->getNormal();

		JGeometry::TVec3<f32> local_a0;
		local_a0.cross(normal, local_88);
		MsVECNormalize(&local_a0, &local_a0);

		local_88.cross(local_a0, normal);
		MsVECNormalize(&local_88, &local_88);

		anmMtx[0][0] = local_a0.x;
		anmMtx[1][0] = local_a0.y;
		anmMtx[2][0] = local_a0.z;

		anmMtx[0][1] = normal.x;
		anmMtx[1][1] = normal.y;
		anmMtx[2][1] = normal.z;

		anmMtx[0][2] = local_88.x;
		anmMtx[1][2] = local_88.y;
		anmMtx[2][2] = local_88.z;
	}

	anmMtx[0][3] = mPosition.x;
	anmMtx[1][3] = mPosition.y;
	anmMtx[2][3] = mPosition.z;
}

void TNameKuri::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TWalkerEnemy::perform(param_1, param_2);
	unk1CC->perform(param_1, param_2);
	if (param_1 & 2) {
		Mtx afStack_50;
		SMS_GetLightPerspectiveForEffectMtx(afStack_50);

		// TODO: need one more inline here somewhere?
		mMActor->getModel()
		    ->getModelData()
		    ->getMaterialNodePointer(0)
		    ->getTexGenBlock()
		    ->getTexMtx(2)
		    ->setEffectMtx(afStack_50);
	}
}

void TNameKuri::moveObject()
{
	TWalkerEnemy::moveObject();

	if (mVelocity.y < 0.0f
	    && (mSpine->getCurrentNerve() == &TNerveWalkerGraphWander::theNerve()
	        || mSpine->getCurrentNerve() == &TNerveWalkerEscape::theNerve())
	    && mPosition.y - mGroundHeight > unk1A4->mSLLandHeight.get()) {
		mSpine->pushNerve(&TNerveNameKuriLand::theNerve());
	}

	if (!isAirborne() && isBckAnm(7)) {
		if (gpMSound->gateCheck(0x2006))
			MSoundSESystem::MSoundSE::startSoundActor(0x2006, mPosition, 0,
			                                          nullptr, 0, 4);
	}

	if (!checkLiveFlag(LIVE_FLAG_HIDDEN)) {
		++unk194;
		int aliveTime = unk1A4->mSLAliveTime.get();
		if (unk194 + 100 > aliveTime && mScaling.x < mBodyScale * 2.0f) {
			mScaling.x = mScaling.y = mScaling.z = 1.01f * mScaling.z;
		}

		if (unk194 > aliveTime)
			kill();
	}
}

void TNameKuri::setBehavior() { }

void TNameKuri::behaveToWater(THitActor* param_1)
{
	if (mSpine->getCurrentNerve() != &TNerveSmallEnemyHitWaterJump::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveSmallEnemyDie::theNerve()) {

		mSpine->pushNerve(&TNerveNameKuriExplosion::theNerve());

		if (!isAirborne())
			onLiveFlag(0x10000);

		onLiveFlag(0x20000);
	}
}

f32 TNameKuri::getGravityY() const
{
	if (checkLiveFlag(0x10000))
		return 0.01f;

	if (mSpine->getCurrentNerve() == &TNerveNameKuriJumpAttack::theNerve())
		return unk1A4->mSLJumpAttackGravity.get();
	else
		return mGravity;
}

void TNameKuri::setBckAnm(int param_1) { TWalkerEnemy::setBckAnm(param_1); }

void TNameKuri::setGenerateAnm()
{
	setBckAnm(5);
	setVelocity(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));
}

void TNameKuri::setWalkAnm() { setBckAnm(7); }

void TNameKuri::setDeadAnm()
{
	setBckAnm(0);

	if (gpMSound->gateCheck(0x2800))
		MSoundSESystem::MSoundSE::startSoundActor(0x2800, &mPosition, 0,
		                                          nullptr, 0, 4);

	MtxPtr mtx = getMActor()->getModel()->getAnmMtx(2);

	if (JPABaseEmitter* emitter
	    = gpMarioParticleManager->emitAndBindToMtxPtr(0x84, mtx, 0, nullptr)) {

		emitter->unk154.set(mScaling);
		emitter->unk174.set(mScaling);
	}

	if (JPABaseEmitter* emitter
	    = gpMarioParticleManager->emitAndBindToMtxPtr(0x83, mtx, 0, nullptr)) {

		emitter->unk154.set(mScaling);
		emitter->unk174.set(mScaling);
	}

	setVelocity(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));
	onLiveFlag(LIVE_FLAG_UNK10);
}

void TNameKuri::setAfterDeadEffect()
{
	if (unk198) {
		void* waterGun = SMS_GetMarioWaterGun();
		// TODO: where my water gun
	}
}

void TNameKuri::setWaitAnm() { setBckAnm(6); }

void TNameKuri::setMeltAnm()
{
	setBckAnm(0);

	MtxPtr mtx = getMActor()->getModel()->getAnmMtx(2);

	if (JPABaseEmitter* emitter
	    = gpMarioParticleManager->emitAndBindToMtxPtr(0x84, mtx, 0, nullptr)) {

		emitter->unk154.set(mScaling);
		emitter->unk174.set(mScaling);
	}

	if (JPABaseEmitter* emitter
	    = gpMarioParticleManager->emitAndBindToMtxPtr(0x83, mtx, 0, nullptr)) {

		emitter->unk154.set(mScaling);
		emitter->unk174.set(mScaling);
	}

	setVelocity(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));
	onLiveFlag(LIVE_FLAG_UNK10);

	if (gpMSound->gateCheck(0x2802))
		MSoundSESystem::MSoundSE::startSoundActor(0x2802, &mPosition, 0,
		                                          nullptr, 0, 4);
}

void TNameKuri::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("name.bmd", 0);
}

void TNameKuri::reset()
{
	gpCurNameKuri = this;
	TWalkerEnemy::reset();
	unk1B4 = MsRandF(0.0f, 360.0f);
	unk194 = 0;
	unk198 = 0;
	setVelocity(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));
	mScaling.x = mBodyScale;
	mScaling.y = mBodyScale;
	mScaling.z = mBodyScale;

	unk1BC.a = 0;
	unk1BC.b = 0;
	unk1BC.g = 0;
	unk1BC.r = 0;

	// TODO: inline?
	TNameKuriManager* man = ((TNameKuriManager*)mManager);
	man->unk60 += 1;
	if (man->unk60 >= 7)
		man->unk60 = 0;
	unk1C4 = nameKuriTevColorData[man->unk60];

	offLiveFlag(0x10);
}

void TNameKuri::attackToMario()
{
	SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
	if (mSpine->getCurrentNerve() == &TNerveNameKuriJumpAttack::theNerve()) {
		setVelocity(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));
		onLiveFlag(0x20000);
		kill();
	}
}

void TNameKuri::behaveToFindMario()
{
	mSpine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
	mSpine->pushAfterCurrent(&TNerveNKFollowMario::theNerve());
}

void TNameKuri::endHitWaterJump()
{
	mScaling.set(mBodyScale, mBodyScale, mBodyScale);
}

bool TNameKuri::isAttackJump() const
{
	if (mSpine->getCurrentNerve() == &TNerveNameKuriJumpAttack::theNerve()
	    && mPosition.y > mGroundHeight + 5.0f && !unk1A8)
		return true;
	else
		return false;
}

bool TNameKuri::isHitWaterJump() const
{
	if (mSpine->getCurrentNerve() == &TNerveSmallEnemyHitWaterJump::theNerve())
		return true;
	else
		return false;
}

bool TNameKuri::canJumpAttack() const { }

bool TNameKuri::isHitValid(u32 param_1)
{
	unk198 = 1;
	if (param_1 == 15)
		unk198 = 0;

	if (checkLiveFlag(2))
		return false;
	else
		return true;
}

bool TNameKuri::isCollidMove(THitActor* param_1)
{
	if ((param_1->getActorType() & 0xffff0000) == 0x40000000) {
		if (mSpine->getCurrentNerve() == &TNerveNameKuriJumpAttack::theNerve())
			kill();
		return true;
	}

	if (!((TLiveActor*)param_1)->isAirborne() && !isAirborne())
		return true;

	return false;
}

const char** TNameKuri::getBasNameTable() const { return namekuri2_bastable; }

void TDiffusionNameKuriManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TNameKuriSaveLoadParams("/enemy/diffusionnamekuri.prm");
	TSmallEnemyManager::load(stream);
}

TSmallEnemy* TDiffusionNameKuriManager::createEnemyInstance()
{
	return new TDiffusionNameKuri;
}

DEFINE_NERVE(TNerveNameKuriLand, TLiveActor)
{
	TNameKuri* self = (TNameKuri*)spine->getBody();

	if (self->isBckAnm(4) && self->checkCurAnmEnd(0))
		return true;

	if (!self->isAirborne())
		self->setBckAnm(4);

	return false;
}

DEFINE_NERVE(TNerveNameKuriJumpAttack, TLiveActor)
{
	TNameKuri* self = (TNameKuri*)spine->getBody();

	if (spine->getTime() < 2) {
		// TODO: WTF?! Why does it use SMS_GetMarioHitActor here
		// but directly use gpMarioAddress everywhere else?
		self->setGoalPathMario();

		self->unk1B0 = self->mScaling.y;
		self->unk1AC = 0.0f;
		self->unk1B8 = 0.0f;
	} else {
		TNameKuriSaveLoadParams* params = self->unk1A4;
		if (self->isBckAnm(6)) {
			if (spine->getTime() > 0) {
				self->setBckAnm(3);
			} else {
				f32 jumpAttackTurnSp = params->mSLJumpAttackTurnSp.get();
				self->walkToCurPathNode(0.0f, jumpAttackTurnSp, 0.0f);
			}
		} else if (self->isBckAnm(3)) {
			if (self->getMActor()->getFrameCtrl(0)->checkPass(62.0f)) {
				JGeometry::TVec3<f32> local_44 = SMS_GetMarioPos();
				f32 jumpAttackSp = self->unk1A4->mSLJumpAttackSp.get();
				f32 grav         = self->getGravityY();
				JGeometry::TVec3<f32> local_6c
				    = self->calcVelocityToJumpToY(local_44, jumpAttackSp, grav);
				self->mPosition.y += 2.0f;
				self->setVelocity(local_6c);
				self->onLiveFlag(LIVE_FLAG_AIRBORNE);
				self->unk1AC = 0.0f;
				self->unk1B8 = 0.0f;

				// TODO: It's different here too?!
				self->setGoalPathMario();
			}

			self->walkToCurPathNode(0.0f, 6.0f, 0.0f);
			int colorChangeRate = self->unk1A4->mSLColorChangeRate.get();
			if (self->getCurAnmFrameNo(0) > 62.0f) {
				self->unk1B8 += 1.0f;
				s16 sVar5
				    = MsClamp<s16>(self->unk1BC.r - colorChangeRate, 0, 255);
				self->unk1BC.r = self->unk1BC.g = self->unk1BC.b = sVar5;
			} else {
				s16 col
				    = abs(JMASin(colorChangeRate * spine->getTime())) * 255.0f;
				self->unk1BC.r = self->unk1BC.g = self->unk1BC.b = col;
			}

			if (self->isAirborne())
				self->mScaling.x = self->unk1B0;

			if (!self->isAirborne() && self->checkCurAnmEnd(0))
				self->setBckAnm(2);
		} else if (self->isBckAnm(2)) {
			if (self->checkCurAnmEnd(0)) {
				self->switchNextGoalPath();
				self->onLiveFlag(LIVE_FLAG_UNK20000);
				spine->pushAfterCurrent(&TNerveSmallEnemyDie::theNerve());
				return true;
			}
			f32 jumpMaxAngle = self->unk1A4->mSLJumpMaxAngle.get();
			self->unk1AC = MsClamp(self->unk1AC * 0.9f, -jumpMaxAngle, 0.0f);
		}
	}

	return false;
}

DEFINE_NERVE(TNerveNameKuriJumpAttackPrepare, TLiveActor)
{
	TNameKuri* self = (TNameKuri*)spine->getBody();
	if (spine->getTime() == 1) {
		self->setGoalPathMario();
		self->setBckAnm(6);
	} else if (self->isBckAnm(6)) {
		if (self->isInSight(SMS_GetMarioPos(), 100000.0f, 30.0f, 0.0f)
		    && !self->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
			if (!self->isAirborne() && !self->unk1A8) {
				spine->pushAfterCurrent(&TNerveNameKuriJumpAttack::theNerve());
				return true;
			}
		} else {
			self->walkToCurPathNode(
			    0.0f, self->unk1A4->mSLJumpAttackTurnSp.get(), 0.0f);
		}
	}

	return false;
}

DEFINE_NERVE(TNerveNameKuriExplosion, TLiveActor)
{
	TNameKuri* self = (TNameKuri*)spine->getBody();
	if (spine->getTime() < TNameKuriManager::mStopMinScaleFrame) {
		self->mScaling.x = self->mScaling.y = self->mScaling.z
		    = self->mBodyScale * 0.5f;
	} else {
		if (self->mScaling.x < self->mBodyScale * 2.0f)
			self->mScaling.x = self->mScaling.y = self->mScaling.z
			    = self->mScaling.z * TNameKuriManager::mExplosionSpeed;
		else
			return true;
	}

	return false;
}

DEFINE_NERVE(TNerveNameKuriDiffuse, TLiveActor)
{
	TNameKuri* self = (TNameKuri*)spine->getBody();
	if (!self->isAirborne())
		return true;

	return false;
}

DEFINE_NERVE(TNerveNameKuriDrawPollute, TLiveActor) { return true; }

DEFINE_NERVE(TNerveNKFollowMario, TLiveActor)
{
	TNameKuri* self = (TNameKuri*)spine->getBody();

	if (spine->getTime() == 0)
		self->setGoalPathMario();

	self->walkToCurPathNode(self->mMarchSpeed, 3.0f, 0.0f);

	TNameKuriSaveLoadParams* params
	    = ((TNameKuriSaveLoadParams*)self->getSaveParam());
	f32 jumpAttackRadius = params->mSLJumpAttackRadius.get();
	f32 jumpAttackAngle  = params->mSLJumpAttackAngle.get();
	if (self->isInSight(SMS_GetMarioPos(), jumpAttackRadius, jumpAttackAngle,
	                    0.0f)) {
		spine->pushAfterCurrent(&TNerveNameKuriJumpAttackPrepare::theNerve());
		return true;
	}

	return false;
}

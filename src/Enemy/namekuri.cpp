#include <Enemy/NameKuri.hpp>
#include <Enemy/Walker.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/Conductor.hpp>
#include <Player/MarioAccess.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <MarioUtil/ScreenUtil.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <Strategic/SharedParts.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DNode.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

f32 TNameKuriManager::mExplosionSpeed    = 1.2f;
u32 TNameKuriManager::mStopMinScaleFrame = 8;

static TNameKuri* gpCurNameKuri;

const char* namekuri2_bastable[] = {
	nullptr, nullptr, nullptr, "/scene/namekuri2/bas/name_jump_start.base",
	nullptr, nullptr, nullptr, nullptr,
};

void TNameKuriLauncher::stateLaunch() { }

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

void TNameIndParCallback::execute(JPABaseEmitter*, JPABaseParticle*) { }

void TNameIndParCallback::draw(JPABaseEmitter*, JPABaseParticle*) { }

BOOL NameKuriAttackCallback(J3DNode*, int) { }

BOOL NameKuriScaleCallback(J3DNode*, int) { }

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

void TNameKuri::calcRootMatrix() { }

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

	if (!checkLiveFlag(LIVE_FLAG_UNK2)) {
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

void TNameKuri::setDeadAnm() { }

void TNameKuri::setAfterDeadEffect() { }

void TNameKuri::setWaitAnm() { setBckAnm(6); }

void TNameKuri::setMeltAnm() { }

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
	SMS_SendMessageToMario(this, 0xE);
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

void TNameKuri::isAttackJump() const { }

void TNameKuri::isHitWaterJump() const { }

void TNameKuri::canJumpAttack() const { }

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

DEFINE_NERVE(TNerveNameKuriLand, TLiveActor) { }

DEFINE_NERVE(TNerveNameKuriJumpAttack, TLiveActor) { }

DEFINE_NERVE(TNerveNameKuriJumpAttackPrepare, TLiveActor) { }

DEFINE_NERVE(TNerveNameKuriExplosion, TLiveActor) { }

DEFINE_NERVE(TNerveNameKuriDiffuse, TLiveActor) { }

DEFINE_NERVE(TNerveNameKuriDrawPollute, TLiveActor) { return true; }

DEFINE_NERVE(TNerveNKFollowMario, TLiveActor) { }

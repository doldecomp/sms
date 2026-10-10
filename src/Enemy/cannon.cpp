#include <Enemy/BombHei.hpp>
#include <Enemy/Cannon.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/EffectObj.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/Igaiga.hpp>
#include <Enemy/Killer.hpp>
#include <Enemy/Popo.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <MSound/MAnmSound.hpp>
#include <MSound/MSound.hpp>
#include <MSound/SoundEffects.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/Strategy.hpp>
#include <System/FlagManager.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/Application.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <macros.h>
#include <math.h>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

u8 TCannon::mChorobeiJntIdx[0x1]     = { 4 };
u8 TCannon::mChorobeiHandJntIdx[0x1] = { 4 };
f32 TCannon::mVelocityRate           = 0.62f;
f32 TCannon::mSearchRate             = 0.02f;

TCannonSaveLoadParams::TCannonSaveLoadParams(const char* param_1)
    : TSmallEnemyParams(param_1)
    , PARAM_INIT(mSLHideDist, 300.0f)
    , PARAM_INIT(mSLBombDist, 2000.0f)
    , PARAM_INIT(mSLKillerDist, 10000.0f)
    , PARAM_INIT(mSLBombInterval, 100)
    , PARAM_INIT(mSLKillerInterval, 50)
    , PARAM_INIT(mSLShootInterval, 100)
    , PARAM_INIT(mSLChorobeiAttackRadius, 100.0f)
    , PARAM_INIT(mSLChorobeiAttackHeight, 100.0f)
    , PARAM_INIT(mSLChorobeiDamageRadius, 100.0f)
    , PARAM_INIT(mSLChorobeiDamageHeight, 100.0f)
    , PARAM_INIT(mSLKillerTransYOffset, -50.0f)
    , PARAM_INIT(mSLBombHeiGenerateRate, 0.7f)
    , PARAM_INIT(mSLThrowXZSpeed, 12.0f)
{
	TParams::load(mPrmPath);
}

TCannonManager::TCannonManager(const char* param_1)
    : TSmallEnemyManager(param_1)
{
}

void TCannonManager::load(JSUMemoryInputStream& param_1)
{
	TSmallEnemyManager::load(param_1);
	unk38 = new TCannonSaveLoadParams("/enemy/cannon.prm");
}

TSpineEnemy* TCannonManager::createEnemyInstance() { return new TCannon; }

TChorobei::TChorobei(TCannon* param_1, int param_2, const char* param_3)
    : THitActor(param_3)
    , unk68(param_1)
    , unk6C(nullptr)
    , unk70(0.0f)
    , unk74(nullptr)
    , unk78(nullptr)
    , unk7C(300.0f)
{
	unk6C = new TSharedParts(unk68, param_2, "/scene/cannon/tyorobe_model1.bmd",
	                         0x10020000, 3);

	if (unk74)
		return;

	MAnmSound* anmSound = new MAnmSound(SMSGetMSound());
	unk74               = anmSound;
	unk74->initAnmSound(nullptr, 1, 0.0f);
}

void TChorobei::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (unk68->checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_HIDDEN
	                         | LIVE_FLAG_CLIPPED_OUT))
		return;

	if (unk70)
		return;

	if (param_1 & CUE_CALC_ANIM) {
		if (unk74 != nullptr && unk78 != nullptr) {
			J3DFrameCtrl* ctrl = unk6C->getMActor()->getFrameCtrl(ANM_TYPE_BCK);
			f32 rate           = ctrl->getRate();
			unk74->animeLoop(&mPosition, ctrl->getFrame(), rate, 0, 4);
		}

		Mtx mtx;
		MTXCopy(unk6C->getConnectedMtx(), mtx);
		mtx[1][3] += unk7C;
		unk6C->getMActor()->getModel()->setBaseTRMtx(mtx);

		mPosition.x = mtx[0][3];
		mPosition.y = mtx[1][3] - 150.0f;
		mPosition.z = mtx[2][3];
	}

	THitActor::perform(param_1, param_2);
	unk6C->getMActor()->perform(param_1, param_2);
}

void TChorobei::setBckAnm(int param_1)
{
	unk6C->getMActor()->setBckFromIndex(param_1);
	unk78 = unk68->getBas(param_1);
	if (unk78 != nullptr)
		unk74->initAnmSound(JKRGetResource(unk78), 1, 0.0f);
	else
		unk74->initAnmSound(nullptr, 1, 0.0f);
}

void TChorobei::checkHit()
{
	for (int i = 0; i < mColCount; i++) {
		THitActor* actor = mCollisions[i];
		if (actor->isActorType(ACTOR_TYPE_MARIO))
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
		if (actor->isActorType(ACTOR_TYPE_BOMB_HEI)) {
			TBombHei* bombHei = static_cast<TBombHei*>(actor);
			unk68->hitHead(bombHei);
		}
		if (actor->isActorType(ACTOR_TYPE_ENEMY_UNK1F)) {
			TKiller* killer = static_cast<TKiller*>(actor);
			if (killer->isRollFly()) {
				const TNerveBase<TLiveActor>* nerve
				    = &TNerveCannonDamage::theNerve();
				unk68->mSpine->pushNerve(nerve);
				killer->kill();
			}
		}
	}
}

BOOL TChorobei::receiveMessage(THitActor* param_1, u32 param_2)
{
	return FALSE;
}

bool TChorobei::isUpEnd()
{
	if (unk6C->getMActor()->curAnmEndsNext()
	    && unk6C->getMActor()->checkCurBckFromIndex(12))
		return true;
	unk70 = 0.0f;
	return false;
}

bool TChorobei::isDownEnd()
{
	if (unk6C->getMActor()->curAnmEndsNext()
	    && unk6C->getMActor()->checkCurBckFromIndex(15)) {
		unk70 = 1.0f;
		return true;
	}
	return false;
}

TCannonDom::TCannonDom(TLiveActor* param_1, int param_2, SDLModelData* param_3,
                       u32 param_4, const char* param_5)
    : TSharedParts(param_1, param_2, param_3, param_4, param_5)
    , unk1C(nullptr)
    , unk20(nullptr)
    , unk24(0)
    , unk28(0.0f)
    , unk2C(0.0f)
    , unk30(0.0f)
{
	TMsRange<f32> range(0.0f, 360.0f);
	unk30 = range.rand();

	if (unk1C)
		return;

	MAnmSound* anmSound = new MAnmSound(SMSGetMSound());
	unk1C               = anmSound;
	unk1C->initAnmSound(nullptr, 1, 0.0f);
}

void TCannonDom::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (unk10->checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_HIDDEN
	                         | LIVE_FLAG_CLIPPED_OUT))
		return;

	if (param_1 == CUE_CALC_ANIM) {
		if (unk1C != nullptr && unk20 != nullptr) {
			J3DFrameCtrl* ctrl = unk18->getFrameCtrl(ANM_TYPE_BCK);
			f32 rate           = ctrl->getRate();
			unk1C->animeLoop((Vec*)&unk10->mPosition, ctrl->getFrame(), rate, 0,
			                 4);
		}

		MtxPtr mtx = getConnectedMtx();

		Mtx rot;
		MsMtxSetRotRPH(rot, unk28, unk2C, 0.0f);
		MTXConcat(mtx, rot, mtx);
		unk18->getModel()->setBaseTRMtx(mtx);
	}

	unk18->perform(param_1, param_2);
}

void TCannonDom::setBckAnm(int param_1)
{
	unk18->setBckFromIndex(param_1);
	unk20 = unk10->getBas(param_1);
	if (unk20 != nullptr)
		unk1C->initAnmSound(JKRGetResource(unk20), 1, 0.0f);
	else
		unk1C->initAnmSound(nullptr, 1, 0.0f);
}

TCannon::TCannon(const char* param_1)
    : TSmallEnemy(param_1)
    , unk1A0(nullptr)
    , unk1A8(nullptr)
    , unk1B8(nullptr)
    , unk1E0(nullptr)
    , unk214(0)
    , unk218(0)
    , unk21C(0)
    , unk220(0.0f)
    , unk230(1)
    , unk238(0)
    , unk239(1)
    , unk254(nullptr)
    , unk258(nullptr)
{
	unk290    = 0;
	unk2AC    = 0.0f;
	unk224[0] = 0.0f;
	unk224[1] = 0.0f;
	unk224[2] = 0.0f;
}

void TCannon::load(JSUMemoryInputStream& param_1)
{
	TSmallEnemy::load(param_1);
	reset();
	mHitPoints = getMaxHitPoints();
	unk230     = SMSGetApplication()->mCurrArea.getStage();
	unk23C     = mPosition;
}

void TCannon::loadAfter()
{
	SMS_LoadParticle("/scene/cannon/jpa/ms_cannon_a.jpa",
	                 CANNON_JPA_MS_CANNON_A);
	SMS_LoadParticle("/scene/cannon/jpa/ms_cannon_b.jpa",
	                 CANNON_JPA_MS_CANNON_B);
	SMS_LoadParticle("/scene/cannon/jpa/ms_cannon_c.jpa",
	                 CANNON_JPA_MS_CANNON_C);
	SMS_LoadParticle("/scene/cannon/jpa/ms_cannon_d.jpa",
	                 CANNON_JPA_MS_CANNON_D);
	SMS_LoadParticle("/scene/cannon/jpa/ms_cannon_e.jpa",
	                 CANNON_JPA_MS_CANNON_E);
	SMS_LoadParticle("/scene/cannon/jpa/ms_cannon_smoke.jpa",
	                 CANNON_JPA_MS_CANNON_SMOKE);

	if (unk230 == 5) {
		unk254 = static_cast<TMapObjBase*>(
		    JDrama::TNameRefGen::search("efMareGate"));
		unk254->kill();
	}
}

// TODO: nonmatching stack frame (0x190 instead of 0x1a0).
void TCannon::init(TLiveManager* param_1)
{
	TSmallEnemy::init(param_1);
	mActorType = ACTOR_TYPE_DPT_CANNON;
	unk150     = 17;
	unk28C     = static_cast<TCannonSaveLoadParams*>(getSaveParam());
	setBckAnm(3);
	void* resource = JKRGetResource("/scene/cannon/cannon_Dom.bmd");
	SDLModelData* modelData
	    = new SDLModelData(J3DModelLoaderDataBase::load(resource, 0x10050000));
	unk234 = mRotation.y;
	unk230 = SMSGetApplication()->mCurrArea.getStage();
	if (unk230 == 5 || unk230 == 9) {
		mSpine->initWith(&TNerveCannonSearch::theNerve());
		if (unk230 == 9) {
			unk239 = 0;
			setGoalPath(
			    TPathNode(JGeometry::TVec3<f32>(-565.0f, 8500.0f, 7675.0f)));
		} else {
			setGoalPath((THitActor*)gpMarioAddress);
		}

		unk1A8 = new TChorobei(this, 0);
		for (u8 i = 0; i < unk1A8->unk6C->getMActor()
		                       ->getModel()
		                       ->getModelData()
		                       ->getJointNum();
		     ++i) { }
		f32 attackRadius = unk28C->mSLChorobeiAttackRadius.get();
		f32 attackHeight = unk28C->mSLChorobeiAttackHeight.get();
		f32 damageRadius = unk28C->mSLChorobeiDamageRadius.get();
		f32 damageHeight = unk28C->mSLChorobeiDamageHeight.get();
		unk1A8->initHitActor(
		    ACTOR_TYPE_ENEMY_UNK1D, 3, HIT_CATEGORY_PLAYER | HIT_CATEGORY_ENEMY,
		    attackRadius, attackHeight, damageRadius, damageHeight);
		static_cast<TIdxGroupObj*>(JDrama::TNameRefGen::search("敵グループ"))
		    ->getChildren()
		    .push_back(unk1A8);

		static const char* sCannonDomPartsJointTable[] = {
			"nullC",
			"nullB",
			"nullA",
		};
		JUTNameTab* jointTable
		    = getMActor()->getModel()->getModelData()->getJointName();
		for (int i = 0; i < ARRAY_COUNT(unk1AC); ++i) {
			int jointIndex = jointTable->getIndex(sCannonDomPartsJointTable[i]);
			unk1AC[i]      = new TCannonDom(this, jointIndex, modelData, 3);
			unk1C0[i]      = new TMapCollisionMove;
			unk1C0[i]->init("/cannon/CannonDom", 2, this);
			unk1C0[i]->setUpTrans(mPosition);
		}
		unk2B0 = new TMapCollisionMove;
		unk2B0->init("/cannon/CannonFuta", 2, this);
		unk2B0->setUpTrans(mPosition);
	} else {
		mSpine->initWith(&TNerveCannonObject::theNerve());
		int jointIndex
		    = getMActor()->getModel()->getModelData()->getJointName()->getIndex(
		        "nullA");
		unk1B8        = new TCannonDom(this, jointIndex, modelData, 3);
		unk1B8->unk24 = 1;
	}
	resource = JKRGetResource("/scene/cannon/hodai_mario.bmd");
	modelData
	    = new SDLModelData(J3DModelLoaderDataBase::load(resource, 0x10010000));
	unk1BC = new TSharedParts(this, 0, modelData, 3);
	for (u8 i = 0; i < getModel()->getModelData()->getJointNum(); ++i) { }

	unk258 = new TMapCollisionMove;
	unk258->init(2, 0, 0, nullptr);
}

void TCannon::reset()
{
	TSmallEnemy::reset();

	mHitPoints = getMaxHitPoints();
	onHitFilter(HIT_FILTER_NO_COLLISION);
	unk214      = 1;
	mHeadHeight = 40.0f;
	onLiveFlag(LIVE_FLAG_UNK10);
	unk2AC = mRotation.y;

	if (unk230 == 9) {
		unk239 = 0;
		setGoalPath(
		    TPathNode(JGeometry::TVec3<f32>(-565.0f, 8500.0f, 7675.0f)));
	}
}

// TODO: frame-only mismatch; velocity and attachment stack slots differ.
void TCannon::moveObject()
{
	TSmallEnemy::moveObject();
	if (unk230 != 5 && unk230 != 9)
		return;

	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		unk1A8->mPosition = mPosition;
	} else {
		MtxPtr mtx          = getModel()->getAnmMtx(mChorobeiJntIdx[0]);
		unk1A8->mPosition.x = mtx[0][3];
		unk1A8->mPosition.y = mtx[1][3] - 100.0f;
		unk1A8->mPosition.z = mtx[2][3];
	}

	unk1A8->checkHit();

	JGeometry::TVec3<f32> vel = getVelocityRef();
	mPosition.y += vel.y;
	mVelocity.y -= getGravityY();
	if (mPosition.y < unk23C.y) {
		mVelocity   = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
		mPosition.y = unk23C.y;
	}

	updateAttachPos();
}

BOOL TCannon::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (param_1->mActorType == ACTOR_TYPE_MARE_CORK
	    && param_2 == HIT_MESSAGE_TAKE && mHolder == nullptr) {
		mHolder = (TTakeActor*)param_1;
		return TRUE;
	}
	if (param_2 == HIT_MESSAGE_SPRAYED_BY_WATER)
		return TRUE;
	return FALSE;
}

static const char* cannon_bastable[20] = {
	nullptr,
	nullptr,
	"/scene/cannon/bas/CannonDom_break.bas",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"/scene/cannon/bas/tyorobe_appear1.bas",
	"/scene/cannon/bas/tyorobe_damage1.bas",
	"/scene/cannon/bas/tyorobe_down1.bas",
	"/scene/cannon/bas/tyorobe_hyde1.bas",
	"/scene/cannon/bas/tyorobe_throw1.bas",
	nullptr,
	nullptr,
	nullptr,
};

void TCannon::calcObjCollision()
{
	static const f32 xzTable[8]
	    = { 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f };

	mHeadHeight               = 50.0f;
	JGeometry::TVec3<f32> pos = mPosition;
	pos.y                     = getMActor()->getModel()->getAnmMtx(4)[1][3];
	for (int i = 0; i < ARRAY_COUNT(unk25C); ++i) {
		unk25C[i] = pos;
		unk25C[i].x += 200.0f * xzTable[2 * i];
		unk25C[i].z += 200.0f * xzTable[2 * i + 1];
	}
}

void TCannon::entryObjCollision()
{
	unk258->setVertexData(0, unk25C[2], unk25C[1], unk25C[0]);
	unk258->setVertexData(1, unk25C[0], unk25C[3], unk25C[2]);
}

const char** TCannon::getBasNameTable() const { return cannon_bastable; }

void TCannon::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TSmallEnemy::perform(param_1, param_2);
	if (unk238) {
		unk1BC->getMActor()->perform(param_1, param_2);
		if ((param_1 & CUE_MOVE) && unk1BC->getMActor()->curAnmEndsNext())
			unk238 = 0;
		if ((param_1 & CUE_CALC_ANIM) && unk230 == 1) {
			if (unk1BC->getMActor()
			        ->getFrameCtrl(ANM_TYPE_BCK)
			        ->checkPass(174.0f)) {
				MtxPtr mtx = unk1B8->getMActor()->getModel()->getAnmMtx(0);
				gpMarioParticleManager->emitAndBindToMtxPtr(
				    CANNON_JPA_MS_CANNON_A, mtx, 0, nullptr);
				gpMarioParticleManager->emitAndBindToMtxPtr(
				    CANNON_JPA_MS_CANNON_B, mtx, 0, nullptr);
				gpMarioParticleManager->emitAndBindToMtxPtr(
				    CANNON_JPA_MS_CANNON_C, mtx, 0, nullptr);
				gpMarioParticleManager->emitAndBindToMtxPtr(
				    CANNON_JPA_MS_CANNON_D, mtx, 0, nullptr);
				gpMarioParticleManager->emitAndBindToMtxPtr(
				    CANNON_JPA_MS_CANNON_E, mtx, 0, nullptr);
			}
			if (unk1BC->getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame()
			    > 175.0f) {
				gpMarioParticleManager->emitAndBindToMtxPtr(
				    CANNON_JPA_MS_CANNON_SMOKE,
				    unk1BC->getMActor()->getModel()->getAnmMtx(0), 1, this);
			}
		}
	}
	if (unk230 == 5 || unk230 == 9) {
		unk1A8->perform(param_1, param_2);
		if ((param_1 & CUE_ENTRY)
		    && mSpine->getCurrentNerve() == &TNerveCannonDamage::theNerve()) {
			unk1A8->unk6C->getMActor()->offMakeDL();
			SMS_AddDamageFogEffect(
			    unk1A8->unk6C->getMActor()->getModel()->getModelData(),
			    mPosition, param_2);
		}
		for (int i = 0; i < ARRAY_COUNT(unk1AC); i++) {
			if (unk1AC[i]->unk24)
				continue;
			if (param_1 == CUE_MOVE) {
				f32 dist = unk28C->mSLBombDist.get();
				if (dist * dist < mDistToMarioSquared && unk230 == 5) {
					unk1AC[i]->unk2C = unk224[i];
					if (i != unk214
					    && mSpine->getCurrentNerve()
					           != &TNerveCannonObject::theNerve()) {
						unk1AC[i]->unk30 += 1.0f;
						if (unk1AC[i]->unk30 > 360.0f)
							unk1AC[i]->unk30 -= 360.0f;
						if (i == 2)
							unk1AC[i]->unk28 = -10.0f
							                   * JMASSin(DEG2SHORTANGLE(
							                       0.5f * unk1AC[i]->unk30));
						else
							unk1AC[i]->unk28 = -20.0f
							                   * JMASSin(DEG2SHORTANGLE(
							                       0.5f * unk1AC[i]->unk30));
					}
				} else {
					unk1AC[i]->unk30 = 0.0f;
					unk1AC[i]->unk28 *= 0.99f;
					unk1C0[i]->moveMtx(unk1AC[i]->getConnectedMtx());
				}
			}
			unk1AC[i]->perform(param_1, param_2);
		}
	} else if (!unk1B8->unk24) {
		unk1B8->perform(param_1, param_2);
	}
}

void TCannon::calcRootMatrix()
{
	if (mSpine->getCurrentNerve() != &TNerveCannonObject::theNerve()) {
		calcObjCollision();
		entryObjCollision();
	}

	if (mHolder != nullptr) {
		MtxPtr mtx = mHolder->getTakingMtx();
		if (mSpine->getCurrentNerve() == &TNerveCannonObject::theNerve()) {
			getModel()->setBaseTRMtx(mtx);
			mPosition.set(((TMtx34f*)mtx)->ref(0, 3),
			              ((TMtx34f*)mtx)->ref(1, 3),
			              ((TMtx34f*)mtx)->ref(2, 3));
		} else {
			if (SMSGetMarDirector()->isDemoModeNow())
				mRotation.y = -80.0f;
			mPosition.set(((TMtx34f*)mtx)->ref(0, 3),
			              ((TMtx34f*)mtx)->ref(1, 3),
			              ((TMtx34f*)mtx)->ref(2, 3));
			MsMtxSetXYZRPH(getMActor()->getModel()->getBaseTRMtx(), mPosition.x,
			               mPosition.y, mPosition.z, mRotation.x, mRotation.y,
			               mRotation.z);
		}
	} else {
		TSpineEnemy::calcRootMatrix();
	}

	if (unk1A8 != nullptr
	    && unk1A8->unk6C->getMActor()->checkCurBckFromIndex(14)
	    && unk1A8->unk6C->getMActor()
	           ->getFrameCtrl(ANM_TYPE_BCK)
	           ->checkPass(2.0f)) {
		for (int i = 0; i < ARRAY_COUNT(unk1AC); i++) {
			TMtx34f* mtx = (TMtx34f*)unk1AC[i]->getConnectedMtx();
			unk294.set(mtx->ref(0, 3), mtx->ref(1, 3), mtx->ref(2, 3));
			unk1AC[i]->setBckAnm(2);
			gpMarioParticleManager->emit(PARTICLE_MS_HIPDROP_C, &unk294, 0,
			                             nullptr);
			gpMarioParticleManager->emit(PARTICLE_MS_HIPDROP_B, &unk294, 0,
			                             nullptr);
			gpMarioParticleManager->emit(PARTICLE_MS_HIPDROP_A, &unk294, 0,
			                             nullptr);
		}
	}
}

MtxPtr TCannon::getTakingMtx()
{
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT))
		return unk1A8->unk6C->getMActor()->getModel()->getBaseTRMtx();

	unk1E4.translation(
	    JGeometry::TVec3<f32>(unk1A8->mPosition.x,
	                          unk1A8->unk6C->getMActor()->getModel()->getAnmMtx(
	                              mChorobeiHandJntIdx[0])[1][3],
	                          unk1A8->mPosition.z));
	return unk1E4;
}

// TODO: frame-only mismatch; the range stack slots are 4 bytes too low.
void TCannon::bombSet()
{
	TMsRange<f32> range(0.0f, 1.0f);
	f32 value = range.rand();
	f32 rate  = unk28C->mSLBombHeiGenerateRate.get();
	unk21C    = 0;
	unk1A4    = nullptr;
	TSpineEnemy* actor;
	if (value < rate) {
		actor = gpConductor->makeOneEnemyAppear(mPosition, "ボム兵マネージャー",
		                                        1);
	} else if (TMsRange<s32>(0, 100).rand() % 2 == 1) {
		actor
		    = gpConductor->makeOneEnemyAppear(mPosition, "ポポマネージャー", 1);
		if (actor)
			static_cast<TPopo*>(actor)->thrownByChorobei();
	} else {
		actor = gpConductor->makeOneEnemyAppear(mPosition,
		                                        "ハムクリマネージャー", 1);
	}

	if (!unk1A4) {
		unk1A4 = actor;
		if (actor) {
			actor->reset();
			MActor* mActor = actor->getMActor();
			mActor->setFrameRate(0.0f, ANM_TYPE_BCK);
		}
	}
	if (unk1A4) {
		unk1A4->mPosition = unk1A8->mPosition;
		unk220            = unk1A4->mScaling.x;
		unk1A4->mScaling.set(0.0f, 0.0f, 0.0f);
		unk1A4->mRotation = mRotation;
		if (unk1A4->receiveMessage(this, HIT_MESSAGE_TAKE))
			mHeldObject = unk1A4;
	}
}

void TCannon::bombShoot()
{
	if (!unk1A4)
		return;

	JGeometry::TVec3<f32> dir(SMS_GetMarioPos().x - mPosition.x, 0.0f,
	                          SMS_GetMarioPos().z - mPosition.z);
	if (dir.x == 0.0f && dir.z == 0.0f)
		dir.x = 1.0f;
	MsVECNormalize(&dir, &dir);

	TMsRange<f32> range(-30.0f, 30.0f);
	f32 angle = range.rand();
	Mtx mtx;
	MsMtxSetRotRPH(mtx, 0.0f, mRotation.y + angle, 0.0f);

	f32 speed = unk28C->mSLThrowXZSpeed.get();
	dir.y     = speed;
	dir.x *= speed;
	dir.z *= speed;

	if (unk21C) {
		TPopo* actor = static_cast<TPopo*>(unk1A4);
		actor->setVelocityAndFlag10(dir.x, dir.y, dir.z);
	} else {
		TBombHei* actor = static_cast<TBombHei*>(unk1A4);
		actor->setVelocity(dir);
		actor->onLiveFlag(LIVE_FLAG_AIRBORNE);
		actor->getMActor()->setFrameRate(SMSGetAnmFrameRate(), ANM_TYPE_BCK);
	}

	unk1A4->mPosition.y += 2.0f;
	unk1A4->receiveMessage(this, HIT_MESSAGE_PUT);
}

void TCannon::bombScaleUp()
{
	if (unk1A4) {
		f32 add            = 0.2f * unk220;
		unk1A4->mScaling.x = MsClamp(unk1A4->mScaling.x + add, 0.0f, unk220);
		unk1A4->mScaling.setAll(mScaling.x);
	}
}

void TCannon::hitHead(TBombHei* param_1)
{
	if (mSpine->getCurrentNerve() == &TNerveCannonDamage::theNerve())
		return;

	if (param_1->isDamageToCannon()) {
		mSpine->pushNerve(&TNerveCannonDamage::theNerve());
		param_1->kill();
	}
}

void TCannon::updateAttachPos()
{
	if (!unk1A0)
		return;

	unk1A0->mPosition = unk194 + unk1A8->mPosition;
	unk1A0->offLiveFlag(LIVE_FLAG_AIRBORNE);

	if (mSpine->getCurrentNerve() == &TNerveCannonClose::theNerve()) {
		unk1A0->kill();
		unk1A0 = nullptr;
		return;
	}
	if (!unk1A0->doKeepDistance()) {
		unk1A0 = nullptr;
		mSpine->pushNerve(&TNerveCannonDamage::theNerve());
	}
}

// TODO: nonmatching stack frame and local stack layout.
void TCannon::killerShoot()
{
	if (unk239) {
		TKiller* killer = static_cast<TKiller*>(gpConductor->makeOneEnemyAppear(
		    mPosition, "キラーマネージャー", 0));
		if (!killer)
			return;
		killer->reset();
		unk1E0 = unk1AC[unk214]->getMActor()->getModel()->getBaseTRMtx();
		unk1E0 = unk1AC[unk214]->getMActor()->getModel()->getAnmMtx(1);
		TPosition3f mtx;
		mtx.translation(JGeometry::TVec3<f32>(0.0f, -60.0f, 150.0f));
		MtxPtr ptr = mtx;
		MTXConcat(unk1E0, ptr, ptr);
		killer->mPosition.set(ptr[0][3], ptr[1][3], ptr[2][3]);
		JGeometry::TVec3<f32> velocity;
		f32 angle;
		f32 speedX                 = SMS_GetMarioSpeedX();
		f32 speedZ                 = SMS_GetMarioSpeedZ();
		JGeometry::TVec3<f32> goal = SMS_GetMarioPos();
		TMsRange<f32> range(-300.0f, 300.0f);
		goal.x += range.rand();
		switch (unk214) {
		case 0:
			goal.z -= 2.0f * fabsf(range.rand());
			break;
		case 1:
			break;
		case 2:
			goal.z += 2.0f * fabsf(range.rand());
			break;
		}
		velocity
		    = killer->calcVelocityToJumpToY(goal, 5.0f, killer->getGravityY());
		JGeometry::TVec3<f32> delta = goal - mPosition;
		f32 time = fabsf(MsVECMag2(&delta) / (velocity.x * mVelocityRate));
		JGeometry::TVec3<f32> predicted;
		killer->unk1A5 = 0;
		TMsRange<s32> killerRange(0, 100);
		f32 rate = mVelocityRate;
		if (killerRange.rand() % 5 == 0) {
			killer->unk1A5 = 1;
		} else {
			if (SMS_GetMarioSpeedX() > 2.0f)
				rate = 0.55f;
			if (SMS_GetMarioSpeedX() < -2.0f)
				rate = 0.68f;
		}
		predicted.set(goal.x + mSearchRate * (speedX * time), goal.y,
		              goal.z + mSearchRate * (speedZ * time));
		velocity = killer->calcVelocityToJumpToY(predicted, 5.0f,
		                                         killer->getGravityY());
		velocity.scale(rate);
		angle = MsAngleWrap(MsGetRotFromZaxisY(velocity));
		killer->mRotation.set(0.0f, angle, 0.0f);
		killer->mScaling.set(0.1f, 0.1f, 0.1f);
		if (SMSGetMarDirector()->mState == TMarDirector::STATE_UNK1) {
			velocity.x *= 0.2f;
			velocity.y *= 0.4f;
			velocity.z *= 0.2f;
			killer->mScaling.set(0.6f, 0.6f, 0.6f);
		}
		killer->setColorType();
		killer->unk1A8 = velocity;
		killer->setVelocity(velocity);
		killer->onLiveFlag(LIVE_FLAG_AIRBORNE);
		JGeometry::TVec3<f32> marioDelta = SMS_GetMarioPos() - mPosition;
		goal.x += marioDelta.x;
		goal.z += marioDelta.z;
		killer->setGoalPath(TPathNode(goal));
		SMSGetMSound()->startSoundActor(MSD_SE_EN_KILLER_FIRE,
		                                &killer->mPosition, 0, nullptr, 0, 4);
		SMSGetMSound()->startSoundActor(MSD_SE_EN_KILLER_FLY,
		                                &killer->mPosition, 0, nullptr, 0, 4);
	} else {
		TIgaiga* igaiga = static_cast<TIgaiga*>(gpConductor->makeOneEnemyAppear(
		    mPosition, "イガイガマネージャー", 1));
		if (!igaiga)
			return;
		igaiga->reset();
		unk1E0 = unk1AC[unk214]->getMActor()->getModel()->getAnmMtx(1);
		TPosition3f mtx;
		mtx.translation(JGeometry::TVec3<f32>(0.0f, -60.0f, 150.0f));
		MtxPtr ptr = mtx;
		MTXConcat(unk1E0, ptr, ptr);
		igaiga->mPosition.set(ptr[0][3], ptr[1][3], ptr[2][3]);
		JGeometry::TVec3<f32> goal;
		JPABaseEmitter* emitter = gpMarioParticleManager->emitWithRotate(
		    PARTICLE_MS_IGA_FUMI_AIR, &igaiga->mPosition, 0,
		    DEG2SHORTANGLE(igaiga->mRotation.y), 0, 0, nullptr);
		if (emitter) {
			JGeometry::TVec3<f32> scale(1.5f, 1.5f, 1.5f);
			scale.mul(mScaling);
			emitter->setGlobalScale(scale);
		}
		igaiga->getTracer()->getGraph()->getFirstGraphNode().getPoint(&goal);
		unk248 = goal;
		JGeometry::TVec3<f32> velocity
		    = igaiga->calcVelocityToJumpToY(goal, 10.0f, igaiga->getGravityY());
		igaiga->mRotation = mRotation;
		igaiga->shoot(velocity);
	}
}

void TCannon::endKillerShoot()
{
	unk214++;
	if (unk214 >= 3)
		unk214 = 0;
}

void TCannon::damage()
{
	setFreezeAnm();
	unk1A8->setBckAnm(13);
	MtxPtr mtx = unk1A8->unk6C->getMActor()->getModel()->getAnmMtx(0);
	unk294.set(mtx[0][3], mtx[1][3], mtx[2][3]);
	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToPosPtr(
	    PARTICLE_MS_CHO_MOKU_A, &unk294, 0, nullptr);
	if (emitter)
		emitter->setGlobalScale(unk1A8->mScaling);
}

void TCannon::setKillerGoalPoint()
{
	if (unk239) {
		TMsRange<f32> range(0.0f, 360000.0f);
		f32 value = range.rand();
		s16 angle = value;
		JGeometry::TVec3<f32> goal(SMS_GetMarioPos(),
		                           JGeometry::TVec3<f32>::ASSIGN_COPY);
		f32 radius = 500.0f;
		goal.x += radius * JMASCos(angle);
		goal.z += radius * JMASSin(angle);
		setGoalPath(TPathNode(goal));
	} else {
		setGoalPath(TPathNode(unk248));
	}
	unk1AC[unk214]->setBckAnm(1);
}

void TCannon::deadCannon()
{
	onHitFilter(HIT_FILTER_NO_COLLISION);
	unk1A8->onHitFilter(HIT_FILTER_NO_COLLISION);
}

void TCannon::startDemo() { }

void TCannon::startMarioDemo() { }

void TCannon::turnToGoal() { walkToCurPathNode(0.0f, mTurnSpeed, 0.0f); }

bool TCannon::isObject()
{
	if (isBckAnm(4) && checkCurAnmEnd(0))
		return true;
	return false;
}

void TCannon::killShootAct()
{
	if (unk1A4)
		unk1A4->kill();
}

void TCannon::gateOpen()
{
	if (!unk254)
		return;
	if (unk254->checkLiveFlag(LIVE_FLAG_DEAD)) {
		unk254->appear();
		unk254->mPosition = mPosition;
		unk254->mScaling.set(0.27f, 0.02f, 0.27f);
		unk254->getMActor()->setBtk("maregate");
	}
	unk254->mScaling.y = MsClamp(unk254->mScaling.y * 1.01f, 0.0f, 0.22f);
}

void TCannon::startChorobeiShout() { }

DEFINE_NERVE(TNerveCannonOpen, TLiveActor)
{
	TCannon* cannon = static_cast<TCannon*>(spine->getBody());
	if (spine->getTime() == 0) {
		cannon->unk1A8->setBckAnm(12);
		cannon->setBckAnm(3);
	}
	if (cannon->unk1A8->isUpEnd() && cannon->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveCannonSearch::theNerve());
		return true;
	}
	return false;
}

// TODO: nonmatching stack layout and callee-saved register allocation.
DEFINE_NERVE(TNerveCannonSearch, TLiveActor)
{
	TCannon* cannon = static_cast<TCannon*>(spine->getBody());
	cannon->updateSquareToMario();
	f32 initialBombDist = cannon->unk28C->mSLBombDist.get();
	initialBombDist *= initialBombDist;
	f32 distSquared = cannon->getDistToMarioSquared();
	if (spine->getTime() == 0) {
		if (distSquared < initialBombDist)
			cannon->setGoalPath((THitActor*)gpMarioAddress);
		cannon->unk1A8->setBckAnm(19);
	}
	if (cannon->unk1A8->unk6C->getMActor()->curAnmEndsNext()
	    && cannon->unk1A8->unk6C->getMActor()->checkCurBckFromIndex(19))
		cannon->unk1A8->setBckAnm(18);
	f32 hideDist = cannon->unk28C->mSLHideDist.get();
	if (distSquared < hideDist * hideDist) {
		spine->pushAfterCurrent(&TNerveCannonClose::theNerve());
		return true;
	}
	f32 bombDist = cannon->unk28C->mSLBombDist.get();
	if (distSquared < bombDist * bombDist) {
		if (spine->getTime() > cannon->unk28C->mSLBombInterval.get()) {
			cannon->unk290 = 1;
			spine->pushAfterCurrent(&TNerveCannonShoot::theNerve());
			return true;
		}
	} else if (cannon->unk230 == 5 || cannon->unk230 == 9) {
		f32 killerDist = cannon->unk28C->mSLKillerDist.get();
		if (distSquared < killerDist * killerDist
		    && spine->getTime() > cannon->unk28C->mSLShootInterval.get()) {
			cannon->unk290 = 0;
			spine->pushAfterCurrent(&TNerveCannonSearch::theNerve());
			spine->pushAfterCurrent(&TNerveCannonForceBombShoot::theNerve());
			spine->pushAfterCurrent(&TNerveCannonShoot::theNerve());
			spine->pushAfterCurrent(&TNerveCannonShoot::theNerve());
			spine->pushAfterCurrent(&TNerveCannonShoot::theNerve());
			return true;
		}
	}
	if (cannon->unk2AC != cannon->mRotation.y) {
		cannon->unk2AC = cannon->mRotation.y;
		SMSGetMSound()->startSoundActor(MSD_SE_EN_CANNON_MOVE,
		                                &cannon->mPosition, 0, nullptr, 0, 4);
	}
	if (SMSGetApplication()->mCurrArea.getStage() == 5
	    && SMSGetMarDirector()->mState == TMarDirector::STATE_UNK1) {
		JGeometry::TVec3<f32> delta = SMS_GetMarioPos() - cannon->mPosition;
		cannon->mRotation.y         = MsGetRotFromZaxis(delta).y;
	} else {
		cannon->turnToGoal();
	}
	return false;
}

DEFINE_NERVE(TNerveCannonShoot, TLiveActor)
{
	TCannon* cannon = static_cast<TCannon*>(spine->getBody());
	if (spine->getTime() == 0) {
		if (!cannon->unk290)
			cannon->setKillerGoalPoint();
		else
			cannon->unk1A8->setBckAnm(17);
	}
	if (cannon->unk290) {
		if (cannon->unk1A8->unk6C->getMActor()->checkCurBckFromIndex(17)) {
			if (cannon->unk1A8->unk6C->getMActor()->curAnmEndsNext()) {
				cannon->unk1A8->setBckAnm(16);
				cannon->bombSet();
			}
			cannon->turnToGoal();
		} else if (cannon->unk1A8->unk6C->getMActor()->checkCurBckFromIndex(
		               16)) {
			if (cannon->unk1A8->unk6C->getMActor()->curAnmEndsNext()) {
				spine->pushAfterCurrent(&TNerveCannonSearch::theNerve());
				return true;
			}
			if (cannon->unk1A8->unk6C->getMActor()
			        ->getFrameCtrl(ANM_TYPE_BCK)
			        ->checkPass(38.0f))
				cannon->bombShoot();
			if (cannon->unk1A8->unk6C->getMActor()
			        ->getFrameCtrl(ANM_TYPE_BCK)
			        ->getFrame()
			    > 26.0f)
				cannon->bombScaleUp();
		}
	} else {
		if (spine->getTime() < 40)
			cannon->turnToGoal();
		if (spine->getTime() == 40)
			cannon->killerShoot();
		if (spine->getTime() > cannon->unk28C->mSLKillerInterval.get()) {
			cannon->endKillerShoot();
			return true;
		}
	}
	return false;
}

DEFINE_NERVE(TNerveCannonForceBombShoot, TLiveActor)
{
	TCannon* cannon = static_cast<TCannon*>(spine->getBody());
	if (spine->getTime() == 0) {
		f32 dist = cannon->unk28C->mSLBombDist.get();
		if (cannon->getDistToMarioSquared() < 2.0f * (dist * dist))
			cannon->unk1A8->setBckAnm(17);
		else
			return true;
	}
	if (cannon->unk1A8->unk6C->getMActor()->checkCurBckFromIndex(17)) {
		if (cannon->unk1A8->unk6C->getMActor()->curAnmEndsNext()) {
			cannon->unk1A8->setBckAnm(16);
			cannon->bombSet();
		}
		cannon->turnToGoal();
	} else if (cannon->unk1A8->unk6C->getMActor()->checkCurBckFromIndex(16)) {
		if (cannon->unk1A8->unk6C->getMActor()->curAnmEndsNext())
			return true;
		if (cannon->unk1A8->unk6C->getMActor()
		        ->getFrameCtrl(ANM_TYPE_BCK)
		        ->checkPass(38.0f))
			cannon->bombShoot();
		if (cannon->unk1A8->unk6C->getMActor()
		        ->getFrameCtrl(ANM_TYPE_BCK)
		        ->getFrame()
		    > 26.0f)
			cannon->bombScaleUp();
	}
	return false;
}

DEFINE_NERVE(TNerveCannonClose, TLiveActor)
{
	TCannon* cannon = static_cast<TCannon*>(spine->getBody());
	if (spine->getTime() < 2) {
		cannon->deadCannon();
		cannon->unk1A8->setBckAnm(15);
		cannon->unk294 = cannon->mPosition;
		cannon->unk294.y += 300.0f;
		gpMarioParticleManager->emitAndBindToPosPtr(
		    PARTICLE_MS_CHO_ASE, &cannon->unk294, 0, nullptr);
	}
	if (cannon->unk1A8->isDownEnd() && !cannon->isBckAnm(0))
		cannon->setBckAnm(0);
	if (cannon->isBckAnm(0)
	    && cannon->getMActor()->getFrameCtrl(ANM_TYPE_BCK)->checkPass(10.0f)) {
		cannon->unk294 = cannon->mPosition;
		cannon->unk294.y += 290.0f;
		JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToPosPtr(
		    PARTICLE_MS_JUMP_ED_B, &cannon->unk294, 0, nullptr);
		if (emitter)
			emitter->setGlobalScale(JGeometry::TVec3<f32>(1.5f, 1.5f, 1.5f));
	}
	cannon->updateSquareToMario();
	f32 hideDist = cannon->unk28C->mSLHideDist.get();
	hideDist *= 3.0f;
	hideDist *= hideDist;
	if (cannon->getDistToMarioSquared() > hideDist) {
		cannon->offHitFilter(HIT_FILTER_NO_COLLISION);
		cannon->unk1A8->offHitFilter(HIT_FILTER_NO_COLLISION);
		spine->pushAfterCurrent(&TNerveCannonOpen::theNerve());
		return true;
	}
	cannon->unk2B0->moveMtx(cannon->getMActor()->getModel()->getAnmMtx(4));
	return false;
}

DEFINE_NERVE(TNerveCannonDamage, TLiveActor)
{
	TCannon* cannon = static_cast<TCannon*>(spine->getBody());
	if (spine->getTime() == 0) {
		JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToPosPtr(
		    EXPLOSION_JPA_MS_BOMB_SMOKE, &cannon->mPosition, 0, nullptr);
		if (emitter)
			emitter->setGlobalScale(JGeometry::TVec3<f32>(2.0f, 2.0f, 2.0f));
		gpMarioParticleManager->emitAndBindToPosPtr(
		    EXPLOSION_JPA_MS_BOMB_BOMB, &cannon->mPosition, 0, nullptr);
		if (emitter)
			emitter->setGlobalScale(JGeometry::TVec3<f32>(2.0f, 2.0f, 2.0f));
		gpMarioParticleManager->emitAndBindToPosPtr(
		    EXPLOSION_JPA_MS_BOMB_HAHEN, &cannon->mPosition, 0, nullptr);
		if (emitter)
			emitter->setGlobalScale(JGeometry::TVec3<f32>(2.0f, 2.0f, 2.0f));
		cannon->decHitPoints();
		if (cannon->mHitPoints == 0) {
			cannon->killShootAct();
			cannon->unk1A8->setBckAnm(13);
			if (SMSGetApplication()->mCurrArea.getStage() == 5)
				SMSRumbleMgr->start(24, static_cast<f32*>(nullptr));
			else
				SMSRumbleMgr->start(23, static_cast<f32*>(nullptr));
			JPABaseEmitter* next = gpMarioParticleManager->emitAndBindToMtxPtr(
			    PARTICLE_MS_CHO_MOKU_B,
			    cannon->unk1A8->unk6C->getMActor()->getModel()->getAnmMtx(12),
			    0, nullptr);
			if (next)
				next->setGlobalScale(cannon->unk1A8->mScaling);
			next = gpMarioParticleManager->emitAndBindToPosPtr(
			    PARTICLE_MS_CHO_MOKU_A, &cannon->unk294, 0, nullptr);
			if (next)
				next->setGlobalScale(cannon->unk1A8->mScaling);
			if (SMSGetApplication()->mCurrArea.getStage() == 5) {
				cannon->unk2A0   = cannon->mPosition;
				cannon->unk2A0.y = 0.0f;
				SMSGetMarDirector()->fireStartDemoCamera(
				    "tyorocam_pinna", &cannon->unk2A0, -1, cannon->mRotation.y,
				    true, nullptr, 0, nullptr, JDrama::TFlagT<u16>(0));
			} else {
				SMSGetMarDirector()->fireStartDemoCamera(
				    "tyorocam_mare", nullptr, -1, 0.0f, true, nullptr, 0,
				    nullptr, JDrama::TFlagT<u16>(0));
			}
			cannon->deadCannon();
		} else {
			cannon->damage();
		}
		cannon->mVelocity = JGeometry::TVec3<f32>(0.0f, 4.0f, 0.0f);
		cannon->onLiveFlag(LIVE_FLAG_AIRBORNE);
		cannon->mPosition.y += 10.0f;
	}
	if (cannon->unk1A8->unk6C->getMActor()->curAnmEndsNext()) {
		SMS_ResetDamageFogEffect(
		    cannon->unk1A8->unk6C->getMActor()->getModel()->getModelData());
		if (cannon->mHitPoints == 0) {
			spine->pushAfterCurrent(&TNerveCannonDamageDemo::theNerve());
			return true;
		}
		spine->pushAfterCurrent(&TNerveCannonSearch::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveCannonDamageDemo, TLiveActor)
{
	TCannon* cannon = static_cast<TCannon*>(spine->getBody());
	if (spine->getTime() == 0)
		cannon->unk1A8->setBckAnm(14);
	if (spine->getTime() > 120
	    && cannon->unk1A8->unk6C->getMActor()->curAnmEndsNext()
	    && !cannon->isBckAnm(4)) {
		cannon->setBckAnm(4);
		SMSGetMSound()->startSoundActor(MSD_SE_EN_CANNON_DOWN,
		                                &cannon->mPosition, 0, nullptr, 0, 4);
		TEffectExplosion* effect
		    = (TEffectExplosion*)gpConductor->makeOneEnemyAppear(
		        cannon->mPosition, "エフェクト爆発マネージャー", 1);
		if (effect) {
			JGeometry::TVec3<f32> scale(2.5f, 2.5f, 2.5f);
			effect->generate(cannon->mPosition, scale);
		}
		TFlagManager::getInstance()->setBool(true, MSF_MONTY_MOLE_DEFEATED);
		spine->pushAfterCurrent(&TNerveCannonObject::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveCannonObject, TLiveActor)
{
	TCannon* cannon = static_cast<TCannon*>(spine->getBody());
	if (spine->getTime() == 0) {
		if (!cannon->isBckAnm(4)) {
			cannon->setBckAnm(4);
			J3DFrameCtrl* ctrl
			    = cannon->getMActor()->getFrameCtrl(ANM_TYPE_BCK);
			ctrl->setFrame(ctrl->getEnd());
		}
		if (cannon->unk1A8)
			cannon->unk1A8->unk70 = 1.0f;
	}
	if (cannon->isBckAnm(4)
	    && cannon->getMActor()->getFrameCtrl(ANM_TYPE_BCK)->checkPass(60.0f)) {
		SMSGetMSound()->startSoundActor(MSD_SE_EN_CANNON_LAND,
		                                &cannon->mPosition, 0, nullptr, 0, 4);
	}
	if (spine->getTime() > 150)
		cannon->gateOpen();
	return false;
}

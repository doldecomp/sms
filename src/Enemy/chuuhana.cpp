#include <Enemy/ChuuHana.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/PathNode.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JDrama/JDRActor.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <JSystem/JParticle/JPACallback.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JStage/JSGActor.hpp>
#include <JSystem/JStage/JSGObject.hpp>
#include <JSystem/JSupport/JSUList.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/MapMirror.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/LiveManager.hpp>
#include <Strategic/MirrorActor.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/ObjManager.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/TakeActor.hpp>
#include <System/Application.hpp>
#include <System/BaseParam.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/ParamInst.hpp>
#include <System/Params.hpp>
#include <System/Particles.hpp>
#include <macros.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static void dummy(Vec* v)
{
	*v = (Vec) { 0.0f, 0.0f, 0.0f };
	*v = (Vec) { 1.0f, 1.0f, 1.0f };
}

static TChuuHana* gpCurChuuHana;

s32 TChuuHana::mCheckOnPanelTimeRoll = 20;
s32 TChuuHana::mCheckOnPanelTime     = 400;
u8 TChuuHana::mBodyJntIndex          = 1;
u8 TChuuHana::mEyeJntIndex           = 12;
u8 TChuuHana::mFootJntIndex          = 5;
u8 TChuuHana::mNewSw                 = 1;
u8 TChuuHana::mCompareHeight         = 1;
f32 TChuuHana::mSmallMirrorR         = 650.0f;
f32 TChuuHana::mMediumMirrorR        = 900.0f;
f32 TChuuHana::mLargeMirrorR         = 1100.0f;
u8 TChuuHana::mAttackVersion         = 1;
u8 TChuuHana::mDamageSw              = 1;

TChuuHanaSaveLoadParams::TChuuHanaSaveLoadParams(const char* param_1)
    : TWalkerEnemyParams(param_1)
    , PARAM_INIT(mSLGetWaterPow, 1.0f)
    , PARAM_INIT(mSLGetGroundPow, 1.0f)
    , PARAM_INIT(mSLKeepBalanceTime, 200)
    , PARAM_INIT(mSLCheckFrame, 5)
    , PARAM_INIT(mSLReverseHeightS, 15.0f)
    , PARAM_INIT(mSLStretchHeightS, 10.0f)
    , PARAM_INIT(mSLMediumStretchHeightS, 7.0f)
    , PARAM_INIT(mSLSmallStretchHeightS, 3.0f)
    , PARAM_INIT(mSLReverseHeightM, 15.0f)
    , PARAM_INIT(mSLStretchHeightM, 10.0f)
    , PARAM_INIT(mSLMediumStretchHeightM, 7.0f)
    , PARAM_INIT(mSLSmallStretchHeightM, 3.0f)
    , PARAM_INIT(mSLReverseHeightL, 15.0f)
    , PARAM_INIT(mSLStretchHeightL, 10.0f)
    , PARAM_INIT(mSLMediumStretchHeightL, 7.0f)
    , PARAM_INIT(mSLSmallStretchHeightL, 3.0f)
    , PARAM_INIT(mSLWalkGravity, 4.0f)
    , PARAM_INIT(mSLWaterHitGravity, 0.2f)
    , PARAM_INIT(mSLJumpGravity, 0.2f)
    , PARAM_INIT(mSLJumpSp, 12.0f)
    , PARAM_INIT(mSLJumpHeight, 300.0f)
    , PARAM_INIT(mSLGetWaterPow2, 1.0f)
    , PARAM_INIT(mSLTacklePow, 100.0f)
    , PARAM_INIT(mSLDashRate, 2.0f)
    , PARAM_INIT(mSLAttackTimer, 300)
    , PARAM_INIT(mSLHitWaterTimer, 60)
{
	TParams::load(mPrmPath);
}

TChuuHanaManager::TChuuHanaManager(const char* param_1)
    : TSmallEnemyManager(param_1)
{
	gpCurChuuHana = nullptr;
	unk60         = 0;
	unk61         = 0;
	unk62         = 0;
}

void TChuuHanaManager::load(JSUMemoryInputStream& param_1)
{
	TSmallEnemyManager::load(param_1);
	unk38 = new TChuuHanaSaveLoadParams("/enemy/chuuhana.prm");
}

void TChuuHanaManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TEnemyManager::perform(param_1, param_2);
}

TSpineEnemy* TChuuHanaManager::createEnemyInstance() { return new TChuuHana; }

void TChuuHanaManager::initSetEnemies()
{
	static const char* graphlist[]
	    = { "kohana0", "kohana1", "kohana1", "kohana2", "kohana2", "kohana2" };

	for (s32 i = 0; i < mCapacity; i++) {
		TGraphWeb* graph = gpConductor->getGraphByName(graphlist[i]);
		TChuuHana* enemy = (TChuuHana*)getObj(i);

		if (i == 0)
			enemy->unk21C = &unk60;
		else if (i < 3)
			enemy->unk21C = &unk61;
		else
			enemy->unk21C = &unk62;

		TMsRange<s32> nodeRange(0, graph->getNodeNum());
		JGeometry::TVec3<f32> point;
		graph->getGraphNode(nodeRange.rand()).getPoint(&point);

		enemy->mPosition = point;
		enemy->mPosition.y += 50.0f;
		enemy->onLiveFlag(LIVE_FLAG_AIRBORNE);
		enemy->getTracer()->setGraph(graph);
		enemy->reset();
	}
}

static BOOL ChuuHanaBodyCallback(J3DNode* param_1, BOOL param_2)
{
	if (param_2 == 0) {
		if (gpCurChuuHana == nullptr || !gpCurChuuHana->isRolling())
			return TRUE;

		MtxPtr mtx = gpCurChuuHana->getModel()->getAnmMtx(
		    ((J3DJoint*)param_1)->getJntNo());
		TPosition3f scale;
		scale.setTrans(0.0f, 0.0f, 0.0f);
		scale.setScale(1.0f, 1.0f, 1.0f);
		Mtx rot;

		JGeometry::TVec3<f32> axis(gpCurChuuHana->unk204.x, 0.0f,
		                           gpCurChuuHana->unk204.z);
		if (axis.x == 0.0f && axis.z == 0.0f)
			axis.x = 0.001f;

		JGeometry::TVec3<f32> side;
		JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
		VECCrossProduct(&up, &axis, &side);

		f32 angle = gpCurChuuHana->unk210;
		JGeometry::TVec3<f32> zDir(mtx[0][2], mtx[1][2], mtx[2][2]);
		JGeometry::TVec3<f32> xDir(mtx[0][0], mtx[1][0], mtx[2][0]);
		JGeometry::TVec3<f32> yDir(mtx[0][1], mtx[1][1], mtx[2][1]);
		JGeometry::TVec3<f32> local;

		f32 lenZ   = zDir.squared();
		f32 localZ = lenZ == 0.0f ? 0.0f : side.dot(zDir) / lenZ;
		f32 lenY   = yDir.squared();
		f32 localY = lenY == 0.0f ? 0.0f : side.dot(yDir) / lenY;
		f32 lenX   = xDir.squared();
		f32 localX = lenX == 0.0f ? 0.0f : side.dot(xDir) / lenX;
		local.set(localX, localY, localZ);

		MTXRotAxisRad(rot, &local, DEG_TO_RAD(angle));
		MTXConcat(mtx, rot, mtx);
		MTXConcat(mtx, scale, mtx);
		MTXConcat(J3DSys::mCurrentMtx, rot, J3DSys::mCurrentMtx);
		MTXConcat(J3DSys::mCurrentMtx, scale, J3DSys::mCurrentMtx);
	}
	return TRUE;
}

TChuuHanaAseParCallback::TChuuHanaAseParCallback(TChuuHana* param_1)
    : unk4(param_1)
{
}

void TChuuHanaAseParCallback::execute(JPABaseEmitter* param_1,
                                      JPABaseParticle* param_2)
{
	if (!unk4->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		param_1->setGlobalRTMatrix(
		    unk4->getMActor()->getModel()->getAnmMtx(TChuuHana::mEyeJntIndex));
		JGeometry::TVec3<f32> scale(2.5f, 2.5f, 2.5f);
		param_1->setGlobalScale(scale);
	}
}

void TChuuHanaAseParCallback::draw(JPABaseEmitter* param_1,
                                   JPABaseParticle* param_2)
{
}

TChuuHana::TChuuHana(const char* param_1)
    : TWalkerEnemy(param_1)
    , unk194(0.0f)
    , unk198(0.0f)
    , unk19C(0.0f)
    , unk1A0(0)
    , unk1A4(0)
    , unk1A8(0.0f)
    , unk1AC(0)
    , unk1B0(1)
    , unk1B1(0)
    , unk1B2(0)
    , unk1B8(0.0f)
    , unk210(0.0f)
    , unk214(0)
    , unk215(0)
    , unk218(nullptr)
    , unk21C(nullptr)
    , unk220(0.0f)
    , unk224(0)
    , unk228(this)
{
}

void TChuuHana::init(TLiveManager* param_1)
{
	TWalkerEnemy::init(param_1);
	mActorType = 0x10000016;
	unk150     = 17;
	offHitFlag(HIT_FLAG_UNK40000000);
	mSpine->initWith(&TNerveChuuHanaWalkOnPanel::theNerve());
	getMActor()->setJointCallback(mBodyJntIndex, &ChuuHanaBodyCallback);
	unk130 = 1;
	getMActor()->initNormalMotionBlend();
	unk1B4 = (TChuuHanaSaveLoadParams*)getSaveParam();
	getMActor()->getModel()->calc();

	TMirrorActor* mirror = new TMirrorActor("チュウハナin鏡");
	mirror->init(getModel(), 0);
}

void TChuuHana::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("default.bmd", 3);
}

void TChuuHana::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TSmallEnemy::perform(param_1, param_2);

	JGeometry::TVec3<f32> marioPos = SMS_GetMarioPos();
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		if (gpMirrorModelManager->isInMirror(mPosition)
		    || gpMirrorModelManager->isInMirror(marioPos)) {
			if (param_1 & CUE_CALC_ANIM) {
				calcRootMatrix();
				mMActor->calc();
			}
			if (param_1 & CUE_CALC_VIEW)
				mMActor->viewCalc();
		}
	}
}

void TChuuHana::reset()
{
	gpCurChuuHana = this;
	TWalkerEnemy::reset();

	unk215      = 0;
	unk1A4      = 30;
	mHeadHeight = 200.0f;
	unk1F8      = mPosition;
	unk19C      = 0.0f;
	unk198      = 0.0f;
	unk1A0      = 0;
	unk224      = 0;
	setSafeGoal();
}

void TChuuHana::setBckAnm(int param_1)
{
	unk194 = 1.0f;
	getMActor()->setMotionBlendRatioForBck(unk194);
	getMActor()->setBckOldMotionBlendAnmPtr(getMActor()->getBckAnm());
	TSmallEnemy::setBckAnm(param_1);
}

void TChuuHana::behaveToWater(THitActor* param_1)
{
	unk165 = true;
	unk224 = 0;

	if (mSpine->getCurrentNerve() == &TNerveChuuHanaRoll::theNerve()) {
		JGeometry::TVec3<f32> away(mPosition.x - SMS_GetMarioPos().x, 0.0f,
		                           mPosition.z - SMS_GetMarioPos().z);
		MsVECNormalize(away, away);
		away.scale(unk1B4->mSLGetWaterPow.get());
		margeVelocity(away);
		onLiveFlag(LIVE_FLAG_AIRBORNE);
		mPosition.y += 10.0f;
		return;
	}

	if (mSpine->getCurrentNerve() == &TNerveChuuHanaWalkOnPanel::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveChuuHanaAttack::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveChuuHanaWait::theNerve()
	    || (mNewSw
	        && mSpine->getCurrentNerve() == &TNerveChuuHanaStick::theNerve())) {
		unk165 = true;
		if (mAttackVersion)
			*unk21C = 1;

		JGeometry::TVec3<f32> away(mPosition.x - SMS_GetMarioPos().x, 0.0f,
		                           mPosition.z - SMS_GetMarioPos().z);
		MsVECNormalize(away, away);
		away.scale(unk1B4->mSLGetWaterPow2.get());

		if (!isAirborne()) {
			if (mCompareHeight)
				mPosition.y += 2.0f;
			else
				mPosition.y += 1.0f;
		}
		mVelocity = away;
		onLiveFlag(LIVE_FLAG_AIRBORNE);

		if (mSpine->getCurrentNerve() != &TNerveChuuHanaStick::theNerve())
			mSpine->pushNerve(&TNerveChuuHanaStick::theNerve());

		mSprayedByWaterCooldown = 0;
	} else if (!mNewSw
	           && mSpine->getCurrentNerve()
	                  == &TNerveChuuHanaKeepBalance::theNerve()) {
		JGeometry::TVec3<f32> away(mPosition.x - SMS_GetMarioPos().x, 10.0f,
		                           mPosition.z - SMS_GetMarioPos().z);
		MsVECNormalize(away, away);
		away.scale(2.0f * unk1B4->mSLGetWaterPow.get());
		mVelocity = away;
		onLiveFlag(LIVE_FLAG_AIRBORNE);
		mPosition.y += 20.0f;
	}
}

void TChuuHana::attackToMario()
{
	if (mSpine->getCurrentNerve() != &TNerveChuuHanaObject::theNerve()) {
		if (mDamageSw) {
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
		} else if (mSpine->getCurrentNerve()
		           == &TNerveChuuHanaAttack::theNerve()) {
			if (SMS_IsMarioTouchGround4cm()) {
				SMS_SendMessageToMario(this, HIT_MESSAGE_THROWN);
				JGeometry::TVec3<f32> toMario = mPosition - SMS_GetMarioPos();

				Mtx rot;
				MsMtxSetRotRPH(rot, 0.0f, MsGetRotFromZaxisY(toMario), 0.0f);

				JGeometry::TVec3<f32> dir(0.0f, 1.0f, -1.0f);
				MTXMultVec(rot, &dir, &dir);
				SMS_ThrowMario(dir, unk1B4->mSLTacklePow.get());
				*unk21C = 0;
			}
		} else {
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
		}
	} else {
		unk215 = 1;
	}
}

void TChuuHana::moveObject()
{
	TWalkerEnemy::moveObject();

	if (unk1A0 == 0) {
		unk198 = unk19C = mPosition.y;
		unk1A8          = 0.0f;
	} else {
		unk1A0++;
		if (unk198 > mPosition.y)
			unk198 = mPosition.y;
		if (unk19C < mPosition.y)
			unk19C = mPosition.y;

		if (unk1A0 > unk1B4->mSLCheckFrame.get()) {
			f32 height = unk19C - unk198;
			if (unk1A8 < height)
				unk1A8 = height;

			if (mPosition.y < unk19C) {
				unk1A8 /= (f32)unk1A0;
				checkStretchType();
			} else {
				if (mSpine->getCurrentNerve()
				    == &TNerveChuuHanaKeepBalance::theNerve())
					setBckAnm(7);
				unk1A0 = 1;
				unk198 = unk19C = mPosition.y;
			}
		}
	}

	if (mSpine->getCurrentNerve() == &TNerveChuuHanaRoll::theNerve()) {
		if (!isAirborne()) {
			f32 power                           = unk1B4->mSLGetGroundPow.get();
			const JGeometry::TVec3<f32>& normal = mGroundPlane->getNormal();
			JGeometry::TVec3<f32> acceleration(power * normal.x, 0.0f,
			                                   power * normal.z);
			margeVelocity(acceleration);
			mPosition.y += 5.0f;
			onLiveFlag(LIVE_FLAG_AIRBORNE);
		}
	}

	unk194 = MsClamp(unk194 - 0.1f, 0.0f, 1.0f);
	mMActor->setMotionBlendRatioForBck(unk194);

	unk1EC = mLinearVelocity;

	if (!isAirborne()
	    && (mGroundPlane->getActor() == nullptr
	        || mGroundPlane->getActor() != unk218))
		kill();
}

bool TChuuHana::isCollidMove(THitActor* param_1)
{
	if (param_1->isActorType(0x10000016)) {
		TChuuHana* other = (TChuuHana*)param_1;
		if (other->isRolling()) {
			if (mSpine->getCurrentNerve()
			    == &TNerveChuuHanaWalkOnPanel::theNerve())
				forceRoll();
		} else if (unk1B2 == 0) {
			if (mSpine->getCurrentNerve() != &TNerveChuuHanaAttack::theNerve()
			    && other->mInstanceIndex > mInstanceIndex) {
				TMsRange<s32> chance(0, 100);
				if (chance.rand() % 4 == 0)
					setSafeGoal();
			}
		}
	}

	if (mSpine->getCurrentNerve() == &TNerveChuuHanaObject::theNerve())
		return false;
	return true;
}

bool TChuuHana::isRolling()
{
	if (mSpine->getCurrentNerve() == &TNerveChuuHanaRoll::theNerve())
		return true;
	return false;
}

void TChuuHana::forceRoll()
{
	mSpine->pushNerve(&TNerveChuuHanaRoll::theNerve());
}

void TChuuHana::calcRootMatrix()
{
	gpCurChuuHana = this;

	if (mSpine->getCurrentNerve() == &TNerveChuuHanaJumpPrepare::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveChuuHanaFall2::theNerve()) {
		J3DModel* model = getMActor()->getModel();
		MsMtxSetXYZRPH(model->getBaseTRMtx(), mPosition.x, mPosition.y + unk220,
		               mPosition.z, mRotation.x, mRotation.y, mRotation.z);
		model->setBaseScale(mScaling);
	} else {
		TSpineEnemy::calcRootMatrix();
	}

	if (mSpine->getCurrentNerve() == &TNerveChuuHanaKeepBalance::theNerve())
		gpMarioParticleManager->emitParticleCallBack(
		    PARTICLE_MS_CHU_ASE, &mPosition, 1, &unk228, this);

	if (isBckAnm(6) && getMActor()->getFrameCtrl(0)->checkPass(2.0f)) {
		JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_CHU_JUMP,
		    getMActor()->getModel()->getAnmMtx(mBodyJntIndex), 0, nullptr);
		if (emitter)
			emitter->setGlobalScale(mScaling);
	}
}

void TChuuHana::bind()
{
	if (mSpine->getCurrentNerve() != &TNerveChuuHanaRoll::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveChuuHanaFall2::theNerve()
	    && mSpine->getCurrentNerve()
	           != &TNerveChuuHanaJumpPrepare::theNerve()) {
		TLiveActor::bind();
		return;
	}

	JGeometry::TVec3<f32> next(mPosition);
	next += mLinearVelocity;
	next += mVelocity;

	mVelocity.y -= getGravityY();
	if (mVelocity.y < mVelocityMinY)
		mVelocity.y = mVelocityMinY;

	mGroundHeight = gpMap->checkGround(next.x, next.y + mHeadHeight, next.z,
	                                   &mGroundPlane);
	mGroundHeight += 1.0f;

	if (next.y <= mGroundHeight + 0.05f && mGroundPlane->getActor() == nullptr
	    && mPosition.y < unk1F8.y - 200.0f) {
		offLiveFlag(LIVE_FLAG_AIRBORNE);
		next.y = mGroundHeight;
	} else {
		onLiveFlag(LIVE_FLAG_AIRBORNE);
	}

	gpMap->isTouchedOneWallAndMoveXZ(&next.x, next.y + mHeadHeight, &next.z,
	                                 mBodyRadius);

	mLinearVelocity = next - mPosition;
}

void TChuuHana::margeVelocity(JGeometry::TVec3<f32>& param_1) { }

BOOL TChuuHana::receiveMessage(THitActor* sender, u32 message)
{
	if (unk1A0 == 0
	    && (mSpine->getCurrentNerve() == &TNerveChuuHanaWalkOnPanel::theNerve()
	        || mSpine->getCurrentNerve()
	               == &TNerveChuuHanaKeepBalance::theNerve())
	    && message == HIT_MESSAGE_HIP_DROP) {
		unk1A0 = 1;
	}

	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		if (mSprayedByWaterCooldown == 0) {
			mSprayedByWaterCooldown = 1;
			behaveToWater(sender);
		}

		unk165 = true;
		gpMarioParticleManager->emit(PARTICLE_MS_ENM_WATHIT, &sender->mPosition,
		                             0, nullptr);
		SMSGetMSound()->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK, &mPosition, 0,
		                              0.0f, 0, 0, 4);
		return TRUE;
	}

	return FALSE;
}

void TChuuHana::setWalkAnm()
{
	bool firstAnm = false;
	if (mCurrentBckAnm < 0)
		firstAnm = true;
	setBckAnm(12);
	if (firstAnm)
		getMActor()->getFrameCtrl(0)->setFrame(10.0f * mInstanceIndex);
}

void TChuuHana::kill()
{
	if (!checkLiveFlag(LIVE_FLAG_DEAD)) {
		onLiveFlag(LIVE_FLAG_HIDDEN);
		if (unk218 != nullptr) {
			unk218->receiveMessage(this, HIT_MESSAGE_UNK8);
			unk218 = nullptr;
		}
		TSmallEnemy::kill();
	}
}

void TChuuHana::forceKill()
{
	if ((!mGroundPlane->isIllegalData()
	     && (mGroundPlane->isDeathPlane() || mGroundPlane->isPool()
	         || mGroundPlane->isWaterSurface()))
	    || !gpMap->isInArea(mPosition.x, mPosition.z)
	    || mGroundPlane->isIllegalData())
		kill();
}

f32 TChuuHana::getGravityY() const
{
	f32 gravity = TLiveActor::getGravityY();

	if (mSpine->getCurrentNerve() == &TNerveChuuHanaWalkOnPanel::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveChuuHanaKeepBalance::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveChuuHanaForceJumped::theNerve())
		gravity = unk1B4->mSLWalkGravity.get();
	else if (mSpine->getCurrentNerve() == &TNerveChuuHanaStick::theNerve())
		gravity = unk1B4->mSLWaterHitGravity.get();
	else if (mSpine->getCurrentNerve() == &TNerveChuuHanaFall2::theNerve()
	         || mSpine->getCurrentNerve()
	                == &TNerveChuuHanaJumpPrepare::theNerve())
		gravity = unk1B4->mSLJumpGravity.get();

	return gravity;
}

void TChuuHana::checkOnPanel()
{
	unk1A4++;
	if (unk1A4 > 20) {
		unk1A4 = 0;
		if (willFall(mCheckOnPanelTime))
			unk1A4 = -100;

		if (!isAirborne() && mGroundPlane->getActor() == nullptr
		    && 200.0f + mPosition.y < unk1F8.y)
			mSpine->pushNerve(&TNerveChuuHanaFall2::theNerve());
	}
}

bool TChuuHana::willFall(s32 param_1)
{
	int index  = mInstanceIndex;
	f32 radius = mSmallMirrorR;
	if (index > 0)
		radius = mMediumMirrorR;
	if (index > 2)
		radius = mLargeMirrorR;
	if (param_1 == mCheckOnPanelTimeRoll)
		radius += 250.0f;

	if (unk218 != nullptr) {
		f32 dx   = mPosition.x - unk218->mPosition.x;
		f32 dy   = mPosition.y - unk218->mPosition.y;
		f32 dz   = mPosition.z - unk218->mPosition.z;
		f32 dist = JGeometry::TUtil<f32>::sqrt(dx * dx + dy * dy + dz * dz);

		if (dist > radius) {
			setSafeGoal();
			return true;
		}
	}

	unk1B2 = 0;
	return false;
}

void TChuuHana::setGoal()
{
	JGeometry::TVec3<f32> goal;
	goal.set(mPosition);
	TMsRange<f32> goalAngle(-30.0f, 30.0f);
	f32 angle = goalAngle.rand();

	JGeometry::TVec3<f32> dir(0.0f, 0.0f, 1.0f);
	Mtx mtx;
	MsMtxSetRotRPH(mtx, mRotation.x, mRotation.y + angle, mRotation.z);
	MTXMultVec(mtx, &dir, &dir);

	goal.x += 1000.0f * dir.x;
	goal.z += 1000.0f * dir.z;

	setGoalPath(goal);
	unk1A4 = mCheckOnPanelTime;
	unk1B2 = 0;
}

void TChuuHana::setSafeGoal()
{
	unk1A4 = mCheckOnPanelTime;

	JGeometry::TVec3<f32> point;
	TMsRange<s32> nodeRange(0, getTracer()->getGraph()->getNodeNum());
	getTracer()->getGraph()->getGraphNode(nodeRange.rand()).getPoint(&point);
	TPathNode pathPoint(point);
	setGoalPath(pathPoint);

	unk1B2 = 1;
}

void TChuuHana::rolling() { }

void TChuuHana::rollStart()
{
	unk210 = 0.0f;
	unk204.set(0.0f, 0.0f, 0.0f);
}

void TChuuHana::checkStretchType()
{
	unk1A0 = 0;

	f32 stretch = unk1A8;
	if (mSpine->getCurrentNerve() == &TNerveChuuHanaKeepBalance::theNerve()) {
		int index         = mInstanceIndex;
		f32 reverseHeight = unk1B4->mSLReverseHeightS.get();
		if (index > 0)
			reverseHeight = unk1B4->mSLReverseHeightM.get();
		if (index > 2)
			reverseHeight = unk1B4->mSLReverseHeightL.get();

		if (stretch > reverseHeight) {
			unk1B1 = 1;
			unk214 = 1;
			mSpine->pushNerve(&TNerveChuuHanaFall2::theNerve());
			mSpine->pushNerve(&TNerveChuuHanaJumpPrepare::theNerve());
			return;
		}
	}

	int index         = mInstanceIndex;
	f32 stretchHeight = unk1B4->mSLStretchHeightS.get();
	if (index > 0)
		stretchHeight = unk1B4->mSLStretchHeightM.get();
	if (index > 2)
		stretchHeight = unk1B4->mSLStretchHeightL.get();

	if (stretch > stretchHeight) {
		unk1B1 = 0;
		unk214 = 0;
		setBckAnm(8);
		mSpine->pushNerve(&TNerveChuuHanaForceJumped::theNerve());
		return;
	}

	f32 mediumHeight = unk1B4->mSLMediumStretchHeightS.get();
	if (index > 0)
		mediumHeight = unk1B4->mSLMediumStretchHeightM.get();
	if (index > 2)
		mediumHeight = unk1B4->mSLMediumStretchHeightL.get();

	if (stretch > mediumHeight) {
		unk1B1 = 0;
		unk214 = 0;
		setBckAnm(9);
		mSpine->pushNerve(&TNerveChuuHanaForceJumped::theNerve());
		return;
	}

	f32 smallHeight = unk1B4->mSLSmallStretchHeightS.get();
	if (index > 0)
		smallHeight = unk1B4->mSLSmallStretchHeightM.get();
	if (index > 2)
		smallHeight = unk1B4->mSLSmallStretchHeightL.get();

	if (stretch > smallHeight) {
		unk1B1 = 0;
		unk214 = 0;
		setBckAnm(10);
		mSpine->pushNerve(&TNerveChuuHanaForceJumped::theNerve());
	}
}

void TChuuHana::entryCollision() { }

void TChuuHana::eventKill() { }

void TChuuHana::getEffectMtx() { }

static const char* tyuhana_bastable[] = {
	"/scene/tyuhana/bas/tyuhana_chance_end.bas",
	0,
	"/scene/tyuhana/bas/tyuhana_chance_start.bas",
	"/scene/tyuhana/bas/tyuhana_jump.bas",
	"/scene/tyuhana/bas/tyuhana_push.bas",
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	"/scene/tyuhana/bas/tyuhana_walk.bas",
};

const char** TChuuHana::getBasNameTable() const { return tyuhana_bastable; }

DEFINE_NERVE(TNerveChuuHanaWalkOnPanel, TLiveActor)
{
	TChuuHana* self = (TChuuHana*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setWalkAnm();
		*self->unk21C = 0;
	}

	if (self->unk218 == nullptr) {
		if (self->mGroundPlane->getActor() != nullptr) {
			self->unk1F8 = self->mGroundPlane->getActor()->mPosition;
			self->unk218 = (THitActor*)self->mGroundPlane->getActor();
		}
	} else {
		self->walkBehavior(2, 1.0f);
	}

	self->checkOnPanel();

	if (self->isReachedToGoalXZ())
		self->setGoal();

	if (*self->unk21C) {
		spine->pushAfterCurrent(&TNerveChuuHanaAttack::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveChuuHanaForceJumped, TLiveActor)
{
	TChuuHana* self = (TChuuHana*)spine->getBody();
	if (spine->getTime() == 0)
		self->setSafeGoal();

	if (self->unk214 && self->getCurAnmFrameNo(0) > 80.0f) {
		if (self->mGroundPlane->getActor() != nullptr) {
			THitActor* actor = (THitActor*)self->mGroundPlane->getActor();
			actor->receiveMessage(self, HIT_MESSAGE_SUPER_HIP_DROP);
		}
		self->unk214 = 0;
	}

	if (self->checkCurAnmEnd(0)) {
		if (self->unk1B1) {
			spine->pushAfterCurrent(&TNerveChuuHanaRoll::theNerve());
		} else {
			spine->reset();
			spine->setDefaultNext();
			spine->pushAfterCurrent(spine->getDefault());
		}
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveChuuHanaKeepBalance, TLiveActor)
{
	TChuuHana* self = (TChuuHana*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setBckAnm(2);
		self->mPosition.x -= 10.0f * self->unk1EC.x;
		self->mPosition.z -= 10.0f * self->unk1EC.z;
	} else if (self->checkCurAnmEnd(0)) {
		if (self->isBckAnm(2)) {
			self->setBckAnm(1);
		} else if (self->isBckAnm(1)) {
			if (spine->getTime() > self->unk1B4->mSLKeepBalanceTime.get())
				self->setBckAnm(0);
			else
				self->setBckAnm(1);
		} else if (self->isBckAnm(0) || self->isBckAnm(7)) {
			spine->reset();
			spine->setNext(&TNerveChuuHanaKeepBalance::theNerve());
			spine->pushAfterCurrent(spine->getDefault());
			self->setSafeGoal();
			return true;
		}
	}

	if (TChuuHana::mAttackVersion)
		*self->unk21C = 1;

	if (!TChuuHana::mNewSw && self->mGroundPlane->getActor() == nullptr) {
		spine->pushAfterCurrent(&TNerveChuuHanaFall::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveChuuHanaStick, TLiveActor)
{
	TChuuHana* self = (TChuuHana*)spine->getBody();
	if (spine->getTime() == 0 || !self->isBckAnm(4)) {
		self->setBckAnm(4);
		self->setGoalPath(SMS_GetMarioPos());
		if (TChuuHana::mAttackVersion)
			*self->unk21C = 1;
	} else {
		self->unk224++;
		if (self->unk224 > self->unk1B4->mSLHitWaterTimer.get()) {
			if (self->checkCurAnmEnd(0)) {
				*self->unk21C = 0;
				spine->pushAfterCurrent(&TNerveChuuHanaWait::theNerve());
				return true;
			}
		} else if (self->checkCurAnmEnd(0)) {
			self->setBckAnm(4);
		}
	}

	if (TChuuHana::mNewSw && self->willFall(TChuuHana::mCheckOnPanelTimeRoll)) {
		spine->pushAfterCurrent(&TNerveChuuHanaKeepBalance::theNerve());
		return true;
	}

	self->walkBehavior(3, 0.2f);
	return false;
}

DEFINE_NERVE(TNerveChuuHanaRoll, TLiveActor)
{
	TChuuHana* self = (TChuuHana*)spine->getBody();
	if (spine->getTime() == 0)
		self->rollStart();

	if (self->unk1B0) {
		if (self->willFall(TChuuHana::mCheckOnPanelTimeRoll)) {
			spine->pushAfterCurrent(&TNerveChuuHanaKeepBalance::theNerve());
			return true;
		}
	} else if (spine->getTime() > 5000) {
		return true;
	}

	self->rolling();
	return false;
}

DEFINE_NERVE(TNerveChuuHanaFall, TLiveActor) { return false; }

DEFINE_NERVE(TNerveChuuHanaFall2, TLiveActor)
{
	TChuuHana* self = (TChuuHana*)spine->getBody();
	if (spine->getTime() == 0)
		self->setBckAnm(6);
	self->unk220 *= 0.98f;
	if (!self->isAirborne() || spine->getTime() > 800) {
		spine->pushAfterCurrent(&TNerveChuuHanaObject::theNerve());
		self->kill();
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveChuuHanaObject, TLiveActor) { return false; }

DEFINE_NERVE(TNerveChuuHanaAttack, TLiveActor)
{
	TChuuHana* self = (TChuuHana*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setBckAnm(12);
		self->getMActor()->setFrameRate(2.0f * SMSGetAnmFrameRate(), 0);
		self->setGoalPath(TPathNode((THitActor*)gpMarioAddress));
	}

	if (SMS_GetMarioGroundPlane()->getActor() != self->unk218)
		*self->unk21C = 0;

	if (spine->getTime() > self->unk1B4->mSLAttackTimer.get()) {
		spine->pushAfterCurrent(&TNerveChuuHanaWalkOnPanel::theNerve());
		spine->pushAfterCurrent(&TNerveChuuHanaWait::theNerve());
		return true;
	}

	self->checkOnPanel();

	if (self->isReachedToGoalXZ())
		self->setGoalPath(TPathNode((THitActor*)gpMarioAddress));

	self->walkBehavior(2, self->unk1B4->mSLDashRate.get());
	return false;
}

DEFINE_NERVE(TNerveChuuHanaJumpPrepare, TLiveActor)
{
	TChuuHana* self = (TChuuHana*)spine->getBody();
	if (spine->getTime() == 0)
		self->setBckAnm(3);

	MtxPtr footMtx
	    = self->getMActor()->getModel()->getAnmMtx(TChuuHana::mFootJntIndex);
	self->unk220 = self->mPosition.y - footMtx[1][3];

	if (self->getMActor()->getFrameCtrl(0)->checkPass(10.0f)) {
		JGeometry::TVec3<f32> target;
		target.x = 2.0f * self->unk1F8.x - self->mPosition.x;
		target.y = 2.0f * self->unk1F8.y - self->mPosition.y;
		target.z = 2.0f * self->unk1F8.z - self->mPosition.z;

		*self->unk21C = 0;
		target.y += self->unk1B4->mSLJumpHeight.get();

		f32 jumpSp = self->unk1B4->mSLJumpSp.get();
		JGeometry::TVec3<f32> velocity
		    = self->calcVelocityToJumpToY(target, jumpSp, self->getGravityY());
		velocity.y += 10.0f;
		self->mPosition.y += 10.0f;
		self->mVelocity = velocity;
		self->onLiveFlag(LIVE_FLAG_AIRBORNE);
		self->unk130 = 0;
	}

	if (self->checkCurAnmEnd(0))
		return true;
	return false;
}

DEFINE_NERVE(TNerveChuuHanaWait, TLiveActor)
{
	TChuuHana* self = (TChuuHana*)spine->getBody();
	if (spine->getTime() == 0)
		self->setBckAnm(11);
	if (self->checkCurAnmEnd(0))
		return true;
	return false;
}

#include <MoveBG/MapObjBlock.hpp>

#include <MarioUtil/RumbleMgr.hpp>
#include <Player/MarioAccess.hpp>
#include <MSound/MSound.hpp>
#include <System/Particles.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <Player/ModelWaterManager.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <M3DUtil/MActor.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <System/MarDirector.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

#include <Map/MapCollisionEntry.hpp>
#include <Map/MapCollisionManager.hpp>
#include <System/FlagManager.hpp>

f32 TSandBlock::mSandScaleUp    = 0.075f;
f32 TSandBlock::mSandScaleDown  = 0.0074999998f;
f32 TSandBlock::mSandScaleMin   = 0.05f;
u32 TSandBlock::mWaitTimeToFall = 0x28;
u32 TSandBlock::mSandWaitTime   = 0x190;

f32 TIceBlock::mMeltSpeedWater = 0.003f;
f32 TIceBlock::mMeltSpeedAuto  = 0.004f;
f32 TIceBlock::mAutoMeltScale  = 0.2f;

void TBreakableBlock::touchPlayer(THitActor* player)
{
	if (marioHipAttack()) {
		SMSRumbleMgr->start(0x15, 0x14, mPosition);
		kill();
	}
}

void TSandBlock::touchPlayer(THitActor* player)
{
	if (marioIsOn() && mState == STATE_NORMAL) {
		startStateTimer(mWaitTimeToFall);
		setState(STATE_TOUCHED);
	}
}

void TSandBlock::control()
{
	TMapObjBase::control();

	switch (mState) {
	case STATE_NORMAL:
		break;
	case STATE_RESTORING:
		mScaling.x += mSandScaleUp;
		mScaling.y += mSandScaleUp;
		mScaling.z += mSandScaleUp;
		if (mScaling.y >= mInitialScaling.x) {
			mScaling.set(mInitialScaling);
			setState(STATE_NORMAL);
		}
		break;
	case STATE_TOUCHED:
		if (!isStateTimerEngaged()) {
			setUpMapCollision(1);
			setState(STATE_FALLING);
		}
		break;
	case STATE_FALLING: {
		mScaling.y -= mSandScaleDown;
		gpMSound->startSoundActor(MSD_SE_OBJ_SANDBLOCK_BREAK, &mPosition, 0,
		                          nullptr, 0, 0x4);
		JGeometry::TVec3<f32> particleScale(mScaling.x, mInitialScaling.y,
		                                    mScaling.z);
		emitAndScale(0x147, 0x1, &mPosition, particleScale);
		emitAndScale(0x148, 0x1, &mPosition, particleScale);
		if (mScaling.y < mSandScaleMin) {
			mScaling.x = mScaling.y;
			mScaling.z = mScaling.y;
			TMapObjBase::sleep();
			startStateTimer(mSandWaitTime);
			setState(STATE_GONE);
		}
	} break;
	case STATE_GONE:
		if (!isStateTimerEngaged()
		    && getDistance(SMS_GetMarioPos()) > mScaling.x * 100.0f) {
			TMapObjBase::awake();
			JGeometry::TVec3<f32> scaleCopy = mScaling;
			mScaling.set(mInitialScaling);
			setUpMapCollision(0);
			mScaling.set(scaleCopy);
			setState(STATE_RESTORING);
		}
		break;
	}
}

void TSandBlock::initMapObj()
{
	TMapObjBase::initMapObj();
	mInitialScaling = mScaling;
	SMS_LoadParticle("/scene/mapObj/SandBlockBreakA.jpa", 0x147);
	SMS_LoadParticle("/scene/mapObj/SandBlockBreakB.jpa", 0x148);
}

void TLeanBlock::touchPlayer(THitActor* player)
{
	if (marioIsOn()) {
		unk158 += JGeometry::TVec3<f32>(
		    unk140 * ((gpMarioPos->x - mPosition.x) / unk138), 0.0f,
		    unk140 * ((gpMarioPos->z - mPosition.z) / unk13C));
		unk158.y -= unk144;
	}
}

void TLeanBlock::control()
{
	TMapObjBase::control();

	calcLeanMtx(getModel()->getAnmMtx(0));

	unk158.z = 0.0f;
	unk158.y = 0.0f;
	unk158.x = 0.0f;
}

void TLeanBlock::calcLeanMtx(MtxPtr mtx)
{
	if (unk158.isZero()) {
		unk14C.y += unk144;
	} else {
		unk14C += unk158;
	}

	MsVECNormalize(&unk14C, &unk14C);

	if (unk14C.x == 0.0f)
		(void)unk14C.x; // debug assert?

	JGeometry::TVec3<f32> axis(unk14C.x, 0.0f, unk14C.z);
	rotateVecByAxisY(&axis, 1.5707963f); // 90 deg rad

	f32 length   = MsSqrtf(unk14C.x * unk14C.x + unk14C.z * unk14C.z);
	f32 rotation = length * unk148;
	makeObjMtxRotByAxis(axis, rotation, mtx);
	concatOnlyRotFromRight(mtx, mDefaultMtx, mtx);
}

void TLeanBlock::calcDefaultMtx()
{
	MtxPtr rootMtx = getModel()->getAnmMtx(0);
	getDefaultMtx().set(rootMtx);
}

void TLeanBlock::initMapObj()
{
	TMapObjBase::initMapObj();
	unk140 = 0.01f;
	unk144 = 0.005f;
	unk148 = 1.0f;
	// TODO: Float registers mismatching
	// I see other places are doing scaling._ * 100.0f aswell, possible inline?
	unk138 = mScaling.x * 100.0f * 0.5f;
	unk13C = mScaling.z * 100.0f * 0.5f;
	calcDefaultMtx();
}

TLeanBlock::TLeanBlock(const char* name)
    : TMapObjBase(name)
    , unk138(0.0f)
    , unk13C(0.0f)
    , unk140(0.0f)
    , unk144(0.0f)
    , unk148(0.0f)
    , unk158(0.0f, 0.0f, 0.0f)
{
	unk14C.set(0.0f, 1.0f, 0.0f);

	// TODO: This is a bit forced, but matching
	// Set did not make sense here as is because of order
	mDefaultMtx.ref(2, 3) = 0.0f;
	mDefaultMtx.ref(1, 3) = 0.0f;
	mDefaultMtx.ref(0, 3) = 0.0f;
	mDefaultMtx.ref(1, 2) = 0.0f;
	mDefaultMtx.ref(0, 2) = 0.0f;
	mDefaultMtx.ref(2, 1) = 0.0f;
	mDefaultMtx.ref(0, 1) = 0.0f;
	mDefaultMtx.ref(2, 0) = 0.0f;
	mDefaultMtx.ref(1, 0) = 0.0f;
	mDefaultMtx.ref(2, 2) = 1.0f;
	mDefaultMtx.ref(1, 1) = 1.0f;
	mDefaultMtx.ref(0, 0) = 1.0f;
}

u32 TIceBlock::getSDLModelFlag() const { return 0; }

u32 TIceBlock::touchWater(THitActor* param_1)
{
	const JGeometry::TVec3<f32>& speed = getWaterSpeed(param_1);

	int id = getWaterID(param_1);
	if (gpModelWaterManager->checkFlagBottom4Bits(id, 0x1)) {
		gpMarioParticleManager->emit(0xE7, &param_1->getPosition(), 0, nullptr);
		gpMSound->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK, &mPosition, 0, 0.0f,
		                        0, 0, 4);
		gpMSound->startSoundActor(MSD_SE_OBJ_ICE_BLOCK_MELT, &mPosition, 0,
		                          nullptr, 0, 4);

		mScaling.x -= mMeltSpeedWater;
		mScaling.y -= mMeltSpeedWater;
		mScaling.z -= mMeltSpeedWater;
		mScaledBodyRadius = mScaling.x * mMapObjData->unk30;
		mScaling.y        = MsClamp(mScaling.y, 0.01f, mInitialScaling.y);
		if (mScaling.x < 0.0f) {
			makeObjDead();
		}
		mColCount = 0;
		return 1;
	}
	return 0;
}

void TIceBlock::control()
{
	JPABaseEmitter* emitter
	    = gpMarioParticleManager->emit(0x157, &mPosition, 1, this);
	if (emitter != nullptr) {
		emitter->unk154.x = mScaling.x;
		emitter->unk154.y = mScaling.y;
		emitter->unk154.z = mScaling.z;
	}

	emitter = gpMarioParticleManager->emit(0x158, &mPosition, 1, this);
	if (emitter != nullptr) {
		emitter->unk154.x = mScaling.x;
		emitter->unk154.y = mScaling.y;
		emitter->unk154.z = mScaling.z;
	}

	offHitFlag(HIT_FLAG_NO_COLLISION);
	setDamageRadius(mScaling.x * 80.0f);
	setDamageHeight(mScaling.y * 250.0f);

	if (mScaling.y < mAutoMeltScale * getInitialScaling().y) {
		mScaling.x -= mMeltSpeedAuto;
		mScaling.y -= mMeltSpeedAuto;
		mScaling.z -= mMeltSpeedAuto;
		mScaling.y = MsClamp(mScaling.y, 0.01f, getInitialScaling().y);

		mScaledBodyRadius = mScaling.x * mMapObjData->unk30;

		gpMSound->startSoundActor(MSD_SE_OBJ_ICE_BLOCK_MELT, &mPosition, 0,
		                          nullptr, 0, 4);

		setObjHitData(0);
		onHitFlag(HIT_FLAG_NO_COLLISION);
		removeMapCollision();

		if (mScaling.x < 0.1f)
			makeObjDead();
	}
}

void TIceBlock::calc()
{
	Mtx mtx;
	SMS_GetLightPerspectiveForEffectMtx(mtx);
	J3DModelData* data = getModel()->getModelData();
	J3DTexMtx* info
	    = data->getMaterialNodePointer(0)->getTexGenBlock()->getTexMtx(1);
	info->setEffectMtx(mtx);
}

void TIceBlock::initMapObj()
{
	TMapObjBase::initMapObj();
	SMS_LoadParticle("/scene/mapObj/IceBlockA.jpa", 0x157);
	SMS_LoadParticle("/scene/mapObj/IceBlockB.jpa", 0x158);
}

void TBrickBlock::kill()
{
	makeObjDead();
	emitAndScale(0x60, 0, &mPosition);
	emitAndScale(0x61, 0, &mPosition);
	emitAndScale(0x62, 0, &mPosition);
	gpMSound->startSoundActor(MSD_SE_OBJ_CLASSIC_BLOCK_B, &mPosition, 0,
	                          nullptr, 0, 4);
	SMSRumbleMgr->start(0x15, 0x14, &mPosition);
	appearObj(100.0f);
}

BOOL TBrickBlock::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->isActorType(0x80000001) && marioHeadAttack()) {
		kill();
		return TRUE;
	} else if (sender->isActorType(0x8000005)
	           && message == HIT_MESSAGE_ATTACK) {
		kill();
		return TRUE;
	}
	return FALSE;
}

void TBrickBlock::initMapObj()
{
	TMapObjBase::initMapObj();
	SMS_LoadParticle("/scene/mapObj/BrickBlockA.jpa", 0x60);
	SMS_LoadParticle("/scene/mapObj/BrickBlockB.jpa", 0x61);
	SMS_LoadParticle("/scene/mapObj/BrickBlockC.jpa", 0x62);
}

void TJuiceBlock::initMapObj()
{
	TMapObjBase::initMapObj();
	SMS_InitPacket_OneTevColor(mMActor->getModel(), 0, GX_TEVREG0, &unk138);
	unk140.set(1.0f, 1.0f, 1.0f);
}

void TJuiceBlock::moveObject()
{
	TLiveActor::moveObject();
	if (unk14C != 0 && unk14C->checkLiveFlag(LIVE_FLAG_DEAD)) {
		kill();
	}
}

void TJuiceBlock::kill()
{
	unk14C = 0;
	makeObjDead();
}

void TTelesaBlock::initMapObj() { TMapObjBase::initMapObj(); }

void TTelesaBlock::perform(u32 cue, JDrama::TGraphics* graphics)
{
	mLiveFlag &= ~LIVE_FLAG_UNK200;
	if (!gpMarDirector->isTalkModeNow()) {
		TMapObjBase::perform(cue, graphics);
	} else {
		if (cue & CUE_MOVE) {
			TJuiceBlock::moveObject();
		}
		mMActor->perform(cue, graphics);
	}

	if (cue & CUE_CALC_ANIM) {

		// TODO: Possibly more TRotation3f inlines?
		TRotation3f mtx;
		mtx.ref(0, 3) = 0.0f;
		mtx.ref(1, 3) = 0.0f;
		mtx.ref(2, 3) = 0.0f;
		mtx.setScale(unk140.x, unk140.y, unk140.z);
		PSMTXConcat(getModel()->getAnmMtx(1), mtx, getModel()->getAnmMtx(1));

		mtx.setScale(unk140.y, unk140.y, unk140.z);
		PSMTXConcat(getModel()->getAnmMtx(0), mtx, getModel()->getAnmMtx(0));
	}
}

void TTelesaBlock::setGroundCollision()
{
	if (!mMapCollisionManager)
		return;

	if (!mMapCollisionManager->getUnk8())
		return;

	mMapCollisionManager->getUnk8()->moveSRT(mPosition, mRotation, unk140);
}

BOOL TSuperHipDropBlock::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_SUPER_HIP_DROP) {
		kill();
		if (mMonteBlockBroken)
			TFlagManager::getInstance()->setBool(true, 0x1038C);

		SMSGetMSound()->startSoundActor(MSD_SE_OBJ_SUPERBLOCK_BREAK, &mPosition,
		                                0, nullptr, 0, 4);
		return TRUE;
	}
	return FALSE;
}

void TSuperHipDropBlock::loadAfter()
{
	TBreakHideObj::loadAfter();

	if (strcmp("モンテゲートブロック", mName) == 0) {
		mMonteBlockBroken = true;
		if (TFlagManager::getInstance()->getBool(0x1038C)) {
			makeObjDead();
		}
		onLiveFlag(LIVE_FLAG_UNK8);
	}
}

#include <MoveBG/MapObjItem2.hpp>
#include <M3DUtil/MActor.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <Player/MarioAccess.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <System/EmitterViewObj.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

TMushroom1up::TMushroom1up(int param_1, const char* name)
    : TMapObjBase(name)
    , unk138(0)
    , unk139(param_1)
    , unk13A(0)
    , unk13C(0)
{
}

void TMushroom1up::touchPlayer(THitActor* param_1)
{
	if (unk13A == 1)
		return;

	if (!param_1->receiveMessage(this, HIT_MESSAGE_ATTACK))
		return;

	unk13C = 0;
	unk13A = 1;
	SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_1UP, 0, nullptr, 0);
	mGroundPlane = TMap::getIllegalCheckData();
}

void TMushroom1up::makeObjAppeared()
{
	TMapObjBase::makeObjAppeared();
	mStateTimer = 1200;
	unk138      = 0;
	unk13A      = 0;
	if (unk139 != 2)
		SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_1UP_APPEAR, 0, nullptr, 0);

	JPABaseEmitter* emitter = gpMarioParticleManager->emit(
	    PARTICLE_MS_ENM_DISAP_A_W, &mPosition, 0, nullptr);
	if (emitter)
		emitter->setScale(mScaling);

	emitter = gpMarioParticleManager->emit(PARTICLE_MS_ENM_DISAP_B, &mPosition,
	                                       0, nullptr);
	if (emitter)
		emitter->setScale(mScaling);
}

void TMushroom1up::initMapObj()
{
	TMapObjBase::initMapObj();
	mGravity = 0.35f;
	offLiveFlag(LIVE_FLAG_AIRBORNE | LIVE_FLAG_UNK10 | LIVE_FLAG_HIDDEN
	            | LIVE_FLAG_DEAD);
	if (unk139 == 2) {
		onLiveFlag(LIVE_FLAG_UNK10);
		makeObjAppeared();
	}
	mScaling.set(1.5f, 1.5f, 1.5f);
}

void TMushroom1up::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);
	offLiveFlag(LIVE_FLAG_AIRBORNE | LIVE_FLAG_UNK10);
}

void TMushroom1up::control()
{
	TMapObjBase::control();

	if (unk13A == 1) {
		int t = 180 - unk13C;
		if (t < 0) {
			kill();
			return;
		}

		JGeometry::TVec3<f32> pos = SMS_GetMarioPos();
		pos.x += 1.5f * (50.0f * JMACos(5.0f * t));
		pos.y += 200.0f;
		pos.z += 1.5f * (50.0f * JMASin(5.0f * t));
		mPosition.set(pos);

		mScaling.set(1.5f, 1.5f, 1.5f);
		mLinearVelocity.zero();
		mVelocity.zero();
		unk13C++;
		return;
	}

	unk13C++;
	if (unk139 == 2) {
		mLinearVelocity.zero();
		mVelocity.zero();
		return;
	}

	if (unk138 == 0) {
		if (isAirborne())
			return;
		unk138 = 1;
	}

	JGeometry::TVec3<f32> diff = SMS_GetMarioPos();
	diff -= mPosition;
	diff.y = 0.0f;
	if (diff.isZero())
		diff.x = 1.0f;
	if (unk139 == 1)
		diff.negate();

	f32 angle = MsGetRotFromZaxisY(diff);

	f32 delta = MsAngleDiff(angle, mRotation.y);
	f32 step;
	if (delta > 0.0f)
		step = MsClamp(delta, -1.0f, 1.0f);
	else
		step = MsClamp(delta, -1.0f, 1.0f);

	mRotation.y = MsWrap(mRotation.y + step, 0.0f, 360.0f);

	VECNormalize(&diff, &diff);
	diff.scale(3.8f);
	mLinearVelocity.add(diff);
}

void TMushroom1up::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (unk139 != 2 && mStateTimer < 240 && (cue & CUE_ENTRY)
	    && gpMarDirector->unk58 % 6 > 2)
		cue &= ~CUE_ENTRY;

	if ((cue & CUE_MOVE) && unk13A == 0 && unk139 != 2 && mStateTimer <= 0)
		kill();

	TMapObjBase::perform(cue, graphics);
}

TJumpBase::TJumpBase(const char* name)
    : TMapObjBase(name)
{
	unk138 = 2;
}

void TJumpBase::initMapObj()
{
	TMapObjBase::initMapObj();
	if (mMapCollisionManager) {
		TMapCollisionBase* base = mMapCollisionManager->unk8;
		base->setAllBGType(7);
		base->setAllActor(this);
		base->setAllData(0x2710);
	}
	unkE8 = 0;
}

void TJumpBase::ensureTakeSituation()
{
	if (mHeldObject && mHeldObject->mHolder != this)
		mHeldObject = nullptr;
	if (mHolder && mHolder->mHeldObject != this)
		mHolder = nullptr;
}

BOOL TJumpBase::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->isActorType(0x80000001)) {
		if (message == HIT_MESSAGE_TAKE) {
			if (unk138 == 0) {
				mHolder = (TTakeActor*)sender;
				onHitFlag(HIT_FLAG_NO_COLLISION);
				if (mMapCollisionManager && mMapCollisionManager->unk8)
					mMapCollisionManager->unk8->remove();
				return TRUE;
			}
		} else if (message == HIT_MESSAGE_UNK8) {
			mHolder = nullptr;
			unk13C  = 0;
			unk138  = 2;
			return TRUE;
		} else if (message == HIT_MESSAGE_PUT) {
			mHolder = nullptr;
			unk13C  = 0;
			unk138  = 2;
			return TRUE;
		} else if (message == HIT_MESSAGE_THROWN) {
			mHolder = nullptr;
			unk13C  = 0;
			unk138  = 5;
			return TRUE;
		} else if (message == HIT_MESSAGE_TRAMPLE) {
			unk13C = 0;
			unk138 = 4;
			return TRUE;
		}
	}

	if (sender->isActorType(0x1000001) && unk138 == 3) {
		unk13C = 0;
		unk138 = 1;
		return TRUE;
	}

	return FALSE;
}

Mtx* TJumpBase::getRootJointMtx() const
{
	return mMActor->getModel()->mNodeMatrices;
}

void TJumpBase::calcRootMatrix()
{
	if (getHolder() != nullptr) {
		J3DModel* model = getModel();
		MtxPtr mtx      = getHolder()->getTakingMtx();
		MTXCopy(mtx, model->unk20);
		model->unk14 = mScaling;
		mPosition.set(mtx[0][3], mtx[1][3], mtx[2][3]);
		return;
	}
	TMapObjBase::calcRootMatrix();
}

void TJumpBase::control()
{
	int prevState = unk138;
	if (!isAirborne())
		onLiveFlag(LIVE_FLAG_UNK10);

	switch (unk138) {
	case 0:
		if (unk13C == 0) {
			getMActor()->setBck("jumpbase_shrink");
			J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(0);
			if (ctrl) {
				ctrl->setFrame((f32)ctrl->getEnd());
				ctrl->setRate(0.0f);
			}
			mScaledBodyRadius = 50.0f;
		}
		break;

	case 3:
		if (unk13C == 0) {
			offHitFlag(HIT_FLAG_NO_COLLISION);
			if (mMapCollisionManager)
				mMapCollisionManager->getUnk8()->setUp();

			getMActor()->setBck("jumpbase_set");
			J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(0);
			if (ctrl) {
				ctrl->setFrame((f32)ctrl->getEnd());
				ctrl->setRate(0.0f);
			}
		}
		break;

	case 2:
		if (unk13C == 0) {
			getMActor()->setBck("jumpbase_set");
			J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(0);
			if (ctrl) {
				ctrl->setFrame(0.0f);
				ctrl->setRate(SMSGetAnmFrameRate());
			}
			offLiveFlag(LIVE_FLAG_UNK10);
			mScaledBodyRadius = 100.0f;
		}
		if (getMActor()->curAnmEndsNext(0, nullptr)) {
			unk13C = 0;
			unk138 = 3;
		}
		break;

	case 1:
		if (unk13C == 0) {
			if (mMapCollisionManager && mMapCollisionManager->getUnk8())
				mMapCollisionManager->getUnk8()->remove();

			getMActor()->setBck("jumpbase_shrink");
			J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(0);
			if (ctrl) {
				ctrl->setFrame(0.0f);
				ctrl->setRate(SMSGetAnmFrameRate());
			}
		}
		if (getMActor()->curAnmEndsNext(0, nullptr)) {
			unk13C = 0;
			unk138 = 0;
		}
		break;

	case 4:
		if (unk13C == 0) {
			getMActor()->setBck("jumpbase_jump");
			J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(0);
			if (ctrl) {
				ctrl->setFrame(0.0f);
				ctrl->setRate(SMSGetAnmFrameRate());
			}
		}
		if (getMActor()->curAnmEndsNext(0, nullptr)) {
			unk13C = 0;
			unk138 = 3;
		}
		break;

	case 5:
		if (unk13C == 0) {
			onLiveFlag(LIVE_FLAG_AIRBORNE);
			int angle = *gpMarioAngleY;
			mVelocity
			    = JGeometry::TVec3<f32>(JMASSin(angle), 0.0f, JMASCos(angle));
			JGeometry::TVec3<f32> v2 = mVelocity;
			mPosition += v2;
			offLiveFlag(LIVE_FLAG_UNK10);
		}
		if (!isAirborne()) {
			unk13C = 0;
			unk138 = 2;
		}
		break;
	}

	if (unk138 == prevState) {
		unk13C++;
		if (unk13C == 0)
			unk13C = 1;
	}

	TMapObjBase::control();

	if (mGroundPlane) {
		if (mGroundPlane->isIllegalData() || mGroundPlane->isWaterSurface()) {
			makeObjDead();
			makeObjDefault();
			makeObjAppeared();
		}
	}

	if (mHolder) {
		mGroundPlane  = SMS_GetMarioGroundPlane();
		mGroundHeight = SMS_GetMarioPos().y;
	}
}

#include <algorithm>
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

// Parked: a binding level over TMarDirector's frame counter. The map shows no
// such accessor, so it stays TU-local; `TMushroom1up::perform` needs exactly
// this one binding expansion (+16 of low region) on top of the two
// `getStateTimer()` reads (+8 together) to reach retail's 0x40 frame. Header
// batch item: a binding `TMarDirector::getFrameCounter()` would serve it.
static inline int MapObjItem2GetDirectorFrame(TMarDirector* director)
{
	int frame = director->unk58;
	return frame;
}

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
		emitter->setGlobalScale(getScaling());

	emitter = gpMarioParticleManager->emit(PARTICLE_MS_ENM_DISAP_B, &mPosition,
	                                       0, nullptr);
	if (emitter)
		emitter->setGlobalScale(getScaling());
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

// TODO: 99.9%, all instructions match, 34 operand-only markers (closure batch
// 123 made it instruction-exact; batch 128 mapped the frame). The residue is
// now fully localised: retail's frame is 0x88 with three 12-byte named slots
// packed at 0x44 (`diff`), 0x50 (`pos`) and 0x5c (unreferenced), 16 bytes of
// int<->float magic doubles at 0x68/0x70, and 56 bytes of inline-temp pool
// (0xc..0x43). Ours is 0x80 with `pos` at 0x50 and `diff` at 0x40 and only 52
// pool bytes. Two independent items:
//   (a) a dead 12-byte local declared before the Mario-follow block fills
//       retail's 0x5c slot and lands the frame at 0x88 exactly (52 -> 34
//       markers, zero instruction change) -- positional evidence only, no
//       candidate the function plausibly wanted, so not committed;
//   (b) after that, the only remaining difference is `diff` at 0x40 vs 0x44,
//       i.e. retail's inline-temp pool is exactly 4 bytes bigger. Measured
//       here: every accessor rung is +8, not +4 (`getPosition()` in the
//       `diff -= mPosition` line, `getRotation().y` at the MsAngleDiff site or
//       at the MsWrap site: each 0x88 -> 0x90); a dead 4-byte local declared
//       last is +8; `SMS_GetMarioPos()` bound to a `const TVec3&`/`const
//       TVec3*` local, or spelled as raw `*gpMarioPos`, is +0 at the `diff`
//       site and costs 12 markers at the `pos` site. So this needs one of the
//       catalogued "+4 low" causes (a global-accessor level per read site, a
//       named cast intermediate), none of which this function has a site for.
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
		f32 deg = 5.0f * t;
		// The y offset comes first: it owns the lower literal id
		// (@3312) than the 1.5f of the x/z lines.
		pos.y += 200.0f;
		pos.x += 1.5f * (50.0f * JMACos(deg));
		pos.z += 1.5f * (50.0f * JMASin(deg));
		mPosition = pos;

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
	// std::min/std::max rather than MsClamp: the const-reference
	// parameters are what put 1.0f and -1.0f in .sdata, not .sdata2.
	if (delta > 0.0f)
		step = std::min(delta, 1.0f);
	else
		step = std::max(delta, -1.0f);

	mRotation.y = MsWrap(mRotation.y + step, 0.0f, 360.0f);

	VECNormalize(&diff, &diff);
	diff.scale(3.8f);
	mLinearVelocity.add(diff);
}

void TMushroom1up::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (unk139 != 2 && getStateTimer() < 240 && (cue & CUE_ENTRY)
	    && MapObjItem2GetDirectorFrame(gpMarDirector) % 6 > 2)
		cue &= ~CUE_ENTRY;

	if ((cue & CUE_MOVE) && unk13A == 0 && unk139 != 2 && getStateTimer() <= 0)
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
		TMapCollisionBase* base = mMapCollisionManager->getUnk8();
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
	return (Mtx*)mMActor->getModel()->getAnmMtx(0);
}

void TJumpBase::calcRootMatrix()
{
	if (getHolder() != nullptr) {
		J3DModel* model = getModel();
		MtxPtr mtx      = getHolder()->getTakingMtx();
		model->setBaseTRMtx(mtx);
		model->setBaseScale(mScaling);
		mPosition.set(mtx[0][3], mtx[1][3], mtx[2][3]);
		return;
	}
	TMapObjBase::calcRootMatrix();
}

// TODO: 97.2%, instruction count now exact (326/326) with 110 operand-only
// markers. Closure batch 128 restored the missing `unk13C = 0; unk138 = 2;`
// pair -- it belongs to the *ground-plane* block at the end of the function
// (after makeObjDead/makeObjDefault/makeObjAppeared), not to case 5 as batch
// 123's note guessed. Two items left:
// (1) `this` and the `.rodata` string-pool base are swapped between r29 and
//     r31 for the whole body (retail: r29 = `this`, r30 = `prevState`, r31 =
//     pool base and later the per-case `ctrl`; ours has `this` in r31 and the
//     pool in r29, with the prologue `mr` emitted one save earlier). This is
//     the "retail ranks the .rodata pool address above `this`" item from
//     frame-gaps.md batch 110; the liveness rule does not cover it. Moving
//     `int prevState = unk138;` below the isAirborne block is worse
//     (97.2 -> 96.5).
// (2) case 5's `JMASSin(angle)`/`JMASCos(angle)` share one `sraw`/`slwi`
//     index derivation where retail derives it twice from a single `lha` +
//     `clrlwi` (the codegen-tells.md batch-56 tell). Not steerable from here:
//     `JMASSin(*gpMarioAngleY)` twice (95.9, +1 instruction and +8 frame),
//     `s16 angle` (95.9), and two separate `s16 sinAngle/cosAngle` locals
//     (95.9) are all worse than the shared `int angle`.
void TJumpBase::control()
{
	int prevState = unk138;
	if (!isAirborne())
		onLiveFlag(LIVE_FLAG_UNK10);

	switch (unk138) {
	case 0:
		if (unk13C == 0) {
			getMActor()->setBck("jumpbase_shrink");
			J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
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
			J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
			if (ctrl) {
				ctrl->setFrame((f32)ctrl->getEnd());
				ctrl->setRate(0.0f);
			}
		}
		break;

	case 2:
		if (unk13C == 0) {
			getMActor()->setBck("jumpbase_set");
			J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
			if (ctrl) {
				ctrl->setFrame(0.0f);
				ctrl->setRate(SMSGetAnmFrameRate());
			}
			offLiveFlag(LIVE_FLAG_UNK10);
			mScaledBodyRadius = 100.0f;
		}
		if (getMActor()->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
			unk13C = 0;
			unk138 = 3;
		}
		break;

	case 1:
		if (unk13C == 0) {
			if (mMapCollisionManager && mMapCollisionManager->getUnk8())
				mMapCollisionManager->getUnk8()->remove();

			getMActor()->setBck("jumpbase_shrink");
			J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
			if (ctrl) {
				ctrl->setFrame(0.0f);
				ctrl->setRate(SMSGetAnmFrameRate());
			}
		}
		if (getMActor()->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
			unk13C = 0;
			unk138 = 0;
		}
		break;

	case 4:
		if (unk13C == 0) {
			getMActor()->setBck("jumpbase_jump");
			J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
			if (ctrl) {
				ctrl->setFrame(0.0f);
				ctrl->setRate(SMSGetAnmFrameRate());
			}
		}
		if (getMActor()->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
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
			unk13C = 0;
			unk138 = 2;
		}
	}

	if (mHolder) {
		mGroundPlane  = SMS_GetMarioGroundPlane();
		mGroundHeight = SMS_GetMarioPos().y;
	}
}

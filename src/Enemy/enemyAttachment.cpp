#include <Enemy/EnemyAttachment.hpp>
#include <Enemy/Conductor.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/SharedParts.hpp>
#include <System/MarDirector.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <M3DUtil/MActor.hpp>
#include <Player/MarioAccess.hpp>
#include <Camera/Camera.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/PollutionManager.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>

TEnemyAttachment::TEnemyAttachment(const char* name)
    : TSpineEnemy(name)
    , unk154(0)
    , unk158(0)
    , unk160(nullptr)
    , unk164(1.0f)
    , unk168(0)
{
}

void TEnemyAttachment::loadInit(TSpineEnemy* param_1, const char* param_2)
{
	unk160        = param_1;
	mMActorKeeper = new TMActorKeeper(unk160->getManager(), 1);
	mMActor       = mMActorKeeper->createMActor(param_2, 3);
}

void TEnemyAttachment::behaveToHitGround()
{
	unk168 = 1;
	offLiveFlag(LIVE_FLAG_AIRBORNE);
	mVelocity.x = 0.0f;
	mVelocity.y = -0.3f;
	mVelocity.z = 0.0f;
	rebirth();
}

void TEnemyAttachment::behaveToHitWall(const TBGCheckData*) { rebirth(); }

void TEnemyAttachment::forceKill()
{
	if (!mGroundPlane->isPool() && gpMap->isInArea(mPosition.x, mPosition.z))
		return;

	kill();

	if (unk160->checkLiveFlag(LIVE_FLAG_HIDDEN)) {
		unk160->kill();
		unk160->onLiveFlag(LIVE_FLAG_UNK20000);
	}
}

void TEnemyAttachment::recoverScale()
{
	if (mScaling.x < unk164) {
		VECScale(&mScaling, &mScaling, 2.0f);
	} else {
		mScaling.x = mScaling.y = mScaling.z = unk164;
	}
}

void TEnemyAttachment::bind()
{
	JGeometry::TVec3<f32> local_1C = mPosition;
	local_1C += mLinearVelocity;
	local_1C += mVelocity;
	recoverScale();
	mVelocity.y -= getNowGravity();
	if (mVelocity.y < mVelocityMinY)
		mVelocity.y = mVelocityMinY;
	if (!unk168) {
		mGroundHeight = gpMap->checkGround(local_1C.x, local_1C.y + mHeadHeight,
		                                   local_1C.z, &mGroundPlane);
		mGroundHeight += 1.0f;
	}

	f32 nextY = local_1C.y;
	if (nextY + mVelocity.y <= mGroundHeight)
		behaveToHitGround();
	else
		onLiveFlag(LIVE_FLAG_AIRBORNE);

	TBGWallCheckRecord local_48(local_1C.x, nextY + mHeadHeight, local_1C.z,
	                            2.0f * mBodyRadius, 1, 0);
	if (gpMap->isTouchedWallsAndMoveXZ(&local_48)) {
		const TBGCheckData* wall = local_48.mResultWalls[0];
		behaveToHitWall(wall);
	}

	mPosition = local_1C;
	// Retail really does subtract the position it has just written, so the
	// linear velocity always ends up zero here. The by-value left operand of
	// operator- is what keeps TVec3::sub a `bl`.
	// TODO: the operator- temporary lands at 0x24 where retail puts it at 0x10
	// (frame and every instruction otherwise exact); ours allocates ~20 bytes
	// of low region ahead of it.
	mLinearVelocity = local_1C - mPosition;

	setBehavior();
	forceKill();

	mPosition += mLinearVelocity;
}

void TEnemyAttachment::rebirth()
{
	unk150 = 0;
	unk158 = 0;
	onHitFlag(HIT_FLAG_NO_COLLISION);
	mVelocity.y = 0.0f;
}

void TEnemyAttachment::kill()
{
	unk150 = 0;
	unk158 = 0;
	onHitFlag(HIT_FLAG_NO_COLLISION);
}

void TEnemyAttachment::set()
{
	mPosition = unk160->getPosition();
	mRotation = unk160->getRotation();
}

void TEnemyAttachment::moveObject()
{
	if (unk150 == 1) {
		set();
		setBehavior();
	} else {
		calcRideMomentum();
		sendMessage();
		bind();
		calcRidePos();
	}
}

void TEnemyAttachment::sendMessage()
{
	for (int i = 0; i < mColCount; ++i) {
		if (mCollisions[i]->isActorType(0x80000001)) {
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
			continue;
		}

		if (mCollisions[i] != unk160) {
			kill();
		}
	}
}

void TEnemyAttachment::calcRootMatrix()
{
	MsMtxSetXYZRPH(getMActor()->getModel()->getBaseTRMtx(), mPosition.x,
	               mPosition.y, mPosition.z, mRotation.x, mRotation.y,
	               mRotation.z);
	getMActor()->getModel()->setBaseScale(mScaling);
}

void TEnemyAttachment::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (unk150 == nullptr) {
		if (cue & CUE_CALC_ANIM)
			behaveToHost();
		return;
	}

	if (!SMSGetMarDirector()->isDemoModeNow()
	    && SMSGetMarDirector()->isTalkModeNow()) {
		performOnlyDraw(cue, graphics);
		return;
	}

	if (cue & CUE_MOVE)
		moveObject();

	if (cue & CUE_CALC_ANIM) {
		calcRootMatrix();
		getMActor()->calcAnm();
	}

	if (cue & CUE_CALC_VIEW)
		getMActor()->viewCalc();

	if (cue & CUE_ENTRY)
		getMActor()->entry();
}

void TEnemyPolluteModelManager::init(TLiveActor* param_1)
{
	unk18 = new TEnemyPolluteModel*[unk14];
}

void TEnemyPolluteModelManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_CALC_ANIM) {
		f32 f31     = 100.0f;
		f32 farClip = gpConductor->getCondParams().mEnemyFarClip.get();
		SetViewFrustumClipCheckPerspective(gpCamera->getFovy(),
		                                   gpCamera->getAspect(),
		                                   graphics->getNearPlane(), farClip);

		for (int i = 0; i < unk14; ++i) {
			if (unk18[i]->unk5D) {
				if (ViewFrustumClipCheck(graphics, &unk18[i]->unk44, f31))
					unk18[i]->unk5C = false;
				else
					unk18[i]->unk5C = true;
			}
		}
	}

	for (int i = 0; i < unk14; ++i)
		unk18[i]->perform(cue, graphics);
}

// TODO: fabricated inline level. Retail calls TBGCheckData::isWaterSurface()
// out of line here (it is a weak header inline emitted in BeeHive.o), which
// only happens when the call sits one level below the guard; spelling
// `check->isWaterSurface()` directly expands it and costs 13 instructions.
// The real helper is unrecoverable from the map (enemyAttachment.cpp lists no
// UNUSED symbol but `generate`), so it is parked here.
static inline bool EnemyAttachmentIsWaterSurface(const TBGCheckData* check)
{
	return check->isWaterSurface();
}

void TEnemyPolluteModelManager::generatePolluteModel(
    JGeometry::TVec3<f32>& param_1, JGeometry::TVec3<f32>& param_2)
{
	TEnemyPolluteModel* model = unk18[unk10];

	const TBGCheckData* check;
	gpMap->checkGround(param_1, &check);
	if (!check->checkFlag(BG_CHECK_FLAG_ILLEGAL)
	    && !EnemyAttachmentIsWaterSurface(check))
		model->generate(param_1, param_2);

	++unk10;
	if (unk10 >= unk14)
		unk10 = 0;
}

TEnemyPolluteModel::TEnemyPolluteModel(TLiveActor* param_1, int param_2,
                                       SDLModelData* param_3, const char* name)
    : JDrama::TViewObj(name)
    , unk5C(1)
    , unk5D(0)
{
	unk10 = new TSharedParts(param_1, param_2, param_3, 3);
}

// TODO: frame 0x48 vs retail's 0x80 -- 56 bytes of dead low region, plus a
// callee-saved swap (retail keeps `cue` in r31 and `this` in r30). trash[56]
// lands the frame and leaves the swap, so the two are independent.
void TEnemyPolluteModel::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (!unk5D || unk5C)
		return;

	if (cue & CUE_CALC_ANIM) {
		if (unk10->unk18->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
			unk5D = false;
			return;
		}

		unk10->unk18->getModel()->setBaseTRMtx(unk14);
		unk10->unk18->getModel()->setBaseScale(unk50);
		unk10->unk18->calcAnm();
	}

	if (cue & CUE_ENTRY)
		gpPollution->stampModel(unk10->unk18->getModel());
}

void TEnemyPolluteModel::generate(JGeometry::TVec3<f32>& param_1,
                                  JGeometry::TVec3<f32>& param_2)
{
	unk44 = param_1;
	unk50 = param_2;

	TPosition3f TStack_58;
	TStack_58.translation(param_1.x, param_1.y, param_1.z);
	unk14.translation(param_1.x, param_1.y, param_1.z);
	unk10->unk18->getModel()->setBaseTRMtx(TStack_58);
	unk5D = true;
	unk5C = false;
	setAnm();
}

#include <MoveBG/MapObjBianco.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <MoveBG/MapObjMessenger.hpp>
#include <System/Particles.hpp>
#include <Camera/cameralib.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <M3DUtil/MActor.hpp>
#include <Map/Map.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Map/MapData.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/WaterGun.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JMath.hpp>
#include <stdio.h>
#include <string.h>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

/// Distance from the big windmill's hub to each of its four blocks.
static f32 sRadius = 2380.0f;
/// How far in front of the bridge the blocks ride.
static f32 sSubZ = 150.0f;
/// Degrees per frame the big windmill turns.
static f32 sSpeed = 0.05f;
/// Phase offset shared by the big windmill's blocks; never written.
static f32 sAngleAdd;

/// Fabricated: the two rotation matrices this file fills by hand. No helper
/// survives in the map, so it was not a real out-of-line function; this shape
/// is what MWCC emits at every call site.
static void setMtxRotZ(MtxPtr mtx, f32 degrees)
{
	f32 sin   = JMASSin((s16)(182.04445f * degrees));
	f32 cos   = JMASCos((s16)(182.04445f * degrees));
	mtx[0][0] = cos;
	mtx[0][1] = -sin;
	mtx[0][2] = 0.0f;
	mtx[0][3] = 0.0f;
	mtx[1][0] = sin;
	mtx[1][1] = cos;
	mtx[1][2] = 0.0f;
	mtx[1][3] = 0.0f;
	mtx[2][0] = 0.0f;
	mtx[2][1] = 0.0f;
	mtx[2][2] = 1.0f;
	mtx[2][3] = 0.0f;
}

static void setMtxRotY(MtxPtr mtx, f32 degrees)
{
	f32 sin   = JMASSin((s16)(182.04445f * degrees));
	f32 cos   = JMASCos((s16)(182.04445f * degrees));
	mtx[0][0] = cos;
	mtx[0][1] = 0.0f;
	mtx[0][2] = sin;
	mtx[0][3] = 0.0f;
	mtx[1][0] = 0.0f;
	mtx[1][1] = 1.0f;
	mtx[1][2] = 0.0f;
	mtx[1][3] = 0.0f;
	mtx[2][0] = -sin;
	mtx[2][1] = 0.0f;
	mtx[2][2] = cos;
	mtx[2][3] = 0.0f;
}

void TBigWindmill::control()
{
	TMapObjBase::control();

	mRotation.z -= sSpeed;
	mRotation.z = MsWrap(mRotation.z, 0.0f, 360.0f);
	setRootMtxRotZ();

	gpMSound->startSoundActorWithInfo(MSD_SE_OBJ_BI_WINDMILLTOWER, &mPosition,
	                                  nullptr, fabsf(sSpeed), 0, 0,
	                                  &mSoundHandle, 0, 4);

	f32 angle = mRotation.z + sAngleAdd;
	for (int i = 0; i < 4; ++i) {
		f32 rad    = 0.017453294f * angle;
		MtxPtr mtx = mBlocks[i]->getModel()->getAnmMtx(0);
		mtx[0][3]  = sRadius * cosf(rad) + getPosition().x;
		f32 offset = mYOffset;
		angle += 90.0f;
		mtx[1][3] = sRadius * sinf(rad) + getPosition().y - offset;
		mtx[2][3] = getPosition().z - sSubZ;
		mBlocks[i]->mPosition.set(mtx[0][3], mtx[1][3], mtx[2][3]);
		if (angle > 360.0f)
			angle -= 360.0f;
	}
}

void TBigWindmill::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);

	for (int i = 0; i < 4; ++i) {
		JGeometry::TVec3<f32> scale(1.0f, 1.0f, 1.0f);
		JGeometry::TVec3<f32> rotation(0.0f, 0.0f, 0.0f);
		JGeometry::TVec3<f32> position(0.0f, 0.0f, 0.0f);
		mBlocks[i] = TMapObjBaseManager::newAndRegisterObj(
		    "bigWindmillBlock", position, rotation, scale);
		mBlocks[i]->appear();
		mBlocks[i]->getModel()->calc();
	}
}

f32 TMapObjRootPakkun::mTremblePower = 15.0f;
f32 TMapObjRootPakkun::mTrembleAccel = 0.95f;
f32 TMapObjRootPakkun::mTrembleBrake = 0.98f;
int TMapObjRootPakkun::mTrembleTime  = 360;

// Binding level worth +8 of low region, landing
// TMapObjRootPakkun::drawObject's frame at 0x28 (batch 121).
static inline bool MapObjBiancoIsStateTimerEngaged(TMapObjRootPakkun* p)
{
	bool stateTimerEngaged = p->isStateTimerEngaged();
	return stateTimerEngaged;
}

void TMapObjRootPakkun::drawObject(JDrama::TGraphics* graphics)
{
	TLiveActor::drawObject(graphics);

	// Only worth animating while Mario is on this side of the valley.
	if (fabsf(gpMarioPos->z - mPosition.z) < 10000.0f) {
		mTrembleEffect->movement();
		if (!MapObjBiancoIsStateTimerEngaged(this)) {
			mTrembleEffect->tremble(mTremblePower, mTrembleAccel,
			                        mTrembleBrake, mTrembleTime);
			mStateTimer = mTrembleTime;
		}
	}
}

void TMapObjRootPakkun::initMapObj()
{
	TMapObjBase::initMapObj();

	mTrembleEffect = new TTrembleModelEffect();
	mTrembleEffect->init(getMActor()->getModel());
	mTrembleEffect->tremble(100.0f, 1.0f, 1.0f, 12000);
}

// TODO: the five statics below are UNUSED in the map, so their values are not
// recoverable; TBiancoWatermill's own code never reads them. The numbers
// mirror the surviving TBiancoWatermillVertical pair.
f32 TBiancoWatermill::mRotAccel         = 0.15f;
f32 TBiancoWatermill::mEnemyRotAccel    = 0.15f;
f32 TBiancoWatermill::mRotSpeedDownRate = 0.005f;
f32 TBiancoWatermill::mRotSpeedMax      = 3.0f;
f32 TBiancoWatermill::mRotSpeedMin      = 0.3f;

void TBiancoWatermill::turnByEnemy(THitActor* enemy, const TBGCheckData* plane)
{
}

/**
 * @brief Spin the wheel from a water or enemy hit.
 *
 * @details UNUSED in the map (0xd8) and dead: touchWater returns 0 before it
 * could be reached, and turnByEnemy is empty. The body is not recoverable.
 */
void TBiancoWatermill::turn(const JGeometry::TVec3<f32>& point,
                            const TBGCheckData* plane, f32 accel)
{
}

u32 TBiancoWatermill::touchWater(THitActor* water) { return 0; }

void TBiancoWatermill::control()
{
	mRotation.z -= mRotSpeed;
	gpMSound->startSoundActorWithInfo(MSD_SE_OBJ_BI_BIGMILL, &mPosition,
	                                  nullptr, fabsf(mRotSpeed), 0, 0,
	                                  &mSoundHandle, 0, 4);
}

void TBiancoWatermill::initMapObj()
{
	TMapObjBase::initMapObj();

	if (strcmp(unkF4, "BiaWatermill01") == 0) {
		mBodyRadius = 1200.0f;
	} else if (strcmp(unkF4, "BiaWatermill00") == 0) {
		mBodyRadius = 1200.0f;
	}
}

TBiancoWatermill::TBiancoWatermill(const char* name)
    : TMapObjBase(name)
    , mRotSpeed(0.3f)
    , mSoundHandle(nullptr)
{
}

f32 TBiancoWatermillVertical::mRotAccel         = 0.15f;
f32 TBiancoWatermillVertical::mRotSpeedDownRate = 0.005f;
f32 TBiancoWatermillVertical::mRotSpeedMax      = 3.0f;
f32 TBiancoWatermillVertical::mBridgeRotRate    = 0.03f;

u32 TBiancoWatermillVertical::touchWater(THitActor* water)
{
	if (getWaterPlane(water) == nullptr) {
		mNeedGroundUpdate = true;
		return 0;
	}

	if (!waterHitPlane(water))
		return 0;

	const JGeometry::TVec3<f32>& pos   = getWaterPos(water);
	const JGeometry::TVec3<f32>& speed = getWaterSpeed(water);

	JGeometry::TVec3<f32> flow(speed.x, 0.0f, speed.z);
	if (flow.x != 0.0f || flow.z != 0.0f)
		MsVECNormalize(&flow, &flow);

	JGeometry::TVec3<f32> tangent;
	getVerticalVecToTargetXZ(pos.x, pos.z, &tangent);
	MsVECNormalize(&tangent, &tangent);

	// Hits near the rim turn the wheel harder than hits near the axle.
	f32 radius = mBodyRadius;
	f32 rate   = (radius - getDistanceXZ(pos)) / radius;

	if (flow.dot(tangent) > 0.0f) {
		if (mRotSpeed < mRotSpeedMax)
			mRotSpeed += mRotAccel * rate;
	} else {
		if (mRotSpeed > -mRotSpeedMax)
			mRotSpeed -= mRotAccel * rate;
	}

	return 1;
}

void TBiancoWatermillVertical::setGroundCollision()
{
	if (mNeedGroundUpdate || mColCount != 0) {
		MtxPtr mtx             = getModel()->getAnmMtx(0);
		TMapCollisionBase* col = getMapCollisionManager()->getUnk8();
		if (col != nullptr)
			col->moveMtx(mtx);
		mNeedGroundUpdate = false;
	}
}

void TBiancoWatermillVertical::control()
{
	if (mRotSpeed != mRotSpeedTarget) {
		if (mRotSpeed > mRotSpeedTarget) {
			mRotSpeed -= mRotSpeedDownRate;
			if (mRotSpeed < mRotSpeedTarget)
				mRotSpeed = mRotSpeedTarget;
		} else {
			mRotSpeed += mRotSpeedDownRate;
			if (mRotSpeed > mRotSpeedTarget)
				mRotSpeed = mRotSpeedTarget;
		}
	}

	mRotation.y += mRotSpeed;
	mRotation.y = MsWrap(mRotation.y, 0.0f, 360.0f);

	f32 bridgeSpeed = mRotSpeed * mBridgeRotRate;
	mBridge->mRotation.y += bridgeSpeed;
	mBridge->mRotation.y = MsWrap(mBridge->mRotation.y, 0.0f, 360.0f);

	gpMSound->startSoundActorWithInfo(MSD_SE_OBJ_BI_STEPMILL_WIND, &mPosition,
	                                  nullptr, fabsf(mRotSpeed), 0, 0,
	                                  &mSoundHandle, 0, 4);
	gpMSound->startSoundActorWithInfo(
	    MSD_SE_OBJ_BI_STEPMILL_MOVE, &mBridge->getPosition(), nullptr,
	    fabsf(bridgeSpeed), 0, 0, &mBridgeSoundHandle, 0, 4);
}

void TBiancoWatermillVertical::loadAfter()
{
	TMapObjBase::loadAfter();

	if (strcmp(getName(), "BiaWatermillVertical 0") == 0)
		mBridge = JDrama::TNameRefGen::search<TMapObjBase>("BiaTurnBridge 0");
	else
		mBridge = JDrama::TNameRefGen::search<TMapObjBase>("BiaTurnBridge 1");

	mBodyRadius = 1000.0f;
}

void TBiancoWatermillVertical::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);

	stream.read(&mRotSpeedTarget, 4);
	mRotSpeedTarget /= 1000.0f;
	mRotSpeed = mRotSpeedTarget;
}

TBiancoWatermillVertical::TBiancoWatermillVertical(const char* name)
    : TMapObjBase(name)
    , mRotSpeed(0.0f)
    , mRotSpeedTarget(0.0f)
    , mBridge(nullptr)
    , mNeedGroundUpdate(false)
    , mSoundHandle(nullptr)
    , mBridgeSoundHandle(nullptr)
{
}

f32 TBiancoMiniWindmill::mRotWaterAccel = 0.01f;
f32 TBiancoMiniWindmill::mFriction      = 0.01f;
f32 TBiancoMiniWindmill::mRotSpeedMax   = 10.0f;

/// How far in front of the windmill its sound/hit messenger sits.
static f32 sMessengerPosZ = 200.0f;
/// How far above it.
static f32 sMessengerPosY = 6400.0f;

u32 TBiancoMiniWindmill::touchWater(THitActor* water)
{
	const JGeometry::TVec3<f32>& pos = getWaterPos(water);
	if (pos.y < mPosition.y + sMessengerPosY - 300.0f)
		return 1;

	// Only water hitting the front of the blades turns them.
	const JGeometry::TVec3<f32>& speed = getWaterSpeed(water);
	MtxPtr mtx                         = getModel()->getAnmMtx(0);
	if (speed.z * mtx[2][2] + (speed.x * mtx[0][2] + speed.y * mtx[1][2])
	    > 0.0f)
		return 0;

	mRotSpeed += mRotWaterAccel;
	if (mRotSpeed > mRotSpeedMax) {
		mRotSpeed = mRotSpeedMax;
		JGeometry::TVec3<f32> point(mPosition.x,
		                            550.0f + mMessenger->mPosition.y,
		                            mPosition.z);
		mAppearSpeed = 0.0f;
		appearObjFromPoint(point);
	}

	return 1;
}

void TBiancoMiniWindmill::calc()
{
	Mtx spin;
	spin[0][0] = 1.0f;
	spin[0][1] = 0.0f;
	spin[0][2] = 0.0f;
	spin[0][3] = 0.0f;
	spin[1][0] = 0.0f;
	spin[1][1] = 1.0f;
	spin[1][2] = 0.0f;
	spin[1][3] = 0.0f;
	spin[2][0] = 0.0f;
	spin[2][1] = 0.0f;
	spin[2][2] = 1.0f;
	spin[2][3] = 0.0f;

	f32 sin    = JMASSin((s16)(182.04445f * mAngle));
	f32 cos    = JMASCos((s16)(182.04445f * mAngle));
	spin[0][0] = cos;
	spin[0][1] = -sin;
	spin[0][2] = 0.0f;
	spin[0][3] = 0.0f;
	spin[1][0] = sin;
	spin[1][1] = cos;
	spin[1][2] = 0.0f;
	spin[1][3] = 0.0f;
	spin[2][0] = 0.0f;
	spin[2][1] = 0.0f;
	spin[2][2] = 1.0f;
	spin[2][3] = 0.0f;

	MTXConcat(getModel()->getAnmMtx(0), spin, spin);

	MtxPtr blades = getModel()->getAnmMtx(1);
	spin[0][3]    = blades[0][3];
	spin[1][3]    = blades[1][3];
	spin[2][3]    = blades[2][3];
	MTXCopy(spin, getModel()->getAnmMtx(1));

	if (gpMSound->getDistPowFromCamera(mMessenger->mPosition) < 36000000.0f)
		gpMSound->startSoundActorWithInfo(
		    MSD_SE_OBJ_BI_COINMILL, &mMessenger->mPosition, nullptr,
		    fabsf(mRotSpeed), 0, 0, &mSoundHandle, 0, 4);
}

void TBiancoMiniWindmill::control()
{
	if (mRotSpeed > mIdleRotSpeed)
		mRotSpeed -= mFriction;
	else
		mRotSpeed = mIdleRotSpeed;

	mAngle += mRotSpeed;
	mAngle = MsWrap(mAngle, 0.0f, 360.0f);
}

void TBiancoMiniWindmill::initMapObj()
{
	TMapObjBase::initMapObj();

	mAppearSpeed = 0.0f;
	mMessenger   = new TMapObjMessenger("地形オブジェメッセンジャー");
	mMessenger->initHitActor(0, 1, 0, 0.0f, 0.0f, 300.0f, 500.0f);
	mMessenger->mPosition.x
	    = sMessengerPosZ * JMASSin((s16)(182.04445f * getRotation().y))
	    + mPosition.x;
	mMessenger->mPosition.y = mPosition.y + sMessengerPosY;
	mMessenger->mPosition.z
	    = sMessengerPosZ * JMASCos((s16)(182.04445f * mRotation.y))
	    + mPosition.z;
}

TBiancoMiniWindmill::TBiancoMiniWindmill(const char* name)
    : THideObjBase(name)
    , mAngle(360.0f * MsRandF())
    , mRotSpeed(0.0f)
    , mIdleRotSpeed(1.0f + MsRandF())
    , mMessenger(nullptr)
    , mSoundHandle(nullptr)
{
}

void TLeafBoat::touchActor(THitActor* other)
{
	if (other->isActorType(0x80000001))
		return;

	JGeometry::TVec3<f32> toOther(other->mPosition.x - mPosition.x, 0.0f,
	                              other->mPosition.z - mPosition.z);
	JGeometry::TVec3<f32> vel(getVelocity());
	if (toOther.dot(vel) < 0.0f)
		return;

	if (toOther.x != 0.0f || toOther.z != 0.0f)
		MsVECNormalize(&toOther, &toOther);

	JGeometry::TVec3<f32> vel2(getVelocity());
	f32 into = toOther.dot(vel2);
	if (other->checkActorType(ACTOR_TYPE_ENEMY)) {
		mVelocity.x -= (1.0f + mEnemyBounce) * (toOther.x * into);
		mVelocity.z -= (1.0f + mEnemyBounce) * (toOther.z * into);
	} else {
		mVelocity.x -= (1.0f + mActorBounce) * (toOther.x * into);
		mVelocity.z -= (1.0f + mActorBounce) * (toOther.z * into);
	}
}

void TLeafBoat::touchWall(JGeometry::TVec3<f32>* pos,
                          TBGWallCheckRecord* record)
{
	int num = record->mResultWallsNum;
	for (int i = 0; i < num; ++i) {
		const TBGCheckData* wall = record->mResultWalls[i];

		JGeometry::TVec3<f32> vel(mVelocity);
		if (vel.z * wall->mNormal.z
		        + (vel.x * wall->mNormal.x + vel.y * wall->mNormal.y)
		    < 0.0f) {
			f32 dist = pos->z * wall->mNormal.z
			    + (pos->x * wall->mNormal.x + pos->y * wall->mNormal.y)
			    + wall->mPlaneDistance;
			pos->x += (mBodyRadius - dist) * wall->mNormal.x;
			pos->z += (mBodyRadius - dist) * wall->mNormal.z;

			JGeometry::TVec3<f32> reflected(mVelocity);
			calcReflectingVelocity(wall, 1.0f, &reflected);
			mVelocity.x = reflected.x * mWallBounce;
			mVelocity.z = reflected.z * mWallBounce;
			return;
		}
	}
}

void TLeafBoat::bind()
{
	JGeometry::TVec3<f32> next(mPosition);

	JGeometry::TVec3<f32> velX(mVelocity);
	next.x += velX.x;
	JGeometry::TVec3<f32> velZ(mVelocity);
	next.z += velZ.z;

	const TBGCheckData* ground;
	if (gpMap->checkGroundIgnoreWaterSurface(next.x, mPosition.y - mYOffset,
	                                         next.z, &ground)
	    > mPosition.y - mYOffset - 50.0f) {
		JGeometry::TVec3<f32> vel(mVelocity);
		calcReflectingVelocity(ground, 1.0f, &vel);
		mVelocity.x *= -1.0f;
		mVelocity.z *= -1.0f;
		next = mPosition;
	}

	JGeometry::TVec3<f32> probe(next.x, next.y - mYOffset, next.z);
	TBGWallCheckRecord record(probe, mBodyRadius, 4,
	                          TBGWallCheckRecord::DONT_MOVE_XZ);
	if (gpMap->isTouchedWallsAndMoveXZ(&record))
		touchWall(&next, &record);

	// `a = b - c` is the shape that reaches the map's out-of-line
	// JGeometry::TVec3<f32>::sub: operator= is one inline level and the
	// difference nested in its argument two more.
	mLinearVelocity = next - mPosition;

	// Standing on the deck counts as an attack so the boat can carry Mario.
	f32 marioY = gpMarioPos->y;
	f32 deckY  = mPosition.y - mYOffset;
	f32 dx     = gpMarioPos->x - mPosition.x;
	f32 radius = mBodyRadius;
	f32 dz     = gpMarioPos->z - mPosition.z;
	if (marioY <= deckY && deckY - 100.0f < marioY
	    && dx * dx + dz * dz < radius * radius)
		SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
}

void TLeafBoat::control()
{
	TMapObjBase::control();

	if (marioHipAttack())
		mVelocity.y -= mHipAttackPower;

	if (marioIsOn()) {
		mVelocity.y -= mRiderWeight;
		if (SMS_GetMarioWaterGun()->mIsEmitWater > 0) {
			MtxPtr emitMtx = SMS_GetMarioWaterGun()->getEmitMtx(0);
			mVelocity.x -= emitMtx[0][0] * mWaterPushRate;
			mVelocity.z -= emitMtx[2][0] * mWaterPushRate;
		}
	}

	// The stream cubes carry a flow direction that drags the boat along.
	int cubeNo = gpCubeStream->getInCubeNo((Vec&)mPosition);
	if (cubeNo != -1) {
		TCubeStreamInfo* info
		    = (TCubeStreamInfo*)gpCubeStream->unk14->getChildren()[cubeNo];
		Mtx flow;
		MsMtxSetXYZRPH(flow, 0.0f, 0.0f, 0.0f, info->unk18.x, info->unk18.y,
		               info->unk18.z);
		f32 power = 0.0001f * info->unk40;
		mVelocity.x += flow[0][2] * power;
		mVelocity.z += flow[2][2] * power;
	}

	mPosition.y += mVelocity.y;
	mVelocity.y
	    += mSpringRate * (mInitialPosition.y - (mPosition.y - mYOffset));
	mVelocity.y *= mVerticalDecay;
	mVelocity.x *= mFriction;
	mVelocity.z *= mFriction;
}

void TLeafBoat::calc()
{
	if (mWaterPushRate != 0.0f) {
		if (mEffectTimer > 8) {
			if (fabsf(mVelocity.x) + fabsf(mVelocity.z) > 0.1f) {
				mEffectPos.set(getPosition().x, getPosition().y - mYOffset,
				               getPosition().z);
				JGeometry::TVec3<f32> scale(2.0f, 2.0f, 2.0f);
				emitAndBindScale(PARTICLE_MS_M_HAMON_B, 3, &mEffectPos, scale);
				emitAndBindScale(PARTICLE_MS_M_HAMON_A, 1, &mEffectPos, scale);
			}
			mEffectTimer = 0;
		} else {
			mEffectTimer++;
		}
	}
}

void TLeafBoat::initMapObj()
{
	TMapObjBase::initMapObj();

	mEnemyBounce = 1.0f;
	mActorBounce = 0.5f;
	mWallBounce  = 0.5f;
	mFriction    = 0.998f;
}

TLeafBoat::TLeafBoat(const char* name)
    : TMapObjBase(name)
    , mEnemyBounce(0.0f)
    , mActorBounce(0.0f)
    , mWallBounce(0.0f)
    , mWaterPushRate(0.03f)
    , mFriction(0.0f)
    , unk14C(1.2f)
    , mRiderWeight(0.03f)
    , mHipAttackPower(2.0f)
    , mSpringRate(0.005f)
    , mVerticalDecay(0.98f)
    , mEffectTimer(0)
{
	mEffectPos.zero();
}

f32 TLeafBoatRotten::mAlphaDownSpeed       = 0.5f;
f32 TLeafBoatRotten::mCollisionRemoveAlpha = 100.0f;
// TODO: both UNUSED in the map, so the values are not recoverable.
int TLeafBoatRotten::mBoatFlushTime     = 60;
int TLeafBoatRotten::mBoatFlushInterval = 10;
GXColorS10 TLeafBoatRotten::mRottenColor = { 100, 100, 180, 255 };

void TLeafBoatRotten::control()
{
	TLeafBoat::control();

	if (marioIsOn()) {
		if (isState(1)) {
			mStateTimer = mRottenTime;
			mState      = 2;
		}
	}

	switch (mState) {
	case 1:
		break;

	case 2: {
		// Fade from white to the rotten tint over the remaining lifetime.
		f32 rate = (f32)getStateTimer() / (f32)mRottenTime;
		mColor.r = (u8)(s32)((f32)(255 - mRottenColor.r) * rate
		                     + (f32)mRottenColor.r);
		mColor.g = (u8)(s32)((f32)(255 - mRottenColor.g) * rate
		                     + (f32)mRottenColor.g);
		mColor.b = (u8)(s32)((f32)(255 - mRottenColor.b) * rate
		                     + (f32)mRottenColor.b);
		if (!isStateTimerEngaged()) {
			mAlpha = 255.0f;
			mState = 3;
		}
		break;
	}

	case 3:
		mAlpha -= mAlphaDownSpeed;
		mColor.a = (u8)(s32)mAlpha;
		if (mAlpha < mCollisionRemoveAlpha) {
			if (getMapCollisionManager()->getUnk8()->isSetUp())
				removeMapCollision();
		}
		if (mAlpha <= 0.0f) {
			mScaling.set(1.0f, 1.0f, 1.0f);
			makeObjDefault();
			makeObjAppeared();
			mColor.r = 255;
			mColor.g = 255;
			mColor.b = 255;
			mColor.a = 255;
			mState   = 1;
		}
		break;
	}
}

void TLeafBoatRotten::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TMapObjBase::perform(cue, graphics);
}

void TLeafBoatRotten::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);

	stream.read(&mRottenTime, 4);
	mRottenTime *= 10;
	SMS_InitPacket_OneTevColor(getModel(), 0, GX_TEVREG0, &mColor);
}

TLeafBoatRotten::TLeafBoatRotten(const char* name)
    : TLeafBoat(name)
    , mRottenTime(0)
{
	mColor.r = 255;
	mColor.g = 255;
	mColor.b = 255;
	mColor.a = 255;
}

void TLampSeesaw::touchPlayer(THitActor* player)
{
	if (marioIsOn())
		mPartner->pushDown(-mPushSpeed);
}

void TLampSeesaw::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);

	f32 drop;
	stream.read(&drop, 4);
	mLowerLimitY = mInitialPosition.y - drop;
	stream.read(&mPushSpeed, 4);
	mPushSpeed *= 0.0001f;
}

TLampSeesaw::TLampSeesaw(const char* name)
    : TMapObjBase(name)
    , mPartner(nullptr)
    , mPushSpeed(0.01f)
{
}

void TLampSeesawMain::pushDown(f32 speed)
{
	mState = 2;
	mSpeed -= speed;
}

/**
 * @brief Advance the seesaw by one frame of mSpeed.
 *
 * @details UNUSED in the map (0x90); control() inlines it in both of its
 * cases.
 */
void TLampSeesawMain::move()
{
	f32 speed = mSpeed;
	f32 nextY = mPosition.y + speed;
	if (nextY < mLowerLimitY
	    || mPartner->mPosition.y - speed < mPartner->mLowerLimitY) {
		if (fabsf(speed) < mMinSpeed)
			mSpeed = 0.0f;
		else
			mSpeed *= -mReboundRate;
	} else {
		mPosition.y = nextY;
		mPartner->mPosition.y -= mSpeed;
	}

	mSpeed *= mSpeedDecay;
}

void TLampSeesawMain::touchPlayer(THitActor* player)
{
	if (marioIsOn())
		pushDown(mPushSpeed);
}

void TLampSeesawMain::control()
{
	TMapObjBase::control();

	switch (mState) {
	case 1:
		break;

	case 2:
		move();
		if (fabsf(mSpeed) < mMinSpeed)
			mState = 3;
		break;

	case 3:
		// Creep back to the rest height at the minimum speed.
		if (mSpeed < 0.0f)
			mSpeed = -mMinSpeed;
		else
			mSpeed = mMinSpeed;

		move();

		f32 minSpeed = mMinSpeed;
		if (fabsf(mSpeed) <= minSpeed
		    && fabsf(mInitialPosition.y - mPosition.y) < minSpeed) {
			mSpeed = 0.0f;
			mState = 1;
		}
		break;
	}
}

void TLampSeesawMain::loadAfter()
{
	// The partner is named after this object with the "（主）" suffix replaced;
	// the four bytes after the shared prefix are copied across verbatim.
	int prefix       = strlen("ランプシーソーＡ");
	const char* name = getName();
	char buffer[64];
	u8 c0 = name[prefix];
	u8 c1 = name[prefix + 1];
	u8 c2 = name[prefix + 2];
	u8 c3 = name[prefix + 3];
	snprintf(buffer, sizeof(buffer), "ランプシーソーＢ００", name);
	buffer[prefix]     = c0;
	buffer[prefix + 1] = c1;
	buffer[prefix + 2] = c2;
	buffer[prefix + 3] = c3;

	mPartner = JDrama::TNameRefGen::search<TLampSeesawMain>(buffer);
	mPartner->mPartner = this;
}

TLampSeesawMain::TLampSeesawMain(const char* name)
    : TLampSeesaw(name)
    , mSpeed(0.0f)
    , mSpeedDecay(0.998f)
    , mReboundRate(0.8f)
    , mMinSpeed(0.5f)
{
}

/**
 * @brief Stop the bell's swing animation.
 *
 * @details UNUSED in the map (0x30) and never called from this TU.
 */
void TBiancoBell::stopToRing()
{
	getMActor()->getFrameCtrl(ANM_TYPE_BCK)->setRate(0.0f);
}

/**
 * @brief Ring this bell with its own assigned animation.
 *
 * @details UNUSED in the map (0x12c); TBellWatermill::control inlines it once
 * per bell.
 */
void TBiancoBell::ring()
{
	if (getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame() == 0.0f
	    || getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame()
	            + getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getRate()
	        >= (f32)getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getEnd()
	            - 1.0f) {
		startAnim(mAnmIndex);
		f32 rate = SMSGetAnmFrameRate();
		getMActor()->getFrameCtrl(ANM_TYPE_BCK)->setRate(rate);
		if (mRingsAloud)
			gpMSound->startSoundActor(MSD_SE_OBJ_BI_BELL, &mPosition, 0,
			                          nullptr, 0, 4);
	}
}

/**
 * @brief Ring this bell on its own, with the shared single-bell animation.
 *
 * @details UNUSED in the map (0x120); touchPlayer and touchWater inline it.
 */
void TBiancoBell::ringSingle()
{
	if (getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame() == 0.0f
	    || getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame()
	            + getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getRate()
	        >= (f32)getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getEnd()
	            - 1.0f) {
		startAnim(4);
		f32 rate = SMSGetAnmFrameRate();
		getMActor()->getFrameCtrl(ANM_TYPE_BCK)->setRate(rate);
		gpMSound->startSoundActor(MSD_SE_OBJ_BI_BELL, &mPosition, 0, nullptr,
		                          0, 4);
	}
}

u32 TBiancoBell::touchWater(THitActor* water)
{
	ringSingle();
	return 1;
}

void TBiancoBell::touchPlayer(THitActor* player) { ringSingle(); }

void TBiancoBell::initMapObj()
{
	TMapObjBase::initMapObj();

	if (strcmp(getName(), "BiaBell 0") == 0) {
		mAnmIndex   = 1;
		mRingsAloud = false;
	} else if (strcmp(getName(), "BiaBell 1") == 0) {
		mAnmIndex   = 2;
		mRingsAloud = true;
	} else {
		mAnmIndex   = 3;
		mRingsAloud = false;
	}
}

TBiancoBell::TBiancoBell(const char* name)
    : TMapObjBase(name)
    , mAnmIndex(0)
    , mRingsAloud(false)
{
}

u32 TBellWatermill::touchWater(THitActor* water)
{
	mSprayedThisFrame = true;

	f32 speed = unk158;
	if (fabsf(speed) > mRotSpeedMax) {
		mRiseSpeed += mRiseAccel;
		unk158 += unk15C;
	} else {
		unk158 = speed + unk15C;
	}

	f32 max = unk164;
	if (unk158 > max)
		unk158 = max;

	return 1;
}

void TBellWatermill::control()
{
	TMapObjBase::control();

	if (unk158 != 0.0f || mRiseSpeed != 0.0f || mHeight != 0.0f) {
		f32 speed = unk158;
		f32 max   = mRotSpeedMax;
		f32 min   = -max;
		if (speed > max)
			speed = max;
		else if (speed < min)
			speed = min;

		unk154 += speed;
		unk154 = MsWrap(unk154, 0.0f, 360.0f);

		gpMSound->startSoundActorWithInfo(MSD_SE_OBJ_BI_UPDOWNMILL,
		                                  &mPosition, nullptr, fabsf(unk158),
		                                  0, 0, &mSoundHandle, 0, 4);

		if (fabsf(unk158) < fabsf(unk160))
			unk158 = 0.0f;

		if (!mSprayedThisFrame) {
			if (unk158 != 0.0f)
				unk158 = unk158 - unk160;
		}

		// Below the rest height the wheel bounces back up instead of falling.
		f32 height = mHeight;
		if (height < 0.0f) {
			f32 rise = mRiseSpeed;
			if (fabsf(rise) < mMinRiseSpeed) {
				mHeight    = 0.0f;
				mRiseSpeed = 0.0f;
			} else {
				mHeight = height - rise;
				mRiseSpeed *= -mReboundRate;
			}
		} else if (!mSprayedThisFrame && height != 0.0f) {
			mRiseSpeed -= mGravity;
		}

		mHeight += mRiseSpeed;

		if (mHeight > mHeightMax) {
			mBells[0]->ring();
			mBells[1]->ring();
			mBells[2]->ring();

			mHeight = mHeightMax;
			if (mShouldPayCoins) {
				for (int i = 0; i < 5; ++i) {
					TMapObjBase* coin
					    = gpItemManager->makeObjAppeared(0x2000000E);
					if (coin != nullptr) {
						coin->mPosition.x = mPosition.x;
						coin->mPosition.y = mPosition.y;
						coin->mPosition.z = mPosition.z;
						coin->mVelocity.set(10.0f, 100.0f * MsRandF() + 10.0f,
						                    10.0f * MsRandF() - 5.0f);
						coin->offLiveFlag(LIVE_FLAG_UNK10);
					}
				}
				mShouldPayCoins = false;
			}
		}

		mPosition.y       = mHeight + mInitialPosition.y + mYOffset;
		mSprayedThisFrame = false;
		mRotation.z       = unk154;

		// The Z matrix really is built once before the branch and again in
		// both arms; the ROM has all three copies.
		Mtx spin;
		setMtxRotZ(spin, mRotation.z);
		if (mRotation.y != 0.0f) {
			Mtx yaw;
			setMtxRotZ(spin, mRotation.z);
			setMtxRotY(yaw, mRotation.y);
			MTXConcat(yaw, spin, spin);
		} else {
			setMtxRotZ(spin, mRotation.z);
		}

		spin[0][3] = mPosition.x;
		spin[1][3] = mPosition.y;
		spin[2][3] = mPosition.z;
		spin[1][3] -= mYOffset;
		MTXCopy(spin, getModel()->getAnmMtx(0));
	}
}

void TBellWatermill::loadAfter()
{
	TMapObjTurn::loadAfter();

	unk150        = 2;
	unk15C        = -0.02f;
	unk160        = -0.008f;
	unk164        = 10.0f;
	unk18C        = 10.0f;
	mHeightMax    = 1000.0f;
	mRiseAccel    = 0.15f;
	mGravity      = 0.1f;
	mRotSpeedMax  = 4.0f;
	mReboundRate  = 0.5f;
	mMinRiseSpeed = 1.0f;

	mBells[0] = JDrama::TNameRefGen::search<TBiancoBell>("BiaBell 0");
	mBells[1] = JDrama::TNameRefGen::search<TBiancoBell>("BiaBell 1");
	mBells[2] = JDrama::TNameRefGen::search<TBiancoBell>("BiaBell 2");

	mShouldPayCoins = true;
}

TBellWatermill::TBellWatermill(const char* name)
    : TMapObjTurn(name)
    , mRotSpeedMax(0.0f)
    , mHeight(0.0f)
    , mHeightMax(0.0f)
    , mRiseSpeed(0.0f)
    , mMinRiseSpeed(0.0f)
    , mRiseAccel(0.0f)
    , mGravity(0.0f)
    , mReboundRate(0.0f)
    , unk18C(0.0f)
    , mSprayedThisFrame(false)
    , mShouldPayCoins(false)
    , mSoundHandle(nullptr)
{
}

void TWoodLog::control()
{
	TMapObjFloatOnSea::control();

	// Push a swimming Mario out to the nearer long side of the log.
	Mtx inverse;
	MTXInverse(getModel()->getAnmMtx(0), inverse);

	JGeometry::TVec3<f32> marioPos;
	marioPos.x = gpMarioPos->x;
	marioPos.y = gpMarioPos->y;
	marioPos.z = gpMarioPos->z;
	JGeometry::TVec3<f32> local;
	MTXMultVec(inverse, marioPos, local);

	if (SMS_IsMarioStatusTypeSwimming() && -232.0f < local.y
	    && -141.0f < local.x && local.x < 141.0f && -441.0f < local.z
	    && local.z < 441.0f) {
		if (local.x > 0.0f)
			local.x = 141.0f;
		else
			local.x = -141.0f;

		JGeometry::TVec3<f32> pushTo;
		MTXMultVec(getModel()->getAnmMtx(0), local, pushTo);
		SMS_MarioMoveRequest(pushTo);
	}
}

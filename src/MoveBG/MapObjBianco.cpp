#include <MoveBG/MapObjBianco.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JSupport/JSUInputStream.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <Map/Map.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapCollisionManager.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <MoveBG/MapObjMessenger.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/WaterGun.hpp>
#include <System/Particles.hpp>
#include <macros.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

static f32 sRadius = 2380.0f;
static f32 sSubZ   = 150.0f;
static f32 sSpeed  = 0.05f;
static f32 sAngleAdd;

void TBigWindmill::control()
{
	TMapObjBase::control();
	mRotation.z -= sSpeed;
	mRotation.z = MsWrap(mRotation.z, 0.0f, 360.0f);
	setRootMtxRotZ();
	SMSGetMSound()->startSoundActorWithInfo(MSD_SE_OBJ_BI_WINDMILLTOWER,
	                                        &mPosition, nullptr, fabsf(sSpeed),
	                                        0, 0, &unk148, 0, 4);
	f32 angle = mRotation.z + sAngleAdd;
	for (int i = 0; i < ARRAY_COUNT(unk138); ++i) {
		MtxPtr mtx = unk138[i]->getModel()->getAnmMtx(0);
		mtx[0][3]  = mPosition.x + sRadius * cosf(0.017453294f * angle);
		mtx[1][3]
		    = mPosition.y + sRadius * sinf(0.017453294f * angle) - mYOffset;
		mtx[2][3] = mPosition.z - sSubZ;
		unk138[i]->mPosition.set(mtx[0][3], mtx[1][3], mtx[2][3]);
		angle += 90.0f;
		if (angle > 360.0f)
			angle -= 360.0f;
	}
}

void TBigWindmill::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);
	for (int i = 0; i < ARRAY_COUNT(unk138); ++i) {
		unk138[i] = TMapObjBaseManager::newAndRegisterObj("bigWindmillBlock");
		unk138[i]->appear();
		unk138[i]->getModel()->calc();
	}
}

f32 TMapObjRootPakkun::mTremblePower = 15.0f;
f32 TMapObjRootPakkun::mTrembleAccel = 0.95f;
f32 TMapObjRootPakkun::mTrembleBrake = 0.98f;
int TMapObjRootPakkun::mTrembleTime  = 360;

void TMapObjRootPakkun::drawObject(JDrama::TGraphics* param_1)
{
	TLiveActor::drawObject(param_1);
	if (fabsf(SMS_GetMarioZ() - mPosition.z) < 10000.0f) {
		unk138->movement();
		if (!isStateTimerEngaged()) {
			unk138->tremble(mTremblePower, mTrembleAccel, mTrembleBrake,
			                mTrembleTime);
			startStateTimer(mTrembleTime);
		}
	}
}

void TMapObjRootPakkun::initMapObj()
{
	TMapObjBase::initMapObj();
	unk138 = new TTrembleModelEffect;
	unk138->init(getMActor()->getModel());
	unk138->tremble(100.0f, 1.0f, 1.0f, 12000);
}

void TBiancoWatermill::turnByEnemy(THitActor* param_1,
                                   const TBGCheckData* param_2)
{
}

void TBiancoWatermill::turn(const JGeometry::TVec3<f32>& param_1,
                            const TBGCheckData* param_2, f32 param_3)
{
}

u32 TBiancoWatermill::touchWater(THitActor* param_1) { return 0; }

void TBiancoWatermill::control()
{
	mRotation.z -= unk138;
	SMSGetMSound()->startSoundActorWithInfo(MSD_SE_OBJ_BI_BIGMILL, &mPosition,
	                                        nullptr, fabsf(unk138), 0, 0,
	                                        &unk13C, 0, 4);
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

TBiancoWatermill::TBiancoWatermill(const char* param_1)
    : TMapObjBase(param_1)
    , unk138(0.3f)
    , unk13C(nullptr)
{
}

f32 TBiancoWatermillVertical::mRotAccel         = 0.15f;
f32 TBiancoWatermillVertical::mRotSpeedDownRate = 0.005f;
f32 TBiancoWatermillVertical::mRotSpeedMax      = 3.0f;
f32 TBiancoWatermillVertical::mBridgeRotRate    = 0.03f;

u32 TBiancoWatermillVertical::touchWater(THitActor* param_1)
{
	if (getWaterPlane(param_1) == nullptr) {
		unk144 = 1;
		return 0;
	}
	if (!waterHitPlane(param_1))
		return 0;
	const JGeometry::TVec3<f32>& waterPos   = getWaterPos(param_1);
	const JGeometry::TVec3<f32>& waterSpeed = getWaterSpeed(param_1);
	JGeometry::TVec3<f32> speedXZ(waterSpeed.x, 0.0f, waterSpeed.z);
	if (speedXZ.x != 0.0f || speedXZ.z != 0.0f)
		MsVECNormalize(&speedXZ, &speedXZ);
	JGeometry::TVec3<f32> targetDir;
	getVerticalVecToTargetXZ(waterPos.x, waterPos.z, &targetDir);
	MsVECNormalize(&targetDir, &targetDir);
	f32 radius = mBodyRadius;
	f32 factor = (radius - getDistanceXZ(waterPos)) / radius;
	if (speedXZ.dot(targetDir) > 0.0f) {
		if (unk138 < mRotSpeedMax)
			unk138 += mRotAccel * factor;
	} else {
		if (unk138 > -mRotSpeedMax)
			unk138 -= mRotAccel * factor;
	}
	return 1;
}

void TBiancoWatermillVertical::setGroundCollision()
{
	if (unk144 || mColCount) {
		MtxPtr mtx = getModel()->getAnmMtx(0);
		if (mMapCollisionManager->getActiveCollision())
			mMapCollisionManager->getActiveCollision()->moveMtx(mtx);
		unk144 = 0;
	}
}

void TBiancoWatermillVertical::control()
{
	if (unk138 != unk13C) {
		if (unk138 > unk13C) {
			unk138 -= mRotSpeedDownRate;
			if (unk138 < unk13C)
				unk138 = unk13C;
		} else {
			unk138 += mRotSpeedDownRate;
			if (unk138 > unk13C)
				unk138 = unk13C;
		}
	}
	mRotation.y += unk138;
	mRotation.y     = MsWrap(mRotation.y, 0.0f, 360.0f);
	f32 bridgeSpeed = unk138 * mBridgeRotRate;
	unk140->mRotation.y += bridgeSpeed;
	unk140->mRotation.y = MsWrap(unk140->mRotation.y, 0.0f, 360.0f);
	SMSGetMSound()->startSoundActorWithInfo(MSD_SE_OBJ_BI_STEPMILL_WIND,
	                                        &mPosition, nullptr, fabsf(unk138),
	                                        0, 0, &unk148, 0, 4);
	SMSGetMSound()->startSoundActorWithInfo(
	    MSD_SE_OBJ_BI_STEPMILL_MOVE, &unk140->mPosition, nullptr,
	    fabsf(bridgeSpeed), 0, 0, &unk14C, 0, 4);
}

void TBiancoWatermillVertical::loadAfter()
{
	TMapObjBase::loadAfter();
	if (strcmp(mName, "BiaWatermillVertical 0") == 0)
		unk140 = static_cast<TMapObjBase*>(
		    JDrama::TNameRefGen::search("BiaTurnBridge 0"));
	else
		unk140 = static_cast<TMapObjBase*>(
		    JDrama::TNameRefGen::search("BiaTurnBridge 1"));
	mBodyRadius = 1000.0f;
}

void TBiancoWatermillVertical::load(JSUMemoryInputStream& param_1)
{
	TMapObjBase::load(param_1);
	param_1 >> unk13C;
	unk13C /= 1000.0f;
	unk138 = unk13C;
}

TBiancoWatermillVertical::TBiancoWatermillVertical(const char* param_1)
    : TMapObjBase(param_1)
    , unk138(0.0f)
    , unk13C(0.0f)
    , unk140(nullptr)
    , unk144(0)
    , unk148(nullptr)
    , unk14C(nullptr)
{
}

f32 TBiancoMiniWindmill::mRotWaterAccel = 0.01f;
f32 TBiancoMiniWindmill::mFriction      = 0.01f;
f32 TBiancoMiniWindmill::mRotSpeedMax   = 10.0f;

static f32 sMessengerPosZ = 200.0f;
static f32 sMessengerPosY = 6400.0f;

u32 TBiancoMiniWindmill::touchWater(THitActor* param_1)
{
	const JGeometry::TVec3<f32>& waterPos = getWaterPos(param_1);
	if (waterPos.y < mPosition.y + sMessengerPosY - 300.0f)
		return 1;
	const JGeometry::TVec3<f32>& waterSpeed = getWaterSpeed(param_1);
	MtxPtr mtx                              = getModel()->getAnmMtx(0);
	if (waterSpeed.x * mtx[0][2] + waterSpeed.y * mtx[1][2]
	        + waterSpeed.z * mtx[2][2]
	    > 0.0f)
		return 0;
	unk154 += mRotWaterAccel;
	if (unk154 > mRotSpeedMax) {
		unk154 = mRotSpeedMax;
		JGeometry::TVec3<f32> point(mPosition.x, 550.0f + unk15C->mPosition.y,
		                            mPosition.z);
		mAppearSpeed = 0.0f;
		appearObjFromPoint(point);
	}
	return 1;
}

void TBiancoMiniWindmill::calc()
{
	JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > mtx;
	MtxPtr mtxP = mtx;
	mtx.identity();
	MsMtxSetRotZ(mtxP, unk150);
	MTXConcat(getModel()->getAnmMtx(0), mtxP, mtxP);
	MtxPtr srcMtx = getModel()->getAnmMtx(1);
	for (int i = 0; i < 3; ++i)
		mtx.ref(i, 3) = srcMtx[i][3];
	MTXCopy(mtxP, getModel()->getAnmMtx(1));
	SMSGetMSound()->startSoundActorWithInfo(MSD_SE_OBJ_BI_COINMILL, &mPosition,
	                                        nullptr, fabsf(unk154), 0, 0,
	                                        &unk160, 0, 4);
}

void TBiancoMiniWindmill::control()
{
	if (unk154 > unk158)
		unk154 -= mFriction;
	else
		unk154 = unk158;
	unk150 += unk154;
	unk150 = MsWrap(unk150, 0.0f, 360.0f);
}

void TBiancoMiniWindmill::initMapObj()
{
	TMapObjBase::initMapObj();
	mAppearSpeed = 0.0f;
	unk15C       = new TMapObjMessenger;
	unk15C->initHitActor(0, 1, 0, 0.0f, 0.0f, 300.0f, 500.0f);
	unk15C->mPosition.x = mPosition.x + sMessengerPosZ * MsSin(mRotation.y);
	unk15C->mPosition.y = mPosition.y + sMessengerPosY;
	unk15C->mPosition.z = mPosition.z + sMessengerPosZ * MsCos(mRotation.y);
}

TBiancoMiniWindmill::TBiancoMiniWindmill(const char* param_1)
    : THideObjBase(param_1)
    , unk150(360.0f * MsRandF())
    , unk154(0.0f)
    , unk158(1.0f + MsRandF())
    , unk15C(nullptr)
    , unk160(nullptr)
{
}

void TLeafBoat::touchActor(THitActor* param_1)
{
	if (param_1->isActorType(0x80000001))
		return;
	JGeometry::TVec3<f32> direction(param_1->mPosition.x - mPosition.x, 0.0f,
	                                param_1->mPosition.z - mPosition.z);
	if (direction.dot(mVelocity) < 0.0f)
		return;
	if (direction.x != 0.0f || direction.z != 0.0f)
		MsVECNormalize(&direction, &direction);
	f32 dot = direction.dot(mVelocity);
	if (param_1->isHitCategory(HIT_CATEGORY_ENEMY)) {
		mVelocity.x -= (1.0f + unk138) * (direction.x * dot);
		mVelocity.z -= (1.0f + unk138) * (direction.z * dot);
	} else {
		mVelocity.x -= (1.0f + unk13C) * (direction.x * dot);
		mVelocity.z -= (1.0f + unk13C) * (direction.z * dot);
	}
}

void TLeafBoat::touchWall(JGeometry::TVec3<f32>* param_1,
                          TBGWallCheckRecord* param_2)
{
	int wallsNum = param_2->mResultWallsNum;
	for (int i = 0; i < wallsNum; ++i) {
		TBGCheckData* data = param_2->mResultWalls[i];
		if (mVelocity.dot(data->getNormal()) < 0.0f) {
			f32 dist
			    = param_1->dot(data->getNormal()) + data->getPlaneDistance();
			param_1->x += (mBodyRadius - dist) * data->getNormal().x;
			param_1->z += (mBodyRadius - dist) * data->getNormal().z;
			JGeometry::TVec3<f32> reflect = mVelocity;
			calcReflectingVelocity(data, 1.0f, &reflect);
			mVelocity.x = reflect.x * unk140;
			mVelocity.z = reflect.z * unk140;
			break;
		}
	}
}

void TLeafBoat::bind()
{
	JGeometry::TVec3<f32> position = mPosition;
	position.x += mVelocity.x;
	position.z += mVelocity.z;
	const TBGCheckData* ground;
	f32 height = gpMap->checkGroundIgnoreWaterSurface(
	    position.x, mPosition.y - mYOffset, position.z, &ground);
	if (height > mPosition.y - mYOffset - 50.0f) {
		JGeometry::TVec3<f32> reflection = mVelocity;
		calcReflectingVelocity(ground, 1.0f, &reflection);
		mVelocity.x *= -1.0f;
		mVelocity.z *= -1.0f;
		position = mPosition;
	}
	JGeometry::TVec3<f32> center;
	center.x = position.x;
	center.y = position.y - mYOffset;
	center.z = position.z;
	TBGWallCheckRecord walls(center, mBodyRadius, 4,
	                         TBGWallCheckRecord::DONT_MOVE_XZ);
	if (gpMap->isTouchedWallsAndMoveXZ(&walls))
		touchWall(&position, &walls);
	mPositionDelta = position - mPosition;
	f32 dx         = SMS_GetMarioPos().x - mPosition.x;
	f32 dz         = SMS_GetMarioPos().z - mPosition.z;
	if (SMS_GetMarioPos().y <= mPosition.y - mYOffset
	    && mPosition.y - mYOffset - 100.0f < SMS_GetMarioPos().y
	    && dx * dx + dz * dz < mBodyRadius * mBodyRadius)
		SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
}

void TLeafBoat::control()
{
	TMapObjBase::control();
	if (marioHipAttack())
		mVelocity.y -= unk154;
	if (marioIsOn()) {
		mVelocity.y -= unk150;
		if (SMS_GetMarioWaterGun()->mIsEmitWater > 0) {
			MtxPtr mtx = SMS_GetMarioWaterGun()->getEmitMtx(0);
			mVelocity.x -= mtx[0][0] * unk144;
			mVelocity.z -= mtx[2][0] * unk144;
		}
	}
	int cubeNo = gpCubeStream->getInCubeNo(mPosition);
	if (cubeNo != -1) {
		TCubeStreamInfo& cube
		    = (TCubeStreamInfo&)(*gpCubeStream->unk14)[cubeNo];
		Mtx mtx;
		MsMtxSetXYZRPH(mtx, 0.0f, 0.0f, 0.0f, cube.unk18.x, cube.unk18.y,
		               cube.unk18.z);
		f32 speed = 0.0001f * cube.unk40;
		mVelocity.x += mtx[0][2] * speed;
		mVelocity.z += mtx[2][2] * speed;
	}
	mPosition.y += mVelocity.y;
	mVelocity.y += unk158 * (mInitialPosition.y - (mPosition.y - mYOffset));
	mVelocity.y *= unk15C;
	mVelocity.x *= unk148;
	mVelocity.z *= unk148;
}

void TLeafBoat::calc()
{
	if (unk144 != 0.0f) {
		if (unk160 > 8) {
			if (fabsf(mVelocity.x) + fabsf(mVelocity.z) > 0.1f) {
				unk164.set(mPosition.x, mPosition.y - mYOffset, mPosition.z);
				JGeometry::TVec3<f32> scale(2.0f, 2.0f, 2.0f);
				emitAndBindScale(PARTICLE_MS_M_HAMON_B, 3, &unk164, scale);
				emitAndBindScale(PARTICLE_MS_M_HAMON_A, 1, &unk164, scale);
			}
			unk160 = 0;
		} else {
			++unk160;
		}
	}
}

void TLeafBoat::initMapObj()
{
	TMapObjBase::initMapObj();
	unk138 = 1.0f;
	unk13C = 0.5f;
	unk140 = 0.5f;
	unk148 = 0.998f;
}

TLeafBoat::TLeafBoat(const char* param_1)
    : TMapObjBase(param_1)
    , unk138(0.0f)
    , unk13C(0.0f)
    , unk140(0.0f)
    , unk144(0.03f)
    , unk148(0.0f)
    , unk14C(1.2f)
    , unk150(0.03f)
    , unk154(2.0f)
    , unk158(0.005f)
    , unk15C(0.98f)
    , unk160(0)
{
	unk164.zero();
}

f32 TLeafBoatRotten::mAlphaDownSpeed       = 0.5f;
f32 TLeafBoatRotten::mCollisionRemoveAlpha = 100.0f;
GXColorS10 TLeafBoatRotten::mRottenColor   = { 100, 100, 180, 255 };

void TLeafBoatRotten::control()
{
	TLeafBoat::control();
	if (marioIsOn() && isState(STATE_NORMAL)) {
		startStateTimer(unk170);
		mState = STATE_UNK2;
	}
	switch (mState) {
	case STATE_NORMAL:
		break;
	case STATE_UNK2: {
		f32 ratio = (f32)mStateTimer / unk170;
		unk178.r  = (u8)((255 - mRottenColor.r) * ratio + mRottenColor.r);
		unk178.g  = (u8)((255 - mRottenColor.g) * ratio + mRottenColor.g);
		unk178.b  = (u8)((255 - mRottenColor.b) * ratio + mRottenColor.b);
		if (!isStateTimerEngaged()) {
			unk174 = 255.0f;
			mState = STATE_UNK3;
		}
		break;
	}
	case STATE_UNK3:
		unk174 -= mAlphaDownSpeed;
		unk178.a = (u8)unk174;
		if (unk174 < mCollisionRemoveAlpha
		    && mMapCollisionManager->getActiveCollision()->isSetUp())
			removeMapCollision();
		if (unk174 <= 0.0f) {
			mScaling.set(1.0f, 1.0f, 1.0f);
			makeObjDefault();
			makeObjAppeared();
			unk178.r = 255;
			unk178.g = 255;
			unk178.b = 255;
			unk178.a = 255;
			mState   = STATE_NORMAL;
		}
		break;
	}
}

void TLeafBoatRotten::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TMapObjBase::perform(param_1, param_2);
}

void TLeafBoatRotten::load(JSUMemoryInputStream& param_1)
{
	TMapObjBase::load(param_1);
	param_1 >> unk170;
	unk170 *= 10;
	SMS_InitPacket_OneTevColor(getModel(), 0, GX_TEVREG0, &unk178);
}

TLeafBoatRotten::TLeafBoatRotten(const char* param_1)
    : TLeafBoat(param_1)
    , unk170(0)
{
	unk178.r = 0xFF;
	unk178.g = 0xFF;
	unk178.b = 0xFF;
	unk178.a = 0xFF;
}

void TLampSeesaw::touchPlayer(THitActor* param_1)
{
	if (marioIsOn()) {
		unk138->pushDown(-unk140);
	}
}

void TLampSeesaw::load(JSUMemoryInputStream& param_1)
{
	f32 height;
	TMapObjBase::load(param_1);
	param_1 >> height;
	unk13C = mInitialPosition.y - height;
	param_1 >> unk140;
	unk140 *= 0.0001f;
}

TLampSeesaw::TLampSeesaw(const char* param_1)
    : TMapObjBase(param_1)
    , unk138(nullptr)
    , unk140(0.01f)
{
}

void TLampSeesawMain::pushDown(f32 param_1)
{
	mState = STATE_UNK2;
	unk144 -= param_1;
}

void TLampSeesawMain::move()
{
	if (mPosition.y + unk144 < unk13C
	    || unk138->mPosition.y - unk144 < unk138->unk13C) {
		if (fabsf(unk144) < unk150)
			unk144 = 0.0f;
		else
			unk144 *= -unk14C;
	} else {
		mPosition.y += unk144;
		unk138->mPosition.y -= unk144;
	}
	unk144 *= unk148;
}

void TLampSeesawMain::touchPlayer(THitActor* param_1)
{
	if (marioIsOn()) {
		pushDown(unk140);
	}
}

void TLampSeesawMain::control()
{
	TMapObjBase::control();

	switch (mState) {
	case STATE_NORMAL:
		break;

	case STATE_UNK2:
		move();
		if (fabsf(unk144) < unk150)
			mState = STATE_UNK3;
		break;

	case STATE_UNK3:
		if (unk144 < 0.0f)
			unk144 = -unk150;
		else
			unk144 = unk150;
		move();
		if (fabsf(unk144) <= unk150) {
			if (fabsf(mInitialPosition.y - mPosition.y) < unk150) {
				unk144 = 0.0f;
				mState = STATE_NORMAL;
			}
		}
		break;
	}
}

void TLampSeesawMain::loadAfter()
{
	size_t len = strlen("ランプシーソーＡ");
	char buffer[4];
	buffer[0] = mName[len];
	buffer[1] = mName[len + 1];
	buffer[2] = mName[len + 2];
	buffer[3] = mName[len + 3];

	char buffer2[0x40];
	snprintf(buffer2, 0x40, "ランプシーソーＢ００");
	buffer2[len]     = buffer[0];
	buffer2[len + 1] = buffer[1];
	buffer2[len + 2] = buffer[2];
	buffer2[len + 3] = buffer[3];

	unk138 = static_cast<TLampSeesaw*>(JDrama::TNameRefGen::search(buffer2));
	unk138->unk138 = this;
}

TLampSeesawMain::TLampSeesawMain(const char* param_1)
    : TLampSeesaw(param_1)
    , unk144(0.0f)
    , unk148(0.998f)
    , unk14C(0.8f)
    , unk150(0.5f)
{
}

void TBiancoBell::stopToRing() { }

void TBiancoBell::ring()
{
	if (getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame() == 0.0f
	    || getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame()
	               + getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getRate()
	           >= getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getEnd() - 1.0f) {
		startAnim(unk138);
		getMActor()->getFrameCtrl(ANM_TYPE_BCK)->setRate(SMSGetAnmFrameRate());
		if (unk13A)
			SMSGetMSound()->startSoundActor(MSD_SE_OBJ_BI_BELL, &mPosition, 0,
			                                nullptr, 0, 4);
	}
}

void TBiancoBell::ringSingle()
{
	if (getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame() == 0.0f
	    || getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame()
	               + getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getRate()
	           >= getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getEnd() - 1.0f) {
		startAnim(4);
		getMActor()->getFrameCtrl(ANM_TYPE_BCK)->setRate(SMSGetAnmFrameRate());
		SMSGetMSound()->startSoundActor(MSD_SE_OBJ_BI_BELL, &mPosition, 0,
		                                nullptr, 0, 4);
	}
}

u32 TBiancoBell::touchWater(THitActor* param_1)
{
	ringSingle();
	return 1;
}

void TBiancoBell::touchPlayer(THitActor* param_1) { ringSingle(); }

void TBiancoBell::initMapObj()
{
	TMapObjBase::initMapObj();
	if (strcmp(mName, "BiaBell 0") == 0) {
		unk138 = 1;
		unk13A = 0;
	} else if (strcmp(mName, "BiaBell 1") == 0) {
		unk138 = 2;
		unk13A = 1;
	} else {
		unk138 = 3;
		unk13A = 0;
	}
}

TBiancoBell::TBiancoBell(const char* param_1)
    : TMapObjBase(param_1)
    , unk138(0)
    , unk13A(0)
{
}

u32 TBellWatermill::touchWater(THitActor* param_1)
{
	unk190 = 1;
	if (fabsf(unk158) > unk16C) {
		unk178 += unk180;
		unk158 += unk15C;
	} else {
		unk158 += unk15C;
	}
	if (unk158 > unk164) {
		unk158 = unk164;
	}
	return 1;
}

void TBellWatermill::control()
{
	TMapObjBase::control();
	if (unk158 == 0.0f && unk178 == 0.0f && unk170 == 0.0f)
		return;

	unk154 += MsClamp(unk158, -unk16C, unk16C);
	unk154 = MsWrap(unk154, 0.0f, 360.0f);
	SMSGetMSound()->startSoundActorWithInfo(MSD_SE_OBJ_BI_UPDOWNMILL,
	                                        &mPosition, nullptr, fabsf(unk158),
	                                        0, 0, &unk1A4, 0, 4);
	if (fabsf(unk158) < fabsf(unk160))
		unk158 = 0.0f;
	if (!unk190 && unk158 != 0.0f)
		unk158 -= unk160;
	if (unk170 < 0.0f) {
		if (fabsf(unk178) < unk17C) {
			unk170 = 0.0f;
			unk178 = 0.0f;
		} else {
			unk170 -= unk178;
			unk178 *= -unk188;
		}
	} else if (!unk190 && unk170 != 0.0f) {
		unk178 -= unk184;
	}
	unk170 += unk178;
	if (unk170 > unk174) {
		unk194->ring();
		unk198->ring();
		unk19C->ring();
		unk170 = unk174;
		if (unk1A0) {
			for (int i = 0; i < 5; ++i) {
				TMapObjBase* item = gpItemManager->makeObjAppeared(0x2000000E);
				if (item) {
					item->mPosition.set(mPosition);
					item->setVelocityAndFlag10(10.0f,
					                           10.0f + 100.0f * MsRandF(),
					                           10.0f * MsRandF() - 5.0f);
				}
			}
			unk1A0 = 0;
		}
	}
	mPosition.y = unk170 + mInitialPosition.y + mYOffset;
	unk190      = 0;
	mRotation.z = unk154;
	Mtx mtx;
	MsMtxSetRotZ(mtx, mRotation.z);
	if (mRotation.y != 0.0f) {
		MsMtxSetRotZ(mtx, mRotation.z);
		Mtx yRot;
		MsMtxSetRotY(yRot, mRotation.y);
		MTXConcat(yRot, mtx, mtx);
	} else {
		MsMtxSetRotZ(mtx, mRotation.z);
	}

	MtxPtr ptr = mtx;
	ptr[0][3]  = mPosition.x;
	ptr[1][3]  = mPosition.y;
	ptr[2][3]  = mPosition.z;
	ptr[1][3] -= mYOffset;
	getModel()->setAnmMtx(0, ptr);
}

void TBellWatermill::loadAfter()
{
	TMapObjTurn::loadAfter();
	unk150 = 2;
	unk15C = -0.02f;
	unk160 = -0.008f;
	unk164 = 10.0f;
	unk18C = 10.0f;
	unk174 = 1000.0f;
	unk180 = 0.15f;
	unk184 = 0.1f;
	unk16C = 4.0f;
	unk188 = 0.5f;
	unk17C = 1.0f;
	unk194
	    = static_cast<TBiancoBell*>(JDrama::TNameRefGen::search("BiaBell 0"));
	unk198
	    = static_cast<TBiancoBell*>(JDrama::TNameRefGen::search("BiaBell 1"));
	unk19C
	    = static_cast<TBiancoBell*>(JDrama::TNameRefGen::search("BiaBell 2"));
	unk1A0 = 1;
}

TBellWatermill::TBellWatermill(const char* param_1)
    : TMapObjTurn(param_1)
    , unk16C(0.0f)
    , unk170(0.0f)
    , unk174(0.0f)
    , unk178(0.0f)
    , unk17C(0.0f)
    , unk180(0.0f)
    , unk184(0.0f)
    , unk188(0.0f)
    , unk18C(0.0f)
    , unk190(0)
    , unk1A0(0)
    , unk1A4(nullptr)
{
}

void TWoodLog::control()
{
	TMapObjFloatOnSea::control();
	Mtx inv;
	MTXInverse(getModel()->getAnmMtx(0), inv);
	JGeometry::TVec3<f32> marioPos;
	marioPos.set(SMS_GetMarioPos());
	JGeometry::TVec3<f32> local;
	MTXMultVec(inv, &marioPos, &local);
	if (SMS_IsMarioStatusTypeSwimming() && -232.0f < local.y
	    && -141.0f < local.x && local.x < 141.0f && -441.0f < local.z
	    && local.z < 441.0f) {
		if (local.x > 0.0f)
			local.x = 141.0f;
		else
			local.x = -141.0f;
		JGeometry::TVec3<f32> world;
		MTXMultVec(getModel()->getAnmMtx(0), &local, &world);
		SMS_MarioMoveRequest(world);
	}
}

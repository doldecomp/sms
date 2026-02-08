#include <Player/ModelWaterManager.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/SplashManager.hpp>
#include <Strategic/LiveActor.hpp>
#include <Camera/Camera.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/PollutionManager.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/MapMirror.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <System/StageUtil.hpp>
#include <System/TimeRec.hpp>
#include <MSound/MSound.hpp>
#include <MarioUtil/DLUtil.hpp>
#include <MarioUtil/ScreenUtil.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/EffectUtil.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JMath.hpp>
#include <stdlib.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

TModelWaterManager* gpModelWaterManager;
TWaterHitActor TModelWaterManager::mStaticHitActor;
GXColor gModelWaterManagerWaterColor[4] = {
	{ 0x3C, 0x46, 0x78, 0x14 },
	{ 0xFE, 0xA8, 0x02, 0x6E },
	{ 0x9B, 0x01, 0xFD, 0x6E },
	{ 0xFD, 0x62, 0xA7, 0x6E },
};

TWaterEmitInfo::TWaterEmitInfo(const char* name)
    : TParams(name)
    , PARAM_INIT(mNum, 1)
    , PARAM_INIT(mAlive, 0xff)
    , PARAM_INIT(mAttack, 1)
    , PARAM_INIT(mDir, JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f))
    , PARAM_INIT(mPos, JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f))
    , PARAM_INIT(mV, JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f))
    , PARAM_INIT(mDirTremble, 0.0f)
    , PARAM_INIT(mPow, 0.0f)
    , PARAM_INIT(mPowTremble, 0.0f)
    , PARAM_INIT(mSize, 0.0f)
    , PARAM_INIT(mSizeTremble, 0.0f)
    , PARAM_INIT(mHitRadius, 0.0f)
    , PARAM_INIT(mHitHeight, 0.0f)
    , PARAM_INIT(mFlag, 0)
    , PARAM_INIT(mType, 0)
    , PARAM_INIT(__padding, 0)
{
	TParams::load(mPrmPath);
}

const char* prmNames[] = {
	"/Mario/WTP00_Normal.prm",     "/Mario/WTP01_Wide.prm",
	"/Mario/WTP02_Bomb.prm",       "/Mario/WTP03_TrgRocket.prm",
	"/Mario/WTP04_HHover.prm",     "/Mario/WTP05_Sinper.prm",
	"/Mario/WTP06_Yoshi.prm",      "/Mario/WTP07_Diving.prm",
	"/Mario/WTP08_Bubble.prm",     "/Mario/WTP09_Parasol.prm",
	"/Mario/WTP10_Back.prm",       "/Mario/WTP11_Sprinkler.prm",
	"/Mario/WTP12_YoshiMouth.prm", "/Mario/WTP13_Turbo.prm",
	"/Mario/WTP14_Rocket.prm",     "/Mario/WTP15_Hover.prm",
	"/Mario/WTP16_SpRocket.prm",
};

TWaterParticleType::TWaterParticleType(const char* path)
    : TParams(path)
    , PARAM_INIT(mExtension, 0.5f)
    , PARAM_INIT(mMagnify, 0.3f)
    , PARAM_INIT(mCleanSize, 12.0f)
    , PARAM_INIT(mGravity, -0.4f)
    , PARAM_INIT(mAlive, 255.0f)
{
	TParams::load(mPrmPath);
}

void TModelWaterManager::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);
	unk10          = SLOT_NUM;
	mParticleCount = 0;

	for (int i = 0; i < SLOT_NUM; ++i) {
		mParticleLifetimeSOA[i] = 0;
		mParticleFlagSOA[i]     = 0;
		mParticleAttackSOA[i]   = 0;
		mParticlePositionSOA[i].set(0.0f, 0.0f, 0.0f);
		mParticleVelocitySOA[i].set(0.0f, 0.0f, 0.0f);
		mParticleTypeSOA[i] = 0;
		unk2914[i]          = nullptr;
		MTXIdentity(unk2D14[i]);
		unk2514[i] = nullptr;
	}

	if (!(mStaticHitActor.mCollisions ? true : false)) {
		mStaticHitActor.initHitActor(0x1000001, 1, 0x5c000000, 50.0f, 80.0f,
		                             50.0f, 80.0f);
		mStaticHitActor.unk68 = -1;
	}

	unk5D14 = 7.0f;
	unk5D18 = 4.0f;
	unk5D1C = 0.07f;
	unk5D20 = (GXColor) { 0xbc, 0xcc, 0xdc, 0xff };
	unk5D24 = (GXColor) { 0x8e, 0x8e, 0x9e, 0xff };
	unk5D28 = 6000.0f;
	unk5D2C = 6000.0f;

	unk5D30 = new TDLTexQuad;
	unk5D30->createBuffer(256);

	unk5D34 = 0;

	ResTIMG* waterref  = (ResTIMG*)JKRGetResource("/mario/timg/waterref.bti");
	unk5D38            = new JUTTexture(waterref);
	ResTIMG* waterMask = (ResTIMG*)JKRGetResource("/mario/timg/waterMask.bti");
	unk5D3C            = new JUTTexture(waterMask);
	ResTIMG* waterSpec = (ResTIMG*)JKRGetResource("/mario/timg/waterSpec.bti");
	unk5D40            = new JUTTexture(waterSpec);
	ResTIMG* waterJumping
	    = (ResTIMG*)JKRGetResource("/mario/timg/waterJumping.bti");
	unk5D44 = new JUTTexture(waterJumping);

	unk5D48 = J3DModelLoaderDataBase::load(
	    JKRGetResource("/mario/bmd/water_shadow_yuka.bmd"), 0x10210000);
	unk5D4C = J3DModelLoaderDataBase::load(
	    JKRGetResource("/mario/bmd/water_shadow_kabe.bmd"), 0x10210000);
	unk5D50 = J3DModelLoaderDataBase::load(
	    JKRGetResource("/mario/bmd/watermask.bmd"), 0x10210000);
	unk5D54 = J3DModelLoaderDataBase::load(
	    JKRGetResource("/mario/bmd/water_hide_yuka_s.bmd"), 0x10210000);
	unk5D58 = J3DModelLoaderDataBase::load(
	    JKRGetResource("/mario/bmd/water_hide_kabe_s.bmd"), 0x10210000);

	unk5D5C = 128;
	unk5D5D = 80;
	unk5D5E = 200;
	unk5D5F = 0;
	unk5D60 = 367;
	unk5D62 = 1;
	unk5D63 = 1;

	unk5D64 = 150;
	unk5D65 = 255;

	unk5D68 = 256.0f;
	unk5D6C = 256.0f;
	unk5D70.set(0.0f, 0.0f, 0.0f);
	unk5D7C.set(0.0f, 0.0f, 0.0f);
	unk5D88[0]  = 0.5f;
	unk5D88[1]  = 0.35f;
	unk5D88[2]  = 0.5f;
	unk5D88[3]  = 10000.0f;
	unk5D88[4]  = 0.2f;
	unk5D88[5]  = 0.0f;
	unk5D88[6]  = 2.5f;
	unk5D88[7]  = 1.0f;
	unk5D88[8]  = 0.3f;
	unk5D88[9]  = 0.5f;
	unk5D88[10] = 0.8f;
	unk5D88[11] = 0.3f;
	unk5D88[12] = 0.08f;

	for (int i = 0; i < 17; ++i)
		mWaterParticleTypes[i] = new TWaterParticleType(prmNames[i]);

	unk5E00 = 0;
	unk5E04 = 1;
	unk5E08 = 5000.0f;
	unk5E0C = 6000.0f;
	MTXIdentity(unk5E10);
	unk5E40 = 100.0f;
	unk5E44 = 5;
	unk5E45 = 132;
	unk5E46 = 120;
	unk5E47 = 0;
	unk5E48 = 20;
	unk5E49 = 40;

	gpModelWaterManager = this;
}

void TModelWaterManager::loadAfter()
{
	unk5D34
	    = JDrama::TNameRefGen::search<TScreenTexture>("スクリーンテクスチャ")
	          ->getTexture();

	int flag = TFlagManager::getInstance()->getFlag(0x40000);
	if (flag > 60)
		flag = 60;

	f32 fVar1 = flag / 60.0f;

	if (gpMarDirector->getCurrentMap() == 1
	    && gpMarDirector->getCurrentStage() == 2)
		fVar1 = 1.0f;

	if (fVar1 < 1.0f)
		unk5E0C = fVar1 * 24000.0f + 8000.0f;
	else
		unk5D60 &= ~0x100;
}

f32 TModelWaterManager::getWPGravity(int i) const
{
	if (SMS_isDivingMap())
		return unk5D88[0];
	else
		return mWaterParticleTypes[mParticleTypeSOA[i]]->mGravity.get();
}

void TModelWaterManager::getWaterAlpha() const { }

bool TModelWaterManager::askHitWaterParticleOnGround(
    const JGeometry::TVec3<f32>& param_1)
{
	for (int i = 0; i < mParticleCount; ++i) {
		if ((mParticleFlagSOA[i] & 0xF) == 2
		    && abs(param_1.x - mParticlePositionSOA[i].x)
		               + abs(param_1.y - mParticlePositionSOA[i].y)
		               + abs(param_1.z - mParticlePositionSOA[i].z)
		           < 300.0f)
			return true;
	}

	return false;
}

static inline f32 MsRandF() { return rand() * (1.f / (RAND_MAX + 1)); }
static inline f32 rand11() { return ((rand() & 0xff) - 128) / 128.0f; }

void TModelWaterManager::makeEmit(const TWaterEmitInfo& param_1)
{
	mParticleLifetimeSOA[mParticleCount] = param_1.mAlive.get();
	mParticleFlagSOA[mParticleCount]     = param_1.mFlag.get() | 1;
	mParticleAttackSOA[mParticleCount]   = param_1.mAttack.get();
	mParticlePositionSOA[mParticleCount] = param_1.mPos.get();

	{
		JGeometry::TVec3<f32> local_3c(0.0f, 0.0f, 0.0f);
		local_3c.x = rand11();
		local_3c.y = rand11();
		local_3c.z = rand11();

		f32 dirScale = param_1.mDirTremble.get();
		VECScale(&local_3c, &local_3c, dirScale);
		VECAdd(param_1.mDir.get(), &local_3c, &local_3c);
		if (local_3c.x == 0.0f && local_3c.y == 0.0f && local_3c.z == 0.0f)
			local_3c.y = -1.0f;

		VECNormalize(&local_3c, &local_3c);
		f32 scale = rand11() * param_1.mPowTremble.get() + param_1.mPow.get();
		VECScale(&local_3c, &local_3c, scale);

		VECAdd(&local_3c, param_1.mV.get(), &local_3c);
		mParticleVelocitySOA[mParticleCount].x = local_3c.x;
		mParticleVelocitySOA[mParticleCount].y = local_3c.y;
		mParticleVelocitySOA[mParticleCount].z = local_3c.z;
	}

	mParticleSizeSOA[mParticleCount]
	    = rand11() * param_1.mSizeTremble.get() + param_1.mSize.get();

	mParticleTypeSOA[mParticleCount] = param_1.mType.get();

	unk2914[mParticleCount] = nullptr;
	unk2514[mParticleCount] = nullptr;
}

int TModelWaterManager::emitRequest(const TWaterEmitInfo& param_1)
{
	int particlesToSpawn = param_1.mNum.get();
	if (particlesToSpawn == 0)
		return 0;

	while (particlesToSpawn > 0) {
		if (mParticleCount >= unk10)
			break;
		makeEmit(param_1);
		--particlesToSpawn;
		mParticleCount += 1;
	}

	return (param_1.mNum.get() - particlesToSpawn) & 0xff;
}

void TModelWaterManager::splashSound(const JGeometry::TVec3<f32>& pos,
                                     f32 volume) const
{
	// TODO: is this the splash sound or some other sound?
	gpMSound->startSoundSet(0x6800, &pos, 0, volume, 0.0f, 0, 4);
}

// TODO: contents of this inline are a wild guess
void TModelWaterManager::splashGround(int i)
{
	mParticleLifetimeSOA[i] -= getPlaneVanishSpeed(unk2914[i]);
	gpPollution->clean(
	    mParticlePositionSOA[i].x, mParticlePositionSOA[i].y,
	    mParticlePositionSOA[i].z,
	    mWaterParticleTypes[mParticleTypeSOA[i]]->mCleanSize.get() * 10.0f);
}

void TModelWaterManager::touchingExec(int i) { }

// TODO: contents of this inline are a wild guess
void TModelWaterManager::splashWall(int i)
{
	mParticleLifetimeSOA[i] -= getPlaneVanishSpeed(unk2914[i]);

	gpPollution->clean(
	    mParticlePositionSOA[i].x, mParticlePositionSOA[i].y,
	    mParticlePositionSOA[i].z,
	    mWaterParticleTypes[mParticleTypeSOA[i]]->mCleanSize.get() * 32.0f);
}

void TModelWaterManager::splashWallPosSize(const JGeometry::TVec3<f32>& param_1,
                                           f32 param_2)
{
}

struct UnknownMWMStruct {
	f32 unk0;
	f32 unk4;
	f32 unk8;
	f32 unkC;
	f32 unk10;
	f32 unk14;
} gWaterManagerPlaneInfo;

// TODO: these 3 inlines could be confused with one another

f32 TModelWaterManager::getPlaneFriction(const TBGCheckData* plane)
{
	if (plane->isWaterSlip())
		return gWaterManagerPlaneInfo.unk0;
	else
		return gWaterManagerPlaneInfo.unkC;
}

f32 TModelWaterManager::getPlaneFall(const TBGCheckData* plane)
{
	if (plane->isWaterSlip())
		return gWaterManagerPlaneInfo.unk4;
	else
		return gWaterManagerPlaneInfo.unk10;
}

f32 TModelWaterManager::getPlaneVanishSpeed(const TBGCheckData* plane)
{
	if (plane->isWaterSlip())
		return gWaterManagerPlaneInfo.unk8;
	else
		return gWaterManagerPlaneInfo.unk14;
}

bool TModelWaterManager::askDoWaterHitCheck()
{
	return unk5E00 % unk5E04 == 0 ? true : false;
}

void TModelWaterManager::wind(const JGeometry::TVec3<f32>& param_1)
{
	for (int i = 0; i < mParticleCount; ++i)
		if ((mParticleFlagSOA[i] & 0xf) == 1)
			mParticleVelocitySOA[i] += param_1;
}

void TModelWaterManager::garbageCollect()
{
	u16 nextFreeSlot = 0;
	for (int i = 0; i < mParticleCount; ++i) {
		if (mParticleLifetimeSOA[i] <= 0.0f)
			continue;

		if (i != nextFreeSlot) {
			mParticleLifetimeSOA[nextFreeSlot] = mParticleLifetimeSOA[i];
			mParticleFlagSOA[nextFreeSlot]     = mParticleFlagSOA[i];
			mParticleAttackSOA[nextFreeSlot]   = mParticleAttackSOA[i];
			mParticlePositionSOA[nextFreeSlot] = mParticlePositionSOA[i];
			mParticleVelocitySOA[nextFreeSlot] = mParticleVelocitySOA[i];
			mParticleSizeSOA[nextFreeSlot]     = mParticleSizeSOA[i];
			mParticleTypeSOA[nextFreeSlot]     = mParticleTypeSOA[i];
			unk2514[nextFreeSlot]              = unk2514[i];
			unk2914[nextFreeSlot]              = unk2914[i];
		}

		++nextFreeSlot;
	}

	mParticleCount = nextFreeSlot;
}

void TModelWaterManager::move()
{
	f32 fVar1 = unk5E08;
	for (int i = 0; i < mParticleCount; ++i) {
		if (unk2514[i] != nullptr) {
			mStaticHitActor.mPosition = mParticlePositionSOA[i];
			mStaticHitActor.unk68     = i;

			if (!unk2514[i]->receiveMessage(&mStaticHitActor,
			                                HIT_MESSAGE_SPRAYED_BY_WATER))
				continue;

			splashSound(mParticlePositionSOA[i], mParticleSizeSOA[i]);

			if (MsRandF() < unk5D88[8])
				gpSplashManager->newSplash(mParticlePositionSOA[i], 5.0f);

			mParticleLifetimeSOA[i] = 0.0f;
		} else {
			JGeometry::TVec3<f32> thing;
			thing.sub(SMS_GetMarioPos(), mParticlePositionSOA[i]);

			if (thing.x * thing.x + thing.z * thing.z > fVar1 * fVar1) {
				mParticleLifetimeSOA[i] = 0.0f;
				continue;
			}

			f32 horVelSq
			    = mParticleVelocitySOA[i].x * mParticleVelocitySOA[i].x
			      + mParticleVelocitySOA[i].z * mParticleVelocitySOA[i].z;
			if (horVelSq > mParticleSizeSOA[i] * mParticleSizeSOA[i]) {
				f32 horLen = std::sqrtf(horVelSq);

				mParticleVelocitySOA[i].x
				    *= (mParticleSizeSOA[i] - 0.1f) * (1.0f / horLen);
				mParticleVelocitySOA[i].z
				    *= (mParticleSizeSOA[i] - 0.1f) * (1.0f / horLen);
			}

			switch (mParticleFlagSOA[i] & 0xf) {
			case 1:
				mParticleVelocitySOA[i].y += getWPGravity(i);

				mParticlePositionSOA[i].x += mParticleVelocitySOA[i].x;
				mParticlePositionSOA[i].y += mParticleVelocitySOA[i].y;
				mParticlePositionSOA[i].z += mParticleVelocitySOA[i].z;
				break;

			case 2: {
				mParticleVelocitySOA[i].x
				    = mParticleVelocitySOA[i].x * getPlaneFriction(unk2914[i])
				      + unk2914[i]->getNormal().x;
				mParticleVelocitySOA[i].y = getWPGravity(i);
				mParticleVelocitySOA[i].x
				    = mParticleVelocitySOA[i].x * getPlaneFriction(unk2914[i])
				      + unk2914[i]->getNormal().z;

				mParticlePositionSOA[i].x += mParticleVelocitySOA[i].x;
				mParticlePositionSOA[i].y += mParticleVelocitySOA[i].y;
				mParticlePositionSOA[i].z += mParticleVelocitySOA[i].z;
				break;
			}

			case 3:
				mParticlePositionSOA[i].y += getPlaneFall(unk2914[i]);
				break;
			}
		}
	}

	const TBGCheckData* local_248 = 0;
	f32 f31;

	for (int i = 0; i < mParticleCount; ++i) {
		if (mParticleVelocitySOA[i].y < 0.0f) {
			f31 = gpMap->checkGroundIgnoreWaterThrough(
			    mParticlePositionSOA[i].x, mParticlePositionSOA[i].y - fVar1,
			    mParticlePositionSOA[i].z, &local_248);

			if (!local_248->isLegal() || 1.0f + f31 < mParticlePositionSOA[i].y)
				continue;

			if (local_248->isWaterSurface()) {
				if (MsRandF() < unk5D88[11])
					SMS_EmitRippleTiny(&mParticlePositionSOA[i]);

				mParticleLifetimeSOA[i] = 0.0f;

				gpMSound->startSoundSet(0x6801, &mParticlePositionSOA[i], 0.0f,
				                        0.0f, 0.0f, 0, 4);
				continue;
			}

			if (local_248->isPool()) {
				mParticleLifetimeSOA[i] = 0.0f;
				if (MsRandF() < unk5D88[11])
					SMS_EmitRippleTiny(&mParticlePositionSOA[i]);

				splashSound(mParticlePositionSOA[i], mParticleSizeSOA[i]);
				continue;
			}

			mParticlePositionSOA[i].y = f31 - getWPGravity(i);

			if (getFlagBottom4Bits(i) == 3) {
				mParticleLifetimeSOA[i] = 0.0f;
				continue;
			}

			if (getFlagBottom4Bits(i) == 1) {
				if (MsRandF() < unk5D88[9])
					gpSplashManager->newSplash(mParticlePositionSOA[i], 5.0f);

				splashSound(mParticlePositionSOA[i], mParticleSizeSOA[i]);

				mParticleSizeSOA[i]
				    *= mWaterParticleTypes[mParticleTypeSOA[i]]->mMagnify.get();
				f32 fVar1;
				if (gpCamera->isLButtonCameraSpecifyMode(gpCamera->mMode))
					fVar1 = unk5D88[0];
				else
					fVar1 = unk5D88[1];

				if (MsRandF() < fVar1) {
					setFlagBottom4Bits(i, 2);
				} else {
					mParticleLifetimeSOA[i] = 0.0f;
				}

				continue;
			}

			unk2914[i] = local_248;
			if (local_248->mActor != nullptr) {
				mStaticHitActor.mPosition = mParticlePositionSOA[i];
				mStaticHitActor.unk68     = i;
				THitActor* hit            = (THitActor*)local_248->mActor;
				if (hit->receiveMessage(&mStaticHitActor,
				                        HIT_MESSAGE_SPRAYED_BY_WATER))
					mParticleLifetimeSOA[i] = 0.0f;
			}
		} else {
			static TBGWallCheckRecord wcheck;

			wcheck.set(mParticlePositionSOA[i].x,
			           mParticlePositionSOA[i].y + mParticleSizeSOA[i] * 0.5f,
			           mParticlePositionSOA[i].z, mParticleSizeSOA[i], 1,
			           TBGWallCheckRecord::IGNORE_WATER_THROUGH);

			if (gpMap->isTouchedWallsAndMoveXZ(&wcheck)) {
				const TBGCheckData* r27 = wcheck.mResultWalls[0];

				if (getFlagBottom4Bits(i) == 2) {
					mParticleLifetimeSOA[i] = 0.0f;
				} else {
					if (local_248 != nullptr && local_248->isLegal()
					    && mParticlePositionSOA[i].y
					           < mParticleSizeSOA[i]
					                     * mWaterParticleTypes
					                           [mParticleTypeSOA[i]]
					                               ->mMagnify.get()
					                 + f31) {
						mParticleLifetimeSOA[i] = 0.0f;
					} else {
						if (r27->mActor != nullptr) {
							mStaticHitActor.mPosition = mParticlePositionSOA[i];
							mStaticHitActor.unk68     = i;
							THitActor* hit            = (THitActor*)r27->mActor;
							hit->receiveMessage(&mStaticHitActor,
							                    HIT_MESSAGE_SPRAYED_BY_WATER);
						}

						JGeometry::TVec3<f32> local_194 = r27->getNormal();
						local_194.scale(
						    r27->mPlaneDistance
						    + mParticlePositionSOA[i].dot(r27->getNormal()));

						mParticlePositionSOA[i] -= local_194;

						if (getFlagBottom4Bits(i) == 1) {
							JGeometry::TVec3<f32> local_1d4 = r27->getNormal();
							local_1d4.scale(mParticleSizeSOA[i]);

							JGeometry::TVec3<f32> local_1A4
							    = mParticlePositionSOA[i];
							local_1A4 += local_1d4;
							if (MsRandF() < unk5D88[10])
								gpSplashManager->newSplash(local_1A4, 5.0f);

							splashSound(mParticlePositionSOA[i],
							            mParticleSizeSOA[i]);

							mParticleSizeSOA[i]
							    *= mWaterParticleTypes[mParticleTypeSOA[i]]
							           ->mMagnify.get();
							if (MsRandF() < unk5D88[2]) {
								setFlagBottom4Bits(i, 3);
							} else {
								mParticleLifetimeSOA[i] = 0.0f;
								continue;
							}
						}
					}
				}

				unk2914[i] = r27;
			} else {
				if (mParticleVelocitySOA[i].y >= 0.0f) {
					const TBGCheckData* local_b4 = nullptr;
					f32 dVar24 = gpMap->checkRoofIgnoreWaterThrough(
					    mParticlePositionSOA[i].x, mParticlePositionSOA[i].y,
					    mParticlePositionSOA[i].z, &local_b4);
					if (local_b4) {
						if (local_b4->mActor != nullptr) {
							mStaticHitActor.mPosition = mParticlePositionSOA[i];
							mStaticHitActor.unk68     = i;
							THitActor* hit = (THitActor*)local_b4->mActor;
							hit->receiveMessage(&mStaticHitActor,
							                    HIT_MESSAGE_SPRAYED_BY_WATER);
						}

						if (mParticlePositionSOA[i].y + mParticleSizeSOA[i]
						    > dVar24) {
							mParticlePositionSOA[i].y
							    = dVar24 - mParticleSizeSOA[i];
							mParticleVelocitySOA[i].x = 0.0f;
							mParticleVelocitySOA[i].y = 0.0f;
							mParticleVelocitySOA[i].z = 0.0f;

							setFlagBottom4Bits(i, 1);
							continue;
						}
					}
				}

				setFlagBottom4Bits(i, 1);
			}
		}
	}

	int r20 = -1;
	int r26 = -1;
	for (int i = 0; i < mParticleCount; ++i) {
		switch (mParticleFlagSOA[i] & 0xf) {
		case 1:
			mParticleLifetimeSOA[i] -= 1.0f;
			break;

		case 2: {
			if (mParticleLifetimeSOA[i] > unk5D68)
				mParticleLifetimeSOA[i] = unk5D68;
			gpMSound->startSoundSet(0x804, &mParticlePositionSOA[i], 0, 0.0f, 0,
			                        0, 4);

			splashGround(i); // TODO: probably wrong inline

			if (unk5D60 & 0x40) {
				if (r20 < 0) {
					r20 = i;
				} else {
					JGeometry::TVec3<f32> kek = vecBetweenParticles(i, r20);

					f32 max = unk5D88[6]
					          * (mParticleSizeSOA[i] + mParticleSizeSOA[r20]);

					if (kek.length() < max) {

						mParticleSizeSOA[r20]     = mParticleSizeSOA[i];
						mParticleLifetimeSOA[r20] = mParticleLifetimeSOA[i];
						mParticleLifetimeSOA[i]   = 0.0f;
					}
				}
			} else {
				r20 = i;
			}
			break;
		}
		case 3:
			if (mParticleLifetimeSOA[i] > unk5D6C)
				mParticleLifetimeSOA[i] = unk5D6C;

			splashWall(i); // TODO: probably wrong inline

			if (unk5D60 & 0x40) {
				if (r26 < 0) {
					r26 = i;
				} else {
					JGeometry::TVec3<f32> kek = vecBetweenParticles(i, r26);

					f32 max = unk5D88[6]
					          * (mParticleSizeSOA[i] + mParticleSizeSOA[r26]);

					// We're merging particles here I guess?
					if (kek.length() < max) {
						mParticleSizeSOA[r26]     = mParticleSizeSOA[i];
						mParticleLifetimeSOA[r26] = mParticleLifetimeSOA[i];
						mParticleLifetimeSOA[i]   = 0.0f;
					}
				}
			} else {
				r26 = i;
			}
			break;
		}
	}

	garbageCollect();
}

void TModelWaterManager::calcWorldMinMax()
{
	if (mParticleCount == 0) {
		JGeometry::TVec3<f32> marioPos = SMS_GetMarioPos();

		unk5D70.x = marioPos.x;
		unk5D70.y = marioPos.y;
		unk5D70.z = marioPos.z;

		unk5D7C.x = marioPos.x;
		unk5D7C.y = marioPos.y;
		unk5D7C.z = marioPos.z;

		unk5D70.x -= 1.0f;
		unk5D70.y -= 1.0f;
		unk5D70.z -= 1.0f;

		unk5D7C.x += 1.0f;
		unk5D7C.y += 1.0f;
		unk5D7C.z += 1.0f;
		return;
	}

	JGeometry::TVec3<f32> fVar789 = mParticlePositionSOA[0];
	fVar789.x -= 1.0f;
	fVar789.y -= 1.0f;
	fVar789.z -= 1.0f;
	JGeometry::TVec3<f32> fVar123 = mParticlePositionSOA[0];
	fVar789.x += 1.0f;
	fVar789.y += 1.0f;
	fVar789.z += 1.0f;
	for (int i = 0; i < mParticleCount; ++i) {
		fVar789.setMax(mParticlePositionSOA[i]);
		fVar123.setMin(mParticlePositionSOA[i]);
	}

	unk5D70.x = fVar789.x - 200.0f;
	unk5D70.y = fVar789.y - 200.0f;
	unk5D70.z = fVar789.z - 200.0f;

	unk5D7C.x = fVar123.x + 200.0f;
	unk5D7C.y = fVar123.y + 200.0f;
	unk5D7C.z = fVar123.z + 200.0f;
}

#pragma dont_inline on
void TModelWaterManager::calcDrawVtx(MtxPtr) { }
#pragma dont_inline off

void TModelWaterManager::calcVMMtxGround(MtxPtr param_1, f32 param_2,
                                         const JGeometry::TVec3<f32>& param_3,
                                         const JGeometry::TVec3<f32>& param_4,
                                         MtxPtr param_5)
{
	// TODO: matching this is ewwwwwwwwwwwwwwwwww

	f32 fVar6  = param_2 * param_4.x;
	f32 fVar7  = param_2 * param_4.y;
	f32 fVar11 = param_4.y * 2.0 + param_3.y;
	f32 fVar12 = -fVar6;

	f32 fVar10 = param_4.x * 2.0 + param_3.x;
	f32 fVar8  = param_2 * param_4.z;
	f32 fVar9  = param_4.z * 2.0 + param_3.z;
	f32 fVar13 = -fVar8;

	{
		f32 fVar4     = param_1[0][0];
		f32 fVar2     = param_1[0][1];
		f32 fVar1     = param_1[0][2];
		f32 fVar5     = param_1[0][3];
		param_5[0][0] = fVar4 * fVar7 + fVar2 * fVar12;
		param_5[0][1] = fVar1 * fVar8 + fVar4 * fVar6 + fVar2 * fVar7;
		param_5[0][2] = fVar2 * fVar13 + fVar1 * fVar7;
		param_5[0][3] = fVar5 + fVar1 * fVar9 + fVar4 * fVar10 + fVar2 * fVar11;
	}

	{
		f32 fVar1     = param_1[1][1];
		f32 fVar2     = param_1[1][2];
		f32 fVar3     = param_1[1][0];
		f32 fVar4     = param_1[1][3];
		param_5[1][0] = fVar3 * fVar7 + fVar1 * fVar12;
		param_5[1][1] = fVar2 * fVar8 + fVar3 * fVar6 + fVar1 * fVar7;
		param_5[1][2] = fVar1 * fVar13 + fVar2 * fVar7;
		param_5[1][3] = fVar4 + fVar2 * fVar9 + fVar3 * fVar10 + fVar1 * fVar11;
	}

	{
		f32 fVar1     = param_1[2][1];
		f32 fVar2     = param_1[2][2];
		f32 fVar3     = param_1[2][0];
		f32 fVar4     = param_1[2][3];
		param_5[2][0] = fVar3 * fVar7 + fVar1 * fVar12;
		param_5[2][1] = fVar2 * fVar8 + fVar3 * fVar6 + fVar1 * fVar7;
		param_5[2][2] = fVar1 * fVar13 + fVar2 * fVar7;
		param_5[2][3] = fVar4 + fVar2 * fVar9 + fVar3 * fVar10 + fVar1 * fVar11;
	}
}

void TModelWaterManager::calcVMMtxWall(MtxPtr param_1, f32 scale,
                                       const JGeometry::TVec3<f32>& param_3,
                                       const JGeometry::TVec3<f32>& param_4,
                                       MtxPtr param_5)
{
	// TODO: matching this is ewwwwwwwwwwwwwwwwww

	f32 fVar7  = scale * param_4.x;
	f32 fVar4  = param_3.y;
	f32 fVar8  = scale * param_4.z;
	f32 fVar11 = -fVar7;
	f32 fVar10 = param_4.x * 2.0 + param_3.x;
	f32 fVar9  = param_4.z * 2.0 + param_3.z;

	{
		f32 fVar3     = param_1[0][1];
		f32 fVar5     = param_1[0][2];
		f32 fVar6     = param_1[0][0];
		f32 fVar1     = param_1[0][3];
		(*param_5)[0] = fVar6 * fVar8 + fVar5 * fVar11;
		(*param_5)[1] = fVar3 * scale;
		(*param_5)[2] = fVar6 * fVar7 + fVar5 * fVar8;
		(*param_5)[3] = fVar1 + fVar5 * fVar9 + fVar6 * fVar10 + fVar3 * fVar4;
	}
	{
		f32 fVar1     = param_1[1][2];
		f32 fVar2     = param_1[1][1];
		f32 fVar3     = param_1[1][0];
		f32 fVar5     = param_1[1][3];
		param_5[1][0] = fVar3 * fVar8 + fVar1 * fVar11;
		param_5[1][1] = fVar2 * scale;
		param_5[1][2] = fVar3 * fVar7 + fVar1 * fVar8;
		param_5[1][3] = fVar5 + fVar1 * fVar9 + fVar3 * fVar10 + fVar2 * fVar4;
	}
	{
		f32 fVar1     = param_1[2][2];
		f32 fVar2     = param_1[2][1];
		f32 fVar3     = param_1[2][0];
		f32 fVar5     = param_1[2][3];
		param_5[2][0] = fVar3 * fVar8 + fVar1 * fVar11;
		param_5[2][1] = fVar2 * scale;
		param_5[2][2] = fVar3 * fVar7 + fVar1 * fVar8;
		param_5[2][3] = fVar5 + fVar1 * fVar9 + fVar3 * fVar10 + fVar2 * fVar4;
	}
}

void TModelWaterManager::calcVMAll(MtxPtr param_1)
{
	for (int i = 0; i < mParticleCount; ++i) {
		if ((mParticleFlagSOA[i] & 0xf) != 2)
			continue;

		f32 f = mParticleSizeSOA[i]
		        * ((1.0f - unk5D88[5]) * (mParticleLifetimeSOA[i] / unk5D68)
		           + unk5D88[5]);
		calcVMMtxGround(param_1, f, mParticlePositionSOA[i],
		                unk2914[i]->getNormal(), unk2D14[i]);
	}

	for (int i = 0; i < mParticleCount; ++i) {
		if ((mParticleFlagSOA[i] & 0xf) != 3)
			continue;

		f32 f = mParticleSizeSOA[i]
		        * ((1.0f - unk5D88[5]) * (mParticleLifetimeSOA[i] / unk5D6C)
		           + unk5D88[5]);
		matan(unk2914[i]->getNormal().z, unk2914[i]->getNormal().x);
		calcVMMtxWall(param_1, f, mParticlePositionSOA[i],
		              unk2914[i]->getNormal(), unk2D14[i]);
	}
}

void TModelWaterManager::drawTouching()
{
	SMS_SettingDrawShape(unk5D48, 0);
	for (int i = 0; i < mParticleCount; ++i) {
		if ((mParticleFlagSOA[i] & 0xf) == 2 && -unk5D2C < unk2D14[i][2][3]) {
			GXLoadPosMtxImm(unk2D14[i], 0);
			SMS_DrawShape(unk5D48, 0);
		}
	}

	SMS_SettingDrawShape(unk5D4C, 0);
	for (int i = 0; i < mParticleCount; ++i) {
		if ((mParticleFlagSOA[i] & 0xf) == 3 && -unk5D2C < unk2D14[i][2][3]) {
			GXLoadPosMtxImm(unk2D14[i], 0);
			SMS_DrawShape(unk5D4C, 0);
		}
	}
}

void TModelWaterManager::drawTouchingMask()
{
	SMS_SettingDrawShape(unk5D50, 0);
	for (int iVar2 = 0; iVar2 < mParticleCount; iVar2 = iVar2 + 1) {
		if ((mParticleFlagSOA[iVar2] & 0xf) == 2
		    || (mParticleFlagSOA[iVar2] & 0xf) == 3) {
			GXLoadPosMtxImm(unk2D14[iVar2], 0);
			SMS_DrawShape(unk5D50, 0);
		}
	}
}

void TModelWaterManager::drawSilhouette(MtxPtr param_1)
{
	if (!gpSilhouetteManager->isUnk48Positive())
		return;

	SMS_FillScreenAlpha(0);

	GXSetColorUpdate(GX_FALSE);
	GXSetAlphaUpdate(GX_TRUE);
	GXSetDstAlpha(GX_FALSE, 0);
	Mtx afStack_5c;
	MTXIdentity(afStack_5c);
	GXSetCurrentMtx(0);
	GXLoadPosMtxImm(afStack_5c, 0);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0xff, 0xff, 0xff, 0xff });
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	unk5D44->load(GX_TEXMAP0);
	GXSetZMode(1, GX_GEQUAL, 0);
	GXSetNumTexGens(1);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0, 0x7d);
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
	unk5D30->draw();
	GXSetCullMode(GX_CULL_BACK);
	GXSetNumTexGens(0);
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);

	SMS_SettingDrawShape(unk5D54, 0);
	for (int i = 0; i < mParticleCount; ++i) {
		if ((mParticleFlagSOA[i] & 0xf) == 2) {
			GXLoadPosMtxImm(unk2D14[i], 0);
			SMS_DrawShape(unk5D54, 0);
		}
	}

	SMS_SettingDrawShape(unk5D58, 0);
	for (int i = 0; i < mParticleCount; ++i) {
		if ((mParticleFlagSOA[i] & 0xf) == 2) {
			GXLoadPosMtxImm(unk2D14[i], 0);
			SMS_DrawShape(unk5D58, 0);
		}
	}

	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
	GXSetCurrentMtx(0);
	GXLoadPosMtxImm(param_1, 0);
	GXSetCullMode(GX_CULL_FRONT);
	GXSetChanMatColor(
	    GX_COLOR0A0,
	    (GXColor) { 0xff, 0xff, 0xff,
	                unk5D5D * gpSilhouetteManager->unk48 * 0.00390625f });
	GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTALPHA, GX_BL_ZERO, GX_LO_NOOP);
	if (unk5D60 & 0x20)
		SMS_DrawCube(unk5D70, unk5D7C);

	GXSetColorUpdate(GX_TRUE);
	GXSetAlphaUpdate(GX_TRUE);
	GXSetDstAlpha(GX_TRUE, 0);
	GXColor local_60 = gpSilhouetteManager->unk12;
	GXSetChanMatColor(
	    GX_COLOR0A0,
	    (GXColor) { local_60.r, local_60.g, local_60.b, unk5D5D * local_60.a });
	GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTALPHA, GX_BL_INVDSTALPHA, GX_LO_NOOP);
	SMS_DrawCube(unk5D70, unk5D7C);
}

void TModelWaterManager::drawWaterVolume(MtxPtr param_1)
{
	SMS_FillScreenAlpha(0);
	Mtx afStack_48;
	MTXIdentity(afStack_48);
	GXSetCurrentMtx(0);
	GXLoadPosMtxImm(afStack_48, 0);
	GXLoadNrmMtxImm(afStack_48, 0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0xff, 0xff, 0xff, unk5D5C });
	GXSetNumTexGens(0);
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
	GXSetColorUpdate(GX_FALSE);
	GXSetAlphaUpdate(GX_TRUE);
	GXSetDstAlpha(GX_FALSE, 0);

	SMS_SettingDrawShape(unk5D48, 0);
	for (int i = 0; i < mParticleCount; ++i) {
		if ((mParticleFlagSOA[i] & 0xf) == 2 && -unk5D2C < unk2D14[i][2][3]) {
			GXLoadPosMtxImm(unk2D14[i], 0);
			GXSetCullMode(GX_CULL_BACK);
			GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
			SMS_DrawShape(unk5D48, 0);
			GXSetCullMode(GX_CULL_FRONT);
			GXSetBlendMode(GX_BM_SUBTRACT, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
			SMS_DrawShape(unk5D48, 0);
		}
	}

	SMS_SettingDrawShape(unk5D4C, 0);
	for (int i = 0; i < mParticleCount; ++i) {
		if ((mParticleFlagSOA[i] & 0xf) == 3 && -unk5D2C < unk2D14[i][2][3]) {
			GXLoadPosMtxImm(unk2D14[i], 0);
			GXSetCullMode(GX_CULL_BACK);
			GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
			SMS_DrawShape(unk5D4C, 0);
			GXSetCullMode(GX_CULL_FRONT);
			GXSetBlendMode(GX_BM_SUBTRACT, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
			SMS_DrawShape(unk5D4C, 0);
		}
	}

	if ((unk5D7C.x - unk5D70.x) + (unk5D7C.z - unk5D70.z) < unk5D88[3]) {
		GXSetCurrentMtx(0);
		GXLoadPosMtxImm(param_1, 0);
		GXClearVtxDesc();
		GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
		GXSetCullMode(GX_CULL_FRONT);
		GXSetNumChans(1);
		GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
		              GX_DF_NONE, GX_AF_NONE);
		GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0xff, 0xff, 0xff, 0xff });
		GXSetNumTevStages(1);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
		              GX_COLOR0A0);
		GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
		GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTALPHA, GX_BL_ONE, GX_LO_NOOP);
		GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);

		for (int i = 0; i < unk5D62; ++i)
			SMS_DrawCube(unk5D70, unk5D7C);

		GXClearVtxDesc();
		GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
		GXSetCurrentMtx(0);
		GXLoadPosMtxImm(param_1, 0);
		GXSetCullMode(GX_CULL_FRONT);
		GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0xff, 0xff, 0xff, unk5D5D });
		GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTALPHA, GX_BL_ZERO, GX_LO_NOOP);

		if (unk5D60 & 0x20)
			SMS_DrawCube(unk5D70, unk5D7C);

		GXSetColorUpdate(GX_TRUE);
		GXSetAlphaUpdate(GX_TRUE);
		GXSetDstAlpha(GX_TRUE, 0);
		GXSetChanMatColor(GX_COLOR0A0, gModelWaterManagerWaterColor[unk5D5F]);
		GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTALPHA, GX_BL_INVDSTALPHA,
		               GX_LO_NOOP);

		SMS_DrawCube(unk5D70, unk5D7C);
	} else {
		GXSetCurrentMtx(0);
		GXLoadPosMtxImm(afStack_48, 0);
		GXSetCullMode(GX_CULL_BACK);
		GXSetNumChans(1);
		GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
		              GX_DF_NONE, GX_AF_NONE);
		GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0xff, 0xff, 0xff, 0xff });
		GXSetNumTevStages(1);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
		              GX_COLOR0A0);
		GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
		GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
		GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTALPHA, GX_BL_ONE, GX_LO_NOOP);
		GXSetColorUpdate(GX_FALSE);
		GXSetAlphaUpdate(GX_TRUE);
		GXSetDstAlpha(GX_TRUE, 0);
		if (unk5D60 & 0x10)
			for (int i = 0; i < unk5D63; ++i)
				drawTouchingMask();
		else
			for (int i = 0; i < unk5D63; ++i)
				drawTouching();

		GXSetCurrentMtx(0);
		GXLoadPosMtxImm(param_1, 0);
		GXClearVtxDesc();
		GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
		GXSetCullMode(GX_CULL_NONE);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
		GXSetNumChans(1);
		GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
		              GX_AF_NONE);

		GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0xff, 0xff, 0xff, unk5D5D });
		GXSetNumTevStages(1);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
		              GX_COLOR0A0);
		GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
		GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
		GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTALPHA, GX_BL_ZERO, GX_LO_NOOP);
		if ((unk5D60 & 0x20) != 0)
			SMS_DrawCube(unk5D70, unk5D7C);
		GXClearVtxDesc();
		GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
		GXSetNumChans(1);
		GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
		              GX_AF_NONE);
		GXSetChanMatColor(GX_COLOR0A0, gModelWaterManagerWaterColor[unk5D5F]);
		GXSetChanCtrl(GX_COLOR1A1, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
		              GX_AF_NONE);
		GXSetColorUpdate(GX_TRUE);
		GXSetAlphaUpdate(GX_TRUE);
		GXSetDstAlpha(GX_TRUE, 0);
		GXSetCullMode(GX_CULL_BACK);
		GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTALPHA, GX_BL_INVDSTALPHA,
		               GX_LO_NOOP);
		GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
		if (unk5D60 & 0x10)
			drawTouchingMask();
		else
			drawTouching();
	}

	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
	GXSetColorUpdate(GX_TRUE);
	GXSetAlphaUpdate(GX_FALSE);
	GXSetDstAlpha(GX_FALSE, 0);
}

void TModelWaterManager::drawMirror(MtxPtr param_1)
{

	GXSetCurrentMtx(0);
	GXLoadPosMtxImm(param_1, 0);
	GXSetCullMode(GX_CULL_NONE);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0xff, 0xff, 0xff, 0xff });
	GXSetNumTexGens(0);
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
	GXSetZCompLoc(GX_TRUE);
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
	GXSetColorUpdate(GX_FALSE);
	GXSetAlphaUpdate(GX_TRUE);
	GXSetDstAlpha(GX_FALSE, 0);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
	SMS_SettingDrawShape(unk5D54, 0);

	const TBGCheckData* pTVar4 = SMS_GetMarioGroundPlane();
	f32 fVar1                  = pTVar4->getPlaneDistance();
	for (int i = 0; i < mParticleCount; ++i) {
		if ((mParticleFlagSOA[i] & 0xf) == 2
		    && SMS_GetMarioGroundPlane()->isLegal()) {
			GXLoadPosMtxImm(unk2D14[i], 0);
			SMS_DrawShape(unk5D54, 0);
		}
	}
	JGeometry::TVec3<f32> local_bc[4][2];

	f32 fVar3 = 1.0 / pTVar4->getNormal().y;

	for (int i = 0; i < 4; ++i) {
		local_bc[i][0].x = SMS_GetMarioPos().x + JMASSin(i * 0x4000) * 1000.0f;
		local_bc[i][0].z = SMS_GetMarioPos().z + JMASCos(i * 0x4000) * 1000.0f;
		local_bc[i][0].y
		    = fVar3
		          * -(fVar1 + pTVar4->getNormal().x * local_bc[0][0].x
		              + pTVar4->getNormal().z * local_bc[0][0].z)
		      + 4.0f;

		local_bc[i][1].x = JMASSin(i * 0x4000) * 1000.0f + SMS_GetMarioPos().x;
		local_bc[i][1].z = JMASSin(i * 0x4000) * 1000.0f + SMS_GetMarioPos().z;
		local_bc[i][1].y
		    = fVar3
		          * -(fVar1 + pTVar4->getNormal().x * local_bc[0][0].x
		              + pTVar4->getNormal().z * local_bc[0][0].z)
		      + 4.0f;
	}

	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	Mtx afStack_ec;
	MTXIdentity(afStack_ec);
	gpMirrorModelManager->unk24->drawSetting(afStack_ec);
	GXSetCullMode(GX_CULL_NONE);
	GXSetCurrentMtx(0);
	GXLoadPosMtxImm(param_1, 0);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetNumTexGens(1);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, 0x1e, 0, 0x7d);
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_ZERO, GX_BL_SRCALPHA, GX_LO_NOOP);
	GXSetZCompLoc(GX_TRUE);
	GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);

	GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 10);
	GXPosition3f32(SMS_GetMarioPos().x, SMS_GetMarioPos().y + 4.0f,
	               SMS_GetMarioPos().z);
	GXColor4u8(0xff, 0xff, 0xff, unk5D64);
	GXPosition3f32(local_bc[0][0].x, local_bc[0][0].y, local_bc[0][0].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[0][1].x, local_bc[0][1].y, local_bc[0][1].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[1][0].x, local_bc[1][0].y, local_bc[1][0].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[1][1].x, local_bc[1][1].y, local_bc[1][1].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[2][0].x, local_bc[2][0].y, local_bc[2][0].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[2][1].x, local_bc[2][1].y, local_bc[2][1].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[3][0].x, local_bc[3][0].y, local_bc[3][0].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[3][1].x, local_bc[3][1].y, local_bc[3][1].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[0][0].x, local_bc[0][0].y, local_bc[0][0].z);
	GXColor4u8(0, 0, 0, 0);
	GXEnd();

	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetCullMode(GX_CULL_NONE);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTALPHA, GX_BL_INVDSTALPHA, GX_LO_NOOP);
	GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
	GXSetColorUpdate(GX_TRUE);
	GXSetAlphaUpdate(GX_TRUE);
	GXSetDstAlpha(GX_TRUE, 0);

	GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 10);
	GXPosition3f32(SMS_GetMarioPos().x, SMS_GetMarioPos().y + 4.0f,
	               SMS_GetMarioPos().z);
	GXColor4u8(0xff, 0xff, 0xff, unk5D64);
	GXPosition3f32(local_bc[0][0].x, local_bc[0][0].y, local_bc[0][0].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[0][1].x, local_bc[0][1].y, local_bc[0][1].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[1][0].x, local_bc[1][0].y, local_bc[1][0].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[1][1].x, local_bc[1][1].y, local_bc[1][1].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[2][0].x, local_bc[2][0].y, local_bc[2][0].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[2][1].x, local_bc[2][1].y, local_bc[2][1].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[3][0].x, local_bc[3][0].y, local_bc[3][0].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[3][1].x, local_bc[3][1].y, local_bc[3][1].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[0][0].x, local_bc[0][0].y, local_bc[0][0].z);
	GXColor4u8(0, 0, 0, 0);
	GXEnd();

	GXSetColorUpdate(GX_TRUE);
	GXSetAlphaUpdate(GX_FALSE);
	GXSetDstAlpha(GX_FALSE, 0);
}

void init_sphere_glist() { }

extern "C" void ReInitializeGX();

static u8 tmp_data;
static void* sphere_glist_p;
static void* sphere_pos_t;

void TModelWaterManager::drawShineShadowVolume(MtxPtr param_1)
{

	if (gpMarDirector->getCurrentMap() == 1) {
		static bool initialized = false;
		if (!initialized) {
			sphere_glist_p = &tmp_data;
			initialized    = 1;
			sphere_pos_t   = &tmp_data + 1000;
		}

		f32 f30 = (((unk5E0C + unk5E40) - unk5E0C) / f32(unk5E44 - 1));
		f32 f31 = unk5E0C;

		GXColor local_2C;
		int r27 = unk5E45;

		ReInitializeGX();

		Mtx afStack_f8;
		MTXIdentity(afStack_f8);

		Mtx local_c8;
		local_c8[2][1] = 0.0;
		local_c8[2][0] = 0.0;
		local_c8[1][2] = 0.0;
		local_c8[1][0] = 0.0;
		local_c8[0][2] = 0.0;
		local_c8[0][1] = 0.0;
		local_c8[0][3] = 0.0;
		local_c8[1][3] = 3600.0;
		local_c8[2][3] = -7458.0;

		GXClearVtxDesc();
		GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
		GXSetNumChans(1);
		GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
		              GX_DF_NONE, GX_AF_NONE);
		GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
		              GX_DF_NONE, GX_AF_NONE);
		GXSetNumTexGens(0);
		GXSetNumTevStages(1);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
		              GX_COLOR_NULL);
		GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
		                GX_CC_ZERO);
		GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
		                GX_TEVPREV);
		GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
		                GX_CA_A0);
		GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
		                GX_TEVPREV);
		GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
		local_2C.a = r27;
		GXSetTevColor(GX_TEVREG0, local_2C);
		GXSetBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_NOOP);
		GXSetColorUpdate(GX_FALSE);
		GXSetAlphaUpdate(GX_TRUE);
		GXSetDstAlpha(GX_FALSE, 0);
		GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
		GXSetCullMode(GX_CULL_NONE);
		GXLoadPosMtxImm(afStack_f8, 0);

		GXBegin(GX_QUADS, GX_VTXFMT0, 4);
		GXPosition3s16(-1000, 1000, -200);
		GXPosition3s16(1000, 1000, -200);
		GXPosition3s16(1000, -1000, -200);
		GXPosition3s16(-1000, -1000, -200);
		GXEnd();

		GXColor local_28;
		int r31    = unk5E44;
		local_28.a = f32(0xff - unk5E45) / unk5E44 + 0.5f;
		GXSetTevColor(GX_TEVREG0, local_28);
		GXSetZMode(GX_TRUE, GX_GREATER, GX_TRUE);
		GXClearVtxDesc();
		GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_RGBA4, 15);
		GXSetArray(GX_VA_POS, sphere_pos_t, 6);
		int iVar5 = 0;
		do {
			local_c8[0][0] = iVar5 * f30 + f31;
			local_c8[1][1] = iVar5 * f30 + f31;
			local_c8[2][2] = iVar5 * f30 + f31;
			Mtx afStack_98;
			MTXConcat(param_1, local_c8, afStack_98);
			GXLoadPosMtxImm(afStack_98, 0);
			GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
			GXSetCullMode(GX_CULL_FRONT);
			GXCallDisplayList(sphere_glist_p, 0x760);
			GXSetBlendMode(GX_BM_SUBTRACT, GX_BL_ZERO, GX_BL_ZERO, GX_LO_NOOP);
			GXSetCullMode(GX_CULL_BACK);
			GXCallDisplayList(sphere_glist_p, 0x760);
			++iVar5;
		} while (iVar5 < r31);

		GXClearVtxDesc();
		GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);

		GXColor local_3c;
		local_3c.r = unk5E47;
		local_3c.g = unk5E48;
		local_3c.b = unk5E49;
		local_3c.a = 0xff;
		GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
		                GX_CC_C0);
		GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
		                GX_CA_ZERO);
		GXSetTevColor(GX_TEVREG0, local_3c);
		GXSetBlendMode(GX_BM_BLEND, GX_BL_INVDSTALPHA, GX_BL_DSTALPHA,
		               GX_LO_NOOP);
		GXSetColorUpdate(GX_TRUE);
		GXSetAlphaUpdate(GX_TRUE);
		GXSetDstAlpha(GX_TRUE, 0);
		GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
		GXSetCullMode(GX_CULL_NONE);
		GXLoadPosMtxImm(afStack_f8, 0);

		GXBegin(GX_QUADS, GX_VTXFMT0, 4);
		GXPosition3s16(-1000, 1000, -200);
		GXPosition3s16(1000, 1000, -200);
		GXPosition3s16(1000, -1000, -200);
		GXPosition3s16(-1000, -1000, -200);
		GXEnd();

		GXSetAlphaUpdate(GX_FALSE);
		GXSetDstAlpha(GX_FALSE, 0);
	}
}

void TModelWaterManager::drawRefracAndSpec() const
{

	Mtx afStack_3c;
	PSMTXIdentity(afStack_3c);
	GXSetCurrentMtx(0);
	GXLoadPosMtxImm(afStack_3c, 0);
	GXLoadNrmMtxImm(afStack_3c, 0);
	GXSetNumChans(0);
	GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetNumTexGens(2);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, 0x1e, 0, 0x7d);
	GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX3x4, GX_TG_TEX0, 0x3c, 0, 0x7d);
	Mtx afStack_6c;
	C_MTXLightPerspective(afStack_6c, gpCamera->getFovy(),
	                      gpCamera->getAspect(), 0.5f, -0.5f, 0.5f, 0.5f);
	GXLoadTexMtxImm(afStack_6c, 0x1e, GX_MTX3x4);
	GXSetNumIndStages(1);
	GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
	GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD1, GX_TEXMAP1);
	GXSetTevIndWarp(GX_TEVSTAGE0, GX_INDTEXSTAGE0, 1, 0, GX_ITM_0);
	f32 local_84[2][3];
	local_84[0][0] = unk5D1C;
	local_84[0][1] = 0.0f;
	local_84[0][2] = 0.0f;
	local_84[1][0] = 0.0f;
	local_84[1][1] = unk5D1C;
	local_84[1][2] = 0.0f;
	GXSetIndTexMtx(GX_ITM_0, local_84, 0);
	GXSetCullMode(GX_CULL_NONE);
	GXSetZCompLoc(1);
	unk5D34->load(GX_TEXMAP0);
	unk5D38->load(GX_TEXMAP1);
	unk5D3C->load(GX_TEXMAP2);
	GXSetTevColor(GX_TEVREG0, (GXColor) { 0, 0, 0, unk5D65 });
	GXSetNumTevStages(2);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
	                GX_CC_TEXC);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
	                GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
	                GX_TEVPREV);
	GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP2, GX_COLOR_NULL);
	GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
	                GX_CC_CPREV);
	GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
	                GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_TEXA,
	                GX_CA_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
	                GX_TEVPREV);
	GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f,
	         (GXColor) { 0xff, 0xff, 0xff, 0xff });
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
	GXSetColorUpdate(GX_TRUE);
	GXSetAlphaUpdate(GX_FALSE);
	GXSetDstAlpha(GX_FALSE, 0);
	if (unk5D60 & 2)
		unk5D30->draw();

	GXSetNumIndStages(0);
	GXSetTevDirect(GX_TEVSTAGE0);
	GXSetNumTexGens(1);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0, 0x7d);
	GXSetTevColor(GX_TEVREG0, gModelWaterManagerWaterColor[unk5D5F]);
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP2, GX_COLOR_NULL);
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
	                GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
	                GX_TEVPREV);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
	if (unk5D60 & 4)
		unk5D30->draw();
	unk5D40->load(GX_TEXMAP3);
	GXSetTevColor(GX_TEVREG0, unk5D20);
	GXSetTevColor(GX_TEVREG1, unk5D24);
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP3, GX_COLOR_NULL);
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_C1);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
	                GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
	                GX_CA_TEXA);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
	                GX_TEVPREV);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
	if (unk5D60 & 8)
		unk5D30->draw();
}

void TModelWaterManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	MtxPtr r29 = param_2->mViewMtx;

	if (param_1 & 1) {
		move();
		calcWorldMinMax();
		unk5E00 += 1;
	}

	if (param_1 & 4) {
		if (unk5D60 & 0x80)
			TTimeRec::startTimer(0xFF, 0x00, 0x00, 0xFE);

		calcDrawVtx(param_2->mViewMtx);
		calcVMAll(param_2->mViewMtx);

		if (unk5D60 & 0x80)
			TTimeRec::endTimer();
	}

	if (param_1 & 8) {
		if (unk5D60 & 0x80)
			TTimeRec::snapGxTimeStart(0xFF, 0x00, 0x00, 0xFD);

		drawSilhouette(r29);
		drawWaterVolume(r29);

		if ((unk5D60 & 0x1) && !gpMirrorModelManager->isUnk18Present())
			drawMirror(r29);

		MTXCopy(r29, unk5E10);
		if ((unk5D60 & 0x100) && !(unk5D60 & 0x200))
			drawShineShadowVolume(param_2->mViewMtx);

		if (unk5D60 & 0x80)
			TTimeRec::snapGxTimeEnd();
	}

	if (param_1 & 0x80) {
		if (unk5D60 & 0x80)
			TTimeRec::snapGxTimeStart(0xFF, 0x00, 0x00, 0xFC);

		drawRefracAndSpec();

		if ((unk5D60 & 0x100) && (unk5D60 & 0x200))
			drawShineShadowVolume(param_2->mViewMtx);

		if (unk5D60 & 0x80)
			TTimeRec::snapGxTimeEnd();
	}
}

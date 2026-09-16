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

bool SMS_isDivingMap();

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

static const f32 cShineShadowVolumePos[] = { 0.0f, 3600.0f, -7458.0f };

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
	    JKRGetResource("/mario/bmd/water_shadow_yuka.bmd"),
	    J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
	        | (1 << J3DMLF_TevStageNumShift));
	unk5D4C = J3DModelLoaderDataBase::load(
	    JKRGetResource("/mario/bmd/water_shadow_kabe.bmd"),
	    J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
	        | (1 << J3DMLF_TevStageNumShift));
	unk5D50 = J3DModelLoaderDataBase::load(
	    JKRGetResource("/mario/bmd/watermask.bmd"),
	    J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
	        | (1 << J3DMLF_TevStageNumShift));
	unk5D54 = J3DModelLoaderDataBase::load(
	    JKRGetResource("/mario/bmd/water_hide_yuka_s.bmd"),
	    J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
	        | (1 << J3DMLF_TevStageNumShift));
	unk5D58 = J3DModelLoaderDataBase::load(
	    JKRGetResource("/mario/bmd/water_hide_kabe_s.bmd"),
	    J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
	        | (1 << J3DMLF_TevStageNumShift));

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

	if (fVar1 < 1.0f) {
		f32 fVar2 = 24000.0f;
		unk5E0C   = fVar1 * fVar2 + 8000.0f;
	} else
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
static inline f32 rand11()
{
	return (1.0f / 128.0f) * ((rand() & 0xff) - 128);
}

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

u8 TModelWaterManager::emitRequest(const TWaterEmitInfo& param_1)
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

	u8 result = (param_1.mNum.get() - particlesToSpawn) & 0xff;
	return result;
}

void TModelWaterManager::splashSound(const JGeometry::TVec3<f32>& pos,
                                     f32 volume) const
{
	// TODO: is this the splash sound or some other sound?
	gpMSound->startSoundSet(MSD_SE_WT_GND, &pos, 0, volume, 0.0f, 0, 4);
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
};

UnknownMWMStruct gWaterManagerPlaneInfo
    = { 0.93f, -3.0f, 2.0f, 0.7f, -1.0f, 1.0f };

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

				gpMSound->startSoundSet(MSD_SE_WT_INTO_WATER,
				                        &mParticlePositionSOA[i], 0.0f, 0.0f,
				                        0.0f, 0, 4);
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
				if (gpCamera->isLButtonCamera())
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
							local_1d4 += mParticlePositionSOA[i];
							if (MsRandF() < unk5D88[10])
								gpSplashManager->newSplash(local_1d4, 5.0f);

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
			gpMSound->startSoundSet(MSD_SE_PO_WT_DRY_UP,
			                        &mParticlePositionSOA[i], 0, 0.0f, 0, 0, 4);

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
		unk5D70 = SMS_GetMarioPos();
		unk5D7C = SMS_GetMarioPos();

		unk5D70.x -= 1.0f;
		unk5D70.y -= 1.0f;
		unk5D70.z -= 1.0f;

		unk5D7C.x += 1.0f;
		unk5D7C.y += 1.0f;
		unk5D7C.z += 1.0f;
		return;
	}

	f32 fVar789 = mParticlePositionSOA[0].x - 1.0f;
	f32 fVar788 = mParticlePositionSOA[0].y - 1.0f;
	f32 fVar787 = mParticlePositionSOA[0].z - 1.0f;
	f32 fVar123 = mParticlePositionSOA[0].x + 1.0f;
	f32 fVar122 = mParticlePositionSOA[0].y + 1.0f;
	f32 fVar121 = mParticlePositionSOA[0].z + 1.0f;
	for (int i = 1; i < mParticleCount; ++i) {
		if (fVar789 > mParticlePositionSOA[i].x)
			fVar789 = mParticlePositionSOA[i].x;
		if (fVar788 > mParticlePositionSOA[i].y)
			fVar788 = mParticlePositionSOA[i].y;
		if (fVar787 > mParticlePositionSOA[i].z)
			fVar787 = mParticlePositionSOA[i].z;
		if (fVar123 < mParticlePositionSOA[i].x)
			fVar123 = mParticlePositionSOA[i].x;
		if (fVar122 < mParticlePositionSOA[i].y)
			fVar122 = mParticlePositionSOA[i].y;
		if (fVar121 < mParticlePositionSOA[i].z)
			fVar121 = mParticlePositionSOA[i].z;
	}

	unk5D70.x = fVar789 - 200.0f;
	unk5D70.y = fVar788 - 200.0f;
	unk5D70.z = fVar787 - 200.0f;

	unk5D7C.x = fVar123 + 200.0f;
	unk5D7C.y = fVar122 + 200.0f;
	unk5D7C.z = fVar121 + 200.0f;
}

#pragma dont_inline on
void TModelWaterManager::calcDrawVtx(MtxPtr param_1)
{
	unk5D30->reset();

	for (int i = 0; i < mParticleCount; ++i) {
		if ((mParticleFlagSOA[i] & 0xf) != 1)
			continue;
		if (!(mParticleLifetimeSOA[i]
		      < mWaterParticleTypes[mParticleTypeSOA[i]]->mAlive.get()
		            - unk5D88[7]))
			continue;

		JGeometry::TVec3<f32> vel;
		JGeometry::TVec3<f32> pos;
		PSMTXMultVec(param_1, &mParticlePositionSOA[i], &pos);
		if (pos.z > 0.0f || pos.z < -unk5D28)
			continue;

		PSMTXMultVecSR(param_1, &mParticleVelocitySOA[i], &vel);
		vel *= mWaterParticleTypes[mParticleTypeSOA[i]]->mExtension.get();

		JGeometry::TVec3<f32> vtx[4];
		f32 halfSize = 0.5f * mParticleSizeSOA[i];
		f32 radius   = 1.414f * halfSize;
		f32 len2     = vel.x * vel.x + vel.y * vel.y;
		if (len2 > 1.0f) {
			f32 scale = (1.0f / std::sqrtf(len2)) * radius;
			f32 x     = vel.x * scale;
			f32 y     = vel.y * scale;

			vtx[0].set(pos.x + x + vel.x * unk5D18,
			           pos.y + y + vel.y * unk5D18, pos.z);
			vtx[1].set(pos.x + y, pos.y - x, pos.z);
			vtx[2].set(pos.x - x - vel.x * unk5D18,
			           pos.y - y - vel.y * unk5D18, pos.z);
			vtx[3].set(pos.x - y, pos.y + x, pos.z);
		} else {
			vtx[0].set(pos.x - radius, pos.y + radius, pos.z);
			vtx[1].set(pos.x + radius, pos.y + radius, pos.z);
			vtx[2].set(pos.x + radius, pos.y - radius, pos.z);
			vtx[3].set(pos.x - radius, pos.y - radius, pos.z);
		}
		unk5D30->request(vtx);
	}

	unk5D30->setEnd();
}
#pragma dont_inline off

void TModelWaterManager::calcVMMtxGround(MtxPtr param_1, f32 param_2,
                                         const JGeometry::TVec3<f32>& param_3,
                                         const JGeometry::TVec3<f32>& param_4,
                                         MtxPtr param_5)
{
	// TODO: matching this is ewwwwwwwwwwwwwwwwww

	f32 fVar6  = param_2 * param_4.x;
	f32 fVar8  = param_2 * param_4.z;
	f32 fVar11 = param_4.y * 2.0 + param_3.y;
	f32 fVar12 = -fVar6;

	f32 fVar10 = param_4.x * 2.0 + param_3.x;
	f32 fVar7  = param_2 * param_4.y;
	f32 fVar13 = -fVar8;
	f32 fVar9  = param_4.z * 2.0 + param_3.z;

	{
		f32 fVar1     = param_1[0][1];
		f32 fVar2     = param_1[0][2];
		f32 fVar3     = param_1[0][0];
		f32 fVar4     = param_1[0][3];
		param_5[0][0] = fVar3 * fVar7 + fVar1 * fVar12;
		param_5[0][1] = fVar2 * fVar8 + fVar3 * fVar6 + fVar1 * fVar7;
		param_5[0][2] = fVar1 * fVar13 + fVar2 * fVar7;
		param_5[0][3] = fVar4 + fVar2 * fVar9 + fVar3 * fVar10 + fVar1 * fVar11;
	}

	{
		f32 fVar1     = param_1[1][1];
		f32 fVar2     = param_1[1][2];
		f32 fVar3     = param_1[1][0];
		f32 fVar4     = param_1[1][3];
		param_5[1][0] = fVar7 * fVar3 + fVar1 * fVar12;
		param_5[1][1] = fVar2 * fVar8 + fVar6 * fVar3 + fVar1 * fVar7;
		param_5[1][2] = fVar1 * fVar13 + fVar2 * fVar7;
		param_5[1][3] = fVar4 + fVar2 * fVar9 + fVar3 * fVar10 + fVar1 * fVar11;
	}

	{
		f32 fVar1     = param_1[2][1];
		f32 fVar2     = param_1[2][2];
		f32 fVar3     = param_1[2][0];
		f32 fVar4     = param_1[2][3];
		param_5[2][0] = fVar3 * fVar7 + fVar1 * fVar12;
		param_5[2][1] = fVar2 * fVar8 + fVar3 * fVar6 + fVar7 * fVar1;
		param_5[2][2] = fVar1 * fVar13 + fVar7 * fVar2;
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
	f32 fVar12 = param_3.y;
	f32 fVar8  = scale * param_4.z;
	f32 fVar11 = -fVar7;
	f32 fVar10 = param_4.x * 2.0 + param_3.x;
	f32 fVar9  = param_4.z * 2.0 + param_3.z;

	{
		f32 fVar1     = param_1[0][1];
		f32 fVar2     = param_1[0][2];
		f32 fVar3     = param_1[0][0];
		f32 fVar4     = param_1[0][3];
		param_5[0][0] = fVar2 * fVar11 + fVar3 * fVar8;
		param_5[0][1] = fVar1 * scale;
		param_5[0][2] = fVar2 * fVar8 + fVar3 * fVar7;
		param_5[0][3] = fVar1 * fVar12 + fVar3 * fVar10 + fVar2 * fVar9 + fVar4;
	}
	{
		f32 fVar1     = param_1[1][1];
		f32 fVar2     = param_1[1][2];
		f32 fVar3     = param_1[1][0];
		f32 fVar4     = param_1[1][3];
		param_5[1][0] = fVar2 * fVar11 + fVar3 * fVar8;
		param_5[1][1] = fVar1 * scale;
		param_5[1][2] = fVar2 * fVar8 + fVar3 * fVar7;
		param_5[1][3] = fVar1 * fVar12 + fVar3 * fVar10 + fVar2 * fVar9 + fVar4;
	}
	{
		f32 fVar1     = param_1[2][1];
		f32 fVar2     = param_1[2][2];
		f32 fVar3     = param_1[2][0];
		f32 fVar4     = param_1[2][3];
		param_5[2][0] = fVar2 * fVar11 + fVar3 * fVar8;
		param_5[2][1] = fVar1 * scale;
		param_5[2][2] = fVar2 * fVar8 + fVar3 * fVar7;
		param_5[2][3] = fVar1 * fVar12 + fVar3 * fVar10 + fVar2 * fVar9 + fVar4;
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
			GXLoadPosMtxImm(unk2D14[i], GX_PNMTX0);
			SMS_DrawShape(unk5D48, 0);
		}
	}

	SMS_SettingDrawShape(unk5D4C, 0);
	for (int i = 0; i < mParticleCount; ++i) {
		if ((mParticleFlagSOA[i] & 0xf) == 3 && -unk5D2C < unk2D14[i][2][3]) {
			GXLoadPosMtxImm(unk2D14[i], GX_PNMTX0);
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
			GXLoadPosMtxImm(unk2D14[iVar2], GX_PNMTX0);
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
	GXSetCurrentMtx(GX_PNMTX0);
	GXLoadPosMtxImm(afStack_5c, GX_PNMTX0);
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
			GXLoadPosMtxImm(unk2D14[i], GX_PNMTX0);
			SMS_DrawShape(unk5D54, 0);
		}
	}

	SMS_SettingDrawShape(unk5D58, 0);
	for (int i = 0; i < mParticleCount; ++i) {
		if ((mParticleFlagSOA[i] & 0xf) == 3) {
			GXLoadPosMtxImm(unk2D14[i], GX_PNMTX0);
			SMS_DrawShape(unk5D58, 0);
		}
	}

	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
	GXSetCurrentMtx(GX_PNMTX0);
	GXLoadPosMtxImm(param_1, GX_PNMTX0);
	GXSetCullMode(GX_CULL_FRONT);
	GXSetChanMatColor(
	    GX_COLOR0A0,
	    (GXColor) { 0xff, 0xff, 0xff,
	                unk5D5D * (gpSilhouetteManager->unk48 * 0.00390625f) });
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
	GXSetCurrentMtx(GX_PNMTX0);
	GXLoadPosMtxImm(afStack_48, GX_PNMTX0);
	GXLoadNrmMtxImm(afStack_48, GX_PNMTX0);
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
			GXLoadPosMtxImm(unk2D14[i], GX_PNMTX0);
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
			GXLoadPosMtxImm(unk2D14[i], GX_PNMTX0);
			GXSetCullMode(GX_CULL_BACK);
			GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
			SMS_DrawShape(unk5D4C, 0);
			GXSetCullMode(GX_CULL_FRONT);
			GXSetBlendMode(GX_BM_SUBTRACT, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
			SMS_DrawShape(unk5D4C, 0);
		}
	}

	if ((unk5D7C.x - unk5D70.x) + (unk5D7C.z - unk5D70.z) < unk5D88[3]) {
		GXSetCurrentMtx(GX_PNMTX0);
		GXLoadPosMtxImm(param_1, GX_PNMTX0);
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
		GXSetCurrentMtx(GX_PNMTX0);
		GXLoadPosMtxImm(param_1, GX_PNMTX0);
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
		GXSetCurrentMtx(GX_PNMTX0);
		GXLoadPosMtxImm(afStack_48, GX_PNMTX0);
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
		GXSetDstAlpha(GX_FALSE, 0);
		if (unk5D60 & 0x10)
			for (int i = 0; i < unk5D63; ++i)
				drawTouchingMask();
		else
			for (int i = 0; i < unk5D63; ++i)
				drawTouching();

		GXSetCurrentMtx(GX_PNMTX0);
		GXLoadPosMtxImm(param_1, GX_PNMTX0);
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

	GXSetCurrentMtx(GX_PNMTX0);
	GXLoadPosMtxImm(param_1, GX_PNMTX0);
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

	const TBGCheckData* pTVar4 = *gpMarioGroundPlane;
	f32 fVar1                  = pTVar4->getPlaneDistance();
	const JGeometry::TVec3<f32>& normal = pTVar4->getNormal();
	for (int i = 0; i < mParticleCount; ++i) {
		if ((mParticleFlagSOA[i] & 0xf) == 2
		    && SMS_GetMarioGroundPlane()->isLegal()) {
			GXLoadPosMtxImm(unk2D14[i], GX_PNMTX0);
			SMS_DrawShape(unk5D54, 0);
		}
	}
	JGeometry::TVec3<f32> local_bc[8];

	f32 fVar3 = 1.0f / normal.y;

	for (int i = 0; i < 8; ++i) {
		local_bc[i].x = SMS_GetMarioPos().x + JMASSin(i * 0x2000) * 1000.0f;
		local_bc[i].z = SMS_GetMarioPos().z + JMASCos(i * 0x2000) * 1000.0f;
		local_bc[i].y
		    = fVar3
		          * -(normal.z * local_bc[i].z + normal.x * local_bc[i].x
		              + fVar1)
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
	GXSetCurrentMtx(GX_PNMTX0);
	GXLoadPosMtxImm(param_1, GX_PNMTX0);
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
	GXPosition3f32(local_bc[0].x, local_bc[0].y, local_bc[0].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[1].x, local_bc[1].y, local_bc[1].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[2].x, local_bc[2].y, local_bc[2].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[3].x, local_bc[3].y, local_bc[3].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[4].x, local_bc[4].y, local_bc[4].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[5].x, local_bc[5].y, local_bc[5].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[6].x, local_bc[6].y, local_bc[6].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[7].x, local_bc[7].y, local_bc[7].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[0].x, local_bc[0].y, local_bc[0].z);
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
	GXPosition3f32(local_bc[0].x, local_bc[0].y, local_bc[0].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[1].x, local_bc[1].y, local_bc[1].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[2].x, local_bc[2].y, local_bc[2].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[3].x, local_bc[3].y, local_bc[3].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[4].x, local_bc[4].y, local_bc[4].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[5].x, local_bc[5].y, local_bc[5].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[6].x, local_bc[6].y, local_bc[6].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[7].x, local_bc[7].y, local_bc[7].z);
	GXColor4u8(0, 0, 0, 0);
	GXPosition3f32(local_bc[0].x, local_bc[0].y, local_bc[0].z);
	GXColor4u8(0, 0, 0, 0);
	GXEnd();

	GXSetColorUpdate(GX_TRUE);
	GXSetAlphaUpdate(GX_FALSE);
	GXSetDstAlpha(GX_FALSE, 0);
}

void init_sphere_glist() { }

extern "C" void ReInitializeGX();

extern "C" void showGPR__12JUTExceptionFP9OSContext();

static void* tmp_data[] ATTRIBUTE_ALIGN(32) = {
	(void*)0x98001600, (void*)0x00000100, (void*)0x0B000C00, (void*)0x20002100,
	(void*)0x35003600, (void*)0x4A004B00, (void*)0x5F006000, (void*)0x74007500,
	(void*)0x89008A00, (void*)0x9E009F00, (void*)0xB300B400, (void*)0xC800C998,
	(void*)0x00160001, (void*)0x0002000C, (void*)0x000D0021, (void*)0x00220036,
	(void*)0x0037004B, (void*)0x004C0060, (void*)0x00610075, (void*)0x0076008A,
	(void*)0x008B009F, (void*)0x00A000B4, (void*)0x00B500C9, (void*)0x00CA9800,
	(void*)0x16000200, (void*)0x03000D00, (void*)0x0E002200, (void*)0x23003700,
	(void*)0x38004C00, (void*)0x4D006100, (void*)0x62007600, (void*)0x77008B00,
	(void*)0x8C00A000, (void*)0xA100B500, (void*)0xB600CA00, (void*)0xCB980016,
	(void*)0x00030004, (void*)0x000E000F, (void*)0x00230024, (void*)0x00380039,
	(void*)0x004D004E, (void*)0x00620063, (void*)0x00770078, (void*)0x008C008D,
	(void*)0x00A100A2, (void*)0x00B600B7, (void*)0x00CB00CC, (void*)0x98001600,
	(void*)0x04000500, (void*)0x0F001000, (void*)0x24002500, (void*)0x39003A00,
	(void*)0x4E004F00, (void*)0x63006400, (void*)0x78007900, (void*)0x8D008E00,
	(void*)0xA200A300, (void*)0xB700B800, (void*)0xCC00CD98, (void*)0x00160005,
	(void*)0x00060010, (void*)0x00110025, (void*)0x0026003A, (void*)0x003B004F,
	(void*)0x00500064, (void*)0x00650079, (void*)0x007A008E, (void*)0x008F00A3,
	(void*)0x00A400B8, (void*)0x00B900CD, (void*)0x00CE9800, (void*)0x16000600,
	(void*)0x07001100, (void*)0x12002600, (void*)0x27003B00, (void*)0x3C005000,
	(void*)0x51006500, (void*)0x66007A00, (void*)0x7B008F00, (void*)0x9000A400,
	(void*)0xA500B900, (void*)0xBA00CE00, (void*)0xCF980016, (void*)0x00070008,
	(void*)0x00120013, (void*)0x00270028, (void*)0x003C003D, (void*)0x00510052,
	(void*)0x00660067, (void*)0x007B007C, (void*)0x00900091, (void*)0x00A500A6,
	(void*)0x00BA00BB, (void*)0x00CF00D0, (void*)0x98001600, (void*)0x08000900,
	(void*)0x13001400, (void*)0x28002900, (void*)0x3D003E00, (void*)0x52005300,
	(void*)0x67006800, (void*)0x7C007D00, (void*)0x91009200, (void*)0xA600A700,
	(void*)0xBB00BC00, (void*)0xD000D198, (void*)0x00160009, (void*)0x000A0014,
	(void*)0x00150029, (void*)0x002A003E, (void*)0x003F0053, (void*)0x00540068,
	(void*)0x0069007D, (void*)0x007E0092, (void*)0x009300A7, (void*)0x00A800BC,
	(void*)0x00BD00D1, (void*)0x00D29800, (void*)0x16001500, (void*)0x0A001600,
	(void*)0x09001700, (void*)0x08001800, (void*)0x07001900, (void*)0x06001A00,
	(void*)0x05001B00, (void*)0x04001C00, (void*)0x03001D00, (void*)0x02001E00,
	(void*)0x01001F00, (void*)0x00980016, (void*)0x002A0015, (void*)0x002B0016,
	(void*)0x002C0017, (void*)0x002D0018, (void*)0x002E0019, (void*)0x002F001A,
	(void*)0x0030001B, (void*)0x0031001C, (void*)0x0032001D, (void*)0x0033001E,
	(void*)0x0034001F, (void*)0x98001600, (void*)0x3F002A00, (void*)0x40002B00,
	(void*)0x41002C00, (void*)0x42002D00, (void*)0x43002E00, (void*)0x44002F00,
	(void*)0x45003000, (void*)0x46003100, (void*)0x47003200, (void*)0x48003300,
	(void*)0x49003498, (void*)0x00160054, (void*)0x003F0055, (void*)0x00400056,
	(void*)0x00410057, (void*)0x00420058, (void*)0x00430059, (void*)0x0044005A,
	(void*)0x0045005B, (void*)0x0046005C, (void*)0x0047005D, (void*)0x0048005E,
	(void*)0x00499800, (void*)0x16006900, (void*)0x54006A00, (void*)0x55006B00,
	(void*)0x56006C00, (void*)0x57006D00, (void*)0x58006E00, (void*)0x59006F00,
	(void*)0x5A007000, (void*)0x5B007100, (void*)0x5C007200, (void*)0x5D007300,
	(void*)0x5E980016, (void*)0x007E0069, (void*)0x007F006A, (void*)0x0080006B,
	(void*)0x0081006C, (void*)0x0082006D, (void*)0x0083006E, (void*)0x0084006F,
	(void*)0x00850070, (void*)0x00860071, (void*)0x00870072, (void*)0x00880073,
	(void*)0x98001600, (void*)0x93007E00, (void*)0x94007F00, (void*)0x95008000,
	(void*)0x96008100, (void*)0x97008200, (void*)0x98008300, (void*)0x99008400,
	(void*)0x9A008500, (void*)0x9B008600, (void*)0x9C008700, (void*)0x9D008898,
	(void*)0x001600A8, (void*)0x009300A9, (void*)0x009400AA, (void*)0x009500AB,
	(void*)0x009600AC, (void*)0x009700AD, (void*)0x009800AE, (void*)0x009900AF,
	(void*)0x009A00B0, (void*)0x009B00B1, (void*)0x009C00B2, (void*)0x009D9800,
	(void*)0x1600BD00, (void*)0xA800BE00, (void*)0xA900BF00, (void*)0xAA00C000,
	(void*)0xAB00C100, (void*)0xAC00C200, (void*)0xAD00C300, (void*)0xAE00C400,
	(void*)0xAF00C500, (void*)0xB000C600, (void*)0xB100C700, (void*)0xB2980016,
	(void*)0x00D200BD, (void*)0x00D300BE, (void*)0x00D400BF, (void*)0x00D500C0,
	(void*)0x00D600C1, (void*)0x00D700C2, (void*)0x00D800C3, (void*)0x00D900C4,
	(void*)0x00DA00C5, (void*)0x00DB00C6, (void*)0x00DC00C7, (void*)0x98001601,
	(void*)0x7E019101, (void*)0x7D019001, (void*)0x7C018F01, (void*)0x7B018E01,
	(void*)0x7A018D01, (void*)0x79018C01, (void*)0x78018B01, (void*)0x77018A01,
	(void*)0x76018901, (void*)0x75018800, (void*)0x0B000098, (void*)0x0016016B,
	(void*)0x017E016A, (void*)0x017D0169, (void*)0x017C0168, (void*)0x017B0167,
	(void*)0x017A0166, (void*)0x01790165, (void*)0x01780164, (void*)0x01770163,
	(void*)0x01760162, (void*)0x01750020, (void*)0x000B9800, (void*)0x16015801,
	(void*)0x6B015701, (void*)0x6A015601, (void*)0x69015501, (void*)0x68015401,
	(void*)0x67015301, (void*)0x66015201, (void*)0x65015101, (void*)0x64015001,
	(void*)0x63014F01, (void*)0x62003500, (void*)0x20980016, (void*)0x01450158,
	(void*)0x01440157, (void*)0x01430156, (void*)0x01420155, (void*)0x01410154,
	(void*)0x01400153, (void*)0x013F0152, (void*)0x013E0151, (void*)0x013D0150,
	(void*)0x013C014F, (void*)0x004A0035, (void*)0x98001601, (void*)0x32014501,
	(void*)0x31014401, (void*)0x30014301, (void*)0x2F014201, (void*)0x2E014101,
	(void*)0x2D014001, (void*)0x2C013F01, (void*)0x2B013E01, (void*)0x2A013D01,
	(void*)0x29013C00, (void*)0x5F004A98, (void*)0x0016011F, (void*)0x0132011E,
	(void*)0x0131011D, (void*)0x0130011C, (void*)0x012F011B, (void*)0x012E011A,
	(void*)0x012D0119, (void*)0x012C0118, (void*)0x012B0117, (void*)0x012A0116,
	(void*)0x01290074, (void*)0x005F9800, (void*)0x16010C01, (void*)0x1F010B01,
	(void*)0x1E010A01, (void*)0x1D010901, (void*)0x1C010801, (void*)0x1B010701,
	(void*)0x1A010601, (void*)0x19010501, (void*)0x18010401, (void*)0x17010301,
	(void*)0x16008900, (void*)0x74980016, (void*)0x00F9010C, (void*)0x00F8010B,
	(void*)0x00F7010A, (void*)0x00F60109, (void*)0x00F50108, (void*)0x00F40107,
	(void*)0x00F30106, (void*)0x00F20105, (void*)0x00F10104, (void*)0x00F00103,
	(void*)0x009E0089, (void*)0x98001600, (void*)0xE600F900, (void*)0xE500F800,
	(void*)0xE400F700, (void*)0xE300F600, (void*)0xE200F500, (void*)0xE100F400,
	(void*)0xE000F300, (void*)0xDF00F200, (void*)0xDE00F100, (void*)0xDD00F000,
	(void*)0xB3009E98, (void*)0x001600D2, (void*)0x00E600D1, (void*)0x00E500D0,
	(void*)0x00E400CF, (void*)0x00E300CE, (void*)0x00E200CD, (void*)0x00E100CC,
	(void*)0x00E000CB, (void*)0x00DF00CA, (void*)0x00DE00C9, (void*)0x00DD00C8,
	(void*)0x00B39800, (void*)0x16000001, (void*)0x88001F01, (void*)0x87003401,
	(void*)0x74004901, (void*)0x61005E01, (void*)0x4E007301, (void*)0x3B008801,
	(void*)0x28009D01, (void*)0x1500B201, (void*)0x0200C700, (void*)0xEF00DC00,
	(void*)0xDB980016, (void*)0x01880189, (void*)0x01870186, (void*)0x01740173,
	(void*)0x01610160, (void*)0x014E014D, (void*)0x013B013A, (void*)0x01280127,
	(void*)0x01150114, (void*)0x01020101, (void*)0x00EF00EE, (void*)0x00DB00DA,
	(void*)0x98001601, (void*)0x89018A01, (void*)0x86018501, (void*)0x73017201,
	(void*)0x60015F01, (void*)0x4D014C01, (void*)0x3A013901, (void*)0x27012601,
	(void*)0x14011301, (void*)0x01010000, (void*)0xEE00ED00, (void*)0xDA00D998,
	(void*)0x0016018A, (void*)0x018B0185, (void*)0x01840172, (void*)0x0171015F,
	(void*)0x015E014C, (void*)0x014B0139, (void*)0x01380126, (void*)0x01250113,
	(void*)0x01120100, (void*)0x00FF00ED, (void*)0x00EC00D9, (void*)0x00D89800,
	(void*)0x16018B01, (void*)0x8C018401, (void*)0x83017101, (void*)0x70015E01,
	(void*)0x5D014B01, (void*)0x4A013801, (void*)0x37012501, (void*)0x24011201,
	(void*)0x1100FF00, (void*)0xFE00EC00, (void*)0xEB00D800, (void*)0xD7980016,
	(void*)0x018C018D, (void*)0x01830182, (void*)0x0170016F, (void*)0x015D015C,
	(void*)0x014A0149, (void*)0x01370136, (void*)0x01240123, (void*)0x01110110,
	(void*)0x00FE00FD, (void*)0x00EB00EA, (void*)0x00D700D6, (void*)0x98001601,
	(void*)0x8D018E01, (void*)0x82018101, (void*)0x6F016E01, (void*)0x5C015B01,
	(void*)0x49014801, (void*)0x36013501, (void*)0x23012201, (void*)0x10010F00,
	(void*)0xFD00FC00, (void*)0xEA00E900, (void*)0xD600D598, (void*)0x0016018E,
	(void*)0x018F0181, (void*)0x0180016E, (void*)0x016D015B, (void*)0x015A0148,
	(void*)0x01470135, (void*)0x01340122, (void*)0x0121010F, (void*)0x010E00FC,
	(void*)0x00FB00E9, (void*)0x00E800D5, (void*)0x00D49800, (void*)0x16018F01,
	(void*)0x90018001, (void*)0x7F016D01, (void*)0x6C015A01, (void*)0x59014701,
	(void*)0x46013401, (void*)0x33012101, (void*)0x20010E01, (void*)0x0D00FB00,
	(void*)0xFA00E800, (void*)0xE700D400, (void*)0xD3980016, (void*)0x01900191,
	(void*)0x017F017E, (void*)0x016C016B, (void*)0x01590158, (void*)0x01460145,
	(void*)0x01330132, (void*)0x0120011F, (void*)0x010D010C, (void*)0x00FA00F9,
	(void*)0x00E700E6, (void*)0x00D300D2, (void*)0x00000000, (void*)0x00000000,
	(void*)0x00008001, (void*)0x00000000, (void*)0x80C9F1DD, (void*)0x000083D3,
	(void*)0xE0F50000, (void*)0x8A5ACD94, (void*)0x00009580, (void*)0xB9000000,
	(void*)0xA57EA57E, (void*)0x0000B900, (void*)0x95800000, (void*)0xCD948A5A,
	(void*)0x0000E0F5, (void*)0x83D30000, (void*)0xF1DD80C9, (void*)0x00000000,
	(void*)0x8001F1DD, (void*)0x80C90000, (void*)0xF03F81F5, (void*)0xF03FEE95,
	(void*)0x8613DD2A, (void*)0xED218EC5, (void*)0xC762EC40, (void*)0x9D40B100,
	(void*)0xEC40B100, (void*)0x9D40ED21, (void*)0xC7628EC5, (void*)0xEE95DD2A,
	(void*)0x8613F03F, (void*)0xF03F81F5, (void*)0xF1DD0000, (void*)0x80C90000,
	(void*)0x0E2380C9, (void*)0x0E230000, (void*)0x80C90FC1, (void*)0xF03F81F5,
	(void*)0x116BDD2A, (void*)0x861312DF, (void*)0xC7628EC5, (void*)0x13C0B100,
	(void*)0x9D4013C0, (void*)0x9D40B100, (void*)0x12DF8EC5, (void*)0xC762116B,
	(void*)0x8613DD2A, (void*)0x0FC181F5, (void*)0xF03F0E23, (void*)0x80C90000,
	(void*)0xE0F583D3, (void*)0x0000DD2A, (void*)0x8613EE95, (void*)0xD9688C39,
	(void*)0xD968D679, (void*)0x982EC1B5, (void*)0xD556AAAB, (void*)0xAAABD679,
	(void*)0xC1B5982E, (void*)0xD968D968, (void*)0x8C39DD2A, (void*)0xEE958613,
	(void*)0xE0F50000, (void*)0x83D3F03F, (void*)0x0FC181F5, (void*)0x00001F0B,
	(void*)0x83D30FC1, (void*)0x0FC181F5, (void*)0x1F0B0000, (void*)0x83D322D6,
	(void*)0xEE958613, (void*)0x2698D968, (void*)0x8C392987, (void*)0xC1B5982E,
	(void*)0x2AAAAAAB, (void*)0xAAAB2987, (void*)0x982EC1B5, (void*)0x26988C39,
	(void*)0xD96822D6, (void*)0x8613EE95, (void*)0x1F0B83D3, (void*)0x0000CD94,
	(void*)0x8A5A0000, (void*)0xC7628EC5, (void*)0xED21C1B5, (void*)0x982ED679,
	(void*)0xBE26A832, (void*)0xBE26BE26, (void*)0xBE26A832, (void*)0xC1B5D679,
	(void*)0x982EC762, (void*)0xED218EC5, (void*)0xCD940000, (void*)0x8A5ADD2A,
	(void*)0x116B8613, (void*)0xEE9522D6, (void*)0x86130000, (void*)0x326C8A5A,
	(void*)0x116B22D6, (void*)0x861322D6, (void*)0x116B8613, (void*)0x326C0000,
	(void*)0x8A5A389E, (void*)0xED218EC5, (void*)0x3E4BD679, (void*)0x982E41DA,
	(void*)0xBE26A832, (void*)0x41DAA832, (void*)0xBE263E4B, (void*)0x982ED679,
	(void*)0x389E8EC5, (void*)0xED21326C, (void*)0x8A5A0000, (void*)0xB9009580,
	(void*)0x0000B100, (void*)0x9D40EC40, (void*)0xAAABAAAB, (void*)0xD556A832,
	(void*)0xBE26BE26, (void*)0xAAABD556, (void*)0xAAABB100, (void*)0xEC409D40,
	(void*)0xB9000000, (void*)0x9580C762, (void*)0x12DF8EC5, (void*)0xD9682698,
	(void*)0x8C39ED21, (void*)0x389E8EC5, (void*)0x00004700, (void*)0x958012DF,
	(void*)0x389E8EC5, (void*)0x26982698, (void*)0x8C39389E, (void*)0x12DF8EC5,
	(void*)0x47000000, (void*)0x95804F00, (void*)0xEC409D40, (void*)0x5555D556,
	(void*)0xAAAB57CE, (void*)0xBE26BE26, (void*)0x5555AAAB, (void*)0xD5564F00,
	(void*)0x9D40EC40, (void*)0x47009580, (void*)0x0000A57E, (void*)0xA57E0000,
	(void*)0x9D40B100, (void*)0xEC40982E, (void*)0xC1B5D679, (void*)0x982ED679,
	(void*)0xC1B59D40, (void*)0xEC40B100, (void*)0xA57E0000, (void*)0xA57EB100,
	(void*)0x13C09D40, (void*)0xC1B52987, (void*)0x982ED679, (void*)0x3E4B982E,
	(void*)0xEC404F00, (void*)0x9D400000, (void*)0x5A82A57E, (void*)0x13C04F00,
	(void*)0x9D402987, (void*)0x3E4B982E, (void*)0x3E4B2987, (void*)0x982E4F00,
	(void*)0x13C09D40, (void*)0x5A820000, (void*)0xA57E62C0, (void*)0xEC40B100,
	(void*)0x67D2D679, (void*)0xC1B567D2, (void*)0xC1B5D679, (void*)0x62C0B100,
	(void*)0xEC405A82, (void*)0xA57E0000, (void*)0x9580B900, (void*)0x00008EC5,
	(void*)0xC762ED21, (void*)0x8C39D968, (void*)0xD9688EC5, (void*)0xED21C762,
	(void*)0x95800000, (void*)0xB9009D40, (void*)0x13C0B100, (void*)0xAAAB2AAA,
	(void*)0xAAABBE26, (void*)0x41DAA832, (void*)0xD5565555, (void*)0xAAABEC40,
	(void*)0x62C0B100, (void*)0x00006A80, (void*)0xB90013C0, (void*)0x62C0B100,
	(void*)0x2AAA5555, (void*)0xAAAB41DA, (void*)0x41DAA832, (void*)0x55552AAA,
	(void*)0xAAAB62C0, (void*)0x13C0B100, (void*)0x6A800000, (void*)0xB900713B,
	(void*)0xED21C762, (void*)0x73C7D968, (void*)0xD968713B, (void*)0xC762ED21,
	(void*)0x6A80B900, (void*)0x00008A5A, (void*)0xCD940000, (void*)0x8613DD2A,
	(void*)0xEE958613, (void*)0xEE95DD2A, (void*)0x8A5A0000, (void*)0xCD948EC5,
	(void*)0x12DFC762, (void*)0x982E2987, (void*)0xC1B5A832, (void*)0x41DABE26,
	(void*)0xBE2657CE, (void*)0xBE26D679, (void*)0x67D2C1B5, (void*)0xED21713B,
	(void*)0xC7620000, (void*)0x75A6CD94, (void*)0x12DF713B, (void*)0xC7622987,
	(void*)0x67D2C1B5, (void*)0x41DA57CE, (void*)0xBE2657CE, (void*)0x41DABE26,
	(void*)0x67D22987, (void*)0xC1B5713B, (void*)0x12DFC762, (void*)0x75A60000,
	(void*)0xCD9479ED, (void*)0xEE95DD2A, (void*)0x79EDDD2A, (void*)0xEE9575A6,
	(void*)0xCD940000, (void*)0x83D3E0F5, (void*)0x000081F5, (void*)0xF03FF03F,
	(void*)0x83D30000, (void*)0xE0F58613, (void*)0x116BDD2A, (void*)0x8C392698,
	(void*)0xD968982E, (void*)0x3E4BD679, (void*)0xAAAB5555, (void*)0xD556C1B5,
	(void*)0x67D2D679, (void*)0xD96873C7, (void*)0xD968EE95, (void*)0x79EDDD2A,
	(void*)0x00007C2D, (void*)0xE0F5116B, (void*)0x79EDDD2A, (void*)0x269873C7,
	(void*)0xD9683E4B, (void*)0x67D2D679, (void*)0x55555555, (void*)0xD55667D2,
	(void*)0x3E4BD679, (void*)0x73C72698, (void*)0xD96879ED, (void*)0x116BDD2A,
	(void*)0x7C2D0000, (void*)0xE0F57E0B, (void*)0xF03FF03F, (void*)0x7C2DE0F5,
	(void*)0x000080C9, (void*)0xF1DD0000, (void*)0x80C90000, (void*)0xF1DD81F5,
	(void*)0x0FC1F03F, (void*)0x861322D6, (void*)0xEE958EC5, (void*)0x389EED21,
	(void*)0x9D404F00, (void*)0xEC40B100, (void*)0x62C0EC40, (void*)0xC762713B,
	(void*)0xED21DD2A, (void*)0x79EDEE95, (void*)0xF03F7E0B, (void*)0xF03F0000,
	(void*)0x7F37F1DD, (void*)0x0FC17E0B, (void*)0xF03F22D6, (void*)0x79EDEE95,
	(void*)0x389E713B, (void*)0xED214F00, (void*)0x62C0EC40, (void*)0x62C04F00,
	(void*)0xEC40713B, (void*)0x389EED21, (void*)0x79ED22D6, (void*)0xEE957E0B,
	(void*)0x0FC1F03F, (void*)0x7F370000, (void*)0xF1DD7F37, (void*)0xF1DD0000,
	(u8*)showGPR__12JUTExceptionFP9OSContext + 0x18, (void*)0x000080C9, (void*)0x0E230000, (void*)0x83D31F0B,
	(void*)0x00008A5A, (void*)0x326C0000, (void*)0x95804700, (void*)0x0000A57E,
	(void*)0x5A820000, (void*)0xB9006A80, (void*)0x0000CD94, (void*)0x75A60000,
	(void*)0xE0F57C2D, (void*)0x0000F1DD, (void*)0x7F370000, (void*)0x00007FFF,
	(void*)0x00000E23, (void*)0x7F370000, (void*)0x1F0B7C2D, (void*)0x0000326C,
	(void*)0x75A60000, (void*)0x47006A80, (void*)0x00005A82, (void*)0x5A820000,
	(void*)0x6A804700, (void*)0x000075A6, (void*)0x326C0000, (void*)0x7C2D1F0B,
	(void*)0x00007F37, (void*)0x0E230000, (void*)0x7FFF0000, (void*)0x000080C9,
	(void*)0x00000E23, (void*)0x81F50FC1, (void*)0x0FC18613, (void*)0x22D6116B,
	(void*)0x8EC5389E, (void*)0x12DF9D40, (void*)0x4F0013C0, (void*)0xB10062C0,
	(void*)0x13C0C762, (void*)0x713B12DF, (void*)0xDD2A79ED, (void*)0x116BF03F,
	(void*)0x7E0B0FC1, (void*)0x00007F37, (void*)0x0E230FC1, (void*)0x7E0B0FC1,
	(void*)0x22D679ED, (void*)0x116B389E, (void*)0x713B12DF, (void*)0x4F0062C0,
	(void*)0x13C062C0, (void*)0x4F0013C0, (void*)0x713B389E, (void*)0x12DF79ED,
	(void*)0x22D6116B, (void*)0x7E0B0FC1, (void*)0x0FC17F37, (void*)0x00000E23,
	(void*)0x81F5F03F, (void*)0x0FC183D3, (void*)0x00001F0B, (void*)0x8613116B,
	(void*)0x22D68C39, (void*)0x26982698, (void*)0x982E3E4B, (void*)0x2987AAAB,
	(void*)0x55552AAA, (void*)0xC1B567D2, (void*)0x2987D968, (void*)0x73C72698,
	(void*)0xEE9579ED, (void*)0x22D60000, (void*)0x7C2D1F0B, (void*)0x116B79ED,
	(void*)0x22D62698, (void*)0x73C72698, (void*)0x3E4B67D2, (void*)0x29875555,
	(void*)0x55552AAA, (void*)0x67D23E4B, (void*)0x298773C7, (void*)0x26982698,
	(void*)0x79ED116B, (void*)0x22D67C2D, (void*)0x00001F0B, (void*)0x7E0BF03F,
	(void*)0x0FC18613, (void*)0xDD2A116B, (void*)0x8613EE95, (void*)0x22D68A5A,
	(void*)0x0000326C, (void*)0x8EC512DF, (void*)0x389E982E, (void*)0x29873E4B,
	(void*)0xA83241DA, (void*)0x41DABE26, (void*)0x57CE41DA, (void*)0xD67967D2,
	(void*)0x3E4BED21, (void*)0x713B389E, (void*)0x000075A6, (void*)0x326C12DF,
	(void*)0x713B389E, (void*)0x298767D2, (void*)0x3E4B41DA, (void*)0x57CE41DA,
	(void*)0x57CE41DA, (void*)0x41DA67D2, (void*)0x29873E4B, (void*)0x713B12DF,
	(void*)0x389E75A6, (void*)0x0000326C, (void*)0x79EDEE95, (void*)0x22D679ED,
	(void*)0xDD2A116B, (void*)0x8EC5C762, (void*)0x12DF8C39, (void*)0xD9682698,
	(void*)0x8EC5ED21, (void*)0x389E9580, (void*)0x00004700, (void*)0x9D4013C0,
	(void*)0x4F00AAAB, (void*)0x2AAA5555, (void*)0xBE2641DA, (void*)0x57CED556,
	(void*)0x55555555, (void*)0xEC4062C0, (void*)0x4F000000, (void*)0x6A804700,
	(void*)0x13C062C0, (void*)0x4F002AAA, (void*)0x55555555, (void*)0x41DA41DA,
	(void*)0x57CE5555, (void*)0x2AAA5555, (void*)0x62C013C0, (void*)0x4F006A80,
	(void*)0x00004700, (void*)0x713BED21, (void*)0x389E73C7, (void*)0xD9682698,
	(void*)0x713BC762, (void*)0x12DF9D40, (void*)0xB10013C0, (void*)0x982EC1B5,
	(void*)0x2987982E, (void*)0xD6793E4B, (void*)0x9D40EC40, (void*)0x4F00A57E,
	(void*)0x00005A82, (void*)0xB10013C0, (void*)0x62C0C1B5, (void*)0x298767D2,
	(void*)0xD6793E4B, (void*)0x67D2EC40, (void*)0x4F0062C0, (void*)0x00005A82,
	(void*)0x5A8213C0, (void*)0x4F0062C0, (void*)0x29873E4B, (void*)0x67D23E4B,
	(void*)0x298767D2, (void*)0x4F0013C0, (void*)0x62C05A82, (void*)0x00005A82,
	(void*)0x62C0EC40, (void*)0x4F0067D2, (void*)0xD6793E4B, (void*)0x67D2C1B5,
	(void*)0x298762C0, (void*)0xB10013C0, (void*)0xB1009D40, (void*)0x13C0AAAB,
	(void*)0xAAAB2AAA, (void*)0xA832BE26, (void*)0x41DAAAAB, (void*)0xD5565555,
	(void*)0xB100EC40, (void*)0x62C0B900, (void*)0x00006A80, (void*)0xC76212DF,
	(void*)0x713BD968, (void*)0x269873C7, (void*)0xED21389E, (void*)0x713B0000,
	(void*)0x47006A80, (void*)0x12DF389E, (void*)0x713B2698, (void*)0x269873C7,
	(void*)0x389E12DF, (void*)0x713B4700, (void*)0x00006A80, (void*)0x4F00EC40,
	(void*)0x62C05555, (void*)0xD5565555, (void*)0x57CEBE26, (void*)0x41DA5555,
	(void*)0xAAAB2AAA, (void*)0x4F009D40, (void*)0x13C0C762, (void*)0x8EC512DF,
	(void*)0xC1B5982E, (void*)0x2987BE26, (void*)0xA83241DA, (void*)0xBE26BE26,
	(void*)0x57CEC1B5, (void*)0xD67967D2, (void*)0xC762ED21, (void*)0x713BCD94,
	(void*)0x000075A6, (void*)0xDD2A116B, (void*)0x79EDEE95, (void*)0x22D679ED,
	(void*)0x0000326C, (void*)0x75A6116B, (void*)0x22D679ED, (void*)0x22D6116B,
	(void*)0x79ED326C, (void*)0x000075A6, (void*)0x389EED21, (void*)0x713B3E4B,
	(void*)0xD67967D2, (void*)0x41DABE26, (void*)0x57CE41DA, (void*)0xA83241DA,
	(void*)0x3E4B982E, (void*)0x2987389E, (void*)0x8EC512DF, (void*)0xDD2A8613,
	(void*)0x116BD968, (void*)0x8C392698, (void*)0xD679982E, (void*)0x3E4BD556,
	(void*)0xAAAB5555, (void*)0xD679C1B5, (void*)0x67D2D968, (void*)0xD96873C7,
	(void*)0xDD2AEE95, (void*)0x79EDE0F5, (void*)0x00007C2D, (void*)0xF03F0FC1,
	(void*)0x7E0B0000, (void*)0x1F0B7C2D, (void*)0x0FC10FC1, (void*)0x7E0B1F0B,
	(void*)0x00007C2D, (void*)0x22D6EE95, (void*)0x79ED2698, (void*)0xD96873C7,
	(void*)0x2987C1B5, (void*)0x67D22AAA, (void*)0xAAAB5555, (void*)0x2987982E,
	(void*)0x3E4B2698, (void*)0x8C392698, (void*)0x22D68613, (void*)0x116BF03F,
	(void*)0x81F50FC1, (void*)0xEE958613, (void*)0x22D6ED21, (void*)0x8EC5389E,
	(void*)0xEC409D40, (void*)0x4F00EC40, (void*)0xB10062C0, (void*)0xED21C762,
	(void*)0x713BEE95, (void*)0xDD2A79ED, (void*)0xF03FF03F, (void*)0x7E0BF1DD,
	(void*)0x00007F37, (void*)0x00000E23, (void*)0x7F370E23, (void*)0x00007F37,
	(void*)0x0FC1F03F, (void*)0x7E0B116B, (void*)0xDD2A79ED, (void*)0x12DFC762,
	(void*)0x713B13C0, (void*)0xB10062C0, (void*)0x13C09D40, (void*)0x4F0012DF,
	(void*)0x8EC5389E, (void*)0x116B8613, (void*)0x22D60FC1, (void*)0x81F50FC1,
	(void*)0x000080C9, (void*)0x0E230000, (void*)0x83D31F0B, (void*)0x00008A5A,
	(void*)0x326C0000, (void*)0x95804700, (void*)0x0000A57E, (void*)0x5A820000,
	(void*)0xB9006A80, (void*)0x0000CD94, (void*)0x75A60000, (void*)0xE0F57C2D,
	(void*)0x0000F1DD, (void*)0x7F370000, (void*)0x00007FFF,
};
static u8* sphere_glist_p;
static u8* sphere_pos_t;

void TModelWaterManager::drawShineShadowVolume(MtxPtr param_1)
{

	if (gpMarDirector->getCurrentMap() == 1) {
		static bool initialized = false;
		if (!initialized) {
			sphere_glist_p = (u8*)tmp_data;
			initialized    = 1;
			sphere_pos_t   = sphere_glist_p + 0x760;
		}

		f32 f30 = (((unk5E0C + unk5E40) - unk5E0C) / f32(unk5E44 - 1));
		f32 f31 = unk5E0C;

		GXColor local_2C;
		int r27 = unk5E45;

		ReInitializeGX();

		Mtx afStack_f8;
		MTXIdentity(afStack_f8);

		JGeometry::TVec3<f32> local_pos(0.0f, 3600.0f, -7458.0f);
		Mtx local_c8;
		local_c8[2][1] = 0.0;
		local_c8[2][0] = 0.0;
		local_c8[1][2] = 0.0;
		local_c8[1][0] = 0.0;
		local_c8[0][2] = 0.0;
		local_c8[0][1] = 0.0;
		local_c8[0][3] = local_pos.x;
		local_c8[1][3] = local_pos.y;
		local_c8[2][3] = local_pos.z;

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
		GXLoadPosMtxImm(afStack_f8, GX_PNMTX0);

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
			GXLoadPosMtxImm(afStack_98, GX_PNMTX0);
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
		GXLoadPosMtxImm(afStack_f8, GX_PNMTX0);

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
	GXSetCurrentMtx(GX_PNMTX0);
	GXLoadPosMtxImm(afStack_3c, GX_PNMTX0);
	GXLoadNrmMtxImm(afStack_3c, GX_PNMTX0);
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
	GXColor local_color;
	local_color = (GXColor) { 0, 0, 0, unk5D65 };
	GXSetTevColor(GX_TEVREG0, local_color);
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

void TModelWaterManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	MtxPtr r29 = graphics->mViewMtx;

	if (cue & CUE_MOVE) {
		move();
		calcWorldMinMax();
		unk5E00 += 1;
	}

	if (cue & CUE_CALC_VIEW) {
		if (unk5D60 & 0x80)
			TTimeRec::startTimer(0xFF, 0x00, 0x00, 0xFE);

		calcDrawVtx(graphics->mViewMtx);
		calcVMAll(graphics->mViewMtx);

		if (unk5D60 & 0x80)
			TTimeRec::endTimer();
	}

	if (cue & CUE_DRAW) {
		if (unk5D60 & 0x80)
			TTimeRec::snapGxTimeStart(0xFF, 0x00, 0x00, 0xFD);

		drawSilhouette(r29);
		drawWaterVolume(r29);

		if ((unk5D60 & 0x1) && !gpMirrorModelManager->isUnk18Present())
			drawMirror(r29);

		MTXCopy(r29, unk5E10);
		if ((unk5D60 & 0x100) && !(unk5D60 & 0x200))
			drawShineShadowVolume(graphics->mViewMtx);

		if (unk5D60 & 0x80)
			TTimeRec::snapGxTimeEnd();
	}

	if (cue & CUE_DRAW_INIT) {
		if (unk5D60 & 0x80)
			TTimeRec::snapGxTimeStart(0xFF, 0x00, 0x00, 0xFC);

		drawRefracAndSpec();

		if ((unk5D60 & 0x100) && (unk5D60 & 0x200))
			drawShineShadowVolume(graphics->mViewMtx);

		if (unk5D60 & 0x80)
			TTimeRec::snapGxTimeEnd();
	}
}

#include <Player/ModelWaterManager.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/SplashManager.hpp>
#include <Camera/Camera.hpp>
#include <Map/MapData.hpp>
#include <Map/PollutionManager.hpp>
#include <Map/MapMirror.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <System/StageUtil.hpp>
#include <MSound/MSound.hpp>
#include <MarioUtil/DLUtil.hpp>
#include <MarioUtil/ScreenUtil.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
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
	unk10 = SLOT_NUM;
	unk12 = 0;

	for (int i = 0; i < SLOT_NUM; ++i) {
		unk14[i]  = 0;
		unk414[i] = 0;
		unk614[i] = 0;
		unk814[i].set(0.0f, 0.0f, 0.0f);
		unk1414[i].set(0.0f, 0.0f, 0.0f);
		mPerParticleType[i] = 0;
		unk2914[i]          = nullptr;
		MTXIdentity(unk2D14[i]);
		unk2514[i] = 0;
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

	unk5D68[0]    = 256.0f;
	unk5D68[1]    = 256.0f;
	unk5D68[2]    = 0.0f;
	unk5D68[3]    = 0.0f;
	unk5D68[4]    = 0.0f;
	unk5D68[5]    = 0.0f;
	unk5D68[6]    = 0.0f;
	unk5D68[7]    = 0.0f;
	unk5D68[8]    = 0.5f;
	unk5D68[9]    = 0.35f;
	unk5D68[10]   = 0.5f;
	unk5D68[0xb]  = 10000.0f;
	unk5D68[0xc]  = 0.2f;
	unk5D68[0xd]  = 0.0f;
	unk5D68[0xe]  = 2.5f;
	unk5D68[0xf]  = 1.0f;
	unk5D68[0x10] = 0.3f;
	unk5D68[0x11] = 0.5f;
	unk5D68[0x12] = 0.8f;
	unk5D68[0x13] = 0.3f;
	unk5D68[0x14] = 0.08f;

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

void TModelWaterManager::getWPGravity(int) const { }

void TModelWaterManager::getWaterAlpha() const { }

bool TModelWaterManager::askHitWaterParticleOnGround(
    const JGeometry::TVec3<f32>& param_1)
{
	for (int i = 0; i < unk12; ++i) {
		if ((unk414[i] & 0xF) == 2
		    && abs(param_1.x - unk814[i].x) + abs(param_1.y - unk814[i].y)
		               + abs(param_1.z - unk814[i].z)
		           < 300.0f)
			return true;
	}

	return false;
}

#pragma dont_inline on
void TModelWaterManager::makeEmit(const TWaterEmitInfo&) { }
#pragma dont_inline off

int TModelWaterManager::emitRequest(const TWaterEmitInfo& param_1)
{
	int iVar1 = param_1.mNum.get();
	if (iVar1 == 0)
		return 0;

	while (iVar1 > 0) {
		if (unk12 >= unk10)
			break;
		makeEmit(param_1);
		--iVar1;
		unk12 += 1;
	}

	return (param_1.mNum.get() - iVar1) & 0xff;
}

void TModelWaterManager::splashSound(const JGeometry::TVec3<f32>&, f32) const {
}

void TModelWaterManager::splashGround(int) { }

void TModelWaterManager::touchingExec(int) { }

void TModelWaterManager::splashWall(int) { }

void TModelWaterManager::splashWallPosSize(const JGeometry::TVec3<f32>&, f32) {
}

void TModelWaterManager::getPlaneFriction(const TBGCheckData*) { }

void TModelWaterManager::getPlaneFall(const TBGCheckData*) { }

void TModelWaterManager::getPlaneVanishSpeed(const TBGCheckData*) { }

bool TModelWaterManager::askDoWaterHitCheck()
{
	return unk5E00 % unk5E04 == 0 ? true : false;
}

void TModelWaterManager::wind(const JGeometry::TVec3<f32>& param_1)
{
	for (int i = 0; i < unk12; ++i)
		if ((unk414[i] & 0xf) == 1)
			unk1414[i] += param_1;
}

void TModelWaterManager::garbageCollect()
{
	int nextFreeSlot = 0;
	for (int i = 0; i < unk12; ++i) {
		if (unk14[i] <= 0.0f)
			continue;

		// TODO: SoA copy inline?
		if (i != nextFreeSlot) {
			unk14[nextFreeSlot]            = unk14[i];
			unk414[nextFreeSlot]           = unk414[i];
			unk614[nextFreeSlot]           = unk614[i];
			unk814[nextFreeSlot]           = unk814[i];
			unk1414[nextFreeSlot]          = unk1414[i];
			unk2014[nextFreeSlot]          = unk2014[i];
			mPerParticleType[nextFreeSlot] = mPerParticleType[i];
			unk2514[nextFreeSlot]          = unk2514[i];
			unk2914[nextFreeSlot]          = unk2914[i];
		}

		++nextFreeSlot;
	}

	unk12 = nextFreeSlot;
}

static inline f32 randf() { return rand() * (1.f / (RAND_MAX + 1)); }

void TModelWaterManager::move()
{
	f32 fVar1 = unk5E08;
	for (int i = 0; i < unk12; ++i) {
		if (unk2514[i] == 0) {
			JGeometry::TVec3<f32> thing = SMS_GetMarioPos();
			thing -= unk814[0];
			if (thing.x * thing.x + thing.z * thing.z <= fVar1 * fVar1) {
				// TODO: stuff
				switch (unk414[i] & 0xf) {
				case 1:
					f32 fVar2;
					if (SMS_isDivingMap()) {
						fVar2 = mWaterParticleTypes[mPerParticleType[i]]
						            ->mGravity.get();
					} else {
						fVar2 = unk5D68[20];
					}
					unk1414[i].y += fVar2;
					unk814[i].y += unk1414[i].y;
					break;
				case 2:
					break;
				case 3:
					break;
				}
			} else {
				unk14[i] = 0.0f;
			}
		} else {
			mStaticHitActor.mPosition = unk814[i];
			mStaticHitActor.unk68     = i;

			if (!unk2514[i]->receiveMessage(&mStaticHitActor, 0xF))
				continue;

			gpMSound->startSoundSet(0x6800, nullptr, 0, unk2014[i], 0, 0, 4);
			if (randf() < unk5D68[16])
				gpSplashManager->newSplash(unk814[i], 5.0f);

			unk14[i] = 0.0f;
		}
	}

	for (int i = 0; i < unk12; ++i) {
		for (int j = 0; j < unk12; ++j) {
			switch (unk414[i] & 0xf) {
			case 1:
				unk14[i] -= 1.0f;
				break;
			case 2:
				break;
			case 3:
				if (unk5D68[1] < unk14[i])
					unk14[i] = unk5D68[1];
				if (unk2914[i]->isWaterSlip()) {
					//
				}
				gpPollution->clean(
				    unk814[i].x, unk814[i].y, unk814[i].z,
				    mWaterParticleTypes[mPerParticleType[j]]->mCleanSize.get());
				if ((unk5D60 & 0x40) && j > 0) { }
				break;
			}
		}
	}
}

void TModelWaterManager::calcWorldMinMax()
{
	if (unk12 == 0) {
		JGeometry::TVec3<f32> marioPos = SMS_GetMarioPos();

		unk5D68[2] = marioPos.x;
		unk5D68[3] = marioPos.y;
		unk5D68[4] = marioPos.z;

		unk5D68[5] = marioPos.x;
		unk5D68[6] = marioPos.y;
		unk5D68[7] = marioPos.z;

		unk5D68[2] -= 1.0f;
		unk5D68[3] -= 1.0f;
		unk5D68[4] -= 1.0f;

		unk5D68[5] += 1.0f;
		unk5D68[6] += 1.0f;
		unk5D68[7] += 1.0f;
		return;
	}

	JGeometry::TVec3<f32> fVar789 = unk814[0];
	fVar789.x -= 1.0f;
	fVar789.y -= 1.0f;
	fVar789.z -= 1.0f;
	JGeometry::TVec3<f32> fVar123 = unk814[0];
	fVar789.x += 1.0f;
	fVar789.y += 1.0f;
	fVar789.z += 1.0f;
	for (int i = 0; i < unk12; ++i) {
		fVar789.setMax(unk814[i]);
		fVar123.setMin(unk814[i]);
	}

	unk5D68[2] = fVar789.x - 200.0f;
	unk5D68[3] = fVar789.y - 200.0f;
	unk5D68[4] = fVar789.z - 200.0f;

	unk5D68[5] = fVar123.x + 200.0f;
	unk5D68[6] = fVar123.y + 200.0f;
	unk5D68[7] = fVar123.z + 200.0f;
}

void TModelWaterManager::calcDrawVtx(MtxPtr) { }

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

void TModelWaterManager::calcVMMtxWall(MtxPtr param_1, f32 param_2,
                                       const JGeometry::TVec3<f32>& param_3,
                                       const JGeometry::TVec3<f32>& param_4,
                                       MtxPtr param_5)
{
	// TODO: matching this is ewwwwwwwwwwwwwwwwww

	f32 fVar7  = param_2 * param_4.x;
	f32 fVar4  = param_3.y;
	f32 fVar8  = param_2 * param_4.z;
	f32 fVar11 = -fVar7;
	f32 fVar10 = param_4.x * 2.0 + param_3.x;
	f32 fVar9  = param_4.z * 2.0 + param_3.z;

	{
		f32 fVar3     = param_1[0][1];
		f32 fVar5     = param_1[0][2];
		f32 fVar6     = param_1[0][0];
		f32 fVar1     = param_1[0][3];
		(*param_5)[0] = fVar6 * fVar8 + fVar5 * fVar11;
		(*param_5)[1] = fVar3 * param_2;
		(*param_5)[2] = fVar6 * fVar7 + fVar5 * fVar8;
		(*param_5)[3] = fVar1 + fVar5 * fVar9 + fVar6 * fVar10 + fVar3 * fVar4;
	}
	{
		f32 fVar1     = param_1[1][2];
		f32 fVar2     = param_1[1][1];
		f32 fVar3     = param_1[1][0];
		f32 fVar5     = param_1[1][3];
		param_5[1][0] = fVar3 * fVar8 + fVar1 * fVar11;
		param_5[1][1] = fVar2 * param_2;
		param_5[1][2] = fVar3 * fVar7 + fVar1 * fVar8;
		param_5[1][3] = fVar5 + fVar1 * fVar9 + fVar3 * fVar10 + fVar2 * fVar4;
	}
	{
		f32 fVar1     = param_1[2][2];
		f32 fVar2     = param_1[2][1];
		f32 fVar3     = param_1[2][0];
		f32 fVar5     = param_1[2][3];
		param_5[2][0] = fVar3 * fVar8 + fVar1 * fVar11;
		param_5[2][1] = fVar2 * param_2;
		param_5[2][2] = fVar3 * fVar7 + fVar1 * fVar8;
		param_5[2][3] = fVar5 + fVar1 * fVar9 + fVar3 * fVar10 + fVar2 * fVar4;
	}
}

void TModelWaterManager::calcVMAll(MtxPtr param_1)
{
	for (int i = 0; i < unk12; ++i) {
		if ((unk414[i] & 0xf) != 2)
			continue;

		f32 f
		    = unk2014[i]
		      * ((1.0f - unk5D68[13]) * (unk14[i] / unk5D68[0]) + unk5D68[13]);
		calcVMMtxGround(param_1, f, unk814[i], unk2914[i]->getNormal(),
		                unk2D14[i]);
	}

	for (int i = 0; i < unk12; ++i) {
		if ((unk414[i] & 0xf) != 3)
			continue;

		f32 f
		    = unk2014[i]
		      * ((1.0f - unk5D68[13]) * (unk14[i] / unk5D68[1]) + unk5D68[13]);
		matan(unk2914[i]->getNormal().z, unk2914[i]->getNormal().x);
		calcVMMtxWall(param_1, f, unk814[i], unk2914[i]->getNormal(),
		              unk2D14[i]);
	}
}

void TModelWaterManager::drawTouching()
{
	SMS_SettingDrawShape(unk5D48, 0);
	for (int i = 0; i < unk12; ++i) {
		if ((unk414[i] & 0xf) == 2 && -unk5D2C < unk2D14[i][2][3]) {
			GXLoadPosMtxImm(unk2D14[i], 0);
			SMS_DrawShape(unk5D48, 0);
		}
	}

	SMS_SettingDrawShape(unk5D4C, 0);
	for (int i = 0; i < unk12; ++i) {
		if ((unk414[i] & 0xf) == 3 && -unk5D2C < unk2D14[i][2][3]) {
			GXLoadPosMtxImm(unk2D14[i], 0);
			SMS_DrawShape(unk5D4C, 0);
		}
	}
}

void TModelWaterManager::drawTouchingMask()
{
	SMS_SettingDrawShape(unk5D50, 0);
	for (int iVar2 = 0; iVar2 < unk12; iVar2 = iVar2 + 1) {
		if ((unk414[iVar2] & 0xf) == 2 || (unk414[iVar2] & 0xf) == 3) {
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
	for (int i = 0; i < unk12; ++i) {
		if ((unk414[i] & 0xf) == 2) {
			GXLoadPosMtxImm(unk2D14[i], 0);
			SMS_DrawShape(unk5D54, 0);
		}
	}

	SMS_SettingDrawShape(unk5D58, 0);
	for (int i = 0; i < unk12; ++i) {
		if ((unk414[i] & 0xf) == 2) {
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
	for (int i = 0; i < unk12; ++i) {
		if ((unk414[i] & 0xf) == 2 && -unk5D2C < unk2D14[i][2][3]) {
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
	for (int i = 0; i < unk12; ++i) {
		if ((unk414[i] & 0xf) == 3 && -unk5D2C < unk2D14[i][2][3]) {
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

	const TBGCheckData* pTVar4 = *gpMarioGroundPlane;
	f32 fVar1                  = pTVar4->unk40;
	for (int i = 0; i < unk12; ++i) {
		if ((unk414[i] & 0xf) == 2
		    && ((*gpMarioGroundPlane)->checkFlag2(0x10) == TRUE ? false
		                                                        : true)) {
			GXLoadPosMtxImm(unk2D14[i], 0);
			SMS_DrawShape(unk5D54, 0);
		}
	}
	JGeometry::TVec3<f32> local_bc[4][2];

	f32 fVar3 = 1.0 / (pTVar4->mNormal).y;

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

void TModelWaterManager::perform(u32 param_1, JDrama::TGraphics* param_2) { }

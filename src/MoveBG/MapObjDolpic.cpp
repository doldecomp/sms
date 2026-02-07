#include <MoveBG/MapObjDolpic.hpp>
#include <MoveBG/ItemManager.hpp>
#include <Player/MarioAccess.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/FlagManager.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <MSound/MSoundSE.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <Camera/CameraShake.hpp>
#include <Strategic/SharedParts.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/JMath.hpp>
#include <dolphin/mtx.h>
#include <stdlib.h>

// rogue includes needed for matching sinit & rodata
#include <M3DUtil/InfectiousStrings.hpp>

// fabricated helper for boolean materialization matching
static u8 isActorTypeOf(THitActor* actor, u32 base)
{
	u8 result;
	if ((actor->mActorType - base) == 1) {
		result = 1;
	} else {
		result = 0;
	}
	return result;
}

// TMonumentShine

TMonumentShine::TMonumentShine(const char* name)
    : TMapObjBase(name)
{
	unk13C = 1000;
	unk140 = 0.0f;
	unk144 = 0;
	unk148 = 1;
	unk149 = 0;
}

void TMonumentShine::initMapObj()
{
	TMapObjBase::initMapObj();
	mMActor->offMakeDL();

	unk138.r = 0xFF;
	unk138.g = 0xFF;
	unk138.b = 0xFF;

	if (TFlagManager::getInstance()->getFlag(0x10063) != 0) {
		unk138.a = 0;
		unk13C   = 0;
		unk149   = 1;
	} else {
		unk138.a = 100;
		unk13C   = 1000;
		unk149   = 0;
	}

	SMS_InitPacket_OneTevKColor(getModel(), 0, GX_KCOLOR0, &unk138);
	unk64 &= ~1;
}

static JGeometry::TVec3<f32> sUpHBW;
static s8 sInitHBW;
static JGeometry::TVec3<f32> sUpRing;
static s8 sInitRing;

void TMonumentShine::hitByWater(THitActor* actor)
{
	JGeometry::TVec3<f32> waterDir;
	JGeometry::TVec3<f32> marioDir;
	f32 _pad[7];
	(void)_pad;

	waterDir = actor->mPosition;
	f32 px   = mPosition.x;
	f32 py   = mPosition.y;
	f32 pz   = mPosition.z;
	waterDir.x -= px;
	waterDir.y -= py;
	waterDir.z -= pz;
	waterDir.y = 0.0f;

	f32 lenSq = waterDir.x * waterDir.x + waterDir.z * waterDir.z
	            + waterDir.y * waterDir.y;
	if (lenSq <= 0.0000038146973f)
		return;
	marioDir = *gpMarioPos;
	marioDir.x -= px;
	marioDir.y -= py;
	marioDir.z -= pz;
	marioDir.y = 0.0f;

	f32 lenSq2 = marioDir.x * marioDir.x + marioDir.y * marioDir.y
	             + marioDir.z * marioDir.z;
	if (lenSq2 <= 0.0000038146973f)
		return;

	if (!sInitHBW) {
		sUpHBW.x = 0.0f;
		sUpHBW.y = 1.0f;
		sUpHBW.z = 0.0f;
		sInitHBW = true;
	}

	f32 cx = sUpHBW.y * marioDir.z - sUpHBW.z * marioDir.y;
	f32 cy = sUpHBW.z * marioDir.x - sUpHBW.x * marioDir.z;
	f32 cz = sUpHBW.x * marioDir.y - sUpHBW.y * marioDir.x;

	f32 dot = cx * waterDir.y + cy * waterDir.x + cz * waterDir.z;
	if (dot > 0.0f) {
		unk140 += 0.004f;
	} else {
		unk140 -= 0.004f;
	}
}

BOOL TMonumentShine::receiveMessage(THitActor* sender, u32 message)
{
	f32 _pad[2];
	(void)_pad;

	if (isActorTypeOf(sender, 0x01000000)) {
		gpMarioParticleManager->emit(0xE7, &sender->mPosition, 0, nullptr);
		gpMSound->startSoundSet(0x6802, (const Vec*)&sender->mPosition, 0, 0.0f,
		                        0, 0, 4);

		if (unk13C == 0)
			return 1;

		unk13C = unk13C - 1;
		hitByWater(sender);

		unk138.a = (u8)(unk13C * 100 / 1000);

		if (unk13C == 0) {
			gpItemManager->makeShineAppearWithDemo(
			    "シャイン（モニュメントシャイン用）",
			    "モニュメントシャインカメラ", mPosition.x, mPosition.y,
			    mPosition.z);

			if (gpMSound->gateCheck(0x484A)) {
				MSoundSESystem::MSoundSE::startSoundSystemSE(0x484A, 0, nullptr,
				                                             0);
			}
		}

		return 1;
	}

	return 0;
}

void TMonumentShine::control()
{
	TMapObjBase::control();

	if (unk149)
		return;
	if (unk144 >= 3)
		return;

	unk140 *= 0.9992f;
	mAngularVelocity.y += unk140;

	if (unk13C > 0) {
		unk148 = (unk140 > 0.0f) ? 1 : -1;
		return;
	}

	if (fabsf(unk140) < 0.1f) {
		if (unk144 == 2) {
			if (unk148 > 0) {
				f32 rot    = mRotation.y;
				f32 target = mInitialRotation.y + 360.0f;
				f32 diff
				    = target - MsWrap(rot, target - 180.0f, target + 180.0f);
				if (diff > 0.1f)
					diff = 0.1f;
				if (0.0f == diff)
					unk144++;
				mAngularVelocity.y += diff;
			} else {
				f32 rot    = mRotation.y;
				f32 target = mInitialRotation.y - 360.0f;
				f32 diff
				    = target - MsWrap(rot, target - 180.0f, target + 180.0f);
				if (diff < -0.1f)
					diff = -0.1f;
				if (0.0f == diff)
					unk144++;
				mAngularVelocity.y += diff;
			}
		} else {
			if (unk148 > 0) {
				mAngularVelocity.y += 0.1f;
				while (mRotation.y + mAngularVelocity.y > 360.0f) {
					mRotation.y -= 360.0f;
					unk144++;
				}
			} else {
				mAngularVelocity.y -= 0.1f;
				f32 step = 360.0f;
				f32 zero = 0.0f;
				while (mRotation.y + mAngularVelocity.y < zero) {
					mRotation.y += step;
					unk144++;
				}
			}
		}
	} else {
		f32 rot   = mRotation.y;
		f32 limit = 360.0f;
		while (rot >= limit)
			rot -= limit;
		f32 zero = 0.0f;
		while (rot < zero)
			rot += limit;
		mRotation.y = rot;
	}
}

// TBellDolpic

TBellDolpic::TBellDolpic(int param_1, const char* name)
    : TMapObjBase(name)
{
	unk13C = param_1;
	unk140 = 1.0f;
	unk144 = 0.0f;
	unk148 = 0.0f;
	unk14C = 0.0f;
	unk150 = 0.0f;
	unk154 = 1000;
	unk158 = 0;
	unk15C = 0;
}

void TBellDolpic::initMapObj()
{
	TMapObjBase::initMapObj();
	mMActor->offMakeDL();

	unk138.r = 0xFF;
	unk138.g = 0xFF;
	unk138.b = 0xFF;

	if ((unk13C == 0 && TFlagManager::getInstance()->getFlag(0x10061) != 0)
	    || (unk13C == 1
	        && TFlagManager::getInstance()->getFlag(0x10060) != 0)) {
		unk138.a = 0;
		unk154   = 0;
	} else {
		unk138.a = 100;
		unk154   = 1000;
	}

	SMS_InitPacket_OneTevKColor(getModel(), 0, GX_KCOLOR0, &unk138);
	unk64 &= ~1;
}

void TBellDolpic::calcRootMatrix()
{
	TMapObjBase::calcRootMatrix();
	J3DModel* model = getModel();
	Mtx temp;
	PSMTXRotAxisRad(temp, (Vec*)&unk140, 0.017453292f * unk14C);
	PSMTXConcat(model->getBaseTRMtx(), temp, model->getBaseTRMtx());
}

void TBellDolpic::ring(const JGeometry::TVec3<f32>& pos)
{
	f32 _pad[2];
	(void)_pad;
	JGeometry::TVec3<f32> diff;

	if (fabsf(unk150) > 0.01f)
		return;
	diff = mPosition;
	diff.x -= pos.x;
	diff.y -= pos.y;
	diff.z -= pos.z;
	diff.y = 0.0f;

	if (!sInitRing) {
		sUpRing.x = 0.0f;
		sUpRing.y = 1.0f;
		sUpRing.z = 0.0f;
		sInitRing = true;
	}

	f32 dy = diff.y;
	f32 uz = sUpRing.z;
	f32 ux = sUpRing.x;
	f32 dz = diff.z;
	f32 uy = sUpRing.y;
	f32 dx = diff.x;
	unk140 = uy * dz - uz * dy;
	unk144 = uz * dx - ux * dz;
	unk148 = ux * dy - uy * dx;

	f32 lenSq = unk140 * unk140 + unk144 * unk144 + unk148 * unk148;
	if (lenSq <= 0.0000038146973f) {
		unk140 = 1.0f;
		unk144 = 0.0f;
		unk148 = 0.0f;
	} else {
		PSVECNormalize((Vec*)&unk140, (Vec*)&unk140);
	}

	unk150 -= 0.5f;

	int r   = rand();
	f32 tmp = (f32)r * 0.000030517578f;
	unk158  = (int)(tmp * 14400.0f) + 0x5460;
}

void TBellDolpic::touchPlayer(THitActor* actor) { ring(actor->mPosition); }

BOOL TBellDolpic::receiveMessage(THitActor* sender, u32 message)
{
	f32 _pad[2];
	(void)_pad;

	if (isActorTypeOf(sender, 0x80000000)) {
		ring(sender->mPosition);
	}

	if (isActorTypeOf(sender, 0x01000000)) {
		gpMarioParticleManager->emit(0xE7, &sender->mPosition, 0, nullptr);

		if (unk154 == 0)
			return 1;

		unk154 = unk154 - 1;

		unk138.a = (u8)(unk154 * 100 / 1000);

		if (unk154 == 0) {
			if (unk13C == 0) {
				gpItemManager->makeShineAppearWithDemo(
				    "シャイン（ドルピック鐘警察署用）",
				    "ドルピック鐘警察署カメラ", mPosition.x, mPosition.y,
				    mPosition.z);
			} else {
				gpItemManager->makeShineAppearWithDemo(
				    "シャイン（ドルピック鐘テャレビ局用）",
				    "ドルピック鐘テャレビ局カメラ", mPosition.x, mPosition.y,
				    mPosition.z);
			}

			if (gpMSound->gateCheck(0x484A)) {
				MSoundSESystem::MSoundSE::startSoundSystemSE(0x484A, 0, nullptr,
				                                             0);
			}
		}

		return 1;
	}

	return 0;
}

void TBellDolpic::control()
{
	JGeometry::TVec3<f32> pos;
	f32 _pad[8];
	(void)_pad;

	if (unk154 == 0) {
		if (unk158 == 0) {
			pos = mPosition;
			pos.z += 100.0f;
			ring(pos);
		} else {
			unk158 = unk158 - 1;
		}
	}

	TMapObjBase::control();

	f32 sinVal = -JMASin(unk14C);
	unk150     = 0.01f * sinVal + unk150;

	unk14C = unk14C + unk150;

	if (unk14C < -30.0f || unk14C > 30.0f) {
		unk150 = -unk150;

		if (unk154 == 0) {
			if (unk15C) {
				if (gpMSound->gateCheck(0x38A5)) {
					MSoundSESystem::MSoundSE::startSoundActor(
					    0x38A5, (const Vec*)&mPosition, 0, nullptr, 0, 4);
				}
			} else {
				if (gpMSound->gateCheck(0x38A6)) {
					MSoundSESystem::MSoundSE::startSoundActor(
					    0x38A6, (const Vec*)&mPosition, 0, nullptr, 0, 4);
				}
			}
		} else {
			if (unk15C) {
				if (gpMSound->gateCheck(0x38A8)) {
					MSoundSESystem::MSoundSE::startSoundActor(
					    0x38A8, (const Vec*)&mPosition, 0, nullptr, 0, 4);
				}
			} else {
				if (gpMSound->gateCheck(0x38A9)) {
					MSoundSESystem::MSoundSE::startSoundActor(
					    0x38A9, (const Vec*)&mPosition, 0, nullptr, 0, 4);
				}
			}
		}

		unk15C ^= 1;
	}

	unk150 = unk150 * 0.9995f;
}

// TDptMonteFence

void TDptMonteFence::touchPlayer(THitActor* actor)
{
	f32 _pad[4];
	(void)_pad;

	if (SMS_IsMarioStatusThrownDown()) {
		if (gpMSound->gateCheck(0x380A)) {
			MSoundSESystem::MSoundSE::startSoundActor(
			    0x380A, (const Vec*)&mPosition, 0, nullptr, 0, 4);
		}
		if (gpMSound->gateCheck(0x3857)) {
			MSoundSESystem::MSoundSE::startSoundActor(
			    0x3857, (const Vec*)&mPosition, 0, nullptr, 0, 4);
		}
		kill();
	}
}

// TMapObjSmoke

u32 TMapObjSmoke::touchWater(THitActor* actor)
{
	appearObj(100.0f);
	makeObjDead();
	return 1;
}

void TMapObjSmoke::control()
{
	emitAndRotateScale(0x152, 1, (const JGeometry::TVec3<f32>*)&mPosition);
}

void TMapObjSmoke::load(JSUMemoryInputStream& in)
{
	THideObjBase::load(in);
	SMS_LoadParticle("/scene/mapObj/ms_map_kokuen.jpa", 0x152);
}

// TMareGate

void TMareGate::control()
{
	f32 _pad[2];
	(void)_pad;

	TMapObjBase::control();
	MSound* sound = gpMSound;
	if (sound->gateCheck(0x3085)) {
		MSoundSESystem::MSoundSE::startSoundActor(
		    0x3085, (const Vec*)&mPosition, 0, (JAISound**)&sound->unk7C, 0, 4);
	}
}

void TMareGate::loadAfter()
{
	TMapObjBase::loadAfter();
	if (!TFlagManager::smInstance->getBool(0x50004)) {
		makeObjDead();
	}
}

// TDemoCannon

void TDemoCannon::loadAfter()
{
	SMS_LoadParticle("/scene/mapObj/demoCannon_a.jpa", 232);
	SMS_LoadParticle("/scene/mapObj/demoCannon_b.jpa", 233);
	SMS_LoadParticle("/scene/mapObj/demoCannon_c.jpa", 234);
	SMS_LoadParticle("/scene/mapObj/demoCannon_d.jpa", 235);
	SMS_LoadParticle("/scene/mapObj/demoCannon_e.jpa", 236);
	SMS_LoadParticle("/scene/mapObj/demoCannon_smoke.jpa", 358);
}

void TDemoCannon::initMapObj()
{
	TMapObjBase::initMapObj();

	mMActor->setBck("democannon_dpt");
	J3DFrameCtrl* frameCtrl  = mMActor->getFrameCtrl(0);
	frameCtrl->mCurrentFrame = (f32)frameCtrl->mEndFrame;

	void* res
	    = JKRFileLoader::getGlbResource("/scene/mapObj/demoCannon_dom.bmd");
	SDLModelData* sdlData
	    = new SDLModelData(J3DModelLoaderDataBase::load(res, 0x10050000));

	JUTNameTab* jointName = mMActor->getModel()->getModelData()->getJointName();

	TSharedParts* parts = new TSharedParts(this, jointName->getIndex("nullA"),
	                                       sdlData, 3, "<TSharedParts>");
	unk138              = parts;

	res = JKRFileLoader::getGlbResource("/scene/mapObj/demoCannon_mario.bmd");
	SDLModelData* sdlData2
	    = new SDLModelData(J3DModelLoaderDataBase::load(res, 0x10010000));

	parts  = new TSharedParts(this, 0, sdlData2, 3, "<TSharedParts>");
	unk13C = parts;
}

void TDemoCannon::startDemo()
{
	unk14C = 1;

	MSound* sound = gpMSound;
	s16 hp        = SMS_GetMarioHP();
	sound->startMarioVoice(30911, hp, 0);

	mMActor->setBck("democannon_dpt");
	unk138->getMActor()->setBck("democannon_dom");
	unk13C->getMActor()->setBck("democannon_mario_fly1");
}

void TDemoCannon::perform(u32 flags, JDrama::TGraphics* gfx)
{
	TMapObjBase::perform(flags, gfx);

	if (!unk14C)
		return;

	unk138->perform(flags, gfx);
	unk13C->getMActor()->perform(flags, gfx);

	if (!(flags & 2))
		return;

	J3DFrameCtrl* frameCtrl = unk13C->getMActor()->getFrameCtrl(0);
	if (frameCtrl->mCurrentFrame < 174.0f) {
		if (gpMSound->gateCheck(8392))
			MSoundSESystem::MSoundSE::startSoundActor(8392, &mPosition, 0,
			                                          nullptr, 0, 4);
	}

	frameCtrl = unk13C->getMActor()->getFrameCtrl(0);
	if (frameCtrl->checkPass(174.0f)) {
		gpCameraShake->startShake((EnumCamShakeMode)36, 1.0f);
		SMSRumbleMgr->start(21, 10, (f32*)0);

		Mtx* mtx = unk138->getMActor()->getModel()->mNodeMatrices;

		gpMarioParticleManager->emitAndBindToMtxPtr(232, (MtxPtr)mtx, 0,
		                                            nullptr);
		gpMarioParticleManager->emitAndBindToMtxPtr(233, (MtxPtr)mtx, 0,
		                                            nullptr);
		gpMarioParticleManager->emitAndBindToMtxPtr(234, (MtxPtr)mtx, 0,
		                                            nullptr);
		gpMarioParticleManager->emitAndBindToMtxPtr(235, (MtxPtr)mtx, 0,
		                                            nullptr);
		gpMarioParticleManager->emitAndBindToMtxPtr(236, (MtxPtr)mtx, 0,
		                                            nullptr);

		if (gpMSound->gateCheck(10574))
			MSoundSESystem::MSoundSE::startSoundActor(10574, &mPosition, 0,
			                                          nullptr, 0, 4);
		if (gpMSound->gateCheck(10605))
			MSoundSESystem::MSoundSE::startSoundActor(10605, &mPosition, 0,
			                                          nullptr, 0, 4);
	}

	frameCtrl = unk13C->getMActor()->getFrameCtrl(0);
	if (frameCtrl->mCurrentFrame > 175.0f) {
		Mtx* mtx = unk13C->getMActor()->getModel()->mNodeMatrices;
		gpMarioParticleManager->emitAndBindToMtxPtr(358, (MtxPtr)mtx, 1, this);
	}
}

// TTurboNozzleDoor

void TTurboNozzleDoor::loadAfter()
{
	f32 _pad[8];
	(void)_pad;

	if (strcmp("空港ドアＡ０", mName) == 0) {
		unk144 = JDrama::TNameRefGen::search<TLiveActor>("空港ドアＡ１");
	} else if (strcmp("空港ドアＡ１", mName) == 0) {
		unk144 = JDrama::TNameRefGen::search<TLiveActor>("空港ドアＡ０");
	} else if (strcmp("空港ドアＢ０", mName) == 0) {
		unk144 = JDrama::TNameRefGen::search<TLiveActor>("空港ドアＢ１");
	} else if (strcmp("空港ドアＢ１", mName) == 0) {
		unk144 = JDrama::TNameRefGen::search<TLiveActor>("空港ドアＢ０");
	}
}

void TTurboNozzleDoor::touchPlayer(THitActor* player)
{
	if (!SMS_IsMarioDashing())
		return;

	if (gpMarDirector->mMap == 1) {
		startBck("nozzledoor");
	} else {
		makeObjDead();
		((TMapObjBase*)unk144)->makeObjDead();
	}

	if (gpMSound->gateCheck(14346))
		MSoundSESystem::MSoundSE::startSoundActor(14346, &mPosition, 0, nullptr,
		                                          0, 4);
	if (gpMSound->gateCheck(14423))
		MSoundSESystem::MSoundSE::startSoundActor(14423, &mPosition, 0, nullptr,
		                                          0, 4);

	JGeometry::TVec3<f32> scale;
	f32 _pad[8];
	(void)_pad;

	scale.x = 1.3f;
	scale.y = 1.3f;
	scale.z = 1.3f;

	f32 yOff = 100.0f;
	unk138.x = mPosition.x;
	unk138.y = yOff + mPosition.y;
	unk138.z = mPosition.z;

	emitAndScale(24, 0, &unk138, scale);
	emitAndScale(25, 0, &unk138, scale);
	emitAndScale(57, 0, &unk138, scale);

	removeMapCollision();
	unk64 |= 1;
}

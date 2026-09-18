#include <MoveBG/MapObjDolpic.hpp>
#include <MoveBG/ItemManager.hpp>
#include <Player/MarioAccess.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/FlagManager.hpp>
#include <MSound/MSound.hpp>
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
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

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
	offHitFlag(HIT_FLAG_NO_COLLISION);
}

void TMonumentShine::hitByWater(THitActor* actor)
{

	JGeometry::TVec3<f32> waterDir = actor->mPosition;

	waterDir.sub(mPosition);
	waterDir.y = 0.0f;

	if (waterDir.squared() <= JGeometry::TUtil<f32>::epsilon())
		return;

	JGeometry::TVec3<f32> marioDir = SMS_GetMarioPos();
	marioDir.sub(mPosition);
	marioDir.y = 0.0f;

	if (marioDir.squared() <= JGeometry::TUtil<f32>::epsilon())
		return;

	static JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);

	// TODO: 5 instructions from exact, all reloads: retail keeps
	// mPosition.x/y/z in f7-f9 across both subtractions and waterDir.y/z in
	// f5/f6 from the first squared() into the dot, where our build reloads
	// all five. The frame is exact (the named `side` below is the missing 8
	// bytes), so the local set is right; what differs is only which values
	// the allocator keeps live. Ruled out: sub() for -= , getPosition() for
	// mPosition (lands the frame but hoists the address into a register),
	// isZero() for the spelled-out epsilon test (identical output), cross2()
	// for cross().
	JGeometry::TVec3<f32> cross;
	cross.cross(up, marioDir);

	f32 side = cross.dot(waterDir);

	if (side > 0.0f) {
		unk140 += 0.004f;
	} else {
		unk140 -= 0.004f;
	}
}

BOOL TMonumentShine::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->isActorType(0x01000001)) {
		gpMarioParticleManager->emit(PARTICLE_MS_ENM_WATHIT, &sender->mPosition,
		                             0, nullptr);
		SMSGetMSound()->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK,
		                              &sender->mPosition, 0, 0.0f, 0, 0, 4);

		if (unk13C == 0)
			return 1;

		unk13C -= 1;
		hitByWater(sender);

		unk138.a = (u8)(unk13C * 100 / 1000);

		if (unk13C == 0) {
			gpItemManager->makeShineAppearWithDemo(
			    "シャイン（モニュメントシャイン用）",
			    "モニュメントシャインカメラ", mPosition.x, mPosition.y,
			    mPosition.z);

			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_CLEAR_SIGN, 0, nullptr,
			                                   0);
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
				f32 diff
				    = MsAngleDiff(mInitialRotation.y + 360.0f, mRotation.y);
				if (diff > 0.1f)
					diff = 0.1f;
				if (0.0f == diff)
					unk144++;
				mAngularVelocity.y += diff;
			} else {
				f32 diff
				    = MsAngleDiff(mInitialRotation.y - 360.0f, mRotation.y);
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
				while (mRotation.y + mAngularVelocity.y < 0.0f) {
					mRotation.y += 360.0f;
					unk144++;
				}
			}
		}
	} else {
		f32 rot   = mRotation.y;
		f32 limit = 360.0f;
		while (rot >= limit)
			rot -= limit;
		while (rot < 0.0f)
			rot += limit;
		mRotation.y = rot;
	}
}

// TBellDolpic

TBellDolpic::TBellDolpic(int param_1, const char* name)
    : TMapObjBase(name)
{
	unk13C = param_1;
	unk140.set(1.0f, 0.0f, 0.0f);
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
	offHitFlag(HIT_FLAG_NO_COLLISION);
}

void TBellDolpic::calcRootMatrix()
{
	TMapObjBase::calcRootMatrix();
	J3DModel* model = getModel();
	Mtx temp;
	PSMTXRotAxisRad(temp, &unk140, 0.017453292f * unk14C);
	PSMTXConcat(model->getBaseTRMtx(), temp, model->getBaseTRMtx());
}

void TBellDolpic::ring(const JGeometry::TVec3<f32>& pos)
{
	if (fabsf(unk150) > 0.01f)
		return;
	JGeometry::TVec3<f32> diff = mPosition;
	diff -= pos;
	diff.y = 0.0f;

	static JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);

	unk140.cross2(up, diff);

	if (unk140.squared() <= JGeometry::TUtil<f32>::epsilon()) {
		unk140.set(1.0f, 0.0f, 0.0f);
	} else {
		VECNormalize(&unk140, &unk140);
	}

	unk150 -= 0.5f;

	int r   = rand();
	f32 tmp = (f32)r * 0.000030517578f;
	unk158  = (int)(tmp * 14400.0f) + 0x5460;
}

void TBellDolpic::touchPlayer(THitActor* actor) { ring(actor->mPosition); }

BOOL TBellDolpic::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->isActorType(0x80000001)) {
		ring(sender->mPosition);
	}

	if (sender->isActorType(0x01000001)) {
		gpMarioParticleManager->emit(PARTICLE_MS_ENM_WATHIT, &sender->mPosition,
		                             0, nullptr);

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
				    "シャイン（ドルピック鐘テレビ局用）",
				    "ドルピック鐘テレビ局カメラ", mPosition.x, mPosition.y,
				    mPosition.z);
			}

			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_CLEAR_SIGN, 0, nullptr,
			                                   0);
		}

		return 1;
	}

	return 0;
}

void TBellDolpic::control()
{
	JGeometry::TVec3<f32> pos;

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
				SMSGetMSound()->startSoundActor(MSD_SE_OBJ_DOL_BEL_GS4,
				                                &mPosition, 0, nullptr, 0, 4);

			} else {
				SMSGetMSound()->startSoundActor(MSD_SE_OBJ_DOL_BEL_E4,
				                                &mPosition, 0, nullptr, 0, 4);
			}
		} else {
			if (unk15C) {
				SMSGetMSound()->startSoundActor(MSD_SE_OBJ_DOL_BEL_GS4_K,
				                                &mPosition, 0, nullptr, 0, 4);
			} else {
				SMSGetMSound()->startSoundActor(MSD_SE_OBJ_DOL_BEL_E4_K,
				                                &mPosition, 0, nullptr, 0, 4);
			}
		}

		unk15C ^= 1;
	}

	unk150 *= 0.9995f;
}

// TDptMonteFence

void TDptMonteFence::touchPlayer(THitActor* actor)
{
	if (SMS_IsMarioStatusThrownDown()) {
		SMSGetMSound()->startSoundActor(MSD_SE_IT_BARREL_CRASH, &mPosition, 0,
		                                nullptr, 0, 4);
		SMSGetMSound()->startSoundActor(MSD_SE_OBJ_GLASS_BREAK, &mPosition, 0,
		                                nullptr, 0, 4);

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
	TMapObjBase::control();

	MSound* sound = SMSGetMSound();
	sound->startSoundActor(MSD_SE_OBJ_MAHRE_GATE_LIGHT, &mPosition,
	                       &sound->unk7C);
}

void TMareGate::loadAfter()
{
	TMapObjBase::loadAfter();
	if (!TFlagManager::smInstance->getBool(0x50004)) {
		makeObjDead();
	}
}

// TWeathercock

void TWeathercock::control() { TMapObjTurn::control(); }

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

	getMActor()->setBck("democannon_dpt");
	J3DFrameCtrl* frameCtrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
	frameCtrl->setFrame(frameCtrl->getEnd());

	void* res
	    = JKRFileLoader::getGlbResource("/scene/mapObj/demoCannon_dom.bmd");
	SDLModelData* sdlData = new SDLModelData(J3DModelLoaderDataBase::load(
	    res, J3DMLF_MaterialPEFull | (5 << J3DMLF_TevStageNumShift)));

	JUTNameTab* jointName
	    = getMActor()->getModel()->getModelData()->getJointName();

	TSharedParts* parts = new TSharedParts(this, jointName->getIndex("nullA"),
	                                       sdlData, 3, "<TSharedParts>");
	unk138              = parts;

	res     = JKRFileLoader::getGlbResource("/scene/mapObj/demoCannon_mario.bmd");
	sdlData = new SDLModelData(J3DModelLoaderDataBase::load(
	    res, J3DMLF_MaterialPEFull | (1 << J3DMLF_TevStageNumShift)));

	parts  = new TSharedParts(this, 0, sdlData, 3, "<TSharedParts>");
	unk13C = parts;
}

void TDemoCannon::startDemo()
{
	unk14C = 1;

	mMActor->setBck("democannon_dpt");
	unk138->getMActor()->setBck("democannon_dom");
	unk13C->getMActor()->setBck("democannon_mario_fly1");
}

void TDemoCannon::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TMapObjBase::perform(cue, graphics);

	if (!unk14C)
		return;

	unk138->perform(cue, graphics);
	unk13C->getMActor()->perform(cue, graphics);

	if (!(cue & CUE_CALC_ANIM))
		return;

	J3DFrameCtrl* frameCtrl = unk13C->getMActor()->getFrameCtrl(ANM_TYPE_BCK);
	if (frameCtrl->getFrame() < 174.0f) {
		SMSGetMSound()->startSoundActor(MSD_SE_EN_CANNON_MOVE, &mPosition, 0,
		                                nullptr, 0, 4);
	}

	frameCtrl = unk13C->getMActor()->getFrameCtrl(ANM_TYPE_BCK);
	if (frameCtrl->checkPass(174.0f)) {
		gpCameraShake->startShake(CAM_SHAKE_MODE_CANNON_DEMO, 1.0f);
		SMSRumbleMgr->start(21, 10, (f32*)nullptr);

		MtxPtr mtx = unk138->getMActor()->getModel()->getAnmMtx(0);

		gpMarioParticleManager->emitAndBindToMtxPtr(232, mtx, 0, nullptr);
		gpMarioParticleManager->emitAndBindToMtxPtr(233, mtx, 0, nullptr);
		gpMarioParticleManager->emitAndBindToMtxPtr(234, mtx, 0, nullptr);
		gpMarioParticleManager->emitAndBindToMtxPtr(235, mtx, 0, nullptr);
		gpMarioParticleManager->emitAndBindToMtxPtr(236, mtx, 0, nullptr);

		SMSGetMSound()->startSoundActor(MSD_SE_EN_CANNON_FIRE_MARIO, &mPosition,
		                                0, nullptr, 0, 4);
		SMSGetMSound()->startSoundActor(MSD_SE_DM_FLY_TO_PINNNA, &mPosition, 0,
		                                nullptr, 0, 4);
	}

	frameCtrl = unk13C->getMActor()->getFrameCtrl(ANM_TYPE_BCK);
	if (frameCtrl->getFrame() > 175.0f) {
		MtxPtr mtx = unk13C->getMActor()->getModel()->getAnmMtx(0);
		gpMarioParticleManager->emitAndBindToMtxPtr(358, mtx, 1, this);

		frameCtrl = unk13C->getMActor()->getFrameCtrl(ANM_TYPE_BCK);
		if (frameCtrl->checkPass(204.0f)) {
			MSound* sound = gpMSound;
			s16 hp        = SMS_GetMarioHP();
			sound->startMarioVoice(30911, hp, 0);

			JAISound* voice = gpMSound->checkMarioVoicePlaying(0);
			if (voice)
				voice->setVolume(0.0f, 60, 0);
		}
	}
}

// TTurboNozzleDoor

void TTurboNozzleDoor::loadAfter()
{
	if (strcmp("空港ドアＡ０", getName()) == 0) {
		unk144 = JDrama::TNameRefGen::search<TMapObjBase>("空港ドアＡ１");
	} else if (strcmp("空港ドアＡ１", getName()) == 0) {
		unk144 = JDrama::TNameRefGen::search<TMapObjBase>("空港ドアＡ０");
	} else if (strcmp("空港ドアＢ０", getName()) == 0) {
		unk144 = JDrama::TNameRefGen::search<TMapObjBase>("空港ドアＢ１");
	} else if (strcmp("空港ドアＢ１", getName()) == 0) {
		unk144 = JDrama::TNameRefGen::search<TMapObjBase>("空港ドアＢ０");
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
		unk144->makeObjDead();
	}

	SMSGetMSound()->startSoundActor(MSD_SE_IT_BARREL_CRASH, &mPosition, 0,
	                                nullptr, 0, 4);
	SMSGetMSound()->startSoundActor(MSD_SE_OBJ_GLASS_BREAK, &mPosition, 0,
	                                nullptr, 0, 4);

	// TODO: 8 bytes of frame short (0x40 vs 0x48) with every instruction
	// exact. The three `getPosition()` reads below are worth +16 with no
	// instruction change; `SMSGetMarDirector()->mMap` over
	// `gpMarDirector->mMap`, a three-argument `scale` constructor and three
	// unnamed scale temporaries are all worth nothing here, and
	// `&getPosition()` for either sound call hoists the address out of the
	// call.
	JGeometry::TVec3<f32> scale(1.3f);

	unk138.set(getPosition().x, getPosition().y + 100.0f, getPosition().z);

	emitAndScale(24, 0, &unk138, scale);
	emitAndScale(25, 0, &unk138, scale);
	emitAndScale(57, 0, &unk138, scale);

	removeMapCollision();
	onHitFlag(HIT_FLAG_NO_COLLISION);
}

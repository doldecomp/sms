// rogue includes needed for matching sinit & bss
#include <JSystem/JAudio/JALibrary/JALSystem.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

#include <Enemy/Conductor.hpp>
#include <Enemy/EffectObj.hpp>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorUtil.hpp>
#include <MSound/MSound.hpp>
#include <MSound/SoundEffects.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/Item.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <MoveBG/MapObjPinna.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/Yoshi.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <macros.h>
#include <version.h>

static s32 switchSnd;

#ifdef VERSION_GMSP01
static f32 rotate_frame_rate = 1.0f;
#endif

s32 TFerrisWheel::becomeCalmlyCallback(uintptr_t param_1, u32 param_2)
{
	TFerrisWheel* wheel = (TFerrisWheel*)param_1;
	if (param_2 == 0) {
		wheel->mState = STATE_UNK2;
		MSound* sound = SMSGetMSound();
		if (sound->unk80) {
			sound->unk80->setVolume(0.0f, 200, 0);
			sound->unk80->setPitch(0.5f, 200, 0);
		}
		wheel->mStateTimer = 120;
	}
	return 0;
}

void TFerrisWheel::control()
{
	TMapObjBase::control();
	if (isState(STATE_UNK2) && !isStateTimerEngaged()) {
		if (unk140 > VERSION_SELECT(GMSJ01(SMSGetAnmFrameRate()),
		                            GMSP01(rotate_frame_rate))
		                 / 4.0f)
			unk140 -= 0.015f;
		else
			mState = STATE_NORMAL;
	}
	if (unk140 > VERSION_SELECT(GMSJ01(SMSGetAnmFrameRate()),
	                            GMSP01(rotate_frame_rate))
	                 / 4.0f) {
		MSound* sound = SMSGetMSound();
		sound->startSoundActor(MSD_SE_OBJ_MAHRE_GATE_LIGHT, &mPosition, 0,
		                       &sound->unk80, 0, 4);
	}
	J3DFrameCtrl* frameCtrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
	frameCtrl->setFrame(frameCtrl->getFrame() + unk140);
	for (int i = 0; i < unk138; ++i) {
		TMapObjBase* gondola = unk13C[i];
		MtxPtr mtx           = getModel()->getAnmMtx(i + 1);
		MTXCopy(mtx, gondola->getModel()->getAnmMtx(0));
		gondola->mPosition.set(mtx[0][3], mtx[1][3] + gondola->mYOffset,
		                       mtx[2][3]);
	}
}

void TFerrisWheel::initMapObj()
{
	TMapObjBase::initMapObj();
	unk138 = getModel()->getModelData()->getJointNum() - 1;
	unk13C = new TMapObjBase*[unk138];
	for (u16 i = 0; i < unk138; ++i) {
		unk13C[i] = TMapObjBaseManager::newAndRegisterObj("FerrisGondola");
		unk13C[i]->appear();
	}
#ifdef VERSION_GMSP01
	if ((SMSGetMarDirector()->getCurrentMap() == 13
	     && SMSGetMarDirector()->getCurrentStage() == 2)
	    || (SMSGetMarDirector()->getCurrentMap() == 5
	        && SMSGetMarDirector()->getCurrentStage() == 4)) {
#else
	if (SMSGetMarDirector()->getCurrentStage() == 2) {
#endif
		unk140 = 10.0f;
	} else {
		unk140 = VERSION_SELECT(GMSJ01(SMSGetAnmFrameRate()),
		                        GMSP01(rotate_frame_rate))
		         / 4.0f;
	}
}

TFerrisWheel::TFerrisWheel(const char* param_1)
    : TMapObjBase(param_1)
    , unk138(0)
    , unk13C(nullptr)
    , unk140(0.0f)
{
}

void THorizontalViking::updateTrans()
{
	f32 sin     = sinf(3.14f * (unk148 / 180.0f));
	mPosition.x = unk138 * sin + mInitialPosition.x;
	f32 cos     = cosf(3.14f * (unk148 / 180.0f));
	mPosition.y = mYOffset + (unk138 * (1.0f - cos) + mInitialPosition.y);
}

void THorizontalViking::moveNormal()
{
	switch (mState) {
	case STATE_NORMAL:
		unk144 -= unk13C;
		unk148 += unk144;
		if (unk148 < 0.0f)
			mState = STATE_UNK2;
		break;
	case STATE_UNK2:
		unk144 += unk13C;
		unk148 += unk144;
		if (unk148 > 0.0f)
			mState = STATE_NORMAL;
		break;
	}
}

void THorizontalViking::control()
{
	TMapObjBase::control();
	moveNormal();
	updateTrans();
}

void THorizontalViking::reset()
{
	unk144 = unk140;
	unk148 = 0.0f;
	if (unk144 > 0.0f)
		mState = STATE_NORMAL;
	else
		mState = STATE_UNK2;
}

void THorizontalViking::initMapObj()
{
	TMapObjBase::initMapObj();
	unk138 = 2500.0f;
	unk13C = 0.0008f;
	unk140 = 0.23f;
	reset();
}

THorizontalViking::THorizontalViking(const char* param_1)
    : TMapObjBase(param_1)
    , unk138(0.0f)
    , unk13C(0.0f)
    , unk140(0.0f)
    , unk144(0.0f)
    , unk148(0.0f)
{
}

void TViking::roll()
{
	switch (mState) {
	case STATE_NORMAL:
		unk144 *= unk154;
		unk144 -= unk13C;
		unk148 += unk144;
		if (unk148 < 0.0f) {
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_OBJ_PIN_BIKING_WING, &mPosition, nullptr, fabsf(unk144),
			    0, 0, nullptr, 0, 4);
			mState = STATE_UNK2;
		}
		if (unk148 > 180.0f) {
			unk148 -= 360.0f;
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_OBJ_PIN_BIKING_WING, &mPosition, nullptr, fabsf(unk144),
			    0, 0, nullptr, 0, 4);
			mState = STATE_UNK4;
		}
		break;
	case STATE_UNK2:
		unk144 *= unk154;
		unk144 += unk13C;
		unk148 += unk144;
		if (unk148 > 0.0f) {
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_OBJ_PIN_BIKING_WING, &mPosition, nullptr, fabsf(unk144),
			    0, 0, nullptr, 0, 4);
			mState = STATE_NORMAL;
		}
		if (unk148 < -180.0f) {
			unk148 += 360.0f;
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_OBJ_PIN_BIKING_WING, &mPosition, nullptr, fabsf(unk144),
			    0, 0, nullptr, 0, 4);
			mState = STATE_UNK3;
		}
		break;
	case STATE_UNK3:
		unk144 *= unk158;
		unk144 -= unk13C;
		unk148 += unk144;
		if (unk148 < 0.0f) {
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_OBJ_PIN_BIKING_WING, &mPosition, nullptr, fabsf(unk144),
			    0, 0, nullptr, 0, 4);
			if (unk144 > -unk150)
				mState = STATE_UNK2;
			else
				mState = STATE_UNK4;
		}
		break;
	case STATE_UNK4:
		unk144 *= unk158;
		unk144 += unk13C;
		unk148 += unk144;
		if (unk148 > 0.0f) {
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_OBJ_PIN_BIKING_WING, &mPosition, nullptr, fabsf(unk144),
			    0, 0, nullptr, 0, 4);
			if (unk144 < unk150)
				mState = STATE_NORMAL;
			else
				mState = STATE_UNK3;
		}
		break;
	}
}

void TViking::control()
{
	switch (unk14C) {
	case MODE_UNK0:
		moveNormal();
		break;
	case MODE_UNK1:
		roll();
		break;
	}
	updateTrans();
	mRotation.z = unk148;
	updateObjMtx();
}

void TViking::reset()
{
	unk144 = unk140;
	unk148 = 0.0f;
	if (unk140 > 0.0f)
		mState = STATE_NORMAL;
	else
		mState = STATE_UNK2;
}

void TViking::loadAfter()
{
	TMapObjBase::loadAfter();
	reset();
}

void TViking::initMapObj()
{
	unk14C = MODE_UNK1;
	if (strcmp(getName(), "viking 0") == 0) {
		unk138 = 1400.0f;
		unk13C = 0.001f;
		unk154 = 1.001f;
		unk158 = 0.999f;
		unk140 = -0.3f;
		unk150 = 0.3f;
	} else {
		unk138 = 1400.0f;
		unk13C = 0.001f;
		unk154 = 1.001f;
		unk158 = 0.999f;
		unk140 = 0.3f;
		unk150 = 0.3f;
	}
	mPosition.y -= unk138;
	TMapObjBase::initMapObj();
}

TViking::TViking(const char* param_1)
    : THorizontalViking(param_1)
    , unk14C(MODE_UNK0)
    , unk150(0.0f)
    , unk154(0.0f)
    , unk158(0.0f)
{
}

void TPinnaShell::opened()
{
	unk6C = -TShellCup::mOpenRotMax;
	unk7C = 360;
	if (unk80 != nullptr && !unk80->checkLiveFlag(LIVE_FLAG_DEAD)) {
		if (unk80->isActorType(ACTOR_TYPE_COIN_BLUE)) {
			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_COLLECT_PRETTY, 0,
			                                   nullptr, 0);
		} else {
			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_COIN_APPEAR, 0,
			                                   nullptr, 0);
		}
	} else {
		SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_NOT_COLLECT, 0, nullptr,
		                                   0);
	}
	unk68 = STATE_UNK2;
}

BOOL TPinnaShell::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (param_2 == HIT_MESSAGE_SPRAYED_BY_WATER) {
		gpMarioParticleManager->emit(PARTICLE_MS_ENM_WATHIT,
		                             &param_1->mPosition, 0, nullptr);
		SMSGetMSound()->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK, &mPosition, 0,
		                              0.0f, 0, 0, 4);
		if (unk68 == STATE_UNK0) {
			unk6C -= TShellCup::mWaterOpenAccel;
			if (unk6C < -TShellCup::mOpenRotMax)
				unk68 = STATE_UNK1;
		}
		return TRUE;
	}
	return FALSE;
}

void TPinnaShell::control()
{
	if (unk7C > 0) {
		unk7C--;
	}
	switch (unk68) {
	case STATE_UNK0:
		if (unk6C < 0.0f) {
			unk6C += TShellCup::mCloseAccel * (0.5f * MsRandF() + 0.5f);
		} else {
			unk6C = 0.0f;
		}
		break;
	case STATE_UNK1:
		unk6C -= 0.8f;
		if (unk6C < -TShellCup::mOpenRotMax) {
			opened();
		}
		break;
	case STATE_UNK2:
		if (unk7C <= 0) {
			unk68 = STATE_UNK3;
			SMSGetMSound()->startSoundActor(MSD_SE_OBJ_PIN_SHELL_CLOSE,
			                                &mPosition, 0, nullptr, 0, 4);
		}
		break;
	case STATE_UNK3:
		unk6C += unk70;
		if (unk6C >= -TShellCup::mShellDamageRot) {
			unk88->offHitFilter(HIT_FILTER_NO_COLLISION);
		}
		if (unk6C >= 0.0f) {
			unk6C = 0.0f;
			unk68 = STATE_UNK0;
			unk88->onHitFilter(HIT_FILTER_NO_COLLISION);
		}
		break;
	}
	mPosition.set(
	    0.7f * (unk74[0][3] - unk8C->mPosition.x) + unk8C->mPosition.x,
	    unk74[1][3] - 100.0f,
	    0.7f * (unk74[2][3] - unk8C->mPosition.z) + unk8C->mPosition.z);
	unk88->mPosition.set(mPosition);
	if (mColCount != 0) {
		Mtx mtx;
		MsMtxSetRotX(mtx, unk6C);
		TMapObjBase::concatOnlyRotFromRight(unk74, mtx, mtx);
		unk84->moveMtx(mtx);
	}
}

TPinnaShell::TPinnaShell(const char* param_1)
    : THitActor(param_1)
    , unk68(0)
    , unk6C(0.0f)
    , unk70(0.0f)
    , unk74(nullptr)
    , unk78(nullptr)
    , unk7C(0)
    , unk80(nullptr)
    , unk84(nullptr)
    , unk88(nullptr)
    , unk8C(nullptr)
{
	initHitActor(ACTOR_TYPE_PINNA_SHELL, 1, HIT_CATEGORY_PLAYER, 250.0f, 400.0f,
	             250.0f, 200.0f);
}

f32 TShellCup::mOpenRotMax     = 90.0f;
f32 TShellCup::mShellDamageRot = 45.0f;
f32 TShellCup::mWaterOpenAccel = 5.0f;
f32 TShellCup::mCloseAccel     = 3.5f;

void TShellCup::control()
{
	mMActor->calc();
	for (int i = 0; i < ARRAY_COUNT(unk138); ++i)
		unk138[i].control();
}

void TShellCup::attachCoin(TCoin* coin, int index)
{
	if (!coin->checkLiveFlag(LIVE_FLAG_DEAD)) {
		coin->mPosition.set(unk138[index].mPosition);
	}
}

void TShellCup::calcAfter()
{
	if (SMSGetMarDirector()->isTalkModeNow()
	    && !SMSGetMarDirector()->isDemoModeNow())
		return;

	for (int i = 0; i < ARRAY_COUNT(unk138); ++i) {
		TPinnaShell* shell = &unk138[i];
		Mtx rot;
		MsMtxSetRotX(rot, shell->unk6C);
		TMapObjBase::concatOnlyRotFromRight(shell->unk74, rot, shell->unk74);
	}
	attachCoin(unk498, 0);
	attachCoin(unk49C, 2);
	attachCoin(unk4A0, 4);
}

void TShellCup::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TMapObjBase::perform(cue, graphics);
	if (cue & CUE_CALC_ANIM)
		calcAfter();
}

void TShellCup::loadAfter()
{
	TMapObjBase::loadAfter();
	for (int i = 0; i < ARRAY_COUNT(unk138); ++i)
		TMapObjBase::joinToGroup("オブジェクトグループ", unk138[i].unk88);

	unk498 = static_cast<TCoin*>(
	    TMapObjBaseManager::newAndRegisterObj("coin_blue"));
	unk49C = gpItemManager->newAndRegisterCoinReal();
	unk4A0 = gpItemManager->newAndRegisterCoinReal();
	unk498->setEventId(2);
	if (!TFlagManager::getInstance()->getBlueCoinFlag(
	        SMSGetMarDirector()->getCurrentMap(), unk498->getEventId())) {
		unk498->makeObjAppeared();
		unk138[0].unk80 = unk498;
	}
	unk49C->makeObjAppeared();
	unk49C->onMapObjFlag(TMapObjBase::MAP_OBJ_FLAG_UNK10000000);
	unk4A0->makeObjAppeared();
	unk4A0->onMapObjFlag(TMapObjBase::MAP_OBJ_FLAG_UNK10000000);
	unk138[2].unk80 = unk49C;
	unk138[4].unk80 = unk4A0;
}

void TShellCup::initMapObj()
{
	TMapObjBase::initMapObj();
	for (int i = 0; i < ARRAY_COUNT(unk138); ++i) {
		unk138[i].unk68 = TPinnaShell::STATE_UNK0;
		unk138[i].unk6C = 0.0f;
		unk138[i].unk70 = 8.0f;
		unk138[i].unk74 = getModel()->getAnmMtx(i + 1);
		unk138[i].unk78
		    = getModel()->getModelData()->getJointNodePointer(i + 1);
		unk138[i].unk7C = MsRandF() * 1200.0f;
		unk138[i].unk8C = this;
		TMapObjBase::joinToGroup("オブジェクトグループ", &unk138[i]);
		unk138[i].unk84 = new TMapCollisionMove;
		unk138[i].unk84->init("/mapObj/ShellCup", 0, this);
		unk138[i].unk84->setUp();
		unk138[i].unk88 = new TDamageObj;
		unk138[i].unk88->mScaling.set(2.0f, 1.2f, 2.0f);
		unk138[i].unk88->init(ACTOR_TYPE_ENEMY_DAMAGE_OBJ);
		unk138[i].unk88->onHitFilter(HIT_FILTER_NO_COLLISION);
	}
	TMapCollisionStatic* rink = new TMapCollisionStatic;
	rink->init("/mapObj/ShellCup_rink", 2, this);
	rink->setUpMtx(getModel()->getAnmMtx(0));
}

TShellCup::TShellCup(const char* param_1)
    : TMapObjBase(param_1)
    , unk498(nullptr)
    , unk49C(nullptr)
    , unk4A0(nullptr)
{
}

f32 TMerrygoround::mRotSpeed = 0.1f;

void TMerrygoround::control()
{
	TMapObjBase::control();
	mRotation.y += mRotSpeed;
	if (mRotation.y > 360.0f)
		mRotation.y -= 360.0f;
	for (int i = 0; i < ARRAY_COUNT(unk138); ++i) {
		MtxPtr mtx = getModel()->getAnmMtx(unk140[i]);
		unk138[i]->setModelMtx(mtx);
		unk138[i]->mPosition.set(mtx[0][3], mtx[1][3], mtx[2][3]);
	}
	for (int i = 0; i < ARRAY_COUNT(unk144); ++i) {
		MtxPtr mtx = getModel()->getAnmMtx(unk18C[i]);
		unk144[i]->unk138.set(mtx);
		unk144[i]->mPosition.set(mtx[0][3], mtx[1][3] - 600.0f, mtx[2][3]);
	}
	if (SMS_IsMarioOnYoshi())
		unk1A0->offHitFilter(HIT_FILTER_NO_COLLISION);
	else
		unk1A0->onHitFilter(HIT_FILTER_NO_COLLISION);
	MtxPtr mtx = getModel()->getAnmMtx(unk1A4);
	unk1A0->mPosition.set(mtx[0][3], mtx[1][3] - 600.0f, mtx[2][3]);
}

void TMerrygoround::draw() const { }

void TMerrygoround::initMapObj()
{
	TMapObjBase::initMapObj();
	int eggCount  = 0;
	int poleCount = 0;
	for (u16 i = 1; i < getModel()->getModelData()->getJointNum(); ++i) {
		const char* name
		    = getModel()->getModelData()->getJointName()->getName(i);
		if (strstr(name, "egg")) {
			unk140[eggCount] = i;
			eggCount++;
		} else if (strcmp(name, "yoshi_warp") == 0) {
			unk1A4 = i;
		} else if (strcmp(name, "up") != 0 && strcmp(name, "down") != 0
		           && strcmp(name, "KAGE_2") != 0) {
			unk18C[poleCount] = i;
			poleCount++;
		}
	}
	for (int i = 0; i < ARRAY_COUNT(unk138); ++i) {
		unk138[i] = TMapObjBaseManager::newAndRegisterObj("merry_egg");
		unk138[i]->appear();
	}
	for (int i = 0; i < ARRAY_COUNT(unk144); ++i) {
		unk144[i] = static_cast<TMerryPole*>(
		    TMapObjBaseManager::newAndRegisterObj("merry_pole"));
		unk144[i]->appear();
		unk168[i] = new TMapCollisionMove;
		unk168[i]->init("/scene/mapObj/merry_yoshi.col", 0, this);
		unk168[i]->setUp();
	}
	unk1A0 = static_cast<TMapObjChangeStage*>(
	    TMapObjBaseManager::newAndRegisterObj("ChangeStageMerrygoround"));
	unk1A0->unk138 = 0x29;
	unk1A0->mScaling.y *= 1.5f;
	unk1A0->makeObjAppeared();
	TMapObjBase::joinToGroup("マップグループ", unk1A0);
}

TMerrygoround::TMerrygoround(const char* param_1)
    : TMapObjBase(param_1)
{
	unk1A0 = nullptr;
	unk1A4 = 0;
	for (int i = 0; i < ARRAY_COUNT(unk138); i++) {
		unk138[i] = nullptr;
		unk140[i] = 0;
	}
	for (int i = 0; i < ARRAY_COUNT(unk144); i++) {
		unk144[i] = nullptr;
		unk18C[i] = 0;
		unk168[i] = nullptr;
	}
}

void TChangeStageMerrygoround::touchPlayer(THitActor* param_1)
{
	if (!isStateTimerEngaged()) {
		if (SMS_GetYoshi()->mType == 1) {
			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_COLLECT_YOSHI, 0,
			                                   nullptr, 0);
			TMapObjChangeStage::touchPlayer(param_1);
			unk13C = 1;
		} else {
			SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_NOT_COLLECT_YOSHI, 0,
			                                   nullptr, 0);
		}
		mStateTimer = 600;
	}
}

void TChangeStageMerrygoround::calc()
{
	if (unk13C) {
		gpMarioParticleManager->emitAndBindToPosPtr(
		    PARTICLE_MS_HIKAGE1_A, &SMS_GetMarioPos(), 1, this);
		gpMarioParticleManager->emitAndBindToPosPtr(
		    PARTICLE_MS_HIKAGE1_B, &SMS_GetMarioPos(), 1, this);
	}
}

void TBalloonKoopaJr::touchActor(THitActor* param_1) { kill(); }

void TBalloonKoopaJr::kill()
{
	TMapObjGeneral::kill();
	emitAndScale(MAPOBJ_BALLOONKOOPAJR, 0, &unk148);
	emitAndScale(MAPOBJ_BALLOONKOOPAJRA, 0, &unk148);
	emitAndScale(MAPOBJ_BALLOONKOOPAJRB, 0, &unk148);
	TFlagManager::getInstance()->incFlag(MSF_BALLOON_COUNT, 1);
	SMSGetMSound()->startSoundActor(MSD_SE_BS_BSPAKU_SLAP, &mPosition, 0,
	                                nullptr, 0, 4);
}

void TBalloonKoopaJr::load(JSUMemoryInputStream& param_1)
{
	TMapObjBase::load(param_1);
	SMS_LoadParticle("/scene/mapObj/balloonKoopaJr.jpa", MAPOBJ_BALLOONKOOPAJR);
	SMS_LoadParticle("/scene/mapObj/balloonKoopaJrA.jpa",
	                 MAPOBJ_BALLOONKOOPAJRA);
	SMS_LoadParticle("/scene/mapObj/balloonKoopaJrB.jpa",
	                 MAPOBJ_BALLOONKOOPAJRB);

	int jointIdx
	    = getModel()->getModelData()->getJointName()->getIndex("center");
	MtxPtr mtx = getModel()->getAnmMtx((u16)jointIdx);
	unk148.set(mtx[0][3], mtx[1][3], mtx[2][3]);
}

void TPinnaEntrance::loadAfter()
{
	TMapObjBase::loadAfter();
	JGeometry::TVec3<f32> rot(90.0f, 0.0f, 0.0f);
	TMapObjBaseManager::newAndRegisterObj(
	    "GateManta", mPosition, rot, JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
}

void TWaterRecoverObj::touchPlayer(THitActor* param_1)
{
	if (param_1->isActorType(ACTOR_TYPE_MARIO) && !isStateTimerEngaged()) {
		param_1->receiveMessage(this, HIT_MESSAGE_ATTACK);
		mStateTimer = 600;
	}
}

void TAmiKing::loadAfter()
{
	TMapObjBase::loadAfter();
	SMS_LoadParticle(VERSION_SELECT(GMSJ01("/scene/Mapobj/amiking.jpa"),
	                                GMSP01("/scene/mapObj/amiking.jpa")),
	                 MAPOBJ_AMIKING);
}

void TAmiKing::initMapObj()
{
	TMapObjBase::initMapObj();
	initAnmSound();
	mMActor->setBck("amiking_sleep1");
	setAnmSound("/scene/mapObj/amiking_sleep1.bas");
	offLiveFlag(LIVE_FLAG_UNK10);
	for (u8 i = 0; i < getMActor()->getModel()->getModelData()->getJointNum();
	     ++i) { }
}

void TAmiKing::moveObject()
{
	TLiveActor::moveObject();
	if (unk138) {
		if (mMActor->checkCurAnm("amiking_flying1_start", ANM_TYPE_BCK)) {
			if (mMActor->curAnmEndsNext())
				mMActor->setBck("amiking_flying1_loop");
			return;
		}
		if (!mGroundPlane->isWaterSurface())
			return;
		if (isAirborne())
			return;
		SMSGetMSound()->startSoundActor(MSD_SE_EN_AMIKING_DIVE, &mPosition, 0,
		                                nullptr, 0, 4);
		JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_DNK_SHIBIRE_B, mMActor->getModel()->getAnmMtx(0), 0,
		    nullptr);
		if (emitter) {
			emitter->setGlobalScale(JGeometry::TVec3<f32>(4.0f, 4.0f, 4.0f));
		}
		TEffectColumWater* effect
		    = (TEffectColumWater*)gpConductor->makeOneEnemyAppear(
		        mPosition, "エフェクト水柱マネージャー", 1);
		if (effect) {
			JGeometry::TVec3<f32> scale(4.0f, 4.0f, 4.0f);
			effect->generate(mPosition, scale);
		}
		gpItemManager->makeShineAppearWithDemo(
		    "シャイン（観覧車シャイン用）", "観覧車シャインカメラ", mPosition.x,
		    mPosition.y, mPosition.z);
		TFerrisWheel* wheel = static_cast<TFerrisWheel*>(
		    JDrama::TNameRefGen::search("FerrisWheel"));
		SMSGetMarDirector()->fireStartDemoCamera(
		    "観覧車正常化カメラ", &wheel->mPosition, -1, 0.0f, true,
		    TFerrisWheel::becomeCalmlyCallback, (uintptr_t)wheel, nullptr,
		    JDrama::TFlagT<u16>(0));
		kill();
	} else {
		TMapObjBase* obj = (TMapObjBase*)mGroundPlane->getActor();
		if (obj && obj->isActorType(ACTOR_TYPE_FENCE_REVOLVE_INNER)
		    && (obj->isState(3) || obj->isState(5) || obj->isState(4)
		        || obj->isState(6))) {
			unk138 = 1;
			setVelocityAndFlag10(5.0f, 10.0f, -10.0f);
			mMActor->setBck("amiking_flying1_start");
			setAnmSound(nullptr);
			SMSGetMarDirector()->fireStartDemoCamera(
			    "観覧車ボス撃沈カメラ", &mPosition, -1, 0.0f, true, nullptr, 0,
			    nullptr, JDrama::TFlagT<u16>(0));
		}
	}
}

#ifdef VERSION_GMSP01
void TAmiKing::calc()
#else
void TAmiKing::calcRootMatrix()
#endif
{
#ifndef VERSION_GMSP01
	TMapObjBase::calcRootMatrix();
#endif
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    MAPOBJ_AMIKING, getModel()->getAnmMtx(0), 1, this);
	if (!unk138) {
		MtxPtr jointMtx = getMActor()->getModel()->getAnmMtx(6);
		unk13C.set(jointMtx[0][3], jointMtx[1][3], jointMtx[2][3]);
		JGeometry::TVec3<f32> offset(0.0f, 0.0f, 200.0f);
		Mtx mtx;
		MsMtxSetRotRPH(mtx, 0.0f, mRotation.y, 0.0f);
		MTXMultVec(mtx, &offset, &offset);
		unk13C += offset;
		JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToPosPtr(
		    PARTICLE_MS_POI_ZZZ, &unk13C, 1, this);
		if (emitter)
			emitter->setGlobalScale(JGeometry::TVec3<f32>(2.0f, 2.0f, 2.0f));
		SMSGetMSound()->startSoundActor(MSD_SE_EN_AMIKING_SPARK, &mPosition, 0,
		                                nullptr, 0, 4);
	} else {
		SMSGetMSound()->startSoundActor(MSD_SE_EN_AMIKING_FLY, &mPosition, 0,
		                                nullptr, 0, 4);
	}
}

void TAmiKing::bind()
{
	if (checkLiveFlag(LIVE_FLAG_UNK10)) {
		gpMap->checkGround(mPosition.x, mPosition.y + mHeadHeight, mPosition.z,
		                   &mGroundPlane);
	} else {
		TLiveActor::bind();
	}
}

void TAmiKing::touchPlayer(THitActor* param_1)
{
	SMS_SendMessageToMario(this, HIT_MESSAGE_ELECTRIC_SHOCK);
}

void TPinnaCoaster::control()
{
	TMapObjBase::control();
	unk138->frameUpdate();
	unk138->calc();
	getModel()->setBaseTRMtx(unk138->getModel()->getAnmMtx(0));
	getMActor()->frameUpdate();
	getMActor()->calc();

	MtxPtr mtx = getModel()->getAnmMtx(0);
	mPosition.set(mtx[0][3], mtx[1][3], mtx[2][3]);

	f32 speed = JGeometry::TVec3<f32>(mPosition - unk140).length();
	if (switchSnd)
		SMSGetMSound()->startSoundActorWithInfo(MSD_SE_OBJ_JET_COASTER,
		                                        &mPosition, nullptr, speed, 0,
		                                        0, nullptr, 0, 4);
	switchSnd ^= 1;
	unk140.set(mPosition);
}

void TPinnaCoaster::initMapObj()
{
	TMapObjBase::initMapObj();
	unk138 = SMS_MakeMActorWithAnmData(
	    "/scene/mapObj/CoasterRail.bmd", mManager->getMActorAnmData(), 3,
	    J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
	        | (1 << J3DMLF_TevStageNumShift));
	unk138->setBck("coasterrail");

	MsMtxSetXYZRPH(unk138->getModel()->getBaseTRMtx(), mPosition.x, mPosition.y,
	               mPosition.z, mRotation.x, mRotation.y, mRotation.z);
	unk138->getFrameCtrl(ANM_TYPE_BCK)->setRate(SMSGetAnmFrameRate() / 4.0f);
	unk140.set(mPosition);
}

TPinnaCoaster::TPinnaCoaster(const char* param_1)
    : TMapObjBase(param_1)
    , unk138(nullptr)
{
	unk140.zero();
}

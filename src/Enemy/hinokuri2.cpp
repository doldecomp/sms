#include <Enemy/Hinokuri2.hpp>
#include <Enemy/Walker.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/Conductor.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Player/MarioAccess.hpp>
#include <Camera/CameraShake.hpp>
#include <Map/MapCollisionManager.hpp>
#include <MoveBG/ItemManager.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Strategy.hpp>
#include <Strategic/question.hpp>
#include <System/MarDirector.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <M3DUtil/M3UJoint.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/ShadowUtil.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <JSystem/JMath.hpp>
#include <stdlib.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

// NOTE: rodata ordering says this has to go here
#include <Map/MapCollisionEntry.hpp>

static const char* hinokuri2_bastable[] = {
	"/scene/hinokuri2/bas/hinokuri2_appear.bas",
	"/scene/hinokuri2/bas/hinokuri2_close.bas",
	"/scene/hinokuri2/bas/hinokuri2_c_open.bas",
	"/scene/hinokuri2/bas/hinokuri2_c_open_wait.bas",
	"/scene/hinokuri2/bas/hinokuri2_c_to_wait.bas",
	"/scene/hinokuri2/bas/hinokuri2_damage.bas",
	"/scene/hinokuri2/bas/hinokuri2_dive.bas",
	"/scene/hinokuri2/bas/hinokuri2_down.bas",
	nullptr,
	nullptr,
	nullptr,
	"/scene/hinokuri2/bas/hinokuri2_jidanda.bas",
	"/scene/hinokuri2/bas/hinokuri2_jump.bas",
	nullptr,
	"/scene/hinokuri2/bas/hinokuri2_landing.bas",
	nullptr,
	"/scene/hinokuri2/bas/hinokuri2_open.bas",
	nullptr,
	"/scene/hinokuri2/bas/hinokuri2_return.bas",
	"/scene/hinokuri2/bas/hinokuri2_squat.bas",
	nullptr,
	"/scene/hinokuri2/bas/hinokuri2_turn.bas",
	nullptr,
	nullptr,
	"/scene/hinokuri2/bas/hinokuri2_walk.bas",
	nullptr,
	"/scene/hinokuri2/bas/hinokuri2_walk_to_wait_l.bas",
	"/scene/hinokuri2/bas/hinokuri2_walk_to_wait_r.bas",
};

THino2Params::THino2Params(const char* path)
    : TSpineEnemyParams(path)
    , PARAM_INIT(mSLMarchSpeedLv0, 6.0f)
    , PARAM_INIT(mSLMarchSpeedLv1, 4.0f)
    , PARAM_INIT(mSLMarchSpeedLv2, 4.0f)
    , PARAM_INIT(mSLTurnSpeedLv0, 1.5f)
    , PARAM_INIT(mSLTurnSpeedLv1, 1.5f)
    , PARAM_INIT(mSLTurnSpeedLv2, 1.5f)
    , PARAM_INIT(mSLBodyScale, 1.9f)
    , PARAM_INIT(mSLWalkShake, 4000.0f)
    , PARAM_INIT(mSLJumpShake, 4000.0f)
    , PARAM_INIT(mSLGravityY, 0.4f)
    , PARAM_INIT(mSLPrePolWait, 2)
    , PARAM_INIT(mSLPolWaitCount, 2)
    , PARAM_INIT(mSLPolIntervalMin, 2000)
    , PARAM_INIT(mSLPolIntervalMax, 3000)
    , PARAM_INIT(mSLDamageHeadScale, 1.3f)
    , PARAM_INIT(mSLDamageTimer, 1800)
    , PARAM_INIT(mSLHeadHitR, 50.0f)
    , PARAM_INIT(mSLHeadHitH, 20.0f)
    , PARAM_INIT(mSLBodyHitR, 175.0f)
    , PARAM_INIT(mSLBodyHitH, 180.0f)
    , PARAM_INIT(mSLBodyHitR0, 200.0f)
    , PARAM_INIT(mSLBodyHitH0, 0.5f)
    , PARAM_INIT(mSLBankProp, 10.0f)
    , PARAM_INIT(mSLBankLimit, 10.0f)
    , PARAM_INIT(mSLJumpQuakeLen, 2000.0f)
    , PARAM_INIT(mSLStampProb, 0.5f)
    , PARAM_INIT(mSLStampCount, 10)
    , PARAM_INIT(mSLStampQuakeLen, 2000.0f)
    , PARAM_INIT(mSLWaterEmitPos, 1000.0f)
    , PARAM_INIT(mSLHitPointMaxLv0, 1)
    , PARAM_INIT(mSLHitPointMaxLv1, 150)
    , PARAM_INIT(mSLHitPointMaxLv2, 100)
    , PARAM_INIT(mSLFreezeTimerLv0, 1800)
    , PARAM_INIT(mSLInvincibleTimer, 300)
    , PARAM_INIT(mSLWalkSpeedRateLv0, 1.2f)
{
	TParams::load(mPrmPath);
}

THinokuri2Manager::THinokuri2Manager(const char* name)
    : TEnemyManager(name)
{
}

void THinokuri2Manager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "hinokuri2_model.bmd", J3DMLF_MaterialPEFull, 0 },
		{ "hinokuri2_mask_model.bmd", J3DMLF_MaterialPEFull, 0 },
		{ "hinokuri2_mask_model_l.bmd", J3DMLF_MaterialPEFull, 0 },
		{ "hinokuri2_mask_model_r.bmd", J3DMLF_MaterialPEFull, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void THinokuri2Manager::load(JSUMemoryInputStream& stream)
{
	unk38 = new THino2Params("/enemy/hinokuri2.prm");
	TEnemyManager::load(stream);
}

TSpineEnemy* THinokuri2Manager::createEnemyInstance()
{
	return new THinokuri2("ヒノクリ２");
}

THino2MtxCalc::THino2MtxCalc(u16 param_1, u16 param_2, u16 param_3, u16 param_4,
                             u16 param_5, u16 param_6, f32 param_7)
    : TMtxCalcFootInv(param_1, param_2, param_3, param_4, param_5, param_6,
                      param_7)
    , unk78(0.0f)
{
}

static THinokuri2* gpCurHinokuri;

// TODO: nonmatching stack frame (0x130 instead of 0x148); no padding.
void THino2MtxCalc::calc(u16 param_1)
{
	j3dSys.setCurrentMtxCalc(this);

	if (gpCurHinokuri->mLevel == 2 && (param_1 == 0x13 || param_1 == 0x17)) {
		J3DTransformInfo info;

		if (mOne[0]) {
			mOne[0]->getTransform(param_1, &info);
		} else {
			J3DJoint* joint
			    = j3dSys.getModel()->getModelData()->getJointNodePointer(
			        param_1);

			info = joint->getTransformInfo();
		}

		info.mScale.x = 1.0f;
		info.mScale.y = 1.0f;
		info.mScale.z = 1.0f;

		calcTransform(param_1, info);
	} else {

		if (!mOne[0] && !mOne[1]) {
			J3DTransformInfo info;
			J3DJoint* joint
			    = j3dSys.getModel()->getModelData()->getJointNodePointer(
			        param_1);

			info = joint->getTransformInfo();

			calcTransform(param_1, info);
		} else if (unk78 == 0.0f || !mOne[1]) {
			J3DTransformInfo auStack_74;
			mOne[0]->getTransform(param_1, &auStack_74);
			calcTransform(param_1, auStack_74);
		} else if (unk78 == 1.0f || !mOne[0]) {
			J3DTransformInfo auStack_94;
			mOne[1]->getTransform(param_1, &auStack_94);
			calcTransform(param_1, auStack_94);
		} else {
			J3DTransformInfo JStack_d4;
			J3DTransformInfo JStack_b4;
			mOne[0]->getTransform(param_1, &JStack_d4);
			J3DTransformInfo* ptr = &JStack_b4;
			mOne[1]->getTransform(param_1, ptr);
			M3UMtxCalcBlendAux(param_1, &JStack_d4, ptr, unk78, false);
		}
	}

	if (unk6C == param_1) {
		FeetInvCalc(j3dSys.getModel(), unk68, unk6A, unk6C, unk74);
	}

	if (unk72 == param_1) {
		FeetInvCalc(j3dSys.getModel(), unk6E, unk70, unk72, unk74);
	}

	if (param_1 == 0)
		unk78 = MsClamp(unk78 - 0.05f, 0.0f, 1.0f);
}

THino2Hit::THino2Hit(THinokuri2* owner, int joint_idx, const char* name)
    : THitActor(name)
    , mOwner(owner)
    , mJointIdx(joint_idx)
{
}

void THino2Hit::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (!(cue & CUE_MOVE))
		return;

	if (mOwner->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		mPosition = mOwner->mPosition;
		return;
	}

	mOwner->getJointTransByIndex(mJointIdx, &mPosition);

	if (mJointIdx == 0x19)
		mPosition.y -= 140.0f;

	if (!mOwner->unk188)
		return;

	for (int i = 0; i < mColCount; ++i) {
		THitActor* col = mCollisions[i];

		if (mJointIdx != 0x10 && mJointIdx != 0x9)
			continue;

		if (col->isActorType(ACTOR_TYPE_MARIO)) {
			col->receiveMessage(this, HIT_MESSAGE_ATTACK);
		} else if ((col->isActorType(ACTOR_TYPE_NAME_KURI)
		            || col->isActorType(ACTOR_TYPE_HAMU_KURI))
		           && mOwner->getMActor()->checkCurBckFromIndex(0xE)) {
			col->receiveMessage(this, HIT_MESSAGE_HIP_DROP);
		}
	}
}

BOOL THino2Hit::receiveMessage(THitActor* sender, u32 message)
{
	if (mJointIdx == 0x19) {
		if (sender->getActorType() == ACTOR_TYPE_MARIO) {
			mOwner->mJointIdxMessageCameFrom = mJointIdx;
			return mOwner->receiveMessage(sender, message);
		}
		if (sender->getActorType() == ACTOR_TYPE_WATER
		    && message == HIT_MESSAGE_SPRAYED_BY_WATER) {
			mOwner->mJointIdxMessageCameFrom = mJointIdx;
			return mOwner->receiveMessage(sender, message);
		}
		if (sender->getActorType() == ACTOR_TYPE_WOOD_BARREL) {
			mOwner->mJointIdxMessageCameFrom = mJointIdx;
			return mOwner->receiveMessage(sender, message);
		}
	}

	if (mJointIdx == 0x13) {
		if (sender->getActorType() == ACTOR_TYPE_WATER
		    && message == HIT_MESSAGE_SPRAYED_BY_WATER) {
			mOwner->mJointIdxMessageCameFrom = mJointIdx;
			return mOwner->receiveMessage(sender, message);
		}
		if (sender->getActorType() == ACTOR_TYPE_WOOD_BARREL) {
			mOwner->mJointIdxMessageCameFrom = mJointIdx;
			return mOwner->receiveMessage(sender, message);
		}
	}

	return false;
}

THino2Mask::THino2Mask(THinokuri2* param_1)
    : unk0(param_1)
    , unk4(1)
    , unk8(0)
    , unkC(0)
    , unk10(nullptr)
    , unk14(nullptr)
    , unk18(nullptr)
{
	TMActorKeeper* keeper = unk0->getActorKeeper();

	unk10 = keeper->createMActor("hinokuri2_mask_model.bmd", 0);
	unk14 = keeper->createMActor("hinokuri2_mask_model_l.bmd", 0);
	unk18 = keeper->createMActor("hinokuri2_mask_model_r.bmd", 0);
	unk10->setBtp("hinokuri2_clack");
}

void THino2Mask::setMatrix(MtxPtr mtx)
{
	if (unk4 != 2)
		MTXCopy(mtx, unk4C);
}

void THino2Mask::breakMask() { }

void THino2Mask::startDamageMotion() { }

// TODO: nonmatching cached animation-cue register (r28 instead of r26).
void THino2Mask::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (unk4 == 0)
		return;

	if (unk4 == 1) {
		if (cue & CUE_CALC_ANIM) {
			unk10->getModel()->setBaseTRMtx(unk4C);
			J3DFrameCtrl* ctrl = unk10->getFrameCtrl(ANM_TYPE_BTP);
			ctrl->setFrame(unkC);
			ctrl->setRate(0.0f);
			unk10->resetDL();
		}
		unk10->perform(cue, graphics);
	} else {
		if (cue & CUE_CALC_ANIM) {
			if (unk8 > 240) {
				unk4 = 0;
				return;
			}

			f32 gravity = unk0->getSaveParams()->mSLGravityY.get();

			unk40.x -= gravity;
			unk34.x -= gravity;

			unk28 += unk40;
			unk1C += unk34;

			++unk8;
		}

		if (cue & CUE_CALC_ANIM) {
			Mtx afStack_58;
			Mtx afStack_88;
			MtxPtr mtx;
			MTXIdentity(afStack_58);
			afStack_58[0][3] = unk28.x;
			afStack_58[1][3] = unk28.y;
			afStack_58[2][3] = unk28.z;
			MsMtxSetRotRPH(afStack_88, 0.0f, 360.0f - unk8, 0.0f);
			MtxPtr base = getUnk4C();
			MTXConcat(base, afStack_58, afStack_58);
			MTXConcat(afStack_58, afStack_88, afStack_58);
			mtx = afStack_58;
			unk14->getModel()->setBaseTRMtx(mtx);

			MTXIdentity(mtx);
			mtx[0][3] = unk1C.x;
			mtx[1][3] = unk1C.y;
			mtx[2][3] = unk1C.z;
			MsMtxSetRotRPH(afStack_88, 0.0f, unk8, 0.0f);
			MTXConcat(base, mtx, mtx);
			MTXConcat(mtx, afStack_88, mtx);
			unk18->getModel()->setBaseTRMtx(mtx);
		}

		if ((cue & CUE_ENTRY) && unk8 > 60 && unk8 % 6 >= 3)
			cue &= ~CUE_ENTRY;

		unk14->perform(cue, graphics);
		unk18->perform(cue, graphics);
	}
}

// TODO: frame is 0xe0 instead of 0x100; joint-index register and yaw-constant
// scheduling also differ. No fabricated padding.
static int Hino2HeadCallback(J3DNode* param_1, int param_2)
{
	if (param_2 == 0) {
		MtxPtr mA = gpCurHinokuri->getModel()->getAnmMtx(
		    ((J3DJoint*)param_1)->getJntNo());

		if (gpCurHinokuri->getLevel() == 1) {
			TPosition3f local_44;
			local_44.setTrans(0.0f, 0.0f, 0.0f);
			f32 scale = gpCurHinokuri->unk194;
			local_44.setScale(scale, scale, scale);

			Mtx local_74;
			MtxPtr mB = local_74;
			MsMtxSetRotY(mB, gpCurHinokuri->unk198);

			MTXConcat(mA, mB, mA);
			MTXConcat(mA, local_44, mA);
			MTXConcat(J3DSys::mCurrentMtx, mB, J3DSys::mCurrentMtx);
			MTXConcat(J3DSys::mCurrentMtx, local_44, J3DSys::mCurrentMtx);
		} else {
			Mtx local_a4;
			MtxPtr mB = local_a4;
			MsMtxSetRotY(mB, gpCurHinokuri->unk198);

			MTXConcat(mA, mB, mA);
			MTXConcat(J3DSys::mCurrentMtx, mB, J3DSys::mCurrentMtx);
		}
	}

	return 1;
}

THinokuri2::THinokuri2(const char* name)
    : TSpineEnemy(name)
    , unk150(0)
    , mCurrentBck(0xffffffff)
    , unk158(0)
    , unk15C(0)
    , mWaitTimer(0)
    , unk164(0)
    , unk168(0)
    , mHead(nullptr)
    , mBody(nullptr)
    , unk174(nullptr)
    , unk178(nullptr)
    , unk180(0)
    , unk184(0)
    , unk188(0)
    , unk18C(0)
    , unk190(0)
    , unk194(1.0f)
    , unk198(0.0f)
    , unk19C(nullptr)
    , unk1A0(nullptr)
    , unk1A4(0)
    , mLevel(0)
{
	mBinder = new TWalker;
	mHead   = new THino2Hit(this, 25, "ヒノクリ２ヒットオブジェクト");
	mBody   = new THino2Hit(this, 19, "ヒノクリ２ヒットオブジェクト");
	unk174  = new THino2Hit(this, 9, "ヒノクリ２ヒットオブジェクト");
	unk178  = new THino2Hit(this, 16, "ヒノクリ２ヒットオブジェクト");
	unk19C  = new TWaterEmitInfo("/enemy/hinokuri2water.prm");
	unk1A0  = new THino2MtxCalc(5, 6, 9, 12, 13, 16, 15.0f);
}

void THinokuri2::init(TLiveManager* param_1)
{
	mManager = param_1;
	mManager->manageActor(this);
	mMActorKeeper = new TMActorKeeper(mManager, 4);
	mMActor       = mMActorKeeper->createMActor("hinokuri2_model.bmd", 0);
	unk1A4        = new THino2Mask(this);
	unk124->init(gpConductor->getGraphByName("hinokuri"));
	mSpine->initWith(&TNerveHino2Appear::theNerve());
	reset();
	mGravity = ((THino2Params*)getSaveParam())->mSLGravityY.get();
	initAnmSound();
	initHitActor(ACTOR_TYPE_HINOKURI2, 5, 0, mBodyRadius, mHeadHeight,
	             mBodyRadius, mHeadHeight);
	unk150 = new TMBindShadowBody(this, getModel(), 1.0f);

	TIdxGroupObj* enemiesGrp
	    = static_cast<TIdxGroupObj*>(JDrama::TNameRefGen::search("敵グループ"));
	enemiesGrp->getChildren().push_back(mHead);
	enemiesGrp->getChildren().push_back(mBody);
	enemiesGrp->getChildren().push_back(unk178);
	enemiesGrp->getChildren().push_back(unk174);

	mHead->initHitActor(ACTOR_TYPE_HINOKURI2, 5,
	                    HIT_CATEGORY_MAP_OBJECT | HIT_CATEGORY_WATER,
	                    mHead->getAttackRadius(), mHead->getAttackHeight(),
	                    mHead->getDamageRadius(), mHead->getDamageHeight());
	mBody->initHitActor(ACTOR_TYPE_HINOKURI2, 5,
	                    HIT_CATEGORY_PLAYER | HIT_CATEGORY_MAP_OBJECT
	                        | HIT_CATEGORY_WATER,
	                    mHead->getAttackRadius(), mHead->getAttackHeight(),
	                    mHead->getDamageRadius(), mHead->getDamageHeight());
	unk174->initHitActor(ACTOR_TYPE_HINOKURI2, 5,
	                     HIT_CATEGORY_PLAYER | HIT_CATEGORY_ENEMY, 70.0f,
	                     120.0f, 70.0f, 120.0f);
	unk178->initHitActor(ACTOR_TYPE_HINOKURI2, 5,
	                     HIT_CATEGORY_PLAYER | HIT_CATEGORY_ENEMY, 70.0f,
	                     120.0f, 70.0f, 120.0f);

	validateCollisionAll();

	getMActor()->setJointCallback(0x17, &Hino2HeadCallback);

	if (SMSGetMarDirector()->mMap == 1) {
		switch (SMSGetMarDirector()->unk7D) {
		case 4:
			setLevel(2);
			break;
		case 0:
		case 1:
		case 2:
		case 3:
		case 5:
		case 6:
		case 7:
		default:
			setLevel(1);
			break;
		}
	} else if (SMSGetMarDirector()->mMap == 0) {
		switch (SMSGetMarDirector()->unk7D) {
		case 7:
			setLevel(2);
			break;
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		default:
			setLevel(1);
			break;
		}
	} else {
		setLevel(getInstanceIndex());
	}

	mHitPoints = calcHitPoints();

	J3DMtxCalc* calc = unk1A0;
	if (getMActor()->getAnmBck()) {
		getMActor()->getAnmBck()->setCalc(calc);
	}
}

void THinokuri2::reset()
{
	TSpineEnemy::reset();
	onLiveFlag(LIVE_FLAG_UNK8);
	resetPolInterval();
	mHitPoints = calcHitPoints();
	validateCollisionAll();
	unk190 = 0;
}

void THinokuri2::kill()
{
	invalidateCollisionAll();
	TSpineEnemy::kill();
}

void THinokuri2::emitPolParticle()
{
	unk15C = 0;

	JGeometry::TVec3<f32> local_60;
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		local_60 = mPosition;
		local_60.y += 500.0f;
	} else {
		getJointTransByIndex(0x18, &local_60);
	}
}

void THinokuri2::stopPolParticle() { }

void THinokuri2::updatePolTrans() { }

// TODO: fake & wrong
template <class T> static inline T randy(T l, T r)
{
	r -= l;
	T k = (1.f / (RAND_MAX + 1)) * rand() * r;
	l += k;
	return l;
}

void THinokuri2::resetPolInterval()
{
	int l  = ((THino2Params*)getSaveParam())->mSLPolIntervalMin.value;
	int r  = ((THino2Params*)getSaveParam())->mSLPolIntervalMax.value;
	unk164 = randy(l, r);
}

// TODO: are validate & invalidate reversed?

void THinokuri2::invalidateCollisionAll()
{
	onHitFilter(HIT_FILTER_NO_COLLISION);
	mHead->onHitFilter(HIT_FILTER_NO_COLLISION);
	mBody->onHitFilter(HIT_FILTER_NO_COLLISION);
	unk174->onHitFilter(HIT_FILTER_NO_COLLISION);
	unk178->onHitFilter(HIT_FILTER_NO_COLLISION);
}

void THinokuri2::validateCollisionAll()
{
	onHitFilter(HIT_FILTER_NO_COLLISION);
	mHead->offHitFilter(HIT_FILTER_NO_COLLISION);
	mBody->offHitFilter(HIT_FILTER_NO_COLLISION);
	unk174->offHitFilter(HIT_FILTER_NO_COLLISION);
	unk178->offHitFilter(HIT_FILTER_NO_COLLISION);
}

void THinokuri2::emitWaterParticle()
{
	if (!unk19C)
		return;

	JGeometry::TVec3<f32> position;
	if (getLevel() >= 1) {
		getJointTransByIndex(0x19, &position);
	} else {
		position = getPosition();
		position.y += getSaveParams()->mSLWaterEmitPos.get();
	}
	unk19C->mPos.value = position;
	gpModelWaterManager->emitRequest(*unk19C);
}

// TODO: UNUSED body is 0xa4 bytes; mario.MAP records 0xa8.
void THinokuri2::shakeCamera(int param_1)
{
	if (param_1 == 0) {
		f32 ws = getSaveParams()->mSLWalkShake.get();
		if (ws * ws < getDistToMarioSquared())
			return;
		gpCameraShake->startShake(CAM_SHAKE_MODE_UNK3, 0.8f);
	} else {
		f32 js = getSaveParams()->mSLJumpShake.get();
		if (js * js < getDistToMarioSquared())
			return;
		gpCameraShake->startShake(CAM_SHAKE_MODE_UNK4, 0.8f);
	}
}

void THinokuri2::makeQuake(f32) { }

void THinokuri2::setLevel(int level)
{
	if (level < 0)
		level = 0;
	else if (level >= 3)
		level = 2;
	mLevel = level;

	// BUG: uninitialized in case prms is nullptr
	// (which shouldn't really ever happen but still)
	const char* graphName;

	if (THino2Params* prms = ((THino2Params*)getSaveParam())) {
		switch (mLevel) {
		case 0:
			graphName   = "hinokuri";
			mMarchSpeed = prms->mSLMarchSpeedLv0.get();
			mTurnSpeed  = prms->mSLTurnSpeedLv0.get();
			break;
		case 1:
			graphName   = "hinokuri";
			mMarchSpeed = prms->mSLMarchSpeedLv1.get();
			mTurnSpeed  = prms->mSLTurnSpeedLv1.get();
			break;
		case 2:
		default:
			graphName   = "hinokuri";
			mMarchSpeed = prms->mSLMarchSpeedLv2.get();
			mTurnSpeed  = prms->mSLTurnSpeedLv2.get();
			break;
		}
		mBodyScale = prms->mSLBodyScale.get();
		mScaling.set(mBodyScale, mBodyScale, mBodyScale);
	}

	if (TGraphWeb* graph = gpConductor->getGraphByName(graphName)) {
		unk124->init(graph);
		unk124->reset();
		goToShortestNextGraphNode();
	}
}

void THinokuri2::generateEnemy()
{
	JGeometry::TVec3<f32> local_50;
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		local_50 = mPosition;
		local_50.y += 100.0f;
	} else {
		getJointTransByIndex(0x14, &local_50);
	}

	static const char* mannameTable[] = {
		"ナメクリマネージャー",
		"ハムクリマネージャー",
	};

	gpConductor->makeEnemyAppear(
	    local_50,
	    mannameTable[rand() * (1.0f / (RAND_MAX + 1.0f)) > 0.5f ? 0 : 1], 1, 0);
}

void THinokuri2::updateAnmSound()
{
	TSpineEnemy::updateAnmSound();
	if (unk158 > 0) {
		if (SMSGetMSound()->gateCheck(MSD_SE_BS_HINO_SEED_LQ_LEV))
			MSoundSESystem::MSoundSE::startSoundActor(
			    MSD_SE_BS_HINO_SEED_LQ_LEV, getPosition(), 0, nullptr, 0, 4);
	}
}

void THinokuri2::changeBck(int param_1)
{
	if (param_1 < 0)
		return;

	int curBck = getMActor()->getCurAnmIdx(ANM_TYPE_BCK);
	if (param_1 != curBck) {
		if (curBck == 0x16 && param_1 == 0x18
		    || curBck == 0x18 && param_1 == 0x16
		    || curBck == 0x18 && param_1 == 0x9
		    || curBck == 0xE && param_1 == 0x18
		    || curBck == 0x16 && param_1 == 0xB
		    || curBck == 0xB && param_1 == 0x18) {
			unk1A0->addTransform(
			    mMActorKeeper->getMActorAnmData()->getUnk2C()->getAnmPtr(
			        param_1));
		} else {
			unk1A0->setAnmTransform(
			    mMActorKeeper->getMActorAnmData()->getUnk2C()->getAnmPtr(
			        param_1));
		}

		getMActor()->getAnmBck()->setFrameCtrl(param_1);
	}

	mCurrentBck = param_1;
	setAnmSound(hinokuri2_bastable[mCurrentBck]);
	J3DFrameCtrl* pJVar7 = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
	if (pJVar7 != nullptr) {
		if (mLevel == 0 && (param_1 - 23U <= 1 || param_1 - 26U <= 1))
			pJVar7->setRate(
			    ((THino2Params*)getSaveParam())->mSLWalkSpeedRateLv0.value);
		else
			pJVar7->setRate(1.0f);
	}
}

BOOL THinokuri2::receiveMessageLv0(THitActor* sender, u32 message)
{
	if (mJointIdxMessageCameFrom == 0x13
	    && sender->getActorType() == ACTOR_TYPE_WATER
	    && message == HIT_MESSAGE_SPRAYED_BY_WATER)
		return true;

	if (sender->getActorType() == ACTOR_TYPE_WATER
	    && message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		if (mJointIdxMessageCameFrom != 0x19)
			return true;

		if (mSpine->getCurrentNerve() != &TNerveHino2Freeze::theNerve())
			mSpine->pushNerve(&TNerveHino2Freeze::theNerve());
	}

	if (sender->getActorType() == ACTOR_TYPE_MARIO
	    && message == HIT_MESSAGE_TRAMPLE) {
		if (mJointIdxMessageCameFrom != 0x19)
			return true;

		mHitPoints = 0;
		if (mSpine->getCurrentNerve() != &TNerveHino2Die::theNerve()) {
			mSpine->reset();
			mSpine->setNext(&TNerveHino2Die::theNerve());
		}
	}

	return false;
}

// TODO: frame-only mismatch (0x68 instead of 0x78); no fabricated padding.
BOOL THinokuri2::receiveMessageLv1(THitActor* sender, u32 message)
{
	if (mJointIdxMessageCameFrom == 0x13
	    && sender->getActorType() == ACTOR_TYPE_WATER
	    && message == HIT_MESSAGE_SPRAYED_BY_WATER)
		return true;

	if (unk180 && sender->getActorType() == ACTOR_TYPE_WATER
	    && message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		if (mJointIdxMessageCameFrom != 0x19)
			return true;

		int dmgAmount
		    = gpModelWaterManager->getParticleAttack((TWaterHitActor*)sender);

		if (dmgAmount <= 0)
			return true;

		int hitPoints = mHitPoints;
		if (dmgAmount >= hitPoints)
			mHitPoints = 0;
		else
			mHitPoints = hitPoints - dmgAmount;

		++unk18C;

		if (mSpine->getCurrentNerve() != &TNerveHino2Damage::theNerve())
			mSpine->setNext(&TNerveHino2Damage::theNerve());

		return true;
	}

	if (unk180 && sender->getActorType() == ACTOR_TYPE_WOOD_BARREL) {
		if (mJointIdxMessageCameFrom != 0x19)
			return true;

		mHitPoints = 0;

		if (mSpine->getCurrentNerve() != &TNerveHino2Damage::theNerve()) {
			mSpine->setNext(&TNerveHino2Damage::theNerve());
		}

		return true;
	}

	return false;
}

BOOL THinokuri2::receiveMessageLv2(THitActor* sender, u32 message)
{
	if (sender->getActorType() == ACTOR_TYPE_WATER
	    && message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		if (mJointIdxMessageCameFrom != 0x13)
			return true;

		int dmgAmount
		    = gpModelWaterManager->getParticleAttack((TWaterHitActor*)sender);

		if (dmgAmount <= 1)
			return true;

		unk190 += 1;
		unk1A4->unkC += 1;

		unk168 = ((THino2Params*)getSaveParam())->mSLInvincibleTimer.get();

		if (unk190 > 2) {
			unk190 = 0;
			setLevel(mLevel - 1);

			unk1A4->reset();

			mHitPoints = calcHitPoints();
		}

		return true;
	}

	if (sender->getActorType() == ACTOR_TYPE_WOOD_BARREL) {
		if (mJointIdxMessageCameFrom != 0x13)
			return true;

		unk190 += 1;
		unk1A4->unkC += 1;

		unk168 = ((THino2Params*)getSaveParam())->mSLInvincibleTimer.get();

		if (unk190 > 2) {
			unk190 = 0;
			setLevel(mLevel - 1);

			unk1A4->reset();

			mHitPoints = calcHitPoints();
		} else {
			unk1A4->unk10->setBckFromIndex(0xF);
		}

		return true;
	}

	return false;
}

BOOL THinokuri2::receiveMessage(THitActor* sender, u32 message)
{
	if (unk168)
		return 0;

	switch (mLevel) {
	case 0:
		return receiveMessageLv0(sender, message);
		break;
	case 1:
		return receiveMessageLv1(sender, message);
		break;
	case 2:
		return receiveMessageLv2(sender, message);
		break;
	}

	return 0;
}

// TODO: nonmatching frame (0xf0 instead of 0x178); no fabricated padding.
void THinokuri2::moveObject()
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;

	if (unk164 > 0)
		--unk164;

	if (mLevel == 1) {
		f32 dhp    = calcHitPoints() - mHitPoints;
		f32 fVar12 = 1.0f
		             + (getSaveParams()->getSLDamageHeadScale() - 1.0f)
		                   * (dhp / calcHitPoints())
		             - unk194;

		unk194
		    += fVar12 > 0.0f ? MsMin(fVar12, 0.004f) : MsMax(fVar12, -0.004f);
	} else {
		unk194 = 1.0f;
	}

	if (SMSGetMarDirector()->mMoveTickCount % 600 == 0)
		generateEnemy();

	doShortCut();
	mPositionDelta.zero();
	mRotationDelta.zero();
	if (mMapCollisionManager != nullptr) {
		JGeometry::TVec3<f32> aTStack_30;
		getJointTransByIndex(0x17, &aTStack_30);
		mMapCollisionManager->moveActiveCollisionTrans(aTStack_30);
	}

	f32 headHitR = getSaveParams()->mSLHeadHitR.value;
	mHead->setDamageRadius(unk194 * (headHitR * mBodyScale));
	f32 headHitH = getSaveParams()->mSLHeadHitH.value;
	mHead->setDamageHeight(unk194 * (headHitH * mBodyScale));

	if (mLevel == 0) {
		f32 bodyHitR0 = getSaveParams()->mSLBodyHitR0.value;
		mBody->setDamageRadius(unk194 * (bodyHitR0 * mBodyScale));
		f32 bodyHitH0 = getSaveParams()->mSLBodyHitH0.value;
		mBody->setDamageHeight(unk194 * (bodyHitH0 * mBodyScale));
	} else {
		f32 bodyHitR = getSaveParams()->mSLBodyHitR.value;
		mBody->setDamageRadius(unk194 * (bodyHitR * mBodyScale));
		f32 bodyHitH = getSaveParams()->mSLBodyHitH.value;
		mBody->setDamageHeight(unk194 * (bodyHitH * mBodyScale));
	}

	updateSquareToMario();

	f32 oldRot = mRotation.y;

	control();
	bind();

	mPosition += mPositionDelta;
	mRotation += mRotationDelta;

	unk198
	    += -MsClamp(MsWrap(MsAngleDiff(mRotation.y, oldRot), -180.0f, 180.0f),
	                -30.0f, 30.0f)
	       * getSaveParams()->mSLBankProp.get();

	unk198 = MsWrap(unk198, -180.0f, 180.0f);

	f32 prop = getSaveParams()->mSLBankLimit.get();
	unk198   = MsClamp(unk198, -prop, prop);

	// looks like symmetric_clamp but it isn't???
	if (unk198 < 0.0f) {
		f32 fVar1 = unk198;
		fVar1 += 0.1f;
		if (fVar1 > 0.0f)
			fVar1 = 0.0f;
		unk198 = fVar1;
	} else {
		f32 fVar1 = unk198 - 0.1f;
		if (fVar1 < 0.0f)
			fVar1 = 0.0f;
		unk198 = fVar1;
	}

	if (unk168 > 0)
		--unk168;
}

// TODO: the question-request vector is at 0x30 instead of 0x34;
// the 0x58-byte frame and saved-register offsets match.
void THinokuri2::perform(u32 cue, JDrama::TGraphics* graphics)
{
	gpCurHinokuri = this;

	mHead->perform(cue, graphics);
	mBody->perform(cue, graphics);
	unk174->perform(cue, graphics);
	unk178->perform(cue, graphics);

	if (cue & CUE_ENTRY) {
		mMActor->resetDL();
		for (u16 i = 2; i < 5; ++i) {
			J3DShape* shape
			    = getModel()->getModelData()->getShapeNodePointer(i);
			if (shape) {
				if (mLevel == 0 || mLevel == 2)
					shape->onFlag(J3DShpFlag_Visible);
				else
					shape->offFlag(J3DShpFlag_Visible);
			}
		}
	}

	if (cue & CUE_CALC_ANIM) {
		calcRootMatrix();
		getMActor()->frameUpdate();
		if (!checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_CLIPPED_OUT)) {
			getMActor()->updateIn();
			getModel()->calc();
			getMActor()->updateOut();
		}
	} else {
		TSpineEnemy::perform(cue, graphics);
	}

	bool inactive = checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_CLIPPED_OUT);
	if (!inactive) {
		if (mLevel == 2 || unk1A4->unk4 == 2) {
			if (cue & CUE_CALC_ANIM) {
				unk1A4->setMatrix(getModel()->getAnmMtx(0x17));
			}
			unk1A4->perform(cue, graphics);
		}

		if (cue & CUE_CALC_VIEW) {
			unk150->entryDrawShadow();
			gpQuestionManager->request(mPosition, mScaledBodyRadius);
		}
	}
}

DEFINE_NERVE(TNerveHino2Appear, TLiveActor)
{
	THinokuri2* self = (THinokuri2*)spine->getBody();

	if (spine->getTime() == 0)
		self->changeBck(0);

	if (self->getMActor()->curAnmEndsNext()) {
		self->goToExclusiveNextGraphNode();
		spine->pushAfterCurrent(&TNerveHino2GraphWander::theNerve());
		return true;
	}

	return false;
}

// TODO: nonmatching stack frame and local offsets; no fabricated padding.
DEFINE_NERVE(TNerveHino2GraphWander, TLiveActor)
{
	THinokuri2* self = (THinokuri2*)spine->getBody();

	self->unk188 = 1;

	if (self->isReachedToGoal()) {
		if (self->jumpToNextGraphNode() >= 0) {
			spine->pushAfterCurrent(&TNerveHino2GraphWander::theNerve());
			spine->pushAfterCurrent(&TNerveHino2JumpIn::theNerve());
			spine->pushAfterCurrent(&TNerveHino2Turn::theNerve());

			return true;
		} else {
			self->goToExclusiveNextGraphNode();
			spine->pushAfterCurrent(&TNerveHino2GraphWander::theNerve());
			return true;
		}
	}

	if (SMSGetMarDirector()->unk7D >= 2 && self->unk164 <= 0) {
		if (self->getLevel() >= 1) {
			spine->pushAfterCurrent(&TNerveHino2PrePol::theNerve());
			return 1;
		}

		self->emitPolParticle();
		self->resetPolInterval();
	}

	if (spine->getTime() == 0)
		self->changeBck(0x18);

	if (self->getCurrentBck() == 0x18) {
		self->walkToCurPathNode(self->getMarchSpeed(), self->getTurnSpeed(),
		                        0.0f);
	}

	int frame = self->getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame();
	if (self->getLevel() != 0 && !self->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)
	    && !self->isAirborne() && (frame == 0x24 || frame == 0x55)) {
		f32 ws = ((THino2Params*)self->getSaveParam())->mSLWalkShake.get();
		if (!(ws * ws < self->getDistToMarioSquared()))
			gpCameraShake->startShake(CAM_SHAKE_MODE_UNK3, 0.8f);

		JGeometry::TVec3<f32> TStack_3C;
		if (frame == 0x24)
			self->getJointTransByIndex(0x9, &TStack_3C);
		else if (frame == 0x55)
			self->getJointTransByIndex(0x10, &TStack_3C);
	}

	return false;
}

DEFINE_NERVE(TNerveHino2Fly, TLiveActor)
{
	THinokuri2* self = (THinokuri2*)spine->getBody();

	if (spine->getTime() == 0)
		self->changeBck(0xC);

	if (self->getMActor()->curAnmEndsNext() && self->getCurrentBck() != 8)
		self->changeBck(0x8);

	if (self->isReachedToGoal()
	    || (spine->getTime() > 0 && !self->isAirborne())) {
		spine->pushAfterCurrent(&TNerveHino2Landing::theNerve());
		return true;
	}

	return false;
}

// TODO: getPoint's subobject address and return-copy registers differ; the
// velocity result is at 0x30 rather than 0x34, and the frame is 0x58, not 0x68.
DEFINE_NERVE(TNerveHino2JumpIn, TLiveActor)
{
	THinokuri2* self = (THinokuri2*)spine->getBody();

	if (spine->getTime() == 0)
		self->changeBck(0x9);

	if (self->getMActor()->curAnmEndsNext()) {
		const JGeometry::TVec3<f32>& p = self->unk104.getPoint();
		f32 f                          = self->unk124->unkC;
		self->mVelocity
		    = self->calcVelocityToJumpToY(p, f, self->getGravityY());
		self->onLiveFlag(LIVE_FLAG_AIRBORNE);
		spine->pushAfterCurrent(&TNerveHino2Fly::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveHino2Landing, TLiveActor)
{
	THinokuri2* self = (THinokuri2*)spine->getBody();

	if (spine->getTime() == 0) {
		self->changeBck(0xE);
		f32 js = ((THino2Params*)self->getSaveParam())->mSLJumpShake.get();
		if (!(js * js < self->getDistToMarioSquared()))
			gpCameraShake->startShake(CAM_SHAKE_MODE_UNK4, 0.8f);
	}

	// TODO: asserts or something? Hard to match
	self->getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame();
	self->checkLiveFlag(LIVE_FLAG_HIDDEN);

	if (self->getMActor()->curAnmEndsNext())
		return true;

	return false;
}

DEFINE_NERVE(TNerveHino2Turn, TLiveActor)
{
	THinokuri2* self = (THinokuri2*)spine->getBody();

	JGeometry::TVec3<f32> posDiff = self->getUnkF4().getPoint();

	posDiff -= self->mPosition;

	f32 angleDiff = MsAngleDiff(
	    MsWrap(MsGetRotFromZaxisY(posDiff), 0.0f, 360.0f), self->mRotation.y);

	if (spine->getTime() == 0 && fabsf(angleDiff) > 15.0f
	    && self->mCurrentBck != 0x15) {
		self->changeBck(0x15);
	}

	f32 turnSpeed = self->getTurnSpeed();
	f32 turn;
	if (angleDiff > 0.0f)
		turn = MsMin(angleDiff, turnSpeed);
	else
		turn = MsMax(angleDiff, -turnSpeed);

	self->mRotation.y = MsWrap(self->mRotation.y + turn, 0.0f, 360.0f);

	if (fabsf(turn) < turnSpeed * 0.5f)
		return true;

	return false;
}

DEFINE_NERVE(TNerveHino2PrePol, TLiveActor)
{
	THinokuri2* self = (THinokuri2*)spine->getBody();

	if (spine->getTime() == 0)
		self->changeBck(0x16);

	if (self->getCurrentBck() == 0x16) {
		if (self->getMActor()->curAnmEndsNext()) {
			int uVar5 = self->mWaitTimer;
			uVar5 += 1;

			int wait = self->getSaveParams()->mSLPrePolWait.get();
			if (uVar5 > wait) {
				f32 prob = self->getSaveParams()->mSLStampProb.get();
				if (rand() * (1.0f / (RAND_MAX + 1)) < prob) {
					spine->pushAfterCurrent(&TNerveHino2Stamp::theNerve());
				} else {
					spine->pushAfterCurrent(&TNerveHino2Pollute::theNerve());
				}

				self->mWaitTimer = 0;
				return true;
			}

			self->mWaitTimer = uVar5;
		}
		return false;
	}

	return false;
}

// TODO: nonmatching frame (0xa0 instead of 0xe8) and local stack offsets;
// the missing inline temporaries are unresolved. No fabricated padding.
DEFINE_NERVE(TNerveHino2Pollute, TLiveActor)
{
	THinokuri2* self = (THinokuri2*)spine->getBody();

	if (spine->getTime() == 0) {
		self->changeBck(0x2);
		return 0;
	}

	if (self->mCurrentBck == 2) {
		if (self->getMActor()->curAnmEndsNext()) {
			self->changeBck(3);
			self->unk180 = TRUE;
		}
		return false;
	}

	if (self->mCurrentBck == 3) {
		if (self->getMActor()->curAnmEndsNext()) {
			int uVar1 = self->mWaitTimer;
			++uVar1;
			int polWait
			    = ((THino2Params*)self->getSaveParam())->mSLPolWaitCount.get();
			if (uVar1 > polWait) {
				self->unk180 = FALSE;
				self->changeBck(16);
				uVar1 = 0;
			}
			self->mWaitTimer = uVar1;
		}
		return false;
	}

	if (self->mCurrentBck == 16) {
		if (self->getMActor()->curAnmEndsNext()) {
			self->changeBck(17);
			self->emitPolParticle();
		}
		return false;
	}

	if (self->mCurrentBck == 17) {
		if (self->getMActor()->curAnmEndsNext()) {
			self->changeBck(1);
		}
		return false;
	}

	if (self->mCurrentBck == 1 && self->getMActor()->curAnmEndsNext()) {
		self->mHitPoints = self->calcHitPoints();

		self->resetPolInterval();
		spine->pushAfterCurrent(&TNerveHino2GraphWander::theNerve());

		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveHino2Damage, TLiveActor)
{
	THinokuri2* self = (THinokuri2*)spine->getBody();

	if (spine->getTime() == 0) {
		self->unk180 = TRUE;
		self->changeBck(0x5);
	}

	if (self->getMActor()->curAnmEndsNext()) {
		if (self->mCurrentBck == 5)
			self->changeBck(2);
		if (self->mCurrentBck == 2)
			self->changeBck(3);
	}

	if (self->unk18C > 0) {
		if (self->mCurrentBck != 5)
			self->changeBck(5);

		self->unk18C = 0;
		if (self->mHitPoints == 0) {
			self->unk180 = FALSE;
			spine->pushAfterCurrent(&TNerveHino2Squat::theNerve());
			return true;
		}
	}

	if (spine->getTime()
	    >= ((THino2Params*)self->getSaveParam())->getSLDamageTimer()) {
		self->unk180 = FALSE;
		if (self->getHitPoints() == 0) {
			spine->pushAfterCurrent(&TNerveHino2Squat::theNerve());
			return true;
		}

		if (self->getCurrentBck() != 4)
			self->changeBck(4);

		if (self->getMActor()->curAnmEndsNext() && self->getCurrentBck() == 4) {
			self->unk124->reset();
			self->goToShortestNextGraphNode();
			spine->reset();
			spine->pushAfterCurrent(&TNerveHino2GraphWander::theNerve());

			self->mHitPoints = self->calcHitPoints();
			self->resetPolInterval();

			return true;
		}
	}

	return false;
}

DEFINE_NERVE(TNerveHino2Squat, TLiveActor)
{
	THinokuri2* self = (THinokuri2*)spine->getBody();

	self->unk188 = 0;

	if (spine->getTime() == 0)
		self->changeBck(0x13);

	if (self->getMActor()->curAnmEndsNext()) {
		if (self->mCurrentBck == 0x13) {
			self->shakeCamera(1);
			self->changeBck(0x14);
		}

		if (self->mCurrentBck == 0x14) {
			spine->pushAfterCurrent(&TNerveHino2Burst::theNerve());
			return true;
		}
	}

	return false;
}

DEFINE_NERVE(TNerveHino2Burst, TLiveActor)
{
	THinokuri2* self = (THinokuri2*)spine->getBody();

	if (spine->getTime() == 0) {
		self->changeBck(0xA);
		self->emitWaterParticle();
	}

	if (self->getMActor()->curAnmEndsNext()) {
		self->setLevel(self->mLevel - 1);
		self->mHitPoints = self->calcHitPoints();
		self->unk124->reset();
		self->goToShortestNextGraphNode();
		spine->reset();
		spine->pushAfterCurrent(&TNerveHino2GraphWander::theNerve());
		return true;
	}

	return false;
}

// TODO: stack-only mismatch; the coin position is at 0x68 instead of 0x64.
// The frame and water-emission position match. No fabricated padding.
DEFINE_NERVE(TNerveHino2Die, TLiveActor)
{
	THinokuri2* self = (THinokuri2*)spine->getBody();
	if (spine->getTime() == 0) {
		self->changeBck(0xD);
		JGeometry::TVec3<f32> local_1C = self->getPosition();
		gpItemManager->makeObjAppear(local_1C.x, local_1C.y, local_1C.z,
		                             ACTOR_TYPE_COIN, false);
		self->invalidateCollisionAll();
		self->emitWaterParticle();
	}

	if (self->getMActor()->curAnmEndsNext()) {
		self->unk124->reset();
		self->goToShortestNextGraphNode();
		spine->reset();
		spine->pushAfterCurrent(&TNerveHino2GraphWander::theNerve());
		self->kill();
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveHino2Stamp, TLiveActor)
{
	THinokuri2* self = (THinokuri2*)spine->getBody();
	if (spine->getTime() == 0) {
		self->changeBck(0xB);
		self->setUnk160(0);
	}

	if (self->getMActor()->curAnmEndsNext()) {
		int uVar7 = self->getUnk160();
		++uVar7;
		int stampCnt
		    = ((THino2Params*)self->getSaveParam())->mSLStampCount.get();
		if (uVar7 > stampCnt) {
			self->setUnk160(0);
			self->resetPolInterval();
			spine->pushAfterCurrent(&TNerveHino2GraphWander::theNerve());
			return true;
		}
		self->setUnk160(uVar7);
	}

	int frame = self->getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame();
	if (!self->isAirborne() && (frame == 0x1C || frame == 0x3E)) {
		f32 js = ((THino2Params*)self->getSaveParam())->mSLJumpShake.get();
		if (!(js * js < self->getDistToMarioSquared()))
			gpCameraShake->startShake(CAM_SHAKE_MODE_UNK4, 0.8f);

		f32 sql = ((THino2Params*)self->getSaveParam())->mSLStampQuakeLen.get();
		sql     = sql * sql;
		if (self->getDistToMarioSquared() < sql)
			SMS_SendMessageToMario(self, HIT_MESSAGE_SUPER_HIP_DROP);
	}

	return false;
}

DEFINE_NERVE(TNerveHino2Freeze, TLiveActor)
{
	THinokuri2* self = (THinokuri2*)spine->getBody();
	if (spine->getTime() == 0) {
		self->changeBck(0x16);
		self->unk184 = 1;
	}
	int timer = ((THino2Params*)self->getSaveParam())->getSLFreezeTimerLv0();
	if (spine->getTime() >= timer) {
		self->unk184 = 0;
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveHino2WaitAnm, TLiveActor)
{
	THinokuri2* self = (THinokuri2*)spine->getBody();

	if (self->getMActor()->curAnmEndsNext())
		return true;

	return false;
}

// @todo: preserve hinokuri2.cpp's .sdata2 prefix @3767 through @5074.
// An extra weak-inline 3.0f otherwise displaces moveObject's bank constants.
void order_sdata2(f32* constants, f64* conversion)
{
	constants[0]  = 3.05175781e-05f;
	constants[1]  = 0.8f;
	conversion[0] = 4503601774854144.0;
	constants[2]  = 500.0f;
	constants[3]  = 0.0f;
	constants[4]  = 90.0f;
	constants[5]  = -90.0f;
	constants[6]  = 0.00549316406f;
	constants[7]  = 180.0f;
	constants[8]  = 360.0f;
	constants[9]  = 15.0f;
	constants[10] = 0.5f;
	constants[11] = 100.0f;
	constants[12] = 1.0f;
	constants[13] = -180.0f;
	constants[14] = 30.0f;
	constants[15] = -30.0f;
	constants[16] = 0.1f;
}

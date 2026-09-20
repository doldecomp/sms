#include <Enemy/Hinokuri2.hpp>
#include <Enemy/Walker.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/Conductor.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Player/MarioAccess.hpp>
#include <Camera/CameraShake.hpp>
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
#include <Map/MapCollisionManager.hpp>
#include <Map/MapCollisionEntry.hpp>

// NOTE: rodata ordering says this has to go here

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
    , PARAM_INIT(mSLBodyHitR0, 100.0f)
    , PARAM_INIT(mSLBodyHitH0, 200.0f)
    , PARAM_INIT(mSLBankProp, 0.5f)
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
		{ "hinokuri2_model.bmd", 0x10000000, 0 },
		{ "hinokuri2_mask_model.bmd", 0x10000000, 0 },
		{ "hinokuri2_mask_model_l.bmd", 0x10000000, 0 },
		{ "hinokuri2_mask_model_r.bmd", 0x10000000, 0 },
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
			M3UMtxCalcBlendAux(param_1, &JStack_d4, ptr, mTwo[0], false);
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

		if (col->isActorType(0x80000001)) {
			col->receiveMessage(this, HIT_MESSAGE_ATTACK);
		} else if ((col->isActorType(0x10000003)
		            || col->isActorType(0x10000002))
		           && mOwner->getMActor()->checkCurBckFromIndex(0xE)) {
			col->receiveMessage(this, HIT_MESSAGE_HIP_DROP);
		}
	}
}

BOOL THino2Hit::receiveMessage(THitActor* sender, u32 message)
{
	if (mJointIdx == 0x19) {
		if (sender->getActorType() == 0x80000001) {
			mOwner->mJointIdxMessageCameFrom = mJointIdx;
			return mOwner->receiveMessage(sender, message);
		}
		if (sender->getActorType() == 0x1000001
		    && message == HIT_MESSAGE_SPRAYED_BY_WATER) {
			mOwner->mJointIdxMessageCameFrom = mJointIdx;
			return mOwner->receiveMessage(sender, message);
		}
		if (sender->getActorType() == 0x4000005a) {
			mOwner->mJointIdxMessageCameFrom = mJointIdx;
			return mOwner->receiveMessage(sender, message);
		}
	}

	if (mJointIdx == 0x13) {
		if (sender->getActorType() == 0x1000001
		    && message == HIT_MESSAGE_SPRAYED_BY_WATER) {
			mOwner->mJointIdxMessageCameFrom = mJointIdx;
			return mOwner->receiveMessage(sender, message);
		}
		if (sender->getActorType() == 0x4000005a) {
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

			f32 gravity = ((THino2Params*)unk0->getSaveParam())->mSLGravityY.get();

			unk40.x -= gravity;
			unk34.x -= gravity;

			unk28 += unk40;
			unk1C += unk34;

			++unk8;
		}

		if (cue & CUE_CALC_ANIM) {
			Mtx afStack_58;
			Mtx afStack_88;
			{
				MTXIdentity(afStack_58);
				afStack_58[0][3] = unk28.x;
				afStack_58[1][3] = unk28.y;
				afStack_58[2][3] = unk28.z;
				MsMtxSetRotRPH(afStack_88, 0.0f, 360.0f - unk8, 0.0f);
				MTXConcat(getUnk4C(), afStack_88, afStack_88);
				MTXConcat(afStack_58, afStack_88, afStack_58);
				unk14->getModel()->setBaseTRMtx(afStack_58);
			}

			{
				PSMTXIdentity(afStack_58);
				afStack_58[0][3] = unk1C.x;
				afStack_58[1][3] = unk1C.y;
				afStack_58[2][3] = unk1C.z;
				MsMtxSetRotRPH(afStack_88, 0.0f, unk8, 0.0f);
				MTXConcat(getUnk4C(), afStack_58, afStack_58);
				MTXConcat(afStack_58, afStack_88, afStack_58);
				unk18->getModel()->setBaseTRMtx(afStack_58);
			}
		}

		if ((cue & CUE_ENTRY) && unk8 > 60 && unk8 % 6 >= 3)
			cue &= ~CUE_ENTRY;

		unk14->perform(cue, graphics);
		unk18->perform(cue, graphics);
	}
}

static int Hino2HeadCallback(J3DNode* param_1, int param_2)
{
	if (param_2 == 0) {
		MtxPtr mA = gpCurHinokuri->getModel()->getAnmMtx(
		    ((J3DJoint*)param_1)->getJntNo());

		if (gpCurHinokuri->mLevel == 1) {
			Mtx local_44;
			local_44[0][3] = 0.0;
			local_44[1][3] = 0.0;
			local_44[2][3] = 0.0;

			f32 scale      = gpCurHinokuri->unk194;
			local_44[0][0] = scale;
			local_44[0][1] = 0.0;
			local_44[0][2] = 0.0;

			local_44[1][0] = 0.0;
			local_44[1][1] = scale;
			local_44[1][2] = 0.0;

			local_44[2][0] = 0.0;
			local_44[2][1] = 0.0;
			local_44[2][2] = scale;

			f32 s = JMASin(gpCurHinokuri->unk198);
			f32 c = JMACos(gpCurHinokuri->unk198);

			Mtx local_74;
			MtxPtr rot = local_74;
			local_74[0][0] = c;
			local_74[0][1] = 0.0;
			local_74[0][2] = s;
			local_74[0][3] = 0.0;

			local_74[1][0] = 0.0;
			local_74[1][1] = 1.0;
			local_74[1][2] = 0.0;
			local_74[1][3] = 0.0;

			local_74[2][0] = -s;
			local_74[2][1] = 0.0;
			local_74[2][2] = c;
			local_74[2][3] = 0.0;

			MTXConcat(mA, rot, mA);
			MTXConcat(mA, local_44, mA);
			MTXConcat(J3DSys::mCurrentMtx, rot, J3DSys::mCurrentMtx);
			MTXConcat(J3DSys::mCurrentMtx, local_44, J3DSys::mCurrentMtx);
		} else {
			Mtx local_a4;
			MtxPtr rot     = local_a4;
			f32 s          = JMASin(gpCurHinokuri->unk198);
			f32 c          = JMACos(gpCurHinokuri->unk198);
			local_a4[0][0] = c;
			local_a4[0][1] = 0.0;
			local_a4[0][2] = s;
			local_a4[0][3] = 0.0;

			local_a4[1][0] = 0.0;
			local_a4[1][1] = 1.0;
			local_a4[1][2] = 0.0;
			local_a4[1][3] = 0.0;

			local_a4[2][0] = -s;
			local_a4[2][1] = 0.0;
			local_a4[2][2] = c;
			local_a4[2][3] = 0.0;

			MTXConcat(mA, rot, mA);
			MTXConcat(J3DSys::mCurrentMtx, rot, J3DSys::mCurrentMtx);
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
	initHitActor(0x8000001, 5, 0, mBodyRadius, mHeadHeight, mBodyRadius,
	             mHeadHeight);
	unk150 = new TMBindShadowBody(this, getModel(), 1.0f);

	TIdxGroupObj* enemiesGrp
	    = JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ");
	enemiesGrp->getChildren().push_back(mHead);
	enemiesGrp->getChildren().push_back(mBody);
	enemiesGrp->getChildren().push_back(unk178);
	enemiesGrp->getChildren().push_back(unk174);

	mHead->initHitActor(0x8000001, 5, 0x41000000, mHead->getAttackRadius(),
	                    mHead->getAttackHeight(), mHead->getDamageRadius(),
	                    mHead->getDamageHeight());
	mBody->initHitActor(0x8000001, 5, -0x3f000000, mHead->getAttackRadius(),
	                    mHead->getAttackHeight(), mHead->getDamageRadius(),
	                    mHead->getDamageHeight());
	unk174->initHitActor(0x8000001, 5, -0x70000000, 70.0f, 120.0f, 70.0f,
	                     120.0f);
	unk178->initHitActor(0x8000001, 5, -0x70000000, 70.0f, 120.0f, 70.0f,
	                     120.0f);

	validateCollisionAll();

	getMActor()->setJointCallback(0x17, &Hino2HeadCallback);

	if (gpMarDirector->mMap == 1) {
		switch (gpMarDirector->unk7D) {
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
	} else if (gpMarDirector->mMap == 0) {
		switch (gpMarDirector->unk7D) {
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

// TODO: figure out which parts of code belong into these inlines

void THinokuri2::emitPolParticle() { }

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
	onHitFlag(HIT_FLAG_NO_COLLISION);
	mHead->onHitFlag(HIT_FLAG_NO_COLLISION);
	mBody->onHitFlag(HIT_FLAG_NO_COLLISION);
	unk174->onHitFlag(HIT_FLAG_NO_COLLISION);
	unk178->onHitFlag(HIT_FLAG_NO_COLLISION);
}

void THinokuri2::validateCollisionAll()
{
	onHitFlag(HIT_FLAG_NO_COLLISION);
	mHead->offHitFlag(HIT_FLAG_NO_COLLISION);
	mBody->offHitFlag(HIT_FLAG_NO_COLLISION);
	unk174->offHitFlag(HIT_FLAG_NO_COLLISION);
	unk178->offHitFlag(HIT_FLAG_NO_COLLISION);
}

void THinokuri2::emitWaterParticle()
{
	if (!unk19C)
		return;

	JGeometry::TVec3<f32> position;
	if (mLevel >= 1) {
		getJointTransByIndex(0x19, &position);
	} else {
		position = mPosition;
		position.y += ((THino2Params*)getSaveParam())->mSLWaterEmitPos.get();
	}
	unk19C->mPos.value = position;
	gpModelWaterManager->emitRequest(*unk19C);
}

void THinokuri2::shakeCamera(int) { }

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
	if (unk158 > 0)
		SMSGetMSound()->startSoundActor(MSD_SE_BS_HINO_SEED_LQ_LEV, &mPosition);
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
			    getActorKeeper()->getMActorAnmData()->getUnk2C()->getAnmPtr(
			        param_1));
		} else {
			unk1A0->setAnmTransform(
			    getActorKeeper()->getMActorAnmData()->getUnk2C()->getAnmPtr(
			        param_1));
		}

		mMActor->getAnmBck()->setFrameCtrl(param_1);
	}

	mCurrentBck = param_1;
	setAnmSound(hinokuri2_bastable[mCurrentBck]);
	J3DFrameCtrl* pJVar7 = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
	if (pJVar7 != nullptr) {
		if (mLevel == 0 && (param_1 - 23U <= 1 || param_1 - 26U <= 1))
			pJVar7->setRate(((THino2Params*)getSaveParam())->mSLWalkSpeedRateLv0.value);
		else
			pJVar7->setRate(1.0f);
	}
}

BOOL THinokuri2::receiveMessageLv0(THitActor* sender, u32 message)
{
	if (mJointIdxMessageCameFrom == 0x13 && sender->getActorType() == 0x1000001
	    && message == HIT_MESSAGE_SPRAYED_BY_WATER)
		return true;

	if (sender->getActorType() == 0x1000001
	    && message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		if (mJointIdxMessageCameFrom != 0x19)
			return true;

		if (mSpine->getCurrentNerve() != &TNerveHino2Freeze::theNerve())
			mSpine->pushNerve(&TNerveHino2Freeze::theNerve());
	}

	if (sender->getActorType() == 0x80000001
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

BOOL THinokuri2::receiveMessageLv1(THitActor* sender, u32 message)
{
	if (mJointIdxMessageCameFrom == 0x13 && sender->getActorType() == 0x1000001
	    && message == HIT_MESSAGE_SPRAYED_BY_WATER)
		return true;

	if (unk180 && sender->getActorType() == 0x1000001
	    && message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		if (mJointIdxMessageCameFrom != 0x19)
			return true;

		int dmgAmount
		    = gpModelWaterManager->getParticleAttack((TWaterHitActor*)sender);

		if (dmgAmount <= 0)
			return true;

		if (dmgAmount >= mHitPoints)
			mHitPoints = 0;
		else
			mHitPoints = mHitPoints - dmgAmount;

		++unk18C;

		if (getSpine()->getCurrentNerve() != &TNerveHino2Damage::theNerve())
			getSpine()->setNext(&TNerveHino2Damage::theNerve());

		return true;
	}

	if (unk180 && sender->getActorType() == 0x4000005A) {
		if (mJointIdxMessageCameFrom != 0x19)
			return true;

		mHitPoints = 0;

		if (getSpine()->getCurrentNerve() != &TNerveHino2Damage::theNerve()) {
			getSpine()->setNext(&TNerveHino2Damage::theNerve());
		}

		return true;
	}

	return false;
}

static inline THino2Params* Hino2Params(THinokuri2* self)
{
	THino2Params* params = (THino2Params*)self->getSaveParam();
	return params;
}

// Parked TU-locally: retail binds the mask pointer at each use.
static inline THino2Mask* Hino2Mask(THinokuri2* self)
{
	THino2Mask* mask = self->unk1A4;
	return mask;
}

BOOL THinokuri2::receiveMessageLv2(THitActor* sender, u32 message)
{
	if (sender->getActorType() == 0x1000001
	    && message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		if (mJointIdxMessageCameFrom != 0x13)
			return true;

		int dmgAmount
		    = gpModelWaterManager->getParticleAttack((TWaterHitActor*)sender);

		if (dmgAmount <= 1)
			return true;

		unk190 += 1;
		Hino2Mask(this)->unkC += 1;

		unk168 = Hino2Params(this)->mSLInvincibleTimer.get();

		if (unk190 > 2) {
			unk190 = 0;
			setLevel(mLevel - 1);

			Hino2Mask(this)->reset();

			mHitPoints = calcHitPoints();
		}

		return true;
	}

	if (sender->getActorType() == 0x4000005A) {
		if (mJointIdxMessageCameFrom != 0x13)
			return true;

		unk190 += 1;
		Hino2Mask(this)->unkC += 1;

		unk168 = ((THino2Params*)getSaveParam())->mSLInvincibleTimer.get();

		if (unk190 > 2) {
			unk190 = 0;
			setLevel(mLevel - 1);

			Hino2Mask(this)->reset();

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

// Parked TU-locally: TPathNode::getPoint() reloads unk0 where retail tests and
// dereferences one load. Shared-header need: a named local in PathNode.hpp.
static inline const JGeometry::TVec3<f32>& Hino2NodePoint(const TPathNode& node)
{
	THitActor* owner = node.unk0;
	if (owner != 0)
		return owner->getPosition();

	return node.unk4;
}

template <class T> static inline T symmetric_clamp(T v, T r)
{
	return v > 0 ? (v > r ? r : v) : (v > -r ? v : -r);
}

void THinokuri2::moveObject()
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;

	if (unk164 > 0)
		--unk164;

	if (mLevel == 1) {
		f32 dhp    = calcHitPoints() - mHitPoints;
		f32 fVar12 = (((THino2Params*)getSaveParam())->getSLDamageHeadScale() - 1.0f)
		                 * (1.0f + dhp / calcHitPoints())
		             - unk194;

		if (fVar12 > 0.0f)
			fVar12 = std::min(fVar12, 0.004f);
		else
			fVar12 = std::max(fVar12, -0.004f);

		unk194 += fVar12;
	} else {
		unk194 = 1.0f;
	}

	if (gpMarDirector->unk58 % 600 == 0)
		generateEnemy();

	doShortCut();
	mLinearVelocity.zero();
	mAngularVelocity.zero();
	if (mMapCollisionManager != nullptr) {
		JGeometry::TVec3<f32> aTStack_30;
		getJointTransByIndex(0x17, &aTStack_30);
		if (mMapCollisionManager->unk8)
			mMapCollisionManager->unk8->moveTrans(aTStack_30);
	}

	f32 headHitR = ((THino2Params*)getSaveParam())->mSLHeadHitR.value;
	mHead->setDamageRadius(unk194 * (headHitR * mBodyScale));
	f32 bodyScale = ((THino2Params*)getSaveParam())->mSLBodyScale.value;
	mHead->setDamageHeight(unk194 * (mBodyScale * bodyScale));

	if (mLevel == 0) {
		f32 bodyHitR0 = ((THino2Params*)getSaveParam())->mSLBodyHitR0.value;
		mBody->setDamageRadius(unk194 * (bodyHitR0 * mBodyScale));
		f32 bodyHitH0 = ((THino2Params*)getSaveParam())->mSLBodyHitH0.value;
		mBody->setDamageHeight(unk194 * (bodyHitH0 * mBodyScale));
	} else {
		f32 bodyHitR = ((THino2Params*)getSaveParam())->mSLBodyHitR.value;
		mBody->setDamageRadius(unk194 * (bodyHitR * mBodyScale));
		f32 bodyHitH = ((THino2Params*)getSaveParam())->mSLBodyHitH.value;
		mBody->setDamageHeight(unk194 * (bodyHitH * mBodyScale));
	}

	updateSquareToMario();

	f32 oldRot = mRotation.y;

	control();
	bind();

	mPosition += mLinearVelocity;
	mRotation += mAngularVelocity;

	unk198
	    += -MsClamp(MsWrap(MsAngleDiff(mRotation.y, oldRot), -180.0f, 180.0f),
	                -30.0f, 30.0f)
	       * ((THino2Params*)getSaveParam())->mSLBankProp.get();

	unk198 = MsWrap(unk198, -180.0f, 180.0f);

	f32 prop = ((THino2Params*)getSaveParam())->mSLBankLimit.get();
	unk198   = MsClamp(unk198, -prop, prop);

	// looks like symmetric_clamp but it isn't???
	if (unk198 < 0.0f) {
		f32 fVar1 = 0.1f + unk198;
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
				if (getLevel() == 0 || getLevel() == 2)
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

	if (!checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_CLIPPED_OUT)) {
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

	if (gpMarDirector->unk7D >= 2 && self->unk164 <= 0) {
		if (self->getLevel() >= 1) {
			spine->pushAfterCurrent(&TNerveHino2PrePol::theNerve());
			return 1;
		}

		self->unk15C = 0;

		JGeometry::TVec3<f32> local_60;
		if (self->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
			local_60 = self->mPosition;
			local_60.y += 500.0f;
		} else {
			self->getJointTransByIndex(0x18, &local_60);
		}

		self->resetPolInterval();
	}

	if (spine->getTime() == 0)
		self->changeBck(0x18);

	if (self->getCurrentBck() == 0x18) {
		self->walkToCurPathNode(self->mMarchSpeed, self->mTurnSpeed, 0.0f);
	}

	int frame = self->getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame();
	if (self->getLevel() != 0 && !self->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)
	    && !self->isAirborne() && (frame == 0x24 || frame == 0x55)) {
		f32 ws = ((THino2Params*)self->getSaveParam())->mSLWalkShake.get();
		if (!(ws * ws < self->mDistToMarioSquared))
			gpCameraShake->startShake(CAM_SHAKE_MODE_ENEMY, 0.8f);

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

DEFINE_NERVE(TNerveHino2JumpIn, TLiveActor)
{
	THinokuri2* self = (THinokuri2*)spine->getBody();

	if (spine->getTime() == 0)
		self->changeBck(0x9);

	if (self->getMActor()->curAnmEndsNext()) {
		const TPathNode& node          = self->unk104;
		const JGeometry::TVec3<f32>& p = Hino2NodePoint(node);
		f32 f = self->unk124->unkC;
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
		if (!(js * js < self->mDistToMarioSquared))
			gpCameraShake->startShake(CAM_SHAKE_MODE_ENEMY2, 0.8f);
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

	const TPathNode& node         = self->unkF4;
	JGeometry::TVec3<f32> posDiff = Hino2NodePoint(node);

	posDiff -= self->mPosition;

	f32 angleDiff = MsAngleDiff(
	    MsWrap(MsGetRotFromZaxisY(posDiff), 0.0f, 360.0f), self->mRotation.y);

	if (spine->getTime() == 0 && fabsf(angleDiff) > 15.0f
	    && self->mCurrentBck != 0x15) {
		self->changeBck(0x15);
	}

	f32 turnSpeed = self->mTurnSpeed;
	f32 fVar3     = symmetric_clamp(angleDiff, turnSpeed);
	// TODO: retail copies the clamped angle into its own FPR in each arm
	// (`fmr f4, f31` twice); ours merges the two arms. Frame is 0x18 short.

	self->mRotation.y = MsWrap(self->mRotation.y + fVar3, 0.0f, 360.0f);

	if (fabsf(fVar3) < turnSpeed * 0.5f)
		return true;

	return false;
}

DEFINE_NERVE(TNerveHino2PrePol, TLiveActor)
{
	THinokuri2* self = (THinokuri2*)spine->getBody();

	if (spine->getTime() == 0)
		self->changeBck(0x16);

	if (self->mCurrentBck == 0x16) {
		if (self->getMActor()->curAnmEndsNext()) {
			int uVar5 = self->mWaitTimer;
			uVar5 += 1;

			int wait = ((THino2Params*)self->getSaveParam())->mSLPrePolWait.get();
			if (wait > uVar5) {
				f32 prob = ((THino2Params*)self->getSaveParam())->mSLStampProb.get();
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
			int polWait = ((THino2Params*)self->getSaveParam())->mSLPolWaitCount.get();
			if (uVar1 > polWait) {
				self->unk180 = FALSE;
				self->changeBck(0x10);
				uVar1 = 0;
			}
			self->mWaitTimer = uVar1;
		}
		return false;
	}

	if (self->mCurrentBck == 16) {
		if (self->getMActor()->curAnmEndsNext()) {
			self->changeBck(0x11);
			self->unk15C = 0;

			JGeometry::TVec3<f32> local_40;
			if (self->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
				local_40 = self->mPosition;
				local_40.y += 500.0f;
			} else {
				self->getJointTransByIndex(0x18, &local_40);
			}
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

	if (spine->getTime() >= ((THino2Params*)self->getSaveParam())->getSLDamageTimer()) {
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

static inline TCameraShake* Hino2CameraShake()
{
	TCameraShake* shake = gpCameraShake;
	return shake;
}

static inline bool Hino2CurAnmEndsNext(THinokuri2* self)
{
	MActor* actor = self->getMActor();
	return actor->curAnmEndsNext();
}

static inline f32 Hino2SLJumpShake(THinokuri2* self)
{
	THino2Params* params = (THino2Params*)self->getSaveParam();
	f32 shake            = params->mSLJumpShake.get();
	return shake;
}

DEFINE_NERVE(TNerveHino2Squat, TLiveActor)
{
	THinokuri2* self = (THinokuri2*)spine->getBody();

	self->unk188 = 0;

	if (spine->getTime() == 0)
		self->changeBck(0x13);

	if (Hino2CurAnmEndsNext(self)) {
		if (self->mCurrentBck == 0x13) {
			f32 js = Hino2SLJumpShake(self);
			if (!(js * js < self->mDistToMarioSquared))
				Hino2CameraShake()->startShake(CAM_SHAKE_MODE_ENEMY2, 0.8f);
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

DEFINE_NERVE(TNerveHino2Die, TLiveActor)
{
	THinokuri2* self = (THinokuri2*)spine->getBody();
	if (spine->getTime() == 0) {
		self->changeBck(0xD);
		JGeometry::TVec3<f32> local_1C = self->mPosition;
		gpItemManager->makeObjAppear(local_1C.x, local_1C.y, local_1C.z,
		                             0x2000000E, false);
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
		// TODO: retail folds this into one `lwz 0x180(r3)`; ours materialises
		// the TParamRT address first (`addi 0x170` + `lwz 0x10`), i.e. get()
		// expands one level deeper here. Reordering the read and a named
		// THino2Params* local both change nothing; the wrapper form that used
		// to fold cost TNerveHino2Burst 2.5%, so the per-site cast stays.
		int stampCnt = ((THino2Params*)self->getSaveParam())->mSLStampCount.get();
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
			gpCameraShake->startShake(CAM_SHAKE_MODE_ENEMY2, 0.8f);

		f32 sql = ((THino2Params*)self->getSaveParam())->mSLStampQuakeLen.get();
		sql     = sql * sql;
		if (self->getDistToMarioSquared() < sql)
			SMS_SendMessageToMario(self, 3);
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

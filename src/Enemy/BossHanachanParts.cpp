#include <Enemy/BossHanachan.hpp>
#include <Camera/cameralib.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <MarioUtil/MapUtil.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/ShadowUtil.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Strategy.hpp>
#include <Strategic/ObjModel.hpp>
#include <System/MarDirector.hpp>
#include <NPC/NpcInbetween.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Player/MarioAccess.hpp>
#include <Map/MapData.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

const char* cMapCollisionJointName = "center";
const char* cBodyMapCollisionFileName = "/scene/bosshanachan/hanabody_col.col";
const char* cHeadMapCollisionFileName = "/scene/bosshanachan/hanahead_col.col";
const char* cLegJointName_L3 = "leg_L3";
const char* cLegJointName_R3 = "leg_R3";
const char* cNoseHallJointName_L = "L_hall";
const char* cNoseHallJointName_R = "R_hall";

void CalcMtxPtrFromJointName(JUTNameTab* names, const char* name,
                           J3DModel* model, MtxPtr* result)
{
	u16 index = names->getIndex(name);
	*result = model->getAnmMtx(index);
}

// Binding level over a raw member read, worth +8 of low region in
// TBossHanachanPartsBase::TBossHanachanPartsBase (batch 127).
static inline TBossHanachan* BossHanachanPartsUnkFC(const TBossHanachanPartsBase* p)
{
	TBossHanachan* vFC = p->unkFC;
	return vFC;
}

TBossHanachanPartsBase::TBossHanachanPartsBase(TBossHanachan* boss,
                                             u32 actorType, int modelIndex,
                                             const char* name)
    : TLiveActor(name)
    , mCurrentAnm(BOSS_HANACHAN_ANM_UNK18)
    , mPreviousAnm(BOSS_HANACHAN_ANM_UNK18)
    , unkFC(boss)
    , unk100(nullptr)
    , unk104(nullptr)
    , unk108(nullptr)
    , unk10C(0)
    , mInbetween(nullptr)
{
	mMActorKeeper = BossHanachanPartsUnkFC(this)->mMActorKeeper;
	mMActor = mMActorKeeper->createMActorFromNthData(modelIndex, 0);
	mMActor->initNormalMotionBlend();
	initHitActor(actorType, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f);
	onHitFlag(HIT_FLAG_NO_COLLISION);
	switch ((int)actorType) {
	case 0x08000015:
		mScaledBodyRadius = unkFC->mCommonParams->mSLBodyShadowSize.get();
		break;
	case 0x08000014:
		mScaledBodyRadius = unkFC->mCommonParams->mSLHeadShadowSize.get();
		break;
	}
	onLiveFlag(LIVE_FLAG_UNK8);
	initAnmSound();
	mMActor->setLightType(1);
	mInbetween = new TNpcInbetween(
	    1, CLBPalFrame(unkFC->mCommonParams->mSLMotionBlendFrames.get()));
}

TBossHanachanPartsBody::TBossHanachanPartsBody(TBossHanachan* boss,
                                             const char* name)
    : TBossHanachanPartsBase(boss, 0x08000015, 0, name)
    , unk114(0)
    , unk120(0.0f)
    , mPreviousPosition(0.0f, 0.0f, 0.0f)
    , mOlderPosition(0.0f, 0.0f, 0.0f)
    , mPreviousRoll(0.0f)
    , mOlderRoll(0.0f)
    , unk144(0.0f)
    , unk148(0.0f)
    , unk154(0.0f, 0.0f, 0.0f)
{
	J3DModel* model = getModel();
	JUTNameTab* names = model->getModelData()->getJointName();
	CalcMtxPtrFromJointName(names, cLegJointName_L3, model, &mLegMtx[0]);
	CalcMtxPtrFromJointName(names, cLegJointName_R3, model, &mLegMtx[1]);
}

TBossHanachanPartsHead::TBossHanachanPartsHead(TBossHanachan* boss,
                                             const char* name)
    : TBossHanachanPartsBase(boss, 0x08000014, 1, name)
{
	J3DModel* model = getModel();
	JUTNameTab* names = model->getModelData()->getJointName();
	CalcMtxPtrFromJointName(names, cNoseHallJointName_L, model, &mLeftNoseMtx);
	CalcMtxPtrFromJointName(names, cNoseHallJointName_R, model, &mRightNoseMtx);
}

void TBossHanachanPartsBase::initMapCollisionAndHitActor_(TIdxGroupObj* group)
{
	TBossHanachanCommonSaveParams* params = unkFC->mCommonParams;
	const char* collisionFile = cBodyMapCollisionFileName;
	f32 attackRadius = params->mSLBodyAttackRadius.get();
	f32 attackHeight = params->mSLBodyAttackHeight.get();
	f32 damageRadius = params->mSLBodyDamageRadius.get();
	f32 damageHeight = params->mSLBodyDamageHeight.get();
	f32 offsetY = params->mSLBodyHitOffsetY.get();
	switch ((int)getActorType()) {
	case 0x08000015:
		break;
	case 0x08000014:
		collisionFile = cHeadMapCollisionFileName;
		attackRadius = params->mSLHeadAttackRadius.get();
		attackHeight = params->mSLHeadAttackHeight.get();
		damageRadius = params->mSLHeadDamageRadius.get();
		damageHeight = params->mSLHeadDamageHeight.get();
		offsetY = params->mSLHeadHitOffsetY.get();
		break;
	}
	J3DModel* model = mMActor->getModel();
	JUTNameTab* names = model->getModelData()->getJointName();
	u16 joint = names->getIndex(cMapCollisionJointName);
	unk108 = getMActor()->getModel()->getAnmMtx(joint);
	unk104 = new TMapCollisionMove;
	unk104->init(collisionFile, 0x8000, this);
	unk100 = new TWaterHitActor("ボスハナチャンのパーツ");
	unk100->initHitActor(getActorType(), 1, ACTOR_TYPE_PLAYER, attackRadius,
	                     attackHeight, damageRadius, damageHeight);
	group->getChildren().push_back(unk100);
	unk100->offHitFlag(HIT_FLAG_NO_COLLISION);
	MtxPtr mtx = unk108;
	unk100->mPosition.set(mtx[0][3], mtx[1][3] - offsetY, mtx[2][3]);
}

// Binding level worth +16 of low region, landing
// TBossHanachanPartsBody::initFootHitActor_'s frame at 0xf0 (batch 121).
static inline J3DModel* BossHanachanPartsGetModel(const MActor* p)
{
	J3DModel* model = p->getModel();
	return model;
}

void TBossHanachanPartsBody::initFootHitActor_(TIdxGroupObj* group)
{
	static const char* sFootJointName[] = { "foot_L", "foot_R" };
	int i;
	J3DModel* model;
	int joints[2];
	TBossHanachanCommonSaveParams* params = unkFC->mCommonParams;
	model = BossHanachanPartsGetModel(getMActor());
	JUTNameTab* names = model->getModelData()->getJointName();
	for (i = 0; i < 2; ++i) {
		joints[i] = names->getIndex(sFootJointName[i]);
		mFeet[i] = new TFootHitActor("ボスハナチャンの足");
		mFeet[i]->initHitActor(getActorType(), 1, ACTOR_TYPE_PLAYER,
		                      params->mSLFootAttackRadius.get(),
		                      params->mSLFootAttackHeight.get(),
		                      params->mSLFootDamageRadius.get(),
		                      params->mSLFootDamageHeight.get());
		group->getChildren().push_back(mFeet[i]);
		mFeet[i]->offHitFlag(HIT_FLAG_NO_COLLISION);
		MtxPtr mtx
		    = BossHanachanPartsGetModel(mMActor)->getAnmMtx((u16)joints[i]);
		mFeet[i]->mJointMtx = mtx;
		mFeet[i]->mPosition.set(mtx[0][3], mtx[1][3], mtx[2][3]);
	}
}

void TBossHanachanPartsBase::offNonstopMotionBlend_()
{
	mInbetween->mForcedBlendRatio = 0.0f;
}

void TBossHanachanPartsBase::setNonstopMotionBlendRatio_(f32 ratio)
{
	mInbetween->mForcedBlendRatio = ratio;
}

void TBossHanachanPartsBase::restartBck_()
{
	mMActor->getFrameCtrl(0)->setFrame(0.0f);
}

void TBossHanachanPartsBase::changeTumbleAnmRate_()
{
	J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(ANM_TYPE_BCK);
	switch (mCurrentAnm) {
	case BOSS_HANACHAN_ANM_UNK16:
	case BOSS_HANACHAN_ANM_UNK17: {
		if (ctrl->getFrame() <= 40.0f)
			return;
		f32 remaining = ctrl->getEnd() - ctrl->getFrame();
		f32 rate = ctrl->getRate();
		CLBChaseConstantSpecifyFrame(&rate, SMSGetAnmFrameRate(), remaining);
		ctrl->setRate(rate);
		break;
	}
	default:
		ctrl->setRate(SMSGetAnmFrameRate());
		break;
	}
}

void TBossHanachanPartsBase::moveMapCollision_()
{
	JGeometry::TVec3<f32> position(unk108[0][3], unk108[1][3], unk108[2][3]);
	unk104->moveTrans(position);
}

void TBossHanachanPartsBase::entryCircleShadow_()
{
	if (unkFC->getSpine()->getCurrentNerve() == &TNerveBossHanachanDead::theNerve()
	    && unkFC->getSpine()->getTime() > 200)
		return;

	TCircleShadowRequest request;
	request.mPosition.set(unk108[0][3], unk108[1][3], unk108[2][3]);
	request.mRadiusX = request.mRadiusZ = mScaledBodyRadius;
	gpBindShadowManager->forceRequest(request, getActorType());
}

void TBossHanachanPartsBase::setDamageFog_(JDrama::TGraphics* graphics)
{
	bool isBody = true;
	if (getActorType() == 0x08000014)
		isBody = false;
	J3DModelData* data = mMActor->getModel()->getModelData();
	u16 materialCount = data->getMaterialNum();
	JGeometry::TVec3<f32> position(unk108[0][3], unk108[1][3], unk108[2][3]);
	if (unkFC->getLatestNerve() == &TNerveBossHanachanDamage::theNerve()) {
		SMS_AddDamageFogEffect(data, position, graphics);
		if (isBody)
			for (u16 i = 0; i < materialCount; ++i)
				data->getMaterialNodePointer(i)->change();
		if (unk10C == 0)
			getMActor()->getModel()->unlock();
	} else {
		SMS_ResetDamageFogEffect(data);
	}
}

// Binding level worth +8 of low region, landing
// TBossHanachanPartsBase::isCurBckAlreadyEnd_'s frame at 0x30 (batch 121).
static inline s16 BossHanachanPartsGetEnd(const J3DFrameCtrl* p)
{
	s16 end = p->getEnd();
	return end;
}

bool TBossHanachanPartsBase::isCurBckAlreadyEnd_() const
{
	bool result = true;
	if (mMActor) {
		J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(0);
		if (ctrl) {
			result = (ctrl->checkState(J3DFrameCtrl::STATE_COMPLETED_ONCE)
			          || ctrl->checkState(J3DFrameCtrl::STATE_LOOPED_ONCE))
			                 || ctrl->getFrame() + 0.1f
			                        >= BossHanachanPartsGetEnd(ctrl);
		}
	}
	return result;
}

// Binding level over a raw mMActor read. Two sites in
// copyFrameFromOldAnmToNewAnm_ target +0x10 of frame.
static inline MActor*
BossHanachanPartsGetActor(const TBossHanachanPartsBase* p)
{
	MActor* actor = p->mMActor;
	return actor;
}

void TBossHanachanPartsBase::copyFrameFromOldAnmToNewAnm_()
{
	J3DAnmTransform* anm = BossHanachanPartsGetActor(this)->getBckAnm();
	J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(0);
	if (anm && ctrl) {
		f32 frame = BossHanachanPartsGetActor(this)->getBckOldMotionBlendFrame();
		anm->setFrame(frame);
		ctrl->setFrame(frame);
	}
}

const TLiveActor* TBossHanachanPartsBase::getSandActor_() const
{
	const TLiveActor* actor = SMS_GetGroundActor(mGroundPlane, 0x400000CD);
	if (!actor)
		actor = SMS_GetGroundActor(mGroundPlane, 0x400000CB);
	return actor;
}

bool TBossHanachanPartsBase::isMarioOn_() const
{
	bool result = false;
	if (SMS_IsMarioTouchGround4cm()) {
		const TBGCheckData* ground = SMS_GetMarioGroundPlane();
		if (ground && ground->getActor() == this)
			result = true;
	}
	return result;
}

void TBossHanachanPartsBase::calcRotateZWhenGetUp_()
{
	if (unk10C == 0) {
		switch (mCurrentAnm) {
		case BOSS_HANACHAN_ANM_UNK8:
		case BOSS_HANACHAN_ANM_UNK11: {
			J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(0);
			f32 frames = 2.0f * (ctrl->getEnd() - ctrl->getFrame());
			if (frames < 0.001f) {
				mRotation.z = 0.0f;
				return;
			}
			CLBChaseConstantSpecifyFrame(&mRotation.z, 0.0f, frames);
			break;
		}
		}
	}
}

bool TBossHanachanPartsBase::isReactToTrampleOrHipDrop_() const
{
	bool result = false;
	const TNerveBase<TLiveActor>* nerve = unkFC->getLatestNerve();
	if (nerve == &TNerveBossHanachanTumble::theNerve()
	    || nerve == &TNerveBossHanachanDown::theNerve()) {
		if (getActorType() == 0x08000015) {
			bool overturned = (mRotation.z == -179.0f || mRotation.z == 179.0f)
			                      ? true : false;
			if (overturned)
				result = true;
		} else {
			result = true;
		}
	}
	return result;
}

void TBossHanachanPartsBase::considerSetAnm_(EnumBossHanachanNerveAnm nerve)
{
	if (nerve == BOSS_HANACHAN_NERVE_ANM_UNK0) {
		switch (mCurrentAnm) {
		case BOSS_HANACHAN_ANM_UNK5:
		case BOSS_HANACHAN_ANM_UNK6:
		case BOSS_HANACHAN_ANM_UNK13:
		case BOSS_HANACHAN_ANM_UNK16:
		case BOSS_HANACHAN_ANM_UNK17:
			if (isCurBckAlreadyEnd_())
				setAnm_(BOSS_HANACHAN_ANM_UNK3,
				        BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
			break;
		}
	} else if (nerve == BOSS_HANACHAN_NERVE_ANM_UNK1) {
		bool marioOn = isMarioOn_();
		switch (mCurrentAnm) {
		case BOSS_HANACHAN_ANM_UNK5:
		case BOSS_HANACHAN_ANM_UNK6:
		case BOSS_HANACHAN_ANM_UNK13:
		case BOSS_HANACHAN_ANM_UNK16:
		case BOSS_HANACHAN_ANM_UNK17:
			if (isCurBckAlreadyEnd_()) {
				if (getActorType() == 0x08000015 && marioOn)
					setAnm_(BOSS_HANACHAN_ANM_UNK2,
					        BOSS_HANACHAN_STOP_MOTION_BLEND_ON);
				else
					setAnm_(BOSS_HANACHAN_ANM_UNK3,
					        BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
			}
			break;
		default:
			if (getActorType() == 0x08000015) {
				bool blending = true;
				if (!mInbetween->isMotionBlending()
				    && !mInbetween->isForcedBlendRatio())
					blending = false;
				if (!blending) {
					if (mCurrentAnm == BOSS_HANACHAN_ANM_UNK2) {
						if (!marioOn)
							setAnm_(BOSS_HANACHAN_ANM_UNK3,
							        BOSS_HANACHAN_STOP_MOTION_BLEND_ON);
					} else if (marioOn) {
						setAnm_(BOSS_HANACHAN_ANM_UNK2,
						        BOSS_HANACHAN_STOP_MOTION_BLEND_ON);
					}
				}
			}
			break;
		}
	} else if (nerve == BOSS_HANACHAN_NERVE_ANM_UNK2) {
		if (unk10C > 0)
			--unk10C;
		if (unk10C == 0) {
			bool ended = isCurBckAlreadyEnd_();
			switch (mCurrentAnm) {
			case BOSS_HANACHAN_ANM_UNK7:
				if (ended)
					setAnm_(BOSS_HANACHAN_ANM_UNK8,
					        BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
				break;
			case BOSS_HANACHAN_ANM_UNK8:
				if (ended)
					setAnm_(BOSS_HANACHAN_ANM_UNK9,
					        BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
				break;
			case BOSS_HANACHAN_ANM_UNK10:
				if (ended)
					setAnm_(BOSS_HANACHAN_ANM_UNK11,
					        BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
				break;
			case BOSS_HANACHAN_ANM_UNK11:
				if (ended)
					setAnm_(BOSS_HANACHAN_ANM_UNK12,
					        BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
				break;
			case BOSS_HANACHAN_ANM_UNK9:
			case BOSS_HANACHAN_ANM_UNK12:
				break;
			default:
				if (mRotation.z < 0.0f)
					setAnm_(BOSS_HANACHAN_ANM_UNK7,
					        BOSS_HANACHAN_STOP_MOTION_BLEND_ON);
				else
					setAnm_(BOSS_HANACHAN_ANM_UNK10,
					        BOSS_HANACHAN_STOP_MOTION_BLEND_ON);
				break;
			}
		}
	} else if (nerve == BOSS_HANACHAN_NERVE_ANM_UNK3) {
		if (unk10C > 0) {
			--unk10C;
			if (unk10C == 0)
				setAnm_(BOSS_HANACHAN_ANM_UNK6,
				        BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
		} else if (mCurrentAnm == BOSS_HANACHAN_ANM_UNK6 && isCurBckAlreadyEnd_()) {
			setAnm_(BOSS_HANACHAN_ANM_UNK4,
			        BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
		}
	} else if (nerve == BOSS_HANACHAN_NERVE_ANM_UNK4) {
		if (unk10C > 0)
			--unk10C;
		if (unk10C == 0 && mCurrentAnm != BOSS_HANACHAN_ANM_UNK14)
			setAnm_(BOSS_HANACHAN_ANM_UNK14,
			        BOSS_HANACHAN_STOP_MOTION_BLEND_ON);
	} else if (nerve == BOSS_HANACHAN_NERVE_ANM_UNK5) {
		if (unk10C > 0)
			--unk10C;
		if (unk10C == 0)
			setAnm_(BOSS_HANACHAN_ANM_UNK15,
			        BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
	}
}

// Two-local binder over mMActor + getModel, +0x10 for body setAnm_.
static inline J3DModel*
BossHanachanPartsGetActorModel(const TBossHanachanPartsBase* p)
{
	MActor* actor = p->mMActor;
	J3DModel* model = actor->getModel();
	return model;
}

// TODO: 93.1%, frame exact; callee-saved rotation only. Retail ranks
// changed (r31) > anm (r30) > this (r29) > blend (r28); ours ranks this first.
// Inert or worse: a TU-local blend helper (by TNpcInbetween* or by this), a
// direct or MActor* model binder in place of the two-local one, and naming the
// table index in the condition.
bool TBossHanachanPartsBody::setAnm_(EnumBossHanachanAnmKind anm,
                                    EnumBossHanachanStopMotionBlendOnOff blend)
{
	static const int sBodyBckIndex[] = {
		19, 15, 10, 13, 0, 12, 9, 2, 3, 4, 5, 6, 7, 8, 16, 1, 17, 18
	};
	bool changed = false;
	if (mCurrentAnm != anm) {
		mPreviousAnm = mCurrentAnm;
		mCurrentAnm = anm;
		if (sBodyBckIndex[anm] != mMActor->getCurAnmIdx(ANM_TYPE_BCK)) {
			int index = sBodyBckIndex[anm];
			if (unk114 == unkFC->mWeakBodyIndex) {
				switch (anm) {
				case BOSS_HANACHAN_ANM_UNK2: index = 11; break;
				case BOSS_HANACHAN_ANM_UNK3: index = 14; break;
				}
			}
			mMActor->setBckFromIndex(index);
			changed = true;
			if (blend == BOSS_HANACHAN_STOP_MOTION_BLEND_ON)
				mInbetween->startMotionBlend();
			else
				mInbetween->stopMotionBlend();
			setCurAnmSound();
		}
		if (anm == BOSS_HANACHAN_ANM_UNK15) {
			mMActor->setBrkFromIndex(0);
			mMActor->getFrameCtrl(ANM_TYPE_BRK)->setAttribute(J3DFrameCtrl::ATTR_ONCE);
			BossHanachanPartsGetActorModel(this)->unlock();
		}
	}
	return changed;
}

// TODO: 97.7%, frame exact; the known-open this-vs-pool-base rotation
// (retail: changed, anm, this, table base, anm*4, blend from r31 down). Inert or
// worse: a one-line returning table-index helper, a blend helper, a named
// mCurrentAnm, non-static const tables.
bool TBossHanachanPartsHead::setAnm_(EnumBossHanachanAnmKind anm,
                                    EnumBossHanachanStopMotionBlendOnOff blend)
{
	static const int sHeadBckIndex[] = {
		36, 32, 29, 31, 20, 30, 28, 22, 23, 24, 25, 26, 27, 28, 33, 21, 34, 35
	};
	static const int sHeadBtpIndex[] = {
		0, 0, 1, 2, 1, 2, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0, 1, 1
	};
	static const int sHeadBtkIndex[] = {
		0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
	bool changed = false;
	if (mCurrentAnm != anm) {
		mPreviousAnm = mCurrentAnm;
		mCurrentAnm = anm;
		if (sHeadBckIndex[anm] != mMActor->getCurAnmIdx(ANM_TYPE_BCK)) {
			mMActor->setBckFromIndex(sHeadBckIndex[anm]);
			changed = true;
			if (blend == BOSS_HANACHAN_STOP_MOTION_BLEND_ON)
				mInbetween->startMotionBlend();
			else
				mInbetween->stopMotionBlend();
			setCurAnmSound();
		}
		if (sHeadBtpIndex[anm] != mMActor->getCurAnmIdx(ANM_TYPE_BTP))
			mMActor->setBtpFromIndex(sHeadBtpIndex[anm]);
		if (sHeadBtkIndex[anm] != mMActor->getCurAnmIdx(ANM_TYPE_BTK))
			mMActor->setBtkFromIndex(sHeadBtkIndex[anm]);
		if (anm == BOSS_HANACHAN_ANM_UNK15) {
			mMActor->setBrkFromIndex(1);
			mMActor->getFrameCtrl(ANM_TYPE_BRK)->setAttribute(J3DFrameCtrl::ATTR_ONCE);
		}
	}
	return changed;
}

// Two-local binder over unkFC + mWeakBodyIndex, +0x10 of
// TBossHanachanPartsBody::receiveMessage. Pair with RecvIdx for +0x18.
static inline s32
BossHanachanPartsRecvWeak(const TBossHanachanPartsBody* p)
{
	TBossHanachan* owner = p->unkFC;
	s32 weak = owner->mWeakBodyIndex;
	return weak;
}

BOOL TBossHanachanPartsBody::receiveMessage(THitActor*, u32 message)
{
	if (gpMarDirector->isThing())
		return FALSE;
	bool received = false;
	if (isReactToTrampleOrHipDrop_()) {
		switch ((int)message) {
		case HIT_MESSAGE_TRAMPLE:
			if (mCurrentAnm == BOSS_HANACHAN_ANM_UNK5)
				restartBck_();
			else
				setAnm_(BOSS_HANACHAN_ANM_UNK5,
				        BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
			received = true;
			break;
		case HIT_MESSAGE_HIP_DROP: {
			bool weak;
			s32 idx = unk114;
			if (idx == BossHanachanPartsRecvWeak(this))
				weak = true;
			else
				weak = false;
			EnumBossHanachanAnmKind anm = mCurrentAnm;
			switch (anm) {
			case BOSS_HANACHAN_ANM_UNK2:
			case BOSS_HANACHAN_ANM_UNK3:
			case BOSS_HANACHAN_ANM_UNK5:
			case BOSS_HANACHAN_ANM_UNK13:
			case BOSS_HANACHAN_ANM_UNK16:
			case BOSS_HANACHAN_ANM_UNK17:
				if (weak) {
					setAnm_(BOSS_HANACHAN_ANM_UNK6,
					        BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
					unkFC->execDamage();
				} else {
					if (anm == BOSS_HANACHAN_ANM_UNK13)
						restartBck_();
					else
						setAnm_(BOSS_HANACHAN_ANM_UNK13,
						        BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
				}
				received = true;
				break;
			}
			break;
		}
		}
	}
	return received;
}

BOOL TBossHanachanPartsHead::receiveMessage(THitActor*, u32 message)
{
	if (SMSGetMarDirector()->isThing())
		return FALSE;
	bool received = false;
	if (isReactToTrampleOrHipDrop_()) {
		switch ((int)message) {
		case HIT_MESSAGE_TRAMPLE:
			if (mCurrentAnm == BOSS_HANACHAN_ANM_UNK5)
				restartBck_();
			else
				setAnm_(BOSS_HANACHAN_ANM_UNK5,
				        BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
			received = true;
			break;
		case HIT_MESSAGE_HIP_DROP:
			setAnm_(BOSS_HANACHAN_ANM_UNK6,
			        BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
			unk100->onWaterHitCounter();
			received = true;
			break;
		}
	}
	return received;
}

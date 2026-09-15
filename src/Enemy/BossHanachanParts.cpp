#include <Enemy/BossHanachan.hpp>
#include <Camera/cameralib.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <MarioUtil/MapUtil.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/ShadowUtil.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <System/MarDirector.hpp>
#include <NPC/NpcInbetween.hpp>
#include <Player/ModelWaterManager.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

class TFootHitActor : public TWaterHitActor {
public:
	virtual ~TFootHitActor() { }
	/* 0x6C */ MtxPtr mJointMtx;
};

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
	mMActorKeeper = unkFC->mMActorKeeper;
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
    , unk120(0.0f, 0.0f, 0.0f)
    , unk12C(0.0f, 0.0f, 0.0f)
    , unk138(0.0f, 0.0f, 0.0f)
    , unk144(0.0f)
    , unk148(0.0f)
    , unk154(0.0f, 0.0f, 0.0f)
{
	J3DModel* model = getModel();
	JUTNameTab* names = model->getModelData()->getJointName();
	CalcMtxPtrFromJointName(names, cLegJointName_L3, model, &mLeftLegMtx);
	CalcMtxPtrFromJointName(names, cLegJointName_R3, model, &mRightLegMtx);
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

void TBossHanachanPartsBase::initMapCollisionAndHitActor_(TIdxGroupObj*)
{
	// TODO
}

void TBossHanachanPartsBody::initFootHitActor_(TIdxGroupObj*)
{
	// TODO
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
	if (unkFC->mSpine->getCurrentNerve() == &TNerveBossHanachanDead::theNerve()
	    && unkFC->mSpine->getTime() > 200)
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
	if (unkFC->mSpine->getLatestNerve() == &TNerveBossHanachanDamage::theNerve()) {
		SMS_AddDamageFogEffect(data, position, graphics);
		if (isBody)
			for (u16 i = 0; i < materialCount; ++i)
				data->getMaterialNodePointer(i)->change();
		if (unk10C == 0)
			mMActor->getModel()->unlock();
	} else {
		SMS_ResetDamageFogEffect(data);
	}
}

bool TBossHanachanPartsBase::isCurBckAlreadyEnd_() const
{
	bool result = true;
	if (mMActor) {
		J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(0);
		if (ctrl) {
			result = (ctrl->checkState(J3DFrameCtrl::STATE_COMPLETED_ONCE)
			          || ctrl->checkState(J3DFrameCtrl::STATE_LOOPED_ONCE))
			                 || ctrl->getFrame() + 0.1f >= ctrl->getEnd();
		}
	}
	return result;
}

void TBossHanachanPartsBase::copyFrameFromOldAnmToNewAnm_()
{
	J3DAnmTransform* anm = mMActor->getBckAnm();
	J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(0);
	if (anm && ctrl) {
		f32 frame = mMActor->getBckOldMotionBlendFrame();
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

BOOL TBossHanachanPartsBase::isMarioOn_() const { return FALSE; /* TODO */ }

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
	const TNerveBase<TLiveActor>* nerve = unkFC->mSpine->getLatestNerve();
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

void TBossHanachanPartsBase::considerSetAnm_(EnumBossHanachanNerveAnm)
{
	// TODO
}

BOOL TBossHanachanPartsBody::setAnm_(EnumBossHanachanAnmKind anm,
                                    EnumBossHanachanStopMotionBlendOnOff blend)
{
	static const int sBodyBckIndex[] = {
		19, 15, 10, 13, 0, 12, 9, 2, 3, 4, 5, 6, 7, 8, 16, 1, 17, 18
	};
	BOOL changed = FALSE;
	if (mCurrentAnm != anm) {
		mPreviousAnm = mCurrentAnm;
		mCurrentAnm = anm;
		int current = mMActor->getCurAnmIdx(ANM_TYPE_BCK);
		int next = sBodyBckIndex[anm];
		if (next != current) {
			int index = next;
			if (unk114 == unkFC->mWeakBodyIndex) {
				switch (anm) {
				case BOSS_HANACHAN_ANM_UNK2: index = 11; break;
				case BOSS_HANACHAN_ANM_UNK3: index = 14; break;
				}
			}
			mMActor->setBckFromIndex(index);
			changed = TRUE;
			if (blend == BOSS_HANACHAN_STOP_MOTION_BLEND_ON)
				mInbetween->startMotionBlend();
			else
				mInbetween->stopMotionBlend();
			setCurAnmSound();
		}
		if (anm == BOSS_HANACHAN_ANM_UNK15) {
			mMActor->setBrkFromIndex(0);
			mMActor->getFrameCtrl(ANM_TYPE_BRK)->setAttribute(J3DFrameCtrl::ATTR_ONCE);
			mMActor->getModel()->unlock();
		}
	}
	return changed;
}

BOOL TBossHanachanPartsHead::setAnm_(EnumBossHanachanAnmKind anm,
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
	BOOL changed = FALSE;
	if (mCurrentAnm != anm) {
		mPreviousAnm = mCurrentAnm;
		mCurrentAnm = anm;
		if (sHeadBckIndex[anm] != mMActor->getCurAnmIdx(ANM_TYPE_BCK)) {
			mMActor->setBckFromIndex(sHeadBckIndex[anm]);
			changed = TRUE;
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
				setAnm_(BOSS_HANACHAN_ANM_UNK5, BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
			received = true;
			break;
		case HIT_MESSAGE_HIP_DROP: {
			bool weak;
			if (unk114 == unkFC->mWeakBodyIndex)
				weak = true;
			else
				weak = false;
			switch (mCurrentAnm) {
			case BOSS_HANACHAN_ANM_UNK2:
			case BOSS_HANACHAN_ANM_UNK3:
			case BOSS_HANACHAN_ANM_UNK5:
			case BOSS_HANACHAN_ANM_UNK13:
			case BOSS_HANACHAN_ANM_UNK16:
			case BOSS_HANACHAN_ANM_UNK17:
				if (weak) {
					setAnm_(BOSS_HANACHAN_ANM_UNK6, BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
					unkFC->execDamage();
				} else {
					if (mCurrentAnm == BOSS_HANACHAN_ANM_UNK13)
						restartBck_();
					else
						setAnm_(BOSS_HANACHAN_ANM_UNK13, BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
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
	if (gpMarDirector->isThing())
		return FALSE;
	bool received = false;
	if (isReactToTrampleOrHipDrop_()) {
		switch ((int)message) {
		case HIT_MESSAGE_TRAMPLE:
			if (mCurrentAnm == BOSS_HANACHAN_ANM_UNK5)
				restartBck_();
			else
				setAnm_(BOSS_HANACHAN_ANM_UNK5, BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
			received = true;
			break;
		case HIT_MESSAGE_HIP_DROP:
			setAnm_(BOSS_HANACHAN_ANM_UNK6, BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
			unk100->onWaterHitCounter();
			received = true;
			break;
		}
	}
	return received;
}

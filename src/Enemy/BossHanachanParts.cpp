#include <Enemy/BossHanachan.hpp>
#include <Camera/cameralib.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <MarioUtil/MapUtil.hpp>
#include <NPC/NpcInbetween.hpp>
#include <Player/ModelWaterManager.hpp>
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
	*result = model->getAnmMtx(names->getIndex(name));
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
	// TODO: recover the boss parameters and model-keeper setup.
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

void TBossHanachanPartsBase::changeTumbleAnmRate_() { /* TODO */ }

void TBossHanachanPartsBase::moveMapCollision_()
{
	JGeometry::TVec3<f32> position(unk108[0][3], unk108[1][3], unk108[2][3]);
	unk104->moveTrans(position);
}

void TBossHanachanPartsBase::entryCircleShadow_() { /* TODO */ }
void TBossHanachanPartsBase::setDamageFog_(JDrama::TGraphics*) { /* TODO */ }

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

BOOL TBossHanachanPartsBase::isReactToTrampleOrHipDrop_() const
{
	return FALSE; // TODO
}

void TBossHanachanPartsBase::considerSetAnm_(EnumBossHanachanNerveAnm)
{
	// TODO
}

BOOL TBossHanachanPartsBody::setAnm_(EnumBossHanachanAnmKind,
                                    EnumBossHanachanStopMotionBlendOnOff)
{
	return FALSE; // TODO
}

BOOL TBossHanachanPartsHead::setAnm_(EnumBossHanachanAnmKind,
                                    EnumBossHanachanStopMotionBlendOnOff)
{
	return FALSE; // TODO
}

BOOL TBossHanachanPartsBody::receiveMessage(THitActor*, u32)
{
	return FALSE; // TODO
}

BOOL TBossHanachanPartsHead::receiveMessage(THitActor*, u32)
{
	return FALSE; // TODO
}

#include <Enemy/Gesso.hpp>
#include <Enemy/Conductor.hpp>
#include <Camera/Camera.hpp>
#include <Player/MarioAccess.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <M3DUtil/MActor.hpp>
#include <MoveBG/ItemManager.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/PollutionManager.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <System/EmitterViewObj.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Strategy.hpp>
#include <Strategic/SharedParts.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DNode.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

TGesso* gpCurGesso;

const char* rikuGesso_bastable[] = {
	nullptr,
	nullptr,
	"/scene/rikuGesso/bas/geso_180turn1.bas",
	"/scene/rikuGesso/bas/geso_down1.bas",
	nullptr,
	"/scene/rikuGesso/bas/geso_gero1.bas",
	"/scene/rikuGesso/bas/geso_hit1.bas",
	"/scene/rikuGesso/bas/geso_land1.bas",
	"/scene/rikuGesso/bas/geso_mahi_end1.bas",
	"/scene/rikuGesso/bas/geso_mahi_loop1.bas",
	"/scene/rikuGesso/bas/geso_mahi_start1.bas",
	"/scene/rikuGesso/bas/geso_punch1.bas",
	"/scene/rikuGesso/bas/geso_run1.bas",
	"/scene/rikuGesso/bas/geso_search1.bas",
	nullptr,
	"/scene/rikuGesso/bas/geso_surf1.bas",
	"/scene/rikuGesso/bas/geso_turn_down1.bas",
	nullptr,
	"/scene/rikuGesso/bas/geso_turn_hit1.bas",
	"/scene/rikuGesso/bas/geso_turn_up1.bas",
	nullptr,
	"/scene/rikuGesso/bas/geso_wait1.bas",
	nullptr,
};

TGessoSaveLoadParams::TGessoSaveLoadParams(const char* path)
    : TWalkerEnemyParams(path)
    , PARAM_INIT(mSLSearchLengthOnObj, 500.0f)
    , PARAM_INIT(mSLSearchAngleOnObj, 180.0f)
    , PARAM_INIT(mSLSearchAwareOnObj, 500.0f)
    , PARAM_INIT(mSLPollutionLength, 200.0f)
    , PARAM_INIT(mSLPollutionInterval, 300)
    , PARAM_INIT(mSLPolluteObjGravity, 0.01f)
    , PARAM_INIT(mSLPolluteObjSpeed, 5.0f)
    , PARAM_INIT(mSLPolluteObjLinerSp, 5.0f)
    , PARAM_INIT(mSLPolluteObjLinerG, 0.01f)
    , PARAM_INIT(mSLDropGravityY, 0.1f)
    , PARAM_INIT(mSLBodyAngMax, 40.0f)
    , PARAM_INIT(mSLPolluteModelScale, 2.0f)
    , PARAM_INIT(mSLTurnLength, 300.0f)
    , PARAM_INIT(mSLHitWaterSpXZ, 12.0f)
    , PARAM_INIT(mSLHitWaterSpY, 5.0f)
{
	TParams::load(mPrmPath);
}

void TGessoPolluteModelManager::init(TLiveActor* param_1)
{
	TEnemyPolluteModelManager::init(param_1);

	void* res = JKRFileLoader::getGlbResource(
	    "/scene/rikuGesso/stamp_gero_model1.bmd");
	SDLModelData* modelData
	    = new SDLModelData(J3DModelLoaderDataBase::load(res, 0x10220000));

	for (int i = 0; i < 5; ++i)
		unk18[i] = new TGessoPolluteModel(param_1, modelData);
}

void TGessoPolluteModel::setAnm()
{
	unk10->getMActor()->setBckFromIndex(0x16);
	unk10->getMActor()->getFrameCtrl(0)->setFrame(0.0f);
}

TGessoManager::TGessoManager(const char* name)
    : TSmallEnemyManager(name)
    , unk60(nullptr)
{

	gpCurGesso = nullptr;
}

void TGessoManager::load(JSUMemoryInputStream& stream)
{
	TSmallEnemyManager::load(stream);
	unk38 = new TGessoSaveLoadParams("/enemy/gesso.prm");
}

void TGessoManager::loadAfter() { TSmallEnemyManager::loadAfter(); }

TSmallEnemy* TGessoManager::createEnemyInstance() { return new TGesso; }

void TGessoManager::clipEnemies(JDrama::TGraphics* param_1)
{
	f32 radius;
	f32 far;
	if (unk38 == nullptr) {
		far    = gpConductor->getCondParams().mEnemyFarClip.get();
		radius = 300.0f;
	} else {
		far    = unk38->mSLFarClip.get();
		radius = unk38->mSLClipRadius.get();
	}

	SetViewFrustumClipCheckPerspective(
	    gpCamera->getFovy(), gpCamera->getAspect(), param_1->mNearPlane, far);

	for (int i = 0; i < mObjNum; ++i) {
		TGesso* gesso = (TGesso*)unk18[i];

		if (ViewFrustumClipCheck(param_1, &gesso->mPosition, radius))
			gesso->offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		else
			gesso->onLiveFlag(LIVE_FLAG_CLIPPED_OUT);

		if (!gesso->getPolluteObj()->isUnk150Zero()) {
			if (ViewFrustumClipCheck(
			        param_1, &gesso->getPolluteObj()->mPosition, radius))
				gesso->getPolluteObj()->offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
			else
				gesso->getPolluteObj()->onLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		}
	}
}

void TGessoManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	gpCurGesso = nullptr;
	TEnemyManager::perform(param_1, param_2);
	for (int i = 0; i < getActiveObjNum(); ++i)
		getObj(i)->mPolluteObj->perform(param_1, param_2);

	unk60->perform(param_1, param_2);
}

void TGessoManager::initSetEnemies()
{
	unk60 = new TGessoPolluteModelManager;
	unk60->init((TLiveActor*)unk18[0]);
}

void TGessoManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "geso_model1.bmd", 0x10230000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TGessoManager::requestPolluteModel(JGeometry::TVec3<float>& position,
                                        JGeometry::TVec3<float>& scale)
{
	unk60->generatePolluteModel(position, scale);
}

static int GessoBodyCallback(J3DNode* param_1, int param_2)
{
	if (param_2 == 0) {
		if (gpCurGesso == nullptr || !gpCurGesso->isNotWandering())
			return true;

		J3DJoint* joint = (J3DJoint*)param_1;
		MtxPtr anmMtx   = gpCurGesso->getModel()->getAnmMtx(joint->getJntNo());

		f32 scale = gpCurGesso->getBodyScale();
		Mtx local_44;
		local_44[0][0] = scale;
		local_44[0][1] = 0.0f;
		local_44[0][2] = 0.0f;
		local_44[0][3] = 0.0f;

		local_44[1][0] = 0.0f;
		local_44[1][1] = scale;
		local_44[1][2] = 0.0f;
		local_44[1][3] = 0.0f;

		local_44[2][0] = 0.0f;
		local_44[2][1] = 0.0f;
		local_44[2][2] = scale;
		local_44[2][3] = 0.0f;

		f32 maxAngle = gpCurGesso->getSaveParams()->mSLBodyAngMax.get();
		f32 angle = MsClamp(gpCurGesso->mBodyTrackingAngle - 90.0f, -maxAngle,
		                    maxAngle);

		f32 s = JMASin(angle);
		f32 c = JMACos(angle);

		Mtx local_74;
		local_74[0][0] = 1.0f;
		local_74[0][1] = 0.0f;
		local_74[0][2] = 0.0f;
		local_74[0][3] = 0.0f;

		local_74[1][0] = 0.0f;
		local_74[1][1] = c;
		local_74[1][2] = -s;
		local_74[1][3] = 0.0f;

		local_74[2][0] = 0.0f;
		local_74[2][1] = s;
		local_74[2][2] = c;
		local_74[2][3] = 0.0f;

		MTXConcat(anmMtx, local_74, anmMtx);
		MTXConcat(anmMtx, local_44, anmMtx);
		MTXConcat(J3DSys::mCurrentMtx, local_74, J3DSys::mCurrentMtx);
		MTXConcat(J3DSys::mCurrentMtx, local_44, J3DSys::mCurrentMtx);
	}
	return true;
}

u8 TGesso::mBodyJntIndex       = 12;
u8 TGesso::mMouthJntIndex      = 13;
f32 TGesso::mBodyRotSpeed      = 0.5f;
f32 TGesso::mPollRange         = 8.0f;
f32 TGesso::mThroughHoseiDistY = 200.0f;
f32 TGesso::mAngTestY          = 0.15f;

TGesso::TGesso(const char* name)
    : TWalkerEnemy(name)
    , mAttackCooldown(0)
    , mPollutionTimer(0)
    , mState(STATE_BEAM_CHILLING)
    , mIsRightSideUp(true)
    , unk1A1(1)
    , unk1A4(0.0f)
    , mPolluteObj(nullptr)
    , mGessoType(TYPE_REGULAR)
    , unk1B4(0)
    , unk1C4(1)
    , mStayYaw(0.0f)
    , mBodyTrackingAngle(90.0f)
    , unk1D0(0.0f)
    , unk1D8(0)
    , mNeedsLanding(false)
{
}

static int GessoBodyCallback(J3DNode*, int);

void TGesso::init(TLiveManager* param_1)
{
	TWalkerEnemy::init(param_1);
	mActorType = 0x10000007;
	unk150     = 17;
	unk1E8     = (TGessoSaveLoadParams*)getSaveParam();
	getMActor()->setJointCallback(mBodyJntIndex, &GessoBodyCallback);
	mPolluteObj = new TGessoPolluteObj;
	mPolluteObj->loadInit(this, "gero_model1.bmd");
	setBckAnm(21);

	J3DFrameCtrl* ctrl0 = getMActor()->getFrameCtrl(0);

	f32 endFrame = ctrl0->getEndFrame();
	f32 inv      = 1.0f / mManager->getCapacity();
	f32 index    = getInstanceIndex();
	f32 frame    = endFrame * index * inv;
	ctrl0->setFrame(frame);
}

void TGesso::reset()
{
	gpCurGesso = this;
	TWalkerEnemy::reset();
	if (mGessoType != TYPE_REGULAR) {
		mState = STATE_WANDERING;
		mSpine->initWith(&TNerveWalkerGraphWander::theNerve());
		unk158 = 1.0f;
	} else {
		mState = STATE_BEAM_CHILLING;
		mSpine->initWith(&TNerveGessoStay::theNerve());
		unk158 = 0.0f;
	}

	offLiveFlag(LIVE_FLAG_UNK1000);
	mNeedsLanding  = false;
	mIsRightSideUp = true;
	unk1A4         = mPosition.y;
	unk1D0         = 0.0f;
	setBckAnm(21);

	J3DFrameCtrl* ctrl0 = getMActor()->getFrameCtrl(0);

	f32 inv      = 1.0f / mManager->getObjNum();
	f32 index    = getInstanceIndex();
	f32 endFrame = ctrl0->getEndFrame();
	f32 frame    = endFrame * index * inv;
	ctrl0->setFrame(frame);
}

void TGesso::load(JSUMemoryInputStream& stream)
{
	TWalkerEnemy::load(stream);
	unk1C4 = 1;
	if (mRotation.y < 0.0f)
		unk1C4 = 0;

	if (mRotation.y == 0.0f || mRotation.y == 180.0f) {
		unk1A1   = 1;
		mStayYaw = 0.0f;
		if (unk1C4 == 0)
			mStayYaw = 180.0f;
	} else {
		unk1A1   = 0;
		mStayYaw = 90.0f;
		if (unk1C4 == 0)
			mStayYaw = 270.0f;
	}

	reset();
	mRotation.set(0.0f, 0.0f, 0.0f);
}

void TGesso::behaveToWater(THitActor* param_1)
{
	unk165 = 0;
	if (mSpine->getCurrentNerve() == &TNerveGessoFreeze::theNerve())
		unk165 = 1;

	mSprayedByWaterCooldown = 0;
	if (mSpine->getCurrentNerve() != &TNerveSmallEnemyDie::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveGessoFall::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveGessoFreeze::theNerve()
	    && mState != STATE_ROLLING) {
		if (mState == STATE_BEAM_CHILLING) {
			if (mIsRightSideUp) {
				JGeometry::TVec3<f32> local_1c(
				    mPosition.x - SMS_GetMarioPos().x, 0.0f,
				    mPosition.z - SMS_GetMarioPos().z);

				if (local_1c.x == 0.0f && local_1c.z == 0.0f)
					local_1c.x = 1.0f;

				MsVECNormalize(&local_1c, &local_1c);
				f32 hitWaterSpXZ = unk1E8->mSLHitWaterSpXZ.get();
				f32 hitWaterSpY  = unk1E8->mSLHitWaterSpY.get();
				local_1c.x *= hitWaterSpXZ;
				if (abs(local_1c.x) < 5.0f) {
					if (local_1c.x > 0.0f)
						local_1c.x = 5.0f;
					else
						local_1c.x = -5.0f;
				}
				local_1c.z *= hitWaterSpXZ;
				if (abs(local_1c.z) < 5.0f) {
					if (local_1c.z > 0.0f)
						local_1c.z = 5.0f;
					else
						local_1c.z = -5.0f;
				}
				local_1c.y *= hitWaterSpY;
				mVelocity = local_1c;
				mPosition.y += 2.0f;
				onLiveFlag(LIVE_FLAG_AIRBORNE);
			}

			mSpine->pushNerve(&TNerveGessoFall::theNerve());
		} else {
			mSpine->pushNerve(&TNerveGessoFreeze::theNerve());
		}
	}
}

void TGesso::walkBehavior(int param_1, float param_2)
{
	if (getCurAnmFrameNo(0) > 20.0f)
		TWalkerEnemy::walkBehavior(param_1, param_2);

	if (param_1 == 2)
		polluteBehavior();

	if (0 < unk1B4 && unk1B4 < 300)
		unk1B4 += 1;
	else
		unk1B4 = 0;
}

bool TGesso::isResignationAttack()
{
	if (TWalkerEnemy::isResignationAttack())
		return true;
	else
		return false;
}

bool TGesso::doKeepDistance()
{
	if (!isBckAnm(9) || SMS_GetMarioPos().y > mPosition.y + 50.0f)
		return false;

	return true;
}

void TGesso::attackToMario()
{
	if (mState != STATE_WANDERING) {
		SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
		mAttackCooldown = 1;
		return;
	}

	if (mSpine->getCurrentNerve() != &TNerveGessoPunch::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveGessoFreeze::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveSmallEnemyDie::theNerve()) {
		mAttackCooldown = 1;
		mSpine->pushNerve(&TNerveWalkerPostAttack::theNerve());
		mSpine->pushNerve(&TNerveGessoPunch::theNerve());
	}

	if (mSpine->getCurrentNerve() == &TNerveGessoPunch::theNerve()) {
		if (mMActor->getFrameCtrl(0)->checkPass(10.0f))
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);

		return;
	}

	if (!isBckAnm(9))
		SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
}

void TGesso::setBehavior()
{
	if (mAttackCooldown > 0)
		mAttackCooldown += 1;

	if (mAttackCooldown > 200)
		mAttackCooldown = 0;

	if (isAirborne() && mPosition.y > mGroundHeight + 250.0f
	    && mSpine->getCurrentNerve() != &TNerveWalkerGenerate::theNerve()) {
		mNeedsLanding = true;
	}

	if (!isAirborne() && mNeedsLanding
	    && (mSpine->getCurrentNerve() == &TNerveWalkerGraphWander::theNerve()
	        || mSpine->getCurrentNerve() == &TNerveWalkerAttack::theNerve())) {
		mSpine->pushNerve(&TNerveGessoLand::theNerve());
	}

	if (!isAirborne())
		mNeedsLanding = false;
}

void TGesso::polluteBehavior()
{
	mPollutionTimer += 1;

	if (!checkCurAnmEnd(0))
		return;

	if (mSpine->getCurrentNerve() == &TNerveGessoPollute::theNerve())
		return;

	if (mPollutionTimer < unk1E8->mSLPollutionInterval.get())
		return;

	if (!MsIsInSight(mPosition, getSightDirection(), SMS_GetMarioPos(),
	                 unk1E8->mSLPollutionLength.get(),
	                 unk1E8->mSLSearchAngleOnObj.get(), 0.0f))
		return;

	mPollutionTimer = 0;
	mPolluteObj->kill();
	mSpine->pushNerve(&TNerveGessoPollute::theNerve());
}

void TGesso::setPolluteGoal()
{
	f32 polluteObjGravity = unk1E8->mSLPolluteObjGravity.get();
	f32 polluteObjSpeed   = unk1E8->mSLPolluteObjSpeed.get();

	if (!mIsRightSideUp)
		polluteObjSpeed = 0.0f;

	if (unk1D8 == 0) {
		mPolluteVelocity.set(
		    SMS_GetMarioPos().x + FakeRandInterval(-100.0f, 100.0f).get(),
		    SMS_GetMarioPos().y,
		    SMS_GetMarioPos().z + FakeRandInterval(-100.0f, 100.0f).get());

		JGeometry::TVec3<f32> local;
		calcVelocityToJumpToY(local, polluteObjSpeed, polluteObjGravity);
		mPolluteVelocity = local;
	} else {
		mPolluteVelocity = SMS_GetMarioPos();
		mPolluteVelocity.x -= mPosition.x;
		mPolluteVelocity.y = 0.0f;
		mPolluteVelocity.z -= mPosition.z;
		MsVECNormalize(&mPolluteVelocity, &mPolluteVelocity);
		f32 polluteObjLinerSp = unk1E8->mSLPolluteObjLinerSp.get();
		mPolluteVelocity.x *= polluteObjLinerSp;
		mPolluteVelocity.y = 0.0f;
		mPolluteVelocity.z *= polluteObjLinerSp;
	}
}

void TGesso::pollute()
{
	if (mState != STATE_WANDERING && !mIsRightSideUp)
		mPolluteVelocity.y = -3.0f;

	mPolluteObj->setVelocity(mPolluteVelocity);
	mPolluteObj->pollute();

	JGeometry::TVec3<f32> tmp;
	tmp.sub(mPolluteVelocity, mPosition);
	tmp.y                    = 0.0f;
	mPolluteObj->mRotation.y = MsGetRotFromZaxisY(tmp);

	MtxPtr mtx = getMActor()->getModel()->getAnmMtx(mMouthJntIndex);

	JGeometry::TVec3<f32> local_2c(0.0f, 0.0f, 100.0f);
	Mtx afStack_5c;
	if (mState == STATE_WANDERING) {
		MsMtxSetRotRPH(afStack_5c, 0.0f, mRotation.y, 0.0f);
	} else {
		MsMtxSetRotRPH(afStack_5c, 0.0f, mRotation.y + mStayYaw, 0.0f);
	}
	MTXMultVec(afStack_5c, &local_2c, &local_2c);
	mPolluteObj->mPosition.x = mtx[0][3] + local_2c.x;
	mPolluteObj->mPosition.y = mtx[1][3];
	mPolluteObj->mPosition.z = mtx[2][3] + local_2c.z;
}

void TGesso::isUseBodyCallBack() const { }

void TGesso::setAfterDeadEffect()
{
	mPolluteObj->kill();
	TSmallEnemy::setAfterDeadEffect();
}

void TGesso::setDeadAnm()
{
	if (mGroundPlane->isWaterSurface())
		onLiveFlag(LIVE_FLAG_HIDDEN);
	else if (mGroundPlane->checkFlag(BG_CHECK_FLAG_ILLEGAL)
	         || mGessoType != TYPE_SURF)
		setBckAnm(3);
	else
		setBckAnm(15);
}

f32 TGesso::getGravityY() const
{
	if (mState != STATE_WANDERING) {
		f32 dropGravityY = unk1E8->mSLDropGravityY.get();
		if (!mIsRightSideUp)
			return dropGravityY;

		return dropGravityY / 2.0f;
	}
	return mGravity;
}

void TGesso::setWalkAnm() { setBckAnm(12); }

void TGesso::setWaitAnm()
{
	if (mState == STATE_WANDERING || mIsRightSideUp) {
		setBckAnm(21);
		unk1D0 = 0.0f;
	} else {
		setBckAnm(20);
		unk1D0 = mDamageHeight;
	}
}

void TGesso::bind()
{
	switch (mState) {
	case STATE_WANDERING:
	case 4:
		TSmallEnemy::bind();
		break;

	case STATE_FALLING:
		mPosition += mVelocity;
		mVelocity.y -= getGravityY();
		if (mVelocity.y < mVelocityMinY)
			mVelocity.y = mVelocityMinY;
		if (isBckAnm(18)) // geso_turn_hit1.bas
			mState = 4;
		else if (mThroughHoseiDistY * 0.5f + mPosition.y < unk1A4)
			mState = 4;
		break;

	case STATE_ROLLING:
		unk1D0 = 0.0f;
		// FALLTHROUGH
	case STATE_BEAM_CHILLING:
		mPosition.y = unk1A4 - unk1D0;
		break;
	}

	if (isNotWandering()) {
		f32 f8;
		f32 f7;
		f32 f1 = 1.0f;

		if (!mIsRightSideUp)
			f1 = -1.0f;

		if (unk1A1 == 0) {
			if (unk1C4 != 0) {
				f8 = f1;
				f7 = 0.0f;
			} else {
				f8 = f1 * -1.0f;
				f7 = 0.0f;
			}
		} else {
			if (unk1C4 != 0) {
				f8 = 0.0f;
				f7 = f1 * -1.0f;
			} else {
				f8 = 0.0f;
				f7 = f1;
			}
		}

		f32 fVar3 = SMS_GetMarioPos().x - mPosition.x;
		f32 fVar4 = SMS_GetMarioPos().z - mPosition.z;
		JGeometry::TVec3<f32> var1(f8, f7, 0.0f);
		JGeometry::TVec3<f32> var2(fVar3, fVar4, 0.0f);
		JGeometry::TVec3<f32> local_48;
		local_48.cross(var1, var2);
		f32 cos   = var1.dot(var2);
		f32 sin   = MsVECMag2(&local_48);
		f32 angle = MsAtan2(sin, cos);
		if (mBodyTrackingAngle != angle) {
			if (mBodyTrackingAngle < angle) {
				mBodyTrackingAngle += mBodyRotSpeed;
				if (mBodyTrackingAngle > angle)
					mBodyTrackingAngle = angle;
			} else {
				mBodyTrackingAngle -= mBodyRotSpeed;
				if (mBodyTrackingAngle < angle)
					mBodyTrackingAngle = angle;
			}
		}
	}
}

void TGesso::kill()
{
	if (!mIsRightSideUp)
		return;

	TSmallEnemy::kill();
	mPolluteObj->kill();
}

void TGesso::calcRootMatrix()
{
	gpCurGesso = this;
	if (mState == STATE_WANDERING) {
		TSpineEnemy::calcRootMatrix();
		return;
	}

	if (!isEaten()) {
		J3DModel* model = mMActor->getModel();
		MtxPtr mA       = model->getBaseTRMtx();

		MsMtxSetXYZRPH(mA, mPosition.x, mPosition.y + unk1D0, mPosition.z,
		               mRotation.x, mRotation.y, mRotation.z);

		f32 s = JMASin(mStayYaw);
		f32 c = JMACos(mStayYaw);
		Mtx local_68;

		local_68[0][0] = c;
		local_68[0][1] = 0.0f;
		local_68[0][2] = s;
		local_68[0][3] = 0.0f;

		local_68[1][0] = 0.0f;
		local_68[1][1] = 1.0f;
		local_68[1][2] = 0.0f;
		local_68[1][3] = 0.0f;

		local_68[2][0] = -s;
		local_68[2][1] = 0.0f;
		local_68[2][2] = c;
		local_68[2][3] = 0.0f;

		MTXConcat(mA, local_68, mA);

		model->setBaseScale(mScaling);
	}
}

void TGesso::genRandomItem()
{
	if (isBckAnm(15)) {
		// yeah
		if (gpItemManager->makeObjAppear(mPosition.x, mPosition.y, mPosition.z,
		                                 0x400000BC, true))
			return;
		else
			return;
	}

	TSmallEnemy::genRandomItem();
}

void TGesso::behaveToFindMario()
{
	if (unk150 & 2) {
		mSpine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
		mSpine->pushAfterCurrent(&TNerveWalkerEscape::theNerve());
		mSpine->pushAfterCurrent(&TNerveSmallEnemyJump::theNerve());
	} else {
		setGoalPathMario();
		mSpine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
		mSpine->pushAfterCurrent(&TNerveWalkerAttack::theNerve());
		if (unk1B4 == 0) {
			mSpine->pushAfterCurrent(&TNerveGessoFindMario::theNerve());
			unk1B4 = 1;
		}
	}
}

void TGesso::rollCheck()
{
	if (mAttackCooldown != 0)
		return;

	f32 aware = unk1E8->mSLSearchAwareOnObj.get();
	if (MsIsInSight(mPosition, getSightDirection(), SMS_GetMarioPos(),
	                unk1E8->mSLSearchLengthOnObj.get(),
	                unk1E8->mSLSearchAngleOnObj.get(), aware)) {
		if ((mIsRightSideUp && mPosition.y > SMS_GetMarioPos().y + 10.0f)
		    || (!mIsRightSideUp && mPosition.y < SMS_GetMarioPos().y - 10.0f)) {
			onHitFlag(HIT_FLAG_NO_COLLISION);
			mState = STATE_ROLLING;
			mSpine->pushNerve(&TNerveGessoRolling::theNerve());
			if (mIsRightSideUp)
				onLiveFlag(LIVE_FLAG_UNK8);
			mAttackCooldown = 0;
		}
	}

	f32 turnLength = unk1E8->mSLTurnLength.get();
	updateSquareToMario();
	if (mDistToMarioSquared < turnLength * turnLength && mIsRightSideUp
	    && !MsIsInSight(mPosition, getSightDirection(), SMS_GetMarioPos(),
	                    unk1E8->mSLSearchLengthOnObj.get(),
	                    unk1E8->mSLSearchAngleOnObj.get(), 0.0f)) {
		const TNerveBase<TLiveActor>* nerve = &TNerveGessoTurn::theNerve();
		mSpine->pushNerve(nerve);
	}
}

void TGesso::rollEnd()
{
	offHitFlag(HIT_FLAG_NO_COLLISION);
	mState         = STATE_BEAM_CHILLING;
	mIsRightSideUp = !mIsRightSideUp;
	if (mIsRightSideUp)
		offLiveFlag(LIVE_FLAG_UNK8);
}

void TGesso::modifyRotate() { }

void TGesso::fallEnd()
{
	mState = STATE_WANDERING;
	unk158 = 1.0f;
	offLiveFlag(LIVE_FLAG_UNK8);
	mRotation.set(0.0f, 0.0f, 0.0f);
	mIsRightSideUp = true;
}

void TGesso::turnIn()
{
	setBckAnm(2);
	mTurnAngle = 0.0f;
	onHitFlag(HIT_FLAG_NO_COLLISION);
}

bool TGesso::turning()
{
	if (mTurnAngle + 7.2f <= 180.0f) {
		mBodyTrackingAngle = 90.0f;
		mRotation.y += 7.2f;
		mTurnAngle += 7.2f;
		return false;
	}

	mTurnAngle = 180.0f;
	if (checkCurAnmEnd(0))
		return true;
	else
		return false;
}

void TGesso::turnOut()
{
	mTurnAngle = 0.0f;
	unk1C4     = !unk1C4;
	offHitFlag(HIT_FLAG_NO_COLLISION);
}

// TODO: the size & logic matches but it won't inline =(
inline bool TGesso::checkDropInWater()
{
	// Don't skip your calculus class, kids.
	JGeometry::TVec3<f32> position = mPosition;
	JGeometry::TVec3<f32> velocity = mVelocity;
	for (int i = 0; i < 50; ++i) {
		position += velocity;
		velocity.y -= getGravityY();
		if (position.y < mGroundHeight)
			break;
	}

	const TBGCheckData* local_34;
	gpMap->checkGround(position.x, mHeadHeight * 2.0f + position.y, position.z,
	                   &local_34);

	if (local_34->isWaterSurface())
		return true;
	else
		return false;
}

void TGesso::initAttacker(THitActor* param_1)
{
	mRotation  = param_1->getRotation();
	unk184     = 1;
	mGessoType = TYPE_LAND;
	reset();
}

const char** TGesso::getBasNameTable() const { return rikuGesso_bastable; }

void TSurfGesso::load(JSUMemoryInputStream& stream)
{
	TGesso::load(stream);
	mGessoType = TYPE_SURF;
	reset();
}

void TSurfGesso::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 2)
		offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
	TGesso::perform(param_1, param_2);
}

void TLandGesso::load(JSUMemoryInputStream& stream)
{
	TGesso::load(stream);
	mGessoType = TYPE_LAND;
	reset();
}

TGessoPolluteObj::TGessoPolluteObj(const char* name)
    : TEnemyAttachment(name)
    , unk16C(0)
{
	unk164 = 1.4f;
}

void TGessoPolluteObj::loadInit(TSpineEnemy* param_1, const char* param_2)
{
	TEnemyAttachment::loadInit(param_1, param_2);

	unk16C = (TGesso*)unk160;
	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(this);

	THitActor::initHitActor(0x10000006, 1, -0x80000000, 10.0f, 10.0f, 10.0f,
	                        10.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);
	unk150       = 0;
	mGroundPlane = TMap::getIllegalCheckData();
}

f32 TGessoPolluteObj::getNowGravity()
{
	f32 gravity = unk16C->getSaveParams()->mSLPolluteObjGravity.get();
	if (unk16C->unk1D8 == 0)
		return gravity;

	return unk16C->getSaveParams()->mSLPolluteObjLinerG.get();
}

void TGessoPolluteObj::pollute()
{
	unk150 = 2;
	unk168 = 0;
	mMActor->setBck("gero_run1");
	mMActor->setBck("gero_run_loop1");
	offHitFlag(HIT_FLAG_NO_COLLISION);
	f32 scale  = unk164;
	mScaling.z = scale;
	mScaling.y = scale;
	mScaling.x = scale;
}

void TGessoPolluteObj::rebirth()
{
	if (unk158 == 0) {
		gpMarioParticleManager->emit(0xBC, &mPosition, 0, nullptr);
		gpMarioParticleManager->emit(0xBD, &mPosition, 0, nullptr);
		if (gpMSound->gateCheck(0x2867))
			MSoundSESystem::MSoundSE::startSoundActor(0x2867, &mPosition, 0,
			                                          nullptr, 0, 4);
	}

	unk158 += 1;

	if (unk158 == 10) {
		mVelocity.y = -15.0f;
		gpPollution->stamp(1, mPosition.x, mPosition.y, mPosition.z,
		                   TGesso::mPollRange * 32.0f * 0.5f);

		((TGesso*)unk160)
		    ->getManager()
		    ->requestPolluteModel(mPosition, mScaling);
	}

	if (unk158 > 20) {
		unk150 = 0;
		unk158 = 0;
		onHitFlag(HIT_FLAG_NO_COLLISION);
	}

	if (mPosition.y < mGroundHeight - 30.0f) {
		mVelocity.y = 0.0f;
		onHitFlag(HIT_FLAG_NO_COLLISION);
	}
}

void TGessoPolluteObj::set()
{
	TEnemyAttachment::set();

	if (unk160->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		mPosition.x = unk160->mPosition.x;
		mPosition.y = unk160->mPosition.y + 200.0f;
		mPosition.z = unk160->mPosition.z;
	} else {
		MtxPtr mtx = unk16C->getModel()->getAnmMtx(TGesso::mMouthJntIndex);

		JGeometry::TVec3<f32> local_54 = getVelocity();

		// TODO: awful things happening with the stack frame here
		JGeometry::TVec3<f32> local_C = getVelocity();
		if (JGeometry::TVec3<f32>(local_C).x != 0.0f
		    || JGeometry::TVec3<f32>(local_C).z != 0.0f)
			MsVECNormalize(&local_54, &local_54);

		mPosition.x = local_54.x * 100.0f + mtx[0][3];
		mPosition.y = mtx[1][3];
		mPosition.z = local_54.z * 100.0f + mtx[2][3];
	}

	mMActor->setBck("gero_run_loop1");
}

void TGessoPolluteObj::calcRootMatrix()
{
	TEnemyAttachment::calcRootMatrix();
	if (unk168 != 0)
		return;

	// TODO: I think the stack frame size is explicitly telling us that
	// there IS an inline for this pattern after all...
	if (gpMSound->gateCheck(0x2012))
		MSoundSESystem::MSoundSE::startSoundActor(0x2012, &mPosition, 0,
		                                          nullptr, 0, 4);

	gpMarioParticleManager->emitAndBindToPosPtr(0x165, &mPosition, 1, this);
}

void TGessoPolluteObj::sendMessage()
{
	for (int i = 0; i < mColCount; ++i) {
		if (mCollisions[i]->isActorType(0x80000001)) {
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
			kill();
		}
	}
}

DEFINE_NERVE(TNerveGessoStay, TLiveActor)
{
	TGesso* self = (TGesso*)spine->getBody();
	if (spine->getTime() == 0)
		self->setWaitAnm();

	self->rollCheck();
	self->polluteBehavior();

	return false;
}

DEFINE_NERVE(TNerveGessoFreeze, TLiveActor)
{
	TGesso* self = (TGesso*)spine->getBody();
	if (spine->getTime() == 0) {
		JGeometry::TVec3<f32> local_88
		    = self->getPosition() - SMS_GetMarioPos();
		MsVECNormalize(&local_88, &local_88);
		local_88.y = 5.0f;
		local_88.x *= 4.0f;
		local_88.z *= 4.0f;
		self->setVelocity(local_88);

		self->mPosition.y += 2.0f;
		self->onLiveFlag(LIVE_FLAG_AIRBORNE);
		self->unk1DC = local_88;

		if (self->checkDropInWater()) {
			spine->pushAfterCurrent(&TNerveGessoFall::theNerve());
			return true;
		}

		self->setBckAnm(10);
	}

	if (self->checkCurAnmEnd(0)) {
		if (self->isBckAnm(10)) {
			self->setBckAnm(9);
		} else if (self->isBckAnm(9)) {
			if (spine->getTime() > self->unk1E8->mSLFreezeWait.get()) {
				u8 tmp = self->unk165;
				if (tmp != 0)
					self->unk165 = 0;

				if (tmp == 0)
					self->setBckAnm(8);
			}
			self->getMActor()->getFrameCtrl(0)->setFrame(0.0f);
		} else if (self->isBckAnm(8)) {
			return true;
		}
	}

	if (!self->isAirborne()) {
		if (self->unk1DC.y < 1.5f) {
			if (self->isBckAnm(9)) {
				gpMarioParticleManager->emitAndBindToPosPtr(
				    0x12F, &self->mPosition, 1, self);
			}
		} else {
			self->unk1DC.y *= 0.3f;
			self->mPosition.y += 2.0f;
			self->onLiveFlag(LIVE_FLAG_AIRBORNE);
			self->setVelocity(self->unk1DC);
		}
	} else if (self->unkF4.unk0 == (THitActor*)gpMarioAddress) {
		self->walkToCurPathNode(0.0f, self->mTurnSpeed, 0.0f);
	}

	return false;
}

DEFINE_NERVE(TNerveGessoPunch, TLiveActor)
{
	TGesso* self = (TGesso*)spine->getBody();

	if (spine->getTime() == 0)
		self->setBckAnm(11);

	if (self->checkCurAnmEnd(0))
		return true;

	return false;
}

// The lil guys spew balls of goop every once in a while. This is that.
// It's not actually polluting anything, simply spitting out a thing that
// disappears upon impact.
DEFINE_NERVE(TNerveGessoPollute, TLiveActor)
{
	TGesso* self = (TGesso*)spine->getBody();

	if (spine->getTime() < 2)
		self->setPolluteGoal();

	if (self->isBckAnm(5) || self->isBckAnm(17)) {
		if (self->getMActor()->getFrameCtrl(0)->checkPass(50.0f))
			self->pollute();
		if (self->checkCurAnmEnd(0))
			return true;
	} else {
		if (self->isWandering() || self->mIsRightSideUp)
			self->setBckAnm(5);
		else
			self->setBckAnm(17);

		gpMarioParticleManager->emitAndBindToMtxPtr(
		    0xBB,
		    self->getMActor()->getModel()->getAnmMtx(TGesso::mMouthJntIndex), 0,
		    nullptr);
		self->mPolluteObj->appear();
	}

	return false;
}

DEFINE_NERVE(TNerveGessoFall, TLiveActor)
{
	TGesso* self = (TGesso*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setGoalPath(TPathNode(SMS_GetMarioPos()));

		if (self->isWandering()) {
			JGeometry::TVec3<f32> local_80
			    = self->getPosition() - SMS_GetMarioPos();
			local_80.y = 0.0f;
			if (local_80.x == 0.0f && local_80.z == 0.0f)
				local_80.z = 1.0f;
			MsVECNormalize(&local_80, &local_80);
			local_80.x *= 4.0f;
			local_80.z *= 4.0f;
			self->setVelocity(local_80);
			self->onLiveFlag(LIVE_FLAG_AIRBORNE);
			self->setBckAnm(6);
		} else {
			if (self->mIsRightSideUp) {
				self->setBckAnm(6);
			} else {
				self->setBckAnm(18);
				self->mPosition.y -= TGesso::mThroughHoseiDistY;
			}

			self->mState = TGesso::STATE_FALLING;
		}
		self->onLiveFlag(LIVE_FLAG_UNK8);
	} else if (self->isWandering()) {
		if (self->checkCurAnmEnd(0)) {
			if (self->isBckAnm(6)) {
				JGeometry::TVec3<f32> local_8C
				    = self->getPosition() - SMS_GetMarioPos();
				local_8C.y = 0.0f;
				if (local_8C.x == 0.0f && local_8C.z == 0.0f)
					local_8C.z = 1.0f;
				MsVECNormalize(&local_8C, &local_8C);
				local_8C.x *= 6.0f;
				local_8C.y = 5.0f;
				local_8C.z *= 6.0f;
				self->setVelocity(local_8C);
				self->onLiveFlag(LIVE_FLAG_AIRBORNE);
				self->setBckAnm(4);
			} else if (!self->isAirborne()) {
				if (self->getGroundPlane()->isWaterSurface()) {
					self->generateEffectColumWater();
					spine->pushAfterCurrent(&TNerveSmallEnemyDie::theNerve());

					return true;
				} else if (self->isBckAnm(4)) {
					self->setBckAnm(7);
				} else if (self->isBckAnm(7)) {
					spine->pushAfterCurrent(
					    &TNerveWalkerGraphWander::theNerve());
					return true;
				}
			}
		}
	} else {
		if (self->isBckAnm(6) || self->isBckAnm(18)) {
			self->unk1D0 *= 0.8f;
			if (self->checkCurAnmEnd(0)) {
				self->mState = 4;
				self->setBckAnm(4);
			}
		} else if (self->isBckAnm(4)) {
			self->unk1D0 *= 0.8f;
			if (!self->isAirborne()) {
				if (self->getGroundPlane()->isWaterSurface()) {
					self->generateEffectColumWater();
					spine->pushAfterCurrent(&TNerveSmallEnemyDie::theNerve());
					return true;
				}

				if (self->checkCurAnmEnd(0))
					self->setBckAnm(7);
			}
		} else if (self->isBckAnm(7)) {
			self->offLiveFlag(LIVE_FLAG_UNK8);
			self->mStayYaw *= 0.8f;

			self->mRotation.x *= 0.8f;
			self->mRotation.y *= 0.8f;
			self->mRotation.z *= 0.8f;

			if (self->checkCurAnmEnd(0)) {
				self->fallEnd();
				spine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
				return true;
			}
		}
	}

	JGeometry::TVec3<f32> velocity = self->getVelocity();
	velocity.x *= 0.99f;
	velocity.z *= 0.99f;
	self->setVelocity(velocity);

	return false;
}

DEFINE_NERVE(TNerveGessoRolling, TLiveActor)
{
	TGesso* self = (TGesso*)spine->getBody();
	if (spine->getTime() < 2) {
		if (self->mIsRightSideUp)
			self->setBckAnm(16);
		else
			self->setBckAnm(19);
	} else {
		if (self->checkCurAnmEnd(0)) {
			self->rollEnd();
			return true;
		}
	}
	return false;
}

DEFINE_NERVE(TNerveGessoFindMario, TLiveActor)
{
	TGesso* self = (TGesso*)spine->getBody();

	if (spine->getTime() == 0)
		self->setBckAnm(13);

	if (self->checkCurAnmEnd(0))
		return true;

	return false;
}

DEFINE_NERVE(TNerveGessoLand, TLiveActor)
{
	TGesso* self = (TGesso*)spine->getBody();

	if (spine->getTime() == 0)
		self->setBckAnm(14);

	if (self->checkCurAnmEnd(0)) {
		self->mNeedsLanding = false;
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveGessoTurn, TLiveActor)
{
	TGesso* self = (TGesso*)spine->getBody();

	if (spine->getTime() < 2) {
		self->turnIn();
	} else if (self->turning()) {
		self->turnOut();
		return true;
	}

	return false;
}

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

	void* res = JKRGetResource("/scene/rikuGesso/stamp_gero_model1.bmd");
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
	f32 fVar1;
	f32 fVar2;
	if (unk38 == nullptr) {
		fVar1 = 300.0f;
		fVar2 = gpConductor->unk84.mEnemyFarClip.get();
	} else {
		fVar2 = unk38->mSLFarClip.get();
		fVar1 = unk38->mSLClipRadius.get();
	}

	SetViewFrustumClipCheckPerspective(
	    gpCamera->getFovy(), gpCamera->getAspect(), param_1->unkE8, fVar2);

	for (int i = 0; i < mObjNum; ++i) {
		TGesso* gesso = (TGesso*)unk18[i];
		if (ViewFrustumClipCheck(param_1, &gesso->mPosition, fVar1)) {
			gesso->offLiveFlag(0x4);
		} else {
			gesso->onLiveFlag(0x4);
		}

		if (!(gesso->unk1A8->unk150 == 0 ? true : false)) {
			if (ViewFrustumClipCheck(param_1, &gesso->unk1A8->mPosition, fVar1))
				gesso->unk1A8->offLiveFlag(0x4);
			else
				gesso->unk1A8->onLiveFlag(0x4);
		}
	}
}

void TGessoManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	gpCurGesso = nullptr;
	TEnemyManager::perform(param_1, param_2);
	for (int i = 0; i < getActiveObjNum(); ++i)
		getObj(i)->unk1A8->perform(param_1, param_2);

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

void TGessoManager::requestPolluteModel(JGeometry::TVec3<float>&,
                                        JGeometry::TVec3<float>&)
{
}

static int GessoBodyCallback(J3DNode* param_1, int param_2)
{
	if (param_2 != 0 || gpCurGesso == nullptr
	    || !(gpCurGesso->unk19C == 1 ? false : true))
		return true;

	MtxPtr mA
	    = gpCurGesso->getModel()->getAnmMtx(((J3DJoint*)param_1)->getJntNo());

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

	f32 fVar2 = gpCurGesso->unk1E8->mSLBodyAngMax.get();
	f32 fVar1 = MsClamp(gpCurGesso->unk1C8.y - 90.0f, -fVar2, fVar2);

	f32 s = JMASin(fVar1);
	f32 c = JMACos(fVar1);

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

	MTXConcat(mA, local_74, mA);
	MTXConcat(mA, local_44, mA);
	MTXConcat(J3DSys::mCurrentMtx, local_74, J3DSys::mCurrentMtx);
	MTXConcat(J3DSys::mCurrentMtx, local_44, J3DSys::mCurrentMtx);
	return 1;
}

u8 TGesso::mBodyJntIndex       = 12;
u8 TGesso::mMouthJntIndex      = 13;
f32 TGesso::mBodyRotSpeed      = 0.5f;
f32 TGesso::mPollRange         = 8.0f;
f32 TGesso::mThroughHoseiDistY = 200.0f;
f32 TGesso::mAngTestY          = 0.15f;

TGesso::TGesso(const char* name)
    : TWalkerEnemy(name)
    , unk194(0)
    , unk198(0)
    , unk19C(0)
    , unk1A0(1)
    , unk1A1(1)
    , unk1A4(0.0f)
    , unk1A8(nullptr)
    , unk1AC(0)
    , unk1B4(0)
    , unk1C4(1)
    , unk1C8(0.0f, 90.0f, 0.0f)
    , unk1D8(0)
    , unk1D9(0)
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
	unk1A8 = new TGessoPolluteObj;
	unk1A8->loadInit(this, "gero_model1.bmd");
	setBckAnm(21);

	J3DFrameCtrl* ctrl0 = getMActor()->getFrameCtrl(0);
	ctrl0->setFrame((1.0f / mManager->getCapacity()) * unk7C
	                * ctrl0->getEndFrame());
}

void TGesso::reset()
{
	gpCurGesso = this;
	TWalkerEnemy::reset();
	if (unk1AC != 0) {
		unk19C = 1;
		mSpine->initWith(&TNerveWalkerGraphWander::theNerve());
		unk158 = 1.0f;
	} else {
		unk19C = 0;
		mSpine->initWith(&TNerveGessoStay::theNerve());
		unk158 = 0.0f;
	}

	offLiveFlag(0x1000);
	unk1D9   = 0;
	unk1A0   = 1;
	unk1A4   = mPosition.y;
	unk1C8.z = 0.0f;
	setBckAnm(21);

	J3DFrameCtrl* ctrl0 = getMActor()->getFrameCtrl(0);
	ctrl0->setFrame((1.0f / mManager->getCapacity()) * unk7C
	                * ctrl0->getEndFrame());
}

void TGesso::load(JSUMemoryInputStream& stream)
{
	TWalkerEnemy::load(stream);
	unk1C4 = 1;
	if (mRotation.y < 0.0f)
		unk1C4 = 0;

	if (mRotation.y == 0.0f || mRotation.y == 180.0f) {
		unk1A1   = 1;
		unk1C8.x = 0.0f;
		if (unk1C4 == 0)
			unk1C8.x = 180.0f;
	} else {
		unk1A1   = 0;
		unk1C8.x = 90.0f;
		if (unk1C4 == 0)
			unk1C8.x = 270.0f;
	}

	reset();
	mRotation.set(0.0f, 0.0f, 0.0f);
}

void TGesso::behaveToWater(THitActor* param_1)
{
	unk165 = 0;
	if (mSpine->getCurrentNerve() == &TNerveGessoFreeze::theNerve())
		unk165 = 1;
	unk160 = 0;
	if (mSpine->getCurrentNerve() != &TNerveSmallEnemyDie::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveGessoFall::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveGessoFreeze::theNerve()
	    && unk19C != 2) {
		if (unk19C == 0) {
			if (unk1A0 != 0) {
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
						local_1c.z = 5.0f;
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
				unkAC = local_1c; // TODO: so is unkAC (sp)eed after all?
				mPosition.y += 2.0f;
				onLiveFlag(0x80);
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

	if (param_1 == 2) {
		unk198 += 1;

		if (checkCurAnmEnd(0)) {
			if (mSpine->getCurrentNerve() != &TNerveGessoPollute::theNerve()
			    && unk198 > unk1E8->mSLPollutionInterval.get()
			    && MsIsInSight(mPosition, getIdk(), SMS_GetMarioPos(),
			                   unk1E8->mSLPollutionLength.get(),
			                   unk1E8->mSLSearchAngleOnObj.get(), 0.0f)) {

				unk198 = 0;
				unk1A8->kill();
				mSpine->pushNerve(&TNerveGessoPollute::theNerve());
			}
		}
	}

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
	if (!(unk15C == 9 ? true : false)
	    || SMS_GetMarioPos().y > mPosition.y + 50.0f)
		return false;

	return true;
}

void TGesso::attackToMario()
{
	if (unk19C != 1) {
		SMS_SendMessageToMario(this, 0xE);
		unk194 = 1;
		return;
	}

	if (mSpine->getCurrentNerve() != &TNerveGessoPunch::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveGessoFreeze::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveSmallEnemyDie::theNerve()) {
		unk194 = 1;
		mSpine->pushNerve(&TNerveWalkerPostAttack::theNerve());
		mSpine->pushNerve(&TNerveGessoPunch::theNerve());
	}

	if (mSpine->getCurrentNerve() == &TNerveGessoPunch::theNerve()) {
		if (mMActor->getFrameCtrl(0)->checkPass(10.0f))
			SMS_SendMessageToMario(this, 0xE);

		return;
	}

	if (!(unk15C == 9 ? true : false))
		SMS_SendMessageToMario(this, 0xE);
}

void TGesso::setBehavior()
{
	if (unk194 > 0)
		unk194 += 1;

	if (unk194 > 200)
		unk194 = 0;

	if (checkLiveFlag2(0x80) && mPosition.y > unkC8 + 250.0f
	    && mSpine->getCurrentNerve() != &TNerveWalkerGenerate::theNerve()) {
		unk1D9 = 1;
	}

	if (!checkLiveFlag2(0x80) && unk1D9 != 0
	    && (mSpine->getCurrentNerve() == &TNerveWalkerGraphWander::theNerve()
	        || mSpine->getCurrentNerve() == &TNerveWalkerAttack::theNerve())) {
		mSpine->pushNerve(&TNerveGessoLand::theNerve());
	}

	if (!checkLiveFlag2(0x80))
		unk1D9 = 0;
}

void TGesso::polluteBehavior() { }

void TGesso::setPolluteGoal()
{
	f32 polluteObjSpeed   = unk1E8->mSLPolluteObjSpeed.get();
	f32 polluteObjGravity = unk1E8->mSLPolluteObjGravity.get();

	if (unk1A0 == 0)
		polluteObjSpeed = 0.0f;

	if (unk1D8 == 0) {
		unk1B8.set(
		    SMS_GetMarioPos().x + FakeRandInterval(-100.0f, 100.0f).get(),
		    SMS_GetMarioPos().y,
		    SMS_GetMarioPos().z + FakeRandInterval(-100.0f, 100.0f).get());

		JGeometry::TVec3<f32> local;
		calcVelocityToJumpToY(local, polluteObjSpeed, polluteObjGravity);
		unk1B8 = local;
	} else {
		unk1B8 = SMS_GetMarioPos();
		unk1B8.x -= mPosition.x;
		unk1B8.y = 0.0f;
		unk1B8.z -= mPosition.z;
		MsVECNormalize(&unk1B8, &unk1B8);
		f32 polluteObjLinerSp = unk1E8->mSLPolluteObjLinerSp.get();
		unk1B8.x *= polluteObjLinerSp;
		unk1B8.y = 0.0f;
		unk1B8.z *= polluteObjLinerSp;
	}
}

void TGesso::pollute()
{
	if (unk19C != 1 && unk1A0 == 0)
		unk1B8.y = -3.0f;

	unk1A8->setUnkAC(unk1B8);
	unk1A8->pollute();

	JGeometry::TVec3<f32> tmp;
	tmp.sub(unk1B8, mPosition);
	tmp.y       = 0.0f;
	mRotation.y = MsGetRotFromZaxisY(tmp);

	MtxPtr mtx = unk1A8->getMActor()->getModel()->getAnmMtx(mMouthJntIndex);

	JGeometry::TVec3<f32> local_2c(0.0f, 0.0f, 100.0f);
	Mtx afStack_5c;
	if (unk19C == 1) {
		MsMtxSetRotRPH(afStack_5c, 0.0f, mRotation.y, 0.0f);
	} else {
		MsMtxSetRotRPH(afStack_5c, 0.0f, mRotation.y + unk1C8.x, 0.0f);
	}
	MTXMultVec(afStack_5c, &local_2c, &local_2c);
	unk1A8->mPosition.x = mtx[0][3] + local_2c.x;
	unk1A8->mPosition.y = mtx[1][3];
	unk1A8->mPosition.z = mtx[2][3] + local_2c.z;
}

void TGesso::isUseBodyCallBack() const { }

void TGesso::setAfterDeadEffect()
{
	unk1A8->kill();
	TSmallEnemy::setAfterDeadEffect();
}

void TGesso::setDeadAnm()
{
	if (unkC4->isWaterSurface())
		onLiveFlag(0x2);
	else if (unkC4->checkFlag2(0x10) || unk1AC != 2)
		setBckAnm(3);
	else
		setBckAnm(15);
}

f32 TGesso::getGravityY() const
{
	if (unk19C != 1) {
		f32 dropGravityY = unk1E8->mSLDropGravityY.get();
		if (unk1A0 == 0)
			return dropGravityY;

		return dropGravityY / 2.0f;
	}
	return unkCC;
}

void TGesso::setWalkAnm() { setBckAnm(12); }

void TGesso::setWaitAnm()
{
	if (unk19C == 1 || unk1A0 != 0) {
		setBckAnm(21);
		unk1C8.z = 0.0f;
	} else {
		setBckAnm(20);
		unk1C8.z = mDamageHeight;
	}
}

void TGesso::bind()
{
	switch (unk19C) {
	case 4:
		TSmallEnemy::bind();
		break;
	case 3:
		mPosition += unkAC;
		unkAC.y -= getGravityY();
		if (unkAC.y < mVelocityMinY)
			unkAC.y = mVelocityMinY;
		if (unk15C == 0x12 ? true : false)
			unk19C = 4;
		else if (mThroughHoseiDistY * 0.5f + mPosition.y < unk1A4)
			unk19C = 4;
		break;
	case 2:
		unk1C8.z    = 0.0f;
		mPosition.y = unk1A4 - unk1C8.z;
	}

	if (unk19C == 1 ? false : true) {
		f32 f8;
		f32 f7;
		f32 f1 = 1.0f;

		if (unk1A0 == 0)
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
		if (unk1C8.y != angle) {
			if (unk1C8.y < angle) {
				unk1C8.y += mBodyRotSpeed;
				if (unk1C8.y > angle)
					unk1C8.y = angle;
			} else {
				unk1C8.y -= mBodyRotSpeed;
				if (unk1C8.y < angle)
					unk1C8.y = angle;
			}
		}
	}
}

void TGesso::kill()
{
	if (!unk1A0)
		return;

	TSmallEnemy::kill();
	unk1A8->kill();
}

void TGesso::calcRootMatrix()
{
	gpCurGesso = this;
	if (unk19C == 1) {
		TSpineEnemy::calcRootMatrix();
		return;
	}

	if (!isEaten()) {
		J3DModel* model = mMActor->getModel();
		MtxPtr mA       = model->getBaseTRMtx();

		MsMtxSetXYZRPH(mA, mPosition.x, mPosition.y + unk1C8.z, mPosition.z,
		               mRotation.x, mRotation.y, mRotation.z);

		f32 s = JMASin(unk1C8.x);
		f32 c = JMACos(unk1C8.x);
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
	if (unk15C == 0xF ? true : false) {
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
		mSpine->pushRaw(&TNerveWalkerGraphWander::theNerve());
		mSpine->pushRaw(&TNerveWalkerEscape::theNerve());
		mSpine->pushRaw(&TNerveSmallEnemyJump::theNerve());
	} else {
		setGoalPathMario();
		mSpine->pushRaw(&TNerveWalkerGraphWander::theNerve());
		mSpine->pushRaw(&TNerveWalkerAttack::theNerve());
		if (unk1B4 == 0) {
			mSpine->pushRaw(&TNerveGessoFindMario::theNerve());
			unk1B4 = 1;
		}
	}
}

void TGesso::rollCheck()
{
	if (unk194 != 0)
		return;

	f32 aware = unk1E8->mSLSearchAwareOnObj.get();
	if (MsIsInSight(mPosition, getIdk(), SMS_GetMarioPos(),
	                unk1E8->mSLSearchLengthOnObj.get(),
	                unk1E8->mSLSearchAngleOnObj.get(), aware)) {
		if ((unk1A0 != 0 && mPosition.y > SMS_GetMarioPos().y + 10.0f)
		    || (unk1A0 == 0 && mPosition.y < SMS_GetMarioPos().y - 10.0f)) {
			onHitFlag(0x1);
			unk19C = 2;
			mSpine->pushNerve(&TNerveGessoRolling::theNerve());
			if (unk1A0 != 0)
				onLiveFlag(0x8);
			unk194 = 0;
		}
	}

	f32 turnLength = unk1E8->mSLTurnLength.get();
	updateSquareToMario();
	if (unk134 < turnLength * turnLength && unk1A0 != 0) {
		if (!MsIsInSight(mPosition, getIdk(), SMS_GetMarioPos(),
		                 unk1E8->mSLSearchLengthOnObj.get(),
		                 unk1E8->mSLSearchAngleOnObj.get(), 0.0f)) {
			mSpine->pushNerve(&TNerveGessoTurn::theNerve());
		}
	}
}

void TGesso::rollEnd() { }

void TGesso::modifyRotate() { }

void TGesso::fallEnd() { }

void TGesso::turnIn() { }

void TGesso::turning() { }

void TGesso::turnOut() { }

void TGesso::checkDropInWater() { }

void TGesso::initAttacker(THitActor* param_1)
{
	mRotation = param_1->getRotation();
	unk184    = 1;
	unk1AC    = 1;
	reset();
}

const char** TGesso::getBasNameTable() const { return rikuGesso_bastable; }

void TSurfGesso::load(JSUMemoryInputStream& stream)
{
	TGesso::load(stream);
	unk1AC = 2;
	reset();
}

void TSurfGesso::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 2)
		offLiveFlag(0x4);
	TGesso::perform(param_1, param_2);
}

void TLandGesso::load(JSUMemoryInputStream& stream)
{
	TGesso::load(stream);
	unk1AC = 1;
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
	offHitFlag(0x1);
	unk150 = 0;
	unkC4  = TMap::getIllegalCheckData();
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
	offHitFlag(0x1);
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
		unkAC.y = -15.0f;
		gpPollution->stamp(1, mPosition.x, mPosition.y, mPosition.z,
		                   TGesso::mPollRange * 32.0f * 0.5f);

		// TODO: one indirection missing
		((TEnemyPolluteModelManager*)unk160->getManager())
		    ->generatePolluteModel(mPosition, mScaling);
	}

	if (unk158 > 20) {
		unk150 = 0;
		unk158 = 0;
		onHitFlag(0x1);
	}

	if (mPosition.y < unkC8 - 30.0f) {
		unkAC.y = 0.0f;
		onHitFlag(0x1);
	}
}

void TGessoPolluteObj::set()
{
	TEnemyAttachment::set();

	if (unk160->checkLiveFlag(0x4)) {
		mPosition.x = unk160->mPosition.x;
		mPosition.y = unk160->mPosition.y + 200.0f;
		mPosition.z = unk160->mPosition.z;
	} else {
		MtxPtr mtx = unk16C->getModel()->getAnmMtx(TGesso::mMouthJntIndex);

		JGeometry::TVec3<f32> local_54 = getUnkAC();

		// TODO: awful thinks happening with the stack frame here
		JGeometry::TVec3<f32> local_C = getUnkAC();
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
			SMS_SendMessageToMario(this, 0xE);
			kill();
		}
	}
}

DEFINE_NERVE(TNerveGessoStay, TLiveActor) { }

DEFINE_NERVE(TNerveGessoFreeze, TLiveActor) { }

DEFINE_NERVE(TNerveGessoPunch, TLiveActor) { }

DEFINE_NERVE(TNerveGessoPollute, TLiveActor) { }

DEFINE_NERVE(TNerveGessoFall, TLiveActor) { }

DEFINE_NERVE(TNerveGessoRolling, TLiveActor) { }

DEFINE_NERVE(TNerveGessoFindMario, TLiveActor) { }

DEFINE_NERVE(TNerveGessoLand, TLiveActor) { }

DEFINE_NERVE(TNerveGessoTurn, TLiveActor) { }

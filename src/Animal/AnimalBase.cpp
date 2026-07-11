#include <Animal/AnimalBase.hpp>
#include <Animal/AnimalManager.hpp>
#include <Animal/AnimalNerve.hpp>
#include <Animal/AnimalSave.hpp>
#include <Camera/cameralib.hpp>
#include <Enemy/Graph.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <Map/Map.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Strategy.hpp>
#include <System/Application.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

JGeometry::TQuat4<f32> SMS_Eular2Quat(const JGeometry::TVec3<f32>& rot)
{
	JGeometry::TQuat4<f32> qz;
	qz.setEulerZ(0.017453294f * rot.z);
	JGeometry::TQuat4<f32> qy;
	qy.setEulerY(0.017453294f * rot.y);
	(void)&qy;
	JGeometry::TQuat4<f32> qx;
	qx.setEulerX(0.017453294f * rot.x);

	JGeometry::TQuat4<f32> result2;
	result2.mul(qx, qz);
	JGeometry::TQuat4<f32> result;
	result.mul(qy, result2);
	return result;
}

TAnimalBase::TAnimalBase(u32 actorType, const char* name)
    : TSpineEnemy(name)
{
	mActorType = actorType;
}

void TAnimalBase::init(TLiveManager* manager)
{
	mManager = manager;
	manager->manageActor(this);

	mMActorKeeper = new TMActorKeeper(manager);
	mMActorKeeper->createMActorFromAllBmd(3);
	mMActor = mMActorKeeper->getMActor(0);

	initHitActor(mActorType, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f);
	mHitFlags |= HIT_FLAG_NO_COLLISION;
	mBodyScale  = 1.0f;
	mMarchSpeed = 0.0f;
	mBodyRadius = 10.0f;
	mWallRadius = 20.0f;
	mLiveFlag |= LIVE_FLAG_UNK10 | LIVE_FLAG_UNK20 | LIVE_FLAG_UNK8;

	mSpine->initWith(&TNerveAnimalGraphWander::theNerve());
	unk124->reset();
	goToShortestNextGraphNode();

	mFrameTimer = (int*)new TAnimalBaseUnk150;

	mMActor->setBckFromIndex(0);
	mMarchSpeed = 0.0f;
	f32 turnSpeed
	    = ((TAnimalManagerBase*)mManager)->mAnimalSave->mSLWalkTurnSpeed.get();
	mTurnSpeed = turnSpeed * SMSGetAnmFrameRate();

	J3DFrameCtrl* frameCtrl = mMActor->getFrameCtrl(0);
	if (frameCtrl) {
		s32 sharedAnmNum = ((TAnimalManagerBase*)manager)
		                       ->mAnimalSave->mSLSharedAnmNum.get();
		f32 phase;
		if (sharedAnmNum == 0)
			phase = MsRandF();
		else
			phase = (1.0f / sharedAnmNum) * (mInstanceIndex % sharedAnmNum);
		frameCtrl->setFrame(phase * frameCtrl->getEnd());
	}

	J3DFrameCtrl* frameCtrl2 = mMActor->getFrameCtrl(3);
	if (frameCtrl2)
		frameCtrl2->setFrame(frameCtrl2->getEnd() * MsRandF());
}

void TAnimalBase::initNoLoad_(TAnimalBase* other)
{
	other->mPosition.x = 1000.0f * (MsRandF() - 0.5f) + mPosition.x;
	other->mPosition.z = 1000.0f * (MsRandF() - 0.5f) + mPosition.z;
	if (mActorType == 0x800001)
		other->mPosition.y = 1000.0f * MsRandF() + mPosition.y;
	else
		other->mPosition.y = mPosition.y - 250.0f * MsRandF();

	other->mScaling = mScaling;

	other->mRotation.x = 0.0f;
	f32 rotY           = 150.0f * (MsRandF() - 0.5f) + mRotation.y;
	other->mRotation.y = MsWrap(rotY, 0.0f, 360.0f);
	other->mRotation.z = 0.0f;

	other->unk3C = unk3C;
	other->unk124->setGraph(unk124->getGraph());
	other->mGroundPlane = TMap::getIllegalCheckData();
	other->init(mManager);

	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(other);
}

void TAnimalBase::load(JSUMemoryInputStream& stream)
{
	TSpineEnemy::load(stream);

	s32 count = stream.readS32() - 1;

	for (int i = 0; i < count; ++i) {
		TAnimalBase* animal = new TAnimalBase(getActorType(), getName());
		initNoLoad_(animal);
	}
}

void TAnimalBase::loadAfter()
{
	TNameRef::loadAfter();
	if (mActorType == 0x800001)
		MSoundSESystem::MSRandPlay::registerTrans(MSD_SE_OBJ_KAMOME_SOLO,
		                                          &mPosition);
}

void TAnimalBase::calcRootMatrix() { }

BOOL TAnimalBase::receiveMessage(THitActor* sender, u32 msg) { return FALSE; }

void TAnimalBase::perform(u32 flags, JDrama::TGraphics* graphics)
{
	if (flags & 1) {
		if (graphics->unk0 & 2) {
			mLinearVelocity.zero();
			control();
			mPosition += mLinearVelocity;
			if (mActorType == 0x800001) {
				SMSGetMSound()->startSeRandPlay(MSD_SE_OBJ_KAMOME_SOLO,
				                                mInstanceIndex);
			}
		}
		flags &= ~1;
	}

	TLiveManager* manager = mManager;
	s32 sharedAnmNum
	    = ((TAnimalManagerBase*)manager)->mAnimalSave->mSLSharedAnmNum.get();

	if (flags & 2) {
		updateAnmSound();
		mMActor->frameUpdate();
		if (!(mLiveFlag & 6))
			calcRootMatrix();
		if ((sharedAnmNum != 0 && mInstanceIndex < sharedAnmNum)
		    || (sharedAnmNum == 0 && !(mLiveFlag & 6)))
			mMActor->calc();
		flags &= ~2;
	}

	if (flags & 4) {
		if (!(mLiveFlag & 6)) {
			Mtx save;
			Mtx local;
			Mtx world;
			MTXCopy(j3dSys.mViewMtx, save);
			CLBCalcRotateZXYTranslateMatrix(local, mRotation, mPosition);
			MTXConcat(save, local, world);
			MTXCopy(world, j3dSys.mViewMtx);

			if (sharedAnmNum == 0 || mInstanceIndex < sharedAnmNum) {
				mMActor->viewCalc();
			} else {
				J3DModel* shared
				    = manager->getObj(mInstanceIndex % sharedAnmNum)
				          ->getModel();
				J3DModel* model    = getModel();
				J3DModelData* data = model->getModelData();
				int count          = data->getDrawMtxNum();

				model->swapAllMtx();

				Mtx* srcArrays[2];
				srcArrays[0] = shared->mNodeMatrices;
				srcArrays[1] = shared->unk5C;

				for (u16 i = 0; i < count; ++i) {
					MTXConcat(world,
					          srcArrays[data->getDrawMtxFlag(i)]
					                   [data->getDrawMtxIndex(i)],
					          model->getDrawMtx(i));
				}

				model->calcNrmMtx();
				DCStoreRange(model->getDrawMtxPtr(), count * sizeof(Mtx));
				DCStoreRange(model->getNrmMtxPtr(), count * sizeof(Mtx33));
				model->prepareShapePackets();
			}

			MTXCopy(save, j3dSys.mViewMtx);
		}
		flags &= ~4;
	}

	TSpineEnemy::perform(flags, graphics);
}

void TAnimalBase::resetRandomCurPathNode()
{
	TPathNode curNode = unkF4;
	if (curNode.unk0 != nullptr)
		return;

	JGeometry::TVec3<f32> pos = curNode.getPoint();
	pos.x += 1000.0f * (MsRandF() - 0.5f);
	pos.z += 1000.0f * (MsRandF() - 0.5f);

	if (mActorType == 0x800001) {
		pos.y += 1000.0f * (pos.y <= 1000.0f ? MsRandF() : (MsRandF() - 0.5f));
	} else {
		pos.y -= 250.0f * MsRandF();
	}

	setGoalPath(pos);
}

void TAnimalBase::getRotationFlyToDir(JGeometry::TVec3<f32>* current_rot,
                                      const JGeometry::TVec3<f32>& target_diff,
                                      f32 speedX, f32 speedY)
{
	JGeometry::TVec3<f32> rot = MsGetRotFromZaxis(target_diff);
	rot.y                     = MsWrap<f32>(rot.y, 0.0f, 360.0f);

	f32 clampedDelta = JGeometry::TUtil<f32>::clamp(
	    MsAngleDiff(rot.y, current_rot->y), -speedY, speedY);

	current_rot->y += clampedDelta;
	current_rot->y = MsWrap<f32>(current_rot->y, 0.0f, 360.0f);

	f32 targetRoll = MsClamp<f32>(30.0f * -clampedDelta, -45.0f, 45.0f);
	CLBChaseGeneralConstantSpecifySpeed<f32>(&current_rot->z, targetRoll,
	                                         0.1f * speedX);

	rot.x          = MsWrap<f32>(rot.x, -180.0f, 180.0f);
	current_rot->x = MsWrap<f32>(current_rot->x, -180.0f, 180.0f);
	CLBChaseGeneralConstantSpecifySpeed<f32>(&current_rot->x, rot.x,
	                                         0.1f * speedX);
}

// UNUSED (Size: 0x4a0 in MAP)
void TAnimalBase::flyToCurPathNode(f32 a1, f32 a2) { }

void TAnimalBase::execWalk(bool moving)
{
	TAnimalSaveIndividual* save = ((TAnimalManagerBase*)mManager)->mAnimalSave;

	if (moving) {
		f32 speed = save->mSLMaxMarchSpeed.get() * SMSGetAnmFrameRate();
		f32 accel = save->mSLMarchAccel.get() * SMSGetAnmFrameRate()
		            * SMSGetAnmFrameRate();
		CLBChaseGeneralConstantSpecifySpeed<f32>(&mMarchSpeed, speed, accel);
	} else {
		f32 decel = save->mSLMarchDecrease.get() * SMSGetAnmFrameRate()
		            * SMSGetAnmFrameRate();
		CLBChaseGeneralConstantSpecifySpeed<f32>(&mMarchSpeed, 0.0f, decel);
	}

	if (mMarchSpeed < 0.001f) {
		f32 waitSpeed = save->mSLWaitTurnSpeed.get();
		mTurnSpeed    = waitSpeed * SMSGetAnmFrameRate();
	} else {
		f32 walkSpeed = save->mSLWalkTurnSpeed.get();
		mTurnSpeed    = walkSpeed * SMSGetAnmFrameRate();
	}

	f32 turnSpeed  = mTurnSpeed;
	f32 marchSpeed = mMarchSpeed;

	JGeometry::TVec3<f32> diff = unkF4.getPoint();
	diff -= mPosition;

	f32 dist = diff.length();

	if (dist < 100.0f)
		return;

	if (dist <= 2.0f * calcMinimumTurnRadius(marchSpeed, turnSpeed))
		turnSpeed = calcTurnSpeedToReach(marchSpeed, 0.5f * dist);

	getRotationFlyToDir(&mRotation, diff, marchSpeed, turnSpeed);

	JGeometry::TQuat4<f32> quat = SMS_Eular2Quat(mRotation);
	JGeometry::TVec3<f32> tmp;
	// TODO: quaternions are still wrong
	quat.rotate(JGeometry::TVec3<f32>(0.0f, 0.0f, marchSpeed), tmp);
	mLinearVelocity = tmp;
}

// UNUSED (Size: 0x5c in MAP)
void TAnimalBase::animalWalkIn() { }

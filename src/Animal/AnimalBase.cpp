#include <Animal/AnimalBase.hpp>
#include <Animal/AnimalManager.hpp>
#include <Animal/AnimalNerve.hpp>
#include <Animal/AnimalSave.hpp>
#include <Camera/cameralib.hpp>
#include <Enemy/Graph.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <System/Application.hpp>
#include <MarioUtil/RandomUtil.hpp>

JGeometry::TQuat4<f32> SMS_Eular2Quat(const JGeometry::TVec3<f32>& rot)
{
	// TODO: 61.8% - logic confirmed correct (result = qy * (qx * qz), axis
	// quaternions from half-angle sin/cos). Remaining diff is MWCC FP
	// scheduling: the original emits cosf before sinf per axis and keeps qy in
	// registers rather than spilling it. Not yet reproducible from source.
	f32 rad;
	rad = 0.5f * (0.017453294f * rot.z);
	JGeometry::TQuat4<f32> qz(0.0f, 0.0f, sinf(rad), cosf(rad));
	rad = 0.5f * (0.017453294f * rot.y);
	JGeometry::TQuat4<f32> qy(0.0f, sinf(rad), 0.0f, cosf(rad));
	rad = 0.5f * (0.017453294f * rot.x);
	JGeometry::TQuat4<f32> qx(sinf(rad), 0.0f, 0.0f, cosf(rad));

	JGeometry::TQuat4<f32> result;
	result.mul(qx, qz);
	result.mul(qy, result);
	return result;
}

TAnimalBase::TAnimalBase(u32 actorType, const char* name)
    : TSpineEnemy(name)
{
	mActorType = actorType;
}

BOOL TAnimalBase::receiveMessage(THitActor* sender, u32 msg) { return FALSE; }

void TAnimalBase::calcRootMatrix() { }

void TAnimalBase::perform(u32 flags, JDrama::TGraphics* graphics)
{
	// TODO: 79.6% - full logic reversed and correct (move: control() +
	// integrate velocity + kamome SE; update:
	// updateAnmSound/frameUpdate/calcRootMatrix + MActor calc; draw: build
	// world matrix, then either MActor viewCalc or the shared-animation path
	// that borrows a flock member's animated joint matrices into this model's
	// draw-matrix buffers). Remaining diff is MWCC register allocation (the
	// original keeps model/data pointers in callee- saved GPRs) and
	// addressing-mode scheduling in the shared-anim loop.
	if (flags & 1) {
		if (graphics->unk0 & 2) {
			mLinearVelocity.z = 0.0f;
			mLinearVelocity.y = 0.0f;
			mLinearVelocity.x = 0.0f;
			control();
			mPosition.x += mLinearVelocity.x;
			mPosition.y += mLinearVelocity.y;
			mPosition.z += mLinearVelocity.z;
			if (mActorType == 0x800001) {
				s16 idx = mInstanceIndex;
				if (gpMSound->gateCheck(MSD_SE_OBJ_KAMOME_SOLO))
					MSoundSESystem::MSRandPlay::startSeRandPlay(
					    MSD_SE_OBJ_KAMOME_SOLO, idx);
			}
		}
		flags &= ~1;
	}

	s32 sharedAnmNum
	    = ((TAnimalManagerBase*)mManager)->mAnimalSave->mSLSharedAnmNum.get();

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
			PSMTXCopy(j3dSys.mViewMtx, save);
			CLBCalcRotateZXYTranslateMatrix(local, (const Vec&)mRotation,
			                                (const Vec&)mPosition);
			PSMTXConcat(save, local, world);
			PSMTXCopy(world, j3dSys.mViewMtx);

			if (sharedAnmNum == 0 || mInstanceIndex < sharedAnmNum) {
				mMActor->viewCalc();
			} else {
				J3DModel* shared
				    = mManager->getObj(mInstanceIndex % sharedAnmNum)
				          ->getModel();
				J3DModel* model    = getModel();
				J3DModelData* data = model->mModelData;
				u16 count          = data->getDrawMtxNum();
				u32 view           = model->mCurrentViewNo;

				Mtx* tmp                    = model->mDrawMtxBuf[0][view];
				model->mDrawMtxBuf[0][view] = model->mDrawMtxBuf[1][view];
				model->mDrawMtxBuf[1][view] = tmp;
				Mtx33* tmpN                 = model->mNrmMtxBuf[0][view];
				model->mNrmMtxBuf[0][view]  = model->mNrmMtxBuf[1][view];
				model->mNrmMtxBuf[1][view]  = tmpN;

				Mtx* srcArrays[2];
				srcArrays[0] = shared->mNodeMatrices;
				srcArrays[1] = shared->unk5C;

				for (u16 i = 0; i < count; ++i) {
					PSMTXConcat(world,
					            srcArrays[data->getDrawMtxFlag(i)]
					                     [data->getDrawMtxIndex(i)],
					            model->mDrawMtxBuf[1][view][i]);
				}

				model->calcNrmMtx();
				DCStoreRange(model->mDrawMtxBuf[1][view], count * 0x30);
				DCStoreRange(model->mNrmMtxBuf[1][view], count * 0x24);
				model->prepareShapePackets();
			}

			PSMTXCopy(save, j3dSys.mViewMtx);
		}
		flags &= ~4;
	}

	TSpineEnemy::perform(flags, graphics);
}

void TAnimalBase::init(TLiveManager* manager)
{
	// TODO: 99.8% - all logic matches; original frame is 0x28 larger (MWCC
	// stack padding from an unresolved inline).
	mManager = manager;
	manager->manageActor(this);

	mMActorKeeper = new TMActorKeeper(manager);
	mMActorKeeper->createMActorFromAllBmd(3);
	mMActor = mMActorKeeper->getMActor(0);

	initHitActor(mActorType, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f);
	mHitFlags |= 1;
	mBodyScale  = 1.0f;
	mMarchSpeed = 0.0f;
	mBodyRadius = 10.0f;
	mWallRadius = 20.0f;
	mLiveFlag |= 0x38;

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

void TAnimalBase::execWalk(bool moving)
{
	// TODO: 27% - logic is fully reversed and correct (accel/decel of
	// mMarchSpeed, wait/walk turn-speed select, turn toward unkF4.getPoint(),
	// bank via CLBChase, then rotate the (0,0,marchSpeed) forward vector by
	// SMS_Eular2Quat(mRotation) into mLinearVelocity). Remaining diff is MWCC
	// register allocation / evaluation order: the original spills many more
	// vector temporaries (frame 0x150 vs 0xf0) and evaluates the .get()*rate
	// products right-to-left. Not yet reproduced.
	TAnimalSaveIndividual* save = ((TAnimalManagerBase*)mManager)->mAnimalSave;

	if (moving) {
		CLBChaseGeneralConstantSpecifySpeed<f32>(
		    &mMarchSpeed, save->mSLMaxMarchSpeed.get() * SMSGetAnmFrameRate(),
		    save->mSLMarchAccel.get() * SMSGetAnmFrameRate()
		        * SMSGetAnmFrameRate());
	} else {
		CLBChaseGeneralConstantSpecifySpeed<f32>(&mMarchSpeed, 0.0f,
		                                         save->mSLMarchDecrease.get()
		                                             * SMSGetAnmFrameRate()
		                                             * SMSGetAnmFrameRate());
	}

	if (mMarchSpeed < 0.001f)
		mTurnSpeed = save->mSLWaitTurnSpeed.get() * SMSGetAnmFrameRate();
	else
		mTurnSpeed = save->mSLWalkTurnSpeed.get() * SMSGetAnmFrameRate();

	JGeometry::TVec3<f32> diff = unkF4.getPoint();
	diff.x -= mPosition.x;
	diff.y -= mPosition.y;
	diff.z -= mPosition.z;

	f32 dist       = diff.length();
	f32 marchSpeed = mMarchSpeed;
	f32 turnSpeed  = mTurnSpeed;

	if (dist < 100.0f)
		return;

	if (dist == 2.0f * calcMinimumTurnRadius(marchSpeed, turnSpeed))
		turnSpeed = calcTurnSpeedToReach(marchSpeed, 0.5f * dist);

	JGeometry::TVec3<f32> rot = MsGetRotFromZaxis(diff);
	rot.y                     = MsWrap<f32>(rot.y, 0.0f, 360.0f);

	f32 delta
	    = rot.y - MsWrap<f32>(mRotation.y, rot.y - 180.0f, 180.0f + rot.y);
	delta = MsClamp<f32>(delta, -turnSpeed, turnSpeed);
	mRotation.y += delta;
	mRotation.y = MsWrap<f32>(mRotation.y, 0.0f, 360.0f);

	f32 chaseSpeed = 0.1f * marchSpeed;
	CLBChaseGeneralConstantSpecifySpeed<f32>(
	    &mRotation.z, MsClamp<f32>(30.0f * -delta, -45.0f, 45.0f), chaseSpeed);

	rot.x       = MsWrap<f32>(rot.x, -180.0f, 180.0f);
	mRotation.x = MsWrap<f32>(mRotation.x, -180.0f, 180.0f);
	CLBChaseGeneralConstantSpecifySpeed<f32>(&mRotation.x, rot.x, chaseSpeed);

	JGeometry::TQuat4<f32> quat = SMS_Eular2Quat(mRotation);
	JGeometry::TVec3<f32> vel(0.0f, 0.0f, marchSpeed);
	quat.rotate(vel);
	mLinearVelocity = vel;
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
		f32 r;
		if (pos.y <= 1000.0f)
			r = MsRandF();
		else
			r = MsRandF() - 0.5f;
		pos.y += 1000.0f * r;
	} else {
		pos.y -= 250.0f * MsRandF();
	}

	setGoalPath(TPathNode(pos));
}

void TAnimalBase::loadAfter()
{
	// TODO: 99.7% - stack frame is 0x10 larger in the original (MWCC padding
	// from an unresolved inline); logic is otherwise byte-identical.
	TNameRef::loadAfter();
	if (mActorType == 0x800001)
		MSoundSESystem::MSRandPlay::registerTrans(MSD_SE_OBJ_KAMOME_SOLO,
		                                          &mPosition);
}

void TAnimalBase::load(JSUMemoryInputStream& stream)
{
	TSpineEnemy::load(stream);

	s32 count;
	stream >> count;

	for (int i = 0; i < count - 1; ++i) {
		initNoLoad_(new TAnimalBase(mActorType, getName()));
	}
}

#include <Animal/AnimalBase.hpp>
#include <Animal/AnimalManager.hpp>
#include <Animal/AnimalNerve.hpp>
#include <Animal/AnimalSave.hpp>
#include <Enemy/Graph.hpp>
#include <M3DUtil/MActor.hpp>
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

#include <Enemy/DebuTelesa.hpp>
#include <Camera/Camera.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <System/Particles.hpp>
#include <Strategic/Spine.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <Map/MapCollisionManager.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static const char* DebuTelesa_bastable[] = {
	"/scene/DebuTelesa/bas/debuTelesa_wait.bas",
};

TDebuTelesa::TDebuTelesa(const char* name)
    : TSmallEnemy(name)
{
	onLiveFlag(LIVE_FLAG_UNK10);
}

void TDebuTelesa::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);
	setMActorAndKeeper();
	mSpine->initWith(&TNerveDebuTelesaWait::theNerve());
	initCollision();
	initAnmSound();

	mNullYodareJointIdx
	    = getModel()->getModelData()->getJointName()->getIndex("null_yodare");
	mRightHandJointIdx
	    = getModel()->getModelData()->getJointName()->getIndex("jnt_Rhand");
}

void TDebuTelesa::reset() { }

void TDebuTelesa::initCollision()
{
	initHitActor(0x10000033, 1, 0x80000000, 10.0f, 10.0f, 10.0f, 10.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);
}

void TDebuTelesa::calcRootMatrix()
{
	TSpineEnemy::calcRootMatrix();
	emitEffects();
}

void TDebuTelesa::kill() { TSmallEnemy::kill(); }

// TODO: 99.8%, frame 0x28 vs retail's 0x30. All 44 instructions are exact and
// every r1 displacement in retail is 8 higher, the LR slot included, so the
// residue is one small object at the bottom of the local area.
// Batch 74 located it: it belongs to MSound::startSoundActor, whose retail body
// bound the callee's JAISound* return to a named local
// (`JAISound* sound = nullptr; if (gateCheck(id)) sound = MSoundSE::...;
// return sound;`). That is worth +8 here and closes 38 callers of the wrapper
// project-wide, but ten other callers want the raw spelled-out gate check
// instead and have to be converted first, so the header edit is not in the tree
// yet. The probe table, the closed list and the ten conversions are in
// docs/catalog/frame-gaps.md, "Closure batch 74".
// Measured and rejected here: gpMSound-> instead of SMSGetMSound()-> (+0), the
// spelled-out gateCheck + MSoundSE::startSoundActor pair (+0), a named nullptr
// handle (+0), a TU-local static inline wrapper around the call (+0, inline
// levels are saturated), &getPosition() (frame becomes 0x30 but costs three
// instructions and a fourth callee-saved register).
BOOL TDebuTelesa::receiveMessage(THitActor* sender, u32 message)
{
	switch (message) {
	case HIT_MESSAGE_TRAMPLE:
	case HIT_MESSAGE_HIP_DROP:
	case HIT_MESSAGE_PUNCH:
		return false;

	case HIT_MESSAGE_UNKB:
		SMSGetMSound()->startSoundActor(MSD_SE_EN_DB_TELSA_EATEN, &mPosition, 0,
		                                nullptr, 0, 4);
		break;
	}

	return TSmallEnemy::receiveMessage(sender, message);
}

const char** TDebuTelesa::getBasNameTable() const
{
	return DebuTelesa_bastable;
}

void TDebuTelesa::behaveToWater(THitActor*) { }

void TDebuTelesa::attackToMario() { sendAttackMsgToMario(); }

bool TDebuTelesa::isCollidMove(THitActor*) { return false; }

bool TDebuTelesa::doKeepDistance() { return true; }

void TDebuTelesa::setDeadAnm()
{
	getMActor()->getFrameCtrl(ANM_TYPE_BCK)->init(1);
}

void TDebuTelesa::emitEffects()
{
	if (isTaken())
		return;

	if (mSpine->getLatestNerve() == &TNerveSmallEnemyDie::theNerve())
		return;

	MtxPtr handMtx = getModel()->getAnmMtx(mRightHandJointIdx);
	mRightHandPos.set(handMtx[0][3], handMtx[1][3], handMtx[2][3]);
	SMS_EasyEmitParticle(PARTICLE_MS_POI_ZZZ, &mRightHandPos, this,
	                     JGeometry::TVec3<f32>(1.5f));

	SMS_EasyEmitParticle(PARTICLE_MS_TLS_YODARE_L,
	                     getModel()->getAnmMtx(mNullYodareJointIdx), this,
	                     JGeometry::TVec3<f32>(2.3f));
}

bool TDebuTelesa::isDying() const
{
	return mSpine->getLatestNerve() == &TNerveSmallEnemyDie::theNerve();
}

TDebuTelesaParams::TDebuTelesaParams(const char* path)
    : TSmallEnemyParams(path)
{
	TParams::load(mPrmPath);
}

TDebuTelesaManager::TDebuTelesaManager(const char* name)
    : TSmallEnemyManager(name)
{
}

void TDebuTelesaManager::load(JSUMemoryInputStream& stream)
{
	TDebuTelesaParams* params = new TDebuTelesaParams("/enemy/debuTelesa.prm");

	unk38 = params;

	params->mSLAttackRadius.set(240);
	params->mSLAttackHeight.set(330);
	params->mSLDamageRadius.set(220);
	params->mSLDamageHeight.set(300);
	params->mBodyScaleRange.set(1.0f, 1.0f);

	TSmallEnemyManager::load(stream);
	unk5C = 0;
}

void TDebuTelesaManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "debuTelesa.bmd", 0x10210000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TDebuTelesaManager::clipEnemies(JDrama::TGraphics* graphics)
{
	int e;
	TDebuTelesa* debu;
	int i;

	SetViewFrustumClipCheckPerspective(gpCamera->getFovy(),
	                                   gpCamera->getAspect(), 350.0f,
	                                   getSaveParam()->mSLFarClip.get());
	for (e = getObjNum(), i = 0; i < e; ++i) {
		debu = getObj(i);

		JGeometry::TVec3<f32> pos = debu->mPosition;

		if (debu->checkLiveFlag(LIVE_FLAG_UNK2000)
		    && SMS_IsInOtherFastCube(pos)) {
			debu->onLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		} else if (ViewFrustumClipCheck(graphics, &debu->mPosition, unk3C)) {
			debu->offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		} else {
			debu->onLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		}
	}
}

DEFINE_NERVE(TNerveDebuTelesaWait, TLiveActor)
{
	TDebuTelesa* self = (TDebuTelesa*)spine->getBody();

	if (spine->getTime() == 0) {
		self->getMActor()->setBck("debutelesa_wait");
		self->setCurAnmSound();
	}

	return false;
}

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
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

// dummy: emits @2602 and @2604
static void dummy(Vec* v)
{
	*v = (Vec) { 0.0f, 0.0f, 0.0f };
	*v = (Vec) { 1.0f, 1.0f, 1.0f };
}

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
	initHitActor(ACTOR_TYPE_DEBU_TELESA, 1, HIT_CATEGORY_PLAYER, 10.0f, 10.0f,
	             10.0f, 10.0f);
	offHitFilter(HIT_FILTER_NO_COLLISION);
}

void TDebuTelesa::calcRootMatrix()
{
	TSpineEnemy::calcRootMatrix();
	emitEffects();
}

void TDebuTelesa::kill() { TSmallEnemy::kill(); }

BOOL TDebuTelesa::receiveMessage(THitActor* sender, u32 message)
{
	switch (message) {
	case HIT_MESSAGE_TRAMPLE:
	case HIT_MESSAGE_HIP_DROP:
	case HIT_MESSAGE_PUNCH:
		return false;

	case HIT_MESSAGE_UNKB:
		if (SMSGetMSound()->gateCheck(MSD_SE_EN_DB_TELSA_EATEN))
			MSoundSESystem::MSoundSE::startSoundActor(
			    MSD_SE_EN_DB_TELSA_EATEN, getPosition(), 0, nullptr, 0, 4);
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
		{ "debuTelesa.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
		      | (1 << J3DMLF_TevStageNumShift),
		  0 },
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

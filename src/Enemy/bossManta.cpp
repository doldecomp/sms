#include <Enemy/BossManta.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoaderFlags.hpp>
#include <Strategic/ObjManager.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <System/EmitterViewObj.hpp>
#include <Strategic/Spine.hpp>
#include <Player/ModelWaterManager.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <System/ParamInst.hpp>
#include <System/Resolution.hpp>

TBossManta::TBossManta(const char* name)
    : TSpineEnemy(name)
{
	unk158   = 0.0f;
	unk15C   = 0.0f;
	unk160   = 0.0f;
	unk164   = 0.0f;
	unk168   = 0.0f;
	unk16C   = 0.0f;
	unk170   = 0.0f;
	unk174   = 0.0f;
	unk178   = 1.0f;
	unk17C.x = 0.0f;
	unk17C.y = 0.0f;
	unk17C.z = 0.0f;
	unk188   = 0;
	unk18C   = 0;
	unk190   = 0.0f;
	unk194   = 0.0f;
	unk198   = 0.0f;
	unk19C   = 0;
	unk1A0   = 0;
}

void TBossManta::init(TLiveManager*) { }
void TBossManta::control() { }
void TBossManta::moveObject() { }
int TBossManta::sCenterJointIndex;

void TBossManta::calcRootMatrix()
{
	if (unk154 < 0x31)
		unk154++;

	Mtx m;
	m[0][3] = mPosition.x;
	m[1][3] = mPosition.y;
	m[2][3] = mPosition.z;
	m[0][0] = 1.0f * unk178 - 0.0f * unk174;
	m[1][0] = 0.0f * unk170 - 0.0f * unk178;
	m[2][0] = -(1.0f * unk170 - 0.0f * unk174);
	m[0][1] = 0.0f;
	m[1][1] = 1.0f;
	m[2][1] = 0.0f;
	m[0][2] = unk170;
	m[1][2] = unk174;
	m[2][2] = unk178;

	J3DModel* model    = getModel();
	MtxPtr joint       = model->mNodeMatrices[sCenterJointIndex];
	unk17C.x           = joint[0][3];
	unk17C.y           = mPosition.y;
	unk17C.z           = joint[2][3];
	(Vec&)model->unk14 = (Vec&)mScaling;
	MTXCopy(m, model->unk20);
}
void TBossManta::drawObject(JDrama::TGraphics*) { }
void TBossManta::reset() { }
BOOL TBossManta::receiveMessage(THitActor* sender, u32 message)
{
	if (message == 0xF
	    && gpModelWaterManager
	               ->mParticleFlagSOA[((TWaterHitActor*)sender)->unk68]
	           & 0x40)
		return collidedWithWater();
	return FALSE;
}
void TBossManta::updateAttractor() { }
f32 TBossManta::getPolluteRadius()
{
	if (unk18C < 4) {
		if (unk18C < 0)
			return 0.0f;
		return ((TBossMantaParams*)getSaveParam())->mSLPolluteRadius.get()
		       * mScaling.x;
	}
	if (unk18C < 6)
		return 100.0f;
	return 0.0f;
}
f32 TBossManta::sScale[] = { 20.0f, 10.0f, 5.0f, 2.0f, 1.0f, 1.0f };

void TBossManta::initNthGeneration(int gen)
{
	static const f32 heights[] = { 10.0f, 5.0f, 1.0f, 0.42f, 0.42f, 0.42f };

	unk18C     = gen;
	f32 s      = sScale[unk18C];
	mScaling.z = s;
	mScaling.x = s;
	mScaling.y = heights[unk18C];
	unk19C     = 0;
	unk150     = 0.5f;
	unk154     = 0x31;

	if (unk18C == 0)
		mSpine->initWith(&TNerveMantaAppearDemo::theNerve());
	else
		mSpine->setNext(&TNerveMantaMove::theNerve());
}
BOOL TBossManta::collidedWithWater() { return FALSE; }
void TBossManta::getIntoGraphVec(JGeometry::TVec3<f32>*) { }

void TBossMantaAdditionalCollision::perform(u32, JDrama::TGraphics*) { }
BOOL TBossMantaAdditionalCollision::receiveMessage(THitActor*, u32)
{
	return FALSE;
}

TBossMantaAdditionalCollisionSet::TBossMantaAdditionalCollisionSet() { }
void TBossMantaAdditionalCollisionSet::update(u32, JDrama::TGraphics*) { }
void TBossMantaAdditionalCollisionSet::adapt(TBossManta*) { }

TBossMantaParams::TBossMantaParams(const char* name)
    : TSpineEnemyParams(name)
    , PARAM_INIT(mSLPolluteRadius, 100.0f)
    , PARAM_INIT(mSLDamageEffectNum, 3)
    , PARAM_INIT(mSLAppearDemoInitialZ, 7600.0f)
    , PARAM_INIT(mSLAppearDemoWalkSpeed, 3.0f)
    , PARAM_INIT(mSLMantaRed, 0xC4)
    , PARAM_INIT(mSLMantaGreen, 0x80)
    , PARAM_INIT(mSLMantaBlue, 0x5F)
    , PARAM_INIT(mSLMantaAlpha, 0x80)
    , PARAM_INIT(mSLAngryMantaRed, 0xD2)
    , PARAM_INIT(mSLAngryMantaGreen, 0x1E)
    , PARAM_INIT(mSLAngryMantaBlue, 0x5A)
    , PARAM_INIT(mSLAngryMantaAlpha, 0x80)
    , PARAM_INIT(mSLAttractorPower, 622.0f)
    , PARAM_INIT(mSLPusherPower, 23559.0f)
    , PARAM_INIT(mSLEscapeLookPoint, 1000.0f)
    , PARAM_INIT(mSLEscapeLookedPoint, 1000.0f)
    , PARAM_INIT(mSLEscapeRegion, 500.0f)
{
	TParams::load(mPrmPath);
}

TBossMantaManager::TBossMantaManager(const char* name)
    : TEnemyManager(name)
{
	unk7C = 0;
	unk80 = 0;
	unk88 = this;
	unk8C = 0;
	unk90 = this;
	unk94 = 0;
	unk7C = new (0x20) u8[SMSGetGameRenderWidth() * SMSGetGameRenderHeight()];
	unk74 = new JGeometry::TVec3<f32>[7];
	unk78 = new JGeometry::TVec3<f32>[2];
}

void TBossMantaManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TBossMantaParams("/enemy/bossmanta.prm");
	TEnemyManager::load(stream);
}
void TBossMantaManager::loadAfter() { }
void TBossMantaManager::perform(u32, JDrama::TGraphics*) { }
void TBossMantaManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "manta.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
		      | (1 << J3DMLF_TevStageNumShift),
		  0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}
void TBossMantaManager::spawn(int gen, const JGeometry::TVec3<f32>& pos)
{
	// TODO: 1.5% - logic correct (ring spawn of counts[gen] manta), but the
	// Y-rotation matrix-mult codegen shape needs work.
	static const int counts[] = { 1, 2, 3, 4, 4 };

	int count     = counts[gen];
	f32 baseAngle = 3.1415927f * (2.0f * MsRandF());
	for (int i = 0; i < count; ++i) {
		TBossManta* manta = (TBossManta*)getDeadEnemy();
		if (manta != nullptr) {
			JGeometry::TVec3<f32> dir(0.0f, 0.0f, 1.0f);
			f32 angle = baseAngle + (2.0f * (f32)i * 3.1415927f) / (f32)count;
			Mtx m;
			MTXRotRad(m, 'y', angle);
			MTXMultVec(m, &dir, &dir);
			manta->unk170    = dir.x;
			manta->unk174    = dir.y;
			manta->unk178    = dir.z;
			manta->mPosition = pos;
			manta->initNthGeneration(gen);
		}
	}
}
void TBossMantaManager::createEnemies(int) { }
void TBossMantaManager::setupEfbAlpha(JDrama::TGraphics*) { }
void TBossMantaManager::updateMantaEscape() { }
void TBossMantaManager::drawMantaShadow(JDrama::TGraphics*) { }
TSpineEnemy* TBossMantaManager::createEnemyInstance()
{
	return new TBossManta("マンタ");
}
void TBossMantaManager::TMantaMessageState::update() { }
void TBossMantaManager::TMantaBattleState::update() { }

DEFINE_NERVE(TNerveMantaSpawn, TLiveActor)
{
	TBossManta* self = (TBossManta*)spine->getBody();

	if (spine->getTime() < 5) {
		self->mScaling.x *= 1.01f;
		self->mScaling.y *= 1.01f;
		self->mScaling.z *= 1.01f;
	} else {
		self->mScaling.x *= 0.9f;
		self->mScaling.y *= 0.9f;
		self->mScaling.z *= 0.9f;
	}

	if (spine->getTime() == 0) {
		static const int particles[] = { 0xFC, 0xFB, 0xFA, 0xF9 };
		gpMarioParticleManager->emitAndBindToPosPtr(particles[self->unk18C],
		                                            &self->unk17C, 0, self);
		static const u32 sounds[] = { 0x8994, 0x8995, 0x8996, 0x8997 };
		u32 snd                   = sounds[self->unk18C];
		if (gpMSound->gateCheck(snd))
			MSoundSESystem::MSoundSE::startSoundActor(
			    snd, (const Vec*)&self->mPosition, 0, nullptr, 0, 4);
		((TBossMantaManager*)self->getManager())
		    ->spawn(self->unk18C + 1, self->mPosition);
	}

	if (spine->getTime() == 0x1E)
		return TRUE;
	return FALSE;
}
DEFINE_NERVE(TNerveMantaMove, TLiveActor) { return FALSE; }
DEFINE_NERVE(TNerveMantaAppearDemo, TLiveActor) { return FALSE; }
DEFINE_NERVE(TNerveMantaDeath, TLiveActor)
{
	TBossManta* self = (TBossManta*)spine->getBody();

	if (spine->getTime() == 0) {
		self->getMActor()->setBckFromIndex(0);
		self->getMActor()->setFrameRate(SMSGetAnmFrameRate(), 0);
		MActorAnmBck* bck = self->getMActor()->getAnmBck();
		if (bck != nullptr)
			bck->setMotionBlendRatio(0.0f);
	}

	if (self->checkCurAnmEnd(0)) {
		gpMarioParticleManager->emitAndBindToPosPtr(0xF8, &self->unk17C, 0,
		                                            self);
		if (gpMSound->gateCheck(MSD_SE_BS_MANTA_VANISH))
			MSoundSESystem::MSoundSE::startSoundActor(
			    MSD_SE_BS_MANTA_VANISH, (const Vec*)&self->mPosition, 0,
			    nullptr, 0, 4);
		self->kill();
	}

	return FALSE;
}
DEFINE_NERVE(TNerveMantaHitWater, TLiveActor) { return FALSE; }

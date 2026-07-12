#include <Enemy/BossManta.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoaderFlags.hpp>
#include <Strategic/ObjManager.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <System/ParamInst.hpp>
#include <System/Resolution.hpp>

TBossManta::TBossManta(const char* name)
    : TSpineEnemy(name)
{
	unk158 = 0.0f;
	unk15C = 0.0f;
	unk160 = 0.0f;
	unk164 = 0.0f;
	unk168 = 0.0f;
	unk16C = 0.0f;
	unk170 = 0.0f;
	unk174 = 0.0f;
	unk178 = 1.0f;
	unk17C = 0.0f;
	unk180 = 0.0f;
	unk184 = 0.0f;
	unk188 = 0;
	unk18C = 0;
	unk190 = 0.0f;
	unk194 = 0.0f;
	unk198 = 0.0f;
	unk19C = 0;
	unk1A0 = 0;
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
	unk17C             = joint[0][3];
	unk180             = mPosition.y;
	unk184             = joint[2][3];
	(Vec&)model->unk14 = (Vec&)mScaling;
	MTXCopy(m, model->unk20);
}
void TBossManta::drawObject(JDrama::TGraphics*) { }
void TBossManta::reset() { }
BOOL TBossManta::receiveMessage(THitActor*, u32) { return FALSE; }
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
void TBossManta::initNthGeneration(int) { }
void TBossManta::collidedWithWater() { }
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
void TBossMantaManager::spawn(int, const JGeometry::TVec3<f32>&) { }
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

DEFINE_NERVE(TNerveMantaSpawn, TLiveActor) { return FALSE; }
DEFINE_NERVE(TNerveMantaMove, TLiveActor) { return FALSE; }
DEFINE_NERVE(TNerveMantaAppearDemo, TLiveActor) { return FALSE; }
DEFINE_NERVE(TNerveMantaDeath, TLiveActor) { return FALSE; }
DEFINE_NERVE(TNerveMantaHitWater, TLiveActor) { return FALSE; }

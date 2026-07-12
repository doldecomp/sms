#include <Enemy/BossManta.hpp>

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
void TBossManta::calcRootMatrix() { }
void TBossManta::drawObject(JDrama::TGraphics*) { }
void TBossManta::reset() { }
BOOL TBossManta::receiveMessage(THitActor*, u32) { return FALSE; }
void TBossManta::updateAttractor() { }
f32 TBossManta::getPolluteRadius() { return 0.0f; }
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
{
}

TBossMantaManager::TBossMantaManager(const char* name)
    : TEnemyManager(name)
{
}

void TBossMantaManager::load(JSUMemoryInputStream&) { }
void TBossMantaManager::loadAfter() { }
void TBossMantaManager::perform(u32, JDrama::TGraphics*) { }
void TBossMantaManager::createModelData() { }
void TBossMantaManager::spawn(int, const JGeometry::TVec3<f32>&) { }
void TBossMantaManager::createEnemies(int) { }
void TBossMantaManager::setupEfbAlpha(JDrama::TGraphics*) { }
void TBossMantaManager::updateMantaEscape() { }
void TBossMantaManager::drawMantaShadow(JDrama::TGraphics*) { }
TSpineEnemy* TBossMantaManager::createEnemyInstance() { return nullptr; }
void TBossMantaManager::TMantaMessageState::update() { }
void TBossMantaManager::TMantaBattleState::update() { }

DEFINE_NERVE(TNerveMantaSpawn, TLiveActor) { return FALSE; }
DEFINE_NERVE(TNerveMantaMove, TLiveActor) { return FALSE; }
DEFINE_NERVE(TNerveMantaAppearDemo, TLiveActor) { return FALSE; }
DEFINE_NERVE(TNerveMantaDeath, TLiveActor) { return FALSE; }
DEFINE_NERVE(TNerveMantaHitWater, TLiveActor) { return FALSE; }

#include <Enemy/NameKuri.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DNode.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

f32 TNameKuriManager::mExplosionSpeed    = 1.2f;
u32 TNameKuriManager::mStopMinScaleFrame = 8;

void TNameKuriLauncher::stateLaunch() { }

TNameKuriSaveLoadParams::TNameKuriSaveLoadParams(const char*) { }

TNameKuriManager::TNameKuriManager(const char* name)
    : TSmallEnemyManager(name)
{
}

void TNameKuriManager::load(JSUMemoryInputStream&) { }

void TNameKuriManager::loadAfter() { }

void TNameKuriManager::initSetEnemies() { }

TSmallEnemy* TNameKuriManager::createEnemyInstance() { }

void TNameKuriManager::createModelData() { }

void TNameKuriManager::perform(u32, JDrama::TGraphics*) { }

TNameIndParCallback::TNameIndParCallback(TNameKuri*) { }

void TNameIndParCallback::execute(JPABaseEmitter*, JPABaseParticle*) { }

void TNameIndParCallback::draw(JPABaseEmitter*, JPABaseParticle*) { }

void NameKuriAttackCallback(J3DNode*, int) { }

void NameKuriScaleCallback(J3DNode*, int) { }

TNameKuri::TNameKuri(const char* name)
    : TWalkerEnemy(name)
{
}

void TNameKuri::init(TLiveManager*) { }

void TNameKuri::calcRootMatrix() { }

void TNameKuri::perform(u32, JDrama::TGraphics*) { }

void TNameKuri::moveObject() { }

void TNameKuri::setBehavior() { }

void TNameKuri::behaveToWater(THitActor*) { }

f32 TNameKuri::getGravityY() const { }

void TNameKuri::setBckAnm(int) { }

void TNameKuri::setGenerateAnm() { }

void TNameKuri::setWalkAnm() { }

void TNameKuri::setDeadAnm() { }

void TNameKuri::setAfterDeadEffect() { }

void TNameKuri::setWaitAnm() { }

void TNameKuri::setMeltAnm() { }

void TNameKuri::setMActorAndKeeper() { }

void TNameKuri::reset() { }

void TNameKuri::attackToMario() { }

void TNameKuri::behaveToFindMario() { }

void TNameKuri::endHitWaterJump() { }

void TNameKuri::isAttackJump() const { }

void TNameKuri::isHitWaterJump() const { }

void TNameKuri::canJumpAttack() const { }

bool TNameKuri::isHitValid(u32) { }

bool TNameKuri::isCollidMove(THitActor*) { }

const char** TNameKuri::getBasNameTable() const { }

void TDiffusionNameKuriManager::load(JSUMemoryInputStream&) { }

TSmallEnemy* TDiffusionNameKuriManager::createEnemyInstance() { }

DEFINE_NERVE(TNerveNameKuriLand, TLiveActor) { }

DEFINE_NERVE(TNerveNameKuriJumpAttack, TLiveActor) { }

DEFINE_NERVE(TNerveNameKuriJumpAttackPrepare, TLiveActor) { }

DEFINE_NERVE(TNerveNameKuriExplosion, TLiveActor) { }

DEFINE_NERVE(TNerveNameKuriDiffuse, TLiveActor) { }

DEFINE_NERVE(TNerveNameKuriDrawPollute, TLiveActor) { return true; }

DEFINE_NERVE(TNerveNKFollowMario, TLiveActor) { }

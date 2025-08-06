#include <Enemy/MameGesso.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

f32 TMameGesso::mBaseHoseiTransY = -20.0f;

const char* mameGesso_bastable[] = {
	nullptr,
	nullptr,
	"/scene/mameGesso/bas/mamegeso_land2.bas",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"/scene/mameGesso/bas/mamegeso_run_end2.bas",
	nullptr,
	nullptr,
	"/scene/mameGesso/bas/mamegeso_run_start2.bas",
	"/scene/mameGesso/bas/mamegeso_turn1.bas",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"/scene/mameGesso/bas/mamegeso_water_return1.bas",
};

TMameGessoSaveLoadParams::TMameGessoSaveLoadParams(const char* path) { }

TMameGessoManager::TMameGessoManager(const char* name)
    : TSmallEnemyManager(name)
{
}

void TMameGessoManager::load(JSUMemoryInputStream&) { }

void TMameGessoManager::loadAfter() { }

TSmallEnemy* TMameGessoManager::createEnemyInstance() { }

void TMameGessoManager::initSetEnemies() { }

void TMameGessoManager::perform(u32, JDrama::TGraphics*) { }

TMameGesso::TMameGesso(const char* name)
    : TWalkerEnemy(name)
{
}

void TMameGesso::load(JSUMemoryInputStream&) { }

void TMameGesso::init(TLiveManager*) { }

void TMameGesso::moveObject() { }

void TMameGesso::setMActorAndKeeper() { }

void TMameGesso::attackToMario() { }

void TMameGesso::calcRootMatrix() { }

void TMameGesso::rebirth() { }

void TMameGesso::reset() { }

bool TMameGesso::isHitValid(u32) { }

void TMameGesso::kill() { }

f32 TMameGesso::getGravityY() const { }

void TMameGesso::setWaitAnm() { }

void TMameGesso::setFreezeAnm() { }

void TMameGesso::setDeadAnm() { }

void TMameGesso::behaveToWater(THitActor*) { }

void TMameGesso::behaveToTaken(THitActor*) { }

void TMameGesso::behaveToRelease() { }

bool TMameGesso::isCollidMove(THitActor*) { }

void TMameGesso::calcObjCollision() { }

void TMameGesso::entryObjCollision() { }

BOOL TMameGesso::isInhibitedForceMove() { return true; }

bool TMameGesso::doKeepDistance() { }

void TMameGesso::checkMarioState() { }

const char** TMameGesso::getBasNameTable() const { return mameGesso_bastable; }

DEFINE_NERVE(TNerveMameGessoGraphJumpWander, TLiveActor) { }

DEFINE_NERVE(TNerveMameGessoDamage, TLiveActor) { }

DEFINE_NERVE(TNerveMameGessoJitabata, TLiveActor) { }

DEFINE_NERVE(TNerveMameGessoTrample, TLiveActor) { }

DEFINE_NERVE(TNerveMameGessoPickUp, TLiveActor) { }

DEFINE_NERVE(TNerveMameGessoThrown, TLiveActor) { }

DEFINE_NERVE(TNerveMameGessoObject, TLiveActor) { }

DEFINE_NERVE(TNerveMameGessoWait, TLiveActor) { }

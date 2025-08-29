#include <Enemy/HamuKuri.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

f32 THamuKuri::mCapGravityY         = 0.15f;
f32 THamuKuri::mCapSpeed            = 12.0f;
f32 THamuKuri::mVGenerateGravityY   = 0.15f;
f32 THamuKuri::mLandAnmFrameNum     = 90.0f;
bool THamuKuriManager::mSearchActSw = true;

bool THaneHamuKuri::mBoundFly;
bool TDangoHamuKuri::mAttackSw;

void THamuKuriLauncher::drawObject(JDrama::TGraphics*) { }

void THamuKuriLauncher::stateLaunch() { }

THamuKuriSaveLoadParams::THamuKuriSaveLoadParams(const char* path)
    : TWalkerEnemyParams(path)
{
}

THaneHamuKuriSaveLoadParams::THaneHamuKuriSaveLoadParams(const char* path)
    : THamuKuriSaveLoadParams(path)
{
}

TBossDangoHamuKuriSaveLoadParams::TBossDangoHamuKuriSaveLoadParams(
    const char* path)
    : THamuKuriSaveLoadParams(path)
{
}

TFireHamuKuriSaveLoadParams::TFireHamuKuriSaveLoadParams(const char* path)
    : THamuKuriSaveLoadParams(path)
{
}

THamuKuriManager::THamuKuriManager(const char* name)
    : TSmallEnemyManager(name)
{
}

void THamuKuriManager::load(JSUMemoryInputStream&) { }

void THamuKuriManager::loadAfter() { }

void THamuKuriManager::createModelData() { }

void THamuKuriManager::createAnmData() { }

void THamuKuriManager::perform(u32, JDrama::TGraphics*) { }

void THamuKuriManager::setSearchHamuKuri() { }

void THamuKuriManager::requestSerialKill(THamuKuri*) { }

void THamuKuriManager::checkSerialKill() { }

TSpineEnemy* THamuKuriManager::createEnemyInstance() { }

THaneHamuKuriManager::THaneHamuKuriManager(const char* name)
    : THamuKuriManager(name)
{
}

void THaneHamuKuriManager::load(JSUMemoryInputStream&) { }

TSpineEnemy* THaneHamuKuriManager::createEnemyInstance() { }

void THaneHamuKuriManager::createModelData() { }

void THaneHamuKuriManager::createAnmData() { }

TDoroHaneKuriManager::TDoroHaneKuriManager(const char* name)
    : THaneHamuKuriManager(name)
{
}

void TDoroHaneKuriManager::load(JSUMemoryInputStream&) { }

void TDoroHaneKuriManager::loadAfter() { }

TSpineEnemy* TDoroHaneKuriManager::createEnemyInstance() { }

void TDoroHaneKuriManager::createModelData() { }

TDangoHamuKuriManager::TDangoHamuKuriManager(const char* name)
    : THamuKuriManager(name)
{
}

void TDangoHamuKuriManager::load(JSUMemoryInputStream&) { }

TSpineEnemy* TDangoHamuKuriManager::createEnemyInstance() { }

void TDangoHamuKuriManager::createModelDataArray(const TModelDataLoadEntry*) { }

void TDoroHaneKuriManager::perform(u32, JDrama::TGraphics*) { }

void TDoroHaneKuriManager::createHige() { }

TBossDangoHamuKuriManager::TBossDangoHamuKuriManager(const char* name)
    : TDangoHamuKuriManager(name)
{
}

void TBossDangoHamuKuriManager::load(JSUMemoryInputStream&) { }

TSpineEnemy* TBossDangoHamuKuriManager::createEnemyInstance() { }

static GXColorS10 mFireHamNoseColorDiff  = { 0, 0, 0, 0 };
static GXColorS10 mFireHamOtherColorDiff = { 0, 0, 0, 0 };

static GXColorS10 mFireHamNoseColorStart  = { 0x1F4, 0xC8, 0x78, 0xFF };
static GXColorS10 mFireHamNoseColorEnd    = { 0xFFEC, 0xFFA6, 0xFF74, 0xFF };
static GXColorS10 mFireHamOtherColorStart = { 0x1F4, 0xFFD8, 0xFFBA, 0xFF };
static GXColorS10 mFireHamOtherColorEnd   = { 0x50, 0x5, 0xFFCE, 0xFF };

TFireHamuKuriManager::TFireHamuKuriManager(const char* name)
    : THamuKuriManager(name)
{
}

void TFireHamuKuriManager::load(JSUMemoryInputStream&) { }

TSpineEnemy* TFireHamuKuriManager::createEnemyInstance() { }

void TFireHamuKuriManager::initSetEnemies() { }

void TFireHamuKuriManager::createModelData() { }

void TDoroHige::perform(u32, JDrama::TGraphics*) { }

TDoroHamuKuriManager::TDoroHamuKuriManager(const char* name)
    : THamuKuriManager(name)
{
}

void TDoroHamuKuriManager::load(JSUMemoryInputStream&) { }

void TDoroHamuKuriManager::loadAfter() { }

TSpineEnemy* TDoroHamuKuriManager::createEnemyInstance() { }

void TDoroHamuKuriManager::createModelData() { }

void TDoroHamuKuriManager::perform(u32, JDrama::TGraphics*) { }

void TDoroHamuKuriManager::createHige() { }

THamuKuri::THamuKuri(const char* name)
    : TWalkerEnemy(name)
{
}

void THamuKuri::init(TLiveManager*) { }

void THamuKuri::setMActorAndKeeper() { }

void THamuKuri::reset() { }

void THamuKuri::bind() { }

void THamuKuri::releaseCap() { }

void THamuKuri::behaveToWater(THitActor*) { }

bool THamuKuri::isFindMario(f32) { }

void THamuKuri::setSearchActor(THitActor*) { }

void THamuKuri::isGiveUpSearchActor() { }

void THamuKuri::jumpToSearchActor() { }

void THamuKuri::canGoForSearchActor() { }

void THamuKuri::behaveToFindMario() { }

void THamuKuri::attackToMario() { }

void THamuKuri::moveObject() { }

void THamuKuri::setBehavior() { }

void THamuKuri::changeCapHolder() { }

void THamuKuri::selectCapHolder() { }

void THamuKuri::onHaveCap() { }

void THamuKuri::makeCapFly(TMapObjBase*) { }

f32 THamuKuri::getGravityY() const { }

void THamuKuri::genRandomItem() { }

void THamuKuri::setAfterDeadEffect() { }

void THamuKuri::endHitWaterJump() { }

void THamuKuri::setBckAnm(int) { }

void THamuKuri::setGenerateAnm() { }

void THamuKuri::setWalkAnm() { }

void THamuKuri::setDeadAnm() { }

void THamuKuri::setWaitAnm() { }

void THamuKuri::setRunAnm() { }

void THamuKuri::setRollAnm() { }

void THamuKuri::setCrashAnm() { }

void THamuKuri::setWallDeadEffect() { }

void THamuKuri::setAppearAnm() { }

void THamuKuri::walkBehavior(int, f32) { }

void THamuKuri::initAttacker(THitActor*) { }

const char** THamuKuri::getBasNameTable() const { }

MtxPtr THamuKuri::getTakingMtx() { }

bool THamuKuri::isResignationAttack() { }

bool THamuKuri::isHitValid(u32) { }

bool THamuKuri::isCollidMove(THitActor*) { }

void THamuKuri::isAttackToHam() { }

void THamuKuri::isSerialWallDie() { }

void THamuKuri::forceRoll(JGeometry::TVec3<f32>, bool) { }

THaneHamuKuri::THaneHamuKuri(const char* name)
    : THamuKuri(name)
{
}

void THaneHamuKuri::init(TLiveManager*) { }

void THaneHamuKuri::moveObject() { }

void THaneHamuKuri::reset() { }

void THaneHamuKuri::walkBehavior(int, f32) { }

void THaneHamuKuri::bind() { }

BOOL THaneHamuKuri::isReachedToGoal() const { }

void THaneHamuKuri::attackToMario() { }

void THaneHamuKuri::setMActorAndKeeper() { }

void THaneHamuKuri::setWaitAnm() { }

void THaneHamuKuri::setWalkAnm() { }

void THaneHamuKuri::setRunAnm() { }

void THaneHamuKuri::setRollAnm() { }

void THaneHamuKuri::behaveToWater(THitActor*) { }

void THaneHamuKuri::setCrashAnm() { }

void THaneHamuKuri::setBckAnm(int) { }

void THaneHamuKuri::setDeadAnm() { }

bool THaneHamuKuri::isCollidMove(THitActor*) { }

bool THaneHamuKuri::isHitValid(u32) { }

void THaneHamuKuri::resetFlyParam() { }

const char** THaneHamuKuri::getBasNameTable() const { }

TDoroHaneKuri::TDoroHaneKuri(const char* name)
    : THaneHamuKuri(name)
{
}

void TDoroHaneKuri::init(TLiveManager*) { }

void TDoroHaneKuri::reset() { }

void TDoroHaneKuri::onHaveCap() { }

void TDoroHaneKuri::attackToMario() { }

void TDoroHaneKuri::setMActorAndKeeper() { }

void TDoroHaneKuri::behaveToWater(THitActor*) { }

void TDoroHaneKuri::setBehavior() { }

bool TDoroHaneKuri::isCollidMove(THitActor*) { }

THaneHamuKuri2::THaneHamuKuri2(const char* name)
    : THaneHamuKuri(name)
{
}

void THaneHamuKuri2::reset() { }

void THaneHamuKuri2::sendAttackMsgToMario() { }

void THaneHamuKuri2::walkBehavior(int, f32) { }

BOOL THaneHamuKuri2::isReachedToGoal() const { }

TDangoHamuKuri::TDangoHamuKuri(const char* name)
    : THamuKuri(name)
{
}

void TDangoHamuKuri::init(TLiveManager*) { }

void TDangoHamuKuri::perform(u32, JDrama::TGraphics*) { }

bool TDangoHamuKuri::changeByJuice() { }

void TDangoHamuKuri::moveObject() { }

void TDangoHamuKuri::updateAnmSound() { }

bool TDangoHamuKuri::isCollidMove(THitActor*) { }

void TDangoHamuKuri::attackToMario() { }

MtxPtr TDangoHamuKuri::getTakingMtx() { }

void TDangoHamuKuri::setRunAnm() { }

void TDangoHamuKuri::calcRootMatrix() { }

void TDangoHamuKuri::reset() { }

BOOL TDangoHamuKuri::receiveMessage(THitActor*, u32) { }

bool TDangoHamuKuri::isHitValid(u32) { }

void TDangoHamuKuri::forceKill() { }

void TDangoHamuKuri::nerveInit() { }

void TDangoHamuKuri::behaveToWater(THitActor*) { }

void TDangoHamuKuri::swingBody() { }

TBossDangoHamuKuri::TBossDangoHamuKuri(const char* name)
    : TDangoHamuKuri(name)
{
}

void TBossDangoHamuKuri::init(TLiveManager*) { }

void TBossDangoHamuKuri::perform(u32, JDrama::TGraphics*) { }

void TBossDangoHamuKuri::reset() { }

void TBossDangoHamuKuri::isDead() { }

void TBossDangoHamuKuri::setGenerateAnm() { }

void TBossDangoHamuKuri::moveObject() { }

void TBossDangoHamuKuri::genEventCoin() { }

void TBossDangoHamuKuri::generateBody() { }

void TBossDangoHamuKuri::isNowAttack() { }

void TBossDangoHamuKuri::isNowGenerate() { }

TFireHamuKuri::TFireHamuKuri(const char* name)
    : THamuKuri(name)
{
}

void TFireHamuKuri::init(TLiveManager*) { }

void TFireHamuKuri::behaveToWater(THitActor*) { }

void TFireHamuKuri::reset() { }

void TFireHamuKuri::setMActorAndKeeper() { }

void TFireHamuKuri::moveObject() { }

void TFireHamuKuri::calcRootMatrix() { }

void TFireHamuKuri::walkBehavior(int, f32) { }

bool TFireHamuKuri::isHitValid(u32) { }

void TFireHamuKuri::recoverFire() { }

void TFireHamuKuri::setWalkAnm() { }

void TFireHamuKuri::genFire() { }

void TFireHamuKuri::dieFire() { }

void TFireHamuKuri::sendAttackMsgToMario() { }

void TFireHamuKuri::changeTevColor() { }

TDoroHamuKuri::TDoroHamuKuri(const char* name)
    : THamuKuri(name)
{
}

void TDoroHamuKuri::init(TLiveManager*) { }

void TDoroHamuKuri::reset() { }

void TDoroHamuKuri::kill() { }

void TDoroHamuKuri::setMActorAndKeeper() { }

void TDoroHamuKuri::attackToMario() { }

void TDoroHamuKuri::setBehavior() { }

void TDoroHamuKuri::onHaveCap() { }

bool TDoroHamuKuri::isCollidMove(THitActor*) { }

DEFINE_NERVE(TNerveHamuKuriGoForSearchActor, TLiveActor) { }

DEFINE_NERVE(TNerveHamuKuriBoundFreeze, TLiveActor) { }

DEFINE_NERVE(TNerveHamuKuriWallDie, TLiveActor) { }

DEFINE_NERVE(TNerveHamuKuriLand, TLiveActor) { }

DEFINE_NERVE(TNerveHamuKuriJitabata, TLiveActor) { }

DEFINE_NERVE(TNerveDangoHamuKuriWait, TLiveActor) { }

DEFINE_NERVE(TNerveDangoHamuKuriAttack, TLiveActor) { }

DEFINE_NERVE(TNerveHaneHamuKuriUpWait, TLiveActor) { }

DEFINE_NERVE(TNerveHaneHamuKuriMoveOnGraph, TLiveActor) { }

DEFINE_NERVE(TNerveDoroHamuKuriRobCap, TLiveActor) { }

DEFINE_NERVE(TNerveFireHamuKuriRecover, TLiveActor) { }

DEFINE_NERVE(TNerveDoroHaneRise, TLiveActor) { }

DEFINE_NERVE(TNerveDoroHaneHitWater, TLiveActor) { }

DEFINE_NERVE(TNerveDoroHanePrepareAttack, TLiveActor) { }

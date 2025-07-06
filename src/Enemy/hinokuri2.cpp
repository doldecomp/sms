#include <Enemy/Hinokuri2.hpp>

THino2Params::THino2Params(const char* path)
    : TSpineEnemyParams(path)
{
}

THinokuri2Manager::THinokuri2Manager(const char* name)
    : TEnemyManager(name)
{
}

void THinokuri2Manager::createModelData() { }

void THinokuri2Manager::load(JSUMemoryInputStream&) { }

TSpineEnemy* THinokuri2Manager::createEnemyInstance() { }

THino2MtxCalc::THino2MtxCalc(u16 param_1, u16 param_2, u16 param_3, u16 param_4,
                             u16 param_5, u16 param_6, f32 param_7)
    : TMtxCalcFootInv(param_1, param_2, param_3, param_4, param_5, param_6,
                      param_7)
{
}

void THino2MtxCalc::calc(u16) { }

THino2Hit::THino2Hit(THinokuri2*, int, const char*) { }

void THino2Hit::perform(u32, JDrama::TGraphics*) { }

BOOL THino2Hit::receiveMessage(THitActor*, u32) { }

THino2Mask::THino2Mask(THinokuri2*) { }

void THino2Mask::setMatrix(MtxPtr) { }

void THino2Mask::breakMask() { }

void THino2Mask::startDamageMotion() { }

void THino2Mask::perform(u32, JDrama::TGraphics*) { }

void Hino2HeadCallback(J3DNode*, int) { }

THinokuri2::THinokuri2(const char* name)
    : TSpineEnemy(name)
{
}

void THinokuri2::init(TLiveManager*) { }

void THinokuri2::reset() { }

void THinokuri2::kill() { }

void THinokuri2::emitPolParticle() { }

void THinokuri2::stopPolParticle() { }

void THinokuri2::updatePolTrans() { }

void THinokuri2::resetPolInterval() { }

void THinokuri2::invalidateCollisionAll() { }

void THinokuri2::validateCollisionAll() { }

void THinokuri2::emitWaterParticle() { }

void THinokuri2::shakeCamera(int) { }

void THinokuri2::makeQuake(f32) { }

void THinokuri2::setLevel(int) { }

void THinokuri2::generateEnemy() { }

void THinokuri2::updateAnmSound() { }

void THinokuri2::changeBck(int) { }

void THinokuri2::receiveMessageLv0(THitActor*, u32) { }

void THinokuri2::receiveMessageLv1(THitActor*, u32) { }

void THinokuri2::receiveMessageLv2(THitActor*, u32) { }

BOOL THinokuri2::receiveMessage(THitActor*, u32) { }

void THinokuri2::moveObject() { }

void THinokuri2::perform(u32, JDrama::TGraphics*) { }

DEFINE_NERVE(TNerveHino2Appear, TLiveActor) { }

DEFINE_NERVE(TNerveHino2GraphWander, TLiveActor) { }

DEFINE_NERVE(TNerveHino2Fly, TLiveActor) { }

DEFINE_NERVE(TNerveHino2JumpIn, TLiveActor) { }

DEFINE_NERVE(TNerveHino2Landing, TLiveActor) { }

DEFINE_NERVE(TNerveHino2Turn, TLiveActor) { }

DEFINE_NERVE(TNerveHino2PrePol, TLiveActor) { }

DEFINE_NERVE(TNerveHino2Pollute, TLiveActor) { }

DEFINE_NERVE(TNerveHino2Damage, TLiveActor) { }

DEFINE_NERVE(TNerveHino2Squat, TLiveActor) { }

DEFINE_NERVE(TNerveHino2Burst, TLiveActor) { }

DEFINE_NERVE(TNerveHino2Die, TLiveActor) { }

DEFINE_NERVE(TNerveHino2Stamp, TLiveActor) { }

DEFINE_NERVE(TNerveHino2Freeze, TLiveActor) { }

DEFINE_NERVE(TNerveHino2WaitAnm, TLiveActor) { }

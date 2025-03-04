#include <MSound/MSoundSE.hpp>
#include <MSound/MSRandVol.hpp>

namespace MSoundSESystem {

MSRandPlayVec::MSRandPlayVec() { }
MSRandPlayVec::~MSRandPlayVec() { }

MSRandVol::MSRandVol(u32 param) { }
void MSRandVol::construct(u32 param) { }
u32 MSRandVol::getRandVol(u32 param) { return 0; }
u32 MSRandVol::getRandomVolume(u32 param1, u32 param2) { return 0; }
JSUList<MSRandVol> MSRandVol::smList;

MSRandPlay::MSRandPlay(u32 p1, s32 p2, s32 p3, f32 p4, f32 p5) { }
void MSRandPlay::construct(u32 p1, s32 p2, s32 p3, f32 p4, f32 p5) { }
void MSRandPlay::createRandPlayVec(u32 param1, u16 param2) { }
void MSRandPlay::createRandPlayVecDynamic(u16 param) { }
void MSRandPlay::randPlay(u32 param) { }
void MSRandPlay::registerTrans(u32 param, const Vec* vec) { }
void MSRandPlay::registerTransDynamic(const Vec* vec) { }
void MSRandPlay::startSeRandPlay(u32 param1, u32 param2) { }
JSUList<MSRandPlay> MSRandPlay::smList;

void* SeInfo::smSeSetting = 0;

MSoundSE::MSoundSE() { }
void MSoundSE::construct() { }
bool MSoundSE::checkMonoSound(u32 param, JAIActor* actor) { return false; }
void MSoundSE::checkSoundArea(u32 param, const Vec& vec) { }
u32 MSoundSE::getNewIDByGroundCode(u32 param, JAIActor* actor) { return 0; }
u32 MSoundSE::getNewIDBySurfaceCode(u32 param, JAIActor* actor) { return 0; }
u32 MSoundSE::getRandomID(u32 param) { return 0; }
void MSoundSE::startSoundActor(u32 p1, const Vec* p2, u32 p3, JAISound** p4,
                               u32 p5, u8 p6)
{
}
void MSoundSE::startSoundActorInner(u32 p1, JAISound** p2, JAIActor* p3, u32 p4,
                                    u8 p5)
{
}
void MSoundSE::startSoundActorWithInfo(u32 p1, const Vec* p2, Vec* p3, f32 p4,
                                       u32 p5, u32 p6, JAISound** p7, u32 p8,
                                       u8 p9)
{
}
void MSoundSE::startSoundNpcActor(u32 p1, const Vec* p2, u32 p3, JAISound** p4,
                                  u32 p5, u8 p6)
{
}
void MSoundSE::startSoundSystemSE(u32 p1, u32 p2, JAISound** p3, u32 p4) { }
MSoundSE* MSoundSE::mObj = 0;

} // namespace MSoundSESystem

template <typename T>
MSSetSoundTL<T>::MSSetSoundTL(u32 p1, const char* p2, T* p3, u8 p4, u8 p5,
                              u8 p6, u8 p7, f32 p8, u8 p9, f32 p10, f32 p11,
                              f32 p12, f32 p13, f32 p14, s32 p15, f32 p16,
                              s32 p17, f32 p18, f32 p19, f32 p20, bool p21)
{
}

template <typename T> MSSetSoundTL<T>::~MSSetSoundTL() { }

template <typename T> void MSSetSoundTL<T>::frameLoopDyna() { }

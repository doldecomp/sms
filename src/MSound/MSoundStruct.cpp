#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundStruct.hpp>

MSSetSound* MSSetSound::smSetSound = 0;

void MSSetSound::init() { }

void MSSetSound::startSoundSet(u32 param1, const Vec* param2, u32 param3,
                               f32 param4, u32 param5, u32 param6, u8 param7)
{
}

void MSSetSoundGrp::startSoundSetGrp(u32 param1, const Vec* param2, u32 param3,
                                     f32 param4, u32 param5, u32 param6,
                                     u8 param7)
{
}

template <typename T>
MSSetSoundTL<T>::MSSetSoundTL(u32 param1, const char* param2, T* param3,
                              u8 param4, u8 param5, u8 param6, u8 param7,
                              f32 param8, u8 param9, f32 param10, f32 param11,
                              f32 param12, f32 param13, f32 param14,
                              s32 param15, f32 param16, s32 param17,
                              f32 param18, f32 param19, f32 param20,
                              bool param21)
{
}

template <typename T> MSSetSoundTL<T>::~MSSetSoundTL() { }

template <typename T>
void MSSetSoundTL<T>::startSoundSetDyna(u32 param1, const Vec* param2,
                                        u32 param3, f32 param4, u32 param5,
                                        u32 param6, u8 param7,
                                        MSSetSoundGrp* param8)
{
}

template <typename T> void MSSetSoundTL<T>::frameLoopDyna() { }

template class MSSetSoundTL<MSSetSound>;
template class MSSetSoundTL<MSSetSoundGrp>;

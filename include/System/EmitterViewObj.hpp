#ifndef EMITTER_VIE_OBJ_HPP
#define EMITTER_VIE_OBJ_HPP

#include <dolphin/types.h>
#include <dolphin/mtx.h>
#include <JSystem/JGeometry.hpp>


struct TMarioParticleManager
{
    u32 emitAndBindToMtxPtr(long, MtxPtr, unsigned char, const void*);
    // NOTE: returns some kind of a ptr I think?
    void* emitAndBindToMtx(long, MtxPtr, unsigned char, const void*);

    u32 emit(long, const JGeometry::TVec3<float>*, unsigned char, const void*);
};

extern TMarioParticleManager* gpMarioParticleManager;

#endif

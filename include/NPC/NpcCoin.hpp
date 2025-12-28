#include "MoveBG/MapObjBase.hpp"
#include "dolphin.h"
#include "dolphin/mtx.h"








class TNpcCoin {
    TNpcCoin(int);
    void requestAppearAfterCoin(const Vec&, f32, int);
    void updateCoin();
    TMapObjBase* unk0;
    s32 unk4;
    JGeometry::TVec3<f32> unk8;
    JGeometry::TVec3<f32> unk14;

};
#include <dolphin.h>

#include "Player/MarioMain.hpp"
#include "JSystem/JGeometry.hpp"

bool SMS_IsMarioRoofing(void) {
    bool ret;

    u32 val = gpMarioOriginal->_07C & 0x1FF;

    if (val >= 0x147 and val <= 0x14A) {
        ret = true;
    } else {
        ret = false;
    }

    return ret;
}


bool SMS_IsMarioFencing(void) {
    bool ret;

    u32 val = gpMarioOriginal->_07C & 0x1FF;

    if (val >= 0x168 and val <= 0x16C) {
        ret = true;
    } else {
        ret = false;
    }

    return ret;
}

u32 SMS_GetMarioStatus(THitActor *actor) {
    return actor->_07C;
}

void *SMS_GetMarioRfPlane(void) {
    return gpMarioOriginal->_0DC;
}

void *SMS_GetMarioWlPlane(void) {
    return gpMarioOriginal->_0D8;
}

void *SMS_GetMarioGrPlane(void) {
    return gpMarioOriginal->_0E0;
}

u32 SMS_GetMarioStatus(void) {
    return gpMarioOriginal->_07C;
}

u32 SMS_WindMoveMario(const JGeometry::TVec3<float>& vec) {
    gpMarioOriginal->windMove(vec);
}

u32 SMS_FlowMoveMario(const JGeometry::TVec3<float>& vec) {
    gpMarioOriginal->flowMove(vec);
}

u32 SMS_MarioWarpRequest(const JGeometry::TVec3<float>& vec, float f) {
    gpMarioOriginal->warpRequest(vec, f);
}





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

void SMS_WindMoveMario(const JGeometry::TVec3<float>& vec) {
    gpMarioOriginal->windMove(vec);
}

void SMS_FlowMoveMario(const JGeometry::TVec3<float>& vec) {
    gpMarioOriginal->flowMove(vec);
}

void SMS_MarioWarpRequest(const JGeometry::TVec3<float>& vec, float f) {
    gpMarioOriginal->warpRequest(vec, f);
}

asm void SMS_MarioMoveRequest(const JGeometry::TVec3<float>& vec) {
    nofralloc
    /* 80152A3C 0014F97C  7C 08 02 A6 */  mflr r0;
    /* 80152A40 0014F980  7C 64 1B 78 */  mr r4, r3;
    /* 80152A44 0014F984  90 01 00 04 */  stw r0, 4(r1);
    /* 80152A48 0014F988  94 21 FF F8 */  stwu r1, -8(r1);
    /* 80152A4C 0014F98C  80 6D 98 B8 */  lwz r3, gpMarioOriginal(r13);
    /* 80152A50 0014F990  81 83 00 00 */  lwz r12, 0(r3);
    /* 80152A54 0014F994  81 8C 00 AC */  lwz r12, 0xac(r12);
    /* 80152A58 0014F998  7D 88 03 A6 */  mtlr r12;
    /* 80152A5C 0014F99C  4E 80 00 21 */  blrl;
    /* 80152A60 0014F9A0  80 01 00 0C */  lwz r0, 0xc(r1);
    /* 80152A64 0014F9A4  38 21 00 08 */  addi r1, r1, 8;
    /* 80152A68 0014F9A8  7C 08 03 A6 */  mtlr r0;
    /* 80152A6C 0014F9AC  4E 80 00 20 */  blr;
}







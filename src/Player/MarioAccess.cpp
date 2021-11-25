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
    return ((TMario*) actor)->_07C;
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

#pragma push

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

#pragma pop

u8 SMS_IsMarioDashing() {
    u8 ret;

    if ((gpMarioOriginal->_118 & 0x4000) != 0) {
        ret = true;
    } else {
        ret = false;
    }

    return !!ret;
}

u32 SMS_IsMarioOnYoshi() {
    u32 ret = gpMarioOriginal->onYoshi(); 

    return !!ret;
}

bool SMS_IsMarioOpeningDoor() {
    if (gpMarioOriginal->_07C == 0x1320 || gpMarioOriginal->_07C == 0x1321) {
        return true;
    } else {
        return false;
    }
}


#pragma push
asm u8 SMS_IsMarioOnWire() {
    nofralloc
    /* 80152B00 0014FA40  80 6D 98 B8 */    lwz r3, gpMarioOriginal(r13)
    /* 80152B04 0014FA44  80 03 00 68 */    lwz r0, 0x68(r3)
    /* 80152B08 0014FA48  28 00 00 00 */    cmplwi r0, 0
    /* 80152B0C 0014FA4C  41 82 00 20 */    beq lbl_80152B2C
    /* 80152B10 0014FA50  80 63 00 68 */    lwz r3, 0x68(r3)
    /* 80152B14 0014FA54  80 63 00 4C */    lwz r3, 0x4c(r3)
    /* 80152B18 0014FA58  3C 03 C0 00 */    addis r0, r3, 0xc000
    /* 80152B1C 0014FA5C  28 00 00 98 */    cmplwi r0, 0x98
    /* 80152B20 0014FA60  40 82 00 0C */    bne lbl_80152B2C
    /* 80152B24 0014FA64  38 00 00 01 */    li r0, 1
    /* 80152B28 0014FA68  48 00 00 08 */    b lbl_80152B30
    lbl_80152B2C:
    /* 80152B2C 0014FA6C  38 00 00 00 */    li r0, 0
    lbl_80152B30:
    /* 80152B30 0014FA70  54 00 06 3E */    clrlwi r0, r0, 0x18
    /* 80152B34 0014FA74  7C 60 00 D0 */    neg r3, r0
    /* 80152B38 0014FA78  30 03 FF FF */    addic r0, r3, -1
    /* 80152B3C 0014FA7C  7C 00 19 10 */    subfe r0, r0, r3
    /* 80152B40 0014FA80  54 03 06 3E */    clrlwi r3, r0, 0x18
    /* 80152B44 0014FA84  4E 80 00 20 */    blr 
}
#pragma pop

u8 SMS_IsMarioTouchGround4cm() {
    u8 ret;

    if (gpMarioOriginal->position.y <= 4.0f + gpMarioOriginal->_0EC) {
        ret = 1;
    } else {
        ret = 0;
    }

    if (ret) return 1;
    else return 0;
}


void SMS_ThrowMario(const JGeometry::TVec3<float>& vec, float f) {
    gpMarioOriginal->throwMario(vec, f);
}
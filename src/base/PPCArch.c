#include <dolphin.h>
#include <dolphin/base/PPCArch.h>

asm u32 PPCMfmsr() {
    nofralloc
    mfmsr r3
    blr
}

asm void PPCMtmsr(register u32 newMSR) {
    nofralloc
    mtmsr newMSR
    blr
}

asm u32 PPCMfhid0() {
    nofralloc
    mfspr r3, HID0
    blr
}

asm void PPCMthid0(register u32 newHID0) {
    nofralloc
    mtspr HID0, newHID0
    blr
}

asm u32 PPCMfl2cr() {
    nofralloc
    mfspr r3, L2CR
    blr
}

asm void PPCMtl2cr(register u32 newL2cr) {
    nofralloc
    mtspr L2CR, newL2cr
    blr
}

asm void PPCMtdec(register u32 newDec) {
    nofralloc
    mtdec newDec
    blr
}

asm void PPCSync() {
    nofralloc
    sc
    blr
}

asm void PPCHalt() {
    nofralloc
    sync
loop:
    nop
    li r3, 0
    nop
    b loop
}

asm u32 PPCMfhid2() {
    nofralloc
    mfspr r3, HID2
    blr
}

asm void PPCMthid2(register u32 newhid2) {
    nofralloc
    mtspr HID2, newhid2
    blr
}

asm void PPCMtwpar(register u32 newwpar) {
    nofralloc
    mtspr WPAR, newwpar
    blr
}

void PPCDisableSpeculation() {
    PPCMthid0(PPCMfhid0() | 0x200);
}

asm void PPCSetFpNonIEEEMode() {
    nofralloc
    mtfsb1 29
    blr
}

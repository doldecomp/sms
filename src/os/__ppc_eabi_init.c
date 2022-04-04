#include "Dolphin/__ppc_eabi_init.h"

extern void __OSPSInit();
extern void __OSCacheInit();

// clang-format off
asm void __init_hardware(void) {
    nofralloc
    mfmsr r0
    ori r0, r0, 0x2000
    mtmsr r0
    
    mflr r31
    bl __OSPSInit
    bl __OSCacheInit
    mtlr r31
    blr
}

asm void __flush_cache(register void* address, register unsigned int size) {
    nofralloc
    lis r5,  0xFFFF
    ori r5, r5, 0xFFF1
    and r5, r5, r3
    subf r3, r5, r3
    add r4, r4, r3

loop:    
    dcbst 0, r5
    sync
    icbi 0, r5
    addic r5, r5, 8
    subic. r4, r4, 8    
    bge loop
    isync
    blr
}
// clang-format on
#if 0
extern void __init_cpp(void);

#pragma scheduling off
void __init_user(void) {
    __init_cpp();
}
#endif

.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0

.global SMSGetNpcInitData__FUl
SMSGetNpcInitData__FUl:
/* 80175424 00172364  3C 80 80 3C */	lis r4, sAllNpcInitData$1622@ha
/* 80175428 00172368  54 63 10 3A */	slwi r3, r3, 2
/* 8017542C 0017236C  38 04 94 70 */	addi r0, r4, sAllNpcInitData$1622@l
/* 80175430 00172370  7C 60 1A 14 */	add r3, r0, r3
/* 80175434 00172374  80 63 00 00 */	lwz r3, 0(r3)
/* 80175438 00172378  4E 80 00 20 */	blr 

.global __sinit_NpcInitData_cpp
__sinit_NpcInitData_cpp:
/* 8017543C 0017237C  3C 60 80 3C */	lis r3, sMareMB_FishingRodData@ha
/* 80175440 00172380  80 0D 84 80 */	lwz r0, cNpcPartsNameRootJoint-_SDA_BASE_(r13)
/* 80175444 00172384  38 83 94 18 */	addi r4, r3, sMareMB_FishingRodData@l
/* 80175448 00172388  90 04 00 00 */	stw r0, 0(r4)
/* 8017544C 0017238C  3C 60 80 3C */	lis r3, sMareWB_BabyData@ha
/* 80175450 00172390  90 03 94 44 */	stw r0, sMareWB_BabyData@l(r3)
/* 80175454 00172394  4E 80 00 20 */	blr 

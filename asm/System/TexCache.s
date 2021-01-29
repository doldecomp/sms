.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0

.global SMS_ResetTexCacheRegion__Fv
SMS_ResetTexCacheRegion__Fv:
/* 800F91FC 000F613C  7C 08 02 A6 */	mflr r0
/* 800F9200 000F6140  3C 60 80 40 */	lis r3, j3dSys@ha
/* 800F9204 000F6144  90 01 00 04 */	stw r0, 4(r1)
/* 800F9208 000F6148  38 83 0E D8 */	addi r4, r3, j3dSys@l
/* 800F920C 000F614C  38 64 00 00 */	addi r3, r4, 0
/* 800F9210 000F6150  94 21 FF F8 */	stwu r1, -8(r1)
/* 800F9214 000F6154  80 04 00 34 */	lwz r0, 0x34(r4)
/* 800F9218 000F6158  64 00 80 00 */	oris r0, r0, 0x8000
/* 800F921C 000F615C  90 04 00 34 */	stw r0, 0x34(r4)
/* 800F9220 000F6160  38 80 00 01 */	li r4, 1
/* 800F9224 000F6164  4B F3 11 19 */	bl setTexCacheRegion__6J3DSysF15_GXTexCacheSize
/* 800F9228 000F6168  80 01 00 0C */	lwz r0, 0xc(r1)
/* 800F922C 000F616C  38 21 00 08 */	addi r1, r1, 8
/* 800F9230 000F6170  7C 08 03 A6 */	mtlr r0
/* 800F9234 000F6174  4E 80 00 20 */	blr 

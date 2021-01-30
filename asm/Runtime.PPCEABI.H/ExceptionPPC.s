.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __unregister_fragment
__unregister_fragment:
/* 80082B4C 0007FA8C  2C 03 00 00 */	cmpwi r3, 0
/* 80082B50 0007FA90  41 80 00 2C */	blt lbl_80082B7C
/* 80082B54 0007FA94  2C 03 00 01 */	cmpwi r3, 1
/* 80082B58 0007FA98  40 80 00 24 */	bge lbl_80082B7C
/* 80082B5C 0007FA9C  1C 83 00 0C */	mulli r4, r3, 0xc
/* 80082B60 0007FAA0  3C 60 80 3F */	lis r3, fragmentinfo@ha
/* 80082B64 0007FAA4  38 03 8E D0 */	addi r0, r3, fragmentinfo@l
/* 80082B68 0007FAA8  7C 60 22 14 */	add r3, r0, r4
/* 80082B6C 0007FAAC  38 00 00 00 */	li r0, 0
/* 80082B70 0007FAB0  90 03 00 00 */	stw r0, 0(r3)
/* 80082B74 0007FAB4  90 03 00 04 */	stw r0, 4(r3)
/* 80082B78 0007FAB8  90 03 00 08 */	stw r0, 8(r3)
lbl_80082B7C:
/* 80082B7C 0007FABC  4E 80 00 20 */	blr 

.global __register_fragment
__register_fragment:
/* 80082B80 0007FAC0  3C A0 80 3F */	lis r5, fragmentinfo@ha
/* 80082B84 0007FAC4  38 A5 8E D0 */	addi r5, r5, fragmentinfo@l
/* 80082B88 0007FAC8  48 00 00 04 */	b lbl_80082B8C
lbl_80082B8C:
/* 80082B8C 0007FACC  48 00 00 04 */	b lbl_80082B90
lbl_80082B90:
/* 80082B90 0007FAD0  80 05 00 08 */	lwz r0, 8(r5)
/* 80082B94 0007FAD4  2C 00 00 00 */	cmpwi r0, 0
/* 80082B98 0007FAD8  40 82 00 1C */	bne lbl_80082BB4
/* 80082B9C 0007FADC  90 65 00 00 */	stw r3, 0(r5)
/* 80082BA0 0007FAE0  38 00 00 01 */	li r0, 1
/* 80082BA4 0007FAE4  38 60 00 00 */	li r3, 0
/* 80082BA8 0007FAE8  90 85 00 04 */	stw r4, 4(r5)
/* 80082BAC 0007FAEC  90 05 00 08 */	stw r0, 8(r5)
/* 80082BB0 0007FAF0  48 00 00 08 */	b lbl_80082BB8
lbl_80082BB4:
/* 80082BB4 0007FAF4  38 60 FF FF */	li r3, -1
lbl_80082BB8:
/* 80082BB8 0007FAF8  4E 80 00 20 */	blr 

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
.global fragmentinfo
fragmentinfo:
	.skip 0x10

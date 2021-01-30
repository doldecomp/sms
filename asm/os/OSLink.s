.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __OSModuleInit
__OSModuleInit:
/* 80090A38 0008D978  3C 80 80 00 */	lis r4, 0x800030CC@ha
/* 80090A3C 0008D97C  38 00 00 00 */	li r0, 0
/* 80090A40 0008D980  90 04 30 CC */	stw r0, 0x800030CC@l(r4)
/* 80090A44 0008D984  90 04 30 C8 */	stw r0, 0x30c8(r4)
/* 80090A48 0008D988  90 04 30 D0 */	stw r0, 0x30d0(r4)
/* 80090A4C 0008D98C  4E 80 00 20 */	blr 

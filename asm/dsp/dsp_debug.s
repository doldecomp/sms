.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0

.global __DSP_debug_printf
__DSP_debug_printf:
/* 8009E79C 0009B6DC  94 21 FF 90 */	stwu r1, -0x70(r1)
/* 8009E7A0 0009B6E0  40 86 00 24 */	bne cr1, lbl_8009E7C4
/* 8009E7A4 0009B6E4  D8 21 00 28 */	stfd f1, 0x28(r1)
/* 8009E7A8 0009B6E8  D8 41 00 30 */	stfd f2, 0x30(r1)
/* 8009E7AC 0009B6EC  D8 61 00 38 */	stfd f3, 0x38(r1)
/* 8009E7B0 0009B6F0  D8 81 00 40 */	stfd f4, 0x40(r1)
/* 8009E7B4 0009B6F4  D8 A1 00 48 */	stfd f5, 0x48(r1)
/* 8009E7B8 0009B6F8  D8 C1 00 50 */	stfd f6, 0x50(r1)
/* 8009E7BC 0009B6FC  D8 E1 00 58 */	stfd f7, 0x58(r1)
/* 8009E7C0 0009B700  D9 01 00 60 */	stfd f8, 0x60(r1)
lbl_8009E7C4:
/* 8009E7C4 0009B704  90 61 00 08 */	stw r3, 8(r1)
/* 8009E7C8 0009B708  90 81 00 0C */	stw r4, 0xc(r1)
/* 8009E7CC 0009B70C  90 A1 00 10 */	stw r5, 0x10(r1)
/* 8009E7D0 0009B710  90 C1 00 14 */	stw r6, 0x14(r1)
/* 8009E7D4 0009B714  90 E1 00 18 */	stw r7, 0x18(r1)
/* 8009E7D8 0009B718  91 01 00 1C */	stw r8, 0x1c(r1)
/* 8009E7DC 0009B71C  91 21 00 20 */	stw r9, 0x20(r1)
/* 8009E7E0 0009B720  91 41 00 24 */	stw r10, 0x24(r1)
/* 8009E7E4 0009B724  38 21 00 70 */	addi r1, r1, 0x70
/* 8009E7E8 0009B728  4E 80 00 20 */	blr 

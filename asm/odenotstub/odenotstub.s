.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0

.global Hu_IsStub
Hu_IsStub:
/* 800AF010 000ABF50  38 60 00 00 */	li r3, 0
/* 800AF014 000ABF54  4E 80 00 20 */	blr 

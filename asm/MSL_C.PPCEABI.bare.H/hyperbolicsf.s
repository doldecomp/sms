.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0

.global fabsf__Ff
fabsf__Ff:
/* 80086B44 00083A84  FC 20 0A 10 */	fabs f1, f1
/* 80086B48 00083A88  4E 80 00 20 */	blr 

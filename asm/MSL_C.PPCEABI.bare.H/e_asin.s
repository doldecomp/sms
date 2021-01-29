.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0

.global fabs__Fd
fabs__Fd:
/* 800865A8 000834E8  FC 20 0A 10 */	fabs f1, f1
/* 800865AC 000834EC  4E 80 00 20 */	blr 

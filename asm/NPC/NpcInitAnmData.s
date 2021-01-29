.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0

.global SMSGetNpcInitAnmData__FUl
SMSGetNpcInitAnmData__FUl:
/* 8017AA20 00177960  3C 80 80 38 */	lis r4, sAllNpcInitAnmData$1577@ha
/* 8017AA24 00177964  54 63 18 38 */	slwi r3, r3, 3
/* 8017AA28 00177968  38 04 6D 88 */	addi r0, r4, sAllNpcInitAnmData$1577@l
/* 8017AA2C 0017796C  7C 60 1A 14 */	add r3, r0, r3
/* 8017AA30 00177970  4E 80 00 20 */	blr 

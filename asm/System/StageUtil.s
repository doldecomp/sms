.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global SMS_getShineIDofExStage__FUc
SMS_getShineIDofExStage__FUc:
/* 800FC504 000F9444  54 64 06 3E */	clrlwi r4, r3, 0x18
/* 800FC508 000F9448  28 04 00 14 */	cmplwi r4, 0x14
/* 800FC50C 000F944C  40 81 00 0C */	ble lbl_800FC518
/* 800FC510 000F9450  28 04 00 35 */	cmplwi r4, 0x35
/* 800FC514 000F9454  41 80 00 0C */	blt lbl_800FC520
lbl_800FC518:
/* 800FC518 000F9458  38 60 00 FF */	li r3, 0xff
/* 800FC51C 000F945C  4E 80 00 20 */	blr 
lbl_800FC520:
/* 800FC520 000F9460  3C 60 80 3B */	lis r3, exShineTable@ha
/* 800FC524 000F9464  38 03 41 28 */	addi r0, r3, exShineTable@l
/* 800FC528 000F9468  7C 60 22 14 */	add r3, r0, r4
/* 800FC52C 000F946C  88 63 FF EB */	lbz r3, -0x15(r3)
/* 800FC530 000F9470  4E 80 00 20 */	blr 

.global SMS_getShineStage__FUc
SMS_getShineStage__FUc:
/* 800FC534 000F9474  3C 80 80 3B */	lis r4, shineStageTable@ha
/* 800FC538 000F9478  54 63 06 3E */	clrlwi r3, r3, 0x18
/* 800FC53C 000F947C  38 04 40 E8 */	addi r0, r4, shineStageTable@l
/* 800FC540 000F9480  7C 60 1A 14 */	add r3, r0, r3
/* 800FC544 000F9484  88 63 00 00 */	lbz r3, 0(r3)
/* 800FC548 000F9488  4E 80 00 20 */	blr 

.global SMS_isOptionMap__Fv
SMS_isOptionMap__Fv:
/* 800FC54C 000F948C  80 8D 97 E8 */	lwz r4, gpMarDirector-_SDA_BASE_(r13)
/* 800FC550 000F9490  38 60 00 00 */	li r3, 0
/* 800FC554 000F9494  88 04 00 7C */	lbz r0, 0x7c(r4)
/* 800FC558 000F9498  28 00 00 0F */	cmplwi r0, 0xf
/* 800FC55C 000F949C  4C 82 00 20 */	bnelr 
/* 800FC560 000F94A0  38 60 00 01 */	li r3, 1
/* 800FC564 000F94A4  4E 80 00 20 */	blr 

.global SMS_isDivingMap__Fv
SMS_isDivingMap__Fv:
/* 800FC568 000F94A8  80 8D 97 E8 */	lwz r4, gpMarDirector-_SDA_BASE_(r13)
/* 800FC56C 000F94AC  38 60 00 00 */	li r3, 0
/* 800FC570 000F94B0  88 04 00 7C */	lbz r0, 0x7c(r4)
/* 800FC574 000F94B4  28 00 00 39 */	cmplwi r0, 0x39
/* 800FC578 000F94B8  40 82 00 08 */	bne lbl_800FC580
/* 800FC57C 000F94BC  38 60 00 01 */	li r3, 1
lbl_800FC580:
/* 800FC580 000F94C0  28 00 00 2C */	cmplwi r0, 0x2c
/* 800FC584 000F94C4  40 82 00 08 */	bne lbl_800FC58C
/* 800FC588 000F94C8  38 60 00 01 */	li r3, 1
lbl_800FC58C:
/* 800FC58C 000F94CC  28 00 00 10 */	cmplwi r0, 0x10
/* 800FC590 000F94D0  4C 82 00 20 */	bnelr 
/* 800FC594 000F94D4  38 60 00 01 */	li r3, 1
/* 800FC598 000F94D8  4E 80 00 20 */	blr 

.global SMS_isMultiPlayerMap__Fv
SMS_isMultiPlayerMap__Fv:
/* 800FC59C 000F94DC  80 8D 97 E8 */	lwz r4, gpMarDirector-_SDA_BASE_(r13)
/* 800FC5A0 000F94E0  38 60 00 00 */	li r3, 0
/* 800FC5A4 000F94E4  88 04 00 7C */	lbz r0, 0x7c(r4)
/* 800FC5A8 000F94E8  28 00 00 0C */	cmplwi r0, 0xc
/* 800FC5AC 000F94EC  4C 82 00 20 */	bnelr 
/* 800FC5B0 000F94F0  88 04 00 7D */	lbz r0, 0x7d(r4)
/* 800FC5B4 000F94F4  28 00 00 00 */	cmplwi r0, 0
/* 800FC5B8 000F94F8  4C 82 00 20 */	bnelr 
/* 800FC5BC 000F94FC  38 60 00 01 */	li r3, 1
/* 800FC5C0 000F9500  4E 80 00 20 */	blr 

.global SMS_isExMap__Fv
SMS_isExMap__Fv:
/* 800FC5C4 000F9504  3C 60 80 3E */	lis r3, gpApplication@ha
/* 800FC5C8 000F9508  38 63 60 00 */	addi r3, r3, gpApplication@l
/* 800FC5CC 000F950C  88 03 00 0E */	lbz r0, 0xe(r3)
/* 800FC5D0 000F9510  38 60 00 00 */	li r3, 0
/* 800FC5D4 000F9514  28 00 00 14 */	cmplwi r0, 0x14
/* 800FC5D8 000F9518  4C 81 00 20 */	blelr 
/* 800FC5DC 000F951C  28 00 00 35 */	cmplwi r0, 0x35
/* 800FC5E0 000F9520  4C 80 00 20 */	bgelr 
/* 800FC5E4 000F9524  38 60 00 01 */	li r3, 1
/* 800FC5E8 000F9528  4E 80 00 20 */	blr 

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global shineStageTable
shineStageTable:
	.incbin "baserom.dol", 0x3B10E8, 0x40
.global exShineTable
exShineTable:
	.incbin "baserom.dol", 0x3B1128, 0x20

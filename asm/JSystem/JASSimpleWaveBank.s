.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __ct__Q28JASystem15TSimpleWaveBankFv
__ct__Q28JASystem15TSimpleWaveBankFv:
/* 8005AB18 00057A58  7C 08 02 A6 */	mflr r0
/* 8005AB1C 00057A5C  3C 80 80 3B */	lis r4, __vt__Q38JASystem13WaveArcLoader7TObject@ha
/* 8005AB20 00057A60  90 01 00 04 */	stw r0, 4(r1)
/* 8005AB24 00057A64  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8005AB28 00057A68  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8005AB2C 00057A6C  3B E0 00 00 */	li r31, 0
/* 8005AB30 00057A70  93 C1 00 10 */	stw r30, 0x10(r1)
/* 8005AB34 00057A74  90 61 00 08 */	stw r3, 8(r1)
/* 8005AB38 00057A78  3C 60 80 3B */	lis r3, __vt__Q28JASystem9TWaveBank@ha
/* 8005AB3C 00057A7C  38 03 AE 6C */	addi r0, r3, __vt__Q28JASystem9TWaveBank@l
/* 8005AB40 00057A80  83 C1 00 08 */	lwz r30, 8(r1)
/* 8005AB44 00057A84  3C 60 80 3B */	lis r3, __vt__Q28JASystem15TSimpleWaveBank@ha
/* 8005AB48 00057A88  38 63 AE DC */	addi r3, r3, __vt__Q28JASystem15TSimpleWaveBank@l
/* 8005AB4C 00057A8C  90 1E 00 00 */	stw r0, 0(r30)
/* 8005AB50 00057A90  38 04 AE 44 */	addi r0, r4, __vt__Q38JASystem13WaveArcLoader7TObject@l
/* 8005AB54 00057A94  90 1E 00 04 */	stw r0, 4(r30)
/* 8005AB58 00057A98  38 03 00 14 */	addi r0, r3, 0x14
/* 8005AB5C 00057A9C  90 7E 00 00 */	stw r3, 0(r30)
/* 8005AB60 00057AA0  38 7E 00 14 */	addi r3, r30, 0x14
/* 8005AB64 00057AA4  90 1E 00 04 */	stw r0, 4(r30)
/* 8005AB68 00057AA8  93 FE 00 08 */	stw r31, 8(r30)
/* 8005AB6C 00057AAC  93 FE 00 0C */	stw r31, 0xc(r30)
/* 8005AB70 00057AB0  93 FE 00 10 */	stw r31, 0x10(r30)
/* 8005AB74 00057AB4  48 00 8B 75 */	bl __ct__Q38JASystem6Kernel5THeapFv
/* 8005AB78 00057AB8  93 FE 00 40 */	stw r31, 0x40(r30)
/* 8005AB7C 00057ABC  7F C3 F3 78 */	mr r3, r30
/* 8005AB80 00057AC0  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8005AB84 00057AC4  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8005AB88 00057AC8  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 8005AB8C 00057ACC  7C 08 03 A6 */	mtlr r0
/* 8005AB90 00057AD0  38 21 00 18 */	addi r1, r1, 0x18
/* 8005AB94 00057AD4  4E 80 00 20 */	blr 

.global __dt__Q28JASystem15TSimpleWaveBankFv
__dt__Q28JASystem15TSimpleWaveBankFv:
/* 8005AB98 00057AD8  7C 08 02 A6 */	mflr r0
/* 8005AB9C 00057ADC  90 01 00 04 */	stw r0, 4(r1)
/* 8005ABA0 00057AE0  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8005ABA4 00057AE4  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8005ABA8 00057AE8  3B E4 00 00 */	addi r31, r4, 0
/* 8005ABAC 00057AEC  93 C1 00 10 */	stw r30, 0x10(r1)
/* 8005ABB0 00057AF0  7C 7E 1B 79 */	or. r30, r3, r3
/* 8005ABB4 00057AF4  41 82 00 54 */	beq lbl_8005AC08
/* 8005ABB8 00057AF8  3C 60 80 3B */	lis r3, __vt__Q28JASystem15TSimpleWaveBank@ha
/* 8005ABBC 00057AFC  38 63 AE DC */	addi r3, r3, __vt__Q28JASystem15TSimpleWaveBank@l
/* 8005ABC0 00057B00  90 7E 00 00 */	stw r3, 0(r30)
/* 8005ABC4 00057B04  38 03 00 14 */	addi r0, r3, 0x14
/* 8005ABC8 00057B08  3C 60 80 06 */	lis r3, __dt__Q38JASystem15TSimpleWaveBank11TWaveHandleFv@ha
/* 8005ABCC 00057B0C  90 1E 00 04 */	stw r0, 4(r30)
/* 8005ABD0 00057B10  38 83 AC 24 */	addi r4, r3, __dt__Q38JASystem15TSimpleWaveBank11TWaveHandleFv@l
/* 8005ABD4 00057B14  80 7E 00 08 */	lwz r3, 8(r30)
/* 8005ABD8 00057B18  48 02 7B 69 */	bl __destroy_new_array
/* 8005ABDC 00057B1C  80 7E 00 10 */	lwz r3, 0x10(r30)
/* 8005ABE0 00057B20  4B FB 1F 35 */	bl __dla__FPv
/* 8005ABE4 00057B24  28 1E 00 00 */	cmplwi r30, 0
/* 8005ABE8 00057B28  41 82 00 10 */	beq lbl_8005ABF8
/* 8005ABEC 00057B2C  3C 60 80 3B */	lis r3, __vt__Q28JASystem9TWaveBank@ha
/* 8005ABF0 00057B30  38 03 AE 6C */	addi r0, r3, __vt__Q28JASystem9TWaveBank@l
/* 8005ABF4 00057B34  90 1E 00 00 */	stw r0, 0(r30)
lbl_8005ABF8:
/* 8005ABF8 00057B38  7F E0 07 35 */	extsh. r0, r31
/* 8005ABFC 00057B3C  40 81 00 0C */	ble lbl_8005AC08
/* 8005AC00 00057B40  7F C3 F3 78 */	mr r3, r30
/* 8005AC04 00057B44  4B FB 1E AD */	bl __dl__FPv
lbl_8005AC08:
/* 8005AC08 00057B48  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8005AC0C 00057B4C  7F C3 F3 78 */	mr r3, r30
/* 8005AC10 00057B50  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8005AC14 00057B54  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 8005AC18 00057B58  7C 08 03 A6 */	mtlr r0
/* 8005AC1C 00057B5C  38 21 00 18 */	addi r1, r1, 0x18
/* 8005AC20 00057B60  4E 80 00 20 */	blr 

.global __dt__Q38JASystem15TSimpleWaveBank11TWaveHandleFv
__dt__Q38JASystem15TSimpleWaveBank11TWaveHandleFv:
/* 8005AC24 00057B64  7C 08 02 A6 */	mflr r0
/* 8005AC28 00057B68  90 01 00 04 */	stw r0, 4(r1)
/* 8005AC2C 00057B6C  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8005AC30 00057B70  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8005AC34 00057B74  7C 7F 1B 79 */	or. r31, r3, r3
/* 8005AC38 00057B78  41 82 00 30 */	beq lbl_8005AC68
/* 8005AC3C 00057B7C  3C 60 80 3B */	lis r3, __vt__Q38JASystem15TSimpleWaveBank11TWaveHandle@ha
/* 8005AC40 00057B80  38 03 AE C8 */	addi r0, r3, __vt__Q38JASystem15TSimpleWaveBank11TWaveHandle@l
/* 8005AC44 00057B84  90 1F 00 00 */	stw r0, 0(r31)
/* 8005AC48 00057B88  41 82 00 10 */	beq lbl_8005AC58
/* 8005AC4C 00057B8C  3C 60 80 3B */	lis r3, __vt__Q28JASystem11TWaveHandle@ha
/* 8005AC50 00057B90  38 03 AE 08 */	addi r0, r3, __vt__Q28JASystem11TWaveHandle@l
/* 8005AC54 00057B94  90 1F 00 00 */	stw r0, 0(r31)
lbl_8005AC58:
/* 8005AC58 00057B98  7C 80 07 35 */	extsh. r0, r4
/* 8005AC5C 00057B9C  40 81 00 0C */	ble lbl_8005AC68
/* 8005AC60 00057BA0  7F E3 FB 78 */	mr r3, r31
/* 8005AC64 00057BA4  4B FB 1E 4D */	bl __dl__FPv
lbl_8005AC68:
/* 8005AC68 00057BA8  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8005AC6C 00057BAC  7F E3 FB 78 */	mr r3, r31
/* 8005AC70 00057BB0  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8005AC74 00057BB4  38 21 00 18 */	addi r1, r1, 0x18
/* 8005AC78 00057BB8  7C 08 03 A6 */	mtlr r0
/* 8005AC7C 00057BBC  4E 80 00 20 */	blr 

.global setWaveTableSize__Q28JASystem15TSimpleWaveBankFUl
setWaveTableSize__Q28JASystem15TSimpleWaveBankFUl:
/* 8005AC80 00057BC0  7C 08 02 A6 */	mflr r0
/* 8005AC84 00057BC4  3C A0 80 06 */	lis r5, __dt__Q38JASystem15TSimpleWaveBank11TWaveHandleFv@ha
/* 8005AC88 00057BC8  90 01 00 04 */	stw r0, 4(r1)
/* 8005AC8C 00057BCC  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 8005AC90 00057BD0  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 8005AC94 00057BD4  3B E5 AC 24 */	addi r31, r5, __dt__Q38JASystem15TSimpleWaveBank11TWaveHandleFv@l
/* 8005AC98 00057BD8  93 C1 00 18 */	stw r30, 0x18(r1)
/* 8005AC9C 00057BDC  3B C4 00 00 */	addi r30, r4, 0
/* 8005ACA0 00057BE0  38 9F 00 00 */	addi r4, r31, 0
/* 8005ACA4 00057BE4  93 A1 00 14 */	stw r29, 0x14(r1)
/* 8005ACA8 00057BE8  3B A3 00 00 */	addi r29, r3, 0
/* 8005ACAC 00057BEC  80 63 00 08 */	lwz r3, 8(r3)
/* 8005ACB0 00057BF0  48 02 7A 91 */	bl __destroy_new_array
/* 8005ACB4 00057BF4  48 00 04 BD */	bl getCurrentHeap__Q28JASystem9TWaveBankFv
/* 8005ACB8 00057BF8  1C BE 00 34 */	mulli r5, r30, 0x34
/* 8005ACBC 00057BFC  38 83 00 00 */	addi r4, r3, 0
/* 8005ACC0 00057C00  38 65 00 08 */	addi r3, r5, 8
/* 8005ACC4 00057C04  38 A0 00 00 */	li r5, 0
/* 8005ACC8 00057C08  4B FB 1D 7D */	bl __nwa__FUlP7JKRHeapi
/* 8005ACCC 00057C0C  3C 80 80 06 */	lis r4, __ct__Q38JASystem15TSimpleWaveBank11TWaveHandleFv@ha
/* 8005ACD0 00057C10  38 84 AD 08 */	addi r4, r4, __ct__Q38JASystem15TSimpleWaveBank11TWaveHandleFv@l
/* 8005ACD4 00057C14  38 BF 00 00 */	addi r5, r31, 0
/* 8005ACD8 00057C18  38 FE 00 00 */	addi r7, r30, 0
/* 8005ACDC 00057C1C  38 C0 00 34 */	li r6, 0x34
/* 8005ACE0 00057C20  48 02 7D 0D */	bl __construct_new_array
/* 8005ACE4 00057C24  90 7D 00 08 */	stw r3, 8(r29)
/* 8005ACE8 00057C28  93 DD 00 0C */	stw r30, 0xc(r29)
/* 8005ACEC 00057C2C  80 01 00 24 */	lwz r0, 0x24(r1)
/* 8005ACF0 00057C30  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 8005ACF4 00057C34  83 C1 00 18 */	lwz r30, 0x18(r1)
/* 8005ACF8 00057C38  7C 08 03 A6 */	mtlr r0
/* 8005ACFC 00057C3C  83 A1 00 14 */	lwz r29, 0x14(r1)
/* 8005AD00 00057C40  38 21 00 20 */	addi r1, r1, 0x20
/* 8005AD04 00057C44  4E 80 00 20 */	blr 

.global __ct__Q38JASystem15TSimpleWaveBank11TWaveHandleFv
__ct__Q38JASystem15TSimpleWaveBank11TWaveHandleFv:
/* 8005AD08 00057C48  3C 80 80 3B */	lis r4, __vt__Q28JASystem11TWaveHandle@ha
/* 8005AD0C 00057C4C  38 04 AE 08 */	addi r0, r4, __vt__Q28JASystem11TWaveHandle@l
/* 8005AD10 00057C50  3C 80 80 3B */	lis r4, __vt__Q38JASystem15TSimpleWaveBank11TWaveHandle@ha
/* 8005AD14 00057C54  90 03 00 00 */	stw r0, 0(r3)
/* 8005AD18 00057C58  38 04 AE C8 */	addi r0, r4, __vt__Q38JASystem15TSimpleWaveBank11TWaveHandle@l
/* 8005AD1C 00057C5C  90 03 00 00 */	stw r0, 0(r3)
/* 8005AD20 00057C60  38 00 00 00 */	li r0, 0
/* 8005AD24 00057C64  90 03 00 30 */	stw r0, 0x30(r3)
/* 8005AD28 00057C68  4E 80 00 20 */	blr 

.global getWaveHandle__Q28JASystem15TSimpleWaveBankCFUl
getWaveHandle__Q28JASystem15TSimpleWaveBankCFUl:
/* 8005AD2C 00057C6C  80 03 00 0C */	lwz r0, 0xc(r3)
/* 8005AD30 00057C70  7C 04 00 40 */	cmplw r4, r0
/* 8005AD34 00057C74  41 80 00 0C */	blt lbl_8005AD40
/* 8005AD38 00057C78  38 60 00 00 */	li r3, 0
/* 8005AD3C 00057C7C  4E 80 00 20 */	blr 
lbl_8005AD40:
/* 8005AD40 00057C80  1C 04 00 34 */	mulli r0, r4, 0x34
/* 8005AD44 00057C84  80 63 00 08 */	lwz r3, 8(r3)
/* 8005AD48 00057C88  7C 63 02 14 */	add r3, r3, r0
/* 8005AD4C 00057C8C  4E 80 00 20 */	blr 

.global setWaveInfo__Q28JASystem15TSimpleWaveBankFUlRCQ28JASystem9TWaveInfo
setWaveInfo__Q28JASystem15TSimpleWaveBankFUlRCQ28JASystem9TWaveInfo:
/* 8005AD50 00057C90  1D 24 00 34 */	mulli r9, r4, 0x34
/* 8005AD54 00057C94  80 C3 00 08 */	lwz r6, 8(r3)
/* 8005AD58 00057C98  80 85 00 00 */	lwz r4, 0(r5)
/* 8005AD5C 00057C9C  80 05 00 04 */	lwz r0, 4(r5)
/* 8005AD60 00057CA0  7D 06 4A 14 */	add r8, r6, r9
/* 8005AD64 00057CA4  90 88 00 04 */	stw r4, 4(r8)
/* 8005AD68 00057CA8  38 C3 00 40 */	addi r6, r3, 0x40
/* 8005AD6C 00057CAC  38 83 00 14 */	addi r4, r3, 0x14
/* 8005AD70 00057CB0  90 08 00 08 */	stw r0, 8(r8)
/* 8005AD74 00057CB4  80 E5 00 08 */	lwz r7, 8(r5)
/* 8005AD78 00057CB8  80 05 00 0C */	lwz r0, 0xc(r5)
/* 8005AD7C 00057CBC  90 E8 00 0C */	stw r7, 0xc(r8)
/* 8005AD80 00057CC0  90 08 00 10 */	stw r0, 0x10(r8)
/* 8005AD84 00057CC4  80 E5 00 10 */	lwz r7, 0x10(r5)
/* 8005AD88 00057CC8  80 05 00 14 */	lwz r0, 0x14(r5)
/* 8005AD8C 00057CCC  90 E8 00 14 */	stw r7, 0x14(r8)
/* 8005AD90 00057CD0  90 08 00 18 */	stw r0, 0x18(r8)
/* 8005AD94 00057CD4  80 E5 00 18 */	lwz r7, 0x18(r5)
/* 8005AD98 00057CD8  80 05 00 1C */	lwz r0, 0x1c(r5)
/* 8005AD9C 00057CDC  90 E8 00 1C */	stw r7, 0x1c(r8)
/* 8005ADA0 00057CE0  90 08 00 20 */	stw r0, 0x20(r8)
/* 8005ADA4 00057CE4  80 E5 00 20 */	lwz r7, 0x20(r5)
/* 8005ADA8 00057CE8  80 05 00 24 */	lwz r0, 0x24(r5)
/* 8005ADAC 00057CEC  90 E8 00 24 */	stw r7, 0x24(r8)
/* 8005ADB0 00057CF0  90 08 00 28 */	stw r0, 0x28(r8)
/* 8005ADB4 00057CF4  80 05 00 28 */	lwz r0, 0x28(r5)
/* 8005ADB8 00057CF8  90 08 00 2C */	stw r0, 0x2c(r8)
/* 8005ADBC 00057CFC  80 03 00 08 */	lwz r0, 8(r3)
/* 8005ADC0 00057D00  7C A0 4A 14 */	add r5, r0, r9
/* 8005ADC4 00057D04  90 C5 00 28 */	stw r6, 0x28(r5)
/* 8005ADC8 00057D08  80 03 00 08 */	lwz r0, 8(r3)
/* 8005ADCC 00057D0C  7C 60 4A 14 */	add r3, r0, r9
/* 8005ADD0 00057D10  90 83 00 30 */	stw r4, 0x30(r3)
/* 8005ADD4 00057D14  4E 80 00 20 */	blr 

.global setWaveArcFileName__Q28JASystem15TSimpleWaveBankFPCc
setWaveArcFileName__Q28JASystem15TSimpleWaveBankFPCc:
/* 8005ADD8 00057D18  7C 08 02 A6 */	mflr r0
/* 8005ADDC 00057D1C  90 01 00 04 */	stw r0, 4(r1)
/* 8005ADE0 00057D20  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 8005ADE4 00057D24  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 8005ADE8 00057D28  93 C1 00 18 */	stw r30, 0x18(r1)
/* 8005ADEC 00057D2C  7C 9E 23 78 */	mr r30, r4
/* 8005ADF0 00057D30  93 A1 00 14 */	stw r29, 0x14(r1)
/* 8005ADF4 00057D34  7C 7D 1B 78 */	mr r29, r3
/* 8005ADF8 00057D38  80 63 00 10 */	lwz r3, 0x10(r3)
/* 8005ADFC 00057D3C  4B FB 1D 19 */	bl __dla__FPv
/* 8005AE00 00057D40  7F C3 F3 78 */	mr r3, r30
/* 8005AE04 00057D44  48 02 B0 A1 */	bl strlen
/* 8005AE08 00057D48  7C 7F 1B 78 */	mr r31, r3
/* 8005AE0C 00057D4C  48 00 03 65 */	bl getCurrentHeap__Q28JASystem9TWaveBankFv
/* 8005AE10 00057D50  38 83 00 00 */	addi r4, r3, 0
/* 8005AE14 00057D54  38 7F 00 01 */	addi r3, r31, 1
/* 8005AE18 00057D58  38 A0 00 00 */	li r5, 0
/* 8005AE1C 00057D5C  4B FB 1C 29 */	bl __nwa__FUlP7JKRHeapi
/* 8005AE20 00057D60  90 7D 00 10 */	stw r3, 0x10(r29)
/* 8005AE24 00057D64  7F C4 F3 78 */	mr r4, r30
/* 8005AE28 00057D68  80 7D 00 10 */	lwz r3, 0x10(r29)
/* 8005AE2C 00057D6C  48 02 AF C5 */	bl strcpy
/* 8005AE30 00057D70  80 01 00 24 */	lwz r0, 0x24(r1)
/* 8005AE34 00057D74  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 8005AE38 00057D78  83 C1 00 18 */	lwz r30, 0x18(r1)
/* 8005AE3C 00057D7C  7C 08 03 A6 */	mtlr r0
/* 8005AE40 00057D80  83 A1 00 14 */	lwz r29, 0x14(r1)
/* 8005AE44 00057D84  38 21 00 20 */	addi r1, r1, 0x20
/* 8005AE48 00057D88  4E 80 00 20 */	blr 

.global getWaveInfo__Q38JASystem15TSimpleWaveBank11TWaveHandleCFv
getWaveInfo__Q38JASystem15TSimpleWaveBank11TWaveHandleCFv:
/* 8005AE4C 00057D8C  38 63 00 04 */	addi r3, r3, 4
/* 8005AE50 00057D90  4E 80 00 20 */	blr 

.global getWavePtr__Q38JASystem15TSimpleWaveBank11TWaveHandleCFv
getWavePtr__Q38JASystem15TSimpleWaveBank11TWaveHandleCFv:
/* 8005AE54 00057D94  80 83 00 30 */	lwz r4, 0x30(r3)
/* 8005AE58 00057D98  80 84 00 08 */	lwz r4, 8(r4)
/* 8005AE5C 00057D9C  28 04 00 00 */	cmplwi r4, 0
/* 8005AE60 00057DA0  40 82 00 0C */	bne lbl_8005AE6C
/* 8005AE64 00057DA4  38 60 00 00 */	li r3, 0
/* 8005AE68 00057DA8  4E 80 00 20 */	blr 
lbl_8005AE6C:
/* 8005AE6C 00057DAC  80 03 00 0C */	lwz r0, 0xc(r3)
/* 8005AE70 00057DB0  7C 64 02 14 */	add r3, r4, r0
/* 8005AE74 00057DB4  4E 80 00 20 */	blr 

.global getType__Q28JASystem15TSimpleWaveBankCFv
getType__Q28JASystem15TSimpleWaveBankCFv:
/* 8005AE78 00057DB8  3C 60 53 4D */	lis r3, 0x534D504C@ha
/* 8005AE7C 00057DBC  38 63 50 4C */	addi r3, r3, 0x534D504C@l
/* 8005AE80 00057DC0  4E 80 00 20 */	blr 

.global getHeap__Q28JASystem15TSimpleWaveBankFv
getHeap__Q28JASystem15TSimpleWaveBankFv:
/* 8005AE84 00057DC4  38 63 00 14 */	addi r3, r3, 0x14
/* 8005AE88 00057DC8  4E 80 00 20 */	blr 

.global getWaveArcFileName__Q28JASystem15TSimpleWaveBankCFv
getWaveArcFileName__Q28JASystem15TSimpleWaveBankCFv:
/* 8005AE8C 00057DCC  80 63 00 10 */	lwz r3, 0x10(r3)
/* 8005AE90 00057DD0  4E 80 00 20 */	blr 

.global getLoadFlagPtr__Q28JASystem15TSimpleWaveBankFv
getLoadFlagPtr__Q28JASystem15TSimpleWaveBankFv:
/* 8005AE94 00057DD4  38 63 00 40 */	addi r3, r3, 0x40
/* 8005AE98 00057DD8  4E 80 00 20 */	blr 

.global "@4@getHeap__Q28JASystem15TSimpleWaveBankFv"
"@4@getHeap__Q28JASystem15TSimpleWaveBankFv":
/* 8005AE9C 00057DDC  38 63 FF FC */	addi r3, r3, -4
/* 8005AEA0 00057DE0  4B FF FF E4 */	b getHeap__Q28JASystem15TSimpleWaveBankFv

.global "@4@getWaveArcFileName__Q28JASystem15TSimpleWaveBankCFv"
"@4@getWaveArcFileName__Q28JASystem15TSimpleWaveBankCFv":
/* 8005AEA4 00057DE4  38 63 FF FC */	addi r3, r3, -4
/* 8005AEA8 00057DE8  4B FF FF E4 */	b getWaveArcFileName__Q28JASystem15TSimpleWaveBankCFv

.global "@4@getLoadFlagPtr__Q28JASystem15TSimpleWaveBankFv"
"@4@getLoadFlagPtr__Q28JASystem15TSimpleWaveBankFv":
/* 8005AEAC 00057DEC  38 63 FF FC */	addi r3, r3, -4
/* 8005AEB0 00057DF0  4B FF FF E4 */	b getLoadFlagPtr__Q28JASystem15TSimpleWaveBankFv

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__Q38JASystem15TSimpleWaveBank11TWaveHandle
__vt__Q38JASystem15TSimpleWaveBank11TWaveHandle:
  .4byte 0
  .4byte 0
  .4byte __dt__Q38JASystem15TSimpleWaveBank11TWaveHandleFv
  .4byte getWaveInfo__Q38JASystem15TSimpleWaveBank11TWaveHandleCFv
  .4byte getWavePtr__Q38JASystem15TSimpleWaveBank11TWaveHandleCFv
.global __vt__Q28JASystem15TSimpleWaveBank
__vt__Q28JASystem15TSimpleWaveBank:
  .4byte 0
  .4byte 0
  .4byte __dt__Q28JASystem15TSimpleWaveBankFv
  .4byte getWaveHandle__Q28JASystem15TSimpleWaveBankCFUl
  .4byte getType__Q28JASystem15TSimpleWaveBankCFv
  .4byte 0
  .4byte 0
  .4byte "@4@getLoadFlagPtr__Q28JASystem15TSimpleWaveBankFv"
  .4byte "@4@getWaveArcFileName__Q28JASystem15TSimpleWaveBankCFv"
  .4byte "@4@getHeap__Q28JASystem15TSimpleWaveBankFv"
  .4byte getLoadFlagPtr__Q28JASystem15TSimpleWaveBankFv
  .4byte getWaveArcFileName__Q28JASystem15TSimpleWaveBankCFv
  .4byte getHeap__Q28JASystem15TSimpleWaveBankFv

.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global DspHandShake__FPv
DspHandShake__FPv:
/* 80081DA0 0007ECE0  7C 08 02 A6 */	mflr r0
/* 80081DA4 0007ECE4  90 01 00 04 */	stw r0, 4(r1)
/* 80081DA8 0007ECE8  94 21 FF E8 */	stwu r1, -0x18(r1)
lbl_80081DAC:
/* 80081DAC 0007ECEC  48 01 C8 BD */	bl DSPCheckMailFromDSP
/* 80081DB0 0007ECF0  90 61 00 14 */	stw r3, 0x14(r1)
/* 80081DB4 0007ECF4  80 01 00 14 */	lwz r0, 0x14(r1)
/* 80081DB8 0007ECF8  28 00 00 00 */	cmplwi r0, 0
/* 80081DBC 0007ECFC  41 82 FF F0 */	beq lbl_80081DAC
/* 80081DC0 0007ED00  48 01 C8 B9 */	bl DSPReadMailFromDSP
/* 80081DC4 0007ED04  48 01 C8 A5 */	bl DSPCheckMailFromDSP
/* 80081DC8 0007ED08  48 00 08 19 */	bl Dsp_Running_Start__Fv
/* 80081DCC 0007ED0C  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80081DD0 0007ED10  38 21 00 18 */	addi r1, r1, 0x18
/* 80081DD4 0007ED14  7C 08 03 A6 */	mtlr r0
/* 80081DD8 0007ED18  4E 80 00 20 */	blr 
/* 80081DDC 0007ED1C  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */

.global DspBoot__FPFPv_v
DspBoot__FPFPv_v:
/* 80081DE0 0007ED20  7C 08 02 A6 */	mflr r0
/* 80081DE4 0007ED24  3C 80 80 3E */	lis r4, audio_task@ha
/* 80081DE8 0007ED28  90 01 00 04 */	stw r0, 4(r1)
/* 80081DEC 0007ED2C  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80081DF0 0007ED30  BF C1 00 10 */	stmw r30, 0x10(r1)
/* 80081DF4 0007ED34  3B C3 00 00 */	addi r30, r3, 0
/* 80081DF8 0007ED38  3B E4 6D E0 */	addi r31, r4, audio_task@l
/* 80081DFC 0007ED3C  48 00 01 85 */	bl DspInitWork__Fv
/* 80081E00 0007ED40  38 00 00 F0 */	li r0, 0xf0
/* 80081E04 0007ED44  3C 60 80 3B */	lis r3, jdsp@ha
/* 80081E08 0007ED48  38 63 CF 20 */	addi r3, r3, jdsp@l
/* 80081E0C 0007ED4C  90 1F 00 04 */	stw r0, 4(r31)
/* 80081E10 0007ED50  3C 03 80 00 */	addis r0, r3, 0x8000
/* 80081E14 0007ED54  3C 9F 80 00 */	addis r4, r31, 0x8000
/* 80081E18 0007ED58  90 1F 00 0C */	stw r0, 0xc(r31)
/* 80081E1C 0007ED5C  38 00 1C E0 */	li r0, 0x1ce0
/* 80081E20 0007ED60  3C 60 80 08 */	lis r3, DspHandShake__FPv@ha
/* 80081E24 0007ED64  38 E0 00 00 */	li r7, 0
/* 80081E28 0007ED68  90 1F 00 10 */	stw r0, 0x10(r31)
/* 80081E2C 0007ED6C  38 C4 00 60 */	addi r6, r4, 0x60
/* 80081E30 0007ED70  38 A0 20 00 */	li r5, 0x2000
/* 80081E34 0007ED74  38 80 00 10 */	li r4, 0x10
/* 80081E38 0007ED78  90 FF 00 14 */	stw r7, 0x14(r31)
/* 80081E3C 0007ED7C  38 03 1D A0 */	addi r0, r3, DspHandShake__FPv@l
/* 80081E40 0007ED80  90 DF 00 18 */	stw r6, 0x18(r31)
/* 80081E44 0007ED84  90 BF 00 1C */	stw r5, 0x1c(r31)
/* 80081E48 0007ED88  90 FF 00 20 */	stw r7, 0x20(r31)
/* 80081E4C 0007ED8C  B0 FF 00 24 */	sth r7, 0x24(r31)
/* 80081E50 0007ED90  B0 9F 00 26 */	sth r4, 0x26(r31)
/* 80081E54 0007ED94  90 1F 00 28 */	stw r0, 0x28(r31)
/* 80081E58 0007ED98  90 FF 00 2C */	stw r7, 0x2c(r31)
/* 80081E5C 0007ED9C  90 FF 00 30 */	stw r7, 0x30(r31)
/* 80081E60 0007EDA0  93 DF 00 34 */	stw r30, 0x34(r31)
/* 80081E64 0007EDA4  48 01 C8 81 */	bl DSPInit
/* 80081E68 0007EDA8  7F E3 FB 78 */	mr r3, r31
/* 80081E6C 0007EDAC  48 00 02 B5 */	bl DSPAddPriorTask__FP15STRUCT_DSP_TASK
/* 80081E70 0007EDB0  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80081E74 0007EDB4  BB C1 00 10 */	lmw r30, 0x10(r1)
/* 80081E78 0007EDB8  38 21 00 18 */	addi r1, r1, 0x18
/* 80081E7C 0007EDBC  7C 08 03 A6 */	mtlr r0
/* 80081E80 0007EDC0  4E 80 00 20 */	blr 
/* 80081E84 0007EDC4  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081E88 0007EDC8  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081E8C 0007EDCC  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081E90 0007EDD0  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081E94 0007EDD4  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081E98 0007EDD8  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081E9C 0007EDDC  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */

.global DSPSendCommands2__FPUlUlPFUs_v
DSPSendCommands2__FPUlUlPFUs_v:
/* 80081EA0 0007EDE0  7C 08 02 A6 */	mflr r0
/* 80081EA4 0007EDE4  90 01 00 04 */	stw r0, 4(r1)
/* 80081EA8 0007EDE8  94 21 FF D0 */	stwu r1, -0x30(r1)
/* 80081EAC 0007EDEC  BF 41 00 18 */	stmw r26, 0x18(r1)
/* 80081EB0 0007EDF0  3B 43 00 00 */	addi r26, r3, 0
/* 80081EB4 0007EDF4  3B 64 00 00 */	addi r27, r4, 0
/* 80081EB8 0007EDF8  3B C5 00 00 */	addi r30, r5, 0
lbl_80081EBC:
/* 80081EBC 0007EDFC  48 00 07 05 */	bl Dsp_Running_Check__Fv
/* 80081EC0 0007EE00  2C 03 00 00 */	cmpwi r3, 0
/* 80081EC4 0007EE04  41 82 FF F8 */	beq lbl_80081EBC
/* 80081EC8 0007EE08  48 00 E3 05 */	bl OSDisableInterrupts
/* 80081ECC 0007EE0C  7C 7D 1B 78 */	mr r29, r3
/* 80081ED0 0007EE10  48 01 C7 89 */	bl DSPCheckMailToDSP
/* 80081ED4 0007EE14  28 03 00 00 */	cmplwi r3, 0
/* 80081ED8 0007EE18  41 82 00 14 */	beq lbl_80081EEC
/* 80081EDC 0007EE1C  7F A3 EB 78 */	mr r3, r29
/* 80081EE0 0007EE20  48 00 E3 15 */	bl OSRestoreInterrupts
/* 80081EE4 0007EE24  38 60 FF FF */	li r3, -1
/* 80081EE8 0007EE28  48 00 00 7C */	b lbl_80081F64
lbl_80081EEC:
/* 80081EEC 0007EE2C  7F 63 DB 78 */	mr r3, r27
/* 80081EF0 0007EE30  48 01 C7 A1 */	bl DSPSendMailToDSP
/* 80081EF4 0007EE34  48 01 C7 B1 */	bl DSPAssertInt
lbl_80081EF8:
/* 80081EF8 0007EE38  48 01 C7 61 */	bl DSPCheckMailToDSP
/* 80081EFC 0007EE3C  28 03 00 00 */	cmplwi r3, 0
/* 80081F00 0007EE40  40 82 FF F8 */	bne lbl_80081EF8
/* 80081F04 0007EE44  28 1B 00 00 */	cmplwi r27, 0
/* 80081F08 0007EE48  40 82 00 08 */	bne lbl_80081F10
/* 80081F0C 0007EE4C  3B 60 00 02 */	li r27, 2
lbl_80081F10:
/* 80081F10 0007EE50  28 1E 00 00 */	cmplwi r30, 0
/* 80081F14 0007EE54  41 82 00 14 */	beq lbl_80081F28
/* 80081F18 0007EE58  80 7A 00 00 */	lwz r3, 0(r26)
/* 80081F1C 0007EE5C  7F C4 F3 78 */	mr r4, r30
/* 80081F20 0007EE60  48 00 00 A1 */	bl DspStartWork__FUlPFUs_v
/* 80081F24 0007EE64  7C 7C 1B 78 */	mr r28, r3
lbl_80081F28:
/* 80081F28 0007EE68  3B C0 00 00 */	li r30, 0
/* 80081F2C 0007EE6C  3B E0 00 00 */	li r31, 0
/* 80081F30 0007EE70  48 00 00 20 */	b lbl_80081F50
lbl_80081F34:
/* 80081F34 0007EE74  7C 7A F8 2E */	lwzx r3, r26, r31
/* 80081F38 0007EE78  48 01 C7 59 */	bl DSPSendMailToDSP
lbl_80081F3C:
/* 80081F3C 0007EE7C  48 01 C7 1D */	bl DSPCheckMailToDSP
/* 80081F40 0007EE80  28 03 00 00 */	cmplwi r3, 0
/* 80081F44 0007EE84  40 82 FF F8 */	bne lbl_80081F3C
/* 80081F48 0007EE88  3B DE 00 01 */	addi r30, r30, 1
/* 80081F4C 0007EE8C  3B FF 00 04 */	addi r31, r31, 4
lbl_80081F50:
/* 80081F50 0007EE90  7C 1E D8 40 */	cmplw r30, r27
/* 80081F54 0007EE94  41 80 FF E0 */	blt lbl_80081F34
/* 80081F58 0007EE98  7F A3 EB 78 */	mr r3, r29
/* 80081F5C 0007EE9C  48 00 E2 99 */	bl OSRestoreInterrupts
/* 80081F60 0007EEA0  7F 83 E3 78 */	mr r3, r28
lbl_80081F64:
/* 80081F64 0007EEA4  BB 41 00 18 */	lmw r26, 0x18(r1)
/* 80081F68 0007EEA8  80 01 00 34 */	lwz r0, 0x34(r1)
/* 80081F6C 0007EEAC  38 21 00 30 */	addi r1, r1, 0x30
/* 80081F70 0007EEB0  7C 08 03 A6 */	mtlr r0
/* 80081F74 0007EEB4  4E 80 00 20 */	blr 
/* 80081F78 0007EEB8  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081F7C 0007EEBC  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */

.global DspInitWork__Fv
DspInitWork__Fv:
/* 80081F80 0007EEC0  38 60 00 00 */	li r3, 0
/* 80081F84 0007EEC4  3C 80 80 3F */	lis r4, taskwork@ha
/* 80081F88 0007EEC8  38 00 00 10 */	li r0, 0x10
/* 80081F8C 0007EECC  38 C3 00 00 */	addi r6, r3, 0
/* 80081F90 0007EED0  38 A4 8E 40 */	addi r5, r4, taskwork@l
/* 80081F94 0007EED4  7C 09 03 A6 */	mtctr r0
lbl_80081F98:
/* 80081F98 0007EED8  7C 85 1A 14 */	add r4, r5, r3
/* 80081F9C 0007EEDC  38 63 00 08 */	addi r3, r3, 8
/* 80081FA0 0007EEE0  90 C4 00 04 */	stw r6, 4(r4)
/* 80081FA4 0007EEE4  42 00 FF F4 */	bdnz lbl_80081F98
/* 80081FA8 0007EEE8  4E 80 00 20 */	blr 
/* 80081FAC 0007EEEC  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081FB0 0007EEF0  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081FB4 0007EEF4  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081FB8 0007EEF8  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80081FBC 0007EEFC  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */

.global DspStartWork__FUlPFUs_v
DspStartWork__FUlPFUs_v:
/* 80081FC0 0007EF00  81 0D 92 6C */	lwz r8, taskwritep@sda21(r13)
/* 80081FC4 0007EF04  80 0D 92 68 */	lwz r0, taskreadp@sda21(r13)
/* 80081FC8 0007EF08  38 A8 00 01 */	addi r5, r8, 1
/* 80081FCC 0007EF0C  54 A7 07 3E */	clrlwi r7, r5, 0x1c
/* 80081FD0 0007EF10  39 25 00 00 */	addi r9, r5, 0
/* 80081FD4 0007EF14  7C 07 00 40 */	cmplw r7, r0
/* 80081FD8 0007EF18  40 82 00 0C */	bne lbl_80081FE4
/* 80081FDC 0007EF1C  38 60 00 00 */	li r3, 0
/* 80081FE0 0007EF20  4E 80 00 20 */	blr 
lbl_80081FE4:
/* 80081FE4 0007EF24  3C A0 80 3F */	lis r5, taskwork@ha
/* 80081FE8 0007EF28  54 66 84 3E */	srwi r6, r3, 0x10
/* 80081FEC 0007EF2C  55 03 18 38 */	slwi r3, r8, 3
/* 80081FF0 0007EF30  38 05 8E 40 */	addi r0, r5, taskwork@l
/* 80081FF4 0007EF34  7C A0 1A 14 */	add r5, r0, r3
/* 80081FF8 0007EF38  38 69 00 00 */	addi r3, r9, 0
/* 80081FFC 0007EF3C  B0 C5 00 00 */	sth r6, 0(r5)
/* 80082000 0007EF40  90 85 00 04 */	stw r4, 4(r5)
/* 80082004 0007EF44  90 ED 92 6C */	stw r7, taskwritep@sda21(r13)
/* 80082008 0007EF48  4E 80 00 20 */	blr 
/* 8008200C 0007EF4C  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80082010 0007EF50  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80082014 0007EF54  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80082018 0007EF58  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 8008201C 0007EF5C  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */

.global DspFinishWork__FUs
DspFinishWork__FUs:
/* 80082020 0007EF60  7C 08 02 A6 */	mflr r0
/* 80082024 0007EF64  3C 80 80 3F */	lis r4, taskwork@ha
/* 80082028 0007EF68  90 01 00 04 */	stw r0, 4(r1)
/* 8008202C 0007EF6C  38 04 8E 40 */	addi r0, r4, taskwork@l
/* 80082030 0007EF70  54 64 04 3E */	clrlwi r4, r3, 0x10
/* 80082034 0007EF74  94 21 FF F8 */	stwu r1, -8(r1)
/* 80082038 0007EF78  80 AD 92 68 */	lwz r5, taskreadp@sda21(r13)
/* 8008203C 0007EF7C  54 A3 18 38 */	slwi r3, r5, 3
/* 80082040 0007EF80  7C 60 1A 14 */	add r3, r0, r3
/* 80082044 0007EF84  A0 03 00 00 */	lhz r0, 0(r3)
/* 80082048 0007EF88  7C 04 00 40 */	cmplw r4, r0
/* 8008204C 0007EF8C  40 82 00 2C */	bne lbl_80082078
/* 80082050 0007EF90  81 83 00 04 */	lwz r12, 4(r3)
/* 80082054 0007EF94  28 0C 00 00 */	cmplwi r12, 0
/* 80082058 0007EF98  41 82 00 10 */	beq lbl_80082068
/* 8008205C 0007EF9C  54 A3 04 3E */	clrlwi r3, r5, 0x10
/* 80082060 0007EFA0  7D 88 03 A6 */	mtlr r12
/* 80082064 0007EFA4  4E 80 00 21 */	blrl 
lbl_80082068:
/* 80082068 0007EFA8  80 6D 92 68 */	lwz r3, taskreadp@sda21(r13)
/* 8008206C 0007EFAC  38 03 00 01 */	addi r0, r3, 1
/* 80082070 0007EFB0  54 00 07 3E */	clrlwi r0, r0, 0x1c
/* 80082074 0007EFB4  90 0D 92 68 */	stw r0, taskreadp@sda21(r13)
lbl_80082078:
/* 80082078 0007EFB8  80 01 00 0C */	lwz r0, 0xc(r1)
/* 8008207C 0007EFBC  38 21 00 08 */	addi r1, r1, 8
/* 80082080 0007EFC0  7C 08 03 A6 */	mtlr r0
/* 80082084 0007EFC4  4E 80 00 20 */	blr 
/* 80082088 0007EFC8  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 8008208C 0007EFCC  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80082090 0007EFD0  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80082094 0007EFD4  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 80082098 0007EFD8  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */
/* 8008209C 0007EFDC  00 00 00 00 */	.4byte 0x00000000  /* unknown instruction */

.section .data, "wa"  # 0x803A8380 - 0x803E6000
jdsp:
	.incbin "baserom.dol", 0x3A9F20, 0x1CE0

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
audio_task:
	.skip 0x60
AUDIO_YIELD_BUFFER:
	.skip 0x2000
taskwork:
	.skip 0x80

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
taskreadp:
	.skip 0x4
taskwritep:
	.skip 0x4

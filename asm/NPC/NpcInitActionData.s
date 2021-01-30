.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global setKinoActionFlag___8TBaseNPCFv
setKinoActionFlag___8TBaseNPCFv:
/* 8017AA34 00177974  80 03 01 6C */	lwz r0, 0x16c(r3)
/* 8017AA38 00177978  2C 00 00 0F */	cmpwi r0, 0xf
/* 8017AA3C 0017797C  40 81 00 0C */	ble lbl_8017AA48
/* 8017AA40 00177980  38 00 00 00 */	li r0, 0
/* 8017AA44 00177984  90 03 01 6C */	stw r0, 0x16c(r3)
lbl_8017AA48:
/* 8017AA48 00177988  80 A3 01 6C */	lwz r5, 0x16c(r3)
/* 8017AA4C 0017798C  3C 80 80 38 */	lis r4, sBaseActionFlagTable$1944@ha
/* 8017AA50 00177990  38 04 6F 18 */	addi r0, r4, sBaseActionFlagTable$1944@l
/* 8017AA54 00177994  54 A4 10 3A */	slwi r4, r5, 2
/* 8017AA58 00177998  7C 80 22 14 */	add r4, r0, r4
/* 8017AA5C 0017799C  80 04 00 00 */	lwz r0, 0(r4)
/* 8017AA60 001779A0  90 03 01 70 */	stw r0, 0x170(r3)
/* 8017AA64 001779A4  4E 80 00 20 */	blr 

.global setMareActionFlag___8TBaseNPCFv
setMareActionFlag___8TBaseNPCFv:
/* 8017AA68 001779A8  80 03 01 6C */	lwz r0, 0x16c(r3)
/* 8017AA6C 001779AC  2C 00 00 10 */	cmpwi r0, 0x10
/* 8017AA70 001779B0  40 81 00 0C */	ble lbl_8017AA7C
/* 8017AA74 001779B4  38 00 00 00 */	li r0, 0
/* 8017AA78 001779B8  90 03 01 6C */	stw r0, 0x16c(r3)
lbl_8017AA7C:
/* 8017AA7C 001779BC  80 A3 01 6C */	lwz r5, 0x16c(r3)
/* 8017AA80 001779C0  3C 80 80 38 */	lis r4, sBaseActionFlagTable$1941@ha
/* 8017AA84 001779C4  38 04 6E D4 */	addi r0, r4, sBaseActionFlagTable$1941@l
/* 8017AA88 001779C8  54 A4 10 3A */	slwi r4, r5, 2
/* 8017AA8C 001779CC  7C 80 22 14 */	add r4, r0, r4
/* 8017AA90 001779D0  80 04 00 00 */	lwz r0, 0(r4)
/* 8017AA94 001779D4  90 03 01 70 */	stw r0, 0x170(r3)
/* 8017AA98 001779D8  4E 80 00 20 */	blr 

.global setMonteActionFlag___8TBaseNPCFv
setMonteActionFlag___8TBaseNPCFv:
/* 8017AA9C 001779DC  80 03 01 6C */	lwz r0, 0x16c(r3)
/* 8017AAA0 001779E0  2C 00 00 18 */	cmpwi r0, 0x18
/* 8017AAA4 001779E4  40 81 00 0C */	ble lbl_8017AAB0
/* 8017AAA8 001779E8  38 00 00 00 */	li r0, 0
/* 8017AAAC 001779EC  90 03 01 6C */	stw r0, 0x16c(r3)
lbl_8017AAB0:
/* 8017AAB0 001779F0  80 A3 01 6C */	lwz r5, 0x16c(r3)
/* 8017AAB4 001779F4  3C 80 80 38 */	lis r4, sBaseActionFlagTable$1938@ha
/* 8017AAB8 001779F8  38 04 6E 70 */	addi r0, r4, sBaseActionFlagTable$1938@l
/* 8017AABC 001779FC  54 A4 10 3A */	slwi r4, r5, 2
/* 8017AAC0 00177A00  7C 80 22 14 */	add r4, r0, r4
/* 8017AAC4 00177A04  80 04 00 00 */	lwz r0, 0(r4)
/* 8017AAC8 00177A08  90 03 01 70 */	stw r0, 0x170(r3)
/* 8017AACC 00177A0C  4E 80 00 20 */	blr 

.section .rodata, "wa"  # 0x8036FFA0 - 0x803A8380
.global sBaseActionFlagTable$1938
sBaseActionFlagTable$1938:
	.incbin "baserom.dol", 0x383E70, 0x64
.global sBaseActionFlagTable$1941
sBaseActionFlagTable$1941:
	.incbin "baserom.dol", 0x383ED4, 0x44
.global sBaseActionFlagTable$1944
sBaseActionFlagTable$1944:
	.incbin "baserom.dol", 0x383F18, 0x40

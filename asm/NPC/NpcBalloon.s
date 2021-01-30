.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global updateBalloon__11TNpcBalloonFv
updateBalloon__11TNpcBalloonFv:
/* 8017B254 00178194  7C 08 02 A6 */	mflr r0
/* 8017B258 00178198  90 01 00 04 */	stw r0, 4(r1)
/* 8017B25C 0017819C  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 8017B260 001781A0  93 E1 00 24 */	stw r31, 0x24(r1)
/* 8017B264 001781A4  7C 7F 1B 78 */	mr r31, r3
/* 8017B268 001781A8  80 83 00 04 */	lwz r4, 4(r3)
/* 8017B26C 001781AC  38 60 00 00 */	li r3, 0
/* 8017B270 001781B0  2C 04 00 00 */	cmpwi r4, 0
/* 8017B274 001781B4  40 81 00 38 */	ble lbl_8017B2AC
/* 8017B278 001781B8  38 04 FF FF */	addi r0, r4, -1
/* 8017B27C 001781BC  90 1F 00 04 */	stw r0, 4(r31)
/* 8017B280 001781C0  80 1F 00 04 */	lwz r0, 4(r31)
/* 8017B284 001781C4  2C 00 00 00 */	cmpwi r0, 0
/* 8017B288 001781C8  40 82 00 24 */	bne lbl_8017B2AC
/* 8017B28C 001781CC  80 6D 97 E8 */	lwz r3, gpMarDirector-_SDA_BASE_(r13)
/* 8017B290 001781D0  38 A0 00 01 */	li r5, 1
/* 8017B294 001781D4  80 9F 00 00 */	lwz r4, 0(r31)
/* 8017B298 001781D8  80 63 00 74 */	lwz r3, 0x74(r3)
/* 8017B29C 001781DC  48 09 03 A9 */	bl startAppearBalloon__11TGCConsole2FUlb
/* 8017B2A0 001781E0  38 00 FF FF */	li r0, -1
/* 8017B2A4 001781E4  90 1F 00 04 */	stw r0, 4(r31)
/* 8017B2A8 001781E8  38 60 00 01 */	li r3, 1
lbl_8017B2AC:
/* 8017B2AC 001781EC  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 8017B2B0 001781F0  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 8017B2B4 001781F4  38 21 00 28 */	addi r1, r1, 0x28
/* 8017B2B8 001781F8  7C 08 03 A6 */	mtlr r0
/* 8017B2BC 001781FC  4E 80 00 20 */	blr 

.global setNextMessage__11TNpcBalloonFUll
setNextMessage__11TNpcBalloonFUll:
/* 8017B2C0 00178200  90 83 00 00 */	stw r4, 0(r3)
/* 8017B2C4 00178204  90 A3 00 04 */	stw r5, 4(r3)
/* 8017B2C8 00178208  4E 80 00 20 */	blr 

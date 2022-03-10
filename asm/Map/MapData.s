.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __ct__12TBGCheckDataFv
__ct__12TBGCheckDataFv:
/* 801F4BC8 001F1B08  38 00 00 00 */	li r0, 0
/* 801F4BCC 001F1B0C  B0 03 00 00 */	sth r0, 0(r3)
/* 801F4BD0 001F1B10  B0 03 00 02 */	sth r0, 2(r3)
/* 801F4BD4 001F1B14  B0 03 00 04 */	sth r0, 4(r3)
/* 801F4BD8 001F1B18  C0 02 D1 28 */	lfs f0, $$22674@sda21(r2)
/* 801F4BDC 001F1B1C  D0 03 00 08 */	stfs f0, 8(r3)
/* 801F4BE0 001F1B20  D0 03 00 0C */	stfs f0, 0xc(r3)
/* 801F4BE4 001F1B24  D0 03 00 40 */	stfs f0, 0x40(r3)
/* 801F4BE8 001F1B28  90 03 00 44 */	stw r0, 0x44(r3)
/* 801F4BEC 001F1B2C  D0 03 00 18 */	stfs f0, 0x18(r3)
/* 801F4BF0 001F1B30  D0 03 00 14 */	stfs f0, 0x14(r3)
/* 801F4BF4 001F1B34  D0 03 00 10 */	stfs f0, 0x10(r3)
/* 801F4BF8 001F1B38  D0 03 00 24 */	stfs f0, 0x24(r3)
/* 801F4BFC 001F1B3C  D0 03 00 20 */	stfs f0, 0x20(r3)
/* 801F4C00 001F1B40  D0 03 00 1C */	stfs f0, 0x1c(r3)
/* 801F4C04 001F1B44  D0 03 00 30 */	stfs f0, 0x30(r3)
/* 801F4C08 001F1B48  D0 03 00 2C */	stfs f0, 0x2c(r3)
/* 801F4C0C 001F1B4C  D0 03 00 28 */	stfs f0, 0x28(r3)
/* 801F4C10 001F1B50  D0 03 00 3C */	stfs f0, 0x3c(r3)
/* 801F4C14 001F1B54  D0 03 00 38 */	stfs f0, 0x38(r3)
/* 801F4C18 001F1B58  D0 03 00 34 */	stfs f0, 0x34(r3)
/* 801F4C1C 001F1B5C  4E 80 00 20 */	blr 

.global getPlaneType__12TBGCheckDataFv
getPlaneType__12TBGCheckDataFv:
/* 801F4C20 001F1B60  A0 03 00 00 */	lhz r0, 0(r3)
/* 801F4C24 001F1B64  28 00 08 01 */	cmplwi r0, 0x801
/* 801F4C28 001F1B68  40 82 00 0C */	bne lbl_801F4C34
/* 801F4C2C 001F1B6C  38 00 00 01 */	li r0, 1
/* 801F4C30 001F1B70  48 00 00 08 */	b lbl_801F4C38
lbl_801F4C34:
/* 801F4C34 001F1B74  38 00 00 00 */	li r0, 0
lbl_801F4C38:
/* 801F4C38 001F1B78  54 00 06 3F */	clrlwi. r0, r0, 0x18
/* 801F4C3C 001F1B7C  41 82 00 0C */	beq lbl_801F4C48
/* 801F4C40 001F1B80  38 60 00 00 */	li r3, 0
/* 801F4C44 001F1B84  4E 80 00 20 */	blr 
lbl_801F4C48:
/* 801F4C48 001F1B88  C0 23 00 38 */	lfs f1, 0x38(r3)
/* 801F4C4C 001F1B8C  C0 02 D1 2C */	lfs f0, $$22693@sda21(r2)
/* 801F4C50 001F1B90  FC 01 00 40 */	fcmpo cr0, f1, f0
/* 801F4C54 001F1B94  40 81 00 0C */	ble lbl_801F4C60
/* 801F4C58 001F1B98  38 60 00 00 */	li r3, 0
/* 801F4C5C 001F1B9C  4E 80 00 20 */	blr 
lbl_801F4C60:
/* 801F4C60 001F1BA0  C0 02 D1 30 */	lfs f0, $$22694@sda21(r2)
/* 801F4C64 001F1BA4  FC 01 00 40 */	fcmpo cr0, f1, f0
/* 801F4C68 001F1BA8  40 80 00 0C */	bge lbl_801F4C74
/* 801F4C6C 001F1BAC  38 60 00 01 */	li r3, 1
/* 801F4C70 001F1BB0  4E 80 00 20 */	blr 
lbl_801F4C74:
/* 801F4C74 001F1BB4  C0 23 00 34 */	lfs f1, 0x34(r3)
/* 801F4C78 001F1BB8  C0 02 D1 34 */	lfs f0, $$22695@sda21(r2)
/* 801F4C7C 001F1BBC  FC 01 00 40 */	fcmpo cr0, f1, f0
/* 801F4C80 001F1BC0  41 80 00 10 */	blt lbl_801F4C90
/* 801F4C84 001F1BC4  C0 02 D1 38 */	lfs f0, $$22696@sda21(r2)
/* 801F4C88 001F1BC8  FC 00 08 40 */	fcmpo cr0, f0, f1
/* 801F4C8C 001F1BCC  40 80 00 14 */	bge lbl_801F4CA0
lbl_801F4C90:
/* 801F4C90 001F1BD0  A0 03 00 04 */	lhz r0, 4(r3)
/* 801F4C94 001F1BD4  60 00 00 08 */	ori r0, r0, 8
/* 801F4C98 001F1BD8  B0 03 00 04 */	sth r0, 4(r3)
/* 801F4C9C 001F1BDC  48 00 00 10 */	b lbl_801F4CAC
lbl_801F4CA0:
/* 801F4CA0 001F1BE0  A0 03 00 04 */	lhz r0, 4(r3)
/* 801F4CA4 001F1BE4  54 00 07 76 */	rlwinm r0, r0, 0, 0x1d, 0x1b
/* 801F4CA8 001F1BE8  B0 03 00 04 */	sth r0, 4(r3)
lbl_801F4CAC:
/* 801F4CAC 001F1BEC  38 60 00 02 */	li r3, 2
/* 801F4CB0 001F1BF0  4E 80 00 20 */	blr 

.global getActiveJumpPower__12TBGCheckDataCFv
getActiveJumpPower__12TBGCheckDataCFv:
/* 801F4CB4 001F1BF4  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 801F4CB8 001F1BF8  80 83 00 44 */	lwz r4, 0x44(r3)
/* 801F4CBC 001F1BFC  28 04 00 00 */	cmplwi r4, 0
/* 801F4CC0 001F1C00  41 82 00 30 */	beq lbl_801F4CF0
/* 801F4CC4 001F1C04  80 84 00 4C */	lwz r4, 0x4c(r4)
/* 801F4CC8 001F1C08  3C 04 C0 00 */	addis r0, r4, 0xc000
/* 801F4CCC 001F1C0C  28 00 00 39 */	cmplwi r0, 0x39
/* 801F4CD0 001F1C10  40 82 00 0C */	bne lbl_801F4CDC
/* 801F4CD4 001F1C14  38 00 00 01 */	li r0, 1
/* 801F4CD8 001F1C18  48 00 00 08 */	b lbl_801F4CE0
lbl_801F4CDC:
/* 801F4CDC 001F1C1C  38 00 00 00 */	li r0, 0
lbl_801F4CE0:
/* 801F4CE0 001F1C20  54 00 06 3F */	clrlwi. r0, r0, 0x18
/* 801F4CE4 001F1C24  41 82 00 0C */	beq lbl_801F4CF0
/* 801F4CE8 001F1C28  C0 2D 87 98 */	lfs f1, mBananaTreeJumpPower__11TMapObjTree@sda21(r13)
/* 801F4CEC 001F1C2C  48 00 00 24 */	b lbl_801F4D10
lbl_801F4CF0:
/* 801F4CF0 001F1C30  A8 63 00 02 */	lha r3, 2(r3)
/* 801F4CF4 001F1C34  3C 00 43 30 */	lis r0, 0x4330
/* 801F4CF8 001F1C38  C8 22 D1 40 */	lfd f1, $$22708@sda21(r2)
/* 801F4CFC 001F1C3C  6C 63 80 00 */	xoris r3, r3, 0x8000
/* 801F4D00 001F1C40  90 61 00 24 */	stw r3, 0x24(r1)
/* 801F4D04 001F1C44  90 01 00 20 */	stw r0, 0x20(r1)
/* 801F4D08 001F1C48  C8 01 00 20 */	lfd f0, 0x20(r1)
/* 801F4D0C 001F1C4C  EC 20 08 28 */	fsubs f1, f0, f1
lbl_801F4D10:
/* 801F4D10 001F1C50  38 21 00 28 */	addi r1, r1, 0x28
/* 801F4D14 001F1C54  4E 80 00 20 */	blr 

.global __sinit_MapData_cpp
__sinit_MapData_cpp:
/* 801F4D18 001F1C58  7C 08 02 A6 */	mflr r0
/* 801F4D1C 001F1C5C  3C 60 80 40 */	lis r3, $$22650@ha
/* 801F4D20 001F1C60  90 01 00 04 */	stw r0, 4(r1)
/* 801F4D24 001F1C64  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 801F4D28 001F1C68  93 E1 00 0C */	stw r31, 0xc(r1)
/* 801F4D2C 001F1C6C  3B E3 8A 58 */	addi r31, r3, $$22650@l
/* 801F4D30 001F1C70  88 0D 97 CC */	lbz r0, __init__smList__15JALList$$05MSBgm$$1@sda21(r13)
/* 801F4D34 001F1C74  7C 00 07 75 */	extsb. r0, r0
/* 801F4D38 001F1C78  40 82 00 28 */	bne lbl_801F4D60
/* 801F4D3C 001F1C7C  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1@sda21
/* 801F4D40 001F1C80  4B E1 9E F5 */	bl initiate__10JSUPtrListFv
/* 801F4D44 001F1C84  3C 60 80 0E */	lis r3, __dt__15JSUList$$05MSBgm$$1Fv@ha
/* 801F4D48 001F1C88  38 83 6A 44 */	addi r4, r3, __dt__15JSUList$$05MSBgm$$1Fv@l
/* 801F4D4C 001F1C8C  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1@sda21
/* 801F4D50 001F1C90  38 BF 00 00 */	addi r5, r31, 0
/* 801F4D54 001F1C94  4B E8 D9 D5 */	bl __register_global_object
/* 801F4D58 001F1C98  38 00 00 01 */	li r0, 1
/* 801F4D5C 001F1C9C  98 0D 97 CC */	stb r0, __init__smList__15JALList$$05MSBgm$$1@sda21(r13)
lbl_801F4D60:
/* 801F4D60 001F1CA0  88 0D 97 CD */	lbz r0, __init__smList__24JALList$$013MSSetSoundGrp$$1@sda21(r13)
/* 801F4D64 001F1CA4  7C 00 07 75 */	extsb. r0, r0
/* 801F4D68 001F1CA8  40 82 00 28 */	bne lbl_801F4D90
/* 801F4D6C 001F1CAC  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1@sda21
/* 801F4D70 001F1CB0  4B E1 9E C5 */	bl initiate__10JSUPtrListFv
/* 801F4D74 001F1CB4  3C 60 80 0E */	lis r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@ha
/* 801F4D78 001F1CB8  38 83 69 EC */	addi r4, r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@l
/* 801F4D7C 001F1CBC  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1@sda21
/* 801F4D80 001F1CC0  38 BF 00 0C */	addi r5, r31, 0xc
/* 801F4D84 001F1CC4  4B E8 D9 A5 */	bl __register_global_object
/* 801F4D88 001F1CC8  38 00 00 01 */	li r0, 1
/* 801F4D8C 001F1CCC  98 0D 97 CD */	stb r0, __init__smList__24JALList$$013MSSetSoundGrp$$1@sda21(r13)
lbl_801F4D90:
/* 801F4D90 001F1CD0  88 0D 97 CE */	lbz r0, __init__smList__21JALList$$010MSSetSound$$1@sda21(r13)
/* 801F4D94 001F1CD4  7C 00 07 75 */	extsb. r0, r0
/* 801F4D98 001F1CD8  40 82 00 28 */	bne lbl_801F4DC0
/* 801F4D9C 001F1CDC  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1@sda21
/* 801F4DA0 001F1CE0  4B E1 9E 95 */	bl initiate__10JSUPtrListFv
/* 801F4DA4 001F1CE4  3C 60 80 0E */	lis r3, __dt__21JSUList$$010MSSetSound$$1Fv@ha
/* 801F4DA8 001F1CE8  38 83 69 94 */	addi r4, r3, __dt__21JSUList$$010MSSetSound$$1Fv@l
/* 801F4DAC 001F1CEC  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1@sda21
/* 801F4DB0 001F1CF0  38 BF 00 18 */	addi r5, r31, 0x18
/* 801F4DB4 001F1CF4  4B E8 D9 75 */	bl __register_global_object
/* 801F4DB8 001F1CF8  38 00 00 01 */	li r0, 1
/* 801F4DBC 001F1CFC  98 0D 97 CE */	stb r0, __init__smList__21JALList$$010MSSetSound$$1@sda21(r13)
lbl_801F4DC0:
/* 801F4DC0 001F1D00  88 0D 8F 8C */	lbz r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1@sda21(r13)
/* 801F4DC4 001F1D04  7C 00 07 75 */	extsb. r0, r0
/* 801F4DC8 001F1D08  40 82 00 28 */	bne lbl_801F4DF0
/* 801F4DCC 001F1D0C  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1@sda21
/* 801F4DD0 001F1D10  4B E1 9E 65 */	bl initiate__10JSUPtrListFv
/* 801F4DD4 001F1D14  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@ha
/* 801F4DD8 001F1D18  38 83 A8 0C */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@l
/* 801F4DDC 001F1D1C  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1@sda21
/* 801F4DE0 001F1D20  38 BF 00 24 */	addi r5, r31, 0x24
/* 801F4DE4 001F1D24  4B E8 D9 45 */	bl __register_global_object
/* 801F4DE8 001F1D28  38 00 00 01 */	li r0, 1
/* 801F4DEC 001F1D2C  98 0D 8F 8C */	stb r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1@sda21(r13)
lbl_801F4DF0:
/* 801F4DF0 001F1D30  88 0D 8F 8D */	lbz r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1@sda21(r13)
/* 801F4DF4 001F1D34  7C 00 07 75 */	extsb. r0, r0
/* 801F4DF8 001F1D38  40 82 00 28 */	bne lbl_801F4E20
/* 801F4DFC 001F1D3C  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1@sda21
/* 801F4E00 001F1D40  4B E1 9E 35 */	bl initiate__10JSUPtrListFv
/* 801F4E04 001F1D44  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@ha
/* 801F4E08 001F1D48  38 83 A7 B4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@l
/* 801F4E0C 001F1D4C  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1@sda21
/* 801F4E10 001F1D50  38 BF 00 30 */	addi r5, r31, 0x30
/* 801F4E14 001F1D54  4B E8 D9 15 */	bl __register_global_object
/* 801F4E18 001F1D58  38 00 00 01 */	li r0, 1
/* 801F4E1C 001F1D5C  98 0D 8F 8D */	stb r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1@sda21(r13)
lbl_801F4E20:
/* 801F4E20 001F1D60  88 0D 8F 8E */	lbz r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1@sda21(r13)
/* 801F4E24 001F1D64  7C 00 07 75 */	extsb. r0, r0
/* 801F4E28 001F1D68  40 82 00 28 */	bne lbl_801F4E50
/* 801F4E2C 001F1D6C  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1@sda21
/* 801F4E30 001F1D70  4B E1 9E 05 */	bl initiate__10JSUPtrListFv
/* 801F4E34 001F1D74  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@ha
/* 801F4E38 001F1D78  38 83 A7 5C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@l
/* 801F4E3C 001F1D7C  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1@sda21
/* 801F4E40 001F1D80  38 BF 00 3C */	addi r5, r31, 0x3c
/* 801F4E44 001F1D84  4B E8 D8 E5 */	bl __register_global_object
/* 801F4E48 001F1D88  38 00 00 01 */	li r0, 1
/* 801F4E4C 001F1D8C  98 0D 8F 8E */	stb r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1@sda21(r13)
lbl_801F4E50:
/* 801F4E50 001F1D90  88 0D 8F 8F */	lbz r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1@sda21(r13)
/* 801F4E54 001F1D94  7C 00 07 75 */	extsb. r0, r0
/* 801F4E58 001F1D98  40 82 00 28 */	bne lbl_801F4E80
/* 801F4E5C 001F1D9C  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1@sda21
/* 801F4E60 001F1DA0  4B E1 9D D5 */	bl initiate__10JSUPtrListFv
/* 801F4E64 001F1DA4  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@ha
/* 801F4E68 001F1DA8  38 83 A7 04 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@l
/* 801F4E6C 001F1DAC  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1@sda21
/* 801F4E70 001F1DB0  38 BF 00 48 */	addi r5, r31, 0x48
/* 801F4E74 001F1DB4  4B E8 D8 B5 */	bl __register_global_object
/* 801F4E78 001F1DB8  38 00 00 01 */	li r0, 1
/* 801F4E7C 001F1DBC  98 0D 8F 8F */	stb r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1@sda21(r13)
lbl_801F4E80:
/* 801F4E80 001F1DC0  88 0D 8F 90 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1@sda21(r13)
/* 801F4E84 001F1DC4  7C 00 07 75 */	extsb. r0, r0
/* 801F4E88 001F1DC8  40 82 00 28 */	bne lbl_801F4EB0
/* 801F4E8C 001F1DCC  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1@sda21
/* 801F4E90 001F1DD0  4B E1 9D A5 */	bl initiate__10JSUPtrListFv
/* 801F4E94 001F1DD4  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@ha
/* 801F4E98 001F1DD8  38 83 A6 AC */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@l
/* 801F4E9C 001F1DDC  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1@sda21
/* 801F4EA0 001F1DE0  38 BF 00 54 */	addi r5, r31, 0x54
/* 801F4EA4 001F1DE4  4B E8 D8 85 */	bl __register_global_object
/* 801F4EA8 001F1DE8  38 00 00 01 */	li r0, 1
/* 801F4EAC 001F1DEC  98 0D 8F 90 */	stb r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1@sda21(r13)
lbl_801F4EB0:
/* 801F4EB0 001F1DF0  88 0D 8F 91 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1@sda21(r13)
/* 801F4EB4 001F1DF4  7C 00 07 75 */	extsb. r0, r0
/* 801F4EB8 001F1DF8  40 82 00 28 */	bne lbl_801F4EE0
/* 801F4EBC 001F1DFC  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1@sda21
/* 801F4EC0 001F1E00  4B E1 9D 75 */	bl initiate__10JSUPtrListFv
/* 801F4EC4 001F1E04  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@ha
/* 801F4EC8 001F1E08  38 83 A6 54 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@l
/* 801F4ECC 001F1E0C  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1@sda21
/* 801F4ED0 001F1E10  38 BF 00 60 */	addi r5, r31, 0x60
/* 801F4ED4 001F1E14  4B E8 D8 55 */	bl __register_global_object
/* 801F4ED8 001F1E18  38 00 00 01 */	li r0, 1
/* 801F4EDC 001F1E1C  98 0D 8F 91 */	stb r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1@sda21(r13)
lbl_801F4EE0:
/* 801F4EE0 001F1E20  88 0D 8F 92 */	lbz r0, __init__smList__26JALList$$015JALSeModEffDist$$1@sda21(r13)
/* 801F4EE4 001F1E24  7C 00 07 75 */	extsb. r0, r0
/* 801F4EE8 001F1E28  40 82 00 28 */	bne lbl_801F4F10
/* 801F4EEC 001F1E2C  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1@sda21
/* 801F4EF0 001F1E30  4B E1 9D 45 */	bl initiate__10JSUPtrListFv
/* 801F4EF4 001F1E34  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@ha
/* 801F4EF8 001F1E38  38 83 A5 FC */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@l
/* 801F4EFC 001F1E3C  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1@sda21
/* 801F4F00 001F1E40  38 BF 00 6C */	addi r5, r31, 0x6c
/* 801F4F04 001F1E44  4B E8 D8 25 */	bl __register_global_object
/* 801F4F08 001F1E48  38 00 00 01 */	li r0, 1
/* 801F4F0C 001F1E4C  98 0D 8F 92 */	stb r0, __init__smList__26JALList$$015JALSeModEffDist$$1@sda21(r13)
lbl_801F4F10:
/* 801F4F10 001F1E50  88 0D 8F 93 */	lbz r0, __init__smList__26JALList$$015JALSeModPitDist$$1@sda21(r13)
/* 801F4F14 001F1E54  7C 00 07 75 */	extsb. r0, r0
/* 801F4F18 001F1E58  40 82 00 28 */	bne lbl_801F4F40
/* 801F4F1C 001F1E5C  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1@sda21
/* 801F4F20 001F1E60  4B E1 9D 15 */	bl initiate__10JSUPtrListFv
/* 801F4F24 001F1E64  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@ha
/* 801F4F28 001F1E68  38 83 A5 A4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@l
/* 801F4F2C 001F1E6C  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1@sda21
/* 801F4F30 001F1E70  38 BF 00 78 */	addi r5, r31, 0x78
/* 801F4F34 001F1E74  4B E8 D7 F5 */	bl __register_global_object
/* 801F4F38 001F1E78  38 00 00 01 */	li r0, 1
/* 801F4F3C 001F1E7C  98 0D 8F 93 */	stb r0, __init__smList__26JALList$$015JALSeModPitDist$$1@sda21(r13)
lbl_801F4F40:
/* 801F4F40 001F1E80  88 0D 8F 94 */	lbz r0, __init__smList__26JALList$$015JALSeModVolDist$$1@sda21(r13)
/* 801F4F44 001F1E84  7C 00 07 75 */	extsb. r0, r0
/* 801F4F48 001F1E88  40 82 00 28 */	bne lbl_801F4F70
/* 801F4F4C 001F1E8C  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1@sda21
/* 801F4F50 001F1E90  4B E1 9C E5 */	bl initiate__10JSUPtrListFv
/* 801F4F54 001F1E94  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@ha
/* 801F4F58 001F1E98  38 83 A5 4C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@l
/* 801F4F5C 001F1E9C  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1@sda21
/* 801F4F60 001F1EA0  38 BF 00 84 */	addi r5, r31, 0x84
/* 801F4F64 001F1EA4  4B E8 D7 C5 */	bl __register_global_object
/* 801F4F68 001F1EA8  38 00 00 01 */	li r0, 1
/* 801F4F6C 001F1EAC  98 0D 8F 94 */	stb r0, __init__smList__26JALList$$015JALSeModVolDist$$1@sda21(r13)
lbl_801F4F70:
/* 801F4F70 001F1EB0  88 0D 8F 95 */	lbz r0, __init__smList__26JALList$$015JALSeModEffFunk$$1@sda21(r13)
/* 801F4F74 001F1EB4  7C 00 07 75 */	extsb. r0, r0
/* 801F4F78 001F1EB8  40 82 00 28 */	bne lbl_801F4FA0
/* 801F4F7C 001F1EBC  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1@sda21
/* 801F4F80 001F1EC0  4B E1 9C B5 */	bl initiate__10JSUPtrListFv
/* 801F4F84 001F1EC4  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@ha
/* 801F4F88 001F1EC8  38 83 A4 F4 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@l
/* 801F4F8C 001F1ECC  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1@sda21
/* 801F4F90 001F1ED0  38 BF 00 90 */	addi r5, r31, 0x90
/* 801F4F94 001F1ED4  4B E8 D7 95 */	bl __register_global_object
/* 801F4F98 001F1ED8  38 00 00 01 */	li r0, 1
/* 801F4F9C 001F1EDC  98 0D 8F 95 */	stb r0, __init__smList__26JALList$$015JALSeModEffFunk$$1@sda21(r13)
lbl_801F4FA0:
/* 801F4FA0 001F1EE0  88 0D 8F 96 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFunk$$1@sda21(r13)
/* 801F4FA4 001F1EE4  7C 00 07 75 */	extsb. r0, r0
/* 801F4FA8 001F1EE8  40 82 00 28 */	bne lbl_801F4FD0
/* 801F4FAC 001F1EEC  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1@sda21
/* 801F4FB0 001F1EF0  4B E1 9C 85 */	bl initiate__10JSUPtrListFv
/* 801F4FB4 001F1EF4  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@ha
/* 801F4FB8 001F1EF8  38 83 A4 9C */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@l
/* 801F4FBC 001F1EFC  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1@sda21
/* 801F4FC0 001F1F00  38 BF 00 9C */	addi r5, r31, 0x9c
/* 801F4FC4 001F1F04  4B E8 D7 65 */	bl __register_global_object
/* 801F4FC8 001F1F08  38 00 00 01 */	li r0, 1
/* 801F4FCC 001F1F0C  98 0D 8F 96 */	stb r0, __init__smList__26JALList$$015JALSeModPitFunk$$1@sda21(r13)
lbl_801F4FD0:
/* 801F4FD0 001F1F10  88 0D 8F 97 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFunk$$1@sda21(r13)
/* 801F4FD4 001F1F14  7C 00 07 75 */	extsb. r0, r0
/* 801F4FD8 001F1F18  40 82 00 28 */	bne lbl_801F5000
/* 801F4FDC 001F1F1C  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1@sda21
/* 801F4FE0 001F1F20  4B E1 9C 55 */	bl initiate__10JSUPtrListFv
/* 801F4FE4 001F1F24  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@ha
/* 801F4FE8 001F1F28  38 83 A4 44 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@l
/* 801F4FEC 001F1F2C  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1@sda21
/* 801F4FF0 001F1F30  38 BF 00 A8 */	addi r5, r31, 0xa8
/* 801F4FF4 001F1F34  4B E8 D7 35 */	bl __register_global_object
/* 801F4FF8 001F1F38  38 00 00 01 */	li r0, 1
/* 801F4FFC 001F1F3C  98 0D 8F 97 */	stb r0, __init__smList__26JALList$$015JALSeModVolFunk$$1@sda21(r13)
lbl_801F5000:
/* 801F5000 001F1F40  80 01 00 14 */	lwz r0, 0x14(r1)
/* 801F5004 001F1F44  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 801F5008 001F1F48  38 21 00 10 */	addi r1, r1, 0x10
/* 801F500C 001F1F4C  7C 08 03 A6 */	mtlr r0
/* 801F5010 001F1F50  4E 80 00 20 */	blr 

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
lbl_constructor:
  .4byte __sinit_MapData_cpp

.section .sdata2, "a"  # 0x8040B460 - 0x80414020
.balign 8
$$22674:
  .4byte 0
$$22693:
	.incbin "baserom.dol", 0x3E8E2C, 0x4
$$22694:
	.incbin "baserom.dol", 0x3E8E30, 0x4
$$22695:
	.incbin "baserom.dol", 0x3E8E34, 0x4
$$22696:
	.incbin "baserom.dol", 0x3E8E38, 0x8
$$22708:
	.incbin "baserom.dol", 0x3E8E40, 0x8

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
$$22650:
	.skip 0xC
$$22651:
	.skip 0xC
$$22652:
	.skip 0xC
$$22653:
	.skip 0xC
$$22654:
	.skip 0xC
$$22655:
	.skip 0xC
$$22656:
	.skip 0xC
$$22657:
	.skip 0xC
$$22658:
	.skip 0xC
$$22659:
	.skip 0xC
$$22660:
	.skip 0xC
$$22661:
	.skip 0xC
$$22662:
	.skip 0xC
$$22663:
	.skip 0xC
$$22664:
	.skip 0x10

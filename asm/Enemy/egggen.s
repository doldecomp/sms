.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __dt__14TEggGenManagerFv
__dt__14TEggGenManagerFv:
/* 8030FCE4 0030CC24  7C 08 02 A6 */	mflr r0
/* 8030FCE8 0030CC28  90 01 00 04 */	stw r0, 4(r1)
/* 8030FCEC 0030CC2C  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8030FCF0 0030CC30  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8030FCF4 0030CC34  3B E4 00 00 */	addi r31, r4, 0
/* 8030FCF8 0030CC38  93 C1 00 10 */	stw r30, 0x10(r1)
/* 8030FCFC 0030CC3C  7C 7E 1B 79 */	or. r30, r3, r3
/* 8030FD00 0030CC40  41 82 00 2C */	beq lbl_8030FD2C
/* 8030FD04 0030CC44  3C 60 80 3E */	lis r3, __vt__14TEggGenManager@ha
/* 8030FD08 0030CC48  38 03 01 60 */	addi r0, r3, __vt__14TEggGenManager@l
/* 8030FD0C 0030CC4C  90 1E 00 00 */	stw r0, 0(r30)
/* 8030FD10 0030CC50  38 7E 00 00 */	addi r3, r30, 0
/* 8030FD14 0030CC54  38 80 00 00 */	li r4, 0
/* 8030FD18 0030CC58  4B F4 2C 91 */	bl __dt__13TEnemyManagerFv
/* 8030FD1C 0030CC5C  7F E0 07 35 */	extsh. r0, r31
/* 8030FD20 0030CC60  40 81 00 0C */	ble lbl_8030FD2C
/* 8030FD24 0030CC64  7F C3 F3 78 */	mr r3, r30
/* 8030FD28 0030CC68  4B CF CD 89 */	bl __dl__FPv
lbl_8030FD2C:
/* 8030FD2C 0030CC6C  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8030FD30 0030CC70  7F C3 F3 78 */	mr r3, r30
/* 8030FD34 0030CC74  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8030FD38 0030CC78  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 8030FD3C 0030CC7C  7C 08 03 A6 */	mtlr r0
/* 8030FD40 0030CC80  38 21 00 18 */	addi r1, r1, 0x18
/* 8030FD44 0030CC84  4E 80 00 20 */	blr 

.global load__14TEggGenManagerFR20JSUMemoryInputStream
load__14TEggGenManagerFR20JSUMemoryInputStream:
/* 8030FD48 0030CC88  7C 08 02 A6 */	mflr r0
/* 8030FD4C 0030CC8C  90 01 00 04 */	stw r0, 4(r1)
/* 8030FD50 0030CC90  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 8030FD54 0030CC94  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 8030FD58 0030CC98  3B E4 00 00 */	addi r31, r4, 0
/* 8030FD5C 0030CC9C  93 C1 00 18 */	stw r30, 0x18(r1)
/* 8030FD60 0030CCA0  3B C3 00 00 */	addi r30, r3, 0
/* 8030FD64 0030CCA4  38 60 00 A8 */	li r3, 0xa8
/* 8030FD68 0030CCA8  93 A1 00 14 */	stw r29, 0x14(r1)
/* 8030FD6C 0030CCAC  4B CF CB 45 */	bl __nw__FUl
/* 8030FD70 0030CCB0  7C 7D 1B 79 */	or. r29, r3, r3
/* 8030FD74 0030CCB4  41 82 00 14 */	beq lbl_8030FD88
/* 8030FD78 0030CCB8  3C 60 80 3A */	lis r3, "@2277"@ha
/* 8030FD7C 0030CCBC  38 83 11 5C */	addi r4, r3, "@2277"@l
/* 8030FD80 0030CCC0  38 7D 00 00 */	addi r3, r29, 0
/* 8030FD84 0030CCC4  4B F4 2E 99 */	bl __ct__17TSpineEnemyParamsFPCc
lbl_8030FD88:
/* 8030FD88 0030CCC8  93 BE 00 38 */	stw r29, 0x38(r30)
/* 8030FD8C 0030CCCC  38 7E 00 00 */	addi r3, r30, 0
/* 8030FD90 0030CCD0  38 9F 00 00 */	addi r4, r31, 0
/* 8030FD94 0030CCD4  4B F4 2A 71 */	bl load__13TEnemyManagerFR20JSUMemoryInputStream
/* 8030FD98 0030CCD8  80 01 00 24 */	lwz r0, 0x24(r1)
/* 8030FD9C 0030CCDC  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 8030FDA0 0030CCE0  83 C1 00 18 */	lwz r30, 0x18(r1)
/* 8030FDA4 0030CCE4  7C 08 03 A6 */	mtlr r0
/* 8030FDA8 0030CCE8  83 A1 00 14 */	lwz r29, 0x14(r1)
/* 8030FDAC 0030CCEC  38 21 00 20 */	addi r1, r1, 0x20
/* 8030FDB0 0030CCF0  4E 80 00 20 */	blr 

.global createModelData__14TEggGenManagerFv
createModelData__14TEggGenManagerFv:
/* 8030FDB4 0030CCF4  7C 08 02 A6 */	mflr r0
/* 8030FDB8 0030CCF8  3C 80 80 3A */	lis r4, entry$2266@ha
/* 8030FDBC 0030CCFC  90 01 00 04 */	stw r0, 4(r1)
/* 8030FDC0 0030CD00  38 84 11 44 */	addi r4, r4, entry$2266@l
/* 8030FDC4 0030CD04  94 21 FF F8 */	stwu r1, -8(r1)
/* 8030FDC8 0030CD08  81 83 00 00 */	lwz r12, 0(r3)
/* 8030FDCC 0030CD0C  81 8C 00 2C */	lwz r12, 0x2c(r12)
/* 8030FDD0 0030CD10  7D 88 03 A6 */	mtlr r12
/* 8030FDD4 0030CD14  4E 80 00 21 */	blrl 
/* 8030FDD8 0030CD18  80 01 00 0C */	lwz r0, 0xc(r1)
/* 8030FDDC 0030CD1C  38 21 00 08 */	addi r1, r1, 8
/* 8030FDE0 0030CD20  7C 08 03 A6 */	mtlr r0
/* 8030FDE4 0030CD24  4E 80 00 20 */	blr 

.global __ct__14TEggGenManagerFPCc
__ct__14TEggGenManagerFPCc:
/* 8030FDE8 0030CD28  7C 08 02 A6 */	mflr r0
/* 8030FDEC 0030CD2C  90 01 00 04 */	stw r0, 4(r1)
/* 8030FDF0 0030CD30  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8030FDF4 0030CD34  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8030FDF8 0030CD38  7C 7F 1B 78 */	mr r31, r3
/* 8030FDFC 0030CD3C  4B F4 2C 41 */	bl __ct__13TEnemyManagerFPCc
/* 8030FE00 0030CD40  3C 60 80 3E */	lis r3, __vt__14TEggGenManager@ha
/* 8030FE04 0030CD44  38 03 01 60 */	addi r0, r3, __vt__14TEggGenManager@l
/* 8030FE08 0030CD48  90 1F 00 00 */	stw r0, 0(r31)
/* 8030FE0C 0030CD4C  7F E3 FB 78 */	mr r3, r31
/* 8030FE10 0030CD50  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8030FE14 0030CD54  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8030FE18 0030CD58  38 21 00 18 */	addi r1, r1, 0x18
/* 8030FE1C 0030CD5C  7C 08 03 A6 */	mtlr r0
/* 8030FE20 0030CD60  4E 80 00 20 */	blr 

.global control__13TEggGeneratorFv
control__13TEggGeneratorFv:
/* 8030FE24 0030CD64  7C 08 02 A6 */	mflr r0
/* 8030FE28 0030CD68  90 01 00 04 */	stw r0, 4(r1)
/* 8030FE2C 0030CD6C  94 21 FF D0 */	stwu r1, -0x30(r1)
/* 8030FE30 0030CD70  93 E1 00 2C */	stw r31, 0x2c(r1)
/* 8030FE34 0030CD74  3B E3 00 00 */	addi r31, r3, 0
/* 8030FE38 0030CD78  38 7F 00 10 */	addi r3, r31, 0x10
/* 8030FE3C 0030CD7C  80 8D 98 B8 */	lwz r4, gpMarioOriginal@sda21(r13)
/* 8030FE40 0030CD80  38 84 00 10 */	addi r4, r4, 0x10
/* 8030FE44 0030CD84  4B D8 51 89 */	bl PSVECSquareDistance
/* 8030FE48 0030CD88  C0 02 FA 08 */	lfs f0, "@2291"@sda21(r2)
/* 8030FE4C 0030CD8C  FC 01 00 40 */	fcmpo cr0, f1, f0
/* 8030FE50 0030CD90  40 80 00 38 */	bge lbl_8030FE88
/* 8030FE54 0030CD94  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 8030FE58 0030CD98  80 63 03 F0 */	lwz r3, 0x3f0(r3)
/* 8030FE5C 0030CD9C  88 03 00 00 */	lbz r0, 0(r3)
/* 8030FE60 0030CDA0  28 00 00 00 */	cmplwi r0, 0
/* 8030FE64 0030CDA4  40 82 00 0C */	bne lbl_8030FE70
/* 8030FE68 0030CDA8  38 00 00 00 */	li r0, 0
/* 8030FE6C 0030CDAC  48 00 00 08 */	b lbl_8030FE74
lbl_8030FE70:
/* 8030FE70 0030CDB0  38 00 00 01 */	li r0, 1
lbl_8030FE74:
/* 8030FE74 0030CDB4  2C 00 00 00 */	cmpwi r0, 0
/* 8030FE78 0030CDB8  40 82 00 10 */	bne lbl_8030FE88
/* 8030FE7C 0030CDBC  80 7F 00 74 */	lwz r3, 0x74(r31)
/* 8030FE80 0030CDC0  38 80 00 00 */	li r4, 0
/* 8030FE84 0030CDC4  4B DC 42 9D */	bl setBckFromIndex__6MActorFi
lbl_8030FE88:
/* 8030FE88 0030CDC8  80 01 00 34 */	lwz r0, 0x34(r1)
/* 8030FE8C 0030CDCC  83 E1 00 2C */	lwz r31, 0x2c(r1)
/* 8030FE90 0030CDD0  38 21 00 30 */	addi r1, r1, 0x30
/* 8030FE94 0030CDD4  7C 08 03 A6 */	mtlr r0
/* 8030FE98 0030CDD8  4E 80 00 20 */	blr 

.global init__13TEggGeneratorFP12TLiveManager
init__13TEggGeneratorFP12TLiveManager:
/* 8030FE9C 0030CDDC  7C 08 02 A6 */	mflr r0
/* 8030FEA0 0030CDE0  90 01 00 04 */	stw r0, 4(r1)
/* 8030FEA4 0030CDE4  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 8030FEA8 0030CDE8  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 8030FEAC 0030CDEC  7C 7F 1B 78 */	mr r31, r3
/* 8030FEB0 0030CDF0  93 C1 00 18 */	stw r30, 0x18(r1)
/* 8030FEB4 0030CDF4  90 83 00 70 */	stw r4, 0x70(r3)
/* 8030FEB8 0030CDF8  7F E4 FB 78 */	mr r4, r31
/* 8030FEBC 0030CDFC  80 63 00 70 */	lwz r3, 0x70(r3)
/* 8030FEC0 0030CE00  4B E0 4A FD */	bl manageActor__12TLiveManagerFP10TLiveActor
/* 8030FEC4 0030CE04  38 60 00 18 */	li r3, 0x18
/* 8030FEC8 0030CE08  4B CF C9 E9 */	bl __nw__FUl
/* 8030FECC 0030CE0C  7C 7E 1B 79 */	or. r30, r3, r3
/* 8030FED0 0030CE10  41 82 00 14 */	beq lbl_8030FEE4
/* 8030FED4 0030CE14  80 9F 00 70 */	lwz r4, 0x70(r31)
/* 8030FED8 0030CE18  38 7E 00 00 */	addi r3, r30, 0
/* 8030FEDC 0030CE1C  38 A0 00 01 */	li r5, 1
/* 8030FEE0 0030CE20  4B E0 64 59 */	bl __ct__13TMActorKeeperFP12TLiveManagerUs
lbl_8030FEE4:
/* 8030FEE4 0030CE24  93 DF 00 78 */	stw r30, 0x78(r31)
/* 8030FEE8 0030CE28  3C 60 80 3A */	lis r3, "@2267"@ha
/* 8030FEEC 0030CE2C  38 83 11 30 */	addi r4, r3, "@2267"@l
/* 8030FEF0 0030CE30  80 7F 00 78 */	lwz r3, 0x78(r31)
/* 8030FEF4 0030CE34  38 A0 00 00 */	li r5, 0
/* 8030FEF8 0030CE38  4B E0 66 11 */	bl createMActor__13TMActorKeeperFPCcUl
/* 8030FEFC 0030CE3C  90 7F 00 74 */	stw r3, 0x74(r31)
/* 8030FF00 0030CE40  3C 80 02 00 */	lis r4, 0x02000001@ha
/* 8030FF04 0030CE44  38 7F 00 00 */	addi r3, r31, 0
/* 8030FF08 0030CE48  C0 22 FA 0C */	lfs f1, "@2309"@sda21(r2)
/* 8030FF0C 0030CE4C  38 84 00 01 */	addi r4, r4, 0x02000001@l
/* 8030FF10 0030CE50  38 A0 00 01 */	li r5, 1
/* 8030FF14 0030CE54  FC 40 08 90 */	fmr f2, f1
/* 8030FF18 0030CE58  3C C0 80 00 */	lis r6, 0x8000
/* 8030FF1C 0030CE5C  FC 60 08 90 */	fmr f3, f1
/* 8030FF20 0030CE60  FC 80 08 90 */	fmr f4, f1
/* 8030FF24 0030CE64  4B E0 D8 D5 */	bl initHitActor__9THitActorFUlUsiffff
/* 8030FF28 0030CE68  80 7F 00 74 */	lwz r3, 0x74(r31)
/* 8030FF2C 0030CE6C  38 80 00 00 */	li r4, 0
/* 8030FF30 0030CE70  4B DC 41 F1 */	bl setBckFromIndex__6MActorFi
/* 8030FF34 0030CE74  C0 5F 00 30 */	lfs f2, 0x30(r31)
/* 8030FF38 0030CE78  C0 22 FA 10 */	lfs f1, "@2310"@sda21(r2)
/* 8030FF3C 0030CE7C  C0 02 FA 14 */	lfs f0, "@2311"@sda21(r2)
/* 8030FF40 0030CE80  EC 42 08 28 */	fsubs f2, f2, f1
/* 8030FF44 0030CE84  48 00 00 08 */	b lbl_8030FF4C
lbl_8030FF48:
/* 8030FF48 0030CE88  EC 42 00 28 */	fsubs f2, f2, f0
lbl_8030FF4C:
/* 8030FF4C 0030CE8C  FC 02 00 40 */	fcmpo cr0, f2, f0
/* 8030FF50 0030CE90  4C 41 13 82 */	cror 2, 1, 2
/* 8030FF54 0030CE94  41 82 FF F4 */	beq lbl_8030FF48
/* 8030FF58 0030CE98  C0 22 FA 14 */	lfs f1, "@2311"@sda21(r2)
/* 8030FF5C 0030CE9C  C0 02 FA 18 */	lfs f0, "@2312"@sda21(r2)
/* 8030FF60 0030CEA0  48 00 00 08 */	b lbl_8030FF68
lbl_8030FF64:
/* 8030FF64 0030CEA4  EC 42 08 2A */	fadds f2, f2, f1
lbl_8030FF68:
/* 8030FF68 0030CEA8  FC 02 00 40 */	fcmpo cr0, f2, f0
/* 8030FF6C 0030CEAC  41 80 FF F8 */	blt lbl_8030FF64
/* 8030FF70 0030CEB0  D0 5F 00 30 */	stfs f2, 0x30(r31)
/* 8030FF74 0030CEB4  80 01 00 24 */	lwz r0, 0x24(r1)
/* 8030FF78 0030CEB8  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 8030FF7C 0030CEBC  83 C1 00 18 */	lwz r30, 0x18(r1)
/* 8030FF80 0030CEC0  7C 08 03 A6 */	mtlr r0
/* 8030FF84 0030CEC4  38 21 00 20 */	addi r1, r1, 0x20
/* 8030FF88 0030CEC8  4E 80 00 20 */	blr 

.global __ct__13TEggGeneratorFPCc
__ct__13TEggGeneratorFPCc:
/* 8030FF8C 0030CECC  7C 08 02 A6 */	mflr r0
/* 8030FF90 0030CED0  90 01 00 04 */	stw r0, 4(r1)
/* 8030FF94 0030CED4  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8030FF98 0030CED8  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8030FF9C 0030CEDC  7C 7F 1B 78 */	mr r31, r3
/* 8030FFA0 0030CEE0  4B F4 02 99 */	bl __ct__11TSpineEnemyFPCc
/* 8030FFA4 0030CEE4  3C 60 80 3E */	lis r3, __vt__13TEggGenerator@ha
/* 8030FFA8 0030CEE8  38 63 01 B4 */	addi r3, r3, __vt__13TEggGenerator@l
/* 8030FFAC 0030CEEC  90 7F 00 00 */	stw r3, 0(r31)
/* 8030FFB0 0030CEF0  38 03 00 24 */	addi r0, r3, 0x24
/* 8030FFB4 0030CEF4  38 7F 00 00 */	addi r3, r31, 0
/* 8030FFB8 0030CEF8  90 1F 00 20 */	stw r0, 0x20(r31)
/* 8030FFBC 0030CEFC  80 1F 00 F0 */	lwz r0, 0xf0(r31)
/* 8030FFC0 0030CF00  60 00 00 10 */	ori r0, r0, 0x10
/* 8030FFC4 0030CF04  90 1F 00 F0 */	stw r0, 0xf0(r31)
/* 8030FFC8 0030CF08  80 1F 00 F0 */	lwz r0, 0xf0(r31)
/* 8030FFCC 0030CF0C  60 00 00 08 */	ori r0, r0, 8
/* 8030FFD0 0030CF10  90 1F 00 F0 */	stw r0, 0xf0(r31)
/* 8030FFD4 0030CF14  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8030FFD8 0030CF18  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8030FFDC 0030CF1C  38 21 00 18 */	addi r1, r1, 0x18
/* 8030FFE0 0030CF20  7C 08 03 A6 */	mtlr r0
/* 8030FFE4 0030CF24  4E 80 00 20 */	blr 

.global __dt__13TEggGeneratorFv
__dt__13TEggGeneratorFv:
/* 8030FFE8 0030CF28  7C 08 02 A6 */	mflr r0
/* 8030FFEC 0030CF2C  90 01 00 04 */	stw r0, 4(r1)
/* 8030FFF0 0030CF30  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8030FFF4 0030CF34  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8030FFF8 0030CF38  3B E4 00 00 */	addi r31, r4, 0
/* 8030FFFC 0030CF3C  93 C1 00 10 */	stw r30, 0x10(r1)
/* 80310000 0030CF40  7C 7E 1B 79 */	or. r30, r3, r3
/* 80310004 0030CF44  41 82 00 34 */	beq lbl_80310038
/* 80310008 0030CF48  3C 60 80 3E */	lis r3, __vt__13TEggGenerator@ha
/* 8031000C 0030CF4C  38 63 01 B4 */	addi r3, r3, __vt__13TEggGenerator@l
/* 80310010 0030CF50  90 7E 00 00 */	stw r3, 0(r30)
/* 80310014 0030CF54  38 03 00 24 */	addi r0, r3, 0x24
/* 80310018 0030CF58  38 7E 00 00 */	addi r3, r30, 0
/* 8031001C 0030CF5C  90 1E 00 20 */	stw r0, 0x20(r30)
/* 80310020 0030CF60  38 80 00 00 */	li r4, 0
/* 80310024 0030CF64  4B F4 01 95 */	bl __dt__11TSpineEnemyFv
/* 80310028 0030CF68  7F E0 07 35 */	extsh. r0, r31
/* 8031002C 0030CF6C  40 81 00 0C */	ble lbl_80310038
/* 80310030 0030CF70  7F C3 F3 78 */	mr r3, r30
/* 80310034 0030CF74  4B CF CA 7D */	bl __dl__FPv
lbl_80310038:
/* 80310038 0030CF78  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8031003C 0030CF7C  7F C3 F3 78 */	mr r3, r30
/* 80310040 0030CF80  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80310044 0030CF84  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 80310048 0030CF88  7C 08 03 A6 */	mtlr r0
/* 8031004C 0030CF8C  38 21 00 18 */	addi r1, r1, 0x18
/* 80310050 0030CF90  4E 80 00 20 */	blr 

.global "@32@__dt__13TEggGeneratorFv"
"@32@__dt__13TEggGeneratorFv":
/* 80310054 0030CF94  38 63 FF E0 */	addi r3, r3, -32
/* 80310058 0030CF98  4B FF FF 90 */	b __dt__13TEggGeneratorFv

.section .rodata, "a"  # 0x8036FFA0 - 0x803A8380
.balign 8
"@1490":
  .4byte 0
  .4byte 0
  .4byte 0
"@1526":
	.4byte 0x83818382
	.4byte 0x838A82AA
	.4byte 0x91AB82E8
	.4byte 0x82DC82B9
	.4byte 0x82F10A00
"@1598":
	.4byte 0x4D416374
	.4byte 0x6F724D74
	.4byte 0x7843616C
	.4byte 0x63547970
	.4byte 0x655F4261
	.4byte 0x73696320
	.4byte 0x834E8389
	.4byte 0x83568362
	.4byte 0x834E8358
	.4byte 0x8350815B
	.4byte 0x838B826E
	.4byte 0x826D0000
"@1599":
	.4byte 0x4D416374
	.4byte 0x6F724D74
	.4byte 0x7843616C
	.4byte 0x63547970
	.4byte 0x655F536F
	.4byte 0x6674696D
	.4byte 0x61676520
	.4byte 0x834E8389
	.4byte 0x83568362
	.4byte 0x834E8358
	.4byte 0x8350815B
	.4byte 0x838B826E
	.4byte 0x82658265
	.4byte 0
"@1600":
	.4byte 0x4D416374
	.4byte 0x6F724D74
	.4byte 0x7843616C
	.4byte 0x63547970
	.4byte 0x655F4D6F
	.4byte 0x74696F6E
	.4byte 0x426C656E
	.4byte 0x64208382
	.4byte 0x815B8356
	.4byte 0x83878393
	.4byte 0x8375838C
	.4byte 0x83938368
	.4byte 0
"@1601":
	.4byte 0x4D416374
	.4byte 0x6F724D74
	.4byte 0x7843616C
	.4byte 0x63547970
	.4byte 0x655F5573
	.4byte 0x65722083
	.4byte 0x86815B83
	.4byte 0x55815B92
	.4byte 0xE88B6000
"@2267":
	.asciz "gene_egg_model1.bmd"
entry$2266:
  .4byte "@2267"
  .4byte 0x10210000
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
"@2277":
	.asciz "/enemy/egggen.prm"
	.balign 4

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__14TEggGenManager
__vt__14TEggGenManager:
  .4byte 0
  .4byte 0
  .4byte __dt__14TEggGenManagerFv
  .4byte getType__Q26JDrama8TNameRefCFv
  .4byte load__14TEggGenManagerFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__Q26JDrama8TNameRefFv
  .4byte searchF__11TObjManagerFUsPCc
  .4byte perform__13TEnemyManagerFUlPQ26JDrama9TGraphics
  .4byte createModelData__14TEggGenManagerFv
  .4byte createAnmData__11TObjManagerFv
  .4byte createModelDataArray__11TObjManagerFPC19TModelDataLoadEntry
  .4byte clipActors__12TLiveManagerFPQ26JDrama9TGraphics
  .4byte setFlagOutOfCube__12TLiveManagerFv
  .4byte createSpcBinary__12TLiveManagerFv
  .4byte hasMapCollision__12TLiveManagerCFv
  .4byte createEnemyInstance__13TEnemyManagerFv
  .4byte clipEnemies__13TEnemyManagerFPQ26JDrama9TGraphics
  .4byte restoreDrawBuffer__13TEnemyManagerFUl
  .4byte createEnemies__13TEnemyManagerFi
  .4byte changeDrawBuffer__13TEnemyManagerFUl
.global __vt__13TEggGenerator
__vt__13TEggGenerator:
  .4byte 0
  .4byte 0
  .4byte __dt__13TEggGeneratorFv
  .4byte getType__Q26JDrama6TActorCFv
  .4byte load__11TSpineEnemyFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__Q26JDrama8TNameRefFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc
  .4byte perform__11TSpineEnemyFUlPQ26JDrama9TGraphics
  .4byte 0
  .4byte 0
  .4byte "@32@__dt__13TEggGeneratorFv"
  .4byte JSGFGetType__Q26JStage6TActorCFv
  .4byte JSGGetName__Q26JStage7TObjectCFv
  .4byte JSGGetFlag__Q26JStage7TObjectCFv
  .4byte JSGSetFlag__Q26JStage7TObjectFUl
  .4byte JSGGetData__Q26JStage7TObjectCFUlPvUl
  .4byte JSGSetData__Q26JStage7TObjectFUlPCvUl
  .4byte JSGGetParent__Q26JStage7TObjectCFPPQ26JStage7TObjectPUl
  .4byte JSGSetParent__Q26JStage7TObjectFPQ26JStage7TObjectUl
  .4byte JSGSetRelation__Q26JStage7TObjectFbPQ26JStage7TObjectUl
  .4byte "@32@JSGGetTranslation__Q26JDrama6TActorCFP3Vec"
  .4byte "@32@JSGSetTranslation__Q26JDrama6TActorFRC3Vec"
  .4byte "@32@JSGGetScaling__Q26JDrama6TActorCFP3Vec"
  .4byte "@32@JSGSetScaling__Q26JDrama6TActorFRC3Vec"
  .4byte "@32@JSGGetRotation__Q26JDrama6TActorCFP3Vec"
  .4byte "@32@JSGSetRotation__Q26JDrama6TActorFRC3Vec"
  .4byte JSGGetShape__Q26JStage6TActorCFv
  .4byte JSGSetShape__Q26JStage6TActorFUl
  .4byte JSGGetAnimation__Q26JStage6TActorCFv
  .4byte JSGSetAnimation__Q26JStage6TActorFUl
  .4byte JSGGetAnimationFrame__Q26JStage6TActorCFv
  .4byte JSGSetAnimationFrame__Q26JStage6TActorFf
  .4byte JSGGetAnimationFrameMax__Q26JStage6TActorCFv
  .4byte JSGGetTranslation__Q26JDrama6TActorCFP3Vec
  .4byte JSGSetTranslation__Q26JDrama6TActorFRC3Vec
  .4byte JSGGetScaling__Q26JDrama6TActorCFP3Vec
  .4byte JSGSetScaling__Q26JDrama6TActorFRC3Vec
  .4byte JSGGetRotation__Q26JDrama6TActorCFP3Vec
  .4byte JSGSetRotation__Q26JDrama6TActorFRC3Vec
  .4byte receiveMessage__11TSpineEnemyFP9THitActorUl
  .4byte getTakingMtx__10TLiveActorFv
  .4byte ensureTakeSituation__10TTakeActorFv
  .4byte "moveRequest__10TTakeActorFRCQ29JGeometry8TVec3<f>"
  .4byte getRadiusAtY__10TTakeActorCFf
  .4byte belongToGround__10TLiveActorCFv
  .4byte getRootJointMtx__10TLiveActorCFv
  .4byte init__13TEggGeneratorFP12TLiveManager
  .4byte calcRootMatrix__11TSpineEnemyFv
  .4byte setGroundCollision__10TLiveActorFv
  .4byte control__13TEggGeneratorFv
  .4byte bind__10TLiveActorFv
  .4byte moveObject__10TLiveActorFv
  .4byte requestShadow__10TLiveActorFv
  .4byte drawObject__10TLiveActorFPQ26JDrama9TGraphics
  .4byte performOnlyDraw__10TLiveActorFUlPQ26JDrama9TGraphics
  .4byte getShadowType__10TLiveActorFv
  .4byte kill__10TLiveActorFv
  .4byte getGravityY__10TLiveActorCFv
  .4byte hasMapCollision__10TLiveActorCFv
  .4byte getFocalPoint__10TLiveActorCFv
  .4byte updateAnmSound__10TLiveActorFv
  .4byte getBasNameTable__10TLiveActorCFv
  .4byte reset__11TSpineEnemyFv
  .4byte "resetToPosition__11TSpineEnemyFRCQ29JGeometry8TVec3<f>"
  .4byte "resetSRTV__11TSpineEnemyFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>"
  .4byte getSaveParam__11TSpineEnemyCFv
  .4byte getPhaseShift__11TSpineEnemyCFv
  .4byte isReachedToGoal__11TSpineEnemyCFv

.section .sdata2, "a"  # 0x8040B460 - 0x80414020
.balign 8
"@2291":
	.4byte 0x48742400
"@2309":
	.4byte 0x41200000
"@2310":
	.4byte 0x42B40000
"@2311":
	.4byte 0x43B40000
"@2312":
  .4byte 0
  .4byte 0

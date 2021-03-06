.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __ct__13JPAExTexShapeFPCUc
__ct__13JPAExTexShapeFPCUc:
/* 8007FA94 0007C9D4  7C 08 02 A6 */	mflr r0
/* 8007FA98 0007C9D8  3C A0 80 3B */	lis r5, __vt__12JPADataBlock@ha
/* 8007FA9C 0007C9DC  90 01 00 04 */	stw r0, 4(r1)
/* 8007FAA0 0007C9E0  38 A5 CB F4 */	addi r5, r5, __vt__12JPADataBlock@l
/* 8007FAA4 0007C9E4  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8007FAA8 0007C9E8  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8007FAAC 0007C9EC  93 C1 00 10 */	stw r30, 0x10(r1)
/* 8007FAB0 0007C9F0  7C 9E 23 78 */	mr r30, r4
/* 8007FAB4 0007C9F4  90 61 00 08 */	stw r3, 8(r1)
/* 8007FAB8 0007C9F8  3C 60 80 3B */	lis r3, __vt__13JPAExTexShape@ha
/* 8007FABC 0007C9FC  38 03 CB E8 */	addi r0, r3, __vt__13JPAExTexShape@l
/* 8007FAC0 0007CA00  83 E1 00 08 */	lwz r31, 8(r1)
/* 8007FAC4 0007CA04  90 BF 00 00 */	stw r5, 0(r31)
/* 8007FAC8 0007CA08  90 1F 00 00 */	stw r0, 0(r31)
/* 8007FACC 0007CA0C  88 04 00 10 */	lbz r0, 0x10(r4)
/* 8007FAD0 0007CA10  98 1F 00 25 */	stb r0, 0x25(r31)
/* 8007FAD4 0007CA14  88 04 00 11 */	lbz r0, 0x11(r4)
/* 8007FAD8 0007CA18  2C 00 00 02 */	cmpwi r0, 2
/* 8007FADC 0007CA1C  41 82 00 80 */	beq lbl_8007FB5C
/* 8007FAE0 0007CA20  40 80 00 10 */	bge lbl_8007FAF0
/* 8007FAE4 0007CA24  2C 00 00 00 */	cmpwi r0, 0
/* 8007FAE8 0007CA28  40 80 00 14 */	bge lbl_8007FAFC
/* 8007FAEC 0007CA2C  48 00 00 84 */	b lbl_8007FB70
lbl_8007FAF0:
/* 8007FAF0 0007CA30  2C 00 00 04 */	cmpwi r0, 4
/* 8007FAF4 0007CA34  40 80 00 7C */	bge lbl_8007FB70
/* 8007FAF8 0007CA38  48 00 00 70 */	b lbl_8007FB68
lbl_8007FAFC:
/* 8007FAFC 0007CA3C  38 00 00 01 */	li r0, 1
/* 8007FB00 0007CA40  90 1F 00 20 */	stw r0, 0x20(r31)
/* 8007FB04 0007CA44  A8 7E 00 12 */	lha r3, 0x12(r30)
/* 8007FB08 0007CA48  4B FF 2E AD */	bl JPAConvertFixToFloat__Fs
/* 8007FB0C 0007CA4C  D0 3F 00 08 */	stfs f1, 8(r31)
/* 8007FB10 0007CA50  A8 7E 00 14 */	lha r3, 0x14(r30)
/* 8007FB14 0007CA54  4B FF 2E A1 */	bl JPAConvertFixToFloat__Fs
/* 8007FB18 0007CA58  D0 3F 00 0C */	stfs f1, 0xc(r31)
/* 8007FB1C 0007CA5C  A8 7E 00 16 */	lha r3, 0x16(r30)
/* 8007FB20 0007CA60  4B FF 2E 95 */	bl JPAConvertFixToFloat__Fs
/* 8007FB24 0007CA64  D0 3F 00 10 */	stfs f1, 0x10(r31)
/* 8007FB28 0007CA68  A8 7E 00 18 */	lha r3, 0x18(r30)
/* 8007FB2C 0007CA6C  4B FF 2E 89 */	bl JPAConvertFixToFloat__Fs
/* 8007FB30 0007CA70  D0 3F 00 14 */	stfs f1, 0x14(r31)
/* 8007FB34 0007CA74  A8 7E 00 1A */	lha r3, 0x1a(r30)
/* 8007FB38 0007CA78  4B FF 2E 7D */	bl JPAConvertFixToFloat__Fs
/* 8007FB3C 0007CA7C  D0 3F 00 18 */	stfs f1, 0x18(r31)
/* 8007FB40 0007CA80  A8 7E 00 1C */	lha r3, 0x1c(r30)
/* 8007FB44 0007CA84  4B FF 2E 71 */	bl JPAConvertFixToFloat__Fs
/* 8007FB48 0007CA88  D0 3F 00 1C */	stfs f1, 0x1c(r31)
/* 8007FB4C 0007CA8C  38 7F 00 08 */	addi r3, r31, 8
/* 8007FB50 0007CA90  38 80 00 18 */	li r4, 0x18
/* 8007FB54 0007CA94  48 00 E4 8D */	bl DCStoreRange
/* 8007FB58 0007CA98  48 00 00 18 */	b lbl_8007FB70
lbl_8007FB5C:
/* 8007FB5C 0007CA9C  38 00 00 05 */	li r0, 5
/* 8007FB60 0007CAA0  90 1F 00 20 */	stw r0, 0x20(r31)
/* 8007FB64 0007CAA4  48 00 00 0C */	b lbl_8007FB70
lbl_8007FB68:
/* 8007FB68 0007CAA8  38 00 00 09 */	li r0, 9
/* 8007FB6C 0007CAAC  90 1F 00 20 */	stw r0, 0x20(r31)
lbl_8007FB70:
/* 8007FB70 0007CAB0  80 61 00 08 */	lwz r3, 8(r1)
/* 8007FB74 0007CAB4  88 1E 00 1E */	lbz r0, 0x1e(r30)
/* 8007FB78 0007CAB8  98 03 00 24 */	stb r0, 0x24(r3)
/* 8007FB7C 0007CABC  88 1E 00 1F */	lbz r0, 0x1f(r30)
/* 8007FB80 0007CAC0  98 03 00 26 */	stb r0, 0x26(r3)
/* 8007FB84 0007CAC4  88 1E 00 20 */	lbz r0, 0x20(r30)
/* 8007FB88 0007CAC8  98 03 00 27 */	stb r0, 0x27(r3)
/* 8007FB8C 0007CACC  88 1E 00 30 */	lbz r0, 0x30(r30)
/* 8007FB90 0007CAD0  98 03 00 28 */	stb r0, 0x28(r3)
/* 8007FB94 0007CAD4  88 1E 00 33 */	lbz r0, 0x33(r30)
/* 8007FB98 0007CAD8  98 03 00 29 */	stb r0, 0x29(r3)
/* 8007FB9C 0007CADC  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8007FBA0 0007CAE0  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8007FBA4 0007CAE4  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 8007FBA8 0007CAE8  7C 08 03 A6 */	mtlr r0
/* 8007FBAC 0007CAEC  38 21 00 18 */	addi r1, r1, 0x18
/* 8007FBB0 0007CAF0  4E 80 00 20 */	blr 

.global __dt__12JPADataBlockFv
__dt__12JPADataBlockFv:
/* 8007FBB4 0007CAF4  7C 08 02 A6 */	mflr r0
/* 8007FBB8 0007CAF8  90 01 00 04 */	stw r0, 4(r1)
/* 8007FBBC 0007CAFC  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8007FBC0 0007CB00  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8007FBC4 0007CB04  7C 7F 1B 79 */	or. r31, r3, r3
/* 8007FBC8 0007CB08  41 82 00 20 */	beq lbl_8007FBE8
/* 8007FBCC 0007CB0C  3C 60 80 3B */	lis r3, __vt__12JPADataBlock@ha
/* 8007FBD0 0007CB10  38 63 CB F4 */	addi r3, r3, __vt__12JPADataBlock@l
/* 8007FBD4 0007CB14  7C 80 07 35 */	extsh. r0, r4
/* 8007FBD8 0007CB18  90 7F 00 00 */	stw r3, 0(r31)
/* 8007FBDC 0007CB1C  40 81 00 0C */	ble lbl_8007FBE8
/* 8007FBE0 0007CB20  7F E3 FB 78 */	mr r3, r31
/* 8007FBE4 0007CB24  4B F8 CE CD */	bl __dl__FPv
lbl_8007FBE8:
/* 8007FBE8 0007CB28  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8007FBEC 0007CB2C  7F E3 FB 78 */	mr r3, r31
/* 8007FBF0 0007CB30  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8007FBF4 0007CB34  38 21 00 18 */	addi r1, r1, 0x18
/* 8007FBF8 0007CB38  7C 08 03 A6 */	mtlr r0
/* 8007FBFC 0007CB3C  4E 80 00 20 */	blr 

.global __dt__13JPAExTexShapeFv
__dt__13JPAExTexShapeFv:
/* 8007FC00 0007CB40  7C 08 02 A6 */	mflr r0
/* 8007FC04 0007CB44  90 01 00 04 */	stw r0, 4(r1)
/* 8007FC08 0007CB48  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8007FC0C 0007CB4C  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8007FC10 0007CB50  7C 7F 1B 79 */	or. r31, r3, r3
/* 8007FC14 0007CB54  41 82 00 30 */	beq lbl_8007FC44
/* 8007FC18 0007CB58  3C 60 80 3B */	lis r3, __vt__13JPAExTexShape@ha
/* 8007FC1C 0007CB5C  38 03 CB E8 */	addi r0, r3, __vt__13JPAExTexShape@l
/* 8007FC20 0007CB60  90 1F 00 00 */	stw r0, 0(r31)
/* 8007FC24 0007CB64  41 82 00 10 */	beq lbl_8007FC34
/* 8007FC28 0007CB68  3C 60 80 3B */	lis r3, __vt__12JPADataBlock@ha
/* 8007FC2C 0007CB6C  38 03 CB F4 */	addi r0, r3, __vt__12JPADataBlock@l
/* 8007FC30 0007CB70  90 1F 00 00 */	stw r0, 0(r31)
lbl_8007FC34:
/* 8007FC34 0007CB74  7C 80 07 35 */	extsh. r0, r4
/* 8007FC38 0007CB78  40 81 00 0C */	ble lbl_8007FC44
/* 8007FC3C 0007CB7C  7F E3 FB 78 */	mr r3, r31
/* 8007FC40 0007CB80  4B F8 CE 71 */	bl __dl__FPv
lbl_8007FC44:
/* 8007FC44 0007CB84  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8007FC48 0007CB88  7F E3 FB 78 */	mr r3, r31
/* 8007FC4C 0007CB8C  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8007FC50 0007CB90  38 21 00 18 */	addi r1, r1, 0x18
/* 8007FC54 0007CB94  7C 08 03 A6 */	mtlr r0
/* 8007FC58 0007CB98  4E 80 00 20 */	blr 

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__13JPAExTexShape
__vt__13JPAExTexShape:
  .4byte 0
  .4byte 0
  .4byte __dt__13JPAExTexShapeFv
.global __vt__12JPADataBlock
__vt__12JPADataBlock:
  .4byte 0
  .4byte 0
  .4byte __dt__12JPADataBlockFv

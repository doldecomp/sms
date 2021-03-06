.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __ct__15J3DJointFactoryFRC13J3DJointBlock
__ct__15J3DJointFactoryFRC13J3DJointBlock:
/* 8002EB84 0002BAC4  7C 08 02 A6 */	mflr r0
/* 8002EB88 0002BAC8  90 01 00 04 */	stw r0, 4(r1)
/* 8002EB8C 0002BACC  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8002EB90 0002BAD0  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8002EB94 0002BAD4  7C 9F 23 78 */	mr r31, r4
/* 8002EB98 0002BAD8  93 C1 00 10 */	stw r30, 0x10(r1)
/* 8002EB9C 0002BADC  3B C3 00 00 */	addi r30, r3, 0
/* 8002EBA0 0002BAE0  38 7F 00 00 */	addi r3, r31, 0
/* 8002EBA4 0002BAE4  80 84 00 0C */	lwz r4, 0xc(r4)
/* 8002EBA8 0002BAE8  48 00 01 F1 */	bl "JSUConvertOffsetToPtr<16J3DJointInitData>__FPCvUl"
/* 8002EBAC 0002BAEC  90 7E 00 00 */	stw r3, 0(r30)
/* 8002EBB0 0002BAF0  7F E3 FB 78 */	mr r3, r31
/* 8002EBB4 0002BAF4  80 9F 00 10 */	lwz r4, 0x10(r31)
/* 8002EBB8 0002BAF8  48 00 01 C9 */	bl "JSUConvertOffsetToPtr<Us>__FPCvUl"
/* 8002EBBC 0002BAFC  90 7E 00 04 */	stw r3, 4(r30)
/* 8002EBC0 0002BB00  7F C3 F3 78 */	mr r3, r30
/* 8002EBC4 0002BB04  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8002EBC8 0002BB08  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8002EBCC 0002BB0C  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 8002EBD0 0002BB10  7C 08 03 A6 */	mtlr r0
/* 8002EBD4 0002BB14  38 21 00 18 */	addi r1, r1, 0x18
/* 8002EBD8 0002BB18  4E 80 00 20 */	blr 

.global create__15J3DJointFactoryFi
create__15J3DJointFactoryFi:
/* 8002EBDC 0002BB1C  7C 08 02 A6 */	mflr r0
/* 8002EBE0 0002BB20  90 01 00 04 */	stw r0, 4(r1)
/* 8002EBE4 0002BB24  94 21 FF C0 */	stwu r1, -0x40(r1)
/* 8002EBE8 0002BB28  93 E1 00 3C */	stw r31, 0x3c(r1)
/* 8002EBEC 0002BB2C  3B E3 00 00 */	addi r31, r3, 0
/* 8002EBF0 0002BB30  38 60 00 64 */	li r3, 0x64
/* 8002EBF4 0002BB34  93 C1 00 38 */	stw r30, 0x38(r1)
/* 8002EBF8 0002BB38  93 A1 00 34 */	stw r29, 0x34(r1)
/* 8002EBFC 0002BB3C  3B A4 00 00 */	addi r29, r4, 0
/* 8002EC00 0002BB40  4B FD DC B1 */	bl __nw__FUl
/* 8002EC04 0002BB44  7C 7E 1B 79 */	or. r30, r3, r3
/* 8002EC08 0002BB48  41 82 00 24 */	beq lbl_8002EC2C
/* 8002EC0C 0002BB4C  93 C1 00 24 */	stw r30, 0x24(r1)
/* 8002EC10 0002BB50  80 61 00 24 */	lwz r3, 0x24(r1)
/* 8002EC14 0002BB54  4B FF A9 21 */	bl __ct__7J3DNodeFv
/* 8002EC18 0002BB58  3C 80 80 3B */	lis r4, __vt__8J3DJoint@ha
/* 8002EC1C 0002BB5C  80 61 00 24 */	lwz r3, 0x24(r1)
/* 8002EC20 0002BB60  38 04 8E 18 */	addi r0, r4, __vt__8J3DJoint@l
/* 8002EC24 0002BB64  90 03 00 00 */	stw r0, 0(r3)
/* 8002EC28 0002BB68  4B FE FF D5 */	bl initialize__8J3DJointFv
lbl_8002EC2C:
/* 8002EC2C 0002BB6C  B3 BE 00 18 */	sth r29, 0x18(r30)
/* 8002EC30 0002BB70  57 A0 08 3C */	slwi r0, r29, 1
/* 8002EC34 0002BB74  38 60 00 00 */	li r3, 0
/* 8002EC38 0002BB78  80 9F 00 04 */	lwz r4, 4(r31)
/* 8002EC3C 0002BB7C  80 BF 00 00 */	lwz r5, 0(r31)
/* 8002EC40 0002BB80  7C 84 02 2E */	lhzx r4, r4, r0
/* 8002EC44 0002BB84  54 84 30 32 */	slwi r4, r4, 6
/* 8002EC48 0002BB88  7C 85 22 2E */	lhzx r4, r5, r4
/* 8002EC4C 0002BB8C  98 9E 00 1A */	stb r4, 0x1a(r30)
/* 8002EC50 0002BB90  80 9F 00 04 */	lwz r4, 4(r31)
/* 8002EC54 0002BB94  80 BF 00 00 */	lwz r5, 0(r31)
/* 8002EC58 0002BB98  7C 84 02 2E */	lhzx r4, r4, r0
/* 8002EC5C 0002BB9C  54 84 30 32 */	slwi r4, r4, 6
/* 8002EC60 0002BBA0  7C 85 22 14 */	add r4, r5, r4
/* 8002EC64 0002BBA4  88 84 00 02 */	lbz r4, 2(r4)
/* 8002EC68 0002BBA8  98 9E 00 1B */	stb r4, 0x1b(r30)
/* 8002EC6C 0002BBAC  80 9F 00 04 */	lwz r4, 4(r31)
/* 8002EC70 0002BBB0  80 BF 00 00 */	lwz r5, 0(r31)
/* 8002EC74 0002BBB4  7C 84 02 2E */	lhzx r4, r4, r0
/* 8002EC78 0002BBB8  54 84 30 32 */	slwi r4, r4, 6
/* 8002EC7C 0002BBBC  38 C4 00 04 */	addi r6, r4, 4
/* 8002EC80 0002BBC0  7C C5 32 14 */	add r6, r5, r6
/* 8002EC84 0002BBC4  C0 06 00 00 */	lfs f0, 0(r6)
/* 8002EC88 0002BBC8  D0 1E 00 1C */	stfs f0, 0x1c(r30)
/* 8002EC8C 0002BBCC  C0 06 00 04 */	lfs f0, 4(r6)
/* 8002EC90 0002BBD0  D0 1E 00 20 */	stfs f0, 0x20(r30)
/* 8002EC94 0002BBD4  C0 06 00 08 */	lfs f0, 8(r6)
/* 8002EC98 0002BBD8  D0 1E 00 24 */	stfs f0, 0x24(r30)
/* 8002EC9C 0002BBDC  A8 86 00 0C */	lha r4, 0xc(r6)
/* 8002ECA0 0002BBE0  B0 9E 00 28 */	sth r4, 0x28(r30)
/* 8002ECA4 0002BBE4  A8 86 00 0E */	lha r4, 0xe(r6)
/* 8002ECA8 0002BBE8  B0 9E 00 2A */	sth r4, 0x2a(r30)
/* 8002ECAC 0002BBEC  A8 86 00 10 */	lha r4, 0x10(r6)
/* 8002ECB0 0002BBF0  B0 9E 00 2C */	sth r4, 0x2c(r30)
/* 8002ECB4 0002BBF4  C0 06 00 14 */	lfs f0, 0x14(r6)
/* 8002ECB8 0002BBF8  D0 1E 00 30 */	stfs f0, 0x30(r30)
/* 8002ECBC 0002BBFC  C0 06 00 18 */	lfs f0, 0x18(r6)
/* 8002ECC0 0002BC00  D0 1E 00 34 */	stfs f0, 0x34(r30)
/* 8002ECC4 0002BC04  C0 06 00 1C */	lfs f0, 0x1c(r6)
/* 8002ECC8 0002BC08  D0 1E 00 38 */	stfs f0, 0x38(r30)
/* 8002ECCC 0002BC0C  80 9F 00 04 */	lwz r4, 4(r31)
/* 8002ECD0 0002BC10  80 BF 00 00 */	lwz r5, 0(r31)
/* 8002ECD4 0002BC14  7C 84 02 2E */	lhzx r4, r4, r0
/* 8002ECD8 0002BC18  54 84 30 32 */	slwi r4, r4, 6
/* 8002ECDC 0002BC1C  7C 85 22 14 */	add r4, r5, r4
/* 8002ECE0 0002BC20  C0 04 00 24 */	lfs f0, 0x24(r4)
/* 8002ECE4 0002BC24  D0 1E 00 3C */	stfs f0, 0x3c(r30)
/* 8002ECE8 0002BC28  80 9F 00 04 */	lwz r4, 4(r31)
/* 8002ECEC 0002BC2C  80 BF 00 00 */	lwz r5, 0(r31)
/* 8002ECF0 0002BC30  7C 84 02 2E */	lhzx r4, r4, r0
/* 8002ECF4 0002BC34  54 84 30 32 */	slwi r4, r4, 6
/* 8002ECF8 0002BC38  38 84 00 28 */	addi r4, r4, 0x28
/* 8002ECFC 0002BC3C  7C C5 22 14 */	add r6, r5, r4
/* 8002ED00 0002BC40  80 A6 00 00 */	lwz r5, 0(r6)
/* 8002ED04 0002BC44  80 86 00 04 */	lwz r4, 4(r6)
/* 8002ED08 0002BC48  90 BE 00 40 */	stw r5, 0x40(r30)
/* 8002ED0C 0002BC4C  90 9E 00 44 */	stw r4, 0x44(r30)
/* 8002ED10 0002BC50  80 86 00 08 */	lwz r4, 8(r6)
/* 8002ED14 0002BC54  90 9E 00 48 */	stw r4, 0x48(r30)
/* 8002ED18 0002BC58  80 9F 00 04 */	lwz r4, 4(r31)
/* 8002ED1C 0002BC5C  80 BF 00 00 */	lwz r5, 0(r31)
/* 8002ED20 0002BC60  7C 04 02 2E */	lhzx r0, r4, r0
/* 8002ED24 0002BC64  54 04 30 32 */	slwi r4, r0, 6
/* 8002ED28 0002BC68  38 04 00 34 */	addi r0, r4, 0x34
/* 8002ED2C 0002BC6C  7C A5 02 14 */	add r5, r5, r0
/* 8002ED30 0002BC70  80 85 00 00 */	lwz r4, 0(r5)
/* 8002ED34 0002BC74  80 05 00 04 */	lwz r0, 4(r5)
/* 8002ED38 0002BC78  90 9E 00 4C */	stw r4, 0x4c(r30)
/* 8002ED3C 0002BC7C  90 1E 00 50 */	stw r0, 0x50(r30)
/* 8002ED40 0002BC80  80 05 00 08 */	lwz r0, 8(r5)
/* 8002ED44 0002BC84  90 1E 00 54 */	stw r0, 0x54(r30)
/* 8002ED48 0002BC88  90 7E 00 58 */	stw r3, 0x58(r30)
/* 8002ED4C 0002BC8C  90 7E 00 5C */	stw r3, 0x5c(r30)
/* 8002ED50 0002BC90  88 1E 00 1B */	lbz r0, 0x1b(r30)
/* 8002ED54 0002BC94  28 00 00 FF */	cmplwi r0, 0xff
/* 8002ED58 0002BC98  40 82 00 08 */	bne lbl_8002ED60
/* 8002ED5C 0002BC9C  98 7E 00 1B */	stb r3, 0x1b(r30)
lbl_8002ED60:
/* 8002ED60 0002BCA0  80 01 00 44 */	lwz r0, 0x44(r1)
/* 8002ED64 0002BCA4  7F C3 F3 78 */	mr r3, r30
/* 8002ED68 0002BCA8  83 E1 00 3C */	lwz r31, 0x3c(r1)
/* 8002ED6C 0002BCAC  83 C1 00 38 */	lwz r30, 0x38(r1)
/* 8002ED70 0002BCB0  7C 08 03 A6 */	mtlr r0
/* 8002ED74 0002BCB4  83 A1 00 34 */	lwz r29, 0x34(r1)
/* 8002ED78 0002BCB8  38 21 00 40 */	addi r1, r1, 0x40
/* 8002ED7C 0002BCBC  4E 80 00 20 */	blr 

.global "JSUConvertOffsetToPtr<Us>__FPCvUl"
"JSUConvertOffsetToPtr<Us>__FPCvUl":
/* 8002ED80 0002BCC0  28 04 00 00 */	cmplwi r4, 0
/* 8002ED84 0002BCC4  40 82 00 0C */	bne lbl_8002ED90
/* 8002ED88 0002BCC8  38 60 00 00 */	li r3, 0
/* 8002ED8C 0002BCCC  4E 80 00 20 */	blr 
lbl_8002ED90:
/* 8002ED90 0002BCD0  7C 63 22 14 */	add r3, r3, r4
/* 8002ED94 0002BCD4  4E 80 00 20 */	blr 

.global "JSUConvertOffsetToPtr<16J3DJointInitData>__FPCvUl"
"JSUConvertOffsetToPtr<16J3DJointInitData>__FPCvUl":
/* 8002ED98 0002BCD8  28 04 00 00 */	cmplwi r4, 0
/* 8002ED9C 0002BCDC  40 82 00 0C */	bne lbl_8002EDA8
/* 8002EDA0 0002BCE0  38 60 00 00 */	li r3, 0
/* 8002EDA4 0002BCE4  4E 80 00 20 */	blr 
lbl_8002EDA8:
/* 8002EDA8 0002BCE8  7C 63 22 14 */	add r3, r3, r4
/* 8002EDAC 0002BCEC  4E 80 00 20 */	blr 

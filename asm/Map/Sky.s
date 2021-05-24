.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __dt__4TSkyFv
__dt__4TSkyFv:
/* 801F1960 001EE8A0  7C 08 02 A6 */	mflr r0
/* 801F1964 001EE8A4  90 01 00 04 */	stw r0, 4(r1)
/* 801F1968 001EE8A8  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 801F196C 001EE8AC  93 E1 00 14 */	stw r31, 0x14(r1)
/* 801F1970 001EE8B0  3B E4 00 00 */	addi r31, r4, 0
/* 801F1974 001EE8B4  93 C1 00 10 */	stw r30, 0x10(r1)
/* 801F1978 001EE8B8  7C 7E 1B 79 */	or. r30, r3, r3
/* 801F197C 001EE8BC  41 82 00 34 */	beq lbl_801F19B0
/* 801F1980 001EE8C0  3C 60 80 3D */	lis r3, __vt__4TSky@ha
/* 801F1984 001EE8C4  38 63 05 A8 */	addi r3, r3, __vt__4TSky@l
/* 801F1988 001EE8C8  90 7E 00 00 */	stw r3, 0(r30)
/* 801F198C 001EE8CC  38 03 00 24 */	addi r0, r3, 0x24
/* 801F1990 001EE8D0  38 7E 00 00 */	addi r3, r30, 0
/* 801F1994 001EE8D4  90 1E 00 20 */	stw r0, 0x20(r30)
/* 801F1998 001EE8D8  38 80 00 00 */	li r4, 0
/* 801F199C 001EE8DC  4B E4 F1 F9 */	bl __dt__Q26JDrama6TActorFv
/* 801F19A0 001EE8E0  7F E0 07 35 */	extsh. r0, r31
/* 801F19A4 001EE8E4  40 81 00 0C */	ble lbl_801F19B0
/* 801F19A8 001EE8E8  7F C3 F3 78 */	mr r3, r30
/* 801F19AC 001EE8EC  4B E1 B1 05 */	bl __dl__FPv
lbl_801F19B0:
/* 801F19B0 001EE8F0  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 801F19B4 001EE8F4  7F C3 F3 78 */	mr r3, r30
/* 801F19B8 001EE8F8  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 801F19BC 001EE8FC  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 801F19C0 001EE900  7C 08 03 A6 */	mtlr r0
/* 801F19C4 001EE904  38 21 00 18 */	addi r1, r1, 0x18
/* 801F19C8 001EE908  4E 80 00 20 */	blr 

.global __ct__4TSkyFPCc
__ct__4TSkyFPCc:
/* 801F19CC 001EE90C  7C 08 02 A6 */	mflr r0
/* 801F19D0 001EE910  90 01 00 04 */	stw r0, 4(r1)
/* 801F19D4 001EE914  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 801F19D8 001EE918  93 E1 00 24 */	stw r31, 0x24(r1)
/* 801F19DC 001EE91C  7C 7F 1B 78 */	mr r31, r3
/* 801F19E0 001EE920  93 C1 00 20 */	stw r30, 0x20(r1)
/* 801F19E4 001EE924  93 E1 00 10 */	stw r31, 0x10(r1)
/* 801F19E8 001EE928  93 E1 00 14 */	stw r31, 0x14(r1)
/* 801F19EC 001EE92C  80 61 00 10 */	lwz r3, 0x10(r1)
/* 801F19F0 001EE930  4B E4 F1 19 */	bl __ct__Q26JDrama8TNameRefFPCc
/* 801F19F4 001EE934  80 61 00 10 */	lwz r3, 0x10(r1)
/* 801F19F8 001EE938  3C 80 80 3B */	lis r4, __vt__Q26JDrama8TViewObj@ha
/* 801F19FC 001EE93C  38 04 A0 C0 */	addi r0, r4, __vt__Q26JDrama8TViewObj@l
/* 801F1A00 001EE940  90 03 00 00 */	stw r0, 0(r3)
/* 801F1A04 001EE944  38 63 00 0C */	addi r3, r3, 0xc
/* 801F1A08 001EE948  38 80 00 00 */	li r4, 0
/* 801F1A0C 001EE94C  4B E4 F0 F5 */	bl __ct__Q26JDrama10TFlagT$$0Us$$1FUs
/* 801F1A10 001EE950  83 C1 00 14 */	lwz r30, 0x14(r1)
/* 801F1A14 001EE954  3C 60 80 3B */	lis r3, __vt__Q26JDrama10TPlacement@ha
/* 801F1A18 001EE958  38 03 A8 B0 */	addi r0, r3, __vt__Q26JDrama10TPlacement@l
/* 801F1A1C 001EE95C  90 1E 00 00 */	stw r0, 0(r30)
/* 801F1A20 001EE960  38 7E 00 10 */	addi r3, r30, 0x10
/* 801F1A24 001EE964  C0 22 D0 AC */	lfs f1, $$22787@sda21(r2)
/* 801F1A28 001EE968  FC 40 08 90 */	fmr f2, f1
/* 801F1A2C 001EE96C  FC 60 08 90 */	fmr f3, f1
/* 801F1A30 001EE970  48 00 00 A5 */	bl set$$0f$$1__Q29JGeometry8TVec3$$0f$$1Ffff
/* 801F1A34 001EE974  38 A0 00 00 */	li r5, 0
/* 801F1A38 001EE978  3C 60 80 3B */	lis r3, __vt__Q26JStage7TObject@ha
/* 801F1A3C 001EE97C  B0 BE 00 1C */	sth r5, 0x1c(r30)
/* 801F1A40 001EE980  38 03 CE 98 */	addi r0, r3, __vt__Q26JStage7TObject@l
/* 801F1A44 001EE984  3C 60 80 3B */	lis r3, __vt__Q26JStage6TActor@ha
/* 801F1A48 001EE988  90 1F 00 20 */	stw r0, 0x20(r31)
/* 801F1A4C 001EE98C  38 03 CD D0 */	addi r0, r3, __vt__Q26JStage6TActor@l
/* 801F1A50 001EE990  3C 60 80 3B */	lis r3, __vt__Q26JDrama6TActor@ha
/* 801F1A54 001EE994  90 1F 00 20 */	stw r0, 0x20(r31)
/* 801F1A58 001EE998  38 63 A0 20 */	addi r3, r3, __vt__Q26JDrama6TActor@l
/* 801F1A5C 001EE99C  90 7F 00 00 */	stw r3, 0(r31)
/* 801F1A60 001EE9A0  38 03 00 24 */	addi r0, r3, 0x24
/* 801F1A64 001EE9A4  3C 60 80 3D */	lis r3, __vt__4TSky@ha
/* 801F1A68 001EE9A8  90 1F 00 20 */	stw r0, 0x20(r31)
/* 801F1A6C 001EE9AC  38 83 05 A8 */	addi r4, r3, __vt__4TSky@l
/* 801F1A70 001EE9B0  38 04 00 24 */	addi r0, r4, 0x24
/* 801F1A74 001EE9B4  C0 02 D0 B0 */	lfs f0, $$22788@sda21(r2)
/* 801F1A78 001EE9B8  7F E3 FB 78 */	mr r3, r31
/* 801F1A7C 001EE9BC  D0 1F 00 24 */	stfs f0, 0x24(r31)
/* 801F1A80 001EE9C0  D0 1F 00 28 */	stfs f0, 0x28(r31)
/* 801F1A84 001EE9C4  D0 1F 00 2C */	stfs f0, 0x2c(r31)
/* 801F1A88 001EE9C8  C0 02 D0 AC */	lfs f0, $$22787@sda21(r2)
/* 801F1A8C 001EE9CC  D0 1F 00 30 */	stfs f0, 0x30(r31)
/* 801F1A90 001EE9D0  D0 1F 00 34 */	stfs f0, 0x34(r31)
/* 801F1A94 001EE9D4  D0 1F 00 38 */	stfs f0, 0x38(r31)
/* 801F1A98 001EE9D8  90 BF 00 3C */	stw r5, 0x3c(r31)
/* 801F1A9C 001EE9DC  90 BF 00 40 */	stw r5, 0x40(r31)
/* 801F1AA0 001EE9E0  90 9F 00 00 */	stw r4, 0(r31)
/* 801F1AA4 001EE9E4  90 1F 00 20 */	stw r0, 0x20(r31)
/* 801F1AA8 001EE9E8  90 BF 00 44 */	stw r5, 0x44(r31)
/* 801F1AAC 001EE9EC  C0 02 D0 B4 */	lfs f0, $$22789@sda21(r2)
/* 801F1AB0 001EE9F0  D0 1F 00 48 */	stfs f0, 0x48(r31)
/* 801F1AB4 001EE9F4  C0 02 D0 B8 */	lfs f0, $$22790@sda21(r2)
/* 801F1AB8 001EE9F8  D0 1F 00 4C */	stfs f0, 0x4c(r31)
/* 801F1ABC 001EE9FC  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 801F1AC0 001EEA00  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 801F1AC4 001EEA04  83 C1 00 20 */	lwz r30, 0x20(r1)
/* 801F1AC8 001EEA08  7C 08 03 A6 */	mtlr r0
/* 801F1ACC 001EEA0C  38 21 00 28 */	addi r1, r1, 0x28
/* 801F1AD0 001EEA10  4E 80 00 20 */	blr 

set$$0f$$1__Q29JGeometry8TVec3$$0f$$1Ffff:
/* 801F1AD4 001EEA14  D0 23 00 00 */	stfs f1, 0(r3)
/* 801F1AD8 001EEA18  D0 43 00 04 */	stfs f2, 4(r3)
/* 801F1ADC 001EEA1C  D0 63 00 08 */	stfs f3, 8(r3)
/* 801F1AE0 001EEA20  4E 80 00 20 */	blr 

.global load__4TSkyFR20JSUMemoryInputStream
load__4TSkyFR20JSUMemoryInputStream:
/* 801F1AE4 001EEA24  7C 08 02 A6 */	mflr r0
/* 801F1AE8 001EEA28  90 01 00 04 */	stw r0, 4(r1)
/* 801F1AEC 001EEA2C  94 21 FF A0 */	stwu r1, -0x60(r1)
/* 801F1AF0 001EEA30  93 E1 00 5C */	stw r31, 0x5c(r1)
/* 801F1AF4 001EEA34  7C 7F 1B 78 */	mr r31, r3
/* 801F1AF8 001EEA38  4B E4 EE 51 */	bl load__Q26JDrama6TActorFR20JSUMemoryInputStream
/* 801F1AFC 001EEA3C  80 8D 9A B0 */	lwz r4, gpMap@sda21(r13)
/* 801F1B00 001EEA40  3C 60 80 39 */	lis r3, $$22827@ha
/* 801F1B04 001EEA44  38 63 2A 98 */	addi r3, r3, $$22827@l
/* 801F1B08 001EEA48  80 84 00 14 */	lwz r4, 0x14(r4)
/* 801F1B0C 001EEA4C  38 A0 00 03 */	li r5, 3
/* 801F1B10 001EEA50  3C C0 10 22 */	lis r6, 0x1022
/* 801F1B14 001EEA54  38 84 00 20 */	addi r4, r4, 0x20
/* 801F1B18 001EEA58  4B EE 7F D5 */	bl SMS_MakeMActorWithAnmData__FPCcP13MActorAnmDataUlUl
/* 801F1B1C 001EEA5C  90 7F 00 44 */	stw r3, 0x44(r31)
/* 801F1B20 001EEA60  80 6D 9A 10 */	lwz r3, gpMapObjManager@sda21(r13)
/* 801F1B24 001EEA64  80 83 00 68 */	lwz r4, 0x68(r3)
/* 801F1B28 001EEA68  28 04 00 00 */	cmplwi r4, 0
/* 801F1B2C 001EEA6C  41 82 00 20 */	beq lbl_801F1B4C
/* 801F1B30 001EEA70  80 7F 00 44 */	lwz r3, 0x44(r31)
/* 801F1B34 001EEA74  38 A0 00 03 */	li r5, 3
/* 801F1B38 001EEA78  80 63 00 04 */	lwz r3, 4(r3)
/* 801F1B3C 001EEA7C  80 63 00 04 */	lwz r3, 4(r3)
/* 801F1B40 001EEA80  4B E3 52 C9 */	bl setMaterialTable__12J3DModelDataFP16J3DMaterialTable19J3DMaterialCopyFlag
/* 801F1B44 001EEA84  80 7F 00 44 */	lwz r3, 0x44(r31)
/* 801F1B48 001EEA88  4B EE 34 C5 */	bl initDL__6MActorFv
lbl_801F1B4C:
/* 801F1B4C 001EEA8C  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 801F1B50 001EEA90  88 03 00 7C */	lbz r0, 0x7c(r3)
/* 801F1B54 001EEA94  28 00 00 0F */	cmplwi r0, 0xf
/* 801F1B58 001EEA98  41 82 00 10 */	beq lbl_801F1B68
/* 801F1B5C 001EEA9C  80 7F 00 44 */	lwz r3, 0x44(r31)
/* 801F1B60 001EEAA0  38 82 D0 BC */	addi r4, r2, $$22828@sda21
/* 801F1B64 001EEAA4  4B FA 0A A5 */	bl startAllAnim__11TMapObjBaseFP6MActorPCc
lbl_801F1B68:
/* 801F1B68 001EEAA8  80 01 00 64 */	lwz r0, 0x64(r1)
/* 801F1B6C 001EEAAC  83 E1 00 5C */	lwz r31, 0x5c(r1)
/* 801F1B70 001EEAB0  38 21 00 60 */	addi r1, r1, 0x60
/* 801F1B74 001EEAB4  7C 08 03 A6 */	mtlr r0
/* 801F1B78 001EEAB8  4E 80 00 20 */	blr 

.global perform__4TSkyFUlPQ26JDrama9TGraphics
perform__4TSkyFUlPQ26JDrama9TGraphics:
/* 801F1B7C 001EEABC  7C 08 02 A6 */	mflr r0
/* 801F1B80 001EEAC0  90 01 00 04 */	stw r0, 4(r1)
/* 801F1B84 001EEAC4  54 80 07 BD */	rlwinm. r0, r4, 0, 0x1e, 0x1e
/* 801F1B88 001EEAC8  94 21 FE C8 */	stwu r1, -0x138(r1)
/* 801F1B8C 001EEACC  DB E1 01 30 */	stfd f31, 0x130(r1)
/* 801F1B90 001EEAD0  93 E1 01 2C */	stw r31, 0x12c(r1)
/* 801F1B94 001EEAD4  93 C1 01 28 */	stw r30, 0x128(r1)
/* 801F1B98 001EEAD8  3B C5 00 00 */	addi r30, r5, 0
/* 801F1B9C 001EEADC  93 A1 01 24 */	stw r29, 0x124(r1)
/* 801F1BA0 001EEAE0  3B A4 00 00 */	addi r29, r4, 0
/* 801F1BA4 001EEAE4  93 81 01 20 */	stw r28, 0x120(r1)
/* 801F1BA8 001EEAE8  3B 83 00 00 */	addi r28, r3, 0
/* 801F1BAC 001EEAEC  41 82 01 80 */	beq lbl_801F1D2C
/* 801F1BB0 001EEAF0  80 6D A8 B0 */	lwz r3, gpCamera@sda21(r13)
/* 801F1BB4 001EEAF4  38 81 00 EC */	addi r4, r1, 0xec
/* 801F1BB8 001EEAF8  3B E3 01 EC */	addi r31, r3, 0x1ec
/* 801F1BBC 001EEAFC  38 7F 00 00 */	addi r3, r31, 0
/* 801F1BC0 001EEB00  4B EA 28 1D */	bl PSMTXInverse
/* 801F1BC4 001EEB04  38 61 00 BC */	addi r3, r1, 0xbc
/* 801F1BC8 001EEB08  4B EA 26 E9 */	bl PSMTXIdentity
/* 801F1BCC 001EEB0C  C0 41 00 EC */	lfs f2, 0xec(r1)
/* 801F1BD0 001EEB10  C0 21 00 F0 */	lfs f1, 0xf0(r1)
/* 801F1BD4 001EEB14  C0 1F 00 1C */	lfs f0, 0x1c(r31)
/* 801F1BD8 001EEB18  FC 80 10 50 */	fneg f4, f2
/* 801F1BDC 001EEB1C  C0 7F 00 0C */	lfs f3, 0xc(r31)
/* 801F1BE0 001EEB20  EC 41 00 32 */	fmuls f2, f1, f0
/* 801F1BE4 001EEB24  C0 21 00 FC */	lfs f1, 0xfc(r1)
/* 801F1BE8 001EEB28  C0 01 01 0C */	lfs f0, 0x10c(r1)
/* 801F1BEC 001EEB2C  C0 A1 00 F4 */	lfs f5, 0xf4(r1)
/* 801F1BF0 001EEB30  FC C0 08 50 */	fneg f6, f1
/* 801F1BF4 001EEB34  EC 24 10 F8 */	fmsubs f1, f4, f3, f2
/* 801F1BF8 001EEB38  C0 9F 00 2C */	lfs f4, 0x2c(r31)
/* 801F1BFC 001EEB3C  C0 41 01 00 */	lfs f2, 0x100(r1)
/* 801F1C00 001EEB40  FC 60 00 50 */	fneg f3, f0
/* 801F1C04 001EEB44  C0 E1 01 04 */	lfs f7, 0x104(r1)
/* 801F1C08 001EEB48  EC 05 09 3C */	fnmsubs f0, f5, f4, f1
/* 801F1C0C 001EEB4C  C0 21 01 10 */	lfs f1, 0x110(r1)
/* 801F1C10 001EEB50  C0 81 01 14 */	lfs f4, 0x114(r1)
/* 801F1C14 001EEB54  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 801F1C18 001EEB58  D0 01 00 C8 */	stfs f0, 0xc8(r1)
/* 801F1C1C 001EEB5C  C0 1F 00 1C */	lfs f0, 0x1c(r31)
/* 801F1C20 001EEB60  C0 BF 00 0C */	lfs f5, 0xc(r31)
/* 801F1C24 001EEB64  EC 02 00 32 */	fmuls f0, f2, f0
/* 801F1C28 001EEB68  C0 5F 00 2C */	lfs f2, 0x2c(r31)
/* 801F1C2C 001EEB6C  EC 06 01 78 */	fmsubs f0, f6, f5, f0
/* 801F1C30 001EEB70  EC 07 00 BC */	fnmsubs f0, f7, f2, f0
/* 801F1C34 001EEB74  D0 01 00 D8 */	stfs f0, 0xd8(r1)
/* 801F1C38 001EEB78  C0 1F 00 1C */	lfs f0, 0x1c(r31)
/* 801F1C3C 001EEB7C  C0 5F 00 0C */	lfs f2, 0xc(r31)
/* 801F1C40 001EEB80  EC 01 00 32 */	fmuls f0, f1, f0
/* 801F1C44 001EEB84  C0 3F 00 2C */	lfs f1, 0x2c(r31)
/* 801F1C48 001EEB88  EC 03 00 B8 */	fmsubs f0, f3, f2, f0
/* 801F1C4C 001EEB8C  EC 04 00 7C */	fnmsubs f0, f4, f1, f0
/* 801F1C50 001EEB90  D0 01 00 E8 */	stfs f0, 0xe8(r1)
/* 801F1C54 001EEB94  88 03 00 7C */	lbz r0, 0x7c(r3)
/* 801F1C58 001EEB98  28 00 00 0F */	cmplwi r0, 0xf
/* 801F1C5C 001EEB9C  40 82 00 BC */	bne lbl_801F1D18
/* 801F1C60 001EEBA0  C0 22 D0 C0 */	lfs f1, $$22864@sda21(r2)
/* 801F1C64 001EEBA4  C0 1C 00 48 */	lfs f0, 0x48(r28)
/* 801F1C68 001EEBA8  EC 21 00 32 */	fmuls f1, f1, f0
/* 801F1C6C 001EEBAC  4B E9 54 99 */	bl sinf
/* 801F1C70 001EEBB0  C0 42 D0 C0 */	lfs f2, $$22864@sda21(r2)
/* 801F1C74 001EEBB4  FF E0 08 90 */	fmr f31, f1
/* 801F1C78 001EEBB8  C0 1C 00 48 */	lfs f0, 0x48(r28)
/* 801F1C7C 001EEBBC  EC 22 00 32 */	fmuls f1, f2, f0
/* 801F1C80 001EEBC0  4B E9 52 F1 */	bl cosf
/* 801F1C84 001EEBC4  D0 21 00 8C */	stfs f1, 0x8c(r1)
/* 801F1C88 001EEBC8  38 81 00 BC */	addi r4, r1, 0xbc
/* 801F1C8C 001EEBCC  C0 62 D0 AC */	lfs f3, $$22787@sda21(r2)
/* 801F1C90 001EEBD0  FC 00 F8 50 */	fneg f0, f31
/* 801F1C94 001EEBD4  C0 42 D0 B0 */	lfs f2, $$22788@sda21(r2)
/* 801F1C98 001EEBD8  D0 61 00 90 */	stfs f3, 0x90(r1)
/* 801F1C9C 001EEBDC  38 A4 00 00 */	addi r5, r4, 0
/* 801F1CA0 001EEBE0  38 61 00 8C */	addi r3, r1, 0x8c
/* 801F1CA4 001EEBE4  D3 E1 00 94 */	stfs f31, 0x94(r1)
/* 801F1CA8 001EEBE8  D0 61 00 98 */	stfs f3, 0x98(r1)
/* 801F1CAC 001EEBEC  D0 61 00 9C */	stfs f3, 0x9c(r1)
/* 801F1CB0 001EEBF0  D0 41 00 A0 */	stfs f2, 0xa0(r1)
/* 801F1CB4 001EEBF4  D0 61 00 A4 */	stfs f3, 0xa4(r1)
/* 801F1CB8 001EEBF8  D0 61 00 A8 */	stfs f3, 0xa8(r1)
/* 801F1CBC 001EEBFC  D0 01 00 AC */	stfs f0, 0xac(r1)
/* 801F1CC0 001EEC00  D0 61 00 B0 */	stfs f3, 0xb0(r1)
/* 801F1CC4 001EEC04  D0 21 00 B4 */	stfs f1, 0xb4(r1)
/* 801F1CC8 001EEC08  D0 61 00 B8 */	stfs f3, 0xb8(r1)
/* 801F1CCC 001EEC0C  4B EA 26 45 */	bl PSMTXConcat
/* 801F1CD0 001EEC10  C0 3C 00 48 */	lfs f1, 0x48(r28)
/* 801F1CD4 001EEC14  C0 1C 00 4C */	lfs f0, 0x4c(r28)
/* 801F1CD8 001EEC18  EC 01 00 2A */	fadds f0, f1, f0
/* 801F1CDC 001EEC1C  D0 1C 00 48 */	stfs f0, 0x48(r28)
/* 801F1CE0 001EEC20  C0 5C 00 48 */	lfs f2, 0x48(r28)
/* 801F1CE4 001EEC24  C0 02 D0 C4 */	lfs f0, $$22865@sda21(r2)
/* 801F1CE8 001EEC28  48 00 00 08 */	b lbl_801F1CF0
lbl_801F1CEC:
/* 801F1CEC 001EEC2C  EC 42 00 28 */	fsubs f2, f2, f0
lbl_801F1CF0:
/* 801F1CF0 001EEC30  FC 02 00 40 */	fcmpo cr0, f2, f0
/* 801F1CF4 001EEC34  4C 41 13 82 */	cror 2, 1, 2
/* 801F1CF8 001EEC38  41 82 FF F4 */	beq lbl_801F1CEC
/* 801F1CFC 001EEC3C  C0 22 D0 C4 */	lfs f1, $$22865@sda21(r2)
/* 801F1D00 001EEC40  C0 02 D0 AC */	lfs f0, $$22787@sda21(r2)
/* 801F1D04 001EEC44  48 00 00 08 */	b lbl_801F1D0C
lbl_801F1D08:
/* 801F1D08 001EEC48  EC 42 08 2A */	fadds f2, f2, f1
lbl_801F1D0C:
/* 801F1D0C 001EEC4C  FC 02 00 40 */	fcmpo cr0, f2, f0
/* 801F1D10 001EEC50  41 80 FF F8 */	blt lbl_801F1D08
/* 801F1D14 001EEC54  D0 5C 00 48 */	stfs f2, 0x48(r28)
lbl_801F1D18:
/* 801F1D18 001EEC58  80 9C 00 44 */	lwz r4, 0x44(r28)
/* 801F1D1C 001EEC5C  38 61 00 BC */	addi r3, r1, 0xbc
/* 801F1D20 001EEC60  80 84 00 04 */	lwz r4, 4(r4)
/* 801F1D24 001EEC64  38 84 00 20 */	addi r4, r4, 0x20
/* 801F1D28 001EEC68  4B EA 25 B5 */	bl PSMTXCopy
lbl_801F1D2C:
/* 801F1D2C 001EEC6C  80 7C 00 44 */	lwz r3, 0x44(r28)
/* 801F1D30 001EEC70  38 9D 00 00 */	addi r4, r29, 0
/* 801F1D34 001EEC74  38 BE 00 00 */	addi r5, r30, 0
/* 801F1D38 001EEC78  4B EE 27 A1 */	bl perform__6MActorFUlPQ26JDrama9TGraphics
/* 801F1D3C 001EEC7C  57 A0 07 39 */	rlwinm. r0, r29, 0, 0x1c, 0x1c
/* 801F1D40 001EEC80  41 82 01 70 */	beq lbl_801F1EB0
/* 801F1D44 001EEC84  4B EB 56 25 */	bl GXClearVtxDesc
/* 801F1D48 001EEC88  38 60 00 09 */	li r3, 9
/* 801F1D4C 001EEC8C  38 80 00 01 */	li r4, 1
/* 801F1D50 001EEC90  4B EB 4B 99 */	bl GXSetVtxDesc
/* 801F1D54 001EEC94  38 60 00 0B */	li r3, 0xb
/* 801F1D58 001EEC98  38 80 00 01 */	li r4, 1
/* 801F1D5C 001EEC9C  4B EB 4B 8D */	bl GXSetVtxDesc
/* 801F1D60 001EECA0  38 60 00 00 */	li r3, 0
/* 801F1D64 001EECA4  38 80 00 09 */	li r4, 9
/* 801F1D68 001EECA8  38 A0 00 01 */	li r5, 1
/* 801F1D6C 001EECAC  38 C0 00 04 */	li r6, 4
/* 801F1D70 001EECB0  38 E0 00 00 */	li r7, 0
/* 801F1D74 001EECB4  4B EB 56 41 */	bl GXSetVtxAttrFmt
/* 801F1D78 001EECB8  38 60 00 00 */	li r3, 0
/* 801F1D7C 001EECBC  38 80 00 0B */	li r4, 0xb
/* 801F1D80 001EECC0  38 A0 00 01 */	li r5, 1
/* 801F1D84 001EECC4  38 C0 00 05 */	li r6, 5
/* 801F1D88 001EECC8  38 E0 00 00 */	li r7, 0
/* 801F1D8C 001EECCC  4B EB 56 29 */	bl GXSetVtxAttrFmt
/* 801F1D90 001EECD0  38 60 00 01 */	li r3, 1
/* 801F1D94 001EECD4  4B EB 6D 9D */	bl GXSetCullMode
/* 801F1D98 001EECD8  38 60 00 01 */	li r3, 1
/* 801F1D9C 001EECDC  4B EB 82 09 */	bl GXSetNumChans
/* 801F1DA0 001EECE0  38 60 00 04 */	li r3, 4
/* 801F1DA4 001EECE4  38 80 00 00 */	li r4, 0
/* 801F1DA8 001EECE8  38 A0 00 00 */	li r5, 0
/* 801F1DAC 001EECEC  38 C0 00 00 */	li r6, 0
/* 801F1DB0 001EECF0  38 E0 00 01 */	li r7, 1
/* 801F1DB4 001EECF4  39 00 00 02 */	li r8, 2
/* 801F1DB8 001EECF8  39 20 00 02 */	li r9, 2
/* 801F1DBC 001EECFC  4B EB 82 35 */	bl GXSetChanCtrl
/* 801F1DC0 001EED00  38 60 00 05 */	li r3, 5
/* 801F1DC4 001EED04  38 80 00 00 */	li r4, 0
/* 801F1DC8 001EED08  38 A0 00 00 */	li r5, 0
/* 801F1DCC 001EED0C  38 C0 00 00 */	li r6, 0
/* 801F1DD0 001EED10  38 E0 00 00 */	li r7, 0
/* 801F1DD4 001EED14  39 00 00 00 */	li r8, 0
/* 801F1DD8 001EED18  39 20 00 02 */	li r9, 2
/* 801F1DDC 001EED1C  4B EB 82 15 */	bl GXSetChanCtrl
/* 801F1DE0 001EED20  80 02 D0 A8 */	lwz r0, $$22748@sda21(r2)
/* 801F1DE4 001EED24  38 81 00 58 */	addi r4, r1, 0x58
/* 801F1DE8 001EED28  38 60 00 04 */	li r3, 4
/* 801F1DEC 001EED2C  90 01 00 54 */	stw r0, 0x54(r1)
/* 801F1DF0 001EED30  80 01 00 54 */	lwz r0, 0x54(r1)
/* 801F1DF4 001EED34  90 01 00 58 */	stw r0, 0x58(r1)
/* 801F1DF8 001EED38  4B EB 80 45 */	bl GXSetChanMatColor
/* 801F1DFC 001EED3C  38 60 00 00 */	li r3, 0
/* 801F1E00 001EED40  4B EB 63 51 */	bl GXSetNumTexGens
/* 801F1E04 001EED44  38 60 00 00 */	li r3, 0
/* 801F1E08 001EED48  4B EB BA 05 */	bl GXSetCurrentMtx
/* 801F1E0C 001EED4C  C0 22 D0 C8 */	lfs f1, $$22866@sda21(r2)
/* 801F1E10 001EED50  38 61 00 5C */	addi r3, r1, 0x5c
/* 801F1E14 001EED54  FC 40 08 90 */	fmr f2, f1
/* 801F1E18 001EED58  FC 60 08 90 */	fmr f3, f1
/* 801F1E1C 001EED5C  4B EA 29 49 */	bl PSMTXScale
/* 801F1E20 001EED60  38 61 00 5C */	addi r3, r1, 0x5c
/* 801F1E24 001EED64  38 80 00 00 */	li r4, 0
/* 801F1E28 001EED68  4B EB B9 05 */	bl GXLoadPosMtxImm
/* 801F1E2C 001EED6C  38 61 00 5C */	addi r3, r1, 0x5c
/* 801F1E30 001EED70  38 80 00 00 */	li r4, 0
/* 801F1E34 001EED74  4B EB B9 65 */	bl GXLoadNrmMtxImm
/* 801F1E38 001EED78  38 60 00 01 */	li r3, 1
/* 801F1E3C 001EED7C  4B EB A5 D1 */	bl GXSetNumTevStages
/* 801F1E40 001EED80  38 60 00 00 */	li r3, 0
/* 801F1E44 001EED84  38 80 00 FF */	li r4, 0xff
/* 801F1E48 001EED88  38 A0 00 FF */	li r5, 0xff
/* 801F1E4C 001EED8C  38 C0 00 04 */	li r6, 4
/* 801F1E50 001EED90  4B EB A3 E1 */	bl GXSetTevOrder
/* 801F1E54 001EED94  38 60 00 00 */	li r3, 0
/* 801F1E58 001EED98  38 80 00 04 */	li r4, 4
/* 801F1E5C 001EED9C  4B EB 9B AD */	bl GXSetTevOp
/* 801F1E60 001EEDA0  38 60 00 01 */	li r3, 1
/* 801F1E64 001EEDA4  4B EB AA 89 */	bl GXSetZCompLoc
/* 801F1E68 001EEDA8  38 60 00 01 */	li r3, 1
/* 801F1E6C 001EEDAC  38 80 00 03 */	li r4, 3
/* 801F1E70 001EEDB0  38 A0 00 01 */	li r5, 1
/* 801F1E74 001EEDB4  4B EB AA 01 */	bl GXSetZMode
/* 801F1E78 001EEDB8  38 60 00 07 */	li r3, 7
/* 801F1E7C 001EEDBC  38 80 00 00 */	li r4, 0
/* 801F1E80 001EEDC0  38 A0 00 01 */	li r5, 1
/* 801F1E84 001EEDC4  38 C0 00 07 */	li r6, 7
/* 801F1E88 001EEDC8  38 E0 00 00 */	li r7, 0
/* 801F1E8C 001EEDCC  4B EB A2 D1 */	bl GXSetAlphaCompare
/* 801F1E90 001EEDD0  38 60 00 01 */	li r3, 1
/* 801F1E94 001EEDD4  38 80 00 01 */	li r4, 1
/* 801F1E98 001EEDD8  38 A0 00 00 */	li r5, 0
/* 801F1E9C 001EEDDC  38 C0 00 05 */	li r6, 5
/* 801F1EA0 001EEDE0  4B EB A8 51 */	bl GXSetBlendMode
/* 801F1EA4 001EEDE4  38 60 00 08 */	li r3, 8
/* 801F1EA8 001EEDE8  38 80 00 10 */	li r4, 0x10
/* 801F1EAC 001EEDEC  4B EB AC DD */	bl GXDrawSphere
lbl_801F1EB0:
/* 801F1EB0 001EEDF0  80 01 01 3C */	lwz r0, 0x13c(r1)
/* 801F1EB4 001EEDF4  CB E1 01 30 */	lfd f31, 0x130(r1)
/* 801F1EB8 001EEDF8  83 E1 01 2C */	lwz r31, 0x12c(r1)
/* 801F1EBC 001EEDFC  7C 08 03 A6 */	mtlr r0
/* 801F1EC0 001EEE00  83 C1 01 28 */	lwz r30, 0x128(r1)
/* 801F1EC4 001EEE04  83 A1 01 24 */	lwz r29, 0x124(r1)
/* 801F1EC8 001EEE08  83 81 01 20 */	lwz r28, 0x120(r1)
/* 801F1ECC 001EEE0C  38 21 01 38 */	addi r1, r1, 0x138
/* 801F1ED0 001EEE10  4E 80 00 20 */	blr 

.global __sinit_Sky_cpp
__sinit_Sky_cpp:
/* 801F1ED4 001EEE14  7C 08 02 A6 */	mflr r0
/* 801F1ED8 001EEE18  3C 60 80 40 */	lis r3, $$22867@ha
/* 801F1EDC 001EEE1C  90 01 00 04 */	stw r0, 4(r1)
/* 801F1EE0 001EEE20  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 801F1EE4 001EEE24  93 E1 00 0C */	stw r31, 0xc(r1)
/* 801F1EE8 001EEE28  3B E3 86 48 */	addi r31, r3, $$22867@l
/* 801F1EEC 001EEE2C  88 0D 97 CC */	lbz r0, __init__smList__15JALList$$05MSBgm$$1@sda21(r13)
/* 801F1EF0 001EEE30  7C 00 07 75 */	extsb. r0, r0
/* 801F1EF4 001EEE34  40 82 00 28 */	bne lbl_801F1F1C
/* 801F1EF8 001EEE38  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1@sda21
/* 801F1EFC 001EEE3C  4B E1 CD 39 */	bl initiate__10JSUPtrListFv
/* 801F1F00 001EEE40  3C 60 80 0E */	lis r3, __dt__15JSUList$$05MSBgm$$1Fv@ha
/* 801F1F04 001EEE44  38 83 6A 44 */	addi r4, r3, __dt__15JSUList$$05MSBgm$$1Fv@l
/* 801F1F08 001EEE48  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1@sda21
/* 801F1F0C 001EEE4C  38 BF 00 00 */	addi r5, r31, 0
/* 801F1F10 001EEE50  4B E9 08 19 */	bl __register_global_object
/* 801F1F14 001EEE54  38 00 00 01 */	li r0, 1
/* 801F1F18 001EEE58  98 0D 97 CC */	stb r0, __init__smList__15JALList$$05MSBgm$$1@sda21(r13)
lbl_801F1F1C:
/* 801F1F1C 001EEE5C  88 0D 97 CD */	lbz r0, __init__smList__24JALList$$013MSSetSoundGrp$$1@sda21(r13)
/* 801F1F20 001EEE60  7C 00 07 75 */	extsb. r0, r0
/* 801F1F24 001EEE64  40 82 00 28 */	bne lbl_801F1F4C
/* 801F1F28 001EEE68  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1@sda21
/* 801F1F2C 001EEE6C  4B E1 CD 09 */	bl initiate__10JSUPtrListFv
/* 801F1F30 001EEE70  3C 60 80 0E */	lis r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@ha
/* 801F1F34 001EEE74  38 83 69 EC */	addi r4, r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@l
/* 801F1F38 001EEE78  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1@sda21
/* 801F1F3C 001EEE7C  38 BF 00 0C */	addi r5, r31, 0xc
/* 801F1F40 001EEE80  4B E9 07 E9 */	bl __register_global_object
/* 801F1F44 001EEE84  38 00 00 01 */	li r0, 1
/* 801F1F48 001EEE88  98 0D 97 CD */	stb r0, __init__smList__24JALList$$013MSSetSoundGrp$$1@sda21(r13)
lbl_801F1F4C:
/* 801F1F4C 001EEE8C  88 0D 97 CE */	lbz r0, __init__smList__21JALList$$010MSSetSound$$1@sda21(r13)
/* 801F1F50 001EEE90  7C 00 07 75 */	extsb. r0, r0
/* 801F1F54 001EEE94  40 82 00 28 */	bne lbl_801F1F7C
/* 801F1F58 001EEE98  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1@sda21
/* 801F1F5C 001EEE9C  4B E1 CC D9 */	bl initiate__10JSUPtrListFv
/* 801F1F60 001EEEA0  3C 60 80 0E */	lis r3, __dt__21JSUList$$010MSSetSound$$1Fv@ha
/* 801F1F64 001EEEA4  38 83 69 94 */	addi r4, r3, __dt__21JSUList$$010MSSetSound$$1Fv@l
/* 801F1F68 001EEEA8  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1@sda21
/* 801F1F6C 001EEEAC  38 BF 00 18 */	addi r5, r31, 0x18
/* 801F1F70 001EEEB0  4B E9 07 B9 */	bl __register_global_object
/* 801F1F74 001EEEB4  38 00 00 01 */	li r0, 1
/* 801F1F78 001EEEB8  98 0D 97 CE */	stb r0, __init__smList__21JALList$$010MSSetSound$$1@sda21(r13)
lbl_801F1F7C:
/* 801F1F7C 001EEEBC  88 0D 8F 8C */	lbz r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1@sda21(r13)
/* 801F1F80 001EEEC0  7C 00 07 75 */	extsb. r0, r0
/* 801F1F84 001EEEC4  40 82 00 28 */	bne lbl_801F1FAC
/* 801F1F88 001EEEC8  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1@sda21
/* 801F1F8C 001EEECC  4B E1 CC A9 */	bl initiate__10JSUPtrListFv
/* 801F1F90 001EEED0  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@ha
/* 801F1F94 001EEED4  38 83 A8 0C */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@l
/* 801F1F98 001EEED8  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1@sda21
/* 801F1F9C 001EEEDC  38 BF 00 24 */	addi r5, r31, 0x24
/* 801F1FA0 001EEEE0  4B E9 07 89 */	bl __register_global_object
/* 801F1FA4 001EEEE4  38 00 00 01 */	li r0, 1
/* 801F1FA8 001EEEE8  98 0D 8F 8C */	stb r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1@sda21(r13)
lbl_801F1FAC:
/* 801F1FAC 001EEEEC  88 0D 8F 8D */	lbz r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1@sda21(r13)
/* 801F1FB0 001EEEF0  7C 00 07 75 */	extsb. r0, r0
/* 801F1FB4 001EEEF4  40 82 00 28 */	bne lbl_801F1FDC
/* 801F1FB8 001EEEF8  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1@sda21
/* 801F1FBC 001EEEFC  4B E1 CC 79 */	bl initiate__10JSUPtrListFv
/* 801F1FC0 001EEF00  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@ha
/* 801F1FC4 001EEF04  38 83 A7 B4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@l
/* 801F1FC8 001EEF08  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1@sda21
/* 801F1FCC 001EEF0C  38 BF 00 30 */	addi r5, r31, 0x30
/* 801F1FD0 001EEF10  4B E9 07 59 */	bl __register_global_object
/* 801F1FD4 001EEF14  38 00 00 01 */	li r0, 1
/* 801F1FD8 001EEF18  98 0D 8F 8D */	stb r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1@sda21(r13)
lbl_801F1FDC:
/* 801F1FDC 001EEF1C  88 0D 8F 8E */	lbz r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1@sda21(r13)
/* 801F1FE0 001EEF20  7C 00 07 75 */	extsb. r0, r0
/* 801F1FE4 001EEF24  40 82 00 28 */	bne lbl_801F200C
/* 801F1FE8 001EEF28  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1@sda21
/* 801F1FEC 001EEF2C  4B E1 CC 49 */	bl initiate__10JSUPtrListFv
/* 801F1FF0 001EEF30  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@ha
/* 801F1FF4 001EEF34  38 83 A7 5C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@l
/* 801F1FF8 001EEF38  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1@sda21
/* 801F1FFC 001EEF3C  38 BF 00 3C */	addi r5, r31, 0x3c
/* 801F2000 001EEF40  4B E9 07 29 */	bl __register_global_object
/* 801F2004 001EEF44  38 00 00 01 */	li r0, 1
/* 801F2008 001EEF48  98 0D 8F 8E */	stb r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1@sda21(r13)
lbl_801F200C:
/* 801F200C 001EEF4C  88 0D 8F 8F */	lbz r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1@sda21(r13)
/* 801F2010 001EEF50  7C 00 07 75 */	extsb. r0, r0
/* 801F2014 001EEF54  40 82 00 28 */	bne lbl_801F203C
/* 801F2018 001EEF58  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1@sda21
/* 801F201C 001EEF5C  4B E1 CC 19 */	bl initiate__10JSUPtrListFv
/* 801F2020 001EEF60  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@ha
/* 801F2024 001EEF64  38 83 A7 04 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@l
/* 801F2028 001EEF68  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1@sda21
/* 801F202C 001EEF6C  38 BF 00 48 */	addi r5, r31, 0x48
/* 801F2030 001EEF70  4B E9 06 F9 */	bl __register_global_object
/* 801F2034 001EEF74  38 00 00 01 */	li r0, 1
/* 801F2038 001EEF78  98 0D 8F 8F */	stb r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1@sda21(r13)
lbl_801F203C:
/* 801F203C 001EEF7C  88 0D 8F 90 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1@sda21(r13)
/* 801F2040 001EEF80  7C 00 07 75 */	extsb. r0, r0
/* 801F2044 001EEF84  40 82 00 28 */	bne lbl_801F206C
/* 801F2048 001EEF88  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1@sda21
/* 801F204C 001EEF8C  4B E1 CB E9 */	bl initiate__10JSUPtrListFv
/* 801F2050 001EEF90  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@ha
/* 801F2054 001EEF94  38 83 A6 AC */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@l
/* 801F2058 001EEF98  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1@sda21
/* 801F205C 001EEF9C  38 BF 00 54 */	addi r5, r31, 0x54
/* 801F2060 001EEFA0  4B E9 06 C9 */	bl __register_global_object
/* 801F2064 001EEFA4  38 00 00 01 */	li r0, 1
/* 801F2068 001EEFA8  98 0D 8F 90 */	stb r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1@sda21(r13)
lbl_801F206C:
/* 801F206C 001EEFAC  88 0D 8F 91 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1@sda21(r13)
/* 801F2070 001EEFB0  7C 00 07 75 */	extsb. r0, r0
/* 801F2074 001EEFB4  40 82 00 28 */	bne lbl_801F209C
/* 801F2078 001EEFB8  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1@sda21
/* 801F207C 001EEFBC  4B E1 CB B9 */	bl initiate__10JSUPtrListFv
/* 801F2080 001EEFC0  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@ha
/* 801F2084 001EEFC4  38 83 A6 54 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@l
/* 801F2088 001EEFC8  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1@sda21
/* 801F208C 001EEFCC  38 BF 00 60 */	addi r5, r31, 0x60
/* 801F2090 001EEFD0  4B E9 06 99 */	bl __register_global_object
/* 801F2094 001EEFD4  38 00 00 01 */	li r0, 1
/* 801F2098 001EEFD8  98 0D 8F 91 */	stb r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1@sda21(r13)
lbl_801F209C:
/* 801F209C 001EEFDC  88 0D 8F 92 */	lbz r0, __init__smList__26JALList$$015JALSeModEffDist$$1@sda21(r13)
/* 801F20A0 001EEFE0  7C 00 07 75 */	extsb. r0, r0
/* 801F20A4 001EEFE4  40 82 00 28 */	bne lbl_801F20CC
/* 801F20A8 001EEFE8  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1@sda21
/* 801F20AC 001EEFEC  4B E1 CB 89 */	bl initiate__10JSUPtrListFv
/* 801F20B0 001EEFF0  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@ha
/* 801F20B4 001EEFF4  38 83 A5 FC */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@l
/* 801F20B8 001EEFF8  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1@sda21
/* 801F20BC 001EEFFC  38 BF 00 6C */	addi r5, r31, 0x6c
/* 801F20C0 001EF000  4B E9 06 69 */	bl __register_global_object
/* 801F20C4 001EF004  38 00 00 01 */	li r0, 1
/* 801F20C8 001EF008  98 0D 8F 92 */	stb r0, __init__smList__26JALList$$015JALSeModEffDist$$1@sda21(r13)
lbl_801F20CC:
/* 801F20CC 001EF00C  88 0D 8F 93 */	lbz r0, __init__smList__26JALList$$015JALSeModPitDist$$1@sda21(r13)
/* 801F20D0 001EF010  7C 00 07 75 */	extsb. r0, r0
/* 801F20D4 001EF014  40 82 00 28 */	bne lbl_801F20FC
/* 801F20D8 001EF018  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1@sda21
/* 801F20DC 001EF01C  4B E1 CB 59 */	bl initiate__10JSUPtrListFv
/* 801F20E0 001EF020  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@ha
/* 801F20E4 001EF024  38 83 A5 A4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@l
/* 801F20E8 001EF028  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1@sda21
/* 801F20EC 001EF02C  38 BF 00 78 */	addi r5, r31, 0x78
/* 801F20F0 001EF030  4B E9 06 39 */	bl __register_global_object
/* 801F20F4 001EF034  38 00 00 01 */	li r0, 1
/* 801F20F8 001EF038  98 0D 8F 93 */	stb r0, __init__smList__26JALList$$015JALSeModPitDist$$1@sda21(r13)
lbl_801F20FC:
/* 801F20FC 001EF03C  88 0D 8F 94 */	lbz r0, __init__smList__26JALList$$015JALSeModVolDist$$1@sda21(r13)
/* 801F2100 001EF040  7C 00 07 75 */	extsb. r0, r0
/* 801F2104 001EF044  40 82 00 28 */	bne lbl_801F212C
/* 801F2108 001EF048  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1@sda21
/* 801F210C 001EF04C  4B E1 CB 29 */	bl initiate__10JSUPtrListFv
/* 801F2110 001EF050  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@ha
/* 801F2114 001EF054  38 83 A5 4C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@l
/* 801F2118 001EF058  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1@sda21
/* 801F211C 001EF05C  38 BF 00 84 */	addi r5, r31, 0x84
/* 801F2120 001EF060  4B E9 06 09 */	bl __register_global_object
/* 801F2124 001EF064  38 00 00 01 */	li r0, 1
/* 801F2128 001EF068  98 0D 8F 94 */	stb r0, __init__smList__26JALList$$015JALSeModVolDist$$1@sda21(r13)
lbl_801F212C:
/* 801F212C 001EF06C  88 0D 8F 95 */	lbz r0, __init__smList__26JALList$$015JALSeModEffFunk$$1@sda21(r13)
/* 801F2130 001EF070  7C 00 07 75 */	extsb. r0, r0
/* 801F2134 001EF074  40 82 00 28 */	bne lbl_801F215C
/* 801F2138 001EF078  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1@sda21
/* 801F213C 001EF07C  4B E1 CA F9 */	bl initiate__10JSUPtrListFv
/* 801F2140 001EF080  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@ha
/* 801F2144 001EF084  38 83 A4 F4 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@l
/* 801F2148 001EF088  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1@sda21
/* 801F214C 001EF08C  38 BF 00 90 */	addi r5, r31, 0x90
/* 801F2150 001EF090  4B E9 05 D9 */	bl __register_global_object
/* 801F2154 001EF094  38 00 00 01 */	li r0, 1
/* 801F2158 001EF098  98 0D 8F 95 */	stb r0, __init__smList__26JALList$$015JALSeModEffFunk$$1@sda21(r13)
lbl_801F215C:
/* 801F215C 001EF09C  88 0D 8F 96 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFunk$$1@sda21(r13)
/* 801F2160 001EF0A0  7C 00 07 75 */	extsb. r0, r0
/* 801F2164 001EF0A4  40 82 00 28 */	bne lbl_801F218C
/* 801F2168 001EF0A8  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1@sda21
/* 801F216C 001EF0AC  4B E1 CA C9 */	bl initiate__10JSUPtrListFv
/* 801F2170 001EF0B0  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@ha
/* 801F2174 001EF0B4  38 83 A4 9C */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@l
/* 801F2178 001EF0B8  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1@sda21
/* 801F217C 001EF0BC  38 BF 00 9C */	addi r5, r31, 0x9c
/* 801F2180 001EF0C0  4B E9 05 A9 */	bl __register_global_object
/* 801F2184 001EF0C4  38 00 00 01 */	li r0, 1
/* 801F2188 001EF0C8  98 0D 8F 96 */	stb r0, __init__smList__26JALList$$015JALSeModPitFunk$$1@sda21(r13)
lbl_801F218C:
/* 801F218C 001EF0CC  88 0D 8F 97 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFunk$$1@sda21(r13)
/* 801F2190 001EF0D0  7C 00 07 75 */	extsb. r0, r0
/* 801F2194 001EF0D4  40 82 00 28 */	bne lbl_801F21BC
/* 801F2198 001EF0D8  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1@sda21
/* 801F219C 001EF0DC  4B E1 CA 99 */	bl initiate__10JSUPtrListFv
/* 801F21A0 001EF0E0  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@ha
/* 801F21A4 001EF0E4  38 83 A4 44 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@l
/* 801F21A8 001EF0E8  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1@sda21
/* 801F21AC 001EF0EC  38 BF 00 A8 */	addi r5, r31, 0xa8
/* 801F21B0 001EF0F0  4B E9 05 79 */	bl __register_global_object
/* 801F21B4 001EF0F4  38 00 00 01 */	li r0, 1
/* 801F21B8 001EF0F8  98 0D 8F 97 */	stb r0, __init__smList__26JALList$$015JALSeModVolFunk$$1@sda21(r13)
lbl_801F21BC:
/* 801F21BC 001EF0FC  80 01 00 14 */	lwz r0, 0x14(r1)
/* 801F21C0 001EF100  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 801F21C4 001EF104  38 21 00 10 */	addi r1, r1, 0x10
/* 801F21C8 001EF108  7C 08 03 A6 */	mtlr r0
/* 801F21CC 001EF10C  4E 80 00 20 */	blr 

.global $$232$$2__dt__4TSkyFv
$$232$$2__dt__4TSkyFv:
/* 801F21D0 001EF110  38 63 FF E0 */	addi r3, r3, -32
/* 801F21D4 001EF114  4B FF F7 8C */	b __dt__4TSkyFv

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
	.incbin "baserom.dol", 0x36CDFC, 0x4

.section .rodata, "wa"  # 0x8036FFA0 - 0x803A8380
$$22827:
	.incbin "baserom.dol", 0x38FA98, 0x18

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__4TSky
__vt__4TSky:
  .4byte 0
  .4byte 0
  .4byte __dt__4TSkyFv
  .4byte getType__Q26JDrama6TActorCFv
  .4byte load__4TSkyFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__Q26JDrama8TNameRefFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc
  .4byte perform__4TSkyFUlPQ26JDrama9TGraphics
  .4byte 0
  .4byte 0
  .4byte $$232$$2__dt__4TSkyFv
  .4byte JSGFGetType__Q26JStage6TActorCFv
  .4byte JSGGetName__Q26JStage7TObjectCFv
  .4byte JSGGetFlag__Q26JStage7TObjectCFv
  .4byte JSGSetFlag__Q26JStage7TObjectFUl
  .4byte JSGGetData__Q26JStage7TObjectCFUlPvUl
  .4byte JSGSetData__Q26JStage7TObjectFUlPCvUl
  .4byte JSGGetParent__Q26JStage7TObjectCFPPQ26JStage7TObjectPUl
  .4byte JSGSetParent__Q26JStage7TObjectFPQ26JStage7TObjectUl
  .4byte JSGSetRelation__Q26JStage7TObjectFbPQ26JStage7TObjectUl
  .4byte $$232$$2JSGGetTranslation__Q26JDrama6TActorCFP3Vec
  .4byte $$232$$2JSGSetTranslation__Q26JDrama6TActorFRC3Vec
  .4byte $$232$$2JSGGetScaling__Q26JDrama6TActorCFP3Vec
  .4byte $$232$$2JSGSetScaling__Q26JDrama6TActorFRC3Vec
  .4byte $$232$$2JSGGetRotation__Q26JDrama6TActorCFP3Vec
  .4byte $$232$$2JSGSetRotation__Q26JDrama6TActorFRC3Vec
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

.section .sdata2, "wa"  # 0x8040B460 - 0x80414020
$$22748:
	.incbin "baserom.dol", 0x3E8DA8, 0x4
$$22787:
	.incbin "baserom.dol", 0x3E8DAC, 0x4
$$22788:
	.incbin "baserom.dol", 0x3E8DB0, 0x4
$$22789:
	.incbin "baserom.dol", 0x3E8DB4, 0x4
$$22790:
	.incbin "baserom.dol", 0x3E8DB8, 0x4
$$22828:
	.incbin "baserom.dol", 0x3E8DBC, 0x4
$$22864:
	.incbin "baserom.dol", 0x3E8DC0, 0x4
$$22865:
	.incbin "baserom.dol", 0x3E8DC4, 0x4
$$22866:
	.incbin "baserom.dol", 0x3E8DC8, 0x8

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
$$22867:
	.skip 0xC
$$22868:
	.skip 0xC
$$22869:
	.skip 0xC
$$22870:
	.skip 0xC
$$22871:
	.skip 0xC
$$22872:
	.skip 0xC
$$22873:
	.skip 0xC
$$22874:
	.skip 0xC
$$22875:
	.skip 0xC
$$22876:
	.skip 0xC
$$22877:
	.skip 0xC
$$22878:
	.skip 0xC
$$22879:
	.skip 0xC
$$22880:
	.skip 0xC
$$22881:
	.skip 0x10

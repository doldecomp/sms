.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __ct__13TPollutionPosFv
__ct__13TPollutionPosFv:
/* 801F08B0 001ED7F0  38 00 00 00 */	li r0, 0
/* 801F08B4 001ED7F4  90 03 00 00 */	stw r0, 0(r3)
/* 801F08B8 001ED7F8  90 03 00 04 */	stw r0, 4(r3)
/* 801F08BC 001ED7FC  90 03 00 08 */	stw r0, 8(r3)
/* 801F08C0 001ED800  90 03 00 0C */	stw r0, 0xc(r3)
/* 801F08C4 001ED804  C0 02 D0 80 */	lfs f0, $$22390@sda21(r2)
/* 801F08C8 001ED808  D0 03 00 10 */	stfs f0, 0x10(r3)
/* 801F08CC 001ED80C  D0 03 00 14 */	stfs f0, 0x14(r3)
/* 801F08D0 001ED810  D0 03 00 18 */	stfs f0, 0x18(r3)
/* 801F08D4 001ED814  90 03 00 1C */	stw r0, 0x1c(r3)
/* 801F08D8 001ED818  90 03 00 20 */	stw r0, 0x20(r3)
/* 801F08DC 001ED81C  4E 80 00 20 */	blr 

.global init__13TPollutionPosFP15TPollutionLayerffPUcii
init__13TPollutionPosFP15TPollutionLayerffPUcii:
/* 801F08E0 001ED820  90 83 00 20 */	stw r4, 0x20(r3)
/* 801F08E4 001ED824  38 80 00 01 */	li r4, 1
/* 801F08E8 001ED828  90 A3 00 1C */	stw r5, 0x1c(r3)
/* 801F08EC 001ED82C  D0 23 00 10 */	stfs f1, 0x10(r3)
/* 801F08F0 001ED830  D0 43 00 14 */	stfs f2, 0x14(r3)
/* 801F08F4 001ED834  C0 22 D0 84 */	lfs f1, $$22392@sda21(r2)
/* 801F08F8 001ED838  C0 03 00 14 */	lfs f0, 0x14(r3)
/* 801F08FC 001ED83C  EC 01 00 24 */	fdivs f0, f1, f0
/* 801F0900 001ED840  D0 03 00 18 */	stfs f0, 0x18(r3)
/* 801F0904 001ED844  90 C3 00 08 */	stw r6, 8(r3)
/* 801F0908 001ED848  90 E3 00 0C */	stw r7, 0xc(r3)
/* 801F090C 001ED84C  80 03 00 08 */	lwz r0, 8(r3)
/* 801F0910 001ED850  7C 80 00 30 */	slw r0, r4, r0
/* 801F0914 001ED854  90 03 00 00 */	stw r0, 0(r3)
/* 801F0918 001ED858  80 03 00 0C */	lwz r0, 0xc(r3)
/* 801F091C 001ED85C  7C 80 00 30 */	slw r0, r4, r0
/* 801F0920 001ED860  90 03 00 04 */	stw r0, 4(r3)
/* 801F0924 001ED864  4E 80 00 20 */	blr 

.global worldToTexSize__13TPollutionPosCFf
worldToTexSize__13TPollutionPosCFf:
/* 801F0928 001ED868  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 801F092C 001ED86C  C0 03 00 18 */	lfs f0, 0x18(r3)
/* 801F0930 001ED870  EC 01 00 32 */	fmuls f0, f1, f0
/* 801F0934 001ED874  FC 00 00 1E */	fctiwz f0, f0
/* 801F0938 001ED878  D8 01 00 10 */	stfd f0, 0x10(r1)
/* 801F093C 001ED87C  80 61 00 14 */	lwz r3, 0x14(r1)
/* 801F0940 001ED880  38 21 00 18 */	addi r1, r1, 0x18
/* 801F0944 001ED884  4E 80 00 20 */	blr 

.global worldToDepth__13TPollutionPosCFf
worldToDepth__13TPollutionPosCFf:
/* 801F0948 001ED888  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 801F094C 001ED88C  C0 43 00 10 */	lfs f2, 0x10(r3)
/* 801F0950 001ED890  C0 02 D0 88 */	lfs f0, $$22396@sda21(r2)
/* 801F0954 001ED894  EC 21 10 28 */	fsubs f1, f1, f2
/* 801F0958 001ED898  EC 21 00 32 */	fmuls f1, f1, f0
/* 801F095C 001ED89C  FC 00 08 1E */	fctiwz f0, f1
/* 801F0960 001ED8A0  D8 01 00 10 */	stfd f0, 0x10(r1)
/* 801F0964 001ED8A4  80 61 00 14 */	lwz r3, 0x14(r1)
/* 801F0968 001ED8A8  38 21 00 18 */	addi r1, r1, 0x18
/* 801F096C 001ED8AC  4E 80 00 20 */	blr 

.global isProhibit__13TPollutionPosCFii
isProhibit__13TPollutionPosCFii:
/* 801F0970 001ED8B0  2C 04 00 00 */	cmpwi r4, 0
/* 801F0974 001ED8B4  41 80 00 24 */	blt lbl_801F0998
/* 801F0978 001ED8B8  80 03 00 00 */	lwz r0, 0(r3)
/* 801F097C 001ED8BC  7C 00 20 00 */	cmpw r0, r4
/* 801F0980 001ED8C0  40 81 00 18 */	ble lbl_801F0998
/* 801F0984 001ED8C4  2C 05 00 00 */	cmpwi r5, 0
/* 801F0988 001ED8C8  41 80 00 10 */	blt lbl_801F0998
/* 801F098C 001ED8CC  80 03 00 04 */	lwz r0, 4(r3)
/* 801F0990 001ED8D0  7C 00 28 00 */	cmpw r0, r5
/* 801F0994 001ED8D4  41 81 00 0C */	bgt lbl_801F09A0
lbl_801F0998:
/* 801F0998 001ED8D8  38 60 00 01 */	li r3, 1
/* 801F099C 001ED8DC  4E 80 00 20 */	blr 
lbl_801F09A0:
/* 801F09A0 001ED8E0  80 C3 00 08 */	lwz r6, 8(r3)
/* 801F09A4 001ED8E4  7C 87 1E 70 */	srawi r7, r4, 3
/* 801F09A8 001ED8E8  81 03 00 1C */	lwz r8, 0x1c(r3)
/* 801F09AC 001ED8EC  7C A3 16 70 */	srawi r3, r5, 2
/* 801F09B0 001ED8F0  38 06 FF FD */	addi r0, r6, -3
/* 801F09B4 001ED8F4  7C 60 00 30 */	slw r0, r3, r0
/* 801F09B8 001ED8F8  7C 07 02 14 */	add r0, r7, r0
/* 801F09BC 001ED8FC  54 83 07 7E */	clrlwi r3, r4, 0x1d
/* 801F09C0 001ED900  54 00 28 34 */	slwi r0, r0, 5
/* 801F09C4 001ED904  7C 00 1A 14 */	add r0, r0, r3
/* 801F09C8 001ED908  54 A3 1E F8 */	rlwinm r3, r5, 3, 0x1b, 0x1c
/* 801F09CC 001ED90C  7C 00 42 14 */	add r0, r0, r8
/* 801F09D0 001ED910  7C 03 00 AE */	lbzx r0, r3, r0
/* 801F09D4 001ED914  2C 00 00 FF */	cmpwi r0, 0xff
/* 801F09D8 001ED918  40 80 00 0C */	bge lbl_801F09E4
/* 801F09DC 001ED91C  38 60 00 00 */	li r3, 0
/* 801F09E0 001ED920  4E 80 00 20 */	blr 
lbl_801F09E4:
/* 801F09E4 001ED924  38 60 00 01 */	li r3, 1
/* 801F09E8 001ED928  4E 80 00 20 */	blr 

.global isSame__13TPollutionPosCFiif
isSame__13TPollutionPosCFiif:
/* 801F09EC 001ED92C  2C 04 00 00 */	cmpwi r4, 0
/* 801F09F0 001ED930  94 21 FF A8 */	stwu r1, -0x58(r1)
/* 801F09F4 001ED934  41 80 00 24 */	blt lbl_801F0A18
/* 801F09F8 001ED938  80 03 00 00 */	lwz r0, 0(r3)
/* 801F09FC 001ED93C  7C 00 20 00 */	cmpw r0, r4
/* 801F0A00 001ED940  40 81 00 18 */	ble lbl_801F0A18
/* 801F0A04 001ED944  2C 05 00 00 */	cmpwi r5, 0
/* 801F0A08 001ED948  41 80 00 10 */	blt lbl_801F0A18
/* 801F0A0C 001ED94C  80 03 00 04 */	lwz r0, 4(r3)
/* 801F0A10 001ED950  7C 00 28 00 */	cmpw r0, r5
/* 801F0A14 001ED954  41 81 00 0C */	bgt lbl_801F0A20
lbl_801F0A18:
/* 801F0A18 001ED958  38 00 00 00 */	li r0, 0
/* 801F0A1C 001ED95C  48 00 00 08 */	b lbl_801F0A24
lbl_801F0A20:
/* 801F0A20 001ED960  38 00 00 01 */	li r0, 1
lbl_801F0A24:
/* 801F0A24 001ED964  54 00 06 3F */	clrlwi. r0, r0, 0x18
/* 801F0A28 001ED968  40 82 00 0C */	bne lbl_801F0A34
/* 801F0A2C 001ED96C  38 60 00 00 */	li r3, 0
/* 801F0A30 001ED970  48 00 00 88 */	b lbl_801F0AB8
lbl_801F0A34:
/* 801F0A34 001ED974  80 C3 00 08 */	lwz r6, 8(r3)
/* 801F0A38 001ED978  7C 88 1E 70 */	srawi r8, r4, 3
/* 801F0A3C 001ED97C  7C A7 16 70 */	srawi r7, r5, 2
/* 801F0A40 001ED980  81 23 00 1C */	lwz r9, 0x1c(r3)
/* 801F0A44 001ED984  38 06 FF FD */	addi r0, r6, -3
/* 801F0A48 001ED988  7C E0 00 30 */	slw r0, r7, r0
/* 801F0A4C 001ED98C  7C 08 02 14 */	add r0, r8, r0
/* 801F0A50 001ED990  54 84 07 7E */	clrlwi r4, r4, 0x1d
/* 801F0A54 001ED994  54 00 28 34 */	slwi r0, r0, 5
/* 801F0A58 001ED998  7C 00 22 14 */	add r0, r0, r4
/* 801F0A5C 001ED99C  54 A4 1E F8 */	rlwinm r4, r5, 3, 0x1b, 0x1c
/* 801F0A60 001ED9A0  7C 00 4A 14 */	add r0, r0, r9
/* 801F0A64 001ED9A4  7C 84 00 AE */	lbzx r4, r4, r0
/* 801F0A68 001ED9A8  2C 04 00 FF */	cmpwi r4, 0xff
/* 801F0A6C 001ED9AC  40 80 00 48 */	bge lbl_801F0AB4
/* 801F0A70 001ED9B0  C0 03 00 10 */	lfs f0, 0x10(r3)
/* 801F0A74 001ED9B4  80 63 00 20 */	lwz r3, 0x20(r3)
/* 801F0A78 001ED9B8  EC 21 00 28 */	fsubs f1, f1, f0
/* 801F0A7C 001ED9BC  C0 02 D0 88 */	lfs f0, $$22396@sda21(r2)
/* 801F0A80 001ED9C0  80 A3 00 48 */	lwz r5, 0x48(r3)
/* 801F0A84 001ED9C4  EC 21 00 32 */	fmuls f1, f1, f0
/* 801F0A88 001ED9C8  7C 05 20 50 */	subf r0, r5, r4
/* 801F0A8C 001ED9CC  FC 00 08 1E */	fctiwz f0, f1
/* 801F0A90 001ED9D0  D8 01 00 50 */	stfd f0, 0x50(r1)
/* 801F0A94 001ED9D4  80 61 00 54 */	lwz r3, 0x54(r1)
/* 801F0A98 001ED9D8  7C 00 18 00 */	cmpw r0, r3
/* 801F0A9C 001ED9DC  41 81 00 18 */	bgt lbl_801F0AB4
/* 801F0AA0 001ED9E0  7C 04 2A 14 */	add r0, r4, r5
/* 801F0AA4 001ED9E4  7C 03 00 00 */	cmpw r3, r0
/* 801F0AA8 001ED9E8  41 81 00 0C */	bgt lbl_801F0AB4
/* 801F0AAC 001ED9EC  38 60 00 01 */	li r3, 1
/* 801F0AB0 001ED9F0  48 00 00 08 */	b lbl_801F0AB8
lbl_801F0AB4:
/* 801F0AB4 001ED9F4  38 60 00 00 */	li r3, 0
lbl_801F0AB8:
/* 801F0AB8 001ED9F8  38 21 00 58 */	addi r1, r1, 0x58
/* 801F0ABC 001ED9FC  4E 80 00 20 */	blr 

.global getDepthWorld__13TPollutionPosCFii
getDepthWorld__13TPollutionPosCFii:
/* 801F0AC0 001EDA00  94 21 FF C8 */	stwu r1, -0x38(r1)
/* 801F0AC4 001EDA04  7C 88 1E 70 */	srawi r8, r4, 3
/* 801F0AC8 001EDA08  7C A7 16 70 */	srawi r7, r5, 2
/* 801F0ACC 001EDA0C  80 C3 00 08 */	lwz r6, 8(r3)
/* 801F0AD0 001EDA10  54 84 07 7E */	clrlwi r4, r4, 0x1d
/* 801F0AD4 001EDA14  81 23 00 1C */	lwz r9, 0x1c(r3)
/* 801F0AD8 001EDA18  38 06 FF FD */	addi r0, r6, -3
/* 801F0ADC 001EDA1C  7C E0 00 30 */	slw r0, r7, r0
/* 801F0AE0 001EDA20  7C 08 02 14 */	add r0, r8, r0
/* 801F0AE4 001EDA24  54 00 28 34 */	slwi r0, r0, 5
/* 801F0AE8 001EDA28  7C 00 22 14 */	add r0, r0, r4
/* 801F0AEC 001EDA2C  54 A4 1E F8 */	rlwinm r4, r5, 3, 0x1b, 0x1c
/* 801F0AF0 001EDA30  7C 00 4A 14 */	add r0, r0, r9
/* 801F0AF4 001EDA34  7C 04 00 AE */	lbzx r0, r4, r0
/* 801F0AF8 001EDA38  2C 00 00 FF */	cmpwi r0, 0xff
/* 801F0AFC 001EDA3C  40 80 00 34 */	bge lbl_801F0B30
/* 801F0B00 001EDA40  6C 00 80 00 */	xoris r0, r0, 0x8000
/* 801F0B04 001EDA44  C8 42 D0 90 */	lfd f2, $$22462@sda21(r2)
/* 801F0B08 001EDA48  90 01 00 34 */	stw r0, 0x34(r1)
/* 801F0B0C 001EDA4C  3C 00 43 30 */	lis r0, 0x4330
/* 801F0B10 001EDA50  C0 03 00 14 */	lfs f0, 0x14(r3)
/* 801F0B14 001EDA54  90 01 00 30 */	stw r0, 0x30(r1)
/* 801F0B18 001EDA58  C0 63 00 10 */	lfs f3, 0x10(r3)
/* 801F0B1C 001EDA5C  C8 21 00 30 */	lfd f1, 0x30(r1)
/* 801F0B20 001EDA60  EC 21 10 28 */	fsubs f1, f1, f2
/* 801F0B24 001EDA64  EC 01 00 32 */	fmuls f0, f1, f0
/* 801F0B28 001EDA68  EC 23 00 2A */	fadds f1, f3, f0
/* 801F0B2C 001EDA6C  48 00 00 08 */	b lbl_801F0B34
lbl_801F0B30:
/* 801F0B30 001EDA70  C0 22 D0 8C */	lfs f1, $$22460@sda21(r2)
lbl_801F0B34:
/* 801F0B34 001EDA74  38 21 00 38 */	addi r1, r1, 0x38
/* 801F0B38 001EDA78  4E 80 00 20 */	blr 

.global getEdgeDegree__13TPollutionPosCFii
getEdgeDegree__13TPollutionPosCFii:
/* 801F0B3C 001EDA7C  94 21 FF C8 */	stwu r1, -0x38(r1)
/* 801F0B40 001EDA80  2C 04 00 00 */	cmpwi r4, 0
/* 801F0B44 001EDA84  93 E1 00 34 */	stw r31, 0x34(r1)
/* 801F0B48 001EDA88  41 80 00 24 */	blt lbl_801F0B6C
/* 801F0B4C 001EDA8C  80 03 00 00 */	lwz r0, 0(r3)
/* 801F0B50 001EDA90  7C 00 20 00 */	cmpw r0, r4
/* 801F0B54 001EDA94  40 81 00 18 */	ble lbl_801F0B6C
/* 801F0B58 001EDA98  2C 05 00 00 */	cmpwi r5, 0
/* 801F0B5C 001EDA9C  41 80 00 10 */	blt lbl_801F0B6C
/* 801F0B60 001EDAA0  80 03 00 04 */	lwz r0, 4(r3)
/* 801F0B64 001EDAA4  7C 00 28 00 */	cmpw r0, r5
/* 801F0B68 001EDAA8  41 81 00 0C */	bgt lbl_801F0B74
lbl_801F0B6C:
/* 801F0B6C 001EDAAC  38 00 00 00 */	li r0, 0
/* 801F0B70 001EDAB0  48 00 00 08 */	b lbl_801F0B78
lbl_801F0B74:
/* 801F0B74 001EDAB4  38 00 00 01 */	li r0, 1
lbl_801F0B78:
/* 801F0B78 001EDAB8  54 00 06 3F */	clrlwi. r0, r0, 0x18
/* 801F0B7C 001EDABC  40 82 00 0C */	bne lbl_801F0B88
/* 801F0B80 001EDAC0  38 60 00 00 */	li r3, 0
/* 801F0B84 001EDAC4  48 00 01 00 */	b lbl_801F0C84
lbl_801F0B88:
/* 801F0B88 001EDAC8  38 00 00 03 */	li r0, 3
/* 801F0B8C 001EDACC  7C 09 03 A6 */	mtctr r0
/* 801F0B90 001EDAD0  39 80 00 00 */	li r12, 0
/* 801F0B94 001EDAD4  3B E0 FF FF */	li r31, -1
lbl_801F0B98:
/* 801F0B98 001EDAD8  7C 05 FA 14 */	add r0, r5, r31
/* 801F0B9C 001EDADC  7C 0A 16 70 */	srawi r10, r0, 2
/* 801F0BA0 001EDAE0  54 0B 1E F8 */	rlwinm r11, r0, 3, 0x1b, 0x1c
/* 801F0BA4 001EDAE4  48 00 00 08 */	b lbl_801F0BAC
/* 801F0BA8 001EDAE8  41 82 00 40 */	beq lbl_801F0BE8
lbl_801F0BAC:
/* 801F0BAC 001EDAEC  80 E3 00 08 */	lwz r7, 8(r3)
/* 801F0BB0 001EDAF0  39 24 FF FF */	addi r9, r4, -1
/* 801F0BB4 001EDAF4  7D 28 1E 70 */	srawi r8, r9, 3
/* 801F0BB8 001EDAF8  80 C3 00 1C */	lwz r6, 0x1c(r3)
/* 801F0BBC 001EDAFC  38 07 FF FD */	addi r0, r7, -3
/* 801F0BC0 001EDB00  7D 40 00 30 */	slw r0, r10, r0
/* 801F0BC4 001EDB04  7C 08 02 14 */	add r0, r8, r0
/* 801F0BC8 001EDB08  55 29 07 7E */	clrlwi r9, r9, 0x1d
/* 801F0BCC 001EDB0C  54 00 28 34 */	slwi r0, r0, 5
/* 801F0BD0 001EDB10  7C 00 4A 14 */	add r0, r0, r9
/* 801F0BD4 001EDB14  7C 0B 02 14 */	add r0, r11, r0
/* 801F0BD8 001EDB18  7C 06 00 AE */	lbzx r0, r6, r0
/* 801F0BDC 001EDB1C  28 00 00 FF */	cmplwi r0, 0xff
/* 801F0BE0 001EDB20  40 82 00 08 */	bne lbl_801F0BE8
/* 801F0BE4 001EDB24  39 8C 00 01 */	addi r12, r12, 1
lbl_801F0BE8:
/* 801F0BE8 001EDB28  38 C0 00 00 */	li r6, 0
/* 801F0BEC 001EDB2C  2C 06 00 00 */	cmpwi r6, 0
/* 801F0BF0 001EDB30  40 82 00 0C */	bne lbl_801F0BFC
/* 801F0BF4 001EDB34  2C 1F 00 00 */	cmpwi r31, 0
/* 801F0BF8 001EDB38  41 82 00 44 */	beq lbl_801F0C3C
lbl_801F0BFC:
/* 801F0BFC 001EDB3C  80 E3 00 08 */	lwz r7, 8(r3)
/* 801F0C00 001EDB40  7C 88 1E 70 */	srawi r8, r4, 3
/* 801F0C04 001EDB44  54 89 07 7E */	clrlwi r9, r4, 0x1d
/* 801F0C08 001EDB48  80 C3 00 1C */	lwz r6, 0x1c(r3)
/* 801F0C0C 001EDB4C  38 07 FF FD */	addi r0, r7, -3
/* 801F0C10 001EDB50  7D 40 00 30 */	slw r0, r10, r0
/* 801F0C14 001EDB54  7C 08 02 14 */	add r0, r8, r0
/* 801F0C18 001EDB58  54 00 28 34 */	slwi r0, r0, 5
/* 801F0C1C 001EDB5C  7C 00 4A 14 */	add r0, r0, r9
/* 801F0C20 001EDB60  7C 0B 02 14 */	add r0, r11, r0
/* 801F0C24 001EDB64  7C 06 00 AE */	lbzx r0, r6, r0
/* 801F0C28 001EDB68  28 00 00 FF */	cmplwi r0, 0xff
/* 801F0C2C 001EDB6C  40 82 00 10 */	bne lbl_801F0C3C
/* 801F0C30 001EDB70  39 8C 00 01 */	addi r12, r12, 1
/* 801F0C34 001EDB74  48 00 00 08 */	b lbl_801F0C3C
/* 801F0C38 001EDB78  41 82 00 40 */	beq lbl_801F0C78
lbl_801F0C3C:
/* 801F0C3C 001EDB7C  80 E3 00 08 */	lwz r7, 8(r3)
/* 801F0C40 001EDB80  39 24 00 01 */	addi r9, r4, 1
/* 801F0C44 001EDB84  7D 28 1E 70 */	srawi r8, r9, 3
/* 801F0C48 001EDB88  80 C3 00 1C */	lwz r6, 0x1c(r3)
/* 801F0C4C 001EDB8C  38 07 FF FD */	addi r0, r7, -3
/* 801F0C50 001EDB90  7D 40 00 30 */	slw r0, r10, r0
/* 801F0C54 001EDB94  7C 08 02 14 */	add r0, r8, r0
/* 801F0C58 001EDB98  55 29 07 7E */	clrlwi r9, r9, 0x1d
/* 801F0C5C 001EDB9C  54 00 28 34 */	slwi r0, r0, 5
/* 801F0C60 001EDBA0  7C 00 4A 14 */	add r0, r0, r9
/* 801F0C64 001EDBA4  7C 0B 02 14 */	add r0, r11, r0
/* 801F0C68 001EDBA8  7C 06 00 AE */	lbzx r0, r6, r0
/* 801F0C6C 001EDBAC  28 00 00 FF */	cmplwi r0, 0xff
/* 801F0C70 001EDBB0  40 82 00 08 */	bne lbl_801F0C78
/* 801F0C74 001EDBB4  39 8C 00 01 */	addi r12, r12, 1
lbl_801F0C78:
/* 801F0C78 001EDBB8  3B FF 00 01 */	addi r31, r31, 1
/* 801F0C7C 001EDBBC  42 00 FF 1C */	bdnz lbl_801F0B98
/* 801F0C80 001EDBC0  7D 83 63 78 */	mr r3, r12
lbl_801F0C84:
/* 801F0C84 001EDBC4  83 E1 00 34 */	lwz r31, 0x34(r1)
/* 801F0C88 001EDBC8  38 21 00 38 */	addi r1, r1, 0x38
/* 801F0C8C 001EDBCC  4E 80 00 20 */	blr 

.global __sinit_PollutionPos_cpp
__sinit_PollutionPos_cpp:
/* 801F0C90 001EDBD0  7C 08 02 A6 */	mflr r0
/* 801F0C94 001EDBD4  3C 60 80 40 */	lis r3, $$22374@ha
/* 801F0C98 001EDBD8  90 01 00 04 */	stw r0, 4(r1)
/* 801F0C9C 001EDBDC  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 801F0CA0 001EDBE0  93 E1 00 0C */	stw r31, 0xc(r1)
/* 801F0CA4 001EDBE4  3B E3 84 D8 */	addi r31, r3, $$22374@l
/* 801F0CA8 001EDBE8  88 0D 97 CC */	lbz r0, __init__smList__15JALList$$05MSBgm$$1@sda21(r13)
/* 801F0CAC 001EDBEC  7C 00 07 75 */	extsb. r0, r0
/* 801F0CB0 001EDBF0  40 82 00 28 */	bne lbl_801F0CD8
/* 801F0CB4 001EDBF4  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1@sda21
/* 801F0CB8 001EDBF8  4B E1 DF 7D */	bl initiate__10JSUPtrListFv
/* 801F0CBC 001EDBFC  3C 60 80 0E */	lis r3, __dt__15JSUList$$05MSBgm$$1Fv@ha
/* 801F0CC0 001EDC00  38 83 6A 44 */	addi r4, r3, __dt__15JSUList$$05MSBgm$$1Fv@l
/* 801F0CC4 001EDC04  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1@sda21
/* 801F0CC8 001EDC08  38 BF 00 00 */	addi r5, r31, 0
/* 801F0CCC 001EDC0C  4B E9 1A 5D */	bl __register_global_object
/* 801F0CD0 001EDC10  38 00 00 01 */	li r0, 1
/* 801F0CD4 001EDC14  98 0D 97 CC */	stb r0, __init__smList__15JALList$$05MSBgm$$1@sda21(r13)
lbl_801F0CD8:
/* 801F0CD8 001EDC18  88 0D 97 CD */	lbz r0, __init__smList__24JALList$$013MSSetSoundGrp$$1@sda21(r13)
/* 801F0CDC 001EDC1C  7C 00 07 75 */	extsb. r0, r0
/* 801F0CE0 001EDC20  40 82 00 28 */	bne lbl_801F0D08
/* 801F0CE4 001EDC24  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1@sda21
/* 801F0CE8 001EDC28  4B E1 DF 4D */	bl initiate__10JSUPtrListFv
/* 801F0CEC 001EDC2C  3C 60 80 0E */	lis r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@ha
/* 801F0CF0 001EDC30  38 83 69 EC */	addi r4, r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@l
/* 801F0CF4 001EDC34  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1@sda21
/* 801F0CF8 001EDC38  38 BF 00 0C */	addi r5, r31, 0xc
/* 801F0CFC 001EDC3C  4B E9 1A 2D */	bl __register_global_object
/* 801F0D00 001EDC40  38 00 00 01 */	li r0, 1
/* 801F0D04 001EDC44  98 0D 97 CD */	stb r0, __init__smList__24JALList$$013MSSetSoundGrp$$1@sda21(r13)
lbl_801F0D08:
/* 801F0D08 001EDC48  88 0D 97 CE */	lbz r0, __init__smList__21JALList$$010MSSetSound$$1@sda21(r13)
/* 801F0D0C 001EDC4C  7C 00 07 75 */	extsb. r0, r0
/* 801F0D10 001EDC50  40 82 00 28 */	bne lbl_801F0D38
/* 801F0D14 001EDC54  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1@sda21
/* 801F0D18 001EDC58  4B E1 DF 1D */	bl initiate__10JSUPtrListFv
/* 801F0D1C 001EDC5C  3C 60 80 0E */	lis r3, __dt__21JSUList$$010MSSetSound$$1Fv@ha
/* 801F0D20 001EDC60  38 83 69 94 */	addi r4, r3, __dt__21JSUList$$010MSSetSound$$1Fv@l
/* 801F0D24 001EDC64  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1@sda21
/* 801F0D28 001EDC68  38 BF 00 18 */	addi r5, r31, 0x18
/* 801F0D2C 001EDC6C  4B E9 19 FD */	bl __register_global_object
/* 801F0D30 001EDC70  38 00 00 01 */	li r0, 1
/* 801F0D34 001EDC74  98 0D 97 CE */	stb r0, __init__smList__21JALList$$010MSSetSound$$1@sda21(r13)
lbl_801F0D38:
/* 801F0D38 001EDC78  88 0D 8F 8C */	lbz r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1@sda21(r13)
/* 801F0D3C 001EDC7C  7C 00 07 75 */	extsb. r0, r0
/* 801F0D40 001EDC80  40 82 00 28 */	bne lbl_801F0D68
/* 801F0D44 001EDC84  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1@sda21
/* 801F0D48 001EDC88  4B E1 DE ED */	bl initiate__10JSUPtrListFv
/* 801F0D4C 001EDC8C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@ha
/* 801F0D50 001EDC90  38 83 A8 0C */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@l
/* 801F0D54 001EDC94  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1@sda21
/* 801F0D58 001EDC98  38 BF 00 24 */	addi r5, r31, 0x24
/* 801F0D5C 001EDC9C  4B E9 19 CD */	bl __register_global_object
/* 801F0D60 001EDCA0  38 00 00 01 */	li r0, 1
/* 801F0D64 001EDCA4  98 0D 8F 8C */	stb r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1@sda21(r13)
lbl_801F0D68:
/* 801F0D68 001EDCA8  88 0D 8F 8D */	lbz r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1@sda21(r13)
/* 801F0D6C 001EDCAC  7C 00 07 75 */	extsb. r0, r0
/* 801F0D70 001EDCB0  40 82 00 28 */	bne lbl_801F0D98
/* 801F0D74 001EDCB4  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1@sda21
/* 801F0D78 001EDCB8  4B E1 DE BD */	bl initiate__10JSUPtrListFv
/* 801F0D7C 001EDCBC  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@ha
/* 801F0D80 001EDCC0  38 83 A7 B4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@l
/* 801F0D84 001EDCC4  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1@sda21
/* 801F0D88 001EDCC8  38 BF 00 30 */	addi r5, r31, 0x30
/* 801F0D8C 001EDCCC  4B E9 19 9D */	bl __register_global_object
/* 801F0D90 001EDCD0  38 00 00 01 */	li r0, 1
/* 801F0D94 001EDCD4  98 0D 8F 8D */	stb r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1@sda21(r13)
lbl_801F0D98:
/* 801F0D98 001EDCD8  88 0D 8F 8E */	lbz r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1@sda21(r13)
/* 801F0D9C 001EDCDC  7C 00 07 75 */	extsb. r0, r0
/* 801F0DA0 001EDCE0  40 82 00 28 */	bne lbl_801F0DC8
/* 801F0DA4 001EDCE4  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1@sda21
/* 801F0DA8 001EDCE8  4B E1 DE 8D */	bl initiate__10JSUPtrListFv
/* 801F0DAC 001EDCEC  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@ha
/* 801F0DB0 001EDCF0  38 83 A7 5C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@l
/* 801F0DB4 001EDCF4  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1@sda21
/* 801F0DB8 001EDCF8  38 BF 00 3C */	addi r5, r31, 0x3c
/* 801F0DBC 001EDCFC  4B E9 19 6D */	bl __register_global_object
/* 801F0DC0 001EDD00  38 00 00 01 */	li r0, 1
/* 801F0DC4 001EDD04  98 0D 8F 8E */	stb r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1@sda21(r13)
lbl_801F0DC8:
/* 801F0DC8 001EDD08  88 0D 8F 8F */	lbz r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1@sda21(r13)
/* 801F0DCC 001EDD0C  7C 00 07 75 */	extsb. r0, r0
/* 801F0DD0 001EDD10  40 82 00 28 */	bne lbl_801F0DF8
/* 801F0DD4 001EDD14  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1@sda21
/* 801F0DD8 001EDD18  4B E1 DE 5D */	bl initiate__10JSUPtrListFv
/* 801F0DDC 001EDD1C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@ha
/* 801F0DE0 001EDD20  38 83 A7 04 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@l
/* 801F0DE4 001EDD24  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1@sda21
/* 801F0DE8 001EDD28  38 BF 00 48 */	addi r5, r31, 0x48
/* 801F0DEC 001EDD2C  4B E9 19 3D */	bl __register_global_object
/* 801F0DF0 001EDD30  38 00 00 01 */	li r0, 1
/* 801F0DF4 001EDD34  98 0D 8F 8F */	stb r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1@sda21(r13)
lbl_801F0DF8:
/* 801F0DF8 001EDD38  88 0D 8F 90 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1@sda21(r13)
/* 801F0DFC 001EDD3C  7C 00 07 75 */	extsb. r0, r0
/* 801F0E00 001EDD40  40 82 00 28 */	bne lbl_801F0E28
/* 801F0E04 001EDD44  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1@sda21
/* 801F0E08 001EDD48  4B E1 DE 2D */	bl initiate__10JSUPtrListFv
/* 801F0E0C 001EDD4C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@ha
/* 801F0E10 001EDD50  38 83 A6 AC */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@l
/* 801F0E14 001EDD54  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1@sda21
/* 801F0E18 001EDD58  38 BF 00 54 */	addi r5, r31, 0x54
/* 801F0E1C 001EDD5C  4B E9 19 0D */	bl __register_global_object
/* 801F0E20 001EDD60  38 00 00 01 */	li r0, 1
/* 801F0E24 001EDD64  98 0D 8F 90 */	stb r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1@sda21(r13)
lbl_801F0E28:
/* 801F0E28 001EDD68  88 0D 8F 91 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1@sda21(r13)
/* 801F0E2C 001EDD6C  7C 00 07 75 */	extsb. r0, r0
/* 801F0E30 001EDD70  40 82 00 28 */	bne lbl_801F0E58
/* 801F0E34 001EDD74  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1@sda21
/* 801F0E38 001EDD78  4B E1 DD FD */	bl initiate__10JSUPtrListFv
/* 801F0E3C 001EDD7C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@ha
/* 801F0E40 001EDD80  38 83 A6 54 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@l
/* 801F0E44 001EDD84  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1@sda21
/* 801F0E48 001EDD88  38 BF 00 60 */	addi r5, r31, 0x60
/* 801F0E4C 001EDD8C  4B E9 18 DD */	bl __register_global_object
/* 801F0E50 001EDD90  38 00 00 01 */	li r0, 1
/* 801F0E54 001EDD94  98 0D 8F 91 */	stb r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1@sda21(r13)
lbl_801F0E58:
/* 801F0E58 001EDD98  88 0D 8F 92 */	lbz r0, __init__smList__26JALList$$015JALSeModEffDist$$1@sda21(r13)
/* 801F0E5C 001EDD9C  7C 00 07 75 */	extsb. r0, r0
/* 801F0E60 001EDDA0  40 82 00 28 */	bne lbl_801F0E88
/* 801F0E64 001EDDA4  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1@sda21
/* 801F0E68 001EDDA8  4B E1 DD CD */	bl initiate__10JSUPtrListFv
/* 801F0E6C 001EDDAC  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@ha
/* 801F0E70 001EDDB0  38 83 A5 FC */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@l
/* 801F0E74 001EDDB4  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1@sda21
/* 801F0E78 001EDDB8  38 BF 00 6C */	addi r5, r31, 0x6c
/* 801F0E7C 001EDDBC  4B E9 18 AD */	bl __register_global_object
/* 801F0E80 001EDDC0  38 00 00 01 */	li r0, 1
/* 801F0E84 001EDDC4  98 0D 8F 92 */	stb r0, __init__smList__26JALList$$015JALSeModEffDist$$1@sda21(r13)
lbl_801F0E88:
/* 801F0E88 001EDDC8  88 0D 8F 93 */	lbz r0, __init__smList__26JALList$$015JALSeModPitDist$$1@sda21(r13)
/* 801F0E8C 001EDDCC  7C 00 07 75 */	extsb. r0, r0
/* 801F0E90 001EDDD0  40 82 00 28 */	bne lbl_801F0EB8
/* 801F0E94 001EDDD4  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1@sda21
/* 801F0E98 001EDDD8  4B E1 DD 9D */	bl initiate__10JSUPtrListFv
/* 801F0E9C 001EDDDC  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@ha
/* 801F0EA0 001EDDE0  38 83 A5 A4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@l
/* 801F0EA4 001EDDE4  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1@sda21
/* 801F0EA8 001EDDE8  38 BF 00 78 */	addi r5, r31, 0x78
/* 801F0EAC 001EDDEC  4B E9 18 7D */	bl __register_global_object
/* 801F0EB0 001EDDF0  38 00 00 01 */	li r0, 1
/* 801F0EB4 001EDDF4  98 0D 8F 93 */	stb r0, __init__smList__26JALList$$015JALSeModPitDist$$1@sda21(r13)
lbl_801F0EB8:
/* 801F0EB8 001EDDF8  88 0D 8F 94 */	lbz r0, __init__smList__26JALList$$015JALSeModVolDist$$1@sda21(r13)
/* 801F0EBC 001EDDFC  7C 00 07 75 */	extsb. r0, r0
/* 801F0EC0 001EDE00  40 82 00 28 */	bne lbl_801F0EE8
/* 801F0EC4 001EDE04  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1@sda21
/* 801F0EC8 001EDE08  4B E1 DD 6D */	bl initiate__10JSUPtrListFv
/* 801F0ECC 001EDE0C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@ha
/* 801F0ED0 001EDE10  38 83 A5 4C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@l
/* 801F0ED4 001EDE14  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1@sda21
/* 801F0ED8 001EDE18  38 BF 00 84 */	addi r5, r31, 0x84
/* 801F0EDC 001EDE1C  4B E9 18 4D */	bl __register_global_object
/* 801F0EE0 001EDE20  38 00 00 01 */	li r0, 1
/* 801F0EE4 001EDE24  98 0D 8F 94 */	stb r0, __init__smList__26JALList$$015JALSeModVolDist$$1@sda21(r13)
lbl_801F0EE8:
/* 801F0EE8 001EDE28  88 0D 8F 95 */	lbz r0, __init__smList__26JALList$$015JALSeModEffFunk$$1@sda21(r13)
/* 801F0EEC 001EDE2C  7C 00 07 75 */	extsb. r0, r0
/* 801F0EF0 001EDE30  40 82 00 28 */	bne lbl_801F0F18
/* 801F0EF4 001EDE34  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1@sda21
/* 801F0EF8 001EDE38  4B E1 DD 3D */	bl initiate__10JSUPtrListFv
/* 801F0EFC 001EDE3C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@ha
/* 801F0F00 001EDE40  38 83 A4 F4 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@l
/* 801F0F04 001EDE44  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1@sda21
/* 801F0F08 001EDE48  38 BF 00 90 */	addi r5, r31, 0x90
/* 801F0F0C 001EDE4C  4B E9 18 1D */	bl __register_global_object
/* 801F0F10 001EDE50  38 00 00 01 */	li r0, 1
/* 801F0F14 001EDE54  98 0D 8F 95 */	stb r0, __init__smList__26JALList$$015JALSeModEffFunk$$1@sda21(r13)
lbl_801F0F18:
/* 801F0F18 001EDE58  88 0D 8F 96 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFunk$$1@sda21(r13)
/* 801F0F1C 001EDE5C  7C 00 07 75 */	extsb. r0, r0
/* 801F0F20 001EDE60  40 82 00 28 */	bne lbl_801F0F48
/* 801F0F24 001EDE64  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1@sda21
/* 801F0F28 001EDE68  4B E1 DD 0D */	bl initiate__10JSUPtrListFv
/* 801F0F2C 001EDE6C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@ha
/* 801F0F30 001EDE70  38 83 A4 9C */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@l
/* 801F0F34 001EDE74  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1@sda21
/* 801F0F38 001EDE78  38 BF 00 9C */	addi r5, r31, 0x9c
/* 801F0F3C 001EDE7C  4B E9 17 ED */	bl __register_global_object
/* 801F0F40 001EDE80  38 00 00 01 */	li r0, 1
/* 801F0F44 001EDE84  98 0D 8F 96 */	stb r0, __init__smList__26JALList$$015JALSeModPitFunk$$1@sda21(r13)
lbl_801F0F48:
/* 801F0F48 001EDE88  88 0D 8F 97 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFunk$$1@sda21(r13)
/* 801F0F4C 001EDE8C  7C 00 07 75 */	extsb. r0, r0
/* 801F0F50 001EDE90  40 82 00 28 */	bne lbl_801F0F78
/* 801F0F54 001EDE94  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1@sda21
/* 801F0F58 001EDE98  4B E1 DC DD */	bl initiate__10JSUPtrListFv
/* 801F0F5C 001EDE9C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@ha
/* 801F0F60 001EDEA0  38 83 A4 44 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@l
/* 801F0F64 001EDEA4  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1@sda21
/* 801F0F68 001EDEA8  38 BF 00 A8 */	addi r5, r31, 0xa8
/* 801F0F6C 001EDEAC  4B E9 17 BD */	bl __register_global_object
/* 801F0F70 001EDEB0  38 00 00 01 */	li r0, 1
/* 801F0F74 001EDEB4  98 0D 8F 97 */	stb r0, __init__smList__26JALList$$015JALSeModVolFunk$$1@sda21(r13)
lbl_801F0F78:
/* 801F0F78 001EDEB8  80 01 00 14 */	lwz r0, 0x14(r1)
/* 801F0F7C 001EDEBC  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 801F0F80 001EDEC0  38 21 00 10 */	addi r1, r1, 0x10
/* 801F0F84 001EDEC4  7C 08 03 A6 */	mtlr r0
/* 801F0F88 001EDEC8  4E 80 00 20 */	blr 

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
lbl_constructor:
  .4byte __sinit_PollutionPos_cpp

.section .sdata2, "a"  # 0x8040B460 - 0x80414020
.balign 8
$$22390:
  .4byte 0
$$22392:
	.incbin "baserom.dol", 0x3E8D84, 0x4
$$22396:
	.incbin "baserom.dol", 0x3E8D88, 0x4
$$22460:
	.incbin "baserom.dol", 0x3E8D8C, 0x4
$$22462:
	.incbin "baserom.dol", 0x3E8D90, 0x8

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
$$22374:
	.skip 0xC
$$22375:
	.skip 0xC
$$22376:
	.skip 0xC
$$22377:
	.skip 0xC
$$22378:
	.skip 0xC
$$22379:
	.skip 0xC
$$22380:
	.skip 0xC
$$22381:
	.skip 0xC
$$22382:
	.skip 0xC
$$22383:
	.skip 0xC
$$22384:
	.skip 0xC
$$22385:
	.skip 0xC
$$22386:
	.skip 0xC
$$22387:
	.skip 0xC
$$22388:
	.skip 0x10

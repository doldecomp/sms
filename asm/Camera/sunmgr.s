.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __dt__7TSunMgrFv
__dt__7TSunMgrFv:
/* 8035DC74 0035ABB4  7C 08 02 A6 */	mflr r0
/* 8035DC78 0035ABB8  90 01 00 04 */	stw r0, 4(r1)
/* 8035DC7C 0035ABBC  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8035DC80 0035ABC0  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8035DC84 0035ABC4  3B E4 00 00 */	addi r31, r4, 0
/* 8035DC88 0035ABC8  93 C1 00 10 */	stw r30, 0x10(r1)
/* 8035DC8C 0035ABCC  7C 7E 1B 79 */	or. r30, r3, r3
/* 8035DC90 0035ABD0  41 82 00 44 */	beq lbl_8035DCD4
/* 8035DC94 0035ABD4  3C 60 80 3E */	lis r3, __vt__7TSunMgr@ha
/* 8035DC98 0035ABD8  38 63 4B 60 */	addi r3, r3, __vt__7TSunMgr@l
/* 8035DC9C 0035ABDC  90 7E 00 00 */	stw r3, 0(r30)
/* 8035DCA0 0035ABE0  38 03 00 24 */	addi r0, r3, 0x24
/* 8035DCA4 0035ABE4  90 1E 00 10 */	stw r0, 0x10(r30)
/* 8035DCA8 0035ABE8  41 82 00 1C */	beq lbl_8035DCC4
/* 8035DCAC 0035ABEC  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TViewObj@ha
/* 8035DCB0 0035ABF0  38 03 A0 C0 */	addi r0, r3, __vt__Q26JDrama8TViewObj@l
/* 8035DCB4 0035ABF4  90 1E 00 00 */	stw r0, 0(r30)
/* 8035DCB8 0035ABF8  38 7E 00 00 */	addi r3, r30, 0
/* 8035DCBC 0035ABFC  38 80 00 00 */	li r4, 0
/* 8035DCC0 0035AC00  4B CE 6B 3D */	bl __dt__Q26JDrama8TNameRefFv
lbl_8035DCC4:
/* 8035DCC4 0035AC04  7F E0 07 35 */	extsh. r0, r31
/* 8035DCC8 0035AC08  40 81 00 0C */	ble lbl_8035DCD4
/* 8035DCCC 0035AC0C  7F C3 F3 78 */	mr r3, r30
/* 8035DCD0 0035AC10  4B CA ED E1 */	bl __dl__FPv
lbl_8035DCD4:
/* 8035DCD4 0035AC14  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8035DCD8 0035AC18  7F C3 F3 78 */	mr r3, r30
/* 8035DCDC 0035AC1C  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8035DCE0 0035AC20  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 8035DCE4 0035AC24  7C 08 03 A6 */	mtlr r0
/* 8035DCE8 0035AC28  38 21 00 18 */	addi r1, r1, 0x18
/* 8035DCEC 0035AC2C  4E 80 00 20 */	blr 

.global drawSyncCallback__7TSunMgrFUs
drawSyncCallback__7TSunMgrFUs:
/* 8035DCF0 0035AC30  7C 08 02 A6 */	mflr r0
/* 8035DCF4 0035AC34  90 01 00 04 */	stw r0, 4(r1)
/* 8035DCF8 0035AC38  94 21 FF F8 */	stwu r1, -8(r1)
/* 8035DCFC 0035AC3C  88 03 00 14 */	lbz r0, 0x14(r3)
/* 8035DD00 0035AC40  28 00 00 00 */	cmplwi r0, 0
/* 8035DD04 0035AC44  41 82 00 0C */	beq lbl_8035DD10
/* 8035DD08 0035AC48  80 6D A8 D0 */	lwz r3, gpSunModel@sda21(r13)
/* 8035DD0C 0035AC4C  48 00 07 E5 */	bl getZBufValue__9TSunModelFv
lbl_8035DD10:
/* 8035DD10 0035AC50  80 01 00 0C */	lwz r0, 0xc(r1)
/* 8035DD14 0035AC54  38 21 00 08 */	addi r1, r1, 8
/* 8035DD18 0035AC58  7C 08 03 A6 */	mtlr r0
/* 8035DD1C 0035AC5C  4E 80 00 20 */	blr 

.global getAddColor__7TSunMgrCFv
getAddColor__7TSunMgrCFv:
/* 8035DD20 0035AC60  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 8035DD24 0035AC64  88 03 00 14 */	lbz r0, 0x14(r3)
/* 8035DD28 0035AC68  38 60 00 00 */	li r3, 0
/* 8035DD2C 0035AC6C  28 00 00 00 */	cmplwi r0, 0
/* 8035DD30 0035AC70  41 82 00 18 */	beq lbl_8035DD48
/* 8035DD34 0035AC74  80 6D A8 D0 */	lwz r3, gpSunModel@sda21(r13)
/* 8035DD38 0035AC78  C0 03 00 AC */	lfs f0, 0xac(r3)
/* 8035DD3C 0035AC7C  FC 00 00 1E */	fctiwz f0, f0
/* 8035DD40 0035AC80  D8 01 00 18 */	stfd f0, 0x18(r1)
/* 8035DD44 0035AC84  80 61 00 1C */	lwz r3, 0x1c(r1)
lbl_8035DD48:
/* 8035DD48 0035AC88  38 21 00 20 */	addi r1, r1, 0x20
/* 8035DD4C 0035AC8C  4E 80 00 20 */	blr 

.global perform__7TSunMgrFUlPQ26JDrama9TGraphics
perform__7TSunMgrFUlPQ26JDrama9TGraphics:
/* 8035DD50 0035AC90  7C 08 02 A6 */	mflr r0
/* 8035DD54 0035AC94  90 01 00 04 */	stw r0, 4(r1)
/* 8035DD58 0035AC98  94 21 FF A0 */	stwu r1, -0x60(r1)
/* 8035DD5C 0035AC9C  93 E1 00 5C */	stw r31, 0x5c(r1)
/* 8035DD60 0035ACA0  93 C1 00 58 */	stw r30, 0x58(r1)
/* 8035DD64 0035ACA4  93 A1 00 54 */	stw r29, 0x54(r1)
/* 8035DD68 0035ACA8  7C 7D 1B 78 */	mr r29, r3
/* 8035DD6C 0035ACAC  88 03 00 15 */	lbz r0, 0x15(r3)
/* 8035DD70 0035ACB0  54 00 07 FF */	clrlwi. r0, r0, 0x1f
/* 8035DD74 0035ACB4  41 82 01 6C */	beq lbl_8035DEE0
/* 8035DD78 0035ACB8  54 80 07 FF */	clrlwi. r0, r4, 0x1f
/* 8035DD7C 0035ACBC  41 82 01 64 */	beq lbl_8035DEE0
/* 8035DD80 0035ACC0  A0 05 00 00 */	lhz r0, 0(r5)
/* 8035DD84 0035ACC4  54 00 07 BD */	rlwinm. r0, r0, 0, 0x1e, 0x1e
/* 8035DD88 0035ACC8  41 82 01 58 */	beq lbl_8035DEE0
/* 8035DD8C 0035ACCC  83 ED A8 B0 */	lwz r31, gpCamera@sda21(r13)
/* 8035DD90 0035ACD0  3B C0 00 00 */	li r30, 0
/* 8035DD94 0035ACD4  80 9F 00 50 */	lwz r4, 0x50(r31)
/* 8035DD98 0035ACD8  7F E3 FB 78 */	mr r3, r31
/* 8035DD9C 0035ACDC  48 00 56 65 */	bl isLButtonCameraSpecifyMode__15CPolarSubCameraCFi
/* 8035DDA0 0035ACE0  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 8035DDA4 0035ACE4  41 82 00 18 */	beq lbl_8035DDBC
/* 8035DDA8 0035ACE8  7F E3 FB 78 */	mr r3, r31
/* 8035DDAC 0035ACEC  4B FF 74 D9 */	bl isNowInbetween__15CPolarSubCameraCFv
/* 8035DDB0 0035ACF0  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 8035DDB4 0035ACF4  40 82 00 08 */	bne lbl_8035DDBC
/* 8035DDB8 0035ACF8  3B C0 00 01 */	li r30, 1
lbl_8035DDBC:
/* 8035DDBC 0035ACFC  57 C0 06 3F */	clrlwi. r0, r30, 0x18
/* 8035DDC0 0035AD00  41 82 00 0C */	beq lbl_8035DDCC
/* 8035DDC4 0035AD04  38 00 00 01 */	li r0, 1
/* 8035DDC8 0035AD08  48 00 00 08 */	b lbl_8035DDD0
lbl_8035DDCC:
/* 8035DDCC 0035AD0C  38 00 00 00 */	li r0, 0
lbl_8035DDD0:
/* 8035DDD0 0035AD10  54 00 06 3F */	clrlwi. r0, r0, 0x18
/* 8035DDD4 0035AD14  41 82 01 0C */	beq lbl_8035DEE0
/* 8035DDD8 0035AD18  80 6D 98 DC */	lwz r3, gpMarioPos@sda21(r13)
/* 8035DDDC 0035AD1C  C0 1D 00 2C */	lfs f0, 0x2c(r29)
/* 8035DDE0 0035AD20  C0 23 00 08 */	lfs f1, 8(r3)
/* 8035DDE4 0035AD24  C0 43 00 00 */	lfs f2, 0(r3)
/* 8035DDE8 0035AD28  EC 61 00 28 */	fsubs f3, f1, f0
/* 8035DDEC 0035AD2C  C0 3D 00 24 */	lfs f1, 0x24(r29)
/* 8035DDF0 0035AD30  C0 02 07 58 */	lfs f0, $$22158@sda21(r2)
/* 8035DDF4 0035AD34  EC 42 08 28 */	fsubs f2, f2, f1
/* 8035DDF8 0035AD38  EC 23 00 F2 */	fmuls f1, f3, f3
/* 8035DDFC 0035AD3C  EC 22 08 BA */	fmadds f1, f2, f2, f1
/* 8035DE00 0035AD40  FC 01 00 40 */	fcmpo cr0, f1, f0
/* 8035DE04 0035AD44  40 80 00 DC */	bge lbl_8035DEE0
/* 8035DE08 0035AD48  80 6D A8 D0 */	lwz r3, gpSunModel@sda21(r13)
/* 8035DE0C 0035AD4C  38 80 00 00 */	li r4, 0
/* 8035DE10 0035AD50  C0 02 07 5C */	lfs f0, $$22159@sda21(r2)
/* 8035DE14 0035AD54  7C 85 23 78 */	mr r5, r4
/* 8035DE18 0035AD58  C4 23 00 F8 */	lfsu f1, 0xf8(r3)
/* 8035DE1C 0035AD5C  7C 80 23 78 */	mr r0, r4
/* 8035DE20 0035AD60  FC 00 08 40 */	fcmpo cr0, f0, f1
/* 8035DE24 0035AD64  4C 40 13 82 */	cror 2, 0, 2
/* 8035DE28 0035AD68  40 82 00 18 */	bne lbl_8035DE40
/* 8035DE2C 0035AD6C  C0 02 07 60 */	lfs f0, $$22160@sda21(r2)
/* 8035DE30 0035AD70  FC 01 00 40 */	fcmpo cr0, f1, f0
/* 8035DE34 0035AD74  4C 40 13 82 */	cror 2, 0, 2
/* 8035DE38 0035AD78  40 82 00 08 */	bne lbl_8035DE40
/* 8035DE3C 0035AD7C  38 00 00 01 */	li r0, 1
lbl_8035DE40:
/* 8035DE40 0035AD80  54 00 06 3F */	clrlwi. r0, r0, 0x18
/* 8035DE44 0035AD84  41 82 00 1C */	beq lbl_8035DE60
/* 8035DE48 0035AD88  C0 22 07 5C */	lfs f1, $$22159@sda21(r2)
/* 8035DE4C 0035AD8C  C0 03 00 04 */	lfs f0, 4(r3)
/* 8035DE50 0035AD90  FC 01 00 40 */	fcmpo cr0, f1, f0
/* 8035DE54 0035AD94  4C 40 13 82 */	cror 2, 0, 2
/* 8035DE58 0035AD98  40 82 00 08 */	bne lbl_8035DE60
/* 8035DE5C 0035AD9C  38 A0 00 01 */	li r5, 1
lbl_8035DE60:
/* 8035DE60 0035ADA0  54 A0 06 3F */	clrlwi. r0, r5, 0x18
/* 8035DE64 0035ADA4  41 82 00 1C */	beq lbl_8035DE80
/* 8035DE68 0035ADA8  C0 23 00 04 */	lfs f1, 4(r3)
/* 8035DE6C 0035ADAC  C0 02 07 60 */	lfs f0, $$22160@sda21(r2)
/* 8035DE70 0035ADB0  FC 01 00 40 */	fcmpo cr0, f1, f0
/* 8035DE74 0035ADB4  4C 40 13 82 */	cror 2, 0, 2
/* 8035DE78 0035ADB8  40 82 00 08 */	bne lbl_8035DE80
/* 8035DE7C 0035ADBC  38 80 00 01 */	li r4, 1
lbl_8035DE80:
/* 8035DE80 0035ADC0  54 80 06 3F */	clrlwi. r0, r4, 0x18
/* 8035DE84 0035ADC4  41 82 00 0C */	beq lbl_8035DE90
/* 8035DE88 0035ADC8  38 00 00 01 */	li r0, 1
/* 8035DE8C 0035ADCC  48 00 00 08 */	b lbl_8035DE94
lbl_8035DE90:
/* 8035DE90 0035ADD0  38 00 00 00 */	li r0, 0
lbl_8035DE94:
/* 8035DE94 0035ADD4  54 00 06 3F */	clrlwi. r0, r0, 0x18
/* 8035DE98 0035ADD8  41 82 00 48 */	beq lbl_8035DEE0
/* 8035DE9C 0035ADDC  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 8035DEA0 0035ADE0  38 80 00 09 */	li r4, 9
/* 8035DEA4 0035ADE4  38 A0 00 00 */	li r5, 0
/* 8035DEA8 0035ADE8  4B D8 F9 F9 */	bl setNextStage__12TMarDirectorFUsPQ26JDrama6TActor
/* 8035DEAC 0035ADEC  83 CD 97 EC */	lwz r30, gpMSound@sda21(r13)
/* 8035DEB0 0035ADF0  80 7E 00 7C */	lwz r3, 0x7c(r30)
/* 8035DEB4 0035ADF4  28 03 00 00 */	cmplwi r3, 0
/* 8035DEB8 0035ADF8  41 82 00 28 */	beq lbl_8035DEE0
/* 8035DEBC 0035ADFC  C0 22 07 64 */	lfs f1, $$22161@sda21(r2)
/* 8035DEC0 0035AE00  38 80 00 64 */	li r4, 0x64
/* 8035DEC4 0035AE04  38 A0 00 00 */	li r5, 0
/* 8035DEC8 0035AE08  4B CF 6F 7D */	bl setVolume__8JAISoundFfUlUc
/* 8035DECC 0035AE0C  80 7E 00 7C */	lwz r3, 0x7c(r30)
/* 8035DED0 0035AE10  38 80 00 64 */	li r4, 0x64
/* 8035DED4 0035AE14  C0 22 07 68 */	lfs f1, $$22162@sda21(r2)
/* 8035DED8 0035AE18  38 A0 00 00 */	li r5, 0
/* 8035DEDC 0035AE1C  4B CF 70 79 */	bl setPitch__8JAISoundFfUlUc
lbl_8035DEE0:
/* 8035DEE0 0035AE20  80 01 00 64 */	lwz r0, 0x64(r1)
/* 8035DEE4 0035AE24  83 E1 00 5C */	lwz r31, 0x5c(r1)
/* 8035DEE8 0035AE28  83 C1 00 58 */	lwz r30, 0x58(r1)
/* 8035DEEC 0035AE2C  7C 08 03 A6 */	mtlr r0
/* 8035DEF0 0035AE30  83 A1 00 54 */	lwz r29, 0x54(r1)
/* 8035DEF4 0035AE34  38 21 00 60 */	addi r1, r1, 0x60
/* 8035DEF8 0035AE38  4E 80 00 20 */	blr 

.global load__7TSunMgrFR20JSUMemoryInputStream
load__7TSunMgrFR20JSUMemoryInputStream:
/* 8035DEFC 0035AE3C  7C 08 02 A6 */	mflr r0
/* 8035DF00 0035AE40  3C A0 80 3A */	lis r5, $$21490@ha
/* 8035DF04 0035AE44  90 01 00 04 */	stw r0, 4(r1)
/* 8035DF08 0035AE48  94 21 FF 50 */	stwu r1, -0xb0(r1)
/* 8035DF0C 0035AE4C  93 E1 00 AC */	stw r31, 0xac(r1)
/* 8035DF10 0035AE50  3B E5 69 60 */	addi r31, r5, $$21490@l
/* 8035DF14 0035AE54  93 C1 00 A8 */	stw r30, 0xa8(r1)
/* 8035DF18 0035AE58  3B C3 00 00 */	addi r30, r3, 0
/* 8035DF1C 0035AE5C  93 A1 00 A4 */	stw r29, 0xa4(r1)
/* 8035DF20 0035AE60  3B A4 00 00 */	addi r29, r4, 0
/* 8035DF24 0035AE64  4B CE 69 2D */	bl load__Q26JDrama8TNameRefFR20JSUMemoryInputStream
/* 8035DF28 0035AE68  38 7D 00 00 */	addi r3, r29, 0
/* 8035DF2C 0035AE6C  38 81 00 8C */	addi r4, r1, 0x8c
/* 8035DF30 0035AE70  38 A0 00 04 */	li r5, 4
/* 8035DF34 0035AE74  4B CB 06 5D */	bl read__14JSUInputStreamFPvl
/* 8035DF38 0035AE78  38 7D 00 00 */	addi r3, r29, 0
/* 8035DF3C 0035AE7C  38 81 00 90 */	addi r4, r1, 0x90
/* 8035DF40 0035AE80  38 A0 00 04 */	li r5, 4
/* 8035DF44 0035AE84  4B CB 06 4D */	bl read__14JSUInputStreamFPvl
/* 8035DF48 0035AE88  38 7D 00 00 */	addi r3, r29, 0
/* 8035DF4C 0035AE8C  38 81 00 94 */	addi r4, r1, 0x94
/* 8035DF50 0035AE90  38 A0 00 04 */	li r5, 4
/* 8035DF54 0035AE94  4B CB 06 3D */	bl read__14JSUInputStreamFPvl
/* 8035DF58 0035AE98  38 7D 00 00 */	addi r3, r29, 0
/* 8035DF5C 0035AE9C  38 81 00 98 */	addi r4, r1, 0x98
/* 8035DF60 0035AEA0  38 A0 00 04 */	li r5, 4
/* 8035DF64 0035AEA4  4B CB 06 2D */	bl read__14JSUInputStreamFPvl
/* 8035DF68 0035AEA8  38 7D 00 00 */	addi r3, r29, 0
/* 8035DF6C 0035AEAC  38 9E 00 20 */	addi r4, r30, 0x20
/* 8035DF70 0035AEB0  38 A0 00 04 */	li r5, 4
/* 8035DF74 0035AEB4  4B CB 06 1D */	bl read__14JSUInputStreamFPvl
/* 8035DF78 0035AEB8  80 81 00 8C */	lwz r4, 0x8c(r1)
/* 8035DF7C 0035AEBC  38 7F 00 50 */	addi r3, r31, 0x50
/* 8035DF80 0035AEC0  80 01 00 90 */	lwz r0, 0x90(r1)
/* 8035DF84 0035AEC4  54 84 40 2E */	slwi r4, r4, 8
/* 8035DF88 0035AEC8  80 C1 00 94 */	lwz r6, 0x94(r1)
/* 8035DF8C 0035AECC  80 A1 00 98 */	lwz r5, 0x98(r1)
/* 8035DF90 0035AED0  7C 80 03 78 */	or r0, r4, r0
/* 8035DF94 0035AED4  54 C4 40 2E */	slwi r4, r6, 8
/* 8035DF98 0035AED8  90 1E 00 18 */	stw r0, 0x18(r30)
/* 8035DF9C 0035AEDC  7C 80 2B 78 */	or r0, r4, r5
/* 8035DFA0 0035AEE0  90 1E 00 1C */	stw r0, 0x1c(r30)
/* 8035DFA4 0035AEE4  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 8035DFA8 0035AEE8  83 A4 00 04 */	lwz r29, 4(r4)
/* 8035DFAC 0035AEEC  4B CE 66 E1 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 8035DFB0 0035AEF0  81 9D 00 00 */	lwz r12, 0(r29)
/* 8035DFB4 0035AEF4  38 83 00 00 */	addi r4, r3, 0
/* 8035DFB8 0035AEF8  38 7D 00 00 */	addi r3, r29, 0
/* 8035DFBC 0035AEFC  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 8035DFC0 0035AF00  38 BF 00 50 */	addi r5, r31, 0x50
/* 8035DFC4 0035AF04  7D 88 03 A6 */	mtlr r12
/* 8035DFC8 0035AF08  4E 80 00 21 */	blrl 
/* 8035DFCC 0035AF0C  28 03 00 00 */	cmplwi r3, 0
/* 8035DFD0 0035AF10  41 82 00 10 */	beq lbl_8035DFE0
/* 8035DFD4 0035AF14  38 00 00 01 */	li r0, 1
/* 8035DFD8 0035AF18  98 1E 00 14 */	stb r0, 0x14(r30)
/* 8035DFDC 0035AF1C  48 00 00 4C */	b lbl_8035E028
lbl_8035DFE0:
/* 8035DFE0 0035AF20  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 8035DFE4 0035AF24  38 7F 00 5C */	addi r3, r31, 0x5c
/* 8035DFE8 0035AF28  83 A4 00 04 */	lwz r29, 4(r4)
/* 8035DFEC 0035AF2C  4B CE 66 A1 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 8035DFF0 0035AF30  81 9D 00 00 */	lwz r12, 0(r29)
/* 8035DFF4 0035AF34  38 83 00 00 */	addi r4, r3, 0
/* 8035DFF8 0035AF38  38 7D 00 00 */	addi r3, r29, 0
/* 8035DFFC 0035AF3C  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 8035E000 0035AF40  38 BF 00 5C */	addi r5, r31, 0x5c
/* 8035E004 0035AF44  7D 88 03 A6 */	mtlr r12
/* 8035E008 0035AF48  4E 80 00 21 */	blrl 
/* 8035E00C 0035AF4C  28 03 00 00 */	cmplwi r3, 0
/* 8035E010 0035AF50  41 82 00 18 */	beq lbl_8035E028
/* 8035E014 0035AF54  38 00 00 01 */	li r0, 1
/* 8035E018 0035AF58  98 1E 00 14 */	stb r0, 0x14(r30)
/* 8035E01C 0035AF5C  88 1E 00 15 */	lbz r0, 0x15(r30)
/* 8035E020 0035AF60  60 00 00 02 */	ori r0, r0, 2
/* 8035E024 0035AF64  98 1E 00 15 */	stb r0, 0x15(r30)
lbl_8035E028:
/* 8035E028 0035AF68  88 1E 00 14 */	lbz r0, 0x14(r30)
/* 8035E02C 0035AF6C  28 00 00 00 */	cmplwi r0, 0
/* 8035E030 0035AF70  41 82 00 78 */	beq lbl_8035E0A8
/* 8035E034 0035AF74  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 8035E038 0035AF78  88 03 00 7C */	lbz r0, 0x7c(r3)
/* 8035E03C 0035AF7C  28 00 00 01 */	cmplwi r0, 1
/* 8035E040 0035AF80  40 82 00 68 */	bne lbl_8035E0A8
/* 8035E044 0035AF84  3C 80 00 05 */	lis r4, 0x00050004@ha
/* 8035E048 0035AF88  80 6D 97 D0 */	lwz r3, smInstance__12TFlagManager@sda21(r13)
/* 8035E04C 0035AF8C  38 84 00 04 */	addi r4, r4, 0x00050004@l
/* 8035E050 0035AF90  4B D8 9F 91 */	bl getBool__12TFlagManagerCFUl
/* 8035E054 0035AF94  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 8035E058 0035AF98  41 82 00 50 */	beq lbl_8035E0A8
/* 8035E05C 0035AF9C  88 1E 00 15 */	lbz r0, 0x15(r30)
/* 8035E060 0035AFA0  60 00 00 01 */	ori r0, r0, 1
/* 8035E064 0035AFA4  98 1E 00 15 */	stb r0, 0x15(r30)
/* 8035E068 0035AFA8  83 AD 8C D0 */	lwz r29, cSunWarpPointName@sda21(r13)
/* 8035E06C 0035AFAC  7F A3 EB 78 */	mr r3, r29
/* 8035E070 0035AFB0  4B CE 66 1D */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 8035E074 0035AFB4  7C 64 1B 78 */	mr r4, r3
/* 8035E078 0035AFB8  80 6D 98 30 */	lwz r3, gpPositionHolder@sda21(r13)
/* 8035E07C 0035AFBC  7F A5 EB 78 */	mr r5, r29
/* 8035E080 0035AFC0  81 83 00 00 */	lwz r12, 0(r3)
/* 8035E084 0035AFC4  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 8035E088 0035AFC8  7D 88 03 A6 */	mtlr r12
/* 8035E08C 0035AFCC  4E 80 00 21 */	blrl 
/* 8035E090 0035AFD0  80 83 00 0C */	lwz r4, 0xc(r3)
/* 8035E094 0035AFD4  80 03 00 10 */	lwz r0, 0x10(r3)
/* 8035E098 0035AFD8  90 9E 00 24 */	stw r4, 0x24(r30)
/* 8035E09C 0035AFDC  90 1E 00 28 */	stw r0, 0x28(r30)
/* 8035E0A0 0035AFE0  80 03 00 14 */	lwz r0, 0x14(r3)
/* 8035E0A4 0035AFE4  90 1E 00 2C */	stw r0, 0x2c(r30)
lbl_8035E0A8:
/* 8035E0A8 0035AFE8  80 01 00 B4 */	lwz r0, 0xb4(r1)
/* 8035E0AC 0035AFEC  83 E1 00 AC */	lwz r31, 0xac(r1)
/* 8035E0B0 0035AFF0  83 C1 00 A8 */	lwz r30, 0xa8(r1)
/* 8035E0B4 0035AFF4  7C 08 03 A6 */	mtlr r0
/* 8035E0B8 0035AFF8  83 A1 00 A4 */	lwz r29, 0xa4(r1)
/* 8035E0BC 0035AFFC  38 21 00 B0 */	addi r1, r1, 0xb0
/* 8035E0C0 0035B000  4E 80 00 20 */	blr 

.global __ct__7TSunMgrFPCc
__ct__7TSunMgrFPCc:
/* 8035E0C4 0035B004  7C 08 02 A6 */	mflr r0
/* 8035E0C8 0035B008  3C A0 80 3B */	lis r5, __vt__Q26JDrama8TNameRef@ha
/* 8035E0CC 0035B00C  90 01 00 04 */	stw r0, 4(r1)
/* 8035E0D0 0035B010  38 05 A8 60 */	addi r0, r5, __vt__Q26JDrama8TNameRef@l
/* 8035E0D4 0035B014  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 8035E0D8 0035B018  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 8035E0DC 0035B01C  3B E3 00 00 */	addi r31, r3, 0
/* 8035E0E0 0035B020  90 03 00 00 */	stw r0, 0(r3)
/* 8035E0E4 0035B024  7C 83 23 78 */	mr r3, r4
/* 8035E0E8 0035B028  90 9F 00 04 */	stw r4, 4(r31)
/* 8035E0EC 0035B02C  4B CE 65 A1 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 8035E0F0 0035B030  B0 7F 00 08 */	sth r3, 8(r31)
/* 8035E0F4 0035B034  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TViewObj@ha
/* 8035E0F8 0035B038  38 03 A0 C0 */	addi r0, r3, __vt__Q26JDrama8TViewObj@l
/* 8035E0FC 0035B03C  90 1F 00 00 */	stw r0, 0(r31)
/* 8035E100 0035B040  38 A0 00 00 */	li r5, 0
/* 8035E104 0035B044  3C 60 80 3B */	lis r3, __vt__17TDrawSyncCallback@ha
/* 8035E108 0035B048  B0 BF 00 0C */	sth r5, 0xc(r31)
/* 8035E10C 0035B04C  38 03 41 74 */	addi r0, r3, __vt__17TDrawSyncCallback@l
/* 8035E110 0035B050  3C 60 80 3E */	lis r3, __vt__7TSunMgr@ha
/* 8035E114 0035B054  90 1F 00 10 */	stw r0, 0x10(r31)
/* 8035E118 0035B058  38 63 4B 60 */	addi r3, r3, __vt__7TSunMgr@l
/* 8035E11C 0035B05C  38 83 00 24 */	addi r4, r3, 0x24
/* 8035E120 0035B060  90 7F 00 00 */	stw r3, 0(r31)
/* 8035E124 0035B064  38 00 FF FF */	li r0, -1
/* 8035E128 0035B068  38 7F 00 00 */	addi r3, r31, 0
/* 8035E12C 0035B06C  90 9F 00 10 */	stw r4, 0x10(r31)
/* 8035E130 0035B070  98 BF 00 14 */	stb r5, 0x14(r31)
/* 8035E134 0035B074  98 BF 00 15 */	stb r5, 0x15(r31)
/* 8035E138 0035B078  90 1F 00 18 */	stw r0, 0x18(r31)
/* 8035E13C 0035B07C  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 8035E140 0035B080  C0 02 07 64 */	lfs f0, $$22161@sda21(r2)
/* 8035E144 0035B084  D0 1F 00 20 */	stfs f0, 0x20(r31)
/* 8035E148 0035B088  D0 1F 00 24 */	stfs f0, 0x24(r31)
/* 8035E14C 0035B08C  D0 1F 00 28 */	stfs f0, 0x28(r31)
/* 8035E150 0035B090  D0 1F 00 2C */	stfs f0, 0x2c(r31)
/* 8035E154 0035B094  93 ED A8 C8 */	stw r31, gpSunMgr@sda21(r13)
/* 8035E158 0035B098  80 01 00 24 */	lwz r0, 0x24(r1)
/* 8035E15C 0035B09C  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 8035E160 0035B0A0  38 21 00 20 */	addi r1, r1, 0x20
/* 8035E164 0035B0A4  7C 08 03 A6 */	mtlr r0
/* 8035E168 0035B0A8  4E 80 00 20 */	blr 

.global __sinit_sunmgr_cpp
__sinit_sunmgr_cpp:
/* 8035E16C 0035B0AC  7C 08 02 A6 */	mflr r0
/* 8035E170 0035B0B0  3C 60 80 40 */	lis r3, $$22240@ha
/* 8035E174 0035B0B4  90 01 00 04 */	stw r0, 4(r1)
/* 8035E178 0035B0B8  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 8035E17C 0035B0BC  93 E1 00 0C */	stw r31, 0xc(r1)
/* 8035E180 0035B0C0  3B E3 06 60 */	addi r31, r3, $$22240@l
/* 8035E184 0035B0C4  88 0D 97 CC */	lbz r0, __init__smList__15JALList$$05MSBgm$$1@sda21(r13)
/* 8035E188 0035B0C8  7C 00 07 75 */	extsb. r0, r0
/* 8035E18C 0035B0CC  40 82 00 28 */	bne lbl_8035E1B4
/* 8035E190 0035B0D0  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1@sda21
/* 8035E194 0035B0D4  4B CB 0A A1 */	bl initiate__10JSUPtrListFv
/* 8035E198 0035B0D8  3C 60 80 0E */	lis r3, __dt__15JSUList$$05MSBgm$$1Fv@ha
/* 8035E19C 0035B0DC  38 83 6A 44 */	addi r4, r3, __dt__15JSUList$$05MSBgm$$1Fv@l
/* 8035E1A0 0035B0E0  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1@sda21
/* 8035E1A4 0035B0E4  38 BF 00 00 */	addi r5, r31, 0
/* 8035E1A8 0035B0E8  4B D2 45 81 */	bl __register_global_object
/* 8035E1AC 0035B0EC  38 00 00 01 */	li r0, 1
/* 8035E1B0 0035B0F0  98 0D 97 CC */	stb r0, __init__smList__15JALList$$05MSBgm$$1@sda21(r13)
lbl_8035E1B4:
/* 8035E1B4 0035B0F4  88 0D 97 CD */	lbz r0, __init__smList__24JALList$$013MSSetSoundGrp$$1@sda21(r13)
/* 8035E1B8 0035B0F8  7C 00 07 75 */	extsb. r0, r0
/* 8035E1BC 0035B0FC  40 82 00 28 */	bne lbl_8035E1E4
/* 8035E1C0 0035B100  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1@sda21
/* 8035E1C4 0035B104  4B CB 0A 71 */	bl initiate__10JSUPtrListFv
/* 8035E1C8 0035B108  3C 60 80 0E */	lis r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@ha
/* 8035E1CC 0035B10C  38 83 69 EC */	addi r4, r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@l
/* 8035E1D0 0035B110  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1@sda21
/* 8035E1D4 0035B114  38 BF 00 0C */	addi r5, r31, 0xc
/* 8035E1D8 0035B118  4B D2 45 51 */	bl __register_global_object
/* 8035E1DC 0035B11C  38 00 00 01 */	li r0, 1
/* 8035E1E0 0035B120  98 0D 97 CD */	stb r0, __init__smList__24JALList$$013MSSetSoundGrp$$1@sda21(r13)
lbl_8035E1E4:
/* 8035E1E4 0035B124  88 0D 97 CE */	lbz r0, __init__smList__21JALList$$010MSSetSound$$1@sda21(r13)
/* 8035E1E8 0035B128  7C 00 07 75 */	extsb. r0, r0
/* 8035E1EC 0035B12C  40 82 00 28 */	bne lbl_8035E214
/* 8035E1F0 0035B130  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1@sda21
/* 8035E1F4 0035B134  4B CB 0A 41 */	bl initiate__10JSUPtrListFv
/* 8035E1F8 0035B138  3C 60 80 0E */	lis r3, __dt__21JSUList$$010MSSetSound$$1Fv@ha
/* 8035E1FC 0035B13C  38 83 69 94 */	addi r4, r3, __dt__21JSUList$$010MSSetSound$$1Fv@l
/* 8035E200 0035B140  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1@sda21
/* 8035E204 0035B144  38 BF 00 18 */	addi r5, r31, 0x18
/* 8035E208 0035B148  4B D2 45 21 */	bl __register_global_object
/* 8035E20C 0035B14C  38 00 00 01 */	li r0, 1
/* 8035E210 0035B150  98 0D 97 CE */	stb r0, __init__smList__21JALList$$010MSSetSound$$1@sda21(r13)
lbl_8035E214:
/* 8035E214 0035B154  88 0D 8F 8C */	lbz r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1@sda21(r13)
/* 8035E218 0035B158  7C 00 07 75 */	extsb. r0, r0
/* 8035E21C 0035B15C  40 82 00 28 */	bne lbl_8035E244
/* 8035E220 0035B160  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1@sda21
/* 8035E224 0035B164  4B CB 0A 11 */	bl initiate__10JSUPtrListFv
/* 8035E228 0035B168  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@ha
/* 8035E22C 0035B16C  38 83 A8 0C */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@l
/* 8035E230 0035B170  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1@sda21
/* 8035E234 0035B174  38 BF 00 24 */	addi r5, r31, 0x24
/* 8035E238 0035B178  4B D2 44 F1 */	bl __register_global_object
/* 8035E23C 0035B17C  38 00 00 01 */	li r0, 1
/* 8035E240 0035B180  98 0D 8F 8C */	stb r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1@sda21(r13)
lbl_8035E244:
/* 8035E244 0035B184  88 0D 8F 8D */	lbz r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1@sda21(r13)
/* 8035E248 0035B188  7C 00 07 75 */	extsb. r0, r0
/* 8035E24C 0035B18C  40 82 00 28 */	bne lbl_8035E274
/* 8035E250 0035B190  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1@sda21
/* 8035E254 0035B194  4B CB 09 E1 */	bl initiate__10JSUPtrListFv
/* 8035E258 0035B198  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@ha
/* 8035E25C 0035B19C  38 83 A7 B4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@l
/* 8035E260 0035B1A0  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1@sda21
/* 8035E264 0035B1A4  38 BF 00 30 */	addi r5, r31, 0x30
/* 8035E268 0035B1A8  4B D2 44 C1 */	bl __register_global_object
/* 8035E26C 0035B1AC  38 00 00 01 */	li r0, 1
/* 8035E270 0035B1B0  98 0D 8F 8D */	stb r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1@sda21(r13)
lbl_8035E274:
/* 8035E274 0035B1B4  88 0D 8F 8E */	lbz r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1@sda21(r13)
/* 8035E278 0035B1B8  7C 00 07 75 */	extsb. r0, r0
/* 8035E27C 0035B1BC  40 82 00 28 */	bne lbl_8035E2A4
/* 8035E280 0035B1C0  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1@sda21
/* 8035E284 0035B1C4  4B CB 09 B1 */	bl initiate__10JSUPtrListFv
/* 8035E288 0035B1C8  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@ha
/* 8035E28C 0035B1CC  38 83 A7 5C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@l
/* 8035E290 0035B1D0  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1@sda21
/* 8035E294 0035B1D4  38 BF 00 3C */	addi r5, r31, 0x3c
/* 8035E298 0035B1D8  4B D2 44 91 */	bl __register_global_object
/* 8035E29C 0035B1DC  38 00 00 01 */	li r0, 1
/* 8035E2A0 0035B1E0  98 0D 8F 8E */	stb r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1@sda21(r13)
lbl_8035E2A4:
/* 8035E2A4 0035B1E4  88 0D 8F 8F */	lbz r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1@sda21(r13)
/* 8035E2A8 0035B1E8  7C 00 07 75 */	extsb. r0, r0
/* 8035E2AC 0035B1EC  40 82 00 28 */	bne lbl_8035E2D4
/* 8035E2B0 0035B1F0  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1@sda21
/* 8035E2B4 0035B1F4  4B CB 09 81 */	bl initiate__10JSUPtrListFv
/* 8035E2B8 0035B1F8  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@ha
/* 8035E2BC 0035B1FC  38 83 A7 04 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@l
/* 8035E2C0 0035B200  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1@sda21
/* 8035E2C4 0035B204  38 BF 00 48 */	addi r5, r31, 0x48
/* 8035E2C8 0035B208  4B D2 44 61 */	bl __register_global_object
/* 8035E2CC 0035B20C  38 00 00 01 */	li r0, 1
/* 8035E2D0 0035B210  98 0D 8F 8F */	stb r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1@sda21(r13)
lbl_8035E2D4:
/* 8035E2D4 0035B214  88 0D 8F 90 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1@sda21(r13)
/* 8035E2D8 0035B218  7C 00 07 75 */	extsb. r0, r0
/* 8035E2DC 0035B21C  40 82 00 28 */	bne lbl_8035E304
/* 8035E2E0 0035B220  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1@sda21
/* 8035E2E4 0035B224  4B CB 09 51 */	bl initiate__10JSUPtrListFv
/* 8035E2E8 0035B228  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@ha
/* 8035E2EC 0035B22C  38 83 A6 AC */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@l
/* 8035E2F0 0035B230  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1@sda21
/* 8035E2F4 0035B234  38 BF 00 54 */	addi r5, r31, 0x54
/* 8035E2F8 0035B238  4B D2 44 31 */	bl __register_global_object
/* 8035E2FC 0035B23C  38 00 00 01 */	li r0, 1
/* 8035E300 0035B240  98 0D 8F 90 */	stb r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1@sda21(r13)
lbl_8035E304:
/* 8035E304 0035B244  88 0D 8F 91 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1@sda21(r13)
/* 8035E308 0035B248  7C 00 07 75 */	extsb. r0, r0
/* 8035E30C 0035B24C  40 82 00 28 */	bne lbl_8035E334
/* 8035E310 0035B250  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1@sda21
/* 8035E314 0035B254  4B CB 09 21 */	bl initiate__10JSUPtrListFv
/* 8035E318 0035B258  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@ha
/* 8035E31C 0035B25C  38 83 A6 54 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@l
/* 8035E320 0035B260  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1@sda21
/* 8035E324 0035B264  38 BF 00 60 */	addi r5, r31, 0x60
/* 8035E328 0035B268  4B D2 44 01 */	bl __register_global_object
/* 8035E32C 0035B26C  38 00 00 01 */	li r0, 1
/* 8035E330 0035B270  98 0D 8F 91 */	stb r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1@sda21(r13)
lbl_8035E334:
/* 8035E334 0035B274  88 0D 8F 92 */	lbz r0, __init__smList__26JALList$$015JALSeModEffDist$$1@sda21(r13)
/* 8035E338 0035B278  7C 00 07 75 */	extsb. r0, r0
/* 8035E33C 0035B27C  40 82 00 28 */	bne lbl_8035E364
/* 8035E340 0035B280  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1@sda21
/* 8035E344 0035B284  4B CB 08 F1 */	bl initiate__10JSUPtrListFv
/* 8035E348 0035B288  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@ha
/* 8035E34C 0035B28C  38 83 A5 FC */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@l
/* 8035E350 0035B290  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1@sda21
/* 8035E354 0035B294  38 BF 00 6C */	addi r5, r31, 0x6c
/* 8035E358 0035B298  4B D2 43 D1 */	bl __register_global_object
/* 8035E35C 0035B29C  38 00 00 01 */	li r0, 1
/* 8035E360 0035B2A0  98 0D 8F 92 */	stb r0, __init__smList__26JALList$$015JALSeModEffDist$$1@sda21(r13)
lbl_8035E364:
/* 8035E364 0035B2A4  88 0D 8F 93 */	lbz r0, __init__smList__26JALList$$015JALSeModPitDist$$1@sda21(r13)
/* 8035E368 0035B2A8  7C 00 07 75 */	extsb. r0, r0
/* 8035E36C 0035B2AC  40 82 00 28 */	bne lbl_8035E394
/* 8035E370 0035B2B0  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1@sda21
/* 8035E374 0035B2B4  4B CB 08 C1 */	bl initiate__10JSUPtrListFv
/* 8035E378 0035B2B8  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@ha
/* 8035E37C 0035B2BC  38 83 A5 A4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@l
/* 8035E380 0035B2C0  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1@sda21
/* 8035E384 0035B2C4  38 BF 00 78 */	addi r5, r31, 0x78
/* 8035E388 0035B2C8  4B D2 43 A1 */	bl __register_global_object
/* 8035E38C 0035B2CC  38 00 00 01 */	li r0, 1
/* 8035E390 0035B2D0  98 0D 8F 93 */	stb r0, __init__smList__26JALList$$015JALSeModPitDist$$1@sda21(r13)
lbl_8035E394:
/* 8035E394 0035B2D4  88 0D 8F 94 */	lbz r0, __init__smList__26JALList$$015JALSeModVolDist$$1@sda21(r13)
/* 8035E398 0035B2D8  7C 00 07 75 */	extsb. r0, r0
/* 8035E39C 0035B2DC  40 82 00 28 */	bne lbl_8035E3C4
/* 8035E3A0 0035B2E0  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1@sda21
/* 8035E3A4 0035B2E4  4B CB 08 91 */	bl initiate__10JSUPtrListFv
/* 8035E3A8 0035B2E8  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@ha
/* 8035E3AC 0035B2EC  38 83 A5 4C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@l
/* 8035E3B0 0035B2F0  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1@sda21
/* 8035E3B4 0035B2F4  38 BF 00 84 */	addi r5, r31, 0x84
/* 8035E3B8 0035B2F8  4B D2 43 71 */	bl __register_global_object
/* 8035E3BC 0035B2FC  38 00 00 01 */	li r0, 1
/* 8035E3C0 0035B300  98 0D 8F 94 */	stb r0, __init__smList__26JALList$$015JALSeModVolDist$$1@sda21(r13)
lbl_8035E3C4:
/* 8035E3C4 0035B304  88 0D 8F 95 */	lbz r0, __init__smList__26JALList$$015JALSeModEffFunk$$1@sda21(r13)
/* 8035E3C8 0035B308  7C 00 07 75 */	extsb. r0, r0
/* 8035E3CC 0035B30C  40 82 00 28 */	bne lbl_8035E3F4
/* 8035E3D0 0035B310  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1@sda21
/* 8035E3D4 0035B314  4B CB 08 61 */	bl initiate__10JSUPtrListFv
/* 8035E3D8 0035B318  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@ha
/* 8035E3DC 0035B31C  38 83 A4 F4 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@l
/* 8035E3E0 0035B320  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1@sda21
/* 8035E3E4 0035B324  38 BF 00 90 */	addi r5, r31, 0x90
/* 8035E3E8 0035B328  4B D2 43 41 */	bl __register_global_object
/* 8035E3EC 0035B32C  38 00 00 01 */	li r0, 1
/* 8035E3F0 0035B330  98 0D 8F 95 */	stb r0, __init__smList__26JALList$$015JALSeModEffFunk$$1@sda21(r13)
lbl_8035E3F4:
/* 8035E3F4 0035B334  88 0D 8F 96 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFunk$$1@sda21(r13)
/* 8035E3F8 0035B338  7C 00 07 75 */	extsb. r0, r0
/* 8035E3FC 0035B33C  40 82 00 28 */	bne lbl_8035E424
/* 8035E400 0035B340  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1@sda21
/* 8035E404 0035B344  4B CB 08 31 */	bl initiate__10JSUPtrListFv
/* 8035E408 0035B348  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@ha
/* 8035E40C 0035B34C  38 83 A4 9C */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@l
/* 8035E410 0035B350  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1@sda21
/* 8035E414 0035B354  38 BF 00 9C */	addi r5, r31, 0x9c
/* 8035E418 0035B358  4B D2 43 11 */	bl __register_global_object
/* 8035E41C 0035B35C  38 00 00 01 */	li r0, 1
/* 8035E420 0035B360  98 0D 8F 96 */	stb r0, __init__smList__26JALList$$015JALSeModPitFunk$$1@sda21(r13)
lbl_8035E424:
/* 8035E424 0035B364  88 0D 8F 97 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFunk$$1@sda21(r13)
/* 8035E428 0035B368  7C 00 07 75 */	extsb. r0, r0
/* 8035E42C 0035B36C  40 82 00 28 */	bne lbl_8035E454
/* 8035E430 0035B370  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1@sda21
/* 8035E434 0035B374  4B CB 08 01 */	bl initiate__10JSUPtrListFv
/* 8035E438 0035B378  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@ha
/* 8035E43C 0035B37C  38 83 A4 44 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@l
/* 8035E440 0035B380  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1@sda21
/* 8035E444 0035B384  38 BF 00 A8 */	addi r5, r31, 0xa8
/* 8035E448 0035B388  4B D2 42 E1 */	bl __register_global_object
/* 8035E44C 0035B38C  38 00 00 01 */	li r0, 1
/* 8035E450 0035B390  98 0D 8F 97 */	stb r0, __init__smList__26JALList$$015JALSeModVolFunk$$1@sda21(r13)
lbl_8035E454:
/* 8035E454 0035B394  80 01 00 14 */	lwz r0, 0x14(r1)
/* 8035E458 0035B398  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 8035E45C 0035B39C  38 21 00 10 */	addi r1, r1, 0x10
/* 8035E460 0035B3A0  7C 08 03 A6 */	mtlr r0
/* 8035E464 0035B3A4  4E 80 00 20 */	blr 

.global $$216$$2drawSyncCallback__7TSunMgrFUs
$$216$$2drawSyncCallback__7TSunMgrFUs:
/* 8035E468 0035B3A8  38 63 FF F0 */	addi r3, r3, -16
/* 8035E46C 0035B3AC  4B FF F8 84 */	b drawSyncCallback__7TSunMgrFUs

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
  .4byte __sinit_sunmgr_cpp

.section .rodata, "wa"  # 0x8036FFA0 - 0x803A8380
$$21490:
	.incbin "baserom.dol", 0x3A3960, 0xC
$$21937:
	.incbin "baserom.dol", 0x3A396C, 0x14
$$22078:
	.incbin "baserom.dol", 0x3A3980, 0xC
$$22079:
	.incbin "baserom.dol", 0x3A398C, 0x10
$$22081:
	.incbin "baserom.dol", 0x3A399C, 0x14
$$22223:
	.incbin "baserom.dol", 0x3A39B0, 0xC
$$22224:
	.incbin "baserom.dol", 0x3A39BC, 0xC

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__7TSunMgr
__vt__7TSunMgr:
  .4byte 0
  .4byte 0
  .4byte __dt__7TSunMgrFv
  .4byte getType__Q26JDrama8TNameRefCFv
  .4byte load__7TSunMgrFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__Q26JDrama8TNameRefFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc
  .4byte perform__7TSunMgrFUlPQ26JDrama9TGraphics
  .4byte 0
  .4byte 0
  .4byte $$216$$2drawSyncCallback__7TSunMgrFUs
  .4byte drawSyncCallback__7TSunMgrFUs
  .4byte 0

.section .sdata, "wa"  # 0x80408AC0 - 0x804097C0
cSunWarpPointName:
	.incbin "baserom.dol", 0x3E3CD0, 0x8

.section .sdata2, "wa"  # 0x8040B460 - 0x80414020
$$22158:
	.incbin "baserom.dol", 0x3EC458, 0x4
$$22159:
	.incbin "baserom.dol", 0x3EC45C, 0x4
$$22160:
	.incbin "baserom.dol", 0x3EC460, 0x4
$$22161:
	.incbin "baserom.dol", 0x3EC464, 0x4
$$22162:
	.incbin "baserom.dol", 0x3EC468, 0x8

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
$$22240:
	.skip 0xC
$$22241:
	.skip 0xC
$$22242:
	.skip 0xC
$$22243:
	.skip 0xC
$$22244:
	.skip 0xC
$$22245:
	.skip 0xC
$$22246:
	.skip 0xC
$$22247:
	.skip 0xC
$$22248:
	.skip 0xC
$$22249:
	.skip 0xC
$$22250:
	.skip 0xC
$$22251:
	.skip 0xC
$$22252:
	.skip 0xC
$$22253:
	.skip 0xC
$$22254:
	.skip 0x10

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
.global gpSunMgr
gpSunMgr:
	.skip 0x8

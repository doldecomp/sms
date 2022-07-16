.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __ct__9THitActorFPCc
__ct__9THitActorFPCc:
/* 8011D6CC 0011A60C  7C 08 02 A6 */	mflr r0
/* 8011D6D0 0011A610  90 01 00 04 */	stw r0, 4(r1)
/* 8011D6D4 0011A614  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 8011D6D8 0011A618  93 E1 00 24 */	stw r31, 0x24(r1)
/* 8011D6DC 0011A61C  7C 7F 1B 78 */	mr r31, r3
/* 8011D6E0 0011A620  93 C1 00 20 */	stw r30, 0x20(r1)
/* 8011D6E4 0011A624  93 E1 00 10 */	stw r31, 0x10(r1)
/* 8011D6E8 0011A628  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8011D6EC 0011A62C  80 61 00 10 */	lwz r3, 0x10(r1)
/* 8011D6F0 0011A630  4B F2 34 19 */	bl __ct__Q26JDrama8TNameRefFPCc
/* 8011D6F4 0011A634  80 61 00 10 */	lwz r3, 0x10(r1)
/* 8011D6F8 0011A638  3C 80 80 3B */	lis r4, __vt__Q26JDrama8TViewObj@ha
/* 8011D6FC 0011A63C  38 04 A0 C0 */	addi r0, r4, __vt__Q26JDrama8TViewObj@l
/* 8011D700 0011A640  90 03 00 00 */	stw r0, 0(r3)
/* 8011D704 0011A644  38 63 00 0C */	addi r3, r3, 0xc
/* 8011D708 0011A648  38 80 00 00 */	li r4, 0
/* 8011D70C 0011A64C  4B F2 33 F5 */	bl "__ct__Q26JDrama10TFlagT<Us>FUs"
/* 8011D710 0011A650  83 C1 00 14 */	lwz r30, 0x14(r1)
/* 8011D714 0011A654  3C 60 80 3B */	lis r3, __vt__Q26JDrama10TPlacement@ha
/* 8011D718 0011A658  38 03 A8 B0 */	addi r0, r3, __vt__Q26JDrama10TPlacement@l
/* 8011D71C 0011A65C  90 1E 00 00 */	stw r0, 0(r30)
/* 8011D720 0011A660  38 7E 00 10 */	addi r3, r30, 0x10
/* 8011D724 0011A664  C0 22 97 70 */	lfs f1, "@1618"@sda21(r2)
/* 8011D728 0011A668  FC 40 08 90 */	fmr f2, f1
/* 8011D72C 0011A66C  FC 60 08 90 */	fmr f3, f1
/* 8011D730 0011A670  48 00 00 B9 */	bl "set<f>__Q29JGeometry8TVec3<f>Ffff"
/* 8011D734 0011A674  38 A0 00 00 */	li r5, 0
/* 8011D738 0011A678  3C 60 80 3B */	lis r3, __vt__Q26JStage7TObject@ha
/* 8011D73C 0011A67C  B0 BE 00 1C */	sth r5, 0x1c(r30)
/* 8011D740 0011A680  38 03 CE 98 */	addi r0, r3, __vt__Q26JStage7TObject@l
/* 8011D744 0011A684  3C 60 80 3B */	lis r3, __vt__Q26JStage6TActor@ha
/* 8011D748 0011A688  90 1F 00 20 */	stw r0, 0x20(r31)
/* 8011D74C 0011A68C  38 03 CD D0 */	addi r0, r3, __vt__Q26JStage6TActor@l
/* 8011D750 0011A690  3C 60 80 3B */	lis r3, __vt__Q26JDrama6TActor@ha
/* 8011D754 0011A694  90 1F 00 20 */	stw r0, 0x20(r31)
/* 8011D758 0011A698  38 63 A0 20 */	addi r3, r3, __vt__Q26JDrama6TActor@l
/* 8011D75C 0011A69C  90 7F 00 00 */	stw r3, 0(r31)
/* 8011D760 0011A6A0  38 03 00 24 */	addi r0, r3, 0x24
/* 8011D764 0011A6A4  3C 60 80 3B */	lis r3, __vt__9THitActor@ha
/* 8011D768 0011A6A8  90 1F 00 20 */	stw r0, 0x20(r31)
/* 8011D76C 0011A6AC  38 83 53 A0 */	addi r4, r3, __vt__9THitActor@l
/* 8011D770 0011A6B0  38 04 00 24 */	addi r0, r4, 0x24
/* 8011D774 0011A6B4  C0 02 97 74 */	lfs f0, "@1619"@sda21(r2)
/* 8011D778 0011A6B8  7F E3 FB 78 */	mr r3, r31
/* 8011D77C 0011A6BC  D0 1F 00 24 */	stfs f0, 0x24(r31)
/* 8011D780 0011A6C0  D0 1F 00 28 */	stfs f0, 0x28(r31)
/* 8011D784 0011A6C4  D0 1F 00 2C */	stfs f0, 0x2c(r31)
/* 8011D788 0011A6C8  C0 02 97 70 */	lfs f0, "@1618"@sda21(r2)
/* 8011D78C 0011A6CC  D0 1F 00 30 */	stfs f0, 0x30(r31)
/* 8011D790 0011A6D0  D0 1F 00 34 */	stfs f0, 0x34(r31)
/* 8011D794 0011A6D4  D0 1F 00 38 */	stfs f0, 0x38(r31)
/* 8011D798 0011A6D8  90 BF 00 3C */	stw r5, 0x3c(r31)
/* 8011D79C 0011A6DC  90 BF 00 40 */	stw r5, 0x40(r31)
/* 8011D7A0 0011A6E0  90 9F 00 00 */	stw r4, 0(r31)
/* 8011D7A4 0011A6E4  90 1F 00 20 */	stw r0, 0x20(r31)
/* 8011D7A8 0011A6E8  90 BF 00 44 */	stw r5, 0x44(r31)
/* 8011D7AC 0011A6EC  B0 BF 00 48 */	sth r5, 0x48(r31)
/* 8011D7B0 0011A6F0  B0 BF 00 4A */	sth r5, 0x4a(r31)
/* 8011D7B4 0011A6F4  90 BF 00 4C */	stw r5, 0x4c(r31)
/* 8011D7B8 0011A6F8  D0 1F 00 50 */	stfs f0, 0x50(r31)
/* 8011D7BC 0011A6FC  D0 1F 00 54 */	stfs f0, 0x54(r31)
/* 8011D7C0 0011A700  D0 1F 00 58 */	stfs f0, 0x58(r31)
/* 8011D7C4 0011A704  D0 1F 00 5C */	stfs f0, 0x5c(r31)
/* 8011D7C8 0011A708  D0 1F 00 60 */	stfs f0, 0x60(r31)
/* 8011D7CC 0011A70C  90 BF 00 64 */	stw r5, 0x64(r31)
/* 8011D7D0 0011A710  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 8011D7D4 0011A714  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 8011D7D8 0011A718  83 C1 00 20 */	lwz r30, 0x20(r1)
/* 8011D7DC 0011A71C  7C 08 03 A6 */	mtlr r0
/* 8011D7E0 0011A720  38 21 00 28 */	addi r1, r1, 0x28
/* 8011D7E4 0011A724  4E 80 00 20 */	blr 

"set<f>__Q29JGeometry8TVec3<f>Ffff":
/* 8011D7E8 0011A728  D0 23 00 00 */	stfs f1, 0(r3)
/* 8011D7EC 0011A72C  D0 43 00 04 */	stfs f2, 4(r3)
/* 8011D7F0 0011A730  D0 63 00 08 */	stfs f3, 8(r3)
/* 8011D7F4 0011A734  4E 80 00 20 */	blr 

.global initHitActor__9THitActorFUlUsiffff
initHitActor__9THitActorFUlUsiffff:
/* 8011D7F8 0011A738  7C 08 02 A6 */	mflr r0
/* 8011D7FC 0011A73C  90 01 00 04 */	stw r0, 4(r1)
/* 8011D800 0011A740  94 21 FF B0 */	stwu r1, -0x50(r1)
/* 8011D804 0011A744  DB E1 00 48 */	stfd f31, 0x48(r1)
/* 8011D808 0011A748  FF E0 20 90 */	fmr f31, f4
/* 8011D80C 0011A74C  DB C1 00 40 */	stfd f30, 0x40(r1)
/* 8011D810 0011A750  FF C0 18 90 */	fmr f30, f3
/* 8011D814 0011A754  DB A1 00 38 */	stfd f29, 0x38(r1)
/* 8011D818 0011A758  FF A0 10 90 */	fmr f29, f2
/* 8011D81C 0011A75C  DB 81 00 30 */	stfd f28, 0x30(r1)
/* 8011D820 0011A760  FF 80 08 90 */	fmr f28, f1
/* 8011D824 0011A764  93 E1 00 2C */	stw r31, 0x2c(r1)
/* 8011D828 0011A768  7C DF 33 78 */	mr r31, r6
/* 8011D82C 0011A76C  93 C1 00 28 */	stw r30, 0x28(r1)
/* 8011D830 0011A770  7C 7E 1B 78 */	mr r30, r3
/* 8011D834 0011A774  90 83 00 4C */	stw r4, 0x4c(r3)
/* 8011D838 0011A778  B0 A3 00 4A */	sth r5, 0x4a(r3)
/* 8011D83C 0011A77C  A0 03 00 4A */	lhz r0, 0x4a(r3)
/* 8011D840 0011A780  54 03 10 3A */	slwi r3, r0, 2
/* 8011D844 0011A784  4B EE F1 6D */	bl __nwa__FUl
/* 8011D848 0011A788  38 80 00 00 */	li r4, 0
/* 8011D84C 0011A78C  90 7E 00 44 */	stw r3, 0x44(r30)
/* 8011D850 0011A790  38 A4 00 00 */	addi r5, r4, 0
/* 8011D854 0011A794  38 C0 00 00 */	li r6, 0
/* 8011D858 0011A798  48 00 00 14 */	b lbl_8011D86C
lbl_8011D85C:
/* 8011D85C 0011A79C  80 7E 00 44 */	lwz r3, 0x44(r30)
/* 8011D860 0011A7A0  38 C6 00 01 */	addi r6, r6, 1
/* 8011D864 0011A7A4  7C A3 21 2E */	stwx r5, r3, r4
/* 8011D868 0011A7A8  38 84 00 04 */	addi r4, r4, 4
lbl_8011D86C:
/* 8011D86C 0011A7AC  A0 1E 00 4A */	lhz r0, 0x4a(r30)
/* 8011D870 0011A7B0  7C 06 00 00 */	cmpw r6, r0
/* 8011D874 0011A7B4  41 80 FF E8 */	blt lbl_8011D85C
/* 8011D878 0011A7B8  80 1E 00 64 */	lwz r0, 0x64(r30)
/* 8011D87C 0011A7BC  38 7E 00 00 */	addi r3, r30, 0
/* 8011D880 0011A7C0  7C 00 FB 78 */	or r0, r0, r31
/* 8011D884 0011A7C4  90 1E 00 64 */	stw r0, 0x64(r30)
/* 8011D888 0011A7C8  D3 9E 00 50 */	stfs f28, 0x50(r30)
/* 8011D88C 0011A7CC  D3 BE 00 54 */	stfs f29, 0x54(r30)
/* 8011D890 0011A7D0  D3 DE 00 58 */	stfs f30, 0x58(r30)
/* 8011D894 0011A7D4  D3 FE 00 5C */	stfs f31, 0x5c(r30)
/* 8011D898 0011A7D8  48 00 00 4D */	bl calcEntryRadius__9THitActorFv
/* 8011D89C 0011A7DC  80 01 00 54 */	lwz r0, 0x54(r1)
/* 8011D8A0 0011A7E0  CB E1 00 48 */	lfd f31, 0x48(r1)
/* 8011D8A4 0011A7E4  CB C1 00 40 */	lfd f30, 0x40(r1)
/* 8011D8A8 0011A7E8  7C 08 03 A6 */	mtlr r0
/* 8011D8AC 0011A7EC  CB A1 00 38 */	lfd f29, 0x38(r1)
/* 8011D8B0 0011A7F0  CB 81 00 30 */	lfd f28, 0x30(r1)
/* 8011D8B4 0011A7F4  83 E1 00 2C */	lwz r31, 0x2c(r1)
/* 8011D8B8 0011A7F8  83 C1 00 28 */	lwz r30, 0x28(r1)
/* 8011D8BC 0011A7FC  38 21 00 50 */	addi r1, r1, 0x50
/* 8011D8C0 0011A800  4E 80 00 20 */	blr 

.global perform__9THitActorFUlPQ26JDrama9TGraphics
perform__9THitActorFUlPQ26JDrama9TGraphics:
/* 8011D8C4 0011A804  7C 08 02 A6 */	mflr r0
/* 8011D8C8 0011A808  90 01 00 04 */	stw r0, 4(r1)
/* 8011D8CC 0011A80C  94 21 FF F8 */	stwu r1, -8(r1)
/* 8011D8D0 0011A810  4B F2 32 81 */	bl perform__Q26JDrama6TActorFUlPQ26JDrama9TGraphics
/* 8011D8D4 0011A814  80 01 00 0C */	lwz r0, 0xc(r1)
/* 8011D8D8 0011A818  38 21 00 08 */	addi r1, r1, 8
/* 8011D8DC 0011A81C  7C 08 03 A6 */	mtlr r0
/* 8011D8E0 0011A820  4E 80 00 20 */	blr 

.global calcEntryRadius__9THitActorFv
calcEntryRadius__9THitActorFv:
/* 8011D8E4 0011A824  94 21 FF C0 */	stwu r1, -0x40(r1)
/* 8011D8E8 0011A828  C0 03 00 50 */	lfs f0, 0x50(r3)
/* 8011D8EC 0011A82C  C0 23 00 58 */	lfs f1, 0x58(r3)
/* 8011D8F0 0011A830  FC 00 08 40 */	fcmpo cr0, f0, f1
/* 8011D8F4 0011A834  40 81 00 0C */	ble lbl_8011D900
/* 8011D8F8 0011A838  FC 40 00 90 */	fmr f2, f0
/* 8011D8FC 0011A83C  48 00 00 08 */	b lbl_8011D904
lbl_8011D900:
/* 8011D900 0011A840  FC 40 08 90 */	fmr f2, f1
lbl_8011D904:
/* 8011D904 0011A844  C0 03 00 54 */	lfs f0, 0x54(r3)
/* 8011D908 0011A848  C0 23 00 5C */	lfs f1, 0x5c(r3)
/* 8011D90C 0011A84C  FC 00 08 40 */	fcmpo cr0, f0, f1
/* 8011D910 0011A850  40 81 00 08 */	ble lbl_8011D918
/* 8011D914 0011A854  48 00 00 08 */	b lbl_8011D91C
lbl_8011D918:
/* 8011D918 0011A858  FC 00 08 90 */	fmr f0, f1
lbl_8011D91C:
/* 8011D91C 0011A85C  EC 20 00 32 */	fmuls f1, f0, f0
/* 8011D920 0011A860  C0 02 97 70 */	lfs f0, "@1618"@sda21(r2)
/* 8011D924 0011A864  EC 42 08 BA */	fmadds f2, f2, f2, f1
/* 8011D928 0011A868  FC 02 00 40 */	fcmpo cr0, f2, f0
/* 8011D92C 0011A86C  40 81 00 28 */	ble lbl_8011D954
/* 8011D930 0011A870  FC 20 10 34 */	frsqrte f1, f2
/* 8011D934 0011A874  C0 02 97 78 */	lfs f0, "@1660"@sda21(r2)
/* 8011D938 0011A878  FC 22 00 72 */	fmul f1, f2, f1
/* 8011D93C 0011A87C  FC 20 08 18 */	frsp f1, f1
/* 8011D940 0011A880  D0 21 00 30 */	stfs f1, 0x30(r1)
/* 8011D944 0011A884  C0 21 00 30 */	lfs f1, 0x30(r1)
/* 8011D948 0011A888  EC 00 00 72 */	fmuls f0, f0, f1
/* 8011D94C 0011A88C  D0 03 00 60 */	stfs f0, 0x60(r3)
/* 8011D950 0011A890  48 00 00 08 */	b lbl_8011D958
lbl_8011D954:
/* 8011D954 0011A894  D0 03 00 60 */	stfs f0, 0x60(r3)
lbl_8011D958:
/* 8011D958 0011A898  38 21 00 40 */	addi r1, r1, 0x40
/* 8011D95C 0011A89C  4E 80 00 20 */	blr 

.global "@32@__dt__9THitActorFv"
"@32@__dt__9THitActorFv":
/* 8011D960 0011A8A0  38 63 FF E0 */	addi r3, r3, -32
/* 8011D964 0011A8A4  4B FE 76 54 */	b __dt__9THitActorFv

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__9THitActor
__vt__9THitActor:
  .4byte 0
  .4byte 0
  .4byte __dt__9THitActorFv
  .4byte getType__Q26JDrama6TActorCFv
  .4byte load__Q26JDrama6TActorFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__Q26JDrama8TNameRefFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc
  .4byte perform__9THitActorFUlPQ26JDrama9TGraphics
  .4byte 0
  .4byte 0
  .4byte "@32@__dt__9THitActorFv"
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
  .4byte receiveMessage__9THitActorFP9THitActorUl
  .4byte 0

.section .sdata2, "a"  # 0x8040B460 - 0x80414020
.balign 8
"@1618":
  .4byte 0
"@1619":
	.4byte 0x3F800000
"@1660":
	.4byte 0x3FB504F3
	.4byte 0

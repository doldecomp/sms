.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __dt__Q26JStage7TCameraFv
__dt__Q26JStage7TCameraFv:
/* 800818A0 0007E7E0  7C 08 02 A6 */	mflr r0
/* 800818A4 0007E7E4  90 01 00 04 */	stw r0, 4(r1)
/* 800818A8 0007E7E8  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 800818AC 0007E7EC  93 E1 00 14 */	stw r31, 0x14(r1)
/* 800818B0 0007E7F0  3B E4 00 00 */	addi r31, r4, 0
/* 800818B4 0007E7F4  93 C1 00 10 */	stw r30, 0x10(r1)
/* 800818B8 0007E7F8  7C 7E 1B 79 */	or. r30, r3, r3
/* 800818BC 0007E7FC  41 82 00 2C */	beq lbl_800818E8
/* 800818C0 0007E800  3C 60 80 3B */	lis r3, __vt__Q26JStage7TCamera@ha
/* 800818C4 0007E804  38 03 CD 48 */	addi r0, r3, __vt__Q26JStage7TCamera@l
/* 800818C8 0007E808  90 1E 00 00 */	stw r0, 0(r30)
/* 800818CC 0007E80C  38 7E 00 00 */	addi r3, r30, 0
/* 800818D0 0007E810  38 80 00 00 */	li r4, 0
/* 800818D4 0007E814  48 00 02 11 */	bl __dt__Q26JStage7TObjectFv
/* 800818D8 0007E818  7F E0 07 35 */	extsh. r0, r31
/* 800818DC 0007E81C  40 81 00 0C */	ble lbl_800818E8
/* 800818E0 0007E820  7F C3 F3 78 */	mr r3, r30
/* 800818E4 0007E824  4B F8 B1 CD */	bl __dl__FPv
lbl_800818E8:
/* 800818E8 0007E828  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 800818EC 0007E82C  7F C3 F3 78 */	mr r3, r30
/* 800818F0 0007E830  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 800818F4 0007E834  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 800818F8 0007E838  7C 08 03 A6 */	mtlr r0
/* 800818FC 0007E83C  38 21 00 18 */	addi r1, r1, 0x18
/* 80081900 0007E840  4E 80 00 20 */	blr 

.global JSGFGetType__Q26JStage7TCameraCFv
JSGFGetType__Q26JStage7TCameraCFv:
/* 80081904 0007E844  38 60 00 03 */	li r3, 3
/* 80081908 0007E848  4E 80 00 20 */	blr 

.global JSGGetProjectionType__Q26JStage7TCameraCFv
JSGGetProjectionType__Q26JStage7TCameraCFv:
/* 8008190C 0007E84C  38 60 00 01 */	li r3, 1
/* 80081910 0007E850  4E 80 00 20 */	blr 

.global JSGSetProjectionType__Q26JStage7TCameraFQ26JStage18TECameraProjection
JSGSetProjectionType__Q26JStage7TCameraFQ26JStage18TECameraProjection:
/* 80081914 0007E854  4E 80 00 20 */	blr 

.global JSGGetProjectionNear__Q26JStage7TCameraCFv
JSGGetProjectionNear__Q26JStage7TCameraCFv:
/* 80081918 0007E858  C0 22 89 38 */	lfs f1, $$2127-_SDA2_BASE_(r2)
/* 8008191C 0007E85C  4E 80 00 20 */	blr 

.global JSGSetProjectionNear__Q26JStage7TCameraFf
JSGSetProjectionNear__Q26JStage7TCameraFf:
/* 80081920 0007E860  4E 80 00 20 */	blr 

.global JSGGetProjectionFar__Q26JStage7TCameraCFv
JSGGetProjectionFar__Q26JStage7TCameraCFv:
/* 80081924 0007E864  C0 22 89 3C */	lfs f1, $$2130-_SDA2_BASE_(r2)
/* 80081928 0007E868  4E 80 00 20 */	blr 

.global JSGSetProjectionFar__Q26JStage7TCameraFf
JSGSetProjectionFar__Q26JStage7TCameraFf:
/* 8008192C 0007E86C  4E 80 00 20 */	blr 

.global JSGGetProjectionFovy__Q26JStage7TCameraCFv
JSGGetProjectionFovy__Q26JStage7TCameraCFv:
/* 80081930 0007E870  C0 22 89 38 */	lfs f1, $$2127-_SDA2_BASE_(r2)
/* 80081934 0007E874  4E 80 00 20 */	blr 

.global JSGSetProjectionFovy__Q26JStage7TCameraFf
JSGSetProjectionFovy__Q26JStage7TCameraFf:
/* 80081938 0007E878  4E 80 00 20 */	blr 

.global JSGGetProjectionAspect__Q26JStage7TCameraCFv
JSGGetProjectionAspect__Q26JStage7TCameraCFv:
/* 8008193C 0007E87C  C0 22 89 38 */	lfs f1, $$2127-_SDA2_BASE_(r2)
/* 80081940 0007E880  4E 80 00 20 */	blr 

.global JSGSetProjectionAspect__Q26JStage7TCameraFf
JSGSetProjectionAspect__Q26JStage7TCameraFf:
/* 80081944 0007E884  4E 80 00 20 */	blr 

.global JSGGetProjectionField__Q26JStage7TCameraCFPf
JSGGetProjectionField__Q26JStage7TCameraCFPf:
/* 80081948 0007E888  4E 80 00 20 */	blr 

.global JSGSetProjectionField__Q26JStage7TCameraFPCf
JSGSetProjectionField__Q26JStage7TCameraFPCf:
/* 8008194C 0007E88C  4E 80 00 20 */	blr 

.global JSGGetViewType__Q26JStage7TCameraCFv
JSGGetViewType__Q26JStage7TCameraCFv:
/* 80081950 0007E890  38 60 00 01 */	li r3, 1
/* 80081954 0007E894  4E 80 00 20 */	blr 

.global JSGSetViewType__Q26JStage7TCameraFQ26JStage12TECameraView
JSGSetViewType__Q26JStage7TCameraFQ26JStage12TECameraView:
/* 80081958 0007E898  4E 80 00 20 */	blr 

.global JSGGetViewPosition__Q26JStage7TCameraCFP3Vec
JSGGetViewPosition__Q26JStage7TCameraCFP3Vec:
/* 8008195C 0007E89C  4E 80 00 20 */	blr 

.global JSGSetViewPosition__Q26JStage7TCameraFRC3Vec
JSGSetViewPosition__Q26JStage7TCameraFRC3Vec:
/* 80081960 0007E8A0  4E 80 00 20 */	blr 

.global JSGGetViewUpVector__Q26JStage7TCameraCFP3Vec
JSGGetViewUpVector__Q26JStage7TCameraCFP3Vec:
/* 80081964 0007E8A4  4E 80 00 20 */	blr 

.global JSGSetViewUpVector__Q26JStage7TCameraFRC3Vec
JSGSetViewUpVector__Q26JStage7TCameraFRC3Vec:
/* 80081968 0007E8A8  4E 80 00 20 */	blr 

.global JSGGetViewTargetPosition__Q26JStage7TCameraCFP3Vec
JSGGetViewTargetPosition__Q26JStage7TCameraCFP3Vec:
/* 8008196C 0007E8AC  4E 80 00 20 */	blr 

.global JSGSetViewTargetPosition__Q26JStage7TCameraFRC3Vec
JSGSetViewTargetPosition__Q26JStage7TCameraFRC3Vec:
/* 80081970 0007E8B0  4E 80 00 20 */	blr 

.global JSGGetViewRoll__Q26JStage7TCameraCFv
JSGGetViewRoll__Q26JStage7TCameraCFv:
/* 80081974 0007E8B4  C0 22 89 38 */	lfs f1, $$2127-_SDA2_BASE_(r2)
/* 80081978 0007E8B8  4E 80 00 20 */	blr 

.global JSGSetViewRoll__Q26JStage7TCameraFf
JSGSetViewRoll__Q26JStage7TCameraFf:
/* 8008197C 0007E8BC  4E 80 00 20 */	blr 

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__Q26JStage7TCamera
__vt__Q26JStage7TCamera:
	.incbin "baserom.dol", 0x3A9D48, 0x88

.section .sdata2, "wa"  # 0x8040B460 - 0x80414020
.global $$2127
$$2127:
	.incbin "baserom.dol", 0x3E4638, 0x4
.global $$2130
$$2130:
	.incbin "baserom.dol", 0x3E463C, 0x4

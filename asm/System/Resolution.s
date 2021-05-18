.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global SMSGetGCLogoVideoHeight__Fv
SMSGetGCLogoVideoHeight__Fv:
/* 800FC5EC 000F952C  7C 08 02 A6 */	mflr r0
/* 800FC5F0 000F9530  90 01 00 04 */	stw r0, 4(r1)
/* 800FC5F4 000F9534  94 21 FF F8 */	stwu r1, -8(r1)
/* 800FC5F8 000F9538  48 00 00 B9 */	bl SMSGetGameVideoHeight__Fv
/* 800FC5FC 000F953C  80 01 00 0C */	lwz r0, 0xc(r1)
/* 800FC600 000F9540  38 21 00 08 */	addi r1, r1, 8
/* 800FC604 000F9544  7C 08 03 A6 */	mtlr r0
/* 800FC608 000F9548  4E 80 00 20 */	blr 

.global SMSGetGCLogoVideoWidth__Fv
SMSGetGCLogoVideoWidth__Fv:
/* 800FC60C 000F954C  38 60 02 80 */	li r3, 0x280
/* 800FC610 000F9550  4E 80 00 20 */	blr 

.global SMSGetGCLogoRenderHeight__Fv
SMSGetGCLogoRenderHeight__Fv:
/* 800FC614 000F9554  38 60 01 C0 */	li r3, 0x1c0
/* 800FC618 000F9558  4E 80 00 20 */	blr 

.global SMSGetGCLogoRenderWidth__Fv
SMSGetGCLogoRenderWidth__Fv:
/* 800FC61C 000F955C  38 60 02 80 */	li r3, 0x280
/* 800FC620 000F9560  4E 80 00 20 */	blr 

.global SMSGetTitleRenderHeight__Fv
SMSGetTitleRenderHeight__Fv:
/* 800FC624 000F9564  38 60 01 C0 */	li r3, 0x1c0
/* 800FC628 000F9568  4E 80 00 20 */	blr 

.global SMSGetTitleRenderWidth__Fv
SMSGetTitleRenderWidth__Fv:
/* 800FC62C 000F956C  38 60 02 80 */	li r3, 0x280
/* 800FC630 000F9570  4E 80 00 20 */	blr 

.global SMSGetGameRenderHeight__Fv
SMSGetGameRenderHeight__Fv:
/* 800FC634 000F9574  38 60 01 C0 */	li r3, 0x1c0
/* 800FC638 000F9578  4E 80 00 20 */	blr 

.global SMSGetGameRenderWidth__Fv
SMSGetGameRenderWidth__Fv:
/* 800FC63C 000F957C  38 60 02 80 */	li r3, 0x280
/* 800FC640 000F9580  4E 80 00 20 */	blr 

.global SMSGetTitleVideoHeight__Fv
SMSGetTitleVideoHeight__Fv:
/* 800FC644 000F9584  7C 08 02 A6 */	mflr r0
/* 800FC648 000F9588  90 01 00 04 */	stw r0, 4(r1)
/* 800FC64C 000F958C  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 800FC650 000F9590  93 E1 00 0C */	stw r31, 0xc(r1)
/* 800FC654 000F9594  3B E0 01 C0 */	li r31, 0x1c0
/* 800FC658 000F9598  4B F9 E9 65 */	bl VIGetTvFormat
/* 800FC65C 000F959C  2C 03 00 02 */	cmpwi r3, 2
/* 800FC660 000F95A0  41 82 00 24 */	beq lbl_800FC684
/* 800FC664 000F95A4  40 80 00 14 */	bge lbl_800FC678
/* 800FC668 000F95A8  2C 03 00 00 */	cmpwi r3, 0
/* 800FC66C 000F95AC  41 82 00 18 */	beq lbl_800FC684
/* 800FC670 000F95B0  40 80 00 1C */	bge lbl_800FC68C
/* 800FC674 000F95B4  48 00 00 1C */	b lbl_800FC690
lbl_800FC678:
/* 800FC678 000F95B8  2C 03 00 05 */	cmpwi r3, 5
/* 800FC67C 000F95BC  41 82 00 08 */	beq lbl_800FC684
/* 800FC680 000F95C0  48 00 00 10 */	b lbl_800FC690
lbl_800FC684:
/* 800FC684 000F95C4  3B E0 01 C0 */	li r31, 0x1c0
/* 800FC688 000F95C8  48 00 00 08 */	b lbl_800FC690
lbl_800FC68C:
/* 800FC68C 000F95CC  3B E0 02 12 */	li r31, 0x212
lbl_800FC690:
/* 800FC690 000F95D0  80 01 00 14 */	lwz r0, 0x14(r1)
/* 800FC694 000F95D4  7F E3 FB 78 */	mr r3, r31
/* 800FC698 000F95D8  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 800FC69C 000F95DC  38 21 00 10 */	addi r1, r1, 0x10
/* 800FC6A0 000F95E0  7C 08 03 A6 */	mtlr r0
/* 800FC6A4 000F95E4  4E 80 00 20 */	blr 

.global SMSGetTitleVideoWidth__Fv
SMSGetTitleVideoWidth__Fv:
/* 800FC6A8 000F95E8  38 60 02 94 */	li r3, 0x294
/* 800FC6AC 000F95EC  4E 80 00 20 */	blr 

.global SMSGetGameVideoHeight__Fv
SMSGetGameVideoHeight__Fv:
/* 800FC6B0 000F95F0  7C 08 02 A6 */	mflr r0
/* 800FC6B4 000F95F4  90 01 00 04 */	stw r0, 4(r1)
/* 800FC6B8 000F95F8  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 800FC6BC 000F95FC  93 E1 00 0C */	stw r31, 0xc(r1)
/* 800FC6C0 000F9600  3B E0 01 C0 */	li r31, 0x1c0
/* 800FC6C4 000F9604  4B F9 E8 F9 */	bl VIGetTvFormat
/* 800FC6C8 000F9608  2C 03 00 02 */	cmpwi r3, 2
/* 800FC6CC 000F960C  41 82 00 24 */	beq lbl_800FC6F0
/* 800FC6D0 000F9610  40 80 00 14 */	bge lbl_800FC6E4
/* 800FC6D4 000F9614  2C 03 00 00 */	cmpwi r3, 0
/* 800FC6D8 000F9618  41 82 00 18 */	beq lbl_800FC6F0
/* 800FC6DC 000F961C  40 80 00 1C */	bge lbl_800FC6F8
/* 800FC6E0 000F9620  48 00 00 1C */	b lbl_800FC6FC
lbl_800FC6E4:
/* 800FC6E4 000F9624  2C 03 00 05 */	cmpwi r3, 5
/* 800FC6E8 000F9628  41 82 00 08 */	beq lbl_800FC6F0
/* 800FC6EC 000F962C  48 00 00 10 */	b lbl_800FC6FC
lbl_800FC6F0:
/* 800FC6F0 000F9630  3B E0 01 C0 */	li r31, 0x1c0
/* 800FC6F4 000F9634  48 00 00 08 */	b lbl_800FC6FC
lbl_800FC6F8:
/* 800FC6F8 000F9638  3B E0 02 12 */	li r31, 0x212
lbl_800FC6FC:
/* 800FC6FC 000F963C  80 01 00 14 */	lwz r0, 0x14(r1)
/* 800FC700 000F9640  7F E3 FB 78 */	mr r3, r31
/* 800FC704 000F9644  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 800FC708 000F9648  38 21 00 10 */	addi r1, r1, 0x10
/* 800FC70C 000F964C  7C 08 03 A6 */	mtlr r0
/* 800FC710 000F9650  4E 80 00 20 */	blr 

.global SMSGetGameVideoWidth__Fv
SMSGetGameVideoWidth__Fv:
/* 800FC714 000F9654  38 60 02 94 */	li r3, 0x294
/* 800FC718 000F9658  4E 80 00 20 */	blr 
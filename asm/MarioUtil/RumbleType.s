.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global getIndex__10RumbleTypeFPc
getIndex__10RumbleTypeFPc:
/* 800D0914 000CD854  7C 08 02 A6 */	mflr r0
/* 800D0918 000CD858  3C 80 80 3B */	lis r4, _info@ha
/* 800D091C 000CD85C  90 01 00 04 */	stw r0, 4(r1)
/* 800D0920 000CD860  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 800D0924 000CD864  BF 61 00 14 */	stmw r27, 0x14(r1)
/* 800D0928 000CD868  3B 63 00 00 */	addi r27, r3, 0
/* 800D092C 000CD86C  3B C4 33 78 */	addi r30, r4, _info@l
/* 800D0930 000CD870  3B 80 00 00 */	li r28, 0
/* 800D0934 000CD874  3B E0 00 00 */	li r31, 0
/* 800D0938 000CD878  48 00 00 2C */	b lbl_800D0964
lbl_800D093C:
/* 800D093C 000CD87C  7F BE FA 14 */	add r29, r30, r31
/* 800D0940 000CD880  80 9D 00 04 */	lwz r4, 4(r29)
/* 800D0944 000CD884  7F 63 DB 78 */	mr r3, r27
/* 800D0948 000CD888  4B FB 53 15 */	bl strcmp
/* 800D094C 000CD88C  2C 03 00 00 */	cmpwi r3, 0
/* 800D0950 000CD890  40 82 00 0C */	bne lbl_800D095C
/* 800D0954 000CD894  80 7D 00 00 */	lwz r3, 0(r29)
/* 800D0958 000CD898  48 00 00 1C */	b lbl_800D0974
lbl_800D095C:
/* 800D095C 000CD89C  3B 9C 00 01 */	addi r28, r28, 1
/* 800D0960 000CD8A0  3B FF 00 08 */	addi r31, r31, 8
lbl_800D0964:
/* 800D0964 000CD8A4  80 0D 84 3C */	lwz r0, channelNum@sda21(r13)
/* 800D0968 000CD8A8  7C 1C 00 00 */	cmpw r28, r0
/* 800D096C 000CD8AC  41 80 FF D0 */	blt lbl_800D093C
/* 800D0970 000CD8B0  38 60 FF FF */	li r3, -1
lbl_800D0974:
/* 800D0974 000CD8B4  BB 61 00 14 */	lmw r27, 0x14(r1)
/* 800D0978 000CD8B8  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 800D097C 000CD8BC  38 21 00 28 */	addi r1, r1, 0x28
/* 800D0980 000CD8C0  7C 08 03 A6 */	mtlr r0
/* 800D0984 000CD8C4  4E 80 00 20 */	blr 

.section .rodata, "wa"  # 0x8036FFA0 - 0x803A8380
$$218:
	.incbin "baserom.dol", 0x36F7B0, 0x10
$$219:
	.incbin "baserom.dol", 0x36F7C0, 0x10
$$220:
	.incbin "baserom.dol", 0x36F7D0, 0x10
$$221:
	.incbin "baserom.dol", 0x36F7E0, 0x14
$$222:
	.incbin "baserom.dol", 0x36F7F4, 0x14
$$223:
	.incbin "baserom.dol", 0x36F808, 0x10
$$224:
	.incbin "baserom.dol", 0x36F818, 0x10
$$225:
	.incbin "baserom.dol", 0x36F828, 0x10
$$226:
	.incbin "baserom.dol", 0x36F838, 0x10
$$227:
	.incbin "baserom.dol", 0x36F848, 0x14
$$228:
	.incbin "baserom.dol", 0x36F85C, 0x10
$$229:
	.incbin "baserom.dol", 0x36F86C, 0x10
$$230:
	.incbin "baserom.dol", 0x36F87C, 0x10
$$231:
	.incbin "baserom.dol", 0x36F88C, 0x10
$$232:
	.incbin "baserom.dol", 0x36F89C, 0x10
$$233:
	.incbin "baserom.dol", 0x36F8AC, 0x10
$$234:
	.incbin "baserom.dol", 0x36F8BC, 0x10
$$235:
	.incbin "baserom.dol", 0x36F8CC, 0x10
$$236:
	.incbin "baserom.dol", 0x36F8DC, 0x10
$$237:
	.incbin "baserom.dol", 0x36F8EC, 0x10
$$238:
	.incbin "baserom.dol", 0x36F8FC, 0xC
$$239:
	.incbin "baserom.dol", 0x36F908, 0xC
$$240:
	.incbin "baserom.dol", 0x36F914, 0x18
$$266:
	.incbin "baserom.dol", 0x36F92C, 0x14

.section .data, "wa"  # 0x803A8380 - 0x803E6000
_info:
	.incbin "baserom.dol", 0x3B0378, 0xB8

.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __ct__7J3DNodeFv
__ct__7J3DNodeFv:
/* 80029534 00026474  3C 80 80 3B */	lis r4, __vt__7J3DNode@ha
/* 80029538 00026478  38 04 98 78 */	addi r0, r4, __vt__7J3DNode@l
/* 8002953C 0002647C  90 03 00 00 */	stw r0, 0(r3)
/* 80029540 00026480  38 00 00 00 */	li r0, 0
/* 80029544 00026484  90 03 00 04 */	stw r0, 4(r3)
/* 80029548 00026488  90 03 00 08 */	stw r0, 8(r3)
/* 8002954C 0002648C  90 03 00 10 */	stw r0, 0x10(r3)
/* 80029550 00026490  90 03 00 14 */	stw r0, 0x14(r3)
/* 80029554 00026494  4E 80 00 20 */	blr 

.global __dt__7J3DNodeFv
__dt__7J3DNodeFv:
/* 80029558 00026498  7C 08 02 A6 */	mflr r0
/* 8002955C 0002649C  90 01 00 04 */	stw r0, 4(r1)
/* 80029560 000264A0  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80029564 000264A4  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80029568 000264A8  7C 7F 1B 79 */	or. r31, r3, r3
/* 8002956C 000264AC  41 82 00 20 */	beq lbl_8002958C
/* 80029570 000264B0  3C 60 80 3B */	lis r3, __vt__7J3DNode@ha
/* 80029574 000264B4  38 63 98 78 */	addi r3, r3, __vt__7J3DNode@l
/* 80029578 000264B8  7C 80 07 35 */	extsh. r0, r4
/* 8002957C 000264BC  90 7F 00 00 */	stw r3, 0(r31)
/* 80029580 000264C0  40 81 00 0C */	ble lbl_8002958C
/* 80029584 000264C4  7F E3 FB 78 */	mr r3, r31
/* 80029588 000264C8  4B FE 35 29 */	bl __dl__FPv
lbl_8002958C:
/* 8002958C 000264CC  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80029590 000264D0  7F E3 FB 78 */	mr r3, r31
/* 80029594 000264D4  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80029598 000264D8  38 21 00 18 */	addi r1, r1, 0x18
/* 8002959C 000264DC  7C 08 03 A6 */	mtlr r0
/* 800295A0 000264E0  4E 80 00 20 */	blr 

.global appendChild__7J3DNodeFP7J3DNode
appendChild__7J3DNodeFP7J3DNode:
/* 800295A4 000264E4  80 03 00 10 */	lwz r0, 0x10(r3)
/* 800295A8 000264E8  28 00 00 00 */	cmplwi r0, 0
/* 800295AC 000264EC  40 82 00 0C */	bne lbl_800295B8
/* 800295B0 000264F0  90 83 00 10 */	stw r4, 0x10(r3)
/* 800295B4 000264F4  4E 80 00 20 */	blr 
lbl_800295B8:
/* 800295B8 000264F8  7C 03 03 78 */	mr r3, r0
/* 800295BC 000264FC  48 00 00 08 */	b lbl_800295C4
lbl_800295C0:
/* 800295C0 00026500  7C 03 03 78 */	mr r3, r0
lbl_800295C4:
/* 800295C4 00026504  80 03 00 14 */	lwz r0, 0x14(r3)
/* 800295C8 00026508  28 00 00 00 */	cmplwi r0, 0
/* 800295CC 0002650C  40 82 FF F4 */	bne lbl_800295C0
/* 800295D0 00026510  90 83 00 14 */	stw r4, 0x14(r3)
/* 800295D4 00026514  4E 80 00 20 */	blr 

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__7J3DNode
__vt__7J3DNode:
  .4byte 0
  .4byte 0
  .4byte init__7J3DNodeFP12J3DModelData
  .4byte updateIn__7J3DNodeFv
  .4byte updateOut__7J3DNodeFv
  .4byte entryIn__7J3DNodeFv
  .4byte calcIn__7J3DNodeFv
  .4byte calcOut__7J3DNodeFv
  .4byte getType__7J3DNodeCFv
  .4byte __dt__7J3DNodeFv

.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __sinit_JAIConst_cpp
__sinit_JAIConst_cpp:
/* 8004DAA4 0004A9E4  7C 08 02 A6 */	mflr r0
/* 8004DAA8 0004A9E8  3C 60 80 40 */	lis r3, nullActor__8JAIConst@ha
/* 8004DAAC 0004A9EC  90 01 00 04 */	stw r0, 4(r1)
/* 8004DAB0 0004A9F0  38 A3 3D F4 */	addi r5, r3, nullActor__8JAIConst@l
/* 8004DAB4 0004A9F4  38 00 00 00 */	li r0, 0
/* 8004DAB8 0004A9F8  94 21 FF F8 */	stwu r1, -8(r1)
/* 8004DABC 0004A9FC  38 6D 8F B0 */	addi r3, r13, random__8JAIConst@sda21
/* 8004DAC0 0004AA00  38 80 00 00 */	li r4, 0
/* 8004DAC4 0004AA04  90 05 00 00 */	stw r0, 0(r5)
/* 8004DAC8 0004AA08  90 05 00 04 */	stw r0, 4(r5)
/* 8004DACC 0004AA0C  90 05 00 08 */	stw r0, 8(r5)
/* 8004DAD0 0004AA10  90 05 00 0C */	stw r0, 0xc(r5)
/* 8004DAD4 0004AA14  48 01 B0 49 */	bl __ct__Q25JMath13TRandom_fast_FUl
/* 8004DAD8 0004AA18  80 01 00 0C */	lwz r0, 0xc(r1)
/* 8004DADC 0004AA1C  38 21 00 08 */	addi r1, r1, 8
/* 8004DAE0 0004AA20  7C 08 03 A6 */	mtlr r0
/* 8004DAE4 0004AA24  4E 80 00 20 */	blr 

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
  .4byte __sinit_JAIConst_cpp

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global dummyZeroVec__8JAIConst
dummyZeroVec__8JAIConst:
  .4byte 0
  .4byte 0
  .4byte 0
.global nullInfoData2__8JAIConst
nullInfoData2__8JAIConst:
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
.global sCInfos_0__8JAIConst
sCInfos_0__8JAIConst:
	.incbin "baserom.dol", 0x3A7CCC, 0x24

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
.global random__8JAIConst
random__8JAIConst:
	.skip 0x8

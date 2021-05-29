.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global TRKInitializeDispatcher
TRKInitializeDispatcher:
/* 80088A94 000859D4  3C 60 80 3F */	lis r3, gTRKDispatchTableSize@ha
/* 80088A98 000859D8  38 00 00 20 */	li r0, 0x20
/* 80088A9C 000859DC  90 03 AA D8 */	stw r0, gTRKDispatchTableSize@l(r3)
/* 80088AA0 000859E0  38 60 00 00 */	li r3, 0
/* 80088AA4 000859E4  4E 80 00 20 */	blr 

.global TRKDispatchMessage
TRKDispatchMessage:
/* 80088AA8 000859E8  7C 08 02 A6 */	mflr r0
/* 80088AAC 000859EC  38 80 00 00 */	li r4, 0
/* 80088AB0 000859F0  90 01 00 04 */	stw r0, 4(r1)
/* 80088AB4 000859F4  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80088AB8 000859F8  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80088ABC 000859FC  3B E0 05 00 */	li r31, 0x500
/* 80088AC0 00085A00  93 C1 00 10 */	stw r30, 0x10(r1)
/* 80088AC4 00085A04  3B C3 00 00 */	addi r30, r3, 0
/* 80088AC8 00085A08  4B FF F7 A9 */	bl TRKSetBufferPosition
/* 80088ACC 00085A0C  38 7E 00 00 */	addi r3, r30, 0
/* 80088AD0 00085A10  38 81 00 08 */	addi r4, r1, 8
/* 80088AD4 00085A14  4B FF FB 21 */	bl TRKReadBuffer1_ui8
/* 80088AD8 00085A18  3C 60 80 3F */	lis r3, gTRKDispatchTableSize@ha
/* 80088ADC 00085A1C  88 81 00 08 */	lbz r4, 8(r1)
/* 80088AE0 00085A20  80 03 AA D8 */	lwz r0, gTRKDispatchTableSize@l(r3)
/* 80088AE4 00085A24  7C 04 00 40 */	cmplw r4, r0
/* 80088AE8 00085A28  40 80 00 28 */	bge lbl_80088B10
/* 80088AEC 00085A2C  3C 60 80 3B */	lis r3, gTRKDispatchTable@ha
/* 80088AF0 00085A30  54 84 10 3A */	slwi r4, r4, 2
/* 80088AF4 00085A34  38 03 EF 58 */	addi r0, r3, gTRKDispatchTable@l
/* 80088AF8 00085A38  7C 60 22 14 */	add r3, r0, r4
/* 80088AFC 00085A3C  81 83 00 00 */	lwz r12, 0(r3)
/* 80088B00 00085A40  38 7E 00 00 */	addi r3, r30, 0
/* 80088B04 00085A44  7D 88 03 A6 */	mtlr r12
/* 80088B08 00085A48  4E 80 00 21 */	blrl 
/* 80088B0C 00085A4C  7C 7F 1B 78 */	mr r31, r3
lbl_80088B10:
/* 80088B10 00085A50  7F E3 FB 78 */	mr r3, r31
/* 80088B14 00085A54  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80088B18 00085A58  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 80088B1C 00085A5C  38 21 00 18 */	addi r1, r1, 0x18
/* 80088B20 00085A60  80 01 00 04 */	lwz r0, 4(r1)
/* 80088B24 00085A64  7C 08 03 A6 */	mtlr r0
/* 80088B28 00085A68  4E 80 00 20 */	blr 

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global gTRKDispatchTable
gTRKDispatchTable:
  .4byte TRKDoUnsupported
  .4byte TRKDoConnect
  .4byte TRKDoDisconnect
  .4byte TRKDoReset
  .4byte TRKDoVersions
  .4byte TRKDoSupportMask
  .4byte TRKDoCPUType
  .4byte TRKDoUnsupported
  .4byte TRKDoUnsupported
  .4byte TRKDoUnsupported
  .4byte TRKDoUnsupported
  .4byte TRKDoUnsupported
  .4byte TRKDoUnsupported
  .4byte TRKDoUnsupported
  .4byte TRKDoUnsupported
  .4byte TRKDoUnsupported
  .4byte TRKDoReadMemory
  .4byte TRKDoWriteMemory
  .4byte TRKDoReadRegisters
  .4byte TRKDoWriteRegisters
  .4byte TRKDoUnsupported
  .4byte TRKDoUnsupported
  .4byte TRKDoFlushCache
  .4byte TRKDoUnsupported
  .4byte TRKDoContinue
  .4byte TRKDoStep
  .4byte TRKDoStop
  .4byte TRKDoUnsupported
  .4byte TRKDoUnsupported
  .4byte TRKDoUnsupported
  .4byte TRKDoUnsupported
  .4byte TRKDoUnsupported
  .4byte 0
  .4byte 0

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
.global gTRKDispatchTableSize
gTRKDispatchTableSize:
	.skip 0x8

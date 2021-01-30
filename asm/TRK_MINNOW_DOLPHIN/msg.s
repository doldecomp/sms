.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global TRKMessageSend
TRKMessageSend:
/* 80088058 00084F98  7C 08 02 A6 */	mflr r0
/* 8008805C 00084F9C  90 01 00 04 */	stw r0, 4(r1)
/* 80088060 00084FA0  94 21 FF F8 */	stwu r1, -8(r1)
/* 80088064 00084FA4  80 83 00 08 */	lwz r4, 8(r3)
/* 80088068 00084FA8  38 63 00 10 */	addi r3, r3, 0x10
/* 8008806C 00084FAC  48 00 42 39 */	bl TRKWriteUARTN
/* 80088070 00084FB0  38 21 00 08 */	addi r1, r1, 8
/* 80088074 00084FB4  80 01 00 04 */	lwz r0, 4(r1)
/* 80088078 00084FB8  7C 08 03 A6 */	mtlr r0
/* 8008807C 00084FBC  4E 80 00 20 */	blr 

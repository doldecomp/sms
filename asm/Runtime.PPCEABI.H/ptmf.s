.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __ptmf_test
__ptmf_test:
/* 80082AF4 0007FA34  80 A3 00 00 */	lwz r5, 0(r3)
/* 80082AF8 0007FA38  80 C3 00 04 */	lwz r6, 4(r3)
/* 80082AFC 0007FA3C  80 E3 00 08 */	lwz r7, 8(r3)
/* 80082B00 0007FA40  38 60 00 01 */	li r3, 1
/* 80082B04 0007FA44  2C 05 00 00 */	cmpwi r5, 0
/* 80082B08 0007FA48  2F 06 00 00 */	cmpwi cr6, r6, 0
/* 80082B0C 0007FA4C  2F 87 00 00 */	cmpwi cr7, r7, 0
/* 80082B10 0007FA50  4C 82 00 20 */	bnelr 
/* 80082B14 0007FA54  4C 9A 00 20 */	bnelr cr6
/* 80082B18 0007FA58  4C 9E 00 20 */	bnelr cr7
/* 80082B1C 0007FA5C  38 60 00 00 */	li r3, 0
/* 80082B20 0007FA60  4E 80 00 20 */	blr 

.global __ptmf_scall
__ptmf_scall:
/* 80082B24 0007FA64  80 0C 00 00 */	lwz r0, 0(r12)
/* 80082B28 0007FA68  81 6C 00 04 */	lwz r11, 4(r12)
/* 80082B2C 0007FA6C  81 8C 00 08 */	lwz r12, 8(r12)
/* 80082B30 0007FA70  7C 63 02 14 */	add r3, r3, r0
/* 80082B34 0007FA74  2C 0B 00 00 */	cmpwi r11, 0
/* 80082B38 0007FA78  41 80 00 0C */	blt lbl_80082B44
/* 80082B3C 0007FA7C  7D 83 60 2E */	lwzx r12, r3, r12
/* 80082B40 0007FA80  7D 8C 58 2E */	lwzx r12, r12, r11
lbl_80082B44:
/* 80082B44 0007FA84  7D 89 03 A6 */	mtctr r12
/* 80082B48 0007FA88  4E 80 04 20 */	bctr 

.section .rodata, "wa"  # 0x8036FFA0 - 0x803A8380
.global __ptmf_null
__ptmf_null:
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0

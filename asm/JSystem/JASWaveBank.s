.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global getCurrentHeap__Q28JASystem9TWaveBankFv
getCurrentHeap__Q28JASystem9TWaveBankFv:
/* 8005B170 000580B0  80 6D 90 A8 */	lwz r3, sCurrentHeap__Q28JASystem9TWaveBank-_SDA_BASE_(r13)
/* 8005B174 000580B4  28 03 00 00 */	cmplwi r3, 0
/* 8005B178 000580B8  4C 82 00 20 */	bnelr 
/* 8005B17C 000580BC  80 6D 92 00 */	lwz r3, JASDram-_SDA_BASE_(r13)
/* 8005B180 000580C0  4E 80 00 20 */	blr 

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
sCurrentHeap__Q28JASystem9TWaveBank:
	.skip 0x8

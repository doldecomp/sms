.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0

.global getCurrentHeap__Q28JASystem5TBankFv
getCurrentHeap__Q28JASystem5TBankFv:
/* 8005830C 0005524C  80 6D 90 78 */	lwz r3, sCurrentHeap__Q28JASystem5TBank-_SDA_BASE_(r13)
/* 80058310 00055250  28 03 00 00 */	cmplwi r3, 0
/* 80058314 00055254  4C 82 00 20 */	bnelr 
/* 80058318 00055258  80 6D 92 00 */	lwz r3, JASDram-_SDA_BASE_(r13)
/* 8005831C 0005525C  4E 80 00 20 */	blr 

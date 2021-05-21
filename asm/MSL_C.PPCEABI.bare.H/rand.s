.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global srand
srand:
/* 80085ABC 000829FC  90 6D 82 30 */	stw r3, next@sda21(r13)
/* 80085AC0 00082A00  4E 80 00 20 */	blr 

.global rand
rand:
/* 80085AC4 00082A04  3C 60 41 C6 */	lis r3, 0x41C64E6D@ha
/* 80085AC8 00082A08  80 8D 82 30 */	lwz r4, next@sda21(r13)
/* 80085ACC 00082A0C  38 03 4E 6D */	addi r0, r3, 0x41C64E6D@l
/* 80085AD0 00082A10  7C 64 01 D6 */	mullw r3, r4, r0
/* 80085AD4 00082A14  38 03 30 39 */	addi r0, r3, 0x3039
/* 80085AD8 00082A18  90 0D 82 30 */	stw r0, next@sda21(r13)
/* 80085ADC 00082A1C  80 0D 82 30 */	lwz r0, next@sda21(r13)
/* 80085AE0 00082A20  54 03 84 7E */	rlwinm r3, r0, 0x10, 0x11, 0x1f
/* 80085AE4 00082A24  4E 80 00 20 */	blr 

.section .sdata, "wa"  # 0x80408AC0 - 0x804097C0
next:
	.incbin "baserom.dol", 0x3E3230, 0x8

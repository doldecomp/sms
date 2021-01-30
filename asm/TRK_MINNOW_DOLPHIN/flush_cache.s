.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global TRK_flush_cache
TRK_flush_cache:
/* 8008A33C 0008727C  3C A0 FF FF */	lis r5, 0xFFFFFFF1@h
/* 8008A340 00087280  60 A5 FF F1 */	ori r5, r5, 0xFFFFFFF1@l
/* 8008A344 00087284  7C A5 18 38 */	and r5, r5, r3
/* 8008A348 00087288  7C 65 18 50 */	subf r3, r5, r3
/* 8008A34C 0008728C  7C 84 1A 14 */	add r4, r4, r3
lbl_8008A350:
/* 8008A350 00087290  7C 00 28 6C */	dcbst 0, r5
/* 8008A354 00087294  7C 00 28 AC */	dcbf 0, r5
/* 8008A358 00087298  7C 00 04 AC */	sync 0
/* 8008A35C 0008729C  7C 00 2F AC */	icbi 0, r5
/* 8008A360 000872A0  30 A5 00 08 */	addic r5, r5, 8
/* 8008A364 000872A4  34 84 FF F8 */	addic. r4, r4, -8
/* 8008A368 000872A8  40 80 FF E8 */	bge lbl_8008A350
/* 8008A36C 000872AC  4C 00 01 2C */	isync 
/* 8008A370 000872B0  4E 80 00 20 */	blr 

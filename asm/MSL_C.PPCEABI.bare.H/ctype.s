.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global toupper
toupper:
/* 8008385C 0008079C  2C 03 FF FF */	cmpwi r3, -1
/* 80083860 000807A0  40 82 00 0C */	bne lbl_8008386C
/* 80083864 000807A4  38 60 FF FF */	li r3, -1
/* 80083868 000807A8  4E 80 00 20 */	blr 
lbl_8008386C:
/* 8008386C 000807AC  3C 80 80 37 */	lis r4, __upper_map@ha
/* 80083870 000807B0  54 63 06 3E */	clrlwi r3, r3, 0x18
/* 80083874 000807B4  38 04 18 20 */	addi r0, r4, __upper_map@l
/* 80083878 000807B8  7C 60 1A 14 */	add r3, r0, r3
/* 8008387C 000807BC  88 63 00 00 */	lbz r3, 0(r3)
/* 80083880 000807C0  4E 80 00 20 */	blr 

.global tolower
tolower:
/* 80083884 000807C4  2C 03 FF FF */	cmpwi r3, -1
/* 80083888 000807C8  40 82 00 0C */	bne lbl_80083894
/* 8008388C 000807CC  38 60 FF FF */	li r3, -1
/* 80083890 000807D0  4E 80 00 20 */	blr 
lbl_80083894:
/* 80083894 000807D4  3C 80 80 37 */	lis r4, __lower_map@ha
/* 80083898 000807D8  54 63 06 3E */	clrlwi r3, r3, 0x18
/* 8008389C 000807DC  38 04 17 20 */	addi r0, r4, __lower_map@l
/* 800838A0 000807E0  7C 60 1A 14 */	add r3, r0, r3
/* 800838A4 000807E4  88 63 00 00 */	lbz r3, 0(r3)
/* 800838A8 000807E8  4E 80 00 20 */	blr 

.section .rodata, "a"  # 0x8036FFA0 - 0x803A8380
.balign 8
.global __ctype_map
__ctype_map:
	.incbin "baserom.dol", 0x36E620, 0x100
.global __lower_map
__lower_map:
	.incbin "baserom.dol", 0x36E720, 0x100
__upper_map:
	.incbin "baserom.dol", 0x36E820, 0x100

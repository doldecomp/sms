.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0

.global OSGetArenaHi
OSGetArenaHi:
/* 8008DCB4 0008ABF4  80 6D 93 08 */	lwz r3, __OSArenaHi-_SDA_BASE_(r13)
/* 8008DCB8 0008ABF8  4E 80 00 20 */	blr 

.global OSGetArenaLo
OSGetArenaLo:
/* 8008DCBC 0008ABFC  80 6D 82 48 */	lwz r3, __OSArenaLo-_SDA_BASE_(r13)
/* 8008DCC0 0008AC00  4E 80 00 20 */	blr 

.global OSSetArenaHi
OSSetArenaHi:
/* 8008DCC4 0008AC04  90 6D 93 08 */	stw r3, __OSArenaHi-_SDA_BASE_(r13)
/* 8008DCC8 0008AC08  4E 80 00 20 */	blr 

.global OSSetArenaLo
OSSetArenaLo:
/* 8008DCCC 0008AC0C  90 6D 82 48 */	stw r3, __OSArenaLo-_SDA_BASE_(r13)
/* 8008DCD0 0008AC10  4E 80 00 20 */	blr 

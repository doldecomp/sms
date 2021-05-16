.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global GetR2__Fv
GetR2__Fv:
/* 8008321C 0008015C  7C 43 13 78 */	mr r3, r2
/* 80083220 00080160  4E 80 00 20 */	blr 

.global __fini_cpp_exceptions
__fini_cpp_exceptions:
/* 80083224 00080164  7C 08 02 A6 */	mflr r0
/* 80083228 00080168  90 01 00 04 */	stw r0, 4(r1)
/* 8008322C 0008016C  94 21 FF F8 */	stwu r1, -8(r1)
/* 80083230 00080170  80 6D 82 20 */	lwz r3, fragmentID-_SDA_BASE_(r13)
/* 80083234 00080174  2C 03 FF FE */	cmpwi r3, -2
/* 80083238 00080178  41 82 00 10 */	beq lbl_80083248
/* 8008323C 0008017C  4B FF F9 11 */	bl __unregister_fragment
/* 80083240 00080180  38 00 FF FE */	li r0, -2
/* 80083244 00080184  90 0D 82 20 */	stw r0, fragmentID-_SDA_BASE_(r13)
lbl_80083248:
/* 80083248 00080188  80 01 00 0C */	lwz r0, 0xc(r1)
/* 8008324C 0008018C  38 21 00 08 */	addi r1, r1, 8
/* 80083250 00080190  7C 08 03 A6 */	mtlr r0
/* 80083254 00080194  4E 80 00 20 */	blr 

.global __init_cpp_exceptions
__init_cpp_exceptions:
/* 80083258 00080198  7C 08 02 A6 */	mflr r0
/* 8008325C 0008019C  90 01 00 04 */	stw r0, 4(r1)
/* 80083260 000801A0  94 21 FF F8 */	stwu r1, -8(r1)
/* 80083264 000801A4  80 0D 82 20 */	lwz r0, fragmentID-_SDA_BASE_(r13)
/* 80083268 000801A8  2C 00 FF FE */	cmpwi r0, -2
/* 8008326C 000801AC  40 82 00 1C */	bne lbl_80083288
/* 80083270 000801B0  4B FF FF AD */	bl GetR2__Fv
/* 80083274 000801B4  3C A0 80 00 */	lis r5, lbl_800055DC@ha
/* 80083278 000801B8  7C 64 1B 78 */	mr r4, r3
/* 8008327C 000801BC  38 65 55 DC */	addi r3, r5, lbl_800055DC@l
/* 80083280 000801C0  4B FF F9 01 */	bl __register_fragment
/* 80083284 000801C4  90 6D 82 20 */	stw r3, fragmentID-_SDA_BASE_(r13)
lbl_80083288:
/* 80083288 000801C8  80 01 00 0C */	lwz r0, 0xc(r1)
/* 8008328C 000801CC  38 21 00 08 */	addi r1, r1, 8
/* 80083290 000801D0  7C 08 03 A6 */	mtlr r0
/* 80083294 000801D4  4E 80 00 20 */	blr 

.section .ctors$10, "wa"  # 0x8036FBA0 - 0x8036FF80
.global __init_cpp_exceptions_reference
__init_cpp_exceptions_reference:
	.incbin "baserom.dol", 0x36CBA0, 0x4

.section .dtors$10, "wa"  # 0x8036FF80 - 0x8036FFA0
.global __destroy_global_chain_reference
__destroy_global_chain_reference:
	.incbin "baserom.dol", 0x36CF80, 0x4

.section .dtors$15, "wa"  # 0x8036FF80 - 0x8036FFA0
.global __fini_cpp_exceptions_reference
__fini_cpp_exceptions_reference:
	.incbin "baserom.dol", 0x36CF84, 0x4

.section .dtors$99, "wa"

.section .sdata, "wa"  # 0x80408AC0 - 0x804097C0
fragmentID:
	.incbin "baserom.dol", 0x3E3220, 0x8

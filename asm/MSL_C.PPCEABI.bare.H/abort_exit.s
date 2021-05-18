.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global exit
exit:
/* 80083298 000801D8  7C 08 02 A6 */	mflr r0
/* 8008329C 000801DC  90 01 00 04 */	stw r0, 4(r1)
/* 800832A0 000801E0  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 800832A4 000801E4  93 E1 00 14 */	stw r31, 0x14(r1)
/* 800832A8 000801E8  80 0D 92 90 */	lwz r0, __aborting-_SDA_BASE_(r13)
/* 800832AC 000801EC  2C 00 00 00 */	cmpwi r0, 0
/* 800832B0 000801F0  40 82 00 84 */	bne lbl_80083334
/* 800832B4 000801F4  3C 60 80 3F */	lis r3, atexit_funcs@ha
/* 800832B8 000801F8  3B E3 8E E0 */	addi r31, r3, atexit_funcs@l
/* 800832BC 000801FC  48 00 00 24 */	b lbl_800832E0
lbl_800832C0:
/* 800832C0 00080200  80 6D 92 94 */	lwz r3, atexit_curr_func-_SDA_BASE_(r13)
/* 800832C4 00080204  38 63 FF FF */	addi r3, r3, -1
/* 800832C8 00080208  54 60 10 3A */	slwi r0, r3, 2
/* 800832CC 0008020C  90 6D 92 94 */	stw r3, atexit_curr_func-_SDA_BASE_(r13)
/* 800832D0 00080210  7C 7F 02 14 */	add r3, r31, r0
/* 800832D4 00080214  81 83 00 00 */	lwz r12, 0(r3)
/* 800832D8 00080218  7D 88 03 A6 */	mtlr r12
/* 800832DC 0008021C  4E 80 00 21 */	blrl 
lbl_800832E0:
/* 800832E0 00080220  80 0D 92 94 */	lwz r0, atexit_curr_func-_SDA_BASE_(r13)
/* 800832E4 00080224  2C 00 00 00 */	cmpwi r0, 0
/* 800832E8 00080228  41 81 FF D8 */	bgt lbl_800832C0
/* 800832EC 0008022C  4B FF F3 F5 */	bl __destroy_global_chain
/* 800832F0 00080230  3C 60 80 37 */	lis r3, _dtors@ha
/* 800832F4 00080234  38 03 FF 80 */	addi r0, r3, _dtors@l
/* 800832F8 00080238  7C 1F 03 78 */	mr r31, r0
/* 800832FC 0008023C  48 00 00 10 */	b lbl_8008330C
lbl_80083300:
/* 80083300 00080240  7D 88 03 A6 */	mtlr r12
/* 80083304 00080244  4E 80 00 21 */	blrl 
/* 80083308 00080248  3B FF 00 04 */	addi r31, r31, 4
lbl_8008330C:
/* 8008330C 0008024C  81 9F 00 00 */	lwz r12, 0(r31)
/* 80083310 00080250  28 0C 00 00 */	cmplwi r12, 0
/* 80083314 00080254  40 82 FF EC */	bne lbl_80083300
/* 80083318 00080258  81 8D 92 9C */	lwz r12, __stdio_exit-_SDA_BASE_(r13)
/* 8008331C 0008025C  28 0C 00 00 */	cmplwi r12, 0
/* 80083320 00080260  41 82 00 14 */	beq lbl_80083334
/* 80083324 00080264  7D 88 03 A6 */	mtlr r12
/* 80083328 00080268  4E 80 00 21 */	blrl 
/* 8008332C 0008026C  38 00 00 00 */	li r0, 0
/* 80083330 00080270  90 0D 92 9C */	stw r0, __stdio_exit-_SDA_BASE_(r13)
lbl_80083334:
/* 80083334 00080274  3C 60 80 3F */	lis r3, __atexit_funcs@ha
/* 80083338 00080278  3B E3 8F E0 */	addi r31, r3, __atexit_funcs@l
/* 8008333C 0008027C  48 00 00 24 */	b lbl_80083360
lbl_80083340:
/* 80083340 00080280  80 6D 92 98 */	lwz r3, __atexit_curr_func-_SDA_BASE_(r13)
/* 80083344 00080284  38 63 FF FF */	addi r3, r3, -1
/* 80083348 00080288  54 60 10 3A */	slwi r0, r3, 2
/* 8008334C 0008028C  90 6D 92 98 */	stw r3, __atexit_curr_func-_SDA_BASE_(r13)
/* 80083350 00080290  7C 7F 02 14 */	add r3, r31, r0
/* 80083354 00080294  81 83 00 00 */	lwz r12, 0(r3)
/* 80083358 00080298  7D 88 03 A6 */	mtlr r12
/* 8008335C 0008029C  4E 80 00 21 */	blrl 
lbl_80083360:
/* 80083360 000802A0  80 0D 92 98 */	lwz r0, __atexit_curr_func-_SDA_BASE_(r13)
/* 80083364 000802A4  2C 00 00 00 */	cmpwi r0, 0
/* 80083368 000802A8  41 81 FF D8 */	bgt lbl_80083340
/* 8008336C 000802AC  48 00 04 ED */	bl __kill_critical_regions
/* 80083370 000802B0  81 8D 92 A0 */	lwz r12, __console_exit-_SDA_BASE_(r13)
/* 80083374 000802B4  28 0C 00 00 */	cmplwi r12, 0
/* 80083378 000802B8  41 82 00 14 */	beq lbl_8008338C
/* 8008337C 000802BC  7D 88 03 A6 */	mtlr r12
/* 80083380 000802C0  4E 80 00 21 */	blrl 
/* 80083384 000802C4  38 00 00 00 */	li r0, 0
/* 80083388 000802C8  90 0D 92 A0 */	stw r0, __console_exit-_SDA_BASE_(r13)
lbl_8008338C:
/* 8008338C 000802CC  48 01 0F 05 */	bl _ExitProcess
/* 80083390 000802D0  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80083394 000802D4  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80083398 000802D8  38 21 00 18 */	addi r1, r1, 0x18
/* 8008339C 000802DC  7C 08 03 A6 */	mtlr r0
/* 800833A0 000802E0  4E 80 00 20 */	blr 

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
atexit_funcs:
	.skip 0x100
__atexit_funcs:
	.skip 0x100

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
__aborting:
	.skip 0x4
atexit_curr_func:
	.skip 0x4
__atexit_curr_func:
	.skip 0x4
__stdio_exit:
	.skip 0x4
__console_exit:
	.skip 0x8

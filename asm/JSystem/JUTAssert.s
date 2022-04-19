.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global flushMessage__12JUTAssertionFv
flushMessage__12JUTAssertionFv:
/* 8003AD74 00037CB4  7C 08 02 A6 */	mflr r0
/* 8003AD78 00037CB8  90 01 00 04 */	stw r0, 4(r1)
/* 8003AD7C 00037CBC  94 21 FF F8 */	stwu r1, -8(r1)
/* 8003AD80 00037CC0  80 6D 8E B0 */	lwz r3, "sMessageLife__Q212JUTAssertion9@unnamed@"@sda21(r13)
/* 8003AD84 00037CC4  28 03 00 00 */	cmplwi r3, 0
/* 8003AD88 00037CC8  40 82 00 0C */	bne lbl_8003AD94
/* 8003AD8C 00037CCC  38 00 00 00 */	li r0, 0
/* 8003AD90 00037CD0  48 00 00 28 */	b lbl_8003ADB8
lbl_8003AD94:
/* 8003AD94 00037CD4  3C 03 00 01 */	addis r0, r3, 1
/* 8003AD98 00037CD8  28 00 FF FF */	cmplwi r0, 0xffff
/* 8003AD9C 00037CDC  41 82 00 0C */	beq lbl_8003ADA8
/* 8003ADA0 00037CE0  38 03 FF FF */	addi r0, r3, -1
/* 8003ADA4 00037CE4  90 0D 8E B0 */	stw r0, "sMessageLife__Q212JUTAssertion9@unnamed@"@sda21(r13)
lbl_8003ADA8:
/* 8003ADA8 00037CE8  80 0D 8E B0 */	lwz r0, "sMessageLife__Q212JUTAssertion9@unnamed@"@sda21(r13)
/* 8003ADAC 00037CEC  28 00 00 05 */	cmplwi r0, 5
/* 8003ADB0 00037CF0  40 80 00 08 */	bge lbl_8003ADB8
/* 8003ADB4 00037CF4  38 00 00 00 */	li r0, 0
lbl_8003ADB8:
/* 8003ADB8 00037CF8  28 00 00 00 */	cmplwi r0, 0
/* 8003ADBC 00037CFC  41 82 00 40 */	beq lbl_8003ADFC
/* 8003ADC0 00037D00  88 0D 80 B8 */	lbz r0, "mVisible__Q212JUTAssertion9@unnamed@"@sda21(r13)
/* 8003ADC4 00037D04  28 00 00 01 */	cmplwi r0, 1
/* 8003ADC8 00037D08  40 82 00 34 */	bne lbl_8003ADFC
/* 8003ADCC 00037D0C  3C 80 80 3E */	lis r4, "sMessageFileLine__Q212JUTAssertion9@unnamed@"@ha
/* 8003ADD0 00037D10  80 6D 8D B0 */	lwz r3, sDirectPrint__14JUTDirectPrint@sda21(r13)
/* 8003ADD4 00037D14  38 C4 61 00 */	addi r6, r4, "sMessageFileLine__Q212JUTAssertion9@unnamed@"@l
/* 8003ADD8 00037D18  38 80 00 10 */	li r4, 0x10
/* 8003ADDC 00037D1C  38 A0 00 10 */	li r5, 0x10
/* 8003ADE0 00037D20  4B FD 67 CD */	bl drawString__14JUTDirectPrintFUsUsPc
/* 8003ADE4 00037D24  3C 80 80 3E */	lis r4, "sMessageString__Q212JUTAssertion9@unnamed@"@ha
/* 8003ADE8 00037D28  80 6D 8D B0 */	lwz r3, sDirectPrint__14JUTDirectPrint@sda21(r13)
/* 8003ADEC 00037D2C  38 C4 61 40 */	addi r6, r4, "sMessageString__Q212JUTAssertion9@unnamed@"@l
/* 8003ADF0 00037D30  38 80 00 10 */	li r4, 0x10
/* 8003ADF4 00037D34  38 A0 00 18 */	li r5, 0x18
/* 8003ADF8 00037D38  4B FD 67 B5 */	bl drawString__14JUTDirectPrintFUsUsPc
lbl_8003ADFC:
/* 8003ADFC 00037D3C  80 01 00 0C */	lwz r0, 0xc(r1)
/* 8003AE00 00037D40  38 21 00 08 */	addi r1, r1, 8
/* 8003AE04 00037D44  7C 08 03 A6 */	mtlr r0
/* 8003AE08 00037D48  4E 80 00 20 */	blr 

.section .sdata, "wa"  # 0x80408AC0 - 0x804097C0
"mVisible__Q212JUTAssertion9@unnamed@":
	.incbin "baserom.dol", 0x3E30B8, 0x8

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
"sMessageFileLine__Q212JUTAssertion9@unnamed@":
	.skip 0x40
"sMessageString__Q212JUTAssertion9@unnamed@":
	.skip 0x60

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
"sMessageLife__Q212JUTAssertion9@unnamed@":
	.skip 0x8

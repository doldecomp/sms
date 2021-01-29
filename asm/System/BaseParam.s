.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0

.global __ct__10TBaseParamFP7TParamsUsPCc
__ct__10TBaseParamFP7TParamsUsPCc:
/* 800DABF4 000D7B34  3C E0 80 3B */	lis r7, __vt__10TBaseParam@ha
/* 800DABF8 000D7B38  38 07 38 28 */	addi r0, r7, __vt__10TBaseParam@l
/* 800DABFC 000D7B3C  90 03 00 00 */	stw r0, 0(r3)
/* 800DAC00 000D7B40  38 00 00 00 */	li r0, 0
/* 800DAC04 000D7B44  B0 A3 00 04 */	sth r5, 4(r3)
/* 800DAC08 000D7B48  90 C3 00 08 */	stw r6, 8(r3)
/* 800DAC0C 000D7B4C  90 03 00 0C */	stw r0, 0xc(r3)
/* 800DAC10 000D7B50  80 04 00 04 */	lwz r0, 4(r4)
/* 800DAC14 000D7B54  28 00 00 00 */	cmplwi r0, 0
/* 800DAC18 000D7B58  41 82 00 24 */	beq lbl_800DAC3C
/* 800DAC1C 000D7B5C  7C 04 03 78 */	mr r4, r0
/* 800DAC20 000D7B60  48 00 00 08 */	b lbl_800DAC28
lbl_800DAC24:
/* 800DAC24 000D7B64  7C 04 03 78 */	mr r4, r0
lbl_800DAC28:
/* 800DAC28 000D7B68  80 04 00 0C */	lwz r0, 0xc(r4)
/* 800DAC2C 000D7B6C  28 00 00 00 */	cmplwi r0, 0
/* 800DAC30 000D7B70  40 82 FF F4 */	bne lbl_800DAC24
/* 800DAC34 000D7B74  90 64 00 0C */	stw r3, 0xc(r4)
/* 800DAC38 000D7B78  4E 80 00 20 */	blr 
lbl_800DAC3C:
/* 800DAC3C 000D7B7C  90 64 00 04 */	stw r3, 4(r4)
/* 800DAC40 000D7B80  4E 80 00 20 */	blr 

.global load__10TBaseParamFR20JSUMemoryInputStream
load__10TBaseParamFR20JSUMemoryInputStream:
/* 800DAC44 000D7B84  4E 80 00 20 */	blr 

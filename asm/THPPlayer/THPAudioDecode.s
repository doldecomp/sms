.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global PopDecodedAudioBuffer
PopDecodedAudioBuffer:
/* 800BD9D4 000BA914  7C 08 02 A6 */	mflr r0
/* 800BD9D8 000BA918  3C 80 80 3F */	lis r4, DecodedAudioBufferQueue@ha
/* 800BD9DC 000BA91C  90 01 00 04 */	stw r0, 4(r1)
/* 800BD9E0 000BA920  38 A3 00 00 */	addi r5, r3, 0
/* 800BD9E4 000BA924  38 64 E0 C0 */	addi r3, r4, DecodedAudioBufferQueue@l
/* 800BD9E8 000BA928  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 800BD9EC 000BA92C  38 81 00 0C */	addi r4, r1, 0xc
/* 800BD9F0 000BA930  4B FD 31 89 */	bl OSReceiveMessage
/* 800BD9F4 000BA934  2C 03 00 01 */	cmpwi r3, 1
/* 800BD9F8 000BA938  40 82 00 0C */	bne lbl_800BDA04
/* 800BD9FC 000BA93C  80 61 00 0C */	lwz r3, 0xc(r1)
/* 800BDA00 000BA940  48 00 00 08 */	b lbl_800BDA08
lbl_800BDA04:
/* 800BDA04 000BA944  38 60 00 00 */	li r3, 0
lbl_800BDA08:
/* 800BDA08 000BA948  80 01 00 14 */	lwz r0, 0x14(r1)
/* 800BDA0C 000BA94C  38 21 00 10 */	addi r1, r1, 0x10
/* 800BDA10 000BA950  7C 08 03 A6 */	mtlr r0
/* 800BDA14 000BA954  4E 80 00 20 */	blr 

.global PushFreeAudioBuffer
PushFreeAudioBuffer:
/* 800BDA18 000BA958  7C 08 02 A6 */	mflr r0
/* 800BDA1C 000BA95C  3C A0 80 3F */	lis r5, FreeAudioBufferQueue@ha
/* 800BDA20 000BA960  90 01 00 04 */	stw r0, 4(r1)
/* 800BDA24 000BA964  38 83 00 00 */	addi r4, r3, 0
/* 800BDA28 000BA968  38 65 E0 A0 */	addi r3, r5, FreeAudioBufferQueue@l
/* 800BDA2C 000BA96C  94 21 FF F8 */	stwu r1, -8(r1)
/* 800BDA30 000BA970  38 A0 00 00 */	li r5, 0
/* 800BDA34 000BA974  4B FD 30 7D */	bl OSSendMessage
/* 800BDA38 000BA978  80 01 00 0C */	lwz r0, 0xc(r1)
/* 800BDA3C 000BA97C  38 21 00 08 */	addi r1, r1, 8
/* 800BDA40 000BA980  7C 08 03 A6 */	mtlr r0
/* 800BDA44 000BA984  4E 80 00 20 */	blr 

.global AudioDecode__FP13THPReadBuffer
AudioDecode__FP13THPReadBuffer:
/* 800BDA48 000BA988  7C 08 02 A6 */	mflr r0
/* 800BDA4C 000BA98C  3C 80 80 3F */	lis r4, ActivePlayer@ha
/* 800BDA50 000BA990  90 01 00 04 */	stw r0, 4(r1)
/* 800BDA54 000BA994  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 800BDA58 000BA998  93 E1 00 24 */	stw r31, 0x24(r1)
/* 800BDA5C 000BA99C  3B E4 E1 00 */	addi r31, r4, ActivePlayer@l
/* 800BDA60 000BA9A0  93 C1 00 20 */	stw r30, 0x20(r1)
/* 800BDA64 000BA9A4  3B DF 00 6C */	addi r30, r31, 0x6c
/* 800BDA68 000BA9A8  93 A1 00 1C */	stw r29, 0x1c(r1)
/* 800BDA6C 000BA9AC  93 81 00 18 */	stw r28, 0x18(r1)
/* 800BDA70 000BA9B0  80 1F 00 6C */	lwz r0, 0x6c(r31)
/* 800BDA74 000BA9B4  80 A3 00 00 */	lwz r5, 0(r3)
/* 800BDA78 000BA9B8  3C 60 80 3F */	lis r3, FreeAudioBufferQueue@ha
/* 800BDA7C 000BA9BC  54 04 10 3A */	slwi r4, r0, 2
/* 800BDA80 000BA9C0  3B 84 00 08 */	addi r28, r4, 8
/* 800BDA84 000BA9C4  38 63 E0 A0 */	addi r3, r3, FreeAudioBufferQueue@l
/* 800BDA88 000BA9C8  3B A5 00 08 */	addi r29, r5, 8
/* 800BDA8C 000BA9CC  7F 85 E2 14 */	add r28, r5, r28
/* 800BDA90 000BA9D0  38 81 00 14 */	addi r4, r1, 0x14
/* 800BDA94 000BA9D4  38 A0 00 01 */	li r5, 1
/* 800BDA98 000BA9D8  4B FD 30 E1 */	bl OSReceiveMessage
/* 800BDA9C 000BA9DC  80 1E 00 00 */	lwz r0, 0(r30)
/* 800BDAA0 000BA9E0  38 80 00 00 */	li r4, 0
/* 800BDAA4 000BA9E4  83 C1 00 14 */	lwz r30, 0x14(r1)
/* 800BDAA8 000BA9E8  28 00 00 00 */	cmplwi r0, 0
/* 800BDAAC 000BA9EC  7C 09 03 A6 */	mtctr r0
/* 800BDAB0 000BA9F0  40 81 00 74 */	ble lbl_800BDB24
lbl_800BDAB4:
/* 800BDAB4 000BA9F4  7C 7F 22 14 */	add r3, r31, r4
/* 800BDAB8 000BA9F8  88 03 00 70 */	lbz r0, 0x70(r3)
/* 800BDABC 000BA9FC  2C 00 00 01 */	cmpwi r0, 1
/* 800BDAC0 000BAA00  41 82 00 08 */	beq lbl_800BDAC8
/* 800BDAC4 000BAA04  48 00 00 4C */	b lbl_800BDB10
lbl_800BDAC8:
/* 800BDAC8 000BAA08  3C 60 80 3F */	lis r3, ActivePlayer@ha
/* 800BDACC 000BAA0C  80 DD 00 00 */	lwz r6, 0(r29)
/* 800BDAD0 000BAA10  38 83 E1 00 */	addi r4, r3, ActivePlayer@l
/* 800BDAD4 000BAA14  80 7E 00 00 */	lwz r3, 0(r30)
/* 800BDAD8 000BAA18  80 04 00 EC */	lwz r0, 0xec(r4)
/* 800BDADC 000BAA1C  38 A0 00 00 */	li r5, 0
/* 800BDAE0 000BAA20  7C 06 01 D6 */	mullw r0, r6, r0
/* 800BDAE4 000BAA24  7C 9C 02 14 */	add r4, r28, r0
/* 800BDAE8 000BAA28  4B FF F9 BD */	bl THPAudioDecode
/* 800BDAEC 000BAA2C  90 7E 00 08 */	stw r3, 8(r30)
/* 800BDAF0 000BAA30  3C 60 80 3F */	lis r3, DecodedAudioBufferQueue@ha
/* 800BDAF4 000BAA34  38 63 E0 C0 */	addi r3, r3, DecodedAudioBufferQueue@l
/* 800BDAF8 000BAA38  80 1E 00 00 */	lwz r0, 0(r30)
/* 800BDAFC 000BAA3C  7F C4 F3 78 */	mr r4, r30
/* 800BDB00 000BAA40  38 A0 00 01 */	li r5, 1
/* 800BDB04 000BAA44  90 1E 00 04 */	stw r0, 4(r30)
/* 800BDB08 000BAA48  4B FD 2F A9 */	bl OSSendMessage
/* 800BDB0C 000BAA4C  48 00 00 18 */	b lbl_800BDB24
lbl_800BDB10:
/* 800BDB10 000BAA50  80 1D 00 00 */	lwz r0, 0(r29)
/* 800BDB14 000BAA54  3B BD 00 04 */	addi r29, r29, 4
/* 800BDB18 000BAA58  38 84 00 01 */	addi r4, r4, 1
/* 800BDB1C 000BAA5C  7F 9C 02 14 */	add r28, r28, r0
/* 800BDB20 000BAA60  42 00 FF 94 */	bdnz lbl_800BDAB4
lbl_800BDB24:
/* 800BDB24 000BAA64  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 800BDB28 000BAA68  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 800BDB2C 000BAA6C  83 C1 00 20 */	lwz r30, 0x20(r1)
/* 800BDB30 000BAA70  7C 08 03 A6 */	mtlr r0
/* 800BDB34 000BAA74  83 A1 00 1C */	lwz r29, 0x1c(r1)
/* 800BDB38 000BAA78  83 81 00 18 */	lwz r28, 0x18(r1)
/* 800BDB3C 000BAA7C  38 21 00 28 */	addi r1, r1, 0x28
/* 800BDB40 000BAA80  4E 80 00 20 */	blr 

.global AudioDecoderForOnMemory__FPv
AudioDecoderForOnMemory__FPv:
/* 800BDB44 000BAA84  7C 08 02 A6 */	mflr r0
/* 800BDB48 000BAA88  3C 80 80 3F */	lis r4, ActivePlayer@ha
/* 800BDB4C 000BAA8C  90 01 00 04 */	stw r0, 4(r1)
/* 800BDB50 000BAA90  94 21 FF D0 */	stwu r1, -0x30(r1)
/* 800BDB54 000BAA94  93 E1 00 2C */	stw r31, 0x2c(r1)
/* 800BDB58 000BAA98  3B E0 00 00 */	li r31, 0
/* 800BDB5C 000BAA9C  93 C1 00 28 */	stw r30, 0x28(r1)
/* 800BDB60 000BAAA0  93 A1 00 24 */	stw r29, 0x24(r1)
/* 800BDB64 000BAAA4  3B A4 E1 00 */	addi r29, r4, ActivePlayer@l
/* 800BDB68 000BAAA8  3C 80 80 3F */	lis r4, AudioDecodeThread@ha
/* 800BDB6C 000BAAAC  93 81 00 20 */	stw r28, 0x20(r1)
/* 800BDB70 000BAAB0  3B C4 CD 90 */	addi r30, r4, AudioDecodeThread@l
/* 800BDB74 000BAAB4  83 9D 00 BC */	lwz r28, 0xbc(r29)
/* 800BDB78 000BAAB8  90 61 00 10 */	stw r3, 0x10(r1)
lbl_800BDB7C:
/* 800BDB7C 000BAABC  93 E1 00 14 */	stw r31, 0x14(r1)
/* 800BDB80 000BAAC0  38 61 00 10 */	addi r3, r1, 0x10
/* 800BDB84 000BAAC4  4B FF FE C5 */	bl AudioDecode__FP13THPReadBuffer
/* 800BDB88 000BAAC8  80 1D 00 C0 */	lwz r0, 0xc0(r29)
/* 800BDB8C 000BAACC  80 9D 00 50 */	lwz r4, 0x50(r29)
/* 800BDB90 000BAAD0  7C 7F 02 14 */	add r3, r31, r0
/* 800BDB94 000BAAD4  7C 03 23 96 */	divwu r0, r3, r4
/* 800BDB98 000BAAD8  7C 00 21 D6 */	mullw r0, r0, r4
/* 800BDB9C 000BAADC  7C 60 18 50 */	subf r3, r0, r3
/* 800BDBA0 000BAAE0  38 04 FF FF */	addi r0, r4, -1
/* 800BDBA4 000BAAE4  7C 03 00 40 */	cmplw r3, r0
/* 800BDBA8 000BAAE8  40 82 00 30 */	bne lbl_800BDBD8
/* 800BDBAC 000BAAEC  88 1D 00 A6 */	lbz r0, 0xa6(r29)
/* 800BDBB0 000BAAF0  54 00 07 FF */	clrlwi. r0, r0, 0x1f
/* 800BDBB4 000BAAF4  41 82 00 18 */	beq lbl_800BDBCC
/* 800BDBB8 000BAAF8  80 61 00 10 */	lwz r3, 0x10(r1)
/* 800BDBBC 000BAAFC  80 1D 00 B4 */	lwz r0, 0xb4(r29)
/* 800BDBC0 000BAB00  83 83 00 00 */	lwz r28, 0(r3)
/* 800BDBC4 000BAB04  90 01 00 10 */	stw r0, 0x10(r1)
/* 800BDBC8 000BAB08  48 00 00 24 */	b lbl_800BDBEC
lbl_800BDBCC:
/* 800BDBCC 000BAB0C  7F C3 F3 78 */	mr r3, r30
/* 800BDBD0 000BAB10  4B FD 5E C1 */	bl OSSuspendThread
/* 800BDBD4 000BAB14  48 00 00 18 */	b lbl_800BDBEC
lbl_800BDBD8:
/* 800BDBD8 000BAB18  80 61 00 10 */	lwz r3, 0x10(r1)
/* 800BDBDC 000BAB1C  80 83 00 00 */	lwz r4, 0(r3)
/* 800BDBE0 000BAB20  7C 03 E2 14 */	add r0, r3, r28
/* 800BDBE4 000BAB24  90 01 00 10 */	stw r0, 0x10(r1)
/* 800BDBE8 000BAB28  7C 9C 23 78 */	mr r28, r4
lbl_800BDBEC:
/* 800BDBEC 000BAB2C  3B FF 00 01 */	addi r31, r31, 1
/* 800BDBF0 000BAB30  4B FF FF 8C */	b lbl_800BDB7C

.global AudioDecoder__FPv
AudioDecoder__FPv:
/* 800BDBF4 000BAB34  7C 08 02 A6 */	mflr r0
/* 800BDBF8 000BAB38  90 01 00 04 */	stw r0, 4(r1)
/* 800BDBFC 000BAB3C  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 800BDC00 000BAB40  93 E1 00 14 */	stw r31, 0x14(r1)
lbl_800BDC04:
/* 800BDC04 000BAB44  48 00 1E F1 */	bl PopReadedBuffer
/* 800BDC08 000BAB48  7C 7F 1B 78 */	mr r31, r3
/* 800BDC0C 000BAB4C  4B FF FE 3D */	bl AudioDecode__FP13THPReadBuffer
/* 800BDC10 000BAB50  7F E3 FB 78 */	mr r3, r31
/* 800BDC14 000BAB54  48 00 1E 4D */	bl PushReadedBuffer2
/* 800BDC18 000BAB58  4B FF FF EC */	b lbl_800BDC04

.global AudioDecodeThreadCancel
AudioDecodeThreadCancel:
/* 800BDC1C 000BAB5C  7C 08 02 A6 */	mflr r0
/* 800BDC20 000BAB60  90 01 00 04 */	stw r0, 4(r1)
/* 800BDC24 000BAB64  94 21 FF F8 */	stwu r1, -8(r1)
/* 800BDC28 000BAB68  80 0D 97 18 */	lwz r0, AudioDecodeThreadCreated-_SDA_BASE_(r13)
/* 800BDC2C 000BAB6C  2C 00 00 00 */	cmpwi r0, 0
/* 800BDC30 000BAB70  41 82 00 18 */	beq lbl_800BDC48
/* 800BDC34 000BAB74  3C 60 80 3F */	lis r3, AudioDecodeThread@ha
/* 800BDC38 000BAB78  38 63 CD 90 */	addi r3, r3, AudioDecodeThread@l
/* 800BDC3C 000BAB7C  4B FD 58 31 */	bl OSCancelThread
/* 800BDC40 000BAB80  38 00 00 00 */	li r0, 0
/* 800BDC44 000BAB84  90 0D 97 18 */	stw r0, AudioDecodeThreadCreated-_SDA_BASE_(r13)
lbl_800BDC48:
/* 800BDC48 000BAB88  80 01 00 0C */	lwz r0, 0xc(r1)
/* 800BDC4C 000BAB8C  38 21 00 08 */	addi r1, r1, 8
/* 800BDC50 000BAB90  7C 08 03 A6 */	mtlr r0
/* 800BDC54 000BAB94  4E 80 00 20 */	blr 

.global AudioDecodeThreadStart
AudioDecodeThreadStart:
/* 800BDC58 000BAB98  7C 08 02 A6 */	mflr r0
/* 800BDC5C 000BAB9C  90 01 00 04 */	stw r0, 4(r1)
/* 800BDC60 000BABA0  94 21 FF F8 */	stwu r1, -8(r1)
/* 800BDC64 000BABA4  80 0D 97 18 */	lwz r0, AudioDecodeThreadCreated-_SDA_BASE_(r13)
/* 800BDC68 000BABA8  2C 00 00 00 */	cmpwi r0, 0
/* 800BDC6C 000BABAC  41 82 00 10 */	beq lbl_800BDC7C
/* 800BDC70 000BABB0  3C 60 80 3F */	lis r3, AudioDecodeThread@ha
/* 800BDC74 000BABB4  38 63 CD 90 */	addi r3, r3, AudioDecodeThread@l
/* 800BDC78 000BABB8  4B FD 5B 91 */	bl OSResumeThread
lbl_800BDC7C:
/* 800BDC7C 000BABBC  80 01 00 0C */	lwz r0, 0xc(r1)
/* 800BDC80 000BABC0  38 21 00 08 */	addi r1, r1, 8
/* 800BDC84 000BABC4  7C 08 03 A6 */	mtlr r0
/* 800BDC88 000BABC8  4E 80 00 20 */	blr 

.global CreateAudioDecodeThread
CreateAudioDecodeThread:
/* 800BDC8C 000BABCC  7C 08 02 A6 */	mflr r0
/* 800BDC90 000BABD0  7C 85 23 79 */	or. r5, r4, r4
/* 800BDC94 000BABD4  90 01 00 04 */	stw r0, 4(r1)
/* 800BDC98 000BABD8  3C 80 80 3F */	lis r4, AudioDecodeThread@ha
/* 800BDC9C 000BABDC  39 03 00 00 */	addi r8, r3, 0
/* 800BDCA0 000BABE0  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 800BDCA4 000BABE4  93 E1 00 14 */	stw r31, 0x14(r1)
/* 800BDCA8 000BABE8  3B E4 CD 90 */	addi r31, r4, AudioDecodeThread@l
/* 800BDCAC 000BABEC  41 82 00 30 */	beq lbl_800BDCDC
/* 800BDCB0 000BABF0  3C 60 80 0C */	lis r3, AudioDecoderForOnMemory__FPv@ha
/* 800BDCB4 000BABF4  38 83 DB 44 */	addi r4, r3, AudioDecoderForOnMemory__FPv@l
/* 800BDCB8 000BABF8  38 7F 00 00 */	addi r3, r31, 0
/* 800BDCBC 000BABFC  38 DF 13 10 */	addi r6, r31, 0x1310
/* 800BDCC0 000BAC00  38 E0 10 00 */	li r7, 0x1000
/* 800BDCC4 000BAC04  39 20 00 01 */	li r9, 1
/* 800BDCC8 000BAC08  4B FD 55 A1 */	bl OSCreateThread
/* 800BDCCC 000BAC0C  2C 03 00 00 */	cmpwi r3, 0
/* 800BDCD0 000BAC10  40 82 00 3C */	bne lbl_800BDD0C
/* 800BDCD4 000BAC14  38 60 00 00 */	li r3, 0
/* 800BDCD8 000BAC18  48 00 00 60 */	b lbl_800BDD38
lbl_800BDCDC:
/* 800BDCDC 000BAC1C  3C 60 80 0C */	lis r3, AudioDecoder__FPv@ha
/* 800BDCE0 000BAC20  38 83 DB F4 */	addi r4, r3, AudioDecoder__FPv@l
/* 800BDCE4 000BAC24  38 7F 00 00 */	addi r3, r31, 0
/* 800BDCE8 000BAC28  38 DF 13 10 */	addi r6, r31, 0x1310
/* 800BDCEC 000BAC2C  38 A0 00 00 */	li r5, 0
/* 800BDCF0 000BAC30  38 E0 10 00 */	li r7, 0x1000
/* 800BDCF4 000BAC34  39 20 00 01 */	li r9, 1
/* 800BDCF8 000BAC38  4B FD 55 71 */	bl OSCreateThread
/* 800BDCFC 000BAC3C  2C 03 00 00 */	cmpwi r3, 0
/* 800BDD00 000BAC40  40 82 00 0C */	bne lbl_800BDD0C
/* 800BDD04 000BAC44  38 60 00 00 */	li r3, 0
/* 800BDD08 000BAC48  48 00 00 30 */	b lbl_800BDD38
lbl_800BDD0C:
/* 800BDD0C 000BAC4C  38 7F 13 10 */	addi r3, r31, 0x1310
/* 800BDD10 000BAC50  38 9F 13 50 */	addi r4, r31, 0x1350
/* 800BDD14 000BAC54  38 A0 00 03 */	li r5, 3
/* 800BDD18 000BAC58  4B FD 2D 39 */	bl OSInitMessageQueue
/* 800BDD1C 000BAC5C  38 7F 13 30 */	addi r3, r31, 0x1330
/* 800BDD20 000BAC60  38 9F 13 5C */	addi r4, r31, 0x135c
/* 800BDD24 000BAC64  38 A0 00 03 */	li r5, 3
/* 800BDD28 000BAC68  4B FD 2D 29 */	bl OSInitMessageQueue
/* 800BDD2C 000BAC6C  38 00 00 01 */	li r0, 1
/* 800BDD30 000BAC70  90 0D 97 18 */	stw r0, AudioDecodeThreadCreated-_SDA_BASE_(r13)
/* 800BDD34 000BAC74  38 60 00 01 */	li r3, 1
lbl_800BDD38:
/* 800BDD38 000BAC78  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 800BDD3C 000BAC7C  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 800BDD40 000BAC80  38 21 00 18 */	addi r1, r1, 0x18
/* 800BDD44 000BAC84  7C 08 03 A6 */	mtlr r0
/* 800BDD48 000BAC88  4E 80 00 20 */	blr 

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
.global AudioDecodeThread
AudioDecodeThread:
	.skip 0x310
.global AudioDecodeThreadStack
AudioDecodeThreadStack:
	.skip 0x1000
.global FreeAudioBufferQueue
FreeAudioBufferQueue:
	.skip 0x20
.global DecodedAudioBufferQueue
DecodedAudioBufferQueue:
	.skip 0x20
.global FreeAudioBufferMessage
FreeAudioBufferMessage:
	.skip 0xC
.global DecodedAudioBufferMessage
DecodedAudioBufferMessage:
	.skip 0x14

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
.global AudioDecodeThreadCreated
AudioDecodeThreadCreated:
	.skip 0x8

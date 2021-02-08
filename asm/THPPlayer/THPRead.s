.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global PushReadedBuffer2
PushReadedBuffer2:
/* 800BFA60 000BC9A0  7C 08 02 A6 */	mflr r0
/* 800BFA64 000BC9A4  3C A0 80 3F */	lis r5, ReadedBufferQueue2@ha
/* 800BFA68 000BC9A8  90 01 00 04 */	stw r0, 4(r1)
/* 800BFA6C 000BC9AC  38 83 00 00 */	addi r4, r3, 0
/* 800BFA70 000BC9B0  38 65 F5 40 */	addi r3, r5, ReadedBufferQueue2@l
/* 800BFA74 000BC9B4  94 21 FF F8 */	stwu r1, -8(r1)
/* 800BFA78 000BC9B8  38 A0 00 01 */	li r5, 1
/* 800BFA7C 000BC9BC  4B FD 10 35 */	bl OSSendMessage
/* 800BFA80 000BC9C0  80 01 00 0C */	lwz r0, 0xc(r1)
/* 800BFA84 000BC9C4  38 21 00 08 */	addi r1, r1, 8
/* 800BFA88 000BC9C8  7C 08 03 A6 */	mtlr r0
/* 800BFA8C 000BC9CC  4E 80 00 20 */	blr 

.global PopReadedBuffer2
PopReadedBuffer2:
/* 800BFA90 000BC9D0  7C 08 02 A6 */	mflr r0
/* 800BFA94 000BC9D4  3C 60 80 3F */	lis r3, ReadedBufferQueue2@ha
/* 800BFA98 000BC9D8  90 01 00 04 */	stw r0, 4(r1)
/* 800BFA9C 000BC9DC  38 63 F5 40 */	addi r3, r3, ReadedBufferQueue2@l
/* 800BFAA0 000BC9E0  38 A0 00 01 */	li r5, 1
/* 800BFAA4 000BC9E4  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 800BFAA8 000BC9E8  38 81 00 08 */	addi r4, r1, 8
/* 800BFAAC 000BC9EC  4B FD 10 CD */	bl OSReceiveMessage
/* 800BFAB0 000BC9F0  80 01 00 14 */	lwz r0, 0x14(r1)
/* 800BFAB4 000BC9F4  80 61 00 08 */	lwz r3, 8(r1)
/* 800BFAB8 000BC9F8  38 21 00 10 */	addi r1, r1, 0x10
/* 800BFABC 000BC9FC  7C 08 03 A6 */	mtlr r0
/* 800BFAC0 000BCA00  4E 80 00 20 */	blr 

.global PushFreeReadBuffer
PushFreeReadBuffer:
/* 800BFAC4 000BCA04  7C 08 02 A6 */	mflr r0
/* 800BFAC8 000BCA08  3C A0 80 3F */	lis r5, FreeReadBufferQueue@ha
/* 800BFACC 000BCA0C  90 01 00 04 */	stw r0, 4(r1)
/* 800BFAD0 000BCA10  38 83 00 00 */	addi r4, r3, 0
/* 800BFAD4 000BCA14  38 65 F5 00 */	addi r3, r5, FreeReadBufferQueue@l
/* 800BFAD8 000BCA18  94 21 FF F8 */	stwu r1, -8(r1)
/* 800BFADC 000BCA1C  38 A0 00 01 */	li r5, 1
/* 800BFAE0 000BCA20  4B FD 0F D1 */	bl OSSendMessage
/* 800BFAE4 000BCA24  80 01 00 0C */	lwz r0, 0xc(r1)
/* 800BFAE8 000BCA28  38 21 00 08 */	addi r1, r1, 8
/* 800BFAEC 000BCA2C  7C 08 03 A6 */	mtlr r0
/* 800BFAF0 000BCA30  4E 80 00 20 */	blr 

.global PopReadedBuffer
PopReadedBuffer:
/* 800BFAF4 000BCA34  7C 08 02 A6 */	mflr r0
/* 800BFAF8 000BCA38  3C 60 80 3F */	lis r3, ReadedBufferQueue@ha
/* 800BFAFC 000BCA3C  90 01 00 04 */	stw r0, 4(r1)
/* 800BFB00 000BCA40  38 63 F5 20 */	addi r3, r3, ReadedBufferQueue@l
/* 800BFB04 000BCA44  38 A0 00 01 */	li r5, 1
/* 800BFB08 000BCA48  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 800BFB0C 000BCA4C  38 81 00 08 */	addi r4, r1, 8
/* 800BFB10 000BCA50  4B FD 10 69 */	bl OSReceiveMessage
/* 800BFB14 000BCA54  80 01 00 14 */	lwz r0, 0x14(r1)
/* 800BFB18 000BCA58  80 61 00 08 */	lwz r3, 8(r1)
/* 800BFB1C 000BCA5C  38 21 00 10 */	addi r1, r1, 0x10
/* 800BFB20 000BCA60  7C 08 03 A6 */	mtlr r0
/* 800BFB24 000BCA64  4E 80 00 20 */	blr 

.global Reader__FPv
Reader__FPv:
/* 800BFB28 000BCA68  7C 08 02 A6 */	mflr r0
/* 800BFB2C 000BCA6C  3C 60 80 3F */	lis r3, ActivePlayer@ha
/* 800BFB30 000BCA70  90 01 00 04 */	stw r0, 4(r1)
/* 800BFB34 000BCA74  94 21 FF C8 */	stwu r1, -0x38(r1)
/* 800BFB38 000BCA78  BF 41 00 20 */	stmw r26, 0x20(r1)
/* 800BFB3C 000BCA7C  3B 83 E1 00 */	addi r28, r3, ActivePlayer@l
/* 800BFB40 000BCA80  3C 60 80 3F */	lis r3, FreeReadBufferQueue@ha
/* 800BFB44 000BCA84  3B 63 F5 00 */	addi r27, r3, FreeReadBufferQueue@l
/* 800BFB48 000BCA88  3B A0 00 00 */	li r29, 0
/* 800BFB4C 000BCA8C  83 FC 00 B8 */	lwz r31, 0xb8(r28)
/* 800BFB50 000BCA90  83 DC 00 BC */	lwz r30, 0xbc(r28)
lbl_800BFB54:
/* 800BFB54 000BCA94  38 7B 00 00 */	addi r3, r27, 0
/* 800BFB58 000BCA98  38 81 00 14 */	addi r4, r1, 0x14
/* 800BFB5C 000BCA9C  38 A0 00 01 */	li r5, 1
/* 800BFB60 000BCAA0  4B FD 10 19 */	bl OSReceiveMessage
/* 800BFB64 000BCAA4  83 41 00 14 */	lwz r26, 0x14(r1)
/* 800BFB68 000BCAA8  38 7C 00 00 */	addi r3, r28, 0
/* 800BFB6C 000BCAAC  38 BE 00 00 */	addi r5, r30, 0
/* 800BFB70 000BCAB0  80 9A 00 00 */	lwz r4, 0(r26)
/* 800BFB74 000BCAB4  38 DF 00 00 */	addi r6, r31, 0
/* 800BFB78 000BCAB8  38 E0 00 02 */	li r7, 2
/* 800BFB7C 000BCABC  4B FD 6B 19 */	bl DVDReadPrio
/* 800BFB80 000BCAC0  7C 03 F0 00 */	cmpw r3, r30
/* 800BFB84 000BCAC4  41 82 00 2C */	beq lbl_800BFBB0
/* 800BFB88 000BCAC8  2C 03 FF FF */	cmpwi r3, -1
/* 800BFB8C 000BCACC  40 82 00 0C */	bne lbl_800BFB98
/* 800BFB90 000BCAD0  38 00 FF FF */	li r0, -1
/* 800BFB94 000BCAD4  90 1C 00 A8 */	stw r0, 0xa8(r28)
lbl_800BFB98:
/* 800BFB98 000BCAD8  2C 1D 00 00 */	cmpwi r29, 0
/* 800BFB9C 000BCADC  40 82 00 0C */	bne lbl_800BFBA8
/* 800BFBA0 000BCAE0  38 60 00 00 */	li r3, 0
/* 800BFBA4 000BCAE4  4B FF F6 4D */	bl PrepareReady__Fi
lbl_800BFBA8:
/* 800BFBA8 000BCAE8  38 7B 00 D8 */	addi r3, r27, 0xd8
/* 800BFBAC 000BCAEC  4B FD 3E E5 */	bl OSSuspendThread
lbl_800BFBB0:
/* 800BFBB0 000BCAF0  93 BA 00 04 */	stw r29, 4(r26)
/* 800BFBB4 000BCAF4  38 9A 00 00 */	addi r4, r26, 0
/* 800BFBB8 000BCAF8  38 7B 00 20 */	addi r3, r27, 0x20
/* 800BFBBC 000BCAFC  38 A0 00 01 */	li r5, 1
/* 800BFBC0 000BCB00  4B FD 0E F1 */	bl OSSendMessage
/* 800BFBC4 000BCB04  80 1C 00 C0 */	lwz r0, 0xc0(r28)
/* 800BFBC8 000BCB08  7F FF F2 14 */	add r31, r31, r30
/* 800BFBCC 000BCB0C  80 BC 00 50 */	lwz r5, 0x50(r28)
/* 800BFBD0 000BCB10  7C 7D 02 14 */	add r3, r29, r0
/* 800BFBD4 000BCB14  80 9A 00 00 */	lwz r4, 0(r26)
/* 800BFBD8 000BCB18  7C 03 2B 96 */	divwu r0, r3, r5
/* 800BFBDC 000BCB1C  83 C4 00 00 */	lwz r30, 0(r4)
/* 800BFBE0 000BCB20  7C 00 29 D6 */	mullw r0, r0, r5
/* 800BFBE4 000BCB24  7C 60 18 50 */	subf r3, r0, r3
/* 800BFBE8 000BCB28  38 05 FF FF */	addi r0, r5, -1
/* 800BFBEC 000BCB2C  7C 03 00 40 */	cmplw r3, r0
/* 800BFBF0 000BCB30  40 82 00 20 */	bne lbl_800BFC10
/* 800BFBF4 000BCB34  88 1C 00 A6 */	lbz r0, 0xa6(r28)
/* 800BFBF8 000BCB38  54 00 07 FF */	clrlwi. r0, r0, 0x1f
/* 800BFBFC 000BCB3C  41 82 00 0C */	beq lbl_800BFC08
/* 800BFC00 000BCB40  83 FC 00 64 */	lwz r31, 0x64(r28)
/* 800BFC04 000BCB44  48 00 00 0C */	b lbl_800BFC10
lbl_800BFC08:
/* 800BFC08 000BCB48  38 7B 00 D8 */	addi r3, r27, 0xd8
/* 800BFC0C 000BCB4C  4B FD 3E 85 */	bl OSSuspendThread
lbl_800BFC10:
/* 800BFC10 000BCB50  3B BD 00 01 */	addi r29, r29, 1
/* 800BFC14 000BCB54  4B FF FF 40 */	b lbl_800BFB54

.global ReadThreadCancel
ReadThreadCancel:
/* 800BFC18 000BCB58  7C 08 02 A6 */	mflr r0
/* 800BFC1C 000BCB5C  90 01 00 04 */	stw r0, 4(r1)
/* 800BFC20 000BCB60  94 21 FF F8 */	stwu r1, -8(r1)
/* 800BFC24 000BCB64  80 0D 97 38 */	lwz r0, ReadThreadCreated-_SDA_BASE_(r13)
/* 800BFC28 000BCB68  2C 00 00 00 */	cmpwi r0, 0
/* 800BFC2C 000BCB6C  41 82 00 18 */	beq lbl_800BFC44
/* 800BFC30 000BCB70  3C 60 80 3F */	lis r3, ReadThread@ha
/* 800BFC34 000BCB74  38 63 F5 D8 */	addi r3, r3, ReadThread@l
/* 800BFC38 000BCB78  4B FD 38 35 */	bl OSCancelThread
/* 800BFC3C 000BCB7C  38 00 00 00 */	li r0, 0
/* 800BFC40 000BCB80  90 0D 97 38 */	stw r0, ReadThreadCreated-_SDA_BASE_(r13)
lbl_800BFC44:
/* 800BFC44 000BCB84  80 01 00 0C */	lwz r0, 0xc(r1)
/* 800BFC48 000BCB88  38 21 00 08 */	addi r1, r1, 8
/* 800BFC4C 000BCB8C  7C 08 03 A6 */	mtlr r0
/* 800BFC50 000BCB90  4E 80 00 20 */	blr 

.global ReadThreadStart
ReadThreadStart:
/* 800BFC54 000BCB94  7C 08 02 A6 */	mflr r0
/* 800BFC58 000BCB98  90 01 00 04 */	stw r0, 4(r1)
/* 800BFC5C 000BCB9C  94 21 FF F8 */	stwu r1, -8(r1)
/* 800BFC60 000BCBA0  80 0D 97 38 */	lwz r0, ReadThreadCreated-_SDA_BASE_(r13)
/* 800BFC64 000BCBA4  2C 00 00 00 */	cmpwi r0, 0
/* 800BFC68 000BCBA8  41 82 00 10 */	beq lbl_800BFC78
/* 800BFC6C 000BCBAC  3C 60 80 3F */	lis r3, ReadThread@ha
/* 800BFC70 000BCBB0  38 63 F5 D8 */	addi r3, r3, ReadThread@l
/* 800BFC74 000BCBB4  4B FD 3B 95 */	bl OSResumeThread
lbl_800BFC78:
/* 800BFC78 000BCBB8  80 01 00 0C */	lwz r0, 0xc(r1)
/* 800BFC7C 000BCBBC  38 21 00 08 */	addi r1, r1, 8
/* 800BFC80 000BCBC0  7C 08 03 A6 */	mtlr r0
/* 800BFC84 000BCBC4  4E 80 00 20 */	blr 

.global CreateReadThread
CreateReadThread:
/* 800BFC88 000BCBC8  7C 08 02 A6 */	mflr r0
/* 800BFC8C 000BCBCC  3C 80 80 3F */	lis r4, FreeReadBufferQueue@ha
/* 800BFC90 000BCBD0  90 01 00 04 */	stw r0, 4(r1)
/* 800BFC94 000BCBD4  39 03 00 00 */	addi r8, r3, 0
/* 800BFC98 000BCBD8  38 A0 00 00 */	li r5, 0
/* 800BFC9C 000BCBDC  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 800BFCA0 000BCBE0  38 E0 10 00 */	li r7, 0x1000
/* 800BFCA4 000BCBE4  39 20 00 01 */	li r9, 1
/* 800BFCA8 000BCBE8  93 E1 00 14 */	stw r31, 0x14(r1)
/* 800BFCAC 000BCBEC  3B E4 F5 00 */	addi r31, r4, FreeReadBufferQueue@l
/* 800BFCB0 000BCBF0  3C 80 80 0C */	lis r4, Reader__FPv@ha
/* 800BFCB4 000BCBF4  38 84 FB 28 */	addi r4, r4, Reader__FPv@l
/* 800BFCB8 000BCBF8  38 7F 00 D8 */	addi r3, r31, 0xd8
/* 800BFCBC 000BCBFC  38 DF 13 E8 */	addi r6, r31, 0x13e8
/* 800BFCC0 000BCC00  4B FD 35 A9 */	bl OSCreateThread
/* 800BFCC4 000BCC04  2C 03 00 00 */	cmpwi r3, 0
/* 800BFCC8 000BCC08  40 82 00 0C */	bne lbl_800BFCD4
/* 800BFCCC 000BCC0C  38 60 00 00 */	li r3, 0
/* 800BFCD0 000BCC10  48 00 00 40 */	b lbl_800BFD10
lbl_800BFCD4:
/* 800BFCD4 000BCC14  38 7F 00 00 */	addi r3, r31, 0
/* 800BFCD8 000BCC18  38 9F 00 60 */	addi r4, r31, 0x60
/* 800BFCDC 000BCC1C  38 A0 00 0A */	li r5, 0xa
/* 800BFCE0 000BCC20  4B FD 0D 71 */	bl OSInitMessageQueue
/* 800BFCE4 000BCC24  38 7F 00 20 */	addi r3, r31, 0x20
/* 800BFCE8 000BCC28  38 9F 00 88 */	addi r4, r31, 0x88
/* 800BFCEC 000BCC2C  38 A0 00 0A */	li r5, 0xa
/* 800BFCF0 000BCC30  4B FD 0D 61 */	bl OSInitMessageQueue
/* 800BFCF4 000BCC34  38 7F 00 40 */	addi r3, r31, 0x40
/* 800BFCF8 000BCC38  38 9F 00 B0 */	addi r4, r31, 0xb0
/* 800BFCFC 000BCC3C  38 A0 00 0A */	li r5, 0xa
/* 800BFD00 000BCC40  4B FD 0D 51 */	bl OSInitMessageQueue
/* 800BFD04 000BCC44  38 00 00 01 */	li r0, 1
/* 800BFD08 000BCC48  90 0D 97 38 */	stw r0, ReadThreadCreated-_SDA_BASE_(r13)
/* 800BFD0C 000BCC4C  38 60 00 01 */	li r3, 1
lbl_800BFD10:
/* 800BFD10 000BCC50  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 800BFD14 000BCC54  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 800BFD18 000BCC58  38 21 00 18 */	addi r1, r1, 0x18
/* 800BFD1C 000BCC5C  7C 08 03 A6 */	mtlr r0
/* 800BFD20 000BCC60  4E 80 00 20 */	blr 

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
FreeReadBufferQueue:
	.skip 0x20
ReadedBufferQueue:
	.skip 0x20
ReadedBufferQueue2:
	.skip 0x20
FreeReadBufferMessage:
	.skip 0x28
ReadedBufferMessage:
	.skip 0x28
ReadedBufferMessage2:
	.skip 0x28
ReadThread:
	.skip 0x310
ReadThreadStack:
	.skip 0x1000

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
ReadThreadCreated:
	.skip 0x8

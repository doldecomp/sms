.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global Console_printf
Console_printf:
/* 8005B968 000588A8  94 21 FF 90 */	stwu r1, -0x70(r1)
/* 8005B96C 000588AC  40 86 00 24 */	bne cr1, lbl_8005B990
/* 8005B970 000588B0  D8 21 00 28 */	stfd f1, 0x28(r1)
/* 8005B974 000588B4  D8 41 00 30 */	stfd f2, 0x30(r1)
/* 8005B978 000588B8  D8 61 00 38 */	stfd f3, 0x38(r1)
/* 8005B97C 000588BC  D8 81 00 40 */	stfd f4, 0x40(r1)
/* 8005B980 000588C0  D8 A1 00 48 */	stfd f5, 0x48(r1)
/* 8005B984 000588C4  D8 C1 00 50 */	stfd f6, 0x50(r1)
/* 8005B988 000588C8  D8 E1 00 58 */	stfd f7, 0x58(r1)
/* 8005B98C 000588CC  D9 01 00 60 */	stfd f8, 0x60(r1)
lbl_8005B990:
/* 8005B990 000588D0  90 61 00 08 */	stw r3, 8(r1)
/* 8005B994 000588D4  90 81 00 0C */	stw r4, 0xc(r1)
/* 8005B998 000588D8  90 A1 00 10 */	stw r5, 0x10(r1)
/* 8005B99C 000588DC  90 C1 00 14 */	stw r6, 0x14(r1)
/* 8005B9A0 000588E0  90 E1 00 18 */	stw r7, 0x18(r1)
/* 8005B9A4 000588E4  91 01 00 1C */	stw r8, 0x1c(r1)
/* 8005B9A8 000588E8  91 21 00 20 */	stw r9, 0x20(r1)
/* 8005B9AC 000588EC  91 41 00 24 */	stw r10, 0x24(r1)
/* 8005B9B0 000588F0  38 21 00 70 */	addi r1, r1, 0x70
/* 8005B9B4 000588F4  4E 80 00 20 */	blr 

.global setDSPSyncCount__Q28JASystem11AudioThreadFUl
setDSPSyncCount__Q28JASystem11AudioThreadFUl:
/* 8005B9B8 000588F8  90 6D 90 C8 */	stw r3, intcount__Q28JASystem11AudioThread@sda21(r13)
/* 8005B9BC 000588FC  4E 80 00 20 */	blr 

.global getDSPSyncCount__Q28JASystem11AudioThreadFv
getDSPSyncCount__Q28JASystem11AudioThreadFv:
/* 8005B9C0 00058900  80 6D 90 C8 */	lwz r3, intcount__Q28JASystem11AudioThread@sda21(r13)
/* 8005B9C4 00058904  4E 80 00 20 */	blr 

.global syncAudio__Q28JASystem11AudioThreadFv
syncAudio__Q28JASystem11AudioThreadFv:
/* 8005B9C8 00058908  7C 08 02 A6 */	mflr r0
/* 8005B9CC 0005890C  90 01 00 04 */	stw r0, 4(r1)
/* 8005B9D0 00058910  94 21 FF F8 */	stwu r1, -8(r1)
/* 8005B9D4 00058914  88 0D 90 DC */	lbz r0, init$121@sda21(r13)
/* 8005B9D8 00058918  7C 00 07 75 */	extsb. r0, r0
/* 8005B9DC 0005891C  40 82 00 10 */	bne lbl_8005B9EC
/* 8005B9E0 00058920  38 00 00 01 */	li r0, 1
/* 8005B9E4 00058924  90 0D 90 D8 */	stw r0, first$120@sda21(r13)
/* 8005B9E8 00058928  98 0D 90 DC */	stb r0, init$121@sda21(r13)
lbl_8005B9EC:
/* 8005B9EC 0005892C  80 0D 90 D8 */	lwz r0, first$120@sda21(r13)
/* 8005B9F0 00058930  28 00 00 00 */	cmplwi r0, 0
/* 8005B9F4 00058934  40 82 00 0C */	bne lbl_8005BA00
/* 8005B9F8 00058938  38 60 00 04 */	li r3, 4
/* 8005B9FC 0005893C  48 00 83 D5 */	bl probeFinish__Q28JASystem6KernelFl
lbl_8005BA00:
/* 8005BA00 00058940  38 00 00 00 */	li r0, 0
/* 8005BA04 00058944  3C 60 80 37 */	lis r3, $$2128@ha
/* 8005BA08 00058948  90 0D 90 D8 */	stw r0, first$120@sda21(r13)
/* 8005BA0C 0005894C  38 83 12 50 */	addi r4, r3, $$2128@l
/* 8005BA10 00058950  38 60 00 04 */	li r3, 4
/* 8005BA14 00058954  48 00 83 B9 */	bl probeStart__Q28JASystem6KernelFlPc
/* 8005BA18 00058958  80 0D 90 C4 */	lwz r0, audioproc_mq_init__Q28JASystem11AudioThread@sda21(r13)
/* 8005BA1C 0005895C  28 00 00 00 */	cmplwi r0, 0
/* 8005BA20 00058960  41 82 00 18 */	beq lbl_8005BA38
/* 8005BA24 00058964  3C 60 80 3E */	lis r3, audioproc_mq__Q28JASystem11AudioThread@ha
/* 8005BA28 00058968  38 63 63 60 */	addi r3, r3, audioproc_mq__Q28JASystem11AudioThread@l
/* 8005BA2C 0005896C  38 80 00 00 */	li r4, 0
/* 8005BA30 00058970  38 A0 00 00 */	li r5, 0
/* 8005BA34 00058974  48 03 50 7D */	bl OSSendMessage
lbl_8005BA38:
/* 8005BA38 00058978  80 01 00 0C */	lwz r0, 0xc(r1)
/* 8005BA3C 0005897C  38 21 00 08 */	addi r1, r1, 8
/* 8005BA40 00058980  7C 08 03 A6 */	mtlr r0
/* 8005BA44 00058984  4E 80 00 20 */	blr 

.global audioproc__Q28JASystem11AudioThreadFPv
audioproc__Q28JASystem11AudioThreadFPv:
/* 8005BA48 00058988  7C 08 02 A6 */	mflr r0
/* 8005BA4C 0005898C  3C 80 80 3E */	lis r4, audioproc_mq__Q28JASystem11AudioThread@ha
/* 8005BA50 00058990  90 01 00 04 */	stw r0, 4(r1)
/* 8005BA54 00058994  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8005BA58 00058998  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8005BA5C 0005899C  93 C1 00 10 */	stw r30, 0x10(r1)
/* 8005BA60 000589A0  3B C4 63 60 */	addi r30, r4, audioproc_mq__Q28JASystem11AudioThread@l
/* 8005BA64 000589A4  38 60 00 04 */	li r3, 4
/* 8005BA68 000589A8  64 63 00 04 */	oris r3, r3, 4
/* 8005BA6C 000589AC  7C 72 E3 A6 */	mtspr 0x392, r3
/* 8005BA70 000589B0  38 60 00 05 */	li r3, 5
/* 8005BA74 000589B4  64 63 00 05 */	oris r3, r3, 5
/* 8005BA78 000589B8  7C 73 E3 A6 */	mtspr 0x393, r3
/* 8005BA7C 000589BC  38 60 00 06 */	li r3, 6
/* 8005BA80 000589C0  64 63 00 06 */	oris r3, r3, 6
/* 8005BA84 000589C4  7C 74 E3 A6 */	mtspr 0x394, r3
/* 8005BA88 000589C8  38 60 00 07 */	li r3, 7
/* 8005BA8C 000589CC  64 63 00 07 */	oris r3, r3, 7
/* 8005BA90 000589D0  7C 75 E3 A6 */	mtspr 0x395, r3
/* 8005BA94 000589D4  38 7E 00 00 */	addi r3, r30, 0
/* 8005BA98 000589D8  38 9E 00 20 */	addi r4, r30, 0x20
/* 8005BA9C 000589DC  38 A0 00 10 */	li r5, 0x10
/* 8005BAA0 000589E0  48 03 4F B1 */	bl OSInitMessageQueue
/* 8005BAA4 000589E4  3B E0 00 01 */	li r31, 1
/* 8005BAA8 000589E8  93 ED 90 C4 */	stw r31, audioproc_mq_init__Q28JASystem11AudioThread@sda21(r13)
/* 8005BAAC 000589EC  48 00 4B 6D */	bl init__Q28JASystem6KernelFv
/* 8005BAB0 000589F0  80 0D 90 C0 */	lwz r0, isDSPBoot__Q28JASystem11AudioThread@sda21(r13)
/* 8005BAB4 000589F4  2C 00 00 00 */	cmpwi r0, 0
/* 8005BAB8 000589F8  40 82 00 14 */	bne lbl_8005BACC
/* 8005BABC 000589FC  3C 80 80 06 */	lis r4, syncDSP__Q28JASystem11AudioThreadFPv@ha
/* 8005BAC0 00058A00  38 64 BB A8 */	addi r3, r4, syncDSP__Q28JASystem11AudioThreadFPv@l
/* 8005BAC4 00058A04  48 02 63 1D */	bl DspBoot__FPFPv_v
/* 8005BAC8 00058A08  93 ED 90 C0 */	stw r31, isDSPBoot__Q28JASystem11AudioThread@sda21(r13)
lbl_8005BACC:
/* 8005BACC 00058A0C  48 00 2E D9 */	bl init__Q28JASystem6DriverFv
/* 8005BAD0 00058A10  80 6D 91 FC */	lwz r3, gAiSetting__Q28JASystem6Kernel@sda21(r13)
/* 8005BAD4 00058A14  48 04 14 F9 */	bl AISetDSPSampleRate
/* 8005BAD8 00058A18  3C 80 80 06 */	lis r4, syncAudio__Q28JASystem11AudioThreadFv@ha
/* 8005BADC 00058A1C  38 64 B9 C8 */	addi r3, r4, syncAudio__Q28JASystem11AudioThreadFv@l
/* 8005BAE0 00058A20  48 04 13 09 */	bl AIRegisterDMACallback
/* 8005BAE4 00058A24  48 04 13 D1 */	bl AIStartDMA
lbl_8005BAE8:
/* 8005BAE8 00058A28  38 7E 00 00 */	addi r3, r30, 0
/* 8005BAEC 00058A2C  38 81 00 0C */	addi r4, r1, 0xc
/* 8005BAF0 00058A30  38 A0 00 01 */	li r5, 1
/* 8005BAF4 00058A34  48 03 50 85 */	bl OSReceiveMessage
/* 8005BAF8 00058A38  80 01 00 0C */	lwz r0, 0xc(r1)
/* 8005BAFC 00058A3C  2C 00 00 02 */	cmpwi r0, 2
/* 8005BB00 00058A40  41 82 FF E8 */	beq lbl_8005BAE8
/* 8005BB04 00058A44  40 80 00 14 */	bge lbl_8005BB18
/* 8005BB08 00058A48  2C 00 00 00 */	cmpwi r0, 0
/* 8005BB0C 00058A4C  41 82 00 18 */	beq lbl_8005BB24
/* 8005BB10 00058A50  40 80 00 1C */	bge lbl_8005BB2C
/* 8005BB14 00058A54  4B FF FF D4 */	b lbl_8005BAE8
lbl_8005BB18:
/* 8005BB18 00058A58  2C 00 00 04 */	cmpwi r0, 4
/* 8005BB1C 00058A5C  40 80 FF CC */	bge lbl_8005BAE8
/* 8005BB20 00058A60  48 00 00 64 */	b lbl_8005BB84
lbl_8005BB24:
/* 8005BB24 00058A64  48 00 4D 75 */	bl updateDac__Q28JASystem6KernelFv
/* 8005BB28 00058A68  4B FF FF C0 */	b lbl_8005BAE8
lbl_8005BB2C:
/* 8005BB2C 00058A6C  80 0D 90 C8 */	lwz r0, intcount__Q28JASystem11AudioThread@sda21(r13)
/* 8005BB30 00058A70  2C 00 00 00 */	cmpwi r0, 0
/* 8005BB34 00058A74  40 82 00 0C */	bne lbl_8005BB40
/* 8005BB38 00058A78  38 60 00 00 */	li r3, 0
/* 8005BB3C 00058A7C  48 00 00 54 */	b lbl_8005BB90
lbl_8005BB40:
/* 8005BB40 00058A80  80 8D 90 C8 */	lwz r4, intcount__Q28JASystem11AudioThread@sda21(r13)
/* 8005BB44 00058A84  38 04 FF FF */	addi r0, r4, -1
/* 8005BB48 00058A88  90 0D 90 C8 */	stw r0, intcount__Q28JASystem11AudioThread@sda21(r13)
/* 8005BB4C 00058A8C  80 0D 90 C8 */	lwz r0, intcount__Q28JASystem11AudioThread@sda21(r13)
/* 8005BB50 00058A90  2C 00 00 00 */	cmpwi r0, 0
/* 8005BB54 00058A94  40 82 00 14 */	bne lbl_8005BB68
/* 8005BB58 00058A98  38 60 00 07 */	li r3, 7
/* 8005BB5C 00058A9C  48 00 82 75 */	bl probeFinish__Q28JASystem6KernelFl
/* 8005BB60 00058AA0  48 00 2F 51 */	bl finishDSPFrame__Q28JASystem6DSPBufFv
/* 8005BB64 00058AA4  4B FF FF 84 */	b lbl_8005BAE8
lbl_8005BB68:
/* 8005BB68 00058AA8  38 60 00 02 */	li r3, 2
/* 8005BB6C 00058AAC  38 82 86 30 */	addi r4, r2, $$2148@sda21
/* 8005BB70 00058AB0  48 00 82 5D */	bl probeStart__Q28JASystem6KernelFlPc
/* 8005BB74 00058AB4  48 00 2E C5 */	bl updateDSP__Q28JASystem6DSPBufFv
/* 8005BB78 00058AB8  38 60 00 02 */	li r3, 2
/* 8005BB7C 00058ABC  48 00 82 55 */	bl probeFinish__Q28JASystem6KernelFl
/* 8005BB80 00058AC0  4B FF FF 68 */	b lbl_8005BAE8
lbl_8005BB84:
/* 8005BB84 00058AC4  38 60 00 00 */	li r3, 0
/* 8005BB88 00058AC8  48 03 78 01 */	bl OSExitThread
/* 8005BB8C 00058ACC  4B FF FF 5C */	b lbl_8005BAE8
lbl_8005BB90:
/* 8005BB90 00058AD0  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8005BB94 00058AD4  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8005BB98 00058AD8  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 8005BB9C 00058ADC  7C 08 03 A6 */	mtlr r0
/* 8005BBA0 00058AE0  38 21 00 18 */	addi r1, r1, 0x18
/* 8005BBA4 00058AE4  4E 80 00 20 */	blr 

.global syncDSP__Q28JASystem11AudioThreadFPv
syncDSP__Q28JASystem11AudioThreadFPv:
/* 8005BBA8 00058AE8  7C 08 02 A6 */	mflr r0
/* 8005BBAC 00058AEC  90 01 00 04 */	stw r0, 4(r1)
/* 8005BBB0 00058AF0  94 21 FF F8 */	stwu r1, -8(r1)
lbl_8005BBB4:
/* 8005BBB4 00058AF4  48 04 2A B5 */	bl DSPCheckMailFromDSP
/* 8005BBB8 00058AF8  28 03 00 00 */	cmplwi r3, 0
/* 8005BBBC 00058AFC  41 82 FF F8 */	beq lbl_8005BBB4
/* 8005BBC0 00058B00  48 04 2A B9 */	bl DSPReadMailFromDSP
/* 8005BBC4 00058B04  A0 0D 81 B8 */	lhz r0, JAS_DSP_PREFIX__Q28JASystem12DSPInterface@sda21(r13)
/* 8005BBC8 00058B08  54 64 84 3E */	srwi r4, r3, 0x10
/* 8005BBCC 00058B0C  7C 04 00 40 */	cmplw r4, r0
/* 8005BBD0 00058B10  40 82 00 50 */	bne lbl_8005BC20
/* 8005BBD4 00058B14  3C 80 00 01 */	lis r4, 0x0000FF00@ha
/* 8005BBD8 00058B18  54 65 04 2E */	rlwinm r5, r3, 0, 0x10, 0x17
/* 8005BBDC 00058B1C  38 04 FF 00 */	addi r0, r4, 0x0000FF00@l
/* 8005BBE0 00058B20  7C 05 00 00 */	cmpw r5, r0
/* 8005BBE4 00058B24  41 82 00 08 */	beq lbl_8005BBEC
/* 8005BBE8 00058B28  48 00 00 30 */	b lbl_8005BC18
lbl_8005BBEC:
/* 8005BBEC 00058B2C  80 0D 90 C4 */	lwz r0, audioproc_mq_init__Q28JASystem11AudioThread@sda21(r13)
/* 8005BBF0 00058B30  28 00 00 00 */	cmplwi r0, 0
/* 8005BBF4 00058B34  41 82 00 1C */	beq lbl_8005BC10
/* 8005BBF8 00058B38  3C 60 80 3E */	lis r3, audioproc_mq__Q28JASystem11AudioThread@ha
/* 8005BBFC 00058B3C  38 63 63 60 */	addi r3, r3, audioproc_mq__Q28JASystem11AudioThread@l
/* 8005BC00 00058B40  38 80 00 01 */	li r4, 1
/* 8005BC04 00058B44  38 A0 00 00 */	li r5, 0
/* 8005BC08 00058B48  48 03 4E A9 */	bl OSSendMessage
/* 8005BC0C 00058B4C  48 00 00 14 */	b lbl_8005BC20
lbl_8005BC10:
/* 8005BC10 00058B50  48 02 5F F1 */	bl DSPReleaseHalt__Fv
/* 8005BC14 00058B54  48 00 00 0C */	b lbl_8005BC20
lbl_8005BC18:
/* 8005BC18 00058B58  54 63 04 3E */	clrlwi r3, r3, 0x10
/* 8005BC1C 00058B5C  48 02 64 05 */	bl DspFinishWork__FUs
lbl_8005BC20:
/* 8005BC20 00058B60  80 01 00 0C */	lwz r0, 0xc(r1)
/* 8005BC24 00058B64  38 21 00 08 */	addi r1, r1, 8
/* 8005BC28 00058B68  7C 08 03 A6 */	mtlr r0
/* 8005BC2C 00058B6C  4E 80 00 20 */	blr 

.global stop__Q28JASystem11AudioThreadFv
stop__Q28JASystem11AudioThreadFv:
/* 8005BC30 00058B70  7C 08 02 A6 */	mflr r0
/* 8005BC34 00058B74  90 01 00 04 */	stw r0, 4(r1)
/* 8005BC38 00058B78  94 21 FF F8 */	stwu r1, -8(r1)
/* 8005BC3C 00058B7C  80 0D 90 C4 */	lwz r0, audioproc_mq_init__Q28JASystem11AudioThread@sda21(r13)
/* 8005BC40 00058B80  28 00 00 00 */	cmplwi r0, 0
/* 8005BC44 00058B84  41 82 00 2C */	beq lbl_8005BC70
/* 8005BC48 00058B88  3C 60 80 3E */	lis r3, audioproc_mq__Q28JASystem11AudioThread@ha
/* 8005BC4C 00058B8C  38 63 63 60 */	addi r3, r3, audioproc_mq__Q28JASystem11AudioThread@l
/* 8005BC50 00058B90  38 80 00 03 */	li r4, 3
/* 8005BC54 00058B94  38 A0 00 00 */	li r5, 0
/* 8005BC58 00058B98  48 03 4E 59 */	bl OSSendMessage
/* 8005BC5C 00058B9C  2C 03 00 00 */	cmpwi r3, 0
/* 8005BC60 00058BA0  40 82 00 10 */	bne lbl_8005BC70
/* 8005BC64 00058BA4  3C 60 80 40 */	lis r3, jac_audioThread__Q28JASystem11AudioThread@ha
/* 8005BC68 00058BA8  38 63 3E C0 */	addi r3, r3, jac_audioThread__Q28JASystem11AudioThread@l
/* 8005BC6C 00058BAC  48 03 78 01 */	bl OSCancelThread
lbl_8005BC70:
/* 8005BC70 00058BB0  80 01 00 0C */	lwz r0, 0xc(r1)
/* 8005BC74 00058BB4  38 21 00 08 */	addi r1, r1, 8
/* 8005BC78 00058BB8  7C 08 03 A6 */	mtlr r0
/* 8005BC7C 00058BBC  4E 80 00 20 */	blr 

.global setPriority__Q28JASystem11AudioThreadFUcUc
setPriority__Q28JASystem11AudioThreadFUcUc:
/* 8005BC80 00058BC0  54 63 06 3E */	clrlwi r3, r3, 0x18
/* 8005BC84 00058BC4  54 80 06 3E */	clrlwi r0, r4, 0x18
/* 8005BC88 00058BC8  90 6D 90 D0 */	stw r3, jac_pri__Q28JASystem11AudioThread@sda21(r13)
/* 8005BC8C 00058BCC  38 60 00 01 */	li r3, 1
/* 8005BC90 00058BD0  90 6D 90 CC */	stw r3, priority_set__Q28JASystem11AudioThread@sda21(r13)
/* 8005BC94 00058BD4  90 0D 90 D4 */	stw r0, dvd_pri__Q28JASystem11AudioThread@sda21(r13)
/* 8005BC98 00058BD8  4E 80 00 20 */	blr 

.global start__Q28JASystem11AudioThreadFP12JKRSolidHeapUlUl
start__Q28JASystem11AudioThreadFP12JKRSolidHeapUlUl:
/* 8005BC9C 00058BDC  7C 08 02 A6 */	mflr r0
/* 8005BCA0 00058BE0  90 01 00 04 */	stw r0, 4(r1)
/* 8005BCA4 00058BE4  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 8005BCA8 00058BE8  93 E1 00 24 */	stw r31, 0x24(r1)
/* 8005BCAC 00058BEC  3B E3 00 00 */	addi r31, r3, 0
/* 8005BCB0 00058BF0  93 C1 00 20 */	stw r30, 0x20(r1)
/* 8005BCB4 00058BF4  3B C4 00 00 */	addi r30, r4, 0
/* 8005BCB8 00058BF8  93 A1 00 1C */	stw r29, 0x1c(r1)
/* 8005BCBC 00058BFC  3B A5 00 00 */	addi r29, r5, 0
/* 8005BCC0 00058C00  80 0D 90 CC */	lwz r0, priority_set__Q28JASystem11AudioThread@sda21(r13)
/* 8005BCC4 00058C04  2C 00 00 00 */	cmpwi r0, 0
/* 8005BCC8 00058C08  40 82 00 20 */	bne lbl_8005BCE8
/* 8005BCCC 00058C0C  48 03 6F BD */	bl OSGetCurrentThread
/* 8005BCD0 00058C10  48 03 81 21 */	bl OSGetThreadPriority
/* 8005BCD4 00058C14  38 03 FF FD */	addi r0, r3, -3
/* 8005BCD8 00058C18  90 0D 90 D0 */	stw r0, jac_pri__Q28JASystem11AudioThread@sda21(r13)
/* 8005BCDC 00058C1C  80 6D 90 D0 */	lwz r3, jac_pri__Q28JASystem11AudioThread@sda21(r13)
/* 8005BCE0 00058C20  38 03 00 01 */	addi r0, r3, 1
/* 8005BCE4 00058C24  90 0D 90 D4 */	stw r0, dvd_pri__Q28JASystem11AudioThread@sda21(r13)
lbl_8005BCE8:
/* 8005BCE8 00058C28  7F E3 FB 78 */	mr r3, r31
/* 8005BCEC 00058C2C  48 00 80 E9 */	bl sysDramSetup__Q28JASystem6KernelFP12JKRSolidHeap
/* 8005BCF0 00058C30  7F C3 F3 78 */	mr r3, r30
/* 8005BCF4 00058C34  48 00 81 91 */	bl sysAramSetup__Q28JASystem6KernelFUl
/* 8005BCF8 00058C38  3C 60 80 40 */	lis r3, jac_audioStack__Q28JASystem11AudioThread@ha
/* 8005BCFC 00058C3C  3B E3 65 00 */	addi r31, r3, jac_audioStack__Q28JASystem11AudioThread@l
/* 8005BD00 00058C40  38 7F 00 00 */	addi r3, r31, 0
/* 8005BD04 00058C44  38 80 02 00 */	li r4, 0x200
/* 8005BD08 00058C48  48 00 7F F5 */	bl stackInit__Q28JASystem6KernelFPUxUl
/* 8005BD0C 00058C4C  57 A0 07 BD */	rlwinm. r0, r29, 0, 0x1e, 0x1e
/* 8005BD10 00058C50  41 82 00 38 */	beq lbl_8005BD48
/* 8005BD14 00058C54  3C 80 80 40 */	lis r4, jac_audioThread__Q28JASystem11AudioThread@ha
/* 8005BD18 00058C58  81 0D 90 D0 */	lwz r8, jac_pri__Q28JASystem11AudioThread@sda21(r13)
/* 8005BD1C 00058C5C  3C 60 80 06 */	lis r3, audioproc__Q28JASystem11AudioThreadFPv@ha
/* 8005BD20 00058C60  3B C4 3E C0 */	addi r30, r4, jac_audioThread__Q28JASystem11AudioThread@l
/* 8005BD24 00058C64  38 83 BA 48 */	addi r4, r3, audioproc__Q28JASystem11AudioThreadFPv@l
/* 8005BD28 00058C68  38 7E 00 00 */	addi r3, r30, 0
/* 8005BD2C 00058C6C  38 DF 10 00 */	addi r6, r31, 0x1000
/* 8005BD30 00058C70  38 A0 00 00 */	li r5, 0
/* 8005BD34 00058C74  38 E0 10 00 */	li r7, 0x1000
/* 8005BD38 00058C78  39 20 00 01 */	li r9, 1
/* 8005BD3C 00058C7C  48 03 75 2D */	bl OSCreateThread
/* 8005BD40 00058C80  7F C3 F3 78 */	mr r3, r30
/* 8005BD44 00058C84  48 03 7A C5 */	bl OSResumeThread
lbl_8005BD48:
/* 8005BD48 00058C88  3C 60 80 40 */	lis r3, jac_dvdStack__Q28JASystem11AudioThread@ha
/* 8005BD4C 00058C8C  3B C3 78 20 */	addi r30, r3, jac_dvdStack__Q28JASystem11AudioThread@l
/* 8005BD50 00058C90  38 7E 00 00 */	addi r3, r30, 0
/* 8005BD54 00058C94  38 80 02 00 */	li r4, 0x200
/* 8005BD58 00058C98  48 00 7F A5 */	bl stackInit__Q28JASystem6KernelFPUxUl
/* 8005BD5C 00058C9C  57 A0 07 FF */	clrlwi. r0, r29, 0x1f
/* 8005BD60 00058CA0  41 82 00 3C */	beq lbl_8005BD9C
/* 8005BD64 00058CA4  48 00 5C B5 */	bl dvdProcInit__Q28JASystem3DvdFv
/* 8005BD68 00058CA8  3C 80 80 40 */	lis r4, jac_dvdThread__Q28JASystem11AudioThread@ha
/* 8005BD6C 00058CAC  81 0D 90 D4 */	lwz r8, dvd_pri__Q28JASystem11AudioThread@sda21(r13)
/* 8005BD70 00058CB0  3C 60 80 06 */	lis r3, dvdProc__Q28JASystem3DvdFPv@ha
/* 8005BD74 00058CB4  3B E4 75 00 */	addi r31, r4, jac_dvdThread__Q28JASystem11AudioThread@l
/* 8005BD78 00058CB8  38 83 1A C4 */	addi r4, r3, dvdProc__Q28JASystem3DvdFPv@l
/* 8005BD7C 00058CBC  38 7F 00 00 */	addi r3, r31, 0
/* 8005BD80 00058CC0  38 DE 10 00 */	addi r6, r30, 0x1000
/* 8005BD84 00058CC4  38 A0 00 00 */	li r5, 0
/* 8005BD88 00058CC8  38 E0 10 00 */	li r7, 0x1000
/* 8005BD8C 00058CCC  39 20 00 01 */	li r9, 1
/* 8005BD90 00058CD0  48 03 74 D9 */	bl OSCreateThread
/* 8005BD94 00058CD4  7F E3 FB 78 */	mr r3, r31
/* 8005BD98 00058CD8  48 03 7A 71 */	bl OSResumeThread
lbl_8005BD9C:
/* 8005BD9C 00058CDC  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 8005BDA0 00058CE0  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 8005BDA4 00058CE4  83 C1 00 20 */	lwz r30, 0x20(r1)
/* 8005BDA8 00058CE8  7C 08 03 A6 */	mtlr r0
/* 8005BDAC 00058CEC  83 A1 00 1C */	lwz r29, 0x1c(r1)
/* 8005BDB0 00058CF0  38 21 00 28 */	addi r1, r1, 0x28
/* 8005BDB4 00058CF4  4E 80 00 20 */	blr 

.section .rodata, "a"  # 0x8036FFA0 - 0x803A8380
.balign 8
$$2128:
	.incbin "baserom.dol", 0x36E250, 0x10

.section .sdata2, "a"  # 0x8040B460 - 0x80414020
.balign 8
$$2148:
	.incbin "baserom.dol", 0x3E4330, 0x8

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
audioproc_mq__Q28JASystem11AudioThread:
	.skip 0x20
msgbuf__Q28JASystem11AudioThread:
	.skip 0x40

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
isDSPBoot__Q28JASystem11AudioThread:
	.skip 0x4
audioproc_mq_init__Q28JASystem11AudioThread:
	.skip 0x4
intcount__Q28JASystem11AudioThread:
	.skip 0x4
priority_set__Q28JASystem11AudioThread:
	.skip 0x4
jac_pri__Q28JASystem11AudioThread:
	.skip 0x4
dvd_pri__Q28JASystem11AudioThread:
	.skip 0x4
first$120:
	.skip 0x4
init$121:
	.skip 0x4

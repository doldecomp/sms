.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global "__ct__Q27JGadget18TList_pointer_voidFRCQ27JGadget14TAllocator<Pv>"
"__ct__Q27JGadget18TList_pointer_voidFRCQ27JGadget14TAllocator<Pv>":
/* 800341FC 0003113C  88 04 00 00 */	lbz r0, 0(r4)
/* 80034200 00031140  38 80 00 00 */	li r4, 0
/* 80034204 00031144  98 03 00 00 */	stb r0, 0(r3)
/* 80034208 00031148  38 03 00 08 */	addi r0, r3, 8
/* 8003420C 0003114C  90 83 00 04 */	stw r4, 4(r3)
/* 80034210 00031150  90 03 00 08 */	stw r0, 8(r3)
/* 80034214 00031154  90 03 00 0C */	stw r0, 0xc(r3)
/* 80034218 00031158  4E 80 00 20 */	blr 

.global "erase__Q27JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>FQ37JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>8iterator"
"erase__Q27JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>FQ37JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>8iterator":
/* 8003421C 0003115C  7C 08 02 A6 */	mflr r0
/* 80034220 00031160  90 01 00 04 */	stw r0, 4(r1)
/* 80034224 00031164  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 80034228 00031168  93 E1 00 24 */	stw r31, 0x24(r1)
/* 8003422C 0003116C  93 C1 00 20 */	stw r30, 0x20(r1)
/* 80034230 00031170  3B C4 00 00 */	addi r30, r4, 0
/* 80034234 00031174  93 A1 00 1C */	stw r29, 0x1c(r1)
/* 80034238 00031178  3B A3 00 00 */	addi r29, r3, 0
/* 8003423C 0003117C  80 A5 00 00 */	lwz r5, 0(r5)
/* 80034240 00031180  83 E5 00 00 */	lwz r31, 0(r5)
/* 80034244 00031184  7C A3 2B 78 */	mr r3, r5
/* 80034248 00031188  80 85 00 04 */	lwz r4, 4(r5)
/* 8003424C 0003118C  93 E4 00 00 */	stw r31, 0(r4)
/* 80034250 00031190  80 05 00 04 */	lwz r0, 4(r5)
/* 80034254 00031194  90 1F 00 04 */	stw r0, 4(r31)
/* 80034258 00031198  4B FD 88 59 */	bl __dl__FPv
/* 8003425C 0003119C  80 7E 00 04 */	lwz r3, 4(r30)
/* 80034260 000311A0  38 03 FF FF */	addi r0, r3, -1
/* 80034264 000311A4  90 1E 00 04 */	stw r0, 4(r30)
/* 80034268 000311A8  93 FD 00 00 */	stw r31, 0(r29)
/* 8003426C 000311AC  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 80034270 000311B0  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 80034274 000311B4  83 C1 00 20 */	lwz r30, 0x20(r1)
/* 80034278 000311B8  7C 08 03 A6 */	mtlr r0
/* 8003427C 000311BC  83 A1 00 1C */	lwz r29, 0x1c(r1)
/* 80034280 000311C0  38 21 00 28 */	addi r1, r1, 0x28
/* 80034284 000311C4  4E 80 00 20 */	blr 

.global __dt__Q27JGadget18TList_pointer_voidFv
__dt__Q27JGadget18TList_pointer_voidFv:
/* 80034288 000311C8  7C 08 02 A6 */	mflr r0
/* 8003428C 000311CC  90 01 00 04 */	stw r0, 4(r1)
/* 80034290 000311D0  94 21 FF 98 */	stwu r1, -0x68(r1)
/* 80034294 000311D4  BF 61 00 54 */	stmw r27, 0x54(r1)
/* 80034298 000311D8  7C 7B 1B 79 */	or. r27, r3, r3
/* 8003429C 000311DC  3B 84 00 00 */	addi r28, r4, 0
/* 800342A0 000311E0  41 82 00 94 */	beq lbl_80034334
/* 800342A4 000311E4  41 82 00 80 */	beq lbl_80034324
/* 800342A8 000311E8  38 61 00 4C */	addi r3, r1, 0x4c
/* 800342AC 000311EC  38 9B 00 08 */	addi r4, r27, 8
/* 800342B0 000311F0  48 00 00 B5 */	bl "__ct__Q37JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>8iteratorFPQ37JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>6TNode_"
/* 800342B4 000311F4  80 01 00 4C */	lwz r0, 0x4c(r1)
/* 800342B8 000311F8  38 61 00 44 */	addi r3, r1, 0x44
/* 800342BC 000311FC  90 01 00 48 */	stw r0, 0x48(r1)
/* 800342C0 00031200  80 9B 00 08 */	lwz r4, 8(r27)
/* 800342C4 00031204  48 00 00 A1 */	bl "__ct__Q37JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>8iteratorFPQ37JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>6TNode_"
/* 800342C8 00031208  80 01 00 44 */	lwz r0, 0x44(r1)
/* 800342CC 0003120C  3B E1 00 30 */	addi r31, r1, 0x30
/* 800342D0 00031210  3B C1 00 28 */	addi r30, r1, 0x28
/* 800342D4 00031214  90 01 00 40 */	stw r0, 0x40(r1)
/* 800342D8 00031218  3B A1 00 24 */	addi r29, r1, 0x24
/* 800342DC 0003121C  48 00 00 24 */	b lbl_80034300
lbl_800342E0:
/* 800342E0 00031220  80 01 00 40 */	lwz r0, 0x40(r1)
/* 800342E4 00031224  38 9B 00 00 */	addi r4, r27, 0
/* 800342E8 00031228  38 BF 00 00 */	addi r5, r31, 0
/* 800342EC 0003122C  90 01 00 30 */	stw r0, 0x30(r1)
/* 800342F0 00031230  38 61 00 2C */	addi r3, r1, 0x2c
/* 800342F4 00031234  4B FF FF 29 */	bl "erase__Q27JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>FQ37JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>8iterator"
/* 800342F8 00031238  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 800342FC 0003123C  90 01 00 40 */	stw r0, 0x40(r1)
lbl_80034300:
/* 80034300 00031240  80 A1 00 48 */	lwz r5, 0x48(r1)
/* 80034304 00031244  7F A3 EB 78 */	mr r3, r29
/* 80034308 00031248  80 01 00 40 */	lwz r0, 0x40(r1)
/* 8003430C 0003124C  7F C4 F3 78 */	mr r4, r30
/* 80034310 00031250  90 A1 00 28 */	stw r5, 0x28(r1)
/* 80034314 00031254  90 01 00 24 */	stw r0, 0x24(r1)
/* 80034318 00031258  48 00 00 35 */	bl "__eq__7JGadgetFQ37JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>8iteratorQ37JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>8iterator"
/* 8003431C 0003125C  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 80034320 00031260  41 82 FF C0 */	beq lbl_800342E0
lbl_80034324:
/* 80034324 00031264  7F 80 07 35 */	extsh. r0, r28
/* 80034328 00031268  40 81 00 0C */	ble lbl_80034334
/* 8003432C 0003126C  7F 63 DB 78 */	mr r3, r27
/* 80034330 00031270  4B FD 87 81 */	bl __dl__FPv
lbl_80034334:
/* 80034334 00031274  7F 63 DB 78 */	mr r3, r27
/* 80034338 00031278  BB 61 00 54 */	lmw r27, 0x54(r1)
/* 8003433C 0003127C  80 01 00 6C */	lwz r0, 0x6c(r1)
/* 80034340 00031280  38 21 00 68 */	addi r1, r1, 0x68
/* 80034344 00031284  7C 08 03 A6 */	mtlr r0
/* 80034348 00031288  4E 80 00 20 */	blr 

.global "__eq__7JGadgetFQ37JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>8iteratorQ37JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>8iterator"
"__eq__7JGadgetFQ37JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>8iteratorQ37JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>8iterator":
/* 8003434C 0003128C  80 63 00 00 */	lwz r3, 0(r3)
/* 80034350 00031290  80 04 00 00 */	lwz r0, 0(r4)
/* 80034354 00031294  7C 03 00 50 */	subf r0, r3, r0
/* 80034358 00031298  7C 00 00 34 */	cntlzw r0, r0
/* 8003435C 0003129C  54 03 D9 7E */	srwi r3, r0, 5
/* 80034360 000312A0  4E 80 00 20 */	blr 

.global "__ct__Q37JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>8iteratorFPQ37JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>6TNode_"
"__ct__Q37JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>8iteratorFPQ37JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>6TNode_":
/* 80034364 000312A4  90 83 00 00 */	stw r4, 0(r3)
/* 80034368 000312A8  4E 80 00 20 */	blr 

.global "insert__Q27JGadget18TList_pointer_voidFQ37JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>8iteratorRCPv"
"insert__Q27JGadget18TList_pointer_voidFQ37JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>8iteratorRCPv":
/* 8003436C 000312AC  7C 08 02 A6 */	mflr r0
/* 80034370 000312B0  90 01 00 04 */	stw r0, 4(r1)
/* 80034374 000312B4  94 21 FF B0 */	stwu r1, -0x50(r1)
/* 80034378 000312B8  93 E1 00 4C */	stw r31, 0x4c(r1)
/* 8003437C 000312BC  93 C1 00 48 */	stw r30, 0x48(r1)
/* 80034380 000312C0  7C 9E 23 78 */	mr r30, r4
/* 80034384 000312C4  90 61 00 08 */	stw r3, 8(r1)
/* 80034388 000312C8  7F C3 F3 78 */	mr r3, r30
/* 8003438C 000312CC  80 05 00 00 */	lwz r0, 0(r5)
/* 80034390 000312D0  90 01 00 44 */	stw r0, 0x44(r1)
/* 80034394 000312D4  80 81 00 44 */	lwz r4, 0x44(r1)
/* 80034398 000312D8  83 E4 00 04 */	lwz r31, 4(r4)
/* 8003439C 000312DC  7F E5 FB 78 */	mr r5, r31
/* 800343A0 000312E0  48 00 00 55 */	bl "CreateNode___Q27JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>FPQ37JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>6TNode_PQ37JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>6TNode_RCPv"
/* 800343A4 000312E4  28 03 00 00 */	cmplwi r3, 0
/* 800343A8 000312E8  40 82 00 14 */	bne lbl_800343BC
/* 800343AC 000312EC  80 81 00 08 */	lwz r4, 8(r1)
/* 800343B0 000312F0  38 1E 00 08 */	addi r0, r30, 8
/* 800343B4 000312F4  90 04 00 00 */	stw r0, 0(r4)
/* 800343B8 000312F8  48 00 00 24 */	b lbl_800343DC
lbl_800343BC:
/* 800343BC 000312FC  80 81 00 44 */	lwz r4, 0x44(r1)
/* 800343C0 00031300  90 64 00 04 */	stw r3, 4(r4)
/* 800343C4 00031304  90 7F 00 00 */	stw r3, 0(r31)
/* 800343C8 00031308  80 9E 00 04 */	lwz r4, 4(r30)
/* 800343CC 0003130C  38 04 00 01 */	addi r0, r4, 1
/* 800343D0 00031310  90 1E 00 04 */	stw r0, 4(r30)
/* 800343D4 00031314  80 81 00 08 */	lwz r4, 8(r1)
/* 800343D8 00031318  90 64 00 00 */	stw r3, 0(r4)
lbl_800343DC:
/* 800343DC 0003131C  80 01 00 54 */	lwz r0, 0x54(r1)
/* 800343E0 00031320  83 E1 00 4C */	lwz r31, 0x4c(r1)
/* 800343E4 00031324  83 C1 00 48 */	lwz r30, 0x48(r1)
/* 800343E8 00031328  7C 08 03 A6 */	mtlr r0
/* 800343EC 0003132C  38 21 00 50 */	addi r1, r1, 0x50
/* 800343F0 00031330  4E 80 00 20 */	blr 

.global "CreateNode___Q27JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>FPQ37JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>6TNode_PQ37JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>6TNode_RCPv"
"CreateNode___Q27JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>FPQ37JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>6TNode_PQ37JGadget36TList<Pv,Q27JGadget14TAllocator<Pv>>6TNode_RCPv":
/* 800343F4 00031334  7C 08 02 A6 */	mflr r0
/* 800343F8 00031338  38 60 00 0C */	li r3, 0xc
/* 800343FC 0003133C  90 01 00 04 */	stw r0, 4(r1)
/* 80034400 00031340  94 21 FF C8 */	stwu r1, -0x38(r1)
/* 80034404 00031344  93 E1 00 34 */	stw r31, 0x34(r1)
/* 80034408 00031348  3B E6 00 00 */	addi r31, r6, 0
/* 8003440C 0003134C  93 C1 00 30 */	stw r30, 0x30(r1)
/* 80034410 00031350  3B C5 00 00 */	addi r30, r5, 0
/* 80034414 00031354  93 A1 00 2C */	stw r29, 0x2c(r1)
/* 80034418 00031358  3B A4 00 00 */	addi r29, r4, 0
/* 8003441C 0003135C  4B FD 84 95 */	bl __nw__FUl
/* 80034420 00031360  28 03 00 00 */	cmplwi r3, 0
/* 80034424 00031364  40 82 00 0C */	bne lbl_80034430
/* 80034428 00031368  38 60 00 00 */	li r3, 0
/* 8003442C 0003136C  48 00 00 20 */	b lbl_8003444C
lbl_80034430:
/* 80034430 00031370  93 A3 00 00 */	stw r29, 0(r3)
/* 80034434 00031374  34 83 00 08 */	addic. r4, r3, 8
/* 80034438 00031378  93 C3 00 04 */	stw r30, 4(r3)
/* 8003443C 0003137C  90 81 00 1C */	stw r4, 0x1c(r1)
/* 80034440 00031380  41 82 00 0C */	beq lbl_8003444C
/* 80034444 00031384  80 1F 00 00 */	lwz r0, 0(r31)
/* 80034448 00031388  90 04 00 00 */	stw r0, 0(r4)
lbl_8003444C:
/* 8003444C 0003138C  80 01 00 3C */	lwz r0, 0x3c(r1)
/* 80034450 00031390  83 E1 00 34 */	lwz r31, 0x34(r1)
/* 80034454 00031394  83 C1 00 30 */	lwz r30, 0x30(r1)
/* 80034458 00031398  7C 08 03 A6 */	mtlr r0
/* 8003445C 0003139C  83 A1 00 2C */	lwz r29, 0x2c(r1)
/* 80034460 000313A0  38 21 00 38 */	addi r1, r1, 0x38
/* 80034464 000313A4  4E 80 00 20 */	blr 

.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __ct__13J3DVertexDataFv
__ct__13J3DVertexDataFv:
/* 80037180 000340C0  38 00 00 00 */	li r0, 0
/* 80037184 000340C4  90 03 00 00 */	stw r0, 0(r3)
/* 80037188 000340C8  90 03 00 04 */	stw r0, 4(r3)
/* 8003718C 000340CC  90 03 00 08 */	stw r0, 8(r3)
/* 80037190 000340D0  90 03 00 0C */	stw r0, 0xc(r3)
/* 80037194 000340D4  90 03 00 10 */	stw r0, 0x10(r3)
/* 80037198 000340D8  90 03 00 14 */	stw r0, 0x14(r3)
/* 8003719C 000340DC  90 03 00 18 */	stw r0, 0x18(r3)
/* 800371A0 000340E0  90 03 00 1C */	stw r0, 0x1c(r3)
/* 800371A4 000340E4  90 03 00 20 */	stw r0, 0x20(r3)
/* 800371A8 000340E8  90 03 00 24 */	stw r0, 0x24(r3)
/* 800371AC 000340EC  90 03 00 28 */	stw r0, 0x28(r3)
/* 800371B0 000340F0  90 03 00 2C */	stw r0, 0x2c(r3)
/* 800371B4 000340F4  90 03 00 30 */	stw r0, 0x30(r3)
/* 800371B8 000340F8  90 03 00 34 */	stw r0, 0x34(r3)
/* 800371BC 000340FC  90 03 00 38 */	stw r0, 0x38(r3)
/* 800371C0 00034100  90 03 00 3C */	stw r0, 0x3c(r3)
/* 800371C4 00034104  90 03 00 40 */	stw r0, 0x40(r3)
/* 800371C8 00034108  4E 80 00 20 */	blr 

.global __dt__13J3DVertexDataFv
__dt__13J3DVertexDataFv:
/* 800371CC 0003410C  7C 08 02 A6 */	mflr r0
/* 800371D0 00034110  90 01 00 04 */	stw r0, 4(r1)
/* 800371D4 00034114  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 800371D8 00034118  93 E1 00 14 */	stw r31, 0x14(r1)
/* 800371DC 0003411C  7C 7F 1B 79 */	or. r31, r3, r3
/* 800371E0 00034120  41 82 00 14 */	beq lbl_800371F4
/* 800371E4 00034124  7C 80 07 35 */	extsh. r0, r4
/* 800371E8 00034128  40 81 00 0C */	ble lbl_800371F4
/* 800371EC 0003412C  7F E3 FB 78 */	mr r3, r31
/* 800371F0 00034130  4B FD 58 C1 */	bl __dl__FPv
lbl_800371F4:
/* 800371F4 00034134  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 800371F8 00034138  7F E3 FB 78 */	mr r3, r31
/* 800371FC 0003413C  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80037200 00034140  38 21 00 18 */	addi r1, r1, 0x18
/* 80037204 00034144  7C 08 03 A6 */	mtlr r0
/* 80037208 00034148  4E 80 00 20 */	blr 

.global __ct__15J3DVertexBufferFP13J3DVertexData
__ct__15J3DVertexBufferFP13J3DVertexData:
/* 8003720C 0003414C  3C A0 80 3B */	lis r5, __vt__15J3DVertexBuffer@ha
/* 80037210 00034150  38 05 9B D8 */	addi r0, r5, __vt__15J3DVertexBuffer@l
/* 80037214 00034154  90 03 00 38 */	stw r0, 0x38(r3)
/* 80037218 00034158  38 A0 00 00 */	li r5, 0
/* 8003721C 0003415C  90 83 00 00 */	stw r4, 0(r3)
/* 80037220 00034160  80 04 00 10 */	lwz r0, 0x10(r4)
/* 80037224 00034164  90 03 00 04 */	stw r0, 4(r3)
/* 80037228 00034168  80 04 00 14 */	lwz r0, 0x14(r4)
/* 8003722C 0003416C  90 03 00 0C */	stw r0, 0xc(r3)
/* 80037230 00034170  80 04 00 1C */	lwz r0, 0x1c(r4)
/* 80037234 00034174  90 03 00 14 */	stw r0, 0x14(r3)
/* 80037238 00034178  90 A3 00 08 */	stw r5, 8(r3)
/* 8003723C 0003417C  90 A3 00 10 */	stw r5, 0x10(r3)
/* 80037240 00034180  90 A3 00 18 */	stw r5, 0x18(r3)
/* 80037244 00034184  80 04 00 10 */	lwz r0, 0x10(r4)
/* 80037248 00034188  90 03 00 1C */	stw r0, 0x1c(r3)
/* 8003724C 0003418C  80 04 00 14 */	lwz r0, 0x14(r4)
/* 80037250 00034190  90 03 00 24 */	stw r0, 0x24(r3)
/* 80037254 00034194  90 A3 00 20 */	stw r5, 0x20(r3)
/* 80037258 00034198  90 A3 00 28 */	stw r5, 0x28(r3)
/* 8003725C 0003419C  80 04 00 10 */	lwz r0, 0x10(r4)
/* 80037260 000341A0  90 03 00 2C */	stw r0, 0x2c(r3)
/* 80037264 000341A4  80 04 00 14 */	lwz r0, 0x14(r4)
/* 80037268 000341A8  90 03 00 30 */	stw r0, 0x30(r3)
/* 8003726C 000341AC  80 04 00 1C */	lwz r0, 0x1c(r4)
/* 80037270 000341B0  90 03 00 34 */	stw r0, 0x34(r3)
/* 80037274 000341B4  4E 80 00 20 */	blr 

.global __dt__15J3DVertexBufferFv
__dt__15J3DVertexBufferFv:
/* 80037278 000341B8  7C 08 02 A6 */	mflr r0
/* 8003727C 000341BC  90 01 00 04 */	stw r0, 4(r1)
/* 80037280 000341C0  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80037284 000341C4  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80037288 000341C8  7C 7F 1B 79 */	or. r31, r3, r3
/* 8003728C 000341CC  41 82 00 20 */	beq lbl_800372AC
/* 80037290 000341D0  3C 60 80 3B */	lis r3, __vt__15J3DVertexBuffer@ha
/* 80037294 000341D4  38 63 9B D8 */	addi r3, r3, __vt__15J3DVertexBuffer@l
/* 80037298 000341D8  7C 80 07 35 */	extsh. r0, r4
/* 8003729C 000341DC  90 7F 00 38 */	stw r3, 0x38(r31)
/* 800372A0 000341E0  40 81 00 0C */	ble lbl_800372AC
/* 800372A4 000341E4  7F E3 FB 78 */	mr r3, r31
/* 800372A8 000341E8  4B FD 58 09 */	bl __dl__FPv
lbl_800372AC:
/* 800372AC 000341EC  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 800372B0 000341F0  7F E3 FB 78 */	mr r3, r31
/* 800372B4 000341F4  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 800372B8 000341F8  38 21 00 18 */	addi r1, r1, 0x18
/* 800372BC 000341FC  7C 08 03 A6 */	mtlr r0
/* 800372C0 00034200  4E 80 00 20 */	blr 

.global copyTransformedVtxArray__15J3DVertexBufferFv
copyTransformedVtxArray__15J3DVertexBufferFv:
/* 800372C4 00034204  7C 08 02 A6 */	mflr r0
/* 800372C8 00034208  90 01 00 04 */	stw r0, 4(r1)
/* 800372CC 0003420C  94 21 FF D0 */	stwu r1, -0x30(r1)
/* 800372D0 00034210  93 E1 00 2C */	stw r31, 0x2c(r1)
/* 800372D4 00034214  93 C1 00 28 */	stw r30, 0x28(r1)
/* 800372D8 00034218  93 A1 00 24 */	stw r29, 0x24(r1)
/* 800372DC 0003421C  7C 7D 1B 78 */	mr r29, r3
/* 800372E0 00034220  80 03 00 1C */	lwz r0, 0x1c(r3)
/* 800372E4 00034224  28 00 00 00 */	cmplwi r0, 0
/* 800372E8 00034228  41 82 00 10 */	beq lbl_800372F8
/* 800372EC 0003422C  80 1D 00 20 */	lwz r0, 0x20(r29)
/* 800372F0 00034230  28 00 00 00 */	cmplwi r0, 0
/* 800372F4 00034234  40 82 00 84 */	bne lbl_80037378
lbl_800372F8:
/* 800372F8 00034238  3B C0 00 00 */	li r30, 0
/* 800372FC 0003423C  3B E0 00 00 */	li r31, 0
lbl_80037300:
/* 80037300 00034240  2C 1E 00 00 */	cmpwi r30, 0
/* 80037304 00034244  41 82 00 14 */	beq lbl_80037318
/* 80037308 00034248  38 1F 00 1C */	addi r0, r31, 0x1c
/* 8003730C 0003424C  7C 1D 00 2E */	lwzx r0, r29, r0
/* 80037310 00034250  28 00 00 00 */	cmplwi r0, 0
/* 80037314 00034254  40 82 00 20 */	bne lbl_80037334
lbl_80037318:
/* 80037318 00034258  80 7D 00 00 */	lwz r3, 0(r29)
/* 8003731C 0003425C  38 80 00 20 */	li r4, 0x20
/* 80037320 00034260  80 03 00 00 */	lwz r0, 0(r3)
/* 80037324 00034264  1C 60 00 0C */	mulli r3, r0, 0xc
/* 80037328 00034268  4B FD 56 D1 */	bl __nwa__FUli
/* 8003732C 0003426C  38 1F 00 1C */	addi r0, r31, 0x1c
/* 80037330 00034270  7C 7D 01 2E */	stwx r3, r29, r0
lbl_80037334:
/* 80037334 00034274  2C 1E 00 00 */	cmpwi r30, 0
/* 80037338 00034278  41 82 00 14 */	beq lbl_8003734C
/* 8003733C 0003427C  38 1F 00 24 */	addi r0, r31, 0x24
/* 80037340 00034280  7C 1D 00 2E */	lwzx r0, r29, r0
/* 80037344 00034284  28 00 00 00 */	cmplwi r0, 0
/* 80037348 00034288  40 82 00 20 */	bne lbl_80037368
lbl_8003734C:
/* 8003734C 0003428C  80 7D 00 00 */	lwz r3, 0(r29)
/* 80037350 00034290  38 80 00 20 */	li r4, 0x20
/* 80037354 00034294  80 03 00 04 */	lwz r0, 4(r3)
/* 80037358 00034298  1C 60 00 0C */	mulli r3, r0, 0xc
/* 8003735C 0003429C  4B FD 56 9D */	bl __nwa__FUli
/* 80037360 000342A0  38 1F 00 24 */	addi r0, r31, 0x24
/* 80037364 000342A4  7C 7D 01 2E */	stwx r3, r29, r0
lbl_80037368:
/* 80037368 000342A8  3B DE 00 01 */	addi r30, r30, 1
/* 8003736C 000342AC  2C 1E 00 02 */	cmpwi r30, 2
/* 80037370 000342B0  3B FF 00 04 */	addi r31, r31, 4
/* 80037374 000342B4  41 80 FF 8C */	blt lbl_80037300
lbl_80037378:
/* 80037378 000342B8  80 01 00 34 */	lwz r0, 0x34(r1)
/* 8003737C 000342BC  83 E1 00 2C */	lwz r31, 0x2c(r1)
/* 80037380 000342C0  83 C1 00 28 */	lwz r30, 0x28(r1)
/* 80037384 000342C4  7C 08 03 A6 */	mtlr r0
/* 80037388 000342C8  83 A1 00 24 */	lwz r29, 0x24(r1)
/* 8003738C 000342CC  38 21 00 30 */	addi r1, r1, 0x30
/* 80037390 000342D0  4E 80 00 20 */	blr 

.global __ct__14J3DDrawMtxDataFv
__ct__14J3DDrawMtxDataFv:
/* 80037394 000342D4  38 00 00 00 */	li r0, 0
/* 80037398 000342D8  B0 03 00 00 */	sth r0, 0(r3)
/* 8003739C 000342DC  90 03 00 04 */	stw r0, 4(r3)
/* 800373A0 000342E0  90 03 00 08 */	stw r0, 8(r3)
/* 800373A4 000342E4  4E 80 00 20 */	blr 

.global __dt__14J3DDrawMtxDataFv
__dt__14J3DDrawMtxDataFv:
/* 800373A8 000342E8  7C 08 02 A6 */	mflr r0
/* 800373AC 000342EC  90 01 00 04 */	stw r0, 4(r1)
/* 800373B0 000342F0  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 800373B4 000342F4  93 E1 00 14 */	stw r31, 0x14(r1)
/* 800373B8 000342F8  7C 7F 1B 79 */	or. r31, r3, r3
/* 800373BC 000342FC  41 82 00 14 */	beq lbl_800373D0
/* 800373C0 00034300  7C 80 07 35 */	extsh. r0, r4
/* 800373C4 00034304  40 81 00 0C */	ble lbl_800373D0
/* 800373C8 00034308  7F E3 FB 78 */	mr r3, r31
/* 800373CC 0003430C  4B FD 56 E5 */	bl __dl__FPv
lbl_800373D0:
/* 800373D0 00034310  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 800373D4 00034314  7F E3 FB 78 */	mr r3, r31
/* 800373D8 00034318  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 800373DC 0003431C  38 21 00 18 */	addi r1, r1, 0x18
/* 800373E0 00034320  7C 08 03 A6 */	mtlr r0
/* 800373E4 00034324  4E 80 00 20 */	blr 

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__15J3DVertexBuffer
__vt__15J3DVertexBuffer:
  .4byte 0
  .4byte 0
  .4byte __dt__15J3DVertexBufferFv
  .4byte 0

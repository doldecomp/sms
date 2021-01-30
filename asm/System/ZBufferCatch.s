.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __dt__11TAlphaCatchFv
__dt__11TAlphaCatchFv:
/* 800F9238 000F6178  7C 08 02 A6 */	mflr r0
/* 800F923C 000F617C  90 01 00 04 */	stw r0, 4(r1)
/* 800F9240 000F6180  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 800F9244 000F6184  93 E1 00 14 */	stw r31, 0x14(r1)
/* 800F9248 000F6188  3B E4 00 00 */	addi r31, r4, 0
/* 800F924C 000F618C  93 C1 00 10 */	stw r30, 0x10(r1)
/* 800F9250 000F6190  7C 7E 1B 79 */	or. r30, r3, r3
/* 800F9254 000F6194  41 82 00 3C */	beq lbl_800F9290
/* 800F9258 000F6198  3C 60 80 3B */	lis r3, __vt__11TAlphaCatch@ha
/* 800F925C 000F619C  38 03 3F F8 */	addi r0, r3, __vt__11TAlphaCatch@l
/* 800F9260 000F61A0  90 1E 00 00 */	stw r0, 0(r30)
/* 800F9264 000F61A4  41 82 00 1C */	beq lbl_800F9280
/* 800F9268 000F61A8  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TViewObj@ha
/* 800F926C 000F61AC  38 03 A0 C0 */	addi r0, r3, __vt__Q26JDrama8TViewObj@l
/* 800F9270 000F61B0  90 1E 00 00 */	stw r0, 0(r30)
/* 800F9274 000F61B4  38 7E 00 00 */	addi r3, r30, 0
/* 800F9278 000F61B8  38 80 00 00 */	li r4, 0
/* 800F927C 000F61BC  4B F4 B5 81 */	bl __dt__Q26JDrama8TNameRefFv
lbl_800F9280:
/* 800F9280 000F61C0  7F E0 07 35 */	extsh. r0, r31
/* 800F9284 000F61C4  40 81 00 0C */	ble lbl_800F9290
/* 800F9288 000F61C8  7F C3 F3 78 */	mr r3, r30
/* 800F928C 000F61CC  4B F1 38 25 */	bl __dl__FPv
lbl_800F9290:
/* 800F9290 000F61D0  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 800F9294 000F61D4  7F C3 F3 78 */	mr r3, r30
/* 800F9298 000F61D8  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 800F929C 000F61DC  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 800F92A0 000F61E0  7C 08 03 A6 */	mtlr r0
/* 800F92A4 000F61E4  38 21 00 18 */	addi r1, r1, 0x18
/* 800F92A8 000F61E8  4E 80 00 20 */	blr 

.global perform__11TAlphaCatchFUlPQ26JDrama9TGraphics
perform__11TAlphaCatchFUlPQ26JDrama9TGraphics:
/* 800F92AC 000F61EC  7C 08 02 A6 */	mflr r0
/* 800F92B0 000F61F0  90 01 00 04 */	stw r0, 4(r1)
/* 800F92B4 000F61F4  54 80 07 39 */	rlwinm. r0, r4, 0, 0x1c, 0x1c
/* 800F92B8 000F61F8  94 21 FF F8 */	stwu r1, -8(r1)
/* 800F92BC 000F61FC  41 82 00 24 */	beq lbl_800F92E0
/* 800F92C0 000F6200  80 6D 98 40 */	lwz r3, smInstance__16TDrawSyncManager-_SDA_BASE_(r13)
/* 800F92C4 000F6204  48 00 37 C9 */	bl pushBreakPoint__16TDrawSyncManagerFv
/* 800F92C8 000F6208  38 60 00 7C */	li r3, 0x7c
/* 800F92CC 000F620C  4B FA EF A1 */	bl GXSetDrawSync
/* 800F92D0 000F6210  80 6D 98 40 */	lwz r3, smInstance__16TDrawSyncManager-_SDA_BASE_(r13)
/* 800F92D4 000F6214  48 00 37 B9 */	bl pushBreakPoint__16TDrawSyncManagerFv
/* 800F92D8 000F6218  38 60 00 00 */	li r3, 0
/* 800F92DC 000F621C  4B FA EF 91 */	bl GXSetDrawSync
lbl_800F92E0:
/* 800F92E0 000F6220  80 01 00 0C */	lwz r0, 0xc(r1)
/* 800F92E4 000F6224  38 21 00 08 */	addi r1, r1, 8
/* 800F92E8 000F6228  7C 08 03 A6 */	mtlr r0
/* 800F92EC 000F622C  4E 80 00 20 */	blr 

.global perform__13TZBufferCatchFUlPQ26JDrama9TGraphics
perform__13TZBufferCatchFUlPQ26JDrama9TGraphics:
/* 800F92F0 000F6230  7C 08 02 A6 */	mflr r0
/* 800F92F4 000F6234  90 01 00 04 */	stw r0, 4(r1)
/* 800F92F8 000F6238  54 80 07 39 */	rlwinm. r0, r4, 0, 0x1c, 0x1c
/* 800F92FC 000F623C  94 21 FF F8 */	stwu r1, -8(r1)
/* 800F9300 000F6240  41 82 00 18 */	beq lbl_800F9318
/* 800F9304 000F6244  80 6D 98 40 */	lwz r3, smInstance__16TDrawSyncManager-_SDA_BASE_(r13)
/* 800F9308 000F6248  48 00 37 85 */	bl pushBreakPoint__16TDrawSyncManagerFv
/* 800F930C 000F624C  4B FD 56 3D */	bl ReInitializeGX
/* 800F9310 000F6250  38 60 00 7D */	li r3, 0x7d
/* 800F9314 000F6254  4B FA EF 59 */	bl GXSetDrawSync
lbl_800F9318:
/* 800F9318 000F6258  80 01 00 0C */	lwz r0, 0xc(r1)
/* 800F931C 000F625C  38 21 00 08 */	addi r1, r1, 8
/* 800F9320 000F6260  7C 08 03 A6 */	mtlr r0
/* 800F9324 000F6264  4E 80 00 20 */	blr 

.global __dt__13TZBufferCatchFv
__dt__13TZBufferCatchFv:
/* 800F9328 000F6268  7C 08 02 A6 */	mflr r0
/* 800F932C 000F626C  90 01 00 04 */	stw r0, 4(r1)
/* 800F9330 000F6270  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 800F9334 000F6274  93 E1 00 14 */	stw r31, 0x14(r1)
/* 800F9338 000F6278  3B E4 00 00 */	addi r31, r4, 0
/* 800F933C 000F627C  93 C1 00 10 */	stw r30, 0x10(r1)
/* 800F9340 000F6280  7C 7E 1B 79 */	or. r30, r3, r3
/* 800F9344 000F6284  41 82 00 3C */	beq lbl_800F9380
/* 800F9348 000F6288  3C 60 80 3B */	lis r3, __vt__13TZBufferCatch@ha
/* 800F934C 000F628C  38 03 40 1C */	addi r0, r3, __vt__13TZBufferCatch@l
/* 800F9350 000F6290  90 1E 00 00 */	stw r0, 0(r30)
/* 800F9354 000F6294  41 82 00 1C */	beq lbl_800F9370
/* 800F9358 000F6298  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TViewObj@ha
/* 800F935C 000F629C  38 03 A0 C0 */	addi r0, r3, __vt__Q26JDrama8TViewObj@l
/* 800F9360 000F62A0  90 1E 00 00 */	stw r0, 0(r30)
/* 800F9364 000F62A4  38 7E 00 00 */	addi r3, r30, 0
/* 800F9368 000F62A8  38 80 00 00 */	li r4, 0
/* 800F936C 000F62AC  4B F4 B4 91 */	bl __dt__Q26JDrama8TNameRefFv
lbl_800F9370:
/* 800F9370 000F62B0  7F E0 07 35 */	extsh. r0, r31
/* 800F9374 000F62B4  40 81 00 0C */	ble lbl_800F9380
/* 800F9378 000F62B8  7F C3 F3 78 */	mr r3, r30
/* 800F937C 000F62BC  4B F1 37 35 */	bl __dl__FPv
lbl_800F9380:
/* 800F9380 000F62C0  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 800F9384 000F62C4  7F C3 F3 78 */	mr r3, r30
/* 800F9388 000F62C8  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 800F938C 000F62CC  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 800F9390 000F62D0  7C 08 03 A6 */	mtlr r0
/* 800F9394 000F62D4  38 21 00 18 */	addi r1, r1, 0x18
/* 800F9398 000F62D8  4E 80 00 20 */	blr 

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__11TAlphaCatch
__vt__11TAlphaCatch:
	.incbin "baserom.dol", 0x3B0FF8, 0x24
.global __vt__13TZBufferCatch
__vt__13TZBufferCatch:
	.incbin "baserom.dol", 0x3B101C, 0x24

.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0

.global DBInit
DBInit:
/* 8008C43C 0008937C  3C 80 80 00 */	lis r4, 0x80000040@ha
/* 8008C440 00089380  38 04 00 40 */	addi r0, r4, 0x80000040@l
/* 8008C444 00089384  3C 60 80 09 */	lis r3, __DBExceptionDestination@ha
/* 8008C448 00089388  90 0D 92 B8 */	stw r0, __DBInterface-_SDA_BASE_(r13)
/* 8008C44C 0008938C  38 63 C4 AC */	addi r3, r3, __DBExceptionDestination@l
/* 8008C450 00089390  3C 03 80 00 */	addis r0, r3, 0x8000
/* 8008C454 00089394  90 04 00 48 */	stw r0, 0x48(r4)
/* 8008C458 00089398  38 00 00 01 */	li r0, 1
/* 8008C45C 0008939C  90 0D 92 BC */	stw r0, DBVerbose-_SDA_BASE_(r13)
/* 8008C460 000893A0  4E 80 00 20 */	blr 

.global __DBExceptionDestinationAux
__DBExceptionDestinationAux:
/* 8008C464 000893A4  7C 08 02 A6 */	mflr r0
/* 8008C468 000893A8  3C 60 80 3B */	lis r3, $$27@ha
/* 8008C46C 000893AC  90 01 00 04 */	stw r0, 4(r1)
/* 8008C470 000893B0  38 63 F0 A8 */	addi r3, r3, $$27@l
/* 8008C474 000893B4  4C C6 31 82 */	crclr 6
/* 8008C478 000893B8  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8008C47C 000893BC  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8008C480 000893C0  80 80 00 C0 */	lwz r4, 0xc0(0)
/* 8008C484 000893C4  3F E4 80 00 */	addis r31, r4, 0x8000
/* 8008C488 000893C8  48 00 2A DD */	bl OSReport
/* 8008C48C 000893CC  7F E3 FB 78 */	mr r3, r31
/* 8008C490 000893D0  48 00 26 35 */	bl OSDumpContext
/* 8008C494 000893D4  4B FF FF 4D */	bl PPCHalt
/* 8008C498 000893D8  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8008C49C 000893DC  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8008C4A0 000893E0  38 21 00 18 */	addi r1, r1, 0x18
/* 8008C4A4 000893E4  7C 08 03 A6 */	mtlr r0
/* 8008C4A8 000893E8  4E 80 00 20 */	blr 

.global __DBExceptionDestination
__DBExceptionDestination:
/* 8008C4AC 000893EC  7C 60 00 A6 */	mfmsr r3
/* 8008C4B0 000893F0  60 63 00 30 */	ori r3, r3, 0x30
/* 8008C4B4 000893F4  7C 60 01 24 */	mtmsr r3
/* 8008C4B8 000893F8  4B FF FF AC */	b __DBExceptionDestinationAux

.global __DBIsExceptionMarked
__DBIsExceptionMarked:
/* 8008C4BC 000893FC  80 8D 92 B8 */	lwz r4, __DBInterface-_SDA_BASE_(r13)
/* 8008C4C0 00089400  54 60 06 3E */	clrlwi r0, r3, 0x18
/* 8008C4C4 00089404  38 60 00 01 */	li r3, 1
/* 8008C4C8 00089408  80 84 00 04 */	lwz r4, 4(r4)
/* 8008C4CC 0008940C  7C 60 00 30 */	slw r0, r3, r0
/* 8008C4D0 00089410  7C 83 00 38 */	and r3, r4, r0
/* 8008C4D4 00089414  4E 80 00 20 */	blr 

.global DBPrintf
DBPrintf:
/* 8008C4D8 00089418  94 21 FF 90 */	stwu r1, -0x70(r1)
/* 8008C4DC 0008941C  40 86 00 24 */	bne cr1, lbl_8008C500
/* 8008C4E0 00089420  D8 21 00 28 */	stfd f1, 0x28(r1)
/* 8008C4E4 00089424  D8 41 00 30 */	stfd f2, 0x30(r1)
/* 8008C4E8 00089428  D8 61 00 38 */	stfd f3, 0x38(r1)
/* 8008C4EC 0008942C  D8 81 00 40 */	stfd f4, 0x40(r1)
/* 8008C4F0 00089430  D8 A1 00 48 */	stfd f5, 0x48(r1)
/* 8008C4F4 00089434  D8 C1 00 50 */	stfd f6, 0x50(r1)
/* 8008C4F8 00089438  D8 E1 00 58 */	stfd f7, 0x58(r1)
/* 8008C4FC 0008943C  D9 01 00 60 */	stfd f8, 0x60(r1)
lbl_8008C500:
/* 8008C500 00089440  90 61 00 08 */	stw r3, 8(r1)
/* 8008C504 00089444  90 81 00 0C */	stw r4, 0xc(r1)
/* 8008C508 00089448  90 A1 00 10 */	stw r5, 0x10(r1)
/* 8008C50C 0008944C  90 C1 00 14 */	stw r6, 0x14(r1)
/* 8008C510 00089450  90 E1 00 18 */	stw r7, 0x18(r1)
/* 8008C514 00089454  91 01 00 1C */	stw r8, 0x1c(r1)
/* 8008C518 00089458  91 21 00 20 */	stw r9, 0x20(r1)
/* 8008C51C 0008945C  91 41 00 24 */	stw r10, 0x24(r1)
/* 8008C520 00089460  38 21 00 70 */	addi r1, r1, 0x70
/* 8008C524 00089464  4E 80 00 20 */	blr 

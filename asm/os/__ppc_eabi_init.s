.include "macros.inc"

.section .init, "ax"  # 0x80003100 - 0x80005540
.global __init_hardware
__init_hardware:
/* 80005440 00002440  7C 00 00 A6 */	mfmsr r0
/* 80005444 00002444  60 00 20 00 */	ori r0, r0, 0x2000
/* 80005448 00002448  7C 00 01 24 */	mtmsr r0
/* 8000544C 0000244C  7F E8 02 A6 */	mflr r31
/* 80005450 00002450  48 08 79 E9 */	bl __OSPSInit
/* 80005454 00002454  48 08 90 89 */	bl __OSCacheInit
/* 80005458 00002458  7F E8 03 A6 */	mtlr r31
/* 8000545C 0000245C  4E 80 00 20 */	blr 

.global __flush_cache
__flush_cache:
/* 80005460 00002460  3C A0 FF FF */	lis r5, 0xFFFFFFF1@h
/* 80005464 00002464  60 A5 FF F1 */	ori r5, r5, 0xFFFFFFF1@l
/* 80005468 00002468  7C A5 18 38 */	and r5, r5, r3
/* 8000546C 0000246C  7C 65 18 50 */	subf r3, r5, r3
/* 80005470 00002470  7C 84 1A 14 */	add r4, r4, r3
lbl_80005474:
/* 80005474 00002474  7C 00 28 6C */	dcbst 0, r5
/* 80005478 00002478  7C 00 04 AC */	sync 0
/* 8000547C 0000247C  7C 00 2F AC */	icbi 0, r5
/* 80005480 00002480  30 A5 00 08 */	addic r5, r5, 8
/* 80005484 00002484  34 84 FF F8 */	addic. r4, r4, -8
/* 80005488 00002488  40 80 FF EC */	bge lbl_80005474
/* 8000548C 0000248C  4C 00 01 2C */	isync 
/* 80005490 00002490  4E 80 00 20 */	blr 

.global lbl_80005494
lbl_80005494:
    .4byte _f_init
    .4byte _f_init
    .4byte _size_init

    .4byte _fextab_
    .4byte _fextab_
    .4byte _size_extab

    .4byte _fextabindex_
    .4byte _fextabindex_
    .4byte _size_extabindex

    .4byte _f_text
    .4byte _f_text
    .4byte _size_text

    .4byte _f_ctors
    .4byte _f_ctors
    .4byte _size_ctors

    .4byte _f_dtors
    .4byte _f_dtors
    .4byte _size_dtors

    .4byte _f_rodata
    .4byte _f_rodata
    .4byte _size_rodata

    .4byte _f_data
    .4byte _f_data
    .4byte _size_data

    .4byte _f_sdata
    .4byte _f_sdata
    .4byte _size_sdata

    .4byte _f_sdata2
    .4byte _f_sdata2
    .4byte _size_sdata2

    /* terminating? */
    .4byte 0
    .4byte 0
    .4byte 0

.balign 4
.global lbl_80005518
lbl_80005518:
    .4byte _f_bss
    .4byte _size_bss
    .4byte _f_sbss
    .4byte _size_sbss

    /* terminating? */
    .4byte 0
    .4byte 0

.balign 8
.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __init_user
__init_user:
/* 8009421C 0009115C  7C 08 02 A6 */	mflr r0
/* 80094220 00091160  90 01 00 04 */	stw r0, 4(r1)
/* 80094224 00091164  94 21 FF F8 */	stwu r1, -8(r1)
/* 80094228 00091168  48 00 00 15 */	bl __init_cpp
/* 8009422C 0009116C  80 01 00 0C */	lwz r0, 0xc(r1)
/* 80094230 00091170  38 21 00 08 */	addi r1, r1, 8
/* 80094234 00091174  7C 08 03 A6 */	mtlr r0
/* 80094238 00091178  4E 80 00 20 */	blr 

.global __init_cpp
__init_cpp:
/* 8009423C 0009117C  7C 08 02 A6 */	mflr r0
/* 80094240 00091180  90 01 00 04 */	stw r0, 4(r1)
/* 80094244 00091184  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 80094248 00091188  93 E1 00 0C */	stw r31, 0xc(r1)
/* 8009424C 0009118C  3C 60 80 37 */	lis r3, _ctors@ha
/* 80094250 00091190  38 03 FB A0 */	addi r0, r3, _ctors@l
/* 80094254 00091194  7C 1F 03 78 */	mr r31, r0
/* 80094258 00091198  48 00 00 04 */	b lbl_8009425C
lbl_8009425C:
/* 8009425C 0009119C  48 00 00 04 */	b lbl_80094260
lbl_80094260:
/* 80094260 000911A0  48 00 00 10 */	b lbl_80094270
lbl_80094264:
/* 80094264 000911A4  7D 88 03 A6 */	mtlr r12
/* 80094268 000911A8  4E 80 00 21 */	blrl 
/* 8009426C 000911AC  3B FF 00 04 */	addi r31, r31, 4
lbl_80094270:
/* 80094270 000911B0  81 9F 00 00 */	lwz r12, 0(r31)
/* 80094274 000911B4  28 0C 00 00 */	cmplwi r12, 0
/* 80094278 000911B8  40 82 FF EC */	bne lbl_80094264
/* 8009427C 000911BC  80 01 00 14 */	lwz r0, 0x14(r1)
/* 80094280 000911C0  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 80094284 000911C4  38 21 00 10 */	addi r1, r1, 0x10
/* 80094288 000911C8  7C 08 03 A6 */	mtlr r0
/* 8009428C 000911CC  4E 80 00 20 */	blr 

.global _ExitProcess
_ExitProcess:
/* 80094290 000911D0  7C 08 02 A6 */	mflr r0
/* 80094294 000911D4  90 01 00 04 */	stw r0, 4(r1)
/* 80094298 000911D8  94 21 FF F8 */	stwu r1, -8(r1)
/* 8009429C 000911DC  4B FF 81 45 */	bl PPCHalt
/* 800942A0 000911E0  80 01 00 0C */	lwz r0, 0xc(r1)
/* 800942A4 000911E4  38 21 00 08 */	addi r1, r1, 8
/* 800942A8 000911E8  7C 08 03 A6 */	mtlr r0
/* 800942AC 000911EC  4E 80 00 20 */	blr 

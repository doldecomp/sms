.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global startControl__9TMapEventFv
startControl__9TMapEventFv:
/* 801E0D68 001DDCA8  38 00 00 02 */	li r0, 2
/* 801E0D6C 001DDCAC  90 03 00 18 */	stw r0, 0x18(r3)
/* 801E0D70 001DDCB0  4E 80 00 20 */	blr 

.global getBuilding__9TMapEventCFi
getBuilding__9TMapEventCFi:
/* 801E0D74 001DDCB4  80 63 00 1C */	lwz r3, 0x1c(r3)
/* 801E0D78 001DDCB8  54 80 10 3A */	slwi r0, r4, 2
/* 801E0D7C 001DDCBC  80 63 00 14 */	lwz r3, 0x14(r3)
/* 801E0D80 001DDCC0  80 63 00 00 */	lwz r3, 0(r3)
/* 801E0D84 001DDCC4  80 63 00 14 */	lwz r3, 0x14(r3)
/* 801E0D88 001DDCC8  7C 63 00 2E */	lwzx r3, r3, r0
/* 801E0D8C 001DDCCC  4E 80 00 20 */	blr 

.global __ct__9TMapEventFPCc
__ct__9TMapEventFPCc:
/* 801E0D90 001DDCD0  7C 08 02 A6 */	mflr r0
/* 801E0D94 001DDCD4  90 01 00 04 */	stw r0, 4(r1)
/* 801E0D98 001DDCD8  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 801E0D9C 001DDCDC  93 E1 00 14 */	stw r31, 0x14(r1)
/* 801E0DA0 001DDCE0  7C 7F 1B 78 */	mr r31, r3
/* 801E0DA4 001DDCE4  4B EF BA 05 */	bl __ct__13TEventWatcherFPCc
/* 801E0DA8 001DDCE8  3C 60 80 3D */	lis r3, __vt__9TMapEvent@ha
/* 801E0DAC 001DDCEC  38 03 FD 40 */	addi r0, r3, __vt__9TMapEvent@l
/* 801E0DB0 001DDCF0  90 1F 00 00 */	stw r0, 0(r31)
/* 801E0DB4 001DDCF4  38 60 00 01 */	li r3, 1
/* 801E0DB8 001DDCF8  38 00 00 00 */	li r0, 0
/* 801E0DBC 001DDCFC  90 7F 00 18 */	stw r3, 0x18(r31)
/* 801E0DC0 001DDD00  7F E3 FB 78 */	mr r3, r31
/* 801E0DC4 001DDD04  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 801E0DC8 001DDD08  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 801E0DCC 001DDD0C  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 801E0DD0 001DDD10  38 21 00 18 */	addi r1, r1, 0x18
/* 801E0DD4 001DDD14  7C 08 03 A6 */	mtlr r0
/* 801E0DD8 001DDD18  4E 80 00 20 */	blr 

.global load__9TMapEventFR20JSUMemoryInputStream
load__9TMapEventFR20JSUMemoryInputStream:
/* 801E0DDC 001DDD1C  7C 08 02 A6 */	mflr r0
/* 801E0DE0 001DDD20  90 01 00 04 */	stw r0, 4(r1)
/* 801E0DE4 001DDD24  94 21 FF D0 */	stwu r1, -0x30(r1)
/* 801E0DE8 001DDD28  93 E1 00 2C */	stw r31, 0x2c(r1)
/* 801E0DEC 001DDD2C  7C 7F 1B 78 */	mr r31, r3
/* 801E0DF0 001DDD30  4B E6 3A 61 */	bl load__Q26JDrama8TNameRefFR20JSUMemoryInputStream
/* 801E0DF4 001DDD34  80 6D 9A B0 */	lwz r3, gpMap@sda21(r13)
/* 801E0DF8 001DDD38  80 63 00 14 */	lwz r3, 0x14(r3)
/* 801E0DFC 001DDD3C  80 63 00 14 */	lwz r3, 0x14(r3)
/* 801E0E00 001DDD40  80 03 00 00 */	lwz r0, 0(r3)
/* 801E0E04 001DDD44  90 1F 00 1C */	stw r0, 0x1c(r31)
/* 801E0E08 001DDD48  80 01 00 34 */	lwz r0, 0x34(r1)
/* 801E0E0C 001DDD4C  83 E1 00 2C */	lwz r31, 0x2c(r1)
/* 801E0E10 001DDD50  38 21 00 30 */	addi r1, r1, 0x30
/* 801E0E14 001DDD54  7C 08 03 A6 */	mtlr r0
/* 801E0E18 001DDD58  4E 80 00 20 */	blr 

.global perform__9TMapEventFUlPQ26JDrama9TGraphics
perform__9TMapEventFUlPQ26JDrama9TGraphics:
/* 801E0E1C 001DDD5C  7C 08 02 A6 */	mflr r0
/* 801E0E20 001DDD60  90 01 00 04 */	stw r0, 4(r1)
/* 801E0E24 001DDD64  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 801E0E28 001DDD68  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 801E0E2C 001DDD6C  7C 7F 1B 78 */	mr r31, r3
/* 801E0E30 001DDD70  80 63 00 18 */	lwz r3, 0x18(r3)
/* 801E0E34 001DDD74  28 03 00 00 */	cmplwi r3, 0
/* 801E0E38 001DDD78  40 82 00 0C */	bne lbl_801E0E44
/* 801E0E3C 001DDD7C  38 00 00 01 */	li r0, 1
/* 801E0E40 001DDD80  48 00 00 08 */	b lbl_801E0E48
lbl_801E0E44:
/* 801E0E44 001DDD84  38 00 00 00 */	li r0, 0
lbl_801E0E48:
/* 801E0E48 001DDD88  54 00 06 3F */	clrlwi. r0, r0, 0x18
/* 801E0E4C 001DDD8C  40 82 00 A8 */	bne lbl_801E0EF4
/* 801E0E50 001DDD90  54 80 07 FF */	clrlwi. r0, r4, 0x1f
/* 801E0E54 001DDD94  41 82 00 A0 */	beq lbl_801E0EF4
/* 801E0E58 001DDD98  28 03 00 01 */	cmplwi r3, 1
/* 801E0E5C 001DDD9C  40 82 00 0C */	bne lbl_801E0E68
/* 801E0E60 001DDDA0  38 00 00 01 */	li r0, 1
/* 801E0E64 001DDDA4  48 00 00 08 */	b lbl_801E0E6C
lbl_801E0E68:
/* 801E0E68 001DDDA8  38 00 00 00 */	li r0, 0
lbl_801E0E6C:
/* 801E0E6C 001DDDAC  54 00 06 3F */	clrlwi. r0, r0, 0x18
/* 801E0E70 001DDDB0  41 82 00 34 */	beq lbl_801E0EA4
/* 801E0E74 001DDDB4  7F E3 FB 78 */	mr r3, r31
/* 801E0E78 001DDDB8  81 9F 00 00 */	lwz r12, 0(r31)
/* 801E0E7C 001DDDBC  81 8C 00 2C */	lwz r12, 0x2c(r12)
/* 801E0E80 001DDDC0  7D 88 03 A6 */	mtlr r12
/* 801E0E84 001DDDC4  4E 80 00 21 */	blrl 
/* 801E0E88 001DDDC8  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 801E0E8C 001DDDCC  41 82 00 18 */	beq lbl_801E0EA4
/* 801E0E90 001DDDD0  7F E3 FB 78 */	mr r3, r31
/* 801E0E94 001DDDD4  81 9F 00 00 */	lwz r12, 0(r31)
/* 801E0E98 001DDDD8  81 8C 00 30 */	lwz r12, 0x30(r12)
/* 801E0E9C 001DDDDC  7D 88 03 A6 */	mtlr r12
/* 801E0EA0 001DDDE0  4E 80 00 21 */	blrl 
lbl_801E0EA4:
/* 801E0EA4 001DDDE4  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 801E0EA8 001DDDE8  28 00 00 02 */	cmplwi r0, 2
/* 801E0EAC 001DDDEC  40 82 00 0C */	bne lbl_801E0EB8
/* 801E0EB0 001DDDF0  38 00 00 01 */	li r0, 1
/* 801E0EB4 001DDDF4  48 00 00 08 */	b lbl_801E0EBC
lbl_801E0EB8:
/* 801E0EB8 001DDDF8  38 00 00 00 */	li r0, 0
lbl_801E0EBC:
/* 801E0EBC 001DDDFC  54 00 06 3F */	clrlwi. r0, r0, 0x18
/* 801E0EC0 001DDE00  41 82 00 34 */	beq lbl_801E0EF4
/* 801E0EC4 001DDE04  7F E3 FB 78 */	mr r3, r31
/* 801E0EC8 001DDE08  81 9F 00 00 */	lwz r12, 0(r31)
/* 801E0ECC 001DDE0C  81 8C 00 34 */	lwz r12, 0x34(r12)
/* 801E0ED0 001DDE10  7D 88 03 A6 */	mtlr r12
/* 801E0ED4 001DDE14  4E 80 00 21 */	blrl 
/* 801E0ED8 001DDE18  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 801E0EDC 001DDE1C  41 82 00 18 */	beq lbl_801E0EF4
/* 801E0EE0 001DDE20  7F E3 FB 78 */	mr r3, r31
/* 801E0EE4 001DDE24  81 9F 00 00 */	lwz r12, 0(r31)
/* 801E0EE8 001DDE28  81 8C 00 38 */	lwz r12, 0x38(r12)
/* 801E0EEC 001DDE2C  7D 88 03 A6 */	mtlr r12
/* 801E0EF0 001DDE30  4E 80 00 21 */	blrl 
lbl_801E0EF4:
/* 801E0EF4 001DDE34  80 01 00 24 */	lwz r0, 0x24(r1)
/* 801E0EF8 001DDE38  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 801E0EFC 001DDE3C  38 21 00 20 */	addi r1, r1, 0x20
/* 801E0F00 001DDE40  7C 08 03 A6 */	mtlr r0
/* 801E0F04 001DDE44  4E 80 00 20 */	blr 

.global finishControl__9TMapEventFv
finishControl__9TMapEventFv:
/* 801E0F08 001DDE48  7C 08 02 A6 */	mflr r0
/* 801E0F0C 001DDE4C  90 01 00 04 */	stw r0, 4(r1)
/* 801E0F10 001DDE50  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 801E0F14 001DDE54  93 E1 00 14 */	stw r31, 0x14(r1)
/* 801E0F18 001DDE58  7C 7F 1B 78 */	mr r31, r3
/* 801E0F1C 001DDE5C  81 9F 00 00 */	lwz r12, 0(r31)
/* 801E0F20 001DDE60  81 8C 00 28 */	lwz r12, 0x28(r12)
/* 801E0F24 001DDE64  7D 88 03 A6 */	mtlr r12
/* 801E0F28 001DDE68  4E 80 00 21 */	blrl 
/* 801E0F2C 001DDE6C  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 801E0F30 001DDE70  41 82 00 10 */	beq lbl_801E0F40
/* 801E0F34 001DDE74  38 00 00 00 */	li r0, 0
/* 801E0F38 001DDE78  90 1F 00 18 */	stw r0, 0x18(r31)
/* 801E0F3C 001DDE7C  48 00 00 0C */	b lbl_801E0F48
lbl_801E0F40:
/* 801E0F40 001DDE80  38 00 00 01 */	li r0, 1
/* 801E0F44 001DDE84  90 1F 00 18 */	stw r0, 0x18(r31)
lbl_801E0F48:
/* 801E0F48 001DDE88  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 801E0F4C 001DDE8C  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 801E0F50 001DDE90  38 21 00 18 */	addi r1, r1, 0x18
/* 801E0F54 001DDE94  7C 08 03 A6 */	mtlr r0
/* 801E0F58 001DDE98  4E 80 00 20 */	blr 

.global isFinishedAll__9TMapEventCFv
isFinishedAll__9TMapEventCFv:
/* 801E0F5C 001DDE9C  80 03 00 18 */	lwz r0, 0x18(r3)
/* 801E0F60 001DDEA0  28 00 00 00 */	cmplwi r0, 0
/* 801E0F64 001DDEA4  40 82 00 0C */	bne lbl_801E0F70
/* 801E0F68 001DDEA8  38 00 00 01 */	li r0, 1
/* 801E0F6C 001DDEAC  48 00 00 08 */	b lbl_801E0F74
lbl_801E0F70:
/* 801E0F70 001DDEB0  38 00 00 00 */	li r0, 0
lbl_801E0F74:
/* 801E0F74 001DDEB4  54 00 06 3F */	clrlwi. r0, r0, 0x18
/* 801E0F78 001DDEB8  41 82 00 0C */	beq lbl_801E0F84
/* 801E0F7C 001DDEBC  38 60 00 01 */	li r3, 1
/* 801E0F80 001DDEC0  4E 80 00 20 */	blr 
lbl_801E0F84:
/* 801E0F84 001DDEC4  38 60 00 00 */	li r3, 0
/* 801E0F88 001DDEC8  4E 80 00 20 */	blr 

.global __sinit_MapEvent_cpp
__sinit_MapEvent_cpp:
/* 801E0F8C 001DDECC  7C 08 02 A6 */	mflr r0
/* 801E0F90 001DDED0  3C 60 80 3F */	lis r3, $$22700@ha
/* 801E0F94 001DDED4  90 01 00 04 */	stw r0, 4(r1)
/* 801E0F98 001DDED8  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 801E0F9C 001DDEDC  93 E1 00 0C */	stw r31, 0xc(r1)
/* 801E0FA0 001DDEE0  3B E3 7A C8 */	addi r31, r3, $$22700@l
/* 801E0FA4 001DDEE4  88 0D 97 CC */	lbz r0, __init__smList__15JALList$$05MSBgm$$1@sda21(r13)
/* 801E0FA8 001DDEE8  7C 00 07 75 */	extsb. r0, r0
/* 801E0FAC 001DDEEC  40 82 00 28 */	bne lbl_801E0FD4
/* 801E0FB0 001DDEF0  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1@sda21
/* 801E0FB4 001DDEF4  4B E2 DC 81 */	bl initiate__10JSUPtrListFv
/* 801E0FB8 001DDEF8  3C 60 80 0E */	lis r3, __dt__15JSUList$$05MSBgm$$1Fv@ha
/* 801E0FBC 001DDEFC  38 83 6A 44 */	addi r4, r3, __dt__15JSUList$$05MSBgm$$1Fv@l
/* 801E0FC0 001DDF00  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1@sda21
/* 801E0FC4 001DDF04  38 BF 00 00 */	addi r5, r31, 0
/* 801E0FC8 001DDF08  4B EA 17 61 */	bl __register_global_object
/* 801E0FCC 001DDF0C  38 00 00 01 */	li r0, 1
/* 801E0FD0 001DDF10  98 0D 97 CC */	stb r0, __init__smList__15JALList$$05MSBgm$$1@sda21(r13)
lbl_801E0FD4:
/* 801E0FD4 001DDF14  88 0D 97 CD */	lbz r0, __init__smList__24JALList$$013MSSetSoundGrp$$1@sda21(r13)
/* 801E0FD8 001DDF18  7C 00 07 75 */	extsb. r0, r0
/* 801E0FDC 001DDF1C  40 82 00 28 */	bne lbl_801E1004
/* 801E0FE0 001DDF20  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1@sda21
/* 801E0FE4 001DDF24  4B E2 DC 51 */	bl initiate__10JSUPtrListFv
/* 801E0FE8 001DDF28  3C 60 80 0E */	lis r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@ha
/* 801E0FEC 001DDF2C  38 83 69 EC */	addi r4, r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@l
/* 801E0FF0 001DDF30  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1@sda21
/* 801E0FF4 001DDF34  38 BF 00 0C */	addi r5, r31, 0xc
/* 801E0FF8 001DDF38  4B EA 17 31 */	bl __register_global_object
/* 801E0FFC 001DDF3C  38 00 00 01 */	li r0, 1
/* 801E1000 001DDF40  98 0D 97 CD */	stb r0, __init__smList__24JALList$$013MSSetSoundGrp$$1@sda21(r13)
lbl_801E1004:
/* 801E1004 001DDF44  88 0D 97 CE */	lbz r0, __init__smList__21JALList$$010MSSetSound$$1@sda21(r13)
/* 801E1008 001DDF48  7C 00 07 75 */	extsb. r0, r0
/* 801E100C 001DDF4C  40 82 00 28 */	bne lbl_801E1034
/* 801E1010 001DDF50  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1@sda21
/* 801E1014 001DDF54  4B E2 DC 21 */	bl initiate__10JSUPtrListFv
/* 801E1018 001DDF58  3C 60 80 0E */	lis r3, __dt__21JSUList$$010MSSetSound$$1Fv@ha
/* 801E101C 001DDF5C  38 83 69 94 */	addi r4, r3, __dt__21JSUList$$010MSSetSound$$1Fv@l
/* 801E1020 001DDF60  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1@sda21
/* 801E1024 001DDF64  38 BF 00 18 */	addi r5, r31, 0x18
/* 801E1028 001DDF68  4B EA 17 01 */	bl __register_global_object
/* 801E102C 001DDF6C  38 00 00 01 */	li r0, 1
/* 801E1030 001DDF70  98 0D 97 CE */	stb r0, __init__smList__21JALList$$010MSSetSound$$1@sda21(r13)
lbl_801E1034:
/* 801E1034 001DDF74  88 0D 8F 8C */	lbz r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1@sda21(r13)
/* 801E1038 001DDF78  7C 00 07 75 */	extsb. r0, r0
/* 801E103C 001DDF7C  40 82 00 28 */	bne lbl_801E1064
/* 801E1040 001DDF80  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1@sda21
/* 801E1044 001DDF84  4B E2 DB F1 */	bl initiate__10JSUPtrListFv
/* 801E1048 001DDF88  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@ha
/* 801E104C 001DDF8C  38 83 A8 0C */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@l
/* 801E1050 001DDF90  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1@sda21
/* 801E1054 001DDF94  38 BF 00 24 */	addi r5, r31, 0x24
/* 801E1058 001DDF98  4B EA 16 D1 */	bl __register_global_object
/* 801E105C 001DDF9C  38 00 00 01 */	li r0, 1
/* 801E1060 001DDFA0  98 0D 8F 8C */	stb r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1@sda21(r13)
lbl_801E1064:
/* 801E1064 001DDFA4  88 0D 8F 8D */	lbz r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1@sda21(r13)
/* 801E1068 001DDFA8  7C 00 07 75 */	extsb. r0, r0
/* 801E106C 001DDFAC  40 82 00 28 */	bne lbl_801E1094
/* 801E1070 001DDFB0  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1@sda21
/* 801E1074 001DDFB4  4B E2 DB C1 */	bl initiate__10JSUPtrListFv
/* 801E1078 001DDFB8  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@ha
/* 801E107C 001DDFBC  38 83 A7 B4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@l
/* 801E1080 001DDFC0  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1@sda21
/* 801E1084 001DDFC4  38 BF 00 30 */	addi r5, r31, 0x30
/* 801E1088 001DDFC8  4B EA 16 A1 */	bl __register_global_object
/* 801E108C 001DDFCC  38 00 00 01 */	li r0, 1
/* 801E1090 001DDFD0  98 0D 8F 8D */	stb r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1@sda21(r13)
lbl_801E1094:
/* 801E1094 001DDFD4  88 0D 8F 8E */	lbz r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1@sda21(r13)
/* 801E1098 001DDFD8  7C 00 07 75 */	extsb. r0, r0
/* 801E109C 001DDFDC  40 82 00 28 */	bne lbl_801E10C4
/* 801E10A0 001DDFE0  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1@sda21
/* 801E10A4 001DDFE4  4B E2 DB 91 */	bl initiate__10JSUPtrListFv
/* 801E10A8 001DDFE8  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@ha
/* 801E10AC 001DDFEC  38 83 A7 5C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@l
/* 801E10B0 001DDFF0  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1@sda21
/* 801E10B4 001DDFF4  38 BF 00 3C */	addi r5, r31, 0x3c
/* 801E10B8 001DDFF8  4B EA 16 71 */	bl __register_global_object
/* 801E10BC 001DDFFC  38 00 00 01 */	li r0, 1
/* 801E10C0 001DE000  98 0D 8F 8E */	stb r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1@sda21(r13)
lbl_801E10C4:
/* 801E10C4 001DE004  88 0D 8F 8F */	lbz r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1@sda21(r13)
/* 801E10C8 001DE008  7C 00 07 75 */	extsb. r0, r0
/* 801E10CC 001DE00C  40 82 00 28 */	bne lbl_801E10F4
/* 801E10D0 001DE010  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1@sda21
/* 801E10D4 001DE014  4B E2 DB 61 */	bl initiate__10JSUPtrListFv
/* 801E10D8 001DE018  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@ha
/* 801E10DC 001DE01C  38 83 A7 04 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@l
/* 801E10E0 001DE020  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1@sda21
/* 801E10E4 001DE024  38 BF 00 48 */	addi r5, r31, 0x48
/* 801E10E8 001DE028  4B EA 16 41 */	bl __register_global_object
/* 801E10EC 001DE02C  38 00 00 01 */	li r0, 1
/* 801E10F0 001DE030  98 0D 8F 8F */	stb r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1@sda21(r13)
lbl_801E10F4:
/* 801E10F4 001DE034  88 0D 8F 90 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1@sda21(r13)
/* 801E10F8 001DE038  7C 00 07 75 */	extsb. r0, r0
/* 801E10FC 001DE03C  40 82 00 28 */	bne lbl_801E1124
/* 801E1100 001DE040  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1@sda21
/* 801E1104 001DE044  4B E2 DB 31 */	bl initiate__10JSUPtrListFv
/* 801E1108 001DE048  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@ha
/* 801E110C 001DE04C  38 83 A6 AC */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@l
/* 801E1110 001DE050  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1@sda21
/* 801E1114 001DE054  38 BF 00 54 */	addi r5, r31, 0x54
/* 801E1118 001DE058  4B EA 16 11 */	bl __register_global_object
/* 801E111C 001DE05C  38 00 00 01 */	li r0, 1
/* 801E1120 001DE060  98 0D 8F 90 */	stb r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1@sda21(r13)
lbl_801E1124:
/* 801E1124 001DE064  88 0D 8F 91 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1@sda21(r13)
/* 801E1128 001DE068  7C 00 07 75 */	extsb. r0, r0
/* 801E112C 001DE06C  40 82 00 28 */	bne lbl_801E1154
/* 801E1130 001DE070  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1@sda21
/* 801E1134 001DE074  4B E2 DB 01 */	bl initiate__10JSUPtrListFv
/* 801E1138 001DE078  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@ha
/* 801E113C 001DE07C  38 83 A6 54 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@l
/* 801E1140 001DE080  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1@sda21
/* 801E1144 001DE084  38 BF 00 60 */	addi r5, r31, 0x60
/* 801E1148 001DE088  4B EA 15 E1 */	bl __register_global_object
/* 801E114C 001DE08C  38 00 00 01 */	li r0, 1
/* 801E1150 001DE090  98 0D 8F 91 */	stb r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1@sda21(r13)
lbl_801E1154:
/* 801E1154 001DE094  88 0D 8F 92 */	lbz r0, __init__smList__26JALList$$015JALSeModEffDist$$1@sda21(r13)
/* 801E1158 001DE098  7C 00 07 75 */	extsb. r0, r0
/* 801E115C 001DE09C  40 82 00 28 */	bne lbl_801E1184
/* 801E1160 001DE0A0  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1@sda21
/* 801E1164 001DE0A4  4B E2 DA D1 */	bl initiate__10JSUPtrListFv
/* 801E1168 001DE0A8  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@ha
/* 801E116C 001DE0AC  38 83 A5 FC */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@l
/* 801E1170 001DE0B0  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1@sda21
/* 801E1174 001DE0B4  38 BF 00 6C */	addi r5, r31, 0x6c
/* 801E1178 001DE0B8  4B EA 15 B1 */	bl __register_global_object
/* 801E117C 001DE0BC  38 00 00 01 */	li r0, 1
/* 801E1180 001DE0C0  98 0D 8F 92 */	stb r0, __init__smList__26JALList$$015JALSeModEffDist$$1@sda21(r13)
lbl_801E1184:
/* 801E1184 001DE0C4  88 0D 8F 93 */	lbz r0, __init__smList__26JALList$$015JALSeModPitDist$$1@sda21(r13)
/* 801E1188 001DE0C8  7C 00 07 75 */	extsb. r0, r0
/* 801E118C 001DE0CC  40 82 00 28 */	bne lbl_801E11B4
/* 801E1190 001DE0D0  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1@sda21
/* 801E1194 001DE0D4  4B E2 DA A1 */	bl initiate__10JSUPtrListFv
/* 801E1198 001DE0D8  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@ha
/* 801E119C 001DE0DC  38 83 A5 A4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@l
/* 801E11A0 001DE0E0  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1@sda21
/* 801E11A4 001DE0E4  38 BF 00 78 */	addi r5, r31, 0x78
/* 801E11A8 001DE0E8  4B EA 15 81 */	bl __register_global_object
/* 801E11AC 001DE0EC  38 00 00 01 */	li r0, 1
/* 801E11B0 001DE0F0  98 0D 8F 93 */	stb r0, __init__smList__26JALList$$015JALSeModPitDist$$1@sda21(r13)
lbl_801E11B4:
/* 801E11B4 001DE0F4  88 0D 8F 94 */	lbz r0, __init__smList__26JALList$$015JALSeModVolDist$$1@sda21(r13)
/* 801E11B8 001DE0F8  7C 00 07 75 */	extsb. r0, r0
/* 801E11BC 001DE0FC  40 82 00 28 */	bne lbl_801E11E4
/* 801E11C0 001DE100  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1@sda21
/* 801E11C4 001DE104  4B E2 DA 71 */	bl initiate__10JSUPtrListFv
/* 801E11C8 001DE108  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@ha
/* 801E11CC 001DE10C  38 83 A5 4C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@l
/* 801E11D0 001DE110  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1@sda21
/* 801E11D4 001DE114  38 BF 00 84 */	addi r5, r31, 0x84
/* 801E11D8 001DE118  4B EA 15 51 */	bl __register_global_object
/* 801E11DC 001DE11C  38 00 00 01 */	li r0, 1
/* 801E11E0 001DE120  98 0D 8F 94 */	stb r0, __init__smList__26JALList$$015JALSeModVolDist$$1@sda21(r13)
lbl_801E11E4:
/* 801E11E4 001DE124  88 0D 8F 95 */	lbz r0, __init__smList__26JALList$$015JALSeModEffFunk$$1@sda21(r13)
/* 801E11E8 001DE128  7C 00 07 75 */	extsb. r0, r0
/* 801E11EC 001DE12C  40 82 00 28 */	bne lbl_801E1214
/* 801E11F0 001DE130  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1@sda21
/* 801E11F4 001DE134  4B E2 DA 41 */	bl initiate__10JSUPtrListFv
/* 801E11F8 001DE138  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@ha
/* 801E11FC 001DE13C  38 83 A4 F4 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@l
/* 801E1200 001DE140  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1@sda21
/* 801E1204 001DE144  38 BF 00 90 */	addi r5, r31, 0x90
/* 801E1208 001DE148  4B EA 15 21 */	bl __register_global_object
/* 801E120C 001DE14C  38 00 00 01 */	li r0, 1
/* 801E1210 001DE150  98 0D 8F 95 */	stb r0, __init__smList__26JALList$$015JALSeModEffFunk$$1@sda21(r13)
lbl_801E1214:
/* 801E1214 001DE154  88 0D 8F 96 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFunk$$1@sda21(r13)
/* 801E1218 001DE158  7C 00 07 75 */	extsb. r0, r0
/* 801E121C 001DE15C  40 82 00 28 */	bne lbl_801E1244
/* 801E1220 001DE160  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1@sda21
/* 801E1224 001DE164  4B E2 DA 11 */	bl initiate__10JSUPtrListFv
/* 801E1228 001DE168  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@ha
/* 801E122C 001DE16C  38 83 A4 9C */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@l
/* 801E1230 001DE170  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1@sda21
/* 801E1234 001DE174  38 BF 00 9C */	addi r5, r31, 0x9c
/* 801E1238 001DE178  4B EA 14 F1 */	bl __register_global_object
/* 801E123C 001DE17C  38 00 00 01 */	li r0, 1
/* 801E1240 001DE180  98 0D 8F 96 */	stb r0, __init__smList__26JALList$$015JALSeModPitFunk$$1@sda21(r13)
lbl_801E1244:
/* 801E1244 001DE184  88 0D 8F 97 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFunk$$1@sda21(r13)
/* 801E1248 001DE188  7C 00 07 75 */	extsb. r0, r0
/* 801E124C 001DE18C  40 82 00 28 */	bne lbl_801E1274
/* 801E1250 001DE190  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1@sda21
/* 801E1254 001DE194  4B E2 D9 E1 */	bl initiate__10JSUPtrListFv
/* 801E1258 001DE198  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@ha
/* 801E125C 001DE19C  38 83 A4 44 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@l
/* 801E1260 001DE1A0  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1@sda21
/* 801E1264 001DE1A4  38 BF 00 A8 */	addi r5, r31, 0xa8
/* 801E1268 001DE1A8  4B EA 14 C1 */	bl __register_global_object
/* 801E126C 001DE1AC  38 00 00 01 */	li r0, 1
/* 801E1270 001DE1B0  98 0D 8F 97 */	stb r0, __init__smList__26JALList$$015JALSeModVolFunk$$1@sda21(r13)
lbl_801E1274:
/* 801E1274 001DE1B4  80 01 00 14 */	lwz r0, 0x14(r1)
/* 801E1278 001DE1B8  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 801E127C 001DE1BC  38 21 00 10 */	addi r1, r1, 0x10
/* 801E1280 001DE1C0  7C 08 03 A6 */	mtlr r0
/* 801E1284 001DE1C4  4E 80 00 20 */	blr 

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
	.incbin "baserom.dol", 0x36CDBC, 0x4

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__9TMapEvent
__vt__9TMapEvent:
	.incbin "baserom.dol", 0x3CCD40, 0x40

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
$$22700:
	.skip 0xC
$$22701:
	.skip 0xC
$$22702:
	.skip 0xC
$$22703:
	.skip 0xC
$$22704:
	.skip 0xC
$$22705:
	.skip 0xC
$$22706:
	.skip 0xC
$$22707:
	.skip 0xC
$$22708:
	.skip 0xC
$$22709:
	.skip 0xC
$$22710:
	.skip 0xC
$$22711:
	.skip 0xC
$$22712:
	.skip 0xC
$$22713:
	.skip 0xC
$$22714:
	.skip 0x10

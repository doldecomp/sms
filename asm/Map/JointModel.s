.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global getJ3DModelDataFlag__11TJointModelCFv
getJ3DModelDataFlag__11TJointModelCFv:
/* 801D9E18 001D6D58  3C 60 10 02 */	lis r3, 0x1002
/* 801D9E1C 001D6D5C  4E 80 00 20 */	blr 

.global newJointObj__9TJointObjCFv
newJointObj__9TJointObjCFv:
/* 801D9E20 001D6D60  7C 08 02 A6 */	mflr r0
/* 801D9E24 001D6D64  38 60 00 20 */	li r3, 0x20
/* 801D9E28 001D6D68  90 01 00 04 */	stw r0, 4(r1)
/* 801D9E2C 001D6D6C  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 801D9E30 001D6D70  93 E1 00 14 */	stw r31, 0x14(r1)
/* 801D9E34 001D6D74  4B E3 2A 7D */	bl __nw__FUl
/* 801D9E38 001D6D78  7C 7F 1B 79 */	or. r31, r3, r3
/* 801D9E3C 001D6D7C  41 82 00 0C */	beq lbl_801D9E48
/* 801D9E40 001D6D80  7F E3 FB 78 */	mr r3, r31
/* 801D9E44 001D6D84  48 00 0B E1 */	bl __ct__9TJointObjFv
lbl_801D9E48:
/* 801D9E48 001D6D88  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 801D9E4C 001D6D8C  7F E3 FB 78 */	mr r3, r31
/* 801D9E50 001D6D90  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 801D9E54 001D6D94  38 21 00 18 */	addi r1, r1, 0x18
/* 801D9E58 001D6D98  7C 08 03 A6 */	mtlr r0
/* 801D9E5C 001D6D9C  4E 80 00 20 */	blr 

.global __ct__11TJointModelFv
__ct__11TJointModelFv:
/* 801D9E60 001D6DA0  7C 08 02 A6 */	mflr r0
/* 801D9E64 001D6DA4  90 01 00 04 */	stw r0, 4(r1)
/* 801D9E68 001D6DA8  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 801D9E6C 001D6DAC  93 E1 00 14 */	stw r31, 0x14(r1)
/* 801D9E70 001D6DB0  7C 7F 1B 78 */	mr r31, r3
/* 801D9E74 001D6DB4  48 00 0B B1 */	bl __ct__9TJointObjFv
/* 801D9E78 001D6DB8  3C 60 80 3D */	lis r3, __vt__11TJointModel@ha
/* 801D9E7C 001D6DBC  38 03 FB 80 */	addi r0, r3, __vt__11TJointModel@l
/* 801D9E80 001D6DC0  90 1F 00 00 */	stw r0, 0(r31)
/* 801D9E84 001D6DC4  38 00 00 00 */	li r0, 0
/* 801D9E88 001D6DC8  38 7F 00 00 */	addi r3, r31, 0
/* 801D9E8C 001D6DCC  90 1F 00 20 */	stw r0, 0x20(r31)
/* 801D9E90 001D6DD0  90 1F 00 24 */	stw r0, 0x24(r31)
/* 801D9E94 001D6DD4  90 1F 00 28 */	stw r0, 0x28(r31)
/* 801D9E98 001D6DD8  90 1F 00 2C */	stw r0, 0x2c(r31)
/* 801D9E9C 001D6DDC  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 801D9EA0 001D6DE0  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 801D9EA4 001D6DE4  38 21 00 18 */	addi r1, r1, 0x18
/* 801D9EA8 001D6DE8  7C 08 03 A6 */	mtlr r0
/* 801D9EAC 001D6DEC  4E 80 00 20 */	blr 

.global initJointModel__11TJointModelFP18TJointModelManagerPCcP13MActorAnmData
initJointModel__11TJointModelFP18TJointModelManagerPCcP13MActorAnmData:
/* 801D9EB0 001D6DF0  7C 08 02 A6 */	mflr r0
/* 801D9EB4 001D6DF4  90 01 00 04 */	stw r0, 4(r1)
/* 801D9EB8 001D6DF8  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 801D9EBC 001D6DFC  93 E1 00 24 */	stw r31, 0x24(r1)
/* 801D9EC0 001D6E00  7C 7F 1B 78 */	mr r31, r3
/* 801D9EC4 001D6E04  90 83 00 20 */	stw r4, 0x20(r3)
/* 801D9EC8 001D6E08  38 85 00 00 */	addi r4, r5, 0
/* 801D9ECC 001D6E0C  38 A6 00 00 */	addi r5, r6, 0
/* 801D9ED0 001D6E10  48 00 00 2D */	bl initActor__11TJointModelFPCcP13MActorAnmData
/* 801D9ED4 001D6E14  80 9F 00 24 */	lwz r4, 0x24(r31)
/* 801D9ED8 001D6E18  7F E3 FB 78 */	mr r3, r31
/* 801D9EDC 001D6E1C  80 84 00 20 */	lwz r4, 0x20(r4)
/* 801D9EE0 001D6E20  80 84 00 00 */	lwz r4, 0(r4)
/* 801D9EE4 001D6E24  48 00 0B 71 */	bl initJointObj__9TJointObjFP8J3DJoint
/* 801D9EE8 001D6E28  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 801D9EEC 001D6E2C  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 801D9EF0 001D6E30  38 21 00 28 */	addi r1, r1, 0x28
/* 801D9EF4 001D6E34  7C 08 03 A6 */	mtlr r0
/* 801D9EF8 001D6E38  4E 80 00 20 */	blr 

.global initActor__11TJointModelFPCcP13MActorAnmData
initActor__11TJointModelFPCcP13MActorAnmData:
/* 801D9EFC 001D6E3C  7C 08 02 A6 */	mflr r0
/* 801D9F00 001D6E40  38 E4 00 00 */	addi r7, r4, 0
/* 801D9F04 001D6E44  90 01 00 04 */	stw r0, 4(r1)
/* 801D9F08 001D6E48  4C C6 31 82 */	crclr 6
/* 801D9F0C 001D6E4C  38 80 00 40 */	li r4, 0x40
/* 801D9F10 001D6E50  94 21 FF 80 */	stwu r1, -0x80(r1)
/* 801D9F14 001D6E54  93 E1 00 7C */	stw r31, 0x7c(r1)
/* 801D9F18 001D6E58  3B E5 00 00 */	addi r31, r5, 0
/* 801D9F1C 001D6E5C  93 C1 00 78 */	stw r30, 0x78(r1)
/* 801D9F20 001D6E60  3B C3 00 00 */	addi r30, r3, 0
/* 801D9F24 001D6E64  93 A1 00 74 */	stw r29, 0x74(r1)
/* 801D9F28 001D6E68  93 81 00 70 */	stw r28, 0x70(r1)
/* 801D9F2C 001D6E6C  80 C3 00 20 */	lwz r6, 0x20(r3)
/* 801D9F30 001D6E70  3C 60 80 39 */	lis r3, $$22324@ha
/* 801D9F34 001D6E74  38 03 14 78 */	addi r0, r3, $$22324@l
/* 801D9F38 001D6E78  80 C6 00 1C */	lwz r6, 0x1c(r6)
/* 801D9F3C 001D6E7C  7C 05 03 78 */	mr r5, r0
/* 801D9F40 001D6E80  38 61 00 30 */	addi r3, r1, 0x30
/* 801D9F44 001D6E84  4B EA A1 81 */	bl snprintf
/* 801D9F48 001D6E88  38 61 00 30 */	addi r3, r1, 0x30
/* 801D9F4C 001D6E8C  4B E3 1D 71 */	bl getGlbResource__13JKRFileLoaderFPCc
/* 801D9F50 001D6E90  38 03 00 00 */	addi r0, r3, 0
/* 801D9F54 001D6E94  38 7E 00 00 */	addi r3, r30, 0
/* 801D9F58 001D6E98  81 9E 00 00 */	lwz r12, 0(r30)
/* 801D9F5C 001D6E9C  7C 1D 03 78 */	mr r29, r0
/* 801D9F60 001D6EA0  81 8C 00 30 */	lwz r12, 0x30(r12)
/* 801D9F64 001D6EA4  7D 88 03 A6 */	mtlr r12
/* 801D9F68 001D6EA8  4E 80 00 21 */	blrl 
/* 801D9F6C 001D6EAC  38 83 00 00 */	addi r4, r3, 0
/* 801D9F70 001D6EB0  38 7D 00 00 */	addi r3, r29, 0
/* 801D9F74 001D6EB4  4B E5 6A F1 */	bl load__22J3DModelLoaderDataBaseFPCvUl
/* 801D9F78 001D6EB8  90 7E 00 24 */	stw r3, 0x24(r30)
/* 801D9F7C 001D6EBC  38 60 00 A0 */	li r3, 0xa0
/* 801D9F80 001D6EC0  4B E3 29 31 */	bl __nw__FUl
/* 801D9F84 001D6EC4  7C 7C 1B 79 */	or. r28, r3, r3
/* 801D9F88 001D6EC8  41 82 00 18 */	beq lbl_801D9FA0
/* 801D9F8C 001D6ECC  80 9E 00 24 */	lwz r4, 0x24(r30)
/* 801D9F90 001D6ED0  38 7C 00 00 */	addi r3, r28, 0
/* 801D9F94 001D6ED4  38 A0 00 00 */	li r5, 0
/* 801D9F98 001D6ED8  38 C0 00 01 */	li r6, 1
/* 801D9F9C 001D6EDC  4B E4 D9 BD */	bl __ct__8J3DModelFP12J3DModelDataUlUl
lbl_801D9FA0:
/* 801D9FA0 001D6EE0  3C 60 80 3B */	lis r3, j3dDefaultMtx@ha
/* 801D9FA4 001D6EE4  93 9E 00 28 */	stw r28, 0x28(r30)
/* 801D9FA8 001D6EE8  3B A3 8D D0 */	addi r29, r3, j3dDefaultMtx@l
/* 801D9FAC 001D6EEC  3B 80 00 00 */	li r28, 0
/* 801D9FB0 001D6EF0  48 00 00 1C */	b lbl_801D9FCC
lbl_801D9FB4:
/* 801D9FB4 001D6EF4  80 64 00 28 */	lwz r3, 0x28(r4)
/* 801D9FB8 001D6EF8  57 80 13 BA */	rlwinm r0, r28, 2, 0xe, 0x1d
/* 801D9FBC 001D6EFC  7F A4 EB 78 */	mr r4, r29
/* 801D9FC0 001D6F00  7C 63 00 2E */	lwzx r3, r3, r0
/* 801D9FC4 001D6F04  4B E4 B0 6D */	bl calc__11J3DMaterialFPA4_f
/* 801D9FC8 001D6F08  3B 9C 00 01 */	addi r28, r28, 1
lbl_801D9FCC:
/* 801D9FCC 001D6F0C  80 9E 00 24 */	lwz r4, 0x24(r30)
/* 801D9FD0 001D6F10  57 80 04 3E */	clrlwi r0, r28, 0x10
/* 801D9FD4 001D6F14  A0 64 00 24 */	lhz r3, 0x24(r4)
/* 801D9FD8 001D6F18  7C 00 18 40 */	cmplw r0, r3
/* 801D9FDC 001D6F1C  41 80 FF D8 */	blt lbl_801D9FB4
/* 801D9FE0 001D6F20  80 7E 00 28 */	lwz r3, 0x28(r30)
/* 801D9FE4 001D6F24  4B E4 E2 19 */	bl makeDL__8J3DModelFv
/* 801D9FE8 001D6F28  80 7E 00 28 */	lwz r3, 0x28(r30)
/* 801D9FEC 001D6F2C  4B E4 DF B1 */	bl lock__8J3DModelFv
/* 801D9FF0 001D6F30  38 60 00 48 */	li r3, 0x48
/* 801D9FF4 001D6F34  4B E3 28 BD */	bl __nw__FUl
/* 801D9FF8 001D6F38  7C 7C 1B 79 */	or. r28, r3, r3
/* 801D9FFC 001D6F3C  41 82 00 10 */	beq lbl_801DA00C
/* 801DA000 001D6F40  38 7C 00 00 */	addi r3, r28, 0
/* 801DA004 001D6F44  38 9F 00 00 */	addi r4, r31, 0
/* 801DA008 001D6F48  4B EF B6 D1 */	bl __ct__6MActorFP13MActorAnmData
lbl_801DA00C:
/* 801DA00C 001D6F4C  93 9E 00 2C */	stw r28, 0x2c(r30)
/* 801DA010 001D6F50  38 A0 00 00 */	li r5, 0
/* 801DA014 001D6F54  80 7E 00 2C */	lwz r3, 0x2c(r30)
/* 801DA018 001D6F58  80 9E 00 28 */	lwz r4, 0x28(r30)
/* 801DA01C 001D6F5C  4B EF B3 C5 */	bl setModel__6MActorFP8J3DModelUl
/* 801DA020 001D6F60  80 01 00 84 */	lwz r0, 0x84(r1)
/* 801DA024 001D6F64  83 E1 00 7C */	lwz r31, 0x7c(r1)
/* 801DA028 001D6F68  83 C1 00 78 */	lwz r30, 0x78(r1)
/* 801DA02C 001D6F6C  7C 08 03 A6 */	mtlr r0
/* 801DA030 001D6F70  83 A1 00 74 */	lwz r29, 0x74(r1)
/* 801DA034 001D6F74  83 81 00 70 */	lwz r28, 0x70(r1)
/* 801DA038 001D6F78  38 21 00 80 */	addi r1, r1, 0x80
/* 801DA03C 001D6F7C  4E 80 00 20 */	blr 

.global perform__11TJointModelFUlPQ26JDrama9TGraphics
perform__11TJointModelFUlPQ26JDrama9TGraphics:
/* 801DA040 001D6F80  7C 08 02 A6 */	mflr r0
/* 801DA044 001D6F84  90 01 00 04 */	stw r0, 4(r1)
/* 801DA048 001D6F88  94 21 FF F8 */	stwu r1, -8(r1)
/* 801DA04C 001D6F8C  80 03 00 08 */	lwz r0, 8(r3)
/* 801DA050 001D6F90  54 00 07 FF */	clrlwi. r0, r0, 0x1f
/* 801DA054 001D6F94  41 82 00 0C */	beq lbl_801DA060
/* 801DA058 001D6F98  38 00 00 01 */	li r0, 1
/* 801DA05C 001D6F9C  48 00 00 08 */	b lbl_801DA064
lbl_801DA060:
/* 801DA060 001D6FA0  38 00 00 00 */	li r0, 0
lbl_801DA064:
/* 801DA064 001D6FA4  54 00 06 3F */	clrlwi. r0, r0, 0x18
/* 801DA068 001D6FA8  40 82 00 0C */	bne lbl_801DA074
/* 801DA06C 001D6FAC  80 63 00 2C */	lwz r3, 0x2c(r3)
/* 801DA070 001D6FB0  4B EF A4 69 */	bl perform__6MActorFUlPQ26JDrama9TGraphics
lbl_801DA074:
/* 801DA074 001D6FB4  80 01 00 0C */	lwz r0, 0xc(r1)
/* 801DA078 001D6FB8  38 21 00 08 */	addi r1, r1, 8
/* 801DA07C 001D6FBC  7C 08 03 A6 */	mtlr r0
/* 801DA080 001D6FC0  4E 80 00 20 */	blr 

.global __sinit_JointModel_cpp
__sinit_JointModel_cpp:
/* 801DA084 001D6FC4  7C 08 02 A6 */	mflr r0
/* 801DA088 001D6FC8  3C 60 80 3F */	lis r3, $$22331@ha
/* 801DA08C 001D6FCC  90 01 00 04 */	stw r0, 4(r1)
/* 801DA090 001D6FD0  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 801DA094 001D6FD4  93 E1 00 0C */	stw r31, 0xc(r1)
/* 801DA098 001D6FD8  3B E3 75 70 */	addi r31, r3, $$22331@l
/* 801DA09C 001D6FDC  88 0D 97 CC */	lbz r0, __init__smList__15JALList$$05MSBgm$$1-_SDA_BASE_(r13)
/* 801DA0A0 001D6FE0  7C 00 07 75 */	extsb. r0, r0
/* 801DA0A4 001D6FE4  40 82 00 28 */	bne lbl_801DA0CC
/* 801DA0A8 001D6FE8  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1-_SDA_BASE_
/* 801DA0AC 001D6FEC  4B E3 4B 89 */	bl initiate__10JSUPtrListFv
/* 801DA0B0 001D6FF0  3C 60 80 0E */	lis r3, __dt__15JSUList$$05MSBgm$$1Fv@ha
/* 801DA0B4 001D6FF4  38 83 6A 44 */	addi r4, r3, __dt__15JSUList$$05MSBgm$$1Fv@l
/* 801DA0B8 001D6FF8  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1-_SDA_BASE_
/* 801DA0BC 001D6FFC  38 BF 00 00 */	addi r5, r31, 0
/* 801DA0C0 001D7000  4B EA 86 69 */	bl __register_global_object
/* 801DA0C4 001D7004  38 00 00 01 */	li r0, 1
/* 801DA0C8 001D7008  98 0D 97 CC */	stb r0, __init__smList__15JALList$$05MSBgm$$1-_SDA_BASE_(r13)
lbl_801DA0CC:
/* 801DA0CC 001D700C  88 0D 97 CD */	lbz r0, __init__smList__24JALList$$013MSSetSoundGrp$$1-_SDA_BASE_(r13)
/* 801DA0D0 001D7010  7C 00 07 75 */	extsb. r0, r0
/* 801DA0D4 001D7014  40 82 00 28 */	bne lbl_801DA0FC
/* 801DA0D8 001D7018  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1-_SDA_BASE_
/* 801DA0DC 001D701C  4B E3 4B 59 */	bl initiate__10JSUPtrListFv
/* 801DA0E0 001D7020  3C 60 80 0E */	lis r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@ha
/* 801DA0E4 001D7024  38 83 69 EC */	addi r4, r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@l
/* 801DA0E8 001D7028  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1-_SDA_BASE_
/* 801DA0EC 001D702C  38 BF 00 0C */	addi r5, r31, 0xc
/* 801DA0F0 001D7030  4B EA 86 39 */	bl __register_global_object
/* 801DA0F4 001D7034  38 00 00 01 */	li r0, 1
/* 801DA0F8 001D7038  98 0D 97 CD */	stb r0, __init__smList__24JALList$$013MSSetSoundGrp$$1-_SDA_BASE_(r13)
lbl_801DA0FC:
/* 801DA0FC 001D703C  88 0D 97 CE */	lbz r0, __init__smList__21JALList$$010MSSetSound$$1-_SDA_BASE_(r13)
/* 801DA100 001D7040  7C 00 07 75 */	extsb. r0, r0
/* 801DA104 001D7044  40 82 00 28 */	bne lbl_801DA12C
/* 801DA108 001D7048  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1-_SDA_BASE_
/* 801DA10C 001D704C  4B E3 4B 29 */	bl initiate__10JSUPtrListFv
/* 801DA110 001D7050  3C 60 80 0E */	lis r3, __dt__21JSUList$$010MSSetSound$$1Fv@ha
/* 801DA114 001D7054  38 83 69 94 */	addi r4, r3, __dt__21JSUList$$010MSSetSound$$1Fv@l
/* 801DA118 001D7058  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1-_SDA_BASE_
/* 801DA11C 001D705C  38 BF 00 18 */	addi r5, r31, 0x18
/* 801DA120 001D7060  4B EA 86 09 */	bl __register_global_object
/* 801DA124 001D7064  38 00 00 01 */	li r0, 1
/* 801DA128 001D7068  98 0D 97 CE */	stb r0, __init__smList__21JALList$$010MSSetSound$$1-_SDA_BASE_(r13)
lbl_801DA12C:
/* 801DA12C 001D706C  88 0D 8F 8C */	lbz r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1-_SDA_BASE_(r13)
/* 801DA130 001D7070  7C 00 07 75 */	extsb. r0, r0
/* 801DA134 001D7074  40 82 00 28 */	bne lbl_801DA15C
/* 801DA138 001D7078  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1-_SDA_BASE_
/* 801DA13C 001D707C  4B E3 4A F9 */	bl initiate__10JSUPtrListFv
/* 801DA140 001D7080  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@ha
/* 801DA144 001D7084  38 83 A8 0C */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@l
/* 801DA148 001D7088  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1-_SDA_BASE_
/* 801DA14C 001D708C  38 BF 00 24 */	addi r5, r31, 0x24
/* 801DA150 001D7090  4B EA 85 D9 */	bl __register_global_object
/* 801DA154 001D7094  38 00 00 01 */	li r0, 1
/* 801DA158 001D7098  98 0D 8F 8C */	stb r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1-_SDA_BASE_(r13)
lbl_801DA15C:
/* 801DA15C 001D709C  88 0D 8F 8D */	lbz r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1-_SDA_BASE_(r13)
/* 801DA160 001D70A0  7C 00 07 75 */	extsb. r0, r0
/* 801DA164 001D70A4  40 82 00 28 */	bne lbl_801DA18C
/* 801DA168 001D70A8  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1-_SDA_BASE_
/* 801DA16C 001D70AC  4B E3 4A C9 */	bl initiate__10JSUPtrListFv
/* 801DA170 001D70B0  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@ha
/* 801DA174 001D70B4  38 83 A7 B4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@l
/* 801DA178 001D70B8  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1-_SDA_BASE_
/* 801DA17C 001D70BC  38 BF 00 30 */	addi r5, r31, 0x30
/* 801DA180 001D70C0  4B EA 85 A9 */	bl __register_global_object
/* 801DA184 001D70C4  38 00 00 01 */	li r0, 1
/* 801DA188 001D70C8  98 0D 8F 8D */	stb r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1-_SDA_BASE_(r13)
lbl_801DA18C:
/* 801DA18C 001D70CC  88 0D 8F 8E */	lbz r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1-_SDA_BASE_(r13)
/* 801DA190 001D70D0  7C 00 07 75 */	extsb. r0, r0
/* 801DA194 001D70D4  40 82 00 28 */	bne lbl_801DA1BC
/* 801DA198 001D70D8  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1-_SDA_BASE_
/* 801DA19C 001D70DC  4B E3 4A 99 */	bl initiate__10JSUPtrListFv
/* 801DA1A0 001D70E0  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@ha
/* 801DA1A4 001D70E4  38 83 A7 5C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@l
/* 801DA1A8 001D70E8  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1-_SDA_BASE_
/* 801DA1AC 001D70EC  38 BF 00 3C */	addi r5, r31, 0x3c
/* 801DA1B0 001D70F0  4B EA 85 79 */	bl __register_global_object
/* 801DA1B4 001D70F4  38 00 00 01 */	li r0, 1
/* 801DA1B8 001D70F8  98 0D 8F 8E */	stb r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1-_SDA_BASE_(r13)
lbl_801DA1BC:
/* 801DA1BC 001D70FC  88 0D 8F 8F */	lbz r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1-_SDA_BASE_(r13)
/* 801DA1C0 001D7100  7C 00 07 75 */	extsb. r0, r0
/* 801DA1C4 001D7104  40 82 00 28 */	bne lbl_801DA1EC
/* 801DA1C8 001D7108  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1-_SDA_BASE_
/* 801DA1CC 001D710C  4B E3 4A 69 */	bl initiate__10JSUPtrListFv
/* 801DA1D0 001D7110  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@ha
/* 801DA1D4 001D7114  38 83 A7 04 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@l
/* 801DA1D8 001D7118  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1-_SDA_BASE_
/* 801DA1DC 001D711C  38 BF 00 48 */	addi r5, r31, 0x48
/* 801DA1E0 001D7120  4B EA 85 49 */	bl __register_global_object
/* 801DA1E4 001D7124  38 00 00 01 */	li r0, 1
/* 801DA1E8 001D7128  98 0D 8F 8F */	stb r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1-_SDA_BASE_(r13)
lbl_801DA1EC:
/* 801DA1EC 001D712C  88 0D 8F 90 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1-_SDA_BASE_(r13)
/* 801DA1F0 001D7130  7C 00 07 75 */	extsb. r0, r0
/* 801DA1F4 001D7134  40 82 00 28 */	bne lbl_801DA21C
/* 801DA1F8 001D7138  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1-_SDA_BASE_
/* 801DA1FC 001D713C  4B E3 4A 39 */	bl initiate__10JSUPtrListFv
/* 801DA200 001D7140  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@ha
/* 801DA204 001D7144  38 83 A6 AC */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@l
/* 801DA208 001D7148  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1-_SDA_BASE_
/* 801DA20C 001D714C  38 BF 00 54 */	addi r5, r31, 0x54
/* 801DA210 001D7150  4B EA 85 19 */	bl __register_global_object
/* 801DA214 001D7154  38 00 00 01 */	li r0, 1
/* 801DA218 001D7158  98 0D 8F 90 */	stb r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1-_SDA_BASE_(r13)
lbl_801DA21C:
/* 801DA21C 001D715C  88 0D 8F 91 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1-_SDA_BASE_(r13)
/* 801DA220 001D7160  7C 00 07 75 */	extsb. r0, r0
/* 801DA224 001D7164  40 82 00 28 */	bne lbl_801DA24C
/* 801DA228 001D7168  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1-_SDA_BASE_
/* 801DA22C 001D716C  4B E3 4A 09 */	bl initiate__10JSUPtrListFv
/* 801DA230 001D7170  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@ha
/* 801DA234 001D7174  38 83 A6 54 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@l
/* 801DA238 001D7178  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1-_SDA_BASE_
/* 801DA23C 001D717C  38 BF 00 60 */	addi r5, r31, 0x60
/* 801DA240 001D7180  4B EA 84 E9 */	bl __register_global_object
/* 801DA244 001D7184  38 00 00 01 */	li r0, 1
/* 801DA248 001D7188  98 0D 8F 91 */	stb r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1-_SDA_BASE_(r13)
lbl_801DA24C:
/* 801DA24C 001D718C  88 0D 8F 92 */	lbz r0, __init__smList__26JALList$$015JALSeModEffDist$$1-_SDA_BASE_(r13)
/* 801DA250 001D7190  7C 00 07 75 */	extsb. r0, r0
/* 801DA254 001D7194  40 82 00 28 */	bne lbl_801DA27C
/* 801DA258 001D7198  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1-_SDA_BASE_
/* 801DA25C 001D719C  4B E3 49 D9 */	bl initiate__10JSUPtrListFv
/* 801DA260 001D71A0  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@ha
/* 801DA264 001D71A4  38 83 A5 FC */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@l
/* 801DA268 001D71A8  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1-_SDA_BASE_
/* 801DA26C 001D71AC  38 BF 00 6C */	addi r5, r31, 0x6c
/* 801DA270 001D71B0  4B EA 84 B9 */	bl __register_global_object
/* 801DA274 001D71B4  38 00 00 01 */	li r0, 1
/* 801DA278 001D71B8  98 0D 8F 92 */	stb r0, __init__smList__26JALList$$015JALSeModEffDist$$1-_SDA_BASE_(r13)
lbl_801DA27C:
/* 801DA27C 001D71BC  88 0D 8F 93 */	lbz r0, __init__smList__26JALList$$015JALSeModPitDist$$1-_SDA_BASE_(r13)
/* 801DA280 001D71C0  7C 00 07 75 */	extsb. r0, r0
/* 801DA284 001D71C4  40 82 00 28 */	bne lbl_801DA2AC
/* 801DA288 001D71C8  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1-_SDA_BASE_
/* 801DA28C 001D71CC  4B E3 49 A9 */	bl initiate__10JSUPtrListFv
/* 801DA290 001D71D0  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@ha
/* 801DA294 001D71D4  38 83 A5 A4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@l
/* 801DA298 001D71D8  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1-_SDA_BASE_
/* 801DA29C 001D71DC  38 BF 00 78 */	addi r5, r31, 0x78
/* 801DA2A0 001D71E0  4B EA 84 89 */	bl __register_global_object
/* 801DA2A4 001D71E4  38 00 00 01 */	li r0, 1
/* 801DA2A8 001D71E8  98 0D 8F 93 */	stb r0, __init__smList__26JALList$$015JALSeModPitDist$$1-_SDA_BASE_(r13)
lbl_801DA2AC:
/* 801DA2AC 001D71EC  88 0D 8F 94 */	lbz r0, __init__smList__26JALList$$015JALSeModVolDist$$1-_SDA_BASE_(r13)
/* 801DA2B0 001D71F0  7C 00 07 75 */	extsb. r0, r0
/* 801DA2B4 001D71F4  40 82 00 28 */	bne lbl_801DA2DC
/* 801DA2B8 001D71F8  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1-_SDA_BASE_
/* 801DA2BC 001D71FC  4B E3 49 79 */	bl initiate__10JSUPtrListFv
/* 801DA2C0 001D7200  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@ha
/* 801DA2C4 001D7204  38 83 A5 4C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@l
/* 801DA2C8 001D7208  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1-_SDA_BASE_
/* 801DA2CC 001D720C  38 BF 00 84 */	addi r5, r31, 0x84
/* 801DA2D0 001D7210  4B EA 84 59 */	bl __register_global_object
/* 801DA2D4 001D7214  38 00 00 01 */	li r0, 1
/* 801DA2D8 001D7218  98 0D 8F 94 */	stb r0, __init__smList__26JALList$$015JALSeModVolDist$$1-_SDA_BASE_(r13)
lbl_801DA2DC:
/* 801DA2DC 001D721C  88 0D 8F 95 */	lbz r0, __init__smList__26JALList$$015JALSeModEffFunk$$1-_SDA_BASE_(r13)
/* 801DA2E0 001D7220  7C 00 07 75 */	extsb. r0, r0
/* 801DA2E4 001D7224  40 82 00 28 */	bne lbl_801DA30C
/* 801DA2E8 001D7228  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1-_SDA_BASE_
/* 801DA2EC 001D722C  4B E3 49 49 */	bl initiate__10JSUPtrListFv
/* 801DA2F0 001D7230  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@ha
/* 801DA2F4 001D7234  38 83 A4 F4 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@l
/* 801DA2F8 001D7238  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1-_SDA_BASE_
/* 801DA2FC 001D723C  38 BF 00 90 */	addi r5, r31, 0x90
/* 801DA300 001D7240  4B EA 84 29 */	bl __register_global_object
/* 801DA304 001D7244  38 00 00 01 */	li r0, 1
/* 801DA308 001D7248  98 0D 8F 95 */	stb r0, __init__smList__26JALList$$015JALSeModEffFunk$$1-_SDA_BASE_(r13)
lbl_801DA30C:
/* 801DA30C 001D724C  88 0D 8F 96 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFunk$$1-_SDA_BASE_(r13)
/* 801DA310 001D7250  7C 00 07 75 */	extsb. r0, r0
/* 801DA314 001D7254  40 82 00 28 */	bne lbl_801DA33C
/* 801DA318 001D7258  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1-_SDA_BASE_
/* 801DA31C 001D725C  4B E3 49 19 */	bl initiate__10JSUPtrListFv
/* 801DA320 001D7260  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@ha
/* 801DA324 001D7264  38 83 A4 9C */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@l
/* 801DA328 001D7268  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1-_SDA_BASE_
/* 801DA32C 001D726C  38 BF 00 9C */	addi r5, r31, 0x9c
/* 801DA330 001D7270  4B EA 83 F9 */	bl __register_global_object
/* 801DA334 001D7274  38 00 00 01 */	li r0, 1
/* 801DA338 001D7278  98 0D 8F 96 */	stb r0, __init__smList__26JALList$$015JALSeModPitFunk$$1-_SDA_BASE_(r13)
lbl_801DA33C:
/* 801DA33C 001D727C  88 0D 8F 97 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFunk$$1-_SDA_BASE_(r13)
/* 801DA340 001D7280  7C 00 07 75 */	extsb. r0, r0
/* 801DA344 001D7284  40 82 00 28 */	bne lbl_801DA36C
/* 801DA348 001D7288  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1-_SDA_BASE_
/* 801DA34C 001D728C  4B E3 48 E9 */	bl initiate__10JSUPtrListFv
/* 801DA350 001D7290  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@ha
/* 801DA354 001D7294  38 83 A4 44 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@l
/* 801DA358 001D7298  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1-_SDA_BASE_
/* 801DA35C 001D729C  38 BF 00 A8 */	addi r5, r31, 0xa8
/* 801DA360 001D72A0  4B EA 83 C9 */	bl __register_global_object
/* 801DA364 001D72A4  38 00 00 01 */	li r0, 1
/* 801DA368 001D72A8  98 0D 8F 97 */	stb r0, __init__smList__26JALList$$015JALSeModVolFunk$$1-_SDA_BASE_(r13)
lbl_801DA36C:
/* 801DA36C 001D72AC  80 01 00 14 */	lwz r0, 0x14(r1)
/* 801DA370 001D72B0  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 801DA374 001D72B4  38 21 00 10 */	addi r1, r1, 0x10
/* 801DA378 001D72B8  7C 08 03 A6 */	mtlr r0
/* 801DA37C 001D72BC  4E 80 00 20 */	blr 

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
	.incbin "baserom.dol", 0x36CDA0, 0x4

.section .rodata, "wa"  # 0x8036FFA0 - 0x803A8380
.global $$22324
$$22324:
	.incbin "baserom.dol", 0x38E478, 0x10

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__11TJointModel
__vt__11TJointModel:
	.incbin "baserom.dol", 0x3CCB80, 0x38

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
.global $$22331
$$22331:
	.skip 0xC
.global $$22332
$$22332:
	.skip 0xC
.global $$22333
$$22333:
	.skip 0xC
.global $$22334
$$22334:
	.skip 0xC
.global $$22335
$$22335:
	.skip 0xC
.global $$22336
$$22336:
	.skip 0xC
.global $$22337
$$22337:
	.skip 0xC
.global $$22338
$$22338:
	.skip 0xC
.global $$22339
$$22339:
	.skip 0xC
.global $$22340
$$22340:
	.skip 0xC
.global $$22341
$$22341:
	.skip 0xC
.global $$22342
$$22342:
	.skip 0xC
.global $$22343
$$22343:
	.skip 0xC
.global $$22344
$$22344:
	.skip 0xC
.global $$22345
$$22345:
	.skip 0x10

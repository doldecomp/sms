.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __ct__21TAnimalSaveIndividualFPCc
__ct__21TAnimalSaveIndividualFPCc:
/* 803678E0 00364820  7C 08 02 A6 */	mflr r0
/* 803678E4 00364824  90 01 00 04 */	stw r0, 4(r1)
/* 803678E8 00364828  38 00 00 00 */	li r0, 0
/* 803678EC 0036482C  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 803678F0 00364830  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 803678F4 00364834  7C 7F 1B 78 */	mr r31, r3
/* 803678F8 00364838  93 C1 00 18 */	stw r30, 0x18(r1)
/* 803678FC 0036483C  93 A1 00 14 */	stw r29, 0x14(r1)
/* 80367900 00364840  93 81 00 10 */	stw r28, 0x10(r1)
/* 80367904 00364844  90 83 00 00 */	stw r4, 0(r3)
/* 80367908 00364848  3C 60 80 3A */	lis r3, $$21490@ha
/* 8036790C 0036484C  3B C3 7C 38 */	addi r30, r3, $$21490@l
/* 80367910 00364850  90 1F 00 04 */	stw r0, 4(r31)
/* 80367914 00364854  38 7E 00 20 */	addi r3, r30, 0x20
/* 80367918 00364858  4B CD CD 75 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 8036791C 0036485C  38 A3 00 00 */	addi r5, r3, 0
/* 80367920 00364860  38 9F 00 00 */	addi r4, r31, 0
/* 80367924 00364864  38 7F 00 08 */	addi r3, r31, 8
/* 80367928 00364868  38 DE 00 20 */	addi r6, r30, 0x20
/* 8036792C 0036486C  4B D7 32 C9 */	bl __ct__10TBaseParamFP7TParamsUsPCc
/* 80367930 00364870  3C 60 80 3B */	lis r3, __vt__10TParamT$$0l$$1@ha
/* 80367934 00364874  38 03 6A 7C */	addi r0, r3, __vt__10TParamT$$0l$$1@l
/* 80367938 00364878  90 1F 00 08 */	stw r0, 8(r31)
/* 8036793C 0036487C  38 00 00 04 */	li r0, 4
/* 80367940 00364880  3C 60 80 3B */	lis r3, __vt__11TParamRT$$0l$$1@ha
/* 80367944 00364884  90 1F 00 18 */	stw r0, 0x18(r31)
/* 80367948 00364888  38 03 6A 70 */	addi r0, r3, __vt__11TParamRT$$0l$$1@l
/* 8036794C 0036488C  38 7E 00 30 */	addi r3, r30, 0x30
/* 80367950 00364890  90 1F 00 08 */	stw r0, 8(r31)
/* 80367954 00364894  4B CD CD 39 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 80367958 00364898  38 A3 00 00 */	addi r5, r3, 0
/* 8036795C 0036489C  38 9F 00 00 */	addi r4, r31, 0
/* 80367960 003648A0  38 7F 00 1C */	addi r3, r31, 0x1c
/* 80367964 003648A4  38 DE 00 30 */	addi r6, r30, 0x30
/* 80367968 003648A8  4B D7 32 8D */	bl __ct__10TBaseParamFP7TParamsUsPCc
/* 8036796C 003648AC  3C 60 80 3B */	lis r3, __vt__10TParamT$$0f$$1@ha
/* 80367970 003648B0  3B A3 29 B8 */	addi r29, r3, __vt__10TParamT$$0f$$1@l
/* 80367974 003648B4  93 BF 00 1C */	stw r29, 0x1c(r31)
/* 80367978 003648B8  3C 60 80 3B */	lis r3, __vt__11TParamRT$$0f$$1@ha
/* 8036797C 003648BC  3B 83 29 F4 */	addi r28, r3, __vt__11TParamRT$$0f$$1@l
/* 80367980 003648C0  C0 02 09 F8 */	lfs f0, $$21564-_SDA2_BASE_(r2)
/* 80367984 003648C4  38 7E 00 40 */	addi r3, r30, 0x40
/* 80367988 003648C8  D0 1F 00 2C */	stfs f0, 0x2c(r31)
/* 8036798C 003648CC  93 9F 00 1C */	stw r28, 0x1c(r31)
/* 80367990 003648D0  4B CD CC FD */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 80367994 003648D4  38 A3 00 00 */	addi r5, r3, 0
/* 80367998 003648D8  38 9F 00 00 */	addi r4, r31, 0
/* 8036799C 003648DC  38 7F 00 30 */	addi r3, r31, 0x30
/* 803679A0 003648E0  38 DE 00 40 */	addi r6, r30, 0x40
/* 803679A4 003648E4  4B D7 32 51 */	bl __ct__10TBaseParamFP7TParamsUsPCc
/* 803679A8 003648E8  93 BF 00 30 */	stw r29, 0x30(r31)
/* 803679AC 003648EC  38 7E 00 50 */	addi r3, r30, 0x50
/* 803679B0 003648F0  C0 02 09 FC */	lfs f0, $$21566-_SDA2_BASE_(r2)
/* 803679B4 003648F4  D0 1F 00 40 */	stfs f0, 0x40(r31)
/* 803679B8 003648F8  93 9F 00 30 */	stw r28, 0x30(r31)
/* 803679BC 003648FC  4B CD CC D1 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 803679C0 00364900  38 A3 00 00 */	addi r5, r3, 0
/* 803679C4 00364904  38 9F 00 00 */	addi r4, r31, 0
/* 803679C8 00364908  38 7F 00 44 */	addi r3, r31, 0x44
/* 803679CC 0036490C  38 DE 00 50 */	addi r6, r30, 0x50
/* 803679D0 00364910  4B D7 32 25 */	bl __ct__10TBaseParamFP7TParamsUsPCc
/* 803679D4 00364914  93 BF 00 44 */	stw r29, 0x44(r31)
/* 803679D8 00364918  38 7E 00 64 */	addi r3, r30, 0x64
/* 803679DC 0036491C  C0 02 0A 00 */	lfs f0, $$21568-_SDA2_BASE_(r2)
/* 803679E0 00364920  D0 1F 00 54 */	stfs f0, 0x54(r31)
/* 803679E4 00364924  93 9F 00 44 */	stw r28, 0x44(r31)
/* 803679E8 00364928  4B CD CC A5 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 803679EC 0036492C  38 A3 00 00 */	addi r5, r3, 0
/* 803679F0 00364930  38 9F 00 00 */	addi r4, r31, 0
/* 803679F4 00364934  38 7F 00 58 */	addi r3, r31, 0x58
/* 803679F8 00364938  38 DE 00 64 */	addi r6, r30, 0x64
/* 803679FC 0036493C  4B D7 31 F9 */	bl __ct__10TBaseParamFP7TParamsUsPCc
/* 80367A00 00364940  93 BF 00 58 */	stw r29, 0x58(r31)
/* 80367A04 00364944  38 7E 00 78 */	addi r3, r30, 0x78
/* 80367A08 00364948  C0 02 0A 04 */	lfs f0, $$21570-_SDA2_BASE_(r2)
/* 80367A0C 0036494C  D0 1F 00 68 */	stfs f0, 0x68(r31)
/* 80367A10 00364950  93 9F 00 58 */	stw r28, 0x58(r31)
/* 80367A14 00364954  4B CD CC 79 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 80367A18 00364958  38 A3 00 00 */	addi r5, r3, 0
/* 80367A1C 0036495C  38 9F 00 00 */	addi r4, r31, 0
/* 80367A20 00364960  38 7F 00 6C */	addi r3, r31, 0x6c
/* 80367A24 00364964  38 DE 00 78 */	addi r6, r30, 0x78
/* 80367A28 00364968  4B D7 31 CD */	bl __ct__10TBaseParamFP7TParamsUsPCc
/* 80367A2C 0036496C  93 BF 00 6C */	stw r29, 0x6c(r31)
/* 80367A30 00364970  38 7E 00 88 */	addi r3, r30, 0x88
/* 80367A34 00364974  C0 02 0A 08 */	lfs f0, $$21572-_SDA2_BASE_(r2)
/* 80367A38 00364978  D0 1F 00 7C */	stfs f0, 0x7c(r31)
/* 80367A3C 0036497C  93 9F 00 6C */	stw r28, 0x6c(r31)
/* 80367A40 00364980  4B CD CC 4D */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 80367A44 00364984  38 A3 00 00 */	addi r5, r3, 0
/* 80367A48 00364988  38 9F 00 00 */	addi r4, r31, 0
/* 80367A4C 0036498C  38 7F 00 80 */	addi r3, r31, 0x80
/* 80367A50 00364990  38 DE 00 88 */	addi r6, r30, 0x88
/* 80367A54 00364994  4B D7 31 A1 */	bl __ct__10TBaseParamFP7TParamsUsPCc
/* 80367A58 00364998  93 BF 00 80 */	stw r29, 0x80(r31)
/* 80367A5C 0036499C  38 7E 00 9C */	addi r3, r30, 0x9c
/* 80367A60 003649A0  C0 02 0A 0C */	lfs f0, $$21574-_SDA2_BASE_(r2)
/* 80367A64 003649A4  D0 1F 00 90 */	stfs f0, 0x90(r31)
/* 80367A68 003649A8  93 9F 00 80 */	stw r28, 0x80(r31)
/* 80367A6C 003649AC  4B CD CC 21 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 80367A70 003649B0  38 A3 00 00 */	addi r5, r3, 0
/* 80367A74 003649B4  38 9F 00 00 */	addi r4, r31, 0
/* 80367A78 003649B8  38 7F 00 94 */	addi r3, r31, 0x94
/* 80367A7C 003649BC  38 DE 00 9C */	addi r6, r30, 0x9c
/* 80367A80 003649C0  4B D7 31 75 */	bl __ct__10TBaseParamFP7TParamsUsPCc
/* 80367A84 003649C4  93 BF 00 94 */	stw r29, 0x94(r31)
/* 80367A88 003649C8  38 7E 00 B0 */	addi r3, r30, 0xb0
/* 80367A8C 003649CC  C0 02 0A 10 */	lfs f0, $$21576-_SDA2_BASE_(r2)
/* 80367A90 003649D0  D0 1F 00 A4 */	stfs f0, 0xa4(r31)
/* 80367A94 003649D4  93 9F 00 94 */	stw r28, 0x94(r31)
/* 80367A98 003649D8  4B CD CB F5 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 80367A9C 003649DC  38 A3 00 00 */	addi r5, r3, 0
/* 80367AA0 003649E0  38 9F 00 00 */	addi r4, r31, 0
/* 80367AA4 003649E4  38 7F 00 A8 */	addi r3, r31, 0xa8
/* 80367AA8 003649E8  38 DE 00 B0 */	addi r6, r30, 0xb0
/* 80367AAC 003649EC  4B D7 31 49 */	bl __ct__10TBaseParamFP7TParamsUsPCc
/* 80367AB0 003649F0  93 BF 00 A8 */	stw r29, 0xa8(r31)
/* 80367AB4 003649F4  7F E3 FB 78 */	mr r3, r31
/* 80367AB8 003649F8  C0 02 0A 14 */	lfs f0, $$21578-_SDA2_BASE_(r2)
/* 80367ABC 003649FC  D0 1F 00 B8 */	stfs f0, 0xb8(r31)
/* 80367AC0 00364A00  93 9F 00 A8 */	stw r28, 0xa8(r31)
/* 80367AC4 00364A04  80 9F 00 00 */	lwz r4, 0(r31)
/* 80367AC8 00364A08  4B D9 04 D1 */	bl load__7TParamsFPCc
/* 80367ACC 00364A0C  80 01 00 24 */	lwz r0, 0x24(r1)
/* 80367AD0 00364A10  7F E3 FB 78 */	mr r3, r31
/* 80367AD4 00364A14  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 80367AD8 00364A18  83 C1 00 18 */	lwz r30, 0x18(r1)
/* 80367ADC 00364A1C  7C 08 03 A6 */	mtlr r0
/* 80367AE0 00364A20  83 A1 00 14 */	lwz r29, 0x14(r1)
/* 80367AE4 00364A24  83 81 00 10 */	lwz r28, 0x10(r1)
/* 80367AE8 00364A28  38 21 00 20 */	addi r1, r1, 0x20
/* 80367AEC 00364A2C  4E 80 00 20 */	blr 

.section .rodata, "wa"  # 0x8036FFA0 - 0x803A8380
$$21490:
	.incbin "baserom.dol", 0x3A4C38, 0xC
$$21526:
	.incbin "baserom.dol", 0x3A4C44, 0x14
$$21562:
	.incbin "baserom.dol", 0x3A4C58, 0x10
$$21563:
	.incbin "baserom.dol", 0x3A4C68, 0x10
$$21565:
	.incbin "baserom.dol", 0x3A4C78, 0x10
$$21567:
	.incbin "baserom.dol", 0x3A4C88, 0x14
$$21569:
	.incbin "baserom.dol", 0x3A4C9C, 0x14
$$21571:
	.incbin "baserom.dol", 0x3A4CB0, 0x10
$$21573:
	.incbin "baserom.dol", 0x3A4CC0, 0x14
$$21575:
	.incbin "baserom.dol", 0x3A4CD4, 0x14
$$21577:
	.incbin "baserom.dol", 0x3A4CE8, 0x18

.section .sdata2, "wa"  # 0x8040B460 - 0x80414020
$$21564:
	.incbin "baserom.dol", 0x3EC6F8, 0x4
$$21566:
	.incbin "baserom.dol", 0x3EC6FC, 0x4
$$21568:
	.incbin "baserom.dol", 0x3EC700, 0x4
$$21570:
	.incbin "baserom.dol", 0x3EC704, 0x4
$$21572:
	.incbin "baserom.dol", 0x3EC708, 0x4
$$21574:
	.incbin "baserom.dol", 0x3EC70C, 0x4
$$21576:
	.incbin "baserom.dol", 0x3EC710, 0x4
$$21578:
	.incbin "baserom.dol", 0x3EC714, 0x4

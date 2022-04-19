.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __ct__17TMapObjFloatOnSeaFPCc
__ct__17TMapObjFloatOnSeaFPCc:
/* 801B5848 001B2788  7C 08 02 A6 */	mflr r0
/* 801B584C 001B278C  90 01 00 04 */	stw r0, 4(r1)
/* 801B5850 001B2790  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 801B5854 001B2794  93 E1 00 14 */	stw r31, 0x14(r1)
/* 801B5858 001B2798  7C 7F 1B 78 */	mr r31, r3
/* 801B585C 001B279C  4B FE 60 71 */	bl __ct__10TLeanBlockFPCc
/* 801B5860 001B27A0  3C 60 80 3D */	lis r3, __vt__17TMapObjFloatOnSea@ha
/* 801B5864 001B27A4  38 63 98 E8 */	addi r3, r3, __vt__17TMapObjFloatOnSea@l
/* 801B5868 001B27A8  90 7F 00 00 */	stw r3, 0(r31)
/* 801B586C 001B27AC  38 63 00 24 */	addi r3, r3, 0x24
/* 801B5870 001B27B0  38 00 00 00 */	li r0, 0
/* 801B5874 001B27B4  90 7F 00 20 */	stw r3, 0x20(r31)
/* 801B5878 001B27B8  7F E3 FB 78 */	mr r3, r31
/* 801B587C 001B27BC  90 1F 01 94 */	stw r0, 0x194(r31)
/* 801B5880 001B27C0  90 1F 01 98 */	stw r0, 0x198(r31)
/* 801B5884 001B27C4  90 1F 01 9C */	stw r0, 0x19c(r31)
/* 801B5888 001B27C8  C0 02 C6 A0 */	lfs f0, "@2512"@sda21(r2)
/* 801B588C 001B27CC  D0 1F 01 A0 */	stfs f0, 0x1a0(r31)
/* 801B5890 001B27D0  D0 1F 01 A4 */	stfs f0, 0x1a4(r31)
/* 801B5894 001B27D4  D0 1F 01 A8 */	stfs f0, 0x1a8(r31)
/* 801B5898 001B27D8  D0 1F 01 AC */	stfs f0, 0x1ac(r31)
/* 801B589C 001B27DC  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 801B58A0 001B27E0  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 801B58A4 001B27E4  38 21 00 18 */	addi r1, r1, 0x18
/* 801B58A8 001B27E8  7C 08 03 A6 */	mtlr r0
/* 801B58AC 001B27EC  4E 80 00 20 */	blr 

.global initMapObj__17TMapObjFloatOnSeaFv
initMapObj__17TMapObjFloatOnSeaFv:
/* 801B58B0 001B27F0  7C 08 02 A6 */	mflr r0
/* 801B58B4 001B27F4  3C 80 80 39 */	lis r4, "@1490"@ha
/* 801B58B8 001B27F8  90 01 00 04 */	stw r0, 4(r1)
/* 801B58BC 001B27FC  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 801B58C0 001B2800  BF 61 00 14 */	stmw r27, 0x14(r1)
/* 801B58C4 001B2804  3B E3 00 00 */	addi r31, r3, 0
/* 801B58C8 001B2808  3B 84 F1 98 */	addi r28, r4, "@1490"@l
/* 801B58CC 001B280C  4B FE 60 A9 */	bl initMapObj__10TLeanBlockFv
/* 801B58D0 001B2810  3B C0 00 00 */	li r30, 0
/* 801B58D4 001B2814  48 00 00 08 */	b lbl_801B58DC
lbl_801B58D8:
/* 801B58D8 001B2818  3B DE 00 2C */	addi r30, r30, 0x2c
lbl_801B58DC:
/* 801B58DC 001B281C  7F BC F2 14 */	add r29, r28, r30
/* 801B58E0 001B2820  80 9F 00 F4 */	lwz r4, 0xf4(r31)
/* 801B58E4 001B2824  80 7D 01 58 */	lwz r3, 0x158(r29)
/* 801B58E8 001B2828  4B ED 03 75 */	bl strcmp
/* 801B58EC 001B282C  2C 03 00 00 */	cmpwi r3, 0
/* 801B58F0 001B2830  40 82 FF E8 */	bne lbl_801B58D8
/* 801B58F4 001B2834  C0 1D 01 5C */	lfs f0, 0x15c(r29)
/* 801B58F8 001B2838  3C 60 80 3F */	lis r3, gParticleFlagLoaded@ha
/* 801B58FC 001B283C  38 63 28 10 */	addi r3, r3, gParticleFlagLoaded@l
/* 801B5900 001B2840  D0 1F 01 40 */	stfs f0, 0x140(r31)
/* 801B5904 001B2844  38 00 00 0F */	li r0, 0xf
/* 801B5908 001B2848  3B 63 01 F6 */	addi r27, r3, 0x1f6
/* 801B590C 001B284C  C0 1D 01 60 */	lfs f0, 0x160(r29)
/* 801B5910 001B2850  D0 1F 01 44 */	stfs f0, 0x144(r31)
/* 801B5914 001B2854  C0 1D 01 64 */	lfs f0, 0x164(r29)
/* 801B5918 001B2858  D0 1F 01 48 */	stfs f0, 0x148(r31)
/* 801B591C 001B285C  C0 1D 01 68 */	lfs f0, 0x168(r29)
/* 801B5920 001B2860  D0 1F 01 38 */	stfs f0, 0x138(r31)
/* 801B5924 001B2864  C0 1D 01 6C */	lfs f0, 0x16c(r29)
/* 801B5928 001B2868  D0 1F 01 3C */	stfs f0, 0x13c(r31)
/* 801B592C 001B286C  90 1F 01 98 */	stw r0, 0x198(r31)
/* 801B5930 001B2870  C0 1D 01 80 */	lfs f0, 0x180(r29)
/* 801B5934 001B2874  D0 1F 01 AC */	stfs f0, 0x1ac(r31)
/* 801B5938 001B2878  88 03 01 F6 */	lbz r0, 0x1f6(r3)
/* 801B593C 001B287C  28 00 00 00 */	cmplwi r0, 0
/* 801B5940 001B2880  40 82 00 1C */	bne lbl_801B595C
/* 801B5944 001B2884  80 6D 98 50 */	lwz r3, gpResourceManager@sda21(r13)
/* 801B5948 001B2888  38 9C 03 68 */	addi r4, r28, 0x368
/* 801B594C 001B288C  38 A0 01 F6 */	li r5, 0x1f6
/* 801B5950 001B2890  4B EC B6 DD */	bl load__18JPAResourceManagerFPCcUs
/* 801B5954 001B2894  38 00 00 01 */	li r0, 1
/* 801B5958 001B2898  98 1B 00 00 */	stb r0, 0(r27)
lbl_801B595C:
/* 801B595C 001B289C  3C 60 80 3F */	lis r3, gParticleFlagLoaded@ha
/* 801B5960 001B28A0  38 63 28 10 */	addi r3, r3, gParticleFlagLoaded@l
/* 801B5964 001B28A4  3B 63 01 C6 */	addi r27, r3, 0x1c6
/* 801B5968 001B28A8  88 03 01 C6 */	lbz r0, 0x1c6(r3)
/* 801B596C 001B28AC  28 00 00 00 */	cmplwi r0, 0
/* 801B5970 001B28B0  40 82 00 1C */	bne lbl_801B598C
/* 801B5974 001B28B4  80 6D 98 50 */	lwz r3, gpResourceManager@sda21(r13)
/* 801B5978 001B28B8  38 9C 03 8C */	addi r4, r28, 0x38c
/* 801B597C 001B28BC  38 A0 01 C6 */	li r5, 0x1c6
/* 801B5980 001B28C0  4B EC B6 AD */	bl load__18JPAResourceManagerFPCcUs
/* 801B5984 001B28C4  38 00 00 01 */	li r0, 1
/* 801B5988 001B28C8  98 1B 00 00 */	stb r0, 0(r27)
lbl_801B598C:
/* 801B598C 001B28CC  38 60 00 18 */	li r3, 0x18
/* 801B5990 001B28D0  4B E5 6F 21 */	bl __nw__FUl
/* 801B5994 001B28D4  7C 7B 1B 79 */	or. r27, r3, r3
/* 801B5998 001B28D8  41 82 00 20 */	beq lbl_801B59B8
/* 801B599C 001B28DC  7C 9C F2 14 */	add r4, r28, r30
/* 801B59A0 001B28E0  C0 24 01 70 */	lfs f1, 0x170(r4)
/* 801B59A4 001B28E4  7F 63 DB 78 */	mr r3, r27
/* 801B59A8 001B28E8  C0 44 01 74 */	lfs f2, 0x174(r4)
/* 801B59AC 001B28EC  C0 64 01 78 */	lfs f3, 0x178(r4)
/* 801B59B0 001B28F0  C0 84 01 7C */	lfs f4, 0x17c(r4)
/* 801B59B4 001B28F4  4B FD AF C9 */	bl __ct__14TMapObjLibWaveFffff
lbl_801B59B8:
/* 801B59B8 001B28F8  93 7F 01 94 */	stw r27, 0x194(r31)
/* 801B59BC 001B28FC  BB 61 00 14 */	lmw r27, 0x14(r1)
/* 801B59C0 001B2900  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 801B59C4 001B2904  38 21 00 28 */	addi r1, r1, 0x28
/* 801B59C8 001B2908  7C 08 03 A6 */	mtlr r0
/* 801B59CC 001B290C  4E 80 00 20 */	blr 

.global calc__17TMapObjFloatOnSeaFv
calc__17TMapObjFloatOnSeaFv:
/* 801B59D0 001B2910  7C 08 02 A6 */	mflr r0
/* 801B59D4 001B2914  90 01 00 04 */	stw r0, 4(r1)
/* 801B59D8 001B2918  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 801B59DC 001B291C  93 E1 00 24 */	stw r31, 0x24(r1)
/* 801B59E0 001B2920  7C 7F 1B 78 */	mr r31, r3
/* 801B59E4 001B2924  C0 03 00 10 */	lfs f0, 0x10(r3)
/* 801B59E8 001B2928  D0 03 01 A0 */	stfs f0, 0x1a0(r3)
/* 801B59EC 001B292C  C0 02 C6 A0 */	lfs f0, "@2512"@sda21(r2)
/* 801B59F0 001B2930  D0 03 01 A4 */	stfs f0, 0x1a4(r3)
/* 801B59F4 001B2934  C0 03 00 18 */	lfs f0, 0x18(r3)
/* 801B59F8 001B2938  D0 03 01 A8 */	stfs f0, 0x1a8(r3)
/* 801B59FC 001B293C  80 63 01 9C */	lwz r3, 0x19c(r3)
/* 801B5A00 001B2940  80 1F 01 98 */	lwz r0, 0x198(r31)
/* 801B5A04 001B2944  7C 03 00 00 */	cmpw r3, r0
/* 801B5A08 001B2948  40 81 00 50 */	ble lbl_801B5A58
/* 801B5A0C 001B294C  C0 1F 01 AC */	lfs f0, 0x1ac(r31)
/* 801B5A10 001B2950  38 7F 00 00 */	addi r3, r31, 0
/* 801B5A14 001B2954  38 DF 01 A0 */	addi r6, r31, 0x1a0
/* 801B5A18 001B2958  D0 01 00 14 */	stfs f0, 0x14(r1)
/* 801B5A1C 001B295C  38 E1 00 14 */	addi r7, r1, 0x14
/* 801B5A20 001B2960  38 80 01 F6 */	li r4, 0x1f6
/* 801B5A24 001B2964  D0 01 00 18 */	stfs f0, 0x18(r1)
/* 801B5A28 001B2968  38 A0 00 03 */	li r5, 3
/* 801B5A2C 001B296C  D0 01 00 1C */	stfs f0, 0x1c(r1)
/* 801B5A30 001B2970  4B FD B0 AD */	bl "emitAndScale__11TMapObjBaseCFlUcPCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>"
/* 801B5A34 001B2974  38 7F 00 00 */	addi r3, r31, 0
/* 801B5A38 001B2978  38 DF 01 A0 */	addi r6, r31, 0x1a0
/* 801B5A3C 001B297C  38 E1 00 14 */	addi r7, r1, 0x14
/* 801B5A40 001B2980  38 80 01 C6 */	li r4, 0x1c6
/* 801B5A44 001B2984  38 A0 00 01 */	li r5, 1
/* 801B5A48 001B2988  4B FD B0 95 */	bl "emitAndScale__11TMapObjBaseCFlUcPCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>"
/* 801B5A4C 001B298C  38 00 00 00 */	li r0, 0
/* 801B5A50 001B2990  90 1F 01 9C */	stw r0, 0x19c(r31)
/* 801B5A54 001B2994  48 00 00 0C */	b lbl_801B5A60
lbl_801B5A58:
/* 801B5A58 001B2998  38 03 00 01 */	addi r0, r3, 1
/* 801B5A5C 001B299C  90 1F 01 9C */	stw r0, 0x19c(r31)
lbl_801B5A60:
/* 801B5A60 001B29A0  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 801B5A64 001B29A4  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 801B5A68 001B29A8  38 21 00 28 */	addi r1, r1, 0x28
/* 801B5A6C 001B29AC  7C 08 03 A6 */	mtlr r0
/* 801B5A70 001B29B0  4E 80 00 20 */	blr 

.global control__17TMapObjFloatOnSeaFv
control__17TMapObjFloatOnSeaFv:
/* 801B5A74 001B29B4  7C 08 02 A6 */	mflr r0
/* 801B5A78 001B29B8  90 01 00 04 */	stw r0, 4(r1)
/* 801B5A7C 001B29BC  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 801B5A80 001B29C0  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 801B5A84 001B29C4  93 C1 00 18 */	stw r30, 0x18(r1)
/* 801B5A88 001B29C8  7C 7E 1B 78 */	mr r30, r3
/* 801B5A8C 001B29CC  4B FE 61 25 */	bl control__10TLeanBlockFv
/* 801B5A90 001B29D0  80 7E 01 94 */	lwz r3, 0x194(r30)
/* 801B5A94 001B29D4  4B FD AF 91 */	bl movement__14TMapObjLibWaveFv
/* 801B5A98 001B29D8  88 0D 9A 64 */	lbz r0, init$2478@sda21(r13)
/* 801B5A9C 001B29DC  7C 00 07 75 */	extsb. r0, r0
/* 801B5AA0 001B29E0  40 82 00 14 */	bne lbl_801B5AB4
/* 801B5AA4 001B29E4  C0 02 C6 A0 */	lfs f0, "@2512"@sda21(r2)
/* 801B5AA8 001B29E8  38 00 00 01 */	li r0, 1
/* 801B5AAC 001B29EC  98 0D 9A 64 */	stb r0, init$2478@sda21(r13)
/* 801B5AB0 001B29F0  D0 0D 9A 60 */	stfs f0, timer$2477@sda21(r13)
lbl_801B5AB4:
/* 801B5AB4 001B29F4  7F C3 F3 78 */	mr r3, r30
/* 801B5AB8 001B29F8  4B F5 C7 B9 */	bl getModel__10TLiveActorCFv
/* 801B5ABC 001B29FC  83 E3 00 58 */	lwz r31, 0x58(r3)
/* 801B5AC0 001B2A00  80 7E 01 94 */	lwz r3, 0x194(r30)
/* 801B5AC4 001B2A04  C0 3E 00 10 */	lfs f1, 0x10(r30)
/* 801B5AC8 001B2A08  C0 5E 00 18 */	lfs f2, 0x18(r30)
/* 801B5ACC 001B2A0C  4B FD AF B1 */	bl getCurrentHeight__14TMapObjLibWaveCFff
/* 801B5AD0 001B2A10  C0 1F 00 1C */	lfs f0, 0x1c(r31)
/* 801B5AD4 001B2A14  EC 00 08 2A */	fadds f0, f0, f1
/* 801B5AD8 001B2A18  D0 1F 00 1C */	stfs f0, 0x1c(r31)
/* 801B5ADC 001B2A1C  80 01 00 24 */	lwz r0, 0x24(r1)
/* 801B5AE0 001B2A20  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 801B5AE4 001B2A24  83 C1 00 18 */	lwz r30, 0x18(r1)
/* 801B5AE8 001B2A28  38 21 00 20 */	addi r1, r1, 0x20
/* 801B5AEC 001B2A2C  7C 08 03 A6 */	mtlr r0
/* 801B5AF0 001B2A30  4E 80 00 20 */	blr 

.global touchPlayer__17TMapObjFloatOnSeaFP9THitActor
touchPlayer__17TMapObjFloatOnSeaFP9THitActor:
/* 801B5AF4 001B2A34  7C 08 02 A6 */	mflr r0
/* 801B5AF8 001B2A38  90 01 00 04 */	stw r0, 4(r1)
/* 801B5AFC 001B2A3C  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 801B5B00 001B2A40  93 E1 00 14 */	stw r31, 0x14(r1)
/* 801B5B04 001B2A44  7C 7F 1B 78 */	mr r31, r3
/* 801B5B08 001B2A48  4B FE 62 15 */	bl touchPlayer__10TLeanBlockFP9THitActor
/* 801B5B0C 001B2A4C  7F E3 FB 78 */	mr r3, r31
/* 801B5B10 001B2A50  4B FD B4 35 */	bl marioIsOn__11TMapObjBaseCFv
/* 801B5B14 001B2A54  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 801B5B18 001B2A58  41 82 00 0C */	beq lbl_801B5B24
/* 801B5B1C 001B2A5C  80 7F 01 94 */	lwz r3, 0x194(r31)
/* 801B5B20 001B2A60  4B FD AF 95 */	bl push__14TMapObjLibWaveFv
lbl_801B5B24:
/* 801B5B24 001B2A64  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 801B5B28 001B2A68  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 801B5B2C 001B2A6C  38 21 00 18 */	addi r1, r1, 0x18
/* 801B5B30 001B2A70  7C 08 03 A6 */	mtlr r0
/* 801B5B34 001B2A74  4E 80 00 20 */	blr 

.global __sinit_MapObjFloat_cpp
__sinit_MapObjFloat_cpp:
/* 801B5B38 001B2A78  7C 08 02 A6 */	mflr r0
/* 801B5B3C 001B2A7C  3C 60 80 3F */	lis r3, "@2557"@ha
/* 801B5B40 001B2A80  90 01 00 04 */	stw r0, 4(r1)
/* 801B5B44 001B2A84  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 801B5B48 001B2A88  93 E1 00 0C */	stw r31, 0xc(r1)
/* 801B5B4C 001B2A8C  3B E3 65 68 */	addi r31, r3, "@2557"@l
/* 801B5B50 001B2A90  88 0D 97 CC */	lbz r0, "__init__smList__15JALList<5MSBgm>"@sda21(r13)
/* 801B5B54 001B2A94  7C 00 07 75 */	extsb. r0, r0
/* 801B5B58 001B2A98  40 82 00 28 */	bne lbl_801B5B80
/* 801B5B5C 001B2A9C  38 6D 97 A8 */	addi r3, r13, "smList__15JALList<5MSBgm>"@sda21
/* 801B5B60 001B2AA0  4B E5 90 D5 */	bl initiate__10JSUPtrListFv
/* 801B5B64 001B2AA4  3C 60 80 0E */	lis r3, "__dt__15JSUList<5MSBgm>Fv"@ha
/* 801B5B68 001B2AA8  38 83 6A 44 */	addi r4, r3, "__dt__15JSUList<5MSBgm>Fv"@l
/* 801B5B6C 001B2AAC  38 6D 97 A8 */	addi r3, r13, "smList__15JALList<5MSBgm>"@sda21
/* 801B5B70 001B2AB0  38 BF 00 00 */	addi r5, r31, 0
/* 801B5B74 001B2AB4  4B EC CB B5 */	bl __register_global_object
/* 801B5B78 001B2AB8  38 00 00 01 */	li r0, 1
/* 801B5B7C 001B2ABC  98 0D 97 CC */	stb r0, "__init__smList__15JALList<5MSBgm>"@sda21(r13)
lbl_801B5B80:
/* 801B5B80 001B2AC0  88 0D 97 CD */	lbz r0, "__init__smList__24JALList<13MSSetSoundGrp>"@sda21(r13)
/* 801B5B84 001B2AC4  7C 00 07 75 */	extsb. r0, r0
/* 801B5B88 001B2AC8  40 82 00 28 */	bne lbl_801B5BB0
/* 801B5B8C 001B2ACC  38 6D 97 B4 */	addi r3, r13, "smList__24JALList<13MSSetSoundGrp>"@sda21
/* 801B5B90 001B2AD0  4B E5 90 A5 */	bl initiate__10JSUPtrListFv
/* 801B5B94 001B2AD4  3C 60 80 0E */	lis r3, "__dt__24JSUList<13MSSetSoundGrp>Fv"@ha
/* 801B5B98 001B2AD8  38 83 69 EC */	addi r4, r3, "__dt__24JSUList<13MSSetSoundGrp>Fv"@l
/* 801B5B9C 001B2ADC  38 6D 97 B4 */	addi r3, r13, "smList__24JALList<13MSSetSoundGrp>"@sda21
/* 801B5BA0 001B2AE0  38 BF 00 0C */	addi r5, r31, 0xc
/* 801B5BA4 001B2AE4  4B EC CB 85 */	bl __register_global_object
/* 801B5BA8 001B2AE8  38 00 00 01 */	li r0, 1
/* 801B5BAC 001B2AEC  98 0D 97 CD */	stb r0, "__init__smList__24JALList<13MSSetSoundGrp>"@sda21(r13)
lbl_801B5BB0:
/* 801B5BB0 001B2AF0  88 0D 97 CE */	lbz r0, "__init__smList__21JALList<10MSSetSound>"@sda21(r13)
/* 801B5BB4 001B2AF4  7C 00 07 75 */	extsb. r0, r0
/* 801B5BB8 001B2AF8  40 82 00 28 */	bne lbl_801B5BE0
/* 801B5BBC 001B2AFC  38 6D 97 C0 */	addi r3, r13, "smList__21JALList<10MSSetSound>"@sda21
/* 801B5BC0 001B2B00  4B E5 90 75 */	bl initiate__10JSUPtrListFv
/* 801B5BC4 001B2B04  3C 60 80 0E */	lis r3, "__dt__21JSUList<10MSSetSound>Fv"@ha
/* 801B5BC8 001B2B08  38 83 69 94 */	addi r4, r3, "__dt__21JSUList<10MSSetSound>Fv"@l
/* 801B5BCC 001B2B0C  38 6D 97 C0 */	addi r3, r13, "smList__21JALList<10MSSetSound>"@sda21
/* 801B5BD0 001B2B10  38 BF 00 18 */	addi r5, r31, 0x18
/* 801B5BD4 001B2B14  4B EC CB 55 */	bl __register_global_object
/* 801B5BD8 001B2B18  38 00 00 01 */	li r0, 1
/* 801B5BDC 001B2B1C  98 0D 97 CE */	stb r0, "__init__smList__21JALList<10MSSetSound>"@sda21(r13)
lbl_801B5BE0:
/* 801B5BE0 001B2B20  88 0D 8F 8C */	lbz r0, "__init__smList__26JALList<15JALSeModEffDGrp>"@sda21(r13)
/* 801B5BE4 001B2B24  7C 00 07 75 */	extsb. r0, r0
/* 801B5BE8 001B2B28  40 82 00 28 */	bne lbl_801B5C10
/* 801B5BEC 001B2B2C  38 6D 8E FC */	addi r3, r13, "smList__26JALList<15JALSeModEffDGrp>"@sda21
/* 801B5BF0 001B2B30  4B E5 90 45 */	bl initiate__10JSUPtrListFv
/* 801B5BF4 001B2B34  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModEffDGrp>Fv"@ha
/* 801B5BF8 001B2B38  38 83 A8 0C */	addi r4, r3, "__dt__26JSUList<15JALSeModEffDGrp>Fv"@l
/* 801B5BFC 001B2B3C  38 6D 8E FC */	addi r3, r13, "smList__26JALList<15JALSeModEffDGrp>"@sda21
/* 801B5C00 001B2B40  38 BF 00 24 */	addi r5, r31, 0x24
/* 801B5C04 001B2B44  4B EC CB 25 */	bl __register_global_object
/* 801B5C08 001B2B48  38 00 00 01 */	li r0, 1
/* 801B5C0C 001B2B4C  98 0D 8F 8C */	stb r0, "__init__smList__26JALList<15JALSeModEffDGrp>"@sda21(r13)
lbl_801B5C10:
/* 801B5C10 001B2B50  88 0D 8F 8D */	lbz r0, "__init__smList__26JALList<15JALSeModPitDGrp>"@sda21(r13)
/* 801B5C14 001B2B54  7C 00 07 75 */	extsb. r0, r0
/* 801B5C18 001B2B58  40 82 00 28 */	bne lbl_801B5C40
/* 801B5C1C 001B2B5C  38 6D 8F 08 */	addi r3, r13, "smList__26JALList<15JALSeModPitDGrp>"@sda21
/* 801B5C20 001B2B60  4B E5 90 15 */	bl initiate__10JSUPtrListFv
/* 801B5C24 001B2B64  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModPitDGrp>Fv"@ha
/* 801B5C28 001B2B68  38 83 A7 B4 */	addi r4, r3, "__dt__26JSUList<15JALSeModPitDGrp>Fv"@l
/* 801B5C2C 001B2B6C  38 6D 8F 08 */	addi r3, r13, "smList__26JALList<15JALSeModPitDGrp>"@sda21
/* 801B5C30 001B2B70  38 BF 00 30 */	addi r5, r31, 0x30
/* 801B5C34 001B2B74  4B EC CA F5 */	bl __register_global_object
/* 801B5C38 001B2B78  38 00 00 01 */	li r0, 1
/* 801B5C3C 001B2B7C  98 0D 8F 8D */	stb r0, "__init__smList__26JALList<15JALSeModPitDGrp>"@sda21(r13)
lbl_801B5C40:
/* 801B5C40 001B2B80  88 0D 8F 8E */	lbz r0, "__init__smList__26JALList<15JALSeModVolDGrp>"@sda21(r13)
/* 801B5C44 001B2B84  7C 00 07 75 */	extsb. r0, r0
/* 801B5C48 001B2B88  40 82 00 28 */	bne lbl_801B5C70
/* 801B5C4C 001B2B8C  38 6D 8F 14 */	addi r3, r13, "smList__26JALList<15JALSeModVolDGrp>"@sda21
/* 801B5C50 001B2B90  4B E5 8F E5 */	bl initiate__10JSUPtrListFv
/* 801B5C54 001B2B94  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModVolDGrp>Fv"@ha
/* 801B5C58 001B2B98  38 83 A7 5C */	addi r4, r3, "__dt__26JSUList<15JALSeModVolDGrp>Fv"@l
/* 801B5C5C 001B2B9C  38 6D 8F 14 */	addi r3, r13, "smList__26JALList<15JALSeModVolDGrp>"@sda21
/* 801B5C60 001B2BA0  38 BF 00 3C */	addi r5, r31, 0x3c
/* 801B5C64 001B2BA4  4B EC CA C5 */	bl __register_global_object
/* 801B5C68 001B2BA8  38 00 00 01 */	li r0, 1
/* 801B5C6C 001B2BAC  98 0D 8F 8E */	stb r0, "__init__smList__26JALList<15JALSeModVolDGrp>"@sda21(r13)
lbl_801B5C70:
/* 801B5C70 001B2BB0  88 0D 8F 8F */	lbz r0, "__init__smList__26JALList<15JALSeModEffFGrp>"@sda21(r13)
/* 801B5C74 001B2BB4  7C 00 07 75 */	extsb. r0, r0
/* 801B5C78 001B2BB8  40 82 00 28 */	bne lbl_801B5CA0
/* 801B5C7C 001B2BBC  38 6D 8F 20 */	addi r3, r13, "smList__26JALList<15JALSeModEffFGrp>"@sda21
/* 801B5C80 001B2BC0  4B E5 8F B5 */	bl initiate__10JSUPtrListFv
/* 801B5C84 001B2BC4  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModEffFGrp>Fv"@ha
/* 801B5C88 001B2BC8  38 83 A7 04 */	addi r4, r3, "__dt__26JSUList<15JALSeModEffFGrp>Fv"@l
/* 801B5C8C 001B2BCC  38 6D 8F 20 */	addi r3, r13, "smList__26JALList<15JALSeModEffFGrp>"@sda21
/* 801B5C90 001B2BD0  38 BF 00 48 */	addi r5, r31, 0x48
/* 801B5C94 001B2BD4  4B EC CA 95 */	bl __register_global_object
/* 801B5C98 001B2BD8  38 00 00 01 */	li r0, 1
/* 801B5C9C 001B2BDC  98 0D 8F 8F */	stb r0, "__init__smList__26JALList<15JALSeModEffFGrp>"@sda21(r13)
lbl_801B5CA0:
/* 801B5CA0 001B2BE0  88 0D 8F 90 */	lbz r0, "__init__smList__26JALList<15JALSeModPitFGrp>"@sda21(r13)
/* 801B5CA4 001B2BE4  7C 00 07 75 */	extsb. r0, r0
/* 801B5CA8 001B2BE8  40 82 00 28 */	bne lbl_801B5CD0
/* 801B5CAC 001B2BEC  38 6D 8F 2C */	addi r3, r13, "smList__26JALList<15JALSeModPitFGrp>"@sda21
/* 801B5CB0 001B2BF0  4B E5 8F 85 */	bl initiate__10JSUPtrListFv
/* 801B5CB4 001B2BF4  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModPitFGrp>Fv"@ha
/* 801B5CB8 001B2BF8  38 83 A6 AC */	addi r4, r3, "__dt__26JSUList<15JALSeModPitFGrp>Fv"@l
/* 801B5CBC 001B2BFC  38 6D 8F 2C */	addi r3, r13, "smList__26JALList<15JALSeModPitFGrp>"@sda21
/* 801B5CC0 001B2C00  38 BF 00 54 */	addi r5, r31, 0x54
/* 801B5CC4 001B2C04  4B EC CA 65 */	bl __register_global_object
/* 801B5CC8 001B2C08  38 00 00 01 */	li r0, 1
/* 801B5CCC 001B2C0C  98 0D 8F 90 */	stb r0, "__init__smList__26JALList<15JALSeModPitFGrp>"@sda21(r13)
lbl_801B5CD0:
/* 801B5CD0 001B2C10  88 0D 8F 91 */	lbz r0, "__init__smList__26JALList<15JALSeModVolFGrp>"@sda21(r13)
/* 801B5CD4 001B2C14  7C 00 07 75 */	extsb. r0, r0
/* 801B5CD8 001B2C18  40 82 00 28 */	bne lbl_801B5D00
/* 801B5CDC 001B2C1C  38 6D 8F 38 */	addi r3, r13, "smList__26JALList<15JALSeModVolFGrp>"@sda21
/* 801B5CE0 001B2C20  4B E5 8F 55 */	bl initiate__10JSUPtrListFv
/* 801B5CE4 001B2C24  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModVolFGrp>Fv"@ha
/* 801B5CE8 001B2C28  38 83 A6 54 */	addi r4, r3, "__dt__26JSUList<15JALSeModVolFGrp>Fv"@l
/* 801B5CEC 001B2C2C  38 6D 8F 38 */	addi r3, r13, "smList__26JALList<15JALSeModVolFGrp>"@sda21
/* 801B5CF0 001B2C30  38 BF 00 60 */	addi r5, r31, 0x60
/* 801B5CF4 001B2C34  4B EC CA 35 */	bl __register_global_object
/* 801B5CF8 001B2C38  38 00 00 01 */	li r0, 1
/* 801B5CFC 001B2C3C  98 0D 8F 91 */	stb r0, "__init__smList__26JALList<15JALSeModVolFGrp>"@sda21(r13)
lbl_801B5D00:
/* 801B5D00 001B2C40  88 0D 8F 92 */	lbz r0, "__init__smList__26JALList<15JALSeModEffDist>"@sda21(r13)
/* 801B5D04 001B2C44  7C 00 07 75 */	extsb. r0, r0
/* 801B5D08 001B2C48  40 82 00 28 */	bne lbl_801B5D30
/* 801B5D0C 001B2C4C  38 6D 8F 44 */	addi r3, r13, "smList__26JALList<15JALSeModEffDist>"@sda21
/* 801B5D10 001B2C50  4B E5 8F 25 */	bl initiate__10JSUPtrListFv
/* 801B5D14 001B2C54  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModEffDist>Fv"@ha
/* 801B5D18 001B2C58  38 83 A5 FC */	addi r4, r3, "__dt__26JSUList<15JALSeModEffDist>Fv"@l
/* 801B5D1C 001B2C5C  38 6D 8F 44 */	addi r3, r13, "smList__26JALList<15JALSeModEffDist>"@sda21
/* 801B5D20 001B2C60  38 BF 00 6C */	addi r5, r31, 0x6c
/* 801B5D24 001B2C64  4B EC CA 05 */	bl __register_global_object
/* 801B5D28 001B2C68  38 00 00 01 */	li r0, 1
/* 801B5D2C 001B2C6C  98 0D 8F 92 */	stb r0, "__init__smList__26JALList<15JALSeModEffDist>"@sda21(r13)
lbl_801B5D30:
/* 801B5D30 001B2C70  88 0D 8F 93 */	lbz r0, "__init__smList__26JALList<15JALSeModPitDist>"@sda21(r13)
/* 801B5D34 001B2C74  7C 00 07 75 */	extsb. r0, r0
/* 801B5D38 001B2C78  40 82 00 28 */	bne lbl_801B5D60
/* 801B5D3C 001B2C7C  38 6D 8F 50 */	addi r3, r13, "smList__26JALList<15JALSeModPitDist>"@sda21
/* 801B5D40 001B2C80  4B E5 8E F5 */	bl initiate__10JSUPtrListFv
/* 801B5D44 001B2C84  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModPitDist>Fv"@ha
/* 801B5D48 001B2C88  38 83 A5 A4 */	addi r4, r3, "__dt__26JSUList<15JALSeModPitDist>Fv"@l
/* 801B5D4C 001B2C8C  38 6D 8F 50 */	addi r3, r13, "smList__26JALList<15JALSeModPitDist>"@sda21
/* 801B5D50 001B2C90  38 BF 00 78 */	addi r5, r31, 0x78
/* 801B5D54 001B2C94  4B EC C9 D5 */	bl __register_global_object
/* 801B5D58 001B2C98  38 00 00 01 */	li r0, 1
/* 801B5D5C 001B2C9C  98 0D 8F 93 */	stb r0, "__init__smList__26JALList<15JALSeModPitDist>"@sda21(r13)
lbl_801B5D60:
/* 801B5D60 001B2CA0  88 0D 8F 94 */	lbz r0, "__init__smList__26JALList<15JALSeModVolDist>"@sda21(r13)
/* 801B5D64 001B2CA4  7C 00 07 75 */	extsb. r0, r0
/* 801B5D68 001B2CA8  40 82 00 28 */	bne lbl_801B5D90
/* 801B5D6C 001B2CAC  38 6D 8F 5C */	addi r3, r13, "smList__26JALList<15JALSeModVolDist>"@sda21
/* 801B5D70 001B2CB0  4B E5 8E C5 */	bl initiate__10JSUPtrListFv
/* 801B5D74 001B2CB4  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModVolDist>Fv"@ha
/* 801B5D78 001B2CB8  38 83 A5 4C */	addi r4, r3, "__dt__26JSUList<15JALSeModVolDist>Fv"@l
/* 801B5D7C 001B2CBC  38 6D 8F 5C */	addi r3, r13, "smList__26JALList<15JALSeModVolDist>"@sda21
/* 801B5D80 001B2CC0  38 BF 00 84 */	addi r5, r31, 0x84
/* 801B5D84 001B2CC4  4B EC C9 A5 */	bl __register_global_object
/* 801B5D88 001B2CC8  38 00 00 01 */	li r0, 1
/* 801B5D8C 001B2CCC  98 0D 8F 94 */	stb r0, "__init__smList__26JALList<15JALSeModVolDist>"@sda21(r13)
lbl_801B5D90:
/* 801B5D90 001B2CD0  88 0D 8F 95 */	lbz r0, "__init__smList__26JALList<15JALSeModEffFunk>"@sda21(r13)
/* 801B5D94 001B2CD4  7C 00 07 75 */	extsb. r0, r0
/* 801B5D98 001B2CD8  40 82 00 28 */	bne lbl_801B5DC0
/* 801B5D9C 001B2CDC  38 6D 8F 68 */	addi r3, r13, "smList__26JALList<15JALSeModEffFunk>"@sda21
/* 801B5DA0 001B2CE0  4B E5 8E 95 */	bl initiate__10JSUPtrListFv
/* 801B5DA4 001B2CE4  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModEffFunk>Fv"@ha
/* 801B5DA8 001B2CE8  38 83 A4 F4 */	addi r4, r3, "__dt__26JSUList<15JALSeModEffFunk>Fv"@l
/* 801B5DAC 001B2CEC  38 6D 8F 68 */	addi r3, r13, "smList__26JALList<15JALSeModEffFunk>"@sda21
/* 801B5DB0 001B2CF0  38 BF 00 90 */	addi r5, r31, 0x90
/* 801B5DB4 001B2CF4  4B EC C9 75 */	bl __register_global_object
/* 801B5DB8 001B2CF8  38 00 00 01 */	li r0, 1
/* 801B5DBC 001B2CFC  98 0D 8F 95 */	stb r0, "__init__smList__26JALList<15JALSeModEffFunk>"@sda21(r13)
lbl_801B5DC0:
/* 801B5DC0 001B2D00  88 0D 8F 96 */	lbz r0, "__init__smList__26JALList<15JALSeModPitFunk>"@sda21(r13)
/* 801B5DC4 001B2D04  7C 00 07 75 */	extsb. r0, r0
/* 801B5DC8 001B2D08  40 82 00 28 */	bne lbl_801B5DF0
/* 801B5DCC 001B2D0C  38 6D 8F 74 */	addi r3, r13, "smList__26JALList<15JALSeModPitFunk>"@sda21
/* 801B5DD0 001B2D10  4B E5 8E 65 */	bl initiate__10JSUPtrListFv
/* 801B5DD4 001B2D14  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModPitFunk>Fv"@ha
/* 801B5DD8 001B2D18  38 83 A4 9C */	addi r4, r3, "__dt__26JSUList<15JALSeModPitFunk>Fv"@l
/* 801B5DDC 001B2D1C  38 6D 8F 74 */	addi r3, r13, "smList__26JALList<15JALSeModPitFunk>"@sda21
/* 801B5DE0 001B2D20  38 BF 00 9C */	addi r5, r31, 0x9c
/* 801B5DE4 001B2D24  4B EC C9 45 */	bl __register_global_object
/* 801B5DE8 001B2D28  38 00 00 01 */	li r0, 1
/* 801B5DEC 001B2D2C  98 0D 8F 96 */	stb r0, "__init__smList__26JALList<15JALSeModPitFunk>"@sda21(r13)
lbl_801B5DF0:
/* 801B5DF0 001B2D30  88 0D 8F 97 */	lbz r0, "__init__smList__26JALList<15JALSeModVolFunk>"@sda21(r13)
/* 801B5DF4 001B2D34  7C 00 07 75 */	extsb. r0, r0
/* 801B5DF8 001B2D38  40 82 00 28 */	bne lbl_801B5E20
/* 801B5DFC 001B2D3C  38 6D 8F 80 */	addi r3, r13, "smList__26JALList<15JALSeModVolFunk>"@sda21
/* 801B5E00 001B2D40  4B E5 8E 35 */	bl initiate__10JSUPtrListFv
/* 801B5E04 001B2D44  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModVolFunk>Fv"@ha
/* 801B5E08 001B2D48  38 83 A4 44 */	addi r4, r3, "__dt__26JSUList<15JALSeModVolFunk>Fv"@l
/* 801B5E0C 001B2D4C  38 6D 8F 80 */	addi r3, r13, "smList__26JALList<15JALSeModVolFunk>"@sda21
/* 801B5E10 001B2D50  38 BF 00 A8 */	addi r5, r31, 0xa8
/* 801B5E14 001B2D54  4B EC C9 15 */	bl __register_global_object
/* 801B5E18 001B2D58  38 00 00 01 */	li r0, 1
/* 801B5E1C 001B2D5C  98 0D 8F 97 */	stb r0, "__init__smList__26JALList<15JALSeModVolFunk>"@sda21(r13)
lbl_801B5E20:
/* 801B5E20 001B2D60  80 01 00 14 */	lwz r0, 0x14(r1)
/* 801B5E24 001B2D64  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 801B5E28 001B2D68  38 21 00 10 */	addi r1, r1, 0x10
/* 801B5E2C 001B2D6C  7C 08 03 A6 */	mtlr r0
/* 801B5E30 001B2D70  4E 80 00 20 */	blr 

.global "@32@__dt__17TMapObjFloatOnSeaFv"
"@32@__dt__17TMapObjFloatOnSeaFv":
/* 801B5E34 001B2D74  38 63 FF E0 */	addi r3, r3, -32
/* 801B5E38 001B2D78  4B F4 ED D4 */	b __dt__17TMapObjFloatOnSeaFv

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
lbl_constructor:
  .4byte __sinit_MapObjFloat_cpp

.section .rodata, "a"  # 0x8036FFA0 - 0x803A8380
.balign 8
"@1490":
  .4byte 0
  .4byte 0
  .4byte 0
"@2111":
	.incbin "baserom.dol", 0x38C1A4, 0x14
"@2178":
	.incbin "baserom.dol", 0x38C1B8, 0x30
"@2179":
	.incbin "baserom.dol", 0x38C1E8, 0x38
"@2180":
	.incbin "baserom.dol", 0x38C220, 0x34
"@2181":
	.incbin "baserom.dol", 0x38C254, 0x24
"@2464":
	.incbin "baserom.dol", 0x38C278, 0xC
"@2467":
	.incbin "baserom.dol", 0x38C284, 0xC
"@2468":
	.incbin "baserom.dol", 0x38C290, 0xC
"@2469":
	.incbin "baserom.dol", 0x38C29C, 0xC
"@2470":
	.incbin "baserom.dol", 0x38C2A8, 0xC
"@2471":
	.incbin "baserom.dol", 0x38C2B4, 0xC
"@2472":
	.incbin "baserom.dol", 0x38C2C0, 0x10
"@2473":
	.incbin "baserom.dol", 0x38C2D0, 0x10
"@2474":
	.incbin "baserom.dol", 0x38C2E0, 0x10
param_table:
  .4byte "@2464"
  .4byte 0x3c23d70a
  .4byte 0x3c23d70a
  .4byte 0x3f000000
  .4byte 0x43160000
  .4byte 0x43c80000
  .4byte 0x42200000
  .4byte 0x42a00000
  .4byte 0x40400000
  .4byte 0x3f800000
  .4byte 0
  .4byte "@2465"
  .4byte 0x3c23d70a
  .4byte 0x3c23d70a
  .4byte 0x3dcccccd
  .4byte 0x43160000
  .4byte 0x43c80000
  .4byte 0x42200000
  .4byte 0x42a00000
  .4byte 0x40400000
  .4byte 0x3f800000
  .4byte 0x40c80000
  .4byte "@2466"
  .4byte 0x3c23d70a
  .4byte 0x3c23d70a
  .4byte 0x3e4ccccd
  .4byte 0x42c80000
  .4byte 0x42c80000
  .4byte 0x42200000
  .4byte 0x42200000
  .4byte 0x40000000
  .4byte 0x3f800000
  .4byte 0x404ccccd
  .4byte "@2467"
  .4byte 0x3c23d70a
  .4byte 0x3c23d70a
  .4byte 0x3e800000
  .4byte 0x42c80000
  .4byte 0x42c80000
  .4byte 0x42200000
  .4byte 0x42b40000
  .4byte 0x40400000
  .4byte 0x3f800000
  .4byte 0x40900000
  .4byte "@2468"
  .4byte 0x3a83126f
  .4byte 0x3ba3d70a
  .4byte 0x3dcccccd
  .4byte 0x437a0000
  .4byte 0x43c80000
  .4byte 0x42200000
  .4byte 0x42200000
  .4byte 0x40400000
  .4byte 0x3f800000
  .4byte 0
  .4byte "@2469"
  .4byte 0x3a83126f
  .4byte 0x3ba3d70a
  .4byte 0x3e19999a
  .4byte 0x437a0000
  .4byte 0x43c80000
  .4byte 0x42200000
  .4byte 0x42200000
  .4byte 0x40400000
  .4byte 0x3f800000
  .4byte 0
  .4byte "@2470"
  .4byte 0x3a83126f
  .4byte 0x3ba3d70a
  .4byte 0x3da3d70a
  .4byte 0x437a0000
  .4byte 0x43c80000
  .4byte 0x42200000
  .4byte 0x42200000
  .4byte 0x40400000
  .4byte 0x3f800000
  .4byte 0
  .4byte "@2471"
  .4byte 0x3a83126f
  .4byte 0x3ba3d70a
  .4byte 0x3d4ccccd
  .4byte 0x437a0000
  .4byte 0x43c80000
  .4byte 0x42200000
  .4byte 0x42200000
  .4byte 0x40400000
  .4byte 0x3f800000
  .4byte 0
  .4byte "@2472"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x447a0000
  .4byte 0x43fa0000
  .4byte 0x42c80000
  .4byte 0
  .4byte 0x3f800000
  .4byte 0x3f000000
  .4byte 0
  .4byte "@2473"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x447a0000
  .4byte 0x43fa0000
  .4byte 0x42c80000
  .4byte 0
  .4byte 0x3f800000
  .4byte 0x3f000000
  .4byte 0
  .4byte "@2474"
  .4byte 0x3c23d70a
  .4byte 0x3c23d70a
  .4byte 0x3e99999a
  .4byte 0x43160000
  .4byte 0x43c80000
  .4byte 0x42200000
  .4byte 0x42a00000
  .4byte 0x40400000
  .4byte 0x3f800000
  .4byte 0x40d00000
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
"@2539":
	.incbin "baserom.dol", 0x38C500, 0x24
"@2540":
	.incbin "baserom.dol", 0x38C524, 0x24

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__17TMapObjFloatOnSea
__vt__17TMapObjFloatOnSea:
  .4byte 0
  .4byte 0
  .4byte __dt__17TMapObjFloatOnSeaFv
  .4byte getType__Q26JDrama6TActorCFv
  .4byte load__11TMapObjBaseFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__11TMapObjBaseFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc
  .4byte perform__11TMapObjBaseFUlPQ26JDrama9TGraphics
  .4byte 0
  .4byte 0
  .4byte "@32@__dt__17TMapObjFloatOnSeaFv"
  .4byte JSGFGetType__Q26JStage6TActorCFv
  .4byte JSGGetName__Q26JStage7TObjectCFv
  .4byte JSGGetFlag__Q26JStage7TObjectCFv
  .4byte JSGSetFlag__Q26JStage7TObjectFUl
  .4byte JSGGetData__Q26JStage7TObjectCFUlPvUl
  .4byte JSGSetData__Q26JStage7TObjectFUlPCvUl
  .4byte JSGGetParent__Q26JStage7TObjectCFPPQ26JStage7TObjectPUl
  .4byte JSGSetParent__Q26JStage7TObjectFPQ26JStage7TObjectUl
  .4byte JSGSetRelation__Q26JStage7TObjectFbPQ26JStage7TObjectUl
  .4byte "@32@JSGGetTranslation__Q26JDrama6TActorCFP3Vec"
  .4byte "@32@JSGSetTranslation__Q26JDrama6TActorFRC3Vec"
  .4byte "@32@JSGGetScaling__Q26JDrama6TActorCFP3Vec"
  .4byte "@32@JSGSetScaling__Q26JDrama6TActorFRC3Vec"
  .4byte "@32@JSGGetRotation__Q26JDrama6TActorCFP3Vec"
  .4byte "@32@JSGSetRotation__Q26JDrama6TActorFRC3Vec"
  .4byte JSGGetShape__Q26JStage6TActorCFv
  .4byte JSGSetShape__Q26JStage6TActorFUl
  .4byte JSGGetAnimation__Q26JStage6TActorCFv
  .4byte JSGSetAnimation__Q26JStage6TActorFUl
  .4byte JSGGetAnimationFrame__Q26JStage6TActorCFv
  .4byte JSGSetAnimationFrame__Q26JStage6TActorFf
  .4byte JSGGetAnimationFrameMax__Q26JStage6TActorCFv
  .4byte JSGGetTranslation__Q26JDrama6TActorCFP3Vec
  .4byte JSGSetTranslation__Q26JDrama6TActorFRC3Vec
  .4byte JSGGetScaling__Q26JDrama6TActorCFP3Vec
  .4byte JSGSetScaling__Q26JDrama6TActorFRC3Vec
  .4byte JSGGetRotation__Q26JDrama6TActorCFP3Vec
  .4byte JSGSetRotation__Q26JDrama6TActorFRC3Vec
  .4byte receiveMessage__11TMapObjBaseFP9THitActorUl
  .4byte getTakingMtx__11TMapObjBaseFv
  .4byte ensureTakeSituation__11TMapObjBaseFv
  .4byte "moveRequest__10TTakeActorFRCQ29JGeometry8TVec3<f>"
  .4byte getRadiusAtY__11TMapObjBaseCFf
  .4byte belongToGround__10TLiveActorCFv
  .4byte getRootJointMtx__11TMapObjBaseCFv
  .4byte init__10TLiveActorFP12TLiveManager
  .4byte calcRootMatrix__11TMapObjBaseFv
  .4byte setGroundCollision__11TMapObjBaseFv
  .4byte control__17TMapObjFloatOnSeaFv
  .4byte bind__10TLiveActorFv
  .4byte moveObject__10TLiveActorFv
  .4byte requestShadow__10TLiveActorFv
  .4byte drawObject__10TLiveActorFPQ26JDrama9TGraphics
  .4byte performOnlyDraw__10TLiveActorFUlPQ26JDrama9TGraphics
  .4byte getShadowType__11TMapObjBaseFv
  .4byte kill__11TMapObjBaseFv
  .4byte getGravityY__10TLiveActorCFv
  .4byte hasMapCollision__10TLiveActorCFv
  .4byte getFocalPoint__10TLiveActorCFv
  .4byte updateAnmSound__10TLiveActorFv
  .4byte getBasNameTable__10TLiveActorCFv
  .4byte appear__11TMapObjBaseFv
  .4byte makeObjAppeared__11TMapObjBaseFv
  .4byte makeObjDead__11TMapObjBaseFv
  .4byte "changeObjSRT__11TMapObjBaseFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>"
  .4byte changeObjMtx__11TMapObjBaseFPA4_f
  .4byte updateObjMtx__11TMapObjBaseFv
  .4byte setUpCurrentMapCollision__11TMapObjBaseFv
  .4byte setObjHitData__11TMapObjBaseFUs
  .4byte setModelMtx__11TMapObjBaseFPA4_f
  .4byte initMapObj__17TMapObjFloatOnSeaFv
  .4byte loadBeforeInit__11TMapObjBaseFR20JSUMemoryInputStream
  .4byte initMapCollisionData__11TMapObjBaseFv
  .4byte makeMActors__11TMapObjBaseFv
  .4byte getSDLModelFlag__11TMapObjBaseCFv
  .4byte checkIllegalAttr__11TMapObjBaseCFv
  .4byte calc__17TMapObjFloatOnSeaFv
  .4byte draw__11TMapObjBaseCFv
  .4byte dead__11TMapObjBaseFv
  .4byte touchActor__11TMapObjBaseFP9THitActor
  .4byte touchPlayer__17TMapObjFloatOnSeaFP9THitActor
  .4byte touchWater__11TMapObjBaseFP9THitActor
  .4byte touchEnemy__11TMapObjBaseFP9THitActor
  .4byte touchBoss__11TMapObjBaseFP9THitActor
  .4byte makeObjDefault__11TMapObjBaseFv
  .4byte getHitObjNumMax__11TMapObjBaseFv
  .4byte getDepthAtFloating__11TMapObjBaseFv
  .4byte calcDefaultMtx__10TLeanBlockFv

.section .sdata2, "a"  # 0x8040B460 - 0x80414020
.balign 8
"@2465":
	.incbin "baserom.dol", 0x3E8390, 0x8
"@2466":
	.incbin "baserom.dol", 0x3E8398, 0x8
"@2512":
  .4byte 0
  .4byte 0

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
"@2557":
	.skip 0xC
"@2558":
	.skip 0xC
"@2559":
	.skip 0xC
"@2560":
	.skip 0xC
"@2561":
	.skip 0xC
"@2562":
	.skip 0xC
"@2563":
	.skip 0xC
"@2564":
	.skip 0xC
"@2565":
	.skip 0xC
"@2566":
	.skip 0xC
"@2567":
	.skip 0xC
"@2568":
	.skip 0xC
"@2569":
	.skip 0xC
"@2570":
	.skip 0xC
"@2571":
	.skip 0x10

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
timer$2477:
	.skip 0x4
init$2478:
	.skip 0x4

.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global getJ3DModelDataFlag__9TMapModelCFv
getJ3DModelDataFlag__9TMapModelCFv:
/* 801E6134 001E3074  3C 60 10 02 */	lis r3, 0x1002
/* 801E6138 001E3078  4E 80 00 20 */	blr 

.global __ct__16TMapModelManagerFPCc
__ct__16TMapModelManagerFPCc:
/* 801E613C 001E307C  7C 08 02 A6 */	mflr r0
/* 801E6140 001E3080  90 01 00 04 */	stw r0, 4(r1)
/* 801E6144 001E3084  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 801E6148 001E3088  93 E1 00 14 */	stw r31, 0x14(r1)
/* 801E614C 001E308C  7C 7F 1B 78 */	mr r31, r3
/* 801E6150 001E3090  4B FF 43 FD */	bl __ct__18TJointModelManagerFPCc
/* 801E6154 001E3094  3C 60 80 3D */	lis r3, __vt__16TMapModelManager@ha
/* 801E6158 001E3098  38 03 00 B8 */	addi r0, r3, __vt__16TMapModelManager@l
/* 801E615C 001E309C  90 1F 00 00 */	stw r0, 0(r31)
/* 801E6160 001E30A0  38 00 00 00 */	li r0, 0
/* 801E6164 001E30A4  38 7F 00 00 */	addi r3, r31, 0
/* 801E6168 001E30A8  90 1F 00 6C */	stw r0, 0x6c(r31)
/* 801E616C 001E30AC  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 801E6170 001E30B0  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 801E6174 001E30B4  38 21 00 18 */	addi r1, r1, 0x18
/* 801E6178 001E30B8  7C 08 03 A6 */	mtlr r0
/* 801E617C 001E30BC  4E 80 00 20 */	blr 

.global init__16TMapModelManagerFv
init__16TMapModelManagerFv:
/* 801E6180 001E30C0  7C 08 02 A6 */	mflr r0
/* 801E6184 001E30C4  3C 80 80 39 */	lis r4, $$22502@ha
/* 801E6188 001E30C8  90 01 00 04 */	stw r0, 4(r1)
/* 801E618C 001E30CC  38 00 00 01 */	li r0, 1
/* 801E6190 001E30D0  38 84 1B 10 */	addi r4, r4, $$22502@l
/* 801E6194 001E30D4  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 801E6198 001E30D8  93 E1 00 14 */	stw r31, 0x14(r1)
/* 801E619C 001E30DC  3B E3 00 00 */	addi r31, r3, 0
/* 801E61A0 001E30E0  93 C1 00 10 */	stw r30, 0x10(r1)
/* 801E61A4 001E30E4  90 03 00 10 */	stw r0, 0x10(r3)
/* 801E61A8 001E30E8  3C 60 80 3D */	lis r3, name_table$2428@ha
/* 801E61AC 001E30EC  38 A3 00 70 */	addi r5, r3, name_table$2428@l
/* 801E61B0 001E30F0  38 7F 00 00 */	addi r3, r31, 0
/* 801E61B4 001E30F4  4B FF 44 1D */	bl initJointModel__18TJointModelManagerFPCcPPCc
/* 801E61B8 001E30F8  38 60 00 64 */	li r3, 0x64
/* 801E61BC 001E30FC  4B E2 66 F5 */	bl __nw__FUl
/* 801E61C0 001E3100  7C 7E 1B 79 */	or. r30, r3, r3
/* 801E61C4 001E3104  41 82 00 0C */	beq lbl_801E61D0
/* 801E61C8 001E3108  7F C3 F3 78 */	mr r3, r30
/* 801E61CC 001E310C  4B FF 9B 6D */	bl __ct__19TMapCollisionStaticFv
lbl_801E61D0:
/* 801E61D0 001E3110  93 DF 00 6C */	stw r30, 0x6c(r31)
/* 801E61D4 001E3114  3C 60 80 39 */	lis r3, $$22503@ha
/* 801E61D8 001E3118  38 83 1B 20 */	addi r4, r3, $$22503@l
/* 801E61DC 001E311C  80 7F 00 6C */	lwz r3, 0x6c(r31)
/* 801E61E0 001E3120  38 A0 00 02 */	li r5, 2
/* 801E61E4 001E3124  38 C0 00 00 */	li r6, 0
/* 801E61E8 001E3128  81 83 00 00 */	lwz r12, 0(r3)
/* 801E61EC 001E312C  81 8C 00 08 */	lwz r12, 8(r12)
/* 801E61F0 001E3130  7D 88 03 A6 */	mtlr r12
/* 801E61F4 001E3134  4E 80 00 21 */	blrl 
/* 801E61F8 001E3138  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 801E61FC 001E313C  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 801E6200 001E3140  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 801E6204 001E3144  7C 08 03 A6 */	mtlr r0
/* 801E6208 001E3148  38 21 00 18 */	addi r1, r1, 0x18
/* 801E620C 001E314C  4E 80 00 20 */	blr 

.global initJointModel__9TMapModelFP18TJointModelManagerPCcP13MActorAnmData
initJointModel__9TMapModelFP18TJointModelManagerPCcP13MActorAnmData:
/* 801E6210 001E3150  7C 08 02 A6 */	mflr r0
/* 801E6214 001E3154  90 01 00 04 */	stw r0, 4(r1)
/* 801E6218 001E3158  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 801E621C 001E315C  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 801E6220 001E3160  3B E5 00 00 */	addi r31, r5, 0
/* 801E6224 001E3164  93 C1 00 18 */	stw r30, 0x18(r1)
/* 801E6228 001E3168  3B C3 00 00 */	addi r30, r3, 0
/* 801E622C 001E316C  4B FF 3C 85 */	bl initJointModel__11TJointModelFP18TJointModelManagerPCcP13MActorAnmData
/* 801E6230 001E3170  80 7E 00 2C */	lwz r3, 0x2c(r30)
/* 801E6234 001E3174  38 9F 00 00 */	addi r4, r31, 0
/* 801E6238 001E3178  38 A0 00 04 */	li r5, 4
/* 801E623C 001E317C  4B EE E0 0D */	bl checkAnmFileExist__6MActorFPCci
/* 801E6240 001E3180  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 801E6244 001E3184  41 82 00 10 */	beq lbl_801E6254
/* 801E6248 001E3188  80 7E 00 2C */	lwz r3, 0x2c(r30)
/* 801E624C 001E318C  7F E4 FB 78 */	mr r4, r31
/* 801E6250 001E3190  4B EE D6 C1 */	bl setBtk__6MActorFPCc
lbl_801E6254:
/* 801E6254 001E3194  80 7E 00 2C */	lwz r3, 0x2c(r30)
/* 801E6258 001E3198  38 9F 00 00 */	addi r4, r31, 0
/* 801E625C 001E319C  38 A0 00 05 */	li r5, 5
/* 801E6260 001E31A0  4B EE DF E9 */	bl checkAnmFileExist__6MActorFPCci
/* 801E6264 001E31A4  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 801E6268 001E31A8  41 82 00 10 */	beq lbl_801E6278
/* 801E626C 001E31AC  80 7E 00 2C */	lwz r3, 0x2c(r30)
/* 801E6270 001E31B0  7F E4 FB 78 */	mr r4, r31
/* 801E6274 001E31B4  4B EE D2 8D */	bl setBrk__6MActorFPCc
lbl_801E6278:
/* 801E6278 001E31B8  80 7E 00 2C */	lwz r3, 0x2c(r30)
/* 801E627C 001E31BC  4B EE E7 C5 */	bl calc__6MActorFv
/* 801E6280 001E31C0  7F C3 F3 78 */	mr r3, r30
/* 801E6284 001E31C4  48 00 00 1D */	bl initUnderpass__9TMapModelFv
/* 801E6288 001E31C8  80 01 00 24 */	lwz r0, 0x24(r1)
/* 801E628C 001E31CC  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 801E6290 001E31D0  83 C1 00 18 */	lwz r30, 0x18(r1)
/* 801E6294 001E31D4  7C 08 03 A6 */	mtlr r0
/* 801E6298 001E31D8  38 21 00 20 */	addi r1, r1, 0x20
/* 801E629C 001E31DC  4E 80 00 20 */	blr 

.global initUnderpass__9TMapModelFv
initUnderpass__9TMapModelFv:
/* 801E62A0 001E31E0  7C 08 02 A6 */	mflr r0
/* 801E62A4 001E31E4  90 01 00 04 */	stw r0, 4(r1)
/* 801E62A8 001E31E8  94 21 FF 90 */	stwu r1, -0x70(r1)
/* 801E62AC 001E31EC  93 E1 00 6C */	stw r31, 0x6c(r1)
/* 801E62B0 001E31F0  7C 7F 1B 78 */	mr r31, r3
/* 801E62B4 001E31F4  93 C1 00 68 */	stw r30, 0x68(r1)
/* 801E62B8 001E31F8  80 A3 00 24 */	lwz r5, 0x24(r3)
/* 801E62BC 001E31FC  3C 60 80 39 */	lis r3, $$22571@ha
/* 801E62C0 001E3200  38 83 1B 34 */	addi r4, r3, $$22571@l
/* 801E62C4 001E3204  80 65 00 B0 */	lwz r3, 0xb0(r5)
/* 801E62C8 001E3208  4B E2 CD 29 */	bl getIndex__10JUTNameTabCFPCc
/* 801E62CC 001E320C  2C 03 00 00 */	cmpwi r3, 0
/* 801E62D0 001E3210  41 80 01 5C */	blt lbl_801E642C
/* 801E62D4 001E3214  80 9F 00 24 */	lwz r4, 0x24(r31)
/* 801E62D8 001E3218  54 60 13 BA */	rlwinm r0, r3, 2, 0xe, 0x1d
/* 801E62DC 001E321C  80 BF 00 10 */	lwz r5, 0x10(r31)
/* 801E62E0 001E3220  38 E0 00 00 */	li r7, 0
/* 801E62E4 001E3224  80 64 00 20 */	lwz r3, 0x20(r4)
/* 801E62E8 001E3228  7C C3 00 2E */	lwzx r6, r3, r0
/* 801E62EC 001E322C  38 60 00 00 */	li r3, 0
/* 801E62F0 001E3230  48 00 00 0C */	b lbl_801E62FC
lbl_801E62F4:
/* 801E62F4 001E3234  38 E7 00 01 */	addi r7, r7, 1
/* 801E62F8 001E3238  38 63 00 04 */	addi r3, r3, 4
lbl_801E62FC:
/* 801E62FC 001E323C  7C 07 28 00 */	cmpw r7, r5
/* 801E6300 001E3240  40 80 00 18 */	bge lbl_801E6318
/* 801E6304 001E3244  80 9F 00 14 */	lwz r4, 0x14(r31)
/* 801E6308 001E3248  7C 84 18 2E */	lwzx r4, r4, r3
/* 801E630C 001E324C  80 04 00 0C */	lwz r0, 0xc(r4)
/* 801E6310 001E3250  7C 00 30 40 */	cmplw r0, r6
/* 801E6314 001E3254  40 82 FF E0 */	bne lbl_801E62F4
lbl_801E6318:
/* 801E6318 001E3258  80 7F 00 14 */	lwz r3, 0x14(r31)
/* 801E631C 001E325C  54 E0 10 3A */	slwi r0, r7, 2
/* 801E6320 001E3260  7C 03 00 2E */	lwzx r0, r3, r0
/* 801E6324 001E3264  90 1F 00 30 */	stw r0, 0x30(r31)
/* 801E6328 001E3268  80 06 00 60 */	lwz r0, 0x60(r6)
/* 801E632C 001E326C  90 1F 00 34 */	stw r0, 0x34(r31)
/* 801E6330 001E3270  80 7F 00 34 */	lwz r3, 0x34(r31)
/* 801E6334 001E3274  4B E3 EF 2D */	bl change__11J3DMaterialFv
/* 801E6338 001E3278  80 7F 00 34 */	lwz r3, 0x34(r31)
/* 801E633C 001E327C  38 80 00 00 */	li r4, 0
/* 801E6340 001E3280  80 03 00 1C */	lwz r0, 0x1c(r3)
/* 801E6344 001E3284  60 00 00 01 */	ori r0, r0, 1
/* 801E6348 001E3288  90 03 00 1C */	stw r0, 0x1c(r3)
/* 801E634C 001E328C  80 7F 00 34 */	lwz r3, 0x34(r31)
/* 801E6350 001E3290  80 63 00 24 */	lwz r3, 0x24(r3)
/* 801E6354 001E3294  81 83 00 00 */	lwz r12, 0(r3)
/* 801E6358 001E3298  81 8C 00 28 */	lwz r12, 0x28(r12)
/* 801E635C 001E329C  7D 88 03 A6 */	mtlr r12
/* 801E6360 001E32A0  4E 80 00 21 */	blrl 
/* 801E6364 001E32A4  38 00 00 01 */	li r0, 1
/* 801E6368 001E32A8  98 03 00 00 */	stb r0, 0(r3)
/* 801E636C 001E32AC  38 80 00 00 */	li r4, 0
/* 801E6370 001E32B0  38 00 00 1E */	li r0, 0x1e
/* 801E6374 001E32B4  98 83 00 01 */	stb r4, 1(r3)
/* 801E6378 001E32B8  98 03 00 02 */	stb r0, 2(r3)
/* 801E637C 001E32BC  38 60 00 C4 */	li r3, 0xc4
/* 801E6380 001E32C0  4B E2 65 31 */	bl __nw__FUl
/* 801E6384 001E32C4  7C 7E 1B 79 */	or. r30, r3, r3
/* 801E6388 001E32C8  41 82 00 14 */	beq lbl_801E639C
/* 801E638C 001E32CC  3C 60 80 3B */	lis r3, j3dDefaultTexMtxInfo@ha
/* 801E6390 001E32D0  38 83 9C F8 */	addi r4, r3, j3dDefaultTexMtxInfo@l
/* 801E6394 001E32D4  38 7E 00 00 */	addi r3, r30, 0
/* 801E6398 001E32D8  4B E3 D3 25 */	bl __as__13J3DTexMtxInfoFRC13J3DTexMtxInfo
lbl_801E639C:
/* 801E639C 001E32DC  38 00 00 02 */	li r0, 2
/* 801E63A0 001E32E0  98 1E 00 01 */	stb r0, 1(r30)
/* 801E63A4 001E32E4  7F C5 F3 78 */	mr r5, r30
/* 801E63A8 001E32E8  38 80 00 00 */	li r4, 0
/* 801E63AC 001E32EC  80 7F 00 34 */	lwz r3, 0x34(r31)
/* 801E63B0 001E32F0  80 63 00 24 */	lwz r3, 0x24(r3)
/* 801E63B4 001E32F4  81 83 00 00 */	lwz r12, 0(r3)
/* 801E63B8 001E32F8  81 8C 00 2C */	lwz r12, 0x2c(r12)
/* 801E63BC 001E32FC  7D 88 03 A6 */	mtlr r12
/* 801E63C0 001E3300  4E 80 00 21 */	blrl 
/* 801E63C4 001E3304  80 7F 00 34 */	lwz r3, 0x34(r31)
/* 801E63C8 001E3308  80 63 00 30 */	lwz r3, 0x30(r3)
/* 801E63CC 001E330C  81 83 00 00 */	lwz r12, 0(r3)
/* 801E63D0 001E3310  81 8C 00 40 */	lwz r12, 0x40(r12)
/* 801E63D4 001E3314  7D 88 03 A6 */	mtlr r12
/* 801E63D8 001E3318  4E 80 00 21 */	blrl 
/* 801E63DC 001E331C  A0 03 00 00 */	lhz r0, 0(r3)
/* 801E63E0 001E3320  3C 80 80 40 */	lis r4, j3dZModeTable@ha
/* 801E63E4 001E3324  38 A4 3D 4C */	addi r5, r4, j3dZModeTable@l
/* 801E63E8 001E3328  1C 00 00 03 */	mulli r0, r0, 3
/* 801E63EC 001E332C  7C 85 02 14 */	add r4, r5, r0
/* 801E63F0 001E3330  88 04 00 00 */	lbz r0, 0(r4)
/* 801E63F4 001E3334  88 84 00 02 */	lbz r4, 2(r4)
/* 801E63F8 001E3338  54 00 20 36 */	slwi r0, r0, 4
/* 801E63FC 001E333C  7C 80 22 14 */	add r4, r0, r4
/* 801E6400 001E3340  38 04 00 0E */	addi r0, r4, 0xe
/* 801E6404 001E3344  B0 03 00 00 */	sth r0, 0(r3)
/* 801E6408 001E3348  A0 03 00 00 */	lhz r0, 0(r3)
/* 801E640C 001E334C  1C 00 00 03 */	mulli r0, r0, 3
/* 801E6410 001E3350  7C A5 02 14 */	add r5, r5, r0
/* 801E6414 001E3354  88 85 00 01 */	lbz r4, 1(r5)
/* 801E6418 001E3358  88 05 00 00 */	lbz r0, 0(r5)
/* 801E641C 001E335C  54 84 08 3C */	slwi r4, r4, 1
/* 801E6420 001E3360  54 00 20 36 */	slwi r0, r0, 4
/* 801E6424 001E3364  7C 04 02 14 */	add r0, r4, r0
/* 801E6428 001E3368  B0 03 00 00 */	sth r0, 0(r3)
lbl_801E642C:
/* 801E642C 001E336C  80 01 00 74 */	lwz r0, 0x74(r1)
/* 801E6430 001E3370  83 E1 00 6C */	lwz r31, 0x6c(r1)
/* 801E6434 001E3374  83 C1 00 68 */	lwz r30, 0x68(r1)
/* 801E6438 001E3378  7C 08 03 A6 */	mtlr r0
/* 801E643C 001E337C  38 21 00 70 */	addi r1, r1, 0x70
/* 801E6440 001E3380  4E 80 00 20 */	blr 

.global perform__9TMapModelFUlPQ26JDrama9TGraphics
perform__9TMapModelFUlPQ26JDrama9TGraphics:
/* 801E6444 001E3384  7C 08 02 A6 */	mflr r0
/* 801E6448 001E3388  90 01 00 04 */	stw r0, 4(r1)
/* 801E644C 001E338C  94 21 FF 28 */	stwu r1, -0xd8(r1)
/* 801E6450 001E3390  BF 61 00 C4 */	stmw r27, 0xc4(r1)
/* 801E6454 001E3394  7C 7D 1B 78 */	mr r29, r3
/* 801E6458 001E3398  3B C4 00 00 */	addi r30, r4, 0
/* 801E645C 001E339C  3B E5 00 00 */	addi r31, r5, 0
/* 801E6460 001E33A0  80 03 00 08 */	lwz r0, 8(r3)
/* 801E6464 001E33A4  54 00 07 FF */	clrlwi. r0, r0, 0x1f
/* 801E6468 001E33A8  41 82 00 0C */	beq lbl_801E6474
/* 801E646C 001E33AC  38 00 00 01 */	li r0, 1
/* 801E6470 001E33B0  48 00 00 08 */	b lbl_801E6478
lbl_801E6474:
/* 801E6474 001E33B4  38 00 00 00 */	li r0, 0
lbl_801E6478:
/* 801E6478 001E33B8  54 00 06 3F */	clrlwi. r0, r0, 0x18
/* 801E647C 001E33BC  40 82 01 54 */	bne lbl_801E65D0
/* 801E6480 001E33C0  57 DB 07 BD */	rlwinm. r27, r30, 0, 0x1e, 0x1e
/* 801E6484 001E33C4  41 82 01 28 */	beq lbl_801E65AC
/* 801E6488 001E33C8  80 1D 00 30 */	lwz r0, 0x30(r29)
/* 801E648C 001E33CC  28 00 00 00 */	cmplwi r0, 0
/* 801E6490 001E33D0  41 82 01 1C */	beq lbl_801E65AC
/* 801E6494 001E33D4  80 6D 98 FC */	lwz r3, gpMarioFlag@sda21(r13)
/* 801E6498 001E33D8  80 03 00 00 */	lwz r0, 0(r3)
/* 801E649C 001E33DC  54 00 07 BD */	rlwinm. r0, r0, 0, 0x1e, 0x1e
/* 801E64A0 001E33E0  41 82 00 0C */	beq lbl_801E64AC
/* 801E64A4 001E33E4  38 00 00 01 */	li r0, 1
/* 801E64A8 001E33E8  48 00 00 08 */	b lbl_801E64B0
lbl_801E64AC:
/* 801E64AC 001E33EC  38 00 00 00 */	li r0, 0
lbl_801E64B0:
/* 801E64B0 001E33F0  54 00 06 3F */	clrlwi. r0, r0, 0x18
/* 801E64B4 001E33F4  41 82 00 E4 */	beq lbl_801E6598
/* 801E64B8 001E33F8  83 8D 98 DC */	lwz r28, gpMarioPos@sda21(r13)
/* 801E64BC 001E33FC  4B F6 C7 61 */	bl SMS_GetMarioGrLevel__Fv
/* 801E64C0 001E3400  C0 42 CE BC */	lfs f2, $$22618@sda21(r2)
/* 801E64C4 001E3404  C0 1C 00 04 */	lfs f0, 4(r28)
/* 801E64C8 001E3408  EC 22 08 2A */	fadds f1, f2, f1
/* 801E64CC 001E340C  FC 00 08 40 */	fcmpo cr0, f0, f1
/* 801E64D0 001E3410  40 80 00 C8 */	bge lbl_801E6598
/* 801E64D4 001E3414  80 7D 00 30 */	lwz r3, 0x30(r29)
/* 801E64D8 001E3418  81 83 00 00 */	lwz r12, 0(r3)
/* 801E64DC 001E341C  81 8C 00 10 */	lwz r12, 0x10(r12)
/* 801E64E0 001E3420  7D 88 03 A6 */	mtlr r12
/* 801E64E4 001E3424  4E 80 00 21 */	blrl 
/* 801E64E8 001E3428  80 6D A8 B0 */	lwz r3, gpCamera@sda21(r13)
/* 801E64EC 001E342C  38 81 00 18 */	addi r4, r1, 0x18
/* 801E64F0 001E3430  81 83 00 00 */	lwz r12, 0(r3)
/* 801E64F4 001E3434  81 8C 00 DC */	lwz r12, 0xdc(r12)
/* 801E64F8 001E3438  7D 88 03 A6 */	mtlr r12
/* 801E64FC 001E343C  4E 80 00 21 */	blrl 
/* 801E6500 001E3440  80 6D A8 B0 */	lwz r3, gpCamera@sda21(r13)
/* 801E6504 001E3444  38 81 00 24 */	addi r4, r1, 0x24
/* 801E6508 001E3448  81 83 00 00 */	lwz r12, 0(r3)
/* 801E650C 001E344C  81 8C 00 E4 */	lwz r12, 0xe4(r12)
/* 801E6510 001E3450  7D 88 03 A6 */	mtlr r12
/* 801E6514 001E3454  4E 80 00 21 */	blrl 
/* 801E6518 001E3458  C0 7D 00 38 */	lfs f3, 0x38(r29)
/* 801E651C 001E345C  38 61 00 60 */	addi r3, r1, 0x60
/* 801E6520 001E3460  C0 22 CE C0 */	lfs f1, $$22619@sda21(r2)
/* 801E6524 001E3464  C0 02 CE C4 */	lfs f0, $$22620@sda21(r2)
/* 801E6528 001E3468  C0 A2 CE B4 */	lfs f5, $$22505@sda21(r2)
/* 801E652C 001E346C  EC 41 00 F2 */	fmuls f2, f1, f3
/* 801E6530 001E3470  EC 20 00 F2 */	fmuls f1, f0, f3
/* 801E6534 001E3474  FC C0 28 90 */	fmr f6, f5
/* 801E6538 001E3478  FC 60 10 90 */	fmr f3, f2
/* 801E653C 001E347C  FC 80 08 90 */	fmr f4, f1
/* 801E6540 001E3480  FC E0 28 90 */	fmr f7, f5
/* 801E6544 001E3484  FD 00 28 90 */	fmr f8, f5
/* 801E6548 001E3488  4B EA E6 21 */	bl C_MTXLightOrtho
/* 801E654C 001E348C  80 CD 98 DC */	lwz r6, gpMarioPos@sda21(r13)
/* 801E6550 001E3490  38 61 00 90 */	addi r3, r1, 0x90
/* 801E6554 001E3494  38 81 00 18 */	addi r4, r1, 0x18
/* 801E6558 001E3498  38 A1 00 24 */	addi r5, r1, 0x24
/* 801E655C 001E349C  4B EA E3 21 */	bl C_MTXLookAt
/* 801E6560 001E34A0  38 61 00 60 */	addi r3, r1, 0x60
/* 801E6564 001E34A4  38 81 00 90 */	addi r4, r1, 0x90
/* 801E6568 001E34A8  38 A1 00 30 */	addi r5, r1, 0x30
/* 801E656C 001E34AC  4B EA DD A5 */	bl PSMTXConcat
/* 801E6570 001E34B0  80 7D 00 34 */	lwz r3, 0x34(r29)
/* 801E6574 001E34B4  38 80 00 00 */	li r4, 0
/* 801E6578 001E34B8  80 63 00 24 */	lwz r3, 0x24(r3)
/* 801E657C 001E34BC  81 83 00 00 */	lwz r12, 0(r3)
/* 801E6580 001E34C0  81 8C 00 34 */	lwz r12, 0x34(r12)
/* 801E6584 001E34C4  7D 88 03 A6 */	mtlr r12
/* 801E6588 001E34C8  4E 80 00 21 */	blrl 
/* 801E658C 001E34CC  38 81 00 30 */	addi r4, r1, 0x30
/* 801E6590 001E34D0  4B F9 03 29 */	bl setEffectMtx__13J3DTexMtxInfoFPA4_f
/* 801E6594 001E34D4  48 00 00 18 */	b lbl_801E65AC
lbl_801E6598:
/* 801E6598 001E34D8  80 7D 00 30 */	lwz r3, 0x30(r29)
/* 801E659C 001E34DC  81 83 00 00 */	lwz r12, 0(r3)
/* 801E65A0 001E34E0  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 801E65A4 001E34E4  7D 88 03 A6 */	mtlr r12
/* 801E65A8 001E34E8  4E 80 00 21 */	blrl 
lbl_801E65AC:
/* 801E65AC 001E34EC  28 1B 00 00 */	cmplwi r27, 0
/* 801E65B0 001E34F0  41 82 00 10 */	beq lbl_801E65C0
/* 801E65B4 001E34F4  80 7D 00 2C */	lwz r3, 0x2c(r29)
/* 801E65B8 001E34F8  57 DE 07 FA */	rlwinm r30, r30, 0, 0x1f, 0x1d
/* 801E65BC 001E34FC  4B EE E1 69 */	bl frameUpdate__6MActorFv
lbl_801E65C0:
/* 801E65C0 001E3500  80 7D 00 2C */	lwz r3, 0x2c(r29)
/* 801E65C4 001E3504  38 9E 00 00 */	addi r4, r30, 0
/* 801E65C8 001E3508  38 BF 00 00 */	addi r5, r31, 0
/* 801E65CC 001E350C  4B EE DF 0D */	bl perform__6MActorFUlPQ26JDrama9TGraphics
lbl_801E65D0:
/* 801E65D0 001E3510  BB 61 00 C4 */	lmw r27, 0xc4(r1)
/* 801E65D4 001E3514  80 01 00 DC */	lwz r0, 0xdc(r1)
/* 801E65D8 001E3518  38 21 00 D8 */	addi r1, r1, 0xd8
/* 801E65DC 001E351C  7C 08 03 A6 */	mtlr r0
/* 801E65E0 001E3520  4E 80 00 20 */	blr 

.global __dt__16TMapModelManagerFv
__dt__16TMapModelManagerFv:
/* 801E65E4 001E3524  7C 08 02 A6 */	mflr r0
/* 801E65E8 001E3528  90 01 00 04 */	stw r0, 4(r1)
/* 801E65EC 001E352C  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 801E65F0 001E3530  93 E1 00 14 */	stw r31, 0x14(r1)
/* 801E65F4 001E3534  3B E4 00 00 */	addi r31, r4, 0
/* 801E65F8 001E3538  93 C1 00 10 */	stw r30, 0x10(r1)
/* 801E65FC 001E353C  7C 7E 1B 79 */	or. r30, r3, r3
/* 801E6600 001E3540  41 82 00 64 */	beq lbl_801E6664
/* 801E6604 001E3544  3C 60 80 3D */	lis r3, __vt__16TMapModelManager@ha
/* 801E6608 001E3548  38 03 00 B8 */	addi r0, r3, __vt__16TMapModelManager@l
/* 801E660C 001E354C  90 1E 00 00 */	stw r0, 0(r30)
/* 801E6610 001E3550  41 82 00 44 */	beq lbl_801E6654
/* 801E6614 001E3554  3C 60 80 3D */	lis r3, __vt__18TJointModelManager@ha
/* 801E6618 001E3558  38 63 FB B8 */	addi r3, r3, __vt__18TJointModelManager@l
/* 801E661C 001E355C  34 1E 00 20 */	addic. r0, r30, 0x20
/* 801E6620 001E3560  90 7E 00 00 */	stw r3, 0(r30)
/* 801E6624 001E3564  41 82 00 10 */	beq lbl_801E6634
/* 801E6628 001E3568  38 7E 00 3C */	addi r3, r30, 0x3c
/* 801E662C 001E356C  38 80 FF FF */	li r4, -1
/* 801E6630 001E3570  4B EF 18 39 */	bl __dt__Q27JGadget68TList$$016MActorSubAnmInfo$$4Q27JGadget30TAllocator$$016MActorSubAnmInfo$$1$$1Fv
lbl_801E6634:
/* 801E6634 001E3574  28 1E 00 00 */	cmplwi r30, 0
/* 801E6638 001E3578  41 82 00 1C */	beq lbl_801E6654
/* 801E663C 001E357C  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TViewObj@ha
/* 801E6640 001E3580  38 03 A0 C0 */	addi r0, r3, __vt__Q26JDrama8TViewObj@l
/* 801E6644 001E3584  90 1E 00 00 */	stw r0, 0(r30)
/* 801E6648 001E3588  38 7E 00 00 */	addi r3, r30, 0
/* 801E664C 001E358C  38 80 00 00 */	li r4, 0
/* 801E6650 001E3590  4B E5 E1 AD */	bl __dt__Q26JDrama8TNameRefFv
lbl_801E6654:
/* 801E6654 001E3594  7F E0 07 35 */	extsh. r0, r31
/* 801E6658 001E3598  40 81 00 0C */	ble lbl_801E6664
/* 801E665C 001E359C  7F C3 F3 78 */	mr r3, r30
/* 801E6660 001E35A0  4B E2 64 51 */	bl __dl__FPv
lbl_801E6664:
/* 801E6664 001E35A4  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 801E6668 001E35A8  7F C3 F3 78 */	mr r3, r30
/* 801E666C 001E35AC  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 801E6670 001E35B0  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 801E6674 001E35B4  7C 08 03 A6 */	mtlr r0
/* 801E6678 001E35B8  38 21 00 18 */	addi r1, r1, 0x18
/* 801E667C 001E35BC  4E 80 00 20 */	blr 

.global newJointModel__16TMapModelManagerCFi
newJointModel__16TMapModelManagerCFi:
/* 801E6680 001E35C0  7C 08 02 A6 */	mflr r0
/* 801E6684 001E35C4  38 60 00 40 */	li r3, 0x40
/* 801E6688 001E35C8  90 01 00 04 */	stw r0, 4(r1)
/* 801E668C 001E35CC  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 801E6690 001E35D0  93 E1 00 14 */	stw r31, 0x14(r1)
/* 801E6694 001E35D4  4B E2 62 1D */	bl __nw__FUl
/* 801E6698 001E35D8  7C 7F 1B 79 */	or. r31, r3, r3
/* 801E669C 001E35DC  41 82 00 34 */	beq lbl_801E66D0
/* 801E66A0 001E35E0  7F E3 FB 78 */	mr r3, r31
/* 801E66A4 001E35E4  4B FF 37 BD */	bl __ct__11TJointModelFv
/* 801E66A8 001E35E8  3C 60 80 3D */	lis r3, __vt__9TMapModel@ha
/* 801E66AC 001E35EC  38 03 00 84 */	addi r0, r3, __vt__9TMapModel@l
/* 801E66B0 001E35F0  90 1F 00 00 */	stw r0, 0(r31)
/* 801E66B4 001E35F4  38 00 00 00 */	li r0, 0
/* 801E66B8 001E35F8  90 1F 00 30 */	stw r0, 0x30(r31)
/* 801E66BC 001E35FC  90 1F 00 34 */	stw r0, 0x34(r31)
/* 801E66C0 001E3600  C0 02 CE B4 */	lfs f0, $$22505@sda21(r2)
/* 801E66C4 001E3604  D0 1F 00 38 */	stfs f0, 0x38(r31)
/* 801E66C8 001E3608  C0 02 CE B8 */	lfs f0, $$22506@sda21(r2)
/* 801E66CC 001E360C  D0 1F 00 3C */	stfs f0, 0x3c(r31)
lbl_801E66D0:
/* 801E66D0 001E3610  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 801E66D4 001E3614  7F E3 FB 78 */	mr r3, r31
/* 801E66D8 001E3618  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 801E66DC 001E361C  38 21 00 18 */	addi r1, r1, 0x18
/* 801E66E0 001E3620  7C 08 03 A6 */	mtlr r0
/* 801E66E4 001E3624  4E 80 00 20 */	blr 

.global __sinit_MapModel_cpp
__sinit_MapModel_cpp:
/* 801E66E8 001E3628  7C 08 02 A6 */	mflr r0
/* 801E66EC 001E362C  3C 60 80 3F */	lis r3, $$22668@ha
/* 801E66F0 001E3630  90 01 00 04 */	stw r0, 4(r1)
/* 801E66F4 001E3634  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 801E66F8 001E3638  93 E1 00 0C */	stw r31, 0xc(r1)
/* 801E66FC 001E363C  3B E3 7D A8 */	addi r31, r3, $$22668@l
/* 801E6700 001E3640  88 0D 97 CC */	lbz r0, __init__smList__15JALList$$05MSBgm$$1@sda21(r13)
/* 801E6704 001E3644  7C 00 07 75 */	extsb. r0, r0
/* 801E6708 001E3648  40 82 00 28 */	bne lbl_801E6730
/* 801E670C 001E364C  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1@sda21
/* 801E6710 001E3650  4B E2 85 25 */	bl initiate__10JSUPtrListFv
/* 801E6714 001E3654  3C 60 80 0E */	lis r3, __dt__15JSUList$$05MSBgm$$1Fv@ha
/* 801E6718 001E3658  38 83 6A 44 */	addi r4, r3, __dt__15JSUList$$05MSBgm$$1Fv@l
/* 801E671C 001E365C  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1@sda21
/* 801E6720 001E3660  38 BF 00 00 */	addi r5, r31, 0
/* 801E6724 001E3664  4B E9 C0 05 */	bl __register_global_object
/* 801E6728 001E3668  38 00 00 01 */	li r0, 1
/* 801E672C 001E366C  98 0D 97 CC */	stb r0, __init__smList__15JALList$$05MSBgm$$1@sda21(r13)
lbl_801E6730:
/* 801E6730 001E3670  88 0D 97 CD */	lbz r0, __init__smList__24JALList$$013MSSetSoundGrp$$1@sda21(r13)
/* 801E6734 001E3674  7C 00 07 75 */	extsb. r0, r0
/* 801E6738 001E3678  40 82 00 28 */	bne lbl_801E6760
/* 801E673C 001E367C  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1@sda21
/* 801E6740 001E3680  4B E2 84 F5 */	bl initiate__10JSUPtrListFv
/* 801E6744 001E3684  3C 60 80 0E */	lis r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@ha
/* 801E6748 001E3688  38 83 69 EC */	addi r4, r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@l
/* 801E674C 001E368C  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1@sda21
/* 801E6750 001E3690  38 BF 00 0C */	addi r5, r31, 0xc
/* 801E6754 001E3694  4B E9 BF D5 */	bl __register_global_object
/* 801E6758 001E3698  38 00 00 01 */	li r0, 1
/* 801E675C 001E369C  98 0D 97 CD */	stb r0, __init__smList__24JALList$$013MSSetSoundGrp$$1@sda21(r13)
lbl_801E6760:
/* 801E6760 001E36A0  88 0D 97 CE */	lbz r0, __init__smList__21JALList$$010MSSetSound$$1@sda21(r13)
/* 801E6764 001E36A4  7C 00 07 75 */	extsb. r0, r0
/* 801E6768 001E36A8  40 82 00 28 */	bne lbl_801E6790
/* 801E676C 001E36AC  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1@sda21
/* 801E6770 001E36B0  4B E2 84 C5 */	bl initiate__10JSUPtrListFv
/* 801E6774 001E36B4  3C 60 80 0E */	lis r3, __dt__21JSUList$$010MSSetSound$$1Fv@ha
/* 801E6778 001E36B8  38 83 69 94 */	addi r4, r3, __dt__21JSUList$$010MSSetSound$$1Fv@l
/* 801E677C 001E36BC  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1@sda21
/* 801E6780 001E36C0  38 BF 00 18 */	addi r5, r31, 0x18
/* 801E6784 001E36C4  4B E9 BF A5 */	bl __register_global_object
/* 801E6788 001E36C8  38 00 00 01 */	li r0, 1
/* 801E678C 001E36CC  98 0D 97 CE */	stb r0, __init__smList__21JALList$$010MSSetSound$$1@sda21(r13)
lbl_801E6790:
/* 801E6790 001E36D0  88 0D 8F 8C */	lbz r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1@sda21(r13)
/* 801E6794 001E36D4  7C 00 07 75 */	extsb. r0, r0
/* 801E6798 001E36D8  40 82 00 28 */	bne lbl_801E67C0
/* 801E679C 001E36DC  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1@sda21
/* 801E67A0 001E36E0  4B E2 84 95 */	bl initiate__10JSUPtrListFv
/* 801E67A4 001E36E4  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@ha
/* 801E67A8 001E36E8  38 83 A8 0C */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@l
/* 801E67AC 001E36EC  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1@sda21
/* 801E67B0 001E36F0  38 BF 00 24 */	addi r5, r31, 0x24
/* 801E67B4 001E36F4  4B E9 BF 75 */	bl __register_global_object
/* 801E67B8 001E36F8  38 00 00 01 */	li r0, 1
/* 801E67BC 001E36FC  98 0D 8F 8C */	stb r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1@sda21(r13)
lbl_801E67C0:
/* 801E67C0 001E3700  88 0D 8F 8D */	lbz r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1@sda21(r13)
/* 801E67C4 001E3704  7C 00 07 75 */	extsb. r0, r0
/* 801E67C8 001E3708  40 82 00 28 */	bne lbl_801E67F0
/* 801E67CC 001E370C  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1@sda21
/* 801E67D0 001E3710  4B E2 84 65 */	bl initiate__10JSUPtrListFv
/* 801E67D4 001E3714  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@ha
/* 801E67D8 001E3718  38 83 A7 B4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@l
/* 801E67DC 001E371C  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1@sda21
/* 801E67E0 001E3720  38 BF 00 30 */	addi r5, r31, 0x30
/* 801E67E4 001E3724  4B E9 BF 45 */	bl __register_global_object
/* 801E67E8 001E3728  38 00 00 01 */	li r0, 1
/* 801E67EC 001E372C  98 0D 8F 8D */	stb r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1@sda21(r13)
lbl_801E67F0:
/* 801E67F0 001E3730  88 0D 8F 8E */	lbz r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1@sda21(r13)
/* 801E67F4 001E3734  7C 00 07 75 */	extsb. r0, r0
/* 801E67F8 001E3738  40 82 00 28 */	bne lbl_801E6820
/* 801E67FC 001E373C  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1@sda21
/* 801E6800 001E3740  4B E2 84 35 */	bl initiate__10JSUPtrListFv
/* 801E6804 001E3744  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@ha
/* 801E6808 001E3748  38 83 A7 5C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@l
/* 801E680C 001E374C  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1@sda21
/* 801E6810 001E3750  38 BF 00 3C */	addi r5, r31, 0x3c
/* 801E6814 001E3754  4B E9 BF 15 */	bl __register_global_object
/* 801E6818 001E3758  38 00 00 01 */	li r0, 1
/* 801E681C 001E375C  98 0D 8F 8E */	stb r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1@sda21(r13)
lbl_801E6820:
/* 801E6820 001E3760  88 0D 8F 8F */	lbz r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1@sda21(r13)
/* 801E6824 001E3764  7C 00 07 75 */	extsb. r0, r0
/* 801E6828 001E3768  40 82 00 28 */	bne lbl_801E6850
/* 801E682C 001E376C  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1@sda21
/* 801E6830 001E3770  4B E2 84 05 */	bl initiate__10JSUPtrListFv
/* 801E6834 001E3774  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@ha
/* 801E6838 001E3778  38 83 A7 04 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@l
/* 801E683C 001E377C  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1@sda21
/* 801E6840 001E3780  38 BF 00 48 */	addi r5, r31, 0x48
/* 801E6844 001E3784  4B E9 BE E5 */	bl __register_global_object
/* 801E6848 001E3788  38 00 00 01 */	li r0, 1
/* 801E684C 001E378C  98 0D 8F 8F */	stb r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1@sda21(r13)
lbl_801E6850:
/* 801E6850 001E3790  88 0D 8F 90 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1@sda21(r13)
/* 801E6854 001E3794  7C 00 07 75 */	extsb. r0, r0
/* 801E6858 001E3798  40 82 00 28 */	bne lbl_801E6880
/* 801E685C 001E379C  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1@sda21
/* 801E6860 001E37A0  4B E2 83 D5 */	bl initiate__10JSUPtrListFv
/* 801E6864 001E37A4  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@ha
/* 801E6868 001E37A8  38 83 A6 AC */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@l
/* 801E686C 001E37AC  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1@sda21
/* 801E6870 001E37B0  38 BF 00 54 */	addi r5, r31, 0x54
/* 801E6874 001E37B4  4B E9 BE B5 */	bl __register_global_object
/* 801E6878 001E37B8  38 00 00 01 */	li r0, 1
/* 801E687C 001E37BC  98 0D 8F 90 */	stb r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1@sda21(r13)
lbl_801E6880:
/* 801E6880 001E37C0  88 0D 8F 91 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1@sda21(r13)
/* 801E6884 001E37C4  7C 00 07 75 */	extsb. r0, r0
/* 801E6888 001E37C8  40 82 00 28 */	bne lbl_801E68B0
/* 801E688C 001E37CC  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1@sda21
/* 801E6890 001E37D0  4B E2 83 A5 */	bl initiate__10JSUPtrListFv
/* 801E6894 001E37D4  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@ha
/* 801E6898 001E37D8  38 83 A6 54 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@l
/* 801E689C 001E37DC  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1@sda21
/* 801E68A0 001E37E0  38 BF 00 60 */	addi r5, r31, 0x60
/* 801E68A4 001E37E4  4B E9 BE 85 */	bl __register_global_object
/* 801E68A8 001E37E8  38 00 00 01 */	li r0, 1
/* 801E68AC 001E37EC  98 0D 8F 91 */	stb r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1@sda21(r13)
lbl_801E68B0:
/* 801E68B0 001E37F0  88 0D 8F 92 */	lbz r0, __init__smList__26JALList$$015JALSeModEffDist$$1@sda21(r13)
/* 801E68B4 001E37F4  7C 00 07 75 */	extsb. r0, r0
/* 801E68B8 001E37F8  40 82 00 28 */	bne lbl_801E68E0
/* 801E68BC 001E37FC  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1@sda21
/* 801E68C0 001E3800  4B E2 83 75 */	bl initiate__10JSUPtrListFv
/* 801E68C4 001E3804  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@ha
/* 801E68C8 001E3808  38 83 A5 FC */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@l
/* 801E68CC 001E380C  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1@sda21
/* 801E68D0 001E3810  38 BF 00 6C */	addi r5, r31, 0x6c
/* 801E68D4 001E3814  4B E9 BE 55 */	bl __register_global_object
/* 801E68D8 001E3818  38 00 00 01 */	li r0, 1
/* 801E68DC 001E381C  98 0D 8F 92 */	stb r0, __init__smList__26JALList$$015JALSeModEffDist$$1@sda21(r13)
lbl_801E68E0:
/* 801E68E0 001E3820  88 0D 8F 93 */	lbz r0, __init__smList__26JALList$$015JALSeModPitDist$$1@sda21(r13)
/* 801E68E4 001E3824  7C 00 07 75 */	extsb. r0, r0
/* 801E68E8 001E3828  40 82 00 28 */	bne lbl_801E6910
/* 801E68EC 001E382C  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1@sda21
/* 801E68F0 001E3830  4B E2 83 45 */	bl initiate__10JSUPtrListFv
/* 801E68F4 001E3834  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@ha
/* 801E68F8 001E3838  38 83 A5 A4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@l
/* 801E68FC 001E383C  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1@sda21
/* 801E6900 001E3840  38 BF 00 78 */	addi r5, r31, 0x78
/* 801E6904 001E3844  4B E9 BE 25 */	bl __register_global_object
/* 801E6908 001E3848  38 00 00 01 */	li r0, 1
/* 801E690C 001E384C  98 0D 8F 93 */	stb r0, __init__smList__26JALList$$015JALSeModPitDist$$1@sda21(r13)
lbl_801E6910:
/* 801E6910 001E3850  88 0D 8F 94 */	lbz r0, __init__smList__26JALList$$015JALSeModVolDist$$1@sda21(r13)
/* 801E6914 001E3854  7C 00 07 75 */	extsb. r0, r0
/* 801E6918 001E3858  40 82 00 28 */	bne lbl_801E6940
/* 801E691C 001E385C  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1@sda21
/* 801E6920 001E3860  4B E2 83 15 */	bl initiate__10JSUPtrListFv
/* 801E6924 001E3864  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@ha
/* 801E6928 001E3868  38 83 A5 4C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@l
/* 801E692C 001E386C  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1@sda21
/* 801E6930 001E3870  38 BF 00 84 */	addi r5, r31, 0x84
/* 801E6934 001E3874  4B E9 BD F5 */	bl __register_global_object
/* 801E6938 001E3878  38 00 00 01 */	li r0, 1
/* 801E693C 001E387C  98 0D 8F 94 */	stb r0, __init__smList__26JALList$$015JALSeModVolDist$$1@sda21(r13)
lbl_801E6940:
/* 801E6940 001E3880  88 0D 8F 95 */	lbz r0, __init__smList__26JALList$$015JALSeModEffFunk$$1@sda21(r13)
/* 801E6944 001E3884  7C 00 07 75 */	extsb. r0, r0
/* 801E6948 001E3888  40 82 00 28 */	bne lbl_801E6970
/* 801E694C 001E388C  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1@sda21
/* 801E6950 001E3890  4B E2 82 E5 */	bl initiate__10JSUPtrListFv
/* 801E6954 001E3894  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@ha
/* 801E6958 001E3898  38 83 A4 F4 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@l
/* 801E695C 001E389C  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1@sda21
/* 801E6960 001E38A0  38 BF 00 90 */	addi r5, r31, 0x90
/* 801E6964 001E38A4  4B E9 BD C5 */	bl __register_global_object
/* 801E6968 001E38A8  38 00 00 01 */	li r0, 1
/* 801E696C 001E38AC  98 0D 8F 95 */	stb r0, __init__smList__26JALList$$015JALSeModEffFunk$$1@sda21(r13)
lbl_801E6970:
/* 801E6970 001E38B0  88 0D 8F 96 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFunk$$1@sda21(r13)
/* 801E6974 001E38B4  7C 00 07 75 */	extsb. r0, r0
/* 801E6978 001E38B8  40 82 00 28 */	bne lbl_801E69A0
/* 801E697C 001E38BC  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1@sda21
/* 801E6980 001E38C0  4B E2 82 B5 */	bl initiate__10JSUPtrListFv
/* 801E6984 001E38C4  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@ha
/* 801E6988 001E38C8  38 83 A4 9C */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@l
/* 801E698C 001E38CC  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1@sda21
/* 801E6990 001E38D0  38 BF 00 9C */	addi r5, r31, 0x9c
/* 801E6994 001E38D4  4B E9 BD 95 */	bl __register_global_object
/* 801E6998 001E38D8  38 00 00 01 */	li r0, 1
/* 801E699C 001E38DC  98 0D 8F 96 */	stb r0, __init__smList__26JALList$$015JALSeModPitFunk$$1@sda21(r13)
lbl_801E69A0:
/* 801E69A0 001E38E0  88 0D 8F 97 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFunk$$1@sda21(r13)
/* 801E69A4 001E38E4  7C 00 07 75 */	extsb. r0, r0
/* 801E69A8 001E38E8  40 82 00 28 */	bne lbl_801E69D0
/* 801E69AC 001E38EC  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1@sda21
/* 801E69B0 001E38F0  4B E2 82 85 */	bl initiate__10JSUPtrListFv
/* 801E69B4 001E38F4  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@ha
/* 801E69B8 001E38F8  38 83 A4 44 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@l
/* 801E69BC 001E38FC  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1@sda21
/* 801E69C0 001E3900  38 BF 00 A8 */	addi r5, r31, 0xa8
/* 801E69C4 001E3904  4B E9 BD 65 */	bl __register_global_object
/* 801E69C8 001E3908  38 00 00 01 */	li r0, 1
/* 801E69CC 001E390C  98 0D 8F 97 */	stb r0, __init__smList__26JALList$$015JALSeModVolFunk$$1@sda21(r13)
lbl_801E69D0:
/* 801E69D0 001E3910  80 01 00 14 */	lwz r0, 0x14(r1)
/* 801E69D4 001E3914  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 801E69D8 001E3918  38 21 00 10 */	addi r1, r1, 0x10
/* 801E69DC 001E391C  7C 08 03 A6 */	mtlr r0
/* 801E69E0 001E3920  4E 80 00 20 */	blr 

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
  .4byte __sinit_MapModel_cpp

.section .rodata, "wa"  # 0x8036FFA0 - 0x803A8380
$$22502:
	.incbin "baserom.dol", 0x38EB10, 0x10
$$22503:
	.incbin "baserom.dol", 0x38EB20, 0x14
$$22571:
	.incbin "baserom.dol", 0x38EB34, 0xC

.section .data, "wa"  # 0x803A8380 - 0x803E6000
name_table$2428:
	.incbin "baserom.dol", 0x3CD070, 0x14
.global __vt__9TMapModel
__vt__9TMapModel:
  .4byte 0
  .4byte 0
  .4byte initJointObj__9TJointObjFP8J3DJoint
  .4byte alive__9TJointObjFv
  .4byte awake__9TJointObjFv
  .4byte stand__9TJointObjFv
  .4byte kill__9TJointObjFv
  .4byte sleep__9TJointObjFv
  .4byte sit__9TJointObjFv
  .4byte newJointObj__9TJointObjCFv
  .4byte initJointModel__9TMapModelFP18TJointModelManagerPCcP13MActorAnmData
  .4byte perform__9TMapModelFUlPQ26JDrama9TGraphics
  .4byte getJ3DModelDataFlag__9TMapModelCFv
.global __vt__16TMapModelManager
__vt__16TMapModelManager:
  .4byte 0
  .4byte 0
  .4byte __dt__16TMapModelManagerFv
  .4byte getType__Q26JDrama8TNameRefCFv
  .4byte load__Q26JDrama8TNameRefFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__Q26JDrama8TNameRefFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc
  .4byte perform__18TJointModelManagerFUlPQ26JDrama9TGraphics
  .4byte newJointModel__16TMapModelManagerCFi

.section .sdata2, "wa"  # 0x8040B460 - 0x80414020
$$22429:
	.incbin "baserom.dol", 0x3E8B98, 0x4
$$22430:
	.incbin "baserom.dol", 0x3E8B9C, 0x8
$$22431:
	.incbin "baserom.dol", 0x3E8BA4, 0x8
$$22432:
	.incbin "baserom.dol", 0x3E8BAC, 0x8
$$22505:
	.incbin "baserom.dol", 0x3E8BB4, 0x4
$$22506:
	.incbin "baserom.dol", 0x3E8BB8, 0x4
$$22618:
	.incbin "baserom.dol", 0x3E8BBC, 0x4
$$22619:
	.incbin "baserom.dol", 0x3E8BC0, 0x4
$$22620:
	.incbin "baserom.dol", 0x3E8BC4, 0x4

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
$$22668:
	.skip 0xC
$$22669:
	.skip 0xC
$$22670:
	.skip 0xC
$$22671:
	.skip 0xC
$$22672:
	.skip 0xC
$$22673:
	.skip 0xC
$$22674:
	.skip 0xC
$$22675:
	.skip 0xC
$$22676:
	.skip 0xC
$$22677:
	.skip 0xC
$$22678:
	.skip 0xC
$$22679:
	.skip 0xC
$$22680:
	.skip 0xC
$$22681:
	.skip 0xC
$$22682:
	.skip 0x10

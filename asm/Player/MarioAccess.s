.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global SMS_IsMarioRoofing__Fv
SMS_IsMarioRoofing__Fv:
/* 80152934 0014F874  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 80152938 0014F878  80 03 00 7C */	lwz r0, 0x7c(r3)
/* 8015293C 0014F87C  54 00 05 FE */	clrlwi r0, r0, 0x17
/* 80152940 0014F880  28 00 01 47 */	cmplwi r0, 0x147
/* 80152944 0014F884  41 80 00 14 */	blt lbl_80152958
/* 80152948 0014F888  28 00 01 4A */	cmplwi r0, 0x14a
/* 8015294C 0014F88C  41 81 00 0C */	bgt lbl_80152958
/* 80152950 0014F890  38 60 00 01 */	li r3, 1
/* 80152954 0014F894  4E 80 00 20 */	blr 
lbl_80152958:
/* 80152958 0014F898  38 60 00 00 */	li r3, 0
/* 8015295C 0014F89C  4E 80 00 20 */	blr 

.global SMS_IsMarioFencing__Fv
SMS_IsMarioFencing__Fv:
/* 80152960 0014F8A0  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 80152964 0014F8A4  80 03 00 7C */	lwz r0, 0x7c(r3)
/* 80152968 0014F8A8  54 00 05 FE */	clrlwi r0, r0, 0x17
/* 8015296C 0014F8AC  28 00 01 68 */	cmplwi r0, 0x168
/* 80152970 0014F8B0  41 80 00 14 */	blt lbl_80152984
/* 80152974 0014F8B4  28 00 01 6C */	cmplwi r0, 0x16c
/* 80152978 0014F8B8  41 81 00 0C */	bgt lbl_80152984
/* 8015297C 0014F8BC  38 60 00 01 */	li r3, 1
/* 80152980 0014F8C0  4E 80 00 20 */	blr 
lbl_80152984:
/* 80152984 0014F8C4  38 60 00 00 */	li r3, 0
/* 80152988 0014F8C8  4E 80 00 20 */	blr 

.global SMS_GetMarioStatus__FP9THitActor
SMS_GetMarioStatus__FP9THitActor:
/* 8015298C 0014F8CC  80 63 00 7C */	lwz r3, 0x7c(r3)
/* 80152990 0014F8D0  4E 80 00 20 */	blr 

.global SMS_GetMarioRfPlane__Fv
SMS_GetMarioRfPlane__Fv:
/* 80152994 0014F8D4  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 80152998 0014F8D8  80 63 00 DC */	lwz r3, 0xdc(r3)
/* 8015299C 0014F8DC  4E 80 00 20 */	blr 

.global SMS_GetMarioWlPlane__Fv
SMS_GetMarioWlPlane__Fv:
/* 801529A0 0014F8E0  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 801529A4 0014F8E4  80 63 00 D8 */	lwz r3, 0xd8(r3)
/* 801529A8 0014F8E8  4E 80 00 20 */	blr 

.global SMS_GetMarioGrPlane__Fv
SMS_GetMarioGrPlane__Fv:
/* 801529AC 0014F8EC  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 801529B0 0014F8F0  80 63 00 E0 */	lwz r3, 0xe0(r3)
/* 801529B4 0014F8F4  4E 80 00 20 */	blr 

.global SMS_GetMarioStatus__Fv
SMS_GetMarioStatus__Fv:
/* 801529B8 0014F8F8  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 801529BC 0014F8FC  80 63 00 7C */	lwz r3, 0x7c(r3)
/* 801529C0 0014F900  4E 80 00 20 */	blr 

.global SMS_WindMoveMario__FRCQ29JGeometry8TVec3$$0f$$1
SMS_WindMoveMario__FRCQ29JGeometry8TVec3$$0f$$1:
/* 801529C4 0014F904  7C 08 02 A6 */	mflr r0
/* 801529C8 0014F908  7C 64 1B 78 */	mr r4, r3
/* 801529CC 0014F90C  90 01 00 04 */	stw r0, 4(r1)
/* 801529D0 0014F910  94 21 FF F8 */	stwu r1, -8(r1)
/* 801529D4 0014F914  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 801529D8 0014F918  4B FE 23 29 */	bl windMove__6TMarioFRCQ29JGeometry8TVec3$$0f$$1
/* 801529DC 0014F91C  80 01 00 0C */	lwz r0, 0xc(r1)
/* 801529E0 0014F920  38 21 00 08 */	addi r1, r1, 8
/* 801529E4 0014F924  7C 08 03 A6 */	mtlr r0
/* 801529E8 0014F928  4E 80 00 20 */	blr 

.global SMS_FlowMoveMario__FRCQ29JGeometry8TVec3$$0f$$1
SMS_FlowMoveMario__FRCQ29JGeometry8TVec3$$0f$$1:
/* 801529EC 0014F92C  7C 08 02 A6 */	mflr r0
/* 801529F0 0014F930  7C 64 1B 78 */	mr r4, r3
/* 801529F4 0014F934  90 01 00 04 */	stw r0, 4(r1)
/* 801529F8 0014F938  94 21 FF F8 */	stwu r1, -8(r1)
/* 801529FC 0014F93C  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 80152A00 0014F940  4B FE 23 35 */	bl flowMove__6TMarioFRCQ29JGeometry8TVec3$$0f$$1
/* 80152A04 0014F944  80 01 00 0C */	lwz r0, 0xc(r1)
/* 80152A08 0014F948  38 21 00 08 */	addi r1, r1, 8
/* 80152A0C 0014F94C  7C 08 03 A6 */	mtlr r0
/* 80152A10 0014F950  4E 80 00 20 */	blr 

.global SMS_MarioWarpRequest__FRCQ29JGeometry8TVec3$$0f$$1f
SMS_MarioWarpRequest__FRCQ29JGeometry8TVec3$$0f$$1f:
/* 80152A14 0014F954  7C 08 02 A6 */	mflr r0
/* 80152A18 0014F958  7C 64 1B 78 */	mr r4, r3
/* 80152A1C 0014F95C  90 01 00 04 */	stw r0, 4(r1)
/* 80152A20 0014F960  94 21 FF F8 */	stwu r1, -8(r1)
/* 80152A24 0014F964  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 80152A28 0014F968  4B FE 23 65 */	bl warpRequest__6TMarioFRCQ29JGeometry8TVec3$$0f$$1f
/* 80152A2C 0014F96C  80 01 00 0C */	lwz r0, 0xc(r1)
/* 80152A30 0014F970  38 21 00 08 */	addi r1, r1, 8
/* 80152A34 0014F974  7C 08 03 A6 */	mtlr r0
/* 80152A38 0014F978  4E 80 00 20 */	blr 

.global SMS_MarioMoveRequest__FRCQ29JGeometry8TVec3$$0f$$1
SMS_MarioMoveRequest__FRCQ29JGeometry8TVec3$$0f$$1:
/* 80152A3C 0014F97C  7C 08 02 A6 */	mflr r0
/* 80152A40 0014F980  7C 64 1B 78 */	mr r4, r3
/* 80152A44 0014F984  90 01 00 04 */	stw r0, 4(r1)
/* 80152A48 0014F988  94 21 FF F8 */	stwu r1, -8(r1)
/* 80152A4C 0014F98C  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 80152A50 0014F990  81 83 00 00 */	lwz r12, 0(r3)
/* 80152A54 0014F994  81 8C 00 AC */	lwz r12, 0xac(r12)
/* 80152A58 0014F998  7D 88 03 A6 */	mtlr r12
/* 80152A5C 0014F99C  4E 80 00 21 */	blrl 
/* 80152A60 0014F9A0  80 01 00 0C */	lwz r0, 0xc(r1)
/* 80152A64 0014F9A4  38 21 00 08 */	addi r1, r1, 8
/* 80152A68 0014F9A8  7C 08 03 A6 */	mtlr r0
/* 80152A6C 0014F9AC  4E 80 00 20 */	blr 

.global SMS_IsMarioDashing__Fv
SMS_IsMarioDashing__Fv:
/* 80152A70 0014F9B0  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 80152A74 0014F9B4  80 03 01 18 */	lwz r0, 0x118(r3)
/* 80152A78 0014F9B8  54 00 04 63 */	rlwinm. r0, r0, 0, 0x11, 0x11
/* 80152A7C 0014F9BC  41 82 00 0C */	beq lbl_80152A88
/* 80152A80 0014F9C0  38 00 00 01 */	li r0, 1
/* 80152A84 0014F9C4  48 00 00 08 */	b lbl_80152A8C
lbl_80152A88:
/* 80152A88 0014F9C8  38 00 00 00 */	li r0, 0
lbl_80152A8C:
/* 80152A8C 0014F9CC  54 00 06 3E */	clrlwi r0, r0, 0x18
/* 80152A90 0014F9D0  7C 60 00 D0 */	neg r3, r0
/* 80152A94 0014F9D4  30 03 FF FF */	addic r0, r3, -1
/* 80152A98 0014F9D8  7C 00 19 10 */	subfe r0, r0, r3
/* 80152A9C 0014F9DC  54 03 06 3E */	clrlwi r3, r0, 0x18
/* 80152AA0 0014F9E0  4E 80 00 20 */	blr 

.global SMS_IsMarioOnYoshi__Fv
SMS_IsMarioOnYoshi__Fv:
/* 80152AA4 0014F9E4  7C 08 02 A6 */	mflr r0
/* 80152AA8 0014F9E8  90 01 00 04 */	stw r0, 4(r1)
/* 80152AAC 0014F9EC  94 21 FF F8 */	stwu r1, -8(r1)
/* 80152AB0 0014F9F0  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 80152AB4 0014F9F4  4B FE 22 05 */	bl onYoshi__6TMarioCFv
/* 80152AB8 0014F9F8  7C 63 00 D0 */	neg r3, r3
/* 80152ABC 0014F9FC  30 03 FF FF */	addic r0, r3, -1
/* 80152AC0 0014FA00  7C 00 19 10 */	subfe r0, r0, r3
/* 80152AC4 0014FA04  54 03 06 3E */	clrlwi r3, r0, 0x18
/* 80152AC8 0014FA08  80 01 00 0C */	lwz r0, 0xc(r1)
/* 80152ACC 0014FA0C  38 21 00 08 */	addi r1, r1, 8
/* 80152AD0 0014FA10  7C 08 03 A6 */	mtlr r0
/* 80152AD4 0014FA14  4E 80 00 20 */	blr 

.global SMS_IsMarioOpeningDoor__Fv
SMS_IsMarioOpeningDoor__Fv:
/* 80152AD8 0014FA18  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 80152ADC 0014FA1C  80 03 00 7C */	lwz r0, 0x7c(r3)
/* 80152AE0 0014FA20  28 00 13 20 */	cmplwi r0, 0x1320
/* 80152AE4 0014FA24  41 82 00 0C */	beq lbl_80152AF0
/* 80152AE8 0014FA28  28 00 13 21 */	cmplwi r0, 0x1321
/* 80152AEC 0014FA2C  40 82 00 0C */	bne lbl_80152AF8
lbl_80152AF0:
/* 80152AF0 0014FA30  38 60 00 01 */	li r3, 1
/* 80152AF4 0014FA34  4E 80 00 20 */	blr 
lbl_80152AF8:
/* 80152AF8 0014FA38  38 60 00 00 */	li r3, 0
/* 80152AFC 0014FA3C  4E 80 00 20 */	blr 

.global SMS_IsMarioOnWire__Fv
SMS_IsMarioOnWire__Fv:
/* 80152B00 0014FA40  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 80152B04 0014FA44  80 03 00 68 */	lwz r0, 0x68(r3)
/* 80152B08 0014FA48  28 00 00 00 */	cmplwi r0, 0
/* 80152B0C 0014FA4C  41 82 00 20 */	beq lbl_80152B2C
/* 80152B10 0014FA50  80 63 00 68 */	lwz r3, 0x68(r3)
/* 80152B14 0014FA54  80 63 00 4C */	lwz r3, 0x4c(r3)
/* 80152B18 0014FA58  3C 03 C0 00 */	addis r0, r3, 0xc000
/* 80152B1C 0014FA5C  28 00 00 98 */	cmplwi r0, 0x98
/* 80152B20 0014FA60  40 82 00 0C */	bne lbl_80152B2C
/* 80152B24 0014FA64  38 00 00 01 */	li r0, 1
/* 80152B28 0014FA68  48 00 00 08 */	b lbl_80152B30
lbl_80152B2C:
/* 80152B2C 0014FA6C  38 00 00 00 */	li r0, 0
lbl_80152B30:
/* 80152B30 0014FA70  54 00 06 3E */	clrlwi r0, r0, 0x18
/* 80152B34 0014FA74  7C 60 00 D0 */	neg r3, r0
/* 80152B38 0014FA78  30 03 FF FF */	addic r0, r3, -1
/* 80152B3C 0014FA7C  7C 00 19 10 */	subfe r0, r0, r3
/* 80152B40 0014FA80  54 03 06 3E */	clrlwi r3, r0, 0x18
/* 80152B44 0014FA84  4E 80 00 20 */	blr 

.global SMS_IsMarioTouchGround4cm__Fv
SMS_IsMarioTouchGround4cm__Fv:
/* 80152B48 0014FA88  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 80152B4C 0014FA8C  C0 22 A1 98 */	lfs f1, $$22019@sda21(r2)
/* 80152B50 0014FA90  C0 03 00 EC */	lfs f0, 0xec(r3)
/* 80152B54 0014FA94  C0 43 00 14 */	lfs f2, 0x14(r3)
/* 80152B58 0014FA98  EC 01 00 2A */	fadds f0, f1, f0
/* 80152B5C 0014FA9C  FC 02 00 40 */	fcmpo cr0, f2, f0
/* 80152B60 0014FAA0  4C 40 13 82 */	cror 2, 0, 2
/* 80152B64 0014FAA4  40 82 00 0C */	bne lbl_80152B70
/* 80152B68 0014FAA8  38 00 00 01 */	li r0, 1
/* 80152B6C 0014FAAC  48 00 00 08 */	b lbl_80152B74
lbl_80152B70:
/* 80152B70 0014FAB0  38 00 00 00 */	li r0, 0
lbl_80152B74:
/* 80152B74 0014FAB4  54 00 06 3F */	clrlwi. r0, r0, 0x18
/* 80152B78 0014FAB8  41 82 00 0C */	beq lbl_80152B84
/* 80152B7C 0014FABC  38 60 00 01 */	li r3, 1
/* 80152B80 0014FAC0  4E 80 00 20 */	blr 
lbl_80152B84:
/* 80152B84 0014FAC4  38 60 00 00 */	li r3, 0
/* 80152B88 0014FAC8  4E 80 00 20 */	blr 

.global SMS_ThrowMario__FRCQ29JGeometry8TVec3$$0f$$1f
SMS_ThrowMario__FRCQ29JGeometry8TVec3$$0f$$1f:
/* 80152B8C 0014FACC  7C 08 02 A6 */	mflr r0
/* 80152B90 0014FAD0  7C 64 1B 78 */	mr r4, r3
/* 80152B94 0014FAD4  90 01 00 04 */	stw r0, 4(r1)
/* 80152B98 0014FAD8  94 21 FF F8 */	stwu r1, -8(r1)
/* 80152B9C 0014FADC  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 80152BA0 0014FAE0  4B FE 0A 51 */	bl throwMario__6TMarioFRCQ29JGeometry8TVec3$$0f$$1f
/* 80152BA4 0014FAE4  80 01 00 0C */	lwz r0, 0xc(r1)
/* 80152BA8 0014FAE8  38 21 00 08 */	addi r1, r1, 8
/* 80152BAC 0014FAEC  7C 08 03 A6 */	mtlr r0
/* 80152BB0 0014FAF0  4E 80 00 20 */	blr 

.global SMS_SendMessageToMario__FP9THitActorUl
SMS_SendMessageToMario__FP9THitActorUl:
/* 80152BB4 0014FAF4  7C 08 02 A6 */	mflr r0
/* 80152BB8 0014FAF8  7C 85 23 78 */	mr r5, r4
/* 80152BBC 0014FAFC  90 01 00 04 */	stw r0, 4(r1)
/* 80152BC0 0014FB00  7C 60 1B 78 */	mr r0, r3
/* 80152BC4 0014FB04  7C 04 03 78 */	mr r4, r0
/* 80152BC8 0014FB08  94 21 FF F8 */	stwu r1, -8(r1)
/* 80152BCC 0014FB0C  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 80152BD0 0014FB10  81 83 00 00 */	lwz r12, 0(r3)
/* 80152BD4 0014FB14  81 8C 00 A0 */	lwz r12, 0xa0(r12)
/* 80152BD8 0014FB18  7D 88 03 A6 */	mtlr r12
/* 80152BDC 0014FB1C  4E 80 00 21 */	blrl 
/* 80152BE0 0014FB20  2C 03 00 00 */	cmpwi r3, 0
/* 80152BE4 0014FB24  41 82 00 0C */	beq lbl_80152BF0
/* 80152BE8 0014FB28  38 60 00 01 */	li r3, 1
/* 80152BEC 0014FB2C  48 00 00 08 */	b lbl_80152BF4
lbl_80152BF0:
/* 80152BF0 0014FB30  38 60 00 00 */	li r3, 0
lbl_80152BF4:
/* 80152BF4 0014FB34  80 01 00 0C */	lwz r0, 0xc(r1)
/* 80152BF8 0014FB38  38 21 00 08 */	addi r1, r1, 8
/* 80152BFC 0014FB3C  7C 08 03 A6 */	mtlr r0
/* 80152C00 0014FB40  4E 80 00 20 */	blr 

.global SMS_GetMarioWaterGun__Fv
SMS_GetMarioWaterGun__Fv:
/* 80152C04 0014FB44  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 80152C08 0014FB48  80 63 03 E4 */	lwz r3, 0x3e4(r3)
/* 80152C0C 0014FB4C  4E 80 00 20 */	blr 

.global SMS_GetMarioGravity__Fv
SMS_GetMarioGravity__Fv:
/* 80152C10 0014FB50  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 80152C14 0014FB54  C0 23 0B 18 */	lfs f1, 0xb18(r3)
/* 80152C18 0014FB58  4E 80 00 20 */	blr 

.global SMS_GetMarioGrLevel__Fv
SMS_GetMarioGrLevel__Fv:
/* 80152C1C 0014FB5C  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 80152C20 0014FB60  C0 23 00 EC */	lfs f1, 0xec(r3)
/* 80152C24 0014FB64  4E 80 00 20 */	blr 

.global SMS_GetMarioDamageRadius__Fv
SMS_GetMarioDamageRadius__Fv:
/* 80152C28 0014FB68  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 80152C2C 0014FB6C  C0 23 00 58 */	lfs f1, 0x58(r3)
/* 80152C30 0014FB70  4E 80 00 20 */	blr 

.global SMS_GetMarioHP__Fv
SMS_GetMarioHP__Fv:
/* 80152C34 0014FB74  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 80152C38 0014FB78  A8 63 01 20 */	lha r3, 0x120(r3)
/* 80152C3C 0014FB7C  4E 80 00 20 */	blr 

.global SMS_IsMarioHeadSlideAttack__Fv
SMS_IsMarioHeadSlideAttack__Fv:
/* 80152C40 0014FB80  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 80152C44 0014FB84  80 63 00 7C */	lwz r3, 0x7c(r3)
/* 80152C48 0014FB88  3C 03 FF 80 */	addis r0, r3, 0xff80
/* 80152C4C 0014FB8C  28 00 04 56 */	cmplwi r0, 0x456
/* 80152C50 0014FB90  41 82 00 0C */	beq lbl_80152C5C
/* 80152C54 0014FB94  28 00 08 8A */	cmplwi r0, 0x88a
/* 80152C58 0014FB98  40 82 00 0C */	bne lbl_80152C64
lbl_80152C5C:
/* 80152C5C 0014FB9C  38 60 00 01 */	li r3, 1
/* 80152C60 0014FBA0  4E 80 00 20 */	blr 
lbl_80152C64:
/* 80152C64 0014FBA4  38 60 00 00 */	li r3, 0
/* 80152C68 0014FBA8  4E 80 00 20 */	blr 

.global SMS_IsMarioStatusElecDamage__Fv
SMS_IsMarioStatusElecDamage__Fv:
/* 80152C6C 0014FBAC  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 80152C70 0014FBB0  80 63 00 7C */	lwz r3, 0x7c(r3)
/* 80152C74 0014FBB4  3C 03 FF FE */	addis r0, r3, 0xfffe
/* 80152C78 0014FBB8  28 00 03 38 */	cmplwi r0, 0x338
/* 80152C7C 0014FBBC  40 82 00 0C */	bne lbl_80152C88
/* 80152C80 0014FBC0  38 60 00 01 */	li r3, 1
/* 80152C84 0014FBC4  4E 80 00 20 */	blr 
lbl_80152C88:
/* 80152C88 0014FBC8  38 60 00 00 */	li r3, 0
/* 80152C8C 0014FBCC  4E 80 00 20 */	blr 

.global SMS_IsMarioStatusThrownDown__Fv
SMS_IsMarioStatusThrownDown__Fv:
/* 80152C90 0014FBD0  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 80152C94 0014FBD4  80 63 00 7C */	lwz r3, 0x7c(r3)
/* 80152C98 0014FBD8  3C 03 FF FE */	addis r0, r3, 0xfffe
/* 80152C9C 0014FBDC  28 00 08 B8 */	cmplwi r0, 0x8b8
/* 80152CA0 0014FBE0  40 82 00 0C */	bne lbl_80152CAC
/* 80152CA4 0014FBE4  38 60 00 01 */	li r3, 1
/* 80152CA8 0014FBE8  4E 80 00 20 */	blr 
lbl_80152CAC:
/* 80152CAC 0014FBEC  38 60 00 00 */	li r3, 0
/* 80152CB0 0014FBF0  4E 80 00 20 */	blr 

.global SMS_IsMarioStatusHipDrop__Fv
SMS_IsMarioStatusHipDrop__Fv:
/* 80152CB4 0014FBF4  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 80152CB8 0014FBF8  80 63 00 7C */	lwz r3, 0x7c(r3)
/* 80152CBC 0014FBFC  3C 03 FF 80 */	addis r0, r3, 0xff80
/* 80152CC0 0014FC00  28 00 08 A9 */	cmplwi r0, 0x8a9
/* 80152CC4 0014FC04  40 82 00 0C */	bne lbl_80152CD0
/* 80152CC8 0014FC08  38 60 00 01 */	li r3, 1
/* 80152CCC 0014FC0C  4E 80 00 20 */	blr 
lbl_80152CD0:
/* 80152CD0 0014FC10  38 60 00 00 */	li r3, 0
/* 80152CD4 0014FC14  4E 80 00 20 */	blr 

.global SMS_IsMarioStatusTypeSwimming__Fv
SMS_IsMarioStatusTypeSwimming__Fv:
/* 80152CD8 0014FC18  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 80152CDC 0014FC1C  80 03 00 7C */	lwz r0, 0x7c(r3)
/* 80152CE0 0014FC20  54 00 04 A5 */	rlwinm. r0, r0, 0, 0x12, 0x12
/* 80152CE4 0014FC24  41 82 00 0C */	beq lbl_80152CF0
/* 80152CE8 0014FC28  38 60 00 01 */	li r3, 1
/* 80152CEC 0014FC2C  4E 80 00 20 */	blr 
lbl_80152CF0:
/* 80152CF0 0014FC30  38 60 00 00 */	li r3, 0
/* 80152CF4 0014FC34  4E 80 00 20 */	blr 

.global SMS_IsMarioStatusTypeJumping__Fv
SMS_IsMarioStatusTypeJumping__Fv:
/* 80152CF8 0014FC38  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 80152CFC 0014FC3C  80 03 00 7C */	lwz r0, 0x7c(r3)
/* 80152D00 0014FC40  54 00 05 29 */	rlwinm. r0, r0, 0, 0x14, 0x14
/* 80152D04 0014FC44  41 82 00 0C */	beq lbl_80152D10
/* 80152D08 0014FC48  38 60 00 01 */	li r3, 1
/* 80152D0C 0014FC4C  4E 80 00 20 */	blr 
lbl_80152D10:
/* 80152D10 0014FC50  38 60 00 00 */	li r3, 0
/* 80152D14 0014FC54  4E 80 00 20 */	blr 

.global SMS_GetMarioLiveActor__Fv
SMS_GetMarioLiveActor__Fv:
/* 80152D18 0014FC58  80 6D 98 D8 */	lwz r3, gpMarioAddress@sda21(r13)
/* 80152D1C 0014FC5C  4E 80 00 20 */	blr 

.global SMS_GetMarioHitActor__Fv
SMS_GetMarioHitActor__Fv:
/* 80152D20 0014FC60  80 6D 98 D8 */	lwz r3, gpMarioAddress@sda21(r13)
/* 80152D24 0014FC64  4E 80 00 20 */	blr 

.global SMS_AskJumpIntoWaterEffectExist__Fv
SMS_AskJumpIntoWaterEffectExist__Fv:
/* 80152D28 0014FC68  7C 08 02 A6 */	mflr r0
/* 80152D2C 0014FC6C  90 01 00 04 */	stw r0, 4(r1)
/* 80152D30 0014FC70  94 21 FF F8 */	stwu r1, -8(r1)
/* 80152D34 0014FC74  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 80152D38 0014FC78  4B FE F3 75 */	bl askJumpIntoWaterEffectExist__6TMarioCFv
/* 80152D3C 0014FC7C  80 01 00 0C */	lwz r0, 0xc(r1)
/* 80152D40 0014FC80  38 21 00 08 */	addi r1, r1, 8
/* 80152D44 0014FC84  7C 08 03 A6 */	mtlr r0
/* 80152D48 0014FC88  4E 80 00 20 */	blr 

.global SMS_GetYoshi__Fv
SMS_GetYoshi__Fv:
/* 80152D4C 0014FC8C  80 6D 98 B8 */	lwz r3, gpMarioOriginal@sda21(r13)
/* 80152D50 0014FC90  80 63 03 F0 */	lwz r3, 0x3f0(r3)
/* 80152D54 0014FC94  4E 80 00 20 */	blr 

.global SMS_SetMarioAccessParams__Fv
SMS_SetMarioAccessParams__Fv:
/* 80152D58 0014FC98  81 0D 98 B8 */	lwz r8, gpMarioOriginal@sda21(r13)
/* 80152D5C 0014FC9C  38 A8 00 94 */	addi r5, r8, 0x94
/* 80152D60 0014FCA0  91 0D 98 D8 */	stw r8, gpMarioAddress@sda21(r13)
/* 80152D64 0014FCA4  39 28 00 A4 */	addi r9, r8, 0xa4
/* 80152D68 0014FCA8  38 88 00 10 */	addi r4, r8, 0x10
/* 80152D6C 0014FCAC  90 AD 98 E0 */	stw r5, gpMarioAngleX@sda21(r13)
/* 80152D70 0014FCB0  38 65 00 02 */	addi r3, r5, 2
/* 80152D74 0014FCB4  38 05 00 04 */	addi r0, r5, 4
/* 80152D78 0014FCB8  90 8D 98 DC */	stw r4, gpMarioPos@sda21(r13)
/* 80152D7C 0014FCBC  38 E9 00 04 */	addi r7, r9, 4
/* 80152D80 0014FCC0  38 C9 00 08 */	addi r6, r9, 8
/* 80152D84 0014FCC4  90 6D 98 E4 */	stw r3, gpMarioAngleY@sda21(r13)
/* 80152D88 0014FCC8  38 A8 00 F8 */	addi r5, r8, 0xf8
/* 80152D8C 0014FCCC  38 88 01 18 */	addi r4, r8, 0x118
/* 80152D90 0014FCD0  90 0D 98 E8 */	stw r0, gpMarioAngleZ@sda21(r13)
/* 80152D94 0014FCD4  38 68 08 20 */	addi r3, r8, 0x820
/* 80152D98 0014FCD8  38 08 00 E0 */	addi r0, r8, 0xe0
/* 80152D9C 0014FCDC  91 2D 98 EC */	stw r9, gpMarioSpeedX@sda21(r13)
/* 80152DA0 0014FCE0  90 ED 98 F0 */	stw r7, gpMarioSpeedY@sda21(r13)
/* 80152DA4 0014FCE4  90 CD 98 F4 */	stw r6, gpMarioSpeedZ@sda21(r13)
/* 80152DA8 0014FCE8  90 AD 98 F8 */	stw r5, gpMarioLightID@sda21(r13)
/* 80152DAC 0014FCEC  90 8D 98 FC */	stw r4, gpMarioFlag@sda21(r13)
/* 80152DB0 0014FCF0  90 6D 99 00 */	stw r3, gpMarioThrowPower@sda21(r13)
/* 80152DB4 0014FCF4  90 0D 99 04 */	stw r0, gpMarioGroundPlane@sda21(r13)
/* 80152DB8 0014FCF8  4E 80 00 20 */	blr 

.section .sdata2, "a"  # 0x8040B460 - 0x80414020
.balign 8
$$22019:
	.incbin "baserom.dol", 0x3E5E98, 0x8

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
.global gpMarioAddress
gpMarioAddress:
	.skip 0x4
.global gpMarioPos
gpMarioPos:
	.skip 0x4
.global gpMarioAngleX
gpMarioAngleX:
	.skip 0x4
.global gpMarioAngleY
gpMarioAngleY:
	.skip 0x4
.global gpMarioAngleZ
gpMarioAngleZ:
	.skip 0x4
.global gpMarioSpeedX
gpMarioSpeedX:
	.skip 0x4
.global gpMarioSpeedY
gpMarioSpeedY:
	.skip 0x4
.global gpMarioSpeedZ
gpMarioSpeedZ:
	.skip 0x4
.global gpMarioLightID
gpMarioLightID:
	.skip 0x4
.global gpMarioFlag
gpMarioFlag:
	.skip 0x4
.global gpMarioThrowPower
gpMarioThrowPower:
	.skip 0x4
.global gpMarioGroundPlane
gpMarioGroundPlane:
	.skip 0x4

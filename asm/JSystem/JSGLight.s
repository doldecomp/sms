.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __dt__Q26JStage6TLightFv
__dt__Q26JStage6TLightFv:
/* 80081A30 0007E970  7C 08 02 A6 */	mflr r0
/* 80081A34 0007E974  90 01 00 04 */	stw r0, 4(r1)
/* 80081A38 0007E978  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80081A3C 0007E97C  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80081A40 0007E980  3B E4 00 00 */	addi r31, r4, 0
/* 80081A44 0007E984  93 C1 00 10 */	stw r30, 0x10(r1)
/* 80081A48 0007E988  7C 7E 1B 79 */	or. r30, r3, r3
/* 80081A4C 0007E98C  41 82 00 2C */	beq lbl_80081A78
/* 80081A50 0007E990  3C 60 80 3B */	lis r3, __vt__Q26JStage6TLight@ha
/* 80081A54 0007E994  38 03 CE 38 */	addi r0, r3, __vt__Q26JStage6TLight@l
/* 80081A58 0007E998  90 1E 00 00 */	stw r0, 0(r30)
/* 80081A5C 0007E99C  38 7E 00 00 */	addi r3, r30, 0
/* 80081A60 0007E9A0  38 80 00 00 */	li r4, 0
/* 80081A64 0007E9A4  48 00 00 81 */	bl __dt__Q26JStage7TObjectFv
/* 80081A68 0007E9A8  7F E0 07 35 */	extsh. r0, r31
/* 80081A6C 0007E9AC  40 81 00 0C */	ble lbl_80081A78
/* 80081A70 0007E9B0  7F C3 F3 78 */	mr r3, r30
/* 80081A74 0007E9B4  4B F8 B0 3D */	bl __dl__FPv
lbl_80081A78:
/* 80081A78 0007E9B8  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80081A7C 0007E9BC  7F C3 F3 78 */	mr r3, r30
/* 80081A80 0007E9C0  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80081A84 0007E9C4  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 80081A88 0007E9C8  7C 08 03 A6 */	mtlr r0
/* 80081A8C 0007E9CC  38 21 00 18 */	addi r1, r1, 0x18
/* 80081A90 0007E9D0  4E 80 00 20 */	blr 

.global JSGFGetType__Q26JStage6TLightCFv
JSGFGetType__Q26JStage6TLightCFv:
/* 80081A94 0007E9D4  38 60 00 05 */	li r3, 5
/* 80081A98 0007E9D8  4E 80 00 20 */	blr 

.global JSGGetLightType__Q26JStage6TLightCFv
JSGGetLightType__Q26JStage6TLightCFv:
/* 80081A9C 0007E9DC  38 60 00 01 */	li r3, 1
/* 80081AA0 0007E9E0  4E 80 00 20 */	blr 

.global JSGSetLightType__Q26JStage6TLightFQ26JStage7TELight
JSGSetLightType__Q26JStage6TLightFQ26JStage7TELight:
/* 80081AA4 0007E9E4  4E 80 00 20 */	blr 

.global JSGGetColor__Q26JStage6TLightCFv
JSGGetColor__Q26JStage6TLightCFv:
/* 80081AA8 0007E9E8  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80081AAC 0007E9EC  80 02 89 48 */	lwz r0, $$2118@sda21(r2)
/* 80081AB0 0007E9F0  90 01 00 10 */	stw r0, 0x10(r1)
/* 80081AB4 0007E9F4  80 61 00 10 */	lwz r3, 0x10(r1)
/* 80081AB8 0007E9F8  38 21 00 18 */	addi r1, r1, 0x18
/* 80081ABC 0007E9FC  4E 80 00 20 */	blr 

.global JSGSetColor__Q26JStage6TLightF8_GXColor
JSGSetColor__Q26JStage6TLightF8_GXColor:
/* 80081AC0 0007EA00  4E 80 00 20 */	blr 

.global JSGGetDistanceAttenuation__Q26JStage6TLightCFPfPfP13_GXDistAttnFn
JSGGetDistanceAttenuation__Q26JStage6TLightCFPfPfP13_GXDistAttnFn:
/* 80081AC4 0007EA04  4E 80 00 20 */	blr 

.global JSGSetDistanceAttenuation__Q26JStage6TLightFff13_GXDistAttnFn
JSGSetDistanceAttenuation__Q26JStage6TLightFff13_GXDistAttnFn:
/* 80081AC8 0007EA08  4E 80 00 20 */	blr 

.global JSGGetAngleAttenuation__Q26JStage6TLightCFPfP9_GXSpotFn
JSGGetAngleAttenuation__Q26JStage6TLightCFPfP9_GXSpotFn:
/* 80081ACC 0007EA0C  4E 80 00 20 */	blr 

.global JSGSetAngleAttenuation__Q26JStage6TLightFf9_GXSpotFn
JSGSetAngleAttenuation__Q26JStage6TLightFf9_GXSpotFn:
/* 80081AD0 0007EA10  4E 80 00 20 */	blr 

.global JSGGetPosition__Q26JStage6TLightCFP3Vec
JSGGetPosition__Q26JStage6TLightCFP3Vec:
/* 80081AD4 0007EA14  4E 80 00 20 */	blr 

.global JSGSetPosition__Q26JStage6TLightFRC3Vec
JSGSetPosition__Q26JStage6TLightFRC3Vec:
/* 80081AD8 0007EA18  4E 80 00 20 */	blr 

.global JSGGetDirection__Q26JStage6TLightCFP3Vec
JSGGetDirection__Q26JStage6TLightCFP3Vec:
/* 80081ADC 0007EA1C  4E 80 00 20 */	blr 

.global JSGSetDirection__Q26JStage6TLightFRC3Vec
JSGSetDirection__Q26JStage6TLightFRC3Vec:
/* 80081AE0 0007EA20  4E 80 00 20 */	blr 

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__Q26JStage6TLight
__vt__Q26JStage6TLight:
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte JSGFGetType__Q26JStage6TLightCFv
  .4byte JSGGetName__Q26JStage7TObjectCFv
  .4byte JSGGetFlag__Q26JStage7TObjectCFv
  .4byte JSGSetFlag__Q26JStage7TObjectFUl
  .4byte JSGGetData__Q26JStage7TObjectCFUlPvUl
  .4byte JSGSetData__Q26JStage7TObjectFUlPCvUl
  .4byte JSGGetParent__Q26JStage7TObjectCFPPQ26JStage7TObjectPUl
  .4byte JSGSetParent__Q26JStage7TObjectFPQ26JStage7TObjectUl
  .4byte JSGSetRelation__Q26JStage7TObjectFbPQ26JStage7TObjectUl
  .4byte JSGGetLightType__Q26JStage6TLightCFv
  .4byte JSGSetLightType__Q26JStage6TLightFQ26JStage7TELight
  .4byte JSGGetPosition__Q26JStage6TLightCFP3Vec
  .4byte JSGSetPosition__Q26JStage6TLightFRC3Vec
  .4byte JSGGetColor__Q26JStage6TLightCFv
  .4byte JSGSetColor__Q26JStage6TLightF8_GXColor
  .4byte JSGGetDistanceAttenuation__Q26JStage6TLightCFPfPfP13_GXDistAttnFn
  .4byte JSGSetDistanceAttenuation__Q26JStage6TLightFff13_GXDistAttnFn
  .4byte JSGGetAngleAttenuation__Q26JStage6TLightCFPfP9_GXSpotFn
  .4byte JSGSetAngleAttenuation__Q26JStage6TLightFf9_GXSpotFn
  .4byte JSGGetDirection__Q26JStage6TLightCFP3Vec
  .4byte JSGSetDirection__Q26JStage6TLightFRC3Vec

.section .sdata2, "a"  # 0x8040B460 - 0x80414020
.balign 8
$$2118:
  .4byte -1
  .4byte 0

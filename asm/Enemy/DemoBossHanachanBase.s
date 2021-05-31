.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global receiveMessage__17TDemoBossHanachanFP9THitActorUl
receiveMessage__17TDemoBossHanachanFP9THitActorUl:
/* 802FC968 002F98A8  38 60 00 00 */	li r3, 0
/* 802FC96C 002F98AC  4E 80 00 20 */	blr 

.global initBase__17TDemoBossHanachanFP12TLiveManagerUl
initBase__17TDemoBossHanachanFP12TLiveManagerUl:
/* 802FC970 002F98B0  7C 08 02 A6 */	mflr r0
/* 802FC974 002F98B4  90 01 00 04 */	stw r0, 4(r1)
/* 802FC978 002F98B8  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 802FC97C 002F98BC  93 E1 00 24 */	stw r31, 0x24(r1)
/* 802FC980 002F98C0  3B E5 00 00 */	addi r31, r5, 0
/* 802FC984 002F98C4  93 C1 00 20 */	stw r30, 0x20(r1)
/* 802FC988 002F98C8  3B C4 00 00 */	addi r30, r4, 0
/* 802FC98C 002F98CC  93 A1 00 1C */	stw r29, 0x1c(r1)
/* 802FC990 002F98D0  3B A3 00 00 */	addi r29, r3, 0
/* 802FC994 002F98D4  38 9D 00 00 */	addi r4, r29, 0
/* 802FC998 002F98D8  93 81 00 18 */	stw r28, 0x18(r1)
/* 802FC99C 002F98DC  93 C3 00 70 */	stw r30, 0x70(r3)
/* 802FC9A0 002F98E0  38 7E 00 00 */	addi r3, r30, 0
/* 802FC9A4 002F98E4  4B E1 80 19 */	bl manageActor__12TLiveManagerFP10TLiveActor
/* 802FC9A8 002F98E8  38 60 00 18 */	li r3, 0x18
/* 802FC9AC 002F98EC  4B D0 FF 05 */	bl __nw__FUl
/* 802FC9B0 002F98F0  7C 7C 1B 79 */	or. r28, r3, r3
/* 802FC9B4 002F98F4  41 82 00 10 */	beq lbl_802FC9C4
/* 802FC9B8 002F98F8  38 7C 00 00 */	addi r3, r28, 0
/* 802FC9BC 002F98FC  38 9E 00 00 */	addi r4, r30, 0
/* 802FC9C0 002F9900  4B E1 98 A1 */	bl __ct__13TMActorKeeperFP12TLiveManager
lbl_802FC9C4:
/* 802FC9C4 002F9904  93 9D 00 78 */	stw r28, 0x78(r29)
/* 802FC9C8 002F9908  38 BF 00 00 */	addi r5, r31, 0
/* 802FC9CC 002F990C  38 80 00 00 */	li r4, 0
/* 802FC9D0 002F9910  80 7D 00 78 */	lwz r3, 0x78(r29)
/* 802FC9D4 002F9914  4B E1 9D 89 */	bl createMActorFromNthData__13TMActorKeeperFiUl
/* 802FC9D8 002F9918  90 7D 00 74 */	stw r3, 0x74(r29)
/* 802FC9DC 002F991C  38 9D 00 C4 */	addi r4, r29, 0xc4
/* 802FC9E0 002F9920  C0 02 F6 3C */	lfs f0, $$22009@sda21(r2)
/* 802FC9E4 002F9924  D0 1D 01 48 */	stfs f0, 0x148(r29)
/* 802FC9E8 002F9928  C0 02 F6 40 */	lfs f0, $$22010@sda21(r2)
/* 802FC9EC 002F992C  D0 1D 00 BC */	stfs f0, 0xbc(r29)
/* 802FC9F0 002F9930  C0 1D 00 BC */	lfs f0, 0xbc(r29)
/* 802FC9F4 002F9934  D0 1D 01 4C */	stfs f0, 0x14c(r29)
/* 802FC9F8 002F9938  C0 02 F6 44 */	lfs f0, $$22011@sda21(r2)
/* 802FC9FC 002F993C  D0 1D 00 C0 */	stfs f0, 0xc0(r29)
/* 802FCA00 002F9940  C0 02 F6 38 */	lfs f0, $$21955@sda21(r2)
/* 802FCA04 002F9944  D0 1D 01 40 */	stfs f0, 0x140(r29)
/* 802FCA08 002F9948  C0 02 F6 48 */	lfs f0, $$22012@sda21(r2)
/* 802FCA0C 002F994C  D0 1D 00 B8 */	stfs f0, 0xb8(r29)
/* 802FCA10 002F9950  80 1D 00 F0 */	lwz r0, 0xf0(r29)
/* 802FCA14 002F9954  60 00 00 18 */	ori r0, r0, 0x18
/* 802FCA18 002F9958  90 1D 00 F0 */	stw r0, 0xf0(r29)
/* 802FCA1C 002F995C  C0 5D 01 48 */	lfs f2, 0x148(r29)
/* 802FCA20 002F9960  C0 3D 00 C0 */	lfs f1, 0xc0(r29)
/* 802FCA24 002F9964  C0 1D 00 14 */	lfs f0, 0x14(r29)
/* 802FCA28 002F9968  EC 42 00 72 */	fmuls f2, f2, f1
/* 802FCA2C 002F996C  80 6D 9A B0 */	lwz r3, gpMap@sda21(r13)
/* 802FCA30 002F9970  C0 3D 00 10 */	lfs f1, 0x10(r29)
/* 802FCA34 002F9974  C0 7D 00 18 */	lfs f3, 0x18(r29)
/* 802FCA38 002F9978  EC 40 10 2A */	fadds f2, f0, f2
/* 802FCA3C 002F997C  4B ED ED C5 */	bl checkGroundIgnoreWaterSurface__4TMapCFfffPPC12TBGCheckData
/* 802FCA40 002F9980  D0 3D 00 C8 */	stfs f1, 0xc8(r29)
/* 802FCA44 002F9984  38 80 00 01 */	li r4, 1
/* 802FCA48 002F9988  80 7D 00 74 */	lwz r3, 0x74(r29)
/* 802FCA4C 002F998C  4B DD 7E E5 */	bl setLightType__6MActorFi
/* 802FCA50 002F9990  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 802FCA54 002F9994  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 802FCA58 002F9998  83 C1 00 20 */	lwz r30, 0x20(r1)
/* 802FCA5C 002F999C  7C 08 03 A6 */	mtlr r0
/* 802FCA60 002F99A0  83 A1 00 1C */	lwz r29, 0x1c(r1)
/* 802FCA64 002F99A4  83 81 00 18 */	lwz r28, 0x18(r1)
/* 802FCA68 002F99A8  38 21 00 28 */	addi r1, r1, 0x28
/* 802FCA6C 002F99AC  4E 80 00 20 */	blr 

.global clipEnemies__24TDemoBossHanachanManagerFPQ26JDrama9TGraphics
clipEnemies__24TDemoBossHanachanManagerFPQ26JDrama9TGraphics:
/* 802FCA70 002F99B0  7C 08 02 A6 */	mflr r0
/* 802FCA74 002F99B4  90 01 00 04 */	stw r0, 4(r1)
/* 802FCA78 002F99B8  94 21 FF F8 */	stwu r1, -8(r1)
/* 802FCA7C 002F99BC  80 C3 00 54 */	lwz r6, 0x54(r3)
/* 802FCA80 002F99C0  C0 46 00 2C */	lfs f2, 0x2c(r6)
/* 802FCA84 002F99C4  C0 26 00 18 */	lfs f1, 0x18(r6)
/* 802FCA88 002F99C8  4B E1 7E 0D */	bl clipActorsAux__12TLiveManagerFPQ26JDrama9TGraphicsff
/* 802FCA8C 002F99CC  80 01 00 0C */	lwz r0, 0xc(r1)
/* 802FCA90 002F99D0  38 21 00 08 */	addi r1, r1, 8
/* 802FCA94 002F99D4  7C 08 03 A6 */	mtlr r0
/* 802FCA98 002F99D8  4E 80 00 20 */	blr 

.global __ct__27TDemoBossHanachanSaveParamsFPCc
__ct__27TDemoBossHanachanSaveParamsFPCc:
/* 802FCA9C 002F99DC  7C 08 02 A6 */	mflr r0
/* 802FCAA0 002F99E0  90 01 00 04 */	stw r0, 4(r1)
/* 802FCAA4 002F99E4  38 00 00 00 */	li r0, 0
/* 802FCAA8 002F99E8  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 802FCAAC 002F99EC  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 802FCAB0 002F99F0  93 C1 00 18 */	stw r30, 0x18(r1)
/* 802FCAB4 002F99F4  93 A1 00 14 */	stw r29, 0x14(r1)
/* 802FCAB8 002F99F8  93 81 00 10 */	stw r28, 0x10(r1)
/* 802FCABC 002F99FC  7C 7C 1B 78 */	mr r28, r3
/* 802FCAC0 002F9A00  90 83 00 00 */	stw r4, 0(r3)
/* 802FCAC4 002F9A04  3C 60 80 3A */	lis r3, $$21490@ha
/* 802FCAC8 002F9A08  3B A3 F9 18 */	addi r29, r3, $$21490@l
/* 802FCACC 002F9A0C  90 1C 00 04 */	stw r0, 4(r28)
/* 802FCAD0 002F9A10  38 7D 00 E0 */	addi r3, r29, 0xe0
/* 802FCAD4 002F9A14  4B D4 7B B9 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 802FCAD8 002F9A18  38 A3 00 00 */	addi r5, r3, 0
/* 802FCADC 002F9A1C  38 9C 00 00 */	addi r4, r28, 0
/* 802FCAE0 002F9A20  38 7C 00 08 */	addi r3, r28, 8
/* 802FCAE4 002F9A24  38 DD 00 E0 */	addi r6, r29, 0xe0
/* 802FCAE8 002F9A28  4B DD E1 0D */	bl __ct__10TBaseParamFP7TParamsUsPCc
/* 802FCAEC 002F9A2C  3C 60 80 3B */	lis r3, __vt__10TParamT$$0f$$1@ha
/* 802FCAF0 002F9A30  3B C3 29 B8 */	addi r30, r3, __vt__10TParamT$$0f$$1@l
/* 802FCAF4 002F9A34  93 DC 00 08 */	stw r30, 8(r28)
/* 802FCAF8 002F9A38  3C 60 80 3B */	lis r3, __vt__11TParamRT$$0f$$1@ha
/* 802FCAFC 002F9A3C  3B E3 29 F4 */	addi r31, r3, __vt__11TParamRT$$0f$$1@l
/* 802FCB00 002F9A40  C0 02 F6 4C */	lfs f0, $$22030@sda21(r2)
/* 802FCB04 002F9A44  38 7D 00 F0 */	addi r3, r29, 0xf0
/* 802FCB08 002F9A48  D0 1C 00 18 */	stfs f0, 0x18(r28)
/* 802FCB0C 002F9A4C  93 FC 00 08 */	stw r31, 8(r28)
/* 802FCB10 002F9A50  4B D4 7B 7D */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 802FCB14 002F9A54  38 A3 00 00 */	addi r5, r3, 0
/* 802FCB18 002F9A58  38 9C 00 00 */	addi r4, r28, 0
/* 802FCB1C 002F9A5C  38 7C 00 1C */	addi r3, r28, 0x1c
/* 802FCB20 002F9A60  38 DD 00 F0 */	addi r6, r29, 0xf0
/* 802FCB24 002F9A64  4B DD E0 D1 */	bl __ct__10TBaseParamFP7TParamsUsPCc
/* 802FCB28 002F9A68  93 DC 00 1C */	stw r30, 0x1c(r28)
/* 802FCB2C 002F9A6C  7F 83 E3 78 */	mr r3, r28
/* 802FCB30 002F9A70  C0 02 F6 50 */	lfs f0, $$22032@sda21(r2)
/* 802FCB34 002F9A74  D0 1C 00 2C */	stfs f0, 0x2c(r28)
/* 802FCB38 002F9A78  93 FC 00 1C */	stw r31, 0x1c(r28)
/* 802FCB3C 002F9A7C  80 9C 00 00 */	lwz r4, 0(r28)
/* 802FCB40 002F9A80  4B DF B4 59 */	bl load__7TParamsFPCc
/* 802FCB44 002F9A84  80 01 00 24 */	lwz r0, 0x24(r1)
/* 802FCB48 002F9A88  7F 83 E3 78 */	mr r3, r28
/* 802FCB4C 002F9A8C  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 802FCB50 002F9A90  83 C1 00 18 */	lwz r30, 0x18(r1)
/* 802FCB54 002F9A94  7C 08 03 A6 */	mtlr r0
/* 802FCB58 002F9A98  83 A1 00 14 */	lwz r29, 0x14(r1)
/* 802FCB5C 002F9A9C  83 81 00 10 */	lwz r28, 0x10(r1)
/* 802FCB60 002F9AA0  38 21 00 20 */	addi r1, r1, 0x20
/* 802FCB64 002F9AA4  4E 80 00 20 */	blr 

.global $$232$$2__dt__17TDemoBossHanachanFv
$$232$$2__dt__17TDemoBossHanachanFv:
/* 802FCB68 002F9AA8  38 63 FF E0 */	addi r3, r3, -32
/* 802FCB6C 002F9AAC  4B E0 13 EC */	b __dt__17TDemoBossHanachanFv

.section .rodata, "wa"  # 0x8036FFA0 - 0x803A8380
$$21490:
  .4byte 0
  .4byte 0
  .4byte 0
$$21526:
	.incbin "baserom.dol", 0x39C924, 0x14
$$21597:
	.incbin "baserom.dol", 0x39C938, 0x30
$$21598:
	.incbin "baserom.dol", 0x39C968, 0x38
$$21599:
	.incbin "baserom.dol", 0x39C9A0, 0x34
$$21600:
	.incbin "baserom.dol", 0x39C9D4, 0x24
$$22029:
	.incbin "baserom.dol", 0x39C9F8, 0x10
$$22031:
	.incbin "baserom.dol", 0x39CA08, 0x18

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__17TDemoBossHanachan
__vt__17TDemoBossHanachan:
  .4byte 0
  .4byte 0
  .4byte __dt__17TDemoBossHanachanFv
  .4byte getType__Q26JDrama6TActorCFv
  .4byte load__11TSpineEnemyFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__Q26JDrama8TNameRefFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc
  .4byte perform__11TSpineEnemyFUlPQ26JDrama9TGraphics
  .4byte 0
  .4byte 0
  .4byte $$232$$2__dt__17TDemoBossHanachanFv
  .4byte JSGFGetType__Q26JStage6TActorCFv
  .4byte JSGGetName__Q26JStage7TObjectCFv
  .4byte JSGGetFlag__Q26JStage7TObjectCFv
  .4byte JSGSetFlag__Q26JStage7TObjectFUl
  .4byte JSGGetData__Q26JStage7TObjectCFUlPvUl
  .4byte JSGSetData__Q26JStage7TObjectFUlPCvUl
  .4byte JSGGetParent__Q26JStage7TObjectCFPPQ26JStage7TObjectPUl
  .4byte JSGSetParent__Q26JStage7TObjectFPQ26JStage7TObjectUl
  .4byte JSGSetRelation__Q26JStage7TObjectFbPQ26JStage7TObjectUl
  .4byte $$232$$2JSGGetTranslation__Q26JDrama6TActorCFP3Vec
  .4byte $$232$$2JSGSetTranslation__Q26JDrama6TActorFRC3Vec
  .4byte $$232$$2JSGGetScaling__Q26JDrama6TActorCFP3Vec
  .4byte $$232$$2JSGSetScaling__Q26JDrama6TActorFRC3Vec
  .4byte $$232$$2JSGGetRotation__Q26JDrama6TActorCFP3Vec
  .4byte $$232$$2JSGSetRotation__Q26JDrama6TActorFRC3Vec
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
  .4byte receiveMessage__17TDemoBossHanachanFP9THitActorUl
  .4byte getTakingMtx__10TLiveActorFv
  .4byte ensureTakeSituation__10TTakeActorFv
  .4byte moveRequest__10TTakeActorFRCQ29JGeometry8TVec3$$0f$$1
  .4byte getRadiusAtY__10TTakeActorCFf
  .4byte belongToGround__10TLiveActorCFv
  .4byte getRootJointMtx__10TLiveActorCFv
  .4byte init__11TSpineEnemyFP12TLiveManager
  .4byte calcRootMatrix__11TSpineEnemyFv
  .4byte setGroundCollision__10TLiveActorFv
  .4byte control__10TLiveActorFv
  .4byte bind__10TLiveActorFv
  .4byte moveObject__10TLiveActorFv
  .4byte requestShadow__10TLiveActorFv
  .4byte drawObject__10TLiveActorFPQ26JDrama9TGraphics
  .4byte performOnlyDraw__10TLiveActorFUlPQ26JDrama9TGraphics
  .4byte getShadowType__10TLiveActorFv
  .4byte kill__10TLiveActorFv
  .4byte getGravityY__10TLiveActorCFv
  .4byte hasMapCollision__10TLiveActorCFv
  .4byte getFocalPoint__10TLiveActorCFv
  .4byte updateAnmSound__10TLiveActorFv
  .4byte getBasNameTable__10TLiveActorCFv
  .4byte reset__11TSpineEnemyFv
  .4byte resetToPosition__11TSpineEnemyFRCQ29JGeometry8TVec3$$0f$$1
  .4byte resetSRTV__11TSpineEnemyFRCQ29JGeometry8TVec3$$0f$$1RCQ29JGeometry8TVec3$$0f$$1RCQ29JGeometry8TVec3$$0f$$1RCQ29JGeometry8TVec3$$0f$$1
  .4byte getSaveParam__11TSpineEnemyCFv
  .4byte getPhaseShift__11TSpineEnemyCFv
  .4byte isReachedToGoal__11TSpineEnemyCFv
.global __vt__24TDemoBossHanachanManager
__vt__24TDemoBossHanachanManager:
  .4byte 0
  .4byte 0
  .4byte __dt__24TDemoBossHanachanManagerFv
  .4byte getType__Q26JDrama8TNameRefCFv
  .4byte load__13TEnemyManagerFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__Q26JDrama8TNameRefFv
  .4byte searchF__11TObjManagerFUsPCc
  .4byte perform__13TEnemyManagerFUlPQ26JDrama9TGraphics
  .4byte createModelData__11TObjManagerFv
  .4byte createAnmData__11TObjManagerFv
  .4byte createModelDataArray__11TObjManagerFPC19TModelDataLoadEntry
  .4byte clipActors__12TLiveManagerFPQ26JDrama9TGraphics
  .4byte setFlagOutOfCube__12TLiveManagerFv
  .4byte createSpcBinary__12TLiveManagerFv
  .4byte hasMapCollision__12TLiveManagerCFv
  .4byte createEnemyInstance__13TEnemyManagerFv
  .4byte clipEnemies__24TDemoBossHanachanManagerFPQ26JDrama9TGraphics
  .4byte restoreDrawBuffer__13TEnemyManagerFUl
  .4byte createEnemies__13TEnemyManagerFi
  .4byte changeDrawBuffer__13TEnemyManagerFUl

.section .sdata2, "wa"  # 0x8040B460 - 0x80414020
$$21955:
  .4byte 0
$$22009:
	.incbin "baserom.dol", 0x3EB33C, 0x4
$$22010:
	.incbin "baserom.dol", 0x3EB340, 0x4
$$22011:
	.incbin "baserom.dol", 0x3EB344, 0x4
$$22012:
	.incbin "baserom.dol", 0x3EB348, 0x4
$$22030:
	.incbin "baserom.dol", 0x3EB34C, 0x4
$$22032:
	.incbin "baserom.dol", 0x3EB350, 0x8

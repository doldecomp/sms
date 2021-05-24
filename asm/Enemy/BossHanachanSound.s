.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global getBasNameTable__22TBossHanachanPartsBaseCFv
getBasNameTable__22TBossHanachanPartsBaseCFv:
/* 8031C050 00318F90  3C 60 80 3E */	lis r3, bosshanachan_bastable@ha
/* 8031C054 00318F94  38 63 10 88 */	addi r3, r3, bosshanachan_bastable@l
/* 8031C058 00318F98  4E 80 00 20 */	blr 

.global $$232$$2__dt__22TBossHanachanPartsBaseFv
$$232$$2__dt__22TBossHanachanPartsBaseFv:
/* 8031C05C 00318F9C  38 63 FF E0 */	addi r3, r3, -32
/* 8031C060 00318FA0  4B FE A4 98 */	b __dt__22TBossHanachanPartsBaseFv

.section .rodata, "wa"  # 0x8036FFA0 - 0x803A8380
$$21490:
	.incbin "baserom.dol", 0x39E9B0, 0xC
$$21526:
	.incbin "baserom.dol", 0x39E9BC, 0x14
$$21755:
	.incbin "baserom.dol", 0x39E9D0, 0x30
$$21756:
	.incbin "baserom.dol", 0x39EA00, 0x38
$$21757:
	.incbin "baserom.dol", 0x39EA38, 0x34
$$21758:
	.incbin "baserom.dol", 0x39EA6C, 0x24
$$21940:
	.incbin "baserom.dol", 0x39EA90, 0x2C
$$21941:
	.incbin "baserom.dol", 0x39EABC, 0x30
$$21942:
	.incbin "baserom.dol", 0x39EAEC, 0x30
$$21943:
	.incbin "baserom.dol", 0x39EB1C, 0x30
$$21944:
	.incbin "baserom.dol", 0x39EB4C, 0x30
$$21945:
	.incbin "baserom.dol", 0x39EB7C, 0x34
$$21946:
	.incbin "baserom.dol", 0x39EBB0, 0x30
$$21947:
	.incbin "baserom.dol", 0x39EBE0, 0x34
$$21948:
	.incbin "baserom.dol", 0x39EC14, 0x30
$$21949:
	.incbin "baserom.dol", 0x39EC44, 0x2C
$$21950:
	.incbin "baserom.dol", 0x39EC70, 0x2C
$$21951:
	.incbin "baserom.dol", 0x39EC9C, 0x2C
$$21952:
	.incbin "baserom.dol", 0x39ECC8, 0x2C
$$21953:
	.incbin "baserom.dol", 0x39ECF4, 0x38
$$21954:
	.incbin "baserom.dol", 0x39ED2C, 0x2C
$$21955:
	.incbin "baserom.dol", 0x39ED58, 0x30
$$21956:
	.incbin "baserom.dol", 0x39ED88, 0x30

.section .data, "wa"  # 0x803A8380 - 0x803E6000
bosshanachan_bastable:
	.incbin "baserom.dol", 0x3DE088, 0x98
.global __vt__22TBossHanachanPartsBase
__vt__22TBossHanachanPartsBase:
  .4byte 0
  .4byte 0
  .4byte __dt__22TBossHanachanPartsBaseFv
  .4byte getType__Q26JDrama6TActorCFv
  .4byte load__10TLiveActorFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__Q26JDrama8TNameRefFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc
  .4byte perform__10TLiveActorFUlPQ26JDrama9TGraphics
  .4byte 0
  .4byte 0
  .4byte $$232$$2__dt__22TBossHanachanPartsBaseFv
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
  .4byte receiveMessage__10TLiveActorFP9THitActorUl
  .4byte getTakingMtx__10TLiveActorFv
  .4byte ensureTakeSituation__10TTakeActorFv
  .4byte moveRequest__10TTakeActorFRCQ29JGeometry8TVec3$$0f$$1
  .4byte getRadiusAtY__10TTakeActorCFf
  .4byte belongToGround__10TLiveActorCFv
  .4byte getRootJointMtx__10TLiveActorCFv
  .4byte init__10TLiveActorFP12TLiveManager
  .4byte calcRootMatrix__10TLiveActorFv
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
  .4byte getBasNameTable__22TBossHanachanPartsBaseCFv
  .4byte 0

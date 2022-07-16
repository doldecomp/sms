.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global setParamInitDataPointer__18JAIGlobalParameterFPv
setParamInitDataPointer__18JAIGlobalParameterFPv:
/* 80054AE0 00051A20  80 8D 8F A0 */	lwz r4, basic__8JAIBasic@sda21(r13)
/* 80054AE4 00051A24  38 00 00 04 */	li r0, 4
/* 80054AE8 00051A28  90 64 00 4C */	stw r3, 0x4c(r4)
/* 80054AEC 00051A2C  80 6D 8F A0 */	lwz r3, basic__8JAIBasic@sda21(r13)
/* 80054AF0 00051A30  98 03 00 13 */	stb r0, 0x13(r3)
/* 80054AF4 00051A34  4E 80 00 20 */	blr 

.global setParamSeqPlayTrackMax__18JAIGlobalParameterFUl
setParamSeqPlayTrackMax__18JAIGlobalParameterFUl:
/* 80054AF8 00051A38  54 60 08 3C */	slwi r0, r3, 1
/* 80054AFC 00051A3C  90 6D 81 08 */	stw r3, seqPlayTrackMax__18JAIGlobalParameter@sda21(r13)
/* 80054B00 00051A40  90 0D 81 0C */	stw r0, seqControlBufferMax__18JAIGlobalParameter@sda21(r13)
/* 80054B04 00051A44  4E 80 00 20 */	blr 

.global setParamSeqControlBufferMax__18JAIGlobalParameterFUl
setParamSeqControlBufferMax__18JAIGlobalParameterFUl:
/* 80054B08 00051A48  90 6D 81 0C */	stw r3, seqControlBufferMax__18JAIGlobalParameter@sda21(r13)
/* 80054B0C 00051A4C  4E 80 00 20 */	blr 

.global setParamAutoHeapMax__18JAIGlobalParameterFUl
setParamAutoHeapMax__18JAIGlobalParameterFUl:
/* 80054B10 00051A50  90 6D 81 18 */	stw r3, autoHeapMax__18JAIGlobalParameter@sda21(r13)
/* 80054B14 00051A54  4E 80 00 20 */	blr 

.global setParamStayHeapMax__18JAIGlobalParameterFUl
setParamStayHeapMax__18JAIGlobalParameterFUl:
/* 80054B18 00051A58  90 6D 81 1C */	stw r3, stayHeapMax__18JAIGlobalParameter@sda21(r13)
/* 80054B1C 00051A5C  4E 80 00 20 */	blr 

.global setParamInputGainDown__18JAIGlobalParameterFf
setParamInputGainDown__18JAIGlobalParameterFf:
/* 80054B20 00051A60  D0 2D 81 48 */	stfs f1, inputGainDown__18JAIGlobalParameter@sda21(r13)
/* 80054B24 00051A64  4E 80 00 20 */	blr 

.global setParamOutputGainUp__18JAIGlobalParameterFf
setParamOutputGainUp__18JAIGlobalParameterFf:
/* 80054B28 00051A68  D0 2D 81 4C */	stfs f1, outputGainUp__18JAIGlobalParameter@sda21(r13)
/* 80054B2C 00051A6C  4E 80 00 20 */	blr 

.global setParamDistanceMax__18JAIGlobalParameterFf
setParamDistanceMax__18JAIGlobalParameterFf:
/* 80054B30 00051A70  D0 2D 81 50 */	stfs f1, distanceMax__18JAIGlobalParameter@sda21(r13)
/* 80054B34 00051A74  4E 80 00 20 */	blr 

.global setParamMaxVolumeDistance__18JAIGlobalParameterFf
setParamMaxVolumeDistance__18JAIGlobalParameterFf:
/* 80054B38 00051A78  D0 2D 81 54 */	stfs f1, maxVolumeDistance__18JAIGlobalParameter@sda21(r13)
/* 80054B3C 00051A7C  4E 80 00 20 */	blr 

.global setParamMinDistanceVolume__18JAIGlobalParameterFf
setParamMinDistanceVolume__18JAIGlobalParameterFf:
/* 80054B40 00051A80  D0 2D 90 6C */	stfs f1, minDistanceVolume__18JAIGlobalParameter@sda21(r13)
/* 80054B44 00051A84  4E 80 00 20 */	blr 

.global setParamStreamInsideBufferCut__18JAIGlobalParameterFb
setParamStreamInsideBufferCut__18JAIGlobalParameterFb:
/* 80054B48 00051A88  80 8D 8F A0 */	lwz r4, basic__8JAIBasic@sda21(r13)
/* 80054B4C 00051A8C  88 04 00 1C */	lbz r0, 0x1c(r4)
/* 80054B50 00051A90  50 60 0F BC */	rlwimi r0, r3, 1, 0x1e, 0x1e
/* 80054B54 00051A94  98 04 00 1C */	stb r0, 0x1c(r4)
/* 80054B58 00051A98  4E 80 00 20 */	blr 

.global setParamAutoHeapRoomSize__18JAIGlobalParameterFUl
setParamAutoHeapRoomSize__18JAIGlobalParameterFUl:
/* 80054B5C 00051A9C  90 6D 81 20 */	stw r3, autoHeapRoomSize__18JAIGlobalParameter@sda21(r13)
/* 80054B60 00051AA0  4E 80 00 20 */	blr 

.global setParamStayHeapSize__18JAIGlobalParameterFUl
setParamStayHeapSize__18JAIGlobalParameterFUl:
/* 80054B64 00051AA4  90 6D 81 24 */	stw r3, stayHeapSize__18JAIGlobalParameter@sda21(r13)
/* 80054B68 00051AA8  4E 80 00 20 */	blr 

.global setParamInitDataFileName__18JAIGlobalParameterFPc
setParamInitDataFileName__18JAIGlobalParameterFPc:
/* 80054B6C 00051AAC  90 6D 81 34 */	stw r3, initDataFileName__18JAIGlobalParameter@sda21(r13)
/* 80054B70 00051AB0  4E 80 00 20 */	blr 

.global setParamWavePath__18JAIGlobalParameterFPc
setParamWavePath__18JAIGlobalParameterFPc:
/* 80054B74 00051AB4  90 6D 81 38 */	stw r3, wavePath__18JAIGlobalParameter@sda21(r13)
/* 80054B78 00051AB8  4E 80 00 20 */	blr 

.global setParamSequenceArchivesPath__18JAIGlobalParameterFPc
setParamSequenceArchivesPath__18JAIGlobalParameterFPc:
/* 80054B7C 00051ABC  90 6D 81 3C */	stw r3, sequenceArchivesPath__18JAIGlobalParameter@sda21(r13)
/* 80054B80 00051AC0  4E 80 00 20 */	blr 

.global setParamStreamPath__18JAIGlobalParameterFPc
setParamStreamPath__18JAIGlobalParameterFPc:
/* 80054B84 00051AC4  90 6D 81 40 */	stw r3, streamPath__18JAIGlobalParameter@sda21(r13)
/* 80054B88 00051AC8  4E 80 00 20 */	blr 

.global setParamSystemTrackMax__18JAIGlobalParameterFl
setParamSystemTrackMax__18JAIGlobalParameterFl:
/* 80054B8C 00051ACC  90 6D 81 74 */	stw r3, systemTrackMax__18JAIGlobalParameter@sda21(r13)
/* 80054B90 00051AD0  4E 80 00 20 */	blr 

.global setParamSystemRootTrackMax__18JAIGlobalParameterFl
setParamSystemRootTrackMax__18JAIGlobalParameterFl:
/* 80054B94 00051AD4  90 6D 81 78 */	stw r3, systemRootTrackMax__18JAIGlobalParameter@sda21(r13)
/* 80054B98 00051AD8  4E 80 00 20 */	blr 

.global setParamSoundOutputMode__18JAIGlobalParameterFUl
setParamSoundOutputMode__18JAIGlobalParameterFUl:
/* 80054B9C 00051ADC  7C 08 02 A6 */	mflr r0
/* 80054BA0 00051AE0  2C 03 00 01 */	cmpwi r3, 1
/* 80054BA4 00051AE4  90 01 00 04 */	stw r0, 4(r1)
/* 80054BA8 00051AE8  38 A0 00 01 */	li r5, 1
/* 80054BAC 00051AEC  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 80054BB0 00051AF0  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 80054BB4 00051AF4  3B E0 00 00 */	li r31, 0
/* 80054BB8 00051AF8  41 82 00 2C */	beq lbl_80054BE4
/* 80054BBC 00051AFC  40 80 00 10 */	bge lbl_80054BCC
/* 80054BC0 00051B00  2C 03 00 00 */	cmpwi r3, 0
/* 80054BC4 00051B04  40 80 00 14 */	bge lbl_80054BD8
/* 80054BC8 00051B08  48 00 00 30 */	b lbl_80054BF8
lbl_80054BCC:
/* 80054BCC 00051B0C  2C 03 00 03 */	cmpwi r3, 3
/* 80054BD0 00051B10  40 80 00 28 */	bge lbl_80054BF8
/* 80054BD4 00051B14  48 00 00 1C */	b lbl_80054BF0
lbl_80054BD8:
/* 80054BD8 00051B18  38 A0 00 00 */	li r5, 0
/* 80054BDC 00051B1C  3B E0 00 00 */	li r31, 0
/* 80054BE0 00051B20  48 00 00 18 */	b lbl_80054BF8
lbl_80054BE4:
/* 80054BE4 00051B24  38 A0 00 01 */	li r5, 1
/* 80054BE8 00051B28  3B E0 00 01 */	li r31, 1
/* 80054BEC 00051B2C  48 00 00 0C */	b lbl_80054BF8
lbl_80054BF0:
/* 80054BF0 00051B30  38 A0 00 02 */	li r5, 2
/* 80054BF4 00051B34  3B E0 00 01 */	li r31, 1
lbl_80054BF8:
/* 80054BF8 00051B38  80 8D 8F A0 */	lwz r4, basic__8JAIBasic@sda21(r13)
/* 80054BFC 00051B3C  54 60 06 3E */	clrlwi r0, r3, 0x18
/* 80054C00 00051B40  38 65 00 00 */	addi r3, r5, 0
/* 80054C04 00051B44  98 04 00 14 */	stb r0, 0x14(r4)
/* 80054C08 00051B48  48 00 9E 19 */	bl setOutputMode__Q28JASystem6DriverFUl
/* 80054C0C 00051B4C  7F E3 FB 78 */	mr r3, r31
/* 80054C10 00051B50  4B FF F1 AD */	bl setOutputMode__Q27JAInter9StreamLibFUl
/* 80054C14 00051B54  80 01 00 24 */	lwz r0, 0x24(r1)
/* 80054C18 00051B58  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 80054C1C 00051B5C  38 21 00 20 */	addi r1, r1, 0x20
/* 80054C20 00051B60  7C 08 03 A6 */	mtlr r0
/* 80054C24 00051B64  4E 80 00 20 */	blr 

.global getParamSeCategoryMax__18JAIGlobalParameterFv
getParamSeCategoryMax__18JAIGlobalParameterFv:
/* 80054C28 00051B68  80 6D 8F A0 */	lwz r3, basic__8JAIBasic@sda21(r13)
/* 80054C2C 00051B6C  80 63 00 00 */	lwz r3, 0(r3)
/* 80054C30 00051B70  88 63 00 89 */	lbz r3, 0x89(r3)
/* 80054C34 00051B74  4E 80 00 20 */	blr 

.global getParamSeqTrackMax__18JAIGlobalParameterFv
getParamSeqTrackMax__18JAIGlobalParameterFv:
/* 80054C38 00051B78  80 6D 81 04 */	lwz r3, seqTrackMax__18JAIGlobalParameter@sda21(r13)
/* 80054C3C 00051B7C  4E 80 00 20 */	blr 

.global getParamOutputGainUp__18JAIGlobalParameterFv
getParamOutputGainUp__18JAIGlobalParameterFv:
/* 80054C40 00051B80  C0 2D 81 4C */	lfs f1, outputGainUp__18JAIGlobalParameter@sda21(r13)
/* 80054C44 00051B84  4E 80 00 20 */	blr 

.global getParamMaxVolumeDistance__18JAIGlobalParameterFv
getParamMaxVolumeDistance__18JAIGlobalParameterFv:
/* 80054C48 00051B88  C0 2D 81 54 */	lfs f1, maxVolumeDistance__18JAIGlobalParameter@sda21(r13)
/* 80054C4C 00051B8C  4E 80 00 20 */	blr 

.section .rodata, "a"  # 0x8036FFA0 - 0x803A8380
.balign 8
"@530":
	.asciz "Seqs/JaiSeInf.bst"
	.balign 4
"@531":
	.asciz "Seqs/JaiSqInf.bst"
	.balign 4
"@532":
	.asciz "Seqs/JaiStInf.bst"
	.balign 4
"@533":
	.asciz "JaiInit.aaf"
"@537":
	.asciz "JaiArcS.hed"
	.4byte 0

.section .sdata, "wa"  # 0x80408AC0 - 0x804097C0
.global distanceParameterMoveTime__18JAIGlobalParameter
distanceParameterMoveTime__18JAIGlobalParameter:
	.byte 0x03
.global audioSystemThreadPriority__18JAIGlobalParameter
audioSystemThreadPriority__18JAIGlobalParameter:
	.byte 0x02
.global audioDvdThreadPriority__18JAIGlobalParameter
audioDvdThreadPriority__18JAIGlobalParameter:
	.byte 0x03
.global seqMuteVolumeSePlay__18JAIGlobalParameter
seqMuteVolumeSePlay__18JAIGlobalParameter:
	.byte 0x28
.global seDistanceFxParameter__18JAIGlobalParameter
seDistanceFxParameter__18JAIGlobalParameter:
	.4byte 0x00960000
.global soundSceneMax__18JAIGlobalParameter
soundSceneMax__18JAIGlobalParameter:
	.4byte 0x00000002
.global seRegistMax__18JAIGlobalParameter
seRegistMax__18JAIGlobalParameter:
	.4byte 0x00000014
.global seTrackMax__18JAIGlobalParameter
seTrackMax__18JAIGlobalParameter:
	.4byte 0x00000020
.global seqTrackMax__18JAIGlobalParameter
seqTrackMax__18JAIGlobalParameter:
	.4byte 0x00000020
.global seqPlayTrackMax__18JAIGlobalParameter
seqPlayTrackMax__18JAIGlobalParameter:
	.4byte 0x00000008
.global seqControlBufferMax__18JAIGlobalParameter
seqControlBufferMax__18JAIGlobalParameter:
	.4byte 0x00000010
.global streamControlBufferMax__18JAIGlobalParameter
streamControlBufferMax__18JAIGlobalParameter:
	.4byte 0x00000008
.global streamParameterBufferMax__18JAIGlobalParameter
streamParameterBufferMax__18JAIGlobalParameter:
	.4byte 0x00000010
.global autoHeapMax__18JAIGlobalParameter
autoHeapMax__18JAIGlobalParameter:
	.4byte 0x00000002
.global stayHeapMax__18JAIGlobalParameter
stayHeapMax__18JAIGlobalParameter:
	.4byte 0x00000002
.global autoHeapRoomSize__18JAIGlobalParameter
autoHeapRoomSize__18JAIGlobalParameter:
	.4byte 0x0000C000
.global stayHeapSize__18JAIGlobalParameter
stayHeapSize__18JAIGlobalParameter:
	.4byte 0x00010000
.global seInfoFileName__18JAIGlobalParameter
seInfoFileName__18JAIGlobalParameter:
  .4byte "@530"
.global seqInfoFileName__18JAIGlobalParameter
seqInfoFileName__18JAIGlobalParameter:
  .4byte "@531"
.global streamInfoFileName__18JAIGlobalParameter
streamInfoFileName__18JAIGlobalParameter:
  .4byte "@532"
.global initDataFileName__18JAIGlobalParameter
initDataFileName__18JAIGlobalParameter:
  .4byte "@533"
.global wavePath__18JAIGlobalParameter
wavePath__18JAIGlobalParameter:
  .4byte "@534"
.global sequenceArchivesPath__18JAIGlobalParameter
sequenceArchivesPath__18JAIGlobalParameter:
  .4byte "@535"
.global streamPath__18JAIGlobalParameter
streamPath__18JAIGlobalParameter:
  .4byte "@536"
.global sequenceArchivesFileName__18JAIGlobalParameter
sequenceArchivesFileName__18JAIGlobalParameter:
  .4byte "@537"
.global inputGainDown__18JAIGlobalParameter
inputGainDown__18JAIGlobalParameter:
	.4byte 0x3F000000
.global outputGainUp__18JAIGlobalParameter
outputGainUp__18JAIGlobalParameter:
	.4byte 0x3F99999A
.global distanceMax__18JAIGlobalParameter
distanceMax__18JAIGlobalParameter:
	.4byte 0x459C4000
.global maxVolumeDistance__18JAIGlobalParameter
maxVolumeDistance__18JAIGlobalParameter:
	.4byte 0x447A0000
.global seDolbyCenterValue__18JAIGlobalParameter
seDolbyCenterValue__18JAIGlobalParameter:
	.4byte 0x41F00000
.global seDolbyFrontDistanceMax__18JAIGlobalParameter
seDolbyFrontDistanceMax__18JAIGlobalParameter:
	.4byte 0xC3960000
.global seDolbyBehindDistanceMax__18JAIGlobalParameter
seDolbyBehindDistanceMax__18JAIGlobalParameter:
	.4byte 0x447A0000
.global dopplarMoveTime__18JAIGlobalParameter
dopplarMoveTime__18JAIGlobalParameter:
	.4byte 0x0000000F
.global dummyPositionMax__18JAIGlobalParameter
dummyPositionMax__18JAIGlobalParameter:
	.4byte 0x0000000A
.global seqMuteMoveSpeedSePlay__18JAIGlobalParameter
seqMuteMoveSpeedSePlay__18JAIGlobalParameter:
	.4byte 0x00000003
.global audioCameraMax__18JAIGlobalParameter
audioCameraMax__18JAIGlobalParameter:
	.4byte 0x00000001
.global systemTrackMax__18JAIGlobalParameter
systemTrackMax__18JAIGlobalParameter:
	.4byte 0x00000100
.global systemRootTrackMax__18JAIGlobalParameter
systemRootTrackMax__18JAIGlobalParameter:
	.4byte 0x00000010
.global panDistanceMax__18JAIGlobalParameter
panDistanceMax__18JAIGlobalParameter:
	.4byte 0x43FA0000
.global panDistance2Max__18JAIGlobalParameter
panDistance2Max__18JAIGlobalParameter:
	.4byte 0x447A0000
.global panAngleParameter__18JAIGlobalParameter
panAngleParameter__18JAIGlobalParameter:
	.4byte 0x41400000
.global panAngleParameter2__18JAIGlobalParameter
panAngleParameter2__18JAIGlobalParameter:
	.4byte 0x4019999A
.global dopplarParameter__18JAIGlobalParameter
dopplarParameter__18JAIGlobalParameter:
	.4byte 0x45480000
.global seDistanceWaitMax__18JAIGlobalParameter
seDistanceWaitMax__18JAIGlobalParameter:
	.4byte 0x000C0000
.global seDistancepitchMax__18JAIGlobalParameter
seDistancepitchMax__18JAIGlobalParameter:
	.4byte 0x3E4CCCCD

.section .sdata2, "a"  # 0x8040B460 - 0x80414020
.balign 8
"@534":
	.asciz "Banks/"
	.balign 4
"@535":
	.asciz "Seqs/"
	.balign 4
"@536":
	.asciz "Stream/"

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
.global seDefaultFx__18JAIGlobalParameter
seDefaultFx__18JAIGlobalParameter:
	.skip 0x4
.global interfaceHeapSize__18JAIGlobalParameter
interfaceHeapSize__18JAIGlobalParameter:
	.skip 0x4
.global audioResPath__18JAIGlobalParameter
audioResPath__18JAIGlobalParameter:
	.skip 0x4
.global minDistanceVolume__18JAIGlobalParameter
minDistanceVolume__18JAIGlobalParameter:
	.skip 0x4

.include "macros.inc"

.section .sdata, "wa"  # 0x80408AC0 - 0x804097C0
.global rootPath$1763
rootPath$1763:
	.incbin "baserom.dol", 0x3E3000, 0x8
.global sMessageBuffer__12JUTException
sMessageBuffer__12JUTException:
	.incbin "baserom.dol", 0x3E3008, 0x8
.global mStickMode__10JUTGamePad
mStickMode__10JUTGamePad:
	.incbin "baserom.dol", 0x3E3010, 0x4
.global sResetPattern__Q210JUTGamePad13C3ButtonReset
sResetPattern__Q210JUTGamePad13C3ButtonReset:
	.incbin "baserom.dol", 0x3E3014, 0x4
.global Unit01
Unit01:
	.incbin "baserom.dol", 0x3E3018, 0x8
.global J3DUnit01
J3DUnit01:
	.incbin "baserom.dol", 0x3E3020, 0x8
.global errorRetry__12JKRDvdRipper
errorRetry__12JKRDvdRipper:
	.incbin "baserom.dol", 0x3E3028, 0x4
.global szpBufferSize__12JKRDvdRipper
szpBufferSize__12JKRDvdRipper:
	.incbin "baserom.dol", 0x3E302C, 0x4
.global errorRetry__16JKRDvdAramRipper
errorRetry__16JKRDvdAramRipper:
	.incbin "baserom.dol", 0x3E3030, 0x4
.global szpBufferSize__16JKRDvdAramRipper
szpBufferSize__16JKRDvdAramRipper:
	.incbin "baserom.dol", 0x3E3034, 0x4
.global GXTexMode0Ids
GXTexMode0Ids:
	.incbin "baserom.dol", 0x3E3038, 0x8
.global GXTexMode1Ids
GXTexMode1Ids:
	.incbin "baserom.dol", 0x3E3040, 0x8
.global GXTexImage0Ids
GXTexImage0Ids:
	.incbin "baserom.dol", 0x3E3048, 0x8
.global GXTexImage1Ids
GXTexImage1Ids:
	.incbin "baserom.dol", 0x3E3050, 0x8
.global GXTexImage2Ids
GXTexImage2Ids:
	.incbin "baserom.dol", 0x3E3058, 0x8
.global GXTexImage3Ids
GXTexImage3Ids:
	.incbin "baserom.dol", 0x3E3060, 0x8
.global GXTexTlutIds
GXTexTlutIds:
	.incbin "baserom.dol", 0x3E3068, 0x8
.global GX2HWFiltConv
GX2HWFiltConv:
	.incbin "baserom.dol", 0x3E3070, 0x8
.global j3dDefaultColInfo
j3dDefaultColInfo:
	.incbin "baserom.dol", 0x3E3078, 0x4
.global j3dDefaultAmbInfo
j3dDefaultAmbInfo:
	.incbin "baserom.dol", 0x3E307C, 0x4
.global j3dDefaultColorChanNum
j3dDefaultColorChanNum:
	.incbin "baserom.dol", 0x3E3080, 0x4
.global j3dDefaultTevOrderInfoNull
j3dDefaultTevOrderInfoNull:
	.incbin "baserom.dol", 0x3E3084, 0x4
.global j3dDefaultIndTexOrderNull
j3dDefaultIndTexOrderNull:
	.incbin "baserom.dol", 0x3E3088, 0x4
.global j3dDefaultTevColor
j3dDefaultTevColor:
	.incbin "baserom.dol", 0x3E308C, 0x4
.global lbl_80408B50
lbl_80408B50:
	.incbin "baserom.dol", 0x3E3090, 0x4
.global j3dDefaultIndTexCoordScaleInfo
j3dDefaultIndTexCoordScaleInfo:
	.incbin "baserom.dol", 0x3E3094, 0x4
.global j3dDefaultTevKColor
j3dDefaultTevKColor:
	.incbin "baserom.dol", 0x3E3098, 0x4
.global j3dDefaultTevSwapMode
j3dDefaultTevSwapMode:
	.incbin "baserom.dol", 0x3E309C, 0x4
.global j3dDefaultTevSwapModeTable
j3dDefaultTevSwapModeTable:
	.incbin "baserom.dol", 0x3E30A0, 0x4
.global j3dDefaultBlendInfo
j3dDefaultBlendInfo:
	.incbin "baserom.dol", 0x3E30A4, 0x4
.global j3dDefaultColorChanInfo
j3dDefaultColorChanInfo:
	.incbin "baserom.dol", 0x3E30A8, 0x8
.global j3dDefaultTevSwapTableID
j3dDefaultTevSwapTableID:
	.incbin "baserom.dol", 0x3E30B0, 0x2
.global j3dDefaultAlphaCmpID
j3dDefaultAlphaCmpID:
	.incbin "baserom.dol", 0x3E30B2, 0x2
.global j3dDefaultZModeID
j3dDefaultZModeID:
	.incbin "baserom.dol", 0x3E30B4, 0x4
.global mVisible__Q212JUTAssertion9$$2unnamed$$2
mVisible__Q212JUTAssertion9$$2unnamed$$2:
	.incbin "baserom.dol", 0x3E30B8, 0x8
.global JRNTexImage1Ids
JRNTexImage1Ids:
	.incbin "baserom.dol", 0x3E30C0, 0x8
.global JRNTexImage2Ids
JRNTexImage2Ids:
	.incbin "baserom.dol", 0x3E30C8, 0x8
.global cEqualCSlope__7JALCalc
cEqualCSlope__7JALCalc:
	.incbin "baserom.dol", 0x3E30D0, 0x4
.global cPlusPSlope__7JALCalc
cPlusPSlope__7JALCalc:
	.incbin "baserom.dol", 0x3E30D4, 0x4
.global asnFileName__Q27JAInter8TAsnData
asnFileName__Q27JAInter8TAsnData:
	.incbin "baserom.dol", 0x3E30D8, 0x8
.global LOOP_BLOCKS__Q27JAInter9StreamLib
LOOP_BLOCKS__Q27JAInter9StreamLib:
	.incbin "baserom.dol", 0x3E30E0, 0x4
.global LOOP_SAMPLESIZE__Q27JAInter9StreamLib
LOOP_SAMPLESIZE__Q27JAInter9StreamLib:
	.incbin "baserom.dol", 0x3E30E4, 0x4
.global outputmode__Q27JAInter9StreamLib
outputmode__Q27JAInter9StreamLib:
	.incbin "baserom.dol", 0x3E30E8, 0x8
.global distanceParameterMoveTime__18JAIGlobalParameter
distanceParameterMoveTime__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E30F0, 0x1
.global audioSystemThreadPriority__18JAIGlobalParameter
audioSystemThreadPriority__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E30F1, 0x1
.global audioDvdThreadPriority__18JAIGlobalParameter
audioDvdThreadPriority__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E30F2, 0x1
.global seqMuteVolumeSePlay__18JAIGlobalParameter
seqMuteVolumeSePlay__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E30F3, 0x1
.global seDistanceFxParameter__18JAIGlobalParameter
seDistanceFxParameter__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E30F4, 0x4
.global soundSceneMax__18JAIGlobalParameter
soundSceneMax__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E30F8, 0x4
.global seRegistMax__18JAIGlobalParameter
seRegistMax__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E30FC, 0x4
.global seTrackMax__18JAIGlobalParameter
seTrackMax__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3100, 0x4
.global seqTrackMax__18JAIGlobalParameter
seqTrackMax__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3104, 0x4
.global seqPlayTrackMax__18JAIGlobalParameter
seqPlayTrackMax__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3108, 0x4
.global seqControlBufferMax__18JAIGlobalParameter
seqControlBufferMax__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E310C, 0x4
.global streamControlBufferMax__18JAIGlobalParameter
streamControlBufferMax__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3110, 0x4
.global streamParameterBufferMax__18JAIGlobalParameter
streamParameterBufferMax__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3114, 0x4
.global autoHeapMax__18JAIGlobalParameter
autoHeapMax__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3118, 0x4
.global stayHeapMax__18JAIGlobalParameter
stayHeapMax__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E311C, 0x4
.global autoHeapRoomSize__18JAIGlobalParameter
autoHeapRoomSize__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3120, 0x4
.global stayHeapSize__18JAIGlobalParameter
stayHeapSize__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3124, 0x4
.global seInfoFileName__18JAIGlobalParameter
seInfoFileName__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3128, 0x4
.global seqInfoFileName__18JAIGlobalParameter
seqInfoFileName__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E312C, 0x4
.global streamInfoFileName__18JAIGlobalParameter
streamInfoFileName__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3130, 0x4
.global initDataFileName__18JAIGlobalParameter
initDataFileName__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3134, 0x4
.global wavePath__18JAIGlobalParameter
wavePath__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3138, 0x4
.global sequenceArchivesPath__18JAIGlobalParameter
sequenceArchivesPath__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E313C, 0x4
.global streamPath__18JAIGlobalParameter
streamPath__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3140, 0x4
.global sequenceArchivesFileName__18JAIGlobalParameter
sequenceArchivesFileName__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3144, 0x4
.global inputGainDown__18JAIGlobalParameter
inputGainDown__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3148, 0x4
.global outputGainUp__18JAIGlobalParameter
outputGainUp__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E314C, 0x4
.global distanceMax__18JAIGlobalParameter
distanceMax__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3150, 0x4
.global maxVolumeDistance__18JAIGlobalParameter
maxVolumeDistance__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3154, 0x4
.global seDolbyCenterValue__18JAIGlobalParameter
seDolbyCenterValue__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3158, 0x4
.global seDolbyFrontDistanceMax__18JAIGlobalParameter
seDolbyFrontDistanceMax__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E315C, 0x4
.global seDolbyBehindDistanceMax__18JAIGlobalParameter
seDolbyBehindDistanceMax__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3160, 0x4
.global dopplarMoveTime__18JAIGlobalParameter
dopplarMoveTime__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3164, 0x4
.global dummyPositionMax__18JAIGlobalParameter
dummyPositionMax__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3168, 0x4
.global seqMuteMoveSpeedSePlay__18JAIGlobalParameter
seqMuteMoveSpeedSePlay__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E316C, 0x4
.global audioCameraMax__18JAIGlobalParameter
audioCameraMax__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3170, 0x4
.global systemTrackMax__18JAIGlobalParameter
systemTrackMax__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3174, 0x4
.global systemRootTrackMax__18JAIGlobalParameter
systemRootTrackMax__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3178, 0x4
.global panDistanceMax__18JAIGlobalParameter
panDistanceMax__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E317C, 0x4
.global panDistance2Max__18JAIGlobalParameter
panDistance2Max__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3180, 0x4
.global panAngleParameter__18JAIGlobalParameter
panAngleParameter__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3184, 0x4
.global panAngleParameter2__18JAIGlobalParameter
panAngleParameter2__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3188, 0x4
.global dopplarParameter__18JAIGlobalParameter
dopplarParameter__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E318C, 0x4
.global seDistanceWaitMax__18JAIGlobalParameter
seDistanceWaitMax__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3190, 0x4
.global seDistancepitchMax__18JAIGlobalParameter
seDistancepitchMax__18JAIGlobalParameter:
	.incbin "baserom.dol", 0x3E3194, 0x4
.global waitMax__Q38JASystem6Driver8DSPQueue
waitMax__Q38JASystem6Driver8DSPQueue:
	.incbin "baserom.dol", 0x3E3198, 0x8
.global MAX_MIXERLEVEL__Q28JASystem6Driver
MAX_MIXERLEVEL__Q28JASystem6Driver:
	.incbin "baserom.dol", 0x3E31A0, 0x4
.global JAS_SYSTEM_OUTPUT_MODE__Q28JASystem6Driver
JAS_SYSTEM_OUTPUT_MODE__Q28JASystem6Driver:
	.incbin "baserom.dol", 0x3E31A4, 0x4
.global JAS_UPDATE_INTERVAL__Q28JASystem6Driver
JAS_UPDATE_INTERVAL__Q28JASystem6Driver:
	.incbin "baserom.dol", 0x3E31A8, 0x4
.global DSP_VOICE_WAITS_MAX__Q28JASystem6Driver
DSP_VOICE_WAITS_MAX__Q28JASystem6Driver:
	.incbin "baserom.dol", 0x3E31AC, 0x4
.global smnFree__Q28JASystem11TDSPChannel
smnFree__Q28JASystem11TDSPChannel:
	.incbin "baserom.dol", 0x3E31B0, 0x4
.global DSP_LIMIT_RATIO__8JASystem
DSP_LIMIT_RATIO__8JASystem:
	.incbin "baserom.dol", 0x3E31B4, 0x4
.global JAS_DSP_PREFIX__Q28JASystem12DSPInterface
JAS_DSP_PREFIX__Q28JASystem12DSPInterface:
	.incbin "baserom.dol", 0x3E31B8, 0x4
.global COMP_BLOCKSAMPLES$234
COMP_BLOCKSAMPLES$234:
	.incbin "baserom.dol", 0x3E31BC, 0x8
.global COMP_BLOCKBYTES$235
COMP_BLOCKBYTES$235:
	.incbin "baserom.dol", 0x3E31C4, 0xC
.global maxCallbacksUser__Q28JASystem6Kernel
maxCallbacksUser__Q28JASystem6Kernel:
	.incbin "baserom.dol", 0x3E31D0, 0x8
.global dvdMsgsSize__Q28JASystem3Dvd
dvdMsgsSize__Q28JASystem3Dvd:
	.incbin "baserom.dol", 0x3E31D8, 0x4
.global dvdBufSize__Q28JASystem3Dvd
dvdBufSize__Q28JASystem3Dvd:
	.incbin "baserom.dol", 0x3E31DC, 0x4
.global maxDics__Q28JASystem3Dvd
maxDics__Q28JASystem3Dvd:
	.incbin "baserom.dol", 0x3E31E0, 0x8
.global playListMax__Q28JASystem10HardStream
playListMax__Q28JASystem10HardStream:
	.incbin "baserom.dol", 0x3E31E8, 0x8
.global gDacRate__Q28JASystem6Kernel
gDacRate__Q28JASystem6Kernel:
	.incbin "baserom.dol", 0x3E31F0, 0x4
.global gSubFrames__Q28JASystem6Kernel
gSubFrames__Q28JASystem6Kernel:
	.incbin "baserom.dol", 0x3E31F4, 0x4
.global gFrameSamples__Q28JASystem6Kernel
gFrameSamples__Q28JASystem6Kernel:
	.incbin "baserom.dol", 0x3E31F8, 0x4
.global gDacSize__Q28JASystem6Kernel
gDacSize__Q28JASystem6Kernel:
	.incbin "baserom.dol", 0x3E31FC, 0x4
.global audioDramSize__Q28JASystem6Kernel
audioDramSize__Q28JASystem6Kernel:
	.incbin "baserom.dol", 0x3E3200, 0x4
.global audioAramSize__Q28JASystem6Kernel
audioAramSize__Q28JASystem6Kernel:
	.incbin "baserom.dol", 0x3E3204, 0x4
.global vlMaxArcs__Q28JASystem5Vload
vlMaxArcs__Q28JASystem5Vload:
	.incbin "baserom.dol", 0x3E3208, 0x8
.global sV0__Q28JASystem6Player
sV0__Q28JASystem6Player:
	.incbin "baserom.dol", 0x3E3210, 0x4
.global sV1__Q28JASystem6Player
sV1__Q28JASystem6Player:
	.incbin "baserom.dol", 0x3E3214, 0x4
.global DSP_MIXERLEVEL
DSP_MIXERLEVEL:
	.incbin "baserom.dol", 0x3E3218, 0x8
.global fragmentID
fragmentID:
	.incbin "baserom.dol", 0x3E3220, 0x8
.global $$2wstringBase0
$$2wstringBase0:
	.incbin "baserom.dol", 0x3E3228, 0x8
.global next
next:
	.incbin "baserom.dol", 0x3E3230, 0x8
.global K1
K1:
	.incbin "baserom.dol", 0x3E3238, 0x4
.global K2
K2:
	.incbin "baserom.dol", 0x3E323C, 0x4
.global __OSCurrHeap
__OSCurrHeap:
	.incbin "baserom.dol", 0x3E3240, 0x8
.global __OSArenaLo
__OSArenaLo:
	.incbin "baserom.dol", 0x3E3248, 0x8
.global $$232
$$232:
	.incbin "baserom.dol", 0x3E3250, 0x8
.global fontEncode$80
fontEncode$80:
	.incbin "baserom.dol", 0x3E3258, 0x8
.global Unit01
Unit01:
	.incbin "baserom.dol", 0x3E3260, 0x8
.global FirstRead
FirstRead:
	.incbin "baserom.dol", 0x3E3268, 0x8
.global $$2116
$$2116:
	.incbin "baserom.dol", 0x3E3270, 0x8
.global autoInvalidation
autoInvalidation:
	.incbin "baserom.dol", 0x3E3278, 0x8
.global $$235
$$235:
	.incbin "baserom.dol", 0x3E3280, 0x4
.global $$240
$$240:
	.incbin "baserom.dol", 0x3E3284, 0x4
.global $$241
$$241:
	.incbin "baserom.dol", 0x3E3288, 0x8
.global ClampRegion
ClampRegion:
	.incbin "baserom.dol", 0x3E3290, 0x8
.global ResettingChan
ResettingChan:
	.incbin "baserom.dol", 0x3E3298, 0x4
.global XPatchBits
XPatchBits:
	.incbin "baserom.dol", 0x3E329C, 0x4
.global AnalogMode
AnalogMode:
	.incbin "baserom.dol", 0x3E32A0, 0x4
.global Spec
Spec:
	.incbin "baserom.dol", 0x3E32A4, 0x4
.global MakeStatus
MakeStatus:
	.incbin "baserom.dol", 0x3E32A8, 0x4
.global CmdReadOrigin
CmdReadOrigin:
	.incbin "baserom.dol", 0x3E32AC, 0x4
.global CmdCalibrate
CmdCalibrate:
	.incbin "baserom.dol", 0x3E32B0, 0x8
.global next
next:
	.incbin "baserom.dol", 0x3E32B8, 0x8
.global __CARDVendorID
__CARDVendorID:
	.incbin "baserom.dol", 0x3E32C0, 0x8
.global gx
gx:
	.incbin "baserom.dol", 0x3E32C8, 0x8
.global tbl1$215
tbl1$215:
	.incbin "baserom.dol", 0x3E32D0, 0x4
.global tbl2$216
tbl2$216:
	.incbin "baserom.dol", 0x3E32D4, 0x4
.global tbl3$217
tbl3$217:
	.incbin "baserom.dol", 0x3E32D8, 0x8
.global GX2HWFiltConv
GX2HWFiltConv:
	.incbin "baserom.dol", 0x3E32E0, 0x8
.global SendCount
SendCount:
	.incbin "baserom.dol", 0x3E32E8, 0x8
.global cm2hw$400
cm2hw$400:
	.incbin "baserom.dol", 0x3E32F0, 0x4
.global cm2hw$515
cm2hw$515:
	.incbin "baserom.dol", 0x3E32F4, 0xC
.global uv
uv:
	.incbin "baserom.dol", 0x3E3300, 0x8
.global className$1930
className$1930:
	.incbin "baserom.dol", 0x3E3308, 0x8
.global className$1944
className$1944:
	.incbin "baserom.dol", 0x3E3310, 0x8
.global mSquareShadowHeight__19TMBindShadowManager
mSquareShadowHeight__19TMBindShadowManager:
	.incbin "baserom.dol", 0x3E3318, 0x4
.global mTreeScale__19TMBindShadowManager
mTreeScale__19TMBindShadowManager:
	.incbin "baserom.dol", 0x3E331C, 0x4
.global mYScalePlus__19TMBindShadowManager
mYScalePlus__19TMBindShadowManager:
	.incbin "baserom.dol", 0x3E3320, 0x8
.global ColorBlack
ColorBlack:
	.incbin "baserom.dol", 0x3E3328, 0x4
.global ColorWhite
ColorWhite:
	.incbin "baserom.dol", 0x3E332C, 0x4
.global mMotorCountLimit__9RumbleMgr
mMotorCountLimit__9RumbleMgr:
	.incbin "baserom.dol", 0x3E3330, 0x4
.global mMotorTimerPeriod__9RumbleMgr
mMotorTimerPeriod__9RumbleMgr:
	.incbin "baserom.dol", 0x3E3334, 0x4
.global mPowerThreshold__9RumbleMgr
mPowerThreshold__9RumbleMgr:
	.incbin "baserom.dol", 0x3E3338, 0x8
.global rumblePoint_00
rumblePoint_00:
	.incbin "baserom.dol", 0x3E3340, 0x4
.global rumblePoint_01
rumblePoint_01:
	.incbin "baserom.dol", 0x3E3344, 0x4
.global rumblePoint_02
rumblePoint_02:
	.incbin "baserom.dol", 0x3E3348, 0x4
.global rumbleFrame_02
rumbleFrame_02:
	.incbin "baserom.dol", 0x3E334C, 0x8
.global rumblePower_02
rumblePower_02:
	.incbin "baserom.dol", 0x3E3354, 0x8
.global rumblePoint_03
rumblePoint_03:
	.incbin "baserom.dol", 0x3E335C, 0x4
.global rumbleFrame_03
rumbleFrame_03:
	.incbin "baserom.dol", 0x3E3360, 0x8
.global rumblePower_03
rumblePower_03:
	.incbin "baserom.dol", 0x3E3368, 0x8
.global rumblePoint_04
rumblePoint_04:
	.incbin "baserom.dol", 0x3E3370, 0x4
.global rumblePoint_05
rumblePoint_05:
	.incbin "baserom.dol", 0x3E3374, 0x4
.global rumblePoint_06
rumblePoint_06:
	.incbin "baserom.dol", 0x3E3378, 0x4
.global rumblePoint_07
rumblePoint_07:
	.incbin "baserom.dol", 0x3E337C, 0x4
.global rumblePoint_08
rumblePoint_08:
	.incbin "baserom.dol", 0x3E3380, 0x4
.global rumblePoint_09
rumblePoint_09:
	.incbin "baserom.dol", 0x3E3384, 0x4
.global rumblePoint_10
rumblePoint_10:
	.incbin "baserom.dol", 0x3E3388, 0x4
.global rumbleFrame_10
rumbleFrame_10:
	.incbin "baserom.dol", 0x3E338C, 0x4
.global rumblePower_10
rumblePower_10:
	.incbin "baserom.dol", 0x3E3390, 0x4
.global rumblePoint_11
rumblePoint_11:
	.incbin "baserom.dol", 0x3E3394, 0x4
.global rumbleFrame_11
rumbleFrame_11:
	.incbin "baserom.dol", 0x3E3398, 0x4
.global rumblePower_11
rumblePower_11:
	.incbin "baserom.dol", 0x3E339C, 0x4
.global rumblePoint_12
rumblePoint_12:
	.incbin "baserom.dol", 0x3E33A0, 0x4
.global rumbleFrame_12
rumbleFrame_12:
	.incbin "baserom.dol", 0x3E33A4, 0x4
.global rumblePower_12
rumblePower_12:
	.incbin "baserom.dol", 0x3E33A8, 0x4
.global rumblePoint_13
rumblePoint_13:
	.incbin "baserom.dol", 0x3E33AC, 0x4
.global rumbleFrame_13
rumbleFrame_13:
	.incbin "baserom.dol", 0x3E33B0, 0x4
.global rumblePower_13
rumblePower_13:
	.incbin "baserom.dol", 0x3E33B4, 0x4
.global rumblePoint_14
rumblePoint_14:
	.incbin "baserom.dol", 0x3E33B8, 0x4
.global rumbleFrame_14
rumbleFrame_14:
	.incbin "baserom.dol", 0x3E33BC, 0x4
.global rumblePower_14
rumblePower_14:
	.incbin "baserom.dol", 0x3E33C0, 0x4
.global rumblePoint_15
rumblePoint_15:
	.incbin "baserom.dol", 0x3E33C4, 0x4
.global rumbleFrame_15
rumbleFrame_15:
	.incbin "baserom.dol", 0x3E33C8, 0x4
.global rumblePower_15
rumblePower_15:
	.incbin "baserom.dol", 0x3E33CC, 0x4
.global rumblePoint_16
rumblePoint_16:
	.incbin "baserom.dol", 0x3E33D0, 0x4
.global rumbleFrame_16
rumbleFrame_16:
	.incbin "baserom.dol", 0x3E33D4, 0x4
.global rumblePower_16
rumblePower_16:
	.incbin "baserom.dol", 0x3E33D8, 0x4
.global rumblePoint_17
rumblePoint_17:
	.incbin "baserom.dol", 0x3E33DC, 0x4
.global rumbleFrame_17
rumbleFrame_17:
	.incbin "baserom.dol", 0x3E33E0, 0x4
.global rumblePower_17
rumblePower_17:
	.incbin "baserom.dol", 0x3E33E4, 0x4
.global rumblePoint_18
rumblePoint_18:
	.incbin "baserom.dol", 0x3E33E8, 0x4
.global rumbleFrame_18
rumbleFrame_18:
	.incbin "baserom.dol", 0x3E33EC, 0x4
.global rumblePower_18
rumblePower_18:
	.incbin "baserom.dol", 0x3E33F0, 0x4
.global rumblePoint_19
rumblePoint_19:
	.incbin "baserom.dol", 0x3E33F4, 0x4
.global rumblePoint_20
rumblePoint_20:
	.incbin "baserom.dol", 0x3E33F8, 0x4
.global rumbleFrame_20
rumbleFrame_20:
	.incbin "baserom.dol", 0x3E33FC, 0x8
.global rumblePower_20
rumblePower_20:
	.incbin "baserom.dol", 0x3E3404, 0x8
.global rumblePoint_21
rumblePoint_21:
	.incbin "baserom.dol", 0x3E340C, 0x4
.global rumbleFrame_21
rumbleFrame_21:
	.incbin "baserom.dol", 0x3E3410, 0x8
.global rumblePower_21
rumblePower_21:
	.incbin "baserom.dol", 0x3E3418, 0x8
.global rumblePoint_22
rumblePoint_22:
	.incbin "baserom.dol", 0x3E3420, 0x4
.global rumbleFrame_22
rumbleFrame_22:
	.incbin "baserom.dol", 0x3E3424, 0x8
.global rumblePower_22
rumblePower_22:
	.incbin "baserom.dol", 0x3E342C, 0x8
.global rumblePoint_23
rumblePoint_23:
	.incbin "baserom.dol", 0x3E3434, 0x4
.global rumblePoint_24
rumblePoint_24:
	.incbin "baserom.dol", 0x3E3438, 0x4
.global channelNum
channelNum:
	.incbin "baserom.dol", 0x3E343C, 0x4
.global $$21211
$$21211:
	.incbin "baserom.dol", 0x3E3440, 0x8
.global sShineViewObjName$3431
sShineViewObjName$3431:
	.incbin "baserom.dol", 0x3E3448, 0x8
.global stages$3022
stages$3022:
	.incbin "baserom.dol", 0x3E3450, 0x8
.global cCameraBckNameShineGetInside
cCameraBckNameShineGetInside:
	.incbin "baserom.dol", 0x3E3458, 0x4
.global cCameraBckNameShineGetOutside
cCameraBckNameShineGetOutside:
	.incbin "baserom.dol", 0x3E345C, 0x4
.global cCameraBckNameGate
cCameraBckNameGate:
	.incbin "baserom.dol", 0x3E3460, 0x8
.global cSunVolumeName
cSunVolumeName:
	.incbin "baserom.dol", 0x3E3468, 0x4
.global cSunsetVolumeName
cSunsetVolumeName:
	.incbin "baserom.dol", 0x3E346C, 0x4
.global cTargetArrowBmdFileName
cTargetArrowBmdFileName:
	.incbin "baserom.dol", 0x3E3470, 0x8
.global mVelocityMinY__10TLiveActor
mVelocityMinY__10TLiveActor:
	.incbin "baserom.dol", 0x3E3478, 0x8
.global cNpcPartsNameRootJoint
cNpcPartsNameRootJoint:
	.incbin "baserom.dol", 0x3E3480, 0x8
.global cDummyPollutionTexName
cDummyPollutionTexName:
	.incbin "baserom.dol", 0x3E3488, 0x4
.global cRealPollutionTexName
cRealPollutionTexName:
	.incbin "baserom.dol", 0x3E348C, 0x4
.global cMonteMDummyStrawTexName
cMonteMDummyStrawTexName:
	.incbin "baserom.dol", 0x3E3490, 0x4
.global cMonteWDummyStrawTexName
cMonteWDummyStrawTexName:
	.incbin "baserom.dol", 0x3E3494, 0x4
.global cMonteMRealStrawTexName
cMonteMRealStrawTexName:
	.incbin "baserom.dol", 0x3E3498, 0x4
.global cMonteWRealStrawTexName
cMonteWRealStrawTexName:
	.incbin "baserom.dol", 0x3E349C, 0x4
.global cScreenTexViewObjName
cScreenTexViewObjName:
	.incbin "baserom.dol", 0x3E34A0, 0x4
.global cJellyFishDummyScreenTexName
cJellyFishDummyScreenTexName:
	.incbin "baserom.dol", 0x3E34A4, 0x4
.global cJellyFishDummyTexName
cJellyFishDummyTexName:
	.incbin "baserom.dol", 0x3E34A8, 0x4
.global cJellyFishRealTexName
cJellyFishRealTexName:
	.incbin "baserom.dol", 0x3E34AC, 0x4
.global cMonteMCommonVolumeName
cMonteMCommonVolumeName:
	.incbin "baserom.dol", 0x3E34B0, 0x4
.global cMonteWCommonVolumeName
cMonteWCommonVolumeName:
	.incbin "baserom.dol", 0x3E34B4, 0x4
.global cMareMCommonVolumeName
cMareMCommonVolumeName:
	.incbin "baserom.dol", 0x3E34B8, 0x4
.global cMareWCommonVolumeName
cMareWCommonVolumeName:
	.incbin "baserom.dol", 0x3E34BC, 0x4
.global cMareCommonNormalBmtName
cMareCommonNormalBmtName:
	.incbin "baserom.dol", 0x3E34C0, 0x4
.global cMareCommonPollutionBmtName
cMareCommonPollutionBmtName:
	.incbin "baserom.dol", 0x3E34C4, 0x4
.global cManiyaParentViewObjName
cManiyaParentViewObjName:
	.incbin "baserom.dol", 0x3E34C8, 0x4
.global cManiyaChildViewObjName
cManiyaChildViewObjName:
	.incbin "baserom.dol", 0x3E34CC, 0x4
.global cNotUseFastCubeViewObjName0
cNotUseFastCubeViewObjName0:
	.incbin "baserom.dol", 0x3E34D0, 0x4
.global cNotUseFastCubeViewObjName1
cNotUseFastCubeViewObjName1:
	.incbin "baserom.dol", 0x3E34D4, 0x4
.global cEyeMaterialName
cEyeMaterialName:
	.incbin "baserom.dol", 0x3E34D8, 0x4
.global cNeckJointName
cNeckJointName:
	.incbin "baserom.dol", 0x3E34DC, 0x4
.global sWaistJointName$localstatic0$setMtxEffect___8TBaseNPCFv
sWaistJointName$localstatic0$setMtxEffect___8TBaseNPCFv:
	.incbin "baserom.dol", 0x3E34E0, 0x8
.global cPeachPartsTextureName
cPeachPartsTextureName:
	.incbin "baserom.dol", 0x3E34E8, 0x4
.global cPeachHostTextureName
cPeachHostTextureName:
	.incbin "baserom.dol", 0x3E34EC, 0x4
.global monteMG_bastable
monteMG_bastable:
	.incbin "baserom.dol", 0x3E34F0, 0x8
.global mareMD_bastable
mareMD_bastable:
	.incbin "baserom.dol", 0x3E34F8, 0x8
.global smMainVolume__5MSBgm
smMainVolume__5MSBgm:
	.incbin "baserom.dol", 0x3E3500, 0x8
.global cPan_MaxAmp__8MSHandle
cPan_MaxAmp__8MSHandle:
	.incbin "baserom.dol", 0x3E3508, 0x4
.global cPan_CAdjust__8MSHandle
cPan_CAdjust__8MSHandle:
	.incbin "baserom.dol", 0x3E350C, 0x4
.global cPan_CShift__8MSHandle
cPan_CShift__8MSHandle:
	.incbin "baserom.dol", 0x3E3510, 0x4
.global cPan_HiSence_Dist__8MSHandle
cPan_HiSence_Dist__8MSHandle:
	.incbin "baserom.dol", 0x3E3514, 0x4
.global cMS_DistanceMax_Sence__8MSHandle
cMS_DistanceMax_Sence__8MSHandle:
	.incbin "baserom.dol", 0x3E3518, 0x4
.global cDol_0Rad__8MSHandle
cDol_0Rad__8MSHandle:
	.incbin "baserom.dol", 0x3E351C, 0x4
.global cDol_HalfRad__8MSHandle
cDol_HalfRad__8MSHandle:
	.incbin "baserom.dol", 0x3E3520, 0x4
.global cDol_FullRad__8MSHandle
cDol_FullRad__8MSHandle:
	.incbin "baserom.dol", 0x3E3524, 0x4
.global mWaitAppearTime__11TWoodBarrel
mWaitAppearTime__11TWoodBarrel:
	.incbin "baserom.dol", 0x3E3528, 0x4
.global mBreakTime__11TWoodBarrel
mBreakTime__11TWoodBarrel:
	.incbin "baserom.dol", 0x3E352C, 0x4
.global mOilBarrelFlushTime__11TWoodBarrel
mOilBarrelFlushTime__11TWoodBarrel:
	.incbin "baserom.dol", 0x3E3530, 0x8
.global streetlamp_sink_data
streetlamp_sink_data:
	.incbin "baserom.dol", 0x3E3538, 0x8
.global lamp_bianco_sink_data
lamp_bianco_sink_data:
	.incbin "baserom.dol", 0x3E3540, 0x8
.global mNormalLivingTime__14TMapObjGeneral
mNormalLivingTime__14TMapObjGeneral:
	.incbin "baserom.dol", 0x3E3548, 0x4
.global mNormalFlushTime__14TMapObjGeneral
mNormalFlushTime__14TMapObjGeneral:
	.incbin "baserom.dol", 0x3E354C, 0x4
.global mNormalFlushInterval__14TMapObjGeneral
mNormalFlushInterval__14TMapObjGeneral:
	.incbin "baserom.dol", 0x3E3550, 0x4
.global mNormalWaitToAppearTime__14TMapObjGeneral
mNormalWaitToAppearTime__14TMapObjGeneral:
	.incbin "baserom.dol", 0x3E3554, 0x4
.global mNormalAppearingScaleUp__14TMapObjGeneral
mNormalAppearingScaleUp__14TMapObjGeneral:
	.incbin "baserom.dol", 0x3E3558, 0x4
.global mNormalThrowSpeedRate__14TMapObjGeneral
mNormalThrowSpeedRate__14TMapObjGeneral:
	.incbin "baserom.dol", 0x3E355C, 0x4
.global mWaveSpeed__14TMapObjLibWave
mWaveSpeed__14TMapObjLibWave:
	.incbin "baserom.dol", 0x3E3560, 0x8
.global mAppearedScaleSpeed__5TItem
mAppearedScaleSpeed__5TItem:
	.incbin "baserom.dol", 0x3E3568, 0x4
.global mCircleRateY__6TShine
mCircleRateY__6TShine:
	.incbin "baserom.dol", 0x3E356C, 0x4
.global mUpSpeed__6TShine
mUpSpeed__6TShine:
	.incbin "baserom.dol", 0x3E3570, 0x4
.global mSpeedDownRate__6TShine
mSpeedDownRate__6TShine:
	.incbin "baserom.dol", 0x3E3574, 0x4
.global mDownHeight__8TManhole
mDownHeight__8TManhole:
	.incbin "baserom.dol", 0x3E3578, 0x4
.global mDownSpeed__8TManhole
mDownSpeed__8TManhole:
	.incbin "baserom.dol", 0x3E357C, 0x4
.global mVibrationSpeed__8TManhole
mVibrationSpeed__8TManhole:
	.incbin "baserom.dol", 0x3E3580, 0x4
.global mVibrationEndHeight__8TManhole
mVibrationEndHeight__8TManhole:
	.incbin "baserom.dol", 0x3E3584, 0x4
.global mVibrationDecreaseRate__8TManhole
mVibrationDecreaseRate__8TManhole:
	.incbin "baserom.dol", 0x3E3588, 0x8
.global mSandScaleUp__10TSandBlock
mSandScaleUp__10TSandBlock:
	.incbin "baserom.dol", 0x3E3590, 0x4
.global mSandScaleDown__10TSandBlock
mSandScaleDown__10TSandBlock:
	.incbin "baserom.dol", 0x3E3594, 0x4
.global mSandScaleMin__10TSandBlock
mSandScaleMin__10TSandBlock:
	.incbin "baserom.dol", 0x3E3598, 0x4
.global mWaitTimeToFall__10TSandBlock
mWaitTimeToFall__10TSandBlock:
	.incbin "baserom.dol", 0x3E359C, 0x4
.global mSandWaitTime__10TSandBlock
mSandWaitTime__10TSandBlock:
	.incbin "baserom.dol", 0x3E35A0, 0x4
.global mMeltSpeedWater__9TIceBlock
mMeltSpeedWater__9TIceBlock:
	.incbin "baserom.dol", 0x3E35A4, 0x4
.global mMeltSpeedAuto__9TIceBlock
mMeltSpeedAuto__9TIceBlock:
	.incbin "baserom.dol", 0x3E35A8, 0x4
.global mAutoMeltScale__9TIceBlock
mAutoMeltScale__9TIceBlock:
	.incbin "baserom.dol", 0x3E35AC, 0x4
.global sRadius
sRadius:
	.incbin "baserom.dol", 0x3E35B0, 0x4
.global sSubZ
sSubZ:
	.incbin "baserom.dol", 0x3E35B4, 0x4
.global sSpeed
sSpeed:
	.incbin "baserom.dol", 0x3E35B8, 0x4
.global mTremblePower__17TMapObjRootPakkun
mTremblePower__17TMapObjRootPakkun:
	.incbin "baserom.dol", 0x3E35BC, 0x4
.global mTrembleAccel__17TMapObjRootPakkun
mTrembleAccel__17TMapObjRootPakkun:
	.incbin "baserom.dol", 0x3E35C0, 0x4
.global mTrembleBrake__17TMapObjRootPakkun
mTrembleBrake__17TMapObjRootPakkun:
	.incbin "baserom.dol", 0x3E35C4, 0x4
.global mTrembleTime__17TMapObjRootPakkun
mTrembleTime__17TMapObjRootPakkun:
	.incbin "baserom.dol", 0x3E35C8, 0x4
.global mRotAccel__24TBiancoWatermillVertical
mRotAccel__24TBiancoWatermillVertical:
	.incbin "baserom.dol", 0x3E35CC, 0x4
.global mRotSpeedDownRate__24TBiancoWatermillVertical
mRotSpeedDownRate__24TBiancoWatermillVertical:
	.incbin "baserom.dol", 0x3E35D0, 0x4
.global mRotSpeedMax__24TBiancoWatermillVertical
mRotSpeedMax__24TBiancoWatermillVertical:
	.incbin "baserom.dol", 0x3E35D4, 0x4
.global mBridgeRotRate__24TBiancoWatermillVertical
mBridgeRotRate__24TBiancoWatermillVertical:
	.incbin "baserom.dol", 0x3E35D8, 0x4
.global mRotWaterAccel__19TBiancoMiniWindmill
mRotWaterAccel__19TBiancoMiniWindmill:
	.incbin "baserom.dol", 0x3E35DC, 0x4
.global mFriction__19TBiancoMiniWindmill
mFriction__19TBiancoMiniWindmill:
	.incbin "baserom.dol", 0x3E35E0, 0x4
.global mRotSpeedMax__19TBiancoMiniWindmill
mRotSpeedMax__19TBiancoMiniWindmill:
	.incbin "baserom.dol", 0x3E35E4, 0x4
.global sMessengerPosZ
sMessengerPosZ:
	.incbin "baserom.dol", 0x3E35E8, 0x4
.global sMessengerPosY
sMessengerPosY:
	.incbin "baserom.dol", 0x3E35EC, 0x4
.global mAlphaDownSpeed__15TLeafBoatRotten
mAlphaDownSpeed__15TLeafBoatRotten:
	.incbin "baserom.dol", 0x3E35F0, 0x4
.global mCollisionRemoveAlpha__15TLeafBoatRotten
mCollisionRemoveAlpha__15TLeafBoatRotten:
	.incbin "baserom.dol", 0x3E35F4, 0x4
.global mRottenColor__15TLeafBoatRotten
mRottenColor__15TLeafBoatRotten:
	.incbin "baserom.dol", 0x3E35F8, 0x8
.global mWaitTime__10TCraneRotY
mWaitTime__10TCraneRotY:
	.incbin "baserom.dol", 0x3E3600, 0x4
.global mRotSpeed__12TCraneUpDown
mRotSpeed__12TCraneUpDown:
	.incbin "baserom.dol", 0x3E3604, 0x4
.global mWaitTime__12TCraneUpDown
mWaitTime__12TCraneUpDown:
	.incbin "baserom.dol", 0x3E3608, 0x4
.global mRotAccel__15TRiccoWatermill
mRotAccel__15TRiccoWatermill:
	.incbin "baserom.dol", 0x3E360C, 0x4
.global mRotSpeedMaxUp__15TRiccoWatermill
mRotSpeedMaxUp__15TRiccoWatermill:
	.incbin "baserom.dol", 0x3E3610, 0x4
.global mRotSpeedMaxDown__15TRiccoWatermill
mRotSpeedMaxDown__15TRiccoWatermill:
	.incbin "baserom.dol", 0x3E3614, 0x4
.global mRotDown__15TRiccoWatermill
mRotDown__15TRiccoWatermill:
	.incbin "baserom.dol", 0x3E3618, 0x4
.global mSubmarineMoveRate__15TRiccoWatermill
mSubmarineMoveRate__15TRiccoWatermill:
	.incbin "baserom.dol", 0x3E361C, 0x4
.global mSubmarineMaxTransY__15TRiccoWatermill
mSubmarineMaxTransY__15TRiccoWatermill:
	.incbin "baserom.dol", 0x3E3620, 0x4
.global mSubmarineBottomTransY__15TRiccoWatermill
mSubmarineBottomTransY__15TRiccoWatermill:
	.incbin "baserom.dol", 0x3E3624, 0x4
.global mWaitTime__15TRiccoWatermill
mWaitTime__15TRiccoWatermill:
	.incbin "baserom.dol", 0x3E3628, 0x4
.global mObjSpeedXZ__14TFruitLauncher
mObjSpeedXZ__14TFruitLauncher:
	.incbin "baserom.dol", 0x3E362C, 0x4
.global mObjSpeedY__14TFruitLauncher
mObjSpeedY__14TFruitLauncher:
	.incbin "baserom.dol", 0x3E3630, 0x4
.global mFruitLiveTime__14TFruitLauncher
mFruitLiveTime__14TFruitLauncher:
	.incbin "baserom.dol", 0x3E3634, 0x4
.global mWitherTime__9TSandBase
mWitherTime__9TSandBase:
	.incbin "baserom.dol", 0x3E3638, 0x4
.global mScaleMin__9TSandBase
mScaleMin__9TSandBase:
	.incbin "baserom.dol", 0x3E363C, 0x4
.global mFiringFrameSpeed__13TSandBombBase
mFiringFrameSpeed__13TSandBombBase:
	.incbin "baserom.dol", 0x3E3640, 0x4
.global mFiringFrameDownSpeed__13TSandBombBase
mFiringFrameDownSpeed__13TSandBombBase:
	.incbin "baserom.dol", 0x3E3644, 0x4
.global mExplodeFrameSpeed__13TSandBombBase
mExplodeFrameSpeed__13TSandBombBase:
	.incbin "baserom.dol", 0x3E3648, 0x4
.global mMarioJumpRate__13TSandBombBase
mMarioJumpRate__13TSandBombBase:
	.incbin "baserom.dol", 0x3E364C, 0x4
.global mExlodingRumbleTime__13TSandBombBase
mExlodingRumbleTime__13TSandBombBase:
	.incbin "baserom.dol", 0x3E3650, 0x4
.global mCollisionRate__11TSandCastle
mCollisionRate__11TSandCastle:
	.incbin "baserom.dol", 0x3E3654, 0x4
.global mGoTargetTime__11TLeanMirror
mGoTargetTime__11TLeanMirror:
	.incbin "baserom.dol", 0x3E3658, 0x4
.global mDemoWaitTime__11TLeanMirror
mDemoWaitTime__11TLeanMirror:
	.incbin "baserom.dol", 0x3E365C, 0x4
.global mDemoLightTime__11TLeanMirror
mDemoLightTime__11TLeanMirror:
	.incbin "baserom.dol", 0x3E3660, 0x4
.global mRotSpeed__17TMammaBlockRotate
mRotSpeed__17TMammaBlockRotate:
	.incbin "baserom.dol", 0x3E3664, 0x4
.global mRotReturnSpeed__17TMammaBlockRotate
mRotReturnSpeed__17TMammaBlockRotate:
	.incbin "baserom.dol", 0x3E3668, 0x4
.global mRotEnd__17TMammaBlockRotate
mRotEnd__17TMammaBlockRotate:
	.incbin "baserom.dol", 0x3E366C, 0x4
.global mMapGoSpeed__17TMammaBlockRotate
mMapGoSpeed__17TMammaBlockRotate:
	.incbin "baserom.dol", 0x3E3670, 0x4
.global mMapBackSpeed__17TMammaBlockRotate
mMapBackSpeed__17TMammaBlockRotate:
	.incbin "baserom.dol", 0x3E3674, 0x4
.global mWaitTime__17TMammaBlockRotate
mWaitTime__17TMammaBlockRotate:
	.incbin "baserom.dol", 0x3E3678, 0x8
.global mOpenRotMax__9TShellCup
mOpenRotMax__9TShellCup:
	.incbin "baserom.dol", 0x3E3680, 0x4
.global mShellDamageRot__9TShellCup
mShellDamageRot__9TShellCup:
	.incbin "baserom.dol", 0x3E3684, 0x4
.global mWaterOpenAccel__9TShellCup
mWaterOpenAccel__9TShellCup:
	.incbin "baserom.dol", 0x3E3688, 0x4
.global mCloseAccel__9TShellCup
mCloseAccel__9TShellCup:
	.incbin "baserom.dol", 0x3E368C, 0x4
.global mRotSpeed__13TMerrygoround
mRotSpeed__13TMerrygoround:
	.incbin "baserom.dol", 0x3E3690, 0x8
.global mWaterLeakSpeed__14TCogwheelScale
mWaterLeakSpeed__14TCogwheelScale:
	.incbin "baserom.dol", 0x3E3698, 0x4
.global sRadius
sRadius:
	.incbin "baserom.dol", 0x3E369C, 0x4
.global mRopeWidthX__9TCogwheel
mRopeWidthX__9TCogwheel:
	.incbin "baserom.dol", 0x3E36A0, 0x4
.global mRopeWidthZ__9TCogwheel
mRopeWidthZ__9TCogwheel:
	.incbin "baserom.dol", 0x3E36A4, 0x4
.global mTexPosRate__9TCogwheel
mTexPosRate__9TCogwheel:
	.incbin "baserom.dol", 0x3E36A8, 0x4
.global mMinSpeed__9TCogwheel
mMinSpeed__9TCogwheel:
	.incbin "baserom.dol", 0x3E36AC, 0x4
.global mGrowStartFrame
mGrowStartFrame:
	.incbin "baserom.dol", 0x3E36B0, 0x4
.global mGrowEndFrame
mGrowEndFrame:
	.incbin "baserom.dol", 0x3E36B4, 0x4
.global mFlutterSpeed__11TMapObjFlag
mFlutterSpeed__11TMapObjFlag:
	.incbin "baserom.dol", 0x3E36B8, 0x8
.global sAlphaCompLarge
sAlphaCompLarge:
	.incbin "baserom.dol", 0x3E36C0, 0x1
.global sAlphaCompSmall
sAlphaCompSmall:
	.incbin "baserom.dol", 0x3E36C1, 0x7
.global mHipDropDownRate__12TMapObjPlane
mHipDropDownRate__12TMapObjPlane:
	.incbin "baserom.dol", 0x3E36C8, 0x4
.global mWaterDownRate__12TMapObjPlane
mWaterDownRate__12TMapObjPlane:
	.incbin "baserom.dol", 0x3E36CC, 0x4
.global mTexScale__12TMapObjPlane
mTexScale__12TMapObjPlane:
	.incbin "baserom.dol", 0x3E36D0, 0x4
.global mAmbColor__12TMapObjPlane
mAmbColor__12TMapObjPlane:
	.incbin "baserom.dol", 0x3E36D4, 0x4
.global mMoveSpeedDefault__10TRideCloud
mMoveSpeedDefault__10TRideCloud:
	.incbin "baserom.dol", 0x3E36D8, 0x4
.global mCushionLen__10TRideCloud
mCushionLen__10TRideCloud:
	.incbin "baserom.dol", 0x3E36DC, 0x4
.global mCushionSpeed__10TRideCloud
mCushionSpeed__10TRideCloud:
	.incbin "baserom.dol", 0x3E36E0, 0x4
.global mScaleSpeed__10TRideCloud
mScaleSpeed__10TRideCloud:
	.incbin "baserom.dol", 0x3E36E4, 0x4
.global mFruitLivingTime__11TResetFruit
mFruitLivingTime__11TResetFruit:
	.incbin "baserom.dol", 0x3E36E8, 0x4
.global mScaleUpSpeed__11TResetFruit
mScaleUpSpeed__11TResetFruit:
	.incbin "baserom.dol", 0x3E36EC, 0x4
.global mBreakingScaleSpeed__11TResetFruit
mBreakingScaleSpeed__11TResetFruit:
	.incbin "baserom.dol", 0x3E36F0, 0x4
.global mFruitWaitTimeToAppear__11TResetFruit
mFruitWaitTimeToAppear__11TResetFruit:
	.incbin "baserom.dol", 0x3E36F4, 0xC
.global mWidth__19TMapObjGrassManager
mWidth__19TMapObjGrassManager:
	.incbin "baserom.dol", 0x3E3700, 0x4
.global mSwingWidth__19TMapObjGrassManager
mSwingWidth__19TMapObjGrassManager:
	.incbin "baserom.dol", 0x3E3704, 0x4
.global mSwingSpeed__19TMapObjGrassManager
mSwingSpeed__19TMapObjGrassManager:
	.incbin "baserom.dol", 0x3E3708, 0x4
.global mDistNear__19TMapObjGrassManager
mDistNear__19TMapObjGrassManager:
	.incbin "baserom.dol", 0x3E370C, 0x4
.global mDistFar__19TMapObjGrassManager
mDistFar__19TMapObjGrassManager:
	.incbin "baserom.dol", 0x3E3710, 0x4
.global sGrassAddTime
sGrassAddTime:
	.incbin "baserom.dol", 0x3E3714, 0xC
.global color_table
color_table:
	.incbin "baserom.dol", 0x3E3720, 0x8
.global mSpeed__20TRevolvingFenceInner
mSpeed__20TRevolvingFenceInner:
	.incbin "baserom.dol", 0x3E3728, 0x4
.global mWaterAccel__11TFenceWater
mWaterAccel__11TFenceWater:
	.incbin "baserom.dol", 0x3E372C, 0x4
.global mBackSpeed__11TFenceWater
mBackSpeed__11TFenceWater:
	.incbin "baserom.dol", 0x3E3730, 0x4
.global mTurnedWaitTime__11TFenceWater
mTurnedWaitTime__11TFenceWater:
	.incbin "baserom.dol", 0x3E3734, 0x4
.global mFallHeight__10TRailFence
mFallHeight__10TRailFence:
	.incbin "baserom.dol", 0x3E3738, 0x4
.global mWaitTime__10TRailFence
mWaitTime__10TRailFence:
	.incbin "baserom.dol", 0x3E373C, 0x4
.global sRumbleTime
sRumbleTime:
	.incbin "baserom.dol", 0x3E3740, 0x8
.global mMarioAccelY__19THangingBridgeBoard
mMarioAccelY__19THangingBridgeBoard:
	.incbin "baserom.dol", 0x3E3748, 0x4
.global mMarioHipDropAccelY__19THangingBridgeBoard
mMarioHipDropAccelY__19THangingBridgeBoard:
	.incbin "baserom.dol", 0x3E374C, 0x4
.global mReturnAccelRate__19THangingBridgeBoard
mReturnAccelRate__19THangingBridgeBoard:
	.incbin "baserom.dol", 0x3E3750, 0x4
.global mSpeedDownRate__19THangingBridgeBoard
mSpeedDownRate__19THangingBridgeBoard:
	.incbin "baserom.dol", 0x3E3754, 0x4
.global mRopeWidthX__19THangingBridgeBoard
mRopeWidthX__19THangingBridgeBoard:
	.incbin "baserom.dol", 0x3E3758, 0x4
.global mRopeWidthZ__19THangingBridgeBoard
mRopeWidthZ__19THangingBridgeBoard:
	.incbin "baserom.dol", 0x3E375C, 0x4
.global mTexPosRate__19THangingBridgeBoard
mTexPosRate__19THangingBridgeBoard:
	.incbin "baserom.dol", 0x3E3760, 0x4
.global mRopeWidthBetweenBoards__14THangingBridge
mRopeWidthBetweenBoards__14THangingBridge:
	.incbin "baserom.dol", 0x3E3764, 0x4
.global mRopeWidthBetweenBoardsY__14THangingBridge
mRopeWidthBetweenBoardsY__14THangingBridge:
	.incbin "baserom.dol", 0x3E3768, 0x4
.global mPointNumBetweenBoards__14THangingBridge
mPointNumBetweenBoards__14THangingBridge:
	.incbin "baserom.dol", 0x3E376C, 0x4
.global mBetweenBoardsTexPosRate__14THangingBridge
mBetweenBoardsTexPosRate__14THangingBridge:
	.incbin "baserom.dol", 0x3E3770, 0x4
.global mBoardWidth__11TSwingBoard
mBoardWidth__11TSwingBoard:
	.incbin "baserom.dol", 0x3E3774, 0x4
.global mRopeWidthX__11TSwingBoard
mRopeWidthX__11TSwingBoard:
	.incbin "baserom.dol", 0x3E3778, 0x4
.global mRopeWidthZ__11TSwingBoard
mRopeWidthZ__11TSwingBoard:
	.incbin "baserom.dol", 0x3E377C, 0x4
.global mTexPosRate__11TSwingBoard
mTexPosRate__11TSwingBoard:
	.incbin "baserom.dol", 0x3E3780, 0x4
.global mReturnAccelRate__11TSwingBoard
mReturnAccelRate__11TSwingBoard:
	.incbin "baserom.dol", 0x3E3784, 0x4
.global mSpeedDownRate__11TSwingBoard
mSpeedDownRate__11TSwingBoard:
	.incbin "baserom.dol", 0x3E3788, 0x4
.global mScaleUpSpeed__6TFluff
mScaleUpSpeed__6TFluff:
	.incbin "baserom.dol", 0x3E378C, 0x4
.global mScaleDownSpeed__6TFluff
mScaleDownSpeed__6TFluff:
	.incbin "baserom.dol", 0x3E3790, 0x4
.global mWindMin__13TFluffManager
mWindMin__13TFluffManager:
	.incbin "baserom.dol", 0x3E3794, 0x4
.global mBananaTreeJumpPower__11TMapObjTree
mBananaTreeJumpPower__11TMapObjTree:
	.incbin "baserom.dol", 0x3E3798, 0x4
.global mScaleSpeedY__16TMapObjTreeScale
mScaleSpeedY__16TMapObjTreeScale:
	.incbin "baserom.dol", 0x3E379C, 0x4
.global mStatusChangeScaleY__16TMapObjTreeScale
mStatusChangeScaleY__16TMapObjTreeScale:
	.incbin "baserom.dol", 0x3E37A0, 0x4
.global mScaleSpeedXZ__16TMapObjTreeScale
mScaleSpeedXZ__16TMapObjTreeScale:
	.incbin "baserom.dol", 0x3E37A4, 0x4
.global mScaleMin__16TMapObjTreeScale
mScaleMin__16TMapObjTreeScale:
	.incbin "baserom.dol", 0x3E37A8, 0x4
.global sWaitTime
sWaitTime:
	.incbin "baserom.dol", 0x3E37AC, 0x4
.global mDownHeight__11TMapObjNail
mDownHeight__11TMapObjNail:
	.incbin "baserom.dol", 0x3E37B0, 0x8
.global $$22831
$$22831:
	.incbin "baserom.dol", 0x3E37B8, 0x4
.global $$22833
$$22833:
	.incbin "baserom.dol", 0x3E37BC, 0x4
.global mHitPointMax__13TLampTrapIron
mHitPointMax__13TLampTrapIron:
	.incbin "baserom.dol", 0x3E37C0, 0x4
.global mFireTimerMax__13TLampTrapIron
mFireTimerMax__13TLampTrapIron:
	.incbin "baserom.dol", 0x3E37C4, 0x4
.global mCleanedDegree__13TMapEventSink
mCleanedDegree__13TMapEventSink:
	.incbin "baserom.dol", 0x3E37C8, 0x8
.global table$2575
table$2575:
	.incbin "baserom.dol", 0x3E37D0, 0x8
.global mMoveTimerSpeed__8TMapWire
mMoveTimerSpeed__8TMapWire:
	.incbin "baserom.dol", 0x3E37D8, 0x4
.global mDownRateMax__8TMapWire
mDownRateMax__8TMapWire:
	.incbin "baserom.dol", 0x3E37DC, 0x4
.global mEndRate__8TMapWire
mEndRate__8TMapWire:
	.incbin "baserom.dol", 0x3E37E0, 0x4
.global mStretchRate__8TMapWire
mStretchRate__8TMapWire:
	.incbin "baserom.dol", 0x3E37E4, 0x4
.global mHeightRate__8TMapWire
mHeightRate__8TMapWire:
	.incbin "baserom.dol", 0x3E37E8, 0x4
.global mReleaseHeight__8TMapWire
mReleaseHeight__8TMapWire:
	.incbin "baserom.dol", 0x3E37EC, 0x4
.global mFootLength__8TMapWire
mFootLength__8TMapWire:
	.incbin "baserom.dol", 0x3E37F0, 0x4
.global mDrawWidth__8TMapWire
mDrawWidth__8TMapWire:
	.incbin "baserom.dol", 0x3E37F4, 0x4
.global mDrawHeight__8TMapWire
mDrawHeight__8TMapWire:
	.incbin "baserom.dol", 0x3E37F8, 0x8
.global mCommonAttackRadius__13TMapWireActor
mCommonAttackRadius__13TMapWireActor:
	.incbin "baserom.dol", 0x3E3800, 0x4
.global mCommonAttackHeight__13TMapWireActor
mCommonAttackHeight__13TMapWireActor:
	.incbin "baserom.dol", 0x3E3804, 0x4
.global mAreaMinRate__15TPollutionLayer
mAreaMinRate__15TPollutionLayer:
	.incbin "baserom.dol", 0x3E3808, 0x4
.global mSpreadArea__15TPollutionLayer
mSpreadArea__15TPollutionLayer:
	.incbin "baserom.dol", 0x3E380C, 0x4
.global mSpreadFrequency__15TPollutionLayer
mSpreadFrequency__15TPollutionLayer:
	.incbin "baserom.dol", 0x3E3810, 0x4
.global mFireArea__15TPollutionLayer
mFireArea__15TPollutionLayer:
	.incbin "baserom.dol", 0x3E3814, 0x4
.global mFireEffectWaitTime__15TPollutionLayer
mFireEffectWaitTime__15TPollutionLayer:
	.incbin "baserom.dol", 0x3E3818, 0x4
.global mThunderArea__15TPollutionLayer
mThunderArea__15TPollutionLayer:
	.incbin "baserom.dol", 0x3E381C, 0x4
.global mGlassWallArea__15TPollutionLayer
mGlassWallArea__15TPollutionLayer:
	.incbin "baserom.dol", 0x3E3820, 0x4
.global mGlassWallEffectTime__15TPollutionLayer
mGlassWallEffectTime__15TPollutionLayer:
	.incbin "baserom.dol", 0x3E3824, 0x4
.global mFlushTime__17TPollutionManager
mFlushTime__17TPollutionManager:
	.incbin "baserom.dol", 0x3E3828, 0x4
.global mEdgeAlpha__17TPollutionManager
mEdgeAlpha__17TPollutionManager:
	.incbin "baserom.dol", 0x3E382C, 0x4
.global mInterval__19TPollutionLayerWave
mInterval__19TPollutionLayerWave:
	.incbin "baserom.dol", 0x3E3830, 0x4
.global mAlpha__19TPollutionLayerWave
mAlpha__19TPollutionLayerWave:
	.incbin "baserom.dol", 0x3E3834, 0x4
.global mEffectTime__15TPollutionLayer
mEffectTime__15TPollutionLayer:
	.incbin "baserom.dol", 0x3E3838, 0x8
.global mAppearSpeed__13TMareWallRock
mAppearSpeed__13TMareWallRock:
	.incbin "baserom.dol", 0x3E3840, 0x4
.global mDepressSpeed__13TMareWallRock
mDepressSpeed__13TMareWallRock:
	.incbin "baserom.dol", 0x3E3844, 0x4
.global mCleanedDegree__13TMareWallRock
mCleanedDegree__13TMareWallRock:
	.incbin "baserom.dol", 0x3E3848, 0x4
.global mWaitTimeToAppear__13TMareWallRock
mWaitTimeToAppear__13TMareWallRock:
	.incbin "baserom.dol", 0x3E384C, 0x4
.global mWaitTimeToDepress__13TMareWallRock
mWaitTimeToDepress__13TMareWallRock:
	.incbin "baserom.dol", 0x3E3850, 0x4
.global mDepressSpeed__21TMareEventDepressWall
mDepressSpeed__21TMareEventDepressWall:
	.incbin "baserom.dol", 0x3E3854, 0x4
.global mRiseSpeed__21TMareEventDepressWall
mRiseSpeed__21TMareEventDepressWall:
	.incbin "baserom.dol", 0x3E3858, 0x4
.global mWaitTimeToWatch__21TMareEventDepressWall
mWaitTimeToWatch__21TMareEventDepressWall:
	.incbin "baserom.dol", 0x3E385C, 0x4
.global scDolpicNewsDolpic0
scDolpicNewsDolpic0:
	.incbin "baserom.dol", 0x3E3860, 0x8
.global scDolpicNewsDolpic1
scDolpicNewsDolpic1:
	.incbin "baserom.dol", 0x3E3868, 0x8
.global scDolpicNewsDolpic5_2
scDolpicNewsDolpic5_2:
	.incbin "baserom.dol", 0x3E3870, 0x8
.global scDolpicNewsDolpic5_3
scDolpicNewsDolpic5_3:
	.incbin "baserom.dol", 0x3E3878, 0x8
.global scDolpicNewsDolpic8_1
scDolpicNewsDolpic8_1:
	.incbin "baserom.dol", 0x3E3880, 0x8
.global fbuf
fbuf:
	.incbin "baserom.dol", 0x3E3888, 0x4
.global vtable_org
vtable_org:
	.incbin "baserom.dol", 0x3E388C, 0x8
.global dec_step
dec_step:
	.incbin "baserom.dol", 0x3E3894, 0x4
.global inc_step
inc_step:
	.incbin "baserom.dol", 0x3E3898, 0x4
.global fbuf2
fbuf2:
	.incbin "baserom.dol", 0x3E389C, 0x4
.global gmover_tex_buffer
gmover_tex_buffer:
	.incbin "baserom.dol", 0x3E38A0, 0x4
.global mag$259
mag$259:
	.incbin "baserom.dol", 0x3E38A4, 0x4
.global mIsCopyAnmMtx__13TEnemyManager
mIsCopyAnmMtx__13TEnemyManager:
	.incbin "baserom.dol", 0x3E38A8, 0x8
.global mBodyJntIndex__6TGesso
mBodyJntIndex__6TGesso:
	.incbin "baserom.dol", 0x3E38B0, 0x1
.global mMouthJntIndex__6TGesso
mMouthJntIndex__6TGesso:
	.incbin "baserom.dol", 0x3E38B1, 0x3
.global mBodyRotSpeed__6TGesso
mBodyRotSpeed__6TGesso:
	.incbin "baserom.dol", 0x3E38B4, 0x4
.global mPollRange__6TGesso
mPollRange__6TGesso:
	.incbin "baserom.dol", 0x3E38B8, 0x4
.global mThroughHoseiDistY__6TGesso
mThroughHoseiDistY__6TGesso:
	.incbin "baserom.dol", 0x3E38BC, 0x4
.global mCapGravityY__9THamuKuri
mCapGravityY__9THamuKuri:
	.incbin "baserom.dol", 0x3E38C0, 0x4
.global mCapSpeed__9THamuKuri
mCapSpeed__9THamuKuri:
	.incbin "baserom.dol", 0x3E38C4, 0x4
.global mVGenerateGravityY__9THamuKuri
mVGenerateGravityY__9THamuKuri:
	.incbin "baserom.dol", 0x3E38C8, 0x4
.global mLandAnmFrameNum__9THamuKuri
mLandAnmFrameNum__9THamuKuri:
	.incbin "baserom.dol", 0x3E38CC, 0x4
.global mSearchActSw__16THamuKuriManager
mSearchActSw__16THamuKuriManager:
	.incbin "baserom.dol", 0x3E38D0, 0x4
.global mFireHamNoseColorDiff
mFireHamNoseColorDiff:
	.incbin "baserom.dol", 0x3E38D4, 0x8
.global mFireHamOtherColorDiff
mFireHamOtherColorDiff:
	.incbin "baserom.dol", 0x3E38DC, 0xC
.global mannameTable$3222
mannameTable$3222:
	.incbin "baserom.dol", 0x3E38E8, 0x8
.global $$23326
$$23326:
	.incbin "baserom.dol", 0x3E38F0, 0x4
.global $$23328
$$23328:
	.incbin "baserom.dol", 0x3E38F4, 0x4
.global mBaseHoseiTransY__10TMameGesso
mBaseHoseiTransY__10TMameGesso:
	.incbin "baserom.dol", 0x3E38F8, 0x8
.global mExplosionSpeed__16TNameKuriManager
mExplosionSpeed__16TNameKuriManager:
	.incbin "baserom.dol", 0x3E3900, 0x4
.global mStopMinScaleFrame__16TNameKuriManager
mStopMinScaleFrame__16TNameKuriManager:
	.incbin "baserom.dol", 0x3E3904, 0x4
.global mRootExplosionScaleRate__14TPakkunManager
mRootExplosionScaleRate__14TPakkunManager:
	.incbin "baserom.dol", 0x3E3908, 0x4
.global mTestFlyAngX__14TPakkunManager
mTestFlyAngX__14TPakkunManager:
	.incbin "baserom.dol", 0x3E390C, 0x4
.global mIgnoreHitWaterY__14TPakkunManager
mIgnoreHitWaterY__14TPakkunManager:
	.incbin "baserom.dol", 0x3E3910, 0x8
.global mBlockWaitTime__18TSmallEnemyManager
mBlockWaitTime__18TSmallEnemyManager:
	.incbin "baserom.dol", 0x3E3918, 0x4
.global mBlockMoveSpeed__18TSmallEnemyManager
mBlockMoveSpeed__18TSmallEnemyManager:
	.incbin "baserom.dol", 0x3E391C, 0x4
.global mBlockWaitMoveY__18TSmallEnemyManager
mBlockWaitMoveY__18TSmallEnemyManager:
	.incbin "baserom.dol", 0x3E3920, 0x4
.global mChangeBlockTime__18TSmallEnemyManager
mChangeBlockTime__18TSmallEnemyManager:
	.incbin "baserom.dol", 0x3E3924, 0x4
.global mBlockXZScale__18TSmallEnemyManager
mBlockXZScale__18TSmallEnemyManager:
	.incbin "baserom.dol", 0x3E3928, 0x4
.global mBlockYScale__18TSmallEnemyManager
mBlockYScale__18TSmallEnemyManager:
	.incbin "baserom.dol", 0x3E392C, 0x4
.global mIsPolluter__11TSmallEnemy
mIsPolluter__11TSmallEnemy:
	.incbin "baserom.dol", 0x3E3930, 0x1
.global mIsAmpPolluter__11TSmallEnemy
mIsAmpPolluter__11TSmallEnemy:
	.incbin "baserom.dol", 0x3E3931, 0x7
.global idx$3335
idx$3335:
	.incbin "baserom.dol", 0x3E3938, 0x8
.global mReflectSw__13TElecNokonoko
mReflectSw__13TElecNokonoko:
	.incbin "baserom.dol", 0x3E3940, 0x8
.global mFlyAngMax__7TTelesa
mFlyAngMax__7TTelesa:
	.incbin "baserom.dol", 0x3E3948, 0x8
.global mTailJntIndex__11TFireWanwan
mTailJntIndex__11TFireWanwan:
	.incbin "baserom.dol", 0x3E3950, 0x8
.global $$23353
$$23353:
	.incbin "baserom.dol", 0x3E3958, 0x4
.global $$23355
$$23355:
	.incbin "baserom.dol", 0x3E395C, 0x4
.global mLandAngle__9TTobiPuku
mLandAngle__9TTobiPuku:
	.incbin "baserom.dol", 0x3E3960, 0x4
.global mBoundSw__9TTobiPuku
mBoundSw__9TTobiPuku:
	.incbin "baserom.dol", 0x3E3964, 0x4
.global mBoundVelocityY__9TTobiPuku
mBoundVelocityY__9TTobiPuku:
	.incbin "baserom.dol", 0x3E3968, 0x4
.global mReturnLaunchSw__9TTobiPuku
mReturnLaunchSw__9TTobiPuku:
	.incbin "baserom.dol", 0x3E396C, 0x4
.global $$23000
$$23000:
	.incbin "baserom.dol", 0x3E3970, 0x4
.global $$23001
$$23001:
	.incbin "baserom.dol", 0x3E3974, 0x4
.global $$23002
$$23002:
	.incbin "baserom.dol", 0x3E3978, 0x4
.global $$23003
$$23003:
	.incbin "baserom.dol", 0x3E397C, 0x4
.global $$23004
$$23004:
	.incbin "baserom.dol", 0x3E3980, 0x4
.global $$23005
$$23005:
	.incbin "baserom.dol", 0x3E3984, 0x4
.global $$23006
$$23006:
	.incbin "baserom.dol", 0x3E3988, 0x4
.global $$23007
$$23007:
	.incbin "baserom.dol", 0x3E398C, 0x4
.global $$23008
$$23008:
	.incbin "baserom.dol", 0x3E3990, 0x4
.global $$23009
$$23009:
	.incbin "baserom.dol", 0x3E3994, 0x4
.global $$23010
$$23010:
	.incbin "baserom.dol", 0x3E3998, 0x4
.global $$23011
$$23011:
	.incbin "baserom.dol", 0x3E399C, 0x4
.global $$23012
$$23012:
	.incbin "baserom.dol", 0x3E39A0, 0x4
.global loopIndirectFilenames$3531
loopIndirectFilenames$3531:
	.incbin "baserom.dol", 0x3E39A4, 0x4
.global mCheckOnPanelTimeRoll__9TChuuHana
mCheckOnPanelTimeRoll__9TChuuHana:
	.incbin "baserom.dol", 0x3E39A8, 0x4
.global mCheckOnPanelTime__9TChuuHana
mCheckOnPanelTime__9TChuuHana:
	.incbin "baserom.dol", 0x3E39AC, 0x4
.global mBodyJntIndex__9TChuuHana
mBodyJntIndex__9TChuuHana:
	.incbin "baserom.dol", 0x3E39B0, 0x1
.global mEyeJntIndex__9TChuuHana
mEyeJntIndex__9TChuuHana:
	.incbin "baserom.dol", 0x3E39B1, 0x1
.global mFootJntIndex__9TChuuHana
mFootJntIndex__9TChuuHana:
	.incbin "baserom.dol", 0x3E39B2, 0x1
.global mNewSw__9TChuuHana
mNewSw__9TChuuHana:
	.incbin "baserom.dol", 0x3E39B3, 0x1
.global mCompareHeight__9TChuuHana
mCompareHeight__9TChuuHana:
	.incbin "baserom.dol", 0x3E39B4, 0x4
.global mSmallMirrorR__9TChuuHana
mSmallMirrorR__9TChuuHana:
	.incbin "baserom.dol", 0x3E39B8, 0x4
.global mMediumMirrorR__9TChuuHana
mMediumMirrorR__9TChuuHana:
	.incbin "baserom.dol", 0x3E39BC, 0x4
.global mLargeMirrorR__9TChuuHana
mLargeMirrorR__9TChuuHana:
	.incbin "baserom.dol", 0x3E39C0, 0x4
.global mAttackVersion__9TChuuHana
mAttackVersion__9TChuuHana:
	.incbin "baserom.dol", 0x3E39C4, 0x1
.global mDamageSw__9TChuuHana
mDamageSw__9TChuuHana:
	.incbin "baserom.dol", 0x3E39C5, 0x3
.global mBoundVal__10TRollEnemy
mBoundVal__10TRollEnemy:
	.incbin "baserom.dol", 0x3E39C8, 0x4
.global mReachNodeDist__7TIgaiga
mReachNodeDist__7TIgaiga:
	.incbin "baserom.dol", 0x3E39CC, 0x4
.global anmlist$3317
anmlist$3317:
	.incbin "baserom.dol", 0x3E39D0, 0x8
.global graphlist$3323
graphlist$3323:
	.incbin "baserom.dol", 0x3E39D8, 0x8
.global mMouthJntIndex__8TPoiHana
mMouthJntIndex__8TPoiHana:
	.incbin "baserom.dol", 0x3E39E0, 0x1
.global mSleepVersion__8TPoiHana
mSleepVersion__8TPoiHana:
	.incbin "baserom.dol", 0x3E39E1, 0x7
.global mStopOnAirTimeMax__9TTamaNoko
mStopOnAirTimeMax__9TTamaNoko:
	.incbin "baserom.dol", 0x3E39E8, 0x4
.global mRollOnAirTimeMax__9TTamaNoko
mRollOnAirTimeMax__9TTamaNoko:
	.incbin "baserom.dol", 0x3E39EC, 0x4
.global mRollOnAirAngle__9TTamaNoko
mRollOnAirAngle__9TTamaNoko:
	.incbin "baserom.dol", 0x3E39F0, 0x4
.global mDropVelocityY__9TTamaNoko
mDropVelocityY__9TTamaNoko:
	.incbin "baserom.dol", 0x3E39F4, 0x4
.global mEnemyGenRate__11TBossTelesa
mEnemyGenRate__11TBossTelesa:
	.incbin "baserom.dol", 0x3E39F8, 0x4
.global mItemGenRate__11TBossTelesa
mItemGenRate__11TBossTelesa:
	.incbin "baserom.dol", 0x3E39FC, 0x4
.global mNormalAlpha__11TBossTelesa
mNormalAlpha__11TBossTelesa:
	.incbin "baserom.dol", 0x3E3A00, 0x4
.global mBaseHoseiPosY__11TBossTelesa
mBaseHoseiPosY__11TBossTelesa:
	.incbin "baserom.dol", 0x3E3A04, 0x4
.global mRouletteUpRate__11TBossTelesa
mRouletteUpRate__11TBossTelesa:
	.incbin "baserom.dol", 0x3E3A08, 0x4
.global mTelesaGenerateInterval__11TBossTelesa
mTelesaGenerateInterval__11TBossTelesa:
	.incbin "baserom.dol", 0x3E3A0C, 0x4
.global mCameraMoveLimit__11TBossTelesa
mCameraMoveLimit__11TBossTelesa:
	.incbin "baserom.dol", 0x3E3A10, 0x4
.global mCameraMoveSp__11TBossTelesa
mCameraMoveSp__11TBossTelesa:
	.incbin "baserom.dol", 0x3E3A14, 0x4
.global mSerialBomb__8TBombHei
mSerialBomb__8TBombHei:
	.incbin "baserom.dol", 0x3E3A18, 0x8
.global mChorobeiJntIdx__7TCannon
mChorobeiJntIdx__7TCannon:
	.incbin "baserom.dol", 0x3E3A20, 0x1
.global mChorobeiHandJntIdx__7TCannon
mChorobeiHandJntIdx__7TCannon:
	.incbin "baserom.dol", 0x3E3A21, 0x3
.global mVelocityRate__7TCannon
mVelocityRate__7TCannon:
	.incbin "baserom.dol", 0x3E3A24, 0x4
.global mSearchRate__7TCannon
mSearchRate__7TCannon:
	.incbin "baserom.dol", 0x3E3A28, 0x8
.global mOpenRollSpeed__8TBossEel
mOpenRollSpeed__8TBossEel:
	.incbin "baserom.dol", 0x3E3A30, 0x4
.global mUseObjCollision__8TBossEel
mUseObjCollision__8TBossEel:
	.incbin "baserom.dol", 0x3E3A34, 0x4
.global mForcePow__8TBossEel
mForcePow__8TBossEel:
	.incbin "baserom.dol", 0x3E3A38, 0x8
.global mSerialBomb__7TKiller
mSerialBomb__7TKiller:
	.incbin "baserom.dol", 0x3E3A40, 0x1
.global mTrampleDie__7TKiller
mTrampleDie__7TKiller:
	.incbin "baserom.dol", 0x3E3A41, 0x3
.global mTestSp__9TFlyEnemy
mTestSp__9TFlyEnemy:
	.incbin "baserom.dol", 0x3E3A44, 0x4
.global mInvalidTime__9TFlyEnemy
mInvalidTime__9TFlyEnemy:
	.incbin "baserom.dol", 0x3E3A48, 0x4
.global mTestMarioSpMax__9TFlyEnemy
mTestMarioSpMax__9TFlyEnemy:
	.incbin "baserom.dol", 0x3E3A4C, 0x4
.global mHeadJntIndex__10THanaSambo
mHeadJntIndex__10THanaSambo:
	.incbin "baserom.dol", 0x3E3A50, 0x1
.global mPollenJntIndex__10THanaSambo
mPollenJntIndex__10THanaSambo:
	.incbin "baserom.dol", 0x3E3A51, 0x7
.global mRollSw__5TPopo
mRollSw__5TPopo:
	.incbin "baserom.dol", 0x3E3A58, 0x1
.global mTriggerSw__5TPopo
mTriggerSw__5TPopo:
	.incbin "baserom.dol", 0x3E3A59, 0x3
.global mTestAng_x__5TPopo
mTestAng_x__5TPopo:
	.incbin "baserom.dol", 0x3E3A5C, 0x4
.global mTestAng_y__5TPopo
mTestAng_y__5TPopo:
	.incbin "baserom.dol", 0x3E3A60, 0x4
.global mNozzleOffsetZ__5TPopo
mNozzleOffsetZ__5TPopo:
	.incbin "baserom.dol", 0x3E3A64, 0x4
.global mCenterJntIndex__5TPopo
mCenterJntIndex__5TPopo:
	.incbin "baserom.dol", 0x3E3A68, 0x1
.global mMouthJntIndex__5TPopo
mMouthJntIndex__5TPopo:
	.incbin "baserom.dol", 0x3E3A69, 0x1
.global mRLegJntIndex__5TPopo
mRLegJntIndex__5TPopo:
	.incbin "baserom.dol", 0x3E3A6A, 0x1
.global mLLegJntIndex__5TPopo
mLLegJntIndex__5TPopo:
	.incbin "baserom.dol", 0x3E3A6B, 0x1
.global mRHandJntIndex__5TPopo
mRHandJntIndex__5TPopo:
	.incbin "baserom.dol", 0x3E3A6C, 0x1
.global mLHandJntIndex__5TPopo
mLHandJntIndex__5TPopo:
	.incbin "baserom.dol", 0x3E3A6D, 0x3
.global mTestBodyScale__5TPopo
mTestBodyScale__5TPopo:
	.incbin "baserom.dol", 0x3E3A70, 0x4
.global mBrkFlag__5TPopo
mBrkFlag__5TPopo:
	.incbin "baserom.dol", 0x3E3A74, 0x4
.global mColOffsetY__5TPopo
mColOffsetY__5TPopo:
	.incbin "baserom.dol", 0x3E3A78, 0x4
.global mColMinVal__5TPopo
mColMinVal__5TPopo:
	.incbin "baserom.dol", 0x3E3A7C, 0x4
.global mLevelShootSw__5TPopo
mLevelShootSw__5TPopo:
	.incbin "baserom.dol", 0x3E3A80, 0x8
.global sleepBossHanachan_bastable
sleepBossHanachan_bastable:
	.incbin "baserom.dol", 0x3E3A88, 0x8
.global $$22707
$$22707:
	.incbin "baserom.dol", 0x3E3A90, 0x4
.global $$22709
$$22709:
	.incbin "baserom.dol", 0x3E3A94, 0x4
.global cSandPillarModelName
cSandPillarModelName:
	.incbin "baserom.dol", 0x3E3A98, 0x4
.global cHitPoint1_RailName
cHitPoint1_RailName:
	.incbin "baserom.dol", 0x3E3A9C, 0x4
.global cHitPoint2_RailName
cHitPoint2_RailName:
	.incbin "baserom.dol", 0x3E3AA0, 0x4
.global cSandTextureName
cSandTextureName:
	.incbin "baserom.dol", 0x3E3AA4, 0x4
.global cDummyTextureName
cDummyTextureName:
	.incbin "baserom.dol", 0x3E3AA8, 0x8
.global cMapCollisionJointName
cMapCollisionJointName:
	.incbin "baserom.dol", 0x3E3AB0, 0x4
.global cBodyMapCollisionFileName
cBodyMapCollisionFileName:
	.incbin "baserom.dol", 0x3E3AB4, 0x4
.global cHeadMapCollisionFileName
cHeadMapCollisionFileName:
	.incbin "baserom.dol", 0x3E3AB8, 0x4
.global cLegJointName_L3
cLegJointName_L3:
	.incbin "baserom.dol", 0x3E3ABC, 0x4
.global cLegJointName_R3
cLegJointName_R3:
	.incbin "baserom.dol", 0x3E3AC0, 0x4
.global cNoseHallJointName_L
cNoseHallJointName_L:
	.incbin "baserom.dol", 0x3E3AC4, 0x4
.global cNoseHallJointName_R
cNoseHallJointName_R:
	.incbin "baserom.dol", 0x3E3AC8, 0x4
.global sFootJointName$2739
sFootJointName$2739:
	.incbin "baserom.dol", 0x3E3ACC, 0xC
.global $$23012
$$23012:
	.incbin "baserom.dol", 0x3E3AD8, 0x4
.global $$23014
$$23014:
	.incbin "baserom.dol", 0x3E3ADC, 0x4
.global mTestAng_y__7TRocket
mTestAng_y__7TRocket:
	.incbin "baserom.dol", 0x3E3AE0, 0x4
.global mNozzleOffsetZ__7TRocket
mNozzleOffsetZ__7TRocket:
	.incbin "baserom.dol", 0x3E3AE4, 0x4
.global mColOffsetY__7TRocket
mColOffsetY__7TRocket:
	.incbin "baserom.dol", 0x3E3AE8, 0x8
.global $$22833
$$22833:
	.incbin "baserom.dol", 0x3E3AF0, 0x4
.global $$22834
$$22834:
	.incbin "baserom.dol", 0x3E3AF4, 0x4
.global $$22835
$$22835:
	.incbin "baserom.dol", 0x3E3AF8, 0x4
.global $$22836
$$22836:
	.incbin "baserom.dol", 0x3E3AFC, 0x4
.global $$22867
$$22867:
	.incbin "baserom.dol", 0x3E3B00, 0x4
.global $$22868
$$22868:
	.incbin "baserom.dol", 0x3E3B04, 0x4
.global $$22869
$$22869:
	.incbin "baserom.dol", 0x3E3B08, 0x4
.global $$22870
$$22870:
	.incbin "baserom.dol", 0x3E3B0C, 0x4
.global koopajrsubmarine_bastable
koopajrsubmarine_bastable:
	.incbin "baserom.dol", 0x3E3B10, 0x4
.global $$22995
$$22995:
	.incbin "baserom.dol", 0x3E3B14, 0x4
.global $$22996
$$22996:
	.incbin "baserom.dol", 0x3E3B18, 0x4
.global $$22997
$$22997:
	.incbin "baserom.dol", 0x3E3B1C, 0x4
.global $$22998
$$22998:
	.incbin "baserom.dol", 0x3E3B20, 0x4
.global $$22999
$$22999:
	.incbin "baserom.dol", 0x3E3B24, 0x4
.global $$23000
$$23000:
	.incbin "baserom.dol", 0x3E3B28, 0x4
.global $$23001
$$23001:
	.incbin "baserom.dol", 0x3E3B2C, 0x4
.global $$23002
$$23002:
	.incbin "baserom.dol", 0x3E3B30, 0x4
.global $$23003
$$23003:
	.incbin "baserom.dol", 0x3E3B34, 0x4
.global $$23004
$$23004:
	.incbin "baserom.dol", 0x3E3B38, 0x4
.global $$23005
$$23005:
	.incbin "baserom.dol", 0x3E3B3C, 0x4
.global $$23006
$$23006:
	.incbin "baserom.dol", 0x3E3B40, 0x4
.global $$23016
$$23016:
	.incbin "baserom.dol", 0x3E3B44, 0x4
.global $$23017
$$23017:
	.incbin "baserom.dol", 0x3E3B48, 0x4
.global $$23018
$$23018:
	.incbin "baserom.dol", 0x3E3B4C, 0x4
.global $$23019
$$23019:
	.incbin "baserom.dol", 0x3E3B50, 0x4
.global $$23020
$$23020:
	.incbin "baserom.dol", 0x3E3B54, 0x4
.global $$23021
$$23021:
	.incbin "baserom.dol", 0x3E3B58, 0x4
.global $$23022
$$23022:
	.incbin "baserom.dol", 0x3E3B5C, 0x4
.global $$23023
$$23023:
	.incbin "baserom.dol", 0x3E3B60, 0x4
.global $$23024
$$23024:
	.incbin "baserom.dol", 0x3E3B64, 0x4
.global $$23025
$$23025:
	.incbin "baserom.dol", 0x3E3B68, 0x4
.global $$23026
$$23026:
	.incbin "baserom.dol", 0x3E3B6C, 0x4
.global $$23027
$$23027:
	.incbin "baserom.dol", 0x3E3B70, 0x4
.global onetimeFilenames$3171
onetimeFilenames$3171:
	.incbin "baserom.dol", 0x3E3B74, 0x4
.global $$22655
$$22655:
	.incbin "baserom.dol", 0x3E3B78, 0x4
.global $$22656
$$22656:
	.incbin "baserom.dol", 0x3E3B7C, 0x4
.global $$22657
$$22657:
	.incbin "baserom.dol", 0x3E3B80, 0x4
.global $$22658
$$22658:
	.incbin "baserom.dol", 0x3E3B84, 0x4
.global $$23092
$$23092:
	.incbin "baserom.dol", 0x3E3B88, 0x4
.global $$23093
$$23093:
	.incbin "baserom.dol", 0x3E3B8C, 0x4
.global $$23094
$$23094:
	.incbin "baserom.dol", 0x3E3B90, 0x4
.global $$23095
$$23095:
	.incbin "baserom.dol", 0x3E3B94, 0x4
.global $$23096
$$23096:
	.incbin "baserom.dol", 0x3E3B98, 0x8
.global $$22521
$$22521:
	.incbin "baserom.dol", 0x3E3BA0, 0x4
.global $$22522
$$22522:
	.incbin "baserom.dol", 0x3E3BA4, 0x4
.global $$22523
$$22523:
	.incbin "baserom.dol", 0x3E3BA8, 0x4
.global $$22524
$$22524:
	.incbin "baserom.dol", 0x3E3BAC, 0x4
.global $$22525
$$22525:
	.incbin "baserom.dol", 0x3E3BB0, 0x8
.global $$22921
$$22921:
	.incbin "baserom.dol", 0x3E3BB8, 0x4
.global $$22922
$$22922:
	.incbin "baserom.dol", 0x3E3BBC, 0x4
.global $$22923
$$22923:
	.incbin "baserom.dol", 0x3E3BC0, 0x4
.global $$22924
$$22924:
	.incbin "baserom.dol", 0x3E3BC4, 0x4
.global $$22925
$$22925:
	.incbin "baserom.dol", 0x3E3BC8, 0x4
.global $$22926
$$22926:
	.incbin "baserom.dol", 0x3E3BCC, 0x4
.global $$22927
$$22927:
	.incbin "baserom.dol", 0x3E3BD0, 0x4
.global $$22928
$$22928:
	.incbin "baserom.dol", 0x3E3BD4, 0x4
.global $$22929
$$22929:
	.incbin "baserom.dol", 0x3E3BD8, 0x4
.global $$22930
$$22930:
	.incbin "baserom.dol", 0x3E3BDC, 0x4
.global $$22931
$$22931:
	.incbin "baserom.dol", 0x3E3BE0, 0x4
.global $$22932
$$22932:
	.incbin "baserom.dol", 0x3E3BE4, 0x4
.global $$22933
$$22933:
	.incbin "baserom.dol", 0x3E3BE8, 0x4
.global $$22934
$$22934:
	.incbin "baserom.dol", 0x3E3BEC, 0x4
.global $$22935
$$22935:
	.incbin "baserom.dol", 0x3E3BF0, 0x4
.global $$22936
$$22936:
	.incbin "baserom.dol", 0x3E3BF4, 0x4
.global $$22937
$$22937:
	.incbin "baserom.dol", 0x3E3BF8, 0x4
.global $$22938
$$22938:
	.incbin "baserom.dol", 0x3E3BFC, 0x4
.global $$22939
$$22939:
	.incbin "baserom.dol", 0x3E3C00, 0x4
.global $$22940
$$22940:
	.incbin "baserom.dol", 0x3E3C04, 0x4
.global $$22941
$$22941:
	.incbin "baserom.dol", 0x3E3C08, 0x4
.global $$22942
$$22942:
	.incbin "baserom.dol", 0x3E3C0C, 0x4
.global $$22943
$$22943:
	.incbin "baserom.dol", 0x3E3C10, 0x4
.global $$22944
$$22944:
	.incbin "baserom.dol", 0x3E3C14, 0x4
.global $$22945
$$22945:
	.incbin "baserom.dol", 0x3E3C18, 0x4
.global $$22946
$$22946:
	.incbin "baserom.dol", 0x3E3C1C, 0x4
.global $$22947
$$22947:
	.incbin "baserom.dol", 0x3E3C20, 0x4
.global $$22948
$$22948:
	.incbin "baserom.dol", 0x3E3C24, 0x4
.global $$22949
$$22949:
	.incbin "baserom.dol", 0x3E3C28, 0x4
.global $$22950
$$22950:
	.incbin "baserom.dol", 0x3E3C2C, 0x4
.global $$22951
$$22951:
	.incbin "baserom.dol", 0x3E3C30, 0x4
.global $$22952
$$22952:
	.incbin "baserom.dol", 0x3E3C34, 0x4
.global $$22953
$$22953:
	.incbin "baserom.dol", 0x3E3C38, 0x4
.global $$22954
$$22954:
	.incbin "baserom.dol", 0x3E3C3C, 0x4
.global $$22955
$$22955:
	.incbin "baserom.dol", 0x3E3C40, 0x4
.global $$22956
$$22956:
	.incbin "baserom.dol", 0x3E3C44, 0x4
.global loopFilenames$3201
loopFilenames$3201:
	.incbin "baserom.dol", 0x3E3C48, 0x8
.global $$22842
$$22842:
	.incbin "baserom.dol", 0x3E3C50, 0x4
.global $$22843
$$22843:
	.incbin "baserom.dol", 0x3E3C54, 0x4
.global DebuTelesa_bastable
DebuTelesa_bastable:
	.incbin "baserom.dol", 0x3E3C58, 0x4
.global $$22830
$$22830:
	.incbin "baserom.dol", 0x3E3C5C, 0x4
.global $$22831
$$22831:
	.incbin "baserom.dol", 0x3E3C60, 0x4
.global $$22832
$$22832:
	.incbin "baserom.dol", 0x3E3C64, 0x4
.global $$22833
$$22833:
	.incbin "baserom.dol", 0x3E3C68, 0x8
.global $$22940
$$22940:
	.incbin "baserom.dol", 0x3E3C70, 0x4
.global $$22941
$$22941:
	.incbin "baserom.dol", 0x3E3C74, 0x4
.global $$22942
$$22942:
	.incbin "baserom.dol", 0x3E3C78, 0x4
.global $$22943
$$22943:
	.incbin "baserom.dol", 0x3E3C7C, 0x4
.global $$22946
$$22946:
	.incbin "baserom.dol", 0x3E3C80, 0x4
.global $$22947
$$22947:
	.incbin "baserom.dol", 0x3E3C84, 0x4
.global $$22948
$$22948:
	.incbin "baserom.dol", 0x3E3C88, 0x4
.global $$22949
$$22949:
	.incbin "baserom.dol", 0x3E3C8C, 0x4
.global $$22950
$$22950:
	.incbin "baserom.dol", 0x3E3C90, 0x4
.global $$22951
$$22951:
	.incbin "baserom.dol", 0x3E3C94, 0x4
.global $$22952
$$22952:
	.incbin "baserom.dol", 0x3E3C98, 0x4
.global $$22953
$$22953:
	.incbin "baserom.dol", 0x3E3C9C, 0x4
.global $$22954
$$22954:
	.incbin "baserom.dol", 0x3E3CA0, 0x4
.global $$22955
$$22955:
	.incbin "baserom.dol", 0x3E3CA4, 0x4
.global $$22956
$$22956:
	.incbin "baserom.dol", 0x3E3CA8, 0x8
.global cStartCamName
cStartCamName:
	.incbin "baserom.dol", 0x3E3CB0, 0x4
.global cStartAfterCamName
cStartAfterCamName:
	.incbin "baserom.dol", 0x3E3CB4, 0x4
.global cJetCoasterCam0BckName
cJetCoasterCam0BckName:
	.incbin "baserom.dol", 0x3E3CB8, 0x4
.global cJetCoasterCam1BckName
cJetCoasterCam1BckName:
	.incbin "baserom.dol", 0x3E3CBC, 0x4
.global cJetCoasterDemoBckName
cJetCoasterDemoBckName:
	.incbin "baserom.dol", 0x3E3CC0, 0x4
.global cStartCamBckFileName
cStartCamBckFileName:
	.incbin "baserom.dol", 0x3E3CC4, 0x4
.global bossGesoViewObjName
bossGesoViewObjName:
	.incbin "baserom.dol", 0x3E3CC8, 0x8
.global cSunWarpPointName
cSunWarpPointName:
	.incbin "baserom.dol", 0x3E3CD0, 0x8
.global cPositionJointName
cPositionJointName:
	.incbin "baserom.dol", 0x3E3CD8, 0x4
.global cLookatJointName
cLookatJointName:
	.incbin "baserom.dol", 0x3E3CDC, 0x4
.global cCameraBckVolumeName
cCameraBckVolumeName:
	.incbin "baserom.dol", 0x3E3CE0, 0x8
.global cLoadCamName
cLoadCamName:
	.incbin "baserom.dol", 0x3E3CE8, 0x18

.include "macros.inc"

.section .sbss, "wa"  # 0x803E6000 - 0x8040B45C
.global sCurrentDirID__10JKRArchive
sCurrentDirID__10JKRArchive:
	.skip 0x8
.global sAramObject__7JKRAram
sAramObject__7JKRAram:
	.skip 0x4
.global szpBuf
szpBuf:
	.skip 0x4
.global szpEnd
szpEnd:
	.skip 0x4
.global refBuf
refBuf:
	.skip 0x4
.global refEnd
refEnd:
	.skip 0x4
.global refCurrent
refCurrent:
	.skip 0x4
.global srcOffset
srcOffset:
	.skip 0x4
.global transLeft
transLeft:
	.skip 0x4
.global srcLimit
srcLimit:
	.skip 0x4
.global srcAddress
srcAddress:
	.skip 0x4
.global fileOffset
fileOffset:
	.skip 0x4
.global readCount
readCount:
	.skip 0x4
.global maxDest
maxDest:
	.skip 0x8
.global whatdo
whatdo:
	.skip 0x4
.global whatdo2
whatdo2:
	.skip 0x4
.global DBfoundSize
DBfoundSize:
	.skip 0x4
.global DBfoundOffset
DBfoundOffset:
	.skip 0x4
.global DBfoundBlock
DBfoundBlock:
	.skip 0x4
.global DBnewFreeBlock
DBnewFreeBlock:
	.skip 0x4
.global DBnewUsedBlock
DBnewUsedBlock:
	.skip 0x8
.global sCurrentVolume__13JKRFileLoader
sCurrentVolume__13JKRFileLoader:
	.skip 0x8
.global sSystemHeap__7JKRHeap
sSystemHeap__7JKRHeap:
	.skip 0x4
.global sCurrentHeap__7JKRHeap
sCurrentHeap__7JKRHeap:
	.skip 0x4
.global sRootHeap__7JKRHeap
sRootHeap__7JKRHeap:
	.skip 0x4
.global mErrorHandler__7JKRHeap
mErrorHandler__7JKRHeap:
	.skip 0x4
.global mCodeStart__7JKRHeap
mCodeStart__7JKRHeap:
	.skip 0x4
.global mCodeEnd__7JKRHeap
mCodeEnd__7JKRHeap:
	.skip 0x4
.global mUserRamStart__7JKRHeap
mUserRamStart__7JKRHeap:
	.skip 0x4
.global mUserRamEnd__7JKRHeap
mUserRamEnd__7JKRHeap:
	.skip 0x4
.global mMemorySize__7JKRHeap
mMemorySize__7JKRHeap:
	.skip 0x8
.global sErrorManager__12JUTException
sErrorManager__12JUTException:
	.skip 0x4
.global sPreUserCallback__12JUTException
sPreUserCallback__12JUTException:
	.skip 0x4
.global sPostUserCallback__12JUTException
sPostUserCallback__12JUTException:
	.skip 0x4
.global sConsoleBuffer__12JUTException
sConsoleBuffer__12JUTException:
	.skip 0x4
.global sConsoleBufferSize__12JUTException
sConsoleBufferSize__12JUTException:
	.skip 0x4
.global sConsole__12JUTException
sConsole__12JUTException:
	.skip 0x4
.global msr__12JUTException
msr__12JUTException:
	.skip 0x4
.global fpscr__12JUTException
fpscr__12JUTException:
	.skip 0x4
.global sDirectPrint__14JUTDirectPrint
sDirectPrint__14JUTDirectPrint:
	.skip 0x8
.global mListInitialized__10JUTGamePad
mListInitialized__10JUTGamePad:
	.skip 0x4
.global mPadAssign__10JUTGamePad
mPadAssign__10JUTGamePad:
	.skip 0x4
.global mSuppressPadReset__10JUTGamePad
mSuppressPadReset__10JUTGamePad:
	.skip 0x4
.global sAnalogMode__10JUTGamePad
sAnalogMode__10JUTGamePad:
	.skip 0x4
.global mStatus__Q210JUTGamePad7CRumble
mStatus__Q210JUTGamePad7CRumble:
	.skip 0x4
.global mEnable__Q210JUTGamePad7CRumble
mEnable__Q210JUTGamePad7CRumble:
	.skip 0x4
.global sCallback__Q210JUTGamePad13C3ButtonReset
sCallback__Q210JUTGamePad13C3ButtonReset:
	.skip 0x4
.global sCallbackArg__Q210JUTGamePad13C3ButtonReset
sCallbackArg__Q210JUTGamePad13C3ButtonReset:
	.skip 0x4
.global sThreshold__Q210JUTGamePad13C3ButtonReset
sThreshold__Q210JUTGamePad13C3ButtonReset:
	.skip 0x4
.global lbl_8040989C
lbl_8040989C:
	.skip 0x4
.global sResetSwitchPushing__Q210JUTGamePad13C3ButtonReset
sResetSwitchPushing__Q210JUTGamePad13C3ButtonReset:
	.skip 0x1
.global sResetOccurred__Q210JUTGamePad13C3ButtonReset
sResetOccurred__Q210JUTGamePad13C3ButtonReset:
	.skip 0x3
.global sResetOccurredPort__Q210JUTGamePad13C3ButtonReset
sResetOccurredPort__Q210JUTGamePad13C3ButtonReset:
	.skip 0x4
.global jmaSinTableSize
jmaSinTableSize:
	.skip 0x4
.global jmaSinShift
jmaSinShift:
	.skip 0x4
.global jmaSinTable
jmaSinTable:
	.skip 0x4
.global jmaCosTable
jmaCosTable:
	.skip 0x4
.global mStrBuff__8J2DPrint
mStrBuff__8J2DPrint:
	.skip 0x4
.global mMutexInit__8J2DPrint
mMutexInit__8J2DPrint:
	.skip 0x1
.global mHeapFlag__8J2DPrint
mHeapFlag__8J2DPrint:
	.skip 0x3
.global mStrBuffSize__8J2DPrint
mStrBuffSize__8J2DPrint:
	.skip 0x4
.global mBufferNotEnough__8J2DPrint
mBufferNotEnough__8J2DPrint:
	.skip 0x4
.global currentPipeline__11J3DShapeMtx
currentPipeline__11J3DShapeMtx:
	.skip 0x8
.global j3dDefaultViewNo
j3dDefaultViewNo:
	.skip 0x8
.global szpBuf
szpBuf:
	.skip 0x4
.global szpEnd
szpEnd:
	.skip 0x4
.global refBuf
refBuf:
	.skip 0x4
.global refEnd
refEnd:
	.skip 0x4
.global refCurrent
refCurrent:
	.skip 0x4
.global srcOffset
srcOffset:
	.skip 0x4
.global transLeft
transLeft:
	.skip 0x4
.global srcLimit
srcLimit:
	.skip 0x4
.global srcFile
srcFile:
	.skip 0x4
.global fileOffset
fileOffset:
	.skip 0x4
.global readCount
readCount:
	.skip 0x4
.global maxDest
maxDest:
	.skip 0x4
.global szpBuf
szpBuf:
	.skip 0x4
.global szpEnd
szpEnd:
	.skip 0x4
.global refBuf
refBuf:
	.skip 0x4
.global refEnd
refEnd:
	.skip 0x4
.global refCurrent
refCurrent:
	.skip 0x4
.global dmaBuf
dmaBuf:
	.skip 0x4
.global dmaEnd
dmaEnd:
	.skip 0x4
.global dmaCurrent
dmaCurrent:
	.skip 0x4
.global srcOffset
srcOffset:
	.skip 0x4
.global transLeft
transLeft:
	.skip 0x4
.global srcLimit
srcLimit:
	.skip 0x4
.global srcFile
srcFile:
	.skip 0x4
.global fileOffset
fileOffset:
	.skip 0x4
.global readCount
readCount:
	.skip 0x4
.global maxDest
maxDest:
	.skip 0x8
.global sDecompObject__9JKRDecomp
sDecompObject__9JKRDecomp:
	.skip 0x8
.global sInterruptFlag__13J3DDrawPacket
sInterruptFlag__13J3DDrawPacket:
	.skip 0x8
.global entryNum__13J3DDrawBuffer
entryNum__13J3DDrawBuffer:
	.skip 0x8
.global sAramStreamObject__13JKRAramStream
sAramStreamObject__13JKRAramStream:
	.skip 0x4
.global transBuffer__13JKRAramStream
transBuffer__13JKRAramStream:
	.skip 0x4
.global transSize__13JKRAramStream
transSize__13JKRAramStream:
	.skip 0x4
.global transHeap__13JKRAramStream
transHeap__13JKRAramStream:
	.skip 0x4
.global sMessageLife__Q212JUTAssertion9$$2unnamed$$2
sMessageLife__Q212JUTAssertion9$$2unnamed$$2:
	.skip 0x8
.global sManager__8JUTVideo
sManager__8JUTVideo:
	.skip 0x8
.global spAboutEncoding___10JUTRomFont
spAboutEncoding___10JUTRomFont:
	.skip 0x4
.global spFontHeader___10JUTRomFont
spFontHeader___10JUTRomFont:
	.skip 0x4
.global suFontHeaderRefered___10JUTRomFont
suFontHeaderRefered___10JUTRomFont:
	.skip 0x8
.global sManager__17JUTConsoleManager
sManager__17JUTConsoleManager:
	.skip 0x4
.global sReportConsole
sReportConsole:
	.skip 0x4
.global sWarningConsole
sWarningConsole:
	.skip 0x8
.global instance__Q26JDrama11TNameRefGen
instance__Q26JDrama11TNameRefGen:
	.skip 0x8
.global smCurrentHeap__10JADHioNode
smCurrentHeap__10JADHioNode:
	.skip 0x8
.global init$357
init$357:
	.skip 0x4
.global oRandom$356
oRandom$356:
	.skip 0x4
.global spFManager__9JALSystem
spFManager__9JALSystem:
	.skip 0x4
.global smList__26JALList$$015JALSeModEffDGrp$$1
smList__26JALList$$015JALSeModEffDGrp$$1:
	.skip 0xC
.global smList__26JALList$$015JALSeModPitDGrp$$1
smList__26JALList$$015JALSeModPitDGrp$$1:
	.skip 0xC
.global smList__26JALList$$015JALSeModVolDGrp$$1
smList__26JALList$$015JALSeModVolDGrp$$1:
	.skip 0xC
.global smList__26JALList$$015JALSeModEffFGrp$$1
smList__26JALList$$015JALSeModEffFGrp$$1:
	.skip 0xC
.global smList__26JALList$$015JALSeModPitFGrp$$1
smList__26JALList$$015JALSeModPitFGrp$$1:
	.skip 0xC
.global smList__26JALList$$015JALSeModVolFGrp$$1
smList__26JALList$$015JALSeModVolFGrp$$1:
	.skip 0xC
.global smList__26JALList$$015JALSeModEffDist$$1
smList__26JALList$$015JALSeModEffDist$$1:
	.skip 0xC
.global smList__26JALList$$015JALSeModPitDist$$1
smList__26JALList$$015JALSeModPitDist$$1:
	.skip 0xC
.global smList__26JALList$$015JALSeModVolDist$$1
smList__26JALList$$015JALSeModVolDist$$1:
	.skip 0xC
.global smList__26JALList$$015JALSeModEffFunk$$1
smList__26JALList$$015JALSeModEffFunk$$1:
	.skip 0xC
.global smList__26JALList$$015JALSeModPitFunk$$1
smList__26JALList$$015JALSeModPitFunk$$1:
	.skip 0xC
.global smList__26JALList$$015JALSeModVolFunk$$1
smList__26JALList$$015JALSeModVolFunk$$1:
	.skip 0xC
.global __init__smList__26JALList$$015JALSeModEffDGrp$$1
__init__smList__26JALList$$015JALSeModEffDGrp$$1:
	.skip 0x1
.global __init__smList__26JALList$$015JALSeModPitDGrp$$1
__init__smList__26JALList$$015JALSeModPitDGrp$$1:
	.skip 0x1
.global __init__smList__26JALList$$015JALSeModVolDGrp$$1
__init__smList__26JALList$$015JALSeModVolDGrp$$1:
	.skip 0x1
.global __init__smList__26JALList$$015JALSeModEffFGrp$$1
__init__smList__26JALList$$015JALSeModEffFGrp$$1:
	.skip 0x1
.global __init__smList__26JALList$$015JALSeModPitFGrp$$1
__init__smList__26JALList$$015JALSeModPitFGrp$$1:
	.skip 0x1
.global __init__smList__26JALList$$015JALSeModVolFGrp$$1
__init__smList__26JALList$$015JALSeModVolFGrp$$1:
	.skip 0x1
.global __init__smList__26JALList$$015JALSeModEffDist$$1
__init__smList__26JALList$$015JALSeModEffDist$$1:
	.skip 0x1
.global __init__smList__26JALList$$015JALSeModPitDist$$1
__init__smList__26JALList$$015JALSeModPitDist$$1:
	.skip 0x1
.global __init__smList__26JALList$$015JALSeModVolDist$$1
__init__smList__26JALList$$015JALSeModVolDist$$1:
	.skip 0x1
.global __init__smList__26JALList$$015JALSeModEffFunk$$1
__init__smList__26JALList$$015JALSeModEffFunk$$1:
	.skip 0x1
.global __init__smList__26JALList$$015JALSeModPitFunk$$1
__init__smList__26JALList$$015JALSeModPitFunk$$1:
	.skip 0x1
.global __init__smList__26JALList$$015JALSeModVolFunk$$1
__init__smList__26JALList$$015JALSeModVolFunk$$1:
	.skip 0x1
.global asnData__Q27JAInter8TAsnData
asnData__Q27JAInter8TAsnData:
	.skip 0x8
.global basic__8JAIBasic
basic__8JAIBasic:
	.skip 0x4
.global _dinfo$1264
_dinfo$1264:
	.skip 0x4
.global _info$1317
_info$1317:
	.skip 0x8
.global random__8JAIConst
random__8JAIConst:
	.skip 0x8
.global currentHeap__Q27JAInter10TDebugHeap
currentHeap__Q27JAInter10TDebugHeap:
	.skip 0x8
.global adpcm_remain__Q27JAInter9StreamLib
adpcm_remain__Q27JAInter9StreamLib:
	.skip 0x4
.global adpcm_loadpoint__Q27JAInter9StreamLib
adpcm_loadpoint__Q27JAInter9StreamLib:
	.skip 0x4
.global loadsize__Q27JAInter9StreamLib
loadsize__Q27JAInter9StreamLib:
	.skip 0x4
.global adpcm_buffer__Q27JAInter9StreamLib
adpcm_buffer__Q27JAInter9StreamLib:
	.skip 0x4
.global loop_buffer__Q27JAInter9StreamLib
loop_buffer__Q27JAInter9StreamLib:
	.skip 0x4
.global store_buffer__Q27JAInter9StreamLib
store_buffer__Q27JAInter9StreamLib:
	.skip 0x4
.global assign_ch__Q27JAInter9StreamLib
assign_ch__Q27JAInter9StreamLib:
	.skip 0x8
.global playside__Q27JAInter9StreamLib
playside__Q27JAInter9StreamLib:
	.skip 0x4
.global playback_samples__Q27JAInter9StreamLib
playback_samples__Q27JAInter9StreamLib:
	.skip 0x4
.global loadup_samples__Q27JAInter9StreamLib
loadup_samples__Q27JAInter9StreamLib:
	.skip 0x4
.global adpcmbuf_state__Q27JAInter9StreamLib
adpcmbuf_state__Q27JAInter9StreamLib:
	.skip 0x4
.global movieframe__Q27JAInter9StreamLib
movieframe__Q27JAInter9StreamLib:
	.skip 0x4
.global stopflag__Q27JAInter9StreamLib
stopflag__Q27JAInter9StreamLib:
	.skip 0x1
.global stopflag2__Q27JAInter9StreamLib
stopflag2__Q27JAInter9StreamLib:
	.skip 0x1
.global playflag__Q27JAInter9StreamLib
playflag__Q27JAInter9StreamLib:
	.skip 0x1
.global playflag2__Q27JAInter9StreamLib
playflag2__Q27JAInter9StreamLib:
	.skip 0x1
.global prepareflag__Q27JAInter9StreamLib
prepareflag__Q27JAInter9StreamLib:
	.skip 0x1
.global dspch_deallockflag__Q27JAInter9StreamLib
dspch_deallockflag__Q27JAInter9StreamLib:
	.skip 0x3
.global outvolume__Q27JAInter9StreamLib
outvolume__Q27JAInter9StreamLib:
	.skip 0x4
.global outpitch__Q27JAInter9StreamLib
outpitch__Q27JAInter9StreamLib:
	.skip 0x4
.global outpan__Q27JAInter9StreamLib
outpan__Q27JAInter9StreamLib:
	.skip 0x4
.global stackvolume__Q27JAInter9StreamLib
stackvolume__Q27JAInter9StreamLib:
	.skip 0x4
.global stackpitch__Q27JAInter9StreamLib
stackpitch__Q27JAInter9StreamLib:
	.skip 0x4
.global stackpan__Q27JAInter9StreamLib
stackpan__Q27JAInter9StreamLib:
	.skip 0x4
.global outflag_volume__Q27JAInter9StreamLib
outflag_volume__Q27JAInter9StreamLib:
	.skip 0x1
.global outflag_pan__Q27JAInter9StreamLib
outflag_pan__Q27JAInter9StreamLib:
	.skip 0x1
.global outflag_pitch__Q27JAInter9StreamLib
outflag_pitch__Q27JAInter9StreamLib:
	.skip 0x2
.global loop_start_flag__Q27JAInter9StreamLib
loop_start_flag__Q27JAInter9StreamLib:
	.skip 0x4
.global outpause__Q27JAInter9StreamLib
outpause__Q27JAInter9StreamLib:
	.skip 0x4
.global playmode__Q27JAInter9StreamLib
playmode__Q27JAInter9StreamLib:
	.skip 0x4
.global shift_sample__Q27JAInter9StreamLib
shift_sample__Q27JAInter9StreamLib:
	.skip 0x4
.global extra_sample__Q27JAInter9StreamLib
extra_sample__Q27JAInter9StreamLib:
	.skip 0x4
.global DvdLoadFlag__Q27JAInter9StreamLib
DvdLoadFlag__Q27JAInter9StreamLib:
	.skip 0x4
.global startInitFlag__Q27JAInter9StreamLib
startInitFlag__Q27JAInter9StreamLib:
	.skip 0x4
.global Mode__Q27JAInter9StreamLib
Mode__Q27JAInter9StreamLib:
	.skip 0x4
.global Head__Q27JAInter9StreamLib
Head__Q27JAInter9StreamLib:
	.skip 0x4
.global bufferMode__Q27JAInter9StreamLib
bufferMode__Q27JAInter9StreamLib:
	.skip 0x1
.global allocFlag__Q27JAInter9StreamLib
allocFlag__Q27JAInter9StreamLib:
	.skip 0x3
.global before$612
before$612:
	.skip 0x4
.global init$613
init$613:
	.skip 0x2
.global L1$629
L1$629:
	.skip 0x2
.global L2$630
L2$630:
	.skip 0x2
.global R1$631
R1$631:
	.skip 0x2
.global R2$632
R2$632:
	.skip 0x4
.global oldstat$823
oldstat$823:
	.skip 0x4
.global init$824
init$824:
	.skip 0x4
.global old_dspside$839
old_dspside$839:
	.skip 0x4
.global init$840
init$840:
	.skip 0x4
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
.global interPointer__8JAISound
interPointer__8JAISound:
	.skip 0x8
.global sCurrentHeap__Q28JASystem5TBank
sCurrentHeap__Q28JASystem5TBank:
	.skip 0x8
.global sTableSize__Q28JASystem7BankMgr
sTableSize__Q28JASystem7BankMgr:
	.skip 0x4
.global sBankArray__Q28JASystem7BankMgr
sBankArray__Q28JASystem7BankMgr:
	.skip 0x4
.global sVir2PhyTable__Q28JASystem7BankMgr
sVir2PhyTable__Q28JASystem7BankMgr:
	.skip 0x8
.global sUsedHeapSize__Q28JASystem9BNKParser
sUsedHeapSize__Q28JASystem9BNKParser:
	.skip 0x8
.global oscp$295
oscp$295:
	.skip 0x4
.global init$296
init$296:
	.skip 0x4
.global init$332
init$332:
	.skip 0x4
.global oRandom$331
oRandom$331:
	.skip 0x4
.global sCurrentHeap__Q28JASystem9TWaveBank
sCurrentHeap__Q28JASystem9TWaveBank:
	.skip 0x8
.global sTableSize__Q28JASystem11WaveBankMgr
sTableSize__Q28JASystem11WaveBankMgr:
	.skip 0x4
.global sWaveBank__Q28JASystem11WaveBankMgr
sWaveBank__Q28JASystem11WaveBankMgr:
	.skip 0x4
.global sUsedHeapSize__Q28JASystem8WSParser
sUsedHeapSize__Q28JASystem8WSParser:
	.skip 0x8
.global isDSPBoot__Q28JASystem11AudioThread
isDSPBoot__Q28JASystem11AudioThread:
	.skip 0x4
.global audioproc_mq_init__Q28JASystem11AudioThread
audioproc_mq_init__Q28JASystem11AudioThread:
	.skip 0x4
.global intcount__Q28JASystem11AudioThread
intcount__Q28JASystem11AudioThread:
	.skip 0x4
.global priority_set__Q28JASystem11AudioThread
priority_set__Q28JASystem11AudioThread:
	.skip 0x4
.global jac_pri__Q28JASystem11AudioThread
jac_pri__Q28JASystem11AudioThread:
	.skip 0x4
.global dvd_pri__Q28JASystem11AudioThread
dvd_pri__Q28JASystem11AudioThread:
	.skip 0x4
.global first$120
first$120:
	.skip 0x4
.global init$121
init$121:
	.skip 0x4
.global waitp__Q38JASystem6Driver8DSPQueue
waitp__Q38JASystem6Driver8DSPQueue:
	.skip 0x4
.global waittime__Q38JASystem6Driver8DSPQueue
waittime__Q38JASystem6Driver8DSPQueue:
	.skip 0x4
.global cur_waits__Q38JASystem6Driver8DSPQueue
cur_waits__Q38JASystem6Driver8DSPQueue:
	.skip 0x4
.global cur_top__Q38JASystem6Driver8DSPQueue
cur_top__Q38JASystem6Driver8DSPQueue:
	.skip 0x4
.global cur_tail__Q38JASystem6Driver8DSPQueue
cur_tail__Q38JASystem6Driver8DSPQueue:
	.skip 0x8
.global GLOBAL_CHANNEL__Q28JASystem8ChGlobal
GLOBAL_CHANNEL__Q28JASystem8ChGlobal:
	.skip 0x4
.global CHANNEL__Q28JASystem8ChGlobal
CHANNEL__Q28JASystem8ChGlobal:
	.skip 0x4
.global OSCILLATOR__Q28JASystem8ChGlobal
OSCILLATOR__Q28JASystem8ChGlobal:
	.skip 0x8
.global write_buffer__Q28JASystem6DSPBuf
write_buffer__Q28JASystem6DSPBuf:
	.skip 0x1
.global read_buffer__Q28JASystem6DSPBuf
read_buffer__Q28JASystem6DSPBuf:
	.skip 0x1
.global dspstatus__Q28JASystem6DSPBuf
dspstatus__Q28JASystem6DSPBuf:
	.skip 0x2
.global dac_sync_counter__Q28JASystem6DSPBuf
dac_sync_counter__Q28JASystem6DSPBuf:
	.skip 0x4
.global DSPCH__Q28JASystem11TDSPChannel
DSPCH__Q28JASystem11TDSPChannel:
	.skip 0x4
.global smnUse__Q28JASystem11TDSPChannel
smnUse__Q28JASystem11TDSPChannel:
	.skip 0x4
.global first$144
first$144:
	.skip 0x4
.global init$145
init$145:
	.skip 0x4
.global old_time__8JASystem
old_time__8JASystem:
	.skip 0x8
.global CH_BUF__Q28JASystem12DSPInterface
CH_BUF__Q28JASystem12DSPInterface:
	.skip 0x4
.global FX_BUF__Q28JASystem12DSPInterface
FX_BUF__Q28JASystem12DSPInterface:
	.skip 0x4
.global JASUniversalDacCounter__Q28JASystem6Kernel
JASUniversalDacCounter__Q28JASystem6Kernel:
	.skip 0x4
.global lastRspMadep__Q28JASystem6Kernel
lastRspMadep__Q28JASystem6Kernel:
	.skip 0x4
.global useRspMadep__Q28JASystem6Kernel
useRspMadep__Q28JASystem6Kernel:
	.skip 0x4
.global vframeWorkRunning__Q28JASystem6Kernel
vframeWorkRunning__Q28JASystem6Kernel:
	.skip 0x4
.global dacCallbackFunc__Q28JASystem6Kernel
dacCallbackFunc__Q28JASystem6Kernel:
	.skip 0x4
.global JASVframeCounter__Q28JASystem6Kernel
JASVframeCounter__Q28JASystem6Kernel:
	.skip 0x4
.global extMixCallback__Q28JASystem6Kernel
extMixCallback__Q28JASystem6Kernel:
	.skip 0x4
.global extMixMode__Q28JASystem6Kernel
extMixMode__Q28JASystem6Kernel:
	.skip 0x4
.global dacp$140
dacp$140:
	.skip 0x4
.global init$141
init$141:
	.skip 0x4
.global JASC_SINTABLE__Q28JASystem4Calc
JASC_SINTABLE__Q28JASystem4Calc:
	.skip 0x4
.global JASC_DOL2TABLE__Q28JASystem4Calc
JASC_DOL2TABLE__Q28JASystem4Calc:
	.skip 0x4
.global callList__Q28JASystem6Kernel
callList__Q28JASystem6Kernel:
	.skip 0x4
.global callbackInit__Q28JASystem6Kernel
callbackInit__Q28JASystem6Kernel:
	.skip 0x4
.global cmd_once
cmd_once:
	.skip 0x8
.global cmd_stay
cmd_stay:
	.skip 0x8
.global mqInit__Q28JASystem3Dvd
mqInit__Q28JASystem3Dvd:
	.skip 0x4
.global buffersize__Q28JASystem3Dvd
buffersize__Q28JASystem3Dvd:
	.skip 0x4
.global buffers__Q28JASystem3Dvd
buffers__Q28JASystem3Dvd:
	.skip 0x4
.global nextBufferSize__Q28JASystem3Dvd
nextBufferSize__Q28JASystem3Dvd:
	.skip 0x4
.global nextBufferTop__Q28JASystem3Dvd
nextBufferTop__Q28JASystem3Dvd:
	.skip 0x4
.global nextBuffers__Q28JASystem3Dvd
nextBuffers__Q28JASystem3Dvd:
	.skip 0x4
.global audioDvdBuffer__Q28JASystem3Dvd
audioDvdBuffer__Q28JASystem3Dvd:
	.skip 0x8
.global bufferLoad__Q28JASystem3Dvd
bufferLoad__Q28JASystem3Dvd:
	.skip 0x4
.global bufferFull__Q28JASystem3Dvd
bufferFull__Q28JASystem3Dvd:
	.skip 0x4
.global callStackArray__Q28JASystem3Dvd
callStackArray__Q28JASystem3Dvd:
	.skip 0x4
.global msgBuf__Q28JASystem3Dvd
msgBuf__Q28JASystem3Dvd:
	.skip 0x4
.global curQ__Q28JASystem3Dvd
curQ__Q28JASystem3Dvd:
	.skip 0x4
.global dvdtSleep__Q28JASystem3Dvd
dvdtSleep__Q28JASystem3Dvd:
	.skip 0x8
.global dvdThreadPauseFlag__Q28JASystem3Dvd
dvdThreadPauseFlag__Q28JASystem3Dvd:
	.skip 0x4
.global errorCallback__Q28JASystem3Dvd
errorCallback__Q28JASystem3Dvd:
	.skip 0x4
.global dvdFileDics__Q28JASystem3Dvd
dvdFileDics__Q28JASystem3Dvd:
	.skip 0x4
.global dvdFile__Q28JASystem3Dvd
dvdFile__Q28JASystem3Dvd:
	.skip 0x4
.global dvdEntryNum__Q28JASystem3Dvd
dvdEntryNum__Q28JASystem3Dvd:
	.skip 0x4
.global arq_index$316
arq_index$316:
	.skip 0x4
.global init$317
init$317:
	.skip 0x4
.global useHardStreaming__Q28JASystem10HardStream
useHardStreaming__Q28JASystem10HardStream:
	.skip 0x4
.global streamFiles__Q28JASystem10HardStream
streamFiles__Q28JASystem10HardStream:
	.skip 0x4
.global playList__Q28JASystem10HardStream
playList__Q28JASystem10HardStream:
	.skip 0x4
.global cur_finfo$426
cur_finfo$426:
	.skip 0x4
.global init$427
init$427:
	.skip 0x1
.global cur_addr_cmd$432
cur_addr_cmd$432:
	.skip 0x1
.global init$433
init$433:
	.skip 0x2
.global last_frame$577
last_frame$577:
	.skip 0x4
.global init$578
init$578:
	.skip 0x8
.global global_id__Q28JASystem6Kernel
global_id__Q28JASystem6Kernel:
	.skip 0x8
.global gOutputRate__Q28JASystem6Kernel
gOutputRate__Q28JASystem6Kernel:
	.skip 0x4
.global gAiSetting__Q28JASystem6Kernel
gAiSetting__Q28JASystem6Kernel:
	.skip 0x4
.global JASDram
JASDram:
	.skip 0x4
.global JASAramDmaBufferTop__Q28JASystem6Kernel
JASAramDmaBufferTop__Q28JASystem6Kernel:
	.skip 0x4
.global audioAramTop__Q28JASystem6Kernel
audioAramTop__Q28JASystem6Kernel:
	.skip 0x4
.global CARD_SECURITY_BUFFER__Q28JASystem6Kernel
CARD_SECURITY_BUFFER__Q28JASystem6Kernel:
	.skip 0x4
.global vlDirName__Q28JASystem5Vload
vlDirName__Q28JASystem5Vload:
	.skip 0x4
.global vlArcName__Q28JASystem5Vload
vlArcName__Q28JASystem5Vload:
	.skip 0x4
.global vlArc__Q28JASystem5Vload
vlArc__Q28JASystem5Vload:
	.skip 0x4
.global vlCurrentArcs__Q28JASystem5Vload
vlCurrentArcs__Q28JASystem5Vload:
	.skip 0x4
.global sUpdateSyncMode__Q28JASystem6TTrack
sUpdateSyncMode__Q28JASystem6TTrack:
	.skip 0x4
.global sCallBackFunc__Q28JASystem6TTrack
sCallBackFunc__Q28JASystem6TTrack:
	.skip 0x4
.global sTrackPool__Q28JASystem8TrackMgr
sTrackPool__Q28JASystem8TrackMgr:
	.skip 0x4
.global sSeqOuter__Q28JASystem8TrackMgr
sSeqOuter__Q28JASystem8TrackMgr:
	.skip 0x4
.global sRootTrack__Q28JASystem8TrackMgr
sRootTrack__Q28JASystem8TrackMgr:
	.skip 0x4
.global sFreeSeqpQueue__Q28JASystem8TrackMgr
sFreeSeqpQueue__Q28JASystem8TrackMgr:
	.skip 0x4
.global sRootSeqCount__Q28JASystem8TrackMgr
sRootSeqCount__Q28JASystem8TrackMgr:
	.skip 0x4
.global sTrackCount__Q28JASystem8TrackMgr
sTrackCount__Q28JASystem8TrackMgr:
	.skip 0x4
.global sTLists__Q28JASystem8TrackMgr
sTLists__Q28JASystem8TrackMgr:
	.skip 0x4
.global seqRemain__Q28JASystem8TrackMgr
seqRemain__Q28JASystem8TrackMgr:
	.skip 0x4
.global getP__Q28JASystem8TrackMgr
getP__Q28JASystem8TrackMgr:
	.skip 0x4
.global backP__Q28JASystem8TrackMgr
backP__Q28JASystem8TrackMgr:
	.skip 0x4
.global FieldRand
FieldRand:
	.skip 0x8
.global pcb__14JPADrawContext
pcb__14JPADrawContext:
	.skip 0x8
.global flag
flag:
	.skip 0x8
.global taskreadp
taskreadp:
	.skip 0x4
.global taskwritep
taskwritep:
	.skip 0x4
.global __DSP_tmp_task
__DSP_tmp_task:
	.skip 0x8
.global DSP_prior_yield
DSP_prior_yield:
	.skip 0x4
.global AUDIO_UPDATE_REQUEST
AUDIO_UPDATE_REQUEST:
	.skip 0x4
.global DSP_prior_task
DSP_prior_task:
	.skip 0x8
.global __global_destructor_chain
__global_destructor_chain:
	.skip 0x8
.global __aborting
__aborting:
	.skip 0x4
.global atexit_curr_func
atexit_curr_func:
	.skip 0x4
.global __atexit_curr_func
__atexit_curr_func:
	.skip 0x4
.global __stdio_exit
__stdio_exit:
	.skip 0x4
.global __console_exit
__console_exit:
	.skip 0x8
.global errno
errno:
	.skip 0x8
.global initialized$16
initialized$16:
	.skip 0x8
.global __DBInterface
__DBInterface:
	.skip 0x4
.global DBVerbose
DBVerbose:
	.skip 0x4
.global BootInfo
BootInfo:
	.skip 0x4
.global BI2DebugFlag
BI2DebugFlag:
	.skip 0x4
.global BI2DebugFlagHolder
BI2DebugFlagHolder:
	.skip 0x4
.global __OSIsGcam
__OSIsGcam:
	.skip 0x4
.global AreWeInitialized
AreWeInitialized:
	.skip 0x4
.global OSExceptionTable
OSExceptionTable:
	.skip 0x4
.global __OSSavedRegionEnd
__OSSavedRegionEnd:
	.skip 0x4
.global __OSSavedRegionStart
__OSSavedRegionStart:
	.skip 0x4
.global __OSInIPL
__OSInIPL:
	.skip 0x8
.global __OSStartTime
__OSStartTime:
	.skip 0x4
.global lbl_80409DAC
lbl_80409DAC:
	.skip 0x4
.global AlarmQueue
AlarmQueue:
	.skip 0x8
.global HeapArray
HeapArray:
	.skip 0x4
.global NumHeaps
NumHeaps:
	.skip 0x4
.global ArenaStart
ArenaStart:
	.skip 0x4
.global ArenaEnd
ArenaEnd:
	.skip 0x4
.global __OSArenaHi
__OSArenaHi:
	.skip 0x8
.global FontData
FontData:
	.skip 0x4
.global SheetImage
SheetImage:
	.skip 0x4
.global WidthTable
WidthTable:
	.skip 0x4
.global CharsInSheet
CharsInSheet:
	.skip 0x4
.global InterruptHandlerTable
InterruptHandlerTable:
	.skip 0x4
.global __OSLastInterruptSrr0
__OSLastInterruptSrr0:
	.skip 0x4
.global __OSLastInterrupt
__OSLastInterrupt:
	.skip 0x8
.global __OSLastInterruptTime
__OSLastInterruptTime:
	.skip 0x4
.global lbl_80409DF4
lbl_80409DF4:
	.skip 0x4
.global SaveStart
SaveStart:
	.skip 0x4
.global SaveEnd
SaveEnd:
	.skip 0x4
.global Prepared
Prepared:
	.skip 0x8
.global ResetFunctionQueue
ResetFunctionQueue:
	.skip 0x8
.global ResetCallback
ResetCallback:
	.skip 0x4
.global Down
Down:
	.skip 0x4
.global LastState
LastState:
	.skip 0x8
.global HoldUp
HoldUp:
	.skip 0x4
.global lbl_80409E24
lbl_80409E24:
	.skip 0x4
.global HoldDown
HoldDown:
	.skip 0x4
.global lbl_80409E2C
lbl_80409E2C:
	.skip 0x4
.global RunQueueBits
RunQueueBits:
	.skip 0x4
.global RunQueueHint
RunQueueHint:
	.skip 0x4
.global Reschedule
Reschedule:
	.skip 0x8
.global StopAtNextInt
StopAtNextInt:
	.skip 0x4
.global LastLength
LastLength:
	.skip 0x4
.global Callback
Callback:
	.skip 0x4
.global ResetCoverCallback
ResetCoverCallback:
	.skip 0x4
.global LastResetEnd
LastResetEnd:
	.skip 0x4
.global lbl_80409E54
lbl_80409E54:
	.skip 0x4
.global ResetOccurred
ResetOccurred:
	.skip 0x4
.global WaitingCoverClose
WaitingCoverClose:
	.skip 0x4
.global Breaking
Breaking:
	.skip 0x4
.global WorkAroundType
WorkAroundType:
	.skip 0x4
.global WorkAroundSeekLocation
WorkAroundSeekLocation:
	.skip 0x8
.global LastReadFinished
LastReadFinished:
	.skip 0x4
.global lbl_80409E74
lbl_80409E74:
	.skip 0x4
.global LastReadIssued
LastReadIssued:
	.skip 0x4
.global lbl_80409E7C
lbl_80409E7C:
	.skip 0x4
.global LastCommandWasRead
LastCommandWasRead:
	.skip 0x4
.global NextCommandNumber
NextCommandNumber:
	.skip 0x4
.global BootInfo
BootInfo:
	.skip 0x4
.global FstStart
FstStart:
	.skip 0x4
.global FstStringStart
FstStringStart:
	.skip 0x4
.global MaxEntryNum
MaxEntryNum:
	.skip 0x4
.global currentDirectory
currentDirectory:
	.skip 0x4
.global __DVDLongFileNameFlag
__DVDLongFileNameFlag:
	.skip 0x4
.global __DVDThreadQueue
__DVDThreadQueue:
	.skip 0x8
.global executing
executing:
	.skip 0x4
.global currID
currID:
	.skip 0x4
.global bootInfo
bootInfo:
	.skip 0x4
.global PauseFlag
PauseFlag:
	.skip 0x4
.global PausingFlag
PausingFlag:
	.skip 0x4
.global AutoFinishing
AutoFinishing:
	.skip 0x4
.global FatalErrorFlag
FatalErrorFlag:
	.skip 0x4
.global CurrCommand
CurrCommand:
	.skip 0x4
.global Canceling
Canceling:
	.skip 0x4
.global CancelCallback
CancelCallback:
	.skip 0x4
.global ResumeFromHere
ResumeFromHere:
	.skip 0x4
.global CancelLastError
CancelLastError:
	.skip 0x4
.global LastError
LastError:
	.skip 0x4
.global NumInternalRetry
NumInternalRetry:
	.skip 0x4
.global ResetRequired
ResetRequired:
	.skip 0x4
.global FirstTimeInBootrom
FirstTimeInBootrom:
	.skip 0x4
.global DVDInitialized
DVDInitialized:
	.skip 0x4
.global LastState
LastState:
	.skip 0x4
.global status
status:
	.skip 0x4
.global bb2
bb2:
	.skip 0x4
.global idTmp
idTmp:
	.skip 0x8
.global retraceCount
retraceCount:
	.skip 0x4
.global flushFlag
flushFlag:
	.skip 0x4
.global retraceQueue
retraceQueue:
	.skip 0x8
.global PreCB
PreCB:
	.skip 0x4
.global PostCB
PostCB:
	.skip 0x4
.global encoderType
encoderType:
	.skip 0x4
.global displayOffsetH
displayOffsetH:
	.skip 0x2
.global displayOffsetV
displayOffsetV:
	.skip 0x2
.global changeMode
changeMode:
	.skip 0x8
.global changed
changed:
	.skip 0x4
.global lbl_80409F2C
lbl_80409F2C:
	.skip 0x4
.global shdwChangeMode
shdwChangeMode:
	.skip 0x8
.global shdwChanged
shdwChanged:
	.skip 0x4
.global lbl_80409F3C
lbl_80409F3C:
	.skip 0x4
.global CurrTiming
CurrTiming:
	.skip 0x4
.global CurrTvMode
CurrTvMode:
	.skip 0x4
.global FBSet
FBSet:
	.skip 0x4
.global message$332
message$332:
	.skip 0x4
.global Initialized
Initialized:
	.skip 0x4
.global EnabledBits
EnabledBits:
	.skip 0x4
.global ResettingBits
ResettingBits:
	.skip 0x4
.global RecalibrateBits
RecalibrateBits:
	.skip 0x4
.global WaitingBits
WaitingBits:
	.skip 0x4
.global CheckingBits
CheckingBits:
	.skip 0x4
.global PendingBits
PendingBits:
	.skip 0x4
.global SamplingCallback
SamplingCallback:
	.skip 0x4
.global recalibrated$396
recalibrated$396:
	.skip 0x4
.global __PADSpec
__PADSpec:
	.skip 0x4
.global __AIS_Callback
__AIS_Callback:
	.skip 0x4
.global __AID_Callback
__AID_Callback:
	.skip 0x4
.global __CallbackStack
__CallbackStack:
	.skip 0x4
.global __OldStack
__OldStack:
	.skip 0x4
.global __AI_init_flag
__AI_init_flag:
	.skip 0x8
.global bound_32KHz
bound_32KHz:
	.skip 0x4
.global lbl_80409F94
lbl_80409F94:
	.skip 0x4
.global bound_48KHz
bound_48KHz:
	.skip 0x4
.global lbl_80409F9C
lbl_80409F9C:
	.skip 0x4
.global min_wait
min_wait:
	.skip 0x4
.global lbl_80409FA4
lbl_80409FA4:
	.skip 0x4
.global max_wait
max_wait:
	.skip 0x4
.global lbl_80409FAC
lbl_80409FAC:
	.skip 0x4
.global buffer
buffer:
	.skip 0x4
.global lbl_80409FB4
lbl_80409FB4:
	.skip 0x4
.global __AR_Callback
__AR_Callback:
	.skip 0x4
.global __AR_Size
__AR_Size:
	.skip 0x4
.global __AR_InternalSize
__AR_InternalSize:
	.skip 0x4
.global __AR_ExpansionSize
__AR_ExpansionSize:
	.skip 0x4
.global __AR_StackPointer
__AR_StackPointer:
	.skip 0x4
.global __AR_FreeBlocks
__AR_FreeBlocks:
	.skip 0x4
.global __AR_BlockLength
__AR_BlockLength:
	.skip 0x4
.global __AR_init_flag
__AR_init_flag:
	.skip 0x4
.global __ARQRequestQueueHi
__ARQRequestQueueHi:
	.skip 0x4
.global __ARQRequestTailHi
__ARQRequestTailHi:
	.skip 0x4
.global __ARQRequestQueueLo
__ARQRequestQueueLo:
	.skip 0x4
.global __ARQRequestTailLo
__ARQRequestTailLo:
	.skip 0x4
.global __ARQRequestPendingHi
__ARQRequestPendingHi:
	.skip 0x4
.global __ARQRequestPendingLo
__ARQRequestPendingLo:
	.skip 0x4
.global __ARQCallbackHi
__ARQCallbackHi:
	.skip 0x4
.global __ARQCallbackLo
__ARQCallbackLo:
	.skip 0x4
.global __ARQChunkSize
__ARQChunkSize:
	.skip 0x4
.global __ARQ_init_flag
__ARQ_init_flag:
	.skip 0x4
.global __DSP_init_flag
__DSP_init_flag:
	.skip 0x8
.global __DSP_last_task
__DSP_last_task:
	.skip 0x4
.global __DSP_first_task
__DSP_first_task:
	.skip 0x4
.global __DSP_curr_task
__DSP_curr_task:
	.skip 0x8
.global __piReg
__piReg:
	.skip 0x4
.global __cpReg
__cpReg:
	.skip 0x4
.global __peReg
__peReg:
	.skip 0x4
.global __memReg
__memReg:
	.skip 0x4
.global CPUFifo
CPUFifo:
	.skip 0x4
.global GPFifo
GPFifo:
	.skip 0x4
.global __GXCurrentThread
__GXCurrentThread:
	.skip 0x4
.global CPGPLinked
CPGPLinked:
	.skip 0x4
.global GXOverflowSuspendInProgress
GXOverflowSuspendInProgress:
	.skip 0x4
.global BreakPointCB
BreakPointCB:
	.skip 0x4
.global __GXOverflowCount
__GXOverflowCount:
	.skip 0x4
.global __GXCurrentBP
__GXCurrentBP:
	.skip 0x4
.global TokenCB
TokenCB:
	.skip 0x4
.global DrawDoneCB
DrawDoneCB:
	.skip 0x4
.global DrawDone
DrawDone:
	.skip 0x4
.global FinishQueue
FinishQueue:
	.skip 0xC
.global MTRCallback
MTRCallback:
	.skip 0x4
.global DBGCallback
DBGCallback:
	.skip 0x4
.global SendMailData
SendMailData:
	.skip 0x4
.global RecvDataLeng
RecvDataLeng:
	.skip 0x4
.global pEXIInputFlag
pEXIInputFlag:
	.skip 0x4
.global EXIInputFlag
EXIInputFlag:
	.skip 0x4
.global __GDCurrentDL
__GDCurrentDL:
	.skip 0x4
.global overflowcb
overflowcb:
	.skip 0x4
.global cmdTypeAndStatus$77
cmdTypeAndStatus$77:
	.skip 0x4
.global cmdTypeAndStatus$371
cmdTypeAndStatus$371:
	.skip 0x4
.global __PADFixBits
__PADFixBits:
	.skip 0x8
.global SamplingRate
SamplingRate:
	.skip 0x8
.global Chan
Chan:
	.skip 0x4
.global Dev
Dev:
	.skip 0x4
.global Enabled
Enabled:
	.skip 0x4
.global BarnacleEnabled
BarnacleEnabled:
	.skip 0x1C
.global Ydchuff
Ydchuff:
	.skip 0x20
.global Udchuff
Udchuff:
	.skip 0x20
.global Vdchuff
Vdchuff:
	.skip 0x20
.global Yachuff
Yachuff:
	.skip 0x20
.global Uachuff
Uachuff:
	.skip 0x20
.global Vachuff
Vachuff:
	.skip 0x4
.global __THPHuffmanBits
__THPHuffmanBits:
	.skip 0x4
.global __THPHuffmanSizeTab
__THPHuffmanSizeTab:
	.skip 0x4
.global __THPHuffmanCodeTab
__THPHuffmanCodeTab:
	.skip 0x14
.global Gbase
Gbase:
	.skip 0x20
.global Gwid
Gwid:
	.skip 0x20
.global Gq
Gq:
	.skip 0x4
.global __THPOldGQR5
__THPOldGQR5:
	.skip 0x4
.global __THPOldGQR6
__THPOldGQR6:
	.skip 0x4
.global __THPWorkArea
__THPWorkArea:
	.skip 0x4
.global __THPInfo
__THPInfo:
	.skip 0x4
.global __THPInitFlag
__THPInitFlag:
	.skip 0x4
.global AudioDecodeThreadCreated
AudioDecodeThreadCreated:
	.skip 0x8
.global Initialized
Initialized:
	.skip 0x4
.global PrepareReadyMessage
PrepareReadyMessage:
	.skip 0x4
.global OldVIPostCallback
OldVIPostCallback:
	.skip 0x4
.global SoundBufferIndex
SoundBufferIndex:
	.skip 0x4
.global LastAudioBuffer
LastAudioBuffer:
	.skip 0x4
.global CurAudioBuffer
CurAudioBuffer:
	.skip 0x4
.global ReadThreadCreated
ReadThreadCreated:
	.skip 0x8
.global VideoDecodeThreadCreated
VideoDecodeThreadCreated:
	.skip 0x4
.global First
First:
	.skip 0x4
.global gpSilhouetteManager
gpSilhouetteManager:
	.skip 0x4
.global sKeepViewClipFovy
sKeepViewClipFovy:
	.skip 0x4
.global sKeepViewClipAspect
sKeepViewClipAspect:
	.skip 0x4
.global sKeepViewClipNear
sKeepViewClipNear:
	.skip 0x4
.global sKeepViewClipFar
sKeepViewClipFar:
	.skip 0x8
.global mAmbAry__12TLightCommon
mAmbAry__12TLightCommon:
	.skip 0x4
.global mLightAry__12TLightCommon
mLightAry__12TLightCommon:
	.skip 0x4
.global mLightPos__12TLightCommon
mLightPos__12TLightCommon:
	.skip 0x4
.global gpLightManager
gpLightManager:
	.skip 0x4
.global gpAfterEffect
gpAfterEffect:
	.skip 0x4
.global gpScreenTexture
gpScreenTexture:
	.skip 0x4
.global gpBindShadowManager
gpBindShadowManager:
	.skip 0x4
.global mJoinDist__19TMBindShadowManager
mJoinDist__19TMBindShadowManager:
	.skip 0x4
.global mTestSw__19TMBindShadowManager
mTestSw__19TMBindShadowManager:
	.skip 0x1
.global mDLSw__19TMBindShadowManager
mDLSw__19TMBindShadowManager:
	.skip 0x1
.global init$2175
init$2175:
	.skip 0x1
.global init$2179
init$2179:
	.skip 0x1
.global init$2193
init$2193:
	.skip 0x1
.global init$2198
init$2198:
	.skip 0x1
.global init$2210
init$2210:
	.skip 0x1
.global init$2219
init$2219:
	.skip 0x1
.global SMSRumbleMgr
SMSRumbleMgr:
	.skip 0x8
.global currentTGDLStatic
currentTGDLStatic:
	.skip 0x8
.global gpMarioParticleManager
gpMarioParticleManager:
	.skip 0x4
.global emitterCallBackBindToPosPtr
emitterCallBackBindToPosPtr:
	.skip 0x4
.global emitterCallBackBindToMtxPtr
emitterCallBackBindToMtxPtr:
	.skip 0x4
.global emitterCallBackBindToSRTMtxPtr
emitterCallBackBindToSRTMtxPtr:
	.skip 0x4
.global smList__15JALList$$05MSBgm$$1
smList__15JALList$$05MSBgm$$1:
	.skip 0xC
.global smList__24JALList$$013MSSetSoundGrp$$1
smList__24JALList$$013MSSetSoundGrp$$1:
	.skip 0xC
.global smList__21JALList$$010MSSetSound$$1
smList__21JALList$$010MSSetSound$$1:
	.skip 0xC
.global __init__smList__15JALList$$05MSBgm$$1
__init__smList__15JALList$$05MSBgm$$1:
	.skip 0x1
.global __init__smList__24JALList$$013MSSetSoundGrp$$1
__init__smList__24JALList$$013MSSetSoundGrp$$1:
	.skip 0x1
.global __init__smList__21JALList$$010MSSetSound$$1
__init__smList__21JALList$$010MSSetSound$$1:
	.skip 0x2
.global smInstance__12TFlagManager
smInstance__12TFlagManager:
	.skip 0x8
.global gpSceneCmnDat
gpSceneCmnDat:
	.skip 0x4
.global gpSceneCmnDatSize
gpSceneCmnDatSize:
	.skip 0x4
.global mArc__7TParams
mArc__7TParams:
	.skip 0x4
.global mSceneArc__7TParams
mSceneArc__7TParams:
	.skip 0x4
.global gpMarDirector
gpMarDirector:
	.skip 0x4
.global gpMSound
gpMSound:
	.skip 0x4
.global gpSetupThreadStack
gpSetupThreadStack:
	.skip 0x4
.global gpCardManager
gpCardManager:
	.skip 0x4
.global gpSystemFont
gpSystemFont:
	.skip 0x4
.global gpRomFont
gpRomFont:
	.skip 0x4
.global sGameInit
sGameInit:
	.skip 0x4
.global arcBufNLogo
arcBufNLogo:
	.skip 0x4
.global arcBufMario
arcBufMario:
	.skip 0x4
.global arcBufCmn
arcBufCmn:
	.skip 0x4
.global bufStageArcBin
bufStageArcBin:
	.skip 0x4
.global spGameHeapBlock
spGameHeapBlock:
	.skip 0x4
.global gArBkConsole
gArBkConsole:
	.skip 0x8
.global gArBkGuide
gArBkGuide:
	.skip 0x8
.global mResetFlag__13TMarioGamePad
mResetFlag__13TMarioGamePad:
	.skip 0x8
.global gpPositionHolder
gpPositionHolder:
	.skip 0x8
.global _instance__8TTimeRec
_instance__8TTimeRec:
	.skip 0x8
.global smInstance__16TDrawSyncManager
smInstance__16TDrawSyncManager:
	.skip 0x8
.global sDetach
sDetach:
	.skip 0x8
.global gpResourceManager
gpResourceManager:
	.skip 0x4
.global gpEmitterManager4D2
gpEmitterManager4D2:
	.skip 0x4
.global msStg__Q210MSMainProc11MSStageInfo
msStg__Q210MSMainProc11MSStageInfo:
	.skip 0x4
.global demoBgm__Q210MSMainProc11MSStageInfo
demoBgm__Q210MSMainProc11MSStageInfo:
	.skip 0x4
.global stageBgm__Q210MSMainProc11MSStageInfo
stageBgm__Q210MSMainProc11MSStageInfo:
	.skip 0x4
.global stageBgmSilent__Q210MSMainProc11MSStageInfo
stageBgmSilent__Q210MSMainProc11MSStageInfo:
	.skip 0x4
.global stageBgmSilentStartStatus__Q210MSMainProc11MSStageInfo
stageBgmSilentStartStatus__Q210MSMainProc11MSStageInfo:
	.skip 0x1
.global flags__Q210MSMainProc11MSStageInfo
flags__Q210MSMainProc11MSStageInfo:
	.skip 0x1
.global volOffCategory__Q210MSMainProc11MSStageInfo
volOffCategory__Q210MSMainProc11MSStageInfo:
	.skip 0x2
.global fadeEvent__Q210MSMainProc11MSStageInfo
fadeEvent__Q210MSMainProc11MSStageInfo:
	.skip 0x4
.global switchBgm__Q210MSMainProc11MSStageInfo
switchBgm__Q210MSMainProc11MSStageInfo:
	.skip 0x4
.global switchBgm2__Q210MSMainProc11MSStageInfo
switchBgm2__Q210MSMainProc11MSStageInfo:
	.skip 0x4
.global cubeFadeRatio__Q210MSMainProc11MSStageInfo
cubeFadeRatio__Q210MSMainProc11MSStageInfo:
	.skip 0x4
.global cubeFadeUsePan__Q210MSMainProc11MSStageInfo
cubeFadeUsePan__Q210MSMainProc11MSStageInfo:
	.skip 0x1
.global bossLives__Q210MSMainProc11MSStageInfo
bossLives__Q210MSMainProc11MSStageInfo:
	.skip 0x1
.global bossLives2__Q210MSMainProc11MSStageInfo
bossLives2__Q210MSMainProc11MSStageInfo:
	.skip 0x1
.global bossNotDamaged__Q210MSMainProc11MSStageInfo
bossNotDamaged__Q210MSMainProc11MSStageInfo:
	.skip 0x1
.global distFadeStageToKage__Q210MSMainProc11MSStageInfo
distFadeStageToKage__Q210MSMainProc11MSStageInfo:
	.skip 0x4
.global smMSStage__7MSStage
smMSStage__7MSStage:
	.skip 0x4
.global gpTargetArrow
gpTargetArrow:
	.skip 0x8
.global gpObjHitCheck
gpObjHitCheck:
	.skip 0x8
.global init$325
init$325:
	.skip 0x8
.global gpStrategy
gpStrategy:
	.skip 0x8
.global gpQuestionManager
gpQuestionManager:
	.skip 0x8
.global gpMarioForCallBack
gpMarioForCallBack:
	.skip 0x8
.global gpMarioOriginal
gpMarioOriginal:
	.skip 0x8
.global init$2693
init$2693:
	.skip 0x4
.global bubbleCallBack
bubbleCallBack:
	.skip 0x4
.global init$2744
init$2744:
	.skip 0x4
.global warpInCallBack
warpInCallBack:
	.skip 0x4
.global gpSplashManager
gpSplashManager:
	.skip 0x8
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
.global gpModelWaterManager
gpModelWaterManager:
	.skip 0x4
.global init$2855
init$2855:
	.skip 0x4
.global sphere_glist_p
sphere_glist_p:
	.skip 0x4
.global sphere_pos_t
sphere_pos_t:
	.skip 0x4
.global initialized$3156
initialized$3156:
	.skip 0x1
.global init$3157
init$3157:
	.skip 0x7
.global mPtrSaveNormal__8TBaseNPC
mPtrSaveNormal__8TBaseNPC:
	.skip 0x4
.global mAngleYDiffWhenTaken__8TBaseNPC
mAngleYDiffWhenTaken__8TBaseNPC:
	.skip 0x4
.global gpCurrentNpc
gpCurrentNpc:
	.skip 0x8
.global mStaticCommonKeeper__18TMonteMBaseManager
mStaticCommonKeeper__18TMonteMBaseManager:
	.skip 0x4
.global mStaticCommonKeeper__18TMonteWBaseManager
mStaticCommonKeeper__18TMonteWBaseManager:
	.skip 0x4
.global mStaticCommonKeeper__17TMareMBaseManager
mStaticCommonKeeper__17TMareMBaseManager:
	.skip 0x4
.global mStaticCommonKeeper__17TMareWBaseManager
mStaticCommonKeeper__17TMareWBaseManager:
	.skip 0x4
.global mStaticBmtNormal__16TMareBaseManager
mStaticBmtNormal__16TMareBaseManager:
	.skip 0x4
.global mStaticBmtPollution__16TMareBaseManager
mStaticBmtPollution__16TMareBaseManager:
	.skip 0x4
.global gpMareJellyFishManager
gpMareJellyFishManager:
	.skip 0x8
.global init$2214
init$2214:
	.skip 0x4
.global instance$2212
instance$2212:
	.skip 0x4
.global init$2245
init$2245:
	.skip 0x4
.global instance$2243
instance$2243:
	.skip 0x4
.global init$2253
init$2253:
	.skip 0x4
.global instance$2251
instance$2251:
	.skip 0x4
.global init$2266
init$2266:
	.skip 0x4
.global instance$2264
instance$2264:
	.skip 0x4
.global init$2277
init$2277:
	.skip 0x4
.global instance$2275
instance$2275:
	.skip 0x4
.global init$2305
init$2305:
	.skip 0x4
.global instance$2303
instance$2303:
	.skip 0x4
.global init$2351
init$2351:
	.skip 0x4
.global instance$2349
instance$2349:
	.skip 0x4
.global init$2361
init$2361:
	.skip 0x4
.global instance$2359
instance$2359:
	.skip 0x4
.global init$2367
init$2367:
	.skip 0x4
.global instance$2365
instance$2365:
	.skip 0x4
.global init$2375
init$2375:
	.skip 0x4
.global instance$2373
instance$2373:
	.skip 0x4
.global init$2386
init$2386:
	.skip 0x4
.global instance$2384
instance$2384:
	.skip 0x4
.global init$2392
init$2392:
	.skip 0x4
.global instance$2390
instance$2390:
	.skip 0x4
.global init$2406
init$2406:
	.skip 0x4
.global instance$2404
instance$2404:
	.skip 0x4
.global init$2416
init$2416:
	.skip 0x4
.global instance$2414
instance$2414:
	.skip 0x4
.global init$2426
init$2426:
	.skip 0x4
.global instance$2424
instance$2424:
	.skip 0x4
.global init$2438
init$2438:
	.skip 0x4
.global instance$2436
instance$2436:
	.skip 0x4
.global mDownSunflowerNum__9TNpcEvent
mDownSunflowerNum__9TNpcEvent:
	.skip 0x4
.global sViewObjName$2632
sViewObjName$2632:
	.skip 0x4
.global init$2633
init$2633:
	.skip 0x8
.global sCheckPollutedStartCounter$localstatic0$initSinkNpc___8TBaseNPCFv
sCheckPollutedStartCounter$localstatic0$initSinkNpc___8TBaseNPCFv:
	.skip 0x4
.global init$localstatic1$initSinkNpc___8TBaseNPCFv
init$localstatic1$initSinkNpc___8TBaseNPCFv:
	.skip 0x4
.global msAmpDecrease__11TNpcTrample
msAmpDecrease__11TNpcTrample:
	.skip 0x8
.global smWaterFilter__12MSSeCallBack
smWaterFilter__12MSSeCallBack:
	.skip 0x2
.global ukuleleFlag$2052
ukuleleFlag$2052:
	.skip 0x1
.global init$2053
init$2053:
	.skip 0x1
.global MSGMSound
MSGMSound:
	.skip 0x4
.global MSGBasic
MSGBasic:
	.skip 0x8
.global mObj__Q214MSoundSESystem8MSoundSE
mObj__Q214MSoundSESystem8MSoundSE:
	.skip 0x4
.global smSeSetting__Q214MSoundSESystem6SeInfo
smSeSetting__Q214MSoundSESystem6SeInfo:
	.skip 0xC
.global gpMapObjManager
gpMapObjManager:
	.skip 0x8
.global gpItemManager
gpItemManager:
	.skip 0x8
.global gpMapObjSwitch
gpMapObjSwitch:
	.skip 0x8
.global sAngleAdd
sAngleAdd:
	.skip 0x8
.global gpCurObject
gpCurObject:
	.skip 0x8
.global mSubmarineSurfaceTransY__15TRiccoWatermill
mSubmarineSurfaceTransY__15TRiccoWatermill:
	.skip 0x8
.global switchSnd
switchSnd:
	.skip 0x8
.global gpMapObjFlagManager
gpMapObjFlagManager:
	.skip 0x4
.global total_use_size$2278
total_use_size$2278:
	.skip 0x4
.global init$2279
init$2279:
	.skip 0x8
.global sColor
sColor:
	.skip 0x4
.global gpMapObjWave
gpMapObjWave:
	.skip 0x4
.global timer$2477
timer$2477:
	.skip 0x4
.global init$2478
init$2478:
	.skip 0x4
.global init$2646
init$2646:
	.skip 0x1
.global init$2711
init$2711:
	.skip 0x7
.global mColorUpper__19TMapObjGrassManager
mColorUpper__19TMapObjGrassManager:
	.skip 0x4
.global mColorLower__19TMapObjGrassManager
mColorLower__19TMapObjGrassManager:
	.skip 0x4
.global mColorFar__19TMapObjGrassManager
mColorFar__19TMapObjGrassManager:
	.skip 0x4
.global mDrawVecS16__19TMapObjGrassManager
mDrawVecS16__19TMapObjGrassManager:
	.skip 0x8
.global gpMapObjGrassManager
gpMapObjGrassManager:
	.skip 0x4
.global gpMapObjPoleManager
gpMapObjPoleManager:
	.skip 0x8
.global mRopeHeight__14THangingBridge
mRopeHeight__14THangingBridge:
	.skip 0x8
.global mColorUpper__17TMapObjTumblePole
mColorUpper__17TMapObjTumblePole:
	.skip 0x4
.global mColorLower__17TMapObjTumblePole
mColorLower__17TMapObjTumblePole:
	.skip 0x4
.global gpPoolManager
gpPoolManager:
	.skip 0x8
.global init$3092
init$3092:
	.skip 0x1
.global init$3289
init$3289:
	.skip 0x7
.global gpMap
gpMap:
	.skip 0x4
.global init$2896
init$2896:
	.skip 0x4
.global gpMapCollisionData
gpMapCollisionData:
	.skip 0x8
.global gpMirrorModelManager
gpMirrorModelManager:
	.skip 0x8
.global mEffectCoronaScale__13TMapStaticObj
mEffectCoronaScale__13TMapStaticObj:
	.skip 0x8
.global gpMapWireManager
gpMapWireManager:
	.skip 0x4
.global mUpperSurface__15TMapWireManager
mUpperSurface__15TMapWireManager:
	.skip 0x4
.global mLowerSurface__15TMapWireManager
mLowerSurface__15TMapWireManager:
	.skip 0x8
.global counter$2576
counter$2576:
	.skip 0x4
.global init$2577
init$2577:
	.skip 0x4
.global gpPollution
gpPollution:
	.skip 0x8
.global effect_counter$2421
effect_counter$2421:
	.skip 0x4
.global init$2422
init$2422:
	.skip 0x1
.global init$2427
init$2427:
	.skip 0x3
.global now_pos_no$2429
now_pos_no$2429:
	.skip 0x4
.global init$2430
init$2430:
	.skip 0x4
.global counter_x$2434
counter_x$2434:
	.skip 0x4
.global init$2435
init$2435:
	.skip 0x4
.global counter_z$2437
counter_z$2437:
	.skip 0x4
.global init$2438
init$2438:
	.skip 0x4
.global effect_timer$2449
effect_timer$2449:
	.skip 0x4
.global init$2450
init$2450:
	.skip 0x4
.global delta$2349
delta$2349:
	.skip 0x4
.global init$2350
init$2350:
	.skip 0x4
.global cDownTopPoint__11TGCConsole2
cDownTopPoint__11TGCConsole2:
	.skip 0x8
.global cDownMidPoint__11TGCConsole2
cDownMidPoint__11TGCConsole2:
	.skip 0x8
.global cDownBotPoint__11TGCConsole2
cDownBotPoint__11TGCConsole2:
	.skip 0x8
.global cUpTopPoint__11TGCConsole2
cUpTopPoint__11TGCConsole2:
	.skip 0x8
.global cUpMidPoint__11TGCConsole2
cUpMidPoint__11TGCConsole2:
	.skip 0x8
.global cUpBotPoint__11TGCConsole2
cUpBotPoint__11TGCConsole2:
	.skip 0x8
.global cCoinTopPoint__11TGCConsole2
cCoinTopPoint__11TGCConsole2:
	.skip 0x8
.global cCoinMidPoint__11TGCConsole2
cCoinMidPoint__11TGCConsole2:
	.skip 0x8
.global cCoinBotPoint__11TGCConsole2
cCoinBotPoint__11TGCConsole2:
	.skip 0x8
.global gpTalk2D
gpTalk2D:
	.skip 0x8
.global gpCardLoad
gpCardLoad:
	.skip 0x8
.global cShineGetRight1__11TConsoleStr
cShineGetRight1__11TConsoleStr:
	.skip 0x8
.global cShineGetLeft1__11TConsoleStr
cShineGetLeft1__11TConsoleStr:
	.skip 0x8
.global cShineGetRight2__11TConsoleStr
cShineGetRight2__11TConsoleStr:
	.skip 0x8
.global cShineGetLeft2__11TConsoleStr
cShineGetLeft2__11TConsoleStr:
	.skip 0x8
.global cShineGetRight3__11TConsoleStr
cShineGetRight3__11TConsoleStr:
	.skip 0x8
.global cShineGetLeft3__11TConsoleStr
cShineGetLeft3__11TConsoleStr:
	.skip 0x8
.global setup_wait
setup_wait:
	.skip 0x8
.global img_wx
img_wx:
	.skip 0x2
.global img_wy
img_wy:
	.skip 0x2
.global vtable
vtable:
	.skip 0x8
.global r$181
r$181:
	.skip 0x4
.global p1$182
p1$182:
	.skip 0x4
.global p2$183
p2$183:
	.skip 0x4
.global p3$184
p3$184:
	.skip 0x4
.global a1$185
a1$185:
	.skip 0x2
.global a2$186
a2$186:
	.skip 0x2
.global a3$187
a3$187:
	.skip 0x4
.global boke$188
boke$188:
	.skip 0x4
.global rot$260
rot$260:
	.skip 0x4
.global fade$261
fade$261:
	.skip 0x4
.global boundstate$263
boundstate$263:
	.skip 0x4
.global boundtimer$264
boundtimer$264:
	.skip 0x4
.global bounddelta$265
bounddelta$265:
	.skip 0x4
.global alpha$266
alpha$266:
	.skip 0x4
.global hxs_logo_resetflag
hxs_logo_resetflag:
	.skip 0x4
.global hxs_logodraw_resetflag
hxs_logodraw_resetflag:
	.skip 0x4
.global dp$320
dp$320:
	.skip 0x4
.global bx$321
bx$321:
	.skip 0x4
.global by$322
by$322:
	.skip 0x4
.global count$323
count$323:
	.skip 0x4
.global r$393
r$393:
	.skip 0x4
.global r$416
r$416:
	.skip 0x4
.global r$432
r$432:
	.skip 0x4
.global thin$469
thin$469:
	.skip 0x4
.global rstep$470
rstep$470:
	.skip 0x4
.global thin_d$471
thin_d$471:
	.skip 0x4
.global rstep_d$472
rstep_d$472:
	.skip 0x8
.global gpConductor
gpConductor:
	.skip 0x8
.global gpEffectObjManager
gpEffectObjManager:
	.skip 0x8
.global gpCurGesso
gpCurGesso:
	.skip 0x4
.global init$3222
init$3222:
	.skip 0x4
.global instance$3220
instance$3220:
	.skip 0x4
.global init$3230
init$3230:
	.skip 0x4
.global instance$3228
instance$3228:
	.skip 0x4
.global init$3265
init$3265:
	.skip 0x4
.global instance$3263
instance$3263:
	.skip 0x4
.global init$3275
init$3275:
	.skip 0x4
.global instance$3273
instance$3273:
	.skip 0x4
.global init$3295
init$3295:
	.skip 0x4
.global instance$3293
instance$3293:
	.skip 0x4
.global init$3358
init$3358:
	.skip 0x4
.global instance$3356
instance$3356:
	.skip 0x4
.global init$3372
init$3372:
	.skip 0x4
.global instance$3370
instance$3370:
	.skip 0x4
.global init$3382
init$3382:
	.skip 0x4
.global instance$3380
instance$3380:
	.skip 0x4
.global init$3392
init$3392:
	.skip 0x4
.global instance$3390
instance$3390:
	.skip 0x8
.global mBoundFly__13THaneHamuKuri
mBoundFly__13THaneHamuKuri:
	.skip 0x1
.global mAttackSw__14TDangoHamuKuri
mAttackSw__14TDangoHamuKuri:
	.skip 0x1
.global init$3758
init$3758:
	.skip 0x2
.global instance$3756
instance$3756:
	.skip 0x4
.global init$3772
init$3772:
	.skip 0x4
.global instance$3770
instance$3770:
	.skip 0x4
.global init$3789
init$3789:
	.skip 0x4
.global instance$3787
instance$3787:
	.skip 0x4
.global init$3801
init$3801:
	.skip 0x4
.global instance$3799
instance$3799:
	.skip 0x4
.global init$3811
init$3811:
	.skip 0x4
.global instance$3809
instance$3809:
	.skip 0x4
.global init$3829
init$3829:
	.skip 0x4
.global instance$3827
instance$3827:
	.skip 0x4
.global init$3845
init$3845:
	.skip 0x4
.global instance$3843
instance$3843:
	.skip 0x4
.global init$3856
init$3856:
	.skip 0x4
.global instance$3854
instance$3854:
	.skip 0x4
.global init$3876
init$3876:
	.skip 0x4
.global instance$3874
instance$3874:
	.skip 0x4
.global init$3890
init$3890:
	.skip 0x4
.global instance$3888
instance$3888:
	.skip 0x4
.global init$3901
init$3901:
	.skip 0x4
.global instance$3899
instance$3899:
	.skip 0x4
.global init$3911
init$3911:
	.skip 0x4
.global instance$3909
instance$3909:
	.skip 0x4
.global init$3924
init$3924:
	.skip 0x4
.global instance$3922
instance$3922:
	.skip 0x4
.global gpCurHinokuri
gpCurHinokuri:
	.skip 0x4
.global init$3387
init$3387:
	.skip 0x4
.global instance$3385
instance$3385:
	.skip 0x4
.global init$3399
init$3399:
	.skip 0x4
.global instance$3397
instance$3397:
	.skip 0x4
.global init$3436
init$3436:
	.skip 0x4
.global instance$3434
instance$3434:
	.skip 0x4
.global init$3452
init$3452:
	.skip 0x4
.global instance$3450
instance$3450:
	.skip 0x4
.global init$3463
init$3463:
	.skip 0x4
.global instance$3461
instance$3461:
	.skip 0x4
.global init$3476
init$3476:
	.skip 0x4
.global instance$3474
instance$3474:
	.skip 0x4
.global init$3495
init$3495:
	.skip 0x4
.global instance$3493
instance$3493:
	.skip 0x4
.global init$3512
init$3512:
	.skip 0x4
.global instance$3510
instance$3510:
	.skip 0x4
.global init$3542
init$3542:
	.skip 0x4
.global instance$3540
instance$3540:
	.skip 0x4
.global init$3571
init$3571:
	.skip 0x4
.global instance$3569
instance$3569:
	.skip 0x4
.global init$3585
init$3585:
	.skip 0x4
.global instance$3583
instance$3583:
	.skip 0x4
.global init$3595
init$3595:
	.skip 0x4
.global instance$3593
instance$3593:
	.skip 0x4
.global init$3607
init$3607:
	.skip 0x4
.global instance$3605
instance$3605:
	.skip 0x4
.global init$3625
init$3625:
	.skip 0x4
.global instance$3623
instance$3623:
	.skip 0x8
.global init$2574
init$2574:
	.skip 0x4
.global instance$2572
instance$2572:
	.skip 0x4
.global init$2632
init$2632:
	.skip 0x4
.global instance$2630
instance$2630:
	.skip 0x4
.global init$2655
init$2655:
	.skip 0x4
.global instance$2653
instance$2653:
	.skip 0x4
.global init$2675
init$2675:
	.skip 0x4
.global instance$2673
instance$2673:
	.skip 0x4
.global init$2695
init$2695:
	.skip 0x4
.global instance$2693
instance$2693:
	.skip 0x4
.global init$2709
init$2709:
	.skip 0x4
.global instance$2707
instance$2707:
	.skip 0x4
.global init$2727
init$2727:
	.skip 0x4
.global instance$2725
instance$2725:
	.skip 0x4
.global init$2754
init$2754:
	.skip 0x4
.global instance$2752
instance$2752:
	.skip 0x4
.global gpCurNameKuri
gpCurNameKuri:
	.skip 0x4
.global init$3070
init$3070:
	.skip 0x4
.global instance$3068
instance$3068:
	.skip 0x4
.global init$3083
init$3083:
	.skip 0x4
.global instance$3081
instance$3081:
	.skip 0x4
.global init$3128
init$3128:
	.skip 0x4
.global instance$3126
instance$3126:
	.skip 0x4
.global init$3147
init$3147:
	.skip 0x4
.global instance$3145
instance$3145:
	.skip 0x4
.global init$3159
init$3159:
	.skip 0x4
.global instance$3157
instance$3157:
	.skip 0x4
.global init$3167
init$3167:
	.skip 0x4
.global instance$3165
instance$3165:
	.skip 0x4
.global init$3173
init$3173:
	.skip 0x4
.global instance$3171
instance$3171:
	.skip 0x8
.global gpCurPakkun
gpCurPakkun:
	.skip 0x4
.global gpCurPakkunSeed
gpCurPakkunSeed:
	.skip 0x4
.global mHeadJntIndex__7TPakkun
mHeadJntIndex__7TPakkun:
	.skip 0x1
.global init$3234
init$3234:
	.skip 0x3
.global instance$3232
instance$3232:
	.skip 0x4
.global init$3254
init$3254:
	.skip 0x4
.global instance$3252
instance$3252:
	.skip 0x4
.global init$3297
init$3297:
	.skip 0x4
.global instance$3295
instance$3295:
	.skip 0x4
.global init$3310
init$3310:
	.skip 0x4
.global instance$3308
instance$3308:
	.skip 0x4
.global init$3322
init$3322:
	.skip 0x4
.global instance$3320
instance$3320:
	.skip 0x4
.global init$3337
init$3337:
	.skip 0x4
.global instance$3335
instance$3335:
	.skip 0x4
.global init$3358
init$3358:
	.skip 0x4
.global instance$3356
instance$3356:
	.skip 0x4
.global init$3388
init$3388:
	.skip 0x4
.global instance$3386
instance$3386:
	.skip 0x4
.global mTestJuiceType__18TSmallEnemyManager
mTestJuiceType__18TSmallEnemyManager:
	.skip 0x1
.global init$3225
init$3225:
	.skip 0x3
.global instance$3223
instance$3223:
	.skip 0x4
.global init$3253
init$3253:
	.skip 0x4
.global instance$3251
instance$3251:
	.skip 0x4
.global init$3264
init$3264:
	.skip 0x4
.global instance$3262
instance$3262:
	.skip 0x4
.global init$3278
init$3278:
	.skip 0x4
.global instance$3276
instance$3276:
	.skip 0x4
.global init$3295
init$3295:
	.skip 0x4
.global instance$3293
instance$3293:
	.skip 0x4
.global init$3307
init$3307:
	.skip 0x4
.global instance$3305
instance$3305:
	.skip 0x4
.global init$2168
init$2168:
	.skip 0x4
.global instance$2166
instance$2166:
	.skip 0x4
.global init$2616
init$2616:
	.skip 0x4
.global instance$2614
instance$2614:
	.skip 0x4
.global init$2627
init$2627:
	.skip 0x4
.global instance$2625
instance$2625:
	.skip 0x4
.global init$2639
init$2639:
	.skip 0x4
.global instance$2637
instance$2637:
	.skip 0x4
.global init$2658
init$2658:
	.skip 0x4
.global instance$2656
instance$2656:
	.skip 0x4
.global init$2669
init$2669:
	.skip 0x4
.global instance$2667
instance$2667:
	.skip 0x4
.global init$2683
init$2683:
	.skip 0x4
.global instance$2681
instance$2681:
	.skip 0x4
.global init$3720
init$3720:
	.skip 0x4
.global instance$3718
instance$3718:
	.skip 0x4
.global init$3738
init$3738:
	.skip 0x4
.global instance$3736
instance$3736:
	.skip 0x4
.global init$3753
init$3753:
	.skip 0x4
.global instance$3751
instance$3751:
	.skip 0x4
.global init$3784
init$3784:
	.skip 0x4
.global instance$3782
instance$3782:
	.skip 0x4
.global init$3804
init$3804:
	.skip 0x4
.global instance$3802
instance$3802:
	.skip 0x4
.global init$3823
init$3823:
	.skip 0x4
.global instance$3821
instance$3821:
	.skip 0x4
.global init$3870
init$3870:
	.skip 0x4
.global instance$3868
instance$3868:
	.skip 0x4
.global init$3886
init$3886:
	.skip 0x4
.global instance$3884
instance$3884:
	.skip 0x4
.global init$3903
init$3903:
	.skip 0x4
.global instance$3901
instance$3901:
	.skip 0x4
.global init$3160
init$3160:
	.skip 0x4
.global instance$3158
instance$3158:
	.skip 0x4
.global init$3183
init$3183:
	.skip 0x4
.global instance$3181
instance$3181:
	.skip 0x4
.global init$3209
init$3209:
	.skip 0x4
.global instance$3207
instance$3207:
	.skip 0x4
.global init$3234
init$3234:
	.skip 0x4
.global instance$3232
instance$3232:
	.skip 0x4
.global init$3264
init$3264:
	.skip 0x4
.global instance$3262
instance$3262:
	.skip 0x4
.global init$3276
init$3276:
	.skip 0x4
.global instance$3274
instance$3274:
	.skip 0x4
.global init$3287
init$3287:
	.skip 0x4
.global instance$3285
instance$3285:
	.skip 0x4
.global init$3306
init$3306:
	.skip 0x4
.global instance$3304
instance$3304:
	.skip 0x4
.global init$3314
init$3314:
	.skip 0x4
.global instance$3312
instance$3312:
	.skip 0x4
.global init$3308
init$3308:
	.skip 0x4
.global instance$3306
instance$3306:
	.skip 0x4
.global init$3334
init$3334:
	.skip 0x4
.global instance$3332
instance$3332:
	.skip 0x4
.global init$3355
init$3355:
	.skip 0x4
.global instance$3353
instance$3353:
	.skip 0x4
.global init$3376
init$3376:
	.skip 0x4
.global instance$3374
instance$3374:
	.skip 0x4
.global init$3388
init$3388:
	.skip 0x4
.global instance$3386
instance$3386:
	.skip 0x4
.global init$3406
init$3406:
	.skip 0x4
.global instance$3404
instance$3404:
	.skip 0x4
.global init$3485
init$3485:
	.skip 0x4
.global instance$3483
instance$3483:
	.skip 0x4
.global init$3499
init$3499:
	.skip 0x4
.global instance$3497
instance$3497:
	.skip 0x4
.global init$3511
init$3511:
	.skip 0x4
.global instance$3509
instance$3509:
	.skip 0x4
.global init$3521
init$3521:
	.skip 0x4
.global instance$3519
instance$3519:
	.skip 0x4
.global init$3534
init$3534:
	.skip 0x4
.global instance$3532
instance$3532:
	.skip 0x4
.global init$3549
init$3549:
	.skip 0x4
.global instance$3547
instance$3547:
	.skip 0x4
.global init$3565
init$3565:
	.skip 0x4
.global instance$3563
instance$3563:
	.skip 0x4
.global init$3575
init$3575:
	.skip 0x4
.global instance$3573
instance$3573:
	.skip 0x4
.global init$3584
init$3584:
	.skip 0x4
.global instance$3582
instance$3582:
	.skip 0x4
.global init$3594
init$3594:
	.skip 0x4
.global instance$3592
instance$3592:
	.skip 0x4
.global init$3609
init$3609:
	.skip 0x4
.global instance$3607
instance$3607:
	.skip 0x4
.global init$3620
init$3620:
	.skip 0x4
.global instance$3618
instance$3618:
	.skip 0x4
.global init$3231
init$3231:
	.skip 0x1
.global init$3277
init$3277:
	.skip 0x1
.global init$3330
init$3330:
	.skip 0x1
.global init$3333
init$3333:
	.skip 0x1
.global init$3337
init$3337:
	.skip 0x1
.global init$3340
init$3340:
	.skip 0x1
.global init$3586
init$3586:
	.skip 0x1
.global init$3635
init$3635:
	.skip 0x1
.global instance$3633
instance$3633:
	.skip 0x4
.global init$3675
init$3675:
	.skip 0x4
.global instance$3673
instance$3673:
	.skip 0x4
.global init$3688
init$3688:
	.skip 0x4
.global instance$3686
instance$3686:
	.skip 0x4
.global init$3717
init$3717:
	.skip 0x4
.global instance$3715
instance$3715:
	.skip 0x4
.global init$3729
init$3729:
	.skip 0x4
.global instance$3727
instance$3727:
	.skip 0x4
.global init$3742
init$3742:
	.skip 0x4
.global instance$3740
instance$3740:
	.skip 0x4
.global init$3755
init$3755:
	.skip 0x4
.global instance$3753
instance$3753:
	.skip 0x4
.global init$3769
init$3769:
	.skip 0x4
.global instance$3767
instance$3767:
	.skip 0x4
.global init$3781
init$3781:
	.skip 0x4
.global instance$3779
instance$3779:
	.skip 0x4
.global init$3805
init$3805:
	.skip 0x4
.global instance$3803
instance$3803:
	.skip 0x4
.global init$3818
init$3818:
	.skip 0x4
.global instance$3816
instance$3816:
	.skip 0x4
.global init$3830
init$3830:
	.skip 0x4
.global instance$3828
instance$3828:
	.skip 0x4
.global init$3844
init$3844:
	.skip 0x4
.global instance$3842
instance$3842:
	.skip 0x4
.global init$3854
init$3854:
	.skip 0x4
.global instance$3852
instance$3852:
	.skip 0x4
.global init$3864
init$3864:
	.skip 0x4
.global instance$3862
instance$3862:
	.skip 0x4
.global init$3881
init$3881:
	.skip 0x4
.global instance$3879
instance$3879:
	.skip 0x4
.global init$3899
init$3899:
	.skip 0x4
.global instance$3897
instance$3897:
	.skip 0x4
.global init$3912
init$3912:
	.skip 0x4
.global instance$3910
instance$3910:
	.skip 0x4
.global init$3928
init$3928:
	.skip 0x4
.global instance$3926
instance$3926:
	.skip 0x4
.global init$3940
init$3940:
	.skip 0x4
.global instance$3938
instance$3938:
	.skip 0x4
.global init$3950
init$3950:
	.skip 0x4
.global instance$3948
instance$3948:
	.skip 0x4
.global init$3964
init$3964:
	.skip 0x4
.global instance$3962
instance$3962:
	.skip 0x4
.global init$4001
init$4001:
	.skip 0x4
.global instance$3999
instance$3999:
	.skip 0x4
.global init$4009
init$4009:
	.skip 0x4
.global instance$4007
instance$4007:
	.skip 0x4
.global init$4019
init$4019:
	.skip 0x4
.global instance$4017
instance$4017:
	.skip 0x4
.global init$4035
init$4035:
	.skip 0x4
.global instance$4033
instance$4033:
	.skip 0x8
.global gpCurTobiPuku
gpCurTobiPuku:
	.skip 0x4
.global init$3085
init$3085:
	.skip 0x4
.global instance$3083
instance$3083:
	.skip 0x4
.global init$3097
init$3097:
	.skip 0x4
.global instance$3095
instance$3095:
	.skip 0x4
.global init$3115
init$3115:
	.skip 0x4
.global instance$3113
instance$3113:
	.skip 0x4
.global init$3130
init$3130:
	.skip 0x4
.global instance$3128
instance$3128:
	.skip 0x4
.global init$3155
init$3155:
	.skip 0x4
.global instance$3153
instance$3153:
	.skip 0x4
.global init$3168
init$3168:
	.skip 0x4
.global instance$3166
instance$3166:
	.skip 0x4
.global init$3180
init$3180:
	.skip 0x4
.global instance$3178
instance$3178:
	.skip 0x4
.global init$3197
init$3197:
	.skip 0x4
.global instance$3195
instance$3195:
	.skip 0x4
.global init$3235
init$3235:
	.skip 0x4
.global instance$3233
instance$3233:
	.skip 0x4
.global init$3248
init$3248:
	.skip 0x4
.global instance$3246
instance$3246:
	.skip 0x4
.global init$3270
init$3270:
	.skip 0x4
.global instance$3268
instance$3268:
	.skip 0x4
.global init$3282
init$3282:
	.skip 0x4
.global instance$3280
instance$3280:
	.skip 0x8
.global init$3540
init$3540:
	.skip 0x4
.global instance$3538
instance$3538:
	.skip 0x4
.global init$3552
init$3552:
	.skip 0x4
.global instance$3550
instance$3550:
	.skip 0x4
.global init$3565
init$3565:
	.skip 0x4
.global instance$3563
instance$3563:
	.skip 0x4
.global init$localstatic1$theNerve__31TNerveWaitForever$$010TLiveActor$$1Fv
init$localstatic1$theNerve__31TNerveWaitForever$$010TLiveActor$$1Fv:
	.skip 0x4
.global instance$localstatic0$theNerve__31TNerveWaitForever$$010TLiveActor$$1Fv
instance$localstatic0$theNerve__31TNerveWaitForever$$010TLiveActor$$1Fv:
	.skip 0x4
.global init$3490
init$3490:
	.skip 0x4
.global instance$3488
instance$3488:
	.skip 0x4
.global init$3537
init$3537:
	.skip 0x4
.global instance$3535
instance$3535:
	.skip 0x4
.global init$3550
init$3550:
	.skip 0x4
.global instance$3548
instance$3548:
	.skip 0x4
.global init$3564
init$3564:
	.skip 0x4
.global instance$3562
instance$3562:
	.skip 0x4
.global init$3575
init$3575:
	.skip 0x4
.global instance$3573
instance$3573:
	.skip 0x4
.global init$3592
init$3592:
	.skip 0x4
.global instance$3590
instance$3590:
	.skip 0x4
.global init$3602
init$3602:
	.skip 0x4
.global instance$3600
instance$3600:
	.skip 0x4
.global init$3621
init$3621:
	.skip 0x4
.global instance$3619
instance$3619:
	.skip 0x4
.global init$3640
init$3640:
	.skip 0x4
.global instance$3638
instance$3638:
	.skip 0x4
.global init$3654
init$3654:
	.skip 0x4
.global instance$3652
instance$3652:
	.skip 0x4
.global init$3664
init$3664:
	.skip 0x4
.global instance$3662
instance$3662:
	.skip 0x4
.global gpCurChuuHana
gpCurChuuHana:
	.skip 0x4
.global init$3083
init$3083:
	.skip 0x4
.global instance$3081
instance$3081:
	.skip 0x4
.global init$3102
init$3102:
	.skip 0x4
.global instance$3100
instance$3100:
	.skip 0x4
.global init$3121
init$3121:
	.skip 0x4
.global instance$3119
instance$3119:
	.skip 0x4
.global init$3149
init$3149:
	.skip 0x4
.global instance$3147
instance$3147:
	.skip 0x4
.global init$3173
init$3173:
	.skip 0x4
.global instance$3171
instance$3171:
	.skip 0x4
.global init$3188
init$3188:
	.skip 0x4
.global instance$3186
instance$3186:
	.skip 0x4
.global init$3194
init$3194:
	.skip 0x4
.global instance$3192
instance$3192:
	.skip 0x4
.global init$3205
init$3205:
	.skip 0x4
.global instance$3203
instance$3203:
	.skip 0x4
.global init$3211
init$3211:
	.skip 0x4
.global instance$3209
instance$3209:
	.skip 0x4
.global init$3227
init$3227:
	.skip 0x4
.global instance$3225
instance$3225:
	.skip 0x4
.global init$3240
init$3240:
	.skip 0x4
.global instance$3238
instance$3238:
	.skip 0x8
.global gpCurRollEnemy
gpCurRollEnemy:
	.skip 0x4
.global mTransYOffset__10TRollEnemy
mTransYOffset__10TRollEnemy:
	.skip 0x4
.global init$3216
init$3216:
	.skip 0x4
.global instance$3214
instance$3214:
	.skip 0x4
.global init$3224
init$3224:
	.skip 0x4
.global instance$3222
instance$3222:
	.skip 0x4
.global init$3242
init$3242:
	.skip 0x4
.global instance$3240
instance$3240:
	.skip 0x4
.global init$3466
init$3466:
	.skip 0x4
.global instance$3464
instance$3464:
	.skip 0x4
.global init$3479
init$3479:
	.skip 0x4
.global instance$3477
instance$3477:
	.skip 0x4
.global mBodyJntIndex__8TPoiHana
mBodyJntIndex__8TPoiHana:
	.skip 0x1
.global init$3100
init$3100:
	.skip 0x3
.global instance$3098
instance$3098:
	.skip 0x4
.global init$3137
init$3137:
	.skip 0x4
.global instance$3135
instance$3135:
	.skip 0x4
.global init$3163
init$3163:
	.skip 0x4
.global instance$3161
instance$3161:
	.skip 0x4
.global init$3183
init$3183:
	.skip 0x4
.global instance$3181
instance$3181:
	.skip 0x4
.global init$3344
init$3344:
	.skip 0x4
.global instance$3342
instance$3342:
	.skip 0x4
.global init$3353
init$3353:
	.skip 0x4
.global instance$3351
instance$3351:
	.skip 0x4
.global init$3416
init$3416:
	.skip 0x4
.global instance$3414
instance$3414:
	.skip 0x4
.global init$3434
init$3434:
	.skip 0x4
.global instance$3432
instance$3432:
	.skip 0x4
.global init$3448
init$3448:
	.skip 0x4
.global instance$3446
instance$3446:
	.skip 0x4
.global init$3458
init$3458:
	.skip 0x4
.global instance$3456
instance$3456:
	.skip 0x4
.global init$3482
init$3482:
	.skip 0x4
.global instance$3480
instance$3480:
	.skip 0x4
.global init$3509
init$3509:
	.skip 0x4
.global instance$3507
instance$3507:
	.skip 0x4
.global init$3208
init$3208:
	.skip 0x4
.global instance$3206
instance$3206:
	.skip 0x4
.global init$3260
init$3260:
	.skip 0x4
.global instance$3258
instance$3258:
	.skip 0x4
.global init$4033
init$4033:
	.skip 0x4
.global instance$4031
instance$4031:
	.skip 0x4
.global init$4075
init$4075:
	.skip 0x4
.global instance$4073
instance$4073:
	.skip 0x4
.global init$4089
init$4089:
	.skip 0x4
.global instance$4087
instance$4087:
	.skip 0x4
.global init$4099
init$4099:
	.skip 0x4
.global instance$4097
instance$4097:
	.skip 0x4
.global init$4112
init$4112:
	.skip 0x4
.global instance$4110
instance$4110:
	.skip 0x4
.global init$4134
init$4134:
	.skip 0x4
.global instance$4132
instance$4132:
	.skip 0x4
.global init$4150
init$4150:
	.skip 0x4
.global instance$4148
instance$4148:
	.skip 0x4
.global init$4166
init$4166:
	.skip 0x4
.global instance$4164
instance$4164:
	.skip 0x4
.global init$4201
init$4201:
	.skip 0x4
.global instance$4199
instance$4199:
	.skip 0x4
.global init$4212
init$4212:
	.skip 0x4
.global instance$4210
instance$4210:
	.skip 0x4
.global init$2176
init$2176:
	.skip 0x4
.global instance$2174
instance$2174:
	.skip 0x4
.global init$3010
init$3010:
	.skip 0x4
.global instance$3008
instance$3008:
	.skip 0x4
.global init$3031
init$3031:
	.skip 0x4
.global instance$3029
instance$3029:
	.skip 0x4
.global init$3040
init$3040:
	.skip 0x4
.global instance$3038
instance$3038:
	.skip 0x4
.global init$3053
init$3053:
	.skip 0x4
.global instance$3051
instance$3051:
	.skip 0x4
.global init$3077
init$3077:
	.skip 0x4
.global instance$3075
instance$3075:
	.skip 0x4
.global init$3087
init$3087:
	.skip 0x4
.global instance$3085
instance$3085:
	.skip 0x4
.global init$3099
init$3099:
	.skip 0x4
.global instance$3097
instance$3097:
	.skip 0x4
.global init$3283
init$3283:
	.skip 0x4
.global instance$3281
instance$3281:
	.skip 0x4
.global init$3296
init$3296:
	.skip 0x4
.global instance$3294
instance$3294:
	.skip 0x4
.global init$3333
init$3333:
	.skip 0x4
.global instance$3331
instance$3331:
	.skip 0x4
.global init$3366
init$3366:
	.skip 0x4
.global instance$3364
instance$3364:
	.skip 0x4
.global init$3390
init$3390:
	.skip 0x4
.global instance$3388
instance$3388:
	.skip 0x4
.global init$3410
init$3410:
	.skip 0x4
.global instance$3408
instance$3408:
	.skip 0x4
.global init$3446
init$3446:
	.skip 0x4
.global instance$3444
instance$3444:
	.skip 0x4
.global init$3462
init$3462:
	.skip 0x4
.global instance$3460
instance$3460:
	.skip 0x4
.global mUseMapCollision__8TBossEel
mUseMapCollision__8TBossEel:
	.skip 0x1
.global init$3225
init$3225:
	.skip 0x3
.global instance$3223
instance$3223:
	.skip 0x4
.global init$3237
init$3237:
	.skip 0x4
.global instance$3235
instance$3235:
	.skip 0x4
.global init$3245
init$3245:
	.skip 0x4
.global instance$3243
instance$3243:
	.skip 0x4
.global init$3262
init$3262:
	.skip 0x4
.global instance$3260
instance$3260:
	.skip 0x4
.global init$3279
init$3279:
	.skip 0x4
.global instance$3277
instance$3277:
	.skip 0x4
.global init$3304
init$3304:
	.skip 0x4
.global instance$3302
instance$3302:
	.skip 0x4
.global init$3782
init$3782:
	.skip 0x4
.global instance$3780
instance$3780:
	.skip 0x4
.global init$3811
init$3811:
	.skip 0x4
.global instance$3809
instance$3809:
	.skip 0x4
.global init$3824
init$3824:
	.skip 0x4
.global instance$3822
instance$3822:
	.skip 0x4
.global init$3837
init$3837:
	.skip 0x4
.global instance$3835
instance$3835:
	.skip 0x4
.global testHeight
testHeight:
	.skip 0x4
.global init$3855
init$3855:
	.skip 0x4
.global instance$3853
instance$3853:
	.skip 0x4
.global init$3891
init$3891:
	.skip 0x4
.global instance$3889
instance$3889:
	.skip 0x4
.global init$3897
init$3897:
	.skip 0x4
.global instance$3895
instance$3895:
	.skip 0x4
.global init$3905
init$3905:
	.skip 0x4
.global instance$3903
instance$3903:
	.skip 0x4
.global init$3936
init$3936:
	.skip 0x4
.global instance$3934
instance$3934:
	.skip 0x4
.global init$3955
init$3955:
	.skip 0x4
.global instance$3953
instance$3953:
	.skip 0x4
.global init$3982
init$3982:
	.skip 0x4
.global instance$3980
instance$3980:
	.skip 0x8
.global gpCurKiller
gpCurKiller:
	.skip 0x4
.global mRollSw__7TKiller
mRollSw__7TKiller:
	.skip 0x1
.global init$2972
init$2972:
	.skip 0x3
.global instance$2970
instance$2970:
	.skip 0x4
.global init$3001
init$3001:
	.skip 0x4
.global instance$2999
instance$2999:
	.skip 0x4
.global init$3154
init$3154:
	.skip 0x4
.global instance$3152
instance$3152:
	.skip 0x8
.global gpBeamManager
gpBeamManager:
	.skip 0x8
.global mBodyJntIndex__10TSamboHead
mBodyJntIndex__10TSamboHead:
	.skip 0x4
.global gpCurSamboHead
gpCurSamboHead:
	.skip 0x4
.global init$2966
init$2966:
	.skip 0x4
.global instance$2964
instance$2964:
	.skip 0x4
.global init$2980
init$2980:
	.skip 0x4
.global instance$2978
instance$2978:
	.skip 0x4
.global init$2998
init$2998:
	.skip 0x4
.global instance$2996
instance$2996:
	.skip 0x4
.global init$3019
init$3019:
	.skip 0x4
.global instance$3017
instance$3017:
	.skip 0x4
.global init$3033
init$3033:
	.skip 0x4
.global instance$3031
instance$3031:
	.skip 0x4
.global init$3055
init$3055:
	.skip 0x4
.global instance$3053
instance$3053:
	.skip 0x4
.global init$3153
init$3153:
	.skip 0x4
.global instance$3151
instance$3151:
	.skip 0x4
.global init$3168
init$3168:
	.skip 0x4
.global instance$3166
instance$3166:
	.skip 0x4
.global init$3209
init$3209:
	.skip 0x4
.global instance$3207
instance$3207:
	.skip 0x4
.global init$3225
init$3225:
	.skip 0x4
.global instance$3223
instance$3223:
	.skip 0x4
.global init$3254
init$3254:
	.skip 0x4
.global instance$3252
instance$3252:
	.skip 0x4
.global init$3265
init$3265:
	.skip 0x4
.global instance$3263
instance$3263:
	.skip 0x4
.global gpCurPopo
gpCurPopo:
	.skip 0x4
.global mTestAng_z__5TPopo
mTestAng_z__5TPopo:
	.skip 0x4
.global mExplosionSw__5TPopo
mExplosionSw__5TPopo:
	.skip 0x1
.global init$3188
init$3188:
	.skip 0x3
.global instance$3186
instance$3186:
	.skip 0x4
.global init$3205
init$3205:
	.skip 0x4
.global instance$3203
instance$3203:
	.skip 0x4
.global init$3224
init$3224:
	.skip 0x4
.global instance$3222
instance$3222:
	.skip 0x4
.global init$3249
init$3249:
	.skip 0x4
.global instance$3247
instance$3247:
	.skip 0x4
.global init$3259
init$3259:
	.skip 0x4
.global instance$3257
instance$3257:
	.skip 0x4
.global init$3268
init$3268:
	.skip 0x4
.global instance$3266
instance$3266:
	.skip 0x4
.global init$2266
init$2266:
	.skip 0x4
.global instance$2264
instance$2264:
	.skip 0x4
.global init$2272
init$2272:
	.skip 0x4
.global instance$2270
instance$2270:
	.skip 0x4
.global init$2719
init$2719:
	.skip 0x1
.global init$2733
init$2733:
	.skip 0x1
.global init$2774
init$2774:
	.skip 0x2
.global instance$2772
instance$2772:
	.skip 0x4
.global init$2796
init$2796:
	.skip 0x4
.global instance$2794
instance$2794:
	.skip 0x4
.global sCommonSaveFileName$2709
sCommonSaveFileName$2709:
	.skip 0x4
.global init$2710
init$2710:
	.skip 0x1
.global init$2728
init$2728:
	.skip 0x3
.global init$2361
init$2361:
	.skip 0x4
.global instance$2359
instance$2359:
	.skip 0x4
.global init$2373
init$2373:
	.skip 0x4
.global instance$2371
instance$2371:
	.skip 0x4
.global init$2385
init$2385:
	.skip 0x4
.global instance$2383
instance$2383:
	.skip 0x4
.global init$2393
init$2393:
	.skip 0x4
.global instance$2391
instance$2391:
	.skip 0x4
.global init$2401
init$2401:
	.skip 0x4
.global instance$2399
instance$2399:
	.skip 0x4
.global init$2410
init$2410:
	.skip 0x4
.global instance$2408
instance$2408:
	.skip 0x4
.global init$2425
init$2425:
	.skip 0x4
.global instance$2423
instance$2423:
	.skip 0x4
.global init$2979
init$2979:
	.skip 0x4
.global instance$2977
instance$2977:
	.skip 0x4
.global init$3009
init$3009:
	.skip 0x4
.global instance$3007
instance$3007:
	.skip 0x4
.global init$3058
init$3058:
	.skip 0x4
.global instance$3056
instance$3056:
	.skip 0x4
.global init$3083
init$3083:
	.skip 0x4
.global instance$3081
instance$3081:
	.skip 0x4
.global init$3261
init$3261:
	.skip 0x4
.global instance$3259
instance$3259:
	.skip 0x4
.global init$3283
init$3283:
	.skip 0x4
.global instance$3281
instance$3281:
	.skip 0x4
.global init$3302
init$3302:
	.skip 0x4
.global instance$3300
instance$3300:
	.skip 0x4
.global init$3363
init$3363:
	.skip 0x4
.global instance$3361
instance$3361:
	.skip 0x4
.global init$3399
init$3399:
	.skip 0x4
.global instance$3397
instance$3397:
	.skip 0x4
.global init$3413
init$3413:
	.skip 0x4
.global instance$3411
instance$3411:
	.skip 0x4
.global init$3426
init$3426:
	.skip 0x4
.global instance$3424
instance$3424:
	.skip 0x4
.global init$3454
init$3454:
	.skip 0x4
.global instance$3452
instance$3452:
	.skip 0x4
.global init$3468
init$3468:
	.skip 0x4
.global instance$3466
instance$3466:
	.skip 0x4
.global init$3482
init$3482:
	.skip 0x4
.global instance$3480
instance$3480:
	.skip 0x4
.global init$2840
init$2840:
	.skip 0x4
.global instance$2838
instance$2838:
	.skip 0x4
.global init$2858
init$2858:
	.skip 0x4
.global instance$2856
instance$2856:
	.skip 0x4
.global init$2874
init$2874:
	.skip 0x4
.global instance$2872
instance$2872:
	.skip 0x4
.global gpCurHauntLeg
gpCurHauntLeg:
	.skip 0x4
.global init$2859
init$2859:
	.skip 0x4
.global instance$2857
instance$2857:
	.skip 0x8
.global init$2673
init$2673:
	.skip 0x4
.global instance$2671
instance$2671:
	.skip 0x4
.global init$2685
init$2685:
	.skip 0x4
.global instance$2683
instance$2683:
	.skip 0x4
.global init$2693
init$2693:
	.skip 0x4
.global instance$2691
instance$2691:
	.skip 0x4
.global init$2704
init$2704:
	.skip 0x4
.global instance$2702
instance$2702:
	.skip 0x4
.global init$2712
init$2712:
	.skip 0x4
.global instance$2710
instance$2710:
	.skip 0x4
.global init$2722
init$2722:
	.skip 0x4
.global instance$2720
instance$2720:
	.skip 0x4
.global init$2730
init$2730:
	.skip 0x4
.global instance$2728
instance$2728:
	.skip 0x4
.global mTestAng_x__7TRocket
mTestAng_x__7TRocket:
	.skip 0x4
.global mTestAng_z__7TRocket
mTestAng_z__7TRocket:
	.skip 0x4
.global init$2897
init$2897:
	.skip 0x4
.global instance$2895
instance$2895:
	.skip 0x4
.global init$2911
init$2911:
	.skip 0x4
.global instance$2909
instance$2909:
	.skip 0x4
.global init$2933
init$2933:
	.skip 0x4
.global instance$2931
instance$2931:
	.skip 0x4
.global init$2842
init$2842:
	.skip 0x4
.global instance$2840
instance$2840:
	.skip 0x4
.global init$2854
init$2854:
	.skip 0x4
.global instance$2852
instance$2852:
	.skip 0x4
.global init$2864
init$2864:
	.skip 0x4
.global instance$2862
instance$2862:
	.skip 0x4
.global init$2876
init$2876:
	.skip 0x4
.global instance$2874
instance$2874:
	.skip 0x4
.global init$2889
init$2889:
	.skip 0x4
.global instance$2887
instance$2887:
	.skip 0x4
.global init$2900
init$2900:
	.skip 0x4
.global instance$2898
instance$2898:
	.skip 0x4
.global init$2911
init$2911:
	.skip 0x4
.global instance$2909
instance$2909:
	.skip 0x4
.global init$2921
init$2921:
	.skip 0x4
.global instance$2919
instance$2919:
	.skip 0x4
.global init$2920
init$2920:
	.skip 0x4
.global instance$2918
instance$2918:
	.skip 0x4
.global init$2926
init$2926:
	.skip 0x4
.global instance$2924
instance$2924:
	.skip 0x4
.global init$2932
init$2932:
	.skip 0x4
.global instance$2930
instance$2930:
	.skip 0x4
.global init$2938
init$2938:
	.skip 0x4
.global instance$2936
instance$2936:
	.skip 0x4
.global init$2944
init$2944:
	.skip 0x4
.global instance$2942
instance$2942:
	.skip 0x4
.global sCenterJointIndex__10TBossManta
sCenterJointIndex__10TBossManta:
	.skip 0x4
.global sBodyJointIndex__10TBossManta
sBodyJointIndex__10TBossManta:
	.skip 0x4
.global sRwingJointIndex__10TBossManta
sRwingJointIndex__10TBossManta:
	.skip 0x4
.global sLwingJointIndex__10TBossManta
sLwingJointIndex__10TBossManta:
	.skip 0x4
.global sEscapeFromMario__10TBossManta
sEscapeFromMario__10TBossManta:
	.skip 0x4
.global sDefeatSE$3156
sDefeatSE$3156:
	.skip 0x4
.global init$2877
init$2877:
	.skip 0x4
.global instance$2875
instance$2875:
	.skip 0x4
.global init$2889
init$2889:
	.skip 0x4
.global instance$2887
instance$2887:
	.skip 0x4
.global init$2902
init$2902:
	.skip 0x4
.global instance$2900
instance$2900:
	.skip 0x4
.global init$2912
init$2912:
	.skip 0x4
.global instance$2910
instance$2910:
	.skip 0x4
.global init$2924
init$2924:
	.skip 0x4
.global instance$2922
instance$2922:
	.skip 0x4
.global init$3120
init$3120:
	.skip 0x4
.global instance$3118
instance$3118:
	.skip 0x4
.global init$3128
init$3128:
	.skip 0x4
.global instance$3126
instance$3126:
	.skip 0x4
.global init$3140
init$3140:
	.skip 0x4
.global instance$3138
instance$3138:
	.skip 0x4
.global init$3148
init$3148:
	.skip 0x4
.global instance$3146
instance$3146:
	.skip 0x4
.global init$3158
init$3158:
	.skip 0x4
.global instance$3156
instance$3156:
	.skip 0x4
.global init$3322
init$3322:
	.skip 0x4
.global instance$3320
instance$3320:
	.skip 0x4
.global init$3330
init$3330:
	.skip 0x4
.global instance$3328
instance$3328:
	.skip 0x4
.global init$3340
init$3340:
	.skip 0x4
.global instance$3338
instance$3338:
	.skip 0x4
.global init$2664
init$2664:
	.skip 0x4
.global instance$2662
instance$2662:
	.skip 0x4
.global init$2676
init$2676:
	.skip 0x4
.global instance$2674
instance$2674:
	.skip 0x4
.global init$2688
init$2688:
	.skip 0x4
.global instance$2686
instance$2686:
	.skip 0x4
.global init$2698
init$2698:
	.skip 0x4
.global instance$2696
instance$2696:
	.skip 0x4
.global init$2715
init$2715:
	.skip 0x4
.global instance$2713
instance$2713:
	.skip 0x4
.global init$2728
init$2728:
	.skip 0x4
.global instance$2726
instance$2726:
	.skip 0x4
.global init$2745
init$2745:
	.skip 0x4
.global instance$2743
instance$2743:
	.skip 0x4
.global init$2755
init$2755:
	.skip 0x4
.global instance$2753
instance$2753:
	.skip 0x4
.global init$2766
init$2766:
	.skip 0x4
.global instance$2764
instance$2764:
	.skip 0x4
.global init$localstatic1$theNerve__16TNerveKoopaTurnRFv
init$localstatic1$theNerve__16TNerveKoopaTurnRFv:
	.skip 0x4
.global nerve$localstatic0$theNerve__16TNerveKoopaTurnRFv
nerve$localstatic0$theNerve__16TNerveKoopaTurnRFv:
	.skip 0x4
.global init$localstatic1$theNerve__16TNerveKoopaTurnLFv
init$localstatic1$theNerve__16TNerveKoopaTurnLFv:
	.skip 0x4
.global nerve$localstatic0$theNerve__16TNerveKoopaTurnLFv
nerve$localstatic0$theNerve__16TNerveKoopaTurnLFv:
	.skip 0x4
.global init$localstatic1$theNerve__17TNerveKoopaTumbleFv
init$localstatic1$theNerve__17TNerveKoopaTumbleFv:
	.skip 0x4
.global nerve$localstatic0$theNerve__17TNerveKoopaTumbleFv
nerve$localstatic0$theNerve__17TNerveKoopaTumbleFv:
	.skip 0x4
.global init$localstatic1$theNerve__18TNerveKoopaProvokeFv
init$localstatic1$theNerve__18TNerveKoopaProvokeFv:
	.skip 0x4
.global nerve$localstatic0$theNerve__18TNerveKoopaProvokeFv
nerve$localstatic0$theNerve__18TNerveKoopaProvokeFv:
	.skip 0x4
.global init$localstatic1$theNerve__15TNerveKoopaWaitFv
init$localstatic1$theNerve__15TNerveKoopaWaitFv:
	.skip 0x4
.global nerve$localstatic0$theNerve__15TNerveKoopaWaitFv
nerve$localstatic0$theNerve__15TNerveKoopaWaitFv:
	.skip 0x4
.global init$localstatic1$theNerve__16TNerveKoopaFlameFv
init$localstatic1$theNerve__16TNerveKoopaFlameFv:
	.skip 0x4
.global nerve$localstatic0$theNerve__16TNerveKoopaFlameFv
nerve$localstatic0$theNerve__16TNerveKoopaFlameFv:
	.skip 0x4
.global init$localstatic1$theNerve__18TNerveKoopaGetDownFv
init$localstatic1$theNerve__18TNerveKoopaGetDownFv:
	.skip 0x4
.global nerve$localstatic0$theNerve__18TNerveKoopaGetDownFv
nerve$localstatic0$theNerve__18TNerveKoopaGetDownFv:
	.skip 0x4
.global init$localstatic1$theNerve__18TNerveKoopaStaggerFv
init$localstatic1$theNerve__18TNerveKoopaStaggerFv:
	.skip 0x4
.global nerve$localstatic0$theNerve__18TNerveKoopaStaggerFv
nerve$localstatic0$theNerve__18TNerveKoopaStaggerFv:
	.skip 0x4
.global init$localstatic1$theNerve__15TNerveKoopaFallFv
init$localstatic1$theNerve__15TNerveKoopaFallFv:
	.skip 0x4
.global nerve$localstatic0$theNerve__15TNerveKoopaFallFv
nerve$localstatic0$theNerve__15TNerveKoopaFallFv:
	.skip 0x4
.global init$localstatic1$theNerve__22TNerveKoopaGetShoweredFv
init$localstatic1$theNerve__22TNerveKoopaGetShoweredFv:
	.skip 0x4
.global nerve$localstatic0$theNerve__22TNerveKoopaGetShoweredFv
nerve$localstatic0$theNerve__22TNerveKoopaGetShoweredFv:
	.skip 0x4
.global bathtub$3040
bathtub$3040:
	.skip 0x4
.global init$3041
init$3041:
	.skip 0x4
.global bathtub$3100
bathtub$3100:
	.skip 0x4
.global init$3101
init$3101:
	.skip 0x4
.global init$3102
init$3102:
	.skip 0x4
.global instance$3100
instance$3100:
	.skip 0x4
.global init$3129
init$3129:
	.skip 0x4
.global instance$3127
instance$3127:
	.skip 0x4
.global init$3144
init$3144:
	.skip 0x4
.global instance$3142
instance$3142:
	.skip 0x4
.global init$3156
init$3156:
	.skip 0x4
.global instance$3154
instance$3154:
	.skip 0x4
.global init$2531
init$2531:
	.skip 0x4
.global instance$2529
instance$2529:
	.skip 0x4
.global init$2543
init$2543:
	.skip 0x4
.global instance$2541
instance$2541:
	.skip 0x4
.global init$2557
init$2557:
	.skip 0x4
.global instance$2555
instance$2555:
	.skip 0x4
.global init$2567
init$2567:
	.skip 0x4
.global instance$2565
instance$2565:
	.skip 0x4
.global init$2577
init$2577:
	.skip 0x4
.global instance$2575
instance$2575:
	.skip 0x4
.global init$localstatic1$theNerve__18TNervePeachStaggerFv
init$localstatic1$theNerve__18TNervePeachStaggerFv:
	.skip 0x4
.global nerve$localstatic0$theNerve__18TNervePeachStaggerFv
nerve$localstatic0$theNerve__18TNervePeachStaggerFv:
	.skip 0x4
.global init$localstatic1$theNerve__17TNervePeachEscapeFv
init$localstatic1$theNerve__17TNervePeachEscapeFv:
	.skip 0x4
.global nerve$localstatic0$theNerve__17TNervePeachEscapeFv
nerve$localstatic0$theNerve__17TNervePeachEscapeFv:
	.skip 0x4
.global init$3127
init$3127:
	.skip 0x4
.global instance$3125
instance$3125:
	.skip 0x4
.global init$3141
init$3141:
	.skip 0x4
.global instance$3139
instance$3139:
	.skip 0x4
.global init$3156
init$3156:
	.skip 0x4
.global instance$3154
instance$3154:
	.skip 0x4
.global init$3170
init$3170:
	.skip 0x4
.global instance$3168
instance$3168:
	.skip 0x4
.global init$3180
init$3180:
	.skip 0x4
.global instance$3178
instance$3178:
	.skip 0x4
.global init$3190
init$3190:
	.skip 0x4
.global instance$3188
instance$3188:
	.skip 0x4
.global init$2870
init$2870:
	.skip 0x4
.global instance$2868
instance$2868:
	.skip 0x4
.global init$2914
init$2914:
	.skip 0x4
.global instance$2912
instance$2912:
	.skip 0x4
.global init$2851
init$2851:
	.skip 0x4
.global instance$2849
instance$2849:
	.skip 0x4
.global cAngleMax__9$$2unnamed$$2
cAngleMax__9$$2unnamed$$2:
	.skip 0x4
.global init$2872
init$2872:
	.skip 0x4
.global instance$2870
instance$2870:
	.skip 0x4
.global init$2887
init$2887:
	.skip 0x4
.global instance$2885
instance$2885:
	.skip 0x4
.global init$2897
init$2897:
	.skip 0x4
.global instance$2895
instance$2895:
	.skip 0x4
.global init$2910
init$2910:
	.skip 0x4
.global instance$2908
instance$2908:
	.skip 0x4
.global init$2923
init$2923:
	.skip 0x4
.global instance$2921
instance$2921:
	.skip 0x4
.global init$2929
init$2929:
	.skip 0x4
.global instance$2927
instance$2927:
	.skip 0x4
.global init$2939
init$2939:
	.skip 0x4
.global instance$2937
instance$2937:
	.skip 0x8
.global init$localstatic1$theNerve__27TNerveLimitKoopaHipDropJumpFv
init$localstatic1$theNerve__27TNerveLimitKoopaHipDropJumpFv:
	.skip 0x4
.global nerve$localstatic0$theNerve__27TNerveLimitKoopaHipDropJumpFv
nerve$localstatic0$theNerve__27TNerveLimitKoopaHipDropJumpFv:
	.skip 0x4
.global init$localstatic1$theNerve__28TNerveLimitKoopaHipDropStartFv
init$localstatic1$theNerve__28TNerveLimitKoopaHipDropStartFv:
	.skip 0x4
.global nerve$localstatic0$theNerve__28TNerveLimitKoopaHipDropStartFv
nerve$localstatic0$theNerve__28TNerveLimitKoopaHipDropStartFv:
	.skip 0x4
.global init$localstatic1$theNerve__22TNerveLimitKoopaTumbleFv
init$localstatic1$theNerve__22TNerveLimitKoopaTumbleFv:
	.skip 0x4
.global nerve$localstatic0$theNerve__22TNerveLimitKoopaTumbleFv
nerve$localstatic0$theNerve__22TNerveLimitKoopaTumbleFv:
	.skip 0x4
.global init$localstatic1$theNerve__20TNerveLimitKoopaWaitFv
init$localstatic1$theNerve__20TNerveLimitKoopaWaitFv:
	.skip 0x4
.global nerve$localstatic0$theNerve__20TNerveLimitKoopaWaitFv
nerve$localstatic0$theNerve__20TNerveLimitKoopaWaitFv:
	.skip 0x4
.global init$localstatic1$theNerve__23TNerveLimitKoopaGetDownFv
init$localstatic1$theNerve__23TNerveLimitKoopaGetDownFv:
	.skip 0x4
.global nerve$localstatic0$theNerve__23TNerveLimitKoopaGetDownFv
nerve$localstatic0$theNerve__23TNerveLimitKoopaGetDownFv:
	.skip 0x4
.global init$localstatic1$theNerve__23TNerveLimitKoopaStaggerFv
init$localstatic1$theNerve__23TNerveLimitKoopaStaggerFv:
	.skip 0x4
.global nerve$localstatic0$theNerve__23TNerveLimitKoopaStaggerFv
nerve$localstatic0$theNerve__23TNerveLimitKoopaStaggerFv:
	.skip 0x4
.global init$localstatic1$theNerve__27TNerveLimitKoopaGetShoweredFv
init$localstatic1$theNerve__27TNerveLimitKoopaGetShoweredFv:
	.skip 0x4
.global nerve$localstatic0$theNerve__27TNerveLimitKoopaGetShoweredFv
nerve$localstatic0$theNerve__27TNerveLimitKoopaGetShoweredFv:
	.skip 0x4
.global init$3001
init$3001:
	.skip 0x4
.global instance$2999
instance$2999:
	.skip 0x4
.global init$3013
init$3013:
	.skip 0x4
.global instance$3011
instance$3011:
	.skip 0x4
.global init$3027
init$3027:
	.skip 0x4
.global instance$3025
instance$3025:
	.skip 0x4
.global init$3037
init$3037:
	.skip 0x4
.global instance$3035
instance$3035:
	.skip 0x4
.global gpCamera
gpCamera:
	.skip 0x8
.global gpCameraMario
gpCameraMario:
	.skip 0x8
.global gpCameraShake
gpCameraShake:
	.skip 0x8
.global gpSunMgr
gpSunMgr:
	.skip 0x8
.global gpSunModel
gpSunModel:
	.skip 0x8
.global gpCubeCamera
gpCubeCamera:
	.skip 0x4
.global gpCubeMirror
gpCubeMirror:
	.skip 0x4
.global gpCubeWire
gpCubeWire:
	.skip 0x4
.global gpCubeStream
gpCubeStream:
	.skip 0x4
.global gpCubeShadow
gpCubeShadow:
	.skip 0x4
.global gpCubeArea
gpCubeArea:
	.skip 0x4
.global gpCubeFastA
gpCubeFastA:
	.skip 0x4
.global gpCubeFastB
gpCubeFastB:
	.skip 0x4
.global gpCubeFastC
gpCubeFastC:
	.skip 0x4
.global gpCubeSoundChange
gpCubeSoundChange:
	.skip 0x4
.global gpCubeSoundEffect
gpCubeSoundEffect:
	.skip 0x8
.global gpCamMapToolTable
gpCamMapToolTable:
	.skip 0x8
.global gpCameraOption
gpCameraOption:
	.skip 0x8
.global init$2002
init$2002:
	.skip 0x4
.global instance$2000
instance$2000:
	.skip 0x4
.global init$3027
init$3027:
	.skip 0x4
.global instance$3025
instance$3025:
	.skip 0x4
.global init$3042
init$3042:
	.skip 0x4
.global instance$3040
instance$3040:
	.skip 0x4
.global init$3057
init$3057:
	.skip 0x4
.global instance$3055
instance$3055:
	.skip 0x4
.global init$3069
init$3069:
	.skip 0x4
.global instance$3067
instance$3067:
	.skip 0x4
.global init$3079
init$3079:
	.skip 0x4
.global instance$3077
instance$3077:
	.skip 0x4
.global init$3098
init$3098:
	.skip 0x4
.global instance$3096
instance$3096:
	.skip 0x4
.global init$3106
init$3106:
	.skip 0x4
.global instance$3104
instance$3104:
	.skip 0x4
.global init$3118
init$3118:
	.skip 0x4
.global instance$3116
instance$3116:
	.skip 0x4
.global init$3130
init$3130:
	.skip 0x4
.global instance$3128
instance$3128:
	.skip 0x4
.global cAngleLimit__9$$2unnamed$$2
cAngleLimit__9$$2unnamed$$2:
	.skip 0x4
.global init$2991
init$2991:
	.skip 0x4
.global instance$2989
instance$2989:
	.skip 0x4
.global init$3005
init$3005:
	.skip 0x4
.global instance$3003
instance$3003:
	.skip 0x4
.global init$3015
init$3015:
	.skip 0x4
.global instance$3013
instance$3013:
	.skip 0x4
.global init$3025
init$3025:
	.skip 0x4
.global instance$3023
instance$3023:
	.skip 0x4
.global init$3037
init$3037:
	.skip 0x4
.global instance$3035
instance$3035:
	.skip 0x4
.global init$3049
init$3049:
	.skip 0x4
.global instance$3047
instance$3047:
	.skip 0x4

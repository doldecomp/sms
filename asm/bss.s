.include "macros.inc"

.section .bss, "wa"  # 0x803E6000 - 0x8040B45C
.global gpApplication
gpApplication:
	.skip 0x50
.global $$256
$$256:
	.skip 0x10
.global $$260
$$260:
	.skip 0x10
.global $$2237
$$2237:
	.skip 0x10
.global $$21483
$$21483:
	.skip 0x10
.global $$225
$$225:
	.skip 0x10
.global exCallbackObject
exCallbackObject:
	.skip 0x14
.global $$21483
$$21483:
	.skip 0xC
.global $$2301
$$2301:
	.skip 0x10
.global $$256
$$256:
	.skip 0x10
.global $$256
$$256:
	.skip 0x10
.global $$260
$$260:
	.skip 0x10
.global sMessageFileLine__Q212JUTAssertion9$$2unnamed$$2
sMessageFileLine__Q212JUTAssertion9$$2unnamed$$2:
	.skip 0x40
.global sMessageString__Q212JUTAssertion9$$2unnamed$$2
sMessageString__Q212JUTAssertion9$$2unnamed$$2:
	.skip 0x60
.global $$21661
$$21661:
	.skip 0xC
.global $$21662
$$21662:
	.skip 0xC
.global $$21663
$$21663:
	.skip 0xC
.global $$21664
$$21664:
	.skip 0xC
.global $$21665
$$21665:
	.skip 0xC
.global $$21666
$$21666:
	.skip 0xC
.global $$21667
$$21667:
	.skip 0xC
.global $$21668
$$21668:
	.skip 0xC
.global $$21669
$$21669:
	.skip 0xC
.global $$21670
$$21670:
	.skip 0xC
.global $$21671
$$21671:
	.skip 0xC
.global $$21672
$$21672:
	.skip 0xC
.global finfo__Q27JAInter9StreamLib
finfo__Q27JAInter9StreamLib:
	.skip 0x3C
.global header__Q27JAInter9StreamLib
header__Q27JAInter9StreamLib:
	.skip 0x20
.global Filename__Q27JAInter9StreamLib
Filename__Q27JAInter9StreamLib:
	.skip 0x64
.global $$2531
$$2531:
	.skip 0x10
.global $$2557
$$2557:
	.skip 0x10
.global osc$294
osc$294:
	.skip 0x18
.global $$2257
$$2257:
	.skip 0xC
.global sAramHeap__Q28JASystem13WaveArcLoader
sAramHeap__Q28JASystem13WaveArcLoader:
	.skip 0x2C
.global audioproc_mq__Q28JASystem11AudioThread
audioproc_mq__Q28JASystem11AudioThread:
	.skip 0x20
.global msgbuf__Q28JASystem11AudioThread
msgbuf__Q28JASystem11AudioThread:
	.skip 0x40
.global dsp_buf__Q28JASystem6DSPBuf
dsp_buf__Q28JASystem6DSPBuf:
	.skip 0x10
.global dac__Q28JASystem6Kernel
dac__Q28JASystem6Kernel:
	.skip 0x10
.global mq__Q28JASystem3Dvd
mq__Q28JASystem3Dvd:
	.skip 0x20
.global finfo$314
finfo$314:
	.skip 0x3C
.global req$315
req$315:
	.skip 0x80
.global finfo$361
finfo$361:
	.skip 0x3C
.global req$362
req$362:
	.skip 0x20
.global finfo$369
finfo$369:
	.skip 0x3C
.global req$370
req$370:
	.skip 0x20
.global finfo$393
finfo$393:
	.skip 0x3C
.global finfo$422
finfo$422:
	.skip 0x3C
.global finfo$426
finfo$426:
	.skip 0x3C
.global $$2245
$$2245:
	.skip 0xC
.global strCtrl__Q28JASystem10HardStream
strCtrl__Q28JASystem10HardStream:
	.skip 0x50
.global finfo$425
finfo$425:
	.skip 0xB4
.global get_addr_cmd$429
get_addr_cmd$429:
	.skip 0x300
.global stop_cmd$430
stop_cmd$430:
	.skip 0x30
.global finish_cmd$431
finish_cmd$431:
	.skip 0x30
.global $$2241
$$2241:
	.skip 0xC
.global audioAramHeap__Q28JASystem6Kernel
audioAramHeap__Q28JASystem6Kernel:
	.skip 0x14
.global oRandom__Q28JASystem6Player
oRandom__Q28JASystem6Player:
	.skip 0x88
.global sTrackList__Q28JASystem8TrackMgr
sTrackList__Q28JASystem8TrackMgr:
	.skip 0x100
.global JPAEmitterInfoObj
JPAEmitterInfoObj:
	.skip 0x180
.global $$21657
$$21657:
	.skip 0xC
.global $$21658
$$21658:
	.skip 0x14
.global audio_task
audio_task:
	.skip 0x60
.global AUDIO_YIELD_BUFFER
AUDIO_YIELD_BUFFER:
	.skip 0x2000
.global taskwork
taskwork:
	.skip 0x80
.global sync_stack
sync_stack:
	.skip 0x10
.global fragmentinfo
fragmentinfo:
	.skip 0x10
.global atexit_funcs
atexit_funcs:
	.skip 0x100
.global __atexit_funcs
__atexit_funcs:
	.skip 0x100
.global gTRKEventQueue
gTRKEventQueue:
	.skip 0x28
.global gTRKBigEndian
gTRKBigEndian:
	.skip 0x8
.global gTRKMsgBufs
gTRKMsgBufs:
	.skip 0x19B0
.global gTRKFramingState
gTRKFramingState:
	.skip 0x14
.global gTRKInputPendingPtr
gTRKInputPendingPtr:
	.skip 0x4
.global gTRKDispatchTableSize
gTRKDispatchTableSize:
	.skip 0x8
.global TRK_saved_exceptionID
TRK_saved_exceptionID:
	.skip 0x4
.global gTRKSaveState
gTRKSaveState:
	.skip 0x94
.global TRKvalue128_temp
TRKvalue128_temp:
	.skip 0x10
.global gTRKState
gTRKState:
	.skip 0xA8
.global gTRKCPUState
gTRKCPUState:
	.skip 0x430
.global lc_base
lc_base:
	.skip 0x8
.global TRK_mainError
TRK_mainError:
	.skip 0x18
.global DriveInfo
DriveInfo:
	.skip 0x20
.global DriveBlock
DriveBlock:
	.skip 0x30
.global __OSErrorTable
__OSErrorTable:
	.skip 0x50
.global Header
Header:
	.skip 0x20
.global Scb
Scb:
	.skip 0x58
.global RunQueue
RunQueue:
	.skip 0x100
.global IdleThread
IdleThread:
	.skip 0x310
.global DefaultThread
DefaultThread:
	.skip 0x310
.global IdleContext
IdleContext:
	.skip 0x2C8
.global CommandList
CommandList:
	.skip 0x40
.global AlarmForWA
AlarmForWA:
	.skip 0x28
.global AlarmForTimeout
AlarmForTimeout:
	.skip 0x28
.global AlarmForBreak
AlarmForBreak:
	.skip 0x28
.global Prev
Prev:
	.skip 0xC
.global Curr
Curr:
	.skip 0x1C
.global tmpBuffer
tmpBuffer:
	.skip 0x80
.global DummyCommandBlock
DummyCommandBlock:
	.skip 0x30
.global ResetAlarm
ResetAlarm:
	.skip 0x28
.global WaitingQueue
WaitingQueue:
	.skip 0x20
.global bb2Buf
bb2Buf:
	.skip 0x40
.global block$16
block$16:
	.skip 0x30
.global regs
regs:
	.skip 0x78
.global shdwRegs
shdwRegs:
	.skip 0x78
.global HorVer
HorVer:
	.skip 0x58
.global Type
Type:
	.skip 0x10
.global Origin
Origin:
	.skip 0x30
.global CmdProbeDevice
CmdProbeDevice:
	.skip 0x10
.global __CARDBlock
__CARDBlock:
	.skip 0x220
.global __CARDDiskNone
__CARDDiskNone:
	.skip 0x20
.global gxData
gxData:
	.skip 0x4F8
.global FifoObj
FifoObj:
	.skip 0x80
.global vcd
vcd:
	.skip 0xD8
.global vat
vat:
	.skip 0x1B0
.global Packet
Packet:
	.skip 0x80
.global Alarm
Alarm:
	.skip 0xA0
.global TypeTime
TypeTime:
	.skip 0x20
.global XferTime
XferTime:
	.skip 0x20
.global TypeCallback
TypeCallback:
	.skip 0x40
.global RDSTHandler
RDSTHandler:
	.skip 0x10
.global InputBufferValid
InputBufferValid:
	.skip 0x10
.global InputBuffer
InputBuffer:
	.skip 0x20
.global InputBufferVcount
InputBufferVcount:
	.skip 0x10
.global cmdFixDevice$326
cmdFixDevice$326:
	.skip 0x10
.global Ecb
Ecb:
	.skip 0xC0
.global __THPIDCTWorkspace
__THPIDCTWorkspace:
	.skip 0x100
.global __THPLCWork512
__THPLCWork512:
	.skip 0xC
.global __THPLCWork640
__THPLCWork640:
	.skip 0xC
.global __THPMCUBuffer
__THPMCUBuffer:
	.skip 0x18
.global AudioDecodeThread
AudioDecodeThread:
	.skip 0x310
.global AudioDecodeThreadStack
AudioDecodeThreadStack:
	.skip 0x1000
.global FreeAudioBufferQueue
FreeAudioBufferQueue:
	.skip 0x20
.global DecodedAudioBufferQueue
DecodedAudioBufferQueue:
	.skip 0x20
.global FreeAudioBufferMessage
FreeAudioBufferMessage:
	.skip 0xC
.global DecodedAudioBufferMessage
DecodedAudioBufferMessage:
	.skip 0x14
.global ActivePlayer
ActivePlayer:
	.skip 0x1E0
.global WorkBuffer
WorkBuffer:
	.skip 0x40
.global PrepareReadyQueue
PrepareReadyQueue:
	.skip 0x20
.global UsedTextureSetQueue
UsedTextureSetQueue:
	.skip 0x20
.global UsedTextureSetMessage
UsedTextureSetMessage:
	.skip 0x20
.global SoundBuffer
SoundBuffer:
	.skip 0x1180
.global FreeReadBufferQueue
FreeReadBufferQueue:
	.skip 0x20
.global ReadedBufferQueue
ReadedBufferQueue:
	.skip 0x20
.global ReadedBufferQueue2
ReadedBufferQueue2:
	.skip 0x20
.global FreeReadBufferMessage
FreeReadBufferMessage:
	.skip 0x28
.global ReadedBufferMessage
ReadedBufferMessage:
	.skip 0x28
.global ReadedBufferMessage2
ReadedBufferMessage2:
	.skip 0x28
.global ReadThread
ReadThread:
	.skip 0x310
.global ReadThreadStack
ReadThreadStack:
	.skip 0x1000
.global VideoDecodeThread
VideoDecodeThread:
	.skip 0x310
.global VideoDecodeThreadStack
VideoDecodeThreadStack:
	.skip 0x1000
.global FreeTextureSetQueue
FreeTextureSetQueue:
	.skip 0x20
.global DecodedTextureSetQueue
DecodedTextureSetQueue:
	.skip 0x20
.global FreeTextureSetMessage
FreeTextureSetMessage:
	.skip 0xC
.global DecodedTextureSetMessage
DecodedTextureSetMessage:
	.skip 0xC
.global sViewPlane
sViewPlane:
	.skip 0x60
.global $$22174
$$22174:
	.skip 0xC
.global setup1$2173
setup1$2173:
	.skip 0x24
.global $$22178
$$22178:
	.skip 0xC
.global cylinder$2177
cylinder$2177:
	.skip 0x24
.global $$22192
$$22192:
	.skip 0xC
.global setup2$2191
setup2$2191:
	.skip 0x24
.global $$22197
$$22197:
	.skip 0xC
.global setup3$2196
setup3$2196:
	.skip 0x24
.global $$22209
$$22209:
	.skip 0xC
.global setup4$2208
setup4$2208:
	.skip 0x24
.global $$22218
$$22218:
	.skip 0xC
.global setup5$2217
setup5$2217:
	.skip 0x24
.global $$27636
$$27636:
	.skip 0xC
.global $$27637
$$27637:
	.skip 0xC
.global $$27638
$$27638:
	.skip 0xC
.global $$27639
$$27639:
	.skip 0xC
.global $$27640
$$27640:
	.skip 0xC
.global $$27641
$$27641:
	.skip 0xC
.global $$27642
$$27642:
	.skip 0xC
.global $$27643
$$27643:
	.skip 0xC
.global $$27644
$$27644:
	.skip 0xC
.global $$27645
$$27645:
	.skip 0xC
.global $$27646
$$27646:
	.skip 0xC
.global $$27647
$$27647:
	.skip 0xC
.global $$27648
$$27648:
	.skip 0xC
.global $$27649
$$27649:
	.skip 0xC
.global $$27650
$$27650:
	.skip 0x10
.global $$22705
$$22705:
	.skip 0xC
.global $$22706
$$22706:
	.skip 0xC
.global $$22707
$$22707:
	.skip 0xC
.global $$22708
$$22708:
	.skip 0xC
.global $$22709
$$22709:
	.skip 0xC
.global $$22710
$$22710:
	.skip 0xC
.global $$22711
$$22711:
	.skip 0xC
.global $$22712
$$22712:
	.skip 0xC
.global $$22713
$$22713:
	.skip 0xC
.global $$22714
$$22714:
	.skip 0xC
.global $$22715
$$22715:
	.skip 0xC
.global $$22716
$$22716:
	.skip 0xC
.global $$22717
$$22717:
	.skip 0xC
.global $$22718
$$22718:
	.skip 0xC
.global $$22719
$$22719:
	.skip 0x10
.global $$22269
$$22269:
	.skip 0xC
.global $$22270
$$22270:
	.skip 0xC
.global $$22271
$$22271:
	.skip 0xC
.global $$22272
$$22272:
	.skip 0xC
.global $$22273
$$22273:
	.skip 0xC
.global $$22274
$$22274:
	.skip 0xC
.global $$22275
$$22275:
	.skip 0xC
.global $$22276
$$22276:
	.skip 0xC
.global $$22277
$$22277:
	.skip 0xC
.global $$22278
$$22278:
	.skip 0xC
.global $$22279
$$22279:
	.skip 0xC
.global $$22280
$$22280:
	.skip 0xC
.global $$22281
$$22281:
	.skip 0xC
.global $$22282
$$22282:
	.skip 0xC
.global $$22283
$$22283:
	.skip 0x10
.global $$23491
$$23491:
	.skip 0xC
.global $$23492
$$23492:
	.skip 0xC
.global $$23493
$$23493:
	.skip 0xC
.global $$23494
$$23494:
	.skip 0xC
.global $$23495
$$23495:
	.skip 0xC
.global $$23496
$$23496:
	.skip 0xC
.global $$23497
$$23497:
	.skip 0xC
.global $$23498
$$23498:
	.skip 0xC
.global $$23499
$$23499:
	.skip 0xC
.global $$23500
$$23500:
	.skip 0xC
.global $$23501
$$23501:
	.skip 0xC
.global $$23502
$$23502:
	.skip 0xC
.global $$23503
$$23503:
	.skip 0xC
.global $$23504
$$23504:
	.skip 0xC
.global $$23505
$$23505:
	.skip 0x10
.global $$22764
$$22764:
	.skip 0xC
.global $$22765
$$22765:
	.skip 0xC
.global $$22766
$$22766:
	.skip 0xC
.global $$22767
$$22767:
	.skip 0xC
.global $$22768
$$22768:
	.skip 0xC
.global $$22769
$$22769:
	.skip 0xC
.global $$22770
$$22770:
	.skip 0xC
.global $$22771
$$22771:
	.skip 0xC
.global $$22772
$$22772:
	.skip 0xC
.global $$22773
$$22773:
	.skip 0xC
.global $$22774
$$22774:
	.skip 0xC
.global $$22775
$$22775:
	.skip 0xC
.global $$22776
$$22776:
	.skip 0xC
.global $$22777
$$22777:
	.skip 0xC
.global $$22778
$$22778:
	.skip 0x10
.global $$23428
$$23428:
	.skip 0xC
.global $$23429
$$23429:
	.skip 0xC
.global $$23430
$$23430:
	.skip 0xC
.global $$23431
$$23431:
	.skip 0xC
.global $$23432
$$23432:
	.skip 0xC
.global $$23433
$$23433:
	.skip 0xC
.global $$23434
$$23434:
	.skip 0xC
.global $$23435
$$23435:
	.skip 0xC
.global $$23436
$$23436:
	.skip 0xC
.global $$23437
$$23437:
	.skip 0xC
.global $$23438
$$23438:
	.skip 0xC
.global $$23439
$$23439:
	.skip 0xC
.global $$23440
$$23440:
	.skip 0xC
.global $$23441
$$23441:
	.skip 0xC
.global $$23442
$$23442:
	.skip 0x10
.global $$26138
$$26138:
	.skip 0xC
.global $$26139
$$26139:
	.skip 0xC
.global $$26140
$$26140:
	.skip 0xC
.global $$26141
$$26141:
	.skip 0xC
.global $$26142
$$26142:
	.skip 0xC
.global $$26143
$$26143:
	.skip 0xC
.global $$26144
$$26144:
	.skip 0xC
.global $$26145
$$26145:
	.skip 0xC
.global $$26146
$$26146:
	.skip 0xC
.global $$26147
$$26147:
	.skip 0xC
.global $$26148
$$26148:
	.skip 0xC
.global $$26149
$$26149:
	.skip 0xC
.global $$26150
$$26150:
	.skip 0xC
.global $$26151
$$26151:
	.skip 0xC
.global $$26152
$$26152:
	.skip 0x10
.global $$23301
$$23301:
	.skip 0xC
.global $$23302
$$23302:
	.skip 0xC
.global $$23303
$$23303:
	.skip 0xC
.global $$23304
$$23304:
	.skip 0xC
.global $$23305
$$23305:
	.skip 0xC
.global $$23306
$$23306:
	.skip 0xC
.global $$23307
$$23307:
	.skip 0xC
.global $$23308
$$23308:
	.skip 0xC
.global $$23309
$$23309:
	.skip 0xC
.global $$23310
$$23310:
	.skip 0xC
.global $$23311
$$23311:
	.skip 0xC
.global $$23312
$$23312:
	.skip 0xC
.global $$23313
$$23313:
	.skip 0xC
.global $$23314
$$23314:
	.skip 0xC
.global $$23315
$$23315:
	.skip 0x10
.global gSetupThread
gSetupThread:
	.skip 0x310
.global $$22501
$$22501:
	.skip 0xC
.global $$22502
$$22502:
	.skip 0xC
.global $$22503
$$22503:
	.skip 0xC
.global $$22504
$$22504:
	.skip 0xC
.global $$22505
$$22505:
	.skip 0xC
.global $$22506
$$22506:
	.skip 0xC
.global $$22507
$$22507:
	.skip 0xC
.global $$22508
$$22508:
	.skip 0xC
.global $$22509
$$22509:
	.skip 0xC
.global $$22510
$$22510:
	.skip 0xC
.global $$22511
$$22511:
	.skip 0xC
.global $$22512
$$22512:
	.skip 0xC
.global $$22513
$$22513:
	.skip 0xC
.global $$22514
$$22514:
	.skip 0xC
.global $$22515
$$22515:
	.skip 0x10
.global $$23239
$$23239:
	.skip 0xC
.global $$23240
$$23240:
	.skip 0xC
.global $$23241
$$23241:
	.skip 0xC
.global $$23242
$$23242:
	.skip 0xC
.global $$23243
$$23243:
	.skip 0xC
.global $$23244
$$23244:
	.skip 0xC
.global $$23245
$$23245:
	.skip 0xC
.global $$23246
$$23246:
	.skip 0xC
.global $$23247
$$23247:
	.skip 0xC
.global $$23248
$$23248:
	.skip 0xC
.global $$23249
$$23249:
	.skip 0xC
.global $$23250
$$23250:
	.skip 0xC
.global $$23251
$$23251:
	.skip 0xC
.global $$23252
$$23252:
	.skip 0xC
.global $$23253
$$23253:
	.skip 0x10
.global gParticleFlagLoaded
gParticleFlagLoaded:
	.skip 0x204
.global $$22634
$$22634:
	.skip 0xC
.global $$22635
$$22635:
	.skip 0xC
.global $$22636
$$22636:
	.skip 0xC
.global $$22637
$$22637:
	.skip 0xC
.global $$22638
$$22638:
	.skip 0xC
.global $$22639
$$22639:
	.skip 0xC
.global $$22640
$$22640:
	.skip 0xC
.global $$22641
$$22641:
	.skip 0xC
.global $$22642
$$22642:
	.skip 0xC
.global $$22643
$$22643:
	.skip 0xC
.global $$22644
$$22644:
	.skip 0xC
.global $$22645
$$22645:
	.skip 0xC
.global $$22646
$$22646:
	.skip 0xC
.global $$22647
$$22647:
	.skip 0xC
.global $$22648
$$22648:
	.skip 0xC
.global $$22913
$$22913:
	.skip 0xC
.global $$22914
$$22914:
	.skip 0xC
.global $$22915
$$22915:
	.skip 0xC
.global $$22916
$$22916:
	.skip 0xC
.global $$22917
$$22917:
	.skip 0xC
.global $$22918
$$22918:
	.skip 0xC
.global $$22919
$$22919:
	.skip 0xC
.global $$22920
$$22920:
	.skip 0xC
.global $$22921
$$22921:
	.skip 0xC
.global $$22922
$$22922:
	.skip 0xC
.global $$22923
$$22923:
	.skip 0xC
.global $$22924
$$22924:
	.skip 0xC
.global $$22925
$$22925:
	.skip 0xC
.global $$22926
$$22926:
	.skip 0xC
.global $$22927
$$22927:
	.skip 0x10
.global $$22579
$$22579:
	.skip 0xC
.global $$22580
$$22580:
	.skip 0xC
.global $$22581
$$22581:
	.skip 0xC
.global $$22582
$$22582:
	.skip 0xC
.global $$22583
$$22583:
	.skip 0xC
.global $$22584
$$22584:
	.skip 0xC
.global $$22585
$$22585:
	.skip 0xC
.global $$22586
$$22586:
	.skip 0xC
.global $$22587
$$22587:
	.skip 0xC
.global $$22588
$$22588:
	.skip 0xC
.global $$22589
$$22589:
	.skip 0xC
.global $$22590
$$22590:
	.skip 0xC
.global $$22591
$$22591:
	.skip 0xC
.global $$22592
$$22592:
	.skip 0xC
.global $$22593
$$22593:
	.skip 0x10
.global $$23490
$$23490:
	.skip 0xC
.global $$23491
$$23491:
	.skip 0xC
.global $$23492
$$23492:
	.skip 0xC
.global $$23493
$$23493:
	.skip 0xC
.global $$23494
$$23494:
	.skip 0xC
.global $$23495
$$23495:
	.skip 0xC
.global $$23496
$$23496:
	.skip 0xC
.global $$23497
$$23497:
	.skip 0xC
.global $$23498
$$23498:
	.skip 0xC
.global $$23499
$$23499:
	.skip 0xC
.global $$23500
$$23500:
	.skip 0xC
.global $$23501
$$23501:
	.skip 0xC
.global $$23502
$$23502:
	.skip 0xC
.global $$23503
$$23503:
	.skip 0xC
.global $$23504
$$23504:
	.skip 0x10
.global $$23355
$$23355:
	.skip 0xC
.global $$23356
$$23356:
	.skip 0xC
.global $$23357
$$23357:
	.skip 0xC
.global $$23358
$$23358:
	.skip 0xC
.global $$23359
$$23359:
	.skip 0xC
.global $$23360
$$23360:
	.skip 0xC
.global $$23361
$$23361:
	.skip 0xC
.global $$23362
$$23362:
	.skip 0xC
.global $$23363
$$23363:
	.skip 0xC
.global $$23364
$$23364:
	.skip 0xC
.global $$23365
$$23365:
	.skip 0xC
.global $$23366
$$23366:
	.skip 0xC
.global $$23367
$$23367:
	.skip 0xC
.global $$23368
$$23368:
	.skip 0xC
.global $$23369
$$23369:
	.skip 0x10
.global $$22728
$$22728:
	.skip 0xC
.global $$22729
$$22729:
	.skip 0xC
.global $$22730
$$22730:
	.skip 0xC
.global $$22731
$$22731:
	.skip 0xC
.global $$22732
$$22732:
	.skip 0xC
.global $$22733
$$22733:
	.skip 0xC
.global $$22734
$$22734:
	.skip 0xC
.global $$22735
$$22735:
	.skip 0xC
.global $$22736
$$22736:
	.skip 0xC
.global $$22737
$$22737:
	.skip 0xC
.global $$22738
$$22738:
	.skip 0xC
.global $$22739
$$22739:
	.skip 0xC
.global $$22740
$$22740:
	.skip 0xC
.global $$22741
$$22741:
	.skip 0xC
.global $$22742
$$22742:
	.skip 0x10
.global cDeformedTerrainCenter
cDeformedTerrainCenter:
	.skip 0x10
.global $$22108
$$22108:
	.skip 0xC
.global $$22109
$$22109:
	.skip 0xC
.global $$22110
$$22110:
	.skip 0xC
.global $$22111
$$22111:
	.skip 0xC
.global $$22112
$$22112:
	.skip 0xC
.global $$22113
$$22113:
	.skip 0xC
.global $$22114
$$22114:
	.skip 0xC
.global $$22115
$$22115:
	.skip 0xC
.global $$22116
$$22116:
	.skip 0xC
.global $$22117
$$22117:
	.skip 0xC
.global $$22118
$$22118:
	.skip 0xC
.global $$22119
$$22119:
	.skip 0xC
.global $$22120
$$22120:
	.skip 0xC
.global $$22121
$$22121:
	.skip 0xC
.global $$22122
$$22122:
	.skip 0x10
.global $$23085
$$23085:
	.skip 0xC
.global $$23086
$$23086:
	.skip 0xC
.global $$23087
$$23087:
	.skip 0xC
.global $$23088
$$23088:
	.skip 0xC
.global $$23089
$$23089:
	.skip 0xC
.global $$23090
$$23090:
	.skip 0xC
.global $$23091
$$23091:
	.skip 0xC
.global $$23092
$$23092:
	.skip 0xC
.global $$23093
$$23093:
	.skip 0xC
.global $$23094
$$23094:
	.skip 0xC
.global $$23095
$$23095:
	.skip 0xC
.global $$23096
$$23096:
	.skip 0xC
.global $$23097
$$23097:
	.skip 0xC
.global $$23098
$$23098:
	.skip 0xC
.global $$23099
$$23099:
	.skip 0x10
.global $$22524
$$22524:
	.skip 0xC
.global $$22525
$$22525:
	.skip 0xC
.global $$22526
$$22526:
	.skip 0xC
.global $$22527
$$22527:
	.skip 0xC
.global $$22528
$$22528:
	.skip 0xC
.global $$22529
$$22529:
	.skip 0xC
.global $$22530
$$22530:
	.skip 0xC
.global $$22531
$$22531:
	.skip 0xC
.global $$22532
$$22532:
	.skip 0xC
.global $$22533
$$22533:
	.skip 0xC
.global $$22534
$$22534:
	.skip 0xC
.global $$22535
$$22535:
	.skip 0xC
.global $$22536
$$22536:
	.skip 0xC
.global $$22537
$$22537:
	.skip 0xC
.global $$22538
$$22538:
	.skip 0x10
.global $$23721
$$23721:
	.skip 0xC
.global $$23722
$$23722:
	.skip 0xC
.global $$23723
$$23723:
	.skip 0xC
.global $$23724
$$23724:
	.skip 0xC
.global $$23725
$$23725:
	.skip 0xC
.global $$23726
$$23726:
	.skip 0xC
.global $$23727
$$23727:
	.skip 0xC
.global $$23728
$$23728:
	.skip 0xC
.global $$23729
$$23729:
	.skip 0xC
.global $$23730
$$23730:
	.skip 0xC
.global $$23731
$$23731:
	.skip 0xC
.global $$23732
$$23732:
	.skip 0xC
.global $$23733
$$23733:
	.skip 0xC
.global $$23734
$$23734:
	.skip 0xC
.global $$23735
$$23735:
	.skip 0x10
.global $$23120
$$23120:
	.skip 0xC
.global $$23121
$$23121:
	.skip 0xC
.global $$23122
$$23122:
	.skip 0xC
.global $$23123
$$23123:
	.skip 0xC
.global $$23124
$$23124:
	.skip 0xC
.global $$23125
$$23125:
	.skip 0xC
.global $$23126
$$23126:
	.skip 0xC
.global $$23127
$$23127:
	.skip 0xC
.global $$23128
$$23128:
	.skip 0xC
.global $$23129
$$23129:
	.skip 0xC
.global $$23130
$$23130:
	.skip 0xC
.global $$23131
$$23131:
	.skip 0xC
.global $$23132
$$23132:
	.skip 0xC
.global $$23133
$$23133:
	.skip 0xC
.global $$23134
$$23134:
	.skip 0x10
.global cDeformedTerrainCenter
cDeformedTerrainCenter:
	.skip 0xC
.global $$22772
$$22772:
	.skip 0xC
.global $$22773
$$22773:
	.skip 0xC
.global $$22774
$$22774:
	.skip 0xC
.global $$22775
$$22775:
	.skip 0xC
.global $$22776
$$22776:
	.skip 0xC
.global $$22777
$$22777:
	.skip 0xC
.global $$22778
$$22778:
	.skip 0xC
.global $$22779
$$22779:
	.skip 0xC
.global $$22780
$$22780:
	.skip 0xC
.global $$22781
$$22781:
	.skip 0xC
.global $$22782
$$22782:
	.skip 0xC
.global $$22783
$$22783:
	.skip 0xC
.global $$22784
$$22784:
	.skip 0xC
.global $$22785
$$22785:
	.skip 0xC
.global $$22786
$$22786:
	.skip 0xC
.global $$24093
$$24093:
	.skip 0xC
.global $$24094
$$24094:
	.skip 0xC
.global $$24095
$$24095:
	.skip 0xC
.global $$24096
$$24096:
	.skip 0xC
.global $$24097
$$24097:
	.skip 0xC
.global $$24098
$$24098:
	.skip 0xC
.global $$24099
$$24099:
	.skip 0xC
.global $$24100
$$24100:
	.skip 0xC
.global $$24101
$$24101:
	.skip 0xC
.global $$24102
$$24102:
	.skip 0xC
.global $$24103
$$24103:
	.skip 0xC
.global $$24104
$$24104:
	.skip 0xC
.global $$24105
$$24105:
	.skip 0xC
.global $$24106
$$24106:
	.skip 0xC
.global $$24107
$$24107:
	.skip 0x10
.global $$22790
$$22790:
	.skip 0xC
.global $$22791
$$22791:
	.skip 0xC
.global $$22792
$$22792:
	.skip 0xC
.global $$22793
$$22793:
	.skip 0xC
.global $$22794
$$22794:
	.skip 0xC
.global $$22795
$$22795:
	.skip 0xC
.global $$22796
$$22796:
	.skip 0xC
.global $$22797
$$22797:
	.skip 0xC
.global $$22798
$$22798:
	.skip 0xC
.global $$22799
$$22799:
	.skip 0xC
.global $$22800
$$22800:
	.skip 0xC
.global $$22801
$$22801:
	.skip 0xC
.global $$22802
$$22802:
	.skip 0xC
.global $$22803
$$22803:
	.skip 0xC
.global $$22804
$$22804:
	.skip 0x10
.global $$23453
$$23453:
	.skip 0xC
.global $$23454
$$23454:
	.skip 0xC
.global $$23455
$$23455:
	.skip 0xC
.global $$23456
$$23456:
	.skip 0xC
.global $$23457
$$23457:
	.skip 0xC
.global $$23458
$$23458:
	.skip 0xC
.global $$23459
$$23459:
	.skip 0xC
.global $$23460
$$23460:
	.skip 0xC
.global $$23461
$$23461:
	.skip 0xC
.global $$23462
$$23462:
	.skip 0xC
.global $$23463
$$23463:
	.skip 0xC
.global $$23464
$$23464:
	.skip 0xC
.global $$23465
$$23465:
	.skip 0xC
.global $$23466
$$23466:
	.skip 0xC
.global $$23467
$$23467:
	.skip 0x10
.global $$23214
$$23214:
	.skip 0xC
.global $$23215
$$23215:
	.skip 0xC
.global $$23216
$$23216:
	.skip 0xC
.global $$23217
$$23217:
	.skip 0xC
.global $$23218
$$23218:
	.skip 0xC
.global $$23219
$$23219:
	.skip 0xC
.global $$23220
$$23220:
	.skip 0xC
.global $$23221
$$23221:
	.skip 0xC
.global $$23222
$$23222:
	.skip 0xC
.global $$23223
$$23223:
	.skip 0xC
.global $$23224
$$23224:
	.skip 0xC
.global $$23225
$$23225:
	.skip 0xC
.global $$23226
$$23226:
	.skip 0xC
.global $$23227
$$23227:
	.skip 0xC
.global $$23228
$$23228:
	.skip 0x10
.global $$22152
$$22152:
	.skip 0xC
.global $$22153
$$22153:
	.skip 0xC
.global $$22154
$$22154:
	.skip 0xC
.global $$22155
$$22155:
	.skip 0xC
.global $$22156
$$22156:
	.skip 0xC
.global $$22157
$$22157:
	.skip 0xC
.global $$22158
$$22158:
	.skip 0xC
.global $$22159
$$22159:
	.skip 0xC
.global $$22160
$$22160:
	.skip 0xC
.global $$22161
$$22161:
	.skip 0xC
.global $$22162
$$22162:
	.skip 0xC
.global $$22163
$$22163:
	.skip 0xC
.global $$22164
$$22164:
	.skip 0xC
.global $$22165
$$22165:
	.skip 0xC
.global $$22166
$$22166:
	.skip 0x10
.global scale$2692
scale$2692:
	.skip 0xC
.global scale$2743
scale$2743:
	.skip 0xC
.global $$23608
$$23608:
	.skip 0xC
.global $$23609
$$23609:
	.skip 0xC
.global $$23610
$$23610:
	.skip 0xC
.global $$23611
$$23611:
	.skip 0xC
.global $$23612
$$23612:
	.skip 0xC
.global $$23613
$$23613:
	.skip 0xC
.global $$23614
$$23614:
	.skip 0xC
.global $$23615
$$23615:
	.skip 0xC
.global $$23616
$$23616:
	.skip 0xC
.global $$23617
$$23617:
	.skip 0xC
.global $$23618
$$23618:
	.skip 0xC
.global $$23619
$$23619:
	.skip 0xC
.global $$23620
$$23620:
	.skip 0xC
.global $$23621
$$23621:
	.skip 0xC
.global $$23622
$$23622:
	.skip 0x10
.global $$22827
$$22827:
	.skip 0xC
.global $$22828
$$22828:
	.skip 0xC
.global $$22829
$$22829:
	.skip 0xC
.global $$22830
$$22830:
	.skip 0xC
.global $$22831
$$22831:
	.skip 0xC
.global $$22832
$$22832:
	.skip 0xC
.global $$22833
$$22833:
	.skip 0xC
.global $$22834
$$22834:
	.skip 0xC
.global $$22835
$$22835:
	.skip 0xC
.global $$22836
$$22836:
	.skip 0xC
.global $$22837
$$22837:
	.skip 0xC
.global $$22838
$$22838:
	.skip 0xC
.global $$22839
$$22839:
	.skip 0xC
.global $$22840
$$22840:
	.skip 0xC
.global $$22841
$$22841:
	.skip 0x10
.global $$22161
$$22161:
	.skip 0xC
.global $$22162
$$22162:
	.skip 0xC
.global $$22163
$$22163:
	.skip 0xC
.global $$22164
$$22164:
	.skip 0xC
.global $$22165
$$22165:
	.skip 0xC
.global $$22166
$$22166:
	.skip 0xC
.global $$22167
$$22167:
	.skip 0xC
.global $$22168
$$22168:
	.skip 0xC
.global $$22169
$$22169:
	.skip 0xC
.global $$22170
$$22170:
	.skip 0xC
.global $$22171
$$22171:
	.skip 0xC
.global $$22172
$$22172:
	.skip 0xC
.global $$22173
$$22173:
	.skip 0xC
.global $$22174
$$22174:
	.skip 0xC
.global $$22175
$$22175:
	.skip 0x10
.global $$23097
$$23097:
	.skip 0xC
.global $$23098
$$23098:
	.skip 0xC
.global $$23099
$$23099:
	.skip 0xC
.global $$23100
$$23100:
	.skip 0xC
.global $$23101
$$23101:
	.skip 0xC
.global $$23102
$$23102:
	.skip 0xC
.global $$23103
$$23103:
	.skip 0xC
.global $$23104
$$23104:
	.skip 0xC
.global $$23105
$$23105:
	.skip 0xC
.global $$23106
$$23106:
	.skip 0xC
.global $$23107
$$23107:
	.skip 0xC
.global $$23108
$$23108:
	.skip 0xC
.global $$23109
$$23109:
	.skip 0xC
.global $$23110
$$23110:
	.skip 0xC
.global $$23111
$$23111:
	.skip 0x10
.global $$25981
$$25981:
	.skip 0xC
.global $$25982
$$25982:
	.skip 0xC
.global $$25983
$$25983:
	.skip 0xC
.global $$25984
$$25984:
	.skip 0xC
.global $$25985
$$25985:
	.skip 0xC
.global $$25986
$$25986:
	.skip 0xC
.global $$25987
$$25987:
	.skip 0xC
.global $$25988
$$25988:
	.skip 0xC
.global $$25989
$$25989:
	.skip 0xC
.global $$25990
$$25990:
	.skip 0xC
.global $$25991
$$25991:
	.skip 0xC
.global $$25992
$$25992:
	.skip 0xC
.global $$25993
$$25993:
	.skip 0xC
.global $$25994
$$25994:
	.skip 0xC
.global $$25995
$$25995:
	.skip 0x10
.global $$23165
$$23165:
	.skip 0xC
.global $$23166
$$23166:
	.skip 0xC
.global $$23167
$$23167:
	.skip 0xC
.global $$23168
$$23168:
	.skip 0xC
.global $$23169
$$23169:
	.skip 0xC
.global $$23170
$$23170:
	.skip 0xC
.global $$23171
$$23171:
	.skip 0xC
.global $$23172
$$23172:
	.skip 0xC
.global $$23173
$$23173:
	.skip 0xC
.global $$23174
$$23174:
	.skip 0xC
.global $$23175
$$23175:
	.skip 0xC
.global $$23176
$$23176:
	.skip 0xC
.global $$23177
$$23177:
	.skip 0xC
.global $$23178
$$23178:
	.skip 0xC
.global $$23179
$$23179:
	.skip 0x10
.global $$22980
$$22980:
	.skip 0xC
.global $$22981
$$22981:
	.skip 0xC
.global $$22982
$$22982:
	.skip 0xC
.global $$22983
$$22983:
	.skip 0xC
.global $$22984
$$22984:
	.skip 0xC
.global $$22985
$$22985:
	.skip 0xC
.global $$22986
$$22986:
	.skip 0xC
.global $$22987
$$22987:
	.skip 0xC
.global $$22988
$$22988:
	.skip 0xC
.global $$22989
$$22989:
	.skip 0xC
.global $$22990
$$22990:
	.skip 0xC
.global $$22991
$$22991:
	.skip 0xC
.global $$22992
$$22992:
	.skip 0xC
.global $$22993
$$22993:
	.skip 0xC
.global $$22994
$$22994:
	.skip 0x10
.global $$22840
$$22840:
	.skip 0xC
.global $$22841
$$22841:
	.skip 0xC
.global $$22842
$$22842:
	.skip 0xC
.global $$22843
$$22843:
	.skip 0xC
.global $$22844
$$22844:
	.skip 0xC
.global $$22845
$$22845:
	.skip 0xC
.global $$22846
$$22846:
	.skip 0xC
.global $$22847
$$22847:
	.skip 0xC
.global $$22848
$$22848:
	.skip 0xC
.global $$22849
$$22849:
	.skip 0xC
.global $$22850
$$22850:
	.skip 0xC
.global $$22851
$$22851:
	.skip 0xC
.global $$22852
$$22852:
	.skip 0xC
.global $$22853
$$22853:
	.skip 0xC
.global $$22854
$$22854:
	.skip 0x10
.global cDeformedTerrainCenter
cDeformedTerrainCenter:
	.skip 0xC
.global $$26586
$$26586:
	.skip 0xC
.global $$26587
$$26587:
	.skip 0xC
.global $$26588
$$26588:
	.skip 0xC
.global $$26589
$$26589:
	.skip 0xC
.global $$26590
$$26590:
	.skip 0xC
.global $$26591
$$26591:
	.skip 0xC
.global $$26592
$$26592:
	.skip 0xC
.global $$26593
$$26593:
	.skip 0xC
.global $$26594
$$26594:
	.skip 0xC
.global $$26595
$$26595:
	.skip 0xC
.global $$26596
$$26596:
	.skip 0xC
.global $$26597
$$26597:
	.skip 0xC
.global $$26598
$$26598:
	.skip 0xC
.global $$26599
$$26599:
	.skip 0xC
.global $$26600
$$26600:
	.skip 0xC
.global $$22664
$$22664:
	.skip 0xC
.global mStaticHitActor__18TModelWaterManager
mStaticHitActor__18TModelWaterManager:
	.skip 0x6C
.global wcheck$2854
wcheck$2854:
	.skip 0x2C
.global $$24749
$$24749:
	.skip 0xC
.global $$24750
$$24750:
	.skip 0xC
.global $$24751
$$24751:
	.skip 0xC
.global $$24752
$$24752:
	.skip 0xC
.global $$24753
$$24753:
	.skip 0xC
.global $$24754
$$24754:
	.skip 0xC
.global $$24755
$$24755:
	.skip 0xC
.global $$24756
$$24756:
	.skip 0xC
.global $$24757
$$24757:
	.skip 0xC
.global $$24758
$$24758:
	.skip 0xC
.global $$24759
$$24759:
	.skip 0xC
.global $$24760
$$24760:
	.skip 0xC
.global $$24761
$$24761:
	.skip 0xC
.global $$24762
$$24762:
	.skip 0xC
.global $$24763
$$24763:
	.skip 0xC
.global $$22806
$$22806:
	.skip 0xC
.global $$22807
$$22807:
	.skip 0xC
.global $$22808
$$22808:
	.skip 0xC
.global $$22809
$$22809:
	.skip 0xC
.global $$22810
$$22810:
	.skip 0xC
.global $$22811
$$22811:
	.skip 0xC
.global $$22812
$$22812:
	.skip 0xC
.global $$22813
$$22813:
	.skip 0xC
.global $$22814
$$22814:
	.skip 0xC
.global $$22815
$$22815:
	.skip 0xC
.global $$22816
$$22816:
	.skip 0xC
.global $$22817
$$22817:
	.skip 0xC
.global $$22818
$$22818:
	.skip 0xC
.global $$22819
$$22819:
	.skip 0xC
.global $$22820
$$22820:
	.skip 0x10
.global $$23257
$$23257:
	.skip 0xC
.global $$23258
$$23258:
	.skip 0xC
.global $$23259
$$23259:
	.skip 0xC
.global $$23260
$$23260:
	.skip 0xC
.global $$23261
$$23261:
	.skip 0xC
.global $$23262
$$23262:
	.skip 0xC
.global $$23263
$$23263:
	.skip 0xC
.global $$23264
$$23264:
	.skip 0xC
.global $$23265
$$23265:
	.skip 0xC
.global $$23266
$$23266:
	.skip 0xC
.global $$23267
$$23267:
	.skip 0xC
.global $$23268
$$23268:
	.skip 0xC
.global $$23269
$$23269:
	.skip 0xC
.global $$23270
$$23270:
	.skip 0xC
.global $$23271
$$23271:
	.skip 0x10
.global $$23002
$$23002:
	.skip 0xC
.global $$23003
$$23003:
	.skip 0xC
.global $$23004
$$23004:
	.skip 0xC
.global $$23005
$$23005:
	.skip 0xC
.global $$23006
$$23006:
	.skip 0xC
.global $$23007
$$23007:
	.skip 0xC
.global $$23008
$$23008:
	.skip 0xC
.global $$23009
$$23009:
	.skip 0xC
.global $$23010
$$23010:
	.skip 0xC
.global $$23011
$$23011:
	.skip 0xC
.global $$23012
$$23012:
	.skip 0xC
.global $$23013
$$23013:
	.skip 0xC
.global $$23014
$$23014:
	.skip 0xC
.global $$23015
$$23015:
	.skip 0xC
.global $$23016
$$23016:
	.skip 0x10
.global $$22936
$$22936:
	.skip 0xC
.global $$22937
$$22937:
	.skip 0xC
.global $$22938
$$22938:
	.skip 0xC
.global $$22939
$$22939:
	.skip 0xC
.global $$22940
$$22940:
	.skip 0xC
.global $$22941
$$22941:
	.skip 0xC
.global $$22942
$$22942:
	.skip 0xC
.global $$22943
$$22943:
	.skip 0xC
.global $$22944
$$22944:
	.skip 0xC
.global $$22945
$$22945:
	.skip 0xC
.global $$22946
$$22946:
	.skip 0xC
.global $$22947
$$22947:
	.skip 0xC
.global $$22948
$$22948:
	.skip 0xC
.global $$22949
$$22949:
	.skip 0xC
.global $$22950
$$22950:
	.skip 0x10
.global $$24339
$$24339:
	.skip 0xC
.global $$24340
$$24340:
	.skip 0xC
.global $$24341
$$24341:
	.skip 0xC
.global $$24342
$$24342:
	.skip 0xC
.global $$24343
$$24343:
	.skip 0xC
.global $$24344
$$24344:
	.skip 0xC
.global $$24345
$$24345:
	.skip 0xC
.global $$24346
$$24346:
	.skip 0xC
.global $$24347
$$24347:
	.skip 0xC
.global $$24348
$$24348:
	.skip 0xC
.global $$24349
$$24349:
	.skip 0xC
.global $$24350
$$24350:
	.skip 0xC
.global $$24351
$$24351:
	.skip 0xC
.global $$24352
$$24352:
	.skip 0xC
.global $$24353
$$24353:
	.skip 0x10
.global $$22213
$$22213:
	.skip 0xC
.global $$22244
$$22244:
	.skip 0xC
.global $$22252
$$22252:
	.skip 0xC
.global $$22265
$$22265:
	.skip 0xC
.global $$22276
$$22276:
	.skip 0xC
.global $$22304
$$22304:
	.skip 0xC
.global $$22350
$$22350:
	.skip 0xC
.global $$22360
$$22360:
	.skip 0xC
.global $$22366
$$22366:
	.skip 0xC
.global $$22374
$$22374:
	.skip 0xC
.global $$22385
$$22385:
	.skip 0xC
.global $$22391
$$22391:
	.skip 0xC
.global $$22405
$$22405:
	.skip 0xC
.global $$22415
$$22415:
	.skip 0xC
.global $$22425
$$22425:
	.skip 0xC
.global $$22437
$$22437:
	.skip 0xC
.global $$22640
$$22640:
	.skip 0xC
.global $$22641
$$22641:
	.skip 0xC
.global $$22642
$$22642:
	.skip 0xC
.global $$22643
$$22643:
	.skip 0xC
.global $$22644
$$22644:
	.skip 0xC
.global $$22645
$$22645:
	.skip 0xC
.global $$22646
$$22646:
	.skip 0xC
.global $$22647
$$22647:
	.skip 0xC
.global $$22648
$$22648:
	.skip 0xC
.global $$22649
$$22649:
	.skip 0xC
.global $$22650
$$22650:
	.skip 0xC
.global $$22651
$$22651:
	.skip 0xC
.global $$22652
$$22652:
	.skip 0xC
.global $$22653
$$22653:
	.skip 0xC
.global $$22654
$$22654:
	.skip 0x10
.global $$23044
$$23044:
	.skip 0xC
.global $$23045
$$23045:
	.skip 0xC
.global $$23046
$$23046:
	.skip 0xC
.global $$23047
$$23047:
	.skip 0xC
.global $$23048
$$23048:
	.skip 0xC
.global $$23049
$$23049:
	.skip 0xC
.global $$23050
$$23050:
	.skip 0xC
.global $$23051
$$23051:
	.skip 0xC
.global $$23052
$$23052:
	.skip 0xC
.global $$23053
$$23053:
	.skip 0xC
.global $$23054
$$23054:
	.skip 0xC
.global $$23055
$$23055:
	.skip 0xC
.global $$23056
$$23056:
	.skip 0xC
.global $$23057
$$23057:
	.skip 0xC
.global $$23058
$$23058:
	.skip 0x10
.global $$22377
$$22377:
	.skip 0xC
.global $$22378
$$22378:
	.skip 0xC
.global $$22379
$$22379:
	.skip 0xC
.global $$22380
$$22380:
	.skip 0xC
.global $$22381
$$22381:
	.skip 0xC
.global $$22382
$$22382:
	.skip 0xC
.global $$22383
$$22383:
	.skip 0xC
.global $$22384
$$22384:
	.skip 0xC
.global $$22385
$$22385:
	.skip 0xC
.global $$22386
$$22386:
	.skip 0xC
.global $$22387
$$22387:
	.skip 0xC
.global $$22388
$$22388:
	.skip 0xC
.global $$22389
$$22389:
	.skip 0xC
.global $$22390
$$22390:
	.skip 0xC
.global $$22391
$$22391:
	.skip 0x10
.global $$22044
$$22044:
	.skip 0xC
.global $$22045
$$22045:
	.skip 0xC
.global $$22046
$$22046:
	.skip 0xC
.global $$22047
$$22047:
	.skip 0xC
.global $$22048
$$22048:
	.skip 0xC
.global $$22049
$$22049:
	.skip 0xC
.global $$22050
$$22050:
	.skip 0xC
.global $$22051
$$22051:
	.skip 0xC
.global $$22052
$$22052:
	.skip 0xC
.global $$22053
$$22053:
	.skip 0xC
.global $$22054
$$22054:
	.skip 0xC
.global $$22055
$$22055:
	.skip 0xC
.global $$22056
$$22056:
	.skip 0xC
.global $$22057
$$22057:
	.skip 0xC
.global $$22058
$$22058:
	.skip 0x10
.global $$23758
$$23758:
	.skip 0xC
.global $$23759
$$23759:
	.skip 0xC
.global $$23760
$$23760:
	.skip 0xC
.global $$23761
$$23761:
	.skip 0xC
.global $$23762
$$23762:
	.skip 0xC
.global $$23763
$$23763:
	.skip 0xC
.global $$23764
$$23764:
	.skip 0xC
.global $$23765
$$23765:
	.skip 0xC
.global $$23766
$$23766:
	.skip 0xC
.global $$23767
$$23767:
	.skip 0xC
.global $$23768
$$23768:
	.skip 0xC
.global $$23769
$$23769:
	.skip 0xC
.global $$23770
$$23770:
	.skip 0xC
.global $$23771
$$23771:
	.skip 0xC
.global $$23772
$$23772:
	.skip 0x10
.global smBgmInTrack__5MSBgm
smBgmInTrack__5MSBgm:
	.skip 0xC
.global $$22048
$$22048:
	.skip 0xC
.global $$22049
$$22049:
	.skip 0xC
.global $$22050
$$22050:
	.skip 0xC
.global $$22051
$$22051:
	.skip 0xC
.global $$22052
$$22052:
	.skip 0xC
.global $$22053
$$22053:
	.skip 0xC
.global $$22054
$$22054:
	.skip 0xC
.global $$22055
$$22055:
	.skip 0xC
.global $$22056
$$22056:
	.skip 0xC
.global $$22057
$$22057:
	.skip 0xC
.global $$22058
$$22058:
	.skip 0xC
.global $$22059
$$22059:
	.skip 0xC
.global $$22060
$$22060:
	.skip 0xC
.global $$22061
$$22061:
	.skip 0xC
.global $$22062
$$22062:
	.skip 0xC
.global $$21951
$$21951:
	.skip 0xC
.global $$21952
$$21952:
	.skip 0xC
.global $$21953
$$21953:
	.skip 0xC
.global $$21954
$$21954:
	.skip 0xC
.global $$21955
$$21955:
	.skip 0xC
.global $$21956
$$21956:
	.skip 0xC
.global $$21957
$$21957:
	.skip 0xC
.global $$21958
$$21958:
	.skip 0xC
.global $$21959
$$21959:
	.skip 0xC
.global $$21960
$$21960:
	.skip 0xC
.global $$21961
$$21961:
	.skip 0xC
.global $$21962
$$21962:
	.skip 0xC
.global $$21963
$$21963:
	.skip 0xC
.global $$21964
$$21964:
	.skip 0xC
.global $$21965
$$21965:
	.skip 0x10
.global _posByCamera
_posByCamera:
	.skip 0xC00
.global $$22002
$$22002:
	.skip 0xC
.global $$22003
$$22003:
	.skip 0xC
.global $$22004
$$22004:
	.skip 0xC
.global $$22005
$$22005:
	.skip 0xC
.global $$22006
$$22006:
	.skip 0xC
.global $$22007
$$22007:
	.skip 0xC
.global $$22008
$$22008:
	.skip 0xC
.global $$22009
$$22009:
	.skip 0xC
.global $$22010
$$22010:
	.skip 0xC
.global $$22011
$$22011:
	.skip 0xC
.global $$22012
$$22012:
	.skip 0xC
.global $$22013
$$22013:
	.skip 0xC
.global $$22014
$$22014:
	.skip 0xC
.global $$22015
$$22015:
	.skip 0xC
.global $$22016
$$22016:
	.skip 0x10
.global $$21969
$$21969:
	.skip 0xC
.global smList__Q214MSoundSESystem9MSRandVol
smList__Q214MSoundSESystem9MSRandVol:
	.skip 0xC
.global $$21981
$$21981:
	.skip 0xC
.global smList__Q214MSoundSESystem10MSRandPlay
smList__Q214MSoundSESystem10MSRandPlay:
	.skip 0xC
.global $$23185
$$23185:
	.skip 0xC
.global $$23186
$$23186:
	.skip 0xC
.global $$23187
$$23187:
	.skip 0xC
.global $$23188
$$23188:
	.skip 0xC
.global $$23189
$$23189:
	.skip 0xC
.global $$23190
$$23190:
	.skip 0xC
.global $$23191
$$23191:
	.skip 0xC
.global $$23192
$$23192:
	.skip 0xC
.global $$23193
$$23193:
	.skip 0xC
.global $$23194
$$23194:
	.skip 0xC
.global $$23195
$$23195:
	.skip 0xC
.global $$23196
$$23196:
	.skip 0xC
.global $$23197
$$23197:
	.skip 0xC
.global $$23198
$$23198:
	.skip 0xC
.global $$23199
$$23199:
	.skip 0x10
.global smSetSound__10MSSetSound
smSetSound__10MSSetSound:
	.skip 0x24
.global $$21932
$$21932:
	.skip 0xC
.global $$21933
$$21933:
	.skip 0xC
.global $$21934
$$21934:
	.skip 0xC
.global $$21935
$$21935:
	.skip 0xC
.global $$21936
$$21936:
	.skip 0xC
.global $$21937
$$21937:
	.skip 0xC
.global $$21938
$$21938:
	.skip 0xC
.global $$21939
$$21939:
	.skip 0xC
.global $$21940
$$21940:
	.skip 0xC
.global $$21941
$$21941:
	.skip 0xC
.global $$21942
$$21942:
	.skip 0xC
.global $$21943
$$21943:
	.skip 0xC
.global $$21944
$$21944:
	.skip 0xC
.global $$21945
$$21945:
	.skip 0xC
.global $$21946
$$21946:
	.skip 0xC
.global $$22082
$$22082:
	.skip 0xC
.global $$22083
$$22083:
	.skip 0xC
.global $$22084
$$22084:
	.skip 0xC
.global $$22085
$$22085:
	.skip 0xC
.global $$22086
$$22086:
	.skip 0xC
.global $$22087
$$22087:
	.skip 0xC
.global $$22088
$$22088:
	.skip 0xC
.global $$22089
$$22089:
	.skip 0xC
.global $$22090
$$22090:
	.skip 0xC
.global $$22091
$$22091:
	.skip 0xC
.global $$22092
$$22092:
	.skip 0xC
.global $$22093
$$22093:
	.skip 0xC
.global $$22094
$$22094:
	.skip 0xC
.global $$22095
$$22095:
	.skip 0xC
.global $$22096
$$22096:
	.skip 0x10
.global $$21945
$$21945:
	.skip 0xC
.global $$21946
$$21946:
	.skip 0xC
.global $$21947
$$21947:
	.skip 0xC
.global $$21948
$$21948:
	.skip 0xC
.global $$21949
$$21949:
	.skip 0xC
.global $$21950
$$21950:
	.skip 0xC
.global $$21951
$$21951:
	.skip 0xC
.global $$21952
$$21952:
	.skip 0xC
.global $$21953
$$21953:
	.skip 0xC
.global $$21954
$$21954:
	.skip 0xC
.global $$21955
$$21955:
	.skip 0xC
.global $$21956
$$21956:
	.skip 0xC
.global $$21957
$$21957:
	.skip 0xC
.global $$21958
$$21958:
	.skip 0xC
.global $$21959
$$21959:
	.skip 0x10
.global $$22899
$$22899:
	.skip 0xC
.global $$22900
$$22900:
	.skip 0xC
.global $$22901
$$22901:
	.skip 0xC
.global $$22902
$$22902:
	.skip 0xC
.global $$22903
$$22903:
	.skip 0xC
.global $$22904
$$22904:
	.skip 0xC
.global $$22905
$$22905:
	.skip 0xC
.global $$22906
$$22906:
	.skip 0xC
.global $$22907
$$22907:
	.skip 0xC
.global $$22908
$$22908:
	.skip 0xC
.global $$22909
$$22909:
	.skip 0xC
.global $$22910
$$22910:
	.skip 0xC
.global $$22911
$$22911:
	.skip 0xC
.global $$22912
$$22912:
	.skip 0xC
.global $$22913
$$22913:
	.skip 0x10
.global $$23845
$$23845:
	.skip 0xC
.global $$23846
$$23846:
	.skip 0xC
.global $$23847
$$23847:
	.skip 0xC
.global $$23848
$$23848:
	.skip 0xC
.global $$23849
$$23849:
	.skip 0xC
.global $$23850
$$23850:
	.skip 0xC
.global $$23851
$$23851:
	.skip 0xC
.global $$23852
$$23852:
	.skip 0xC
.global $$23853
$$23853:
	.skip 0xC
.global $$23854
$$23854:
	.skip 0xC
.global $$23855
$$23855:
	.skip 0xC
.global $$23856
$$23856:
	.skip 0xC
.global $$23857
$$23857:
	.skip 0xC
.global $$23858
$$23858:
	.skip 0xC
.global $$23859
$$23859:
	.skip 0x10
.global $$24410
$$24410:
	.skip 0xC
.global $$24411
$$24411:
	.skip 0xC
.global $$24412
$$24412:
	.skip 0xC
.global $$24413
$$24413:
	.skip 0xC
.global $$24414
$$24414:
	.skip 0xC
.global $$24415
$$24415:
	.skip 0xC
.global $$24416
$$24416:
	.skip 0xC
.global $$24417
$$24417:
	.skip 0xC
.global $$24418
$$24418:
	.skip 0xC
.global $$24419
$$24419:
	.skip 0xC
.global $$24420
$$24420:
	.skip 0xC
.global $$24421
$$24421:
	.skip 0xC
.global $$24422
$$24422:
	.skip 0xC
.global $$24423
$$24423:
	.skip 0xC
.global $$24424
$$24424:
	.skip 0x10
.global $$23033
$$23033:
	.skip 0xC
.global $$23034
$$23034:
	.skip 0xC
.global $$23035
$$23035:
	.skip 0xC
.global $$23036
$$23036:
	.skip 0xC
.global $$23037
$$23037:
	.skip 0xC
.global $$23038
$$23038:
	.skip 0xC
.global $$23039
$$23039:
	.skip 0xC
.global $$23040
$$23040:
	.skip 0xC
.global $$23041
$$23041:
	.skip 0xC
.global $$23042
$$23042:
	.skip 0xC
.global $$23043
$$23043:
	.skip 0xC
.global $$23044
$$23044:
	.skip 0xC
.global $$23045
$$23045:
	.skip 0xC
.global $$23046
$$23046:
	.skip 0xC
.global $$23047
$$23047:
	.skip 0x10
.global $$24343
$$24343:
	.skip 0xC
.global $$24344
$$24344:
	.skip 0xC
.global $$24345
$$24345:
	.skip 0xC
.global $$24346
$$24346:
	.skip 0xC
.global $$24347
$$24347:
	.skip 0xC
.global $$24348
$$24348:
	.skip 0xC
.global $$24349
$$24349:
	.skip 0xC
.global $$24350
$$24350:
	.skip 0xC
.global $$24351
$$24351:
	.skip 0xC
.global $$24352
$$24352:
	.skip 0xC
.global $$24353
$$24353:
	.skip 0xC
.global $$24354
$$24354:
	.skip 0xC
.global $$24355
$$24355:
	.skip 0xC
.global $$24356
$$24356:
	.skip 0xC
.global $$24357
$$24357:
	.skip 0x10
.global $$24274
$$24274:
	.skip 0xC
.global $$24275
$$24275:
	.skip 0xC
.global $$24276
$$24276:
	.skip 0xC
.global $$24277
$$24277:
	.skip 0xC
.global $$24278
$$24278:
	.skip 0xC
.global $$24279
$$24279:
	.skip 0xC
.global $$24280
$$24280:
	.skip 0xC
.global $$24281
$$24281:
	.skip 0xC
.global $$24282
$$24282:
	.skip 0xC
.global $$24283
$$24283:
	.skip 0xC
.global $$24284
$$24284:
	.skip 0xC
.global $$24285
$$24285:
	.skip 0xC
.global $$24286
$$24286:
	.skip 0xC
.global $$24287
$$24287:
	.skip 0xC
.global $$24288
$$24288:
	.skip 0x10
.global $$24856
$$24856:
	.skip 0xC
.global $$24857
$$24857:
	.skip 0xC
.global $$24858
$$24858:
	.skip 0xC
.global $$24859
$$24859:
	.skip 0xC
.global $$24860
$$24860:
	.skip 0xC
.global $$24861
$$24861:
	.skip 0xC
.global $$24862
$$24862:
	.skip 0xC
.global $$24863
$$24863:
	.skip 0xC
.global $$24864
$$24864:
	.skip 0xC
.global $$24865
$$24865:
	.skip 0xC
.global $$24866
$$24866:
	.skip 0xC
.global $$24867
$$24867:
	.skip 0xC
.global $$24868
$$24868:
	.skip 0xC
.global $$24869
$$24869:
	.skip 0xC
.global $$24870
$$24870:
	.skip 0x10
.global $$22778
$$22778:
	.skip 0xC
.global $$22779
$$22779:
	.skip 0xC
.global $$22780
$$22780:
	.skip 0xC
.global $$22781
$$22781:
	.skip 0xC
.global $$22782
$$22782:
	.skip 0xC
.global $$22783
$$22783:
	.skip 0xC
.global $$22784
$$22784:
	.skip 0xC
.global $$22785
$$22785:
	.skip 0xC
.global $$22786
$$22786:
	.skip 0xC
.global $$22787
$$22787:
	.skip 0xC
.global $$22788
$$22788:
	.skip 0xC
.global $$22789
$$22789:
	.skip 0xC
.global $$22790
$$22790:
	.skip 0xC
.global $$22791
$$22791:
	.skip 0xC
.global $$22792
$$22792:
	.skip 0x10
.global $$23821
$$23821:
	.skip 0xC
.global $$23822
$$23822:
	.skip 0xC
.global $$23823
$$23823:
	.skip 0xC
.global $$23824
$$23824:
	.skip 0xC
.global $$23825
$$23825:
	.skip 0xC
.global $$23826
$$23826:
	.skip 0xC
.global $$23827
$$23827:
	.skip 0xC
.global $$23828
$$23828:
	.skip 0xC
.global $$23829
$$23829:
	.skip 0xC
.global $$23830
$$23830:
	.skip 0xC
.global $$23831
$$23831:
	.skip 0xC
.global $$23832
$$23832:
	.skip 0xC
.global $$23833
$$23833:
	.skip 0xC
.global $$23834
$$23834:
	.skip 0xC
.global $$23835
$$23835:
	.skip 0x10
.global $$23395
$$23395:
	.skip 0xC
.global $$23396
$$23396:
	.skip 0xC
.global $$23397
$$23397:
	.skip 0xC
.global $$23398
$$23398:
	.skip 0xC
.global $$23399
$$23399:
	.skip 0xC
.global $$23400
$$23400:
	.skip 0xC
.global $$23401
$$23401:
	.skip 0xC
.global $$23402
$$23402:
	.skip 0xC
.global $$23403
$$23403:
	.skip 0xC
.global $$23404
$$23404:
	.skip 0xC
.global $$23405
$$23405:
	.skip 0xC
.global $$23406
$$23406:
	.skip 0xC
.global $$23407
$$23407:
	.skip 0xC
.global $$23408
$$23408:
	.skip 0xC
.global $$23409
$$23409:
	.skip 0x10
.global $$24393
$$24393:
	.skip 0xC
.global $$24394
$$24394:
	.skip 0xC
.global $$24395
$$24395:
	.skip 0xC
.global $$24396
$$24396:
	.skip 0xC
.global $$24397
$$24397:
	.skip 0xC
.global $$24398
$$24398:
	.skip 0xC
.global $$24399
$$24399:
	.skip 0xC
.global $$24400
$$24400:
	.skip 0xC
.global $$24401
$$24401:
	.skip 0xC
.global $$24402
$$24402:
	.skip 0xC
.global $$24403
$$24403:
	.skip 0xC
.global $$24404
$$24404:
	.skip 0xC
.global $$24405
$$24405:
	.skip 0xC
.global $$24406
$$24406:
	.skip 0xC
.global $$24407
$$24407:
	.skip 0x10
.global $$25259
$$25259:
	.skip 0xC
.global $$25260
$$25260:
	.skip 0xC
.global $$25261
$$25261:
	.skip 0xC
.global $$25262
$$25262:
	.skip 0xC
.global $$25263
$$25263:
	.skip 0xC
.global $$25264
$$25264:
	.skip 0xC
.global $$25265
$$25265:
	.skip 0xC
.global $$25266
$$25266:
	.skip 0xC
.global $$25267
$$25267:
	.skip 0xC
.global $$25268
$$25268:
	.skip 0xC
.global $$25269
$$25269:
	.skip 0xC
.global $$25270
$$25270:
	.skip 0xC
.global $$25271
$$25271:
	.skip 0xC
.global $$25272
$$25272:
	.skip 0xC
.global $$25273
$$25273:
	.skip 0x10
.global submarineCranePos_forSound
submarineCranePos_forSound:
	.skip 0xC
.global submarineSetWtPos_forSound
submarineSetWtPos_forSound:
	.skip 0xC
.global $$23529
$$23529:
	.skip 0xC
.global $$23530
$$23530:
	.skip 0xC
.global $$23531
$$23531:
	.skip 0xC
.global $$23532
$$23532:
	.skip 0xC
.global $$23533
$$23533:
	.skip 0xC
.global $$23534
$$23534:
	.skip 0xC
.global $$23535
$$23535:
	.skip 0xC
.global $$23536
$$23536:
	.skip 0xC
.global $$23537
$$23537:
	.skip 0xC
.global $$23538
$$23538:
	.skip 0xC
.global $$23539
$$23539:
	.skip 0xC
.global $$23540
$$23540:
	.skip 0xC
.global $$23541
$$23541:
	.skip 0xC
.global $$23542
$$23542:
	.skip 0xC
.global $$23543
$$23543:
	.skip 0x10
.global $$25417
$$25417:
	.skip 0xC
.global $$25418
$$25418:
	.skip 0xC
.global $$25419
$$25419:
	.skip 0xC
.global $$25420
$$25420:
	.skip 0xC
.global $$25421
$$25421:
	.skip 0xC
.global $$25422
$$25422:
	.skip 0xC
.global $$25423
$$25423:
	.skip 0xC
.global $$25424
$$25424:
	.skip 0xC
.global $$25425
$$25425:
	.skip 0xC
.global $$25426
$$25426:
	.skip 0xC
.global $$25427
$$25427:
	.skip 0xC
.global $$25428
$$25428:
	.skip 0xC
.global $$25429
$$25429:
	.skip 0xC
.global $$25430
$$25430:
	.skip 0xC
.global $$25431
$$25431:
	.skip 0x10
.global $$24378
$$24378:
	.skip 0xC
.global $$24379
$$24379:
	.skip 0xC
.global $$24380
$$24380:
	.skip 0xC
.global $$24381
$$24381:
	.skip 0xC
.global $$24382
$$24382:
	.skip 0xC
.global $$24383
$$24383:
	.skip 0xC
.global $$24384
$$24384:
	.skip 0xC
.global $$24385
$$24385:
	.skip 0xC
.global $$24386
$$24386:
	.skip 0xC
.global $$24387
$$24387:
	.skip 0xC
.global $$24388
$$24388:
	.skip 0xC
.global $$24389
$$24389:
	.skip 0xC
.global $$24390
$$24390:
	.skip 0xC
.global $$24391
$$24391:
	.skip 0xC
.global $$24392
$$24392:
	.skip 0x10
.global $$22483
$$22483:
	.skip 0xC
.global $$22484
$$22484:
	.skip 0xC
.global $$22485
$$22485:
	.skip 0xC
.global $$22486
$$22486:
	.skip 0xC
.global $$22487
$$22487:
	.skip 0xC
.global $$22488
$$22488:
	.skip 0xC
.global $$22489
$$22489:
	.skip 0xC
.global $$22490
$$22490:
	.skip 0xC
.global $$22491
$$22491:
	.skip 0xC
.global $$22492
$$22492:
	.skip 0xC
.global $$22493
$$22493:
	.skip 0xC
.global $$22494
$$22494:
	.skip 0xC
.global $$22495
$$22495:
	.skip 0xC
.global $$22496
$$22496:
	.skip 0xC
.global $$22497
$$22497:
	.skip 0x10
.global fall_upper_pos
fall_upper_pos:
	.skip 0xC
.global $$24401
$$24401:
	.skip 0xC
.global $$24402
$$24402:
	.skip 0xC
.global $$24403
$$24403:
	.skip 0xC
.global $$24404
$$24404:
	.skip 0xC
.global $$24405
$$24405:
	.skip 0xC
.global $$24406
$$24406:
	.skip 0xC
.global $$24407
$$24407:
	.skip 0xC
.global $$24408
$$24408:
	.skip 0xC
.global $$24409
$$24409:
	.skip 0xC
.global $$24410
$$24410:
	.skip 0xC
.global $$24411
$$24411:
	.skip 0xC
.global $$24412
$$24412:
	.skip 0xC
.global $$24413
$$24413:
	.skip 0xC
.global $$24414
$$24414:
	.skip 0xC
.global $$24415
$$24415:
	.skip 0xC
.global $$22827
$$22827:
	.skip 0xC
.global $$22828
$$22828:
	.skip 0xC
.global $$22829
$$22829:
	.skip 0xC
.global $$22830
$$22830:
	.skip 0xC
.global $$22831
$$22831:
	.skip 0xC
.global $$22832
$$22832:
	.skip 0xC
.global $$22833
$$22833:
	.skip 0xC
.global $$22834
$$22834:
	.skip 0xC
.global $$22835
$$22835:
	.skip 0xC
.global $$22836
$$22836:
	.skip 0xC
.global $$22837
$$22837:
	.skip 0xC
.global $$22838
$$22838:
	.skip 0xC
.global $$22839
$$22839:
	.skip 0xC
.global $$22840
$$22840:
	.skip 0xC
.global $$22841
$$22841:
	.skip 0x10
.global $$22971
$$22971:
	.skip 0xC
.global $$22972
$$22972:
	.skip 0xC
.global $$22973
$$22973:
	.skip 0xC
.global $$22974
$$22974:
	.skip 0xC
.global $$22975
$$22975:
	.skip 0xC
.global $$22976
$$22976:
	.skip 0xC
.global $$22977
$$22977:
	.skip 0xC
.global $$22978
$$22978:
	.skip 0xC
.global $$22979
$$22979:
	.skip 0xC
.global $$22980
$$22980:
	.skip 0xC
.global $$22981
$$22981:
	.skip 0xC
.global $$22982
$$22982:
	.skip 0xC
.global $$22983
$$22983:
	.skip 0xC
.global $$22984
$$22984:
	.skip 0xC
.global $$22985
$$22985:
	.skip 0x10
.global $$22557
$$22557:
	.skip 0xC
.global $$22558
$$22558:
	.skip 0xC
.global $$22559
$$22559:
	.skip 0xC
.global $$22560
$$22560:
	.skip 0xC
.global $$22561
$$22561:
	.skip 0xC
.global $$22562
$$22562:
	.skip 0xC
.global $$22563
$$22563:
	.skip 0xC
.global $$22564
$$22564:
	.skip 0xC
.global $$22565
$$22565:
	.skip 0xC
.global $$22566
$$22566:
	.skip 0xC
.global $$22567
$$22567:
	.skip 0xC
.global $$22568
$$22568:
	.skip 0xC
.global $$22569
$$22569:
	.skip 0xC
.global $$22570
$$22570:
	.skip 0xC
.global $$22571
$$22571:
	.skip 0x10
.global $$23130
$$23130:
	.skip 0xC
.global $$23131
$$23131:
	.skip 0xC
.global $$23132
$$23132:
	.skip 0xC
.global $$23133
$$23133:
	.skip 0xC
.global $$23134
$$23134:
	.skip 0xC
.global $$23135
$$23135:
	.skip 0xC
.global $$23136
$$23136:
	.skip 0xC
.global $$23137
$$23137:
	.skip 0xC
.global $$23138
$$23138:
	.skip 0xC
.global $$23139
$$23139:
	.skip 0xC
.global $$23140
$$23140:
	.skip 0xC
.global $$23141
$$23141:
	.skip 0xC
.global $$23142
$$23142:
	.skip 0xC
.global $$23143
$$23143:
	.skip 0xC
.global $$23144
$$23144:
	.skip 0x10
.global $$23023
$$23023:
	.skip 0xC
.global $$23024
$$23024:
	.skip 0xC
.global $$23025
$$23025:
	.skip 0xC
.global $$23026
$$23026:
	.skip 0xC
.global $$23027
$$23027:
	.skip 0xC
.global $$23028
$$23028:
	.skip 0xC
.global $$23029
$$23029:
	.skip 0xC
.global $$23030
$$23030:
	.skip 0xC
.global $$23031
$$23031:
	.skip 0xC
.global $$23032
$$23032:
	.skip 0xC
.global $$23033
$$23033:
	.skip 0xC
.global $$23034
$$23034:
	.skip 0xC
.global $$23035
$$23035:
	.skip 0xC
.global $$23036
$$23036:
	.skip 0xC
.global $$23037
$$23037:
	.skip 0x10
.global $$25478
$$25478:
	.skip 0xC
.global $$25479
$$25479:
	.skip 0xC
.global $$25480
$$25480:
	.skip 0xC
.global $$25481
$$25481:
	.skip 0xC
.global $$25482
$$25482:
	.skip 0xC
.global $$25483
$$25483:
	.skip 0xC
.global $$25484
$$25484:
	.skip 0xC
.global $$25485
$$25485:
	.skip 0xC
.global $$25486
$$25486:
	.skip 0xC
.global $$25487
$$25487:
	.skip 0xC
.global $$25488
$$25488:
	.skip 0xC
.global $$25489
$$25489:
	.skip 0xC
.global $$25490
$$25490:
	.skip 0xC
.global $$25491
$$25491:
	.skip 0xC
.global $$25492
$$25492:
	.skip 0x10
.global $$22992
$$22992:
	.skip 0xC
.global $$22993
$$22993:
	.skip 0xC
.global $$22994
$$22994:
	.skip 0xC
.global $$22995
$$22995:
	.skip 0xC
.global $$22996
$$22996:
	.skip 0xC
.global $$22997
$$22997:
	.skip 0xC
.global $$22998
$$22998:
	.skip 0xC
.global $$22999
$$22999:
	.skip 0xC
.global $$23000
$$23000:
	.skip 0xC
.global $$23001
$$23001:
	.skip 0xC
.global $$23002
$$23002:
	.skip 0xC
.global $$23003
$$23003:
	.skip 0xC
.global $$23004
$$23004:
	.skip 0xC
.global $$23005
$$23005:
	.skip 0xC
.global $$23006
$$23006:
	.skip 0x10
.global up$2645
up$2645:
	.skip 0xC
.global up$2710
up$2710:
	.skip 0xC
.global $$23395
$$23395:
	.skip 0xC
.global $$23396
$$23396:
	.skip 0xC
.global $$23397
$$23397:
	.skip 0xC
.global $$23398
$$23398:
	.skip 0xC
.global $$23399
$$23399:
	.skip 0xC
.global $$23400
$$23400:
	.skip 0xC
.global $$23401
$$23401:
	.skip 0xC
.global $$23402
$$23402:
	.skip 0xC
.global $$23403
$$23403:
	.skip 0xC
.global $$23404
$$23404:
	.skip 0xC
.global $$23405
$$23405:
	.skip 0xC
.global $$23406
$$23406:
	.skip 0xC
.global $$23407
$$23407:
	.skip 0xC
.global $$23408
$$23408:
	.skip 0xC
.global $$23409
$$23409:
	.skip 0x10
.global $$22737
$$22737:
	.skip 0xC
.global $$22738
$$22738:
	.skip 0xC
.global $$22739
$$22739:
	.skip 0xC
.global $$22740
$$22740:
	.skip 0xC
.global $$22741
$$22741:
	.skip 0xC
.global $$22742
$$22742:
	.skip 0xC
.global $$22743
$$22743:
	.skip 0xC
.global $$22744
$$22744:
	.skip 0xC
.global $$22745
$$22745:
	.skip 0xC
.global $$22746
$$22746:
	.skip 0xC
.global $$22747
$$22747:
	.skip 0xC
.global $$22748
$$22748:
	.skip 0xC
.global $$22749
$$22749:
	.skip 0xC
.global $$22750
$$22750:
	.skip 0xC
.global $$22751
$$22751:
	.skip 0x10
.global mDrawVec__19TMapObjGrassManager
mDrawVec__19TMapObjGrassManager:
	.skip 0xC
.global $$22726
$$22726:
	.skip 0xC
.global $$22727
$$22727:
	.skip 0xC
.global $$22728
$$22728:
	.skip 0xC
.global $$22729
$$22729:
	.skip 0xC
.global $$22730
$$22730:
	.skip 0xC
.global $$22731
$$22731:
	.skip 0xC
.global $$22732
$$22732:
	.skip 0xC
.global $$22733
$$22733:
	.skip 0xC
.global $$22734
$$22734:
	.skip 0xC
.global $$22735
$$22735:
	.skip 0xC
.global $$22736
$$22736:
	.skip 0xC
.global $$22737
$$22737:
	.skip 0xC
.global $$22738
$$22738:
	.skip 0xC
.global $$22739
$$22739:
	.skip 0xC
.global $$22740
$$22740:
	.skip 0xC
.global $$22311
$$22311:
	.skip 0xC
.global $$22312
$$22312:
	.skip 0xC
.global $$22313
$$22313:
	.skip 0xC
.global $$22314
$$22314:
	.skip 0xC
.global $$22315
$$22315:
	.skip 0xC
.global $$22316
$$22316:
	.skip 0xC
.global $$22317
$$22317:
	.skip 0xC
.global $$22318
$$22318:
	.skip 0xC
.global $$22319
$$22319:
	.skip 0xC
.global $$22320
$$22320:
	.skip 0xC
.global $$22321
$$22321:
	.skip 0xC
.global $$22322
$$22322:
	.skip 0xC
.global $$22323
$$22323:
	.skip 0xC
.global $$22324
$$22324:
	.skip 0xC
.global $$22325
$$22325:
	.skip 0x10
.global $$22811
$$22811:
	.skip 0xC
.global $$22812
$$22812:
	.skip 0xC
.global $$22813
$$22813:
	.skip 0xC
.global $$22814
$$22814:
	.skip 0xC
.global $$22815
$$22815:
	.skip 0xC
.global $$22816
$$22816:
	.skip 0xC
.global $$22817
$$22817:
	.skip 0xC
.global $$22818
$$22818:
	.skip 0xC
.global $$22819
$$22819:
	.skip 0xC
.global $$22820
$$22820:
	.skip 0xC
.global $$22821
$$22821:
	.skip 0xC
.global $$22822
$$22822:
	.skip 0xC
.global $$22823
$$22823:
	.skip 0xC
.global $$22824
$$22824:
	.skip 0xC
.global $$22825
$$22825:
	.skip 0x10
.global $$23423
$$23423:
	.skip 0xC
.global $$23424
$$23424:
	.skip 0xC
.global $$23425
$$23425:
	.skip 0xC
.global $$23426
$$23426:
	.skip 0xC
.global $$23427
$$23427:
	.skip 0xC
.global $$23428
$$23428:
	.skip 0xC
.global $$23429
$$23429:
	.skip 0xC
.global $$23430
$$23430:
	.skip 0xC
.global $$23431
$$23431:
	.skip 0xC
.global $$23432
$$23432:
	.skip 0xC
.global $$23433
$$23433:
	.skip 0xC
.global $$23434
$$23434:
	.skip 0xC
.global $$23435
$$23435:
	.skip 0xC
.global $$23436
$$23436:
	.skip 0xC
.global $$23437
$$23437:
	.skip 0x10
.global $$23789
$$23789:
	.skip 0xC
.global $$23790
$$23790:
	.skip 0xC
.global $$23791
$$23791:
	.skip 0xC
.global $$23792
$$23792:
	.skip 0xC
.global $$23793
$$23793:
	.skip 0xC
.global $$23794
$$23794:
	.skip 0xC
.global $$23795
$$23795:
	.skip 0xC
.global $$23796
$$23796:
	.skip 0xC
.global $$23797
$$23797:
	.skip 0xC
.global $$23798
$$23798:
	.skip 0xC
.global $$23799
$$23799:
	.skip 0xC
.global $$23800
$$23800:
	.skip 0xC
.global $$23801
$$23801:
	.skip 0xC
.global $$23802
$$23802:
	.skip 0xC
.global $$23803
$$23803:
	.skip 0x10
.global $$22895
$$22895:
	.skip 0xC
.global $$22896
$$22896:
	.skip 0xC
.global $$22897
$$22897:
	.skip 0xC
.global $$22898
$$22898:
	.skip 0xC
.global $$22899
$$22899:
	.skip 0xC
.global $$22900
$$22900:
	.skip 0xC
.global $$22901
$$22901:
	.skip 0xC
.global $$22902
$$22902:
	.skip 0xC
.global $$22903
$$22903:
	.skip 0xC
.global $$22904
$$22904:
	.skip 0xC
.global $$22905
$$22905:
	.skip 0xC
.global $$22906
$$22906:
	.skip 0xC
.global $$22907
$$22907:
	.skip 0xC
.global $$22908
$$22908:
	.skip 0xC
.global $$22909
$$22909:
	.skip 0x10
.global $$24475
$$24475:
	.skip 0xC
.global $$24476
$$24476:
	.skip 0xC
.global $$24477
$$24477:
	.skip 0xC
.global $$24478
$$24478:
	.skip 0xC
.global $$24479
$$24479:
	.skip 0xC
.global $$24480
$$24480:
	.skip 0xC
.global $$24481
$$24481:
	.skip 0xC
.global $$24482
$$24482:
	.skip 0xC
.global $$24483
$$24483:
	.skip 0xC
.global $$24484
$$24484:
	.skip 0xC
.global $$24485
$$24485:
	.skip 0xC
.global $$24486
$$24486:
	.skip 0xC
.global $$24487
$$24487:
	.skip 0xC
.global $$24488
$$24488:
	.skip 0xC
.global $$24489
$$24489:
	.skip 0x10
.global $$23303
$$23303:
	.skip 0xC
.global $$23304
$$23304:
	.skip 0xC
.global $$23305
$$23305:
	.skip 0xC
.global $$23306
$$23306:
	.skip 0xC
.global $$23307
$$23307:
	.skip 0xC
.global $$23308
$$23308:
	.skip 0xC
.global $$23309
$$23309:
	.skip 0xC
.global $$23310
$$23310:
	.skip 0xC
.global $$23311
$$23311:
	.skip 0xC
.global $$23312
$$23312:
	.skip 0xC
.global $$23313
$$23313:
	.skip 0xC
.global $$23314
$$23314:
	.skip 0xC
.global $$23315
$$23315:
	.skip 0xC
.global $$23316
$$23316:
	.skip 0xC
.global $$23317
$$23317:
	.skip 0x10
.global $$22855
$$22855:
	.skip 0xC
.global $$22856
$$22856:
	.skip 0xC
.global $$22857
$$22857:
	.skip 0xC
.global $$22858
$$22858:
	.skip 0xC
.global $$22859
$$22859:
	.skip 0xC
.global $$22860
$$22860:
	.skip 0xC
.global $$22861
$$22861:
	.skip 0xC
.global $$22862
$$22862:
	.skip 0xC
.global $$22863
$$22863:
	.skip 0xC
.global $$22864
$$22864:
	.skip 0xC
.global $$22865
$$22865:
	.skip 0xC
.global $$22866
$$22866:
	.skip 0xC
.global $$22867
$$22867:
	.skip 0xC
.global $$22868
$$22868:
	.skip 0xC
.global $$22869
$$22869:
	.skip 0x10
.global $$22809
$$22809:
	.skip 0xC
.global $$22810
$$22810:
	.skip 0xC
.global $$22811
$$22811:
	.skip 0xC
.global $$22812
$$22812:
	.skip 0xC
.global $$22813
$$22813:
	.skip 0xC
.global $$22814
$$22814:
	.skip 0xC
.global $$22815
$$22815:
	.skip 0xC
.global $$22816
$$22816:
	.skip 0xC
.global $$22817
$$22817:
	.skip 0xC
.global $$22818
$$22818:
	.skip 0xC
.global $$22819
$$22819:
	.skip 0xC
.global $$22820
$$22820:
	.skip 0xC
.global $$22821
$$22821:
	.skip 0xC
.global $$22822
$$22822:
	.skip 0xC
.global $$22823
$$22823:
	.skip 0x10
.global $$22782
$$22782:
	.skip 0xC
.global $$22783
$$22783:
	.skip 0xC
.global $$22784
$$22784:
	.skip 0xC
.global $$22785
$$22785:
	.skip 0xC
.global $$22786
$$22786:
	.skip 0xC
.global $$22787
$$22787:
	.skip 0xC
.global $$22788
$$22788:
	.skip 0xC
.global $$22789
$$22789:
	.skip 0xC
.global $$22790
$$22790:
	.skip 0xC
.global $$22791
$$22791:
	.skip 0xC
.global $$22792
$$22792:
	.skip 0xC
.global $$22793
$$22793:
	.skip 0xC
.global $$22794
$$22794:
	.skip 0xC
.global $$22795
$$22795:
	.skip 0xC
.global $$22796
$$22796:
	.skip 0x10
.global yDown$3091
yDown$3091:
	.skip 0xC
.global y$3288
y$3288:
	.skip 0xC
.global $$26029
$$26029:
	.skip 0xC
.global $$26030
$$26030:
	.skip 0xC
.global $$26031
$$26031:
	.skip 0xC
.global $$26032
$$26032:
	.skip 0xC
.global $$26033
$$26033:
	.skip 0xC
.global $$26034
$$26034:
	.skip 0xC
.global $$26035
$$26035:
	.skip 0xC
.global $$26036
$$26036:
	.skip 0xC
.global $$26037
$$26037:
	.skip 0xC
.global $$26038
$$26038:
	.skip 0xC
.global $$26039
$$26039:
	.skip 0xC
.global $$26040
$$26040:
	.skip 0xC
.global $$26041
$$26041:
	.skip 0xC
.global $$26042
$$26042:
	.skip 0xC
.global $$26043
$$26043:
	.skip 0x10
.global $$23377
$$23377:
	.skip 0xC
.global $$23378
$$23378:
	.skip 0xC
.global $$23379
$$23379:
	.skip 0xC
.global $$23380
$$23380:
	.skip 0xC
.global $$23381
$$23381:
	.skip 0xC
.global $$23382
$$23382:
	.skip 0xC
.global $$23383
$$23383:
	.skip 0xC
.global $$23384
$$23384:
	.skip 0xC
.global $$23385
$$23385:
	.skip 0xC
.global $$23386
$$23386:
	.skip 0xC
.global $$23387
$$23387:
	.skip 0xC
.global $$23388
$$23388:
	.skip 0xC
.global $$23389
$$23389:
	.skip 0xC
.global $$23390
$$23390:
	.skip 0xC
.global $$23391
$$23391:
	.skip 0x10
.global $$23950
$$23950:
	.skip 0xC
.global $$23951
$$23951:
	.skip 0xC
.global $$23952
$$23952:
	.skip 0xC
.global $$23953
$$23953:
	.skip 0xC
.global $$23954
$$23954:
	.skip 0xC
.global $$23955
$$23955:
	.skip 0xC
.global $$23956
$$23956:
	.skip 0xC
.global $$23957
$$23957:
	.skip 0xC
.global $$23958
$$23958:
	.skip 0xC
.global $$23959
$$23959:
	.skip 0xC
.global $$23960
$$23960:
	.skip 0xC
.global $$23961
$$23961:
	.skip 0xC
.global $$23962
$$23962:
	.skip 0xC
.global $$23963
$$23963:
	.skip 0xC
.global $$23964
$$23964:
	.skip 0x10
.global $$23256
$$23256:
	.skip 0xC
.global $$23257
$$23257:
	.skip 0xC
.global $$23258
$$23258:
	.skip 0xC
.global $$23259
$$23259:
	.skip 0xC
.global $$23260
$$23260:
	.skip 0xC
.global $$23261
$$23261:
	.skip 0xC
.global $$23262
$$23262:
	.skip 0xC
.global $$23263
$$23263:
	.skip 0xC
.global $$23264
$$23264:
	.skip 0xC
.global $$23265
$$23265:
	.skip 0xC
.global $$23266
$$23266:
	.skip 0xC
.global $$23267
$$23267:
	.skip 0xC
.global $$23268
$$23268:
	.skip 0xC
.global $$23269
$$23269:
	.skip 0xC
.global $$23270
$$23270:
	.skip 0x10
.global $$22331
$$22331:
	.skip 0xC
.global $$22332
$$22332:
	.skip 0xC
.global $$22333
$$22333:
	.skip 0xC
.global $$22334
$$22334:
	.skip 0xC
.global $$22335
$$22335:
	.skip 0xC
.global $$22336
$$22336:
	.skip 0xC
.global $$22337
$$22337:
	.skip 0xC
.global $$22338
$$22338:
	.skip 0xC
.global $$22339
$$22339:
	.skip 0xC
.global $$22340
$$22340:
	.skip 0xC
.global $$22341
$$22341:
	.skip 0xC
.global $$22342
$$22342:
	.skip 0xC
.global $$22343
$$22343:
	.skip 0xC
.global $$22344
$$22344:
	.skip 0xC
.global $$22345
$$22345:
	.skip 0x10
.global $$22405
$$22405:
	.skip 0xC
.global $$22406
$$22406:
	.skip 0xC
.global $$22407
$$22407:
	.skip 0xC
.global $$22408
$$22408:
	.skip 0xC
.global $$22409
$$22409:
	.skip 0xC
.global $$22410
$$22410:
	.skip 0xC
.global $$22411
$$22411:
	.skip 0xC
.global $$22412
$$22412:
	.skip 0xC
.global $$22413
$$22413:
	.skip 0xC
.global $$22414
$$22414:
	.skip 0xC
.global $$22415
$$22415:
	.skip 0xC
.global $$22416
$$22416:
	.skip 0xC
.global $$22417
$$22417:
	.skip 0xC
.global $$22418
$$22418:
	.skip 0xC
.global $$22419
$$22419:
	.skip 0x10
.global $$22519
$$22519:
	.skip 0xC
.global $$22520
$$22520:
	.skip 0xC
.global $$22521
$$22521:
	.skip 0xC
.global $$22522
$$22522:
	.skip 0xC
.global $$22523
$$22523:
	.skip 0xC
.global $$22524
$$22524:
	.skip 0xC
.global $$22525
$$22525:
	.skip 0xC
.global $$22526
$$22526:
	.skip 0xC
.global $$22527
$$22527:
	.skip 0xC
.global $$22528
$$22528:
	.skip 0xC
.global $$22529
$$22529:
	.skip 0xC
.global $$22530
$$22530:
	.skip 0xC
.global $$22531
$$22531:
	.skip 0xC
.global $$22532
$$22532:
	.skip 0xC
.global $$22533
$$22533:
	.skip 0x10
.global pos$2895
pos$2895:
	.skip 0xC
.global $$23812
$$23812:
	.skip 0xC
.global $$23813
$$23813:
	.skip 0xC
.global $$23814
$$23814:
	.skip 0xC
.global $$23815
$$23815:
	.skip 0xC
.global $$23816
$$23816:
	.skip 0xC
.global $$23817
$$23817:
	.skip 0xC
.global $$23818
$$23818:
	.skip 0xC
.global $$23819
$$23819:
	.skip 0xC
.global $$23820
$$23820:
	.skip 0xC
.global $$23821
$$23821:
	.skip 0xC
.global $$23822
$$23822:
	.skip 0xC
.global $$23823
$$23823:
	.skip 0xC
.global $$23824
$$23824:
	.skip 0xC
.global $$23825
$$23825:
	.skip 0xC
.global $$23826
$$23826:
	.skip 0xC
.global $$22530
$$22530:
	.skip 0xC
.global $$22531
$$22531:
	.skip 0xC
.global $$22532
$$22532:
	.skip 0xC
.global $$22533
$$22533:
	.skip 0xC
.global $$22534
$$22534:
	.skip 0xC
.global $$22535
$$22535:
	.skip 0xC
.global $$22536
$$22536:
	.skip 0xC
.global $$22537
$$22537:
	.skip 0xC
.global $$22538
$$22538:
	.skip 0xC
.global $$22539
$$22539:
	.skip 0xC
.global $$22540
$$22540:
	.skip 0xC
.global $$22541
$$22541:
	.skip 0xC
.global $$22542
$$22542:
	.skip 0xC
.global $$22543
$$22543:
	.skip 0xC
.global $$22544
$$22544:
	.skip 0x10
.global mIllegalCheckData__17TMapCollisionData
mIllegalCheckData__17TMapCollisionData:
	.skip 0x48
.global $$22159
$$22159:
	.skip 0xC
.global $$22160
$$22160:
	.skip 0xC
.global $$22161
$$22161:
	.skip 0xC
.global $$22162
$$22162:
	.skip 0xC
.global $$22163
$$22163:
	.skip 0xC
.global $$22164
$$22164:
	.skip 0xC
.global $$22165
$$22165:
	.skip 0xC
.global $$22166
$$22166:
	.skip 0xC
.global $$22167
$$22167:
	.skip 0xC
.global $$22168
$$22168:
	.skip 0xC
.global $$22169
$$22169:
	.skip 0xC
.global $$22170
$$22170:
	.skip 0xC
.global $$22171
$$22171:
	.skip 0xC
.global $$22172
$$22172:
	.skip 0xC
.global $$22173
$$22173:
	.skip 0x10
.global $$22402
$$22402:
	.skip 0xC
.global $$22403
$$22403:
	.skip 0xC
.global $$22404
$$22404:
	.skip 0xC
.global $$22405
$$22405:
	.skip 0xC
.global $$22406
$$22406:
	.skip 0xC
.global $$22407
$$22407:
	.skip 0xC
.global $$22408
$$22408:
	.skip 0xC
.global $$22409
$$22409:
	.skip 0xC
.global $$22410
$$22410:
	.skip 0xC
.global $$22411
$$22411:
	.skip 0xC
.global $$22412
$$22412:
	.skip 0xC
.global $$22413
$$22413:
	.skip 0xC
.global $$22414
$$22414:
	.skip 0xC
.global $$22415
$$22415:
	.skip 0xC
.global $$22416
$$22416:
	.skip 0x10
.global $$22700
$$22700:
	.skip 0xC
.global $$22701
$$22701:
	.skip 0xC
.global $$22702
$$22702:
	.skip 0xC
.global $$22703
$$22703:
	.skip 0xC
.global $$22704
$$22704:
	.skip 0xC
.global $$22705
$$22705:
	.skip 0xC
.global $$22706
$$22706:
	.skip 0xC
.global $$22707
$$22707:
	.skip 0xC
.global $$22708
$$22708:
	.skip 0xC
.global $$22709
$$22709:
	.skip 0xC
.global $$22710
$$22710:
	.skip 0xC
.global $$22711
$$22711:
	.skip 0xC
.global $$22712
$$22712:
	.skip 0xC
.global $$22713
$$22713:
	.skip 0xC
.global $$22714
$$22714:
	.skip 0x10
.global $$23811
$$23811:
	.skip 0xC
.global $$23812
$$23812:
	.skip 0xC
.global $$23813
$$23813:
	.skip 0xC
.global $$23814
$$23814:
	.skip 0xC
.global $$23815
$$23815:
	.skip 0xC
.global $$23816
$$23816:
	.skip 0xC
.global $$23817
$$23817:
	.skip 0xC
.global $$23818
$$23818:
	.skip 0xC
.global $$23819
$$23819:
	.skip 0xC
.global $$23820
$$23820:
	.skip 0xC
.global $$23821
$$23821:
	.skip 0xC
.global $$23822
$$23822:
	.skip 0xC
.global $$23823
$$23823:
	.skip 0xC
.global $$23824
$$23824:
	.skip 0xC
.global $$23825
$$23825:
	.skip 0x10
.global $$22744
$$22744:
	.skip 0xC
.global $$22745
$$22745:
	.skip 0xC
.global $$22746
$$22746:
	.skip 0xC
.global $$22747
$$22747:
	.skip 0xC
.global $$22748
$$22748:
	.skip 0xC
.global $$22749
$$22749:
	.skip 0xC
.global $$22750
$$22750:
	.skip 0xC
.global $$22751
$$22751:
	.skip 0xC
.global $$22752
$$22752:
	.skip 0xC
.global $$22753
$$22753:
	.skip 0xC
.global $$22754
$$22754:
	.skip 0xC
.global $$22755
$$22755:
	.skip 0xC
.global $$22756
$$22756:
	.skip 0xC
.global $$22757
$$22757:
	.skip 0xC
.global $$22758
$$22758:
	.skip 0x10
.global $$23293
$$23293:
	.skip 0xC
.global $$23294
$$23294:
	.skip 0xC
.global $$23295
$$23295:
	.skip 0xC
.global $$23296
$$23296:
	.skip 0xC
.global $$23297
$$23297:
	.skip 0xC
.global $$23298
$$23298:
	.skip 0xC
.global $$23299
$$23299:
	.skip 0xC
.global $$23300
$$23300:
	.skip 0xC
.global $$23301
$$23301:
	.skip 0xC
.global $$23302
$$23302:
	.skip 0xC
.global $$23303
$$23303:
	.skip 0xC
.global $$23304
$$23304:
	.skip 0xC
.global $$23305
$$23305:
	.skip 0xC
.global $$23306
$$23306:
	.skip 0xC
.global $$23307
$$23307:
	.skip 0x10
.global $$22668
$$22668:
	.skip 0xC
.global $$22669
$$22669:
	.skip 0xC
.global $$22670
$$22670:
	.skip 0xC
.global $$22671
$$22671:
	.skip 0xC
.global $$22672
$$22672:
	.skip 0xC
.global $$22673
$$22673:
	.skip 0xC
.global $$22674
$$22674:
	.skip 0xC
.global $$22675
$$22675:
	.skip 0xC
.global $$22676
$$22676:
	.skip 0xC
.global $$22677
$$22677:
	.skip 0xC
.global $$22678
$$22678:
	.skip 0xC
.global $$22679
$$22679:
	.skip 0xC
.global $$22680
$$22680:
	.skip 0xC
.global $$22681
$$22681:
	.skip 0xC
.global $$22682
$$22682:
	.skip 0x10
.global $$22684
$$22684:
	.skip 0xC
.global $$22685
$$22685:
	.skip 0xC
.global $$22686
$$22686:
	.skip 0xC
.global $$22687
$$22687:
	.skip 0xC
.global $$22688
$$22688:
	.skip 0xC
.global $$22689
$$22689:
	.skip 0xC
.global $$22690
$$22690:
	.skip 0xC
.global $$22691
$$22691:
	.skip 0xC
.global $$22692
$$22692:
	.skip 0xC
.global $$22693
$$22693:
	.skip 0xC
.global $$22694
$$22694:
	.skip 0xC
.global $$22695
$$22695:
	.skip 0xC
.global $$22696
$$22696:
	.skip 0xC
.global $$22697
$$22697:
	.skip 0xC
.global $$22698
$$22698:
	.skip 0x10
.global $$23581
$$23581:
	.skip 0xC
.global $$23582
$$23582:
	.skip 0xC
.global $$23583
$$23583:
	.skip 0xC
.global $$23584
$$23584:
	.skip 0xC
.global $$23585
$$23585:
	.skip 0xC
.global $$23586
$$23586:
	.skip 0xC
.global $$23587
$$23587:
	.skip 0xC
.global $$23588
$$23588:
	.skip 0xC
.global $$23589
$$23589:
	.skip 0xC
.global $$23590
$$23590:
	.skip 0xC
.global $$23591
$$23591:
	.skip 0xC
.global $$23592
$$23592:
	.skip 0xC
.global $$23593
$$23593:
	.skip 0xC
.global $$23594
$$23594:
	.skip 0xC
.global $$23595
$$23595:
	.skip 0x10
.global $$22358
$$22358:
	.skip 0xC
.global $$22359
$$22359:
	.skip 0xC
.global $$22360
$$22360:
	.skip 0xC
.global $$22361
$$22361:
	.skip 0xC
.global $$22362
$$22362:
	.skip 0xC
.global $$22363
$$22363:
	.skip 0xC
.global $$22364
$$22364:
	.skip 0xC
.global $$22365
$$22365:
	.skip 0xC
.global $$22366
$$22366:
	.skip 0xC
.global $$22367
$$22367:
	.skip 0xC
.global $$22368
$$22368:
	.skip 0xC
.global $$22369
$$22369:
	.skip 0xC
.global $$22370
$$22370:
	.skip 0xC
.global $$22371
$$22371:
	.skip 0xC
.global $$22372
$$22372:
	.skip 0x10
.global $$22938
$$22938:
	.skip 0xC
.global $$22939
$$22939:
	.skip 0xC
.global $$22940
$$22940:
	.skip 0xC
.global $$22941
$$22941:
	.skip 0xC
.global $$22942
$$22942:
	.skip 0xC
.global $$22943
$$22943:
	.skip 0xC
.global $$22944
$$22944:
	.skip 0xC
.global $$22945
$$22945:
	.skip 0xC
.global $$22946
$$22946:
	.skip 0xC
.global $$22947
$$22947:
	.skip 0xC
.global $$22948
$$22948:
	.skip 0xC
.global $$22949
$$22949:
	.skip 0xC
.global $$22950
$$22950:
	.skip 0xC
.global $$22951
$$22951:
	.skip 0xC
.global $$22952
$$22952:
	.skip 0x10
.global $$22581
$$22581:
	.skip 0xC
.global $$22582
$$22582:
	.skip 0xC
.global $$22583
$$22583:
	.skip 0xC
.global $$22584
$$22584:
	.skip 0xC
.global $$22585
$$22585:
	.skip 0xC
.global $$22586
$$22586:
	.skip 0xC
.global $$22587
$$22587:
	.skip 0xC
.global $$22588
$$22588:
	.skip 0xC
.global $$22589
$$22589:
	.skip 0xC
.global $$22590
$$22590:
	.skip 0xC
.global $$22591
$$22591:
	.skip 0xC
.global $$22592
$$22592:
	.skip 0xC
.global $$22593
$$22593:
	.skip 0xC
.global $$22594
$$22594:
	.skip 0xC
.global $$22595
$$22595:
	.skip 0x10
.global $$22615
$$22615:
	.skip 0xC
.global $$22616
$$22616:
	.skip 0xC
.global $$22617
$$22617:
	.skip 0xC
.global $$22618
$$22618:
	.skip 0xC
.global $$22619
$$22619:
	.skip 0xC
.global $$22620
$$22620:
	.skip 0xC
.global $$22621
$$22621:
	.skip 0xC
.global $$22622
$$22622:
	.skip 0xC
.global $$22623
$$22623:
	.skip 0xC
.global $$22624
$$22624:
	.skip 0xC
.global $$22625
$$22625:
	.skip 0xC
.global $$22626
$$22626:
	.skip 0xC
.global $$22627
$$22627:
	.skip 0xC
.global $$22628
$$22628:
	.skip 0xC
.global $$22629
$$22629:
	.skip 0x10
.global $$23519
$$23519:
	.skip 0xC
.global $$23520
$$23520:
	.skip 0xC
.global $$23521
$$23521:
	.skip 0xC
.global $$23522
$$23522:
	.skip 0xC
.global $$23523
$$23523:
	.skip 0xC
.global $$23524
$$23524:
	.skip 0xC
.global $$23525
$$23525:
	.skip 0xC
.global $$23526
$$23526:
	.skip 0xC
.global $$23527
$$23527:
	.skip 0xC
.global $$23528
$$23528:
	.skip 0xC
.global $$23529
$$23529:
	.skip 0xC
.global $$23530
$$23530:
	.skip 0xC
.global $$23531
$$23531:
	.skip 0xC
.global $$23532
$$23532:
	.skip 0xC
.global $$23533
$$23533:
	.skip 0x10
.global $$23239
$$23239:
	.skip 0xC
.global $$23240
$$23240:
	.skip 0xC
.global $$23241
$$23241:
	.skip 0xC
.global $$23242
$$23242:
	.skip 0xC
.global $$23243
$$23243:
	.skip 0xC
.global $$23244
$$23244:
	.skip 0xC
.global $$23245
$$23245:
	.skip 0xC
.global $$23246
$$23246:
	.skip 0xC
.global $$23247
$$23247:
	.skip 0xC
.global $$23248
$$23248:
	.skip 0xC
.global $$23249
$$23249:
	.skip 0xC
.global $$23250
$$23250:
	.skip 0xC
.global $$23251
$$23251:
	.skip 0xC
.global $$23252
$$23252:
	.skip 0xC
.global $$23253
$$23253:
	.skip 0x10
.global $$22734
$$22734:
	.skip 0xC
.global $$22735
$$22735:
	.skip 0xC
.global $$22736
$$22736:
	.skip 0xC
.global $$22737
$$22737:
	.skip 0xC
.global $$22738
$$22738:
	.skip 0xC
.global $$22739
$$22739:
	.skip 0xC
.global $$22740
$$22740:
	.skip 0xC
.global $$22741
$$22741:
	.skip 0xC
.global $$22742
$$22742:
	.skip 0xC
.global $$22743
$$22743:
	.skip 0xC
.global $$22744
$$22744:
	.skip 0xC
.global $$22745
$$22745:
	.skip 0xC
.global $$22746
$$22746:
	.skip 0xC
.global $$22747
$$22747:
	.skip 0xC
.global $$22748
$$22748:
	.skip 0x10
.global $$22374
$$22374:
	.skip 0xC
.global $$22375
$$22375:
	.skip 0xC
.global $$22376
$$22376:
	.skip 0xC
.global $$22377
$$22377:
	.skip 0xC
.global $$22378
$$22378:
	.skip 0xC
.global $$22379
$$22379:
	.skip 0xC
.global $$22380
$$22380:
	.skip 0xC
.global $$22381
$$22381:
	.skip 0xC
.global $$22382
$$22382:
	.skip 0xC
.global $$22383
$$22383:
	.skip 0xC
.global $$22384
$$22384:
	.skip 0xC
.global $$22385
$$22385:
	.skip 0xC
.global $$22386
$$22386:
	.skip 0xC
.global $$22387
$$22387:
	.skip 0xC
.global $$22388
$$22388:
	.skip 0x10
.global $$22809
$$22809:
	.skip 0xC
.global $$22810
$$22810:
	.skip 0xC
.global $$22811
$$22811:
	.skip 0xC
.global $$22812
$$22812:
	.skip 0xC
.global $$22813
$$22813:
	.skip 0xC
.global $$22814
$$22814:
	.skip 0xC
.global $$22815
$$22815:
	.skip 0xC
.global $$22816
$$22816:
	.skip 0xC
.global $$22817
$$22817:
	.skip 0xC
.global $$22818
$$22818:
	.skip 0xC
.global $$22819
$$22819:
	.skip 0xC
.global $$22820
$$22820:
	.skip 0xC
.global $$22821
$$22821:
	.skip 0xC
.global $$22822
$$22822:
	.skip 0xC
.global $$22823
$$22823:
	.skip 0x10
.global $$22867
$$22867:
	.skip 0xC
.global $$22868
$$22868:
	.skip 0xC
.global $$22869
$$22869:
	.skip 0xC
.global $$22870
$$22870:
	.skip 0xC
.global $$22871
$$22871:
	.skip 0xC
.global $$22872
$$22872:
	.skip 0xC
.global $$22873
$$22873:
	.skip 0xC
.global $$22874
$$22874:
	.skip 0xC
.global $$22875
$$22875:
	.skip 0xC
.global $$22876
$$22876:
	.skip 0xC
.global $$22877
$$22877:
	.skip 0xC
.global $$22878
$$22878:
	.skip 0xC
.global $$22879
$$22879:
	.skip 0xC
.global $$22880
$$22880:
	.skip 0xC
.global $$22881
$$22881:
	.skip 0x10
.global $$22812
$$22812:
	.skip 0xC
.global $$22813
$$22813:
	.skip 0xC
.global $$22814
$$22814:
	.skip 0xC
.global $$22815
$$22815:
	.skip 0xC
.global $$22816
$$22816:
	.skip 0xC
.global $$22817
$$22817:
	.skip 0xC
.global $$22818
$$22818:
	.skip 0xC
.global $$22819
$$22819:
	.skip 0xC
.global $$22820
$$22820:
	.skip 0xC
.global $$22821
$$22821:
	.skip 0xC
.global $$22822
$$22822:
	.skip 0xC
.global $$22823
$$22823:
	.skip 0xC
.global $$22824
$$22824:
	.skip 0xC
.global $$22825
$$22825:
	.skip 0xC
.global $$22826
$$22826:
	.skip 0x10
.global pos$2426
pos$2426:
	.skip 0x78
.global $$23091
$$23091:
	.skip 0xC
.global $$23092
$$23092:
	.skip 0xC
.global $$23093
$$23093:
	.skip 0xC
.global $$23094
$$23094:
	.skip 0xC
.global $$23095
$$23095:
	.skip 0xC
.global $$23096
$$23096:
	.skip 0xC
.global $$23097
$$23097:
	.skip 0xC
.global $$23098
$$23098:
	.skip 0xC
.global $$23099
$$23099:
	.skip 0xC
.global $$23100
$$23100:
	.skip 0xC
.global $$23101
$$23101:
	.skip 0xC
.global $$23102
$$23102:
	.skip 0xC
.global $$23103
$$23103:
	.skip 0xC
.global $$23104
$$23104:
	.skip 0xC
.global $$23105
$$23105:
	.skip 0x10
.global $$22727
$$22727:
	.skip 0xC
.global $$22728
$$22728:
	.skip 0xC
.global $$22729
$$22729:
	.skip 0xC
.global $$22730
$$22730:
	.skip 0xC
.global $$22731
$$22731:
	.skip 0xC
.global $$22732
$$22732:
	.skip 0xC
.global $$22733
$$22733:
	.skip 0xC
.global $$22734
$$22734:
	.skip 0xC
.global $$22735
$$22735:
	.skip 0xC
.global $$22736
$$22736:
	.skip 0xC
.global $$22737
$$22737:
	.skip 0xC
.global $$22738
$$22738:
	.skip 0xC
.global $$22739
$$22739:
	.skip 0xC
.global $$22740
$$22740:
	.skip 0xC
.global $$22741
$$22741:
	.skip 0x10
.global $$22330
$$22330:
	.skip 0xC
.global $$22331
$$22331:
	.skip 0xC
.global $$22332
$$22332:
	.skip 0xC
.global $$22333
$$22333:
	.skip 0xC
.global $$22334
$$22334:
	.skip 0xC
.global $$22335
$$22335:
	.skip 0xC
.global $$22336
$$22336:
	.skip 0xC
.global $$22337
$$22337:
	.skip 0xC
.global $$22338
$$22338:
	.skip 0xC
.global $$22339
$$22339:
	.skip 0xC
.global $$22340
$$22340:
	.skip 0xC
.global $$22341
$$22341:
	.skip 0xC
.global $$22342
$$22342:
	.skip 0xC
.global $$22343
$$22343:
	.skip 0xC
.global $$22344
$$22344:
	.skip 0x10
.global $$22650
$$22650:
	.skip 0xC
.global $$22651
$$22651:
	.skip 0xC
.global $$22652
$$22652:
	.skip 0xC
.global $$22653
$$22653:
	.skip 0xC
.global $$22654
$$22654:
	.skip 0xC
.global $$22655
$$22655:
	.skip 0xC
.global $$22656
$$22656:
	.skip 0xC
.global $$22657
$$22657:
	.skip 0xC
.global $$22658
$$22658:
	.skip 0xC
.global $$22659
$$22659:
	.skip 0xC
.global $$22660
$$22660:
	.skip 0xC
.global $$22661
$$22661:
	.skip 0xC
.global $$22662
$$22662:
	.skip 0xC
.global $$22663
$$22663:
	.skip 0xC
.global $$22664
$$22664:
	.skip 0x10
.global $$23102
$$23102:
	.skip 0xC
.global $$23103
$$23103:
	.skip 0xC
.global $$23104
$$23104:
	.skip 0xC
.global $$23105
$$23105:
	.skip 0xC
.global $$23106
$$23106:
	.skip 0xC
.global $$23107
$$23107:
	.skip 0xC
.global $$23108
$$23108:
	.skip 0xC
.global $$23109
$$23109:
	.skip 0xC
.global $$23110
$$23110:
	.skip 0xC
.global $$23111
$$23111:
	.skip 0xC
.global $$23112
$$23112:
	.skip 0xC
.global $$23113
$$23113:
	.skip 0xC
.global $$23114
$$23114:
	.skip 0xC
.global $$23115
$$23115:
	.skip 0xC
.global $$23116
$$23116:
	.skip 0x10
.global $$23971
$$23971:
	.skip 0xC
.global $$23972
$$23972:
	.skip 0xC
.global $$23973
$$23973:
	.skip 0xC
.global $$23974
$$23974:
	.skip 0xC
.global $$23975
$$23975:
	.skip 0xC
.global $$23976
$$23976:
	.skip 0xC
.global $$23977
$$23977:
	.skip 0xC
.global $$23978
$$23978:
	.skip 0xC
.global $$23979
$$23979:
	.skip 0xC
.global $$23980
$$23980:
	.skip 0xC
.global $$23981
$$23981:
	.skip 0xC
.global $$23982
$$23982:
	.skip 0xC
.global $$23983
$$23983:
	.skip 0xC
.global $$23984
$$23984:
	.skip 0xC
.global $$23985
$$23985:
	.skip 0x10
.global $$26023
$$26023:
	.skip 0xC
.global $$26024
$$26024:
	.skip 0xC
.global $$26025
$$26025:
	.skip 0xC
.global $$26026
$$26026:
	.skip 0xC
.global $$26027
$$26027:
	.skip 0xC
.global $$26028
$$26028:
	.skip 0xC
.global $$26029
$$26029:
	.skip 0xC
.global $$26030
$$26030:
	.skip 0xC
.global $$26031
$$26031:
	.skip 0xC
.global $$26032
$$26032:
	.skip 0xC
.global $$26033
$$26033:
	.skip 0xC
.global $$26034
$$26034:
	.skip 0xC
.global $$26035
$$26035:
	.skip 0xC
.global $$26036
$$26036:
	.skip 0xC
.global $$26037
$$26037:
	.skip 0x10
.global $$22405
$$22405:
	.skip 0xC
.global $$22406
$$22406:
	.skip 0xC
.global $$22407
$$22407:
	.skip 0xC
.global $$22408
$$22408:
	.skip 0xC
.global $$22409
$$22409:
	.skip 0xC
.global $$22410
$$22410:
	.skip 0xC
.global $$22411
$$22411:
	.skip 0xC
.global $$22412
$$22412:
	.skip 0xC
.global $$22413
$$22413:
	.skip 0xC
.global $$22414
$$22414:
	.skip 0xC
.global $$22415
$$22415:
	.skip 0xC
.global $$22416
$$22416:
	.skip 0xC
.global $$22417
$$22417:
	.skip 0xC
.global $$22418
$$22418:
	.skip 0xC
.global $$22419
$$22419:
	.skip 0x10
.global $$211646
$$211646:
	.skip 0xC
.global $$211647
$$211647:
	.skip 0xC
.global $$211648
$$211648:
	.skip 0xC
.global $$211649
$$211649:
	.skip 0xC
.global $$211650
$$211650:
	.skip 0xC
.global $$211651
$$211651:
	.skip 0xC
.global $$211652
$$211652:
	.skip 0xC
.global $$211653
$$211653:
	.skip 0xC
.global $$211654
$$211654:
	.skip 0xC
.global $$211655
$$211655:
	.skip 0xC
.global $$211656
$$211656:
	.skip 0xC
.global $$211657
$$211657:
	.skip 0xC
.global $$211658
$$211658:
	.skip 0xC
.global $$211659
$$211659:
	.skip 0xC
.global $$211660
$$211660:
	.skip 0x10
.global $$25013
$$25013:
	.skip 0xC
.global $$25014
$$25014:
	.skip 0xC
.global $$25015
$$25015:
	.skip 0xC
.global $$25016
$$25016:
	.skip 0xC
.global $$25017
$$25017:
	.skip 0xC
.global $$25018
$$25018:
	.skip 0xC
.global $$25019
$$25019:
	.skip 0xC
.global $$25020
$$25020:
	.skip 0xC
.global $$25021
$$25021:
	.skip 0xC
.global $$25022
$$25022:
	.skip 0xC
.global $$25023
$$25023:
	.skip 0xC
.global $$25024
$$25024:
	.skip 0xC
.global $$25025
$$25025:
	.skip 0xC
.global $$25026
$$25026:
	.skip 0xC
.global $$25027
$$25027:
	.skip 0x10
.global $$23375
$$23375:
	.skip 0xC
.global $$23376
$$23376:
	.skip 0xC
.global $$23377
$$23377:
	.skip 0xC
.global $$23378
$$23378:
	.skip 0xC
.global $$23379
$$23379:
	.skip 0xC
.global $$23380
$$23380:
	.skip 0xC
.global $$23381
$$23381:
	.skip 0xC
.global $$23382
$$23382:
	.skip 0xC
.global $$23383
$$23383:
	.skip 0xC
.global $$23384
$$23384:
	.skip 0xC
.global $$23385
$$23385:
	.skip 0xC
.global $$23386
$$23386:
	.skip 0xC
.global $$23387
$$23387:
	.skip 0xC
.global $$23388
$$23388:
	.skip 0xC
.global $$23389
$$23389:
	.skip 0x10
.global $$26914
$$26914:
	.skip 0xC
.global $$26915
$$26915:
	.skip 0xC
.global $$26916
$$26916:
	.skip 0xC
.global $$26917
$$26917:
	.skip 0xC
.global $$26918
$$26918:
	.skip 0xC
.global $$26919
$$26919:
	.skip 0xC
.global $$26920
$$26920:
	.skip 0xC
.global $$26921
$$26921:
	.skip 0xC
.global $$26922
$$26922:
	.skip 0xC
.global $$26923
$$26923:
	.skip 0xC
.global $$26924
$$26924:
	.skip 0xC
.global $$26925
$$26925:
	.skip 0xC
.global $$26926
$$26926:
	.skip 0xC
.global $$26927
$$26927:
	.skip 0xC
.global $$26928
$$26928:
	.skip 0x10
.global $$28348
$$28348:
	.skip 0xC
.global $$28349
$$28349:
	.skip 0xC
.global $$28350
$$28350:
	.skip 0xC
.global $$28351
$$28351:
	.skip 0xC
.global $$28352
$$28352:
	.skip 0xC
.global $$28353
$$28353:
	.skip 0xC
.global $$28354
$$28354:
	.skip 0xC
.global $$28355
$$28355:
	.skip 0xC
.global $$28356
$$28356:
	.skip 0xC
.global $$28357
$$28357:
	.skip 0xC
.global $$28358
$$28358:
	.skip 0xC
.global $$28359
$$28359:
	.skip 0xC
.global $$28360
$$28360:
	.skip 0xC
.global $$28361
$$28361:
	.skip 0xC
.global $$28362
$$28362:
	.skip 0x10
.global $$24101
$$24101:
	.skip 0xC
.global $$24102
$$24102:
	.skip 0xC
.global $$24103
$$24103:
	.skip 0xC
.global $$24104
$$24104:
	.skip 0xC
.global $$24105
$$24105:
	.skip 0xC
.global $$24106
$$24106:
	.skip 0xC
.global $$24107
$$24107:
	.skip 0xC
.global $$24108
$$24108:
	.skip 0xC
.global $$24109
$$24109:
	.skip 0xC
.global $$24110
$$24110:
	.skip 0xC
.global $$24111
$$24111:
	.skip 0xC
.global $$24112
$$24112:
	.skip 0xC
.global $$24113
$$24113:
	.skip 0xC
.global $$24114
$$24114:
	.skip 0xC
.global $$24115
$$24115:
	.skip 0x10
.global $$23131
$$23131:
	.skip 0xC
.global $$23132
$$23132:
	.skip 0xC
.global $$23133
$$23133:
	.skip 0xC
.global $$23134
$$23134:
	.skip 0xC
.global $$23135
$$23135:
	.skip 0xC
.global $$23136
$$23136:
	.skip 0xC
.global $$23137
$$23137:
	.skip 0xC
.global $$23138
$$23138:
	.skip 0xC
.global $$23139
$$23139:
	.skip 0xC
.global $$23140
$$23140:
	.skip 0xC
.global $$23141
$$23141:
	.skip 0xC
.global $$23142
$$23142:
	.skip 0xC
.global $$23143
$$23143:
	.skip 0xC
.global $$23144
$$23144:
	.skip 0xC
.global $$23145
$$23145:
	.skip 0x10
.global cCenter__19TSelectShineManager
cCenter__19TSelectShineManager:
	.skip 0x10
.global $$24133
$$24133:
	.skip 0xC
.global $$24134
$$24134:
	.skip 0xC
.global $$24135
$$24135:
	.skip 0xC
.global $$24136
$$24136:
	.skip 0xC
.global $$24137
$$24137:
	.skip 0xC
.global $$24138
$$24138:
	.skip 0xC
.global $$24139
$$24139:
	.skip 0xC
.global $$24140
$$24140:
	.skip 0xC
.global $$24141
$$24141:
	.skip 0xC
.global $$24142
$$24142:
	.skip 0xC
.global $$24143
$$24143:
	.skip 0xC
.global $$24144
$$24144:
	.skip 0xC
.global $$24145
$$24145:
	.skip 0xC
.global $$24146
$$24146:
	.skip 0xC
.global $$24147
$$24147:
	.skip 0x10
.global hx
hx:
	.skip 0x80
.global hx_buffer
hx_buffer:
	.skip 0x3300
.global $$22300
$$22300:
	.skip 0xC
.global $$22301
$$22301:
	.skip 0xC
.global $$22302
$$22302:
	.skip 0xC
.global $$22303
$$22303:
	.skip 0xC
.global $$22304
$$22304:
	.skip 0xC
.global $$22305
$$22305:
	.skip 0xC
.global $$22306
$$22306:
	.skip 0xC
.global $$22307
$$22307:
	.skip 0xC
.global $$22308
$$22308:
	.skip 0xC
.global $$22309
$$22309:
	.skip 0xC
.global $$22310
$$22310:
	.skip 0xC
.global $$22311
$$22311:
	.skip 0xC
.global $$22312
$$22312:
	.skip 0xC
.global $$22313
$$22313:
	.skip 0xC
.global $$22314
$$22314:
	.skip 0x10
.global $$23585
$$23585:
	.skip 0xC
.global $$23586
$$23586:
	.skip 0xC
.global $$23587
$$23587:
	.skip 0xC
.global $$23588
$$23588:
	.skip 0xC
.global $$23589
$$23589:
	.skip 0xC
.global $$23590
$$23590:
	.skip 0xC
.global $$23591
$$23591:
	.skip 0xC
.global $$23592
$$23592:
	.skip 0xC
.global $$23593
$$23593:
	.skip 0xC
.global $$23594
$$23594:
	.skip 0xC
.global $$23595
$$23595:
	.skip 0xC
.global $$23596
$$23596:
	.skip 0xC
.global $$23597
$$23597:
	.skip 0xC
.global $$23598
$$23598:
	.skip 0xC
.global $$23599
$$23599:
	.skip 0x10
.global $$23175
$$23175:
	.skip 0xC
.global $$23176
$$23176:
	.skip 0xC
.global $$23177
$$23177:
	.skip 0xC
.global $$23178
$$23178:
	.skip 0xC
.global $$23179
$$23179:
	.skip 0xC
.global $$23180
$$23180:
	.skip 0xC
.global $$23181
$$23181:
	.skip 0xC
.global $$23182
$$23182:
	.skip 0xC
.global $$23183
$$23183:
	.skip 0xC
.global $$23184
$$23184:
	.skip 0xC
.global $$23185
$$23185:
	.skip 0xC
.global $$23186
$$23186:
	.skip 0xC
.global $$23187
$$23187:
	.skip 0xC
.global $$23188
$$23188:
	.skip 0xC
.global $$23189
$$23189:
	.skip 0x10
.global $$24324
$$24324:
	.skip 0xC
.global $$24325
$$24325:
	.skip 0xC
.global $$24326
$$24326:
	.skip 0xC
.global $$24327
$$24327:
	.skip 0xC
.global $$24328
$$24328:
	.skip 0xC
.global $$24329
$$24329:
	.skip 0xC
.global $$24330
$$24330:
	.skip 0xC
.global $$24331
$$24331:
	.skip 0xC
.global $$24332
$$24332:
	.skip 0xC
.global $$24333
$$24333:
	.skip 0xC
.global $$24334
$$24334:
	.skip 0xC
.global $$24335
$$24335:
	.skip 0xC
.global $$24336
$$24336:
	.skip 0xC
.global $$24337
$$24337:
	.skip 0xC
.global $$24338
$$24338:
	.skip 0x10
.global $$23908
$$23908:
	.skip 0xC
.global $$23909
$$23909:
	.skip 0xC
.global $$23910
$$23910:
	.skip 0xC
.global $$23911
$$23911:
	.skip 0xC
.global $$23912
$$23912:
	.skip 0xC
.global $$23913
$$23913:
	.skip 0xC
.global $$23914
$$23914:
	.skip 0xC
.global $$23915
$$23915:
	.skip 0xC
.global $$23916
$$23916:
	.skip 0xC
.global $$23917
$$23917:
	.skip 0xC
.global $$23918
$$23918:
	.skip 0xC
.global $$23919
$$23919:
	.skip 0xC
.global $$23920
$$23920:
	.skip 0xC
.global $$23921
$$23921:
	.skip 0xC
.global $$23922
$$23922:
	.skip 0x10
.global $$26776
$$26776:
	.skip 0xC
.global $$26777
$$26777:
	.skip 0xC
.global $$26778
$$26778:
	.skip 0xC
.global $$26779
$$26779:
	.skip 0xC
.global $$26780
$$26780:
	.skip 0xC
.global $$26781
$$26781:
	.skip 0xC
.global $$26782
$$26782:
	.skip 0xC
.global $$26783
$$26783:
	.skip 0xC
.global $$26784
$$26784:
	.skip 0xC
.global $$26785
$$26785:
	.skip 0xC
.global $$26786
$$26786:
	.skip 0xC
.global $$26787
$$26787:
	.skip 0xC
.global $$26788
$$26788:
	.skip 0xC
.global $$26789
$$26789:
	.skip 0xC
.global $$26790
$$26790:
	.skip 0x10
.global $$23221
$$23221:
	.skip 0xC
.global $$23229
$$23229:
	.skip 0xC
.global $$23264
$$23264:
	.skip 0xC
.global $$23274
$$23274:
	.skip 0xC
.global $$23294
$$23294:
	.skip 0xC
.global $$23357
$$23357:
	.skip 0xC
.global $$23371
$$23371:
	.skip 0xC
.global $$23381
$$23381:
	.skip 0xC
.global $$23391
$$23391:
	.skip 0xC
.global $$25456
$$25456:
	.skip 0xC
.global $$25457
$$25457:
	.skip 0xC
.global $$25458
$$25458:
	.skip 0xC
.global $$25459
$$25459:
	.skip 0xC
.global $$25460
$$25460:
	.skip 0xC
.global $$25461
$$25461:
	.skip 0xC
.global $$25462
$$25462:
	.skip 0xC
.global $$25463
$$25463:
	.skip 0xC
.global $$25464
$$25464:
	.skip 0xC
.global $$25465
$$25465:
	.skip 0xC
.global $$25466
$$25466:
	.skip 0xC
.global $$25467
$$25467:
	.skip 0xC
.global $$25468
$$25468:
	.skip 0xC
.global $$25469
$$25469:
	.skip 0xC
.global $$25470
$$25470:
	.skip 0xC
.global $$23757
$$23757:
	.skip 0xC
.global $$23771
$$23771:
	.skip 0xC
.global $$23788
$$23788:
	.skip 0xC
.global $$23800
$$23800:
	.skip 0xC
.global $$23810
$$23810:
	.skip 0xC
.global $$23828
$$23828:
	.skip 0xC
.global $$23836
$$23836:
	.skip 0xC
.global $$23844
$$23844:
	.skip 0xC
.global $$23855
$$23855:
	.skip 0xC
.global $$23875
$$23875:
	.skip 0xC
.global $$23889
$$23889:
	.skip 0xC
.global $$23900
$$23900:
	.skip 0xC
.global $$23910
$$23910:
	.skip 0xC
.global $$23923
$$23923:
	.skip 0xC
.global $$28477
$$28477:
	.skip 0xC
.global $$28478
$$28478:
	.skip 0xC
.global $$28479
$$28479:
	.skip 0xC
.global $$28480
$$28480:
	.skip 0xC
.global $$28481
$$28481:
	.skip 0xC
.global $$28482
$$28482:
	.skip 0xC
.global $$28483
$$28483:
	.skip 0xC
.global $$28484
$$28484:
	.skip 0xC
.global $$28485
$$28485:
	.skip 0xC
.global $$28486
$$28486:
	.skip 0xC
.global $$28487
$$28487:
	.skip 0xC
.global $$28488
$$28488:
	.skip 0xC
.global $$28489
$$28489:
	.skip 0xC
.global $$28490
$$28490:
	.skip 0xC
.global $$28491
$$28491:
	.skip 0x10
.global $$23386
$$23386:
	.skip 0xC
.global $$23398
$$23398:
	.skip 0xC
.global $$23435
$$23435:
	.skip 0xC
.global $$23451
$$23451:
	.skip 0xC
.global $$23462
$$23462:
	.skip 0xC
.global $$23475
$$23475:
	.skip 0xC
.global $$23494
$$23494:
	.skip 0xC
.global $$23511
$$23511:
	.skip 0xC
.global $$23541
$$23541:
	.skip 0xC
.global $$23570
$$23570:
	.skip 0xC
.global $$23584
$$23584:
	.skip 0xC
.global $$23594
$$23594:
	.skip 0xC
.global $$23606
$$23606:
	.skip 0xC
.global $$23624
$$23624:
	.skip 0xC
.global $$23634
$$23634:
	.skip 0xC
.global $$26476
$$26476:
	.skip 0xC
.global $$26477
$$26477:
	.skip 0xC
.global $$26478
$$26478:
	.skip 0xC
.global $$26479
$$26479:
	.skip 0xC
.global $$26480
$$26480:
	.skip 0xC
.global $$26481
$$26481:
	.skip 0xC
.global $$26482
$$26482:
	.skip 0xC
.global $$26483
$$26483:
	.skip 0xC
.global $$26484
$$26484:
	.skip 0xC
.global $$26485
$$26485:
	.skip 0xC
.global $$26486
$$26486:
	.skip 0xC
.global $$26487
$$26487:
	.skip 0xC
.global $$26488
$$26488:
	.skip 0xC
.global $$26489
$$26489:
	.skip 0xC
.global $$26490
$$26490:
	.skip 0xC
.global $$22573
$$22573:
	.skip 0xC
.global $$22631
$$22631:
	.skip 0xC
.global $$22654
$$22654:
	.skip 0xC
.global $$22674
$$22674:
	.skip 0xC
.global $$22694
$$22694:
	.skip 0xC
.global $$22708
$$22708:
	.skip 0xC
.global $$22726
$$22726:
	.skip 0xC
.global $$22753
$$22753:
	.skip 0xC
.global $$24055
$$24055:
	.skip 0xC
.global $$24056
$$24056:
	.skip 0xC
.global $$24057
$$24057:
	.skip 0xC
.global $$24058
$$24058:
	.skip 0xC
.global $$24059
$$24059:
	.skip 0xC
.global $$24060
$$24060:
	.skip 0xC
.global $$24061
$$24061:
	.skip 0xC
.global $$24062
$$24062:
	.skip 0xC
.global $$24063
$$24063:
	.skip 0xC
.global $$24064
$$24064:
	.skip 0xC
.global $$24065
$$24065:
	.skip 0xC
.global $$24066
$$24066:
	.skip 0xC
.global $$23069
$$23069:
	.skip 0xC
.global $$23082
$$23082:
	.skip 0xC
.global $$23127
$$23127:
	.skip 0xC
.global $$23146
$$23146:
	.skip 0xC
.global $$23158
$$23158:
	.skip 0xC
.global $$23166
$$23166:
	.skip 0xC
.global $$23172
$$23172:
	.skip 0xC
.global $$24760
$$24760:
	.skip 0xC
.global $$24761
$$24761:
	.skip 0xC
.global $$24762
$$24762:
	.skip 0xC
.global $$24763
$$24763:
	.skip 0xC
.global $$24764
$$24764:
	.skip 0xC
.global $$24765
$$24765:
	.skip 0xC
.global $$24766
$$24766:
	.skip 0xC
.global $$24767
$$24767:
	.skip 0xC
.global $$24768
$$24768:
	.skip 0xC
.global $$24769
$$24769:
	.skip 0xC
.global $$24770
$$24770:
	.skip 0xC
.global $$24771
$$24771:
	.skip 0xC
.global $$24772
$$24772:
	.skip 0xC
.global $$24773
$$24773:
	.skip 0xC
.global $$24774
$$24774:
	.skip 0xC
.global $$23233
$$23233:
	.skip 0xC
.global $$23253
$$23253:
	.skip 0xC
.global $$23296
$$23296:
	.skip 0xC
.global $$23309
$$23309:
	.skip 0xC
.global $$23321
$$23321:
	.skip 0xC
.global $$23336
$$23336:
	.skip 0xC
.global $$23357
$$23357:
	.skip 0xC
.global $$23387
$$23387:
	.skip 0xC
.global $$25423
$$25423:
	.skip 0xC
.global $$25424
$$25424:
	.skip 0xC
.global $$25425
$$25425:
	.skip 0xC
.global $$25426
$$25426:
	.skip 0xC
.global $$25427
$$25427:
	.skip 0xC
.global $$25428
$$25428:
	.skip 0xC
.global $$25429
$$25429:
	.skip 0xC
.global $$25430
$$25430:
	.skip 0xC
.global $$25431
$$25431:
	.skip 0xC
.global $$25432
$$25432:
	.skip 0xC
.global $$25433
$$25433:
	.skip 0xC
.global $$25434
$$25434:
	.skip 0xC
.global $$25435
$$25435:
	.skip 0xC
.global $$25436
$$25436:
	.skip 0xC
.global $$25437
$$25437:
	.skip 0x10
.global $$23224
$$23224:
	.skip 0xC
.global $$23252
$$23252:
	.skip 0xC
.global $$23263
$$23263:
	.skip 0xC
.global $$23277
$$23277:
	.skip 0xC
.global $$23294
$$23294:
	.skip 0xC
.global $$23306
$$23306:
	.skip 0xC
.global $$25009
$$25009:
	.skip 0xC
.global $$25010
$$25010:
	.skip 0xC
.global $$25011
$$25011:
	.skip 0xC
.global $$25012
$$25012:
	.skip 0xC
.global $$25013
$$25013:
	.skip 0xC
.global $$25014
$$25014:
	.skip 0xC
.global $$25015
$$25015:
	.skip 0xC
.global $$25016
$$25016:
	.skip 0xC
.global $$25017
$$25017:
	.skip 0xC
.global $$25018
$$25018:
	.skip 0xC
.global $$25019
$$25019:
	.skip 0xC
.global $$25020
$$25020:
	.skip 0xC
.global $$25021
$$25021:
	.skip 0xC
.global $$25022
$$25022:
	.skip 0xC
.global $$25023
$$25023:
	.skip 0x10
.global $$22167
$$22167:
	.skip 0x10
.global $$22615
$$22615:
	.skip 0xC
.global $$22626
$$22626:
	.skip 0xC
.global $$22638
$$22638:
	.skip 0xC
.global $$22657
$$22657:
	.skip 0xC
.global $$22668
$$22668:
	.skip 0xC
.global $$22682
$$22682:
	.skip 0xC
.global $$23421
$$23421:
	.skip 0xC
.global $$23422
$$23422:
	.skip 0xC
.global $$23423
$$23423:
	.skip 0xC
.global $$23424
$$23424:
	.skip 0xC
.global $$23425
$$23425:
	.skip 0xC
.global $$23426
$$23426:
	.skip 0xC
.global $$23427
$$23427:
	.skip 0xC
.global $$23428
$$23428:
	.skip 0xC
.global $$23429
$$23429:
	.skip 0xC
.global $$23430
$$23430:
	.skip 0xC
.global $$23431
$$23431:
	.skip 0xC
.global $$23432
$$23432:
	.skip 0xC
.global $$23433
$$23433:
	.skip 0xC
.global $$23434
$$23434:
	.skip 0xC
.global $$23435
$$23435:
	.skip 0x10
.global $$23719
$$23719:
	.skip 0xC
.global $$23737
$$23737:
	.skip 0xC
.global $$23752
$$23752:
	.skip 0xC
.global $$23783
$$23783:
	.skip 0xC
.global $$23803
$$23803:
	.skip 0xC
.global $$23822
$$23822:
	.skip 0xC
.global $$23869
$$23869:
	.skip 0xC
.global $$23885
$$23885:
	.skip 0xC
.global $$23902
$$23902:
	.skip 0xC
.global $$29261
$$29261:
	.skip 0xC
.global $$29262
$$29262:
	.skip 0xC
.global $$29263
$$29263:
	.skip 0xC
.global $$29264
$$29264:
	.skip 0xC
.global $$29265
$$29265:
	.skip 0xC
.global $$29266
$$29266:
	.skip 0xC
.global $$29267
$$29267:
	.skip 0xC
.global $$29268
$$29268:
	.skip 0xC
.global $$29269
$$29269:
	.skip 0xC
.global $$29270
$$29270:
	.skip 0xC
.global $$29271
$$29271:
	.skip 0xC
.global $$29272
$$29272:
	.skip 0xC
.global $$29273
$$29273:
	.skip 0xC
.global $$29274
$$29274:
	.skip 0xC
.global $$29275
$$29275:
	.skip 0xC
.global $$23159
$$23159:
	.skip 0xC
.global $$23182
$$23182:
	.skip 0xC
.global $$23208
$$23208:
	.skip 0xC
.global $$23233
$$23233:
	.skip 0xC
.global $$23263
$$23263:
	.skip 0xC
.global $$23275
$$23275:
	.skip 0xC
.global $$23286
$$23286:
	.skip 0xC
.global $$23305
$$23305:
	.skip 0xC
.global $$23313
$$23313:
	.skip 0xC
.global $$25419
$$25419:
	.skip 0xC
.global $$25420
$$25420:
	.skip 0xC
.global $$25421
$$25421:
	.skip 0xC
.global $$25422
$$25422:
	.skip 0xC
.global $$25423
$$25423:
	.skip 0xC
.global $$25424
$$25424:
	.skip 0xC
.global $$25425
$$25425:
	.skip 0xC
.global $$25426
$$25426:
	.skip 0xC
.global $$25427
$$25427:
	.skip 0xC
.global $$25428
$$25428:
	.skip 0xC
.global $$25429
$$25429:
	.skip 0xC
.global $$25430
$$25430:
	.skip 0xC
.global $$25431
$$25431:
	.skip 0xC
.global $$25432
$$25432:
	.skip 0xC
.global $$25433
$$25433:
	.skip 0xC
.global $$23307
$$23307:
	.skip 0xC
.global $$23333
$$23333:
	.skip 0xC
.global $$23354
$$23354:
	.skip 0xC
.global $$23375
$$23375:
	.skip 0xC
.global $$23387
$$23387:
	.skip 0xC
.global $$23405
$$23405:
	.skip 0xC
.global $$25543
$$25543:
	.skip 0xC
.global $$25544
$$25544:
	.skip 0xC
.global $$25545
$$25545:
	.skip 0xC
.global $$25546
$$25546:
	.skip 0xC
.global $$25547
$$25547:
	.skip 0xC
.global $$25548
$$25548:
	.skip 0xC
.global $$25549
$$25549:
	.skip 0xC
.global $$25550
$$25550:
	.skip 0xC
.global $$25551
$$25551:
	.skip 0xC
.global $$25552
$$25552:
	.skip 0xC
.global $$25553
$$25553:
	.skip 0xC
.global $$25554
$$25554:
	.skip 0xC
.global $$25555
$$25555:
	.skip 0xC
.global $$25556
$$25556:
	.skip 0xC
.global $$25557
$$25557:
	.skip 0x10
.global $$23484
$$23484:
	.skip 0xC
.global $$23498
$$23498:
	.skip 0xC
.global $$23510
$$23510:
	.skip 0xC
.global $$23520
$$23520:
	.skip 0xC
.global $$23533
$$23533:
	.skip 0xC
.global $$23548
$$23548:
	.skip 0xC
.global $$23564
$$23564:
	.skip 0xC
.global $$23574
$$23574:
	.skip 0xC
.global $$23583
$$23583:
	.skip 0xC
.global $$23593
$$23593:
	.skip 0xC
.global $$23608
$$23608:
	.skip 0xC
.global $$23619
$$23619:
	.skip 0xC
.global $$29726
$$29726:
	.skip 0xC
.global $$29727
$$29727:
	.skip 0xC
.global $$29728
$$29728:
	.skip 0xC
.global $$29729
$$29729:
	.skip 0xC
.global $$29730
$$29730:
	.skip 0xC
.global $$29731
$$29731:
	.skip 0xC
.global $$29732
$$29732:
	.skip 0xC
.global $$29733
$$29733:
	.skip 0xC
.global $$29734
$$29734:
	.skip 0xC
.global $$29735
$$29735:
	.skip 0xC
.global $$29736
$$29736:
	.skip 0xC
.global $$29737
$$29737:
	.skip 0xC
.global $$29738
$$29738:
	.skip 0xC
.global $$29739
$$29739:
	.skip 0xC
.global $$29740
$$29740:
	.skip 0x10
.global up$3230
up$3230:
	.skip 0xC
.global up$3276
up$3276:
	.skip 0xC
.global goal$3329
goal$3329:
	.skip 0xC
.global start$3332
start$3332:
	.skip 0xC
.global goal$3336
goal$3336:
	.skip 0xC
.global start$3339
start$3339:
	.skip 0xC
.global s$3585
s$3585:
	.skip 0xC
.global $$23634
$$23634:
	.skip 0xC
.global $$23674
$$23674:
	.skip 0xC
.global $$23687
$$23687:
	.skip 0xC
.global $$23716
$$23716:
	.skip 0xC
.global $$23728
$$23728:
	.skip 0xC
.global $$23741
$$23741:
	.skip 0xC
.global $$23754
$$23754:
	.skip 0xC
.global $$23768
$$23768:
	.skip 0xC
.global $$23780
$$23780:
	.skip 0xC
.global $$23804
$$23804:
	.skip 0xC
.global $$23817
$$23817:
	.skip 0xC
.global $$23829
$$23829:
	.skip 0xC
.global $$23843
$$23843:
	.skip 0xC
.global $$23853
$$23853:
	.skip 0xC
.global $$23863
$$23863:
	.skip 0xC
.global $$23880
$$23880:
	.skip 0xC
.global $$23898
$$23898:
	.skip 0xC
.global $$23911
$$23911:
	.skip 0xC
.global $$23927
$$23927:
	.skip 0xC
.global $$23939
$$23939:
	.skip 0xC
.global $$23949
$$23949:
	.skip 0xC
.global $$23963
$$23963:
	.skip 0xC
.global $$24000
$$24000:
	.skip 0xC
.global $$24008
$$24008:
	.skip 0xC
.global $$24018
$$24018:
	.skip 0xC
.global $$24034
$$24034:
	.skip 0xC
.global $$28431
$$28431:
	.skip 0xC
.global $$28432
$$28432:
	.skip 0xC
.global $$28433
$$28433:
	.skip 0xC
.global $$28434
$$28434:
	.skip 0xC
.global $$28435
$$28435:
	.skip 0xC
.global $$28436
$$28436:
	.skip 0xC
.global $$28437
$$28437:
	.skip 0xC
.global $$28438
$$28438:
	.skip 0xC
.global $$28439
$$28439:
	.skip 0xC
.global $$28440
$$28440:
	.skip 0xC
.global $$28441
$$28441:
	.skip 0xC
.global $$28442
$$28442:
	.skip 0xC
.global $$28443
$$28443:
	.skip 0xC
.global $$28444
$$28444:
	.skip 0xC
.global $$28445
$$28445:
	.skip 0xC
.global $$23084
$$23084:
	.skip 0xC
.global $$23096
$$23096:
	.skip 0xC
.global $$23114
$$23114:
	.skip 0xC
.global $$23129
$$23129:
	.skip 0xC
.global $$23154
$$23154:
	.skip 0xC
.global $$23167
$$23167:
	.skip 0xC
.global $$23179
$$23179:
	.skip 0xC
.global $$23196
$$23196:
	.skip 0xC
.global $$23234
$$23234:
	.skip 0xC
.global $$23247
$$23247:
	.skip 0xC
.global $$23269
$$23269:
	.skip 0xC
.global $$23281
$$23281:
	.skip 0xC
.global $$25223
$$25223:
	.skip 0xC
.global $$25224
$$25224:
	.skip 0xC
.global $$25225
$$25225:
	.skip 0xC
.global $$25226
$$25226:
	.skip 0xC
.global $$25227
$$25227:
	.skip 0xC
.global $$25228
$$25228:
	.skip 0xC
.global $$25229
$$25229:
	.skip 0xC
.global $$25230
$$25230:
	.skip 0xC
.global $$25231
$$25231:
	.skip 0xC
.global $$25232
$$25232:
	.skip 0xC
.global $$25233
$$25233:
	.skip 0xC
.global $$25234
$$25234:
	.skip 0xC
.global $$25235
$$25235:
	.skip 0xC
.global $$25236
$$25236:
	.skip 0xC
.global $$25237
$$25237:
	.skip 0x10
.global TTinKoopa_jointIndexTable
TTinKoopa_jointIndexTable:
	.skip 0x3C
.global $$23539
$$23539:
	.skip 0xC
.global $$23551
$$23551:
	.skip 0xC
.global $$23564
$$23564:
	.skip 0xC
.global $$26180
$$26180:
	.skip 0xC
.global $$26181
$$26181:
	.skip 0xC
.global $$26182
$$26182:
	.skip 0xC
.global $$26183
$$26183:
	.skip 0xC
.global $$26184
$$26184:
	.skip 0xC
.global $$26185
$$26185:
	.skip 0xC
.global $$26186
$$26186:
	.skip 0xC
.global $$26187
$$26187:
	.skip 0xC
.global $$26188
$$26188:
	.skip 0xC
.global $$26189
$$26189:
	.skip 0xC
.global $$26190
$$26190:
	.skip 0xC
.global $$26191
$$26191:
	.skip 0xC
.global $$26192
$$26192:
	.skip 0xC
.global $$26193
$$26193:
	.skip 0xC
.global $$26194
$$26194:
	.skip 0x10
.global $$22768
$$22768:
	.skip 0xC
.global $$23340
$$23340:
	.skip 0xC
.global $$23341
$$23341:
	.skip 0xC
.global $$23342
$$23342:
	.skip 0xC
.global $$23343
$$23343:
	.skip 0xC
.global $$23344
$$23344:
	.skip 0xC
.global $$23345
$$23345:
	.skip 0xC
.global $$23346
$$23346:
	.skip 0xC
.global $$23347
$$23347:
	.skip 0xC
.global $$23348
$$23348:
	.skip 0xC
.global $$23349
$$23349:
	.skip 0xC
.global $$23350
$$23350:
	.skip 0xC
.global $$23351
$$23351:
	.skip 0xC
.global $$23352
$$23352:
	.skip 0xC
.global $$23353
$$23353:
	.skip 0xC
.global $$23354
$$23354:
	.skip 0xC
.global BW_BATH_POS
BW_BATH_POS:
	.skip 0xC
.global BW_PICKET_START
BW_PICKET_START:
	.skip 0xC
.global BW_HEAD_START
BW_HEAD_START:
	.skip 0xC
.global $$23489
$$23489:
	.skip 0xC
.global $$23536
$$23536:
	.skip 0xC
.global $$23549
$$23549:
	.skip 0xC
.global $$23563
$$23563:
	.skip 0xC
.global $$23574
$$23574:
	.skip 0xC
.global $$23591
$$23591:
	.skip 0xC
.global $$23601
$$23601:
	.skip 0xC
.global $$23620
$$23620:
	.skip 0xC
.global $$23639
$$23639:
	.skip 0xC
.global $$23653
$$23653:
	.skip 0xC
.global $$23663
$$23663:
	.skip 0xC
.global $$27662
$$27662:
	.skip 0xC
.global $$27663
$$27663:
	.skip 0xC
.global $$27664
$$27664:
	.skip 0xC
.global $$27665
$$27665:
	.skip 0xC
.global $$27666
$$27666:
	.skip 0xC
.global $$27667
$$27667:
	.skip 0xC
.global $$27668
$$27668:
	.skip 0xC
.global $$27669
$$27669:
	.skip 0xC
.global $$27670
$$27670:
	.skip 0xC
.global $$27671
$$27671:
	.skip 0xC
.global $$27672
$$27672:
	.skip 0xC
.global $$27673
$$27673:
	.skip 0xC
.global $$27674
$$27674:
	.skip 0xC
.global $$27675
$$27675:
	.skip 0xC
.global $$27676
$$27676:
	.skip 0x10
.global $$23082
$$23082:
	.skip 0xC
.global $$23101
$$23101:
	.skip 0xC
.global $$23120
$$23120:
	.skip 0xC
.global $$23148
$$23148:
	.skip 0xC
.global $$23172
$$23172:
	.skip 0xC
.global $$23187
$$23187:
	.skip 0xC
.global $$23193
$$23193:
	.skip 0xC
.global $$23204
$$23204:
	.skip 0xC
.global $$23210
$$23210:
	.skip 0xC
.global $$23226
$$23226:
	.skip 0xC
.global $$23239
$$23239:
	.skip 0xC
.global $$25853
$$25853:
	.skip 0xC
.global $$25854
$$25854:
	.skip 0xC
.global $$25855
$$25855:
	.skip 0xC
.global $$25856
$$25856:
	.skip 0xC
.global $$25857
$$25857:
	.skip 0xC
.global $$25858
$$25858:
	.skip 0xC
.global $$25859
$$25859:
	.skip 0xC
.global $$25860
$$25860:
	.skip 0xC
.global $$25861
$$25861:
	.skip 0xC
.global $$25862
$$25862:
	.skip 0xC
.global $$25863
$$25863:
	.skip 0xC
.global $$25864
$$25864:
	.skip 0xC
.global $$25865
$$25865:
	.skip 0xC
.global $$25866
$$25866:
	.skip 0xC
.global $$25867
$$25867:
	.skip 0xC
.global $$23215
$$23215:
	.skip 0xC
.global $$23223
$$23223:
	.skip 0xC
.global $$23241
$$23241:
	.skip 0xC
.global $$23465
$$23465:
	.skip 0xC
.global $$23478
$$23478:
	.skip 0xC
.global $$25845
$$25845:
	.skip 0xC
.global $$25846
$$25846:
	.skip 0xC
.global $$25847
$$25847:
	.skip 0xC
.global $$25848
$$25848:
	.skip 0xC
.global $$25849
$$25849:
	.skip 0xC
.global $$25850
$$25850:
	.skip 0xC
.global $$25851
$$25851:
	.skip 0xC
.global $$25852
$$25852:
	.skip 0xC
.global $$25853
$$25853:
	.skip 0xC
.global $$25854
$$25854:
	.skip 0xC
.global $$25855
$$25855:
	.skip 0xC
.global $$25856
$$25856:
	.skip 0xC
.global $$25857
$$25857:
	.skip 0xC
.global $$25858
$$25858:
	.skip 0xC
.global $$25859
$$25859:
	.skip 0xC
.global $$23099
$$23099:
	.skip 0xC
.global $$23136
$$23136:
	.skip 0xC
.global $$23162
$$23162:
	.skip 0xC
.global $$23182
$$23182:
	.skip 0xC
.global $$24621
$$24621:
	.skip 0xC
.global $$24622
$$24622:
	.skip 0xC
.global $$24623
$$24623:
	.skip 0xC
.global $$24624
$$24624:
	.skip 0xC
.global $$24625
$$24625:
	.skip 0xC
.global $$24626
$$24626:
	.skip 0xC
.global $$24627
$$24627:
	.skip 0xC
.global $$24628
$$24628:
	.skip 0xC
.global $$24629
$$24629:
	.skip 0xC
.global $$24630
$$24630:
	.skip 0xC
.global $$24631
$$24631:
	.skip 0xC
.global $$24632
$$24632:
	.skip 0xC
.global $$24633
$$24633:
	.skip 0xC
.global $$24634
$$24634:
	.skip 0xC
.global $$24635
$$24635:
	.skip 0x10
.global $$23343
$$23343:
	.skip 0xC
.global $$23352
$$23352:
	.skip 0xC
.global $$23415
$$23415:
	.skip 0xC
.global $$23433
$$23433:
	.skip 0xC
.global $$23447
$$23447:
	.skip 0xC
.global $$23457
$$23457:
	.skip 0xC
.global $$23481
$$23481:
	.skip 0xC
.global $$23508
$$23508:
	.skip 0xC
.global $$25309
$$25309:
	.skip 0xC
.global $$25310
$$25310:
	.skip 0xC
.global $$25311
$$25311:
	.skip 0xC
.global $$25312
$$25312:
	.skip 0xC
.global $$25313
$$25313:
	.skip 0xC
.global $$25314
$$25314:
	.skip 0xC
.global $$25315
$$25315:
	.skip 0xC
.global $$25316
$$25316:
	.skip 0xC
.global $$25317
$$25317:
	.skip 0xC
.global $$25318
$$25318:
	.skip 0xC
.global $$25319
$$25319:
	.skip 0xC
.global $$25320
$$25320:
	.skip 0xC
.global $$25321
$$25321:
	.skip 0xC
.global $$25322
$$25322:
	.skip 0xC
.global $$25323
$$25323:
	.skip 0x10
.global $$23207
$$23207:
	.skip 0xC
.global $$23259
$$23259:
	.skip 0xC
.global $$24032
$$24032:
	.skip 0xC
.global $$24074
$$24074:
	.skip 0xC
.global $$24088
$$24088:
	.skip 0xC
.global $$24098
$$24098:
	.skip 0xC
.global $$24111
$$24111:
	.skip 0xC
.global $$24133
$$24133:
	.skip 0xC
.global $$24149
$$24149:
	.skip 0xC
.global $$24165
$$24165:
	.skip 0xC
.global $$24200
$$24200:
	.skip 0xC
.global $$24211
$$24211:
	.skip 0xC
.global $$29236
$$29236:
	.skip 0xC
.global $$29237
$$29237:
	.skip 0xC
.global $$29238
$$29238:
	.skip 0xC
.global $$29239
$$29239:
	.skip 0xC
.global $$29240
$$29240:
	.skip 0xC
.global $$29241
$$29241:
	.skip 0xC
.global $$29242
$$29242:
	.skip 0xC
.global $$29243
$$29243:
	.skip 0xC
.global $$29244
$$29244:
	.skip 0xC
.global $$29245
$$29245:
	.skip 0xC
.global $$29246
$$29246:
	.skip 0xC
.global $$29247
$$29247:
	.skip 0xC
.global $$29248
$$29248:
	.skip 0xC
.global $$29249
$$29249:
	.skip 0xC
.global $$29250
$$29250:
	.skip 0x10
.global $$22175
$$22175:
	.skip 0x10
.global $$23009
$$23009:
	.skip 0xC
.global $$23030
$$23030:
	.skip 0xC
.global $$23039
$$23039:
	.skip 0xC
.global $$23052
$$23052:
	.skip 0xC
.global $$23076
$$23076:
	.skip 0xC
.global $$23086
$$23086:
	.skip 0xC
.global $$23098
$$23098:
	.skip 0xC
.global $$24266
$$24266:
	.skip 0xC
.global $$24267
$$24267:
	.skip 0xC
.global $$24268
$$24268:
	.skip 0xC
.global $$24269
$$24269:
	.skip 0xC
.global $$24270
$$24270:
	.skip 0xC
.global $$24271
$$24271:
	.skip 0xC
.global $$24272
$$24272:
	.skip 0xC
.global $$24273
$$24273:
	.skip 0xC
.global $$24274
$$24274:
	.skip 0xC
.global $$24275
$$24275:
	.skip 0xC
.global $$24276
$$24276:
	.skip 0xC
.global $$24277
$$24277:
	.skip 0xC
.global $$24278
$$24278:
	.skip 0xC
.global $$24279
$$24279:
	.skip 0xC
.global $$24280
$$24280:
	.skip 0xC
.global $$23282
$$23282:
	.skip 0xC
.global $$23295
$$23295:
	.skip 0xC
.global $$23332
$$23332:
	.skip 0xC
.global $$23365
$$23365:
	.skip 0xC
.global $$23389
$$23389:
	.skip 0xC
.global $$23409
$$23409:
	.skip 0xC
.global $$23445
$$23445:
	.skip 0xC
.global $$23461
$$23461:
	.skip 0xC
.global $$26263
$$26263:
	.skip 0xC
.global $$26264
$$26264:
	.skip 0xC
.global $$26265
$$26265:
	.skip 0xC
.global $$26266
$$26266:
	.skip 0xC
.global $$26267
$$26267:
	.skip 0xC
.global $$26268
$$26268:
	.skip 0xC
.global $$26269
$$26269:
	.skip 0xC
.global $$26270
$$26270:
	.skip 0xC
.global $$26271
$$26271:
	.skip 0xC
.global $$26272
$$26272:
	.skip 0xC
.global $$26273
$$26273:
	.skip 0xC
.global $$26274
$$26274:
	.skip 0xC
.global $$26275
$$26275:
	.skip 0xC
.global $$26276
$$26276:
	.skip 0xC
.global $$26277
$$26277:
	.skip 0x10
.global $$23224
$$23224:
	.skip 0xC
.global $$23236
$$23236:
	.skip 0xC
.global $$23244
$$23244:
	.skip 0xC
.global $$23261
$$23261:
	.skip 0xC
.global $$23278
$$23278:
	.skip 0xC
.global $$23303
$$23303:
	.skip 0xC
.global $$23781
$$23781:
	.skip 0xC
.global $$23810
$$23810:
	.skip 0xC
.global $$23823
$$23823:
	.skip 0xC
.global $$23836
$$23836:
	.skip 0xC
.global $$23854
$$23854:
	.skip 0xC
.global $$23890
$$23890:
	.skip 0xC
.global $$23896
$$23896:
	.skip 0xC
.global $$23904
$$23904:
	.skip 0xC
.global $$23935
$$23935:
	.skip 0xC
.global $$23954
$$23954:
	.skip 0xC
.global $$23981
$$23981:
	.skip 0xC
.global $$29463
$$29463:
	.skip 0xC
.global $$29464
$$29464:
	.skip 0xC
.global $$29465
$$29465:
	.skip 0xC
.global $$29466
$$29466:
	.skip 0xC
.global $$29467
$$29467:
	.skip 0xC
.global $$29468
$$29468:
	.skip 0xC
.global $$29469
$$29469:
	.skip 0xC
.global $$29470
$$29470:
	.skip 0xC
.global $$29471
$$29471:
	.skip 0xC
.global $$29472
$$29472:
	.skip 0xC
.global $$29473
$$29473:
	.skip 0xC
.global $$29474
$$29474:
	.skip 0xC
.global $$29475
$$29475:
	.skip 0xC
.global $$29476
$$29476:
	.skip 0xC
.global $$29477
$$29477:
	.skip 0xC
.global $$22971
$$22971:
	.skip 0xC
.global $$23000
$$23000:
	.skip 0xC
.global $$23153
$$23153:
	.skip 0xC
.global $$24642
$$24642:
	.skip 0xC
.global $$24643
$$24643:
	.skip 0xC
.global $$24644
$$24644:
	.skip 0xC
.global $$24645
$$24645:
	.skip 0xC
.global $$24646
$$24646:
	.skip 0xC
.global $$24647
$$24647:
	.skip 0xC
.global $$24648
$$24648:
	.skip 0xC
.global $$24649
$$24649:
	.skip 0xC
.global $$24650
$$24650:
	.skip 0xC
.global $$24651
$$24651:
	.skip 0xC
.global $$24652
$$24652:
	.skip 0xC
.global $$24653
$$24653:
	.skip 0xC
.global $$24654
$$24654:
	.skip 0xC
.global $$24655
$$24655:
	.skip 0xC
.global $$24656
$$24656:
	.skip 0xC
.global $$22965
$$22965:
	.skip 0xC
.global $$22979
$$22979:
	.skip 0xC
.global $$22997
$$22997:
	.skip 0xC
.global $$23018
$$23018:
	.skip 0xC
.global $$23032
$$23032:
	.skip 0xC
.global $$23054
$$23054:
	.skip 0xC
.global $$23152
$$23152:
	.skip 0xC
.global $$23167
$$23167:
	.skip 0xC
.global $$23208
$$23208:
	.skip 0xC
.global $$23224
$$23224:
	.skip 0xC
.global $$23253
$$23253:
	.skip 0xC
.global $$23264
$$23264:
	.skip 0xC
.global $$25985
$$25985:
	.skip 0xC
.global $$25986
$$25986:
	.skip 0xC
.global $$25987
$$25987:
	.skip 0xC
.global $$25988
$$25988:
	.skip 0xC
.global $$25989
$$25989:
	.skip 0xC
.global $$25990
$$25990:
	.skip 0xC
.global $$25991
$$25991:
	.skip 0xC
.global $$25992
$$25992:
	.skip 0xC
.global $$25993
$$25993:
	.skip 0xC
.global $$25994
$$25994:
	.skip 0xC
.global $$25995
$$25995:
	.skip 0xC
.global $$25996
$$25996:
	.skip 0xC
.global $$25997
$$25997:
	.skip 0xC
.global $$25998
$$25998:
	.skip 0xC
.global $$25999
$$25999:
	.skip 0x10
.global $$23187
$$23187:
	.skip 0xC
.global $$23204
$$23204:
	.skip 0xC
.global $$23223
$$23223:
	.skip 0xC
.global $$23248
$$23248:
	.skip 0xC
.global $$23258
$$23258:
	.skip 0xC
.global $$23267
$$23267:
	.skip 0xC
.global $$25446
$$25446:
	.skip 0xC
.global $$25447
$$25447:
	.skip 0xC
.global $$25448
$$25448:
	.skip 0xC
.global $$25449
$$25449:
	.skip 0xC
.global $$25450
$$25450:
	.skip 0xC
.global $$25451
$$25451:
	.skip 0xC
.global $$25452
$$25452:
	.skip 0xC
.global $$25453
$$25453:
	.skip 0xC
.global $$25454
$$25454:
	.skip 0xC
.global $$25455
$$25455:
	.skip 0xC
.global $$25456
$$25456:
	.skip 0xC
.global $$25457
$$25457:
	.skip 0xC
.global $$25458
$$25458:
	.skip 0xC
.global $$25459
$$25459:
	.skip 0xC
.global $$25460
$$25460:
	.skip 0x10
.global $$22265
$$22265:
	.skip 0xC
.global $$22271
$$22271:
	.skip 0xC
.global up$2718
up$2718:
	.skip 0xC
.global up$2732
up$2732:
	.skip 0xC
.global $$22773
$$22773:
	.skip 0xC
.global $$22795
$$22795:
	.skip 0xC
.global $$23689
$$23689:
	.skip 0xC
.global $$23690
$$23690:
	.skip 0xC
.global $$23691
$$23691:
	.skip 0xC
.global $$23692
$$23692:
	.skip 0xC
.global $$23693
$$23693:
	.skip 0xC
.global $$23694
$$23694:
	.skip 0xC
.global $$23695
$$23695:
	.skip 0xC
.global $$23696
$$23696:
	.skip 0xC
.global $$23697
$$23697:
	.skip 0xC
.global $$23698
$$23698:
	.skip 0xC
.global $$23699
$$23699:
	.skip 0xC
.global $$23700
$$23700:
	.skip 0xC
.global $$23701
$$23701:
	.skip 0xC
.global $$23702
$$23702:
	.skip 0xC
.global $$23703
$$23703:
	.skip 0x10
.global $$24906
$$24906:
	.skip 0xC
.global $$24907
$$24907:
	.skip 0xC
.global $$24908
$$24908:
	.skip 0xC
.global $$24909
$$24909:
	.skip 0xC
.global $$24910
$$24910:
	.skip 0xC
.global $$24911
$$24911:
	.skip 0xC
.global $$24912
$$24912:
	.skip 0xC
.global $$24913
$$24913:
	.skip 0xC
.global $$24914
$$24914:
	.skip 0xC
.global $$24915
$$24915:
	.skip 0xC
.global $$24916
$$24916:
	.skip 0xC
.global $$24917
$$24917:
	.skip 0xC
.global $$24918
$$24918:
	.skip 0xC
.global $$24919
$$24919:
	.skip 0xC
.global $$24920
$$24920:
	.skip 0x10
.global $$22360
$$22360:
	.skip 0xC
.global $$22372
$$22372:
	.skip 0xC
.global $$22384
$$22384:
	.skip 0xC
.global $$22392
$$22392:
	.skip 0xC
.global $$22400
$$22400:
	.skip 0xC
.global $$22409
$$22409:
	.skip 0xC
.global $$22424
$$22424:
	.skip 0xC
.global $$22647
$$22647:
	.skip 0xC
.global $$22648
$$22648:
	.skip 0xC
.global $$22649
$$22649:
	.skip 0xC
.global $$22650
$$22650:
	.skip 0xC
.global $$22651
$$22651:
	.skip 0xC
.global $$22652
$$22652:
	.skip 0xC
.global $$22653
$$22653:
	.skip 0xC
.global $$22654
$$22654:
	.skip 0xC
.global $$22655
$$22655:
	.skip 0xC
.global $$22656
$$22656:
	.skip 0xC
.global $$22657
$$22657:
	.skip 0xC
.global $$22658
$$22658:
	.skip 0xC
.global $$22659
$$22659:
	.skip 0xC
.global $$22660
$$22660:
	.skip 0xC
.global $$22661
$$22661:
	.skip 0xC
.global $$23816
$$23816:
	.skip 0xC
.global $$23817
$$23817:
	.skip 0xC
.global $$23818
$$23818:
	.skip 0xC
.global $$23819
$$23819:
	.skip 0xC
.global $$23820
$$23820:
	.skip 0xC
.global $$23821
$$23821:
	.skip 0xC
.global $$23822
$$23822:
	.skip 0xC
.global $$23823
$$23823:
	.skip 0xC
.global $$23824
$$23824:
	.skip 0xC
.global $$23825
$$23825:
	.skip 0xC
.global $$23826
$$23826:
	.skip 0xC
.global $$23827
$$23827:
	.skip 0xC
.global $$23828
$$23828:
	.skip 0xC
.global $$23829
$$23829:
	.skip 0xC
.global $$23830
$$23830:
	.skip 0x10
.global $$22978
$$22978:
	.skip 0xC
.global $$23008
$$23008:
	.skip 0xC
.global $$23047
$$23047:
	.skip 0xC
.global $$23057
$$23057:
	.skip 0xC
.global $$23082
$$23082:
	.skip 0xC
.global $$24468
$$24468:
	.skip 0xC
.global $$24469
$$24469:
	.skip 0xC
.global $$24470
$$24470:
	.skip 0xC
.global $$24471
$$24471:
	.skip 0xC
.global $$24472
$$24472:
	.skip 0xC
.global $$24473
$$24473:
	.skip 0xC
.global $$24474
$$24474:
	.skip 0xC
.global $$24475
$$24475:
	.skip 0xC
.global $$24476
$$24476:
	.skip 0xC
.global $$24477
$$24477:
	.skip 0xC
.global $$24478
$$24478:
	.skip 0xC
.global $$24479
$$24479:
	.skip 0xC
.global $$24480
$$24480:
	.skip 0xC
.global $$24481
$$24481:
	.skip 0xC
.global $$24482
$$24482:
	.skip 0xC
.global $$23260
$$23260:
	.skip 0xC
.global $$23282
$$23282:
	.skip 0xC
.global $$23301
$$23301:
	.skip 0xC
.global $$23362
$$23362:
	.skip 0xC
.global $$23398
$$23398:
	.skip 0xC
.global $$23412
$$23412:
	.skip 0xC
.global $$23425
$$23425:
	.skip 0xC
.global $$23453
$$23453:
	.skip 0xC
.global $$23467
$$23467:
	.skip 0xC
.global $$23481
$$23481:
	.skip 0xC
.global $$25793
$$25793:
	.skip 0xC
.global $$25794
$$25794:
	.skip 0xC
.global $$25795
$$25795:
	.skip 0xC
.global $$25796
$$25796:
	.skip 0xC
.global $$25797
$$25797:
	.skip 0xC
.global $$25798
$$25798:
	.skip 0xC
.global $$25799
$$25799:
	.skip 0xC
.global $$25800
$$25800:
	.skip 0xC
.global $$25801
$$25801:
	.skip 0xC
.global $$25802
$$25802:
	.skip 0xC
.global $$25803
$$25803:
	.skip 0xC
.global $$25804
$$25804:
	.skip 0xC
.global $$25805
$$25805:
	.skip 0xC
.global $$25806
$$25806:
	.skip 0xC
.global $$25807
$$25807:
	.skip 0x10
.global $$22731
$$22731:
	.skip 0xC
.global $$22732
$$22732:
	.skip 0xC
.global $$22733
$$22733:
	.skip 0xC
.global $$22734
$$22734:
	.skip 0xC
.global $$22735
$$22735:
	.skip 0xC
.global $$22736
$$22736:
	.skip 0xC
.global $$22737
$$22737:
	.skip 0xC
.global $$22738
$$22738:
	.skip 0xC
.global $$22739
$$22739:
	.skip 0xC
.global $$22740
$$22740:
	.skip 0xC
.global $$22741
$$22741:
	.skip 0xC
.global $$22742
$$22742:
	.skip 0xC
.global $$22743
$$22743:
	.skip 0xC
.global $$22744
$$22744:
	.skip 0xC
.global $$22745
$$22745:
	.skip 0x10
.global $$22839
$$22839:
	.skip 0xC
.global $$22857
$$22857:
	.skip 0xC
.global $$22873
$$22873:
	.skip 0xC
.global $$23268
$$23268:
	.skip 0xC
.global $$23269
$$23269:
	.skip 0xC
.global $$23270
$$23270:
	.skip 0xC
.global $$23271
$$23271:
	.skip 0xC
.global $$23272
$$23272:
	.skip 0xC
.global $$23273
$$23273:
	.skip 0xC
.global $$23274
$$23274:
	.skip 0xC
.global $$23275
$$23275:
	.skip 0xC
.global $$23276
$$23276:
	.skip 0xC
.global $$23277
$$23277:
	.skip 0xC
.global $$23278
$$23278:
	.skip 0xC
.global $$23279
$$23279:
	.skip 0xC
.global $$23280
$$23280:
	.skip 0xC
.global $$23281
$$23281:
	.skip 0xC
.global $$23282
$$23282:
	.skip 0xC
.global $$23018
$$23018:
	.skip 0xC
.global $$23019
$$23019:
	.skip 0xC
.global $$23020
$$23020:
	.skip 0xC
.global $$23021
$$23021:
	.skip 0xC
.global $$23022
$$23022:
	.skip 0xC
.global $$23023
$$23023:
	.skip 0xC
.global $$23024
$$23024:
	.skip 0xC
.global $$23025
$$23025:
	.skip 0xC
.global $$23026
$$23026:
	.skip 0xC
.global $$23027
$$23027:
	.skip 0xC
.global $$23028
$$23028:
	.skip 0xC
.global $$23029
$$23029:
	.skip 0xC
.global $$23030
$$23030:
	.skip 0xC
.global $$23031
$$23031:
	.skip 0xC
.global $$23032
$$23032:
	.skip 0x10
.global $$26698
$$26698:
	.skip 0xC
.global $$26699
$$26699:
	.skip 0xC
.global $$26700
$$26700:
	.skip 0xC
.global $$26701
$$26701:
	.skip 0xC
.global $$26702
$$26702:
	.skip 0xC
.global $$26703
$$26703:
	.skip 0xC
.global $$26704
$$26704:
	.skip 0xC
.global $$26705
$$26705:
	.skip 0xC
.global $$26706
$$26706:
	.skip 0xC
.global $$26707
$$26707:
	.skip 0xC
.global $$26708
$$26708:
	.skip 0xC
.global $$26709
$$26709:
	.skip 0xC
.global $$26710
$$26710:
	.skip 0xC
.global $$26711
$$26711:
	.skip 0xC
.global $$26712
$$26712:
	.skip 0x10
.global $$23023
$$23023:
	.skip 0xC
.global $$23024
$$23024:
	.skip 0xC
.global $$23025
$$23025:
	.skip 0xC
.global $$23026
$$23026:
	.skip 0xC
.global $$23027
$$23027:
	.skip 0xC
.global $$23028
$$23028:
	.skip 0xC
.global $$23029
$$23029:
	.skip 0xC
.global $$23030
$$23030:
	.skip 0xC
.global $$23031
$$23031:
	.skip 0xC
.global $$23032
$$23032:
	.skip 0xC
.global $$23033
$$23033:
	.skip 0xC
.global $$23034
$$23034:
	.skip 0xC
.global $$23035
$$23035:
	.skip 0xC
.global $$23036
$$23036:
	.skip 0xC
.global $$23037
$$23037:
	.skip 0x10
.global $$22858
$$22858:
	.skip 0xC
.global $$23486
$$23486:
	.skip 0xC
.global $$23487
$$23487:
	.skip 0xC
.global $$23488
$$23488:
	.skip 0xC
.global $$23489
$$23489:
	.skip 0xC
.global $$23490
$$23490:
	.skip 0xC
.global $$23491
$$23491:
	.skip 0xC
.global $$23492
$$23492:
	.skip 0xC
.global $$23493
$$23493:
	.skip 0xC
.global $$23494
$$23494:
	.skip 0xC
.global $$23495
$$23495:
	.skip 0xC
.global $$23496
$$23496:
	.skip 0xC
.global $$23497
$$23497:
	.skip 0xC
.global $$23498
$$23498:
	.skip 0xC
.global $$23499
$$23499:
	.skip 0xC
.global $$23500
$$23500:
	.skip 0xC
.global $$22571
$$22571:
	.skip 0xC
.global $$22672
$$22672:
	.skip 0xC
.global $$22684
$$22684:
	.skip 0xC
.global $$22692
$$22692:
	.skip 0xC
.global $$22703
$$22703:
	.skip 0xC
.global $$22711
$$22711:
	.skip 0xC
.global $$22721
$$22721:
	.skip 0xC
.global $$22729
$$22729:
	.skip 0xC
.global $$24138
$$24138:
	.skip 0xC
.global $$24139
$$24139:
	.skip 0xC
.global $$24140
$$24140:
	.skip 0xC
.global $$24141
$$24141:
	.skip 0xC
.global $$24142
$$24142:
	.skip 0xC
.global $$24143
$$24143:
	.skip 0xC
.global $$24144
$$24144:
	.skip 0xC
.global $$24145
$$24145:
	.skip 0xC
.global $$24146
$$24146:
	.skip 0xC
.global $$24147
$$24147:
	.skip 0xC
.global $$24148
$$24148:
	.skip 0xC
.global $$24149
$$24149:
	.skip 0xC
.global $$24150
$$24150:
	.skip 0xC
.global $$24151
$$24151:
	.skip 0xC
.global $$24152
$$24152:
	.skip 0x10
.global $$22896
$$22896:
	.skip 0xC
.global $$22910
$$22910:
	.skip 0xC
.global $$22932
$$22932:
	.skip 0xC
.global $$23752
$$23752:
	.skip 0xC
.global $$23753
$$23753:
	.skip 0xC
.global $$23754
$$23754:
	.skip 0xC
.global $$23755
$$23755:
	.skip 0xC
.global $$23756
$$23756:
	.skip 0xC
.global $$23757
$$23757:
	.skip 0xC
.global $$23758
$$23758:
	.skip 0xC
.global $$23759
$$23759:
	.skip 0xC
.global $$23760
$$23760:
	.skip 0xC
.global $$23761
$$23761:
	.skip 0xC
.global $$23762
$$23762:
	.skip 0xC
.global $$23763
$$23763:
	.skip 0xC
.global $$23764
$$23764:
	.skip 0xC
.global $$23765
$$23765:
	.skip 0xC
.global $$23766
$$23766:
	.skip 0xC
.global $$22841
$$22841:
	.skip 0xC
.global $$22853
$$22853:
	.skip 0xC
.global $$22863
$$22863:
	.skip 0xC
.global $$22875
$$22875:
	.skip 0xC
.global $$22888
$$22888:
	.skip 0xC
.global $$22899
$$22899:
	.skip 0xC
.global $$22910
$$22910:
	.skip 0xC
.global $$22920
$$22920:
	.skip 0xC
.global $$24907
$$24907:
	.skip 0xC
.global $$24908
$$24908:
	.skip 0xC
.global $$24909
$$24909:
	.skip 0xC
.global $$24910
$$24910:
	.skip 0xC
.global $$24911
$$24911:
	.skip 0xC
.global $$24912
$$24912:
	.skip 0xC
.global $$24913
$$24913:
	.skip 0xC
.global $$24914
$$24914:
	.skip 0xC
.global $$24915
$$24915:
	.skip 0xC
.global $$24916
$$24916:
	.skip 0xC
.global $$24917
$$24917:
	.skip 0xC
.global $$24918
$$24918:
	.skip 0xC
.global $$24919
$$24919:
	.skip 0xC
.global $$24920
$$24920:
	.skip 0xC
.global $$24921
$$24921:
	.skip 0x10
.global $$22919
$$22919:
	.skip 0xC
.global $$22925
$$22925:
	.skip 0xC
.global $$22931
$$22931:
	.skip 0xC
.global $$22937
$$22937:
	.skip 0xC
.global $$22943
$$22943:
	.skip 0xC
.global $$25909
$$25909:
	.skip 0xC
.global $$25910
$$25910:
	.skip 0xC
.global $$25911
$$25911:
	.skip 0xC
.global $$25912
$$25912:
	.skip 0xC
.global $$25913
$$25913:
	.skip 0xC
.global $$25914
$$25914:
	.skip 0xC
.global $$25915
$$25915:
	.skip 0xC
.global $$25916
$$25916:
	.skip 0xC
.global $$25917
$$25917:
	.skip 0xC
.global $$25918
$$25918:
	.skip 0xC
.global $$25919
$$25919:
	.skip 0xC
.global $$25920
$$25920:
	.skip 0xC
.global $$25921
$$25921:
	.skip 0xC
.global $$25922
$$25922:
	.skip 0xC
.global $$25923
$$25923:
	.skip 0xC
.global $$22876
$$22876:
	.skip 0xC
.global $$22888
$$22888:
	.skip 0xC
.global $$22901
$$22901:
	.skip 0xC
.global $$22911
$$22911:
	.skip 0xC
.global $$22923
$$22923:
	.skip 0xC
.global $$24148
$$24148:
	.skip 0xC
.global $$24149
$$24149:
	.skip 0xC
.global $$24150
$$24150:
	.skip 0xC
.global $$24151
$$24151:
	.skip 0xC
.global $$24152
$$24152:
	.skip 0xC
.global $$24153
$$24153:
	.skip 0xC
.global $$24154
$$24154:
	.skip 0xC
.global $$24155
$$24155:
	.skip 0xC
.global $$24156
$$24156:
	.skip 0xC
.global $$24157
$$24157:
	.skip 0xC
.global $$24158
$$24158:
	.skip 0xC
.global $$24159
$$24159:
	.skip 0xC
.global $$24160
$$24160:
	.skip 0xC
.global $$24161
$$24161:
	.skip 0xC
.global $$24162
$$24162:
	.skip 0xC
.global TKoopaJr_jointIndexTable
TKoopaJr_jointIndexTable:
	.skip 0x14
.global $$23119
$$23119:
	.skip 0xC
.global $$23127
$$23127:
	.skip 0xC
.global $$23139
$$23139:
	.skip 0xC
.global $$23147
$$23147:
	.skip 0xC
.global $$23157
$$23157:
	.skip 0xC
.global $$23321
$$23321:
	.skip 0xC
.global $$23329
$$23329:
	.skip 0xC
.global $$23339
$$23339:
	.skip 0xC
.global $$26227
$$26227:
	.skip 0xC
.global $$26228
$$26228:
	.skip 0xC
.global $$26229
$$26229:
	.skip 0xC
.global $$26230
$$26230:
	.skip 0xC
.global $$26231
$$26231:
	.skip 0xC
.global $$26232
$$26232:
	.skip 0xC
.global $$26233
$$26233:
	.skip 0xC
.global $$26234
$$26234:
	.skip 0xC
.global $$26235
$$26235:
	.skip 0xC
.global $$26236
$$26236:
	.skip 0xC
.global $$26237
$$26237:
	.skip 0xC
.global $$26238
$$26238:
	.skip 0xC
.global $$26239
$$26239:
	.skip 0xC
.global $$26240
$$26240:
	.skip 0xC
.global $$26241
$$26241:
	.skip 0xC
.global $$22663
$$22663:
	.skip 0xC
.global $$22675
$$22675:
	.skip 0xC
.global $$22687
$$22687:
	.skip 0xC
.global $$22697
$$22697:
	.skip 0xC
.global $$22714
$$22714:
	.skip 0xC
.global $$22727
$$22727:
	.skip 0xC
.global $$22744
$$22744:
	.skip 0xC
.global $$22754
$$22754:
	.skip 0xC
.global $$22765
$$22765:
	.skip 0x10
.global $$22978
$$22978:
	.skip 0xC
.global $$22984
$$22984:
	.skip 0xC
.global $$22990
$$22990:
	.skip 0xC
.global $$22994
$$22994:
	.skip 0xC
.global $$22998
$$22998:
	.skip 0xC
.global $$23002
$$23002:
	.skip 0xC
.global $$23006
$$23006:
	.skip 0xC
.global $$23010
$$23010:
	.skip 0xC
.global $$23014
$$23014:
	.skip 0xC
.global $$23018
$$23018:
	.skip 0xC
.global $$28079
$$28079:
	.skip 0xC
.global $$28080
$$28080:
	.skip 0xC
.global $$28081
$$28081:
	.skip 0xC
.global $$28082
$$28082:
	.skip 0xC
.global $$28083
$$28083:
	.skip 0xC
.global $$28084
$$28084:
	.skip 0xC
.global $$28085
$$28085:
	.skip 0xC
.global $$28086
$$28086:
	.skip 0xC
.global $$28087
$$28087:
	.skip 0xC
.global $$28088
$$28088:
	.skip 0xC
.global $$28089
$$28089:
	.skip 0xC
.global $$28090
$$28090:
	.skip 0xC
.global $$28091
$$28091:
	.skip 0xC
.global $$28092
$$28092:
	.skip 0xC
.global $$28093
$$28093:
	.skip 0x10
.global $$23101
$$23101:
	.skip 0xC
.global $$23116
$$23116:
	.skip 0xC
.global $$23128
$$23128:
	.skip 0xC
.global $$23143
$$23143:
	.skip 0xC
.global $$23155
$$23155:
	.skip 0xC
.global $$24837
$$24837:
	.skip 0xC
.global $$24838
$$24838:
	.skip 0xC
.global $$24839
$$24839:
	.skip 0xC
.global $$24840
$$24840:
	.skip 0xC
.global $$24841
$$24841:
	.skip 0xC
.global $$24842
$$24842:
	.skip 0xC
.global $$24843
$$24843:
	.skip 0xC
.global $$24844
$$24844:
	.skip 0xC
.global $$24845
$$24845:
	.skip 0xC
.global $$24846
$$24846:
	.skip 0xC
.global $$24847
$$24847:
	.skip 0xC
.global $$24848
$$24848:
	.skip 0xC
.global $$24849
$$24849:
	.skip 0xC
.global $$24850
$$24850:
	.skip 0xC
.global $$24851
$$24851:
	.skip 0xC
.global $$22530
$$22530:
	.skip 0xC
.global $$22542
$$22542:
	.skip 0xC
.global $$22556
$$22556:
	.skip 0xC
.global $$22566
$$22566:
	.skip 0xC
.global $$22576
$$22576:
	.skip 0x10
.global $$22938
$$22938:
	.skip 0xC
.global $$22944
$$22944:
	.skip 0xC
.global $$23508
$$23508:
	.skip 0xC
.global $$23509
$$23509:
	.skip 0xC
.global $$23510
$$23510:
	.skip 0xC
.global $$23511
$$23511:
	.skip 0xC
.global $$23512
$$23512:
	.skip 0xC
.global $$23513
$$23513:
	.skip 0xC
.global $$23514
$$23514:
	.skip 0xC
.global $$23515
$$23515:
	.skip 0xC
.global $$23516
$$23516:
	.skip 0xC
.global $$23517
$$23517:
	.skip 0xC
.global $$23518
$$23518:
	.skip 0xC
.global $$23519
$$23519:
	.skip 0xC
.global $$23520
$$23520:
	.skip 0xC
.global $$23521
$$23521:
	.skip 0xC
.global $$23522
$$23522:
	.skip 0x10
.global $$23126
$$23126:
	.skip 0xC
.global $$23140
$$23140:
	.skip 0xC
.global $$23155
$$23155:
	.skip 0xC
.global $$23169
$$23169:
	.skip 0xC
.global $$23179
$$23179:
	.skip 0xC
.global $$23189
$$23189:
	.skip 0xC
.global $$25824
$$25824:
	.skip 0xC
.global $$25825
$$25825:
	.skip 0xC
.global $$25826
$$25826:
	.skip 0xC
.global $$25827
$$25827:
	.skip 0xC
.global $$25828
$$25828:
	.skip 0xC
.global $$25829
$$25829:
	.skip 0xC
.global $$25830
$$25830:
	.skip 0xC
.global $$25831
$$25831:
	.skip 0xC
.global $$25832
$$25832:
	.skip 0xC
.global $$25833
$$25833:
	.skip 0xC
.global $$25834
$$25834:
	.skip 0xC
.global $$25835
$$25835:
	.skip 0xC
.global $$25836
$$25836:
	.skip 0xC
.global $$25837
$$25837:
	.skip 0xC
.global $$25838
$$25838:
	.skip 0x10
.global $$22869
$$22869:
	.skip 0xC
.global $$22913
$$22913:
	.skip 0xC
.global $$23890
$$23890:
	.skip 0xC
.global $$23891
$$23891:
	.skip 0xC
.global $$23892
$$23892:
	.skip 0xC
.global $$23893
$$23893:
	.skip 0xC
.global $$23894
$$23894:
	.skip 0xC
.global $$23895
$$23895:
	.skip 0xC
.global $$23896
$$23896:
	.skip 0xC
.global $$23897
$$23897:
	.skip 0xC
.global $$23898
$$23898:
	.skip 0xC
.global $$23899
$$23899:
	.skip 0xC
.global $$23900
$$23900:
	.skip 0xC
.global $$23901
$$23901:
	.skip 0xC
.global $$23902
$$23902:
	.skip 0xC
.global $$23903
$$23903:
	.skip 0xC
.global $$23904
$$23904:
	.skip 0x10
.global $$22850
$$22850:
	.skip 0xC
.global $$23125
$$23125:
	.skip 0xC
.global $$23126
$$23126:
	.skip 0xC
.global $$23127
$$23127:
	.skip 0xC
.global $$23128
$$23128:
	.skip 0xC
.global $$23129
$$23129:
	.skip 0xC
.global $$23130
$$23130:
	.skip 0xC
.global $$23131
$$23131:
	.skip 0xC
.global $$23132
$$23132:
	.skip 0xC
.global $$23133
$$23133:
	.skip 0xC
.global $$23134
$$23134:
	.skip 0xC
.global $$23135
$$23135:
	.skip 0xC
.global $$23136
$$23136:
	.skip 0xC
.global $$23137
$$23137:
	.skip 0xC
.global $$23138
$$23138:
	.skip 0xC
.global $$23139
$$23139:
	.skip 0xC
.global $$22871
$$22871:
	.skip 0xC
.global $$22886
$$22886:
	.skip 0xC
.global $$22896
$$22896:
	.skip 0xC
.global $$22909
$$22909:
	.skip 0xC
.global $$22922
$$22922:
	.skip 0xC
.global $$22928
$$22928:
	.skip 0xC
.global $$22938
$$22938:
	.skip 0xC
.global $$24971
$$24971:
	.skip 0xC
.global $$24972
$$24972:
	.skip 0xC
.global $$24973
$$24973:
	.skip 0xC
.global $$24974
$$24974:
	.skip 0xC
.global $$24975
$$24975:
	.skip 0xC
.global $$24976
$$24976:
	.skip 0xC
.global $$24977
$$24977:
	.skip 0xC
.global $$24978
$$24978:
	.skip 0xC
.global $$24979
$$24979:
	.skip 0xC
.global $$24980
$$24980:
	.skip 0xC
.global $$24981
$$24981:
	.skip 0xC
.global $$24982
$$24982:
	.skip 0xC
.global $$24983
$$24983:
	.skip 0xC
.global $$24984
$$24984:
	.skip 0xC
.global $$24985
$$24985:
	.skip 0xC
.global $$22944
$$22944:
	.skip 0xC
.global $$22948
$$22948:
	.skip 0xC
.global $$22969
$$22969:
	.skip 0xC
.global $$22975
$$22975:
	.skip 0xC
.global $$22981
$$22981:
	.skip 0xC
.global $$22985
$$22985:
	.skip 0xC
.global $$22989
$$22989:
	.skip 0xC
.global $$22993
$$22993:
	.skip 0xC
.global $$22997
$$22997:
	.skip 0xC
.global $$23001
$$23001:
	.skip 0xC
.global $$23005
$$23005:
	.skip 0xC
.global $$25479
$$25479:
	.skip 0xC
.global $$25480
$$25480:
	.skip 0xC
.global $$25481
$$25481:
	.skip 0xC
.global $$25482
$$25482:
	.skip 0xC
.global $$25483
$$25483:
	.skip 0xC
.global $$25484
$$25484:
	.skip 0xC
.global $$25485
$$25485:
	.skip 0xC
.global $$25486
$$25486:
	.skip 0xC
.global $$25487
$$25487:
	.skip 0xC
.global $$25488
$$25488:
	.skip 0xC
.global $$25489
$$25489:
	.skip 0xC
.global $$25490
$$25490:
	.skip 0xC
.global $$25491
$$25491:
	.skip 0xC
.global $$25492
$$25492:
	.skip 0xC
.global $$25493
$$25493:
	.skip 0xC
.global $$23000
$$23000:
	.skip 0xC
.global $$23012
$$23012:
	.skip 0xC
.global $$23026
$$23026:
	.skip 0xC
.global $$23036
$$23036:
	.skip 0xC
.global $$23843
$$23843:
	.skip 0xC
.global $$23844
$$23844:
	.skip 0xC
.global $$23845
$$23845:
	.skip 0xC
.global $$23846
$$23846:
	.skip 0xC
.global $$23847
$$23847:
	.skip 0xC
.global $$23848
$$23848:
	.skip 0xC
.global $$23849
$$23849:
	.skip 0xC
.global $$23850
$$23850:
	.skip 0xC
.global $$23851
$$23851:
	.skip 0xC
.global $$23852
$$23852:
	.skip 0xC
.global $$23853
$$23853:
	.skip 0xC
.global $$23854
$$23854:
	.skip 0xC
.global $$23855
$$23855:
	.skip 0xC
.global $$23856
$$23856:
	.skip 0xC
.global $$23857
$$23857:
	.skip 0x10
.global $$23186
$$23186:
	.skip 0xC
.global $$23187
$$23187:
	.skip 0xC
.global $$23188
$$23188:
	.skip 0xC
.global $$23189
$$23189:
	.skip 0xC
.global $$23190
$$23190:
	.skip 0xC
.global $$23191
$$23191:
	.skip 0xC
.global $$23192
$$23192:
	.skip 0xC
.global $$23193
$$23193:
	.skip 0xC
.global $$23194
$$23194:
	.skip 0xC
.global $$23195
$$23195:
	.skip 0xC
.global $$23196
$$23196:
	.skip 0xC
.global $$23197
$$23197:
	.skip 0xC
.global $$23198
$$23198:
	.skip 0xC
.global $$23199
$$23199:
	.skip 0xC
.global $$23200
$$23200:
	.skip 0x10
.global CLBConstUpVec
CLBConstUpVec:
	.skip 0x10
.global $$22240
$$22240:
	.skip 0xC
.global $$22241
$$22241:
	.skip 0xC
.global $$22242
$$22242:
	.skip 0xC
.global $$22243
$$22243:
	.skip 0xC
.global $$22244
$$22244:
	.skip 0xC
.global $$22245
$$22245:
	.skip 0xC
.global $$22246
$$22246:
	.skip 0xC
.global $$22247
$$22247:
	.skip 0xC
.global $$22248
$$22248:
	.skip 0xC
.global $$22249
$$22249:
	.skip 0xC
.global $$22250
$$22250:
	.skip 0xC
.global $$22251
$$22251:
	.skip 0xC
.global $$22252
$$22252:
	.skip 0xC
.global $$22253
$$22253:
	.skip 0xC
.global $$22254
$$22254:
	.skip 0x10
.global $$22284
$$22284:
	.skip 0xC
.global $$22285
$$22285:
	.skip 0xC
.global $$22286
$$22286:
	.skip 0xC
.global $$22287
$$22287:
	.skip 0xC
.global $$22288
$$22288:
	.skip 0xC
.global $$22289
$$22289:
	.skip 0xC
.global $$22290
$$22290:
	.skip 0xC
.global $$22291
$$22291:
	.skip 0xC
.global $$22292
$$22292:
	.skip 0xC
.global $$22293
$$22293:
	.skip 0xC
.global $$22294
$$22294:
	.skip 0xC
.global $$22295
$$22295:
	.skip 0xC
.global $$22296
$$22296:
	.skip 0xC
.global $$22297
$$22297:
	.skip 0xC
.global $$22298
$$22298:
	.skip 0x10
.global $$22300
$$22300:
	.skip 0x10
.global $$23346
$$23346:
	.skip 0xC
.global $$23347
$$23347:
	.skip 0xC
.global $$23348
$$23348:
	.skip 0xC
.global $$23349
$$23349:
	.skip 0xC
.global $$23350
$$23350:
	.skip 0xC
.global $$23351
$$23351:
	.skip 0xC
.global $$23352
$$23352:
	.skip 0xC
.global $$23353
$$23353:
	.skip 0xC
.global $$23354
$$23354:
	.skip 0xC
.global $$23355
$$23355:
	.skip 0xC
.global $$23356
$$23356:
	.skip 0xC
.global $$23357
$$23357:
	.skip 0xC
.global $$23358
$$23358:
	.skip 0xC
.global $$23359
$$23359:
	.skip 0xC
.global $$23360
$$23360:
	.skip 0x10
.global $$22616
$$22616:
	.skip 0xC
.global $$22617
$$22617:
	.skip 0xC
.global $$22618
$$22618:
	.skip 0xC
.global $$22619
$$22619:
	.skip 0xC
.global $$22620
$$22620:
	.skip 0xC
.global $$22621
$$22621:
	.skip 0xC
.global $$22622
$$22622:
	.skip 0xC
.global $$22623
$$22623:
	.skip 0xC
.global $$22624
$$22624:
	.skip 0xC
.global $$22625
$$22625:
	.skip 0xC
.global $$22626
$$22626:
	.skip 0xC
.global $$22627
$$22627:
	.skip 0xC
.global $$22628
$$22628:
	.skip 0xC
.global $$22629
$$22629:
	.skip 0xC
.global $$22630
$$22630:
	.skip 0x10
.global $$22001
$$22001:
	.skip 0x10
.global $$23026
$$23026:
	.skip 0xC
.global $$23041
$$23041:
	.skip 0xC
.global $$23056
$$23056:
	.skip 0xC
.global $$23068
$$23068:
	.skip 0xC
.global $$23078
$$23078:
	.skip 0xC
.global $$23097
$$23097:
	.skip 0xC
.global $$23105
$$23105:
	.skip 0xC
.global $$23117
$$23117:
	.skip 0xC
.global $$23129
$$23129:
	.skip 0xC
.global $$24981
$$24981:
	.skip 0xC
.global $$24982
$$24982:
	.skip 0xC
.global $$24983
$$24983:
	.skip 0xC
.global $$24984
$$24984:
	.skip 0xC
.global $$24985
$$24985:
	.skip 0xC
.global $$24986
$$24986:
	.skip 0xC
.global $$24987
$$24987:
	.skip 0xC
.global $$24988
$$24988:
	.skip 0xC
.global $$24989
$$24989:
	.skip 0xC
.global $$24990
$$24990:
	.skip 0xC
.global $$24991
$$24991:
	.skip 0xC
.global $$24992
$$24992:
	.skip 0xC
.global $$24993
$$24993:
	.skip 0xC
.global $$24994
$$24994:
	.skip 0xC
.global $$24995
$$24995:
	.skip 0xC
.global $$22990
$$22990:
	.skip 0xC
.global $$23004
$$23004:
	.skip 0xC
.global $$23014
$$23014:
	.skip 0xC
.global $$23024
$$23024:
	.skip 0xC
.global $$23036
$$23036:
	.skip 0xC
.global $$23048
$$23048:
	.skip 0xC
.global $$25213
$$25213:
	.skip 0xC
.global $$25214
$$25214:
	.skip 0xC
.global $$25215
$$25215:
	.skip 0xC
.global $$25216
$$25216:
	.skip 0xC
.global $$25217
$$25217:
	.skip 0xC
.global $$25218
$$25218:
	.skip 0xC
.global $$25219
$$25219:
	.skip 0xC
.global $$25220
$$25220:
	.skip 0xC
.global $$25221
$$25221:
	.skip 0xC
.global $$25222
$$25222:
	.skip 0xC
.global $$25223
$$25223:
	.skip 0xC
.global $$25224
$$25224:
	.skip 0xC
.global $$25225
$$25225:
	.skip 0xC
.global $$25226
$$25226:
	.skip 0xC
.global $$25227
$$25227:
	.skip 0x10
.global $$22422
$$22422:
	.skip 0xC
.global sAramList__11JKRAramHeap
sAramList__11JKRAramHeap:
	.skip 0xC
.global sAramCommandList__7JKRAram
sAramCommandList__7JKRAram:
	.skip 0xC
.global sDvdList__10JKRDvdFile
sDvdList__10JKRDvdFile:
	.skip 0xC
.global sVolumeList__13JKRFileLoader
sVolumeList__13JKRFileLoader:
	.skip 0xC
.global sThreadList__9JKRThread
sThreadList__9JKRThread:
	.skip 0xC
.global sMapFileList__12JUTException
sMapFileList__12JUTException:
	.skip 0xC
.global mPadList__10JUTGamePad
mPadList__10JUTGamePad:
	.skip 0x17C
.global mPadStatus__10JUTGamePad
mPadStatus__10JUTGamePad:
	.skip 0x30
.global mPadButton__10JUTGamePad
mPadButton__10JUTGamePad:
	.skip 0xC0
.global mPadMStick__10JUTGamePad
mPadMStick__10JUTGamePad:
	.skip 0x40
.global mPadSStick__10JUTGamePad
mPadSStick__10JUTGamePad:
	.skip 0x40
.global mMutex__8J2DPrint
mMutex__8J2DPrint:
	.skip 0x18
.global j3dSys
j3dSys:
	.skip 0x1AC
.global mCurrentMtx__6J3DSys
mCurrentMtx__6J3DSys:
	.skip 0x30
.global mCurrentS__6J3DSys
mCurrentS__6J3DSys:
	.skip 0xC
.global mParentS__6J3DSys
mParentS__6J3DSys:
	.skip 0xC
.global sTexCoordScaleTable__6J3DSys
sTexCoordScaleTable__6J3DSys:
	.skip 0x40
.global sDvdAsyncList__12JKRDvdRipper
sDvdAsyncList__12JKRDvdRipper:
	.skip 0xC
.global sDvdAramAsyncList__16JKRDvdAramRipper
sDvdAramAsyncList__16JKRDvdAramRipper:
	.skip 0xC
.global j3dTexCoordTable
j3dTexCoordTable:
	.skip 0x2528
.global j3dTevSwapTableTable
j3dTevSwapTableTable:
	.skip 0x400
.global j3dAlphaCmpTable
j3dAlphaCmpTable:
	.skip 0x300
.global j3dZModeTable
j3dZModeTable:
	.skip 0x60
.global sAramPieceCommandList__12JKRAramPiece
sAramPieceCommandList__12JKRAramPiece:
	.skip 0x24
.global mMutex__12JKRAramPiece
mMutex__12JKRAramPiece:
	.skip 0x18
.global JAInullCamera
JAInullCamera:
	.skip 0xC
.global nullActor__8JAIConst
nullActor__8JAIConst:
	.skip 0x58
.global camMtx__8JAIConst
camMtx__8JAIConst:
	.skip 0x30
.global camTrans__8JAIConst
camTrans__8JAIConst:
	.skip 0xC
.global camPreTrans__8JAIConst
camPreTrans__8JAIConst:
	.skip 0xC
.global streamHeap__Q27JAInter9StreamLib
streamHeap__Q27JAInter9StreamLib:
	.skip 0x14
.global systemPortCmd__18JAISystemInterface
systemPortCmd__18JAISystemInterface:
	.skip 0x18
.global jac_audioThread__Q28JASystem11AudioThread
jac_audioThread__Q28JASystem11AudioThread:
	.skip 0x2640
.global jac_audioStack__Q28JASystem11AudioThread
jac_audioStack__Q28JASystem11AudioThread:
	.skip 0x1000
.global jac_dvdThread__Q28JASystem11AudioThread
jac_dvdThread__Q28JASystem11AudioThread:
	.skip 0x320
.global jac_dvdStack__Q28JASystem11AudioThread
jac_dvdStack__Q28JASystem11AudioThread:
	.skip 0x1000
.global sParser__Q28JASystem6TTrack
sParser__Q28JASystem6TTrack:
	.skip 0x4
.global vc__7JPADraw
vc__7JPADraw:
	.skip 0x1EC
.global cb__7JPADraw
cb__7JPADraw:
	.skip 0xB0

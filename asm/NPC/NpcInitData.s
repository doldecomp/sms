.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global SMSGetNpcInitData__FUl
SMSGetNpcInitData__FUl:
/* 80175424 00172364  3C 80 80 3C */	lis r4, sAllNpcInitData$1622@ha
/* 80175428 00172368  54 63 10 3A */	slwi r3, r3, 2
/* 8017542C 0017236C  38 04 94 70 */	addi r0, r4, sAllNpcInitData$1622@l
/* 80175430 00172370  7C 60 1A 14 */	add r3, r0, r3
/* 80175434 00172374  80 63 00 00 */	lwz r3, 0(r3)
/* 80175438 00172378  4E 80 00 20 */	blr 

.global __sinit_NpcInitData_cpp
__sinit_NpcInitData_cpp:
/* 8017543C 0017237C  3C 60 80 3C */	lis r3, sMareMB_FishingRodData@ha
/* 80175440 00172380  80 0D 84 80 */	lwz r0, cNpcPartsNameRootJoint-_SDA_BASE_(r13)
/* 80175444 00172384  38 83 94 18 */	addi r4, r3, sMareMB_FishingRodData@l
/* 80175448 00172388  90 04 00 00 */	stw r0, 0(r4)
/* 8017544C 0017238C  3C 60 80 3C */	lis r3, sMareWB_BabyData@ha
/* 80175450 00172390  90 03 94 44 */	stw r0, sMareWB_BabyData@l(r3)
/* 80175454 00172394  4E 80 00 20 */	blr 

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
	.incbin "baserom.dol", 0x36CCCC, 0x4

.section .rodata, "wa"  # 0x8036FFA0 - 0x803A8380
.global $$21490
$$21490:
	.incbin "baserom.dol", 0x37CC20, 0xC
.global $$21526
$$21526:
	.incbin "baserom.dol", 0x37CC2C, 0x14
.global $$21530
$$21530:
	.incbin "baserom.dol", 0x37CC40, 0x10
.global $$21535
$$21535:
	.incbin "baserom.dol", 0x37CC50, 0x24
.global $$21536
$$21536:
	.incbin "baserom.dol", 0x37CC74, 0x14
.global sMonteM_HatAColorBuf0
sMonteM_HatAColorBuf0:
	.incbin "baserom.dol", 0x37CC88, 0x38
.global sMonteM_HatAColorBuf1
sMonteM_HatAColorBuf1:
	.incbin "baserom.dol", 0x37CCC0, 0x38
.global $$21537
$$21537:
	.incbin "baserom.dol", 0x37CCF8, 0xC
.global sMonteM_HatAColorChange
sMonteM_HatAColorChange:
	.incbin "baserom.dol", 0x37CD04, 0x10
.global sMonteM_HatABeltColorBuf
sMonteM_HatABeltColorBuf:
	.incbin "baserom.dol", 0x37CD14, 0x38
.global $$21538
$$21538:
	.incbin "baserom.dol", 0x37CD4C, 0xC
.global sMonteM_HatABeltColorChange
sMonteM_HatABeltColorChange:
	.incbin "baserom.dol", 0x37CD58, 0x10
.global $$21540
$$21540:
	.incbin "baserom.dol", 0x37CD68, 0x10
.global sMonteM_HatAData
sMonteM_HatAData:
	.incbin "baserom.dol", 0x37CD78, 0x2C
.global sMonteM_HatBColorBuf
sMonteM_HatBColorBuf:
	.incbin "baserom.dol", 0x37CDA4, 0x18
.global sMonteM_HatBColorChange
sMonteM_HatBColorChange:
	.incbin "baserom.dol", 0x37CDBC, 0x10
.global sMonteM_HatBBeltColorBuf
sMonteM_HatBBeltColorBuf:
	.incbin "baserom.dol", 0x37CDCC, 0x18
.global sMonteM_HatBBeltColorChange
sMonteM_HatBBeltColorChange:
	.incbin "baserom.dol", 0x37CDE4, 0x10
.global $$21541
$$21541:
	.incbin "baserom.dol", 0x37CDF4, 0x10
.global sMonteM_HatBData
sMonteM_HatBData:
	.incbin "baserom.dol", 0x37CE04, 0x2C
.global sMonteM_HatDColorBuf
sMonteM_HatDColorBuf:
	.incbin "baserom.dol", 0x37CE30, 0x10
.global sMonteM_HatDColorChange
sMonteM_HatDColorChange:
	.incbin "baserom.dol", 0x37CE40, 0x10
.global $$21542
$$21542:
	.incbin "baserom.dol", 0x37CE50, 0x10
.global sMonteM_HatDData
sMonteM_HatDData:
	.incbin "baserom.dol", 0x37CE60, 0x2C
.global sMonteM_HatEColorBuf0
sMonteM_HatEColorBuf0:
	.incbin "baserom.dol", 0x37CE8C, 0x10
.global sMonteM_HatEColorBuf1
sMonteM_HatEColorBuf1:
	.incbin "baserom.dol", 0x37CE9C, 0x10
.global sMonteM_HatEColorChange
sMonteM_HatEColorChange:
	.incbin "baserom.dol", 0x37CEAC, 0x10
.global $$21543
$$21543:
	.incbin "baserom.dol", 0x37CEBC, 0x10
.global sMonteM_HatEData
sMonteM_HatEData:
	.incbin "baserom.dol", 0x37CECC, 0x2C
.global sMonteM_HatFColorBuf0
sMonteM_HatFColorBuf0:
	.incbin "baserom.dol", 0x37CEF8, 0x10
.global sMonteM_HatFColorBuf1
sMonteM_HatFColorBuf1:
	.incbin "baserom.dol", 0x37CF08, 0x10
.global sMonteM_HatFColorChange
sMonteM_HatFColorChange:
	.incbin "baserom.dol", 0x37CF18, 0x10
.global $$21544
$$21544:
	.incbin "baserom.dol", 0x37CF28, 0x10
.global sMonteM_HatFData
sMonteM_HatFData:
	.incbin "baserom.dol", 0x37CF38, 0x2C
.global sMonteM_HatGColorBuf0
sMonteM_HatGColorBuf0:
	.incbin "baserom.dol", 0x37CF64, 0x28
.global sMonteM_HatGColorBuf1
sMonteM_HatGColorBuf1:
	.incbin "baserom.dol", 0x37CF8C, 0x28
.global sMonteM_HatGColorChange
sMonteM_HatGColorChange:
	.incbin "baserom.dol", 0x37CFB4, 0x10
.global $$21545
$$21545:
	.incbin "baserom.dol", 0x37CFC4, 0x10
.global sMonteM_HatGData
sMonteM_HatGData:
	.incbin "baserom.dol", 0x37CFD4, 0x2C
.global sMonteM_MustacheAColorBuf0
sMonteM_MustacheAColorBuf0:
	.incbin "baserom.dol", 0x37D000, 0x4
.global lbl_80380004
lbl_80380004:
	.incbin "baserom.dol", 0x37D004, 0x1C
.global $$21546
$$21546:
	.incbin "baserom.dol", 0x37D020, 0xC
.global sMonteM_MustacheAColorChange
sMonteM_MustacheAColorChange:
	.incbin "baserom.dol", 0x37D02C, 0x10
.global $$21547
$$21547:
	.incbin "baserom.dol", 0x37D03C, 0x10
.global sMonteM_MustacheAData
sMonteM_MustacheAData:
	.incbin "baserom.dol", 0x37D04C, 0x2C
.global $$21548
$$21548:
	.incbin "baserom.dol", 0x37D078, 0x14
.global sMonteM_GlassesAData
sMonteM_GlassesAData:
	.incbin "baserom.dol", 0x37D08C, 0x2C
.global sMonteM_GlassesBColorBuf0
sMonteM_GlassesBColorBuf0:
	.incbin "baserom.dol", 0x37D0B8, 0x18
.global $$21549
$$21549:
	.incbin "baserom.dol", 0x37D0D0, 0xC
.global sMonteM_GlassesBColorChange
sMonteM_GlassesBColorChange:
	.incbin "baserom.dol", 0x37D0DC, 0x10
.global $$21550
$$21550:
	.incbin "baserom.dol", 0x37D0EC, 0x14
.global sMonteM_GlassesBData
sMonteM_GlassesBData:
	.incbin "baserom.dol", 0x37D100, 0x2C
.global sMonteM_LapelAColorBuf0
sMonteM_LapelAColorBuf0:
	.incbin "baserom.dol", 0x37D12C, 0x10
.global sMonteM_LapelAColorBuf1
sMonteM_LapelAColorBuf1:
	.incbin "baserom.dol", 0x37D13C, 0x10
.global $$21551
$$21551:
	.incbin "baserom.dol", 0x37D14C, 0xC
.global sMonteM_LapelAColorChange
sMonteM_LapelAColorChange:
	.incbin "baserom.dol", 0x37D158, 0x10
.global $$21552
$$21552:
	.incbin "baserom.dol", 0x37D168, 0xC
.global $$21553
$$21553:
	.incbin "baserom.dol", 0x37D174, 0x10
.global sMonteM_LapelAData
sMonteM_LapelAData:
	.incbin "baserom.dol", 0x37D184, 0x2C
.global $$21554
$$21554:
	.incbin "baserom.dol", 0x37D1B0, 0xC
.global sMonteM_TieBColorChange
sMonteM_TieBColorChange:
	.incbin "baserom.dol", 0x37D1BC, 0x10
.global $$21555
$$21555:
	.incbin "baserom.dol", 0x37D1CC, 0x10
.global sMonteM_TieBData
sMonteM_TieBData:
	.incbin "baserom.dol", 0x37D1DC, 0x2C
.global $$21556
$$21556:
	.incbin "baserom.dol", 0x37D208, 0x14
.global sMonteM_LuggageData
sMonteM_LuggageData:
	.incbin "baserom.dol", 0x37D21C, 0x2C
.global $$21557
$$21557:
	.incbin "baserom.dol", 0x37D248, 0x10
.global sMonteMF_TubeData
sMonteMF_TubeData:
	.incbin "baserom.dol", 0x37D258, 0x2C
.global $$21558
$$21558:
	.incbin "baserom.dol", 0x37D284, 0xC
.global $$21559
$$21559:
	.incbin "baserom.dol", 0x37D290, 0x10
.global sMonteMG_MopData
sMonteMG_MopData:
	.incbin "baserom.dol", 0x37D2A0, 0x2C
.global $$21560
$$21560:
	.incbin "baserom.dol", 0x37D2CC, 0x14
.global sMonteMH_UkleleData
sMonteMH_UkleleData:
	.incbin "baserom.dol", 0x37D2E0, 0x2C
.global sMonteW_FlowerInColorBuf
sMonteW_FlowerInColorBuf:
	.incbin "baserom.dol", 0x37D30C, 0x18
.global $$21561
$$21561:
	.incbin "baserom.dol", 0x37D324, 0xC
.global sMonteW_FlowerInColorChange
sMonteW_FlowerInColorChange:
	.incbin "baserom.dol", 0x37D330, 0x10
.global sMonteW_FlowerOutColorBuf
sMonteW_FlowerOutColorBuf:
	.incbin "baserom.dol", 0x37D340, 0x18
.global $$21562
$$21562:
	.incbin "baserom.dol", 0x37D358, 0xC
.global sMonteW_FlowerOutColorChange
sMonteW_FlowerOutColorChange:
	.incbin "baserom.dol", 0x37D364, 0x10
.global $$21563
$$21563:
	.incbin "baserom.dol", 0x37D374, 0xC
.global $$21564
$$21564:
	.incbin "baserom.dol", 0x37D380, 0x14
.global sMonteW_FlowerData
sMonteW_FlowerData:
	.incbin "baserom.dol", 0x37D394, 0x2C
.global sMonteW_HatAMainColorBuf0
sMonteW_HatAMainColorBuf0:
	.incbin "baserom.dol", 0x37D3C0, 0x20
.global sMonteW_HatAMainColorBuf1
sMonteW_HatAMainColorBuf1:
	.incbin "baserom.dol", 0x37D3E0, 0x20
.global sMonteW_HatAMainColorChange
sMonteW_HatAMainColorChange:
	.incbin "baserom.dol", 0x37D400, 0x10
.global sMonteW_HatABeltColorBuf
sMonteW_HatABeltColorBuf:
	.incbin "baserom.dol", 0x37D410, 0x20
.global sMonteW_HatABeltColorChange
sMonteW_HatABeltColorChange:
	.incbin "baserom.dol", 0x37D430, 0x10
.global $$21565
$$21565:
	.incbin "baserom.dol", 0x37D440, 0x10
.global sMonteW_HatAData
sMonteW_HatAData:
	.incbin "baserom.dol", 0x37D450, 0x2C
.global sMonteW_GlassBColorBuf
sMonteW_GlassBColorBuf:
	.incbin "baserom.dol", 0x37D47C, 0x18
.global sMonteW_GlassBColorChange
sMonteW_GlassBColorChange:
	.incbin "baserom.dol", 0x37D494, 0x10
.global $$21566
$$21566:
	.incbin "baserom.dol", 0x37D4A4, 0x10
.global sMonteW_GlassBData
sMonteW_GlassBData:
	.incbin "baserom.dol", 0x37D4B4, 0x2C
.global $$21567
$$21567:
	.incbin "baserom.dol", 0x37D4E0, 0x14
.global sMonteW_ArrowRData
sMonteW_ArrowRData:
	.incbin "baserom.dol", 0x37D4F4, 0x2C
.global $$21568
$$21568:
	.incbin "baserom.dol", 0x37D520, 0x14
.global sMonteW_ArrowLData
sMonteW_ArrowLData:
	.incbin "baserom.dol", 0x37D534, 0x2C
.global $$21569
$$21569:
	.incbin "baserom.dol", 0x37D560, 0x10
.global sMonteWC_HatCData
sMonteWC_HatCData:
	.incbin "baserom.dol", 0x37D570, 0x2C
.global $$21570
$$21570:
	.incbin "baserom.dol", 0x37D59C, 0x14
.global sMonteWC_BraceletRData
sMonteWC_BraceletRData:
	.incbin "baserom.dol", 0x37D5B0, 0x2C
.global $$21571
$$21571:
	.incbin "baserom.dol", 0x37D5DC, 0xC
.global $$21572
$$21572:
	.incbin "baserom.dol", 0x37D5E8, 0x14
.global sMonteWC_BraceletLData
sMonteWC_BraceletLData:
	.incbin "baserom.dol", 0x37D5FC, 0x2C
.global sMareM_HatAColorBuf0
sMareM_HatAColorBuf0:
	.incbin "baserom.dol", 0x37D628, 0x48
.global sMareM_HatAColorBuf1
sMareM_HatAColorBuf1:
	.incbin "baserom.dol", 0x37D670, 0x48
.global sMareM_HatAColorChange
sMareM_HatAColorChange:
	.incbin "baserom.dol", 0x37D6B8, 0x10
.global $$21574
$$21574:
	.incbin "baserom.dol", 0x37D6C8, 0x10
.global sMareM_HatAData
sMareM_HatAData:
	.incbin "baserom.dol", 0x37D6D8, 0x2C
.global sMareM_HatBColorBuf0
sMareM_HatBColorBuf0:
	.incbin "baserom.dol", 0x37D704, 0x30
.global sMareM_HatBColorBuf1
sMareM_HatBColorBuf1:
	.incbin "baserom.dol", 0x37D734, 0x30
.global sMareM_HatBColorChange
sMareM_HatBColorChange:
	.incbin "baserom.dol", 0x37D764, 0x10
.global $$21575
$$21575:
	.incbin "baserom.dol", 0x37D774, 0x10
.global sMareM_HatBData
sMareM_HatBData:
	.incbin "baserom.dol", 0x37D784, 0x2C
.global sMareM_HatEColorBuf0
sMareM_HatEColorBuf0:
	.incbin "baserom.dol", 0x37D7B0, 0x58
.global sMareM_HatEColorBuf1
sMareM_HatEColorBuf1:
	.incbin "baserom.dol", 0x37D808, 0x58
.global sMareM_HatEColorChange
sMareM_HatEColorChange:
	.incbin "baserom.dol", 0x37D860, 0x10
.global $$21576
$$21576:
	.incbin "baserom.dol", 0x37D870, 0x10
.global sMareM_HatEData
sMareM_HatEData:
	.incbin "baserom.dol", 0x37D880, 0x2C
.global sMareM_ShellAColorBuf0
sMareM_ShellAColorBuf0:
	.incbin "baserom.dol", 0x37D8AC, 0x50
.global sMareM_ShellAColorBuf1
sMareM_ShellAColorBuf1:
	.incbin "baserom.dol", 0x37D8FC, 0x50
.global sMareM_ShellAColorChange
sMareM_ShellAColorChange:
	.incbin "baserom.dol", 0x37D94C, 0x10
.global $$21578
$$21578:
	.incbin "baserom.dol", 0x37D95C, 0x14
.global sMareM_ShellAData
sMareM_ShellAData:
	.incbin "baserom.dol", 0x37D970, 0x2C
.global sMareM_ShellBColorBuf0
sMareM_ShellBColorBuf0:
	.incbin "baserom.dol", 0x37D99C, 0x50
.global sMareM_ShellBColorBuf1
sMareM_ShellBColorBuf1:
	.incbin "baserom.dol", 0x37D9EC, 0x50
.global sMareM_ShellBColorChange
sMareM_ShellBColorChange:
	.incbin "baserom.dol", 0x37DA3C, 0x10
.global $$21579
$$21579:
	.incbin "baserom.dol", 0x37DA4C, 0x14
.global sMareM_ShellBData
sMareM_ShellBData:
	.incbin "baserom.dol", 0x37DA60, 0x2C
.global sMareM_BivalveBColorBuf0
sMareM_BivalveBColorBuf0:
	.incbin "baserom.dol", 0x37DA8C, 0x30
.global sMareM_BivalveBColorBuf1
sMareM_BivalveBColorBuf1:
	.incbin "baserom.dol", 0x37DABC, 0x30
.global sMareM_BivalveBColorChange
sMareM_BivalveBColorChange:
	.incbin "baserom.dol", 0x37DAEC, 0x10
.global $$21580
$$21580:
	.incbin "baserom.dol", 0x37DAFC, 0x14
.global sMareM_BivalveBData
sMareM_BivalveBData:
	.incbin "baserom.dol", 0x37DB10, 0x2C
.global sMareM_GlassColorBuf0
sMareM_GlassColorBuf0:
	.incbin "baserom.dol", 0x37DB3C, 0x18
.global sMareM_GlassColorBuf1
sMareM_GlassColorBuf1:
	.incbin "baserom.dol", 0x37DB54, 0x18
.global $$21581
$$21581:
	.incbin "baserom.dol", 0x37DB6C, 0xC
.global sMareM_GlassColorChange
sMareM_GlassColorChange:
	.incbin "baserom.dol", 0x37DB78, 0x10
.global $$21582
$$21582:
	.incbin "baserom.dol", 0x37DB88, 0x10
.global sMareM_GlassData
sMareM_GlassData:
	.incbin "baserom.dol", 0x37DB98, 0x2C
.global sMareMB_ElderMat1ColorChange
sMareMB_ElderMat1ColorChange:
	.incbin "baserom.dol", 0x37DBC4, 0x10
.global sMareMB_ElderMat2ColorChange
sMareMB_ElderMat2ColorChange:
	.incbin "baserom.dol", 0x37DBD4, 0x10
.global sMareMB_ElderMat3ColorChange
sMareMB_ElderMat3ColorChange:
	.incbin "baserom.dol", 0x37DBE4, 0x10
.global $$21585
$$21585:
	.incbin "baserom.dol", 0x37DBF4, 0x10
.global sMareMB_ElderData
sMareMB_ElderData:
	.incbin "baserom.dol", 0x37DC04, 0x2C
.global sMareMB_FishingRodMat1ColorChange
sMareMB_FishingRodMat1ColorChange:
	.incbin "baserom.dol", 0x37DC30, 0x10
.global sMareMB_FishingRodMat2ColorChange
sMareMB_FishingRodMat2ColorChange:
	.incbin "baserom.dol", 0x37DC40, 0x10
.global sMareMB_FishingRodMat3ColorChange
sMareMB_FishingRodMat3ColorChange:
	.incbin "baserom.dol", 0x37DC50, 0x10
.global $$21586
$$21586:
	.incbin "baserom.dol", 0x37DC60, 0x14
.global sMareMC_HatFColorChange
sMareMC_HatFColorChange:
	.incbin "baserom.dol", 0x37DC74, 0x10
.global $$21587
$$21587:
	.incbin "baserom.dol", 0x37DC84, 0x10
.global sMareMC_HatFData
sMareMC_HatFData:
	.incbin "baserom.dol", 0x37DC94, 0x2C
.global sMareMC_BeardColorChange
sMareMC_BeardColorChange:
	.incbin "baserom.dol", 0x37DCC0, 0x10
.global $$21588
$$21588:
	.incbin "baserom.dol", 0x37DCD0, 0x14
.global sMareMC_BeardData
sMareMC_BeardData:
	.incbin "baserom.dol", 0x37DCE4, 0x2C
.global sMareMC_MustacheColorChange
sMareMC_MustacheColorChange:
	.incbin "baserom.dol", 0x37DD10, 0x10
.global $$21589
$$21589:
	.incbin "baserom.dol", 0x37DD20, 0x14
.global sMareMC_MustacheData
sMareMC_MustacheData:
	.incbin "baserom.dol", 0x37DD34, 0x2C
.global sMareMD_PipeColorChange
sMareMD_PipeColorChange:
	.incbin "baserom.dol", 0x37DD60, 0x10
.global $$21591
$$21591:
	.incbin "baserom.dol", 0x37DD70, 0x14
.global sMareMD_PipeData
sMareMD_PipeData:
	.incbin "baserom.dol", 0x37DD84, 0x2C
.global sMareW_PearlAColorBuf0
sMareW_PearlAColorBuf0:
	.incbin "baserom.dol", 0x37DDB0, 0x30
.global sMareW_PearlAColorBuf1
sMareW_PearlAColorBuf1:
	.incbin "baserom.dol", 0x37DDE0, 0x30
.global sMareW_PearlAColorChange
sMareW_PearlAColorChange:
	.incbin "baserom.dol", 0x37DE10, 0x10
.global $$21592
$$21592:
	.incbin "baserom.dol", 0x37DE20, 0x14
.global sMareW_PearlAData
sMareW_PearlAData:
	.incbin "baserom.dol", 0x37DE34, 0x2C
.global sMareW_HatAColorBuf0
sMareW_HatAColorBuf0:
	.incbin "baserom.dol", 0x37DE60, 0x30
.global sMareW_HatAColorBuf1
sMareW_HatAColorBuf1:
	.incbin "baserom.dol", 0x37DE90, 0x30
.global sMareW_HatAColorChange
sMareW_HatAColorChange:
	.incbin "baserom.dol", 0x37DEC0, 0x10
.global $$21593
$$21593:
	.incbin "baserom.dol", 0x37DED0, 0x10
.global sMareW_HatAData
sMareW_HatAData:
	.incbin "baserom.dol", 0x37DEE0, 0x2C
.global sMareW_HatDColorBuf0
sMareW_HatDColorBuf0:
	.incbin "baserom.dol", 0x37DF0C, 0x58
.global sMareW_HatDColorBuf1
sMareW_HatDColorBuf1:
	.incbin "baserom.dol", 0x37DF64, 0x58
.global sMareW_HatDColorChange
sMareW_HatDColorChange:
	.incbin "baserom.dol", 0x37DFBC, 0x10
.global $$21594
$$21594:
	.incbin "baserom.dol", 0x37DFCC, 0x10
.global sMareW_HatDData
sMareW_HatDData:
	.incbin "baserom.dol", 0x37DFDC, 0x2C
.global sMareW_ShellAColorBuf0
sMareW_ShellAColorBuf0:
	.incbin "baserom.dol", 0x37E008, 0x30
.global sMareW_ShellAColorBuf1
sMareW_ShellAColorBuf1:
	.incbin "baserom.dol", 0x37E038, 0x30
.global sMareW_ShellAColorChange
sMareW_ShellAColorChange:
	.incbin "baserom.dol", 0x37E068, 0x10
.global $$21595
$$21595:
	.incbin "baserom.dol", 0x37E078, 0x10
.global sMareW_ShellAData
sMareW_ShellAData:
	.incbin "baserom.dol", 0x37E088, 0x2C
.global sMareW_ShellBColorBuf0
sMareW_ShellBColorBuf0:
	.incbin "baserom.dol", 0x37E0B4, 0x48
.global sMareW_ShellBColorBuf1
sMareW_ShellBColorBuf1:
	.incbin "baserom.dol", 0x37E0FC, 0x48
.global sMareW_ShellBColorChange
sMareW_ShellBColorChange:
	.incbin "baserom.dol", 0x37E144, 0x10
.global $$21596
$$21596:
	.incbin "baserom.dol", 0x37E154, 0x10
.global sMareW_ShellBData
sMareW_ShellBData:
	.incbin "baserom.dol", 0x37E164, 0x2C
.global sMareW_BivalveAColorBuf0
sMareW_BivalveAColorBuf0:
	.incbin "baserom.dol", 0x37E190, 0x30
.global sMareW_BivalveAColorBuf1
sMareW_BivalveAColorBuf1:
	.incbin "baserom.dol", 0x37E1C0, 0x30
.global sMareW_BivalveAColorChange
sMareW_BivalveAColorChange:
	.incbin "baserom.dol", 0x37E1F0, 0x10
.global $$21597
$$21597:
	.incbin "baserom.dol", 0x37E200, 0x14
.global sMareW_BivalveAData
sMareW_BivalveAData:
	.incbin "baserom.dol", 0x37E214, 0x2C
.global sMareWB_BabyBodyColorChange
sMareWB_BabyBodyColorChange:
	.incbin "baserom.dol", 0x37E240, 0x10
.global $$21599
$$21599:
	.incbin "baserom.dol", 0x37E250, 0xC
.global sMareWB_BabyHatColorChange
sMareWB_BabyHatColorChange:
	.incbin "baserom.dol", 0x37E25C, 0x10
.global $$21600
$$21600:
	.incbin "baserom.dol", 0x37E26C, 0xC
.global sMareWB_BabyBedColorChange
sMareWB_BabyBedColorChange:
	.incbin "baserom.dol", 0x37E278, 0x10
.global $$21601
$$21601:
	.incbin "baserom.dol", 0x37E288, 0x10
.global $$21602
$$21602:
	.incbin "baserom.dol", 0x37E298, 0x18
.global sKinopio_SunGlassesData
sKinopio_SunGlassesData:
	.incbin "baserom.dol", 0x37E2B0, 0x2C
.global $$21603
$$21603:
	.incbin "baserom.dol", 0x37E2DC, 0x10
.global $$21604
$$21604:
	.incbin "baserom.dol", 0x37E2EC, 0x14
.global sKinojii_StickData
sKinojii_StickData:
	.incbin "baserom.dol", 0x37E300, 0x2C
.global $$21605
$$21605:
	.incbin "baserom.dol", 0x37E32C, 0x18
.global sPeach_HairNormalData
sPeach_HairNormalData:
	.incbin "baserom.dol", 0x37E344, 0x2C
.global $$21606
$$21606:
	.incbin "baserom.dol", 0x37E370, 0xC
.global $$21607
$$21607:
	.incbin "baserom.dol", 0x37E37C, 0x14
.global sPeach_Hand1LData
sPeach_Hand1LData:
	.incbin "baserom.dol", 0x37E390, 0x2C
.global $$21608
$$21608:
	.incbin "baserom.dol", 0x37E3BC, 0xC
.global $$21609
$$21609:
	.incbin "baserom.dol", 0x37E3C8, 0x14
.global sPeach_Hand1RData
sPeach_Hand1RData:
	.incbin "baserom.dol", 0x37E3DC, 0x2C
.global $$21610
$$21610:
	.incbin "baserom.dol", 0x37E408, 0x18
.global sPeach_HairPonytailData
sPeach_HairPonytailData:
	.incbin "baserom.dol", 0x37E420, 0x2C
.global $$21611
$$21611:
	.incbin "baserom.dol", 0x37E44C, 0x14
.global sPeach_ParasolData
sPeach_ParasolData:
	.incbin "baserom.dol", 0x37E460, 0x2C
.global $$21612
$$21612:
	.incbin "baserom.dol", 0x37E48C, 0x14
.global sPeach_Hand2LData
sPeach_Hand2LData:
	.incbin "baserom.dol", 0x37E4A0, 0x2C
.global $$21613
$$21613:
	.incbin "baserom.dol", 0x37E4CC, 0x14
.global sPeach_Hand2RData
sPeach_Hand2RData:
	.incbin "baserom.dol", 0x37E4E0, 0x2C
.global sRaccoonDog_TubeColorBuf0
sRaccoonDog_TubeColorBuf0:
	.incbin "baserom.dol", 0x37E50C, 0x10
.global sRaccoonDog_TubeColorBuf1
sRaccoonDog_TubeColorBuf1:
	.incbin "baserom.dol", 0x37E51C, 0x10
.global sRaccoonDog_TubeColorChange
sRaccoonDog_TubeColorChange:
	.incbin "baserom.dol", 0x37E52C, 0x10
.global $$21615
$$21615:
	.incbin "baserom.dol", 0x37E53C, 0xC
.global $$21616
$$21616:
	.incbin "baserom.dol", 0x37E548, 0xC
.global sRaccoonDog_TubeData
sRaccoonDog_TubeData:
	.incbin "baserom.dol", 0x37E554, 0x2C
.global sMonteM_BodyColorBuf
sMonteM_BodyColorBuf:
	.incbin "baserom.dol", 0x37E580, 0x50
.global $$21617
$$21617:
	.incbin "baserom.dol", 0x37E5D0, 0xC
.global sMonteM_BodyColor
sMonteM_BodyColor:
	.incbin "baserom.dol", 0x37E5DC, 0x10
.global sMonteMA_ClothColorBuf0
sMonteMA_ClothColorBuf0:
	.incbin "baserom.dol", 0x37E5EC, 0x58
.global sMonteMA_ClothColorBuf1
sMonteMA_ClothColorBuf1:
	.incbin "baserom.dol", 0x37E644, 0x58
.global $$21618
$$21618:
	.incbin "baserom.dol", 0x37E69C, 0xC
.global sMonteMA_ClothColor
sMonteMA_ClothColor:
	.incbin "baserom.dol", 0x37E6A8, 0x10
.global sMonteMB_BodyColorBuf
sMonteMB_BodyColorBuf:
	.incbin "baserom.dol", 0x37E6B8, 0x20
.global sMonteMB_BodyColor
sMonteMB_BodyColor:
	.incbin "baserom.dol", 0x37E6D8, 0x10
.global sMonteMB_ClothColorBuf
sMonteMB_ClothColorBuf:
	.incbin "baserom.dol", 0x37E6E8, 0x30
.global sMonteMB_ClothColor
sMonteMB_ClothColor:
	.incbin "baserom.dol", 0x37E718, 0x10
.global sMonteMC_ClothColorBuf0
sMonteMC_ClothColorBuf0:
	.incbin "baserom.dol", 0x37E728, 0x58
.global sMonteMC_ClothColorBuf1
sMonteMC_ClothColorBuf1:
	.incbin "baserom.dol", 0x37E780, 0x58
.global sMonteMC_ClothColor
sMonteMC_ClothColor:
	.incbin "baserom.dol", 0x37E7D8, 0x10
.global sMonteMD_ClothColorBuf
sMonteMD_ClothColorBuf:
	.incbin "baserom.dol", 0x37E7E8, 0x28
.global sMonteMD_ClothColor
sMonteMD_ClothColor:
	.incbin "baserom.dol", 0x37E810, 0x10
.global sMonteW_ColorBuf
sMonteW_ColorBuf:
	.incbin "baserom.dol", 0x37E820, 0x30
.global sMonteW_BodyColor
sMonteW_BodyColor:
	.incbin "baserom.dol", 0x37E850, 0x10
.global sMonteWA_ClothColorBuf
sMonteWA_ClothColorBuf:
	.incbin "baserom.dol", 0x37E860, 0x30
.global sMonteWA_ClothColor
sMonteWA_ClothColor:
	.incbin "baserom.dol", 0x37E890, 0x10
.global sMonteWB_ClothColorBuf0
sMonteWB_ClothColorBuf0:
	.incbin "baserom.dol", 0x37E8A0, 0x48
.global sMonteWB_ClothColorBuf1
sMonteWB_ClothColorBuf1:
	.incbin "baserom.dol", 0x37E8E8, 0x48
.global sMonteWB_ClothColor
sMonteWB_ClothColor:
	.incbin "baserom.dol", 0x37E930, 0x10
.global sMareM_ColorBuf
sMareM_ColorBuf:
	.incbin "baserom.dol", 0x37E940, 0x30
.global sMareM_BodyColor
sMareM_BodyColor:
	.incbin "baserom.dol", 0x37E970, 0x10
.global sMareW_ColorBuf
sMareW_ColorBuf:
	.incbin "baserom.dol", 0x37E980, 0x30
.global sMareW_BodyColor
sMareW_BodyColor:
	.incbin "baserom.dol", 0x37E9B0, 0x10
.global sKinopio_BodyColorData0
sKinopio_BodyColorData0:
	.incbin "baserom.dol", 0x37E9C0, 0x28
.global sKinopio_BodyColorData1
sKinopio_BodyColorData1:
	.incbin "baserom.dol", 0x37E9E8, 0x28
.global $$21619
$$21619:
	.incbin "baserom.dol", 0x37EA10, 0xC
.global sKinopio_BodyColor
sKinopio_BodyColor:
	.incbin "baserom.dol", 0x37EA1C, 0x10
.global sKinopio_ClothColorData0
sKinopio_ClothColorData0:
	.incbin "baserom.dol", 0x37EA2C, 0x28
.global sKinopio_ClothColorData1
sKinopio_ClothColorData1:
	.incbin "baserom.dol", 0x37EA54, 0x28
.global $$21620
$$21620:
	.incbin "baserom.dol", 0x37EA7C, 0xC
.global sKinopio_ClothColor
sKinopio_ClothColor:
	.incbin "baserom.dol", 0x37EA88, 0x10
.global sRaccoonDog_ClothColorBuf0
sRaccoonDog_ClothColorBuf0:
	.incbin "baserom.dol", 0x37EA98, 0x10
.global sRaccoonDog_ClothColorBuf1
sRaccoonDog_ClothColorBuf1:
	.incbin "baserom.dol", 0x37EAA8, 0x10
.global sRaccoonDog_ClothColor
sRaccoonDog_ClothColor:
	.incbin "baserom.dol", 0x37EAB8, 0x10
.global sMonteM_InitData
sMonteM_InitData:
	.incbin "baserom.dol", 0x37EAC8, 0x54
.global sMonteMA_InitData
sMonteMA_InitData:
	.incbin "baserom.dol", 0x37EB1C, 0x54
.global sMonteMB_InitData
sMonteMB_InitData:
	.incbin "baserom.dol", 0x37EB70, 0x54
.global sMonteMC_InitData
sMonteMC_InitData:
	.incbin "baserom.dol", 0x37EBC4, 0x54
.global sMonteMD_InitData
sMonteMD_InitData:
	.incbin "baserom.dol", 0x37EC18, 0x54
.global sMonteME_InitData
sMonteME_InitData:
	.incbin "baserom.dol", 0x37EC6C, 0x54
.global sMonteMF_InitData
sMonteMF_InitData:
	.incbin "baserom.dol", 0x37ECC0, 0x54
.global sMonteMG_InitData
sMonteMG_InitData:
	.incbin "baserom.dol", 0x37ED14, 0x54
.global sMonteMH_InitData
sMonteMH_InitData:
	.incbin "baserom.dol", 0x37ED68, 0x54
.global sMonteW_InitData
sMonteW_InitData:
	.incbin "baserom.dol", 0x37EDBC, 0x54
.global sMonteWA_InitData
sMonteWA_InitData:
	.incbin "baserom.dol", 0x37EE10, 0x54
.global sMonteWB_InitData
sMonteWB_InitData:
	.incbin "baserom.dol", 0x37EE64, 0x54
.global sMonteWC_InitData
sMonteWC_InitData:
	.incbin "baserom.dol", 0x37EEB8, 0x54
.global sMareM_InitData
sMareM_InitData:
	.incbin "baserom.dol", 0x37EF0C, 0x54
.global sMareMB_InitData
sMareMB_InitData:
	.incbin "baserom.dol", 0x37EF60, 0x54
.global sMareMC_InitData
sMareMC_InitData:
	.incbin "baserom.dol", 0x37EFB4, 0x54
.global sMareMD_InitData
sMareMD_InitData:
	.incbin "baserom.dol", 0x37F008, 0x54
.global sMareW_InitData
sMareW_InitData:
	.incbin "baserom.dol", 0x37F05C, 0x54
.global sMareWB_InitData
sMareWB_InitData:
	.incbin "baserom.dol", 0x37F0B0, 0x54
.global sKinopio_InitData
sKinopio_InitData:
	.incbin "baserom.dol", 0x37F104, 0x54
.global sKinojii_InitData
sKinojii_InitData:
	.incbin "baserom.dol", 0x37F158, 0x54
.global sPeach_InitData
sPeach_InitData:
	.incbin "baserom.dol", 0x37F1AC, 0x54
.global sRaccoonDog_InitData
sRaccoonDog_InitData:
	.incbin "baserom.dol", 0x37F200, 0x54
.global sSunflowerL_InitData
sSunflowerL_InitData:
	.incbin "baserom.dol", 0x37F254, 0x54
.global sSunflowerS_InitData
sSunflowerS_InitData:
	.incbin "baserom.dol", 0x37F2A8, 0x54
.global sBoardNpc_InitData
sBoardNpc_InitData:
	.incbin "baserom.dol", 0x37F2FC, 0x54

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3B63F0, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3B63FC, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3B6408, 0x10
.global sMareMB_FishingRodData
sMareMB_FishingRodData:
	.incbin "baserom.dol", 0x3B6418, 0x2C
.global sMareWB_BabyData
sMareWB_BabyData:
	.incbin "baserom.dol", 0x3B6444, 0x2C
.global sAllNpcInitData$1622
sAllNpcInitData$1622:
	.incbin "baserom.dol", 0x3B6470, 0x78

.section .sdata2, "wa"  # 0x8040B460 - 0x80414020
.global sPeach_TakenMiddle
sPeach_TakenMiddle:
	.incbin "baserom.dol", 0x3E6478, 0x8
.global $$21539
$$21539:
	.incbin "baserom.dol", 0x3E6480, 0x8
.global sMonteM_TieBColorBuf0
sMonteM_TieBColorBuf0:
	.incbin "baserom.dol", 0x3E6488, 0x8
.global sMonteM_TieBColorBuf1
sMonteM_TieBColorBuf1:
	.incbin "baserom.dol", 0x3E6490, 0x8
.global $$21573
$$21573:
	.incbin "baserom.dol", 0x3E6498, 0x8
.global $$21577
$$21577:
	.incbin "baserom.dol", 0x3E64A0, 0x8
.global sMareMB_ElderMat1ColorBuf0
sMareMB_ElderMat1ColorBuf0:
	.incbin "baserom.dol", 0x3E64A8, 0x8
.global sMareMB_ElderMat1ColorBuf1
sMareMB_ElderMat1ColorBuf1:
	.incbin "baserom.dol", 0x3E64B0, 0x8
.global sMareMB_ElderMat2ColorBuf0
sMareMB_ElderMat2ColorBuf0:
	.incbin "baserom.dol", 0x3E64B8, 0x8
.global sMareMB_ElderMat2ColorBuf1
sMareMB_ElderMat2ColorBuf1:
	.incbin "baserom.dol", 0x3E64C0, 0x8
.global $$21583
$$21583:
	.incbin "baserom.dol", 0x3E64C8, 0x8
.global sMareMB_ElderMat3ColorBuf0
sMareMB_ElderMat3ColorBuf0:
	.incbin "baserom.dol", 0x3E64D0, 0x8
.global sMareMB_ElderMat3ColorBuf1
sMareMB_ElderMat3ColorBuf1:
	.incbin "baserom.dol", 0x3E64D8, 0x8
.global $$21584
$$21584:
	.incbin "baserom.dol", 0x3E64E0, 0x8
.global sMareMB_FishingRodMat1ColorBuf0
sMareMB_FishingRodMat1ColorBuf0:
	.incbin "baserom.dol", 0x3E64E8, 0x8
.global sMareMB_FishingRodMat1ColorBuf1
sMareMB_FishingRodMat1ColorBuf1:
	.incbin "baserom.dol", 0x3E64F0, 0x8
.global sMareMB_FishingRodMat2ColorBuf0
sMareMB_FishingRodMat2ColorBuf0:
	.incbin "baserom.dol", 0x3E64F8, 0x8
.global sMareMB_FishingRodMat2ColorBuf1
sMareMB_FishingRodMat2ColorBuf1:
	.incbin "baserom.dol", 0x3E6500, 0x8
.global sMareMB_FishingRodMat3ColorBuf0
sMareMB_FishingRodMat3ColorBuf0:
	.incbin "baserom.dol", 0x3E6508, 0x8
.global sMareMB_FishingRodMat3ColorBuf1
sMareMB_FishingRodMat3ColorBuf1:
	.incbin "baserom.dol", 0x3E6510, 0x8
.global sMareMC_HatFColorBuf0
sMareMC_HatFColorBuf0:
	.incbin "baserom.dol", 0x3E6518, 0x8
.global sMareMC_HatFColorBuf1
sMareMC_HatFColorBuf1:
	.incbin "baserom.dol", 0x3E6520, 0x8
.global sMareMC_BeardColorBuf0
sMareMC_BeardColorBuf0:
	.incbin "baserom.dol", 0x3E6528, 0x8
.global sMareMC_BeardColorBuf1
sMareMC_BeardColorBuf1:
	.incbin "baserom.dol", 0x3E6530, 0x8
.global sMareMC_MustacheColorBuf0
sMareMC_MustacheColorBuf0:
	.incbin "baserom.dol", 0x3E6538, 0x8
.global sMareMC_MustacheColorBuf1
sMareMC_MustacheColorBuf1:
	.incbin "baserom.dol", 0x3E6540, 0x8
.global sMareMD_PipeColorBuf0
sMareMD_PipeColorBuf0:
	.incbin "baserom.dol", 0x3E6548, 0x8
.global sMareMD_PipeColorBuf1
sMareMD_PipeColorBuf1:
	.incbin "baserom.dol", 0x3E6550, 0x8
.global $$21590
$$21590:
	.incbin "baserom.dol", 0x3E6558, 0x8
.global sMareWB_BabyBodyColorBuf0
sMareWB_BabyBodyColorBuf0:
	.incbin "baserom.dol", 0x3E6560, 0x8
.global $$21598
$$21598:
	.incbin "baserom.dol", 0x3E6568, 0x8
.global sMareWB_BabyHatColorBuf0
sMareWB_BabyHatColorBuf0:
	.incbin "baserom.dol", 0x3E6570, 0x8
.global sMareWB_BabyHatColorBuf1
sMareWB_BabyHatColorBuf1:
	.incbin "baserom.dol", 0x3E6578, 0x8
.global sMareWB_BabyBedColorBuf0
sMareWB_BabyBedColorBuf0:
	.incbin "baserom.dol", 0x3E6580, 0x8
.global sMareWB_BabyBedColorBuf1
sMareWB_BabyBedColorBuf1:
	.incbin "baserom.dol", 0x3E6588, 0x8
.global $$21614
$$21614:
	.incbin "baserom.dol", 0x3E6590, 0x8
.global $$21621
$$21621:
	.incbin "baserom.dol", 0x3E6598, 0x8

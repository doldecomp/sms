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
/* 80175440 00172380  80 0D 84 80 */	lwz r0, cNpcPartsNameRootJoint@sda21(r13)
/* 80175444 00172384  38 83 94 18 */	addi r4, r3, sMareMB_FishingRodData@l
/* 80175448 00172388  90 04 00 00 */	stw r0, 0(r4)
/* 8017544C 0017238C  3C 60 80 3C */	lis r3, sMareWB_BabyData@ha
/* 80175450 00172390  90 03 94 44 */	stw r0, sMareWB_BabyData@l(r3)
/* 80175454 00172394  4E 80 00 20 */	blr 

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
  .4byte __sinit_NpcInitData_cpp

.section .rodata, "wa"  # 0x8036FFA0 - 0x803A8380
$$21490:
	.incbin "baserom.dol", 0x37CC20, 0xC
$$21526:
	.incbin "baserom.dol", 0x37CC2C, 0x14
$$21530:
	.incbin "baserom.dol", 0x37CC40, 0x10
$$21535:
	.incbin "baserom.dol", 0x37CC50, 0x24
$$21536:
	.incbin "baserom.dol", 0x37CC74, 0x14
sMonteM_HatAColorBuf0:
	.incbin "baserom.dol", 0x37CC88, 0x38
sMonteM_HatAColorBuf1:
	.incbin "baserom.dol", 0x37CCC0, 0x38
$$21537:
	.incbin "baserom.dol", 0x37CCF8, 0xC
sMonteM_HatAColorChange:
	.incbin "baserom.dol", 0x37CD04, 0x10
sMonteM_HatABeltColorBuf:
	.incbin "baserom.dol", 0x37CD14, 0x38
$$21538:
	.incbin "baserom.dol", 0x37CD4C, 0xC
sMonteM_HatABeltColorChange:
	.incbin "baserom.dol", 0x37CD58, 0x10
$$21540:
	.incbin "baserom.dol", 0x37CD68, 0x10
sMonteM_HatAData:
	.incbin "baserom.dol", 0x37CD78, 0x2C
sMonteM_HatBColorBuf:
	.incbin "baserom.dol", 0x37CDA4, 0x18
sMonteM_HatBColorChange:
	.incbin "baserom.dol", 0x37CDBC, 0x10
sMonteM_HatBBeltColorBuf:
	.incbin "baserom.dol", 0x37CDCC, 0x18
sMonteM_HatBBeltColorChange:
	.incbin "baserom.dol", 0x37CDE4, 0x10
$$21541:
	.incbin "baserom.dol", 0x37CDF4, 0x10
sMonteM_HatBData:
	.incbin "baserom.dol", 0x37CE04, 0x2C
sMonteM_HatDColorBuf:
	.incbin "baserom.dol", 0x37CE30, 0x10
sMonteM_HatDColorChange:
	.incbin "baserom.dol", 0x37CE40, 0x10
$$21542:
	.incbin "baserom.dol", 0x37CE50, 0x10
sMonteM_HatDData:
	.incbin "baserom.dol", 0x37CE60, 0x2C
sMonteM_HatEColorBuf0:
	.incbin "baserom.dol", 0x37CE8C, 0x10
sMonteM_HatEColorBuf1:
	.incbin "baserom.dol", 0x37CE9C, 0x10
sMonteM_HatEColorChange:
	.incbin "baserom.dol", 0x37CEAC, 0x10
$$21543:
	.incbin "baserom.dol", 0x37CEBC, 0x10
sMonteM_HatEData:
	.incbin "baserom.dol", 0x37CECC, 0x2C
sMonteM_HatFColorBuf0:
	.incbin "baserom.dol", 0x37CEF8, 0x10
sMonteM_HatFColorBuf1:
	.incbin "baserom.dol", 0x37CF08, 0x10
sMonteM_HatFColorChange:
	.incbin "baserom.dol", 0x37CF18, 0x10
$$21544:
	.incbin "baserom.dol", 0x37CF28, 0x10
sMonteM_HatFData:
	.incbin "baserom.dol", 0x37CF38, 0x2C
sMonteM_HatGColorBuf0:
	.incbin "baserom.dol", 0x37CF64, 0x28
sMonteM_HatGColorBuf1:
	.incbin "baserom.dol", 0x37CF8C, 0x28
sMonteM_HatGColorChange:
	.incbin "baserom.dol", 0x37CFB4, 0x10
$$21545:
	.incbin "baserom.dol", 0x37CFC4, 0x10
sMonteM_HatGData:
	.incbin "baserom.dol", 0x37CFD4, 0x2C
sMonteM_MustacheAColorBuf0:
	.incbin "baserom.dol", 0x37D000, 0x4
.global lbl_80380004
lbl_80380004:
	.incbin "baserom.dol", 0x37D004, 0x1C
$$21546:
	.incbin "baserom.dol", 0x37D020, 0xC
sMonteM_MustacheAColorChange:
	.incbin "baserom.dol", 0x37D02C, 0x10
$$21547:
	.incbin "baserom.dol", 0x37D03C, 0x10
sMonteM_MustacheAData:
	.incbin "baserom.dol", 0x37D04C, 0x2C
$$21548:
	.incbin "baserom.dol", 0x37D078, 0x14
sMonteM_GlassesAData:
	.incbin "baserom.dol", 0x37D08C, 0x2C
sMonteM_GlassesBColorBuf0:
	.incbin "baserom.dol", 0x37D0B8, 0x18
$$21549:
	.incbin "baserom.dol", 0x37D0D0, 0xC
sMonteM_GlassesBColorChange:
	.incbin "baserom.dol", 0x37D0DC, 0x10
$$21550:
	.incbin "baserom.dol", 0x37D0EC, 0x14
sMonteM_GlassesBData:
	.incbin "baserom.dol", 0x37D100, 0x2C
sMonteM_LapelAColorBuf0:
	.incbin "baserom.dol", 0x37D12C, 0x10
sMonteM_LapelAColorBuf1:
	.incbin "baserom.dol", 0x37D13C, 0x10
$$21551:
	.incbin "baserom.dol", 0x37D14C, 0xC
sMonteM_LapelAColorChange:
	.incbin "baserom.dol", 0x37D158, 0x10
$$21552:
	.incbin "baserom.dol", 0x37D168, 0xC
$$21553:
	.incbin "baserom.dol", 0x37D174, 0x10
sMonteM_LapelAData:
	.incbin "baserom.dol", 0x37D184, 0x2C
$$21554:
	.incbin "baserom.dol", 0x37D1B0, 0xC
sMonteM_TieBColorChange:
	.incbin "baserom.dol", 0x37D1BC, 0x10
$$21555:
	.incbin "baserom.dol", 0x37D1CC, 0x10
sMonteM_TieBData:
	.incbin "baserom.dol", 0x37D1DC, 0x2C
$$21556:
	.incbin "baserom.dol", 0x37D208, 0x14
sMonteM_LuggageData:
	.incbin "baserom.dol", 0x37D21C, 0x2C
$$21557:
	.incbin "baserom.dol", 0x37D248, 0x10
sMonteMF_TubeData:
	.incbin "baserom.dol", 0x37D258, 0x2C
$$21558:
	.incbin "baserom.dol", 0x37D284, 0xC
$$21559:
	.incbin "baserom.dol", 0x37D290, 0x10
sMonteMG_MopData:
	.incbin "baserom.dol", 0x37D2A0, 0x2C
$$21560:
	.incbin "baserom.dol", 0x37D2CC, 0x14
sMonteMH_UkleleData:
	.incbin "baserom.dol", 0x37D2E0, 0x2C
sMonteW_FlowerInColorBuf:
	.incbin "baserom.dol", 0x37D30C, 0x18
$$21561:
	.incbin "baserom.dol", 0x37D324, 0xC
sMonteW_FlowerInColorChange:
	.incbin "baserom.dol", 0x37D330, 0x10
sMonteW_FlowerOutColorBuf:
	.incbin "baserom.dol", 0x37D340, 0x18
$$21562:
	.incbin "baserom.dol", 0x37D358, 0xC
sMonteW_FlowerOutColorChange:
	.incbin "baserom.dol", 0x37D364, 0x10
$$21563:
	.incbin "baserom.dol", 0x37D374, 0xC
$$21564:
	.incbin "baserom.dol", 0x37D380, 0x14
sMonteW_FlowerData:
	.incbin "baserom.dol", 0x37D394, 0x2C
sMonteW_HatAMainColorBuf0:
	.incbin "baserom.dol", 0x37D3C0, 0x20
sMonteW_HatAMainColorBuf1:
	.incbin "baserom.dol", 0x37D3E0, 0x20
sMonteW_HatAMainColorChange:
	.incbin "baserom.dol", 0x37D400, 0x10
sMonteW_HatABeltColorBuf:
	.incbin "baserom.dol", 0x37D410, 0x20
sMonteW_HatABeltColorChange:
	.incbin "baserom.dol", 0x37D430, 0x10
$$21565:
	.incbin "baserom.dol", 0x37D440, 0x10
sMonteW_HatAData:
	.incbin "baserom.dol", 0x37D450, 0x2C
sMonteW_GlassBColorBuf:
	.incbin "baserom.dol", 0x37D47C, 0x18
sMonteW_GlassBColorChange:
	.incbin "baserom.dol", 0x37D494, 0x10
$$21566:
	.incbin "baserom.dol", 0x37D4A4, 0x10
sMonteW_GlassBData:
	.incbin "baserom.dol", 0x37D4B4, 0x2C
$$21567:
	.incbin "baserom.dol", 0x37D4E0, 0x14
sMonteW_ArrowRData:
	.incbin "baserom.dol", 0x37D4F4, 0x2C
$$21568:
	.incbin "baserom.dol", 0x37D520, 0x14
sMonteW_ArrowLData:
	.incbin "baserom.dol", 0x37D534, 0x2C
$$21569:
	.incbin "baserom.dol", 0x37D560, 0x10
sMonteWC_HatCData:
	.incbin "baserom.dol", 0x37D570, 0x2C
$$21570:
	.incbin "baserom.dol", 0x37D59C, 0x14
sMonteWC_BraceletRData:
	.incbin "baserom.dol", 0x37D5B0, 0x2C
$$21571:
	.incbin "baserom.dol", 0x37D5DC, 0xC
$$21572:
	.incbin "baserom.dol", 0x37D5E8, 0x14
sMonteWC_BraceletLData:
	.incbin "baserom.dol", 0x37D5FC, 0x2C
sMareM_HatAColorBuf0:
	.incbin "baserom.dol", 0x37D628, 0x48
sMareM_HatAColorBuf1:
	.incbin "baserom.dol", 0x37D670, 0x48
sMareM_HatAColorChange:
	.incbin "baserom.dol", 0x37D6B8, 0x10
$$21574:
	.incbin "baserom.dol", 0x37D6C8, 0x10
sMareM_HatAData:
	.incbin "baserom.dol", 0x37D6D8, 0x2C
sMareM_HatBColorBuf0:
	.incbin "baserom.dol", 0x37D704, 0x30
sMareM_HatBColorBuf1:
	.incbin "baserom.dol", 0x37D734, 0x30
sMareM_HatBColorChange:
	.incbin "baserom.dol", 0x37D764, 0x10
$$21575:
	.incbin "baserom.dol", 0x37D774, 0x10
sMareM_HatBData:
	.incbin "baserom.dol", 0x37D784, 0x2C
sMareM_HatEColorBuf0:
	.incbin "baserom.dol", 0x37D7B0, 0x58
sMareM_HatEColorBuf1:
	.incbin "baserom.dol", 0x37D808, 0x58
sMareM_HatEColorChange:
	.incbin "baserom.dol", 0x37D860, 0x10
$$21576:
	.incbin "baserom.dol", 0x37D870, 0x10
sMareM_HatEData:
	.incbin "baserom.dol", 0x37D880, 0x2C
sMareM_ShellAColorBuf0:
	.incbin "baserom.dol", 0x37D8AC, 0x50
sMareM_ShellAColorBuf1:
	.incbin "baserom.dol", 0x37D8FC, 0x50
sMareM_ShellAColorChange:
	.incbin "baserom.dol", 0x37D94C, 0x10
$$21578:
	.incbin "baserom.dol", 0x37D95C, 0x14
sMareM_ShellAData:
	.incbin "baserom.dol", 0x37D970, 0x2C
sMareM_ShellBColorBuf0:
	.incbin "baserom.dol", 0x37D99C, 0x50
sMareM_ShellBColorBuf1:
	.incbin "baserom.dol", 0x37D9EC, 0x50
sMareM_ShellBColorChange:
	.incbin "baserom.dol", 0x37DA3C, 0x10
$$21579:
	.incbin "baserom.dol", 0x37DA4C, 0x14
sMareM_ShellBData:
	.incbin "baserom.dol", 0x37DA60, 0x2C
sMareM_BivalveBColorBuf0:
	.incbin "baserom.dol", 0x37DA8C, 0x30
sMareM_BivalveBColorBuf1:
	.incbin "baserom.dol", 0x37DABC, 0x30
sMareM_BivalveBColorChange:
	.incbin "baserom.dol", 0x37DAEC, 0x10
$$21580:
	.incbin "baserom.dol", 0x37DAFC, 0x14
sMareM_BivalveBData:
	.incbin "baserom.dol", 0x37DB10, 0x2C
sMareM_GlassColorBuf0:
	.incbin "baserom.dol", 0x37DB3C, 0x18
sMareM_GlassColorBuf1:
	.incbin "baserom.dol", 0x37DB54, 0x18
$$21581:
	.incbin "baserom.dol", 0x37DB6C, 0xC
sMareM_GlassColorChange:
	.incbin "baserom.dol", 0x37DB78, 0x10
$$21582:
	.incbin "baserom.dol", 0x37DB88, 0x10
sMareM_GlassData:
	.incbin "baserom.dol", 0x37DB98, 0x2C
sMareMB_ElderMat1ColorChange:
	.incbin "baserom.dol", 0x37DBC4, 0x10
sMareMB_ElderMat2ColorChange:
	.incbin "baserom.dol", 0x37DBD4, 0x10
sMareMB_ElderMat3ColorChange:
	.incbin "baserom.dol", 0x37DBE4, 0x10
$$21585:
	.incbin "baserom.dol", 0x37DBF4, 0x10
sMareMB_ElderData:
	.incbin "baserom.dol", 0x37DC04, 0x2C
sMareMB_FishingRodMat1ColorChange:
	.incbin "baserom.dol", 0x37DC30, 0x10
sMareMB_FishingRodMat2ColorChange:
	.incbin "baserom.dol", 0x37DC40, 0x10
sMareMB_FishingRodMat3ColorChange:
	.incbin "baserom.dol", 0x37DC50, 0x10
$$21586:
	.incbin "baserom.dol", 0x37DC60, 0x14
sMareMC_HatFColorChange:
	.incbin "baserom.dol", 0x37DC74, 0x10
$$21587:
	.incbin "baserom.dol", 0x37DC84, 0x10
sMareMC_HatFData:
	.incbin "baserom.dol", 0x37DC94, 0x2C
sMareMC_BeardColorChange:
	.incbin "baserom.dol", 0x37DCC0, 0x10
$$21588:
	.incbin "baserom.dol", 0x37DCD0, 0x14
sMareMC_BeardData:
	.incbin "baserom.dol", 0x37DCE4, 0x2C
sMareMC_MustacheColorChange:
	.incbin "baserom.dol", 0x37DD10, 0x10
$$21589:
	.incbin "baserom.dol", 0x37DD20, 0x14
sMareMC_MustacheData:
	.incbin "baserom.dol", 0x37DD34, 0x2C
sMareMD_PipeColorChange:
	.incbin "baserom.dol", 0x37DD60, 0x10
$$21591:
	.incbin "baserom.dol", 0x37DD70, 0x14
sMareMD_PipeData:
	.incbin "baserom.dol", 0x37DD84, 0x2C
sMareW_PearlAColorBuf0:
	.incbin "baserom.dol", 0x37DDB0, 0x30
sMareW_PearlAColorBuf1:
	.incbin "baserom.dol", 0x37DDE0, 0x30
sMareW_PearlAColorChange:
	.incbin "baserom.dol", 0x37DE10, 0x10
$$21592:
	.incbin "baserom.dol", 0x37DE20, 0x14
sMareW_PearlAData:
	.incbin "baserom.dol", 0x37DE34, 0x2C
sMareW_HatAColorBuf0:
	.incbin "baserom.dol", 0x37DE60, 0x30
sMareW_HatAColorBuf1:
	.incbin "baserom.dol", 0x37DE90, 0x30
sMareW_HatAColorChange:
	.incbin "baserom.dol", 0x37DEC0, 0x10
$$21593:
	.incbin "baserom.dol", 0x37DED0, 0x10
sMareW_HatAData:
	.incbin "baserom.dol", 0x37DEE0, 0x2C
sMareW_HatDColorBuf0:
	.incbin "baserom.dol", 0x37DF0C, 0x58
sMareW_HatDColorBuf1:
	.incbin "baserom.dol", 0x37DF64, 0x58
sMareW_HatDColorChange:
	.incbin "baserom.dol", 0x37DFBC, 0x10
$$21594:
	.incbin "baserom.dol", 0x37DFCC, 0x10
sMareW_HatDData:
	.incbin "baserom.dol", 0x37DFDC, 0x2C
sMareW_ShellAColorBuf0:
	.incbin "baserom.dol", 0x37E008, 0x30
sMareW_ShellAColorBuf1:
	.incbin "baserom.dol", 0x37E038, 0x30
sMareW_ShellAColorChange:
	.incbin "baserom.dol", 0x37E068, 0x10
$$21595:
	.incbin "baserom.dol", 0x37E078, 0x10
sMareW_ShellAData:
	.incbin "baserom.dol", 0x37E088, 0x2C
sMareW_ShellBColorBuf0:
	.incbin "baserom.dol", 0x37E0B4, 0x48
sMareW_ShellBColorBuf1:
	.incbin "baserom.dol", 0x37E0FC, 0x48
sMareW_ShellBColorChange:
	.incbin "baserom.dol", 0x37E144, 0x10
$$21596:
	.incbin "baserom.dol", 0x37E154, 0x10
sMareW_ShellBData:
	.incbin "baserom.dol", 0x37E164, 0x2C
sMareW_BivalveAColorBuf0:
	.incbin "baserom.dol", 0x37E190, 0x30
sMareW_BivalveAColorBuf1:
	.incbin "baserom.dol", 0x37E1C0, 0x30
sMareW_BivalveAColorChange:
	.incbin "baserom.dol", 0x37E1F0, 0x10
$$21597:
	.incbin "baserom.dol", 0x37E200, 0x14
sMareW_BivalveAData:
	.incbin "baserom.dol", 0x37E214, 0x2C
sMareWB_BabyBodyColorChange:
	.incbin "baserom.dol", 0x37E240, 0x10
$$21599:
	.incbin "baserom.dol", 0x37E250, 0xC
sMareWB_BabyHatColorChange:
	.incbin "baserom.dol", 0x37E25C, 0x10
$$21600:
	.incbin "baserom.dol", 0x37E26C, 0xC
sMareWB_BabyBedColorChange:
	.incbin "baserom.dol", 0x37E278, 0x10
$$21601:
	.incbin "baserom.dol", 0x37E288, 0x10
$$21602:
	.incbin "baserom.dol", 0x37E298, 0x18
sKinopio_SunGlassesData:
	.incbin "baserom.dol", 0x37E2B0, 0x2C
$$21603:
	.incbin "baserom.dol", 0x37E2DC, 0x10
$$21604:
	.incbin "baserom.dol", 0x37E2EC, 0x14
sKinojii_StickData:
	.incbin "baserom.dol", 0x37E300, 0x2C
$$21605:
	.incbin "baserom.dol", 0x37E32C, 0x18
sPeach_HairNormalData:
	.incbin "baserom.dol", 0x37E344, 0x2C
$$21606:
	.incbin "baserom.dol", 0x37E370, 0xC
$$21607:
	.incbin "baserom.dol", 0x37E37C, 0x14
sPeach_Hand1LData:
	.incbin "baserom.dol", 0x37E390, 0x2C
$$21608:
	.incbin "baserom.dol", 0x37E3BC, 0xC
$$21609:
	.incbin "baserom.dol", 0x37E3C8, 0x14
sPeach_Hand1RData:
	.incbin "baserom.dol", 0x37E3DC, 0x2C
$$21610:
	.incbin "baserom.dol", 0x37E408, 0x18
sPeach_HairPonytailData:
	.incbin "baserom.dol", 0x37E420, 0x2C
$$21611:
	.incbin "baserom.dol", 0x37E44C, 0x14
sPeach_ParasolData:
	.incbin "baserom.dol", 0x37E460, 0x2C
$$21612:
	.incbin "baserom.dol", 0x37E48C, 0x14
sPeach_Hand2LData:
	.incbin "baserom.dol", 0x37E4A0, 0x2C
$$21613:
	.incbin "baserom.dol", 0x37E4CC, 0x14
sPeach_Hand2RData:
	.incbin "baserom.dol", 0x37E4E0, 0x2C
sRaccoonDog_TubeColorBuf0:
	.incbin "baserom.dol", 0x37E50C, 0x10
sRaccoonDog_TubeColorBuf1:
	.incbin "baserom.dol", 0x37E51C, 0x10
sRaccoonDog_TubeColorChange:
	.incbin "baserom.dol", 0x37E52C, 0x10
$$21615:
	.incbin "baserom.dol", 0x37E53C, 0xC
$$21616:
	.incbin "baserom.dol", 0x37E548, 0xC
sRaccoonDog_TubeData:
	.incbin "baserom.dol", 0x37E554, 0x2C
sMonteM_BodyColorBuf:
	.incbin "baserom.dol", 0x37E580, 0x50
$$21617:
	.incbin "baserom.dol", 0x37E5D0, 0xC
sMonteM_BodyColor:
	.incbin "baserom.dol", 0x37E5DC, 0x10
sMonteMA_ClothColorBuf0:
	.incbin "baserom.dol", 0x37E5EC, 0x58
sMonteMA_ClothColorBuf1:
	.incbin "baserom.dol", 0x37E644, 0x58
$$21618:
	.incbin "baserom.dol", 0x37E69C, 0xC
sMonteMA_ClothColor:
	.incbin "baserom.dol", 0x37E6A8, 0x10
sMonteMB_BodyColorBuf:
	.incbin "baserom.dol", 0x37E6B8, 0x20
sMonteMB_BodyColor:
	.incbin "baserom.dol", 0x37E6D8, 0x10
sMonteMB_ClothColorBuf:
	.incbin "baserom.dol", 0x37E6E8, 0x30
sMonteMB_ClothColor:
	.incbin "baserom.dol", 0x37E718, 0x10
sMonteMC_ClothColorBuf0:
	.incbin "baserom.dol", 0x37E728, 0x58
sMonteMC_ClothColorBuf1:
	.incbin "baserom.dol", 0x37E780, 0x58
sMonteMC_ClothColor:
	.incbin "baserom.dol", 0x37E7D8, 0x10
sMonteMD_ClothColorBuf:
	.incbin "baserom.dol", 0x37E7E8, 0x28
sMonteMD_ClothColor:
	.incbin "baserom.dol", 0x37E810, 0x10
sMonteW_ColorBuf:
	.incbin "baserom.dol", 0x37E820, 0x30
sMonteW_BodyColor:
	.incbin "baserom.dol", 0x37E850, 0x10
sMonteWA_ClothColorBuf:
	.incbin "baserom.dol", 0x37E860, 0x30
sMonteWA_ClothColor:
	.incbin "baserom.dol", 0x37E890, 0x10
sMonteWB_ClothColorBuf0:
	.incbin "baserom.dol", 0x37E8A0, 0x48
sMonteWB_ClothColorBuf1:
	.incbin "baserom.dol", 0x37E8E8, 0x48
sMonteWB_ClothColor:
	.incbin "baserom.dol", 0x37E930, 0x10
sMareM_ColorBuf:
	.incbin "baserom.dol", 0x37E940, 0x30
sMareM_BodyColor:
	.incbin "baserom.dol", 0x37E970, 0x10
sMareW_ColorBuf:
	.incbin "baserom.dol", 0x37E980, 0x30
sMareW_BodyColor:
	.incbin "baserom.dol", 0x37E9B0, 0x10
sKinopio_BodyColorData0:
	.incbin "baserom.dol", 0x37E9C0, 0x28
sKinopio_BodyColorData1:
	.incbin "baserom.dol", 0x37E9E8, 0x28
$$21619:
	.incbin "baserom.dol", 0x37EA10, 0xC
sKinopio_BodyColor:
	.incbin "baserom.dol", 0x37EA1C, 0x10
sKinopio_ClothColorData0:
	.incbin "baserom.dol", 0x37EA2C, 0x28
sKinopio_ClothColorData1:
	.incbin "baserom.dol", 0x37EA54, 0x28
$$21620:
	.incbin "baserom.dol", 0x37EA7C, 0xC
sKinopio_ClothColor:
	.incbin "baserom.dol", 0x37EA88, 0x10
sRaccoonDog_ClothColorBuf0:
	.incbin "baserom.dol", 0x37EA98, 0x10
sRaccoonDog_ClothColorBuf1:
	.incbin "baserom.dol", 0x37EAA8, 0x10
sRaccoonDog_ClothColor:
	.incbin "baserom.dol", 0x37EAB8, 0x10
sMonteM_InitData:
	.incbin "baserom.dol", 0x37EAC8, 0x54
sMonteMA_InitData:
	.incbin "baserom.dol", 0x37EB1C, 0x54
sMonteMB_InitData:
	.incbin "baserom.dol", 0x37EB70, 0x54
sMonteMC_InitData:
	.incbin "baserom.dol", 0x37EBC4, 0x54
sMonteMD_InitData:
	.incbin "baserom.dol", 0x37EC18, 0x54
sMonteME_InitData:
	.incbin "baserom.dol", 0x37EC6C, 0x54
sMonteMF_InitData:
	.incbin "baserom.dol", 0x37ECC0, 0x54
sMonteMG_InitData:
	.incbin "baserom.dol", 0x37ED14, 0x54
sMonteMH_InitData:
	.incbin "baserom.dol", 0x37ED68, 0x54
sMonteW_InitData:
	.incbin "baserom.dol", 0x37EDBC, 0x54
sMonteWA_InitData:
	.incbin "baserom.dol", 0x37EE10, 0x54
sMonteWB_InitData:
	.incbin "baserom.dol", 0x37EE64, 0x54
sMonteWC_InitData:
	.incbin "baserom.dol", 0x37EEB8, 0x54
sMareM_InitData:
	.incbin "baserom.dol", 0x37EF0C, 0x54
sMareMB_InitData:
	.incbin "baserom.dol", 0x37EF60, 0x54
sMareMC_InitData:
	.incbin "baserom.dol", 0x37EFB4, 0x54
sMareMD_InitData:
	.incbin "baserom.dol", 0x37F008, 0x54
sMareW_InitData:
	.incbin "baserom.dol", 0x37F05C, 0x54
sMareWB_InitData:
	.incbin "baserom.dol", 0x37F0B0, 0x54
sKinopio_InitData:
	.incbin "baserom.dol", 0x37F104, 0x54
sKinojii_InitData:
	.incbin "baserom.dol", 0x37F158, 0x54
sPeach_InitData:
	.incbin "baserom.dol", 0x37F1AC, 0x54
sRaccoonDog_InitData:
	.incbin "baserom.dol", 0x37F200, 0x54
sSunflowerL_InitData:
	.incbin "baserom.dol", 0x37F254, 0x54
sSunflowerS_InitData:
	.incbin "baserom.dol", 0x37F2A8, 0x54
sBoardNpc_InitData:
	.incbin "baserom.dol", 0x37F2FC, 0x54

.section .data, "wa"  # 0x803A8380 - 0x803E6000
$$21431:
	.incbin "baserom.dol", 0x3B63F0, 0xC
$$21411:
	.incbin "baserom.dol", 0x3B63FC, 0xC
$$21210:
	.incbin "baserom.dol", 0x3B6408, 0x10
sMareMB_FishingRodData:
	.incbin "baserom.dol", 0x3B6418, 0x2C
sMareWB_BabyData:
	.incbin "baserom.dol", 0x3B6444, 0x2C
sAllNpcInitData$1622:
	.incbin "baserom.dol", 0x3B6470, 0x78

.section .sdata2, "wa"  # 0x8040B460 - 0x80414020
sPeach_TakenMiddle:
	.incbin "baserom.dol", 0x3E6478, 0x8
$$21539:
	.incbin "baserom.dol", 0x3E6480, 0x8
sMonteM_TieBColorBuf0:
	.incbin "baserom.dol", 0x3E6488, 0x8
sMonteM_TieBColorBuf1:
	.incbin "baserom.dol", 0x3E6490, 0x8
$$21573:
	.incbin "baserom.dol", 0x3E6498, 0x8
$$21577:
	.incbin "baserom.dol", 0x3E64A0, 0x8
sMareMB_ElderMat1ColorBuf0:
	.incbin "baserom.dol", 0x3E64A8, 0x8
sMareMB_ElderMat1ColorBuf1:
	.incbin "baserom.dol", 0x3E64B0, 0x8
sMareMB_ElderMat2ColorBuf0:
	.incbin "baserom.dol", 0x3E64B8, 0x8
sMareMB_ElderMat2ColorBuf1:
	.incbin "baserom.dol", 0x3E64C0, 0x8
$$21583:
	.incbin "baserom.dol", 0x3E64C8, 0x8
sMareMB_ElderMat3ColorBuf0:
	.incbin "baserom.dol", 0x3E64D0, 0x8
sMareMB_ElderMat3ColorBuf1:
	.incbin "baserom.dol", 0x3E64D8, 0x8
$$21584:
	.incbin "baserom.dol", 0x3E64E0, 0x8
sMareMB_FishingRodMat1ColorBuf0:
	.incbin "baserom.dol", 0x3E64E8, 0x8
sMareMB_FishingRodMat1ColorBuf1:
	.incbin "baserom.dol", 0x3E64F0, 0x8
sMareMB_FishingRodMat2ColorBuf0:
	.incbin "baserom.dol", 0x3E64F8, 0x8
sMareMB_FishingRodMat2ColorBuf1:
	.incbin "baserom.dol", 0x3E6500, 0x8
sMareMB_FishingRodMat3ColorBuf0:
	.incbin "baserom.dol", 0x3E6508, 0x8
sMareMB_FishingRodMat3ColorBuf1:
	.incbin "baserom.dol", 0x3E6510, 0x8
sMareMC_HatFColorBuf0:
	.incbin "baserom.dol", 0x3E6518, 0x8
sMareMC_HatFColorBuf1:
	.incbin "baserom.dol", 0x3E6520, 0x8
sMareMC_BeardColorBuf0:
	.incbin "baserom.dol", 0x3E6528, 0x8
sMareMC_BeardColorBuf1:
	.incbin "baserom.dol", 0x3E6530, 0x8
sMareMC_MustacheColorBuf0:
	.incbin "baserom.dol", 0x3E6538, 0x8
sMareMC_MustacheColorBuf1:
	.incbin "baserom.dol", 0x3E6540, 0x8
sMareMD_PipeColorBuf0:
	.incbin "baserom.dol", 0x3E6548, 0x8
sMareMD_PipeColorBuf1:
	.incbin "baserom.dol", 0x3E6550, 0x8
$$21590:
	.incbin "baserom.dol", 0x3E6558, 0x8
sMareWB_BabyBodyColorBuf0:
	.incbin "baserom.dol", 0x3E6560, 0x8
$$21598:
	.incbin "baserom.dol", 0x3E6568, 0x8
sMareWB_BabyHatColorBuf0:
	.incbin "baserom.dol", 0x3E6570, 0x8
sMareWB_BabyHatColorBuf1:
	.incbin "baserom.dol", 0x3E6578, 0x8
sMareWB_BabyBedColorBuf0:
	.incbin "baserom.dol", 0x3E6580, 0x8
sMareWB_BabyBedColorBuf1:
	.incbin "baserom.dol", 0x3E6588, 0x8
$$21614:
	.incbin "baserom.dol", 0x3E6590, 0x8
$$21621:
	.incbin "baserom.dol", 0x3E6598, 0x8

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
lbl_constructor:
  .4byte __sinit_NpcInitData_cpp

.section .rodata, "a"  # 0x8036FFA0 - 0x803A8380
.balign 8
"@1490":
  .4byte 0
  .4byte 0
  .4byte 0
"@1526":
	.incbin "baserom.dol", 0x37CC2C, 0x14
"@1530":
	.incbin "baserom.dol", 0x37CC40, 0x10
"@1535":
	.incbin "baserom.dol", 0x37CC50, 0x24
"@1536":
	.incbin "baserom.dol", 0x37CC74, 0x14
sMonteM_HatAColorBuf0:
	.incbin "baserom.dol", 0x37CC88, 0x38
sMonteM_HatAColorBuf1:
	.incbin "baserom.dol", 0x37CCC0, 0x38
"@1537":
	.incbin "baserom.dol", 0x37CCF8, 0xC
sMonteM_HatAColorChange:
  .4byte 0x2
  .4byte "@1537"
  .4byte sMonteM_HatAColorBuf0
  .4byte sMonteM_HatAColorBuf1
sMonteM_HatABeltColorBuf:
	.incbin "baserom.dol", 0x37CD14, 0x38
"@1538":
	.incbin "baserom.dol", 0x37CD4C, 0xC
sMonteM_HatABeltColorChange:
  .4byte 0x1
  .4byte "@1538"
  .4byte sMonteM_HatABeltColorBuf
  .4byte 0
"@1540":
	.incbin "baserom.dol", 0x37CD68, 0x10
sMonteM_HatAData:
  .4byte "@1539"
  .4byte 0
  .4byte "@1540"
  .4byte 0
  .4byte sMonteM_HatAColorChange
  .4byte 0
  .4byte sMonteM_HatABeltColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
sMonteM_HatBColorBuf:
	.incbin "baserom.dol", 0x37CDA4, 0x18
sMonteM_HatBColorChange:
  .4byte 0
  .4byte "@1537"
  .4byte sMonteM_HatBColorBuf
  .4byte 0
sMonteM_HatBBeltColorBuf:
	.incbin "baserom.dol", 0x37CDCC, 0x18
sMonteM_HatBBeltColorChange:
  .4byte 0x1
  .4byte "@1538"
  .4byte sMonteM_HatBBeltColorBuf
  .4byte 0
"@1541":
	.incbin "baserom.dol", 0x37CDF4, 0x10
sMonteM_HatBData:
  .4byte "@1539"
  .4byte 0
  .4byte "@1541"
  .4byte 0
  .4byte sMonteM_HatBColorChange
  .4byte 0
  .4byte sMonteM_HatBBeltColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
sMonteM_HatDColorBuf:
	.incbin "baserom.dol", 0x37CE30, 0x10
sMonteM_HatDColorChange:
  .4byte 0x1
  .4byte "@1538"
  .4byte sMonteM_HatDColorBuf
  .4byte 0
"@1542":
	.incbin "baserom.dol", 0x37CE50, 0x10
sMonteM_HatDData:
  .4byte "@1539"
  .4byte 0
  .4byte "@1542"
  .4byte 0
  .4byte sMonteM_HatDColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
sMonteM_HatEColorBuf0:
	.incbin "baserom.dol", 0x37CE8C, 0x10
sMonteM_HatEColorBuf1:
	.incbin "baserom.dol", 0x37CE9C, 0x10
sMonteM_HatEColorChange:
  .4byte 0x2
  .4byte "@1537"
  .4byte sMonteM_HatEColorBuf0
  .4byte sMonteM_HatEColorBuf1
"@1543":
	.incbin "baserom.dol", 0x37CEBC, 0x10
sMonteM_HatEData:
  .4byte "@1539"
  .4byte 0
  .4byte "@1543"
  .4byte 0
  .4byte sMonteM_HatEColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
sMonteM_HatFColorBuf0:
	.incbin "baserom.dol", 0x37CEF8, 0x10
sMonteM_HatFColorBuf1:
	.incbin "baserom.dol", 0x37CF08, 0x10
sMonteM_HatFColorChange:
  .4byte 0x2
  .4byte "@1537"
  .4byte sMonteM_HatFColorBuf0
  .4byte sMonteM_HatFColorBuf1
"@1544":
	.incbin "baserom.dol", 0x37CF28, 0x10
sMonteM_HatFData:
  .4byte "@1539"
  .4byte 0
  .4byte "@1544"
  .4byte 0
  .4byte sMonteM_HatFColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
sMonteM_HatGColorBuf0:
	.incbin "baserom.dol", 0x37CF64, 0x28
sMonteM_HatGColorBuf1:
	.incbin "baserom.dol", 0x37CF8C, 0x28
sMonteM_HatGColorChange:
  .4byte 0x2
  .4byte "@1537"
  .4byte sMonteM_HatGColorBuf0
  .4byte sMonteM_HatGColorBuf1
"@1545":
	.incbin "baserom.dol", 0x37CFC4, 0x10
sMonteM_HatGData:
  .4byte "@1539"
  .4byte 0
  .4byte "@1545"
  .4byte 0
  .4byte sMonteM_HatGColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
sMonteM_MustacheAColorBuf0:
  .4byte 0
.global lbl_80380004
lbl_80380004:
	.incbin "baserom.dol", 0x37D004, 0x1C
"@1546":
	.incbin "baserom.dol", 0x37D020, 0xC
sMonteM_MustacheAColorChange:
  .4byte 0x1
  .4byte "@1546"
  .4byte sMonteM_MustacheAColorBuf0
  .4byte 0
"@1547":
	.incbin "baserom.dol", 0x37D03C, 0x10
sMonteM_MustacheAData:
  .4byte "@1539"
  .4byte 0
  .4byte "@1547"
  .4byte 0
  .4byte sMonteM_MustacheAColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x10000
"@1548":
	.incbin "baserom.dol", 0x37D078, 0x14
sMonteM_GlassesAData:
  .4byte "@1539"
  .4byte 0
  .4byte "@1548"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x20000
sMonteM_GlassesBColorBuf0:
	.incbin "baserom.dol", 0x37D0B8, 0x18
"@1549":
	.incbin "baserom.dol", 0x37D0D0, 0xC
sMonteM_GlassesBColorChange:
  .4byte 0
  .4byte "@1549"
  .4byte sMonteM_GlassesBColorBuf0
  .4byte 0
"@1550":
	.incbin "baserom.dol", 0x37D0EC, 0x14
sMonteM_GlassesBData:
  .4byte "@1539"
  .4byte 0
  .4byte "@1550"
  .4byte 0
  .4byte sMonteM_GlassesBColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x20000
sMonteM_LapelAColorBuf0:
	.incbin "baserom.dol", 0x37D12C, 0x10
sMonteM_LapelAColorBuf1:
	.incbin "baserom.dol", 0x37D13C, 0x10
"@1551":
	.incbin "baserom.dol", 0x37D14C, 0xC
sMonteM_LapelAColorChange:
  .4byte 0x2
  .4byte "@1551"
  .4byte sMonteM_LapelAColorBuf0
  .4byte sMonteM_LapelAColorBuf1
"@1552":
	.incbin "baserom.dol", 0x37D168, 0xC
"@1553":
	.incbin "baserom.dol", 0x37D174, 0x10
sMonteM_LapelAData:
  .4byte "@1552"
  .4byte 0
  .4byte "@1553"
  .4byte 0
  .4byte sMonteM_LapelAColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x10000
"@1554":
	.incbin "baserom.dol", 0x37D1B0, 0xC
sMonteM_TieBColorChange:
  .4byte 0x2
  .4byte "@1554"
  .4byte sMonteM_TieBColorBuf0
  .4byte sMonteM_TieBColorBuf1
"@1555":
	.incbin "baserom.dol", 0x37D1CC, 0x10
sMonteM_TieBData:
  .4byte "@1552"
  .4byte 0
  .4byte "@1555"
  .4byte 0
  .4byte sMonteM_TieBColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
"@1556":
	.incbin "baserom.dol", 0x37D208, 0x14
sMonteM_LuggageData:
  .4byte "@1552"
  .4byte 0
  .4byte "@1556"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
"@1557":
	.incbin "baserom.dol", 0x37D248, 0x10
sMonteMF_TubeData:
  .4byte "@1552"
  .4byte 0
  .4byte "@1557"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
"@1558":
	.incbin "baserom.dol", 0x37D284, 0xC
"@1559":
	.incbin "baserom.dol", 0x37D290, 0x10
sMonteMG_MopData:
  .4byte "@1558"
  .4byte 0
  .4byte "@1559"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
"@1560":
	.incbin "baserom.dol", 0x37D2CC, 0x14
sMonteMH_UkleleData:
  .4byte "@1552"
  .4byte 0
  .4byte "@1560"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
sMonteW_FlowerInColorBuf:
	.incbin "baserom.dol", 0x37D30C, 0x18
"@1561":
	.incbin "baserom.dol", 0x37D324, 0xC
sMonteW_FlowerInColorChange:
  .4byte 0x1
  .4byte "@1561"
  .4byte sMonteW_FlowerInColorBuf
  .4byte 0
sMonteW_FlowerOutColorBuf:
	.incbin "baserom.dol", 0x37D340, 0x18
"@1562":
	.incbin "baserom.dol", 0x37D358, 0xC
sMonteW_FlowerOutColorChange:
  .4byte 0
  .4byte "@1562"
  .4byte sMonteW_FlowerOutColorBuf
  .4byte 0
"@1563":
	.incbin "baserom.dol", 0x37D374, 0xC
"@1564":
	.incbin "baserom.dol", 0x37D380, 0x14
sMonteW_FlowerData:
  .4byte "@1563"
  .4byte 0
  .4byte "@1564"
  .4byte 0
  .4byte sMonteW_FlowerInColorChange
  .4byte 0
  .4byte sMonteW_FlowerOutColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x10000
sMonteW_HatAMainColorBuf0:
	.incbin "baserom.dol", 0x37D3C0, 0x20
sMonteW_HatAMainColorBuf1:
	.incbin "baserom.dol", 0x37D3E0, 0x20
sMonteW_HatAMainColorChange:
  .4byte 0x2
  .4byte "@1537"
  .4byte sMonteW_HatAMainColorBuf0
  .4byte sMonteW_HatAMainColorBuf1
sMonteW_HatABeltColorBuf:
	.incbin "baserom.dol", 0x37D410, 0x20
sMonteW_HatABeltColorChange:
  .4byte 0x1
  .4byte "@1538"
  .4byte sMonteW_HatABeltColorBuf
  .4byte 0
"@1565":
	.incbin "baserom.dol", 0x37D440, 0x10
sMonteW_HatAData:
  .4byte "@1539"
  .4byte 0
  .4byte "@1565"
  .4byte 0
  .4byte sMonteW_HatAMainColorChange
  .4byte 0
  .4byte sMonteW_HatABeltColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
sMonteW_GlassBColorBuf:
	.incbin "baserom.dol", 0x37D47C, 0x18
sMonteW_GlassBColorChange:
  .4byte 0
  .4byte "@1549"
  .4byte sMonteW_GlassBColorBuf
  .4byte 0
"@1566":
	.incbin "baserom.dol", 0x37D4A4, 0x10
sMonteW_GlassBData:
  .4byte "@1539"
  .4byte 0
  .4byte "@1566"
  .4byte 0
  .4byte sMonteW_GlassBColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x20000
"@1567":
	.incbin "baserom.dol", 0x37D4E0, 0x14
sMonteW_ArrowRData:
  .4byte "@1558"
  .4byte 0
  .4byte "@1567"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
"@1568":
	.incbin "baserom.dol", 0x37D520, 0x14
sMonteW_ArrowLData:
  .4byte "@1558"
  .4byte 0
  .4byte "@1568"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
"@1569":
	.incbin "baserom.dol", 0x37D560, 0x10
sMonteWC_HatCData:
  .4byte "@1539"
  .4byte 0
  .4byte "@1569"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
"@1570":
	.incbin "baserom.dol", 0x37D59C, 0x14
sMonteWC_BraceletRData:
  .4byte "@1558"
  .4byte 0
  .4byte "@1570"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
"@1571":
	.incbin "baserom.dol", 0x37D5DC, 0xC
"@1572":
	.incbin "baserom.dol", 0x37D5E8, 0x14
sMonteWC_BraceletLData:
  .4byte "@1571"
  .4byte 0
  .4byte "@1572"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
sMareM_HatAColorBuf0:
	.incbin "baserom.dol", 0x37D628, 0x48
sMareM_HatAColorBuf1:
	.incbin "baserom.dol", 0x37D670, 0x48
sMareM_HatAColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareM_HatAColorBuf0
  .4byte sMareM_HatAColorBuf1
"@1574":
	.incbin "baserom.dol", 0x37D6C8, 0x10
sMareM_HatAData:
  .4byte "@1539"
  .4byte 0
  .4byte "@1574"
  .4byte 0
  .4byte sMareM_HatAColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x101
sMareM_HatBColorBuf0:
	.incbin "baserom.dol", 0x37D704, 0x30
sMareM_HatBColorBuf1:
	.incbin "baserom.dol", 0x37D734, 0x30
sMareM_HatBColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareM_HatBColorBuf0
  .4byte sMareM_HatBColorBuf1
"@1575":
	.incbin "baserom.dol", 0x37D774, 0x10
sMareM_HatBData:
  .4byte "@1539"
  .4byte 0
  .4byte "@1575"
  .4byte 0
  .4byte sMareM_HatBColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x1
sMareM_HatEColorBuf0:
	.incbin "baserom.dol", 0x37D7B0, 0x58
sMareM_HatEColorBuf1:
	.incbin "baserom.dol", 0x37D808, 0x58
sMareM_HatEColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareM_HatEColorBuf0
  .4byte sMareM_HatEColorBuf1
"@1576":
	.incbin "baserom.dol", 0x37D870, 0x10
sMareM_HatEData:
  .4byte "@1539"
  .4byte 0
  .4byte "@1576"
  .4byte 0
  .4byte sMareM_HatEColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x1
sMareM_ShellAColorBuf0:
	.incbin "baserom.dol", 0x37D8AC, 0x50
sMareM_ShellAColorBuf1:
	.incbin "baserom.dol", 0x37D8FC, 0x50
sMareM_ShellAColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareM_ShellAColorBuf0
  .4byte sMareM_ShellAColorBuf1
"@1578":
	.incbin "baserom.dol", 0x37D95C, 0x14
sMareM_ShellAData:
  .4byte "@1577"
  .4byte 0
  .4byte "@1578"
  .4byte 0
  .4byte sMareM_ShellAColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x10101
sMareM_ShellBColorBuf0:
	.incbin "baserom.dol", 0x37D99C, 0x50
sMareM_ShellBColorBuf1:
	.incbin "baserom.dol", 0x37D9EC, 0x50
sMareM_ShellBColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareM_ShellBColorBuf0
  .4byte sMareM_ShellBColorBuf1
"@1579":
	.incbin "baserom.dol", 0x37DA4C, 0x14
sMareM_ShellBData:
  .4byte "@1577"
  .4byte 0
  .4byte "@1579"
  .4byte 0
  .4byte sMareM_ShellBColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x10001
sMareM_BivalveBColorBuf0:
	.incbin "baserom.dol", 0x37DA8C, 0x30
sMareM_BivalveBColorBuf1:
	.incbin "baserom.dol", 0x37DABC, 0x30
sMareM_BivalveBColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareM_BivalveBColorBuf0
  .4byte sMareM_BivalveBColorBuf1
"@1580":
	.incbin "baserom.dol", 0x37DAFC, 0x14
sMareM_BivalveBData:
  .4byte "@1577"
  .4byte 0
  .4byte "@1580"
  .4byte 0
  .4byte sMareM_BivalveBColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x10001
sMareM_GlassColorBuf0:
	.incbin "baserom.dol", 0x37DB3C, 0x18
sMareM_GlassColorBuf1:
	.incbin "baserom.dol", 0x37DB54, 0x18
"@1581":
	.incbin "baserom.dol", 0x37DB6C, 0xC
sMareM_GlassColorChange:
  .4byte 0x2
  .4byte "@1581"
  .4byte sMareM_GlassColorBuf0
  .4byte sMareM_GlassColorBuf1
"@1582":
	.incbin "baserom.dol", 0x37DB88, 0x10
sMareM_GlassData:
  .4byte "@1539"
  .4byte 0
  .4byte "@1582"
  .4byte 0
  .4byte sMareM_GlassColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x20000
sMareMB_ElderMat1ColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareMB_ElderMat1ColorBuf0
  .4byte sMareMB_ElderMat1ColorBuf1
sMareMB_ElderMat2ColorChange:
  .4byte 0x2
  .4byte "@1583"
  .4byte sMareMB_ElderMat2ColorBuf0
  .4byte sMareMB_ElderMat2ColorBuf1
sMareMB_ElderMat3ColorChange:
  .4byte 0x2
  .4byte "@1584"
  .4byte sMareMB_ElderMat3ColorBuf0
  .4byte sMareMB_ElderMat3ColorBuf1
"@1585":
	.incbin "baserom.dol", 0x37DBF4, 0x10
sMareMB_ElderData:
  .4byte "@1539"
  .4byte 0
  .4byte "@1585"
  .4byte 0
  .4byte sMareMB_ElderMat1ColorChange
  .4byte 0
  .4byte sMareMB_ElderMat2ColorChange
  .4byte 0
  .4byte sMareMB_ElderMat3ColorChange
  .4byte 0
  .4byte 0x20001
sMareMB_FishingRodMat1ColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareMB_FishingRodMat1ColorBuf0
  .4byte sMareMB_FishingRodMat1ColorBuf1
sMareMB_FishingRodMat2ColorChange:
  .4byte 0x2
  .4byte "@1583"
  .4byte sMareMB_FishingRodMat2ColorBuf0
  .4byte sMareMB_FishingRodMat2ColorBuf1
sMareMB_FishingRodMat3ColorChange:
  .4byte 0x2
  .4byte "@1584"
  .4byte sMareMB_FishingRodMat3ColorBuf0
  .4byte sMareMB_FishingRodMat3ColorBuf1
"@1586":
	.incbin "baserom.dol", 0x37DC60, 0x14
sMareMC_HatFColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareMC_HatFColorBuf0
  .4byte sMareMC_HatFColorBuf1
"@1587":
	.incbin "baserom.dol", 0x37DC84, 0x10
sMareMC_HatFData:
  .4byte "@1539"
  .4byte 0
  .4byte "@1587"
  .4byte 0
  .4byte sMareMC_HatFColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x1
sMareMC_BeardColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareMC_BeardColorBuf0
  .4byte sMareMC_BeardColorBuf1
"@1588":
	.incbin "baserom.dol", 0x37DCD0, 0x14
sMareMC_BeardData:
  .4byte "@1539"
  .4byte 0
  .4byte "@1588"
  .4byte 0
  .4byte sMareMC_BeardColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x1
sMareMC_MustacheColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareMC_MustacheColorBuf0
  .4byte sMareMC_MustacheColorBuf1
"@1589":
	.incbin "baserom.dol", 0x37DD20, 0x14
sMareMC_MustacheData:
  .4byte "@1539"
  .4byte 0
  .4byte "@1589"
  .4byte 0
  .4byte sMareMC_MustacheColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x1
sMareMD_PipeColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareMD_PipeColorBuf0
  .4byte sMareMD_PipeColorBuf1
"@1591":
	.incbin "baserom.dol", 0x37DD70, 0x14
sMareMD_PipeData:
  .4byte "@1590"
  .4byte 0
  .4byte "@1591"
  .4byte 0
  .4byte sMareMD_PipeColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x20001
sMareW_PearlAColorBuf0:
	.incbin "baserom.dol", 0x37DDB0, 0x30
sMareW_PearlAColorBuf1:
	.incbin "baserom.dol", 0x37DDE0, 0x30
sMareW_PearlAColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareW_PearlAColorBuf0
  .4byte sMareW_PearlAColorBuf1
"@1592":
	.incbin "baserom.dol", 0x37DE20, 0x14
sMareW_PearlAData:
  .4byte "@1539"
  .4byte 0
  .4byte "@1592"
  .4byte 0
  .4byte sMareW_PearlAColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x101
sMareW_HatAColorBuf0:
	.incbin "baserom.dol", 0x37DE60, 0x30
sMareW_HatAColorBuf1:
	.incbin "baserom.dol", 0x37DE90, 0x30
sMareW_HatAColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareW_HatAColorBuf0
  .4byte sMareW_HatAColorBuf1
"@1593":
	.incbin "baserom.dol", 0x37DED0, 0x10
sMareW_HatAData:
  .4byte "@1539"
  .4byte 0
  .4byte "@1593"
  .4byte 0
  .4byte sMareW_HatAColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x1
sMareW_HatDColorBuf0:
	.incbin "baserom.dol", 0x37DF0C, 0x58
sMareW_HatDColorBuf1:
	.incbin "baserom.dol", 0x37DF64, 0x58
sMareW_HatDColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareW_HatDColorBuf0
  .4byte sMareW_HatDColorBuf1
"@1594":
	.incbin "baserom.dol", 0x37DFCC, 0x10
sMareW_HatDData:
  .4byte "@1539"
  .4byte 0
  .4byte "@1594"
  .4byte 0
  .4byte sMareW_HatDColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x1
sMareW_ShellAColorBuf0:
	.incbin "baserom.dol", 0x37E008, 0x30
sMareW_ShellAColorBuf1:
	.incbin "baserom.dol", 0x37E038, 0x30
sMareW_ShellAColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareW_ShellAColorBuf0
  .4byte sMareW_ShellAColorBuf1
"@1595":
	.incbin "baserom.dol", 0x37E078, 0x10
sMareW_ShellAData:
  .4byte "@1577"
  .4byte 0
  .4byte "@1595"
  .4byte 0
  .4byte sMareW_ShellAColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x10101
sMareW_ShellBColorBuf0:
	.incbin "baserom.dol", 0x37E0B4, 0x48
sMareW_ShellBColorBuf1:
	.incbin "baserom.dol", 0x37E0FC, 0x48
sMareW_ShellBColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareW_ShellBColorBuf0
  .4byte sMareW_ShellBColorBuf1
"@1596":
	.incbin "baserom.dol", 0x37E154, 0x10
sMareW_ShellBData:
  .4byte "@1577"
  .4byte 0
  .4byte "@1596"
  .4byte 0
  .4byte sMareW_ShellBColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x10001
sMareW_BivalveAColorBuf0:
	.incbin "baserom.dol", 0x37E190, 0x30
sMareW_BivalveAColorBuf1:
	.incbin "baserom.dol", 0x37E1C0, 0x30
sMareW_BivalveAColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareW_BivalveAColorBuf0
  .4byte sMareW_BivalveAColorBuf1
"@1597":
	.incbin "baserom.dol", 0x37E200, 0x14
sMareW_BivalveAData:
  .4byte "@1577"
  .4byte 0
  .4byte "@1597"
  .4byte 0
  .4byte sMareW_BivalveAColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x10001
sMareWB_BabyBodyColorChange:
  .4byte 0x1
  .4byte "@1598"
  .4byte sMareWB_BabyBodyColorBuf0
  .4byte 0
"@1599":
	.incbin "baserom.dol", 0x37E250, 0xC
sMareWB_BabyHatColorChange:
  .4byte 0x2
  .4byte "@1599"
  .4byte sMareWB_BabyHatColorBuf0
  .4byte sMareWB_BabyHatColorBuf1
"@1600":
	.incbin "baserom.dol", 0x37E26C, 0xC
sMareWB_BabyBedColorChange:
  .4byte 0x2
  .4byte "@1600"
  .4byte sMareWB_BabyBedColorBuf0
  .4byte sMareWB_BabyBedColorBuf1
"@1601":
	.incbin "baserom.dol", 0x37E288, 0x10
"@1602":
	.incbin "baserom.dol", 0x37E298, 0x18
sKinopio_SunGlassesData:
  .4byte "@1539"
  .4byte 0
  .4byte "@1602"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
"@1603":
	.incbin "baserom.dol", 0x37E2DC, 0x10
"@1604":
	.incbin "baserom.dol", 0x37E2EC, 0x14
sKinojii_StickData:
  .4byte "@1603"
  .4byte 0
  .4byte "@1604"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
"@1605":
	.incbin "baserom.dol", 0x37E32C, 0x18
sPeach_HairNormalData:
  .4byte "@1539"
  .4byte 0
  .4byte "@1605"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
"@1606":
	.incbin "baserom.dol", 0x37E370, 0xC
"@1607":
	.incbin "baserom.dol", 0x37E37C, 0x14
sPeach_Hand1LData:
  .4byte "@1606"
  .4byte 0
  .4byte "@1607"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
"@1608":
	.incbin "baserom.dol", 0x37E3BC, 0xC
"@1609":
	.incbin "baserom.dol", 0x37E3C8, 0x14
sPeach_Hand1RData:
  .4byte "@1608"
  .4byte 0
  .4byte "@1609"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
"@1610":
	.incbin "baserom.dol", 0x37E408, 0x18
sPeach_HairPonytailData:
  .4byte "@1539"
  .4byte 0
  .4byte "@1610"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
"@1611":
	.incbin "baserom.dol", 0x37E44C, 0x14
sPeach_ParasolData:
  .4byte "@1608"
  .4byte 0
  .4byte "@1611"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
"@1612":
	.incbin "baserom.dol", 0x37E48C, 0x14
sPeach_Hand2LData:
  .4byte "@1606"
  .4byte 0
  .4byte "@1612"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
"@1613":
	.incbin "baserom.dol", 0x37E4CC, 0x14
sPeach_Hand2RData:
  .4byte "@1608"
  .4byte 0
  .4byte "@1613"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
sRaccoonDog_TubeColorBuf0:
	.incbin "baserom.dol", 0x37E50C, 0x10
sRaccoonDog_TubeColorBuf1:
	.incbin "baserom.dol", 0x37E51C, 0x10
sRaccoonDog_TubeColorChange:
  .4byte 0x2
  .4byte "@1614"
  .4byte sRaccoonDog_TubeColorBuf0
  .4byte sRaccoonDog_TubeColorBuf1
"@1615":
	.incbin "baserom.dol", 0x37E53C, 0xC
"@1616":
	.incbin "baserom.dol", 0x37E548, 0xC
sRaccoonDog_TubeData:
  .4byte "@1615"
  .4byte 0
  .4byte "@1616"
  .4byte 0
  .4byte sRaccoonDog_TubeColorChange
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
sMonteM_BodyColorBuf:
	.incbin "baserom.dol", 0x37E580, 0x50
"@1617":
	.incbin "baserom.dol", 0x37E5D0, 0xC
sMonteM_BodyColor:
  .4byte 0x1
  .4byte "@1617"
  .4byte sMonteM_BodyColorBuf
  .4byte 0
sMonteMA_ClothColorBuf0:
	.incbin "baserom.dol", 0x37E5EC, 0x58
sMonteMA_ClothColorBuf1:
	.incbin "baserom.dol", 0x37E644, 0x58
"@1618":
	.incbin "baserom.dol", 0x37E69C, 0xC
sMonteMA_ClothColor:
  .4byte 0x2
  .4byte "@1618"
  .4byte sMonteMA_ClothColorBuf0
  .4byte sMonteMA_ClothColorBuf1
sMonteMB_BodyColorBuf:
	.incbin "baserom.dol", 0x37E6B8, 0x20
sMonteMB_BodyColor:
  .4byte 0x1
  .4byte "@1617"
  .4byte sMonteMB_BodyColorBuf
  .4byte 0
sMonteMB_ClothColorBuf:
	.incbin "baserom.dol", 0x37E6E8, 0x30
sMonteMB_ClothColor:
  .4byte 0x1
  .4byte "@1618"
  .4byte sMonteMB_ClothColorBuf
  .4byte 0
sMonteMC_ClothColorBuf0:
	.incbin "baserom.dol", 0x37E728, 0x58
sMonteMC_ClothColorBuf1:
	.incbin "baserom.dol", 0x37E780, 0x58
sMonteMC_ClothColor:
  .4byte 0x2
  .4byte "@1618"
  .4byte sMonteMC_ClothColorBuf0
  .4byte sMonteMC_ClothColorBuf1
sMonteMD_ClothColorBuf:
	.incbin "baserom.dol", 0x37E7E8, 0x28
sMonteMD_ClothColor:
  .4byte 0x1
  .4byte "@1618"
  .4byte sMonteMD_ClothColorBuf
  .4byte 0
sMonteW_ColorBuf:
	.incbin "baserom.dol", 0x37E820, 0x30
sMonteW_BodyColor:
  .4byte 0x1
  .4byte "@1617"
  .4byte sMonteW_ColorBuf
  .4byte 0
sMonteWA_ClothColorBuf:
	.incbin "baserom.dol", 0x37E860, 0x30
sMonteWA_ClothColor:
  .4byte 0x1
  .4byte "@1618"
  .4byte sMonteWA_ClothColorBuf
  .4byte 0
sMonteWB_ClothColorBuf0:
	.incbin "baserom.dol", 0x37E8A0, 0x48
sMonteWB_ClothColorBuf1:
	.incbin "baserom.dol", 0x37E8E8, 0x48
sMonteWB_ClothColor:
  .4byte 0x2
  .4byte "@1618"
  .4byte sMonteWB_ClothColorBuf0
  .4byte sMonteWB_ClothColorBuf1
sMareM_ColorBuf:
	.incbin "baserom.dol", 0x37E940, 0x30
sMareM_BodyColor:
  .4byte 0x1
  .4byte "@1598"
  .4byte sMareM_ColorBuf
  .4byte 0
sMareW_ColorBuf:
	.incbin "baserom.dol", 0x37E980, 0x30
sMareW_BodyColor:
  .4byte 0x1
  .4byte "@1598"
  .4byte sMareW_ColorBuf
  .4byte 0
sKinopio_BodyColorData0:
	.incbin "baserom.dol", 0x37E9C0, 0x28
sKinopio_BodyColorData1:
	.incbin "baserom.dol", 0x37E9E8, 0x28
"@1619":
	.incbin "baserom.dol", 0x37EA10, 0xC
sKinopio_BodyColor:
  .4byte 0x2
  .4byte "@1619"
  .4byte sKinopio_BodyColorData0
  .4byte sKinopio_BodyColorData1
sKinopio_ClothColorData0:
	.incbin "baserom.dol", 0x37EA2C, 0x28
sKinopio_ClothColorData1:
	.incbin "baserom.dol", 0x37EA54, 0x28
"@1620":
	.incbin "baserom.dol", 0x37EA7C, 0xC
sKinopio_ClothColor:
  .4byte 0x2
  .4byte "@1620"
  .4byte sKinopio_ClothColorData0
  .4byte sKinopio_ClothColorData1
sRaccoonDog_ClothColorBuf0:
	.incbin "baserom.dol", 0x37EA98, 0x10
sRaccoonDog_ClothColorBuf1:
	.incbin "baserom.dol", 0x37EAA8, 0x10
sRaccoonDog_ClothColor:
  .4byte 0x2
  .4byte "@1621"
  .4byte sRaccoonDog_ClothColorBuf0
  .4byte sRaccoonDog_ClothColorBuf1
sMonteM_InitData:
  .4byte 0
  .4byte sMonteM_HatAData
  .4byte sMonteM_MustacheAData
  .4byte sMonteM_GlassesAData
  .4byte sMonteM_GlassesBData
  .4byte sMonteM_HatBData
  .4byte sMonteM_HatEData
  .4byte sMonteM_HatDData
  .4byte sMonteM_HatFData
  .4byte sMonteM_HatGData
  .4byte sMonteM_LapelAData
  .4byte sMonteM_TieBData
  .4byte sMonteM_LuggageData
  .4byte sMonteM_BodyColor
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x42c80000
  .4byte 0x43480000
  .4byte 0x42c80000
  .4byte 0x43480000
sMonteMA_InitData:
  .4byte 0
  .4byte sMonteM_HatAData
  .4byte sMonteM_MustacheAData
  .4byte sMonteM_GlassesAData
  .4byte sMonteM_GlassesBData
  .4byte sMonteM_HatBData
  .4byte sMonteM_HatEData
  .4byte sMonteM_HatDData
  .4byte sMonteM_HatFData
  .4byte sMonteM_HatGData
  .4byte sMonteM_LapelAData
  .4byte sMonteM_TieBData
  .4byte sMonteM_LuggageData
  .4byte sMonteM_BodyColor
  .4byte 0
  .4byte sMonteMA_ClothColor
  .4byte 0
  .4byte 0x42c80000
  .4byte 0x43480000
  .4byte 0x42c80000
  .4byte 0x43480000
sMonteMB_InitData:
  .4byte 0
  .4byte sMonteM_HatAData
  .4byte sMonteM_MustacheAData
  .4byte sMonteM_GlassesAData
  .4byte sMonteM_GlassesBData
  .4byte sMonteM_HatBData
  .4byte sMonteM_HatEData
  .4byte sMonteM_HatDData
  .4byte sMonteM_HatFData
  .4byte sMonteM_HatGData
  .4byte sMonteM_LapelAData
  .4byte sMonteM_TieBData
  .4byte sMonteM_LuggageData
  .4byte sMonteMB_BodyColor
  .4byte 0
  .4byte sMonteMB_ClothColor
  .4byte 0
  .4byte 0x42c80000
  .4byte 0x43480000
  .4byte 0x42c80000
  .4byte 0x43480000
sMonteMC_InitData:
  .4byte 0
  .4byte sMonteM_HatAData
  .4byte sMonteM_MustacheAData
  .4byte sMonteM_GlassesAData
  .4byte sMonteM_GlassesBData
  .4byte sMonteM_HatBData
  .4byte sMonteM_HatEData
  .4byte sMonteM_HatDData
  .4byte sMonteM_HatFData
  .4byte sMonteM_HatGData
  .4byte sMonteM_LapelAData
  .4byte sMonteM_TieBData
  .4byte sMonteM_LuggageData
  .4byte sMonteM_BodyColor
  .4byte 0
  .4byte sMonteMC_ClothColor
  .4byte 0
  .4byte 0x42c80000
  .4byte 0x43480000
  .4byte 0x42c80000
  .4byte 0x43480000
sMonteMD_InitData:
  .4byte 0
  .4byte sMonteM_HatAData
  .4byte sMonteM_MustacheAData
  .4byte sMonteM_GlassesAData
  .4byte sMonteM_GlassesBData
  .4byte sMonteM_HatBData
  .4byte sMonteM_HatEData
  .4byte sMonteM_HatDData
  .4byte sMonteM_HatFData
  .4byte sMonteM_HatGData
  .4byte sMonteM_LapelAData
  .4byte sMonteM_TieBData
  .4byte sMonteM_LuggageData
  .4byte sMonteM_BodyColor
  .4byte 0
  .4byte sMonteMD_ClothColor
  .4byte 0
  .4byte 0x42c80000
  .4byte 0x43480000
  .4byte 0x42c80000
  .4byte 0x43480000
sMonteME_InitData:
  .4byte 0
  .4byte sMonteM_HatAData
  .4byte sMonteM_MustacheAData
  .4byte sMonteM_GlassesAData
  .4byte sMonteM_GlassesBData
  .4byte sMonteM_HatBData
  .4byte sMonteM_HatEData
  .4byte sMonteM_HatDData
  .4byte sMonteM_HatFData
  .4byte sMonteM_HatGData
  .4byte sMonteM_LapelAData
  .4byte sMonteM_TieBData
  .4byte sMonteM_LuggageData
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x42c80000
  .4byte 0x43480000
  .4byte 0x42c80000
  .4byte 0x43480000
sMonteMF_InitData:
  .4byte 0
  .4byte sMonteM_HatAData
  .4byte sMonteM_MustacheAData
  .4byte sMonteM_GlassesAData
  .4byte sMonteM_GlassesBData
  .4byte sMonteM_HatBData
  .4byte sMonteM_HatEData
  .4byte sMonteM_HatDData
  .4byte sMonteM_HatFData
  .4byte sMonteM_HatGData
  .4byte sMonteM_LapelAData
  .4byte sMonteM_TieBData
  .4byte sMonteMF_TubeData
  .4byte sMonteM_BodyColor
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x43160000
  .4byte 0x43480000
  .4byte 0x43160000
  .4byte 0x43480000
sMonteMG_InitData:
  .4byte 0
  .4byte sMonteM_HatAData
  .4byte sMonteM_MustacheAData
  .4byte sMonteM_GlassesAData
  .4byte sMonteM_GlassesBData
  .4byte sMonteM_HatBData
  .4byte sMonteM_HatEData
  .4byte sMonteM_HatDData
  .4byte sMonteM_HatFData
  .4byte sMonteM_HatGData
  .4byte sMonteM_LapelAData
  .4byte sMonteM_TieBData
  .4byte sMonteMG_MopData
  .4byte sMonteM_BodyColor
  .4byte 0
  .4byte sMonteMC_ClothColor
  .4byte 0
  .4byte 0x42c80000
  .4byte 0x43480000
  .4byte 0x42c80000
  .4byte 0x43480000
sMonteMH_InitData:
  .4byte 0
  .4byte sMonteM_HatAData
  .4byte sMonteM_MustacheAData
  .4byte sMonteM_GlassesAData
  .4byte sMonteM_GlassesBData
  .4byte sMonteM_HatBData
  .4byte sMonteM_HatEData
  .4byte sMonteM_HatDData
  .4byte sMonteM_HatFData
  .4byte sMonteM_HatGData
  .4byte sMonteM_LapelAData
  .4byte sMonteM_TieBData
  .4byte sMonteMH_UkleleData
  .4byte sMonteM_BodyColor
  .4byte 0
  .4byte sMonteMA_ClothColor
  .4byte 0
  .4byte 0x42c80000
  .4byte 0x43480000
  .4byte 0x42c80000
  .4byte 0x43480000
sMonteW_InitData:
  .4byte 0
  .4byte sMonteW_FlowerData
  .4byte sMonteW_HatAData
  .4byte sMonteW_GlassBData
  .4byte sMonteW_ArrowRData
  .4byte sMonteW_ArrowLData
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte sMonteW_BodyColor
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x42dc0000
  .4byte 0x433e0000
  .4byte 0x42dc0000
  .4byte 0x433e0000
sMonteWA_InitData:
  .4byte 0
  .4byte sMonteW_FlowerData
  .4byte sMonteW_HatAData
  .4byte sMonteW_GlassBData
  .4byte sMonteW_ArrowRData
  .4byte sMonteW_ArrowLData
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte sMonteW_BodyColor
  .4byte 0
  .4byte sMonteWA_ClothColor
  .4byte 0
  .4byte 0x42dc0000
  .4byte 0x433e0000
  .4byte 0x42dc0000
  .4byte 0x433e0000
sMonteWB_InitData:
  .4byte 0
  .4byte sMonteW_FlowerData
  .4byte sMonteW_HatAData
  .4byte sMonteW_GlassBData
  .4byte sMonteW_ArrowRData
  .4byte sMonteW_ArrowLData
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte sMonteW_BodyColor
  .4byte 0
  .4byte sMonteWB_ClothColor
  .4byte 0
  .4byte 0x42dc0000
  .4byte 0x433e0000
  .4byte 0x42dc0000
  .4byte 0x433e0000
sMonteWC_InitData:
  .4byte 0
  .4byte sMonteW_FlowerData
  .4byte sMonteW_HatAData
  .4byte sMonteW_GlassBData
  .4byte sMonteW_ArrowRData
  .4byte sMonteW_ArrowLData
  .4byte sMonteWC_HatCData
  .4byte sMonteWC_BraceletRData
  .4byte sMonteWC_BraceletLData
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte sMonteW_BodyColor
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x42dc0000
  .4byte 0x433e0000
  .4byte 0x42dc0000
  .4byte 0x433e0000
sMareM_InitData:
  .4byte 0
  .4byte sMareM_HatAData
  .4byte sMareM_HatBData
  .4byte 0
  .4byte sMareM_HatEData
  .4byte sMareM_ShellAData
  .4byte sMareM_ShellBData
  .4byte sMareM_GlassData
  .4byte sMareM_BivalveBData
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte sMareM_BodyColor
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x42a00000
  .4byte 0x42dc0000
  .4byte 0x42a00000
  .4byte 0x42dc0000
sMareMB_InitData:
  .4byte 0
  .4byte sMareM_HatAData
  .4byte sMareM_HatBData
  .4byte 0
  .4byte sMareM_HatEData
  .4byte sMareM_ShellAData
  .4byte sMareM_ShellBData
  .4byte sMareM_GlassData
  .4byte sMareM_BivalveBData
  .4byte sMareMB_ElderData
  .4byte sMareMB_FishingRodData
  .4byte 0
  .4byte 0
  .4byte sMareM_BodyColor
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x42a00000
  .4byte 0x42dc0000
  .4byte 0x42a00000
  .4byte 0x42dc0000
sMareMC_InitData:
  .4byte 0
  .4byte sMareM_HatAData
  .4byte sMareM_HatBData
  .4byte 0
  .4byte sMareM_HatEData
  .4byte sMareM_ShellAData
  .4byte sMareM_ShellBData
  .4byte sMareM_GlassData
  .4byte sMareM_BivalveBData
  .4byte sMareMC_HatFData
  .4byte sMareMC_BeardData
  .4byte sMareMC_MustacheData
  .4byte 0
  .4byte sMareM_BodyColor
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x42a00000
  .4byte 0x42dc0000
  .4byte 0x42a00000
  .4byte 0x42dc0000
sMareMD_InitData:
  .4byte 0
  .4byte sMareM_HatAData
  .4byte sMareM_HatBData
  .4byte 0
  .4byte sMareM_HatEData
  .4byte sMareM_ShellAData
  .4byte sMareM_ShellBData
  .4byte sMareM_GlassData
  .4byte sMareM_BivalveBData
  .4byte sMareMD_PipeData
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte sMareM_BodyColor
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x42a00000
  .4byte 0x42dc0000
  .4byte 0x42a00000
  .4byte 0x42dc0000
sMareW_InitData:
  .4byte 0
  .4byte sMareW_PearlAData
  .4byte sMareW_HatAData
  .4byte 0
  .4byte 0
  .4byte sMareW_ShellAData
  .4byte sMareW_ShellBData
  .4byte 0
  .4byte 0
  .4byte sMareW_BivalveAData
  .4byte sMareW_HatDData
  .4byte 0
  .4byte 0
  .4byte sMareW_BodyColor
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x42a00000
  .4byte 0x43020000
  .4byte 0x42a00000
  .4byte 0x43020000
sMareWB_InitData:
  .4byte 0
  .4byte sMareW_PearlAData
  .4byte sMareW_HatAData
  .4byte 0
  .4byte 0
  .4byte sMareW_ShellAData
  .4byte sMareW_ShellBData
  .4byte 0
  .4byte 0
  .4byte sMareW_BivalveAData
  .4byte sMareW_HatDData
  .4byte sMareWB_BabyData
  .4byte 0
  .4byte sMareW_BodyColor
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x42a00000
  .4byte 0x43020000
  .4byte 0x42a00000
  .4byte 0x43020000
sKinopio_InitData:
  .4byte 0
  .4byte sKinopio_SunGlassesData
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte sKinopio_BodyColor
  .4byte 0
  .4byte sKinopio_ClothColor
  .4byte 0
  .4byte 0x42200000
  .4byte 0x42dc0000
  .4byte 0x42200000
  .4byte 0x42dc0000
sKinojii_InitData:
  .4byte 0
  .4byte sKinojii_StickData
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x42200000
  .4byte 0x42dc0000
  .4byte 0x42200000
  .4byte 0x42dc0000
sPeach_InitData:
  .4byte sPeach_TakenMiddle
  .4byte sPeach_HairNormalData
  .4byte sPeach_Hand1LData
  .4byte sPeach_Hand1RData
  .4byte sPeach_HairPonytailData
  .4byte sPeach_ParasolData
  .4byte sPeach_Hand2LData
  .4byte sPeach_Hand2RData
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0x42480000
  .4byte 0x432a0000
  .4byte 0x42480000
  .4byte 0x432a0000
sRaccoonDog_InitData:
  .4byte 0
  .4byte sRaccoonDog_TubeData
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte sRaccoonDog_ClothColor
  .4byte 0
  .4byte 0x42c80000
  .4byte 0x43340000
  .4byte 0x42c80000
  .4byte 0x43340000
sSunflowerL_InitData:
	.incbin "baserom.dol", 0x37F254, 0x54
sSunflowerS_InitData:
	.incbin "baserom.dol", 0x37F2A8, 0x54
sBoardNpc_InitData:
	.incbin "baserom.dol", 0x37F2FC, 0x54

.section .data, "wa"  # 0x803A8380 - 0x803E6000
"@1431":
	.incbin "baserom.dol", 0x3B63F0, 0xC
"@1411":
	.incbin "baserom.dol", 0x3B63FC, 0xC
"@1210":
	.incbin "baserom.dol", 0x3B6408, 0x10
sMareMB_FishingRodData:
  .4byte 0
  .4byte 0
  .4byte "@1586"
  .4byte 0
  .4byte sMareMB_FishingRodMat1ColorChange
  .4byte 0
  .4byte sMareMB_FishingRodMat2ColorChange
  .4byte 0
  .4byte sMareMB_FishingRodMat3ColorChange
  .4byte 0
  .4byte 0x1
sMareWB_BabyData:
  .4byte 0
  .4byte 0
  .4byte "@1601"
  .4byte 0
  .4byte sMareWB_BabyBodyColorChange
  .4byte 0
  .4byte sMareWB_BabyHatColorChange
  .4byte 0
  .4byte sMareWB_BabyBedColorChange
  .4byte 0
  .4byte 0x20000
sAllNpcInitData$1622:
  .4byte sMonteM_InitData
  .4byte sMonteMA_InitData
  .4byte sMonteMB_InitData
  .4byte sMonteMC_InitData
  .4byte sMonteMD_InitData
  .4byte sMonteME_InitData
  .4byte sMonteMF_InitData
  .4byte sMonteMG_InitData
  .4byte sMonteMH_InitData
  .4byte sMonteW_InitData
  .4byte sMonteWA_InitData
  .4byte sMonteWB_InitData
  .4byte sMonteWC_InitData
  .4byte sMareM_InitData
  .4byte sMareM_InitData
  .4byte sMareMB_InitData
  .4byte sMareMC_InitData
  .4byte sMareMD_InitData
  .4byte sMareW_InitData
  .4byte sMareW_InitData
  .4byte sMareWB_InitData
  .4byte sKinopio_InitData
  .4byte sKinojii_InitData
  .4byte sPeach_InitData
  .4byte sRaccoonDog_InitData
  .4byte sSunflowerL_InitData
  .4byte sSunflowerS_InitData
  .4byte 0
  .4byte sBoardNpc_InitData
  .4byte 0

.section .sdata2, "a"  # 0x8040B460 - 0x80414020
.balign 8
sPeach_TakenMiddle:
  .4byte "@1535"
  .4byte "@1536"
"@1539":
	.incbin "baserom.dol", 0x3E6480, 0x8
sMonteM_TieBColorBuf0:
	.incbin "baserom.dol", 0x3E6488, 0x8
sMonteM_TieBColorBuf1:
	.incbin "baserom.dol", 0x3E6490, 0x8
"@1573":
	.incbin "baserom.dol", 0x3E6498, 0x8
"@1577":
	.incbin "baserom.dol", 0x3E64A0, 0x8
sMareMB_ElderMat1ColorBuf0:
	.incbin "baserom.dol", 0x3E64A8, 0x8
sMareMB_ElderMat1ColorBuf1:
	.incbin "baserom.dol", 0x3E64B0, 0x8
sMareMB_ElderMat2ColorBuf0:
	.incbin "baserom.dol", 0x3E64B8, 0x8
sMareMB_ElderMat2ColorBuf1:
	.incbin "baserom.dol", 0x3E64C0, 0x8
"@1583":
	.incbin "baserom.dol", 0x3E64C8, 0x8
sMareMB_ElderMat3ColorBuf0:
	.incbin "baserom.dol", 0x3E64D0, 0x8
sMareMB_ElderMat3ColorBuf1:
	.incbin "baserom.dol", 0x3E64D8, 0x8
"@1584":
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
"@1590":
	.incbin "baserom.dol", 0x3E6558, 0x8
sMareWB_BabyBodyColorBuf0:
	.incbin "baserom.dol", 0x3E6560, 0x8
"@1598":
	.incbin "baserom.dol", 0x3E6568, 0x8
sMareWB_BabyHatColorBuf0:
	.incbin "baserom.dol", 0x3E6570, 0x8
sMareWB_BabyHatColorBuf1:
	.incbin "baserom.dol", 0x3E6578, 0x8
sMareWB_BabyBedColorBuf0:
	.incbin "baserom.dol", 0x3E6580, 0x8
sMareWB_BabyBedColorBuf1:
	.incbin "baserom.dol", 0x3E6588, 0x8
"@1614":
	.incbin "baserom.dol", 0x3E6590, 0x8
"@1621":
	.incbin "baserom.dol", 0x3E6598, 0x8

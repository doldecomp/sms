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
	.4byte 0x83818382
	.4byte 0x838A82AA
	.4byte 0x91AB82E8
	.4byte 0x82DC82B9
	.4byte 0x82F10A00
"@1530":
	.asciz "__ROOT_JOINT__"
	.balign 4
"@1535":
	.asciz "/scene/peach/peach_carry_offset.bmd"
"@1536":
	.asciz "null_peach_center"
	.balign 4
sMonteM_HatAColorBuf0:
	.4byte 0x00C800C8
	.4byte 0x007800FF
	.4byte 0x00460046
	.4byte 0x004600FF
	.4byte 0x00C800C8
	.4byte 0x009600FF
	.4byte 0x006400C8
	.4byte 0x00C800FF
	.4byte 0x0000001E
	.4byte 0x009600FF
	.4byte 0x00C800DC
	.4byte 0x007800FF
	.4byte 0x008C0032
	.4byte 0x000000FF
sMonteM_HatAColorBuf1:
	.4byte 0x00A00082
	.4byte 0x003200FF
	.4byte 0x00460046
	.4byte 0x004600FF
	.4byte 0x00C800C8
	.4byte 0x009600FF
	.4byte 0x00320078
	.4byte 0x00A000FF
	.4byte 0x0000001E
	.4byte 0x009600FF
	.4byte 0x00000082
	.4byte 0x003200FF
	.4byte 0x008C0032
	.4byte 0x000000FF
"@1537":
	.asciz "_boushi_mat"
sMonteM_HatAColorChange:
  .4byte 0x2
  .4byte "@1537"
  .4byte sMonteM_HatAColorBuf0
  .4byte sMonteM_HatAColorBuf1
sMonteM_HatABeltColorBuf:
	.4byte 0x00640000
	.4byte 0x000000FF
	.4byte 0
	.4byte 0x000000FF
	.4byte 0
	.4byte 0x000000FF
	.4byte 0x00000064
	.4byte 0x012C00FF
	.4byte 0x000000C8
	.4byte 0x009600FF
	.4byte 0x00000082
	.4byte 0x008200FF
	.4byte 0x00640000
	.4byte 0x000000FF
"@1538":
	.asciz "_obi_mat"
	.balign 4
sMonteM_HatABeltColorChange:
  .4byte 0x1
  .4byte "@1538"
  .4byte sMonteM_HatABeltColorBuf
  .4byte 0
"@1540":
	.asciz "hatA_model.bmd"
	.balign 4
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
	.4byte 0x00FF00FF
	.4byte 0x003C00FF
	.4byte 0x00FF0082
	.4byte 0x000000FF
	.4byte 0x00FF00FF
	.4byte 0x000000FF
sMonteM_HatBColorChange:
  .4byte 0
  .4byte "@1537"
  .4byte sMonteM_HatBColorBuf
  .4byte 0
sMonteM_HatBBeltColorBuf:
	.4byte 0x00640046
	.4byte 0x003200FF
	.4byte 0x00640046
	.4byte 0x003200FF
	.4byte 0x00000064
	.4byte 0x00FF00FF
sMonteM_HatBBeltColorChange:
  .4byte 0x1
  .4byte "@1538"
  .4byte sMonteM_HatBBeltColorBuf
  .4byte 0
"@1541":
	.asciz "hatB_model.bmd"
	.balign 4
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
	.4byte 0x00000064
	.4byte 0x00E600FF
	.4byte 0x01900190
	.4byte 0x015E00FF
sMonteM_HatDColorChange:
  .4byte 0x1
  .4byte "@1538"
  .4byte sMonteM_HatDColorBuf
  .4byte 0
"@1542":
	.asciz "hatD_model.bmd"
	.balign 4
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
	.4byte 0x00000046
	.4byte 0x009600FF
	.4byte 0x00E600E6
	.4byte 0x00D200FF
sMonteM_HatEColorBuf1:
	.4byte 0x00E600E6
	.4byte 0x00D200FF
	.4byte 0x00000046
	.4byte 0x009600FF
sMonteM_HatEColorChange:
  .4byte 0x2
  .4byte "@1537"
  .4byte sMonteM_HatEColorBuf0
  .4byte sMonteM_HatEColorBuf1
"@1543":
	.asciz "hatE_model.bmd"
	.balign 4
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
	.4byte 0x00320096
	.4byte 0x008200FF
	.4byte 0x003C0028
	.4byte 0x000000FF
sMonteM_HatFColorBuf1:
	.4byte 0x00E600E6
	.4byte 0x00D200FF
	.4byte 0x00A00096
	.4byte 0x003C00FF
sMonteM_HatFColorChange:
  .4byte 0x2
  .4byte "@1537"
  .4byte sMonteM_HatFColorBuf0
  .4byte sMonteM_HatFColorBuf1
"@1544":
	.asciz "hatF_model.bmd"
	.balign 4
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
	.4byte 0x00000032
	.4byte 0x007800FF
	.4byte 0x00640078
	.4byte 0x000000FF
	.4byte 0x00AA0078
	.4byte 0x000000FF
	.4byte 0x00000064
	.4byte 0x00FF00FF
	.4byte 0x008C0032
	.4byte 0x000000FF
sMonteM_HatGColorBuf1:
	.4byte 0x00000032
	.4byte 0x007800FF
	.4byte 0x00640078
	.4byte 0x000000FF
	.4byte 0x00AA0078
	.4byte 0x000000FF
	.4byte 0x000000B4
	.4byte 0x00FF00FF
	.4byte 0x00C80078
	.4byte 0x000000FF
sMonteM_HatGColorChange:
  .4byte 0x2
  .4byte "@1537"
  .4byte sMonteM_HatGColorBuf0
  .4byte sMonteM_HatGColorBuf1
"@1545":
	.asciz "hatG_model.bmd"
	.balign 4
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
	.4byte 0x000000FF
	.4byte 0x00FF00FF
	.4byte 0x009600FF
	.4byte 0x00640000
	.4byte 0x000000FF
	.4byte 0x00FF00C8
	.4byte 0x000000FF
"@1546":
	.asciz "_hige_mat"
	.balign 4
sMonteM_MustacheAColorChange:
  .4byte 0x1
  .4byte "@1546"
  .4byte sMonteM_MustacheAColorBuf0
  .4byte 0
"@1547":
	.asciz "higeA_model.bmd"
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
	.asciz "glassesA_model.bmd"
	.balign 4
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
	.4byte 0x00FF00E6
	.4byte 0x003200FF
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
	.4byte 0
	.4byte 0x000000FF
"@1549":
	.asciz "_megane_mat"
sMonteM_GlassesBColorChange:
  .4byte 0
  .4byte "@1549"
  .4byte sMonteM_GlassesBColorBuf0
  .4byte 0
"@1550":
	.asciz "glassesB_model.bmd"
	.balign 4
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
	.4byte 0x00000046
	.4byte 0x009600FF
	.4byte 0x00FF00FF
	.4byte 0x00E600FF
sMonteM_LapelAColorBuf1:
	.4byte 0x00FF00FF
	.4byte 0x00E600FF
	.4byte 0x00000046
	.4byte 0x009600FF
"@1551":
	.asciz "_eri_mat"
	.balign 4
sMonteM_LapelAColorChange:
  .4byte 0x2
  .4byte "@1551"
  .4byte sMonteM_LapelAColorBuf0
  .4byte sMonteM_LapelAColorBuf1
"@1552":
	.asciz "body_jnt"
	.balign 4
"@1553":
	.asciz "eriA_model.bmd"
	.balign 4
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
	.asciz "_tie_mat"
	.balign 4
sMonteM_TieBColorChange:
  .4byte 0x2
  .4byte "@1554"
  .4byte sMonteM_TieBColorBuf0
  .4byte sMonteM_TieBColorBuf1
"@1555":
	.asciz "tieB_model.bmd"
	.balign 4
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
	.asciz "nimotsu_model.bmd"
	.balign 4
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
	.asciz "tube_model.bmd"
	.balign 4
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
	.asciz "handR_jnt"
	.balign 4
"@1559":
	.asciz "mop_model.bmd"
	.balign 4
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
	.asciz "uklele_model.bmd"
	.balign 4
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
	.4byte 0x00FF00FF
	.4byte 0x000000FF
	.4byte 0x00FF00FF
	.4byte 0x000000FF
	.4byte 0x00FF00FF
	.4byte 0x000000FF
"@1561":
	.asciz "_naka_mat"
	.balign 4
sMonteW_FlowerInColorChange:
  .4byte 0x1
  .4byte "@1561"
  .4byte sMonteW_FlowerInColorBuf
  .4byte 0
sMonteW_FlowerOutColorBuf:
	.4byte 0x00DC0028
	.4byte 0x007800FF
	.4byte 0x00DC0028
	.4byte 0x000000FF
	.4byte 0x00C800DC
	.4byte 0x000000FF
"@1562":
	.asciz "_hana_mat"
	.balign 4
sMonteW_FlowerOutColorChange:
  .4byte 0
  .4byte "@1562"
  .4byte sMonteW_FlowerOutColorBuf
  .4byte 0
"@1563":
	.asciz "yashi_jnt"
	.balign 4
"@1564":
	.asciz "flower_model.bmd"
	.balign 4
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
	.4byte 0x00C800C8
	.4byte 0x007800FF
	.4byte 0x00A00000
	.4byte 0x000000FF
	.4byte 0x00BE0096
	.4byte 0x007800FF
	.4byte 0x00C800DC
	.4byte 0x00DC00FF
sMonteW_HatAMainColorBuf1:
	.4byte 0x00A00082
	.4byte 0x003200FF
	.4byte 0x00A00000
	.4byte 0x000000FF
	.4byte 0x00780046
	.4byte 0x003200FF
	.4byte 0x006400AA
	.4byte 0x00C800FF
sMonteW_HatAMainColorChange:
  .4byte 0x2
  .4byte "@1537"
  .4byte sMonteW_HatAMainColorBuf0
  .4byte sMonteW_HatAMainColorBuf1
sMonteW_HatABeltColorBuf:
	.4byte 0x012C0064
	.4byte 0x006400FF
	.4byte 0x015E0064
	.4byte 0x006400FF
	.4byte 0x00640000
	.4byte 0x000000FF
	.4byte 0x01900190
	.4byte 0x017C00FF
sMonteW_HatABeltColorChange:
  .4byte 0x1
  .4byte "@1538"
  .4byte sMonteW_HatABeltColorBuf
  .4byte 0
"@1565":
	.asciz "hwA_model.bmd"
	.balign 4
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
	.4byte 0x00960000
	.4byte 0x000000FF
	.4byte 0x000000C8
	.4byte 0x000000FF
	.4byte 0x00C800C8
	.4byte 0x000000FF
sMonteW_GlassBColorChange:
  .4byte 0
  .4byte "@1549"
  .4byte sMonteW_GlassBColorBuf
  .4byte 0
"@1566":
	.asciz "gwB_model.bmd"
	.balign 4
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
	.asciz "arrowR_model.bmd"
	.balign 4
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
	.asciz "arrowL_model.bmd"
	.balign 4
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
	.asciz "hwC_model.bmd"
	.balign 4
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
	.asciz "udewaR_model.bmd"
	.balign 4
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
	.asciz "handL_jnt"
	.balign 4
"@1572":
	.asciz "udewaL_model.bmd"
	.balign 4
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
	.4byte 0x00140000
	.4byte 0x00A000FF
	.4byte 0x0014003C
	.4byte 0x006400FF
	.4byte 0x00AA0078
	.4byte 0x001400FF
	.4byte 0x000A000A
	.4byte 0x000A00FF
	.4byte 0xFFD8FFF6
	.4byte 0xFFF600FF
	.4byte 0x00000014
	.4byte 0x001E00FF
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
	.4byte 0xFF60FF38
	.4byte 0xFFBA00FF
	.4byte 0x00F000F0
	.4byte 0x00BE00FF
sMareM_HatAColorBuf1:
	.4byte 0x008C00AA
	.4byte 0x00F000FF
	.4byte 0x00A000E6
	.4byte 0x004600FF
	.4byte 0xFFEC00DC
	.4byte 0x005000FF
	.4byte 0x0096FFE2
	.4byte 0x002800FF
	.4byte 0xFF88FF60
	.4byte 0xFF2400FF
	.4byte 0x006400A0
	.4byte 0x008C00FF
	.4byte 0x008C006E
	.4byte 0x00BE00FF
	.4byte 0xFF1A0000
	.4byte 0xFF6000FF
	.4byte 0x0050003C
	.4byte 0x002800FF
sMareM_HatAColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareM_HatAColorBuf0
  .4byte sMareM_HatAColorBuf1
"@1574":
	.asciz "mareMhat_A.bmd"
	.balign 4
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
	.4byte 0x00AA00E6
	.4byte 0x00D200FF
	.4byte 0x009600A0
	.4byte 0x00C800FF
	.4byte 0x00C800BE
	.4byte 0x000A00FF
	.4byte 0x00FFFFF6
	.4byte 0xFFCE00FF
	.4byte 0x003C0064
	.4byte 0x009600FF
	.4byte 0x0032000A
	.4byte 0x006E00FF
sMareM_HatBColorBuf1:
	.4byte 0x00190014
	.4byte 0x001400FF
	.4byte 0xFF24FF1A
	.4byte 0x000000FF
	.4byte 0xFF38FF1A
	.4byte 0x000000FF
	.4byte 0xFF6AFF9C
	.4byte 0x001400FF
	.4byte 0x00140014
	.4byte 0x000A00FF
	.4byte 0x00A00082
	.4byte 0x00DC00FF
sMareM_HatBColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareM_HatBColorBuf0
  .4byte sMareM_HatBColorBuf1
"@1575":
	.asciz "mareMhat_B.bmd"
	.balign 4
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
	.4byte 0x014AFF6A
	.4byte 0x00C800FF
	.4byte 0x00FF00FF
	.4byte 0x01A400FF
	.4byte 0x00000078
	.4byte 0x006400FF
	.4byte 0x00BE00F0
	.4byte 0x00FA00FF
	.4byte 0x00140078
	.4byte 0x001E00FF
	.4byte 0xFFC4FF7E
	.4byte 0x000000FF
	.4byte 0x00DC00DC
	.4byte 0x00F000FF
	.4byte 0x00C800E6
	.4byte 0x00A000FF
	.4byte 0x0000000A
	.4byte 0x001400FF
	.4byte 0x00C800D2
	.4byte 0x00E600FF
	.4byte 0x00C800F0
	.4byte 0x00F000FF
sMareM_HatEColorBuf1:
	.4byte 0x00AA00D2
	.4byte 0x00E600FF
	.4byte 0x00E6FF9C
	.4byte 0x014000FF
	.4byte 0xFFBAFFD8
	.4byte 0x008200FF
	.4byte 0x003C005A
	.4byte 0xFFBA00FF
	.4byte 0x00DC00E6
	.4byte 0x006400FF
	.4byte 0xFFF6FFCE
	.4byte 0x003200FF
	.4byte 0x0064003C
	.4byte 0x00B400FF
	.4byte 0x00C800B4
	.4byte 0x001E00FF
	.4byte 0x008C010E
	.4byte 0x003200FF
	.4byte 0x00BE0032
	.4byte 0x001E00FF
	.4byte 0x005A006E
	.4byte 0x00A000FF
sMareM_HatEColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareM_HatEColorBuf0
  .4byte sMareM_HatEColorBuf1
"@1576":
	.asciz "mareMhat_E.bmd"
	.balign 4
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
	.4byte 0x0064003C
	.4byte 0x003200FF
	.4byte 0xFFC401C2
	.4byte 0x00FA00FF
	.4byte 0x0000FF38
	.4byte 0xFF0600FF
	.4byte 0xFF2EFF24
	.4byte 0xFE5C00FF
	.4byte 0xFF56FF38
	.4byte 0x000000FF
	.4byte 0x008C00C8
	.4byte 0x00FF00FF
	.4byte 0x0000FF92
	.4byte 0x001E00FF
	.4byte 0x00BE0078
	.4byte 0x001400FF
	.4byte 0x0096006E
	.4byte 0xFFD800FF
	.4byte 0x00C80032
	.4byte 0x002800FF
sMareM_ShellAColorBuf1:
	.4byte 0x01F401F4
	.4byte 0x015E00FF
	.4byte 0x01400046
	.4byte 0x006400FF
	.4byte 0xFFF6FFEC
	.4byte 0xFEA200FF
	.4byte 0x006E0064
	.4byte 0x00C800FF
	.4byte 0x00B400BE
	.4byte 0x00AA00FF
	.4byte 0xFF01FF10
	.4byte 0xFF9C00FF
	.4byte 0x00E600E6
	.4byte 0x00E600FF
	.4byte 0x00F000FF
	.4byte 0x008200FF
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
sMareM_ShellAColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareM_ShellAColorBuf0
  .4byte sMareM_ShellAColorBuf1
"@1578":
	.asciz "mareMmakigai_A.bmd"
	.balign 4
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
	.4byte 0x006E007D
	.4byte 0x00C800FF
	.4byte 0xFF7EFF88
	.4byte 0xFF9C00FF
	.4byte 0xFF88001E
	.4byte 0x001400FF
	.4byte 0xFF38FF38
	.4byte 0xFF0600FF
	.4byte 0xFF60FF38
	.4byte 0x000A00FF
	.4byte 0xFF38FF1A
	.4byte 0xFF6000FF
	.4byte 0x00280028
	.4byte 0x002800FF
	.4byte 0x00280014
	.4byte 0x000000FF
	.4byte 0x0028001E
	.4byte 0xFF6000FF
	.4byte 0x0028001E
	.4byte 0x001400FF
sMareM_ShellBColorBuf1:
	.4byte 0x00190014
	.4byte 0x001400FF
	.4byte 0x012C0118
	.4byte 0x000A00FF
	.4byte 0x001E0000
	.4byte 0x000A00FF
	.4byte 0xFF42FF9C
	.4byte 0x001400FF
	.4byte 0xFFC4FFBA
	.4byte 0x000A00FF
	.4byte 0xFF9CFF92
	.4byte 0x00C800FF
	.4byte 0x001EFF6A
	.4byte 0xFFC400FF
	.4byte 0x008C0046
	.4byte 0x000A00FF
	.4byte 0x005A0050
	.4byte 0x009600FF
	.4byte 0x00AA0096
	.4byte 0x007800FF
sMareM_ShellBColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareM_ShellBColorBuf0
  .4byte sMareM_ShellBColorBuf1
"@1579":
	.asciz "mareMmakigai_B.bmd"
	.balign 4
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
	.4byte 0x014AFF6A
	.4byte 0x00C800FF
	.4byte 0x00FF00FF
	.4byte 0x01A400FF
	.4byte 0x00000078
	.4byte 0x006400FF
	.4byte 0x0064003C
	.4byte 0x00B400FF
	.4byte 0x00C800E6
	.4byte 0x00A000FF
	.4byte 0x0000000A
	.4byte 0x001400FF
sMareM_BivalveBColorBuf1:
	.4byte 0x00AA00D2
	.4byte 0x00E600FF
	.4byte 0x00E6FF9C
	.4byte 0x014000FF
	.4byte 0xFFBAFFD8
	.4byte 0x008200FF
	.4byte 0x00DC00DC
	.4byte 0x00F000FF
	.4byte 0x00C800B4
	.4byte 0x001E00FF
	.4byte 0x008C010E
	.4byte 0x003200FF
sMareM_BivalveBColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareM_BivalveBColorBuf0
  .4byte sMareM_BivalveBColorBuf1
"@1580":
	.asciz "mareMbivalve_B.bmd"
	.balign 4
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
	.4byte 0xFF56FF38
	.4byte 0x001400FF
	.4byte 0x00280000
	.4byte 0x000A00FF
	.4byte 0x00280046
	.4byte 0x001400FF
sMareM_GlassColorBuf1:
	.4byte 0x00BE00B4
	.4byte 0x00DC00FF
	.4byte 0x00500078
	.4byte 0x001400FF
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
"@1581":
	.asciz "_matgrass"
	.balign 4
sMareM_GlassColorChange:
  .4byte 0x2
  .4byte "@1581"
  .4byte sMareM_GlassColorBuf0
  .4byte sMareM_GlassColorBuf1
"@1582":
	.asciz "mareM_grass.bmd"
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
	.asciz "mareMB_set.bmd"
	.balign 4
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
	.asciz "mareMBTurizao.bmd"
	.balign 4
sMareMC_HatFColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareMC_HatFColorBuf0
  .4byte sMareMC_HatFColorBuf1
"@1587":
	.asciz "mareMChat_F.bmd"
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
	.asciz "mareMCagohige.bmd"
	.balign 4
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
	.asciz "mareMCkuchihige.bmd"
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
	.asciz "mareMDhoragai_A.bmd"
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
	.4byte 0xFFECFFF6
	.4byte 0xFF9C00FF
	.4byte 0xFFE2FFEC
	.4byte 0x000000FF
	.4byte 0x00280014
	.4byte 0x004600FF
	.4byte 0x0014006E
	.4byte 0x000000FF
	.4byte 0xFFF6FFF6
	.4byte 0x00F000FF
	.4byte 0x00A00096
	.4byte 0x00AA00FF
sMareW_PearlAColorBuf1:
	.4byte 0x00DC00FF
	.4byte 0x00FF00FF
	.4byte 0x00DC00F0
	.4byte 0x00B400FF
	.4byte 0x00640046
	.4byte 0x00A500FF
	.4byte 0x00AAFFF6
	.4byte 0x009600FF
	.4byte 0x00960096
	.4byte 0x00F000FF
	.4byte 0x00FA00FF
	.4byte 0x00E600FF
sMareW_PearlAColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareW_PearlAColorBuf0
  .4byte sMareW_PearlAColorBuf1
"@1592":
	.asciz "mareWpearl_A.bmd"
	.balign 4
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
	.4byte 0x000A0000
	.4byte 0x007800FF
	.4byte 0x0104003C
	.4byte 0x001400FF
	.4byte 0xFF56FF38
	.4byte 0xFF0600FF
	.4byte 0x00460118
	.4byte 0x006400FF
	.4byte 0x00D200C8
	.4byte 0x004600FF
	.4byte 0x00E60064
	.4byte 0x00AA00FF
sMareW_HatAColorBuf1:
	.4byte 0x00780096
	.4byte 0xFFCE00FF
	.4byte 0xFFDDFFCE
	.4byte 0xFF6A00FF
	.4byte 0x00F000F0
	.4byte 0x00BE00FF
	.4byte 0x008C0050
	.4byte 0x00DC00FF
	.4byte 0x008C0064
	.4byte 0x003200FF
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
sMareW_HatAColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareW_HatAColorBuf0
  .4byte sMareW_HatAColorBuf1
"@1593":
	.asciz "mareWhatW_A.bmd"
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
	.4byte 0x00640000
	.4byte 0x001E00FF
	.4byte 0x001E001E
	.4byte 0x007800FF
	.4byte 0xFFC4FF9C
	.4byte 0xFFD800FF
	.4byte 0xFFF6008C
	.4byte 0x003C00FF
	.4byte 0xFF380028
	.4byte 0xFF9C00FF
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
	.4byte 0x00DC00F0
	.4byte 0xFFEC00FF
	.4byte 0x00FA0096
	.4byte 0x00D200FF
	.4byte 0x00DC00F0
	.4byte 0xFFEC00FF
	.4byte 0x00FF00FF
	.4byte 0x009600FF
	.4byte 0x00E600FA
	.4byte 0x00E600FF
sMareW_HatDColorBuf1:
	.4byte 0xFFCE0014
	.4byte 0x003C00FF
	.4byte 0x00280078
	.4byte 0x00C800FF
	.4byte 0x0050001E
	.4byte 0x008C00FF
	.4byte 0x00DC000A
	.4byte 0x000A00FF
	.4byte 0x0000000A
	.4byte 0x000A00FF
	.4byte 0x002800A0
	.4byte 0x005000FF
	.4byte 0x00C800D2
	.4byte 0x003200FF
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
	.4byte 0x00AA0096
	.4byte 0x00D200FF
	.4byte 0xFFF6FF9C
	.4byte 0x000000FF
	.4byte 0xFF38FFC4
	.4byte 0xFFEC00FF
sMareW_HatDColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareW_HatDColorBuf0
  .4byte sMareW_HatDColorBuf1
"@1594":
	.asciz "mareWhatW_D.bmd"
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
	.4byte 0x00A00014
	.4byte 0x002800FF
	.4byte 0x00000064
	.4byte 0x00AA00FF
	.4byte 0x00500014
	.4byte 0xFFBA00FF
	.4byte 0x00140078
	.4byte 0x010400FF
	.4byte 0x00C800D2
	.4byte 0xFFE200FF
	.4byte 0x00DC00D2
	.4byte 0x00A000FF
sMareW_ShellAColorBuf1:
	.4byte 0x00FF00C8
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
	.4byte 0x00A50091
	.4byte 0x00B900FF
	.4byte 0x00F000FA
	.4byte 0x00F000FF
	.4byte 0x00500050
	.4byte 0x00A000FF
	.4byte 0x01680154
	.4byte 0x006400FF
sMareW_ShellAColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareW_ShellAColorBuf0
  .4byte sMareW_ShellAColorBuf1
"@1595":
	.asciz "mareWkai_A.bmd"
	.balign 4
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
	.4byte 0xFF9CFFCE
	.4byte 0xFFF600FF
	.4byte 0xFFCEFFD8
	.4byte 0xFF8800FF
	.4byte 0xFF4CFF2E
	.4byte 0x000A00FF
	.4byte 0x0082005A
	.4byte 0xFFD800FF
	.4byte 0x00E600FA
	.4byte 0x008200FF
	.4byte 0x00C8003C
	.4byte 0x007800FF
	.4byte 0x0032001E
	.4byte 0x001400FF
	.4byte 0x00D2005A
	.4byte 0x001E00FF
	.4byte 0x0000007D
	.4byte 0x003200FF
sMareW_ShellBColorBuf1:
	.4byte 0x000A0014
	.4byte 0x008200FF
	.4byte 0x00000046
	.4byte 0x000000FF
	.4byte 0x00F000F0
	.4byte 0x00BE00FF
	.4byte 0xFF1A0000
	.4byte 0x002800FF
	.4byte 0x00A00064
	.4byte 0x000000FF
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
	.4byte 0x00BEFFBA
	.4byte 0x00C800FF
	.4byte 0xFFF6FFBF
	.4byte 0x000000FF
	.4byte 0x00A000F0
	.4byte 0x008200FF
sMareW_ShellBColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareW_ShellBColorBuf0
  .4byte sMareW_ShellBColorBuf1
"@1596":
	.asciz "mareWkai_B.bmd"
	.balign 4
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
	.4byte 0x00640000
	.4byte 0x001E00FF
	.4byte 0x001E001E
	.4byte 0x007800FF
	.4byte 0xFFC4FF9C
	.4byte 0xFFD800FF
	.4byte 0xFFF6008C
	.4byte 0x003C00FF
	.4byte 0xFF380028
	.4byte 0xFF9C00FF
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
sMareW_BivalveAColorBuf1:
	.4byte 0xFFCE0014
	.4byte 0x003C00FF
	.4byte 0x00280078
	.4byte 0x00C800FF
	.4byte 0x0050001E
	.4byte 0x008C00FF
	.4byte 0x00DC000A
	.4byte 0x000A00FF
	.4byte 0x0000000A
	.4byte 0x000A00FF
	.4byte 0x002800A0
	.4byte 0x005000FF
sMareW_BivalveAColorChange:
  .4byte 0x2
  .4byte "@1573"
  .4byte sMareW_BivalveAColorBuf0
  .4byte sMareW_BivalveAColorBuf1
"@1597":
	.asciz "mareWbivalveW_A.bmd"
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
	.asciz "_baby_hat"
	.balign 4
sMareWB_BabyHatColorChange:
  .4byte 0x2
  .4byte "@1599"
  .4byte sMareWB_BabyHatColorBuf0
  .4byte sMareWB_BabyHatColorBuf1
"@1600":
	.asciz "_baby_bed"
	.balign 4
sMareWB_BabyBedColorChange:
  .4byte 0x2
  .4byte "@1600"
  .4byte sMareWB_BabyBedColorBuf0
  .4byte sMareWB_BabyBedColorBuf1
"@1601":
	.asciz "mareWBbaby.bmd"
	.balign 4
"@1602":
	.asciz "kinopio_sunmegane.bmd"
	.balign 4
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
	.asciz "jnt_rfinger_1"
	.balign 4
"@1604":
	.asciz "kinoji_stick.bmd"
	.balign 4
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
	.asciz "peach_hair_normal.bmd"
	.balign 4
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
	.asciz "jnt_hand_L"
	.balign 4
"@1607":
	.asciz "peach_hand1_l.bmd"
	.balign 4
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
	.asciz "jnt_hand_R"
	.balign 4
"@1609":
	.asciz "peach_hand1_r.bmd"
	.balign 4
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
	.asciz "peach_hair_ponytail.bmd"
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
	.asciz "peach_parasol.bmd"
	.balign 4
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
	.asciz "peach_hand2_l.bmd"
	.balign 4
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
	.asciz "peach_hand2_r.bmd"
	.balign 4
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
	.4byte 0x009B0000
	.4byte 0x000000FF
	.4byte 0x000A001E
	.4byte 0x009600FF
sRaccoonDog_TubeColorBuf1:
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
	.4byte 0x00F000DC
	.4byte 0x00C800FF
sRaccoonDog_TubeColorChange:
  .4byte 0x2
  .4byte "@1614"
  .4byte sRaccoonDog_TubeColorBuf0
  .4byte sRaccoonDog_TubeColorBuf1
"@1615":
	.asciz "ukiwa_null"
	.balign 4
"@1616":
	.asciz "ukiwa.bmd"
	.balign 4
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
	.4byte 0x006400FF
	.4byte 0x012C00FF
	.4byte 0x00780078
	.4byte 0x012C00FF
	.4byte 0x015E012C
	.4byte 0x000000FF
	.4byte 0x00C80046
	.4byte 0x000000FF
	.4byte 0x012C0082
	.4byte 0x00FF00FF
	.4byte 0x00FF015E
	.4byte 0x000000FF
	.4byte 0x019000FF
	.4byte 0x00FF00FF
	.4byte 0x0140008C
	.4byte 0x000000FF
	.4byte 0x00C800FF
	.4byte 0x019000FF
	.4byte 0x019000FA
	.4byte 0x006400FF
"@1617":
	.asciz "_hand_mat"
	.balign 4
sMonteM_BodyColor:
  .4byte 0x1
  .4byte "@1617"
  .4byte sMonteM_BodyColorBuf
  .4byte 0
sMonteMA_ClothColorBuf0:
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
	.4byte 0x00C800C8
	.4byte 0x00AA00FF
	.4byte 0x00320032
	.4byte 0x003200FF
	.4byte 0x009600C8
	.4byte 0x00FF00FF
	.4byte 0x00000046
	.4byte 0x009600FF
	.4byte 0x0190012C
	.4byte 0x00C800FF
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
	.4byte 0x009600FF
sMonteMA_ClothColorBuf1:
	.4byte 0x00FA0082
	.4byte 0x003200FF
	.4byte 0x00320082
	.4byte 0x006400FF
	.4byte 0x009600B4
	.4byte 0x001400FF
	.4byte 0x00C800C8
	.4byte 0x00AA00FF
	.4byte 0x00320032
	.4byte 0x003200FF
	.4byte 0x009600C8
	.4byte 0x00FF00FF
	.4byte 0x00000046
	.4byte 0x009600FF
	.4byte 0x00E60096
	.4byte 0x006400FF
	.4byte 0x003C0096
	.4byte 0x00E600FF
	.4byte 0x00B40096
	.4byte 0x00C800FF
	.4byte 0x006400DC
	.4byte 0x012C00FF
"@1618":
	.asciz "_fuku_mat"
	.balign 4
sMonteMA_ClothColor:
  .4byte 0x2
  .4byte "@1618"
  .4byte sMonteMA_ClothColorBuf0
  .4byte sMonteMA_ClothColorBuf1
sMonteMB_BodyColorBuf:
	.4byte 0x00A000C8
	.4byte 0x012C00FF
	.4byte 0x00FF00A0
	.4byte 0x009600FF
	.4byte 0x012C00C8
	.4byte 0x005000FF
	.4byte 0x00C8012C
	.4byte 0x006400FF
sMonteMB_BodyColor:
  .4byte 0x1
  .4byte "@1617"
  .4byte sMonteMB_BodyColorBuf
  .4byte 0
sMonteMB_ClothColorBuf:
	.4byte 0x00460082
	.4byte 0x00C800FF
	.4byte 0x00C80014
	.4byte 0x001400FF
	.4byte 0x0082001E
	.4byte 0x005000FF
	.4byte 0x008200C8
	.4byte 0x005000FF
	.4byte 0x00E600C8
	.4byte 0x005000FF
	.4byte 0x00320064
	.4byte 0x009600FF
sMonteMB_ClothColor:
  .4byte 0x1
  .4byte "@1618"
  .4byte sMonteMB_ClothColorBuf
  .4byte 0
sMonteMC_ClothColorBuf0:
	.4byte 0x00E600E6
	.4byte 0x00D200FF
	.4byte 0x00960046
	.4byte 0x000000FF
	.4byte 0x00E600E6
	.4byte 0x00D200FF
	.4byte 0x00000046
	.4byte 0x009600FF
	.4byte 0x00320096
	.4byte 0x008200FF
	.4byte 0x003C0028
	.4byte 0x000000FF
	.4byte 0x00000064
	.4byte 0x006400FF
	.4byte 0x00000096
	.4byte 0x00C800FF
	.4byte 0x00000032
	.4byte 0x006400FF
	.4byte 0x00640064
	.4byte 0x000000FF
	.4byte 0x00640000
	.4byte 0x000000FF
sMonteMC_ClothColorBuf1:
	.4byte 0x00E600E6
	.4byte 0x00D200FF
	.4byte 0x00960046
	.4byte 0x000000FF
	.4byte 0x00000046
	.4byte 0x009600FF
	.4byte 0x00E600E6
	.4byte 0x00D200FF
	.4byte 0x00E600E6
	.4byte 0x00D200FF
	.4byte 0x00A00096
	.4byte 0x003C00FF
	.4byte 0x00000064
	.4byte 0x006400FF
	.4byte 0x00000096
	.4byte 0x00C800FF
	.4byte 0x00000032
	.4byte 0x006400FF
	.4byte 0
	.4byte 0x000000FF
	.4byte 0
	.4byte 0x000000FF
sMonteMC_ClothColor:
  .4byte 0x2
  .4byte "@1618"
  .4byte sMonteMC_ClothColorBuf0
  .4byte sMonteMC_ClothColorBuf1
sMonteMD_ClothColorBuf:
	.4byte 0x015E0168
	.4byte 0x015400FF
	.4byte 0x00320064
	.4byte 0x000000FF
	.4byte 0x00960000
	.4byte 0x000000FF
	.4byte 0x0000012C
	.4byte 0x015E00FF
	.4byte 0x00000064
	.4byte 0x00FA00FF
sMonteMD_ClothColor:
  .4byte 0x1
  .4byte "@1618"
  .4byte sMonteMD_ClothColorBuf
  .4byte 0
sMonteW_ColorBuf:
	.4byte 0x012C0064
	.4byte 0x00C800FF
	.4byte 0x01900096
	.4byte 0x000000FF
	.4byte 0x012C014A
	.4byte 0x000000FF
	.4byte 0x0190014A
	.4byte 0x000000FF
	.4byte 0x014A0028
	.4byte 0x000000FF
	.4byte 0x019000C8
	.4byte 0x00FF00FF
sMonteW_BodyColor:
  .4byte 0x1
  .4byte "@1617"
  .4byte sMonteW_ColorBuf
  .4byte 0
sMonteWA_ClothColorBuf:
	.4byte 0x017C014A
	.4byte 0x009600FF
	.4byte 0x012C0064
	.4byte 0x00C800FF
	.4byte 0x0168015E
	.4byte 0x012C00FF
	.4byte 0x012C0032
	.4byte 0x000000FF
	.4byte 0x01900096
	.4byte 0x006400FF
	.4byte 0x00780096
	.4byte 0x012C00FF
sMonteWA_ClothColor:
  .4byte 0x1
  .4byte "@1618"
  .4byte sMonteWA_ClothColorBuf
  .4byte 0
sMonteWB_ClothColorBuf0:
	.4byte 0x00DC00C8
	.4byte 0x00DC00FF
	.4byte 0x00C800DC
	.4byte 0x00DC00FF
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
	.4byte 0x00DC00E6
	.4byte 0x00DC00FF
	.4byte 0x00B40064
	.4byte 0x006E00FF
	.4byte 0x00C80064
	.4byte 0x000000FF
	.4byte 0x00000064
	.4byte 0x009600FF
	.4byte 0x00FF00C8
	.4byte 0x006400FF
sMonteWB_ClothColorBuf1:
	.4byte 0x00640050
	.4byte 0x00C800FF
	.4byte 0x006400AA
	.4byte 0x00C800FF
	.4byte 0x00960000
	.4byte 0x003C00FF
	.4byte 0x00B40078
	.4byte 0x00C800FF
	.4byte 0x008C00B4
	.4byte 0x012C00FF
	.4byte 0x00B40064
	.4byte 0x006E00FF
	.4byte 0x00C80064
	.4byte 0x000000FF
	.4byte 0x00000064
	.4byte 0x009600FF
	.4byte 0x00FF00C8
	.4byte 0x006400FF
sMonteWB_ClothColor:
  .4byte 0x2
  .4byte "@1618"
  .4byte sMonteWB_ClothColorBuf0
  .4byte sMonteWB_ClothColorBuf1
sMareM_ColorBuf:
	.4byte 0x01720122
	.4byte 0x00AA00FF
	.4byte 0x015E0154
	.4byte 0x007800FF
	.4byte 0x010400B4
	.4byte 0x006400FF
	.4byte 0x012C0096
	.4byte 0x008200FF
	.4byte 0x012C00C8
	.4byte 0x003C00FF
	.4byte 0x00F000A0
	.4byte 0x00E600FF
sMareM_BodyColor:
  .4byte 0x1
  .4byte "@1598"
  .4byte sMareM_ColorBuf
  .4byte 0
sMareW_ColorBuf:
	.4byte 0x019A00FF
	.4byte 0x011800FF
	.4byte 0x01AE014A
	.4byte 0x009600FF
	.4byte 0x01B8014A
	.4byte 0x00FF00FF
	.4byte 0x012C00C8
	.4byte 0x008200FF
	.4byte 0x012C00BE
	.4byte 0x00DC00FF
	.4byte 0x019000FA
	.4byte 0x005A00FF
sMareW_BodyColor:
  .4byte 0x1
  .4byte "@1598"
  .4byte sMareW_ColorBuf
  .4byte 0
sKinopio_BodyColorData0:
	.4byte 0x001E001E
	.4byte 0x00C800FF
	.4byte 0x00E60000
	.4byte 0x000000FF
	.4byte 0x001E0078
	.4byte 0x001E00FF
	.4byte 0x00FA00DC
	.4byte 0x001E00FF
	.4byte 0x00960000
	.4byte 0x00C800FF
sKinopio_BodyColorData1:
	.4byte 0x00E600E6
	.4byte 0x00E600FF
	.4byte 0x00E600E6
	.4byte 0x00E600FF
	.4byte 0x00E600E6
	.4byte 0x00E600FF
	.4byte 0x00E600E6
	.4byte 0x00E600FF
	.4byte 0x00E600E6
	.4byte 0x00E600FF
"@1619":
	.asciz "_mat_cap"
	.balign 4
sKinopio_BodyColor:
  .4byte 0x2
  .4byte "@1619"
  .4byte sKinopio_BodyColorData0
  .4byte sKinopio_BodyColorData1
sKinopio_ClothColorData0:
	.4byte 0x00FA0096
	.4byte 0x000000FF
	.4byte 0x00FA0096
	.4byte 0x000000FF
	.4byte 0x00FA0096
	.4byte 0x000000FF
	.4byte 0x00FA0096
	.4byte 0x000000FF
	.4byte 0x00FA0096
	.4byte 0x000000FF
sKinopio_ClothColorData1:
	.4byte 0x001E001E
	.4byte 0x00C800FF
	.4byte 0x00E60000
	.4byte 0x000000FF
	.4byte 0x001E0078
	.4byte 0x001E00FF
	.4byte 0x00FA00DC
	.4byte 0x001E00FF
	.4byte 0x00960000
	.4byte 0x00C800FF
"@1620":
	.asciz "_mat_cloth"
	.balign 4
sKinopio_ClothColor:
  .4byte 0x2
  .4byte "@1620"
  .4byte sKinopio_ClothColorData0
  .4byte sKinopio_ClothColorData1
sRaccoonDog_ClothColorBuf0:
	.4byte 0
	.4byte 0x000000FF
	.4byte 0x010E0118
	.4byte 0x011300FF
sRaccoonDog_ClothColorBuf1:
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
	.4byte 0xFFEC0064
	.4byte 0xFFCE00FF
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
	.4byte 0
	.4byte 0
	.4byte 0x43480000
	.4byte 0x443B8000
	.4byte 0x43480000
	.4byte 0x443B8000
sSunflowerS_InitData:
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
	.4byte 0
	.4byte 0
	.4byte 0x42A00000
	.4byte 0x43960000
	.4byte 0x42A00000
	.4byte 0x43960000
sBoardNpc_InitData:
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
	.4byte 0
	.4byte 0
	.4byte 0x42A00000
	.4byte 0x43200000
	.4byte 0x42A00000
	.4byte 0x43200000

.section .data, "wa"  # 0x803A8380 - 0x803E6000
"@1431":
	.4byte 0x3F800000
	.4byte 0x3F800000
	.4byte 0x3F800000
"@1411":
	.4byte 0x3F800000
	.4byte 0x3F800000
	.4byte 0x3F800000
"@1210":
	.4byte 0
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x00000003
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
	.asciz "kubi"
	.balign 4
sMonteM_TieBColorBuf0:
	.4byte 0x00640000
	.4byte 0x000000FF
sMonteM_TieBColorBuf1:
	.4byte 0x00960082
	.4byte 0x000000FF
"@1573":
	.asciz "_mat1"
	.balign 4
"@1577":
	.asciz "koshi"
	.balign 4
sMareMB_ElderMat1ColorBuf0:
	.4byte 0xFF2E0028
	.4byte 0x006400FF
sMareMB_ElderMat1ColorBuf1:
	.4byte 0x00640046
	.4byte 0x00C800FF
sMareMB_ElderMat2ColorBuf0:
	.4byte 0x00AA00AA
	.4byte 0x00E600FF
sMareMB_ElderMat2ColorBuf1:
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
"@1583":
	.asciz "_mat2"
	.balign 4
sMareMB_ElderMat3ColorBuf0:
	.4byte 0x000A0014
	.4byte 0x001E00FF
sMareMB_ElderMat3ColorBuf1:
	.4byte 0x00F00064
	.4byte 0x001400FF
"@1584":
	.asciz "_mat3"
	.balign 4
sMareMB_FishingRodMat1ColorBuf0:
	.4byte 0x003CFF38
	.4byte 0xFF3800FF
sMareMB_FishingRodMat1ColorBuf1:
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
sMareMB_FishingRodMat2ColorBuf0:
	.4byte 0x00FA00B4
	.4byte 0x001E00FF
sMareMB_FishingRodMat2ColorBuf1:
	.4byte 0x00FF00FF
	.4byte 0x00FF00FF
sMareMB_FishingRodMat3ColorBuf0:
	.4byte 0x00460028
	.4byte 0x000000FF
sMareMB_FishingRodMat3ColorBuf1:
	.4byte 0x00FF00FF
	.4byte 0x008200FF
sMareMC_HatFColorBuf0:
	.4byte 0xFF56FF38
	.4byte 0x000000FF
sMareMC_HatFColorBuf1:
	.4byte 0x00780078
	.4byte 0x005000FF
sMareMC_BeardColorBuf0:
	.4byte 0xFF88FF92
	.4byte 0xFF6000FF
sMareMC_BeardColorBuf1:
	.4byte 0x00B4FFBA
	.4byte 0x017C00FF
sMareMC_MustacheColorBuf0:
	.4byte 0xFF88FF92
	.4byte 0xFF6000FF
sMareMC_MustacheColorBuf1:
	.4byte 0x00B4FFBA
	.4byte 0x017C00FF
sMareMD_PipeColorBuf0:
	.4byte 0x0000FF9C
	.4byte 0x003200FF
sMareMD_PipeColorBuf1:
	.4byte 0x019000FA
	.4byte 0x009600FF
"@1590":
	.asciz "migite"
	.balign 4
sMareWB_BabyBodyColorBuf0:
	.4byte 0x01B8014A
	.4byte 0x00FF00FF
"@1598":
	.asciz "_body"
	.balign 4
sMareWB_BabyHatColorBuf0:
	.4byte 0x00460000
	.4byte 0x000000FF
sMareWB_BabyHatColorBuf1:
	.4byte 0x00BE003C
	.4byte 0x001E00FF
sMareWB_BabyBedColorBuf0:
	.4byte 0x0046000A
	.4byte 0x000A00FF
sMareWB_BabyBedColorBuf1:
	.4byte 0x00C800B4
	.4byte 0x000A00FF
"@1614":
	.asciz "_ukiwa"
	.balign 4
"@1621":
	.asciz "_pants"
	.balign 4

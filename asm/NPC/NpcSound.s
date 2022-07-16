.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global getBasNameTable__8TBaseNPCCFv
getBasNameTable__8TBaseNPCCFv:
/* 8017731C 0017425C  7C 08 02 A6 */	mflr r0
/* 80177320 00174260  3C 80 80 3C */	lis r4, "@1431"@ha
/* 80177324 00174264  90 01 00 04 */	stw r0, 4(r1)
/* 80177328 00174268  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 8017732C 0017426C  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 80177330 00174270  93 C1 00 18 */	stw r30, 0x18(r1)
/* 80177334 00174274  3B C4 94 E8 */	addi r30, r4, "@1431"@l
/* 80177338 00174278  93 A1 00 14 */	stw r29, 0x14(r1)
/* 8017733C 0017427C  3B A0 00 00 */	li r29, 0
/* 80177340 00174280  93 81 00 10 */	stw r28, 0x10(r1)
/* 80177344 00174284  3B 83 00 00 */	addi r28, r3, 0
/* 80177348 00174288  4B FF 52 C5 */	bl isChild__8TBaseNPCCFv
/* 8017734C 0017428C  3B E3 00 00 */	addi r31, r3, 0
/* 80177350 00174290  38 7C 00 00 */	addi r3, r28, 0
/* 80177354 00174294  4B FF 54 5D */	bl isNormalMonteM__8TBaseNPCCFv
/* 80177358 00174298  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 8017735C 0017429C  41 82 00 1C */	beq lbl_80177378
/* 80177360 001742A0  57 E0 06 3F */	clrlwi. r0, r31, 0x18
/* 80177364 001742A4  41 82 00 0C */	beq lbl_80177370
/* 80177368 001742A8  3B BE 00 94 */	addi r29, r30, 0x94
/* 8017736C 001742AC  48 00 01 14 */	b lbl_80177480
lbl_80177370:
/* 80177370 001742B0  3B BE 00 38 */	addi r29, r30, 0x38
/* 80177374 001742B4  48 00 01 0C */	b lbl_80177480
lbl_80177378:
/* 80177378 001742B8  7F 83 E3 78 */	mr r3, r28
/* 8017737C 001742BC  4B FF 54 09 */	bl isNormalMonteW__8TBaseNPCCFv
/* 80177380 001742C0  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 80177384 001742C4  41 82 00 1C */	beq lbl_801773A0
/* 80177388 001742C8  57 E0 06 3F */	clrlwi. r0, r31, 0x18
/* 8017738C 001742CC  41 82 00 0C */	beq lbl_80177398
/* 80177390 001742D0  3B BE 01 C0 */	addi r29, r30, 0x1c0
/* 80177394 001742D4  48 00 00 EC */	b lbl_80177480
lbl_80177398:
/* 80177398 001742D8  3B BE 01 60 */	addi r29, r30, 0x160
/* 8017739C 001742DC  48 00 00 E4 */	b lbl_80177480
lbl_801773A0:
/* 801773A0 001742E0  80 7C 00 4C */	lwz r3, 0x4c(r28)
/* 801773A4 001742E4  3C 83 FC 00 */	addis r4, r3, 0xfc00
/* 801773A8 001742E8  38 84 FF FA */	addi r4, r4, -6
/* 801773AC 001742EC  28 04 00 17 */	cmplwi r4, 0x17
/* 801773B0 001742F0  41 81 00 D0 */	bgt lbl_80177480
/* 801773B4 001742F4  3C 60 80 3C */	lis r3, "@2235"@ha
/* 801773B8 001742F8  38 63 9B C8 */	addi r3, r3, "@2235"@l
/* 801773BC 001742FC  54 84 10 3A */	slwi r4, r4, 2
/* 801773C0 00174300  7C 03 20 2E */	lwzx r0, r3, r4
/* 801773C4 00174304  7C 09 03 A6 */	mtctr r0
/* 801773C8 00174308  4E 80 04 20 */	bctr 
lbl_801773CC:
/* 801773CC 0017430C  3B BE 00 F0 */	addi r29, r30, 0xf0
/* 801773D0 00174310  48 00 00 B0 */	b lbl_80177480
lbl_801773D4:
/* 801773D4 00174314  3B BE 01 28 */	addi r29, r30, 0x128
/* 801773D8 00174318  48 00 00 A8 */	b lbl_80177480
lbl_801773DC:
/* 801773DC 0017431C  3B AD 84 F0 */	addi r29, r13, monteMG_bastable@sda21
/* 801773E0 00174320  48 00 00 A0 */	b lbl_80177480
lbl_801773E4:
/* 801773E4 00174324  3B BE 01 40 */	addi r29, r30, 0x140
/* 801773E8 00174328  48 00 00 98 */	b lbl_80177480
lbl_801773EC:
/* 801773EC 0017432C  3B BE 02 20 */	addi r29, r30, 0x220
/* 801773F0 00174330  48 00 00 90 */	b lbl_80177480
lbl_801773F4:
/* 801773F4 00174334  57 E0 06 3F */	clrlwi. r0, r31, 0x18
/* 801773F8 00174338  41 82 00 0C */	beq lbl_80177404
/* 801773FC 0017433C  3B BE 02 9C */	addi r29, r30, 0x29c
/* 80177400 00174340  48 00 00 80 */	b lbl_80177480
lbl_80177404:
/* 80177404 00174344  3B BE 02 48 */	addi r29, r30, 0x248
/* 80177408 00174348  48 00 00 78 */	b lbl_80177480
lbl_8017740C:
/* 8017740C 0017434C  3B BE 02 F0 */	addi r29, r30, 0x2f0
/* 80177410 00174350  48 00 00 70 */	b lbl_80177480
lbl_80177414:
/* 80177414 00174354  3B BE 03 18 */	addi r29, r30, 0x318
/* 80177418 00174358  48 00 00 68 */	b lbl_80177480
lbl_8017741C:
/* 8017741C 0017435C  3B BE 03 48 */	addi r29, r30, 0x348
/* 80177420 00174360  48 00 00 60 */	b lbl_80177480
lbl_80177424:
/* 80177424 00174364  3B AD 84 F8 */	addi r29, r13, mareMD_bastable@sda21
/* 80177428 00174368  48 00 00 58 */	b lbl_80177480
lbl_8017742C:
/* 8017742C 0017436C  57 E0 06 3F */	clrlwi. r0, r31, 0x18
/* 80177430 00174370  41 82 00 0C */	beq lbl_8017743C
/* 80177434 00174374  3B BE 03 C0 */	addi r29, r30, 0x3c0
/* 80177438 00174378  48 00 00 48 */	b lbl_80177480
lbl_8017743C:
/* 8017743C 0017437C  3B BE 03 70 */	addi r29, r30, 0x370
/* 80177440 00174380  48 00 00 40 */	b lbl_80177480
lbl_80177444:
/* 80177444 00174384  3B BE 04 10 */	addi r29, r30, 0x410
/* 80177448 00174388  48 00 00 38 */	b lbl_80177480
lbl_8017744C:
/* 8017744C 0017438C  3B BE 04 30 */	addi r29, r30, 0x430
/* 80177450 00174390  48 00 00 30 */	b lbl_80177480
lbl_80177454:
/* 80177454 00174394  3B BE 04 60 */	addi r29, r30, 0x460
/* 80177458 00174398  48 00 00 28 */	b lbl_80177480
lbl_8017745C:
/* 8017745C 0017439C  3B BE 05 28 */	addi r29, r30, 0x528
/* 80177460 001743A0  48 00 00 20 */	b lbl_80177480
lbl_80177464:
/* 80177464 001743A4  3B BE 05 B8 */	addi r29, r30, 0x5b8
/* 80177468 001743A8  48 00 00 18 */	b lbl_80177480
lbl_8017746C:
/* 8017746C 001743AC  3B BE 06 70 */	addi r29, r30, 0x670
/* 80177470 001743B0  48 00 00 10 */	b lbl_80177480
lbl_80177474:
/* 80177474 001743B4  3B BE 06 90 */	addi r29, r30, 0x690
/* 80177478 001743B8  48 00 00 08 */	b lbl_80177480
lbl_8017747C:
/* 8017747C 001743BC  3B BE 06 B8 */	addi r29, r30, 0x6b8
lbl_80177480:
/* 80177480 001743C0  80 01 00 24 */	lwz r0, 0x24(r1)
/* 80177484 001743C4  7F A3 EB 78 */	mr r3, r29
/* 80177488 001743C8  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 8017748C 001743CC  83 C1 00 18 */	lwz r30, 0x18(r1)
/* 80177490 001743D0  7C 08 03 A6 */	mtlr r0
/* 80177494 001743D4  83 A1 00 14 */	lwz r29, 0x14(r1)
/* 80177498 001743D8  83 81 00 10 */	lwz r28, 0x10(r1)
/* 8017749C 001743DC  38 21 00 20 */	addi r1, r1, 0x20
/* 801774A0 001743E0  4E 80 00 20 */	blr 

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
"@1755":
	.4byte 0x4D416374
	.4byte 0x6F724D74
	.4byte 0x7843616C
	.4byte 0x63547970
	.4byte 0x655F4261
	.4byte 0x73696320
	.4byte 0x834E8389
	.4byte 0x83568362
	.4byte 0x834E8358
	.4byte 0x8350815B
	.4byte 0x838B826E
	.4byte 0x826D0000
"@1756":
	.4byte 0x4D416374
	.4byte 0x6F724D74
	.4byte 0x7843616C
	.4byte 0x63547970
	.4byte 0x655F536F
	.4byte 0x6674696D
	.4byte 0x61676520
	.4byte 0x834E8389
	.4byte 0x83568362
	.4byte 0x834E8358
	.4byte 0x8350815B
	.4byte 0x838B826E
	.4byte 0x82658265
	.4byte 0
"@1757":
	.4byte 0x4D416374
	.4byte 0x6F724D74
	.4byte 0x7843616C
	.4byte 0x63547970
	.4byte 0x655F4D6F
	.4byte 0x74696F6E
	.4byte 0x426C656E
	.4byte 0x64208382
	.4byte 0x815B8356
	.4byte 0x83878393
	.4byte 0x8375838C
	.4byte 0x83938368
	.4byte 0
"@1758":
	.4byte 0x4D416374
	.4byte 0x6F724D74
	.4byte 0x7843616C
	.4byte 0x63547970
	.4byte 0x655F5573
	.4byte 0x65722083
	.4byte 0x86815B83
	.4byte 0x55815B92
	.4byte 0xE88B6000
"@1938":
	.asciz "/scene/monteMCommon/bas/mom_appear.bas"
	.balign 4
"@1939":
	.asciz "/scene/monteMCommon/bas/mom_dance.bas"
	.balign 4
"@1940":
	.asciz "/scene/monteMCommon/bas/mom_fall.bas"
	.balign 4
"@1941":
	.asciz "/scene/monteMCommon/bas/mom_happy.bas"
	.balign 4
"@1942":
	.asciz "/scene/monteMCommon/bas/mom_mad.bas"
"@1943":
	.asciz "/scene/monteMCommon/bas/mom_madloop.bas"
"@1944":
	.asciz "/scene/monteMCommon/bas/mom_recover.bas"
"@1945":
	.asciz "/scene/monteMCommon/bas/mom_run.bas"
"@1946":
	.asciz "/scene/monteMCommon/bas/mom_sitwet.bas"
	.balign 4
"@1947":
	.asciz "/scene/monteMCommon/bas/mom_throw.bas"
	.balign 4
"@1948":
	.asciz "/scene/monteMCommon/bas/mom_wait_yogore.bas"
"@1949":
	.asciz "/scene/monteMCommon/bas/mom_walk.bas"
	.balign 4
"@1950":
	.asciz "/scene/monteMCommon/bas/mom_wet.bas"
"@1951":
	.asciz "/scene/monteMCommon/bas/mom_wet_A.bas"
	.balign 4
"@1952":
	.asciz "/scene/monteMCommon/bas/mom_wet_yogore.bas"
	.balign 4
"@1953":
	.asciz "/scene/monteMCommon/bas/mom_appear_c.bas"
	.balign 4
"@1954":
	.asciz "/scene/monteMCommon/bas/mom_dance_c.bas"
"@1955":
	.asciz "/scene/monteMCommon/bas/mom_fall_c.bas"
	.balign 4
"@1956":
	.asciz "/scene/monteMCommon/bas/mom_happy_c.bas"
"@1957":
	.asciz "/scene/monteMCommon/bas/mom_mad_c.bas"
	.balign 4
"@1958":
	.asciz "/scene/monteMCommon/bas/mom_madloop_c.bas"
	.balign 4
"@1959":
	.asciz "/scene/monteMCommon/bas/mom_recover_c.bas"
	.balign 4
"@1960":
	.asciz "/scene/monteMCommon/bas/mom_run_c.bas"
	.balign 4
"@1961":
	.asciz "/scene/monteMCommon/bas/mom_sitwet_c.bas"
	.balign 4
"@1962":
	.asciz "/scene/monteMCommon/bas/mom_throw_c.bas"
"@1963":
	.asciz "/scene/monteMCommon/bas/mom_wait_yogore_c.bas"
	.balign 4
"@1964":
	.asciz "/scene/monteMCommon/bas/mom_walk_c.bas"
	.balign 4
"@1965":
	.asciz "/scene/monteMCommon/bas/mom_wet_c.bas"
	.balign 4
"@1966":
	.asciz "/scene/monteMCommon/bas/mom_wet_A_c.bas"
"@1967":
	.asciz "/scene/monteMCommon/bas/mom_wet_yogore_c.bas"
	.balign 4
"@1970":
	.asciz "/scene/monteME/bas/momE_down.bas"
	.balign 4
"@1971":
	.asciz "/scene/monteME/bas/momE_up.bas"
	.balign 4
"@1972":
	.asciz "/scene/monteME/bas/momE_wet.bas"
"@1975":
	.asciz "/scene/monteMF/bas/momF_swim.bas"
	.balign 4
"@1976":
	.asciz "/scene/monteMF/bas/momF_swimmad.bas"
"@1977":
	.asciz "/scene/monteMF/bas/momF_swimtalk.bas"
	.balign 4
"@1980":
	.asciz "/scene/monteMG/bas/momG_wait_cleanup.bas"
	.balign 4
"@1981":
	.asciz "/scene/monteMG/bas/momG_walk_cleanup.bas"
	.balign 4
"@1984":
	.asciz "/scene/monteMH/bas/momH_play.bas"
	.balign 4
"@1985":
	.asciz "/scene/monteMH/bas/momH_walk_play.bas"
	.balign 4
"@1986":
	.asciz "/scene/monteMH/bas/momH_wet_play.bas"
	.balign 4
"@1989":
	.asciz "/scene/monteWCommon/bas/mow_appear.bas"
	.balign 4
"@1990":
	.asciz "/scene/monteWCommon/bas/mow_dance.bas"
	.balign 4
"@1991":
	.asciz "/scene/monteWCommon/bas/mow_fall.bas"
	.balign 4
"@1992":
	.asciz "/scene/monteWCommon/bas/mow_happy.bas"
	.balign 4
"@1993":
	.asciz "/scene/monteWCommon/bas/mow_mad.bas"
"@1994":
	.asciz "/scene/monteWCommon/bas/mow_madloop.bas"
"@1995":
	.asciz "/scene/monteWCommon/bas/mow_run.bas"
"@1996":
	.asciz "/scene/monteWCommon/bas/mow_sitwet.bas"
	.balign 4
"@1997":
	.asciz "/scene/monteWCommon/bas/mow_throw.bas"
	.balign 4
"@1998":
	.asciz "/scene/monteWCommon/bas/mow_wait_arrow.bas"
	.balign 4
"@1999":
	.asciz "/scene/monteWCommon/bas/mow_wait_yogore.bas"
"@2000":
	.asciz "/scene/monteWCommon/bas/mow_walk.bas"
	.balign 4
"@2001":
	.asciz "/scene/monteWCommon/bas/mow_wet.bas"
"@2002":
	.asciz "/scene/monteWCommon/bas/mow_wet_A.bas"
	.balign 4
"@2003":
	.asciz "/scene/monteWCommon/bas/mow_wet_yogore.bas"
	.balign 4
"@2004":
	.asciz "/scene/monteWCommon/bas/mow_appear_c.bas"
	.balign 4
"@2005":
	.asciz "/scene/monteWCommon/bas/mow_dance_c.bas"
"@2006":
	.asciz "/scene/monteWCommon/bas/mow_fall_c.bas"
	.balign 4
"@2007":
	.asciz "/scene/monteWCommon/bas/mow_happy_c.bas"
"@2008":
	.asciz "/scene/monteWCommon/bas/mow_mad_c.bas"
	.balign 4
"@2009":
	.asciz "/scene/monteWCommon/bas/mow_madloop_c.bas"
	.balign 4
"@2010":
	.asciz "/scene/monteWCommon/bas/mow_run_c.bas"
	.balign 4
"@2011":
	.asciz "/scene/monteWCommon/bas/mow_sitwet_c.bas"
	.balign 4
"@2012":
	.asciz "/scene/monteWCommon/bas/mow_throw_c.bas"
"@2013":
	.asciz "/scene/monteWCommon/bas/mow_wait_arrow_c.bas"
	.balign 4
"@2014":
	.asciz "/scene/monteWCommon/bas/mow_wait_yogore_c.bas"
	.balign 4
"@2015":
	.asciz "/scene/monteWCommon/bas/mow_walk_c.bas"
	.balign 4
"@2016":
	.asciz "/scene/monteWCommon/bas/mow_wet_c.bas"
	.balign 4
"@2017":
	.asciz "/scene/monteWCommon/bas/mow_wet_A_c.bas"
"@2018":
	.asciz "/scene/monteWCommon/bas/mow_wet_yogore_c.bas"
	.balign 4
"@2021":
	.asciz "/scene/monteWC/bas/mowC_mad.bas"
"@2022":
	.asciz "/scene/monteWC/bas/mowC_wet.bas"
"@2023":
	.asciz "/scene/monteWC/bas/mowC_wet_A.bas"
	.balign 4
"@2026":
	.asciz "/scene/mareM/bas/mareM_appear.bas"
	.balign 4
"@2027":
	.asciz "/scene/mareM/bas/mareM_dance.bas"
	.balign 4
"@2028":
	.asciz "/scene/mareM/bas/mareM_fall.bas"
"@2029":
	.asciz "/scene/mareM/bas/mareM_happy.bas"
	.balign 4
"@2030":
	.asciz "/scene/mareM/bas/mareM_jump.bas"
"@2031":
	.asciz "/scene/mareM/bas/mareM_recover.bas"
	.balign 4
"@2032":
	.asciz "/scene/mareM/bas/mareM_run.bas"
	.balign 4
"@2033":
	.asciz "/scene/mareM/bas/mareM_stand.bas"
	.balign 4
"@2034":
	.asciz "/scene/mareM/bas/mareM_stand_A.bas"
	.balign 4
"@2035":
	.asciz "/scene/mareM/bas/mareM_wait_yogore.bas"
	.balign 4
"@2036":
	.asciz "/scene/mareM/bas/mareM_walk.bas"
"@2037":
	.asciz "/scene/mareM/bas/mareM_wash.bas"
"@2038":
	.asciz "/scene/mareM/bas/mareM_wet.bas"
	.balign 4
"@2039":
	.asciz "/scene/mareM/bas/mareM_wet_A.bas"
	.balign 4
"@2040":
	.asciz "/scene/mareM/bas/mareM_appear_c.bas"
"@2041":
	.asciz "/scene/mareM/bas/mareM_dance_c.bas"
	.balign 4
"@2042":
	.asciz "/scene/mareM/bas/mareM_fall_c.bas"
	.balign 4
"@2043":
	.asciz "/scene/mareM/bas/mareM_happy_c.bas"
	.balign 4
"@2044":
	.asciz "/scene/mareM/bas/mareM_jump_c.bas"
	.balign 4
"@2045":
	.asciz "/scene/mareM/bas/mareM_recover_c.bas"
	.balign 4
"@2046":
	.asciz "/scene/mareM/bas/mareM_run_c.bas"
	.balign 4
"@2047":
	.asciz "/scene/mareM/bas/mareM_stand_c.bas"
	.balign 4
"@2048":
	.asciz "/scene/mareM/bas/mareM_stand_A_c.bas"
	.balign 4
"@2049":
	.asciz "/scene/mareM/bas/mareM_walk_c.bas"
	.balign 4
"@2050":
	.asciz "/scene/mareM/bas/mareM_wash_c.bas"
	.balign 4
"@2051":
	.asciz "/scene/mareM/bas/mareM_wet_c.bas"
	.balign 4
"@2052":
	.asciz "/scene/mareM/bas/mareM_wet_A_c.bas"
	.balign 4
"@2057":
	.asciz "/scene/mareMB/bas/mareMB_wet.bas"
	.balign 4
"@2059":
	.asciz "/scene/mareMC/bas/mareMC_stand.bas"
	.balign 4
"@2060":
	.asciz "/scene/mareMC/bas/mareMC_stand_A.bas"
	.balign 4
"@2061":
	.asciz "/scene/mareMC/bas/mareMC_wet.bas"
	.balign 4
"@2062":
	.asciz "/scene/mareMC/bas/mareMC_wet_A.bas"
	.balign 4
"@2065":
	.asciz "/scene/mareMD/bas/mareMD_hue.bas"
	.balign 4
"@2066":
	.asciz "/scene/mareMD/bas/mareMD_wet.bas"
	.balign 4
"@2069":
	.asciz "/scene/mareW/bas/mareW_appear.bas"
	.balign 4
"@2070":
	.asciz "/scene/mareW/bas/mareW_dance.bas"
	.balign 4
"@2071":
	.asciz "/scene/mareW/bas/mareW_fall.bas"
"@2072":
	.asciz "/scene/mareW/bas/mareW_happy.bas"
	.balign 4
"@2073":
	.asciz "/scene/mareW/bas/mareW_recover.bas"
	.balign 4
"@2074":
	.asciz "/scene/mareW/bas/mareW_run.bas"
	.balign 4
"@2075":
	.asciz "/scene/mareW/bas/mareW_stand.bas"
	.balign 4
"@2076":
	.asciz "/scene/mareW/bas/mareW_stand_A.bas"
	.balign 4
"@2077":
	.asciz "/scene/mareW/bas/mareW_wait_yogore.bas"
	.balign 4
"@2078":
	.asciz "/scene/mareW/bas/mareW_walk.bas"
"@2079":
	.asciz "/scene/mareW/bas/mareW_wash.bas"
"@2080":
	.asciz "/scene/mareW/bas/mareW_wet.bas"
	.balign 4
"@2081":
	.asciz "/scene/mareW/bas/mareW_wet_A.bas"
	.balign 4
"@2082":
	.asciz "/scene/mareW/bas/mareW_appear_c.bas"
"@2083":
	.asciz "/scene/mareW/bas/mareW_dance_c.bas"
	.balign 4
"@2084":
	.asciz "/scene/mareW/bas/mareW_fall_c.bas"
	.balign 4
"@2085":
	.asciz "/scene/mareW/bas/mareW_happy_c.bas"
	.balign 4
"@2086":
	.asciz "/scene/mareW/bas/mareW_recover_c.bas"
	.balign 4
"@2087":
	.asciz "/scene/mareW/bas/mareW_run_c.bas"
	.balign 4
"@2088":
	.asciz "/scene/mareW/bas/mareW_stand_c.bas"
	.balign 4
"@2089":
	.asciz "/scene/mareW/bas/mareW_stand_A_c.bas"
	.balign 4
"@2090":
	.asciz "/scene/mareW/bas/mareW_walk_c.bas"
	.balign 4
"@2091":
	.asciz "/scene/mareW/bas/mareW_wash_c.bas"
	.balign 4
"@2092":
	.asciz "/scene/mareW/bas/mareW_wet_c.bas"
	.balign 4
"@2093":
	.asciz "/scene/mareW/bas/mareW_wet_A_c.bas"
	.balign 4
"@2098":
	.asciz "/scene/mareWB/bas/mareWBbaby_hold.bas"
	.balign 4
"@2099":
	.asciz "/scene/mareWB/bas/mareWBbaby_holdTalk.bas"
	.balign 4
"@2100":
	.asciz "/scene/mareWB/bas/mareWBbaby_holdWet.bas"
	.balign 4
"@2101":
	.asciz "/scene/mareWB/bas/mareWB_holdTalk.bas"
	.balign 4
"@2102":
	.asciz "/scene/mareWB/bas/mareWB_holdWet.bas"
	.balign 4
"@2105":
	.asciz "/scene/kinopio/bas/kinopio_appear.bas"
	.balign 4
"@2106":
	.asciz "/scene/kinopio/bas/kinopio_fall.bas"
"@2107":
	.asciz "/scene/kinopio/bas/kinopio_happy.bas"
	.balign 4
"@2108":
	.asciz "/scene/kinopio/bas/kinopio_recover.bas"
	.balign 4
"@2109":
	.asciz "/scene/kinopio/bas/kinopio_run.bas"
	.balign 4
"@2110":
	.asciz "/scene/kinopio/bas/kinopio_sitshake.bas"
"@2111":
	.asciz "/scene/kinopio/bas/kinopio_sitwet.bas"
	.balign 4
"@2112":
	.asciz "/scene/kinopio/bas/kinopio_stumble.bas"
	.balign 4
"@2113":
	.asciz "/scene/kinopio/bas/kinopio_wait_yogore.bas"
	.balign 4
"@2114":
	.asciz "/scene/kinopio/bas/kinopio_walk.bas"
"@2115":
	.asciz "/scene/kinopio/bas/kinopio_wet.bas"
	.balign 4
"@2116":
	.asciz "/scene/kinopio/bas/kinopio_wet_A.bas"
	.balign 4
"@2117":
	.asciz "/scene/kinopio/bas/kinopio_wet_yogore.bas"
	.balign 4
"@2120":
	.asciz "/scene/kinojii/bas/kinoji_fall.bas"
	.balign 4
"@2121":
	.asciz "/scene/kinojii/bas/kinoji_recover.bas"
	.balign 4
"@2122":
	.asciz "/scene/kinojii/bas/kinoji_sitshake.bas"
	.balign 4
"@2123":
	.asciz "/scene/kinojii/bas/kinoji_sitwet.bas"
	.balign 4
"@2124":
	.asciz "/scene/kinojii/bas/kinoji_stumble.bas"
	.balign 4
"@2125":
	.asciz "/scene/kinojii/bas/kinoji_walk.bas"
	.balign 4
"@2126":
	.asciz "/scene/kinojii/bas/kinoji_wet.bas"
	.balign 4
"@2127":
	.asciz "/scene/kinojii/bas/kinoji_wet_A.bas"
"@2130":
	.asciz "/scene/peach/bas/peach_anger_wait.bas"
	.balign 4
"@2131":
	.asciz "/scene/peach/bas/peach_carry.bas"
	.balign 4
"@2132":
	.asciz "/scene/peach/bas/peach_fear_wait.bas"
	.balign 4
"@2133":
	.asciz "/scene/peach/bas/peach_parasol_wet.bas"
	.balign 4
"@2134":
	.asciz "/scene/peach/bas/peach_recover.bas"
	.balign 4
"@2135":
	.asciz "/scene/peach/bas/peach_tired_wait.bas"
	.balign 4
"@2136":
	.asciz "/scene/peach/bas/peach_wet.bas"
	.balign 4
"@2139":
	.asciz "/scene/raccoonDog/bas/tanuki_wait_A.bas"
"@2140":
	.asciz "/scene/raccoonDog/bas/tanuki_wait_B.bas"
"@2141":
	.asciz "/scene/raccoonDog/bas/tanuki_wet.bas"
	.balign 4
"@2144":
	.asciz "/scene/sunflowerL/bas/sunflower_stand.bas"
	.balign 4
"@2145":
	.asciz "/scene/sunflowerL/bas/sunflower_wet.bas"
"@2146":
	.asciz "/scene/sunflowerL/bas/sunflower_wet_B.bas"
	.balign 4
"@2150":
	.asciz "/scene/sunflowerS/bas/sunflower_s_stand.bas"
"@2151":
	.asciz "/scene/sunflowerS/bas/sunflower_s_wet.bas"
	.balign 4
"@2152":
	.asciz "/scene/sunflowerS/bas/sunflower_s_wet_B.bas"
	.4byte 0

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
MtxCalcTypeName:
  .4byte "@1755"
  .4byte "@1756"
  .4byte "@1757"
  .4byte "@1758"
monteMCommon_bastable:
  .4byte "@1938"
  .4byte "@1939"
  .4byte "@1940"
  .4byte "@1941"
  .4byte "@1942"
  .4byte "@1943"
  .4byte "@1944"
  .4byte "@1945"
  .4byte 0
  .4byte 0
  .4byte "@1946"
  .4byte 0
  .4byte 0
  .4byte "@1947"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte "@1948"
  .4byte "@1949"
  .4byte "@1950"
  .4byte "@1951"
  .4byte "@1952"
monteMCommon_bas_c_table:
  .4byte "@1953"
  .4byte "@1954"
  .4byte "@1955"
  .4byte "@1956"
  .4byte "@1957"
  .4byte "@1958"
  .4byte "@1959"
  .4byte "@1960"
  .4byte 0
  .4byte 0
  .4byte "@1961"
  .4byte 0
  .4byte 0
  .4byte "@1962"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte "@1963"
  .4byte "@1964"
  .4byte "@1965"
  .4byte "@1966"
  .4byte "@1967"
monteME_bastable:
  .4byte "@1970"
  .4byte 0
  .4byte 0
  .4byte "@1971"
  .4byte 0
  .4byte 0
  .4byte "@1972"
monteME_bas_c_table:
  .4byte "@1970"
  .4byte 0
  .4byte 0
  .4byte "@1971"
  .4byte 0
  .4byte 0
  .4byte "@1972"
monteMF_bastable:
  .4byte "@1975"
  .4byte "@1976"
  .4byte "@1977"
monteMF_bas_c_table:
  .4byte "@1975"
  .4byte "@1976"
  .4byte "@1977"
monteMH_bastable:
  .4byte "@1984"
  .4byte 0
  .4byte "@1985"
  .4byte "@1986"
monteMH_bas_c_table:
  .4byte "@1984"
  .4byte 0
  .4byte "@1985"
  .4byte "@1986"
monteWCommon_bastable:
  .4byte "@1989"
  .4byte "@1990"
  .4byte "@1991"
  .4byte "@1992"
  .4byte "@1993"
  .4byte "@1994"
  .4byte 0
  .4byte "@1995"
  .4byte 0
  .4byte 0
  .4byte "@1996"
  .4byte 0
  .4byte 0
  .4byte "@1997"
  .4byte 0
  .4byte 0
  .4byte "@1998"
  .4byte 0
  .4byte 0
  .4byte "@1999"
  .4byte "@2000"
  .4byte "@2001"
  .4byte "@2002"
  .4byte "@2003"
monteWCommon_bas_c_table:
  .4byte "@2004"
  .4byte "@2005"
  .4byte "@2006"
  .4byte "@2007"
  .4byte "@2008"
  .4byte "@2009"
  .4byte 0
  .4byte "@2010"
  .4byte 0
  .4byte 0
  .4byte "@2011"
  .4byte 0
  .4byte 0
  .4byte "@2012"
  .4byte 0
  .4byte 0
  .4byte "@2013"
  .4byte 0
  .4byte 0
  .4byte "@2014"
  .4byte "@2015"
  .4byte "@2016"
  .4byte "@2017"
  .4byte "@2018"
monteWC_bastable:
  .4byte 0
  .4byte "@2021"
  .4byte 0
  .4byte "@2022"
  .4byte "@2023"
monteWC_bas_c_table:
  .4byte 0
  .4byte "@2021"
  .4byte 0
  .4byte "@2022"
  .4byte "@2023"
mareM_bastable:
  .4byte "@2026"
  .4byte "@2027"
  .4byte "@2028"
  .4byte "@2029"
  .4byte "@2030"
  .4byte "@2031"
  .4byte "@2032"
  .4byte 0
  .4byte 0
  .4byte "@2033"
  .4byte "@2034"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte "@2035"
  .4byte "@2036"
  .4byte "@2037"
  .4byte "@2038"
  .4byte "@2039"
mareM_bas_c_table:
  .4byte "@2040"
  .4byte "@2041"
  .4byte "@2042"
  .4byte "@2043"
  .4byte "@2044"
  .4byte "@2045"
  .4byte "@2046"
  .4byte 0
  .4byte 0
  .4byte "@2047"
  .4byte "@2048"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte "@2035"
  .4byte "@2049"
  .4byte "@2050"
  .4byte "@2051"
  .4byte "@2052"
mareMA_bastable:
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
mareMA_bas_c_table:
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
mareMB_bastable:
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte "@2057"
mareMB_bas_c_table:
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte "@2057"
mareMC_bastable:
  .4byte "@2059"
  .4byte "@2060"
  .4byte 0
  .4byte "@2061"
  .4byte "@2062"
mareMC_bas_c_table:
  .4byte "@2059"
  .4byte "@2060"
  .4byte 0
  .4byte "@2061"
  .4byte "@2062"
mareW_bastable:
  .4byte "@2069"
  .4byte "@2070"
  .4byte "@2071"
  .4byte "@2072"
  .4byte "@2073"
  .4byte "@2074"
  .4byte 0
  .4byte 0
  .4byte "@2075"
  .4byte "@2076"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte "@2077"
  .4byte "@2078"
  .4byte "@2079"
  .4byte "@2080"
  .4byte "@2081"
mareW_bas_c_table:
  .4byte "@2082"
  .4byte "@2083"
  .4byte "@2084"
  .4byte "@2085"
  .4byte "@2086"
  .4byte "@2087"
  .4byte 0
  .4byte 0
  .4byte "@2088"
  .4byte "@2089"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte "@2077"
  .4byte "@2090"
  .4byte "@2091"
  .4byte "@2092"
  .4byte "@2093"
mareWA_bastable:
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
mareWA_bas_c_table:
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
mareWB_bastable:
  .4byte "@2098"
  .4byte "@2099"
  .4byte "@2100"
  .4byte 0
  .4byte "@2101"
  .4byte "@2102"
mareWB_bas_c_table:
  .4byte "@2098"
  .4byte "@2099"
  .4byte "@2100"
  .4byte 0
  .4byte "@2101"
  .4byte "@2102"
kinopio_bastable:
  .4byte "@2105"
  .4byte "@2106"
  .4byte "@2107"
  .4byte "@2108"
  .4byte "@2109"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte "@2110"
  .4byte 0
  .4byte "@2111"
  .4byte 0
  .4byte 0
  .4byte "@2112"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte "@2113"
  .4byte "@2114"
  .4byte 0
  .4byte "@2115"
  .4byte "@2116"
  .4byte "@2117"
  .4byte 0
kinopio_bas_c_table:
  .4byte "@2105"
  .4byte "@2106"
  .4byte "@2107"
  .4byte "@2108"
  .4byte "@2109"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte "@2110"
  .4byte 0
  .4byte "@2111"
  .4byte 0
  .4byte 0
  .4byte "@2112"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte "@2113"
  .4byte "@2114"
  .4byte 0
  .4byte "@2115"
  .4byte "@2116"
  .4byte "@2117"
  .4byte 0
kinojii_bastable:
  .4byte "@2120"
  .4byte "@2121"
  .4byte 0
  .4byte 0
  .4byte "@2122"
  .4byte 0
  .4byte "@2123"
  .4byte 0
  .4byte 0
  .4byte "@2124"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte "@2125"
  .4byte 0
  .4byte "@2126"
  .4byte "@2127"
  .4byte 0
kinojii_bas_c_table:
  .4byte "@2120"
  .4byte "@2121"
  .4byte 0
  .4byte 0
  .4byte "@2122"
  .4byte 0
  .4byte "@2123"
  .4byte 0
  .4byte 0
  .4byte "@2124"
  .4byte 0
  .4byte 0
  .4byte 0
  .4byte "@2125"
  .4byte 0
  .4byte "@2126"
  .4byte "@2127"
  .4byte 0
peach_bastable:
  .4byte "@2130"
  .4byte "@2131"
  .4byte "@2132"
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
  .4byte "@2133"
  .4byte "@2134"
  .4byte "@2135"
  .4byte 0
  .4byte "@2136"
peach_bas_c_table:
  .4byte "@2130"
  .4byte "@2131"
  .4byte "@2132"
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
  .4byte "@2133"
  .4byte "@2134"
  .4byte "@2135"
  .4byte 0
  .4byte "@2136"
raccoonDog_bastable:
  .4byte "@2139"
  .4byte "@2140"
  .4byte 0
  .4byte "@2141"
raccoonDog_bas_c_table:
  .4byte "@2139"
  .4byte "@2140"
  .4byte 0
  .4byte "@2141"
sunflowerL_bastable:
  .4byte 0
  .4byte "@2144"
  .4byte 0
  .4byte "@2145"
  .4byte "@2146"
sunflowerL_bas_c_table:
  .4byte 0
  .4byte "@2144"
  .4byte 0
  .4byte "@2145"
  .4byte "@2146"
sunflowerS_bastable:
  .4byte 0
  .4byte "@2150"
  .4byte 0
  .4byte "@2151"
  .4byte "@2152"
sunflowerS_bas_c_table:
  .4byte 0
  .4byte "@2150"
  .4byte 0
  .4byte "@2151"
  .4byte "@2152"
"@2235":
    .4byte lbl_801773CC
    .4byte lbl_801773D4
    .4byte lbl_801773DC
    .4byte lbl_801773E4
    .4byte lbl_80177480
    .4byte lbl_80177480
    .4byte lbl_80177480
    .4byte lbl_801773EC
    .4byte lbl_801773F4
    .4byte lbl_8017740C
    .4byte lbl_80177414
    .4byte lbl_8017741C
    .4byte lbl_80177424
    .4byte lbl_8017742C
    .4byte lbl_80177444
    .4byte lbl_8017744C
    .4byte lbl_80177454
    .4byte lbl_8017745C
    .4byte lbl_80177464
    .4byte lbl_8017746C
    .4byte lbl_80177474
    .4byte lbl_8017747C
    .4byte lbl_80177480
    .4byte lbl_80177480

.section .sdata, "wa"  # 0x80408AC0 - 0x804097C0
monteMG_bastable:
  .4byte "@1980"
  .4byte "@1981"
mareMD_bastable:
  .4byte "@2065"
  .4byte "@2066"

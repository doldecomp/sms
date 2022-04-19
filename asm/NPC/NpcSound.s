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
	.incbin "baserom.dol", 0x37F7EC, 0x14
"@1755":
	.incbin "baserom.dol", 0x37F800, 0x30
"@1756":
	.incbin "baserom.dol", 0x37F830, 0x38
"@1757":
	.incbin "baserom.dol", 0x37F868, 0x34
"@1758":
	.incbin "baserom.dol", 0x37F89C, 0x24
"@1938":
	.incbin "baserom.dol", 0x37F8C0, 0x28
"@1939":
	.incbin "baserom.dol", 0x37F8E8, 0x28
"@1940":
	.incbin "baserom.dol", 0x37F910, 0x28
"@1941":
	.incbin "baserom.dol", 0x37F938, 0x28
"@1942":
	.incbin "baserom.dol", 0x37F960, 0x24
"@1943":
	.incbin "baserom.dol", 0x37F984, 0x28
"@1944":
	.incbin "baserom.dol", 0x37F9AC, 0x28
"@1945":
	.incbin "baserom.dol", 0x37F9D4, 0x24
"@1946":
	.incbin "baserom.dol", 0x37F9F8, 0x28
"@1947":
	.incbin "baserom.dol", 0x37FA20, 0x28
"@1948":
	.incbin "baserom.dol", 0x37FA48, 0x2C
"@1949":
	.incbin "baserom.dol", 0x37FA74, 0x28
"@1950":
	.incbin "baserom.dol", 0x37FA9C, 0x24
"@1951":
	.incbin "baserom.dol", 0x37FAC0, 0x28
"@1952":
	.incbin "baserom.dol", 0x37FAE8, 0x2C
"@1953":
	.incbin "baserom.dol", 0x37FB14, 0x2C
"@1954":
	.incbin "baserom.dol", 0x37FB40, 0x28
"@1955":
	.incbin "baserom.dol", 0x37FB68, 0x28
"@1956":
	.incbin "baserom.dol", 0x37FB90, 0x28
"@1957":
	.incbin "baserom.dol", 0x37FBB8, 0x28
"@1958":
	.incbin "baserom.dol", 0x37FBE0, 0x2C
"@1959":
	.incbin "baserom.dol", 0x37FC0C, 0x2C
"@1960":
	.incbin "baserom.dol", 0x37FC38, 0x28
"@1961":
	.incbin "baserom.dol", 0x37FC60, 0x2C
"@1962":
	.incbin "baserom.dol", 0x37FC8C, 0x28
"@1963":
	.incbin "baserom.dol", 0x37FCB4, 0x30
"@1964":
	.incbin "baserom.dol", 0x37FCE4, 0x28
"@1965":
	.incbin "baserom.dol", 0x37FD0C, 0x28
"@1966":
	.incbin "baserom.dol", 0x37FD34, 0x28
"@1967":
	.incbin "baserom.dol", 0x37FD5C, 0x30
"@1970":
	.incbin "baserom.dol", 0x37FD8C, 0x24
"@1971":
	.incbin "baserom.dol", 0x37FDB0, 0x20
"@1972":
	.incbin "baserom.dol", 0x37FDD0, 0x20
"@1975":
	.incbin "baserom.dol", 0x37FDF0, 0x24
"@1976":
	.incbin "baserom.dol", 0x37FE14, 0x24
"@1977":
	.incbin "baserom.dol", 0x37FE38, 0x28
"@1980":
	.incbin "baserom.dol", 0x37FE60, 0x2C
"@1981":
	.incbin "baserom.dol", 0x37FE8C, 0x2C
"@1984":
	.incbin "baserom.dol", 0x37FEB8, 0x24
"@1985":
	.incbin "baserom.dol", 0x37FEDC, 0x28
"@1986":
	.incbin "baserom.dol", 0x37FF04, 0x28
"@1989":
	.incbin "baserom.dol", 0x37FF2C, 0x28
"@1990":
	.incbin "baserom.dol", 0x37FF54, 0x28
"@1991":
	.incbin "baserom.dol", 0x37FF7C, 0x28
"@1992":
	.incbin "baserom.dol", 0x37FFA4, 0x28
"@1993":
	.incbin "baserom.dol", 0x37FFCC, 0x24
"@1994":
	.incbin "baserom.dol", 0x37FFF0, 0x28
"@1995":
	.incbin "baserom.dol", 0x380018, 0x24
"@1996":
	.incbin "baserom.dol", 0x38003C, 0x28
"@1997":
	.incbin "baserom.dol", 0x380064, 0x28
"@1998":
	.incbin "baserom.dol", 0x38008C, 0x2C
"@1999":
	.incbin "baserom.dol", 0x3800B8, 0x2C
"@2000":
	.incbin "baserom.dol", 0x3800E4, 0x28
"@2001":
	.incbin "baserom.dol", 0x38010C, 0x24
"@2002":
	.incbin "baserom.dol", 0x380130, 0x28
"@2003":
	.incbin "baserom.dol", 0x380158, 0x2C
"@2004":
	.incbin "baserom.dol", 0x380184, 0x2C
"@2005":
	.incbin "baserom.dol", 0x3801B0, 0x28
"@2006":
	.incbin "baserom.dol", 0x3801D8, 0x28
"@2007":
	.incbin "baserom.dol", 0x380200, 0x28
"@2008":
	.incbin "baserom.dol", 0x380228, 0x28
"@2009":
	.incbin "baserom.dol", 0x380250, 0x2C
"@2010":
	.incbin "baserom.dol", 0x38027C, 0x28
"@2011":
	.incbin "baserom.dol", 0x3802A4, 0x2C
"@2012":
	.incbin "baserom.dol", 0x3802D0, 0x28
"@2013":
	.incbin "baserom.dol", 0x3802F8, 0x30
"@2014":
	.incbin "baserom.dol", 0x380328, 0x30
"@2015":
	.incbin "baserom.dol", 0x380358, 0x28
"@2016":
	.incbin "baserom.dol", 0x380380, 0x28
"@2017":
	.incbin "baserom.dol", 0x3803A8, 0x28
"@2018":
	.incbin "baserom.dol", 0x3803D0, 0x30
"@2021":
	.incbin "baserom.dol", 0x380400, 0x20
"@2022":
	.incbin "baserom.dol", 0x380420, 0x20
"@2023":
	.incbin "baserom.dol", 0x380440, 0x24
"@2026":
	.incbin "baserom.dol", 0x380464, 0x24
"@2027":
	.incbin "baserom.dol", 0x380488, 0x24
"@2028":
	.incbin "baserom.dol", 0x3804AC, 0x20
"@2029":
	.incbin "baserom.dol", 0x3804CC, 0x24
"@2030":
	.incbin "baserom.dol", 0x3804F0, 0x20
"@2031":
	.incbin "baserom.dol", 0x380510, 0x24
"@2032":
	.incbin "baserom.dol", 0x380534, 0x20
"@2033":
	.incbin "baserom.dol", 0x380554, 0x24
"@2034":
	.incbin "baserom.dol", 0x380578, 0x24
"@2035":
	.incbin "baserom.dol", 0x38059C, 0x28
"@2036":
	.incbin "baserom.dol", 0x3805C4, 0x20
"@2037":
	.incbin "baserom.dol", 0x3805E4, 0x20
"@2038":
	.incbin "baserom.dol", 0x380604, 0x20
"@2039":
	.incbin "baserom.dol", 0x380624, 0x24
"@2040":
	.incbin "baserom.dol", 0x380648, 0x24
"@2041":
	.incbin "baserom.dol", 0x38066C, 0x24
"@2042":
	.incbin "baserom.dol", 0x380690, 0x24
"@2043":
	.incbin "baserom.dol", 0x3806B4, 0x24
"@2044":
	.incbin "baserom.dol", 0x3806D8, 0x24
"@2045":
	.incbin "baserom.dol", 0x3806FC, 0x28
"@2046":
	.incbin "baserom.dol", 0x380724, 0x24
"@2047":
	.incbin "baserom.dol", 0x380748, 0x24
"@2048":
	.incbin "baserom.dol", 0x38076C, 0x28
"@2049":
	.incbin "baserom.dol", 0x380794, 0x24
"@2050":
	.incbin "baserom.dol", 0x3807B8, 0x24
"@2051":
	.incbin "baserom.dol", 0x3807DC, 0x24
"@2052":
	.incbin "baserom.dol", 0x380800, 0x24
"@2057":
	.incbin "baserom.dol", 0x380824, 0x24
"@2059":
	.incbin "baserom.dol", 0x380848, 0x24
"@2060":
	.incbin "baserom.dol", 0x38086C, 0x28
"@2061":
	.incbin "baserom.dol", 0x380894, 0x24
"@2062":
	.incbin "baserom.dol", 0x3808B8, 0x24
"@2065":
	.incbin "baserom.dol", 0x3808DC, 0x24
"@2066":
	.incbin "baserom.dol", 0x380900, 0x24
"@2069":
	.incbin "baserom.dol", 0x380924, 0x24
"@2070":
	.incbin "baserom.dol", 0x380948, 0x24
"@2071":
	.incbin "baserom.dol", 0x38096C, 0x20
"@2072":
	.incbin "baserom.dol", 0x38098C, 0x24
"@2073":
	.incbin "baserom.dol", 0x3809B0, 0x24
"@2074":
	.incbin "baserom.dol", 0x3809D4, 0x20
"@2075":
	.incbin "baserom.dol", 0x3809F4, 0x24
"@2076":
	.incbin "baserom.dol", 0x380A18, 0x24
"@2077":
	.incbin "baserom.dol", 0x380A3C, 0x28
"@2078":
	.incbin "baserom.dol", 0x380A64, 0x20
"@2079":
	.incbin "baserom.dol", 0x380A84, 0x20
"@2080":
	.incbin "baserom.dol", 0x380AA4, 0x20
"@2081":
	.incbin "baserom.dol", 0x380AC4, 0x24
"@2082":
	.incbin "baserom.dol", 0x380AE8, 0x24
"@2083":
	.incbin "baserom.dol", 0x380B0C, 0x24
"@2084":
	.incbin "baserom.dol", 0x380B30, 0x24
"@2085":
	.incbin "baserom.dol", 0x380B54, 0x24
"@2086":
	.incbin "baserom.dol", 0x380B78, 0x28
"@2087":
	.incbin "baserom.dol", 0x380BA0, 0x24
"@2088":
	.incbin "baserom.dol", 0x380BC4, 0x24
"@2089":
	.incbin "baserom.dol", 0x380BE8, 0x28
"@2090":
	.incbin "baserom.dol", 0x380C10, 0x24
"@2091":
	.incbin "baserom.dol", 0x380C34, 0x24
"@2092":
	.incbin "baserom.dol", 0x380C58, 0x24
"@2093":
	.incbin "baserom.dol", 0x380C7C, 0x24
"@2098":
	.incbin "baserom.dol", 0x380CA0, 0x28
"@2099":
	.incbin "baserom.dol", 0x380CC8, 0x2C
"@2100":
	.incbin "baserom.dol", 0x380CF4, 0x2C
"@2101":
	.incbin "baserom.dol", 0x380D20, 0x28
"@2102":
	.incbin "baserom.dol", 0x380D48, 0x28
"@2105":
	.incbin "baserom.dol", 0x380D70, 0x28
"@2106":
	.incbin "baserom.dol", 0x380D98, 0x24
"@2107":
	.incbin "baserom.dol", 0x380DBC, 0x28
"@2108":
	.incbin "baserom.dol", 0x380DE4, 0x28
"@2109":
	.incbin "baserom.dol", 0x380E0C, 0x24
"@2110":
	.incbin "baserom.dol", 0x380E30, 0x28
"@2111":
	.incbin "baserom.dol", 0x380E58, 0x28
"@2112":
	.incbin "baserom.dol", 0x380E80, 0x28
"@2113":
	.incbin "baserom.dol", 0x380EA8, 0x2C
"@2114":
	.incbin "baserom.dol", 0x380ED4, 0x24
"@2115":
	.incbin "baserom.dol", 0x380EF8, 0x24
"@2116":
	.incbin "baserom.dol", 0x380F1C, 0x28
"@2117":
	.incbin "baserom.dol", 0x380F44, 0x2C
"@2120":
	.incbin "baserom.dol", 0x380F70, 0x24
"@2121":
	.incbin "baserom.dol", 0x380F94, 0x28
"@2122":
	.incbin "baserom.dol", 0x380FBC, 0x28
"@2123":
	.incbin "baserom.dol", 0x380FE4, 0x28
"@2124":
	.incbin "baserom.dol", 0x38100C, 0x28
"@2125":
	.incbin "baserom.dol", 0x381034, 0x24
"@2126":
	.incbin "baserom.dol", 0x381058, 0x24
"@2127":
	.incbin "baserom.dol", 0x38107C, 0x24
"@2130":
	.incbin "baserom.dol", 0x3810A0, 0x28
"@2131":
	.incbin "baserom.dol", 0x3810C8, 0x24
"@2132":
	.incbin "baserom.dol", 0x3810EC, 0x28
"@2133":
	.incbin "baserom.dol", 0x381114, 0x28
"@2134":
	.incbin "baserom.dol", 0x38113C, 0x24
"@2135":
	.incbin "baserom.dol", 0x381160, 0x28
"@2136":
	.incbin "baserom.dol", 0x381188, 0x20
"@2139":
	.incbin "baserom.dol", 0x3811A8, 0x28
"@2140":
	.incbin "baserom.dol", 0x3811D0, 0x28
"@2141":
	.incbin "baserom.dol", 0x3811F8, 0x28
"@2144":
	.incbin "baserom.dol", 0x381220, 0x2C
"@2145":
	.incbin "baserom.dol", 0x38124C, 0x28
"@2146":
	.incbin "baserom.dol", 0x381274, 0x2C
"@2150":
	.incbin "baserom.dol", 0x3812A0, 0x2C
"@2151":
	.incbin "baserom.dol", 0x3812CC, 0x2C
"@2152":
	.incbin "baserom.dol", 0x3812F8, 0x30

.section .data, "wa"  # 0x803A8380 - 0x803E6000
"@1431":
	.incbin "baserom.dol", 0x3B64E8, 0xC
"@1411":
	.incbin "baserom.dol", 0x3B64F4, 0xC
"@1210":
	.incbin "baserom.dol", 0x3B6500, 0x10
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

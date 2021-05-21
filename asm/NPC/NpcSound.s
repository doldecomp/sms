.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global getBasNameTable__8TBaseNPCCFv
getBasNameTable__8TBaseNPCCFv:
/* 8017731C 0017425C  7C 08 02 A6 */	mflr r0
/* 80177320 00174260  3C 80 80 3C */	lis r4, $$21431@ha
/* 80177324 00174264  90 01 00 04 */	stw r0, 4(r1)
/* 80177328 00174268  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 8017732C 0017426C  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 80177330 00174270  93 C1 00 18 */	stw r30, 0x18(r1)
/* 80177334 00174274  3B C4 94 E8 */	addi r30, r4, $$21431@l
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
/* 801773B4 001742F4  3C 60 80 3C */	lis r3, $$22235@ha
/* 801773B8 001742F8  38 63 9B C8 */	addi r3, r3, $$22235@l
/* 801773BC 001742FC  54 84 10 3A */	slwi r4, r4, 2
/* 801773C0 00174300  7C 03 20 2E */	lwzx r0, r3, r4
/* 801773C4 00174304  7C 09 03 A6 */	mtctr r0
/* 801773C8 00174308  4E 80 04 20 */	bctr 
/* 801773CC 0017430C  3B BE 00 F0 */	addi r29, r30, 0xf0
/* 801773D0 00174310  48 00 00 B0 */	b lbl_80177480
/* 801773D4 00174314  3B BE 01 28 */	addi r29, r30, 0x128
/* 801773D8 00174318  48 00 00 A8 */	b lbl_80177480
/* 801773DC 0017431C  3B AD 84 F0 */	addi r29, r13, monteMG_bastable@sda21
/* 801773E0 00174320  48 00 00 A0 */	b lbl_80177480
/* 801773E4 00174324  3B BE 01 40 */	addi r29, r30, 0x140
/* 801773E8 00174328  48 00 00 98 */	b lbl_80177480
/* 801773EC 0017432C  3B BE 02 20 */	addi r29, r30, 0x220
/* 801773F0 00174330  48 00 00 90 */	b lbl_80177480
/* 801773F4 00174334  57 E0 06 3F */	clrlwi. r0, r31, 0x18
/* 801773F8 00174338  41 82 00 0C */	beq lbl_80177404
/* 801773FC 0017433C  3B BE 02 9C */	addi r29, r30, 0x29c
/* 80177400 00174340  48 00 00 80 */	b lbl_80177480
lbl_80177404:
/* 80177404 00174344  3B BE 02 48 */	addi r29, r30, 0x248
/* 80177408 00174348  48 00 00 78 */	b lbl_80177480
/* 8017740C 0017434C  3B BE 02 F0 */	addi r29, r30, 0x2f0
/* 80177410 00174350  48 00 00 70 */	b lbl_80177480
/* 80177414 00174354  3B BE 03 18 */	addi r29, r30, 0x318
/* 80177418 00174358  48 00 00 68 */	b lbl_80177480
/* 8017741C 0017435C  3B BE 03 48 */	addi r29, r30, 0x348
/* 80177420 00174360  48 00 00 60 */	b lbl_80177480
/* 80177424 00174364  3B AD 84 F8 */	addi r29, r13, mareMD_bastable@sda21
/* 80177428 00174368  48 00 00 58 */	b lbl_80177480
/* 8017742C 0017436C  57 E0 06 3F */	clrlwi. r0, r31, 0x18
/* 80177430 00174370  41 82 00 0C */	beq lbl_8017743C
/* 80177434 00174374  3B BE 03 C0 */	addi r29, r30, 0x3c0
/* 80177438 00174378  48 00 00 48 */	b lbl_80177480
lbl_8017743C:
/* 8017743C 0017437C  3B BE 03 70 */	addi r29, r30, 0x370
/* 80177440 00174380  48 00 00 40 */	b lbl_80177480
/* 80177444 00174384  3B BE 04 10 */	addi r29, r30, 0x410
/* 80177448 00174388  48 00 00 38 */	b lbl_80177480
/* 8017744C 0017438C  3B BE 04 30 */	addi r29, r30, 0x430
/* 80177450 00174390  48 00 00 30 */	b lbl_80177480
/* 80177454 00174394  3B BE 04 60 */	addi r29, r30, 0x460
/* 80177458 00174398  48 00 00 28 */	b lbl_80177480
/* 8017745C 0017439C  3B BE 05 28 */	addi r29, r30, 0x528
/* 80177460 001743A0  48 00 00 20 */	b lbl_80177480
/* 80177464 001743A4  3B BE 05 B8 */	addi r29, r30, 0x5b8
/* 80177468 001743A8  48 00 00 18 */	b lbl_80177480
/* 8017746C 001743AC  3B BE 06 70 */	addi r29, r30, 0x670
/* 80177470 001743B0  48 00 00 10 */	b lbl_80177480
/* 80177474 001743B4  3B BE 06 90 */	addi r29, r30, 0x690
/* 80177478 001743B8  48 00 00 08 */	b lbl_80177480
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

.section .rodata, "wa"  # 0x8036FFA0 - 0x803A8380
$$21490:
	.incbin "baserom.dol", 0x37F7E0, 0xC
$$21526:
	.incbin "baserom.dol", 0x37F7EC, 0x14
$$21755:
	.incbin "baserom.dol", 0x37F800, 0x30
$$21756:
	.incbin "baserom.dol", 0x37F830, 0x38
$$21757:
	.incbin "baserom.dol", 0x37F868, 0x34
$$21758:
	.incbin "baserom.dol", 0x37F89C, 0x24
$$21938:
	.incbin "baserom.dol", 0x37F8C0, 0x28
$$21939:
	.incbin "baserom.dol", 0x37F8E8, 0x28
$$21940:
	.incbin "baserom.dol", 0x37F910, 0x28
$$21941:
	.incbin "baserom.dol", 0x37F938, 0x28
$$21942:
	.incbin "baserom.dol", 0x37F960, 0x24
$$21943:
	.incbin "baserom.dol", 0x37F984, 0x28
$$21944:
	.incbin "baserom.dol", 0x37F9AC, 0x28
$$21945:
	.incbin "baserom.dol", 0x37F9D4, 0x24
$$21946:
	.incbin "baserom.dol", 0x37F9F8, 0x28
$$21947:
	.incbin "baserom.dol", 0x37FA20, 0x28
$$21948:
	.incbin "baserom.dol", 0x37FA48, 0x2C
$$21949:
	.incbin "baserom.dol", 0x37FA74, 0x28
$$21950:
	.incbin "baserom.dol", 0x37FA9C, 0x24
$$21951:
	.incbin "baserom.dol", 0x37FAC0, 0x28
$$21952:
	.incbin "baserom.dol", 0x37FAE8, 0x2C
$$21953:
	.incbin "baserom.dol", 0x37FB14, 0x2C
$$21954:
	.incbin "baserom.dol", 0x37FB40, 0x28
$$21955:
	.incbin "baserom.dol", 0x37FB68, 0x28
$$21956:
	.incbin "baserom.dol", 0x37FB90, 0x28
$$21957:
	.incbin "baserom.dol", 0x37FBB8, 0x28
$$21958:
	.incbin "baserom.dol", 0x37FBE0, 0x2C
$$21959:
	.incbin "baserom.dol", 0x37FC0C, 0x2C
$$21960:
	.incbin "baserom.dol", 0x37FC38, 0x28
$$21961:
	.incbin "baserom.dol", 0x37FC60, 0x2C
$$21962:
	.incbin "baserom.dol", 0x37FC8C, 0x28
$$21963:
	.incbin "baserom.dol", 0x37FCB4, 0x30
$$21964:
	.incbin "baserom.dol", 0x37FCE4, 0x28
$$21965:
	.incbin "baserom.dol", 0x37FD0C, 0x28
$$21966:
	.incbin "baserom.dol", 0x37FD34, 0x28
$$21967:
	.incbin "baserom.dol", 0x37FD5C, 0x30
$$21970:
	.incbin "baserom.dol", 0x37FD8C, 0x24
$$21971:
	.incbin "baserom.dol", 0x37FDB0, 0x20
$$21972:
	.incbin "baserom.dol", 0x37FDD0, 0x20
$$21975:
	.incbin "baserom.dol", 0x37FDF0, 0x24
$$21976:
	.incbin "baserom.dol", 0x37FE14, 0x24
$$21977:
	.incbin "baserom.dol", 0x37FE38, 0x28
$$21980:
	.incbin "baserom.dol", 0x37FE60, 0x2C
$$21981:
	.incbin "baserom.dol", 0x37FE8C, 0x2C
$$21984:
	.incbin "baserom.dol", 0x37FEB8, 0x24
$$21985:
	.incbin "baserom.dol", 0x37FEDC, 0x28
$$21986:
	.incbin "baserom.dol", 0x37FF04, 0x28
$$21989:
	.incbin "baserom.dol", 0x37FF2C, 0x28
$$21990:
	.incbin "baserom.dol", 0x37FF54, 0x28
$$21991:
	.incbin "baserom.dol", 0x37FF7C, 0x28
$$21992:
	.incbin "baserom.dol", 0x37FFA4, 0x28
$$21993:
	.incbin "baserom.dol", 0x37FFCC, 0x24
$$21994:
	.incbin "baserom.dol", 0x37FFF0, 0x28
$$21995:
	.incbin "baserom.dol", 0x380018, 0x24
$$21996:
	.incbin "baserom.dol", 0x38003C, 0x28
$$21997:
	.incbin "baserom.dol", 0x380064, 0x28
$$21998:
	.incbin "baserom.dol", 0x38008C, 0x2C
$$21999:
	.incbin "baserom.dol", 0x3800B8, 0x2C
$$22000:
	.incbin "baserom.dol", 0x3800E4, 0x28
$$22001:
	.incbin "baserom.dol", 0x38010C, 0x24
$$22002:
	.incbin "baserom.dol", 0x380130, 0x28
$$22003:
	.incbin "baserom.dol", 0x380158, 0x2C
$$22004:
	.incbin "baserom.dol", 0x380184, 0x2C
$$22005:
	.incbin "baserom.dol", 0x3801B0, 0x28
$$22006:
	.incbin "baserom.dol", 0x3801D8, 0x28
$$22007:
	.incbin "baserom.dol", 0x380200, 0x28
$$22008:
	.incbin "baserom.dol", 0x380228, 0x28
$$22009:
	.incbin "baserom.dol", 0x380250, 0x2C
$$22010:
	.incbin "baserom.dol", 0x38027C, 0x28
$$22011:
	.incbin "baserom.dol", 0x3802A4, 0x2C
$$22012:
	.incbin "baserom.dol", 0x3802D0, 0x28
$$22013:
	.incbin "baserom.dol", 0x3802F8, 0x30
$$22014:
	.incbin "baserom.dol", 0x380328, 0x30
$$22015:
	.incbin "baserom.dol", 0x380358, 0x28
$$22016:
	.incbin "baserom.dol", 0x380380, 0x28
$$22017:
	.incbin "baserom.dol", 0x3803A8, 0x28
$$22018:
	.incbin "baserom.dol", 0x3803D0, 0x30
$$22021:
	.incbin "baserom.dol", 0x380400, 0x20
$$22022:
	.incbin "baserom.dol", 0x380420, 0x20
$$22023:
	.incbin "baserom.dol", 0x380440, 0x24
$$22026:
	.incbin "baserom.dol", 0x380464, 0x24
$$22027:
	.incbin "baserom.dol", 0x380488, 0x24
$$22028:
	.incbin "baserom.dol", 0x3804AC, 0x20
$$22029:
	.incbin "baserom.dol", 0x3804CC, 0x24
$$22030:
	.incbin "baserom.dol", 0x3804F0, 0x20
$$22031:
	.incbin "baserom.dol", 0x380510, 0x24
$$22032:
	.incbin "baserom.dol", 0x380534, 0x20
$$22033:
	.incbin "baserom.dol", 0x380554, 0x24
$$22034:
	.incbin "baserom.dol", 0x380578, 0x24
$$22035:
	.incbin "baserom.dol", 0x38059C, 0x28
$$22036:
	.incbin "baserom.dol", 0x3805C4, 0x20
$$22037:
	.incbin "baserom.dol", 0x3805E4, 0x20
$$22038:
	.incbin "baserom.dol", 0x380604, 0x20
$$22039:
	.incbin "baserom.dol", 0x380624, 0x24
$$22040:
	.incbin "baserom.dol", 0x380648, 0x24
$$22041:
	.incbin "baserom.dol", 0x38066C, 0x24
$$22042:
	.incbin "baserom.dol", 0x380690, 0x24
$$22043:
	.incbin "baserom.dol", 0x3806B4, 0x24
$$22044:
	.incbin "baserom.dol", 0x3806D8, 0x24
$$22045:
	.incbin "baserom.dol", 0x3806FC, 0x28
$$22046:
	.incbin "baserom.dol", 0x380724, 0x24
$$22047:
	.incbin "baserom.dol", 0x380748, 0x24
$$22048:
	.incbin "baserom.dol", 0x38076C, 0x28
$$22049:
	.incbin "baserom.dol", 0x380794, 0x24
$$22050:
	.incbin "baserom.dol", 0x3807B8, 0x24
$$22051:
	.incbin "baserom.dol", 0x3807DC, 0x24
$$22052:
	.incbin "baserom.dol", 0x380800, 0x24
$$22057:
	.incbin "baserom.dol", 0x380824, 0x24
$$22059:
	.incbin "baserom.dol", 0x380848, 0x24
$$22060:
	.incbin "baserom.dol", 0x38086C, 0x28
$$22061:
	.incbin "baserom.dol", 0x380894, 0x24
$$22062:
	.incbin "baserom.dol", 0x3808B8, 0x24
$$22065:
	.incbin "baserom.dol", 0x3808DC, 0x24
$$22066:
	.incbin "baserom.dol", 0x380900, 0x24
$$22069:
	.incbin "baserom.dol", 0x380924, 0x24
$$22070:
	.incbin "baserom.dol", 0x380948, 0x24
$$22071:
	.incbin "baserom.dol", 0x38096C, 0x20
$$22072:
	.incbin "baserom.dol", 0x38098C, 0x24
$$22073:
	.incbin "baserom.dol", 0x3809B0, 0x24
$$22074:
	.incbin "baserom.dol", 0x3809D4, 0x20
$$22075:
	.incbin "baserom.dol", 0x3809F4, 0x24
$$22076:
	.incbin "baserom.dol", 0x380A18, 0x24
$$22077:
	.incbin "baserom.dol", 0x380A3C, 0x28
$$22078:
	.incbin "baserom.dol", 0x380A64, 0x20
$$22079:
	.incbin "baserom.dol", 0x380A84, 0x20
$$22080:
	.incbin "baserom.dol", 0x380AA4, 0x20
$$22081:
	.incbin "baserom.dol", 0x380AC4, 0x24
$$22082:
	.incbin "baserom.dol", 0x380AE8, 0x24
$$22083:
	.incbin "baserom.dol", 0x380B0C, 0x24
$$22084:
	.incbin "baserom.dol", 0x380B30, 0x24
$$22085:
	.incbin "baserom.dol", 0x380B54, 0x24
$$22086:
	.incbin "baserom.dol", 0x380B78, 0x28
$$22087:
	.incbin "baserom.dol", 0x380BA0, 0x24
$$22088:
	.incbin "baserom.dol", 0x380BC4, 0x24
$$22089:
	.incbin "baserom.dol", 0x380BE8, 0x28
$$22090:
	.incbin "baserom.dol", 0x380C10, 0x24
$$22091:
	.incbin "baserom.dol", 0x380C34, 0x24
$$22092:
	.incbin "baserom.dol", 0x380C58, 0x24
$$22093:
	.incbin "baserom.dol", 0x380C7C, 0x24
$$22098:
	.incbin "baserom.dol", 0x380CA0, 0x28
$$22099:
	.incbin "baserom.dol", 0x380CC8, 0x2C
$$22100:
	.incbin "baserom.dol", 0x380CF4, 0x2C
$$22101:
	.incbin "baserom.dol", 0x380D20, 0x28
$$22102:
	.incbin "baserom.dol", 0x380D48, 0x28
$$22105:
	.incbin "baserom.dol", 0x380D70, 0x28
$$22106:
	.incbin "baserom.dol", 0x380D98, 0x24
$$22107:
	.incbin "baserom.dol", 0x380DBC, 0x28
$$22108:
	.incbin "baserom.dol", 0x380DE4, 0x28
$$22109:
	.incbin "baserom.dol", 0x380E0C, 0x24
$$22110:
	.incbin "baserom.dol", 0x380E30, 0x28
$$22111:
	.incbin "baserom.dol", 0x380E58, 0x28
$$22112:
	.incbin "baserom.dol", 0x380E80, 0x28
$$22113:
	.incbin "baserom.dol", 0x380EA8, 0x2C
$$22114:
	.incbin "baserom.dol", 0x380ED4, 0x24
$$22115:
	.incbin "baserom.dol", 0x380EF8, 0x24
$$22116:
	.incbin "baserom.dol", 0x380F1C, 0x28
$$22117:
	.incbin "baserom.dol", 0x380F44, 0x2C
$$22120:
	.incbin "baserom.dol", 0x380F70, 0x24
$$22121:
	.incbin "baserom.dol", 0x380F94, 0x28
$$22122:
	.incbin "baserom.dol", 0x380FBC, 0x28
$$22123:
	.incbin "baserom.dol", 0x380FE4, 0x28
$$22124:
	.incbin "baserom.dol", 0x38100C, 0x28
$$22125:
	.incbin "baserom.dol", 0x381034, 0x24
$$22126:
	.incbin "baserom.dol", 0x381058, 0x24
$$22127:
	.incbin "baserom.dol", 0x38107C, 0x24
$$22130:
	.incbin "baserom.dol", 0x3810A0, 0x28
$$22131:
	.incbin "baserom.dol", 0x3810C8, 0x24
$$22132:
	.incbin "baserom.dol", 0x3810EC, 0x28
$$22133:
	.incbin "baserom.dol", 0x381114, 0x28
$$22134:
	.incbin "baserom.dol", 0x38113C, 0x24
$$22135:
	.incbin "baserom.dol", 0x381160, 0x28
$$22136:
	.incbin "baserom.dol", 0x381188, 0x20
$$22139:
	.incbin "baserom.dol", 0x3811A8, 0x28
$$22140:
	.incbin "baserom.dol", 0x3811D0, 0x28
$$22141:
	.incbin "baserom.dol", 0x3811F8, 0x28
$$22144:
	.incbin "baserom.dol", 0x381220, 0x2C
$$22145:
	.incbin "baserom.dol", 0x38124C, 0x28
$$22146:
	.incbin "baserom.dol", 0x381274, 0x2C
$$22150:
	.incbin "baserom.dol", 0x3812A0, 0x2C
$$22151:
	.incbin "baserom.dol", 0x3812CC, 0x2C
$$22152:
	.incbin "baserom.dol", 0x3812F8, 0x30

.section .data, "wa"  # 0x803A8380 - 0x803E6000
$$21431:
	.incbin "baserom.dol", 0x3B64E8, 0xC
$$21411:
	.incbin "baserom.dol", 0x3B64F4, 0xC
$$21210:
	.incbin "baserom.dol", 0x3B6500, 0x10
MtxCalcTypeName:
	.incbin "baserom.dol", 0x3B6510, 0x10
monteMCommon_bastable:
	.incbin "baserom.dol", 0x3B6520, 0x5C
monteMCommon_bas_c_table:
	.incbin "baserom.dol", 0x3B657C, 0x5C
monteME_bastable:
	.incbin "baserom.dol", 0x3B65D8, 0x1C
monteME_bas_c_table:
	.incbin "baserom.dol", 0x3B65F4, 0x1C
monteMF_bastable:
	.incbin "baserom.dol", 0x3B6610, 0xC
monteMF_bas_c_table:
	.incbin "baserom.dol", 0x3B661C, 0xC
monteMH_bastable:
	.incbin "baserom.dol", 0x3B6628, 0x10
monteMH_bas_c_table:
	.incbin "baserom.dol", 0x3B6638, 0x10
monteWCommon_bastable:
	.incbin "baserom.dol", 0x3B6648, 0x60
monteWCommon_bas_c_table:
	.incbin "baserom.dol", 0x3B66A8, 0x60
monteWC_bastable:
	.incbin "baserom.dol", 0x3B6708, 0x14
monteWC_bas_c_table:
	.incbin "baserom.dol", 0x3B671C, 0x14
mareM_bastable:
	.incbin "baserom.dol", 0x3B6730, 0x54
mareM_bas_c_table:
	.incbin "baserom.dol", 0x3B6784, 0x54
mareMA_bastable:
	.incbin "baserom.dol", 0x3B67D8, 0x14
mareMA_bas_c_table:
	.incbin "baserom.dol", 0x3B67EC, 0x14
mareMB_bastable:
	.incbin "baserom.dol", 0x3B6800, 0x18
mareMB_bas_c_table:
	.incbin "baserom.dol", 0x3B6818, 0x18
mareMC_bastable:
	.incbin "baserom.dol", 0x3B6830, 0x14
mareMC_bas_c_table:
	.incbin "baserom.dol", 0x3B6844, 0x14
mareW_bastable:
	.incbin "baserom.dol", 0x3B6858, 0x50
mareW_bas_c_table:
	.incbin "baserom.dol", 0x3B68A8, 0x50
mareWA_bastable:
	.incbin "baserom.dol", 0x3B68F8, 0x10
mareWA_bas_c_table:
	.incbin "baserom.dol", 0x3B6908, 0x10
mareWB_bastable:
	.incbin "baserom.dol", 0x3B6918, 0x18
mareWB_bas_c_table:
	.incbin "baserom.dol", 0x3B6930, 0x18
kinopio_bastable:
	.incbin "baserom.dol", 0x3B6948, 0x64
kinopio_bas_c_table:
	.incbin "baserom.dol", 0x3B69AC, 0x64
kinojii_bastable:
	.incbin "baserom.dol", 0x3B6A10, 0x48
kinojii_bas_c_table:
	.incbin "baserom.dol", 0x3B6A58, 0x48
peach_bastable:
	.incbin "baserom.dol", 0x3B6AA0, 0x5C
peach_bas_c_table:
	.incbin "baserom.dol", 0x3B6AFC, 0x5C
raccoonDog_bastable:
	.incbin "baserom.dol", 0x3B6B58, 0x10
raccoonDog_bas_c_table:
	.incbin "baserom.dol", 0x3B6B68, 0x10
sunflowerL_bastable:
	.incbin "baserom.dol", 0x3B6B78, 0x14
sunflowerL_bas_c_table:
	.incbin "baserom.dol", 0x3B6B8C, 0x14
sunflowerS_bastable:
	.incbin "baserom.dol", 0x3B6BA0, 0x14
sunflowerS_bas_c_table:
	.incbin "baserom.dol", 0x3B6BB4, 0x14
$$22235:
	.incbin "baserom.dol", 0x3B6BC8, 0x60

.section .sdata, "wa"  # 0x80408AC0 - 0x804097C0
monteMG_bastable:
	.incbin "baserom.dol", 0x3E34F0, 0x8
mareMD_bastable:
	.incbin "baserom.dol", 0x3E34F8, 0x8

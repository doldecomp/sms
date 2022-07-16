.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __dt__10TBGPolDropFv
__dt__10TBGPolDropFv:
/* 80311120 0030E060  7C 08 02 A6 */	mflr r0
/* 80311124 0030E064  90 01 00 04 */	stw r0, 4(r1)
/* 80311128 0030E068  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8031112C 0030E06C  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80311130 0030E070  3B E4 00 00 */	addi r31, r4, 0
/* 80311134 0030E074  93 C1 00 10 */	stw r30, 0x10(r1)
/* 80311138 0030E078  7C 7E 1B 79 */	or. r30, r3, r3
/* 8031113C 0030E07C  41 82 00 34 */	beq lbl_80311170
/* 80311140 0030E080  3C 60 80 3E */	lis r3, __vt__10TBGPolDrop@ha
/* 80311144 0030E084  38 63 04 60 */	addi r3, r3, __vt__10TBGPolDrop@l
/* 80311148 0030E088  90 7E 00 00 */	stw r3, 0(r30)
/* 8031114C 0030E08C  38 03 00 24 */	addi r0, r3, 0x24
/* 80311150 0030E090  38 7E 00 00 */	addi r3, r30, 0
/* 80311154 0030E094  90 1E 00 20 */	stw r0, 0x20(r30)
/* 80311158 0030E098  38 80 00 00 */	li r4, 0
/* 8031115C 0030E09C  4B D2 FA 39 */	bl __dt__Q26JDrama6TActorFv
/* 80311160 0030E0A0  7F E0 07 35 */	extsh. r0, r31
/* 80311164 0030E0A4  40 81 00 0C */	ble lbl_80311170
/* 80311168 0030E0A8  7F C3 F3 78 */	mr r3, r30
/* 8031116C 0030E0AC  4B CF B9 45 */	bl __dl__FPv
lbl_80311170:
/* 80311170 0030E0B0  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80311174 0030E0B4  7F C3 F3 78 */	mr r3, r30
/* 80311178 0030E0B8  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8031117C 0030E0BC  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 80311180 0030E0C0  7C 08 03 A6 */	mtlr r0
/* 80311184 0030E0C4  38 21 00 18 */	addi r1, r1, 0x18
/* 80311188 0030E0C8  4E 80 00 20 */	blr 

.global perform__10TBGPolDropFUlPQ26JDrama9TGraphics
perform__10TBGPolDropFUlPQ26JDrama9TGraphics:
/* 8031118C 0030E0CC  7C 08 02 A6 */	mflr r0
/* 80311190 0030E0D0  90 01 00 04 */	stw r0, 4(r1)
/* 80311194 0030E0D4  94 21 FF 30 */	stwu r1, -0xd0(r1)
/* 80311198 0030E0D8  BF 61 00 BC */	stmw r27, 0xbc(r1)
/* 8031119C 0030E0DC  7C 7B 1B 78 */	mr r27, r3
/* 803111A0 0030E0E0  3B 84 00 00 */	addi r28, r4, 0
/* 803111A4 0030E0E4  3B A5 00 00 */	addi r29, r5, 0
/* 803111A8 0030E0E8  80 03 00 58 */	lwz r0, 0x58(r3)
/* 803111AC 0030E0EC  2C 00 00 00 */	cmpwi r0, 0
/* 803111B0 0030E0F0  41 82 01 FC */	beq lbl_803113AC
/* 803111B4 0030E0F4  57 80 07 FF */	clrlwi. r0, r28, 0x1f
/* 803111B8 0030E0F8  41 82 00 0C */	beq lbl_803111C4
/* 803111BC 0030E0FC  7F 63 DB 78 */	mr r3, r27
/* 803111C0 0030E100  48 00 02 5D */	bl move__10TBGPolDropFv
lbl_803111C4:
/* 803111C4 0030E104  57 9F 07 BD */	rlwinm. r31, r28, 0, 0x1e, 0x1e
/* 803111C8 0030E108  41 82 01 A0 */	beq lbl_80311368
/* 803111CC 0030E10C  80 7B 00 50 */	lwz r3, 0x50(r27)
/* 803111D0 0030E110  80 1B 00 58 */	lwz r0, 0x58(r27)
/* 803111D4 0030E114  80 63 00 04 */	lwz r3, 4(r3)
/* 803111D8 0030E118  2C 00 00 01 */	cmpwi r0, 1
/* 803111DC 0030E11C  3B C3 00 20 */	addi r30, r3, 0x20
/* 803111E0 0030E120  40 82 01 08 */	bne lbl_803112E8
/* 803111E4 0030E124  3C 60 00 01 */	lis r3, 0x0000C000@ha
/* 803111E8 0030E128  80 0D 8D EC */	lwz r0, jmaSinShift@sda21(r13)
/* 803111EC 0030E12C  38 63 C0 00 */	addi r3, r3, 0x0000C000@l
/* 803111F0 0030E130  80 AD 8D F0 */	lwz r5, jmaSinTable@sda21(r13)
/* 803111F4 0030E134  7C 60 06 30 */	sraw r0, r3, r0
/* 803111F8 0030E138  80 8D 8D F4 */	lwz r4, jmaCosTable@sda21(r13)
/* 803111FC 0030E13C  54 00 10 3A */	slwi r0, r0, 2
/* 80311200 0030E140  C0 02 FA 40 */	lfs f0, "@2932"@sda21(r2)
/* 80311204 0030E144  7C 45 04 2E */	lfsx f2, r5, r0
/* 80311208 0030E148  38 61 00 64 */	addi r3, r1, 0x64
/* 8031120C 0030E14C  7C 64 04 2E */	lfsx f3, r4, r0
/* 80311210 0030E150  38 9B 00 44 */	addi r4, r27, 0x44
/* 80311214 0030E154  C0 22 FA 44 */	lfs f1, "@2933"@sda21(r2)
/* 80311218 0030E158  D0 01 00 70 */	stfs f0, 0x70(r1)
/* 8031121C 0030E15C  FC 00 10 50 */	fneg f0, f2
/* 80311220 0030E160  D0 21 00 74 */	stfs f1, 0x74(r1)
/* 80311224 0030E164  D0 21 00 78 */	stfs f1, 0x78(r1)
/* 80311228 0030E168  D0 21 00 7C */	stfs f1, 0x7c(r1)
/* 8031122C 0030E16C  D0 21 00 80 */	stfs f1, 0x80(r1)
/* 80311230 0030E170  D0 61 00 84 */	stfs f3, 0x84(r1)
/* 80311234 0030E174  D0 01 00 88 */	stfs f0, 0x88(r1)
/* 80311238 0030E178  D0 21 00 8C */	stfs f1, 0x8c(r1)
/* 8031123C 0030E17C  D0 21 00 90 */	stfs f1, 0x90(r1)
/* 80311240 0030E180  D0 41 00 94 */	stfs f2, 0x94(r1)
/* 80311244 0030E184  D0 61 00 98 */	stfs f3, 0x98(r1)
/* 80311248 0030E188  D0 21 00 9C */	stfs f1, 0x9c(r1)
/* 8031124C 0030E18C  4B DB 4D 45 */	bl "MsGetRotFromZaxis__FRCQ29JGeometry8TVec3<f>"
/* 80311250 0030E190  80 81 00 64 */	lwz r4, 0x64(r1)
/* 80311254 0030E194  7F C3 F3 78 */	mr r3, r30
/* 80311258 0030E198  80 01 00 68 */	lwz r0, 0x68(r1)
/* 8031125C 0030E19C  90 9B 00 30 */	stw r4, 0x30(r27)
/* 80311260 0030E1A0  90 1B 00 34 */	stw r0, 0x34(r27)
/* 80311264 0030E1A4  80 01 00 6C */	lwz r0, 0x6c(r1)
/* 80311268 0030E1A8  90 1B 00 38 */	stw r0, 0x38(r27)
/* 8031126C 0030E1AC  C0 1B 00 38 */	lfs f0, 0x38(r27)
/* 80311270 0030E1B0  C0 42 FA 48 */	lfs f2, "@2934"@sda21(r2)
/* 80311274 0030E1B4  C0 3B 00 34 */	lfs f1, 0x34(r27)
/* 80311278 0030E1B8  EC 02 00 32 */	fmuls f0, f2, f0
/* 8031127C 0030E1BC  C0 9B 00 30 */	lfs f4, 0x30(r27)
/* 80311280 0030E1C0  EC 22 00 72 */	fmuls f1, f2, f1
/* 80311284 0030E1C4  C0 7B 00 18 */	lfs f3, 0x18(r27)
/* 80311288 0030E1C8  EC 42 01 32 */	fmuls f2, f2, f4
/* 8031128C 0030E1CC  FC 80 08 1E */	fctiwz f4, f1
/* 80311290 0030E1D0  C0 3B 00 10 */	lfs f1, 0x10(r27)
/* 80311294 0030E1D4  FC A0 10 1E */	fctiwz f5, f2
/* 80311298 0030E1D8  C0 5B 00 14 */	lfs f2, 0x14(r27)
/* 8031129C 0030E1DC  FC 00 00 1E */	fctiwz f0, f0
/* 803112A0 0030E1E0  D8 81 00 A8 */	stfd f4, 0xa8(r1)
/* 803112A4 0030E1E4  D8 A1 00 B0 */	stfd f5, 0xb0(r1)
/* 803112A8 0030E1E8  80 A1 00 AC */	lwz r5, 0xac(r1)
/* 803112AC 0030E1EC  D8 01 00 A0 */	stfd f0, 0xa0(r1)
/* 803112B0 0030E1F0  80 81 00 B4 */	lwz r4, 0xb4(r1)
/* 803112B4 0030E1F4  80 C1 00 A4 */	lwz r6, 0xa4(r1)
/* 803112B8 0030E1F8  4B DB 4B 31 */	bl MsMtxSetXYZRPH__FPA4_ffffsss
/* 803112BC 0030E1FC  38 7E 00 00 */	addi r3, r30, 0
/* 803112C0 0030E200  38 BE 00 00 */	addi r5, r30, 0
/* 803112C4 0030E204  38 81 00 70 */	addi r4, r1, 0x70
/* 803112C8 0030E208  4B D8 30 49 */	bl PSMTXConcat
/* 803112CC 0030E20C  80 6D 97 98 */	lwz r3, gpMarioParticleManager@sda21(r13)
/* 803112D0 0030E210  38 FB 00 00 */	addi r7, r27, 0
/* 803112D4 0030E214  38 BB 00 10 */	addi r5, r27, 0x10
/* 803112D8 0030E218  38 80 01 3A */	li r4, 0x13a
/* 803112DC 0030E21C  38 C0 00 01 */	li r6, 1
/* 803112E0 0030E220  4B DC A4 F1 */	bl "emitAndBindToPosPtr__21TMarioParticleManagerFlPCQ29JGeometry8TVec3<f>UcPCv"
/* 803112E4 0030E224  48 00 00 24 */	b lbl_80311308
lbl_803112E8:
/* 803112E8 0030E228  7F C3 F3 78 */	mr r3, r30
/* 803112EC 0030E22C  4B D8 2F C5 */	bl PSMTXIdentity
/* 803112F0 0030E230  C0 1B 00 10 */	lfs f0, 0x10(r27)
/* 803112F4 0030E234  D0 1E 00 0C */	stfs f0, 0xc(r30)
/* 803112F8 0030E238  C0 1B 00 14 */	lfs f0, 0x14(r27)
/* 803112FC 0030E23C  D0 1E 00 1C */	stfs f0, 0x1c(r30)
/* 80311300 0030E240  C0 1B 00 18 */	lfs f0, 0x18(r27)
/* 80311304 0030E244  D0 1E 00 2C */	stfs f0, 0x2c(r30)
lbl_80311308:
/* 80311308 0030E248  80 9B 00 50 */	lwz r4, 0x50(r27)
/* 8031130C 0030E24C  80 7B 00 24 */	lwz r3, 0x24(r27)
/* 80311310 0030E250  80 84 00 04 */	lwz r4, 4(r4)
/* 80311314 0030E254  80 1B 00 28 */	lwz r0, 0x28(r27)
/* 80311318 0030E258  90 64 00 14 */	stw r3, 0x14(r4)
/* 8031131C 0030E25C  90 04 00 18 */	stw r0, 0x18(r4)
/* 80311320 0030E260  80 1B 00 2C */	lwz r0, 0x2c(r27)
/* 80311324 0030E264  90 04 00 1C */	stw r0, 0x1c(r4)
/* 80311328 0030E268  80 1B 00 58 */	lwz r0, 0x58(r27)
/* 8031132C 0030E26C  2C 00 00 02 */	cmpwi r0, 2
/* 80311330 0030E270  40 82 00 38 */	bne lbl_80311368
/* 80311334 0030E274  80 BB 00 54 */	lwz r5, 0x54(r27)
/* 80311338 0030E278  7F C3 F3 78 */	mr r3, r30
/* 8031133C 0030E27C  80 9B 00 24 */	lwz r4, 0x24(r27)
/* 80311340 0030E280  80 A5 00 04 */	lwz r5, 4(r5)
/* 80311344 0030E284  80 1B 00 28 */	lwz r0, 0x28(r27)
/* 80311348 0030E288  90 85 00 14 */	stw r4, 0x14(r5)
/* 8031134C 0030E28C  90 05 00 18 */	stw r0, 0x18(r5)
/* 80311350 0030E290  80 1B 00 2C */	lwz r0, 0x2c(r27)
/* 80311354 0030E294  90 05 00 1C */	stw r0, 0x1c(r5)
/* 80311358 0030E298  80 9B 00 54 */	lwz r4, 0x54(r27)
/* 8031135C 0030E29C  80 84 00 04 */	lwz r4, 4(r4)
/* 80311360 0030E2A0  38 84 00 20 */	addi r4, r4, 0x20
/* 80311364 0030E2A4  4B D8 2F 79 */	bl PSMTXCopy
lbl_80311368:
/* 80311368 0030E2A8  80 7B 00 50 */	lwz r3, 0x50(r27)
/* 8031136C 0030E2AC  38 9C 00 00 */	addi r4, r28, 0
/* 80311370 0030E2B0  38 BD 00 00 */	addi r5, r29, 0
/* 80311374 0030E2B4  4B DC 31 65 */	bl perform__6MActorFUlPQ26JDrama9TGraphics
/* 80311378 0030E2B8  80 1B 00 58 */	lwz r0, 0x58(r27)
/* 8031137C 0030E2BC  2C 00 00 02 */	cmpwi r0, 2
/* 80311380 0030E2C0  40 82 00 2C */	bne lbl_803113AC
/* 80311384 0030E2C4  28 1F 00 00 */	cmplwi r31, 0
/* 80311388 0030E2C8  41 82 00 0C */	beq lbl_80311394
/* 8031138C 0030E2CC  80 7B 00 54 */	lwz r3, 0x54(r27)
/* 80311390 0030E2D0  4B DC 37 B9 */	bl calcAnm__6MActorFv
lbl_80311394:
/* 80311394 0030E2D4  57 80 05 AD */	rlwinm. r0, r28, 0, 0x16, 0x16
/* 80311398 0030E2D8  41 82 00 14 */	beq lbl_803113AC
/* 8031139C 0030E2DC  80 9B 00 54 */	lwz r4, 0x54(r27)
/* 803113A0 0030E2E0  80 6D 9A E8 */	lwz r3, gpPollution@sda21(r13)
/* 803113A4 0030E2E4  80 84 00 04 */	lwz r4, 4(r4)
/* 803113A8 0030E2E8  4B ED E7 35 */	bl stampModel__17TPollutionManagerFP8J3DModel
lbl_803113AC:
/* 803113AC 0030E2EC  BB 61 00 BC */	lmw r27, 0xbc(r1)
/* 803113B0 0030E2F0  80 01 00 D4 */	lwz r0, 0xd4(r1)
/* 803113B4 0030E2F4  38 21 00 D0 */	addi r1, r1, 0xd0
/* 803113B8 0030E2F8  7C 08 03 A6 */	mtlr r0
/* 803113BC 0030E2FC  4E 80 00 20 */	blr 

.global "launch__10TBGPolDropFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>"
"launch__10TBGPolDropFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>":
/* 803113C0 0030E300  80 E5 00 00 */	lwz r7, 0(r5)
/* 803113C4 0030E304  38 00 00 01 */	li r0, 1
/* 803113C8 0030E308  80 C5 00 04 */	lwz r6, 4(r5)
/* 803113CC 0030E30C  90 E3 00 44 */	stw r7, 0x44(r3)
/* 803113D0 0030E310  90 C3 00 48 */	stw r6, 0x48(r3)
/* 803113D4 0030E314  80 A5 00 08 */	lwz r5, 8(r5)
/* 803113D8 0030E318  90 A3 00 4C */	stw r5, 0x4c(r3)
/* 803113DC 0030E31C  80 C4 00 00 */	lwz r6, 0(r4)
/* 803113E0 0030E320  80 A4 00 04 */	lwz r5, 4(r4)
/* 803113E4 0030E324  90 C3 00 10 */	stw r6, 0x10(r3)
/* 803113E8 0030E328  90 A3 00 14 */	stw r5, 0x14(r3)
/* 803113EC 0030E32C  80 84 00 08 */	lwz r4, 8(r4)
/* 803113F0 0030E330  90 83 00 18 */	stw r4, 0x18(r3)
/* 803113F4 0030E334  C0 02 FA 40 */	lfs f0, "@2932"@sda21(r2)
/* 803113F8 0030E338  D0 03 00 24 */	stfs f0, 0x24(r3)
/* 803113FC 0030E33C  D0 03 00 28 */	stfs f0, 0x28(r3)
/* 80311400 0030E340  D0 03 00 2C */	stfs f0, 0x2c(r3)
/* 80311404 0030E344  C0 02 FA 44 */	lfs f0, "@2933"@sda21(r2)
/* 80311408 0030E348  D0 03 00 38 */	stfs f0, 0x38(r3)
/* 8031140C 0030E34C  D0 03 00 34 */	stfs f0, 0x34(r3)
/* 80311410 0030E350  D0 03 00 30 */	stfs f0, 0x30(r3)
/* 80311414 0030E354  90 03 00 58 */	stw r0, 0x58(r3)
/* 80311418 0030E358  4E 80 00 20 */	blr 

.global move__10TBGPolDropFv
move__10TBGPolDropFv:
/* 8031141C 0030E35C  7C 08 02 A6 */	mflr r0
/* 80311420 0030E360  90 01 00 04 */	stw r0, 4(r1)
/* 80311424 0030E364  94 21 FF C8 */	stwu r1, -0x38(r1)
/* 80311428 0030E368  93 E1 00 34 */	stw r31, 0x34(r1)
/* 8031142C 0030E36C  7C 7F 1B 78 */	mr r31, r3
/* 80311430 0030E370  93 C1 00 30 */	stw r30, 0x30(r1)
/* 80311434 0030E374  80 03 00 58 */	lwz r0, 0x58(r3)
/* 80311438 0030E378  2C 00 00 00 */	cmpwi r0, 0
/* 8031143C 0030E37C  41 82 02 18 */	beq lbl_80311654
/* 80311440 0030E380  80 7F 00 10 */	lwz r3, 0x10(r31)
/* 80311444 0030E384  3B C1 00 2C */	addi r30, r1, 0x2c
/* 80311448 0030E388  80 1F 00 14 */	lwz r0, 0x14(r31)
/* 8031144C 0030E38C  90 61 00 24 */	stw r3, 0x24(r1)
/* 80311450 0030E390  90 01 00 28 */	stw r0, 0x28(r1)
/* 80311454 0030E394  80 1F 00 18 */	lwz r0, 0x18(r31)
/* 80311458 0030E398  90 01 00 2C */	stw r0, 0x2c(r1)
/* 8031145C 0030E39C  C0 21 00 24 */	lfs f1, 0x24(r1)
/* 80311460 0030E3A0  C0 1F 00 44 */	lfs f0, 0x44(r31)
/* 80311464 0030E3A4  EC 01 00 2A */	fadds f0, f1, f0
/* 80311468 0030E3A8  D0 01 00 24 */	stfs f0, 0x24(r1)
/* 8031146C 0030E3AC  C0 21 00 28 */	lfs f1, 0x28(r1)
/* 80311470 0030E3B0  C0 1F 00 48 */	lfs f0, 0x48(r31)
/* 80311474 0030E3B4  EC 01 00 2A */	fadds f0, f1, f0
/* 80311478 0030E3B8  D0 01 00 28 */	stfs f0, 0x28(r1)
/* 8031147C 0030E3BC  C0 21 00 2C */	lfs f1, 0x2c(r1)
/* 80311480 0030E3C0  C0 1F 00 4C */	lfs f0, 0x4c(r31)
/* 80311484 0030E3C4  EC 01 00 2A */	fadds f0, f1, f0
/* 80311488 0030E3C8  D0 01 00 2C */	stfs f0, 0x2c(r1)
/* 8031148C 0030E3CC  80 1F 00 58 */	lwz r0, 0x58(r31)
/* 80311490 0030E3D0  2C 00 00 01 */	cmpwi r0, 1
/* 80311494 0030E3D4  40 82 01 80 */	bne lbl_80311614
/* 80311498 0030E3D8  C0 3F 00 48 */	lfs f1, 0x48(r31)
/* 8031149C 0030E3DC  38 81 00 20 */	addi r4, r1, 0x20
/* 803114A0 0030E3E0  C0 02 FA 4C */	lfs f0, "@2988"@sda21(r2)
/* 803114A4 0030E3E4  EC 01 00 28 */	fsubs f0, f1, f0
/* 803114A8 0030E3E8  D0 1F 00 48 */	stfs f0, 0x48(r31)
/* 803114AC 0030E3EC  80 6D 9A B0 */	lwz r3, gpMap@sda21(r13)
/* 803114B0 0030E3F0  C0 21 00 24 */	lfs f1, 0x24(r1)
/* 803114B4 0030E3F4  C0 5F 00 14 */	lfs f2, 0x14(r31)
/* 803114B8 0030E3F8  C0 61 00 2C */	lfs f3, 0x2c(r1)
/* 803114BC 0030E3FC  4B EC A2 7D */	bl checkGround__4TMapCFfffPPC12TBGCheckData
/* 803114C0 0030E400  80 61 00 20 */	lwz r3, 0x20(r1)
/* 803114C4 0030E404  C0 02 FA 40 */	lfs f0, "@2932"@sda21(r2)
/* 803114C8 0030E408  A0 03 00 04 */	lhz r0, 4(r3)
/* 803114CC 0030E40C  EC 21 00 2A */	fadds f1, f1, f0
/* 803114D0 0030E410  54 00 06 F7 */	rlwinm. r0, r0, 0, 0x1b, 0x1b
/* 803114D4 0030E414  41 82 00 0C */	beq lbl_803114E0
/* 803114D8 0030E418  38 00 00 01 */	li r0, 1
/* 803114DC 0030E41C  48 00 00 08 */	b lbl_803114E4
lbl_803114E0:
/* 803114E0 0030E420  38 00 00 00 */	li r0, 0
lbl_803114E4:
/* 803114E4 0030E424  54 00 06 3F */	clrlwi. r0, r0, 0x18
/* 803114E8 0030E428  41 82 00 10 */	beq lbl_803114F8
/* 803114EC 0030E42C  38 00 00 00 */	li r0, 0
/* 803114F0 0030E430  90 1F 00 58 */	stw r0, 0x58(r31)
/* 803114F4 0030E434  48 00 01 60 */	b lbl_80311654
lbl_803114F8:
/* 803114F8 0030E438  C0 01 00 28 */	lfs f0, 0x28(r1)
/* 803114FC 0030E43C  FC 00 08 40 */	fcmpo cr0, f0, f1
/* 80311500 0030E440  40 80 00 B0 */	bge lbl_803115B0
/* 80311504 0030E444  38 00 00 02 */	li r0, 2
/* 80311508 0030E448  90 1F 00 58 */	stw r0, 0x58(r31)
/* 8031150C 0030E44C  38 80 00 0C */	li r4, 0xc
/* 80311510 0030E450  D0 21 00 28 */	stfs f1, 0x28(r1)
/* 80311514 0030E454  C0 02 FA 44 */	lfs f0, "@2933"@sda21(r2)
/* 80311518 0030E458  D0 1F 00 4C */	stfs f0, 0x4c(r31)
/* 8031151C 0030E45C  D0 1F 00 48 */	stfs f0, 0x48(r31)
/* 80311520 0030E460  D0 1F 00 44 */	stfs f0, 0x44(r31)
/* 80311524 0030E464  80 7F 00 50 */	lwz r3, 0x50(r31)
/* 80311528 0030E468  4B DC 2B C9 */	bl checkCurBckFromIndex__6MActorFi
/* 8031152C 0030E46C  2C 03 00 00 */	cmpwi r3, 0
/* 80311530 0030E470  40 82 00 1C */	bne lbl_8031154C
/* 80311534 0030E474  80 7F 00 50 */	lwz r3, 0x50(r31)
/* 80311538 0030E478  38 80 00 0C */	li r4, 0xc
/* 8031153C 0030E47C  4B DC 2B E5 */	bl setBckFromIndex__6MActorFi
/* 80311540 0030E480  80 7F 00 54 */	lwz r3, 0x54(r31)
/* 80311544 0030E484  38 80 00 0D */	li r4, 0xd
/* 80311548 0030E488  4B DC 2B D9 */	bl setBckFromIndex__6MActorFi
lbl_8031154C:
/* 8031154C 0030E48C  80 6D 97 98 */	lwz r3, gpMarioParticleManager@sda21(r13)
/* 80311550 0030E490  38 A1 00 24 */	addi r5, r1, 0x24
/* 80311554 0030E494  38 80 00 9E */	li r4, 0x9e
/* 80311558 0030E498  38 C0 00 00 */	li r6, 0
/* 8031155C 0030E49C  38 E0 00 00 */	li r7, 0
/* 80311560 0030E4A0  4B DC A5 BD */	bl "emit__21TMarioParticleManagerFlPCQ29JGeometry8TVec3<f>UcPCv"
/* 80311564 0030E4A4  80 6D 97 98 */	lwz r3, gpMarioParticleManager@sda21(r13)
/* 80311568 0030E4A8  38 A1 00 24 */	addi r5, r1, 0x24
/* 8031156C 0030E4AC  38 80 00 9F */	li r4, 0x9f
/* 80311570 0030E4B0  38 C0 00 00 */	li r6, 0
/* 80311574 0030E4B4  38 E0 00 00 */	li r7, 0
/* 80311578 0030E4B8  4B DC A5 A5 */	bl "emit__21TMarioParticleManagerFlPCQ29JGeometry8TVec3<f>UcPCv"
/* 8031157C 0030E4BC  80 6D 97 EC */	lwz r3, gpMSound@sda21(r13)
/* 80311580 0030E4C0  38 80 28 71 */	li r4, 0x2871
/* 80311584 0030E4C4  4B E6 B4 CD */	bl gateCheck__6MSoundFUl
/* 80311588 0030E4C8  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 8031158C 0030E4CC  41 82 00 5C */	beq lbl_803115E8
/* 80311590 0030E4D0  38 9F 00 10 */	addi r4, r31, 0x10
/* 80311594 0030E4D4  38 60 28 71 */	li r3, 0x2871
/* 80311598 0030E4D8  38 A0 00 00 */	li r5, 0
/* 8031159C 0030E4DC  38 C0 00 00 */	li r6, 0
/* 803115A0 0030E4E0  38 E0 00 00 */	li r7, 0
/* 803115A4 0030E4E4  39 00 00 04 */	li r8, 4
/* 803115A8 0030E4E8  4B E7 09 51 */	bl startSoundActor__Q214MSoundSESystem8MSoundSEFUlPC3VecUlPP8JAISoundUlUc
/* 803115AC 0030E4EC  48 00 00 3C */	b lbl_803115E8
lbl_803115B0:
/* 803115B0 0030E4F0  80 7F 00 50 */	lwz r3, 0x50(r31)
/* 803115B4 0030E4F4  38 80 00 0B */	li r4, 0xb
/* 803115B8 0030E4F8  4B DC 2B 39 */	bl checkCurBckFromIndex__6MActorFi
/* 803115BC 0030E4FC  2C 03 00 00 */	cmpwi r3, 0
/* 803115C0 0030E500  41 82 00 1C */	beq lbl_803115DC
/* 803115C4 0030E504  80 7F 00 50 */	lwz r3, 0x50(r31)
/* 803115C8 0030E508  38 80 00 00 */	li r4, 0
/* 803115CC 0030E50C  38 A0 00 00 */	li r5, 0
/* 803115D0 0030E510  4B DC 3B DD */	bl curAnmEndsNext__6MActorFiPc
/* 803115D4 0030E514  2C 03 00 00 */	cmpwi r3, 0
/* 803115D8 0030E518  41 82 00 10 */	beq lbl_803115E8
lbl_803115DC:
/* 803115DC 0030E51C  80 7F 00 50 */	lwz r3, 0x50(r31)
/* 803115E0 0030E520  38 80 00 0B */	li r4, 0xb
/* 803115E4 0030E524  4B DC 2B 3D */	bl setBckFromIndex__6MActorFi
lbl_803115E8:
/* 803115E8 0030E528  80 6D 9A B0 */	lwz r3, gpMap@sda21(r13)
/* 803115EC 0030E52C  7F C5 F3 78 */	mr r5, r30
/* 803115F0 0030E530  C0 21 00 28 */	lfs f1, 0x28(r1)
/* 803115F4 0030E534  38 81 00 24 */	addi r4, r1, 0x24
/* 803115F8 0030E538  C0 42 FA 50 */	lfs f2, "@2989"@sda21(r2)
/* 803115FC 0030E53C  4B EC A3 21 */	bl isTouchedOneWallAndMoveXZ__4TMapCFPffPff
/* 80311600 0030E540  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 80311604 0030E544  41 82 00 38 */	beq lbl_8031163C
/* 80311608 0030E548  38 00 00 00 */	li r0, 0
/* 8031160C 0030E54C  90 1F 00 58 */	stw r0, 0x58(r31)
/* 80311610 0030E550  48 00 00 2C */	b lbl_8031163C
lbl_80311614:
/* 80311614 0030E554  2C 00 00 02 */	cmpwi r0, 2
/* 80311618 0030E558  40 82 00 24 */	bne lbl_8031163C
/* 8031161C 0030E55C  80 7F 00 50 */	lwz r3, 0x50(r31)
/* 80311620 0030E560  38 80 00 00 */	li r4, 0
/* 80311624 0030E564  38 A0 00 00 */	li r5, 0
/* 80311628 0030E568  4B DC 3B 85 */	bl curAnmEndsNext__6MActorFiPc
/* 8031162C 0030E56C  2C 03 00 00 */	cmpwi r3, 0
/* 80311630 0030E570  41 82 00 0C */	beq lbl_8031163C
/* 80311634 0030E574  38 00 00 00 */	li r0, 0
/* 80311638 0030E578  90 1F 00 58 */	stw r0, 0x58(r31)
lbl_8031163C:
/* 8031163C 0030E57C  80 61 00 24 */	lwz r3, 0x24(r1)
/* 80311640 0030E580  80 01 00 28 */	lwz r0, 0x28(r1)
/* 80311644 0030E584  90 7F 00 10 */	stw r3, 0x10(r31)
/* 80311648 0030E588  90 1F 00 14 */	stw r0, 0x14(r31)
/* 8031164C 0030E58C  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 80311650 0030E590  90 1F 00 18 */	stw r0, 0x18(r31)
lbl_80311654:
/* 80311654 0030E594  80 01 00 3C */	lwz r0, 0x3c(r1)
/* 80311658 0030E598  83 E1 00 34 */	lwz r31, 0x34(r1)
/* 8031165C 0030E59C  83 C1 00 30 */	lwz r30, 0x30(r1)
/* 80311660 0030E5A0  7C 08 03 A6 */	mtlr r0
/* 80311664 0030E5A4  38 21 00 38 */	addi r1, r1, 0x38
/* 80311668 0030E5A8  4E 80 00 20 */	blr 

.global __ct__10TBGPolDropFPCc
__ct__10TBGPolDropFPCc:
/* 8031166C 0030E5AC  7C 08 02 A6 */	mflr r0
/* 80311670 0030E5B0  90 01 00 04 */	stw r0, 4(r1)
/* 80311674 0030E5B4  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 80311678 0030E5B8  93 E1 00 24 */	stw r31, 0x24(r1)
/* 8031167C 0030E5BC  7C 7F 1B 78 */	mr r31, r3
/* 80311680 0030E5C0  93 C1 00 20 */	stw r30, 0x20(r1)
/* 80311684 0030E5C4  93 E1 00 10 */	stw r31, 0x10(r1)
/* 80311688 0030E5C8  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8031168C 0030E5CC  80 61 00 10 */	lwz r3, 0x10(r1)
/* 80311690 0030E5D0  4B D2 F4 79 */	bl __ct__Q26JDrama8TNameRefFPCc
/* 80311694 0030E5D4  80 61 00 10 */	lwz r3, 0x10(r1)
/* 80311698 0030E5D8  3C 80 80 3B */	lis r4, __vt__Q26JDrama8TViewObj@ha
/* 8031169C 0030E5DC  38 04 A0 C0 */	addi r0, r4, __vt__Q26JDrama8TViewObj@l
/* 803116A0 0030E5E0  90 03 00 00 */	stw r0, 0(r3)
/* 803116A4 0030E5E4  38 63 00 0C */	addi r3, r3, 0xc
/* 803116A8 0030E5E8  38 80 00 00 */	li r4, 0
/* 803116AC 0030E5EC  4B D2 F4 55 */	bl "__ct__Q26JDrama10TFlagT<Us>FUs"
/* 803116B0 0030E5F0  83 C1 00 14 */	lwz r30, 0x14(r1)
/* 803116B4 0030E5F4  3C 60 80 3B */	lis r3, __vt__Q26JDrama10TPlacement@ha
/* 803116B8 0030E5F8  38 03 A8 B0 */	addi r0, r3, __vt__Q26JDrama10TPlacement@l
/* 803116BC 0030E5FC  90 1E 00 00 */	stw r0, 0(r30)
/* 803116C0 0030E600  38 7E 00 10 */	addi r3, r30, 0x10
/* 803116C4 0030E604  C0 22 FA 44 */	lfs f1, "@2933"@sda21(r2)
/* 803116C8 0030E608  FC 40 08 90 */	fmr f2, f1
/* 803116CC 0030E60C  FC 60 08 90 */	fmr f3, f1
/* 803116D0 0030E610  48 00 00 A9 */	bl "set<f>__Q29JGeometry8TVec3<f>Ffff"
/* 803116D4 0030E614  38 A0 00 00 */	li r5, 0
/* 803116D8 0030E618  3C 60 80 3B */	lis r3, __vt__Q26JStage7TObject@ha
/* 803116DC 0030E61C  B0 BE 00 1C */	sth r5, 0x1c(r30)
/* 803116E0 0030E620  38 03 CE 98 */	addi r0, r3, __vt__Q26JStage7TObject@l
/* 803116E4 0030E624  3C 60 80 3B */	lis r3, __vt__Q26JStage6TActor@ha
/* 803116E8 0030E628  90 1F 00 20 */	stw r0, 0x20(r31)
/* 803116EC 0030E62C  38 03 CD D0 */	addi r0, r3, __vt__Q26JStage6TActor@l
/* 803116F0 0030E630  3C 60 80 3B */	lis r3, __vt__Q26JDrama6TActor@ha
/* 803116F4 0030E634  90 1F 00 20 */	stw r0, 0x20(r31)
/* 803116F8 0030E638  38 63 A0 20 */	addi r3, r3, __vt__Q26JDrama6TActor@l
/* 803116FC 0030E63C  90 7F 00 00 */	stw r3, 0(r31)
/* 80311700 0030E640  38 03 00 24 */	addi r0, r3, 0x24
/* 80311704 0030E644  3C 60 80 3E */	lis r3, __vt__10TBGPolDrop@ha
/* 80311708 0030E648  90 1F 00 20 */	stw r0, 0x20(r31)
/* 8031170C 0030E64C  38 83 04 60 */	addi r4, r3, __vt__10TBGPolDrop@l
/* 80311710 0030E650  38 04 00 24 */	addi r0, r4, 0x24
/* 80311714 0030E654  C0 02 FA 40 */	lfs f0, "@2932"@sda21(r2)
/* 80311718 0030E658  7F E3 FB 78 */	mr r3, r31
/* 8031171C 0030E65C  D0 1F 00 24 */	stfs f0, 0x24(r31)
/* 80311720 0030E660  D0 1F 00 28 */	stfs f0, 0x28(r31)
/* 80311724 0030E664  D0 1F 00 2C */	stfs f0, 0x2c(r31)
/* 80311728 0030E668  C0 02 FA 44 */	lfs f0, "@2933"@sda21(r2)
/* 8031172C 0030E66C  D0 1F 00 30 */	stfs f0, 0x30(r31)
/* 80311730 0030E670  D0 1F 00 34 */	stfs f0, 0x34(r31)
/* 80311734 0030E674  D0 1F 00 38 */	stfs f0, 0x38(r31)
/* 80311738 0030E678  90 BF 00 3C */	stw r5, 0x3c(r31)
/* 8031173C 0030E67C  90 BF 00 40 */	stw r5, 0x40(r31)
/* 80311740 0030E680  90 9F 00 00 */	stw r4, 0(r31)
/* 80311744 0030E684  90 1F 00 20 */	stw r0, 0x20(r31)
/* 80311748 0030E688  90 BF 00 50 */	stw r5, 0x50(r31)
/* 8031174C 0030E68C  90 BF 00 54 */	stw r5, 0x54(r31)
/* 80311750 0030E690  90 BF 00 58 */	stw r5, 0x58(r31)
/* 80311754 0030E694  D0 1F 00 4C */	stfs f0, 0x4c(r31)
/* 80311758 0030E698  D0 1F 00 48 */	stfs f0, 0x48(r31)
/* 8031175C 0030E69C  D0 1F 00 44 */	stfs f0, 0x44(r31)
/* 80311760 0030E6A0  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 80311764 0030E6A4  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 80311768 0030E6A8  83 C1 00 20 */	lwz r30, 0x20(r1)
/* 8031176C 0030E6AC  7C 08 03 A6 */	mtlr r0
/* 80311770 0030E6B0  38 21 00 28 */	addi r1, r1, 0x28
/* 80311774 0030E6B4  4E 80 00 20 */	blr 

"set<f>__Q29JGeometry8TVec3<f>Ffff":
/* 80311778 0030E6B8  D0 23 00 00 */	stfs f1, 0(r3)
/* 8031177C 0030E6BC  D0 43 00 04 */	stfs f2, 4(r3)
/* 80311780 0030E6C0  D0 63 00 08 */	stfs f3, 8(r3)
/* 80311784 0030E6C4  4E 80 00 20 */	blr 

.global __sinit_bgpoldrop_cpp
__sinit_bgpoldrop_cpp:
/* 80311788 0030E6C8  7C 08 02 A6 */	mflr r0
/* 8031178C 0030E6CC  3C 60 80 40 */	lis r3, "@3018"@ha
/* 80311790 0030E6D0  90 01 00 04 */	stw r0, 4(r1)
/* 80311794 0030E6D4  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 80311798 0030E6D8  93 E1 00 0C */	stw r31, 0xc(r1)
/* 8031179C 0030E6DC  3B E3 F3 48 */	addi r31, r3, "@3018"@l
/* 803117A0 0030E6E0  88 0D 97 CC */	lbz r0, "__init__smList__15JALList<5MSBgm>"@sda21(r13)
/* 803117A4 0030E6E4  7C 00 07 75 */	extsb. r0, r0
/* 803117A8 0030E6E8  40 82 00 28 */	bne lbl_803117D0
/* 803117AC 0030E6EC  38 6D 97 A8 */	addi r3, r13, "smList__15JALList<5MSBgm>"@sda21
/* 803117B0 0030E6F0  4B CF D4 85 */	bl initiate__10JSUPtrListFv
/* 803117B4 0030E6F4  3C 60 80 0E */	lis r3, "__dt__15JSUList<5MSBgm>Fv"@ha
/* 803117B8 0030E6F8  38 83 6A 44 */	addi r4, r3, "__dt__15JSUList<5MSBgm>Fv"@l
/* 803117BC 0030E6FC  38 6D 97 A8 */	addi r3, r13, "smList__15JALList<5MSBgm>"@sda21
/* 803117C0 0030E700  38 BF 00 00 */	addi r5, r31, 0
/* 803117C4 0030E704  4B D7 0F 65 */	bl __register_global_object
/* 803117C8 0030E708  38 00 00 01 */	li r0, 1
/* 803117CC 0030E70C  98 0D 97 CC */	stb r0, "__init__smList__15JALList<5MSBgm>"@sda21(r13)
lbl_803117D0:
/* 803117D0 0030E710  88 0D 97 CD */	lbz r0, "__init__smList__24JALList<13MSSetSoundGrp>"@sda21(r13)
/* 803117D4 0030E714  7C 00 07 75 */	extsb. r0, r0
/* 803117D8 0030E718  40 82 00 28 */	bne lbl_80311800
/* 803117DC 0030E71C  38 6D 97 B4 */	addi r3, r13, "smList__24JALList<13MSSetSoundGrp>"@sda21
/* 803117E0 0030E720  4B CF D4 55 */	bl initiate__10JSUPtrListFv
/* 803117E4 0030E724  3C 60 80 0E */	lis r3, "__dt__24JSUList<13MSSetSoundGrp>Fv"@ha
/* 803117E8 0030E728  38 83 69 EC */	addi r4, r3, "__dt__24JSUList<13MSSetSoundGrp>Fv"@l
/* 803117EC 0030E72C  38 6D 97 B4 */	addi r3, r13, "smList__24JALList<13MSSetSoundGrp>"@sda21
/* 803117F0 0030E730  38 BF 00 0C */	addi r5, r31, 0xc
/* 803117F4 0030E734  4B D7 0F 35 */	bl __register_global_object
/* 803117F8 0030E738  38 00 00 01 */	li r0, 1
/* 803117FC 0030E73C  98 0D 97 CD */	stb r0, "__init__smList__24JALList<13MSSetSoundGrp>"@sda21(r13)
lbl_80311800:
/* 80311800 0030E740  88 0D 97 CE */	lbz r0, "__init__smList__21JALList<10MSSetSound>"@sda21(r13)
/* 80311804 0030E744  7C 00 07 75 */	extsb. r0, r0
/* 80311808 0030E748  40 82 00 28 */	bne lbl_80311830
/* 8031180C 0030E74C  38 6D 97 C0 */	addi r3, r13, "smList__21JALList<10MSSetSound>"@sda21
/* 80311810 0030E750  4B CF D4 25 */	bl initiate__10JSUPtrListFv
/* 80311814 0030E754  3C 60 80 0E */	lis r3, "__dt__21JSUList<10MSSetSound>Fv"@ha
/* 80311818 0030E758  38 83 69 94 */	addi r4, r3, "__dt__21JSUList<10MSSetSound>Fv"@l
/* 8031181C 0030E75C  38 6D 97 C0 */	addi r3, r13, "smList__21JALList<10MSSetSound>"@sda21
/* 80311820 0030E760  38 BF 00 18 */	addi r5, r31, 0x18
/* 80311824 0030E764  4B D7 0F 05 */	bl __register_global_object
/* 80311828 0030E768  38 00 00 01 */	li r0, 1
/* 8031182C 0030E76C  98 0D 97 CE */	stb r0, "__init__smList__21JALList<10MSSetSound>"@sda21(r13)
lbl_80311830:
/* 80311830 0030E770  88 0D 8F 8C */	lbz r0, "__init__smList__26JALList<15JALSeModEffDGrp>"@sda21(r13)
/* 80311834 0030E774  7C 00 07 75 */	extsb. r0, r0
/* 80311838 0030E778  40 82 00 28 */	bne lbl_80311860
/* 8031183C 0030E77C  38 6D 8E FC */	addi r3, r13, "smList__26JALList<15JALSeModEffDGrp>"@sda21
/* 80311840 0030E780  4B CF D3 F5 */	bl initiate__10JSUPtrListFv
/* 80311844 0030E784  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModEffDGrp>Fv"@ha
/* 80311848 0030E788  38 83 A8 0C */	addi r4, r3, "__dt__26JSUList<15JALSeModEffDGrp>Fv"@l
/* 8031184C 0030E78C  38 6D 8E FC */	addi r3, r13, "smList__26JALList<15JALSeModEffDGrp>"@sda21
/* 80311850 0030E790  38 BF 00 24 */	addi r5, r31, 0x24
/* 80311854 0030E794  4B D7 0E D5 */	bl __register_global_object
/* 80311858 0030E798  38 00 00 01 */	li r0, 1
/* 8031185C 0030E79C  98 0D 8F 8C */	stb r0, "__init__smList__26JALList<15JALSeModEffDGrp>"@sda21(r13)
lbl_80311860:
/* 80311860 0030E7A0  88 0D 8F 8D */	lbz r0, "__init__smList__26JALList<15JALSeModPitDGrp>"@sda21(r13)
/* 80311864 0030E7A4  7C 00 07 75 */	extsb. r0, r0
/* 80311868 0030E7A8  40 82 00 28 */	bne lbl_80311890
/* 8031186C 0030E7AC  38 6D 8F 08 */	addi r3, r13, "smList__26JALList<15JALSeModPitDGrp>"@sda21
/* 80311870 0030E7B0  4B CF D3 C5 */	bl initiate__10JSUPtrListFv
/* 80311874 0030E7B4  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModPitDGrp>Fv"@ha
/* 80311878 0030E7B8  38 83 A7 B4 */	addi r4, r3, "__dt__26JSUList<15JALSeModPitDGrp>Fv"@l
/* 8031187C 0030E7BC  38 6D 8F 08 */	addi r3, r13, "smList__26JALList<15JALSeModPitDGrp>"@sda21
/* 80311880 0030E7C0  38 BF 00 30 */	addi r5, r31, 0x30
/* 80311884 0030E7C4  4B D7 0E A5 */	bl __register_global_object
/* 80311888 0030E7C8  38 00 00 01 */	li r0, 1
/* 8031188C 0030E7CC  98 0D 8F 8D */	stb r0, "__init__smList__26JALList<15JALSeModPitDGrp>"@sda21(r13)
lbl_80311890:
/* 80311890 0030E7D0  88 0D 8F 8E */	lbz r0, "__init__smList__26JALList<15JALSeModVolDGrp>"@sda21(r13)
/* 80311894 0030E7D4  7C 00 07 75 */	extsb. r0, r0
/* 80311898 0030E7D8  40 82 00 28 */	bne lbl_803118C0
/* 8031189C 0030E7DC  38 6D 8F 14 */	addi r3, r13, "smList__26JALList<15JALSeModVolDGrp>"@sda21
/* 803118A0 0030E7E0  4B CF D3 95 */	bl initiate__10JSUPtrListFv
/* 803118A4 0030E7E4  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModVolDGrp>Fv"@ha
/* 803118A8 0030E7E8  38 83 A7 5C */	addi r4, r3, "__dt__26JSUList<15JALSeModVolDGrp>Fv"@l
/* 803118AC 0030E7EC  38 6D 8F 14 */	addi r3, r13, "smList__26JALList<15JALSeModVolDGrp>"@sda21
/* 803118B0 0030E7F0  38 BF 00 3C */	addi r5, r31, 0x3c
/* 803118B4 0030E7F4  4B D7 0E 75 */	bl __register_global_object
/* 803118B8 0030E7F8  38 00 00 01 */	li r0, 1
/* 803118BC 0030E7FC  98 0D 8F 8E */	stb r0, "__init__smList__26JALList<15JALSeModVolDGrp>"@sda21(r13)
lbl_803118C0:
/* 803118C0 0030E800  88 0D 8F 8F */	lbz r0, "__init__smList__26JALList<15JALSeModEffFGrp>"@sda21(r13)
/* 803118C4 0030E804  7C 00 07 75 */	extsb. r0, r0
/* 803118C8 0030E808  40 82 00 28 */	bne lbl_803118F0
/* 803118CC 0030E80C  38 6D 8F 20 */	addi r3, r13, "smList__26JALList<15JALSeModEffFGrp>"@sda21
/* 803118D0 0030E810  4B CF D3 65 */	bl initiate__10JSUPtrListFv
/* 803118D4 0030E814  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModEffFGrp>Fv"@ha
/* 803118D8 0030E818  38 83 A7 04 */	addi r4, r3, "__dt__26JSUList<15JALSeModEffFGrp>Fv"@l
/* 803118DC 0030E81C  38 6D 8F 20 */	addi r3, r13, "smList__26JALList<15JALSeModEffFGrp>"@sda21
/* 803118E0 0030E820  38 BF 00 48 */	addi r5, r31, 0x48
/* 803118E4 0030E824  4B D7 0E 45 */	bl __register_global_object
/* 803118E8 0030E828  38 00 00 01 */	li r0, 1
/* 803118EC 0030E82C  98 0D 8F 8F */	stb r0, "__init__smList__26JALList<15JALSeModEffFGrp>"@sda21(r13)
lbl_803118F0:
/* 803118F0 0030E830  88 0D 8F 90 */	lbz r0, "__init__smList__26JALList<15JALSeModPitFGrp>"@sda21(r13)
/* 803118F4 0030E834  7C 00 07 75 */	extsb. r0, r0
/* 803118F8 0030E838  40 82 00 28 */	bne lbl_80311920
/* 803118FC 0030E83C  38 6D 8F 2C */	addi r3, r13, "smList__26JALList<15JALSeModPitFGrp>"@sda21
/* 80311900 0030E840  4B CF D3 35 */	bl initiate__10JSUPtrListFv
/* 80311904 0030E844  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModPitFGrp>Fv"@ha
/* 80311908 0030E848  38 83 A6 AC */	addi r4, r3, "__dt__26JSUList<15JALSeModPitFGrp>Fv"@l
/* 8031190C 0030E84C  38 6D 8F 2C */	addi r3, r13, "smList__26JALList<15JALSeModPitFGrp>"@sda21
/* 80311910 0030E850  38 BF 00 54 */	addi r5, r31, 0x54
/* 80311914 0030E854  4B D7 0E 15 */	bl __register_global_object
/* 80311918 0030E858  38 00 00 01 */	li r0, 1
/* 8031191C 0030E85C  98 0D 8F 90 */	stb r0, "__init__smList__26JALList<15JALSeModPitFGrp>"@sda21(r13)
lbl_80311920:
/* 80311920 0030E860  88 0D 8F 91 */	lbz r0, "__init__smList__26JALList<15JALSeModVolFGrp>"@sda21(r13)
/* 80311924 0030E864  7C 00 07 75 */	extsb. r0, r0
/* 80311928 0030E868  40 82 00 28 */	bne lbl_80311950
/* 8031192C 0030E86C  38 6D 8F 38 */	addi r3, r13, "smList__26JALList<15JALSeModVolFGrp>"@sda21
/* 80311930 0030E870  4B CF D3 05 */	bl initiate__10JSUPtrListFv
/* 80311934 0030E874  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModVolFGrp>Fv"@ha
/* 80311938 0030E878  38 83 A6 54 */	addi r4, r3, "__dt__26JSUList<15JALSeModVolFGrp>Fv"@l
/* 8031193C 0030E87C  38 6D 8F 38 */	addi r3, r13, "smList__26JALList<15JALSeModVolFGrp>"@sda21
/* 80311940 0030E880  38 BF 00 60 */	addi r5, r31, 0x60
/* 80311944 0030E884  4B D7 0D E5 */	bl __register_global_object
/* 80311948 0030E888  38 00 00 01 */	li r0, 1
/* 8031194C 0030E88C  98 0D 8F 91 */	stb r0, "__init__smList__26JALList<15JALSeModVolFGrp>"@sda21(r13)
lbl_80311950:
/* 80311950 0030E890  88 0D 8F 92 */	lbz r0, "__init__smList__26JALList<15JALSeModEffDist>"@sda21(r13)
/* 80311954 0030E894  7C 00 07 75 */	extsb. r0, r0
/* 80311958 0030E898  40 82 00 28 */	bne lbl_80311980
/* 8031195C 0030E89C  38 6D 8F 44 */	addi r3, r13, "smList__26JALList<15JALSeModEffDist>"@sda21
/* 80311960 0030E8A0  4B CF D2 D5 */	bl initiate__10JSUPtrListFv
/* 80311964 0030E8A4  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModEffDist>Fv"@ha
/* 80311968 0030E8A8  38 83 A5 FC */	addi r4, r3, "__dt__26JSUList<15JALSeModEffDist>Fv"@l
/* 8031196C 0030E8AC  38 6D 8F 44 */	addi r3, r13, "smList__26JALList<15JALSeModEffDist>"@sda21
/* 80311970 0030E8B0  38 BF 00 6C */	addi r5, r31, 0x6c
/* 80311974 0030E8B4  4B D7 0D B5 */	bl __register_global_object
/* 80311978 0030E8B8  38 00 00 01 */	li r0, 1
/* 8031197C 0030E8BC  98 0D 8F 92 */	stb r0, "__init__smList__26JALList<15JALSeModEffDist>"@sda21(r13)
lbl_80311980:
/* 80311980 0030E8C0  88 0D 8F 93 */	lbz r0, "__init__smList__26JALList<15JALSeModPitDist>"@sda21(r13)
/* 80311984 0030E8C4  7C 00 07 75 */	extsb. r0, r0
/* 80311988 0030E8C8  40 82 00 28 */	bne lbl_803119B0
/* 8031198C 0030E8CC  38 6D 8F 50 */	addi r3, r13, "smList__26JALList<15JALSeModPitDist>"@sda21
/* 80311990 0030E8D0  4B CF D2 A5 */	bl initiate__10JSUPtrListFv
/* 80311994 0030E8D4  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModPitDist>Fv"@ha
/* 80311998 0030E8D8  38 83 A5 A4 */	addi r4, r3, "__dt__26JSUList<15JALSeModPitDist>Fv"@l
/* 8031199C 0030E8DC  38 6D 8F 50 */	addi r3, r13, "smList__26JALList<15JALSeModPitDist>"@sda21
/* 803119A0 0030E8E0  38 BF 00 78 */	addi r5, r31, 0x78
/* 803119A4 0030E8E4  4B D7 0D 85 */	bl __register_global_object
/* 803119A8 0030E8E8  38 00 00 01 */	li r0, 1
/* 803119AC 0030E8EC  98 0D 8F 93 */	stb r0, "__init__smList__26JALList<15JALSeModPitDist>"@sda21(r13)
lbl_803119B0:
/* 803119B0 0030E8F0  88 0D 8F 94 */	lbz r0, "__init__smList__26JALList<15JALSeModVolDist>"@sda21(r13)
/* 803119B4 0030E8F4  7C 00 07 75 */	extsb. r0, r0
/* 803119B8 0030E8F8  40 82 00 28 */	bne lbl_803119E0
/* 803119BC 0030E8FC  38 6D 8F 5C */	addi r3, r13, "smList__26JALList<15JALSeModVolDist>"@sda21
/* 803119C0 0030E900  4B CF D2 75 */	bl initiate__10JSUPtrListFv
/* 803119C4 0030E904  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModVolDist>Fv"@ha
/* 803119C8 0030E908  38 83 A5 4C */	addi r4, r3, "__dt__26JSUList<15JALSeModVolDist>Fv"@l
/* 803119CC 0030E90C  38 6D 8F 5C */	addi r3, r13, "smList__26JALList<15JALSeModVolDist>"@sda21
/* 803119D0 0030E910  38 BF 00 84 */	addi r5, r31, 0x84
/* 803119D4 0030E914  4B D7 0D 55 */	bl __register_global_object
/* 803119D8 0030E918  38 00 00 01 */	li r0, 1
/* 803119DC 0030E91C  98 0D 8F 94 */	stb r0, "__init__smList__26JALList<15JALSeModVolDist>"@sda21(r13)
lbl_803119E0:
/* 803119E0 0030E920  88 0D 8F 95 */	lbz r0, "__init__smList__26JALList<15JALSeModEffFunk>"@sda21(r13)
/* 803119E4 0030E924  7C 00 07 75 */	extsb. r0, r0
/* 803119E8 0030E928  40 82 00 28 */	bne lbl_80311A10
/* 803119EC 0030E92C  38 6D 8F 68 */	addi r3, r13, "smList__26JALList<15JALSeModEffFunk>"@sda21
/* 803119F0 0030E930  4B CF D2 45 */	bl initiate__10JSUPtrListFv
/* 803119F4 0030E934  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModEffFunk>Fv"@ha
/* 803119F8 0030E938  38 83 A4 F4 */	addi r4, r3, "__dt__26JSUList<15JALSeModEffFunk>Fv"@l
/* 803119FC 0030E93C  38 6D 8F 68 */	addi r3, r13, "smList__26JALList<15JALSeModEffFunk>"@sda21
/* 80311A00 0030E940  38 BF 00 90 */	addi r5, r31, 0x90
/* 80311A04 0030E944  4B D7 0D 25 */	bl __register_global_object
/* 80311A08 0030E948  38 00 00 01 */	li r0, 1
/* 80311A0C 0030E94C  98 0D 8F 95 */	stb r0, "__init__smList__26JALList<15JALSeModEffFunk>"@sda21(r13)
lbl_80311A10:
/* 80311A10 0030E950  88 0D 8F 96 */	lbz r0, "__init__smList__26JALList<15JALSeModPitFunk>"@sda21(r13)
/* 80311A14 0030E954  7C 00 07 75 */	extsb. r0, r0
/* 80311A18 0030E958  40 82 00 28 */	bne lbl_80311A40
/* 80311A1C 0030E95C  38 6D 8F 74 */	addi r3, r13, "smList__26JALList<15JALSeModPitFunk>"@sda21
/* 80311A20 0030E960  4B CF D2 15 */	bl initiate__10JSUPtrListFv
/* 80311A24 0030E964  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModPitFunk>Fv"@ha
/* 80311A28 0030E968  38 83 A4 9C */	addi r4, r3, "__dt__26JSUList<15JALSeModPitFunk>Fv"@l
/* 80311A2C 0030E96C  38 6D 8F 74 */	addi r3, r13, "smList__26JALList<15JALSeModPitFunk>"@sda21
/* 80311A30 0030E970  38 BF 00 9C */	addi r5, r31, 0x9c
/* 80311A34 0030E974  4B D7 0C F5 */	bl __register_global_object
/* 80311A38 0030E978  38 00 00 01 */	li r0, 1
/* 80311A3C 0030E97C  98 0D 8F 96 */	stb r0, "__init__smList__26JALList<15JALSeModPitFunk>"@sda21(r13)
lbl_80311A40:
/* 80311A40 0030E980  88 0D 8F 97 */	lbz r0, "__init__smList__26JALList<15JALSeModVolFunk>"@sda21(r13)
/* 80311A44 0030E984  7C 00 07 75 */	extsb. r0, r0
/* 80311A48 0030E988  40 82 00 28 */	bne lbl_80311A70
/* 80311A4C 0030E98C  38 6D 8F 80 */	addi r3, r13, "smList__26JALList<15JALSeModVolFunk>"@sda21
/* 80311A50 0030E990  4B CF D1 E5 */	bl initiate__10JSUPtrListFv
/* 80311A54 0030E994  3C 60 80 05 */	lis r3, "__dt__26JSUList<15JALSeModVolFunk>Fv"@ha
/* 80311A58 0030E998  38 83 A4 44 */	addi r4, r3, "__dt__26JSUList<15JALSeModVolFunk>Fv"@l
/* 80311A5C 0030E99C  38 6D 8F 80 */	addi r3, r13, "smList__26JALList<15JALSeModVolFunk>"@sda21
/* 80311A60 0030E9A0  38 BF 00 A8 */	addi r5, r31, 0xa8
/* 80311A64 0030E9A4  4B D7 0C C5 */	bl __register_global_object
/* 80311A68 0030E9A8  38 00 00 01 */	li r0, 1
/* 80311A6C 0030E9AC  98 0D 8F 97 */	stb r0, "__init__smList__26JALList<15JALSeModVolFunk>"@sda21(r13)
lbl_80311A70:
/* 80311A70 0030E9B0  80 01 00 14 */	lwz r0, 0x14(r1)
/* 80311A74 0030E9B4  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 80311A78 0030E9B8  38 21 00 10 */	addi r1, r1, 0x10
/* 80311A7C 0030E9BC  7C 08 03 A6 */	mtlr r0
/* 80311A80 0030E9C0  4E 80 00 20 */	blr 

.global "@32@__dt__10TBGPolDropFv"
"@32@__dt__10TBGPolDropFv":
/* 80311A84 0030E9C4  38 63 FF E0 */	addi r3, r3, -32
/* 80311A88 0030E9C8  4B FF F6 98 */	b __dt__10TBGPolDropFv

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
lbl_constructor:
  .4byte __sinit_bgpoldrop_cpp

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__10TBGPolDrop
__vt__10TBGPolDrop:
  .4byte 0
  .4byte 0
  .4byte __dt__10TBGPolDropFv
  .4byte getType__Q26JDrama6TActorCFv
  .4byte load__Q26JDrama6TActorFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__Q26JDrama8TNameRefFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc
  .4byte perform__10TBGPolDropFUlPQ26JDrama9TGraphics
  .4byte 0
  .4byte 0
  .4byte "@32@__dt__10TBGPolDropFv"
  .4byte JSGFGetType__Q26JStage6TActorCFv
  .4byte JSGGetName__Q26JStage7TObjectCFv
  .4byte JSGGetFlag__Q26JStage7TObjectCFv
  .4byte JSGSetFlag__Q26JStage7TObjectFUl
  .4byte JSGGetData__Q26JStage7TObjectCFUlPvUl
  .4byte JSGSetData__Q26JStage7TObjectFUlPCvUl
  .4byte JSGGetParent__Q26JStage7TObjectCFPPQ26JStage7TObjectPUl
  .4byte JSGSetParent__Q26JStage7TObjectFPQ26JStage7TObjectUl
  .4byte JSGSetRelation__Q26JStage7TObjectFbPQ26JStage7TObjectUl
  .4byte "@32@JSGGetTranslation__Q26JDrama6TActorCFP3Vec"
  .4byte "@32@JSGSetTranslation__Q26JDrama6TActorFRC3Vec"
  .4byte "@32@JSGGetScaling__Q26JDrama6TActorCFP3Vec"
  .4byte "@32@JSGSetScaling__Q26JDrama6TActorFRC3Vec"
  .4byte "@32@JSGGetRotation__Q26JDrama6TActorCFP3Vec"
  .4byte "@32@JSGSetRotation__Q26JDrama6TActorFRC3Vec"
  .4byte JSGGetShape__Q26JStage6TActorCFv
  .4byte JSGSetShape__Q26JStage6TActorFUl
  .4byte JSGGetAnimation__Q26JStage6TActorCFv
  .4byte JSGSetAnimation__Q26JStage6TActorFUl
  .4byte JSGGetAnimationFrame__Q26JStage6TActorCFv
  .4byte JSGSetAnimationFrame__Q26JStage6TActorFf
  .4byte JSGGetAnimationFrameMax__Q26JStage6TActorCFv
  .4byte JSGGetTranslation__Q26JDrama6TActorCFP3Vec
  .4byte JSGSetTranslation__Q26JDrama6TActorFRC3Vec
  .4byte JSGGetScaling__Q26JDrama6TActorCFP3Vec
  .4byte JSGSetScaling__Q26JDrama6TActorFRC3Vec
  .4byte JSGGetRotation__Q26JDrama6TActorCFP3Vec
  .4byte JSGSetRotation__Q26JDrama6TActorFRC3Vec

.section .sdata2, "a"  # 0x8040B460 - 0x80414020
.balign 8
"@2932":
	.4byte 0x3F800000
"@2933":
  .4byte 0
"@2934":
	.4byte 0x43360B61
"@2988":
	.4byte 0x3E4CCCCD
"@2989":
	.4byte 0x42A00000
	.4byte 0

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
"@3018":
	.skip 0xC
"@3019":
	.skip 0xC
"@3020":
	.skip 0xC
"@3021":
	.skip 0xC
"@3022":
	.skip 0xC
"@3023":
	.skip 0xC
"@3024":
	.skip 0xC
"@3025":
	.skip 0xC
"@3026":
	.skip 0xC
"@3027":
	.skip 0xC
"@3028":
	.skip 0xC
"@3029":
	.skip 0xC
"@3030":
	.skip 0xC
"@3031":
	.skip 0xC
"@3032":
	.skip 0x10

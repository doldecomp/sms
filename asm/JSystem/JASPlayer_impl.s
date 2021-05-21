.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global extend8to16__Q28JASystem6PlayerFUc
extend8to16__Q28JASystem6PlayerFUc:
/* 800646E8 00061628  54 60 06 31 */	rlwinm. r0, r3, 0, 0x18, 0x18
/* 800646EC 0006162C  54 64 06 3E */	clrlwi r4, r3, 0x18
/* 800646F0 00061630  41 82 00 10 */	beq lbl_80064700
/* 800646F4 00061634  3C 64 00 01 */	addis r3, r4, 1
/* 800646F8 00061638  38 63 FF 00 */	addi r3, r3, -256
/* 800646FC 0006163C  4E 80 00 20 */	blr 
lbl_80064700:
/* 80064700 00061640  7C 83 23 78 */	mr r3, r4
/* 80064704 00061644  4E 80 00 20 */	blr 

.global pitchToCent__Q28JASystem6PlayerFff
pitchToCent__Q28JASystem6PlayerFff:
/* 80064708 00061648  94 21 FF D0 */	stwu r1, -0x30(r1)
/* 8006470C 0006164C  3C 00 43 30 */	lis r0, 0x4330
/* 80064710 00061650  C0 02 87 08 */	lfs f0, $$2127@sda21(r2)
/* 80064714 00061654  C8 62 87 18 */	lfd f3, $$2134@sda21(r2)
/* 80064718 00061658  EC 20 00 72 */	fmuls f1, f0, f1
/* 8006471C 0006165C  C0 02 87 0C */	lfs f0, $$2128@sda21(r2)
/* 80064720 00061660  EC 41 00 B2 */	fmuls f2, f1, f2
/* 80064724 00061664  FC 20 10 1E */	fctiwz f1, f2
/* 80064728 00061668  FC 02 00 40 */	fcmpo cr0, f2, f0
/* 8006472C 0006166C  D8 21 00 20 */	stfd f1, 0x20(r1)
/* 80064730 00061670  80 61 00 24 */	lwz r3, 0x24(r1)
/* 80064734 00061674  D8 21 00 28 */	stfd f1, 0x28(r1)
/* 80064738 00061678  7C 63 07 34 */	extsh r3, r3
/* 8006473C 0006167C  6C 63 80 00 */	xoris r3, r3, 0x8000
/* 80064740 00061680  80 81 00 2C */	lwz r4, 0x2c(r1)
/* 80064744 00061684  90 61 00 1C */	stw r3, 0x1c(r1)
/* 80064748 00061688  90 01 00 18 */	stw r0, 0x18(r1)
/* 8006474C 0006168C  C8 21 00 18 */	lfd f1, 0x18(r1)
/* 80064750 00061690  EC 21 18 28 */	fsubs f1, f1, f3
/* 80064754 00061694  EC 22 08 28 */	fsubs f1, f2, f1
/* 80064758 00061698  40 80 00 18 */	bge lbl_80064770
/* 8006475C 0006169C  FC 00 08 00 */	fcmpu cr0, f0, f1
/* 80064760 000616A0  41 82 00 10 */	beq lbl_80064770
/* 80064764 000616A4  C0 02 87 10 */	lfs f0, $$2129@sda21(r2)
/* 80064768 000616A8  38 84 FF FF */	addi r4, r4, -1
/* 8006476C 000616AC  EC 21 00 2A */	fadds f1, f1, f0
lbl_80064770:
/* 80064770 000616B0  C0 02 87 10 */	lfs f0, $$2129@sda21(r2)
/* 80064774 000616B4  FC 01 00 40 */	fcmpo cr0, f1, f0
/* 80064778 000616B8  4C 41 13 82 */	cror 2, 1, 2
/* 8006477C 000616BC  40 82 00 0C */	bne lbl_80064788
/* 80064780 000616C0  EC 21 00 28 */	fsubs f1, f1, f0
/* 80064784 000616C4  38 84 00 01 */	addi r4, r4, 1
lbl_80064788:
/* 80064788 000616C8  C0 02 87 14 */	lfs f0, $$2130@sda21(r2)
/* 8006478C 000616CC  7C 80 07 34 */	extsh r0, r4
/* 80064790 000616D0  3C 60 80 3B */	lis r3, C5BASE_PITCHTABLE__Q28JASystem6Driver@ha
/* 80064794 000616D4  EC 00 00 72 */	fmuls f0, f0, f1
/* 80064798 000616D8  3C 80 80 3B */	lis r4, s_key_table__Q28JASystem6Player@ha
/* 8006479C 000616DC  38 63 B0 00 */	addi r3, r3, C5BASE_PITCHTABLE__Q28JASystem6Driver@l
/* 800647A0 000616E0  54 00 10 3A */	slwi r0, r0, 2
/* 800647A4 000616E4  FC 00 00 1E */	fctiwz f0, f0
/* 800647A8 000616E8  7C 63 02 14 */	add r3, r3, r0
/* 800647AC 000616EC  38 84 BC 08 */	addi r4, r4, s_key_table__Q28JASystem6Player@l
/* 800647B0 000616F0  D8 01 00 18 */	stfd f0, 0x18(r1)
/* 800647B4 000616F4  C0 03 00 F0 */	lfs f0, 0xf0(r3)
/* 800647B8 000616F8  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 800647BC 000616FC  38 21 00 30 */	addi r1, r1, 0x30
/* 800647C0 00061700  54 00 13 BA */	rlwinm r0, r0, 2, 0xe, 0x1d
/* 800647C4 00061704  7C 64 02 14 */	add r3, r4, r0
/* 800647C8 00061708  C0 23 00 00 */	lfs f1, 0(r3)
/* 800647CC 0006170C  EC 21 00 32 */	fmuls f1, f1, f0
/* 800647D0 00061710  4E 80 00 20 */	blr 

.global getRandomS32__Q28JASystem6PlayerFv
getRandomS32__Q28JASystem6PlayerFv:
/* 800647D4 00061714  3C 60 13 58 */	lis r3, 0x13579BDE@ha
/* 800647D8 00061718  80 8D 82 10 */	lwz r4, sV0__Q28JASystem6Player@sda21(r13)
/* 800647DC 0006171C  38 03 9B DE */	addi r0, r3, 0x13579BDE@l
/* 800647E0 00061720  80 AD 82 14 */	lwz r5, sV1__Q28JASystem6Player@sda21(r13)
/* 800647E4 00061724  3C 60 98 76 */	lis r3, 0x98765432@ha
/* 800647E8 00061728  38 63 54 32 */	addi r3, r3, 0x98765432@l
/* 800647EC 0006172C  90 AD 82 10 */	stw r5, sV0__Q28JASystem6Player@sda21(r13)
/* 800647F0 00061730  7C 04 01 D6 */	mullw r0, r4, r0
/* 800647F4 00061734  7C 65 19 D6 */	mullw r3, r5, r3
/* 800647F8 00061738  7C 00 26 70 */	srawi r0, r0, 4
/* 800647FC 0006173C  7C 63 02 14 */	add r3, r3, r0
/* 80064800 00061740  38 63 00 01 */	addi r3, r3, 1
/* 80064804 00061744  90 6D 82 14 */	stw r3, sV1__Q28JASystem6Player@sda21(r13)
/* 80064808 00061748  4E 80 00 20 */	blr 

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global CUTOFF_TO_IIR_TABLE__Q28JASystem6Player
CUTOFF_TO_IIR_TABLE__Q28JASystem6Player:
	.incbin "baserom.dol", 0x3A8808, 0x400
s_key_table__Q28JASystem6Player:
	.incbin "baserom.dol", 0x3A8C08, 0x100
.global sAdsrDef__Q28JASystem6Player
sAdsrDef__Q28JASystem6Player:
	.incbin "baserom.dol", 0x3A8D08, 0x18
.global sEnvelopeDef__Q28JASystem6Player
sEnvelopeDef__Q28JASystem6Player:
	.incbin "baserom.dol", 0x3A8D20, 0x18
.global sVibratoDef__Q28JASystem6Player
sVibratoDef__Q28JASystem6Player:
	.incbin "baserom.dol", 0x3A8D38, 0x18
.global sTremoroDef__Q28JASystem6Player
sTremoroDef__Q28JASystem6Player:
	.incbin "baserom.dol", 0x3A8D50, 0x18
.global sAdsTable__Q28JASystem6Player
sAdsTable__Q28JASystem6Player:
	.incbin "baserom.dol", 0x3A8D68, 0x18
.global sRelTable__Q28JASystem6Player
sRelTable__Q28JASystem6Player:
	.incbin "baserom.dol", 0x3A8D80, 0xC
sVibTable__Q28JASystem6Player:
	.incbin "baserom.dol", 0x3A8D8C, 0x24
sTreTable__Q28JASystem6Player:
	.incbin "baserom.dol", 0x3A8DB0, 0x28

.section .sdata, "wa"  # 0x80408AC0 - 0x804097C0
sV0__Q28JASystem6Player:
	.incbin "baserom.dol", 0x3E3210, 0x4
sV1__Q28JASystem6Player:
	.incbin "baserom.dol", 0x3E3214, 0x4

.section .sdata2, "wa"  # 0x8040B460 - 0x80414020
$$2127:
	.incbin "baserom.dol", 0x3E4408, 0x4
$$2128:
	.incbin "baserom.dol", 0x3E440C, 0x4
$$2129:
	.incbin "baserom.dol", 0x3E4410, 0x4
$$2130:
	.incbin "baserom.dol", 0x3E4414, 0x4
$$2134:
	.incbin "baserom.dol", 0x3E4418, 0x8

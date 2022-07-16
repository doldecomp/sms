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
/* 80064710 00061650  C0 02 87 08 */	lfs f0, "@127"@sda21(r2)
/* 80064714 00061654  C8 62 87 18 */	lfd f3, "@134"@sda21(r2)
/* 80064718 00061658  EC 20 00 72 */	fmuls f1, f0, f1
/* 8006471C 0006165C  C0 02 87 0C */	lfs f0, "@128"@sda21(r2)
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
/* 80064764 000616A4  C0 02 87 10 */	lfs f0, "@129"@sda21(r2)
/* 80064768 000616A8  38 84 FF FF */	addi r4, r4, -1
/* 8006476C 000616AC  EC 21 00 2A */	fadds f1, f1, f0
lbl_80064770:
/* 80064770 000616B0  C0 02 87 10 */	lfs f0, "@129"@sda21(r2)
/* 80064774 000616B4  FC 01 00 40 */	fcmpo cr0, f1, f0
/* 80064778 000616B8  4C 41 13 82 */	cror 2, 1, 2
/* 8006477C 000616BC  40 82 00 0C */	bne lbl_80064788
/* 80064780 000616C0  EC 21 00 28 */	fsubs f1, f1, f0
/* 80064784 000616C4  38 84 00 01 */	addi r4, r4, 1
lbl_80064788:
/* 80064788 000616C8  C0 02 87 14 */	lfs f0, "@130"@sda21(r2)
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
    .balign 8
.global CUTOFF_TO_IIR_TABLE__Q28JASystem6Player
CUTOFF_TO_IIR_TABLE__Q28JASystem6Player:
	.4byte 0x0F5C0A3D
	.4byte 0x46653999
	.4byte 0x103F0A28
	.4byte 0x45D73925
	.4byte 0x11220A14
	.4byte 0x454A38B0
	.4byte 0x120509FF
	.4byte 0x44BC383C
	.4byte 0x12E809EA
	.4byte 0x442E37C8
	.4byte 0x13CB09D6
	.4byte 0x43A03754
	.4byte 0x14AE09C1
	.4byte 0x431236E0
	.4byte 0x159109AC
	.4byte 0x4284366C
	.4byte 0x16740998
	.4byte 0x41F635F8
	.4byte 0x17570983
	.4byte 0x41683584
	.4byte 0x183A096E
	.4byte 0x40DA3510
	.4byte 0x191D095A
	.4byte 0x404C349C
	.4byte 0x1A000945
	.4byte 0x3FBE3427
	.4byte 0x1AE30931
	.4byte 0x3F3133B3
	.4byte 0x1BC6091C
	.4byte 0x3EA3333F
	.4byte 0x1CA90907
	.4byte 0x3E1532CB
	.4byte 0x1D8C08F3
	.4byte 0x3D873257
	.4byte 0x1E6F08DE
	.4byte 0x3CF931E3
	.4byte 0x1F5208C9
	.4byte 0x3C6B316F
	.4byte 0x203508B5
	.4byte 0x3BDD30FB
	.4byte 0x211808A0
	.4byte 0x3B4F3087
	.4byte 0x21FC088B
	.4byte 0x3AC13012
	.4byte 0x22DF0877
	.4byte 0x3A332F9E
	.4byte 0x23C20862
	.4byte 0x39A62F2A
	.4byte 0x24A5084D
	.4byte 0x39182EB6
	.4byte 0x25880839
	.4byte 0x388A2E42
	.4byte 0x266B0824
	.4byte 0x37FC2DCE
	.4byte 0x274E0810
	.4byte 0x376E2D5A
	.4byte 0x283107FB
	.4byte 0x36E02CE6
	.4byte 0x291407E6
	.4byte 0x36522C72
	.4byte 0x29F707D2
	.4byte 0x35C42BFE
	.4byte 0x2ADA07BD
	.4byte 0x35362B89
	.4byte 0x2BBD07A8
	.4byte 0x34A82B15
	.4byte 0x2CA00794
	.4byte 0x341B2AA1
	.4byte 0x2D83077F
	.4byte 0x338D2A2D
	.4byte 0x2E66076A
	.4byte 0x32FF29B9
	.4byte 0x2F490756
	.4byte 0x32712945
	.4byte 0x302C0741
	.4byte 0x31E328D1
	.4byte 0x310F072D
	.4byte 0x3155285D
	.4byte 0x31F20718
	.4byte 0x30C727E9
	.4byte 0x32D50703
	.4byte 0x30392775
	.4byte 0x33B806EF
	.4byte 0x2FAB2700
	.4byte 0x349C06DA
	.4byte 0x2F1D268C
	.4byte 0x357F06C5
	.4byte 0x2E8F2618
	.4byte 0x366206B1
	.4byte 0x2E0225A4
	.4byte 0x3745069C
	.4byte 0x2D742530
	.4byte 0x38280687
	.4byte 0x2CE624BC
	.4byte 0x390B0673
	.4byte 0x2C582448
	.4byte 0x39EE065E
	.4byte 0x2BCA23D4
	.4byte 0x3AD10649
	.4byte 0x2B3C2360
	.4byte 0x3BB40635
	.4byte 0x2AAE22EB
	.4byte 0x3C970620
	.4byte 0x2A202277
	.4byte 0x3D7A060C
	.4byte 0x29922203
	.4byte 0x3E5D05F7
	.4byte 0x2904218F
	.4byte 0x3F4005E2
	.4byte 0x2877211B
	.4byte 0x402305CE
	.4byte 0x27E920A7
	.4byte 0x410605B9
	.4byte 0x275B2033
	.4byte 0x41E905A4
	.4byte 0x26CD1FBF
	.4byte 0x42CC0590
	.4byte 0x263F1F4B
	.4byte 0x43AF057B
	.4byte 0x25B11ED7
	.4byte 0x44920566
	.4byte 0x25231E62
	.4byte 0x45750552
	.4byte 0x24951DEE
	.4byte 0x4658053D
	.4byte 0x24071D7A
	.4byte 0x473B0529
	.4byte 0x23791D06
	.4byte 0x481F0514
	.4byte 0x22EB1C92
	.4byte 0x490204FF
	.4byte 0x225E1C1E
	.4byte 0x49E504EB
	.4byte 0x21D01BAA
	.4byte 0x4AC804D6
	.4byte 0x21421B36
	.4byte 0x4BAB04C1
	.4byte 0x20B41AC2
	.4byte 0x4C8E04AD
	.4byte 0x20261A4E
	.4byte 0x4D710498
	.4byte 0x1F9819D9
	.4byte 0x4E540483
	.4byte 0x1F0A1965
	.4byte 0x4F37046F
	.4byte 0x1E7C18F1
	.4byte 0x501A045A
	.4byte 0x1DEE187D
	.4byte 0x50FD0445
	.4byte 0x1D601809
	.4byte 0x51E00431
	.4byte 0x1CD31795
	.4byte 0x52C3041C
	.4byte 0x1C451721
	.4byte 0x53A60408
	.4byte 0x1BB716AD
	.4byte 0x548903F3
	.4byte 0x1B291639
	.4byte 0x556C03DE
	.4byte 0x1A9B15C4
	.4byte 0x564F03CA
	.4byte 0x1A0D1550
	.4byte 0x573203B5
	.4byte 0x197F14DC
	.4byte 0x581503A0
	.4byte 0x18F11468
	.4byte 0x58F8038C
	.4byte 0x186313F4
	.4byte 0x59DB0377
	.4byte 0x17D51380
	.4byte 0x5ABF0362
	.4byte 0x1747130C
	.4byte 0x5BA2034E
	.4byte 0x16BA1298
	.4byte 0x5C850339
	.4byte 0x162C1224
	.4byte 0x5D680324
	.4byte 0x159E11B0
	.4byte 0x5E4B0310
	.4byte 0x1510113B
	.4byte 0x5F2E02FB
	.4byte 0x148210C7
	.4byte 0x601102E7
	.4byte 0x13F41053
	.4byte 0x60F402D2
	.4byte 0x13660FDF
	.4byte 0x61D702BD
	.4byte 0x12D80F6B
	.4byte 0x62BA02A9
	.4byte 0x124A0EF7
	.4byte 0x639D0294
	.4byte 0x11BC0E83
	.4byte 0x6480027F
	.4byte 0x112F0E0F
	.4byte 0x6563026B
	.4byte 0x10A10D9B
	.4byte 0x66460256
	.4byte 0x10130D27
	.4byte 0x67290241
	.4byte 0x0F850CB2
	.4byte 0x680C022D
	.4byte 0x0EF70C3E
	.4byte 0x68EF0218
	.4byte 0x0E690BCA
	.4byte 0x69D20204
	.4byte 0x0DDB0B56
	.4byte 0x6AB501EF
	.4byte 0x0D4D0AE2
	.4byte 0x6B9801DA
	.4byte 0x0CBF0A6E
	.4byte 0x6C7B01C6
	.4byte 0x0C3109FA
	.4byte 0x6D5F01B1
	.4byte 0x0BA30986
	.4byte 0x6E42019C
	.4byte 0x0B160912
	.4byte 0x6F250188
	.4byte 0x0A88089D
	.4byte 0x70080173
	.4byte 0x09FA0829
	.4byte 0x70EB015E
	.4byte 0x096C07B5
	.4byte 0x71CE014A
	.4byte 0x08DE0741
	.4byte 0x72B10135
	.4byte 0x085006CD
	.4byte 0x73940120
	.4byte 0x07C20659
	.4byte 0x7477010C
	.4byte 0x073405E5
	.4byte 0x755A00F7
	.4byte 0x06A60571
	.4byte 0x763D00E3
	.4byte 0x061804FD
	.4byte 0x772000CE
	.4byte 0x058B0489
	.4byte 0x780300B9
	.4byte 0x04FD0414
	.4byte 0x78E600A5
	.4byte 0x046F03A0
	.4byte 0x79C90090
	.4byte 0x03E1032C
	.4byte 0x7AAC007B
	.4byte 0x035302B8
	.4byte 0x7B8F0067
	.4byte 0x02C50244
	.4byte 0x7C720052
	.4byte 0x023701D0
	.4byte 0x7D55003D
	.4byte 0x01A9015C
	.4byte 0x7E380029
	.4byte 0x011B00E8
	.4byte 0x7F1B0014
	.4byte 0x008D0074
	.4byte 0x7FFF0000
	.4byte 0
s_key_table__Q28JASystem6Player:
	.4byte 0x3F800000
	.4byte 0x3F801DD2
	.4byte 0x3F803BAC
	.4byte 0x3F80598E
	.4byte 0x3F807770
	.4byte 0x3F809564
	.4byte 0x3F80B357
	.4byte 0x3F80D152
	.4byte 0x3F80EF56
	.4byte 0x3F810D5A
	.4byte 0x3F812B6F
	.4byte 0x3F814984
	.4byte 0x3F8167A1
	.4byte 0x3F8185CF
	.4byte 0x3F81A3F4
	.4byte 0x3F81C22B
	.4byte 0x3F81E069
	.4byte 0x3F81FEA8
	.4byte 0x3F821CEF
	.4byte 0x3F823B3F
	.4byte 0x3F825996
	.4byte 0x3F8277F7
	.4byte 0x3F82965F
	.4byte 0x3F82B4C8
	.4byte 0x3F82D339
	.4byte 0x3F82F1BB
	.4byte 0x3F83103D
	.4byte 0x3F832EBE
	.4byte 0x3F834D51
	.4byte 0x3F836BEC
	.4byte 0x3F838A87
	.4byte 0x3F83A92A
	.4byte 0x3F83C7D6
	.4byte 0x3F83E68A
	.4byte 0x3F840547
	.4byte 0x3F84240B
	.4byte 0x3F8442D0
	.4byte 0x3F8461A6
	.4byte 0x3F84807C
	.4byte 0x3F849F5A
	.4byte 0x3F84BE40
	.4byte 0x3F84DD2F
	.4byte 0x3F84FC1E
	.4byte 0x3F851B1E
	.4byte 0x3F853A1D
	.4byte 0x3F855925
	.4byte 0x3F857836
	.4byte 0x3F85974E
	.4byte 0x3F85B670
	.4byte 0x3F85D599
	.4byte 0x3F85F4C3
	.4byte 0x3F8613FD
	.4byte 0x3F863337
	.4byte 0x3F86527A
	.4byte 0x3F8671C5
	.4byte 0x3F869119
	.4byte 0x3F86B075
	.4byte 0x3F86CFD1
	.4byte 0x3F86EF3D
	.4byte 0x3F870EAA
	.4byte 0x3F872E1F
	.4byte 0x3F874D9C
	.4byte 0x3F876D22
	.4byte 0x3F878CB0
.global sAdsrDef__Q28JASystem6Player
sAdsrDef__Q28JASystem6Player:
	.4byte 0
	.4byte 0x3F800000
	.4byte 0
	.4byte 0
	.4byte 0x3F800000
	.4byte 0
.global sEnvelopeDef__Q28JASystem6Player
sEnvelopeDef__Q28JASystem6Player:
    .4byte 0
    .float 1.0
    .4byte 0
    .4byte sRelTable__Q28JASystem6Player
    .float 1.0
    .4byte 0
.global sVibratoDef__Q28JASystem6Player
sVibratoDef__Q28JASystem6Player:
    .4byte 0x01000000
    .float 0.5
    .4byte sVibTable__Q28JASystem6Player
    .4byte sVibTable__Q28JASystem6Player
    .4byte 0
    .float 1.0
.global sTremoroDef__Q28JASystem6Player
sTremoroDef__Q28JASystem6Player:
    .4byte 0
    .float 0.5
    .4byte sTreTable__Q28JASystem6Player
    .4byte sTreTable__Q28JASystem6Player
    .4byte 0
    .float 1.0
.global sAdsTable__Q28JASystem6Player
sAdsTable__Q28JASystem6Player:
	.4byte 0
	.4byte 0x7FFF0000
	.4byte 0x00007FFF
	.4byte 0
	.4byte 0x0000000E
	.4byte 0
.global sRelTable__Q28JASystem6Player
sRelTable__Q28JASystem6Player:
	.4byte 0x0000000A
	.4byte 0x0000000F
	.4byte 0x00010000
sVibTable__Q28JASystem6Player:
	.4byte 0
	.4byte 0
	.4byte 0x000C7FFF
	.4byte 0x0000000C
	.4byte 0
	.4byte 0x000CC000
	.4byte 0x0000000C
	.4byte 0x0000000D
	.4byte 0x00000001
sTreTable__Q28JASystem6Player:
	.4byte 0
	.4byte 0x7FFF0000
	.4byte 0x00140000
	.4byte 0x00000014
	.4byte 0x80010000  ;# ptr
	.4byte 0x00140000
	.4byte 0x00000014
	.4byte 0x7FFF000D
	.4byte 0x00000001
	.4byte 0

.section .sdata, "wa"  # 0x80408AC0 - 0x804097C0
sV0__Q28JASystem6Player:
	.4byte 0x00001000
sV1__Q28JASystem6Player:
	.4byte 0x00005555

.section .sdata2, "a"  # 0x8040B460 - 0x80414020
.balign 8
"@127":
	.4byte 0x40800000
"@128":
  .4byte 0
"@129":
	.4byte 0x3F800000
"@130":
	.4byte 0x42800000
"@134":
	.4byte 0x43300000
	.4byte 0x80000000

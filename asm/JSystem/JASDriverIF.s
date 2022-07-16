.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global init__Q28JASystem6DriverFv
init__Q28JASystem6DriverFv:
/* 8005E9A4 0005B8E4  7C 08 02 A6 */	mflr r0
/* 8005E9A8 0005B8E8  38 60 00 00 */	li r3, 0
/* 8005E9AC 0005B8EC  90 01 00 04 */	stw r0, 4(r1)
/* 8005E9B0 0005B8F0  94 21 FF F8 */	stwu r1, -8(r1)
/* 8005E9B4 0005B8F4  48 00 01 21 */	bl process__Q28JASystem6DSPBufFQ38JASystem6DSPBuf13DSPBUF_EVENTS
/* 8005E9B8 0005B8F8  48 00 0E 71 */	bl initBuffer__Q28JASystem12DSPInterfaceFv
/* 8005E9BC 0005B8FC  48 00 06 89 */	bl initAll__Q28JASystem11TDSPChannelFv
/* 8005E9C0 0005B900  4B FF FB 8D */	bl init__Q28JASystem8ChGlobalFv
/* 8005E9C4 0005B904  80 6D 81 AC */	lwz r3, DSP_VOICE_WAITS_MAX__Q28JASystem6Driver@sda21(r13)
/* 8005E9C8 0005B908  4B FF D3 F1 */	bl init__Q38JASystem6Driver8DSPQueueFUl
/* 8005E9CC 0005B90C  80 01 00 0C */	lwz r0, 0xc(r1)
/* 8005E9D0 0005B910  38 21 00 08 */	addi r1, r1, 8
/* 8005E9D4 0005B914  7C 08 03 A6 */	mtlr r0
/* 8005E9D8 0005B918  4E 80 00 20 */	blr 

.global setMixerLevel__Q28JASystem6DriverFff
setMixerLevel__Q28JASystem6DriverFff:
/* 8005E9DC 0005B91C  7C 08 02 A6 */	mflr r0
/* 8005E9E0 0005B920  90 01 00 04 */	stw r0, 4(r1)
/* 8005E9E4 0005B924  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8005E9E8 0005B928  C0 02 86 68 */	lfs f0, "@120"@sda21(r2)
/* 8005E9EC 0005B92C  EC 00 00 72 */	fmuls f0, f0, f1
/* 8005E9F0 0005B930  FC 20 10 90 */	fmr f1, f2
/* 8005E9F4 0005B934  FC 00 00 1E */	fctiwz f0, f0
/* 8005E9F8 0005B938  D8 01 00 10 */	stfd f0, 0x10(r1)
/* 8005E9FC 0005B93C  80 01 00 14 */	lwz r0, 0x14(r1)
/* 8005EA00 0005B940  B0 0D 81 A0 */	sth r0, MAX_MIXERLEVEL__Q28JASystem6Driver@sda21(r13)
/* 8005EA04 0005B944  48 02 32 FD */	bl DsetMixerLevel__Ff
/* 8005EA08 0005B948  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8005EA0C 0005B94C  38 21 00 18 */	addi r1, r1, 0x18
/* 8005EA10 0005B950  7C 08 03 A6 */	mtlr r0
/* 8005EA14 0005B954  4E 80 00 20 */	blr 

.global getMixerLevel__Q28JASystem6DriverFv
getMixerLevel__Q28JASystem6DriverFv:
/* 8005EA18 0005B958  A0 6D 81 A0 */	lhz r3, MAX_MIXERLEVEL__Q28JASystem6Driver@sda21(r13)
/* 8005EA1C 0005B95C  4E 80 00 20 */	blr 

.global setOutputMode__Q28JASystem6DriverFUl
setOutputMode__Q28JASystem6DriverFUl:
/* 8005EA20 0005B960  90 6D 81 A4 */	stw r3, JAS_SYSTEM_OUTPUT_MODE__Q28JASystem6Driver@sda21(r13)
/* 8005EA24 0005B964  4E 80 00 20 */	blr 

.global getOutputMode__Q28JASystem6DriverFv
getOutputMode__Q28JASystem6DriverFv:
/* 8005EA28 0005B968  80 6D 81 A4 */	lwz r3, JAS_SYSTEM_OUTPUT_MODE__Q28JASystem6Driver@sda21(r13)
/* 8005EA2C 0005B96C  4E 80 00 20 */	blr 

.global getUpdateInterval__Q28JASystem6DriverFv
getUpdateInterval__Q28JASystem6DriverFv:
/* 8005EA30 0005B970  88 6D 81 A8 */	lbz r3, JAS_UPDATE_INTERVAL__Q28JASystem6Driver@sda21(r13)
/* 8005EA34 0005B974  4E 80 00 20 */	blr 

.section .sdata, "wa"  # 0x80408AC0 - 0x804097C0
MAX_MIXERLEVEL__Q28JASystem6Driver:
	.4byte 0x2EE00000
JAS_SYSTEM_OUTPUT_MODE__Q28JASystem6Driver:
	.4byte 0x00000001
JAS_UPDATE_INTERVAL__Q28JASystem6Driver:
	.4byte 0x01000000
DSP_VOICE_WAITS_MAX__Q28JASystem6Driver:
	.4byte 0x00000020

.section .sdata2, "a"  # 0x8040B460 - 0x80414020
.balign 8
"@120":
	.4byte 0x46800000
	.4byte 0

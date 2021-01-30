.include "macros.inc"

.section .rodata, "wa"  # 0x8036FFA0 - 0x803A8380
.global $$2395
$$2395:
	.incbin "baserom.dol", 0x36E130, 0xC
.global $$2415
$$2415:
	.incbin "baserom.dol", 0x36E13C, 0x3C
.global $$2416
$$2416:
	.incbin "baserom.dol", 0x36E178, 0x10
.global $$2417
$$2417:
	.incbin "baserom.dol", 0x36E188, 0x60

.section .sdata, "wa"  # 0x80408AC0 - 0x804097C0
.global asnFileName__Q27JAInter8TAsnData
asnFileName__Q27JAInter8TAsnData:
	.incbin "baserom.dol", 0x3E30D8, 0x8

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
.global asnData__Q27JAInter8TAsnData
asnData__Q27JAInter8TAsnData:
	.skip 0x8

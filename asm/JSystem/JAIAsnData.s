.include "macros.inc"

.section .rodata, "a"  # 0x8036FFA0 - 0x803A8380
.balign 8
"@395":
	.incbin "baserom.dol", 0x36E130, 0xC
"@415":
	.incbin "baserom.dol", 0x36E13C, 0x3C
"@416":
	.incbin "baserom.dol", 0x36E178, 0x10
"@417":
	.incbin "baserom.dol", 0x36E188, 0x60

.section .sdata, "wa"  # 0x80408AC0 - 0x804097C0
.global asnFileName__Q27JAInter8TAsnData
asnFileName__Q27JAInter8TAsnData:
  .4byte "@395"
  .4byte 0

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
.global asnData__Q27JAInter8TAsnData
asnData__Q27JAInter8TAsnData:
	.skip 0x8

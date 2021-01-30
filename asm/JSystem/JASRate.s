.include "macros.inc"

.section .sdata, "wa"  # 0x80408AC0 - 0x804097C0
.global gDacRate__Q28JASystem6Kernel
gDacRate__Q28JASystem6Kernel:
	.incbin "baserom.dol", 0x3E31F0, 0x4
.global gSubFrames__Q28JASystem6Kernel
gSubFrames__Q28JASystem6Kernel:
	.incbin "baserom.dol", 0x3E31F4, 0x4
.global gFrameSamples__Q28JASystem6Kernel
gFrameSamples__Q28JASystem6Kernel:
	.incbin "baserom.dol", 0x3E31F8, 0x4
.global gDacSize__Q28JASystem6Kernel
gDacSize__Q28JASystem6Kernel:
	.incbin "baserom.dol", 0x3E31FC, 0x4

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
.global gOutputRate__Q28JASystem6Kernel
gOutputRate__Q28JASystem6Kernel:
	.skip 0x4
.global gAiSetting__Q28JASystem6Kernel
gAiSetting__Q28JASystem6Kernel:
	.skip 0x4

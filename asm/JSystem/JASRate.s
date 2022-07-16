.include "macros.inc"

.section .sdata, "wa"  # 0x80408AC0 - 0x804097C0
.global gDacRate__Q28JASystem6Kernel
gDacRate__Q28JASystem6Kernel:
	.4byte 0x46FA3900
.global gSubFrames__Q28JASystem6Kernel
gSubFrames__Q28JASystem6Kernel:
	.4byte 0x00000007
.global gFrameSamples__Q28JASystem6Kernel
gFrameSamples__Q28JASystem6Kernel:
	.4byte 0x00000230
.global gDacSize__Q28JASystem6Kernel
gDacSize__Q28JASystem6Kernel:
	.4byte 0x00000460

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
.global gOutputRate__Q28JASystem6Kernel
gOutputRate__Q28JASystem6Kernel:
	.skip 0x4
.global gAiSetting__Q28JASystem6Kernel
gAiSetting__Q28JASystem6Kernel:
	.skip 0x4

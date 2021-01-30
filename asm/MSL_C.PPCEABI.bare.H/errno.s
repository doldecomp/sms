.include "macros.inc"

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
.global errno
errno:
	.skip 0x8

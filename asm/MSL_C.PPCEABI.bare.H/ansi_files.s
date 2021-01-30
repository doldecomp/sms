.include "macros.inc"

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __files
__files:
	.incbin "baserom.dol", 0x3ABC00, 0xD8

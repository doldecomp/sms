.include "macros.inc"

.section .data, "wa"  # 0x803A8380 - 0x803E6000
    .balign 8
.global __float_nan
__float_nan:
	.incbin "baserom.dol", 0x3ABF40, 0x4
.global __float_huge
__float_huge:
	.incbin "baserom.dol", 0x3ABF44, 0x4

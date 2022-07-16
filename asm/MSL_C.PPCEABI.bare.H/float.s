.include "macros.inc"

.section .data, "wa"  # 0x803A8380 - 0x803E6000
    .balign 8
.global __float_nan
__float_nan:
	.4byte 0x7FFFFFFF
.global __float_huge
__float_huge:
	.4byte 0x7F800000

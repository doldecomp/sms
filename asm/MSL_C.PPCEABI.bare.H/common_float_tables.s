.include "macros.inc"

.section .rodata, "wa"  # 0x8036FFA0 - 0x803A8380
.global __two_to_log2e_m1_tI
__two_to_log2e_m1_tI:
	.incbin "baserom.dol", 0x36EA90, 0x2C8
.global __one_over_F
__one_over_F:
	.incbin "baserom.dol", 0x36ED58, 0x204
.global __sincos_on_quadrant
__sincos_on_quadrant:
	.incbin "baserom.dol", 0x36EF5C, 0x20
.global __sincos_poly
__sincos_poly:
	.incbin "baserom.dol", 0x36EF7C, 0x2C

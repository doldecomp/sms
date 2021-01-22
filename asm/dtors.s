.include "macros.inc"

.section .dtors, "wa"  # 0x8036FF80 - 0x8036FFA0
.global lbl_8036FF80
lbl_8036FF80:
	.incbin "baserom.dol", 0x36CF80, 0x20

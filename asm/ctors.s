.include "macros.inc"

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
.global lbl_8036FBA0
lbl_8036FBA0:
	.incbin "baserom.dol", 0x36CBA0, 0x3E0

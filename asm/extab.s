.include "macros.inc"

.section extab_, "wa"  # 0x80005540 - 0x800055A0
	.incbin "baserom.dol", 0x36CAE0, 0x60

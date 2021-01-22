.include "macros.inc"

.section extabindex_, "wa"  # 0x800055A0 - 0x80005600
	.incbin "baserom.dol", 0x36CB40, 0x3C
.global lbl_800055DC
lbl_800055DC:
	.incbin "baserom.dol", 0x36CB7C, 0x24

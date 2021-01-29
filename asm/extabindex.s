.include "macros.inc"

.section extabindex_, "wa"  # 0x800055A0 - 0x80005600
.global $$2153
$$2153:
	.incbin "baserom.dol", 0x36CB40, 0xC
.global $$2158
$$2158:
	.incbin "baserom.dol", 0x36CB4C, 0xC
.global $$2175
$$2175:
	.incbin "baserom.dol", 0x36CB58, 0xC
.global $$2183
$$2183:
	.incbin "baserom.dol", 0x36CB64, 0xC
.global $$2202
$$2202:
	.incbin "baserom.dol", 0x36CB70, 0xC
.global lbl_800055DC
lbl_800055DC:
	.incbin "baserom.dol", 0x36CB7C, 0x24

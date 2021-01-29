.include "macros.inc"

.section extab_, "wa"  # 0x80005540 - 0x800055A0
.global $$2152
$$2152:
	.incbin "baserom.dol", 0x36CAE0, 0x8
.global $$2157
$$2157:
	.incbin "baserom.dol", 0x36CAE8, 0x8
.global $$2174
$$2174:
	.incbin "baserom.dol", 0x36CAF0, 0x18
.global $$2182
$$2182:
	.incbin "baserom.dol", 0x36CB08, 0x8
.global $$2201
$$2201:
	.incbin "baserom.dol", 0x36CB10, 0x30

.include "macros.inc"

.section .sdata, "wa"  # 0x80408AC0 - 0x804097C0
.global __CARDVendorID
__CARDVendorID:
	.incbin "baserom.dol", 0x3E32C0, 0x8

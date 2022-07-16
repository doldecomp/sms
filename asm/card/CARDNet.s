.include "macros.inc"

.section .sdata, "wa"  # 0x80408AC0 - 0x804097C0
.global __CARDVendorID
__CARDVendorID:
	.4byte 0xFFFF0000
	.4byte 0

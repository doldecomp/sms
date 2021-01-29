.include "macros.inc"

.section .dtors, "wa"  # 0x8036FF80 - 0x8036FFA0
.global __destroy_global_chain_reference
__destroy_global_chain_reference:
	.incbin "baserom.dol", 0x36CF80, 0x4
.global __fini_cpp_exceptions_reference
__fini_cpp_exceptions_reference:
	.incbin "baserom.dol", 0x36CF84, 0x4
.global __destroy_global_chain_reference
__destroy_global_chain_reference:
	.incbin "baserom.dol", 0x36CF88, 0x18

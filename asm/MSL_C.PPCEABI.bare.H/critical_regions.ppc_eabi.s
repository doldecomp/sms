.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __kill_critical_regions
__kill_critical_regions:
/* 80083858 00080798  4E 80 00 20 */	blr 

.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global probeStart__Q28JASystem6KernelFlPc
probeStart__Q28JASystem6KernelFlPc:
/* 80063DCC 00060D0C  4E 80 00 20 */	blr 

.global probeFinish__Q28JASystem6KernelFl
probeFinish__Q28JASystem6KernelFl:
/* 80063DD0 00060D10  4E 80 00 20 */	blr 

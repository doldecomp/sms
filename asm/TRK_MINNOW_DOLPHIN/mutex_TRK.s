.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0

.global TRKInitializeMutex
TRKInitializeMutex:
/* 8008A24C 0008718C  38 60 00 00 */	li r3, 0
/* 8008A250 00087190  4E 80 00 20 */	blr 

.global TRKAcquireMutex
TRKAcquireMutex:
/* 8008A254 00087194  38 60 00 00 */	li r3, 0
/* 8008A258 00087198  4E 80 00 20 */	blr 

.global TRKReleaseMutex
TRKReleaseMutex:
/* 8008A25C 0008719C  38 60 00 00 */	li r3, 0
/* 8008A260 000871A0  4E 80 00 20 */	blr 

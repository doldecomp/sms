.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global EXI2_Init
EXI2_Init:
/* 800AEFE0 000ABF20  4E 80 00 20 */	blr 

.global EXI2_EnableInterrupts
EXI2_EnableInterrupts:
/* 800AEFE4 000ABF24  4E 80 00 20 */	blr 

.global EXI2_Poll
EXI2_Poll:
/* 800AEFE8 000ABF28  38 60 00 00 */	li r3, 0
/* 800AEFEC 000ABF2C  4E 80 00 20 */	blr 

.global EXI2_ReadN
EXI2_ReadN:
/* 800AEFF0 000ABF30  38 60 00 00 */	li r3, 0
/* 800AEFF4 000ABF34  4E 80 00 20 */	blr 

.global EXI2_WriteN
EXI2_WriteN:
/* 800AEFF8 000ABF38  38 60 00 00 */	li r3, 0
/* 800AEFFC 000ABF3C  4E 80 00 20 */	blr 

.global EXI2_Reserve
EXI2_Reserve:
/* 800AF000 000ABF40  4E 80 00 20 */	blr 

.global EXI2_Unreserve
EXI2_Unreserve:
/* 800AF004 000ABF44  4E 80 00 20 */	blr 

.global AMC_IsStub
AMC_IsStub:
/* 800AF008 000ABF48  38 60 00 01 */	li r3, 1
/* 800AF00C 000ABF4C  4E 80 00 20 */	blr 

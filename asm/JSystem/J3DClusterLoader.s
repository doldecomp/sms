.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0

.global JSUConvertOffsetToPtr$$0Us$$1__FPCvPCv
JSUConvertOffsetToPtr$$0Us$$1__FPCvPCv:
/* 8002EB3C 0002BA7C  28 04 00 00 */	cmplwi r4, 0
/* 8002EB40 0002BA80  40 82 00 0C */	bne lbl_8002EB4C
/* 8002EB44 0002BA84  38 60 00 00 */	li r3, 0
/* 8002EB48 0002BA88  4E 80 00 20 */	blr 
lbl_8002EB4C:
/* 8002EB4C 0002BA8C  7C 63 22 14 */	add r3, r3, r4
/* 8002EB50 0002BA90  4E 80 00 20 */	blr 

.global JSUConvertOffsetToPtr$$0f$$1__FPCvPCv
JSUConvertOffsetToPtr$$0f$$1__FPCvPCv:
/* 8002EB54 0002BA94  28 04 00 00 */	cmplwi r4, 0
/* 8002EB58 0002BA98  40 82 00 0C */	bne lbl_8002EB64
/* 8002EB5C 0002BA9C  38 60 00 00 */	li r3, 0
/* 8002EB60 0002BAA0  4E 80 00 20 */	blr 
lbl_8002EB64:
/* 8002EB64 0002BAA4  7C 63 22 14 */	add r3, r3, r4
/* 8002EB68 0002BAA8  4E 80 00 20 */	blr 

.global JSUConvertOffsetToPtr$$07ResNTAB$$1__FPCvPCv
JSUConvertOffsetToPtr$$07ResNTAB$$1__FPCvPCv:
/* 8002EB6C 0002BAAC  28 04 00 00 */	cmplwi r4, 0
/* 8002EB70 0002BAB0  40 82 00 0C */	bne lbl_8002EB7C
/* 8002EB74 0002BAB4  38 60 00 00 */	li r3, 0
/* 8002EB78 0002BAB8  4E 80 00 20 */	blr 
lbl_8002EB7C:
/* 8002EB7C 0002BABC  7C 63 22 14 */	add r3, r3, r4
/* 8002EB80 0002BAC0  4E 80 00 20 */	blr 

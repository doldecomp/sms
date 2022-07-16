.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global PSVECAdd
PSVECAdd:
/* 80094E8C 00091DCC  E0 43 00 00 */	psq_l f2, 0(r3), 0, qr0
/* 80094E90 00091DD0  E0 84 00 00 */	psq_l f4, 0(r4), 0, qr0
/* 80094E94 00091DD4  10 C2 20 2A */	ps_add f6, f2, f4
/* 80094E98 00091DD8  F0 C5 00 00 */	psq_st f6, 0(r5), 0, qr0
/* 80094E9C 00091DDC  E0 63 80 08 */	psq_l f3, 8(r3), 1, qr0
/* 80094EA0 00091DE0  E0 A4 80 08 */	psq_l f5, 8(r4), 1, qr0
/* 80094EA4 00091DE4  10 E3 28 2A */	ps_add f7, f3, f5
/* 80094EA8 00091DE8  F0 E5 80 08 */	psq_st f7, 8(r5), 1, qr0
/* 80094EAC 00091DEC  4E 80 00 20 */	blr 

.global PSVECSubtract
PSVECSubtract:
/* 80094EB0 00091DF0  E0 43 00 00 */	psq_l f2, 0(r3), 0, qr0
/* 80094EB4 00091DF4  E0 84 00 00 */	psq_l f4, 0(r4), 0, qr0
/* 80094EB8 00091DF8  10 C2 20 28 */	ps_sub f6, f2, f4
/* 80094EBC 00091DFC  F0 C5 00 00 */	psq_st f6, 0(r5), 0, qr0
/* 80094EC0 00091E00  E0 63 80 08 */	psq_l f3, 8(r3), 1, qr0
/* 80094EC4 00091E04  E0 A4 80 08 */	psq_l f5, 8(r4), 1, qr0
/* 80094EC8 00091E08  10 E3 28 28 */	ps_sub f7, f3, f5
/* 80094ECC 00091E0C  F0 E5 80 08 */	psq_st f7, 8(r5), 1, qr0
/* 80094ED0 00091E10  4E 80 00 20 */	blr 

.global PSVECScale
PSVECScale:
/* 80094ED4 00091E14  E0 03 00 00 */	psq_l f0, 0(r3), 0, qr0
/* 80094ED8 00091E18  E0 43 80 08 */	psq_l f2, 8(r3), 1, qr0
/* 80094EDC 00091E1C  10 00 00 58 */	ps_muls0 f0, f0, f1
/* 80094EE0 00091E20  F0 04 00 00 */	psq_st f0, 0(r4), 0, qr0
/* 80094EE4 00091E24  10 02 00 58 */	ps_muls0 f0, f2, f1
/* 80094EE8 00091E28  F0 04 80 08 */	psq_st f0, 8(r4), 1, qr0
/* 80094EEC 00091E2C  4E 80 00 20 */	blr 

.global PSVECNormalize
PSVECNormalize:
/* 80094EF0 00091E30  C0 02 8A C8 */	lfs f0, "@116"@sda21(r2)
/* 80094EF4 00091E34  C0 22 8A CC */	lfs f1, "@117"@sda21(r2)
/* 80094EF8 00091E38  E0 43 00 00 */	psq_l f2, 0(r3), 0, qr0
/* 80094EFC 00091E3C  10 A2 00 B2 */	ps_mul f5, f2, f2
/* 80094F00 00091E40  E0 63 80 08 */	psq_l f3, 8(r3), 1, qr0
/* 80094F04 00091E44  10 83 28 FA */	ps_madd f4, f3, f3, f5
/* 80094F08 00091E48  10 84 28 D4 */	ps_sum0 f4, f4, f3, f5
/* 80094F0C 00091E4C  FC A0 20 34 */	frsqrte f5, f4
/* 80094F10 00091E50  EC C5 01 72 */	fmuls f6, f5, f5
/* 80094F14 00091E54  EC 05 00 32 */	fmuls f0, f5, f0
/* 80094F18 00091E58  EC C6 09 3C */	fnmsubs f6, f6, f4, f1
/* 80094F1C 00091E5C  EC A6 00 32 */	fmuls f5, f6, f0
/* 80094F20 00091E60  10 42 01 58 */	ps_muls0 f2, f2, f5
/* 80094F24 00091E64  F0 44 00 00 */	psq_st f2, 0(r4), 0, qr0
/* 80094F28 00091E68  10 63 01 58 */	ps_muls0 f3, f3, f5
/* 80094F2C 00091E6C  F0 64 80 08 */	psq_st f3, 8(r4), 1, qr0
/* 80094F30 00091E70  4E 80 00 20 */	blr 

.global PSVECMag
PSVECMag:
/* 80094F34 00091E74  E0 03 00 00 */	psq_l f0, 0(r3), 0, qr0
/* 80094F38 00091E78  10 00 00 32 */	ps_mul f0, f0, f0
/* 80094F3C 00091E7C  C0 23 00 08 */	lfs f1, 8(r3)
/* 80094F40 00091E80  10 21 00 7A */	ps_madd f1, f1, f1, f0
/* 80094F44 00091E84  C0 82 8A C8 */	lfs f4, "@116"@sda21(r2)
/* 80094F48 00091E88  10 21 00 14 */	ps_sum0 f1, f1, f0, f0
/* 80094F4C 00091E8C  FC 00 08 34 */	frsqrte f0, f1
/* 80094F50 00091E90  C0 62 8A CC */	lfs f3, "@117"@sda21(r2)
/* 80094F54 00091E94  EC 40 00 32 */	fmuls f2, f0, f0
/* 80094F58 00091E98  EC 00 01 32 */	fmuls f0, f0, f4
/* 80094F5C 00091E9C  EC 42 18 7C */	fnmsubs f2, f2, f1, f3
/* 80094F60 00091EA0  EC 02 00 32 */	fmuls f0, f2, f0
/* 80094F64 00091EA4  FC 00 08 2E */	fsel f0, f0, f0, f1
/* 80094F68 00091EA8  EC 21 00 32 */	fmuls f1, f1, f0
/* 80094F6C 00091EAC  4E 80 00 20 */	blr 

.global PSVECDotProduct
PSVECDotProduct:
/* 80094F70 00091EB0  E0 43 00 04 */	psq_l f2, 4(r3), 0, qr0
/* 80094F74 00091EB4  E0 64 00 04 */	psq_l f3, 4(r4), 0, qr0
/* 80094F78 00091EB8  10 42 00 F2 */	ps_mul f2, f2, f3
/* 80094F7C 00091EBC  E0 A3 00 00 */	psq_l f5, 0(r3), 0, qr0
/* 80094F80 00091EC0  E0 84 00 00 */	psq_l f4, 0(r4), 0, qr0
/* 80094F84 00091EC4  10 65 11 3A */	ps_madd f3, f5, f4, f2
/* 80094F88 00091EC8  10 23 10 94 */	ps_sum0 f1, f3, f2, f2
/* 80094F8C 00091ECC  4E 80 00 20 */	blr 

.global PSVECCrossProduct
PSVECCrossProduct:
/* 80094F90 00091ED0  E0 24 00 00 */	psq_l f1, 0(r4), 0, qr0
/* 80094F94 00091ED4  C0 43 00 08 */	lfs f2, 8(r3)
/* 80094F98 00091ED8  E0 03 00 00 */	psq_l f0, 0(r3), 0, qr0
/* 80094F9C 00091EDC  10 C1 0C A0 */	ps_merge10 f6, f1, f1
/* 80094FA0 00091EE0  C0 64 00 08 */	lfs f3, 8(r4)
/* 80094FA4 00091EE4  10 81 00 B2 */	ps_mul f4, f1, f2
/* 80094FA8 00091EE8  10 E1 00 18 */	ps_muls0 f7, f1, f0
/* 80094FAC 00091EEC  10 A0 20 F8 */	ps_msub f5, f0, f3, f4
/* 80094FB0 00091EF0  11 00 39 B8 */	ps_msub f8, f0, f6, f7
/* 80094FB4 00091EF4  11 25 2C E0 */	ps_merge11 f9, f5, f5
/* 80094FB8 00091EF8  11 45 44 60 */	ps_merge01 f10, f5, f8
/* 80094FBC 00091EFC  F1 25 80 00 */	psq_st f9, 0(r5), 1, qr0
/* 80094FC0 00091F00  11 40 50 50 */	ps_neg f10, f10
/* 80094FC4 00091F04  F1 45 00 04 */	psq_st f10, 4(r5), 0, qr0
/* 80094FC8 00091F08  4E 80 00 20 */	blr 

.global PSVECSquareDistance
PSVECSquareDistance:
/* 80094FCC 00091F0C  E0 03 00 04 */	psq_l f0, 4(r3), 0, qr0
/* 80094FD0 00091F10  E0 24 00 04 */	psq_l f1, 4(r4), 0, qr0
/* 80094FD4 00091F14  10 40 08 28 */	ps_sub f2, f0, f1
/* 80094FD8 00091F18  E0 03 00 00 */	psq_l f0, 0(r3), 0, qr0
/* 80094FDC 00091F1C  E0 24 00 00 */	psq_l f1, 0(r4), 0, qr0
/* 80094FE0 00091F20  10 42 00 B2 */	ps_mul f2, f2, f2
/* 80094FE4 00091F24  10 00 08 28 */	ps_sub f0, f0, f1
/* 80094FE8 00091F28  10 20 10 3A */	ps_madd f1, f0, f0, f2
/* 80094FEC 00091F2C  10 21 10 94 */	ps_sum0 f1, f1, f2, f2
/* 80094FF0 00091F30  4E 80 00 20 */	blr 

.global PSVECDistance
PSVECDistance:
/* 80094FF4 00091F34  E0 03 00 04 */	psq_l f0, 4(r3), 0, qr0
/* 80094FF8 00091F38  E0 24 00 04 */	psq_l f1, 4(r4), 0, qr0
/* 80094FFC 00091F3C  10 40 08 28 */	ps_sub f2, f0, f1
/* 80095000 00091F40  E0 03 00 00 */	psq_l f0, 0(r3), 0, qr0
/* 80095004 00091F44  E0 24 00 00 */	psq_l f1, 0(r4), 0, qr0
/* 80095008 00091F48  10 42 00 B2 */	ps_mul f2, f2, f2
/* 8009500C 00091F4C  10 00 08 28 */	ps_sub f0, f0, f1
/* 80095010 00091F50  C0 62 8A C8 */	lfs f3, "@116"@sda21(r2)
/* 80095014 00091F54  10 00 10 3A */	ps_madd f0, f0, f0, f2
/* 80095018 00091F58  10 00 10 94 */	ps_sum0 f0, f0, f2, f2
/* 8009501C 00091F5C  C0 82 8A CC */	lfs f4, "@117"@sda21(r2)
/* 80095020 00091F60  FC 20 00 34 */	frsqrte f1, f0
/* 80095024 00091F64  EC 41 00 72 */	fmuls f2, f1, f1
/* 80095028 00091F68  EC 21 00 F2 */	fmuls f1, f1, f3
/* 8009502C 00091F6C  EC 42 20 3C */	fnmsubs f2, f2, f0, f4
/* 80095030 00091F70  EC 22 00 72 */	fmuls f1, f2, f1
/* 80095034 00091F74  FC 21 00 6E */	fsel f1, f1, f1, f0
/* 80095038 00091F78  EC 20 00 72 */	fmuls f1, f0, f1
/* 8009503C 00091F7C  4E 80 00 20 */	blr 

.section .sdata2, "a"  # 0x8040B460 - 0x80414020
.balign 8
"@116":
	.4byte 0x3F000000
"@117":
	.4byte 0x40400000

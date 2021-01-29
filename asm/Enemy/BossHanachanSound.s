.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0

.global getBasNameTable__22TBossHanachanPartsBaseCFv
getBasNameTable__22TBossHanachanPartsBaseCFv:
/* 8031C050 00318F90  3C 60 80 3E */	lis r3, bosshanachan_bastable@ha
/* 8031C054 00318F94  38 63 10 88 */	addi r3, r3, bosshanachan_bastable@l
/* 8031C058 00318F98  4E 80 00 20 */	blr 

.global $$232$$2__dt__22TBossHanachanPartsBaseFv
$$232$$2__dt__22TBossHanachanPartsBaseFv:
/* 8031C05C 00318F9C  38 63 FF E0 */	addi r3, r3, -32
/* 8031C060 00318FA0  4B FE A4 98 */	b __dt__22TBossHanachanPartsBaseFv

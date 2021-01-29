.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0

.global PSMTXMultVec
PSMTXMultVec:
/* 80094BF0 00091B30  E0 04 00 00 */	psq_l f0, 0(r4), 0, qr0
/* 80094BF4 00091B34  E0 43 00 00 */	psq_l f2, 0(r3), 0, qr0
/* 80094BF8 00091B38  E0 24 80 08 */	psq_l f1, 8(r4), 1, qr0
/* 80094BFC 00091B3C  10 82 00 32 */	ps_mul f4, f2, f0
/* 80094C00 00091B40  E0 63 00 08 */	psq_l f3, 8(r3), 0, qr0
/* 80094C04 00091B44  10 A3 20 7A */	ps_madd f5, f3, f1, f4
/* 80094C08 00091B48  E1 03 00 10 */	psq_l f8, 16(r3), 0, qr0
/* 80094C0C 00091B4C  10 C5 29 94 */	ps_sum0 f6, f5, f6, f5
/* 80094C10 00091B50  E1 23 00 18 */	psq_l f9, 24(r3), 0, qr0
/* 80094C14 00091B54  11 48 00 32 */	ps_mul f10, f8, f0
/* 80094C18 00091B58  F0 C5 80 00 */	psq_st f6, 0(r5), 1, qr0
/* 80094C1C 00091B5C  11 69 50 7A */	ps_madd f11, f9, f1, f10
/* 80094C20 00091B60  E0 43 00 20 */	psq_l f2, 32(r3), 0, qr0
/* 80094C24 00091B64  11 8B 5B 14 */	ps_sum0 f12, f11, f12, f11
/* 80094C28 00091B68  E0 63 00 28 */	psq_l f3, 40(r3), 0, qr0
/* 80094C2C 00091B6C  10 82 00 32 */	ps_mul f4, f2, f0
/* 80094C30 00091B70  F1 85 80 04 */	psq_st f12, 4(r5), 1, qr0
/* 80094C34 00091B74  10 A3 20 7A */	ps_madd f5, f3, f1, f4
/* 80094C38 00091B78  10 C5 29 94 */	ps_sum0 f6, f5, f6, f5
/* 80094C3C 00091B7C  F0 C5 80 08 */	psq_st f6, 8(r5), 1, qr0
/* 80094C40 00091B80  4E 80 00 20 */	blr 

.global PSMTXMultVecArray
PSMTXMultVecArray:
/* 80094C44 00091B84  E1 A3 00 00 */	psq_l f13, 0(r3), 0, qr0
/* 80094C48 00091B88  E1 83 00 10 */	psq_l f12, 16(r3), 0, qr0
/* 80094C4C 00091B8C  38 C6 FF FF */	addi r6, r6, -1
/* 80094C50 00091B90  E1 63 00 08 */	psq_l f11, 8(r3), 0, qr0
/* 80094C54 00091B94  10 0D 64 20 */	ps_merge00 f0, f13, f12
/* 80094C58 00091B98  38 A5 FF FC */	addi r5, r5, -4
/* 80094C5C 00091B9C  E1 43 00 18 */	psq_l f10, 24(r3), 0, qr0
/* 80094C60 00091BA0  10 2D 64 E0 */	ps_merge11 f1, f13, f12
/* 80094C64 00091BA4  7C C9 03 A6 */	mtctr r6
/* 80094C68 00091BA8  E0 83 00 20 */	psq_l f4, 32(r3), 0, qr0
/* 80094C6C 00091BAC  10 4B 54 20 */	ps_merge00 f2, f11, f10
/* 80094C70 00091BB0  E0 A3 00 28 */	psq_l f5, 40(r3), 0, qr0
/* 80094C74 00091BB4  10 6B 54 E0 */	ps_merge11 f3, f11, f10
/* 80094C78 00091BB8  E0 C4 00 00 */	psq_l f6, 0(r4), 0, qr0
/* 80094C7C 00091BBC  E4 E4 80 08 */	psq_lu f7, 8(r4), 1, qr0
/* 80094C80 00091BC0  11 00 19 9C */	ps_madds0 f8, f0, f6, f3
/* 80094C84 00091BC4  11 24 01 B2 */	ps_mul f9, f4, f6
/* 80094C88 00091BC8  11 01 41 9E */	ps_madds1 f8, f1, f6, f8
/* 80094C8C 00091BCC  11 45 49 FA */	ps_madd f10, f5, f7, f9
lbl_80094C90:
/* 80094C90 00091BD0  E4 C4 00 04 */	psq_lu f6, 4(r4), 0, qr0
/* 80094C94 00091BD4  11 82 41 DC */	ps_madds0 f12, f2, f7, f8
/* 80094C98 00091BD8  E4 E4 80 08 */	psq_lu f7, 8(r4), 1, qr0
/* 80094C9C 00091BDC  11 AA 52 54 */	ps_sum0 f13, f10, f9, f10
/* 80094CA0 00091BE0  11 00 19 9C */	ps_madds0 f8, f0, f6, f3
/* 80094CA4 00091BE4  11 24 01 B2 */	ps_mul f9, f4, f6
/* 80094CA8 00091BE8  F5 85 00 04 */	psq_stu f12, 4(r5), 0, qr0
/* 80094CAC 00091BEC  11 01 41 9E */	ps_madds1 f8, f1, f6, f8
/* 80094CB0 00091BF0  F5 A5 80 08 */	psq_stu f13, 8(r5), 1, qr0
/* 80094CB4 00091BF4  11 45 49 FA */	ps_madd f10, f5, f7, f9
/* 80094CB8 00091BF8  42 00 FF D8 */	bdnz lbl_80094C90
/* 80094CBC 00091BFC  11 82 41 DC */	ps_madds0 f12, f2, f7, f8
/* 80094CC0 00091C00  11 AA 52 54 */	ps_sum0 f13, f10, f9, f10
/* 80094CC4 00091C04  F5 85 00 04 */	psq_stu f12, 4(r5), 0, qr0
/* 80094CC8 00091C08  F5 A5 80 08 */	psq_stu f13, 8(r5), 1, qr0
/* 80094CCC 00091C0C  4E 80 00 20 */	blr 

.global PSMTXMultVecSR
PSMTXMultVecSR:
/* 80094CD0 00091C10  E0 03 00 00 */	psq_l f0, 0(r3), 0, qr0
/* 80094CD4 00091C14  E0 C4 00 00 */	psq_l f6, 0(r4), 0, qr0
/* 80094CD8 00091C18  E0 43 00 10 */	psq_l f2, 16(r3), 0, qr0
/* 80094CDC 00091C1C  11 00 01 B2 */	ps_mul f8, f0, f6
/* 80094CE0 00091C20  E0 83 00 20 */	psq_l f4, 32(r3), 0, qr0
/* 80094CE4 00091C24  11 42 01 B2 */	ps_mul f10, f2, f6
/* 80094CE8 00091C28  E0 E4 80 08 */	psq_l f7, 8(r4), 1, qr0
/* 80094CEC 00091C2C  11 84 01 B2 */	ps_mul f12, f4, f6
/* 80094CF0 00091C30  E0 63 00 18 */	psq_l f3, 24(r3), 0, qr0
/* 80094CF4 00091C34  11 08 42 14 */	ps_sum0 f8, f8, f8, f8
/* 80094CF8 00091C38  E0 A3 00 28 */	psq_l f5, 40(r3), 0, qr0
/* 80094CFC 00091C3C  11 4A 52 94 */	ps_sum0 f10, f10, f10, f10
/* 80094D00 00091C40  E0 23 00 08 */	psq_l f1, 8(r3), 0, qr0
/* 80094D04 00091C44  11 8C 63 14 */	ps_sum0 f12, f12, f12, f12
/* 80094D08 00091C48  11 21 41 FA */	ps_madd f9, f1, f7, f8
/* 80094D0C 00091C4C  F1 25 80 00 */	psq_st f9, 0(r5), 1, qr0
/* 80094D10 00091C50  11 63 51 FA */	ps_madd f11, f3, f7, f10
/* 80094D14 00091C54  F1 65 80 04 */	psq_st f11, 4(r5), 1, qr0
/* 80094D18 00091C58  11 A5 61 FA */	ps_madd f13, f5, f7, f12
/* 80094D1C 00091C5C  F1 A5 80 08 */	psq_st f13, 8(r5), 1, qr0
/* 80094D20 00091C60  4E 80 00 20 */	blr 

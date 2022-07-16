.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global DSPCheckMailToDSP
DSPCheckMailToDSP:
/* 8009E658 0009B598  3C 60 CC 00 */	lis r3, 0xCC005000@ha
/* 8009E65C 0009B59C  A0 03 50 00 */	lhz r0, 0xCC005000@l(r3)
/* 8009E660 0009B5A0  54 03 8F FE */	rlwinm r3, r0, 0x11, 0x1f, 0x1f
/* 8009E664 0009B5A4  4E 80 00 20 */	blr 

.global DSPCheckMailFromDSP
DSPCheckMailFromDSP:
/* 8009E668 0009B5A8  3C 60 CC 00 */	lis r3, 0xCC005004@ha
/* 8009E66C 0009B5AC  A0 03 50 04 */	lhz r0, 0xCC005004@l(r3)
/* 8009E670 0009B5B0  54 03 8F FE */	rlwinm r3, r0, 0x11, 0x1f, 0x1f
/* 8009E674 0009B5B4  4E 80 00 20 */	blr 

.global DSPReadMailFromDSP
DSPReadMailFromDSP:
/* 8009E678 0009B5B8  3C 60 CC 00 */	lis r3, 0xCC005000@ha
/* 8009E67C 0009B5BC  38 63 50 00 */	addi r3, r3, 0xCC005000@l
/* 8009E680 0009B5C0  A0 03 00 04 */	lhz r0, 4(r3)
/* 8009E684 0009B5C4  A0 63 00 06 */	lhz r3, 6(r3)
/* 8009E688 0009B5C8  50 03 80 1E */	rlwimi r3, r0, 0x10, 0, 0xf
/* 8009E68C 0009B5CC  4E 80 00 20 */	blr 

.global DSPSendMailToDSP
DSPSendMailToDSP:
/* 8009E690 0009B5D0  3C 80 CC 00 */	lis r4, 0xCC005000@ha
/* 8009E694 0009B5D4  54 60 84 3E */	srwi r0, r3, 0x10
/* 8009E698 0009B5D8  B0 04 50 00 */	sth r0, 0xCC005000@l(r4)
/* 8009E69C 0009B5DC  B0 64 50 02 */	sth r3, 0x5002(r4)
/* 8009E6A0 0009B5E0  4E 80 00 20 */	blr 

.global DSPAssertInt
DSPAssertInt:
/* 8009E6A4 0009B5E4  7C 08 02 A6 */	mflr r0
/* 8009E6A8 0009B5E8  90 01 00 04 */	stw r0, 4(r1)
/* 8009E6AC 0009B5EC  94 21 FF F8 */	stwu r1, -8(r1)
/* 8009E6B0 0009B5F0  4B FF 1B 1D */	bl OSDisableInterrupts
/* 8009E6B4 0009B5F4  3C 80 CC 00 */	lis r4, 0xCC005000@ha
/* 8009E6B8 0009B5F8  38 84 50 00 */	addi r4, r4, 0xCC005000@l
/* 8009E6BC 0009B5FC  A0 A4 00 0A */	lhz r5, 0xa(r4)
/* 8009E6C0 0009B600  38 00 FF 57 */	li r0, -169
/* 8009E6C4 0009B604  7C A0 00 38 */	and r0, r5, r0
/* 8009E6C8 0009B608  60 00 00 02 */	ori r0, r0, 2
/* 8009E6CC 0009B60C  B0 04 00 0A */	sth r0, 0xa(r4)
/* 8009E6D0 0009B610  4B FF 1B 25 */	bl OSRestoreInterrupts
/* 8009E6D4 0009B614  80 01 00 0C */	lwz r0, 0xc(r1)
/* 8009E6D8 0009B618  38 21 00 08 */	addi r1, r1, 8
/* 8009E6DC 0009B61C  7C 08 03 A6 */	mtlr r0
/* 8009E6E0 0009B620  4E 80 00 20 */	blr 

.global DSPInit
DSPInit:
/* 8009E6E4 0009B624  7C 08 02 A6 */	mflr r0
/* 8009E6E8 0009B628  3C 60 80 3B */	lis r3, "@16"@ha
/* 8009E6EC 0009B62C  90 01 00 04 */	stw r0, 4(r1)
/* 8009E6F0 0009B630  38 63 12 08 */	addi r3, r3, "@16"@l
/* 8009E6F4 0009B634  4C C6 31 82 */	crclr 6
/* 8009E6F8 0009B638  38 83 00 20 */	addi r4, r3, 0x20
/* 8009E6FC 0009B63C  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 8009E700 0009B640  38 A3 00 2C */	addi r5, r3, 0x2c
/* 8009E704 0009B644  93 E1 00 0C */	stw r31, 0xc(r1)
/* 8009E708 0009B648  48 00 00 95 */	bl __DSP_debug_printf
/* 8009E70C 0009B64C  80 0D 95 40 */	lwz r0, __DSP_init_flag@sda21(r13)
/* 8009E710 0009B650  2C 00 00 01 */	cmpwi r0, 1
/* 8009E714 0009B654  41 82 00 74 */	beq lbl_8009E788
/* 8009E718 0009B658  4B FF 1A B5 */	bl OSDisableInterrupts
/* 8009E71C 0009B65C  3C 80 80 08 */	lis r4, __DSPHandler@ha
/* 8009E720 0009B660  3B E3 00 00 */	addi r31, r3, 0
/* 8009E724 0009B664  38 84 21 A0 */	addi r4, r4, __DSPHandler@l
/* 8009E728 0009B668  38 60 00 07 */	li r3, 7
/* 8009E72C 0009B66C  4B FF 1A ED */	bl __OSSetInterruptHandler
/* 8009E730 0009B670  3C 60 01 00 */	lis r3, 0x100
/* 8009E734 0009B674  4B FF 1E E9 */	bl __OSUnmaskInterrupts
/* 8009E738 0009B678  3C 60 CC 00 */	lis r3, 0xCC005000@ha
/* 8009E73C 0009B67C  38 C3 50 00 */	addi r6, r3, 0xCC005000@l
/* 8009E740 0009B680  A0 63 50 0A */	lhz r3, 0x500a(r3)
/* 8009E744 0009B684  38 00 FF 57 */	li r0, -169
/* 8009E748 0009B688  7C 60 00 38 */	and r0, r3, r0
/* 8009E74C 0009B68C  60 00 08 00 */	ori r0, r0, 0x800
/* 8009E750 0009B690  B0 06 00 0A */	sth r0, 0xa(r6)
/* 8009E754 0009B694  38 A0 FF 53 */	li r5, -173
/* 8009E758 0009B698  38 80 00 00 */	li r4, 0
/* 8009E75C 0009B69C  A0 E6 00 0A */	lhz r7, 0xa(r6)
/* 8009E760 0009B6A0  38 00 00 01 */	li r0, 1
/* 8009E764 0009B6A4  38 7F 00 00 */	addi r3, r31, 0
/* 8009E768 0009B6A8  7C E5 28 38 */	and r5, r7, r5
/* 8009E76C 0009B6AC  B0 A6 00 0A */	sth r5, 0xa(r6)
/* 8009E770 0009B6B0  90 8D 92 70 */	stw r4, __DSP_tmp_task@sda21(r13)
/* 8009E774 0009B6B4  90 8D 95 50 */	stw r4, __DSP_curr_task@sda21(r13)
/* 8009E778 0009B6B8  90 8D 95 48 */	stw r4, __DSP_last_task@sda21(r13)
/* 8009E77C 0009B6BC  90 8D 95 4C */	stw r4, __DSP_first_task@sda21(r13)
/* 8009E780 0009B6C0  90 0D 95 40 */	stw r0, __DSP_init_flag@sda21(r13)
/* 8009E784 0009B6C4  4B FF 1A 71 */	bl OSRestoreInterrupts
lbl_8009E788:
/* 8009E788 0009B6C8  80 01 00 14 */	lwz r0, 0x14(r1)
/* 8009E78C 0009B6CC  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 8009E790 0009B6D0  38 21 00 10 */	addi r1, r1, 0x10
/* 8009E794 0009B6D4  7C 08 03 A6 */	mtlr r0
/* 8009E798 0009B6D8  4E 80 00 20 */	blr 

.section .data, "wa"  # 0x803A8380 - 0x803E6000
"@16":
	.asciz "DSPInit(): Build Date: %s %s\n"
	.balign 4
"@17":
	.asciz "Dec 17 2001"
"@18":
	.asciz "18:25:00"
	.balign 4

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
.global __DSP_init_flag
__DSP_init_flag:
	.skip 0x8

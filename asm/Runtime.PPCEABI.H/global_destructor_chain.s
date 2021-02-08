.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __destroy_global_chain
__destroy_global_chain:
/* 800826E0 0007F620  7C 08 02 A6 */	mflr r0
/* 800826E4 0007F624  90 01 00 04 */	stw r0, 4(r1)
/* 800826E8 0007F628  94 21 FF F8 */	stwu r1, -8(r1)
/* 800826EC 0007F62C  48 00 00 20 */	b lbl_8008270C
lbl_800826F0:
/* 800826F0 0007F630  80 03 00 00 */	lwz r0, 0(r3)
/* 800826F4 0007F634  38 80 FF FF */	li r4, -1
/* 800826F8 0007F638  90 0D 92 88 */	stw r0, __global_destructor_chain-_SDA_BASE_(r13)
/* 800826FC 0007F63C  81 83 00 04 */	lwz r12, 4(r3)
/* 80082700 0007F640  80 63 00 08 */	lwz r3, 8(r3)
/* 80082704 0007F644  7D 88 03 A6 */	mtlr r12
/* 80082708 0007F648  4E 80 00 21 */	blrl 
lbl_8008270C:
/* 8008270C 0007F64C  80 6D 92 88 */	lwz r3, __global_destructor_chain-_SDA_BASE_(r13)
/* 80082710 0007F650  28 03 00 00 */	cmplwi r3, 0
/* 80082714 0007F654  40 82 FF DC */	bne lbl_800826F0
/* 80082718 0007F658  80 01 00 0C */	lwz r0, 0xc(r1)
/* 8008271C 0007F65C  38 21 00 08 */	addi r1, r1, 8
/* 80082720 0007F660  7C 08 03 A6 */	mtlr r0
/* 80082724 0007F664  4E 80 00 20 */	blr 

.global __register_global_object
__register_global_object:
/* 80082728 0007F668  80 0D 92 88 */	lwz r0, __global_destructor_chain-_SDA_BASE_(r13)
/* 8008272C 0007F66C  90 05 00 00 */	stw r0, 0(r5)
/* 80082730 0007F670  90 85 00 04 */	stw r4, 4(r5)
/* 80082734 0007F674  90 65 00 08 */	stw r3, 8(r5)
/* 80082738 0007F678  90 AD 92 88 */	stw r5, __global_destructor_chain-_SDA_BASE_(r13)
/* 8008273C 0007F67C  4E 80 00 20 */	blr 

.section .dtors, "wa"  # 0x8036FF80 - 0x8036FFA0
__destroy_global_chain_reference:
	.incbin "baserom.dol", 0x36CF88, 0x4

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
__global_destructor_chain:
	.skip 0x8

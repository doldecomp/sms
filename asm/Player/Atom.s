.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global setUp__17TMapCollisionBaseFv
setUp__17TMapCollisionBaseFv:
/* 8011E7C8 0011B708  A0 03 00 5C */	lhz r0, 0x5c(r3)
/* 8011E7CC 0011B70C  54 00 00 3C */	rlwinm r0, r0, 0, 0, 0x1e
/* 8011E7D0 0011B710  B0 03 00 5C */	sth r0, 0x5c(r3)
/* 8011E7D4 0011B714  4E 80 00 20 */	blr 

.global __sinit_Atom_cpp
__sinit_Atom_cpp:
/* 8011E7D8 0011B718  C0 22 97 80 */	lfs f1, $$21908-_SDA2_BASE_(r2)
/* 8011E7DC 0011B71C  3C 60 80 3F */	lis r3, cDeformedTerrainCenter@ha
/* 8011E7E0 0011B720  D4 23 2E 60 */	stfsu f1, cDeformedTerrainCenter@l(r3)
/* 8011E7E4 0011B724  C0 02 97 84 */	lfs f0, $$22110-_SDA2_BASE_(r2)
/* 8011E7E8 0011B728  D0 03 00 04 */	stfs f0, 4(r3)
/* 8011E7EC 0011B72C  D0 23 00 08 */	stfs f1, 8(r3)
/* 8011E7F0 0011B730  4E 80 00 20 */	blr 

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
	.incbin "baserom.dol", 0x36CC5C, 0x4

.section .sdata2, "wa"  # 0x8040B460 - 0x80414020
$$21908:
	.incbin "baserom.dol", 0x3E5480, 0x4
$$22110:
	.incbin "baserom.dol", 0x3E5484, 0x4

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
cDeformedTerrainCenter:
	.skip 0x10

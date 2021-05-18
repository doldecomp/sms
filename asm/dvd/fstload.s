.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global cb
cb:
/* 800993F8 00096338  7C 08 02 A6 */	mflr r0
/* 800993FC 0009633C  2C 03 00 00 */	cmpwi r3, 0
/* 80099400 00096340  90 01 00 04 */	stw r0, 4(r1)
/* 80099404 00096344  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80099408 00096348  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8009940C 0009634C  3B E4 00 00 */	addi r31, r4, 0
/* 80099410 00096350  40 81 00 7C */	ble lbl_8009948C
/* 80099414 00096354  80 0D 94 30 */	lwz r0, status-_SDA_BASE_(r13)
/* 80099418 00096358  2C 00 00 01 */	cmpwi r0, 1
/* 8009941C 0009635C  41 82 00 3C */	beq lbl_80099458
/* 80099420 00096360  40 80 00 9C */	bge lbl_800994BC
/* 80099424 00096364  2C 00 00 00 */	cmpwi r0, 0
/* 80099428 00096368  40 80 00 08 */	bge lbl_80099430
/* 8009942C 0009636C  48 00 00 90 */	b lbl_800994BC
lbl_80099430:
/* 80099430 00096370  38 00 00 01 */	li r0, 1
/* 80099434 00096374  80 8D 94 34 */	lwz r4, bb2-_SDA_BASE_(r13)
/* 80099438 00096378  3C 60 80 0A */	lis r3, cb@ha
/* 8009943C 0009637C  90 0D 94 30 */	stw r0, status-_SDA_BASE_(r13)
/* 80099440 00096380  38 E3 93 F8 */	addi r7, r3, cb@l
/* 80099444 00096384  38 7F 00 00 */	addi r3, r31, 0
/* 80099448 00096388  38 A0 00 20 */	li r5, 0x20
/* 8009944C 0009638C  38 C0 04 20 */	li r6, 0x420
/* 80099450 00096390  4B FF F0 19 */	bl DVDReadAbsAsyncForBS
/* 80099454 00096394  48 00 00 68 */	b lbl_800994BC
lbl_80099458:
/* 80099458 00096398  38 00 00 02 */	li r0, 2
/* 8009945C 0009639C  80 CD 94 34 */	lwz r6, bb2-_SDA_BASE_(r13)
/* 80099460 000963A0  90 0D 94 30 */	stw r0, status-_SDA_BASE_(r13)
/* 80099464 000963A4  3C 60 80 0A */	lis r3, cb@ha
/* 80099468 000963A8  38 E3 93 F8 */	addi r7, r3, cb@l
/* 8009946C 000963AC  80 A6 00 08 */	lwz r5, 8(r6)
/* 80099470 000963B0  7F E3 FB 78 */	mr r3, r31
/* 80099474 000963B4  80 86 00 10 */	lwz r4, 0x10(r6)
/* 80099478 000963B8  38 05 00 1F */	addi r0, r5, 0x1f
/* 8009947C 000963BC  80 C6 00 04 */	lwz r6, 4(r6)
/* 80099480 000963C0  54 05 00 34 */	rlwinm r5, r0, 0, 0, 0x1a
/* 80099484 000963C4  4B FF EF E5 */	bl DVDReadAbsAsyncForBS
/* 80099488 000963C8  48 00 00 34 */	b lbl_800994BC
lbl_8009948C:
/* 8009948C 000963CC  2C 03 FF FF */	cmpwi r3, -1
/* 80099490 000963D0  41 82 00 2C */	beq lbl_800994BC
/* 80099494 000963D4  2C 03 FF FC */	cmpwi r3, -4
/* 80099498 000963D8  40 82 00 24 */	bne lbl_800994BC
/* 8009949C 000963DC  38 00 00 00 */	li r0, 0
/* 800994A0 000963E0  90 0D 94 30 */	stw r0, status-_SDA_BASE_(r13)
/* 800994A4 000963E4  4B FF F5 31 */	bl DVDReset
/* 800994A8 000963E8  3C 60 80 0A */	lis r3, cb@ha
/* 800994AC 000963EC  80 8D 94 38 */	lwz r4, idTmp-_SDA_BASE_(r13)
/* 800994B0 000963F0  38 A3 93 F8 */	addi r5, r3, cb@l
/* 800994B4 000963F4  38 7F 00 00 */	addi r3, r31, 0
/* 800994B8 000963F8  4B FF F0 81 */	bl DVDReadDiskID
lbl_800994BC:
/* 800994BC 000963FC  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 800994C0 00096400  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 800994C4 00096404  38 21 00 18 */	addi r1, r1, 0x18
/* 800994C8 00096408  7C 08 03 A6 */	mtlr r0
/* 800994CC 0009640C  4E 80 00 20 */	blr 

.global __fstLoad
__fstLoad:
/* 800994D0 00096410  7C 08 02 A6 */	mflr r0
/* 800994D4 00096414  3C 60 80 3B */	lis r3, $$236@ha
/* 800994D8 00096418  90 01 00 04 */	stw r0, 4(r1)
/* 800994DC 0009641C  94 21 FF A0 */	stwu r1, -0x60(r1)
/* 800994E0 00096420  93 E1 00 5C */	stw r31, 0x5c(r1)
/* 800994E4 00096424  3B E3 0E C0 */	addi r31, r3, $$236@l
/* 800994E8 00096428  93 C1 00 58 */	stw r30, 0x58(r1)
/* 800994EC 0009642C  93 A1 00 54 */	stw r29, 0x54(r1)
/* 800994F0 00096430  4B FF 47 C5 */	bl OSGetArenaHi
/* 800994F4 00096434  3C 60 80 3F */	lis r3, bb2Buf@ha
/* 800994F8 00096438  38 63 BD 58 */	addi r3, r3, bb2Buf@l
/* 800994FC 0009643C  38 81 00 2B */	addi r4, r1, 0x2b
/* 80099500 00096440  38 03 00 1F */	addi r0, r3, 0x1f
/* 80099504 00096444  54 83 00 34 */	rlwinm r3, r4, 0, 0, 0x1a
/* 80099508 00096448  54 00 00 34 */	rlwinm r0, r0, 0, 0, 0x1a
/* 8009950C 0009644C  90 6D 94 38 */	stw r3, idTmp-_SDA_BASE_(r13)
/* 80099510 00096450  90 0D 94 34 */	stw r0, bb2-_SDA_BASE_(r13)
/* 80099514 00096454  4B FF F4 C1 */	bl DVDReset
/* 80099518 00096458  3C 60 80 3F */	lis r3, block$16@ha
/* 8009951C 0009645C  80 8D 94 38 */	lwz r4, idTmp-_SDA_BASE_(r13)
/* 80099520 00096460  3C A0 80 0A */	lis r5, cb@ha
/* 80099524 00096464  38 63 BD 98 */	addi r3, r3, block$16@l
/* 80099528 00096468  38 A5 93 F8 */	addi r5, r5, cb@l
/* 8009952C 0009646C  4B FF F0 0D */	bl DVDReadDiskID
lbl_80099530:
/* 80099530 00096470  4B FF F5 35 */	bl DVDGetDriveStatus
/* 80099534 00096474  2C 03 00 00 */	cmpwi r3, 0
/* 80099538 00096478  40 82 FF F8 */	bne lbl_80099530
/* 8009953C 0009647C  80 6D 94 34 */	lwz r3, bb2-_SDA_BASE_(r13)
/* 80099540 00096480  3F A0 80 00 */	lis r29, 0x80000000@ha
/* 80099544 00096484  3F C0 80 00 */	lis r30, 0x80000038@ha
/* 80099548 00096488  80 03 00 10 */	lwz r0, 0x10(r3)
/* 8009954C 0009648C  38 7D 00 00 */	addi r3, r29, 0x80000000@l
/* 80099550 00096490  38 A0 00 20 */	li r5, 0x20
/* 80099554 00096494  90 1E 00 38 */	stw r0, 0x80000038@l(r30)
/* 80099558 00096498  80 8D 94 34 */	lwz r4, bb2-_SDA_BASE_(r13)
/* 8009955C 0009649C  80 04 00 0C */	lwz r0, 0xc(r4)
/* 80099560 000964A0  90 1E 00 3C */	stw r0, 0x3c(r30)
/* 80099564 000964A4  80 8D 94 38 */	lwz r4, idTmp-_SDA_BASE_(r13)
/* 80099568 000964A8  4B F6 9C 8D */	bl memcpy
/* 8009956C 000964AC  38 6D 82 80 */	addi r3, r13, $$235-_SDA_BASE_
/* 80099570 000964B0  4C C6 31 82 */	crclr 6
/* 80099574 000964B4  4B FF 59 F1 */	bl OSReport
/* 80099578 000964B8  88 9D 00 00 */	lbz r4, 0(r29)
/* 8009957C 000964BC  7F E3 FB 78 */	mr r3, r31
/* 80099580 000964C0  88 BD 00 01 */	lbz r5, 1(r29)
/* 80099584 000964C4  4C C6 31 82 */	crclr 6
/* 80099588 000964C8  88 DD 00 02 */	lbz r6, 2(r29)
/* 8009958C 000964CC  88 FD 00 03 */	lbz r7, 3(r29)
/* 80099590 000964D0  7C 84 07 74 */	extsb r4, r4
/* 80099594 000964D4  7C A5 07 74 */	extsb r5, r5
/* 80099598 000964D8  7C C6 07 74 */	extsb r6, r6
/* 8009959C 000964DC  7C E7 07 74 */	extsb r7, r7
/* 800995A0 000964E0  4B FF 59 C5 */	bl OSReport
/* 800995A4 000964E4  88 9D 00 04 */	lbz r4, 4(r29)
/* 800995A8 000964E8  38 7F 00 1C */	addi r3, r31, 0x1c
/* 800995AC 000964EC  88 BD 00 05 */	lbz r5, 5(r29)
/* 800995B0 000964F0  4C C6 31 82 */	crclr 6
/* 800995B4 000964F4  7C 84 07 74 */	extsb r4, r4
/* 800995B8 000964F8  7C A5 07 74 */	extsb r5, r5
/* 800995BC 000964FC  4B FF 59 A9 */	bl OSReport
/* 800995C0 00096500  88 9D 00 06 */	lbz r4, 6(r29)
/* 800995C4 00096504  38 7F 00 34 */	addi r3, r31, 0x34
/* 800995C8 00096508  4C C6 31 82 */	crclr 6
/* 800995CC 0009650C  4B FF 59 99 */	bl OSReport
/* 800995D0 00096510  88 9D 00 07 */	lbz r4, 7(r29)
/* 800995D4 00096514  38 7F 00 48 */	addi r3, r31, 0x48
/* 800995D8 00096518  4C C6 31 82 */	crclr 6
/* 800995DC 0009651C  4B FF 59 89 */	bl OSReport
/* 800995E0 00096520  88 1E 00 08 */	lbz r0, 8(r30)
/* 800995E4 00096524  28 00 00 00 */	cmplwi r0, 0
/* 800995E8 00096528  40 82 00 0C */	bne lbl_800995F4
/* 800995EC 0009652C  38 8D 82 84 */	addi r4, r13, $$240-_SDA_BASE_
/* 800995F0 00096530  48 00 00 08 */	b lbl_800995F8
lbl_800995F4:
/* 800995F4 00096534  38 8D 82 88 */	addi r4, r13, $$241-_SDA_BASE_
lbl_800995F8:
/* 800995F8 00096538  4C C6 31 82 */	crclr 6
/* 800995FC 0009653C  38 7F 00 5C */	addi r3, r31, 0x5c
/* 80099600 00096540  4B FF 59 65 */	bl OSReport
/* 80099604 00096544  38 6D 82 80 */	addi r3, r13, $$235-_SDA_BASE_
/* 80099608 00096548  4C C6 31 82 */	crclr 6
/* 8009960C 0009654C  4B FF 59 59 */	bl OSReport
/* 80099610 00096550  80 6D 94 34 */	lwz r3, bb2-_SDA_BASE_(r13)
/* 80099614 00096554  80 63 00 10 */	lwz r3, 0x10(r3)
/* 80099618 00096558  4B FF 46 AD */	bl OSSetArenaHi
/* 8009961C 0009655C  80 01 00 64 */	lwz r0, 0x64(r1)
/* 80099620 00096560  83 E1 00 5C */	lwz r31, 0x5c(r1)
/* 80099624 00096564  83 C1 00 58 */	lwz r30, 0x58(r1)
/* 80099628 00096568  83 A1 00 54 */	lwz r29, 0x54(r1)
/* 8009962C 0009656C  38 21 00 60 */	addi r1, r1, 0x60
/* 80099630 00096570  7C 08 03 A6 */	mtlr r0
/* 80099634 00096574  4E 80 00 20 */	blr 

.section .data, "wa"  # 0x803A8380 - 0x803E6000
$$236:
	.incbin "baserom.dol", 0x3ADEC0, 0x1C
$$237:
	.incbin "baserom.dol", 0x3ADEDC, 0x18
$$238:
	.incbin "baserom.dol", 0x3ADEF4, 0x14
$$239:
	.incbin "baserom.dol", 0x3ADF08, 0x14
$$242:
	.incbin "baserom.dol", 0x3ADF1C, 0x14

.section .sdata, "wa"  # 0x80408AC0 - 0x804097C0
$$235:
	.incbin "baserom.dol", 0x3E3280, 0x4
$$240:
	.incbin "baserom.dol", 0x3E3284, 0x4
$$241:
	.incbin "baserom.dol", 0x3E3288, 0x8

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
bb2Buf:
	.skip 0x40
block$16:
	.skip 0x30

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
status:
	.skip 0x4
bb2:
	.skip 0x4
idTmp:
	.skip 0x8
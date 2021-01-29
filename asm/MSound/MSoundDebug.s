.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0

.global __ct__10JADPrm$$0Uc$$1FUcPCc
__ct__10JADPrm$$0Uc$$1FUcPCc:
/* 80180598 0017D4D8  98 83 00 00 */	stb r4, 0(r3)
/* 8018059C 0017D4DC  4E 80 00 20 */	blr 

.global __sinit_MSoundDebug_cpp
__sinit_MSoundDebug_cpp:
/* 801805A0 0017D4E0  7C 08 02 A6 */	mflr r0
/* 801805A4 0017D4E4  3C 60 80 3F */	lis r3, $$21951@ha
/* 801805A8 0017D4E8  90 01 00 04 */	stw r0, 4(r1)
/* 801805AC 0017D4EC  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 801805B0 0017D4F0  93 E1 00 0C */	stw r31, 0xc(r1)
/* 801805B4 0017D4F4  3B E3 47 00 */	addi r31, r3, $$21951@l
/* 801805B8 0017D4F8  88 0D 97 CC */	lbz r0, __init__smList__15JALList$$05MSBgm$$1-_SDA_BASE_(r13)
/* 801805BC 0017D4FC  7C 00 07 75 */	extsb. r0, r0
/* 801805C0 0017D500  40 82 00 28 */	bne lbl_801805E8
/* 801805C4 0017D504  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1-_SDA_BASE_
/* 801805C8 0017D508  4B E8 E6 6D */	bl initiate__10JSUPtrListFv
/* 801805CC 0017D50C  3C 60 80 0E */	lis r3, __dt__15JSUList$$05MSBgm$$1Fv@ha
/* 801805D0 0017D510  38 83 6A 44 */	addi r4, r3, __dt__15JSUList$$05MSBgm$$1Fv@l
/* 801805D4 0017D514  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1-_SDA_BASE_
/* 801805D8 0017D518  38 BF 00 00 */	addi r5, r31, 0
/* 801805DC 0017D51C  4B F0 21 4D */	bl __register_global_object
/* 801805E0 0017D520  38 00 00 01 */	li r0, 1
/* 801805E4 0017D524  98 0D 97 CC */	stb r0, __init__smList__15JALList$$05MSBgm$$1-_SDA_BASE_(r13)
lbl_801805E8:
/* 801805E8 0017D528  88 0D 97 CD */	lbz r0, __init__smList__24JALList$$013MSSetSoundGrp$$1-_SDA_BASE_(r13)
/* 801805EC 0017D52C  7C 00 07 75 */	extsb. r0, r0
/* 801805F0 0017D530  40 82 00 28 */	bne lbl_80180618
/* 801805F4 0017D534  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1-_SDA_BASE_
/* 801805F8 0017D538  4B E8 E6 3D */	bl initiate__10JSUPtrListFv
/* 801805FC 0017D53C  3C 60 80 0E */	lis r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@ha
/* 80180600 0017D540  38 83 69 EC */	addi r4, r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@l
/* 80180604 0017D544  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1-_SDA_BASE_
/* 80180608 0017D548  38 BF 00 0C */	addi r5, r31, 0xc
/* 8018060C 0017D54C  4B F0 21 1D */	bl __register_global_object
/* 80180610 0017D550  38 00 00 01 */	li r0, 1
/* 80180614 0017D554  98 0D 97 CD */	stb r0, __init__smList__24JALList$$013MSSetSoundGrp$$1-_SDA_BASE_(r13)
lbl_80180618:
/* 80180618 0017D558  88 0D 97 CE */	lbz r0, __init__smList__21JALList$$010MSSetSound$$1-_SDA_BASE_(r13)
/* 8018061C 0017D55C  7C 00 07 75 */	extsb. r0, r0
/* 80180620 0017D560  40 82 00 28 */	bne lbl_80180648
/* 80180624 0017D564  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1-_SDA_BASE_
/* 80180628 0017D568  4B E8 E6 0D */	bl initiate__10JSUPtrListFv
/* 8018062C 0017D56C  3C 60 80 0E */	lis r3, __dt__21JSUList$$010MSSetSound$$1Fv@ha
/* 80180630 0017D570  38 83 69 94 */	addi r4, r3, __dt__21JSUList$$010MSSetSound$$1Fv@l
/* 80180634 0017D574  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1-_SDA_BASE_
/* 80180638 0017D578  38 BF 00 18 */	addi r5, r31, 0x18
/* 8018063C 0017D57C  4B F0 20 ED */	bl __register_global_object
/* 80180640 0017D580  38 00 00 01 */	li r0, 1
/* 80180644 0017D584  98 0D 97 CE */	stb r0, __init__smList__21JALList$$010MSSetSound$$1-_SDA_BASE_(r13)
lbl_80180648:
/* 80180648 0017D588  88 0D 8F 8C */	lbz r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1-_SDA_BASE_(r13)
/* 8018064C 0017D58C  7C 00 07 75 */	extsb. r0, r0
/* 80180650 0017D590  40 82 00 28 */	bne lbl_80180678
/* 80180654 0017D594  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1-_SDA_BASE_
/* 80180658 0017D598  4B E8 E5 DD */	bl initiate__10JSUPtrListFv
/* 8018065C 0017D59C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@ha
/* 80180660 0017D5A0  38 83 A8 0C */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@l
/* 80180664 0017D5A4  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1-_SDA_BASE_
/* 80180668 0017D5A8  38 BF 00 24 */	addi r5, r31, 0x24
/* 8018066C 0017D5AC  4B F0 20 BD */	bl __register_global_object
/* 80180670 0017D5B0  38 00 00 01 */	li r0, 1
/* 80180674 0017D5B4  98 0D 8F 8C */	stb r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1-_SDA_BASE_(r13)
lbl_80180678:
/* 80180678 0017D5B8  88 0D 8F 8D */	lbz r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1-_SDA_BASE_(r13)
/* 8018067C 0017D5BC  7C 00 07 75 */	extsb. r0, r0
/* 80180680 0017D5C0  40 82 00 28 */	bne lbl_801806A8
/* 80180684 0017D5C4  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1-_SDA_BASE_
/* 80180688 0017D5C8  4B E8 E5 AD */	bl initiate__10JSUPtrListFv
/* 8018068C 0017D5CC  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@ha
/* 80180690 0017D5D0  38 83 A7 B4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@l
/* 80180694 0017D5D4  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1-_SDA_BASE_
/* 80180698 0017D5D8  38 BF 00 30 */	addi r5, r31, 0x30
/* 8018069C 0017D5DC  4B F0 20 8D */	bl __register_global_object
/* 801806A0 0017D5E0  38 00 00 01 */	li r0, 1
/* 801806A4 0017D5E4  98 0D 8F 8D */	stb r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1-_SDA_BASE_(r13)
lbl_801806A8:
/* 801806A8 0017D5E8  88 0D 8F 8E */	lbz r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1-_SDA_BASE_(r13)
/* 801806AC 0017D5EC  7C 00 07 75 */	extsb. r0, r0
/* 801806B0 0017D5F0  40 82 00 28 */	bne lbl_801806D8
/* 801806B4 0017D5F4  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1-_SDA_BASE_
/* 801806B8 0017D5F8  4B E8 E5 7D */	bl initiate__10JSUPtrListFv
/* 801806BC 0017D5FC  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@ha
/* 801806C0 0017D600  38 83 A7 5C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@l
/* 801806C4 0017D604  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1-_SDA_BASE_
/* 801806C8 0017D608  38 BF 00 3C */	addi r5, r31, 0x3c
/* 801806CC 0017D60C  4B F0 20 5D */	bl __register_global_object
/* 801806D0 0017D610  38 00 00 01 */	li r0, 1
/* 801806D4 0017D614  98 0D 8F 8E */	stb r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1-_SDA_BASE_(r13)
lbl_801806D8:
/* 801806D8 0017D618  88 0D 8F 8F */	lbz r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1-_SDA_BASE_(r13)
/* 801806DC 0017D61C  7C 00 07 75 */	extsb. r0, r0
/* 801806E0 0017D620  40 82 00 28 */	bne lbl_80180708
/* 801806E4 0017D624  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1-_SDA_BASE_
/* 801806E8 0017D628  4B E8 E5 4D */	bl initiate__10JSUPtrListFv
/* 801806EC 0017D62C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@ha
/* 801806F0 0017D630  38 83 A7 04 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@l
/* 801806F4 0017D634  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1-_SDA_BASE_
/* 801806F8 0017D638  38 BF 00 48 */	addi r5, r31, 0x48
/* 801806FC 0017D63C  4B F0 20 2D */	bl __register_global_object
/* 80180700 0017D640  38 00 00 01 */	li r0, 1
/* 80180704 0017D644  98 0D 8F 8F */	stb r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1-_SDA_BASE_(r13)
lbl_80180708:
/* 80180708 0017D648  88 0D 8F 90 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1-_SDA_BASE_(r13)
/* 8018070C 0017D64C  7C 00 07 75 */	extsb. r0, r0
/* 80180710 0017D650  40 82 00 28 */	bne lbl_80180738
/* 80180714 0017D654  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1-_SDA_BASE_
/* 80180718 0017D658  4B E8 E5 1D */	bl initiate__10JSUPtrListFv
/* 8018071C 0017D65C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@ha
/* 80180720 0017D660  38 83 A6 AC */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@l
/* 80180724 0017D664  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1-_SDA_BASE_
/* 80180728 0017D668  38 BF 00 54 */	addi r5, r31, 0x54
/* 8018072C 0017D66C  4B F0 1F FD */	bl __register_global_object
/* 80180730 0017D670  38 00 00 01 */	li r0, 1
/* 80180734 0017D674  98 0D 8F 90 */	stb r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1-_SDA_BASE_(r13)
lbl_80180738:
/* 80180738 0017D678  88 0D 8F 91 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1-_SDA_BASE_(r13)
/* 8018073C 0017D67C  7C 00 07 75 */	extsb. r0, r0
/* 80180740 0017D680  40 82 00 28 */	bne lbl_80180768
/* 80180744 0017D684  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1-_SDA_BASE_
/* 80180748 0017D688  4B E8 E4 ED */	bl initiate__10JSUPtrListFv
/* 8018074C 0017D68C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@ha
/* 80180750 0017D690  38 83 A6 54 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@l
/* 80180754 0017D694  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1-_SDA_BASE_
/* 80180758 0017D698  38 BF 00 60 */	addi r5, r31, 0x60
/* 8018075C 0017D69C  4B F0 1F CD */	bl __register_global_object
/* 80180760 0017D6A0  38 00 00 01 */	li r0, 1
/* 80180764 0017D6A4  98 0D 8F 91 */	stb r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1-_SDA_BASE_(r13)
lbl_80180768:
/* 80180768 0017D6A8  88 0D 8F 92 */	lbz r0, __init__smList__26JALList$$015JALSeModEffDist$$1-_SDA_BASE_(r13)
/* 8018076C 0017D6AC  7C 00 07 75 */	extsb. r0, r0
/* 80180770 0017D6B0  40 82 00 28 */	bne lbl_80180798
/* 80180774 0017D6B4  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1-_SDA_BASE_
/* 80180778 0017D6B8  4B E8 E4 BD */	bl initiate__10JSUPtrListFv
/* 8018077C 0017D6BC  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@ha
/* 80180780 0017D6C0  38 83 A5 FC */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@l
/* 80180784 0017D6C4  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1-_SDA_BASE_
/* 80180788 0017D6C8  38 BF 00 6C */	addi r5, r31, 0x6c
/* 8018078C 0017D6CC  4B F0 1F 9D */	bl __register_global_object
/* 80180790 0017D6D0  38 00 00 01 */	li r0, 1
/* 80180794 0017D6D4  98 0D 8F 92 */	stb r0, __init__smList__26JALList$$015JALSeModEffDist$$1-_SDA_BASE_(r13)
lbl_80180798:
/* 80180798 0017D6D8  88 0D 8F 93 */	lbz r0, __init__smList__26JALList$$015JALSeModPitDist$$1-_SDA_BASE_(r13)
/* 8018079C 0017D6DC  7C 00 07 75 */	extsb. r0, r0
/* 801807A0 0017D6E0  40 82 00 28 */	bne lbl_801807C8
/* 801807A4 0017D6E4  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1-_SDA_BASE_
/* 801807A8 0017D6E8  4B E8 E4 8D */	bl initiate__10JSUPtrListFv
/* 801807AC 0017D6EC  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@ha
/* 801807B0 0017D6F0  38 83 A5 A4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@l
/* 801807B4 0017D6F4  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1-_SDA_BASE_
/* 801807B8 0017D6F8  38 BF 00 78 */	addi r5, r31, 0x78
/* 801807BC 0017D6FC  4B F0 1F 6D */	bl __register_global_object
/* 801807C0 0017D700  38 00 00 01 */	li r0, 1
/* 801807C4 0017D704  98 0D 8F 93 */	stb r0, __init__smList__26JALList$$015JALSeModPitDist$$1-_SDA_BASE_(r13)
lbl_801807C8:
/* 801807C8 0017D708  88 0D 8F 94 */	lbz r0, __init__smList__26JALList$$015JALSeModVolDist$$1-_SDA_BASE_(r13)
/* 801807CC 0017D70C  7C 00 07 75 */	extsb. r0, r0
/* 801807D0 0017D710  40 82 00 28 */	bne lbl_801807F8
/* 801807D4 0017D714  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1-_SDA_BASE_
/* 801807D8 0017D718  4B E8 E4 5D */	bl initiate__10JSUPtrListFv
/* 801807DC 0017D71C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@ha
/* 801807E0 0017D720  38 83 A5 4C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@l
/* 801807E4 0017D724  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1-_SDA_BASE_
/* 801807E8 0017D728  38 BF 00 84 */	addi r5, r31, 0x84
/* 801807EC 0017D72C  4B F0 1F 3D */	bl __register_global_object
/* 801807F0 0017D730  38 00 00 01 */	li r0, 1
/* 801807F4 0017D734  98 0D 8F 94 */	stb r0, __init__smList__26JALList$$015JALSeModVolDist$$1-_SDA_BASE_(r13)
lbl_801807F8:
/* 801807F8 0017D738  88 0D 8F 95 */	lbz r0, __init__smList__26JALList$$015JALSeModEffFunk$$1-_SDA_BASE_(r13)
/* 801807FC 0017D73C  7C 00 07 75 */	extsb. r0, r0
/* 80180800 0017D740  40 82 00 28 */	bne lbl_80180828
/* 80180804 0017D744  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1-_SDA_BASE_
/* 80180808 0017D748  4B E8 E4 2D */	bl initiate__10JSUPtrListFv
/* 8018080C 0017D74C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@ha
/* 80180810 0017D750  38 83 A4 F4 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@l
/* 80180814 0017D754  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1-_SDA_BASE_
/* 80180818 0017D758  38 BF 00 90 */	addi r5, r31, 0x90
/* 8018081C 0017D75C  4B F0 1F 0D */	bl __register_global_object
/* 80180820 0017D760  38 00 00 01 */	li r0, 1
/* 80180824 0017D764  98 0D 8F 95 */	stb r0, __init__smList__26JALList$$015JALSeModEffFunk$$1-_SDA_BASE_(r13)
lbl_80180828:
/* 80180828 0017D768  88 0D 8F 96 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFunk$$1-_SDA_BASE_(r13)
/* 8018082C 0017D76C  7C 00 07 75 */	extsb. r0, r0
/* 80180830 0017D770  40 82 00 28 */	bne lbl_80180858
/* 80180834 0017D774  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1-_SDA_BASE_
/* 80180838 0017D778  4B E8 E3 FD */	bl initiate__10JSUPtrListFv
/* 8018083C 0017D77C  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@ha
/* 80180840 0017D780  38 83 A4 9C */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@l
/* 80180844 0017D784  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1-_SDA_BASE_
/* 80180848 0017D788  38 BF 00 9C */	addi r5, r31, 0x9c
/* 8018084C 0017D78C  4B F0 1E DD */	bl __register_global_object
/* 80180850 0017D790  38 00 00 01 */	li r0, 1
/* 80180854 0017D794  98 0D 8F 96 */	stb r0, __init__smList__26JALList$$015JALSeModPitFunk$$1-_SDA_BASE_(r13)
lbl_80180858:
/* 80180858 0017D798  88 0D 8F 97 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFunk$$1-_SDA_BASE_(r13)
/* 8018085C 0017D79C  7C 00 07 75 */	extsb. r0, r0
/* 80180860 0017D7A0  40 82 00 28 */	bne lbl_80180888
/* 80180864 0017D7A4  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1-_SDA_BASE_
/* 80180868 0017D7A8  4B E8 E3 CD */	bl initiate__10JSUPtrListFv
/* 8018086C 0017D7AC  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@ha
/* 80180870 0017D7B0  38 83 A4 44 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@l
/* 80180874 0017D7B4  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1-_SDA_BASE_
/* 80180878 0017D7B8  38 BF 00 A8 */	addi r5, r31, 0xa8
/* 8018087C 0017D7BC  4B F0 1E AD */	bl __register_global_object
/* 80180880 0017D7C0  38 00 00 01 */	li r0, 1
/* 80180884 0017D7C4  98 0D 8F 97 */	stb r0, __init__smList__26JALList$$015JALSeModVolFunk$$1-_SDA_BASE_(r13)
lbl_80180888:
/* 80180888 0017D7C8  80 01 00 14 */	lwz r0, 0x14(r1)
/* 8018088C 0017D7CC  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 80180890 0017D7D0  38 21 00 10 */	addi r1, r1, 0x10
/* 80180894 0017D7D4  7C 08 03 A6 */	mtlr r0
/* 80180898 0017D7D8  4E 80 00 20 */	blr 

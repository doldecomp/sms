.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global preEntry__12TMarDirectorFP12TPerformList
preEntry__12TMarDirectorFP12TPerformList:
/* 800EF6B4 000EC5F4  7C 08 02 A6 */	mflr r0
/* 800EF6B8 000EC5F8  3C A0 80 37 */	lis r5, $$21490@ha
/* 800EF6BC 000EC5FC  90 01 00 04 */	stw r0, 4(r1)
/* 800EF6C0 000EC600  94 21 FF 18 */	stwu r1, -0xe8(r1)
/* 800EF6C4 000EC604  BF 21 00 CC */	stmw r25, 0xcc(r1)
/* 800EF6C8 000EC608  3B C5 38 E0 */	addi r30, r5, $$21490@l
/* 800EF6CC 000EC60C  3B E3 00 00 */	addi r31, r3, 0
/* 800EF6D0 000EC610  38 7E 00 54 */	addi r3, r30, 0x54
/* 800EF6D4 000EC614  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800EF6D8 000EC618  83 A4 00 04 */	lwz r29, 4(r4)
/* 800EF6DC 000EC61C  4B F5 4F B1 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800EF6E0 000EC620  81 9D 00 00 */	lwz r12, 0(r29)
/* 800EF6E4 000EC624  38 83 00 00 */	addi r4, r3, 0
/* 800EF6E8 000EC628  38 7D 00 00 */	addi r3, r29, 0
/* 800EF6EC 000EC62C  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800EF6F0 000EC630  38 BE 00 54 */	addi r5, r30, 0x54
/* 800EF6F4 000EC634  7D 88 03 A6 */	mtlr r12
/* 800EF6F8 000EC638  4E 80 00 21 */	blrl 
/* 800EF6FC 000EC63C  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800EF700 000EC640  3B A3 00 00 */	addi r29, r3, 0
/* 800EF704 000EC644  38 7E 00 6C */	addi r3, r30, 0x6c
/* 800EF708 000EC648  83 84 00 04 */	lwz r28, 4(r4)
/* 800EF70C 000EC64C  4B F5 4F 81 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800EF710 000EC650  81 9C 00 00 */	lwz r12, 0(r28)
/* 800EF714 000EC654  38 83 00 00 */	addi r4, r3, 0
/* 800EF718 000EC658  38 7C 00 00 */	addi r3, r28, 0
/* 800EF71C 000EC65C  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800EF720 000EC660  38 BE 00 6C */	addi r5, r30, 0x6c
/* 800EF724 000EC664  7D 88 03 A6 */	mtlr r12
/* 800EF728 000EC668  4E 80 00 21 */	blrl 
/* 800EF72C 000EC66C  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800EF730 000EC670  3B 63 00 00 */	addi r27, r3, 0
/* 800EF734 000EC674  38 7E 00 80 */	addi r3, r30, 0x80
/* 800EF738 000EC678  83 84 00 04 */	lwz r28, 4(r4)
/* 800EF73C 000EC67C  4B F5 4F 51 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800EF740 000EC680  81 9C 00 00 */	lwz r12, 0(r28)
/* 800EF744 000EC684  38 83 00 00 */	addi r4, r3, 0
/* 800EF748 000EC688  38 7C 00 00 */	addi r3, r28, 0
/* 800EF74C 000EC68C  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800EF750 000EC690  38 BE 00 80 */	addi r5, r30, 0x80
/* 800EF754 000EC694  7D 88 03 A6 */	mtlr r12
/* 800EF758 000EC698  4E 80 00 21 */	blrl 
/* 800EF75C 000EC69C  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800EF760 000EC6A0  3B 43 00 00 */	addi r26, r3, 0
/* 800EF764 000EC6A4  38 7E 00 90 */	addi r3, r30, 0x90
/* 800EF768 000EC6A8  83 84 00 04 */	lwz r28, 4(r4)
/* 800EF76C 000EC6AC  4B F5 4F 21 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800EF770 000EC6B0  81 9C 00 00 */	lwz r12, 0(r28)
/* 800EF774 000EC6B4  38 83 00 00 */	addi r4, r3, 0
/* 800EF778 000EC6B8  38 7C 00 00 */	addi r3, r28, 0
/* 800EF77C 000EC6BC  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800EF780 000EC6C0  38 BE 00 90 */	addi r5, r30, 0x90
/* 800EF784 000EC6C4  7D 88 03 A6 */	mtlr r12
/* 800EF788 000EC6C8  4E 80 00 21 */	blrl 
/* 800EF78C 000EC6CC  3B 23 00 00 */	addi r25, r3, 0
/* 800EF790 000EC6D0  38 7F 00 00 */	addi r3, r31, 0
/* 800EF794 000EC6D4  38 99 00 00 */	addi r4, r25, 0
/* 800EF798 000EC6D8  38 A0 00 10 */	li r5, 0x10
/* 800EF79C 000EC6DC  48 00 8D 79 */	bl push_back__12TPerformListFPQ26JDrama8TViewObjUl
/* 800EF7A0 000EC6E0  38 7F 00 00 */	addi r3, r31, 0
/* 800EF7A4 000EC6E4  38 9D 00 00 */	addi r4, r29, 0
/* 800EF7A8 000EC6E8  38 A0 00 04 */	li r5, 4
/* 800EF7AC 000EC6EC  48 00 8D 69 */	bl push_back__12TPerformListFPQ26JDrama8TViewObjUl
/* 800EF7B0 000EC6F0  38 7F 00 00 */	addi r3, r31, 0
/* 800EF7B4 000EC6F4  38 9E 00 9C */	addi r4, r30, 0x9c
/* 800EF7B8 000EC6F8  38 A0 04 80 */	li r5, 0x480
/* 800EF7BC 000EC6FC  48 00 8E 19 */	bl push_back__12TPerformListFPCcUl
/* 800EF7C0 000EC700  38 7F 00 00 */	addi r3, r31, 0
/* 800EF7C4 000EC704  38 9E 00 AC */	addi r4, r30, 0xac
/* 800EF7C8 000EC708  38 A0 04 80 */	li r5, 0x480
/* 800EF7CC 000EC70C  48 00 8E 09 */	bl push_back__12TPerformListFPCcUl
/* 800EF7D0 000EC710  38 7F 00 00 */	addi r3, r31, 0
/* 800EF7D4 000EC714  38 9E 00 BC */	addi r4, r30, 0xbc
/* 800EF7D8 000EC718  38 A0 02 04 */	li r5, 0x204
/* 800EF7DC 000EC71C  48 00 8D F9 */	bl push_back__12TPerformListFPCcUl
/* 800EF7E0 000EC720  38 7F 00 00 */	addi r3, r31, 0
/* 800EF7E4 000EC724  38 9E 00 C8 */	addi r4, r30, 0xc8
/* 800EF7E8 000EC728  38 A0 04 80 */	li r5, 0x480
/* 800EF7EC 000EC72C  48 00 8D E9 */	bl push_back__12TPerformListFPCcUl
/* 800EF7F0 000EC730  38 7F 00 00 */	addi r3, r31, 0
/* 800EF7F4 000EC734  38 9E 00 D8 */	addi r4, r30, 0xd8
/* 800EF7F8 000EC738  38 A0 04 80 */	li r5, 0x480
/* 800EF7FC 000EC73C  48 00 8D D9 */	bl push_back__12TPerformListFPCcUl
/* 800EF800 000EC740  38 7F 00 00 */	addi r3, r31, 0
/* 800EF804 000EC744  38 9A 00 00 */	addi r4, r26, 0
/* 800EF808 000EC748  38 A0 02 04 */	li r5, 0x204
/* 800EF80C 000EC74C  48 00 8D 09 */	bl push_back__12TPerformListFPQ26JDrama8TViewObjUl
/* 800EF810 000EC750  38 7F 00 00 */	addi r3, r31, 0
/* 800EF814 000EC754  38 9B 00 00 */	addi r4, r27, 0
/* 800EF818 000EC758  38 A0 02 04 */	li r5, 0x204
/* 800EF81C 000EC75C  48 00 8C F9 */	bl push_back__12TPerformListFPQ26JDrama8TViewObjUl
/* 800EF820 000EC760  38 7F 00 00 */	addi r3, r31, 0
/* 800EF824 000EC764  38 9E 00 E8 */	addi r4, r30, 0xe8
/* 800EF828 000EC768  38 A0 04 80 */	li r5, 0x480
/* 800EF82C 000EC76C  48 00 8D A9 */	bl push_back__12TPerformListFPCcUl
/* 800EF830 000EC770  38 7F 00 00 */	addi r3, r31, 0
/* 800EF834 000EC774  38 9E 01 08 */	addi r4, r30, 0x108
/* 800EF838 000EC778  38 A0 04 80 */	li r5, 0x480
/* 800EF83C 000EC77C  48 00 8D 99 */	bl push_back__12TPerformListFPCcUl
/* 800EF840 000EC780  3C 80 04 00 */	lis r4, 0x04000200@ha
/* 800EF844 000EC784  38 7F 00 00 */	addi r3, r31, 0
/* 800EF848 000EC788  38 A4 02 00 */	addi r5, r4, 0x04000200@l
/* 800EF84C 000EC78C  38 82 90 E8 */	addi r4, r2, $$21626@sda21
/* 800EF850 000EC790  48 00 8D 85 */	bl push_back__12TPerformListFPCcUl
/* 800EF854 000EC794  38 7F 00 00 */	addi r3, r31, 0
/* 800EF858 000EC798  38 9E 01 28 */	addi r4, r30, 0x128
/* 800EF85C 000EC79C  38 A0 04 80 */	li r5, 0x480
/* 800EF860 000EC7A0  48 00 8D 75 */	bl push_back__12TPerformListFPCcUl
/* 800EF864 000EC7A4  38 7F 00 00 */	addi r3, r31, 0
/* 800EF868 000EC7A8  38 9E 01 48 */	addi r4, r30, 0x148
/* 800EF86C 000EC7AC  38 A0 04 80 */	li r5, 0x480
/* 800EF870 000EC7B0  48 00 8D 65 */	bl push_back__12TPerformListFPCcUl
/* 800EF874 000EC7B4  3C 80 02 00 */	lis r4, 0x02000200@ha
/* 800EF878 000EC7B8  38 7F 00 00 */	addi r3, r31, 0
/* 800EF87C 000EC7BC  38 A4 02 00 */	addi r5, r4, 0x02000200@l
/* 800EF880 000EC7C0  38 82 90 E8 */	addi r4, r2, $$21626@sda21
/* 800EF884 000EC7C4  48 00 8D 51 */	bl push_back__12TPerformListFPCcUl
/* 800EF888 000EC7C8  38 7F 00 00 */	addi r3, r31, 0
/* 800EF88C 000EC7CC  38 9E 01 68 */	addi r4, r30, 0x168
/* 800EF890 000EC7D0  38 A0 04 80 */	li r5, 0x480
/* 800EF894 000EC7D4  48 00 8D 41 */	bl push_back__12TPerformListFPCcUl
/* 800EF898 000EC7D8  38 7F 00 00 */	addi r3, r31, 0
/* 800EF89C 000EC7DC  38 9E 01 7C */	addi r4, r30, 0x17c
/* 800EF8A0 000EC7E0  38 A0 02 04 */	li r5, 0x204
/* 800EF8A4 000EC7E4  48 00 8D 31 */	bl push_back__12TPerformListFPCcUl
/* 800EF8A8 000EC7E8  38 7F 00 00 */	addi r3, r31, 0
/* 800EF8AC 000EC7EC  38 9E 01 8C */	addi r4, r30, 0x18c
/* 800EF8B0 000EC7F0  38 A0 04 00 */	li r5, 0x400
/* 800EF8B4 000EC7F4  48 00 8D 21 */	bl push_back__12TPerformListFPCcUl
/* 800EF8B8 000EC7F8  38 7F 00 00 */	addi r3, r31, 0
/* 800EF8BC 000EC7FC  38 9E 01 A0 */	addi r4, r30, 0x1a0
/* 800EF8C0 000EC800  38 A0 04 80 */	li r5, 0x480
/* 800EF8C4 000EC804  48 00 8D 11 */	bl push_back__12TPerformListFPCcUl
/* 800EF8C8 000EC808  38 7F 00 00 */	addi r3, r31, 0
/* 800EF8CC 000EC80C  38 9E 01 B0 */	addi r4, r30, 0x1b0
/* 800EF8D0 000EC810  38 A0 04 80 */	li r5, 0x480
/* 800EF8D4 000EC814  48 00 8D 01 */	bl push_back__12TPerformListFPCcUl
/* 800EF8D8 000EC818  38 7F 00 00 */	addi r3, r31, 0
/* 800EF8DC 000EC81C  38 9E 01 C0 */	addi r4, r30, 0x1c0
/* 800EF8E0 000EC820  38 A0 02 04 */	li r5, 0x204
/* 800EF8E4 000EC824  48 00 8C F1 */	bl push_back__12TPerformListFPCcUl
/* 800EF8E8 000EC828  38 7F 00 00 */	addi r3, r31, 0
/* 800EF8EC 000EC82C  38 9E 01 D8 */	addi r4, r30, 0x1d8
/* 800EF8F0 000EC830  38 A0 02 04 */	li r5, 0x204
/* 800EF8F4 000EC834  48 00 8C E1 */	bl push_back__12TPerformListFPCcUl
/* 800EF8F8 000EC838  38 60 00 24 */	li r3, 0x24
/* 800EF8FC 000EC83C  4B F1 CF B5 */	bl __nw__FUl
/* 800EF900 000EC840  7C 7A 1B 79 */	or. r26, r3, r3
/* 800EF904 000EC844  41 82 00 1C */	beq lbl_800EF920
/* 800EF908 000EC848  38 61 00 B8 */	addi r3, r1, 0xb8
/* 800EF90C 000EC84C  48 00 95 15 */	bl SMSGetRederRect_Game__Fv
/* 800EF910 000EC850  38 7A 00 00 */	addi r3, r26, 0
/* 800EF914 000EC854  38 81 00 B8 */	addi r4, r1, 0xb8
/* 800EF918 000EC858  38 BE 01 E8 */	addi r5, r30, 0x1e8
/* 800EF91C 000EC85C  4B F5 76 B9 */	bl __ct__Q26JDrama9TViewportFRCQ26JDrama5TRectPCc
lbl_800EF920:
/* 800EF920 000EC860  38 7F 00 00 */	addi r3, r31, 0
/* 800EF924 000EC864  38 9A 00 00 */	addi r4, r26, 0
/* 800EF928 000EC868  38 A0 00 08 */	li r5, 8
/* 800EF92C 000EC86C  48 00 8B E9 */	bl push_back__12TPerformListFPQ26JDrama8TViewObjUl
/* 800EF930 000EC870  38 7F 00 00 */	addi r3, r31, 0
/* 800EF934 000EC874  38 99 00 00 */	addi r4, r25, 0
/* 800EF938 000EC878  38 A0 00 10 */	li r5, 0x10
/* 800EF93C 000EC87C  48 00 8B D9 */	bl push_back__12TPerformListFPQ26JDrama8TViewObjUl
/* 800EF940 000EC880  38 7F 00 00 */	addi r3, r31, 0
/* 800EF944 000EC884  38 9E 01 F8 */	addi r4, r30, 0x1f8
/* 800EF948 000EC888  38 A0 00 04 */	li r5, 4
/* 800EF94C 000EC88C  48 00 8C 89 */	bl push_back__12TPerformListFPCcUl
/* 800EF950 000EC890  38 7F 00 00 */	addi r3, r31, 0
/* 800EF954 000EC894  38 9E 02 08 */	addi r4, r30, 0x208
/* 800EF958 000EC898  38 A0 00 04 */	li r5, 4
/* 800EF95C 000EC89C  48 00 8C 79 */	bl push_back__12TPerformListFPCcUl
/* 800EF960 000EC8A0  38 7F 00 00 */	addi r3, r31, 0
/* 800EF964 000EC8A4  38 9E 02 1C */	addi r4, r30, 0x21c
/* 800EF968 000EC8A8  38 A0 00 04 */	li r5, 4
/* 800EF96C 000EC8AC  48 00 8C 69 */	bl push_back__12TPerformListFPCcUl
/* 800EF970 000EC8B0  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 800EF974 000EC8B4  38 7E 02 34 */	addi r3, r30, 0x234
/* 800EF978 000EC8B8  83 84 00 04 */	lwz r28, 4(r4)
/* 800EF97C 000EC8BC  4B F5 4D 11 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 800EF980 000EC8C0  81 9C 00 00 */	lwz r12, 0(r28)
/* 800EF984 000EC8C4  38 83 00 00 */	addi r4, r3, 0
/* 800EF988 000EC8C8  38 7C 00 00 */	addi r3, r28, 0
/* 800EF98C 000EC8CC  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 800EF990 000EC8D0  38 BE 02 34 */	addi r5, r30, 0x234
/* 800EF994 000EC8D4  7D 88 03 A6 */	mtlr r12
/* 800EF998 000EC8D8  4E 80 00 21 */	blrl 
/* 800EF99C 000EC8DC  28 03 00 00 */	cmplwi r3, 0
/* 800EF9A0 000EC8E0  3B 43 00 00 */	addi r26, r3, 0
/* 800EF9A4 000EC8E4  41 82 00 28 */	beq lbl_800EF9CC
/* 800EF9A8 000EC8E8  38 7F 00 00 */	addi r3, r31, 0
/* 800EF9AC 000EC8EC  38 9E 02 4C */	addi r4, r30, 0x24c
/* 800EF9B0 000EC8F0  38 A0 04 80 */	li r5, 0x480
/* 800EF9B4 000EC8F4  48 00 8C 21 */	bl push_back__12TPerformListFPCcUl
/* 800EF9B8 000EC8F8  3C A0 40 00 */	lis r5, 0x40000204@ha
/* 800EF9BC 000EC8FC  38 7F 00 00 */	addi r3, r31, 0
/* 800EF9C0 000EC900  38 9A 00 00 */	addi r4, r26, 0
/* 800EF9C4 000EC904  38 A5 02 04 */	addi r5, r5, 0x40000204@l
/* 800EF9C8 000EC908  48 00 8B 4D */	bl push_back__12TPerformListFPQ26JDrama8TViewObjUl
lbl_800EF9CC:
/* 800EF9CC 000EC90C  38 7F 00 00 */	addi r3, r31, 0
/* 800EF9D0 000EC910  38 99 00 00 */	addi r4, r25, 0
/* 800EF9D4 000EC914  38 A0 00 10 */	li r5, 0x10
/* 800EF9D8 000EC918  48 00 8B 3D */	bl push_back__12TPerformListFPQ26JDrama8TViewObjUl
/* 800EF9DC 000EC91C  38 7F 00 00 */	addi r3, r31, 0
/* 800EF9E0 000EC920  38 9D 00 00 */	addi r4, r29, 0
/* 800EF9E4 000EC924  38 A0 00 04 */	li r5, 4
/* 800EF9E8 000EC928  48 00 8B 2D */	bl push_back__12TPerformListFPQ26JDrama8TViewObjUl
/* 800EF9EC 000EC92C  38 7F 00 00 */	addi r3, r31, 0
/* 800EF9F0 000EC930  38 9E 02 60 */	addi r4, r30, 0x260
/* 800EF9F4 000EC934  3C A0 10 00 */	lis r5, 0x1000
/* 800EF9F8 000EC938  48 00 8B DD */	bl push_back__12TPerformListFPCcUl
/* 800EF9FC 000EC93C  38 7F 00 00 */	addi r3, r31, 0
/* 800EFA00 000EC940  38 9E 02 60 */	addi r4, r30, 0x260
/* 800EFA04 000EC944  38 A0 02 04 */	li r5, 0x204
/* 800EFA08 000EC948  48 00 8B CD */	bl push_back__12TPerformListFPCcUl
/* 800EFA0C 000EC94C  38 7F 00 00 */	addi r3, r31, 0
/* 800EFA10 000EC950  38 9E 02 60 */	addi r4, r30, 0x260
/* 800EFA14 000EC954  3C A0 08 00 */	lis r5, 0x800
/* 800EFA18 000EC958  48 00 8B BD */	bl push_back__12TPerformListFPCcUl
/* 800EFA1C 000EC95C  BB 21 00 CC */	lmw r25, 0xcc(r1)
/* 800EFA20 000EC960  80 01 00 EC */	lwz r0, 0xec(r1)
/* 800EFA24 000EC964  38 21 00 E8 */	addi r1, r1, 0xe8
/* 800EFA28 000EC968  7C 08 03 A6 */	mtlr r0
/* 800EFA2C 000EC96C  4E 80 00 20 */	blr 

.section .rodata, "wa"  # 0x8036FFA0 - 0x803A8380
$$21490:
  .4byte 0
  .4byte 0
  .4byte 0
$$21526:
	.incbin "baserom.dol", 0x3708EC, 0x14
cDirtyFileName:
	.incbin "baserom.dol", 0x370900, 0x24
cDirtyTexName:
	.incbin "baserom.dol", 0x370924, 0x10
$$21615:
	.incbin "baserom.dol", 0x370934, 0x18
$$21616:
	.incbin "baserom.dol", 0x37094C, 0x14
$$21617:
	.incbin "baserom.dol", 0x370960, 0x10
$$21618:
	.incbin "baserom.dol", 0x370970, 0xC
$$21619:
	.incbin "baserom.dol", 0x37097C, 0x10
$$21620:
	.incbin "baserom.dol", 0x37098C, 0x10
$$21621:
	.incbin "baserom.dol", 0x37099C, 0xC
$$21622:
	.incbin "baserom.dol", 0x3709A8, 0x10
$$21623:
	.incbin "baserom.dol", 0x3709B8, 0x10
$$21624:
	.incbin "baserom.dol", 0x3709C8, 0x20
$$21625:
	.incbin "baserom.dol", 0x3709E8, 0x20
$$21627:
	.incbin "baserom.dol", 0x370A08, 0x20
$$21628:
	.incbin "baserom.dol", 0x370A28, 0x20
$$21629:
	.incbin "baserom.dol", 0x370A48, 0x14
$$21630:
	.incbin "baserom.dol", 0x370A5C, 0x10
$$21631:
	.incbin "baserom.dol", 0x370A6C, 0x14
$$21632:
	.incbin "baserom.dol", 0x370A80, 0x10
$$21633:
	.incbin "baserom.dol", 0x370A90, 0x10
$$21634:
	.incbin "baserom.dol", 0x370AA0, 0x18
$$21635:
	.incbin "baserom.dol", 0x370AB8, 0x10
$$21636:
	.incbin "baserom.dol", 0x370AC8, 0x10
$$21637:
	.incbin "baserom.dol", 0x370AD8, 0x10
$$21638:
	.incbin "baserom.dol", 0x370AE8, 0x14
$$21639:
	.incbin "baserom.dol", 0x370AFC, 0x18
$$21640:
	.incbin "baserom.dol", 0x370B14, 0x18
$$21641:
	.incbin "baserom.dol", 0x370B2C, 0x14
$$21642:
	.incbin "baserom.dol", 0x370B40, 0x18

.section .sdata2, "wa"  # 0x8040B460 - 0x80414020
$$21626:
	.incbin "baserom.dol", 0x3E4DE8, 0x8

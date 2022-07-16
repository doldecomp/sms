.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global __dt__16TSwitchHelpActorFv
__dt__16TSwitchHelpActorFv:
/* 80218A98 002159D8  7C 08 02 A6 */	mflr r0
/* 80218A9C 002159DC  90 01 00 04 */	stw r0, 4(r1)
/* 80218AA0 002159E0  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80218AA4 002159E4  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80218AA8 002159E8  3B E4 00 00 */	addi r31, r4, 0
/* 80218AAC 002159EC  93 C1 00 10 */	stw r30, 0x10(r1)
/* 80218AB0 002159F0  7C 7E 1B 79 */	or. r30, r3, r3
/* 80218AB4 002159F4  41 82 00 64 */	beq lbl_80218B18
/* 80218AB8 002159F8  3C 60 80 3D */	lis r3, __vt__16TSwitchHelpActor@ha
/* 80218ABC 002159FC  38 63 12 C8 */	addi r3, r3, __vt__16TSwitchHelpActor@l
/* 80218AC0 00215A00  90 7E 00 00 */	stw r3, 0(r30)
/* 80218AC4 00215A04  38 03 00 24 */	addi r0, r3, 0x24
/* 80218AC8 00215A08  90 1E 00 20 */	stw r0, 0x20(r30)
/* 80218ACC 00215A0C  41 82 00 3C */	beq lbl_80218B08
/* 80218AD0 00215A10  3C 60 80 3D */	lis r3, __vt__10THelpActor@ha
/* 80218AD4 00215A14  38 63 13 70 */	addi r3, r3, __vt__10THelpActor@l
/* 80218AD8 00215A18  90 7E 00 00 */	stw r3, 0(r30)
/* 80218ADC 00215A1C  38 03 00 24 */	addi r0, r3, 0x24
/* 80218AE0 00215A20  90 1E 00 20 */	stw r0, 0x20(r30)
/* 80218AE4 00215A24  41 82 00 24 */	beq lbl_80218B08
/* 80218AE8 00215A28  3C 60 80 3B */	lis r3, __vt__9THitActor@ha
/* 80218AEC 00215A2C  38 63 53 A0 */	addi r3, r3, __vt__9THitActor@l
/* 80218AF0 00215A30  90 7E 00 00 */	stw r3, 0(r30)
/* 80218AF4 00215A34  38 03 00 24 */	addi r0, r3, 0x24
/* 80218AF8 00215A38  38 7E 00 00 */	addi r3, r30, 0
/* 80218AFC 00215A3C  90 1E 00 20 */	stw r0, 0x20(r30)
/* 80218B00 00215A40  38 80 00 00 */	li r4, 0
/* 80218B04 00215A44  4B E2 80 91 */	bl __dt__Q26JDrama6TActorFv
lbl_80218B08:
/* 80218B08 00215A48  7F E0 07 35 */	extsh. r0, r31
/* 80218B0C 00215A4C  40 81 00 0C */	ble lbl_80218B18
/* 80218B10 00215A50  7F C3 F3 78 */	mr r3, r30
/* 80218B14 00215A54  4B DF 3F 9D */	bl __dl__FPv
lbl_80218B18:
/* 80218B18 00215A58  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80218B1C 00215A5C  7F C3 F3 78 */	mr r3, r30
/* 80218B20 00215A60  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80218B24 00215A64  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 80218B28 00215A68  7C 08 03 A6 */	mtlr r0
/* 80218B2C 00215A6C  38 21 00 18 */	addi r1, r1, 0x18
/* 80218B30 00215A70  4E 80 00 20 */	blr 

.global check__16TSwitchHelpActorFv
check__16TSwitchHelpActorFv:
/* 80218B34 00215A74  80 63 00 70 */	lwz r3, 0x70(r3)
/* 80218B38 00215A78  A0 03 00 FC */	lhz r0, 0xfc(r3)
/* 80218B3C 00215A7C  28 00 00 03 */	cmplwi r0, 3
/* 80218B40 00215A80  40 82 00 0C */	bne lbl_80218B4C
/* 80218B44 00215A84  38 00 00 01 */	li r0, 1
/* 80218B48 00215A88  48 00 00 08 */	b lbl_80218B50
lbl_80218B4C:
/* 80218B4C 00215A8C  38 00 00 00 */	li r0, 0
lbl_80218B50:
/* 80218B50 00215A90  54 00 06 3F */	clrlwi. r0, r0, 0x18
/* 80218B54 00215A94  41 82 00 0C */	beq lbl_80218B60
/* 80218B58 00215A98  38 60 00 01 */	li r3, 1
/* 80218B5C 00215A9C  4E 80 00 20 */	blr 
lbl_80218B60:
/* 80218B60 00215AA0  38 60 00 00 */	li r3, 0
/* 80218B64 00215AA4  4E 80 00 20 */	blr 

.global __ct__16TSwitchHelpActorFPCc
__ct__16TSwitchHelpActorFPCc:
/* 80218B68 00215AA8  7C 08 02 A6 */	mflr r0
/* 80218B6C 00215AAC  90 01 00 04 */	stw r0, 4(r1)
/* 80218B70 00215AB0  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80218B74 00215AB4  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80218B78 00215AB8  7C 7F 1B 78 */	mr r31, r3
/* 80218B7C 00215ABC  4B F0 4B 51 */	bl __ct__9THitActorFPCc
/* 80218B80 00215AC0  3C 60 80 3D */	lis r3, __vt__10THelpActor@ha
/* 80218B84 00215AC4  38 63 13 70 */	addi r3, r3, __vt__10THelpActor@l
/* 80218B88 00215AC8  90 7F 00 00 */	stw r3, 0(r31)
/* 80218B8C 00215ACC  38 03 00 24 */	addi r0, r3, 0x24
/* 80218B90 00215AD0  3C 60 80 3D */	lis r3, __vt__16TSwitchHelpActor@ha
/* 80218B94 00215AD4  90 1F 00 20 */	stw r0, 0x20(r31)
/* 80218B98 00215AD8  38 00 FF FF */	li r0, -1
/* 80218B9C 00215ADC  38 83 12 C8 */	addi r4, r3, __vt__16TSwitchHelpActor@l
/* 80218BA0 00215AE0  90 1F 00 68 */	stw r0, 0x68(r31)
/* 80218BA4 00215AE4  38 A0 00 00 */	li r5, 0
/* 80218BA8 00215AE8  38 04 00 24 */	addi r0, r4, 0x24
/* 80218BAC 00215AEC  90 BF 00 6C */	stw r5, 0x6c(r31)
/* 80218BB0 00215AF0  7F E3 FB 78 */	mr r3, r31
/* 80218BB4 00215AF4  90 BF 00 70 */	stw r5, 0x70(r31)
/* 80218BB8 00215AF8  98 BF 00 74 */	stb r5, 0x74(r31)
/* 80218BBC 00215AFC  90 9F 00 00 */	stw r4, 0(r31)
/* 80218BC0 00215B00  90 1F 00 20 */	stw r0, 0x20(r31)
/* 80218BC4 00215B04  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80218BC8 00215B08  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80218BCC 00215B0C  38 21 00 18 */	addi r1, r1, 0x18
/* 80218BD0 00215B10  7C 08 03 A6 */	mtlr r0
/* 80218BD4 00215B14  4E 80 00 20 */	blr 

.global __dt__10THelpActorFv
__dt__10THelpActorFv:
/* 80218BD8 00215B18  7C 08 02 A6 */	mflr r0
/* 80218BDC 00215B1C  90 01 00 04 */	stw r0, 4(r1)
/* 80218BE0 00215B20  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80218BE4 00215B24  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80218BE8 00215B28  3B E4 00 00 */	addi r31, r4, 0
/* 80218BEC 00215B2C  93 C1 00 10 */	stw r30, 0x10(r1)
/* 80218BF0 00215B30  7C 7E 1B 79 */	or. r30, r3, r3
/* 80218BF4 00215B34  41 82 00 4C */	beq lbl_80218C40
/* 80218BF8 00215B38  3C 60 80 3D */	lis r3, __vt__10THelpActor@ha
/* 80218BFC 00215B3C  38 63 13 70 */	addi r3, r3, __vt__10THelpActor@l
/* 80218C00 00215B40  90 7E 00 00 */	stw r3, 0(r30)
/* 80218C04 00215B44  38 03 00 24 */	addi r0, r3, 0x24
/* 80218C08 00215B48  90 1E 00 20 */	stw r0, 0x20(r30)
/* 80218C0C 00215B4C  41 82 00 24 */	beq lbl_80218C30
/* 80218C10 00215B50  3C 60 80 3B */	lis r3, __vt__9THitActor@ha
/* 80218C14 00215B54  38 63 53 A0 */	addi r3, r3, __vt__9THitActor@l
/* 80218C18 00215B58  90 7E 00 00 */	stw r3, 0(r30)
/* 80218C1C 00215B5C  38 03 00 24 */	addi r0, r3, 0x24
/* 80218C20 00215B60  38 7E 00 00 */	addi r3, r30, 0
/* 80218C24 00215B64  90 1E 00 20 */	stw r0, 0x20(r30)
/* 80218C28 00215B68  38 80 00 00 */	li r4, 0
/* 80218C2C 00215B6C  4B E2 7F 69 */	bl __dt__Q26JDrama6TActorFv
lbl_80218C30:
/* 80218C30 00215B70  7F E0 07 35 */	extsh. r0, r31
/* 80218C34 00215B74  40 81 00 0C */	ble lbl_80218C40
/* 80218C38 00215B78  7F C3 F3 78 */	mr r3, r30
/* 80218C3C 00215B7C  4B DF 3E 75 */	bl __dl__FPv
lbl_80218C40:
/* 80218C40 00215B80  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80218C44 00215B84  7F C3 F3 78 */	mr r3, r30
/* 80218C48 00215B88  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80218C4C 00215B8C  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 80218C50 00215B90  7C 08 03 A6 */	mtlr r0
/* 80218C54 00215B94  38 21 00 18 */	addi r1, r1, 0x18
/* 80218C58 00215B98  4E 80 00 20 */	blr 

.global check__10THelpActorFv
check__10THelpActorFv:
/* 80218C5C 00215B9C  80 63 00 70 */	lwz r3, 0x70(r3)
/* 80218C60 00215BA0  80 03 00 F0 */	lwz r0, 0xf0(r3)
/* 80218C64 00215BA4  54 00 07 FE */	clrlwi r0, r0, 0x1f
/* 80218C68 00215BA8  7C 60 00 D0 */	neg r3, r0
/* 80218C6C 00215BAC  30 03 FF FF */	addic r0, r3, -1
/* 80218C70 00215BB0  7C 00 19 10 */	subfe r0, r0, r3
/* 80218C74 00215BB4  54 03 06 3E */	clrlwi r3, r0, 0x18
/* 80218C78 00215BB8  4E 80 00 20 */	blr 

.global perform__10THelpActorFUlPQ26JDrama9TGraphics
perform__10THelpActorFUlPQ26JDrama9TGraphics:
/* 80218C7C 00215BBC  7C 08 02 A6 */	mflr r0
/* 80218C80 00215BC0  90 01 00 04 */	stw r0, 4(r1)
/* 80218C84 00215BC4  54 80 07 FF */	clrlwi. r0, r4, 0x1f
/* 80218C88 00215BC8  94 21 FF A8 */	stwu r1, -0x58(r1)
/* 80218C8C 00215BCC  93 E1 00 54 */	stw r31, 0x54(r1)
/* 80218C90 00215BD0  3B E3 00 00 */	addi r31, r3, 0
/* 80218C94 00215BD4  41 82 01 2C */	beq lbl_80218DC0
/* 80218C98 00215BD8  88 1F 00 74 */	lbz r0, 0x74(r31)
/* 80218C9C 00215BDC  28 00 00 00 */	cmplwi r0, 0
/* 80218CA0 00215BE0  41 82 00 94 */	beq lbl_80218D34
/* 80218CA4 00215BE4  80 1F 00 70 */	lwz r0, 0x70(r31)
/* 80218CA8 00215BE8  28 00 00 00 */	cmplwi r0, 0
/* 80218CAC 00215BEC  41 82 00 28 */	beq lbl_80218CD4
/* 80218CB0 00215BF0  7F E3 FB 78 */	mr r3, r31
/* 80218CB4 00215BF4  81 9F 00 00 */	lwz r12, 0(r31)
/* 80218CB8 00215BF8  81 8C 00 A4 */	lwz r12, 0xa4(r12)
/* 80218CBC 00215BFC  7D 88 03 A6 */	mtlr r12
/* 80218CC0 00215C00  4E 80 00 21 */	blrl 
/* 80218CC4 00215C04  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 80218CC8 00215C08  41 82 00 0C */	beq lbl_80218CD4
/* 80218CCC 00215C0C  38 00 FF FF */	li r0, -1
/* 80218CD0 00215C10  48 00 00 34 */	b lbl_80218D04
lbl_80218CD4:
/* 80218CD4 00215C14  A0 1F 00 48 */	lhz r0, 0x48(r31)
/* 80218CD8 00215C18  28 00 00 00 */	cmplwi r0, 0
/* 80218CDC 00215C1C  41 82 00 24 */	beq lbl_80218D00
/* 80218CE0 00215C20  80 7F 00 44 */	lwz r3, 0x44(r31)
/* 80218CE4 00215C24  80 63 00 00 */	lwz r3, 0(r3)
/* 80218CE8 00215C28  80 63 00 4C */	lwz r3, 0x4c(r3)
/* 80218CEC 00215C2C  3C 03 80 00 */	addis r0, r3, 0x8000
/* 80218CF0 00215C30  28 00 00 01 */	cmplwi r0, 1
/* 80218CF4 00215C34  40 82 00 0C */	bne lbl_80218D00
/* 80218CF8 00215C38  80 1F 00 68 */	lwz r0, 0x68(r31)
/* 80218CFC 00215C3C  48 00 00 08 */	b lbl_80218D04
lbl_80218D00:
/* 80218D00 00215C40  38 00 FF FF */	li r0, -1
lbl_80218D04:
/* 80218D04 00215C44  2C 00 FF FF */	cmpwi r0, -1
/* 80218D08 00215C48  40 82 00 B8 */	bne lbl_80218DC0
/* 80218D0C 00215C4C  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 80218D10 00215C50  38 A0 00 00 */	li r5, 0
/* 80218D14 00215C54  80 9F 00 68 */	lwz r4, 0x68(r31)
/* 80218D18 00215C58  80 63 00 74 */	lwz r3, 0x74(r3)
/* 80218D1C 00215C5C  4B FF 2B E5 */	bl startDisappearBalloon__11TGCConsole2FUlb
/* 80218D20 00215C60  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 80218D24 00215C64  41 82 00 9C */	beq lbl_80218DC0
/* 80218D28 00215C68  38 00 00 00 */	li r0, 0
/* 80218D2C 00215C6C  98 1F 00 74 */	stb r0, 0x74(r31)
/* 80218D30 00215C70  48 00 00 90 */	b lbl_80218DC0
lbl_80218D34:
/* 80218D34 00215C74  80 1F 00 70 */	lwz r0, 0x70(r31)
/* 80218D38 00215C78  28 00 00 00 */	cmplwi r0, 0
/* 80218D3C 00215C7C  41 82 00 28 */	beq lbl_80218D64
/* 80218D40 00215C80  7F E3 FB 78 */	mr r3, r31
/* 80218D44 00215C84  81 9F 00 00 */	lwz r12, 0(r31)
/* 80218D48 00215C88  81 8C 00 A4 */	lwz r12, 0xa4(r12)
/* 80218D4C 00215C8C  7D 88 03 A6 */	mtlr r12
/* 80218D50 00215C90  4E 80 00 21 */	blrl 
/* 80218D54 00215C94  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 80218D58 00215C98  41 82 00 0C */	beq lbl_80218D64
/* 80218D5C 00215C9C  38 00 FF FF */	li r0, -1
/* 80218D60 00215CA0  48 00 00 34 */	b lbl_80218D94
lbl_80218D64:
/* 80218D64 00215CA4  A0 1F 00 48 */	lhz r0, 0x48(r31)
/* 80218D68 00215CA8  28 00 00 00 */	cmplwi r0, 0
/* 80218D6C 00215CAC  41 82 00 24 */	beq lbl_80218D90
/* 80218D70 00215CB0  80 7F 00 44 */	lwz r3, 0x44(r31)
/* 80218D74 00215CB4  80 63 00 00 */	lwz r3, 0(r3)
/* 80218D78 00215CB8  80 63 00 4C */	lwz r3, 0x4c(r3)
/* 80218D7C 00215CBC  3C 03 80 00 */	addis r0, r3, 0x8000
/* 80218D80 00215CC0  28 00 00 01 */	cmplwi r0, 1
/* 80218D84 00215CC4  40 82 00 0C */	bne lbl_80218D90
/* 80218D88 00215CC8  80 1F 00 68 */	lwz r0, 0x68(r31)
/* 80218D8C 00215CCC  48 00 00 08 */	b lbl_80218D94
lbl_80218D90:
/* 80218D90 00215CD0  38 00 FF FF */	li r0, -1
lbl_80218D94:
/* 80218D94 00215CD4  2C 00 FF FF */	cmpwi r0, -1
/* 80218D98 00215CD8  41 82 00 28 */	beq lbl_80218DC0
/* 80218D9C 00215CDC  80 6D 97 E8 */	lwz r3, gpMarDirector@sda21(r13)
/* 80218DA0 00215CE0  38 A0 00 00 */	li r5, 0
/* 80218DA4 00215CE4  80 9F 00 68 */	lwz r4, 0x68(r31)
/* 80218DA8 00215CE8  80 63 00 74 */	lwz r3, 0x74(r3)
/* 80218DAC 00215CEC  4B FF 28 99 */	bl startAppearBalloon__11TGCConsole2FUlb
/* 80218DB0 00215CF0  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 80218DB4 00215CF4  41 82 00 0C */	beq lbl_80218DC0
/* 80218DB8 00215CF8  38 00 00 01 */	li r0, 1
/* 80218DBC 00215CFC  98 1F 00 74 */	stb r0, 0x74(r31)
lbl_80218DC0:
/* 80218DC0 00215D00  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 80218DC4 00215D04  83 E1 00 54 */	lwz r31, 0x54(r1)
/* 80218DC8 00215D08  38 21 00 58 */	addi r1, r1, 0x58
/* 80218DCC 00215D0C  7C 08 03 A6 */	mtlr r0
/* 80218DD0 00215D10  4E 80 00 20 */	blr 

.global loadAfter__10THelpActorFv
loadAfter__10THelpActorFv:
/* 80218DD4 00215D14  7C 08 02 A6 */	mflr r0
/* 80218DD8 00215D18  90 01 00 04 */	stw r0, 4(r1)
/* 80218DDC 00215D1C  94 21 FF B0 */	stwu r1, -0x50(r1)
/* 80218DE0 00215D20  93 E1 00 4C */	stw r31, 0x4c(r1)
/* 80218DE4 00215D24  93 C1 00 48 */	stw r30, 0x48(r1)
/* 80218DE8 00215D28  93 A1 00 44 */	stw r29, 0x44(r1)
/* 80218DEC 00215D2C  7C 7D 1B 78 */	mr r29, r3
/* 80218DF0 00215D30  4B E2 BA BD */	bl loadAfter__Q26JDrama8TNameRefFv
/* 80218DF4 00215D34  80 6D 8E E0 */	lwz r3, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 80218DF8 00215D38  83 FD 00 6C */	lwz r31, 0x6c(r29)
/* 80218DFC 00215D3C  83 C3 00 04 */	lwz r30, 4(r3)
/* 80218E00 00215D40  7F E3 FB 78 */	mr r3, r31
/* 80218E04 00215D44  4B E2 B8 89 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 80218E08 00215D48  81 9E 00 00 */	lwz r12, 0(r30)
/* 80218E0C 00215D4C  38 83 00 00 */	addi r4, r3, 0
/* 80218E10 00215D50  38 7E 00 00 */	addi r3, r30, 0
/* 80218E14 00215D54  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 80218E18 00215D58  7F E5 FB 78 */	mr r5, r31
/* 80218E1C 00215D5C  7D 88 03 A6 */	mtlr r12
/* 80218E20 00215D60  4E 80 00 21 */	blrl 
/* 80218E24 00215D64  90 7D 00 70 */	stw r3, 0x70(r29)
/* 80218E28 00215D68  3C 60 80 39 */	lis r3, "@1991"@ha
/* 80218E2C 00215D6C  3B E3 3A F0 */	addi r31, r3, "@1991"@l
/* 80218E30 00215D70  80 8D 8E E0 */	lwz r4, instance__Q26JDrama11TNameRefGen@sda21(r13)
/* 80218E34 00215D74  7F E3 FB 78 */	mr r3, r31
/* 80218E38 00215D78  83 C4 00 04 */	lwz r30, 4(r4)
/* 80218E3C 00215D7C  4B E2 B8 51 */	bl calcKeyCode__Q26JDrama8TNameRefFPCc
/* 80218E40 00215D80  81 9E 00 00 */	lwz r12, 0(r30)
/* 80218E44 00215D84  38 83 00 00 */	addi r4, r3, 0
/* 80218E48 00215D88  38 7E 00 00 */	addi r3, r30, 0
/* 80218E4C 00215D8C  81 8C 00 1C */	lwz r12, 0x1c(r12)
/* 80218E50 00215D90  38 BF 00 00 */	addi r5, r31, 0
/* 80218E54 00215D94  7D 88 03 A6 */	mtlr r12
/* 80218E58 00215D98  4E 80 00 21 */	blrl 
/* 80218E5C 00215D9C  7F A4 EB 78 */	mr r4, r29
/* 80218E60 00215DA0  4B FF 5F 01 */	bl entryHelpActor__11TGCConsole2FP10THelpActor
/* 80218E64 00215DA4  80 01 00 54 */	lwz r0, 0x54(r1)
/* 80218E68 00215DA8  83 E1 00 4C */	lwz r31, 0x4c(r1)
/* 80218E6C 00215DAC  83 C1 00 48 */	lwz r30, 0x48(r1)
/* 80218E70 00215DB0  7C 08 03 A6 */	mtlr r0
/* 80218E74 00215DB4  83 A1 00 44 */	lwz r29, 0x44(r1)
/* 80218E78 00215DB8  38 21 00 50 */	addi r1, r1, 0x50
/* 80218E7C 00215DBC  4E 80 00 20 */	blr 

.global load__10THelpActorFR20JSUMemoryInputStream
load__10THelpActorFR20JSUMemoryInputStream:
/* 80218E80 00215DC0  7C 08 02 A6 */	mflr r0
/* 80218E84 00215DC4  90 01 00 04 */	stw r0, 4(r1)
/* 80218E88 00215DC8  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 80218E8C 00215DCC  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 80218E90 00215DD0  3B E4 00 00 */	addi r31, r4, 0
/* 80218E94 00215DD4  93 C1 00 18 */	stw r30, 0x18(r1)
/* 80218E98 00215DD8  3B C3 00 00 */	addi r30, r3, 0
/* 80218E9C 00215DDC  4B E2 7A AD */	bl load__Q26JDrama6TActorFR20JSUMemoryInputStream
/* 80218EA0 00215DE0  38 7F 00 00 */	addi r3, r31, 0
/* 80218EA4 00215DE4  38 81 00 14 */	addi r4, r1, 0x14
/* 80218EA8 00215DE8  38 A0 00 04 */	li r5, 4
/* 80218EAC 00215DEC  4B DF 56 E5 */	bl read__14JSUInputStreamFPvl
/* 80218EB0 00215DF0  38 7F 00 00 */	addi r3, r31, 0
/* 80218EB4 00215DF4  38 81 00 10 */	addi r4, r1, 0x10
/* 80218EB8 00215DF8  38 A0 00 04 */	li r5, 4
/* 80218EBC 00215DFC  4B DF 56 D5 */	bl read__14JSUInputStreamFPvl
/* 80218EC0 00215E00  7F E3 FB 78 */	mr r3, r31
/* 80218EC4 00215E04  4B DF 57 25 */	bl readString__14JSUInputStreamFv
/* 80218EC8 00215E08  90 7E 00 6C */	stw r3, 0x6c(r30)
/* 80218ECC 00215E0C  3C 80 40 00 */	lis r4, 0x40000320@ha
/* 80218ED0 00215E10  38 7E 00 00 */	addi r3, r30, 0
/* 80218ED4 00215E14  C0 62 D6 3C */	lfs f3, "@1996"@sda21(r2)
/* 80218ED8 00215E18  38 84 03 20 */	addi r4, r4, 0x40000320@l
/* 80218EDC 00215E1C  C0 42 D6 38 */	lfs f2, "@1995"@sda21(r2)
/* 80218EE0 00215E20  38 A0 00 01 */	li r5, 1
/* 80218EE4 00215E24  C0 3E 00 24 */	lfs f1, 0x24(r30)
/* 80218EE8 00215E28  C0 1E 00 28 */	lfs f0, 0x28(r30)
/* 80218EEC 00215E2C  FC 80 18 90 */	fmr f4, f3
/* 80218EF0 00215E30  EC 22 00 72 */	fmuls f1, f2, f1
/* 80218EF4 00215E34  3C C0 80 00 */	lis r6, 0x8000
/* 80218EF8 00215E38  EC 42 00 32 */	fmuls f2, f2, f0
/* 80218EFC 00215E3C  4B F0 48 FD */	bl initHitActor__9THitActorFUlUsiffff
/* 80218F00 00215E40  80 61 00 10 */	lwz r3, 0x10(r1)
/* 80218F04 00215E44  3C 63 00 0E */	addis r3, r3, 0xe
/* 80218F08 00215E48  38 03 00 30 */	addi r0, r3, 0x30
/* 80218F0C 00215E4C  90 1E 00 68 */	stw r0, 0x68(r30)
/* 80218F10 00215E50  80 01 00 24 */	lwz r0, 0x24(r1)
/* 80218F14 00215E54  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 80218F18 00215E58  83 C1 00 18 */	lwz r30, 0x18(r1)
/* 80218F1C 00215E5C  38 21 00 20 */	addi r1, r1, 0x20
/* 80218F20 00215E60  7C 08 03 A6 */	mtlr r0
/* 80218F24 00215E64  4E 80 00 20 */	blr 

.global __ct__10THelpActorFPCc
__ct__10THelpActorFPCc:
/* 80218F28 00215E68  7C 08 02 A6 */	mflr r0
/* 80218F2C 00215E6C  90 01 00 04 */	stw r0, 4(r1)
/* 80218F30 00215E70  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80218F34 00215E74  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80218F38 00215E78  7C 7F 1B 78 */	mr r31, r3
/* 80218F3C 00215E7C  4B F0 47 91 */	bl __ct__9THitActorFPCc
/* 80218F40 00215E80  3C 60 80 3D */	lis r3, __vt__10THelpActor@ha
/* 80218F44 00215E84  38 63 13 70 */	addi r3, r3, __vt__10THelpActor@l
/* 80218F48 00215E88  90 7F 00 00 */	stw r3, 0(r31)
/* 80218F4C 00215E8C  38 03 00 24 */	addi r0, r3, 0x24
/* 80218F50 00215E90  38 80 FF FF */	li r4, -1
/* 80218F54 00215E94  90 1F 00 20 */	stw r0, 0x20(r31)
/* 80218F58 00215E98  38 00 00 00 */	li r0, 0
/* 80218F5C 00215E9C  38 7F 00 00 */	addi r3, r31, 0
/* 80218F60 00215EA0  90 9F 00 68 */	stw r4, 0x68(r31)
/* 80218F64 00215EA4  90 1F 00 6C */	stw r0, 0x6c(r31)
/* 80218F68 00215EA8  90 1F 00 70 */	stw r0, 0x70(r31)
/* 80218F6C 00215EAC  98 1F 00 74 */	stb r0, 0x74(r31)
/* 80218F70 00215EB0  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 80218F74 00215EB4  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 80218F78 00215EB8  38 21 00 18 */	addi r1, r1, 0x18
/* 80218F7C 00215EBC  7C 08 03 A6 */	mtlr r0
/* 80218F80 00215EC0  4E 80 00 20 */	blr 

.global "@32@__dt__10THelpActorFv"
"@32@__dt__10THelpActorFv":
/* 80218F84 00215EC4  38 63 FF E0 */	addi r3, r3, -32
/* 80218F88 00215EC8  4B FF FC 50 */	b __dt__10THelpActorFv

.global "@32@__dt__16TSwitchHelpActorFv"
"@32@__dt__16TSwitchHelpActorFv":
/* 80218F8C 00215ECC  38 63 FF E0 */	addi r3, r3, -32
/* 80218F90 00215ED0  4B FF FB 08 */	b __dt__16TSwitchHelpActorFv

.section .rodata, "a"  # 0x8036FFA0 - 0x803A8380
.balign 8
"@1991":
	.4byte 0x47438352
	.4byte 0x8393835C
	.4byte 0x815B838B
	.4byte 0

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__16TSwitchHelpActor
__vt__16TSwitchHelpActor:
  .4byte 0
  .4byte 0
  .4byte __dt__16TSwitchHelpActorFv
  .4byte getType__Q26JDrama6TActorCFv
  .4byte load__10THelpActorFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__10THelpActorFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc
  .4byte perform__10THelpActorFUlPQ26JDrama9TGraphics
  .4byte 0
  .4byte 0
  .4byte "@32@__dt__16TSwitchHelpActorFv"
  .4byte JSGFGetType__Q26JStage6TActorCFv
  .4byte JSGGetName__Q26JStage7TObjectCFv
  .4byte JSGGetFlag__Q26JStage7TObjectCFv
  .4byte JSGSetFlag__Q26JStage7TObjectFUl
  .4byte JSGGetData__Q26JStage7TObjectCFUlPvUl
  .4byte JSGSetData__Q26JStage7TObjectFUlPCvUl
  .4byte JSGGetParent__Q26JStage7TObjectCFPPQ26JStage7TObjectPUl
  .4byte JSGSetParent__Q26JStage7TObjectFPQ26JStage7TObjectUl
  .4byte JSGSetRelation__Q26JStage7TObjectFbPQ26JStage7TObjectUl
  .4byte "@32@JSGGetTranslation__Q26JDrama6TActorCFP3Vec"
  .4byte "@32@JSGSetTranslation__Q26JDrama6TActorFRC3Vec"
  .4byte "@32@JSGGetScaling__Q26JDrama6TActorCFP3Vec"
  .4byte "@32@JSGSetScaling__Q26JDrama6TActorFRC3Vec"
  .4byte "@32@JSGGetRotation__Q26JDrama6TActorCFP3Vec"
  .4byte "@32@JSGSetRotation__Q26JDrama6TActorFRC3Vec"
  .4byte JSGGetShape__Q26JStage6TActorCFv
  .4byte JSGSetShape__Q26JStage6TActorFUl
  .4byte JSGGetAnimation__Q26JStage6TActorCFv
  .4byte JSGSetAnimation__Q26JStage6TActorFUl
  .4byte JSGGetAnimationFrame__Q26JStage6TActorCFv
  .4byte JSGSetAnimationFrame__Q26JStage6TActorFf
  .4byte JSGGetAnimationFrameMax__Q26JStage6TActorCFv
  .4byte JSGGetTranslation__Q26JDrama6TActorCFP3Vec
  .4byte JSGSetTranslation__Q26JDrama6TActorFRC3Vec
  .4byte JSGGetScaling__Q26JDrama6TActorCFP3Vec
  .4byte JSGSetScaling__Q26JDrama6TActorFRC3Vec
  .4byte JSGGetRotation__Q26JDrama6TActorCFP3Vec
  .4byte JSGSetRotation__Q26JDrama6TActorFRC3Vec
  .4byte receiveMessage__9THitActorFP9THitActorUl
  .4byte check__16TSwitchHelpActorFv
.global __vt__10THelpActor
__vt__10THelpActor:
  .4byte 0
  .4byte 0
  .4byte __dt__10THelpActorFv
  .4byte getType__Q26JDrama6TActorCFv
  .4byte load__10THelpActorFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__10THelpActorFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc
  .4byte perform__10THelpActorFUlPQ26JDrama9TGraphics
  .4byte 0
  .4byte 0
  .4byte "@32@__dt__10THelpActorFv"
  .4byte JSGFGetType__Q26JStage6TActorCFv
  .4byte JSGGetName__Q26JStage7TObjectCFv
  .4byte JSGGetFlag__Q26JStage7TObjectCFv
  .4byte JSGSetFlag__Q26JStage7TObjectFUl
  .4byte JSGGetData__Q26JStage7TObjectCFUlPvUl
  .4byte JSGSetData__Q26JStage7TObjectFUlPCvUl
  .4byte JSGGetParent__Q26JStage7TObjectCFPPQ26JStage7TObjectPUl
  .4byte JSGSetParent__Q26JStage7TObjectFPQ26JStage7TObjectUl
  .4byte JSGSetRelation__Q26JStage7TObjectFbPQ26JStage7TObjectUl
  .4byte "@32@JSGGetTranslation__Q26JDrama6TActorCFP3Vec"
  .4byte "@32@JSGSetTranslation__Q26JDrama6TActorFRC3Vec"
  .4byte "@32@JSGGetScaling__Q26JDrama6TActorCFP3Vec"
  .4byte "@32@JSGSetScaling__Q26JDrama6TActorFRC3Vec"
  .4byte "@32@JSGGetRotation__Q26JDrama6TActorCFP3Vec"
  .4byte "@32@JSGSetRotation__Q26JDrama6TActorFRC3Vec"
  .4byte JSGGetShape__Q26JStage6TActorCFv
  .4byte JSGSetShape__Q26JStage6TActorFUl
  .4byte JSGGetAnimation__Q26JStage6TActorCFv
  .4byte JSGSetAnimation__Q26JStage6TActorFUl
  .4byte JSGGetAnimationFrame__Q26JStage6TActorCFv
  .4byte JSGSetAnimationFrame__Q26JStage6TActorFf
  .4byte JSGGetAnimationFrameMax__Q26JStage6TActorCFv
  .4byte JSGGetTranslation__Q26JDrama6TActorCFP3Vec
  .4byte JSGSetTranslation__Q26JDrama6TActorFRC3Vec
  .4byte JSGGetScaling__Q26JDrama6TActorCFP3Vec
  .4byte JSGSetScaling__Q26JDrama6TActorFRC3Vec
  .4byte JSGGetRotation__Q26JDrama6TActorCFP3Vec
  .4byte JSGSetRotation__Q26JDrama6TActorFRC3Vec
  .4byte receiveMessage__9THitActorFP9THitActorUl
  .4byte check__10THelpActorFv

.section .sdata2, "a"  # 0x8040B460 - 0x80414020
.balign 8
"@1995":
	.4byte 0x42C80000
"@1996":
	.4byte 0x3F800000

.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global init__Q38JASystem6Driver8DSPQueueFUl
init__Q38JASystem6Driver8DSPQueueFUl:
/* 8005BDB8 00058CF8  7C 08 02 A6 */	mflr r0
/* 8005BDBC 00058CFC  38 A0 00 00 */	li r5, 0
/* 8005BDC0 00058D00  90 01 00 04 */	stw r0, 4(r1)
/* 8005BDC4 00058D04  94 21 FF F8 */	stwu r1, -8(r1)
/* 8005BDC8 00058D08  90 6D 81 98 */	stw r3, waitMax__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BDCC 00058D0C  80 8D 92 00 */	lwz r4, JASDram@sda21(r13)
/* 8005BDD0 00058D10  80 0D 81 98 */	lwz r0, waitMax__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BDD4 00058D14  54 03 10 3A */	slwi r3, r0, 2
/* 8005BDD8 00058D18  4B FB 0C 6D */	bl __nwa__FUlP7JKRHeapi
/* 8005BDDC 00058D1C  80 0D 81 98 */	lwz r0, waitMax__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BDE0 00058D20  38 A0 00 00 */	li r5, 0
/* 8005BDE4 00058D24  90 6D 90 E0 */	stw r3, waitp__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BDE8 00058D28  80 8D 92 00 */	lwz r4, JASDram@sda21(r13)
/* 8005BDEC 00058D2C  54 03 10 3A */	slwi r3, r0, 2
/* 8005BDF0 00058D30  4B FB 0C 55 */	bl __nwa__FUlP7JKRHeapi
/* 8005BDF4 00058D34  80 0D 81 98 */	lwz r0, waitMax__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BDF8 00058D38  90 6D 90 E4 */	stw r3, waittime__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BDFC 00058D3C  80 6D 90 E0 */	lwz r3, waitp__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BE00 00058D40  54 04 10 3A */	slwi r4, r0, 2
/* 8005BE04 00058D44  48 00 54 1D */	bl bzero__Q28JASystem4CalcFPvUl
/* 8005BE08 00058D48  80 0D 81 98 */	lwz r0, waitMax__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BE0C 00058D4C  80 6D 90 E4 */	lwz r3, waittime__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BE10 00058D50  54 04 10 3A */	slwi r4, r0, 2
/* 8005BE14 00058D54  48 00 54 0D */	bl bzero__Q28JASystem4CalcFPvUl
/* 8005BE18 00058D58  80 01 00 0C */	lwz r0, 0xc(r1)
/* 8005BE1C 00058D5C  38 21 00 08 */	addi r1, r1, 8
/* 8005BE20 00058D60  7C 08 03 A6 */	mtlr r0
/* 8005BE24 00058D64  4E 80 00 20 */	blr 

.global enQueue__Q38JASystem6Driver8DSPQueueFPQ28JASystem8TChannel
enQueue__Q38JASystem6Driver8DSPQueueFPQ28JASystem8TChannel:
/* 8005BE28 00058D68  7C 08 02 A6 */	mflr r0
/* 8005BE2C 00058D6C  90 01 00 04 */	stw r0, 4(r1)
/* 8005BE30 00058D70  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8005BE34 00058D74  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8005BE38 00058D78  7C 7F 1B 78 */	mr r31, r3
/* 8005BE3C 00058D7C  80 8D 90 E8 */	lwz r4, cur_waits__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BE40 00058D80  80 0D 81 98 */	lwz r0, waitMax__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BE44 00058D84  7C 04 00 40 */	cmplw r4, r0
/* 8005BE48 00058D88  40 82 00 0C */	bne lbl_8005BE54
/* 8005BE4C 00058D8C  38 60 00 00 */	li r3, 0
/* 8005BE50 00058D90  48 00 00 74 */	b lbl_8005BEC4
lbl_8005BE54:
/* 8005BE54 00058D94  80 1F 00 C0 */	lwz r0, 0xc0(r31)
/* 8005BE58 00058D98  54 03 06 3E */	clrlwi r3, r0, 0x18
/* 8005BE5C 00058D9C  48 00 35 F1 */	bl breakLower__Q28JASystem11TDSPChannelFUc
/* 8005BE60 00058DA0  2C 03 00 00 */	cmpwi r3, 0
/* 8005BE64 00058DA4  40 82 00 0C */	bne lbl_8005BE70
/* 8005BE68 00058DA8  38 60 00 00 */	li r3, 0
/* 8005BE6C 00058DAC  48 00 00 58 */	b lbl_8005BEC4
lbl_8005BE70:
/* 8005BE70 00058DB0  80 0D 90 F0 */	lwz r0, cur_tail__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BE74 00058DB4  38 A0 00 00 */	li r5, 0
/* 8005BE78 00058DB8  80 6D 90 E4 */	lwz r3, waittime__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BE7C 00058DBC  54 00 10 3A */	slwi r0, r0, 2
/* 8005BE80 00058DC0  7C A3 01 2E */	stwx r5, r3, r0
/* 8005BE84 00058DC4  80 0D 90 F0 */	lwz r0, cur_tail__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BE88 00058DC8  80 6D 90 E0 */	lwz r3, waitp__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BE8C 00058DCC  54 00 10 3A */	slwi r0, r0, 2
/* 8005BE90 00058DD0  7F E3 01 2E */	stwx r31, r3, r0
/* 8005BE94 00058DD4  80 8D 90 F0 */	lwz r4, cur_tail__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BE98 00058DD8  80 6D 90 E8 */	lwz r3, cur_waits__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BE9C 00058DDC  38 84 00 01 */	addi r4, r4, 1
/* 8005BEA0 00058DE0  80 0D 81 98 */	lwz r0, waitMax__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BEA4 00058DE4  90 8D 90 F0 */	stw r4, cur_tail__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BEA8 00058DE8  38 83 00 01 */	addi r4, r3, 1
/* 8005BEAC 00058DEC  80 6D 90 F0 */	lwz r3, cur_tail__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BEB0 00058DF0  90 8D 90 E8 */	stw r4, cur_waits__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BEB4 00058DF4  7C 03 00 40 */	cmplw r3, r0
/* 8005BEB8 00058DF8  40 82 00 08 */	bne lbl_8005BEC0
/* 8005BEBC 00058DFC  90 AD 90 F0 */	stw r5, cur_tail__Q38JASystem6Driver8DSPQueue@sda21(r13)
lbl_8005BEC0:
/* 8005BEC0 00058E00  38 60 00 01 */	li r3, 1
lbl_8005BEC4:
/* 8005BEC4 00058E04  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8005BEC8 00058E08  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8005BECC 00058E0C  38 21 00 18 */	addi r1, r1, 0x18
/* 8005BED0 00058E10  7C 08 03 A6 */	mtlr r0
/* 8005BED4 00058E14  4E 80 00 20 */	blr 

.global deQueue__Q38JASystem6Driver8DSPQueueFUc
deQueue__Q38JASystem6Driver8DSPQueueFUc:
/* 8005BED8 00058E18  7C 08 02 A6 */	mflr r0
/* 8005BEDC 00058E1C  90 01 00 04 */	stw r0, 4(r1)
/* 8005BEE0 00058E20  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 8005BEE4 00058E24  93 E1 00 14 */	stw r31, 0x14(r1)
/* 8005BEE8 00058E28  54 7F 06 3E */	clrlwi r31, r3, 0x18
/* 8005BEEC 00058E2C  93 C1 00 10 */	stw r30, 0x10(r1)
/* 8005BEF0 00058E30  48 00 00 CC */	b lbl_8005BFBC
lbl_8005BEF4:
/* 8005BEF4 00058E34  80 6D 90 EC */	lwz r3, cur_top__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BEF8 00058E38  80 8D 90 E0 */	lwz r4, waitp__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BEFC 00058E3C  54 60 10 3A */	slwi r0, r3, 2
/* 8005BF00 00058E40  7C 04 00 2E */	lwzx r0, r4, r0
/* 8005BF04 00058E44  28 00 00 00 */	cmplwi r0, 0
/* 8005BF08 00058E48  7C 1E 03 78 */	mr r30, r0
/* 8005BF0C 00058E4C  41 82 00 84 */	beq lbl_8005BF90
/* 8005BF10 00058E50  38 60 00 00 */	li r3, 0
/* 8005BF14 00058E54  38 9E 00 00 */	addi r4, r30, 0
/* 8005BF18 00058E58  48 00 32 6D */	bl alloc__Q28JASystem11TDSPChannelFUlUl
/* 8005BF1C 00058E5C  28 03 00 00 */	cmplwi r3, 0
/* 8005BF20 00058E60  41 82 00 A8 */	beq lbl_8005BFC8
/* 8005BF24 00058E64  90 7E 00 20 */	stw r3, 0x20(r30)
/* 8005BF28 00058E68  7F C3 F3 78 */	mr r3, r30
/* 8005BF2C 00058E6C  48 00 17 C1 */	bl playLogicalChannel__Q28JASystem8TChannelFv
/* 8005BF30 00058E70  80 7E 00 04 */	lwz r3, 4(r30)
/* 8005BF34 00058E74  7F C4 F3 78 */	mr r4, r30
/* 8005BF38 00058E78  48 00 20 19 */	bl cutList__Q28JASystem11TChannelMgrFPQ28JASystem8TChannel
/* 8005BF3C 00058E7C  2C 03 FF FF */	cmpwi r3, -1
/* 8005BF40 00058E80  41 82 00 14 */	beq lbl_8005BF54
/* 8005BF44 00058E84  80 7E 00 04 */	lwz r3, 4(r30)
/* 8005BF48 00058E88  38 9E 00 00 */	addi r4, r30, 0
/* 8005BF4C 00058E8C  38 A0 00 01 */	li r5, 1
/* 8005BF50 00058E90  48 00 1E F5 */	bl addListTail__Q28JASystem11TChannelMgrFPQ28JASystem8TChannelUl
lbl_8005BF54:
/* 8005BF54 00058E94  80 6D 90 EC */	lwz r3, cur_top__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BF58 00058E98  80 0D 81 98 */	lwz r0, waitMax__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BF5C 00058E9C  38 63 00 01 */	addi r3, r3, 1
/* 8005BF60 00058EA0  90 6D 90 EC */	stw r3, cur_top__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BF64 00058EA4  80 6D 90 EC */	lwz r3, cur_top__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BF68 00058EA8  7C 03 00 40 */	cmplw r3, r0
/* 8005BF6C 00058EAC  40 82 00 0C */	bne lbl_8005BF78
/* 8005BF70 00058EB0  38 00 00 00 */	li r0, 0
/* 8005BF74 00058EB4  90 0D 90 EC */	stw r0, cur_top__Q38JASystem6Driver8DSPQueue@sda21(r13)
lbl_8005BF78:
/* 8005BF78 00058EB8  80 6D 90 E8 */	lwz r3, cur_waits__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BF7C 00058EBC  28 1F 00 01 */	cmplwi r31, 1
/* 8005BF80 00058EC0  38 03 FF FF */	addi r0, r3, -1
/* 8005BF84 00058EC4  90 0D 90 E8 */	stw r0, cur_waits__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BF88 00058EC8  41 82 00 40 */	beq lbl_8005BFC8
/* 8005BF8C 00058ECC  48 00 00 30 */	b lbl_8005BFBC
lbl_8005BF90:
/* 8005BF90 00058ED0  38 63 00 01 */	addi r3, r3, 1
/* 8005BF94 00058ED4  80 0D 81 98 */	lwz r0, waitMax__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BF98 00058ED8  90 6D 90 EC */	stw r3, cur_top__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BF9C 00058EDC  80 6D 90 EC */	lwz r3, cur_top__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BFA0 00058EE0  7C 03 00 40 */	cmplw r3, r0
/* 8005BFA4 00058EE4  40 82 00 0C */	bne lbl_8005BFB0
/* 8005BFA8 00058EE8  38 00 00 00 */	li r0, 0
/* 8005BFAC 00058EEC  90 0D 90 EC */	stw r0, cur_top__Q38JASystem6Driver8DSPQueue@sda21(r13)
lbl_8005BFB0:
/* 8005BFB0 00058EF0  80 6D 90 E8 */	lwz r3, cur_waits__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BFB4 00058EF4  38 03 FF FF */	addi r0, r3, -1
/* 8005BFB8 00058EF8  90 0D 90 E8 */	stw r0, cur_waits__Q38JASystem6Driver8DSPQueue@sda21(r13)
lbl_8005BFBC:
/* 8005BFBC 00058EFC  80 0D 90 E8 */	lwz r0, cur_waits__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BFC0 00058F00  28 00 00 00 */	cmplwi r0, 0
/* 8005BFC4 00058F04  40 82 FF 30 */	bne lbl_8005BEF4
lbl_8005BFC8:
/* 8005BFC8 00058F08  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 8005BFCC 00058F0C  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 8005BFD0 00058F10  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 8005BFD4 00058F14  7C 08 03 A6 */	mtlr r0
/* 8005BFD8 00058F18  38 21 00 18 */	addi r1, r1, 0x18
/* 8005BFDC 00058F1C  4E 80 00 20 */	blr 

.global deleteQueue__Q38JASystem6Driver8DSPQueueFPQ28JASystem8TChannel
deleteQueue__Q38JASystem6Driver8DSPQueueFPQ28JASystem8TChannel:
/* 8005BFE0 00058F20  80 0D 90 E8 */	lwz r0, cur_waits__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BFE4 00058F24  39 00 00 00 */	li r8, 0
/* 8005BFE8 00058F28  80 ED 90 E0 */	lwz r7, waitp__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BFEC 00058F2C  28 00 00 00 */	cmplwi r0, 0
/* 8005BFF0 00058F30  7C 09 03 A6 */	mtctr r0
/* 8005BFF4 00058F34  80 AD 90 EC */	lwz r5, cur_top__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BFF8 00058F38  80 CD 81 98 */	lwz r6, waitMax__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005BFFC 00058F3C  40 81 00 3C */	ble lbl_8005C038
lbl_8005C000:
/* 8005C000 00058F40  7C 85 42 14 */	add r4, r5, r8
/* 8005C004 00058F44  7C 04 33 96 */	divwu r0, r4, r6
/* 8005C008 00058F48  7C 00 31 D6 */	mullw r0, r0, r6
/* 8005C00C 00058F4C  7C 00 20 50 */	subf r0, r0, r4
/* 8005C010 00058F50  54 04 10 3A */	slwi r4, r0, 2
/* 8005C014 00058F54  7C 07 20 2E */	lwzx r0, r7, r4
/* 8005C018 00058F58  7C 00 18 40 */	cmplw r0, r3
/* 8005C01C 00058F5C  40 82 00 14 */	bne lbl_8005C030
/* 8005C020 00058F60  38 00 00 00 */	li r0, 0
/* 8005C024 00058F64  7C 07 21 2E */	stwx r0, r7, r4
/* 8005C028 00058F68  38 60 00 01 */	li r3, 1
/* 8005C02C 00058F6C  4E 80 00 20 */	blr 
lbl_8005C030:
/* 8005C030 00058F70  39 08 00 01 */	addi r8, r8, 1
/* 8005C034 00058F74  42 00 FF CC */	bdnz lbl_8005C000
lbl_8005C038:
/* 8005C038 00058F78  38 60 00 00 */	li r3, 0
/* 8005C03C 00058F7C  4E 80 00 20 */	blr 

.global checkQueue__Q38JASystem6Driver8DSPQueueFv
checkQueue__Q38JASystem6Driver8DSPQueueFv:
/* 8005C040 00058F80  7C 08 02 A6 */	mflr r0
/* 8005C044 00058F84  38 60 00 00 */	li r3, 0
/* 8005C048 00058F88  90 01 00 04 */	stw r0, 4(r1)
/* 8005C04C 00058F8C  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 8005C050 00058F90  93 E1 00 0C */	stw r31, 0xc(r1)
/* 8005C054 00058F94  4B FF FE 85 */	bl deQueue__Q38JASystem6Driver8DSPQueueFUc
/* 8005C058 00058F98  3B E0 00 00 */	li r31, 0
/* 8005C05C 00058F9C  48 00 00 A0 */	b lbl_8005C0FC
lbl_8005C060:
/* 8005C060 00058FA0  80 0D 90 EC */	lwz r0, cur_top__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005C064 00058FA4  80 6D 81 98 */	lwz r3, waitMax__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005C068 00058FA8  7C 80 FA 14 */	add r4, r0, r31
/* 8005C06C 00058FAC  80 AD 90 E0 */	lwz r5, waitp__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005C070 00058FB0  7C 04 1B 96 */	divwu r0, r4, r3
/* 8005C074 00058FB4  7C 00 19 D6 */	mullw r0, r0, r3
/* 8005C078 00058FB8  7C 00 20 50 */	subf r0, r0, r4
/* 8005C07C 00058FBC  54 06 10 3A */	slwi r6, r0, 2
/* 8005C080 00058FC0  7C 05 30 2E */	lwzx r0, r5, r6
/* 8005C084 00058FC4  28 00 00 00 */	cmplwi r0, 0
/* 8005C088 00058FC8  7C 03 03 78 */	mr r3, r0
/* 8005C08C 00058FCC  41 82 00 6C */	beq lbl_8005C0F8
/* 8005C090 00058FD0  80 AD 90 E4 */	lwz r5, waittime__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005C094 00058FD4  7C 85 30 2E */	lwzx r4, r5, r6
/* 8005C098 00058FD8  38 04 00 01 */	addi r0, r4, 1
/* 8005C09C 00058FDC  7C 05 31 2E */	stwx r0, r5, r6
/* 8005C0A0 00058FE0  80 83 00 30 */	lwz r4, 0x30(r3)
/* 8005C0A4 00058FE4  2C 04 00 00 */	cmpwi r4, 0
/* 8005C0A8 00058FE8  40 81 00 0C */	ble lbl_8005C0B4
/* 8005C0AC 00058FEC  38 04 FF FF */	addi r0, r4, -1
/* 8005C0B0 00058FF0  90 03 00 30 */	stw r0, 0x30(r3)
lbl_8005C0B4:
/* 8005C0B4 00058FF4  80 03 00 30 */	lwz r0, 0x30(r3)
/* 8005C0B8 00058FF8  2C 00 00 00 */	cmpwi r0, 0
/* 8005C0BC 00058FFC  40 82 00 3C */	bne lbl_8005C0F8
/* 8005C0C0 00059000  81 83 00 28 */	lwz r12, 0x28(r3)
/* 8005C0C4 00059004  38 80 00 06 */	li r4, 6
/* 8005C0C8 00059008  7D 88 03 A6 */	mtlr r12
/* 8005C0CC 0005900C  4E 80 00 21 */	blrl 
/* 8005C0D0 00059010  80 0D 90 EC */	lwz r0, cur_top__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005C0D4 00059014  38 C0 00 00 */	li r6, 0
/* 8005C0D8 00059018  80 6D 81 98 */	lwz r3, waitMax__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005C0DC 0005901C  7C 80 FA 14 */	add r4, r0, r31
/* 8005C0E0 00059020  80 AD 90 E0 */	lwz r5, waitp__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005C0E4 00059024  7C 04 1B 96 */	divwu r0, r4, r3
/* 8005C0E8 00059028  7C 00 19 D6 */	mullw r0, r0, r3
/* 8005C0EC 0005902C  7C 00 20 50 */	subf r0, r0, r4
/* 8005C0F0 00059030  54 00 10 3A */	slwi r0, r0, 2
/* 8005C0F4 00059034  7C C5 01 2E */	stwx r6, r5, r0
lbl_8005C0F8:
/* 8005C0F8 00059038  3B FF 00 01 */	addi r31, r31, 1
lbl_8005C0FC:
/* 8005C0FC 0005903C  80 0D 90 E8 */	lwz r0, cur_waits__Q38JASystem6Driver8DSPQueue@sda21(r13)
/* 8005C100 00059040  7C 1F 00 40 */	cmplw r31, r0
/* 8005C104 00059044  41 80 FF 5C */	blt lbl_8005C060
/* 8005C108 00059048  80 01 00 14 */	lwz r0, 0x14(r1)
/* 8005C10C 0005904C  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 8005C110 00059050  38 21 00 10 */	addi r1, r1, 0x10
/* 8005C114 00059054  7C 08 03 A6 */	mtlr r0
/* 8005C118 00059058  4E 80 00 20 */	blr 

.section .sdata, "wa"  # 0x80408AC0 - 0x804097C0
waitMax__Q38JASystem6Driver8DSPQueue:
	.4byte 0x00000020
	.4byte 0

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
waitp__Q38JASystem6Driver8DSPQueue:
	.skip 0x4
waittime__Q38JASystem6Driver8DSPQueue:
	.skip 0x4
cur_waits__Q38JASystem6Driver8DSPQueue:
	.skip 0x4
cur_top__Q38JASystem6Driver8DSPQueue:
	.skip 0x4
cur_tail__Q38JASystem6Driver8DSPQueue:
	.skip 0x8

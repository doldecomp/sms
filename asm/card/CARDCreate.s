.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global CreateCallbackFat
CreateCallbackFat:
/* 800A3E78 000A0DB8  7C 08 02 A6 */	mflr r0
/* 800A3E7C 000A0DBC  90 01 00 04 */	stw r0, 4(r1)
/* 800A3E80 000A0DC0  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 800A3E84 000A0DC4  BF 61 00 14 */	stmw r27, 0x14(r1)
/* 800A3E88 000A0DC8  3B 83 00 00 */	addi r28, r3, 0
/* 800A3E8C 000A0DCC  3C 60 80 3F */	lis r3, __CARDBlock@ha
/* 800A3E90 000A0DD0  1C BC 01 10 */	mulli r5, r28, 0x110
/* 800A3E94 000A0DD4  38 03 BF 60 */	addi r0, r3, __CARDBlock@l
/* 800A3E98 000A0DD8  7F E0 2A 14 */	add r31, r0, r5
/* 800A3E9C 000A0DDC  83 BF 00 D0 */	lwz r29, 0xd0(r31)
/* 800A3EA0 000A0DE0  3B 60 00 00 */	li r27, 0
/* 800A3EA4 000A0DE4  7C 9E 23 79 */	or. r30, r4, r4
/* 800A3EA8 000A0DE8  93 7F 00 D0 */	stw r27, 0xd0(r31)
/* 800A3EAC 000A0DEC  41 80 00 C0 */	blt lbl_800A3F6C
/* 800A3EB0 000A0DF0  7F E3 FB 78 */	mr r3, r31
/* 800A3EB4 000A0DF4  4B FF D8 7D */	bl __CARDGetDirBlock
/* 800A3EB8 000A0DF8  A0 1F 00 BC */	lhz r0, 0xbc(r31)
/* 800A3EBC 000A0DFC  38 A0 00 04 */	li r5, 4
/* 800A3EC0 000A0E00  80 9F 01 0C */	lwz r4, 0x10c(r31)
/* 800A3EC4 000A0E04  54 00 30 32 */	slwi r0, r0, 6
/* 800A3EC8 000A0E08  7F C3 02 14 */	add r30, r3, r0
/* 800A3ECC 000A0E0C  38 7E 00 00 */	addi r3, r30, 0
/* 800A3ED0 000A0E10  4B F5 F3 25 */	bl memcpy
/* 800A3ED4 000A0E14  80 9F 01 0C */	lwz r4, 0x10c(r31)
/* 800A3ED8 000A0E18  38 7E 00 04 */	addi r3, r30, 4
/* 800A3EDC 000A0E1C  38 A0 00 02 */	li r5, 2
/* 800A3EE0 000A0E20  38 84 00 04 */	addi r4, r4, 4
/* 800A3EE4 000A0E24  4B F5 F3 11 */	bl memcpy
/* 800A3EE8 000A0E28  38 00 00 04 */	li r0, 4
/* 800A3EEC 000A0E2C  98 1E 00 34 */	stb r0, 0x34(r30)
/* 800A3EF0 000A0E30  38 00 FF FF */	li r0, -1
/* 800A3EF4 000A0E34  3C 60 80 00 */	lis r3, 0x800000F8@ha
/* 800A3EF8 000A0E38  9B 7E 00 35 */	stb r27, 0x35(r30)
/* 800A3EFC 000A0E3C  A0 9F 00 BE */	lhz r4, 0xbe(r31)
/* 800A3F00 000A0E40  B0 9E 00 36 */	sth r4, 0x36(r30)
/* 800A3F04 000A0E44  9B 7E 00 07 */	stb r27, 7(r30)
/* 800A3F08 000A0E48  90 1E 00 2C */	stw r0, 0x2c(r30)
/* 800A3F0C 000A0E4C  B3 7E 00 30 */	sth r27, 0x30(r30)
/* 800A3F10 000A0E50  B3 7E 00 32 */	sth r27, 0x32(r30)
/* 800A3F14 000A0E54  90 1E 00 3C */	stw r0, 0x3c(r30)
/* 800A3F18 000A0E58  A0 1E 00 32 */	lhz r0, 0x32(r30)
/* 800A3F1C 000A0E5C  54 00 00 3A */	rlwinm r0, r0, 0, 0, 0x1d
/* 800A3F20 000A0E60  60 00 00 01 */	ori r0, r0, 1
/* 800A3F24 000A0E64  B0 1E 00 32 */	sth r0, 0x32(r30)
/* 800A3F28 000A0E68  80 9F 00 C0 */	lwz r4, 0xc0(r31)
/* 800A3F2C 000A0E6C  93 64 00 08 */	stw r27, 8(r4)
/* 800A3F30 000A0E70  A0 1E 00 36 */	lhz r0, 0x36(r30)
/* 800A3F34 000A0E74  80 9F 00 C0 */	lwz r4, 0xc0(r31)
/* 800A3F38 000A0E78  B0 04 00 10 */	sth r0, 0x10(r4)
/* 800A3F3C 000A0E7C  80 03 00 F8 */	lwz r0, 0x800000F8@l(r3)
/* 800A3F40 000A0E80  54 1B F0 BE */	srwi r27, r0, 2
/* 800A3F44 000A0E84  4B FE FE B5 */	bl OSGetTime
/* 800A3F48 000A0E88  38 DB 00 00 */	addi r6, r27, 0
/* 800A3F4C 000A0E8C  38 A0 00 00 */	li r5, 0
/* 800A3F50 000A0E90  4B FD ED B5 */	bl __div2i
/* 800A3F54 000A0E94  90 9E 00 28 */	stw r4, 0x28(r30)
/* 800A3F58 000A0E98  38 7C 00 00 */	addi r3, r28, 0
/* 800A3F5C 000A0E9C  38 9D 00 00 */	addi r4, r29, 0
/* 800A3F60 000A0EA0  4B FF D9 71 */	bl __CARDUpdateDir
/* 800A3F64 000A0EA4  7C 7E 1B 79 */	or. r30, r3, r3
/* 800A3F68 000A0EA8  40 80 00 2C */	bge lbl_800A3F94
lbl_800A3F6C:
/* 800A3F6C 000A0EAC  38 7F 00 00 */	addi r3, r31, 0
/* 800A3F70 000A0EB0  38 9E 00 00 */	addi r4, r30, 0
/* 800A3F74 000A0EB4  4B FF BC D9 */	bl __CARDPutControlBlock
/* 800A3F78 000A0EB8  28 1D 00 00 */	cmplwi r29, 0
/* 800A3F7C 000A0EBC  41 82 00 18 */	beq lbl_800A3F94
/* 800A3F80 000A0EC0  39 9D 00 00 */	addi r12, r29, 0
/* 800A3F84 000A0EC4  7D 88 03 A6 */	mtlr r12
/* 800A3F88 000A0EC8  38 7C 00 00 */	addi r3, r28, 0
/* 800A3F8C 000A0ECC  38 9E 00 00 */	addi r4, r30, 0
/* 800A3F90 000A0ED0  4E 80 00 21 */	blrl 
lbl_800A3F94:
/* 800A3F94 000A0ED4  BB 61 00 14 */	lmw r27, 0x14(r1)
/* 800A3F98 000A0ED8  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 800A3F9C 000A0EDC  38 21 00 28 */	addi r1, r1, 0x28
/* 800A3FA0 000A0EE0  7C 08 03 A6 */	mtlr r0
/* 800A3FA4 000A0EE4  4E 80 00 20 */	blr 

.global CARDCreateAsync
CARDCreateAsync:
/* 800A3FA8 000A0EE8  7C 08 02 A6 */	mflr r0
/* 800A3FAC 000A0EEC  90 01 00 04 */	stw r0, 4(r1)
/* 800A3FB0 000A0EF0  94 21 FF B8 */	stwu r1, -0x48(r1)
/* 800A3FB4 000A0EF4  BE E1 00 24 */	stmw r23, 0x24(r1)
/* 800A3FB8 000A0EF8  3B 44 00 00 */	addi r26, r4, 0
/* 800A3FBC 000A0EFC  3B 23 00 00 */	addi r25, r3, 0
/* 800A3FC0 000A0F00  3B 65 00 00 */	addi r27, r5, 0
/* 800A3FC4 000A0F04  3B 86 00 00 */	addi r28, r6, 0
/* 800A3FC8 000A0F08  3B A7 00 00 */	addi r29, r7, 0
/* 800A3FCC 000A0F0C  38 7A 00 00 */	addi r3, r26, 0
/* 800A3FD0 000A0F10  4B FE 1E D5 */	bl strlen
/* 800A3FD4 000A0F14  28 03 00 20 */	cmplwi r3, 0x20
/* 800A3FD8 000A0F18  40 81 00 0C */	ble lbl_800A3FE4
/* 800A3FDC 000A0F1C  38 60 FF F4 */	li r3, -12
/* 800A3FE0 000A0F20  48 00 01 D4 */	b lbl_800A41B4
lbl_800A3FE4:
/* 800A3FE4 000A0F24  38 79 00 00 */	addi r3, r25, 0
/* 800A3FE8 000A0F28  38 81 00 1C */	addi r4, r1, 0x1c
/* 800A3FEC 000A0F2C  4B FF BB A9 */	bl __CARDGetControlBlock
/* 800A3FF0 000A0F30  2C 03 00 00 */	cmpwi r3, 0
/* 800A3FF4 000A0F34  40 80 00 08 */	bge lbl_800A3FFC
/* 800A3FF8 000A0F38  48 00 01 BC */	b lbl_800A41B4
lbl_800A3FFC:
/* 800A3FFC 000A0F3C  28 1B 00 00 */	cmplwi r27, 0
/* 800A4000 000A0F40  41 82 00 1C */	beq lbl_800A401C
/* 800A4004 000A0F44  80 61 00 1C */	lwz r3, 0x1c(r1)
/* 800A4008 000A0F48  80 83 00 0C */	lwz r4, 0xc(r3)
/* 800A400C 000A0F4C  7C 1B 23 96 */	divwu r0, r27, r4
/* 800A4010 000A0F50  7C 00 21 D6 */	mullw r0, r0, r4
/* 800A4014 000A0F54  7C 00 D8 51 */	subf. r0, r0, r27
/* 800A4018 000A0F58  41 82 00 0C */	beq lbl_800A4024
lbl_800A401C:
/* 800A401C 000A0F5C  38 60 FF 80 */	li r3, -128
/* 800A4020 000A0F60  48 00 01 94 */	b lbl_800A41B4
lbl_800A4024:
/* 800A4024 000A0F64  3C 80 00 01 */	lis r4, 0x0000FFFF@ha
/* 800A4028 000A0F68  3B C4 FF FF */	addi r30, r4, 0x0000FFFF@l
/* 800A402C 000A0F6C  4B FF D7 05 */	bl __CARDGetDirBlock
/* 800A4030 000A0F70  3B E3 00 00 */	addi r31, r3, 0
/* 800A4034 000A0F74  3B 1F 00 00 */	addi r24, r31, 0
/* 800A4038 000A0F78  3A E0 00 00 */	li r23, 0
/* 800A403C 000A0F7C  48 00 00 8C */	b lbl_800A40C8
lbl_800A4040:
/* 800A4040 000A0F80  88 18 00 00 */	lbz r0, 0(r24)
/* 800A4044 000A0F84  28 00 00 FF */	cmplwi r0, 0xff
/* 800A4048 000A0F88  40 82 00 18 */	bne lbl_800A4060
/* 800A404C 000A0F8C  57 C0 04 3E */	clrlwi r0, r30, 0x10
/* 800A4050 000A0F90  28 00 FF FF */	cmplwi r0, 0xffff
/* 800A4054 000A0F94  40 82 00 6C */	bne lbl_800A40C0
/* 800A4058 000A0F98  7E FE BB 78 */	mr r30, r23
/* 800A405C 000A0F9C  48 00 00 64 */	b lbl_800A40C0
lbl_800A4060:
/* 800A4060 000A0FA0  80 81 00 1C */	lwz r4, 0x1c(r1)
/* 800A4064 000A0FA4  38 78 00 00 */	addi r3, r24, 0
/* 800A4068 000A0FA8  38 A0 00 04 */	li r5, 4
/* 800A406C 000A0FAC  80 84 01 0C */	lwz r4, 0x10c(r4)
/* 800A4070 000A0FB0  4B FD FB 55 */	bl memcmp
/* 800A4074 000A0FB4  2C 03 00 00 */	cmpwi r3, 0
/* 800A4078 000A0FB8  40 82 00 48 */	bne lbl_800A40C0
/* 800A407C 000A0FBC  80 81 00 1C */	lwz r4, 0x1c(r1)
/* 800A4080 000A0FC0  38 78 00 04 */	addi r3, r24, 4
/* 800A4084 000A0FC4  38 A0 00 02 */	li r5, 2
/* 800A4088 000A0FC8  80 84 01 0C */	lwz r4, 0x10c(r4)
/* 800A408C 000A0FCC  38 84 00 04 */	addi r4, r4, 4
/* 800A4090 000A0FD0  4B FD FB 35 */	bl memcmp
/* 800A4094 000A0FD4  2C 03 00 00 */	cmpwi r3, 0
/* 800A4098 000A0FD8  40 82 00 28 */	bne lbl_800A40C0
/* 800A409C 000A0FDC  38 78 00 00 */	addi r3, r24, 0
/* 800A40A0 000A0FE0  38 9A 00 00 */	addi r4, r26, 0
/* 800A40A4 000A0FE4  4B FF FA D9 */	bl __CARDCompareFileName
/* 800A40A8 000A0FE8  2C 03 00 00 */	cmpwi r3, 0
/* 800A40AC 000A0FEC  41 82 00 14 */	beq lbl_800A40C0
/* 800A40B0 000A0FF0  80 61 00 1C */	lwz r3, 0x1c(r1)
/* 800A40B4 000A0FF4  38 80 FF F9 */	li r4, -7
/* 800A40B8 000A0FF8  4B FF BB 95 */	bl __CARDPutControlBlock
/* 800A40BC 000A0FFC  48 00 00 F8 */	b lbl_800A41B4
lbl_800A40C0:
/* 800A40C0 000A1000  3B 18 00 40 */	addi r24, r24, 0x40
/* 800A40C4 000A1004  3A F7 00 01 */	addi r23, r23, 1
lbl_800A40C8:
/* 800A40C8 000A1008  56 E0 04 3E */	clrlwi r0, r23, 0x10
/* 800A40CC 000A100C  28 00 00 7F */	cmplwi r0, 0x7f
/* 800A40D0 000A1010  41 80 FF 70 */	blt lbl_800A4040
/* 800A40D4 000A1014  57 C0 04 3E */	clrlwi r0, r30, 0x10
/* 800A40D8 000A1018  28 00 FF FF */	cmplwi r0, 0xffff
/* 800A40DC 000A101C  40 82 00 14 */	bne lbl_800A40F0
/* 800A40E0 000A1020  80 61 00 1C */	lwz r3, 0x1c(r1)
/* 800A40E4 000A1024  38 80 FF F8 */	li r4, -8
/* 800A40E8 000A1028  4B FF BB 65 */	bl __CARDPutControlBlock
/* 800A40EC 000A102C  48 00 00 C8 */	b lbl_800A41B4
lbl_800A40F0:
/* 800A40F0 000A1030  80 61 00 1C */	lwz r3, 0x1c(r1)
/* 800A40F4 000A1034  4B FF D2 D5 */	bl __CARDGetFatBlock
/* 800A40F8 000A1038  80 81 00 1C */	lwz r4, 0x1c(r1)
/* 800A40FC 000A103C  A0 03 00 06 */	lhz r0, 6(r3)
/* 800A4100 000A1040  80 64 00 0C */	lwz r3, 0xc(r4)
/* 800A4104 000A1044  7C 03 01 D6 */	mullw r0, r3, r0
/* 800A4108 000A1048  7C 00 D8 40 */	cmplw r0, r27
/* 800A410C 000A104C  40 80 00 14 */	bge lbl_800A4120
/* 800A4110 000A1050  38 64 00 00 */	addi r3, r4, 0
/* 800A4114 000A1054  38 80 FF F7 */	li r4, -9
/* 800A4118 000A1058  4B FF BB 35 */	bl __CARDPutControlBlock
/* 800A411C 000A105C  48 00 00 98 */	b lbl_800A41B4
lbl_800A4120:
/* 800A4120 000A1060  28 1D 00 00 */	cmplwi r29, 0
/* 800A4124 000A1064  41 82 00 0C */	beq lbl_800A4130
/* 800A4128 000A1068  7F A0 EB 78 */	mr r0, r29
/* 800A412C 000A106C  48 00 00 0C */	b lbl_800A4138
lbl_800A4130:
/* 800A4130 000A1070  3C 60 80 0A */	lis r3, __CARDDefaultApiCallback@ha
/* 800A4134 000A1074  38 03 EC 4C */	addi r0, r3, __CARDDefaultApiCallback@l
lbl_800A4138:
/* 800A4138 000A1078  90 04 00 D0 */	stw r0, 0xd0(r4)
/* 800A413C 000A107C  57 C0 32 B2 */	rlwinm r0, r30, 6, 0xa, 0x19
/* 800A4140 000A1080  7C FF 02 14 */	add r7, r31, r0
/* 800A4144 000A1084  80 61 00 1C */	lwz r3, 0x1c(r1)
/* 800A4148 000A1088  57 DD 04 3E */	clrlwi r29, r30, 0x10
/* 800A414C 000A108C  38 9A 00 00 */	addi r4, r26, 0
/* 800A4150 000A1090  B3 C3 00 BC */	sth r30, 0xbc(r3)
/* 800A4154 000A1094  38 67 00 08 */	addi r3, r7, 8
/* 800A4158 000A1098  38 A0 00 20 */	li r5, 0x20
/* 800A415C 000A109C  80 C1 00 1C */	lwz r6, 0x1c(r1)
/* 800A4160 000A10A0  80 06 00 0C */	lwz r0, 0xc(r6)
/* 800A4164 000A10A4  7C 1B 03 96 */	divwu r0, r27, r0
/* 800A4168 000A10A8  B0 07 00 38 */	sth r0, 0x38(r7)
/* 800A416C 000A10AC  4B FE 1C 41 */	bl strncpy
/* 800A4170 000A10B0  80 81 00 1C */	lwz r4, 0x1c(r1)
/* 800A4174 000A10B4  3C 60 80 0A */	lis r3, CreateCallbackFat@ha
/* 800A4178 000A10B8  38 A3 3E 78 */	addi r5, r3, CreateCallbackFat@l
/* 800A417C 000A10BC  93 84 00 C0 */	stw r28, 0xc0(r4)
/* 800A4180 000A10C0  7F 23 CB 78 */	mr r3, r25
/* 800A4184 000A10C4  93 3C 00 00 */	stw r25, 0(r28)
/* 800A4188 000A10C8  93 BC 00 04 */	stw r29, 4(r28)
/* 800A418C 000A10CC  80 81 00 1C */	lwz r4, 0x1c(r1)
/* 800A4190 000A10D0  80 04 00 0C */	lwz r0, 0xc(r4)
/* 800A4194 000A10D4  7C 9B 03 96 */	divwu r4, r27, r0
/* 800A4198 000A10D8  4B FF D3 D5 */	bl __CARDAllocBlock
/* 800A419C 000A10DC  7C 64 1B 79 */	or. r4, r3, r3
/* 800A41A0 000A10E0  40 80 00 10 */	bge lbl_800A41B0
/* 800A41A4 000A10E4  80 61 00 1C */	lwz r3, 0x1c(r1)
/* 800A41A8 000A10E8  4B FF BA A5 */	bl __CARDPutControlBlock
/* 800A41AC 000A10EC  48 00 00 08 */	b lbl_800A41B4
lbl_800A41B0:
/* 800A41B0 000A10F0  7C 83 23 78 */	mr r3, r4
lbl_800A41B4:
/* 800A41B4 000A10F4  BA E1 00 24 */	lmw r23, 0x24(r1)
/* 800A41B8 000A10F8  80 01 00 4C */	lwz r0, 0x4c(r1)
/* 800A41BC 000A10FC  38 21 00 48 */	addi r1, r1, 0x48
/* 800A41C0 000A1100  7C 08 03 A6 */	mtlr r0
/* 800A41C4 000A1104  4E 80 00 20 */	blr 

.global CARDCreate
CARDCreate:
/* 800A41C8 000A1108  7C 08 02 A6 */	mflr r0
/* 800A41CC 000A110C  3C E0 80 0A */	lis r7, __CARDSyncCallback@ha
/* 800A41D0 000A1110  90 01 00 04 */	stw r0, 4(r1)
/* 800A41D4 000A1114  38 E7 EC 50 */	addi r7, r7, __CARDSyncCallback@l
/* 800A41D8 000A1118  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 800A41DC 000A111C  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 800A41E0 000A1120  3B E3 00 00 */	addi r31, r3, 0
/* 800A41E4 000A1124  4B FF FD C5 */	bl CARDCreateAsync
/* 800A41E8 000A1128  2C 03 00 00 */	cmpwi r3, 0
/* 800A41EC 000A112C  40 80 00 08 */	bge lbl_800A41F4
/* 800A41F0 000A1130  48 00 00 0C */	b lbl_800A41FC
lbl_800A41F4:
/* 800A41F4 000A1134  7F E3 FB 78 */	mr r3, r31
/* 800A41F8 000A1138  4B FF BC 09 */	bl __CARDSync
lbl_800A41FC:
/* 800A41FC 000A113C  80 01 00 24 */	lwz r0, 0x24(r1)
/* 800A4200 000A1140  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 800A4204 000A1144  38 21 00 20 */	addi r1, r1, 0x20
/* 800A4208 000A1148  7C 08 03 A6 */	mtlr r0
/* 800A420C 000A114C  4E 80 00 20 */	blr 
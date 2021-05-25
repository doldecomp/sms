.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global loadAfter__14TPollutionTestFv
loadAfter__14TPollutionTestFv:
/* 801F40C4 001F1004  7C 08 02 A6 */	mflr r0
/* 801F40C8 001F1008  90 01 00 04 */	stw r0, 4(r1)
/* 801F40CC 001F100C  94 21 FF F8 */	stwu r1, -8(r1)
/* 801F40D0 001F1010  4B E5 07 DD */	bl loadAfter__Q26JDrama8TNameRefFv
/* 801F40D4 001F1014  80 01 00 0C */	lwz r0, 0xc(r1)
/* 801F40D8 001F1018  38 21 00 08 */	addi r1, r1, 8
/* 801F40DC 001F101C  7C 08 03 A6 */	mtlr r0
/* 801F40E0 001F1020  4E 80 00 20 */	blr 

.global __dt__14TPollutionTestFv
__dt__14TPollutionTestFv:
/* 801F40E4 001F1024  7C 08 02 A6 */	mflr r0
/* 801F40E8 001F1028  90 01 00 04 */	stw r0, 4(r1)
/* 801F40EC 001F102C  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 801F40F0 001F1030  93 E1 00 14 */	stw r31, 0x14(r1)
/* 801F40F4 001F1034  3B E4 00 00 */	addi r31, r4, 0
/* 801F40F8 001F1038  93 C1 00 10 */	stw r30, 0x10(r1)
/* 801F40FC 001F103C  7C 7E 1B 79 */	or. r30, r3, r3
/* 801F4100 001F1040  41 82 00 3C */	beq lbl_801F413C
/* 801F4104 001F1044  3C 60 80 3D */	lis r3, __vt__14TPollutionTest@ha
/* 801F4108 001F1048  38 03 08 C8 */	addi r0, r3, __vt__14TPollutionTest@l
/* 801F410C 001F104C  90 1E 00 00 */	stw r0, 0(r30)
/* 801F4110 001F1050  41 82 00 1C */	beq lbl_801F412C
/* 801F4114 001F1054  3C 60 80 3B */	lis r3, __vt__Q26JDrama8TViewObj@ha
/* 801F4118 001F1058  38 03 A0 C0 */	addi r0, r3, __vt__Q26JDrama8TViewObj@l
/* 801F411C 001F105C  90 1E 00 00 */	stw r0, 0(r30)
/* 801F4120 001F1060  38 7E 00 00 */	addi r3, r30, 0
/* 801F4124 001F1064  38 80 00 00 */	li r4, 0
/* 801F4128 001F1068  4B E5 06 D5 */	bl __dt__Q26JDrama8TNameRefFv
lbl_801F412C:
/* 801F412C 001F106C  7F E0 07 35 */	extsh. r0, r31
/* 801F4130 001F1070  40 81 00 0C */	ble lbl_801F413C
/* 801F4134 001F1074  7F C3 F3 78 */	mr r3, r30
/* 801F4138 001F1078  4B E1 89 79 */	bl __dl__FPv
lbl_801F413C:
/* 801F413C 001F107C  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 801F4140 001F1080  7F C3 F3 78 */	mr r3, r30
/* 801F4144 001F1084  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 801F4148 001F1088  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 801F414C 001F108C  7C 08 03 A6 */	mtlr r0
/* 801F4150 001F1090  38 21 00 18 */	addi r1, r1, 0x18
/* 801F4154 001F1094  4E 80 00 20 */	blr 

.global perform__14TPollutionTestFUlPQ26JDrama9TGraphics
perform__14TPollutionTestFUlPQ26JDrama9TGraphics:
/* 801F4158 001F1098  4E 80 00 20 */	blr 

.global __sinit_PollutionEvent_cpp
__sinit_PollutionEvent_cpp:
/* 801F415C 001F109C  7C 08 02 A6 */	mflr r0
/* 801F4160 001F10A0  3C 60 80 40 */	lis r3, $$22727@ha
/* 801F4164 001F10A4  90 01 00 04 */	stw r0, 4(r1)
/* 801F4168 001F10A8  94 21 FF F0 */	stwu r1, -0x10(r1)
/* 801F416C 001F10AC  93 E1 00 0C */	stw r31, 0xc(r1)
/* 801F4170 001F10B0  3B E3 88 E8 */	addi r31, r3, $$22727@l
/* 801F4174 001F10B4  88 0D 97 CC */	lbz r0, __init__smList__15JALList$$05MSBgm$$1@sda21(r13)
/* 801F4178 001F10B8  7C 00 07 75 */	extsb. r0, r0
/* 801F417C 001F10BC  40 82 00 28 */	bne lbl_801F41A4
/* 801F4180 001F10C0  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1@sda21
/* 801F4184 001F10C4  4B E1 AA B1 */	bl initiate__10JSUPtrListFv
/* 801F4188 001F10C8  3C 60 80 0E */	lis r3, __dt__15JSUList$$05MSBgm$$1Fv@ha
/* 801F418C 001F10CC  38 83 6A 44 */	addi r4, r3, __dt__15JSUList$$05MSBgm$$1Fv@l
/* 801F4190 001F10D0  38 6D 97 A8 */	addi r3, r13, smList__15JALList$$05MSBgm$$1@sda21
/* 801F4194 001F10D4  38 BF 00 00 */	addi r5, r31, 0
/* 801F4198 001F10D8  4B E8 E5 91 */	bl __register_global_object
/* 801F419C 001F10DC  38 00 00 01 */	li r0, 1
/* 801F41A0 001F10E0  98 0D 97 CC */	stb r0, __init__smList__15JALList$$05MSBgm$$1@sda21(r13)
lbl_801F41A4:
/* 801F41A4 001F10E4  88 0D 97 CD */	lbz r0, __init__smList__24JALList$$013MSSetSoundGrp$$1@sda21(r13)
/* 801F41A8 001F10E8  7C 00 07 75 */	extsb. r0, r0
/* 801F41AC 001F10EC  40 82 00 28 */	bne lbl_801F41D4
/* 801F41B0 001F10F0  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1@sda21
/* 801F41B4 001F10F4  4B E1 AA 81 */	bl initiate__10JSUPtrListFv
/* 801F41B8 001F10F8  3C 60 80 0E */	lis r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@ha
/* 801F41BC 001F10FC  38 83 69 EC */	addi r4, r3, __dt__24JSUList$$013MSSetSoundGrp$$1Fv@l
/* 801F41C0 001F1100  38 6D 97 B4 */	addi r3, r13, smList__24JALList$$013MSSetSoundGrp$$1@sda21
/* 801F41C4 001F1104  38 BF 00 0C */	addi r5, r31, 0xc
/* 801F41C8 001F1108  4B E8 E5 61 */	bl __register_global_object
/* 801F41CC 001F110C  38 00 00 01 */	li r0, 1
/* 801F41D0 001F1110  98 0D 97 CD */	stb r0, __init__smList__24JALList$$013MSSetSoundGrp$$1@sda21(r13)
lbl_801F41D4:
/* 801F41D4 001F1114  88 0D 97 CE */	lbz r0, __init__smList__21JALList$$010MSSetSound$$1@sda21(r13)
/* 801F41D8 001F1118  7C 00 07 75 */	extsb. r0, r0
/* 801F41DC 001F111C  40 82 00 28 */	bne lbl_801F4204
/* 801F41E0 001F1120  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1@sda21
/* 801F41E4 001F1124  4B E1 AA 51 */	bl initiate__10JSUPtrListFv
/* 801F41E8 001F1128  3C 60 80 0E */	lis r3, __dt__21JSUList$$010MSSetSound$$1Fv@ha
/* 801F41EC 001F112C  38 83 69 94 */	addi r4, r3, __dt__21JSUList$$010MSSetSound$$1Fv@l
/* 801F41F0 001F1130  38 6D 97 C0 */	addi r3, r13, smList__21JALList$$010MSSetSound$$1@sda21
/* 801F41F4 001F1134  38 BF 00 18 */	addi r5, r31, 0x18
/* 801F41F8 001F1138  4B E8 E5 31 */	bl __register_global_object
/* 801F41FC 001F113C  38 00 00 01 */	li r0, 1
/* 801F4200 001F1140  98 0D 97 CE */	stb r0, __init__smList__21JALList$$010MSSetSound$$1@sda21(r13)
lbl_801F4204:
/* 801F4204 001F1144  88 0D 8F 8C */	lbz r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1@sda21(r13)
/* 801F4208 001F1148  7C 00 07 75 */	extsb. r0, r0
/* 801F420C 001F114C  40 82 00 28 */	bne lbl_801F4234
/* 801F4210 001F1150  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1@sda21
/* 801F4214 001F1154  4B E1 AA 21 */	bl initiate__10JSUPtrListFv
/* 801F4218 001F1158  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@ha
/* 801F421C 001F115C  38 83 A8 0C */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDGrp$$1Fv@l
/* 801F4220 001F1160  38 6D 8E FC */	addi r3, r13, smList__26JALList$$015JALSeModEffDGrp$$1@sda21
/* 801F4224 001F1164  38 BF 00 24 */	addi r5, r31, 0x24
/* 801F4228 001F1168  4B E8 E5 01 */	bl __register_global_object
/* 801F422C 001F116C  38 00 00 01 */	li r0, 1
/* 801F4230 001F1170  98 0D 8F 8C */	stb r0, __init__smList__26JALList$$015JALSeModEffDGrp$$1@sda21(r13)
lbl_801F4234:
/* 801F4234 001F1174  88 0D 8F 8D */	lbz r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1@sda21(r13)
/* 801F4238 001F1178  7C 00 07 75 */	extsb. r0, r0
/* 801F423C 001F117C  40 82 00 28 */	bne lbl_801F4264
/* 801F4240 001F1180  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1@sda21
/* 801F4244 001F1184  4B E1 A9 F1 */	bl initiate__10JSUPtrListFv
/* 801F4248 001F1188  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@ha
/* 801F424C 001F118C  38 83 A7 B4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDGrp$$1Fv@l
/* 801F4250 001F1190  38 6D 8F 08 */	addi r3, r13, smList__26JALList$$015JALSeModPitDGrp$$1@sda21
/* 801F4254 001F1194  38 BF 00 30 */	addi r5, r31, 0x30
/* 801F4258 001F1198  4B E8 E4 D1 */	bl __register_global_object
/* 801F425C 001F119C  38 00 00 01 */	li r0, 1
/* 801F4260 001F11A0  98 0D 8F 8D */	stb r0, __init__smList__26JALList$$015JALSeModPitDGrp$$1@sda21(r13)
lbl_801F4264:
/* 801F4264 001F11A4  88 0D 8F 8E */	lbz r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1@sda21(r13)
/* 801F4268 001F11A8  7C 00 07 75 */	extsb. r0, r0
/* 801F426C 001F11AC  40 82 00 28 */	bne lbl_801F4294
/* 801F4270 001F11B0  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1@sda21
/* 801F4274 001F11B4  4B E1 A9 C1 */	bl initiate__10JSUPtrListFv
/* 801F4278 001F11B8  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@ha
/* 801F427C 001F11BC  38 83 A7 5C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDGrp$$1Fv@l
/* 801F4280 001F11C0  38 6D 8F 14 */	addi r3, r13, smList__26JALList$$015JALSeModVolDGrp$$1@sda21
/* 801F4284 001F11C4  38 BF 00 3C */	addi r5, r31, 0x3c
/* 801F4288 001F11C8  4B E8 E4 A1 */	bl __register_global_object
/* 801F428C 001F11CC  38 00 00 01 */	li r0, 1
/* 801F4290 001F11D0  98 0D 8F 8E */	stb r0, __init__smList__26JALList$$015JALSeModVolDGrp$$1@sda21(r13)
lbl_801F4294:
/* 801F4294 001F11D4  88 0D 8F 8F */	lbz r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1@sda21(r13)
/* 801F4298 001F11D8  7C 00 07 75 */	extsb. r0, r0
/* 801F429C 001F11DC  40 82 00 28 */	bne lbl_801F42C4
/* 801F42A0 001F11E0  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1@sda21
/* 801F42A4 001F11E4  4B E1 A9 91 */	bl initiate__10JSUPtrListFv
/* 801F42A8 001F11E8  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@ha
/* 801F42AC 001F11EC  38 83 A7 04 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFGrp$$1Fv@l
/* 801F42B0 001F11F0  38 6D 8F 20 */	addi r3, r13, smList__26JALList$$015JALSeModEffFGrp$$1@sda21
/* 801F42B4 001F11F4  38 BF 00 48 */	addi r5, r31, 0x48
/* 801F42B8 001F11F8  4B E8 E4 71 */	bl __register_global_object
/* 801F42BC 001F11FC  38 00 00 01 */	li r0, 1
/* 801F42C0 001F1200  98 0D 8F 8F */	stb r0, __init__smList__26JALList$$015JALSeModEffFGrp$$1@sda21(r13)
lbl_801F42C4:
/* 801F42C4 001F1204  88 0D 8F 90 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1@sda21(r13)
/* 801F42C8 001F1208  7C 00 07 75 */	extsb. r0, r0
/* 801F42CC 001F120C  40 82 00 28 */	bne lbl_801F42F4
/* 801F42D0 001F1210  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1@sda21
/* 801F42D4 001F1214  4B E1 A9 61 */	bl initiate__10JSUPtrListFv
/* 801F42D8 001F1218  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@ha
/* 801F42DC 001F121C  38 83 A6 AC */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFGrp$$1Fv@l
/* 801F42E0 001F1220  38 6D 8F 2C */	addi r3, r13, smList__26JALList$$015JALSeModPitFGrp$$1@sda21
/* 801F42E4 001F1224  38 BF 00 54 */	addi r5, r31, 0x54
/* 801F42E8 001F1228  4B E8 E4 41 */	bl __register_global_object
/* 801F42EC 001F122C  38 00 00 01 */	li r0, 1
/* 801F42F0 001F1230  98 0D 8F 90 */	stb r0, __init__smList__26JALList$$015JALSeModPitFGrp$$1@sda21(r13)
lbl_801F42F4:
/* 801F42F4 001F1234  88 0D 8F 91 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1@sda21(r13)
/* 801F42F8 001F1238  7C 00 07 75 */	extsb. r0, r0
/* 801F42FC 001F123C  40 82 00 28 */	bne lbl_801F4324
/* 801F4300 001F1240  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1@sda21
/* 801F4304 001F1244  4B E1 A9 31 */	bl initiate__10JSUPtrListFv
/* 801F4308 001F1248  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@ha
/* 801F430C 001F124C  38 83 A6 54 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFGrp$$1Fv@l
/* 801F4310 001F1250  38 6D 8F 38 */	addi r3, r13, smList__26JALList$$015JALSeModVolFGrp$$1@sda21
/* 801F4314 001F1254  38 BF 00 60 */	addi r5, r31, 0x60
/* 801F4318 001F1258  4B E8 E4 11 */	bl __register_global_object
/* 801F431C 001F125C  38 00 00 01 */	li r0, 1
/* 801F4320 001F1260  98 0D 8F 91 */	stb r0, __init__smList__26JALList$$015JALSeModVolFGrp$$1@sda21(r13)
lbl_801F4324:
/* 801F4324 001F1264  88 0D 8F 92 */	lbz r0, __init__smList__26JALList$$015JALSeModEffDist$$1@sda21(r13)
/* 801F4328 001F1268  7C 00 07 75 */	extsb. r0, r0
/* 801F432C 001F126C  40 82 00 28 */	bne lbl_801F4354
/* 801F4330 001F1270  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1@sda21
/* 801F4334 001F1274  4B E1 A9 01 */	bl initiate__10JSUPtrListFv
/* 801F4338 001F1278  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@ha
/* 801F433C 001F127C  38 83 A5 FC */	addi r4, r3, __dt__26JSUList$$015JALSeModEffDist$$1Fv@l
/* 801F4340 001F1280  38 6D 8F 44 */	addi r3, r13, smList__26JALList$$015JALSeModEffDist$$1@sda21
/* 801F4344 001F1284  38 BF 00 6C */	addi r5, r31, 0x6c
/* 801F4348 001F1288  4B E8 E3 E1 */	bl __register_global_object
/* 801F434C 001F128C  38 00 00 01 */	li r0, 1
/* 801F4350 001F1290  98 0D 8F 92 */	stb r0, __init__smList__26JALList$$015JALSeModEffDist$$1@sda21(r13)
lbl_801F4354:
/* 801F4354 001F1294  88 0D 8F 93 */	lbz r0, __init__smList__26JALList$$015JALSeModPitDist$$1@sda21(r13)
/* 801F4358 001F1298  7C 00 07 75 */	extsb. r0, r0
/* 801F435C 001F129C  40 82 00 28 */	bne lbl_801F4384
/* 801F4360 001F12A0  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1@sda21
/* 801F4364 001F12A4  4B E1 A8 D1 */	bl initiate__10JSUPtrListFv
/* 801F4368 001F12A8  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@ha
/* 801F436C 001F12AC  38 83 A5 A4 */	addi r4, r3, __dt__26JSUList$$015JALSeModPitDist$$1Fv@l
/* 801F4370 001F12B0  38 6D 8F 50 */	addi r3, r13, smList__26JALList$$015JALSeModPitDist$$1@sda21
/* 801F4374 001F12B4  38 BF 00 78 */	addi r5, r31, 0x78
/* 801F4378 001F12B8  4B E8 E3 B1 */	bl __register_global_object
/* 801F437C 001F12BC  38 00 00 01 */	li r0, 1
/* 801F4380 001F12C0  98 0D 8F 93 */	stb r0, __init__smList__26JALList$$015JALSeModPitDist$$1@sda21(r13)
lbl_801F4384:
/* 801F4384 001F12C4  88 0D 8F 94 */	lbz r0, __init__smList__26JALList$$015JALSeModVolDist$$1@sda21(r13)
/* 801F4388 001F12C8  7C 00 07 75 */	extsb. r0, r0
/* 801F438C 001F12CC  40 82 00 28 */	bne lbl_801F43B4
/* 801F4390 001F12D0  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1@sda21
/* 801F4394 001F12D4  4B E1 A8 A1 */	bl initiate__10JSUPtrListFv
/* 801F4398 001F12D8  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@ha
/* 801F439C 001F12DC  38 83 A5 4C */	addi r4, r3, __dt__26JSUList$$015JALSeModVolDist$$1Fv@l
/* 801F43A0 001F12E0  38 6D 8F 5C */	addi r3, r13, smList__26JALList$$015JALSeModVolDist$$1@sda21
/* 801F43A4 001F12E4  38 BF 00 84 */	addi r5, r31, 0x84
/* 801F43A8 001F12E8  4B E8 E3 81 */	bl __register_global_object
/* 801F43AC 001F12EC  38 00 00 01 */	li r0, 1
/* 801F43B0 001F12F0  98 0D 8F 94 */	stb r0, __init__smList__26JALList$$015JALSeModVolDist$$1@sda21(r13)
lbl_801F43B4:
/* 801F43B4 001F12F4  88 0D 8F 95 */	lbz r0, __init__smList__26JALList$$015JALSeModEffFunk$$1@sda21(r13)
/* 801F43B8 001F12F8  7C 00 07 75 */	extsb. r0, r0
/* 801F43BC 001F12FC  40 82 00 28 */	bne lbl_801F43E4
/* 801F43C0 001F1300  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1@sda21
/* 801F43C4 001F1304  4B E1 A8 71 */	bl initiate__10JSUPtrListFv
/* 801F43C8 001F1308  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@ha
/* 801F43CC 001F130C  38 83 A4 F4 */	addi r4, r3, __dt__26JSUList$$015JALSeModEffFunk$$1Fv@l
/* 801F43D0 001F1310  38 6D 8F 68 */	addi r3, r13, smList__26JALList$$015JALSeModEffFunk$$1@sda21
/* 801F43D4 001F1314  38 BF 00 90 */	addi r5, r31, 0x90
/* 801F43D8 001F1318  4B E8 E3 51 */	bl __register_global_object
/* 801F43DC 001F131C  38 00 00 01 */	li r0, 1
/* 801F43E0 001F1320  98 0D 8F 95 */	stb r0, __init__smList__26JALList$$015JALSeModEffFunk$$1@sda21(r13)
lbl_801F43E4:
/* 801F43E4 001F1324  88 0D 8F 96 */	lbz r0, __init__smList__26JALList$$015JALSeModPitFunk$$1@sda21(r13)
/* 801F43E8 001F1328  7C 00 07 75 */	extsb. r0, r0
/* 801F43EC 001F132C  40 82 00 28 */	bne lbl_801F4414
/* 801F43F0 001F1330  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1@sda21
/* 801F43F4 001F1334  4B E1 A8 41 */	bl initiate__10JSUPtrListFv
/* 801F43F8 001F1338  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@ha
/* 801F43FC 001F133C  38 83 A4 9C */	addi r4, r3, __dt__26JSUList$$015JALSeModPitFunk$$1Fv@l
/* 801F4400 001F1340  38 6D 8F 74 */	addi r3, r13, smList__26JALList$$015JALSeModPitFunk$$1@sda21
/* 801F4404 001F1344  38 BF 00 9C */	addi r5, r31, 0x9c
/* 801F4408 001F1348  4B E8 E3 21 */	bl __register_global_object
/* 801F440C 001F134C  38 00 00 01 */	li r0, 1
/* 801F4410 001F1350  98 0D 8F 96 */	stb r0, __init__smList__26JALList$$015JALSeModPitFunk$$1@sda21(r13)
lbl_801F4414:
/* 801F4414 001F1354  88 0D 8F 97 */	lbz r0, __init__smList__26JALList$$015JALSeModVolFunk$$1@sda21(r13)
/* 801F4418 001F1358  7C 00 07 75 */	extsb. r0, r0
/* 801F441C 001F135C  40 82 00 28 */	bne lbl_801F4444
/* 801F4420 001F1360  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1@sda21
/* 801F4424 001F1364  4B E1 A8 11 */	bl initiate__10JSUPtrListFv
/* 801F4428 001F1368  3C 60 80 05 */	lis r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@ha
/* 801F442C 001F136C  38 83 A4 44 */	addi r4, r3, __dt__26JSUList$$015JALSeModVolFunk$$1Fv@l
/* 801F4430 001F1370  38 6D 8F 80 */	addi r3, r13, smList__26JALList$$015JALSeModVolFunk$$1@sda21
/* 801F4434 001F1374  38 BF 00 A8 */	addi r5, r31, 0xa8
/* 801F4438 001F1378  4B E8 E2 F1 */	bl __register_global_object
/* 801F443C 001F137C  38 00 00 01 */	li r0, 1
/* 801F4440 001F1380  98 0D 8F 97 */	stb r0, __init__smList__26JALList$$015JALSeModVolFunk$$1@sda21(r13)
lbl_801F4444:
/* 801F4444 001F1384  80 01 00 14 */	lwz r0, 0x14(r1)
/* 801F4448 001F1388  83 E1 00 0C */	lwz r31, 0xc(r1)
/* 801F444C 001F138C  38 21 00 10 */	addi r1, r1, 0x10
/* 801F4450 001F1390  7C 08 03 A6 */	mtlr r0
/* 801F4454 001F1394  4E 80 00 20 */	blr 

.section .ctors, "wa"  # 0x8036FBA0 - 0x8036FF80
  .4byte __sinit_PollutionEvent_cpp

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__14TPollutionTest
__vt__14TPollutionTest:
  .4byte 0
  .4byte 0
  .4byte __dt__14TPollutionTestFv
  .4byte getType__Q26JDrama8TNameRefCFv
  .4byte load__Q26JDrama8TNameRefFR20JSUMemoryInputStream
  .4byte save__Q26JDrama8TNameRefFR21JSUMemoryOutputStream
  .4byte loadAfter__14TPollutionTestFv
  .4byte searchF__Q26JDrama8TNameRefFUsPCc
  .4byte perform__14TPollutionTestFUlPQ26JDrama9TGraphics
  .4byte 0

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
$$22727:
	.skip 0xC
$$22728:
	.skip 0xC
$$22729:
	.skip 0xC
$$22730:
	.skip 0xC
$$22731:
	.skip 0xC
$$22732:
	.skip 0xC
$$22733:
	.skip 0xC
$$22734:
	.skip 0xC
$$22735:
	.skip 0xC
$$22736:
	.skip 0xC
$$22737:
	.skip 0xC
$$22738:
	.skip 0xC
$$22739:
	.skip 0xC
$$22740:
	.skip 0xC
$$22741:
	.skip 0x10

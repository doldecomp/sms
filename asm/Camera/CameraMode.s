.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global isSlopeCameraMode__15CPolarSubCameraCFv
isSlopeCameraMode__15CPolarSubCameraCFv:
/* 80362F94 0035FED4  80 03 00 50 */	lwz r0, 0x50(r3)
/* 80362F98 0035FED8  38 60 00 00 */	li r3, 0
/* 80362F9C 0035FEDC  28 00 00 48 */	cmplwi r0, 0x48
/* 80362FA0 0035FEE0  4D 81 00 20 */	bgtlr 
/* 80362FA4 0035FEE4  3C 80 80 3E */	lis r4, $$21781@ha
/* 80362FA8 0035FEE8  38 84 4E 14 */	addi r4, r4, $$21781@l
/* 80362FAC 0035FEEC  54 00 10 3A */	slwi r0, r0, 2
/* 80362FB0 0035FEF0  7C 04 00 2E */	lwzx r0, r4, r0
/* 80362FB4 0035FEF4  7C 09 03 A6 */	mtctr r0
/* 80362FB8 0035FEF8  4E 80 04 20 */	bctr 
/* 80362FBC 0035FEFC  38 60 00 01 */	li r3, 1
/* 80362FC0 0035FF00  4E 80 00 20 */	blr 

.global isOverHipAttackSpecifyMode__15CPolarSubCameraCFi
isOverHipAttackSpecifyMode__15CPolarSubCameraCFi:
/* 80362FC4 0035FF04  7C 08 02 A6 */	mflr r0
/* 80362FC8 0035FF08  90 01 00 04 */	stw r0, 4(r1)
/* 80362FCC 0035FF0C  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 80362FD0 0035FF10  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 80362FD4 0035FF14  3B E0 00 01 */	li r31, 1
/* 80362FD8 0035FF18  93 C1 00 18 */	stw r30, 0x18(r1)
/* 80362FDC 0035FF1C  3B C0 00 00 */	li r30, 0
/* 80362FE0 0035FF20  93 A1 00 14 */	stw r29, 0x14(r1)
/* 80362FE4 0035FF24  3B A4 00 00 */	addi r29, r4, 0
/* 80362FE8 0035FF28  93 81 00 10 */	stw r28, 0x10(r1)
/* 80362FEC 0035FF2C  3B 83 00 00 */	addi r28, r3, 0
/* 80362FF0 0035FF30  48 00 00 B5 */	bl isFixCameraSpecifyMode__15CPolarSubCameraCFi
/* 80362FF4 0035FF34  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 80362FF8 0035FF38  40 82 00 1C */	bne lbl_80363014
/* 80362FFC 0035FF3C  38 7C 00 00 */	addi r3, r28, 0
/* 80363000 0035FF40  38 9D 00 00 */	addi r4, r29, 0
/* 80363004 0035FF44  48 00 00 6D */	bl isDefiniteCameraSpecifyMode__15CPolarSubCameraCFi
/* 80363008 0035FF48  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 8036300C 0035FF4C  40 82 00 08 */	bne lbl_80363014
/* 80363010 0035FF50  3B E0 00 00 */	li r31, 0
lbl_80363014:
/* 80363014 0035FF54  57 E0 06 3F */	clrlwi. r0, r31, 0x18
/* 80363018 0035FF58  41 82 00 0C */	beq lbl_80363024
/* 8036301C 0035FF5C  3B C0 00 01 */	li r30, 1
/* 80363020 0035FF60  48 00 00 2C */	b lbl_8036304C
lbl_80363024:
/* 80363024 0035FF64  38 1D FF F8 */	addi r0, r29, -8
/* 80363028 0035FF68  28 00 00 40 */	cmplwi r0, 0x40
/* 8036302C 0035FF6C  41 81 00 20 */	bgt lbl_8036304C
/* 80363030 0035FF70  3C 60 80 3E */	lis r3, $$21809@ha
/* 80363034 0035FF74  38 63 4F 38 */	addi r3, r3, $$21809@l
/* 80363038 0035FF78  54 00 10 3A */	slwi r0, r0, 2
/* 8036303C 0035FF7C  7C 03 00 2E */	lwzx r0, r3, r0
/* 80363040 0035FF80  7C 09 03 A6 */	mtctr r0
/* 80363044 0035FF84  4E 80 04 20 */	bctr 
/* 80363048 0035FF88  3B C0 00 01 */	li r30, 1
lbl_8036304C:
/* 8036304C 0035FF8C  80 01 00 24 */	lwz r0, 0x24(r1)
/* 80363050 0035FF90  7F C3 F3 78 */	mr r3, r30
/* 80363054 0035FF94  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 80363058 0035FF98  83 C1 00 18 */	lwz r30, 0x18(r1)
/* 8036305C 0035FF9C  7C 08 03 A6 */	mtlr r0
/* 80363060 0035FFA0  83 A1 00 14 */	lwz r29, 0x14(r1)
/* 80363064 0035FFA4  83 81 00 10 */	lwz r28, 0x10(r1)
/* 80363068 0035FFA8  38 21 00 20 */	addi r1, r1, 0x20
/* 8036306C 0035FFAC  4E 80 00 20 */	blr 

.global isDefiniteCameraSpecifyMode__15CPolarSubCameraCFi
isDefiniteCameraSpecifyMode__15CPolarSubCameraCFi:
/* 80363070 0035FFB0  2C 04 00 26 */	cmpwi r4, 0x26
/* 80363074 0035FFB4  38 60 00 00 */	li r3, 0
/* 80363078 0035FFB8  40 80 00 1C */	bge lbl_80363094
/* 8036307C 0035FFBC  2C 04 00 09 */	cmpwi r4, 9
/* 80363080 0035FFC0  41 82 00 1C */	beq lbl_8036309C
/* 80363084 0035FFC4  4D 80 00 20 */	bltlr 
/* 80363088 0035FFC8  2C 04 00 1E */	cmpwi r4, 0x1e
/* 8036308C 0035FFCC  40 80 00 10 */	bge lbl_8036309C
/* 80363090 0035FFD0  4E 80 00 20 */	blr 
lbl_80363094:
/* 80363094 0035FFD4  2C 04 00 3B */	cmpwi r4, 0x3b
/* 80363098 0035FFD8  4C 82 00 20 */	bnelr 
lbl_8036309C:
/* 8036309C 0035FFDC  38 60 00 01 */	li r3, 1
/* 803630A0 0035FFE0  4E 80 00 20 */	blr 

.global isFixCameraSpecifyMode__15CPolarSubCameraCFi
isFixCameraSpecifyMode__15CPolarSubCameraCFi:
/* 803630A4 0035FFE4  2C 04 00 3A */	cmpwi r4, 0x3a
/* 803630A8 0035FFE8  38 60 00 00 */	li r3, 0
/* 803630AC 0035FFEC  41 82 00 18 */	beq lbl_803630C4
/* 803630B0 0035FFF0  4C 80 00 20 */	bgelr 
/* 803630B4 0035FFF4  2C 04 00 1E */	cmpwi r4, 0x1e
/* 803630B8 0035FFF8  4C 80 00 20 */	bgelr 
/* 803630BC 0035FFFC  2C 04 00 16 */	cmpwi r4, 0x16
/* 803630C0 00360000  4D 80 00 20 */	bltlr 
lbl_803630C4:
/* 803630C4 00360004  38 60 00 01 */	li r3, 1
/* 803630C8 00360008  4E 80 00 20 */	blr 

.global isRailCameraSpecifyMode__15CPolarSubCameraCFi
isRailCameraSpecifyMode__15CPolarSubCameraCFi:
/* 803630CC 0036000C  38 60 00 00 */	li r3, 0
/* 803630D0 00360010  4E 80 00 20 */	blr 

.global isFollowCameraSpecifyMode__15CPolarSubCameraCFi
isFollowCameraSpecifyMode__15CPolarSubCameraCFi:
/* 803630D4 00360014  2C 04 00 37 */	cmpwi r4, 0x37
/* 803630D8 00360018  38 60 00 00 */	li r3, 0
/* 803630DC 0036001C  40 80 00 1C */	bge lbl_803630F8
/* 803630E0 00360020  2C 04 00 00 */	cmpwi r4, 0
/* 803630E4 00360024  41 82 00 24 */	beq lbl_80363108
/* 803630E8 00360028  4D 80 00 20 */	bltlr 
/* 803630EC 0036002C  2C 04 00 35 */	cmpwi r4, 0x35
/* 803630F0 00360030  40 80 00 18 */	bge lbl_80363108
/* 803630F4 00360034  4E 80 00 20 */	blr 
lbl_803630F8:
/* 803630F8 00360038  2C 04 00 47 */	cmpwi r4, 0x47
/* 803630FC 0036003C  4C 80 00 20 */	bgelr 
/* 80363100 00360040  2C 04 00 45 */	cmpwi r4, 0x45
/* 80363104 00360044  4D 80 00 20 */	bltlr 
lbl_80363108:
/* 80363108 00360048  38 60 00 01 */	li r3, 1
/* 8036310C 0036004C  4E 80 00 20 */	blr 

.global isTowerCameraSpecifyMode__15CPolarSubCameraCFi
isTowerCameraSpecifyMode__15CPolarSubCameraCFi:
/* 80363110 00360050  2C 04 00 37 */	cmpwi r4, 0x37
/* 80363114 00360054  38 60 00 00 */	li r3, 0
/* 80363118 00360058  41 82 00 24 */	beq lbl_8036313C
/* 8036311C 0036005C  40 80 00 18 */	bge lbl_80363134
/* 80363120 00360060  2C 04 00 2A */	cmpwi r4, 0x2a
/* 80363124 00360064  4C 80 00 20 */	bgelr 
/* 80363128 00360068  2C 04 00 27 */	cmpwi r4, 0x27
/* 8036312C 0036006C  40 80 00 10 */	bge lbl_8036313C
/* 80363130 00360070  4E 80 00 20 */	blr 
lbl_80363134:
/* 80363134 00360074  2C 04 00 41 */	cmpwi r4, 0x41
/* 80363138 00360078  4C 82 00 20 */	bnelr 
lbl_8036313C:
/* 8036313C 0036007C  38 60 00 01 */	li r3, 1
/* 80363140 00360080  4E 80 00 20 */	blr 

.global isNormalCameraCompletely__15CPolarSubCameraCFv
isNormalCameraCompletely__15CPolarSubCameraCFv:
/* 80363144 00360084  7C 08 02 A6 */	mflr r0
/* 80363148 00360088  90 01 00 04 */	stw r0, 4(r1)
/* 8036314C 0036008C  94 21 FF E8 */	stwu r1, -0x18(r1)
/* 80363150 00360090  93 E1 00 14 */	stw r31, 0x14(r1)
/* 80363154 00360094  3B E0 00 00 */	li r31, 0
/* 80363158 00360098  93 C1 00 10 */	stw r30, 0x10(r1)
/* 8036315C 0036009C  7C 7E 1B 78 */	mr r30, r3
/* 80363160 003600A0  80 83 00 50 */	lwz r4, 0x50(r3)
/* 80363164 003600A4  48 00 00 71 */	bl isNormalCameraSpecifyMode__15CPolarSubCameraCFi
/* 80363168 003600A8  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 8036316C 003600AC  41 82 00 4C */	beq lbl_803631B8
/* 80363170 003600B0  7F C3 F3 78 */	mr r3, r30
/* 80363174 003600B4  4B FF 21 11 */	bl isNowInbetween__15CPolarSubCameraCFv
/* 80363178 003600B8  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 8036317C 003600BC  41 82 00 38 */	beq lbl_803631B4
/* 80363180 003600C0  80 1E 00 54 */	lwz r0, 0x54(r30)
/* 80363184 003600C4  38 80 00 00 */	li r4, 0
/* 80363188 003600C8  28 00 00 48 */	cmplwi r0, 0x48
/* 8036318C 003600CC  41 81 00 20 */	bgt lbl_803631AC
/* 80363190 003600D0  3C 60 80 3E */	lis r3, $$21895@ha
/* 80363194 003600D4  38 63 50 3C */	addi r3, r3, $$21895@l
/* 80363198 003600D8  54 00 10 3A */	slwi r0, r0, 2
/* 8036319C 003600DC  7C 03 00 2E */	lwzx r0, r3, r0
/* 803631A0 003600E0  7C 09 03 A6 */	mtctr r0
/* 803631A4 003600E4  4E 80 04 20 */	bctr 
/* 803631A8 003600E8  38 80 00 01 */	li r4, 1
lbl_803631AC:
/* 803631AC 003600EC  54 80 06 3F */	clrlwi. r0, r4, 0x18
/* 803631B0 003600F0  41 82 00 08 */	beq lbl_803631B8
lbl_803631B4:
/* 803631B4 003600F4  3B E0 00 01 */	li r31, 1
lbl_803631B8:
/* 803631B8 003600F8  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 803631BC 003600FC  7F E3 FB 78 */	mr r3, r31
/* 803631C0 00360100  83 E1 00 14 */	lwz r31, 0x14(r1)
/* 803631C4 00360104  83 C1 00 10 */	lwz r30, 0x10(r1)
/* 803631C8 00360108  7C 08 03 A6 */	mtlr r0
/* 803631CC 0036010C  38 21 00 18 */	addi r1, r1, 0x18
/* 803631D0 00360110  4E 80 00 20 */	blr 

.global isNormalCameraSpecifyMode__15CPolarSubCameraCFi
isNormalCameraSpecifyMode__15CPolarSubCameraCFi:
/* 803631D4 00360114  28 04 00 48 */	cmplwi r4, 0x48
/* 803631D8 00360118  38 60 00 00 */	li r3, 0
/* 803631DC 0036011C  4D 81 00 20 */	bgtlr 
/* 803631E0 00360120  3C A0 80 3E */	lis r5, $$21939@ha
/* 803631E4 00360124  38 A5 51 60 */	addi r5, r5, $$21939@l
/* 803631E8 00360128  54 80 10 3A */	slwi r0, r4, 2
/* 803631EC 0036012C  7C 05 00 2E */	lwzx r0, r5, r0
/* 803631F0 00360130  7C 09 03 A6 */	mtctr r0
/* 803631F4 00360134  4E 80 04 20 */	bctr 
/* 803631F8 00360138  38 60 00 01 */	li r3, 1
/* 803631FC 0036013C  4E 80 00 20 */	blr 

.global isTalkCameraInbetween__15CPolarSubCameraCFv
isTalkCameraInbetween__15CPolarSubCameraCFv:
/* 80363200 00360140  7C 08 02 A6 */	mflr r0
/* 80363204 00360144  90 01 00 04 */	stw r0, 4(r1)
/* 80363208 00360148  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 8036320C 0036014C  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 80363210 00360150  3B E0 00 00 */	li r31, 0
/* 80363214 00360154  93 C1 00 18 */	stw r30, 0x18(r1)
/* 80363218 00360158  3B C3 00 00 */	addi r30, r3, 0
/* 8036321C 0036015C  4B FF 20 69 */	bl isNowInbetween__15CPolarSubCameraCFv
/* 80363220 00360160  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 80363224 00360164  41 82 00 B8 */	beq lbl_803632DC
/* 80363228 00360168  80 1E 00 50 */	lwz r0, 0x50(r30)
/* 8036322C 0036016C  38 60 00 00 */	li r3, 0
/* 80363230 00360170  2C 00 00 2D */	cmpwi r0, 0x2d
/* 80363234 00360174  41 82 00 40 */	beq lbl_80363274
/* 80363238 00360178  40 80 00 28 */	bge lbl_80363260
/* 8036323C 0036017C  2C 00 00 0B */	cmpwi r0, 0xb
/* 80363240 00360180  41 82 00 38 */	beq lbl_80363278
/* 80363244 00360184  40 80 00 10 */	bge lbl_80363254
/* 80363248 00360188  2C 00 00 0A */	cmpwi r0, 0xa
/* 8036324C 0036018C  40 80 00 28 */	bge lbl_80363274
/* 80363250 00360190  48 00 00 28 */	b lbl_80363278
lbl_80363254:
/* 80363254 00360194  2C 00 00 0D */	cmpwi r0, 0xd
/* 80363258 00360198  40 80 00 20 */	bge lbl_80363278
/* 8036325C 0036019C  48 00 00 18 */	b lbl_80363274
lbl_80363260:
/* 80363260 003601A0  2C 00 00 41 */	cmpwi r0, 0x41
/* 80363264 003601A4  40 80 00 14 */	bge lbl_80363278
/* 80363268 003601A8  2C 00 00 3F */	cmpwi r0, 0x3f
/* 8036326C 003601AC  40 80 00 08 */	bge lbl_80363274
/* 80363270 003601B0  48 00 00 08 */	b lbl_80363278
lbl_80363274:
/* 80363274 003601B4  38 60 00 01 */	li r3, 1
lbl_80363278:
/* 80363278 003601B8  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 8036327C 003601BC  40 82 00 5C */	bne lbl_803632D8
/* 80363280 003601C0  80 1E 00 54 */	lwz r0, 0x54(r30)
/* 80363284 003601C4  38 60 00 00 */	li r3, 0
/* 80363288 003601C8  2C 00 00 2D */	cmpwi r0, 0x2d
/* 8036328C 003601CC  41 82 00 40 */	beq lbl_803632CC
/* 80363290 003601D0  40 80 00 28 */	bge lbl_803632B8
/* 80363294 003601D4  2C 00 00 0B */	cmpwi r0, 0xb
/* 80363298 003601D8  41 82 00 38 */	beq lbl_803632D0
/* 8036329C 003601DC  40 80 00 10 */	bge lbl_803632AC
/* 803632A0 003601E0  2C 00 00 0A */	cmpwi r0, 0xa
/* 803632A4 003601E4  40 80 00 28 */	bge lbl_803632CC
/* 803632A8 003601E8  48 00 00 28 */	b lbl_803632D0
lbl_803632AC:
/* 803632AC 003601EC  2C 00 00 0D */	cmpwi r0, 0xd
/* 803632B0 003601F0  40 80 00 20 */	bge lbl_803632D0
/* 803632B4 003601F4  48 00 00 18 */	b lbl_803632CC
lbl_803632B8:
/* 803632B8 003601F8  2C 00 00 41 */	cmpwi r0, 0x41
/* 803632BC 003601FC  40 80 00 14 */	bge lbl_803632D0
/* 803632C0 00360200  2C 00 00 3F */	cmpwi r0, 0x3f
/* 803632C4 00360204  40 80 00 08 */	bge lbl_803632CC
/* 803632C8 00360208  48 00 00 08 */	b lbl_803632D0
lbl_803632CC:
/* 803632CC 0036020C  38 60 00 01 */	li r3, 1
lbl_803632D0:
/* 803632D0 00360210  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 803632D4 00360214  41 82 00 08 */	beq lbl_803632DC
lbl_803632D8:
/* 803632D8 00360218  3B E0 00 01 */	li r31, 1
lbl_803632DC:
/* 803632DC 0036021C  80 01 00 24 */	lwz r0, 0x24(r1)
/* 803632E0 00360220  7F E3 FB 78 */	mr r3, r31
/* 803632E4 00360224  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 803632E8 00360228  83 C1 00 18 */	lwz r30, 0x18(r1)
/* 803632EC 0036022C  7C 08 03 A6 */	mtlr r0
/* 803632F0 00360230  38 21 00 20 */	addi r1, r1, 0x20
/* 803632F4 00360234  4E 80 00 20 */	blr 

.global isTalkCameraSpecifyMode__15CPolarSubCameraCFi
isTalkCameraSpecifyMode__15CPolarSubCameraCFi:
/* 803632F8 00360238  2C 04 00 2D */	cmpwi r4, 0x2d
/* 803632FC 0036023C  38 60 00 00 */	li r3, 0
/* 80363300 00360240  41 82 00 3C */	beq lbl_8036333C
/* 80363304 00360244  40 80 00 28 */	bge lbl_8036332C
/* 80363308 00360248  2C 04 00 0B */	cmpwi r4, 0xb
/* 8036330C 0036024C  4D 82 00 20 */	beqlr 
/* 80363310 00360250  40 80 00 10 */	bge lbl_80363320
/* 80363314 00360254  2C 04 00 0A */	cmpwi r4, 0xa
/* 80363318 00360258  40 80 00 24 */	bge lbl_8036333C
/* 8036331C 0036025C  4E 80 00 20 */	blr 
lbl_80363320:
/* 80363320 00360260  2C 04 00 0D */	cmpwi r4, 0xd
/* 80363324 00360264  4C 80 00 20 */	bgelr 
/* 80363328 00360268  48 00 00 14 */	b lbl_8036333C
lbl_8036332C:
/* 8036332C 0036026C  2C 04 00 41 */	cmpwi r4, 0x41
/* 80363330 00360270  4C 80 00 20 */	bgelr 
/* 80363334 00360274  2C 04 00 3F */	cmpwi r4, 0x3f
/* 80363338 00360278  4D 80 00 20 */	bltlr 
lbl_8036333C:
/* 8036333C 0036027C  38 60 00 01 */	li r3, 1
/* 80363340 00360280  4E 80 00 20 */	blr 

.global isJetCoaster1stCamera__15CPolarSubCameraCFv
isJetCoaster1stCamera__15CPolarSubCameraCFv:
/* 80363344 00360284  80 03 00 50 */	lwz r0, 0x50(r3)
/* 80363348 00360288  38 80 00 00 */	li r4, 0
/* 8036334C 0036028C  2C 00 00 2E */	cmpwi r0, 0x2e
/* 80363350 00360290  40 82 00 20 */	bne lbl_80363370
/* 80363354 00360294  80 63 02 B8 */	lwz r3, 0x2b8(r3)
/* 80363358 00360298  28 03 00 00 */	cmplwi r3, 0
/* 8036335C 0036029C  41 82 00 14 */	beq lbl_80363370
/* 80363360 003602A0  88 03 00 0C */	lbz r0, 0xc(r3)
/* 80363364 003602A4  54 00 07 FF */	clrlwi. r0, r0, 0x1f
/* 80363368 003602A8  41 82 00 08 */	beq lbl_80363370
/* 8036336C 003602AC  38 80 00 01 */	li r4, 1
lbl_80363370:
/* 80363370 003602B0  7C 83 23 78 */	mr r3, r4
/* 80363374 003602B4  4E 80 00 20 */	blr 

.global isLButtonCameraInbetween__15CPolarSubCameraCFv
isLButtonCameraInbetween__15CPolarSubCameraCFv:
/* 80363378 003602B8  7C 08 02 A6 */	mflr r0
/* 8036337C 003602BC  90 01 00 04 */	stw r0, 4(r1)
/* 80363380 003602C0  94 21 FF E0 */	stwu r1, -0x20(r1)
/* 80363384 003602C4  93 E1 00 1C */	stw r31, 0x1c(r1)
/* 80363388 003602C8  3B E0 00 00 */	li r31, 0
/* 8036338C 003602CC  93 C1 00 18 */	stw r30, 0x18(r1)
/* 80363390 003602D0  3B C3 00 00 */	addi r30, r3, 0
/* 80363394 003602D4  4B FF 1E F1 */	bl isNowInbetween__15CPolarSubCameraCFv
/* 80363398 003602D8  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 8036339C 003602DC  41 82 00 48 */	beq lbl_803633E4
/* 803633A0 003602E0  80 1E 00 50 */	lwz r0, 0x50(r30)
/* 803633A4 003602E4  38 60 00 00 */	li r3, 0
/* 803633A8 003602E8  2C 00 00 07 */	cmpwi r0, 7
/* 803633AC 003602EC  41 82 00 08 */	beq lbl_803633B4
/* 803633B0 003602F0  48 00 00 08 */	b lbl_803633B8
lbl_803633B4:
/* 803633B4 003602F4  38 60 00 01 */	li r3, 1
lbl_803633B8:
/* 803633B8 003602F8  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 803633BC 003602FC  40 82 00 24 */	bne lbl_803633E0
/* 803633C0 00360300  80 1E 00 54 */	lwz r0, 0x54(r30)
/* 803633C4 00360304  38 60 00 00 */	li r3, 0
/* 803633C8 00360308  2C 00 00 07 */	cmpwi r0, 7
/* 803633CC 0036030C  41 82 00 08 */	beq lbl_803633D4
/* 803633D0 00360310  48 00 00 08 */	b lbl_803633D8
lbl_803633D4:
/* 803633D4 00360314  38 60 00 01 */	li r3, 1
lbl_803633D8:
/* 803633D8 00360318  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 803633DC 0036031C  41 82 00 08 */	beq lbl_803633E4
lbl_803633E0:
/* 803633E0 00360320  3B E0 00 01 */	li r31, 1
lbl_803633E4:
/* 803633E4 00360324  80 01 00 24 */	lwz r0, 0x24(r1)
/* 803633E8 00360328  7F E3 FB 78 */	mr r3, r31
/* 803633EC 0036032C  83 E1 00 1C */	lwz r31, 0x1c(r1)
/* 803633F0 00360330  83 C1 00 18 */	lwz r30, 0x18(r1)
/* 803633F4 00360334  7C 08 03 A6 */	mtlr r0
/* 803633F8 00360338  38 21 00 20 */	addi r1, r1, 0x20
/* 803633FC 0036033C  4E 80 00 20 */	blr 

.global isLButtonCameraSpecifyMode__15CPolarSubCameraCFi
isLButtonCameraSpecifyMode__15CPolarSubCameraCFi:
/* 80363400 00360340  2C 04 00 07 */	cmpwi r4, 7
/* 80363404 00360344  38 60 00 00 */	li r3, 0
/* 80363408 00360348  4C 82 00 20 */	bnelr 
/* 8036340C 0036034C  38 60 00 01 */	li r3, 1
/* 80363410 00360350  4E 80 00 20 */	blr 

.section .rodata, "wa"  # 0x8036FFA0 - 0x803A8380
$$21490:
	.incbin "baserom.dol", 0x3A3D20, 0xC
$$21526:
	.incbin "baserom.dol", 0x3A3D2C, 0x14
$$21535:
	.incbin "baserom.dol", 0x3A3D40, 0x18
$$21536:
	.incbin "baserom.dol", 0x3A3D58, 0x1C
$$21537:
	.incbin "baserom.dol", 0x3A3D74, 0x1C
$$21538:
	.incbin "baserom.dol", 0x3A3D90, 0x1C
$$21539:
	.incbin "baserom.dol", 0x3A3DAC, 0x1C
$$21540:
	.incbin "baserom.dol", 0x3A3DC8, 0x1C
$$21541:
	.incbin "baserom.dol", 0x3A3DE4, 0x18
$$21542:
	.incbin "baserom.dol", 0x3A3DFC, 0x18
$$21543:
	.incbin "baserom.dol", 0x3A3E14, 0x20
$$21544:
	.incbin "baserom.dol", 0x3A3E34, 0x1C
$$21545:
	.incbin "baserom.dol", 0x3A3E50, 0x18
$$21546:
	.incbin "baserom.dol", 0x3A3E68, 0x1C
$$21547:
	.incbin "baserom.dol", 0x3A3E84, 0x18
$$21548:
	.incbin "baserom.dol", 0x3A3E9C, 0x1C
$$21549:
	.incbin "baserom.dol", 0x3A3EB8, 0x18
$$21550:
	.incbin "baserom.dol", 0x3A3ED0, 0x18
$$21551:
	.incbin "baserom.dol", 0x3A3EE8, 0x1C
$$21552:
	.incbin "baserom.dol", 0x3A3F04, 0x1C
$$21553:
	.incbin "baserom.dol", 0x3A3F20, 0x1C
$$21554:
	.incbin "baserom.dol", 0x3A3F3C, 0x1C
$$21555:
	.incbin "baserom.dol", 0x3A3F58, 0x18
$$21556:
	.incbin "baserom.dol", 0x3A3F70, 0x18
$$21557:
	.incbin "baserom.dol", 0x3A3F88, 0x18
$$21558:
	.incbin "baserom.dol", 0x3A3FA0, 0x18
$$21559:
	.incbin "baserom.dol", 0x3A3FB8, 0x18
$$21560:
	.incbin "baserom.dol", 0x3A3FD0, 0x18
$$21561:
	.incbin "baserom.dol", 0x3A3FE8, 0x18
$$21562:
	.incbin "baserom.dol", 0x3A4000, 0x18
$$21563:
	.incbin "baserom.dol", 0x3A4018, 0x18
$$21564:
	.incbin "baserom.dol", 0x3A4030, 0x18
$$21565:
	.incbin "baserom.dol", 0x3A4048, 0x1C
$$21566:
	.incbin "baserom.dol", 0x3A4064, 0x1C
$$21567:
	.incbin "baserom.dol", 0x3A4080, 0x1C
$$21568:
	.incbin "baserom.dol", 0x3A409C, 0x1C
$$21569:
	.incbin "baserom.dol", 0x3A40B8, 0x1C
$$21570:
	.incbin "baserom.dol", 0x3A40D4, 0x1C
$$21571:
	.incbin "baserom.dol", 0x3A40F0, 0x1C
$$21572:
	.incbin "baserom.dol", 0x3A410C, 0x1C
$$21573:
	.incbin "baserom.dol", 0x3A4128, 0x18
$$21574:
	.incbin "baserom.dol", 0x3A4140, 0x18
$$21575:
	.incbin "baserom.dol", 0x3A4158, 0x18
$$21576:
	.incbin "baserom.dol", 0x3A4170, 0x18
$$21577:
	.incbin "baserom.dol", 0x3A4188, 0x18
$$21578:
	.incbin "baserom.dol", 0x3A41A0, 0x18
$$21579:
	.incbin "baserom.dol", 0x3A41B8, 0x18
$$21580:
	.incbin "baserom.dol", 0x3A41D0, 0x18
$$21581:
	.incbin "baserom.dol", 0x3A41E8, 0x1C
$$21582:
	.incbin "baserom.dol", 0x3A4204, 0x1C
$$21583:
	.incbin "baserom.dol", 0x3A4220, 0x18
$$21584:
	.incbin "baserom.dol", 0x3A4238, 0x1C
$$21585:
	.incbin "baserom.dol", 0x3A4254, 0x1C
$$21586:
	.incbin "baserom.dol", 0x3A4270, 0x1C
$$21587:
	.incbin "baserom.dol", 0x3A428C, 0x1C
$$21588:
	.incbin "baserom.dol", 0x3A42A8, 0x18
$$21589:
	.incbin "baserom.dol", 0x3A42C0, 0x18
$$21590:
	.incbin "baserom.dol", 0x3A42D8, 0x18
$$21591:
	.incbin "baserom.dol", 0x3A42F0, 0x20
$$21592:
	.incbin "baserom.dol", 0x3A4310, 0x1C
$$21593:
	.incbin "baserom.dol", 0x3A432C, 0x18
$$21594:
	.incbin "baserom.dol", 0x3A4344, 0x1C
$$21595:
	.incbin "baserom.dol", 0x3A4360, 0x18
$$21596:
	.incbin "baserom.dol", 0x3A4378, 0x1C
$$21597:
	.incbin "baserom.dol", 0x3A4394, 0x1C
$$21598:
	.incbin "baserom.dol", 0x3A43B0, 0x18
$$21599:
	.incbin "baserom.dol", 0x3A43C8, 0x18
$$21600:
	.incbin "baserom.dol", 0x3A43E0, 0x18
$$21601:
	.incbin "baserom.dol", 0x3A43F8, 0x1C
$$21602:
	.incbin "baserom.dol", 0x3A4414, 0x18
$$21603:
	.incbin "baserom.dol", 0x3A442C, 0x20
$$21604:
	.incbin "baserom.dol", 0x3A444C, 0x18
$$21605:
	.incbin "baserom.dol", 0x3A4464, 0x18
$$21606:
	.incbin "baserom.dol", 0x3A447C, 0x1C
$$21607:
	.incbin "baserom.dol", 0x3A4498, 0x20

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global mCamKindNameSaveFile__15CPolarSubCamera
mCamKindNameSaveFile__15CPolarSubCamera:
	.incbin "baserom.dol", 0x3E1CF0, 0x124
$$21781:
	.incbin "baserom.dol", 0x3E1E14, 0x124
$$21809:
	.incbin "baserom.dol", 0x3E1F38, 0x104
$$21895:
	.incbin "baserom.dol", 0x3E203C, 0x124
$$21939:
	.incbin "baserom.dol", 0x3E2160, 0x128

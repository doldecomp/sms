.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0
.global addMoveCameraAndMario__15CPolarSubCameraFRC3Vec
addMoveCameraAndMario__15CPolarSubCameraFRC3Vec:
/* 80362988 0035F8C8  7C 08 02 A6 */	mflr r0
/* 8036298C 0035F8CC  90 01 00 04 */	stw r0, 4(r1)
/* 80362990 0035F8D0  94 21 FF 30 */	stwu r1, -0xd0(r1)
/* 80362994 0035F8D4  93 E1 00 CC */	stw r31, 0xcc(r1)
/* 80362998 0035F8D8  93 C1 00 C8 */	stw r30, 0xc8(r1)
/* 8036299C 0035F8DC  3B C4 00 00 */	addi r30, r4, 0
/* 803629A0 0035F8E0  93 A1 00 C4 */	stw r29, 0xc4(r1)
/* 803629A4 0035F8E4  3B A3 00 00 */	addi r29, r3, 0
/* 803629A8 0035F8E8  C0 23 00 10 */	lfs f1, 0x10(r3)
/* 803629AC 0035F8EC  38 61 00 38 */	addi r3, r1, 0x38
/* 803629B0 0035F8F0  C0 04 00 00 */	lfs f0, 0(r4)
/* 803629B4 0035F8F4  C0 5E 00 04 */	lfs f2, 4(r30)
/* 803629B8 0035F8F8  EC 01 00 2A */	fadds f0, f1, f0
/* 803629BC 0035F8FC  C0 3E 00 08 */	lfs f1, 8(r30)
/* 803629C0 0035F900  D0 1D 00 10 */	stfs f0, 0x10(r29)
/* 803629C4 0035F904  C0 1D 00 14 */	lfs f0, 0x14(r29)
/* 803629C8 0035F908  EC 00 10 2A */	fadds f0, f0, f2
/* 803629CC 0035F90C  D0 1D 00 14 */	stfs f0, 0x14(r29)
/* 803629D0 0035F910  C0 1D 00 18 */	lfs f0, 0x18(r29)
/* 803629D4 0035F914  EC 00 08 2A */	fadds f0, f0, f1
/* 803629D8 0035F918  D0 1D 00 18 */	stfs f0, 0x18(r29)
/* 803629DC 0035F91C  C0 3D 00 3C */	lfs f1, 0x3c(r29)
/* 803629E0 0035F920  C0 1E 00 00 */	lfs f0, 0(r30)
/* 803629E4 0035F924  C0 5E 00 04 */	lfs f2, 4(r30)
/* 803629E8 0035F928  EC 01 00 2A */	fadds f0, f1, f0
/* 803629EC 0035F92C  C0 3E 00 08 */	lfs f1, 8(r30)
/* 803629F0 0035F930  D0 1D 00 3C */	stfs f0, 0x3c(r29)
/* 803629F4 0035F934  C0 1D 00 40 */	lfs f0, 0x40(r29)
/* 803629F8 0035F938  EC 00 10 2A */	fadds f0, f0, f2
/* 803629FC 0035F93C  D0 1D 00 40 */	stfs f0, 0x40(r29)
/* 80362A00 0035F940  C0 1D 00 44 */	lfs f0, 0x44(r29)
/* 80362A04 0035F944  EC 00 08 2A */	fadds f0, f0, f1
/* 80362A08 0035F948  D0 1D 00 44 */	stfs f0, 0x44(r29)
/* 80362A0C 0035F94C  C0 3D 01 24 */	lfs f1, 0x124(r29)
/* 80362A10 0035F950  C0 1E 00 00 */	lfs f0, 0(r30)
/* 80362A14 0035F954  C0 5E 00 04 */	lfs f2, 4(r30)
/* 80362A18 0035F958  EC 01 00 2A */	fadds f0, f1, f0
/* 80362A1C 0035F95C  C0 3E 00 08 */	lfs f1, 8(r30)
/* 80362A20 0035F960  D0 1D 01 24 */	stfs f0, 0x124(r29)
/* 80362A24 0035F964  C0 1D 01 28 */	lfs f0, 0x128(r29)
/* 80362A28 0035F968  EC 00 10 2A */	fadds f0, f0, f2
/* 80362A2C 0035F96C  D0 1D 01 28 */	stfs f0, 0x128(r29)
/* 80362A30 0035F970  C0 1D 01 2C */	lfs f0, 0x12c(r29)
/* 80362A34 0035F974  EC 00 08 2A */	fadds f0, f0, f1
/* 80362A38 0035F978  D0 1D 01 2C */	stfs f0, 0x12c(r29)
/* 80362A3C 0035F97C  C0 3D 01 48 */	lfs f1, 0x148(r29)
/* 80362A40 0035F980  C0 1E 00 00 */	lfs f0, 0(r30)
/* 80362A44 0035F984  C0 5E 00 04 */	lfs f2, 4(r30)
/* 80362A48 0035F988  EC 01 00 2A */	fadds f0, f1, f0
/* 80362A4C 0035F98C  C0 3E 00 08 */	lfs f1, 8(r30)
/* 80362A50 0035F990  D0 1D 01 48 */	stfs f0, 0x148(r29)
/* 80362A54 0035F994  C0 1D 01 4C */	lfs f0, 0x14c(r29)
/* 80362A58 0035F998  EC 00 10 2A */	fadds f0, f0, f2
/* 80362A5C 0035F99C  D0 1D 01 4C */	stfs f0, 0x14c(r29)
/* 80362A60 0035F9A0  C0 1D 01 50 */	lfs f0, 0x150(r29)
/* 80362A64 0035F9A4  EC 00 08 2A */	fadds f0, f0, f1
/* 80362A68 0035F9A8  D0 1D 01 50 */	stfs f0, 0x150(r29)
/* 80362A6C 0035F9AC  83 ED A8 B8 */	lwz r31, gpCameraMario-_SDA_BASE_(r13)
/* 80362A70 0035F9B0  4B D7 AA 9D */	bl set__Q29JGeometry8TVec3$$0f$$1FRC3Vec
/* 80362A74 0035F9B4  C0 3F 00 00 */	lfs f1, 0(r31)
/* 80362A78 0035F9B8  7F C4 F3 78 */	mr r4, r30
/* 80362A7C 0035F9BC  C0 01 00 38 */	lfs f0, 0x38(r1)
/* 80362A80 0035F9C0  EC 01 00 2A */	fadds f0, f1, f0
/* 80362A84 0035F9C4  D0 1F 00 00 */	stfs f0, 0(r31)
/* 80362A88 0035F9C8  C0 3F 00 04 */	lfs f1, 4(r31)
/* 80362A8C 0035F9CC  C0 01 00 3C */	lfs f0, 0x3c(r1)
/* 80362A90 0035F9D0  EC 01 00 2A */	fadds f0, f1, f0
/* 80362A94 0035F9D4  D0 1F 00 04 */	stfs f0, 4(r31)
/* 80362A98 0035F9D8  C0 3F 00 08 */	lfs f1, 8(r31)
/* 80362A9C 0035F9DC  C0 01 00 40 */	lfs f0, 0x40(r1)
/* 80362AA0 0035F9E0  EC 01 00 2A */	fadds f0, f1, f0
/* 80362AA4 0035F9E4  D0 1F 00 08 */	stfs f0, 8(r31)
/* 80362AA8 0035F9E8  80 7D 00 6C */	lwz r3, 0x6c(r29)
/* 80362AAC 0035F9EC  4B FF 3E 1D */	bl addMoveCameraAndMario__16TCameraInbetweenFRC3Vec
/* 80362AB0 0035F9F0  C0 3D 00 80 */	lfs f1, 0x80(r29)
/* 80362AB4 0035F9F4  C0 1E 00 00 */	lfs f0, 0(r30)
/* 80362AB8 0035F9F8  C0 5E 00 04 */	lfs f2, 4(r30)
/* 80362ABC 0035F9FC  EC 01 00 2A */	fadds f0, f1, f0
/* 80362AC0 0035FA00  C0 3E 00 08 */	lfs f1, 8(r30)
/* 80362AC4 0035FA04  D0 1D 00 80 */	stfs f0, 0x80(r29)
/* 80362AC8 0035FA08  C0 1D 00 84 */	lfs f0, 0x84(r29)
/* 80362ACC 0035FA0C  EC 00 10 2A */	fadds f0, f0, f2
/* 80362AD0 0035FA10  D0 1D 00 84 */	stfs f0, 0x84(r29)
/* 80362AD4 0035FA14  C0 1D 00 88 */	lfs f0, 0x88(r29)
/* 80362AD8 0035FA18  EC 00 08 2A */	fadds f0, f0, f1
/* 80362ADC 0035FA1C  D0 1D 00 88 */	stfs f0, 0x88(r29)
/* 80362AE0 0035FA20  C0 3D 00 8C */	lfs f1, 0x8c(r29)
/* 80362AE4 0035FA24  C0 1E 00 00 */	lfs f0, 0(r30)
/* 80362AE8 0035FA28  C0 5E 00 04 */	lfs f2, 4(r30)
/* 80362AEC 0035FA2C  EC 01 00 2A */	fadds f0, f1, f0
/* 80362AF0 0035FA30  C0 3E 00 08 */	lfs f1, 8(r30)
/* 80362AF4 0035FA34  D0 1D 00 8C */	stfs f0, 0x8c(r29)
/* 80362AF8 0035FA38  C0 1D 00 90 */	lfs f0, 0x90(r29)
/* 80362AFC 0035FA3C  EC 00 10 2A */	fadds f0, f0, f2
/* 80362B00 0035FA40  D0 1D 00 90 */	stfs f0, 0x90(r29)
/* 80362B04 0035FA44  C0 1D 00 94 */	lfs f0, 0x94(r29)
/* 80362B08 0035FA48  EC 00 08 2A */	fadds f0, f0, f1
/* 80362B0C 0035FA4C  D0 1D 00 94 */	stfs f0, 0x94(r29)
/* 80362B10 0035FA50  C0 3D 00 98 */	lfs f1, 0x98(r29)
/* 80362B14 0035FA54  C0 1E 00 00 */	lfs f0, 0(r30)
/* 80362B18 0035FA58  C0 5E 00 04 */	lfs f2, 4(r30)
/* 80362B1C 0035FA5C  EC 01 00 2A */	fadds f0, f1, f0
/* 80362B20 0035FA60  C0 3E 00 08 */	lfs f1, 8(r30)
/* 80362B24 0035FA64  D0 1D 00 98 */	stfs f0, 0x98(r29)
/* 80362B28 0035FA68  C0 1D 00 9C */	lfs f0, 0x9c(r29)
/* 80362B2C 0035FA6C  EC 00 10 2A */	fadds f0, f0, f2
/* 80362B30 0035FA70  D0 1D 00 9C */	stfs f0, 0x9c(r29)
/* 80362B34 0035FA74  C0 1D 00 A0 */	lfs f0, 0xa0(r29)
/* 80362B38 0035FA78  EC 00 08 2A */	fadds f0, f0, f1
/* 80362B3C 0035FA7C  D0 1D 00 A0 */	stfs f0, 0xa0(r29)
/* 80362B40 0035FA80  C0 3D 00 B4 */	lfs f1, 0xb4(r29)
/* 80362B44 0035FA84  C0 1E 00 00 */	lfs f0, 0(r30)
/* 80362B48 0035FA88  C0 5E 00 04 */	lfs f2, 4(r30)
/* 80362B4C 0035FA8C  EC 01 00 2A */	fadds f0, f1, f0
/* 80362B50 0035FA90  C0 3E 00 08 */	lfs f1, 8(r30)
/* 80362B54 0035FA94  D0 1D 00 B4 */	stfs f0, 0xb4(r29)
/* 80362B58 0035FA98  C0 1D 00 B8 */	lfs f0, 0xb8(r29)
/* 80362B5C 0035FA9C  EC 00 10 2A */	fadds f0, f0, f2
/* 80362B60 0035FAA0  D0 1D 00 B8 */	stfs f0, 0xb8(r29)
/* 80362B64 0035FAA4  C0 1D 00 BC */	lfs f0, 0xbc(r29)
/* 80362B68 0035FAA8  EC 00 08 2A */	fadds f0, f0, f1
/* 80362B6C 0035FAAC  D0 1D 00 BC */	stfs f0, 0xbc(r29)
/* 80362B70 0035FAB0  C0 3D 00 C0 */	lfs f1, 0xc0(r29)
/* 80362B74 0035FAB4  C0 1E 00 00 */	lfs f0, 0(r30)
/* 80362B78 0035FAB8  C0 5E 00 04 */	lfs f2, 4(r30)
/* 80362B7C 0035FABC  EC 01 00 2A */	fadds f0, f1, f0
/* 80362B80 0035FAC0  C0 3E 00 08 */	lfs f1, 8(r30)
/* 80362B84 0035FAC4  D0 1D 00 C0 */	stfs f0, 0xc0(r29)
/* 80362B88 0035FAC8  C0 1D 00 C4 */	lfs f0, 0xc4(r29)
/* 80362B8C 0035FACC  EC 00 10 2A */	fadds f0, f0, f2
/* 80362B90 0035FAD0  D0 1D 00 C4 */	stfs f0, 0xc4(r29)
/* 80362B94 0035FAD4  C0 1D 00 C8 */	lfs f0, 0xc8(r29)
/* 80362B98 0035FAD8  EC 00 08 2A */	fadds f0, f0, f1
/* 80362B9C 0035FADC  D0 1D 00 C8 */	stfs f0, 0xc8(r29)
/* 80362BA0 0035FAE0  C0 3D 00 CC */	lfs f1, 0xcc(r29)
/* 80362BA4 0035FAE4  C0 1E 00 00 */	lfs f0, 0(r30)
/* 80362BA8 0035FAE8  C0 5E 00 04 */	lfs f2, 4(r30)
/* 80362BAC 0035FAEC  EC 01 00 2A */	fadds f0, f1, f0
/* 80362BB0 0035FAF0  C0 3E 00 08 */	lfs f1, 8(r30)
/* 80362BB4 0035FAF4  D0 1D 00 CC */	stfs f0, 0xcc(r29)
/* 80362BB8 0035FAF8  C0 1D 00 D0 */	lfs f0, 0xd0(r29)
/* 80362BBC 0035FAFC  EC 00 10 2A */	fadds f0, f0, f2
/* 80362BC0 0035FB00  D0 1D 00 D0 */	stfs f0, 0xd0(r29)
/* 80362BC4 0035FB04  C0 1D 00 D4 */	lfs f0, 0xd4(r29)
/* 80362BC8 0035FB08  EC 00 08 2A */	fadds f0, f0, f1
/* 80362BCC 0035FB0C  D0 1D 00 D4 */	stfs f0, 0xd4(r29)
/* 80362BD0 0035FB10  80 01 00 D4 */	lwz r0, 0xd4(r1)
/* 80362BD4 0035FB14  83 E1 00 CC */	lwz r31, 0xcc(r1)
/* 80362BD8 0035FB18  83 C1 00 C8 */	lwz r30, 0xc8(r1)
/* 80362BDC 0035FB1C  83 A1 00 C4 */	lwz r29, 0xc4(r1)
/* 80362BE0 0035FB20  38 21 00 D0 */	addi r1, r1, 0xd0
/* 80362BE4 0035FB24  7C 08 03 A6 */	mtlr r0
/* 80362BE8 0035FB28  4E 80 00 20 */	blr 

.global warpPosAndAt__15CPolarSubCameraFfs
warpPosAndAt__15CPolarSubCameraFfs:
/* 80362BEC 0035FB2C  7C 08 02 A6 */	mflr r0
/* 80362BF0 0035FB30  90 01 00 04 */	stw r0, 4(r1)
/* 80362BF4 0035FB34  94 21 FF A8 */	stwu r1, -0x58(r1)
/* 80362BF8 0035FB38  DB E1 00 50 */	stfd f31, 0x50(r1)
/* 80362BFC 0035FB3C  FF E0 08 90 */	fmr f31, f1
/* 80362C00 0035FB40  93 E1 00 4C */	stw r31, 0x4c(r1)
/* 80362C04 0035FB44  7C 7F 1B 78 */	mr r31, r3
/* 80362C08 0035FB48  93 C1 00 48 */	stw r30, 0x48(r1)
/* 80362C0C 0035FB4C  3B C4 00 00 */	addi r30, r4, 0
/* 80362C10 0035FB50  80 03 00 50 */	lwz r0, 0x50(r3)
/* 80362C14 0035FB54  2C 00 00 49 */	cmpwi r0, 0x49
/* 80362C18 0035FB58  40 80 01 FC */	bge lbl_80362E14
/* 80362C1C 0035FB5C  54 00 10 3A */	slwi r0, r0, 2
/* 80362C20 0035FB60  80 7F 00 68 */	lwz r3, 0x68(r31)
/* 80362C24 0035FB64  7C 9F 02 14 */	add r4, r31, r0
/* 80362C28 0035FB68  80 84 02 D8 */	lwz r4, 0x2d8(r4)
/* 80362C2C 0035FB6C  4B FF 43 75 */	bl copySaveParam__16TCameraKindParamFRC17TCamSaveKindParam
/* 80362C30 0035FB70  38 61 00 20 */	addi r3, r1, 0x20
/* 80362C34 0035FB74  38 9F 00 00 */	addi r4, r31, 0
/* 80362C38 0035FB78  4B FF 25 C9 */	bl getUsualLookat__15CPolarSubCameraCFv
/* 80362C3C 0035FB7C  C0 21 00 20 */	lfs f1, 0x20(r1)
/* 80362C40 0035FB80  7F E3 FB 78 */	mr r3, r31
/* 80362C44 0035FB84  C0 01 00 24 */	lfs f0, 0x24(r1)
/* 80362C48 0035FB88  D0 21 00 2C */	stfs f1, 0x2c(r1)
/* 80362C4C 0035FB8C  D0 01 00 30 */	stfs f0, 0x30(r1)
/* 80362C50 0035FB90  C0 01 00 28 */	lfs f0, 0x28(r1)
/* 80362C54 0035FB94  D0 01 00 34 */	stfs f0, 0x34(r1)
/* 80362C58 0035FB98  80 9F 00 50 */	lwz r4, 0x50(r31)
/* 80362C5C 0035FB9C  48 00 07 A5 */	bl isLButtonCameraSpecifyMode__15CPolarSubCameraCFi
/* 80362C60 0035FBA0  54 60 06 3F */	clrlwi. r0, r3, 0x18
/* 80362C64 0035FBA4  41 82 00 34 */	beq lbl_80362C98
/* 80362C68 0035FBA8  C0 02 08 D0 */	lfs f0, $$21767-_SDA2_BASE_(r2)
/* 80362C6C 0035FBAC  FC 20 F8 90 */	fmr f1, f31
/* 80362C70 0035FBB0  FC 1F 00 40 */	fcmpo cr0, f31, f0
/* 80362C74 0035FBB4  40 81 00 0C */	ble lbl_80362C80
/* 80362C78 0035FBB8  FC 20 00 90 */	fmr f1, f0
/* 80362C7C 0035FBBC  48 00 00 14 */	b lbl_80362C90
lbl_80362C80:
/* 80362C80 0035FBC0  C0 02 08 D4 */	lfs f0, $$21768-_SDA2_BASE_(r2)
/* 80362C84 0035FBC4  FC 1F 00 40 */	fcmpo cr0, f31, f0
/* 80362C88 0035FBC8  40 80 00 08 */	bge lbl_80362C90
/* 80362C8C 0035FBCC  FC 20 00 90 */	fmr f1, f0
lbl_80362C90:
/* 80362C90 0035FBD0  D0 3F 00 A8 */	stfs f1, 0xa8(r31)
/* 80362C94 0035FBD4  48 00 00 30 */	b lbl_80362CC4
lbl_80362C98:
/* 80362C98 0035FBD8  C0 5F 02 6C */	lfs f2, 0x26c(r31)
/* 80362C9C 0035FBDC  FC 00 F8 90 */	fmr f0, f31
/* 80362CA0 0035FBE0  C0 3F 02 68 */	lfs f1, 0x268(r31)
/* 80362CA4 0035FBE4  FC 1F 10 40 */	fcmpo cr0, f31, f2
/* 80362CA8 0035FBE8  40 81 00 0C */	ble lbl_80362CB4
/* 80362CAC 0035FBEC  FC 00 10 90 */	fmr f0, f2
/* 80362CB0 0035FBF0  48 00 00 10 */	b lbl_80362CC0
lbl_80362CB4:
/* 80362CB4 0035FBF4  FC 1F 08 40 */	fcmpo cr0, f31, f1
/* 80362CB8 0035FBF8  40 80 00 08 */	bge lbl_80362CC0
/* 80362CBC 0035FBFC  FC 00 08 90 */	fmr f0, f1
lbl_80362CC0:
/* 80362CC0 0035FC00  D0 1F 00 A8 */	stfs f0, 0xa8(r31)
lbl_80362CC4:
/* 80362CC4 0035FC04  7F E3 FB 78 */	mr r3, r31
/* 80362CC8 0035FC08  4B FF 25 09 */	bl calcAngleXFromXRotRatio___15CPolarSubCameraCFv
/* 80362CCC 0035FC0C  B0 7F 00 A4 */	sth r3, 0xa4(r31)
/* 80362CD0 0035FC10  7F E3 FB 78 */	mr r3, r31
/* 80362CD4 0035FC14  B3 DF 00 A6 */	sth r30, 0xa6(r31)
/* 80362CD8 0035FC18  4B FF 24 C9 */	bl calcDistFromXRotRatio___15CPolarSubCameraCFv
/* 80362CDC 0035FC1C  A8 BF 00 A4 */	lha r5, 0xa4(r31)
/* 80362CE0 0035FC20  38 61 00 2C */	addi r3, r1, 0x2c
/* 80362CE4 0035FC24  A8 DF 00 A6 */	lha r6, 0xa6(r31)
/* 80362CE8 0035FC28  38 81 00 38 */	addi r4, r1, 0x38
/* 80362CEC 0035FC2C  4B FF 58 61 */	bl CLBPolarToCross__FRC3VecP3Vecfss
/* 80362CF0 0035FC30  80 1F 00 50 */	lwz r0, 0x50(r31)
/* 80362CF4 0035FC34  2C 00 00 49 */	cmpwi r0, 0x49
/* 80362CF8 0035FC38  40 80 01 1C */	bge lbl_80362E14
/* 80362CFC 0035FC3C  54 00 10 3A */	slwi r0, r0, 2
/* 80362D00 0035FC40  80 7F 00 68 */	lwz r3, 0x68(r31)
/* 80362D04 0035FC44  7C 9F 02 14 */	add r4, r31, r0
/* 80362D08 0035FC48  80 84 02 D8 */	lwz r4, 0x2d8(r4)
/* 80362D0C 0035FC4C  4B FF 42 95 */	bl copySaveParam__16TCameraKindParamFRC17TCamSaveKindParam
/* 80362D10 0035FC50  7F E3 FB 78 */	mr r3, r31
/* 80362D14 0035FC54  4B FF 33 19 */	bl killHeightPan___15CPolarSubCameraFv
/* 80362D18 0035FC58  C0 01 00 38 */	lfs f0, 0x38(r1)
/* 80362D1C 0035FC5C  38 81 00 38 */	addi r4, r1, 0x38
/* 80362D20 0035FC60  38 A1 00 2C */	addi r5, r1, 0x2c
/* 80362D24 0035FC64  D0 1F 00 10 */	stfs f0, 0x10(r31)
/* 80362D28 0035FC68  C0 01 00 3C */	lfs f0, 0x3c(r1)
/* 80362D2C 0035FC6C  D0 1F 00 14 */	stfs f0, 0x14(r31)
/* 80362D30 0035FC70  C0 01 00 40 */	lfs f0, 0x40(r1)
/* 80362D34 0035FC74  D0 1F 00 18 */	stfs f0, 0x18(r31)
/* 80362D38 0035FC78  C0 01 00 2C */	lfs f0, 0x2c(r1)
/* 80362D3C 0035FC7C  D0 1F 00 3C */	stfs f0, 0x3c(r31)
/* 80362D40 0035FC80  C0 01 00 30 */	lfs f0, 0x30(r1)
/* 80362D44 0035FC84  D0 1F 00 40 */	stfs f0, 0x40(r31)
/* 80362D48 0035FC88  C0 01 00 34 */	lfs f0, 0x34(r1)
/* 80362D4C 0035FC8C  D0 1F 00 44 */	stfs f0, 0x44(r31)
/* 80362D50 0035FC90  C0 01 00 38 */	lfs f0, 0x38(r1)
/* 80362D54 0035FC94  D0 1F 01 24 */	stfs f0, 0x124(r31)
/* 80362D58 0035FC98  C0 01 00 3C */	lfs f0, 0x3c(r1)
/* 80362D5C 0035FC9C  D0 1F 01 28 */	stfs f0, 0x128(r31)
/* 80362D60 0035FCA0  C0 01 00 40 */	lfs f0, 0x40(r1)
/* 80362D64 0035FCA4  D0 1F 01 2C */	stfs f0, 0x12c(r31)
/* 80362D68 0035FCA8  C0 01 00 2C */	lfs f0, 0x2c(r1)
/* 80362D6C 0035FCAC  D0 1F 01 48 */	stfs f0, 0x148(r31)
/* 80362D70 0035FCB0  C0 01 00 30 */	lfs f0, 0x30(r1)
/* 80362D74 0035FCB4  D0 1F 01 4C */	stfs f0, 0x14c(r31)
/* 80362D78 0035FCB8  C0 01 00 34 */	lfs f0, 0x34(r1)
/* 80362D7C 0035FCBC  D0 1F 01 50 */	stfs f0, 0x150(r31)
/* 80362D80 0035FCC0  80 7F 00 6C */	lwz r3, 0x6c(r31)
/* 80362D84 0035FCC4  4B FF 3C 2D */	bl warpPosAndAt__16TCameraInbetweenFRC3VecRC3Vec
/* 80362D88 0035FCC8  80 9F 00 6C */	lwz r4, 0x6c(r31)
/* 80362D8C 0035FCCC  38 00 00 00 */	li r0, 0
/* 80362D90 0035FCD0  38 7F 00 00 */	addi r3, r31, 0
/* 80362D94 0035FCD4  90 04 00 04 */	stw r0, 4(r4)
/* 80362D98 0035FCD8  38 81 00 38 */	addi r4, r1, 0x38
/* 80362D9C 0035FCDC  38 A1 00 2C */	addi r5, r1, 0x2c
/* 80362DA0 0035FCE0  4B FF 22 BD */	bl calcNowTargetFromPosAndAt___15CPolarSubCameraFRC3VecRC3Vec
/* 80362DA4 0035FCE4  80 7F 00 80 */	lwz r3, 0x80(r31)
/* 80362DA8 0035FCE8  80 1F 00 84 */	lwz r0, 0x84(r31)
/* 80362DAC 0035FCEC  90 7F 00 B4 */	stw r3, 0xb4(r31)
/* 80362DB0 0035FCF0  90 1F 00 B8 */	stw r0, 0xb8(r31)
/* 80362DB4 0035FCF4  80 1F 00 88 */	lwz r0, 0x88(r31)
/* 80362DB8 0035FCF8  90 1F 00 BC */	stw r0, 0xbc(r31)
/* 80362DBC 0035FCFC  80 7F 00 8C */	lwz r3, 0x8c(r31)
/* 80362DC0 0035FD00  80 1F 00 90 */	lwz r0, 0x90(r31)
/* 80362DC4 0035FD04  90 7F 00 C0 */	stw r3, 0xc0(r31)
/* 80362DC8 0035FD08  90 1F 00 C4 */	stw r0, 0xc4(r31)
/* 80362DCC 0035FD0C  80 1F 00 94 */	lwz r0, 0x94(r31)
/* 80362DD0 0035FD10  90 1F 00 C8 */	stw r0, 0xc8(r31)
/* 80362DD4 0035FD14  80 7F 00 98 */	lwz r3, 0x98(r31)
/* 80362DD8 0035FD18  80 1F 00 9C */	lwz r0, 0x9c(r31)
/* 80362DDC 0035FD1C  90 7F 00 CC */	stw r3, 0xcc(r31)
/* 80362DE0 0035FD20  90 1F 00 D0 */	stw r0, 0xd0(r31)
/* 80362DE4 0035FD24  80 1F 00 A0 */	lwz r0, 0xa0(r31)
/* 80362DE8 0035FD28  90 1F 00 D4 */	stw r0, 0xd4(r31)
/* 80362DEC 0035FD2C  A8 1F 00 A4 */	lha r0, 0xa4(r31)
/* 80362DF0 0035FD30  B0 1F 00 D8 */	sth r0, 0xd8(r31)
/* 80362DF4 0035FD34  A8 1F 00 A6 */	lha r0, 0xa6(r31)
/* 80362DF8 0035FD38  B0 1F 00 DA */	sth r0, 0xda(r31)
/* 80362DFC 0035FD3C  C0 1F 00 A8 */	lfs f0, 0xa8(r31)
/* 80362E00 0035FD40  D0 1F 00 DC */	stfs f0, 0xdc(r31)
/* 80362E04 0035FD44  A8 1F 00 AC */	lha r0, 0xac(r31)
/* 80362E08 0035FD48  B0 1F 00 E0 */	sth r0, 0xe0(r31)
/* 80362E0C 0035FD4C  C0 1F 00 B0 */	lfs f0, 0xb0(r31)
/* 80362E10 0035FD50  D0 1F 00 E4 */	stfs f0, 0xe4(r31)
lbl_80362E14:
/* 80362E14 0035FD54  80 01 00 5C */	lwz r0, 0x5c(r1)
/* 80362E18 0035FD58  CB E1 00 50 */	lfd f31, 0x50(r1)
/* 80362E1C 0035FD5C  83 E1 00 4C */	lwz r31, 0x4c(r1)
/* 80362E20 0035FD60  7C 08 03 A6 */	mtlr r0
/* 80362E24 0035FD64  83 C1 00 48 */	lwz r30, 0x48(r1)
/* 80362E28 0035FD68  38 21 00 58 */	addi r1, r1, 0x58
/* 80362E2C 0035FD6C  4E 80 00 20 */	blr 

.global warpPosAndAt__15CPolarSubCameraFRC3VecRC3Vec
warpPosAndAt__15CPolarSubCameraFRC3VecRC3Vec:
/* 80362E30 0035FD70  7C 08 02 A6 */	mflr r0
/* 80362E34 0035FD74  90 01 00 04 */	stw r0, 4(r1)
/* 80362E38 0035FD78  94 21 FF D8 */	stwu r1, -0x28(r1)
/* 80362E3C 0035FD7C  93 E1 00 24 */	stw r31, 0x24(r1)
/* 80362E40 0035FD80  7C 7F 1B 78 */	mr r31, r3
/* 80362E44 0035FD84  93 C1 00 20 */	stw r30, 0x20(r1)
/* 80362E48 0035FD88  3B C5 00 00 */	addi r30, r5, 0
/* 80362E4C 0035FD8C  93 A1 00 1C */	stw r29, 0x1c(r1)
/* 80362E50 0035FD90  3B A4 00 00 */	addi r29, r4, 0
/* 80362E54 0035FD94  80 03 00 50 */	lwz r0, 0x50(r3)
/* 80362E58 0035FD98  2C 00 00 49 */	cmpwi r0, 0x49
/* 80362E5C 0035FD9C  40 80 01 1C */	bge lbl_80362F78
/* 80362E60 0035FDA0  54 00 10 3A */	slwi r0, r0, 2
/* 80362E64 0035FDA4  80 7F 00 68 */	lwz r3, 0x68(r31)
/* 80362E68 0035FDA8  7C 9F 02 14 */	add r4, r31, r0
/* 80362E6C 0035FDAC  80 84 02 D8 */	lwz r4, 0x2d8(r4)
/* 80362E70 0035FDB0  4B FF 41 31 */	bl copySaveParam__16TCameraKindParamFRC17TCamSaveKindParam
/* 80362E74 0035FDB4  7F E3 FB 78 */	mr r3, r31
/* 80362E78 0035FDB8  4B FF 31 B5 */	bl killHeightPan___15CPolarSubCameraFv
/* 80362E7C 0035FDBC  C0 1D 00 00 */	lfs f0, 0(r29)
/* 80362E80 0035FDC0  38 9D 00 00 */	addi r4, r29, 0
/* 80362E84 0035FDC4  38 BE 00 00 */	addi r5, r30, 0
/* 80362E88 0035FDC8  D0 1F 00 10 */	stfs f0, 0x10(r31)
/* 80362E8C 0035FDCC  C0 1D 00 04 */	lfs f0, 4(r29)
/* 80362E90 0035FDD0  D0 1F 00 14 */	stfs f0, 0x14(r31)
/* 80362E94 0035FDD4  C0 1D 00 08 */	lfs f0, 8(r29)
/* 80362E98 0035FDD8  D0 1F 00 18 */	stfs f0, 0x18(r31)
/* 80362E9C 0035FDDC  C0 1E 00 00 */	lfs f0, 0(r30)
/* 80362EA0 0035FDE0  D0 1F 00 3C */	stfs f0, 0x3c(r31)
/* 80362EA4 0035FDE4  C0 1E 00 04 */	lfs f0, 4(r30)
/* 80362EA8 0035FDE8  D0 1F 00 40 */	stfs f0, 0x40(r31)
/* 80362EAC 0035FDEC  C0 1E 00 08 */	lfs f0, 8(r30)
/* 80362EB0 0035FDF0  D0 1F 00 44 */	stfs f0, 0x44(r31)
/* 80362EB4 0035FDF4  C0 1D 00 00 */	lfs f0, 0(r29)
/* 80362EB8 0035FDF8  D0 1F 01 24 */	stfs f0, 0x124(r31)
/* 80362EBC 0035FDFC  C0 1D 00 04 */	lfs f0, 4(r29)
/* 80362EC0 0035FE00  D0 1F 01 28 */	stfs f0, 0x128(r31)
/* 80362EC4 0035FE04  C0 1D 00 08 */	lfs f0, 8(r29)
/* 80362EC8 0035FE08  D0 1F 01 2C */	stfs f0, 0x12c(r31)
/* 80362ECC 0035FE0C  C0 1E 00 00 */	lfs f0, 0(r30)
/* 80362ED0 0035FE10  D0 1F 01 48 */	stfs f0, 0x148(r31)
/* 80362ED4 0035FE14  C0 1E 00 04 */	lfs f0, 4(r30)
/* 80362ED8 0035FE18  D0 1F 01 4C */	stfs f0, 0x14c(r31)
/* 80362EDC 0035FE1C  C0 1E 00 08 */	lfs f0, 8(r30)
/* 80362EE0 0035FE20  D0 1F 01 50 */	stfs f0, 0x150(r31)
/* 80362EE4 0035FE24  80 7F 00 6C */	lwz r3, 0x6c(r31)
/* 80362EE8 0035FE28  4B FF 3A C9 */	bl warpPosAndAt__16TCameraInbetweenFRC3VecRC3Vec
/* 80362EEC 0035FE2C  80 9F 00 6C */	lwz r4, 0x6c(r31)
/* 80362EF0 0035FE30  38 00 00 00 */	li r0, 0
/* 80362EF4 0035FE34  38 7F 00 00 */	addi r3, r31, 0
/* 80362EF8 0035FE38  90 04 00 04 */	stw r0, 4(r4)
/* 80362EFC 0035FE3C  38 9D 00 00 */	addi r4, r29, 0
/* 80362F00 0035FE40  38 BE 00 00 */	addi r5, r30, 0
/* 80362F04 0035FE44  4B FF 21 59 */	bl calcNowTargetFromPosAndAt___15CPolarSubCameraFRC3VecRC3Vec
/* 80362F08 0035FE48  80 7F 00 80 */	lwz r3, 0x80(r31)
/* 80362F0C 0035FE4C  80 1F 00 84 */	lwz r0, 0x84(r31)
/* 80362F10 0035FE50  90 7F 00 B4 */	stw r3, 0xb4(r31)
/* 80362F14 0035FE54  90 1F 00 B8 */	stw r0, 0xb8(r31)
/* 80362F18 0035FE58  80 1F 00 88 */	lwz r0, 0x88(r31)
/* 80362F1C 0035FE5C  90 1F 00 BC */	stw r0, 0xbc(r31)
/* 80362F20 0035FE60  80 7F 00 8C */	lwz r3, 0x8c(r31)
/* 80362F24 0035FE64  80 1F 00 90 */	lwz r0, 0x90(r31)
/* 80362F28 0035FE68  90 7F 00 C0 */	stw r3, 0xc0(r31)
/* 80362F2C 0035FE6C  90 1F 00 C4 */	stw r0, 0xc4(r31)
/* 80362F30 0035FE70  80 1F 00 94 */	lwz r0, 0x94(r31)
/* 80362F34 0035FE74  90 1F 00 C8 */	stw r0, 0xc8(r31)
/* 80362F38 0035FE78  80 7F 00 98 */	lwz r3, 0x98(r31)
/* 80362F3C 0035FE7C  80 1F 00 9C */	lwz r0, 0x9c(r31)
/* 80362F40 0035FE80  90 7F 00 CC */	stw r3, 0xcc(r31)
/* 80362F44 0035FE84  90 1F 00 D0 */	stw r0, 0xd0(r31)
/* 80362F48 0035FE88  80 1F 00 A0 */	lwz r0, 0xa0(r31)
/* 80362F4C 0035FE8C  90 1F 00 D4 */	stw r0, 0xd4(r31)
/* 80362F50 0035FE90  A8 1F 00 A4 */	lha r0, 0xa4(r31)
/* 80362F54 0035FE94  B0 1F 00 D8 */	sth r0, 0xd8(r31)
/* 80362F58 0035FE98  A8 1F 00 A6 */	lha r0, 0xa6(r31)
/* 80362F5C 0035FE9C  B0 1F 00 DA */	sth r0, 0xda(r31)
/* 80362F60 0035FEA0  C0 1F 00 A8 */	lfs f0, 0xa8(r31)
/* 80362F64 0035FEA4  D0 1F 00 DC */	stfs f0, 0xdc(r31)
/* 80362F68 0035FEA8  A8 1F 00 AC */	lha r0, 0xac(r31)
/* 80362F6C 0035FEAC  B0 1F 00 E0 */	sth r0, 0xe0(r31)
/* 80362F70 0035FEB0  C0 1F 00 B0 */	lfs f0, 0xb0(r31)
/* 80362F74 0035FEB4  D0 1F 00 E4 */	stfs f0, 0xe4(r31)
lbl_80362F78:
/* 80362F78 0035FEB8  80 01 00 2C */	lwz r0, 0x2c(r1)
/* 80362F7C 0035FEBC  83 E1 00 24 */	lwz r31, 0x24(r1)
/* 80362F80 0035FEC0  83 C1 00 20 */	lwz r30, 0x20(r1)
/* 80362F84 0035FEC4  7C 08 03 A6 */	mtlr r0
/* 80362F88 0035FEC8  83 A1 00 1C */	lwz r29, 0x1c(r1)
/* 80362F8C 0035FECC  38 21 00 28 */	addi r1, r1, 0x28
/* 80362F90 0035FED0  4E 80 00 20 */	blr 

.section .sdata2, "wa"  # 0x8040B460 - 0x80414020
$$21767:
	.incbin "baserom.dol", 0x3EC5D0, 0x4
$$21768:
	.incbin "baserom.dol", 0x3EC5D4, 0x4
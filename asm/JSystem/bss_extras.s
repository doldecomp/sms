.include "macros.inc"

.section .bss, "wa"  # 0x803E6000 - 0x80408AC0
.global sAramList__11JKRAramHeap
sAramList__11JKRAramHeap:
	.skip 0xC
.global sAramCommandList__7JKRAram
sAramCommandList__7JKRAram:
	.skip 0xC
.global sDvdList__10JKRDvdFile
sDvdList__10JKRDvdFile:
	.skip 0xC
.global sVolumeList__13JKRFileLoader
sVolumeList__13JKRFileLoader:
	.skip 0xC
.global sThreadList__9JKRThread
sThreadList__9JKRThread:
	.skip 0xC
.global sMapFileList__12JUTException
sMapFileList__12JUTException:
	.skip 0xC
.global mPadList__10JUTGamePad
mPadList__10JUTGamePad:
	.skip 0x17C
.global mPadStatus__10JUTGamePad
mPadStatus__10JUTGamePad:
	.skip 0x30
.global mPadButton__10JUTGamePad
mPadButton__10JUTGamePad:
	.skip 0xC0
.global mPadMStick__10JUTGamePad
mPadMStick__10JUTGamePad:
	.skip 0x40
.global mPadSStick__10JUTGamePad
mPadSStick__10JUTGamePad:
	.skip 0x40
.global mMutex__8J2DPrint
mMutex__8J2DPrint:
	.skip 0x18
.global j3dSys
j3dSys:
	.skip 0x1AC
.global mCurrentMtx__6J3DSys
mCurrentMtx__6J3DSys:
	.skip 0x30
.global mCurrentS__6J3DSys
mCurrentS__6J3DSys:
	.skip 0xC
.global mParentS__6J3DSys
mParentS__6J3DSys:
	.skip 0xC
.global sTexCoordScaleTable__6J3DSys
sTexCoordScaleTable__6J3DSys:
	.skip 0x40
.global sDvdAsyncList__12JKRDvdRipper
sDvdAsyncList__12JKRDvdRipper:
	.skip 0xC
.global sDvdAramAsyncList__16JKRDvdAramRipper
sDvdAramAsyncList__16JKRDvdAramRipper:
	.skip 0xC
.global j3dTexCoordTable
j3dTexCoordTable:
	.skip 0x2528
.global j3dTevSwapTableTable
j3dTevSwapTableTable:
	.skip 0x400
.global j3dAlphaCmpTable
j3dAlphaCmpTable:
	.skip 0x300
.global j3dZModeTable
j3dZModeTable:
	.skip 0x60
.global sAramPieceCommandList__12JKRAramPiece
sAramPieceCommandList__12JKRAramPiece:
	.skip 0x24
.global mMutex__12JKRAramPiece
mMutex__12JKRAramPiece:
	.skip 0x18
.global JAInullCamera
JAInullCamera:
	.skip 0xC
.global nullActor__8JAIConst
nullActor__8JAIConst:
	.skip 0x58
.global camMtx__8JAIConst
camMtx__8JAIConst:
	.skip 0x30
.global camTrans__8JAIConst
camTrans__8JAIConst:
	.skip 0xC
.global camPreTrans__8JAIConst
camPreTrans__8JAIConst:
	.skip 0xC
.global streamHeap__Q27JAInter9StreamLib
streamHeap__Q27JAInter9StreamLib:
	.skip 0x14
.global systemPortCmd__18JAISystemInterface
systemPortCmd__18JAISystemInterface:
	.skip 0x18
.global jac_audioThread__Q28JASystem11AudioThread
jac_audioThread__Q28JASystem11AudioThread:
	.skip 0x2640
.global jac_audioStack__Q28JASystem11AudioThread
jac_audioStack__Q28JASystem11AudioThread:
	.skip 0x1000
.global jac_dvdThread__Q28JASystem11AudioThread
jac_dvdThread__Q28JASystem11AudioThread:
	.skip 0x320
.global jac_dvdStack__Q28JASystem11AudioThread
jac_dvdStack__Q28JASystem11AudioThread:
	.skip 0x1000
.global sParser__Q28JASystem6TTrack
sParser__Q28JASystem6TTrack:
	.skip 0x4
.global vc__7JPADraw
vc__7JPADraw:
	.skip 0x1EC
.global cb__7JPADraw
cb__7JPADraw:
	.skip 0xB0

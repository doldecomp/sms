.include "macros.inc"

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __vt__7JKRFile
__vt__7JKRFile:
	.incbin "baserom.dol", 0x3A5380, 0x20
.global __vt__14JKRAramArchive
__vt__14JKRAramArchive:
	.incbin "baserom.dol", 0x3A53A0, 0x48
.global __vt__11JKRAramHeap
__vt__11JKRAramHeap:
	.incbin "baserom.dol", 0x3A53E8, 0x10
.global sMessageBuffer__7JKRAram
sMessageBuffer__7JKRAram:
	.incbin "baserom.dol", 0x3A53F8, 0x10
.global sMessageQueue__7JKRAram
sMessageQueue__7JKRAram:
	.incbin "baserom.dol", 0x3A5408, 0x20
.global __vt__7JKRAram
__vt__7JKRAram:
	.incbin "baserom.dol", 0x3A5428, 0x10
.global __vt__10JKRArchive
__vt__10JKRArchive:
	.incbin "baserom.dol", 0x3A5438, 0x48
.global __vt__14JKRCompArchive
__vt__14JKRCompArchive:
	.incbin "baserom.dol", 0x3A5480, 0x48
.global __vt__11JKRDisposer
__vt__11JKRDisposer:
	.incbin "baserom.dol", 0x3A54C8, 0x10
.global __vt__13JKRDvdArchive
__vt__13JKRDvdArchive:
	.incbin "baserom.dol", 0x3A54D8, 0x48
.global __vt__10JKRDvdFile
__vt__10JKRDvdFile:
	.incbin "baserom.dol", 0x3A5520, 0x28
.global __vt__10JKRExpHeap
__vt__10JKRExpHeap:
	.incbin "baserom.dol", 0x3A5548, 0x50
.global __vt__13JKRFileFinder
__vt__13JKRFileFinder:
	.incbin "baserom.dol", 0x3A5598, 0x10
.global __vt__12JKRArcFinder
__vt__12JKRArcFinder:
	.incbin "baserom.dol", 0x3A55A8, 0x10
.global __vt__13JKRFileLoader
__vt__13JKRFileLoader:
	.incbin "baserom.dol", 0x3A55B8, 0x40
.global __vt__7JKRHeap
__vt__7JKRHeap:
	.incbin "baserom.dol", 0x3A55F8, 0x50
.global __vt__13JKRMemArchive
__vt__13JKRMemArchive:
	.incbin "baserom.dol", 0x3A5648, 0x48
.global __vt__12JKRSolidHeap
__vt__12JKRSolidHeap:
	.incbin "baserom.dol", 0x3A5690, 0x50
.global __vt__10JKRStdHeap
__vt__10JKRStdHeap:
	.incbin "baserom.dol", 0x3A56E0, 0x50
.global __vt__9JKRThread
__vt__9JKRThread:
	.incbin "baserom.dol", 0x3A5730, 0x10
.global __vt__21JSURandomOutputStream
__vt__21JSURandomOutputStream:
	.incbin "baserom.dol", 0x3A5740, 0x28
.global __vt__10JSUIosBase
__vt__10JSUIosBase:
	.incbin "baserom.dol", 0x3A5768, 0xC
.global __vt__15JSUOutputStream
__vt__15JSUOutputStream:
	.incbin "baserom.dol", 0x3A5774, 0x14
.global __vt__20JSURandomInputStream
__vt__20JSURandomInputStream:
	.incbin "baserom.dol", 0x3A5788, 0x24
.global __vt__14JSUInputStream
__vt__14JSUInputStream:
	.incbin "baserom.dol", 0x3A57AC, 0x1C
.global __vt__21JSUMemoryOutputStream
__vt__21JSUMemoryOutputStream:
	.incbin "baserom.dol", 0x3A57C8, 0x28
.global __vt__20JSUMemoryInputStream
__vt__20JSUMemoryInputStream:
	.incbin "baserom.dol", 0x3A57F0, 0x28
.global __vt__18JSUFileInputStream
__vt__18JSUFileInputStream:
	.incbin "baserom.dol", 0x3A5818, 0x28
.global sMessageQueue__12JUTException
sMessageQueue__12JUTException:
	.incbin "baserom.dol", 0x3A5840, 0x20
.global sCpuExpName__12JUTException
sCpuExpName__12JUTException:
	.incbin "baserom.dol", 0x3A5860, 0x40
.global __vt__12JUTException
__vt__12JUTException:
	.incbin "baserom.dol", 0x3A58A0, 0x10
.global sAsciiTable__14JUTDirectPrint
sAsciiTable__14JUTDirectPrint:
	.incbin "baserom.dol", 0x3A58B0, 0x80
.global sFontData__14JUTDirectPrint
sFontData__14JUTDirectPrint:
	.incbin "baserom.dol", 0x3A5930, 0x100
.global sFontData2__14JUTDirectPrint
sFontData2__14JUTDirectPrint:
	.incbin "baserom.dol", 0x3A5A30, 0x138
.global __vt__7JUTFont
__vt__7JUTFont:
	.incbin "baserom.dol", 0x3A5B68, 0x38
.global channel_mask
channel_mask:
	.incbin "baserom.dol", 0x3A5BA0, 0x10
.global __vt__10JUTGamePad
__vt__10JUTGamePad:
	.incbin "baserom.dol", 0x3A5BB0, 0x10
.global __vt__7J2DPane
__vt__7J2DPane:
	.incbin "baserom.dol", 0x3A5BC0, 0x30
.global __vt__10J2DPicture
__vt__10J2DPicture:
	.incbin "baserom.dol", 0x3A5BF0, 0x30
.global $$2426
$$2426:
	.incbin "baserom.dol", 0x3A5C20, 0x60
.global __vt__8J2DPrint
__vt__8J2DPrint:
	.incbin "baserom.dol", 0x3A5C80, 0x10
.global $$2323
$$2323:
	.incbin "baserom.dol", 0x3A5C90, 0x50
.global __vt__12J2DSetScreen
__vt__12J2DSetScreen:
	.incbin "baserom.dol", 0x3A5CE0, 0x34
.global __vt__9J2DScreen
__vt__9J2DScreen:
	.incbin "baserom.dol", 0x3A5D14, 0x34
.global __vt__10J2DTextBox
__vt__10J2DTextBox:
	.incbin "baserom.dol", 0x3A5D48, 0x30
.global __vt__9J2DWindow
__vt__9J2DWindow:
	.incbin "baserom.dol", 0x3A5D78, 0x2C
.global __vt__Q29J2DWindow7Texture
__vt__Q29J2DWindow7Texture:
	.incbin "baserom.dol", 0x3A5DA4, 0xC
.global j3dDefaultTransformInfo
j3dDefaultTransformInfo:
	.incbin "baserom.dol", 0x3A5DB0, 0x20
.global j3dDefaultMtx
j3dDefaultMtx:
	.incbin "baserom.dol", 0x3A5DD0, 0x30
.global PSMulUnit01
PSMulUnit01:
	.incbin "baserom.dol", 0x3A5E00, 0x8
.global __vt__13J3DSkinDeform
__vt__13J3DSkinDeform:
	.incbin "baserom.dol", 0x3A5E08, 0x10
.global __vt__8J3DJoint
__vt__8J3DJoint:
	.incbin "baserom.dol", 0x3A5E18, 0x28
.global __vt__17J3DMtxCalcMayaAnm
__vt__17J3DMtxCalcMayaAnm:
	.incbin "baserom.dol", 0x3A5E40, 0x54
.global __vt__22J3DMtxCalcSoftimageAnm
__vt__22J3DMtxCalcSoftimageAnm:
	.incbin "baserom.dol", 0x3A5E94, 0x54
.global __vt__18J3DMtxCalcBasicAnm
__vt__18J3DMtxCalcBasicAnm:
	.incbin "baserom.dol", 0x3A5EE8, 0x54
.global __vt__14J3DMtxCalcMaya
__vt__14J3DMtxCalcMaya:
	.incbin "baserom.dol", 0x3A5F3C, 0x44
.global __vt__19J3DMtxCalcSoftimage
__vt__19J3DMtxCalcSoftimage:
	.incbin "baserom.dol", 0x3A5F80, 0x44
.global __vt__15J3DMtxCalcBasic
__vt__15J3DMtxCalcBasic:
	.incbin "baserom.dol", 0x3A5FC4, 0x44
.global __vt__10J3DMtxCalc
__vt__10J3DMtxCalc:
	.incbin "baserom.dol", 0x3A6008, 0x20
.global __vt__13J3DMtxCalcAnm
__vt__13J3DMtxCalcAnm:
	.incbin "baserom.dol", 0x3A6028, 0x30
.global __vt__14J3DPEBlockFull
__vt__14J3DPEBlockFull:
	.incbin "baserom.dol", 0x3A6058, 0x64
.global __vt__15J3DIndBlockFull
__vt__15J3DIndBlockFull:
	.incbin "baserom.dol", 0x3A60BC, 0x48
.global __vt__13J3DTevBlock16
__vt__13J3DTevBlock16:
	.incbin "baserom.dol", 0x3A6104, 0x9C
.global __vt__12J3DTevBlock4
__vt__12J3DTevBlock4:
	.incbin "baserom.dol", 0x3A61A0, 0x9C
.global __vt__12J3DTevBlock2
__vt__12J3DTevBlock2:
	.incbin "baserom.dol", 0x3A623C, 0x9C
.global __vt__12J3DTevBlock1
__vt__12J3DTevBlock1:
	.incbin "baserom.dol", 0x3A62D8, 0x9C
.global __vt__19J3DTexGenBlockBasic
__vt__19J3DTexGenBlockBasic:
	.incbin "baserom.dol", 0x3A6374, 0x50
.global __vt__20J3DColorBlockLightOn
__vt__20J3DColorBlockLightOn:
	.incbin "baserom.dol", 0x3A63C4, 0x64
.global __vt__21J3DColorBlockLightOff
__vt__21J3DColorBlockLightOff:
	.incbin "baserom.dol", 0x3A6428, 0x64
.global __vt__13J3DPEBlockXlu
__vt__13J3DPEBlockXlu:
	.incbin "baserom.dol", 0x3A648C, 0x64
.global __vt__17J3DPEBlockTexEdge
__vt__17J3DPEBlockTexEdge:
	.incbin "baserom.dol", 0x3A64F0, 0x64
.global __vt__13J3DPEBlockOpa
__vt__13J3DPEBlockOpa:
	.incbin "baserom.dol", 0x3A6554, 0x64
.global __vt__10J3DPEBlock
__vt__10J3DPEBlock:
	.incbin "baserom.dol", 0x3A65B8, 0x64
.global __vt__15J3DIndBlockNull
__vt__15J3DIndBlockNull:
	.incbin "baserom.dol", 0x3A661C, 0x48
.global __vt__11J3DIndBlock
__vt__11J3DIndBlock:
	.incbin "baserom.dol", 0x3A6664, 0x48
.global __vt__11J3DTevBlock
__vt__11J3DTevBlock:
	.incbin "baserom.dol", 0x3A66AC, 0x9C
.global __vt__14J3DTexGenBlock
__vt__14J3DTexGenBlock:
	.incbin "baserom.dol", 0x3A6748, 0x50
.global __vt__13J3DColorBlock
__vt__13J3DColorBlock:
	.incbin "baserom.dol", 0x3A6798, 0x68
.global $$2988
$$2988:
	.incbin "baserom.dol", 0x3A6800, 0x4C
.global __vt__8J3DModel
__vt__8J3DModel:
	.incbin "baserom.dol", 0x3A684C, 0x1C
.global __vt__12J3DModelData
__vt__12J3DModelData:
	.incbin "baserom.dol", 0x3A6868, 0x10
.global __vt__7J3DNode
__vt__7J3DNode:
	.incbin "baserom.dol", 0x3A6878, 0x28
.global $$2952
$$2952:
	.incbin "baserom.dol", 0x3A68A0, 0xC
.global $$2953
$$2953:
	.incbin "baserom.dol", 0x3A68AC, 0xC
.global $$2954
$$2954:
	.incbin "baserom.dol", 0x3A68B8, 0xC
.global $$2955
$$2955:
	.incbin "baserom.dol", 0x3A68C4, 0xC
.global mtxLoadPipeline__11J3DShapeMtx
mtxLoadPipeline__11J3DShapeMtx:
	.incbin "baserom.dol", 0x3A68D0, 0x30
.global __vt__12J3DShapeDraw
__vt__12J3DShapeDraw:
	.incbin "baserom.dol", 0x3A6900, 0xC
.global __vt__16J3DShapeMtxMulti
__vt__16J3DShapeMtxMulti:
	.incbin "baserom.dol", 0x3A690C, 0x20
.global __vt__13J3DShapeMtxDL
__vt__13J3DShapeMtxDL:
	.incbin "baserom.dol", 0x3A692C, 0x20
.global __vt__11J3DShapeMtx
__vt__11J3DShapeMtx:
	.incbin "baserom.dol", 0x3A694C, 0x34
.global NullTexData
NullTexData:
	.incbin "baserom.dol", 0x3A6980, 0x10
.global __vt__14J3DAnmColorKey
__vt__14J3DAnmColorKey:
	.incbin "baserom.dol", 0x3A6990, 0x10
.global __vt__11J3DAnmColor
__vt__11J3DAnmColor:
	.incbin "baserom.dol", 0x3A69A0, 0x10
.global __vt__15J3DAnmColorFull
__vt__15J3DAnmColorFull:
	.incbin "baserom.dol", 0x3A69B0, 0x10
.global __vt__17J3DAnmVtxColorKey
__vt__17J3DAnmVtxColorKey:
	.incbin "baserom.dol", 0x3A69C0, 0x10
.global __vt__14J3DAnmVtxColor
__vt__14J3DAnmVtxColor:
	.incbin "baserom.dol", 0x3A69D0, 0x10
.global __vt__18J3DAnmVtxColorFull
__vt__18J3DAnmVtxColorFull:
	.incbin "baserom.dol", 0x3A69E0, 0x10
.global __vt__16J3DAnmClusterKey
__vt__16J3DAnmClusterKey:
	.incbin "baserom.dol", 0x3A69F0, 0x10
.global __vt__13J3DAnmCluster
__vt__13J3DAnmCluster:
	.incbin "baserom.dol", 0x3A6A00, 0x10
.global __vt__17J3DAnmClusterFull
__vt__17J3DAnmClusterFull:
	.incbin "baserom.dol", 0x3A6A10, 0x10
.global __vt__18J3DAnmTransformKey
__vt__18J3DAnmTransformKey:
	.incbin "baserom.dol", 0x3A6A20, 0x14
.global __vt__15J3DAnmTransform
__vt__15J3DAnmTransform:
	.incbin "baserom.dol", 0x3A6A34, 0x10
.global __vt__19J3DAnmTransformFull
__vt__19J3DAnmTransformFull:
	.incbin "baserom.dol", 0x3A6A44, 0x10
.global __vt__10J3DAnmBase
__vt__10J3DAnmBase:
	.incbin "baserom.dol", 0x3A6A54, 0xC
.global __vt__18J3DModelLoader_v21
__vt__18J3DModelLoader_v21:
	.incbin "baserom.dol", 0x3A6A60, 0x28
.global __vt__18J3DModelLoader_v26
__vt__18J3DModelLoader_v26:
	.incbin "baserom.dol", 0x3A6A88, 0x28
.global __vt__10J3DTexture
__vt__10J3DTexture:
	.incbin "baserom.dol", 0x3A6AB0, 0xC
.global __vt__14J3DModelLoader
__vt__14J3DModelLoader:
	.incbin "baserom.dol", 0x3A6ABC, 0x2C
.global __vt__19J3DAnmKeyLoader_v15
__vt__19J3DAnmKeyLoader_v15:
	.incbin "baserom.dol", 0x3A6AE8, 0x10
.global __vt__20J3DAnmFullLoader_v15
__vt__20J3DAnmFullLoader_v15:
	.incbin "baserom.dol", 0x3A6AF8, 0x10
.global __vt__12J3DAnmLoader
__vt__12J3DAnmLoader:
	.incbin "baserom.dol", 0x3A6B08, 0x10
.global __vt__20J3DAnmVisibilityFull
__vt__20J3DAnmVisibilityFull:
	.incbin "baserom.dol", 0x3A6B18, 0xC
.global __vt__16J3DAnmTexPattern
__vt__16J3DAnmTexPattern:
	.incbin "baserom.dol", 0x3A6B24, 0xC
.global __vt__15J3DAnmTevRegKey
__vt__15J3DAnmTevRegKey:
	.incbin "baserom.dol", 0x3A6B30, 0xC
.global __vt__19J3DAnmTextureSRTKey
__vt__19J3DAnmTextureSRTKey:
	.incbin "baserom.dol", 0x3A6B3C, 0xC
.global __vt__14J2DGrafContext
__vt__14J2DGrafContext:
	.incbin "baserom.dol", 0x3A6B48, 0x28
.global sMessageBuffer__9JKRDecomp
sMessageBuffer__9JKRDecomp:
	.incbin "baserom.dol", 0x3A6B70, 0x10
.global sMessageQueue__9JKRDecomp
sMessageQueue__9JKRDecomp:
	.incbin "baserom.dol", 0x3A6B80, 0x20
.global __vt__9JKRDecomp
__vt__9JKRDecomp:
	.incbin "baserom.dol", 0x3A6BA0, 0x10
.global __vt__13J2DOrthoGraph
__vt__13J2DOrthoGraph:
	.incbin "baserom.dol", 0x3A6BB0, 0x28
.global __vt__15J3DVertexBuffer
__vt__15J3DVertexBuffer:
	.incbin "baserom.dol", 0x3A6BD8, 0x10
.global __vt__14J3DShapePacket
__vt__14J3DShapePacket:
	.incbin "baserom.dol", 0x3A6BE8, 0x18
.global __vt__12J3DMatPacket
__vt__12J3DMatPacket:
	.incbin "baserom.dol", 0x3A6C00, 0x18
.global __vt__13J3DDrawPacket
__vt__13J3DDrawPacket:
	.incbin "baserom.dol", 0x3A6C18, 0x18
.global __vt__17J3DCallBackPacket
__vt__17J3DCallBackPacket:
	.incbin "baserom.dol", 0x3A6C30, 0x18
.global __vt__9J3DPacket
__vt__9J3DPacket:
	.incbin "baserom.dol", 0x3A6C48, 0x18
.global $$2804
$$2804:
	.incbin "baserom.dol", 0x3A6C60, 0x44
.global j3dDefaultLightInfo
j3dDefaultLightInfo:
	.incbin "baserom.dol", 0x3A6CA4, 0x34
.global j3dDefaultTexCoordInfo
j3dDefaultTexCoordInfo:
	.incbin "baserom.dol", 0x3A6CD8, 0x20
.global j3dDefaultTexMtxInfo
j3dDefaultTexMtxInfo:
	.incbin "baserom.dol", 0x3A6CF8, 0x64
.global j3dDefaultIndTexMtxInfo
j3dDefaultIndTexMtxInfo:
	.incbin "baserom.dol", 0x3A6D5C, 0x1C
.global j3dDefaultTevStageInfo
j3dDefaultTevStageInfo:
	.incbin "baserom.dol", 0x3A6D78, 0x14
.global j3dDefaultIndTevStageInfo
j3dDefaultIndTevStageInfo:
	.incbin "baserom.dol", 0x3A6D8C, 0xC
.global j3dDefaultFogInfo
j3dDefaultFogInfo:
	.incbin "baserom.dol", 0x3A6D98, 0x2C
.global j3dDefaultNBTScaleInfo
j3dDefaultNBTScaleInfo:
	.incbin "baserom.dol", 0x3A6DC4, 0x14
.global $$2814
$$2814:
	.incbin "baserom.dol", 0x3A6DD8, 0xC
.global $$2815
$$2815:
	.incbin "baserom.dol", 0x3A6DE4, 0xC
.global $$2816
$$2816:
	.incbin "baserom.dol", 0x3A6DF0, 0xC
.global $$2817
$$2817:
	.incbin "baserom.dol", 0x3A6DFC, 0xC
.global $$2818
$$2818:
	.incbin "baserom.dol", 0x3A6E08, 0xC
.global $$2819
$$2819:
	.incbin "baserom.dol", 0x3A6E14, 0xC
.global sortFuncTable__13J3DDrawBuffer
sortFuncTable__13J3DDrawBuffer:
	.incbin "baserom.dol", 0x3A6E20, 0x48
.global $$2820
$$2820:
	.incbin "baserom.dol", 0x3A6E68, 0xC
.global $$2821
$$2821:
	.incbin "baserom.dol", 0x3A6E74, 0xC
.global drawFuncTable__13J3DDrawBuffer
drawFuncTable__13J3DDrawBuffer:
	.incbin "baserom.dol", 0x3A6E80, 0x18
.global __vt__14J3DMaterialAnm
__vt__14J3DMaterialAnm:
	.incbin "baserom.dol", 0x3A6E98, 0x10
.global __vt__11J3DTexNoAnm
__vt__11J3DTexNoAnm:
	.incbin "baserom.dol", 0x3A6EA8, 0x10
.global __vt__12JKRAramBlock
__vt__12JKRAramBlock:
	.incbin "baserom.dol", 0x3A6EB8, 0x10
.global sMessageBuffer__13JKRAramStream
sMessageBuffer__13JKRAramStream:
	.incbin "baserom.dol", 0x3A6EC8, 0x10
.global sMessageQueue__13JKRAramStream
sMessageQueue__13JKRAramStream:
	.incbin "baserom.dol", 0x3A6ED8, 0x20
.global __vt__13JKRAramStream
__vt__13JKRAramStream:
	.incbin "baserom.dol", 0x3A6EF8, 0x10
.global saoAboutEncoding___10JUTResFont
saoAboutEncoding___10JUTResFont:
	.incbin "baserom.dol", 0x3A6F08, 0xC
.global __vt__10JUTResFont
__vt__10JUTResFont:
	.incbin "baserom.dol", 0x3A6F14, 0x3C
.global saoAboutEncoding___10JUTRomFont
saoAboutEncoding___10JUTRomFont:
	.incbin "baserom.dol", 0x3A6F50, 0x18
.global __vt__10JUTRomFont
__vt__10JUTRomFont:
	.incbin "baserom.dol", 0x3A6F68, 0x40
.global $$2320
$$2320:
	.incbin "baserom.dol", 0x3A6FA8, 0x54
.global c2r$795
c2r$795:
	.incbin "baserom.dol", 0x3A6FFC, 0x14
.global __vt__10JUTConsole
__vt__10JUTConsole:
	.incbin "baserom.dol", 0x3A7010, 0x10
.global __vt__Q26JDrama6TActor
__vt__Q26JDrama6TActor:
	.incbin "baserom.dol", 0x3A7020, 0xA0
.global __vt__Q26JDrama8TViewObj
__vt__Q26JDrama8TViewObj:
	.incbin "baserom.dol", 0x3A70C0, 0x28
.global __vt__Q26JDrama10TOrthoProj
__vt__Q26JDrama10TOrthoProj:
	.incbin "baserom.dol", 0x3A70E8, 0xD4
.global __vt__Q26JDrama13TLookAtCamera
__vt__Q26JDrama13TLookAtCamera:
	.incbin "baserom.dol", 0x3A71BC, 0xF4
.global __vt__Q26JDrama12TPolarCamera
__vt__Q26JDrama12TPolarCamera:
	.incbin "baserom.dol", 0x3A72B0, 0xDC
.global __vt__Q26JDrama7TCamera
__vt__Q26JDrama7TCamera:
	.incbin "baserom.dol", 0x3A738C, 0xC4
.global __vt__Q26JDrama10TCharacter
__vt__Q26JDrama10TCharacter:
	.incbin "baserom.dol", 0x3A7450, 0x24
.global __vt__Q26JDrama10TSmplChara
__vt__Q26JDrama10TSmplChara:
	.incbin "baserom.dol", 0x3A7474, 0x24
.global __vt__Q26JDrama9TDirector
__vt__Q26JDrama9TDirector:
	.incbin "baserom.dol", 0x3A7498, 0x70
.global __vt__Q26JDrama8TDisplay
__vt__Q26JDrama8TDisplay:
	.incbin "baserom.dol", 0x3A7508, 0x10
.global __vt__Q26JDrama11TDrawBufObj
__vt__Q26JDrama11TDrawBufObj:
	.incbin "baserom.dol", 0x3A7518, 0x28
.global __vt__Q26JDrama55TViewObjPtrListT$$0Q26JDrama8TViewObj$$4Q26JDrama8TViewObj$$1
__vt__Q26JDrama55TViewObjPtrListT$$0Q26JDrama8TViewObj$$4Q26JDrama8TViewObj$$1:
	.incbin "baserom.dol", 0x3A7540, 0x2C
.global __vt__Q26JDrama11TDStageDisp
__vt__Q26JDrama11TDStageDisp:
	.incbin "baserom.dol", 0x3A756C, 0x24
.global __vt__Q26JDrama12TDStageGroup
__vt__Q26JDrama12TDStageGroup:
	.incbin "baserom.dol", 0x3A7590, 0x30
.global __vt__Q26JDrama11TEfbCtrlTex
__vt__Q26JDrama11TEfbCtrlTex:
	.incbin "baserom.dol", 0x3A75C0, 0x24
.global __vt__Q26JDrama12TEfbCtrlDisp
__vt__Q26JDrama12TEfbCtrlDisp:
	.incbin "baserom.dol", 0x3A75E4, 0x24
.global __vt__Q26JDrama8TEfbCtrl
__vt__Q26JDrama8TEfbCtrl:
	.incbin "baserom.dol", 0x3A7608, 0x28
.global __vt__Q26JDrama9TFrmGXSet
__vt__Q26JDrama9TFrmGXSet:
	.incbin "baserom.dol", 0x3A7630, 0x28
.global __vt__Q26JDrama9TLightMap
__vt__Q26JDrama9TLightMap:
	.incbin "baserom.dol", 0x3A7658, 0x24
.global __vt__Q26JDrama7TAmbAry
__vt__Q26JDrama7TAmbAry:
	.incbin "baserom.dol", 0x3A767C, 0x24
.global __vt__Q26JDrama9TAmbColor
__vt__Q26JDrama9TAmbColor:
	.incbin "baserom.dol", 0x3A76A0, 0x64
.global __vt__Q26JDrama9TIdxLight
__vt__Q26JDrama9TIdxLight:
	.incbin "baserom.dol", 0x3A7704, 0x9C
.global __vt__Q26JDrama9TLightAry
__vt__Q26JDrama9TLightAry:
	.incbin "baserom.dol", 0x3A77A0, 0x24
.global __vt__Q26JDrama6TLight
__vt__Q26JDrama6TLight:
	.incbin "baserom.dol", 0x3A77C4, 0x9C
.global __vt__Q26JDrama8TNameRef
__vt__Q26JDrama8TNameRef:
	.incbin "baserom.dol", 0x3A7860, 0x20
.global __vt__Q26JDrama55TNameRefPtrListT$$0Q26JDrama8TNameRef$$4Q26JDrama8TNameRef$$1
__vt__Q26JDrama55TNameRefPtrListT$$0Q26JDrama8TNameRef$$4Q26JDrama8TNameRef$$1:
	.incbin "baserom.dol", 0x3A7880, 0x20
.global __vt__Q26JDrama11TNameRefGen
__vt__Q26JDrama11TNameRefGen:
	.incbin "baserom.dol", 0x3A78A0, 0x10
.global __vt__Q26JDrama10TPlacement
__vt__Q26JDrama10TPlacement:
	.incbin "baserom.dol", 0x3A78B0, 0x28
.global __vt__Q26JDrama7TScreen
__vt__Q26JDrama7TScreen:
	.incbin "baserom.dol", 0x3A78D8, 0x24
.global __vt__13TCamConnecter
__vt__13TCamConnecter:
	.incbin "baserom.dol", 0x3A78FC, 0x24
.global __vt__Q26JDrama9TSmJ3DAct
__vt__Q26JDrama9TSmJ3DAct:
	.incbin "baserom.dol", 0x3A7920, 0xA0
.global __vt__12J3DFrameCtrl
__vt__12J3DFrameCtrl:
	.incbin "baserom.dol", 0x3A79C0, 0x10
.global __vt__Q26JDrama9TSmJ3DScn
__vt__Q26JDrama9TSmJ3DScn:
	.incbin "baserom.dol", 0x3A79D0, 0x30
.global __vt__Q26JDrama14TViewConnecter
__vt__Q26JDrama14TViewConnecter:
	.incbin "baserom.dol", 0x3A7A00, 0x28
.global __vt__Q26JDrama9TViewport
__vt__Q26JDrama9TViewport:
	.incbin "baserom.dol", 0x3A7A28, 0x28
.global __vt__31JALSeModData$$015JALSeModVolFunk$$1
__vt__31JALSeModData$$015JALSeModVolFunk$$1:
	.incbin "baserom.dol", 0x3A7A50, 0x10
.global __vt__31JALSeModData$$015JALSeModPitFunk$$1
__vt__31JALSeModData$$015JALSeModPitFunk$$1:
	.incbin "baserom.dol", 0x3A7A60, 0x10
.global __vt__31JALSeModData$$015JALSeModEffFunk$$1
__vt__31JALSeModData$$015JALSeModEffFunk$$1:
	.incbin "baserom.dol", 0x3A7A70, 0x10
.global __vt__31JALSeModData$$015JALSeModVolDist$$1
__vt__31JALSeModData$$015JALSeModVolDist$$1:
	.incbin "baserom.dol", 0x3A7A80, 0x10
.global __vt__31JALSeModData$$015JALSeModEffDist$$1
__vt__31JALSeModData$$015JALSeModEffDist$$1:
	.incbin "baserom.dol", 0x3A7A90, 0x10
.global __vt__31JALSeModData$$015JALSeModPitDist$$1
__vt__31JALSeModData$$015JALSeModPitDist$$1:
	.incbin "baserom.dol", 0x3A7AA0, 0x10
.global __vt__31JALSeModData$$015JALSeModVolFGrp$$1
__vt__31JALSeModData$$015JALSeModVolFGrp$$1:
	.incbin "baserom.dol", 0x3A7AB0, 0x10
.global __vt__31JALSeModData$$015JALSeModPitFGrp$$1
__vt__31JALSeModData$$015JALSeModPitFGrp$$1:
	.incbin "baserom.dol", 0x3A7AC0, 0x10
.global __vt__31JALSeModData$$015JALSeModEffFGrp$$1
__vt__31JALSeModData$$015JALSeModEffFGrp$$1:
	.incbin "baserom.dol", 0x3A7AD0, 0x10
.global __vt__31JALSeModData$$015JALSeModVolDGrp$$1
__vt__31JALSeModData$$015JALSeModVolDGrp$$1:
	.incbin "baserom.dol", 0x3A7AE0, 0x10
.global __vt__31JALSeModData$$015JALSeModEffDGrp$$1
__vt__31JALSeModData$$015JALSeModEffDGrp$$1:
	.incbin "baserom.dol", 0x3A7AF0, 0x10
.global __vt__31JALSeModData$$015JALSeModPitDGrp$$1
__vt__31JALSeModData$$015JALSeModPitDGrp$$1:
	.incbin "baserom.dol", 0x3A7B00, 0x10
.global __vt__15JALSeModPitDGrp
__vt__15JALSeModPitDGrp:
	.incbin "baserom.dol", 0x3A7B10, 0x10
.global __vt__34JALSeModDataGrp$$015JALSeModPitDGrp$$1
__vt__34JALSeModDataGrp$$015JALSeModPitDGrp$$1:
	.incbin "baserom.dol", 0x3A7B20, 0x10
.global __vt__15JALSeModEffDGrp
__vt__15JALSeModEffDGrp:
	.incbin "baserom.dol", 0x3A7B30, 0x10
.global __vt__34JALSeModDataGrp$$015JALSeModEffDGrp$$1
__vt__34JALSeModDataGrp$$015JALSeModEffDGrp$$1:
	.incbin "baserom.dol", 0x3A7B40, 0x10
.global __vt__15JALSeModVolDGrp
__vt__15JALSeModVolDGrp:
	.incbin "baserom.dol", 0x3A7B50, 0x10
.global __vt__34JALSeModDataGrp$$015JALSeModVolDGrp$$1
__vt__34JALSeModDataGrp$$015JALSeModVolDGrp$$1:
	.incbin "baserom.dol", 0x3A7B60, 0x10
.global __vt__15JALSeModEffFGrp
__vt__15JALSeModEffFGrp:
	.incbin "baserom.dol", 0x3A7B70, 0x10
.global __vt__34JALSeModDataGrp$$015JALSeModEffFGrp$$1
__vt__34JALSeModDataGrp$$015JALSeModEffFGrp$$1:
	.incbin "baserom.dol", 0x3A7B80, 0x10
.global __vt__15JALSeModPitFGrp
__vt__15JALSeModPitFGrp:
	.incbin "baserom.dol", 0x3A7B90, 0x10
.global __vt__34JALSeModDataGrp$$015JALSeModPitFGrp$$1
__vt__34JALSeModDataGrp$$015JALSeModPitFGrp$$1:
	.incbin "baserom.dol", 0x3A7BA0, 0x10
.global __vt__15JALSeModVolFGrp
__vt__15JALSeModVolFGrp:
	.incbin "baserom.dol", 0x3A7BB0, 0x10
.global __vt__34JALSeModDataGrp$$015JALSeModVolFGrp$$1
__vt__34JALSeModDataGrp$$015JALSeModVolFGrp$$1:
	.incbin "baserom.dol", 0x3A7BC0, 0x10
.global __vt__15JALSeModPitDist
__vt__15JALSeModPitDist:
	.incbin "baserom.dol", 0x3A7BD0, 0x10
.global __vt__15JALSeModEffDist
__vt__15JALSeModEffDist:
	.incbin "baserom.dol", 0x3A7BE0, 0x10
.global __vt__15JALSeModVolDist
__vt__15JALSeModVolDist:
	.incbin "baserom.dol", 0x3A7BF0, 0x10
.global __vt__15JALSeModEffFunk
__vt__15JALSeModEffFunk:
	.incbin "baserom.dol", 0x3A7C00, 0x10
.global __vt__15JALSeModPitFunk
__vt__15JALSeModPitFunk:
	.incbin "baserom.dol", 0x3A7C10, 0x10
.global __vt__15JALSeModVolFunk
__vt__15JALSeModVolFunk:
	.incbin "baserom.dol", 0x3A7C20, 0x10
.global __vt__13JAIAnimeSound
__vt__13JAIAnimeSound:
	.incbin "baserom.dol", 0x3A7C30, 0x10
.global $$2547
$$2547:
	.incbin "baserom.dol", 0x3A7C40, 0x24
.global __vt__8JAIBasic
__vt__8JAIBasic:
	.incbin "baserom.dol", 0x3A7C64, 0x3C
.global dummyZeroVec__8JAIConst
dummyZeroVec__8JAIConst:
	.incbin "baserom.dol", 0x3A7CA0, 0xC
.global nullInfoData2__8JAIConst
nullInfoData2__8JAIConst:
	.incbin "baserom.dol", 0x3A7CAC, 0x20
.global sCInfos_0__8JAIConst
sCInfos_0__8JAIConst:
	.incbin "baserom.dol", 0x3A7CCC, 0x24
.global filter_table__Q27JAInter9StreamLib
filter_table__Q27JAInter9StreamLib:
	.incbin "baserom.dol", 0x3A7CF0, 0x40
.global table4__Q27JAInter9StreamLib
table4__Q27JAInter9StreamLib:
	.incbin "baserom.dol", 0x3A7D30, 0x20
.global __vt__8JAISound
__vt__8JAISound:
	.incbin "baserom.dol", 0x3A7D50, 0x38
.global OSC_RELEASE_TABLE__Q28JASystem7BankMgr
OSC_RELEASE_TABLE__Q28JASystem7BankMgr:
	.incbin "baserom.dol", 0x3A7D88, 0xC
.global OSC_ENV__Q28JASystem7BankMgr
OSC_ENV__Q28JASystem7BankMgr:
	.incbin "baserom.dol", 0x3A7D94, 0x1C
.global __vt__Q28JASystem10TBasicBank
__vt__Q28JASystem10TBasicBank:
	.incbin "baserom.dol", 0x3A7DB0, 0x14
.global __vt__Q28JASystem5TBank
__vt__Q28JASystem5TBank:
	.incbin "baserom.dol", 0x3A7DC4, 0x14
.global __vt__Q28JASystem10TBasicInst
__vt__Q28JASystem10TBasicInst:
	.incbin "baserom.dol", 0x3A7DD8, 0x18
.global __vt__Q28JASystem5TInst
__vt__Q28JASystem5TInst:
	.incbin "baserom.dol", 0x3A7DF0, 0x18
.global __vt__Q28JASystem11TWaveHandle
__vt__Q28JASystem11TWaveHandle:
	.incbin "baserom.dol", 0x3A7E08, 0x14
.global __vt__Q38JASystem14TBasicWaveBank11TWaveHandle
__vt__Q38JASystem14TBasicWaveBank11TWaveHandle:
	.incbin "baserom.dol", 0x3A7E1C, 0x14
.global __vt__Q38JASystem14TBasicWaveBank10TWaveGroup
__vt__Q38JASystem14TBasicWaveBank10TWaveGroup:
	.incbin "baserom.dol", 0x3A7E30, 0x14
.global __vt__Q38JASystem13WaveArcLoader7TObject
__vt__Q38JASystem13WaveArcLoader7TObject:
	.incbin "baserom.dol", 0x3A7E44, 0x14
.global __vt__Q28JASystem14TBasicWaveBank
__vt__Q28JASystem14TBasicWaveBank:
	.incbin "baserom.dol", 0x3A7E58, 0x14
.global __vt__Q28JASystem9TWaveBank
__vt__Q28JASystem9TWaveBank:
	.incbin "baserom.dol", 0x3A7E6C, 0x14
.global __vt__Q28JASystem11TInstEffect
__vt__Q28JASystem11TInstEffect:
	.incbin "baserom.dol", 0x3A7E80, 0x10
.global __vt__Q28JASystem8TDrumSet
__vt__Q28JASystem8TDrumSet:
	.incbin "baserom.dol", 0x3A7E90, 0x18
.global __vt__Q28JASystem9TInstRand
__vt__Q28JASystem9TInstRand:
	.incbin "baserom.dol", 0x3A7EA8, 0x10
.global __vt__Q28JASystem10TInstSense
__vt__Q28JASystem10TInstSense:
	.incbin "baserom.dol", 0x3A7EB8, 0x10
.global __vt__Q38JASystem15TSimpleWaveBank11TWaveHandle
__vt__Q38JASystem15TSimpleWaveBank11TWaveHandle:
	.incbin "baserom.dol", 0x3A7EC8, 0x14
.global __vt__Q28JASystem15TSimpleWaveBank
__vt__Q28JASystem15TSimpleWaveBank:
	.incbin "baserom.dol", 0x3A7EDC, 0x34
.global sCurrentDir__Q28JASystem13WaveArcLoader
sCurrentDir__Q28JASystem13WaveArcLoader:
	.incbin "baserom.dol", 0x3A7F10, 0x40
.global calc_sw_table__Q28JASystem6Driver
calc_sw_table__Q28JASystem6Driver:
	.incbin "baserom.dol", 0x3A7F50, 0x54
.global $$2326
$$2326:
	.incbin "baserom.dol", 0x3A7FA4, 0x20
.global $$2325
$$2325:
	.incbin "baserom.dol", 0x3A7FC4, 0x3C
.global C5BASE_PITCHTABLE__Q28JASystem6Driver
C5BASE_PITCHTABLE__Q28JASystem6Driver:
	.incbin "baserom.dol", 0x3A8000, 0x200
.global DSPADPCM_FILTER__Q28JASystem12DSPInterface
DSPADPCM_FILTER__Q28JASystem12DSPInterface:
	.incbin "baserom.dol", 0x3A8200, 0x40
.global DSPRES_FILTER__Q28JASystem12DSPInterface
DSPRES_FILTER__Q28JASystem12DSPInterface:
	.incbin "baserom.dol", 0x3A8240, 0x500
.global history__8JASystem
history__8JASystem:
	.incbin "baserom.dol", 0x3A8740, 0x28
.global SEND_TABLE__Q28JASystem12DSPInterface
SEND_TABLE__Q28JASystem12DSPInterface:
	.incbin "baserom.dol", 0x3A8768, 0x18
.global connect_table$341
connect_table$341:
	.incbin "baserom.dol", 0x3A8780, 0x18
.global FORCE_STOP_TABLE__Q28JASystem6Driver
FORCE_STOP_TABLE__Q28JASystem6Driver:
	.incbin "baserom.dol", 0x3A8798, 0x10
.global audioRootPath__Q28JASystem3Dvd
audioRootPath__Q28JASystem3Dvd:
	.incbin "baserom.dol", 0x3A87A8, 0x20
.global rootDir__Q28JASystem10HardStream
rootDir__Q28JASystem10HardStream:
	.incbin "baserom.dol", 0x3A87C8, 0x20
.global $$2550
$$2550:
	.incbin "baserom.dol", 0x3A87E8, 0x20
.global CUTOFF_TO_IIR_TABLE__Q28JASystem6Player
CUTOFF_TO_IIR_TABLE__Q28JASystem6Player:
	.incbin "baserom.dol", 0x3A8808, 0x400
.global s_key_table__Q28JASystem6Player
s_key_table__Q28JASystem6Player:
	.incbin "baserom.dol", 0x3A8C08, 0x100
.global sAdsrDef__Q28JASystem6Player
sAdsrDef__Q28JASystem6Player:
	.incbin "baserom.dol", 0x3A8D08, 0x18
.global sEnvelopeDef__Q28JASystem6Player
sEnvelopeDef__Q28JASystem6Player:
	.incbin "baserom.dol", 0x3A8D20, 0x18
.global sVibratoDef__Q28JASystem6Player
sVibratoDef__Q28JASystem6Player:
	.incbin "baserom.dol", 0x3A8D38, 0x18
.global sTremoroDef__Q28JASystem6Player
sTremoroDef__Q28JASystem6Player:
	.incbin "baserom.dol", 0x3A8D50, 0x18
.global sAdsTable__Q28JASystem6Player
sAdsTable__Q28JASystem6Player:
	.incbin "baserom.dol", 0x3A8D68, 0x18
.global sRelTable__Q28JASystem6Player
sRelTable__Q28JASystem6Player:
	.incbin "baserom.dol", 0x3A8D80, 0xC
.global sVibTable__Q28JASystem6Player
sVibTable__Q28JASystem6Player:
	.incbin "baserom.dol", 0x3A8D8C, 0x24
.global sTreTable__Q28JASystem6Player
sTreTable__Q28JASystem6Player:
	.incbin "baserom.dol", 0x3A8DB0, 0x28
.global sOscTable__Q28JASystem6TTrack
sOscTable__Q28JASystem6TTrack:
	.incbin "baserom.dol", 0x3A8DD8, 0x8
.global $$2982
$$2982:
	.incbin "baserom.dol", 0x3A8DE0, 0x34
.global $$21090
$$21090:
	.incbin "baserom.dol", 0x3A8E14, 0x44
.global $$21324
$$21324:
	.incbin "baserom.dol", 0x3A8E58, 0x48
.global $$2137
$$2137:
	.incbin "baserom.dol", 0x3A8EA0, 0xC
.global $$2138
$$2138:
	.incbin "baserom.dol", 0x3A8EAC, 0xC
.global $$2139
$$2139:
	.incbin "baserom.dol", 0x3A8EB8, 0xC
.global $$2140
$$2140:
	.incbin "baserom.dol", 0x3A8EC4, 0xC
.global $$2141
$$2141:
	.incbin "baserom.dol", 0x3A8ED0, 0xC
.global $$2142
$$2142:
	.incbin "baserom.dol", 0x3A8EDC, 0xC
.global $$2143
$$2143:
	.incbin "baserom.dol", 0x3A8EE8, 0xC
.global $$2144
$$2144:
	.incbin "baserom.dol", 0x3A8EF4, 0xC
.global $$2145
$$2145:
	.incbin "baserom.dol", 0x3A8F00, 0xC
.global $$2146
$$2146:
	.incbin "baserom.dol", 0x3A8F0C, 0xC
.global $$2147
$$2147:
	.incbin "baserom.dol", 0x3A8F18, 0xC
.global $$2148
$$2148:
	.incbin "baserom.dol", 0x3A8F24, 0xC
.global $$2149
$$2149:
	.incbin "baserom.dol", 0x3A8F30, 0xC
.global $$2150
$$2150:
	.incbin "baserom.dol", 0x3A8F3C, 0xC
.global $$2151
$$2151:
	.incbin "baserom.dol", 0x3A8F48, 0xC
.global $$2152
$$2152:
	.incbin "baserom.dol", 0x3A8F54, 0xC
.global $$2153
$$2153:
	.incbin "baserom.dol", 0x3A8F60, 0xC
.global $$2154
$$2154:
	.incbin "baserom.dol", 0x3A8F6C, 0xC
.global $$2155
$$2155:
	.incbin "baserom.dol", 0x3A8F78, 0xC
.global $$2156
$$2156:
	.incbin "baserom.dol", 0x3A8F84, 0xC
.global $$2157
$$2157:
	.incbin "baserom.dol", 0x3A8F90, 0xC
.global $$2158
$$2158:
	.incbin "baserom.dol", 0x3A8F9C, 0xC
.global $$2159
$$2159:
	.incbin "baserom.dol", 0x3A8FA8, 0xC
.global $$2160
$$2160:
	.incbin "baserom.dol", 0x3A8FB4, 0xC
.global $$2161
$$2161:
	.incbin "baserom.dol", 0x3A8FC0, 0xC
.global $$2162
$$2162:
	.incbin "baserom.dol", 0x3A8FCC, 0xC
.global $$2163
$$2163:
	.incbin "baserom.dol", 0x3A8FD8, 0xC
.global $$2164
$$2164:
	.incbin "baserom.dol", 0x3A8FE4, 0xC
.global $$2165
$$2165:
	.incbin "baserom.dol", 0x3A8FF0, 0xC
.global $$2166
$$2166:
	.incbin "baserom.dol", 0x3A8FFC, 0xC
.global $$2167
$$2167:
	.incbin "baserom.dol", 0x3A9008, 0xC
.global $$2168
$$2168:
	.incbin "baserom.dol", 0x3A9014, 0xC
.global $$2169
$$2169:
	.incbin "baserom.dol", 0x3A9020, 0xC
.global $$2170
$$2170:
	.incbin "baserom.dol", 0x3A902C, 0xC
.global $$2171
$$2171:
	.incbin "baserom.dol", 0x3A9038, 0xC
.global $$2172
$$2172:
	.incbin "baserom.dol", 0x3A9044, 0xC
.global $$2173
$$2173:
	.incbin "baserom.dol", 0x3A9050, 0xC
.global $$2174
$$2174:
	.incbin "baserom.dol", 0x3A905C, 0xC
.global $$2175
$$2175:
	.incbin "baserom.dol", 0x3A9068, 0xC
.global $$2176
$$2176:
	.incbin "baserom.dol", 0x3A9074, 0xC
.global $$2177
$$2177:
	.incbin "baserom.dol", 0x3A9080, 0xC
.global $$2178
$$2178:
	.incbin "baserom.dol", 0x3A908C, 0xC
.global $$2179
$$2179:
	.incbin "baserom.dol", 0x3A9098, 0xC
.global $$2180
$$2180:
	.incbin "baserom.dol", 0x3A90A4, 0xC
.global $$2181
$$2181:
	.incbin "baserom.dol", 0x3A90B0, 0xC
.global $$2182
$$2182:
	.incbin "baserom.dol", 0x3A90BC, 0xC
.global $$2183
$$2183:
	.incbin "baserom.dol", 0x3A90C8, 0xC
.global $$2184
$$2184:
	.incbin "baserom.dol", 0x3A90D4, 0xC
.global $$2185
$$2185:
	.incbin "baserom.dol", 0x3A90E0, 0xC
.global $$2186
$$2186:
	.incbin "baserom.dol", 0x3A90EC, 0xC
.global $$2187
$$2187:
	.incbin "baserom.dol", 0x3A90F8, 0xC
.global $$2188
$$2188:
	.incbin "baserom.dol", 0x3A9104, 0xC
.global $$2189
$$2189:
	.incbin "baserom.dol", 0x3A9110, 0xC
.global sCmdPList__Q28JASystem10TSeqParser
sCmdPList__Q28JASystem10TSeqParser:
	.incbin "baserom.dol", 0x3A911C, 0x304
.global __vt__16J3DMaterialTable
__vt__16J3DMaterialTable:
	.incbin "baserom.dol", 0x3A9420, 0x10
.global $$22833
$$22833:
	.incbin "baserom.dol", 0x3A9430, 0x1C
.global $$23104
$$23104:
	.incbin "baserom.dol", 0x3A944C, 0x2C
.global $$22535
$$22535:
	.incbin "baserom.dol", 0x3A9478, 0x20
.global __vt__12JPADragField
__vt__12JPADragField:
	.incbin "baserom.dol", 0x3A9498, 0x18
.global __vt__14JPARandomField
__vt__14JPARandomField:
	.incbin "baserom.dol", 0x3A94B0, 0x18
.global __vt__18JPAConvectionField
__vt__18JPAConvectionField:
	.incbin "baserom.dol", 0x3A94C8, 0x18
.global __vt__14JPAVortexField
__vt__14JPAVortexField:
	.incbin "baserom.dol", 0x3A94E0, 0x18
.global __vt__14JPANewtonField
__vt__14JPANewtonField:
	.incbin "baserom.dol", 0x3A94F8, 0x18
.global __vt__14JPAMagnetField
__vt__14JPAMagnetField:
	.incbin "baserom.dol", 0x3A9510, 0x18
.global __vt__11JPAAirField
__vt__11JPAAirField:
	.incbin "baserom.dol", 0x3A9528, 0x18
.global __vt__15JPAGravityField
__vt__15JPAGravityField:
	.incbin "baserom.dol", 0x3A9540, 0x18
.global __vt__12JPABaseField
__vt__12JPABaseField:
	.incbin "baserom.dol", 0x3A9558, 0x18
.global __vt__11JPAParticle
__vt__11JPAParticle:
	.incbin "baserom.dol", 0x3A9570, 0x58
.global __vt__15JPABaseParticle
__vt__15JPABaseParticle:
	.incbin "baserom.dol", 0x3A95C8, 0x58
.global $$22383
$$22383:
	.incbin "baserom.dol", 0x3A9620, 0x2C
.global $$22382
$$22382:
	.incbin "baserom.dol", 0x3A964C, 0x2C
.global __vt__25JPADrawCalcEmitterVisitor
__vt__25JPADrawCalcEmitterVisitor:
	.incbin "baserom.dol", 0x3A9678, 0x10
.global __vt__26JPADrawCalcParticleVisitor
__vt__26JPADrawCalcParticleVisitor:
	.incbin "baserom.dol", 0x3A9688, 0x10
.global __vt__26JPADrawExecParticleVisitor
__vt__26JPADrawExecParticleVisitor:
	.incbin "baserom.dol", 0x3A9698, 0x10
.global __vt__25JPADrawExecEmitterVisitor
__vt__25JPADrawExecEmitterVisitor:
	.incbin "baserom.dol", 0x3A96A8, 0x10
.global __vt__24JPADrawCalcChildScaleOut
__vt__24JPADrawCalcChildScaleOut:
	.incbin "baserom.dol", 0x3A96B8, 0x10
.global __vt__24JPADrawCalcChildAlphaOut
__vt__24JPADrawCalcChildAlphaOut:
	.incbin "baserom.dol", 0x3A96C8, 0x10
.global __vt__28JPADrawCalcAlphaFlickMultSin
__vt__28JPADrawCalcAlphaFlickMultSin:
	.incbin "baserom.dol", 0x3A96D8, 0x10
.global __vt__27JPADrawCalcAlphaFlickAddSin
__vt__27JPADrawCalcAlphaFlickAddSin:
	.incbin "baserom.dol", 0x3A96E8, 0x10
.global __vt__27JPADrawCalcAlphaFlickNrmSin
__vt__27JPADrawCalcAlphaFlickNrmSin:
	.incbin "baserom.dol", 0x3A96F8, 0x10
.global __vt__16JPADrawCalcAlpha
__vt__16JPADrawCalcAlpha:
	.incbin "baserom.dol", 0x3A9708, 0x10
.global __vt__31JPADrawCalcColorCopyFromEmitter
__vt__31JPADrawCalcColorCopyFromEmitter:
	.incbin "baserom.dol", 0x3A9718, 0x10
.global __vt__33JPADrawCalcScaleAnmTimingReverseY
__vt__33JPADrawCalcScaleAnmTimingReverseY:
	.incbin "baserom.dol", 0x3A9728, 0x10
.global __vt__33JPADrawCalcScaleAnmTimingReverseX
__vt__33JPADrawCalcScaleAnmTimingReverseX:
	.incbin "baserom.dol", 0x3A9738, 0x10
.global __vt__32JPADrawCalcScaleAnmTimingRepeatY
__vt__32JPADrawCalcScaleAnmTimingRepeatY:
	.incbin "baserom.dol", 0x3A9748, 0x10
.global __vt__32JPADrawCalcScaleAnmTimingRepeatX
__vt__32JPADrawCalcScaleAnmTimingRepeatX:
	.incbin "baserom.dol", 0x3A9758, 0x10
.global __vt__31JPADrawCalcScaleAnmTimingNormal
__vt__31JPADrawCalcScaleAnmTimingNormal:
	.incbin "baserom.dol", 0x3A9768, 0x10
.global __vt__23JPADrawCalcScaleCopyX2Y
__vt__23JPADrawCalcScaleCopyX2Y:
	.incbin "baserom.dol", 0x3A9778, 0x10
.global __vt__24JPADrawCalcScaleYBySpeed
__vt__24JPADrawCalcScaleYBySpeed:
	.incbin "baserom.dol", 0x3A9788, 0x10
.global __vt__24JPADrawCalcScaleXBySpeed
__vt__24JPADrawCalcScaleXBySpeed:
	.incbin "baserom.dol", 0x3A9798, 0x10
.global __vt__17JPADrawCalcScaleY
__vt__17JPADrawCalcScaleY:
	.incbin "baserom.dol", 0x3A97A8, 0x10
.global __vt__17JPADrawCalcScaleX
__vt__17JPADrawCalcScaleX:
	.incbin "baserom.dol", 0x3A97B8, 0x10
.global __vt__19JPADrawExecCallBack
__vt__19JPADrawExecCallBack:
	.incbin "baserom.dol", 0x3A97C8, 0x24
.global __vt__32JPADrawCalcTextureAnmIndexRandom
__vt__32JPADrawCalcTextureAnmIndexRandom:
	.incbin "baserom.dol", 0x3A97EC, 0x24
.global __vt__31JPADrawCalcTextureAnmIndexMerge
__vt__31JPADrawCalcTextureAnmIndexMerge:
	.incbin "baserom.dol", 0x3A9810, 0x24
.global __vt__33JPADrawCalcTextureAnmIndexReverse
__vt__33JPADrawCalcTextureAnmIndexReverse:
	.incbin "baserom.dol", 0x3A9834, 0x24
.global __vt__32JPADrawCalcTextureAnmIndexRepeat
__vt__32JPADrawCalcTextureAnmIndexRepeat:
	.incbin "baserom.dol", 0x3A9858, 0x24
.global __vt__32JPADrawCalcTextureAnmIndexNormal
__vt__32JPADrawCalcTextureAnmIndexNormal:
	.incbin "baserom.dol", 0x3A987C, 0x24
.global __vt__30JPADrawCalcColorAnmFrameRandom
__vt__30JPADrawCalcColorAnmFrameRandom:
	.incbin "baserom.dol", 0x3A98A0, 0x24
.global __vt__29JPADrawCalcColorAnmFrameMerge
__vt__29JPADrawCalcColorAnmFrameMerge:
	.incbin "baserom.dol", 0x3A98C4, 0x24
.global __vt__31JPADrawCalcColorAnmFrameReverse
__vt__31JPADrawCalcColorAnmFrameReverse:
	.incbin "baserom.dol", 0x3A98E8, 0x24
.global __vt__30JPADrawCalcColorAnmFrameRepeat
__vt__30JPADrawCalcColorAnmFrameRepeat:
	.incbin "baserom.dol", 0x3A990C, 0x24
.global __vt__30JPADrawCalcColorAnmFrameNormal
__vt__30JPADrawCalcColorAnmFrameNormal:
	.incbin "baserom.dol", 0x3A9930, 0x24
.global __vt__19JPADrawCalcColorEnv
__vt__19JPADrawCalcColorEnv:
	.incbin "baserom.dol", 0x3A9954, 0x24
.global __vt__19JPADrawCalcColorPrm
__vt__19JPADrawCalcColorPrm:
	.incbin "baserom.dol", 0x3A9978, 0x24
.global __vt__31JPADrawExecRegisterColorChildPE
__vt__31JPADrawExecRegisterColorChildPE:
	.incbin "baserom.dol", 0x3A999C, 0x10
.global __vt__32JPADrawExecRegisterColorEmitterE
__vt__32JPADrawExecRegisterColorEmitterE:
	.incbin "baserom.dol", 0x3A99AC, 0x10
.global __vt__32JPADrawExecRegisterColorEmitterP
__vt__32JPADrawExecRegisterColorEmitterP:
	.incbin "baserom.dol", 0x3A99BC, 0x10
.global __vt__33JPADrawExecRegisterColorEmitterPE
__vt__33JPADrawExecRegisterColorEmitterPE:
	.incbin "baserom.dol", 0x3A99CC, 0x10
.global __vt__22JPADrawExecStripeCross
__vt__22JPADrawExecStripeCross:
	.incbin "baserom.dol", 0x3A99DC, 0x10
.global __vt__17JPADrawExecStripe
__vt__17JPADrawExecStripe:
	.incbin "baserom.dol", 0x3A99EC, 0x10
.global __vt__15JPADrawExecLine
__vt__15JPADrawExecLine:
	.incbin "baserom.dol", 0x3A99FC, 0x10
.global __vt__16JPADrawExecPoint
__vt__16JPADrawExecPoint:
	.incbin "baserom.dol", 0x3A9A0C, 0x10
.global __vt__24JPADrawExecRotationCross
__vt__24JPADrawExecRotationCross:
	.incbin "baserom.dol", 0x3A9A1C, 0x10
.global __vt__19JPADrawExecRotation
__vt__19JPADrawExecRotation:
	.incbin "baserom.dol", 0x3A9A2C, 0x10
.global __vt__23JPADrawExecDirBillBoard
__vt__23JPADrawExecDirBillBoard:
	.incbin "baserom.dol", 0x3A9A3C, 0x10
.global __vt__30JPADrawExecRotDirectionalCross
__vt__30JPADrawExecRotDirectionalCross:
	.incbin "baserom.dol", 0x3A9A4C, 0x10
.global __vt__27JPADrawExecDirectionalCross
__vt__27JPADrawExecDirectionalCross:
	.incbin "baserom.dol", 0x3A9A5C, 0x10
.global __vt__25JPADrawExecRotDirectional
__vt__25JPADrawExecRotDirectional:
	.incbin "baserom.dol", 0x3A9A6C, 0x10
.global __vt__22JPADrawExecDirectional
__vt__22JPADrawExecDirectional:
	.incbin "baserom.dol", 0x3A9A7C, 0x10
.global __vt__24JPADrawExecRotYBillBoard
__vt__24JPADrawExecRotYBillBoard:
	.incbin "baserom.dol", 0x3A9A8C, 0x10
.global __vt__21JPADrawExecYBillBoard
__vt__21JPADrawExecYBillBoard:
	.incbin "baserom.dol", 0x3A9A9C, 0x10
.global __vt__23JPADrawExecRotBillBoard
__vt__23JPADrawExecRotBillBoard:
	.incbin "baserom.dol", 0x3A9AAC, 0x10
.global __vt__20JPADrawExecBillBoard
__vt__20JPADrawExecBillBoard:
	.incbin "baserom.dol", 0x3A9ABC, 0x10
.global __vt__26JPADrawExecRegisterPrmAEnv
__vt__26JPADrawExecRegisterPrmAEnv:
	.incbin "baserom.dol", 0x3A9ACC, 0x10
.global __vt__26JPADrawExecRegisterPrmCEnv
__vt__26JPADrawExecRegisterPrmCEnv:
	.incbin "baserom.dol", 0x3A9ADC, 0x10
.global __vt__30JPADrawExecRegisterEnvColorAnm
__vt__30JPADrawExecRegisterEnvColorAnm:
	.incbin "baserom.dol", 0x3A9AEC, 0x10
.global __vt__30JPADrawExecRegisterPrmAlphaAnm
__vt__30JPADrawExecRegisterPrmAlphaAnm:
	.incbin "baserom.dol", 0x3A9AFC, 0x10
.global __vt__30JPADrawExecRegisterPrmColorAnm
__vt__30JPADrawExecRegisterPrmColorAnm:
	.incbin "baserom.dol", 0x3A9B0C, 0x10
.global __vt__23JPADrawExecSetLineWidth
__vt__23JPADrawExecSetLineWidth:
	.incbin "baserom.dol", 0x3A9B1C, 0x10
.global __vt__23JPADrawExecSetPointSize
__vt__23JPADrawExecSetPointSize:
	.incbin "baserom.dol", 0x3A9B2C, 0x10
.global __vt__22JPADrawExecLoadTexture
__vt__22JPADrawExecLoadTexture:
	.incbin "baserom.dol", 0x3A9B3C, 0x24
.global __vt__29JPADrawExecLoadDefaultTexture
__vt__29JPADrawExecLoadDefaultTexture:
	.incbin "baserom.dol", 0x3A9B60, 0x10
.global __vt__20JPADrawExecSetTexMtx
__vt__20JPADrawExecSetTexMtx:
	.incbin "baserom.dol", 0x3A9B70, 0x24
.global __vt__20JPADrawExecGenIdtMtx
__vt__20JPADrawExecGenIdtMtx:
	.incbin "baserom.dol", 0x3A9B94, 0x10
.global __vt__21JPADrawExecGenTexMtx0
__vt__21JPADrawExecGenTexMtx0:
	.incbin "baserom.dol", 0x3A9BA4, 0x10
.global __vt__23JPADrawExecGenPrjTexMtx
__vt__23JPADrawExecGenPrjTexMtx:
	.incbin "baserom.dol", 0x3A9BB4, 0x10
.global __vt__20JPADrawExecGenPrjMtx
__vt__20JPADrawExecGenPrjMtx:
	.incbin "baserom.dol", 0x3A9BC4, 0x10
.global __vt__20JPADrawExecLoadExTex
__vt__20JPADrawExecLoadExTex:
	.incbin "baserom.dol", 0x3A9BD4, 0x14
.global __vt__13JPAExTexShape
__vt__13JPAExTexShape:
	.incbin "baserom.dol", 0x3A9BE8, 0xC
.global __vt__12JPADataBlock
__vt__12JPADataBlock:
	.incbin "baserom.dol", 0x3A9BF4, 0xC
.global __vt__16JPAKeyFrameAnime
__vt__16JPAKeyFrameAnime:
	.incbin "baserom.dol", 0x3A9C00, 0xC
.global __vt__16JPAEmitterLoader
__vt__16JPAEmitterLoader:
	.incbin "baserom.dol", 0x3A9C0C, 0xC
.global __vt__20JPAEmitterLoader_v10
__vt__20JPAEmitterLoader_v10:
	.incbin "baserom.dol", 0x3A9C18, 0x10
.global stBlendMode__12JPABaseShape
stBlendMode__12JPABaseShape:
	.incbin "baserom.dol", 0x3A9C28, 0x10
.global stBlendFactor__12JPABaseShape
stBlendFactor__12JPABaseShape:
	.incbin "baserom.dol", 0x3A9C38, 0x28
.global stLogicOp__12JPABaseShape
stLogicOp__12JPABaseShape:
	.incbin "baserom.dol", 0x3A9C60, 0x40
.global stCompare__12JPABaseShape
stCompare__12JPABaseShape:
	.incbin "baserom.dol", 0x3A9CA0, 0x20
.global stAlphaOp__12JPABaseShape
stAlphaOp__12JPABaseShape:
	.incbin "baserom.dol", 0x3A9CC0, 0x10
.global __vt__12JPABaseShape
__vt__12JPABaseShape:
	.incbin "baserom.dol", 0x3A9CD0, 0x10
.global __vt__13JPAExtraShape
__vt__13JPAExtraShape:
	.incbin "baserom.dol", 0x3A9CE0, 0x10
.global __vt__13JPASweepShape
__vt__13JPASweepShape:
	.incbin "baserom.dol", 0x3A9CF0, 0x10
.global __vt__10JPATexture
__vt__10JPATexture:
	.incbin "baserom.dol", 0x3A9D00, 0x10
.global __vt__Q26JStage13TAmbientLight
__vt__Q26JStage13TAmbientLight:
	.incbin "baserom.dol", 0x3A9D10, 0x38
.global __vt__Q26JStage7TCamera
__vt__Q26JStage7TCamera:
	.incbin "baserom.dol", 0x3A9D48, 0x88
.global __vt__Q26JStage6TActor
__vt__Q26JStage6TActor:
	.incbin "baserom.dol", 0x3A9DD0, 0x68
.global __vt__Q26JStage6TLight
__vt__Q26JStage6TLight:
	.incbin "baserom.dol", 0x3A9E38, 0x60
.global __vt__Q26JStage7TObject
__vt__Q26JStage7TObject:
	.incbin "baserom.dol", 0x3A9E98, 0x30
.global __vt__Q26JStage7TSystem
__vt__Q26JStage7TSystem:
	.incbin "baserom.dol", 0x3A9EC8, 0x58
.global jdsp
jdsp:
	.incbin "baserom.dol", 0x3A9F20, 0x1CE0
.global __files
__files:
	.incbin "baserom.dol", 0x3ABC00, 0xD8
.global $$21009
$$21009:
	.incbin "baserom.dol", 0x3ABCD8, 0x84
.global $$21066
$$21066:
	.incbin "baserom.dol", 0x3ABD5C, 0x84
.global $$21186
$$21186:
	.incbin "baserom.dol", 0x3ABDE0, 0xD0
.global $$21185
$$21185:
	.incbin "baserom.dol", 0x3ABEB0, 0x48
.global $$2410
$$2410:
	.incbin "baserom.dol", 0x3ABEF8, 0x48
.global __float_nan
__float_nan:
	.incbin "baserom.dol", 0x3ABF40, 0x4
.global __float_huge
__float_huge:
	.incbin "baserom.dol", 0x3ABF44, 0x4
.global __four_over_pi_m1
__four_over_pi_m1:
	.incbin "baserom.dol", 0x3ABF48, 0x10
.global gTRKDispatchTable
gTRKDispatchTable:
	.incbin "baserom.dol", 0x3ABF58, 0x88
.global $$2175
$$2175:
	.incbin "baserom.dol", 0x3ABFE0, 0x1C
.global $$2214
$$2214:
	.incbin "baserom.dol", 0x3ABFFC, 0x1C
.global gTRKRestoreFlags
gTRKRestoreFlags:
	.incbin "baserom.dol", 0x3AC018, 0xC
.global gTRKExceptionStatus
gTRKExceptionStatus:
	.incbin "baserom.dol", 0x3AC024, 0x10
.global gTRKStepStatus
gTRKStepStatus:
	.incbin "baserom.dol", 0x3AC034, 0x14
.global TRK_ISR_OFFSETS
TRK_ISR_OFFSETS:
	.incbin "baserom.dol", 0x3AC048, 0x40
.global gDBCommTable
gDBCommTable:
	.incbin "baserom.dol", 0x3AC088, 0x20
.global $$27
$$27:
	.incbin "baserom.dol", 0x3AC0A8, 0x18
.global $$284
$$284:
	.incbin "baserom.dol", 0x3AC0C0, 0x20
.global $$285
$$285:
	.incbin "baserom.dol", 0x3AC0E0, 0x18
.global $$286
$$286:
	.incbin "baserom.dol", 0x3AC0F8, 0xC
.global $$287
$$287:
	.incbin "baserom.dol", 0x3AC104, 0xC
.global $$288
$$288:
	.incbin "baserom.dol", 0x3AC110, 0x10
.global $$289
$$289:
	.incbin "baserom.dol", 0x3AC120, 0xC
.global $$290
$$290:
	.incbin "baserom.dol", 0x3AC12C, 0x10
.global $$291
$$291:
	.incbin "baserom.dol", 0x3AC13C, 0x10
.global $$292
$$292:
	.incbin "baserom.dol", 0x3AC14C, 0x10
.global $$293
$$293:
	.incbin "baserom.dol", 0x3AC15C, 0x10
.global $$294
$$294:
	.incbin "baserom.dol", 0x3AC16C, 0x14
.global $$295
$$295:
	.incbin "baserom.dol", 0x3AC180, 0x10
.global $$296
$$296:
	.incbin "baserom.dol", 0x3AC190, 0x18
.global __OSExceptionLocations
__OSExceptionLocations:
	.incbin "baserom.dol", 0x3AC1A8, 0x3C
.global $$2130
$$2130:
	.incbin "baserom.dol", 0x3AC1E4, 0x1C
.global $$2131
$$2131:
	.incbin "baserom.dol", 0x3AC200, 0x30
.global $$2132
$$2132:
	.incbin "baserom.dol", 0x3AC230, 0x30
.global $$2133
$$2133:
	.incbin "baserom.dol", 0x3AC260, 0x20
.global $$2352
$$2352:
	.incbin "baserom.dol", 0x3AC280, 0x24
.global $$2353
$$2353:
	.incbin "baserom.dol", 0x3AC2A4, 0x38
.global $$2354
$$2354:
	.incbin "baserom.dol", 0x3AC2DC, 0x28
.global $$2355
$$2355:
	.incbin "baserom.dol", 0x3AC304, 0x50
.global $$2356
$$2356:
	.incbin "baserom.dol", 0x3AC354, 0x40
.global $$2357
$$2357:
	.incbin "baserom.dol", 0x3AC394, 0x38
.global $$2358
$$2358:
	.incbin "baserom.dol", 0x3AC3CC, 0x4C
.global $$2359
$$2359:
	.incbin "baserom.dol", 0x3AC418, 0x34
.global $$2360
$$2360:
	.incbin "baserom.dol", 0x3AC44C, 0x40
.global $$2361
$$2361:
	.incbin "baserom.dol", 0x3AC48C, 0x3C
.global $$2362
$$2362:
	.incbin "baserom.dol", 0x3AC4C8, 0x48
.global $$2363
$$2363:
	.incbin "baserom.dol", 0x3AC510, 0x60
.global $$2364
$$2364:
	.incbin "baserom.dol", 0x3AC570, 0x2C
.global $$2383
$$2383:
	.incbin "baserom.dol", 0x3AC59C, 0x14
.global $$2384
$$2384:
	.incbin "baserom.dol", 0x3AC5B0, 0x14
.global $$2385
$$2385:
	.incbin "baserom.dol", 0x3AC5C4, 0x1C
.global $$2386
$$2386:
	.incbin "baserom.dol", 0x3AC5E0, 0x14
.global $$2387
$$2387:
	.incbin "baserom.dol", 0x3AC5F4, 0x10
.global $$2388
$$2388:
	.incbin "baserom.dol", 0x3AC604, 0x14
.global DSPInitCode
DSPInitCode:
	.incbin "baserom.dol", 0x3AC618, 0x80
.global $$269
$$269:
	.incbin "baserom.dol", 0x3AC698, 0x2C
.global $$290
$$290:
	.incbin "baserom.dol", 0x3AC6C4, 0x18
.global $$291
$$291:
	.incbin "baserom.dol", 0x3AC6DC, 0x1C
.global $$292
$$292:
	.incbin "baserom.dol", 0x3AC6F8, 0x30
.global $$293
$$293:
	.incbin "baserom.dol", 0x3AC728, 0x3C
.global $$294
$$294:
	.incbin "baserom.dol", 0x3AC764, 0x38
.global $$295
$$295:
	.incbin "baserom.dol", 0x3AC79C, 0x40
.global $$296
$$296:
	.incbin "baserom.dol", 0x3AC7DC, 0x2C
.global $$297
$$297:
	.incbin "baserom.dol", 0x3AC808, 0x20
.global $$298
$$298:
	.incbin "baserom.dol", 0x3AC828, 0x1C
.global $$2110
$$2110:
	.incbin "baserom.dol", 0x3AC844, 0x1C
.global $$2111
$$2111:
	.incbin "baserom.dol", 0x3AC860, 0x1C
.global $$2112
$$2112:
	.incbin "baserom.dol", 0x3AC87C, 0x18
.global $$2113
$$2113:
	.incbin "baserom.dol", 0x3AC894, 0x34
.global $$259
$$259:
	.incbin "baserom.dol", 0x3AC8C8, 0x44
.global $$260
$$260:
	.incbin "baserom.dol", 0x3AC90C, 0x30
.global $$261
$$261:
	.incbin "baserom.dol", 0x3AC93C, 0x30
.global $$262
$$262:
	.incbin "baserom.dol", 0x3AC96C, 0x30
.global $$263
$$263:
	.incbin "baserom.dol", 0x3AC99C, 0x14
.global $$264
$$264:
	.incbin "baserom.dol", 0x3AC9B0, 0x24
.global $$265
$$265:
	.incbin "baserom.dol", 0x3AC9D4, 0x14
.global $$266
$$266:
	.incbin "baserom.dol", 0x3AC9E8, 0x1C
.global $$267
$$267:
	.incbin "baserom.dol", 0x3ACA04, 0x14
.global $$268
$$268:
	.incbin "baserom.dol", 0x3ACA18, 0x20
.global $$269
$$269:
	.incbin "baserom.dol", 0x3ACA38, 0x28
.global $$270
$$270:
	.incbin "baserom.dol", 0x3ACA60, 0x1C
.global $$274
$$274:
	.incbin "baserom.dol", 0x3ACA7C, 0x24
.global $$210
$$210:
	.incbin "baserom.dol", 0x3ACAA0, 0x18
.global $$211
$$211:
	.incbin "baserom.dol", 0x3ACAB8, 0x28
.global $$212
$$212:
	.incbin "baserom.dol", 0x3ACAE0, 0x1C
.global $$230
$$230:
	.incbin "baserom.dol", 0x3ACAFC, 0x20
.global $$231
$$231:
	.incbin "baserom.dol", 0x3ACB1C, 0x18
.global $$233
$$233:
	.incbin "baserom.dol", 0x3ACB34, 0x34
.global $$234
$$234:
	.incbin "baserom.dol", 0x3ACB68, 0x10
.global $$235
$$235:
	.incbin "baserom.dol", 0x3ACB78, 0x60
.global $$236
$$236:
	.incbin "baserom.dol", 0x3ACBD8, 0x4C
.global $$237
$$237:
	.incbin "baserom.dol", 0x3ACC24, 0x64
.global $$238
$$238:
	.incbin "baserom.dol", 0x3ACC88, 0x60
.global $$239
$$239:
	.incbin "baserom.dol", 0x3ACCE8, 0x20
.global $$240
$$240:
	.incbin "baserom.dol", 0x3ACD08, 0x20
.global $$241
$$241:
	.incbin "baserom.dol", 0x3ACD28, 0x1C
.global $$242
$$242:
	.incbin "baserom.dol", 0x3ACD44, 0x38
.global $$243
$$243:
	.incbin "baserom.dol", 0x3ACD7C, 0x44
.global HankakuToCode
HankakuToCode:
	.incbin "baserom.dol", 0x3ACDC0, 0x180
.global Zenkaku2Code
Zenkaku2Code:
	.incbin "baserom.dol", 0x3ACF40, 0x990
.global InterruptPrioTable
InterruptPrioTable:
	.incbin "baserom.dol", 0x3AD8D0, 0x30
.global ResetFunctionInfo
ResetFunctionInfo:
	.incbin "baserom.dol", 0x3AD900, 0x10
.global YearDays
YearDays:
	.incbin "baserom.dol", 0x3AD910, 0x30
.global LeapYearDays
LeapYearDays:
	.incbin "baserom.dol", 0x3AD940, 0x30
.global $$2117
$$2117:
	.incbin "baserom.dol", 0x3AD970, 0xC8
.global $$2138
$$2138:
	.incbin "baserom.dol", 0x3ADA38, 0x38
.global $$2237
$$2237:
	.incbin "baserom.dol", 0x3ADA70, 0x34
.global $$2263
$$2263:
	.incbin "baserom.dol", 0x3ADAA4, 0x30
.global $$2269
$$2269:
	.incbin "baserom.dol", 0x3ADAD4, 0x28
.global $$2309
$$2309:
	.incbin "baserom.dol", 0x3ADAFC, 0x3C
.global $$2340
$$2340:
	.incbin "baserom.dol", 0x3ADB38, 0x68
.global $$2341
$$2341:
	.incbin "baserom.dol", 0x3ADBA0, 0x58
.global $$2342
$$2342:
	.incbin "baserom.dol", 0x3ADBF8, 0x5C
.global $$2373
$$2373:
	.incbin "baserom.dol", 0x3ADC54, 0x64
.global $$2374
$$2374:
	.incbin "baserom.dol", 0x3ADCB8, 0x50
.global $$2375
$$2375:
	.incbin "baserom.dol", 0x3ADD08, 0x58
.global $$213
$$213:
	.incbin "baserom.dol", 0x3ADD60, 0x18
.global $$214
$$214:
	.incbin "baserom.dol", 0x3ADD78, 0xC
.global $$215
$$215:
	.incbin "baserom.dol", 0x3ADD84, 0x1C
.global $$216
$$216:
	.incbin "baserom.dol", 0x3ADDA0, 0xC
.global $$2293
$$2293:
	.incbin "baserom.dol", 0x3ADDAC, 0x20
.global $$2331
$$2331:
	.incbin "baserom.dol", 0x3ADDCC, 0x40
.global $$2835
$$2835:
	.incbin "baserom.dol", 0x3ADE0C, 0x34
.global $$2936
$$2936:
	.incbin "baserom.dol", 0x3ADE40, 0x38
.global ErrorTable
ErrorTable:
	.incbin "baserom.dol", 0x3ADE78, 0x48
.global $$236
$$236:
	.incbin "baserom.dol", 0x3ADEC0, 0x1C
.global $$237
$$237:
	.incbin "baserom.dol", 0x3ADEDC, 0x18
.global $$238
$$238:
	.incbin "baserom.dol", 0x3ADEF4, 0x14
.global $$239
$$239:
	.incbin "baserom.dol", 0x3ADF08, 0x14
.global $$242
$$242:
	.incbin "baserom.dol", 0x3ADF1C, 0x14
.global timing
timing:
	.incbin "baserom.dol", 0x3ADF30, 0x130
.global taps
taps:
	.incbin "baserom.dol", 0x3AE060, 0x34
.global $$292
$$292:
	.incbin "baserom.dol", 0x3AE094, 0x58
.global $$2336
$$2336:
	.incbin "baserom.dol", 0x3AE0EC, 0x2C
.global $$2337
$$2337:
	.incbin "baserom.dol", 0x3AE118, 0x2C
.global $$2338
$$2338:
	.incbin "baserom.dol", 0x3AE144, 0x2C
.global $$2339
$$2339:
	.incbin "baserom.dol", 0x3AE170, 0x2C
.global $$2340
$$2340:
	.incbin "baserom.dol", 0x3AE19C, 0x2C
.global $$2341
$$2341:
	.incbin "baserom.dol", 0x3AE1C8, 0x30
.global ResetFunctionInfo
ResetFunctionInfo:
	.incbin "baserom.dol", 0x3AE1F8, 0x10
.global $$216
$$216:
	.incbin "baserom.dol", 0x3AE208, 0x20
.global $$217
$$217:
	.incbin "baserom.dol", 0x3AE228, 0xC
.global $$218
$$218:
	.incbin "baserom.dol", 0x3AE234, 0xC
.global $$2264
$$2264:
	.incbin "baserom.dol", 0x3AE240, 0x20
.global $$2265
$$2265:
	.incbin "baserom.dol", 0x3AE260, 0x30
.global $$2266
$$2266:
	.incbin "baserom.dol", 0x3AE290, 0x30
.global $$2267
$$2267:
	.incbin "baserom.dol", 0x3AE2C0, 0x30
.global $$2268
$$2268:
	.incbin "baserom.dol", 0x3AE2F0, 0x30
.global $$2269
$$2269:
	.incbin "baserom.dol", 0x3AE320, 0x30
.global $$2292
$$2292:
	.incbin "baserom.dol", 0x3AE350, 0x30
.global ResetFunctionInfo
ResetFunctionInfo:
	.incbin "baserom.dol", 0x3AE380, 0x20
.global CardData
CardData:
	.incbin "baserom.dol", 0x3AE3A0, 0x160
.global SectorSizeTable
SectorSizeTable:
	.incbin "baserom.dol", 0x3AE500, 0x20
.global LatencyTable
LatencyTable:
	.incbin "baserom.dol", 0x3AE520, 0x20
.global $$2128
$$2128:
	.incbin "baserom.dol", 0x3AE540, 0x68
.global $$2189
$$2189:
	.incbin "baserom.dol", 0x3AE5A8, 0x68
.global $$2256
$$2256:
	.incbin "baserom.dol", 0x3AE610, 0x68
.global $$2302
$$2302:
	.incbin "baserom.dol", 0x3AE678, 0x44
.global $$2329
$$2329:
	.incbin "baserom.dol", 0x3AE6BC, 0x44
.global $$2393
$$2393:
	.incbin "baserom.dol", 0x3AE700, 0x44
.global $$2499
$$2499:
	.incbin "baserom.dol", 0x3AE744, 0x1C
.global $$2498
$$2498:
	.incbin "baserom.dol", 0x3AE760, 0x58
.global GXNtsc480IntDf
GXNtsc480IntDf:
	.incbin "baserom.dol", 0x3AE7B8, 0x3C
.global GXNtsc480Int
GXNtsc480Int:
	.incbin "baserom.dol", 0x3AE7F4, 0x3C
.global GXMpal480IntDf
GXMpal480IntDf:
	.incbin "baserom.dol", 0x3AE830, 0x3C
.global GXPal528IntDf
GXPal528IntDf:
	.incbin "baserom.dol", 0x3AE86C, 0x3C
.global GXEurgb60Hz480IntDf
GXEurgb60Hz480IntDf:
	.incbin "baserom.dol", 0x3AE8A8, 0x40
.global $$292
$$292:
	.incbin "baserom.dol", 0x3AE8E8, 0xF4
.global $$2133
$$2133:
	.incbin "baserom.dol", 0x3AE9DC, 0xF4
.global $$2168
$$2168:
	.incbin "baserom.dol", 0x3AEAD0, 0x40
.global c2r$71
c2r$71:
	.incbin "baserom.dol", 0x3AEB10, 0x28
.global p2f$194
p2f$194:
	.incbin "baserom.dol", 0x3AEB38, 0x20
.global $$2242
$$2242:
	.incbin "baserom.dol", 0x3AEB58, 0x5C
.global $$2241
$$2241:
	.incbin "baserom.dol", 0x3AEBB4, 0x94
.global $$2121
$$2121:
	.incbin "baserom.dol", 0x3AEC48, 0x68
.global $$2207
$$2207:
	.incbin "baserom.dol", 0x3AECB0, 0x48
.global c2r$382
c2r$382:
	.incbin "baserom.dol", 0x3AECF8, 0x10
.global Si
Si:
	.incbin "baserom.dol", 0x3AED08, 0x14
.global Type
Type:
	.incbin "baserom.dol", 0x3AED1C, 0x10
.global $$2456
$$2456:
	.incbin "baserom.dol", 0x3AED2C, 0xC
.global $$2458
$$2458:
	.incbin "baserom.dol", 0x3AED38, 0x10
.global $$2459
$$2459:
	.incbin "baserom.dol", 0x3AED48, 0x10
.global $$2460
$$2460:
	.incbin "baserom.dol", 0x3AED58, 0x10
.global $$2461
$$2461:
	.incbin "baserom.dol", 0x3AED68, 0xC
.global $$2462
$$2462:
	.incbin "baserom.dol", 0x3AED74, 0x10
.global $$2463
$$2463:
	.incbin "baserom.dol", 0x3AED84, 0x14
.global $$2464
$$2464:
	.incbin "baserom.dol", 0x3AED98, 0x14
.global $$2465
$$2465:
	.incbin "baserom.dol", 0x3AEDAC, 0x14
.global $$2466
$$2466:
	.incbin "baserom.dol", 0x3AEDC0, 0xC
.global $$2467
$$2467:
	.incbin "baserom.dol", 0x3AEDCC, 0xC
.global XYNTSC
XYNTSC:
	.incbin "baserom.dol", 0x3AEDD8, 0x30
.global XYPAL
XYPAL:
	.incbin "baserom.dol", 0x3AEE08, 0x30
.global $$216
$$216:
	.incbin "baserom.dol", 0x3AEE38, 0x38
.global VolumeTable
VolumeTable:
	.incbin "baserom.dol", 0x3AEE70, 0x100
.global __vt__15TDLColorTexQuad
__vt__15TDLColorTexQuad:
	.incbin "baserom.dol", 0x3AEF70, 0x20
.global __vt__10TDLTexQuad
__vt__10TDLTexQuad:
	.incbin "baserom.dol", 0x3AEF90, 0x20
.global __vt__11TSilhouette
__vt__11TSilhouette:
	.incbin "baserom.dol", 0x3AEFB0, 0x28
.global __vt__22TLightWithDBSetManager
__vt__22TLightWithDBSetManager:
	.incbin "baserom.dol", 0x3AEFD8, 0x24
.global __vt__23TIndirectLightWithDBSet
__vt__23TIndirectLightWithDBSet:
	.incbin "baserom.dol", 0x3AEFFC, 0x28
.global __vt__24TMapObjectLightWithDBSet
__vt__24TMapObjectLightWithDBSet:
	.incbin "baserom.dol", 0x3AF024, 0x28
.global __vt__21TObjectLightWithDBSet
__vt__21TObjectLightWithDBSet:
	.incbin "baserom.dol", 0x3AF04C, 0x28
.global __vt__21TPlayerLightWithDBSet
__vt__21TPlayerLightWithDBSet:
	.incbin "baserom.dol", 0x3AF074, 0x28
.global __vt__15TLightWithDBSet
__vt__15TLightWithDBSet:
	.incbin "baserom.dol", 0x3AF09C, 0x28
.global __vt__16TLightDrawBuffer
__vt__16TLightDrawBuffer:
	.incbin "baserom.dol", 0x3AF0C4, 0x28
.global __vt__11TLightMario
__vt__11TLightMario:
	.incbin "baserom.dol", 0x3AF0EC, 0x34
.global __vt__12TLightShadow
__vt__12TLightShadow:
	.incbin "baserom.dol", 0x3AF120, 0x34
.global __vt__12TLightCommon
__vt__12TLightCommon:
	.incbin "baserom.dol", 0x3AF154, 0x34
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3AF188, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3AF194, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3AF1A0, 0x10
.global atntable
atntable:
	.incbin "baserom.dol", 0x3AF1B0, 0x808
.global __vt__10TParamT$$0f$$1
__vt__10TParamT$$0f$$1:
	.incbin "baserom.dol", 0x3AF9B8, 0xC
.global __vt__30TParamT$$0Q29JGeometry8TVec3$$0f$$1$$1
__vt__30TParamT$$0Q29JGeometry8TVec3$$0f$$1$$1:
	.incbin "baserom.dol", 0x3AF9C4, 0xC
.global __vt__20TMtxSwingRZReverseXZ
__vt__20TMtxSwingRZReverseXZ:
	.incbin "baserom.dol", 0x3AF9D0, 0xC
.global __vt__9TParamVec
__vt__9TParamVec:
	.incbin "baserom.dol", 0x3AF9DC, 0xC
.global __vt__11TMtxSwingRZ
__vt__11TMtxSwingRZ:
	.incbin "baserom.dol", 0x3AF9E8, 0xC
.global __vt__11TParamRT$$0f$$1
__vt__11TParamRT$$0f$$1:
	.incbin "baserom.dol", 0x3AF9F4, 0xC
.global __vt__11TMtxTimeLag
__vt__11TMtxTimeLag:
	.incbin "baserom.dol", 0x3AFA00, 0xC
.global __vt__14TMtxEffectBase
__vt__14TMtxEffectBase:
	.incbin "baserom.dol", 0x3AFA0C, 0xC
.global __vt__14TScreenTexture
__vt__14TScreenTexture:
	.incbin "baserom.dol", 0x3AFA18, 0x24
.global __vt__12TAfterEffect
__vt__12TAfterEffect:
	.incbin "baserom.dol", 0x3AFA3C, 0x24
.global vl$localstatic0$makeDL__Q219TMBindShadowManager26TSetup1$2172ShadowUtil_cppFv
vl$localstatic0$makeDL__Q219TMBindShadowManager26TSetup1$2172ShadowUtil_cppFv:
	.incbin "baserom.dol", 0x3AFA60, 0x10
.global fl$localstatic1$makeDL__Q219TMBindShadowManager26TSetup1$2172ShadowUtil_cppFv
fl$localstatic1$makeDL__Q219TMBindShadowManager26TSetup1$2172ShadowUtil_cppFv:
	.incbin "baserom.dol", 0x3AFA70, 0x20
.global __vt__19TMBindShadowManager
__vt__19TMBindShadowManager:
	.incbin "baserom.dol", 0x3AFA90, 0x24
.global __vt__Q219TMBindShadowManager26TSetup5$2216ShadowUtil_cpp
__vt__Q219TMBindShadowManager26TSetup5$2216ShadowUtil_cpp:
	.incbin "baserom.dol", 0x3AFAB4, 0x10
.global __vt__Q219TMBindShadowManager26TSetup4$2207ShadowUtil_cpp
__vt__Q219TMBindShadowManager26TSetup4$2207ShadowUtil_cpp:
	.incbin "baserom.dol", 0x3AFAC4, 0x10
.global __vt__Q219TMBindShadowManager26TSetup3$2195ShadowUtil_cpp
__vt__Q219TMBindShadowManager26TSetup3$2195ShadowUtil_cpp:
	.incbin "baserom.dol", 0x3AFAD4, 0x10
.global __vt__Q219TMBindShadowManager26TSetup2$2190ShadowUtil_cpp
__vt__Q219TMBindShadowManager26TSetup2$2190ShadowUtil_cpp:
	.incbin "baserom.dol", 0x3AFAE4, 0x10
.global __vt__Q219TMBindShadowManager28TCylinder$2171ShadowUtil_cpp
__vt__Q219TMBindShadowManager28TCylinder$2171ShadowUtil_cpp:
	.incbin "baserom.dol", 0x3AFAF4, 0x10
.global __vt__Q219TMBindShadowManager26TSetup1$2172ShadowUtil_cpp
__vt__Q219TMBindShadowManager26TSetup1$2172ShadowUtil_cpp:
	.incbin "baserom.dol", 0x3AFB04, 0x1C
.global NullTexData
NullTexData:
	.incbin "baserom.dol", 0x3AFB20, 0x10
.global IdentityMtx
IdentityMtx:
	.incbin "baserom.dol", 0x3AFB30, 0x30
.global rumbleFrame_00
rumbleFrame_00:
	.incbin "baserom.dol", 0x3AFB60, 0x2C
.global rumblePower_00
rumblePower_00:
	.incbin "baserom.dol", 0x3AFB8C, 0x2C
.global rumbleFrame_01
rumbleFrame_01:
	.incbin "baserom.dol", 0x3AFBB8, 0x60
.global rumblePower_01
rumblePower_01:
	.incbin "baserom.dol", 0x3AFC18, 0x60
.global rumbleFrame_04
rumbleFrame_04:
	.incbin "baserom.dol", 0x3AFC78, 0x1C
.global rumblePower_04
rumblePower_04:
	.incbin "baserom.dol", 0x3AFC94, 0x1C
.global rumbleFrame_05
rumbleFrame_05:
	.incbin "baserom.dol", 0x3AFCB0, 0x54
.global rumblePower_05
rumblePower_05:
	.incbin "baserom.dol", 0x3AFD04, 0x54
.global rumbleFrame_06
rumbleFrame_06:
	.incbin "baserom.dol", 0x3AFD58, 0x28
.global rumblePower_06
rumblePower_06:
	.incbin "baserom.dol", 0x3AFD80, 0x28
.global rumbleFrame_07
rumbleFrame_07:
	.incbin "baserom.dol", 0x3AFDA8, 0x50
.global rumblePower_07
rumblePower_07:
	.incbin "baserom.dol", 0x3AFDF8, 0x50
.global rumbleFrame_08
rumbleFrame_08:
	.incbin "baserom.dol", 0x3AFE48, 0x14
.global rumblePower_08
rumblePower_08:
	.incbin "baserom.dol", 0x3AFE5C, 0x14
.global rumbleFrame_09
rumbleFrame_09:
	.incbin "baserom.dol", 0x3AFE70, 0x10
.global rumblePower_09
rumblePower_09:
	.incbin "baserom.dol", 0x3AFE80, 0x10
.global rumbleFrame_19
rumbleFrame_19:
	.incbin "baserom.dol", 0x3AFE90, 0x14
.global rumblePower_19
rumblePower_19:
	.incbin "baserom.dol", 0x3AFEA4, 0x14
.global rumbleFrame_23
rumbleFrame_23:
	.incbin "baserom.dol", 0x3AFEB8, 0x134
.global rumblePower_23
rumblePower_23:
	.incbin "baserom.dol", 0x3AFFEC, 0x134
.global rumbleFrame_24
rumbleFrame_24:
	.incbin "baserom.dol", 0x3B0120, 0x94
.global rumblePower_24
rumblePower_24:
	.incbin "baserom.dol", 0x3B01B4, 0x94
.global channelDataTbl
channelDataTbl:
	.incbin "baserom.dol", 0x3B0248, 0x130
.global _info
_info:
	.incbin "baserom.dol", 0x3B0378, 0xB8
.global $$22227
$$22227:
	.incbin "baserom.dol", 0x3B0430, 0x30
.global __vt__10TGDLStatic
__vt__10TGDLStatic:
	.incbin "baserom.dol", 0x3B0460, 0x10
.global __vt__Q210TGDLStatic12TGDLSentinel
__vt__Q210TGDLStatic12TGDLSentinel:
	.incbin "baserom.dol", 0x3B0470, 0x10
.global __vt__Q24Koga8ToolData
__vt__Q24Koga8ToolData:
	.incbin "baserom.dol", 0x3B0480, 0x10
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3B0490, 0xC
.global __vt__24M3UMtxCalcSIAnmBlendQuat
__vt__24M3UMtxCalcSIAnmBlendQuat:
	.incbin "baserom.dol", 0x3B049C, 0x44
.global __vt__8M3UModel
__vt__8M3UModel:
	.incbin "baserom.dol", 0x3B04E0, 0x24
.global __vt__14M3UModelCommon
__vt__14M3UModelCommon:
	.incbin "baserom.dol", 0x3B0504, 0xC
.global __vt__33MActorAnmEach$$016J3DAnmClusterKey$$1
__vt__33MActorAnmEach$$016J3DAnmClusterKey$$1:
	.incbin "baserom.dol", 0x3B0510, 0x28
.global __vt__35MActorAnmMatEach$$015J3DAnmTevRegKey$$1
__vt__35MActorAnmMatEach$$015J3DAnmTevRegKey$$1:
	.incbin "baserom.dol", 0x3B0538, 0x28
.global __vt__32MActorAnmEach$$015J3DAnmTevRegKey$$1
__vt__32MActorAnmEach$$015J3DAnmTevRegKey$$1:
	.incbin "baserom.dol", 0x3B0560, 0x28
.global __vt__39MActorAnmMatEach$$019J3DAnmTextureSRTKey$$1
__vt__39MActorAnmMatEach$$019J3DAnmTextureSRTKey$$1:
	.incbin "baserom.dol", 0x3B0588, 0x28
.global __vt__36MActorAnmEach$$019J3DAnmTextureSRTKey$$1
__vt__36MActorAnmEach$$019J3DAnmTextureSRTKey$$1:
	.incbin "baserom.dol", 0x3B05B0, 0x28
.global __vt__36MActorAnmMatEach$$016J3DAnmTexPattern$$1
__vt__36MActorAnmMatEach$$016J3DAnmTexPattern$$1:
	.incbin "baserom.dol", 0x3B05D8, 0x28
.global __vt__33MActorAnmEach$$016J3DAnmTexPattern$$1
__vt__33MActorAnmEach$$016J3DAnmTexPattern$$1:
	.incbin "baserom.dol", 0x3B0600, 0x28
.global __vt__34MActorAnmMatEach$$014J3DAnmColorKey$$1
__vt__34MActorAnmMatEach$$014J3DAnmColorKey$$1:
	.incbin "baserom.dol", 0x3B0628, 0x28
.global __vt__31MActorAnmEach$$014J3DAnmColorKey$$1
__vt__31MActorAnmEach$$014J3DAnmColorKey$$1:
	.incbin "baserom.dol", 0x3B0650, 0x28
.global __vt__35MActorAnmEach$$018J3DAnmTransformKey$$1
__vt__35MActorAnmEach$$018J3DAnmTransformKey$$1:
	.incbin "baserom.dol", 0x3B0678, 0x28
.global __vt__12MActorAnmBlk
__vt__12MActorAnmBlk:
	.incbin "baserom.dol", 0x3B06A0, 0x28
.global __vt__12MActorAnmBrk
__vt__12MActorAnmBrk:
	.incbin "baserom.dol", 0x3B06C8, 0x28
.global __vt__12MActorAnmBpk
__vt__12MActorAnmBpk:
	.incbin "baserom.dol", 0x3B06F0, 0x28
.global __vt__12MActorAnmBtk
__vt__12MActorAnmBtk:
	.incbin "baserom.dol", 0x3B0718, 0x28
.global __vt__12MActorAnmBtp
__vt__12MActorAnmBtp:
	.incbin "baserom.dol", 0x3B0740, 0x28
.global __vt__12MActorAnmBck
__vt__12MActorAnmBck:
	.incbin "baserom.dol", 0x3B0768, 0x28
.global __vt__13MActorAnmBase
__vt__13MActorAnmBase:
	.incbin "baserom.dol", 0x3B0790, 0x28
.global __vt__8SDLModel
__vt__8SDLModel:
	.incbin "baserom.dol", 0x3B07B8, 0x20
.global __vt__12SDLMatPacket
__vt__12SDLMatPacket:
	.incbin "baserom.dol", 0x3B07D8, 0x18
.global __vt__15SampleCtrlJoint
__vt__15SampleCtrlJoint:
	.incbin "baserom.dol", 0x3B07F0, 0xC
.global __vt__18SampleCtrlMaterial
__vt__18SampleCtrlMaterial:
	.incbin "baserom.dol", 0x3B07FC, 0xC
.global __vt__15SampleCtrlShape
__vt__15SampleCtrlShape:
	.incbin "baserom.dol", 0x3B0808, 0x10
.global __vt__19SampleCtrlModelData
__vt__19SampleCtrlModelData:
	.incbin "baserom.dol", 0x3B0818, 0x10
.global __vt__10TBaseParam
__vt__10TBaseParam:
	.incbin "baserom.dol", 0x3B0828, 0x10
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3B0838, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3B0844, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3B0850, 0x10
.global __vt__21TMarioParticleManager
__vt__21TMarioParticleManager:
	.incbin "baserom.dol", 0x3B0860, 0x48
.global __vt__36TMarioEmitterCallBackBindToSRTMtxPtr
__vt__36TMarioEmitterCallBackBindToSRTMtxPtr:
	.incbin "baserom.dol", 0x3B08A8, 0x14
.global __vt__33TMarioEmitterCallBackBindToMtxPtr
__vt__33TMarioEmitterCallBackBindToMtxPtr:
	.incbin "baserom.dol", 0x3B08BC, 0x14
.global __vt__33TMarioEmitterCallBackBindToPosPtr
__vt__33TMarioEmitterCallBackBindToPosPtr:
	.incbin "baserom.dol", 0x3B08D0, 0x14
.global __vt__23TEmitterIndirectViewObj
__vt__23TEmitterIndirectViewObj:
	.incbin "baserom.dol", 0x3B08E4, 0x24
.global __vt__15TEmitterViewObj
__vt__15TEmitterViewObj:
	.incbin "baserom.dol", 0x3B0908, 0x24
.global __vt__34JPACallBackBase$$0P14JPABaseEmitter$$1
__vt__34JPACallBackBase$$0P14JPABaseEmitter$$1:
	.incbin "baserom.dol", 0x3B092C, 0x14
.global __vt__13TEventWatcher
__vt__13TEventWatcher:
	.incbin "baserom.dol", 0x3B0940, 0x28
.global __vt__32TSpcTypedInterp$$013TEventWatcher$$1
__vt__32TSpcTypedInterp$$013TEventWatcher$$1:
	.incbin "baserom.dol", 0x3B0968, 0x14
.global __vt__32TSpcTypedBinary$$013TEventWatcher$$1
__vt__32TSpcTypedBinary$$013TEventWatcher$$1:
	.incbin "baserom.dol", 0x3B097C, 0x14
.global $$22031
$$22031:
	.incbin "baserom.dol", 0x3B0990, 0x2C
.global $$22069
$$22069:
	.incbin "baserom.dol", 0x3B09BC, 0x2C
.global $$22087
$$22087:
	.incbin "baserom.dol", 0x3B09E8, 0x28
.global $$22135
$$22135:
	.incbin "baserom.dol", 0x3B0A10, 0x2C
.global $$22168
$$22168:
	.incbin "baserom.dol", 0x3B0A3C, 0x2C
.global __vt__10TGCLogoDir
__vt__10TGCLogoDir:
	.incbin "baserom.dol", 0x3B0A68, 0x6C
.global __vt__11TNintendo2D
__vt__11TNintendo2D:
	.incbin "baserom.dol", 0x3B0AD4, 0x24
.global __vt__14TGXAlphaUpdate
__vt__14TGXAlphaUpdate:
	.incbin "baserom.dol", 0x3B0AF8, 0x24
.global __vt__9TReInitGX
__vt__9TReInitGX:
	.incbin "baserom.dol", 0x3B0B1C, 0x24
.global __vt__12TSMSDrawInit
__vt__12TSMSDrawInit:
	.incbin "baserom.dol", 0x3B0B40, 0x24
.global __vt__17TJ3DSysSetViewMtx
__vt__17TJ3DSysSetViewMtx:
	.incbin "baserom.dol", 0x3B0B64, 0x24
.global __vt__11TJ3DSysFlag
__vt__11TJ3DSysFlag:
	.incbin "baserom.dol", 0x3B0B88, 0x28
.global $$23623
$$23623:
	.incbin "baserom.dol", 0x3B0BB0, 0x20
.global $$24168
$$24168:
	.incbin "baserom.dol", 0x3B0BD0, 0x34
.global $$24377
$$24377:
	.incbin "baserom.dol", 0x3B0C04, 0x30
.global $$24667
$$24667:
	.incbin "baserom.dol", 0x3B0C34, 0x34
.global $$22830
$$22830:
	.incbin "baserom.dol", 0x3B0C68, 0x38
.global __vt__12TMarDirector
__vt__12TMarDirector:
	.incbin "baserom.dol", 0x3B0CA0, 0x70
.global __vt__14TMarNameRefGen
__vt__14TMarNameRefGen:
	.incbin "baserom.dol", 0x3B0D10, 0x10
.global __vt__53TNameRefAryT$$018TStagePositionInfo$$4Q26JDrama8TNameRef$$1
__vt__53TNameRefAryT$$018TStagePositionInfo$$4Q26JDrama8TNameRef$$1:
	.incbin "baserom.dol", 0x3B0D20, 0x20
.global __vt__93TNameRefPtrAryT$$055TNameRefAryT$$020TScenarioArchiveName$$4Q26JDrama8TNameRef$$1$$4Q26JDrama8TNameRef$$1
__vt__93TNameRefPtrAryT$$055TNameRefAryT$$020TScenarioArchiveName$$4Q26JDrama8TNameRef$$1$$4Q26JDrama8TNameRef$$1:
	.incbin "baserom.dol", 0x3B0D40, 0x20
.global __vt__55TNameRefAryT$$020TScenarioArchiveName$$4Q26JDrama8TNameRef$$1
__vt__55TNameRefAryT$$020TScenarioArchiveName$$4Q26JDrama8TNameRef$$1:
	.incbin "baserom.dol", 0x3B0D60, 0x20
.global __vt__54TNameRefPtrAryT$$016TCubeGeneralInfo$$4Q26JDrama8TNameRef$$1
__vt__54TNameRefPtrAryT$$016TCubeGeneralInfo$$4Q26JDrama8TNameRef$$1:
	.incbin "baserom.dol", 0x3B0D80, 0x20
.global __vt__49TNameRefAryT$$014TCameraMapTool$$4Q26JDrama8TNameRef$$1
__vt__49TNameRefAryT$$014TCameraMapTool$$4Q26JDrama8TNameRef$$1:
	.incbin "baserom.dol", 0x3B0DA0, 0x20
.global __vt__50TNameRefAryT$$015TStageEventInfo$$4Q26JDrama8TNameRef$$1
__vt__50TNameRefAryT$$015TStageEventInfo$$4Q26JDrama8TNameRef$$1:
	.incbin "baserom.dol", 0x3B0DC0, 0x20
.global __vt__16TCubeManagerFast
__vt__16TCubeManagerFast:
	.incbin "baserom.dol", 0x3B0DE0, 0x24
.global __vt__16TCubeManagerArea
__vt__16TCubeManagerArea:
	.incbin "baserom.dol", 0x3B0E04, 0x24
.global __vt__19TCubeManagerMarioIn
__vt__19TCubeManagerMarioIn:
	.incbin "baserom.dol", 0x3B0E28, 0x24
.global __vt__Q26JDrama47TViewObjPtrListT$$09THitActor$$4Q26JDrama8TViewObj$$1
__vt__Q26JDrama47TViewObjPtrListT$$09THitActor$$4Q26JDrama8TViewObj$$1:
	.incbin "baserom.dol", 0x3B0E4C, 0x2C
.global __vt__10TSmplFader
__vt__10TSmplFader:
	.incbin "baserom.dol", 0x3B0E78, 0x30
.global __vt__13TMenuDirector
__vt__13TMenuDirector:
	.incbin "baserom.dol", 0x3B0EA8, 0x70
.global __vt__12TPerformList
__vt__12TPerformList:
	.incbin "baserom.dol", 0x3B0F18, 0x28
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3B0F40, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3B0F4C, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3B0F58, 0x10
.global SMSAASamplePattern_non
SMSAASamplePattern_non:
	.incbin "baserom.dol", 0x3B0F68, 0x18
.global SMSAASamplePattern_aa
SMSAASamplePattern_aa:
	.incbin "baserom.dol", 0x3B0F80, 0x18
.global SMSVFilter_non
SMSVFilter_non:
	.incbin "baserom.dol", 0x3B0F98, 0x8
.global SMSVFilter_flicker
SMSVFilter_flicker:
	.incbin "baserom.dol", 0x3B0FA0, 0x8
.global __vt__12TSnapTimeObj
__vt__12TSnapTimeObj:
	.incbin "baserom.dol", 0x3B0FA8, 0x28
.global __vt__11TTalkCursor
__vt__11TTalkCursor:
	.incbin "baserom.dol", 0x3B0FD0, 0x28
.global __vt__11TAlphaCatch
__vt__11TAlphaCatch:
	.incbin "baserom.dol", 0x3B0FF8, 0x24
.global __vt__13TZBufferCatch
__vt__13TZBufferCatch:
	.incbin "baserom.dol", 0x3B101C, 0x24
.global $$22633
$$22633:
	.incbin "baserom.dol", 0x3B1040, 0x34
.global $$22929
$$22929:
	.incbin "baserom.dol", 0x3B1074, 0x28
.global __vt__13TMarioGamePad
__vt__13TMarioGamePad:
	.incbin "baserom.dol", 0x3B109C, 0xC
.global __vt__20TScenarioArchiveName
__vt__20TScenarioArchiveName:
	.incbin "baserom.dol", 0x3B10A8, 0x20
.global __vt__15TStageEventInfo
__vt__15TStageEventInfo:
	.incbin "baserom.dol", 0x3B10C8, 0x20
.global shineStageTable
shineStageTable:
	.incbin "baserom.dol", 0x3B10E8, 0x40
.global exShineTable
exShineTable:
	.incbin "baserom.dol", 0x3B1128, 0x20
.global __vt__18TStagePositionInfo
__vt__18TStagePositionInfo:
	.incbin "baserom.dol", 0x3B1148, 0x20
.global __vt__8TTimeRec
__vt__8TTimeRec:
	.incbin "baserom.dol", 0x3B1168, 0xC
.global __vt__17TDrawSyncCallback
__vt__17TDrawSyncCallback:
	.incbin "baserom.dol", 0x3B1174, 0xC
.global __vt__10TTHPRender
__vt__10TTHPRender:
	.incbin "baserom.dol", 0x3B1180, 0x28
.global __vt__24TNameKuriLauncherManager
__vt__24TNameKuriLauncherManager:
	.incbin "baserom.dol", 0x3B11A8, 0x54
.global __vt__24THamuKuriLauncherManager
__vt__24THamuKuriLauncherManager:
	.incbin "baserom.dol", 0x3B11FC, 0x54
.global __vt__17TSirenaRollMapObj
__vt__17TSirenaRollMapObj:
	.incbin "baserom.dol", 0x3B1250, 0x170
.global __vt__15TCasinoRoulette
__vt__15TCasinoRoulette:
	.incbin "baserom.dol", 0x3B13C0, 0x168
.global __vt__11TSirenaGate
__vt__11TSirenaGate:
	.incbin "baserom.dol", 0x3B1528, 0x164
.global __vt__10TTakeActor
__vt__10TTakeActor:
	.incbin "baserom.dol", 0x3B168C, 0xB4
.global __vt__13TMareWManager
__vt__13TMareWManager:
	.incbin "baserom.dol", 0x3B1740, 0x58
.global __vt__13TMareMManager
__vt__13TMareMManager:
	.incbin "baserom.dol", 0x3B1798, 0x58
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3B17F0, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3B17FC, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3B1808, 0x10
.global titles
titles:
	.incbin "baserom.dol", 0x3B1818, 0x1C
.global comments
comments:
	.incbin "baserom.dol", 0x3B1834, 0x1C
.global $$21818
$$21818:
	.incbin "baserom.dol", 0x3B1850, 0x38
.global movies$2032
movies$2032:
	.incbin "baserom.dol", 0x3B1888, 0x50
.global __vt__14TMovieDirector
__vt__14TMovieDirector:
	.incbin "baserom.dol", 0x3B18D8, 0x70
.global $$23810
$$23810:
	.incbin "baserom.dol", 0x3B1948, 0x28
.global $$23898
$$23898:
	.incbin "baserom.dol", 0x3B1970, 0x28
.global $$23897
$$23897:
	.incbin "baserom.dol", 0x3B1998, 0x40
.global $$23414
$$23414:
	.incbin "baserom.dol", 0x3B19D8, 0x28
.global $$23413
$$23413:
	.incbin "baserom.dol", 0x3B1A00, 0xF4
.global __vt__24MSSTageSimpleEnvironment
__vt__24MSSTageSimpleEnvironment:
	.incbin "baserom.dol", 0x3B1AF4, 0x14
.global __vt__17MSStageCubeSwitch
__vt__17MSStageCubeSwitch:
	.incbin "baserom.dol", 0x3B1B08, 0x14
.global __vt__20MSStageCubeFadeMonte
__vt__20MSStageCubeFadeMonte:
	.incbin "baserom.dol", 0x3B1B1C, 0x14
.global __vt__15MSStageCubeFade
__vt__15MSStageCubeFade:
	.incbin "baserom.dol", 0x3B1B30, 0x14
.global __vt__20MSStageDistFadeMonte
__vt__20MSStageDistFadeMonte:
	.incbin "baserom.dol", 0x3B1B44, 0x14
.global __vt__15MSStageDistFade
__vt__15MSStageDistFade:
	.incbin "baserom.dol", 0x3B1B58, 0x14
.global __vt__7MSStage
__vt__7MSStage:
	.incbin "baserom.dol", 0x3B1B6C, 0x14
.global __vt__12TTargetArrow
__vt__12TTargetArrow:
	.incbin "baserom.dol", 0x3B1B80, 0x28
.global __vt__10TLiveActor
__vt__10TLiveActor:
	.incbin "baserom.dol", 0x3B1BA8, 0x100
.global $$23444
$$23444:
	.incbin "baserom.dol", 0x3B1CA8, 0x1B0
.global __vt__12TLiveManager
__vt__12TLiveManager:
	.incbin "baserom.dol", 0x3B1E58, 0x40
.global __vt__29TSpcTypedBinary$$010TLiveActor$$1
__vt__29TSpcTypedBinary$$010TLiveActor$$1:
	.incbin "baserom.dol", 0x3B1E98, 0x10
.global __vt__11TObjManager
__vt__11TObjManager:
	.incbin "baserom.dol", 0x3B1EA8, 0x30
.global __vt__9TObjChara
__vt__9TObjChara:
	.incbin "baserom.dol", 0x3B1ED8, 0x28
.global $$2285
$$2285:
	.incbin "baserom.dol", 0x3B1F00, 0xC
.global $$2286
$$2286:
	.incbin "baserom.dol", 0x3B1F0C, 0xC
.global $$2287
$$2287:
	.incbin "baserom.dol", 0x3B1F18, 0xC
.global $$2288
$$2288:
	.incbin "baserom.dol", 0x3B1F24, 0xC
.global $$2289
$$2289:
	.incbin "baserom.dol", 0x3B1F30, 0xC
.global $$2290
$$2290:
	.incbin "baserom.dol", 0x3B1F3C, 0xC
.global $$2291
$$2291:
	.incbin "baserom.dol", 0x3B1F48, 0xC
.global $$2292
$$2292:
	.incbin "baserom.dol", 0x3B1F54, 0xC
.global $$2293
$$2293:
	.incbin "baserom.dol", 0x3B1F60, 0xC
.global $$2294
$$2294:
	.incbin "baserom.dol", 0x3B1F6C, 0xC
.global $$2295
$$2295:
	.incbin "baserom.dol", 0x3B1F78, 0xC
.global $$2296
$$2296:
	.incbin "baserom.dol", 0x3B1F84, 0xC
.global $$2297
$$2297:
	.incbin "baserom.dol", 0x3B1F90, 0xC
.global $$2298
$$2298:
	.incbin "baserom.dol", 0x3B1F9C, 0xC
.global $$2299
$$2299:
	.incbin "baserom.dol", 0x3B1FA8, 0xC
.global $$2300
$$2300:
	.incbin "baserom.dol", 0x3B1FB4, 0xC
.global $$2301
$$2301:
	.incbin "baserom.dol", 0x3B1FC0, 0xC
.global $$2302
$$2302:
	.incbin "baserom.dol", 0x3B1FCC, 0xC
.global $$2303
$$2303:
	.incbin "baserom.dol", 0x3B1FD8, 0xC
.global $$2304
$$2304:
	.incbin "baserom.dol", 0x3B1FE4, 0xC
.global $$2305
$$2305:
	.incbin "baserom.dol", 0x3B1FF0, 0xC
.global $$2306
$$2306:
	.incbin "baserom.dol", 0x3B1FFC, 0xC
.global $$2307
$$2307:
	.incbin "baserom.dol", 0x3B2008, 0xC
.global $$2308
$$2308:
	.incbin "baserom.dol", 0x3B2014, 0xC
.global $$2309
$$2309:
	.incbin "baserom.dol", 0x3B2020, 0xC
.global $$2310
$$2310:
	.incbin "baserom.dol", 0x3B202C, 0xC
.global $$2311
$$2311:
	.incbin "baserom.dol", 0x3B2038, 0xC
.global $$2312
$$2312:
	.incbin "baserom.dol", 0x3B2044, 0xC
.global $$2313
$$2313:
	.incbin "baserom.dol", 0x3B2050, 0xC
.global $$2314
$$2314:
	.incbin "baserom.dol", 0x3B205C, 0xC
.global $$2315
$$2315:
	.incbin "baserom.dol", 0x3B2068, 0xC
.global $$2316
$$2316:
	.incbin "baserom.dol", 0x3B2074, 0xC
.global $$2317
$$2317:
	.incbin "baserom.dol", 0x3B2080, 0xC
.global $$2318
$$2318:
	.incbin "baserom.dol", 0x3B208C, 0xC
.global $$2319
$$2319:
	.incbin "baserom.dol", 0x3B2098, 0xC
.global $$2320
$$2320:
	.incbin "baserom.dol", 0x3B20A4, 0xC
.global $$2321
$$2321:
	.incbin "baserom.dol", 0x3B20B0, 0xC
.global $$2322
$$2322:
	.incbin "baserom.dol", 0x3B20BC, 0xC
.global $$2323
$$2323:
	.incbin "baserom.dol", 0x3B20C8, 0xC
.global $$2324
$$2324:
	.incbin "baserom.dol", 0x3B20D4, 0xC
.global table$284
table$284:
	.incbin "baserom.dol", 0x3B20E0, 0x1E0
.global __vt__10TSpcInterp
__vt__10TSpcInterp:
	.incbin "baserom.dol", 0x3B22C0, 0x14
.global __vt__21TSpcStack$$09TSpcSlice$$1
__vt__21TSpcStack$$09TSpcSlice$$1:
	.incbin "baserom.dol", 0x3B22D4, 0xC
.global __vt__13TSpcStack$$0Ul$$1
__vt__13TSpcStack$$0Ul$$1:
	.incbin "baserom.dol", 0x3B22E0, 0xC
.global __vt__10TSpcBinary
__vt__10TSpcBinary:
	.incbin "baserom.dol", 0x3B22EC, 0x14
.global __vt__9TStrategy
__vt__9TStrategy:
	.incbin "baserom.dol", 0x3B2300, 0x24
.global __vt__12TIdxGroupObj
__vt__12TIdxGroupObj:
	.incbin "baserom.dol", 0x3B2324, 0x2C
.global __vt__16TQuestionManager
__vt__16TQuestionManager:
	.incbin "baserom.dol", 0x3B2350, 0x28
.global __vt__13TSMSSmplChara
__vt__13TSMSSmplChara:
	.incbin "baserom.dol", 0x3B2378, 0x28
.global __vt__9THitActor
__vt__9THitActor:
	.incbin "baserom.dol", 0x3B23A0, 0xA8
.global __vt__7TBinder
__vt__7TBinder:
	.incbin "baserom.dol", 0x3B2448, 0x10
.global __vt__12TSharedParts
__vt__12TSharedParts:
	.incbin "baserom.dol", 0x3B2458, 0x28
.global __vt__12TMirrorActor
__vt__12TMirrorActor:
	.incbin "baserom.dol", 0x3B2480, 0x28
.global __vt__13M3UModelMario
__vt__13M3UModelMario:
	.incbin "baserom.dol", 0x3B24A8, 0x28
.global __vt__9TMarioCap
__vt__9TMarioCap:
	.incbin "baserom.dol", 0x3B24D0, 0x10
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3B24E0, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3B24EC, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3B24F8, 0x10
.global MtxCalcTypeName
MtxCalcTypeName:
	.incbin "baserom.dol", 0x3B2508, 0x10
.global marioAnimeFiles
marioAnimeFiles:
	.incbin "baserom.dol", 0x3B2518, 0x638
.global gMarioAnimeData
gMarioAnimeData:
	.incbin "baserom.dol", 0x3B2B50, 0xA80
.global marioAnimeTexPatternFilenames
marioAnimeTexPatternFilenames:
	.incbin "baserom.dol", 0x3B35D0, 0x60
.global $$25888
$$25888:
	.incbin "baserom.dol", 0x3B3630, 0x90
.global __vt__19M3UModelCommonMario
__vt__19M3UModelCommonMario:
	.incbin "baserom.dol", 0x3B36C0, 0x10
.global $$24631
$$24631:
	.incbin "baserom.dol", 0x3B36D0, 0x28
.global $$25707
$$25707:
	.incbin "baserom.dol", 0x3B36F8, 0x28
.global $$26663
$$26663:
	.incbin "baserom.dol", 0x3B3720, 0x24
.global $$26662
$$26662:
	.incbin "baserom.dol", 0x3B3744, 0x2C
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3B3770, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3B377C, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3B3788, 0x10
.global MtxCalcTypeName
MtxCalcTypeName:
	.incbin "baserom.dol", 0x3B3798, 0x10
.global param$3393
param$3393:
	.incbin "baserom.dol", 0x3B37A8, 0x14
.global param$3396
param$3396:
	.incbin "baserom.dol", 0x3B37BC, 0x14
.global param$3399
param$3399:
	.incbin "baserom.dol", 0x3B37D0, 0x14
.global param$3402
param$3402:
	.incbin "baserom.dol", 0x3B37E4, 0x14
.global param$3407
param$3407:
	.incbin "baserom.dol", 0x3B37F8, 0x14
.global param$3410
param$3410:
	.incbin "baserom.dol", 0x3B380C, 0x14
.global cParticleFileNames
cParticleFileNames:
	.incbin "baserom.dol", 0x3B3820, 0xC
.global __vt__15TWarpInCallBack
__vt__15TWarpInCallBack:
	.incbin "baserom.dol", 0x3B382C, 0x10
.global $$23223
$$23223:
	.incbin "baserom.dol", 0x3B383C, 0x28
.global __vt__15TBubbleCallBack
__vt__15TBubbleCallBack:
	.incbin "baserom.dol", 0x3B3864, 0x10
.global __vt__54JPACallBackBase2$$0P14JPABaseEmitter$$4P15JPABaseParticle$$1
__vt__54JPACallBackBase2$$0P14JPABaseEmitter$$4P15JPABaseParticle$$1:
	.incbin "baserom.dol", 0x3B3874, 0x14
.global __vt__14TSplashManager
__vt__14TSplashManager:
	.incbin "baserom.dol", 0x3B3888, 0x28
.global __vt__12TYoshiTongue
__vt__12TYoshiTongue:
	.incbin "baserom.dol", 0x3B38B0, 0xC4
.global $$22843
$$22843:
	.incbin "baserom.dol", 0x3B3974, 0x24
.global nozzleBmdData
nozzleBmdData:
	.incbin "baserom.dol", 0x3B3998, 0xA8
.global __vt__9TWaterGun
__vt__9TWaterGun:
	.incbin "baserom.dol", 0x3B3A40, 0xC
.global $$24827
$$24827:
	.incbin "baserom.dol", 0x3B3A4C, 0x24
.global __vt__11TParamRT$$0l$$1
__vt__11TParamRT$$0l$$1:
	.incbin "baserom.dol", 0x3B3A70, 0xC
.global __vt__10TParamT$$0l$$1
__vt__10TParamT$$0l$$1:
	.incbin "baserom.dol", 0x3B3A7C, 0xC
.global __vt__12TParamRT$$0Uc$$1
__vt__12TParamRT$$0Uc$$1:
	.incbin "baserom.dol", 0x3B3A88, 0xC
.global __vt__11TParamT$$0Uc$$1
__vt__11TParamT$$0Uc$$1:
	.incbin "baserom.dol", 0x3B3A94, 0xC
.global __vt__11TParamRT$$0s$$1
__vt__11TParamRT$$0s$$1:
	.incbin "baserom.dol", 0x3B3AA0, 0xC
.global __vt__10TParamT$$0s$$1
__vt__10TParamT$$0s$$1:
	.incbin "baserom.dol", 0x3B3AAC, 0xC
.global __vt__13TNozzleDeform
__vt__13TNozzleDeform:
	.incbin "baserom.dol", 0x3B3AB8, 0x28
.global __vt__14TNozzleTrigger
__vt__14TNozzleTrigger:
	.incbin "baserom.dol", 0x3B3AE0, 0x28
.global __vt__11TNozzleBase
__vt__11TNozzleBase:
	.incbin "baserom.dol", 0x3B3B08, 0x28
.global $$23436
$$23436:
	.incbin "baserom.dol", 0x3B3B30, 0x24
.global $$23802
$$23802:
	.incbin "baserom.dol", 0x3B3B54, 0x24
.global __vt__12TMarioEffect
__vt__12TMarioEffect:
	.incbin "baserom.dol", 0x3B3B78, 0xA8
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3B3C20, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3B3C2C, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3B3C38, 0x10
.global __vt__6TMario
__vt__6TMario:
	.incbin "baserom.dol", 0x3B3C48, 0x218
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3B3E60, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3B3E6C, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3B3E78, 0x10
.global MtxCalcTypeName
MtxCalcTypeName:
	.incbin "baserom.dol", 0x3B3E88, 0x10
.global gModelWaterManagerWaterColor
gModelWaterManagerWaterColor:
	.incbin "baserom.dol", 0x3B3E98, 0x10
.global prmNames
prmNames:
	.incbin "baserom.dol", 0x3B3EA8, 0x44
.global gWaterManagerPlaneInfo
gWaterManagerPlaneInfo:
	.incbin "baserom.dol", 0x3B3EEC, 0x34
.global tmp_data
tmp_data:
	.incbin "baserom.dol", 0x3B3F20, 0x10CC
.global __vt__18TModelWaterManager
__vt__18TModelWaterManager:
	.incbin "baserom.dol", 0x3B4FEC, 0x14C
.global __vt__17TMarioPositionObj
__vt__17TMarioPositionObj:
	.incbin "baserom.dol", 0x3B5138, 0x28
.global $$24230
$$24230:
	.incbin "baserom.dol", 0x3B5160, 0xF0
.global $$23884
$$23884:
	.incbin "baserom.dol", 0x3B5250, 0x54
.global $$23883
$$23883:
	.incbin "baserom.dol", 0x3B52A4, 0x50
.global $$23880
$$23880:
	.incbin "baserom.dol", 0x3B52F4, 0x5C
.global $$24719
$$24719:
	.incbin "baserom.dol", 0x3B5350, 0x58
.global __vt__8TBaseNPC
__vt__8TBaseNPC:
	.incbin "baserom.dol", 0x3B53A8, 0x114
.global $$23445
$$23445:
	.incbin "baserom.dol", 0x3B54BC, 0x58
.global $$23991
$$23991:
	.incbin "baserom.dol", 0x3B5514, 0x28
.global $$23990
$$23990:
	.incbin "baserom.dol", 0x3B553C, 0x5C
.global $$24128
$$24128:
	.incbin "baserom.dol", 0x3B5598, 0x5C
.global $$24190
$$24190:
	.incbin "baserom.dol", 0x3B55F4, 0x5C
.global __vt__18TSunflowerSManager
__vt__18TSunflowerSManager:
	.incbin "baserom.dol", 0x3B5650, 0x58
.global $$22899
$$22899:
	.incbin "baserom.dol", 0x3B56A8, 0x38
.global __vt__17TMareWBaseManager
__vt__17TMareWBaseManager:
	.incbin "baserom.dol", 0x3B56E0, 0x58
.global __vt__17TMareMBaseManager
__vt__17TMareMBaseManager:
	.incbin "baserom.dol", 0x3B5738, 0x58
.global __vt__15TMonteWBManager
__vt__15TMonteWBManager:
	.incbin "baserom.dol", 0x3B5790, 0x58
.global __vt__15TMonteWAManager
__vt__15TMonteWAManager:
	.incbin "baserom.dol", 0x3B57E8, 0x58
.global __vt__14TMonteWManager
__vt__14TMonteWManager:
	.incbin "baserom.dol", 0x3B5840, 0x58
.global __vt__15TMonteMEManager
__vt__15TMonteMEManager:
	.incbin "baserom.dol", 0x3B5898, 0x58
.global __vt__15TMonteMDManager
__vt__15TMonteMDManager:
	.incbin "baserom.dol", 0x3B58F0, 0x58
.global __vt__15TMonteMCManager
__vt__15TMonteMCManager:
	.incbin "baserom.dol", 0x3B5948, 0x58
.global __vt__15TMonteMBManager
__vt__15TMonteMBManager:
	.incbin "baserom.dol", 0x3B59A0, 0x58
.global __vt__15TMonteMAManager
__vt__15TMonteMAManager:
	.incbin "baserom.dol", 0x3B59F8, 0x58
.global __vt__14TMonteMManager
__vt__14TMonteMManager:
	.incbin "baserom.dol", 0x3B5A50, 0x58
.global __vt__18TSunflowerLManager
__vt__18TSunflowerLManager:
	.incbin "baserom.dol", 0x3B5AA8, 0x58
.global __vt__18TRaccoonDogManager
__vt__18TRaccoonDogManager:
	.incbin "baserom.dol", 0x3B5B00, 0x58
.global __vt__13TPeachManager
__vt__13TPeachManager:
	.incbin "baserom.dol", 0x3B5B58, 0x58
.global __vt__15TKinojiiManager
__vt__15TKinojiiManager:
	.incbin "baserom.dol", 0x3B5BB0, 0x58
.global __vt__15TKinopioManager
__vt__15TKinopioManager:
	.incbin "baserom.dol", 0x3B5C08, 0x58
.global __vt__14TMareWBManager
__vt__14TMareWBManager:
	.incbin "baserom.dol", 0x3B5C60, 0x58
.global __vt__14TMareWAManager
__vt__14TMareWAManager:
	.incbin "baserom.dol", 0x3B5CB8, 0x58
.global __vt__14TMareMDManager
__vt__14TMareMDManager:
	.incbin "baserom.dol", 0x3B5D10, 0x58
.global __vt__14TMareMCManager
__vt__14TMareMCManager:
	.incbin "baserom.dol", 0x3B5D68, 0x58
.global __vt__14TMareMBManager
__vt__14TMareMBManager:
	.incbin "baserom.dol", 0x3B5DC0, 0x58
.global __vt__14TMareMAManager
__vt__14TMareMAManager:
	.incbin "baserom.dol", 0x3B5E18, 0x58
.global __vt__15TMonteWCManager
__vt__15TMonteWCManager:
	.incbin "baserom.dol", 0x3B5E70, 0x58
.global __vt__15TMonteMHManager
__vt__15TMonteMHManager:
	.incbin "baserom.dol", 0x3B5EC8, 0x58
.global __vt__15TMonteMGManager
__vt__15TMonteMGManager:
	.incbin "baserom.dol", 0x3B5F20, 0x58
.global __vt__15TMonteMFManager
__vt__15TMonteMFManager:
	.incbin "baserom.dol", 0x3B5F78, 0x58
.global __vt__21TMonteWSpecialManager
__vt__21TMonteWSpecialManager:
	.incbin "baserom.dol", 0x3B5FD0, 0x58
.global __vt__21TMonteMSpecialManager
__vt__21TMonteMSpecialManager:
	.incbin "baserom.dol", 0x3B6028, 0x58
.global __vt__18TMonteWBaseManager
__vt__18TMonteWBaseManager:
	.incbin "baserom.dol", 0x3B6080, 0x58
.global __vt__18TMonteMBaseManager
__vt__18TMonteMBaseManager:
	.incbin "baserom.dol", 0x3B60D8, 0x58
.global __vt__16TMareBaseManager
__vt__16TMareBaseManager:
	.incbin "baserom.dol", 0x3B6130, 0x58
.global __vt__11TNPCManager
__vt__11TNPCManager:
	.incbin "baserom.dol", 0x3B6188, 0x58
.global __vt__21TMareJellyFishManager
__vt__21TMareJellyFishManager:
	.incbin "baserom.dol", 0x3B61E0, 0x30
.global __vt__16TBoardNpcManager
__vt__16TBoardNpcManager:
	.incbin "baserom.dol", 0x3B6210, 0x40
.global __vt__18TNerveNPCMareStand
__vt__18TNerveNPCMareStand:
	.incbin "baserom.dol", 0x3B6250, 0x10
.global __vt__24TNerveBase$$010TLiveActor$$1
__vt__24TNerveBase$$010TLiveActor$$1:
	.incbin "baserom.dol", 0x3B6260, 0x10
.global __vt__14TNerveNPCBlown
__vt__14TNerveNPCBlown:
	.incbin "baserom.dol", 0x3B6270, 0x10
.global __vt__12TNerveNPCMad
__vt__12TNerveNPCMad:
	.incbin "baserom.dol", 0x3B6280, 0x10
.global __vt__14TNerveNPCThrow
__vt__14TNerveNPCThrow:
	.incbin "baserom.dol", 0x3B6290, 0x10
.global __vt__13TNerveNPCTalk
__vt__13TNerveNPCTalk:
	.incbin "baserom.dol", 0x3B62A0, 0x10
.global __vt__30TNerveNPCSetPosAfterSinkBottom
__vt__30TNerveNPCSetPosAfterSinkBottom:
	.incbin "baserom.dol", 0x3B62B0, 0x10
.global __vt__21TNerveNPCRecoverAfter
__vt__21TNerveNPCRecoverAfter:
	.incbin "baserom.dol", 0x3B62C0, 0x10
.global __vt__24TNerveNPCRecoverFromSink
__vt__24TNerveNPCRecoverFromSink:
	.incbin "baserom.dol", 0x3B62D0, 0x10
.global __vt__13TNerveNPCSink
__vt__13TNerveNPCSink:
	.incbin "baserom.dol", 0x3B62E0, 0x10
.global __vt__12TNerveNPCWet
__vt__12TNerveNPCWet:
	.incbin "baserom.dol", 0x3B62F0, 0x10
.global __vt__20TNerveNPCTurnToMario
__vt__20TNerveNPCTurnToMario:
	.incbin "baserom.dol", 0x3B6300, 0x10
.global __vt__26TNerveNPCWaitMarioApproach
__vt__26TNerveNPCWaitMarioApproach:
	.incbin "baserom.dol", 0x3B6310, 0x10
.global __vt__21TNerveNPCWaitContinue
__vt__21TNerveNPCWaitContinue:
	.incbin "baserom.dol", 0x3B6320, 0x10
.global __vt__18TNerveNPCGraphWait
__vt__18TNerveNPCGraphWait:
	.incbin "baserom.dol", 0x3B6330, 0x10
.global __vt__14TNerveNPCUTurn
__vt__14TNerveNPCUTurn:
	.incbin "baserom.dol", 0x3B6340, 0x10
.global __vt__20TNerveNPCGraphWander
__vt__20TNerveNPCGraphWander:
	.incbin "baserom.dol", 0x3B6350, 0x10
.global sSaveFileName$1960
sSaveFileName$1960:
	.incbin "baserom.dol", 0x3B6360, 0x78
.global sCameraNames$2626
sCameraNames$2626:
	.incbin "baserom.dol", 0x3B63D8, 0x18
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3B63F0, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3B63FC, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3B6408, 0x10
.global sMareMB_FishingRodData
sMareMB_FishingRodData:
	.incbin "baserom.dol", 0x3B6418, 0x2C
.global sMareWB_BabyData
sMareWB_BabyData:
	.incbin "baserom.dol", 0x3B6444, 0x2C
.global sAllNpcInitData$1622
sAllNpcInitData$1622:
	.incbin "baserom.dol", 0x3B6470, 0x78
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3B64E8, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3B64F4, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3B6500, 0x10
.global MtxCalcTypeName
MtxCalcTypeName:
	.incbin "baserom.dol", 0x3B6510, 0x10
.global monteMCommon_bastable
monteMCommon_bastable:
	.incbin "baserom.dol", 0x3B6520, 0x5C
.global monteMCommon_bas_c_table
monteMCommon_bas_c_table:
	.incbin "baserom.dol", 0x3B657C, 0x5C
.global monteME_bastable
monteME_bastable:
	.incbin "baserom.dol", 0x3B65D8, 0x1C
.global monteME_bas_c_table
monteME_bas_c_table:
	.incbin "baserom.dol", 0x3B65F4, 0x1C
.global monteMF_bastable
monteMF_bastable:
	.incbin "baserom.dol", 0x3B6610, 0xC
.global monteMF_bas_c_table
monteMF_bas_c_table:
	.incbin "baserom.dol", 0x3B661C, 0xC
.global monteMH_bastable
monteMH_bastable:
	.incbin "baserom.dol", 0x3B6628, 0x10
.global monteMH_bas_c_table
monteMH_bas_c_table:
	.incbin "baserom.dol", 0x3B6638, 0x10
.global monteWCommon_bastable
monteWCommon_bastable:
	.incbin "baserom.dol", 0x3B6648, 0x60
.global monteWCommon_bas_c_table
monteWCommon_bas_c_table:
	.incbin "baserom.dol", 0x3B66A8, 0x60
.global monteWC_bastable
monteWC_bastable:
	.incbin "baserom.dol", 0x3B6708, 0x14
.global monteWC_bas_c_table
monteWC_bas_c_table:
	.incbin "baserom.dol", 0x3B671C, 0x14
.global mareM_bastable
mareM_bastable:
	.incbin "baserom.dol", 0x3B6730, 0x54
.global mareM_bas_c_table
mareM_bas_c_table:
	.incbin "baserom.dol", 0x3B6784, 0x54
.global mareMA_bastable
mareMA_bastable:
	.incbin "baserom.dol", 0x3B67D8, 0x14
.global mareMA_bas_c_table
mareMA_bas_c_table:
	.incbin "baserom.dol", 0x3B67EC, 0x14
.global mareMB_bastable
mareMB_bastable:
	.incbin "baserom.dol", 0x3B6800, 0x18
.global mareMB_bas_c_table
mareMB_bas_c_table:
	.incbin "baserom.dol", 0x3B6818, 0x18
.global mareMC_bastable
mareMC_bastable:
	.incbin "baserom.dol", 0x3B6830, 0x14
.global mareMC_bas_c_table
mareMC_bas_c_table:
	.incbin "baserom.dol", 0x3B6844, 0x14
.global mareW_bastable
mareW_bastable:
	.incbin "baserom.dol", 0x3B6858, 0x50
.global mareW_bas_c_table
mareW_bas_c_table:
	.incbin "baserom.dol", 0x3B68A8, 0x50
.global mareWA_bastable
mareWA_bastable:
	.incbin "baserom.dol", 0x3B68F8, 0x10
.global mareWA_bas_c_table
mareWA_bas_c_table:
	.incbin "baserom.dol", 0x3B6908, 0x10
.global mareWB_bastable
mareWB_bastable:
	.incbin "baserom.dol", 0x3B6918, 0x18
.global mareWB_bas_c_table
mareWB_bas_c_table:
	.incbin "baserom.dol", 0x3B6930, 0x18
.global kinopio_bastable
kinopio_bastable:
	.incbin "baserom.dol", 0x3B6948, 0x64
.global kinopio_bas_c_table
kinopio_bas_c_table:
	.incbin "baserom.dol", 0x3B69AC, 0x64
.global kinojii_bastable
kinojii_bastable:
	.incbin "baserom.dol", 0x3B6A10, 0x48
.global kinojii_bas_c_table
kinojii_bas_c_table:
	.incbin "baserom.dol", 0x3B6A58, 0x48
.global peach_bastable
peach_bastable:
	.incbin "baserom.dol", 0x3B6AA0, 0x5C
.global peach_bas_c_table
peach_bas_c_table:
	.incbin "baserom.dol", 0x3B6AFC, 0x5C
.global raccoonDog_bastable
raccoonDog_bastable:
	.incbin "baserom.dol", 0x3B6B58, 0x10
.global raccoonDog_bas_c_table
raccoonDog_bas_c_table:
	.incbin "baserom.dol", 0x3B6B68, 0x10
.global sunflowerL_bastable
sunflowerL_bastable:
	.incbin "baserom.dol", 0x3B6B78, 0x14
.global sunflowerL_bas_c_table
sunflowerL_bas_c_table:
	.incbin "baserom.dol", 0x3B6B8C, 0x14
.global sunflowerS_bastable
sunflowerS_bastable:
	.incbin "baserom.dol", 0x3B6BA0, 0x14
.global sunflowerS_bas_c_table
sunflowerS_bas_c_table:
	.incbin "baserom.dol", 0x3B6BB4, 0x14
.global $$22235
$$22235:
	.incbin "baserom.dol", 0x3B6BC8, 0x60
.global __vt__12MAnmSoundNPC
__vt__12MAnmSoundNPC:
	.incbin "baserom.dol", 0x3B6C28, 0x10
.global __vt__9MAnmSound
__vt__9MAnmSound:
	.incbin "baserom.dol", 0x3B6C38, 0x10
.global smTrackCategory__12MSSeCallBack
smTrackCategory__12MSSeCallBack:
	.incbin "baserom.dol", 0x3B6C48, 0x40
.global smPolifonic__12MSSeCallBack
smPolifonic__12MSSeCallBack:
	.incbin "baserom.dol", 0x3B6C88, 0x10
.global __vt__6MSound
__vt__6MSound:
	.incbin "baserom.dol", 0x3B6C98, 0x38
.global $$22858
$$22858:
	.incbin "baserom.dol", 0x3B6CD0, 0x58
.global $$22331
$$22331:
	.incbin "baserom.dol", 0x3B6D28, 0xC0
.global __vt__Q214MSoundSESystem10MSRandPlay
__vt__Q214MSoundSESystem10MSRandPlay:
	.incbin "baserom.dol", 0x3B6DE8, 0xC
.global $$22334
$$22334:
	.incbin "baserom.dol", 0x3B6DF4, 0x1C
.global $$22460
$$22460:
	.incbin "baserom.dol", 0x3B6E10, 0x54
.global $$22459
$$22459:
	.incbin "baserom.dol", 0x3B6E64, 0x1C
.global __vt__29MSSetSoundTL$$013MSSetSoundGrp$$1
__vt__29MSSetSoundTL$$013MSSetSoundGrp$$1:
	.incbin "baserom.dol", 0x3B6E80, 0x18
.global __vt__13MSSetSoundGrp
__vt__13MSSetSoundGrp:
	.incbin "baserom.dol", 0x3B6E98, 0x18
.global __vt__Q214MSoundSESystem9MSRandVol
__vt__Q214MSoundSESystem9MSRandVol:
	.incbin "baserom.dol", 0x3B6EB0, 0xC
.global __vt__33JALListFrameLoop$$013MSSetSoundGrp$$1
__vt__33JALListFrameLoop$$013MSSetSoundGrp$$1:
	.incbin "baserom.dol", 0x3B6EBC, 0xC
.global __vt__26MSSetSoundTL$$010MSSetSound$$1
__vt__26MSSetSoundTL$$010MSSetSound$$1:
	.incbin "baserom.dol", 0x3B6EC8, 0x18
.global __vt__10MSSetSound
__vt__10MSSetSound:
	.incbin "baserom.dol", 0x3B6EE0, 0x18
.global __vt__30JALListFrameLoop$$010MSSetSound$$1
__vt__30JALListFrameLoop$$010MSSetSound$$1:
	.incbin "baserom.dol", 0x3B6EF8, 0x10
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3B6F08, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3B6F14, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3B6F20, 0x10
.global smACosPrm__8MSHandle
smACosPrm__8MSHandle:
	.incbin "baserom.dol", 0x3B6F30, 0x194
.global smSeCategory__8MSHandle
smSeCategory__8MSHandle:
	.incbin "baserom.dol", 0x3B70C4, 0x100
.global __vt__8MSHandle
__vt__8MSHandle:
	.incbin "baserom.dol", 0x3B71C4, 0x34
.global $$21960
$$21960:
	.incbin "baserom.dol", 0x3B71F8, 0x20
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3B7218, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3B7224, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3B7230, 0x10
.global scTiming__10MSBgmXFade
scTiming__10MSBgmXFade:
	.incbin "baserom.dol", 0x3B7240, 0x48
.global scExp__10MSBgmXFade
scExp__10MSBgmXFade:
	.incbin "baserom.dol", 0x3B7288, 0x48
.global __vt__11TWoodBarrel
__vt__11TWoodBarrel:
	.incbin "baserom.dol", 0x3B72D0, 0x1E0
.global __vt__11TMapObjBase
__vt__11TMapObjBase:
	.incbin "baserom.dol", 0x3B74B0, 0x168
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3B7618, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3B7624, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3B7630, 0x10
.global MtxCalcTypeName
MtxCalcTypeName:
	.incbin "baserom.dol", 0x3B7640, 0x10
.global mDefaultSound__14TMapObjGeneral
mDefaultSound__14TMapObjGeneral:
	.incbin "baserom.dol", 0x3B7650, 0x28
.global normal_sound_data
normal_sound_data:
	.incbin "baserom.dol", 0x3B7678, 0x28
.global end_data
end_data:
	.incbin "baserom.dol", 0x3B76A0, 0x3C
.global no_data
no_data:
	.incbin "baserom.dol", 0x3B76DC, 0x3C
.global billboard_dolphin_data
billboard_dolphin_data:
	.incbin "baserom.dol", 0x3B7718, 0x3C
.global billboard_sun_data
billboard_sun_data:
	.incbin "baserom.dol", 0x3B7754, 0x3C
.global billboard_restaurant_data
billboard_restaurant_data:
	.incbin "baserom.dol", 0x3B7790, 0x3C
.global billboard_fish_data
billboard_fish_data:
	.incbin "baserom.dol", 0x3B77CC, 0x3C
.global HideObj_data
HideObj_data:
	.incbin "baserom.dol", 0x3B7808, 0x3C
.global WaterHitHideObj_data
WaterHitHideObj_data:
	.incbin "baserom.dol", 0x3B7844, 0x3C
.global FruitHitHideObj_data
FruitHitHideObj_data:
	.incbin "baserom.dol", 0x3B7880, 0x3C
.global HipDropHideObj_data
HipDropHideObj_data:
	.incbin "baserom.dol", 0x3B78BC, 0x3C
.global MonteChair_data
MonteChair_data:
	.incbin "baserom.dol", 0x3B78F8, 0x3C
.global door_data
door_data:
	.incbin "baserom.dol", 0x3B7934, 0x3C
.global doorHotel_data
doorHotel_data:
	.incbin "baserom.dol", 0x3B7970, 0x3C
.global manhole_data
manhole_data:
	.incbin "baserom.dol", 0x3B79AC, 0x3C
.global FruitBasket_data
FruitBasket_data:
	.incbin "baserom.dol", 0x3B79E8, 0x3C
.global BasketReverse_data
BasketReverse_data:
	.incbin "baserom.dol", 0x3B7A24, 0x3C
.global CoconutJuice_data
CoconutJuice_data:
	.incbin "baserom.dol", 0x3B7A60, 0x3C
.global Pile_data
Pile_data:
	.incbin "baserom.dol", 0x3B7A9C, 0x3C
.global ChangeStage_data
ChangeStage_data:
	.incbin "baserom.dol", 0x3B7AD8, 0x3C
.global ChangeStageMerrygoround_data
ChangeStageMerrygoround_data:
	.incbin "baserom.dol", 0x3B7B14, 0x3C
.global StartDemo_data
StartDemo_data:
	.incbin "baserom.dol", 0x3B7B50, 0x3C
.global ChipShine_data
ChipShine_data:
	.incbin "baserom.dol", 0x3B7B8C, 0x3C
.global KoopaJrSignM_data
KoopaJrSignM_data:
	.incbin "baserom.dol", 0x3B7BC8, 0x3C
.global StarSign_data
StarSign_data:
	.incbin "baserom.dol", 0x3B7C04, 0x3C
.global SignCircle_data
SignCircle_data:
	.incbin "baserom.dol", 0x3B7C40, 0x3C
.global SignCross_data
SignCross_data:
	.incbin "baserom.dol", 0x3B7C7C, 0x3C
.global SignTriangle_data
SignTriangle_data:
	.incbin "baserom.dol", 0x3B7CB8, 0x3C
.global ArrowBoardLR_data
ArrowBoardLR_data:
	.incbin "baserom.dol", 0x3B7CF4, 0x3C
.global ArrowBoardUp_data
ArrowBoardUp_data:
	.incbin "baserom.dol", 0x3B7D30, 0x3C
.global ArrowBoardDown_data
ArrowBoardDown_data:
	.incbin "baserom.dol", 0x3B7D6C, 0x3C
.global WaterSprayCylinder_data
WaterSprayCylinder_data:
	.incbin "baserom.dol", 0x3B7DA8, 0x3C
.global WaterSprayBox_data
WaterSprayBox_data:
	.incbin "baserom.dol", 0x3B7DE4, 0x3C
.global ObjSwitch_data
ObjSwitch_data:
	.incbin "baserom.dol", 0x3B7E20, 0x3C
.global RedCoinSwitch_data
RedCoinSwitch_data:
	.incbin "baserom.dol", 0x3B7E5C, 0x3C
.global bucket_data
bucket_data:
	.incbin "baserom.dol", 0x3B7E98, 0x3C
.global GeneralHitObj_data
GeneralHitObj_data:
	.incbin "baserom.dol", 0x3B7ED4, 0x3C
.global bottle_large_data
bottle_large_data:
	.incbin "baserom.dol", 0x3B7F10, 0x3C
.global bottle_short_data
bottle_short_data:
	.incbin "baserom.dol", 0x3B7F4C, 0x3C
.global WaterRecoverObj_data
WaterRecoverObj_data:
	.incbin "baserom.dol", 0x3B7F88, 0x3C
.global watergun_item_data
watergun_item_data:
	.incbin "baserom.dol", 0x3B7FC4, 0x3C
.global nozzle_normal_physical_data
nozzle_normal_physical_data:
	.incbin "baserom.dol", 0x3B8000, 0x34
.global nozzle_normal_physical_info
nozzle_normal_physical_info:
	.incbin "baserom.dol", 0x3B8034, 0xC
.global nozzle_normal_item_data
nozzle_normal_item_data:
	.incbin "baserom.dol", 0x3B8040, 0x3C
.global yoshi_whistle_item_data
yoshi_whistle_item_data:
	.incbin "baserom.dol", 0x3B807C, 0x3C
.global nozzle_rocket_item_data
nozzle_rocket_item_data:
	.incbin "baserom.dol", 0x3B80B8, 0x3C
.global nozzle_back_item_data
nozzle_back_item_data:
	.incbin "baserom.dol", 0x3B80F4, 0x3C
.global coin_physical_data
coin_physical_data:
	.incbin "baserom.dol", 0x3B8130, 0x34
.global coin_physical_info
coin_physical_info:
	.incbin "baserom.dol", 0x3B8164, 0xC
.global coin_data
coin_data:
	.incbin "baserom.dol", 0x3B8170, 0x3C
.global invisible_coin_data
invisible_coin_data:
	.incbin "baserom.dol", 0x3B81AC, 0x3C
.global coin_red_data
coin_red_data:
	.incbin "baserom.dol", 0x3B81E8, 0x3C
.global coin_blue_data
coin_blue_data:
	.incbin "baserom.dol", 0x3B8224, 0x3C
.global shine_data
shine_data:
	.incbin "baserom.dol", 0x3B8260, 0x3C
.global mario_cap_data
mario_cap_data:
	.incbin "baserom.dol", 0x3B829C, 0x3C
.global joint_coin_data
joint_coin_data:
	.incbin "baserom.dol", 0x3B82D8, 0x3C
.global eggYoshi_data
eggYoshi_data:
	.incbin "baserom.dol", 0x3B8314, 0x3C
.global EggYoshi_physical_data
EggYoshi_physical_data:
	.incbin "baserom.dol", 0x3B8350, 0x34
.global EggYoshi_physical_info
EggYoshi_physical_info:
	.incbin "baserom.dol", 0x3B8384, 0xC
.global eggYoshiEvent_data
eggYoshiEvent_data:
	.incbin "baserom.dol", 0x3B8390, 0x3C
.global NozzleBox_data
NozzleBox_data:
	.incbin "baserom.dol", 0x3B83CC, 0x3C
.global mushroom1up_data
mushroom1up_data:
	.incbin "baserom.dol", 0x3B8408, 0x3C
.global mushroom1upR_data
mushroom1upR_data:
	.incbin "baserom.dol", 0x3B8444, 0x3C
.global mushroom1upX_data
mushroom1upX_data:
	.incbin "baserom.dol", 0x3B8480, 0x3C
.global jumpbase_data
jumpbase_data:
	.incbin "baserom.dol", 0x3B84BC, 0x3C
.global coconut_physical_data
coconut_physical_data:
	.incbin "baserom.dol", 0x3B84F8, 0x34
.global coconut_physical_info
coconut_physical_info:
	.incbin "baserom.dol", 0x3B852C, 0xC
.global coconut_data
coconut_data:
	.incbin "baserom.dol", 0x3B8538, 0x3C
.global papaya_physical_data
papaya_physical_data:
	.incbin "baserom.dol", 0x3B8574, 0x34
.global papaya_physical_info
papaya_physical_info:
	.incbin "baserom.dol", 0x3B85A8, 0xC
.global papaya_data
papaya_data:
	.incbin "baserom.dol", 0x3B85B4, 0x3C
.global pine_physical_data
pine_physical_data:
	.incbin "baserom.dol", 0x3B85F0, 0x34
.global pine_physical_info
pine_physical_info:
	.incbin "baserom.dol", 0x3B8624, 0xC
.global pine_data
pine_data:
	.incbin "baserom.dol", 0x3B8630, 0x3C
.global CoverPine_data
CoverPine_data:
	.incbin "baserom.dol", 0x3B866C, 0x3C
.global durian_physical_data
durian_physical_data:
	.incbin "baserom.dol", 0x3B86A8, 0x34
.global durian_physical_info
durian_physical_info:
	.incbin "baserom.dol", 0x3B86DC, 0xC
.global durian_data
durian_data:
	.incbin "baserom.dol", 0x3B86E8, 0x3C
.global banana_physical_data
banana_physical_data:
	.incbin "baserom.dol", 0x3B8724, 0x34
.global banana_physical_info
banana_physical_info:
	.incbin "baserom.dol", 0x3B8758, 0xC
.global banana_data
banana_data:
	.incbin "baserom.dol", 0x3B8764, 0x3C
.global red_pepper_physical_data
red_pepper_physical_data:
	.incbin "baserom.dol", 0x3B87A0, 0x34
.global red_pepper_physical_info
red_pepper_physical_info:
	.incbin "baserom.dol", 0x3B87D4, 0xC
.global RedPepper_data
RedPepper_data:
	.incbin "baserom.dol", 0x3B87E0, 0x3C
.global fence_normal_data
fence_normal_data:
	.incbin "baserom.dol", 0x3B881C, 0x3C
.global fence3x3_data
fence3x3_data:
	.incbin "baserom.dol", 0x3B8858, 0x3C
.global fence_revolve_data
fence_revolve_data:
	.incbin "baserom.dol", 0x3B8894, 0x3C
.global fence_revolve_inner_data
fence_revolve_inner_data:
	.incbin "baserom.dol", 0x3B88D0, 0x3C
.global fenceInnerGreen_data
fenceInnerGreen_data:
	.incbin "baserom.dol", 0x3B890C, 0x3C
.global FenceWaterV_data
FenceWaterV_data:
	.incbin "baserom.dol", 0x3B8948, 0x3C
.global FenceWaterH_data
FenceWaterH_data:
	.incbin "baserom.dol", 0x3B8984, 0x3C
.global RailFence_data
RailFence_data:
	.incbin "baserom.dol", 0x3B89C0, 0x3C
.global bambooFenceRevolveInner_data
bambooFenceRevolveInner_data:
	.incbin "baserom.dol", 0x3B89FC, 0x3C
.global bambooFenceRevolveOuter_data
bambooFenceRevolveOuter_data:
	.incbin "baserom.dol", 0x3B8A38, 0x3C
.global bambooRailFence_data
bambooRailFence_data:
	.incbin "baserom.dol", 0x3B8A74, 0x3C
.global wood_barrel_physical_data
wood_barrel_physical_data:
	.incbin "baserom.dol", 0x3B8AB0, 0x34
.global wood_barrel_physical_info
wood_barrel_physical_info:
	.incbin "baserom.dol", 0x3B8AE4, 0xC
.global wood_barrel_hold_data
wood_barrel_hold_data:
	.incbin "baserom.dol", 0x3B8AF0, 0x14
.global wood_barrel_data
wood_barrel_data:
	.incbin "baserom.dol", 0x3B8B04, 0x3C
.global wood_barrel_once_data
wood_barrel_once_data:
	.incbin "baserom.dol", 0x3B8B40, 0x3C
.global barrel_float_data
barrel_float_data:
	.incbin "baserom.dol", 0x3B8B7C, 0x3C
.global drum_can_data
drum_can_data:
	.incbin "baserom.dol", 0x3B8BB8, 0x3C
.global barrel_oil_data
barrel_oil_data:
	.incbin "baserom.dol", 0x3B8BF4, 0x3C
.global breakable_block_data
breakable_block_data:
	.incbin "baserom.dol", 0x3B8C30, 0x3C
.global supermario_block_data
supermario_block_data:
	.incbin "baserom.dol", 0x3B8C6C, 0x3C
.global move_block_data
move_block_data:
	.incbin "baserom.dol", 0x3B8CA8, 0x3C
.global fall_slow_block_data
fall_slow_block_data:
	.incbin "baserom.dol", 0x3B8CE4, 0x3C
.global expand_block_data
expand_block_data:
	.incbin "baserom.dol", 0x3B8D20, 0x3C
.global spread_block_data
spread_block_data:
	.incbin "baserom.dol", 0x3B8D5C, 0x3C
.global water_roll_block_data
water_roll_block_data:
	.incbin "baserom.dol", 0x3B8D98, 0x3C
.global sand_block_data
sand_block_data:
	.incbin "baserom.dol", 0x3B8DD4, 0x3C
.global water_power_lift_data
water_power_lift_data:
	.incbin "baserom.dol", 0x3B8E10, 0x3C
.global water_power_inertial_lift_data
water_power_inertial_lift_data:
	.incbin "baserom.dol", 0x3B8E4C, 0x3C
.global water_power_ship_data
water_power_ship_data:
	.incbin "baserom.dol", 0x3B8E88, 0x3C
.global lean_direct_block_data
lean_direct_block_data:
	.incbin "baserom.dol", 0x3B8EC4, 0x3C
.global lean_indirect_block_data
lean_indirect_block_data:
	.incbin "baserom.dol", 0x3B8F00, 0x3C
.global lean_block_data
lean_block_data:
	.incbin "baserom.dol", 0x3B8F3C, 0x3C
.global skate_block_data
skate_block_data:
	.incbin "baserom.dol", 0x3B8F78, 0x3C
.global MoveCoin_data
MoveCoin_data:
	.incbin "baserom.dol", 0x3B8FB4, 0x3C
.global cluster_block_data
cluster_block_data:
	.incbin "baserom.dol", 0x3B8FF0, 0x3C
.global NormalBlock_data
NormalBlock_data:
	.incbin "baserom.dol", 0x3B902C, 0x3C
.global IceBlock_data
IceBlock_data:
	.incbin "baserom.dol", 0x3B9068, 0x3C
.global BrickBlock_data
BrickBlock_data:
	.incbin "baserom.dol", 0x3B90A4, 0x3C
.global WaterMelonBlock_data
WaterMelonBlock_data:
	.incbin "baserom.dol", 0x3B90E0, 0x3C
.global TelesaBlock_data
TelesaBlock_data:
	.incbin "baserom.dol", 0x3B911C, 0x3C
.global WoodBlockPole_data
WoodBlockPole_data:
	.incbin "baserom.dol", 0x3B9158, 0x3C
.global JuiceBlock_data
JuiceBlock_data:
	.incbin "baserom.dol", 0x3B9194, 0x3C
.global SuperHipDropBlock_data
SuperHipDropBlock_data:
	.incbin "baserom.dol", 0x3B91D0, 0x3C
.global palmNormal_data
palmNormal_data:
	.incbin "baserom.dol", 0x3B920C, 0x3C
.global palmOugi_data
palmOugi_data:
	.incbin "baserom.dol", 0x3B9248, 0x3C
.global palmSago_data
palmSago_data:
	.incbin "baserom.dol", 0x3B9284, 0x3C
.global palmNatume_data
palmNatume_data:
	.incbin "baserom.dol", 0x3B92C0, 0x3C
.global palmLeaf_data
palmLeaf_data:
	.incbin "baserom.dol", 0x3B92FC, 0x3C
.global BananaTree_data
BananaTree_data:
	.incbin "baserom.dol", 0x3B9338, 0x3C
.global FruitTree_data
FruitTree_data:
	.incbin "baserom.dol", 0x3B9374, 0x3C
.global flower_data
flower_data:
	.incbin "baserom.dol", 0x3B93B0, 0x3C
.global flowerOrange_data
flowerOrange_data:
	.incbin "baserom.dol", 0x3B93EC, 0x3C
.global flowerPink_data
flowerPink_data:
	.incbin "baserom.dol", 0x3B9428, 0x3C
.global flowerPurple_data
flowerPurple_data:
	.incbin "baserom.dol", 0x3B9464, 0x3C
.global flowerRed_data
flowerRed_data:
	.incbin "baserom.dol", 0x3B94A0, 0x3C
.global flowerYellow_data
flowerYellow_data:
	.incbin "baserom.dol", 0x3B94DC, 0x3C
.global flowerSunflower_data
flowerSunflower_data:
	.incbin "baserom.dol", 0x3B9518, 0x3C
.global telegraph_pole_l_data
telegraph_pole_l_data:
	.incbin "baserom.dol", 0x3B9554, 0x3C
.global telegraph_pole_s_data
telegraph_pole_s_data:
	.incbin "baserom.dol", 0x3B9590, 0x3C
.global streetlamp_data
streetlamp_data:
	.incbin "baserom.dol", 0x3B95CC, 0x3C
.global PoleNormal_data
PoleNormal_data:
	.incbin "baserom.dol", 0x3B9608, 0x3C
.global football_physical_data
football_physical_data:
	.incbin "baserom.dol", 0x3B9644, 0x34
.global football_physical_info
football_physical_info:
	.incbin "baserom.dol", 0x3B9678, 0xC
.global football_data
football_data:
	.incbin "baserom.dol", 0x3B9684, 0x3C
.global football_goal_data
football_goal_data:
	.incbin "baserom.dol", 0x3B96C0, 0x3C
.global baloonball_physical_data
baloonball_physical_data:
	.incbin "baserom.dol", 0x3B96FC, 0x34
.global baloonball_physical_info
baloonball_physical_info:
	.incbin "baserom.dol", 0x3B9730, 0xC
.global baloonball_data
baloonball_data:
	.incbin "baserom.dol", 0x3B973C, 0x3C
.global coconutball_data
coconutball_data:
	.incbin "baserom.dol", 0x3B9778, 0x3C
.global watermelon_physical_data
watermelon_physical_data:
	.incbin "baserom.dol", 0x3B97B4, 0x34
.global watermelon_physical_info
watermelon_physical_info:
	.incbin "baserom.dol", 0x3B97E8, 0xC
.global watermelon_data
watermelon_data:
	.incbin "baserom.dol", 0x3B97F4, 0x3C
.global WatermelonStatic_data
WatermelonStatic_data:
	.incbin "baserom.dol", 0x3B9830, 0x3C
.global cloud_data
cloud_data:
	.incbin "baserom.dol", 0x3B986C, 0x3C
.global normallift_data
normallift_data:
	.incbin "baserom.dol", 0x3B98A8, 0x3C
.global exrollcube_data
exrollcube_data:
	.incbin "baserom.dol", 0x3B98E4, 0x3C
.global exkickboard_data
exkickboard_data:
	.incbin "baserom.dol", 0x3B9920, 0x3C
.global railblockr_data
railblockr_data:
	.incbin "baserom.dol", 0x3B995C, 0x3C
.global railblocky_data
railblocky_data:
	.incbin "baserom.dol", 0x3B9998, 0x3C
.global railblockb_data
railblockb_data:
	.incbin "baserom.dol", 0x3B99D4, 0x3C
.global rollblockr_data
rollblockr_data:
	.incbin "baserom.dol", 0x3B9A10, 0x3C
.global rollblocky_data
rollblocky_data:
	.incbin "baserom.dol", 0x3B9A4C, 0x3C
.global rollblockb_data
rollblockb_data:
	.incbin "baserom.dol", 0x3B9A88, 0x3C
.global umaibou_data
umaibou_data:
	.incbin "baserom.dol", 0x3B9AC4, 0x3C
.global kamaboko_data
kamaboko_data:
	.incbin "baserom.dol", 0x3B9B00, 0x3C
.global getag_data
getag_data:
	.incbin "baserom.dol", 0x3B9B3C, 0x3C
.global getao_data
getao_data:
	.incbin "baserom.dol", 0x3B9B78, 0x3C
.global uirou_data
uirou_data:
	.incbin "baserom.dol", 0x3B9BB4, 0x3C
.global hikidashi_data
hikidashi_data:
	.incbin "baserom.dol", 0x3B9BF0, 0x3C
.global castella_data
castella_data:
	.incbin "baserom.dol", 0x3B9C2C, 0x3C
.global yoshiblock_data
yoshiblock_data:
	.incbin "baserom.dol", 0x3B9C68, 0x3C
.global WoodBlockTriangle_data
WoodBlockTriangle_data:
	.incbin "baserom.dol", 0x3B9CA4, 0x3C
.global WoodBlockPyramid_data
WoodBlockPyramid_data:
	.incbin "baserom.dol", 0x3B9CE0, 0x3C
.global WoodBlockLong_data
WoodBlockLong_data:
	.incbin "baserom.dol", 0x3B9D1C, 0x3C
.global WoodBlockLarge_data
WoodBlockLarge_data:
	.incbin "baserom.dol", 0x3B9D58, 0x3C
.global WoodBlockCone_data
WoodBlockCone_data:
	.incbin "baserom.dol", 0x3B9D94, 0x3C
.global WoodBlockL_data
WoodBlockL_data:
	.incbin "baserom.dol", 0x3B9DD0, 0x3C
.global WoodBlockBridge_data
WoodBlockBridge_data:
	.incbin "baserom.dol", 0x3B9E0C, 0x3C
.global WoodBlockCube_data
WoodBlockCube_data:
	.incbin "baserom.dol", 0x3B9E48, 0x3C
.global normalvariant0_data
normalvariant0_data:
	.incbin "baserom.dol", 0x3B9E84, 0x3C
.global normalvariant1_data
normalvariant1_data:
	.incbin "baserom.dol", 0x3B9EC0, 0x3C
.global normalvariant2_data
normalvariant2_data:
	.incbin "baserom.dol", 0x3B9EFC, 0x3C
.global normalvariant3_data
normalvariant3_data:
	.incbin "baserom.dol", 0x3B9F38, 0x3C
.global normalvariant4_data
normalvariant4_data:
	.incbin "baserom.dol", 0x3B9F74, 0x3C
.global railvariant0_data
railvariant0_data:
	.incbin "baserom.dol", 0x3B9FB0, 0x3C
.global railvariant1_data
railvariant1_data:
	.incbin "baserom.dol", 0x3B9FEC, 0x3C
.global railvariant2_data
railvariant2_data:
	.incbin "baserom.dol", 0x3BA028, 0x3C
.global railvariant3_data
railvariant3_data:
	.incbin "baserom.dol", 0x3BA064, 0x3C
.global railvariant4_data
railvariant4_data:
	.incbin "baserom.dol", 0x3BA0A0, 0x3C
.global rollvariant0_data
rollvariant0_data:
	.incbin "baserom.dol", 0x3BA0DC, 0x3C
.global rollvariant1_data
rollvariant1_data:
	.incbin "baserom.dol", 0x3BA118, 0x3C
.global rollvariant2_data
rollvariant2_data:
	.incbin "baserom.dol", 0x3BA154, 0x3C
.global rollvariant3_data
rollvariant3_data:
	.incbin "baserom.dol", 0x3BA190, 0x3C
.global rollvariant4_data
rollvariant4_data:
	.incbin "baserom.dol", 0x3BA1CC, 0x3C
.global lamptrapspike_data
lamptrapspike_data:
	.incbin "baserom.dol", 0x3BA208, 0x3C
.global lamptrapiron_data
lamptrapiron_data:
	.incbin "baserom.dol", 0x3BA244, 0x3C
.global airplane_data
airplane_data:
	.incbin "baserom.dol", 0x3BA280, 0x3C
.global WoodBox_data
WoodBox_data:
	.incbin "baserom.dol", 0x3BA2BC, 0x3C
.global AirportPole_data
AirportPole_data:
	.incbin "baserom.dol", 0x3BA2F8, 0x3C
.global ice_car_data
ice_car_data:
	.incbin "baserom.dol", 0x3BA334, 0x3C
.global move_ice_car_data
move_ice_car_data:
	.incbin "baserom.dol", 0x3BA370, 0x3C
.global AirportBuoy_data
AirportBuoy_data:
	.incbin "baserom.dol", 0x3BA3AC, 0x3C
.global dptCannon_data
dptCannon_data:
	.incbin "baserom.dol", 0x3BA3E8, 0x3C
.global dptKing_data
dptKing_data:
	.incbin "baserom.dol", 0x3BA424, 0x3C
.global KoopaJrSubmarine_data
KoopaJrSubmarine_data:
	.incbin "baserom.dol", 0x3BA460, 0x3C
.global monumentshine_data
monumentshine_data:
	.incbin "baserom.dol", 0x3BA49C, 0x3C
.global belldolpic_data
belldolpic_data:
	.incbin "baserom.dol", 0x3BA4D8, 0x3C
.global dptWeathercock_data
dptWeathercock_data:
	.incbin "baserom.dol", 0x3BA514, 0x3C
.global dptMonteFence_data
dptMonteFence_data:
	.incbin "baserom.dol", 0x3BA550, 0x3C
.global dptCoronaFence_data
dptCoronaFence_data:
	.incbin "baserom.dol", 0x3BA58C, 0x3C
.global MapSmoke_data
MapSmoke_data:
	.incbin "baserom.dol", 0x3BA5C8, 0x3C
.global MareGate_data
MareGate_data:
	.incbin "baserom.dol", 0x3BA604, 0x3C
.global DemoCannon_data
DemoCannon_data:
	.incbin "baserom.dol", 0x3BA640, 0x3C
.global NozzleDoor_data
NozzleDoor_data:
	.incbin "baserom.dol", 0x3BA67C, 0x3C
.global DokanGate_data
DokanGate_data:
	.incbin "baserom.dol", 0x3BA6B8, 0x3C
.global crane_data
crane_data:
	.incbin "baserom.dol", 0x3BA6F4, 0x3C
.global crane90Scene2_data
crane90Scene2_data:
	.incbin "baserom.dol", 0x3BA730, 0x3C
.global crane180Scene1_data
crane180Scene1_data:
	.incbin "baserom.dol", 0x3BA76C, 0x3C
.global craneUpDown_data
craneUpDown_data:
	.incbin "baserom.dol", 0x3BA7A8, 0x3C
.global craneCargoUpDown_data
craneCargoUpDown_data:
	.incbin "baserom.dol", 0x3BA7E4, 0x3C
.global tank_data
tank_data:
	.incbin "baserom.dol", 0x3BA820, 0x3C
.global container_data
container_data:
	.incbin "baserom.dol", 0x3BA85C, 0x3C
.global submarine_data
submarine_data:
	.incbin "baserom.dol", 0x3BA898, 0x3C
.global riccoShip_data
riccoShip_data:
	.incbin "baserom.dol", 0x3BA8D4, 0x3C
.global riccoPole_data
riccoPole_data:
	.incbin "baserom.dol", 0x3BA910, 0x3C
.global riccoLog_data
riccoLog_data:
	.incbin "baserom.dol", 0x3BA94C, 0x3C
.global gesoSurfboard_data
gesoSurfboard_data:
	.incbin "baserom.dol", 0x3BA988, 0x3C
.global gesoSurfboardStatic_data
gesoSurfboardStatic_data:
	.incbin "baserom.dol", 0x3BA9C4, 0x3C
.global riccoWatermill_data
riccoWatermill_data:
	.incbin "baserom.dol", 0x3BAA00, 0x3C
.global riccoShipLog_data
riccoShipLog_data:
	.incbin "baserom.dol", 0x3BAA3C, 0x3C
.global riccoShipDol_data
riccoShipDol_data:
	.incbin "baserom.dol", 0x3BAA78, 0x3C
.global riccoYachtL_data
riccoYachtL_data:
	.incbin "baserom.dol", 0x3BAAB4, 0x3C
.global riccoYachtS_data
riccoYachtS_data:
	.incbin "baserom.dol", 0x3BAAF0, 0x3C
.global riccoBoatL_data
riccoBoatL_data:
	.incbin "baserom.dol", 0x3BAB2C, 0x3C
.global riccoBoatS_data
riccoBoatS_data:
	.incbin "baserom.dol", 0x3BAB68, 0x3C
.global riccoBasket_data
riccoBasket_data:
	.incbin "baserom.dol", 0x3BABA4, 0x3C
.global riccoGangway_data
riccoGangway_data:
	.incbin "baserom.dol", 0x3BABE0, 0x3C
.global riccoShipFish_data
riccoShipFish_data:
	.incbin "baserom.dol", 0x3BAC1C, 0x3C
.global riccoShipGeso_data
riccoShipGeso_data:
	.incbin "baserom.dol", 0x3BAC58, 0x3C
.global riccoSwitch_data
riccoSwitch_data:
	.incbin "baserom.dol", 0x3BAC94, 0x3C
.global riccoSwitchShine_data
riccoSwitchShine_data:
	.incbin "baserom.dol", 0x3BACD0, 0x3C
.global riccoFenceMaze_data
riccoFenceMaze_data:
	.incbin "baserom.dol", 0x3BAD0C, 0x3C
.global riccoStand_data
riccoStand_data:
	.incbin "baserom.dol", 0x3BAD48, 0x3C
.global riccoGrille_data
riccoGrille_data:
	.incbin "baserom.dol", 0x3BAD84, 0x3C
.global riccoArrow_data
riccoArrow_data:
	.incbin "baserom.dol", 0x3BADC0, 0x3C
.global riccoStreetStall_data
riccoStreetStall_data:
	.incbin "baserom.dol", 0x3BADFC, 0x3C
.global riccoHericopter_data
riccoHericopter_data:
	.incbin "baserom.dol", 0x3BAE38, 0x3C
.global surfgeso_red_data
surfgeso_red_data:
	.incbin "baserom.dol", 0x3BAE74, 0x3C
.global surfgeso_yellow_data
surfgeso_yellow_data:
	.incbin "baserom.dol", 0x3BAEB0, 0x3C
.global surfgeso_green_data
surfgeso_green_data:
	.incbin "baserom.dol", 0x3BAEEC, 0x3C
.global big_windmill_data
big_windmill_data:
	.incbin "baserom.dol", 0x3BAF28, 0x3C
.global windmill_far_data
windmill_far_data:
	.incbin "baserom.dol", 0x3BAF64, 0x3C
.global MiniWindmillL_data
MiniWindmillL_data:
	.incbin "baserom.dol", 0x3BAFA0, 0x3C
.global MiniWindmillS_data
MiniWindmillS_data:
	.incbin "baserom.dol", 0x3BAFDC, 0x3C
.global WindmillRoof_data
WindmillRoof_data:
	.incbin "baserom.dol", 0x3BB018, 0x3C
.global lamp_bianco_data
lamp_bianco_data:
	.incbin "baserom.dol", 0x3BB054, 0x3C
.global root_pakkun_data
root_pakkun_data:
	.incbin "baserom.dol", 0x3BB090, 0x3C
.global windmill_block_data
windmill_block_data:
	.incbin "baserom.dol", 0x3BB0CC, 0x3C
.global PolluterPakkun_data
PolluterPakkun_data:
	.incbin "baserom.dol", 0x3BB108, 0x3C
.global BiaBell_data
BiaBell_data:
	.incbin "baserom.dol", 0x3BB144, 0x3C
.global BiaWatermill00_data
BiaWatermill00_data:
	.incbin "baserom.dol", 0x3BB180, 0x3C
.global BiaWatermill01_data
BiaWatermill01_data:
	.incbin "baserom.dol", 0x3BB1BC, 0x3C
.global BiaWatermillVertical_data
BiaWatermillVertical_data:
	.incbin "baserom.dol", 0x3BB1F8, 0x3C
.global BiaTurnBridge_data
BiaTurnBridge_data:
	.incbin "baserom.dol", 0x3BB234, 0x3C
.global LeafBoat_data
LeafBoat_data:
	.incbin "baserom.dol", 0x3BB270, 0x3C
.global LeafBoatRotten_data
LeafBoatRotten_data:
	.incbin "baserom.dol", 0x3BB2AC, 0x3C
.global LampSeesaw_data
LampSeesaw_data:
	.incbin "baserom.dol", 0x3BB2E8, 0x3C
.global BiancoDoor_data
BiancoDoor_data:
	.incbin "baserom.dol", 0x3BB324, 0x3C
.global BiaBridge_data
BiaBridge_data:
	.incbin "baserom.dol", 0x3BB360, 0x3C
.global SandBird_data
SandBird_data:
	.incbin "baserom.dol", 0x3BB39C, 0x3C
.global SandBirdBlock_data
SandBirdBlock_data:
	.incbin "baserom.dol", 0x3BB3D8, 0x3C
.global SkyIsland_data
SkyIsland_data:
	.incbin "baserom.dol", 0x3BB414, 0x3C
.global SandLeaf_data
SandLeaf_data:
	.incbin "baserom.dol", 0x3BB450, 0x3C
.global SandLeafBase00_data
SandLeafBase00_data:
	.incbin "baserom.dol", 0x3BB48C, 0x3C
.global SandLeafBase01_data
SandLeafBase01_data:
	.incbin "baserom.dol", 0x3BB4C8, 0x3C
.global SandLeafBase02_data
SandLeafBase02_data:
	.incbin "baserom.dol", 0x3BB504, 0x3C
.global SandLeafBase03_data
SandLeafBase03_data:
	.incbin "baserom.dol", 0x3BB540, 0x3C
.global SandBomb_data
SandBomb_data:
	.incbin "baserom.dol", 0x3BB57C, 0x3C
.global MirrorL_data
MirrorL_data:
	.incbin "baserom.dol", 0x3BB5B8, 0x3C
.global MirrorM_data
MirrorM_data:
	.incbin "baserom.dol", 0x3BB5F4, 0x3C
.global MirrorS_data
MirrorS_data:
	.incbin "baserom.dol", 0x3BB630, 0x3C
.global SandCastle_data
SandCastle_data:
	.incbin "baserom.dol", 0x3BB66C, 0x3C
.global MammaBlockRotate_data
MammaBlockRotate_data:
	.incbin "baserom.dol", 0x3BB6A8, 0x3C
.global SandEgg_data
SandEgg_data:
	.incbin "baserom.dol", 0x3BB6E4, 0x3C
.global SandEggBroken_data
SandEggBroken_data:
	.incbin "baserom.dol", 0x3BB720, 0x3C
.global ShiningStone_data
ShiningStone_data:
	.incbin "baserom.dol", 0x3BB75C, 0x3C
.global MammaSurfboard00_data
MammaSurfboard00_data:
	.incbin "baserom.dol", 0x3BB798, 0x3C
.global MammaSurfboard01_data
MammaSurfboard01_data:
	.incbin "baserom.dol", 0x3BB7D4, 0x3C
.global MammaSurfboard02_data
MammaSurfboard02_data:
	.incbin "baserom.dol", 0x3BB810, 0x3C
.global MammaSurfboard03_data
MammaSurfboard03_data:
	.incbin "baserom.dol", 0x3BB84C, 0x3C
.global MammaSurfboardStand_data
MammaSurfboardStand_data:
	.incbin "baserom.dol", 0x3BB888, 0x3C
.global MammaYacht00_data
MammaYacht00_data:
	.incbin "baserom.dol", 0x3BB8C4, 0x3C
.global MammaYacht01_data
MammaYacht01_data:
	.incbin "baserom.dol", 0x3BB900, 0x3C
.global MammaYacht02_data
MammaYacht02_data:
	.incbin "baserom.dol", 0x3BB93C, 0x3C
.global SandBombBase00_data
SandBombBase00_data:
	.incbin "baserom.dol", 0x3BB978, 0x3C
.global SandBombBaseMushroom_data
SandBombBaseMushroom_data:
	.incbin "baserom.dol", 0x3BB9B4, 0x3C
.global SandBombBasePyramid_data
SandBombBasePyramid_data:
	.incbin "baserom.dol", 0x3BB9F0, 0x3C
.global SandBombBaseShit_data
SandBombBaseShit_data:
	.incbin "baserom.dol", 0x3BBA2C, 0x3C
.global SandBombBaseStar_data
SandBombBaseStar_data:
	.incbin "baserom.dol", 0x3BBA68, 0x3C
.global SandBombBaseTurtle_data
SandBombBaseTurtle_data:
	.incbin "baserom.dol", 0x3BBAA4, 0x3C
.global SandBombBaseFoot_data
SandBombBaseFoot_data:
	.incbin "baserom.dol", 0x3BBAE0, 0x3C
.global SandBombBaseHand_data
SandBombBaseHand_data:
	.incbin "baserom.dol", 0x3BBB1C, 0x3C
.global SandBombBaseStairs_data
SandBombBaseStairs_data:
	.incbin "baserom.dol", 0x3BBB58, 0x3C
.global coral00_data
coral00_data:
	.incbin "baserom.dol", 0x3BBB94, 0x3C
.global coral01_data
coral01_data:
	.incbin "baserom.dol", 0x3BBBD0, 0x3C
.global GoalWatermelon_data
GoalWatermelon_data:
	.incbin "baserom.dol", 0x3BBC0C, 0x3C
.global TeethOfJuicer_data
TeethOfJuicer_data:
	.incbin "baserom.dol", 0x3BBC48, 0x3C
.global SandEggRepair_data
SandEggRepair_data:
	.incbin "baserom.dol", 0x3BBC84, 0x3C
.global merrygoround_data
merrygoround_data:
	.incbin "baserom.dol", 0x3BBCC0, 0x3C
.global merry_egg_data
merry_egg_data:
	.incbin "baserom.dol", 0x3BBCFC, 0x3C
.global merry_pole_data
merry_pole_data:
	.incbin "baserom.dol", 0x3BBD38, 0x3C
.global FerrisWheel_data
FerrisWheel_data:
	.incbin "baserom.dol", 0x3BBD74, 0x3C
.global gondola_data
gondola_data:
	.incbin "baserom.dol", 0x3BBDB0, 0x3C
.global FerrisLOD_data
FerrisLOD_data:
	.incbin "baserom.dol", 0x3BBDEC, 0x3C
.global viking_data
viking_data:
	.incbin "baserom.dol", 0x3BBE28, 0x3C
.global PinnaEntrance_data
PinnaEntrance_data:
	.incbin "baserom.dol", 0x3BBE64, 0x3C
.global PinnaEntranceOpen_data
PinnaEntranceOpen_data:
	.incbin "baserom.dol", 0x3BBEA0, 0x3C
.global SirenaGate_data
SirenaGate_data:
	.incbin "baserom.dol", 0x3BBEDC, 0x3C
.global BalloonKoopaJr_data
BalloonKoopaJr_data:
	.incbin "baserom.dol", 0x3BBF18, 0x3C
.global ShellCup_data
ShellCup_data:
	.incbin "baserom.dol", 0x3BBF54, 0x3C
.global Gateshell_data
Gateshell_data:
	.incbin "baserom.dol", 0x3BBF90, 0x3C
.global PinnaHangingBridgeBoard_data
PinnaHangingBridgeBoard_data:
	.incbin "baserom.dol", 0x3BBFCC, 0x3C
.global GateManta_data
GateManta_data:
	.incbin "baserom.dol", 0x3BC008, 0x3C
.global PinnaSunFlower_data
PinnaSunFlower_data:
	.incbin "baserom.dol", 0x3BC044, 0x3C
.global AmiKing_data
AmiKing_data:
	.incbin "baserom.dol", 0x3BC080, 0x3C
.global PinnaCoaster_data
PinnaCoaster_data:
	.incbin "baserom.dol", 0x3BC0BC, 0x3C
.global casinoroulette_data
casinoroulette_data:
	.incbin "baserom.dol", 0x3BC0F8, 0x3C
.global roulette00_data
roulette00_data:
	.incbin "baserom.dol", 0x3BC134, 0x3C
.global roulette01_data
roulette01_data:
	.incbin "baserom.dol", 0x3BC170, 0x3C
.global roulette02_data
roulette02_data:
	.incbin "baserom.dol", 0x3BC1AC, 0x3C
.global slotdrum_data
slotdrum_data:
	.incbin "baserom.dol", 0x3BC1E8, 0x3C
.global telesaslot_data
telesaslot_data:
	.incbin "baserom.dol", 0x3BC224, 0x3C
.global donchou_data
donchou_data:
	.incbin "baserom.dol", 0x3BC260, 0x3C
.global casino_panel_gate_data
casino_panel_gate_data:
	.incbin "baserom.dol", 0x3BC29C, 0x3C
.global SakuCasino_data
SakuCasino_data:
	.incbin "baserom.dol", 0x3BC2D8, 0x3C
.global SirenabossWall_data
SirenabossWall_data:
	.incbin "baserom.dol", 0x3BC314, 0x3C
.global SirenaCasinoRoof_data
SirenaCasinoRoof_data:
	.incbin "baserom.dol", 0x3BC350, 0x3C
.global GlassBreak_data
GlassBreak_data:
	.incbin "baserom.dol", 0x3BC38C, 0x3C
.global Closet_data
Closet_data:
	.incbin "baserom.dol", 0x3BC3C8, 0x3C
.global ChestRevolve_data
ChestRevolve_data:
	.incbin "baserom.dol", 0x3BC404, 0x3C
.global PosterTeresa_data
PosterTeresa_data:
	.incbin "baserom.dol", 0x3BC440, 0x3C
.global PictureTeresa_data
PictureTeresa_data:
	.incbin "baserom.dol", 0x3BC47C, 0x3C
.global PanelBreak_data
PanelBreak_data:
	.incbin "baserom.dol", 0x3BC4B8, 0x3C
.global PanelRevolve_data
PanelRevolve_data:
	.incbin "baserom.dol", 0x3BC4F4, 0x3C
.global SirenaBlockBreak_data
SirenaBlockBreak_data:
	.incbin "baserom.dol", 0x3BC530, 0x3C
.global SirenaShop_data
SirenaShop_data:
	.incbin "baserom.dol", 0x3BC56C, 0x3C
.global StainHotel_data
StainHotel_data:
	.incbin "baserom.dol", 0x3BC5A8, 0x3C
.global cogwheel_data
cogwheel_data:
	.incbin "baserom.dol", 0x3BC5E4, 0x3C
.global CogwheelPlate_data
CogwheelPlate_data:
	.incbin "baserom.dol", 0x3BC620, 0x3C
.global CogwheelPot_data
CogwheelPot_data:
	.incbin "baserom.dol", 0x3BC65C, 0x3C
.global mare_float_house_data
mare_float_house_data:
	.incbin "baserom.dol", 0x3BC698, 0x3C
.global StopRock_physical_data
StopRock_physical_data:
	.incbin "baserom.dol", 0x3BC6D4, 0x34
.global StopRock_physical_info
StopRock_physical_info:
	.incbin "baserom.dol", 0x3BC708, 0xC
.global StopRock_data
StopRock_data:
	.incbin "baserom.dol", 0x3BC714, 0x3C
.global ElasticCode_data
ElasticCode_data:
	.incbin "baserom.dol", 0x3BC750, 0x3C
.global GrowthTree_data
GrowthTree_data:
	.incbin "baserom.dol", 0x3BC78C, 0x3C
.global WireBellPurple_data
WireBellPurple_data:
	.incbin "baserom.dol", 0x3BC7C8, 0x3C
.global MuddyBoat_data
MuddyBoat_data:
	.incbin "baserom.dol", 0x3BC804, 0x3C
.global Puncher_data
Puncher_data:
	.incbin "baserom.dol", 0x3BC840, 0x3C
.global HatoPop_data
HatoPop_data:
	.incbin "baserom.dol", 0x3BC87C, 0x3C
.global MareWaterJump_data
MareWaterJump_data:
	.incbin "baserom.dol", 0x3BC8B8, 0x3C
.global BumpyWall_data
BumpyWall_data:
	.incbin "baserom.dol", 0x3BC8F4, 0x3C
.global ExBottle_data
ExBottle_data:
	.incbin "baserom.dol", 0x3BC930, 0x3C
.global CoinFish_data
CoinFish_data:
	.incbin "baserom.dol", 0x3BC96C, 0x3C
.global MareFall_data
MareFall_data:
	.incbin "baserom.dol", 0x3BC9A8, 0x3C
.global MareCork_data
MareCork_data:
	.incbin "baserom.dol", 0x3BC9E4, 0x3C
.global MonteRoot_data
MonteRoot_data:
	.incbin "baserom.dol", 0x3BCA20, 0x3C
.global JumpMushroom_data
JumpMushroom_data:
	.incbin "baserom.dol", 0x3BCA5C, 0x3C
.global MonteGoalFlag_data
MonteGoalFlag_data:
	.incbin "baserom.dol", 0x3BCA98, 0x3C
.global HangingBridgeBoard_data
HangingBridgeBoard_data:
	.incbin "baserom.dol", 0x3BCAD4, 0x3C
.global HangingBridge_data
HangingBridge_data:
	.incbin "baserom.dol", 0x3BCB10, 0x3C
.global SwingBoard_data
SwingBoard_data:
	.incbin "baserom.dol", 0x3BCB4C, 0x3C
.global Fluff_data
Fluff_data:
	.incbin "baserom.dol", 0x3BCB88, 0x3C
.global FluffManager_data
FluffManager_data:
	.incbin "baserom.dol", 0x3BCBC4, 0x3C
.global Bathtub_data
Bathtub_data:
	.incbin "baserom.dol", 0x3BCC00, 0x3C
.global BathtubStand_data
BathtubStand_data:
	.incbin "baserom.dol", 0x3BCC3C, 0x3C
.global BathtubStandBreaking_animation
BathtubStandBreaking_animation:
	.incbin "baserom.dol", 0x3BCC78, 0xC
.global BathtubStandBreaking_data
BathtubStandBreaking_data:
	.incbin "baserom.dol", 0x3BCC84, 0x3C
.global ex1_turn_lift_move_data
ex1_turn_lift_move_data:
	.incbin "baserom.dol", 0x3BCCC0, 0xC
.global ex1_turn_lift_data
ex1_turn_lift_data:
	.incbin "baserom.dol", 0x3BCCCC, 0x3C
.global z_turn_disk_move_data
z_turn_disk_move_data:
	.incbin "baserom.dol", 0x3BCD08, 0xC
.global z_turn_disk_data
z_turn_disk_data:
	.incbin "baserom.dol", 0x3BCD14, 0x3C
.global y_turn_lift_move_data
y_turn_lift_move_data:
	.incbin "baserom.dol", 0x3BCD50, 0xC
.global y_turn_lift_data
y_turn_lift_data:
	.incbin "baserom.dol", 0x3BCD5C, 0x3C
.global EXskyTumiki_data
EXskyTumiki_data:
	.incbin "baserom.dol", 0x3BCD98, 0x3C
.global EXskyWindow_data
EXskyWindow_data:
	.incbin "baserom.dol", 0x3BCDD4, 0x3C
.global EXskySenro_data
EXskySenro_data:
	.incbin "baserom.dol", 0x3BCE10, 0x3C
.global Nail_data
Nail_data:
	.incbin "baserom.dol", 0x3BCE4C, 0x3C
.global PachinkoNail_data
PachinkoNail_data:
	.incbin "baserom.dol", 0x3BCE88, 0x3C
.global FileLoadBlockA_data
FileLoadBlockA_data:
	.incbin "baserom.dol", 0x3BCEC4, 0x3C
.global FileLoadBlockB_data
FileLoadBlockB_data:
	.incbin "baserom.dol", 0x3BCF00, 0x3C
.global FileLoadBlockC_data
FileLoadBlockC_data:
	.incbin "baserom.dol", 0x3BCF3C, 0x3C
.global sObjDataTable
sObjDataTable:
	.incbin "baserom.dol", 0x3BCF78, 0x5A0
.global __vt__14TMapObjGeneral
__vt__14TMapObjGeneral:
	.incbin "baserom.dol", 0x3BD518, 0x1E8
.global $$23438
$$23438:
	.incbin "baserom.dol", 0x3BD700, 0x28
.global item_names$3004
item_names$3004:
	.incbin "baserom.dol", 0x3BD728, 0x18
.global __vt__18TMapObjBaseManager
__vt__18TMapObjBaseManager:
	.incbin "baserom.dol", 0x3BD740, 0x40
.global __vt__10TMerryPole
__vt__10TMerryPole:
	.incbin "baserom.dol", 0x3BD780, 0x164
.global __vt__14TMapObjManager
__vt__14TMapObjManager:
	.incbin "baserom.dol", 0x3BD8E4, 0x44
.global __vt__11TMapObjTurn
__vt__11TMapObjTurn:
	.incbin "baserom.dol", 0x3BD928, 0x174
.global __vt__16TMapObjMessenger
__vt__16TMapObjMessenger:
	.incbin "baserom.dol", 0x3BDA9C, 0xA4
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3BDB40, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3BDB4C, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3BDB58, 0x10
.global MtxCalcTypeName
MtxCalcTypeName:
	.incbin "baserom.dol", 0x3BDB68, 0x10
.global mPromiLife__6TShine
mPromiLife__6TShine:
	.incbin "baserom.dol", 0x3BDB78, 0x10
.global mSenkoRate__6TShine
mSenkoRate__6TShine:
	.incbin "baserom.dol", 0x3BDB88, 0x10
.global mKiraRate__6TShine
mKiraRate__6TShine:
	.incbin "baserom.dol", 0x3BDB98, 0x10
.global mBowRate__6TShine
mBowRate__6TShine:
	.incbin "baserom.dol", 0x3BDBA8, 0x10
.global __vt__10TNozzleBox
__vt__10TNozzleBox:
	.incbin "baserom.dol", 0x3BDBB8, 0x1DC
.global $$23722
$$23722:
	.incbin "baserom.dol", 0x3BDD94, 0x44
.global $$24268
$$24268:
	.incbin "baserom.dol", 0x3BDDD8, 0x4C
.global __vt__11TItemNozzle
__vt__11TItemNozzle:
	.incbin "baserom.dol", 0x3BDE24, 0x1E4
.global __vt__9TEggYoshi
__vt__9TEggYoshi:
	.incbin "baserom.dol", 0x3BE008, 0x1DC
.global __vt__6TShine
__vt__6TShine:
	.incbin "baserom.dol", 0x3BE1E4, 0x1E4
.global __vt__9TCoinBlue
__vt__9TCoinBlue:
	.incbin "baserom.dol", 0x3BE3C8, 0x1E4
.global __vt__8TCoinRed
__vt__8TCoinRed:
	.incbin "baserom.dol", 0x3BE5AC, 0x1E4
.global __vt__10TCoinEmpty
__vt__10TCoinEmpty:
	.incbin "baserom.dol", 0x3BE790, 0x1E4
.global __vt__11TFlowerCoin
__vt__11TFlowerCoin:
	.incbin "baserom.dol", 0x3BE974, 0x1E4
.global __vt__5TCoin
__vt__5TCoin:
	.incbin "baserom.dol", 0x3BEB58, 0x1E4
.global __vt__5TItem
__vt__5TItem:
	.incbin "baserom.dol", 0x3BED3C, 0x1E4
.global __vt__12TItemManager
__vt__12TItemManager:
	.incbin "baserom.dol", 0x3BEF20, 0x40
.global __vt__14TBasketReverse
__vt__14TBasketReverse:
	.incbin "baserom.dol", 0x3BEF60, 0x164
.global __vt__14TRedCoinSwitch
__vt__14TRedCoinSwitch:
	.incbin "baserom.dol", 0x3BF0C4, 0x164
.global __vt__13TMapObjSwitch
__vt__13TMapObjSwitch:
	.incbin "baserom.dol", 0x3BF228, 0x164
.global __vt__12THideObjInfo
__vt__12THideObjInfo:
	.incbin "baserom.dol", 0x3BF38C, 0xA4
.global __vt__17TMapObjWaterSpray
__vt__17TMapObjWaterSpray:
	.incbin "baserom.dol", 0x3BF430, 0x164
.global __vt__10TDamageObj
__vt__10TDamageObj:
	.incbin "baserom.dol", 0x3BF594, 0xA4
.global __vt__16TMapObjStartDemo
__vt__16TMapObjStartDemo:
	.incbin "baserom.dol", 0x3BF638, 0x164
.global __vt__25TMapObjChangeStageHipDrop
__vt__25TMapObjChangeStageHipDrop:
	.incbin "baserom.dol", 0x3BF79C, 0x164
.global __vt__18TMapObjChangeStage
__vt__18TMapObjChangeStage:
	.incbin "baserom.dol", 0x3BF900, 0x164
.global __vt__16TMapObjBillboard
__vt__16TMapObjBillboard:
	.incbin "baserom.dol", 0x3BFA64, 0x170
.global __vt__8TManhole
__vt__8TManhole:
	.incbin "baserom.dol", 0x3BFBD4, 0x1DC
.global __vt__5TDoor
__vt__5TDoor:
	.incbin "baserom.dol", 0x3BFDB0, 0x168
.global __vt__18TSuperHipDropBlock
__vt__18TSuperHipDropBlock:
	.incbin "baserom.dol", 0x3BFF18, 0x170
.global __vt__12TTelesaBlock
__vt__12TTelesaBlock:
	.incbin "baserom.dol", 0x3C0088, 0x164
.global __vt__11TJuiceBlock
__vt__11TJuiceBlock:
	.incbin "baserom.dol", 0x3C01EC, 0x164
.global __vt__11TBrickBlock
__vt__11TBrickBlock:
	.incbin "baserom.dol", 0x3C0350, 0x170
.global __vt__9TIceBlock
__vt__9TIceBlock:
	.incbin "baserom.dol", 0x3C04C0, 0x164
.global __vt__10TLeanBlock
__vt__10TLeanBlock:
	.incbin "baserom.dol", 0x3C0624, 0x168
.global __vt__10TSandBlock
__vt__10TSandBlock:
	.incbin "baserom.dol", 0x3C078C, 0x164
.global __vt__15TBreakableBlock
__vt__15TBreakableBlock:
	.incbin "baserom.dol", 0x3C08F0, 0x1E0
.global __vt__8TWoodLog
__vt__8TWoodLog:
	.incbin "baserom.dol", 0x3C0AD0, 0x168
.global __vt__14TBellWatermill
__vt__14TBellWatermill:
	.incbin "baserom.dol", 0x3C0C38, 0x174
.global __vt__11TBiancoBell
__vt__11TBiancoBell:
	.incbin "baserom.dol", 0x3C0DAC, 0x164
.global __vt__15TLampSeesawMain
__vt__15TLampSeesawMain:
	.incbin "baserom.dol", 0x3C0F10, 0x168
.global __vt__11TLampSeesaw
__vt__11TLampSeesaw:
	.incbin "baserom.dol", 0x3C1078, 0x168
.global __vt__15TLeafBoatRotten
__vt__15TLeafBoatRotten:
	.incbin "baserom.dol", 0x3C11E0, 0x164
.global __vt__9TLeafBoat
__vt__9TLeafBoat:
	.incbin "baserom.dol", 0x3C1344, 0x164
.global __vt__19TBiancoMiniWindmill
__vt__19TBiancoMiniWindmill:
	.incbin "baserom.dol", 0x3C14A8, 0x170
.global __vt__24TBiancoWatermillVertical
__vt__24TBiancoWatermillVertical:
	.incbin "baserom.dol", 0x3C1618, 0x164
.global __vt__16TBiancoWatermill
__vt__16TBiancoWatermill:
	.incbin "baserom.dol", 0x3C177C, 0x164
.global __vt__17TMapObjRootPakkun
__vt__17TMapObjRootPakkun:
	.incbin "baserom.dol", 0x3C18E0, 0x164
.global __vt__12TBigWindmill
__vt__12TBigWindmill:
	.incbin "baserom.dol", 0x3C1A44, 0x164
.global __vt__14TPictureTelesa
__vt__14TPictureTelesa:
	.incbin "baserom.dol", 0x3C1BA8, 0x17C
.global __vt__13TPanelRevolve
__vt__13TPanelRevolve:
	.incbin "baserom.dol", 0x3C1D24, 0x164
.global __vt__13TChestRevolve
__vt__13TChestRevolve:
	.incbin "baserom.dol", 0x3C1E88, 0x164
.global __vt__14TWarpAreaActor
__vt__14TWarpAreaActor:
	.incbin "baserom.dol", 0x3C1FEC, 0xA4
.global __vt__17TSirenaCasinoRoof
__vt__17TSirenaCasinoRoof:
	.incbin "baserom.dol", 0x3C2090, 0x164
.global __vt__15TSirenabossWall
__vt__15TSirenabossWall:
	.incbin "baserom.dol", 0x3C21F4, 0x164
.global __vt__11TSakuCasino
__vt__11TSakuCasino:
	.incbin "baserom.dol", 0x3C2358, 0x164
.global __vt__7TCloset
__vt__7TCloset:
	.incbin "baserom.dol", 0x3C24BC, 0x170
.global __vt__8TDonchou
__vt__8TDonchou:
	.incbin "baserom.dol", 0x3C262C, 0x164
.global __vt__16TCasinoPanelGate
__vt__16TCasinoPanelGate:
	.incbin "baserom.dol", 0x3C2790, 0x170
.global __vt__13TItemSlotDrum
__vt__13TItemSlotDrum:
	.incbin "baserom.dol", 0x3C2900, 0x174
.global __vt__9TSlotDrum
__vt__9TSlotDrum:
	.incbin "baserom.dol", 0x3C2A74, 0x174
.global __vt__9TRoulette
__vt__9TRoulette:
	.incbin "baserom.dol", 0x3C2BE8, 0x168
.global __vt__11TRouletteSw
__vt__11TRouletteSw:
	.incbin "baserom.dol", 0x3C2D50, 0xA8
.global __vt__14TFruitLauncher
__vt__14TFruitLauncher:
	.incbin "baserom.dol", 0x3C2DF8, 0x164
.global __vt__12TFruitSwitch
__vt__12TFruitSwitch:
	.incbin "baserom.dol", 0x3C2F5C, 0x164
.global __vt__12TSurfGesoObj
__vt__12TSurfGesoObj:
	.incbin "baserom.dol", 0x3C30C0, 0x1E4
.global __vt__15TRiccoWatermill
__vt__15TRiccoWatermill:
	.incbin "baserom.dol", 0x3C32A4, 0x164
.global __vt__11TCraneCargo
__vt__11TCraneCargo:
	.incbin "baserom.dol", 0x3C3408, 0x168
.global __vt__12TCraneUpDown
__vt__12TCraneUpDown:
	.incbin "baserom.dol", 0x3C3570, 0x164
.global __vt__10TCraneRotY
__vt__10TCraneRotY:
	.incbin "baserom.dol", 0x3C36D4, 0x164
.global __vt__8TSandEgg
__vt__8TSandEgg:
	.incbin "baserom.dol", 0x3C3838, 0x164
.global $$25077
$$25077:
	.incbin "baserom.dol", 0x3C399C, 0x24
.global __vt__23TMammaMirrorMapOperator
__vt__23TMammaMirrorMapOperator:
	.incbin "baserom.dol", 0x3C39C0, 0x24
.global __vt__15TGoalWatermelon
__vt__15TGoalWatermelon:
	.incbin "baserom.dol", 0x3C39E4, 0x164
.global __vt__9TSandBird
__vt__9TSandBird:
	.incbin "baserom.dol", 0x3C3B48, 0x170
.global __vt__11TMammaYacht
__vt__11TMammaYacht:
	.incbin "baserom.dol", 0x3C3CB8, 0x164
.global __vt__17TMammaBlockRotate
__vt__17TMammaBlockRotate:
	.incbin "baserom.dol", 0x3C3E1C, 0x164
.global __vt__13TShiningStone
__vt__13TShiningStone:
	.incbin "baserom.dol", 0x3C3F80, 0xA4
.global __vt__11TLeanMirror
__vt__11TLeanMirror:
	.incbin "baserom.dol", 0x3C4024, 0x164
.global __vt__11TSandCastle
__vt__11TSandCastle:
	.incbin "baserom.dol", 0x3C4188, 0x184
.global __vt__13TSandBombBase
__vt__13TSandBombBase:
	.incbin "baserom.dol", 0x3C430C, 0x184
.global __vt__9TSandBomb
__vt__9TSandBomb:
	.incbin "baserom.dol", 0x3C4490, 0x164
.global __vt__13TSandLeafBase
__vt__13TSandLeafBase:
	.incbin "baserom.dol", 0x3C45F4, 0x16C
.global __vt__9TSandBase
__vt__9TSandBase:
	.incbin "baserom.dol", 0x3C4760, 0x16C
.global __vt__9TSandLeaf
__vt__9TSandLeaf:
	.incbin "baserom.dol", 0x3C48CC, 0x164
.global __vt__13TPinnaCoaster
__vt__13TPinnaCoaster:
	.incbin "baserom.dol", 0x3C4A30, 0x164
.global __vt__8TAmiKing
__vt__8TAmiKing:
	.incbin "baserom.dol", 0x3C4B94, 0x164
.global __vt__16TWaterRecoverObj
__vt__16TWaterRecoverObj:
	.incbin "baserom.dol", 0x3C4CF8, 0x164
.global __vt__14TPinnaEntrance
__vt__14TPinnaEntrance:
	.incbin "baserom.dol", 0x3C4E5C, 0x164
.global __vt__15TBalloonKoopaJr
__vt__15TBalloonKoopaJr:
	.incbin "baserom.dol", 0x3C4FC0, 0x1DC
.global __vt__24TChangeStageMerrygoround
__vt__24TChangeStageMerrygoround:
	.incbin "baserom.dol", 0x3C519C, 0x164
.global __vt__13TMerrygoround
__vt__13TMerrygoround:
	.incbin "baserom.dol", 0x3C5300, 0x164
.global __vt__9TShellCup
__vt__9TShellCup:
	.incbin "baserom.dol", 0x3C5464, 0x164
.global __vt__11TPinnaShell
__vt__11TPinnaShell:
	.incbin "baserom.dol", 0x3C55C8, 0xA4
.global __vt__7TViking
__vt__7TViking:
	.incbin "baserom.dol", 0x3C566C, 0x168
.global __vt__17THorizontalViking
__vt__17THorizontalViking:
	.incbin "baserom.dol", 0x3C57D4, 0x168
.global __vt__12TFerrisWheel
__vt__12TFerrisWheel:
	.incbin "baserom.dol", 0x3C593C, 0x164
.global __vt__11TGateShadow
__vt__11TGateShadow:
	.incbin "baserom.dol", 0x3C5AA0, 0x28
.global __vt__15TMareEventPoint
__vt__15TMareEventPoint:
	.incbin "baserom.dol", 0x3C5AC8, 0xA4
.global __vt__9TMareCork
__vt__9TMareCork:
	.incbin "baserom.dol", 0x3C5B6C, 0x164
.global __vt__9TMareFall
__vt__9TMareFall:
	.incbin "baserom.dol", 0x3C5CD0, 0x164
.global __vt__10TMuddyBoat
__vt__10TMuddyBoat:
	.incbin "baserom.dol", 0x3C5E34, 0x164
.global __vt__14TMapObjPuncher
__vt__14TMapObjPuncher:
	.incbin "baserom.dol", 0x3C5F98, 0x164
.global __vt__9TWireBell
__vt__9TWireBell:
	.incbin "baserom.dol", 0x3C60FC, 0x164
.global __vt__15TMapObjGrowTree
__vt__15TMapObjGrowTree:
	.incbin "baserom.dol", 0x3C6260, 0x164
.global __vt__18TMapObjElasticCode
__vt__18TMapObjElasticCode:
	.incbin "baserom.dol", 0x3C63C4, 0x164
.global __vt__9TCogwheel
__vt__9TCogwheel:
	.incbin "baserom.dol", 0x3C6528, 0x164
.global __vt__14TCogwheelScale
__vt__14TCogwheelScale:
	.incbin "baserom.dol", 0x3C668C, 0x164
.global __vt__18TMapObjFlagManager
__vt__18TMapObjFlagManager:
	.incbin "baserom.dol", 0x3C67F0, 0x24
.global __vt__11TMapObjFlag
__vt__11TMapObjFlag:
	.incbin "baserom.dol", 0x3C6814, 0xAC
.global __vt__11TMapObjWave
__vt__11TMapObjWave:
	.incbin "baserom.dol", 0x3C68C0, 0x28
.global __vt__17TMapObjFloatOnSea
__vt__17TMapObjFloatOnSea:
	.incbin "baserom.dol", 0x3C68E8, 0x168
.global __vt__10TSandPlane
__vt__10TSandPlane:
	.incbin "baserom.dol", 0x3C6A50, 0xFC
.global __vt__10TRockPlane
__vt__10TRockPlane:
	.incbin "baserom.dol", 0x3C6B4C, 0xFC
.global __vt__12TMapObjPlane
__vt__12TMapObjPlane:
	.incbin "baserom.dol", 0x3C6C48, 0x100
.global __vt__10TRideCloud
__vt__10TRideCloud:
	.incbin "baserom.dol", 0x3C6D48, 0x170
.global __vt__14TBigWatermelon
__vt__14TBigWatermelon:
	.incbin "baserom.dol", 0x3C6EB8, 0x1F0
.global $$24190
$$24190:
	.incbin "baserom.dol", 0x3C70A8, 0x38
.global __vt__11TCoverFruit
__vt__11TCoverFruit:
	.incbin "baserom.dol", 0x3C70E0, 0x164
.global __vt__12TRandomFruit
__vt__12TRandomFruit:
	.incbin "baserom.dol", 0x3C7244, 0x1F0
.global __vt__11TResetFruit
__vt__11TResetFruit:
	.incbin "baserom.dol", 0x3C7434, 0x1F0
.global __vt__11TMapObjBall
__vt__11TMapObjBall:
	.incbin "baserom.dol", 0x3C7624, 0x1F4
.global __vt__17TAirportEventSink
__vt__17TAirportEventSink:
	.incbin "baserom.dol", 0x3C7818, 0x54
.global __vt__14TAirportSwitch
__vt__14TAirportSwitch:
	.incbin "baserom.dol", 0x3C786C, 0x1DC
.global __vt__16TTurboNozzleDoor
__vt__16TTurboNozzleDoor:
	.incbin "baserom.dol", 0x3C7A48, 0x164
.global __vt__11TDemoCannon
__vt__11TDemoCannon:
	.incbin "baserom.dol", 0x3C7BAC, 0x164
.global __vt__9TMareGate
__vt__9TMareGate:
	.incbin "baserom.dol", 0x3C7D10, 0x164
.global __vt__12TMapObjSmoke
__vt__12TMapObjSmoke:
	.incbin "baserom.dol", 0x3C7E74, 0x170
.global __vt__14TDptMonteFence
__vt__14TDptMonteFence:
	.incbin "baserom.dol", 0x3C7FE4, 0x164
.global __vt__11TBellDolpic
__vt__11TBellDolpic:
	.incbin "baserom.dol", 0x3C8148, 0x164
.global __vt__14TMonumentShine
__vt__14TMonumentShine:
	.incbin "baserom.dol", 0x3C82AC, 0x164
.global __vt__23TMapObjRevivalPollution
__vt__23TMapObjRevivalPollution:
	.incbin "baserom.dol", 0x3C8410, 0x24
.global __vt__13TPolluterBase
__vt__13TPolluterBase:
	.incbin "baserom.dol", 0x3C8434, 0x164
.global __vt__19TMapObjGrassManager
__vt__19TMapObjGrassManager:
	.incbin "baserom.dol", 0x3C8598, 0x24
.global __vt__17TMapObjGrassGroup
__vt__17TMapObjGrassGroup:
	.incbin "baserom.dol", 0x3C85BC, 0xA4
.global __vt__18TMapObjPoleManager
__vt__18TMapObjPoleManager:
	.incbin "baserom.dol", 0x3C8660, 0x28
.global __vt__18TMapObjWaterFilter
__vt__18TMapObjWaterFilter:
	.incbin "baserom.dol", 0x3C8688, 0xA0
.global __vt__18TMapObjSeaIndirect
__vt__18TMapObjSeaIndirect:
	.incbin "baserom.dol", 0x3C8728, 0xA0
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3C87C8, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3C87D4, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3C87E0, 0x10
.global MtxCalcTypeName
MtxCalcTypeName:
	.incbin "baserom.dol", 0x3C87F0, 0x10
.global gateMActorNames
gateMActorNames:
	.incbin "baserom.dol", 0x3C8800, 0x14
.global gateNames$2573
gateNames$2573:
	.incbin "baserom.dol", 0x3C8814, 0x18
.global __vt__10TModelGate
__vt__10TModelGate:
	.incbin "baserom.dol", 0x3C882C, 0xB4
.global $$23054
$$23054:
	.incbin "baserom.dol", 0x3C88E0, 0xD8
.global __vt__10TRailFence
__vt__10TRailFence:
	.incbin "baserom.dol", 0x3C89B8, 0x164
.global __vt__12TFenceWaterH
__vt__12TFenceWaterH:
	.incbin "baserom.dol", 0x3C8B1C, 0x16C
.global __vt__11TFenceWater
__vt__11TFenceWater:
	.incbin "baserom.dol", 0x3C8C88, 0x16C
.global __vt__20TRevolvingFenceInner
__vt__20TRevolvingFenceInner:
	.incbin "baserom.dol", 0x3C8DF4, 0x164
.global __vt__20TRevolvingFenceOuter
__vt__20TRevolvingFenceOuter:
	.incbin "baserom.dol", 0x3C8F58, 0x164
.global __vt__6TFence
__vt__6TFence:
	.incbin "baserom.dol", 0x3C90BC, 0x164
.global __vt__14TFileLoadBlock
__vt__14TFileLoadBlock:
	.incbin "baserom.dol", 0x3C9220, 0x164
.global __vt__17TMapObjOptionWall
__vt__17TMapObjOptionWall:
	.incbin "baserom.dol", 0x3C9384, 0xA4
.global __vt__10TWoodBlock
__vt__10TWoodBlock:
	.incbin "baserom.dol", 0x3C9428, 0x170
.global __vt__10TRollBlock
__vt__10TRollBlock:
	.incbin "baserom.dol", 0x3C9598, 0x164
.global __vt__10TRailBlock
__vt__10TRailBlock:
	.incbin "baserom.dol", 0x3C96FC, 0x170
.global __vt__11TNormalLift
__vt__11TNormalLift:
	.incbin "baserom.dol", 0x3C986C, 0x170
.global __vt__11TRailMapObj
__vt__11TRailMapObj:
	.incbin "baserom.dol", 0x3C99DC, 0x174
.global __vt__13TFluffManager
__vt__13TFluffManager:
	.incbin "baserom.dol", 0x3C9B50, 0x164
.global __vt__6TFluff
__vt__6TFluff:
	.incbin "baserom.dol", 0x3C9CB4, 0x164
.global __vt__9TGoalFlag
__vt__9TGoalFlag:
	.incbin "baserom.dol", 0x3C9E18, 0x164
.global __vt__11TSwingBoard
__vt__11TSwingBoard:
	.incbin "baserom.dol", 0x3C9F7C, 0x164
.global __vt__14THangingBridge
__vt__14THangingBridge:
	.incbin "baserom.dol", 0x3CA0E0, 0x24
.global __vt__19THangingBridgeBoard
__vt__19THangingBridgeBoard:
	.incbin "baserom.dol", 0x3CA104, 0x168
.global __vt__13TJumpMushroom
__vt__13TJumpMushroom:
	.incbin "baserom.dol", 0x3CA26C, 0x164
.global __vt__16TMapObjMonteRoot
__vt__16TMapObjMonteRoot:
	.incbin "baserom.dol", 0x3CA3D0, 0x168
.global __vt__16TMapObjTreeScale
__vt__16TMapObjTreeScale:
	.incbin "baserom.dol", 0x3CA538, 0x1DC
.global __vt__11TMapObjTree
__vt__11TMapObjTree:
	.incbin "baserom.dol", 0x3CA714, 0x1DC
.global __vt__12TMapObjSteam
__vt__12TMapObjSteam:
	.incbin "baserom.dol", 0x3CA8F0, 0x170
.global __vt__10TJointCoin
__vt__10TJointCoin:
	.incbin "baserom.dol", 0x3CAA60, 0x170
.global __vt__11TMapObjNail
__vt__11TMapObjNail:
	.incbin "baserom.dol", 0x3CABD0, 0x170
.global __vt__5TPool
__vt__5TPool:
	.incbin "baserom.dol", 0x3CAD40, 0x164
.global __vt__12TPoolManager
__vt__12TPoolManager:
	.incbin "baserom.dol", 0x3CAEA4, 0x44
.global __vt__8TBathtub
__vt__8TBathtub:
	.incbin "baserom.dol", 0x3CAEE8, 0x164
.global __vt__12TBathtubGrip
__vt__12TBathtubGrip:
	.incbin "baserom.dol", 0x3CB04C, 0x164
.global __vt__21TBathtubGripPartsHard
__vt__21TBathtubGripPartsHard:
	.incbin "baserom.dol", 0x3CB1B0, 0xFC
.global __vt__24TBathtubGripPartsFragile
__vt__24TBathtubGripPartsFragile:
	.incbin "baserom.dol", 0x3CB2AC, 0xFC
.global __vt__17TBathtubGripParts
__vt__17TBathtubGripParts:
	.incbin "baserom.dol", 0x3CB3A8, 0x100
.global __vt__9TJumpBase
__vt__9TJumpBase:
	.incbin "baserom.dol", 0x3CB4A8, 0x164
.global __vt__12TMushroom1up
__vt__12TMushroom1up:
	.incbin "baserom.dol", 0x3CB60C, 0x164
.global __vt__8TWoodBox
__vt__8TWoodBox:
	.incbin "baserom.dol", 0x3CB770, 0x170
.global __vt__13TBreakHideObj
__vt__13TBreakHideObj:
	.incbin "baserom.dol", 0x3CB8E0, 0x170
.global __vt__19THideObjPictureTwin
__vt__19THideObjPictureTwin:
	.incbin "baserom.dol", 0x3CBA50, 0x17C
.global __vt__23TWaterHitPictureHideObj
__vt__23TWaterHitPictureHideObj:
	.incbin "baserom.dol", 0x3CBBCC, 0x17C
.global __vt__15THipDropHideObj
__vt__15THipDropHideObj:
	.incbin "baserom.dol", 0x3CBD48, 0x170
.global __vt__17TFruitBasketEvent
__vt__17TFruitBasketEvent:
	.incbin "baserom.dol", 0x3CBEB8, 0x178
.global __vt__12TFruitBasket
__vt__12TFruitBasket:
	.incbin "baserom.dol", 0x3CC030, 0x178
.global __vt__16TFruitHitHideObj
__vt__16TFruitHitHideObj:
	.incbin "baserom.dol", 0x3CC1A8, 0x174
.global __vt__16TWaterHitHideObj
__vt__16TWaterHitHideObj:
	.incbin "baserom.dol", 0x3CC31C, 0x170
.global __vt__8THideObj
__vt__8THideObj:
	.incbin "baserom.dol", 0x3CC48C, 0x170
.global __vt__12THideObjBase
__vt__12THideObjBase:
	.incbin "baserom.dol", 0x3CC5FC, 0x174
.global __vt__13TLampTrapIron
__vt__13TLampTrapIron:
	.incbin "baserom.dol", 0x3CC770, 0x164
.global __vt__16TLampTrapIronHit
__vt__16TLampTrapIronHit:
	.incbin "baserom.dol", 0x3CC8D4, 0xA4
.global __vt__14TLampTrapSpike
__vt__14TLampTrapSpike:
	.incbin "baserom.dol", 0x3CC978, 0x164
.global __vt__17TLampTrapSpikeHit
__vt__17TLampTrapSpikeHit:
	.incbin "baserom.dol", 0x3CCADC, 0xA4
.global __vt__11TJointModel
__vt__11TJointModel:
	.incbin "baserom.dol", 0x3CCB80, 0x38
.global __vt__18TJointModelManager
__vt__18TJointModelManager:
	.incbin "baserom.dol", 0x3CCBB8, 0x28
.global __vt__9TJointObj
__vt__9TJointObj:
	.incbin "baserom.dol", 0x3CCBE0, 0x28
.global __vt__4TMap
__vt__4TMap:
	.incbin "baserom.dol", 0x3CCC08, 0x24
.global $$23341
$$23341:
	.incbin "baserom.dol", 0x3CCC2C, 0x44
.global __vt__16TBGCheckListWarp
__vt__16TBGCheckListWarp:
	.incbin "baserom.dol", 0x3CCC70, 0xC
.global __vt__12TBGCheckList
__vt__12TBGCheckList:
	.incbin "baserom.dol", 0x3CCC7C, 0xC
.global __vt__17TMapCollisionWarp
__vt__17TMapCollisionWarp:
	.incbin "baserom.dol", 0x3CCC88, 0x24
.global __vt__17TMapCollisionMove
__vt__17TMapCollisionMove:
	.incbin "baserom.dol", 0x3CCCAC, 0x24
.global __vt__19TMapCollisionStatic
__vt__19TMapCollisionStatic:
	.incbin "baserom.dol", 0x3CCCD0, 0x24
.global __vt__17TMapCollisionBase
__vt__17TMapCollisionBase:
	.incbin "baserom.dol", 0x3CCCF4, 0x24
.global __vt__12TMapDrawWall
__vt__12TMapDrawWall:
	.incbin "baserom.dol", 0x3CCD18, 0x28
.global __vt__9TMapEvent
__vt__9TMapEvent:
	.incbin "baserom.dol", 0x3CCD40, 0x40
.global __vt__24TMapEventSinkShadowMario
__vt__24TMapEventSinkShadowMario:
	.incbin "baserom.dol", 0x3CCD80, 0x54
.global __vt__19TMapEventSinkBianco
__vt__19TMapEventSinkBianco:
	.incbin "baserom.dol", 0x3CCDD4, 0x54
.global __vt__29TMapEventSinkInPollutionReset
__vt__29TMapEventSinkInPollutionReset:
	.incbin "baserom.dol", 0x3CCE28, 0x54
.global __vt__24TMapEventSinkInPollution
__vt__24TMapEventSinkInPollution:
	.incbin "baserom.dol", 0x3CCE7C, 0x54
.global __vt__13TMapEventSink
__vt__13TMapEventSink:
	.incbin "baserom.dol", 0x3CCED0, 0x58
.global __vt__17TMirrorMapDrawBuf
__vt__17TMirrorMapDrawBuf:
	.incbin "baserom.dol", 0x3CCF28, 0x24
.global __vt__19TMirrorModelManager
__vt__19TMirrorModelManager:
	.incbin "baserom.dol", 0x3CCF4C, 0x24
.global __vt__15TMirrorModelObj
__vt__15TMirrorModelObj:
	.incbin "baserom.dol", 0x3CCF70, 0x1C
.global __vt__12TMirrorModel
__vt__12TMirrorModel:
	.incbin "baserom.dol", 0x3CCF8C, 0x1C
.global __vt__13TMirrorCamera
__vt__13TMirrorCamera:
	.incbin "baserom.dol", 0x3CCFA8, 0xC8
.global name_table$2428
name_table$2428:
	.incbin "baserom.dol", 0x3CD070, 0x14
.global __vt__9TMapModel
__vt__9TMapModel:
	.incbin "baserom.dol", 0x3CD084, 0x34
.global __vt__16TMapModelManager
__vt__16TMapModelManager:
	.incbin "baserom.dol", 0x3CD0B8, 0x28
.global __vt__17TMapObjSoundGroup
__vt__17TMapObjSoundGroup:
	.incbin "baserom.dol", 0x3CD0E0, 0x24
.global __vt__14TMapModelActor
__vt__14TMapModelActor:
	.incbin "baserom.dol", 0x3CD104, 0xA4
.global __vt__13TMapStaticObj
__vt__13TMapStaticObj:
	.incbin "baserom.dol", 0x3CD1A8, 0xA8
.global __vt__15TMapWireManager
__vt__15TMapWireManager:
	.incbin "baserom.dol", 0x3CD250, 0x24
.global __vt__13TMapWireActor
__vt__13TMapWireActor:
	.incbin "baserom.dol", 0x3CD274, 0xB4
.global __vt__21TPollutionCounterBase
__vt__21TPollutionCounterBase:
	.incbin "baserom.dol", 0x3CD328, 0x14
.global __vt__20TPollutionCounterObj
__vt__20TPollutionCounterObj:
	.incbin "baserom.dol", 0x3CD33C, 0x14
.global __vt__22TPollutionCounterLayer
__vt__22TPollutionCounterLayer:
	.incbin "baserom.dol", 0x3CD350, 0x18
.global mare_name_table$2805
mare_name_table$2805:
	.incbin "baserom.dol", 0x3CD368, 0x28
.global name_table$2816
name_table$2816:
	.incbin "baserom.dol", 0x3CD390, 0x54
.global __vt__17TPollutionManager
__vt__17TPollutionManager:
	.incbin "baserom.dol", 0x3CD3E4, 0x28
.global $$23043
$$23043:
	.incbin "baserom.dol", 0x3CD40C, 0x1C
.global __vt__25TPollutionLayerWallMinusX
__vt__25TPollutionLayerWallMinusX:
	.incbin "baserom.dol", 0x3CD428, 0x5C
.global __vt__25TPollutionLayerWallMinusZ
__vt__25TPollutionLayerWallMinusZ:
	.incbin "baserom.dol", 0x3CD484, 0x5C
.global __vt__13TPollutionObj
__vt__13TPollutionObj:
	.incbin "baserom.dol", 0x3CD4E0, 0x28
.global __vt__8TShimmer
__vt__8TShimmer:
	.incbin "baserom.dol", 0x3CD508, 0xA0
.global __vt__4TSky
__vt__4TSky:
	.incbin "baserom.dol", 0x3CD5A8, 0xA0
.global __vt__19TMapEventSirenaSink
__vt__19TMapEventSirenaSink:
	.incbin "baserom.dol", 0x3CD648, 0x60
.global x_offset_table$2432
x_offset_table$2432:
	.incbin "baserom.dol", 0x3CD6A8, 0x28
.global z_offset_table$2433
z_offset_table$2433:
	.incbin "baserom.dol", 0x3CD6D0, 0x28
.global __vt__15TPollutionLayer
__vt__15TPollutionLayer:
	.incbin "baserom.dol", 0x3CD6F8, 0x5C
.global __vt__23TPollutionLayerWallBase
__vt__23TPollutionLayerWallBase:
	.incbin "baserom.dol", 0x3CD754, 0x5C
.global __vt__24TPollutionLayerWallPlusX
__vt__24TPollutionLayerWallPlusX:
	.incbin "baserom.dol", 0x3CD7B0, 0x5C
.global __vt__24TPollutionLayerWallPlusZ
__vt__24TPollutionLayerWallPlusZ:
	.incbin "baserom.dol", 0x3CD80C, 0x5C
.global __vt__19TPollutionLayerWave
__vt__19TPollutionLayerWave:
	.incbin "baserom.dol", 0x3CD868, 0x60
.global __vt__14TPollutionTest
__vt__14TPollutionTest:
	.incbin "baserom.dol", 0x3CD8C8, 0x28
.global __vt__11TMarineSnow
__vt__11TMarineSnow:
	.incbin "baserom.dol", 0x3CD8F0, 0xA0
.global __vt__26TDolpicEventRiccoMammaGate
__vt__26TDolpicEventRiccoMammaGate:
	.incbin "baserom.dol", 0x3CD990, 0x40
.global __vt__22TDolpicEventBiancoGate
__vt__22TDolpicEventBiancoGate:
	.incbin "baserom.dol", 0x3CD9D0, 0x40
.global __vt__19TMareEventBumpyWall
__vt__19TMareEventBumpyWall:
	.incbin "baserom.dol", 0x3CDA10, 0x164
.global $$23790
$$23790:
	.incbin "baserom.dol", 0x3CDB74, 0x20
.global __vt__21TMareEventDepressWall
__vt__21TMareEventDepressWall:
	.incbin "baserom.dol", 0x3CDB94, 0x24
.global __vt__18TMareEventWallRock
__vt__18TMareEventWallRock:
	.incbin "baserom.dol", 0x3CDBB8, 0x24
.global __vt__13TMareWallRock
__vt__13TMareWallRock:
	.incbin "baserom.dol", 0x3CDBDC, 0x104
.global clear_z_TX$localstatic0$__ct__22TBathWaterMeshRendererFP22TBathWaterGlobalParamsP10JUTTexture
clear_z_TX$localstatic0$__ct__22TBathWaterMeshRendererFP22TBathWaterGlobalParamsP10JUTTexture:
	.incbin "baserom.dol", 0x3CDCE0, 0x40
.global fileNames__17TBathWaterManager
fileNames__17TBathWaterManager:
	.incbin "baserom.dol", 0x3CDD20, 0x8
.global __vt__17TBathWaterManager
__vt__17TBathWaterManager:
	.incbin "baserom.dol", 0x3CDD28, 0x24
.global __vt__10TBathWater
__vt__10TBathWater:
	.incbin "baserom.dol", 0x3CDD4C, 0xA4
.global __vt__22TBathWaterMeshRenderer
__vt__22TBathWaterMeshRenderer:
	.incbin "baserom.dol", 0x3CDDF0, 0x14
.global __vt__22TBathWaterFlatRenderer
__vt__22TBathWaterFlatRenderer:
	.incbin "baserom.dol", 0x3CDE04, 0x14
.global __vt__18TBathWaterRenderer
__vt__18TBathWaterRenderer:
	.incbin "baserom.dol", 0x3CDE18, 0x14
.global __vt__22TBathWaterPreprocessor
__vt__22TBathWaterPreprocessor:
	.incbin "baserom.dol", 0x3CDE2C, 0x24
.global __vt__19TStickyStainManager
__vt__19TStickyStainManager:
	.incbin "baserom.dol", 0x3CDE50, 0x28
.global __vt__10TFlashPane
__vt__10TFlashPane:
	.incbin "baserom.dol", 0x3CDE78, 0x24
.global __vt__10TMenuPlane
__vt__10TMenuPlane:
	.incbin "baserom.dol", 0x3CDE9C, 0x24
.global __vt__9TMenuBase
__vt__9TMenuBase:
	.incbin "baserom.dol", 0x3CDEC0, 0x28
.global __vt__9TSMSFader
__vt__9TSMSFader:
	.incbin "baserom.dol", 0x3CDEE8, 0x30
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3CDF18, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3CDF24, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3CDF30, 0x10
.global scDolpicNewsDolpic5_1
scDolpicNewsDolpic5_1:
	.incbin "baserom.dol", 0x3CDF40, 0xC
.global scDolpicNewsDolpic5_4
scDolpicNewsDolpic5_4:
	.incbin "baserom.dol", 0x3CDF4C, 0xC
.global scDolpicNewsDolpic6
scDolpicNewsDolpic6:
	.incbin "baserom.dol", 0x3CDF58, 0xC
.global scDolpicNewsDolpic7
scDolpicNewsDolpic7:
	.incbin "baserom.dol", 0x3CDF64, 0xC
.global scDolpicNewsDolpic8_2
scDolpicNewsDolpic8_2:
	.incbin "baserom.dol", 0x3CDF70, 0xC
.global scDolpicNewsDolpic8_3
scDolpicNewsDolpic8_3:
	.incbin "baserom.dol", 0x3CDF7C, 0xC
.global scDolpicNewsDolpic8_4
scDolpicNewsDolpic8_4:
	.incbin "baserom.dol", 0x3CDF88, 0xC
.global scDolpicNewsDolpic8_5
scDolpicNewsDolpic8_5:
	.incbin "baserom.dol", 0x3CDF94, 0xC
.global scDolpicNewsDolpic8_6
scDolpicNewsDolpic8_6:
	.incbin "baserom.dol", 0x3CDFA0, 0x10
.global scDolpicNewsDolpic8_7
scDolpicNewsDolpic8_7:
	.incbin "baserom.dol", 0x3CDFB0, 0xC
.global scDolpicNewsDolpic9
scDolpicNewsDolpic9:
	.incbin "baserom.dol", 0x3CDFBC, 0xC
.global scDolpicNewsDolpic10
scDolpicNewsDolpic10:
	.incbin "baserom.dol", 0x3CDFC8, 0xC
.global MtxCalcTypeName
MtxCalcTypeName:
	.incbin "baserom.dol", 0x3CDFD4, 0x10
.global __vt__11TGCConsole2
__vt__11TGCConsole2:
	.incbin "baserom.dol", 0x3CDFE4, 0x24
.global $$27091
$$27091:
	.incbin "baserom.dol", 0x3CE008, 0x74
.global $$27077
$$27077:
	.incbin "baserom.dol", 0x3CE07C, 0x2C
.global $$27657
$$27657:
	.incbin "baserom.dol", 0x3CE0A8, 0x58
.global cColorTable__8TTalk2D2
cColorTable__8TTalk2D2:
	.incbin "baserom.dol", 0x3CE100, 0x18
.global __vt__8TTalk2D2
__vt__8TTalk2D2:
	.incbin "baserom.dol", 0x3CE118, 0x24
.global $$23886
$$23886:
	.incbin "baserom.dol", 0x3CE13C, 0x24
.global scShineConvTable
scShineConvTable:
	.incbin "baserom.dol", 0x3CE160, 0x28
.global scScenarioNameTable
scScenarioNameTable:
	.incbin "baserom.dol", 0x3CE188, 0x118
.global __vt__11TPauseMenu2
__vt__11TPauseMenu2:
	.incbin "baserom.dol", 0x3CE2A0, 0x28
.global __vt__16TSwitchHelpActor
__vt__16TSwitchHelpActor:
	.incbin "baserom.dol", 0x3CE2C8, 0xA8
.global __vt__10THelpActor
__vt__10THelpActor:
	.incbin "baserom.dol", 0x3CE370, 0xA8
.global cMessageID__9TCardSave
cMessageID__9TCardSave:
	.incbin "baserom.dol", 0x3CE418, 0xD4
.global __vt__9TCardSave
__vt__9TCardSave:
	.incbin "baserom.dol", 0x3CE4EC, 0x24
.global $$24199
$$24199:
	.incbin "baserom.dol", 0x3CE510, 0xD8
.global scShineConvTable
scShineConvTable:
	.incbin "baserom.dol", 0x3CE5E8, 0x28
.global scEtcShineConvTable
scEtcShineConvTable:
	.incbin "baserom.dol", 0x3CE610, 0x28
.global cMessageID__9TCardLoad
cMessageID__9TCardLoad:
	.incbin "baserom.dol", 0x3CE638, 0xD4
.global __vt__9TCardLoad
__vt__9TCardLoad:
	.incbin "baserom.dol", 0x3CE70C, 0x24
.global $$24549
$$24549:
	.incbin "baserom.dol", 0x3CE730, 0xCC
.global $$27209
$$27209:
	.incbin "baserom.dol", 0x3CE7FC, 0x20
.global $$27651
$$27651:
	.incbin "baserom.dol", 0x3CE81C, 0x28
.global $$27648
$$27648:
	.incbin "baserom.dol", 0x3CE844, 0x2C
.global scShineConvTable
scShineConvTable:
	.incbin "baserom.dol", 0x3CE870, 0x28
.global scScenarioNameTable
scScenarioNameTable:
	.incbin "baserom.dol", 0x3CE898, 0x118
.global __vt__11TConsoleStr
__vt__11TConsoleStr:
	.incbin "baserom.dol", 0x3CE9B0, 0x28
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3CE9D8, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3CE9E4, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3CE9F0, 0x10
.global scShineConvTable
scShineConvTable:
	.incbin "baserom.dol", 0x3CEA00, 0x28
.global scEtcShineConvTable
scEtcShineConvTable:
	.incbin "baserom.dol", 0x3CEA28, 0x28
.global scScenarioNameTable
scScenarioNameTable:
	.incbin "baserom.dol", 0x3CEA50, 0x118
.global scNormalStageTable
scNormalStageTable:
	.incbin "baserom.dol", 0x3CEB68, 0x28
.global __vt__11TSelectMenu
__vt__11TSelectMenu:
	.incbin "baserom.dol", 0x3CEB90, 0x24
.global $$23617
$$23617:
	.incbin "baserom.dol", 0x3CEBB4, 0x4C
.global __vt__11TSelectGrad
__vt__11TSelectGrad:
	.incbin "baserom.dol", 0x3CEC00, 0x28
.global __vt__10TSelectDir
__vt__10TSelectDir:
	.incbin "baserom.dol", 0x3CEC28, 0x70
.global __vt__12TSelectShine
__vt__12TSelectShine:
	.incbin "baserom.dol", 0x3CEC98, 0x10
.global __vt__19TSelectShineManager
__vt__19TSelectShineManager:
	.incbin "baserom.dol", 0x3CECA8, 0x28
.global __vt__10TBlendPane
__vt__10TBlendPane:
	.incbin "baserom.dol", 0x3CECD0, 0x10
.global scShineConvTable
scShineConvTable:
	.incbin "baserom.dol", 0x3CECE0, 0x28
.global scEtcShineConvTable
scEtcShineConvTable:
	.incbin "baserom.dol", 0x3CED08, 0x28
.global scNormalStageTable
scNormalStageTable:
	.incbin "baserom.dol", 0x3CED30, 0x28
.global __vt__6TGuide
__vt__6TGuide:
	.incbin "baserom.dol", 0x3CED58, 0x24
.global $$22657
$$22657:
	.incbin "baserom.dol", 0x3CED7C, 0x34
.global __vt__9TSunShine
__vt__9TSunShine:
	.incbin "baserom.dol", 0x3CEDB0, 0x28
.global __vt__9TSunGlass
__vt__9TSunGlass:
	.incbin "baserom.dol", 0x3CEDD8, 0x28
.global __vt__11TShineFader
__vt__11TShineFader:
	.incbin "baserom.dol", 0x3CEE00, 0x30
.global __vt__11TProgSelect
__vt__11TProgSelect:
	.incbin "baserom.dol", 0x3CEE30, 0x28
.global camLoc$96
camLoc$96:
	.incbin "baserom.dol", 0x3CEE58, 0xC
.global objPt$97
objPt$97:
	.incbin "baserom.dol", 0x3CEE64, 0xC
.global up$98
up$98:
	.incbin "baserom.dol", 0x3CEE70, 0xC
.global handle_table
handle_table:
	.incbin "baserom.dol", 0x3CEE7C, 0x3C
.global handle_type
handle_type:
	.incbin "baserom.dol", 0x3CEEB8, 0x10
.global boundtable$262
boundtable$262:
	.incbin "baserom.dol", 0x3CEEC8, 0x38
.global drawpath_table
drawpath_table:
	.incbin "baserom.dol", 0x3CEF00, 0x144
.global $$2943
$$2943:
	.incbin "baserom.dol", 0x3CF044, 0x24
.global __vt__14TMovieSubTitle
__vt__14TMovieSubTitle:
	.incbin "baserom.dol", 0x3CF068, 0x28
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3CF090, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3CF09C, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3CF0A8, 0x10
.global cSoundSettings__16TOptionSoundUnit
cSoundSettings__16TOptionSoundUnit:
	.incbin "baserom.dol", 0x3CF0B8, 0x18
.global cFlagInfos__16TOptionSoundUnit
cFlagInfos__16TOptionSoundUnit:
	.incbin "baserom.dol", 0x3CF0D0, 0x18
.global $$22212
$$22212:
	.incbin "baserom.dol", 0x3CF0E8, 0xC
.global $$22214
$$22214:
	.incbin "baserom.dol", 0x3CF0F4, 0xC
.global $$22216
$$22216:
	.incbin "baserom.dol", 0x3CF100, 0xC
.global $$22218
$$22218:
	.incbin "baserom.dol", 0x3CF10C, 0xC
.global $$22221
$$22221:
	.incbin "baserom.dol", 0x3CF118, 0xC
.global $$22223
$$22223:
	.incbin "baserom.dol", 0x3CF124, 0xC
.global $$22225
$$22225:
	.incbin "baserom.dol", 0x3CF130, 0xC
.global $$22227
$$22227:
	.incbin "baserom.dol", 0x3CF13C, 0xC
.global $$22230
$$22230:
	.incbin "baserom.dol", 0x3CF148, 0xC
.global $$22232
$$22232:
	.incbin "baserom.dol", 0x3CF154, 0xC
.global $$22234
$$22234:
	.incbin "baserom.dol", 0x3CF160, 0xC
.global $$22236
$$22236:
	.incbin "baserom.dol", 0x3CF16C, 0x1C
.global __vt__12TMovieRumble
__vt__12TMovieRumble:
	.incbin "baserom.dol", 0x3CF188, 0x28
.global __vt__10TConductor
__vt__10TConductor:
	.incbin "baserom.dol", 0x3CF1B0, 0x28
.global entry$2672
entry$2672:
	.incbin "baserom.dol", 0x3CF1D8, 0x18
.global entry$2684
entry$2684:
	.incbin "baserom.dol", 0x3CF1F0, 0x18
.global entry$2697
entry$2697:
	.incbin "baserom.dol", 0x3CF208, 0x18
.global entry$2706
entry$2706:
	.incbin "baserom.dol", 0x3CF220, 0x18
.global __vt__16TEffectExplosion
__vt__16TEffectExplosion:
	.incbin "baserom.dol", 0x3CF238, 0x114
.global __vt__19TEffectModelManager
__vt__19TEffectModelManager:
	.incbin "baserom.dol", 0x3CF34C, 0x54
.global __vt__23TEffectExplosionManager
__vt__23TEffectExplosionManager:
	.incbin "baserom.dol", 0x3CF3A0, 0x54
.global __vt__16TEffectColumSand
__vt__16TEffectColumSand:
	.incbin "baserom.dol", 0x3CF3F4, 0x114
.global __vt__23TEffectColumSandManager
__vt__23TEffectColumSandManager:
	.incbin "baserom.dol", 0x3CF508, 0x54
.global __vt__21TEffectBombColumWater
__vt__21TEffectBombColumWater:
	.incbin "baserom.dol", 0x3CF55C, 0x114
.global __vt__28TEffectBombColumWaterManager
__vt__28TEffectBombColumWaterManager:
	.incbin "baserom.dol", 0x3CF670, 0x54
.global __vt__17TEffectColumWater
__vt__17TEffectColumWater:
	.incbin "baserom.dol", 0x3CF6C4, 0x114
.global __vt__24TEffectColumWaterManager
__vt__24TEffectColumWaterManager:
	.incbin "baserom.dol", 0x3CF7D8, 0x54
.global __vt__12TEffectModel
__vt__12TEffectModel:
	.incbin "baserom.dol", 0x3CF82C, 0x114
.global __vt__17TEffectObjManager
__vt__17TEffectObjManager:
	.incbin "baserom.dol", 0x3CF940, 0x24
.global __vt__14TEffectObjBase
__vt__14TEffectObjBase:
	.incbin "baserom.dol", 0x3CF964, 0xB4
.global __vt__19TEffectBiancoFunsui
__vt__19TEffectBiancoFunsui:
	.incbin "baserom.dol", 0x3CFA18, 0xA4
.global __vt__18TEffectPinnaFunsui
__vt__18TEffectPinnaFunsui:
	.incbin "baserom.dol", 0x3CFABC, 0xA4
.global __vt__13TSimpleEffect
__vt__13TSimpleEffect:
	.incbin "baserom.dol", 0x3CFB60, 0xA8
.global __vt__14TEMarioManager
__vt__14TEMarioManager:
	.incbin "baserom.dol", 0x3CFC08, 0x54
.global __vt__7TEMario
__vt__7TEMario:
	.incbin "baserom.dol", 0x3CFC5C, 0x114
.global __vt__11TSpineEnemy
__vt__11TSpineEnemy:
	.incbin "baserom.dol", 0x3CFD70, 0x114
.global __vt__41TSolidStack$$0PC24TNerveBase$$010TLiveActor$$1$$1
__vt__41TSolidStack$$0PC24TNerveBase$$010TLiveActor$$1$$1:
	.incbin "baserom.dol", 0x3CFE84, 0xC
.global __vt__24TSpineBase$$010TLiveActor$$1
__vt__24TSpineBase$$010TLiveActor$$1:
	.incbin "baserom.dol", 0x3CFE90, 0x10
.global __vt__23TSolidStack$$09TPathNode$$1
__vt__23TSolidStack$$09TPathNode$$1:
	.incbin "baserom.dol", 0x3CFEA0, 0x10
.global __vt__18TEnemyPolluteModel
__vt__18TEnemyPolluteModel:
	.incbin "baserom.dol", 0x3CFEB0, 0x28
.global __vt__25TEnemyPolluteModelManager
__vt__25TEnemyPolluteModelManager:
	.incbin "baserom.dol", 0x3CFED8, 0x28
.global __vt__16TEnemyAttachment
__vt__16TEnemyAttachment:
	.incbin "baserom.dol", 0x3CFF00, 0x148
.global __vt__13TEnemyManager
__vt__13TEnemyManager:
	.incbin "baserom.dol", 0x3D0048, 0x58
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3D00A0, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3D00AC, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3D00B8, 0x10
.global MtxCalcTypeName
MtxCalcTypeName:
	.incbin "baserom.dol", 0x3D00C8, 0x10
.global marioAnimeFiles
marioAnimeFiles:
	.incbin "baserom.dol", 0x3D00D8, 0x638
.global recordFileNamesDolpic1
recordFileNamesDolpic1:
	.incbin "baserom.dol", 0x3D0710, 0x20
.global recordFileNamesMonteMan
recordFileNamesMonteMan:
	.incbin "baserom.dol", 0x3D0730, 0xC
.global names$3274
names$3274:
	.incbin "baserom.dol", 0x3D073C, 0x14
.global bmdFileNames$3287
bmdFileNames$3287:
	.incbin "baserom.dol", 0x3D0750, 0x1C
.global __vt__11TEnemyMario
__vt__11TEnemyMario:
	.incbin "baserom.dol", 0x3D076C, 0xEC
.global $$24414
$$24414:
	.incbin "baserom.dol", 0x3D0858, 0x3C
.global $$24674
$$24674:
	.incbin "baserom.dol", 0x3D0894, 0x1B4
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3D0A48, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3D0A54, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3D0A60, 0x10
.global __vt__15TMtxCalcFootInv
__vt__15TMtxCalcFootInv:
	.incbin "baserom.dol", 0x3D0A70, 0xC8
.global rikuGesso_bastable
rikuGesso_bastable:
	.incbin "baserom.dol", 0x3D0B38, 0x5C
.global entry$2957
entry$2957:
	.incbin "baserom.dol", 0x3D0B94, 0x18
.global __vt__15TNerveGessoTurn
__vt__15TNerveGessoTurn:
	.incbin "baserom.dol", 0x3D0BAC, 0x10
.global __vt__15TNerveGessoLand
__vt__15TNerveGessoLand:
	.incbin "baserom.dol", 0x3D0BBC, 0x10
.global __vt__20TNerveGessoFindMario
__vt__20TNerveGessoFindMario:
	.incbin "baserom.dol", 0x3D0BCC, 0x10
.global __vt__18TNerveGessoRolling
__vt__18TNerveGessoRolling:
	.incbin "baserom.dol", 0x3D0BDC, 0x10
.global __vt__15TNerveGessoFall
__vt__15TNerveGessoFall:
	.incbin "baserom.dol", 0x3D0BEC, 0x10
.global __vt__18TNerveGessoPollute
__vt__18TNerveGessoPollute:
	.incbin "baserom.dol", 0x3D0BFC, 0x10
.global __vt__16TNerveGessoPunch
__vt__16TNerveGessoPunch:
	.incbin "baserom.dol", 0x3D0C0C, 0x10
.global __vt__17TNerveGessoFreeze
__vt__17TNerveGessoFreeze:
	.incbin "baserom.dol", 0x3D0C1C, 0x10
.global __vt__15TNerveGessoStay
__vt__15TNerveGessoStay:
	.incbin "baserom.dol", 0x3D0C2C, 0x10
.global __vt__16TGessoPolluteObj
__vt__16TGessoPolluteObj:
	.incbin "baserom.dol", 0x3D0C3C, 0x148
.global __vt__10TLandGesso
__vt__10TLandGesso:
	.incbin "baserom.dol", 0x3D0D84, 0x1B8
.global __vt__10TSurfGesso
__vt__10TSurfGesso:
	.incbin "baserom.dol", 0x3D0F3C, 0x1B8
.global __vt__6TGesso
__vt__6TGesso:
	.incbin "baserom.dol", 0x3D10F4, 0x1B8
.global __vt__13TGessoManager
__vt__13TGessoManager:
	.incbin "baserom.dol", 0x3D12AC, 0x58
.global __vt__18TGessoPolluteModel
__vt__18TGessoPolluteModel:
	.incbin "baserom.dol", 0x3D1304, 0x28
.global __vt__25TGessoPolluteModelManager
__vt__25TGessoPolluteModelManager:
	.incbin "baserom.dol", 0x3D132C, 0x2C
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3D1358, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3D1364, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3D1370, 0x10
.global MtxCalcTypeName
MtxCalcTypeName:
	.incbin "baserom.dol", 0x3D1380, 0x10
.global grDummyRail
grDummyRail:
	.incbin "baserom.dol", 0x3D1390, 0xCC
.global __vt__9TGraphWeb
__vt__9TGraphWeb:
	.incbin "baserom.dol", 0x3D145C, 0xC
.global hanekuri_bastable
hanekuri_bastable:
	.incbin "baserom.dol", 0x3D1468, 0x10
.global hamukurianm_bastable
hamukurianm_bastable:
	.incbin "baserom.dol", 0x3D1478, 0x3C
.global entry$3048
entry$3048:
	.incbin "baserom.dol", 0x3D14B4, 0x18
.global entry$3115
entry$3115:
	.incbin "baserom.dol", 0x3D14CC, 0x18
.global entry$3125
entry$3125:
	.incbin "baserom.dol", 0x3D14E4, 0x18
.global entry$3156
entry$3156:
	.incbin "baserom.dol", 0x3D14FC, 0x18
.global entry$3168
entry$3168:
	.incbin "baserom.dol", 0x3D1514, 0x18
.global __vt__27TNerveDoroHanePrepareAttack
__vt__27TNerveDoroHanePrepareAttack:
	.incbin "baserom.dol", 0x3D152C, 0x10
.global __vt__22TNerveDoroHaneHitWater
__vt__22TNerveDoroHaneHitWater:
	.incbin "baserom.dol", 0x3D153C, 0x10
.global __vt__18TNerveDoroHaneRise
__vt__18TNerveDoroHaneRise:
	.incbin "baserom.dol", 0x3D154C, 0x10
.global __vt__25TNerveFireHamuKuriRecover
__vt__25TNerveFireHamuKuriRecover:
	.incbin "baserom.dol", 0x3D155C, 0x10
.global __vt__24TNerveDoroHamuKuriRobCap
__vt__24TNerveDoroHamuKuriRobCap:
	.incbin "baserom.dol", 0x3D156C, 0x10
.global __vt__29TNerveHaneHamuKuriMoveOnGraph
__vt__29TNerveHaneHamuKuriMoveOnGraph:
	.incbin "baserom.dol", 0x3D157C, 0x10
.global __vt__24TNerveHaneHamuKuriUpWait
__vt__24TNerveHaneHamuKuriUpWait:
	.incbin "baserom.dol", 0x3D158C, 0x10
.global __vt__23TNerveDangoHamuKuriWait
__vt__23TNerveDangoHamuKuriWait:
	.incbin "baserom.dol", 0x3D159C, 0x10
.global __vt__22TNerveHamuKuriJitabata
__vt__22TNerveHamuKuriJitabata:
	.incbin "baserom.dol", 0x3D15AC, 0x10
.global __vt__18TNerveHamuKuriLand
__vt__18TNerveHamuKuriLand:
	.incbin "baserom.dol", 0x3D15BC, 0x10
.global __vt__21TNerveHamuKuriWallDie
__vt__21TNerveHamuKuriWallDie:
	.incbin "baserom.dol", 0x3D15CC, 0x10
.global __vt__25TNerveHamuKuriBoundFreeze
__vt__25TNerveHamuKuriBoundFreeze:
	.incbin "baserom.dol", 0x3D15DC, 0x10
.global __vt__30TNerveHamuKuriGoForSearchActor
__vt__30TNerveHamuKuriGoForSearchActor:
	.incbin "baserom.dol", 0x3D15EC, 0x10
.global __vt__13TDoroHamuKuri
__vt__13TDoroHamuKuri:
	.incbin "baserom.dol", 0x3D15FC, 0x1C8
.global __vt__13TFireHamuKuri
__vt__13TFireHamuKuri:
	.incbin "baserom.dol", 0x3D17C4, 0x1C8
.global __vt__18TBossDangoHamuKuri
__vt__18TBossDangoHamuKuri:
	.incbin "baserom.dol", 0x3D198C, 0x1C8
.global __vt__14TDangoHamuKuri
__vt__14TDangoHamuKuri:
	.incbin "baserom.dol", 0x3D1B54, 0x1C8
.global __vt__14THaneHamuKuri2
__vt__14THaneHamuKuri2:
	.incbin "baserom.dol", 0x3D1D1C, 0x1C8
.global __vt__13TDoroHaneKuri
__vt__13TDoroHaneKuri:
	.incbin "baserom.dol", 0x3D1EE4, 0x1C8
.global __vt__13THaneHamuKuri
__vt__13THaneHamuKuri:
	.incbin "baserom.dol", 0x3D20AC, 0x1C8
.global __vt__9THamuKuri
__vt__9THamuKuri:
	.incbin "baserom.dol", 0x3D2274, 0x1C8
.global __vt__20TDoroHamuKuriManager
__vt__20TDoroHamuKuriManager:
	.incbin "baserom.dol", 0x3D243C, 0x58
.global __vt__9TDoroHige
__vt__9TDoroHige:
	.incbin "baserom.dol", 0x3D2494, 0x24
.global __vt__20TFireHamuKuriManager
__vt__20TFireHamuKuriManager:
	.incbin "baserom.dol", 0x3D24B8, 0x58
.global __vt__25TBossDangoHamuKuriManager
__vt__25TBossDangoHamuKuriManager:
	.incbin "baserom.dol", 0x3D2510, 0x58
.global __vt__21TDangoHamuKuriManager
__vt__21TDangoHamuKuriManager:
	.incbin "baserom.dol", 0x3D2568, 0x58
.global __vt__20TDoroHaneKuriManager
__vt__20TDoroHaneKuriManager:
	.incbin "baserom.dol", 0x3D25C0, 0x58
.global __vt__20THaneHamuKuriManager
__vt__20THaneHamuKuriManager:
	.incbin "baserom.dol", 0x3D2618, 0x58
.global __vt__16THamuKuriManager
__vt__16THamuKuriManager:
	.incbin "baserom.dol", 0x3D2670, 0x58
.global __vt__17THamuKuriLauncher
__vt__17THamuKuriLauncher:
	.incbin "baserom.dol", 0x3D26C8, 0x130
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3D27F8, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3D2804, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3D2810, 0x10
.global MtxCalcTypeName
MtxCalcTypeName:
	.incbin "baserom.dol", 0x3D2820, 0x10
.global hinokuri2_bastable
hinokuri2_bastable:
	.incbin "baserom.dol", 0x3D2830, 0x70
.global __vt__18TNerveHino2WaitAnm
__vt__18TNerveHino2WaitAnm:
	.incbin "baserom.dol", 0x3D28A0, 0xDC
.global __vt__17TNerveHino2Freeze
__vt__17TNerveHino2Freeze:
	.incbin "baserom.dol", 0x3D297C, 0x10
.global __vt__16TNerveHino2Stamp
__vt__16TNerveHino2Stamp:
	.incbin "baserom.dol", 0x3D298C, 0x10
.global __vt__14TNerveHino2Die
__vt__14TNerveHino2Die:
	.incbin "baserom.dol", 0x3D299C, 0x10
.global __vt__16TNerveHino2Burst
__vt__16TNerveHino2Burst:
	.incbin "baserom.dol", 0x3D29AC, 0x10
.global __vt__16TNerveHino2Squat
__vt__16TNerveHino2Squat:
	.incbin "baserom.dol", 0x3D29BC, 0x10
.global __vt__17TNerveHino2Damage
__vt__17TNerveHino2Damage:
	.incbin "baserom.dol", 0x3D29CC, 0x10
.global __vt__18TNerveHino2Pollute
__vt__18TNerveHino2Pollute:
	.incbin "baserom.dol", 0x3D29DC, 0x10
.global __vt__17TNerveHino2PrePol
__vt__17TNerveHino2PrePol:
	.incbin "baserom.dol", 0x3D29EC, 0x10
.global __vt__15TNerveHino2Turn
__vt__15TNerveHino2Turn:
	.incbin "baserom.dol", 0x3D29FC, 0x10
.global __vt__18TNerveHino2Landing
__vt__18TNerveHino2Landing:
	.incbin "baserom.dol", 0x3D2A0C, 0x10
.global __vt__17TNerveHino2JumpIn
__vt__17TNerveHino2JumpIn:
	.incbin "baserom.dol", 0x3D2A1C, 0x10
.global __vt__14TNerveHino2Fly
__vt__14TNerveHino2Fly:
	.incbin "baserom.dol", 0x3D2A2C, 0x10
.global __vt__22TNerveHino2GraphWander
__vt__22TNerveHino2GraphWander:
	.incbin "baserom.dol", 0x3D2A3C, 0x10
.global __vt__17TNerveHino2Appear
__vt__17TNerveHino2Appear:
	.incbin "baserom.dol", 0x3D2A4C, 0x10
.global __vt__10THinokuri2
__vt__10THinokuri2:
	.incbin "baserom.dol", 0x3D2A5C, 0x114
.global __vt__9THino2Hit
__vt__9THino2Hit:
	.incbin "baserom.dol", 0x3D2B70, 0xA4
.global __vt__13THino2MtxCalc
__vt__13THino2MtxCalc:
	.incbin "baserom.dol", 0x3D2C14, 0x54
.global __vt__17THinokuri2Manager
__vt__17THinokuri2Manager:
	.incbin "baserom.dol", 0x3D2C68, 0x58
.global mameGesso_bastable
mameGesso_bastable:
	.incbin "baserom.dol", 0x3D2CC0, 0x4C
.global __vt__19TNerveMameGessoWait
__vt__19TNerveMameGessoWait:
	.incbin "baserom.dol", 0x3D2D0C, 0x10
.global __vt__21TNerveMameGessoObject
__vt__21TNerveMameGessoObject:
	.incbin "baserom.dol", 0x3D2D1C, 0x10
.global __vt__21TNerveMameGessoThrown
__vt__21TNerveMameGessoThrown:
	.incbin "baserom.dol", 0x3D2D2C, 0x10
.global __vt__21TNerveMameGessoPickUp
__vt__21TNerveMameGessoPickUp:
	.incbin "baserom.dol", 0x3D2D3C, 0x10
.global __vt__22TNerveMameGessoTrample
__vt__22TNerveMameGessoTrample:
	.incbin "baserom.dol", 0x3D2D4C, 0x10
.global __vt__23TNerveMameGessoJitabata
__vt__23TNerveMameGessoJitabata:
	.incbin "baserom.dol", 0x3D2D5C, 0x10
.global __vt__21TNerveMameGessoDamage
__vt__21TNerveMameGessoDamage:
	.incbin "baserom.dol", 0x3D2D6C, 0x10
.global __vt__30TNerveMameGessoGraphJumpWander
__vt__30TNerveMameGessoGraphJumpWander:
	.incbin "baserom.dol", 0x3D2D7C, 0x10
.global __vt__10TMameGesso
__vt__10TMameGesso:
	.incbin "baserom.dol", 0x3D2D8C, 0x1B8
.global __vt__17TMameGessoManager
__vt__17TMameGessoManager:
	.incbin "baserom.dol", 0x3D2F44, 0x5C
.global namekuri2_bastable
namekuri2_bastable:
	.incbin "baserom.dol", 0x3D2FA0, 0x20
.global entry$2947
entry$2947:
	.incbin "baserom.dol", 0x3D2FC0, 0x18
.global __vt__19TNerveNKFollowMario
__vt__19TNerveNKFollowMario:
	.incbin "baserom.dol", 0x3D2FD8, 0x10
.global __vt__18TDiffusionNameKuri
__vt__18TDiffusionNameKuri:
	.incbin "baserom.dol", 0x3D2FE8, 0x1B8
.global __vt__25TNerveNameKuriDrawPollute
__vt__25TNerveNameKuriDrawPollute:
	.incbin "baserom.dol", 0x3D31A0, 0x10
.global __vt__21TNerveNameKuriDiffuse
__vt__21TNerveNameKuriDiffuse:
	.incbin "baserom.dol", 0x3D31B0, 0x10
.global __vt__23TNerveNameKuriExplosion
__vt__23TNerveNameKuriExplosion:
	.incbin "baserom.dol", 0x3D31C0, 0x10
.global __vt__31TNerveNameKuriJumpAttackPrepare
__vt__31TNerveNameKuriJumpAttackPrepare:
	.incbin "baserom.dol", 0x3D31D0, 0x10
.global __vt__24TNerveNameKuriJumpAttack
__vt__24TNerveNameKuriJumpAttack:
	.incbin "baserom.dol", 0x3D31E0, 0x10
.global __vt__18TNerveNameKuriLand
__vt__18TNerveNameKuriLand:
	.incbin "baserom.dol", 0x3D31F0, 0x10
.global __vt__25TDiffusionNameKuriManager
__vt__25TDiffusionNameKuriManager:
	.incbin "baserom.dol", 0x3D3200, 0x58
.global __vt__9TNameKuri
__vt__9TNameKuri:
	.incbin "baserom.dol", 0x3D3258, 0x1B8
.global __vt__19TNameIndParCallback
__vt__19TNameIndParCallback:
	.incbin "baserom.dol", 0x3D3410, 0x10
.global __vt__16TNameKuriManager
__vt__16TNameKuriManager:
	.incbin "baserom.dol", 0x3D3420, 0x58
.global __vt__17TNameKuriLauncher
__vt__17TNameKuriLauncher:
	.incbin "baserom.dol", 0x3D3478, 0x130
.global pakkun_bastable
pakkun_bastable:
	.incbin "baserom.dol", 0x3D35A8, 0x28
.global entry$3011
entry$3011:
	.incbin "baserom.dol", 0x3D35D0, 0x18
.global __vt__22TNerveStayPakkunAppear
__vt__22TNerveStayPakkunAppear:
	.incbin "baserom.dol", 0x3D35E8, 0x10
.global __vt__20TNerveStayPakkunHide
__vt__20TNerveStayPakkunHide:
	.incbin "baserom.dol", 0x3D35F8, 0x10
.global __vt__18TNervePakkunFreeze
__vt__18TNervePakkunFreeze:
	.incbin "baserom.dol", 0x3D3608, 0x10
.global __vt__17TNervePakkunShoot
__vt__17TNervePakkunShoot:
	.incbin "baserom.dol", 0x3D3618, 0x10
.global __vt__16TNervePakkunHide
__vt__16TNervePakkunHide:
	.incbin "baserom.dol", 0x3D3628, 0x10
.global __vt__18TNervePakkunAppear
__vt__18TNervePakkunAppear:
	.incbin "baserom.dol", 0x3D3638, 0x10
.global __vt__16TNervePakkunStay
__vt__16TNervePakkunStay:
	.incbin "baserom.dol", 0x3D3648, 0x10
.global __vt__20TNervePakkunGenerate
__vt__20TNervePakkunGenerate:
	.incbin "baserom.dol", 0x3D3658, 0x10
.global __vt__11TStayPakkun
__vt__11TStayPakkun:
	.incbin "baserom.dol", 0x3D3668, 0x1B0
.global __vt__11TPakkunSeed
__vt__11TPakkunSeed:
	.incbin "baserom.dol", 0x3D3818, 0x148
.global __vt__7TPakkun
__vt__7TPakkun:
	.incbin "baserom.dol", 0x3D3960, 0x1B0
.global __vt__14TPakkunManager
__vt__14TPakkunManager:
	.incbin "baserom.dol", 0x3D3B10, 0x58
.global entry$3004
entry$3004:
	.incbin "baserom.dol", 0x3D3B68, 0x18
.global __vt__22TNerveSmallEnemyChange
__vt__22TNerveSmallEnemyChange:
	.incbin "baserom.dol", 0x3D3B80, 0x10
.global __vt__20TNerveSmallEnemyWait
__vt__20TNerveSmallEnemyWait:
	.incbin "baserom.dol", 0x3D3B90, 0x10
.global __vt__28TNerveSmallEnemyHitWaterJump
__vt__28TNerveSmallEnemyHitWaterJump:
	.incbin "baserom.dol", 0x3D3BA0, 0x10
.global __vt__20TNerveSmallEnemyJump
__vt__20TNerveSmallEnemyJump:
	.incbin "baserom.dol", 0x3D3BB0, 0x10
.global __vt__22TNerveSmallEnemyFreeze
__vt__22TNerveSmallEnemyFreeze:
	.incbin "baserom.dol", 0x3D3BC0, 0x10
.global __vt__19TNerveSmallEnemyDie
__vt__19TNerveSmallEnemyDie:
	.incbin "baserom.dol", 0x3D3BD0, 0x10
.global __vt__11TSmallEnemy
__vt__11TSmallEnemy:
	.incbin "baserom.dol", 0x3D3BE0, 0x1A8
.global __vt__18TSmallEnemyManager
__vt__18TSmallEnemyManager:
	.incbin "baserom.dol", 0x3D3D88, 0x58
.global __vt__7TSpider
__vt__7TSpider:
	.incbin "baserom.dol", 0x3D3DE0, 0x10
.global __vt__24TNerveTypicalGraphWander
__vt__24TNerveTypicalGraphWander:
	.incbin "baserom.dol", 0x3D3DF0, 0x10
.global __vt__15TTypicalManager
__vt__15TTypicalManager:
	.incbin "baserom.dol", 0x3D3E00, 0x54
.global __vt__13TTypicalEnemy
__vt__13TTypicalEnemy:
	.incbin "baserom.dol", 0x3D3E54, 0x114
.global __vt__7TWalker
__vt__7TWalker:
	.incbin "baserom.dol", 0x3D3F68, 0x10
.global __vt__29TSolidStack$$0PC12TBGCheckData$$1
__vt__29TSolidStack$$0PC12TBGCheckData$$1:
	.incbin "baserom.dol", 0x3D3F78, 0x10
.global __vt__22TNerveWalkerTraceMario
__vt__22TNerveWalkerTraceMario:
	.incbin "baserom.dol", 0x3D3F88, 0x10
.global __vt__18TNerveWalkerEscape
__vt__18TNerveWalkerEscape:
	.incbin "baserom.dol", 0x3D3F98, 0x10
.global __vt__22TNerveWalkerPostAttack
__vt__22TNerveWalkerPostAttack:
	.incbin "baserom.dol", 0x3D3FA8, 0x10
.global __vt__18TNerveWalkerAttack
__vt__18TNerveWalkerAttack:
	.incbin "baserom.dol", 0x3D3FB8, 0x10
.global __vt__23TNerveWalkerGraphWander
__vt__23TNerveWalkerGraphWander:
	.incbin "baserom.dol", 0x3D3FC8, 0x10
.global __vt__20TNerveWalkerGenerate
__vt__20TNerveWalkerGenerate:
	.incbin "baserom.dol", 0x3D3FD8, 0x10
.global __vt__12TWalkerEnemy
__vt__12TWalkerEnemy:
	.incbin "baserom.dol", 0x3D3FE8, 0x1B8
.global bgeso_bastable
bgeso_bastable:
	.incbin "baserom.dol", 0x3D41A0, 0x70
.global __vt__12TNerveBGRoll
__vt__12TNerveBGRoll:
	.incbin "baserom.dol", 0x3D4210, 0x10
.global $$26268
$$26268:
	.incbin "baserom.dol", 0x3D4220, 0x20
.global $$27548
$$27548:
	.incbin "baserom.dol", 0x3D4240, 0x20
.global __vt__15TNerveBGPolDrop
__vt__15TNerveBGPolDrop:
	.incbin "baserom.dol", 0x3D4260, 0x10
.global __vt__15TNerveBGPollute
__vt__15TNerveBGPollute:
	.incbin "baserom.dol", 0x3D4270, 0x10
.global __vt__11TNerveBGDie
__vt__11TNerveBGDie:
	.incbin "baserom.dol", 0x3D4280, 0x10
.global __vt__11TNerveBGTug
__vt__11TNerveBGTug:
	.incbin "baserom.dol", 0x3D4290, 0x10
.global __vt__22TNerveBGTentacleDamage
__vt__22TNerveBGTentacleDamage:
	.incbin "baserom.dol", 0x3D42A0, 0x10
.global __vt__18TNerveBGBeakDamage
__vt__18TNerveBGBeakDamage:
	.incbin "baserom.dol", 0x3D42B0, 0x10
.global __vt__17TNerveBGEyeDamage
__vt__17TNerveBGEyeDamage:
	.incbin "baserom.dol", 0x3D42C0, 0x10
.global __vt__12TNerveBGWait
__vt__12TNerveBGWait:
	.incbin "baserom.dol", 0x3D42D0, 0x10
.global __vt__17TBossGessoManager
__vt__17TBossGessoManager:
	.incbin "baserom.dol", 0x3D42E0, 0x54
.global __vt__10TBossGesso
__vt__10TBossGesso:
	.incbin "baserom.dol", 0x3D4334, 0x114
.global __vt__9TBGBinder
__vt__9TBGBinder:
	.incbin "baserom.dol", 0x3D4448, 0x10
.global __vt__17TBossGessoMtxCalc
__vt__17TBossGessoMtxCalc:
	.incbin "baserom.dol", 0x3D4458, 0x44
.global __vt__10TBGBodyHit
__vt__10TBGBodyHit:
	.incbin "baserom.dol", 0x3D449C, 0xA4
.global __vt__9TBGEyeHit
__vt__9TBGEyeHit:
	.incbin "baserom.dol", 0x3D4540, 0xA4
.global __vt__10TBGBeakHit
__vt__10TBGBeakHit:
	.incbin "baserom.dol", 0x3D45E4, 0xB4
.global dennoko_bastable
dennoko_bastable:
	.incbin "baserom.dol", 0x3D4698, 0x48
.global entry$2916
entry$2916:
	.incbin "baserom.dol", 0x3D46E0, 0x18
.global __vt__24TNerveElecCarapaceReturn
__vt__24TNerveElecCarapaceReturn:
	.incbin "baserom.dol", 0x3D46F8, 0x10
.global __vt__22TNerveElecCarapaceWait
__vt__22TNerveElecCarapaceWait:
	.incbin "baserom.dol", 0x3D4708, 0x10
.global __vt__22TNerveElecCarapaceMove
__vt__22TNerveElecCarapaceMove:
	.incbin "baserom.dol", 0x3D4718, 0x10
.global __vt__24TNerveElecNokonokoAttack
__vt__24TNerveElecNokonokoAttack:
	.incbin "baserom.dol", 0x3D4728, 0x10
.global __vt__25TNerveElecNokonokoRebirth
__vt__25TNerveElecNokonokoRebirth:
	.incbin "baserom.dol", 0x3D4738, 0x10
.global __vt__24TNerveElecNokonokoFreeze
__vt__24TNerveElecNokonokoFreeze:
	.incbin "baserom.dol", 0x3D4748, 0x10
.global __vt__22TNerveElecNokonokoTurn
__vt__22TNerveElecNokonokoTurn:
	.incbin "baserom.dol", 0x3D4758, 0x10
.global __vt__25TNerveElecNokonokoCollect
__vt__25TNerveElecNokonokoCollect:
	.incbin "baserom.dol", 0x3D4768, 0x10
.global __vt__23TNerveElecNokonokoShoot
__vt__23TNerveElecNokonokoShoot:
	.incbin "baserom.dol", 0x3D4778, 0x10
.global __vt__13TElecCarapace
__vt__13TElecCarapace:
	.incbin "baserom.dol", 0x3D4788, 0x148
.global __vt__13TElecNokonoko
__vt__13TElecNokonoko:
	.incbin "baserom.dol", 0x3D48D0, 0x1BC
.global __vt__20TElecNokonokoManager
__vt__20TElecNokonokoManager:
	.incbin "baserom.dol", 0x3D4A8C, 0x5C
.global telesa_bastable
telesa_bastable:
	.incbin "baserom.dol", 0x3D4AE8, 0x1C
.global entry$2992
entry$2992:
	.incbin "baserom.dol", 0x3D4B04, 0x18
.global entry$3402
entry$3402:
	.incbin "baserom.dol", 0x3D4B1C, 0x18
.global __vt__25TNerveKageMarioModokiWait
__vt__25TNerveKageMarioModokiWait:
	.incbin "baserom.dol", 0x3D4B34, 0x10
.global $$24007
$$24007:
	.incbin "baserom.dol", 0x3D4B44, 0x34
.global __vt__16TKageMarioModoki
__vt__16TKageMarioModoki:
	.incbin "baserom.dol", 0x3D4B78, 0x1B8
.global __vt__23TKageMarioModokiManager
__vt__23TKageMarioModokiManager:
	.incbin "baserom.dol", 0x3D4D30, 0x58
.global __vt__19TNerveTelesaTrample
__vt__19TNerveTelesaTrample:
	.incbin "baserom.dol", 0x3D4D88, 0x10
.global __vt__23TNerveTelesaAttackMario
__vt__23TNerveTelesaAttackMario:
	.incbin "baserom.dol", 0x3D4D98, 0x10
.global __vt__18TNerveTelesaFreeze
__vt__18TNerveTelesaFreeze:
	.incbin "baserom.dol", 0x3D4DA8, 0x10
.global __vt__15TNerveTelesaDie
__vt__15TNerveTelesaDie:
	.incbin "baserom.dol", 0x3D4DB8, 0x10
.global __vt__19TNerveTelesaImitate
__vt__19TNerveTelesaImitate:
	.incbin "baserom.dol", 0x3D4DC8, 0x10
.global __vt__18TMarioModokiTelesa
__vt__18TMarioModokiTelesa:
	.incbin "baserom.dol", 0x3D4DD8, 0x1B8
.global __vt__10TBoxTelesa
__vt__10TBoxTelesa:
	.incbin "baserom.dol", 0x3D4F90, 0x1B8
.global __vt__11TLoopTelesa
__vt__11TLoopTelesa:
	.incbin "baserom.dol", 0x3D5148, 0x1B8
.global __vt__10TSeeTelesa
__vt__10TSeeTelesa:
	.incbin "baserom.dol", 0x3D5300, 0x1B8
.global __vt__7TTelesa
__vt__7TTelesa:
	.incbin "baserom.dol", 0x3D54B8, 0x1B8
.global __vt__14TTelesaManager
__vt__14TTelesaManager:
	.incbin "baserom.dol", 0x3D5670, 0x58
.global fireWanwan_bastable
fireWanwan_bastable:
	.incbin "baserom.dol", 0x3D56C8, 0x20
.global entry$3027
entry$3027:
	.incbin "baserom.dol", 0x3D56E8, 0x18
.global __vt__22TNerveFireWanwanEscape
__vt__22TNerveFireWanwanEscape:
	.incbin "baserom.dol", 0x3D5700, 0x10
.global __vt__22TNerveFireWanwanFreeze
__vt__22TNerveFireWanwanFreeze:
	.incbin "baserom.dol", 0x3D5710, 0x10
.global __vt__19TNerveFireWanwanFly
__vt__19TNerveFireWanwanFly:
	.incbin "baserom.dol", 0x3D5720, 0x10
.global __vt__24TNerveFireWanwanHungTail
__vt__24TNerveFireWanwanHungTail:
	.incbin "baserom.dol", 0x3D5730, 0x10
.global __vt__19TNerveFireWanwanDie
__vt__19TNerveFireWanwanDie:
	.incbin "baserom.dol", 0x3D5740, 0x10
.global __vt__23TNerveFireWanwanRecover
__vt__23TNerveFireWanwanRecover:
	.incbin "baserom.dol", 0x3D5750, 0x10
.global __vt__28TNerveFireWanwanRecoverGraph
__vt__28TNerveFireWanwanRecoverGraph:
	.incbin "baserom.dol", 0x3D5760, 0x10
.global __vt__21TNerveFireWanwanTired
__vt__21TNerveFireWanwanTired:
	.incbin "baserom.dol", 0x3D5770, 0x10
.global __vt__22TNerveFireWanwanAttack
__vt__22TNerveFireWanwanAttack:
	.incbin "baserom.dol", 0x3D5780, 0x10
.global __vt__25TNerveFireWanwanFindMario
__vt__25TNerveFireWanwanFindMario:
	.incbin "baserom.dol", 0x3D5790, 0x10
.global __vt__20TNerveFireWanwanTurn
__vt__20TNerveFireWanwanTurn:
	.incbin "baserom.dol", 0x3D57A0, 0x10
.global __vt__27TNerveFireWanwanGraphWander
__vt__27TNerveFireWanwanGraphWander:
	.incbin "baserom.dol", 0x3D57B0, 0x10
.global __vt__11TFireWanwan
__vt__11TFireWanwan:
	.incbin "baserom.dol", 0x3D57C0, 0x1A8
.global __vt__18TFireWanwanTailHit
__vt__18TFireWanwanTailHit:
	.incbin "baserom.dol", 0x3D5968, 0xB4
.global __vt__18TFireWanwanManager
__vt__18TFireWanwanManager:
	.incbin "baserom.dol", 0x3D5A1C, 0x5C
.global __vt__15TStageEnemyInfo
__vt__15TStageEnemyInfo:
	.incbin "baserom.dol", 0x3D5A78, 0x20
.global __vt__20TStageEnemyInfoTable
__vt__20TStageEnemyInfoTable:
	.incbin "baserom.dol", 0x3D5A98, 0x20
.global __vt__53TNameRefPtrAryT$$015TStageEnemyInfo$$4Q26JDrama8TNameRef$$1
__vt__53TNameRefPtrAryT$$015TStageEnemyInfo$$4Q26JDrama8TNameRef$$1:
	.incbin "baserom.dol", 0x3D5AB8, 0x20
.global __vt__17TOneShotGenerator
__vt__17TOneShotGenerator:
	.incbin "baserom.dol", 0x3D5AD8, 0xA4
.global __vt__10TGenerator
__vt__10TGenerator:
	.incbin "baserom.dol", 0x3D5B7C, 0x24
.global bosspakkun_bastable
bosspakkun_bastable:
	.incbin "baserom.dol", 0x3D5BA0, 0x78
.global __vt__15TNerveBPCannonL
__vt__15TNerveBPCannonL:
	.incbin "baserom.dol", 0x3D5C18, 0x10
.global __vt__13TNerveBPWaitL
__vt__13TNerveBPWaitL:
	.incbin "baserom.dol", 0x3D5C28, 0x10
.global __vt__18TNerveBPBreakSleep
__vt__18TNerveBPBreakSleep:
	.incbin "baserom.dol", 0x3D5C38, 0x10
.global __vt__13TNerveBPSleep
__vt__13TNerveBPSleep:
	.incbin "baserom.dol", 0x3D5C48, 0x10
.global __vt__12TNerveBPFall
__vt__12TNerveBPFall:
	.incbin "baserom.dol", 0x3D5C58, 0x10
.global __vt__13TNerveBPHover
__vt__13TNerveBPHover:
	.incbin "baserom.dol", 0x3D5C68, 0x10
.global __vt__16TNerveBPFlyPivot
__vt__16TNerveBPFlyPivot:
	.incbin "baserom.dol", 0x3D5C78, 0x10
.global __vt__17TNerveBPFlyCannon
__vt__17TNerveBPFlyCannon:
	.incbin "baserom.dol", 0x3D5C88, 0x10
.global __vt__17TNerveBPTouchDown
__vt__17TNerveBPTouchDown:
	.incbin "baserom.dol", 0x3D5C98, 0x10
.global __vt__11TNerveBPFly
__vt__11TNerveBPFly:
	.incbin "baserom.dol", 0x3D5CA8, 0x10
.global __vt__15TNerveBPTakeOff
__vt__15TNerveBPTakeOff:
	.incbin "baserom.dol", 0x3D5CB8, 0x10
.global __vt__11TNerveBPDie
__vt__11TNerveBPDie:
	.incbin "baserom.dol", 0x3D5CC8, 0x10
.global __vt__14TNerveBPPreDie
__vt__14TNerveBPPreDie:
	.incbin "baserom.dol", 0x3D5CD8, 0x10
.global __vt__17TNerveBPJumpReact
__vt__17TNerveBPJumpReact:
	.incbin "baserom.dol", 0x3D5CE8, 0x10
.global __vt__18TNerveBPStompReact
__vt__18TNerveBPStompReact:
	.incbin "baserom.dol", 0x3D5CF8, 0x10
.global __vt__13TNerveBPSwing
__vt__13TNerveBPSwing:
	.incbin "baserom.dol", 0x3D5D08, 0x10
.global __vt__13TNerveBPGetUp
__vt__13TNerveBPGetUp:
	.incbin "baserom.dol", 0x3D5D18, 0x10
.global __vt__17TNerveBPTumbleOut
__vt__17TNerveBPTumbleOut:
	.incbin "baserom.dol", 0x3D5D28, 0x10
.global __vt__14TNerveBPTumble
__vt__14TNerveBPTumble:
	.incbin "baserom.dol", 0x3D5D38, 0x10
.global __vt__16TNerveBPTumbleIn
__vt__16TNerveBPTumbleIn:
	.incbin "baserom.dol", 0x3D5D48, 0x10
.global __vt__15TNerveBPSwallow
__vt__15TNerveBPSwallow:
	.incbin "baserom.dol", 0x3D5D58, 0x10
.global __vt__13TNerveBPPivot
__vt__13TNerveBPPivot:
	.incbin "baserom.dol", 0x3D5D68, 0x10
.global __vt__15TNerveBPTornado
__vt__15TNerveBPTornado:
	.incbin "baserom.dol", 0x3D5D78, 0x10
.global __vt__13TNerveBPVomit
__vt__13TNerveBPVomit:
	.incbin "baserom.dol", 0x3D5D88, 0x10
.global __vt__14TNerveBPCannon
__vt__14TNerveBPCannon:
	.incbin "baserom.dol", 0x3D5D98, 0x10
.global __vt__12TNerveBPWait
__vt__12TNerveBPWait:
	.incbin "baserom.dol", 0x3D5DA8, 0x10
.global __vt__18TBossPakkunManager
__vt__18TBossPakkunManager:
	.incbin "baserom.dol", 0x3D5DB8, 0x54
.global __vt__11TBossPakkun
__vt__11TBossPakkun:
	.incbin "baserom.dol", 0x3D5E0C, 0x114
.global __vt__18TBossPakkunMtxCalc
__vt__18TBossPakkunMtxCalc:
	.incbin "baserom.dol", 0x3D5F20, 0x44
.global __vt__8TBPNavel
__vt__8TBPNavel:
	.incbin "baserom.dol", 0x3D5F64, 0xA4
.global __vt__10TBPHeadHit
__vt__10TBPHeadHit:
	.incbin "baserom.dol", 0x3D6008, 0xA4
.global __vt__10TBPTornado
__vt__10TBPTornado:
	.incbin "baserom.dol", 0x3D60AC, 0xA4
.global __vt__8TBPVomit
__vt__8TBPVomit:
	.incbin "baserom.dol", 0x3D6150, 0x24
.global __vt__10TBPPolDrop
__vt__10TBPPolDrop:
	.incbin "baserom.dol", 0x3D6174, 0xA4
.global pukupuku_bastable
pukupuku_bastable:
	.incbin "baserom.dol", 0x3D6218, 0x2C
.global moepuku_bastable
moepuku_bastable:
	.incbin "baserom.dol", 0x3D6244, 0x2C
.global __vt__24TNerveTobiPukuSwimWander
__vt__24TNerveTobiPukuSwimWander:
	.incbin "baserom.dol", 0x3D6270, 0x10
.global __vt__26TNerveTobiPukuReturnLaunch
__vt__26TNerveTobiPukuReturnLaunch:
	.incbin "baserom.dol", 0x3D6280, 0x10
.global __vt__24TNerveTobiPukuPrepareFly
__vt__24TNerveTobiPukuPrepareFly:
	.incbin "baserom.dol", 0x3D6290, 0x10
.global __vt__19TNerveTobiPukuBound
__vt__19TNerveTobiPukuBound:
	.incbin "baserom.dol", 0x3D62A0, 0x10
.global __vt__18TNerveTobiPukuLand
__vt__18TNerveTobiPukuLand:
	.incbin "baserom.dol", 0x3D62B0, 0x10
.global __vt__17TNerveTobiPukuDie
__vt__17TNerveTobiPukuDie:
	.incbin "baserom.dol", 0x3D62C0, 0x10
.global __vt__22TNerveTobiPukuPitiPiti
__vt__22TNerveTobiPukuPitiPiti:
	.incbin "baserom.dol", 0x3D62D0, 0x10
.global __vt__18TNerveTobiPukuFall
__vt__18TNerveTobiPukuFall:
	.incbin "baserom.dol", 0x3D62E0, 0x10
.global __vt__22TNerveTobiPukuHitWater
__vt__22TNerveTobiPukuHitWater:
	.incbin "baserom.dol", 0x3D62F0, 0x10
.global __vt__20TNerveTobiPukuAttack
__vt__20TNerveTobiPukuAttack:
	.incbin "baserom.dol", 0x3D6300, 0x10
.global __vt__17TNerveTobiPukuFly
__vt__17TNerveTobiPukuFly:
	.incbin "baserom.dol", 0x3D6310, 0x10
.global __vt__22TNerveTobiPukuGenerate
__vt__22TNerveTobiPukuGenerate:
	.incbin "baserom.dol", 0x3D6320, 0x10
.global __vt__8TMoePuku
__vt__8TMoePuku:
	.incbin "baserom.dol", 0x3D6330, 0x200
.global __vt__9TPukuPuku
__vt__9TPukuPuku:
	.incbin "baserom.dol", 0x3D6530, 0x200
.global __vt__9TTobiPuku
__vt__9TTobiPuku:
	.incbin "baserom.dol", 0x3D6730, 0x200
.global __vt__17TMoePukuLaunchPad
__vt__17TMoePukuLaunchPad:
	.incbin "baserom.dol", 0x3D6930, 0x1AC
.global __vt__18TTobiPukuLaunchPad
__vt__18TTobiPukuLaunchPad:
	.incbin "baserom.dol", 0x3D6ADC, 0x1AC
.global __vt__15TMoePukuManager
__vt__15TMoePukuManager:
	.incbin "baserom.dol", 0x3D6C88, 0x58
.global __vt__16TTobiPukuManager
__vt__16TTobiPukuManager:
	.incbin "baserom.dol", 0x3D6CE0, 0x58
.global __vt__24TMoePukuLaunchPadManager
__vt__24TMoePukuLaunchPadManager:
	.incbin "baserom.dol", 0x3D6D38, 0x58
.global __vt__25TTobiPukuLaunchPadManager
__vt__25TTobiPukuLaunchPadManager:
	.incbin "baserom.dol", 0x3D6D90, 0x58
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3D6DE8, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3D6DF4, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3D6E00, 0x10
.global MtxCalcTypeName
MtxCalcTypeName:
	.incbin "baserom.dol", 0x3D6E10, 0x10
.global tinkoopa_bastable
tinkoopa_bastable:
	.incbin "baserom.dol", 0x3D6E20, 0x44
.global TTinKoopa_jointNameTable
TTinKoopa_jointNameTable:
	.incbin "baserom.dol", 0x3D6E64, 0x3C
.global table$3029
table$3029:
	.incbin "baserom.dol", 0x3D6EA0, 0x18
.global table$3036
table$3036:
	.incbin "baserom.dol", 0x3D6EB8, 0x18
.global table$3041
table$3041:
	.incbin "baserom.dol", 0x3D6ED0, 0x18
.global table$3042
table$3042:
	.incbin "baserom.dol", 0x3D6EE8, 0x18
.global table$3043
table$3043:
	.incbin "baserom.dol", 0x3D6F00, 0x14
.global table$3044
table$3044:
	.incbin "baserom.dol", 0x3D6F14, 0x14
.global table$3045
table$3045:
	.incbin "baserom.dol", 0x3D6F28, 0x14
.global table$3046
table$3046:
	.incbin "baserom.dol", 0x3D6F3C, 0x14
.global table$3047
table$3047:
	.incbin "baserom.dol", 0x3D6F50, 0x14
.global breastTrackJointNameTable
breastTrackJointNameTable:
	.incbin "baserom.dol", 0x3D6F64, 0x18
.global bellyTrackJointNameTable
bellyTrackJointNameTable:
	.incbin "baserom.dol", 0x3D6F7C, 0x18
.global rightArmTrackJointNameTable
rightArmTrackJointNameTable:
	.incbin "baserom.dol", 0x3D6F94, 0x10
.global leftArmTrackJointNameTable
leftArmTrackJointNameTable:
	.incbin "baserom.dol", 0x3D6FA4, 0x10
.global onetimeFilenames$3493
onetimeFilenames$3493:
	.incbin "baserom.dol", 0x3D6FB4, 0x1C
.global loopFilenames$3507
loopFilenames$3507:
	.incbin "baserom.dol", 0x3D6FD0, 0x44
.global __vt__19TNerveTinKoopaBreak
__vt__19TNerveTinKoopaBreak:
	.incbin "baserom.dol", 0x3D7014, 0x70
.global __vt__20TNerveTinKoopaDamage
__vt__20TNerveTinKoopaDamage:
	.incbin "baserom.dol", 0x3D7084, 0x10
.global __vt__18TNerveTinKoopaWait
__vt__18TNerveTinKoopaWait:
	.incbin "baserom.dol", 0x3D7094, 0x10
.global __vt__16TTinKoopaManager
__vt__16TTinKoopaManager:
	.incbin "baserom.dol", 0x3D70A4, 0x54
.global __vt__16TTinKoopaMtxCalc
__vt__16TTinKoopaMtxCalc:
	.incbin "baserom.dol", 0x3D70F8, 0x44
.global __vt__18TTinKoopaPartsBase
__vt__18TTinKoopaPartsBase:
	.incbin "baserom.dol", 0x3D713C, 0x100
.global __vt__14TTinKoopaFlame
__vt__14TTinKoopaFlame:
	.incbin "baserom.dol", 0x3D723C, 0xA4
.global __vt__9TTinKoopa
__vt__9TTinKoopa:
	.incbin "baserom.dol", 0x3D72E0, 0x118
.global clauncher_bastable
clauncher_bastable:
	.incbin "baserom.dol", 0x3D73F8, 0x10
.global __vt__22TCommonLauncherManager
__vt__22TCommonLauncherManager:
	.incbin "baserom.dol", 0x3D7408, 0x54
.global __vt__31TNerveWaitForever$$010TLiveActor$$1
__vt__31TNerveWaitForever$$010TLiveActor$$1:
	.incbin "baserom.dol", 0x3D745C, 0x10
.global __vt__15TCommonLauncher
__vt__15TCommonLauncher:
	.incbin "baserom.dol", 0x3D746C, 0x12C
.global __vt__16TLauncherManager
__vt__16TLauncherManager:
	.incbin "baserom.dol", 0x3D7598, 0x54
.global __vt__9TLauncher
__vt__9TLauncher:
	.incbin "baserom.dol", 0x3D75EC, 0x12C
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3D7718, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3D7724, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3D7730, 0x10
.global MtxCalcTypeName
MtxCalcTypeName:
	.incbin "baserom.dol", 0x3D7740, 0x10
.global bwanwan_bastable
bwanwan_bastable:
	.incbin "baserom.dol", 0x3D7750, 0x1C
.global __vt__12TNerveBWFall
__vt__12TNerveBWFall:
	.incbin "baserom.dol", 0x3D776C, 0x160
.global __vt__13TNerveBWShake
__vt__13TNerveBWShake:
	.incbin "baserom.dol", 0x3D78CC, 0x10
.global __vt__16TNerveBWJumpAway
__vt__16TNerveBWJumpAway:
	.incbin "baserom.dol", 0x3D78DC, 0x10
.global __vt__11TNerveBWDie
__vt__11TNerveBWDie:
	.incbin "baserom.dol", 0x3D78EC, 0x10
.global __vt__18TNerveBWJumpToBath
__vt__18TNerveBWJumpToBath:
	.incbin "baserom.dol", 0x3D78FC, 0x10
.global __vt__14TNerveBWWakeup
__vt__14TNerveBWWakeup:
	.incbin "baserom.dol", 0x3D790C, 0x10
.global __vt__12TNerveBWStun
__vt__12TNerveBWStun:
	.incbin "baserom.dol", 0x3D791C, 0x10
.global __vt__12TNerveBWJump
__vt__12TNerveBWJump:
	.incbin "baserom.dol", 0x3D792C, 0x10
.global __vt__12TNerveBWBark
__vt__12TNerveBWBark:
	.incbin "baserom.dol", 0x3D793C, 0x10
.global __vt__12TNerveBWRoll
__vt__12TNerveBWRoll:
	.incbin "baserom.dol", 0x3D794C, 0x10
.global __vt__19TNerveBWGraphWander
__vt__19TNerveBWGraphWander:
	.incbin "baserom.dol", 0x3D795C, 0x10
.global __vt__18TBossWanwanManager
__vt__18TBossWanwanManager:
	.incbin "baserom.dol", 0x3D796C, 0x54
.global __vt__11TBossWanwan
__vt__11TBossWanwan:
	.incbin "baserom.dol", 0x3D79C0, 0x114
.global __vt__18TBossWanwanMtxCalc
__vt__18TBossWanwanMtxCalc:
	.incbin "baserom.dol", 0x3D7AD4, 0x44
.global __vt__9TBWBinder
__vt__9TBWBinder:
	.incbin "baserom.dol", 0x3D7B18, 0x10
.global __vt__6TBWHit
__vt__6TBWHit:
	.incbin "baserom.dol", 0x3D7B28, 0xA4
.global __vt__9TBWPicket
__vt__9TBWPicket:
	.incbin "baserom.dol", 0x3D7BCC, 0xB4
.global __vt__8TBWLeash
__vt__8TBWLeash:
	.incbin "baserom.dol", 0x3D7C80, 0x24
.global __vt__12TBWLeashNode
__vt__12TBWLeashNode:
	.incbin "baserom.dol", 0x3D7CA4, 0xA4
.global tyuhana_bastable
tyuhana_bastable:
	.incbin "baserom.dol", 0x3D7D48, 0x34
.global graphlist$2835
graphlist$2835:
	.incbin "baserom.dol", 0x3D7D7C, 0x18
.global __vt__18TNerveChuuHanaWait
__vt__18TNerveChuuHanaWait:
	.incbin "baserom.dol", 0x3D7D94, 0x10
.global __vt__25TNerveChuuHanaJumpPrepare
__vt__25TNerveChuuHanaJumpPrepare:
	.incbin "baserom.dol", 0x3D7DA4, 0x10
.global __vt__20TNerveChuuHanaAttack
__vt__20TNerveChuuHanaAttack:
	.incbin "baserom.dol", 0x3D7DB4, 0x10
.global __vt__20TNerveChuuHanaObject
__vt__20TNerveChuuHanaObject:
	.incbin "baserom.dol", 0x3D7DC4, 0x10
.global __vt__19TNerveChuuHanaFall2
__vt__19TNerveChuuHanaFall2:
	.incbin "baserom.dol", 0x3D7DD4, 0x10
.global __vt__18TNerveChuuHanaFall
__vt__18TNerveChuuHanaFall:
	.incbin "baserom.dol", 0x3D7DE4, 0x10
.global __vt__18TNerveChuuHanaRoll
__vt__18TNerveChuuHanaRoll:
	.incbin "baserom.dol", 0x3D7DF4, 0x10
.global __vt__19TNerveChuuHanaStick
__vt__19TNerveChuuHanaStick:
	.incbin "baserom.dol", 0x3D7E04, 0x10
.global __vt__25TNerveChuuHanaKeepBalance
__vt__25TNerveChuuHanaKeepBalance:
	.incbin "baserom.dol", 0x3D7E14, 0x10
.global __vt__25TNerveChuuHanaForceJumped
__vt__25TNerveChuuHanaForceJumped:
	.incbin "baserom.dol", 0x3D7E24, 0x10
.global __vt__25TNerveChuuHanaWalkOnPanel
__vt__25TNerveChuuHanaWalkOnPanel:
	.incbin "baserom.dol", 0x3D7E34, 0x10
.global __vt__9TChuuHana
__vt__9TChuuHana:
	.incbin "baserom.dol", 0x3D7E44, 0x1B8
.global __vt__23TChuuHanaAseParCallback
__vt__23TChuuHanaAseParCallback:
	.incbin "baserom.dol", 0x3D7FFC, 0x10
.global __vt__16TChuuHanaManager
__vt__16TChuuHanaManager:
	.incbin "baserom.dol", 0x3D800C, 0x5C
.global igaiga_bastable
igaiga_bastable:
	.incbin "baserom.dol", 0x3D8068, 0x20
.global gorogoro_bastable
gorogoro_bastable:
	.incbin "baserom.dol", 0x3D8088, 0x10
.global entry$3110
entry$3110:
	.incbin "baserom.dol", 0x3D8098, 0x18
.global entry$3337
entry$3337:
	.incbin "baserom.dol", 0x3D80B0, 0x18
.global __vt__17TNerveGorogoroDie
__vt__17TNerveGorogoroDie:
	.incbin "baserom.dol", 0x3D80C8, 0x10
.global __vt__25TNerveGorogoroRollOnGraph
__vt__25TNerveGorogoroRollOnGraph:
	.incbin "baserom.dol", 0x3D80D8, 0x10
.global __vt__9TGorogoro
__vt__9TGorogoro:
	.incbin "baserom.dol", 0x3D80E8, 0x1D0
.global __vt__16TGorogoroManager
__vt__16TGorogoroManager:
	.incbin "baserom.dol", 0x3D82B8, 0x58
.global __vt__21TGorogoroPolluteModel
__vt__21TGorogoroPolluteModel:
	.incbin "baserom.dol", 0x3D8310, 0x28
.global __vt__28TGorogoroPolluteModelManager
__vt__28TGorogoroPolluteModelManager:
	.incbin "baserom.dol", 0x3D8338, 0x28
.global __vt__27TNerveIgaigaShootFromCannon
__vt__27TNerveIgaigaShootFromCannon:
	.incbin "baserom.dol", 0x3D8360, 0x10
.global __vt__20TNerveIgaigaWaterHit
__vt__20TNerveIgaigaWaterHit:
	.incbin "baserom.dol", 0x3D8370, 0x10
.global __vt__23TNerveIgaigaRollOnGraph
__vt__23TNerveIgaigaRollOnGraph:
	.incbin "baserom.dol", 0x3D8380, 0x10
.global __vt__7TIgaiga
__vt__7TIgaiga:
	.incbin "baserom.dol", 0x3D8390, 0x1D0
.global __vt__14TIgaigaManager
__vt__14TIgaigaManager:
	.incbin "baserom.dol", 0x3D8560, 0x58
.global __vt__19TIgaigaPolluteModel
__vt__19TIgaigaPolluteModel:
	.incbin "baserom.dol", 0x3D85B8, 0x28
.global __vt__26TIgaigaPolluteModelManager
__vt__26TIgaigaPolluteModelManager:
	.incbin "baserom.dol", 0x3D85E0, 0x28
.global __vt__10TRollEnemy
__vt__10TRollEnemy:
	.incbin "baserom.dol", 0x3D8608, 0x1D0
.global poihana_bastable
poihana_bastable:
	.incbin "baserom.dol", 0x3D87D8, 0x38
.global __vt__20TNervePoihanaTrapped
__vt__20TNervePoihanaTrapped:
	.incbin "baserom.dol", 0x3D8810, 0x10
.global __vt__18TNervePoihanaThrow
__vt__18TNervePoihanaThrow:
	.incbin "baserom.dol", 0x3D8820, 0x10
.global __vt__19TNervePoihanaFreeze
__vt__19TNervePoihanaFreeze:
	.incbin "baserom.dol", 0x3D8830, 0x10
.global __vt__18TNervePoihanaSleep
__vt__18TNervePoihanaSleep:
	.incbin "baserom.dol", 0x3D8840, 0x10
.global __vt__13TSleepPoiHana
__vt__13TSleepPoiHana:
	.incbin "baserom.dol", 0x3D8850, 0x1B8
.global __vt__11TPoiHanaRed
__vt__11TPoiHanaRed:
	.incbin "baserom.dol", 0x3D8A08, 0x1B8
.global __vt__8TPoiHana
__vt__8TPoiHana:
	.incbin "baserom.dol", 0x3D8BC0, 0x1B8
.global __vt__17TPoiHanaCollision
__vt__17TPoiHanaCollision:
	.incbin "baserom.dol", 0x3D8D78, 0xA4
.global __vt__15TPoiHanaManager
__vt__15TPoiHanaManager:
	.incbin "baserom.dol", 0x3D8E1C, 0x5C
.global tamaNoko_bastable
tamaNoko_bastable:
	.incbin "baserom.dol", 0x3D8E78, 0x44
.global entry$3149
entry$3149:
	.incbin "baserom.dol", 0x3D8EBC, 0x18
.global __vt__18TNerveTamaNokoWait
__vt__18TNerveTamaNokoWait:
	.incbin "baserom.dol", 0x3D8ED4, 0x10
.global __vt__22TNerveTamaNokoHitWater
__vt__22TNerveTamaNokoHitWater:
	.incbin "baserom.dol", 0x3D8EE4, 0x10
.global __vt__18TNerveTamaNokoSink
__vt__18TNerveTamaNokoSink:
	.incbin "baserom.dol", 0x3D8EF4, 0x10
.global __vt__20TNerveTamaNokoThrown
__vt__20TNerveTamaNokoThrown:
	.incbin "baserom.dol", 0x3D8F04, 0x10
.global __vt__20TNerveTamaNokoPickUp
__vt__20TNerveTamaNokoPickUp:
	.incbin "baserom.dol", 0x3D8F14, 0x10
.global __vt__18TNerveTamaNokoDown
__vt__18TNerveTamaNokoDown:
	.incbin "baserom.dol", 0x3D8F24, 0x10
.global __vt__20TNerveTamaNokoAttack
__vt__20TNerveTamaNokoAttack:
	.incbin "baserom.dol", 0x3D8F34, 0x10
.global __vt__19TNerveTamaNokoSleep
__vt__19TNerveTamaNokoSleep:
	.incbin "baserom.dol", 0x3D8F44, 0x10
.global __vt__9TTamaNoko
__vt__9TTamaNoko:
	.incbin "baserom.dol", 0x3D8F54, 0x1B8
.global __vt__16TTamaNokoManager
__vt__16TTamaNokoManager:
	.incbin "baserom.dol", 0x3D910C, 0x58
.global __vt__15TTamaNokoFlower
__vt__15TTamaNokoFlower:
	.incbin "baserom.dol", 0x3D9164, 0x24
.global btelesa_bastable
btelesa_bastable:
	.incbin "baserom.dol", 0x3D9188, 0x44
.global entry$3173
entry$3173:
	.incbin "baserom.dol", 0x3D91CC, 0x18
.global entry$3276
entry$3276:
	.incbin "baserom.dol", 0x3D91E4, 0x18
.global manNameTable$3933
manNameTable$3933:
	.incbin "baserom.dol", 0x3D91FC, 0x24
.global __vt__24TNerveBossTelesaFallDemo
__vt__24TNerveBossTelesaFallDemo:
	.incbin "baserom.dol", 0x3D9220, 0x10
.global __vt__25TBossTelesaKillSmallEnemy
__vt__25TBossTelesaKillSmallEnemy:
	.incbin "baserom.dol", 0x3D9230, 0xA4
.global __vt__22TNerveBossTelesaFreeze
__vt__22TNerveBossTelesaFreeze:
	.incbin "baserom.dol", 0x3D92D4, 0x10
.global __vt__27TNerveBossTelesaPrepareSlot
__vt__27TNerveBossTelesaPrepareSlot:
	.incbin "baserom.dol", 0x3D92E4, 0x10
.global __vt__28TNerveBossTelesaSpitSlotItem
__vt__28TNerveBossTelesaSpitSlotItem:
	.incbin "baserom.dol", 0x3D92F4, 0x10
.global __vt__25TNerveBossTelesaSlotStart
__vt__25TNerveBossTelesaSlotStart:
	.incbin "baserom.dol", 0x3D9304, 0x10
.global __vt__22TNerveBossTelesaAppear
__vt__22TNerveBossTelesaAppear:
	.incbin "baserom.dol", 0x3D9314, 0x10
.global __vt__24TNerveBossTelesaHideWait
__vt__24TNerveBossTelesaHideWait:
	.incbin "baserom.dol", 0x3D9324, 0x10
.global __vt__20TNerveBossTelesaHide
__vt__20TNerveBossTelesaHide:
	.incbin "baserom.dol", 0x3D9334, 0x10
.global __vt__20TNerveBossTelesaSpit
__vt__20TNerveBossTelesaSpit:
	.incbin "baserom.dol", 0x3D9344, 0x10
.global __vt__19TNerveBossTelesaDie
__vt__19TNerveBossTelesaDie:
	.incbin "baserom.dol", 0x3D9354, 0x10
.global __vt__11TBossTelesa
__vt__11TBossTelesa:
	.incbin "baserom.dol", 0x3D9364, 0x114
.global __vt__11TTelesaSlot
__vt__11TTelesaSlot:
	.incbin "baserom.dol", 0x3D9478, 0x174
.global __vt__17TBossTelesaTongue
__vt__17TBossTelesaTongue:
	.incbin "baserom.dol", 0x3D95EC, 0xA4
.global __vt__15TBossTelesaBody
__vt__15TBossTelesaBody:
	.incbin "baserom.dol", 0x3D9690, 0xA4
.global __vt__18TBossTelesaManager
__vt__18TBossTelesaManager:
	.incbin "baserom.dol", 0x3D9734, 0x54
.global __vt__17TNerveBubbleSplit
__vt__17TNerveBubbleSplit:
	.incbin "baserom.dol", 0x3D9788, 0x10
.global __vt__16TNerveBubbleLive
__vt__16TNerveBubbleLive:
	.incbin "baserom.dol", 0x3D9798, 0x10
.global __vt__7TBubble
__vt__7TBubble:
	.incbin "baserom.dol", 0x3D97A8, 0x1B8
.global __vt__14TBubbleManager
__vt__14TBubbleManager:
	.incbin "baserom.dol", 0x3D9960, 0x58
.global __vt__19TNerveRHGraphWander
__vt__19TNerveRHGraphWander:
	.incbin "baserom.dol", 0x3D99B8, 0x10
.global __vt__17TRiccoHookManager
__vt__17TRiccoHookManager:
	.incbin "baserom.dol", 0x3D99C8, 0x54
.global __vt__10TRiccoHook
__vt__10TRiccoHook:
	.incbin "baserom.dol", 0x3D9A1C, 0x114
.global __vt__9THookTake
__vt__9THookTake:
	.incbin "baserom.dol", 0x3D9B30, 0xB8
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3D9BE8, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3D9BF4, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3D9C00, 0x10
.global MtxCalcTypeName
MtxCalcTypeName:
	.incbin "baserom.dol", 0x3D9C10, 0x10
.global bombhei_bastable
bombhei_bastable:
	.incbin "baserom.dol", 0x3D9C20, 0x1C
.global entry$2918
entry$2918:
	.incbin "baserom.dol", 0x3D9C3C, 0x24
.global __vt__22TNerveBombHeiExplosion
__vt__22TNerveBombHeiExplosion:
	.incbin "baserom.dol", 0x3D9C60, 0x50
.global __vt__19TNerveBombHeiThrown
__vt__19TNerveBombHeiThrown:
	.incbin "baserom.dol", 0x3D9CB0, 0x10
.global __vt__19TNerveBombHeiPickUp
__vt__19TNerveBombHeiPickUp:
	.incbin "baserom.dol", 0x3D9CC0, 0x10
.global __vt__26TNerveBombHeiWaitExplosion
__vt__26TNerveBombHeiWaitExplosion:
	.incbin "baserom.dol", 0x3D9CD0, 0x10
.global __vt__26TNerveBombHeiWalkExplosion
__vt__26TNerveBombHeiWalkExplosion:
	.incbin "baserom.dol", 0x3D9CE0, 0x10
.global __vt__19TNerveBombHeiAttack
__vt__19TNerveBombHeiAttack:
	.incbin "baserom.dol", 0x3D9CF0, 0x10
.global __vt__21TNerveBombHeiGenerate
__vt__21TNerveBombHeiGenerate:
	.incbin "baserom.dol", 0x3D9D00, 0x10
.global __vt__8TBombHei
__vt__8TBombHei:
	.incbin "baserom.dol", 0x3D9D10, 0x1B8
.global __vt__15TBombHeiManager
__vt__15TBombHeiManager:
	.incbin "baserom.dol", 0x3D9EC8, 0x58
.global cannon_bastable
cannon_bastable:
	.incbin "baserom.dol", 0x3D9F20, 0x50
.global sCannonDomPartsJointTable$3043
sCannonDomPartsJointTable$3043:
	.incbin "baserom.dol", 0x3D9F70, 0xC
.global __vt__18TNerveCannonObject
__vt__18TNerveCannonObject:
	.incbin "baserom.dol", 0x3D9F7C, 0x10
.global __vt__22TNerveCannonDamageDemo
__vt__22TNerveCannonDamageDemo:
	.incbin "baserom.dol", 0x3D9F8C, 0x10
.global __vt__18TNerveCannonDamage
__vt__18TNerveCannonDamage:
	.incbin "baserom.dol", 0x3D9F9C, 0x10
.global __vt__17TNerveCannonClose
__vt__17TNerveCannonClose:
	.incbin "baserom.dol", 0x3D9FAC, 0x10
.global __vt__26TNerveCannonForceBombShoot
__vt__26TNerveCannonForceBombShoot:
	.incbin "baserom.dol", 0x3D9FBC, 0x10
.global __vt__17TNerveCannonShoot
__vt__17TNerveCannonShoot:
	.incbin "baserom.dol", 0x3D9FCC, 0x10
.global __vt__18TNerveCannonSearch
__vt__18TNerveCannonSearch:
	.incbin "baserom.dol", 0x3D9FDC, 0x10
.global __vt__16TNerveCannonOpen
__vt__16TNerveCannonOpen:
	.incbin "baserom.dol", 0x3D9FEC, 0x10
.global __vt__7TCannon
__vt__7TCannon:
	.incbin "baserom.dol", 0x3D9FFC, 0x1AC
.global __vt__10TCannonDom
__vt__10TCannonDom:
	.incbin "baserom.dol", 0x3DA1A8, 0x24
.global __vt__9TChorobei
__vt__9TChorobei:
	.incbin "baserom.dol", 0x3DA1CC, 0xA4
.global __vt__14TCannonManager
__vt__14TCannonManager:
	.incbin "baserom.dol", 0x3DA270, 0x58
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3DA2C8, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3DA2D4, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3DA2E0, 0x10
.global MtxCalcTypeName
MtxCalcTypeName:
	.incbin "baserom.dol", 0x3DA2F0, 0x10
.global bosseel_bastable
bosseel_bastable:
	.incbin "baserom.dol", 0x3DA300, 0x5C
.global bossEelTears_bastable
bossEelTears_bastable:
	.incbin "baserom.dol", 0x3DA35C, 0x10
.global entry$3126
entry$3126:
	.incbin "baserom.dol", 0x3DA36C, 0x24
.global sEyePartsJointTable$3535
sEyePartsJointTable$3535:
	.incbin "baserom.dol", 0x3DA390, 0x10
.global sToothPartsJointTable$3546
sToothPartsJointTable$3546:
	.incbin "baserom.dol", 0x3DA3A0, 0x20
.global sCollisionJointTable$3578
sCollisionJointTable$3578:
	.incbin "baserom.dol", 0x3DA3C0, 0x10
.global sCollisionFileTable$3582
sCollisionFileTable$3582:
	.incbin "baserom.dol", 0x3DA3D0, 0x10
.global __vt__26TNerveBossEelSleepOnBottom
__vt__26TNerveBossEelSleepOnBottom:
	.incbin "baserom.dol", 0x3DA3E0, 0x74
.global __vt__26TNerveBossEelMouthOpenWait
__vt__26TNerveBossEelMouthOpenWait:
	.incbin "baserom.dol", 0x3DA454, 0x10
.global __vt__16TNerveBossEelDie
__vt__16TNerveBossEelDie:
	.incbin "baserom.dol", 0x3DA464, 0x10
.global __vt__16TNerveBossEelEat
__vt__16TNerveBossEelEat:
	.incbin "baserom.dol", 0x3DA474, 0x10
.global __vt__22TNerveBossEelQuickBack
__vt__22TNerveBossEelQuickBack:
	.incbin "baserom.dol", 0x3DA484, 0x10
.global __vt__21TNerveBossEelSlowBack
__vt__21TNerveBossEelSlowBack:
	.incbin "baserom.dol", 0x3DA494, 0x10
.global __vt__20TNerveBossEelOutWait
__vt__20TNerveBossEelOutWait:
	.incbin "baserom.dol", 0x3DA4A4, 0x10
.global __vt__19TNerveBossEelAppear
__vt__19TNerveBossEelAppear:
	.incbin "baserom.dol", 0x3DA4B4, 0x10
.global __vt__23TNerveBossEelSecondSpin
__vt__23TNerveBossEelSecondSpin:
	.incbin "baserom.dol", 0x3DA4C4, 0x10
.global __vt__22TNerveBossEelFirstSpin
__vt__22TNerveBossEelFirstSpin:
	.incbin "baserom.dol", 0x3DA4D4, 0x10
.global __vt__23TNerveBossEelWaitAppear
__vt__23TNerveBossEelWaitAppear:
	.incbin "baserom.dol", 0x3DA4E4, 0x10
.global __vt__8TBossEel
__vt__8TBossEel:
	.incbin "baserom.dol", 0x3DA4F4, 0x114
.global __vt__29TBossEelTearsRecoverCollision
__vt__29TBossEelTearsRecoverCollision:
	.incbin "baserom.dol", 0x3DA608, 0xAC
.global __vt__24TBossEelBarrierCollision
__vt__24TBossEelBarrierCollision:
	.incbin "baserom.dol", 0x3DA6B4, 0xAC
.global __vt__20TBossEelAwaCollision
__vt__20TBossEelAwaCollision:
	.incbin "baserom.dol", 0x3DA760, 0xAC
.global __vt__21TBossEelBodyCollision
__vt__21TBossEelBodyCollision:
	.incbin "baserom.dol", 0x3DA80C, 0xAC
.global __vt__17TBossEelCollision
__vt__17TBossEelCollision:
	.incbin "baserom.dol", 0x3DA8B8, 0xAC
.global __vt__17TBossEelHeartCoin
__vt__17TBossEelHeartCoin:
	.incbin "baserom.dol", 0x3DA964, 0x24
.global __vt__11TBossEelEye
__vt__11TBossEelEye:
	.incbin "baserom.dol", 0x3DA988, 0x24
.global __vt__14TBossEelVortex
__vt__14TBossEelVortex:
	.incbin "baserom.dol", 0x3DA9AC, 0xA8
.global __vt__13TBossEelTooth
__vt__13TBossEelTooth:
	.incbin "baserom.dol", 0x3DAA54, 0xA4
.global __vt__15TBossEelManager
__vt__15TBossEelManager:
	.incbin "baserom.dol", 0x3DAAF8, 0x54
.global __vt__17TNerveOilBallStay
__vt__17TNerveOilBallStay:
	.incbin "baserom.dol", 0x3DAB4C, 0x10
.global __vt__8TOilBall
__vt__8TOilBall:
	.incbin "baserom.dol", 0x3DAB5C, 0x118
.global __vt__20TNerveBEelTearsSplit
__vt__20TNerveBEelTearsSplit:
	.incbin "baserom.dol", 0x3DAC74, 0x10
.global __vt__27TNerveBEelTearsMarioRecover
__vt__27TNerveBEelTearsMarioRecover:
	.incbin "baserom.dol", 0x3DAC84, 0x10
.global __vt__23TNerveBEelTearsWaterHit
__vt__23TNerveBEelTearsWaterHit:
	.incbin "baserom.dol", 0x3DAC94, 0x10
.global __vt__21TNerveBEelTearsMoveUp
__vt__21TNerveBEelTearsMoveUp:
	.incbin "baserom.dol", 0x3DACA4, 0x10
.global __vt__23TNerveBEelTearsGenerate
__vt__23TNerveBEelTearsGenerate:
	.incbin "baserom.dol", 0x3DACB4, 0x10
.global __vt__10TBEelTears
__vt__10TBEelTears:
	.incbin "baserom.dol", 0x3DACC4, 0x118
.global __vt__17TBEelTearsManager
__vt__17TBEelTearsManager:
	.incbin "baserom.dol", 0x3DADDC, 0x54
.global __vt__14TBEelTearsDrop
__vt__14TBEelTearsDrop:
	.incbin "baserom.dol", 0x3DAE30, 0xA8
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3DAED8, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3DAEE4, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3DAEF0, 0x10
.global MtxCalcTypeName
MtxCalcTypeName:
	.incbin "baserom.dol", 0x3DAF00, 0x10
.global killer_bastable
killer_bastable:
	.incbin "baserom.dol", 0x3DAF10, 0x14
.global entry$3036
entry$3036:
	.incbin "baserom.dol", 0x3DAF24, 0x24
.global __vt__21TNerveKillerExplosion
__vt__21TNerveKillerExplosion:
	.incbin "baserom.dol", 0x3DAF48, 0x50
.global __vt__7TKiller
__vt__7TKiller:
	.incbin "baserom.dol", 0x3DAF98, 0x1C4
.global __vt__14TKillerManager
__vt__14TKillerManager:
	.incbin "baserom.dol", 0x3DB15C, 0x58
.global __vt__22TNerveFlyEnemyChaseFly
__vt__22TNerveFlyEnemyChaseFly:
	.incbin "baserom.dol", 0x3DB1B4, 0x10
.global __vt__23TNerveFlyEnemyNormalFly
__vt__23TNerveFlyEnemyNormalFly:
	.incbin "baserom.dol", 0x3DB1C4, 0x10
.global __vt__9TFlyEnemy
__vt__9TFlyEnemy:
	.incbin "baserom.dol", 0x3DB1D4, 0x1C4
.global __vt__12TBeamManager
__vt__12TBeamManager:
	.incbin "baserom.dol", 0x3DB398, 0x28
.global sambo_bastable
sambo_bastable:
	.incbin "baserom.dol", 0x3DB3C0, 0x2C
.global sambohead_bastable
sambohead_bastable:
	.incbin "baserom.dol", 0x3DB3EC, 0x34
.global entry$2760
entry$2760:
	.incbin "baserom.dol", 0x3DB420, 0x18
.global entry$2894
entry$2894:
	.incbin "baserom.dol", 0x3DB438, 0x24
.global jIndexTable$3043
jIndexTable$3043:
	.incbin "baserom.dol", 0x3DB45C, 0x10
.global entry$3085
entry$3085:
	.incbin "baserom.dol", 0x3DB46C, 0x18
.global __vt__22TNerveSamboHeadHitWall
__vt__22TNerveSamboHeadHitWall:
	.incbin "baserom.dol", 0x3DB484, 0x10
.global __vt__27TNerveSamboHeadRecoverWater
__vt__27TNerveSamboHeadRecoverWater:
	.incbin "baserom.dol", 0x3DB494, 0x10
.global __vt__23TNerveSamboHeadHitWater
__vt__23TNerveSamboHeadHitWater:
	.incbin "baserom.dol", 0x3DB4A4, 0x10
.global __vt__19TNerveSamboHeadHide
__vt__19TNerveSamboHeadHide:
	.incbin "baserom.dol", 0x3DB4B4, 0x10
.global __vt__21TNerveSamboHeadAttack
__vt__21TNerveSamboHeadAttack:
	.incbin "baserom.dol", 0x3DB4C4, 0x10
.global __vt__21TNerveSamboHeadAppear
__vt__21TNerveSamboHeadAppear:
	.incbin "baserom.dol", 0x3DB4D4, 0x10
.global __vt__10TSamboHead
__vt__10TSamboHead:
	.incbin "baserom.dol", 0x3DB4E4, 0x1B8
.global __vt__17TSamboHeadManager
__vt__17TSamboHeadManager:
	.incbin "baserom.dol", 0x3DB69C, 0x58
.global __vt__21TNerveHanaSamboFreeze
__vt__21TNerveHanaSamboFreeze:
	.incbin "baserom.dol", 0x3DB6F4, 0x10
.global __vt__18TNerveHanaSamboDie
__vt__18TNerveHanaSamboDie:
	.incbin "baserom.dol", 0x3DB704, 0x10
.global __vt__19TNerveHanaSamboHide
__vt__19TNerveHanaSamboHide:
	.incbin "baserom.dol", 0x3DB714, 0x10
.global __vt__21TNerveHanaSamboAttack
__vt__21TNerveHanaSamboAttack:
	.incbin "baserom.dol", 0x3DB724, 0x10
.global __vt__19TNerveHanaSamboWait
__vt__19TNerveHanaSamboWait:
	.incbin "baserom.dol", 0x3DB734, 0x10
.global __vt__21TNerveHanaSamboAppear
__vt__21TNerveHanaSamboAppear:
	.incbin "baserom.dol", 0x3DB744, 0x10
.global __vt__10THanaSambo
__vt__10THanaSambo:
	.incbin "baserom.dol", 0x3DB754, 0x1A8
.global __vt__14THanaSamboHead
__vt__14THanaSamboHead:
	.incbin "baserom.dol", 0x3DB8FC, 0xA4
.global __vt__17THanaSamboManager
__vt__17THanaSamboManager:
	.incbin "baserom.dol", 0x3DB9A0, 0x58
.global __vt__12TSamboFlower
__vt__12TSamboFlower:
	.incbin "baserom.dol", 0x3DB9F8, 0x118
.global __vt__19TSamboFlowerManager
__vt__19TSamboFlowerManager:
	.incbin "baserom.dol", 0x3DBB10, 0x58
.global __vt__10TSamboLeaf
__vt__10TSamboLeaf:
	.incbin "baserom.dol", 0x3DBB68, 0x28
.global popo_bastable
popo_bastable:
	.incbin "baserom.dol", 0x3DBB90, 0x1C
.global entry$2927
entry$2927:
	.incbin "baserom.dol", 0x3DBBAC, 0x24
.global __vt__16TNervePopoThrown
__vt__16TNervePopoThrown:
	.incbin "baserom.dol", 0x3DBBD0, 0x10
.global __vt__14TNervePopoWait
__vt__14TNervePopoWait:
	.incbin "baserom.dol", 0x3DBBE0, 0x10
.global __vt__19TNervePopoExplosion
__vt__19TNervePopoExplosion:
	.incbin "baserom.dol", 0x3DBBF0, 0x10
.global __vt__13TNervePopoFly
__vt__13TNervePopoFly:
	.incbin "baserom.dol", 0x3DBC00, 0x10
.global __vt__16TNervePopoAttack
__vt__16TNervePopoAttack:
	.incbin "baserom.dol", 0x3DBC10, 0x10
.global __vt__25TNervePopoPossessedNozzle
__vt__25TNervePopoPossessedNozzle:
	.incbin "baserom.dol", 0x3DBC20, 0x10
.global __vt__5TPopo
__vt__5TPopo:
	.incbin "baserom.dol", 0x3DBC30, 0x1B8
.global __vt__14TPopoCollision
__vt__14TPopoCollision:
	.incbin "baserom.dol", 0x3DBDE8, 0xA4
.global __vt__12TPopoManager
__vt__12TPopoManager:
	.incbin "baserom.dol", 0x3DBE8C, 0x5C
.global __vt__14TNerveSBH_Fall
__vt__14TNerveSBH_Fall:
	.incbin "baserom.dol", 0x3DBEE8, 0x10
.global __vt__23TNerveSBH_SleepContinue
__vt__23TNerveSBH_SleepContinue:
	.incbin "baserom.dol", 0x3DBEF8, 0x10
.global __vt__18TSleepBossHanachan
__vt__18TSleepBossHanachan:
	.incbin "baserom.dol", 0x3DBF08, 0x114
.global __vt__25TSleepBossHanachanManager
__vt__25TSleepBossHanachanManager:
	.incbin "baserom.dol", 0x3DC01C, 0x54
.global __vt__17TDemoBossHanachan
__vt__17TDemoBossHanachan:
	.incbin "baserom.dol", 0x3DC070, 0x114
.global __vt__24TDemoBossHanachanManager
__vt__24TDemoBossHanachanManager:
	.incbin "baserom.dol", 0x3DC184, 0x54
.global __vt__24TNerveFruitsBoatBckTrace
__vt__24TNerveFruitsBoatBckTrace:
	.incbin "baserom.dol", 0x3DC1D8, 0x10
.global __vt__27TNerveFruitsBoatGraphWander
__vt__27TNerveFruitsBoatGraphWander:
	.incbin "baserom.dol", 0x3DC1E8, 0x10
.global __vt__18TFruitsBoatManager
__vt__18TFruitsBoatManager:
	.incbin "baserom.dol", 0x3DC1F8, 0x54
.global __vt__11TFruitsBoat
__vt__11TFruitsBoat:
	.incbin "baserom.dol", 0x3DC24C, 0x114
.global __vt__14TWaterHitActor
__vt__14TWaterHitActor:
	.incbin "baserom.dol", 0x3DC360, 0xA8
.global sChangeSaveFileName$2712
sChangeSaveFileName$2712:
	.incbin "baserom.dol", 0x3DC408, 0xC
.global entry$2725
entry$2725:
	.incbin "baserom.dol", 0x3DC414, 0x30
.global __vt__13TBossHanachan
__vt__13TBossHanachan:
	.incbin "baserom.dol", 0x3DC444, 0x114
.global __vt__20TBossHanachanManager
__vt__20TBossHanachanManager:
	.incbin "baserom.dol", 0x3DC558, 0x58
.global __vt__22TNerveBossHanachanDead
__vt__22TNerveBossHanachanDead:
	.incbin "baserom.dol", 0x3DC5B0, 0x10
.global __vt__23TNerveBossHanachanSnort
__vt__23TNerveBossHanachanSnort:
	.incbin "baserom.dol", 0x3DC5C0, 0x10
.global __vt__24TNerveBossHanachanDamage
__vt__24TNerveBossHanachanDamage:
	.incbin "baserom.dol", 0x3DC5D0, 0x10
.global __vt__23TNerveBossHanachanGetUp
__vt__23TNerveBossHanachanGetUp:
	.incbin "baserom.dol", 0x3DC5E0, 0x10
.global __vt__22TNerveBossHanachanDown
__vt__22TNerveBossHanachanDown:
	.incbin "baserom.dol", 0x3DC5F0, 0x10
.global __vt__24TNerveBossHanachanTumble
__vt__24TNerveBossHanachanTumble:
	.incbin "baserom.dol", 0x3DC600, 0x10
.global __vt__29TNerveBossHanachanGraphWander
__vt__29TNerveBossHanachanGraphWander:
	.incbin "baserom.dol", 0x3DC610, 0x10
.global __vt__22TBossHanachanPartsHead
__vt__22TBossHanachanPartsHead:
	.incbin "baserom.dol", 0x3DC620, 0x100
.global $$23085
$$23085:
	.incbin "baserom.dol", 0x3DC720, 0x48
.global __vt__13TFootHitActor
__vt__13TFootHitActor:
	.incbin "baserom.dol", 0x3DC768, 0xA4
.global __vt__22TBossHanachanPartsBody
__vt__22TBossHanachanPartsBody:
	.incbin "baserom.dol", 0x3DC80C, 0x104
.global amiNoko_bastable
amiNoko_bastable:
	.incbin "baserom.dol", 0x3DC910, 0x40
.global entry$2820
entry$2820:
	.incbin "baserom.dol", 0x3DC950, 0x18
.global __vt__19TNerveAmiNokoFreeze
__vt__19TNerveAmiNokoFreeze:
	.incbin "baserom.dol", 0x3DC968, 0x10
.global __vt__16TNerveAmiNokoDie
__vt__16TNerveAmiNokoDie:
	.incbin "baserom.dol", 0x3DC978, 0x10
.global __vt__17TNerveAmiNokoTurn
__vt__17TNerveAmiNokoTurn:
	.incbin "baserom.dol", 0x3DC988, 0x10
.global __vt__24TNerveAmiNokoWalkOnFence
__vt__24TNerveAmiNokoWalkOnFence:
	.incbin "baserom.dol", 0x3DC998, 0x10
.global __vt__8TAmiNoko
__vt__8TAmiNoko:
	.incbin "baserom.dol", 0x3DC9A8, 0x1B8
.global __vt__7TAmiHit
__vt__7TAmiHit:
	.incbin "baserom.dol", 0x3DCB60, 0xA4
.global __vt__15TAmiNokoManager
__vt__15TAmiNokoManager:
	.incbin "baserom.dol", 0x3DCC04, 0x5C
.global gatekeeper_bastable
gatekeeper_bastable:
	.incbin "baserom.dol", 0x3DCC60, 0x50
.global __vt__19TNerveBGKLaunchName
__vt__19TNerveBGKLaunchName:
	.incbin "baserom.dol", 0x3DCCB0, 0x10
.global __vt__12TBGKObstacle
__vt__12TBGKObstacle:
	.incbin "baserom.dol", 0x3DCCC0, 0xA4
.global __vt__19TNerveBGKLaunchGoro
__vt__19TNerveBGKLaunchGoro:
	.incbin "baserom.dol", 0x3DCD64, 0x10
.global __vt__13TNerveBGKDive
__vt__13TNerveBGKDive:
	.incbin "baserom.dol", 0x3DCD74, 0x10
.global __vt__12TNerveBGKDie
__vt__12TNerveBGKDie:
	.incbin "baserom.dol", 0x3DCD84, 0x10
.global __vt__20TNerveBGKAwakeDamage
__vt__20TNerveBGKAwakeDamage:
	.incbin "baserom.dol", 0x3DCD94, 0x10
.global __vt__20TNerveBGKSleepDamage
__vt__20TNerveBGKSleepDamage:
	.incbin "baserom.dol", 0x3DCDA4, 0x10
.global __vt__14TNerveBGKWait2
__vt__14TNerveBGKWait2:
	.incbin "baserom.dol", 0x3DCDB4, 0x10
.global __vt__13TNerveBGKWait
__vt__13TNerveBGKWait:
	.incbin "baserom.dol", 0x3DCDC4, 0x10
.global __vt__15TNerveBGKAppear
__vt__15TNerveBGKAppear:
	.incbin "baserom.dol", 0x3DCDD4, 0x10
.global __vt__14TNerveBGKSleep
__vt__14TNerveBGKSleep:
	.incbin "baserom.dol", 0x3DCDE4, 0x10
.global __vt__17TBiancoGateKeeper
__vt__17TBiancoGateKeeper:
	.incbin "baserom.dol", 0x3DCDF4, 0x114
.global __vt__11TBGKMtxCalc
__vt__11TBGKMtxCalc:
	.incbin "baserom.dol", 0x3DCF08, 0x44
.global __vt__24TBiancoGateKeeperManager
__vt__24TBiancoGateKeeperManager:
	.incbin "baserom.dol", 0x3DCF4C, 0x58
.global __vt__15TGateKeeperBase
__vt__15TGateKeeperBase:
	.incbin "baserom.dol", 0x3DCFA4, 0x114
.global __vt__9TGKHitObj
__vt__9TGKHitObj:
	.incbin "baserom.dol", 0x3DD0B8, 0xA8
.global __vt__14TEggGenManager
__vt__14TEggGenManager:
	.incbin "baserom.dol", 0x3DD160, 0x54
.global __vt__13TEggGenerator
__vt__13TEggGenerator:
	.incbin "baserom.dol", 0x3DD1B4, 0x114
.global __vt__13TNerveSealDie
__vt__13TNerveSealDie:
	.incbin "baserom.dol", 0x3DD2C8, 0x10
.global __vt__14TNerveSealWait
__vt__14TNerveSealWait:
	.incbin "baserom.dol", 0x3DD2D8, 0x10
.global __vt__15TNerveSealSleep
__vt__15TNerveSealSleep:
	.incbin "baserom.dol", 0x3DD2E8, 0x10
.global __vt__12TSealManager
__vt__12TSealManager:
	.incbin "baserom.dol", 0x3DD2F8, 0x54
.global __vt__5TSeal
__vt__5TSeal:
	.incbin "baserom.dol", 0x3DD34C, 0x114
.global __vt__10TBGPolDrop
__vt__10TBGPolDrop:
	.incbin "baserom.dol", 0x3DD460, 0xA0
.global $$21431
$$21431:
	.incbin "baserom.dol", 0x3DD500, 0xC
.global $$21411
$$21411:
	.incbin "baserom.dol", 0x3DD50C, 0xC
.global $$21210
$$21210:
	.incbin "baserom.dol", 0x3DD518, 0x10
.global MtxCalcTypeName
MtxCalcTypeName:
	.incbin "baserom.dol", 0x3DD528, 0x10
.global tstatestr
tstatestr:
	.incbin "baserom.dol", 0x3DD538, 0x2C
.global jntidx$3428
jntidx$3428:
	.incbin "baserom.dol", 0x3DD564, 0x10
.global __vt__11TBGTentacle
__vt__11TBGTentacle:
	.incbin "baserom.dol", 0x3DD574, 0x24
.global $$24236
$$24236:
	.incbin "baserom.dol", 0x3DD598, 0x2C
.global $$24448
$$24448:
	.incbin "baserom.dol", 0x3DD5C4, 0x2C
.global $$24838
$$24838:
	.incbin "baserom.dol", 0x3DD5F0, 0x1A8
.global __vt__12TBGAttackHit
__vt__12TBGAttackHit:
	.incbin "baserom.dol", 0x3DD798, 0xA4
.global __vt__10TBGTakeHit
__vt__10TBGTakeHit:
	.incbin "baserom.dol", 0x3DD83C, 0xB4
.global __vt__18TBGTentacleMtxCalc
__vt__18TBGTentacleMtxCalc:
	.incbin "baserom.dol", 0x3DD8F0, 0x58
.global __vt__12TEffectEnemy
__vt__12TEffectEnemy:
	.incbin "baserom.dol", 0x3DD948, 0x1B8
.global __vt__19TEffectEnemyManager
__vt__19TEffectEnemyManager:
	.incbin "baserom.dol", 0x3DDB00, 0x58
.global hauntleg_bastable
hauntleg_bastable:
	.incbin "baserom.dol", 0x3DDB58, 0xC
.global entry$2798
entry$2798:
	.incbin "baserom.dol", 0x3DDB64, 0xC
.global __vt__19TNerveHauntLegHaunt
__vt__19TNerveHauntLegHaunt:
	.incbin "baserom.dol", 0x3DDB70, 0x10
.global __vt__9THauntLeg
__vt__9THauntLeg:
	.incbin "baserom.dol", 0x3DDB80, 0x1B8
.global __vt__14THauntedObject
__vt__14THauntedObject:
	.incbin "baserom.dol", 0x3DDD38, 0xA4
.global __vt__16THauntLegManager
__vt__16THauntLegManager:
	.incbin "baserom.dol", 0x3DDDDC, 0x5C
.global __vt__20TAreaCylinderManager
__vt__20TAreaCylinderManager:
	.incbin "baserom.dol", 0x3DDE38, 0x24
.global __vt__13TAreaCylinder
__vt__13TAreaCylinder:
	.incbin "baserom.dol", 0x3DDE5C, 0x24
.global __vt__20TNerveWireTrapGoWait
__vt__20TNerveWireTrapGoWait:
	.incbin "baserom.dol", 0x3DDE80, 0x10
.global $$23924
$$23924:
	.incbin "baserom.dol", 0x3DDE90, 0x30
.global __vt__18TNerveWireTrapWait
__vt__18TNerveWireTrapWait:
	.incbin "baserom.dol", 0x3DDEC0, 0x10
.global __vt__20TNerveWireTrapSearch
__vt__20TNerveWireTrapSearch:
	.incbin "baserom.dol", 0x3DDED0, 0x10
.global __vt__27TNerveWireTrapOnewayMoveEnd
__vt__27TNerveWireTrapOnewayMoveEnd:
	.incbin "baserom.dol", 0x3DDEE0, 0x10
.global __vt__24TNerveWireTrapOnewayMove
__vt__24TNerveWireTrapOnewayMove:
	.incbin "baserom.dol", 0x3DDEF0, 0x10
.global __vt__29TNerveWireTrapOnewayMoveStart
__vt__29TNerveWireTrapOnewayMoveStart:
	.incbin "baserom.dol", 0x3DDF00, 0x10
.global __vt__24TNerveWireTrapReturnMove
__vt__24TNerveWireTrapReturnMove:
	.incbin "baserom.dol", 0x3DDF10, 0x10
.global __vt__16TWireTrapManager
__vt__16TWireTrapManager:
	.incbin "baserom.dol", 0x3DDF20, 0x54
.global __vt__9TWireTrap
__vt__9TWireTrap:
	.incbin "baserom.dol", 0x3DDF74, 0x114
.global bosshanachan_bastable
bosshanachan_bastable:
	.incbin "baserom.dol", 0x3DE088, 0x98
.global __vt__22TBossHanachanPartsBase
__vt__22TBossHanachanPartsBase:
	.incbin "baserom.dol", 0x3DE120, 0x100
.global rocket_bastable
rocket_bastable:
	.incbin "baserom.dol", 0x3DE220, 0x10
.global entry$2821
entry$2821:
	.incbin "baserom.dol", 0x3DE230, 0x18
.global __vt__16TNerveRocketWait
__vt__16TNerveRocketWait:
	.incbin "baserom.dol", 0x3DE248, 0x10
.global __vt__15TNerveRocketFly
__vt__15TNerveRocketFly:
	.incbin "baserom.dol", 0x3DE258, 0x10
.global __vt__27TNerveRocketPossessedNozzle
__vt__27TNerveRocketPossessedNozzle:
	.incbin "baserom.dol", 0x3DE268, 0x10
.global __vt__7TRocket
__vt__7TRocket:
	.incbin "baserom.dol", 0x3DE278, 0x1A8
.global __vt__14TRocketManager
__vt__14TRocketManager:
	.incbin "baserom.dol", 0x3DE420, 0x58
.global Kazekun_bastable
Kazekun_bastable:
	.incbin "baserom.dol", 0x3DE478, 0x14
.global __vt__21TNerveKazekunHitWater
__vt__21TNerveKazekunHitWater:
	.incbin "baserom.dol", 0x3DE48C, 0x10
.global __vt__17TNerveKazekunWait
__vt__17TNerveKazekunWait:
	.incbin "baserom.dol", 0x3DE49C, 0x10
.global __vt__22TNerveKazekunDisappear
__vt__22TNerveKazekunDisappear:
	.incbin "baserom.dol", 0x3DE4AC, 0x10
.global __vt__19TNerveKazekunAttack
__vt__19TNerveKazekunAttack:
	.incbin "baserom.dol", 0x3DE4BC, 0x10
.global __vt__22TNerveKazekunPreAttack
__vt__22TNerveKazekunPreAttack:
	.incbin "baserom.dol", 0x3DE4CC, 0x10
.global __vt__17TNerveKazekunTurn
__vt__17TNerveKazekunTurn:
	.incbin "baserom.dol", 0x3DE4DC, 0x10
.global __vt__19TNerveKazekunAppear
__vt__19TNerveKazekunAppear:
	.incbin "baserom.dol", 0x3DE4EC, 0x10
.global __vt__19TNerveKazekunSearch
__vt__19TNerveKazekunSearch:
	.incbin "baserom.dol", 0x3DE4FC, 0x10
.global __vt__15TKazekunManager
__vt__15TKazekunManager:
	.incbin "baserom.dol", 0x3DE50C, 0x58
.global __vt__8TKazekun
__vt__8TKazekun:
	.incbin "baserom.dol", 0x3DE564, 0x1AC
.global sFrameRate__10TBossManta
sFrameRate__10TBossManta:
	.incbin "baserom.dol", 0x3DE710, 0x18
.global sScale__10TBossManta
sScale__10TBossManta:
	.incbin "baserom.dol", 0x3DE728, 0x18
.global onetimeFilenames$3261
onetimeFilenames$3261:
	.incbin "baserom.dol", 0x3DE740, 0x14
.global loopFilenames$3273
loopFilenames$3273:
	.incbin "baserom.dol", 0x3DE754, 0x28
.global __vt__17TBossMantaManager
__vt__17TBossMantaManager:
	.incbin "baserom.dol", 0x3DE77C, 0x54
.global __vt__29TBossMantaAdditionalCollision
__vt__29TBossMantaAdditionalCollision:
	.incbin "baserom.dol", 0x3DE7D0, 0xA4
.global __vt__10TBossManta
__vt__10TBossManta:
	.incbin "baserom.dol", 0x3DE874, 0x114
.global __vt__21TNerveMantaAppearDemo
__vt__21TNerveMantaAppearDemo:
	.incbin "baserom.dol", 0x3DE988, 0x10
.global __vt__16TNerveMantaDeath
__vt__16TNerveMantaDeath:
	.incbin "baserom.dol", 0x3DE998, 0x10
.global __vt__16TNerveMantaSpawn
__vt__16TNerveMantaSpawn:
	.incbin "baserom.dol", 0x3DE9A8, 0x10
.global __vt__19TNerveMantaHitWater
__vt__19TNerveMantaHitWater:
	.incbin "baserom.dol", 0x3DE9B8, 0x10
.global __vt__15TNerveMantaMove
__vt__15TNerveMantaMove:
	.incbin "baserom.dol", 0x3DE9C8, 0x10
.global __vt__11TWireBinder
__vt__11TWireBinder:
	.incbin "baserom.dol", 0x3DE9D8, 0x10
.global sambohead_bastable
sambohead_bastable:
	.incbin "baserom.dol", 0x3DE9E8, 0x34
.global __vt__17TNerveYumboFreeze
__vt__17TNerveYumboFreeze:
	.incbin "baserom.dol", 0x3DEA1C, 0x10
.global __vt__17TNerveYumboAttack
__vt__17TNerveYumboAttack:
	.incbin "baserom.dol", 0x3DEA2C, 0x10
.global __vt__20TNerveYumboAppearing
__vt__20TNerveYumboAppearing:
	.incbin "baserom.dol", 0x3DEA3C, 0x10
.global __vt__17TNerveYumboHiding
__vt__17TNerveYumboHiding:
	.incbin "baserom.dol", 0x3DEA4C, 0x10
.global __vt__18TNerveYumboDancing
__vt__18TNerveYumboDancing:
	.incbin "baserom.dol", 0x3DEA5C, 0x10
.global __vt__6TYumbo
__vt__6TYumbo:
	.incbin "baserom.dol", 0x3DEA6C, 0x1A8
.global __vt__13TYumboManager
__vt__13TYumboManager:
	.incbin "baserom.dol", 0x3DEC14, 0x58
.global __vt__10TYumboSeed
__vt__10TYumboSeed:
	.incbin "baserom.dol", 0x3DEC6C, 0xAC
.global koopajr_bastable
koopajr_bastable:
	.incbin "baserom.dol", 0x3DED18, 0x10
.global TKoopaJr_jointNameTable
TKoopaJr_jointNameTable:
	.incbin "baserom.dol", 0x3DED28, 0x14
.global __vt__24TKoopaJrSubmarineManager
__vt__24TKoopaJrSubmarineManager:
	.incbin "baserom.dol", 0x3DED3C, 0x54
.global __vt__34TNerveKoopaJrSubmarineLaunchKiller
__vt__34TNerveKoopaJrSubmarineLaunchKiller:
	.incbin "baserom.dol", 0x3DED90, 0x10
.global __vt__37TNerveKoopaJrSubmarineCannonOpenClose
__vt__37TNerveKoopaJrSubmarineCannonOpenClose:
	.incbin "baserom.dol", 0x3DEDA0, 0x10
.global __vt__26TNerveKoopaJrSubmarineWait
__vt__26TNerveKoopaJrSubmarineWait:
	.incbin "baserom.dol", 0x3DEDB0, 0x10
.global __vt__17TKoopaJrSubmarine
__vt__17TKoopaJrSubmarine:
	.incbin "baserom.dol", 0x3DEDC0, 0x114
.global __vt__15TKoopaJrManager
__vt__15TKoopaJrManager:
	.incbin "baserom.dol", 0x3DEED4, 0x54
.global __vt__18TNerveKoopaJrYahoo
__vt__18TNerveKoopaJrYahoo:
	.incbin "baserom.dol", 0x3DEF28, 0x10
.global __vt__19TNerveKoopaJrLaunch
__vt__19TNerveKoopaJrLaunch:
	.incbin "baserom.dol", 0x3DEF38, 0x10
.global __vt__17TNerveKoopaJrDemo
__vt__17TNerveKoopaJrDemo:
	.incbin "baserom.dol", 0x3DEF48, 0x10
.global __vt__19TNerveKoopaJrDamage
__vt__19TNerveKoopaJrDamage:
	.incbin "baserom.dol", 0x3DEF58, 0x10
.global __vt__17TNerveKoopaJrWait
__vt__17TNerveKoopaJrWait:
	.incbin "baserom.dol", 0x3DEF68, 0x10
.global __vt__8TKoopaJr
__vt__8TKoopaJr:
	.incbin "baserom.dol", 0x3DEF78, 0x114
.global __vt__17TCallbackHitActor
__vt__17TCallbackHitActor:
	.incbin "baserom.dol", 0x3DF08C, 0xA4
.global kumokun_bastable
kumokun_bastable:
	.incbin "baserom.dol", 0x3DF130, 0x34
.global kumokun_bas_c_table
kumokun_bas_c_table:
	.incbin "baserom.dol", 0x3DF164, 0x34
.global __vt__19TNerveKumokunPreFly
__vt__19TNerveKumokunPreFly:
	.incbin "baserom.dol", 0x3DF198, 0x10
.global __vt__16TSolidStack$$0PCc$$1
__vt__16TSolidStack$$0PCc$$1:
	.incbin "baserom.dol", 0x3DF1A8, 0xC
.global __vt__16TNerveKumokunFly
__vt__16TNerveKumokunFly:
	.incbin "baserom.dol", 0x3DF1B4, 0x10
.global __vt__23TNerveKumokunPostFreeze
__vt__23TNerveKumokunPostFreeze:
	.incbin "baserom.dol", 0x3DF1C4, 0x10
.global __vt__19TNerveKumokunFreeze
__vt__19TNerveKumokunFreeze:
	.incbin "baserom.dol", 0x3DF1D4, 0x10
.global __vt__17TNerveKumokunWait
__vt__17TNerveKumokunWait:
	.incbin "baserom.dol", 0x3DF1E4, 0x10
.global __vt__19TNerveKumokunSearch
__vt__19TNerveKumokunSearch:
	.incbin "baserom.dol", 0x3DF1F4, 0x10
.global __vt__21TNerveKumokunPostWalk
__vt__21TNerveKumokunPostWalk:
	.incbin "baserom.dol", 0x3DF204, 0x10
.global __vt__17TNerveKumokunWalk
__vt__17TNerveKumokunWalk:
	.incbin "baserom.dol", 0x3DF214, 0x10
.global __vt__20TNerveKumokunPreWalk
__vt__20TNerveKumokunPreWalk:
	.incbin "baserom.dol", 0x3DF224, 0x10
.global __vt__15TKumokunManager
__vt__15TKumokunManager:
	.incbin "baserom.dol", 0x3DF234, 0x58
.global __vt__8TKumokun
__vt__8TKumokun:
	.incbin "baserom.dol", 0x3DF28C, 0x1AC
.global koopa_bastable
koopa_bastable:
	.incbin "baserom.dol", 0x3DF438, 0x3C
.global __vt__13TKoopaManager
__vt__13TKoopaManager:
	.incbin "baserom.dol", 0x3DF474, 0x54
.global $$25035
$$25035:
	.incbin "baserom.dol", 0x3DF4C8, 0x3C
.global __vt__16TNerveKoopaTurnR
__vt__16TNerveKoopaTurnR:
	.incbin "baserom.dol", 0x3DF504, 0x10
.global __vt__16TNerveKoopaTurnL
__vt__16TNerveKoopaTurnL:
	.incbin "baserom.dol", 0x3DF514, 0x10
.global __vt__15TNerveKoopaTurn
__vt__15TNerveKoopaTurn:
	.incbin "baserom.dol", 0x3DF524, 0x10
.global __vt__6TKoopa
__vt__6TKoopa:
	.incbin "baserom.dol", 0x3DF534, 0x114
.global __vt__10TKoopaBody
__vt__10TKoopaBody:
	.incbin "baserom.dol", 0x3DF648, 0xAC
.global __vt__10TKoopaHead
__vt__10TKoopaHead:
	.incbin "baserom.dol", 0x3DF6F4, 0xAC
.global __vt__10TKoopaHand
__vt__10TKoopaHand:
	.incbin "baserom.dol", 0x3DF7A0, 0xAC
.global __vt__11TKoopaFlame
__vt__11TKoopaFlame:
	.incbin "baserom.dol", 0x3DF84C, 0xAC
.global __vt__11TKoopaParts
__vt__11TKoopaParts:
	.incbin "baserom.dol", 0x3DF8F8, 0xAC
.global __vt__18TNerveKoopaGetDown
__vt__18TNerveKoopaGetDown:
	.incbin "baserom.dol", 0x3DF9A4, 0x10
.global __vt__22TNerveKoopaGetShowered
__vt__22TNerveKoopaGetShowered:
	.incbin "baserom.dol", 0x3DF9B4, 0x10
.global __vt__18TNerveKoopaStagger
__vt__18TNerveKoopaStagger:
	.incbin "baserom.dol", 0x3DF9C4, 0x10
.global __vt__18TNerveKoopaProvoke
__vt__18TNerveKoopaProvoke:
	.incbin "baserom.dol", 0x3DF9D4, 0x10
.global __vt__16TNerveKoopaFlame
__vt__16TNerveKoopaFlame:
	.incbin "baserom.dol", 0x3DF9E4, 0x10
.global __vt__15TNerveKoopaFall
__vt__15TNerveKoopaFall:
	.incbin "baserom.dol", 0x3DF9F4, 0x10
.global __vt__17TNerveKoopaTumble
__vt__17TNerveKoopaTumble:
	.incbin "baserom.dol", 0x3DFA04, 0x10
.global __vt__15TNerveKoopaWait
__vt__15TNerveKoopaWait:
	.incbin "baserom.dol", 0x3DFA14, 0x14
.global tori_bastable
tori_bastable:
	.incbin "baserom.dol", 0x3DFA28, 0x18
.global __vt__23TNerveKukkuRecoverGraph
__vt__23TNerveKukkuRecoverGraph:
	.incbin "baserom.dol", 0x3DFA40, 0x10
.global __vt__19TNerveKukkuPostFall
__vt__19TNerveKukkuPostFall:
	.incbin "baserom.dol", 0x3DFA50, 0x10
.global __vt__15TNerveKukkuFall
__vt__15TNerveKukkuFall:
	.incbin "baserom.dol", 0x3DFA60, 0x10
.global __vt__22TNerveKukkuGraphWander
__vt__22TNerveKukkuGraphWander:
	.incbin "baserom.dol", 0x3DFA70, 0x10
.global __vt__6TKukku
__vt__6TKukku:
	.incbin "baserom.dol", 0x3DFA80, 0x1A8
.global __vt__13TKukkuManager
__vt__13TKukkuManager:
	.incbin "baserom.dol", 0x3DFC28, 0x58
.global __vt__10TKukkuBall
__vt__10TKukkuBall:
	.incbin "baserom.dol", 0x3DFC80, 0xA8
.global amenbo_bastable
amenbo_bastable:
	.incbin "baserom.dol", 0x3DFD28, 0x1C
.global __vt__20TNerveAmenboHitWater
__vt__20TNerveAmenboHitWater:
	.incbin "baserom.dol", 0x3DFD44, 0x10
.global __vt__16TNerveAmenboWalk
__vt__16TNerveAmenboWalk:
	.incbin "baserom.dol", 0x3DFD54, 0x10
.global __vt__21TNerveAmenboPreAttack
__vt__21TNerveAmenboPreAttack:
	.incbin "baserom.dol", 0x3DFD64, 0x10
.global __vt__16TNerveAmenboTurn
__vt__16TNerveAmenboTurn:
	.incbin "baserom.dol", 0x3DFD74, 0x10
.global __vt__18TNerveAmenboSearch
__vt__18TNerveAmenboSearch:
	.incbin "baserom.dol", 0x3DFD84, 0x10
.global __vt__7TAmenbo
__vt__7TAmenbo:
	.incbin "baserom.dol", 0x3DFD94, 0x1A8
.global __vt__14TAmenboManager
__vt__14TAmenboManager:
	.incbin "baserom.dol", 0x3DFF3C, 0x5C
.global bathtubpeach_bastable
bathtubpeach_bastable:
	.incbin "baserom.dol", 0x3DFF98, 0x5C
.global __vt__20TBathtubPeachManager
__vt__20TBathtubPeachManager:
	.incbin "baserom.dol", 0x3DFFF4, 0x54
.global __vt__17TNervePeachEscape
__vt__17TNervePeachEscape:
	.incbin "baserom.dol", 0x3E0048, 0x10
.global __vt__18TNervePeachStagger
__vt__18TNervePeachStagger:
	.incbin "baserom.dol", 0x3E0058, 0x10
.global __vt__13TBathtubPeach
__vt__13TBathtubPeach:
	.incbin "baserom.dol", 0x3E0068, 0x118
.global bathtubkiller_bastable
bathtubkiller_bastable:
	.incbin "baserom.dol", 0x3E0180, 0xC
.global entry$3235
entry$3235:
	.incbin "baserom.dol", 0x3E018C, 0x24
.global __vt__21TBathtubKillerManager
__vt__21TBathtubKillerManager:
	.incbin "baserom.dol", 0x3E01B0, 0x58
.global __vt__28TNerveBathtubKillerExplosion
__vt__28TNerveBathtubKillerExplosion:
	.incbin "baserom.dol", 0x3E0208, 0x10
.global __vt__24TNerveBathtubKillerBreak
__vt__24TNerveBathtubKillerBreak:
	.incbin "baserom.dol", 0x3E0218, 0x10
.global __vt__27TNerveBathtubKillerStraight
__vt__27TNerveBathtubKillerStraight:
	.incbin "baserom.dol", 0x3E0228, 0x10
.global __vt__32TNerveBathtubKillerChaseStraight
__vt__32TNerveBathtubKillerChaseStraight:
	.incbin "baserom.dol", 0x3E0238, 0x10
.global __vt__24TNerveBathtubKillerChase
__vt__24TNerveBathtubKillerChase:
	.incbin "baserom.dol", 0x3E0248, 0x10
.global __vt__25TNerveBathtubKillerWander
__vt__25TNerveBathtubKillerWander:
	.incbin "baserom.dol", 0x3E0258, 0x10
.global __vt__14TBathtubKiller
__vt__14TBathtubKiller:
	.incbin "baserom.dol", 0x3E0268, 0x1A8
.global killer_bastable
killer_bastable:
	.incbin "baserom.dol", 0x3E0410, 0x14
.global entry$2930
entry$2930:
	.incbin "baserom.dol", 0x3E0424, 0x24
.global __vt__21TCoasterKillerManager
__vt__21TCoasterKillerManager:
	.incbin "baserom.dol", 0x3E0448, 0x58
.global __vt__28TNerveCoasterKillerExplosion
__vt__28TNerveCoasterKillerExplosion:
	.incbin "baserom.dol", 0x3E04A0, 0x10
.global __vt__14TCoasterKiller
__vt__14TCoasterKiller:
	.incbin "baserom.dol", 0x3E04B0, 0x1C4
.global __vt__24TNerveCoasterEnemyWander
__vt__24TNerveCoasterEnemyWander:
	.incbin "baserom.dol", 0x3E0674, 0x10
.global __vt__13TCoasterEnemy
__vt__13TCoasterEnemy:
	.incbin "baserom.dol", 0x3E0684, 0x1C4
.global __vt__20TNerveDebuTelesaWait
__vt__20TNerveDebuTelesaWait:
	.incbin "baserom.dol", 0x3E0848, 0x10
.global __vt__18TDebuTelesaManager
__vt__18TDebuTelesaManager:
	.incbin "baserom.dol", 0x3E0858, 0x58
.global __vt__11TDebuTelesa
__vt__11TDebuTelesa:
	.incbin "baserom.dol", 0x3E08B0, 0x1A8
.global tabepuku_bastable
tabepuku_bastable:
	.incbin "baserom.dol", 0x3E0A58, 0xC
.global __vt__18TNerveTabePukuDrag
__vt__18TNerveTabePukuDrag:
	.incbin "baserom.dol", 0x3E0A64, 0x10
.global __vt__18TNerveTabePukuDive
__vt__18TNerveTabePukuDive:
	.incbin "baserom.dol", 0x3E0A74, 0x10
.global __vt__18TNerveTabePukuBite
__vt__18TNerveTabePukuBite:
	.incbin "baserom.dol", 0x3E0A84, 0x10
.global __vt__20TNerveTabePukuAttack
__vt__20TNerveTabePukuAttack:
	.incbin "baserom.dol", 0x3E0A94, 0x10
.global __vt__26TNerveTabePukuRecoverGraph
__vt__26TNerveTabePukuRecoverGraph:
	.incbin "baserom.dol", 0x3E0AA4, 0x10
.global __vt__19TNerveTabePukuFound
__vt__19TNerveTabePukuFound:
	.incbin "baserom.dol", 0x3E0AB4, 0x10
.global __vt__25TNerveTabePukuGraphWander
__vt__25TNerveTabePukuGraphWander:
	.incbin "baserom.dol", 0x3E0AC4, 0x10
.global __vt__16TTabePukuManager
__vt__16TTabePukuManager:
	.incbin "baserom.dol", 0x3E0AD4, 0x58
.global __vt__9TTabePuku
__vt__9TTabePuku:
	.incbin "baserom.dol", 0x3E0B2C, 0x1A8
.global __vt__11TTPHitActor
__vt__11TTPHitActor:
	.incbin "baserom.dol", 0x3E0CD4, 0xAC
.global __vt__14TBathtubBinder
__vt__14TBathtubBinder:
	.incbin "baserom.dol", 0x3E0D80, 0x10
.global __vt__18TLimitKoopaManager
__vt__18TLimitKoopaManager:
	.incbin "baserom.dol", 0x3E0D90, 0x54
.global __vt__20TNerveLimitKoopaTurn
__vt__20TNerveLimitKoopaTurn:
	.incbin "baserom.dol", 0x3E0DE4, 0x10
.global __vt__11TLimitKoopa
__vt__11TLimitKoopa:
	.incbin "baserom.dol", 0x3E0DF4, 0x114
.global __vt__15TLimitKoopaBody
__vt__15TLimitKoopaBody:
	.incbin "baserom.dol", 0x3E0F08, 0x100
.global __vt__15TLimitKoopaHead
__vt__15TLimitKoopaHead:
	.incbin "baserom.dol", 0x3E1008, 0x100
.global __vt__15TLimitKoopaHand
__vt__15TLimitKoopaHand:
	.incbin "baserom.dol", 0x3E1108, 0x100
.global __vt__16TLimitKoopaFlame
__vt__16TLimitKoopaFlame:
	.incbin "baserom.dol", 0x3E1208, 0x100
.global __vt__16TLimitKoopaParts
__vt__16TLimitKoopaParts:
	.incbin "baserom.dol", 0x3E1308, 0x100
.global __vt__23TNerveLimitKoopaGetDown
__vt__23TNerveLimitKoopaGetDown:
	.incbin "baserom.dol", 0x3E1408, 0x10
.global __vt__27TNerveLimitKoopaGetShowered
__vt__27TNerveLimitKoopaGetShowered:
	.incbin "baserom.dol", 0x3E1418, 0x10
.global __vt__23TNerveLimitKoopaStagger
__vt__23TNerveLimitKoopaStagger:
	.incbin "baserom.dol", 0x3E1428, 0x10
.global __vt__22TNerveLimitKoopaTumble
__vt__22TNerveLimitKoopaTumble:
	.incbin "baserom.dol", 0x3E1438, 0x10
.global __vt__20TNerveLimitKoopaWait
__vt__20TNerveLimitKoopaWait:
	.incbin "baserom.dol", 0x3E1448, 0x10
.global __vt__28TNerveLimitKoopaHipDropStart
__vt__28TNerveLimitKoopaHipDropStart:
	.incbin "baserom.dol", 0x3E1458, 0x10
.global __vt__27TNerveLimitKoopaHipDropJump
__vt__27TNerveLimitKoopaHipDropJump:
	.incbin "baserom.dol", 0x3E1468, 0x10
.global koopajr_bastable
koopajr_bastable:
	.incbin "baserom.dol", 0x3E1478, 0x10
.global __vt__20TLimitKoopaJrManager
__vt__20TLimitKoopaJrManager:
	.incbin "baserom.dol", 0x3E1488, 0x54
.global __vt__23TNerveLimitKoopaJrYahoo
__vt__23TNerveLimitKoopaJrYahoo:
	.incbin "baserom.dol", 0x3E14DC, 0x10
.global __vt__24TNerveLimitKoopaJrLaunch
__vt__24TNerveLimitKoopaJrLaunch:
	.incbin "baserom.dol", 0x3E14EC, 0x10
.global __vt__22TNerveLimitKoopaJrWait
__vt__22TNerveLimitKoopaJrWait:
	.incbin "baserom.dol", 0x3E14FC, 0x10
.global __vt__21TNerveLimitKoopaJrRun
__vt__21TNerveLimitKoopaJrRun:
	.incbin "baserom.dol", 0x3E150C, 0x10
.global __vt__13TLimitKoopaJr
__vt__13TLimitKoopaJr:
	.incbin "baserom.dol", 0x3E151C, 0x114
.global $$23698
$$23698:
	.incbin "baserom.dol", 0x3E1630, 0xCC
.global $$23874
$$23874:
	.incbin "baserom.dol", 0x3E16FC, 0x124
.global __vt__15CPolarSubCamera
__vt__15CPolarSubCamera:
	.incbin "baserom.dol", 0x3E1820, 0xF8
.global $$21767
$$21767:
	.incbin "baserom.dol", 0x3E1918, 0xE8
.global $$21790
$$21790:
	.incbin "baserom.dol", 0x3E1A00, 0xE8
.global sNoticeActorManagerName
sNoticeActorManagerName:
	.incbin "baserom.dol", 0x3E1AE8, 0x10
.global sPositionNameTable$localstatic0$calcTowerCenterPos___15CPolarSubCameraFP3Vec
sPositionNameTable$localstatic0$calcTowerCenterPos___15CPolarSubCameraFP3Vec:
	.incbin "baserom.dol", 0x3E1AF8, 0x18
.global __vt__10TLensFlare
__vt__10TLensFlare:
	.incbin "baserom.dol", 0x3E1B10, 0x28
.global __vt__9TLensGlow
__vt__9TLensGlow:
	.incbin "baserom.dol", 0x3E1B38, 0x28
.global __vt__7TSunMgr
__vt__7TSunMgr:
	.incbin "baserom.dol", 0x3E1B60, 0x38
.global __vt__9TSunModel
__vt__9TSunModel:
	.incbin "baserom.dol", 0x3E1B98, 0xA0
.global __vt__16TCubeManagerBase
__vt__16TCubeManagerBase:
	.incbin "baserom.dol", 0x3E1C38, 0x28
.global __vt__14TCameraMapTool
__vt__14TCameraMapTool:
	.incbin "baserom.dol", 0x3E1C60, 0x20
.global __vt__15TCubeStreamInfo
__vt__15TCubeStreamInfo:
	.incbin "baserom.dol", 0x3E1C80, 0x20
.global __vt__15TCubeCameraInfo
__vt__15TCubeCameraInfo:
	.incbin "baserom.dol", 0x3E1CA0, 0x20
.global __vt__16TCubeGeneralInfo
__vt__16TCubeGeneralInfo:
	.incbin "baserom.dol", 0x3E1CC0, 0x20
.global sAddBckFileNameTable
sAddBckFileNameTable:
	.incbin "baserom.dol", 0x3E1CE0, 0x10
.global mCamKindNameSaveFile__15CPolarSubCamera
mCamKindNameSaveFile__15CPolarSubCamera:
	.incbin "baserom.dol", 0x3E1CF0, 0x124
.global $$21781
$$21781:
	.incbin "baserom.dol", 0x3E1E14, 0x124
.global $$21809
$$21809:
	.incbin "baserom.dol", 0x3E1F38, 0x104
.global $$21895
$$21895:
	.incbin "baserom.dol", 0x3E203C, 0x124
.global $$21939
$$21939:
	.incbin "baserom.dol", 0x3E2160, 0x128
.global mCamShakeNameSave__12TCameraShake
mCamShakeNameSave__12TCameraShake:
	.incbin "baserom.dol", 0x3E2288, 0xA8
.global __vt__11TBoidLeader
__vt__11TBoidLeader:
	.incbin "baserom.dol", 0x3E2330, 0x28
.global __vt__15TFishoidManager
__vt__15TFishoidManager:
	.incbin "baserom.dol", 0x3E2358, 0x54
.global __vt__8TFishoid
__vt__8TFishoid:
	.incbin "baserom.dol", 0x3E23AC, 0x118
.global __vt__5TFish
__vt__5TFish:
	.incbin "baserom.dol", 0x3E24C4, 0xB8
.global __vt__8TRealoid
__vt__8TRealoid:
	.incbin "baserom.dol", 0x3E257C, 0x118
.global __vt__13TRealoidActor
__vt__13TRealoidActor:
	.incbin "baserom.dol", 0x3E2694, 0xBC
.global __vt__11TAnimalBase
__vt__11TAnimalBase:
	.incbin "baserom.dol", 0x3E2750, 0x118
.global __vt__11TMewManager
__vt__11TMewManager:
	.incbin "baserom.dol", 0x3E2868, 0x54
.global __vt__18TAnimalManagerBase
__vt__18TAnimalManagerBase:
	.incbin "baserom.dol", 0x3E28BC, 0x54
.global __vt__23TNerveAnimalGraphWander
__vt__23TNerveAnimalGraphWander:
	.incbin "baserom.dol", 0x3E2910, 0x10
.global bird_bastable
bird_bastable:
	.incbin "baserom.dol", 0x3E2920, 0x24
.global __vt__23TNerveAnimalBirdLanding
__vt__23TNerveAnimalBirdLanding:
	.incbin "baserom.dol", 0x3E2944, 0x10
.global __vt__26TNerveAnimalBirdPreLanding
__vt__26TNerveAnimalBirdPreLanding:
	.incbin "baserom.dol", 0x3E2954, 0x10
.global __vt__24TNerveAnimalBirdComeback
__vt__24TNerveAnimalBirdComeback:
	.incbin "baserom.dol", 0x3E2964, 0x10
.global __vt__28TNerveAnimalBirdChangeToCoin
__vt__28TNerveAnimalBirdChangeToCoin:
	.incbin "baserom.dol", 0x3E2974, 0x10
.global __vt__27TNerveAnimalBirdGraphWander
__vt__27TNerveAnimalBirdGraphWander:
	.incbin "baserom.dol", 0x3E2984, 0x10
.global __vt__23TNerveAnimalBirdTakeoff
__vt__23TNerveAnimalBirdTakeoff:
	.incbin "baserom.dol", 0x3E2994, 0x10
.global __vt__28TNerveAnimalBirdWalkOnGround
__vt__28TNerveAnimalBirdWalkOnGround:
	.incbin "baserom.dol", 0x3E29A4, 0x10
.global __vt__30TNerveAnimalBirdActionOnGround
__vt__30TNerveAnimalBirdActionOnGround:
	.incbin "baserom.dol", 0x3E29B4, 0x10
.global __vt__28TNerveAnimalBirdWaitOnGround
__vt__28TNerveAnimalBirdWaitOnGround:
	.incbin "baserom.dol", 0x3E29C4, 0x10
.global __vt__18TAnimalBirdManager
__vt__18TAnimalBirdManager:
	.incbin "baserom.dol", 0x3E29D4, 0x54
.global __vt__11TAnimalBird
__vt__11TAnimalBird:
	.incbin "baserom.dol", 0x3E2A28, 0x118
.global __vt__18TNerveBeeHiveReset
__vt__18TNerveBeeHiveReset:
	.incbin "baserom.dol", 0x3E2B40, 0x10
.global __vt__25TNerveBeeHiveMarioWaterIn
__vt__25TNerveBeeHiveMarioWaterIn:
	.incbin "baserom.dol", 0x3E2B50, 0x10
.global __vt__19TNerveBeeHiveAttack
__vt__19TNerveBeeHiveAttack:
	.incbin "baserom.dol", 0x3E2B60, 0x10
.global __vt__18TNerveBeeHiveBreak
__vt__18TNerveBeeHiveBreak:
	.incbin "baserom.dol", 0x3E2B70, 0x10
.global __vt__17TNerveBeeHiveFall
__vt__17TNerveBeeHiveFall:
	.incbin "baserom.dol", 0x3E2B80, 0x10
.global __vt__17TNerveBeeHiveWait
__vt__17TNerveBeeHiveWait:
	.incbin "baserom.dol", 0x3E2B90, 0x10
.global __vt__15TBeeHiveManager
__vt__15TBeeHiveManager:
	.incbin "baserom.dol", 0x3E2BA0, 0x54
.global __vt__8TBeeHive
__vt__8TBeeHive:
	.incbin "baserom.dol", 0x3E2BF4, 0x118
.global __vt__4TBee
__vt__4TBee:
	.incbin "baserom.dol", 0x3E2D0C, 0xBC
.global __vt__19TButterfloidManager
__vt__19TButterfloidManager:
	.incbin "baserom.dol", 0x3E2DC8, 0x54
.global __vt__12TButterfloid
__vt__12TButterfloid:
	.incbin "baserom.dol", 0x3E2E1C, 0x118
.global __vt__10TButterfly
__vt__10TButterfly:
	.incbin "baserom.dol", 0x3E2F34, 0xCC

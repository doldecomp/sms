#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTevBlocks.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DPEBlocks.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DColorBlocks.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DIndBlocks.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <dolphin/gd.h>
#include <dolphin/os.h>
#include <macros.h>

#pragma opt_strength_reduction off

void J3DColorBlockLightOff::initialize()
{
	mColorChanNum = 0;
	for (u32 i = 0; i < ARRAY_COUNT(mMatColor); ++i)
		mMatColor[i] = j3dDefaultColInfo;
	for (u32 i = 0; i < ARRAY_COUNT(mColorChan); ++i)
		mColorChan[i].mChanCtrl = 0xFFFF;
}

void J3DColorBlockLightOn::initialize()
{
	mColorChanNum = 0;
	for (u32 i = 0; i < ARRAY_COUNT(mMatColor); i++)
		mMatColor[i] = j3dDefaultColInfo;
	for (u32 i = 0; i < ARRAY_COUNT(mAmbColor); i++)
		mAmbColor[i] = j3dDefaultAmbInfo;
	for (u32 i = 0; i < ARRAY_COUNT(mColorChan); ++i)
		mColorChan[i].mChanCtrl = 0xFFFF;
	for (u32 i = 0; i < ARRAY_COUNT(mLight); ++i)
		mLight[i] = nullptr;
}

void J3DTevBlock2::initialize()
{
	mTexNo[0] = 0xFFFF;
	mTexNo[1] = 0xFFFF;

	mTevStageNum = 1;

	mTevStage[0].mTevColorReg = 0xC0;
	mTevStage[0].mTevAlphaReg = 0xC1;
	mTevStage[1].mTevColorReg = 0xC2;
	mTevStage[1].mTevAlphaReg = 0xC3;

	mTevKColorSel[0] = GX_TEV_KCSEL_K0;
	mTevKColorSel[1] = GX_TEV_KCSEL_K0;
	mTevKAlphaSel[0] = GX_TEV_KASEL_K0_A;
	mTevKAlphaSel[1] = GX_TEV_KASEL_K0_A;

	for (int i = 0; i < 3; ++i)
		mTevColor[i] = j3dDefaultTevColor;

	for (int i = 0; i < ARRAY_COUNT(mTevKColor); ++i)
		mTevKColor[i] = j3dDefaultTevKColor;
}
void J3DTevBlock4::initialize()
{
	mTexNo[0] = 0xFFFF;
	mTexNo[1] = 0xFFFF;
	mTexNo[2] = 0xFFFF;
	mTexNo[3] = 0xFFFF;

	mTevStageNum = 1;

	mTevStage[0].mTevColorReg = 0xC0;
	mTevStage[0].mTevAlphaReg = 0xC1;
	mTevStage[1].mTevColorReg = 0xC2;
	mTevStage[1].mTevAlphaReg = 0xC3;
	mTevStage[2].mTevColorReg = 0xC4;
	mTevStage[2].mTevAlphaReg = 0xC5;
	mTevStage[3].mTevColorReg = 0xC6;
	mTevStage[3].mTevAlphaReg = 0xC7;

	mTevKColorSel[0] = GX_TEV_KCSEL_K0;
	mTevKColorSel[1] = GX_TEV_KCSEL_K0;
	mTevKColorSel[2] = GX_TEV_KCSEL_K0;
	mTevKColorSel[3] = GX_TEV_KCSEL_K0;
	mTevKAlphaSel[0] = GX_TEV_KASEL_K0_A;
	mTevKAlphaSel[1] = GX_TEV_KASEL_K0_A;
	mTevKAlphaSel[2] = GX_TEV_KASEL_K0_A;
	mTevKAlphaSel[3] = GX_TEV_KASEL_K0_A;

	for (int i = 0; i < 3; ++i)
		mTevColor[i] = j3dDefaultTevColor;

	for (int i = 0; i < ARRAY_COUNT(mTevKColor); ++i)
		mTevKColor[i] = j3dDefaultTevKColor;
}
void J3DTevBlock16::initialize()
{
	for (int i = 0; i < ARRAY_COUNT(mTexNo); ++i)
		mTexNo[i] = 0xFFFF;

	mTevStageNum = 1;

	for (int i = 0; i < 3; i++)
		mTevColor[i] = j3dDefaultTevColor;

	for (int i = 0; i < ARRAY_COUNT(mTevKColor); ++i)
		mTevKColor[i] = j3dDefaultTevKColor;

	for (int i = 0; i < ARRAY_COUNT(mTevKColorSel); ++i)
		mTevKColorSel[i] = GX_TEV_KCSEL_K0;

	for (int i = 0; i < ARRAY_COUNT(mTevKAlphaSel); ++i)
		mTevKAlphaSel[i] = GX_TEV_KASEL_K0_A;

	for (int i = 0; i < ARRAY_COUNT(mTevStage); ++i) {
		mTevStage[i].mTevColorReg = 0xC0 + i * 2;
		mTevStage[i].mTevAlphaReg = 0xC1 + i * 2;
	}
}

s32 J3DColorBlockLightOff::countDLSize() { return 0x60; }
s32 J3DColorBlockLightOn::countDLSize() { return 0x140; }

s32 J3DIndBlockFull::countDLSize() { return 0x64; }

s32 J3DTevBlock1::countDLSize() { return 0x80; }
s32 J3DTevBlock2::countDLSize() { return 0x180; }
s32 J3DTevBlock4::countDLSize() { return 0x260; }
s32 J3DTevBlock16::countDLSize() { return 0x580; }

s32 J3DPEBlockOpa::countDLSize() { return 0x20; }
s32 J3DPEBlockTexEdge::countDLSize() { return 0x20; }
s32 J3DPEBlockXlu::countDLSize() { return 0x20; }
s32 J3DPEBlockFull::countDLSize() { return 0x60; }

// TODO: send to some header? Doesn't look like it should be public
extern void loadTexNo(u32, const u16&);

inline void loadTexCoordScale(GXTexCoordID coord,
                              const J3DTexCoordScaleInfo& info)
{
	J3DGDSetTexCoordScale2(coord, info.field_0x00, info.field_0x04 == 1, 0,
	                       info.field_0x02, info.field_0x06 == 1, 0);
}

// TODO: header
extern void loadCullMode(u8);

void J3DColorBlockLightOff::load()
{
	for (u32 i = 0; i < ARRAY_COUNT(mMatColor); ++i)
		J3DGDSetChanMatColor((GXChannelID)(GX_COLOR0A0 + i),
		                     mMatColor[i].color);

	for (u32 i = 0; i < ARRAY_COUNT(mColorChan); ++i)
		mColorChan[i].load(i);

	if (mCullMode != 0xFF)
		loadCullMode(mCullMode);
}

void J3DColorBlockLightOn::load()
{
	for (u32 i = 0; i < ARRAY_COUNT(mMatColor); ++i)
		J3DGDSetChanMatColor((GXChannelID)(GX_COLOR0A0 + i),
		                     mMatColor[i].color);

	for (u32 i = 0; i < ARRAY_COUNT(mAmbColor); ++i)
		J3DGDSetChanAmbColor((GXChannelID)(GX_COLOR0A0 + i),
		                     mAmbColor[i].color);

	for (u32 i = 0; i < ARRAY_COUNT(mColorChan); ++i)
		mColorChan[i].load(i);

	for (u32 i = 0; i < 8; i++) {
		if (mLight[i]) {
			mLight[i]->load(i);
		}
	}

	if (mCullMode != 0xFF)
		loadCullMode(mCullMode);
}

void J3DIndBlockFull::load(J3DTevBlock*)
{
	u8 indTexStageNum = mIndTexStageNum;
	for (u32 i = 0; i < indTexStageNum; i++) {
		mIndTexMtx[i].load(i + 1);
	}
	for (u32 i = 0; i < indTexStageNum; i += 2) {
		JRNSetIndTexCoordScale(
		    (GXIndTexStageID)i, (GXIndTexScale)mIndTexCoordScale[i].getScaleS(),
		    (GXIndTexScale)mIndTexCoordScale[i].getScaleT(),
		    (GXIndTexScale)mIndTexCoordScale[i + 1].getScaleS(),
		    (GXIndTexScale)mIndTexCoordScale[i + 1].getScaleT());
	}
	loadTexCoordScale(
	    (GXTexCoordID)mIndTexOrder[0].getCoord(),
	    J3DSys::sTexCoordScaleTable[mIndTexOrder[0].getMap() & 7]);
	loadTexCoordScale(
	    (GXTexCoordID)mIndTexOrder[1].getCoord(),
	    J3DSys::sTexCoordScaleTable[mIndTexOrder[1].getMap() & 7]);
	loadTexCoordScale(
	    (GXTexCoordID)mIndTexOrder[2].getCoord(),
	    J3DSys::sTexCoordScaleTable[mIndTexOrder[2].getMap() & 7]);
	loadTexCoordScale(
	    (GXTexCoordID)mIndTexOrder[3].getCoord(),
	    J3DSys::sTexCoordScaleTable[mIndTexOrder[3].getMap() & 7]);
	JRNSetIndTexOrder(indTexStageNum, (GXTexCoordID)mIndTexOrder[0].getCoord(),
	                  (GXTexMapID)mIndTexOrder[0].getMap(),
	                  (GXTexCoordID)mIndTexOrder[1].getCoord(),
	                  (GXTexMapID)mIndTexOrder[1].getMap(),
	                  (GXTexCoordID)mIndTexOrder[2].getCoord(),
	                  (GXTexMapID)mIndTexOrder[2].getMap(),
	                  (GXTexCoordID)mIndTexOrder[3].getCoord(),
	                  (GXTexMapID)mIndTexOrder[3].getMap());
}

void J3DTevBlock1::load()
{
	if (mTexNo[0] != 0xffff) {
		loadTexNo(0, mTexNo[0]);
	}
	JRNISetTevOrder(GX_TEVSTAGE0,
	                (GXTexCoordID)mTevOrder[0].getTevOrderInfo().mTexCoord,
	                (GXTexMapID)mTevOrder[0].getTevOrderInfo().mTexMap,
	                (GXChannelID)mTevOrder[0].getTevOrderInfo().mColorChan,
	                GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
	loadTexCoordScale(
	    GXTexCoordID(mTevOrder[0].getTevOrderInfo().mTexCoord),
	    J3DSys::sTexCoordScaleTable[mTevOrder[0].getTevOrderInfo().mTexMap
	                                & 7]);
	mTevStage[0].load(0);
	mIndTevStage[0].load(0);
}

inline void loadTevColor(u32 reg, const J3DGXColorS10& color)
{
	JRNISetTevColorS10((GXTevRegID)(reg + 1), color.color);
}

inline void loadTevKColor(u32 reg, const J3DGXColor& color)
{
	J3DGDSetTevKColor((GXTevKColorID)reg, color.color);
}

void J3DTevBlock2::load()
{
	u8 tevStageNum = mTevStageNum;
	for (u32 i = 0; i < 2; i++) {
		if (mTexNo[i] != 0xffff) {
			loadTexNo(i, mTexNo[i]);
		}
	}
	JRNISetTevOrder(GX_TEVSTAGE0,
	                (GXTexCoordID)mTevOrder[0].getTevOrderInfo().mTexCoord,
	                (GXTexMapID)mTevOrder[0].getTevOrderInfo().mTexMap,
	                (GXChannelID)mTevOrder[0].getTevOrderInfo().mColorChan,
	                (GXTexCoordID)mTevOrder[1].getTevOrderInfo().mTexCoord,
	                (GXTexMapID)mTevOrder[1].getTevOrderInfo().mTexMap,
	                (GXChannelID)mTevOrder[1].getTevOrderInfo().mColorChan);
	loadTexCoordScale(
	    (GXTexCoordID)mTevOrder[0].getTevOrderInfo().mTexCoord,
	    J3DSys::sTexCoordScaleTable[mTevOrder[0].getTevOrderInfo().mTexMap
	                                & 7]);
	loadTexCoordScale(
	    (GXTexCoordID)(mTevOrder[1].getTevOrderInfo().mTexCoord & 7),
	    J3DSys::sTexCoordScaleTable[mTevOrder[1].getTevOrderInfo().mTexMap
	                                & 7]);

	for (u32 i = 0; i < 3; i++) {
		loadTevColor(i, mTevColor[i]);
	}
	for (u32 i = 0; i < 4; i++) {
		loadTevKColor(i, mTevKColor[i]);
	}
	for (u32 i = 0; i < tevStageNum; i++) {
		mTevStage[i].load(i);
		mIndTevStage[i].load(i);
	}
	for (u32 i = 0; i < 16; i += 4) {
		J3DGDSetTevKonstantSel_SwapModeTable(
		    (GXTevStageID)i, (GXTevKColorSel)mTevKColorSel[0],
		    (GXTevKAlphaSel)mTevKAlphaSel[0], (GXTevKColorSel)mTevKColorSel[1],
		    (GXTevKAlphaSel)mTevKAlphaSel[1],
		    (GXTevColorChan)mTevSwapModeTable[i / 4].getR(),
		    (GXTevColorChan)mTevSwapModeTable[i / 4].getG());
		J3DGDSetTevKonstantSel_SwapModeTable(
		    (GXTevStageID)(i + 2), (GXTevKColorSel)mTevKColorSel[0],
		    (GXTevKAlphaSel)mTevKAlphaSel[0], (GXTevKColorSel)mTevKColorSel[1],
		    (GXTevKAlphaSel)mTevKAlphaSel[1],
		    (GXTevColorChan)mTevSwapModeTable[i / 4].getB(),
		    (GXTevColorChan)mTevSwapModeTable[i / 4].getA());
	}
}

void J3DTevBlock4::load()
{
	u8 tevStageNum = mTevStageNum;
	for (u32 i = 0; i < 4; i++) {
		if (mTexNo[i] != 0xffff) {
			loadTexNo(i, mTexNo[i]);
		}
	}
	for (u32 i = 0; i < tevStageNum; i += 2) {
		JRNISetTevOrder(
		    (GXTevStageID)i,
		    (GXTexCoordID)mTevOrder[i].getTevOrderInfo().mTexCoord,
		    (GXTexMapID)mTevOrder[i].getTevOrderInfo().mTexMap,
		    (GXChannelID)mTevOrder[i].getTevOrderInfo().mColorChan,
		    (GXTexCoordID)mTevOrder[i + 1].getTevOrderInfo().mTexCoord,
		    (GXTexMapID)mTevOrder[i + 1].getTevOrderInfo().mTexMap,
		    (GXChannelID)mTevOrder[i + 1].getTevOrderInfo().mColorChan);
		loadTexCoordScale(
		    (GXTexCoordID)(mTevOrder[i].getTevOrderInfo().mTexCoord & 7),
		    J3DSys::sTexCoordScaleTable[mTevOrder[i].getTevOrderInfo().mTexMap
		                                & 7]);
		loadTexCoordScale(
		    (GXTexCoordID)(mTevOrder[i + 1].getTevOrderInfo().mTexCoord & 7),
		    J3DSys::sTexCoordScaleTable
		        [mTevOrder[i + 1].getTevOrderInfo().mTexMap & 7]);
	}
	for (u32 i = 0; i < 3; i++) {
		loadTevColor(i, mTevColor[i]);
	}
	for (u32 i = 0; i < 4; i++) {
		loadTevKColor(i, mTevKColor[i]);
	}
	for (u32 i = 0; i < tevStageNum; i++) {
		mTevStage[i].load(i);
		mIndTevStage[i].load(i);
	}
	for (u32 i = 0; i < 16; i += 4) {
		J3DGDSetTevKonstantSel_SwapModeTable(
		    (GXTevStageID)i, (GXTevKColorSel)mTevKColorSel[i],
		    (GXTevKAlphaSel)mTevKAlphaSel[i],
		    (GXTevKColorSel)mTevKColorSel[i + 1],
		    (GXTevKAlphaSel)mTevKAlphaSel[i + 1],
		    (GXTevColorChan)mTevSwapModeTable[i / 4].getR(),
		    (GXTevColorChan)mTevSwapModeTable[i / 4].getG());
		J3DGDSetTevKonstantSel_SwapModeTable(
		    (GXTevStageID)(i + 2), (GXTevKColorSel)mTevKColorSel[i + 2],
		    (GXTevKAlphaSel)mTevKAlphaSel[i + 2],
		    (GXTevKColorSel)mTevKColorSel[i + 3],
		    (GXTevKAlphaSel)mTevKAlphaSel[i + 3],
		    (GXTevColorChan)mTevSwapModeTable[i / 4].getB(),
		    (GXTevColorChan)mTevSwapModeTable[i / 4].getA());
	}
}

void J3DTevBlock16::load()
{

	u8 tevStageNum = mTevStageNum;
	for (u32 i = 0; i < 8; i++) {
		if (mTexNo[i] != 0xffff) {
			loadTexNo(i, mTexNo[i]);
		}
	}
	for (u32 i = 0; i < tevStageNum; i += 2) {
		JRNISetTevOrder(
		    (GXTevStageID)i,
		    (GXTexCoordID)mTevOrder[i].getTevOrderInfo().mTexCoord,
		    (GXTexMapID)mTevOrder[i].getTevOrderInfo().mTexMap,
		    (GXChannelID)mTevOrder[i].getTevOrderInfo().mColorChan,
		    (GXTexCoordID)mTevOrder[i + 1].getTevOrderInfo().mTexCoord,
		    (GXTexMapID)mTevOrder[i + 1].getTevOrderInfo().mTexMap,
		    (GXChannelID)mTevOrder[i + 1].getTevOrderInfo().mColorChan);
		loadTexCoordScale(
		    (GXTexCoordID)(mTevOrder[i].getTevOrderInfo().mTexCoord & 7),
		    J3DSys::sTexCoordScaleTable[mTevOrder[i].getTevOrderInfo().mTexMap
		                                & 7]);
		loadTexCoordScale(
		    (GXTexCoordID)(mTevOrder[i + 1].getTevOrderInfo().mTexCoord & 7),
		    J3DSys::sTexCoordScaleTable
		        [mTevOrder[i + 1].getTevOrderInfo().mTexMap & 7]);
	}
	for (u32 i = 0; i < 3; i++) {
		loadTevColor(i, mTevColor[i]);
	}
	for (u32 i = 0; i < 4; i++) {
		loadTevKColor(i, mTevKColor[i]);
	}
	for (u32 i = 0; i < tevStageNum; i++) {
		mTevStage[i].load(i);
		mIndTevStage[i].load(i);
	}
	for (u32 i = 0; i < 16; i += 4) {
		J3DGDSetTevKonstantSel_SwapModeTable(
		    (GXTevStageID)i, (GXTevKColorSel)mTevKColorSel[i],
		    (GXTevKAlphaSel)mTevKAlphaSel[i],
		    (GXTevKColorSel)mTevKColorSel[i + 1],
		    (GXTevKAlphaSel)mTevKAlphaSel[i + 1],
		    (GXTevColorChan)mTevSwapModeTable[i / 4].getR(),
		    (GXTevColorChan)mTevSwapModeTable[i / 4].getG());
		J3DGDSetTevKonstantSel_SwapModeTable(
		    (GXTevStageID)(i + 2), (GXTevKColorSel)mTevKColorSel[i + 2],
		    (GXTevKAlphaSel)mTevKAlphaSel[i + 2],
		    (GXTevKColorSel)mTevKColorSel[i + 3],
		    (GXTevKAlphaSel)mTevKAlphaSel[i + 3],
		    (GXTevColorChan)mTevSwapModeTable[i / 4].getB(),
		    (GXTevColorChan)mTevSwapModeTable[i / 4].getA());
	}
}

void J3DPEBlockOpa::load()
{
	GDSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
	GDSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_COPY);
	GDSetZMode(1, GX_LEQUAL, 1);
	J3DGDSetZCompLoc(1);
}

void J3DPEBlockTexEdge::load()
{
	GDSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_AND, GX_LEQUAL, 0xff);
	GDSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_COPY);
	GDSetZMode(1, GX_LEQUAL, 1);
	J3DGDSetZCompLoc(0);
}

void J3DPEBlockXlu::load()
{
	GDSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
	GDSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_COPY);
	GDSetZMode(1, GX_LEQUAL, 0);
	J3DGDSetZCompLoc(1);
}

inline void loadZCompLoc(u8 compLoc)
{
	if (compLoc != 0xff)
		J3DGDSetZCompLoc(compLoc);
}

// TODO: header
extern void loadDither(u8);

void J3DPEBlockFull::load()
{
	if (mFog) {
		mFog->load();
	}
	mAlphaComp.load();
	mBlend.load();
	mZMode.load();
	loadZCompLoc(mZCompLoc);
	if (mDither != 0xFF)
		loadDither(mDither);
}

void J3DColorBlockLightOff::reset(J3DColorBlock* block)
{
	mColorChanNum = block->getColorChanNum();

	for (u32 i = 0; i < ARRAY_COUNT(mMatColor); i++)
		mMatColor[i] = *block->getMatColor(i);

	for (u32 i = 0; i < ARRAY_COUNT(mColorChan); i++)
		mColorChan[i] = *block->getColorChan(i);
}

void J3DColorBlockLightOn::reset(J3DColorBlock* block)
{
	mColorChanNum = block->getColorChanNum();

	for (u32 i = 0; i < ARRAY_COUNT(mMatColor); i++)
		mMatColor[i] = *block->getMatColor(i);

	for (u32 i = 0; i < ARRAY_COUNT(mColorChan); i++)
		mColorChan[i] = *block->getColorChan(i);

	for (u32 i = 0; i < ARRAY_COUNT(mAmbColor); i++) {
		if (block->getAmbColor(i))
			mAmbColor[i] = *block->getAmbColor(i);
	}

	for (u32 i = 0; i < ARRAY_COUNT(mLight); i++) {
		if (block->getLight(i) != NULL) {
			if (!mLight[i])
				mLight[i] = new J3DLightObj;

			memcpy(mLight[i], block->getLight(i), sizeof(*mLight[i]));
			DCStoreRange(mLight[i], sizeof(*mLight[i]));
		}
	}
}

void J3DTevBlock1::reset(J3DTevBlock* block)
{
	mTexNo[0]       = block->getTexNo(0);
	mTevOrder[0]    = *block->getTevOrder(0);
	mTevStage[0]    = *block->getTevStage(0);
	mIndTevStage[0] = *block->getIndTevStage(0);
}

void J3DTevBlock2::reset(J3DTevBlock* block)
{
	mTevStageNum     = block->getTevStageNum();
	mTexNo[0]        = block->getTexNo(0);
	mTexNo[1]        = block->getTexNo(1);
	mTevStage[0]     = *block->getTevStage(0);
	mTevStage[1]     = *block->getTevStage(1);
	mIndTevStage[0]  = *block->getIndTevStage(0);
	mIndTevStage[1]  = *block->getIndTevStage(1);
	mTevOrder[0]     = *block->getTevOrder(0);
	mTevOrder[1]     = *block->getTevOrder(1);
	mTevKColorSel[0] = block->getTevKColorSel(0);
	mTevKColorSel[1] = block->getTevKColorSel(1);
	mTevKAlphaSel[0] = block->getTevKAlphaSel(0);
	mTevKAlphaSel[1] = block->getTevKAlphaSel(1);
	for (u32 i = 0; i < 4; ++i)
		mTevColor[i] = *block->getTevColor(i);
	for (u32 i = 0; i < 4; ++i)
		mTevKColor[i] = *block->getTevKColor(i);
	for (u32 i = 0; i < 4; ++i)
		mTevSwapModeTable[i] = *block->getTevSwapModeTable(i);
}

void J3DTevBlock4::reset(J3DTevBlock* block)
{
	mTevStageNum     = block->getTevStageNum();
	mTexNo[0]        = block->getTexNo(0);
	mTexNo[1]        = block->getTexNo(1);
	mTexNo[2]        = block->getTexNo(2);
	mTexNo[3]        = block->getTexNo(3);
	mTevStage[0]     = *block->getTevStage(0);
	mTevStage[1]     = *block->getTevStage(1);
	mTevStage[2]     = *block->getTevStage(2);
	mTevStage[3]     = *block->getTevStage(3);
	mIndTevStage[0]  = *block->getIndTevStage(0);
	mIndTevStage[1]  = *block->getIndTevStage(1);
	mIndTevStage[2]  = *block->getIndTevStage(2);
	mIndTevStage[3]  = *block->getIndTevStage(3);
	mTevOrder[0]     = *block->getTevOrder(0);
	mTevOrder[1]     = *block->getTevOrder(1);
	mTevOrder[2]     = *block->getTevOrder(2);
	mTevOrder[3]     = *block->getTevOrder(3);
	mTevKColorSel[0] = block->getTevKColorSel(0);
	mTevKColorSel[1] = block->getTevKColorSel(1);
	mTevKColorSel[2] = block->getTevKColorSel(2);
	mTevKColorSel[3] = block->getTevKColorSel(3);
	mTevKAlphaSel[0] = block->getTevKAlphaSel(0);
	mTevKAlphaSel[1] = block->getTevKAlphaSel(1);
	mTevKAlphaSel[2] = block->getTevKAlphaSel(2);
	mTevKAlphaSel[3] = block->getTevKAlphaSel(3);
	for (u32 i = 0; i < 4; ++i)
		mTevColor[i] = *block->getTevColor(i);
	for (u32 i = 0; i < 4; ++i)
		mTevKColor[i] = *block->getTevKColor(i);
	for (u32 i = 0; i < 4; ++i)
		mTevSwapModeTable[i] = *block->getTevSwapModeTable(i);
}

void J3DTevBlock16::reset(J3DTevBlock* block)
{
	mTevStageNum = block->getTevStageNum();
	for (u32 i = 0; i < 8; ++i)
		mTexNo[i] = block->getTexNo(i);
	for (u32 i = 0; i < 16; ++i)
		mTevOrder[i] = *block->getTevOrder(i);
	for (u32 i = 0; i < 16; ++i) {
		mTevStage[i]    = *block->getTevStage(i);
		mIndTevStage[i] = *block->getIndTevStage(i);
	}
	for (u32 i = 0; i < 4; ++i)
		mTevColor[i] = *block->getTevColor(i);
	for (u32 i = 0; i < 4; ++i)
		mTevKColor[i] = *block->getTevKColor(i);
	for (u32 i = 0; i < 16; ++i)
		mTevKColorSel[i] = block->getTevKColorSel(i);
	for (u32 i = 0; i < 16; ++i)
		mTevKAlphaSel[i] = block->getTevKAlphaSel(i);
	for (u32 i = 0; i < 4; ++i)
		mTevSwapModeTable[i] = *block->getTevSwapModeTable(i);
}

void J3DIndBlockFull::reset(J3DIndBlock* block)
{
	mIndTexStageNum = block->getIndTexStageNum();
	for (u32 i = 0; i < 4; i++)
		mIndTexOrder[i] = *block->getIndTexOrder(i);
	for (u32 i = 0; i < 3; i++)
		mIndTexMtx[i] = *block->getIndTexMtx(i);
	for (u32 i = 0; i < 4; i++)
		mIndTexCoordScale[i] = *block->getIndTexCoordScale(i);
}

void J3DPEBlockFull::reset(J3DPEBlock* block)
{
	if (block->getFog() != NULL) {
		if (!mFog)
			mFog = new J3DFog;

		memcpy(mFog, block->getFog(), sizeof(*mFog));
		DCStoreRange(mFog, sizeof(*mFog));
	}

	// TODO: ton of stupid inlines missing here
	switch (block->getType()) {
	case 'PEOP':
		mAlphaComp.mRef0       = 0;
		mAlphaComp.mRef1       = 0;
		mAlphaComp.mAlphaCmpID = 0xE7;

		mBlend.mBlendMode = 0;
		mBlend.mSrcFactor = 1;
		mBlend.mDstFactor = 0;
		mBlend.mLogicOp   = 3;

		mZMode.mZModeID = 0x17;

		mZCompLoc = 1;
		break;
	case 'PEED':
		mAlphaComp.mRef0       = 0x80;
		mAlphaComp.mRef1       = 0XFF;
		mAlphaComp.mAlphaCmpID = 0XC3;

		mBlend.mBlendMode = 0;
		mBlend.mSrcFactor = 1;
		mBlend.mDstFactor = 0;
		mBlend.mLogicOp   = 3;

		mZMode.mZModeID = 0x17;

		mZCompLoc = 0;
		break;
	case 'PEXL':
		mAlphaComp.mRef0       = 0;
		mAlphaComp.mRef1       = 0;
		mAlphaComp.mAlphaCmpID = 0XE7;

		mBlend.mBlendMode = 1;
		mBlend.mSrcFactor = 4;
		mBlend.mDstFactor = 5;
		mBlend.mLogicOp   = 3;

		mZMode.mZModeID = 0x16;

		mZCompLoc = 1;
		break;
	case 'PEFL':
		mAlphaComp = *block->getAlphaComp();
		mBlend     = *block->getBlend();
		mZMode     = *block->getZMode();
		mZCompLoc  = block->getZCompLoc();
		break;
	}
}

#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTevBlocks.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <dolphin/gd.h>
#include <macros.h>

#pragma opt_strength_reduction off

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

s32 J3DTevBlock1::countDLSize() { return 0x80; }
s32 J3DTevBlock2::countDLSize() { return 0x180; }
s32 J3DTevBlock4::countDLSize() { return 0x260; }
s32 J3DTevBlock16::countDLSize() { return 0x580; }

// TODO: send to some header? Doesn't look like it should be public
extern void loadTexNo(u32, const u16&);

inline void loadTexCoordScale(GXTexCoordID coord,
                              const J3DTexCoordScaleInfo& info)
{
	J3DGDSetTexCoordScale2(coord, info.field_0x00, info.field_0x04 == 1, 0,
	                       info.field_0x02, info.field_0x06 == 1, 0);
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

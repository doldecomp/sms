#include <MarioUtil/PacketUtil.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <JSystem/J3D/J3DGraphBase/Blocks/J3DPEBlocks.hpp>
#include <dolphin/gd/GDTev.h>

static const GXColor sFogOffColor = { 0, 0, 0, 0 };

static void FifoSetChanMatColor(GXChannelID param_1, GXColor param_2)
{
	GXWGFifo.u8  = GX_CMD_LOAD_XF_REG;
	GXWGFifo.u16 = 0;
	GXWGFifo.u16 = 0x100C + (param_1 & 1);
	GXWGFifo.u32 = param_2.r << 24 | param_2.g << 16 | param_2.b << 8
	                 | param_2.a;
}

static void FifoSetTevColorS10(GXTevRegID param_1, GXColorS10 param_2)
{
	u32 regRA = BP_TEV_COLOR_REG_RA(param_2.r & 0x7FF, param_2.a & 0x7FF, 0,
	                                0xE0 + param_1 * 2);
	u32 regBG = BP_TEV_COLOR_REG_BG(param_2.b & 0x7FF, param_2.g & 0x7FF, 0,
	                                0xE1 + param_1 * 2);
	GXWGFifo.u8  = GX_CMD_LOAD_BP_REG;
	GXWGFifo.u32 = regRA;
	GXWGFifo.u8  = GX_CMD_LOAD_BP_REG;
	GXWGFifo.u32 = regBG;
	GXWGFifo.u8  = GX_CMD_LOAD_BP_REG;
	GXWGFifo.u32 = regBG;
	GXWGFifo.u8  = GX_CMD_LOAD_BP_REG;
	GXWGFifo.u32 = regBG;
}

static void FifoSetTevKColor(GXTevKColorID param_1, GXColor param_2)
{
	u32 regRA = BP_TEV_COLOR_REG_RA(param_2.r, param_2.a, 1,
	                                0xE0 + param_1 * 2);
	u32 regBG = BP_TEV_COLOR_REG_BG(param_2.b, param_2.g, 1,
	                                0xE1 + param_1 * 2);
	GXWGFifo.u8  = GX_CMD_LOAD_BP_REG;
	GXWGFifo.u32 = regRA;
	GXWGFifo.u8  = GX_CMD_LOAD_BP_REG;
	GXWGFifo.u32 = regBG;
}

static void FifoSetFogRangeAdj(u8 param_1, u16 param_2,
                               GXFogAdjTable* param_3)
{
	if (param_1) {
		for (int i = 0; i < 10; i += 2) {
			u32 reg = (0xE9 + (i / 2)) << 24 | param_3->r[i + 1] << 12
			          | param_3->r[i];
			GXWGFifo.u8  = GX_CMD_LOAD_BP_REG;
			GXWGFifo.u32 = reg;
		}
	}

	u32 reg      = 0xE8 << 24 | (param_2 + 342) | param_1 << 10;
	GXWGFifo.u8  = GX_CMD_LOAD_BP_REG;
	GXWGFifo.u32 = reg;
}

static void FifoSetFog(GXFogType, float, float, float, float, GXColor) { }

static void SetFogBase(const J3DFogInfo* param_1)
{
	FifoSetFog((GXFogType)param_1->mType, param_1->mStartZ, param_1->mEndZ,
	           param_1->mNearZ, param_1->mFarZ, param_1->mColor);
	FifoSetFogRangeAdj(param_1->mAdjEnable, param_1->mCenter,
	                   (GXFogAdjTable*)param_1->mFogAdjTable);
}

// fabricated
struct PacketUserData_MatColor {
	u32 unk0;
	GXChannelID unk4;
	const GXColor* unk8;
};

struct PacketUserData_OneTevColor {
	u32 unk0;
	GXTevRegID unk4;
	const GXColorS10* unk8;
};

struct PacketUserData_TwoTevColor {
	u32 unk0;
	GXTevRegID unk4;
	GXTevRegID unk8;
	const GXColorS10* unkC;
	const GXColorS10* unk10;
};

struct PacketUserData_ThreeTevColor {
	u32 unk0;
	GXTevRegID unk4;
	GXTevRegID unk8;
	GXTevRegID unkC;
	const GXColorS10* unk10;
	const GXColorS10* unk14;
	const GXColorS10* unk18;
};

struct PacketUserData_Fog {
	u32 unk0;
	J3DFog* unk4;
};

struct PacketUserData_OneTevKColor {
	u32 unk0;
	GXTevKColorID unk4;
	const GXColor* unk8;
};

struct PacketUserData_TwoTevKColor {
	u32 unk0;
	GXTevKColorID unk4;
	GXTevKColorID unk8;
	const GXColor* unkC;
	const GXColor* unk10;
};

struct PacketUserData_OneTevKColorAndFog {
	u32 unk0;
	u32 unk4;
	GXTevKColorID unk8;
	const GXColor* unkC;
	u32 unk10;
	J3DFog* unk14;
};

struct PacketUserData_OneTevColorAndOneTevKColor {
	u32 unk0;
	GXTevRegID unk4;
	const GXColorS10* unk8;
	const GXColor* unkC;
};

struct PacketUserData_TwoTevColorAndOneTevKColor {
	u32 unk0;
	GXTevRegID unk4;
	GXTevRegID unk8;
	const GXColorS10* unkC;
	const GXColorS10* unk10;
	const GXColor* unk14;
};

static bool ShapePacketCallBackFunc(J3DCallBackPacket* param_1, int param_2)
{
	u32* data = (u32*)param_1->getUserArea();
	if (param_2 == 0) {
		switch (data[0]) {
		case 0: {
			PacketUserData_MatColor* p = (PacketUserData_MatColor*)data;
			FifoSetChanMatColor(p->unk4, *p->unk8);
			break;
		}
		case 1: {
			PacketUserData_OneTevColor* p = (PacketUserData_OneTevColor*)data;
			FifoSetTevColorS10(p->unk4, *p->unk8);
			break;
		}
		case 2: {
			PacketUserData_TwoTevColor* p = (PacketUserData_TwoTevColor*)data;
			FifoSetTevColorS10(p->unk4, *p->unkC);
			FifoSetTevColorS10(p->unk8, *p->unk10);
			break;
		}
		case 3: {
			PacketUserData_ThreeTevColor* p = (PacketUserData_ThreeTevColor*)data;
			FifoSetTevColorS10(p->unk4, *p->unk10);
			FifoSetTevColorS10(p->unk8, *p->unk14);
			FifoSetTevColorS10(p->unkC, *p->unk18);
			break;
		}
		case 4:
			GXCallDisplayList((void*)data[1], data[2]);
			break;
		case 5: {
			PacketUserData_Fog* p = (PacketUserData_Fog*)data;
			SetFogBase(p->unk4);
			break;
		}
		case 6: {
			PacketUserData_OneTevKColor* p = (PacketUserData_OneTevKColor*)data;
			FifoSetTevKColor(p->unk4, *p->unk8);
			break;
		}
		case 7: {
			PacketUserData_TwoTevKColor* p = (PacketUserData_TwoTevKColor*)data;
			FifoSetTevKColor(p->unk4, *p->unkC);
			FifoSetTevKColor(p->unk8, *p->unk10);
			break;
		}
		case 8: {
			PacketUserData_OneTevKColorAndFog* p
			    = (PacketUserData_OneTevKColorAndFog*)data;
			FifoSetTevKColor(p->unk8, *p->unkC);
			SetFogBase(p->unk14);
			break;
		}
		case 9: {
			PacketUserData_OneTevColorAndOneTevKColor* p
			    = (PacketUserData_OneTevColorAndOneTevKColor*)data;
			FifoSetTevColorS10(p->unk4, *p->unk8);
			FifoSetTevKColor(GX_KCOLOR0, *p->unkC);
			break;
		}
		case 10: {
			PacketUserData_TwoTevColorAndOneTevKColor* p
			    = (PacketUserData_TwoTevColorAndOneTevKColor*)data;
			FifoSetTevColorS10(p->unk4, *p->unkC);
			FifoSetTevColorS10(p->unk8, *p->unk10);
			FifoSetTevKColor(GX_KCOLOR0, *p->unk14);
			break;
		}
		}
	} else if (param_2 == 1) {
		switch (data[0]) {
		case 5:
		case 8:
			FifoSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f,
			           sFogOffColor);
		}
	}
	return true;
}

static J3DShapePacket* InitPacket_Sub(J3DModel* model, u16 mat_idx)
{
	J3DMaterial* mat = model->getModelData()->getMaterialNodePointer(mat_idx);
	return model->getShapePacket(mat->getShape()->getIndex());
}

void SMS_InitPacket_MatColor(J3DModel* param_1, u16 param_2,
                             GXChannelID param_3, const GXColor* param_4)
{
	J3DShapePacket* packet = InitPacket_Sub(param_1, param_2);

	PacketUserData_MatColor* userData = new PacketUserData_MatColor;

	userData->unk0 = 0;
	userData->unk4 = param_3;
	userData->unk8 = param_4;

	packet->setUserArea((u32)userData);
	packet->setCallback((J3DCallBackPacket::CallbackT)&ShapePacketCallBackFunc);
}

void SMS_InitPacket_OneTevColor(J3DModel* param_1, u16 param_2,
                                GXTevRegID param_3, const GXColorS10* param_4)
{
	J3DShapePacket* packet = InitPacket_Sub(param_1, param_2);

	PacketUserData_OneTevColor* userData = new PacketUserData_OneTevColor;

	userData->unk0 = 1;
	userData->unk4 = param_3;
	userData->unk8 = param_4;

	packet->setUserArea((u32)userData);
	packet->setCallback((J3DCallBackPacket::CallbackT)&ShapePacketCallBackFunc);
}

void SMS_InitPacket_TwoTevColor(J3DModel* param_1, u16 param_2,
                                GXTevRegID param_3, const GXColorS10* param_4,
                                GXTevRegID param_5, const GXColorS10* param_6)
{
	J3DShapePacket* packet = InitPacket_Sub(param_1, param_2);

	PacketUserData_TwoTevColor* userData = new PacketUserData_TwoTevColor;

	userData->unk0  = 2;
	userData->unk4  = param_3;
	userData->unkC  = param_4;
	userData->unk8  = param_5;
	userData->unk10 = param_6;

	packet->setUserArea((u32)userData);
	packet->setCallback((J3DCallBackPacket::CallbackT)&ShapePacketCallBackFunc);
}

void SMS_InitPacket_ThreeTevColor(J3DModel* param_1, u16 param_2,
                                  GXTevRegID param_3, const GXColorS10* param_4,
                                  GXTevRegID param_5, const GXColorS10* param_6,
                                  GXTevRegID param_7, const GXColorS10* param_8)
{
	J3DShapePacket* packet = InitPacket_Sub(param_1, param_2);

	PacketUserData_ThreeTevColor* userData = new PacketUserData_ThreeTevColor;

	userData->unk0  = 3;
	userData->unk4  = param_3;
	userData->unk10 = param_4;
	userData->unk8  = param_5;
	userData->unk14 = param_6;
	userData->unkC  = param_7;
	userData->unk18 = param_8;

	packet->setUserArea((u32)userData);
	packet->setCallback((J3DCallBackPacket::CallbackT)&ShapePacketCallBackFunc);
}

void SMS_InitPacket_Fog(J3DModel* param_1, u16 param_2)
{
	J3DMaterial* material
	    = param_1->getModelData()->getMaterialNodePointer(param_2);
	J3DPEBlock* peBlock = material->getPEBlock();
	J3DShapePacket* packet = InitPacket_Sub(param_1, param_2);
	J3DFog* fog = peBlock->getFog();

	PacketUserData_Fog* userData = new PacketUserData_Fog;
	userData->unk0               = 5;
	userData->unk4               = fog;

	packet->setUserArea((u32)userData);
	packet->setCallback((J3DCallBackPacket::CallbackT)&ShapePacketCallBackFunc);
}

void SMS_InitPacket_OneTevKColor(J3DModel* param_1, u16 param_2,
                                 GXTevKColorID param_3, const GXColor* param_4)
{
	J3DShapePacket* packet = InitPacket_Sub(param_1, param_2);

	PacketUserData_OneTevKColor* userData = new PacketUserData_OneTevKColor;

	userData->unk0 = 6;
	userData->unk4 = param_3;
	userData->unk8 = param_4;

	packet->setUserArea((u32)userData);
	packet->setCallback((J3DCallBackPacket::CallbackT)&ShapePacketCallBackFunc);
}

void SMS_InitPacket_TwoTevKColor(J3DModel* param_1, u16 param_2,
                                 GXTevKColorID param_3, const GXColor* param_4,
                                 GXTevKColorID param_5, const GXColor* param_6)
{
	J3DShapePacket* packet = InitPacket_Sub(param_1, param_2);

	PacketUserData_TwoTevKColor* userData = new PacketUserData_TwoTevKColor;

	userData->unk0  = 7;
	userData->unk4  = param_3;
	userData->unkC  = param_4;
	userData->unk8  = param_5;
	userData->unk10 = param_6;

	packet->setUserArea((u32)userData);
	packet->setCallback((J3DCallBackPacket::CallbackT)&ShapePacketCallBackFunc);
}

void SMS_InitPacket_OneTevKColorAndFog(J3DModel* param_1, u16 param_2,
                                       GXTevKColorID param_3,
                                       const GXColor* param_4)
{
	J3DShapePacket* packet = InitPacket_Sub(param_1, param_2);

	PacketUserData_OneTevKColorAndFog* userData
	    = new PacketUserData_OneTevKColorAndFog;

	userData->unk0 = 8;
	userData->unk4 = 6;
	userData->unk8 = param_3;

	if (param_4 != nullptr) {
		userData->unkC = param_4;
	} else {
		userData->unkC = &param_1->getModelData()
		                      ->getMaterialNodePointer(param_2)
		                      ->getTevBlock()
		                      ->getTevKColor(param_3)
		                      ->color;
	}

	J3DFog* fog = param_1->getModelData()
	                  ->getMaterialNodePointer(param_2)
	                  ->getPEBlock()
	                  ->getFog();

	userData->unk10 = 5;
	userData->unk14 = fog;

	packet->setUserArea((u32)userData);
	packet->setCallback((J3DCallBackPacket::CallbackT)&ShapePacketCallBackFunc);
}

void SMS_InitPacket_OneTevColorAndOneTevKColor(J3DModel* param_1, u16 param_2,
                                               GXTevRegID param_3,
                                               const GXColorS10* param_4,
                                               const GXColor* param_5)
{
	J3DShapePacket* packet = InitPacket_Sub(param_1, param_2);

	PacketUserData_OneTevColorAndOneTevKColor* userData
	    = new PacketUserData_OneTevColorAndOneTevKColor;

	userData->unk0 = 9;
	userData->unk4 = param_3;
	userData->unk8 = param_4;
	userData->unkC = param_5;

	packet->setUserArea((u32)userData);
	packet->setCallback((J3DCallBackPacket::CallbackT)&ShapePacketCallBackFunc);
}

void SMS_InitPacket_TwoTevColorAndOneTevKColor(J3DModel* param_1, u16 param_2,
                                               GXTevRegID param_3,
                                               const GXColorS10* param_4,
                                               GXTevRegID param_5,
                                               const GXColorS10* param_6,
                                               const GXColor* param_7)
{
	J3DShapePacket* packet = InitPacket_Sub(param_1, param_2);

	PacketUserData_TwoTevColorAndOneTevKColor* userData
	    = new PacketUserData_TwoTevColorAndOneTevKColor;

	userData->unk0  = 10;
	userData->unk4  = param_3;
	userData->unkC  = param_4;
	userData->unk8  = param_5;
	userData->unk10 = param_6;
	userData->unk14 = param_7;

	packet->setUserArea((u32)userData);
	packet->setCallback((J3DCallBackPacket::CallbackT)&ShapePacketCallBackFunc);
}

void SMS_HideAllShapePacket(J3DModel* model)
{
	u16 mats = model->getModelData()->getMaterialNum();
	for (u16 i = 0; i < mats; ++i)
		model->getShapePacket(i)->hide();
}

void SMS_ShowAllShapePacket(J3DModel* model)
{
	u16 mats = model->getModelData()->getMaterialNum();
	for (u16 i = 0; i < mats; ++i)
		model->getShapePacket(i)->show();
}

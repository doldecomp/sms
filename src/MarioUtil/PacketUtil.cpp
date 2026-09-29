#include <MarioUtil/PacketUtil.hpp>
#include <dolphin/gd.h>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <JSystem/J3D/J3DGraphBase/Blocks/J3DPEBlocks.hpp>

static void FifoSetChanMatColor(GXChannelID channel, GXColor color)
{
	GXCmd1u8(GX_CMD_LOAD_XF_REG);
	GXCmd1u16(0);
	GXCmd1u16(XF_REG_MATERIAL0_ID + (channel & 1));
	GXCmd1u32(color.r << 24 | color.g << 16 | color.b << 8 | color.a);
}

static void FifoSetTevColorS10(GXTevRegID id, GXColorS10 color)
{
	u32 regRA = BP_TEV_COLOR_REG_RA(color.r & 0x7FF, color.a & 0x7FF, 0,
	                                0xE0 + id * 2);
	u32 regBG = BP_TEV_COLOR_REG_BG(color.b & 0x7FF, color.g & 0x7FF, 0,
	                                0xE1 + id * 2);
	GXCmd1u8(GX_CMD_LOAD_BP_REG);
	GXCmd1u32(regRA);
	GXCmd1u8(GX_CMD_LOAD_BP_REG);
	GXCmd1u32(regBG);
	GXCmd1u8(GX_CMD_LOAD_BP_REG);
	GXCmd1u32(regBG);
	GXCmd1u8(GX_CMD_LOAD_BP_REG);
	GXCmd1u32(regBG);
}

static void FifoSetTevKColor(GXTevKColorID id, GXColor color)
{
	u32 regRA = BP_TEV_COLOR_REG_RA(color.r, color.a, 1, 0xE0 + id * 2);
	u32 regBG = BP_TEV_COLOR_REG_BG(color.b, color.g, 1, 0xE1 + id * 2);
	GXCmd1u8(GX_CMD_LOAD_BP_REG);
	GXCmd1u32(regRA);
	GXCmd1u8(GX_CMD_LOAD_BP_REG);
	GXCmd1u32(regBG);
}

static void FifoSetFogRangeAdj(u8 enable, u16 center, GXFogAdjTable* table)
{
	if (enable) {
		for (int i = 0; i < 10; i += 2) {
			u32 reg
			    = (0xE9 + (i / 2)) << 24 | table->r[i + 1] << 12 | table->r[i];
			GXCmd1u8(GX_CMD_LOAD_BP_REG);
			GXCmd1u32(reg);
		}
	}
	u32 reg = 0xE8 << 24 | (center + 342) | enable << 10;
	GXCmd1u8(GX_CMD_LOAD_BP_REG);
	GXCmd1u32(reg);
}

static void FifoSetFog(GXFogType type, float startz, float endz, float nearz,
                       float farz, GXColor color)
{
	float A;
	float B;
	float B_mant;
	float C;
	float A_f;
	u32 b_expn;
	u32 b_m;
	u32 a_hex;
	u32 c_hex;

	if ((farz == nearz) || (endz == startz)) {
		A = 0.0f;
		B = 0.5f;
		C = 0.0f;
	} else {
		A = (farz * nearz) / ((farz - nearz) * (endz - startz));
		B = farz / (farz - nearz);
		C = startz / (endz - startz);
	}

	B_mant = B;
	b_expn = 1;
	while (B_mant > 1.0) {
		B_mant *= 0.5f;
		b_expn++;
	}
	while (B_mant > 0.0f && B_mant < 0.5) {
		B_mant *= 2.0f;
		b_expn--;
	}

	A_f = A / (1 << b_expn);
	b_m = (u32)(8388638.0f * B_mant);

	a_hex = *(u32*)&A_f;
	c_hex = *(u32*)&C;

	GXCmd1u8(GX_CMD_LOAD_BP_REG);
	GXCmd1u32(BP_FOG_UNK0(a_hex >> 12, 0xEE));
	GXCmd1u8(GX_CMD_LOAD_BP_REG);
	GXCmd1u32(BP_FOG_UNK1(b_m, 0xEF));
	GXCmd1u8(GX_CMD_LOAD_BP_REG);
	GXCmd1u32(BP_FOG_UNK2(b_expn, 0xF0));
	GXCmd1u8(GX_CMD_LOAD_BP_REG);
	GXCmd1u32(BP_FOG_UNK3(c_hex >> 12, 0, type, 0xF1));
	u32 fogclr = BP_FOG_COLOR(color.r, color.g, color.b, 0xF2);
	GXCmd1u8(GX_CMD_LOAD_BP_REG);
	GXCmd1u32(fogclr);
}

static void SetFogBase(const J3DFogInfo* fog)
{
	FifoSetFog((GXFogType)fog->mType, fog->mStartZ, fog->mEndZ, fog->mNearZ,
	           fog->mFarZ, fog->mColor);
	FifoSetFogRangeAdj(fog->mAdjEnable, fog->mCenter,
	                   (GXFogAdjTable*)fog->mFogAdjTable);
}

// fabricated
struct PacketUserData_MatColor {
	u32 unk0;
	GXChannelID unk4;
	const GXColor* unk8;
};

// fabricated
struct PacketUserData_OneTevColor {
	u32 unk0;
	GXTevRegID unk4;
	const GXColorS10* unk8;
};

// fabricated
struct PacketUserData_TwoTevColor {
	u32 unk0;
	GXTevRegID unk4;
	GXTevRegID unk8;
	const GXColorS10* unkC;
	const GXColorS10* unk10;
};

// fabricated
struct PacketUserData_ThreeTevColor {
	u32 unk0;
	GXTevRegID unk4;
	GXTevRegID unk8;
	GXTevRegID unkC;
	const GXColorS10* unk10;
	const GXColorS10* unk14;
	const GXColorS10* unk18;
};

// fabricated
struct PacketUserData_CallDL {
	u32 unk0;
	u8* unk4;
	u32 unk8;
};

// fabricated
struct PacketUserData_Fog {
	u32 unk0;
	J3DFog* unk4;
};

// fabricated
struct PacketUserData_OneTevKColor {
	u32 unk0;
	GXTevKColorID unk4;
	const GXColor* unk8;
};

// fabricated
struct PacketUserData_TwoTevKColor {
	u32 unk0;
	GXTevKColorID unk4;
	GXTevKColorID unk8;
	const GXColor* unkC;
	const GXColor* unk10;
};

// fabricated
struct PacketUserData_OneTevKColorAndFog {
	u32 unk0;
	u32 unk4;
	GXTevKColorID unk8;
	const GXColor* unkC;
	u32 unk10;
	J3DFog* unk14;
};

// fabricated
struct PacketUserData_OneTevColorAndOneTevKColor {
	u32 unk0;
	GXTevRegID unk4;
	const GXColorS10* unk8;
	const GXColor* unkC;
};

// fabricated
struct PacketUserData_TwoTevColorAndOneTevKColor {
	u32 unk0;
	GXTevRegID unk4;
	GXTevRegID unk8;
	const GXColorS10* unkC;
	const GXColorS10* unk10;
	const GXColor* unk14;
};

static void ShapePacketCallBackFunc(J3DCallBackPacket* packet, int draw)
{
	void* data = (void*)packet->getUserArea();
	if (draw == 0) {
		switch (*(u32*)data) {
		case 0: {
			PacketUserData_MatColor* user = (PacketUserData_MatColor*)data;
			FifoSetChanMatColor(user->unk4, *user->unk8);
			break;
		}
		case 1: {
			PacketUserData_OneTevColor* user
			    = (PacketUserData_OneTevColor*)data;
			FifoSetTevColorS10(user->unk4, *user->unk8);
			break;
		}
		case 2: {
			PacketUserData_TwoTevColor* user
			    = (PacketUserData_TwoTevColor*)data;
			FifoSetTevColorS10(user->unk4, *user->unkC);
			FifoSetTevColorS10(user->unk8, *user->unk10);
			break;
		}
		case 3: {
			PacketUserData_ThreeTevColor* user
			    = (PacketUserData_ThreeTevColor*)data;
			FifoSetTevColorS10(user->unk4, *user->unk10);
			FifoSetTevColorS10(user->unk8, *user->unk14);
			FifoSetTevColorS10(user->unkC, *user->unk18);
			break;
		}
		case 4: {
			PacketUserData_CallDL* user = (PacketUserData_CallDL*)data;
			GXCallDisplayList(user->unk4, user->unk8);
			break;
		}
		case 5: {
			PacketUserData_Fog* user = (PacketUserData_Fog*)data;
			SetFogBase(user->unk4);
			break;
		}
		case 6: {
			PacketUserData_OneTevKColor* user
			    = (PacketUserData_OneTevKColor*)data;
			FifoSetTevKColor(user->unk4, *user->unk8);
			break;
		}
		case 7: {
			PacketUserData_TwoTevKColor* user
			    = (PacketUserData_TwoTevKColor*)data;
			FifoSetTevKColor(user->unk4, *user->unkC);
			FifoSetTevKColor(user->unk8, *user->unk10);
			break;
		}
		case 8: {
			PacketUserData_OneTevKColorAndFog* user
			    = (PacketUserData_OneTevKColorAndFog*)data;
			FifoSetTevKColor(user->unk8, *user->unkC);
			SetFogBase(user->unk14);
			break;
		}
		case 9: {
			PacketUserData_OneTevColorAndOneTevKColor* user
			    = (PacketUserData_OneTevColorAndOneTevKColor*)data;
			FifoSetTevColorS10(user->unk4, *user->unk8);
			FifoSetTevKColor(GX_KCOLOR0, *user->unkC);
			break;
		}
		case 10: {
			PacketUserData_TwoTevColorAndOneTevKColor* user
			    = (PacketUserData_TwoTevColorAndOneTevKColor*)data;
			FifoSetTevColorS10(user->unk4, *user->unkC);
			FifoSetTevColorS10(user->unk8, *user->unk10);
			FifoSetTevKColor(GX_KCOLOR0, *user->unk14);
			break;
		}
		}
	} else if (draw == 1) {
		switch (*(u32*)data) {
		case 5:
		case 8: {
			static const GXColor sFogOffColor = { 0, 0, 0, 0 };
			FifoSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, sFogOffColor);
			break;
		}
		}
	}
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

	packet->setUserArea((uintptr_t)userData);
	packet->setCallback(&ShapePacketCallBackFunc);
}

void SMS_InitPacket_OneTevColor(J3DModel* param_1, u16 param_2,
                                GXTevRegID param_3, const GXColorS10* param_4)
{
	J3DShapePacket* packet = InitPacket_Sub(param_1, param_2);

	PacketUserData_OneTevColor* userData = new PacketUserData_OneTevColor;

	userData->unk0 = 1;
	userData->unk4 = param_3;
	userData->unk8 = param_4;

	packet->setUserArea((uintptr_t)userData);
	packet->setCallback(&ShapePacketCallBackFunc);
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

	packet->setUserArea((uintptr_t)userData);
	packet->setCallback(&ShapePacketCallBackFunc);
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

	packet->setUserArea((uintptr_t)userData);
	packet->setCallback(&ShapePacketCallBackFunc);
}

void SMS_InitPacket_CallDL(J3DModel* param_1, u16 param_2, u8* param_3,
                           u32 param_4)
{
	J3DShapePacket* packet = InitPacket_Sub(param_1, param_2);

	PacketUserData_CallDL* userData = new PacketUserData_CallDL;

	userData->unk0 = 4;
	userData->unk4 = param_3;
	userData->unk8 = param_4;

	packet->setUserArea((uintptr_t)userData);
	packet->setCallback(&ShapePacketCallBackFunc);
}

void SMS_InitPacket_Fog(J3DModel* param_1, u16 param_2)
{
	J3DPEBlock* peBlock = param_1->getModelData()
	                          ->getMaterialNodePointer(param_2)
	                          ->getPEBlock();
	J3DShapePacket* packet = InitPacket_Sub(param_1, param_2);

	J3DFog* fog = peBlock->getFog();

	PacketUserData_Fog* userData = new PacketUserData_Fog;
	userData->unk0               = 5;
	userData->unk4               = fog;

	packet->setUserArea((uintptr_t)userData);
	packet->setCallback(&ShapePacketCallBackFunc);
}

void SMS_InitPacket_OneTevKColor(J3DModel* param_1, u16 param_2,
                                 GXTevKColorID param_3, const GXColor* param_4)
{
	J3DShapePacket* packet = InitPacket_Sub(param_1, param_2);

	PacketUserData_OneTevKColor* userData = new PacketUserData_OneTevKColor;

	userData->unk0 = 6;
	userData->unk4 = param_3;
	userData->unk8 = param_4;

	packet->setUserArea((uintptr_t)userData);
	packet->setCallback(&ShapePacketCallBackFunc);
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

	packet->setUserArea((uintptr_t)userData);
	packet->setCallback(&ShapePacketCallBackFunc);
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

	packet->setUserArea((uintptr_t)userData);
	packet->setCallback(&ShapePacketCallBackFunc);
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

	packet->setUserArea((uintptr_t)userData);
	packet->setCallback(&ShapePacketCallBackFunc);
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

	packet->setUserArea((uintptr_t)userData);
	packet->setCallback(&ShapePacketCallBackFunc);
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

#include <MarioUtil/PacketUtil.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <JSystem/J3D/J3DGraphBase/Blocks/J3DPEBlocks.hpp>

// fabricated: the per-packet user areas the SMS_InitPacket_* helpers build
// and ShapePacketCallBackFunc reads back. The first word is the packet type.
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

struct PacketUserData_DisplayList {
	u32 unk0;
	u8* unk4;
	u32 unk8;
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

// FIFO-only equivalents of GXSetChanMatColor / GXSetTevColorS10 /
// GXSetTevKColor: the same register encodings, written straight into the
// graphics FIFO so they can live inside a display list, and without touching
// the GX register cache.
static void FifoSetChanMatColor(GXChannelID chan, GXColor color)
{
	GXWGFifo.u8  = 0x10;
	GXWGFifo.u16 = 0;
	GXWGFifo.u16 = 0x100C + (chan & 1);
	GXWGFifo.u32 = color.r << 24 | color.g << 16 | color.b << 8 | color.a;
}

static void FifoSetTevColorS10(GXTevRegID id, GXColorS10 color)
{
	u32 regRA = (color.r & 0x7FF) | (color.a & 0x7FF) << 12
	            | (224 + id * 2) << 24;
	u32 regBG = (color.b & 0x7FF) | (color.g & 0x7FF) << 12
	            | (225 + id * 2) << 24;

	GXWGFifo.u8  = 0x61;
	GXWGFifo.u32 = regRA;
	GXWGFifo.u8  = 0x61;
	GXWGFifo.u32 = regBG;
	GXWGFifo.u8  = 0x61;
	GXWGFifo.u32 = regBG;
	GXWGFifo.u8  = 0x61;
	GXWGFifo.u32 = regBG;
}

static void FifoSetTevKColor(GXTevKColorID id, GXColor color)
{
	u32 regRA = color.r | color.a << 12 | 8 << 20 | (224 + id * 2) << 24;
	u32 regBG = color.b | color.g << 12 | 8 << 20 | (225 + id * 2) << 24;

	GXWGFifo.u8  = 0x61;
	GXWGFifo.u32 = regRA;
	GXWGFifo.u8  = 0x61;
	GXWGFifo.u32 = regBG;
}

// Local re-implementations of GXSetFogRangeAdj/GXSetFog that write straight
// into the graphics FIFO instead of going through the GX register cache, so a
// display list can carry the fog setup. The bit layouts are GXSetFog's; the
// masking SET_REG_FIELD does is dropped because every field is already in
// range, and the range-adjust loop keeps `i / 2` signed (retail still emits the
// srawi/addze pair in every unrolled copy).
static void FifoSetFogRangeAdj(u8 enable, u16 center, GXFogAdjTable* table)
{
	if (enable) {
		for (int i = 0; i < 10; i += 2) {
			u32 range_adj = ((i / 2) + 0xE9) << 24 | table->r[i + 1] << 12
			                | table->r[i];
			GXWGFifo.u8  = 0x61;
			GXWGFifo.u32 = range_adj;
		}
	}

	u32 range_c  = (center + 342) | 0xE8000000 | enable << 10;
	GXWGFifo.u8  = 0x61;
	GXWGFifo.u32 = range_c;
}

// TODO: every instruction matches; frame 0x58 vs retail's 0x50. Every
// referenced slot is exactly 8 high, so it is 8 bytes of low region. `c_hex` is
// required (it is retail's r27 across the __cvt_fp2unsigned call) and costs
// exactly that 8; dropping it lands the frame but costs two instructions.
static void FifoSetFog(GXFogType type, f32 startz, f32 endz, f32 nearz,
                       f32 farz, GXColor color)
{
	f32 A;
	f32 B;
	f32 C;
	f32 B_mant;
	u32 B_expn;
	f32 a;
	u32 c_hex;
	u32 fog0;
	u32 fog1;
	u32 fog2;
	u32 fog3;
	u32 fogclr;

	if (farz == nearz || endz == startz) {
		A = 0.0f;
		B = 0.5f;
		C = 0.0f;
	} else {
		A = (farz * nearz) / ((farz - nearz) * (endz - startz));
		B = farz / (farz - nearz);
		C = startz / (endz - startz);
	}

	B_mant = B;
	B_expn = 1;
	while (B_mant > 1.0) {
		B_mant *= 0.5f;
		B_expn++;
	}
	while (B_mant > 0.0f && B_mant < 0.5) {
		B_mant *= 2.0f;
		B_expn--;
	}

	a     = A / (f32)(1 << B_expn);
	c_hex = *(u32*)&C;

	fog0         = (*(u32*)&a >> 12) | 0xEE000000;
	GXWGFifo.u8  = 0x61;
	GXWGFifo.u32 = fog0;

	fog1         = (u32)(8.388638e6f * B_mant) | 0xEF000000;
	GXWGFifo.u8  = 0x61;
	GXWGFifo.u32 = fog1;

	fog2         = B_expn | 0xF0000000;
	GXWGFifo.u8  = 0x61;
	GXWGFifo.u32 = fog2;

	fog3         = (type << 21) | (c_hex >> 12) | 0xF1000000;
	GXWGFifo.u8  = 0x61;
	GXWGFifo.u32 = fog3;

	fogclr       = color.b | (color.g << 8) | (color.r << 16) | 0xF2000000;
	GXWGFifo.u8  = 0x61;
	GXWGFifo.u32 = fogclr;
}

static void SetFogBase(const J3DFogInfo* fog)
{
	FifoSetFog((GXFogType)fog->mType, fog->mStartZ, fog->mEndZ, fog->mNearZ,
	           fog->mFarZ, fog->mColor);
	FifoSetFogRangeAdj(fog->mAdjEnable, fog->mCenter,
	                   (GXFogAdjTable*)fog->mFogAdjTable);
}

// TODO: frame 0xe8 vs retail's 0x178. All four inlined helpers' UNUSED sizes
// are byte-exact (0x48/0x60/0x58/0x174), so the bodies are right; the residue
// is 8 bytes of low region per inlined expansion (18 expansions = the 0x90
// delta), the header-round-15 depth-surcharge shape. The register numbering
// differences all follow from it.
//
// Dispatches the per-packet colour/fog override recorded by the
// SMS_InitPacket_* helpers. The user area's first word is the packet type; the
// jump table in .data has eleven entries, so types 0-10 all exist even though
// type 4 (a nested display list) has no SMS_InitPacket_* constructor here.
static BOOL ShapePacketCallBackFunc(J3DCallBackPacket* packet, int phase)
{
	static const GXColor sFogOffColor = { 0, 0, 0, 0 };

	void* userData = (void*)packet->getUserArea();

	if (phase == 0) {
		switch (*(u32*)userData) {
		case 0: {
			PacketUserData_MatColor* d = (PacketUserData_MatColor*)userData;
			FifoSetChanMatColor(d->unk4, *d->unk8);
			break;
		}

		case 1: {
			PacketUserData_OneTevColor* d
			    = (PacketUserData_OneTevColor*)userData;
			FifoSetTevColorS10(d->unk4, *d->unk8);
			break;
		}

		case 2: {
			PacketUserData_TwoTevColor* d
			    = (PacketUserData_TwoTevColor*)userData;
			FifoSetTevColorS10(d->unk4, *d->unkC);
			FifoSetTevColorS10(d->unk8, *d->unk10);
			break;
		}

		case 3: {
			PacketUserData_ThreeTevColor* d
			    = (PacketUserData_ThreeTevColor*)userData;
			FifoSetTevColorS10(d->unk4, *d->unk10);
			FifoSetTevColorS10(d->unk8, *d->unk14);
			FifoSetTevColorS10(d->unkC, *d->unk18);
			break;
		}

		case 4: {
			PacketUserData_DisplayList* d
			    = (PacketUserData_DisplayList*)userData;
			GXCallDisplayList(d->unk4, d->unk8);
			break;
		}

		case 5: {
			PacketUserData_Fog* d = (PacketUserData_Fog*)userData;
			SetFogBase(d->unk4);
			break;
		}

		case 6: {
			PacketUserData_OneTevKColor* d
			    = (PacketUserData_OneTevKColor*)userData;
			FifoSetTevKColor(d->unk4, *d->unk8);
			break;
		}

		case 7: {
			PacketUserData_TwoTevKColor* d
			    = (PacketUserData_TwoTevKColor*)userData;
			FifoSetTevKColor(d->unk4, *d->unkC);
			FifoSetTevKColor(d->unk8, *d->unk10);
			break;
		}

		case 8: {
			PacketUserData_OneTevKColorAndFog* d
			    = (PacketUserData_OneTevKColorAndFog*)userData;
			FifoSetTevKColor(d->unk8, *d->unkC);
			SetFogBase(d->unk14);
			break;
		}

		case 9: {
			PacketUserData_OneTevColorAndOneTevKColor* d
			    = (PacketUserData_OneTevColorAndOneTevKColor*)userData;
			FifoSetTevColorS10(d->unk4, *d->unk8);
			FifoSetTevKColor(GX_KCOLOR0, *d->unkC);
			break;
		}

		case 10: {
			PacketUserData_TwoTevColorAndOneTevKColor* d
			    = (PacketUserData_TwoTevColorAndOneTevKColor*)userData;
			FifoSetTevColorS10(d->unk4, *d->unkC);
			FifoSetTevColorS10(d->unk8, *d->unk10);
			FifoSetTevKColor(GX_KCOLOR0, *d->unk14);
			break;
		}
		}
	} else if (phase == 1) {
		switch (*(u32*)userData) {
		case 5:
		case 8:
			FifoSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, sFogOffColor);
			break;
		}
	}

	return TRUE;
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

	packet->setUserArea((u32)userData);
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

	packet->setUserArea((u32)userData);
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

	packet->setUserArea((u32)userData);
	packet->setCallback(&ShapePacketCallBackFunc);
}

void SMS_InitPacket_CallDL(J3DModel* param_1, u16 param_2, u8* param_3,
                          u32 param_4)
{
	J3DShapePacket* packet = InitPacket_Sub(param_1, param_2);

	PacketUserData_DisplayList* userData = new PacketUserData_DisplayList;

	userData->unk0 = 4;
	userData->unk4 = param_3;
	userData->unk8 = param_4;

	packet->setUserArea((u32)userData);
	packet->setCallback(&ShapePacketCallBackFunc);
}

// TODO: 93.1%. Retail loads the PE block's vtable before the shape index and
// its frame is 8 bytes bigger; evaluating the fog chain first or naming the
// material both make it worse (45.4% / 65.6%).
void SMS_InitPacket_Fog(J3DModel* param_1, u16 param_2)
{
	J3DShapePacket* packet = InitPacket_Sub(param_1, param_2);

	J3DFog* fog = param_1->getModelData()
	                  ->getMaterialNodePointer(param_2)
	                  ->getPEBlock()
	                  ->getFog();

	PacketUserData_Fog* userData = new PacketUserData_Fog;
	userData->unk0               = 5;
	userData->unk4               = fog;

	packet->setUserArea((u32)userData);
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

	packet->setUserArea((u32)userData);
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

	packet->setUserArea((u32)userData);
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

	packet->setUserArea((u32)userData);
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

	packet->setUserArea((u32)userData);
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

	packet->setUserArea((u32)userData);
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

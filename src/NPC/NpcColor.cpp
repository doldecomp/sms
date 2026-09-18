#include <NPC/NpcColor.hpp>

#include <MarioUtil/PacketUtil.hpp>
#include <M3DUtil/MActor.hpp>

static void InitChangeOneColor_Base(J3DModel* model, u16 material,
                                    GXTevRegID reg, const GXColorS10* color,
                                    const GXColor* konst)
{
	if (konst != nullptr)
		SMS_InitPacket_OneTevColorAndOneTevKColor(model, material, reg, color,
		                                        konst);
	else
		SMS_InitPacket_OneTevColor(model, material, reg, color);
}

static void InitChangeTwoColor_Base(J3DModel* model, u16 material,
                                    const GXColorS10* color1,
                                    const GXColorS10* color2,
                                    const GXColor* konst)
{
	if (konst != nullptr)
		SMS_InitPacket_TwoTevColorAndOneTevKColor(
		    model, material, GX_TEVREG1, color1, GX_TEVREG2, color2, konst);
	else
		SMS_InitPacket_TwoTevColor(model, material, GX_TEVREG1, color1,
		                           GX_TEVREG2, color2);
}

// Closed in batch 115. Every instruction had always matched and only the frame
// was wrong (0x40 against 0x38), bisected to the named `operator new` result in
// case 0, which reserves 8 bytes retail does not pay for. The 8 bytes come back
// one statement earlier: naming the `J3DModelData*` of the material-name lookup
// is worth **-8** here, so the `new` keeps its slot and the frame lands.
// (The same -8 is reached by `modelData->mMaterialName` raw with or without the
// named pointer, and by a named `JUTNameTab*` over the raw member; the raw
// member alone, with no named local, overshoots to 0x30, so the accessor pair
// plus one named pointer is the combination that fits.)
// Rejected before that, all leaving 0x40: `new GXColor` without parentheses,
// explicit `(GXColor*)operator new(sizeof(GXColor))`, a `void*` intermediate,
// a split `GXColor* matColor; matColor = new GXColor();`, `GXColor& c = *new
// GXColor()`, `register`, declaring the pointer at function scope, declaring
// all three pointers C-style at the top, casting each field store, spelling the
// two InitChange*_Base helpers out at all four call sites, and `param1->mModel`
// instead of `getModel()`.
void SMS_InitChangeNpcColor(const MActor* param1,
                            const TColorChangeInfo* param2, s16 param3,
                            const GXColor* param4)
{
	J3DModel* model         = param1->getModel();
	J3DModelData* modelData = model->getModelData();
	s32 matIdx = modelData->getMaterialName()->getIndex(param2->unk4);
	switch (param2->unk0) {
	case 0:
		if (param2->unk8 != nullptr) {
			GXColor* matColor = new GXColor();
			matColor->r       = param2->unk8[param3].r;
			matColor->g       = param2->unk8[param3].g;
			matColor->b       = param2->unk8[param3].b;
			matColor->a       = 0xff;
			SMS_InitPacket_MatColor(model, matIdx, GX_COLOR0, matColor);
		}
		break;
	case 1:
		if (param2->unk8 != nullptr) {
			const GXColorS10* tevColor = &param2->unk8[param3];
			InitChangeOneColor_Base(model, matIdx, GX_TEVREG0, tevColor,
			                        param4);
		}
		break;
	case 2:
		if (param2->unk8 != nullptr && param2->unkC != nullptr) {
			const GXColorS10* tevColor  = &param2->unkC[param3];
			const GXColorS10* tevColor2 = &param2->unk8[param3];
			InitChangeTwoColor_Base(model, matIdx, tevColor2, tevColor, param4);
		} else if (param2->unk8 != nullptr && param2->unkC == nullptr) {
			const GXColorS10* tevColor = &param2->unk8[param3];
			InitChangeOneColor_Base(model, matIdx, GX_TEVREG1, tevColor,
			                        param4);
		} else if (param2->unk8 == nullptr && param2->unkC != nullptr) {
			const GXColorS10* tevColor = &param2->unkC[param3];
			InitChangeOneColor_Base(model, matIdx, GX_TEVREG2, tevColor,
			                        param4);
		}
		break;
	}
}

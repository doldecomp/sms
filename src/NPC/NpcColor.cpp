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

// TODO: every instruction matches; the frame is 8 bytes too big (0x40 vs
// 0x38). Bisected to one statement: binding `operator new`'s result to a
// local in case 0. With the whole `new` removed the frame is exactly 0x38, so
// every other level in this function is already right, and with the `new` and
// no field stores at all it is already 0x40 -- the eight bytes are the result
// slot, not the colour copy. Retail calls the same `operator new(4)` and pays
// nothing for it. Rejected: `new GXColor` without parentheses, explicit
// `(GXColor*)operator new(sizeof(GXColor))`, a `void*` intermediate,
// `GXColor& c = *new GXColor()`, `register`, declaring the pointer at
// function scope, declaring all three pointers C-style at the top, casting
// each field store, spelling the two InitChange*_Base helpers out at all four
// call sites, and `param1->mModel` instead of `getModel()`.
void SMS_InitChangeNpcColor(const MActor* param1,
                            const TColorChangeInfo* param2, s16 param3,
                            const GXColor* param4)
{
	J3DModel* model = param1->getModel();
	s32 matIdx
	    = model->getModelData()->getMaterialName()->getIndex(param2->unk4);
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

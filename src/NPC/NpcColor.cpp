#include <NPC/NpcColor.hpp>

#include <MarioUtil/PacketUtil.hpp>
#include <M3DUtil/MActor.hpp>

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
			if (param4 != nullptr) {
				SMS_InitPacket_OneTevColorAndOneTevKColor(
				    model, matIdx, GX_TEVREG0, tevColor, param4);
			} else {
				SMS_InitPacket_OneTevColor(model, matIdx, GX_TEVREG0, tevColor);
			}
		}
		break;
	case 2:
		if (param2->unk8 != nullptr && param2->unkC != nullptr) {
			const GXColorS10* tevColor  = &param2->unkC[param3];
			const GXColorS10* tevColor2 = &param2->unk8[param3];
			if (param4 != nullptr) {
				SMS_InitPacket_TwoTevColorAndOneTevKColor(
				    model, matIdx, GX_TEVREG1, tevColor2, GX_TEVREG2, tevColor,
				    param4);
			} else {
				SMS_InitPacket_TwoTevColor(model, matIdx, GX_TEVREG1, tevColor2,
				                           GX_TEVREG2, tevColor);
			}
		} else if (param2->unk8 != nullptr && param2->unkC == nullptr) {
			const GXColorS10* tevColor = &param2->unk8[param3];
			if (param4 != nullptr) {
				SMS_InitPacket_OneTevColorAndOneTevKColor(
				    model, matIdx, GX_TEVREG1, tevColor, param4);
			} else {
				SMS_InitPacket_OneTevColor(model, matIdx, GX_TEVREG1, tevColor);
			}
		} else if (param2->unk8 == nullptr && param2->unkC != nullptr) {
			const GXColorS10* tevColor = &param2->unkC[param3];
			if (param4 != nullptr) {
				SMS_InitPacket_OneTevColorAndOneTevKColor(
				    model, matIdx, GX_TEVREG2, tevColor, param4);
			} else {
				SMS_InitPacket_OneTevColor(model, matIdx, GX_TEVREG2, tevColor);
			}
		}
		break;
	}
}

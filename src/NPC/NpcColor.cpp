#include <NPC/NpcColor.hpp>

#include <MarioUtil/PacketUtil.hpp>
#include <M3DUtil/MActor.hpp>

static void InitChangeOneColor_Base(J3DModel* model, u16 mat_idx,
                                    GXTevRegID reg, const GXColorS10* color,
                                    const GXColor* kcolor)
{
	if (kcolor != nullptr) {
		SMS_InitPacket_OneTevColorAndOneTevKColor(model, mat_idx, reg, color,
		                                          kcolor);
	} else {
		SMS_InitPacket_OneTevColor(model, mat_idx, reg, color);
	}
}

static void InitChangeTwoColor_Base(J3DModel* model, u16 mat_idx,
                                    const GXColorS10* color1,
                                    const GXColorS10* color2,
                                    const GXColor* kcolor)
{
	if (kcolor != nullptr) {
		SMS_InitPacket_TwoTevColorAndOneTevKColor(model, mat_idx, GX_TEVREG1,
		                                          color1, GX_TEVREG2, color2,
		                                          kcolor);
	} else {
		SMS_InitPacket_TwoTevColor(model, mat_idx, GX_TEVREG1, color1,
		                           GX_TEVREG2, color2);
	}
}

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
			InitChangeOneColor_Base(model, matIdx, GX_TEVREG0,
			                        &param2->unk8[param3], param4);
		}
		break;
	case 2:
		if (param2->unk8 != nullptr && param2->unkC != nullptr) {
			InitChangeTwoColor_Base(model, matIdx, &param2->unk8[param3],
			                        &param2->unkC[param3], param4);
		} else if (param2->unk8 != nullptr && param2->unkC == nullptr) {
			InitChangeOneColor_Base(model, matIdx, GX_TEVREG1,
			                        &param2->unk8[param3], param4);
		} else if (param2->unk8 == nullptr && param2->unkC != nullptr) {
			InitChangeOneColor_Base(model, matIdx, GX_TEVREG2,
			                        &param2->unkC[param3], param4);
		}
		break;
	}
}

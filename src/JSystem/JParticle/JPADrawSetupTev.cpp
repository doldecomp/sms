#include <JSystem/JParticle/JPADrawSetupTev.hpp>
#include <JSystem/JParticle/JPABaseShape.hpp>
#include <JSystem/JParticle/JPAExTexShape.hpp>
#include <dolphin/gx.h>

void JPADrawSetupTev::setupTev(JPABaseShape* param_1, JPAExTexShape* param_2)
{
	JPABaseShape::TevArgs* args = &param_1->unk48;

	unk1 = 1;
	unk0 = 1;
	unk2 = 0;
	GXSetTevColorIn(GX_TEVSTAGE0, args->unk0, args->unk4, args->unk8,
	                args->unkC);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
	                GX_TEVPREV);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
	                GX_TEVPREV);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
	if (param_2 != nullptr) {
		GXTevStageID stage = GX_TEVSTAGE1;
		GXTexCoordID coord = GX_TEXCOORD1;
		GXTexMapID map     = GX_TEXMAP1;
		switch (param_2->mIndTexMode) {
		case 1:
			GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD1, GX_TEXMAP1);
			GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
			GXSetIndTexMtx(GX_ITM_0, *param_2->getIndTexMtx(),
			               param_2->getExpScale());
			GXSetTevIndirect(GX_TEVSTAGE0, GX_INDTEXSTAGE0, GX_ITF_8,
			                 GX_ITB_STU, param_2->getIndTexMtxID(), GX_ITW_OFF,
			                 GX_ITW_OFF, 0, 0, GX_ITBA_OFF);
			coord = GX_TEXCOORD2;
			map   = GX_TEXMAP2;
			++unk2;
			++unk1;
			break;
		case 2:
			GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD1, GX_TEXMAP1);
			GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
			GXSetIndTexMtx(GX_ITM_0, *param_2->getIndTexMtx(),
			               param_2->getExpScale());
			GXSetTevIndirect(GX_TEVSTAGE1, GX_INDTEXSTAGE0, GX_ITF_8,
			                 GX_ITB_STU, param_2->getIndTexMtxID(), GX_ITW_OFF,
			                 GX_ITW_OFF, 0, 0, GX_ITBA_OFF);
			GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD2, GX_TEXMAP2,
			              GX_COLOR_NULL);
			GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_TEXC, GX_CC_CPREV,
			                GX_CC_TEXA);
			GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
			                GX_CA_APREV);
			GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
			                1, GX_TEVPREV);
			GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
			                1, GX_TEVPREV);
			stage = GX_TEVSTAGE2;
			coord = GX_TEXCOORD3;
			unk0  = unk0 + 1;
			map   = GX_TEXMAP3;
			unk1 += 2;
			unk2 += 1;
			break;
		}

		if ((param_2->unk28 & 1) != 0) {
			GXSetTevOrder(stage, coord, map, GX_COLOR_NULL);
			GXSetTevColorIn(stage, GX_CC_ZERO, GX_CC_TEXC, GX_CC_CPREV,
			                GX_CC_ZERO);
			GXSetTevAlphaIn(stage, GX_CA_ZERO, GX_CA_TEXA, GX_CA_APREV,
			                GX_CA_ZERO);
			GXSetTevColorOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
			                GX_TEVPREV);
			GXSetTevAlphaOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
			                GX_TEVPREV);
			++unk0;
			++unk1;
		}
	}
	GXSetNumTexGens(unk1);
	GXSetNumTevStages(unk0);
	GXSetNumIndStages(unk2);
}

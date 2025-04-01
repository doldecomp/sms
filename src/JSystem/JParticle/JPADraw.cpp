#include <JSystem/JParticle/JPADraw.hpp>
#include <JSystem/JParticle/JPABaseShape.hpp>
#include <JSystem/JParticle/JPAExtraShape.hpp>
#include <JSystem/JParticle/JPASweepShape.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <JSystem/JParticle/JPADrawSetupTev.hpp>
#include <JSystem/JParticle/JPAParticle.hpp>
#include <dolphin/gx.h>
#include <macros.h>

JPADrawVisitorContainer JPADraw::vc;
JPADrawClipBoard JPADraw::cb;

BOOL JPADraw::initialize(JPABaseEmitter* param_1, JPATextureResource* param_2)
{
	JPADrawContext::pcb = &cb;

	unk90.unk0  = param_1;
	unk90.unk4  = unk90.unk0->unk118->unk4;
	unk90.unk8  = unk90.unk0->unk118->unk8;
	unk90.unkC  = unk90.unk0->unk118->unkC;
	unk90.unk10 = unk90.unk0->unk118->unk10;
	unk90.unk14 = this;
	unk90.unk1C = param_2;
	unk90.unk20 = unk90.unk0->unk118->unk1C;
	unk90.unk18 = &unk90.unk0->unkF4;

	unkC2 = 0;
	unkB4 = 1.0f;

	if (unk90.unk4->unk83 & 1) {
		if (!(unk90.unk4->unk83 & 2))
			unkB8 = unk90.unk4->unk60;
	} else {
		unkB8 = (GXColor) { 0xff, 0xff, 0xff, 0xff };
	}

	if (unk90.unk4->unk84 & 1) {
		if (!(unk90.unk4->unk84 & 2))
			unkBC = unk90.unk4->unk64;
	} else {
		unkBC = (GXColor) { 0xff, 0xff, 0xff, 0xff };
	}

	unk8F = 0;
	unk8D = 0;
	unk8E = 0;
	unk8C = 0;
	unk8B = 0;
	unk8A = 0;
	unk89 = 0;
	unk88 = 0;

	for (int i = 0; i < ARRAY_COUNT(unk0); ++i)
		unk0[i] = nullptr;

	for (int i = 0; i < ARRAY_COUNT(unk4); ++i)
		unk4[i] = nullptr;

	for (int i = 0; i < ARRAY_COUNT(unk18); ++i)
		unk18[i] = nullptr;

	for (int i = 0; i < ARRAY_COUNT(unk24); ++i)
		unk24[i] = nullptr;

	for (int i = 0; i < ARRAY_COUNT(unk34); ++i)
		unk34[i] = nullptr;

	for (int i = 0; i < ARRAY_COUNT(unk70); ++i)
		unk70[i] = nullptr;

	for (int i = 0; i < ARRAY_COUNT(unk48); ++i)
		unk48[i] = nullptr;

	for (int i = 0; i < ARRAY_COUNT(unk80); ++i)
		unk80[i] = nullptr;

	JPADraw::JPADrawVisitorDefFlags local_28;

	local_28.unk0  = !unk90.unkC || (unk90.unkC->unk49 & 1);
	local_28.unk4  = (unk90.unk4->unk83 & 1) && (unk90.unk4->unk83 & 2);
	local_28.unk8  = (unk90.unk4->unk84 & 1) && (unk90.unk4->unk84 & 2);
	local_28.unkC  = unk90.unk4->unk69 == 5 || unk90.unk4->unk69 == 6;
	local_28.unk10 = unk90.unk4->unk69 == 0 || unk90.unk4->unk69 == 1;
	local_28.unk14 = unk90.unk8 && (unk90.unk8->unk78 & 1);

	setDrawExecVisitorsBeforeCB(local_28);
	unk4[unk89++]  = &vc.unk90;
	unk18[unk8A++] = &vc.unk90;
	unk34[unk8C++] = &vc.unk90;
	unk70[unk8E++] = &vc.unk90;
	setDrawExecVisitorsAfterCB(local_28);
	setDrawCalcVisitors(local_28);

	return true;
}

void JPADraw::draw(MtxPtr param_1)
{
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_F32, 0);
	GXSetCurrentMtx(0);
	GXSetNumChans(0);
	GXSetTevDirect(GX_TEVSTAGE0);
	GXSetTevDirect(GX_TEVSTAGE1);
	GXSetTevDirect(GX_TEVSTAGE2);
	GXSetCoPlanar(0);
	GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetBlendMode(JPABaseShape::stBlendMode[unk90.unk4->unk6D],
	               JPABaseShape::stBlendFactor[unk90.unk4->unk6E],
	               JPABaseShape::stBlendFactor[unk90.unk4->unk6F],
	               JPABaseShape::stLogicOp[unk90.unk4->unk70]);

	JPABaseEmitter* be = unk90.unk0;

	cb.unk98 = be->unk180;
	cb.unk99 = be->unk181;
	cb.unk9A = be->unk182;
	cb.unk9B = be->unk183;

	// TODO: fakematch? These are probably structs...
	be       = unk90.unk0;
	cb.unk9C = be->unk184;
	cb.unk9D = be->unk185;
	cb.unk9E = be->unk186;
	cb.unk9F = 0xff;

	// TODO: wut?!
	cb.unk34 = (Mtx44*)param_1;

	cb.unk0.setupTev(unk90.unk4, unk90.unk10);

	for (int i = 0; i < unk88; ++i)
		unk0[i]->exec(&unk90);

	if ((int)unk90.unk4->unk7A != 0)
		zDraw();

	unkC2 &= ~0x1;

	if ((unk90.unk4->unk7C >> 1 & 1) == 1UL && unk90.unkC != nullptr)
		drawChild();

	drawParticle();

	if ((unk90.unk4->unk7C >> 1 & 1) == 0UL && unk90.unkC != nullptr)
		drawChild();

	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
}

void JPADraw::calc()
{
	for (int i = 0; i < unk8B; ++i)
		unk24[i]->calc(&unk90);
}

void JPADraw::calcParticle(JPABaseParticle* particle)
{
	JPADrawParams* params = particle->getDrawParamPPtr();
	params->unk34 += params->unk36;
	for (int i = 0; i < unk8D; ++i)
		unk48[i]->calc(&unk90, particle);
}

void JPADraw::calcChild(JPABaseParticle* particle)
{
	JPADrawParams* params = particle->getDrawParamCPtr();
	params->unk34 += params->unk36;
	for (int i = 0; i < unk8F; ++i)
		unk80[i]->calc(&unk90, particle);
}

void JPADraw::initParticle(JPABaseParticle* particle)
{
	JPADrawParams* params = particle->getDrawParamCPtr();
	JPAEmitterInfo* info  = JPAGetEmitterInfoPtr();

	f32* v         = info->unk48[1];
	params->unk0.x = v[0];
	params->unk0.y = v[1];
	params->unk0.z = v[2];

	params->unk2C = unkB8;
	params->unk30 = unkBC;
	params->unk20 = 1.0f;

	s16 thing     = unk90.unk4->unk58;
	params->unk28 = (int)(unk90.unk0->getRandomF() * thing);

	if (unk90.unk8 != nullptr) {
		if ((int)unk90.unk8->unk7F != 0) {
			params->unk34
			    = unk90.unk8->unk60 * 32768.0f
			      + unk90.unk0->getRandomSF() * unk90.unk8->unk6C * 65536.0f;

			s16 sVar4;
			if (unk90.unk8->unk70 < unk90.unk0->getRandomRF()) {

				sVar4 = unk90.unk8->unk64
				        * (unk90.unk8->unk68 * unk90.unk0->getRandomRF() + 1.0f)
				        * 32768.0f;
			} else {
				sVar4 = -unk90.unk8->unk64
				        * (unk90.unk8->unk68 * unk90.unk0->getRandomRF() + 1.0f)
				        * 32768.0f;
			}
			params->unk36 = sVar4;
		} else {
			params->unk34 = 0;
			params->unk36 = 0;
		}

		f32 rnd
		    = unkB4 * ((unk90.unk0->getRandomRF()) * unk90.unk8->unk34 + 1.0f);
		params->unk0.x = rnd;
		params->unk14  = rnd;
		params->unk10  = rnd;

		params->unk24 = (unk90.unk0->getRandomRF()) * unk90.unk8->unk28 + 1.0f;
	} else {
		params->unk34 = 0;
		params->unk36 = 0;
		params->unkC = params->unk10 = params->unk14 = unkB4;

		params->unk24 = 1.0f;
	}
}

void JPADraw::initChild(JPABaseParticle* param_1, JPABaseParticle* param_2)
{
	JPADrawParams* params1 = param_1->getDrawParamPPtr();
	JPADrawParams* params2 = param_2->getDrawParamCPtr();

	params2->unk0 = params1->unk0;

	params2->unk20 = 1.0f;

	if (unk90.unkC->unk4E & 4) {
		f32 f = unk90.unkC->unk34;

		params2->unk2C.r = params1->unk2C.r * f;
		params2->unk2C.g = params1->unk2C.g * f;
		params2->unk2C.b = params1->unk2C.b * f;

		params2->unk30.r = params1->unk30.r * f;
		params2->unk30.g = params1->unk30.g * f;
		params2->unk30.b = params1->unk30.b * f;
	} else {
		params2->unk2C = unk90.unkC->unk38;
		params2->unk30 = unk90.unkC->unk3C;
	}

	if (unk90.unkC->unk4E & 2) {
		f32 f = params1->unk20 * unk90.unkC->unk2C;

		params2->unk2C.a = params1->unk2C.a * f;
		params2->unk30.a = params1->unk30.a * f;
	} else {
		params2->unk2C.a = unk90.unkC->unk38.a;
		params2->unk30.a = unk90.unkC->unk3C.a;
	}

	if (unk90.unkC->unk4E & 1) {
		f32 f1         = unk90.unkC->unk28 * params1->unk10;
		params2->unkC  = f1;
		params2->unk10 = f1;
		f32 f2         = unk90.unkC->unk28 * params1->unk14;
		params2->unk24 = f2;
		params2->unk14 = f2;
	} else {
		params2->unk24 = 1.0f;
		params2->unk14 = 1.0f;
		params2->unkC  = 1.0f;
		params2->unk10 = 1.0f;
	}

	params2->unk34 = params1->unk34;
	if ((int)unk90.unkC->unk4D != 0) {
		params2->unk36 = unk90.unkC->unk30 * 32768.0f;
	} else {
		params2->unk36 = 0;
	}
}

const ResTIMG* JPADraw::swapImage(const ResTIMG* param_1, s16 param_2)
{
	if (param_2 < 0)
		return nullptr;

	JPATexture* tex = unk90.unk1C->unk2C[unk90.unk20[(u8)param_2]];

	const ResTIMG* old = tex->unk8.getTexInfo();
	tex->unk8.storeTIMG(param_1);

	return old;
}

void JPADraw::loadTexture(unsigned char, _GXTexMapID) { }

void JPADraw::setDrawExecVisitorsBeforeCB(
    const JPADraw::JPADrawVisitorDefFlags& flags)
{
	if (unk90.unk10)
		unk0[unk88++] = &vc.unk50;

	if (flags.unk10) {
		unk4[unk89++] = &vc.unkC;
	} else if ((int)unk90.unk4->unk7B != 0) {
		if (unk90.unk4->unk87)
			unk4[unk89++] = &vc.unk4;
		else
			unk4[unk89++] = &vc.unk0;
	} else if (unk90.unk4->unk87) {
		if (flags.unkC)
			unk4[unk89++] = &vc.unk10;
		else
			unk4[unk89++] = &vc.unk8;
	} else {
		unk4[unk89++] = &vc.unkC;
	}

	if (unk90.unk4->unk68 & 2) {
		if (flags.unkC || !flags.unk14) {
			unk4[unk89++] = &vc.unk20;
		} else if (flags.unk14) {
			unk4[unk89++] = &vc.unk28;
		}
	} else {
		if ((!(unk90.unk4->unk83 & 1) || !(unk90.unk4->unk83 & 2))
		    && flags.unk14 == 0) {
			if (!(unk90.unk4->unk84 & 1) || !(unk90.unk4->unk84 & 2)) {
				unk4[unk89++] = &vc.unk20;
			} else {
				unk4[unk89++] = &vc.unk24;
			}
		} else if ((unk90.unk4->unk84 & 1) == 0
		           || (unk90.unk4->unk84 & 2) == 0) {
			unk4[unk89++] = &vc.unk28;
		}
	}

	if (unk90.unk4->unk81 == 0
	    && (unk90.unk4->unk80 == 0 || (unk90.unk4->unk68 & 1))) {
		unk4[unk89++] = &vc.unk48;
	} else if (unk90.unk4->unk81 != 0) {
		unk4[unk89++] = &vc.unk44;
	}

	if (unk90.unkC != nullptr && (int)unk90.unkC->unk4B == 0
	    && !(unk90.unkC->unk4E & 2) && !(unk90.unkC->unk4E & 4)) {
		unk18[unk8A++] = &vc.unk2C;
	}

	if (!(unk90.unk4->unk68 & 2)) {
		if (flags.unk4) {
			if (flags.unk8)
				unk34[unk8C++] = &vc.unk3C;
			else
				unk34[unk8C++] = &vc.unk30;
		} else {
			if (flags.unk14) {
				if (flags.unk8)
					unk34[unk8C++] = &vc.unk40;
				else
					unk34[unk8C++] = &vc.unk34;
			} else {
				if (flags.unk8)
					unk34[unk8C++] = &vc.unk38;
			}
		}
	} else if (flags.unk14 && !flags.unkC) {
		unk34[unk8C++] = &vc.unk34;
	}

	if (unk90.unk4->unk81 == 0 && unk90.unk4->unk80 != 0
	    && !(unk90.unk4->unk68 & 1)) {
		unk34[unk8C++] = &vc.unk48;
	}

	switch (unk90.unk4->unk69) {
	case 0:
		if (unk90.unk8 && (unk90.unk8->unk7E & 1))
			unk34[unk8C++] = &vc.unk18;
		break;
	case 1:
		if (unk90.unk8 && (unk90.unk8->unk7E & 1))
			unk34[unk8C++] = &vc.unk1C;
		break;

	case 2:
	case 3:
	case 4:
	case 7:
	case 8:
	case 9:
	case 10:
		if (unk90.unk4->unk87 && (int)unk90.unk4->unk7B == 0)
			unk34[unk8C++] = &vc.unk10;
		break;
	}

	if (unk90.unkC) {
		if ((int)unk90.unkC->unk4B != 0 || (unk90.unkC->unk4E & 2)
		    || (unk90.unkC->unk4E & 4))
			unk70[unk8E++] = &vc.unk3C;

		switch (unk90.unkC->unk44) {
		case 0:
			unk70[unk8E++] = &vc.unk18;
			break;
		case 1:
			unk70[unk8E++] = &vc.unk1C;
			break;
		}
	}
}

void JPADraw::setDrawExecVisitorsAfterCB(
    const JPADraw::JPADrawVisitorDefFlags& flags)
{
	if (flags.unk0) {
		switch (unk90.unk4->unk69) {
		case 0:
			unk34[unk8C++] = &vc.unk80;
			break;
		case 1:
			unk34[unk8C++] = &vc.unk84;
			break;

		case 2:
			if (unk90.unk8 && (int)unk90.unk8->unk7F != 0)
				unk34[unk8C++] = &vc.unk58;
			else
				unk34[unk8C++] = &vc.unk54;
			break;
		case 3:
			if (unk90.unk8 && (int)unk90.unk8->unk7F != 0)
				unk34[unk8C++] = &vc.unk68;
			else
				unk34[unk8C++] = &vc.unk64;
			break;
		case 4:
			if (unk90.unk8 && (int)unk90.unk8->unk7F != 0)
				unk34[unk8C++] = &vc.unk70;
			else
				unk34[unk8C++] = &vc.unk6C;
			break;
		case 10:
			if (unk90.unk8 && (int)unk90.unk8->unk7F != 0)
				unk34[unk8C++] = &vc.unk60;
			else
				unk34[unk8C++] = &vc.unk5C;
			break;

		case 7:
			unk34[unk8C++] = &vc.unk78;
			break;
		case 8:
			unk34[unk8C++] = &vc.unk7C;
			break;
		case 9:
			unk34[unk8C++] = &vc.unk74;
			break;

		case 5:
			unk4[unk89++] = &vc.unk88;
			break;
		case 6:
			unk4[unk89++] = &vc.unk8C;
			break;
		}
	}

	if (unk90.unkC != nullptr) {
		switch (unk90.unkC->unk44) {
		case 0:
			unk70[unk8E++] = &vc.unk80;
			break;
		case 1:
			unk70[unk8E++] = &vc.unk84;
			break;

		case 2:
			if ((unk90.unk8 && (int)unk90.unk8->unk7F != 0)
			    || (int)unk90.unkC->unk4D != 0)
				unk70[unk8E++] = &vc.unk58;
			else
				unk70[unk8E++] = &vc.unk54;
			break;
		case 3:
			if ((unk90.unk8 && (int)unk90.unk8->unk7F != 0)
			    || (int)unk90.unkC->unk4D != 0)
				unk70[unk8E++] = &vc.unk68;
			else
				unk70[unk8E++] = &vc.unk64;
			break;
		case 4:
			if ((unk90.unk8 && (int)unk90.unk8->unk7F != 0)
			    || (int)unk90.unkC->unk4D != 0)
				unk70[unk8E++] = &vc.unk70;
			else
				unk70[unk8E++] = &vc.unk6C;
			break;
		case 10:
			if ((unk90.unk8 && (int)unk90.unk8->unk7F != 0)
			    || (int)unk90.unkC->unk4D != 0)
				unk70[unk8E++] = &vc.unk60;
			else
				unk70[unk8E++] = &vc.unk5C;
			break;

		case 7:
			unk70[unk8E++] = &vc.unk78;
			break;
		case 8:
			unk70[unk8E++] = &vc.unk7C;
			break;
		case 9:
			unk70[unk8E++] = &vc.unk74;
			break;

		case 5:
			unk18[unk8A++] = &vc.unk88;
			break;
		case 6:
			unk18[unk8A++] = &vc.unk8C;
			break;
		}
	}
}

void JPADraw::setDrawCalcVisitors(const JPADraw::JPADrawVisitorDefFlags& flags)
{
	if ((unk90.unk4->unk68 & 2) && (flags.unk4 != 0 || flags.unk8 != 0)) {
		switch (unk90.unk4->unk82) {
		case 0:
			unk24[unk8B++] = &vc.unkD4;
			break;
		case 1:
			unk24[unk8B++] = &vc.unkDC;
			break;
		case 2:
			unk24[unk8B++] = &vc.unkE4;
			break;
		case 3:
			unk24[unk8B++] = &vc.unkEC;
			break;
		case 4:
			unk24[unk8B++] = &vc.unkF4;
			break;
		}

		if (flags.unk4 != 0)
			unk24[unk8B++] = &vc.unkC0;

		if (flags.unk8 != 0)
			unk24[unk8B++] = &vc.unkC8;
	}

	if (unk90.unk4->unk81 == 0 && unk90.unk4->unk80 != 0
	    && (unk90.unk4->unk68 & 1)) {
		switch (unk90.unk4->unk7D) {
		case 0:
			unk24[unk8B++] = &vc.unk10C;
			break;
		case 1:
			unk24[unk8B++] = &vc.unk114;
			break;
		case 2:
			unk24[unk8B++] = &vc.unk11C;
			break;
		case 3:
			unk24[unk8B++] = &vc.unk124;
			break;
		case 4:
			unk24[unk8B++] = &vc.unk12C;
			break;
		}
	}

	if (unk90.unk8 != nullptr && (unk90.unk8->unk7E & 1)) {

		if (unk90.unk8->unk7E & 8) {
			if (unk90.unk8->unk7D)
				unk48[unk8D++] = &vc.unkB8;
			else
				unk48[unk8D++] = &vc.unkB0;
		} else {
			unk48[unk8D++] = &vc.unkAC;
		}

		if ((unk90.unk8->unk7E & 0x20) && unk90.unk4->unk69 != 1)
			unk48[unk8D++] = &vc.unk9C;
		else
			unk48[unk8D++] = &vc.unk98;

		if (unk90.unk4->unk69 != 0) {
			if ((unk90.unk8->unk7E & 2) == 0) {
				unk48[unk8D++] = &vc.unkA8;
			} else {
				if (unk90.unk8->unk7E & 4) {
					if (unk90.unk8->unk7B) {
						unk48[unk8D++] = &vc.unkBC;
					} else {
						unk48[unk8D++] = &vc.unkB4;
					}
				} else {
					if ((unk90.unk8->unk7E & 8) != 0) {
						unk48[unk8D++] = &vc.unkAC;
					}
				}

				if (unk90.unk8->unk7E & 0x10) {
					unk48[unk8D++] = &vc.unkA4;
				} else {
					unk48[unk8D++] = &vc.unkA0;
				}
			}
		}
	}

	if ((unk90.unk4->unk68 & 2) == 0) {
		if (flags.unk4 || flags.unk8) {
			switch (unk90.unk4->unk82) {
			case 0:
				unk48[unk8D++] = &vc.unkD4;
				break;
			case 1:
				unk48[unk8D++] = &vc.unkDC;
				break;
			case 2:
				unk48[unk8D++] = &vc.unkE4;
				break;
			case 3:
				unk48[unk8D++] = &vc.unkEC;
				break;
			case 4:
				unk48[unk8D++] = &vc.unkF4;
				break;
			}

			if (flags.unk4)
				unk48[unk8D++] = &vc.unkC0;

			if (flags.unk8)
				unk48[unk8D++] = &vc.unkC8;
		}
	} else {
		unk48[unk8D++] = &vc.unkD0;
	}

	if (flags.unk14 && !flags.unkC) {
		unk48[unk8D++] = &vc.unkFC;
		if (unk90.unk8->unk78 & 2) {
			switch (unk90.unk8->unk79) {
			case 0:
				unk48[unk8D++] = &vc.unk100;
				break;
			case 1:
				unk48[unk8D++] = &vc.unk104;
				break;
			case 2:
				unk48[unk8D++] = &vc.unk108;
				break;
			}
		}
	}

	if (unk90.unk4->unk81 == 0 && unk90.unk4->unk80 != 0
	    && (unk90.unk4->unk68 & 1) == 0) {
		switch (unk90.unk4->unk7D) {
		case 0:
			unk48[unk8D++] = &vc.unk10C;
			break;
		case 1:
			unk48[unk8D++] = &vc.unk114;
			break;
		case 2:
			unk48[unk8D++] = &vc.unk11C;
			break;
		case 3:
			unk48[unk8D++] = &vc.unk124;
			break;
		case 4:
			unk48[unk8D++] = &vc.unk12C;
			break;
		}
	}

	if (unk90.unkC) {
		if ((int)unk90.unkC->unk4B != 0)
			unk80[unk8F++] = &vc.unk134;

		if ((int)unk90.unkC->unk4A != 0)
			unk80[unk8F++] = &vc.unk138;
	}
}

void JPADraw::setParticleClipBoard()
{
	switch (unk90.unk4->unk69) {
	case 2:
	case 9:
		MTXIdentity(cb.unk68);
		break;
	case 10:
		loadYBBMtx(*cb.unk34);
		break;
	default:
		MTXCopy(*cb.unk34, cb.unk68);
		break;
	}

	GXLoadPosMtxImm(cb.unk68, 0); // TODO: TVec2 arithmetic?
	f32 a   = unk90.unk0->unk174;
	f32 b   = unk90.unk0->unk178;
	cb.unk4 = a * (unk90.unk4->unk14 * 25.0f);
	cb.unk8 = b * (unk90.unk4->unk18 * 25.0f);

	if (unk90.unk4->unk69 == 0) {
		cb.unk4 *= 1.02f;
		cb.unk8 *= 1.02f;
	} else if (unk90.unk4->unk69 == 1) {
		cb.unk4 *= 1.02f;
		cb.unk8 *= 0.4f;
	}

	if (unk90.unk8 != nullptr && (unk90.unk8->unk7E & 1)) {
		cb.unkC  = cb.unk4 * (unk90.unk8->unk7C - 1.0f);
		cb.unk10 = cb.unk8 * (unk90.unk8->unk7A - 1.0f);
	} else {
		cb.unk10 = 0.0f;
		cb.unkC  = 0.0f;
	}

	f32 x = unk90.unk4->unk2C;
	f32 y = unk90.unk4->unk30;

	cb.unk14[0].x = 0.0f;
	cb.unk14[0].y = 0.0f;
	cb.unk14[1].x = x;
	cb.unk14[1].y = 0.0f;

	cb.unk14[2].x = x;
	cb.unk14[2].y = y;

	cb.unk14[3].x = 0.0f;
	cb.unk14[3].y = y;

	if (unk90.unk4->unk81 == 0 && unk90.unk4->unk80 == 0)
		unkC0 = unk90.unk20[unk90.unk4->unk7F];
	cb.unkA0 = nullptr;
	cb.unkA4 = nullptr;

	if (unk90.unk4->unk69 == 3 || unk90.unk4->unk69 == 9
	    || unk90.unk4->unk69 == 4 || unk90.unk4->unk69 == 5
	    || unk90.unk4->unk69 == 6) {
		switch (unk90.unk4->unk6A) {
		case 0:
			cb.unkA0 = &dirTypeVel;
			break;
		case 1:
			cb.unkA0 = &dirTypePos;
			break;
		case 2:
			cb.unkA0 = &dirTypePosInv;
			break;
		case 3:
			cb.unkA0 = &dirTypeEmtrDir;
			break;
		case 4:
			cb.unkA0 = &dirTypePrevPtcl;
			break;
		}
	}

	if (unk90.unk4->unk69 == 3 || unk90.unk4->unk69 == 4
	    || unk90.unk4->unk69 == 7 || unk90.unk4->unk69 == 8) {
		switch (unk90.unk4->unk6B) {
		case 0:
			cb.unkA4 = &rotTypeY;
			break;
		case 1:
			cb.unkA4 = &rotTypeX;
			break;
		case 2:
			cb.unkA4 = &rotTypeZ;
			break;
		case 3:
			cb.unkA4 = &rotTypeXYZ;
			break;
		case 4:
			cb.unkA4 = &rotTypeYJiggle;
			break;
		}
	}
}

void JPADraw::setChildClipBoard() { }

void JPADraw::drawParticle() { }

void JPADraw::drawChild() { }

void JPADraw::zDraw() { }

void JPADraw::zDrawParticle() { }

void JPADraw::zDrawChild() { }

void JPADraw::getMainTextureID(unsigned char) { }

void JPADraw::getIndTextureID() { }

void JPADraw::getIndSubTextureID() { }

void JPADraw::getSecondTextureID() { }

void JPADraw::loadYBBMtx(MtxPtr) { }

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

	GXColor white = { 0xff, 0xff, 0xff, 0xff };

	if (unk90.unk4->isEnablePrm()) {
		if (!(unk90.unk4->isEnablePrmAnm()))
			unkB8 = unk90.unk4->getPrmColor();
	} else {
		unkB8 = white;
	}

	if (unk90.unk4->isEnableEnv()) {
		if (!(unk90.unk4->isEnableEnvAnm()))
			unkBC = unk90.unk4->getEnvColor();
	} else {
		unkBC = white;
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

	JPADrawVisitorDefFlags local_28;

	local_28.unk0  = unk90.unkC == nullptr || unk90.unkC->isEnableDrawParent();
	local_28.unk4  = unk90.unk4->isEnablePrm() && unk90.unk4->isEnablePrmAnm();
	local_28.unk8  = unk90.unk4->isEnableEnv() && unk90.unk4->isEnableEnvAnm();
	local_28.unkC  = unk90.unk4->getType() == 5 || unk90.unk4->getType() == 6;
	local_28.unk10 = unk90.unk4->getType() == 0 || unk90.unk4->getType() == 1;
	local_28.unk14 = unk90.unk8 != nullptr && unk90.unk8->isEnableAlpha();

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

	GXSetBlendMode(unk90.unk4->getBlendMode1(),
	               unk90.unk4->getSrcBlendFactor1(),
	               unk90.unk4->getDstBlendFactor1(), unk90.unk4->getBlendOp1());

	// TODO: fakematch
	JPABaseEmitter* be = unk90.unk0;

	cb.unk98.r = be->unk180.r;
	cb.unk98.g = be->unk180.g;
	cb.unk98.b = be->unk180.b;
	cb.unk98.a = be->unk180.a;

	be         = unk90.unk0;
	cb.unk9C.r = be->unk184;
	cb.unk9C.g = be->unk185;
	cb.unk9C.b = be->unk186;
	cb.unk9C.a = 0xff;

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
	JPADrawParams* params = particle->getDrawParamPPtr();
	JPAEmitterInfo* info  = JPAGetEmitterInfoPtr();

	// TODO: something wrong here
	f32* v         = info->unk48[1];
	params->unk0.x = v[0];
	params->unk0.y = v[1];
	params->unk0.z = v[2];

	params->unk2C = unkB8;
	params->unk30 = unkBC;
	params->unk20 = 1.0f;

	params->unk28
	    = (int)(unk90.unk0->getRandomF() * unk90.unk4->getLoopOffset());

	if (unk90.unk8 != nullptr) {
		if (unk90.unk8->isEnableRotate() != 0) {
			params->unk34 = unk90.unk8->getRotateAngle() * 32768.0f
			                + unk90.unk0->getRandomSF()
			                      * unk90.unk8->getRotateRandomAngle()
			                      * 65536.0f;

			s16 sVar4;
			if (unk90.unk0->getRandomRF() < unk90.unk8->getRotateDirection()) {
				sVar4 = unk90.unk8->getRotateSpeed()
				        * (unk90.unk8->getRotateRandomSpeed()
				               * unk90.unk0->getRandomRF()
				           + 1.0f)
				        * 32768.0f;
			} else {
				sVar4 = -unk90.unk8->getRotateSpeed()
				        * (unk90.unk8->getRotateRandomSpeed()
				               * unk90.unk0->getRandomRF()
				           + 1.0f)
				        * 32768.0f;
			}
			params->unk36 = sVar4;
		} else {
			params->unk34 = 0;
			params->unk36 = 0;
		}

		params->unk10 = params->unk14 = params->unkC
		    = unkB4
		      * (unk90.unk0->getRandomRF() * unk90.unk8->getRandomScale()
		         + 1.0f);

		params->unk24
		    = unk90.unk0->getRandomRF() * unk90.unk8->getAlphaWaveRandom()
		      + 1.0f;
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

	if (unk90.unkC->isInheritedRGB()) {
		f32 f = unk90.unkC->getInheritRGB();

		params2->unk2C.r = params1->unk2C.r * f;
		params2->unk2C.g = params1->unk2C.g * f;
		params2->unk2C.b = params1->unk2C.b * f;

		params2->unk30.r = params1->unk30.r * f;
		params2->unk30.g = params1->unk30.g * f;
		params2->unk30.b = params1->unk30.b * f;
	} else {
		params2->unk2C = unk90.unkC->getPrm();
		params2->unk30 = unk90.unkC->getEnv();
	}

	if (unk90.unkC->isInheritedAlpha()) {
		f32 f = params1->unk20 * unk90.unkC->getInheritAlpha();

		params2->unk2C.a = params1->unk2C.a * f;
		params2->unk30.a = params1->unk30.a * f;
	} else {
		params2->unk2C.a = unk90.unkC->getPrmAlpha();
		params2->unk30.a = unk90.unkC->getEnvAlpha();
	}

	if (unk90.unkC->isInheritedScale()) {
		f32 f = unk90.unkC->getInheritScale();

		params2->unk10 = params2->unkC = f * params1->unk10;
		params2->unk14 = params2->unk24 = f * params1->unk14;
	} else {
		params2->unk24 = 1.0f;
		params2->unk14 = 1.0f;
		params2->unkC  = 1.0f;
		params2->unk10 = 1.0f;
	}

	params2->unk34 = params1->unk34;
	if (unk90.unkC->isEnableRotate() != 0) {
		params2->unk36 = unk90.unkC->getRotateSpeed() * 32768.0f;
	} else {
		params2->unk36 = 0;
	}
}

const ResTIMG* JPADraw::swapImage(const ResTIMG* param_1, s16 param_2)
{
	if (param_2 < 0)
		return nullptr;

	return unk90.unk1C->swapImage(param_1, unk90.getTexIdx(param_2));
}

void JPADraw::loadTexture(u8 idx, GXTexMapID map_id) { }

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
	switch (unk90.unk4->getType()) {
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

	GXLoadPosMtxImm(cb.unk68, 0);

	// TODO: TVec2 arithmetic?
	f32 a   = unk90.unk0->unk174;
	f32 b   = unk90.unk0->unk178;
	cb.unk4 = a * (unk90.unk4->unk14 * 25.0f);
	cb.unk8 = b * (unk90.unk4->unk18 * 25.0f);

	if (unk90.unk4->getType() == 0) {
		cb.unk4 *= 1.02f;
		cb.unk8 *= 1.02f;
	} else if (unk90.unk4->getType() == 1) {
		cb.unk4 *= 1.02f;
		cb.unk8 *= 0.4f;
	}

	if (unk90.unk8 != nullptr && unk90.unk8->isEnableScale()) {
		cb.unkC  = cb.unk4 * (unk90.unk8->unk7C - 1.0f);
		cb.unk10 = cb.unk8 * (unk90.unk8->unk7A - 1.0f);
	} else {
		cb.unk10 = 0.0f;
		cb.unkC  = 0.0f;
	}

	f32 x = unk90.unk4->getTilingX();
	f32 y = unk90.unk4->getTilingY();

	cb.unk14[0].x = 0.0f;
	cb.unk14[0].y = 0.0f;
	cb.unk14[1].x = x;
	cb.unk14[1].y = 0.0f;

	cb.unk14[2].x = x;
	cb.unk14[2].y = y;

	cb.unk14[3].x = 0.0f;
	cb.unk14[3].y = y;

	if (!unk90.unk4->textureIsEmpty() && !unk90.unk4->isEnableTextureAnm())
		unkC0 = unk90.unk20[unk90.unk4->getTextureIndex()];

	cb.unkA0 = nullptr;
	cb.unkA4 = nullptr;

	if (unk90.unk4->getType() == 3 || unk90.unk4->getType() == 9
	    || unk90.unk4->getType() == 4 || unk90.unk4->getType() == 5
	    || unk90.unk4->getType() == 6) {
		switch (unk90.unk4->getDirType()) {
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

	if (unk90.unk4->getType() == 3 || unk90.unk4->getType() == 4
	    || unk90.unk4->getType() == 7 || unk90.unk4->getType() == 8) {
		switch (unk90.unk4->getRotType()) {
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

void JPADraw::setChildClipBoard()
{
	switch (unk90.unk4->getType()) {
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

	GXLoadPosMtxImm(cb.unk68, 0);

	f32 fVar3 = unk90.unk0->unk174;
	f32 fVar4 = unk90.unk0->unk178;
	if (!unk90.unkC->isInheritedScale()) {
		cb.unk4 = fVar3 * (25.0f * unk90.unkC->getScaleX());
		cb.unk8 = fVar4 * (25.0f * unk90.unkC->getScaleY());
	} else {
		cb.unk4 = fVar3 * (25.0f * unk90.unk4->getBaseSizeX());
		cb.unk8 = fVar4 * (25.0f * unk90.unk4->getBaseSizeY());
	}

	if (unk90.unkC->getType() == 0) {
		cb.unk4 *= 1.02f;
		cb.unk8 *= 1.02f;
	} else if (unk90.unkC->getType() == 1) {
		cb.unk4 *= 1.02f;
		cb.unk8 *= 0.4f;
	}

	cb.unkC = cb.unk10 = 0.0f;

	cb.unk14[1].y = 0.0;
	cb.unk14[0].y = 0.0;
	cb.unk14[3].x = 0.0;
	cb.unk14[0].x = 0.0;
	cb.unk14[3].y = 1.0;
	cb.unk14[2].y = 1.0;
	cb.unk14[2].x = 1.0;
	cb.unk14[1].x = 1.0;

	cb.unkA0 = nullptr;
	cb.unkA4 = nullptr;

	if (unk90.unkC->getType() == 3 || unk90.unkC->getType() == 9
	    || unk90.unkC->getType() == 4 || unk90.unkC->getType() == 5
	    || unk90.unkC->getType() == 6) {
		switch (unk90.unkC->getDirType()) {
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

	if (unk90.unkC->getType() == 3 || unk90.unkC->getType() == 4
	    || unk90.unkC->getType() == 7 || unk90.unkC->getType() == 8) {
		switch (unk90.unkC->getRotType()) {
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

void JPADraw::drawParticle()
{
	unkC2 &= ~0x2;
	setParticleClipBoard();
	unk90.unk18 = &unk90.unk0->unkF4;

	GXSetPointSize(cb.unk4, GX_TO_ONE);
	GXSetLineWidth(cb.unk4, GX_TO_ONE);
	GXSetZMode(unk90.unk4->isEnableZCmp(), unk90.unk4->getZCmpFunction(),
	           unk90.unk4->isEnableZCmpUpdate());
	GXSetZCompLoc(unk90.unk4->getZCompLoc());
	GXSetAlphaCompare(
	    unk90.unk4->getAlphaCmpComp0(), unk90.unk4->getAlphaCmpRef0(),
	    unk90.unk4->getAlphaCmpOp(), unk90.unk4->getAlphaCmpComp1(),
	    unk90.unk4->getAlphaCmpRef1());

	GXSetAlphaUpdate(unk90.unk4->isEnableAlphaUpdate());
	GXSetColorUpdate(GX_TRUE);
	GXSetCullMode(GX_CULL_NONE);
	if ((unk90.unk4->isClipOn())) {
		GXSetClipMode(GX_CLIP_ENABLE);
		GXSetMisc(GX_MT_XF_FLUSH, 8);
	} else {
		GXSetClipMode(GX_CLIP_DISABLE);
	}

	for (int i = 0; i < unk89; ++i)
		unk4[i]->exec(&unk90);

	JSUList<JPABaseParticle>* particles = unk90.unk0->getParticleList();
	if (unk90.unk4->getListOrder()) {
		JSULink<JPABaseParticle>* link;
		for (link = particles->getFirst(); link; link = link->getNext()) {
			JPABaseParticle* particle = link->getObject();
			for (int i = 0; i < unk8C; ++i)
				unk34[i]->exec(&unk90, particle);
		}
	} else {
		JSULink<JPABaseParticle>* link;
		for (link = particles->getLast(); link; link = link->getPrev()) {
			JPABaseParticle* particle = link->getObject();
			for (int i = 0; i < unk8C; ++i)
				unk34[i]->exec(&unk90, particle);
		}
	}

	GXSetMisc(GX_MT_XF_FLUSH, 0);
}

void JPADraw::drawChild()
{
	unkC2 |= 0x2;
	setChildClipBoard();
	unk90.unk18 = &unk90.unk0->unk100;
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
	GXEnableTexOffsets(GX_TEXCOORD0, GX_TRUE, GX_TRUE);

	// TODO: figure out how to use JPADraw::loadTexture
	if (unk90.unk4->textureIsEmpty()) {
		unk90.unk1C->loadDefaultTexture(GX_TEXMAP0);
	} else {
		unk90.unk1C->load(unk90.unk20[unk90.unkC->getTextureIndex()],
		                  GX_TEXMAP0);
	}

	GXSetZMode(unk90.unk4->isEnableZCmp(), unk90.unk4->getZCmpFunction(),
	           unk90.unk4->isEnableZCmpUpdate());
	GXSetZCompLoc(unk90.unk4->getZCompLoc());
	GXSetAlphaCompare(
	    unk90.unk4->getAlphaCmpComp0(), unk90.unk4->getAlphaCmpRef0(),
	    unk90.unk4->getAlphaCmpOp(), unk90.unk4->getAlphaCmpComp1(),
	    unk90.unk4->getAlphaCmpRef1());
	GXSetAlphaUpdate(unk90.unk4->isEnableAlphaUpdate());
	GXSetColorUpdate(GX_TRUE);
	GXSetCullMode(GX_CULL_NONE);

	if (unk90.unkC->isClipOn()) {
		GXSetClipMode(GX_CLIP_ENABLE);
		GXSetMisc(GX_MT_XF_FLUSH, 8);
	} else {
		GXSetClipMode(GX_CLIP_DISABLE);
	}

	for (int i = 0; i < unk8A; ++i)
		unk18[i]->exec(&unk90);

	JSUList<JPABaseParticle>* particles = unk90.unk0->getChildParticleList();
	if (unk90.unk4->getListOrder()) {
		JSULink<JPABaseParticle>* link;
		for (link = particles->getFirst(); link; link = link->getNext()) {
			JPABaseParticle* particle = link->getObject();
			for (int i = 0; i < unk8E; ++i)
				unk70[i]->exec(&unk90, particle);
		}
	} else {
		JSULink<JPABaseParticle>* link;
		for (link = particles->getLast(); link; link = link->getPrev()) {
			JPABaseParticle* particle = link->getObject();
			for (int i = 0; i < unk8E; ++i)
				unk70[i]->exec(&unk90, particle);
		}
	}

	GXSetMisc(GX_MT_XF_FLUSH, 0);
}

void JPADraw::zDraw()
{
	unkC2 |= 1;
	if (unk90.unk4->getChildOrder() == 1 && unk90.unkC != nullptr)
		zDrawChild();

	zDrawParticle();

	if (unk90.unk4->getChildOrder() == 0 && unk90.unkC != nullptr)
		zDrawChild();
}

void JPADraw::zDrawParticle()
{
	unkC2 &= ~0x2;
	setParticleClipBoard();
	unk90.unk18 = unk90.unk0->getParticleList();

	GXSetPointSize(cb.unk4, GX_TO_ONE);
	GXSetLineWidth(cb.unk4, GX_TO_ONE);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetZCompLoc(GX_FALSE);

	GXSetAlphaCompare(GX_GEQUAL, unk90.unk0->unk180.a, GX_AOP_OR, GX_GEQUAL,
	                  unk90.unk0->unk180.a);
	GXSetAlphaUpdate(GX_FALSE);
	GXSetColorUpdate(GX_FALSE);
	GXSetCullMode(GX_CULL_NONE);
	if (unk90.unk4->isClipOn()) {
		GXSetClipMode(GX_CLIP_ENABLE);
		GXSetMisc(GX_MT_XF_FLUSH, 8);
	} else {
		GXSetClipMode(GX_CLIP_DISABLE);
	}

	for (int i = 0; i < unk89; ++i)
		unk4[i]->exec(&unk90);

	JSUList<JPABaseParticle>* particles = unk90.unk0->getParticleList();
	if (unk90.unk4->getListOrder()) {
		JSULink<JPABaseParticle>* link;
		for (link = particles->getFirst(); link; link = link->getNext()) {
			JPABaseParticle* particle = link->getObject();
			for (int i = 0; i < unk8C; ++i)
				unk34[i]->exec(&unk90, particle);
		}
	} else {
		JSULink<JPABaseParticle>* link;
		for (link = particles->getLast(); link; link = link->getPrev()) {
			JPABaseParticle* particle = link->getObject();
			for (int i = 0; i < unk8C; ++i)
				unk34[i]->exec(&unk90, particle);
		}
	}

	GXSetMisc(GX_MT_XF_FLUSH, 0);
}

void JPADraw::zDrawChild()
{
	unkC2 |= 0x2;
	setChildClipBoard();
	unk90.unk18 = unk90.unk0->getChildParticleList();
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
	GXEnableTexOffsets(GX_TEXCOORD0, GX_TRUE, GX_TRUE);

	// TODO: figure out how to use JPADraw::loadTexture
	if (unk90.unk4->textureIsEmpty()) {
		unk90.unk1C->loadDefaultTexture(GX_TEXMAP0);
	} else {
		unk90.unk1C->load(unk90.unk20[unk90.unkC->getTextureIndex()],
		                  GX_TEXMAP0);
	}

	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetZCompLoc(GX_FALSE);
	GXSetAlphaCompare(GX_GEQUAL, unk90.unk0->unk180.a, GX_AOP_OR, GX_GEQUAL,
	                  unk90.unk0->unk180.a);
	GXSetAlphaUpdate(GX_FALSE);
	GXSetColorUpdate(GX_FALSE);
	GXSetCullMode(GX_CULL_NONE);

	if (unk90.unkC->isClipOn()) {
		GXSetClipMode(GX_CLIP_ENABLE);
		GXSetMisc(GX_MT_XF_FLUSH, 8);
	} else {
		GXSetClipMode(GX_CLIP_DISABLE);
	}

	for (int i = 0; i < unk8A; ++i)
		unk18[i]->exec(&unk90);

	JSUList<JPABaseParticle>* particles = unk90.unk0->getChildParticleList();
	if (unk90.unk4->getListOrder()) {
		JSULink<JPABaseParticle>* link;
		for (link = particles->getFirst(); link; link = link->getNext()) {
			JPABaseParticle* particle = link->getObject();
			for (int i = 0; i < unk8E; ++i)
				unk70[i]->exec(&unk90, particle);
		}
	} else {
		JSULink<JPABaseParticle>* link;
		for (link = particles->getLast(); link; link = link->getPrev()) {
			JPABaseParticle* particle = link->getObject();
			for (int i = 0; i < unk8E; ++i)
				unk70[i]->exec(&unk90, particle);
		}
	}

	GXSetMisc(GX_MT_XF_FLUSH, 0);
}

u32 JPADraw::getMainTextureID(u8 i)
{
	u32 result = -1;
	if (unk90.unk4->unk80 != 0) {
		if (i < unk90.unk4->unk7E)
			result = unk90.unk4->unk8[i];
	} else {
		if (i == 0)
			result = unk90.unk4->unk7F;
	}
	return result;
}

void JPADraw::getIndTextureID() { }

void JPADraw::getIndSubTextureID() { }

void JPADraw::getSecondTextureID() { }

void JPADraw::loadYBBMtx(MtxPtr mtx)
{
	JGeometry::TVec3<f32> v;
	v.set(0.0f, mtx[1][1], mtx[2][1]);
	v.normalize();

	cb.unk38[0][0] = 1.0f;
	cb.unk38[0][1] = 0.0f;
	cb.unk38[0][2] = 0.0f;
	cb.unk38[0][3] = mtx[0][3];

	cb.unk38[1][0] = 0.0f;
	cb.unk38[1][1] = v.y;
	cb.unk38[1][2] = -v.z;
	cb.unk38[1][3] = mtx[1][3];

	cb.unk38[2][0] = 0.0f;
	cb.unk38[2][1] = v.z;
	cb.unk38[2][2] = v.y;
	cb.unk38[2][3] = mtx[2][3];

	MTXIdentity(cb.unk68);
}

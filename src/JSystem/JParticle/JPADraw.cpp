#include <JSystem/JParticle/JPADraw.hpp>
#include <JSystem/JParticle/JPABaseShape.hpp>
#include <JSystem/JParticle/JPAExtraShape.hpp>
#include <JSystem/JParticle/JPASweepShape.hpp>

JPADrawVisitorContainer JPADraw::vc;
JPADrawClipBoard JPADraw::cb;

void JPADraw::initialize(JPABaseEmitter*, JPATextureResource*) { }

void JPADraw::draw(MtxPtr) { }

void JPADraw::calc() { }

void JPADraw::calcParticle(JPABaseParticle*) { }

void JPADraw::calcChild(JPABaseParticle*) { }

void JPADraw::initParticle(JPABaseParticle*) { }

void JPADraw::initChild(JPABaseParticle*, JPABaseParticle*) { }

void JPADraw::swapImage(const ResTIMG*, short) { }

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

void JPADraw::setParticleClipBoard() { }

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

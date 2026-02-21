#include <GC2D/Menu.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <System/MarioGamePad.hpp>
#include <dolphin/gx.h>

void TMenuBase::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 0x8) {
		J2DOrthoGraph orthoGraph(param_2->getUnk54());
		orthoGraph.setup2D();
		unk10->draw(0, 0, &orthoGraph);
		const JUTRect& rect = param_2->getUnk64();
		GXSetScissor(rect.x1, rect.y1, rect.getWidth(), rect.getHeight());
	}
}

TMenuPlane::TMenuPlane(const TMarioGamePad* param_1, J2DPane* param_2,
                       u32 param_3, u32 param_4)
    : JDrama::TViewObj("<TMenuPlane>")
    , unk10(param_1)
    , unk14(param_2)
    , unk18(0)
    , unk1C(255, 60, 0, 255)
    , unk20(255, 255, 0, 255)
    , unk24(255, 255, 255, 255)
    , unk28(0)
    , unk2C(0)
    , unk30(nullptr)
    , unk34(param_3)
    , unk38(param_4)
    , unk3C(0)
{
	J2DTextBox* local_420[256];

	JSUTreeIterator<J2DPane> iterator;
	for (iterator = unk14->mPaneTree.getFirstChild();
	     iterator != unk14->mPaneTree.getEndChild();) {
		J2DPane* pane = iterator.getObject();

		if (pane->mInfoTag == 0x13 && pane->mUserInfoTag != 'rset') {
			J2DTextBox* textBox = (J2DTextBox*)pane;
			local_420[unk28]    = textBox;
			if (unk28 == 0) {
				unk24               = textBox->mCharColor.get();
				textBox->mCharColor = unk1C.get();
				textBox->mGradColor = unk20.get();
			}
			++unk28;
		}

		++iterator;
	}

	unk30 = new J2DTextBox*[unk28];
	for (u32 i = 0; i < unk28; ++i)
		unk30[i] = local_420[i];
}

void TMenuPlane::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (unk18 & 0x4)
		return;

	if (param_1 & 0x1) {
		if (unk10->checkFrameMeaning(unk34)) {
			unk18 |= 0x1;
			return;
		}

		if (unk10->checkFrameMeaning(unk38)) {
			unk18 |= 0x2;
			return;
		}

		if (unk28 > 1 && unk10->checkFrameMeaning(0x1E)) {
			unk30[unk2C]->mCharColor = unk24.get();
			unk30[unk2C]->mGradColor = unk24.get();
			if (unk10->checkFrameMeaning(0x18)) {
				if (unk2C < unk3C) {
					if (unk28 > unk2C + unk3C) {
						unk2C += unk3C;
					}
				} else {
					unk2C -= unk3C;
				}
			}

			if (unk10->checkFrameMeaning(0x2)) {
				{
					if (unk2C == 0)
						unk2C = unk28;
					unk2C -= 1;
				}
			} else if (unk10->checkFrameMeaning(0x4)) {
				unk2C += 1;
				if (unk2C >= unk28)
					unk2C = 0;
			}

			unk30[unk2C]->mCharColor = unk1C.get();
			unk30[unk2C]->mGradColor = unk20.get();
		}
	}
}

void TFlashPane::perform(u32 param_1, JDrama::TGraphics*)
{
	if (param_1 & 0x1) {
		unk14 += unk18;
		if (unk18 < 0) {
			if (unk14 <= 0) {
				unk14 = 0;
				unk18 = 4;
			}
		} else if (unk14 >= 255) {
			unk14 = 255;
			unk18 = -4;
		}

		unk10->mAlpha = unk14;
	}
}

#include <GC2D/Menu.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <System/MarioGamePad.hpp>
#include <dolphin/gx.h>

// Binding level over a raw member read, worth +8 of low region in
// TMenuBase::perform (batch 127).
static inline J2DScreen* MenuUnk10(const TMenuBase* p)
{
	J2DScreen* v10 = p->unk10;
	return v10;
}

void TMenuBase::perform(u32 cue, JDrama::TGraphics* graphics)
{
	// TODO: 99.8%, frame 0x110 vs 0x118 with `orthoGraph` at 0x14 instead of
	// 0x18, i.e. four bytes of low region short (frames are 8-aligned, so
	// +4 low reads as +8). Naming the viewport reference is +0.1 and no
	// frame; `getScreen()` over `unk10`, spelling the scissor width out as
	// `x2 - x1`, and taking the scissor by value (which is +0x10 and three
	// instructions) all measured worse or inert.
	if (cue & CUE_DRAW) {
		const JUTRect& viewport = graphics->getViewport();
		J2DOrthoGraph orthoGraph(viewport);
		orthoGraph.setup2D();
		MenuUnk10(this)->draw(0, 0, &orthoGraph);
		const JUTRect& rect = graphics->getScissor();
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
	// The loop bound below must be `int`: retail's `cmpw` is a signed
	// compare against the `int` member, a `u32` index gives `cmplw`.
	//
	// TODO: besides the JUTColor temp stride described at perform(), the
	// loop body has one structural residue. Retail reloads `this` from its
	// 8(r1) spill slot into r6 at the top of the `mInfoTag == 0x13` block
	// and then reads `unk28` *twice* -- once for `local_420[unk28]` and
	// again for the `unk28 == 0` test, because the `stwx` between them
	// invalidates the cached load. We keep `this` in r31 across the whole
	// loop and therefore cache `unk28` in r3. Retail's r31 holds something
	// else for the duration (it reloads `this` again after the loop), so
	// there is one more value live in retail's loop than in ours; the
	// candidate is a second iterator-derived local we have not identified.
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
	for (int i = 0; i < unk28; ++i)
		unk30[i] = local_420[i];
}

// TODO: 99.5%. Every instruction matches; the frame is 0x98 against
// retail's 0x78 because each `JUtility::TColor = <TColor>.get()` assignment
// below parks its conversion temporary in an 8-byte slot where retail uses
// 4 (ours 0x78/0x80/0x88/0x90, retail 0x64/0x68/0x6c/0x70 -- four temps,
// four bytes each of difference, plus alignment = 0x20). The same stride
// difference is the whole residue of TMenuPlane::TMenuPlane (0x4b8 vs
// 0x4c0, three temps) and of TMenuBase::perform. The assignment spelling
// itself is right: `J2DTextBox::setGradColor(a, b)` (the by-value pair)
// collapses the temporaries away entirely (frame 0x48, 101 instructions,
// 83.8%), and `mCharColor.set(x.get())`, `mCharColor = x` and
// `set(x.toUInt32())` each drop an instruction per site. The suspect is the
// commented-out `TColor(const TColor&)` copy constructor in
// JSystem/JUtility/JUTColor.hpp -- a shared header with its own trial
// table, so it is reported rather than changed here.
void TMenuPlane::perform(u32 cue, JDrama::TGraphics*)
{
	if (unk18 & 0x4)
		return;

	if (cue & CUE_MOVE) {
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

void TFlashPane::perform(u32 cue, JDrama::TGraphics*)
{
	if (cue & CUE_MOVE) {
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

#include <GC2D/ExPane.hpp>
#include <JSystem/J2D/J2DScreen.hpp>

TExPane::TExPane(J2DScreen* param_1, u32 param_2)
{
	unk0  = param_1->search(param_2);
	unk4  = unk0->mBounds;
	unk44 = 255.0f;
	unk48 = 0.0f;
	unk4C = 0xff;
	unk4E = false;
	unk4F = false;
	unk50 = false;
}

TExPane::TExPane(JUTTexture*, GXCullMode) { }

bool TExPane::update()
{
	if (unk4E) {
		if (unk14.update())
			unk4E = false;
		unk0->move(unk4.x1 + unk14.getSomeX(), unk4.y1 + unk14.getSomeY());
	}

	if (unk4F) {
		if (unk2C.update())
			unk4F = false;
		unk0->move(unk2C.getSomeX(), unk2C.getSomeY());
	}

	if (unk50) {
		if ((unk48 < 0.0f && unk44 <= unk4C)
		    || (unk48 >= 0.0f && unk4C <= unk44)) {
			unk50 = false;
		}

		unk44 += unk48;
		s16 fVar1    = unk44 < 0.0f ? 0.0f : unk44;
		s16 fVar2    = fVar1 > 255 ? 255 : fVar1;
		unk0->mAlpha = fVar2;
	}

	// TODO: inlines
	bool bVar1 = false;
	bool bVar2 = false;
	if (!unk4E && !unk4F)
		bVar1 = true;
	if (bVar1) {
		if (!unk50)
			bVar2 = true;
		if (bVar2)
			return true;
	}

	return false;
}

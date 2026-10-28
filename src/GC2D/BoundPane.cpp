#include <GC2D/BoundPane.hpp>
#include <JSystem/J2D/J2DScreen.hpp>

TBoundPane::TBoundPane(J2DScreen* param_1, u32 param_2)
{
	unk0  = param_1->search(param_2);
	unk4  = unk0->getBounds();
	unk28 = 0.0f;
	unk2C = 0.0f;
	unk30 = 0.0f;
	unk34 = 0.0f;
	unk24 = false;
	unk25 = false;
}

TBoundPane::TBoundPane(JUTTexture*, GXCullMode) { }

void TBoundPane::setPanePosition(s32 param_1, const JUTPoint& param_2,
                                 const JUTPoint& param_3,
                                 const JUTPoint& param_4)
{
	unk28 = 0.0f;
	unk2C = 1.0f / param_1;
	unk38 = param_2;
	unk40 = param_3;
	unk48 = param_4;
	unk24 = true;
}

void TBoundPane::setPaneSize(s32 param_1, const JUTPoint& param_2,
                             const JUTPoint& param_3, const JUTPoint& param_4)
{
	unk30 = 0.0f;
	unk34 = 1.0f / param_1;
	unk50 = param_2;
	unk58 = param_3;
	unk60 = param_4;
	unk25 = true;
}

bool TBoundPane::update()
{
	if (unk24) {
		if (unk28 > 1.0f) {
			unk28 = 1.0f;
			unk24 = false;
		}

		makeNewPosition(unk28, unk14, unk38, unk40, unk48);
		unk0->move(unk4.x1 + unk14.x, unk4.y1 + unk14.y);
		unk28 += unk2C;
	}

	if (unk25) {
		if (unk30 > 1.0f) {
			unk30 = 1.0f;
			unk25 = false;
		}

		makeNewPosition(unk30, unk1C, unk50, unk58, unk60);
		unk0->resize(unk1C.x + unk4.getWidth(), unk1C.y + unk4.getHeight());
		unk30 += unk34;
	}

	bool result = false;
	if (!unk24 && !unk25)
		result = true;

	return result;
}

void TBoundPane::makeNewPosition(f32 t, JUTPoint& out, JUTPoint& start,
                                 JUTPoint& control, JUTPoint& end)
{
	f32 tt = t * t;
	f32 u  = 1.0f - t;
	f32 uu = u * u;
	f32 ut = 2.0f * u * t;

	f32 x = end.x * tt + (start.x * uu + control.x * ut);
	f32 y = end.y * tt + (start.y * uu + control.y * ut);

	s16 roundedX = x + (x > 0.0f ? 0.5f : -0.5f);
	s16 roundedY = y + (y > 0.0f ? 0.5f : -0.5f);
	out.set(roundedX, roundedY);
}

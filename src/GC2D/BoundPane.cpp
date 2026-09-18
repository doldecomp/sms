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

// TODO: UNUSED, map size 0x118 (280 bytes); the empty body plus the implicit
// JUTRect/JUTPoint member initialisation is only 0x88. The extra ~144 bytes
// presumably build a J2DPicture from the texture and apply the cull mode, but
// nothing in the TU or the map's closure names it, so it is left empty.
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

// Quadratic Bezier between start, control and end at parameter t, rounded away
// from zero. UNUSED in the ROM (map size 0x138, reproduced exactly): inlined at
// both update() sites. The parameter names are a guess; only their order is
// evidence-backed (it is the order setPanePosition/setPaneSize store them in).
void TBoundPane::makeNewPosition(f32 t, JUTPoint& out, JUTPoint& start,
                                 JUTPoint& control, JUTPoint& end)
{
	f32 t2   = t * t;
	f32 omt  = 1.0f - t;
	f32 omt2 = omt * omt;
	f32 mid  = 2.0f * omt * t;

	f32 posX = start.x * omt2 + control.x * mid + end.x * t2;
	f32 posY = start.y * omt2 + control.y * mid + end.y * t2;

	s16 nx = posX + (posX > 0.0f ? 0.5f : -0.5f);
	s16 ny = posY + (posY > 0.0f ? 0.5f : -0.5f);

	out.set(nx, ny);
}

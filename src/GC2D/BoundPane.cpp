#include <GC2D/BoundPane.hpp>
#include <JSystem/J2D/J2DScreen.hpp>

// TODO: nonmatching, we call JUTRect::set(0, 0, 0, 0) where the target
// zeroes the rect without the call
TBoundPane::TBoundPane(J2DScreen* param_1, u32 param_2)
{
	unk0  = param_1->search(param_2);
	unk4  = unk0->mBounds;
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

// TODO: nonmatching, frame 0x68 vs target 0x58 (one extra temp)
bool TBoundPane::update()
{
	JUTPoint position;
	if (unk24) {
		if (unk28 > 1.0f) {
			unk28 = 1.0f;
			unk24 = false;
		}

		makeNewPosition(unk28, position, unk38, unk40, unk48);
		unk14.x1 = position.x;
		unk14.y1 = position.y;
		unk0->move(unk4.x1 + unk14.x1, unk4.y1 + unk14.y1);

		unk28 += unk2C;
	}

	if (unk25) {
		if (unk30 > 1.0f) {
			unk30 = 1.0f;
			unk25 = false;
		}

		makeNewPosition(unk30, position, unk50, unk58, unk60);
		unk14.x2 = position.x;
		unk14.y2 = position.y;
		unk0->resize(unk14.x2 + unk4.getWidth(), unk14.y2 + unk4.getHeight());

		unk30 += unk34;
	}

	bool result = false;
	if (!unk24 && !unk25)
		result = true;

	return result;
}

void TBoundPane::makeNewPosition(f32 param_1, JUTPoint& param_2,
                                 JUTPoint& param_3, JUTPoint& param_4,
                                 JUTPoint& param_5)
{
	f32 square       = param_1 * param_1;
	f32 inverse      = 1.0f - param_1;
	f32 startWeight  = inverse * inverse;
	f32 middleWeight = 2.0f * inverse * param_1;
	f32 x            = param_5.x * square
	        + (param_3.x * startWeight + param_4.x * middleWeight);
	f32 y = param_5.y * square
	        + (param_3.y * startWeight + param_4.y * middleWeight);

	s16 adjustedX = x + (x > 0.0f ? 0.5f : -0.5f);
	s16 adjustedY = y + (y > 0.0f ? 0.5f : -0.5f);
	param_2.set(adjustedX, adjustedY);
}

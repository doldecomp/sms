#include <GC2D/BoundPane.hpp>
#include <JSystem/J2D/J2DScreen.hpp>

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

bool TBoundPane::update()
{
	if (unk24) {
		if (unk28 > 1.0f) {
			unk28 = 1.0f;
			unk24 = false;
		}

		JUTPoint point;
		makeNewPosition(unk28, unk38, unk40, unk48, point);

		unk14.x1 = point.x;
		unk14.y1 = point.y;
		unk0->move(unk4.x1 + unk14.x1, unk4.y1 + unk14.y1);

		unk28 += unk2C;
	}

	if (unk25) {
		if (unk30 > 1.0f) {
			unk30 = 1.0f;
			unk25 = false;
		}

		JUTPoint point;
		makeNewPosition(unk30, unk50, unk58, unk60, point);

		unk14.x2 = point.x;
		unk14.y2 = point.y;
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
	f32 fVar1 = param_1 * param_1;
	f32 fVar2 = 1.0f - param_1;
	f32 fVar4 = 2.0f * fVar2 * param_1;
	f32 fVar3 = fVar2 * fVar2;
	f32 fVar5 = param_4.x * fVar1 + param_2.x * fVar3 + param_3.x * fVar4;
	f32 fVar6 = param_4.y * fVar1 + param_2.y * fVar3 + param_3.y * fVar4;

	fVar5 += fVar5 > 0.0f ? 0.5f : -0.5f;
	fVar6 += fVar6 > 0.0f ? 0.5f : -0.5f;
	param_5.set(fVar5, fVar6);
}

#include <GC2D/Coord2D.hpp>
#include <Camera/cameralib.hpp>

TCoord2D::TCoord2D()
{
	unk0.zero();
	unk8.zero();
	unk10.zero();
}

void TCoord2D::setValue(s32 param_1, f32 param_2, f32 param_3, f32 param_4,
                        f32 param_5)
{
	unk8.set(param_2, param_3);
	unk0.set(param_4, param_5);
	if (param_1 <= 0) {
		unk10.set(0.0f, 0.0f);
	} else {
		unk10.set((param_2 - param_4) / param_1, (param_3 - param_5) / param_1);
	}
}

bool TCoord2D::update()
{
	bool result = false;

	CLBChaseGeneralConstantSpecifySpeed(&unk0.x, unk8.x, unk10.x);
	CLBChaseGeneralConstantSpecifySpeed(&unk0.y, unk8.y, unk10.y);

	if (unk0.x == unk8.x && unk0.y == unk8.y ? true : false) {
		unk10.set(0.0f, 0.0f);
		result = true;
	}

	return result;
}

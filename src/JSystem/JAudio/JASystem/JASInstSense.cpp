#include <JSystem/JAudio/JASystem/JASInstSense.hpp>

namespace JASystem {

f32 TInstSense::getY(int param1, int param2) const
{
	int r6 = 0;
	switch (unk8) {
	case 1:
		r6 = param2;
		break;
	case 2:
		r6 = param1;
		break;
	}
	f32 ret;
	if (unk9 == 0x7f || unk9 == 0)
		ret = unkC + r6 * (unk10 - unkC) / 127.0f;
	else if (r6 < unk9)
		ret = unkC + (1.0f - unkC) * (r6 / (f32)unk9);
	else
		ret = (unk10 - 1.0f) * ((r6 - unk9) / (f32)(0x7f - unk9)) + 1.0f;
	return ret;
}

void TInstSense::setParams(int param1, int param2, f32 param3, f32 param4)
{
	unk8  = param1;
	unk9  = param2;
	unkC  = param3;
	unk10 = param4;
}

} // namespace JASystem

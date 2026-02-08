#include <JSystem/JDrama/JDRViewObj.hpp>

void JDrama::TViewObj::testPerform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (((param_1 & 0x1000) != 0) && unkC.check(0x1000)) {
		param_1 = param_1 & ~0x1;
	}
	if (((param_1 & 0x2000) != 0) && unkC.check(0x2000)) {
		param_1 = param_1 & ~0x1;
	}
	param_1 &= ~unkC.get();
	if (param_1) {
		perform(param_1, param_2);
	}
}

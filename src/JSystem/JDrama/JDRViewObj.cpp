#include <JSystem/JDrama/JDRViewObj.hpp>

void JDrama::TViewObj::testPerform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (((param_1 & 0x1000) != 0) && ((unkC.mValue & 0x1000) != 0)) {
		param_1 = param_1 & ~1;
	}
	if (((param_1 & 0x2000) != 0) && ((unkC.mValue & 0x2000) != 0)) {
		param_1 = param_1 & ~1;
	}
	param_1 &= ~unkC.mValue;
	if (param_1) {
		perform(param_1, param_2);
	}
}

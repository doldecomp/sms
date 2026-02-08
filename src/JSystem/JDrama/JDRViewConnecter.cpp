#include <JSystem/JDrama/JDRViewConnecter.hpp>

using namespace JDrama;

TViewConnecter::TViewConnecter(TViewObj* param_1, TViewObj* param_2,
                               TFlagT<u16> param_3, const char* name)
    : TViewObj(name)
    , unk10(param_1)
    , unk14(param_2)
    , unk18(param_3)
{
}

void TViewConnecter::perform(u32 param_1, TGraphics* param_2)
{
	if (!(param_1 & 8))
		return;

	if (unk10)
		unk10->testPerform(0x80, param_2);

	if (unk14)
		unk14->testPerform(param_1, param_2);

	if (unk10 && unk18.check(0x1))
		unk10->testPerform(8, param_2);
}

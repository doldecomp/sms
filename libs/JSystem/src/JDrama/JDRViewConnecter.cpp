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

void TViewConnecter::perform(u32 cue, TGraphics* graphics)
{
	if (!(cue & CUE_DRAW))
		return;

	if (unk10)
		unk10->testPerform(CUE_DRAW_INIT, graphics);

	if (unk14)
		unk14->testPerform(cue, graphics);

	if (unk10 && unk18.check(0x1))
		unk10->testPerform(CUE_DRAW, graphics);
}

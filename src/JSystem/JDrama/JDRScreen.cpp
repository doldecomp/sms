#include <JSystem/JDrama/JDRScreen.hpp>
#include <JSystem/JDrama/JDRViewport.hpp>

using namespace JDrama;

void TCamConnecter::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (!(param_1 & 0x8))
		return;

	if (unk10)
		unk10->testPerform(0x10, param_2);

	if (unk14)
		unk14->testPerform(param_1, param_2);

	if (unk10 && unk18.check(0x1))
		unk10->testPerform(0x8, param_2);
}

TScreen::TScreen(const TRect& param_1, const char* name)
    : TViewConnecter(nullptr, nullptr, 0, name)
{
	unk10 = new TViewport(param_1, "<Viewport>");
	unk14 = new TCamConnecter;
}

void TScreen::assignCamera(TViewObj* cam)
{
	((TViewConnecter*)unk14)->unk10 = cam;
}

void TScreen::assignViewObj(TViewObj* view_obj)
{
	((TViewConnecter*)unk14)->unk14 = view_obj;
}

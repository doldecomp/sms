#include <JSystem/JDrama/JDRScreen.hpp>
#include <JSystem/JDrama/JDRViewport.hpp>

using namespace JDrama;

void TCamConnecter::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (!(cue & CUE_DRAW))
		return;

	if (unk10)
		unk10->testPerform(CUE_SET_PROJECTION, graphics);

	if (unk14)
		unk14->testPerform(cue, graphics);

	if (unk10 && unk18.check(0x1))
		unk10->testPerform(CUE_DRAW, graphics);
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

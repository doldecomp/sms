#include <JSystem/J2D/J2DScreen.hpp>
#include <dolphin/gx.h>
#include <dolphin/os.h>

J2DScreen::~J2DScreen() { }

void J2DScreen::makeHiearachyPanes(J2DPane*, JSURandomInputStream*, bool, bool,
                                   bool, long*)
{
}

bool J2DScreen::makeUserPane(u16, J2DPane*, JSURandomInputStream*) { }

bool J2DScreen::makeUserPane(u32, J2DPane*, JSURandomInputStream*)
{
	return false;
}

bool J2DScreen::stop()
{
	OSPanic(__FILE__, 311, "There is a unknown pane in SCRN resource\n");
	return false;
}

void J2DScreen::draw(int x, int y, const J2DGrafContext* pCtx) { }

J2DPane* J2DScreen::search(u32 tag)
{
	if (tag == 0)
		return nullptr;
	return J2DPane::search(tag);
}

J2DSetScreen::J2DSetScreen(const char*, JKRArchive*) { }

void J2DScreen::drawSelf(int x, int y, Mtx* mtx)
{
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3s16(0, 0, 0);
	GXColor1u32(mColor);
	GXPosition3s16((u32)getWidth(), 0, 0);
	GXColor1u32(mColor);
	GXPosition3s16((u32)getWidth(), (u32)getHeight(), 0);
	GXColor1u32(mColor);
	GXPosition3s16(0, (u32)getHeight(), 0);
	GXColor1u32(mColor);

	GXEnd();
}

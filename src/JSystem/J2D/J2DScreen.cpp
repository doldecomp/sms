#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>
#include <dolphin/gx.h>
#include <dolphin/os.h>

J2DScreen::~J2DScreen() { }

#pragma dont_inline on
void J2DScreen::makeHiearachyPanes(J2DPane*, JSURandomInputStream*, bool, bool,
                                   bool, long*)
{
}
#pragma dont_inline reset

bool J2DScreen::makeUserPane(u16, J2DPane*, JSURandomInputStream*)
{
	OSPanic(__FILE__, 270, "SCRN resource is broken.\n");
	return false;
}

bool J2DScreen::makeUserPane(u32, J2DPane*, JSURandomInputStream*)
{
	return false;
}

bool J2DScreen::stop()
{
	OSPanic(__FILE__, 311, "There is a unknown pane in SCRN resource\n");
	return false;
}

void J2DScreen::draw(int x, int y, const J2DGrafContext* pCtx)
{
	if (pCtx) {
		J2DGrafContext ctx(*pCtx);
		J2DPane::draw(x, y, pCtx, mbClipToParent);
		ctx.setScissor();
	} else {
		J2DOrthoGraph graph(0, 0, 640, 480);
		graph.setPort();
		J2DPane::draw(x, y, &graph, mbClipToParent);
		graph.setScissor();
	}
	GXSetNumTexGens(0);
	GXSetNumTevStages(1);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetVtxDesc(GX_VA_TEX0, GX_NONE);
	GXSetCullMode(GX_CULL_NONE);
}

J2DPane* J2DScreen::search(u32 tag)
{
	if (tag == 0)
		return nullptr;
	return J2DPane::search(tag);
}

J2DSetScreen::J2DSetScreen(const char* name, JKRArchive* arch)
    : J2DScreen()
{
	void* res = JKRFileLoader::getGlbResource(name, (JKRFileLoader*)arch);
	if (res) {
		u32 sz = JKRFileLoader::getResSize(res, nullptr);
		JSUMemoryInputStream stream;
		stream.setBuffer(res, sz);
		makeHiearachyPanes(this, &stream, false, true, false, nullptr);
	}
	mbClipToParent = false;
}

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

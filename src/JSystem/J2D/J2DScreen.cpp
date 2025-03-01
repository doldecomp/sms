#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <JSystem/J2D/J2DWindow.hpp>
#include <JSystem/J2D/J2DPicture.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <JSystem/JKernel/JKRArchive.hpp>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>
#include <dolphin/gx.h>
#include <dolphin/os.h>

J2DScreen::~J2DScreen() { }

void J2DScreen::makeHiearachyPanes(J2DPane* parent,
                                   JSURandomInputStream* stream,
                                   bool allow_user_panes, bool we_are_root,
                                   bool is_ex, s32* out_next_pane_offset)
{
	s32 nextPaneOffset;
	J2DPane* nextParent = parent;
	if (we_are_root) {
		u32 magic;
		stream->peek(&magic, 4);
		is_ex  = magic == 'SCRN' ? true : false;
		mColor = 0;
		if (is_ex) {
			stream->skip(4);
			u32 magic2 = stream->readU32();
			if (magic2 != 'blo1')
				return;
			stream->skip(0x18);
			u32 magic3 = stream->readU32();
			if (magic3 != 'INF1')
				return;
			stream->skip(4);
			s16 w                = stream->readS16();
			s16 h                = stream->readS16();
			mBounds              = JUTRect(0, 0, w, h);
			mColor               = stream->readU32();
			out_next_pane_offset = &nextPaneOffset;
		}
	}

	while (true) {
		if (is_ex) {
			u32 magic;
			if (stream->read(&magic, 4) != 4) {
				OSPanic("J2DScreen.cpp", 0x91, "SCRN resource is broken.\n");
			}
			u32 size;
			if (stream->peek(&size, 4) != 4) {
				OSPanic("J2DScreen.cpp", 0x96, "SCRN resource is broken.\n");
			}
			stream->skip(-4);
			*out_next_pane_offset = size + stream->getPosition();

			switch (magic) {
			case 'EXT1':
				return;
			case 'BGN1':
				stream->seek(*out_next_pane_offset, JSUStreamSeekFrom_SET);
				makeHiearachyPanes(nextParent, stream, allow_user_panes, false,
				                   is_ex, out_next_pane_offset);
				break;
			case 'END1':
				stream->seek(*out_next_pane_offset, JSUStreamSeekFrom_SET);
				return;
			case 'PAN1':
				nextParent = new J2DPane(parent, stream, is_ex);
				break;
			case 'WIN1':
				nextParent = new J2DWindow(parent, stream, is_ex);
				break;
			case 'PIC1':
				nextParent = new J2DPicture(parent, stream, is_ex);
				break;
			case 'TBX1':
				nextParent = new J2DTextBox(parent, stream, is_ex);
				break;
			default:
				if (allow_user_panes)
					nextParent = makeUserPane(mKind, parent, stream);
				break;
			}
			stream->seek(*out_next_pane_offset, JSUStreamSeekFrom_SET);
		} else {
			u16 tag;
			if (stream->peek(&tag, 2) != 2) {
				OSPanic("J2DScreen.cpp", 199, "SCRN resource is broken.\n");
			}
			switch (tag) {
			case 0:
				return;
			case 1:
				stream->skip(4);
				makeHiearachyPanes(nextParent, stream, allow_user_panes, false,
				                   is_ex, nullptr);
				break;
			case 2:
				stream->skip(4);
				return;
			case 0x10:
				nextParent = new J2DPane(parent, stream, is_ex);

				if (we_are_root)
					mBounds = JUTRect(0, 0, nextParent->getWidth(),
					                  nextParent->getHeight());
				break;
			case 0x11:
				nextParent = new J2DWindow(parent, stream, is_ex);
				break;
			case 0x12:
				nextParent = new J2DPicture(parent, stream, is_ex);
				break;
			case 0x13:
				nextParent = new J2DTextBox(parent, stream, is_ex);
				break;
			default:
				nextParent = allow_user_panes
				                 ? makeUserPane(tag, parent, stream)
				                 : stop();
				break;
			}
		}
	}
}

J2DPane* J2DScreen::makeUserPane(u16, J2DPane*, JSURandomInputStream*)
{
	OSPanic(__FILE__, 270, "There is a unknown pane in SCRN resource\n");
	return nullptr;
}

J2DPane* J2DScreen::makeUserPane(u32, J2DPane*, JSURandomInputStream*)
{
	return nullptr;
}

J2DPane* J2DScreen::stop()
{
	OSPanic(__FILE__, 311, "There is a unknown pane in SCRN resource\n");
	return nullptr;
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
	u8* res = (u8*)JKRGetNameResource(name, arch);
	if (res) {
		u32 sz = JKRFileLoader::getResSize(res, nullptr);
		JSUMemoryInputStream stream(res, sz);
		makeHiearachyPanes(this, &stream, false, true, false, nullptr);
	}
	mbClipToParent = false;
}

void J2DScreen::drawSelf(int x, int y, Mtx* mtx)
{
	int trash[0x4]; // ???

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

static void dummy(J2DSetScreen* s) { s->~J2DSetScreen(); }

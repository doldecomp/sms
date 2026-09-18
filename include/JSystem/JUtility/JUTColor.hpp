#ifndef JUT_COLOR_HPP
#define JUT_COLOR_HPP

#include "dolphin/gx/GXStruct.h"

namespace JUtility {

struct TColor : public GXColor {
	TColor(u8 r, u8 g, u8 b, u8 a) { set(r, g, b, a); }
	TColor() { set(0xffffffff); }
	TColor(u32 u32Color) { set(u32Color); }
	// TODO: by value, and that costs a dead stack slot at every
	// `GXSetChanMatColor(ch, TColor(c))` site. Taking `const GXColor&` here
	// does reclaim 24 bytes of TMBindShadowManager::drawShadow's frame
	// (0x128 -> 0x110 against the target's 0xf8, so still 24 over and the
	// score does not move), but it is wrong everywhere else:
	//
	//   parameter                 | JSGSetColor | ElasticCode::draw | others
	//   --------------------------+-------------+-------------------+--------
	//   GXColor (this)            |      100.0  |             100.0 |   --
	//   const GXColor& on the ctor|       99.43 |             99.93 | drawDVDErr
	//                             |             |                   | 98.51 ->
	//                             |             |                   | 97.49,
	//                             |             |                   | Cogwheel/
	//                             |             |                   | WireBell
	//                             |             |                   | initDraw
	//                             |             |                   | 99.96 ->
	//                             |             |                   | 99.93
	//   const GXColor& on set()   |       99.43 |             99.93 | same two
	//                             |             |                   | initDraws
	//
	// So both halves of the copy are load-bearing for the units that already
	// match, and drawShadow's remaining 24 bytes are not this. Left by value.
	TColor(GXColor color) { set(color); }

	// Kept commented out. The copy has to stay implicit/trivial: writing it
	// out explicitly costs JUTFont, J2DPicture, J2DPrint, J2DTextBox,
	// J2DWindow, J2DGrafContext, JUTConsole, JDREfbCtrl and JDRLighting
	// their exact functions (nine units off 100%, DOL broken) and does not
	// move TMenuPlane::perform at all.
	// TColor(const TColor& other) { *(GXColor*)this = *(GXColor*)&other; }
	//
	// Open: the `x = c.get()` conversion temporary is 4 bytes wide for retail
	// and 8 for us. TMenuPlane::perform (GC2D/Menu) holds four of them at an
	// 8-byte stride (0x78/0x80/0x88/0x90, frame 0x98) where retail uses a
	// 4-byte stride (0x64/0x68/0x6c/0x70, frame 0x78), and TMenuPlane's ctor
	// has three more. Everything else in both functions is instruction-exact,
	// so the whole gap is the stride. Measured and rejected in round 14, all
	// whole-tree with the DOL checked:
	//   - explicit copy ctor (above): nine units regress, Menu unmoved.
	//   - `TColor& operator=(const GXColor&)`: Menu 99.5 -> 85.3 and
	//     J2DPrint/J2DScreen/J2DTextBox/J2DWindow/JUTConsole/JDRFrmGXSet/
	//     JDRLighting regress -- the sites really do want the TColor
	//     conversion, not a GXColor assignment.
	//   - `TColor get() const` instead of `GXColor get() const`: Menu
	//     unmoved, J2DPicture::setTevMode and TMenuPlane's ctor regress.
	//   - at the call site, `mCharColor = unk24` (no `.get()`) removes the
	//     temporary entirely and leaves perform a leaf function (85.3%), and
	//     `mCharColor.set(unk24.get())` is also 85.3%.
	// So the temporary is the get()-plus-conversion chain in both builds and
	// only its width differs; nothing spellable in this class changes that.

	operator u32() const { return toUInt32(); }
	u32 toUInt32() const { return *(u32*)&r; }

	void set(u8 cR, u8 cG, u8 cB, u8 cA)
	{
		r = cR;
		g = cG;
		b = cB;
		a = cA;
	}

	void set(u32 u32Color) { *(u32*)&r = u32Color; }
	void set(GXColor gxColor) { *(GXColor*)&r = gxColor; }
	GXColor get() const { return *this; }
};

} // namespace JUtility

#endif

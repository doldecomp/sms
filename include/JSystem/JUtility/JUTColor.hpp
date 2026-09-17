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

	// TColor(const TColor& other) { *(GXColor*)this = *(GXColor*)&other; }

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

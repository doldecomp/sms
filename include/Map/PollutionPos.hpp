#ifndef MAP_POLLUTION_POS_HPP
#define MAP_POLLUTION_POS_HPP

#include <dolphin/types.h>

class TPollutionLayer;

class TPollutionPos {
public:
	TPollutionPos();
	void init(TPollutionLayer*, f32, f32, u8*, int, int);
	int worldToTexSize(f32) const;
	int worldToDepth(f32) const;
	bool isProhibit(int, int) const;
	void subtractFromYMap(int, int, f32) const;
	bool isSame(int, int, f32) const;
	f32 getDepthWorld(int, int) const;
	int getEdgeDegree(int, int) const;

	// fabricated
	u32 index(int x, int y) const
	{
		// clang-format off
    return
        (y & 3U) * 8
      + ((x >> 3) + ((y >> 2) << (unk8 - 3))) * 0x20
      + (x & 7U);
		// clang-format on
	}
	int getDepth(int x, int y) const { return mMap[index(x, y)]; }
	void setDepth(int x, int y, int v) const { mMap[index(x, y)] = v; }

public:
	/* 0x0 */ int mWidth;
	/* 0x4 */ int mHeight;
	/* 0x8 */ u32 unk8;
	/* 0xC */ u32 unkC;
	/* 0x10 */ f32 mVerticalOffset;
	/* 0x14 */ f32 mVerticalScale;
	/* 0x18 */ f32 mInverseVerticalScale;
	/* 0x1C */ u8* mMap;
	/* 0x20 */ TPollutionLayer* mOwner;
};

#endif

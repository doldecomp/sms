#ifndef MAP_POLLUTION_POS_HPP
#define MAP_POLLUTION_POS_HPP

#include <dolphin/types.h>

class TPollutionLayer;

/**
 * @brief Describes how a pollution texture is positioned in a layer.
 * @details Contains the information required for mapping from a layer's
 * object-space coordinates to texture coordinate space of the pollution
 * texture, as well as a heightmap for the region of the world covered by the
 * layer.
 */
class TPollutionPos {
public:
	TPollutionPos();
	void init(TPollutionLayer* owner, f32 vertical_offset, f32 texel_size,
	          u8* depth_map, int log2_width, int log2_height);
	int worldToTexSize(f32) const;
	int worldToDepth(f32) const;
	bool isProhibit(int, int) const;
	void subtractFromYMap(int, int, f32) const;
	bool isSame(int, int, f32) const;
	f32 getDepthWorld(int, int) const;
	int getEdgeDegree(int, int) const;

	// fabricated
	u32 index(int x, int z) const
	{
		// clang-format off
    return
        (z & 3U) * 8
      + ((x >> 3) + ((z >> 2) << (mLog2Width - 3))) * 0x20
      + (x & 7U);
		// clang-format on
	}
	int getDepth(int x, int z) const { return mHeightMap[index(x, z)]; }
	void setDepth(int x, int z, int v) const { mHeightMap[index(x, z)] = v; }

	bool isInArea(int x, int z) const
	{
		if (x < 0 || mWidth <= x || z < 0 || mHeight <= z)
			return false;
		return true;
	}

	int getWidth() const { return mWidth; }
	int getHeight() const { return mHeight; }
	f32 getTexelSize() const { return mTexelSize; }
	f32 texToWorldSize(int v) const { return v * mTexelSize; }

public:
	/* 0x0 */ int mWidth;
	/* 0x4 */ int mHeight;
	/* 0x8 */ int mLog2Width;
	/* 0xC */ int mLog2Height;
	/* 0x10 */ f32 mVerticalOffset;
	/* 0x14 */ f32 mTexelSize;
	/* 0x18 */ f32 mInverseTexelScale;
	/* 0x1C */ u8* mHeightMap;
	/* 0x20 */ TPollutionLayer* mOwner;
};

#endif

#ifndef MAP_MAP_WARP_HPP
#define MAP_MAP_WARP_HPP

#include <JSystem/JGeometry.hpp>

class JSUMemoryInputStream;

class TMapWarp {
public:
	TMapWarp();

	class TMapWarpInfo {
	public:
		TMapWarpInfo() { }

	public:
		/* 0x0 */ u32 mDestinationModelIndex;
		/* 0x4 */ u32 mSourceModelIndex;
		/* 0x8 */ JGeometry::TVec3<f32> mWarpOffset;
	};

	void init(JSUMemoryInputStream&);
	void initModel();
	void watchToWarp();
	void warp(int index);
	void changeModel(int);

public:
	/* 0x0 */ int mWarpPairCount;
	/* 0x4 */ TMapWarpInfo* mWarpInfos;
	/* 0x8 */ int mCurrentModelIndex;
	/* 0xC */ f32 unkC;
};

#endif
